// SPDX-License-Identifier: BSD-3-Clause
#pragma once
#include <rex/input/input_driver.h>
#include <rex/ui/window_listener.h>
#include <rex/ui/virtual_key.h>
#include <array>
#include <chrono>
#include <mutex>

namespace soulcalibur {
// Keep brief key taps visible for one guest frame. SDL gamepads continue
// to be merged by InputSystem; this driver supplies two local keyboard players.
class KeyboardController final : public rex::input::InputDriver,
                                 public rex::ui::WindowInputListener,
                                 public rex::ui::WindowListener {
  using Clock=std::chrono::steady_clock;
  std::mutex mutex_;
  std::array<bool,256> keys_down_{};
  std::array<Clock::time_point,32> visible_until_{};
  rex::ui::Window* attached_=nullptr;
  uint32_t packet_=0;
  static unsigned User(rex::ui::VirtualKey key) {
    using V=rex::ui::VirtualKey;
    switch(key) { case V::kI:case V::kJ:case V::kK:case V::kL:case V::kU:case V::kH:case V::kO:case V::kP:case V::kReturn:return 1;default:return 0; }
  }
  static uint16_t Button(rex::ui::VirtualKey key) {
    using V=rex::ui::VirtualKey;
    using namespace rex::input;
    switch(key) {
      case V::kU:case V::kSpace:return X_INPUT_GAMEPAD_A;
      case V::kH:case V::kShift:return X_INPUT_GAMEPAD_B;
      case V::kO:case V::kR:return X_INPUT_GAMEPAD_X;
      case V::kP:case V::kE:return X_INPUT_GAMEPAD_Y;
      case V::kI:case V::kUp:case V::kW:return X_INPUT_GAMEPAD_DPAD_UP;
      case V::kK:case V::kDown:case V::kS:return X_INPUT_GAMEPAD_DPAD_DOWN;
      case V::kJ:case V::kLeft:case V::kA:return X_INPUT_GAMEPAD_DPAD_LEFT;
      case V::kL:case V::kRight:case V::kD:return X_INPUT_GAMEPAD_DPAD_RIGHT;
      case V::kReturn:case V::kEscape:return X_INPUT_GAMEPAD_START;
      case V::kTab:return X_INPUT_GAMEPAD_BACK;
      default:return 0;
    }
  }
 public:
  explicit KeyboardController(rex::ui::Window* window):InputDriver(window,0){}
  ~KeyboardController() override { if(attached_) { attached_->RemoveInputListener(this); attached_->RemoveListener(this); } }
  rex::X_STATUS Setup() override { return 0; }
  void OnWindowAvailable(rex::ui::Window* window) override {
    attached_=window; window->AddInputListener(this,0); window->AddListener(this);
  }
  void OnClosing(rex::ui::UIEvent&) override { attached_=nullptr; }
  void OnLostFocus(rex::ui::UISetupEvent&) override {
    std::lock_guard lock(mutex_); keys_down_.fill(false); visible_until_.fill({}); ++packet_;
  }
  void OnKeyDown(rex::ui::KeyEvent& event) override {
    const auto button=Button(event.virtual_key());
    if(!button)return;
    std::lock_guard lock(mutex_);
    for(unsigned bit=0;bit<16;++bit)if(button&(1u<<bit)) {
      keys_down_[static_cast<unsigned>(event.virtual_key())]=true; visible_until_[bit+16*User(event.virtual_key())]=Clock::now()+std::chrono::milliseconds(80);
    }
    ++packet_;

  }
  void OnKeyUp(rex::ui::KeyEvent& event) override {
    const auto button=Button(event.virtual_key());
    std::lock_guard lock(mutex_);
    if(button) keys_down_[static_cast<unsigned>(event.virtual_key())]=false;
    ++packet_;
  }
  rex::X_RESULT GetState(uint32_t user,rex::input::X_INPUT_STATE* state) override {
    if(user>1)return 0x48F;
    std::lock_guard lock(mutex_);
    *state={}; state->packet_number=packet_;
    uint16_t buttons=0;
    for(unsigned key=0;key<keys_down_.size();++key)if(keys_down_[key] && User(static_cast<rex::ui::VirtualKey>(key))==user) buttons|=Button(static_cast<rex::ui::VirtualKey>(key));
    for(unsigned bit=0;bit<16;++bit)if(Clock::now()<visible_until_[bit+16*user])buttons|=static_cast<uint16_t>(1u<<bit);
    state->gamepad.buttons=buttons; return 0;
  }
  rex::X_RESULT GetCapabilities(uint32_t user,uint32_t,rex::input::X_INPUT_CAPABILITIES* caps) override {
    if(user>1)return 0x48F;
    *caps={};caps->type=1;caps->sub_type=1;caps->gamepad.buttons=0xFFFF;return 0;
  }
  rex::X_RESULT SetState(uint32_t user,rex::input::X_INPUT_VIBRATION*) override {return user<2?0:0x48F;}
  rex::X_RESULT GetKeystroke(uint32_t,uint32_t,rex::input::X_INPUT_KEYSTROKE*) override {return 0x10D2;}
};
}
