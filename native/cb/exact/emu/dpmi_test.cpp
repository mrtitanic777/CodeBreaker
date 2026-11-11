#include "cb/exact/emu/dpmi.hpp"

#include <array>
#include <map>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::exact::emu::dpmi;

// DIVERGES from the Python module (dpmi.py), on purpose:
//   * a descriptor request that would run past the 512-entry LDT is an error to the client (INT 31h 0000: AX=8011h with
//     the carry flag) or an EmulationError (the host's own allocations); the Python wrote past the table
//   * INT 31h 000Ch on a selector that is not an LDT one (or past the table) answers AX=8022h with the carry flag; the
//     Python wrote the descriptor into the LDT at the GDT selector's index
//   * a real-mode vector that was never set reads back as 0000:0000 (as the Python's dict.get default did)

namespace {

// A machine of 16 MB of RAM and registers. A 16-bit register is the low half of its 32-bit one.
class FakeEngine final : public Engine {
public:
    FakeEngine() : memory_(kPoolTop, 0) {}

    std::uint32_t reg(Reg r) override {
        switch (r) {
            case Reg::Ax: return eax & 0xFFFF;
            case Reg::Bx: return ebx & 0xFFFF;
            case Reg::Cx: return ecx & 0xFFFF;
            case Reg::Dx: return edx & 0xFFFF;
            case Reg::Si: return esi & 0xFFFF;
            case Reg::Di: return edi & 0xFFFF;
            case Reg::Sp: return esp & 0xFFFF;
            case Reg::Ip: return eip & 0xFFFF;
            case Reg::Cs: return cs;
            case Reg::Ds: return ds;
            case Reg::Es: return es;
            case Reg::Ss: return ss;
            case Reg::Eax: return eax;
            case Reg::Ebx: return ebx;
            case Reg::Ecx: return ecx;
            case Reg::Edx: return edx;
            case Reg::Esi: return esi;
            case Reg::Edi: return edi;
            case Reg::Ebp: return ebp;
            case Reg::Esp: return esp;
            case Reg::Eip: return eip;
            case Reg::Eflags: return eflags;
            case Reg::Cr0: return cr0;
        }
        return 0;
    }
    void set_reg(Reg r, std::uint32_t v) override {
        const auto low = [v](std::uint32_t& whole) { whole = (whole & 0xFFFF0000u) | (v & 0xFFFF); };
        switch (r) {
            case Reg::Ax: low(eax); break;
            case Reg::Bx: low(ebx); break;
            case Reg::Cx: low(ecx); break;
            case Reg::Dx: low(edx); break;
            case Reg::Si: low(esi); break;
            case Reg::Di: low(edi); break;
            case Reg::Sp: low(esp); break;
            case Reg::Ip: low(eip); break;
            case Reg::Cs: cs = v & 0xFFFF; break;
            case Reg::Ds: ds = v & 0xFFFF; break;
            case Reg::Es: es = v & 0xFFFF; break;
            case Reg::Ss: ss = v & 0xFFFF; break;
            case Reg::Eax: eax = v; break;
            case Reg::Ebx: ebx = v; break;
            case Reg::Ecx: ecx = v; break;
            case Reg::Edx: edx = v; break;
            case Reg::Esi: esi = v; break;
            case Reg::Edi: edi = v; break;
            case Reg::Ebp: ebp = v; break;
            case Reg::Esp: esp = v; break;
            case Reg::Eip: eip = v; break;
            case Reg::Eflags: eflags = v; break;
            case Reg::Cr0: cr0 = v; break;
        }
    }
    void set_table(TableReg r, std::uint16_t selector, std::uint32_t base, std::uint32_t limit, std::uint32_t flags) override {
        tables[r] = {selector, base, limit, flags};
    }
    Bytes read(std::uint32_t address, std::size_t size) override {
        if (static_cast<std::uint64_t>(address) + size > memory_.size()) throw EmulationError("read of unmapped memory");
        return Bytes(memory_.begin() + address, memory_.begin() + address + static_cast<std::ptrdiff_t>(size));
    }
    void write(std::uint32_t address, ByteView data) override {
        if (static_cast<std::uint64_t>(address) + data.size() > memory_.size()) throw EmulationError("write to unmapped memory");
        std::copy(data.begin(), data.end(), memory_.begin() + address);
    }
    void map_ram(std::uint32_t address, std::uint64_t size) override { mapped.emplace_back(address, size); }
    void stop() override { stopped = true; }

