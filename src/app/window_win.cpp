// app/window_win.cpp -- the CodeBreaker window (Windows): the Win32 shell around the drawing of window_render_win.cpp and the pipeline of convert.hpp.
//
// A top-level window with its own 48 px title bar (WM_NCCALCSIZE takes the system caption away; WM_NCHITTEST gives the system back the caption, the
// caption buttons -- so the snap layouts flyout and the system menu work -- and the resize borders), the Mica backdrop on Windows 11 (a solid colour
// elsewhere), the system theme and accent colour followed live, per-monitor DPI. Everything inside is drawn by Direct2D (window_render_win.cpp).
//
// The conversion runs on a worker thread; its progress reaches the window by messages (WM_APP + n), so the window never blocks.
// `codebreaker app --screenshot OUT.png` draws the same window into a PNG instead.
#ifdef _WIN32
#include <windows.h>
#include <windowsx.h>

#include <dwmapi.h>
#include <objbase.h>
#include <ole2.h>
#include <shellapi.h>
#include <shobjidl.h>
#include <wincodec.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cctype>
#include <cmath>
#include <condition_variable>
#include <cstdlib>
#include <cstring>
#include <format>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <string>
#include <thread>
#include <unordered_map>

#include "app/batch_queue.hpp"
#include "app/board.hpp"
#include "app/board_index.hpp"
#include "app/board_signals.hpp"
#include "app/board_trace.hpp"
#include "app/convert.hpp"
#include "app/notify.hpp"
#include "app/recent.hpp"
#include "app/settings_edit.hpp"
#include "app/source_preview.hpp"
#include "app/tools.hpp"
#include "app/tools_run.hpp"
#include "app/window_board.hpp"
#include "app/window_pages.hpp"
#include "app/window.hpp"
#include "app/window_convert.hpp"
#include "app/window_functions.hpp"
#include "app/function_source.hpp"
#include "app/palette.hpp"
#include "app/window_layout.hpp"
#include "app/window_render_win.hpp"
#include "app/window_shell_win.hpp"
#include "base/config.hpp"
#include "base/corpus.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/process.hpp"
#include "formats/media.hpp"
#include "core/text.hpp"

namespace cb::app::window {

namespace {

namespace stdfs = std::filesystem;

// DWM attributes newer than some SDKs' headers.
constexpr DWORD kWwUseImmersiveDarkMode = 20;
constexpr DWORD kWwWindowCornerPreference = 33;
constexpr DWORD kWwSystemBackdropType = 38;
constexpr int kWwRound = 2;
constexpr int kWwMainWindowBackdrop = 2;   // DWMSBT_MAINWINDOW: Mica
constexpr DWORD kWwCaptionColor = 35;      // DWMWA_CAPTION_COLOR
constexpr DWORD kWwColorNone = 0xFFFFFFFE;    // DWMWA_COLOR_NONE: no colour, the backdrop shows

constexpr UINT kWmProgress = WM_APP + 1;   // the worker has progress to show
constexpr UINT_PTR kWwRunTimer = 1;       // while a tool runs: its output is read into the page
constexpr UINT_PTR kWwStreamTimer = 2;    // while a conversion runs: the character stream types on (about 30 frames a second)
constexpr UINT kWwStreamFrameMs = 33;
constexpr std::size_t kWwStreamRows = 40;  // the most lines of the stream the drop area can show
constexpr UINT_PTR kWwFadeTimer = 3;      // while the welcome shows or a card changes: about 60 frames a second
constexpr UINT kWwFadeFrameMs = 16;
// the welcome: its words fade in, stay, fade out, then the page fades in (seconds from the window opening)
constexpr double kWwWelcomeIn = 0.6, kWwWelcomeHold = 1.5, kWwWelcomeOut = 2.0, kWwWelcomeEnd = 2.4;
// a card change: the old card fades out, then the new one fades in (seconds)
constexpr double kWwCardOut = 0.2, kWwCardIn = 0.3;

// 0..1 with a gentle start and end.
float ww_ease(double t) {
    const double x = std::clamp(t, 0.0, 1.0);
    return static_cast<float>(x * x * (3 - 2 * x));
}
constexpr UINT kWmFinished = WM_APP + 2;   // the worker is done: its outcome is ready
constexpr UINT kWmToast = WM_APP + 3;      // a button or click on the notification: wParam 1 open the folder, 2 show the window
constexpr UINT kWmTray = WM_APP + 4;       // the balloon's callbacks (where there is no toast)
constexpr UINT kWmBoardProgress = WM_APP + 5;   // the board's extraction has progress to show
constexpr UINT kWmBoardDone = WM_APP + 6;       // the board's extraction or load has ended
constexpr UINT_PTR kWwTaskbarTimer = 4;    // a failure shown on the taskbar button while the window is in front: cleared after a few seconds
constexpr UINT kWwTaskbarErrorMs = 4000;
constexpr UINT_PTR kWwMinuteTimer = 5;     // the Recent list's "when" words are made new
constexpr UINT kWwMinuteMs = 30000;
constexpr UINT_PTR kWwScoresTimer = 17;    // the Board looks for new rows in its score table
constexpr UINT kWwScoresMs = 5000;
constexpr UINT_PTR kWwFlowTimer = 6;       // while Flow, Trace or Library shows: the projected-call dots move
constexpr UINT kWwFlowFrameMs = 33;
constexpr wchar_t kWwClass[] = L"CodeBreakerWindow";

// ---- what the system says ------------------------------------------------------------------------------------------------------------

// Windows' "Animation effects" (Settings > Accessibility > Visual effects): when off, nothing in the window moves on its own.
bool ww_animations_on() {
    BOOL on = TRUE;
    if (!SystemParametersInfoW(SPI_GETCLIENTAREAANIMATION, 0, &on, 0)) return true;
    return on != FALSE;
}

bool ww_system_dark() {
    DWORD value = 1, size = sizeof value;
    if (RegGetValueW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", L"AppsUseLightTheme", RRF_RT_REG_DWORD, nullptr, &value,
                     &size) != ERROR_SUCCESS)
        return false;
    return value == 0;
}

// The accent colour for the theme: the Windows accent palette's "dark 1" on light, "light 2" on dark (what Windows' own filled buttons use); 0 when
// the palette cannot be read (the design's default blue is used).
std::uint32_t ww_accent(bool dark) {
    BYTE palette[32] = {};
    DWORD size = sizeof palette;
    if (RegGetValueW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Accent", L"AccentPalette", RRF_RT_REG_BINARY, nullptr, palette, &size) !=
            ERROR_SUCCESS ||
        size < 32)
        return 0;
    const BYTE* c = palette + (dark ? 1 : 4) * 4;   // R, G, B, A
    return (static_cast<std::uint32_t>(c[0]) << 16) | (static_cast<std::uint32_t>(c[1]) << 8) | c[2];
}

float ww_dip(int pixels, UINT dpi) { return static_cast<float>(pixels) * 96.0f / static_cast<float>(dpi); }

// The Language list of the window from what the translator says it can write.
std::vector<LanguageItem> ww_items(const std::vector<translate::capability::Target>& targets) {
    std::vector<LanguageItem> items;
    for (const translate::capability::Target& t : targets) items.push_back({t.id, t.name, t.supported, t.reason});
    return items;
}

// The index of the language `id` in `items`; the first supported one when it is not there.
int ww_index_of(const std::vector<LanguageItem>& items, const std::string& id) {
    for (std::size_t i = 0; i < items.size(); ++i)
        if (items[i].id == id) return static_cast<int>(i);
    return 0;
}


// ---- the Tools and Settings models -------------------------------------------------------------------------------------------------------

int ww_tool_index(const tools::Tool* t) { return static_cast<int>(t - tools::all().data()); }

// The list as shown: every group that has a tool matching the search, its heading, then the tools (their indexes in tools::all()).
void ww_fill_tool_rows(Model& m) {
    m.tool_rows.clear();
    const std::vector<const tools::Tool*> found = tools::search(m.tool_query, m.developer_tools);
    for (const std::string& group : tools::groups()) {
        bool heading = false;
        for (const tools::Tool* t : found) {
            if (t->group != group) continue;
            if (!heading) {
                m.tool_rows.push_back({true, group, -1});
                heading = true;
            }
            m.tool_rows.push_back({false, t->title, ww_tool_index(t)});
        }
    }
    m.tools_count_text = "Search tools";
}

// The selected tool's title, sentence and form, with the values entered so far.
void ww_load_tool(Model& m, int tool, const tools::Values& values) {
    m.tool_selected = tool;
    m.tool_fields.clear();
    if (tool < 0) return;
    const tools::Tool& t = tools::all()[static_cast<std::size_t>(tool)];
    m.tool_title = t.title;
    m.tool_description = t.description;
    for (std::size_t i = 0; i < t.inputs.size(); ++i) {
        const tools::Input& in = t.inputs[i];
        FieldItem f;
        f.label = in.label;
        f.text = i < values.inputs.size() ? values.inputs[i] : std::string();
        f.required = true;
        if (in.kind == tools::InputKind::Module) {
            f.kind = 6;
            f.hint = "Type to search the work folder's modules";
        } else {
            f.kind = 5;
            f.accept = in.kind == tools::InputKind::Folder ? 1 : in.kind == tools::InputKind::Either ? 2 : 0;
            f.hint = in.accepts.empty() ? std::string(f.accept == 1 ? "or Browse for one" : "or Browse for one") : in.accepts;
            std::error_code ec;
            if (!f.text.empty() && stdfs::is_directory(fs::from_text(f.text), ec)) f.note = "folder";
        }
        m.tool_fields.push_back(std::move(f));
    }
    m.tool_choice_label = t.choice_label;
    m.tool_choices.clear();
    for (const tools::Choice& c : t.choices) m.tool_choices.push_back(c.label);
    m.tool_choice = std::clamp(values.choice, 0, std::max(0, static_cast<int>(t.choices.size()) - 1));
    m.tool_can_run = !tools::missing(t, values).has_value();
}

// The key the Settings page's "Show developer tools" row is known by (a preference of this window, kept per user beside the theme: no settings.json key).
constexpr std::string_view kWwDeveloperKey = "#developer_tools";

// The Settings page from the settings: the main view is Theme, the output folder and Keep the computer responsive, and what this computer can
// convert; Advanced (m.settings_advanced) is every other setting as a path (Browse), a switch or text.
void ww_fill_settings(Model& m, const std::vector<settings::Row>& rows, settings::Theme theme) {
    m.setting_fields.clear();
    m.setting_keys.clear();
    m.capabilities.clear();
    const auto field_of = [](const settings::Row& r) {
        FieldItem f;
        f.label = r.label.empty() ? r.key : r.label;
        f.note = r.hint;
        f.text = r.value;
        if (r.kind == settings::Kind::Path) {
            f.kind = 0;
            f.hint = r.set ? "Not found" : "Not set";
        } else if (r.kind == settings::Kind::Toggle) {
            f.kind = 4;
            f.text = text::lower(r.value) == "true" ? "1" : "";
        } else {
            f.kind = 2;
            f.hint = "Not set";
        }
        return f;
    };
    if (m.settings_advanced) {
        for (const settings::Row& r : rows) {
            if (settings::is_everyday(r.key)) continue;
            m.setting_fields.push_back(field_of(r));
            m.setting_keys.push_back(r.key);
        }
        return;
    }
    m.setting_fields.push_back({"Theme", 3, settings::theme_name(theme), {"System", "Light", "Dark"}, false, "", "Follow Windows, or always light or dark."});
    m.setting_keys.push_back("");
    for (const char* key : {"converted", "low_load"}) {
        for (const settings::Row& r : rows) {
            if (r.key != key) continue;
            FieldItem f = field_of(r);
            if (r.key == "converted") {
                f.label = "Save converted source to";
                f.kind = 1;
            }
            m.setting_fields.push_back(std::move(f));
            m.setting_keys.push_back(r.key);
        }
    }
    m.setting_fields.push_back({"Show developer tools", 4, m.developer_tools ? "1" : "", {}, false, "",
                                "The batch, check and maintenance tools are listed on the Tools page too."});
    m.setting_keys.push_back(std::string(kWwDeveloperKey));
    for (const settings::Capability& c : settings::capabilities(rows)) m.capabilities.push_back({c.label, c.note, c.ready, c.key, c.folder});
}

// A file or folder the person chooses ("" when they cancel).
std::string ww_choose_path(HWND owner, bool folder) {
    ComPtr<IFileOpenDialog> dialog;
    if (FAILED(CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(dialog.put())))) return {};
    DWORD options = 0;
    dialog->GetOptions(&options);
    dialog->SetOptions(options | FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST | (folder ? FOS_PICKFOLDERS : FOS_FILEMUSTEXIST));
    dialog->SetTitle(folder ? L"Choose a folder" : L"Choose a file");
    if (FAILED(dialog->Show(owner))) return {};
    ComPtr<IShellItem> item;
    if (FAILED(dialog->GetResult(item.put()))) return {};
    PWSTR path = nullptr;
    if (FAILED(item->GetDisplayName(SIGDN_FILESYSPATH, &path)) || path == nullptr) return {};
    const std::string chosen = narrow(path);
    CoTaskMemFree(path);
    return chosen;
}

// A UTF-8 string one character before / after byte `at`.
std::size_t ww_prev_char(const std::string& s, std::size_t at) {
    if (at == 0) return 0;
    --at;
    while (at > 0 && (static_cast<unsigned char>(s[at]) & 0xC0) == 0x80) --at;
    return at;
}
std::size_t ww_next_char(const std::string& s, std::size_t at) {
    if (at >= s.size()) return s.size();
    ++at;
    while (at < s.size() && (static_cast<unsigned char>(s[at]) & 0xC0) == 0x80) ++at;
    return at;
}

// ---- the drop target -------------------------------------------------------------------------------------------------------------------

class WwApp;

class WwDropTarget final : public IDropTarget {
public:
    explicit WwDropTarget(WwApp* app) : app_(app) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID id, void** out) override {
        if (id == IID_IUnknown || id == IID_IDropTarget) {
            *out = static_cast<IDropTarget*>(this);
            return S_OK;
        }
        *out = nullptr;
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return 2; }   // (one per process: not counted)
    ULONG STDMETHODCALLTYPE Release() override { return 1; }
    HRESULT STDMETHODCALLTYPE DragEnter(IDataObject* data, DWORD, POINTL, DWORD* effect) override;
    HRESULT STDMETHODCALLTYPE DragOver(DWORD, POINTL, DWORD* effect) override;
    HRESULT STDMETHODCALLTYPE DragLeave() override;
    HRESULT STDMETHODCALLTYPE Drop(IDataObject* data, DWORD, POINTL, DWORD* effect) override;

private:
    WwApp* app_;
    bool acceptable_ = false;
};

// ---- the window ------------------------------------------------------------------------------------------------------------------------

class WwApp {
public:
    WwApp() : painter_(), drop_(this) {}

    int run();
    void open_at_start(const stdfs::path& file) { initial_file_ = file; }

    // the drop target's calls
    void drag_changed(bool over);
    // (not while the folder is being dragged out of this window, nor once Cancel was pressed)
    bool can_take_files() const { return !drag_out_active_ && !(running_ && cancel_); }
    // Files to convert: they start a run, or join the queue of the run under way.
    void add_files(const std::vector<stdfs::path>& files);
    bool drop_on_tool(const std::vector<stdfs::path>& files);
    bool drop_on_board(const std::vector<stdfs::path>& files);
    void open_board_at_start(const stdfs::path& image) { initial_board_ = image; }

private:
    static LRESULT CALLBACK proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT handle(UINT msg, WPARAM wp, LPARAM lp);
    bool create_window();
    void apply_theme();
    void relayout();
    void invalidate() { InvalidateRect(hwnd_, nullptr, FALSE); }
    void paint();
    void make_target();
    Control control_at(LPARAM client_lparam) const;
    PageHit hit_at(float x, float y) const;
    PageHit hit_at(LPARAM client_lparam) const;
    Control control_at_screen(POINT screen) const;
    void set_hot(Control c, int index = -1);
    void activate(Control c);
    void activate_page(Control c, int index);
    void move_focus(bool backwards);
    void browse();
    void open_output();
    void show_system_menu(POINT screen);
    void copy_output_path();
    void set_language_list(const std::vector<translate::capability::Target>& targets);
    void open_language_list(bool open);
    void choose_language(int index);
    void language_key(WPARAM key, bool alt);
    LanguageList language_list() const { return language_list_layout(layout_, model_.languages); }
    // the rail and the other pages
    void set_page(Page page);
    std::string* edit_target();
    void set_focus(Control c, int index);
    void edited();
    void commit_setting(int field);
    void fill_settings();
    void open_advanced(bool open);
    void locate_tool(int index);
    void blur();
    void insert_text(const std::string& utf8);
    bool edit_key(WPARAM key, bool ctrl);
    void select_tool(int tool);
    void run_tool();
    void stop_tool();
    void poll_tool();
    void browse_field(int index, bool folder_wanted = false);
    void set_tool_input(int field, const std::string& path);
    void open_result();
    const std::vector<std::string>& work_modules();
    std::vector<LanguageItem> field_popup_items() const;
    LanguageList field_popup_list() const;
    void open_field_popup(int index);
    void choose_in_popup(int item);
    void popup_key(WPARAM key);
    bool popup_is_search() const {   // the open list is a module field's matches (typing goes on into the field)
        return model_.page == Page::Tools && model_.field_popup >= 0 && static_cast<std::size_t>(model_.field_popup) < model_.tool_fields.size() &&
               model_.tool_fields[static_cast<std::size_t>(model_.field_popup)].kind == 6;
    }
    void scroll_page(float x, float y, int lines);
    void cancel_conversion();
    // the queue, Recent, the preview, the drag, the notification (window_convert.hpp, recent.hpp, batch_queue.hpp, window_shell_win.hpp)
    void begin_run();
    void back_to_program();
    void run_next();
    void sync_queue();
    void finish_run();
    void present_item(const batch::Item& item, const std::string& stage, const std::string& detail, const std::vector<std::string>& stream);
    void refresh_recent();
    void remember_item(const batch::Item& item);
    void open_recent(int index);
    void remove_recent(int index);
    void load_preview_folder(const stdfs::path& folder, const std::string& main_stem);
    void select_preview(int index);
    bool scroll_convert(float x, float y, int lines, bool horizontal);
    bool convert_key(WPARAM key, bool ctrl);
    void scroll_preview_to(float first_line);
    void copy_preview_text();
    // the Source card's tabs and the command palette
    void load_functions(const stdfs::path& folder, const std::vector<convert::FunctionRecord>& recorded, const stdfs::path& input, bool can_recheck);
    void set_source_tab(SourceTab tab);
    void open_compare(int index);
    void close_compare();
    void compare_click(int side, int line);
    void open_picker(bool open);
    void picker_key(WPARAM key);
    void cycle_filter();
    bool source_key(WPARAM key);
    bool scroll_source(float x, float y, int lines);
    void palette_refresh();
    void open_palette();
    void close_palette();
    void palette_run(int pick);
    void palette_key(WPARAM key);
    void begin_drag_out();
    void update_taskbar();
    void notify_finished();
    void on_toast(WPARAM action);
    LanguageItem language_for(const stdfs::path& file) const;
    void on_progress();
    void stream_frame();
    void begin_card_fade(View from);
    void start_welcome();
    void skip_welcome();
    void fade_frame();
    void on_finished();
    void reset_to_idle(const std::string& status);
    // the Board page
    void browse_board();
    void open_board(const stdfs::path& image, const stdfs::path& kept);
    void on_board_progress();
    void on_board_done();
    void refresh_board();
    void board_convert(int index);
    void board_open_card(int index);
    void board_sync_states();
    void board_release(float x, float y);
    void sync_board_title();
    void sync_flow_timer();

    HWND hwnd_ = nullptr;
    UINT dpi_ = 96;
    Painter painter_;
    ComPtr<ID2D1HwndRenderTarget> target_;
    Model model_;
    Layout layout_;
    Appearance look_;
    bool tracking_ = false;
    bool backdrop_ = false;
    WwDropTarget drop_;
    stdfs::path initial_file_;   // a file named on the command line: converted as soon as the window is up

    // the Tools and Settings pages
    std::map<int, tools::Values> tool_values_;       // what has been entered, per tool
    std::unique_ptr<tools::Run> run_;                // the tool running now (one at a time)
    int last_run_tool_ = -1;                         // the tool whose run the output belongs to
    std::string last_output_, last_note_, last_result_;
    std::optional<std::vector<std::string>> modules_;   // the work folder's modules (read the first time a module tool needs them)
    bool last_done_ = false;
    int last_percent_ = -1;
    float phase_ = 0;
    wchar_t pending_high_ = 0;                       // the first half of a surrogate pair typed, waiting for its second
    settings::Theme theme_ = settings::Theme::System;
    config::Config settings_config_ = config::Config::load(config::current().checkout_root());

    // the queue of files (one file is a queue of one), Recent, the preview's files, the drag out, the taskbar and the notification
    batch::Queue queue_;
    int running_item_ = -1;                          // the queue's item the worker is on
    std::vector<recent::Entry> recent_;
    std::vector<preview::FileEntry> preview_entries_;
    std::vector<notify::Result> run_results_;        // how each file of this run ended (the notification says it)
    bool drag_pending_ = false, drag_out_active_ = false;
    POINT drag_origin_{};
    Taskbar taskbar_;
    std::unique_ptr<Notifier> notifier_;
    UINT taskbar_message_ = 0;
    std::string run_language_name_;                  // the language the run writes (empty: Original)

    // the conversion
    std::thread worker_;
    std::atomic<bool> cancel_{false};
    std::mutex lock_;
    std::optional<convert::Progress> latest_;
    std::vector<std::string> stream_waiting_;        // lines from the worker not yet handed to the stream (under lock_: latest_ keeps only the newest report)
    CharStream stream_;                              // the character stream (UI thread)
    std::chrono::steady_clock::time_point stream_tick_{};
    std::uint32_t stream_frames_ = 0;
    std::chrono::steady_clock::time_point fade_start_{}, welcome_start_{};   // the fades (fade_frame)
    bool card_fading_ = false;
    std::optional<convert::Outcome> outcome_;
    std::atomic<bool> progress_posted_{false};
    std::atomic<bool> running_{false};

    // the Board page: the thread that extracts or loads, what it hands over (under board_lock_), and each card by its file
    stdfs::path initial_board_;
    std::thread board_worker_;
    std::atomic<bool> board_cancel_{false}, board_posted_{false};
    std::mutex board_lock_;
    std::optional<board::ScanProgress> board_latest_;
    std::optional<board::Board> board_result_;
    std::optional<board::Index> board_result_index_;
    std::optional<board::Signals> board_result_signals_;
    std::string board_failure_;
    std::unordered_map<std::string, std::uint32_t> board_paths_;
    int board_press_card_ = -1;
    POINT board_press_at_{};
    bool board_tried_last_ = false;
    std::wstring board_title_ = L"CodeBreaker";
    bool flow_timer_on_ = false;
    std::chrono::steady_clock::time_point flow_tick_{};
    friend class WwDropTarget;
};

stdfs::path ww_boards_root();

WwApp* ww_app(HWND hwnd) { return reinterpret_cast<WwApp*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA)); }

