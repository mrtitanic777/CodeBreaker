// cb/app/window_win.cpp -- the CodeBreaker window (Windows): the Win32 shell around the drawing of window_render_win.cpp and the pipeline of convert.hpp.
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
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <thread>

#include "cb/app/convert.hpp"
#include "cb/app/settings_edit.hpp"
#include "cb/app/tools.hpp"
#include "cb/app/tools_run.hpp"
#include "cb/app/window_pages.hpp"
#include "cb/app/window.hpp"
#include "cb/app/window_layout.hpp"
#include "cb/app/window_render_win.hpp"
#include "cb/base/config.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"

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

int ww_field_kind(const tools::Arg& a) {
    switch (a.kind) {
        case tools::ArgKind::File: return 0;
        case tools::ArgKind::Folder: return 1;
        case tools::ArgKind::Choice: return 3;
        case tools::ArgKind::Toggle: return 4;
        default: return 2;
    }
}

std::string ww_field_hint(const tools::Arg& a) {
    if (a.list) return "Separate entries with ;";
    switch (a.kind) {
        case tools::ArgKind::File: return a.required ? "Browse, or type a path or name" : "Optional";
        case tools::ArgKind::Folder: return a.required ? "Browse, or type a folder" : "Optional";
        case tools::ArgKind::Words: return "Arguments, separated by spaces";
        case tools::ArgKind::Text: return a.required ? std::string() : "Optional";
        default: return {};
    }
}

int ww_tool_index(const tools::Tool* t) { return static_cast<int>(t - tools::all().data()); }

// The list as shown: every group that has a tool matching the search, its heading, then the tools (their indexes in tools::all()).
void ww_fill_tool_rows(Model& m) {
    m.tool_rows.clear();
    const std::vector<const tools::Tool*> found = tools::search(m.tool_query);
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
    for (std::size_t i = 0; i < t.args.size(); ++i) {
        const tools::Arg& a = t.args[i];
        FieldItem f;
        f.label = a.label;
        f.kind = ww_field_kind(a);
        f.text = i < values.text.size() ? values.text[i] : std::string();
        f.choices = a.choices;
        f.required = a.required;
        f.hint = ww_field_hint(a);
        m.tool_fields.push_back(std::move(f));
    }
    m.tool_can_run = !tools::missing(t, values).has_value();
}

// The Settings form: the theme first, then every setting of settings.json as a path (Browse), a switch or text.
void ww_fill_settings(Model& m, const std::vector<settings::Row>& rows, settings::Theme theme) {
    m.setting_fields.clear();
    m.setting_keys.clear();
    m.setting_fields.push_back({"Theme", 3, settings::theme_name(theme), {"System", "Light", "Dark"}, false, ""});
    for (const settings::Row& r : rows) {
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
        m.setting_fields.push_back(std::move(f));
        m.setting_keys.push_back(r.key);
    }
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
    bool can_take_files() const { return model_.view != View::Working; }
    void start_conversion(const stdfs::path& file);

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
    void blur();
    void insert_text(const std::string& utf8);
    bool edit_key(WPARAM key, bool ctrl);
    void select_tool(int tool);
    void run_tool();
    void stop_tool();
    void poll_tool();
    void browse_field(int index);
    std::vector<LanguageItem> field_popup_items() const;
    LanguageList field_popup_list() const;
    void open_field_popup(int index);
    void choose_in_popup(int item);
    void popup_key(WPARAM key);
    void scroll_page(float x, float y, int lines);
    void cancel_conversion();
    void on_progress();
    void stream_frame();
    void begin_card_fade(View from);
    void start_welcome();
    void skip_welcome();
    void fade_frame();
    void on_finished();
    void reset_to_idle(const std::string& status);

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
    std::string last_output_, last_note_;
    bool last_done_ = false;
    int last_percent_ = -1;
    float phase_ = 0;
    wchar_t pending_high_ = 0;                       // the first half of a surrogate pair typed, waiting for its second
    settings::Theme theme_ = settings::Theme::System;
    config::Config settings_config_ = config::Config::load(config::current().checkout_root());

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
    friend class WwDropTarget;
};

