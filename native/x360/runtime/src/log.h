#pragma once

#include <cstdio>
#include <cstdlib>
#include <format>
#include <mutex>
#include <string>

// Runtime log: every line goes to stderr, to <program>.log in the working directory and, with live_log, to the
// Xbox LIVE server (live::SendLog).
namespace logging
{
    void Write(const std::string& line);
    // Before the process ends: the log file, and the lines still queued for the Xbox LIVE server (live_log).
    void Flush();
    // When the process owns its console (started by double-click), wait for a key so the last message stays visible.
    void PauseIfOwnConsole();
    // Without a console (the Windows program): the fatal error in a message box (not in CB_HEADLESS runs).
    void ShowFatal(const std::string& message);
}

template<class... Args>
void Log(std::format_string<Args...> fmt, Args&&... args)
{
    logging::Write(std::format(fmt, std::forward<Args>(args)...));
}

template<class... Args>
[[noreturn]] void Fatal(std::format_string<Args...> fmt, Args&&... args)
{
    const std::string message = std::format(fmt, std::forward<Args>(args)...);
    logging::Write("FATAL: " + message);
    logging::Flush();
    logging::ShowFatal(message);
    logging::PauseIfOwnConsole();
    std::_Exit(1);
}