// The files of a drag, when it holds files (all of them: several files are converted one after another).
std::vector<stdfs::path> ww_files(IDataObject* data) {
    FORMATETC fmt{CF_HDROP, nullptr, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
    STGMEDIUM medium{};
    std::vector<stdfs::path> result;
    if (data == nullptr || FAILED(data->GetData(&fmt, &medium))) return result;
    const HDROP drop = static_cast<HDROP>(GlobalLock(medium.hGlobal));
    if (drop != nullptr) {
        const UINT count = DragQueryFileW(drop, 0xFFFFFFFF, nullptr, 0);
        for (UINT i = 0; i < count; ++i) {
            wchar_t path[MAX_PATH * 4] = {};
            if (DragQueryFileW(drop, i, path, static_cast<UINT>(std::size(path))) > 0) result.emplace_back(path);
        }
        GlobalUnlock(medium.hGlobal);
    }
    ReleaseStgMedium(&medium);
    return result;
}

HRESULT WwDropTarget::DragEnter(IDataObject* data, DWORD, POINTL, DWORD* effect) {
    acceptable_ = app_->can_take_files() && !ww_files(data).empty();
    *effect = acceptable_ ? DROPEFFECT_COPY : DROPEFFECT_NONE;
    app_->drag_changed(acceptable_);
    return S_OK;
}
HRESULT WwDropTarget::DragOver(DWORD, POINTL, DWORD* effect) {
    *effect = acceptable_ ? DROPEFFECT_COPY : DROPEFFECT_NONE;
    return S_OK;
}
HRESULT WwDropTarget::DragLeave() {
    app_->drag_changed(false);
    return S_OK;
}
HRESULT WwDropTarget::Drop(IDataObject* data, DWORD, POINTL, DWORD* effect) {
    app_->drag_changed(false);
    *effect = DROPEFFECT_NONE;
    if (!acceptable_) return S_OK;
    const std::vector<stdfs::path> files = ww_files(data);
    if (!files.empty()) {
        *effect = DROPEFFECT_COPY;
        if (!app_->drop_on_tool(files) && !app_->drop_on_board(files)) app_->add_files(files);
    }
    return S_OK;
}

// A drop while the Tools page shows a tool: the first drop box that takes it gets it (two files on a two-input tool fill both). False: not for the page.
bool WwApp::drop_on_tool(const std::vector<stdfs::path>& files) {
    if (model_.page != Page::Tools || model_.tool_selected < 0) return false;
    const tools::Tool& tool = tools::all()[static_cast<std::size_t>(model_.tool_selected)];
    std::size_t next = 0;
    bool any = false;
    for (std::size_t i = 0; i < tool.inputs.size() && next < files.size(); ++i) {
        std::error_code ec;
        const bool folder = stdfs::is_directory(files[next], ec);
        if (!tools::accepts(tool.inputs[i], folder)) continue;
        set_tool_input(static_cast<int>(i), fs::to_text(files[next]));
        ++next;
        any = true;
    }
    if (!any) model_.status_text = tool.inputs.empty() ? tool.title + " takes nothing to drop" : "That is not what " + tool.title + " takes";
    invalidate();
    return true;
}

void WwApp::drag_changed(bool over) {
    if (model_.dragging == over) return;
    model_.dragging = over;
    invalidate();
}

bool WwApp::create_window() {
    WNDCLASSEXW wc{};
    wc.cbSize = sizeof wc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = &WwApp::proc;
    wc.hInstance = GetModuleHandleW(nullptr);
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hIcon = LoadIconW(wc.hInstance, MAKEINTRESOURCEW(1));
    wc.hIconSm = wc.hIcon;
    wc.lpszClassName = kWwClass;
    if (RegisterClassExW(&wc) == 0) return false;
    const UINT system_dpi = GetDpiForSystem();
    // the client area is the design's size; the window adds the system's side and bottom borders (the caption is ours)
    const int frame = GetSystemMetricsForDpi(SM_CXSIZEFRAME, system_dpi) + GetSystemMetricsForDpi(SM_CXPADDEDBORDER, system_dpi);
    const int w = static_cast<int>(kDefaultWidth * static_cast<float>(system_dpi) / 96.0f) + 2 * frame;
    const int h = static_cast<int>(kDefaultHeight * static_cast<float>(system_dpi) / 96.0f) + frame;
    // (no WS_CAPTION: the title bar is ours, so the system draws no caption and no caption buttons; the thick frame keeps the shadow, resizing and snapping)
    hwnd_ = CreateWindowExW(0, kWwClass, L"CodeBreaker", WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, w, h, nullptr, nullptr, wc.hInstance, this);
    return hwnd_ != nullptr;
}

int WwApp::run() {
    if (FAILED(OleInitialize(nullptr))) return 2;
    set_app_identity();   // (this process is "CodeBreaker" to the taskbar and the notification centre)
    taskbar_message_ = RegisterWindowMessageW(L"TaskbarButtonCreated");
    recent_ = recent::load(recent::store_file(convert::settings_file()));
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);   // (the manifest says it too; this covers a copy without it)
    if (!painter_.ok()) {
        err_text("codebreaker: Direct2D is not available\n");
        return 2;
    }
    theme_ = settings::remembered_theme();
    model_.developer_tools = settings::remembered_developer_tools();
    model_.platforms = convert::platform_summary();
    model_.drop_hint = convert::drop_hint();
    set_language_list(convert::targets_for({}));
    if (!create_window()) return 2;
    refresh_recent();
    SetTimer(hwnd_, kWwMinuteTimer, kWwMinuteMs, nullptr);
    SetTimer(hwnd_, kWwScoresTimer, kWwScoresMs, nullptr);
    if (initial_file_.empty()) start_welcome();   // (a window opened on a file goes straight to work)
    ShowWindow(hwnd_, SW_SHOWNORMAL);
    UpdateWindow(hwnd_);
    if (std::error_code ec; !initial_board_.empty() && stdfs::is_directory(initial_board_, ec)) open_board({}, initial_board_);   // (a board already kept)
    else if (!initial_board_.empty()) open_board(initial_board_, {});
    else if (!initial_file_.empty()) add_files({initial_file_});
    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    board_cancel_ = true;
    if (board_worker_.joinable()) board_worker_.join();   // (the scan stops between files; nothing is half-kept: a board is saved only when whole)
    if (running_) {   // closed with a conversion under way: ask it to stop, wait a little, then end the process (the staging folder is its own)
        cancel_ = true;
        for (int i = 0; i < 30 && running_; ++i) Sleep(100);
        if (worker_.joinable()) {
            if (running_) ExitProcess(0);
            worker_.join();
        }
    }
    // What holds COM objects lets go of them first: released after OleUninitialize, they call into code that has been unloaded.
    taskbar_.release();
    notifier_.reset();
    OleUninitialize();
    return static_cast<int>(msg.wParam);
}

void WwApp::apply_theme() {
    model_.dark = theme_ == settings::Theme::Dark || (theme_ == settings::Theme::System && ww_system_dark());
    look_.dark = model_.dark;
    look_.accent = ww_accent(model_.dark);
    look_.backdrop = backdrop_;
    const BOOL dark = model_.dark ? TRUE : FALSE;
    DwmSetWindowAttribute(hwnd_, kWwUseImmersiveDarkMode, &dark, sizeof dark);
}

void WwApp::relayout() {
    RECT rc{};
    GetClientRect(hwnd_, &rc);
    layout_ = compute_layout(model_.view, ww_dip(rc.right - rc.left, dpi_), ww_dip(rc.bottom - rc.top, dpi_), kRailWidth, extras_for(model_, model_.view));
}

void WwApp::make_target() {
    RECT rc{};
    GetClientRect(hwnd_, &rc);
    target_ = painter_.make_window_target(hwnd_, static_cast<UINT>(std::max<LONG>(1, rc.right - rc.left)), static_cast<UINT>(std::max<LONG>(1, rc.bottom - rc.top)));
}

void WwApp::paint() {
    PAINTSTRUCT ps;
    BeginPaint(hwnd_, &ps);
    if (!target_) make_target();
    if (target_) {
        target_->SetDpi(static_cast<float>(dpi_), static_cast<float>(dpi_));
        target_->BeginDraw();
        painter_.paint(target_.get(), model_, layout_, look_);
        if (target_->EndDraw() == D2DERR_RECREATE_TARGET) target_.reset();
    }
    EndPaint(hwnd_, &ps);
}

Control WwApp::control_at(LPARAM lp) const {
    return hit_test(layout_, model_.view, ww_dip(GET_X_LPARAM(lp), dpi_), ww_dip(GET_Y_LPARAM(lp), dpi_));
}

Control WwApp::control_at_screen(POINT screen) const {
    ScreenToClient(hwnd_, &screen);
    return hit_test(layout_, model_.view, ww_dip(screen.x, dpi_), ww_dip(screen.y, dpi_));
}

void WwApp::set_hot(Control c, int index) {
    if (model_.hot == c && model_.hot_index == index) return;
    model_.hot = c;
    model_.hot_index = index;
    invalidate();
}

void WwApp::move_focus(bool backwards) {
    std::vector<PageHit> order;
    if (model_.page == Page::Tools) {
        order = focus_order_tools(model_);
    } else if (model_.page == Page::Settings) {
        order = focus_order_settings(model_);
    } else if (model_.page == Page::Board) {
        order = focus_order_board(model_);
    } else {
        order = focus_order_convert(layout_, model_);
        for (const Control c : {Control::NavConvert, Control::NavTools, Control::NavBoard, Control::NavSettings}) order.push_back({c, -1});
    }
    if (order.empty()) return;
    std::size_t at = order.size();
    for (std::size_t i = 0; i < order.size(); ++i)
        if (order[i].control == model_.focus && (order[i].index == model_.focus_index || (model_.page == Page::Convert && order[i].index < 0))) at = i;
    std::size_t next = 0;
    if (at == order.size()) next = backwards ? order.size() - 1 : 0;
    else next = backwards ? (at + order.size() - 1) % order.size() : (at + 1) % order.size();
    set_focus(order[next].control, order[next].index);
    if (model_.page == Page::Settings && order[next].control != Control::NavConvert && order[next].index >= 0) {   // keep the field in view
        const SettingsLayout sl = compute_settings_layout(layout_, model_);
        const std::size_t f = static_cast<std::size_t>(order[next].index);
        const std::vector<Rect>& rows = order[next].control == Control::Locate ? sl.capability_rows : sl.field_rows;
        if (model_.settings_advanced ? f < sl.form.fields.size() : f < rows.size()) {
            const Rect& r = model_.settings_advanced ? sl.form.fields[f].input : rows[f];
            if (r.y < sl.fields_area.y) model_.setting_scroll = std::max(0.0f, model_.setting_scroll - (sl.fields_area.y - r.y) - 24);
            else if (r.bottom() > sl.fields_area.bottom()) model_.setting_scroll += r.bottom() - sl.fields_area.bottom() + 24;
        }
    }
    invalidate();
}

void WwApp::activate(Control c) {
    if (!control_enabled(c, model_.view)) return;
    switch (c) {
        case Control::Plus:
        case Control::Browse:
        case Control::TryAnother: browse(); break;
        case Control::Cancel: cancel_conversion(); break;
        case Control::OpenFolder: open_output(); break;
        case Control::CopyPath: copy_output_path(); break;
        case Control::Language: open_language_list(!model_.language_open); break;
        case Control::CaptionMinimize: PostMessageW(hwnd_, WM_SYSCOMMAND, SC_MINIMIZE, 0); break;
        case Control::CaptionMaximize: PostMessageW(hwnd_, WM_SYSCOMMAND, IsZoomed(hwnd_) ? SC_RESTORE : SC_MAXIMIZE, 0); break;
        case Control::CaptionClose: PostMessageW(hwnd_, WM_SYSCOMMAND, SC_CLOSE, 0); break;
        case Control::None: break;
    }
}

// The system file picker.
void WwApp::browse() {
    if (!can_take_files()) return;
    ComPtr<IFileOpenDialog> dialog;
    if (FAILED(CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(dialog.put())))) return;
    std::wstring programs;
    for (const std::string& e : convert::program_extensions()) programs += (programs.empty() ? L"*" : L";*") + widen(e);
    const COMDLG_FILTERSPEC types[] = {{L"Programs and images", programs.c_str()}, {L"All files", L"*.*"}};
    dialog->SetFileTypes(static_cast<UINT>(std::size(types)), types);
    dialog->SetTitle(L"Choose a program");
    DWORD options = 0;
    dialog->GetOptions(&options);
    dialog->SetOptions(options | FOS_FORCEFILESYSTEM | FOS_FILEMUSTEXIST | FOS_PATHMUSTEXIST | FOS_ALLOWMULTISELECT);   // (several files are converted one after another)
    if (FAILED(dialog->Show(hwnd_))) return;
    ComPtr<IShellItemArray> chosen;
    if (FAILED(dialog->GetResults(chosen.put()))) return;
    DWORD count = 0;
    if (FAILED(chosen->GetCount(&count))) return;
    std::vector<stdfs::path> files;
    for (DWORD i = 0; i < count; ++i) {
        ComPtr<IShellItem> item;
        if (FAILED(chosen->GetItemAt(i, item.put()))) continue;
        PWSTR path = nullptr;
        if (SUCCEEDED(item->GetDisplayName(SIGDN_FILESYSPATH, &path)) && path != nullptr) {
            files.emplace_back(path);
            CoTaskMemFree(path);
        }
    }
    add_files(files);
}

// The system menu (Restore, Move, Size, Minimize, Maximize, Close) where a click on the icon or a right click on the title bar asks for it. The window has
// no WS_SYSMENU (that would make the system draw its caption buttons over ours), so the menu is shown by hand.
void WwApp::show_system_menu(POINT screen) {
    const HMENU menu = GetSystemMenu(hwnd_, FALSE);
    if (menu == nullptr) return;
    const bool zoomed = IsZoomed(hwnd_) != FALSE;
    EnableMenuItem(menu, SC_RESTORE, MF_BYCOMMAND | (zoomed ? MF_ENABLED : MF_GRAYED));
    EnableMenuItem(menu, SC_MAXIMIZE, MF_BYCOMMAND | (zoomed ? MF_GRAYED : MF_ENABLED));
    EnableMenuItem(menu, SC_SIZE, MF_BYCOMMAND | (zoomed ? MF_GRAYED : MF_ENABLED));
    EnableMenuItem(menu, SC_MOVE, MF_BYCOMMAND | (zoomed ? MF_GRAYED : MF_ENABLED));
    const UINT command = static_cast<UINT>(TrackPopupMenuEx(menu, TPM_RETURNCMD | TPM_LEFTALIGN | TPM_TOPALIGN, screen.x, screen.y, hwnd_, nullptr));
    if (command != 0) PostMessageW(hwnd_, WM_SYSCOMMAND, command, 0);
}

void WwApp::open_output() {
    if (model_.output_path.empty()) return;
    ShellExecuteW(hwnd_, L"open", widen(model_.output_path).c_str(), nullptr, nullptr, SW_SHOWNORMAL);
}

void WwApp::copy_output_path() {
    if (model_.output_path.empty() || !OpenClipboard(hwnd_)) return;
    const std::wstring text = widen(model_.output_path);
    EmptyClipboard();
    const HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, (text.size() + 1) * sizeof(wchar_t));
    if (mem != nullptr) {
        void* p = GlobalLock(mem);
        std::memcpy(p, text.c_str(), (text.size() + 1) * sizeof(wchar_t));
        GlobalUnlock(mem);
        if (SetClipboardData(CF_UNICODETEXT, mem) == nullptr) GlobalFree(mem);
    }
    CloseClipboard();
}

// The list the translator gives for the file now chosen; the language stays the one chosen when the list still has it.
void WwApp::set_language_list(const std::vector<translate::capability::Target>& targets) {
    const std::string current = model_.languages.empty() ? convert::remembered_target() : model_.languages[static_cast<std::size_t>(model_.language_selected)].id;
    model_.languages = ww_items(targets);
    model_.language_selected = ww_index_of(model_.languages, current);
    model_.language_open = false;
    model_.language_hot = -1;
}

void WwApp::open_language_list(bool open) {
    if (open && (!control_enabled(Control::Language, model_.view) || model_.languages.empty())) return;
    model_.language_open = open;
    model_.language_hot = open ? model_.language_selected : -1;
    if (open) model_.focus = Control::Language;
    invalidate();
}

// The person chose a language: it is used from the next run and remembered for the next time the window starts.
void WwApp::choose_language(int index) {
    if (index < 0 || index >= static_cast<int>(model_.languages.size()) || !model_.languages[static_cast<std::size_t>(index)].supported) return;
    model_.language_selected = index;
    convert::remember_target(model_.languages[static_cast<std::size_t>(index)].id);
    invalidate();
}

// The keys of the Language control: Alt+Down or F4 opens it; in the list the arrows, Home and End move over the languages that can be chosen, Enter or
// Space chooses, Esc closes; closed, Up and Down change the language directly (as a ComboBox does).
void WwApp::language_key(WPARAM key, bool alt) {
    const auto& items = model_.languages;
    if (!model_.language_open) {
        if (key == VK_F4 || (alt && (key == VK_DOWN || key == VK_UP))) {
            open_language_list(true);
        } else if (key == VK_DOWN || key == VK_UP) {
            if (control_enabled(Control::Language, model_.view)) choose_language(language_next_enabled(items, model_.language_selected, key == VK_DOWN ? 1 : -1));
        }
        return;
    }
    switch (key) {
        case VK_DOWN: model_.language_hot = language_next_enabled(items, model_.language_hot, 1); break;
        case VK_UP: model_.language_hot = language_next_enabled(items, model_.language_hot, -1); break;
        case VK_HOME: model_.language_hot = language_next_enabled(items, -1, 1); break;
        case VK_END: model_.language_hot = language_next_enabled(items, static_cast<int>(items.size()), -1); break;
        case VK_RETURN:
        case VK_SPACE:
            choose_language(model_.language_hot);
            open_language_list(false);
            break;
        case VK_ESCAPE:
        case VK_F4: open_language_list(false); break;
        default: break;
    }
    invalidate();
}


// ---- the rail, the Tools page and the Settings page -------------------------------------------------------------------------------------

PageHit WwApp::hit_at(float x, float y) const {
    if (layout_.rail.contains(x, y)) return hit_test_rail(layout_, x, y);
    switch (model_.page) {
        case Page::Tools: return hit_test_tools(compute_tools_layout(layout_, model_), model_, x, y);
        case Page::Settings: return hit_test_settings(compute_settings_layout(layout_, model_), model_, x, y);
        case Page::Board: return hit_test_board(compute_board_layout(layout_, model_), model_, x, y);
        default: return hit_test_convert(layout_, model_, x, y);
    }
}

PageHit WwApp::hit_at(LPARAM lp) const { return hit_at(ww_dip(GET_X_LPARAM(lp), dpi_), ww_dip(GET_Y_LPARAM(lp), dpi_)); }

void WwApp::set_page(Page page) {
    if (model_.page == page) return;
    blur();
    model_.field_popup = -1;
    model_.language_open = false;
    model_.page = page;
    sync_board_title();
    model_.focus = Control::None;
    model_.focus_index = -1;
    if (page == Page::Tools && model_.tool_rows.empty()) ww_fill_tool_rows(model_);
    if (page == Page::Settings) {
        model_.settings_advanced = false;
        fill_settings();
    }
    if (page == Page::Board && !model_.board && !model_.board_busy && !board_tried_last_) {   // the board open last comes back as it was, with no extraction
        board_tried_last_ = true;
        try {
            const stdfs::path last = ww_boards_root() / "last.txt";
            std::error_code ec;
            if (stdfs::is_regular_file(last, ec)) open_board({}, fs::from_text(text::trim(fs::read_text(last))));
        } catch (const std::exception&) {}
    }
    relayout();
    invalidate();
}

// ---- text entry

std::string* WwApp::edit_target() {
    if (model_.focus == Control::ToolSearch) return &model_.tool_query;
    if (model_.focus == Control::BoardSearch) return &model_.board_query;
    if (model_.focus != Control::Field || model_.focus_index < 0) return nullptr;
    std::vector<FieldItem>& fields = model_.page == Page::Settings ? model_.setting_fields : model_.tool_fields;
    if (static_cast<std::size_t>(model_.focus_index) >= fields.size()) return nullptr;
    FieldItem& f = fields[static_cast<std::size_t>(model_.focus_index)];
    return f.kind == 0 || f.kind == 1 || f.kind == 2 || f.kind == 6 ? &f.text : nullptr;
}

void WwApp::set_focus(Control c, int index) {
    if (model_.focus != c || model_.focus_index != index) blur();
    model_.focus = c;
    model_.focus_index = index;
    if (const std::string* text = edit_target()) model_.caret = static_cast<int>(text->size());
    invalidate();
}

// Something the person typed or chose in a field of the selected tool, or in the search box: the page follows.
void WwApp::edited() {
    if (model_.page == Page::Board) {
        refresh_board();
        return;
    }
    if (model_.page == Page::Tools) {
        if (model_.focus == Control::ToolSearch) {
            ww_fill_tool_rows(model_);
            model_.tool_scroll = 0;
        } else if (model_.tool_selected >= 0) {
            tools::Values& v = tool_values_[model_.tool_selected];
            for (std::size_t i = 0; i < model_.tool_fields.size() && i < v.inputs.size(); ++i) v.inputs[i] = model_.tool_fields[i].text;
            v.choice = model_.tool_choice;
            model_.tool_can_run = !tools::missing(tools::all()[static_cast<std::size_t>(model_.tool_selected)], v).has_value();
            // a module field: what has been typed searches the work folder's modules, and the matches drop down under it
            if (model_.focus == Control::Field && model_.focus_index >= 0 && static_cast<std::size_t>(model_.focus_index) < model_.tool_fields.size()) {
                FieldItem& f = model_.tool_fields[static_cast<std::size_t>(model_.focus_index)];
                if (f.kind == 6) {
                    f.choices = tools::filter_modules(work_modules(), f.text, 8);
                    if (f.choices.size() == 1 && text::lower(f.choices.front()) == text::lower(f.text)) f.choices.clear();   // (it is chosen)
                    model_.field_popup = f.choices.empty() ? -1 : model_.focus_index;
                    model_.language_hot = 0;
                    if (f.choices.empty() && work_modules().empty()) model_.status_text = "No modules: the work folder has none (Settings)";
                }
            }
        }
    }
    invalidate();
}

// A setting changed on the Settings page is written to settings.json when the field is left, a switch or a choice when it is changed.
void WwApp::commit_setting(int field) {
    if (model_.page != Page::Settings || field < 0 || static_cast<std::size_t>(field) >= model_.setting_fields.size()) return;
    const FieldItem& f = model_.setting_fields[static_cast<std::size_t>(field)];
    if (static_cast<std::size_t>(field) >= model_.setting_keys.size()) return;
    const std::string& key = model_.setting_keys[static_cast<std::size_t>(field)];
    if (key == kWwDeveloperKey) {   // Show developer tools: the list follows at once
        model_.developer_tools = !f.text.empty();
        settings::remember_developer_tools(model_.developer_tools);
        if (!model_.developer_tools && model_.tool_selected >= 0 && tools::all()[static_cast<std::size_t>(model_.tool_selected)].developer) {
            model_.tool_selected = -1;
            model_.tool_fields.clear();
        }
        ww_fill_tool_rows(model_);
        model_.status_text = model_.developer_tools ? "Developer tools are shown" : "Developer tools are hidden";
        return;
    }
    if (key.empty()) {   // the theme
        theme_ = f.text == "Light" ? settings::Theme::Light : f.text == "Dark" ? settings::Theme::Dark : settings::Theme::System;
        settings::remember_theme(theme_);
        apply_theme();
        return;
    }
    const std::string error = settings::write(settings_config_, key, f.text, f.kind == 4 ? settings::Kind::Toggle : f.kind == 0 ? settings::Kind::Path : settings::Kind::Text);
    model_.status_text = error.empty() ? "Saved: " + f.label : "Could not save " + f.label + ": " + error;
}

// The Settings page read again from settings.json (after Locate..., or moving between the main view and Advanced).
void WwApp::fill_settings() {
    settings_config_ = config::Config::load(config::current().checkout_root());
    ww_fill_settings(model_, settings::rows(settings_config_), theme_);
    model_.setting_scroll = 0;
}

void WwApp::open_advanced(bool open) {
    blur();
    model_.field_popup = -1;
    model_.settings_advanced = open;
    fill_settings();
    model_.focus = open ? Control::SettingsBack : Control::SettingsAdvanced;
    model_.focus_index = -1;
    invalidate();
}

// Locate... on a row of what this computer cannot convert yet: the tool's file or folder is written to its setting, and the rows are read again.
void WwApp::locate_tool(int index) {
    if (index < 0 || static_cast<std::size_t>(index) >= model_.capabilities.size()) return;
    const CapabilityItem c = model_.capabilities[static_cast<std::size_t>(index)];
    const std::string chosen = ww_choose_path(hwnd_, c.folder);
    if (chosen.empty()) return;
    const std::string error = settings::write(settings_config_, c.key, chosen, settings::Kind::Path);
    fill_settings();
    model_.status_text = error.empty() ? "Saved: " + settings::plain_label(c.key) : "Could not save " + settings::plain_label(c.key) + ": " + error;
    invalidate();
}

void WwApp::blur() {
    if (model_.page == Page::Settings && model_.focus == Control::Field && model_.focus_index >= 0 &&
        static_cast<std::size_t>(model_.focus_index) < model_.setting_keys.size() && !model_.setting_keys[static_cast<std::size_t>(model_.focus_index)].empty())
        commit_setting(model_.focus_index);
}

void WwApp::insert_text(const std::string& utf8) {
    std::string* text = edit_target();
    if (text == nullptr || utf8.empty()) return;
    const std::size_t at = std::min(static_cast<std::size_t>(std::max(0, model_.caret)), text->size());
    text->insert(at, utf8);
    model_.caret = static_cast<int>(at + utf8.size());
    edited();
}

// Editing keys for the focused text box. True when the key was one.
bool WwApp::edit_key(WPARAM key, bool ctrl) {
    std::string* text = edit_target();
    if (text == nullptr) return false;
    std::size_t at = std::min(static_cast<std::size_t>(std::max(0, model_.caret)), text->size());
    switch (key) {
        case VK_BACK:
            if (at > 0) {
                const std::size_t from = ww_prev_char(*text, at);
                text->erase(from, at - from);
                model_.caret = static_cast<int>(from);
                edited();
            }
            return true;
        case VK_DELETE:
            if (at < text->size()) {
                text->erase(at, ww_next_char(*text, at) - at);
                edited();
            }
            return true;
        case VK_LEFT: model_.caret = static_cast<int>(ww_prev_char(*text, at)); invalidate(); return true;
        case VK_RIGHT: model_.caret = static_cast<int>(ww_next_char(*text, at)); invalidate(); return true;
        case VK_HOME: model_.caret = 0; invalidate(); return true;
        case VK_END: model_.caret = static_cast<int>(text->size()); invalidate(); return true;
        default: break;
    }
    if (ctrl && key == 'V' && OpenClipboard(hwnd_)) {   // paste (one line)
        if (const HANDLE h = GetClipboardData(CF_UNICODETEXT)) {
            if (const auto* w = static_cast<const wchar_t*>(GlobalLock(h))) {
                std::string pasted = narrow(w);
                GlobalUnlock(h);
                for (char& c : pasted)
                    if (c == '\r' || c == '\n' || c == '\t') c = ' ';
                CloseClipboard();
                insert_text(pasted);
                return true;
            }
        }
        CloseClipboard();
        return true;
    }
    if (ctrl && key == 'A') {   // (no selection: select all clears the box, as a reset)
        return true;
    }
    return false;
}

// ---- the tools

void WwApp::select_tool(int tool) {
    blur();
    model_.field_popup = -1;
    if (tool >= 0 && !tool_values_.contains(tool)) tool_values_[tool] = tools::empty_values(tools::all()[static_cast<std::size_t>(tool)]);
    ww_load_tool(model_, tool, tool >= 0 ? tool_values_[tool] : tools::Values{});
    const bool this_run = tool >= 0 && tool == last_run_tool_;   // the output, bar and note belong to the tool that ran
    model_.tool_output = this_run ? last_output_ : std::string();
    model_.tool_note = this_run ? last_note_ : std::string();
    model_.tool_done = this_run && last_done_;
    model_.tool_running = this_run && run_ != nullptr;
    model_.tool_percent = this_run ? last_percent_ : -1;
    model_.tool_result = this_run ? last_result_ : std::string();
    model_.output_scroll = 0;
    model_.focus = Control::ToolRow;
    model_.focus_index = tool;
    relayout();
    invalidate();
}