    struct Table {
        std::uint16_t selector = 0;
        std::uint32_t base = 0, limit = 0, flags = 0;
    };
    std::uint32_t eax = 0, ebx = 0, ecx = 0, edx = 0, esi = 0, edi = 0, ebp = 0, esp = 0, eip = 0, eflags = 0, cr0 = 0;
    std::uint32_t cs = 0, ds = 0, es = 0, ss = 0;
    std::map<TableReg, Table> tables;
    std::vector<std::pair<std::uint32_t, std::uint64_t>> mapped;
    bool stopped = false;

private:
    Bytes memory_;
};

// The executive: a flat DOS arena of 0x1000 paragraphs from segment 0x2000, a PSP at 0x1FF0, and a INT 21h that adds
// one to AX and notes the segments it was told to read DS and ES from.
class FakeExecutive final : public Context {
public:
    Engine& engine() override { return mu; }
    State& state() override { return st; }
    std::uint16_t psp() override { return 0x1FF0; }
    void int21() override {
        ++int21_calls;
        seen_ds = st.override_ds;
        seen_es = st.override_es;
        mu.eax = (mu.eax & 0xFFFF0000u) | static_cast<std::uint16_t>((mu.eax & 0xFFFF) + 1);
        mu.ebx = 0xBBBB1234;
        mu.eflags |= 1;   // (an error return)
        st.out_es = std::uint16_t{0x3456};
    }
    void dos_int(std::uint8_t n) override { dos_ints.push_back(n); }
    std::optional<std::uint16_t> allocate(std::uint16_t paragraphs) override {
        if (paragraphs > free_paragraphs) return std::nullopt;
        const std::uint16_t seg = next_segment;
        next_segment = static_cast<std::uint16_t>(next_segment + paragraphs);
        free_paragraphs = static_cast<std::uint16_t>(free_paragraphs - paragraphs);
        blocks[seg] = paragraphs;
        return seg;
    }
    std::uint16_t largest() override { return free_paragraphs; }
    std::optional<std::uint16_t> free(std::uint16_t segment) override {
        if (!blocks.erase(segment)) return std::uint16_t{9};
        return std::nullopt;
    }
    ArenaResult resize(std::uint16_t segment, std::uint16_t paragraphs) override {
        const auto found = blocks.find(segment);
        if (found == blocks.end()) return {ArenaResult::Kind::Failed, 9};
        if (paragraphs > found->second + free_paragraphs) return {ArenaResult::Kind::TooLarge, static_cast<std::uint16_t>(found->second + free_paragraphs)};
        free_paragraphs = static_cast<std::uint16_t>(free_paragraphs + found->second - paragraphs);
        found->second = paragraphs;
        return {ArenaResult::Kind::Done, 0};
    }

    FakeEngine mu;
    State st;
    std::uint16_t free_paragraphs = 0x1000;
    std::uint16_t next_segment = 0x2000;
    std::map<std::uint16_t, std::uint16_t> blocks;
    int int21_calls = 0;
    std::optional<std::uint16_t> seen_ds, seen_es;
    std::vector<std::uint8_t> dos_ints;
};

bool carry_set(FakeExecutive& x) { return x.mu.eflags & 1; }

// A client in protected mode (the mode switch done), its registers as the tests leave them.
struct Client {
    FakeExecutive x;
    Host host{x};
    Client() {
        // real mode, a far-call frame at SS:SP (return 0x3000:0x0100), DS = 0x2800
        x.mu.ss = 0x1800;
        x.mu.esp = 0x00F0;
        x.mu.ds = 0x2800;
        x.mu.write(0x1800 * 16 + 0xF0, Bytes{0x00, 0x01, 0x00, 0x30});
        CB_CHECK(host.handle(0xFF));
        x.mu.cr0 = 1;
        x.mu.stopped = false;
        x.mu.eflags = 0;
    }
    // INT 31h with AX = fn
    void int31(std::uint16_t fn) {
        x.mu.eax = fn;
        CB_CHECK(host.handle(0x31));
    }
};

std::uint64_t u64_at(FakeEngine& mu, std::uint32_t address) {
    const Bytes raw = mu.read(address, 8);
    io::ByteReader in(raw);
    return in.u64();
}

}  // namespace

CB_TEST("exact.emu.dpmi.descriptors_are_encoded_as_the_cpu_reads_them", [] {
    // a 16-bit code segment at 0x12345678, 64 KB
    CB_CHECK((descriptor(0x12345678, 0xFFFF, 0x9A, 0x00) == Bytes{0xFF, 0xFF, 0x78, 0x56, 0x34, 0x9A, 0x00, 0x12}));
    // a flat 32-bit data segment: the limit is in pages, G and D/B set
    CB_CHECK((descriptor(0, 0xFFFFFFFF, 0x92, 0xC0) == Bytes{0xFF, 0xFF, 0x00, 0x00, 0x00, 0x92, 0xCF, 0x00}));
    // a limit past 1 MB is byte-granular even when the caller did not say so
    CB_CHECK((descriptor(0x00400000, 0x00200000, 0x92, 0x00) == Bytes{0x00, 0x02, 0x00, 0x00, 0x40, 0x92, 0x80, 0x00}));
    // the LDT's own descriptor
    CB_CHECK((descriptor(kLdt, kLdtCount * 8 - 1, 0x82, 0x00) == Bytes{0xFF, 0x0F, 0x00, 0x08, 0x11, 0x82, 0x00, 0x00}));
});

