// codebreaker/exact/emu/dpmi.cpp -- what is left of the old port of codebreaker.exact.emu.dpmi: Unicorn by the DLL the
// Python package carries (see dpmi.hpp). The host is cb/exact/emu/dpmi.cpp.
//   COMPAT(py): remove with UnicornEngine when exact/emu/dos and build/native_dos are refactored.

#include "codebreaker/exact/emu/dpmi.hpp"

#include <string>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <unicorn/unicorn.h>
#include <unicorn/x86.h>

namespace codebreaker::exact::emu::dpmi {

// ---- Unicorn, by the DLL Python's package carries ---------------------------------------------------

namespace {
struct UcLib {
    decltype(&uc_open) open = nullptr;
    decltype(&uc_close) close = nullptr;
    decltype(&uc_reg_read) reg_read = nullptr;
    decltype(&uc_reg_write) reg_write = nullptr;
    decltype(&uc_mem_read) mem_read = nullptr;
    decltype(&uc_mem_write) mem_write = nullptr;
    decltype(&uc_mem_map) mem_map = nullptr;
    decltype(&uc_emu_start) emu_start = nullptr;
    decltype(&uc_emu_stop) emu_stop = nullptr;
    decltype(&uc_strerror) strerror = nullptr;
};
UcLib UC;
bool uc_loaded = false;

template <typename T> bool load_symbol(HMODULE m, const char* name, T& slot) {
    slot = reinterpret_cast<T>(GetProcAddress(m, name));
    return slot != nullptr;
}

const UcLib& unicorn() {
    if (!uc_loaded) throw py::RuntimeError("No module named 'unicorn'");
    return UC;
}

void check(uc_err e) {
    if (e != UC_ERR_OK) throw UcError(unicorn().strerror(e));
}
}  // namespace

void load_unicorn(const py::str& dll_path) {
    std::wstring w;
    for (char32_t c : dll_path.u32()) {
        if (c >= 0x10000) {
            w.push_back(static_cast<wchar_t>(0xD800 + ((c - 0x10000) >> 10)));
            w.push_back(static_cast<wchar_t>(0xDC00 + ((c - 0x10000) & 0x3FF)));
        } else {
            w.push_back(static_cast<wchar_t>(c));
        }
    }
    HMODULE u = LoadLibraryW(w.c_str());
    if (!u) throw py::RuntimeError("cannot load unicorn.dll (error " + std::to_string(GetLastError()) + ")");
    uc_loaded = load_symbol(u, "uc_open", UC.open) && load_symbol(u, "uc_close", UC.close) &&
                load_symbol(u, "uc_reg_read", UC.reg_read) && load_symbol(u, "uc_reg_write", UC.reg_write) &&
                load_symbol(u, "uc_mem_read", UC.mem_read) && load_symbol(u, "uc_mem_write", UC.mem_write) &&
                load_symbol(u, "uc_mem_map", UC.mem_map) && load_symbol(u, "uc_emu_start", UC.emu_start) &&
                load_symbol(u, "uc_emu_stop", UC.emu_stop) && load_symbol(u, "uc_strerror", UC.strerror);
    if (!uc_loaded) throw py::RuntimeError("unicorn.dll lacks an entry point");
}

UnicornEngine::UnicornEngine() {
    uc_engine* uc = nullptr;
    check(unicorn().open(UC_ARCH_X86, UC_MODE_16, &uc));
    uc_ = uc;
}

UnicornEngine::~UnicornEngine() {
    if (uc_ != nullptr) UC.close(static_cast<uc_engine*>(uc_));
}

int64_t UnicornEngine::reg_read(int64_t reg) {
    uint64_t v = 0;
    check(unicorn().reg_read(static_cast<uc_engine*>(uc_), static_cast<int>(reg), &v));
    return static_cast<int64_t>(v);
}

void UnicornEngine::reg_write(int64_t reg, int64_t value) {
    uint64_t v = static_cast<uint64_t>(value);
    check(unicorn().reg_write(static_cast<uc_engine*>(uc_), static_cast<int>(reg), &v));
}

void UnicornEngine::reg_write_mmr(int64_t reg, int64_t selector, int64_t base, int64_t limit, int64_t flags) {
    uc_x86_mmr m;
    m.selector = static_cast<uint16_t>(selector);
    m.base = static_cast<uint64_t>(base);
    m.limit = static_cast<uint32_t>(limit);
    m.flags = static_cast<uint32_t>(flags);
    check(unicorn().reg_write(static_cast<uc_engine*>(uc_), static_cast<int>(reg), &m));
}

std::tuple<int64_t, int64_t, int64_t, int64_t> UnicornEngine::reg_read_mmr(int64_t reg) {
    uc_x86_mmr m;
    m.selector = 0;
    m.base = 0;
    m.limit = 0;
    m.flags = 0;
    check(unicorn().reg_read(static_cast<uc_engine*>(uc_), static_cast<int>(reg), &m));
    return {m.selector, static_cast<int64_t>(m.base), m.limit, m.flags};
}

py::bytes UnicornEngine::mem_read(int64_t address, int64_t size) {
    std::string buf(static_cast<size_t>(size), '\0');
    check(unicorn().mem_read(static_cast<uc_engine*>(uc_), static_cast<uint64_t>(address), &buf[0], static_cast<size_t>(size)));
    return py::bytes(buf);
}

void UnicornEngine::mem_write(int64_t address, const py::bytes& data) {
    check(unicorn().mem_write(static_cast<uc_engine*>(uc_), static_cast<uint64_t>(address), data.raw().data(), data.raw().size()));
}

void UnicornEngine::mem_map(int64_t address, int64_t size, int64_t perms) {
    check(unicorn().mem_map(static_cast<uc_engine*>(uc_), static_cast<uint64_t>(address), static_cast<size_t>(size),
                            static_cast<uint32_t>(perms)));
}

void UnicornEngine::emu_stop() { check(unicorn().emu_stop(static_cast<uc_engine*>(uc_))); }

void UnicornEngine::emu_start(int64_t begin, int64_t until, int64_t timeout, int64_t count) {
    check(unicorn().emu_start(static_cast<uc_engine*>(uc_), static_cast<uint64_t>(begin), static_cast<uint64_t>(until),
                              static_cast<uint64_t>(timeout), static_cast<size_t>(count)));
}

}  // namespace codebreaker::exact::emu::dpmi
