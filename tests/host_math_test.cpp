// SPDX-License-Identifier: BSD-3-Clause
#include <cmath>
#include <cfenv>
#include <limits>
#include <cstdio>
#include <initializer_list>
extern "C" float roundevenf(float);
int main() {
  const float input[] = {0.5f, 1.5f, 2.5f, 3.5f, -0.5f, -1.5f, -2.5f, -3.5f, 0.49f, 0.51f, 8388607.5f, 8388608.0f};
  const float expected[] = {0, 2, 2, 4, -0.0f, -2, -2, -4, 0, 1, 8388608, 8388608};
  for (int mode : {FE_TONEAREST, FE_DOWNWARD, FE_UPWARD, FE_TOWARDZERO}) {
    std::fesetround(mode);
    for (unsigned i = 0; i < sizeof(input)/sizeof(input[0]); ++i) {
      const float result = roundevenf(input[i]);
      if (result != expected[i] || std::signbit(result) != std::signbit(expected[i])) return 1;
    }
  }
  if (!std::isinf(roundevenf(std::numeric_limits<float>::infinity()))) return 2;
  if (!std::isnan(roundevenf(std::numeric_limits<float>::quiet_NaN()))) return 3;
  std::puts("Ties-to-even math passed under all four host rounding modes.");
}
