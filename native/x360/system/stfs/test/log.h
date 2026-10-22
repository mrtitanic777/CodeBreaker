#pragma once

// The tests' stand-in for a runtime's "log.h": lines to stdout.

#include <cstdio>
#include <format>
#include <string>

template<class... Args>
void Log(std::format_string<Args...> fmt, Args&&... args)
{
    const std::string line = std::format(fmt, std::forward<Args>(args)...);
    std::fprintf(stdout, "[log] %s\n", line.c_str());
}
