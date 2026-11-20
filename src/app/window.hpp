// app/window.hpp -- the CodeBreaker window: `codebreaker` with no command, and `codebreaker app`.
//
//     codebreaker                                      the window (drop a file, get its source code)
//     codebreaker app                                  the same
//     codebreaker app --screenshot OUT.png [--state idle|working|done|unsupported|failed|language|translating|tools|settings|recent|batch|preview|dragout] [--theme light|dark] [--size WxH]
//                                                      render the real window to a PNG instead of showing it (review, and the gate's UI test)
//
// The window is Windows 11 native: plain Win32 with Direct2D and DirectWrite, the DWM caption and Mica backdrop, the system accent colour and
// theme (window_win.cpp and its parts). It runs the conversion pipeline of convert.hpp on a worker thread, one file after another when several are
// dropped (batch_queue.hpp); the Source card previews the output folder (source_preview.hpp), the Program card lists recent conversions (recent.hpp), the
// output folder can be dragged out, and the taskbar button and a notification (notify.hpp) follow a run (window_shell_win.hpp). On other systems there is no window
// yet: `run` says so and returns 2.
#pragma once

#include <span>
#include <string>

namespace cb::app::window {

// Whether this build of the program has a window.
bool available();

// The words after `app` (none: the window). The exit code. Started with no command from an Explorer shortcut (the only process on its console)
// it hides its console and runs the window; started with no command from a terminal it starts a detached copy of itself and returns at once.
int run(std::span<const std::string> args, bool no_command_given);

}  // namespace cb::app::window
