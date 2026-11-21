// core/working_folder.hpp -- the process's working folder, held for one tool that runs inside this process.
//
//     const WorkingFolder in(folder);   // the compiler or linker opens its files relative to `folder`; given back at the end
//
// The working folder belongs to the whole process, so every tool that moves it takes the one lock: a compile on one thread
// must not move it while a link on another is opening its response file.
#pragma once

#include <filesystem>
#include <mutex>
#include <system_error>

namespace cb {

class WorkingFolder {
public:
    explicit WorkingFolder(const std::filesystem::path& folder) : hold_(lock()), before_(std::filesystem::current_path()) {
        if (!folder.empty()) std::filesystem::current_path(folder);
    }
    ~WorkingFolder() {
        std::error_code ec;
        std::filesystem::current_path(before_, ec);
    }
    WorkingFolder(const WorkingFolder&) = delete;
    WorkingFolder& operator=(const WorkingFolder&) = delete;

private:
    static std::mutex& lock() {
        static std::mutex m;
        return m;
    }

    std::lock_guard<std::mutex> hold_;
    std::filesystem::path before_;
};

}  // namespace cb
