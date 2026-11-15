// cb/core/dynlib.cpp -- finding the engine libraries; loading them is the platform's (dynlib_win.cpp, dynlib_posix.cpp).
#include "cb/core/dynlib.hpp"

#include <format>

#include "cb/core/fs.hpp"

#ifndef CB_THIRD_PARTY_BIN
#define CB_THIRD_PARTY_BIN ""
#endif

namespace cb {

std::vector<std::filesystem::path> DynLib::search_dirs() {
    std::vector<std::filesystem::path> dirs;
    const auto exe = fs::executable_path().parent_path();
    if (!exe.empty()) {
        dirs.push_back(exe);
        dirs.push_back(exe / "third_party" / "bin");
    }
    if (auto env = fs::get_env("CB_THIRD_PARTY_BIN"); env && !env->empty()) dirs.push_back(fs::from_text(*env));
    if (std::string_view(CB_THIRD_PARTY_BIN).size() != 0) dirs.push_back(fs::from_text(CB_THIRD_PARTY_BIN));
    return dirs;
}

std::string DynLib::file_name_for(std::string_view stem) {
#ifdef _WIN32
    return std::string(stem) + ".dll";
#else
    return (stem.starts_with("lib") ? std::string(stem) : "lib" + std::string(stem)) + ".so";
#endif
}

DynLib DynLib::load_beside_exe(std::string_view file_name) {
    // "unicorn.dll" names the library on every platform: the platform's own file name is made from the stem.
    std::string stem(file_name);
    if (stem.size() > 4 && stem.ends_with(".dll")) stem.resize(stem.size() - 4);
    const std::string native_name = file_name_for(stem);
    std::string searched;
    for (const auto& dir : search_dirs()) {
        const std::filesystem::path candidate = dir / fs::from_text(native_name);
        std::error_code ec;
        if (std::filesystem::is_regular_file(candidate, ec)) return load(candidate);
        searched += (searched.empty() ? "" : "; ") + fs::to_text(dir);
    }
    if (auto system = load_from_system(stem)) return std::move(*system);
    throw NotFoundError(std::string(file_name), 2,
                        std::format("not found beside the executable (looked in: {})", searched));
}

}  // namespace cb
