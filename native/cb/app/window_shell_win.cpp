// cb-unity: alone
// cb/app/window_shell_win.cpp -- see window_shell_win.hpp.
#ifdef _WIN32
#include "cb/app/window_shell_win.hpp"

#include <objbase.h>
#include <ole2.h>
#include <shellapi.h>
#include <shlobj.h>
#include <shobjidl.h>

#include <cstring>
#include <cwchar>
#include <string>

#include "cb/app/window_render_win.hpp"

// The Windows toast needs WinRT's headers and the Windows Runtime Template Library, which MSVC and clang-cl have and MinGW does not: there the
// notification is a balloon.
#if defined(_MSC_VER) && !defined(CB_NO_TOAST)
#define CB_HAVE_TOAST 1
#include <roapi.h>
#include <windows.data.xml.dom.h>
#include <windows.ui.notifications.h>
#include <wrl/client.h>
#include <wrl/event.h>
#include <wrl/wrappers/corewrappers.h>
#endif

namespace cb::app::window {

namespace {

constexpr wchar_t kAppUserModelId[] = L"CodeBreaker.Converter";

#ifdef CB_HAVE_TOAST
// (the window's own ComPtr, window_render_win.hpp, is not WRL's)
template <class T>
using WrlPtr = Microsoft::WRL::ComPtr<T>;
#endif

// `text` into a fixed wide buffer, cut to fit.
template <std::size_t N>
void sh_copy(wchar_t (&to)[N], const std::wstring& text) {
    const std::size_t n = text.size() < N - 1 ? text.size() : N - 1;
    std::memcpy(to, text.data(), n * sizeof(wchar_t));
    to[n] = L'\0';
}

}  // namespace

// ---- the taskbar button

Taskbar::~Taskbar() {
    if (list_ != nullptr) static_cast<ITaskbarList3*>(list_)->Release();
}

void Taskbar::attach(HWND window) {
    window_ = window;
    if (list_ == nullptr) {
        ITaskbarList3* list = nullptr;
        if (SUCCEEDED(CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_INPROC_SERVER, IID_ITaskbarList3, reinterpret_cast<void**>(&list))) && list != nullptr) {
            if (SUCCEEDED(list->HrInit())) list_ = list;
            else list->Release();
        }
    }
    apply();
}

void Taskbar::set(State state, int percent) {
    state_ = state;
    percent_ = percent < 0 ? 0 : percent > 100 ? 100 : percent;
    apply();
}

void Taskbar::apply() {
    if (list_ == nullptr || window_ == nullptr) return;
    ITaskbarList3* list = static_cast<ITaskbarList3*>(list_);
    switch (state_) {
        case State::None: list->SetProgressState(window_, TBPF_NOPROGRESS); break;
        case State::Indeterminate: list->SetProgressState(window_, TBPF_INDETERMINATE); break;
        case State::Normal:
            list->SetProgressState(window_, TBPF_NORMAL);
            list->SetProgressValue(window_, static_cast<ULONGLONG>(percent_), 100);
            break;
        case State::Error:
            list->SetProgressState(window_, TBPF_ERROR);
            list->SetProgressValue(window_, 100, 100);
            break;
    }
}

// ---- the process's identity

void set_app_identity() { SetCurrentProcessExplicitAppUserModelID(kAppUserModelId); }

namespace {

// A toast from a program that is not packaged needs its AppUserModelID registered with a display name; written once, under the current user only.
void sh_register_identity() {
    const std::wstring key = std::wstring(L"Software\\Classes\\AppUserModelId\\") + kAppUserModelId;
    wchar_t existing[64] = {};
    DWORD size = sizeof existing;
    if (RegGetValueW(HKEY_CURRENT_USER, key.c_str(), L"DisplayName", RRF_RT_REG_SZ, nullptr, existing, &size) == ERROR_SUCCESS) return;
    HKEY handle = nullptr;
    if (RegCreateKeyExW(HKEY_CURRENT_USER, key.c_str(), 0, nullptr, 0, KEY_SET_VALUE, nullptr, &handle, nullptr) != ERROR_SUCCESS) return;
    const wchar_t name[] = L"CodeBreaker";
    RegSetValueExW(handle, L"DisplayName", 0, REG_SZ, reinterpret_cast<const BYTE*>(name), sizeof name);
    RegCloseKey(handle);
}

// Windows says whether this is a moment to leave the person alone: a full-screen program, a presentation, or a screen that is not being looked at. (Quiet
// hours / Do not disturb are the system's own: a toast sent then goes to the notification centre without a banner or a sound.)
bool sh_may_notify() {
    QUERY_USER_NOTIFICATION_STATE state = QUNS_ACCEPTS_NOTIFICATIONS;
    if (FAILED(SHQueryUserNotificationState(&state))) return true;
    return state == QUNS_ACCEPTS_NOTIFICATIONS || state == QUNS_QUIET_TIME || state == QUNS_APP;
}

}  // namespace

