// dpmi.cc -- the DPMI host (exact/emu/dpmi.py), so a DOS-extended tool (the Win95 DDK's Phar Lap
// TNT ML.EXE) runs on the same single UC_MODE_16 engine as the real-mode executive.
//
// INT 2Fh/1687h reports the host; the client's far call to the entry switches to protected mode the
// hardware way (a trampoline sets CR0.PE and far-jumps into a stub that loads the client's LDT
// selectors); INT 31h serves descriptors, DOS memory (on the MCB arena), vectors, real-mode
// interrupt simulation (back through the executive's INT 21h), the raw mode switches (host code at
// fixed addresses driven by parameter blocks), extended memory, paging and the virtual interrupt
// flag.  A protected-mode INT n with a client handler (0205h) enters it as an interrupt gate would.
// A selector is keyed by sel & 0xFFFC in the run's desc table, so the executive's lin() resolves a
// protected-mode address to the linear byte the real-mode side sees.
#include "dosexec.h"
#include "cbdos.h"

#include <cstdio>

#include <unicorn/x86.h>

namespace cbd {

static constexpr uint64_t ENTRY = 0x00000660;      // the INT-marker the mode-switch entry (ES:DI) points at
static constexpr uint64_t TRAMP = 0x00000680;      // the real-mode -> protected-mode trampoline
static constexpr uint64_t HOSTB = 0x00110000;      // host tables, just above the real-mode megabyte
static constexpr uint64_t GDT = HOSTB + 0x0000;
static constexpr uint64_t LDT = HOSTB + 0x0800;
static constexpr uint64_t STUB = HOSTB + 0x2000;   // the host's protected-mode code segment
static constexpr uint64_t POOL = 0x00200000;       // extended-memory pool (INT 31h 0501)
static constexpr uint64_t POOLTOP = 0x01000000;
static constexpr uint64_t HOSTTOP = POOLTOP;

static constexpr uint64_t SEL_STUBCODE = 0x08, SEL_FLATDATA = 0x10, SEL_LDT = 0x18, SEL_RMDATA = 0x20;

static constexpr uint64_t HSEG = 0x0060;           // the real-mode host segment
static constexpr uint64_t RAW_TRAMP = 0x000006A0;
static constexpr uint64_t RM_SAVE = 0x000006B8;
static constexpr uint64_t RAW_RM_ENTRY = 0x000006BC;
static constexpr uint64_t RM_LOAD = 0x000006C0;
static constexpr uint64_t RM_PARAMS = 0x00000700;
static constexpr uint64_t PM_LOAD = 0x0100;        // STUB offsets
static constexpr uint64_t PM_PARAMS = 0x0180;
static constexpr uint64_t RAW_PM_ENTRY = 0x0200;
static constexpr uint64_t PM_TO_RM = 0x0210;
static constexpr uint64_t PM_SAVE = 0x0240;
static constexpr int RAW_INT = 0xFE;
static constexpr uint64_t DEF_BASE = 0x0400;
static constexpr int DEF_INT = 0xFD;
static constexpr uint64_t EXC_BASE = 0x0800;
static constexpr int64_t LDT_FIRST = 16;
static constexpr int64_t LDT_COUNT = 512;

static Bytes desc8(uint64_t base, uint64_t limit, uint64_t access, uint64_t gran) {
    // one 8-byte descriptor; a limit past 1 MB is byte-granular and scaled to 4 KB pages
    if (limit > 0xFFFFF) {
        limit >>= 12;
        gran |= 0x80;
    }
    uint64_t q = (limit & 0xFFFF) | ((base & 0xFFFFFF) << 16) | ((access & 0xFF) << 40) |
                 (((limit >> 16) & 0xF) << 48) | ((gran & 0xF0) << 48) | (((base >> 24) & 0xFF) << 56);
    Bytes b(8);
    for (int i = 0; i < 8; i++) b[i] = uint8_t(q >> (8 * i));
    return b;
}

static void putle(Bytes &b, size_t at, uint64_t v, int n) {
    for (int i = 0; i < n; i++) b[at + i] = uint8_t(v >> (8 * i));
}

static uint64_t getle(const Bytes &b, size_t at, int n) {
    uint64_t v = 0;
    for (int i = 0; i < n; i++) v |= uint64_t(b[at + i]) << (8 * i);
    return v;
}

static Bytes w16(uint64_t v) { return Bytes{uint8_t(v), uint8_t(v >> 8)}; }

static Bytes cat(std::initializer_list<Bytes> parts) {
    Bytes o;
    for (const auto &p : parts) o.insert(o.end(), p.begin(), p.end());
    return o;
}

static int64_t floordiv(int64_t a, int64_t b) {
    int64_t q = a / b;
    if ((a % b != 0) && ((a < 0) != (b < 0))) q--;
    return q;
}

void Host::attach() {
    uc_err e = UC.mem_map(ctx.uc, HOSTB, HOSTTOP - HOSTB, UC_PROT_ALL);   // host tables + extended pool
    (void)e;                                                              // (already mapped: a re-attach)
    ctx.wr(ENTRY, Bytes{0xCD, 0xFF});                                     // the mode-switch entry: INT FFh
}

bool Host::handle(int intno) {
    if (intno == 0xFF) { switch_(); return true; }
    if (intno == RAW_INT && pm) { raw_switch(); return true; }
    if (pm && (ctx.R(UC_X86_REG_CR0) & 1)) {
        if (intno == DEF_INT) { default_int(); return true; }
        if (pm_vec.count(intno) && intno != 0x31 && intno != RAW_INT && intno != 0xFF) { dispatch(intno); return true; }
    }
    if (intno == 0x2F && (ctx.R(UC_X86_REG_AX) & 0xFFFF) == 0x1687) { int2f_1687(); return true; }
    if (intno == 0x31) { int31(); return true; }
    return false;
}

void Host::int2f_1687() {
    if (ctx.S.debug) fprintf(stderr, "dpmi 2f/1687\n");
    ctx.W(UC_X86_REG_AX, 0x0000);        // DPMI available
    ctx.W(UC_X86_REG_BX, 0x0001);        // 32-bit programs supported
    ctx.W(UC_X86_REG_CX, 0x0003);        // 80386
    ctx.W(UC_X86_REG_DX, 0x005A);        // DPMI 0.90
    ctx.W(UC_X86_REG_SI, 0x0000);        // no host private data
    ctx.W(UC_X86_REG_ES, ENTRY >> 4);    // ES:DI = the mode-switch entry
    ctx.W(UC_X86_REG_DI, ENTRY & 0xF);
}

void Host::switch_() {
    uint64_t ss = ctx.R(UC_X86_REG_SS), sp = ctx.R(UC_X86_REG_SP);
    Bytes fr = ctx.rd(ss * 16 + sp, 4);                                   // the FAR-CALL frame
    uint64_t ret_ip = getle(fr, 0, 2), ret_cs = getle(fr, 2, 2);
    if (ctx.S.debug)
        fprintf(stderr, "dpmi switch from %04llx:%04llx ax=%04llx ds=%04llx es=%04llx ss:sp=%04llx:%04llx\n",
                (unsigned long long)ret_cs, (unsigned long long)ret_ip, (unsigned long long)ctx.R(UC_X86_REG_AX),
                (unsigned long long)ctx.R(UC_X86_REG_DS), (unsigned long long)ctx.R(UC_X86_REG_ES),
                (unsigned long long)ss, (unsigned long long)sp);
    uint64_t ds = ctx.R(UC_X86_REG_DS);
    Bytes gdt(8, 0);
    for (const Bytes &d : {desc8(STUB, 0xFFFF, 0x9A, 0x00), desc8(0, 0xFFFFFFFF, 0x92, 0xC0),
                           desc8(LDT, LDT_COUNT * 8 - 1, 0x82, 0x00), desc8(0, 0xFFFF, 0x92, 0x00)})
        gdt.insert(gdt.end(), d.begin(), d.end());
    ctx.desc[SEL_STUBCODE] = Desc{STUB, 0xFFFF, 0x9A};
    ctx.desc[SEL_FLATDATA] = Desc{0, 0xFFFFFFFF, 0x92};
    ctx.desc[SEL_RMDATA] = Desc{0, 0xFFFF, 0x92};
    client32 = bool(ctx.R(UC_X86_REG_AX) & 1);
    ctx.wr(GDT, gdt);
    ctx.wr(LDT, Bytes(size_t(LDT_COUNT * 8), 0));
    ldt_used.clear();
    next_ldt = LDT_FIRST;
    uint64_t sel_cs = make_ldt(ret_cs * 16, 0xFFFF, 0x9A);
    uint64_t sel_ds = make_ldt(ds * 16, 0xFFFF, 0x92);
    uint64_t sel_ss = make_ldt(ss * 16, 0xFFFF, 0x92);
    uint64_t sel_psp = make_ldt(uint64_t(ctx.PSP) * 16, 0xFF, 0x92);
    uc_x86_mmr g = {0, GDT, uint32_t(gdt.size() - 1), 0};
    uc_err e = UC.reg_write(ctx.uc, UC_X86_REG_GDTR, &g);
    if (e != UC_ERR_OK) throw uc_error(e);
    uc_x86_mmr l = {uint16_t(SEL_LDT), LDT, uint32_t(LDT_COUNT * 8 - 1), 0x82};
    e = UC.reg_write(ctx.uc, UC_X86_REG_LDTR, &l);
    if (e != UC_ERR_OK) throw uc_error(e);
    // the protected-mode landing stub: the client's data selectors, the stack past the far-call frame,
    // a far jump to the client
    Bytes stub = cat({Bytes{0x66, 0xB8}, w16(sel_ds), Bytes{0, 0, 0x8E, 0xD8},
                      Bytes{0x66, 0xB8}, w16(sel_ss), Bytes{0, 0, 0x8E, 0xD0},
                      Bytes{0xBC}, w16((sp + 4) & 0xFFFF),
                      Bytes{0x66, 0xB8}, w16(sel_psp), Bytes{0, 0, 0x8E, 0xC0},
                      Bytes{0xEA}, w16(ret_ip), w16(sel_cs)});
    ctx.wr(STUB, stub);
    write_raw_code();
    write_vector_stubs();
    Bytes tramp = cat({Bytes{0xFA, 0x0F, 0x20, 0xC0, 0x0C, 0x01, 0x0F, 0x22, 0xC0, 0xEA}, w16(0), w16(SEL_STUBCODE)});
    ctx.wr(TRAMP, tramp);
    pm = true;
    ctx.pm = true;
    ctx.W(UC_X86_REG_CS, TRAMP >> 4);
    ctx.W(UC_X86_REG_IP, TRAMP & 0xF);
    ctx.again = true;
    ctx.stop();
}

void Host::seg(uint64_t sel, uint64_t &base, uint64_t &limit, uint64_t &access, uint64_t &flags) {
    uint64_t tbl = (sel & 4) ? LDT : GDT;
    uint64_t e = getle(ctx.rd(tbl + (sel & 0xFFF8), 8), 0, 8);
    base = ((e >> 16) & 0xFFFFFF) | (((e >> 56) & 0xFF) << 24);
    limit = (e & 0xFFFF) | (((e >> 48) & 0xF) << 16);
    flags = ((e >> 52) & 0xF) << 4;
    if (flags & 0x80) limit = (limit << 12) | 0xFFF;
    access = (e >> 40) & 0xFF;
}

void Host::push(const std::vector<uint64_t> &values) {
    uint64_t ss = ctx.R(UC_X86_REG_SS), base, lim, acc, flags;
    seg(ss, base, lim, acc, flags);
    bool big = flags & 0x40;
    uint64_t sp = big ? ctx.R(UC_X86_REG_ESP) : ctx.R(UC_X86_REG_SP);
    int n = client32 ? 4 : 2;
    uint64_t mask = big ? 0xFFFFFFFFull : 0xFFFFull;
    for (uint64_t v : values) {
        sp = (sp - uint64_t(n)) & mask;
        Bytes b(static_cast<size_t>(n));
        putle(b, 0, v & (n == 4 ? 0xFFFFFFFFull : 0xFFFFull), n);
        ctx.wr(base + sp, b);
    }
    ctx.set_reg(big ? UC_X86_REG_ESP : UC_X86_REG_SP, sp);
}

void Host::dispatch(int intno) {
    auto so = pm_vec[intno];
    uint64_t fl = ctx.R(UC_X86_REG_EFLAGS);
    if (ctx.S.debug)
        fprintf(stderr, "dpmi int %02x ax=%04llx from %04llx:%08llx -> %04llx:%08llx\n", intno,
                (unsigned long long)ctx.R(UC_X86_REG_AX), (unsigned long long)ctx.R(UC_X86_REG_CS),
                (unsigned long long)ctx.R(UC_X86_REG_EIP), (unsigned long long)so.first, (unsigned long long)so.second);
    push({fl, ctx.R(UC_X86_REG_CS), ctx.R(UC_X86_REG_EIP)});
    ctx.set_reg(UC_X86_REG_EFLAGS, fl & ~uint64_t(0x300));
    ctx.set_reg(UC_X86_REG_CS, so.first);
    ctx.set_reg(UC_X86_REG_EIP, so.second);
}

void Host::default_int() {
    int64_t eip = int64_t(ctx.R(UC_X86_REG_EIP));
    if (eip >= int64_t(EXC_BASE))
        throw runtime_error(fmt("DPMI: exception %02llXh reached the host's default",
                                (long long)floordiv(eip - int64_t(EXC_BASE) - 2, 4)));
    int64_t n = floordiv(eip - int64_t(DEF_BASE) - 2, 4);
    if (ctx.S.debug) fprintf(stderr, "dpmi default int %02llx ax=%04llx\n", (long long)n, (unsigned long long)ctx.R(UC_X86_REG_AX));
    ctx.dos_int(int(n));
}

void Host::write_vector_stubs() {
    Bytes iret = client32 ? Bytes{0x66, 0xCF} : Bytes{0xCF, 0x90};
    Bytes stubs;
    for (int k = 0; k < 256; k++) {
        stubs.push_back(0xCD);
        stubs.push_back(uint8_t(DEF_INT));
        stubs.insert(stubs.end(), iret.begin(), iret.end());
    }
    ctx.wr(STUB + DEF_BASE, stubs);
    Bytes exc;
    for (int k = 0; k < 32; k++) {
        exc.push_back(0xCD);
        exc.push_back(uint8_t(DEF_INT));
        if (client32) { exc.push_back(0x66); exc.push_back(0xCB); }
        else { exc.push_back(0xCB); exc.push_back(0x90); }
    }
    ctx.wr(STUB + EXC_BASE, exc);
}

void Host::write_raw_code() {
    auto o = [](uint64_t a) { return w16(a - HSEG * 16); };   // a real-mode offset in HSEG
    auto p = [](uint64_t a) { return w16(a); };
    // real mode -> protected mode: the trampoline, the save/restore retf, the entry marker
    ctx.wr(RAW_TRAMP, cat({Bytes{0xFA, 0x0F, 0x20, 0xC0, 0x0C, 0x01, 0x0F, 0x22, 0xC0, 0xEA}, p(PM_LOAD), p(SEL_STUBCODE)}));
    ctx.wr(RM_SAVE, Bytes{0xCB});
    ctx.wr(RAW_RM_ENTRY, Bytes{0xCD, uint8_t(RAW_INT)});
    uint64_t P = RM_PARAMS;                       // +0 ds +2 es +4 ss +6 sp +8 ip +A cs +C fs/gs +10 eax
    ctx.wr(RM_LOAD, cat({Bytes{0x2E, 0x8E, 0x1E}, o(P + 0), Bytes{0x2E, 0x8E, 0x06}, o(P + 2),
                         Bytes{0x2E, 0x8E, 0x16}, o(P + 4), Bytes{0x2E, 0x8B, 0x26}, o(P + 6),
                         Bytes{0x2E, 0x8E, 0x26}, o(P + 0xC), Bytes{0x2E, 0x8E, 0x2E}, o(P + 0xC),
                         Bytes{0x2E, 0x66, 0xA1}, o(P + 0x10),
                         Bytes{0x2E, 0xFF, 0x2E}, o(P + 8)}));
    uint64_t Q = PM_PARAMS;                       // +0 ds +2 es +4 ss +6 fs/gs +8 esp +C eip +10 cs +14 eax
    ctx.wr(STUB + PM_LOAD, cat({Bytes{0x2E, 0x8E, 0x1E}, p(Q + 0), Bytes{0x2E, 0x8E, 0x06}, p(Q + 2),
                                Bytes{0x2E, 0x8E, 0x16}, p(Q + 4), Bytes{0x2E, 0x66, 0x8B, 0x26}, p(Q + 8),
                                Bytes{0x2E, 0x8E, 0x26}, p(Q + 6), Bytes{0x2E, 0x8E, 0x2E}, p(Q + 6),
                                Bytes{0x2E, 0x66, 0xA1}, p(Q + 0x14),
                                Bytes{0x2E, 0x66, 0xFF, 0x2E}, p(Q + 0xC)}));
    ctx.wr(STUB + RAW_PM_ENTRY, Bytes{0xCD, uint8_t(RAW_INT)});
    ctx.wr(STUB + PM_TO_RM, cat({Bytes{0xB8}, p(SEL_RMDATA),
                                 Bytes{0x8E, 0xD8, 0x8E, 0xC0, 0x8E, 0xE0, 0x8E, 0xE8, 0x8E, 0xD0},
                                 Bytes{0x0F, 0x20, 0xC0, 0x24, 0xFE, 0x0F, 0x22, 0xC0},
                                 Bytes{0xEA}, o(RM_LOAD), p(HSEG)}));
    ctx.wr(STUB + PM_SAVE, client32 ? Bytes{0x66, 0xCB} : Bytes{0xCB});
}

void Host::raw_switch() {
    // AX=DS CX=ES DX=SS (E)BX=(E)SP SI=CS (E)DI=(E)IP of the other mode
    uint64_t ax = ctx.R(UC_X86_REG_AX) & 0xFFFF, cx = ctx.R(UC_X86_REG_CX) & 0xFFFF;
    uint64_t dx = ctx.R(UC_X86_REG_DX) & 0xFFFF, si = ctx.R(UC_X86_REG_SI) & 0xFFFF;
    uint64_t ebx = ctx.R(UC_X86_REG_EBX), edi = ctx.R(UC_X86_REG_EDI), eax = ctx.R(UC_X86_REG_EAX);
    if (ctx.R(UC_X86_REG_CR0) & 1) {                           // protected -> real
        if (ctx.S.debug)
            fprintf(stderr, "dpmi raw -> real %04llx:%04llx ss:sp=%04llx:%04llx ds=%04llx es=%04llx\n",
                    (unsigned long long)si, (unsigned long long)(edi & 0xFFFF), (unsigned long long)dx,
                    (unsigned long long)(ebx & 0xFFFF), (unsigned long long)ax, (unsigned long long)cx);
        Bytes b(20, 0);
        putle(b, 0, ax, 2); putle(b, 2, cx, 2); putle(b, 4, dx, 2); putle(b, 6, ebx & 0xFFFF, 2);
        putle(b, 8, edi & 0xFFFF, 2); putle(b, 10, si, 2); putle(b, 12, 0, 2); putle(b, 16, eax & 0xFFFFFFFF, 4);
        ctx.wr(RM_PARAMS, b);
        ctx.pm = false;
        ctx.set_reg(UC_X86_REG_ESP, ebx & 0xFFFF);              // (real-mode code addresses [esp]: high word 0)
        ctx.set_reg(UC_X86_REG_EIP, PM_TO_RM);                  // (CS is already SEL_STUBCODE)
    } else {                                                    // real -> protected
        if (ctx.S.debug)
            fprintf(stderr, "dpmi raw -> prot %04llx:%08llx ss:esp=%04llx:%08llx ds=%04llx es=%04llx\n",
                    (unsigned long long)si, (unsigned long long)edi, (unsigned long long)dx,
                    (unsigned long long)ebx, (unsigned long long)ax, (unsigned long long)cx);
        Bytes b(0x18, 0);
        putle(b, 0, ax, 2); putle(b, 2, cx, 2); putle(b, 4, dx, 2); putle(b, 6, 0, 2);
        putle(b, 8, ebx & 0xFFFFFFFF, 4); putle(b, 0xC, edi & 0xFFFFFFFF, 4); putle(b, 0x10, si, 2); putle(b, 0x14, eax & 0xFFFFFFFF, 4);
        ctx.wr(STUB + PM_PARAMS, b);
        ctx.pm = true;
        ctx.W(UC_X86_REG_CS, RAW_TRAMP >> 4);
        ctx.W(UC_X86_REG_IP, RAW_TRAMP & 0xF);
    }
    ctx.again = true;
    ctx.stop();
}

uint64_t Host::make_ldt(uint64_t base, uint64_t limit, uint64_t access, std::optional<int64_t> index) {
    int64_t idx;
    if (!index) {
        idx = next_ldt;
        while (ldt_used.count(idx)) idx++;
        next_ldt = idx + 1;
    } else {
        idx = *index;
    }
    ldt_used[idx] = {base, limit, access};
    uint64_t gran = limit > 0xFFFFF ? 0x80 : 0x00;
    if ((access & 0x08) && limit > 0xFFFF) gran |= 0x40;      // a wide code selector is 32-bit (D=1)
    ctx.wr(LDT + uint64_t(idx) * 8, desc8(base, limit, access, gran | ((gran & 0x80) ? 0x40 : 0)));
    uint64_t sel = (uint64_t(idx) << 3) | 4;
    ctx.desc[sel & 0xFFFC] = Desc{base, limit, access};
    return sel;
}

std::tuple<uint64_t, uint64_t, uint64_t> Host::get(uint64_t sel) {
    if (!(sel & 4)) {                                           // a GDT selector of the host's
        auto it = ctx.desc.find(sel & 0xFFFC);
        if (it != ctx.desc.end()) return {it->second.base, it->second.limit, it->second.access};
        return {0, 0, 0x92};
    }
    auto it = ldt_used.find(ldt_index(sel));
    return it != ldt_used.end() ? it->second : std::tuple<uint64_t, uint64_t, uint64_t>(0, 0, 0x92);
}

void Host::set(uint64_t sel, std::optional<uint64_t> base, std::optional<uint64_t> limit, std::optional<uint64_t> access) {
    int64_t idx = ldt_index(sel);
    auto it = ldt_used.find(idx);
    auto [b, l, a] = it != ldt_used.end() ? it->second : std::tuple<uint64_t, uint64_t, uint64_t>(0, 0, 0x92);
    if (base) b = *base & 0xFFFFFFFF;
    if (limit) l = *limit & 0xFFFFFFFF;
    if (access) a = *access & 0xFF;
    uint64_t gran = 0x00;
    if (l > 0xFFFFF) gran = 0x80;
    if ((a & 0x08) && l > 0xFFFF) gran |= 0x40;               // a 32-bit code selector (D=1)
    ldt_used[idx] = {b, l, a};
    ctx.desc[sel & 0xFFFC] = Desc{b, l, a};
    ctx.wr(LDT + uint64_t(idx) * 8, desc8(b, l, a, gran));
}

uint64_t Host::lin_esedi() {
    uint64_t es = ctx.R(UC_X86_REG_ES), edi = ctx.R(UC_X86_REG_EDI);
    auto it = ctx.desc.find(es & 0xFFFC);
    return (it != ctx.desc.end() ? it->second.base : es * 16) + edi;
}

std::optional<uint64_t> Host::pool_alloc(uint64_t size) {
    size = (size + 0xFFF) & ~uint64_t(0xFFF);                  // page-align
    if (pool + size > POOLTOP) return std::nullopt;
    uint64_t base = pool;
    pool += size;
    return base;
}

void Host::mem_info() {
    uint64_t free_ = POOLTOP - pool;
    Bytes info(0x30, 0);
    putle(info, 0x00, free_, 4);                                 // largest available block (bytes)
    putle(info, 0x04, free_ >> 12, 4);
    putle(info, 0x08, free_ >> 12, 4);
    putle(info, 0x0C, (POOLTOP - POOL) >> 12, 4);
    putle(info, 0x10, free_ >> 12, 4);
    ctx.wr(lin_esedi(), info);
}

void Host::vectors(int fn, uint64_t bx, uint64_t cx, uint64_t dx) {
    int bl = int(bx & 0xFF);
    if (fn == 0x0200) {                                        // get real-mode interrupt vector BL -> CX:DX
        auto it = ctx.vec.find(bl);
        auto v = it != ctx.vec.end() ? it->second : std::pair<uint64_t, uint64_t>(0, 0);
        ctx.W(UC_X86_REG_CX, v.first);
        ctx.W(UC_X86_REG_DX, v.second);
        ctx.carry(false);
        return;
    }
    if (fn == 0x0201) {                                        // set real-mode interrupt vector BL = CX:DX
        ctx.vec[bl] = {cx, dx};
        ctx.carry(false);
        return;
    }
    if (fn == 0x0202 || fn == 0x0204) {                        // get exception / interrupt handler BL -> CX:(E)DX
        auto &tbl = fn == 0x0202 ? exc_vec : pm_vec;
        uint64_t base = fn == 0x0202 ? EXC_BASE : DEF_BASE;
        auto it = tbl.find(bl);
        auto v = it != tbl.end() ? it->second : std::pair<uint64_t, uint64_t>(SEL_STUBCODE, base + 4 * uint64_t(bl));
        ctx.W(UC_X86_REG_CX, v.first);
        ctx.set_reg(UC_X86_REG_EDX, v.second);
        ctx.carry(false);
        return;
    }
    auto &tbl = fn == 0x0203 ? exc_vec : pm_vec;
    uint64_t base = fn == 0x0203 ? EXC_BASE : DEF_BASE;
    uint64_t off = client32 ? ctx.R(UC_X86_REG_EDX) : dx;
    if (cx == SEL_STUBCODE && off == base + 4 * uint64_t(bl)) tbl.erase(bl);   // back to the host's default
    else tbl[bl] = {cx, off};
    ctx.carry(false);
}

void Host::simulate_rm_int() {
    // BL = interrupt, ES:(E)DI -> the real-mode call structure.  Its DS/ES are real-mode segments: two
    // scratch selectors (base = seg<<4) stand for them while the executive's INT 21h runs, the CPU
    // left in protected mode.
    int bl = int(ctx.R(UC_X86_REG_BX) & 0xFF);
    uint64_t rmcs = lin_esedi();
    Bytes s = ctx.rd(rmcs, 0x32);
    uint64_t r_edi = getle(s, 0, 4), r_esi = getle(s, 4, 4), r_ebp = getle(s, 8, 4), r_ebx = getle(s, 0x10, 4);
    uint64_t r_edx = getle(s, 0x14, 4), r_ecx = getle(s, 0x18, 4), r_eax = getle(s, 0x1C, 4);
    uint64_t r_es = getle(s, 0x22, 2), r_ds = getle(s, 0x24, 2);
    if (!scratch_ds) {
        scratch_ds = make_ldt(0, 0xFFFF, 0x92);
        scratch_es = make_ldt(0, 0xFFFF, 0x92);
    }
    set(*scratch_ds, r_ds * 16, std::nullopt, std::nullopt);
    set(*scratch_es, r_es * 16, std::nullopt, std::nullopt);
    static const int SAVED[] = {UC_X86_REG_EAX, UC_X86_REG_EBX, UC_X86_REG_ECX, UC_X86_REG_EDX,
                                UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP, UC_X86_REG_EFLAGS};
    uint64_t saved[8];
    for (int i = 0; i < 8; i++) saved[i] = ctx.R(SAVED[i]);
    ctx.ov_ds = *scratch_ds;
    ctx.ov_es = *scratch_es;
    ctx.out_ds.reset();
    ctx.out_es.reset();
    uint64_t out_ax, out_bx, out_cx, out_dx, out_si, out_di, out_fl;
    auto restore = [&] {
        for (int i = 0; i < 8; i++) ctx.set_reg(SAVED[i], saved[i]);
        ctx.ov_ds.reset();
        ctx.ov_es.reset();
    };
    try {
        ctx.set_reg(UC_X86_REG_EAX, r_eax);
        ctx.set_reg(UC_X86_REG_EBX, r_ebx);
        ctx.set_reg(UC_X86_REG_ECX, r_ecx);
        ctx.set_reg(UC_X86_REG_EDX, r_edx);
        ctx.set_reg(UC_X86_REG_ESI, r_esi);
        ctx.set_reg(UC_X86_REG_EDI, r_edi);
        ctx.set_reg(UC_X86_REG_EBP, r_ebp);
        if (bl == 0x21) ctx.int21();
        else throw runtime_error(fmt("DPMI 0300 reflect of INT %02Xh not implemented", bl));
        out_ax = ctx.R(UC_X86_REG_EAX) & 0xFFFF;
        out_bx = ctx.R(UC_X86_REG_EBX) & 0xFFFF;
        out_cx = ctx.R(UC_X86_REG_ECX) & 0xFFFF;
        out_dx = ctx.R(UC_X86_REG_EDX) & 0xFFFF;
        out_si = ctx.R(UC_X86_REG_ESI) & 0xFFFF;
        out_di = ctx.R(UC_X86_REG_EDI) & 0xFFFF;
        out_fl = ctx.R(UC_X86_REG_EFLAGS) & 0xFFFF;
    } catch (...) {
        restore();
        throw;
    }
    restore();
    Bytes rs = s;                                               // the results, back into the call structure
    putle(rs, 0x1C, out_ax, 4);
    putle(rs, 0x10, out_bx, 4);
    putle(rs, 0x18, out_cx, 4);
    putle(rs, 0x14, out_dx, 4);
    putle(rs, 0x04, out_si, 4);
    putle(rs, 0x00, out_di, 4);
    putle(rs, 0x20, out_fl, 2);
    if (ctx.out_es) putle(rs, 0x22, *ctx.out_es, 2);
    if (ctx.out_ds) putle(rs, 0x24, *ctx.out_ds, 2);
    ctx.wr(rmcs, rs);
    ctx.carry(false);
}

void Host::int31() {
    uint64_t ax = ctx.R(UC_X86_REG_AX) & 0xFFFF;
    int fn = int(ax);
    uint64_t bx = ctx.R(UC_X86_REG_BX), cx = ctx.R(UC_X86_REG_CX), dx = ctx.R(UC_X86_REG_DX);
    uint64_t si = ctx.R(UC_X86_REG_SI), di = ctx.R(UC_X86_REG_DI);
    auto W = [&](int r, uint64_t v) { ctx.W(r, v); };
    auto carry = [&](bool on) { ctx.carry(on); };
    if (ctx.S.debug)
        fprintf(stderr, "int31 ax=%04llx bx=%04llx cx=%04llx dx=%04llx si=%04llx di=%04llx\n", (unsigned long long)ax,
                (unsigned long long)bx, (unsigned long long)cx, (unsigned long long)dx, (unsigned long long)si,
                (unsigned long long)di);
    if (fn == 0x0000) {                                        // allocate CX descriptors (LDT)
        int64_t n = cx ? int64_t(cx) : 1;
        int64_t base_idx = next_ldt;
        for (int64_t k = 0; k < n; k++) make_ldt(0, 0, 0x92, base_idx + k);
        next_ldt = base_idx + n;
        W(UC_X86_REG_AX, (uint64_t(base_idx) << 3) | 4);
        carry(false);
        return;
    }
    if (fn == 0x000D) {                                        // allocate the specific descriptor BX (04h-7Ch)
        int64_t idx = ldt_index(bx);
        if (!(bx & 4) || idx >= LDT_FIRST || ldt_used.count(idx)) {
            W(UC_X86_REG_AX, ldt_used.count(idx) ? 0x8022 : 0x8025);
            carry(true);
            return;
        }
        make_ldt(0, 0, 0x92, idx);
        carry(false);
        return;
    }
    if (fn == 0x0001) {                                        // free descriptor BX
        ldt_used.erase(ldt_index(bx));
        ctx.desc.erase(bx & 0xFFFC);
        carry(false);
        return;
    }
    if (fn == 0x0003) { W(UC_X86_REG_AX, 8); carry(false); return; }
    if (fn == 0x0006) {                                        // get base of selector BX -> CX:DX
        uint64_t base = std::get<0>(get(bx));
        W(UC_X86_REG_CX, (base >> 16) & 0xFFFF);
        W(UC_X86_REG_DX, base & 0xFFFF);
        carry(false);
        return;
    }
    if (fn == 0x0007) { set(bx, (cx << 16) | dx, std::nullopt, std::nullopt); carry(false); return; }
    if (fn == 0x0008) { set(bx, std::nullopt, (cx << 16) | dx, std::nullopt); carry(false); return; }
    if (fn == 0x0009) { set(bx, std::nullopt, std::nullopt, cx & 0xFF); carry(false); return; }
    if (fn == 0x000A) {                                        // a data alias of code selector BX
        auto [base, limit, acc] = get(bx);
        (void)acc;
        W(UC_X86_REG_AX, make_ldt(base, limit, 0x92));
        carry(false);
        return;
    }
    if (fn == 0x000B) {                                        // get descriptor BX into ES:EDI
        uint64_t tbl = (bx & 4) ? LDT : GDT;
        Bytes d = ctx.rd(tbl + (bx & 0xFFF8), 8);
        ctx.wr(lin_esedi(), d);
        carry(false);
        return;
    }
    if (fn == 0x000C) {                                        // set descriptor BX from ES:EDI
        Bytes raw = ctx.rd(lin_esedi(), 8);
        if (ctx.S.debug) fprintf(stderr, "  000C %04llx <- %s\n", (unsigned long long)bx, hex_str(raw.data(), 8).c_str());
        int64_t idx = ldt_index(bx);
        ctx.wr(LDT + uint64_t(idx) * 8, raw);
        uint64_t e = getle(raw, 0, 8);
        uint64_t base = ((e >> 16) & 0xFFFFFF) | (((e >> 56) & 0xFF) << 24);
        uint64_t limit = (e & 0xFFFF) | (((e >> 48) & 0xF) << 16);
        if ((e >> 55) & 1) limit = (limit << 12) | 0xFFF;
        ldt_used[idx] = {base, limit, (e >> 40) & 0xFF};
        ctx.desc[bx & 0xFFFC] = Desc{base, limit, (e >> 40) & 0xFF};
        carry(false);
        return;
    }
    if (fn == 0x0100) {                                        // allocate BX paragraphs of DOS memory
        auto s = mcb_alloc(ctx, ctx.first_mcb, int64_t(bx), ctx.PSP);
        if (!s) {
            W(UC_X86_REG_AX, 8);
            W(UC_X86_REG_BX, uint64_t(mcb_largest(ctx, ctx.first_mcb)));
            carry(true);
            return;
        }
        uint64_t sel = make_ldt(uint64_t(*s) * 16, bx ? bx * 16 - 1 : 0, 0x92);
        W(UC_X86_REG_AX, uint64_t(*s));
        W(UC_X86_REG_DX, sel);
        carry(false);
        return;
    }
    if (fn == 0x0101 || fn == 0x0102) {                        // free / resize DOS memory (selector DX)
        uint64_t base = std::get<0>(get(dx));
        try {
            if (fn == 0x0101) {
                mcb_free(ctx, ctx.first_mcb, int64_t(base >> 4));
                ldt_used.erase(ldt_index(dx));
                ctx.desc.erase(dx & 0xFFFC);
            } else {
                auto got = mcb_resize(ctx, ctx.first_mcb, int64_t(base >> 4), int64_t(bx));
                if (got) {
                    W(UC_X86_REG_AX, 8);
                    W(UC_X86_REG_BX, uint64_t(*got));
                    carry(true);
                    return;
                }
                set(dx, std::nullopt, bx ? bx * 16 - 1 : 0, std::nullopt);
            }
        } catch (ArenaError &e) {
            W(UC_X86_REG_AX, uint64_t(e.code));
            carry(true);
            return;
        }
        carry(false);
        return;
    }
    if (fn >= 0x0200 && fn <= 0x0205) { vectors(fn, bx, cx, dx); return; }
    if (fn == 0x0300) { simulate_rm_int(); return; }
    if (fn == 0x0305) {                                        // state save/restore: no state, a retf each
        W(UC_X86_REG_AX, 0);
        W(UC_X86_REG_BX, HSEG);
        W(UC_X86_REG_CX, RM_SAVE - HSEG * 16);
        W(UC_X86_REG_SI, SEL_STUBCODE);
        ctx.set_reg(UC_X86_REG_EDI, PM_SAVE);
        carry(false);
        return;
    }
    if (fn == 0x0306) {                                        // raw mode-switch addresses
        W(UC_X86_REG_BX, HSEG);
        W(UC_X86_REG_CX, RAW_RM_ENTRY - HSEG * 16);
        W(UC_X86_REG_SI, SEL_STUBCODE);
        ctx.set_reg(UC_X86_REG_EDI, RAW_PM_ENTRY);
        carry(false);
        return;
    }
    if (fn == 0x0800 || fn == 0x0801 || fn == 0x0702 || fn == 0x0703) { carry(false); return; }
    if (fn == 0x0604) { W(UC_X86_REG_BX, 0); W(UC_X86_REG_CX, 0x1000); carry(false); return; }
    if (fn == 0x0900 || fn == 0x0901 || fn == 0x0902) {        // virtual interrupt state: AL = the old one
        int64_t old = vif;
        if (fn != 0x0902) vif = fn == 0x0900 ? 0 : 1;
        W(UC_X86_REG_AX, (ax & 0xFF00) | uint64_t(old));
        carry(false);
        return;
    }
    if (fn == 0x0002) {                                        // segment to descriptor: one per real-mode segment
        if (!segsel.count(bx)) segsel[bx] = make_ldt(bx * 16, 0xFFFF, 0x92);
        W(UC_X86_REG_AX, segsel[bx]);
        carry(false);
        return;
    }
    if (fn == 0x0A00 || fn == 0x0401) { W(UC_X86_REG_AX, 0x8001); carry(true); return; }
    if (fn == 0x0400) {                                        // version 0.90; the PICs at INT 08h / 70h
        W(UC_X86_REG_AX, 0x005A);
        W(UC_X86_REG_BX, 0x0003);
        W(UC_X86_REG_CX, 0x0003);
        W(UC_X86_REG_DX, 0x0870);
        carry(false);
        return;
    }
    if (fn == 0x0500) { mem_info(); carry(false); return; }
    if (fn == 0x0501) {                                        // allocate a memory block (BX:CX bytes)
        uint64_t size = (bx << 16) | cx;
        auto base = pool_alloc(size);
        if (!base) { carry(true); return; }
        int64_t h = next_handle++;
        mem[h] = {*base, size};
        W(UC_X86_REG_BX, (*base >> 16) & 0xFFFF);
        W(UC_X86_REG_CX, *base & 0xFFFF);
        W(UC_X86_REG_SI, (uint64_t(h) >> 16) & 0xFFFF);
        W(UC_X86_REG_DI, uint64_t(h) & 0xFFFF);
        carry(false);
        return;
    }
    if (fn == 0x0502) { mem.erase(int64_t((si << 16) | di)); carry(false); return; }
    if (fn == 0x0503) {                                        // resize: keep the base, take the new size
        int64_t h = int64_t((si << 16) | di);
        auto it = mem.find(h);
        if (it != mem.end()) {
            uint64_t base = it->second.first;
            it->second = {base, (bx << 16) | cx};
            W(UC_X86_REG_BX, (base >> 16) & 0xFFFF);
            W(UC_X86_REG_CX, base & 0xFFFF);
            carry(false);
        } else {
            carry(true);
        }
        return;
    }
    if (fn >= 0x0600 && fn <= 0x0603) { carry(false); return; }
    if (fn == 0x0E00 || fn == 0x0E01) {
        if (fn == 0x0E00) W(UC_X86_REG_AX, 0x0045);
        carry(false);
        return;
    }
    throw runtime_error(fmt("DPMI INT 31h AX=%04X not implemented", fn));
}

}  // namespace cbd
