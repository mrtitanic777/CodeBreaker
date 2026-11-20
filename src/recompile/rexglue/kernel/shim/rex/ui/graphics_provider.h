// graphics_provider.h - MINIMAL SHIM for the ReXGlue/Xenia DXBC translator.
//
// The full rex/ui/graphics_provider.h drags in the entire windowing / imgui /
// renderdoc UI layer (~40 files). The DXBC shader translator only needs ONE
// thing from it: the GpuVendorID enum (for an Intel shader-compiler workaround).
// This shim provides exactly that so the translator vendors without the UI
// stack. Values/enumerators match the upstream Xenia header verbatim.
//
// Upstream: Xenia (Copyright Ben Vanik, BSD-3), adapted for ReXGlue by Tom Clay.
#pragma once
#include <cstdint>

namespace rex {
namespace ui {

class GraphicsProvider {
 public:
  enum class GpuVendorID {
    kAMD = 0x1002,
    kApple = 0x106B,
    kArm = 0x13B5,
    kImagination = 0x1010,
    kIntel = 0x8086,
    kMicrosoft = 0x1414,
    kNvidia = 0x10DE,
    kQualcomm = 0x5143,
  };
};

}  // namespace ui
}  // namespace rex
