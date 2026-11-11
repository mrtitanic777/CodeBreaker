#include "cb/core/unicorn_api.hpp"

#include <format>

namespace cb::emu::unicorn {

namespace {

// The library and its table, built together on first use. They live to the end of the program: unloading an
// emulator library while another thread might still call into it is never worth the tidiness.
struct Loaded {
    DynLib lib;
    Api api;
    Loaded() : lib(DynLib::load_beside_exe("unicorn.dll")) {
#define CB_X(name) api.name = lib.function<decltype(&::name)>(#name);
        CB_UNICORN_FUNCTIONS(CB_X)
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

void check(uc_err code, std::string_view what) {
    if (code == UC_ERR_OK) return;
    throw EmulationError(std::format("{}: {} (unicorn error {})", what, api().uc_strerror(code), static_cast<int>(code)));
}

std::string version_string() {
    unsigned major = 0, minor = 0;
    api().uc_version(&major, &minor);
    return std::format("{}.{}", major, minor);
}

}  // namespace cb::emu::unicorn
