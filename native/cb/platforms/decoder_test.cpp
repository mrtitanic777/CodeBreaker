#include "cb/platforms/decoder.hpp"

#include <thread>

#include "cb/core/capstone_api.hpp"
#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

// DIVERGES from platforms.decoder (Python capstone 4.0.2 and its port):
//   * the engine is Capstone 5.0.7 (native/third_party), so operand text follows 5.x (PowerPC registers print as "r3",
//     not "3"; 4.x had no operand-text for some prefixes); the decompiler is checked against the judge (7.2), not text;
//   * a width Capstone has no mode for is a UsageError (the Python raised KeyError); an unknown architecture cannot be
//     named (Arch is an enum, the Python raised ValueError for a string);
//   * a decoder is a value (Decoder) owned by a DecoderSet per thread, not a shared_ptr in a thread_local map.

using namespace cb;
using namespace cb::platforms;

CB_TEST("platforms.decoder.x86_16", [] {
    // push bp; mov bp,sp; mov ax,[bp+4]; pop bp; retf  -- the prologue and epilogue of a 16-bit far function
    const Bytes code = {0x55, 0x8B, 0xEC, 0x8B, 0x46, 0x04, 0x5D, 0xCB};
    const Decoder d = Decoder::open(Arch::X86, 16, false);
    const std::vector<Insn> out = d.disasm(code, 0x1000);
    CB_CHECK_EQ(out.size(), 5u);
    CB_CHECK_EQ(out[0].mnemonic, "push");
    CB_CHECK_EQ(out[0].op_str, "bp");
    CB_CHECK_EQ(out[0].address, 0x1000u);
    CB_CHECK_EQ(out[1].mnemonic, "mov");
    CB_CHECK_EQ(out[1].op_str, "bp, sp");
    CB_CHECK_EQ(out[1].address, 0x1001u);
    CB_CHECK_EQ(out[2].op_str, "ax, word ptr [bp + 4]");
    CB_CHECK_EQ(out[2].size, 3);
    CB_CHECK(equal_bytes(out[2].raw(), ByteView(code).subspan(3, 3)));
    CB_CHECK_EQ(out[4].mnemonic, "retf");
    CB_CHECK(d.arch() == Arch::X86);
    CB_CHECK_EQ(d.bits(), 16u);
});

CB_TEST("platforms.decoder.width_changes_the_decoding", [] {
    const Bytes code = {0x40};   // inc ax (16-bit) / inc eax (32-bit)
    CB_CHECK_EQ(Decoder::open(Arch::X86, 16, false).disasm(code, 0)[0].op_str, "ax");
    CB_CHECK_EQ(Decoder::open(Arch::X86, 32, false).disasm(code, 0)[0].op_str, "eax");
    const Bytes rex = {0x48, 0x89, 0xE5};   // mov rbp, rsp in 64-bit mode
    CB_CHECK_EQ(Decoder::open(Arch::X86, 64, false).disasm(rex, 0)[0].op_str, "rbp, rsp");
});

CB_TEST("platforms.decoder.powerpc_big_endian", [] {
    const Bytes code = {0x38, 0x60, 0x00, 0x05, 0x4E, 0x80, 0x00, 0x20};   // li r3, 5; blr
    const std::vector<Insn> out = Decoder::open(Arch::Ppc, 32, false).disasm(code, 0x82000000);
    CB_CHECK_EQ(out.size(), 2u);
    CB_CHECK_EQ(out[0].mnemonic, "li");
    CB_CHECK_EQ(out[0].op_str, "r3, 5");
    CB_CHECK_EQ(out[1].mnemonic, "blr");
    CB_CHECK_EQ(out[1].address, 0x82000004u);       // addresses past 32 bits of signed are fine
});

CB_TEST("platforms.decoder.stops_at_undecodable_and_count", [] {
    const Decoder d = Decoder::open(Arch::X86, 32, false);
    // nop; nop; (FF FF is not an instruction: opcode FF, reg field 7); nop
    const Bytes code = {0x90, 0x90, 0xFF, 0xFF, 0x90};
    CB_CHECK_EQ(d.disasm(code, 0).size(), 2u);
    CB_CHECK_EQ(d.disasm(Bytes{0x90, 0x90, 0x90, 0x90}, 0, 3).size(), 3u);   // count limits
    CB_CHECK(d.disasm(Bytes{}, 0).empty());                                  // empty input
    CB_CHECK(d.disasm(Bytes{0xFF, 0xFF}, 0).empty());
    CB_CHECK_EQ(d.disasm(Bytes{0xB8, 0x01}, 0).size(), 0u);                  // a truncated instruction is not one
});

