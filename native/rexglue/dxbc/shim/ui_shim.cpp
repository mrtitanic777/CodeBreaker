// ui_shim.cpp - shim implementations for the rex/ui symbols the DXBC translator
// references but which live in the (unvendored) UI layer. See the shim headers.
#include <rex/ui/renderdoc_api.h>

namespace rex {
namespace ui {

// No UI build -> no live RenderDoc capture session.
std::unique_ptr<RenderDocAPI> RenderDocAPI::CreateIfConnected() { return nullptr; }

}  // namespace ui
}  // namespace rex
