#pragma once

// Seamless switches between the ports' processes (owner, 2026-10-16): the dashboard starting a game, a game going
// back to the dashboard. A console shows no desktop in between, so the process that leaves fades its picture to
// black (its presenter), covers the screen with a black window on top of everything, starts the next one, and goes
// only when that one's boot animation is ready (its first frame with content is on the screen: FrameShown /
// FramePresented); its cover goes with it. Runtime-neutral (logs through the runtime's "log.h").
//
// The next process learns how to signal through the environment: XBOX360RECOMP_READY_EVENT names an event the
// launcher waits on. A process started any other way (double-click) has none, and SignalReady does nothing.

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <string>

namespace transition
{
    // Launcher side, before starting the next process: a black borderless window over the monitor this process'
    // window is on (else the primary one), above every other window, until Uncover or the end of this process.
    void CoverScreen();
    void Uncover();
    // How long the leaving process' picture takes to fade to black before CoverScreen (its presenter does it).
    constexpr std::chrono::milliseconds kFadeToBlack{ 500 };

    enum class Result { kReady, kExited, kTimedOut, kFailed };

    // Starts `executable` in `directory` (command line `commandLine`, empty = the quoted path; the environment is
    // this process' plus XBOX360RECOMP_READY_EVENT) and waits until it calls SignalReady (kReady), ends (kExited), or
    // `timeout` passes (kTimedOut: it may still be loading; the launcher goes anyway). kFailed: it could not start
    // (`error` = the Windows error). `processId` receives the new process' ID. The new process may bring its
    // window to the front (AllowSetForegroundWindow).
    Result Launch(const std::wstring& executable, const std::wstring& directory, uint32_t creationFlags,
                  std::chrono::milliseconds timeout, uint32_t* processId = nullptr, uint32_t* error = nullptr,
                  const std::wstring& commandLine = {});

    // Started side: this process is ready to be seen (only the first call does something). Runtimes call it through
    // FrameShown / FramePresented, not directly.
    void SignalReady();

    // Started side (owner, 2026-10-16): the launcher's cover stays while this process only shows black (its window
    // appearing, loading, the black screen before the boot animation) and goes when the boot animation is ready to
    // start: the first frame with something on it is on the screen.
    // FrameShown: the command processor at each swap, with the guest front buffer's bytes (32-bit pixels, any
    // tiling): returns at once when no launcher waits or it was signalled. Headless (`windowed` false) it signals
    // there; with a window, FramePresented (the presenter, after each Present) signals once such a frame is up.
    void FrameShown(const uint8_t* pixels, size_t bytes, bool windowed);
    void FramePresented();
}
