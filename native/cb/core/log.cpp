#include "cb/core/log.hpp"

#include <windows.h>

#include <atomic>
#include <chrono>
#include <mutex>

#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb {

namespace {

// Write all bytes to a handle; true on success.
bool write_handle(HANDLE h, std::string_view data) {
    while (!data.empty()) {
        DWORD wrote = 0;
        const DWORD chunk = static_cast<DWORD>(std::min<std::size_t>(data.size(), 1u << 20));
        if (!WriteFile(h, data.data(), chunk, &wrote, nullptr) || wrote == 0) return false;
        data.remove_prefix(wrote);
    }
    return true;
}

bool is_console(HANDLE h) {
    DWORD mode = 0;
    return GetConsoleMode(h, &mode) != 0;
}

// The buffered stdout. A function-local static: constructed on first use, flushed when the program exits.
class StdoutSink {
public:
    void write(std::string_view text) {
        std::lock_guard lock(mutex_);
        buffer_.append(text);
        // (the handle is looked up each time: a test, or a parent, may have redirected stdout)
        if (buffer_.size() >= kLimit || is_console(GetStdHandle(STD_OUTPUT_HANDLE))) flush_locked();
    }
    void flush() {
        std::lock_guard lock(mutex_);
        flush_locked();
    }
    ~StdoutSink() { flush(); }

private:
    static constexpr std::size_t kLimit = 1 << 16;
    void flush_locked() {
        if (buffer_.empty()) return;
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        if (h != INVALID_HANDLE_VALUE && h != nullptr) write_handle(h, buffer_);
        buffer_.clear();
    }
    std::mutex mutex_;
    std::string buffer_;
};

StdoutSink& stdout_sink() {
    static StdoutSink sink;
    return sink;
}

}  // namespace

void out_text(std::string_view text) { stdout_sink().write(text); }
void flush_out() { stdout_sink().flush(); }

void err_text(std::string_view text) {
    // Order with stdout: whatever is buffered for stdout was written before this message.
    flush_out();
    HANDLE h = GetStdHandle(STD_ERROR_HANDLE);
    if (h != INVALID_HANDLE_VALUE && h != nullptr) write_handle(h, text);
}

}  // namespace cb

namespace cb::log {

namespace {

struct State {
    std::atomic<Level> level{Level::Warn};
    std::atomic<bool> timestamps{false};
    std::mutex mutex;   // guards `file` and the ordering of lines
    std::filesystem::path file;
};

State& state() {
    static State s;
    return s;
}

const char* label(Level l) {
    switch (l) {
        case Level::Error: return "error";
        case Level::Warn: return "warn";
        case Level::Info: return "info";
        case Level::Debug: return "debug";
    }
    return "?";
}

std::string timestamp() {
    SYSTEMTIME t;
    GetLocalTime(&t);
    return std::format("{:02}:{:02}:{:02}.{:03} ", t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
}

}  // namespace

Level level() { return state().level.load(); }
void set_level(Level l) { state().level.store(l); }
void set_timestamps(bool on) { state().timestamps.store(on); }

bool set_level_by_name(std::string_view name) {
    const std::string n = text::lower(text::trim(name));
    if (n == "error") set_level(Level::Error);
    else if (n == "warn" || n == "warning") set_level(Level::Warn);
    else if (n == "info") set_level(Level::Info);
    else if (n == "debug") set_level(Level::Debug);
    else return false;
    return true;
}

void init_from_env() {
    if (auto v = fs::get_env("CB_LOG")) (void)set_level_by_name(*v);
    if (auto v = fs::get_env("CB_LOG_TIME")) set_timestamps(!v->empty() && *v != "0");
}

void set_file(const std::filesystem::path& p) {
    std::lock_guard lock(state().mutex);
    state().file = p;
}

bool enabled(Level l) { return static_cast<int>(l) <= static_cast<int>(level()); }

void write(Level l, std::string_view message) {
    if (!enabled(l)) return;
    std::string line;
    if (state().timestamps.load()) line += timestamp();
    line += std::format("{}: ", label(l));
    line.append(message);
    line.push_back('\n');
    std::lock_guard lock(state().mutex);
    err_text(line);
    if (!state().file.empty()) {
        try {
            fs::append_file(state().file, as_bytes(line));
        } catch (const Error&) {
            // A log that cannot be written must not take the program down.
        }
    }
}

}  // namespace cb::log