WwApp* ww_app(HWND hwnd) { return reinterpret_cast<WwApp*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA)); }

// The first file of a drag, when it holds files.
std::optional<stdfs::path> ww_first_file(IDataObject* data) {
    FORMATETC fmt{CF_HDROP, nullptr, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
    STGMEDIUM medium{};
    if (data == nullptr || FAILED(data->GetData(&fmt, &medium))) return std::nullopt;
    std::optional<stdfs::path> result;
    const HDROP drop = static_cast<HDROP>(GlobalLock(medium.hGlobal));
    if (drop != nullptr) {
        wchar_t path[MAX_PATH * 4] = {};
        if (DragQueryFileW(drop, 0, path, static_cast<UINT>(std::size(path))) > 0) result = stdfs::path(path);
        GlobalUnlock(medium.hGlobal);
    }
    ReleaseStgMedium(&medium);
    return result;
}

HRESULT WwDropTarget::DragEnter(IDataObject* data, DWORD, POINTL, DWORD* effect) {
    acceptable_ = app_->can_take_files() && ww_first_file(data).has_value();
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
    if (const std::optional<stdfs::path> file = ww_first_file(data)) {
        *effect = DROPEFFECT_COPY;
        app_->start_conversion(*file);
    }
    return S_OK;
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
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);   // (the manifest says it too; this covers a copy without it)
    if (!painter_.ok()) {
        err_text("codebreaker: Direct2D is not available\n");
        return 2;
    }
    theme_ = settings::remembered_theme();
    model_.platforms = convert::platform_summary();
    model_.drop_hint = convert::drop_hint();
    set_language_list(convert::targets_for({}));
    if (!create_window()) return 2;
    if (initial_file_.empty()) start_welcome();   // (a window opened on a file goes straight to work)
    ShowWindow(hwnd_, SW_SHOWNORMAL);
    UpdateWindow(hwnd_);
    if (!initial_file_.empty()) start_conversion(initial_file_);
    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    if (running_) {   // closed with a conversion under way: ask it to stop, wait a little, then end the process (the staging folder is its own)
        cancel_ = true;
        for (int i = 0; i < 30 && running_; ++i) Sleep(100);
        if (worker_.joinable()) {
            if (running_) ExitProcess(0);
            worker_.join();
        }
    }
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
    layout_ = compute_layout(model_.view, ww_dip(rc.right - rc.left, dpi_), ww_dip(rc.bottom - rc.top, dpi_), kRailWidth);
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
    } else {
        for (const Control c : focus_order(model_.view)) order.push_back({c, -1});
        for (const Control c : {Control::NavConvert, Control::NavTools, Control::NavSettings}) order.push_back({c, -1});
    }
    if (order.empty()) return;
    std::size_t at = order.size();
    for (std::size_t i = 0; i < order.size(); ++i)
        if (order[i].control == model_.focus && (order[i].index == model_.focus_index || model_.page == Page::Convert)) at = i;
    std::size_t next = 0;
    if (at == order.size()) next = backwards ? order.size() - 1 : 0;
    else next = backwards ? (at + order.size() - 1) % order.size() : (at + 1) % order.size();
    set_focus(order[next].control, order[next].index);
    if (model_.page == Page::Settings && order[next].control != Control::NavConvert && order[next].index >= 0) {   // keep the field in view
        const SettingsLayout sl = compute_settings_layout(layout_, model_);
        const std::size_t f = static_cast<std::size_t>(order[next].index);
        if (f < sl.form.fields.size()) {
            const Rect& r = sl.form.fields[f].input;
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
    if (running_) return;
    ComPtr<IFileOpenDialog> dialog;
    if (FAILED(CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(dialog.put())))) return;
    std::wstring programs;
    for (const std::string& e : convert::program_extensions()) programs += (programs.empty() ? L"*" : L";*") + widen(e);
    const COMDLG_FILTERSPEC types[] = {{L"Programs and images", programs.c_str()}, {L"All files", L"*.*"}};
    dialog->SetFileTypes(static_cast<UINT>(std::size(types)), types);
    dialog->SetTitle(L"Choose a program");
    DWORD options = 0;
    dialog->GetOptions(&options);
    dialog->SetOptions(options | FOS_FORCEFILESYSTEM | FOS_FILEMUSTEXIST | FOS_PATHMUSTEXIST);
    if (FAILED(dialog->Show(hwnd_))) return;
    ComPtr<IShellItem> item;
    if (FAILED(dialog->GetResult(item.put()))) return;
    PWSTR path = nullptr;
    if (SUCCEEDED(item->GetDisplayName(SIGDN_FILESYSPATH, &path)) && path != nullptr) {
        const stdfs::path file(path);
        CoTaskMemFree(path);
        start_conversion(file);
    }
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
        default: return {hit_test(layout_, model_.view, x, y), -1};
    }
}

