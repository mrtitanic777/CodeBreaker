// uc.cc -- Unicorn's entry points in the DLL Python loaded (LoadLibraryW of a path already loaded
// hands back that same module), so a run here uses the very Unicorn the Python executive uses.
#include "dosexec.h"

#include <cstdio>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace cbd {

UcLib UC;

template <typename T>
static bool get(HMODULE m, const char *name, T &slot, char *err, size_t errlen) {
    slot = reinterpret_cast<T>(GetProcAddress(m, name));
    if (!slot)
        snprintf(err, errlen, "no %s in unicorn.dll", name);
    return slot != nullptr;
}

bool load_unicorn(const wchar_t *dll, char *err, size_t errlen) {
    HMODULE u = LoadLibraryW(dll);
    if (!u) {
        snprintf(err, errlen, "cannot load unicorn.dll (error %lu)", GetLastError());
        return false;
    }
    return get(u, "uc_open", UC.open, err, errlen) && get(u, "uc_close", UC.close, err, errlen) &&
           get(u, "uc_reg_read", UC.reg_read, err, errlen) && get(u, "uc_reg_write", UC.reg_write, err, errlen) &&
           get(u, "uc_mem_read", UC.mem_read, err, errlen) && get(u, "uc_mem_write", UC.mem_write, err, errlen) &&
           get(u, "uc_mem_map", UC.mem_map, err, errlen) && get(u, "uc_mem_map_ptr", UC.mem_map_ptr, err, errlen) &&
           get(u, "uc_emu_start", UC.emu_start, err, errlen) && get(u, "uc_emu_stop", UC.emu_stop, err, errlen) &&
           get(u, "uc_hook_add", UC.hook_add, err, errlen) && get(u, "uc_strerror", UC.strerror, err, errlen);
}

}  // namespace cbd