CB_TEST("exact.emu.dpmi.the_host_announces_itself", [] {
    FakeExecutive x;
    Host host(x);
    CB_CHECK_EQ(x.mu.mapped.size(), std::size_t{1});
    CB_CHECK_EQ(x.mu.mapped[0].first, kHost);
    CB_CHECK((x.mu.read(kEntry, 2) == Bytes{0xCD, 0xFF}));   // the entry is an INT FFh the executive traps
    x.mu.eax = 0x1687;
    CB_CHECK(host.handle(0x2F));
    CB_CHECK_EQ(x.mu.reg(Reg::Ax), 0u);       // DPMI available
    CB_CHECK_EQ(x.mu.reg(Reg::Bx), 1u);       // 32-bit programs supported
    CB_CHECK_EQ(x.mu.reg(Reg::Dx), 0x5Au);    // version 0.90
    CB_CHECK_EQ(x.mu.reg(Reg::Es) * 16 + x.mu.reg(Reg::Di), kEntry);   // ES:DI is the entry
    // another function of INT 2Fh, and an interrupt that is not DPMI's, are not the host's
    x.mu.eax = 0x1600;
    CB_CHECK(!host.handle(0x2F));
    CB_CHECK(!host.handle(0x21));
});

CB_TEST("exact.emu.dpmi.the_mode_switch_builds_the_tables_and_the_trampoline", [] {
    Client c;
    FakeEngine& mu = c.x.mu;
    CB_CHECK(c.host.in_protected_mode());
    CB_CHECK(c.x.st.protected_mode);
    CB_CHECK(c.x.st.again);   // the run restarts at the trampoline
    CB_CHECK_EQ(mu.cs * 16 + mu.eip, kTrampoline);
    // GDTR: five descriptors; LDTR: the LDT selector
    CB_CHECK_EQ(mu.tables.at(TableReg::Gdtr).base, kGdt);
    CB_CHECK_EQ(mu.tables.at(TableReg::Gdtr).limit, 39u);
    CB_CHECK_EQ(mu.tables.at(TableReg::Ldtr).selector, kSelLdt);
    CB_CHECK_EQ(mu.tables.at(TableReg::Ldtr).limit, kLdtCount * 8u - 1);
    CB_CHECK((mu.read(kGdt + kSelStubCode, 8) == descriptor(kStub, 0xFFFF, 0x9A, 0)));
    CB_CHECK((mu.read(kGdt + kSelFlatData, 8) == descriptor(0, 0xFFFFFFFF, 0x92, 0xC0)));
    CB_CHECK((mu.read(kGdt + kSelRmData, 8) == descriptor(0, 0xFFFF, 0x92, 0)));
    // the client's selectors: code, data, stack, PSP -- the first four LDT slots the host hands out
    CB_CHECK((mu.read(kLdt + kLdtFirst * 8, 8) == descriptor(0x30000, 0xFFFF, 0x9A, 0)));
    CB_CHECK((mu.read(kLdt + (kLdtFirst + 1) * 8, 8) == descriptor(0x28000, 0xFFFF, 0x92, 0)));
    CB_CHECK((mu.read(kLdt + (kLdtFirst + 2) * 8, 8) == descriptor(0x18000, 0xFFFF, 0x92, 0)));
    CB_CHECK((mu.read(kLdt + (kLdtFirst + 3) * 8, 8) == descriptor(0x1FF00, 0xFF, 0x92, 0)));
    // the landing stub: mov eax, DS sel; mov ds, ax; ... mov sp, 0xF4 ...; jmp far CS sel:0x0100
    // the landing stub: load the client's DS, SS and ES selectors, set SP past the far-call frame, far-jump to its CS
    const Bytes want_stub{0x66, 0xB8, 0x8C, 0x00, 0x00, 0x00, 0x8E, 0xD8, 0x66, 0xB8, 0x94, 0x00, 0x00, 0x00, 0x8E, 0xD0,
                          0xBC, 0xF4, 0x00, 0x66, 0xB8, 0x9C, 0x00, 0x00, 0x00, 0x8E, 0xC0, 0xEA, 0x00, 0x01, 0x84, 0x00};
    CB_CHECK((mu.read(kStub, want_stub.size()) == want_stub));
    // the real-mode trampoline: cli; mov eax, cr0; or al, 1; mov cr0, eax; jmp far kSelStubCode:0
    CB_CHECK((mu.read(kTrampoline, 14) == Bytes{0xFA, 0x0F, 0x20, 0xC0, 0x0C, 0x01, 0x0F, 0x22, 0xC0, 0xEA, 0x00, 0x00, 0x08, 0x00}));
    // every vector has the host's default handler: INT FDh; iret (a 16-bit client)
    CB_CHECK((mu.read(kStub + kDefaultBase + 4 * 0x21, 4) == Bytes{0xCD, 0xFD, 0xCF, 0x90}));
    CB_CHECK((mu.read(kStub + kExceptionBase + 4 * 13, 4) == Bytes{0xCD, 0xFD, 0xCB, 0x90}));
    // the executive can resolve the selectors the client now holds
    CB_CHECK_EQ(c.x.st.descriptors.at(0x84).base, 0x30000u);
    CB_CHECK_EQ(c.x.st.descriptors.at(kSelFlatData).limit, 0xFFFFFFFFu);
});

