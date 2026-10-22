// System UI: the dialogs a console's Guide shows for a title (XamShow*UI).
//
// One dialog can be open at a time (another request gets ERROR_ACCESS_DENIED). Opening one broadcasts XN_SYS_UI 1,
// XN_SYS_UI 0 follows once it has gone, after its closing animation. The dialog is answered with the controller of the
// user it was opened for (input.h: the title's controllers read as idle until XN_SYS_UI 0, like XAM's). Every dialog and answer is logged, so scripted headless runs
// (CB_INPUT_SCRIPT) can drive them.
// The dialogs are drawn the way the console draws them, from XAM's own assets (xui/system_ui.h), in the game window
// and in CB_PRESENT_DUMP frames: message boxes (A picks the focused button, B or BACK cancels, the d-pad or left stick
// moves the focus), the sign-in and achievements panes (lists: the d-pad moves, A picks, B or BACK closes) and the
// virtual keyboard (its own keys and button shortcuts; START is Done). Without those assets (system_ui/ missing;
// logged) a native dialog shows the same text and buttons in the game window instead; it can be answered with the
// mouse or keyboard, and whichever answer comes first closes both.
// The Xbox Guide's screens (Community, Profiles, sign-in pane ...) are the shared Guide library
// (native/x360/system/guide); its system UI is a Dialog here (guide::host::OpenSystemUi), the rest of its host is
// kernel/guide_host.cpp.

#include "kernel/kernel.h"
#include "kernel/overlapped.h"
#include "kernel/profile_import.h"
#include "kernel/profile_store.h"
#include "kernel/spa.h"
#include "gpu/presenter.h"
#include "input.h"
#include "kernel/users.h"
#include "live/client.h"
#include "pool.h"
#include "settings.h"
#include "xui/system_ui.h"

#include "guide/guide.h"
#include "guide/host.h"

#include <windows.h>
#include <commctrl.h>

#include <ctime>
#include <format>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#pragma comment(lib, "comctl32.lib")
#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

namespace
{
    constexpr uint32_t kErrorSuccess = 0;
    constexpr uint32_t kErrorAccessDenied = 5;
    constexpr uint32_t kErrorInvalidParameter = 0x57;
    constexpr uint32_t kErrorCancelled = 0x4C7;
    constexpr uint32_t kXnSysUi = 0x00000009;
    constexpr uint32_t kXnSysSigninChanged = 0x0000000A;
    constexpr uint32_t kUserIndexAny = 0xFF;

    // XMB_* (xbox.h)
    constexpr uint32_t kMessageBoxMaxButtons = 3;
    constexpr uint32_t kMessageBoxPasscodeMode = 0x00010000, kMessageBoxVerifyPasscodeMode = 0x00020000;
    constexpr uint32_t kMessageBoxCancelId = 0xFFFFFFFF;

    constexpr uint16_t kButtonA = 0x1000, kButtonB = 0x2000, kButtonBack = 0x0020;
    constexpr uint16_t kButtonUp = 0x0001, kButtonDown = 0x0002;

    std::string Utf8(const std::u16string& text)
    {
        std::wstring wide(text.begin(), text.end());
        const int size = WideCharToMultiByte(CP_UTF8, 0, wide.data(), int(wide.size()), nullptr, 0, nullptr, nullptr);
        std::string out(size_t(size), '\0');
        WideCharToMultiByte(CP_UTF8, 0, wide.data(), int(wide.size()), out.data(), size, nullptr, nullptr);
        return out;
    }

    // A dialog with buttons; the answer is a button index or kMessageBoxCancelId.
    struct Dialog
    {
        std::string kind;
        uint32_t user = 0;
        std::u16string title, text;
        std::vector<std::u16string> buttons;
        uint32_t focus = 0;
        bool cancellable = true;
        // Writes the results and completes the request; `user` is the user index of the controller that answered
        // (input::LastActiveUser() for the native dialog).
        std::function<void(uint32_t answer, uint32_t user)> complete;
        uint32_t overlapped = 0;
        HWND hostDialog = nullptr;
        bool done = false;
        bool hud = false; // drawn by the system UI overlay (xui/system_ui.h)
        xui::MessageIcon icon = xui::MessageIcon::kNone;
        // Sign-in and achievements: the rows of the Guide's list pane; A on row i answers i when i < choosable.
        bool listPane = false;
        size_t choosable = 0;
        std::vector<xui::SignInEntry> signIn;
        std::vector<xui::DeviceEntry> devices; // the device selector's rows
        std::vector<xui::AchievementEntry> achievements;
        // Keyboard UI: the text being edited (guarded by g_mutex), its maximum length and the host edit control style.
        bool textInput = false;
        std::u16string input;
        uint32_t maxLength = 0;
        DWORD editStyle = 0;
        // The sign-in pane's rows after the profiles (Create Profile, Download Profile): A on one of them, by the
        // controller of `user`.
        std::function<void(uint32_t row, uint32_t user)> onExtraRow;
        // The Xbox Guide: takes the controller input itself (and shows itself).
        std::function<bool(uint32_t user, uint16_t pressed)> onPress;
    };

    std::mutex g_mutex;
    std::shared_ptr<Dialog> g_dialog; // the open dialog

    constexpr uint32_t kAnswerTitleCancelled = 0xFFFFFFFE; // XCancelOverlapped by the title

    // XAM's UI from the first request until its scene has gone (xam.xex 17559, 0x816CDxxx): XN_SYS_UI 1 and the
    // system app taking the controllers when it starts, XN_SYS_UI 0 and the controllers back to the title only when
    // its loop ends, after the closing animation. A popup requested while the previous one closes keeps it busy.
    std::mutex g_busyMutex;
    int g_busy = 0;

    void UiBusy()
    {
        std::lock_guard lock(g_busyMutex);
        if (g_busy++ == 0)
        {
            input::HoldTitleInput(true);
            kernel::BroadcastNotification(kXnSysUi, 1);
        }
    }

    void UiIdle()
    {
        std::lock_guard lock(g_busyMutex);
        if (g_busy == 0 || --g_busy != 0)
            return;
        Log("System UI: gone (XN_SYS_UI 0)");
        kernel::BroadcastNotification(kXnSysUi, 0);
        input::HoldTitleInput(false);
    }

    // Closes the dialog with an answer once (whichever of controller and host dialog answers first). Returns true if
    // this call closed it.
    bool Answer(const std::shared_ptr<Dialog>& dialog, uint32_t answer, const std::string& source, uint32_t user)
    {
        HWND host = nullptr;
        {
            std::lock_guard lock(g_mutex);
            if (dialog->done)
                return false;
            dialog->done = true;
            host = dialog->hostDialog;
            if (g_dialog == dialog)
                g_dialog = nullptr;
        }
        Log("System UI: {} answered by {}: {}", dialog->kind, source,
            answer == kMessageBoxCancelId ? std::string("cancelled") : answer == kAnswerTitleCancelled ? std::string("closed") :
                answer < dialog->buttons.size() ? std::format("button {} \"{}\"", answer, Utf8(dialog->buttons[answer]))
                                                : std::format("row {}", answer));
        if (answer == kAnswerTitleCancelled)
            kernel::CompleteOverlapped(dialog->overlapped, kErrorCancelled, 0x80070000 | kErrorCancelled, 0);
        else
            dialog->complete(answer, user);
        if (dialog->hud)
        {
            if (answer < dialog->buttons.size())
                xui::Press(answer);
            xui::ClosePopup(UiIdle);
        }
        else
            UiIdle();
        const int hostId = answer < dialog->buttons.size() ? 100 + int(answer) : IDCANCEL;
        if (host && dialog->textInput)
            PostMessageW(host, WM_COMMAND, MAKEWPARAM(hostId, BN_CLICKED), 0);
        else if (host)
            PostMessageW(host, TDM_CLICK_BUTTON, hostId, 0);
        return true;
    }

    // The keyboard UI's native dialog: description, an edit control and Done / Cancel, from an in-memory template.
    void ShowHostTextDialog(std::shared_ptr<Dialog> dialog)
    {
        std::thread([dialog] {
            std::vector<WORD> t;
            auto align = [&t] { if (t.size() % 2) t.push_back(0); };
            auto text = [&t](const std::wstring& s) { t.insert(t.end(), s.begin(), s.end()); t.push_back(0); };
            auto dword = [&t](DWORD v) { t.push_back(LOWORD(v)); t.push_back(HIWORD(v)); };
            auto item = [&](DWORD style, short x, short y, short cx, short cy, WORD id, WORD atom, const std::wstring& title) {
                align();
                dword(style | WS_CHILD | WS_VISIBLE);
                dword(0);
                t.insert(t.end(), { WORD(x), WORD(y), WORD(cx), WORD(cy), id, 0xFFFF, atom });
                text(title);
                t.push_back(0);
            };
            dword(DS_MODALFRAME | DS_SETFONT | DS_CENTER | WS_POPUP | WS_CAPTION | WS_SYSMENU);
            dword(0);
            t.insert(t.end(), { 4, 0, 0, 260, 84, 0, 0 }); // 4 items, x, y, cx, cy, no menu, default class
            text(std::wstring(dialog->title.begin(), dialog->title.end()));
            t.push_back(9);
            text(L"Segoe UI");
            item(SS_LEFT, 7, 7, 246, 26, 0xFFFF, 0x0082, std::wstring(dialog->text.begin(), dialog->text.end()));
            item(ES_AUTOHSCROLL | WS_BORDER | WS_TABSTOP | dialog->editStyle, 7, 36, 246, 14, 200, 0x0081, L"");
            item(BS_DEFPUSHBUTTON | WS_TABSTOP, 149, 60, 50, 16, 100, 0x0080, L"Done");
            item(BS_PUSHBUTTON | WS_TABSTOP, 203, 60, 50, 16, IDCANCEL, 0x0080, L"Cancel");

            auto proc = [](HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) -> INT_PTR {
                auto* d = reinterpret_cast<Dialog*>(GetWindowLongPtrW(hwnd, DWLP_USER));
                if (message == WM_INITDIALOG)
                {
                    SetWindowLongPtrW(hwnd, DWLP_USER, lParam);
                    d = reinterpret_cast<Dialog*>(lParam);
                    std::wstring initial;
                    {
                        std::lock_guard lock(g_mutex);
                        initial.assign(d->input.begin(), d->input.end());
                    }
                    SendDlgItemMessageW(hwnd, 200, EM_LIMITTEXT, d->maxLength, 0);
                    SetDlgItemTextW(hwnd, 200, initial.c_str()); // sends EN_CHANGE: outside the lock
                    std::lock_guard lock(g_mutex);
                    d->hostDialog = hwnd;
                    if (d->done)
                        PostMessageW(hwnd, WM_COMMAND, MAKEWPARAM(IDCANCEL, BN_CLICKED), 0);
                    return TRUE;
                }
                if (message == WM_COMMAND && LOWORD(wParam) == 200 && HIWORD(wParam) == EN_CHANGE && d)
                {
                    wchar_t buffer[1024] = {};
                    GetDlgItemTextW(hwnd, 200, buffer, int(std::size(buffer)));
                    std::lock_guard lock(g_mutex);
                    if (!d->done)
                        d->input.assign(buffer, buffer + wcslen(buffer));
                    return TRUE;
                }
                if (message == WM_COMMAND && (LOWORD(wParam) == 100 || LOWORD(wParam) == IDCANCEL))
                {
                    EndDialog(hwnd, LOWORD(wParam));
                    return TRUE;
                }
                return FALSE;
            };
            const INT_PTR pressed = DialogBoxIndirectParamW(GetModuleHandleW(nullptr),
                reinterpret_cast<LPCDLGTEMPLATEW>(t.data()), nullptr, proc, reinterpret_cast<LPARAM>(dialog.get()));
            if (pressed <= 0)
            {
                Log("System UI: the native dialog could not be shown; answer with the controller");
                return;
            }
            if (Answer(dialog, pressed == 100 ? 0 : kMessageBoxCancelId, "the native dialog", input::LastActiveUser()))
                input::EndSystemUi();
        }).detach();
    }

