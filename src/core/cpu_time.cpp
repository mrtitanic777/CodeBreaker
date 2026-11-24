// cb-unity: alone (the Windows headers' macros)
#include "core/cpu_time.hpp"

#include <chrono>
#include <ctime>

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace cb {

double thread_cpu_seconds() {
#if defined(_WIN32)
    FILETIME created, exited, kernel, user;
    if (GetThreadTimes(GetCurrentThread(), &created, &exited, &kernel, &user)) {
        const auto ticks = [](const FILETIME& t) { return static_cast<double>((static_cast<unsigned long long>(t.dwHighDateTime) << 32) | t.dwLowDateTime); };
        return (ticks(kernel) + ticks(user)) * 1e-7;   // (100 ns units)
    }
#elif defined(CLOCK_THREAD_CPUTIME_ID)
    timespec t;
    if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t) == 0) return static_cast<double>(t.tv_sec) + static_cast<double>(t.tv_nsec) * 1e-9;
#endif
    return std::chrono::duration<double>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

}  // namespace cb