void WwApp::run_tool() {
    if (run_ != nullptr || model_.tool_selected < 0) return;
    const tools::Tool& tool = tools::all()[static_cast<std::size_t>(model_.tool_selected)];
    const tools::Values& values = tool_values_[model_.tool_selected];
    if (tools::missing(tool, values)) return;
    run_ = std::make_unique<tools::Run>(tools::build_argv(tool, values));
    last_run_tool_ = model_.tool_selected;
    last_output_.clear();
    last_note_ = "Running…";
    last_done_ = false;
    last_percent_ = -1;
    last_result_.clear();
    model_.tool_result.clear();
    model_.tool_running = true;
    model_.tool_output.clear();
    model_.tool_note = last_note_;
    model_.tool_done = false;
    model_.tool_percent = -1;
    model_.output_scroll = 0;
    model_.activity = true;
    model_.status_text = "Running " + tool.title;
    SetTimer(hwnd_, kWwRunTimer, 80, nullptr);
    relayout();
    invalidate();
}

void WwApp::stop_tool() {
    if (run_ != nullptr) run_->stop();   // (the child this window started, and only it)
}

// The timer while a tool runs: its output and percentage into the page, and the end of the run.
void WwApp::poll_tool() {
    if (run_ == nullptr) {
        KillTimer(hwnd_, kWwRunTimer);
        return;
    }
    const tools::RunState s = run_->state();
    last_output_ = s.failure.empty() ? s.output : "Could not start: " + s.failure + "\n";
    last_percent_ = s.percent;
    phase_ = std::fmod(phase_ + 0.045f, 2.0f);
    if (!s.running) {
        last_done_ = true;
        last_note_ = !s.failure.empty() ? "Could not start" : s.stopped ? "Stopped" : s.exit_code == 0 ? "Finished" : "Exit code " + std::to_string(s.exit_code);
        last_result_ = s.failure.empty() && !s.stopped ? fs::to_text(tools::result_folder(s.output)) : std::string();
        run_.reset();
        KillTimer(hwnd_, kWwRunTimer);
        model_.activity = false;
        model_.status_text = last_note_ == "Finished" ? "Done" : last_note_;
    }
    if (last_run_tool_ == model_.tool_selected) {
        model_.tool_output = last_output_;
        model_.tool_percent = last_percent_;
        model_.tool_note = last_note_;
        model_.tool_done = last_done_;
        model_.tool_running = run_ != nullptr;
        model_.tool_result = last_result_;
        model_.tool_phase = phase_ <= 1.0f ? phase_ : 2.0f - phase_;
        model_.output_scroll = 0;
    }
    invalidate();
}

// ---- choosing a file or a folder for a field

void WwApp::browse_field(int index, bool folder_wanted) {
    std::vector<FieldItem>& fields = model_.page == Page::Settings ? model_.setting_fields : model_.tool_fields;
    if (index < 0 || static_cast<std::size_t>(index) >= fields.size()) return;
    FieldItem& f = fields[static_cast<std::size_t>(index)];
    bool folder = f.kind == 1 || folder_wanted || (f.kind == 5 && f.accept == 1);
    if (!folder && model_.page == Page::Settings) {   // a path setting that names a folder now is chosen as one
        std::error_code ec;
        folder = !f.text.empty() && stdfs::is_directory(fs::from_text(f.text), ec);
    }
    ComPtr<IFileOpenDialog> dialog;
    if (FAILED(CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(dialog.put())))) return;
    DWORD options = 0;
    dialog->GetOptions(&options);
    dialog->SetOptions(options | FOS_FORCEFILESYSTEM | FOS_PATHMUSTEXIST | (folder ? FOS_PICKFOLDERS : FOS_FILEMUSTEXIST));
    dialog->SetTitle(widen(folder ? "Choose a folder" : "Choose a file").c_str());
    if (FAILED(dialog->Show(hwnd_))) return;
    ComPtr<IShellItem> item;
    if (FAILED(dialog->GetResult(item.put()))) return;
    PWSTR path = nullptr;
    if (SUCCEEDED(item->GetDisplayName(SIGDN_FILESYSPATH, &path)) && path != nullptr) {
        const std::string chosen = narrow(path);
        CoTaskMemFree(path);
        if (f.kind == 5) {
            set_tool_input(index, chosen);
            return;
        }
        std::string& text = f.text;
        text = chosen;
        model_.focus = Control::Field;
        model_.focus_index = index;
        model_.caret = static_cast<int>(text.size());
        edited();
        if (model_.page == Page::Settings) commit_setting(index);
    }
}

// A file or folder for a drop box of the selected tool (browsed or dropped): it is shown, and Run follows.
void WwApp::set_tool_input(int field, const std::string& path) {
    if (model_.page != Page::Tools || field < 0 || static_cast<std::size_t>(field) >= model_.tool_fields.size()) return;
    FieldItem& f = model_.tool_fields[static_cast<std::size_t>(field)];
    f.text = path;
    std::error_code ec;
    f.note = stdfs::is_directory(fs::from_text(path), ec) ? "folder" : "";
    const Control keep = model_.focus;
    const int keep_index = model_.focus_index;
    model_.focus = Control::FieldBrowse;
    model_.focus_index = field;
    edited();
    if (keep == Control::FieldBrowseFolder) model_.focus = keep, model_.focus_index = keep_index;
    invalidate();
}

// Open folder: the folder a finished run left its result in, in Explorer.
void WwApp::open_result() {
    if (model_.tool_result.empty()) return;
    ShellExecuteW(hwnd_, L"open", widen(model_.tool_result).c_str(), nullptr, nullptr, SW_SHOWNORMAL);
}

// The work folder's modules, for a module field: the program files of the selected corpus (none when there is no work folder).
const std::vector<std::string>& WwApp::work_modules() {
    if (!modules_) {
        try {
            base::Corpus corpus(config::current().products());
            modules_ = tools::modules_in(corpus.root());
        } catch (const std::exception&) {
            modules_ = std::vector<std::string>{};
        }
    }
    return *modules_;
}

// ---- a form's drop-down

std::vector<LanguageItem> WwApp::field_popup_items() const {
    std::vector<LanguageItem> items;
    const std::vector<FieldItem>& fields = model_.page == Page::Settings ? model_.setting_fields : model_.tool_fields;
    if (model_.field_popup >= 0 && static_cast<std::size_t>(model_.field_popup) < fields.size())
        for (const std::string& c : fields[static_cast<std::size_t>(model_.field_popup)].choices) items.push_back({c, c, true, {}});
    return items;
}

LanguageList WwApp::field_popup_list() const {
    const std::vector<LanguageItem> items = field_popup_items();
    const FormLayout form = model_.page == Page::Settings ? compute_settings_layout(layout_, model_).form : compute_tools_layout(layout_, model_).form;
    if (model_.field_popup < 0 || static_cast<std::size_t>(model_.field_popup) >= form.fields.size()) return {};
    return popup_list_layout(form.fields[static_cast<std::size_t>(model_.field_popup)].input, layout_.status_bar.y - 8, kTitleBarHeight + 8, items);
}

void WwApp::open_field_popup(int index) {
    std::vector<FieldItem>& fields = model_.page == Page::Settings ? model_.setting_fields : model_.tool_fields;
    if (index < 0 || static_cast<std::size_t>(index) >= fields.size()) return;
    model_.field_popup = index;
    model_.focus = Control::Field;
    model_.focus_index = index;
    const FieldItem& f = fields[static_cast<std::size_t>(index)];
    model_.language_hot = 0;
    for (std::size_t i = 0; i < f.choices.size(); ++i)
        if (f.choices[i] == f.text) model_.language_hot = static_cast<int>(i);
    invalidate();
}

void WwApp::choose_in_popup(int item) {
    const std::vector<LanguageItem> items = field_popup_items();
    const int field = model_.field_popup;
    model_.field_popup = -1;
    if (item >= 0 && static_cast<std::size_t>(item) < items.size()) {
        std::vector<FieldItem>& fields = model_.page == Page::Settings ? model_.setting_fields : model_.tool_fields;
        fields[static_cast<std::size_t>(field)].text = items[static_cast<std::size_t>(item)].id;
        edited();
        if (model_.page == Page::Settings) commit_setting(field);
    }
    invalidate();
}

// Keys while a form's drop-down is open.
void WwApp::popup_key(WPARAM key) {
    const std::vector<LanguageItem> items = field_popup_items();
    switch (key) {
        case VK_DOWN: model_.language_hot = language_next_enabled(items, model_.language_hot, 1); break;
        case VK_UP: model_.language_hot = language_next_enabled(items, model_.language_hot, -1); break;
        case VK_HOME: model_.language_hot = language_next_enabled(items, -1, 1); break;
        case VK_END: model_.language_hot = language_next_enabled(items, static_cast<int>(items.size()), -1); break;
        case VK_RETURN:
        case VK_SPACE: choose_in_popup(model_.language_hot); break;
        case VK_ESCAPE:
        case VK_F4: model_.field_popup = -1; break;
        default: break;
    }
    invalidate();
}

// ---- the Board page -------------------------------------------------------------------------------------------------------------------------

// The folder boards are kept in, and the file naming the one open last.
stdfs::path ww_boards_root() { return config::current().state("boards"); }

bool ww_is_disc_image(const stdfs::path& file) {
    const std::string ext = text::lower(fs::to_text(file.extension()));
    return ext == ".iso" || ext == ".img" || ext == ".ima" || ext == ".vfd" || ext == ".bin";
}

// A drop on the Board page is for the board: a disc image opens, anything else is said to be no image.
bool WwApp::drop_on_board(const std::vector<stdfs::path>& files) {
    if (model_.page != Page::Board) return false;
    if (files.size() == 1 && ww_is_disc_image(files[0])) open_board(files[0], {});
    else model_.status_text = "The board takes one disc image (.iso, .img)";
    invalidate();
    return true;
}

void WwApp::browse_board() {
    if (model_.board_busy) return;
    ComPtr<IFileOpenDialog> dialog;
    if (FAILED(CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(dialog.put())))) return;
    const COMDLG_FILTERSPEC types[] = {{L"Disc images", L"*.iso;*.img;*.ima;*.vfd;*.bin"}, {L"All files", L"*.*"}};
    dialog->SetFileTypes(static_cast<UINT>(std::size(types)), types);
    dialog->SetTitle(L"Choose a disc image");
    DWORD options = 0;
    dialog->GetOptions(&options);
    dialog->SetOptions(options | FOS_FORCEFILESYSTEM | FOS_FILEMUSTEXIST | FOS_PATHMUSTEXIST);
    if (FAILED(dialog->Show(hwnd_))) return;
    ComPtr<IShellItem> item;
    if (FAILED(dialog->GetResult(item.put()))) return;
    PWSTR path = nullptr;
    if (FAILED(item->GetDisplayName(SIGDN_FILESYSPATH, &path)) || path == nullptr) return;
    const stdfs::path image(path);
    CoTaskMemFree(path);
    open_board(image, {});
}

// Reads the kept board of `image` (or the board at `kept`), or extracts the image, on a thread of its own.
void WwApp::open_board(const stdfs::path& image, const stdfs::path& kept) {
    if (model_.board_busy) return;
    board_tried_last_ = true;   // (set_page below must not start a second load of the board opened last)
    if (board_worker_.joinable()) board_worker_.join();
    stdfs::path root = kept;
    try {
        if (root.empty()) root = board::board_folder(ww_boards_root(), image);
    } catch (const std::exception& e) {
        model_.board_error = e.what();
        invalidate();
        return;
    }
    set_page(Page::Board);
    model_.board_busy = true;
    model_.board_error.clear();
    model_.board_percent = 0;
    model_.board_stage = "Opening";
    model_.board_detail = fs::to_text(image.filename());
    model_.status_text = "Laying out " + model_.board_detail;
    board_cancel_ = false;
    HWND window = hwnd_;
    board_worker_ = std::thread([this, window, image, root] {
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);
        std::optional<board::Board> result;
        std::optional<board::Index> index;
        std::optional<board::Signals> signals;
        std::string failure;
        try {
            result = board::load(root);
            if (!result && !image.empty()) {
                result = board::scan(image, root, [this, window](const board::ScanProgress& p) {
                    {
                        std::lock_guard hold(board_lock_);
                        board_latest_ = p;
                    }
                    if (!board_posted_.exchange(true)) PostMessageW(window, kWmBoardProgress, 0, 0);
                }, board_cancel_);
                if (result->complete) board::save(*result);
            }
            if (result && result->complete) {   // how the files link: read once, then kept beside the board
                index = board::load_index(*result);
                if (!index) {
                    index = board::build_index(*result, [this, window](const board::IndexProgress& p) {
                        board::ScanProgress shown;
                        shown.stage = "Linking files: " + p.stage;
                        shown.detail = board::count_words(p.done) + " of " + board::count_words(p.total);
                        shown.percent = p.total == 0 ? 0 : static_cast<int>(p.done * 100 / p.total);
                        {
                            std::lock_guard hold(board_lock_);
                            board_latest_ = shown;
                        }
                        if (!board_posted_.exchange(true)) PostMessageW(window, kWmBoardProgress, 0, 0);
                    }, board_cancel_);
                    if (index) board::save_index(*result, *index);
                }
                if (index) {   // the channels the files share: read once, then kept too
                    signals = board::load_signals(*result);
                    if (!signals) {
                        signals = board::build_signals(*result, *index, [this, window](const board::IndexProgress& p) {
                            board::ScanProgress shown;
                            shown.stage = p.stage;
                            shown.detail = board::count_words(p.done) + " of " + board::count_words(p.total);
                            shown.percent = p.total == 0 ? 0 : static_cast<int>(p.done * 100 / p.total);
                            {
                                std::lock_guard hold(board_lock_);
                                board_latest_ = shown;
                            }
                            if (!board_posted_.exchange(true)) PostMessageW(window, kWmBoardProgress, 0, 0);
                        }, board_cancel_);
                        if (signals) board::save_signals(*result, *signals);
                    }
                }
            }
        } catch (const std::exception& e) {
            failure = e.what();
        }
        {
            std::lock_guard hold(board_lock_);
            board_result_ = std::move(result);
            board_result_index_ = std::move(index);
            board_result_signals_ = std::move(signals);
            board_failure_ = std::move(failure);
        }
        PostMessageW(window, kWmBoardDone, 0, 0);
    });
    relayout();
    invalidate();
}

void WwApp::on_board_progress() {
    std::optional<board::ScanProgress> p;
    {
        std::lock_guard hold(board_lock_);
        p = std::move(board_latest_);
        board_latest_.reset();
    }
    board_posted_ = false;
    if (!p || !model_.board_busy) return;
    model_.board_stage = p->stage;
    model_.board_detail = p->detail;
    model_.board_percent = p->percent;
    invalidate();
}

void WwApp::on_board_done() {
    if (board_worker_.joinable()) board_worker_.join();
    std::optional<board::Board> result;
    std::optional<board::Index> index;
    std::optional<board::Signals> signals;
    std::string failure;
    {
        std::lock_guard hold(board_lock_);
        result = std::move(board_result_);
        board_result_.reset();
        index = std::move(board_result_index_);
        board_result_index_.reset();
        signals = std::move(board_result_signals_);
        board_result_signals_.reset();
        failure = std::move(board_failure_);
    }
    model_.board_busy = false;
    board_posted_ = false;
    if (!result || !result->complete) {
        model_.board_error = !failure.empty() ? failure : result ? std::string() : std::string("There is no board there.");
        model_.status_text = !failure.empty() ? "The image could not be read" : "Cancelled";
        relayout();
        invalidate();
        return;
    }
    model_.board = std::make_shared<board::Board>(std::move(*result));
    model_.board_selected = -1;
    model_.board_hscroll = 0;
    model_.board_scroll = {};
    model_.board_expanded = {};
    model_.board_show_done = {};
    model_.board_filters.clear();
    model_.board_orphans.reset();
    model_.board_orphans_only = false;
    model_.board_decks_open = false;
    model_.board_batch_armed = 0;
    model_.board_signal_type = -1;
    model_.board_signal_rows.clear();
    model_.board_view = 0;
    model_.board_focus = -1;
    model_.board_index = index ? std::make_shared<const board::Index>(std::move(*index)) : nullptr;
    model_.board_signals = signals ? std::make_shared<const board::Signals>(std::move(*signals)) : nullptr;
    model_.board_graph.reset();
    model_.board_flow.reset();
    model_.board_trace.reset();
    model_.board_library.reset();
    model_.board_inspecting = false;
    model_.board_inspector.reset();
    model_.board_hover.reset();
    model_.board_hover_linked = {};
    model_.board_projection.reset();
    model_.board_crumbs.clear();
    model_.board_function.clear();
    model_.board_hover_export = -1;
    board_paths_.clear();
    board_paths_.reserve(model_.board->cards.size());
    for (std::size_t i = 0; i < model_.board->cards.size(); ++i)
        board_paths_.emplace(text::lower(fs::to_text(model_.board->file_of(model_.board->cards[i]))), static_cast<std::uint32_t>(i));
    try {
        fs::write_text(ww_boards_root() / "last.txt", fs::to_text(model_.board->root));
    } catch (const std::exception&) {}   // (only what opens by itself next time)
    model_.status_text = board::count_words(model_.board->cards.size()) + " files on the board";
    board_load_decks(model_);
    if (model_.board_scores) model_.status_text += " · " + board::c_score_words(*model_.board_scores);
    refresh_board();
}

void WwApp::refresh_board() {
    board_rebuild_stacks(model_);
    relayout();
    invalidate();
}

// A card goes to the convert queue. It reaches its foundation only if the conversion ends Done (board_sync_states).
void WwApp::board_convert(int index) {
    if (!model_.board || index < 0 || static_cast<std::size_t>(index) >= model_.board->cards.size()) return;
    board::Card& card = model_.board->cards[static_cast<std::size_t>(index)];
    if (card.conv == board::Conv::Done) {
        model_.status_text = card.name + " is already converted";
    } else if (card.conv == board::Conv::Queued || card.conv == board::Conv::Running) {
        model_.status_text = card.name + " is already on its way";
    } else if (!can_take_files()) {
        model_.status_text = "Wait for the cancelled run to stop";
    } else {
        card.conv = board::Conv::Queued;
        card.note.clear();
        add_files({model_.board->file_of(card)});
        board_sync_states();
    }
    invalidate();
}

void WwApp::board_open_card(int index) {
    if (!model_.board || index < 0 || static_cast<std::size_t>(index) >= model_.board->cards.size()) return;
    const board::Card& card = model_.board->cards[static_cast<std::size_t>(index)];
    if (card.conv == board::Conv::Done) {
        ShellExecuteW(hwnd_, L"open", fs::from_text(card.output).c_str(), nullptr, nullptr, SW_SHOWNORMAL);
        return;
    }
    const std::wstring select = L"/select,\"" + stdfs::path(model_.board->file_of(card)).make_preferred().wstring() + L"\"";
    ShellExecuteW(hwnd_, L"open", L"explorer.exe", select.c_str(), nullptr, SW_SHOWNORMAL);
}

// The cards the board sent to the queue follow it. Only what the queue really says moves a card, and a Done needs its output folder (board::record).
void WwApp::board_sync_states() {
    if (!model_.board) return;
    bool moved = false, ended = false;
    for (const batch::Item& item : queue_.items()) {
        const auto it = board_paths_.find(text::lower(fs::to_text(item.file)));
        if (it == board_paths_.end()) continue;
        board::Card& card = model_.board->cards[it->second];
        if (card.conv != board::Conv::Queued && card.conv != board::Conv::Running) continue;
        switch (item.state) {
            case batch::ItemState::Waiting: break;
            case batch::ItemState::Running:
                moved = moved || card.conv != board::Conv::Running;
                card.conv = board::Conv::Running;
                break;
            case batch::ItemState::Done: board::record(card, board::Conv::Done, item.output, {}); moved = ended = true; break;
            case batch::ItemState::CannotConvert: board::record(card, board::Conv::CannotConvert, {}, item.message); moved = ended = true; break;
            case batch::ItemState::Failed: board::record(card, board::Conv::Failed, {}, item.message); moved = ended = true; break;
            case batch::ItemState::Cancelled: board::record(card, board::Conv::Cancelled, {}, {}); moved = ended = true; break;
        }
    }
    if (ended) {
        try {
            board::save_states(*model_.board);
        } catch (const std::exception& e) {
            model_.status_text = std::string("The board could not be saved: ") + e.what();
        }
    }
    if (moved) refresh_board();
}

// The window's name (the taskbar's and Alt+Tab's) says where on the map it is.
// The dots move only while a map is on screen, and not at all with Windows' "Animation effects" off.
void WwApp::sync_flow_timer() {
    const bool wanted = model_.page == Page::Board && model_.board_view != 0 && model_.board_projection != nullptr && ww_animations_on();
    if (wanted == flow_timer_on_) return;
    flow_timer_on_ = wanted;
    if (wanted) {
        flow_tick_ = std::chrono::steady_clock::now();
        SetTimer(hwnd_, kWwFlowTimer, kWwFlowFrameMs, nullptr);
    } else {
        KillTimer(hwnd_, kWwFlowTimer);
    }
}

void WwApp::sync_board_title() {
    sync_flow_timer();
    board_sync_inspector(model_);
    std::wstring title = L"CodeBreaker";
    if (model_.page == Page::Board && model_.board_view != 0 && model_.board_graph) {
        const std::vector<std::string> words = board_crumb_words(model_);
        title += L" - " + widen(words.back());
        if (model_.board_view == 3 && model_.board_library && model_.board_hover_export >= 0 && static_cast<std::size_t>(model_.board_hover_export) < model_.board_library->right.size()) {
            const board::LibraryChannel& hovered = model_.board_library->right[static_cast<std::size_t>(model_.board_hover_export)];
            title += L" [" + widen(hovered.label) + L": " + std::to_wstring(hovered.users.size()) + L" files]";
        }
    }
    if (model_.page == Page::Board && model_.board_view == 0 && model_.board_hover && model_.board)
        title += L" - " + widen(model_.board->cards[model_.board_hover->card].name) + L" [uses " + std::to_wstring(model_.board_hover->uses) + L", used by " + std::to_wstring(model_.board_hover->used_by) + L"]";
    if (model_.page == Page::Board && model_.board_inspecting && model_.board_inspector && model_.board)
        title += L" (X-Ray: " + widen(model_.board->cards[model_.board_inspector->card].name) + L")";
    if (model_.page == Page::Board && model_.board_orphans_only) title += L" {orphans}";
    if (model_.page == Page::Board && model_.board_active_deck >= 0 && static_cast<std::size_t>(model_.board_active_deck) < model_.board_decks.size())
        title += L" {deck: " + widen(model_.board_decks[static_cast<std::size_t>(model_.board_active_deck)].name) + L"}";
    if (model_.page == Page::Board && model_.board_batch_armed > 0) title += L" {queue " + std::to_wstring(model_.board_batch_armed) + L"?}";
    if (model_.page == Page::Board && !model_.board_filters.empty()) title += L" {" + std::to_wstring(model_.board_filters.size()) + L" filters}";
    if (title != board_title_) {
        board_title_ = title;
        SetWindowTextW(hwnd_, title.c_str());
    }
}

// A dragged card let go: on a foundation it is sent to be converted; it does not land there until that is true.
void WwApp::board_release(float x, float y) {
    const int card = model_.board_drag;
    model_.board_drag = -1;
    model_.board_drop_pile = -1;
    board_press_card_ = -1;
    if (card < 0 || !model_.board) return;
    if (board_foundation_at(compute_board_layout(layout_, model_), model_, x, y) >= 0) board_convert(card);
    invalidate();
}