PageHit WwApp::hit_at(LPARAM lp) const { return hit_at(ww_dip(GET_X_LPARAM(lp), dpi_), ww_dip(GET_Y_LPARAM(lp), dpi_)); }

void WwApp::set_page(Page page) {
    if (model_.page == page) return;
    blur();
    model_.field_popup = -1;
    model_.language_open = false;
    model_.page = page;
    model_.focus = Control::None;
    model_.focus_index = -1;
    if (page == Page::Tools && model_.tool_rows.empty()) ww_fill_tool_rows(model_);
    if (page == Page::Settings) {
        settings_config_ = config::Config::load(config::current().checkout_root());
        ww_fill_settings(model_, settings::rows(settings_config_), theme_);
        model_.setting_scroll = 0;
    }
    relayout();
    invalidate();
}

// ---- text entry

std::string* WwApp::edit_target() {
    if (model_.focus == Control::ToolSearch) return &model_.tool_query;
    if (model_.focus != Control::Field || model_.focus_index < 0) return nullptr;
    std::vector<FieldItem>& fields = model_.page == Page::Settings ? model_.setting_fields : model_.tool_fields;
    if (static_cast<std::size_t>(model_.focus_index) >= fields.size()) return nullptr;
    FieldItem& f = fields[static_cast<std::size_t>(model_.focus_index)];
    return f.kind == 0 || f.kind == 1 || f.kind == 2 ? &f.text : nullptr;
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
    if (model_.page == Page::Tools) {
        if (model_.focus == Control::ToolSearch) {
            ww_fill_tool_rows(model_);
            model_.tool_scroll = 0;
        } else if (model_.tool_selected >= 0) {
            tools::Values& v = tool_values_[model_.tool_selected];
            for (std::size_t i = 0; i < model_.tool_fields.size() && i < v.text.size(); ++i) v.text[i] = model_.tool_fields[i].text;
            model_.tool_can_run = !tools::missing(tools::all()[static_cast<std::size_t>(model_.tool_selected)], v).has_value();
        }
    }
    invalidate();
}

// A setting changed on the Settings page is written to settings.json when the field is left, a switch or a choice when it is changed.
void WwApp::commit_setting(int field) {
    if (model_.page != Page::Settings || field < 0 || static_cast<std::size_t>(field) >= model_.setting_fields.size()) return;
    const FieldItem& f = model_.setting_fields[static_cast<std::size_t>(field)];
    if (field == 0) {   // the theme
        theme_ = f.text == "Light" ? settings::Theme::Light : f.text == "Dark" ? settings::Theme::Dark : settings::Theme::System;
        settings::remember_theme(theme_);
        apply_theme();
        return;
    }
    const std::string& key = model_.setting_keys[static_cast<std::size_t>(field) - 1];
    const std::string error = settings::write(settings_config_, key, f.text, f.kind == 4 ? settings::Kind::Toggle : f.kind == 0 ? settings::Kind::Path : settings::Kind::Text);
    model_.status_text = error.empty() ? "Saved: " + f.label : "Could not save " + f.label + ": " + error;
}

