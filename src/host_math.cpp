// SPDX-License-Identifier: BSD-3-Clause
#include <cmath>
// C23 roundevenf is absent from the Windows CRT used by this toolchain.
// This operation always rounds ties to even, independent of the host FP mode.
extern "C" float roundevenf(float value) {
  if (!std::isfinite(value) || std::fabs(value) >= 8388608.0f) return value;
  const float magnitude = std::fabs(value);
  float integral = std::floor(magnitude);
  const float fraction = magnitude - integral;
  if (fraction > 0.5f || (fraction == 0.5f && std::fmod(integral, 2.0f) != 0.0f))
    integral += 1.0f;
  return std::copysign(integral, value);
}