CB_TEST("exact.emu.dpmi.a_32_bit_client_gets_32_bit_stubs", [] {
    FakeExecutive x;
    Host host(x);
    x.mu.ss = 0x1800;
    x.mu.esp = 0xF0;
    x.mu.write(0x1800 * 16 + 0xF0, Bytes{0, 1, 0, 0x30});
    x.mu.eax = 1;   // AX bit 0: a 32-bit client
    host.handle(0xFF);
    CB_CHECK((x.mu.read(kStub + kDefaultBase, 4) == Bytes{0xCD, 0xFD, 0x66, 0xCF}));
    CB_CHECK((x.mu.read(kStub + kExceptionBase, 4) == Bytes{0xCD, 0xFD, 0x66, 0xCB}));
    CB_CHECK((x.mu.read(kStub + kPmSave, 2) == Bytes{0x66, 0xCB}));
});

CB_TEST("exact.emu.dpmi.descriptors_allocated_changed_and_freed", [] {
    Client c;
    FakeEngine& mu = c.x.mu;
    // allocate two: the next free slots after the client's four
    mu.ecx = 2;
    c.int31(0x0000);
    CB_CHECK(!carry_set(c.x));
    CB_CHECK_EQ(mu.reg(Reg::Ax), static_cast<std::uint32_t>(((kLdtFirst + 4) << 3) | 4));
    const std::uint16_t first = static_cast<std::uint16_t>(mu.reg(Reg::Ax));
    // set the base (CX:DX), then the limit, then read the base back
    mu.ebx = first;
    mu.ecx = 0x0012;
    mu.edx = 0x3456;
    c.int31(0x0007);
    mu.ecx = 0x0000;
    mu.edx = 0x7FFF;
    c.int31(0x0008);
    mu.ecx = 0;
    mu.edx = 0;
    c.int31(0x0006);
    CB_CHECK_EQ(mu.reg(Reg::Cx), 0x12u);
    CB_CHECK_EQ(mu.reg(Reg::Dx), 0x3456u);
    CB_CHECK((mu.read(kLdt + ((first >> 3) * 8), 8) == descriptor(0x123456, 0x7FFF, 0x92, 0)));
    // the executive resolves the selector to the new base
    CB_CHECK_EQ(c.x.st.descriptors.at(first & 0xFFFC).base, 0x123456u);
    // access rights: a code descriptor of a wide limit is 32-bit
    mu.ecx = 0x9A;
    c.int31(0x0009);
    mu.ecx = 0x0001;
    mu.edx = 0x0000;   // limit 0x10000
    c.int31(0x0008);
    CB_CHECK((mu.read(kLdt + ((first >> 3) * 8), 8) == descriptor(0x123456, 0x10000, 0x9A, 0x40)));
    // an alias of a code selector is a data one over the same bytes
    mu.ebx = first;
    c.int31(0x000A);
    const std::uint16_t alias = static_cast<std::uint16_t>(mu.reg(Reg::Ax));
    CB_CHECK_EQ(alias, static_cast<std::uint16_t>(first + 8 * 1 + 8));   // (the slot after the second one allocated)
    CB_CHECK_EQ(c.x.st.descriptors.at(alias & 0xFFFC).access, 0x92);
    CB_CHECK_EQ(c.x.st.descriptors.at(alias & 0xFFFC).base, 0x123456u);
    // free: the executive forgets it
    mu.ebx = first;
    c.int31(0x0001);
    CB_CHECK(!c.x.st.descriptors.contains(first & 0xFFFC));
    // the selector increment
    c.int31(0x0003);
    CB_CHECK_EQ(mu.reg(Reg::Ax), 8u);
});

CB_TEST("exact.emu.dpmi.a_full_ldt_is_an_error_not_an_overrun", [] {
    Client c;
    FakeEngine& mu = c.x.mu;
    mu.ecx = kLdtCount - kLdtFirst - 4;   // every slot that is left
    c.int31(0x0000);
    CB_CHECK(!carry_set(c.x));
    mu.eflags = 0;
    mu.ecx = 1;
    c.int31(0x0000);
    CB_CHECK(carry_set(c.x));
    CB_CHECK_EQ(mu.reg(Reg::Ax), 0x8011u);
    // the host's own allocations (a segment-to-descriptor, an alias) cannot be answered with an error code
    mu.ebx = 0x5000;
    CB_CHECK_THROWS_AS(EmulationError, c.int31(0x0002));
});

