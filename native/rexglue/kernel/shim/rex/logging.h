// rex/logging.h -- the kernel core's logging without spdlog. CodeBreaker's: the kernel's
// REXLOG_* / REXSYS_* ... macros print to stderr through fmt, at the level the REX_LOG
// environment variable names (trace, debug, info, warn, error, critical; default warn) -- so a
// recompiled title says why it stopped. (The DXBC translator's copy of this shim stays silent.)
#pragma once
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fmt/format.h>

namespace rex::log_shim {
inline int threshold() {
  static const int level = [] {
    const char* e = std::getenv("REX_LOG");
    const char* names[] = {"trace", "debug", "info", "warn", "error", "critical"};
    for (int i = 0; e && i < 6; ++i)
      if (!std::strcmp(e, names[i])) return i;
    return 3;
  }();
  return level;
}
template <typename... A>
inline void emit(int level, const char* tag, fmt::string_view f, const A&... a) {
  if (level < threshold()) return;
  try {
    std::fprintf(stderr, "[%s] %s\n", tag, fmt::vformat(f, fmt::make_format_args(a...)).c_str());
  } catch (...) {
    std::fprintf(stderr, "[%s] (unformattable message)\n", tag);
  }
}
}  // namespace rex::log_shim

// category definition/declaration -> nothing
#define REXLOG_DEFINE_CATEGORY(...)
#define REXLOG_DECLARE_CATEGORY(...)

