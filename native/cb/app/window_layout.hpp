// cb/app/window_layout.hpp -- where everything of the CodeBreaker window is: the model the window shows, the layout of its parts in device-independent
// pixels (1 = 1/96 inch), and which part a point is over. No operating-system API here, so it is tested on every system (window_layout_test.cpp);
// the drawing (window_render_win.cpp) and the Win32 shell (window_win.cpp) are the Windows parts.
//
// The design is the approved Windows 11 one (state/work/ui-mockup/win11.html): a title bar 48 tall, two equal cards (8 radius) with 24 outer padding
// and a 16 gap, a status bar 32 tall. The left card (Program) holds a dashed drop area, which becomes the progress view while a conversion runs, and an
// InfoBar above it when a file cannot be converted or the run failed; the right card (Source) holds the output folder and its two buttons.
#pragma once

#include <string>
#include <vector>

namespace cb::app::window {

struct Rect {
    float x = 0, y = 0, w = 0, h = 0;
    float right() const { return x + w; }
    float bottom() const { return y + h; }
    bool contains(float px, float py) const { return px >= x && px < x + w && py >= y && py < y + h; }
};

// What the window shows.
enum class View { Idle, Working, Done, Unsupported, Failed };

// The parts a person can point at, click or reach with Tab.
enum class Control { None, Plus, Browse, Cancel, TryAnother, OpenFolder, CopyPath, CaptionMinimize, CaptionMaximize, CaptionClose };

// Everything the window draws that changes.
struct Model {
    View view = View::Idle;
    bool dark = false;
    bool dragging = false;             // a file is being dragged over the window: the drop area is highlighted
    bool maximized = false;            // the maximise caption button shows "restore"
    std::string file_name;             // the program being converted
    std::string stage_label;           // "Disassembling"
    std::string detail;                // "17 of 39 functions"
    int percent = 0;
    std::string info_title;            // the InfoBar's title and message (Unsupported, Failed)
    std::string info_message;
    std::string output_path;           // Done: the folder
    std::string done_note;             // Done: "12 files written."
    std::string status_text = "Ready";
    std::string platforms;             // the status bar's right side
    Control hot = Control::None;       // under the pointer
    Control pressed = Control::None;
    Control focus = Control::None;     // the keyboard focus (drawn as a ring)
    bool window_active = true;
};

// Every rectangle of the window for a client size.
struct Layout {
    float width = 0, height = 0;
    // title bar
    Rect title_bar, app_icon, title_text, caption_minimize, caption_maximize, caption_close;
    // cards and status bar
    Rect left_card, right_card, status_bar;
    Rect left_title, left_desc, right_title, right_desc;
    Rect drop;                         // the dashed drop area (below the InfoBar when there is one)
    Rect info_bar, info_icon, info_title, info_message, try_another;
    // the idle group inside the drop area
    Rect plus, drop_line1, drop_line2, browse;
    // the working / done group inside the drop area
    Rect file_icon, file_name, bar, bar_label, bar_percent, detail, cancel;
    // the right card's group
    Rect output_label, field, open_folder, copy_path, hint;
    // the status bar
    Rect status_dot, status_text, platforms;
};

inline constexpr float kTitleBarHeight = 48;
inline constexpr float kCaptionButtonWidth = 46;
inline constexpr float kStatusBarHeight = 32;
inline constexpr float kOuterPadding = 24;
inline constexpr float kCardGap = 16;
inline constexpr float kCardRadius = 8;
inline constexpr float kMinWidth = 800;
inline constexpr float kMinHeight = 600;
inline constexpr float kDefaultWidth = 1180;
inline constexpr float kDefaultHeight = 700;

// The layout of a window `width` x `height` (client area, DIPs) showing `view`.
Layout compute_layout(View view, float width, float height);

// The control at a point, None over nothing. Only the controls that exist in `view` (a disabled one is still found: `enabled` says whether it acts).
Control hit_test(const Layout& layout, View view, float x, float y);
bool control_exists(Control control, View view);
bool control_enabled(Control control, View view);
// The controls Tab visits, in order, in `view`.
std::vector<Control> focus_order(View view);
// The rectangle of a control.
Rect control_rect(const Layout& layout, Control control);

}  // namespace cb::app::window
