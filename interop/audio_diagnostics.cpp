// SPDX-License-Identifier: BSD-3-Clause
#include "generated/default/soulcalibur_hd_init.h"
#include <atomic>
#include <bit>
#include <cmath>
DECLARE_REX_FUNC(SetAudioRenderCallback);
DECLARE_REX_FUNC(SubmitAudioRenderFrame);
REX_EXTERN(SetAudioRenderCallback) {
  const auto object=ctx.r3.u32,callback=ctx.r4.u32;
  REXLOG_INFO("Audio render callback setup: object={:08X} callback={:08X}",object,callback);
  __imp__SetAudioRenderCallback(ctx,base);
  REXLOG_INFO("Audio render callback setup returned: result={:08X} driver={:08X}",ctx.r3.u32,REX_LOAD_U32(object+24));
}
REX_EXTERN(SubmitAudioRenderFrame) {
  static std::atomic<unsigned> count=0;
  const unsigned frame=++count;
  if(frame<=3 || frame%500==0) {
    const auto descriptor=ctx.r4.u32;
    const auto samples=REX_LOAD_U32(descriptor+20);
    float peak=0.0f;
    if(samples) for(unsigned i=0;i<256*6;++i) {
      const float value=std::bit_cast<float>(REX_LOAD_U32(samples+i*4));
      if(std::isfinite(value)) peak=std::max(peak,std::abs(value));
    }
    REXLOG_INFO("Audio guest frame #{} descriptor={:08X} PCM={:08X} peak={:.6f}",frame,descriptor,samples,peak);
  }
  __imp__SubmitAudioRenderFrame(ctx,base);
}