    void ShowHostDialog(std::shared_ptr<Dialog> dialog)
    {
        std::thread([dialog] {
            std::wstring title(dialog->title.begin(), dialog->title.end());
            std::wstring text(dialog->text.begin(), dialog->text.end());
            std::vector<std::wstring> labels;
            for (const auto& b : dialog->buttons)
                labels.emplace_back(b.begin(), b.end());
            std::vector<TASKDIALOG_BUTTON> buttons;
            for (size_t i = 0; i < labels.size(); i++)
                buttons.push_back({ int(100 + i), labels[i].c_str() });
            TASKDIALOGCONFIG config{ sizeof(config) };
            config.dwFlags = TDF_POSITION_RELATIVE_TO_WINDOW | (dialog->cancellable ? TDF_ALLOW_DIALOG_CANCELLATION : 0);
            config.pszWindowTitle = L"Xbox Guide";
            config.pszMainInstruction = title.c_str();
            config.pszContent = text.c_str();
            config.cButtons = UINT(buttons.size());
            config.pButtons = buttons.data();
            config.nDefaultButton = 100 + int(dialog->focus);
            config.lpCallbackData = reinterpret_cast<LONG_PTR>(dialog.get());
            config.pfCallback = [](HWND hwnd, UINT notification, WPARAM, LPARAM, LONG_PTR data) -> HRESULT {
                if (notification == TDN_CREATED)
                {
                    auto* d = reinterpret_cast<Dialog*>(data);
                    std::lock_guard lock(g_mutex);
                    d->hostDialog = hwnd;
                    if (d->done)
                        PostMessageW(hwnd, TDM_CLICK_BUTTON, 100 + d->focus, 0);
                }
                return S_OK;
            };
            int pressed = 0;
            if (FAILED(TaskDialogIndirect(&config, &pressed, nullptr, nullptr)))
            {
                Log("System UI: the native dialog could not be shown; answer with the controller");
                return;
            }
            const uint32_t answer = pressed >= 100 && pressed < 100 + int(labels.size()) ? uint32_t(pressed - 100) :
                kMessageBoxCancelId;
            if (answer == kMessageBoxCancelId && !dialog->cancellable)
                return;
            if (Answer(dialog, answer, "the native dialog", input::LastActiveUser()))
                input::EndSystemUi();
        }).detach();
    }

    // Opens a dialog; returns false if another system UI is open.
    bool Open(std::shared_ptr<Dialog> dialog)
    {
        {
            std::lock_guard lock(g_mutex);
            if (g_dialog)
                return false;
            g_dialog = dialog;
        }
        std::string buttons;
        for (size_t i = 0; i < dialog->buttons.size(); i++)
            buttons += std::format("{}[{}] \"{}\"", i ? ", " : "", i, Utf8(dialog->buttons[i]));
        Log("System UI: {} for user {}: \"{}\" / \"{}\"; buttons {}; focus {}{}", dialog->kind, dialog->user,
            Utf8(dialog->title), Utf8(dialog->text), buttons, dialog->focus,
            dialog->textInput ? std::format("; text \"{}\" (up to {} characters)", Utf8(dialog->input), dialog->maxLength) :
                std::string());
        UiBusy();
        if (dialog->onPress)
        {
            input::BeginSystemUi(dialog->user, dialog->onPress);
            return true;
        }
        input::BeginSystemUi(dialog->user, [dialog](uint32_t user, uint16_t pressed) {
            const std::string source = std::format("the controller of user {}", user);
            // Not answerable while it waits for the previous popup's closing animation.
            if (dialog->hud && !xui::PopupShowing())
            {
                std::lock_guard lock(g_mutex);
                return dialog->done;
            }
            if (dialog->hud && dialog->listPane)
            {
                const xui::PaneAction action = xui::PanePress(pressed);
                if (action.kind == xui::PaneAction::kCancelled)
                    return Answer(dialog, kMessageBoxCancelId, source, user) || true;
                if (action.kind == xui::PaneAction::kChosen && action.index < dialog->choosable)
                    return Answer(dialog, action.index, source, user) || true;
                if (action.kind == xui::PaneAction::kChosen && dialog->onExtraRow)
                    dialog->onExtraRow(action.index, user);
                else if (action.kind == xui::PaneAction::kChosen)
                    Log("System UI: {}: row {} has no action here", dialog->kind, action.index);
                std::lock_guard lock(g_mutex);
                return dialog->done;
            }
            if (dialog->hud && dialog->textInput)
            {
                // The Guide's keyboard handles its own keys; it answers with Done or a cancel.
                const xui::KeyboardAction action = xui::KeyboardPress(pressed);
                if (action == xui::KeyboardAction::kDone)
                {
                    const std::u16string text = xui::KeyboardText();
                    {
                        std::lock_guard lock(g_mutex);
                        dialog->input = text;
                    }
                    return Answer(dialog, 0, source, user) || true;
                }
                if (action == xui::KeyboardAction::kCancel)
                    return Answer(dialog, kMessageBoxCancelId, source, user) || true;
                std::lock_guard lock(g_mutex);
                return dialog->done;
            }
            if (pressed & kButtonA)
                return Answer(dialog, dialog->focus, source, user) || true;
            if ((pressed & (kButtonB | kButtonBack)) && dialog->cancellable)
                return Answer(dialog, kMessageBoxCancelId, source, user) || true;
            const uint32_t before = dialog->focus;
            if ((pressed & kButtonUp) && dialog->focus > 0)
                Log("System UI: focus on button {}", --dialog->focus);
            if ((pressed & kButtonDown) && dialog->focus + 1 < dialog->buttons.size())
                Log("System UI: focus on button {}", ++dialog->focus);
            if (dialog->hud && dialog->focus != before)
                xui::SetFocus(dialog->focus);
            std::lock_guard lock(g_mutex);
            return dialog->done;
        });
        if ((dialog->kind == "message box" || dialog->kind == "Xbox Live sign-in") && xui::SystemUiAvailable())
        {
            dialog->hud = true;
            xui::ShowMessageBox(dialog->title, dialog->text, dialog->buttons, dialog->focus, dialog->icon);
        }
        else if (dialog->textInput && xui::SystemUiAvailable())
        {
            dialog->hud = true;
            xui::ShowKeyboard(dialog->title, dialog->text, dialog->input, dialog->maxLength);
        }
        else if (dialog->kind == "device selector" && xui::SystemUiAvailable())
        {
            dialog->hud = dialog->listPane = true;
            xui::ShowDeviceSelector(dialog->devices, dialog->focus);
        }
        else if (dialog->kind == "sign-in UI" && xui::SystemUiAvailable())
        {
            dialog->hud = dialog->listPane = true;
            xui::ShowSignIn(dialog->signIn, dialog->focus);
        }
        else if (dialog->kind == "achievements UI" && xui::SystemUiAvailable())
        {
            dialog->hud = dialog->listPane = true;
            xui::ShowAchievements(kernel::spa::TitleName(), dialog->achievements);
        }
        else if (gpu::PresenterActive())
            dialog->textInput ? ShowHostTextDialog(dialog) : ShowHostDialog(dialog);
        return true;
    }

    // ---- The Xbox Guide (native/x360/system/guide): its system UI is a Dialog here ----

    // The Guide's system UI (guide/host.h): the open Dialog of kind "Xbox Guide" it got from OpenSystemUi.
    class GuideUi : public guide::SystemUi
    {
    public:
        explicit GuideUi(std::weak_ptr<Dialog> dialog) : dialog_(std::move(dialog)) {}
        bool Close(const char* why, uint32_t user) override
        {
            auto d = dialog_.lock();
            return d && Answer(d, kMessageBoxCancelId, why, user);
        }
        bool Closed() const override
        {
            auto d = dialog_.lock();
            std::lock_guard lock(g_mutex);
            return !d || d->done;
        }

    private:
        std::weak_ptr<Dialog> dialog_;
    };

    // The sign-in pane's row for a profile ID among `eligible` (profile indices), else 0.
    uint32_t RowOf(const std::vector<size_t>& eligible, const std::vector<uint64_t>& ids)
    {
        for (size_t row = 0; row < eligible.size(); row++)
            for (uint64_t id : ids)
                if (kernel::users::Profiles()[eligible[row]].xuid == id)
                    return uint32_t(row);
        return 0;
    }
}

uint32_t kernel::ShowCommunityTab(uint32_t user, int tab)
{
    return guide::ShowCommunity(user, guide::Tab(tab & 3));
}

std::shared_ptr<guide::SystemUi> guide::host::OpenSystemUi(
    uint32_t user, std::function<bool(SystemUi& ui, uint32_t user, uint16_t pressed)> onPress, std::string& busyKind)
{
    auto dialog = std::make_shared<Dialog>();
    dialog->kind = "Xbox Guide";
    dialog->user = user;
    dialog->title = u"Xbox Guide";
    dialog->hud = true;
    dialog->complete = [](uint32_t, uint32_t) {};
    auto ui = std::make_shared<GuideUi>(dialog);
    dialog->onPress = [ui, onPress = std::move(onPress)](uint32_t u, uint16_t pressed) {
        return ui->Closed() || onPress(*ui, u, pressed);
    };
    if (!Open(dialog))
    {
        std::lock_guard lock(g_mutex);
        busyKind = g_dialog ? g_dialog->kind : std::string("system UI");
        return nullptr;
    }
    return ui;
}

void guide::host::EndSystemUiInput()
{
    input::EndSystemUi();
}

namespace
{
    bool ValidGamertag(const std::u16string& g); // below, with the create-profile dialog
}

std::optional<size_t> kernel::CreateLocalProfile(const std::u16string& gamertag, std::u16string& reason)
{
    if (!ValidGamertag(gamertag))
    {
        Log("System UI: create profile: \"{}\" is not a valid gamertag", Utf8(gamertag));
        reason = u"That gamertag can't be used. Use 1 to 15 letters, numbers and single spaces, starting with a letter.";
        return std::nullopt;
    }
    const auto xuid = kernel::profile::CreateProfile(gamertag);
    if (!xuid)
    {
        reason = u"The profile could not be saved to the hard drive.";
        return std::nullopt;
    }
    kernel::profile::EnsureProfileHeaders();
    kernel::users::AddStoredProfiles();
    const auto& profiles = kernel::users::Profiles();
    for (size_t i = 0; i < profiles.size(); i++)
        if (profiles[i].xuid == *xuid)
            return i;
    reason = u"The profile could not be listed.";
    return std::nullopt;
}

void kernel::InitializeGuide()
{
    guide::host::createProfile = kernel::CreateLocalProfile; // the Guide sign-in pane's Create Profile
    input::SetGuideHandler(guide::GuideButton);
    input::SetTextInput(xui::KeyboardOpen, xui::KeyboardTypeChar); // typing into the Guide's keyboard
    xui::PreloadSystemUi();
}

bool kernel::SystemUiActive()
{
    std::lock_guard lock(g_busyMutex);
    return g_busy != 0;
}

