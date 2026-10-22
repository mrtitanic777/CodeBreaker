#include "log.h"

#include "live/client.h"
#include "transition/transition.h"

#include <windows.h>

#include <conio.h>

namespace logging
{
    // The log's file name: the program's own, <program>.log (in the working directory, the title's folder).
    const std::string& DefaultFile()
    {
        static const std::string name = [] {
            char path[MAX_PATH] = {};
            GetModuleFileNameA(nullptr, path, MAX_PATH);
            std::string stem = path;
            if (const size_t slash = stem.find_last_of("\\/"); slash != std::string::npos)
                stem = stem.substr(slash + 1);
            if (const size_t dot = stem.rfind('.'); dot != std::string::npos)
                stem = stem.substr(0, dot);
            return (stem.empty() ? std::string("x360") : stem) + ".log";
        }();
        return name;
    }

    void Write(const std::string& line)
    {
        static std::mutex mutex;
        // A dashboard started by the previous one (XamLoaderLaunchTitleEx, kernel/xam_loader.cpp) continues its log.
        // CB_LOG=<file>: another log (the launcher's thumbnail runs, beside the launcher's own).
        static FILE* file = std::fopen(std::getenv("CB_LOG") && std::getenv("CB_LOG")[0] ? std::getenv("CB_LOG") : DefaultFile().c_str(),
                                       std::getenv("XBOX360RECOMP_DASH_RELAUNCH") ? "a" : "w");
        // CB_LOG_TIME=1: each line starts with the milliseconds since the first one (finding hitches).
        static const bool timed = [] { const char* v = std::getenv("CB_LOG_TIME"); return v && v[0] == '1'; }();
        static const ULONGLONG start = GetTickCount64();
        std::lock_guard lock(mutex);
        std::fprintf(stderr, "%s\n", line.c_str());
        if (file)
        {
            if (timed)
                std::fprintf(file, "[%7llu] ", GetTickCount64() - start);
            std::fprintf(file, "%s\n", line.c_str());
            std::fflush(file);
        }
        live::SendLog(line); // kept only with live_log (console_settings.ini); queued, sent by the client's thread
    }

    void Flush()
    {
        std::fflush(nullptr);
        live::FlushLog();
    }
}

namespace logging
{
    void ShowFatal(const std::string& message)
    {
        // A Windows program has no console (CMakeLists: /SUBSYSTEM:WINDOWS): the error goes in a box (not in headless
        // test runs, which must not wait), above a transition's black cover (taken away first).
        const char* headless = std::getenv("CB_HEADLESS");
        if (GetConsoleWindow() || (headless && headless[0] == '1'))
            return;
        transition::Uncover();
        const std::string text = message + "\n\nThe log is " + DefaultFile() + " in the title's folder.";
        std::wstring wide(size_t(MultiByteToWideChar(CP_UTF8, 0, text.data(), int(text.size()), nullptr, 0)), L'\0');
        MultiByteToWideChar(CP_UTF8, 0, text.data(), int(text.size()), wide.data(), int(wide.size()));
        MessageBoxW(nullptr, wide.c_str(), L"Xbox 360", MB_OK | MB_ICONERROR | MB_TOPMOST);
    }

    void PauseIfOwnConsole()
    {
        DWORD processes[2];
        if (GetConsoleProcessList(processes, 2) == 1)
        {
            std::fprintf(stderr, "\nPress any key to close this window.\n");
            _getch();
        }
    }
}
