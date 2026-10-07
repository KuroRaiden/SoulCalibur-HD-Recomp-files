// SPDX-License-Identifier: BSD-3-Clause
#pragma once
#include <cstdint>
namespace soulcalibur {
struct NativeScene;
// Game-specific scene interface. Guest layout decoding belongs in interop.
class SceneRenderer {
 public:
  virtual ~SceneRenderer() = default;
  virtual void BeginFrame(std::uint64_t frame_number) = 0;
  virtual void SubmitScene(const NativeScene& scene) = 0;
  virtual void EndFrame() = 0;
};
enum class RendererPath { XenosReference, NativeSdlGpu };
}