// Activating a part of the Tools or Settings page (a click released on it, or Enter or Space on the focused one).
void WwApp::activate_page(Control c, int index) {
    switch (c) {
        case Control::NavConvert:
            if (model_.page == Page::Convert) back_to_program();   // (on the Source card: back to the Program card)
            else set_page(Page::Convert);
            break;
        case Control::RecentRow: open_recent(index); break;
        case Control::RecentRemove: remove_recent(index); break;
        case Control::PreviewFile:
            if (index >= 0 && index != model_.preview_selected) select_preview(index);
            break;
        case Control::CodeView:
        case Control::OutputBox: break;
        case Control::TabFiles: set_source_tab(SourceTab::Files); break;
        case Control::TabFunctions: set_source_tab(SourceTab::Functions); break;
        case Control::TabCheck: set_source_tab(SourceTab::Check); break;
        case Control::FnRow: open_compare(index); break;
        case Control::FnFilter: cycle_filter(); break;
        case Control::FnAll: close_compare(); break;
        case Control::FnPicker: open_picker(!model_.cmp_picker_open); break;
        case Control::FnPickItem: break;
        case Control::CheckShow:
            model_.fn_filter = 1;
            set_source_tab(SourceTab::Functions);
            break;
        case Control::CheckAgain: break;   // (disabled until a route can judge again: Outcome::can_recheck)
        case Control::CmpAsmLine: compare_click(0, index); break;
        case Control::CmpCLine: compare_click(1, index); break;
        case Control::NavTools: set_page(Page::Tools); break;
        case Control::NavBoard: set_page(Page::Board); break;
        case Control::BoardOpen: browse_board(); break;
        case Control::BoardCancel:
            board_cancel_ = true;
            model_.board_stage = "Cancelling";
            invalidate();
            break;
        case Control::BoardSearch: set_focus(Control::BoardSearch, -1); break;
        case Control::BoardFoundation:   // the pile shows what is really on it, or back to what is left to do
            if (index >= 0 && index < static_cast<int>(board::kPileCount)) {
                model_.board_show_done[static_cast<std::size_t>(index)] = !model_.board_show_done[static_cast<std::size_t>(index)];
                model_.board_scroll[static_cast<std::size_t>(index)] = 0;
                invalidate();
            }
            break;
        case Control::BoardExpand:
            if (index >= 0 && index < static_cast<int>(board::kPileCount)) {
                model_.board_expanded[static_cast<std::size_t>(index)] = !model_.board_expanded[static_cast<std::size_t>(index)];
                model_.board_scroll[static_cast<std::size_t>(index)] = 0;
                invalidate();
            }
            break;
        case Control::BoardCard:   // a library opens on its imports and exports; any other card is selected
            model_.board_selected = index;
            board_hover_card(model_, -1);
            if (model_.board_index && model_.board && index >= 0 && model_.board->cards[static_cast<std::size_t>(index)].pile == board::Pile::Libraries) board_show_view(model_, 3);
            else if (model_.board_index) board_inspect_card(model_, index);
            relayout();
            invalidate();
            break;
        case Control::BoardViewTab:
            board_show_view(model_, index);
            relayout();
            invalidate();
            break;
        case Control::BoardMap:
            model_.board_selected = index;
            board_show_view(model_, 3);
            relayout();
            invalidate();
            break;
        case Control::BoardLinkChip:
            if (index >= 0 && index < 3 && model_.board_width == board::Width::Files) {   // (Links and Calls are of imports only)
                const unsigned bit = 1u << static_cast<unsigned>(index);
                if ((model_.board_links & ~bit) != 0 || (model_.board_links & bit) == 0) {   // (one kind always stays on)
                    model_.board_links ^= bit;
                    board_refresh_maps(model_);
                    relayout();
                    invalidate();
                }
            }
            break;
        case Control::BoardWidth:
            if (index >= 0 && index < 3) {
                model_.board_width = static_cast<board::Width>(index);
                board_refresh_maps(model_);
                relayout();
                invalidate();
            }
            break;
        case Control::BoardFlowBand:   // a band: on to its most linked file
            if (model_.board_flow) {
                const bool right = index >= 100;
                const auto at = static_cast<std::size_t>(right ? index - 100 : index);
                const std::vector<board::FlowBand>& bands = right ? model_.board_flow->out : model_.board_flow->in;
                if (at < bands.size()) {
                    model_.board_function.clear();
                    board_go_to(model_, static_cast<int>(bands[at].top));
                }
                relayout();
                invalidate();
            }
            break;
        case Control::BoardNode:
        case Control::BoardUser:
            board_go_to(model_, index);
            relayout();
            invalidate();
            break;
        case Control::BoardLeft:
        case Control::BoardRight:   // a channel: a module, or a function of the one in view, becomes the centre
            if (model_.board_library) {
                const std::vector<board::LibraryChannel>& side = c == Control::BoardLeft ? model_.board_library->left : model_.board_library->right;
                if (index >= 0 && static_cast<std::size_t>(index) < side.size()) {
                    const board::LibraryChannel& channel = side[static_cast<std::size_t>(index)];
                    if (channel.module >= 0) board_go_to(model_, static_cast<int>(channel.module));
                    else board_go_to(model_, model_.board_focus, std::string(channel.key));
                    relayout();
                    invalidate();
                }
            }
            break;
        case Control::BoardCView:
            board_toggle_c_view(model_, static_cast<board::CView>(index));
            relayout();
            invalidate();
            break;
        case Control::BoardOrphans:
            board_toggle_orphans(model_);
            relayout();
            invalidate();
            break;
        case Control::BoardDecks:
            model_.board_decks_open = !model_.board_decks_open;
            invalidate();
            break;
        case Control::BoardDeckRow:
            try {
                if (index < 0) board_save_deck(model_);
                else board_apply_deck(model_, index);
            } catch (const std::exception& e) {
                model_.status_text = std::string("The deck could not be saved: ") + e.what();
            }
            model_.board_decks_open = false;
            relayout();
            invalidate();
            break;
        case Control::BoardDeckRemove:
            try {
                board_remove_deck(model_, index);
            } catch (const std::exception& e) {
                model_.status_text = std::string("The deck could not be removed: ") + e.what();
            }
            relayout();
            invalidate();
            break;
        case Control::BoardExport:
            if (model_.board && model_.board_index) {
                try {
                    const stdfs::path written = board_export(model_);
                    model_.status_text = "Exported JSON, GraphML and a Maltego table: " + fs::to_text(written);
                } catch (const std::exception& e) {
                    model_.status_text = std::string("The export failed: ") + e.what();
                }
                invalidate();
            }
            break;
        case Control::BoardBatch:   // once: how many. Again: they join the convert queue, most linked first. While a run is under way: stop it.
            if (running_ || queue_.active()) {
                cancel_conversion();
                model_.board_batch_armed = 0;
            } else if (model_.board_batch_armed == 0) {
                model_.board_batch_armed = model_.board_batch.size();
            } else if (model_.board) {
                std::vector<stdfs::path> files;
                files.reserve(model_.board_batch.size());
                for (const std::uint32_t card : model_.board_batch) {
                    board::Card& queued = model_.board->cards[card];
                    queued.conv = board::Conv::Queued;
                    queued.note.clear();
                    files.push_back(model_.board->file_of(queued));
                }
                model_.board_batch_armed = 0;
                add_files(files);
                board_sync_states();
                board_rebuild_stacks(model_);
            }
            invalidate();
            break;
        case Control::BoardInspect:
            if (model_.board_inspecting) board_inspect_card(model_, -1);
            else board_inspect_card(model_, index);
            relayout();
            invalidate();
            break;
        case Control::BoardInspectClose:
            board_inspect_card(model_, -1);
            relayout();
            invalidate();
            break;
        case Control::BoardInspectRow:   // a file: the drawer goes to it; a signal: the board shows what shares it
            if (model_.board_inspector && index >= 0 && static_cast<std::size_t>(index) < model_.board_inspector->rows.size()) {
                const board::InspectorRow row = model_.board_inspector->rows[static_cast<std::size_t>(index)];
                if (row.card >= 0) {
                    board_inspect_card(model_, static_cast<int>(row.card));
                } else if (row.signal >= 0) {
                    if (model_.board_view != 0) board_crumb_back(model_, -1);
                    if (std::find(model_.board_filters.begin(), model_.board_filters.end(), static_cast<std::uint32_t>(row.signal)) == model_.board_filters.end())
                        board_toggle_filter(model_, static_cast<std::uint32_t>(row.signal));
                }
                relayout();
                invalidate();
            }
            break;
        case Control::BoardSignalType:
            board_open_signals(model_, index);
            invalidate();
            break;
        case Control::BoardSignalRow:
            board_toggle_filter(model_, static_cast<std::uint32_t>(index));
            relayout();
            invalidate();
            break;
        case Control::BoardFilterChip:
            board_toggle_filter(model_, static_cast<std::uint32_t>(index));
            relayout();
            invalidate();
            break;
        case Control::BoardGroupCopies:
            model_.board_group_copies = !model_.board_group_copies;
            board_rebuild_stacks(model_);
            relayout();
            invalidate();
            break;
        case Control::BoardCrumb:
            board_crumb_back(model_, index);
            relayout();
            invalidate();
            break;
        case Control::BoardCardOpen: board_open_card(index); break;
        case Control::BoardCardConvert: board_convert(index); break;
        case Control::NavSettings: set_page(Page::Settings); break;
        case Control::ToolRow: select_tool(index); break;
        case Control::Run:
            if (run_ != nullptr) stop_tool();
            else run_tool();
            break;
        case Control::FieldBrowse: browse_field(index); break;
        case Control::FieldBrowseFolder: browse_field(index, true); break;
        case Control::OpenResult: open_result(); break;
        case Control::ToolChoice:
            if (model_.tool_selected >= 0 && index >= 0 && index < static_cast<int>(model_.tool_choices.size())) {
                model_.tool_choice = index;
                model_.focus = Control::ToolChoice;
                model_.focus_index = index;
                tool_values_[model_.tool_selected].choice = index;
                invalidate();
            }
            break;
        case Control::SettingsAdvanced: open_advanced(true); break;
        case Control::SettingsBack: open_advanced(false); break;
        case Control::Locate: locate_tool(index); break;
        case Control::Field: {
            std::vector<FieldItem>& fields = model_.page == Page::Settings ? model_.setting_fields : model_.tool_fields;
            if (index < 0 || static_cast<std::size_t>(index) >= fields.size()) break;
            FieldItem& f = fields[static_cast<std::size_t>(index)];
            if (f.kind == 4) {   // a switch
                f.text = f.text.empty() ? "1" : "";
                model_.focus = Control::Field;
                model_.focus_index = index;
                edited();
                if (model_.page == Page::Settings) commit_setting(index);
            } else if (f.kind == 3) {
                open_field_popup(model_.field_popup == index ? -1 : index);
            } else {
                set_focus(Control::Field, index);
            }
            break;
        }
        default: break;
    }
}

void WwApp::scroll_page(float x, float y, int lines) {
    const float step = 36.0f * static_cast<float>(lines);
    if (model_.page == Page::Tools) {
        const ToolsLayout t = compute_tools_layout(layout_, model_);
        if (t.list_area.contains(x, y)) {
            model_.tool_scroll = std::clamp(model_.tool_scroll - step, 0.0f, std::max(0.0f, t.list_content_height - t.list_area.h));
        } else if (t.output.contains(x, y)) {
            model_.output_scroll = std::max(0.0f, model_.output_scroll + static_cast<float>(lines) * 3.0f);
        }
    } else if (model_.page == Page::Settings) {
        const SettingsLayout s = compute_settings_layout(layout_, model_);
        model_.setting_scroll = std::clamp(model_.setting_scroll - step, 0.0f, std::max(0.0f, s.form.content_height - s.fields_area.h));
    } else if (model_.page == Page::Board && model_.board_inspecting && compute_board_layout(layout_, model_).drawer.contains(x, y)) {   // the X-Ray drawer
        model_.board_inspect_scroll = std::clamp(model_.board_inspect_scroll - step * 2, 0.0f, layout_inspector(compute_board_layout(layout_, model_), model_).max_scroll);
    } else if (model_.page == Page::Board && model_.board && model_.board_view == 0 && model_.board_signal_type >= 0 &&
               layout_signal_panel(compute_board_layout(layout_, model_), model_).outline.contains(x, y)) {   // the open list of signals
        model_.board_signal_scroll = std::clamp(model_.board_signal_scroll - step * 2, 0.0f, layout_signal_panel(compute_board_layout(layout_, model_), model_).max_scroll);
    } else if (model_.page == Page::Board && model_.board && model_.board_view == 3 && model_.board_library) {   // the Library view: the list under the pointer
        const LibraryLayout library = layout_library(compute_board_layout(layout_, model_).canvas, *model_.board_library, model_.board_left_scroll, model_.board_right_scroll, model_.board_hover_export);
        if (library.left_area.contains(x, y)) model_.board_left_scroll = std::clamp(model_.board_left_scroll - step * 2, 0.0f, library.left_max_scroll);
        else if (library.right_area.contains(x, y)) model_.board_right_scroll = std::clamp(model_.board_right_scroll - step * 2, 0.0f, library.right_max_scroll);
    } else if (model_.page == Page::Board && model_.board) {
        const BoardLayout b = compute_board_layout(layout_, model_);
        bool scrolled = false;
        if ((GetKeyState(VK_SHIFT) & 0x8000) == 0) {   // over an expanded column: it scrolls; anywhere else the columns move sideways
            for (std::size_t i = 0; i < board::kPileCount && !scrolled; ++i) {
                const BoardColumn c = compute_board_column(b, model_, i);
                if (!c.expanded || !c.area.contains(x, y)) continue;
                model_.board_scroll[i] = std::clamp(model_.board_scroll[i] - step * 2, 0.0f, board_max_scroll(c));
                scrolled = true;
            }
        }
        if (!scrolled) model_.board_hscroll = std::clamp(model_.board_hscroll - step * 2, 0.0f, board_max_hscroll(b));
    }
    invalidate();
}

void WwApp::cancel_conversion() {
    if (!running_) return;
    cancel_ = true;
    queue_.cancel_waiting();   // (Cancel all: the files still waiting are not started)
    model_.status_text = "Cancelling…";
    model_.stage_label = "Cancelling";
    sync_queue();
}

void WwApp::reset_to_idle(const std::string& status) {
    Model m = model_;   // (the pages and what they hold stay; the Convert page starts over)
    m.view = View::Idle;
    m.dragging = false;
    m.file_name.clear();
    m.stage_label.clear();
    m.detail.clear();
    m.percent = 0;
    m.info_title.clear();
    m.info_message.clear();
    m.output_path.clear();
    m.done_note.clear();
    m.stream.clear();
    m.stream_cursor = false;
    m.language_open = false;
    m.pressed = Control::None;
    m.status_text = status;
    model_ = std::move(m);
    stream_.clear();
    KillTimer(hwnd_, kWwStreamTimer);
    relayout();
    invalidate();
}

// The language the run writes `file` in: the one chosen when the translator can write it for this file, else the first it can (not remembered).
LanguageItem WwApp::language_for(const stdfs::path& file) const {
    const std::vector<LanguageItem> items = ww_items(convert::targets_for(file));
    const std::string wanted = model_.languages.empty() ? std::string("original") : model_.languages[static_cast<std::size_t>(model_.language_selected)].id;
    for (const LanguageItem& i : items)
        if (i.id == wanted && i.supported) return i;
    for (const LanguageItem& i : items)
        if (i.supported) return i;
    return LanguageItem{"original", "Original", true, {}};
}

// Files to convert. With no run under way they start one (a single file behaves as it always did: its own progress, then its Source card); while a run
// is under way they join the end of its queue, and the Program card lists the files.
void WwApp::add_files(const std::vector<stdfs::path>& files) {
    if (files.empty()) return;
    if (running_ || queue_.active()) {
        if (cancel_) return;
        queue_.add(files);
        sync_queue();
        return;
    }
    if (worker_.joinable()) worker_.join();
    queue_.clear();
    queue_.add(files);
    run_results_.clear();
    begin_run();
}

// The window turns to the working view for the queue just made, then runs its first file.
void WwApp::begin_run() {
    const stdfs::path first = queue_.items().front().file;
    // the translator says which languages it can write for this file; one the file cannot have gives way to the first it can (not remembered)
    set_language_list(convert::targets_for(first));
    model_.language_selected = ww_index_of(model_.languages, language_for(first).id);
    const View before = model_.view;
    skip_welcome();
    model_.view = View::Working;
    begin_card_fade(before);   // (from the Source card: it fades out with what it showed -- the output path and note stay until the next Done)
    model_.focus = Control::None;
    model_.queue.clear();
    model_.queue_scroll = 0;
    taskbar_.set(Taskbar::State::Indeterminate);
    KillTimer(hwnd_, kWwTaskbarTimer);
    if (ww_animations_on()) {
        stream_tick_ = std::chrono::steady_clock::now();
        stream_frames_ = 0;
        SetTimer(hwnd_, kWwStreamTimer, kWwStreamFrameMs, nullptr);
    }
    run_next();
}

// Start the next file of the queue on the worker thread.
void WwApp::run_next() {
    const int next = queue_.next_waiting();
    if (next < 0) return;
    if (worker_.joinable()) worker_.join();
    const stdfs::path file = queue_.items()[static_cast<std::size_t>(next)].file;
    const LanguageItem language = language_for(file);
    const std::string target = language.id;
    queue_.start(static_cast<std::size_t>(next));
    board_sync_states();
    running_item_ = next;
    run_language_name_ = target == "original" ? std::string() : language.name;
    model_.file_name = queue_.items()[static_cast<std::size_t>(next)].name;
    model_.stage_label = std::string(convert::state_label(convert::State::Identifying));
    model_.detail.clear();
    model_.percent = 0;
    model_.status_text = (target == "original" ? "Converting " + model_.file_name : "Translating " + model_.file_name + " to " + language.name);
    stream_.clear();
    model_.stream.clear();
    model_.stream_cursor = false;
    {
        std::lock_guard hold(lock_);
        stream_waiting_.clear();
    }
    cancel_ = false;   // (before the queue is shown: the status text of several files is not kept while cancelling)
    sync_queue();
    running_ = true;
    HWND window = hwnd_;
    worker_ = std::thread([this, file, window, target] {
        convert::Request request;
        request.input = file;
        request.converted_root = convert::converted_root();
        convert::Outcome outcome;
        try {
            outcome = convert::convert(
                request, convert::standard_planner(target),
                [this, window](const convert::Progress& p) {
                    {
                        std::lock_guard hold(lock_);
                        latest_ = p;
                        stream_waiting_.insert(stream_waiting_.end(), p.lines.begin(), p.lines.end());
                        if (stream_waiting_.size() > CharStream::kStreamPendingLines)
                            stream_waiting_.erase(stream_waiting_.begin(), stream_waiting_.end() - static_cast<std::ptrdiff_t>(CharStream::kStreamPendingLines));
                    }
                    if (!progress_posted_.exchange(true)) PostMessageW(window, kWmProgress, 0, 0);
                },
                cancel_);
        } catch (const std::exception& e) {
            outcome.state = convert::State::Failed;
            outcome.message = e.what();
        }
        {
            std::lock_guard hold(lock_);
            outcome_ = std::move(outcome);
        }
        running_ = false;
        PostMessageW(window, kWmFinished, 0, 0);
    });
}

// The queue's rows, "n of m done" and the status text of several files, the layout they imply, and the taskbar button's progress.
void WwApp::sync_queue() {
    const bool batch = queue_.total() > 1;
    model_.queue.clear();
    if (batch) {
        for (const batch::Item& item : queue_.items()) {
            QueueRowItem row;
            row.name = item.name;
            row.status = batch::Queue::status_words(item);
            row.percent = item.percent;
            row.state = static_cast<int>(item.state);   // (Waiting, Running, Done, CannotConvert, Failed, Cancelled are 0..5)
            if (item.state == batch::ItemState::Running) row.detail = item.stage + (item.detail.empty() ? std::string() : " \xC2\xB7 " + item.detail);
            model_.queue.push_back(std::move(row));
        }
        model_.queue_note = queue_.progress_words();
        if (!cancel_) model_.status_text = "Converting " + std::to_string(queue_.total()) + " files";
    } else {
        model_.queue_note.clear();
        model_.queue_scroll = 0;
    }
    relayout();
    if (batch) model_.queue_scroll = std::clamp(model_.queue_scroll, 0.0f, queue_max_scroll(compute_queue_layout(layout_, model_)));
    invalidate();
    update_taskbar();
}

// The taskbar button's bar follows the run: a moving one until there is a percentage, then the percentage (of the whole queue when there are several files).
void WwApp::update_taskbar() {
    if (!queue_.active()) return;
    const int percent = queue_.total() > 1 ? queue_.overall_percent() : model_.percent;
    taskbar_.set(percent <= 0 ? Taskbar::State::Indeterminate : Taskbar::State::Normal, percent);
}

void WwApp::on_progress() {
    std::optional<convert::Progress> p;
    std::vector<std::string> lines;
    {
        std::lock_guard hold(lock_);
        p = std::move(latest_);
        latest_.reset();
        lines.swap(stream_waiting_);
    }
    progress_posted_ = false;
    if (!p || model_.view != View::Working) return;
    if (!cancel_) model_.stage_label = p->label;
    model_.detail = p->detail;
    model_.percent = p->percent;
    if (running_item_ >= 0) queue_.progress(static_cast<std::size_t>(running_item_), p->percent, p->label, p->detail);
    if (!lines.empty()) {
        stream_.push(lines);
        if (!ww_animations_on()) {   // Windows' "Animation effects" off: the newest lines, still, no typing and no blinking cursor
            stream_.show_all();
            model_.stream = stream_.visible(kWwStreamRows);
            model_.stream_cursor = false;
        }
    }
    sync_queue();
}

// ---- Recent, the preview, the drag out and the notification -----------------------------------------------------------------------------------

// A game-console image or cartridge (the Recent row shows a game controller for it rather than a document).
bool ww_is_console_file(const std::string& name) {
    static const char* const kExtensions[] = {".sfc", ".smc", ".nes", ".gb", ".gbc", ".gba", ".nds", ".n64", ".z64", ".v64", ".md", ".gen", ".smd", ".sms", ".gg",
                                              ".pce", ".a26", ".a78", ".vec", ".ngp", ".ngc", ".32x", ".xex", ".xbe", ".iso", ".cue"};
    const std::size_t dot = name.find_last_of('.');
    if (dot == std::string::npos) return false;
    const std::string ext = text::lower(name.substr(dot));
    for (const char* e : kExtensions)
        if (ext == e) return true;
    return false;
}

// The Program card's Recent rows from the list kept: what each became, files and when, and whether its folder is still there.
void WwApp::refresh_recent() {
    const std::int64_t now = recent::now_seconds();
    const int offset = recent::utc_offset_seconds(now);
    model_.recent.clear();
    for (const recent::Entry& e : recent_) {
        RecentItem row;
        row.name = e.name;
        row.chip = e.chip;
        std::error_code ec;
        row.missing = !stdfs::is_directory(fs::from_text(e.folder), ec);
        row.meta = row.missing ? std::string("folder moved or deleted")
                               : std::to_string(e.files) + (e.files == 1 ? " file" : " files") + " \xC2\xB7 " + recent::when_words(now, e.when, offset);
        row.glyph_console = ww_is_console_file(e.name);
        model_.recent.push_back(std::move(row));
    }
    if (hwnd_ != nullptr) {
        relayout();
        invalidate();
    }
}

// A conversion that ended Done joins Recent (newest first, five at most), kept for the next time the window starts.
void WwApp::remember_item(const batch::Item& item) {
    std::string chip = run_language_name_;   // (the language it was translated to; Original: what the main source is)
    if (chip.empty()) chip = preview::chip_for_files(preview::list_files(item.output, fs::to_text(item.file.stem())));
    recent::Entry entry;
    entry.name = item.name;
    entry.chip = chip;
    entry.folder = narrow(item.output.wstring());
    entry.files = item.files;
    entry.when = recent::now_seconds();
    recent::add(recent_, std::move(entry));
    recent::save(recent::store_file(convert::settings_file()), recent_);
    refresh_recent();
}

void WwApp::remove_recent(int index) {
    if (index < 0 || static_cast<std::size_t>(index) >= recent_.size()) return;
    recent::remove_at(recent_, static_cast<std::size_t>(index));
    recent::save(recent::store_file(convert::settings_file()), recent_);
    model_.focus = Control::None;
    model_.focus_index = -1;
    refresh_recent();
}

// A click on a Recent row: the Source card of that conversion (when its folder is still there; else the row says so and offers Remove).
void WwApp::open_recent(int index) {
    if (running_ || queue_.active() || index < 0 || static_cast<std::size_t>(index) >= recent_.size()) return;
    const recent::Entry entry = recent_[static_cast<std::size_t>(index)];
    std::error_code ec;
    if (!stdfs::is_directory(fs::from_text(entry.folder), ec)) {
        refresh_recent();
        return;
    }
    const View before = model_.view;
    skip_welcome();
    model_.view = View::Done;
    model_.focus = Control::None;
    model_.file_name = entry.name;
    model_.done_file_name = entry.name;
    model_.output_path = entry.folder;
    model_.done_note = std::to_string(entry.files) + (entry.files == 1 ? " file written." : " files written.");
    model_.status_text = "Done";
    load_preview_folder(fs::from_text(entry.folder), fs::to_text(fs::from_text(entry.name).stem()));
    load_functions(fs::from_text(entry.folder), {}, {}, false);
    begin_card_fade(before);
    relayout();
    invalidate();
}

// The Source card back to the Program card (Esc, or the Convert item of the rail).
void WwApp::back_to_program() {
    if (model_.view != View::Done || running_ || queue_.active()) return;
    const View before = model_.view;
    reset_to_idle("Ready");
    begin_card_fade(before);
    refresh_recent();
}

// The files the output folder holds (the main source first) and the first one's source.
void WwApp::load_preview_folder(const stdfs::path& folder, const std::string& main_stem) {
    preview_entries_ = preview::list_files(folder, main_stem);
    model_.source_tab = SourceTab::Files;   // (a conversion's Source card opens on its files; load_functions follows)
    model_.functions.clear();
    model_.can_recheck = false;
    model_.fn_filter = 0;
    model_.fn_scroll = 0;
    model_.cmp_index = -1;
    model_.cmp_picker_open = false;
    model_.preview_files.clear();
    for (const preview::FileEntry& e : preview_entries_) model_.preview_files.push_back({e.path, preview::is_script_file(e.path)});
    model_.preview_list_scroll = 0;
    select_preview(0);
}

// Show a file of the list: its first kilobytes as lines, coloured, from the top.
void WwApp::select_preview(int index) {
    model_.preview_lines.clear();
    model_.preview_spans.clear();
    model_.preview_note.clear();
    model_.preview_scroll = 0;
    model_.preview_hscroll = 0;
    if (index < 0 || static_cast<std::size_t>(index) >= preview_entries_.size()) {
        model_.preview_selected = 0;
        invalidate();
        return;
    }
    model_.preview_selected = index;
    if (model_.focus == Control::PreviewFile) model_.focus_index = index;
    const preview::FileEntry& entry = preview_entries_[static_cast<std::size_t>(index)];
    preview::Preview p = preview::read_preview(fs::from_text(model_.output_path) / fs::from_text(entry.path));
    model_.preview_spans = preview::highlight(preview::lang_for(entry.path), p.lines);
    model_.preview_lines = std::move(p.lines);
    model_.preview_note = p.note;
    relayout();
    const PreviewLayout pl = compute_preview_layout(layout_, model_);   // the row stays in view
    const float top = static_cast<float>(index) * kFileRowHeight;
    if (top < model_.preview_list_scroll) model_.preview_list_scroll = top;
    else if (top + kFileRowHeight > model_.preview_list_scroll + pl.list_area.h) model_.preview_list_scroll = top + kFileRowHeight - pl.list_area.h;
    model_.preview_list_scroll = std::clamp(model_.preview_list_scroll, 0.0f, preview_list_max_scroll(pl));
    invalidate();
}

