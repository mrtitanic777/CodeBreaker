// libs.cpp -- the recorder's Unicorn and Capstone tables (see libs.hpp).
#include "cb/emulate/tracer/libs.hpp"

#include <atomic>
#include <cstdio>
#include <mutex>

#include <windows.h>

namespace cb::emulate::tracer {

namespace {

// The tables in force, set once: by use_libraries() before the first run, else from cb/core on first use. The pointers
// are read on every hook of every run, so the fast path is one acquire load.
struct Tables {
    UcLib uc;
    CsLib cs;
};
Tables g_tables;
std::atomic<const Tables*> g_in_force{nullptr};
std::mutex g_set_lock;

const Tables& in_force() {
    if (const Tables* t = g_in_force.load(std::memory_order_acquire)) return *t;
    std::lock_guard<std::mutex> hold(g_set_lock);
    if (const Tables* t = g_in_force.load(std::memory_order_acquire)) return *t;
    const auto& u = emu::unicorn::api();
    g_tables.uc = {u.uc_reg_read, u.uc_reg_write, u.uc_mem_read, u.uc_mem_write, u.uc_mem_map,
                   u.uc_mem_unmap, u.uc_emu_stop, u.uc_hook_add, u.uc_hook_del, u.uc_emu_start, u.uc_ctl};
    const auto& c = disasm::capstone::api();
    g_tables.cs = {c.cs_open, c.cs_option, c.cs_disasm, c.cs_free, c.cs_reg_name, c.cs_regs_access};
    g_in_force.store(&g_tables, std::memory_order_release);
    return g_tables;
}

template <typename T>
bool get(HMODULE m, const char* name, T& slot, char* err, std::size_t errlen) {
    slot = reinterpret_cast<T>(GetProcAddress(m, name));
    if (!slot) snprintf(err, errlen, "no %s in the DLL", name);
    return slot != nullptr;
}

}  // namespace

const UcLib& ucl() { return in_force().uc; }
const CsLib& csl() { return in_force().cs; }

bool use_libraries(const wchar_t* unicorn_dll, const wchar_t* capstone_dll, char* err, std::size_t errlen) {
    std::lock_guard<std::mutex> hold(g_set_lock);
    if (g_in_force.load(std::memory_order_acquire) != nullptr) return true;   // the first call counts
    // LoadLibraryW of a path already loaded hands back that same module: the Unicorn whose engines the caller made.
    HMODULE u = LoadLibraryW(unicorn_dll);
    if (!u) {
        snprintf(err, errlen, "cannot load unicorn.dll (error %lu)", GetLastError());
        return false;
    }
    HMODULE c = LoadLibraryW(capstone_dll);
    if (!c) {
        snprintf(err, errlen, "cannot load capstone.dll (error %lu)", GetLastError());
        return false;
    }
    Tables made;
    const bool ok = get(u, "uc_reg_read", made.uc.reg_read, err, errlen) && get(u, "uc_reg_write", made.uc.reg_write, err, errlen) &&
                    get(u, "uc_mem_read", made.uc.mem_read, err, errlen) && get(u, "uc_mem_write", made.uc.mem_write, err, errlen) &&
                    get(u, "uc_mem_map", made.uc.mem_map, err, errlen) && get(u, "uc_mem_unmap", made.uc.mem_unmap, err, errlen) &&
                    get(u, "uc_emu_stop", made.uc.emu_stop, err, errlen) && get(u, "uc_hook_add", made.uc.hook_add, err, errlen) &&
                    get(u, "uc_hook_del", made.uc.hook_del, err, errlen) && get(u, "uc_emu_start", made.uc.emu_start, err, errlen) &&
                    get(u, "uc_ctl", made.uc.ctl, err, errlen) && get(c, "cs_open", made.cs.open, err, errlen) &&
                    get(c, "cs_option", made.cs.option, err, errlen) && get(c, "cs_disasm", made.cs.disasm, err, errlen) &&
                    get(c, "cs_free", made.cs.free, err, errlen) && get(c, "cs_reg_name", made.cs.reg_name, err, errlen) &&
                    get(c, "cs_regs_access", made.cs.regs_access, err, errlen);
    if (!ok) return false;
    g_tables = made;
    g_in_force.store(&g_tables, std::memory_order_release);
    return true;
}

}  // namespace cb::emulate::tracer