// VOID XamAppUnloadStack(BOOL) (XAM 0x816CAF08 -> 0x816CAD78): unloads the stack of system apps XAM has loaded (hud,
// signin, vk, gamerprofile...), ending their UI. XAM's message boxes are scenes of XAM's own UI (0x81694058 ->
// 0x817CE9F0), not loaded apps, so they stay. The dash calls it 60 ms (KeTimeStampBundle ticks) after opening its
// error box (export 10578), clearing other apps out of the box's way. Here: an open app-backed UI (sign-in, Guide,
// keyboard, achievements) closes as if its title had cancelled it; a message box stays.
GUEST_IMPORT(XamAppUnloadStack)
{
    std::shared_ptr<Dialog> dialog;
    {
        std::lock_guard lock(g_mutex);
        dialog = g_dialog;
    }
    if (dialog && dialog->kind != "message box" &&
        Answer(dialog, kAnswerTitleCancelled, "XamAppUnloadStack", kUserIndexAny))
        input::EndSystemUi();
}

bool kernel::CancelSystemUi(uint32_t overlapped)
{
    std::shared_ptr<Dialog> dialog;
    {
        std::lock_guard lock(g_mutex);
        if (!g_dialog || g_dialog->overlapped != overlapped)
            return false;
        dialog = g_dialog;
    }
    if (Answer(dialog, kAnswerTitleCancelled, "the title", kUserIndexAny))
        input::EndSystemUi();
    return true;
}

std::u16string kernel::ReadGuestWideString(uint32_t address, size_t maxLength)
{
    std::u16string text;
    if (address == 0)
        return text;
    for (size_t i = 0; i < maxLength; i++)
    {
        const char16_t c = char16_t(guest::g_memory.Read16(address + uint32_t(i) * 2));
        if (c == 0)
            break;
        text += c;
    }
    return text;
}

namespace
{
    // XamShowMessageBoxUI's pass code modes (XAM's box with the skin's PasscodeMessageBox): four of the pass code
    // buttons (X Y LB RB LT RT and the d-pad, as VK_PAD_* codes), B / BACK cancels (XMB_CANCELID). XMB_PASSCODEMODE
    // returns them in MESSAGEBOX_RESULT.rgwPasscode; XMB_VERIFYPASSCODEMODE compares them with the four the caller put
    // there (the dash's Family Settings: ConvertXOnlinePasscodeToVKey of XConfig 3/0x17) and answers 0 once they
    // match, "Pass code incorrect." and another try otherwise.
    uint32_t ShowPasscodeBox(uint32_t user, const std::u16string& title, const std::u16string& text, uint32_t flags,
                             uint32_t result, uint32_t overlapped)
    {
        if ((user >= 4 && user != kUserIndexAny) || result == 0 || overlapped == 0)
            Fatal("XamShowMessageBoxUI: unexpected pass code arguments (user {}, result {:08X}, overlapped {:08X})", user,
                  result, overlapped);
        struct Passcode
        {
            bool verify = false;
            uint16_t expected[4] = {};
            std::vector<uint16_t> entered;
        };
        auto passcode = std::make_shared<Passcode>();
        passcode->verify = (flags & kMessageBoxVerifyPasscodeMode) != 0;
        for (uint32_t i = 0; i < 4; i++)
            passcode->expected[i] = guest::g_memory.Read16(result + i * 2);
        auto dialog = std::make_shared<Dialog>();
        dialog->kind = passcode->verify ? "pass code verify box" : "pass code box";
        dialog->user = user;
        dialog->title = title;
        dialog->text = text.empty() ? xui::CommonString(0x87, u"Enter pass code.") : text;
        dialog->hud = true;
        dialog->overlapped = overlapped;
        dialog->complete = [result, overlapped, passcode](uint32_t answer, uint32_t) {
            if (answer == 0 && !passcode->verify)
                for (uint32_t i = 0; i < 4; i++)
                    guest::g_memory.Write16(result + i * 2, passcode->entered[i]);
            else
                guest::g_memory.Write32(result, answer);
            kernel::CompleteOverlapped(overlapped, kErrorSuccess, 0, 0);
        };
        std::weak_ptr<Dialog> weak = dialog;
        dialog->onPress = [weak, passcode](uint32_t u, uint16_t pressed) {
            auto dialog = weak.lock();
            if (!dialog)
                return true;
            const std::string source = std::format("the controller of user {}", u);
            if (!xui::PopupShowing())
            {
                std::lock_guard lock(g_mutex);
                return dialog->done;
            }
            if (pressed & (kButtonB | kButtonBack))
                return Answer(dialog, kMessageBoxCancelId, source, u) || true;
            static const std::pair<uint16_t, uint16_t> kKeys[] = { { 0x4000, 0x5802 }, { 0x8000, 0x5803 },
                { 0x0004, 0x5812 }, { 0x0008, 0x5813 }, { 0x0001, 0x5810 }, { 0x0002, 0x5811 },
                { input::kUiLeftTrigger, 0x5806 }, { input::kUiRightTrigger, 0x5807 }, { 0x0100, 0x5805 },
                { 0x0200, 0x5804 } };
            for (const auto& [bit, vk] : kKeys)
                if ((pressed & bit) && passcode->entered.size() < 4)
                    passcode->entered.push_back(vk);
            if (passcode->entered.size() < 4)
            {
                xui::SetPasscodeEntered(uint32_t(passcode->entered.size()), u"");
                std::lock_guard lock(g_mutex);
                return dialog->done;
            }
            if (!passcode->verify || std::equal(passcode->entered.begin(), passcode->entered.end(), passcode->expected))
                return Answer(dialog, 0, source, u) || true;
            Log("System UI: pass code incorrect");
            passcode->entered.clear();
            xui::SetPasscodeEntered(0, u"Pass code incorrect."); // passcode.xur's txtIncorrectPasscode
            std::lock_guard lock(g_mutex);
            return dialog->done;
        };
        kernel::BeginOverlapped(overlapped);
        if (!xui::SystemUiAvailable() || !Open(dialog))
        {
            Log("System UI: {}: {}", dialog->kind, xui::SystemUiAvailable() ? "another system UI is open" :
                "needs the console's system UI assets");
            kernel::CompleteOverlapped(overlapped, kErrorAccessDenied, 0, 0);
            return kErrorAccessDenied;
        }
        xui::ShowPasscodeBox(dialog->title, dialog->text, xui::MessageIcon(flags & 3));
        return kernel::kErrorIoPending;
    }
}

// DWORD XamShowMessageBoxUI(DWORD UserIndex, LPCWSTR Title, LPCWSTR Text, DWORD ButtonCount, LPCWSTR* Buttons,
//     DWORD FocusButton, DWORD Flags, PMESSAGEBOX_RESULT Result, PXOVERLAPPED Overlapped)
// MESSAGEBOX_RESULT: dwButtonPressed (u32) or a 4-WORD passcode. The XDK: the overlapped request completes with
// ERROR_SUCCESS when the gamer pressed a button, with the button index or XMB_CANCELID (B / BACK) in the result.
// ButtonCount 0: XAM's box (0x81694058) supplies its own buttons for flag 0x100000 (common string 0x79 "OK") and
// 0x200000 (0x12A "Yes", 0x78 "No"); other flags with no buttons stop.
GUEST_IMPORT(XamShowMessageBoxUI)
{
    auto& mem = guest::g_memory;
    const uint32_t user = ctx.r3.u32;
    const uint32_t buttonCount = ctx.r6.u32;
    const uint32_t buttonsPtr = ctx.r7.u32;
    const uint32_t focus = ctx.r8.u32;
    const uint32_t flags = ctx.r9.u32;
    const uint32_t result = ctx.r10.u32;
    const uint32_t overlapped = kernel::StackArg32(ctx, 8);
    if (flags & (kMessageBoxPasscodeMode | kMessageBoxVerifyPasscodeMode))
    {
        ctx.r3.u64 = ShowPasscodeBox(user, kernel::ReadGuestWideString(ctx.r4.u32, 256),
                                     kernel::ReadGuestWideString(ctx.r5.u32, 1024), flags, result, overlapped);
        return;
    }
    constexpr uint32_t kDefaultOk = 0x100000, kDefaultYesNo = 0x200000;
    const bool defaultButtons = buttonCount == 0 && (flags & (kDefaultOk | kDefaultYesNo));
    const uint32_t shownButtons = defaultButtons ? ((flags & kDefaultOk) ? 1 : 2) : buttonCount;
    if ((user >= 4 && user != kUserIndexAny) || shownButtons == 0 || buttonCount > kMessageBoxMaxButtons ||
        (buttonsPtr == 0 && !defaultButtons) || focus >= shownButtons || result == 0 || overlapped == 0)
        Fatal("XamShowMessageBoxUI: unexpected arguments (user {}, {} buttons at {:08X}, focus {}, result {:08X}, "
            "overlapped {:08X})", user, buttonCount, buttonsPtr, focus, result, overlapped);

    auto dialog = std::make_shared<Dialog>();
    dialog->kind = "message box";
    dialog->user = user;
    dialog->title = kernel::ReadGuestWideString(ctx.r4.u32, 256);
    dialog->text = kernel::ReadGuestWideString(ctx.r5.u32, 1024);
    if (defaultButtons && (flags & kDefaultOk))
        dialog->buttons = { xui::CommonString(0x79, u"OK") };
    else if (defaultButtons)
        dialog->buttons = { xui::CommonString(0x12A, u"Yes"), xui::CommonString(0x78, u"No") };
    for (uint32_t i = 0; i < buttonCount; i++)
        dialog->buttons.push_back(kernel::ReadGuestWideString(mem.Read32(buttonsPtr + i * 4), 256));
    dialog->focus = focus;
    dialog->icon = xui::MessageIcon(flags & 3); // XMB_NOICON / ERRORICON / WARNINGICON / ALERTICON
    dialog->overlapped = overlapped;
    dialog->complete = [result, overlapped](uint32_t answer, uint32_t) {
        guest::g_memory.Write32(result, answer);
        kernel::CompleteOverlapped(overlapped, kErrorSuccess, 0, 0);
    };
    kernel::BeginOverlapped(overlapped);
    if (!Open(dialog))
    {
        kernel::CompleteOverlapped(overlapped, kErrorAccessDenied, 0, 0);
        ctx.r3.u64 = kErrorAccessDenied;
        return;
    }
    ctx.r3.u64 = kernel::kErrorIoPending;
}

// XamShowFofUI(DWORD UserIndex, XUID Xuid, LPCSTR Gamertag) (XAM 0x816C44A0: message 0x21002 to the friends app,
// flags 0x100000, the gamertag up to 15 characters): that player's friends. XamShowJoinSessionInProgressUI(DWORD
// UserIndex, XUID Xuid) (0x816C4370, flags 0x8020): joining their game session. XAM does nothing for a XUID that is
// not an online one (0009...).
GUEST_IMPORT(XamShowFofUI)
{
    const uint32_t user = ctx.r3.u32;
    const uint64_t xuid = ctx.r4.u64;
    std::string tag;
    for (uint32_t i = 0; ctx.r5.u32 && i < 15; i++)
    {
        const char c = char(guest::g_memory.Read8(ctx.r5.u32 + i));
        if (!c)
            break;
        tag += c;
    }
    Log("XamShowFofUI(user {}, {:016X}, \"{}\")", user, xuid, tag);
    ctx.r3.u64 = (xuid >> 48) == 0x0009 ? guide::ShowFriendsOf(user, xuid, tag) : 0;
}

