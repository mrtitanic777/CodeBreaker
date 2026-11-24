// app/window_layout.hpp -- where everything of the CodeBreaker window is: the model the window shows, the layout of its parts in device-independent
// pixels (1 = 1/96 inch), and which part a point is over. No operating-system API here, so it is tested on every system (window_layout_test.cpp);
// the drawing (window_render_win.cpp) and the Win32 shell (window_win.cpp) are the Windows parts.
//
// The design is the approved Windows 11 one (work/ui-mockup/win11.html): a title bar 48 tall, two equal cards (8 radius) with 24 outer padding
// and a 16 gap, a status bar 32 tall. The left card (Program) holds a dashed drop area, which becomes the progress view while a conversion runs, and an
// InfoBar above it when a file cannot be converted or the run failed; the right card (Source) holds the output folder and its two buttons.
#pragma once

#include <array>
#include <chrono>
#include <cstddef>
#include <deque>
#include <memory>
#include <string>
#include <vector>

#include "app/board.hpp"
#include "app/board_automation.hpp"
#include "app/board_c_compare.hpp"
#include "app/board_c_history.hpp"
#include "app/board_c_leaders.hpp"
#include "app/board_c_view.hpp"
#include "app/board_flow.hpp"
#include "app/board_index.hpp"
#include "app/board_inspect.hpp"
#include "app/board_projection.hpp"
#include "app/board_signals.hpp"
#include "app/convert.hpp"
#include "app/function_source.hpp"
#include "app/palette.hpp"
#include "app/source_preview.hpp"

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
    NavConvert, NavTools, NavSettings, ToolSearch, ToolRow, Field, FieldBrowse, Run, OutputPane,
    // the Tools page: a button of the tool's one choice (its index), Browse for a folder in a drop box that takes a file or a folder, Open folder
    ToolChoice, FieldBrowseFolder, OpenResult,
    // the Settings page: the Advanced row, Back from Advanced, and Locate... on a row of what this computer cannot convert yet (its index)
    SettingsAdvanced, SettingsBack, Locate,
    // the Convert page's lists: a recent conversion and its Remove (Model::hot_index is the row), a file of the preview, the code view, the output box
    // that the folder is dragged out of
    RecentRow, RecentRemove, PreviewFile, CodeView, OutputBox,
    // the Source card's tabs (Files, Functions, Check) and what the Functions, Check and Compare views hold (Model::hot_index is the function / line)
    TabFiles, TabFunctions, TabCheck, FnRow, FnFilter, FnAll, FnPicker, FnPickItem, CheckShow, CheckAgain, CmpAsmLine, CmpCLine,
    // the Board page (window_board.hpp): Model::hot_index is the column of a foundation or head, the card of a card
    NavBoard, BoardOpen, BoardCancel, BoardSearch, BoardFoundation, BoardExpand, BoardCard, BoardCardOpen, BoardCardConvert,
    // its Flow and Trace views: a view tab or a kind of link (index), Map on the selected card, a band (index; + 100 on the right), a module of the trace (index)
    BoardViewTab, BoardLinkChip, BoardMap, BoardFlowBand, BoardNode, BoardWidth,
    // the Library view: a breadcrumb (index; -1 the board), a channel on the left or right (its position), a file that uses the export under the pointer (its module)
    BoardCrumb, BoardLeft, BoardRight, BoardUser,
    // the signal filters: a kind of signal (index), a value in its open list (the signal), an active filter's remove (the signal), Group copies
    BoardSignalType, BoardSignalRow, BoardFilterChip, BoardGroupCopies,
    // the X-Ray inspector: Inspect on the selected card, its close button, one of its rows (index)
    BoardInspect, BoardInspectClose, BoardInspectRow,
    // automation: Orphans, Decks and its list (a row's index; -1 Save this view) and a deck's remove, Export, the batch button
    BoardOrphans, BoardDecks, BoardDeckRow, BoardDeckRemove, BoardExport, BoardBatch,
    // a filter by C result (index: board::CView)
    BoardCView,
    // the function compare panel
    BoardCompareClose, BoardComparePrev, BoardCompareNext, BoardCompareBytes,
    // the whole OS's closest functions: the button, the list's close, a row (its rank)
    BoardLeaders, BoardLeadersClose, BoardLeaderRow, BoardLeadersSearch, BoardLeadersExport,
    // what changed since the last snapshot of the scores: the button, the list's close, keeping a snapshot now
    BoardHistory, BoardHistoryClose, BoardHistorySnapshot
};