void WwApp::scroll_preview_to(float first_line) {
    const PreviewLayout p = compute_preview_layout(layout_, model_);
    model_.preview_scroll = std::clamp(first_line, 0.0f, static_cast<float>(preview_max_first_line(model_, p)));
    invalidate();
}

// ---- the Source card's tabs (Functions, Check, Compare) and the command palette ---------------------------------------------------------------

// The functions of a finished conversion: what the route recorded (else what the output folder's source defines), with the verdicts the function store
// holds for the program `input` (empty for a conversion opened from Recent: the program is not known then).
void WwApp::load_functions(const stdfs::path& folder, const std::vector<convert::FunctionRecord>& recorded, const stdfs::path& input, bool can_recheck) {
    std::map<std::string, convert::FunctionVerdict> verdicts;
    if (!input.empty()) verdicts = fnsrc::stored_verdicts(input, config::current().state());
    model_.functions = fnsrc::collect(folder, recorded, verdicts);
    model_.can_recheck = can_recheck;
}

void WwApp::set_source_tab(SourceTab tab) {
    if (model_.view != View::Done || model_.preview_files.empty()) return;
    model_.source_tab = tab;
    if (tab != SourceTab::Functions) model_.cmp_index = -1;
    model_.cmp_picker_open = false;
    model_.fn_scroll = 0;
    if (model_.focus != Control::TabFiles && model_.focus != Control::TabFunctions && model_.focus != Control::TabCheck) {
        model_.focus = Control::None;
        model_.focus_index = -1;
    }
    relayout();
    invalidate();
}

// Compare: one function's assembly beside its rebuilt source.
void WwApp::open_compare(int index) {
    if (index < 0 || static_cast<std::size_t>(index) >= model_.functions.size()) return;
    const fnsrc::Pair pair = fnsrc::load_pair(fs::from_text(model_.output_path), model_.functions[static_cast<std::size_t>(index)]);
    model_.source_tab = SourceTab::Functions;
    model_.cmp_index = index;
    model_.cmp_asm = pair.asm_lines;
    model_.cmp_c = pair.c_lines;
    model_.cmp_asm_spans = preview::highlight(preview::Lang::Asm, model_.cmp_asm);
    model_.cmp_c_spans = preview::highlight(preview::Lang::C, model_.cmp_c);
    model_.cmp_links = pair.links;
    model_.cmp_asm_note = pair.asm_note;
    model_.cmp_c_note = pair.c_note;
    model_.cmp_side = -1;
    model_.cmp_line = -1;
    model_.cmp_scroll_asm = 0;
    model_.cmp_scroll_c = 0;
    model_.cmp_picker_open = false;
    model_.focus = Control::FnAll;
    model_.focus_index = -1;
    relayout();
    invalidate();
}

void WwApp::close_compare() {
    model_.cmp_index = -1;
    model_.cmp_picker_open = false;
    model_.focus = Control::None;
    model_.focus_index = -1;
    invalidate();
}

// A click on a line of Compare: it is lit with the lines that match it; the other side shows them.
void WwApp::compare_click(int side, int line) {
    model_.cmp_side = side;
    model_.cmp_line = line;
    const fnsrc::Link* link = fnsrc::link_at(model_.cmp_links, side == 0 ? fnsrc::Side::Asm : fnsrc::Side::C, line);
    if (link != nullptr) {
        const CompareLayout c = compute_compare_layout(layout_, model_);
        const int other = side == 0 ? 1 : 0;
        const int first = other == 0 ? link->asm_first : link->c_first, last = other == 0 ? link->asm_last : link->c_last;
        float& scroll = other == 0 ? model_.cmp_scroll_asm : model_.cmp_scroll_c;
        if (first < static_cast<int>(scroll) || last >= static_cast<int>(scroll) + c.visible_lines)
            scroll = static_cast<float>(std::clamp(first - 1, 0, compare_max_first_line(model_, c, other)));
    }
    invalidate();
}

void WwApp::open_picker(bool open) {
    model_.cmp_picker_open = open && model_.cmp_index >= 0;
    if (model_.cmp_picker_open) {
        model_.cmp_picker_hot = model_.cmp_index;
        model_.cmp_picker_top = std::max(0, model_.cmp_index - 4);
    }
    invalidate();
}

// Keys while the function drop-down is open.
void WwApp::picker_key(WPARAM key) {
    const int count = static_cast<int>(model_.functions.size());
    switch (key) {
        case VK_DOWN: model_.cmp_picker_hot = std::min(count - 1, model_.cmp_picker_hot + 1); break;
        case VK_UP: model_.cmp_picker_hot = std::max(0, model_.cmp_picker_hot - 1); break;
        case VK_HOME: model_.cmp_picker_hot = 0; break;
        case VK_END: model_.cmp_picker_hot = count - 1; break;
        case VK_RETURN:
        case VK_SPACE: {
            const int chosen = model_.cmp_picker_hot;
            open_picker(false);
            open_compare(chosen);
            return;
        }
        case VK_ESCAPE:
        case VK_F4: open_picker(false); return;
        default: break;
    }
    const int rows = static_cast<int>(kPickerRows);
    if (model_.cmp_picker_hot < model_.cmp_picker_top) model_.cmp_picker_top = model_.cmp_picker_hot;
    if (model_.cmp_picker_hot >= model_.cmp_picker_top + rows) model_.cmp_picker_top = model_.cmp_picker_hot - rows + 1;
    invalidate();
}

// Show: All, Differs, Not checked, one after the other.
void WwApp::cycle_filter() {
    model_.fn_filter = (model_.fn_filter + 1) % 3;
    model_.fn_scroll = 0;
    invalidate();
}

// Keys of the Source card's tabs. False when the key is not one of theirs.
bool WwApp::source_key(WPARAM key) {
    if (model_.source_tab == SourceTab::Files) return false;
    if (model_.cmp_picker_open) {
        picker_key(key);
        return true;
    }
    const bool on_tab = model_.focus == Control::TabFiles || model_.focus == Control::TabFunctions || model_.focus == Control::TabCheck;
    if (on_tab && (key == VK_LEFT || key == VK_RIGHT)) {   // the arrows move between the tabs
        const int at = model_.focus == Control::TabFiles ? 0 : model_.focus == Control::TabFunctions ? 1 : 2;
        const int to = std::clamp(at + (key == VK_RIGHT ? 1 : -1), 0, 2);
        set_source_tab(static_cast<SourceTab>(to));
        model_.focus = to == 0 ? Control::TabFiles : to == 1 ? Control::TabFunctions : Control::TabCheck;
        model_.focus_index = -1;
        invalidate();
        return true;
    }
    if (model_.source_tab == SourceTab::Functions && model_.cmp_index < 0) {
        const FunctionsLayout f = compute_functions_layout(layout_, model_);
        if (model_.focus == Control::FnRow && !f.shown.empty()) {
            int at = 0;
            for (std::size_t i = 0; i < f.shown.size(); ++i)
                if (f.shown[i] == model_.focus_index) at = static_cast<int>(i);
            const int page = std::max(1, static_cast<int>(f.list_area.h / kFnRowHeight) - 1);
            int to = at;
            switch (key) {
                case VK_UP: to = at - 1; break;
                case VK_DOWN: to = at + 1; break;
                case VK_PRIOR: to = at - page; break;
                case VK_NEXT: to = at + page; break;
                case VK_HOME: to = 0; break;
                case VK_END: to = static_cast<int>(f.shown.size()) - 1; break;
                default: return false;
            }
            to = std::clamp(to, 0, static_cast<int>(f.shown.size()) - 1);
            model_.focus_index = f.shown[static_cast<std::size_t>(to)];
            const float top = static_cast<float>(to) * kFnRowHeight;   // keep the row in view
            if (top < model_.fn_scroll) model_.fn_scroll = top;
            else if (top + kFnRowHeight > model_.fn_scroll + f.list_area.h) model_.fn_scroll = top + kFnRowHeight - f.list_area.h;
            model_.fn_scroll = std::clamp(model_.fn_scroll, 0.0f, functions_max_scroll(f));
            invalidate();
            return true;
        }
        return false;
    }
    if (model_.source_tab == SourceTab::Functions && model_.cmp_index >= 0) {
        const CompareLayout c = compute_compare_layout(layout_, model_);
        const float page = static_cast<float>(std::max(1, c.visible_lines - 1));
        float delta = 0;
        switch (key) {
            case VK_UP: delta = -1; break;
            case VK_DOWN: delta = 1; break;
            case VK_PRIOR: delta = -page; break;
            case VK_NEXT: delta = page; break;
            case VK_ESCAPE: close_compare(); return true;
            default: return false;
        }
        model_.cmp_scroll_asm = std::clamp(model_.cmp_scroll_asm + delta, 0.0f, static_cast<float>(compare_max_first_line(model_, c, 0)));
        model_.cmp_scroll_c = std::clamp(model_.cmp_scroll_c + delta, 0.0f, static_cast<float>(compare_max_first_line(model_, c, 1)));
        invalidate();
        return true;
    }
    return false;
}

// The wheel over the Functions list, a pane of Compare or the function drop-down. False when the point is over none of them.
bool WwApp::scroll_source(float x, float y, int lines) {
    if (model_.view != View::Done || model_.preview_files.empty() || model_.source_tab != SourceTab::Functions) return false;
    if (model_.cmp_picker_open) {
        const PickerLayout p = compute_picker_layout(layout_, model_);
        if (p.outline.contains(x, y)) {
            model_.cmp_picker_top = std::clamp(model_.cmp_picker_top - lines, 0, std::max(0, static_cast<int>(model_.functions.size()) - static_cast<int>(kPickerRows)));
            invalidate();
            return true;
        }
    }
    if (model_.cmp_index < 0) {
        const FunctionsLayout f = compute_functions_layout(layout_, model_);
        if (!f.list_area.contains(x, y)) return false;
        model_.fn_scroll = std::clamp(model_.fn_scroll - 36.0f * static_cast<float>(lines), 0.0f, functions_max_scroll(f));
        invalidate();
        return true;
    }
    const CompareLayout c = compute_compare_layout(layout_, model_);
    for (int side = 0; side < 2; ++side) {
        if (!(side == 0 ? c.asm_pane : c.c_pane).contains(x, y)) continue;
        float& scroll = side == 0 ? model_.cmp_scroll_asm : model_.cmp_scroll_c;
        scroll = std::clamp(scroll - 3.0f * static_cast<float>(lines), 0.0f, static_cast<float>(compare_max_first_line(model_, c, side)));
        invalidate();
        return true;
    }
    return false;
}

// ---- the command palette (Ctrl+K)

void WwApp::palette_refresh() {
    model_.palette_result = palette::search(model_.palette_items, model_.palette_query);
    model_.palette_selected = palette::move(model_.palette_result, model_.palette_selected, 0);
    invalidate();
}

void WwApp::open_palette() {
    const std::int64_t now = recent::now_seconds();
    const int offset = recent::utc_offset_seconds(now);
    std::vector<palette::RecentEntry> recents;
    for (const recent::Entry& e : recent_) {
        std::error_code ec;
        recents.push_back({e.name, "converted " + recent::when_words(now, e.when, offset), !stdfs::is_directory(fs::from_text(e.folder), ec)});
    }
    model_.palette_items = palette::make_items(tools::all(), recents, model_.developer_tools);
    model_.palette_query.clear();
    model_.palette_selected = 0;
    model_.palette_open = true;
    model_.language_open = false;
    model_.field_popup = -1;
    model_.cmp_picker_open = false;
    palette_refresh();
}

void WwApp::close_palette() {
    model_.palette_open = false;
    invalidate();
}

// Enter (or a click) on the choosable row `pick`.
void WwApp::palette_run(int pick) {
    if (pick < 0 || static_cast<std::size_t>(pick) >= model_.palette_result.picks.size()) return;
    const palette::Item item = model_.palette_items[static_cast<std::size_t>(model_.palette_result.picks[static_cast<std::size_t>(pick)])];
    close_palette();
    switch (item.kind) {
        case palette::Kind::Tool:
            set_page(Page::Tools);
            if (!model_.tool_query.empty()) {   // (the tool must be in the list shown)
                model_.tool_query.clear();
                ww_fill_tool_rows(model_);
            }
            select_tool(item.index);
            break;
        case palette::Kind::Recent:
            set_page(Page::Convert);
            open_recent(item.index);
            break;
        case palette::Kind::GoTo:
            switch (static_cast<palette::Place>(item.index)) {
                case palette::Place::Convert: set_page(Page::Convert); break;
                case palette::Place::Tools: set_page(Page::Tools); break;
                case palette::Place::Settings: set_page(Page::Settings); break;
                case palette::Place::SettingsAdvanced:
                    set_page(Page::Settings);
                    open_advanced(true);
                    break;
            }
            break;
    }
}

void WwApp::palette_key(WPARAM key) {
    switch (key) {
        case VK_ESCAPE: close_palette(); return;
        case VK_RETURN: palette_run(model_.palette_selected); return;
        case VK_DOWN: model_.palette_selected = palette::move(model_.palette_result, model_.palette_selected, 1); break;
        case VK_UP: model_.palette_selected = palette::move(model_.palette_result, model_.palette_selected, -1); break;
        case VK_HOME: model_.palette_selected = 0; break;
        case VK_END: model_.palette_selected = std::max(0, static_cast<int>(model_.palette_result.picks.size()) - 1); break;
        case VK_BACK:
            while (!model_.palette_query.empty() && (static_cast<unsigned char>(model_.palette_query.back()) & 0xC0) == 0x80) model_.palette_query.pop_back();
            if (!model_.palette_query.empty()) model_.palette_query.pop_back();
            model_.palette_selected = 0;
            palette_refresh();
            return;
        default: return;
    }
    invalidate();
}

// The wheel over a list of the Convert page: the queue, the preview's files or its code (Shift: sideways). False when the point is over neither.
bool WwApp::scroll_convert(float x, float y, int lines, bool horizontal) {
    if (model_.page != Page::Convert) return false;
    if (model_.view == View::Working && !model_.queue.empty()) {
        const QueueLayout q = compute_queue_layout(layout_, model_);
        if (!q.area.contains(x, y)) return false;
        model_.queue_scroll = std::clamp(model_.queue_scroll - 36.0f * static_cast<float>(lines), 0.0f, queue_max_scroll(q));
        invalidate();
        return true;
    }
    if (scroll_source(x, y, lines)) return true;
    if (model_.view == View::Done && !model_.preview_files.empty() && model_.source_tab == SourceTab::Files) {
        const PreviewLayout p = compute_preview_layout(layout_, model_);
        if (layout_.preview_list.contains(x, y)) {
            model_.preview_list_scroll = std::clamp(model_.preview_list_scroll - 36.0f * static_cast<float>(lines), 0.0f, preview_list_max_scroll(p));
            invalidate();
            return true;
        }
        if (layout_.preview_code.contains(x, y)) {
            if (horizontal) model_.preview_hscroll = std::clamp(model_.preview_hscroll - 4.0f * static_cast<float>(lines), 0.0f, static_cast<float>(preview_max_hscroll(model_, p)));
            else scroll_preview_to(model_.preview_scroll - 3.0f * static_cast<float>(lines));
            invalidate();
            return true;
        }
    }
    return false;
}

// Keys of the Convert page's lists: in the preview the arrows, Page Up / Down, Home and End move through the files (when the list has the keyboard) or the
// code; Left / Right scroll the code sideways; Ctrl+C copies the file shown. Delete removes the focused Recent row; Esc returns from the Source card.
bool WwApp::convert_key(WPARAM key, bool ctrl) {
    if (model_.page != Page::Convert) return false;
    if (model_.view == View::Idle) {
        if (key == VK_DELETE && (model_.focus == Control::RecentRow || model_.focus == Control::RecentRemove) && model_.focus_index >= 0) {
            remove_recent(model_.focus_index);
            return true;
        }
        return false;
    }
    if (model_.view != View::Done) return false;
    if (source_key(key)) return true;
    if (key == VK_ESCAPE) {
        back_to_program();
        return true;
    }
    if (model_.preview_files.empty() || model_.source_tab != SourceTab::Files) return false;
    const PreviewLayout p = compute_preview_layout(layout_, model_);
    const bool list = model_.focus == Control::PreviewFile;
    const int files = static_cast<int>(model_.preview_files.size());
    const float page = static_cast<float>(std::max(1, p.visible_lines - 1));
    switch (key) {
        case VK_UP:
            if (list) select_preview(std::max(0, model_.preview_selected - 1));
            else scroll_preview_to(model_.preview_scroll - 1);
            return true;
        case VK_DOWN:
            if (list) select_preview(std::min(files - 1, model_.preview_selected + 1));
            else scroll_preview_to(model_.preview_scroll + 1);
            return true;
        case VK_PRIOR: scroll_preview_to(model_.preview_scroll - page); return true;
        case VK_NEXT: scroll_preview_to(model_.preview_scroll + page); return true;
        case VK_HOME:
            if (list) select_preview(0);
            else scroll_preview_to(0);
            return true;
        case VK_END:
            if (list) select_preview(files - 1);
            else scroll_preview_to(static_cast<float>(preview_max_first_line(model_, p)));
            return true;
        case VK_LEFT:
            model_.preview_hscroll = std::max(0.0f, model_.preview_hscroll - 4.0f);
            invalidate();
            return true;
        case VK_RIGHT:
            model_.preview_hscroll = std::min(static_cast<float>(preview_max_hscroll(model_, p)), model_.preview_hscroll + 4.0f);
            invalidate();
            return true;
        default: break;
    }
    if (ctrl && key == 'C') {
        copy_preview_text();
        return true;
    }
    return false;
}

// The lines of the file shown, to the clipboard.
void WwApp::copy_preview_text() {
    if (model_.preview_lines.empty() || !OpenClipboard(hwnd_)) return;
    std::string text;
    for (const std::string& line : model_.preview_lines) text += line + "\r\n";
    const std::wstring wide = widen(text);
    EmptyClipboard();
    const HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, (wide.size() + 1) * sizeof(wchar_t));
    if (mem != nullptr) {
        void* p = GlobalLock(mem);
        std::memcpy(p, wide.c_str(), (wide.size() + 1) * sizeof(wchar_t));
        GlobalUnlock(mem);
        if (SetClipboardData(CF_UNICODETEXT, mem) == nullptr) GlobalFree(mem);
    }
    CloseClipboard();
}

// The output box was pressed and the pointer has moved: the folder is dragged out (OLE; copy or link) until it is dropped or Esc is pressed.
void WwApp::begin_drag_out() {
    drag_pending_ = false;
    if (model_.output_path.empty()) return;
    if (GetCapture() == hwnd_) ReleaseCapture();
    model_.pressed = Control::None;
    model_.pressed_index = -1;
    model_.drag_out = true;
    drag_out_active_ = true;
    invalidate();
    UpdateWindow(hwnd_);
    drag_folder(fs::from_text(model_.output_path));
    drag_out_active_ = false;
    model_.drag_out = false;
    model_.hot = Control::None;
    invalidate();
}

// A finished run that the person is not looking at (the window is not in front, or is minimised) says so.
void WwApp::notify_finished() {
    if (!notifier_ || run_results_.empty()) return;
    if (GetForegroundWindow() == hwnd_ && !IsIconic(hwnd_)) return;
    notifier_->show(notify::compose(run_results_));
}

// A button or click on the notification: Open folder, or Show (the window comes forward).
void WwApp::on_toast(WPARAM action) {
    if (action == 1) {
        open_output();
        return;
    }
    if (IsIconic(hwnd_)) ShowWindow(hwnd_, SW_RESTORE);
    SetForegroundWindow(hwnd_);
}

// ---- the fades

// The welcome as the window opens: its words fade in, stay, fade out, then the page fades in. Not when Windows' animations are off.
void WwApp::start_welcome() {
    if (!ww_animations_on()) return;
    model_.welcome = true;
    model_.welcome_alpha = 0;
    model_.chrome_alpha = 0;
    welcome_start_ = std::chrono::steady_clock::now();
    SetTimer(hwnd_, kWwFadeTimer, kWwFadeFrameMs, nullptr);
}

// A click or a key during the welcome: straight to the page fading in.
void WwApp::skip_welcome() {
    if (!model_.welcome) return;
    const double elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - welcome_start_).count();
    if (elapsed < kWwWelcomeOut)
        welcome_start_ = std::chrono::steady_clock::now() - std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<double>(kWwWelcomeOut));
    model_.welcome_alpha = 0;
}

// The Convert page's one card changes between Program and Source: the old one fades out, then the new one fades in.
void WwApp::begin_card_fade(View from) {
    const bool was_source = from == View::Done, is_source = model_.view == View::Done;
    if (was_source == is_source || !ww_animations_on()) {
        model_.card_alpha = 1;
        model_.fade_from_alpha = 0;
        return;
    }
    model_.fade_from = from;
    model_.fade_from_alpha = 1;
    model_.card_alpha = 0;
    card_fading_ = true;
    fade_start_ = std::chrono::steady_clock::now();
    SetTimer(hwnd_, kWwFadeTimer, kWwFadeFrameMs, nullptr);
}

void WwApp::fade_frame() {
    const auto now = std::chrono::steady_clock::now();
    if (model_.welcome) {
        const double t = std::chrono::duration<double>(now - welcome_start_).count();
        if (t < kWwWelcomeIn) model_.welcome_alpha = ww_ease(t / kWwWelcomeIn);
        else if (t < kWwWelcomeHold) model_.welcome_alpha = 1;
        else if (t < kWwWelcomeOut) model_.welcome_alpha = 1 - ww_ease((t - kWwWelcomeHold) / (kWwWelcomeOut - kWwWelcomeHold));
        else model_.welcome_alpha = 0;
        model_.chrome_alpha = t < kWwWelcomeOut ? 0 : ww_ease((t - kWwWelcomeOut) / (kWwWelcomeEnd - kWwWelcomeOut));
        if (t >= kWwWelcomeEnd) {
            model_.welcome = false;
            model_.chrome_alpha = 1;
        }
    }
    if (card_fading_) {
        const double t = std::chrono::duration<double>(now - fade_start_).count();
        model_.fade_from_alpha = t < kWwCardOut ? 1 - ww_ease(t / kWwCardOut) : 0;
        model_.card_alpha = t < kWwCardOut ? 0 : ww_ease((t - kWwCardOut) / kWwCardIn);
        if (t >= kWwCardOut + kWwCardIn) {
            card_fading_ = false;
            model_.card_alpha = 1;
            model_.fade_from_alpha = 0;
        }
    }
    if (!model_.welcome && !card_fading_) KillTimer(hwnd_, kWwFadeTimer);
    invalidate();
}

// The stream's timer: type on by the time that passed, blink the cursor, draw. It stops when the conversion is no longer running.
void WwApp::stream_frame() {
    if (model_.view != View::Working) {
        KillTimer(hwnd_, kWwStreamTimer);
        return;
    }
    const auto now = std::chrono::steady_clock::now();
    const double seconds = std::min(0.25, std::chrono::duration<double>(now - stream_tick_).count());
    stream_tick_ = now;
    ++stream_frames_;
    const bool was_typing = stream_.typing();
    stream_.advance(seconds);
    const bool cursor = (stream_frames_ / 16) % 2 == 0;   // on and off about every half second
    if (!was_typing && !stream_.typing() && cursor == model_.stream_cursor) return;   // nothing changed on screen
    model_.stream = stream_.visible(kWwStreamRows);
    model_.stream_cursor = cursor;
    invalidate();
}

// The Source card (or the InfoBar) for the file `item` ended with; what the Program card showed as it ended (`stage`, `detail`, `stream`) fades out with it.
void WwApp::present_item(const batch::Item& item, const std::string& stage, const std::string& detail, const std::vector<std::string>& stream) {
    const View before = model_.view;
    reset_to_idle("Ready");
    model_.file_name = item.name;
    switch (item.state) {
        case batch::ItemState::Done: {
            model_.view = View::Done;
            model_.stage_label = stage;
            model_.detail = detail;
            model_.percent = 100;
            model_.stream = stream;
            model_.done_file_name = item.name;
            model_.output_path = narrow(item.output.wstring());
            model_.done_note = std::to_string(item.files) + (item.files == 1 ? " file written." : " files written.");
            if (!item.checked.empty()) model_.done_note += " Checked: " + item.checked;
            model_.status_text = "Done";
            load_preview_folder(item.output, fs::to_text(item.file.stem()));
            load_functions(item.output, item.functions, item.file, item.can_recheck);
            break;
        }
        case batch::ItemState::CannotConvert:
            model_.view = View::Unsupported;
            model_.info_title = "This file can't be converted";
            model_.info_message = item.message;
            model_.status_text = "Cannot convert this file";
            break;
        case batch::ItemState::Cancelled: model_.status_text = "Cancelled"; break;
        default:
            model_.view = View::Failed;
            model_.info_title = "The conversion failed";
            model_.info_message = item.message;
            model_.status_text = "Something went wrong";
            break;
    }
    begin_card_fade(before);
    relayout();
    invalidate();
}

