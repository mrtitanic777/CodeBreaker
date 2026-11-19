// cb/app/window_layout.hpp -- where everything of the CodeBreaker window is: the model the window shows, the layout of its parts in device-independent
// pixels (1 = 1/96 inch), and which part a point is over. No operating-system API here, so it is tested on every system (window_layout_test.cpp);
// the drawing (window_render_win.cpp) and the Win32 shell (window_win.cpp) are the Windows parts.
//
// The design is the approved Windows 11 one (state/work/ui-mockup/win11.html): a title bar 48 tall, two equal cards (8 radius) with 24 outer padding
// and a 16 gap, a status bar 32 tall. The left card (Program) holds a dashed drop area, which becomes the progress view while a conversion runs, and an
// InfoBar above it when a file cannot be converted or the run failed; the right card (Source) holds the output folder and its two buttons.
#pragma once

#include <cstddef>
#include <deque>
#include <string>
#include <vector>

namespace cb::app::window {

// The character stream behind a running conversion: the lines the pipeline produces (convert::Progress::lines) typed out one character at a
// time, like an old home computer printing a listing. Pure time arithmetic, so it is tested on every system; the window advances it from a
// timer and draws `visible()`.
//
// Pace: kStreamCharsPerSecond while the stream keeps up; when lines pile up it types faster in proportion to the backlog, so it never falls
// minutes behind the work; a backlog beyond kStreamPendingLines drops its oldest lines.
class CharStream {
public:
    static constexpr double kStreamCharsPerSecond = 110;
    static constexpr std::size_t kStreamPendingLines = 300;
    static constexpr std::size_t kStreamKeptLines = 80;

    void push(const std::vector<std::string>& lines);
    // `seconds` more of typing.
    void advance(double seconds);
    // Everything waiting is typed at once (Windows' "Animation effects" turned off).
    void show_all();
    void clear();
    // The last `rows` lines, oldest first; the last one may be partly typed.
    std::vector<std::string> visible(std::size_t rows) const;
    // Characters are still waiting to be typed.
    bool typing() const { return has_current_ || !pending_.empty(); }
    bool empty() const { return shown_.empty() && !typing(); }

private:
    std::size_t backlog() const;
    void start_next();

    std::deque<std::string> pending_;   // lines not begun
    std::deque<std::string> shown_;     // lines typed in full (the newest kStreamKeptLines)
    std::string current_;               // the line being typed
    std::size_t typed_ = 0;             // how much of it
    bool has_current_ = false;
    double carry_ = 0;                  // a fraction of a character from the last advance
};

struct Rect {
    float x = 0, y = 0, w = 0, h = 0;
    float right() const { return x + w; }
    float bottom() const { return y + h; }
    bool contains(float px, float py) const { return px >= x && px < x + w && py >= y && py < y + h; }
};

// What the window shows.
enum class View { Idle, Working, Done, Unsupported, Failed };

// The parts a person can point at, click or reach with Tab.
enum class Control {
    None, Plus, Browse, Cancel, TryAnother, OpenFolder, CopyPath, Language, CaptionMinimize, CaptionMaximize, CaptionClose,
    // the navigation rail, and the Tools and Settings pages (window_pages.hpp): a field, a tool or a setting is named by Model::*_index beside the Control
    NavConvert, NavTools, NavSettings, ToolSearch, ToolRow, Field, FieldBrowse, Run, OutputPane
};

// The pages of the rail.
enum class Page { Convert, Tools, Settings };

// One line of the Tools list: a group heading, or a tool (its title and where it is in tools::all()).
struct ToolRowItem {
    bool heading = false;
    std::string text;
    int tool = -1;
};

// One field of a form (the selected tool's, or the Settings page's): what the label says, what kind of control it is, what has been entered.
// kind: 0 file (text + Browse), 1 folder (text + Browse), 2 text, 3 choice (drop-down), 4 toggle ("1" or "").
struct FieldItem {
    std::string label;
    int kind = 2;
    std::string text;
    std::vector<std::string> choices;
    bool required = false;
    std::string hint;                  // grey text in an empty field
    std::string note;                  // one line under the label (a setting's explanation); may be empty
};

// One line of the Language drop-down: a language the translator writes, whether it can for the current file, and if not why.
struct LanguageItem {
    std::string id;                    // "original", "cpp17"
    std::string name;                  // "Original", "C++17"
    bool supported = true;
    std::string reason;                // shown under the name of an item that is not supported
};

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
    std::vector<std::string> stream;   // Working: the character stream's lines to draw behind the progress (CharStream::visible)
    bool stream_cursor = false;        // its block cursor is showing (it blinks)
    // ---- the fades, moved by the window's own timer (1 = fully shown). The Convert page shows one card: Program, or Source when Done; when the
    // card changes the old one fades out, then the new one fades in.
    float card_alpha = 1;              // the card of `view`
    View fade_from = View::Idle;       // the card fading out (while fade_from_alpha > 0)
    float fade_from_alpha = 0;
    bool welcome = false;              // the welcome screen is showing (the window has just opened)
    float welcome_alpha = 0;           // its words
    float chrome_alpha = 1;            // everything below the title bar: the rail, the page, the status bar (0 while the welcome shows)
    std::string info_title;            // the InfoBar's title and message (Unsupported, Failed)
    std::string info_message;
    std::string output_path;           // Done: the folder
    std::string done_note;             // Done: "12 files written."
    std::string done_file_name;        // Done: the program that was converted (the Source card keeps it while it fades out for the next file)
    std::string status_text = "Ready";
    std::string platforms;             // the status bar's right side
    std::string drop_hint = ".exe, .dll, ROMs, disc images and more";   // the drop area's second line
    std::vector<LanguageItem> languages;   // the Language drop-down's list
    int language_selected = 0;         // the chosen item
    bool language_open = false;        // the list is dropped down
    int language_hot = -1;             // the item under the pointer or the keyboard in the open list; -1 none
    Control hot = Control::None;       // under the pointer
    Control pressed = Control::None;
    Control focus = Control::None;     // the keyboard focus (drawn as a ring)
    bool window_active = true;

