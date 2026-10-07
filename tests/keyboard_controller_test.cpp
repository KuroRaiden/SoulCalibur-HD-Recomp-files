// SPDX-License-Identifier: BSD-3-Clause
#include "keyboard_controller.h"
#include <rex/ui/ui_event.h>
#include <thread>
#include <cstdlib>
using V=rex::ui::VirtualKey;
using namespace rex::input;
static void Check(bool value) { if(!value) std::abort(); }
static rex::ui::KeyEvent Key(V key) { return rex::ui::KeyEvent(nullptr,key,1,false,false,false,false,false); }
int main() {
  soulcalibur::KeyboardController driver(nullptr);
  X_INPUT_STATE first{},second{};
  auto confirm=Key(V::kSpace); driver.OnKeyDown(confirm); driver.OnKeyUp(confirm);
  driver.GetState(0,&first); driver.GetState(1,&second);
  Check(first.gamepad.buttons & X_INPUT_GAMEPAD_A);
  Check(second.gamepad.buttons == 0);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  driver.GetState(0,&first); Check(first.gamepad.buttons == 0);
  auto up=Key(V::kW),alias=Key(V::kUp); driver.OnKeyDown(up); driver.OnKeyDown(alias); driver.OnKeyUp(up);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  driver.GetState(0,&first); Check(first.gamepad.buttons & X_INPUT_GAMEPAD_DPAD_UP);
  auto other=Key(V::kO); driver.OnKeyDown(other); driver.GetState(1,&second);
  Check(second.gamepad.buttons & X_INPUT_GAMEPAD_X);
  driver.GetState(0,&first); Check(!(first.gamepad.buttons & X_INPUT_GAMEPAD_X));
  rex::ui::UISetupEvent lost; driver.OnLostFocus(lost);
  driver.GetState(0,&first); driver.GetState(1,&second);
  Check(first.gamepad.buttons == 0 && second.gamepad.buttons == 0);
  Check(driver.GetState(2,&first)==0x48F);
}
