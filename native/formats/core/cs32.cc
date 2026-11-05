// cs32.cc -- see cs32.h.
#include "cs32.h"

#include <capstone/capstone.h>

#include <mutex>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "bytes.h"

namespace cbf {

namespace {
typedef cs_err (*open_t)(cs_arch, cs_mode, csh *);
typedef cs_err (*option_t)(csh, cs_opt_type, size_t);
typedef size_t (*disasm_t)(csh, const uint8_t *, size_t, uint64_t, size_t, cs_insn **);
typedef void (*free_t)(cs_insn *, size_t);
open_t p_open;
option_t p_option;
disasm_t p_disasm;
free_t p_free;
bool loaded;
std::mutex mu;

void load(const std::wstring &dll) {
    std::lock_guard<std::mutex> g(mu);
    if (loaded) return;
    HMODULE m = LoadLibraryW(dll.c_str());
    if (!m) raise("ImportError", "cannot load capstone.dll (error " + std::to_string(GetLastError()) + ")");
    p_open = (open_t)GetProcAddress(m, "cs_open");
    p_option = (option_t)GetProcAddress(m, "cs_option");
    p_disasm = (disasm_t)GetProcAddress(m, "cs_disasm");
    p_free = (free_t)GetProcAddress(m, "cs_free");
    if (!p_open || !p_option || !p_disasm || !p_free) raise("ImportError", "capstone.dll lacks its entry points");
    loaded = true;
}

csh handle(bool detail) {                       // one per thread and kind, as platforms.decoder keeps them
    thread_local csh h[2] = {0, 0};
    if (!h[detail]) {
        if (p_open(CS_ARCH_X86, CS_MODE_32, &h[detail]) != CS_ERR_OK) raise("RuntimeError", "cs_open failed");
        p_option(h[detail], CS_OPT_DETAIL, detail ? CS_OPT_ON : CS_OPT_OFF);
    }
    return h[detail];
}
}  // namespace

std::vector<CsIns> cs32_disasm(const std::wstring &dll, const uint8_t *code, size_t n, int64_t address, size_t count,
                               bool detail) {
    load(dll);
    std::vector<CsIns> out;
    if (n == 0) return out;
    cs_insn *all = nullptr;
    size_t k = p_disasm(handle(detail), code, n, static_cast<uint64_t>(address), count, &all);
    out.resize(k);
    for (size_t i = 0; i < k; i++) {
        CsIns &o = out[i];
        o.address = static_cast<int64_t>(all[i].address);
        o.size = all[i].size;
        o.mnemonic = all[i].mnemonic;
        o.op_str = all[i].op_str;
        if (detail && all[i].detail) {
            const cs_x86 &x = all[i].detail->x86;
            if (x.op_count > 0 && x.operands[0].type == X86_OP_IMM) {
                o.imm0 = true;
                o.imm = x.operands[0].imm;
            }
        }
    }
    if (all) p_free(all, k);
    return out;
}

}  // namespace cbf