    // ---- the rail and the other two pages
    Page page = Page::Convert;
    int hot_index = -1, pressed_index = -1, focus_index = -1;   // which tool row / field the Control above is
    bool activity = false;                                      // a tool is running: the rail's Tools item shows a dot
    // Tools page
    std::string tool_query;                                     // the search box
    std::string tools_count_text = "Search tools";              // its placeholder
    std::vector<ToolRowItem> tool_rows;                         // the list as shown (headings and the tools that match)
    int tool_selected = -1;                                     // the selected tool's index in tools::all(); -1 none
    float tool_scroll = 0;                                      // list scroll, DIPs
    std::string tool_title, tool_description;                   // the selected tool
    std::vector<FieldItem> tool_fields;
    bool tool_can_run = false;                                  // the required fields are filled in
    bool tool_running = false;
    int tool_percent = -1;                                      // -1: the command prints none (an indeterminate bar); 0..100
    std::string tool_output;                                    // the output pane's text
    std::string tool_note;                                      // beside Run: "Running...", "Finished", "Stopped", "Exit code 1"
    bool tool_done = false;                                     // a run has finished (the bar is full)
    float tool_phase = 0;                                       // 0..1, moves the indeterminate bar
    float output_scroll = 0;                                    // lines scrolled up from the end
    int caret = 0;                                              // the text caret in the focused field (UTF-8 bytes)
    // Settings page
    std::vector<FieldItem> setting_fields;                      // the first is the theme (a choice), then every setting
    std::vector<std::string> setting_keys;                      // the setting each field after the theme edits
    float setting_scroll = 0;
    // a drop-down of a form field is open (the Language drop-down has its own flag above)
    int field_popup = -1;
};

// Every rectangle of the window for a client size.
struct Layout {
    float width = 0, height = 0;
    // title bar
    Rect title_bar, app_icon, title_text, caption_minimize, caption_maximize, caption_close;
    // the navigation rail (empty when compute_layout is given no rail)
    Rect rail, rail_convert, rail_tools, rail_settings;
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
    Rect language_label, language_combo, output_label, field, open_folder, copy_path, hint;
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

// The layout of a window `width` x `height` (client area, DIPs) showing `view`. With a `rail` width (kRailWidth: the navigation rail of the window) the
// cards start after it (the design's 4 DIP gap) instead of at the outer padding; the title bar and the status bar still span the window.
inline constexpr float kRailWidth = 64;
Layout compute_layout(View view, float width, float height, float rail = 0);

// The control at a point, None over nothing. Only the controls that exist in `view` (a disabled one is still found: `enabled` says whether it acts).
Control hit_test(const Layout& layout, View view, float x, float y);
bool control_exists(Control control, View view);
bool control_enabled(Control control, View view);
// The controls Tab visits, in order, in `view`.
std::vector<Control> focus_order(View view);
// The rectangle of a control.
Rect control_rect(const Layout& layout, Control control);

// The dropped-down list of the Language control: its outline and one row per item (a row of an item with a reason is taller, the reason sits under
// the name). It opens below the control, or above it where there is no room below.
struct LanguageList {
    Rect outline;
    std::vector<Rect> rows;
};
inline constexpr float kLanguageRowHeight = 36;
inline constexpr float kLanguageRowWithReasonHeight = 48;
LanguageList language_list_layout(const Layout& layout, const std::vector<LanguageItem>& items);
// The item of the open list at a point; -1 over none.
int language_item_at(const LanguageList& list, float x, float y);
// The next enabled item from `from` in a direction (+1, -1), staying put at the ends; -1 when none is enabled.
int language_next_enabled(const std::vector<LanguageItem>& items, int from, int direction);

}  // namespace cb::app::window
