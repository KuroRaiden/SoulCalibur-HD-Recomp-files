// SPDX-License-Identifier: BSD-3-Clause
#include "generated/default/soulcalibur_hd_init.h"
#include <chrono>
DECLARE_REX_FUNC(D3DPresentViaVdSwap);
REX_EXTERN(D3DPresentViaVdSwap) {
  using Clock=std::chrono::steady_clock;
  static auto start=Clock::now();
  static unsigned frames=0;
  __imp__D3DPresentViaVdSwap(ctx,base);
  if(++frames==300) {
    auto now=Clock::now();
    const double seconds=std::chrono::duration<double>(now-start).count();
    REXLOG_INFO("Guest frame pacing: {:.1f} FPS over {} frames ({:.2f} seconds)",frames/seconds,frames,seconds);
    start=now; frames=0;
  }
}