// The Source card's three tabs; Compare (one function's original beside its rebuilt source) is a view of Functions.
enum class SourceTab { Files, Functions, Check };

// The pages of the rail.
enum class Page { Convert, Tools, Settings, Board };

// One line of the Tools list: a group heading, or a tool (its title and where it is in tools::all()).
struct ToolRowItem {
    bool heading = false;
    std::string text;
    int tool = -1;
};

// One field of a form (the selected tool's, or the Settings page's): what the label says, what kind of control it is, what has been entered.
// kind: 0 file (text + Browse), 1 folder (text + Browse), 2 text, 3 choice (drop-down), 4 toggle ("1" or ""); the Tools page's two: 5 a drop box (a file
// or folder dropped on the window or browsed; nothing typed), 6 a module (typing searches the work folder's modules; `choices` are the matches).
struct FieldItem {
    std::string label;
    int kind = 2;
    std::string text;
    std::vector<std::string> choices;
    bool required = false;
    std::string hint;                  // grey text in an empty field
    std::string note;                  // one line under the label (a setting's explanation); may be empty
    int accept = 0;                    // a drop box: 0 a file, 1 a folder, 2 either
};

// One row of the Settings page's "What this computer can convert": the first says what is ready; each after it is something that needs a tool,
// with Locate... to say where that tool is (`key`: the setting Locate... fills in; `folder`: it names a folder, not a file).
struct CapabilityItem {
    std::string label;
    std::string note;
    bool ready = true;
    std::string key;
    bool folder = true;
};

// One line of the Language drop-down: a language the translator writes, whether it can for the current file, and if not why.
struct LanguageItem {
    std::string id;                    // "original", "cpp17"
    std::string name;                  // "Original", "C++17"
    bool supported = true;
    std::string reason;                // shown under the name of an item that is not supported
};

// One row of the Program card's Recent list.
struct RecentItem {
    std::string name;                  // "lz32.dll"
    std::string chip;                  // "C"
    std::string meta;                  // "12 files · 2 minutes ago", or "folder moved or deleted"
    bool missing = false;              // the output folder is gone: the row offers Remove instead
    bool glyph_console = false;        // a game-console image (its icon is a game controller)
};

// One row of the queue: a file waiting, running or finished.
struct QueueRowItem {
    std::string name;
    std::string detail;                // the running row's stage: "Decompiling · 17 of 39 functions"
    std::string status;                // "Done", "62%", "Waiting", "Can't convert"
    int state = 0;                     // 0 waiting, 1 running, 2 done, 3 cannot convert, 4 failed, 5 cancelled
    int percent = 0;
};

// One file of the Source card's preview list.
struct PreviewFileItem {
    std::string name;                  // the path relative to the output folder
    bool script = false;               // a build script (its icon differs)
};

// The hint under the output box: what the box is for.
inline constexpr const char* kDragHint = "Drag the folder anywhere: Explorer, VS Code, a USB drive.";

