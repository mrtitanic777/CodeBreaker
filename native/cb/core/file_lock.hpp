// cb/core/file_lock.hpp -- a lock between processes, held on a file.
//
// The lock is the operating system's byte-range lock on a file, so it belongs to the process that holds it and goes when that process
// ends, however it ends: a run that was killed leaves a file behind but no lock, and the next one simply takes it (there is no stale
// lock to detect or to clean up). The file's text names the holder ("pid 1234"), for the message a waiter gives.
//
//     auto lock = file_lock::acquire(state / "work" / "regress" / "build.lock", [](const std::string& holder) { cb::out("waiting for {}\n", holder); });
//     ... the work only one process may do at a time ...        // the lock is let go when `lock` goes out of scope
#pragma once

#include <chrono>
#include <filesystem>
#include <functional>
#include <memory>
#include <optional>
#include <string>

namespace cb::file_lock {

class FileLock {
public:
    FileLock();
    FileLock(FileLock&&) noexcept;
    FileLock& operator=(FileLock&&) noexcept;
    FileLock(const FileLock&) = delete;
    FileLock& operator=(const FileLock&) = delete;
    ~FileLock();
    explicit operator bool() const { return impl_ != nullptr; }
    void release();

private:
    struct Impl;
    explicit FileLock(std::unique_ptr<Impl> impl);
    std::unique_ptr<Impl> impl_;
    friend std::optional<FileLock> try_acquire(const std::filesystem::path&, const std::string&);
};

// Take the lock at once, or nullopt when another process (or another holder in this one) has it. `holder` is written into the file
// for the others to read; the default names this process.
std::optional<FileLock> try_acquire(const std::filesystem::path& file, const std::string& holder = std::string());

// Wait for the lock. `waiting` is called with the holder's text when the lock is first found taken, and again whenever the holder
// changes. Gives up (nullopt) after `timeout`, when one is given.
std::optional<FileLock> acquire(const std::filesystem::path& file, const std::function<void(const std::string& holder)>& waiting = {},
                                std::optional<std::chrono::milliseconds> timeout = std::nullopt,
                                std::chrono::milliseconds poll = std::chrono::milliseconds(500));

// Who holds it, as the file says ("" when the file is not there); meaningful only while the lock is taken.
std::string holder_of(const std::filesystem::path& file);

}  // namespace cb::file_lock
