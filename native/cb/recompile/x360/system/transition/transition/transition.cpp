#include "transition/transition.h"

#include "log.h"

#include <windows.h>

#include <atomic>
#include <format>
#include <mutex>
#include <thread>

namespace transition
{
    namespace
    {
        constexpr const wchar_t* kEventVariable = L"XBOX360RECOMP_READY_EVENT";
        constexpr const wchar_t* kCoverClass = L"Xbox360RecompTransitionCover";

        std::mutex g_mutex;
        HWND g_cover = nullptr;
        std::thread g_coverThread;

        LRESULT CALLBACK CoverProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
        {
            switch (message)
            {
            case WM_SETCURSOR:
                SetCursor(nullptr); // no mouse pointer over the black screen
                return TRUE;
            case WM_ERASEBKGND:
            {
                RECT rect;
                GetClientRect(hwnd, &rect);
                FillRect(reinterpret_cast<HDC>(wParam), &rect, static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));
                return 1;
            }
            case WM_CLOSE:
                DestroyWindow(hwnd);
                return 0;
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
            }
            return DefWindowProcW(hwnd, message, wParam, lParam);
        }

        // The monitor to cover: the one this process' visible window is on, else the primary one.
        RECT CoverRect()
        {
            HWND own = nullptr;
            EnumWindows([](HWND hwnd, LPARAM out) -> BOOL {
                DWORD pid = 0;
                GetWindowThreadProcessId(hwnd, &pid);
                if (pid == GetCurrentProcessId() && IsWindowVisible(hwnd))
                {
                    *reinterpret_cast<HWND*>(out) = hwnd;
                    return FALSE;
                }
                return TRUE;
            }, reinterpret_cast<LPARAM>(&own));
            MONITORINFO monitor{ sizeof(monitor) };
            GetMonitorInfoW(own ? MonitorFromWindow(own, MONITOR_DEFAULTTOPRIMARY)
                                : MonitorFromPoint({ 0, 0 }, MONITOR_DEFAULTTOPRIMARY), &monitor);
            return monitor.rcMonitor;
        }
    }

    void CoverScreen()
    {
        std::lock_guard lock(g_mutex);
        if (g_cover)
            return;
        // A process drawing inside a window host's window (XBOX360RECOMP_HOST_WINDOW: the dashboard port's
        // window_host.h): the host's window stays on the screen, black behind the titles' views, and is the cover.
        if (GetEnvironmentVariableW(L"XBOX360RECOMP_HOST_WINDOW", nullptr, 0) != 0)
            return;
        const RECT rect = CoverRect();
        std::atomic<HWND> created = nullptr;
        std::atomic<bool> done = false;
        g_coverThread = std::thread([rect, &created, &done] {
            WNDCLASSEXW wc{ sizeof(wc) };
            wc.lpfnWndProc = CoverProc;
            wc.hInstance = GetModuleHandleW(nullptr);
            wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
            wc.lpszClassName = kCoverClass;
            RegisterClassExW(&wc);
            HWND hwnd = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TOOLWINDOW, kCoverClass, L"", WS_POPUP, rect.left,
                                        rect.top, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr,
                                        wc.hInstance, nullptr);
            if (hwnd)
            {
                ShowWindow(hwnd, SW_SHOW);
                UpdateWindow(hwnd);
            }
            created = hwnd;
            done = true;
            if (!hwnd)
                return;
            MSG msg;
            while (GetMessageW(&msg, nullptr, 0, 0) > 0)
            {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
        });
        while (!done)
            std::this_thread::yield();
        g_cover = created;
        if (!g_cover)
        {
            g_coverThread.join();
            Log("Transition: the black cover window could not be created (error {})", GetLastError());
            return;
        }
        // Painted before anything else changes on the screen.
        RedrawWindow(g_cover, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW);
        GdiFlush();
    }

    void Uncover()
    {
        std::lock_guard lock(g_mutex);
        if (!g_cover)
            return;
        PostMessageW(g_cover, WM_CLOSE, 0, 0);
        g_coverThread.join();
        g_cover = nullptr;
    }

    Result Launch(const std::wstring& executable, const std::wstring& directory, uint32_t creationFlags,
                  std::chrono::milliseconds timeout, uint32_t* processId, uint32_t* error,
                  const std::wstring& arguments)
    {
        // A fresh name per launch; the child inherits it through the environment.
        const std::wstring name = std::format(L"Local\\Xbox360RecompReady_{}_{}", GetCurrentProcessId(), GetTickCount64());
        HANDLE ready = CreateEventW(nullptr, TRUE, FALSE, name.c_str());
        SetEnvironmentVariableW(kEventVariable, name.c_str());
        std::wstring commandLine = arguments.empty() ? L"\"" + executable + L"\"" : arguments;
        STARTUPINFOW startup{ sizeof(startup) };
        PROCESS_INFORMATION process{};
        const BOOL started = CreateProcessW(executable.c_str(), commandLine.data(), nullptr, nullptr, FALSE,
                                            creationFlags, nullptr, directory.empty() ? nullptr : directory.c_str(),
                                            &startup, &process);
        SetEnvironmentVariableW(kEventVariable, nullptr);
        if (!started)
        {
            if (error)
                *error = GetLastError();
            if (ready)
                CloseHandle(ready);
            return Result::kFailed;
        }
        if (processId)
            *processId = process.dwProcessId;
        AllowSetForegroundWindow(process.dwProcessId);
        CloseHandle(process.hThread);
        Result result = Result::kTimedOut;
        if (ready)
        {
            HANDLE handles[2] = { ready, process.hProcess };
            const DWORD waited = WaitForMultipleObjects(2, handles, FALSE, DWORD(timeout.count()));
            result = waited == WAIT_OBJECT_0 ? Result::kReady
                   : waited == WAIT_OBJECT_0 + 1 ? Result::kExited
                   : Result::kTimedOut;
            CloseHandle(ready);
        }
        CloseHandle(process.hProcess);
        return result;
    }

    namespace
    {
        std::atomic<bool> g_signalled = false;
        std::atomic<bool> g_contentShown = false; // a frame with content went to the window
        std::atomic<int> g_presentsSince = 0;     // Presents since then

        bool LauncherWaits()
        {
            static const bool waits = GetEnvironmentVariableW(kEventVariable, nullptr, 0) > 0;
            return waits && !g_signalled;
        }

        // Something on it: a sample of pixels (an odd stride, so no tile pattern lines up with it) where at least two
        // of the four bytes are above 24, i.e. not black whatever the byte order and alpha. A small logo on black is
        // enough (0.2 % of the samples).
        bool HasContent(const uint8_t* pixels, size_t bytes)
        {
            constexpr size_t kStride = 37;
            size_t samples = 0, lit = 0;
            for (size_t i = 0; i + 4 <= bytes; i += 4 * kStride)
            {
                samples++;
                lit += (pixels[i] > 24) + (pixels[i + 1] > 24) + (pixels[i + 2] > 24) + (pixels[i + 3] > 24) >= 2;
            }
            return samples && lit * 1000 >= samples * 2;
        }
    }

    void FrameShown(const uint8_t* pixels, size_t bytes, bool windowed)
    {
        if (!LauncherWaits() || g_contentShown || !pixels || !HasContent(pixels, bytes))
            return;
        Log("Transition: the first frame with content (the boot animation is ready)");
        if (!windowed)
            SignalReady();
        else
            g_contentShown = true;
    }

    void FramePresented()
    {
        // The second Present after that frame returns once it is on the screen.
        if (g_contentShown && !g_signalled && ++g_presentsSince >= 2)
            SignalReady();
    }

    void SignalReady()
    {
        if (g_signalled.exchange(true))
            return;
        wchar_t name[256];
        if (!GetEnvironmentVariableW(kEventVariable, name, 256))
            return;
        // Not passed on to processes this one starts.
        SetEnvironmentVariableW(kEventVariable, nullptr);
        if (HANDLE ready = OpenEventW(EVENT_MODIFY_STATE, FALSE, name))
        {
            SetEvent(ready);
            CloseHandle(ready);
            Log("Transition: ready; the launcher's cover goes");
        }
    }
}
