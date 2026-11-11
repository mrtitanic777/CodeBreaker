// cb/core/dynlib.hpp -- loading a prebuilt DLL and finding its functions.
//
// CodeBreaker ships `unicorn.dll`, `capstone.dll` and `libz3.dll` beside codebreaker.exe (docs/CPP_DESIGN.md
// section 9, decisions 1 and 2; the files and their licences are listed in native/third_party/README.md).
// Nothing is read from Python's site-packages. `DynLib` is the loader the three API modules (unicorn_api.hpp,
// capstone_api.hpp, z3_api.hpp) share: it finds the DLL in a fixed list of places, loads it once, and hands out
// typed function pointers.
#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/error.hpp"

namespace cb {

class DynLib {
public:
    // Load the DLL named `file_name` ("unicorn.dll") from the first place that has it: the directory of the
    // running executable, its `third_party/bin` subfolder, the CB_THIRD_PARTY_BIN environment variable's
    // directory, and the build tree's native/third_party/bin (so tests run from the build folder). Throws
    // NotFoundError naming the places searched, or IoError if the DLL exists but will not load.
    static DynLib load_beside_exe(std::string_view file_name);
    // Load a DLL by full path.
    static DynLib load(const std::filesystem::path& path);

    DynLib(DynLib&&) noexcept;
    DynLib& operator=(DynLib&&) noexcept;
    DynLib(const DynLib&) = delete;
    DynLib& operator=(const DynLib&) = delete;
    ~DynLib();

    const std::filesystem::path& path() const { return path_; }

    // The function called `name`, as `Fn` (a function-pointer type). Throws NotFoundError if the DLL does not
    // export it: a DLL older than the API table is a clear error at load, not a crash at the call.
    template <class Fn>
    Fn function(const char* name) const {
        return reinterpret_cast<Fn>(address(name));
    }

    // The places load_beside_exe looks, in order (for messages and tests).
    static std::vector<std::filesystem::path> search_dirs();

private:
    DynLib(void* module, std::filesystem::path path) : module_(module), path_(std::move(path)) {}
    void* address(const char* name) const;
    void* module_ = nullptr;
    std::filesystem::path path_;
};

}  // namespace cb
