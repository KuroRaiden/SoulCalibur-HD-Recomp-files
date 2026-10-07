// SPDX-License-Identifier: BSD-3-Clause
#include "generated/default/soulcalibur_hd_init.h"
DECLARE_REX_FUNC(QueryUserSigninState);
DECLARE_REX_FUNC(QueryUserSigninInfo);
DECLARE_REX_FUNC(RequireSignedInMenuUser);
DECLARE_REX_FUNC(HandleFrontMenuSelection);

// The frozen runtime provides one local profile, only at user zero. Offline
// controller slots share that profile; input state and active-player indexes
// are not rewritten. Invalid user indexes retain the original error behavior.
REX_EXTERN(QueryUserSigninState) {
  const auto user=ctx.r3.u32;
  if(user<4)ctx.r3.u32=0;
  __imp__QueryUserSigninState(ctx,base);
}
REX_EXTERN(QueryUserSigninInfo) {
  if(ctx.r3.u32<4)ctx.r3.u32=0;
  __imp__QueryUserSigninInfo(ctx,base);
}
REX_EXTERN(RequireSignedInMenuUser) {
  const auto user=ctx.r3.u32;
  __imp__RequireSignedInMenuUser(ctx,base);
  REXLOG_INFO("Front menu sign-in gate: controller user={} accepted={}",user,ctx.r3.u32);
}
REX_EXTERN(HandleFrontMenuSelection) {
  REXLOG_INFO("Front menu selection: controller user={}",REX_LOAD_U8(ctx.r5.u32));
  __imp__HandleFrontMenuSelection(ctx,base);
}