CB_TEST("exact.emu.dpmi.specific_descriptors_and_raw_descriptor_access", [] {
    Client c;
    FakeEngine& mu = c.x.mu;
    // 000Dh: the specific selectors 04h-7Ch; one in use or outside the range is refused
    mu.ebx = 0x0004;
    c.int31(0x000D);
    CB_CHECK(!carry_set(c.x));
    mu.eflags = 0;
    c.int31(0x000D);
    CB_CHECK(carry_set(c.x));
    CB_CHECK_EQ(mu.reg(Reg::Ax), 0x8022u);   // already allocated
    mu.eflags = 0;
    mu.ebx = 0x00C4;   // index 24: past the specific selectors, not one the host has given
    c.int31(0x000D);
    CB_CHECK(carry_set(c.x));
    CB_CHECK_EQ(mu.reg(Reg::Ax), 0x8025u);
    mu.eflags = 0;
    mu.ebx = 0x0008;   // a GDT selector
    c.int31(0x000D);
    CB_CHECK_EQ(mu.reg(Reg::Ax), 0x8025u);
    // 000Ch writes a descriptor from ES:EDI (the executive's es is a real-mode segment here); 000Bh reads it back
    mu.eflags = 0;
    mu.es = 0x0800;
    mu.edi = 0x10;
    const Bytes raw = descriptor(0x00ABCDEF, 0x0FFFFF, 0x9A, 0xC0);
    mu.write(0x8000 + 0x10, raw);
    mu.ebx = 0x0004;
    c.int31(0x000C);
    CB_CHECK(!carry_set(c.x));
    CB_CHECK((mu.read(kLdt + 8 * 0, 8) == raw));   // index 0 of the LDT
    CB_CHECK_EQ(c.x.st.descriptors.at(0x0004).base, 0x00ABCDEFu);
    mu.edi = 0x40;
    c.int31(0x000B);
    CB_CHECK((mu.read(0x8000 + 0x40, 8) == raw));
    // a GDT selector cannot be set
    mu.ebx = kSelStubCode;
    c.int31(0x000C);
    CB_CHECK(carry_set(c.x));
    CB_CHECK_EQ(mu.reg(Reg::Ax), 0x8022u);
    // ... but its descriptor can be read, from the GDT
    mu.eflags = 0;
    c.int31(0x000B);
    CB_CHECK((mu.read(0x8000 + 0x40, 8) == descriptor(kStub, 0xFFFF, 0x9A, 0)));
});

CB_TEST("exact.emu.dpmi.dos_memory_in_the_executives_arena", [] {
    Client c;
    FakeEngine& mu = c.x.mu;
    mu.ebx = 0x100;   // 0x100 paragraphs
    c.int31(0x0100);
    CB_CHECK(!carry_set(c.x));
    const std::uint16_t segment = static_cast<std::uint16_t>(mu.reg(Reg::Ax));
    const std::uint16_t selector = static_cast<std::uint16_t>(mu.reg(Reg::Dx));
    CB_CHECK_EQ(segment, 0x2000);
    CB_CHECK_EQ(c.x.st.descriptors.at(selector & 0xFFFC).base, 0x20000u);
    CB_CHECK_EQ(c.x.st.descriptors.at(selector & 0xFFFC).limit, 0xFFFu);
    // more than there is: DOS error 8 and the largest block
    mu.eflags = 0;
    mu.ebx = 0x2000;
    c.int31(0x0100);
    CB_CHECK(carry_set(c.x));
    CB_CHECK_EQ(mu.reg(Reg::Ax), 8u);
    CB_CHECK_EQ(mu.reg(Reg::Bx), 0xF00u);
    // resize within what is free, then past it
    mu.eflags = 0;
    mu.edx = selector;
    mu.ebx = 0x200;
    c.int31(0x0102);
    CB_CHECK(!carry_set(c.x));
    CB_CHECK_EQ(c.x.st.descriptors.at(selector & 0xFFFC).limit, 0x1FFFu);
    mu.ebx = 0x4000;
    c.int31(0x0102);
    CB_CHECK(carry_set(c.x));
    CB_CHECK_EQ(mu.reg(Reg::Ax), 8u);
    CB_CHECK_EQ(mu.reg(Reg::Bx), 0x1000u);
    // free: the block and the selector go; freeing it again is DOS error 9
    mu.eflags = 0;
    c.int31(0x0101);
    CB_CHECK(!carry_set(c.x));
    CB_CHECK(!c.x.st.descriptors.contains(selector & 0xFFFC));
    mu.edx = 0x0004;
    c.int31(0x0101);
    CB_CHECK(carry_set(c.x));
    CB_CHECK_EQ(mu.reg(Reg::Ax), 9u);
});

CB_TEST("exact.emu.dpmi.segment_to_descriptor_is_one_per_segment", [] {
    Client c;
    c.x.mu.ebx = 0xB800;
    c.int31(0x0002);
    const std::uint32_t a = c.x.mu.reg(Reg::Ax);
    c.x.mu.ebx = 0xB800;
    c.int31(0x0002);
    CB_CHECK_EQ(c.x.mu.reg(Reg::Ax), a);
    CB_CHECK_EQ(c.x.st.descriptors.at(static_cast<std::uint16_t>(a & 0xFFFC)).base, 0xB8000u);
    c.x.mu.ebx = 0xA000;
    c.int31(0x0002);
    CB_CHECK_NE(c.x.mu.reg(Reg::Ax), a);
});

