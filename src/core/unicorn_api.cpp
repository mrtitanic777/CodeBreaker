#include "core/unicorn_api.hpp"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <deque>
#include <format>
#include <mutex>
#include <utility>

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

// Unicorn 2.1.4 on Windows gives each machine a page of its own: a stub at its start, registered as a vectored exception handler (at
// the end of the list), that calls Unicorn's handler with the machine. uc_close removes the handler and frees the page at once -- but
// every exception on any thread (a C++ throw is one) calls every vectored handler, and a thread the dispatcher has already sent to that
// stub then runs freed memory: the process dies with an access violation, executing the page's first byte, with nothing said (a check
// of a large module, whose judging threads open and close machines while others throw, did so now and then). So Unicorn's freeing of
// such a page is held back: the stub is made to return at once ("not mine", EXCEPTION_CONTINUE_SEARCH), and the page is freed only
// once it has been retired for kRetiredFor, long past any dispatch that could have seen the handler.
using VirtualFreeFn = BOOL(WINAPI*)(LPVOID, SIZE_T, DWORD);
VirtualFreeFn g_virtual_free = nullptr;
constexpr std::chrono::seconds kRetiredFor{5};
constexpr std::size_t kHandlerPage = 0x1000;
constexpr std::size_t kHandlerData = 0x800;   // (where the stub's `mov rax, imm64` points: the machine and Unicorn's handler)

bool is_handler_page(void* page) {
    MEMORY_BASIC_INFORMATION region{};
    if (VirtualQuery(page, &region, sizeof region) == 0 || region.AllocationBase != page || region.RegionSize != kHandlerPage ||
        region.State != MEM_COMMIT || region.Protect != PAGE_EXECUTE_READWRITE)
        return false;
    const auto* bytes = static_cast<const unsigned char*>(page);
    std::uint64_t data = 0;
    std::memcpy(&data, bytes + 2, sizeof data);
    return bytes[0] == 0x48 && bytes[1] == 0xb8 && data == reinterpret_cast<std::uintptr_t>(page) + kHandlerData;
}

BOOL WINAPI held_virtual_free(LPVOID address, SIZE_T size, DWORD type) {
    if (address == nullptr || size != 0 || type != MEM_RELEASE || !is_handler_page(address)) return g_virtual_free(address, size, type);
    static constexpr unsigned char kReturnAtOnce[] = {0x31, 0xc0, 0xc3};   // xor eax, eax; ret
    std::memcpy(address, kReturnAtOnce, sizeof kReturnAtOnce);
    FlushInstructionCache(GetCurrentProcess(), address, sizeof kReturnAtOnce);
    static std::mutex lock;
    static std::deque<std::pair<std::chrono::steady_clock::time_point, void*>> retired;
    const auto now = std::chrono::steady_clock::now();
    const std::lock_guard hold(lock);
    while (!retired.empty() && now - retired.front().first >= kRetiredFor) {
        g_virtual_free(retired.front().second, 0, MEM_RELEASE);
        retired.pop_front();
    }
    retired.emplace_back(now, address);
    return TRUE;
}

// Unicorn's import of VirtualFree pointed at held_virtual_free (its import table in its own image: nothing else is touched).
void hold_handler_pages(HMODULE unicorn) {
    const auto* base = reinterpret_cast<const unsigned char*>(unicorn);
    const auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(base);
    const auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(base + dos->e_lfanew);
    const IMAGE_DATA_DIRECTORY& imports = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    if (imports.VirtualAddress == 0) return;
    for (auto* d = reinterpret_cast<const IMAGE_IMPORT_DESCRIPTOR*>(base + imports.VirtualAddress); d->Name != 0; ++d) {
        if (_stricmp(reinterpret_cast<const char*>(base + d->Name), "kernel32.dll") != 0 || d->OriginalFirstThunk == 0) continue;
        const auto* names = reinterpret_cast<const IMAGE_THUNK_DATA*>(base + d->OriginalFirstThunk);
        auto* slots = reinterpret_cast<IMAGE_THUNK_DATA*>(const_cast<unsigned char*>(base) + d->FirstThunk);
        for (; names->u1.AddressOfData != 0; ++names, ++slots) {
            if (IMAGE_SNAP_BY_ORDINAL(names->u1.Ordinal)) continue;
            const auto* by_name = reinterpret_cast<const IMAGE_IMPORT_BY_NAME*>(base + names->u1.AddressOfData);
            if (std::strcmp(reinterpret_cast<const char*>(by_name->Name), "VirtualFree") != 0) continue;
            DWORD was = 0;
            if (!VirtualProtect(&slots->u1.Function, sizeof slots->u1.Function, PAGE_READWRITE, &was)) return;
            g_virtual_free = reinterpret_cast<VirtualFreeFn>(slots->u1.Function);
            slots->u1.Function = reinterpret_cast<ULONG_PTR>(&held_virtual_free);
            VirtualProtect(&slots->u1.Function, sizeof slots->u1.Function, was, &was);
            return;
        }
    }
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
#ifdef _WIN32
        HMODULE unicorn = nullptr;
        if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                               reinterpret_cast<LPCWSTR>(api.uc_open), &unicorn))
            hold_handler_pages(unicorn);
#endif
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
