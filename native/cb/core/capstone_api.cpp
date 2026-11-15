#include "cb/core/capstone_api.hpp"

#include <format>

namespace cb::disasm::capstone {

namespace {

struct Loaded {
    DynLib lib;
    Api api;
    Loaded() : lib(DynLib::load_beside_exe("capstone.dll")) {
#define CB_X(name) api.name = lib.function<decltype(&::name)>(#name);
        CB_CAPSTONE_FUNCTIONS(CB_X)
#undef CB_X
        // The structures in capstone.h are those of one major version: a library of another lays its records out differently
        // (a system's libcapstone 4 under this program's 5 headers), and reading them would be reading the wrong bytes.
        int major = 0, minor = 0;
        api.cs_version(&major, &minor);
        if (major != CS_API_MAJOR)
            throw NotFoundError(lib.path(), 0, std::format("this program needs Capstone {}.x, the library found is {}.{}", CS_API_MAJOR, major, minor));
    }
};

const Loaded& loaded() {
    // (never destroyed: thread-local objects of other threads and of main may close their handles through it after
    // the static destructors have run, on the standard libraries that order them so)
    static const Loaded& instance = *new Loaded;
    return instance;
}

}  // namespace

const Api& api() { return loaded().api; }
const DynLib& library() { return loaded().lib; }

void check(cs_err code, std::string_view what) {
    if (code == CS_ERR_OK) return;
    throw Error(std::format("{}: {} (capstone error {})", what, api().cs_strerror(code), static_cast<int>(code)));
}

std::string version_string() {
    int major = 0, minor = 0;
    api().cs_version(&major, &minor);
    return std::format("{}.{}", major, minor);
}

}  // namespace cb::disasm::capstone
