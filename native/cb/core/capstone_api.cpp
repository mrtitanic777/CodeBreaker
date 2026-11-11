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
    }
};

const Loaded& loaded() {
    static const Loaded instance;
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
