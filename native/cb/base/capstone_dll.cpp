#include "cb/base/capstone_dll.hpp"

#include <string>

#include "cb/core/dynlib.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"

namespace cb::base::capstone_dll {

namespace {

namespace stdfs = std::filesystem;

stdfs::path locate(const char* env_name, const char* file_name) {
    std::error_code ec;
    if (const auto env = fs::get_env(env_name); env && !env->empty()) {
        const stdfs::path named = fs::from_text(*env);
        if (stdfs::is_regular_file(named, ec)) return named;
    }
    std::string searched;
    for (const stdfs::path& dir : DynLib::search_dirs()) {
        const stdfs::path candidate = dir / file_name;
        if (stdfs::is_regular_file(candidate, ec)) return candidate;
        searched += "\n  " + fs::to_text(dir);
    }
    throw NotFoundError(file_name, 2, std::string("not found; looked in") + searched);
}

}  // namespace

stdfs::path capstone() { return locate("CB_CAPSTONE_DLL", "capstone.dll"); }

stdfs::path unicorn() { return locate("CB_UNICORN_DLL", "unicorn.dll"); }

}  // namespace cb::base::capstone_dll