CB_TEST("platforms.decoder.detail_gives_registers_and_operands", [] {
    const Decoder d = Decoder::open(Arch::X86, 32, true);
    CB_CHECK(d.detail());
    const Bytes code = {0x8B, 0x44, 0x24, 0x04};   // mov eax, [esp+4]
    int seen = 0;
    d.each(code, 0, 0, [&](const cs_insn& c) {
        ++seen;
        CB_CHECK(c.detail != nullptr);
        CB_CHECK_EQ(c.detail->x86.op_count, 2);
        const std::optional<RegisterAccess> access = d.register_access(c);
        CB_CHECK(access.has_value());
        std::vector<std::string> written, read;
        for (std::uint16_t r : access->written) written.push_back(d.register_name(r));
        for (std::uint16_t r : access->read) read.push_back(d.register_name(r));
        CB_CHECK((written == std::vector<std::string>{"eax"}));
        CB_CHECK(std::find(read.begin(), read.end(), "esp") != read.end());
        return true;
    });
    CB_CHECK_EQ(seen, 1);
});

CB_TEST("platforms.decoder.each_can_stop_early", [] {
    const Decoder d = Decoder::open(Arch::X86, 32, false);
    int n = 0;
    d.each(Bytes{0x90, 0x90, 0x90}, 0, 0, [&](const cs_insn&) { return ++n < 2; });
    CB_CHECK_EQ(n, 2);
    // a visitor that throws still frees what Capstone allocated (ASan would see the leak)
    CB_CHECK_THROWS_AS(ParseError, d.each(Bytes{0x90}, 0, 0, [](const cs_insn&) -> bool { throw ParseError("x", 1, 1, "y"); }));
});

CB_TEST("platforms.decoder.unsupported_widths_are_misuse", [] {
    CB_CHECK_THROWS_AS(UsageError, (void)Decoder::open(Arch::X86, 8, false));
    CB_CHECK_THROWS_AS(UsageError, (void)Decoder::open(Arch::X86, 24, false));
    CB_CHECK_THROWS_AS(UsageError, (void)Decoder::open(Arch::Ppc, 16, false));
});

CB_TEST("platforms.decoder.set_makes_one_of_each_kind", [] {
    DecoderSet set;
    const Decoder& a = set.get(Arch::X86, 16);
    const Decoder& b = set.get(Arch::X86, 16);
    const Decoder& c = set.get(Arch::X86, 16, true);
    const Decoder& d = set.get(Arch::X86, 32);
    CB_CHECK(&a == &b);                  // the same engine
    CB_CHECK(&a != &c);                  // detail is part of the kind
    CB_CHECK(&a != &d);
    CB_CHECK(!a.detail());
    CB_CHECK(c.detail());
    CB_CHECK_EQ(set.get(Arch::Ppc, 32).disasm(Bytes{0x4E, 0x80, 0x00, 0x20}, 0)[0].mnemonic, "blr");
    CB_CHECK_THROWS_AS(UsageError, (void)set.get(Arch::X86, 7));
});

CB_TEST("platforms.decoder.threads_have_their_own_sets", [] {
    // an engine is not shared between threads: two threads, each with a set, decode the same bytes independently
    auto work = [](std::size_t* total) {
        DecoderSet set;
        const Bytes code = {0x55, 0x8B, 0xEC, 0x5D, 0xC3};
        for (int i = 0; i < 2000; ++i) *total += set.get(Arch::X86, 32).disasm(code, 0).size();
    };
    std::size_t t1 = 0, t2 = 0;
    std::thread a(work, &t1), b(work, &t2);
    a.join();
    b.join();
    CB_CHECK_EQ(t1, 8000u);
    CB_CHECK_EQ(t2, 8000u);
});

CB_TEST("platforms.decoder.moves", [] {
    Decoder a = Decoder::open(Arch::X86, 32, false);
    Decoder b = std::move(a);
    CB_CHECK_EQ(b.disasm(Bytes{0x90}, 0).size(), 1u);
    Decoder c = Decoder::open(Arch::X86, 16, false);
    c = std::move(b);
    CB_CHECK_EQ(c.bits(), 32u);
    CB_CHECK_EQ(c.disasm(Bytes{0x90}, 0).size(), 1u);
});
