// app/window_other.cpp -- the window on systems that have none yet (see window.hpp).
#ifndef _WIN32
#include "app/window.hpp"

#include "core/log.hpp"

namespace cb::app::window {

bool available() { return false; }

int run(std::span<const std::string>, bool) {
    err_text("codebreaker: the window is Windows-only for now; use `codebreaker convert FILE` on this system\n");
    return 2;
}

}  // namespace cb::app::window
#endif
