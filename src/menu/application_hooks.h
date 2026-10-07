// SPDX-License-Identifier: BSD-3-Clause
#pragma once
#include <functional>
namespace soulcalibur {
struct OnlineReadiness {
  bool offline_versus_verified = false;
  bool deterministic_restore_verified = false;
};
class MenuApplicationHooks {
 public:
  OnlineReadiness readiness;
  std::function<void()> open_online_menu;
  bool OnlineAvailable() const {
    return readiness.offline_versus_verified &&
           readiness.deterministic_restore_verified && bool(open_online_menu);
  }
  bool RequestOnline() const {
    if (!OnlineAvailable()) return false;
    open_online_menu();
    return true;
  }
};
}
