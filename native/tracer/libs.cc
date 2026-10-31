// libs.cc -- find Unicorn's and Capstone's entry points in the DLLs Python loaded.
#include "libs.h"

#include <cstdio>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace cbt {

UcLib UC;
CsLib CS;

template <typename T>
static bool get(HMODULE m, const char *name, T &slot, char *err, size_t errlen) {
    slot = reinterpret_cast<T>(GetProcAddress(m, name));
    if (!slot)
        snprintf(err, errlen, "no %s in the DLL", name);
    return slot != nullptr;
}

bool load_libs(const wchar_t *unicorn_dll, const wchar_t *capstone_dll, char *err, size_t errlen) {
    // LoadLibraryW of a path already loaded hands back that same module: the Unicorn whose
    // engines Python made.
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
    return get(u, "uc_reg_read", UC.reg_read, err, errlen) && get(u, "uc_reg_write", UC.reg_write, err, errlen) &&
           get(u, "uc_mem_read", UC.mem_read, err, errlen) && get(u, "uc_mem_write", UC.mem_write, err, errlen) &&
           get(u, "uc_mem_map", UC.mem_map, err, errlen) && get(u, "uc_mem_unmap", UC.mem_unmap, err, errlen) &&
           get(u, "uc_emu_stop", UC.emu_stop, err, errlen) && get(u, "uc_hook_add", UC.hook_add, err, errlen) &&
           get(u, "uc_hook_del", UC.hook_del, err, errlen) &&
           get(c, "cs_open", CS.open, err, errlen) && get(c, "cs_option", CS.option, err, errlen) &&
           get(c, "cs_disasm", CS.disasm, err, errlen) && get(c, "cs_free", CS.free, err, errlen) &&
           get(c, "cs_reg_name", CS.reg_name, err, errlen) && get(c, "cs_regs_access", CS.regs_access, err, errlen);
}

}  // namespace cbt