// Every file of the queue has ended: the card shows the last one that was converted (or, when none was, how the last one ended), Recent has them all,
// the taskbar button is cleared (red when something failed), and a window that is not in front says so with a notification.
void WwApp::finish_run() {
    board_sync_states();   // (files still waiting when Cancel was pressed)
    const std::string stage = model_.stage_label, detail = model_.detail;
    const std::vector<std::string> stream = model_.stream;
    const std::size_t total = queue_.total(), converted = queue_.done();
    int shown = queue_.last_done();
    if (shown < 0) shown = static_cast<int>(total) - 1;
    const batch::Item item = queue_.items()[static_cast<std::size_t>(shown)];
    bool cancelled = false, failed = false;
    for (const batch::Item& i : queue_.items()) {
        cancelled = cancelled || i.state == batch::ItemState::Cancelled;
        failed = failed || i.state == batch::ItemState::Failed;
    }
    present_item(item, stage, detail, stream);
    if (total > 1) {
        if (cancelled) {
            model_.status_text = "Cancelled: " + std::to_string(converted) + " of " + std::to_string(total) + " converted";
        } else if (item.state == batch::ItemState::Done) {
            model_.status_text = "Done: " + std::to_string(converted) + " of " + std::to_string(total) + " converted";
        } else {   // none could be converted: the InfoBar says so
            model_.info_title = "None of the " + std::to_string(total) + " files could be converted";
        }
    }
    taskbar_.set(failed ? Taskbar::State::Error : Taskbar::State::None, 100);
    if (failed && model_.window_active) SetTimer(hwnd_, kWwTaskbarTimer, kWwTaskbarErrorMs, nullptr);
    if (!cancelled) notify_finished();
    queue_.clear();
    running_item_ = -1;
    relayout();
    invalidate();
}

void WwApp::on_finished() {
    if (worker_.joinable()) worker_.join();
    std::optional<convert::Outcome> outcome;
    {
        std::lock_guard hold(lock_);
        outcome = std::move(outcome_);
        outcome_.reset();
    }
    progress_posted_ = false;
    if (!outcome) return;
    const int index = running_item_;
    running_item_ = -1;
    if (index < 0 || static_cast<std::size_t>(index) >= queue_.total()) return;
    batch::Item ended;
    ended.output = outcome->output;
    ended.files = outcome->files;
    ended.message = outcome->message;
    ended.checked = outcome->checked;
    ended.functions = outcome->functions;
    ended.can_recheck = outcome->can_recheck;
    batch::ItemState state = batch::ItemState::Failed;
    switch (outcome->state) {
        case convert::State::Done: state = batch::ItemState::Done; break;
        case convert::State::Unsupported: state = batch::ItemState::CannotConvert; break;
        case convert::State::Cancelled: state = batch::ItemState::Cancelled; break;
        default: break;
    }
    queue_.finish(static_cast<std::size_t>(index), state, std::move(ended));
    board_sync_states();
    const batch::Item& item = queue_.items()[static_cast<std::size_t>(index)];
    if (state == batch::ItemState::Done) remember_item(item);
    if (state != batch::ItemState::Cancelled) {
        notify::Result result;
        result.name = item.name;
        result.outcome = state == batch::ItemState::Done ? notify::Outcome::Done : state == batch::ItemState::CannotConvert ? notify::Outcome::CannotConvert : notify::Outcome::Failed;
        result.files = item.files;
        result.folder = fs::to_text(item.output.filename());
        result.detail = item.message;
        run_results_.push_back(std::move(result));
    }
    if (queue_.next_waiting() >= 0) {   // the next file of the queue
        sync_queue();
        run_next();
        return;
    }
    finish_run();
}

LRESULT CALLBACK WwApp::proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_NCCREATE) {
        auto* cs = reinterpret_cast<CREATESTRUCTW*>(lp);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
        static_cast<WwApp*>(cs->lpCreateParams)->hwnd_ = hwnd;
    }
    WwApp* app = ww_app(hwnd);
    return app != nullptr ? app->handle(msg, wp, lp) : DefWindowProcW(hwnd, msg, wp, lp);
}

LRESULT WwApp::handle(UINT msg, WPARAM wp, LPARAM lp) {
    if (msg != 0 && msg == taskbar_message_) {   // the taskbar button exists (it is made again when Explorer restarts): progress can be shown on it
        taskbar_.attach(hwnd_);
        return 0;
    }
    switch (msg) {
        case WM_CREATE: {
            dpi_ = GetDpiForWindow(hwnd_);
            // Mica: the frame extends over the whole window and the system draws the backdrop behind it (Windows 11 22H2); where that is refused the
            // window is a solid colour.
            const int corner = kWwRound;
            DwmSetWindowAttribute(hwnd_, kWwWindowCornerPreference, &corner, sizeof corner);
            const int backdrop = kWwMainWindowBackdrop;
            backdrop_ = SUCCEEDED(DwmSetWindowAttribute(hwnd_, kWwSystemBackdropType, &backdrop, sizeof backdrop)) && !fs::get_env("CB_NO_BACKDROP");
            if (backdrop_) {
                const MARGINS all{-1, -1, -1, -1};
                DwmExtendFrameIntoClientArea(hwnd_, &all);
                // with "accent colour on title bars" on, the system paints its (unused) caption strip in the accent colour over the backdrop: none
                const DWORD none = kWwColorNone;
                DwmSetWindowAttribute(hwnd_, kWwCaptionColor, &none, sizeof none);
            }
            apply_theme();
            notifier_ = std::make_unique<Notifier>(hwnd_, kWmToast, kWmTray);
            RegisterDragDrop(hwnd_, &drop_);
            SetWindowPos(hwnd_, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);   // (the system takes its caption away: WM_NCCALCSIZE)
            return 0;
        }
        case WM_NCCALCSIZE: {
            if (wp == TRUE) {
                // the system takes its borders; the caption it would take stays ours (the client area reaches the top)
                auto* params = reinterpret_cast<NCCALCSIZE_PARAMS*>(lp);
                const LONG top = params->rgrc[0].top;
                const LRESULT result = DefWindowProcW(hwnd_, msg, wp, lp);
                if (result != 0) return result;
                params->rgrc[0].top = top;
                if (IsZoomed(hwnd_)) params->rgrc[0].top += GetSystemMetricsForDpi(SM_CYFRAME, dpi_) + GetSystemMetricsForDpi(SM_CXPADDEDBORDER, dpi_);
                return 0;
            }
            return DefWindowProcW(hwnd_, msg, wp, lp);
        }
        case WM_NCHITTEST: {
            const LRESULT system = DefWindowProcW(hwnd_, msg, wp, lp);
            if (system != HTCLIENT) return system;   // the side and bottom borders
            POINT p{GET_X_LPARAM(lp), GET_Y_LPARAM(lp)};
            ScreenToClient(hwnd_, &p);
            RECT rc{};
            GetClientRect(hwnd_, &rc);
            const int border = GetSystemMetricsForDpi(SM_CYFRAME, dpi_) + GetSystemMetricsForDpi(SM_CXPADDEDBORDER, dpi_) - 2;
            if (!IsZoomed(hwnd_) && p.y < border) {
                if (p.x < border) return HTTOPLEFT;
                if (p.x >= rc.right - border) return HTTOPRIGHT;
                return HTTOP;
            }
            const float x = ww_dip(p.x, dpi_), y = ww_dip(p.y, dpi_);
            if (layout_.caption_close.contains(x, y)) return HTCLOSE;
            if (layout_.caption_maximize.contains(x, y)) return HTMAXBUTTON;
            if (layout_.caption_minimize.contains(x, y)) return HTMINBUTTON;
            if (layout_.app_icon.contains(x, y)) return HTSYSMENU;
            if (y < kTitleBarHeight) return HTCAPTION;
            return HTCLIENT;
        }
        case WM_NCMOUSEMOVE: {
            Control c = Control::None;
            if (wp == HTCLOSE) c = Control::CaptionClose;
            else if (wp == HTMAXBUTTON) c = Control::CaptionMaximize;
            else if (wp == HTMINBUTTON) c = Control::CaptionMinimize;
            set_hot(c);
            if (!tracking_) {
                TRACKMOUSEEVENT t{sizeof t, TME_LEAVE | TME_NONCLIENT, hwnd_, 0};
                tracking_ = TrackMouseEvent(&t) != FALSE;
            }
            return DefWindowProcW(hwnd_, msg, wp, lp);
        }
        case WM_NCMOUSELEAVE:
            tracking_ = false;
            set_hot(Control::None);
            return 0;
        case WM_NCLBUTTONDOWN:
            if (wp == HTSYSMENU) {   // a click on the icon
                RECT rc{};
                GetWindowRect(hwnd_, &rc);
                show_system_menu(POINT{rc.left + 8, rc.top + static_cast<LONG>(kTitleBarHeight * static_cast<float>(dpi_) / 96.0f)});
                return 0;
            }
            if (wp == HTCLOSE || wp == HTMAXBUTTON || wp == HTMINBUTTON) {   // the caption buttons are ours: the click is acted on at the release
                model_.pressed = wp == HTCLOSE ? Control::CaptionClose : wp == HTMAXBUTTON ? Control::CaptionMaximize : Control::CaptionMinimize;
                SetCapture(hwnd_);
                invalidate();
                return 0;
            }
            break;
        case WM_NCRBUTTONUP:
            if (wp == HTCAPTION || wp == HTSYSMENU) {
                show_system_menu(POINT{GET_X_LPARAM(lp), GET_Y_LPARAM(lp)});
                return 0;
            }
            break;
        case WM_XBUTTONUP:   // the mouse's Back button: one breadcrumb back
            if (model_.page == Page::Board && model_.board_view != 0 && GET_XBUTTON_WPARAM(wp) == XBUTTON1) {
                board_step_back(model_);
                sync_board_title();
                relayout();
                invalidate();
                return TRUE;
            }
            break;
        case WM_SYSKEYDOWN:
            if (wp == VK_LEFT && model_.page == Page::Board && model_.board_view != 0) {   // Alt+Left: one breadcrumb back
                board_step_back(model_);
                sync_board_title();
                relayout();
                invalidate();
                return 0;
            }
            if (wp == VK_SPACE) {   // Alt+Space
                RECT rc{};
                GetWindowRect(hwnd_, &rc);
                show_system_menu(POINT{rc.left + 8, rc.top + static_cast<LONG>(kTitleBarHeight * static_cast<float>(dpi_) / 96.0f)});
                return 0;
            }
            if ((wp == VK_DOWN || wp == VK_UP) && model_.focus == Control::Language) {   // Alt+Down: drop the Language list down
                language_key(wp, true);
                return 0;
            }
            break;
        case WM_NCLBUTTONUP:
            if (wp == HTCLOSE || wp == HTMAXBUTTON || wp == HTMINBUTTON) return 0;
            break;
        case WM_MOUSEMOVE: {
            const float mx = ww_dip(GET_X_LPARAM(lp), dpi_), my = ww_dip(GET_Y_LPARAM(lp), dpi_);
            if (board_press_card_ >= 0 && (wp & MK_LBUTTON) != 0) {   // a card pressed: moving it a little picks it up
                if (model_.board_drag < 0 && (std::abs(GET_X_LPARAM(lp) - board_press_at_.x) >= GetSystemMetricsForDpi(SM_CXDRAG, dpi_) ||
                                              std::abs(GET_Y_LPARAM(lp) - board_press_at_.y) >= GetSystemMetricsForDpi(SM_CYDRAG, dpi_)))
                {
                    model_.board_drag = board_press_card_;
                    board_hover_card(model_, -1);
                }
                if (model_.board_drag >= 0) {
                    model_.board_drag_x = mx;
                    model_.board_drag_y = my;
                    model_.board_drop_pile = board_foundation_at(compute_board_layout(layout_, model_), model_, mx, my);
                    invalidate();
                    return 0;
                }
            }
            if (drag_pending_) {   // the output box was pressed: moving a little with the button down drags the folder out
                if ((wp & MK_LBUTTON) == 0) {
                    drag_pending_ = false;
                } else if (std::abs(GET_X_LPARAM(lp) - drag_origin_.x) >= GetSystemMetricsForDpi(SM_CXDRAG, dpi_) ||
                           std::abs(GET_Y_LPARAM(lp) - drag_origin_.y) >= GetSystemMetricsForDpi(SM_CYDRAG, dpi_)) {
                    begin_drag_out();
                    return 0;
                }
            }
            if (model_.palette_open) {   // the row under the pointer is the chosen one
                const int pick = palette_pick_at(compute_palette_layout(layout_, model_), model_, mx, my);
                if (pick >= 0 && pick != model_.palette_selected) {
                    model_.palette_selected = pick;
                    invalidate();
                }
                return 0;
            }
            if (model_.cmp_picker_open && model_.page == Page::Convert) {
                const int item = picker_item_at(compute_picker_layout(layout_, model_), mx, my);
                if (item >= 0 && item != model_.cmp_picker_hot) {
                    model_.cmp_picker_hot = item;
                    invalidate();
                }
            }
            if (model_.language_open && model_.page == Page::Convert) {   // over the open list: the item under the pointer is the one a click chooses
                const int item = language_item_at(language_list(), mx, my);
                const int hot = item >= 0 && model_.languages[static_cast<std::size_t>(item)].supported ? item : -1;
                if (hot != model_.language_hot) {
                    model_.language_hot = hot;
                    invalidate();
                }
            }
            if (model_.field_popup >= 0) {
                const int item = language_item_at(field_popup_list(), mx, my);
                if (item != model_.language_hot) {
                    model_.language_hot = item;
                    invalidate();
                }
            }
            const PageHit h = hit_at(lp);
            set_hot(h.control, h.index);
            if (model_.page == Page::Board && model_.board_view == 0) {   // the card under the pointer lights what it is linked to
                const int card = h.control == Control::BoardCard ? h.index : -1;
                if (card != (model_.board_hover ? static_cast<int>(model_.board_hover->card) : -1)) {
                    board_hover_card(model_, card);
                    sync_board_title();
                    invalidate();
                }
            }
            if (model_.page == Page::Board && model_.board_view == 3 && model_.board_library) {   // an export's lines stay while the pointer travels along them to its files
                int hover = model_.board_hover_export;
                if (h.control == Control::BoardRight && model_.board_library->function_is_empty()) hover = h.index;
                else if (h.control != Control::BoardUser) {
                    const LibraryLayout library = layout_library(compute_board_layout(layout_, model_).canvas, *model_.board_library, model_.board_left_scroll, model_.board_right_scroll, hover);
                    if (mx < library.right_area.x || !library.right_area.contains(library.right_area.x, my)) hover = -1;
                }
                if (hover != model_.board_hover_export) {
                    model_.board_hover_export = hover;
                    sync_board_title();
                    invalidate();
                }
            }
            if (!tracking_) {
                TRACKMOUSEEVENT t{sizeof t, TME_LEAVE, hwnd_, 0};
                tracking_ = TrackMouseEvent(&t) != FALSE;
            }
            return 0;
        }
        case WM_MOUSELEAVE:
            tracking_ = false;
            if (model_.board_hover) {
                board_hover_card(model_, -1);
                sync_board_title();
                invalidate();
            }
            if (model_.pressed == Control::None) set_hot(Control::None);
            return 0;
        case WM_MOUSEWHEEL: {
            POINT p{GET_X_LPARAM(lp), GET_Y_LPARAM(lp)};
            ScreenToClient(hwnd_, &p);
            const float wx = ww_dip(p.x, dpi_), wy = ww_dip(p.y, dpi_);
            const int notches = GET_WHEEL_DELTA_WPARAM(wp) / WHEEL_DELTA;
            if (!scroll_convert(wx, wy, notches, (GET_KEYSTATE_WPARAM(wp) & MK_SHIFT) != 0)) scroll_page(wx, wy, notches);
            return 0;
        }
        case WM_CHAR: {
            if (model_.palette_open) {   // typing in the palette
                if (wp >= 32 && wp != 127) {
                    const wchar_t unit = static_cast<wchar_t>(wp);
                    if (unit >= 0xD800 && unit <= 0xDBFF) {
                        pending_high_ = unit;
                        return 0;
                    }
                    std::wstring text(1, unit);
                    if (pending_high_ != 0 && unit >= 0xDC00 && unit <= 0xDFFF) text = std::wstring{pending_high_, unit};
                    pending_high_ = 0;
                    model_.palette_query += narrow(text);
                    model_.palette_selected = 0;
                    palette_refresh();
                }
                return 0;
            }
            if (wp >= 32 && wp != 127 && (model_.field_popup < 0 || popup_is_search()) && edit_target() != nullptr) {
                const wchar_t unit = static_cast<wchar_t>(wp);
                if (unit >= 0xD800 && unit <= 0xDBFF) {   // the first half of a surrogate pair waits for the second
                    pending_high_ = unit;
                    return 0;
                }
                std::wstring text(1, unit);
                if (pending_high_ != 0 && unit >= 0xDC00 && unit <= 0xDFFF) text = std::wstring{pending_high_, unit};
                pending_high_ = 0;
                insert_text(narrow(text));
                return 0;
            }
            break;
        }
        case WM_TIMER:
            if (wp == kWwRunTimer) {
                poll_tool();
                return 0;
            }
            if (wp == kWwStreamTimer) {
                stream_frame();
                return 0;
            }
            if (wp == kWwFadeTimer) {
                fade_frame();
                return 0;
            }
            if (wp == kWwTaskbarTimer) {   // a failure has been on the taskbar button long enough for a window that is in front
                KillTimer(hwnd_, kWwTaskbarTimer);
                if (taskbar_.state() == Taskbar::State::Error) taskbar_.set(Taskbar::State::None);
                return 0;
            }
            if (wp == kWwFlowTimer) {
                const auto now = std::chrono::steady_clock::now();
                model_.board_flow_time = std::fmod(model_.board_flow_time + std::chrono::duration<double>(now - flow_tick_).count(), 3600.0);
                flow_tick_ = now;
                if (!IsIconic(hwnd_)) invalidate();
                return 0;
            }
            if (wp == kWwScoresTimer) {
                if (model_.page == Page::Board && !IsIconic(hwnd_) && board_refresh_scores(model_)) {
                    relayout();
                    invalidate();
                }
                return 0;
            }
            if (wp == kWwMinuteTimer) {   // "2 minutes ago" moves on
                if (model_.page == Page::Convert && model_.view == View::Idle && !model_.recent.empty()) refresh_recent();
                return 0;
            }
            break;
        case WM_LBUTTONDOWN: {
            const float mx = ww_dip(GET_X_LPARAM(lp), dpi_), my = ww_dip(GET_Y_LPARAM(lp), dpi_);
            if (model_.welcome && my >= kTitleBarHeight) {   // a click below the title bar during the welcome: on to the page (the caption buttons still work)
                skip_welcome();
                return 0;
            }
            if (model_.palette_open) {   // a click on a row runs it; a click outside the box closes the palette
                const PaletteLayout pl = compute_palette_layout(layout_, model_);
                const int pick = palette_pick_at(pl, model_, mx, my);
                if (pick >= 0) palette_run(pick);
                else if (!pl.box.contains(mx, my)) close_palette();
                return 0;
            }
            if (model_.cmp_picker_open && model_.page == Page::Convert) {   // the function drop-down: a click chooses a function or closes it
                const int item = picker_item_at(compute_picker_layout(layout_, model_), mx, my);
                open_picker(false);
                if (item >= 0) open_compare(item);
                return 0;
            }
            if (model_.field_popup >= 0) {   // a click while a form's list is open chooses an item or closes it, and does nothing else
                const int item = language_item_at(field_popup_list(), mx, my);
                choose_in_popup(item);
                return 0;
            }
            if (model_.language_open && model_.page == Page::Convert) {   // a click while the list is open chooses an item or closes the list, and does nothing else
                const int item = language_item_at(language_list(), mx, my);
                open_language_list(false);
                if (item >= 0) choose_language(item);
                return 0;
            }
            const PageHit h = hit_at(lp);
            if (model_.board_batch_armed > 0 && h.control != Control::BoardBatch) {   // a click anywhere else, on a control or not, takes the question back
                model_.board_batch_armed = 0;
                sync_board_title();
                invalidate();
            }
            if (model_.page == Page::Board && model_.board_decks_open && h.control != Control::BoardDeckRow && h.control != Control::BoardDeckRemove && h.control != Control::BoardDecks &&
                !layout_deck_panel(compute_board_layout(layout_, model_), model_).outline.contains(mx, my)) {   // a click outside the open list closes it
                model_.board_decks_open = false;
                invalidate();
                return 0;
            }
            if (model_.page == Page::Board && model_.board_signal_type >= 0 && h.control != Control::BoardSignalRow && h.control != Control::BoardSignalType &&
                !layout_signal_panel(compute_board_layout(layout_, model_), model_).outline.contains(mx, my)) {   // a click outside the open list closes it
                board_open_signals(model_, -1);
                invalidate();
                return 0;
            }
            if (model_.page == Page::Convert) {
                model_.focus = Control::None;
            } else {
                // a text box takes the keyboard at once (a switch or a drop-down acts when the click is released)
                const std::vector<FieldItem>& fields = model_.page == Page::Settings ? model_.setting_fields : model_.tool_fields;
                const bool text_field = h.control == Control::ToolSearch || h.control == Control::BoardSearch ||
                                        (h.control == Control::Field && h.index >= 0 && static_cast<std::size_t>(h.index) < fields.size() && fields[static_cast<std::size_t>(h.index)].kind <= 2);
                if (text_field) set_focus(h.control, h.index);
                else set_focus(Control::None, -1);
            }
            if (h.control != Control::None) {
                model_.pressed = h.control;
                model_.pressed_index = h.index;
                SetCapture(hwnd_);
                if (h.control == Control::BoardCard) {
                    board_press_card_ = h.index;
                    board_press_at_ = POINT{GET_X_LPARAM(lp), GET_Y_LPARAM(lp)};
                }
                if (h.control == Control::OutputBox && model_.view == View::Done) {
                    drag_pending_ = true;
                    drag_origin_ = POINT{GET_X_LPARAM(lp), GET_Y_LPARAM(lp)};
                }
            }
            set_hot(h.control, h.index);
            invalidate();
            return 0;
        }
        case WM_LBUTTONUP: {
            drag_pending_ = false;
            if (model_.board_drag >= 0) {
                model_.pressed = Control::None;
                model_.pressed_index = -1;
                board_release(ww_dip(GET_X_LPARAM(lp), dpi_), ww_dip(GET_Y_LPARAM(lp), dpi_));   // (before the capture goes: losing it ends a drag with no drop)
                if (GetCapture() == hwnd_) ReleaseCapture();
                return 0;
            }
            board_press_card_ = -1;
            const Control pressed = model_.pressed;
            const int pressed_index = model_.pressed_index;
            model_.pressed = Control::None;
            model_.pressed_index = -1;
            if (GetCapture() == hwnd_) ReleaseCapture();
            const PageHit h = hit_at(lp);
            set_hot(h.control, h.index);
            invalidate();
            if (pressed != Control::None && pressed == h.control && pressed_index == h.index) {
                if (pressed >= Control::NavConvert) {
                    if (pressed != Control::ToolSearch && pressed != Control::BoardSearch) activate_page(pressed, pressed_index);
                    sync_board_title();
                } else {
                    activate(pressed);
                }
            }
            return 0;
        }
        case WM_CAPTURECHANGED:
            if (model_.board_drag >= 0 || board_press_card_ >= 0) {   // (the drag is over without a drop)
                model_.board_drag = -1;
                model_.board_drop_pile = -1;
                board_press_card_ = -1;
                invalidate();
            }
            if (model_.pressed != Control::None) {
                model_.pressed = Control::None;
                invalidate();
            }
            return 0;
        case WM_KEYDOWN: {
            if (model_.welcome) {   // any key during the welcome: on to the page
                skip_welcome();
                return 0;
            }
            const bool ctrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
            if (ctrl && wp == 'K') {   // Ctrl+K: the command palette, anywhere
                if (model_.palette_open) close_palette();
                else open_palette();
                return 0;
            }
            if (model_.palette_open) {
                palette_key(wp);
                return 0;
            }
            if (ctrl && wp == VK_OEM_COMMA) {   // Ctrl+,: Settings, Advanced
                set_page(Page::Settings);
                open_advanced(true);
                return 0;
            }
            if (ctrl && wp == '4') {   // Ctrl+4: the Board
                set_page(Page::Board);
                return 0;
            }
            if (ctrl && wp >= '1' && wp <= '3') {   // Ctrl+1, 2, 3: Convert, Tools, Settings
                set_page(wp == '1' ? Page::Convert : wp == '2' ? Page::Tools : Page::Settings);
                return 0;
            }
            if (model_.field_popup >= 0 && !(popup_is_search() && wp != VK_UP && wp != VK_DOWN && wp != VK_RETURN && wp != VK_ESCAPE)) {   // a form's open list takes the keys
                popup_key(wp);
                return 0;
            }
            if (model_.page != Page::Convert) {
                if (wp == VK_TAB) {
                    move_focus((GetKeyState(VK_SHIFT) & 0x8000) != 0);
                    return 0;
                }
                if (edit_key(wp, ctrl)) return 0;
                if (model_.focus == Control::ToolRow && (wp == VK_UP || wp == VK_DOWN)) {   // the arrows move through the list
                    int at = -1;
                    for (std::size_t i = 0; i < model_.tool_rows.size(); ++i)
                        if (!model_.tool_rows[i].heading && model_.tool_rows[i].tool == model_.tool_selected) at = static_cast<int>(i);
                    const int step = wp == VK_DOWN ? 1 : -1;
                    for (int i = at + step; i >= 0 && i < static_cast<int>(model_.tool_rows.size()); i += step) {
                        if (model_.tool_rows[static_cast<std::size_t>(i)].heading) continue;
                        select_tool(model_.tool_rows[static_cast<std::size_t>(i)].tool);
                        const ToolsLayout t = compute_tools_layout(layout_, model_);   // keep the chosen row in view
                        const Rect& r = t.rows[static_cast<std::size_t>(i)];
                        if (r.y < t.list_area.y) model_.tool_scroll = std::max(0.0f, model_.tool_scroll - (t.list_area.y - r.y));
                        else if (r.bottom() > t.list_area.bottom()) model_.tool_scroll += r.bottom() - t.list_area.bottom();
                        break;
                    }
                    return 0;
                }
                if (wp == VK_ESCAPE && run_ != nullptr) {
                    stop_tool();
                    return 0;
                }
                if ((wp == VK_RETURN || wp == VK_SPACE) && (lp & (1 << 30)) == 0 && model_.focus != Control::None && model_.focus != Control::ToolSearch &&
                    !(model_.focus == Control::Field && edit_target() != nullptr)) {
                    activate_page(model_.focus, model_.focus_index);
                    return 0;
                }
                if (wp == VK_F4 && model_.focus == Control::Field) {
                    activate_page(Control::Field, model_.focus_index);
                    return 0;
                }
                break;
            }
            if (model_.language_open && wp != VK_TAB) {   // the open list takes the keys
                language_key(wp, false);
                return 0;
            }
            if (model_.language_open) open_language_list(false);
            if (model_.focus == Control::Language && wp != VK_TAB && wp != VK_RETURN && wp != VK_SPACE && wp != VK_ESCAPE && !ctrl) {
                language_key(wp, false);
                if (wp == VK_UP || wp == VK_DOWN || wp == VK_F4) return 0;
            }
            if (wp != VK_TAB && convert_key(wp, ctrl)) return 0;
            if (wp == VK_TAB) {
                move_focus((GetKeyState(VK_SHIFT) & 0x8000) != 0);
                return 0;
            }
            if (wp == VK_ESCAPE && running_) {
                cancel_conversion();
                return 0;
            }
            if (ctrl && wp == 'O') {
                browse();
                return 0;
            }
            if ((wp == VK_RETURN || wp == VK_SPACE) && (lp & (1 << 30)) == 0) {
                if (model_.focus >= Control::NavConvert) activate_page(model_.focus, model_.focus_index);
                else if (model_.focus != Control::None) activate(model_.focus);
                else if (model_.view == View::Idle && wp == VK_RETURN) browse();
                return 0;
            }
            break;
        }
        case WM_SIZE:
            if (target_) {
                RECT rc{};
                GetClientRect(hwnd_, &rc);
                target_->Resize(D2D1::SizeU(static_cast<UINT>(std::max<LONG>(1, rc.right - rc.left)), static_cast<UINT>(std::max<LONG>(1, rc.bottom - rc.top))));
            }
            model_.maximized = wp == SIZE_MAXIMIZED;
            relayout();
            invalidate();
            return 0;
        case WM_DPICHANGED: {
            dpi_ = HIWORD(wp);
            const RECT* r = reinterpret_cast<const RECT*>(lp);
            SetWindowPos(hwnd_, nullptr, r->left, r->top, r->right - r->left, r->bottom - r->top, SWP_NOZORDER | SWP_NOACTIVATE);
            relayout();
            invalidate();
            return 0;
        }
        case WM_GETMINMAXINFO: {
            auto* info = reinterpret_cast<MINMAXINFO*>(lp);
            RECT wr{}, cr{};
            if (hwnd_ != nullptr && GetWindowRect(hwnd_, &wr) && GetClientRect(hwnd_, &cr)) {
                const float s = static_cast<float>(dpi_) / 96.0f;
                info->ptMinTrackSize.x = static_cast<LONG>(kMinWidth * s) + (wr.right - wr.left) - (cr.right - cr.left);
                info->ptMinTrackSize.y = static_cast<LONG>(kMinHeight * s) + (wr.bottom - wr.top) - (cr.bottom - cr.top);
            }
            return 0;
        }
        case WM_ACTIVATE:
            model_.window_active = LOWORD(wp) != WA_INACTIVE;
            if (model_.window_active) {   // looked at again: a failure on the taskbar button has been seen, and folders may have moved since
                if (taskbar_.state() == Taskbar::State::Error) taskbar_.set(Taskbar::State::None);
                if (!running_) refresh_recent();
            }
            if (!model_.window_active) model_.language_open = false;
            invalidate();
            break;
        case WM_SETTINGCHANGE:
            if (lp != 0 && lstrcmpW(reinterpret_cast<LPCWSTR>(lp), L"ImmersiveColorSet") == 0) {   // the theme or the accent colour changed: follow it live
                apply_theme();
                invalidate();
            }
            break;
        case WM_ERASEBKGND: return 1;
        case WM_PAINT: paint(); return 0;
        case kWmProgress: on_progress(); return 0;
        case kWmFinished: on_finished(); return 0;
        case kWmBoardProgress: on_board_progress(); return 0;
        case kWmBoardDone: on_board_done(); return 0;
        case kWmToast: on_toast(wp); return 0;
        case kWmTray:
            if (notifier_) notifier_->on_tray(lp);
            return 0;
        case WM_CLOSE:
            if (running_) cancel_ = true;
            board_cancel_ = true;
            break;
        case WM_DESTROY:
            RevokeDragDrop(hwnd_);
            PostQuitMessage(0);
            return 0;
        default: break;
    }
    return DefWindowProcW(hwnd_, msg, wp, lp);
}