// Everything the window draws that changes.
struct Model {
    View view = View::Idle;
    bool dark = false;
    bool dragging = false;             // a file is being dragged over the window: the drop area is highlighted
    bool maximized = false;            // the maximise caption button shows "restore"
    std::string file_name;             // the program being converted
    std::string stage_label;           // "Disassembling"
    std::string detail;                // "17 of 39 functions"
    std::string elapsed;               // Working: how long the run has taken so far, a running clock ("12:04"); empty before it starts
    int percent = 0;
    std::vector<std::string> stream;   // Working: the character stream's lines to draw behind the progress (CharStream::visible)
    bool stream_cursor = false;        // its block cursor is showing (it blinks)
    // ---- Recent (Idle), the queue (Working, several files), the preview and the drag source (Done)
    std::vector<RecentItem> recent;    // the Program card's "Recent" list (newest first)
    std::vector<QueueRowItem> queue;   // not empty: the Program card lists the files being converted
    std::string queue_note;            // "2 of 5 done"
    float queue_scroll = 0;            // DIPs
    std::vector<PreviewFileItem> preview_files;   // not empty: the Source card shows the files and one's source
    int preview_selected = 0;
    std::vector<std::string> preview_lines;       // the selected file's first lines
    std::vector<std::vector<preview::Span>> preview_spans;   // their colouring
    std::string preview_note;          // "Showing the first 128 KB of 3.4 MB."
    float preview_scroll = 0;          // the first visible line
    float preview_hscroll = 0;         // columns scrolled left
    float preview_list_scroll = 0;     // DIPs
    bool drag_out = false;             // the output folder is being dragged out (the box keeps its outline)
    bool drag_ghost = false;           // a drag image is drawn beside the box (screenshots only: the real drag has the system's)
    // ---- the Source card's tabs: Functions (the list, its filter), Check (the counts), Compare (one function, both sides)
    SourceTab source_tab = SourceTab::Files;
    std::vector<convert::FunctionRecord> functions;   // every function of the result (function_source.hpp's collect)
    bool can_recheck = false;          // the route can judge again: "Check again" is enabled
    int fn_filter = 0;                 // Show: 0 All, 1 Differs, 2 Not checked
    float fn_scroll = 0;               // DIPs
    int cmp_index = -1;                // >= 0: Compare shows functions[cmp_index]
    std::vector<std::string> cmp_asm, cmp_c;       // its two sides
    std::vector<std::vector<preview::Span>> cmp_asm_spans, cmp_c_spans;
    std::vector<fnsrc::Link> cmp_links;            // the lines that correspond
    std::string cmp_asm_note, cmp_c_note;          // why a side is empty
    int cmp_side = -1;                 // the line clicked: 0 original, 1 rebuilt; -1 none
    int cmp_line = -1;
    float cmp_scroll_asm = 0, cmp_scroll_c = 0;    // the first visible line of each side
    bool cmp_picker_open = false;      // the function drop-down is dropped down
    int cmp_picker_hot = 0;            // its item under the pointer or the keyboard
    int cmp_picker_top = 0;            // its first visible item
    // ---- the command palette (Ctrl+K)
    bool palette_open = false;
    std::string palette_query;
    int palette_selected = 0;          // the choosable row (palette::Result::picks)
    std::vector<palette::Item> palette_items;
    palette::Result palette_result;
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
    std::vector<FieldItem> tool_fields;                         // its inputs: drop boxes (kind 5) or a module (kind 6)
    std::string tool_choice_label;                              // its one choice ("Platform"), shown as buttons; empty: none
    std::vector<std::string> tool_choices;                      // "PC", "Xbox 360"
    int tool_choice = 0;                                        // the button chosen
    std::string tool_result;                                    // the folder a finished run left its result in: Open folder shows it
    bool developer_tools = false;                               // "Show developer tools": the developer groups are listed
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
    std::vector<FieldItem> setting_fields;                      // the main view: Theme, the output folder, Keep responsive; Advanced: every other setting
    std::vector<std::string> setting_keys;                      // the setting each field edits ("" for the theme), one per field
    float setting_scroll = 0;
    bool settings_advanced = false;                             // Advanced is open: every tool location and limit, as a form
    std::vector<CapabilityItem> capabilities;                   // the main view's "What this computer can convert"
    // a drop-down of a form field is open (the Language drop-down has its own flag above)
    int field_popup = -1;
    // ---- the Board page (board.hpp, window_board.hpp). The board is shared, not copied, when the Model is.
    std::shared_ptr<board::Board> board;
    board::Stacks board_stacks;                                 // what each column lists, for board_query
    std::string board_query;
    std::array<bool, board::kPileCount> board_expanded{};       // the column is a scrolling list rather than a fan
    std::array<bool, board::kPileCount> board_show_done{};      // the column lists the cards on its foundation
    std::array<float, board::kPileCount> board_scroll{};        // an expanded column's scroll, DIPs
    float board_hscroll = 0;                                    // the columns, sideways
    int board_selected = -1;                                    // a card (Board::cards)
    bool board_busy = false;                                    // an image is being extracted
    int board_percent = 0;
    std::string board_stage, board_detail, board_error;
    int board_drag = -1;                                        // the card being dragged
    float board_drag_x = 0, board_drag_y = 0;
    int board_drop_pile = -1;                                   // the foundation it is over
    bool board_orphans_only = false;                            // only the files nothing links to
    std::shared_ptr<const board::Orphans> board_orphans;
    std::vector<board::Deck> board_decks;                       // the built-in ones, then the person's
    bool board_decks_open = false;
    int board_active_deck = -1;
    std::vector<std::string> board_deck_names;                  // the active deck's file names (any of them)
    std::vector<std::uint32_t> board_batch;                     // the binaries among the cards shown that a batch would convert, most linked first
    std::size_t board_batch_armed = 0;                          // the batch button was pressed once: pressing it again queues this many
    bool board_inspecting = false;                              // the X-Ray drawer is open
    std::shared_ptr<const board::Inspector> board_inspector;
    float board_inspect_scroll = 0;
    std::shared_ptr<const board::CFunctionCompare> board_compare;   // one function side by side, over the columns
    std::array<float, 2> board_compare_scroll{};                // its C and its assembly, DIPs
    std::vector<board::CCompareItem> board_compare_list;        // what previous and next go through
    int board_compare_at = -1;
    bool board_compare_bytes = false;                           // the panel shows the two sides' bytes rather than C and assembly
    std::shared_ptr<const board::CLeaders> board_leaders;       // the whole OS's closest functions, as last read
    bool board_leaders_open = false, board_leaders_busy = false;
    float board_leaders_scroll = 0;
    std::string board_leaders_query;                            // finds any function of the OS
    std::vector<board::CLeader> board_leaders_found;            // what it found, as many as are shown
    std::uint64_t board_leaders_matches = 0;
    std::shared_ptr<const board::CHistory> board_history;      // the scores now against the newest snapshot
    bool board_history_open = false;
    float board_history_scroll = 0;
    std::shared_ptr<const board::HoverLinks> board_hover;       // the card under the pointer and what it is linked to
    std::array<std::uint32_t, board::kPileCount> board_hover_linked{};   // how many cards of each column are lit
    bool board_group_copies = true;                             // identical files are one master card
    std::vector<std::uint32_t> board_filters;                   // the signals a card must be wired to, all of them
    int board_signal_type = -1;                                 // the kind of signal whose list is open
    std::vector<std::pair<std::uint32_t, std::uint32_t>> board_signal_rows;   // that list: (signal, distinct files), busiest first
    float board_signal_scroll = 0;
    int board_view = 0;                                         // 0 the cards, 1 Flow, 2 Trace, 3 Library
    std::shared_ptr<const board::LibraryView> board_library;
    std::shared_ptr<const board::Projection> board_projection;  // calls per hour: measured where a trace saw them, else a model (board_projection.hpp)
    std::shared_ptr<const board::Measured> board_measured;      // the trace kept with this board
    std::shared_ptr<const board::CScores> board_scores;         // how its generated C recompiled
    std::shared_ptr<const board::CMarks> board_c_marks;         // those scores by card, and the whole OS (with the stacks)
    board::CView board_c_view = board::CView::All;              // the filter by C result
    std::pair<std::uint64_t, std::int64_t> board_scores_stamp{};   // the size and time of the table those scores were read from
    std::array<bool, board::kPileCount> board_c_kept_done{};    // which columns listed their foundation before that filter
    double board_flow_time = 2.5;                               // seconds: where the flow dots are
    std::vector<std::pair<int, std::string>> board_crumbs;      // the way here: (module, function or empty), the last is what is in view
    std::string board_function;                                 // the Library view is centred on this function of the module in focus
    int board_hover_export = -1;                                // the export channel whose lines are showing
    float board_left_scroll = 0, board_right_scroll = 0;        // the Library view's two lists, DIPs
    unsigned board_links = board::kLinkImports;                 // which kinds of link the maps count
    board::Width board_width = board::Width::Files;             // what a channel's width counts
    int board_focus = -1;                                       // the module the maps are centred on
    std::shared_ptr<const board::Index> board_index;
    std::shared_ptr<const board::Signals> board_signals;
    std::shared_ptr<const board::Graph> board_graph;
    std::shared_ptr<const board::Flow> board_flow;
    std::shared_ptr<const board::Trace> board_trace;
};

