// core/fs.cpp -- the parts of cb/core/fs.hpp that are the same on every platform, built on the platform's primitives
// (fs_win.cpp, fs_posix.cpp).
#include "core/fs.hpp"

#include <atomic>
#include <format>
#include <utility>

#include "core/error.hpp"

namespace cb::fs {

std::string unique_suffix() {
    static std::atomic<std::uint32_t> counter{0};
    return std::format("{:x}-{:x}-{:x}", process_id(), monotonic_ticks() & 0xFFFFFFFFu, counter.fetch_add(1));
}

std::string read_text(const stdfs::path& p) {
    Bytes b = read_file(p);
    return std::string(as_text(b));
}

void write_text(const stdfs::path& p, std::string_view text) { write_file(p, as_bytes(text)); }

// ---- ScopedTempDir ------------------------------------------------------------------------------------

ScopedTempDir::ScopedTempDir(std::string_view prefix, const std::optional<stdfs::path>& base) {
    const stdfs::path root = full_names(base ? *base : stdfs::temp_directory_path());
    make_dirs(root);
    path_ = root / from_text(std::string(prefix) + "-" + unique_suffix());
    make_dirs(path_);
}

ScopedTempDir::ScopedTempDir(ScopedTempDir&& other) noexcept : path_(std::move(other.path_)) { other.path_.clear(); }

ScopedTempDir& ScopedTempDir::operator=(ScopedTempDir&& other) noexcept {
    if (this != &other) {
        this->~ScopedTempDir();
        path_ = std::move(other.path_);
        other.path_.clear();
    }
    return *this;
}

ScopedTempDir::~ScopedTempDir() {
    if (path_.empty()) return;
    try {
        remove_tree(path_);
    } catch (...) {
        // Destructors must not throw; a folder that cannot be removed is left for the next sweep.
    }
}

stdfs::path ScopedTempDir::release() {
    stdfs::path out = std::move(path_);
    path_.clear();
    return out;
}


ScopedEnv::ScopedEnv(std::string_view name, std::optional<std::string_view> value)
    : name_(name), previous_(get_env(name)) {
    set_env(name_, value);
}

ScopedEnv::~ScopedEnv() {
    set_env(name_, previous_ ? std::optional<std::string_view>(*previous_) : std::nullopt);
}

}  // namespace cb::fs
