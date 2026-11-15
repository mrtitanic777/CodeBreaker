#include "cb/core/z3_api.hpp"

#include <format>

namespace cb::z3 {

namespace {

struct Loaded {
    DynLib lib;
    Api api;
    Loaded() : lib(DynLib::load_beside_exe("libz3.dll")) {
#define CB_X(name) api.name = lib.function<decltype(&::name)>(#name);
        CB_Z3_FUNCTIONS(CB_X)
#undef CB_X
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

std::string version_string() {
    unsigned major = 0, minor = 0, build = 0, revision = 0;
    api().Z3_get_version(&major, &minor, &build, &revision);
    return std::format("{}.{}.{}.{}", major, minor, build, revision);
}

}  // namespace cb::z3
