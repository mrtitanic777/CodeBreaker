// uc.cc -- Unicorn's entry points in the DLL Python's `unicorn` package carries (LoadLibraryW of a
// path already loaded hands back that same module).
#include "uc.h"

#include <cstdio>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "bytes.h"

namespace cbf {

namespace {
UcLib UC;
bool loaded = false;

template <typename T>
bool get(HMODULE m, const char *name, T &slot, char *err, size_t errlen) {
    slot = reinterpret_cast<T>(GetProcAddress(m, name));
    if (!slot) snprintf(err, errlen, "no %s in unicorn.dll", name);
    return slot != nullptr;
}
}  // namespace

int load_unicorn(const wchar_t *path, char *err, size_t errlen) {
    HMODULE u = LoadLibraryW(path);
    if (!u) {
        snprintf(err, errlen, "cannot load unicorn.dll (error %lu)", GetLastError());
        return 1;
    }
    loaded = get(u, "uc_open", UC.open, err, errlen) && get(u, "uc_close", UC.close, err, errlen) &&
             get(u, "uc_reg_read", UC.reg_read, err, errlen) && get(u, "uc_reg_write", UC.reg_write, err, errlen) &&
             get(u, "uc_mem_read", UC.mem_read, err, errlen) && get(u, "uc_mem_write", UC.mem_write, err, errlen) &&
             get(u, "uc_mem_map", UC.mem_map, err, errlen) && get(u, "uc_emu_start", UC.emu_start, err, errlen) &&
             get(u, "uc_emu_stop", UC.emu_stop, err, errlen) && get(u, "uc_hook_add", UC.hook_add, err, errlen) &&
             get(u, "uc_strerror", UC.strerror, err, errlen);
    return loaded ? 0 : 1;
}

const UcLib &unicorn() {
    if (!loaded) raise("ImportError", "No module named 'unicorn'");
    return UC;
}

}  // namespace cbf