GUEST_IMPORT(XamShowJoinSessionInProgressUI)
{
    const uint32_t user = ctx.r3.u32;
    const uint64_t xuid = ctx.r4.u64;
    Log("XamShowJoinSessionInProgressUI(user {}, {:016X})", user, xuid);
    ctx.r3.u64 = (xuid >> 48) == 0x0009 ? guide::ShowJoinSessionInProgress(user, xuid) : 0;
}

// ---- Kinect (Nui) screens: this console has no Kinect sensor ----
// XAM's Nui screens (17559) first ask FUN_816C1808: XamNuiHudIsEnabled false (no sensor) -> 5 (ERROR_ACCESS_DENIED)
// and nothing is shown. XamShowNuiHardwareRequiredUI(0) is XamShowNuiTroubleshooterUI(0xFF, 0, 0x400000); without the
// Kinect experience (XamPackageManagerGetExperienceMode bit 2: 1 here) the troubleshooter is XAM's box with common
// strings 0x11F "Update Required" / 0x4F "A system update is required to use Kinect...", OK and the error icon (flags
// 0x100001), no request to complete.
namespace
{
    void NuiUnavailable(PPCContext& ctx, const char* name)
    {
        Log("{}: no Kinect sensor (XamNuiHudIsEnabled false): ERROR_ACCESS_DENIED", name);
        ctx.r3.u64 = kErrorAccessDenied;
    }

    uint32_t ShowNuiTroubleshooter(uint32_t user)
    {
        auto dialog = std::make_shared<Dialog>();
        dialog->kind = "message box";
        dialog->user = user;
        dialog->title = xui::CommonString(0x11F, u"Update Required");
        dialog->text = xui::CommonString(0x4F, u"A system update is required to use Kinect.");
        dialog->buttons = { xui::CommonString(0x79, u"OK") };
        dialog->icon = xui::MessageIcon::kError;
        dialog->complete = [](uint32_t, uint32_t) {};
        return Open(dialog) ? kErrorSuccess : kErrorAccessDenied;
    }
}

// XamNuiHudGetEngagedTrackingID / XamNuiHudSetEngagedTrackingID (XAM 0x816AC408 / 0x816AC428): no Nui HUD here, so
// no engaged player (0), and setting one fails with E_FAIL (0 is accepted).
GUEST_IMPORT(XamNuiHudGetEngagedTrackingID) { ctx.r3.u64 = 0; }
GUEST_IMPORT(XamNuiHudSetEngagedTrackingID) { ctx.r3.u64 = ctx.r3.u32 ? 0xFFFFFFFF80004005ull : 0; }

GUEST_IMPORT(XamShowNuiAchievementsUI) { NuiUnavailable(ctx, "XamShowNuiAchievementsUI"); }
GUEST_IMPORT(XamShowNuiGamerCardUIForXUID) { NuiUnavailable(ctx, "XamShowNuiGamerCardUIForXUID"); }
GUEST_IMPORT(XamShowNuiGamesUI) { NuiUnavailable(ctx, "XamShowNuiGamesUI"); }
GUEST_IMPORT(XamShowNuiGuideUI) { NuiUnavailable(ctx, "XamShowNuiGuideUI"); }
GUEST_IMPORT(XamShowNuiJoinSessionInProgressUI) { NuiUnavailable(ctx, "XamShowNuiJoinSessionInProgressUI"); }
GUEST_IMPORT(XamShowNuiMessageBoxUI) { NuiUnavailable(ctx, "XamShowNuiMessageBoxUI"); }
GUEST_IMPORT(XamShowNuiMessagesUI) { NuiUnavailable(ctx, "XamShowNuiMessagesUI"); }

GUEST_IMPORT(XamShowNuiTroubleshooterUI)
{
    Log("XamShowNuiTroubleshooterUI(user {}, flags {:08X})", ctx.r3.u32, ctx.r5.u32);
    ctx.r3.u64 = ShowNuiTroubleshooter(ctx.r3.u32);
}

GUEST_IMPORT(XamShowNuiHardwareRequiredUI)
{
    if (ctx.r3.u32 != 0)
    {
        ctx.r3.u64 = kErrorInvalidParameter;
        return;
    }
    Log("XamShowNuiHardwareRequiredUI");
    ctx.r3.u64 = ShowNuiTroubleshooter(kUserIndexAny);
}

// DWORD XamShowOptionsUI(DWORD UserIndex) (XAM 0x816C2BD8: the HUD's mode 6): Settings > Preferences.
GUEST_IMPORT(XamShowOptionsUI)
{
    Log("XamShowOptionsUI(user {})", ctx.r3.u32);
    ctx.r3.u64 = guide::ShowPreferences(ctx.r3.u32);
}

// XamShowMessageBoxUIEx (XAM 0x816C5788) passes its arguments on to XamShowMessageBoxUI unchanged.
GUEST_IMPORT(XamShowMessageBoxUIEx)
{
    __imp__XamShowMessageBoxUI(ctx, base);
}

// VOID XamShowMessageBox(...) (XAM 0x816CB168): the system process's own box (KeGetCurrentProcessType 2, drawn by
// XAM's renderer); called from a title it does nothing, as on the console.
GUEST_IMPORT(XamShowMessageBox)
{
    Log("XamShowMessageBox: called from the title: nothing shown (system process only)");
}

// DWORD XamShowOptionalMediaUpdateRequiredUI(DWORD UserIndex, PXOVERLAPPED Overlapped) (XAM 0x816AD570): message
// 0x21022 to the HUD with XAM's common strings 124 "Optional Media Update Required", 123 "A media update is required
// to play this content." (122 the signed-out text): a box with OK; the request completes when it is answered.
GUEST_IMPORT(XamShowOptionalMediaUpdateRequiredUI)
{
    const uint32_t user = ctx.r3.u32, overlapped = ctx.r4.u32;
    auto dialog = std::make_shared<Dialog>();
    dialog->kind = "message box";
    dialog->user = user;
    dialog->title = xui::CommonString(124, u"Optional Media Update Required");
    dialog->text = xui::CommonString(123, u"A media update is required to play this content.");
    dialog->buttons = { xui::CommonString(0x79, u"OK") };
    dialog->overlapped = overlapped;
    dialog->complete = [overlapped](uint32_t, uint32_t) {
        if (overlapped)
            kernel::CompleteOverlapped(overlapped, kErrorSuccess, 0, 0);
    };
    if (overlapped)
        kernel::BeginOverlapped(overlapped);
    if (!Open(dialog))
    {
        if (overlapped)
            kernel::CompleteOverlapped(overlapped, kErrorAccessDenied, 0, 0);
        ctx.r3.u64 = kErrorAccessDenied;
        return;
    }
    ctx.r3.u64 = overlapped ? kernel::kErrorIoPending : kErrorSuccess;
}

// DWORD XamShowKeyboardUI(DWORD UserIndex, DWORD Flags, LPCWSTR DefaultText, LPCWSTR TitleText,
//     LPCWSTR DescriptionText, LPWSTR ResultText, DWORD cchResultText, PXOVERLAPPED Overlapped) (XShowKeyboardUI)
// The Guide's virtual keyboard, starting with DefaultText. Returns ERROR_IO_PENDING; the XDK: the request's
// dwExtendedError is ERROR_SUCCESS when the gamer pressed Done (ResultText then holds the text, terminated, cut to
// cchResultText - 1 characters) and ERROR_CANCELLED for Cancel or the Guide button. InternalLow is ERROR_SUCCESS
// either way (as Xenia does; TU46 tests only dwExtendedError, 0x8276A970) and ResultText is left alone on cancel.
// The Guide's keyboard (xui::ShowKeyboard) edits the text with the controller: the d-pad picks a key, A types it, X
// Backspace, Y Space, LB/RB move the cursor, LT/RT switch to the symbols / accents layouts, the left stick toggles
// Caps, START (or the Done key) confirms and B cancels; scripted runs press START to keep DefaultText. Without the
// XAM assets the native dialog edits it instead (A on its Done confirms the current text). The VKBD_LATIN_* character
// subsets and password mode are not enforced on the Guide's keyboard (only on the native edit control).
GUEST_IMPORT(XamShowKeyboardUI)
{
    const uint32_t user = ctx.r3.u32, flags = ctx.r4.u32;
    const uint32_t result = ctx.r8.u32, resultLength = ctx.r9.u32, overlapped = ctx.r10.u32;
    constexpr uint32_t kNumeric = 0x00000020, kPassword = 0x00000080; // VKBD_LATIN_*
    if (result == 0 || resultLength == 0)
    {
        ctx.r3.u64 = kErrorInvalidParameter;
        return;
    }
    if (user >= 4 || overlapped == 0)
        Fatal("XamShowKeyboardUI: unexpected arguments (user {}, overlapped {:08X})", user, overlapped);

    auto dialog = std::make_shared<Dialog>();
    dialog->kind = "keyboard UI";
    dialog->user = user;
    dialog->title = kernel::ReadGuestWideString(ctx.r6.u32, 256);
    dialog->text = kernel::ReadGuestWideString(ctx.r7.u32, 1024);
    dialog->buttons = { u"Done", u"Cancel" };
    dialog->textInput = true;
    dialog->maxLength = resultLength - 1;
    dialog->input = kernel::ReadGuestWideString(ctx.r5.u32, dialog->maxLength);
    dialog->editStyle = (flags & kNumeric ? ES_NUMBER : 0) | (flags & kPassword ? ES_PASSWORD : 0);
    dialog->overlapped = overlapped;
    Dialog* d = dialog.get();
    dialog->complete = [d, result, overlapped](uint32_t answer, uint32_t) {
        if (answer != 0)
        {
            kernel::CompleteOverlapped(overlapped, kErrorSuccess, kErrorCancelled, 0);
            return;
        }
        std::u16string text;
        {
            std::lock_guard lock(g_mutex);
            text = d->input.substr(0, d->maxLength);
        }
        Log("System UI: keyboard UI result \"{}\"", Utf8(text));
        for (size_t i = 0; i < text.size(); i++)
            guest::g_memory.Write16(result + uint32_t(i) * 2, text[i]);
        guest::g_memory.Write16(result + uint32_t(text.size()) * 2, 0);
        kernel::CompleteOverlapped(overlapped, kErrorSuccess, kErrorSuccess, 0);
    };
    kernel::BeginOverlapped(overlapped);
    if (!Open(dialog))
    {
        kernel::CompleteOverlapped(overlapped, kErrorAccessDenied, 0, 0);
        ctx.r3.u64 = kErrorAccessDenied;
        return;
    }
    ctx.r3.u64 = kernel::kErrorIoPending;
}

namespace
{
    // The console's gamertag rules: 1-15 characters, letters, digits and single spaces, starting with a letter.
    bool ValidGamertag(const std::u16string& g)
    {
        auto letter = [](char16_t c) { return (c >= u'A' && c <= u'Z') || (c >= u'a' && c <= u'z'); };
        auto digit = [](char16_t c) { return c >= u'0' && c <= u'9'; };
        if (g.empty() || g.size() > 15 || !letter(g[0]) || g.back() == u' ')
            return false;
        for (size_t i = 0; i < g.size(); i++)
            if (!(letter(g[i]) || digit(g[i]) || (g[i] == u' ' && g[i - 1] != u' ')))
                return false;
        return true;
    }

