#include "cb/exact/emu/dpmi.hpp"

#include <format>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"
#include "cb/core/log.hpp"

namespace cb::exact::emu::dpmi {

namespace {

// DPMI error codes (AX with the carry flag set).
constexpr std::uint16_t kErrDescriptorUnavailable = 0x8011;
constexpr std::uint16_t kErrInvalidSelector = 0x8022;
constexpr std::uint16_t kErrSelectorNotAllocatable = 0x8025;
constexpr std::uint16_t kErrUnsupported = 0x8001;
constexpr std::uint16_t kDosOutOfMemory = 8;

// Descriptor access bytes the host writes.
constexpr std::uint8_t kCode16 = 0x9A;     // present, ring 0, code, readable
constexpr std::uint8_t kData = 0x92;       // present, ring 0, data, writable
constexpr std::uint8_t kLdtSystem = 0x82;  // present, LDT
constexpr std::uint8_t kCodeBit = 0x08;    // the executable bit of an access byte

constexpr std::uint32_t kFlagCarry = 1;
constexpr std::uint32_t kFlagInterruptAndTrap = 0x300;   // IF and TF
constexpr std::uint32_t kRealModeCallBytes = 0x32;       // the DPMI real-mode call structure
constexpr std::uint8_t kLdtBit = 4;                      // selector bit 2: the descriptor is in the LDT

constexpr std::uint8_t kFlagBig = 0x40;   // D/B in a descriptor's flag nibble
constexpr std::uint8_t kFlagGranular = 0x80;

std::uint16_t make_selector(std::uint16_t ldt_index) { return static_cast<std::uint16_t>((ldt_index << 3) | kLdtBit); }
std::uint16_t ldt_index_of(std::uint16_t selector) { return static_cast<std::uint16_t>((selector & 0xFFF8) >> 3); }
std::uint16_t table_key(std::uint16_t selector) { return selector & 0xFFFC; }   // RPL stripped, the LDT bit kept

std::uint64_t le64(ByteView b) {
    io::ByteReader in(b, "dpmi descriptor");
    return in.u64();
}

std::uint32_t base_of(std::uint64_t e) {
    return static_cast<std::uint32_t>(((e >> 16) & 0xFFFFFF) | (((e >> 56) & 0xFF) << 24));
}

std::uint32_t limit_of(std::uint64_t e) {
    return static_cast<std::uint32_t>((e & 0xFFFF) | (((e >> 48) & 0xF) << 16));
}

std::uint8_t access_of(std::uint64_t e) { return static_cast<std::uint8_t>((e >> 40) & 0xFF); }

// the paragraph count of a block as the limit a descriptor over it has (0 paragraphs: a limit of 0)
std::uint32_t block_limit(std::uint16_t paragraphs) { return paragraphs ? static_cast<std::uint32_t>(paragraphs) * 16 - 1 : 0; }

void append_u16(Bytes& out, std::uint32_t v) {
    out.push_back(static_cast<std::uint8_t>(v & 0xFF));
    out.push_back(static_cast<std::uint8_t>((v >> 8) & 0xFF));
}

Bytes cat(std::initializer_list<ByteView> parts) {
    Bytes out;
    for (const ByteView p : parts) append(out, p);
    return out;
}

Bytes u16_bytes(std::uint32_t v) {
    Bytes out;
    append_u16(out, v);
    return out;
}

}  // namespace

Bytes descriptor(std::uint32_t base, std::uint32_t limit, std::uint8_t access, std::uint8_t granularity) {
    if (limit > 0xFFFFF) {   // byte-granular: the limit counts 4 KB pages
        limit >>= 12;
        granularity |= kFlagGranular;
    }
    const std::uint64_t q = (limit & 0xFFFFull) | (static_cast<std::uint64_t>(base & 0xFFFFFF) << 16) |
                            (static_cast<std::uint64_t>(access) << 40) | (static_cast<std::uint64_t>((limit >> 16) & 0xF) << 48) |
                            (static_cast<std::uint64_t>(granularity & 0xF0) << 48) | (static_cast<std::uint64_t>(base >> 24) << 56);
    io::ByteWriter out;
    out.u64(q);
    return out.take();
}

Host::Host(Context& ctx) : ctx_(ctx) {
    Engine& mu = ctx_.engine();
    mu.map_ram(kHost, kPoolTop - kHost);   // host tables and the extended-memory pool
    const Bytes marker{0xCD, 0xFF};        // the mode-switch entry: INT FFh, which the executive traps
    mu.write(kEntry, marker);
}

void Host::carry(bool on) {
    const std::uint32_t f = r(Reg::Eflags);
    w(Reg::Eflags, on ? (f | kFlagCarry) : (f & ~kFlagCarry));
}

bool Host::handle(std::uint8_t number) {
    if (number == 0xFF) {   // the mode-switch entry (our INT marker)
        switch_to_protected();
        return true;
    }
    if (number == kRawInt && protected_mode_) {   // a raw mode switch (the entries INT 31h 0306 hands out)
        raw_switch();
        return true;
    }
    if (protected_mode_ && (r(Reg::Cr0) & 1)) {
        if (number == kDefaultInt) {   // a host default handler: the host's own service
            default_interrupt();
            return true;
        }
        if (interrupt_handlers_.contains(number) && number != 0x31 && number != kRawInt && number != 0xFF) {
            dispatch(number);   // the client's handler, as the CPU would call it
            return true;
        }
    }
    if (number == 0x2F && (r(Reg::Ax) & 0xFFFF) == 0x1687) {
        int2f_1687();
        return true;
    }
    if (number == 0x31) {
        int31();
        return true;
    }
    return false;
}

// INT 2Fh AX=1687h: here is a DPMI host
void Host::int2f_1687() {
    log::debug("dpmi 2f/1687");
    w(Reg::Ax, 0x0000);   // 0: DPMI available
    w(Reg::Bx, 0x0001);   // bit 0: 32-bit programs supported
    w(Reg::Cx, 0x0003);   // an 80386
    w(Reg::Dx, 0x005A);   // DPMI version 0.90
    w(Reg::Si, 0x0000);   // paragraphs of host private data the client must provide: none
    w(Reg::Es, kEntry >> 4);   // ES:DI = the mode-switch entry
    w(Reg::Di, kEntry & 0xF);
}

// The real-mode to protected-mode switch (the client far-calls the entry).
void Host::switch_to_protected() {
    Engine& mu = ctx_.engine();
    State& st = ctx_.state();
    const std::uint32_t ss = r(Reg::Ss), sp = r(Reg::Sp);
    const Bytes frame_bytes = mu.read(ss * 16 + sp, 4);   // the far-call frame: the client's return address
    io::ByteReader frame(frame_bytes);
    const std::uint16_t ret_ip = frame.u16(), ret_cs = frame.u16();
    log::debug(std::format("dpmi switch from {:04x}:{:04x} ax={:04x} ds={:04x} es={:04x} ss:sp={:04x}:{:04x}", ret_cs, ret_ip,
                           r(Reg::Ax), r(Reg::Ds), r(Reg::Es), ss, sp));
    const std::uint32_t ds = r(Reg::Ds);   // (ES at entry is the host-data block; the PSP is the executive's)
    // The client is a 16-bit DPMI client unless AX bit 0 is set; a 32-bit one loads an extender that then runs 32-bit,
    // decoded through its own D=1 code descriptors (handled the same way).
    // The GDT: the null descriptor, the host stub, flat data, the LDT, and the real-mode-compatible data.
    Bytes gdt(8, 0);
    append(gdt, descriptor(kStub, 0xFFFF, kCode16, 0x00));                    // kSelStubCode: 16-bit code at kStub
    append(gdt, descriptor(0, 0xFFFFFFFF, kData, 0xC0));                      // kSelFlatData: flat 32-bit data
    append(gdt, descriptor(kLdt, kLdtCount * 8 - 1, kLdtSystem, 0x00));       // kSelLdt: the LDT
    append(gdt, descriptor(0, 0xFFFF, kData, 0x00));                          // kSelRmData: real-mode-compatible data
    // (so the executive's address resolution and the restart find them too)
    st.descriptors[kSelStubCode] = {kStub, 0xFFFF, kCode16};
    st.descriptors[kSelFlatData] = {0, 0xFFFFFFFF, kData};
    st.descriptors[kSelRmData] = {0, 0xFFFF, kData};
    client32_ = (r(Reg::Ax) & 1) != 0;
    mu.write(kGdt, gdt);
    mu.write(kLdt, Bytes(static_cast<std::size_t>(kLdtCount) * 8, 0));
    ldt_used_.clear();
    next_ldt_ = kLdtFirst;
    // The client's first selectors: code, data, stack and the PSP, each based at its real-mode segment * 16.
    const std::uint16_t sel_cs = make_ldt(ret_cs * 16u, 0xFFFF, kCode16);
    const std::uint16_t sel_ds = make_ldt(ds * 16, 0xFFFF, kData);
    const std::uint16_t sel_ss = make_ldt(ss * 16, 0xFFFF, kData);
    const std::uint16_t sel_psp = make_ldt(ctx_.psp() * 16u, 0xFF, kData);
    mu.set_table(TableReg::Gdtr, 0, kGdt, static_cast<std::uint32_t>(gdt.size() - 1), 0);
    mu.set_table(TableReg::Ldtr, kSelLdt, kLdt, kLdtCount * 8 - 1, kLdtSystem);
    // The protected-mode landing stub (at kStub, reached through kSelStubCode): load the client's data selectors, put
    // the stack just past the far-call frame, far-jump to the client.
    io::ByteWriter stub;
    const auto load_segment = [&stub](std::uint16_t selector, std::uint8_t mov_modrm) {   // mov eax, selector; mov <seg>, ax
        stub.u8(0x66);
        stub.u8(0xB8);
        stub.u16(selector);
        stub.u16(0);
        stub.u8(0x8E);
        stub.u8(mov_modrm);
    };
    load_segment(sel_ds, 0xD8);   // ds
    load_segment(sel_ss, 0xD0);   // ss
    stub.u8(0xBC);
    stub.u16(static_cast<std::uint16_t>(sp + 4));   // mov sp, the stack past the frame
    load_segment(sel_psp, 0xC0);                    // es
    stub.u8(0xEA);
    stub.u16(ret_ip);
    stub.u16(sel_cs);   // jmp far sel_cs:ret_ip
    mu.write(kStub, stub.data());
    write_raw_code();
    write_vector_stubs();
    // The real-mode trampoline: cli; set CR0.PE; far-jump into the stub (the hardware switch).
    const Bytes tramp = cat({Bytes{0xFA, 0x0F, 0x20, 0xC0, 0x0C, 0x01, 0x0F, 0x22, 0xC0, 0xEA}, u16_bytes(0), u16_bytes(kSelStubCode)});
    mu.write(kTrampoline, tramp);
    protected_mode_ = true;
    st.protected_mode = true;
    // Run the trampoline in real mode; it does the switch itself.
    w(Reg::Cs, kTrampoline >> 4);
    w(Reg::Ip, kTrampoline & 0xF);
    st.again = true;
    mu.stop();
}

// ---- protected-mode interrupts: the client's handlers (0205) and the host's defaults --------------------------------
Host::TableEntry Host::table_entry(std::uint16_t selector) {
    const std::uint32_t table = (selector & kLdtBit) ? kLdt : kGdt;
    const std::uint64_t e = le64(ctx_.engine().read(table + (selector & 0xFFF8u), 8));
    TableEntry out;
    out.segment = {base_of(e), limit_of(e), access_of(e)};
    out.flags = static_cast<std::uint8_t>(((e >> 52) & 0xF) << 4);   // G, D/B, L, AVL as the high nibble
    if (out.flags & kFlagGranular) out.segment.limit = (out.segment.limit << 12) | 0xFFF;
    return out;
}

// Push words (a 16-bit client) or dwords (a 32-bit one) on the client's stack: SP or ESP by the stack segment's B bit.
void Host::push_words(std::initializer_list<std::uint32_t> values) {
    Engine& mu = ctx_.engine();
    const TableEntry stack = table_entry(static_cast<std::uint16_t>(r(Reg::Ss)));
    const bool big = (stack.flags & kFlagBig) != 0;
    std::uint32_t sp = big ? r(Reg::Esp) : r(Reg::Sp);
    const std::uint32_t width = client32_ ? 4 : 2;
    const std::uint32_t mask = big ? 0xFFFFFFFFu : 0xFFFFu;
    for (const std::uint32_t v : values) {
        sp = (sp - width) & mask;
        io::ByteWriter item;
        if (width == 4) item.u32(v);
        else item.u16(static_cast<std::uint16_t>(v));
        mu.write(stack.segment.base + sp, item.data());
    }
    w(big ? Reg::Esp : Reg::Sp, sp);
}

// INT n in protected mode with a client handler: push (E)FLAGS, CS and (E)IP and enter it with IF and TF clear, as an
// interrupt gate would. (Unicorn takes a CS write inside the hook as a full descriptor load, D bit and all, and
// resumes there.)
void Host::dispatch(std::uint8_t number) {
    const Gate gate = interrupt_handlers_.at(number);
    const std::uint32_t flags = r(Reg::Eflags);
    log::debug(std::format("dpmi int {:02x} ax={:04x} from {:04x}:{:08x} -> {:04x}:{:08x}", number, r(Reg::Ax), r(Reg::Cs),
                           r(Reg::Eip), gate.selector, gate.offset));
    push_words({flags, r(Reg::Cs), r(Reg::Eip)});
    w(Reg::Eflags, flags & ~kFlagInterruptAndTrap);
    w(Reg::Cs, gate.selector);
    w(Reg::Eip, gate.offset);
}

// INT FDh in a default-handler stub: the vector is the stub's; run the host's service for it (INT 21h straight to the
// executive, the rest as the executive does them in real mode).
void Host::default_interrupt() {
    const std::uint32_t eip = r(Reg::Eip);
    if (eip >= kExceptionBase) {
        throw EmulationError(std::format("DPMI: exception {:02X}h reached the host's default", (eip - kExceptionBase - 2) / 4));
    }
    if (eip < kDefaultBase + 2) throw EmulationError(std::format("DPMI: INT FDh from {:#x}, which is no default handler of the host's", eip));
    const std::uint32_t n = (eip - kDefaultBase - 2) / 4;
    log::debug(std::format("dpmi default int {:02x} ax={:04x}", n, r(Reg::Ax)));
    ctx_.dos_int(static_cast<std::uint8_t>(n));
}

// One four-byte stub for every vector (INT FDh, then iret) and every exception (INT FDh, then retf).
void Host::write_vector_stubs() {
    io::ByteWriter vectors, exceptions;
    for (int n = 0; n < 256; ++n) {
        vectors.u8(0xCD);
        vectors.u8(kDefaultInt);
        if (client32_) {
            vectors.u8(0x66);
            vectors.u8(0xCF);
        } else {
            vectors.u8(0xCF);
            vectors.u8(0x90);
        }
    }
    for (int n = 0; n < 32; ++n) {
        exceptions.u8(0xCD);
        exceptions.u8(kDefaultInt);
        if (client32_) {
            exceptions.u8(0x66);
            exceptions.u8(0xCB);
        } else {
            exceptions.u8(0xCB);
            exceptions.u8(0x90);
        }
    }
    ctx_.engine().write(kStub + kDefaultBase, vectors.data());
    ctx_.engine().write(kStub + kExceptionBase, exceptions.data());
}

// ---- the raw mode switches (INT 31h 0306) ------------------------------------------------------------------------------
void Host::write_raw_code() {
    Engine& mu = ctx_.engine();
    // an offset in the real-mode host segment, and a plain word
    const auto o = [](std::uint32_t a) { return u16_bytes(a - kHostSegment * 16u); };
    const auto p = [](std::uint32_t a) { return u16_bytes(a); };
    // real mode to protected mode: the trampoline, the save/restore retf, the entry marker
    mu.write(kRawTrampoline, cat({Bytes{0xFA, 0x0F, 0x20, 0xC0, 0x0C, 0x01, 0x0F, 0x22, 0xC0, 0xEA}, p(kPmLoad), p(kSelStubCode)}));
    mu.write(kRmSave, Bytes{0xCB});
    mu.write(kRawRmEntry, Bytes{0xCD, kRawInt});
    const std::uint32_t P = kRmParams;   // +0 ds +2 es +4 ss +6 sp +8 ip +A cs +C fs/gs +10 eax
    mu.write(kRmLoad, cat({Bytes{0x2E, 0x8E, 0x1E}, o(P + 0), Bytes{0x2E, 0x8E, 0x06}, o(P + 2),     // mov ds/es, cs:[..]
                           Bytes{0x2E, 0x8E, 0x16}, o(P + 4), Bytes{0x2E, 0x8B, 0x26}, o(P + 6),     // mov ss, cs:[..]; mov sp, cs:[..]
                           Bytes{0x2E, 0x8E, 0x26}, o(P + 0xC), Bytes{0x2E, 0x8E, 0x2E}, o(P + 0xC), // mov fs/gs, cs:[..]
                           Bytes{0x2E, 0x66, 0xA1}, o(P + 0x10),                                     // mov eax, cs:[..]
                           Bytes{0x2E, 0xFF, 0x2E}, o(P + 8)}));                                     // jmp far cs:[..] (m16:16)
    // protected mode (offsets in the stub's code segment)
    const std::uint32_t Q = kPmParams;   // +0 ds +2 es +4 ss +6 fs/gs +8 esp +C eip +10 cs +14 eax
    mu.write(kStub + kPmLoad, cat({Bytes{0x2E, 0x8E, 0x1E}, p(Q + 0), Bytes{0x2E, 0x8E, 0x06}, p(Q + 2),
                                   Bytes{0x2E, 0x8E, 0x16}, p(Q + 4), Bytes{0x2E, 0x66, 0x8B, 0x26}, p(Q + 8),   // mov ss; mov esp
                                   Bytes{0x2E, 0x8E, 0x26}, p(Q + 6), Bytes{0x2E, 0x8E, 0x2E}, p(Q + 6),
                                   Bytes{0x2E, 0x66, 0xA1}, p(Q + 0x14),
                                   Bytes{0x2E, 0x66, 0xFF, 0x2E}, p(Q + 0xC)}));                                 // jmp far cs:[..] (m16:32)
    mu.write(kStub + kRawPmEntry, Bytes{0xCD, kRawInt});
    mu.write(kStub + kPmToRm, cat({Bytes{0xB8}, p(kSelRmData),
                                   Bytes{0x8E, 0xD8, 0x8E, 0xC0, 0x8E, 0xE0, 0x8E, 0xE8, 0x8E, 0xD0},   // ds es fs gs ss = real-mode compatible
                                   Bytes{0x0F, 0x20, 0xC0, 0x24, 0xFE, 0x0F, 0x22, 0xC0},               // CR0.PE = 0
                                   Bytes{0xEA}, o(kRmLoad), p(kHostSegment)}));                         // jmp far kHostSegment:kRmLoad (real mode)
    mu.write(kStub + kPmSave, client32_ ? Bytes{0x66, 0xCB} : Bytes{0xCB});
}

// The client jumped to a raw-switch entry: AX=DS CX=ES DX=SS (E)BX=(E)SP SI=CS (E)DI=(E)IP of the other mode. Fill
// that mode's parameter block and run the host's switch code.
void Host::raw_switch() {
    Engine& mu = ctx_.engine();
    State& st = ctx_.state();
    const std::uint16_t ax = static_cast<std::uint16_t>(r(Reg::Ax)), cx = static_cast<std::uint16_t>(r(Reg::Cx)),
                        dx = static_cast<std::uint16_t>(r(Reg::Dx)), si = static_cast<std::uint16_t>(r(Reg::Si));
    const std::uint32_t ebx = r(Reg::Ebx), edi = r(Reg::Edi), eax = r(Reg::Eax);
    io::ByteWriter block;
    if (r(Reg::Cr0) & 1) {   // protected to real
        log::debug(std::format("dpmi raw -> real {:04x}:{:04x} ss:sp={:04x}:{:04x} ds={:04x} es={:04x}", si, edi & 0xFFFF, dx, ebx & 0xFFFF, ax, cx));
        for (const std::uint16_t v : {ax, cx, dx, static_cast<std::uint16_t>(ebx), static_cast<std::uint16_t>(edi), si, std::uint16_t{0}}) block.u16(v);
        block.zeros(2);
        block.u32(eax);
        mu.write(kRmParams, block.data());
        st.protected_mode = false;
        w(Reg::Esp, ebx & 0xFFFF);   // (real-mode code addresses [esp]: the high word 0)
        w(Reg::Eip, kPmToRm);        // (CS is already the stub: the entry is in it)
    } else {                         // real to protected
        log::debug(std::format("dpmi raw -> prot {:04x}:{:08x} ss:esp={:04x}:{:08x} ds={:04x} es={:04x}", si, edi, dx, ebx, ax, cx));
        for (const std::uint16_t v : {ax, cx, dx, std::uint16_t{0}}) block.u16(v);
        block.u32(ebx);
        block.u32(edi);
        block.u16(si);
        block.zeros(2);
        block.u32(eax);
        mu.write(kStub + kPmParams, block.data());
        st.protected_mode = true;
        w(Reg::Cs, kRawTrampoline >> 4);
        w(Reg::Ip, kRawTrampoline & 0xF);
    }
    st.again = true;
    mu.stop();
}

// ---- LDT descriptor bookkeeping ---------------------------------------------------------------------------------------
// Write an LDT descriptor, record it for the executive's address resolution, and return its selector (index << 3 | TI).
std::uint16_t Host::make_ldt(std::uint32_t base, std::uint32_t limit, std::uint8_t access, std::optional<std::uint16_t> index) {
    std::uint16_t i;
    if (index) {
        i = *index;
    } else {
        i = next_ldt_;
        while (ldt_used_.contains(i)) ++i;
        next_ldt_ = static_cast<std::uint16_t>(i + 1);
    }
    if (i >= kLdtCount) throw EmulationError("DPMI: the LDT is full");
    ldt_used_[i] = {base, limit, access};
    std::uint8_t gran = limit > 0xFFFFF ? kFlagGranular : 0x00;
    if ((access & kCodeBit) && limit > 0xFFFF) gran |= kFlagBig;   // a wide code selector is 32-bit (D=1)
    ctx_.engine().write(kLdt + i * 8u, descriptor(base, limit, access, gran | ((gran & kFlagGranular) ? kFlagBig : 0)));
    const std::uint16_t selector = make_selector(i);
    ctx_.state().descriptors[table_key(selector)] = {base, limit, access};
    return selector;
}

SegmentInfo Host::get(std::uint16_t selector) {
    if (!(selector & kLdtBit)) {   // a GDT selector of the host's
        const auto found = ctx_.state().descriptors.find(table_key(selector));
        return found != ctx_.state().descriptors.end() ? found->second : SegmentInfo{0, 0, kData};
    }
    const auto found = ldt_used_.find(ldt_index_of(selector));
    return found != ldt_used_.end() ? found->second : SegmentInfo{0, 0, kData};
}

void Host::set(std::uint16_t selector, std::optional<std::uint32_t> base, std::optional<std::uint32_t> limit, std::optional<std::uint8_t> access) {
    const std::uint16_t index = ldt_index_of(selector);
    const auto found = ldt_used_.find(index);
    SegmentInfo seg = found != ldt_used_.end() ? found->second : SegmentInfo{0, 0, kData};
    if (base) seg.base = *base;
    if (limit) seg.limit = *limit;
    if (access) seg.access = *access;
    std::uint8_t gran = seg.limit > 0xFFFFF ? kFlagGranular : 0x00;
    if ((seg.access & kCodeBit) && seg.limit > 0xFFFF) gran |= kFlagBig;   // a 32-bit code selector (D=1)
    ldt_used_[index] = seg;
    ctx_.state().descriptors[table_key(selector)] = seg;
    ctx_.engine().write(kLdt + index * 8u, descriptor(seg.base, seg.limit, seg.access, gran));
}

// ---- INT 31h --------------------------------------------------------------------------------------------------------
void Host::int31() {
    Engine& mu = ctx_.engine();
    State& st = ctx_.state();
    const std::uint16_t ax = static_cast<std::uint16_t>(r(Reg::Ax));
    const std::uint16_t fn = ax;
    const std::uint16_t bx = static_cast<std::uint16_t>(r(Reg::Bx)), cx = static_cast<std::uint16_t>(r(Reg::Cx)),
                        dx = static_cast<std::uint16_t>(r(Reg::Dx)), si = static_cast<std::uint16_t>(r(Reg::Si)),
                        di = static_cast<std::uint16_t>(r(Reg::Di));
    log::debug(std::format("int31 ax={:04x} bx={:04x} cx={:04x} dx={:04x} si={:04x} di={:04x}", ax, bx, cx, dx, si, di));
    const auto fail = [&](std::uint16_t code) {
        w(Reg::Ax, code);
        carry(true);
    };

    switch (fn) {
        case 0x0000: {   // allocate CX descriptors (LDT)
            const std::uint32_t n = cx ? cx : 1;
            const std::uint32_t first = next_ldt_;
            if (first + n > kLdtCount) {
                fail(kErrDescriptorUnavailable);
                return;
            }
            for (std::uint32_t k = 0; k < n; ++k) make_ldt(0, 0, kData, static_cast<std::uint16_t>(first + k));
            next_ldt_ = static_cast<std::uint16_t>(first + n);
            w(Reg::Ax, make_selector(static_cast<std::uint16_t>(first)));
            carry(false);
            return;
        }
        case 0x000D: {   // allocate the specific descriptor BX (04h-7Ch)
            const std::uint16_t index = ldt_index_of(bx);
            if (!(bx & kLdtBit) || index >= kLdtFirst || ldt_used_.contains(index)) {
                fail(ldt_used_.contains(index) ? kErrInvalidSelector : kErrSelectorNotAllocatable);
                return;
            }
            make_ldt(0, 0, kData, index);
            carry(false);
            return;
        }
        case 0x0001:   // free descriptor BX
            ldt_used_.erase(ldt_index_of(bx));
            st.descriptors.erase(table_key(bx));
            carry(false);
            return;
        case 0x0003:   // selector increment value
            w(Reg::Ax, 8);
            carry(false);
            return;
        case 0x0006: {   // get the base of selector BX into CX:DX
            const std::uint32_t base = get(bx).base;
            w(Reg::Cx, base >> 16);
            w(Reg::Dx, base & 0xFFFF);
            carry(false);
            return;
        }
        case 0x0007:   // set the base of selector BX from CX:DX
            set(bx, (static_cast<std::uint32_t>(cx) << 16) | dx, std::nullopt, std::nullopt);
            carry(false);
            return;
        case 0x0008:   // set the limit of selector BX from CX:DX (bytes)
            set(bx, std::nullopt, (static_cast<std::uint32_t>(cx) << 16) | dx, std::nullopt);
            carry(false);
            return;
        case 0x0009:   // set the access rights of selector BX from CL/CH
            set(bx, std::nullopt, std::nullopt, static_cast<std::uint8_t>(cx & 0xFF));
            carry(false);
            return;
        case 0x000A: {   // create a data alias of code selector BX
            const SegmentInfo seg = get(bx);
            w(Reg::Ax, make_ldt(seg.base, seg.limit, kData));
            carry(false);
            return;
        }
        case 0x000B: {   // get descriptor BX into ES:EDI (8 bytes)
            const std::uint32_t table = (bx & kLdtBit) ? kLdt : kGdt;   // (a GDT selector the host gave: read it there)
            mu.write(es_edi(), mu.read(table + (bx & 0xFFF8u), 8));
            carry(false);
            return;
        }
        case 0x000C: {   // set descriptor BX from ES:EDI (8 bytes)
            const std::uint16_t index = ldt_index_of(bx);
            if (!(bx & kLdtBit) || index >= kLdtCount) {   // only an LDT selector can be set
                fail(kErrInvalidSelector);
                return;
            }
            const Bytes raw = mu.read(es_edi(), 8);
            log::debug(std::format("  000C {:04x} <- {:016x}", bx, le64(raw)));
            mu.write(kLdt + index * 8u, raw);
            const std::uint64_t e = le64(raw);
            std::uint32_t limit = limit_of(e);
            if ((e >> 55) & 1) limit = (limit << 12) | 0xFFF;
            const SegmentInfo seg{base_of(e), limit, access_of(e)};
            ldt_used_[index] = seg;
            st.descriptors[table_key(bx)] = seg;
            carry(false);
            return;
        }
        case 0x0100: {   // allocate BX paragraphs of DOS memory
            const std::optional<std::uint16_t> seg = ctx_.allocate(bx);
            if (!seg) {
                w(Reg::Ax, kDosOutOfMemory);
                w(Reg::Bx, ctx_.largest());
                carry(true);
                return;
            }
            const std::uint16_t selector = make_ldt(*seg * 16u, block_limit(bx), kData);
            w(Reg::Ax, *seg);       // the real-mode segment
            w(Reg::Dx, selector);   // the protected-mode selector
            carry(false);
            return;
        }
        case 0x0101:
        case 0x0102: {   // free or resize DOS memory (selector DX)
            const std::uint32_t base = get(dx).base;
            if (fn == 0x0101) {
                if (const std::optional<std::uint16_t> error = ctx_.free(static_cast<std::uint16_t>(base >> 4))) {
                    fail(*error);
                    return;
                }
                ldt_used_.erase(ldt_index_of(dx));
                st.descriptors.erase(table_key(dx));
            } else {
                const ArenaResult result = ctx_.resize(static_cast<std::uint16_t>(base >> 4), bx);
                if (result.kind == ArenaResult::Kind::Failed) {
                    fail(result.value);
                    return;
                }
                if (result.kind == ArenaResult::Kind::TooLarge) {
                    w(Reg::Ax, kDosOutOfMemory);
                    w(Reg::Bx, result.value);
                    carry(true);
                    return;
                }
                set(dx, std::nullopt, block_limit(bx), std::nullopt);
            }
            carry(false);
            return;
        }
        case 0x0200:
        case 0x0201:
        case 0x0202:
        case 0x0203:
        case 0x0204:
        case 0x0205:
            vectors(fn, bx, cx, dx);
            return;
        case 0x0300:
            simulate_real_mode_interrupt();
            return;
        case 0x0305:   // state save/restore: no state, a retf each
            w(Reg::Ax, 0);
            w(Reg::Bx, kHostSegment);
            w(Reg::Cx, kRmSave - kHostSegment * 16u);
            w(Reg::Si, kSelStubCode);
            w(Reg::Edi, kPmSave);
            carry(false);
            return;
        case 0x0306:   // the raw mode-switch addresses
            w(Reg::Bx, kHostSegment);
            w(Reg::Cx, kRawRmEntry - kHostSegment * 16u);
            w(Reg::Si, kSelStubCode);
            w(Reg::Edi, kRawPmEntry);
            carry(false);
            return;
        case 0x0800:   // physical address mapping: no paging, identity
        case 0x0801:
        case 0x0702:   // page paging hints and discard: no paging here
        case 0x0703:
        case 0x0600:   // lock and unlock a linear region: no paging here
        case 0x0601:
        case 0x0602:
        case 0x0603:
            carry(false);
            return;
        case 0x0604:   // the page size: 4 KB
            w(Reg::Bx, 0);
            w(Reg::Cx, 0x1000);
            carry(false);
            return;
        case 0x0900:   // virtual interrupt state: AL = the old one
        case 0x0901:
        case 0x0902: {
            const std::uint8_t old = virtual_interrupts_;
            if (fn != 0x0902) virtual_interrupts_ = fn == 0x0900 ? 0 : 1;
            w(Reg::Ax, (ax & 0xFF00) | old);
            carry(false);
            return;
        }
        case 0x0002: {   // segment to descriptor: one per real-mode segment
            auto found = segment_selectors_.find(bx);
            if (found == segment_selectors_.end()) found = segment_selectors_.emplace(bx, make_ldt(bx * 16u, 0xFFFF, kData)).first;
            w(Reg::Ax, found->second);
            carry(false);
            return;
        }
        case 0x0A00:   // the vendor API entry, and the capabilities (a DPMI 1.0 call; this is a 0.90 host): unsupported
        case 0x0401:
            fail(kErrUnsupported);
            return;
        case 0x0400:   // the DPMI version
            w(Reg::Ax, 0x005A);   // 0.90
            w(Reg::Bx, 0x0003);
            w(Reg::Cx, 0x0003);   // an 80386
            w(Reg::Dx, 0x0870);   // the PICs: master at INT 08h, slave at INT 70h
            carry(false);
            return;
        case 0x0500:   // free memory information (0x30 bytes at ES:EDI)
            memory_info();
            carry(false);
            return;
        case 0x0501: {   // allocate a memory block (BX:CX bytes)
            const std::uint32_t size = (static_cast<std::uint32_t>(bx) << 16) | cx;
            const std::optional<std::uint32_t> base = pool_alloc(size);
            if (!base) {
                carry(true);
                return;
            }
            const std::uint32_t handle = next_handle_++;
            memory_[handle] = {*base, size};
            w(Reg::Bx, *base >> 16);
            w(Reg::Cx, *base & 0xFFFF);
            w(Reg::Si, handle >> 16);
            w(Reg::Di, handle & 0xFFFF);
            carry(false);
            return;
        }
        case 0x0502:   // free the memory block with handle SI:DI
            memory_.erase((static_cast<std::uint32_t>(si) << 16) | di);
            carry(false);
            return;
        case 0x0503: {   // resize: keep the base, accept the new size
            const auto found = memory_.find((static_cast<std::uint32_t>(si) << 16) | di);
            if (found == memory_.end()) {
                carry(true);
                return;
            }
            found->second.size = (static_cast<std::uint32_t>(bx) << 16) | cx;
            w(Reg::Bx, found->second.base >> 16);
            w(Reg::Cx, found->second.base & 0xFFFF);
            carry(false);
            return;
        }
        case 0x0E00:   // the 80x87: report present
        case 0x0E01:   // ... and accept a set
            if (fn == 0x0E00) w(Reg::Ax, 0x0045);
            carry(false);
            return;
        default: break;
    }
    throw EmulationError(std::format("DPMI INT 31h AX={:04X} not implemented", fn));
}

// The linear address ES:EDI names (through the descriptor the host knows for ES, else as a real-mode segment).
std::uint32_t Host::es_edi() {
    const std::uint32_t es = r(Reg::Es), edi = r(Reg::Edi);
    const auto found = ctx_.state().descriptors.find(table_key(static_cast<std::uint16_t>(es)));
    return (found != ctx_.state().descriptors.end() ? found->second.base : es * 16) + edi;
}

std::optional<std::uint32_t> Host::pool_alloc(std::uint32_t size) {
    const std::uint64_t paged = (static_cast<std::uint64_t>(size) + 0xFFF) & ~0xFFFull;   // page-aligned
    if (pool_ + paged > kPoolTop) return std::nullopt;
    const std::uint32_t base = pool_;
    pool_ += static_cast<std::uint32_t>(paged);
    return base;
}

void Host::memory_info() {
    const std::uint32_t free = kPoolTop - pool_;
    io::ByteWriter info;
    info.u32(free);                      // the largest available block (bytes)
    info.u32(free >> 12);                // the largest unlocked page allocation
    info.u32(free >> 12);                // the largest locked page allocation
    info.u32((kPoolTop - kPool) >> 12);  // the linear address space, in pages
    info.u32(free >> 12);                // the number of free pages
    info.zeros(0x30 - info.size());
    ctx_.engine().write(es_edi(), info.data());
}

// 0200/0201 the real-mode vectors (the executive's table), 0202-0205 the protected-mode exception and interrupt handlers.
void Host::vectors(std::uint16_t fn, std::uint16_t bx, std::uint16_t cx, std::uint16_t dx) {
    State& st = ctx_.state();
    const std::uint8_t bl = static_cast<std::uint8_t>(bx & 0xFF);
    if (fn == 0x0200) {   // get real-mode interrupt vector BL into CX:DX
        const auto found = st.vectors.find(bl);
        const Far16 v = found != st.vectors.end() ? found->second : Far16{};
        w(Reg::Cx, v.seg);
        w(Reg::Dx, v.off);
        carry(false);
        return;
    }
    if (fn == 0x0201) {   // set real-mode interrupt vector BL from CX:DX
        st.vectors[bl] = Far16{cx, dx};
        carry(false);
        return;
    }
    const bool exception = fn == 0x0202 || fn == 0x0203;
    auto& table = exception ? exception_handlers_ : interrupt_handlers_;
    const std::uint32_t base = exception ? kExceptionBase : kDefaultBase;
    if (fn == 0x0202 || fn == 0x0204) {   // get the exception or interrupt handler BL into CX:(E)DX
        const auto found = table.find(bl);
        const Gate gate = found != table.end() ? found->second : Gate{kSelStubCode, base + 4u * bl};
        w(Reg::Cx, gate.selector);
        w(Reg::Edx, gate.offset);
        carry(false);
        return;
    }
    const std::uint32_t offset = client32_ ? r(Reg::Edx) : dx;   // 0203/0205: set the handler
    if (cx == kSelStubCode && offset == base + 4u * bl) table.erase(bl);   // set back to the host's default
    else table[bl] = Gate{cx, offset};
    carry(false);
}

// BL = interrupt, ES:(E)DI = the DPMI real-mode call structure. Load its real-mode register image and run the
// executive's INT 21h. The structure's DS and ES are real-mode segments: two scratch selectors (base = segment << 4)
// are pointed at them and the CPU stays in protected mode, so the executive resolves exactly the linear byte the
// client's DOS transfer buffer occupies, without ever loading a raw real-mode segment value into a protected-mode
// segment register.
void Host::simulate_real_mode_interrupt() {
    Engine& mu = ctx_.engine();
    State& st = ctx_.state();
    const std::uint8_t bl = static_cast<std::uint8_t>(r(Reg::Bx) & 0xFF);
    const std::uint32_t call_structure = es_edi();
    Bytes image = mu.read(call_structure, kRealModeCallBytes);
    const Bytes call_copy = image;   // (read before the results are written into `image`)
    io::ByteReader in(call_copy, "dpmi real-mode call structure");
    const std::uint32_t r_edi = in.u32(), r_esi = in.u32(), r_ebp = in.u32();
    in.skip(4);
    const std::uint32_t r_ebx = in.u32(), r_edx = in.u32(), r_ecx = in.u32(), r_eax = in.u32();
    in.skip(2);   // flags
    const std::uint16_t r_es = in.u16(), r_ds = in.u16();
    if (!scratch_ds_) {   // two reusable scratch data selectors
        scratch_ds_ = make_ldt(0, 0xFFFF, kData);
        scratch_es_ = make_ldt(0, 0xFFFF, kData);
    }
    set(*scratch_ds_, r_ds * 16u, std::nullopt, std::nullopt);
    set(*scratch_es_, r_es * 16u, std::nullopt, std::nullopt);
    st.override_ds = *scratch_ds_;   // the executive reads DS and ES from these (base = segment << 4), not from the CPU
    st.override_es = *scratch_es_;   // segment registers, which are left untouched
    st.out_ds = st.out_es = std::nullopt;   // a segment the call returns (ES:BX of 35h/52h) lands here
    std::uint32_t out_ax = 0, out_bx = 0, out_cx = 0, out_dx = 0, out_si = 0, out_di = 0, out_flags = 0;
    {
        // The caller's registers are put back, and the overrides cleared, however the call ends.
        static constexpr Reg kSaved[] = {Reg::Eax, Reg::Ebx, Reg::Ecx, Reg::Edx, Reg::Esi, Reg::Edi, Reg::Ebp, Reg::Eflags};
        struct Restore {
            Host& host;
            State& state;
            std::uint32_t saved[std::size(kSaved)];
            ~Restore() {
                for (std::size_t i = 0; i < std::size(kSaved); ++i) host.w(kSaved[i], saved[i]);
                state.override_ds.reset();
                state.override_es.reset();
            }
        } restore{*this, st, {}};
        for (std::size_t i = 0; i < std::size(kSaved); ++i) restore.saved[i] = r(kSaved[i]);
        w(Reg::Eax, r_eax);
        w(Reg::Ebx, r_ebx);
        w(Reg::Ecx, r_ecx);
        w(Reg::Edx, r_edx);
        w(Reg::Esi, r_esi);
        w(Reg::Edi, r_edi);
        w(Reg::Ebp, r_ebp);
        if (bl != 0x21) throw EmulationError(std::format("DPMI 0300 reflect of INT {:02X}h not implemented", bl));
        ctx_.int21();
        out_ax = r(Reg::Eax) & 0xFFFF;
        out_bx = r(Reg::Ebx) & 0xFFFF;
        out_cx = r(Reg::Ecx) & 0xFFFF;
        out_dx = r(Reg::Edx) & 0xFFFF;
        out_si = r(Reg::Esi) & 0xFFFF;
        out_di = r(Reg::Edi) & 0xFFFF;
        out_flags = r(Reg::Eflags) & 0xFFFF;
    }
    // the results go back into the real-mode call structure
    const auto put = [&image](std::size_t at, std::uint32_t v, int width) {
        for (int i = 0; i < width; ++i) image[at + i] = static_cast<std::uint8_t>(v >> (8 * i));
    };
    put(0x1C, out_ax, 4);
    put(0x10, out_bx, 4);
    put(0x18, out_cx, 4);
    put(0x14, out_dx, 4);
    put(0x04, out_si, 4);
    put(0x00, out_di, 4);
    put(0x20, out_flags, 2);
    if (st.out_es) put(0x22, *st.out_es, 2);
    if (st.out_ds) put(0x24, *st.out_ds, 2);
    mu.write(call_structure, image);
    carry(false);
}

}  // namespace cb::exact::emu::dpmi
