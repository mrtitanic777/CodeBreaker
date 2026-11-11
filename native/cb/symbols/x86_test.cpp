// Tests of cb/symbols/x86 on instructions written out as bytes (16-bit and 32-bit x86), with and without detail. The
// encodings are the manual's: the expected text is Capstone's, and the offsets are where the manual puts the
// displacement and the immediate.
#include "cb/symbols/x86.hpp"

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

namespace x86 = cb::symbols::x86;
namespace pf = cb::platforms;
using cb::Bytes;

CB_TEST("symbols.x86.decode_with_detail", [] {
    pf::DecoderSet decoders;
    const pf::Decoder& cs = decoders.get(pf::Arch::X86, 32, true);
    // mov eax, [0x1234] ; push 0x12345678 ; call +0x10 ; ret
    const Bytes code = {0x8B, 0x05, 0x34, 0x12, 0x00, 0x00, 0x68, 0x78, 0x56, 0x34, 0x12, 0xE8, 0x10, 0x00, 0x00, 0x00, 0xC3};
    const std::vector<x86::Insn> insns = x86::decode(cs, code, 0x1000);
    CB_CHECK_EQ(insns.size(), std::size_t{4});
    CB_CHECK_EQ(insns[0].mnemonic, "mov");
    CB_CHECK_EQ(insns[0].address, std::uint64_t{0x1000});
    CB_CHECK_EQ(insns[0].size, std::uint8_t{6});
    CB_CHECK_EQ(insns[0].disp, std::int64_t{0x1234});
    CB_CHECK_EQ(insns[0].disp_size, std::uint8_t{4});
    CB_CHECK_EQ(insns[0].disp_offset, std::uint8_t{2});
    CB_CHECK(!insns[0].first_immediate.has_value());   // its first operand is a register
    CB_CHECK_EQ(insns[1].mnemonic, "push");
    CB_CHECK_EQ(insns[1].address, std::uint64_t{0x1006});
    CB_CHECK_EQ(insns[1].imm_size, std::uint8_t{4});
    CB_CHECK_EQ(insns[1].imm_offset, std::uint8_t{1});
    CB_CHECK_EQ(insns[1].first_immediate.value_or(0), std::int64_t{0x12345678});
    CB_CHECK_EQ(insns[2].mnemonic, "call");
    CB_CHECK_EQ(insns[2].first_immediate.value_or(0), std::int64_t{0x1020});   // the target: 0x100B + 5 + 0x10
    CB_CHECK_EQ(insns[3].mnemonic, "ret");
    // `count` limits how many
    CB_CHECK_EQ(x86::decode(cs, code, 0x1000, 2).size(), std::size_t{2});
});

CB_TEST("symbols.x86.decode_without_detail_leaves_the_fields_empty", [] {
    pf::DecoderSet decoders;
    const pf::Decoder& cs = decoders.get(pf::Arch::X86, 32, false);
    const Bytes code = {0x8B, 0x05, 0x34, 0x12, 0x00, 0x00};
    const std::vector<x86::Insn> insns = x86::decode(cs, code, 0);
    CB_CHECK_EQ(insns.size(), std::size_t{1});
    CB_CHECK_EQ(insns[0].op_str, "eax, dword ptr [0x1234]");
    CB_CHECK_EQ(insns[0].disp, std::int64_t{0});
    CB_CHECK_EQ(insns[0].disp_size, std::uint8_t{0});
    CB_CHECK(!insns[0].first_immediate.has_value());
});

CB_TEST("symbols.x86.sixteen_bit", [] {
    pf::DecoderSet decoders;
    const pf::Decoder& cs = decoders.get(pf::Arch::X86, 16, true);
    const Bytes code = {0xB8, 0x34, 0x12, 0xCD, 0x21};   // mov ax, 0x1234 ; int 0x21
    const std::vector<x86::Insn> insns = x86::decode(cs, code, 0x100);
    CB_CHECK_EQ(insns.size(), std::size_t{2});
    CB_CHECK_EQ(insns[0].mnemonic, "mov");
    CB_CHECK_EQ(insns[0].op_str, "ax, 0x1234");
    CB_CHECK_EQ(insns[0].imm_size, std::uint8_t{2});
    CB_CHECK_EQ(insns[1].mnemonic, "int");
    CB_CHECK_EQ(insns[1].first_immediate.value_or(0), std::int64_t{0x21});
});

CB_TEST("symbols.x86.first_and_stopping_at_a_byte_that_does_not_decode", [] {
    pf::DecoderSet decoders;
    const pf::Decoder& cs = decoders.get(pf::Arch::X86, 32, false);
    const std::optional<x86::Insn> first = x86::decode_first(cs, Bytes{0x90, 0xC3}, 0x20);
    CB_CHECK(first.has_value());
    if (first) {
        CB_CHECK_EQ(first->mnemonic, "nop");
        CB_CHECK_EQ(first->address, std::uint64_t{0x20});
    }
    // a call whose displacement is cut off is no instruction: decoding stops there, and a first byte that does not decode
    // gives nothing
    CB_CHECK_EQ(x86::decode(cs, Bytes{0x90, 0xE8, 0x00}, 0).size(), std::size_t{1});
    CB_CHECK(!x86::decode_first(cs, Bytes{0xE8, 0x00}, 0).has_value());
    CB_CHECK(x86::decode(cs, Bytes{}, 0).empty());
});

CB_TEST("symbols.x86.needs_an_x86_decoder", [] {
    pf::DecoderSet decoders;
    const pf::Decoder& ppc = decoders.get(pf::Arch::Ppc, 32, false);
    CB_CHECK_THROWS_AS(cb::UsageError, (void)x86::decode(ppc, Bytes{0x4E, 0x80, 0x00, 0x20}, 0));
});