void WwApp::blur() {
    if (model_.page == Page::Settings && model_.focus == Control::Field && model_.focus_index > 0) commit_setting(model_.focus_index);
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
    model_.tool_running = true;
    model_.tool_output.clear();
    model_.tool_note = last_note_;
    model_.tool_done = false;
    model_.tool_percent = -1;
    model_.output_scroll = 0;
    model_.activity = true;
    model_.status_text = "Running " + tool.title;
    SetTimer(hwnd_, kWwRunTimer, 80, nullptr);
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
        model_.tool_phase = phase_ <= 1.0f ? phase_ : 2.0f - phase_;
        model_.output_scroll = 0;
    }
    invalidate();
}

// ---- choosing a file or a folder for a field

void WwApp::browse_field(int index) {
    std::vector<FieldItem>& fields = model_.page == Page::Settings ? model_.setting_fields : model_.tool_fields;
    if (index < 0 || static_cast<std::size_t>(index) >= fields.size()) return;
    FieldItem& f = fields[static_cast<std::size_t>(index)];
    bool folder = f.kind == 1;
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
        // a list field gets the new entry after the ones it has
        std::string& text = f.text;
        const bool list = model_.page == Page::Tools && model_.tool_selected >= 0 && static_cast<std::size_t>(index) < tools::all()[static_cast<std::size_t>(model_.tool_selected)].args.size() &&
                          tools::all()[static_cast<std::size_t>(model_.tool_selected)].args[static_cast<std::size_t>(index)].list;
        text = list && !text.empty() ? text + "; " + chosen : chosen;
        model_.focus = Control::Field;
        model_.focus_index = index;
        model_.caret = static_cast<int>(text.size());
        edited();
        if (model_.page == Page::Settings) commit_setting(index);
    }
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

// Activating a part of the Tools or Settings page (a click released on it, or Enter or Space on the focused one).
void WwApp::activate_page(Control c, int index) {
    switch (c) {
        case Control::NavConvert: set_page(Page::Convert); break;
        case Control::NavTools: set_page(Page::Tools); break;
        case Control::NavSettings: set_page(Page::Settings); break;
        case Control::ToolRow: select_tool(index); break;
        case Control::Run:
            if (run_ != nullptr) stop_tool();
            else run_tool();
            break;
        case Control::FieldBrowse: browse_field(index); break;
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
    }
    invalidate();
}