    // XAM's create-profile flow (XamShowCreateProfileUIEx: message 0x2100E to the HUD): the gamertag on the Guide's
    // keyboard, then the new offline profile is created on the hard drive and signed in to the controller that asked
    // (XN_SYS_SIGNINCHANGED, the sign-in toast). A gamertag the rules refuse opens the keyboard again with the reason.
    bool ShowCreateProfile(uint32_t user, const std::u16string& initial, const std::u16string& message)
    {
        auto dialog = std::make_shared<Dialog>();
        dialog->kind = "create profile UI";
        dialog->user = user;
        dialog->title = u"Create Profile";
        dialog->text = message;
        dialog->buttons = { u"Done", u"Cancel" };
        dialog->textInput = true;
        dialog->maxLength = 15;
        dialog->input = initial.substr(0, 15);
        Dialog* d = dialog.get();
        dialog->complete = [d, user](uint32_t answer, uint32_t answering) {
            if (answer != 0)
                return; // cancelled: nothing created
            std::u16string gamertag;
            {
                std::lock_guard lock(g_mutex);
                gamertag = d->input.substr(0, d->maxLength);
            }
            const uint32_t target = user < kernel::users::kMaxUsers ? user : answering;
            std::u16string reason;
            const std::optional<size_t> index = kernel::CreateLocalProfile(gamertag, reason);
            if (!index)
            {
                std::thread([target, gamertag, reason] {
                    for (int i = 0; i < 100 && !ShowCreateProfile(target, gamertag, reason); i++)
                        Sleep(50);
                }).detach();
                return;
            }
            const kernel::users::Profile profile = kernel::users::Profiles()[*index];
            if (target < kernel::users::kMaxUsers)
            {
                kernel::users::SignIn(target, *index);
                kernel::BroadcastNotification(kXnSysSigninChanged, 1u << target);
                kernel::SignInToast(target, profile.gamertag, false, false);
                // Then XAM's Save Profile (createprofile.cp's Congratulations) once this keyboard has closed.
                std::thread([target] {
                    for (int i = 0; i < 100 && guide::ShowProfileCreated(target) != 0; i++)
                        Sleep(50);
                }).detach();
            }
        };
        return Open(dialog);
    }
}

// DWORD XamShowCreateProfileUIEx(DWORD UserIndex, DWORD Type, LPCSTR Gamertag): the create-profile flow, Gamertag
// (if any) as the keyboard's starting text; ERROR_SUCCESS once shown (no overlapped: the title learns of the profile
// from XN_SYS_SIGNINCHANGED). Type 1 / 0xB / 0xE asks XAM for a different HUD load mode (XamAppRequestLoadEx); the
// flow is the same here.
GUEST_IMPORT(XamShowCreateProfileUIEx)
{
    const std::string initial = ctx.r5.u32 ? std::string(reinterpret_cast<const char*>(guest::g_memory.Translate(ctx.r5.u32)),
        strnlen(reinterpret_cast<const char*>(guest::g_memory.Translate(ctx.r5.u32)), 0x72)) : std::string();
    const bool shown = ShowCreateProfile(ctx.r3.u32, std::u16string(initial.begin(), initial.end()),
        u"Enter a gamertag for the new profile.");
    ctx.r3.u64 = shown ? kErrorSuccess : kErrorAccessDenied;
}

// DWORD XamShowCreateProfileUI(DWORD UserIndex, DWORD Type): XamShowCreateProfileUIEx without a gamertag (XAM
// 0x816C5A78 is `li r5, 0` and a branch to it).
GUEST_IMPORT(XamShowCreateProfileUI)
{
    ctx.r5.u64 = 0;
    XamShowCreateProfileUIEx_impl(ctx, base);
}

// DWORD XamShowSigninUI(DWORD PaneCount, DWORD Flags) (XShowSigninUI): the Guide's sign-in panes, shown
// asynchronously; ERROR_SUCCESS once open. The XDK: one-pane sign-in is additive (the profile picked is signed in to
// the controller that picked it, if it was not signed in and that controller was not in use; otherwise it moves or
// replaces); when sign-in completes and a signed-in player changed, XN_SYS_SIGNINCHANGED is sent with a bit per user
// index signed in by this sign-in (sent before XN_SYS_UI 0, as Xenia orders them); cancelling changes nothing.
// Every profile here is a local offline one (kernel/users.h): XSSUI_FLAGS_SHOWONLYONLINEENABLED lists none, and
// without a LIVE-enabled sponsor there is no guest option (XSSUI_FLAGS_ENABLE_GUEST). XSSUI_FLAGS_ADDUSER leaves out
// profiles already signed in. The pane is driven by any controller: A on a profile signs it in to that controller's
// user index (the native dialog's mouse answer: the controller that pressed a button last), B cancels.
// TU46 (0x82757AB8) asks for 1 pane with LOCALSIGNINONLY [| ADDUSER] [| DISALLOW_GUEST] for players joining, and
// for 2/4 panes only together with SHOWONLYONLINEENABLED; the cases it never uses stop with a message.
// Drawn as XAM's one-pane sign-in (signin1.xur): the eligible profiles, then "Create Profile" (closes the pane and
// starts XamShowCreateProfileUI's flow for that controller) and "Download Profile", which copies the new profiles of the `profiles` folder (kernel/profile_import.h; the console
// would recover one from Xbox LIVE) and lists them; a 2/4-pane request that lists no profile shows the same pane
// (deviation: the console would show signin2/4.xur). A profile signing in raises XAM's toast.
GUEST_IMPORT(XamShowSigninUI)
{
    const uint32_t panes = ctx.r3.u32, flags = ctx.r4.u32;
    constexpr uint32_t kShowOnlyOnlineEnabled = 0x00000002, kAllowSignout = 0x00000004, kAddUser = 0x00010000;
    constexpr uint32_t kConvertOfflineToGuest = 0x00400000; // XSSUI_FLAGS_*
    constexpr uint32_t kKnownFlags = 0x00000001 | kShowOnlyOnlineEnabled | kAllowSignout | 0x00000010 | kAddUser |
        0x00080000 | kConvertOfflineToGuest | 0x01000000;
    if (panes != 1 && panes != 2 && panes != 4)
    {
        ctx.r3.u64 = kErrorInvalidParameter;
        return;
    }
    if (flags & ~kKnownFlags)
        Fatal("XamShowSigninUI: unknown flags {:08X}", flags);
    if (flags & (kAllowSignout | kConvertOfflineToGuest))
        Fatal("XamShowSigninUI({}, {:08X}): in-place sign-out / converting to a guest is not implemented", panes, flags);

    // The profiles this sign-in may pick (again after Download Profile added some).
    auto eligible = std::make_shared<std::vector<size_t>>();
    auto list = [flags, eligible](Dialog& dialog) {
        const auto& profiles = kernel::users::Profiles();
        eligible->clear();
        if (!(flags & kShowOnlyOnlineEnabled))
            for (size_t i = 0; i < profiles.size(); i++)
                if (!(flags & kAddUser) || !kernel::users::UserOfXuid(profiles[i].xuid))
                    eligible->push_back(i);
        dialog.signIn.clear();
        dialog.buttons.clear();
        if (eligible->empty())
        {
            dialog.text = (flags & kShowOnlyOnlineEnabled) ?
                u"There is no gamer profile on this console that can sign in to Xbox Live." :
                u"Every gamer profile on this console is signed in.";
            dialog.buttons = { u"Back" };
        }
        else
            dialog.text = u"Select a gamer profile. It is signed in to the controller that selects it.";
        for (size_t i : *eligible)
        {
            // "Xbox Live" under a LIVE profile's gamertag (none for local ones); all live on the hard drive.
            dialog.signIn.push_back({ std::u16string(profiles[i].gamertag.begin(), profiles[i].gamertag.end()),
                                      profiles[i].liveEnabled ? u"Xbox Live" : u"", u"",
                                      kernel::profile::GamerPicture(profiles[i].xuid) });
            std::string label = profiles[i].gamertag;
            if (const auto user = kernel::users::UserOfXuid(profiles[i].xuid))
                label += std::format(" (signed in to controller {})", *user + 1);
            dialog.buttons.emplace_back(label.begin(), label.end());
        }
        dialog.choosable = eligible->size();
    };
    auto dialog = std::make_shared<Dialog>();
    dialog->kind = "sign-in UI";
    dialog->user = kUserIndexAny;
    dialog->title = u"Sign In";
    list(*dialog);
    if (panes != 1 && !eligible->empty())
        Fatal("XamShowSigninUI({}, {:08X}): sign-in with several panes is not implemented", panes, flags);
    dialog->complete = [eligible](uint32_t answer, uint32_t user) {
        if (answer >= eligible->size() || user >= kernel::users::kMaxUsers)
            return;
        const kernel::users::Profile profile = kernel::users::Profiles()[(*eligible)[answer]];
        if (kernel::users::Xuid(user) == profile.xuid)
            return; // already signed in to this controller: nothing changed
        kernel::users::SignIn(user, (*eligible)[answer]);
        kernel::BroadcastNotification(kXnSysSigninChanged, 1u << user);
        // XAM's toast for the sign-in: "<gamertag> signed in" for a local profile; an Xbox LIVE profile's once its LIVE
        // sign-in (users::SignIn's thread) has its answer: "signed in to Xbox Live", or "signed in offline" without a
        // server or when kept offline (up to 15 s).
        if (!profile.liveEnabled)
            return kernel::SignInToast(user, profile.gamertag, false, false);
        std::thread([user, gamertag = profile.gamertag] {
            for (int i = 0; i < 150 && !kernel::UserOnLive(user) && !live::Held(user); i++)
                Sleep(100);
            kernel::SignInToast(user, gamertag, true, kernel::UserOnLive(user));
        }).detach();
    };
    // Create Profile (the row after the profiles): the sign-in pane closes (nothing signed in by it) and the
    // create-profile flow starts for the controller that chose it, which signs the new profile in (XamShowCreateProfileUI).
    // Download Profile (the row after it): copies the `profiles` folder's new profiles and lists them.
    std::weak_ptr<Dialog> weak = dialog;
    dialog->onExtraRow = [weak, eligible, list](uint32_t row, uint32_t user) {
        auto d = weak.lock();
        if (!d || !d->hud)
            return;
        if (row == d->choosable)
        {
            Answer(d, kMessageBoxCancelId, std::format("the controller of user {} (Create Profile)", user), user);
            std::thread([user] {
                for (int i = 0; i < 100 && !ShowCreateProfile(user, u"", u"Enter a gamertag for the new profile."); i++)
                    Sleep(50);
            }).detach();
            return;
        }
        if (row != d->choosable + 1)
            return;
        const std::vector<uint64_t> added = guide::host::DownloadProfiles();
        list(*d);
        xui::ShowSignIn(d->signIn, RowOf(*eligible, added));
    };
    if (!Open(dialog))
    {
        ctx.r3.u64 = kErrorAccessDenied;
        return;
    }
    ctx.r3.u64 = kErrorSuccess;
}