// ---- screenshots -----------------------------------------------------------------------------------------------------------------------

struct WwShot {
    std::string board_focus;   // the Flow and Trace states centre on this file
    std::string board_width;   // and size their channels by "files", "links" or "calls"
    std::string path, state = "idle", theme, board_root;   // board_root: the Board states draw this kept board instead of the sample
    float width = kDefaultWidth, height = kDefaultHeight, scale = 1.0f;
};

// The sample the Board states draw: six thousand made-up cards (the screenshot shows no one's own files), some converted, one failed.
std::shared_ptr<board::Board> ww_board_sample() {
    static constexpr int kCounts[board::kPileCount] = {412, 980, 640, 720, 130, 96, 84, 1530, 31, 1377};
    static constexpr const char* kExt[board::kPileCount] = {"exe", "dll", "vxd", "inf", "txt", "hlp", "ttf", "bmp", "cab", "dat"};
    static constexpr const char* kFormat[board::kPileCount] = {"PE", "PE", "LE", "text", "text", "", "", "", "CAB", ""};
    auto b = std::make_shared<board::Board>();
    b->source = "Sample OS.iso";
    b->complete = true;
    b->cabinets = 31;
    for (std::size_t p = 0; p < board::kPileCount; ++p) {
        for (int i = 0; i < kCounts[p]; ++i) {
            board::Card c;
            c.name = std::format("sample{:04}.{}", i, kExt[p]);
            c.rel = std::format("disc/{}/{}", kExt[p], c.name);
            c.size = 1024ull + static_cast<std::uint64_t>(i) * 733;
            c.pile = static_cast<board::Pile>(p);
            c.format = kFormat[p];
            if (p <= 2 && i % 5 == 0) c.conv = board::Conv::Done;
            else if (p <= 2 && i % 17 == 1) c.conv = board::Conv::CannotConvert;
            else if (p == 1 && i == 3) c.conv = board::Conv::Failed;
            b->cards.push_back(std::move(c));
        }
    }
    return b;
}

Model ww_model_for(const std::string& state, bool dark, const std::string& board_root = {}, const std::string& board_focus_name = {}, const std::string& board_width = {}) {
    Model m;
    m.dark = dark;
    m.platforms = convert::platform_summary();
    m.drop_hint = convert::drop_hint();
    m.languages = ww_items(convert::targets_for({}));
    if (state.starts_with("board")) {   // board (fans), board-expanded (a long column, a card selected), board-empty, board-scanning
        m.page = Page::Board;
        if (state == "board-empty") return m;
        if (state == "board-scanning") {
            m.board_busy = true;
            m.board_stage = "Opening cabinets";
            m.board_detail = "12 of 31";
            m.board_percent = 69;
            m.status_text = "Laying out Sample OS.iso";
            return m;
        }
        if (!board_root.empty()) {
            std::optional<board::Board> kept = board::load(fs::from_text(board_root));
            if (kept) m.board = std::make_shared<board::Board>(std::move(*kept));
        }
        if (!m.board) {
            m.board = ww_board_sample();
            auto links = std::make_shared<board::Index>();   // made-up links for the sample: programs on twelve libraries, libraries on three
            links->nodes.resize(m.board->cards.size());
            for (std::uint32_t i = 0; i < links->nodes.size(); ++i) links->nodes[i].group = i;
            for (std::uint32_t i = 0; i < 412; ++i) links->edges.push_back({i, 412 + i % 12, board::Link::Imports, 100});
            for (std::uint32_t i = 415; i < 1392; ++i) links->edges.push_back({i, 412 + i % 3, board::Link::Imports, 100});
            links->finish();
            m.board_index = links;
        } else {
            if (std::optional<board::Index> kept = board::load_index(*m.board)) m.board_index = std::make_shared<const board::Index>(std::move(*kept));
            if (std::optional<board::Signals> kept = board::load_signals(*m.board)) m.board_signals = std::make_shared<const board::Signals>(std::move(*kept));
        }
        board_load_decks(m);
        board_rebuild_stacks(m);
        if (state == "board-orphans") board_toggle_orphans(m);
        if (state == "board-c-identical") board_toggle_c_view(m, board::CView::MostIdentical);
        if (state == "board-c-near") board_toggle_c_view(m, board::CView::NearMisses);
        if (state == "board-c-nocompile") board_toggle_c_view(m, board::CView::WillNotCompile);
        if (state == "board-c-unscored") board_toggle_c_view(m, board::CView::NotScored);
        if (state.starts_with("board-c-") && !m.board_stacks[static_cast<std::size_t>(board::Pile::Programs)].done.empty())
            m.board_selected = static_cast<int>(m.board_stacks[static_cast<std::size_t>(board::Pile::Programs)].done[0]);   // (its result on the strip)
        if (state == "board-deck") {
            board_apply_deck(m, 0);
            m.board_decks_open = true;
        }
        if (state == "board-filter" && m.board_signals) {   // the first Mutex shared by the most files, as a filter, with the list open
            board_open_signals(m, static_cast<int>(board::SignalType::Mutex));
            if (!m.board_signal_rows.empty()) board_toggle_filter(m, m.board_signal_rows[0].first);
        }
        if ((state == "board-inspect" || state == "board-hover") && m.board_index) {   // a named file (else the first program), in the drawer or under the pointer
            int card = -1;
            const std::string wanted = text::lower(board_focus_name);
            for (const board::Stack& stack : m.board_stacks) {
                for (const std::uint32_t i : stack.todo)
                    if (card < 0 && (wanted.empty() ? m.board->cards[i].pile == board::Pile::Programs : text::lower(m.board->cards[i].name) == wanted)) card = static_cast<int>(i);
            }
            for (std::size_t i = 0; card < 0 && !wanted.empty() && i < m.board->cards.size(); ++i)   // (a converted file is in no stack's to-do list)
                if (text::lower(m.board->cards[i].name) == wanted) card = static_cast<int>(i);
            if (state == "board-inspect") board_inspect_card(m, card);
            else {
                board_hover_card(m, card);
                m.hot = Control::BoardCard;
                m.hot_index = card;
            }
        }
        if (state == "board-flow" || state == "board-trace" || state.starts_with("board-library")) {
            m.board_width = board_width == "links" ? board::Width::Links : board_width == "calls" ? board::Width::Calls : board::Width::Files;
            board_show_view(m, state == "board-flow" ? 1 : state == "board-trace" ? 2 : 3);
            if (m.board_graph && !board_focus_name.empty()) {
                const std::string wanted = text::lower(board_focus_name);
                for (std::size_t i = 0; i < m.board_graph->modules.size(); ++i)
                    if (m.board_graph->modules[i].name == wanted) board_go_to(m, static_cast<int>(i));
            }
            if (state == "board-library-hover" && m.board_library && !m.board_library->right.empty()) m.board_hover_export = 0;    // the heaviest export, its lines showing
            if (state == "board-library-function" && m.board_library && !m.board_library->right.empty()) board_go_to(m, m.board_focus, m.board_library->right[0].key);
        }
        m.status_text = board::count_words(m.board->cards.size()) + " files on the board";
        if (m.board_scores) m.status_text += " · " + board::c_score_words(*m.board_scores);
        if (state == "board-expanded") {
            m.board_expanded[1] = true;
            m.board_scroll[1] = 46 * 200;
            if (m.board_stacks[1].todo.size() > 205) m.board_selected = static_cast<int>(m.board_stacks[1].todo[203]);
            else if (!m.board_stacks[1].todo.empty()) m.board_selected = static_cast<int>(m.board_stacks[1].todo[0]);
        }
        return m;
    }
    if (state == "welcome") {   // the welcome as the window opens, its words fully shown
        m.welcome = true;
        m.welcome_alpha = 1;
        m.chrome_alpha = 0;
        return m;
    }
    if (state == "tools") {   // a tool selected, its form filled in, a run finished (sample text: the screenshot shows no one's own files)
        m.page = Page::Tools;
        m.status_text = "Done";
        ww_fill_tool_rows(m);
        const tools::Tool* t = tools::find("java-port");
        tools::Values v = tools::empty_values(*t);
        v.inputs[0] = "C:\\Games\\Minecraft\\a1.2.6.jar";
        v.choice = 1;
        ww_load_tool(m, ww_tool_index(t), v);
        m.tool_done = true;
        m.tool_percent = 100;
        m.tool_note = "Finished";
        m.tool_result = "C:\\Games\\Minecraft\\a1.2.6-converted";
        m.tool_output =
            "C:\\Games\\Minecraft\\a1.2.6.jar -> C++ for Xbox 360\n  input: a game jar\n  0%  Decompiling Minecraft a1.2.6\n 14%  Converting the Java to C++\n"
            "  612 class(es) -> 618 header(s), 640 source(s)\n 28%  Writing the platform config and the Visual Studio solution\n 42%  Copying the game's textures and resources into Content\n"
            " 57%  Making the XACT audio banks\n 71%  Mapping the achievements\n 85%  Building the solution\n  built\n100%  Done\nOutput: C:\\Games\\Minecraft\\a1.2.6-converted\n";
        return m;
    }
    if (state == "settings" || state == "advanced") {
        m.page = Page::Settings;
        m.settings_advanced = state == "advanced";
        m.status_text = "Saved: " + settings::plain_label("low_load");
        const auto row = [](const char* key, const char* value, settings::Kind kind, bool present) {
            return settings::Row{key, value, kind, present, *value != 0, settings::plain_label(key), settings::plain_hint(key)};
        };
        const std::vector<settings::Row> rows = {
            row("ilspycmd", "C:\\Tools\\ilspycmd.exe", settings::Kind::Path, true),
            row("java", "C:\\Program Files\\Java\\jdk-21", settings::Kind::Path, true),
            row("xbox360_sdk", "", settings::Kind::Path, false),
            {"openwatcom", "C:\\Tools\\WATCOM", settings::Kind::Path, true, true, settings::plain_label("openwatcom"), settings::plain_hint("openwatcom")},
            {"converted", "C:\\CodeBreaker\\Converted", settings::Kind::Path, true, true, settings::plain_label("converted"), settings::plain_hint("converted")},
            {"workers", "4", settings::Kind::Text, true, true, settings::plain_label("workers"), settings::plain_hint("workers")},
            {"low_load", "true", settings::Kind::Toggle, true, true, settings::plain_label("low_load"), settings::plain_hint("low_load")},
        };
        ww_fill_settings(m, rows, dark ? settings::Theme::Dark : settings::Theme::Light);
        return m;
    }
    if (state == "language" || state == "translating") {
        // a C# file is chosen: the list is what the translator says it can write from C# (C++17 chosen)
        m.languages = ww_items(translate::capability::targets_for_source("csharp"));
        m.language_selected = ww_index_of(m.languages, "cpp17");
    }
    if (state == "language") {
        m.language_open = true;
        m.language_hot = m.language_selected;
        m.file_name = "Bank.cs";
        m.status_text = "Ready";
    } else if (state == "translating") {
        m.view = View::Working;
        m.file_name = "Bank.cs";
        m.stage_label = std::string(convert::state_label(convert::State::Translating));
        m.detail = "9 of 14 functions";
        m.percent = 31;
        m.status_text = "Translating Bank.cs to C++17";
        // a frame of the character stream (sample text in the shape the translator writes)
        m.stream = {"=== Bank.cpp ===", "#include \"cs_runtime.hpp\"", "namespace Bank {", "class Account {", "public:",
                    "    std::int64_t Balance = 0;", "    void Deposit(std::int64_t amount) {", "        if (amount <= 0) throw cs::ArgumentException(\"amount\");",
                    "        Balance += amount;", "    }", "    bool Withdraw(std::int64_t amount) {", "        if (amount > Balance) return false;",
                    "        Balance -= amount;", "        return true;", "    }", "};", "std::int64_t Total(const cs::List<Account>& all) {",
                    "    std::int64_t sum = 0;", "    for (const Account& a : all) sum += a.Balance;", "    return sum;", "}", "int Main() {",
                    "    cs::List<Account> accounts;", "    accounts.Add(Account{});", "    accounts[0].Dep"};
        m.stream_cursor = true;
    } else if (state == "working") {
        m.view = View::Working;
        m.file_name = "setup.exe";
        m.stage_label = std::string(convert::state_label(convert::State::Decompiling));
        m.detail = "17 of 39 functions";
        m.percent = 62;
        m.status_text = "Converting setup.exe";
        // a frame of the character stream (sample text in the shape the writers produce: the file's bytes, then its assembly source)
        m.stream = {"0000  4D 5A 90 00 03 00 00 00  04 00 00 00 FF FF 00 00  MZ..............",
                    "0010  B8 00 00 00 00 00 00 00  40 00 00 00 00 00 00 00  ........@.......",
                    "=== setup.asm ===", "; setup.exe -- 32-bit Windows program (PE), i386", "bits 32", "section .text",
                    "WinMain:", "        push    ebp", "        mov     ebp, esp", "        sub     esp, 0x54", "        push    ebx",
                    "        push    esi", "        push    edi", "        push    0", "        call    [GetModuleHandleA]",
                    "        mov     [hInstance], eax", "        call    InitApplication", "        test    eax, eax", "        jz      .quit",
                    "        push    dword [ebp+0x14]", "        push    eax", "        call    InitInstance", "        test    eax, eax",
                    "        jz      .quit", ".pump:", "        lea     eax, [ebp-0x54]", "        push    0", "        push    0",
                    "        push    0", "        push    eax", "        call    [GetMess"};
        m.stream_cursor = true;
    } else if (state == "done") {
        m.view = View::Done;
        m.file_name = "lz32.dll";
        m.output_path = "C:\\CodeBreaker\\Converted\\lz32";   // (a sample: the screenshot shows no one's own folders)
        m.done_note = "12 files written.";
        m.status_text = "Done";
    } else if (state == "unsupported") {
        m.view = View::Unsupported;
        m.file_name = "photo.png";
        m.info_title = "This file can't be converted";
        m.info_message = "This looks like a PNG image. CodeBreaker cannot convert it.";
        m.status_text = "Cannot convert this file";
    } else if (state == "failed") {
        m.view = View::Failed;
        m.file_name = "broken.exe";
        m.info_title = "The conversion failed";
        m.info_message = "The file is cut short: its section table runs past the end of the file.";
        m.status_text = "Something went wrong";
    } else if (state == "recent") {   // the Program card with the Recent list (sample rows: the screenshot shows no one's own files)
        m.recent = {{"lz32.dll", "C", "12 files \xC2\xB7 2 minutes ago", false, false},
                    {"Bank.cs", "C++17", "3 files \xC2\xB7 today 11:52 PM", false, false},
                    {"gothicvania.sfc", "Assembly", "4 files \xC2\xB7 yesterday", false, true}};
        m.hot = Control::RecentRow;
        m.hot_index = 0;
        m.status_text = "Ready";
    } else if (state == "batch") {   // several files: the queue, one running
        m.view = View::Working;
        m.file_name = "setup.exe";
        m.queue = {{"lz32.dll", "", "Done", 2, 100},
                   {"ver.dll", "", "Done", 2, 100},
                   {"setup.exe", "Decompiling \xC2\xB7 17 of 39 functions", "62%", 1, 62},
                   {"mpr.dll", "", "Waiting", 0, 0},
                   {"msacm32.dll", "", "Waiting", 0, 0},
                   {"photo.png", "", "Can't convert", 3, 0}};
        m.queue_note = "3 of 6 done";
        m.status_text = "Converting 6 files";
    } else if (state == "preview") {   // the Source card with the output folder's files and the main source (sample text)
        m.view = View::Done;
        m.file_name = "lz32.dll";
        m.done_file_name = "lz32.dll";
        m.output_path = "C:\\CodeBreaker\\Converted\\lz32";
        m.done_note = "12 files written.";
        m.status_text = "Done";
        m.preview_files = {{"lz32.c", false}, {"lz32.asm", false}, {"lz32.inc", false}, {"lz32.def", false}, {"lz32.rc", false}, {"build.cmd", true}};
        m.preview_lines = {"/* lz32.dll -- decompiled by CodeBreaker */",
                           "#include <windows.h>",
                           "",
                           "INT WINAPI LZStart(void)",
                           "{",
                           "    return 1;",
                           "}",
                           "",
                           "HFILE WINAPI LZOpenFileA(LPSTR name, LPOFSTRUCT of, WORD style)",
                           "{",
                           "    HFILE h = OpenFile(name, of, style);",
                           "    if (h == HFILE_ERROR) return LZERROR_BADINHANDLE;",
                           "    if (style & (OF_WRITE | OF_READWRITE)) return h;",
                           "    return LZInit(h);",
                           "}",
                           "",
                           "LONG WINAPI LZCopy(HFILE from, HFILE to)",
                           "{",
                           "    BYTE buffer[0x1000];",
                           "    LONG total = 0;",
                           "    INT n;",
                           "    while ((n = LZRead(from, (LPSTR)buffer, sizeof buffer)) > 0) {",
                           "        if (_lwrite(to, (LPCSTR)buffer, n) != (UINT)n) return LZERROR_WRITE;",
                           "        total += n;",
                           "    }",
                           "    return n < 0 ? n : total;",
                           "}"};
        m.preview_spans = preview::highlight(preview::Lang::C, m.preview_lines);
    } else if (state == "dragout") {   // the Source card (no preview) while the folder is dragged out of its output box
        m.view = View::Done;
        m.file_name = "lz32.dll";
        m.done_file_name = "lz32.dll";
        m.output_path = "C:\\CodeBreaker\\Converted\\lz32";
        m.done_note = "12 files written.";
        m.status_text = "Done";
        m.hot = Control::OutputBox;
        m.drag_out = true;
        m.drag_ghost = true;
    } else if (state == "functions" || state == "check" || state == "checknone" || state == "compare") {   // the Source card's tabs (sample functions, no one's own)
        m.view = View::Done;
        m.file_name = "lz32.dll";
        m.done_file_name = "lz32.dll";
        m.output_path = "C:\\CodeBreaker\\Converted\\lz32";
        m.done_note = "12 files written.";
        m.status_text = "Done";
        m.preview_files = {{"lz32.c", false}, {"lz32.asm", false}, {"lz32.inc", false}, {"lz32.def", false}, {"lz32.rc", false}, {"build.cmd", true}};
        const char* names[] = {"LZStart", "LZInit", "LZOpenFileA", "LZSeek", "LZRead", "LZCopy", "LZClose", "CopyLZFile", "GetExpandedNameA", "lz_decode_block", "lz_fill_buffer"};
        const char* addresses[] = {"0x1000", "0x1024", "0x10B8", "0x1192", "0x11F0", "0x1388", "0x1452", "0x1480", "0x14C4", "0x1590", "0x1702"};
        for (int i = 0; i < 42; ++i) {   // 38 behave like the original, 1 differs (LZRead), 3 were not checked (GetExpandedNameA and two more)
            convert::FunctionRecord f;
            f.name = i < 11 ? names[i] : "lz_helper_" + std::to_string(i - 10);
            if (i < 11) {
                f.address = addresses[i];
            } else {
                char hex[16];
                std::snprintf(hex, sizeof hex, "0x%X", 0x1800 + (i - 11) * 0x40);
                f.address = hex;
            }
            f.verdict = i == 4 ? convert::FunctionVerdict::Differs : (i == 8 || i == 40 || i == 41) ? convert::FunctionVerdict::NotChecked : convert::FunctionVerdict::Matches;
            f.source_file = "lz32.c";
            m.functions.push_back(std::move(f));
        }
        if (state == "checknone")
            for (convert::FunctionRecord& f : m.functions) f.verdict = convert::FunctionVerdict::NotChecked;
        m.source_tab = state == "check" || state == "checknone" ? SourceTab::Check : SourceTab::Functions;
        if (state == "functions") {
            m.hot = Control::FnRow;
            m.hot_index = 4;
        }
        if (state == "compare") {
            m.cmp_index = 4;
            m.cmp_asm = {"LZRead:", "    push    ebp", "    mov     ebp, esp", "    push    ebx", "    mov     eax, [ebp+8]", "    call    lz_handle_of",
                         "    test    eax, eax", "    jz      .bad", "    mov     ebx, eax", "    mov     ecx, [ebp+16]", "    cmp     ecx, [ebx+LZ.left]", "    jbe     .fits",
                         "    mov     ecx, [ebx+LZ.left]", ".fits:"};
            m.cmp_c = {"INT WINAPI LZRead(HFILE h, LPSTR buf, INT count)", "{", "    LZSTATE *lz = lz_handle_of(h);", "    if (lz == NULL)",
                       "        return LZERROR_BADINHANDLE;", "    if (count > lz->left)", "        count = lz->left;", "    return lz_copy_out(lz, buf, count);", "}"};
            m.cmp_asm_spans = preview::highlight(preview::Lang::Asm, m.cmp_asm);
            m.cmp_c_spans = preview::highlight(preview::Lang::C, m.cmp_c);
            m.cmp_links = {{4, 7, 2, 4}, {9, 12, 5, 6}};   // (the sample's lines that correspond)
            m.cmp_side = 1;
            m.cmp_line = 3;
        }
    } else if (state == "palette") {   // the command palette over the Program card, "s" typed (the real tool list; one sample recent conversion)
        m.palette_open = true;
        m.palette_query = "s";
        m.palette_items = palette::make_items(tools::all(), {{"lz32.dll", "converted 2 minutes ago", false}});
        m.palette_result = palette::search(m.palette_items, m.palette_query);
    }
    return m;
}