// ---- the notification

struct Notifier::Impl {
    HWND window = nullptr;
    UINT activation = 0, tray = 0;
    bool balloon_up = false;
#ifdef CB_HAVE_TOAST
    WrlPtr<ABI::Windows::UI::Notifications::IToastNotifier> notifier;
    WrlPtr<ABI::Windows::UI::Notifications::IToastNotification> toast;
    EventRegistrationToken token{};
    bool show_toast(const std::string& xml);
#endif
    bool show_balloon(const notify::Message& message);
    void remove_balloon() {
        if (!balloon_up) return;
        NOTIFYICONDATAW nid{};
        nid.cbSize = sizeof nid;
        nid.hWnd = window;
        nid.uID = 1;
        Shell_NotifyIconW(NIM_DELETE, &nid);
        balloon_up = false;
    }
};

#ifdef CB_HAVE_TOAST
bool Notifier::Impl::show_toast(const std::string& xml) {
    using Microsoft::WRL::Callback;
    using Microsoft::WRL::Implements;
    using Microsoft::WRL::RuntimeClassFlags;
    using Microsoft::WRL::ClassicCom;
    using Microsoft::WRL::FtmBase;
    using namespace Microsoft::WRL::Wrappers;
    namespace Notif = ABI::Windows::UI::Notifications;
    namespace Dom = ABI::Windows::Data::Xml::Dom;

    WrlPtr<Notif::IToastNotificationManagerStatics> manager;
    if (FAILED(Windows::Foundation::GetActivationFactory(HStringReference(RuntimeClass_Windows_UI_Notifications_ToastNotificationManager).Get(), &manager))) return false;
    WrlPtr<Notif::IToastNotifier> made;
    if (FAILED(manager->CreateToastNotifierWithId(HStringReference(kAppUserModelId).Get(), &made))) return false;
    Notif::NotificationSetting setting = Notif::NotificationSetting_Enabled;   // notifications turned off for CodeBreaker, or by policy: nothing is shown
    if (SUCCEEDED(made->get_Setting(&setting)) && setting != Notif::NotificationSetting_Enabled) return false;

    WrlPtr<IInspectable> instance;
    if (FAILED(RoActivateInstance(HStringReference(RuntimeClass_Windows_Data_Xml_Dom_XmlDocument).Get(), &instance))) return false;
    WrlPtr<Dom::IXmlDocument> document;
    WrlPtr<Dom::IXmlDocumentIO> io;
    if (FAILED(instance.As(&document)) || FAILED(document.As(&io))) return false;
    const std::wstring wide = widen(xml);
    if (FAILED(io->LoadXml(HStringReference(wide.c_str()).Get()))) return false;

    WrlPtr<Notif::IToastNotificationFactory> factory;
    if (FAILED(Windows::Foundation::GetActivationFactory(HStringReference(RuntimeClass_Windows_UI_Notifications_ToastNotification).Get(), &factory))) return false;
    WrlPtr<Notif::IToastNotification> made_toast;
    if (FAILED(factory->CreateToastNotification(document.Get(), &made_toast))) return false;

    if (toast && notifier) notifier->Hide(toast.Get());   // (the previous one has been answered by this)
    const HWND target = window;
    const UINT message = activation;
    EventRegistrationToken registered{};
    // A click on the toast or one of its buttons: which one goes to the window, on its own thread.
    const HRESULT added = made_toast->add_Activated(
        Callback<Implements<RuntimeClassFlags<ClassicCom>, ABI::Windows::Foundation::ITypedEventHandler<Notif::ToastNotification*, IInspectable*>, FtmBase>>(
            [target, message](Notif::IToastNotification*, IInspectable* args) -> HRESULT {
                WPARAM action = 2;
                WrlPtr<Notif::IToastActivatedEventArgs> activated;
                if (args != nullptr && SUCCEEDED(args->QueryInterface(IID_PPV_ARGS(activated.GetAddressOf())))) {
                    HString text;
                    if (SUCCEEDED(activated->get_Arguments(text.GetAddressOf()))) {
                        const wchar_t* raw = text.GetRawBuffer(nullptr);
                        if (raw != nullptr && std::wcscmp(raw, L"open") == 0) action = 1;
                    }
                }
                PostMessageW(target, message, action, 0);
                return S_OK;
            })
            .Get(),
        &registered);
    if (FAILED(added)) return false;
    if (FAILED(made->Show(made_toast.Get()))) return false;
    notifier = made;
    toast = made_toast;
    token = registered;
    return true;
}
#endif

