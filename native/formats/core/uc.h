// uc.h -- Unicorn's C API, by the unicorn.dll Python's `unicorn` package carries (found by path,
// handed over once through cbf_unicorn), so an emulated run here executes as the Python one did.
// Nothing of Unicorn is linked: without the DLL a run raises the RuntimeError the caller reports.
#pragma once

#include <unicorn/unicorn.h>

namespace cbf {

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
    decltype(&uc_hook_add) hook_add = nullptr;
    decltype(&uc_strerror) strerror = nullptr;
};

// the loaded entry points; raises RuntimeError (Python's ImportError text) when there are none
const UcLib &unicorn();
// load the DLL at `path`: 0, or nonzero with `err` filled
int load_unicorn(const wchar_t *path, char *err, size_t errlen);

}  // namespace cbf