bool ww_save_png(IWICImagingFactory* wic, IWICBitmap* bitmap, UINT width, UINT height, const std::wstring& file) {
    ComPtr<IWICStream> stream;
    ComPtr<IWICBitmapEncoder> encoder;
    ComPtr<IWICBitmapFrameEncode> frame;
    ComPtr<IPropertyBag2> bag;
    ComPtr<IWICFormatConverter> converter;
    if (FAILED(wic->CreateStream(stream.put())) || FAILED(stream->InitializeFromFilename(file.c_str(), GENERIC_WRITE))) return false;
    if (FAILED(wic->CreateEncoder(GUID_ContainerFormatPng, nullptr, encoder.put())) || FAILED(encoder->Initialize(stream.get(), WICBitmapEncoderNoCache))) return false;
    if (FAILED(encoder->CreateNewFrame(frame.put(), bag.put())) || FAILED(frame->Initialize(bag.get()))) return false;
    if (FAILED(frame->SetSize(width, height))) return false;
    WICPixelFormatGUID format = GUID_WICPixelFormat32bppBGRA;
    if (FAILED(frame->SetPixelFormat(&format))) return false;
    if (FAILED(wic->CreateFormatConverter(converter.put())) ||
        FAILED(converter->Initialize(bitmap, GUID_WICPixelFormat32bppBGRA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom)))
        return false;
    return SUCCEEDED(frame->WriteSource(converter.get(), nullptr)) && SUCCEEDED(frame->Commit()) && SUCCEEDED(encoder->Commit());
}

int ww_screenshot(const WwShot& shot) {
    if (FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED))) return 2;
    Painter painter;
    if (!painter.ok()) {
        err_text("codebreaker: Direct2D is not available\n");
        return 2;
    }
    ComPtr<IWICImagingFactory> wic;
    if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(wic.put())))) return 2;
    const bool dark = shot.theme.empty() ? ww_system_dark() : shot.theme == "dark";
    const UINT px_w = static_cast<UINT>(shot.width * shot.scale), px_h = static_cast<UINT>(shot.height * shot.scale);
    ComPtr<IWICBitmap> bitmap;
    if (FAILED(wic->CreateBitmap(px_w, px_h, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnLoad, bitmap.put()))) return 2;
    ComPtr<ID2D1RenderTarget> target;
    D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_SOFTWARE, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));
    if (FAILED(painter.d2d()->CreateWicBitmapRenderTarget(bitmap.get(), props, target.put()))) return 2;
    target->SetDpi(96.0f * shot.scale, 96.0f * shot.scale);
    const Model model = ww_model_for(shot.state, dark, shot.board_root, shot.board_focus, shot.board_width);
    const Layout layout = compute_layout(model.view, shot.width, shot.height, kRailWidth, extras_for(model, model.view));
    Appearance look;
    look.dark = dark;
    look.accent = ww_accent(dark);
    look.rounded_corners = true;
    target->BeginDraw();
    painter.paint(target.get(), model, layout, look);
    if (FAILED(target->EndDraw())) return 2;
    if (!ww_save_png(wic.get(), bitmap.get(), px_w, px_h, widen(shot.path))) {
        err_text(std::string("codebreaker: cannot write ") + shot.path + "\n");
        return 1;
    }
    out("{}\n", shot.path);
    return 0;
}

// `app --board-scan IMAGE [--board-out FOLDER]`: the board with no window (what the window's worker does): extracted or reloaded, with the real counts printed.
int ww_board_scan(const stdfs::path& image, const std::string& out_folder) {
    try {
        const stdfs::path root = out_folder.empty() ? board::board_folder(ww_boards_root(), image) : fs::from_text(out_folder);
        const auto start = std::chrono::steady_clock::now();
        std::optional<board::Board> b = board::load(root);
        const bool reloaded = b.has_value();
        if (!b) {
            const std::atomic<bool> never{false};
            std::string last;
            b = board::scan(image, root, [&](const board::ScanProgress& p) {
                if (p.stage != last) out("{}\n", p.stage);
                last = p.stage;
            }, never);
            board::save(*b);
        }
        const double seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
        out("{} in {:.2f} s: {}\n", reloaded ? "reloaded" : "extracted", seconds, fs::to_text(root));
        out("files {}  cabinets {}  could not be read {}\n", b->cards.size(), b->cabinets, b->failures.size());
        const board::Stacks stacks = board::build_stacks(*b);
        std::uint64_t bytes = 0;
        for (const board::Card& c : b->cards) bytes += c.size;
        for (std::size_t i = 0; i < board::kPileCount; ++i)
            out("  {:<10} {:>6}  ({} done)\n", board::pile_name(static_cast<board::Pile>(i)), stacks[i].total(), stacks[i].done.size());
        out("total size {}\n", board::size_words(bytes));
        for (const board::Failure& f : b->failures) out("failed: {}: {}\n", f.path, f.why);
        const auto index_start = std::chrono::steady_clock::now();
        std::optional<board::Index> index = board::load_index(*b);
        const bool index_kept = index.has_value();
        if (!index) {
            const std::atomic<bool> go{false};   // (the board command builds what the window would)
            index = board::build_index(*b, {}, go);
            if (index) board::save_index(*b, *index);
        }
        if (index) {
            std::uint64_t by_link[3] = {}, unresolved = 0, groups = 0, duplicated = 0, no_inbound = 0;
            for (const board::Edge& e : index->edges) ++by_link[static_cast<int>(e.link)];
            for (std::uint32_t i = 0; i < index->nodes.size(); ++i) {
                const board::Node& n = index->nodes[i];
                unresolved += n.unresolved;
                if (n.group == i) {
                    ++groups;
                    duplicated += n.copies > 1 ? 1 : 0;
                }
                no_inbound += index->into(i).empty() ? 1 : 0;
            }
            out("index {} in {:.2f} s\n", index_kept ? "reloaded" : "built", std::chrono::duration<double>(std::chrono::steady_clock::now() - index_start).count());
            out("distinct files {}  with copies {}\n", groups, duplicated);
            out("links {}  imports {}  installs {}  references {}\n", index->edges.size(), by_link[0], by_link[1], by_link[2]);
            out("names not on the board {}  unreadable {}  damaged tables {}  cards with no inbound link {}\n", unresolved, index->unreadable, index->damaged, no_inbound);
            std::uint64_t symbols = 0, matched = 0, calls = 0;
            for (const board::Edge& e : index->edges) {
                symbols += e.symbols;
                matched += e.matched;
                calls += e.calls;
            }
            out("imported functions {}  matched to a real export {}  static call sites {}\n", symbols, matched, calls);
            {
                const std::optional<board::Signals> kept_signals = board::load_signals(*b);
                const std::optional<board::Measured> kept_trace = board::load_measured(b->root);
                const board::Projection projection = board::build_projection(*b, *index, kept_signals ? &*kept_signals : nullptr, kept_trace ? &*kept_trace : nullptr);
                const board::Graph graph = board::build_graph(*b, *index, board::kLinkImports);
                std::string path;
                for (const std::uint32_t m : projection.hot_path) path += (path.empty() ? "" : " > ") + graph.modules[m].name;
                const board::Orphans orphans = board::find_orphans(*b, *index, kept_signals ? &*kept_signals : nullptr, kept_trace ? &*kept_trace : nullptr);
                out("orphans (no link found): {} distinct files; {} more had no link but were seen running\n", orphans.distinct, orphans.seen_running);
                for (const board::Deck& deck : board::builtin_decks()) {
                    const std::vector<char> named = board::cards_named(*b, deck.names);
                    std::set<std::uint32_t> files;
                    for (std::uint32_t c = 0; c < named.size(); ++c)
                        if (named[c]) files.insert(index->nodes[c].group);
                    out("deck {}: {} distinct files\n", deck.name, files.size());
                }
                out("{}: {} calls/hour in all, busiest channel {}/h, hot path {}\n", projection.has_measured ? "measured where traced, else a scaled model" : "projected (a model, not measured)", board::rate_words(projection.total), board::rate_words(projection.max_channel), path);
                std::vector<std::pair<double, std::uint64_t>> top;
                for (const auto& [key, rate] : projection.channels) top.emplace_back(rate, key);
                std::sort(top.begin(), top.end(), [](const auto& x, const auto& y) { return x.first != y.first ? x.first > y.first : x.second < y.second; });
                for (std::size_t i = 0; i < top.size() && i < 8; ++i)
                    out("  {:>7}/h  {:>3.0f}% density  {} > {}\n", board::rate_words(top[i].first), 100 * board::flow_intensity(top[i].first, projection.max_channel), graph.modules[top[i].second >> 32].name, graph.modules[static_cast<std::uint32_t>(top[i].second)].name);
            }
            const auto signals_start = std::chrono::steady_clock::now();
            std::optional<board::Signals> signals = board::load_signals(*b);
            const bool signals_kept = signals.has_value();
            if (!signals) {
                const std::atomic<bool> go{false};
                signals = board::build_signals(*b, *index, {}, go);
                if (signals) board::save_signals(*b, *signals);
            }
            if (signals) {
                out("signals {} in {:.2f} s: {} distinct, {} file-to-signal links\n", signals_kept ? "reloaded" : "built", std::chrono::duration<double>(std::chrono::steady_clock::now() - signals_start).count(),
                    signals->signals.size(), signals->links.size());
                for (std::size_t type = 0; type < board::kSignalTypes; ++type) {
                    std::uint64_t distinct = 0, links = 0, shared = 0, best = 0;
                    std::string busiest;
                    for (std::uint32_t id = 0; id < signals->signals.size(); ++id) {
                        if (static_cast<std::size_t>(signals->signals[id].type) != type) continue;
                        ++distinct;
                        std::set<std::uint32_t> files;
                        for (const std::uint32_t at : signals->of_signal(id)) files.insert(index->nodes[signals->links[at].node].group);
                        links += signals->of_signal(id).size();
                        shared += files.size() > 1 ? 1 : 0;
                        if (files.size() > best) {
                            best = files.size();
                            busiest = signals->signals[id].value;
                        }
                    }
                    out("  {:<15} {:>6} distinct  {:>6} links  {:>5} shared by 2+ files  busiest: {} ({} files)\n", board::signal_type_name(static_cast<board::SignalType>(type)), distinct, links, shared, busiest.substr(0, 60), best);
                }
            }
        }
        return 0;
    } catch (const std::exception& e) {
        err_text(std::string("codebreaker app --board-scan: ") + e.what() + "\n");
        return 1;
    }
}

// `app --board-convert-one FILE --board-out BOARD`: one batch worker. Converts FILE into BOARD/converted and says how it ended on its last line; only the batch writes the board's state.
int ww_board_convert_one(const std::string& file, const std::string& board_folder) {
    convert::Request request;
    request.input = fs::from_text(file);
    request.converted_root = fs::from_text(board_folder) / "converted";
    const std::atomic<bool> go{false};
    convert::Outcome outcome;
    try {
        fs::make_dirs(request.converted_root);
        outcome = convert::convert(request, convert::standard_planner("original"), [](const convert::Progress&) {}, go);
    } catch (const std::exception& e) {
        outcome.state = convert::State::Failed;
        outcome.message = e.what();
    }
    std::string message = outcome.message;
    std::replace_if(message.begin(), message.end(), [](char c) { return c == '\t' || c == '\r' || c == '\n'; }, ' ');
    const char* const state = outcome.state == convert::State::Done ? "done" : outcome.state == convert::State::Unsupported ? "refused" : "failed";
    out("\nBOARDRESULT\t{}\t{}\t{}\t{}\n", state, outcome.files, fs::to_text(outcome.output), message);
    return 0;
}

// `app --board-batch N --board-out BOARD [--board-jobs J]`: convert the next N binaries, what really ran first, J at a time, each in its own process, keeping each real result as it ends.
// Two files with one name never run together: their output folders and scratch folders are chosen by name, and a second `s3` is given the folder `s3_2`, which a file
// called s3_2 would ask for too, so a trailing _N does not tell names apart here.
int ww_board_batch(std::size_t count, const std::string& board_folder, std::size_t jobs) {
    try {
        std::optional<board::Board> b = board::load(fs::from_text(board_folder));
        if (!b) {
            err_text("codebreaker app --board-batch: no board there\n");
            return 1;
        }
        const std::optional<board::Index> index = board::load_index(*b);
        if (!index) {
            err_text("codebreaker app --board-batch: the board has no index\n");
            return 1;
        }
        const std::optional<board::Signals> signals = board::load_signals(*b);
        const std::optional<board::Measured> measured = board::load_measured(b->root);
        const board::Projection projection = board::build_projection(*b, *index, signals ? &*signals : nullptr, measured ? &*measured : nullptr);
        board::StackOptions options;
        std::vector<std::uint32_t> group(b->cards.size());
        for (std::size_t i = 0; i < group.size(); ++i) group[i] = index->nodes[i].group;
        options.group = &group;
        const std::vector<std::uint32_t> order = board::batch_order(*b, *index, board::build_stacks(*b, {}, options), &projection);
        fs::make_dirs(b->root / "converted");
        const std::size_t total = std::min(count, order.size());
        const auto name_of = [&](std::size_t i) {
            std::string name = fs::to_text(b->file_of(b->cards[order[i]]).stem());
            std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
            if (const std::size_t bar = name.rfind('_'); bar != std::string::npos && bar > 0 && bar + 1 < name.size() &&
                                                         std::all_of(name.begin() + static_cast<std::ptrdiff_t>(bar) + 1, name.end(), [](unsigned char c) { return std::isdigit(c) != 0; }))
                name.resize(bar);
            return name;
        };
        std::mutex lock;
        std::condition_variable freed;
        std::vector<char> started(total, 0);
        std::set<std::string> running;
        std::size_t finished = 0, done = 0, refused = 0, failed = 0;
        const stdfs::path self = fs::executable_path();
        const auto worker = [&] {
            while (true) {
                std::size_t at = total;
                std::string name;
                stdfs::path file;
                {
                    std::unique_lock held(lock);
                    while (true) {
                        bool waiting = false;
                        for (std::size_t i = 0; i < total && at == total; ++i) {
                            if (started[i]) continue;
                            waiting = true;
                            if (std::string n = name_of(i); !running.contains(n)) {
                                at = i;
                                name = std::move(n);
                            }
                        }
                        if (at != total || !waiting) break;
                        freed.wait(held);
                    }
                    if (at == total) return;
                    started[at] = 1;
                    running.insert(name);
                    file = b->file_of(b->cards[order[at]]);
                }
                const auto began = std::chrono::steady_clock::now();
                std::string state = "failed", files = "0", output, note;
                try {
                    process::ProcessSpec spec;
                    spec.argv = {fs::to_text(self), "app", "--board-convert-one", fs::to_text(file), "--board-out", board_folder};
                    spec.err = process::Capture::Discard;
                    spec.priority = process::Priority::BelowNormal;
                    const process::ProcessResult result = process::run(spec);
                    const std::string text(reinterpret_cast<const char*>(result.out.data()), result.out.size());
                    const std::size_t line = text.rfind("BOARDRESULT\t");
                    if (result.ok() && line != std::string::npos) {
                        std::vector<std::string> parts;
                        std::size_t from = line;
                        const std::size_t stop = std::min(text.find('\n', line), text.size());
                        while (from <= stop) {
                            const std::size_t tab = std::min(text.find('\t', from), stop);
                            parts.push_back(text.substr(from, tab - from));
                            from = tab + 1;
                        }
                        if (parts.size() >= 5) {
                            state = parts[1];
                            files = parts[2];
                            output = parts[3];
                            note = parts[4];
                            if (!note.empty() && note.back() == '\r') note.pop_back();
                        }
                    } else {
                        note = std::format("the converter stopped with exit code {:#x}", result.exit_code);
                    }
                } catch (const std::exception& e) {
                    note = e.what();
                }
                std::lock_guard held(lock);
                board::Card& card = b->cards[order[at]];
                const board::Conv conv = state == "done" ? board::Conv::Done : state == "refused" ? board::Conv::CannotConvert : board::Conv::Failed;
                board::record(card, conv, conv == board::Conv::Done ? fs::from_text(output) : stdfs::path{}, note);
                board::save_states(*b);
                done += conv == board::Conv::Done ? 1 : 0;
                refused += conv == board::Conv::CannotConvert ? 1 : 0;
                failed += conv == board::Conv::Failed ? 1 : 0;
                ++finished;
                out("[{}/{}] {:<18} {:<14} {:>6.1f} s  {} files  {}/h\n", finished, total, card.name, board::conv_words(card.conv), std::chrono::duration<double>(std::chrono::steady_clock::now() - began).count(), files,
                    board::rate_words(projection.activity[projection.module_of_card[order[at]]]));
                if (card.conv != board::Conv::Done && !card.note.empty()) out("        {}\n", card.note.substr(0, 150));
                running.erase(name);
                freed.notify_all();
            }
        };
        std::vector<std::thread> workers;
        for (std::size_t i = 0; i < std::clamp<std::size_t>(jobs, 1, 16); ++i) workers.emplace_back(worker);
        for (std::thread& t : workers) t.join();
        out("converted {}  cannot convert {}  failed {}  of {} (with {} more waiting)\n", done, refused, failed, total, order.size() - total);
        return 0;
    } catch (const std::exception& e) {
        err_text(std::string("codebreaker app --board-batch: ") + e.what() + "\n");
        return 1;
    }
}

// `app --board-trace TRACE --board-trace-disk IMAGE|FOLDER --board-out BOARD`: a measured run of the system, named with the files installed on that machine and kept with the board.
int ww_board_trace(const std::string& trace, const std::string& disk, const std::string& board_folder) {
    try {
        if (board_folder.empty() || disk.empty()) {
            err_text("codebreaker app --board-trace: needs --board-trace-disk and --board-out\n");
            return 2;
        }
        const stdfs::path root = fs::from_text(board_folder), source = fs::from_text(disk);
        stdfs::path installed = source;
        if (std::error_code ec; !stdfs::is_directory(source, ec)) {
            installed = root / "trace_installed";
            if (!stdfs::is_directory(installed, ec)) {
                const std::vector<std::string> kinds{"*.dll", "*.exe", "*.drv", "*.cpl", "*.ocx", "*.scr", "*.acm", "*.ax", "*.tlb", "*.ime"};
                formats::media::extract(source, installed, kinds);
            }
        }
        const std::vector<board::InstalledImage> images = board::read_installed(installed);
        const std::optional<board::Measured> m = board::read_trace(fs::read_text(fs::from_text(trace)), images);
        if (!m) {
            err_text("codebreaker app --board-trace: that is not a calltrace file with a clock\n");
            return 1;
        }
        board::save_measured(root, *m);
        out("{} installed PE images; {:.1f} s of guest time; {} indirect transfers\n", images.size(), m->seconds, board::count_words(m->transfers));
        out("  ring 0 (VxD) {}  not an export {}  source not an installed image {}  ambiguous {}  inside one module {}\n", board::count_words(m->ring0), board::count_words(m->not_export),
            board::count_words(m->no_source), board::count_words(m->ambiguous), board::count_words(m->inside));
        out("named import calls {} over {} channels, {} function paths\n", board::count_words(m->named), m->channels.size(), m->functions.size());
        for (std::size_t i = 0; i < m->channels.size() && i < 8; ++i)
            out("  {:>8}/h  {:>9}  {} > {}\n", board::rate_words(m->per_hour(m->channels[i].count)), m->channels[i].count, m->channels[i].from, m->channels[i].to);
        if (const std::optional<board::Board> b = board::load(root)) {
            if (const std::optional<board::Index> index = board::load_index(*b)) {
                const std::optional<board::Signals> signals = board::load_signals(*b);
                const board::Projection p = board::build_projection(*b, *index, signals ? &*signals : nullptr, &*m);
                const board::Graph graph = board::build_graph(*b, *index, board::kLinkImports);
                std::string path;
                for (const std::uint32_t module : p.hot_path) path += (path.empty() ? "" : " > ") + graph.modules[module].name;
                out("measured hot path: {}\n", path);
                static constexpr const char* kKinds[] = {"message pump", "sync", "memory and strings", "paint", "window state", "file and registry", "lifecycle", "other"};
                for (std::size_t k = 0; k < p.kind_scale.size(); ++k) out("  model scale, {}: {:.4f}\n", kKinds[k], p.kind_scale[k]);
            }
        }
        return 0;
    } catch (const std::exception& e) {
        err_text(std::string("codebreaker app --board-trace: ") + e.what() + "\n");
        return 1;
    }
}

// ---- the entry --------------------------------------------------------------------------------------------------------------------------

// A copy of this program that has no console, running the window.
bool ww_start_detached_copy(const std::string& file, const std::string& board) {
    wchar_t exe[MAX_PATH * 2] = {};
    GetModuleFileNameW(nullptr, exe, static_cast<DWORD>(std::size(exe)));
    std::wstring command = L"\"" + std::wstring(exe) + L"\" app --in-process" + (board.empty() ? std::wstring() : L" --board \"" + widen(board) + L"\"") + (file.empty() ? std::wstring() : L" \"" + widen(file) + L"\"");
    STARTUPINFOW si{};
    si.cb = sizeof si;
    PROCESS_INFORMATION pi{};
    if (!CreateProcessW(exe, command.data(), nullptr, nullptr, FALSE, DETACHED_PROCESS | CREATE_NEW_PROCESS_GROUP, nullptr, nullptr, &si, &pi)) return false;
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return true;
}

}  // namespace

bool available() { return true; }

int run(std::span<const std::string> args, bool no_command_given) {
    (void)no_command_given;
    WwShot shot;
    bool screenshot = false, in_process = false;
    std::string open_file, open_board_image, board_scan, board_out, board_trace, board_trace_disk, board_batch, board_jobs, board_one;
    for (std::size_t i = 0; i < args.size(); ++i) {
        const std::string& a = args[i];
        const auto value = [&]() -> std::string { return i + 1 < args.size() ? args[++i] : std::string(); };
        if (a == "--screenshot") {
            screenshot = true;
            shot.path = value();
        } else if (a == "--state") {
            shot.state = value();
        } else if (a == "--theme") {
            shot.theme = value();
        } else if (a == "--size") {
            const std::string v = value();
            const std::size_t x = v.find('x');
            if (x == std::string::npos) {
                err_text("codebreaker app: --size takes WIDTHxHEIGHT\n");
                return 2;
            }
            shot.width = static_cast<float>(std::atoi(v.substr(0, x).c_str()));
            shot.height = static_cast<float>(std::atoi(v.substr(x + 1).c_str()));
        } else if (a == "--scale") {
            shot.scale = static_cast<float>(std::atof(value().c_str()));
        } else if (a == "--board") {   // the window opens on the Board page with this image; with --screenshot, the kept board in this folder
            open_board_image = value();
            shot.board_root = open_board_image;
        } else if (a == "--board-focus") {
            shot.board_focus = value();
        } else if (a == "--board-width") {
            shot.board_width = value();
        } else if (a == "--board-scan") {
            board_scan = value();
        } else if (a == "--board-out") {
            board_out = value();
        } else if (a == "--board-batch") {
            board_batch = value();
        } else if (a == "--board-jobs") {
            board_jobs = value();
        } else if (a == "--board-convert-one") {
            board_one = value();
        } else if (a == "--board-trace") {
            board_trace = value();
        } else if (a == "--board-trace-disk") {
            board_trace_disk = value();
        } else if (a == "--in-process") {
            in_process = true;
        } else if (!a.starts_with("--") && open_file.empty()) {
            open_file = a;
        } else {
            err_text("usage: codebreaker app [FILE] [--screenshot OUT.png [--state welcome|idle|working|done|unsupported|failed|language|translating|tools|settings|advanced|recent|batch|preview|dragout|functions|check|compare|palette|board|board-expanded|board-flow|board-trace|board-library|board-library-hover|board-library-function|board-filter|board-inspect|board-hover|board-orphans|board-c-identical|board-c-near|board-c-nocompile|board-c-unscored|board-deck|board-empty|board-scanning] [--board FOLDER] [--theme light|dark] [--size WxH] [--scale N]] [--board IMAGE] [--board-scan IMAGE [--board-out FOLDER]] [--board-trace TRACE --board-trace-disk IMAGE --board-out FOLDER]\n");
            return 2;
        }
    }
    if (!board_one.empty()) return ww_board_convert_one(board_one, board_out);
    if (!board_batch.empty()) return ww_board_batch(static_cast<std::size_t>(std::atoi(board_batch.c_str())), board_out, board_jobs.empty() ? 4 : static_cast<std::size_t>(std::max(1, std::atoi(board_jobs.c_str()))));
    if (!board_trace.empty()) return ww_board_trace(board_trace, board_trace_disk, board_out);
    if (!board_scan.empty()) return ww_board_scan(fs::from_text(board_scan), board_out);
    if (screenshot) {
        if (shot.path.empty() || shot.width < 200 || shot.height < 200 || shot.scale < 0.5f || shot.scale > 4.0f) {
            err_text("codebreaker app: --screenshot needs a file name; --size at least 200x200; --scale between 0.5 and 4\n");
            return 2;
        }
        return ww_screenshot(shot);
    }
    if (!in_process) {
        // Started from Explorer or a shortcut (the only process on its console): the console goes away and the window runs here. Started from a
        // terminal: the window runs in a detached copy and the prompt comes back at once.
        DWORD ids[4] = {};
        const DWORD attached = GetConsoleProcessList(ids, static_cast<DWORD>(std::size(ids)));
        if (attached >= 2) {
            if (ww_start_detached_copy(open_file, open_board_image)) return 0;
        } else if (attached == 1) {
            ShowWindow(GetConsoleWindow(), SW_HIDE);
            FreeConsole();
        }
    }
    WwApp app;
    if (!open_file.empty()) app.open_at_start(fs::from_text(open_file));
    if (!open_board_image.empty()) app.open_board_at_start(fs::from_text(open_board_image));
    return app.run();
}

}  // namespace cb::app::window
#endif  // _WIN32