// A balloon from the notification area (Windows draws it as a toast on Windows 10 and 11; it has no buttons, a click is "Show").
bool Notifier::Impl::show_balloon(const notify::Message& message) {
    remove_balloon();
    NOTIFYICONDATAW nid{};
    nid.cbSize = sizeof nid;
    nid.hWnd = window;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
    nid.uCallbackMessage = tray;
    nid.hIcon = LoadIconW(GetModuleHandleW(nullptr), MAKEINTRESOURCEW(1));
    sh_copy(nid.szTip, L"CodeBreaker");
    sh_copy(nid.szInfoTitle, widen(message.title));
    sh_copy(nid.szInfo, widen(message.body));
    nid.dwInfoFlags = NIIF_USER | NIIF_RESPECT_QUIET_TIME;
    if (!Shell_NotifyIconW(NIM_ADD, &nid)) return false;
    nid.uVersion = NOTIFYICON_VERSION_4;
    Shell_NotifyIconW(NIM_SETVERSION, &nid);
    balloon_up = true;
    return true;
}

Notifier::Notifier(HWND window, UINT activation_message, UINT tray_message) : impl_(std::make_unique<Impl>()) {
    impl_->window = window;
    impl_->activation = activation_message;
    impl_->tray = tray_message;
}

Notifier::~Notifier() {
    if (!impl_) return;
    impl_->remove_balloon();
#ifdef CB_HAVE_TOAST
    if (impl_->toast && impl_->token.value != 0) impl_->toast->remove_Activated(impl_->token);
#endif
}

bool Notifier::show(const notify::Message& message) {
    if (!sh_may_notify()) return false;
    sh_register_identity();
#ifdef CB_HAVE_TOAST
    if (impl_->show_toast(notify::toast_xml(message))) return true;
#endif
    return impl_->show_balloon(message);
}

void Notifier::on_tray(LPARAM lparam) {
    constexpr UINT kHide = WM_USER + 3, kTimeout = WM_USER + 4, kUserClick = WM_USER + 5;   // NIN_BALLOONHIDE, NIN_BALLOONTIMEOUT, NIN_BALLOONUSERCLICK
    const UINT event = LOWORD(lparam);
    if (event == kUserClick) PostMessageW(impl_->window, impl_->activation, 2, 0);
    if (event == kUserClick || event == kTimeout || event == kHide) impl_->remove_balloon();
}

// ---- dragging the folder out