// DWORD XamShowSigninUIp(DWORD UserIndex, DWORD PaneCount, DWORD Flags) (XAM 0x816C19C8): XAM's private sign-in UI.
// XAM requests an app load (ERROR_ACCESS_DENIED if one is already requested or a system UI is up) and loads
// signin.xex with {0, user, panes, flags, 0}; XN_SYS_UI 1 and XamIsUIActive while it runs, XN_SYS_UI 0 at its end;
// returns 0 once queued. signin.xex's flow factory (0x90106968) picks by flags; implemented here is the one the
// dash uses after signing a LIVE-enabled profile in locally (XamUserLogonEx 0x25, then (user, 1, 0x24030000)):
// flag 0x20000, signin's mode 0 (0x9010A178), the Xbox LIVE logon of the signed-in profiles (0x901183E0:
// XamUserLogonEx(users, 0x120420, ...)). This console has no network: XAM's logon fails at once with
// XONLINE_E_LOGON_NO_NETWORK_CONNECTION 0x80151000 (link down, XAM 0x81738xxx), and signin's handler for it
// (0x90119D70 -> 0x90119868) asks with XAM's message box (flags 0x200000: XAM's own Yes / No buttons, 0x81694058;
// error icon): title common string 0x128 "Xbox Live", text signin string 21 "Your console can't connect to Xbox
// Live. Do you want to exit this session and test your network connection? ...". No or B: the logon ends with E_ABORT
// and signin closes; the profile stays signed in locally (the dash then carries on). Yes (0x901167D8): signin
// unloads the app stack and relaunches the dashboard with launch data {2, 0xFE} (its network test): not implemented.
// Deviation: signin shows its "Signing in" status scene (status.xur) while the logon runs; nothing is drawn here
// meanwhile (the UI is active, the dash waits on its profile select screen). The 0x04000000 / 0x20000000 / 0x10000 flags' meaning is not established (accepted).
GUEST_IMPORT(XamShowSigninUIp)
{
    const uint32_t user = ctx.r3.u32, panes = ctx.r4.u32, flags = ctx.r5.u32;
    constexpr uint32_t kLiveLogon = 0x00020000;
    // Without the LIVE logon flag it is the ordinary sign-in: XamShowSigninUI(panes, flags) is XAM's
    // XamShowSigninUIp(0xFF, panes, flags) (0x816C3CD8). The 6719 dash calls (0, 1, 0) from its Sign In item.
    if (!(flags & kLiveLogon) && (user < kernel::users::kMaxUsers || user == 0xFF))
    {
        ctx.r3.u64 = panes;
        ctx.r4.u64 = flags;
        XamShowSigninUI_impl(ctx, base);
        return;
    }
    constexpr uint32_t kKnownFlags = kLiveLogon | 0x00010000 | 0x04000000 | 0x20000000;
    if (user >= kernel::users::kMaxUsers || panes != 1 || !(flags & kLiveLogon) || (flags & ~kKnownFlags))
        Fatal("XamShowSigninUIp({}, {}, {:08X}): only the Xbox LIVE logon of a signed-in profile (one pane, flag "
            "0x20000) is implemented", user, panes, flags);
    const auto profile = kernel::users::SignedIn(user);
    if (!profile || !profile->liveEnabled)
        Fatal("XamShowSigninUIp({}, 1, {:08X}): user {} is not signed in with a LIVE-enabled profile", user, flags,
            user);

    // signin.xex is an app XAM loads: XamShowSigninUIp only queues it (returns at once) and the UI is active from
    // then on (XN_SYS_UI 1); its "Signing in" status scene runs the logon. The dash (FUN_928B45B8) polls XamIsUIActive
    // until it comes and goes before it moves on from the profile select screen. The logon runs on a thread here:
    // waiting for the server on the dash's thread (no server: the connect timeout) let another dash thread set the
    // flag that ends that wait, and the dash went to its hub under the box.
    UiBusy();
    std::thread([user, profile = *profile] {
        // Our Xbox LIVE (kernel/xam_live.cpp) has the profile: the logon succeeds; XAM's logon commit sends
        // XN_LIVE_CONNECTIONCHANGED (CONNECTION_ESTABLISHED) and XN_SYS_SIGNINCHANGED, and signin closes.
        if (!kernel::UserOnLive(user))
            kernel::users::LiveSignIn(user, profile);
        if (kernel::UserOnLive(user))
        {
            Log("XamShowSigninUIp: Xbox LIVE logon of {} (user {}) succeeded", profile.gamertag, user);
            kernel::LiveLogonCommitted(user);
            kernel::SignInToast(user, profile.gamertag, true, true);
            kernel::BroadcastNotification(0x02000001, 0x001510F0); // XN_LIVE_CONNECTIONCHANGED
            kernel::BroadcastNotification(kXnSysSigninChanged, kernel::users::SignedInMask());
            UiIdle();
            return;
        }

        static constexpr uint32_t kNoNetworkConnection = 0x80151000; // XONLINE_E_LOGON_NO_NETWORK_CONNECTION
        auto dialog = std::make_shared<Dialog>();
        dialog->kind = "Xbox Live sign-in";
        dialog->user = user;
        dialog->title = xui::CommonString(0x128, u"Xbox Live");
        dialog->text = xui::SigninString(21, u"Your console can't connect to Xbox Live. Do you want to exit this "
            u"session and test your network connection? If you are playing a game, you will lose any unsaved progress.");
        dialog->buttons = { xui::CommonString(0x12A, u"Yes"), xui::CommonString(0x78, u"No") };
        dialog->icon = xui::MessageIcon::kError;
        dialog->complete = [user, gamertag = profile.gamertag](uint32_t answer, uint32_t) {
            if (answer == 0)
            {
                // Yes (signin 0x901167D8): the dashboard again with the request {+4 command 2, +8 user 0xFE}, its
                // network test (XamLoaderSetLaunchData, then XamLoaderLaunchTitleEx(NULL)); the profile stays signed
                // in locally (offline).
                Log("XamShowSigninUIp: Yes: the dashboard's network test");
                std::vector<uint8_t> request(0x3FC, 0);
                request[7] = 2;
                request[11] = 0xFE;
                kernel::RelaunchDashboard(&request);
            }
            Log("XamShowSigninUIp: Xbox LIVE logon of {} (user {}) failed with {:08X} (no network); {} stays signed "
                "in locally", gamertag, user, kNoNetworkConnection, gamertag);
            // The title's logon is over: a later sign-in (the Guide's Connect to Xbox LIVE) is announced to it
            // (XN_LIVE_CONNECTIONCHANGED, kernel/xam_live.cpp).
            kernel::LiveLogonCommitted(user);
            kernel::SignInToast(user, gamertag, true, false);
        };
        // The box keeps the UI active until it is answered (and its closing animation is over).
        if (!Open(dialog))
            Log("XamShowSigninUIp: Xbox LIVE logon of {} failed and another system UI is open: no box",
                profile.gamertag);
        UiIdle();
    }).detach();
    ctx.r3.u64 = kErrorSuccess;
}

void kernel::SignInToast(uint32_t user, const std::string& gamertag, bool liveProfile, bool online)
{
    xui::QueueNotification(!liveProfile ? xui::kNotifySignedIn : online ? xui::kNotifySignedInLive
                                                                        : xui::kNotifySignedInOffline,
                           user, std::u16string(gamertag.begin(), gamertag.end()));
}

// DWORD XamShowAchievementsUI(DWORD UserIndex, DWORD TitleId) (XShowAchievementsUI; TitleId 0 = this title): the
// Guide's achievement list for the user, shown asynchronously; ERROR_SUCCESS once open, XN_SYS_UI 0 when closed.
// Earned ones show label, description and gamerscore; unearned ones the "unachieved" text, or "Secret Achievement"
// unless the SPA marks them XACHIEVEMENT_DETAILS_SHOWUNACHIEVED. Drawn as gamerprofile.xex's achievement grid
// (xui::ShowAchievements); A on a tile does nothing (the console opens the achievement's details).
GUEST_IMPORT(XamShowAchievementsUI)
{
    const uint32_t user = ctx.r3.u32, titleId = ctx.r4.u32;
    constexpr uint32_t kErrorNoSuchUser = 0x525;
    constexpr uint32_t kShowUnachieved = 0x8, kAchieved = 0x20000; // XACHIEVEMENT_DETAILS_*
    if (user >= 4 || !kernel::UserSignedIn(user))
    {
        ctx.r3.u64 = kErrorNoSuchUser;
        return;
    }
    if (titleId != 0 && titleId != kernel::TitleId())
        Fatal("XamShowAchievementsUI: achievements of title {:08X}", titleId);

    std::u16string list;
    std::vector<xui::AchievementEntry> achievementEntries;
    uint32_t earnedCount = 0, earnedScore = 0, totalScore = 0;
    const auto& achievements = kernel::spa::Achievements();
    for (const kernel::spa::Achievement& a : achievements)
    {
        totalScore += a.gamerscore;
        const auto record = kernel::profile::ReadAchievement(kernel::users::Xuid(user), kernel::TitleId(), a.id);
        const bool earned = record && (record->flags & kAchieved);
        xui::AchievementEntry entry;
        entry.label = kernel::spa::String(a.labelId);
        entry.description = kernel::spa::String(a.descriptionId);
        entry.unachieved = kernel::spa::String(a.unachievedId);
        entry.gamerscore = a.gamerscore;
        entry.earned = earned;
        entry.showUnachieved = (a.flags & kShowUnachieved) != 0;
        entry.unlockTime = earned ? record->unlockTime : 0;
        const auto image = kernel::spa::Image(a.imageId);
        entry.image.assign(image.begin(), image.end());
        achievementEntries.push_back(std::move(entry));
        const std::string score = std::format(" ({}G)", a.gamerscore);
        list += u"\n";
        if (earned)
        {
            earnedCount++;
            earnedScore += a.gamerscore;
            list += u"[x] " + kernel::spa::String(a.labelId) + std::u16string(score.begin(), score.end()) + u": " +
                kernel::spa::String(a.descriptionId);
        }
        else if (a.flags & kShowUnachieved)
            list += u"[ ] " + kernel::spa::String(a.labelId) + std::u16string(score.begin(), score.end()) + u": " +
                kernel::spa::String(a.unachievedId);
        else
            list += u"[ ] Secret Achievement" + std::u16string(score.begin(), score.end());
    }
    const std::string summary = std::format("{} of {} achievements unlocked, {} of {} gamerscore", earnedCount,
        achievements.size(), earnedScore, totalScore);

    auto dialog = std::make_shared<Dialog>();
    dialog->kind = "achievements UI";
    dialog->user = user;
    dialog->title = kernel::spa::TitleName() + u" Achievements";
    dialog->text = std::u16string(summary.begin(), summary.end()) + list;
    dialog->buttons = { u"Back" };
    dialog->achievements = std::move(achievementEntries);
    dialog->complete = [](uint32_t, uint32_t) {};
    if (!Open(dialog))
    {
        ctx.r3.u64 = kErrorAccessDenied;
        return;
    }
    ctx.r3.u64 = kErrorSuccess;
}

// DWORD XamShowMessagesUI(DWORD UserIndex) (XAM 0x816C3DE8 -> app 2 message 0x21002, flags 1): the Guide's messages.
GUEST_IMPORT(XamShowMessagesUI)
{
    ctx.r3.u64 = guide::ShowCommunity(ctx.r3.u32, guide::Tab::kMessages);
}

// DWORD XamShowPartyUI(DWORD UserIndex) (XAM 0x816C3EF8): the Guide's party screen.
GUEST_IMPORT(XamShowPartyUI)
{
    ctx.r3.u64 = guide::ShowCommunity(ctx.r3.u32, guide::Tab::kParty);
}

// DWORD XamShowPartyInviteUI(DWORD UserIndex, ...): choosing friends to invite: the Guide's friends list (Invite to
// Party is a friend's action there).
GUEST_IMPORT(XamShowPartyInviteUI)
{
    ctx.r3.u64 = guide::ShowCommunity(ctx.r3.u32, guide::Tab::kFriends);
}

