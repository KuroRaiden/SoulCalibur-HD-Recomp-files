// SPDX-License-Identifier: BSD-3-Clause
#include "generated/default/soulcalibur_hd_init.h"
#include <rex/runtime.h>
#include <rex/input/input_system.h>
#include <rex/ui/virtual_key.h>
#include <array>
#include <deque>
#include <mutex>

// The pinned SDK pins ANY_USER keystroke queries to slot zero and its
// keyboard driver does not enqueue controller keystrokes. Translate real
// controller state edges here, at the game's menu-input boundary.
REX_EXTERN(ReadMenuControllerKeystroke) {
  using namespace rex::input;
  using rex::X_RESULT;
  using V = rex::ui::VirtualKey;
  struct Binding { uint16_t button; V key; };
  static constexpr Binding bindings[] = {
    {X_INPUT_GAMEPAD_A,V::kXInputPadA}, {X_INPUT_GAMEPAD_B,V::kXInputPadB},
    {X_INPUT_GAMEPAD_X,V::kXInputPadX}, {X_INPUT_GAMEPAD_Y,V::kXInputPadY},
    {X_INPUT_GAMEPAD_DPAD_UP,V::kXInputPadDpadUp},
    {X_INPUT_GAMEPAD_DPAD_DOWN,V::kXInputPadDpadDown},
    {X_INPUT_GAMEPAD_DPAD_LEFT,V::kXInputPadDpadLeft},
    {X_INPUT_GAMEPAD_DPAD_RIGHT,V::kXInputPadDpadRight},
    {X_INPUT_GAMEPAD_START,V::kXInputPadStart},
    {X_INPUT_GAMEPAD_BACK,V::kXInputPadBack}
  };
  static std::mutex mutex;
  static std::array<uint16_t,4> previous{};
  static std::deque<X_INPUT_KEYSTROKE> pending;
  std::lock_guard lock(mutex);
  auto* input = dynamic_cast<InputSystem*>(rex::Runtime::instance()->input_system());
  if (!input || !ctx.r5.u32) { ctx.r3.u32 = X_ERROR_BAD_ARGUMENTS; return; }
  const auto requested_user = ctx.r3.u32;
  const bool any_user = requested_user == 0xFF || (ctx.r4.u32 & 0x40000000);
  for (uint32_t user=0; user<4; ++user) {
    X_INPUT_STATE state{};
    uint16_t current = 0;
    if (input->GetState(user,&state) == X_ERROR_SUCCESS) {
      current = state.gamepad.buttons;
      if (state.gamepad.thumb_lx > 16000) current |= X_INPUT_GAMEPAD_DPAD_RIGHT;
      if (state.gamepad.thumb_lx < -16000) current |= X_INPUT_GAMEPAD_DPAD_LEFT;
      if (state.gamepad.thumb_ly > 16000) current |= X_INPUT_GAMEPAD_DPAD_UP;
      if (state.gamepad.thumb_ly < -16000) current |= X_INPUT_GAMEPAD_DPAD_DOWN;
    }
    for (const auto& binding: bindings) {
      if ((current ^ previous[user]) & binding.button) {
        X_INPUT_KEYSTROKE event{};
        event.virtual_key = static_cast<uint16_t>(binding.key);
        event.flags = current & binding.button ? X_INPUT_KEYSTROKE_KEYDOWN : X_INPUT_KEYSTROKE_KEYUP;
        event.user_index = static_cast<uint8_t>(user);
        if (pending.size() >= 128) pending.pop_front();
        pending.push_back(event);
      }
    }
    previous[user]=current;
  }
  for (auto it=pending.begin(); it!=pending.end(); ++it) {
    if (any_user || it->user_index == requested_user) {
      auto event=*it;
      std::memcpy(base+ctx.r5.u32,&event,sizeof(event));
      pending.erase(it);
      REXLOG_INFO("Menu controller event: user={} key={:04X} flags={}",event.user_index,static_cast<uint16_t>(event.virtual_key),static_cast<uint16_t>(event.flags));
      ctx.r3.u32=X_ERROR_SUCCESS;
      return;
    }
  }
  ctx.r3.u32=X_ERROR_EMPTY;
}
