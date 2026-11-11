#include "cb/core/dynlib.hpp"

#include <windows.h>

#include <format>

#include "cb/core/fs.hpp"
#include "cb/core/win.hpp"

#ifndef CB_THIRD_PARTY_BIN
#define CB_THIRD_PARTY_BIN ""
#endif

namespace cb {

namespace {

std::filesystem::path exe_dir() {
    std::wstring buf(1024, L'\0');
    while (true) {
        const DWORD n = GetModuleFileNameW(nullptr, buf.data(), static_cast<DWORD>(buf.size()));
        if (n == 0) return {};
        if (n < buf.size()) {
            buf.resize(n);
            break;
        }
        buf.resize(buf.size() * 2);
    }
    return std::filesystem::path(buf).parent_path();
}

}  // namespace

std::vector<std::filesystem::path> DynLib::search_dirs() {
    std::vector<std::filesystem::path> dirs;
    const auto exe = exe_dir();
    if (!exe.empty()) {
        dirs.push_back(exe);
        dirs.push_back(exe / "third_party" / "bin");
    }
    if (auto env = fs::get_env("CB_THIRD_PARTY_BIN"); env && !env->empty()) dirs.push_back(fs::from_text(*env));
    if (std::string_view(CB_THIRD_PARTY_BIN).size() != 0) dirs.push_back(fs::from_text(CB_THIRD_PARTY_BIN));
    return dirs;
}

DynLib DynLib::load(const std::filesystem::path& path) {
    // LOAD_WITH_ALTERED_SEARCH_PATH: the DLL's own dependencies are looked for beside it.
    HMODULE module = LoadLibraryExW(path.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
    if (module == nullptr) {
        const DWORD code = GetLastError();
        if (code == ERROR_MOD_NOT_FOUND || code == ERROR_FILE_NOT_FOUND || code == ERROR_PATH_NOT_FOUND)
            throw NotFoundError(path, static_cast<int>(code), "cannot load: " + win::error_message(code));
        throw IoError(path, static_cast<int>(code), "cannot load: " + win::error_message(code));
    }
    return DynLib(module, path);
}

DynLib DynLib::load_beside_exe(std::string_view file_name) {
    std::string searched;
    for (const auto& dir : search_dirs()) {
        const std::filesystem::path candidate = dir / fs::from_text(file_name);
        if (std::filesystem::is_regular_file(candidate)) return load(candidate);
        searched += (searched.empty() ? "" : "; ") + fs::to_text(dir);
    }
    throw NotFoundError(std::string(file_name), ERROR_FILE_NOT_FOUND,
                        std::format("not found beside the executable (looked in: {})", searched));
}

DynLib::DynLib(DynLib&& other) noexcept : module_(other.module_), path_(std::move(other.path_)) { other.module_ = nullptr; }

DynLib& DynLib::operator=(DynLib&& other) noexcept {
    if (this != &other) {
        if (module_ != nullptr) FreeLibrary(static_cast<HMODULE>(module_));
        module_ = other.module_;
        path_ = std::move(other.path_);
        other.module_ = nullptr;
    }
    return *this;
}

DynLib::~DynLib() {
    if (module_ != nullptr) FreeLibrary(static_cast<HMODULE>(module_));
}

void* DynLib::address(const char* name) const {
    FARPROC p = GetProcAddress(static_cast<HMODULE>(module_), name);
    if (p == nullptr)
        throw NotFoundError(path_, static_cast<int>(GetLastError()), std::format("does not export '{}'", name));
    return reinterpret_cast<void*>(p);
}

}  // namespace cb
