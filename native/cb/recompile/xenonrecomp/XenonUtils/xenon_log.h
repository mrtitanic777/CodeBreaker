#pragma once

// Where XenonRecomp's and XenonAnalyse's messages go: stdout, as the programs always printed them, or -- when they run
// inside another program (CodeBreaker calls xenon_recomp_main / xenon_analyse_main in-process) -- the string the caller
// collects them in. The sink is per thread, so recompiles on different threads keep their own messages.
#include <cstdio>
#include <string>
#include <string_view>
#include <utility>

#include <fmt/format.h>

namespace xenon_log {

inline thread_local std::string* sink = nullptr;

inline void write(std::string_view text)
{
    if (sink != nullptr)
        sink->append(text);
    else
        std::fwrite(text.data(), 1, text.size(), stdout);
}

template <typename... Args>
inline void print(fmt::format_string<Args...> format, Args&&... args)
{
    write(fmt::format(format, std::forward<Args>(args)...));
}

template <typename... Args>
inline void println(fmt::format_string<Args...> format, Args&&... args)
{
    std::string line = fmt::format(format, std::forward<Args>(args)...);
    line += '\n';
    write(line);
}

// Collects the messages of this thread into `into` while it lives.
class Capture
{
public:
    explicit Capture(std::string& into) : previous(sink) { sink = &into; }
    ~Capture() { sink = previous; }
    Capture(const Capture&) = delete;
    Capture& operator=(const Capture&) = delete;

private:
    std::string* previous;
};

} // namespace xenon_log
