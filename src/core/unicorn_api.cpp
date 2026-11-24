#include "core/unicorn_api.hpp"

#include <cstddef>
#include <format>
#include <mutex>

#ifdef _WIN32
#include <windows.h>
#endif

namespace cb::emu::unicorn {

namespace {

#ifdef _WIN32
// A write by Unicorn's own code to a page it reserved as executable and has not committed: commit up to Unicorn's 4 MB, no further than
// the reservation goes. Anything else is left to the next handler.
LONG CALLBACK commit_within_reservation(EXCEPTION_POINTERS* info) {
    const EXCEPTION_RECORD& record = *info->ExceptionRecord;
    if (record.ExceptionCode != EXCEPTION_ACCESS_VIOLATION || record.NumberParameters < 2 || record.ExceptionInformation[0] != 1)
        return EXCEPTION_CONTINUE_SEARCH;
    MEMORY_BASIC_INFORMATION region{};
    if (VirtualQuery(reinterpret_cast<const void*>(record.ExceptionInformation[1]), &region, sizeof region) == 0 ||
        region.State != MEM_RESERVE || region.AllocationProtect != PAGE_EXECUTE_READWRITE)
        return EXCEPTION_CONTINUE_SEARCH;
    HMODULE writer = nullptr;
    if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                            static_cast<LPCWSTR>(record.ExceptionAddress), &writer) ||
        GetProcAddress(writer, "uc_open") == nullptr)
        return EXCEPTION_CONTINUE_SEARCH;
    constexpr std::size_t kUnicornStep = 0x400000;
    const std::size_t size = region.RegionSize < kUnicornStep ? region.RegionSize : kUnicornStep;
    return VirtualAlloc(region.BaseAddress, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE) ? EXCEPTION_CONTINUE_EXECUTION
                                                                                       : EXCEPTION_CONTINUE_SEARCH;
}
#endif

// The library and its table, built together on first use. They live to the end of the program: unloading an
// emulator library while another thread might still call into it is never worth the tidiness.
struct Loaded {
    DynLib lib;
    Api api;
    Loaded() : lib(DynLib::load_beside_exe("unicorn.dll")) {
#define CB_X(name) api.name = lib.function<decltype(&::name)>(#name);
        CB_UNICORN_FUNCTIONS(CB_X)
#undef CB_X
        commit_code_buffer_to_its_end();
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

void commit_code_buffer_to_its_end() {
#ifdef _WIN32
    static std::once_flag once;
    std::call_once(once, [] { AddVectoredExceptionHandler(1, commit_within_reservation); });
#endif
}

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
