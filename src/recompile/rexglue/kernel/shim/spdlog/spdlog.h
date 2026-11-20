// spdlog/spdlog.h -- CodeBreaker's stand-in: the kernel core is built without spdlog, and the
// base library's few direct calls (spdlog::error ...) only need to print. Forwards to fmt.
#pragma once
#include <cstdio>
#include <fmt/format.h>

namespace spdlog {
template <typename... Args> void error(fmt::format_string<Args...> f, Args&&... a) {
  std::fprintf(stderr, "%s\n", fmt::format(f, std::forward<Args>(a)...).c_str());
}
template <typename... Args> void warn(fmt::format_string<Args...> f, Args&&... a) {
  std::fprintf(stderr, "%s\n", fmt::format(f, std::forward<Args>(a)...).c_str());
}
template <typename... Args> void info(fmt::format_string<Args...> f, Args&&... a) {
  std::fprintf(stdout, "%s\n", fmt::format(f, std::forward<Args>(a)...).c_str());
}
template <typename... Args> void debug(fmt::format_string<Args...>, Args&&...) {}
}  // namespace spdlog
