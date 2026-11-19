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
#include <cstdlib>
#include <cstring>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <thread>

#include "cb/app/convert.hpp"
#include "cb/app/window.hpp"
#include "cb/app/window_layout.hpp"
#include "cb/app/window_render_win.hpp"
#include "cb/base/config.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"

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
constexpr UINT kWmFinished = WM_APP + 2;   // the worker is done: its outcome is ready
constexpr wchar_t kWwClass[] = L"CodeBreakerWindow";

// ---- what the system says ------------------------------------------------------------------------------------------------------------

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
    Control control_at_screen(POINT screen) const;
    void set_hot(Control c);
    void activate(Control c);
    void move_focus(bool backwards);
    void browse();
    void open_output();
    void show_system_menu(POINT screen);
    void copy_output_path();
    void cancel_conversion();
    void on_progress();
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

    // the conversion
    std::thread worker_;
    std::atomic<bool> cancel_{false};
    std::mutex lock_;
    std::optional<convert::Progress> latest_;
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
    model_.platforms = convert::platform_summary();
    if (!create_window()) return 2;
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
    model_.dark = ww_system_dark();
    look_.dark = model_.dark;
    look_.accent = ww_accent(model_.dark);
    look_.backdrop = backdrop_;
    const BOOL dark = model_.dark ? TRUE : FALSE;
    DwmSetWindowAttribute(hwnd_, kWwUseImmersiveDarkMode, &dark, sizeof dark);
}

void WwApp::relayout() {
    RECT rc{};
    GetClientRect(hwnd_, &rc);
    layout_ = compute_layout(model_.view, ww_dip(rc.right - rc.left, dpi_), ww_dip(rc.bottom - rc.top, dpi_));
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

void WwApp::set_hot(Control c) {
    if (model_.hot == c) return;
    model_.hot = c;
    invalidate();
}

void WwApp::move_focus(bool backwards) {
    const std::vector<Control> order = focus_order(model_.view);
    if (order.empty()) return;
    std::size_t at = order.size();
    for (std::size_t i = 0; i < order.size(); ++i)
        if (order[i] == model_.focus) at = i;
    std::size_t next = 0;
    if (at == order.size()) next = backwards ? order.size() - 1 : 0;
    else next = backwards ? (at + order.size() - 1) % order.size() : (at + 1) % order.size();
    model_.focus = order[next];
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

void WwApp::cancel_conversion() {
    if (!running_) return;
    cancel_ = true;
    model_.status_text = "Cancelling…";
    model_.stage_label = "Cancelling";
    invalidate();
}

void WwApp::reset_to_idle(const std::string& status) {
    model_ = [&] {
        Model m;
        m.dark = model_.dark;
        m.maximized = model_.maximized;
        m.platforms = model_.platforms;
        m.hot = model_.hot;
        m.status_text = status;
        return m;
    }();
    relayout();
    invalidate();
}

void WwApp::start_conversion(const stdfs::path& file) {
    if (running_) return;
    if (worker_.joinable()) worker_.join();
    model_.view = View::Working;
    model_.file_name = narrow(file.filename().wstring());
    model_.stage_label = std::string(convert::state_label(convert::State::Identifying));
    model_.detail.clear();
    model_.percent = 0;
    model_.output_path.clear();
    model_.done_note.clear();
    model_.status_text = "Converting " + model_.file_name;
    model_.focus = Control::None;
    relayout();
    invalidate();
    cancel_ = false;
    running_ = true;
    HWND window = hwnd_;
    worker_ = std::thread([this, file, window] {
        convert::Request request;
        request.input = file;
        request.converted_root = convert::converted_root();
        convert::Outcome outcome;
        try {
            outcome = convert::convert(
                request, convert::standard_planner(),
                [this, window](const convert::Progress& p) {
                    {
                        std::lock_guard hold(lock_);
                        latest_ = p;
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
    {
        std::lock_guard hold(lock_);
        p = std::move(latest_);
        latest_.reset();
    }
    progress_posted_ = false;
    if (!p || model_.view != View::Working) return;
    if (!cancel_) model_.stage_label = p->label;
    model_.detail = p->detail;
    model_.percent = p->percent;
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
    reset_to_idle("Ready");
    model_.file_name = file_name;
    switch (outcome->state) {
        case convert::State::Done: {
            model_.view = View::Done;
            model_.output_path = narrow(outcome->output.wstring());
            model_.done_note = std::to_string(outcome->files) + (outcome->files == 1 ? " file written." : " files written.");
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
            break;
        case WM_NCLBUTTONUP:
            if (wp == HTCLOSE || wp == HTMAXBUTTON || wp == HTMINBUTTON) return 0;
            break;
        case WM_MOUSEMOVE: {
            set_hot(control_at(lp));
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
        case WM_LBUTTONDOWN: {
            const Control c = control_at(lp);
            model_.focus = Control::None;
            if (c != Control::None) {
                model_.pressed = c;
                SetCapture(hwnd_);
            }
            set_hot(c);
            invalidate();
            return 0;
        }
        case WM_LBUTTONUP: {
            const Control pressed = model_.pressed;
            model_.pressed = Control::None;
            if (GetCapture() == hwnd_) ReleaseCapture();
            const Control c = control_at(lp);
            set_hot(c);
            invalidate();
            if (pressed != Control::None && pressed == c) activate(c);
            return 0;
        }
        case WM_CAPTURECHANGED:
            if (model_.pressed != Control::None) {
                model_.pressed = Control::None;
                invalidate();
            }
            return 0;
        case WM_KEYDOWN: {
            const bool ctrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
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
                if (model_.focus != Control::None) activate(model_.focus);
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
    if (state == "working") {
        m.view = View::Working;
        m.file_name = "setup.exe";
        m.stage_label = std::string(convert::state_label(convert::State::Decompiling));
        m.detail = "17 of 39 functions";
        m.percent = 62;
        m.status_text = "Converting setup.exe";
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
    const Layout layout = compute_layout(model.view, shot.width, shot.height);
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
            err_text("usage: codebreaker app [FILE] [--screenshot OUT.png [--state idle|working|done|unsupported|failed] [--theme light|dark] [--size WxH] [--scale N]]\n");
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