// DWORD XamShowFriendRequestUI(DWORD UserIndex, XUID Xuid) (the dash's Lua ShowAddFriendUI passes 0): with no XUID the
// Guide's Add Friend (gamertag on the keyboard); with one, a friend request to that player after the console's
// question.
// DWORD XamShowGamerCardUIForXUID(DWORD UserIndex, XUID XuidPlayer) (XShowGamerCardUI): the player's Profile over
// the title (GuideProfile; B closes it). The user must be on Xbox LIVE (ShowCommunityUI). The user's own XUIDs (or
// 0) show their own Profile, as XAM's XamShowGamerCardUIForXUIDp (0x816C4290) hands those to XamShowGamerCardUI.
GUEST_IMPORT(XamShowGamerCardUIForXUID)
{
    const uint32_t user = ctx.r3.u32;
    const uint64_t xuid = ctx.r4.u64;
    Log("XamShowGamerCardUIForXUID(user {}, {:016X})", user, xuid);
    ctx.r3.u64 = guide::ShowGamerCard(user, xuid);
}

// DWORD XamShowGamerCardUI(DWORD UserIndex) (XAM 0x816C40F0: message 0x21008 to the HUD, gamerprofile.xex's
// GamerCardScene for the signed-in user; hud.xex's Settings > Profile calls it too): the user's own Profile over the
// title, on Xbox LIVE or not (GuideOwnProfile; B closes it).
GUEST_IMPORT(XamShowGamerCardUI)
{
    const uint32_t user = ctx.r3.u32;
    Log("XamShowGamerCardUI(user {})", user);
    ctx.r3.u64 = guide::ShowOwnProfile(user);
}

// DWORD XamShowGamesUI(DWORD UserIndex, XUID XuidPlayer) (XAM: gamerprofile.xex's GameShowcaseMeScene, or
// GameShowcaseYouScene for another player): the user's played games, or Compare Games with that player.
GUEST_IMPORT(XamShowGamesUI)
{
    const uint32_t user = ctx.r3.u32;
    const uint64_t xuid = ctx.r4.u64;
    Log("XamShowGamesUI(user {}, {:016X})", user, xuid);
    ctx.r3.u64 = guide::ShowGames(user, xuid);
}

// DWORD XamShowPlayerReviewUI(DWORD UserIndex, XUID XuidFeedbackTarget) / XamShowComplaintUI (same arguments): XAM
// (0x816C23D8 / 0x816C2450) sends message 0x21009 to the HUD, mode 0 (review) or 1 (complaint): Submit Player Review
// or File Complaint about that player.
GUEST_IMPORT(XamShowPlayerReviewUI)
{
    Log("XamShowPlayerReviewUI(user {}, {:016X})", ctx.r3.u32, ctx.r4.u64);
    ctx.r3.u64 = guide::ShowFeedback(ctx.r3.u32, ctx.r4.u64, false);
}

GUEST_IMPORT(XamShowComplaintUI)
{
    Log("XamShowComplaintUI(user {}, {:016X})", ctx.r3.u32, ctx.r4.u64);
    ctx.r3.u64 = guide::ShowFeedback(ctx.r3.u32, ctx.r4.u64, true);
}

// DWORD XamShowPasscodeVerifyUIEx(DWORD UserIndex, PMESSAGEBOX_RESULT Result, DWORD Flags, PXOVERLAPPED Overlapped)
// (XAM 0x816C5EB8; XamShowPasscodeVerifyUI passes flags 0x20000): the console's pass code (XConfig 3/0x17, four
// XONLINE_PASSCODE_TYPE bytes, XAM 0x816E0798: 1 X, 2 Y, 3 left, 4 right, 5 up, 6 down, 9 LT, 10 RT, 11 LB, 12 RB) as
// VK_PAD_* codes into Result, then the message box in verify mode with common string 0x87 "Enter pass code.". With no
// pass code set (XAM shows nothing and leaves the request pending) the request completes at once, answered 0.
namespace
{
    uint32_t ShowPasscodeVerify(uint32_t user, uint32_t result, uint32_t flags, uint32_t overlapped)
    {
        static const uint16_t kVk[13] = { 0, 0x5802, 0x5803, 0x5812, 0x5813, 0x5810, 0x5811, 0, 0, 0x5806, 0x5807,
                                          0x5805, 0x5804 };
        const std::vector<uint8_t> code = kernel::XConfigRead(3, 0x17);
        bool set = code.size() >= 4;
        for (size_t i = 0; set && i < 4; i++)
            set = code[i] < 13 && kVk[code[i]] != 0;
        if (!set)
        {
            Log("XamShowPasscodeVerifyUI(user {}): no console pass code set: verified", user);
            guest::g_memory.Write32(result, 0);
            kernel::BeginOverlapped(overlapped);
            kernel::CompleteOverlapped(overlapped, kErrorSuccess, 0, 0);
            return kernel::kErrorIoPending;
        }
        for (uint32_t i = 0; i < 4; i++)
            guest::g_memory.Write16(result + i * 2, kVk[code[i]]);
        return ShowPasscodeBox(user, u"", xui::CommonString(0x87, u"Enter pass code."),
                               flags | kMessageBoxVerifyPasscodeMode, result, overlapped);
    }
}

GUEST_IMPORT(XamShowPasscodeVerifyUI)
{
    Log("XamShowPasscodeVerifyUI(user {})", ctx.r3.u32);
    ctx.r3.u64 = ShowPasscodeVerify(ctx.r3.u32, ctx.r4.u32, kMessageBoxVerifyPasscodeMode, ctx.r5.u32);
}