CB_TEST("exact.emu.dpmi.interrupt_vectors", [] {
    Client c;
    FakeEngine& mu = c.x.mu;
    // real-mode vectors live in the executive's table
    mu.ebx = 0x0021;
    mu.ecx = 0x1234;
    mu.edx = 0x5678;
    c.int31(0x0201);
    mu.ecx = mu.edx = 0;
    c.int31(0x0200);
    CB_CHECK_EQ(mu.reg(Reg::Cx), 0x1234u);
    CB_CHECK_EQ(mu.reg(Reg::Dx), 0x5678u);
    mu.ebx = 0x0022;   // never set
    c.int31(0x0200);
    CB_CHECK_EQ(mu.reg(Reg::Cx), 0u);
    // protected-mode handlers: the host's default until set
    mu.ebx = 0x0021;
    c.int31(0x0204);
    CB_CHECK_EQ(mu.reg(Reg::Cx), static_cast<std::uint32_t>(kSelStubCode));
    CB_CHECK_EQ(mu.reg(Reg::Edx), kDefaultBase + 4 * 0x21);
    mu.ebx = 13;
    c.int31(0x0202);   // an exception's default
    CB_CHECK_EQ(mu.reg(Reg::Edx), kExceptionBase + 4 * 13);
    // set one (0205) and an INT 21h in protected mode enters it as an interrupt gate
    mu.ebx = 0x0021;
    mu.ecx = 0x008C;
    mu.edx = 0x0200;
    c.int31(0x0205);
    mu.ss = 0x94;   // the client's stack selector: base 0x18000, 16-bit (B clear)
    mu.esp = 0x00F4;
    mu.cs = 0x84;
    mu.eip = 0x0123;
    mu.eflags = 0x0302;
    CB_CHECK(c.host.handle(0x21));
    CB_CHECK_EQ(mu.cs, 0x8Cu);
    CB_CHECK_EQ(mu.eip, 0x0200u);
    CB_CHECK_EQ(mu.eflags & 0x300, 0u);   // IF and TF clear
    CB_CHECK_EQ(mu.reg(Reg::Sp), 0xEEu);  // three words pushed
    const Bytes frame = mu.read(0x18000 + 0xEE, 6);
    CB_CHECK((frame == Bytes{0x23, 0x01, 0x84, 0x00, 0x02, 0x03}));   // IP, CS, FLAGS
    // the DPMI interrupts themselves are never given to a client handler
    CB_CHECK(!c.host.handle(0x30));
    // setting the default back removes the handler
    mu.ebx = 0x0021;
    mu.ecx = kSelStubCode;
    mu.edx = kDefaultBase + 4 * 0x21;
    c.int31(0x0205);
    mu.cr0 = 1;
    CB_CHECK(!c.host.handle(0x21));
    // a stub of the host's: INT FDh from the default handler of vector 0x10 runs the executive's service
    mu.eip = kDefaultBase + 4 * 0x10 + 2;
    CB_CHECK(c.host.handle(kDefaultInt));
    CB_CHECK_EQ(c.x.dos_ints.size(), std::size_t{1});
    CB_CHECK_EQ(c.x.dos_ints[0], 0x10);
    // ... and from an exception's: the client has no handler, the run cannot go on
    mu.eip = kExceptionBase + 4 * 13 + 2;
    CB_CHECK_THROWS_AS(EmulationError, c.host.handle(kDefaultInt));
});

CB_TEST("exact.emu.dpmi.a_32_bit_client_pushes_dwords_on_a_big_stack", [] {
    FakeExecutive x;
    Host host(x);
    x.mu.ss = 0x1800;
    x.mu.esp = 0xF0;
    x.mu.write(0x1800 * 16 + 0xF0, Bytes{0, 1, 0, 0x30});
    x.mu.eax = 1;
    host.handle(0xFF);
    x.mu.cr0 = 1;
    // a flat 32-bit stack: selector 0x10 (the GDT's flat data), B set
    x.mu.ebx = 0x2A;
    x.mu.ecx = kSelFlatData;
    x.mu.edx = 0x00400000;
    x.mu.eax = 0x0205;
    host.handle(0x31);
    x.mu.ss = kSelFlatData;
    x.mu.esp = 0x00100000;
    x.mu.cs = 0x84;
    x.mu.eip = 0x11223344;
    x.mu.eflags = 0x202;
    CB_CHECK(host.handle(0x2A));
    CB_CHECK_EQ(x.mu.eip, 0x00400000u);
    CB_CHECK_EQ(x.mu.esp, 0x00100000u - 12);
    CB_CHECK((x.mu.read(0x00100000 - 12, 12) == Bytes{0x44, 0x33, 0x22, 0x11, 0x84, 0, 0, 0, 0x02, 0x02, 0, 0}));
});

