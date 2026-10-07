// SPDX-License-Identifier: BSD-3-Clause
// Soulcalibur HD application entry and process-local audio selection.
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif
#include "generated/default/soulcalibur_hd_init.h"
#include "soulcalibur_hd_app.h"

// Configure before main: the SDK creates its SDL context before asking for
// the application factory, and SDL caches its environment at initialization.
[[maybe_unused]] static const bool audio_environment_configured = [] {
#ifdef _WIN32
  // The pinned SDL default backend stalls after losing focus on this system.
  // Set the process environment before SDK SDL initialization. The SDK and
  // application have separate SDL instances, so an application SDL hint alone
  // does not configure the runtime audio driver. Preserve explicit overrides.
  if (GetEnvironmentVariableW(L"SDL_AUDIO_DRIVER", nullptr, 0) == 0)
    SetEnvironmentVariableW(L"SDL_AUDIO_DRIVER", L"directsound");
#endif
  return true;
}();

REX_DEFINE_APP(soulcalibur_hd, SoulcaliburHdApp::Create)
