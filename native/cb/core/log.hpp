// cb/core/log.hpp -- logging to stderr, and output to stdout.
//
// stdout is for results, stderr for everything else. Machine-read output (the `PROGRESS n m` lines, the TSV the
// Dashboard's tools parse) is a per-command contract and is written with cb::out; diagnostics go through
// cb::log and never touch stdout.
//
//     cb::log::warn(std::format("{} has no exports", name));
//     cb::out("{:08x} {}\n", addr, name);
//
// The log level comes from --verbose/--quiet (set_level) or the CB_LOG environment variable (error, warn, info,
// debug; default warn). CB_LOG_TIME=1 adds a timestamp to each line. Both loggers are thread-safe.
// See docs/CPP_DESIGN.md 3.12.
#pragma once

#include <filesystem>
#include <format>
#include <string>
#include <string_view>

namespace cb::log {

enum class Level { Error = 0, Warn = 1, Info = 2, Debug = 3 };

Level level();
void set_level(Level l);
// "error", "warn", "info", "debug" (case-insensitive); nullopt-style false for anything else.
bool set_level_by_name(std::string_view name);
// Read CB_LOG and CB_LOG_TIME. Called once at startup by the application.
void init_from_env();
// Also write every line to this file (appending); an empty path stops.
void set_file(const std::filesystem::path& p);
// Add a timestamp to each line.
void set_timestamps(bool on);

bool enabled(Level l);
void write(Level l, std::string_view message);

inline void error(std::string_view m) { write(Level::Error, m); }
inline void warn(std::string_view m) { write(Level::Warn, m); }
inline void info(std::string_view m) { write(Level::Info, m); }
inline void debug(std::string_view m) { write(Level::Debug, m); }

}  // namespace cb::log

namespace cb {

// Write UTF-8 text to stdout, exactly as given (no newline added, no \n -> \r\n translation). Output is
// buffered; it is flushed when the buffer fills, by flush_out(), when stdout is a console, and at exit.
void out_text(std::string_view text);
template <class... Args>
void out(std::format_string<Args...> fmt, Args&&... args) {
    out_text(std::format(fmt, std::forward<Args>(args)...));
}
// Write to stderr, unbuffered (for messages that are not log lines: usage text, a final error).
void err_text(std::string_view text);
template <class... Args>
void err(std::format_string<Args...> fmt, Args&&... args) {
    err_text(std::format(fmt, std::forward<Args>(args)...));
}
void flush_out();

}  // namespace cb