GUEST_IMPORT(XamShowPasscodeVerifyUIEx)
{
    Log("XamShowPasscodeVerifyUIEx(user {}, flags {:08X})", ctx.r3.u32, ctx.r5.u32);
    ctx.r3.u64 = ShowPasscodeVerify(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
}

namespace
{
    uint32_t ShowServiceUnavailable(uint32_t user, uint32_t overlapped, const char* name);
    uint32_t LiveSignup(uint32_t user);
    uint32_t Marketplace(uint32_t user, uint32_t mode, const char* name);
}

void kernel::ShowSystemScreenForTest(uint32_t user, const std::string& screen)
{
    const size_t slash = screen.find('/');
    const std::string name = screen.substr(0, slash);
    const uint64_t arg = slash == std::string::npos ? 0 : std::strtoull(screen.c_str() + slash + 1, nullptr, 16);
    uint32_t result = 0;
    if (name == "games")
        result = guide::ShowGames(user, arg);
    else if (name == "gamercard")
        result = guide::ShowGamerCard(user, arg);
    else if (name == "profile")
        result = guide::ShowOwnProfile(user);
    else if (name == "passcode")
    {
        // A verify-mode pass code box expecting <hex>'s four XONLINE_PASSCODE_TYPE bytes (high byte first), with a
        // result and request in kernel pool memory (kept: the box writes them when answered).
        static const uint16_t kVk[13] = { 0, 0x5802, 0x5803, 0x5812, 0x5813, 0x5810, 0x5811, 0, 0, 0x5806, 0x5807,
                                          0x5805, 0x5804 };
        const uint32_t block = KernelPool().Allocate(0x40);
        for (uint32_t i = 0; i < 4; i++)
            guest::g_memory.Write16(block + i * 2, kVk[std::min<uint32_t>(uint32_t(arg >> (24 - 8 * i)) & 0xFF, 12)]);
        result = ShowPasscodeBox(user, u"", xui::CommonString(0x87, u"Enter pass code."), kMessageBoxVerifyPasscodeMode,
                                 block, block + 0x10);
    }
    else if (name == "mediaupdate")
    {
        PPCContext ctx{};
        ctx.r3.u64 = user;
        __imp__XamShowOptionalMediaUpdateRequiredUI(ctx, guest::g_memory.base);
        result = ctx.r3.u32;
    }
    else if (name == "fof")
        result = guide::ShowFriendsOf(user, arg, "");
    else if (name == "joinsession")
        result = guide::ShowJoinSessionInProgress(user, arg);
    else if (name == "kinect")
        result = ShowNuiTroubleshooter(user);
    else if (name == "createprofile")
        result = ShowCreateProfile(user, u"", u"Enter a gamertag for the new profile.") ? 0 : kErrorAccessDenied;
    else if (name == "options")
        result = guide::ShowPreferences(user);
    else if (name == "review" || name == "complaint")
        result = guide::ShowFeedback(user, arg, name == "complaint");
    else if (name == "signup")
        result = LiveSignup(user);
    else if (name == "marketplace" || name == "namechange" || name == "payment" || name == "terms")
        result = name == "marketplace" ? Marketplace(user, uint32_t(arg), "SHOW=marketplace")
                                       : Marketplace(user, name == "namechange" ? 8 : name == "payment" ? 0x13 : 0x10,
                                                     "SHOW");
    else if (name == "gold" || name == "tfa" || name == "beacons" || name == "socialpost" || name == "avatar" ||
             name == "csv")
        result = ShowServiceUnavailable(user, 0, ("SHOW=" + name).c_str());
    else if (name == "videochat")
        result = ShowNuiTroubleshooter(user);
    else
    {
        Log("SHOW={}: no such screen", screen);
        return;
    }
    Log("SHOW={} (user {}): {:08X}", screen, user, result);
}

GUEST_IMPORT(XamShowFriendRequestUI)
{
    ctx.r3.u64 = guide::ShowFriendRequest(ctx.r3.u32, ctx.r4.u64);
}

// DWORD XamShowMessageComposeUI(DWORD UserIndex, const XUID* Recipients, DWORD RecipientCount, LPCWSTR Text): a text
// message to the recipients (the keyboard, prefilled with Text).
GUEST_IMPORT(XamShowMessageComposeUI)
{
    auto& mem = guest::g_memory;
    const uint32_t user = ctx.r3.u32, recipientsPtr = ctx.r4.u32, count = ctx.r5.u32;
    std::vector<uint64_t> recipients;
    for (uint32_t i = 0; i < count && i < 100 && recipientsPtr; i++)
        recipients.push_back(mem.Read64(recipientsPtr + i * 8));
    ctx.r3.u64 = guide::ShowMessageCompose(user, recipients);
}

namespace kernel
{
    bool ShowDeviceSelectorUi(uint32_t user, const std::vector<DeviceChoice>& devices,
                              std::function<void(int row)> done)
    {
        if (!xui::SystemUiAvailable())
            return false;
        auto dialog = std::make_shared<Dialog>();
        dialog->kind = "device selector";
        dialog->user = user < 4 ? user : 0;
        dialog->title = u"Select a Device";
        for (const DeviceChoice& d : devices)
            dialog->devices.push_back({ d.name, d.kind, d.freeBytes, d.totalBytes });
        dialog->choosable = devices.size();
        dialog->complete = [done](uint32_t answer, uint32_t) {
            done(answer == kMessageBoxCancelId ? -1 : int(answer));
        };
        return Open(dialog);
    }
}

// ---- Xbox LIVE services the ports' own LIVE does not have (stores, purchases, social networks: out of scope) ----
// XAM sends these to the HUD (messages 0x2100A marketplace, 0x2100E sign-up, 0x21018 avatar creator, 0x21037 social
// post, 0x2103E beacons, 0x21041 two-factor, 0x21042 Kinect CSV transition, 0x21002 Friends UI for a video chat) after
// its own checks, which are kept here with their results. Where the service does not exist, the screen is XAM's own
// "not available" box (common strings 0x1E "Not Available" and 4 "This feature is temporarily unavailable. Please try
// again later.", error icon), answered with the controller like any message box; the call returns as XAM's does once
// the HUD has the request (ERROR_SUCCESS, or ERROR_IO_PENDING with an overlapped completed when the box closes).

namespace
{
    constexpr uint32_t kErrorFunctionFailed = 0x65B;

    uint32_t ShowServiceUnavailable(uint32_t user, uint32_t overlapped, const char* name)
    {
        Log("{}(user {}): this LIVE service is not on the ports' own LIVE: \"Not Available\" box", name, user);
        auto dialog = std::make_shared<Dialog>();
        dialog->kind = "message box";
        dialog->user = user < 4 ? user : 0;
        dialog->title = xui::CommonString(0x1E, u"Not Available");
        dialog->text = xui::CommonString(4, u"This feature is temporarily unavailable. Please try again later.");
        dialog->buttons = { xui::CommonString(0x79, u"OK") };
        dialog->icon = xui::MessageIcon::kError;
        dialog->overlapped = overlapped;
        dialog->complete = [overlapped](uint32_t, uint32_t) {
            if (overlapped)
                kernel::CompleteOverlapped(overlapped, kErrorSuccess, 0, 0);
        };
        if (overlapped)
            kernel::BeginOverlapped(overlapped);
        if (!Open(dialog))
        {
            if (overlapped)
                kernel::CompleteOverlapped(overlapped, kErrorAccessDenied, 0, 0);
            return kErrorAccessDenied;
        }
        return overlapped ? kernel::kErrorIoPending : kErrorSuccess;
    }

    bool OnlineEnabled(uint32_t user)
    {
        const auto profile = user < kernel::users::kMaxUsers ? kernel::users::SignedIn(user) : std::nullopt;
        return profile && profile->liveEnabled;
    }

    // XAM 0x816C1718 (Beacons, social post, video chat): only for a user signed in to LIVE (any signed-in user for
    // XUSER_INDEX_ANY); otherwise nothing is sent.
    bool OnLiveForHud(uint32_t user)
    {
        if (user == kUserIndexAny)
        {
            for (uint32_t u = 0; u < kernel::users::kMaxUsers; u++)
                if (kernel::UserOnLive(u))
                    return true;
            return false;
        }
        return user < kernel::users::kMaxUsers && kernel::UserOnLive(user);
    }

    // XamShowLiveSignupUI's sign-up (HUD message 0x2100E): with nobody signed in, a new profile (the create-profile
    // flow, which signs it in and so to LIVE); a signed-in profile not on LIVE gets its account and is signed in
    // (live::Reconnect, which also makes a missing account), with XAM's sign-in toast, or "Can't Connect" (common
    // strings 0x55, 0x5E) when the server does not answer. A profile on LIVE already: nothing to sign up for.
    uint32_t LiveSignup(uint32_t user)
    {
        const auto profile = user < kernel::users::kMaxUsers ? kernel::users::SignedIn(user) : std::nullopt;
        if (!profile)
            return ShowCreateProfile(user < 4 ? user : 0, u"", u"Enter a gamertag for the new profile.") ? kErrorSuccess
                                                                                                        : kErrorAccessDenied;
        if (kernel::UserOnLive(user))
        {
            Log("XamShowLiveSignupUI: {} is on Xbox LIVE already", profile->gamertag);
            return kErrorSuccess;
        }
        std::thread([user, gamertag = profile->gamertag] {
            if (const auto error = live::Reconnect(user))
                Log("XamShowLiveSignupUI: {}: {}", gamertag, *error);
            for (int i = 0; i < 150 && !kernel::UserOnLive(user); i++)
                Sleep(100);
            if (kernel::UserOnLive(user))
                return kernel::SignInToast(user, gamertag, true, true);
            auto dialog = std::make_shared<Dialog>();
            dialog->kind = "message box";
            dialog->user = user;
            dialog->title = xui::CommonString(0x55, u"Can't Connect");
            dialog->text = xui::CommonString(0x5E, u"A problem occurred while signing in. Please try again.");
            dialog->buttons = { xui::CommonString(0x79, u"OK") };
            dialog->icon = xui::MessageIcon::kError;
            dialog->complete = [](uint32_t, uint32_t) {};
            for (int i = 0; i < 100 && !Open(dialog); i++)
                Sleep(100); // after any box still on screen
        }).detach();
        return kErrorSuccess;
    }

    // XamShowMarketplaceUIEx's checks (XAM 0x816C6168): a user index below 4 and a mode below 0x19.
    uint32_t Marketplace(uint32_t user, uint32_t mode, const char* name)
    {
        if (user >= 4 || mode >= 0x19)
            return kErrorInvalidParameter;
        return ShowServiceUnavailable(user, 0, name);
    }
}

// DWORD XamShowLiveSignupUI(DWORD UserIndex, BOOL Flag) (XAM 0x816C3628: sign-up type Flag ? 2 : 1).
GUEST_IMPORT(XamShowLiveSignupUI)
{
    Log("XamShowLiveSignupUI(user {}, flag {})", ctx.r3.u32, ctx.r4.u32);
    ctx.r3.u64 = LiveSignup(ctx.r3.u32);
}

// DWORD XamShowGoldUpgradeUI(DWORD UserIndex, DWORD Reason, DWORD Context) (XAM 0x816C6240): nothing for a user who is
// not an Xbox LIVE account, nor for one who has Gold already (privilege 0xFD: every account on the ports' own LIVE);
// the Gold offer (marketplace mode 0xF) is not available otherwise.
GUEST_IMPORT(XamShowGoldUpgradeUI)
{
    const uint32_t user = ctx.r3.u32;
    Log("XamShowGoldUpgradeUI(user {}, reason {}, context {:08X})", user, ctx.r4.u32, ctx.r5.u32);
    if (!OnlineEnabled(user) || kernel::UserOnLive(user))
    {
        ctx.r3.u64 = kErrorSuccess;
        return;
    }
    ctx.r3.u64 = Marketplace(user, 0xF, "XamShowGoldUpgradeUI");
}

// DWORD XamShowMarketplaceUIEx(DWORD UserIndex, DWORD Mode, ULONGLONG OfferId, DWORD ContentCategories, ...,
// DWORD TitleId) (XAM 0x816C6168 -> 0x2100A).
GUEST_IMPORT(XamShowMarketplaceUIEx)
{
    Log("XamShowMarketplaceUIEx(user {}, mode {:X}, offer {:016X})", ctx.r3.u32, ctx.r4.u32, ctx.r5.u64);
    ctx.r3.u64 = Marketplace(ctx.r3.u32, ctx.r4.u32, "XamShowMarketplaceUIEx");
}

// XamShowForcedNameChangeUI(DWORD UserIndex) (XAM 0x816C6220): XamShowMarketplaceUIEx mode 8 (the paid gamertag change).
GUEST_IMPORT(XamShowForcedNameChangeUI)
{
    ctx.r3.u64 = Marketplace(ctx.r3.u32, 8, "XamShowForcedNameChangeUI");
}

// XamShowPaymentOptionsUI(DWORD UserIndex, BYTE, PVOID, DWORD) (XAM 0x816C4FE8: marketplace mode 0x13): an Xbox LIVE
// account only.
GUEST_IMPORT(XamShowPaymentOptionsUI)
{
    const uint32_t user = ctx.r3.u32;
    ctx.r3.u64 = OnlineEnabled(user) ? Marketplace(user, 0x13, "XamShowPaymentOptionsUI") : kErrorFunctionFailed;
}

// XamShowTermsOfUseUI(DWORD UserIndex, BOOL Flag) (XAM 0x816C5078: marketplace mode Flag ? 0x18 : 0x10): an Xbox LIVE
// account only, else ERROR_INVALID_PARAMETER.
GUEST_IMPORT(XamShowTermsOfUseUI)
{
    const uint32_t user = ctx.r3.u32;
    ctx.r3.u64 = OnlineEnabled(user) ? Marketplace(user, ctx.r4.u32 ? 0x18 : 0x10, "XamShowTermsOfUseUI")
                                     : kErrorInvalidParameter;
}

// DWORD XamShowTFAUI(DWORD UserIndex, DWORD Type, LPCWSTR Text, PBYTE Data, DWORD, DWORD, PXOVERLAPPED Overlapped)
// (XAM 0x816C37D0 -> 0x21041): LIVE's two-factor verification.
GUEST_IMPORT(XamShowTFAUI)
{
    ctx.r3.u64 = ShowServiceUnavailable(ctx.r3.u32, ctx.r9.u32, "XamShowTFAUI");
}

// XamShowCsvTransitionUI(DWORD TrackingId, DWORD UserIndex, BOOL Flag, DWORD) (XAM 0x816C5D98 -> 0x21042): Flag 0 is
// the Kinect path, which needs the Nui HUD: without a sensor, XAM's own "Update Required" box (common strings 0x11F,
// 0x4F) and nothing sent.
GUEST_IMPORT(XamShowCsvTransitionUI)
{
    const uint32_t user = ctx.r4.u32;
    Log("XamShowCsvTransitionUI(tracking {:08X}, user {}, flag {})", ctx.r3.u32, user, ctx.r5.u32);
    if (ctx.r5.u32 == 0)
    {
        ShowNuiTroubleshooter(user);
        ctx.r3.u64 = kErrorSuccess;
        return;
    }
    ctx.r3.u64 = ShowServiceUnavailable(user, 0, "XamShowCsvTransitionUI");
}

// DWORD XamShowBeaconsUI(DWORD UserIndex, LPCSTR Text) (XAM 0x816C1C60 -> 0x2103E): a user signed in to LIVE only.
GUEST_IMPORT(XamShowBeaconsUI)
{
    const uint32_t user = ctx.r3.u32;
    ctx.r3.u64 = OnLiveForHud(user) ? ShowServiceUnavailable(user, 0, "XamShowBeaconsUI") : kErrorFunctionFailed;
}

// DWORD XamShowSocialPostUI(PXSOCIAL_POST_PARAMS) (XAM 0x816C1AC0 -> 0x21037): ERROR_INVALID_PARAMETER without the
// block, its data (+0x14) or a size (+0x18) of 1-0x1FD8 bytes, a user (+4) below 4 or the +0x24 pointer; then a user
// signed in to LIVE only. Overlapped at +0x1C.
GUEST_IMPORT(XamShowSocialPostUI)
{
    auto& mem = guest::g_memory;
    const uint32_t params = ctx.r3.u32;
    if (!params || !mem.Read32(params + 0x14) || mem.Read32(params + 0x18) == 0 || mem.Read32(params + 0x18) > 0x1FD8 ||
        mem.Read32(params + 4) > 3 || !mem.Read32(params + 0x24))
    {
        ctx.r3.u64 = kErrorInvalidParameter;
        return;
    }
    const uint32_t user = mem.Read32(params + 4), overlapped = mem.Read32(params + 0x1C);
    if (!OnLiveForHud(user))
    {
        ctx.r3.u64 = kErrorFunctionFailed;
        return;
    }
    ctx.r3.u64 = ShowServiceUnavailable(user, overlapped, "XamShowSocialPostUI");
}

// DWORD XamShowVideoChatInviteUI(DWORD UserIndex, const XUID* Xuid) (XAM 0x816C3060: the Friends UI, flags 0x1020):
// Video Kinect needs the sensor: XAM's Kinect box. A user signed in to LIVE only.
GUEST_IMPORT(XamShowVideoChatInviteUI)
{
    const uint32_t user = ctx.r3.u32;
    Log("XamShowVideoChatInviteUI(user {})", user);
    ctx.r3.u64 = OnLiveForHud(user) ? ShowNuiTroubleshooter(user) : kErrorFunctionFailed;
}

// DWORD XamShowAvatarMiniCreatorUI(DWORD UserIndex, DWORD) (XAM 0x816C45F8 -> 0x21018): the avatar editor (avatars
// are not drawn by the ports: no avatar runtime).
GUEST_IMPORT(XamShowAvatarMiniCreatorUI)
{
    ctx.r3.u64 = ShowServiceUnavailable(ctx.r3.u32, 0, "XamShowAvatarMiniCreatorUI");
}
