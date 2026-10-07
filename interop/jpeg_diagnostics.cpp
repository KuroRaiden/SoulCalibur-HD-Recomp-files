// SPDX-License-Identifier: BSD-3-Clause
#include "generated/default/soulcalibur_hd_init.h"
DECLARE_REX_FUNC(JpegReadMarkers);
DECLARE_REX_FUNC(JpegFindFirstMarker);
DECLARE_REX_FUNC(JpegFindNextMarker);
REX_EXTERN(JpegReadMarkers) {
  REXLOG_INFO("JPEG read markers: object={:08X} caller={:08X}", ctx.r3.u32, static_cast<uint32_t>(ctx.lr));
  __imp__JpegReadMarkers(ctx, base);
}
REX_EXTERN(JpegFindFirstMarker) {
  REXLOG_INFO("JPEG first marker: object={:08X} saved_object={:08X} stack={:08X}", ctx.r3.u32, ctx.r31.u32, ctx.r1.u32);
  const auto object = ctx.r3.u32;
  const auto source = REX_LOAD_U32(object + 24);
  const auto error = REX_LOAD_U32(object);
  const auto bytes = REX_LOAD_U32(source);
  const auto count = REX_LOAD_U32(source + 4);
  REXLOG_INFO("JPEG source={:08X} bytes={:08X} count={} refill={:08X} error_exit={:08X}", source, bytes, count, REX_LOAD_U32(source + 12), REX_LOAD_U32(error));
  if (count >= 2) REXLOG_INFO("JPEG initial bytes {:02X} {:02X}", REX_LOAD_U8(bytes), REX_LOAD_U8(bytes + 1));
  __imp__JpegFindFirstMarker(ctx, base);
  REXLOG_INFO("JPEG first marker returned: result={} restored_object={:08X} stack={:08X}", ctx.r3.u32, ctx.r31.u32, ctx.r1.u32);
}
REX_EXTERN(JpegFindNextMarker) {
  REXLOG_INFO("JPEG next marker: object={:08X} saved_object={:08X} stack={:08X}", ctx.r3.u32, ctx.r31.u32, ctx.r1.u32);
  __imp__JpegFindNextMarker(ctx, base);
  REXLOG_INFO("JPEG next marker returned: result={} restored_object={:08X} stack={:08X}", ctx.r3.u32, ctx.r31.u32, ctx.r1.u32);
}