CB_TEST("exact.emu.dpmi.simulating_a_real_mode_int21", [] {
    Client c;
    FakeEngine& mu = c.x.mu;
    // the real-mode call structure at ES:EDI: AX=0x4000, DS=0x2222, ES=0x3333, EBX=7
    Bytes call(0x32, 0);
    const auto put = [&call](std::size_t at, std::uint32_t v, int n) {
        for (int i = 0; i < n; ++i) call[at + i] = static_cast<std::uint8_t>(v >> (8 * i));
    };
    put(0x10, 7, 4);        // EBX
    put(0x1C, 0x4000, 4);   // EAX
    put(0x22, 0x3333, 2);   // ES
    put(0x24, 0x2222, 2);   // DS
    mu.es = 0x0900;
    mu.edi = 0;
    mu.write(0x9000, call);
    mu.ebx = 0x0021;
    mu.eax = 0x0300;
    mu.eflags = 0;
    mu.esi = 0xCAFE0001;   // (the caller's registers come back)
    CB_CHECK(c.host.handle(0x31));
    CB_CHECK(!carry_set(c.x));
    CB_CHECK_EQ(c.x.int21_calls, 1);
    // the executive was told to read DS and ES from two selectors based at the structure's segments
    CB_CHECK(c.x.seen_ds.has_value() && c.x.seen_es.has_value());
    CB_CHECK_EQ(c.x.st.descriptors.at(*c.x.seen_ds & 0xFFFC).base, 0x22220u);
    CB_CHECK_EQ(c.x.st.descriptors.at(*c.x.seen_es & 0xFFFC).base, 0x33330u);
    CB_CHECK(!c.x.st.override_ds.has_value());   // (and not afterwards)
    // the results are in the structure: AX+1, BX 0x1234, the flags of the call (carry), the returned ES
    const Bytes after = mu.read(0x9000, 0x32);
    CB_CHECK_EQ(after[0x1C] | (after[0x1D] << 8), 0x4001);
    CB_CHECK_EQ(after[0x10] | (after[0x11] << 8), 0x1234);
    CB_CHECK_EQ(after[0x20] & 1, 1);
    CB_CHECK_EQ(after[0x22] | (after[0x23] << 8), 0x3456);
    CB_CHECK_EQ(after[0x24] | (after[0x25] << 8), 0x2222);   // (DS: nothing returned, as it was)
    // the caller's own registers are back, and the DPMI call itself succeeded
    CB_CHECK_EQ(mu.esi, 0xCAFE0001u);
    // any other interrupt is not served
    mu.ebx = 0x0010;
    mu.eax = 0x0300;
    CB_CHECK_THROWS_AS(EmulationError, c.host.handle(0x31));
    CB_CHECK(!c.x.st.override_ds.has_value());   // the overrides are cleared even then
});

CB_TEST("exact.emu.dpmi.extended_memory", [] {
    Client c;
    FakeEngine& mu = c.x.mu;
    // 0501: BX:CX bytes, page-aligned from the pool; handle in SI:DI
    mu.ebx = 0;
    mu.ecx = 0x1800;
    c.int31(0x0501);
    CB_CHECK(!carry_set(c.x));
    CB_CHECK_EQ((mu.reg(Reg::Bx) << 16) | mu.reg(Reg::Cx), kPool);
    const std::uint32_t handle = (mu.reg(Reg::Si) << 16) | mu.reg(Reg::Di);
    mu.ebx = 0;
    mu.ecx = 0x10;
    c.int31(0x0501);
    CB_CHECK_EQ((mu.reg(Reg::Bx) << 16) | mu.reg(Reg::Cx), kPool + 0x2000);   // (the first took two pages)
    // 0500: the free memory block
    mu.es = 0x0900;
    mu.edi = 0;
    c.int31(0x0500);
    const Bytes info_bytes = mu.read(0x9000, 0x30);
    io::ByteReader info(info_bytes);
    CB_CHECK_EQ(info.u32(), kPoolTop - kPool - 0x3000);
    // 0503 resizes in place, 0502 frees
    mu.esi = handle >> 16;
    mu.edi = handle & 0xFFFF;
    mu.ebx = 0;
    mu.ecx = 0x5000;
    c.int31(0x0503);
    CB_CHECK(!carry_set(c.x));
    CB_CHECK_EQ((mu.reg(Reg::Bx) << 16) | mu.reg(Reg::Cx), kPool);
    c.int31(0x0502);
    CB_CHECK(!carry_set(c.x));
    c.int31(0x0503);
    CB_CHECK(carry_set(c.x));   // no such block now
    // more than the pool holds
    mu.eflags = 0;
    mu.ebx = 0x0200;   // 32 MB
    mu.ecx = 0;
    c.int31(0x0501);
    CB_CHECK(carry_set(c.x));
});

