// renderdoc_api.h - MINIMAL SHIM for the ReXGlue/Xenia DXBC translator.
//
// The translator references ui::RenderDocAPI::CreateIfConnected() only to detect
// a live RenderDoc capture session (a debugging aid). Outside a full UI build
// there is none, so this shim returns nullptr. Signature matches upstream.
//
// Upstream: Xenia (Copyright Ben Vanik, BSD-3), adapted for ReXGlue by Tom Clay.
#pragma once
#include <memory>

namespace rex {
namespace ui {

class RenderDocAPI {
 public:
  static std::unique_ptr<RenderDocAPI> CreateIfConnected();  // shim: nullptr
  RenderDocAPI() = default;
  RenderDocAPI(const RenderDocAPI&) = delete;
  RenderDocAPI& operator=(const RenderDocAPI&) = delete;
};

}  // namespace ui
}  // namespace rex
