// cb-unity: alone
// The POSIX implementation of cb/core/dynlib.hpp (dlopen).
#ifndef _WIN32

#include <dlfcn.h>

#include <format>

#include "cb/core/dynlib.hpp"
#include "cb/core/fs.hpp"

namespace cb {

DynLib DynLib::load(const std::filesystem::path& path) {
    // RTLD_LOCAL: the library's symbols stay its own (as a DLL's do); RTLD_NOW: an unresolved symbol is an error here, not at the call.
    void* module = ::dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
    if (module == nullptr) {
        const char* why = ::dlerror();
        std::error_code ec;
        const int code = std::filesystem::exists(path, ec) ? 0 : 2;
        const std::string what = std::string("cannot load: ") + (why != nullptr ? why : "unknown error");
        if (code == 2) throw NotFoundError(path, code, what);
        throw IoError(path, code, what);
    }
    return DynLib(module, path);
}

// The system's own copy of a library (the distribution's libcapstone.so.5 and the like), by the stem's usual names.
std::optional<DynLib> DynLib::load_from_system(std::string_view stem) {
    const std::string base = file_name_for(stem);   // libcapstone.so
    for (const std::string& name : {base, base + ".6", base + ".5", base + ".4", base + ".3", base + ".2", base + ".1"}) {
        if (void* module = ::dlopen(name.c_str(), RTLD_NOW | RTLD_LOCAL)) return DynLib(module, fs::from_text(name));
    }
    return std::nullopt;
}

DynLib::DynLib(DynLib&& other) noexcept : module_(other.module_), path_(std::move(other.path_)) { other.module_ = nullptr; }

DynLib& DynLib::operator=(DynLib&& other) noexcept {
    if (this != &other) {
        if (module_ != nullptr) ::dlclose(module_);
        module_ = other.module_;
        path_ = std::move(other.path_);
        other.module_ = nullptr;
    }
    return *this;
}

DynLib::~DynLib() {
    if (module_ != nullptr) ::dlclose(module_);
}

void* DynLib::address(const char* name) const {
    void* p = ::dlsym(module_, name);
    if (p == nullptr) throw NotFoundError(path_, 2, std::format("does not export '{}'", name));
    return p;
}

}  // namespace cb

#endif  // !_WIN32