void WwApp::cancel_conversion() {
    if (!running_) return;
    cancel_ = true;
    model_.status_text = "Cancelling…";
    model_.stage_label = "Cancelling";
    invalidate();
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

void WwApp::start_conversion(const stdfs::path& file) {
    if (running_) return;
    if (worker_.joinable()) worker_.join();
    // the translator says which languages it can write for this file; one the file cannot have gives way to the first it can (not remembered)
    set_language_list(convert::targets_for(file));
    if (const auto at = static_cast<std::size_t>(model_.language_selected); at >= model_.languages.size() || !model_.languages[at].supported) {
        for (std::size_t i = 0; i < model_.languages.size(); ++i) {
            if (model_.languages[i].supported) {
                model_.language_selected = static_cast<int>(i);
                break;
            }
        }
    }
    const LanguageItem& language = model_.languages[static_cast<std::size_t>(model_.language_selected)];
    const std::string target = language.id;
    const View before = model_.view;
    skip_welcome();
    model_.view = View::Working;
    begin_card_fade(before);   // (from the Source card: it fades out with what it showed -- the output path and note stay until the next Done)
    model_.file_name = narrow(file.filename().wstring());
    model_.stage_label = std::string(convert::state_label(convert::State::Identifying));
    model_.detail.clear();
    model_.percent = 0;
    model_.status_text = (target == "original" ? "Converting " + model_.file_name : "Translating " + model_.file_name + " to " + language.name);
    model_.focus = Control::None;
    stream_.clear();
    model_.stream.clear();
    model_.stream_cursor = false;
    {
        std::lock_guard hold(lock_);
        stream_waiting_.clear();
    }
    if (ww_animations_on()) {
        stream_tick_ = std::chrono::steady_clock::now();
        stream_frames_ = 0;
        SetTimer(hwnd_, kWwStreamTimer, kWwStreamFrameMs, nullptr);
    }
    relayout();
    invalidate();
    cancel_ = false;
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
    if (!lines.empty()) {
        stream_.push(lines);
        if (!ww_animations_on()) {   // Windows' "Animation effects" off: the newest lines, still, no typing and no blinking cursor
            stream_.show_all();
            model_.stream = stream_.visible(kWwStreamRows);
            model_.stream_cursor = false;
        }
    }
    invalidate();
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
    const std::string file_name = model_.file_name;
    const View before = model_.view;
    // what the Program card showed as it ended, so it fades out as it was
    const std::string stage = model_.stage_label, detail = model_.detail;
    const std::vector<std::string> stream = model_.stream;
    reset_to_idle("Ready");
    model_.file_name = file_name;
    switch (outcome->state) {
        case convert::State::Done: {
            model_.view = View::Done;
            model_.stage_label = stage;
            model_.detail = detail;
            model_.percent = 100;
            model_.stream = stream;
            model_.done_file_name = file_name;
            model_.output_path = narrow(outcome->output.wstring());
            model_.done_note = std::to_string(outcome->files) + (outcome->files == 1 ? " file written." : " files written.");
            if (!outcome->checked.empty()) model_.done_note += " Checked: " + outcome->checked;
            model_.status_text = "Done";
            break;
        }
        case convert::State::Unsupported:
            model_.view = View::Unsupported;
            model_.info_title = "This file can't be converted";
            model_.info_message = outcome->message;
            model_.status_text = "Cannot convert this file";
            break;
        case convert::State::Cancelled: model_.status_text = "Cancelled"; break;
        default:
            model_.view = View::Failed;
            model_.info_title = "The conversion failed";
            model_.info_message = outcome->message;
            model_.status_text = "Something went wrong";
            break;
    }
    begin_card_fade(before);
    relayout();
    invalidate();
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
        case WM_SYSKEYDOWN:
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
            if (!tracking_) {
                TRACKMOUSEEVENT t{sizeof t, TME_LEAVE, hwnd_, 0};
                tracking_ = TrackMouseEvent(&t) != FALSE;
            }
            return 0;
        }
        case WM_MOUSELEAVE:
            tracking_ = false;
            if (model_.pressed == Control::None) set_hot(Control::None);
            return 0;
        case WM_MOUSEWHEEL: {
            POINT p{GET_X_LPARAM(lp), GET_Y_LPARAM(lp)};
            ScreenToClient(hwnd_, &p);
            scroll_page(ww_dip(p.x, dpi_), ww_dip(p.y, dpi_), GET_WHEEL_DELTA_WPARAM(wp) / WHEEL_DELTA);
            return 0;
        }
        case WM_CHAR: {
            if (wp >= 32 && wp != 127 && model_.field_popup < 0 && edit_target() != nullptr) {
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
            break;
        case WM_LBUTTONDOWN: {
            const float mx = ww_dip(GET_X_LPARAM(lp), dpi_), my = ww_dip(GET_Y_LPARAM(lp), dpi_);
            if (model_.welcome && my >= kTitleBarHeight) {   // a click below the title bar during the welcome: on to the page (the caption buttons still work)
                skip_welcome();
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
            if (model_.page == Page::Convert) {
                model_.focus = Control::None;
            } else {
                // a text box takes the keyboard at once (a switch or a drop-down acts when the click is released)
                const std::vector<FieldItem>& fields = model_.page == Page::Settings ? model_.setting_fields : model_.tool_fields;
                const bool text_field = h.control == Control::ToolSearch ||
                                        (h.control == Control::Field && h.index >= 0 && static_cast<std::size_t>(h.index) < fields.size() && fields[static_cast<std::size_t>(h.index)].kind <= 2);
                if (text_field) set_focus(h.control, h.index);
                else set_focus(Control::None, -1);
            }
            if (h.control != Control::None) {
                model_.pressed = h.control;
                model_.pressed_index = h.index;
                SetCapture(hwnd_);
            }
            set_hot(h.control, h.index);
            invalidate();
            return 0;
        }
        case WM_LBUTTONUP: {
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
                    if (pressed != Control::ToolSearch) activate_page(pressed, pressed_index);
                } else {
                    activate(pressed);
                }
            }
            return 0;
        }
        case WM_CAPTURECHANGED:
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
            if (ctrl && wp >= '1' && wp <= '3') {   // Ctrl+1, 2, 3: Convert, Tools, Settings
                set_page(wp == '1' ? Page::Convert : wp == '2' ? Page::Tools : Page::Settings);
                return 0;
            }
            if (model_.field_popup >= 0) {   // a form's open list takes the keys
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
        case WM_CLOSE:
            if (running_) cancel_ = true;
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
    std::string path, state = "idle", theme;
    float width = kDefaultWidth, height = kDefaultHeight, scale = 1.0f;
};

Model ww_model_for(const std::string& state, bool dark) {
    Model m;
    m.dark = dark;
    m.platforms = convert::platform_summary();
    m.drop_hint = convert::drop_hint();
    m.languages = ww_items(convert::targets_for({}));
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
        const tools::Tool* t = tools::find("pe-source");
        tools::Values v = tools::empty_values(*t);
        v.text[0] = "C:\\Games\\Pinball\\PINBALL.EXE";
        ww_load_tool(m, ww_tool_index(t), v);
        m.tool_done = true;
        m.tool_percent = 100;
        m.tool_note = "Finished";
        m.tool_output =
            "reading PINBALL.EXE (PE32, i386, 312 KB)\nfunctions found: 1,284\nwriting pinball.asm, pinball.inc, build.cmd\nassembling with NASM ... linking ...\n"
            "rebuilt file is identical to the original (312,320 bytes)\nsource: C:\\CodeBreaker\\Converted\\PINBALL\n";
        return m;
    }
    if (state == "settings") {
        m.page = Page::Settings;
        m.status_text = "Saved: " + settings::plain_label("low_load");
        const std::vector<settings::Row> rows = {
            {"nasm", "C:\\Tools\\nasm\\nasm.exe", settings::Kind::Path, true, true, settings::plain_label("nasm"), settings::plain_hint("nasm")},
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
    const Model model = ww_model_for(shot.state, dark);
    const Layout layout = compute_layout(model.view, shot.width, shot.height, kRailWidth);
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

// ---- the entry --------------------------------------------------------------------------------------------------------------------------

// A copy of this program that has no console, running the window.
bool ww_start_detached_copy(const std::string& file) {
    wchar_t exe[MAX_PATH * 2] = {};
    GetModuleFileNameW(nullptr, exe, static_cast<DWORD>(std::size(exe)));
    std::wstring command = L"\"" + std::wstring(exe) + L"\" app --in-process" + (file.empty() ? std::wstring() : L" \"" + widen(file) + L"\"");
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
    std::string open_file;
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
        } else if (a == "--in-process") {
            in_process = true;
        } else if (!a.starts_with("--") && open_file.empty()) {
            open_file = a;
        } else {
            err_text("usage: codebreaker app [FILE] [--screenshot OUT.png [--state welcome|idle|working|done|unsupported|failed|language|translating|tools|settings] [--theme light|dark] [--size WxH] [--scale N]]\n");
            return 2;
        }
    }
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
            if (ww_start_detached_copy(open_file)) return 0;
        } else if (attached == 1) {
            ShowWindow(GetConsoleWindow(), SW_HIDE);
            FreeConsole();
        }
    }
    WwApp app;
    if (!open_file.empty()) app.open_at_start(fs::from_text(open_file));
    return app.run();
}

}  // namespace cb::app::window
#endif  // _WIN32
