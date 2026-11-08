// codebreaker/exact/emu/dpmi.cpp -- C++ port of codebreaker.exact.emu.dpmi (see dpmi.hpp).

#include "codebreaker/exact/emu/dpmi.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <unicorn/unicorn.h>
#include <unicorn/x86.h>

namespace codebreaker::exact::emu::dpmi {

namespace {

using Desc = std::tuple<int64_t, int64_t, int64_t>;
using Vec = std::tuple<int64_t, int64_t>;

constexpr int64_t UC_PROT_ALL_ = 7;   // UC_PROT_ALL

// os.environ.get("DOSRUN_DEBUG") is truthy
bool debug_on() {
    const char* e = std::getenv("DOSRUN_DEBUG");
    return e != nullptr && *e != '\0';
}

// print(..., file=sys.stderr)
void debug_print(const py::str& s) { std::cerr << s.utf8() << "\n"; }

// struct.pack("<H", v) as a std::string
std::string H(int64_t v) { return py::pack("<H", v).raw(); }

// b"..." literal of byte values
std::string B(std::initializer_list<int> bytes_) {
    std::string s;
    for (int b : bytes_) s.push_back(static_cast<char>(b));
    return s;
}

py::bytes join(std::initializer_list<std::string> parts) {
    std::string s;
    for (const std::string& p : parts) s += p;
    return py::bytes(s);
}

}  // namespace

// ---------------------------------------------------------------------------------------------
py::bytes _desc(int64_t base, int64_t limit, int64_t access, int64_t gran) {
    if (limit > 0xFFFFF) {
        limit >>= 12;
        gran |= 0x80;
    }
    // struct.pack("<Q", ...): every term is masked non-negative, the value may use bit 63
    uint64_t v = static_cast<uint64_t>(limit & 0xFFFF) | (static_cast<uint64_t>(base & 0xFFFFFF) << 16) |
                 (static_cast<uint64_t>(access & 0xFF) << 40) | (static_cast<uint64_t>((limit >> 16) & 0xF) << 48) |
                 (static_cast<uint64_t>(gran & 0xF0) << 48) | (static_cast<uint64_t>((base >> 24) & 0xFF) << 56);
    std::string out;
    for (int k = 0; k < 8; ++k) out.push_back(static_cast<char>((v >> (8 * k)) & 0xFF));
    return py::bytes(out);
}

// ---------------------------------------------------------------------------------------------
void Host::attach(Context& c) {
    ctx = &c;
    Engine& mu = ctx->mu();
    try {
        mu.mem_map(HOST, HOSTTOP - HOST, UC_PROT_ALL_);       // host tables + extended pool
    } catch (const UcError&) {
        // already mapped (a re-attach)
    }
    mu.mem_write(ENTRY, py::bytes(B({0xCD, 0xFF})));          // the mode-switch entry: INT FFh (we trap it)
    // ctx.st["desc"] = ctx.st.get("desc", {}): State::desc is always there
}

bool Host::handle(int64_t intno) {
    if (intno == 0xFF) {                                      // the mode-switch entry (our INT marker)
        _switch();
        return true;
    }
    if (intno == RAW_INT && pm) {                             // a raw mode switch (INT 31h 0306's entries)
        _raw_switch();
        return true;
    }
    if (pm && (ctx->R(UC_X86_REG_CR0) & 1)) {
        if (intno == DEF_INT) {                               // a host default handler: the host's own service
            _default_int();
            return true;
        }
        if (pm_vec.contains(intno) && intno != 0x31 && intno != RAW_INT && intno != 0xFF) {
            _dispatch(intno);                                 // the client's handler, as the CPU would call it
            return true;
        }
    }
    if (intno == 0x2F && (ctx->R(UC_X86_REG_AX) & 0xFFFF) == 0x1687) {
        _int2f_1687();
        return true;
    }
    if (intno == 0x31) {
        _int31();
        return true;
    }
    return false;
}

// -- INT 2Fh AX=1687h: here is a DPMI host --------------------------------------------------
void Host::_int2f_1687() {
    if (debug_on()) debug_print(py::str("dpmi 2f/1687"));
    ctx->W(UC_X86_REG_AX, 0x0000);        // 0: DPMI available
    ctx->W(UC_X86_REG_BX, 0x0001);        // bit 0: 32-bit programs supported
    ctx->W(UC_X86_REG_CX, 0x0003);        // 80386
    ctx->W(UC_X86_REG_DX, 0x005A);        // DPMI version 0.90
    ctx->W(UC_X86_REG_SI, 0x0000);        // paragraphs of host private data the client must provide: none
    ctx->W(UC_X86_REG_ES, ENTRY >> 4);    // ES:DI = the mode-switch entry
    ctx->W(UC_X86_REG_DI, ENTRY & 0xF);
}

// -- the real-mode -> protected-mode switch (the client FAR CALLs the entry) -----------------
void Host::_switch() {
    Engine& mu = ctx->mu();
    State& st = ctx->st();
    int64_t ss = ctx->R(UC_X86_REG_SS);
    int64_t sp = ctx->R(UC_X86_REG_SP);
    auto [ret_ip, ret_cs] = py::unpack_from<int64_t, int64_t>("<HH", mu.mem_read(ss * 16 + sp, 4));   // the FAR-CALL frame
    if (debug_on()) {
        debug_print(py::percent(py::str("dpmi switch from %04x:%04x ax=%04x ds=%04x es=%04x ss:sp=%04x:%04x"), ret_cs, ret_ip,
                                ctx->R(UC_X86_REG_AX), ctx->R(UC_X86_REG_DS), ctx->R(UC_X86_REG_ES), ss, sp));
    }
    int64_t ds = ctx->R(UC_X86_REG_DS);       // ES at entry is the host-data block (DPMI); the PSP is the executive's
    // the client is a 16-bit DPMI client: AX bit 0 = 0. (A 32-bit client sets bit 0; the extender
    // it loads then runs 32-bit, decoded via its own D=1 code descriptors -- handled the same way.)
    // Build the GDT (host stub + flat data + the LDT descriptor).
    py::bytearray gdt(py::bytes(std::string(8, '\0')));
    gdt += _desc(STUB, 0xFFFF, 0x9A, 0x00);                 // SEL_STUBCODE 0x08: 16-bit code at STUB
    gdt += _desc(0, 0xFFFFFFFF, 0x92, 0xC0);                // SEL_FLATDATA 0x10: flat 32-bit data
    gdt += _desc(LDT, LDT_COUNT * 8 - 1, 0x82, 0x00);       // SEL_LDT 0x18: the LDT
    gdt += _desc(0, 0xFFFF, 0x92, 0x00);                    // SEL_RMDATA 0x20: real-mode-compatible data
    struct SelDesc { int64_t sel, base, lim, acc; };
    for (const SelDesc& d : {SelDesc{SEL_STUBCODE, STUB, 0xFFFF, 0x9A}, SelDesc{SEL_FLATDATA, 0, 0xFFFFFFFF, 0x92},
                             SelDesc{SEL_RMDATA, 0, 0xFFFF, 0x92}}) {
        st.desc[d.sel] = Desc(d.base, d.lim, d.acc);         // (so lin() and the restart resolve them too)
    }
    client32 = (ctx->R(UC_X86_REG_AX) & 1) != 0;
    mu.mem_write(GDT, gdt.view());
    mu.mem_write(LDT, py::bytes(std::string(static_cast<size_t>(LDT_COUNT * 8), '\0')));
    ldt_used = py::dict<int64_t, Desc>();
    next_ldt = LDT_FIRST;
    // the client's initial selectors: code, data, stack, PSP -- each base = its real-mode seg<<4.
    int64_t sel_cs = _make_ldt(ret_cs * 16, 0xFFFF, 0x9A);     // 16-bit code
    int64_t sel_ds = _make_ldt(ds * 16, 0xFFFF, 0x92);
    int64_t sel_ss = _make_ldt(ss * 16, 0xFFFF, 0x92);
    int64_t sel_psp = _make_ldt(ctx->psp() * 16, 0xFF, 0x92);
    mu.reg_write_mmr(UC_X86_REG_GDTR, 0, GDT, py::len(gdt) - 1, 0);
    mu.reg_write_mmr(UC_X86_REG_LDTR, SEL_LDT, LDT, LDT_COUNT * 8 - 1, 0x82);
    // the protected-mode landing stub (at STUB, reached through SEL_STUBCODE): load the client's
    // data selectors, set the stack to just past the far-call frame, far-jump to the client.
    py::bytes stub = join({B({0x66, 0xB8}), H(sel_ds), B({0x00, 0x00}), B({0x8E, 0xD8}),           // mov eax,ds_sel; mov ds,ax
                           B({0x66, 0xB8}), H(sel_ss), B({0x00, 0x00}), B({0x8E, 0xD0}),           // mov eax,ss_sel; mov ss,ax
                           B({0xBC}), H((sp + 4) & 0xFFFF),                                        // mov sp, retsp
                           B({0x66, 0xB8}), H(sel_psp), B({0x00, 0x00}), B({0x8E, 0xC0}),          // mov eax,psp_sel; mov es,ax
                           B({0xEA}), H(ret_ip), H(sel_cs)});                                      // jmp far sel_cs:ret_ip
    mu.mem_write(STUB, stub);
    _write_raw_code();
    _write_vector_stubs();
    // the real-mode trampoline: cli ; set CR0.PE ; far-jump into the stub (the hardware switch).
    py::bytes tramp = join({B({0xFA}), B({0x0F, 0x20, 0xC0}), B({0x0C, 0x01}), B({0x0F, 0x22, 0xC0}),
                            B({0xEA}), H(0), H(SEL_STUBCODE)});
    mu.mem_write(TRAMP, tramp);
    pm = true;
    st.pm = true;
    // run the trampoline in real mode; it performs the switch itself.
    ctx->W(UC_X86_REG_CS, TRAMP >> 4);
    ctx->W(UC_X86_REG_IP, TRAMP & 0xF);
    st.again = true;
    mu.emu_stop();
}

// -- protected-mode interrupts: the client's handlers (0205) and the host's defaults -----------
// (base, limit, access, flags) of a selector, read from the descriptor table itself.
std::tuple<int64_t, int64_t, int64_t, int64_t> Host::_seg(int64_t sel) {
    int64_t tbl = (sel & 4) ? LDT : GDT;
    py::bytes raw = ctx->mu().mem_read(tbl + (sel & 0xFFF8), 8);
    uint64_t e = 0;
    for (int k = 0; k < 8; ++k) e |= static_cast<uint64_t>(raw.byte_(k)) << (8 * k);
    int64_t base = static_cast<int64_t>(((e >> 16) & 0xFFFFFF) | (((e >> 56) & 0xFF) << 24));
    int64_t limit = static_cast<int64_t>((e & 0xFFFF) | (((e >> 48) & 0xF) << 16));
    int64_t flags = static_cast<int64_t>(((e >> 52) & 0xF) << 4);   // G D/B L AVL, as the high nibble
    if (flags & 0x80) {
        limit = (limit << 12) | 0xFFF;
    }
    return {base, limit, static_cast<int64_t>((e >> 40) & 0xFF), flags};
}

// push words (16-bit client) or dwords (32-bit client) on the client's stack (SP or ESP by
// the stack segment's B bit).
void Host::_push(const py::list<int64_t>& values) {
    Engine& mu = ctx->mu();
    int64_t ss = ctx->R(UC_X86_REG_SS);
    auto [base, _lim, _acc, flags] = _seg(ss);
    bool big = (flags & 0x40) != 0;
    int64_t sp = big ? ctx->R(UC_X86_REG_ESP) : ctx->R(UC_X86_REG_SP);
    int64_t n = client32 ? 4 : 2;
    for (int64_t v : values) {
        sp = (sp - n) & (big ? 0xFFFFFFFF : 0xFFFF);
        mu.mem_write(base + sp, py::pack(n == 4 ? "<I" : "<H", v & (n == 4 ? 0xFFFFFFFF : 0xFFFF)));
    }
    if (big) {
        mu.reg_write(UC_X86_REG_ESP, sp);
    } else {
        mu.reg_write(UC_X86_REG_SP, sp);
    }
}

// INT n in protected mode with a client handler: push (E)FLAGS, CS, (E)IP and enter it with IF
// and TF clear -- an interrupt gate. (Unicorn takes a CS write inside the hook as a full
// descriptor load, D bit and all, and resumes there.)
void Host::_dispatch(int64_t intno) {
    Engine& mu = ctx->mu();
    auto [sel, off] = pm_vec.at(intno);
    int64_t fl = ctx->R(UC_X86_REG_EFLAGS);
    if (debug_on()) {
        debug_print(py::percent(py::str("dpmi int %02x ax=%04x from %04x:%08x -> %04x:%08x"), intno, ctx->R(UC_X86_REG_AX),
                                ctx->R(UC_X86_REG_CS), ctx->R(UC_X86_REG_EIP), sel, off));
    }
    _push(py::list<int64_t>{fl, ctx->R(UC_X86_REG_CS), ctx->R(UC_X86_REG_EIP)});
    mu.reg_write(UC_X86_REG_EFLAGS, fl & ~int64_t(0x300));
    mu.reg_write(UC_X86_REG_CS, sel);
    mu.reg_write(UC_X86_REG_EIP, off);
}

// INT FDh in a default-handler stub: the vector is the stub's; run the host's service for it
// (INT 21h straight to the executive, the rest as the executive does in real mode).
void Host::_default_int() {
    int64_t eip = ctx->R(UC_X86_REG_EIP);
    if (eip >= EXC_BASE) {
        throw py::RuntimeError(py::percent(py::str("DPMI: exception %02Xh reached the host's default"),
                                           py::floordiv(eip - EXC_BASE - 2, 4)));
    }
    int64_t n = py::floordiv(eip - DEF_BASE - 2, 4);
    if (debug_on()) debug_print(py::percent(py::str("dpmi default int %02x ax=%04x"), n, ctx->R(UC_X86_REG_AX)));
    ctx->dos_int(n);
}

void Host::_write_vector_stubs() {
    std::string iret = client32 ? B({0x66, 0xCF}) : B({0xCF});
    while (iret.size() < 2) iret.push_back(static_cast<char>(0x90));    // iret.ljust(2, b"\x90")
    std::string stubs;
    for (int n = 0; n < 256; ++n) stubs += B({0xCD, static_cast<int>(DEF_INT)}) + iret;
    ctx->mu().mem_write(STUB + DEF_BASE, py::bytes(stubs));
    std::string exc;
    for (int n = 0; n < 32; ++n) exc += B({0xCD, static_cast<int>(DEF_INT)}) + (client32 ? B({0x66, 0xCB}) : B({0xCB, 0x90}));
    ctx->mu().mem_write(STUB + EXC_BASE, py::bytes(exc));
}

// -- the raw mode switches (INT 31h 0306) ------------------------------------------------------
void Host::_write_raw_code() {
    Engine& mu = ctx->mu();
    auto o = [](int64_t a) { return H(a - HSEG * 16); };    // a real-mode offset in HSEG
    auto p = [](int64_t a) { return H(a); };
    // real mode -> protected mode: the trampoline, the save/restore retf, the entry marker
    mu.mem_write(RAW_TRAMP, join({B({0xFA, 0x0F, 0x20, 0xC0, 0x0C, 0x01, 0x0F, 0x22, 0xC0, 0xEA}), p(PM_LOAD), p(SEL_STUBCODE)}));
    mu.mem_write(RM_SAVE, py::bytes(B({0xCB})));
    mu.mem_write(RAW_RM_ENTRY, py::bytes(B({0xCD, static_cast<int>(RAW_INT)})));
    int64_t P = RM_PARAMS;                                          // +0 ds +2 es +4 ss +6 sp +8 ip +A cs +C fs/gs +10 eax
    mu.mem_write(RM_LOAD, join({
        B({0x2E, 0x8E, 0x1E}) + o(P + 0), B({0x2E, 0x8E, 0x06}) + o(P + 2),          // mov ds/es, cs:[..]
        B({0x2E, 0x8E, 0x16}) + o(P + 4), B({0x2E, 0x8B, 0x26}) + o(P + 6),          // mov ss, cs:[..] ; mov sp, cs:[..]
        B({0x2E, 0x8E, 0x26}) + o(P + 0xC), B({0x2E, 0x8E, 0x2E}) + o(P + 0xC),      // mov fs/gs, cs:[..]
        B({0x2E, 0x66, 0xA1}) + o(P + 0x10),                                         // mov eax, cs:[..]
        B({0x2E, 0xFF, 0x2E}) + o(P + 8)}));                                         // jmp far cs:[..]  (m16:16)
    // protected mode (offsets in the STUB code segment)
    int64_t Q = PM_PARAMS;                                          // +0 ds +2 es +4 ss +6 fs/gs +8 esp +C eip +10 cs +14 eax
    mu.mem_write(STUB + PM_LOAD, join({
        B({0x2E, 0x8E, 0x1E}) + p(Q + 0), B({0x2E, 0x8E, 0x06}) + p(Q + 2),
        B({0x2E, 0x8E, 0x16}) + p(Q + 4), B({0x2E, 0x66, 0x8B, 0x26}) + p(Q + 8),     // mov ss ; mov esp
        B({0x2E, 0x8E, 0x26}) + p(Q + 6), B({0x2E, 0x8E, 0x2E}) + p(Q + 6),
        B({0x2E, 0x66, 0xA1}) + p(Q + 0x14),
        B({0x2E, 0x66, 0xFF, 0x2E}) + p(Q + 0xC)}));                                  // jmp far cs:[..]  (m16:32)
    mu.mem_write(STUB + RAW_PM_ENTRY, py::bytes(B({0xCD, static_cast<int>(RAW_INT)})));
    mu.mem_write(STUB + PM_TO_RM, join({
        B({0xB8}) + p(SEL_RMDATA),
        B({0x8E, 0xD8, 0x8E, 0xC0, 0x8E, 0xE0, 0x8E, 0xE8, 0x8E, 0xD0}),              // ds es fs gs ss = real-mode-compatible
        B({0x0F, 0x20, 0xC0, 0x24, 0xFE, 0x0F, 0x22, 0xC0}),                           // CR0.PE = 0
        B({0xEA}) + o(RM_LOAD) + p(HSEG)}));                                           // jmp far HSEG:RM_LOAD (real mode)
    mu.mem_write(STUB + PM_SAVE, py::bytes(client32 ? B({0x66, 0xCB}) : B({0xCB})));
}

// the client jumped to a raw-switch entry: AX=DS CX=ES DX=SS (E)BX=(E)SP SI=CS (E)DI=(E)IP of
// the other mode. Fill that mode's parameter block and run the host's switch code.
void Host::_raw_switch() {
    Engine& mu = ctx->mu();
    State& st = ctx->st();
    int64_t ax = ctx->R(UC_X86_REG_AX) & 0xFFFF;
    int64_t cx = ctx->R(UC_X86_REG_CX) & 0xFFFF;
    int64_t dx = ctx->R(UC_X86_REG_DX) & 0xFFFF;
    int64_t si = ctx->R(UC_X86_REG_SI) & 0xFFFF;
    int64_t ebx = ctx->R(UC_X86_REG_EBX);
    int64_t edi = ctx->R(UC_X86_REG_EDI);
    int64_t eax = ctx->R(UC_X86_REG_EAX);
    if (ctx->R(UC_X86_REG_CR0) & 1) {                              // protected -> real
        if (debug_on()) {
            debug_print(py::percent(py::str("dpmi raw -> real %04x:%04x ss:sp=%04x:%04x ds=%04x es=%04x"), si, edi & 0xFFFF, dx,
                                    ebx & 0xFFFF, ax, cx));
        }
        mu.mem_write(RM_PARAMS, py::pack("<7H2xI", ax, cx, dx, ebx & 0xFFFF, edi & 0xFFFF, si, int64_t(0), eax));
        st.pm = false;
        mu.reg_write(UC_X86_REG_ESP, ebx & 0xFFFF);          // (real-mode code addresses [esp]: high word 0)
        mu.reg_write(UC_X86_REG_EIP, PM_TO_RM);               // (CS is already SEL_STUBCODE: the entry is in it)
    } else {                                                   // real -> protected
        if (debug_on()) {
            debug_print(py::percent(py::str("dpmi raw -> prot %04x:%08x ss:esp=%04x:%08x ds=%04x es=%04x"), si, edi, dx, ebx, ax, cx));
        }
        mu.mem_write(STUB + PM_PARAMS, py::pack("<4HIIH2xI", ax, cx, dx, int64_t(0), ebx, edi, si, eax));
        st.pm = true;
        ctx->W(UC_X86_REG_CS, RAW_TRAMP >> 4);
        ctx->W(UC_X86_REG_IP, RAW_TRAMP & 0xF);
    }
    st.again = true;
    mu.emu_stop();
}

// -- LDT descriptor bookkeeping --------------------------------------------------------------
// write an LDT descriptor, record it in st["desc"] (so lin() can resolve it), return the
// selector (index<<3 | TI=4).
int64_t Host::_make_ldt(int64_t base, int64_t limit, int64_t access, std::optional<int64_t> index_) {
    int64_t index;
    if (!index_.has_value()) {
        index = next_ldt;
        while (ldt_used.contains(index)) index += 1;
        next_ldt = index + 1;
    } else {
        index = *index_;
    }
    ldt_used[index] = Desc(base, limit, access);
    int64_t gran = limit > 0xFFFFF ? 0x80 : 0x00;
    if ((access & 0x08) && limit > 0xFFFF) {                   // a wide code selector is 32-bit (D=1)
        gran |= 0x40;
    }
    ctx->mu().mem_write(LDT + index * 8, _desc(base, limit, access, gran | ((gran & 0x80) ? 0x40 : 0)));
    int64_t sel = (index << 3) | 4;
    ctx->st().desc[sel & 0xFFFC] = Desc(base, limit, access);
    return sel;
}

int64_t Host::_ldt_index(int64_t sel) { return (sel & 0xFFF8) >> 3; }

// -- INT 31h ---------------------------------------------------------------------------------
void Host::_int31() {
    auto R = [this](int64_t r) { return ctx->R(r); };
    auto W = [this](int64_t r, int64_t v) { ctx->W(r, v); };
    auto carry = [this](bool on) { ctx->carry(on); };
    int64_t ax = R(UC_X86_REG_AX) & 0xFFFF;
    int64_t fn = ax;
    int64_t bx = R(UC_X86_REG_BX), cx = R(UC_X86_REG_CX), dx = R(UC_X86_REG_DX);
    int64_t si = R(UC_X86_REG_SI), di = R(UC_X86_REG_DI);
    Engine& mu = ctx->mu();
    if (debug_on()) {
        debug_print(py::percent(py::str("int31 ax=%04x bx=%04x cx=%04x dx=%04x si=%04x di=%04x"), ax, bx, cx, dx, si, di));
    }

    if (fn == 0x0000) {                                       // allocate CX descriptors (LDT)
        int64_t n = py::truthy(cx) ? cx : 1;
        int64_t base_idx = next_ldt;
        for (int64_t k = 0; k < n; ++k) {
            _make_ldt(0, 0, 0x92, base_idx + k);
        }
        next_ldt = base_idx + n;
        W(UC_X86_REG_AX, (base_idx << 3) | 4);
        carry(false);
        return;
    }
    if (fn == 0x000D) {                                       // allocate the specific descriptor BX (04h-7Ch)
        int64_t idx = _ldt_index(bx);
        if (!(bx & 4) || idx >= LDT_FIRST || ldt_used.contains(idx)) {
            W(UC_X86_REG_AX, ldt_used.contains(idx) ? 0x8022 : 0x8025);
            carry(true);
            return;
        }
        _make_ldt(0, 0, 0x92, idx);
        carry(false);
        return;
    }
    if (fn == 0x0001) {                                       // free descriptor BX
        ldt_used.pop(_ldt_index(bx), Desc(0, 0, 0));
        ctx->st().desc.pop(bx & 0xFFFC, Desc(0, 0, 0));
        carry(false);
        return;
    }
    if (fn == 0x0003) {                                       // selector increment value
        W(UC_X86_REG_AX, 8);
        carry(false);
        return;
    }
    if (fn == 0x0006) {                                       // get base of selector BX -> CX:DX
        int64_t base = _base(bx);
        W(UC_X86_REG_CX, (base >> 16) & 0xFFFF);
        W(UC_X86_REG_DX, base & 0xFFFF);
        carry(false);
        return;
    }
    if (fn == 0x0007) {                                       // set base of selector BX = CX:DX
        _set(bx, (cx << 16) | dx);
        carry(false);
        return;
    }
    if (fn == 0x0008) {                                       // set limit of selector BX = CX:DX (bytes)
        _set(bx, std::nullopt, (cx << 16) | dx);
        carry(false);
        return;
    }
    if (fn == 0x0009) {                                       // set access rights of selector BX = CL/CH
        _set(bx, std::nullopt, std::nullopt, cx & 0xFF);
        carry(false);
        return;
    }
    if (fn == 0x000A) {                                       // create a data alias of code selector BX
        auto [base, limit, _acc] = _get(bx);
        W(UC_X86_REG_AX, _make_ldt(base, limit, 0x92));
        carry(false);
        return;
    }
    if (fn == 0x000B) {                                       // get descriptor BX into ES:EDI (8 bytes)
        int64_t tbl = (bx & 4) ? LDT : GDT;                   // (a GDT selector the host gave: read it there)
        mu.mem_write(_lin_esedi(), mu.mem_read(tbl + (bx & 0xFFF8), 8));
        carry(false);
        return;
    }
    if (fn == 0x000C) {                                       // set descriptor BX from ES:EDI (8 bytes)
        py::bytes raw = mu.mem_read(_lin_esedi(), 8);
        if (debug_on()) debug_print(py::percent(py::str("  000C %04x <- %s"), bx, raw.hex()));
        int64_t idx = _ldt_index(bx);
        mu.mem_write(LDT + idx * 8, raw);
        uint64_t e = 0;
        for (int k = 0; k < 8; ++k) e |= static_cast<uint64_t>(raw.byte_(k)) << (8 * k);
        int64_t base = static_cast<int64_t>(((e >> 16) & 0xFFFFFF) | (((e >> 56) & 0xFF) << 24));
        int64_t limit = static_cast<int64_t>((e & 0xFFFF) | (((e >> 48) & 0xF) << 16));
        if ((e >> 55) & 1) {
            limit = (limit << 12) | 0xFFF;
        }
        ldt_used[idx] = Desc(base, limit, static_cast<int64_t>((e >> 40) & 0xFF));
        ctx->st().desc[bx & 0xFFFC] = Desc(base, limit, static_cast<int64_t>((e >> 40) & 0xFF));
        carry(false);
        return;
    }
    if (fn == 0x0100) {                                       // allocate BX paragraphs of DOS memory
        std::optional<int64_t> seg = ctx->alloc(bx);
        if (!seg.has_value()) {
            W(UC_X86_REG_AX, 8);
            W(UC_X86_REG_BX, ctx->largest());
            carry(true);
            return;
        }
        int64_t sel = _make_ldt(*seg * 16, py::truthy(bx) ? bx * 16 - 1 : 0, 0x92);
        W(UC_X86_REG_AX, *seg);                               // real-mode segment
        W(UC_X86_REG_DX, sel);                                // protected-mode selector
        carry(false);
        return;
    }
    if (fn == 0x0101 || fn == 0x0102) {                       // free / resize DOS memory (selector DX)
        int64_t base = std::get<0>(_get(dx));
        try {
            if (fn == 0x0101) {
                ctx->free(base >> 4);
                ldt_used.pop(_ldt_index(dx), Desc(0, 0, 0));
                ctx->st().desc.pop(dx & 0xFFFC, Desc(0, 0, 0));
            } else {
                std::optional<int64_t> got = ctx->resize(base >> 4, bx);
                if (got.has_value()) {
                    W(UC_X86_REG_AX, 8);
                    W(UC_X86_REG_BX, *got);
                    carry(true);
                    return;
                }
                _set(dx, std::nullopt, py::truthy(bx) ? bx * 16 - 1 : 0);
            }
        } catch (const ArenaError& e) {                       // (DOS error 7 or 9)
            W(UC_X86_REG_AX, e.code);
            carry(true);
            return;
        }
        carry(false);
        return;
    }
    if (fn == 0x0200 || fn == 0x0201 || fn == 0x0202 || fn == 0x0203 || fn == 0x0204 || fn == 0x0205) {
        _vectors(fn, bx, cx, dx);
        return;
    }
    if (fn == 0x0300) {                                       // simulate a real-mode interrupt
        _simulate_rm_int();
        return;
    }
    if (fn == 0x0305) {                                       // state save/restore: no state, a retf each
        W(UC_X86_REG_AX, 0);
        W(UC_X86_REG_BX, HSEG);
        W(UC_X86_REG_CX, RM_SAVE - HSEG * 16);
        W(UC_X86_REG_SI, SEL_STUBCODE);
        mu.reg_write(UC_X86_REG_EDI, PM_SAVE);
        carry(false);
        return;
    }
    if (fn == 0x0306) {                                       // raw mode-switch addresses
        W(UC_X86_REG_BX, HSEG);
        W(UC_X86_REG_CX, RAW_RM_ENTRY - HSEG * 16);
        W(UC_X86_REG_SI, SEL_STUBCODE);
        mu.reg_write(UC_X86_REG_EDI, RAW_PM_ENTRY);
        carry(false);
        return;
    }
    if (fn == 0x0800) {                                       // physical address mapping: no paging, identity
        carry(false);
        return;
    }
    if (fn == 0x0801) {
        carry(false);
        return;
    }
    if (fn == 0x0702 || fn == 0x0703) {                       // page paging hints / discard: no paging here
        carry(false);
        return;
    }
    if (fn == 0x0604) {                                       // page size: 4 KB
        W(UC_X86_REG_BX, 0);
        W(UC_X86_REG_CX, 0x1000);
        carry(false);
        return;
    }
    if (fn == 0x0900 || fn == 0x0901 || fn == 0x0902) {       // virtual interrupt state: AL = the old one
        int64_t old = vif;
        if (fn != 0x0902) {
            vif = fn == 0x0900 ? 0 : 1;
        }
        W(UC_X86_REG_AX, (ax & 0xFF00) | old);
        carry(false);
        return;
    }
    if (fn == 0x0002) {                                       // segment to descriptor: one per real-mode segment
        if (!segsel.contains(bx)) {
            segsel[bx] = _make_ldt(bx * 16, 0xFFFF, 0x92);
        }
        W(UC_X86_REG_AX, segsel.at(bx));
        carry(false);
        return;
    }
    if (fn == 0x0A00 || fn == 0x0401) {                       // vendor API entry; capabilities (a DPMI 1.0
        W(UC_X86_REG_AX, 0x8001);                             // call - this is a 0.90 host): unsupported
        carry(true);
        return;
    }
    if (fn == 0x0400) {                                       // get DPMI version
        W(UC_X86_REG_AX, 0x005A);                             // 0.90
        W(UC_X86_REG_BX, 0x0003);                             // 32-bit, V86... (bit0 = paged? report plain)
        W(UC_X86_REG_CX, 0x0003);                             // 80386
        W(UC_X86_REG_DX, 0x0870);                             // the PICs: master at INT 08h, slave at INT 70h
        carry(false);
        return;
    }
    if (fn == 0x0500) {                                       // get free memory information (0x30 bytes at ES:EDI)
        _mem_info();
        carry(false);
        return;
    }
    if (fn == 0x0501) {                                       // allocate a memory block (BX:CX bytes)
        int64_t size = (bx << 16) | cx;
        std::optional<int64_t> base = _pool_alloc(size);
        if (!base.has_value()) {
            carry(true);
            return;
        }
        int64_t h = next_handle;
        next_handle += 1;
        mem[h] = std::make_tuple(*base, size);
        W(UC_X86_REG_BX, (*base >> 16) & 0xFFFF);
        W(UC_X86_REG_CX, *base & 0xFFFF);
        W(UC_X86_REG_SI, (h >> 16) & 0xFFFF);
        W(UC_X86_REG_DI, h & 0xFFFF);
        carry(false);
        return;
    }
    if (fn == 0x0502) {                                       // free memory block (handle SI:DI)
        mem.pop((si << 16) | di, Vec(0, 0));
        carry(false);
        return;
    }
    if (fn == 0x0503) {                                       // resize: keep the base, accept new size
        int64_t h = (si << 16) | di;
        if (mem.contains(h)) {
            int64_t base = std::get<0>(mem.at(h));
            mem[h] = std::make_tuple(base, (bx << 16) | cx);
            W(UC_X86_REG_BX, (base >> 16) & 0xFFFF);
            W(UC_X86_REG_CX, base & 0xFFFF);
            carry(false);
        } else {
            carry(true);
        }
        return;
    }
    if (fn == 0x0600 || fn == 0x0601 || fn == 0x0602 || fn == 0x0603) {   // lock/unlock linear region: no paging here
        carry(false);
        return;
    }
    if (fn == 0x0E00 || fn == 0x0E01) {                       // 80x87: report present, accept set
        if (fn == 0x0E00) {
            W(UC_X86_REG_AX, 0x0045);
        }
        carry(false);
        return;
    }
    throw py::RuntimeError(py::percent(py::str("DPMI INT 31h AX=%04X not implemented"), fn));
}

// -- descriptor helpers ----------------------------------------------------------------------
std::tuple<int64_t, int64_t, int64_t> Host::_get(int64_t sel) {
    if (!(sel & 4)) {                                          // a GDT selector of the host's
        return ctx->st().desc.get(sel & 0xFFFC, Desc(0, 0, 0x92));
    }
    return ldt_used.get(_ldt_index(sel), Desc(0, 0, 0x92));
}

int64_t Host::_base(int64_t sel) { return std::get<0>(_get(sel)); }

void Host::_set(int64_t sel, std::optional<int64_t> base, std::optional<int64_t> limit, std::optional<int64_t> access) {
    int64_t idx = _ldt_index(sel);
    auto [b, l, a] = ldt_used.get(idx, Desc(0, 0, 0x92));
    if (base.has_value()) {
        b = *base & 0xFFFFFFFF;
    }
    if (limit.has_value()) {
        l = *limit & 0xFFFFFFFF;
    }
    if (access.has_value()) {
        a = *access & 0xFF;
    }
    int64_t gran = 0x00;
    if (l > 0xFFFFF) {
        gran = 0x80;
    }
    if ((a & 0x08) && l > 0xFFFF) {
        gran |= 0x40;                                          // a 32-bit code selector (D=1)
    }
    ldt_used[idx] = Desc(b, l, a);
    ctx->st().desc[sel & 0xFFFC] = Desc(b, l, a);
    ctx->mu().mem_write(LDT + idx * 8, _desc(b, l, a, gran));
}

int64_t Host::_lin_esedi() {
    int64_t es = ctx->R(UC_X86_REG_ES);
    int64_t edi = ctx->R(UC_X86_REG_EDI);
    return std::get<0>(ctx->st().desc.get(es & 0xFFFC, Desc(es * 16, 0, 0))) + edi;
}

std::optional<int64_t> Host::_pool_alloc(int64_t size) {
    size = (size + 0xFFF) & ~int64_t(0xFFF);                   // page-align
    if (pool + size > POOLTOP) {
        return std::nullopt;
    }
    int64_t base = pool;
    pool += size;
    return base;
}

void Host::_mem_info() {
    int64_t free = POOLTOP - pool;
    py::bytearray info = py::bytearray::zeros(0x30);
    py::pack_into("<I", info, 0x00, free);                     // largest available block (bytes)
    py::pack_into("<I", info, 0x04, free >> 12);               // max unlocked pages
    py::pack_into("<I", info, 0x08, free >> 12);               // max locked pages
    py::pack_into("<I", info, 0x0C, (POOLTOP - POOL) >> 12);
    py::pack_into("<I", info, 0x10, free >> 12);
    ctx->mu().mem_write(_lin_esedi(), info.view());
}

void Host::_vectors(int64_t fn, int64_t bx, int64_t cx, int64_t dx) {
    int64_t bl = bx & 0xFF;
    if (fn == 0x0200) {                                        // get real-mode interrupt vector BL -> CX:DX
        auto [seg, off] = ctx->st().vec.get(bl, Vec(0, 0));
        ctx->W(UC_X86_REG_CX, seg);
        ctx->W(UC_X86_REG_DX, off);
        ctx->carry(false);
        return;
    }
    if (fn == 0x0201) {                                        // set real-mode interrupt vector BL = CX:DX
        ctx->st().vec[bl] = Vec(cx, dx);
        ctx->carry(false);
        return;
    }
    if (fn == 0x0202 || fn == 0x0204) {                        // get exception / interrupt handler BL -> CX:(E)DX
        py::dict<int64_t, Vec>& tbl = fn == 0x0202 ? exc_vec : pm_vec;
        int64_t base = fn == 0x0202 ? EXC_BASE : DEF_BASE;
        auto [sel, off] = tbl.get(bl, Vec(SEL_STUBCODE, base + 4 * bl));
        ctx->W(UC_X86_REG_CX, sel);
        ctx->mu().reg_write(UC_X86_REG_EDX, off);
        ctx->carry(false);
        return;
    }
    py::dict<int64_t, Vec>& tbl = fn == 0x0203 ? exc_vec : pm_vec;
    int64_t base = fn == 0x0203 ? EXC_BASE : DEF_BASE;
    int64_t off = client32 ? ctx->R(UC_X86_REG_EDX) : dx;
    if (cx == SEL_STUBCODE && off == base + 4 * bl) {
        tbl.pop(bl, Vec(0, 0));                                // set back to the host's default
    } else {
        tbl[bl] = Vec(cx, off);
    }
    ctx->carry(false);
}

// -- INT 31h 0300: simulate a real-mode interrupt -> dos.run's int21 -------------------------
// BL = interrupt, ES:(E)DI -> the DPMI real-mode call structure. Load its real-mode register
// image and run dos.run's int21(). The structure's DS/ES are REAL-MODE segments; we point two
// scratch selectors (base = seg<<4) at them and leave the CPU in protected mode, so int21()'s
// lin() resolves exactly the linear byte the client's DOS transfer buffer occupies -- without
// ever loading a raw real-mode segment value into a protected-mode segment register.
void Host::_simulate_rm_int() {
    Engine& mu = ctx->mu();
    State& st = ctx->st();
    int64_t bl = ctx->R(UC_X86_REG_BX) & 0xFF;
    int64_t rmcs = _lin_esedi();                               // the call structure (ES:EDI)
    py::bytes s = mu.mem_read(rmcs, 0x32);
    auto [r_edi, r_esi, r_ebp, _res, r_ebx, r_edx, r_ecx, r_eax] =
        py::unpack_from<int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t>("<8I", s, 0);
    auto [r_flags, r_es, r_ds, r_fs, r_gs, r_ip, r_cs, r_sp, r_ss] =
        py::unpack_from<int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t>("<9H", s, 0x20);
    if (!scratch_ds.has_value()) {                             // two reusable scratch data selectors
        scratch_ds = _make_ldt(0, 0xFFFF, 0x92);
        scratch_es = _make_ldt(0, 0xFFFF, 0x92);
    }
    _set(*scratch_ds, r_ds * 16);
    _set(*scratch_es, r_es * 16);
    static const int64_t saved_regs[] = {UC_X86_REG_EAX, UC_X86_REG_EBX, UC_X86_REG_ECX, UC_X86_REG_EDX,
                                         UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP, UC_X86_REG_EFLAGS};
    std::vector<std::pair<int64_t, int64_t>> saved;
    for (int64_t r : saved_regs) saved.emplace_back(r, ctx->R(r));
    st.ov_ds = scratch_ds;                  // int21 reads ds/es from these (lin() base = seg<<4),
    st.ov_es = scratch_es;                  // NOT from the CPU segment registers (left untouched)
    st.out_ds = st.out_es = std::nullopt;   // a segment int21 returns (ES:BX of 35h/52h) lands here
    auto restore = [&]() {                  // (the Python's finally:)
        for (const auto& rv : saved) mu.reg_write(rv.first, rv.second);
        st.ov_ds = std::nullopt;
        st.ov_es = std::nullopt;
    };
    int64_t out_ax, out_bx, out_cx, out_dx, out_si, out_di, out_fl;
    try {
        mu.reg_write(UC_X86_REG_EAX, r_eax);
        mu.reg_write(UC_X86_REG_EBX, r_ebx);
        mu.reg_write(UC_X86_REG_ECX, r_ecx);
        mu.reg_write(UC_X86_REG_EDX, r_edx);
        mu.reg_write(UC_X86_REG_ESI, r_esi);
        mu.reg_write(UC_X86_REG_EDI, r_edi);
        mu.reg_write(UC_X86_REG_EBP, r_ebp);
        if (bl == 0x21) {
            ctx->int21();
        } else {
            throw py::RuntimeError(py::percent(py::str("DPMI 0300 reflect of INT %02Xh not implemented"), bl));
        }
        out_ax = ctx->R(UC_X86_REG_EAX) & 0xFFFF;
        out_bx = ctx->R(UC_X86_REG_EBX) & 0xFFFF;
        out_cx = ctx->R(UC_X86_REG_ECX) & 0xFFFF;
        out_dx = ctx->R(UC_X86_REG_EDX) & 0xFFFF;
        out_si = ctx->R(UC_X86_REG_ESI) & 0xFFFF;
        out_di = ctx->R(UC_X86_REG_EDI) & 0xFFFF;
        out_fl = ctx->R(UC_X86_REG_EFLAGS) & 0xFFFF;
    } catch (...) {
        restore();
        throw;
    }
    restore();
    // write the results back into the real-mode call structure
    py::bytearray rs{s};
    py::pack_into("<I", rs, 0x1C, out_ax);
    py::pack_into("<I", rs, 0x10, out_bx);
    py::pack_into("<I", rs, 0x18, out_cx);
    py::pack_into("<I", rs, 0x14, out_dx);
    py::pack_into("<I", rs, 0x04, out_si);
    py::pack_into("<I", rs, 0x00, out_di);
    py::pack_into("<H", rs, 0x20, out_fl);
    if (st.out_es.has_value()) {
        py::pack_into("<H", rs, 0x22, *st.out_es);
    }
    if (st.out_ds.has_value()) {
        py::pack_into("<H", rs, 0x24, *st.out_ds);
    }
    mu.mem_write(rmcs, rs.view());
    ctx->carry(false);
}

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