// Every rectangle of the window for a client size.
struct Layout {
    float width = 0, height = 0;
    // title bar
    Rect title_bar, app_icon, title_text, caption_minimize, caption_maximize, caption_close;
    // the navigation rail (empty when compute_layout is given no rail)
    Rect rail, rail_convert, rail_tools, rail_board, rail_settings;
    // cards and status bar
    Rect left_card, right_card, status_bar;
    Rect left_title, left_desc, right_title, right_desc;
    Rect drop;                         // the dashed drop area (below the InfoBar when there is one)
    Rect info_bar, info_icon, info_title, info_message, try_another;
    // the idle group inside the drop area
    Rect plus, drop_line1, drop_line2, browse;
    // the working / done group inside the drop area
    Rect file_icon, file_name, bar, bar_label, bar_percent, detail, elapsed, cancel;
    // the right card's group
    Rect language_label, language_combo, output_label, field, open_folder, copy_path, hint;
    // the Source card's preview (empty unless Extras::preview): the file list on the left, the code on the right
    Rect preview, preview_list, preview_code;
    // the Source card's tab strip (empty unless Extras::preview) and the Functions / Check / Compare view under it (`preview` is that view's area, with
    // `preview_list` and `preview_code` empty, when the Files tab is not the one shown)
    Rect tabs, tab_files, tab_functions, tab_check;
    // the Program card's Recent list (empty unless Extras::recent_rows): its heading, a row each, and the Remove button of a row whose folder is gone
    Rect recent_heading;
    std::vector<Rect> recent_rows, recent_remove;
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
//
// What the Convert page holds besides the view decides some rectangles: `recent_rows` Recent rows shrink the idle drop area; `preview` gives the Source card
// the file list and code view (the path row moves above them, the finished file's name to the left); `queue` puts "Cancel all" at the foot of the Program
// card (the files are listed in window_convert.hpp's queue layout). extras_for takes them from a Model for the card of `view`.
struct Extras {
    int recent_rows = 0;
    bool preview = false;
    bool queue = false;
    int tab = 0;                       // the Source card's tab (SourceTab): the Files tab has the path row, the others give its room to their view
};
Extras extras_for(const Model& model, View view);

// A run's time so far as a clock that ticks each second: "0:07", "12:04", "1:02:03".
std::string elapsed_clock(std::chrono::seconds taken);
// A finished run's time in words: "7 s", "12 min 4 s", "1 h 2 min".
std::string took_words(std::chrono::seconds taken);

Layout compute_layout(View view, float width, float height, float rail = 0, const Extras& extras = {});

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
