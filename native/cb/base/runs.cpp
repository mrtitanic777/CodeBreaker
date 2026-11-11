#include "cb/base/runs.hpp"

#include <process.h>

#include <ctime>
#include <format>
#include <string>
#include <thread>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"

namespace cb::base::runs {

namespace stdfs = std::filesystem;

namespace {

constexpr std::size_t kMaxTagLength = 40;

stdfs::path runs_folder(const stdfs::path& state, std::string_view tool) {
    stdfs::path folder = state / std::string(tool) / "runs";
    fs::make_dirs(folder);
    return folder;
}

bool is_tag_char(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '.' || c == '-';
}

// Runs of anything but letters, digits, '.' and '-' become one '-'; cut to kMaxTagLength; "run" if nothing is left.
std::string clean_tag(std::string_view tag) {
    std::string out;
    bool in_run = false;
    for (char c : tag) {
        if (is_tag_char(c)) {
            out.push_back(c);
            in_run = false;
        } else if (!in_run) {
            out.push_back('-');
            in_run = true;
        }
    }
    if (out.size() > kMaxTagLength) out.resize(kMaxTagLength);
    return out.empty() ? "run" : out;
}

std::string timestamp() {
    const std::time_t now = std::time(nullptr);
    std::tm local{};
    localtime_s(&local, &now);
    char buf[32];
    std::strftime(buf, sizeof buf, "%Y%m%d-%H%M%S", &local);
    return buf;
}

bool older_than(const stdfs::path& p, std::chrono::seconds age) {
    std::error_code ec;
    const auto written = stdfs::last_write_time(p, ec);
    if (ec) return false;
    const auto written_at = std::chrono::clock_cast<std::chrono::system_clock>(written);
    return std::chrono::system_clock::now() - written_at > age;
}

}  // namespace

stdfs::path results(const stdfs::path& state, std::string_view tool, std::string_view name) {
    stdfs::path folder = state / std::string(tool) / std::string(name);
    fs::make_dirs(folder);
    return folder;
}

bool remove(const stdfs::path& folder, int tries) {
    for (int attempt = 0; attempt < tries; ++attempt) {
        try {
            (void)fs::remove_tree(folder);
        } catch (const IoError&) {
            // still held open: looked at below, tried again after a moment
        }
        std::error_code ec;
        if (!stdfs::exists(folder, ec)) return true;
        if (attempt + 1 < tries) std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return false;
}

std::vector<stdfs::path> sweep(const stdfs::path& state, std::string_view tool, std::chrono::seconds age) {
    std::vector<stdfs::path> gone;
    std::error_code ec;
    for (const auto& entry : stdfs::directory_iterator(runs_folder(state, tool), ec)) {
        std::error_code entry_ec;
        if (entry.is_directory(entry_ec) && older_than(entry.path(), age) && remove(entry.path(), 1))
            gone.push_back(entry.path());
    }
    return gone;
}

stdfs::path start(const stdfs::path& state, std::string_view tool, std::string_view tag) {
    (void)sweep(state, tool);
    const std::string name = std::format("{}_{}_{}", clean_tag(tag), timestamp(), _getpid());
    const stdfs::path base = runs_folder(state, tool);
    stdfs::path path = base / name;
    for (int n = 1; stdfs::exists(path); ++n) path = base / std::format("{}_{}", name, n);
    fs::make_dirs(path);
    return path;
}

}  // namespace cb::base::runs
