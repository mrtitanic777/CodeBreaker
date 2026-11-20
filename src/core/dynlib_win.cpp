// The Windows implementation of cb/core/dynlib.hpp (LoadLibraryExW).
#ifdef _WIN32

#include <windows.h>

#include <format>

#include "core/dynlib.hpp"
#include "core/fs.hpp"
#include "core/win.hpp"

namespace cb {

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

std::optional<DynLib> DynLib::load_from_system(std::string_view) { return std::nullopt; }   // (Windows has no system folder of its own for these)

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

#endif  // _WIN32
