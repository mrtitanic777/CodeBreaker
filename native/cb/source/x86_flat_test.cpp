// Tests of what the whole-file x86 writers share: decoding with the detail they read, which instructions are branches and string operations,
// the number at the end of an operand, the register-to-register forms, and the operand spelling NASM wants.

#include "cb/source/x86_flat.hpp"

#include "cb/core/testing.hpp"

namespace cb::source::x86_flat {
namespace {

Insn decoded(unsigned bits, const Bytes& code, std::uint32_t address = 0x100) {
    platforms::DecoderSet decoders;
    const std::optional<Insn> insn = decode(decoders.get(platforms::Arch::X86, bits, true), code, address);
    CB_CHECK(insn.has_value());
    return insn.value_or(Insn{});
}

CB_TEST("source.x86_flat.decode_gives_the_text_the_bytes_and_where_the_immediate_is", [] {
    const Insn call = decoded(16, {0xE8, 0x10, 0x00, 0xFF});   // call near, 16-bit displacement; the 0xFF is not part of it
    CB_CHECK_EQ(call.mnemonic, "call");
    CB_CHECK_EQ(call.size, 3u);
    CB_CHECK_EQ(call.op_str, "0x113");                          // address + 3 + 0x10
    CB_CHECK_EQ(call.address, 0x100u);
    CB_CHECK_EQ(call.imm_size, 2u);
    CB_CHECK_EQ(call.imm_offset, 1u);
    CB_CHECK((std::vector<std::uint8_t>(call.raw().begin(), call.raw().end()) == Bytes{0xE8, 0x10, 0x00}));
    CB_CHECK(branch_target(call) == std::optional<std::uint32_t>(0x113));

    const Insn load = decoded(32, {0x8B, 0x86, 0x78, 0x56, 0x34, 0x12});   // mov eax, [esi + 0x12345678]
    CB_CHECK_EQ(load.disp_size, 4u);
    CB_CHECK_EQ(load.disp_offset, 2u);
    CB_CHECK(!branch_target(load));

    platforms::DecoderSet decoders;
    CB_CHECK(!decode(decoders.get(platforms::Arch::X86, 16, true), Bytes{}, 0));            // nothing to decode
    CB_CHECK(!decode(decoders.get(platforms::Arch::X86, 32, true), Bytes{0xE8, 0x01}, 0));   // cut short
});

CB_TEST("source.x86_flat.branches_and_string_operations", [] {
    for (const char* m : {"call", "jmp", "je", "jne", "jecxz", "jcxz", "loop", "loope", "loopne"}) CB_CHECK(is_branch(m));
    CB_CHECK(!is_branch("ret") && !is_branch("mov") && !is_branch("j") && !is_branch(""));
    for (const char* m : {"movsb", "movsw", "movsd", "rep stosb", "repe cmpsw", "repne scasb", "lodsd", "insb", "outsw"}) CB_CHECK(is_string_op(m));
    CB_CHECK(!is_string_op("movsq") && is_string_op("movsq", true));
    for (const char* m : {"mov", "movs", "stosx", "rep", "rep mov", "movsbx"}) CB_CHECK(!is_string_op(m, true));
    CB_CHECK(is_segment_prefix(0x2E) && is_segment_prefix(0x65) && !is_segment_prefix(0x66));
    CB_CHECK(is_arith("add") && is_arith("push") && is_arith("test") && !is_arith("lea"));
    CB_CHECK(is_register("eax") && is_register("bh") && is_register("si") && !is_register("rax") && !is_register("ds"));
});

CB_TEST("source.x86_flat.the_number_at_the_end_of_an_operand", [] {
    const auto n = [](std::string_view s) { return trailing_number(s); };
    CB_CHECK(n("ax, 0x1f")->start == 4 && n("ax, 0x1f")->value == 0x1f);
    CB_CHECK(n("ax, -0x10")->start == 4 && n("ax, -0x10")->value == -16);   // the sign is part of it
    CB_CHECK(n("bx, 12")->start == 4 && n("bx, 12")->value == 12);
    CB_CHECK(n("bx, -7")->start == 4 && n("bx, -7")->value == -7);
    CB_CHECK(!n("[bx + 0x12]"));
    CB_CHECK(!n("ax, bx") && !n("") && !n("ebx") && !n("0x"));
    CB_CHECK(n("dword ptr [eax], 0x80")->value == 0x80);
    CB_CHECK(n("[si + 5]") == std::nullopt);
    CB_CHECK(n("ds:[0x10], 0xffff")->value == 0xffff);
});

CB_TEST("source.x86_flat.the_register_to_register_forms_take_the_direction_bit_into_the_text", [] {
    // mov ax, bx as 89 D8 (NASM's own form) and as 8B C3 (the direction bit set: the other form the compiler writes)
    const Insn own = decoded(16, {0x89, 0xD8});
    const Insn other = decoded(16, {0x8B, 0xC3});
    CB_CHECK_EQ(own.op_str, "ax, bx");
    CB_CHECK_EQ(register_pair_text(own).value_or(""), "mov ax, bx");
    CB_CHECK_EQ(register_pair_text(other).value_or(""), "mov_ ax, bx");
    // with the operand-size prefix the opcode is the second byte
    const Insn prefixed = decoded(16, {0x66, 0x8B, 0xC3});
    CB_CHECK_EQ(register_pair_text(prefixed).value_or(""), "mov_ eax, ebx");
    // an instruction that is not a pair of registers, or not in the table, has none
    CB_CHECK(!register_pair_text(decoded(16, {0x8B, 0x07})));      // mov ax, [bx]
    CB_CHECK(!register_pair_text(decoded(16, {0x8D, 0x07})));      // lea ax, [bx]
    CB_CHECK(!register_pair_text(decoded(16, {0x40})));            // inc ax
    CB_CHECK(has_register_pair_form("xor") && !has_register_pair_form("lea"));
    const auto ops = register_pair_ops();
    CB_CHECK_EQ(ops.size(), 9u);
    for (std::size_t i = 1; i < ops.size(); ++i) CB_CHECK(ops[i - 1].mnemonic < ops[i].mnemonic);   // in name order
    CB_CHECK(ops.front().mnemonic == "adc" && ops.front().opcode == 0x10 && ops.back().mnemonic == "xor" && ops.back().opcode == 0x30);
});

CB_TEST("source.x86_flat.operands_are_spelled_for_nasm", [] {
    CB_CHECK_EQ(nasm_operands("word ptr [bx + si], ax"), "word [bx + si], ax");
    CB_CHECK_EQ(nasm_operands("xmmword ptr [eax], xmm0"), "oword [eax], xmm0");
    CB_CHECK_EQ(nasm_operands("xword ptr [eax]"), "tword [eax]");
    CB_CHECK_EQ(nasm_operands("st(1), st(0)"), "st1, st0");
    CB_CHECK_EQ(nasm_operands("ax, word ptr ds:[bx + 4]"), "ax, word [ds:bx + 4]");
    CB_CHECK_EQ(nasm_operands("es:[di], al"), "[es:di], al");
    CB_CHECK_EQ(nasm_operands("ax, bx"), "ax, bx");
    CB_CHECK_EQ(nasm_operands(""), "");
});

CB_TEST("source.x86_flat.the_bytes_beside_the_text", [] {
    const Bytes b = {0x01, 0xAB, 0xFF};
    CB_CHECK_EQ(db_bytes(b, 0, 3), "0x01,0xab,0xff");
    CB_CHECK_EQ(db_bytes(b, 1, 2), "0xab");
    CB_CHECK_EQ(db_bytes(b, 2, 9), "0xff");     // (cut to the bytes there are)
    CB_CHECK_EQ(db_bytes(b, 2, 2), "");
    const Insn call = decoded(16, {0xE8, 0x10, 0x00});
    // the line is padded to column 40 and the original text follows; a longer line gets one space
    CB_CHECK_EQ(with_original("db 0xe8", call), "db 0xe8" + std::string(33, ' ') + "; call 0x113");
    CB_CHECK_EQ(with_original(std::string(45, 'x'), call), std::string(45, 'x') + " ; call 0x113");
    CB_CHECK_EQ(with_original(std::string(40, 'x'), call), std::string(40, 'x') + " ; call 0x113");
});

}  // namespace
}  // namespace cb::source::x86_flat