namespace {

// A data object holding one thing, a CF_HDROP of one path: what Explorer, an editor or a drive window takes as "this folder".
class ShFolderData final : public IDataObject {
public:
    explicit ShFolderData(std::wstring path) : path_(std::move(path)) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID id, void** out) override {
        if (id == IID_IUnknown || id == IID_IDataObject) {
            *out = static_cast<IDataObject*>(this);
            return S_OK;
        }
        *out = nullptr;
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return 2; }   // (lives on the caller's stack for the whole drag: not counted)
    ULONG STDMETHODCALLTYPE Release() override { return 1; }
    HRESULT STDMETHODCALLTYPE GetData(FORMATETC* format, STGMEDIUM* medium) override {
        if (format == nullptr || medium == nullptr) return E_INVALIDARG;
        if (QueryGetData(format) != S_OK) return DV_E_FORMATETC;
        const std::size_t bytes = sizeof(DROPFILES) + (path_.size() + 2) * sizeof(wchar_t);   // the path, its end, and the list's end
        const HGLOBAL memory = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, bytes);
        if (memory == nullptr) return STG_E_MEDIUMFULL;
        auto* drop = static_cast<DROPFILES*>(GlobalLock(memory));
        drop->pFiles = sizeof(DROPFILES);
        drop->fWide = TRUE;
        std::memcpy(reinterpret_cast<char*>(drop) + sizeof(DROPFILES), path_.c_str(), path_.size() * sizeof(wchar_t));
        GlobalUnlock(memory);
        medium->tymed = TYMED_HGLOBAL;
        medium->hGlobal = memory;
        medium->pUnkForRelease = nullptr;
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE GetDataHere(FORMATETC*, STGMEDIUM*) override { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE QueryGetData(FORMATETC* format) override {
        if (format == nullptr) return E_INVALIDARG;
        if (format->cfFormat != CF_HDROP || (format->tymed & TYMED_HGLOBAL) == 0 || format->dwAspect != DVASPECT_CONTENT) return DV_E_FORMATETC;
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE GetCanonicalFormatEtc(FORMATETC*, FORMATETC* out) override {
        out->ptd = nullptr;
        return DATA_S_SAMEFORMATETC;
    }
    HRESULT STDMETHODCALLTYPE SetData(FORMATETC*, STGMEDIUM*, BOOL) override { return E_NOTIMPL; }
    HRESULT STDMETHODCALLTYPE EnumFormatEtc(DWORD direction, IEnumFORMATETC** out) override {
        if (direction != DATADIR_GET) return E_NOTIMPL;
        FORMATETC format{CF_HDROP, nullptr, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
        return SHCreateStdEnumFmtEtc(1, &format, out);
    }
    HRESULT STDMETHODCALLTYPE DAdvise(FORMATETC*, DWORD, IAdviseSink*, DWORD*) override { return OLE_E_ADVISENOTSUPPORTED; }
    HRESULT STDMETHODCALLTYPE DUnadvise(DWORD) override { return OLE_E_ADVISENOTSUPPORTED; }
    HRESULT STDMETHODCALLTYPE EnumDAdvise(IEnumSTATDATA**) override { return OLE_E_ADVISENOTSUPPORTED; }

private:
    std::wstring path_;
};

// The drag ends when the button is let go (dropped) or Esc is pressed (cancelled).
class ShDragSource final : public IDropSource {
public:
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID id, void** out) override {
        if (id == IID_IUnknown || id == IID_IDropSource) {
            *out = static_cast<IDropSource*>(this);
            return S_OK;
        }
        *out = nullptr;
        return E_NOINTERFACE;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return 2; }
    ULONG STDMETHODCALLTYPE Release() override { return 1; }
    HRESULT STDMETHODCALLTYPE QueryContinueDrag(BOOL escape, DWORD keys) override {
        if (escape) return DRAGDROP_S_CANCEL;
        if ((keys & MK_LBUTTON) == 0) return DRAGDROP_S_DROP;
        return S_OK;
    }
    HRESULT STDMETHODCALLTYPE GiveFeedback(DWORD) override { return DRAGDROP_S_USEDEFAULTCURSORS; }
};

}  // namespace

bool drag_folder(const std::filesystem::path& folder) {
    if (folder.empty()) return false;
    ShFolderData data(folder.wstring());
    ShDragSource source;
    DWORD effect = DROPEFFECT_NONE;
    const HRESULT result = DoDragDrop(&data, &source, DROPEFFECT_COPY | DROPEFFECT_LINK, &effect);
    return result == DRAGDROP_S_DROP && effect != DROPEFFECT_NONE;
}

}  // namespace cb::app::window
#endif  // _WIN32