#define REXLOG_TRACE(...) ::rex::log_shim::emit(0, "trace", __VA_ARGS__)
#define REXLOG_DEBUG(...) ::rex::log_shim::emit(1, "debug", __VA_ARGS__)
#define REXLOG_INFO(...) ::rex::log_shim::emit(2, "info", __VA_ARGS__)
#define REXLOG_WARN(...) ::rex::log_shim::emit(3, "warn", __VA_ARGS__)
#define REXLOG_ERROR(...) ::rex::log_shim::emit(4, "error", __VA_ARGS__)
#define REXLOG_CRITICAL(...) ::rex::log_shim::emit(5, "critical", __VA_ARGS__)
#define REXLOG_CAT_TRACE(cat, ...) ::rex::log_shim::emit(0, "trace", __VA_ARGS__)
#define REXLOG_CAT_DEBUG(cat, ...) ::rex::log_shim::emit(1, "debug", __VA_ARGS__)
#define REXLOG_CAT_INFO(cat, ...) ::rex::log_shim::emit(2, "info", __VA_ARGS__)
#define REXLOG_CAT_WARN(cat, ...) ::rex::log_shim::emit(3, "warn", __VA_ARGS__)
#define REXLOG_CAT_ERROR(cat, ...) ::rex::log_shim::emit(4, "error", __VA_ARGS__)
#define REXLOG_CAT_CRITICAL(cat, ...) ::rex::log_shim::emit(5, "critical", __VA_ARGS__)
#define REXLOG_CAT_TID_TRACE(cat, ...) ::rex::log_shim::emit(0, "trace", __VA_ARGS__)
#define REXLOG_CAT_TID_DEBUG(cat, ...) ::rex::log_shim::emit(1, "debug", __VA_ARGS__)
#define REXLOG_CAT_TID_INFO(cat, ...) ::rex::log_shim::emit(2, "info", __VA_ARGS__)
#define REXLOG_CAT_TID_WARN(cat, ...) ::rex::log_shim::emit(3, "warn", __VA_ARGS__)
#define REXLOG_CAT_TID_ERROR(cat, ...) ::rex::log_shim::emit(4, "error", __VA_ARGS__)
#define REXLOG_CAT_TID_CRITICAL(cat, ...) ::rex::log_shim::emit(5, "critical", __VA_ARGS__)
#define REXLOG_CAT_FN_TRACE(cat, ...) ::rex::log_shim::emit(0, "trace", __VA_ARGS__)
#define REXLOG_CAT_FN_DEBUG(cat, ...) ::rex::log_shim::emit(1, "debug", __VA_ARGS__)
#define REXLOG_CAT_FN_INFO(cat, ...) ::rex::log_shim::emit(2, "info", __VA_ARGS__)
#define REXLOG_CAT_FN_WARN(cat, ...) ::rex::log_shim::emit(3, "warn", __VA_ARGS__)
#define REXLOG_CAT_FN_ERROR(cat, ...) ::rex::log_shim::emit(4, "error", __VA_ARGS__)
#define REXLOG_CAT_FN_CRITICAL(cat, ...) ::rex::log_shim::emit(5, "critical", __VA_ARGS__)
#define REXAPU_TRACE(...) ::rex::log_shim::emit(0, "apu trace", __VA_ARGS__)
#define REXAPU_DEBUG(...) ::rex::log_shim::emit(1, "apu debug", __VA_ARGS__)
#define REXAPU_INFO(...) ::rex::log_shim::emit(2, "apu info", __VA_ARGS__)
#define REXAPU_WARN(...) ::rex::log_shim::emit(3, "apu warn", __VA_ARGS__)
#define REXAPU_ERROR(...) ::rex::log_shim::emit(4, "apu error", __VA_ARGS__)
#define REXAPU_CRITICAL(...) ::rex::log_shim::emit(5, "apu critical", __VA_ARGS__)
#define REXCPU_TRACE(...) ::rex::log_shim::emit(0, "cpu trace", __VA_ARGS__)
#define REXCPU_DEBUG(...) ::rex::log_shim::emit(1, "cpu debug", __VA_ARGS__)
#define REXCPU_INFO(...) ::rex::log_shim::emit(2, "cpu info", __VA_ARGS__)
#define REXCPU_WARN(...) ::rex::log_shim::emit(3, "cpu warn", __VA_ARGS__)
#define REXCPU_ERROR(...) ::rex::log_shim::emit(4, "cpu error", __VA_ARGS__)
#define REXCPU_CRITICAL(...) ::rex::log_shim::emit(5, "cpu critical", __VA_ARGS__)
#define REXGPU_TRACE(...) ::rex::log_shim::emit(0, "gpu trace", __VA_ARGS__)
#define REXGPU_DEBUG(...) ::rex::log_shim::emit(1, "gpu debug", __VA_ARGS__)
#define REXGPU_INFO(...) ::rex::log_shim::emit(2, "gpu info", __VA_ARGS__)
#define REXGPU_WARN(...) ::rex::log_shim::emit(3, "gpu warn", __VA_ARGS__)
#define REXGPU_ERROR(...) ::rex::log_shim::emit(4, "gpu error", __VA_ARGS__)
#define REXGPU_CRITICAL(...) ::rex::log_shim::emit(5, "gpu critical", __VA_ARGS__)
#define REXFS_TRACE(...) ::rex::log_shim::emit(0, "fs trace", __VA_ARGS__)
#define REXFS_DEBUG(...) ::rex::log_shim::emit(1, "fs debug", __VA_ARGS__)
#define REXFS_INFO(...) ::rex::log_shim::emit(2, "fs info", __VA_ARGS__)
#define REXFS_WARN(...) ::rex::log_shim::emit(3, "fs warn", __VA_ARGS__)
#define REXFS_ERROR(...) ::rex::log_shim::emit(4, "fs error", __VA_ARGS__)
#define REXFS_CRITICAL(...) ::rex::log_shim::emit(5, "fs critical", __VA_ARGS__)
#define REXKRNL_TRACE(...) ::rex::log_shim::emit(0, "krnl trace", __VA_ARGS__)
#define REXKRNL_DEBUG(...) ::rex::log_shim::emit(1, "krnl debug", __VA_ARGS__)
#define REXKRNL_INFO(...) ::rex::log_shim::emit(2, "krnl info", __VA_ARGS__)
#define REXKRNL_WARN(...) ::rex::log_shim::emit(3, "krnl warn", __VA_ARGS__)
#define REXKRNL_ERROR(...) ::rex::log_shim::emit(4, "krnl error", __VA_ARGS__)
#define REXKRNL_CRITICAL(...) ::rex::log_shim::emit(5, "krnl critical", __VA_ARGS__)
#define REXKRNLFN_TRACE(...) ::rex::log_shim::emit(0, "krnlfn trace", __VA_ARGS__)
#define REXKRNLFN_DEBUG(...) ::rex::log_shim::emit(1, "krnlfn debug", __VA_ARGS__)
#define REXKRNLFN_INFO(...) ::rex::log_shim::emit(2, "krnlfn info", __VA_ARGS__)
#define REXKRNLFN_WARN(...) ::rex::log_shim::emit(3, "krnlfn warn", __VA_ARGS__)
#define REXKRNLFN_ERROR(...) ::rex::log_shim::emit(4, "krnlfn error", __VA_ARGS__)
#define REXKRNLFN_CRITICAL(...) ::rex::log_shim::emit(5, "krnlfn critical", __VA_ARGS__)
#define REXLOGFN_TRACE(...) ::rex::log_shim::emit(0, "logfn trace", __VA_ARGS__)
#define REXLOGFN_DEBUG(...) ::rex::log_shim::emit(1, "logfn debug", __VA_ARGS__)
#define REXLOGFN_INFO(...) ::rex::log_shim::emit(2, "logfn info", __VA_ARGS__)
#define REXLOGFN_WARN(...) ::rex::log_shim::emit(3, "logfn warn", __VA_ARGS__)
#define REXLOGFN_ERROR(...) ::rex::log_shim::emit(4, "logfn error", __VA_ARGS__)
#define REXLOGFN_CRITICAL(...) ::rex::log_shim::emit(5, "logfn critical", __VA_ARGS__)
#define REXSYS_TRACE(...) ::rex::log_shim::emit(0, "sys trace", __VA_ARGS__)
#define REXSYS_DEBUG(...) ::rex::log_shim::emit(1, "sys debug", __VA_ARGS__)
#define REXSYS_INFO(...) ::rex::log_shim::emit(2, "sys info", __VA_ARGS__)
#define REXSYS_WARN(...) ::rex::log_shim::emit(3, "sys warn", __VA_ARGS__)
#define REXSYS_ERROR(...) ::rex::log_shim::emit(4, "sys error", __VA_ARGS__)
#define REXSYS_CRITICAL(...) ::rex::log_shim::emit(5, "sys critical", __VA_ARGS__)
#define REXCODEGEN_TRACE(...) ::rex::log_shim::emit(0, "codegen trace", __VA_ARGS__)
#define REXCODEGEN_DEBUG(...) ::rex::log_shim::emit(1, "codegen debug", __VA_ARGS__)
#define REXCODEGEN_INFO(...) ::rex::log_shim::emit(2, "codegen info", __VA_ARGS__)
#define REXCODEGEN_WARN(...) ::rex::log_shim::emit(3, "codegen warn", __VA_ARGS__)
#define REXCODEGEN_ERROR(...) ::rex::log_shim::emit(4, "codegen error", __VA_ARGS__)
#define REXCODEGEN_CRITICAL(...) ::rex::log_shim::emit(5, "codegen critical", __VA_ARGS__)
#define REX_LOG_CATEGORY_NOOP(PREFIX)