CB_TEST("exact.emu.dpmi.the_small_services", [] {
    Client c;
    FakeEngine& mu = c.x.mu;
    c.int31(0x0400);   // the version: 0.90, the PICs
    CB_CHECK_EQ(mu.reg(Reg::Ax), 0x5Au);
    CB_CHECK_EQ(mu.reg(Reg::Dx), 0x0870u);
    c.int31(0x0604);   // the page size
    CB_CHECK_EQ(mu.reg(Reg::Cx), 0x1000u);
    // the virtual interrupt flag: disable returns the old state, enable the one before
    mu.eax = 0x0900;
    c.host.handle(0x31);
    CB_CHECK_EQ(mu.reg(Reg::Ax) & 0xFF, 1u);
    mu.eax = 0x0902;
    c.host.handle(0x31);
    CB_CHECK_EQ(mu.reg(Reg::Ax) & 0xFF, 0u);
    mu.eax = 0x0901;
    c.host.handle(0x31);
    CB_CHECK_EQ(mu.reg(Reg::Ax) & 0xFF, 0u);
    mu.eax = 0x0902;
    c.host.handle(0x31);
    CB_CHECK_EQ(mu.reg(Reg::Ax) & 0xFF, 1u);
    // a DPMI 1.0 call is unsupported (carry), paging calls are accepted
    c.int31(0x0401);
    CB_CHECK(carry_set(c.x));
    CB_CHECK_EQ(mu.reg(Reg::Ax), 0x8001u);
    mu.eflags = 0;
    c.int31(0x0600);
    CB_CHECK(!carry_set(c.x));
    c.int31(0x0E00);
    CB_CHECK_EQ(mu.reg(Reg::Ax), 0x45u);
    // a service the host does not have
    CB_CHECK_THROWS_AS(EmulationError, c.int31(0x1234));
});

CB_TEST("exact.emu.dpmi.the_raw_mode_switches", [] {
    Client c;
    FakeEngine& mu = c.x.mu;
    // 0306 gives the addresses of the two entries
    c.int31(0x0306);
    CB_CHECK_EQ(mu.reg(Reg::Bx), static_cast<std::uint32_t>(kHostSegment));
    CB_CHECK_EQ(mu.reg(Reg::Cx), kRawRmEntry - kHostSegment * 16u);
    CB_CHECK_EQ(mu.reg(Reg::Si), static_cast<std::uint32_t>(kSelStubCode));
    CB_CHECK_EQ(mu.edi, kRawPmEntry);
    CB_CHECK((mu.read(kStub + kRawPmEntry, 2) == Bytes{0xCD, kRawInt}));
    CB_CHECK((mu.read(kRawRmEntry, 2) == Bytes{0xCD, kRawInt}));
    // the client jumps to the protected-mode entry to go to real mode: AX=DS CX=ES DX=SS BX=SP SI=CS EDI=IP
    mu.cr0 = 1;
    mu.eax = 0x2000;
    mu.ecx = 0x2100;
    mu.edx = 0x2200;
    mu.ebx = 0xAAAA5678;
    mu.esi = 0x3000;
    mu.edi = 0xBBBB0100;
    CB_CHECK(c.host.handle(kRawInt));
    CB_CHECK(!c.x.st.protected_mode);
    CB_CHECK(c.x.st.again);
    CB_CHECK(mu.stopped);
    CB_CHECK_EQ(mu.eip, kPmToRm);
    CB_CHECK_EQ(mu.esp, 0x5678u);
    {
        const Bytes params = mu.read(kRmParams, 0x14);
        io::ByteReader p(params);   // ds es ss sp ip cs fs/gs pad eax
        CB_CHECK_EQ(p.u16(), 0x2000);
        CB_CHECK_EQ(p.u16(), 0x2100);
        CB_CHECK_EQ(p.u16(), 0x2200);
        CB_CHECK_EQ(p.u16(), 0x5678);
        CB_CHECK_EQ(p.u16(), 0x0100);
        CB_CHECK_EQ(p.u16(), 0x3000);
    }
    // and back: from real mode to protected mode, the parameter block is the stub's
    mu.cr0 = 0;
    mu.stopped = false;
    mu.eax = 0x0084;
    mu.ecx = 0x008C;
    mu.edx = 0x0094;
    mu.ebx = 0x00123456;
    mu.esi = 0x009C;
    mu.edi = 0x00400000;
    CB_CHECK(c.host.handle(kRawInt));
    CB_CHECK(c.x.st.protected_mode);
    CB_CHECK(mu.stopped);
    CB_CHECK_EQ(mu.cs * 16 + mu.eip, kRawTrampoline);
    const Bytes pm_params = mu.read(kStub + kPmParams, 0x18);
    io::ByteReader q(pm_params);   // ds es ss pad esp eip cs pad eax
    CB_CHECK_EQ(q.u16(), 0x0084);
    CB_CHECK_EQ(q.u16(), 0x008C);
    CB_CHECK_EQ(q.u16(), 0x0094);
    q.skip(2);
    CB_CHECK_EQ(q.u32(), 0x00123456u);
    CB_CHECK_EQ(q.u32(), 0x00400000u);
    CB_CHECK_EQ(q.u16(), 0x009C);
    // the switch code itself is at the addresses the entries name
    CB_CHECK_EQ(mu.read(kRawTrampoline, 1)[0], 0xFA);
    CB_CHECK_EQ(mu.read(kStub + kPmToRm, 1)[0], 0xB8);
    // a raw switch before the mode switch is not the host's
    FakeExecutive other;
    Host idle(other);
    CB_CHECK(!idle.handle(kRawInt));
});
