// app/window_shell_win.hpp -- the parts of the CodeBreaker window that talk to the Windows shell: the taskbar button's progress, the notification when a
// conversion finishes in the background, and the folder dragged out of the Source card. (Windows only; the words of the notification are notify.hpp.)
#pragma once
#ifdef _WIN32

#include <windows.h>

#include <filesystem>
#include <memory>

#include "app/notify.hpp"

namespace cb::app::window {

// The taskbar button's progress (ITaskbarList3): none, a moving bar, a bar at a percentage, or the red bar of a failure.
class Taskbar {
public:
    enum class State { None, Indeterminate, Normal, Error };
    Taskbar() = default;
    ~Taskbar();
    Taskbar(const Taskbar&) = delete;
    Taskbar& operator=(const Taskbar&) = delete;
    // The window's taskbar button exists (WM_TASKBARBUTTONCREATED): from here the state is shown; what was asked before is applied now.
    void attach(HWND window);
    void set(State state, int percent = 0);
    State state() const { return state_; }

private:
    void apply();
    HWND window_ = nullptr;
    void* list_ = nullptr;   // ITaskbarList3*
    State state_ = State::None;
    int percent_ = 0;
};

// Tell Windows this process is "CodeBreaker" (an explicit AppUserModelID, so its toasts and its taskbar button are its own).
void set_app_identity();

// The notification of a finished conversion. Windows' own toast (an AppUserModelID registered under HKCU with the name "CodeBreaker", buttons Open folder and
// Show, in-process activation); where that is not available (MinGW's headers lack WinRT, or the toast cannot be made) a balloon from the notification
// area. Either way a click or a button posts `activation_message` to the window (wParam 1: open the folder, 2: show the window); the balloon's own
// callbacks come as `tray_message` and go to on_tray. Nothing is shown when the system says not to disturb (a full-screen app, a presentation, notifications
// off for this app).
class Notifier {
public:
    Notifier(HWND window, UINT activation_message, UINT tray_message);
    ~Notifier();
    Notifier(const Notifier&) = delete;
    Notifier& operator=(const Notifier&) = delete;
    // True when the message was handed to the system.
    bool show(const notify::Message& message);
    void on_tray(LPARAM lparam);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

// Drag a folder out of the window (OLE DoDragDrop with a CF_HDROP data object; copy or link). Returns when the drag ends; true when it was dropped.
bool drag_folder(const std::filesystem::path& folder);

}  // namespace cb::app::window

#endif  // _WIN32
