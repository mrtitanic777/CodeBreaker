#include "cb/exact/x86.hpp"

#include <format>

#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
using namespace cb::exact;

namespace {

Bytes hex(std::string_view h) {
    Bytes out;
    for (std::size_t i = 0; i + 1 < h.size(); i += 2) out.push_back(static_cast<std::uint8_t>(std::stoi(std::string(h.substr(i, 2)), nullptr, 16)));
    return out;
}

// "address mnemonic operands" per instruction, to read a listing at a glance.
std::vector<std::string> listing(const std::vector<x86::Insn>& insns) {
    std::vector<std::string> out;
    for (const x86::Insn& i : insns) out.push_back(std::format("{} {}{}{}", i.address, i.mnemonic, i.op_str.empty() ? "" : " ", i.op_str));
    return out;
}

}  // namespace

CB_TEST("exact.x86.plain_code", [] {
    // push bp; mov bp, sp; xor ax, ax; pop bp; ret
    const Bytes code = hex("558bec33c05dc3");
    const auto insns = x86::disasm(code, 0, static_cast<std::uint32_t>(code.size()));
    CB_CHECK((listing(insns) == (std::vector<std::string>{"0 push bp", "1 mov bp, sp", "3 xor ax, ax", "5 pop bp", "6 ret"})));
    CB_CHECK_EQ(insns[1].size(), 2u);
    CB_CHECK((insns[1].bytes == hex("8bec")));
    // a range of the image: addresses are the image's
    const auto tail = x86::disasm(code, 3, 7);
    CB_CHECK((listing(tail) == (std::vector<std::string>{"3 xor ax, ax", "5 pop bp", "6 ret"})));
});

CB_TEST("exact.x86.the_8087_emulator_interrupts_are_the_instructions_they_stand_for", [] {
    // INT 35h stands for ESC D9h: cd 35 46 04 is fld dword ptr [bp + 4]
    const auto fld = x86::disasm(hex("cd35460490"), 0, 5);
    CB_CHECK((listing(fld) == (std::vector<std::string>{"0 fld dword ptr [bp + 4]", "4 nop"})));
    CB_CHECK_EQ(fld[0].size(), 4u);
    CB_CHECK((fld[0].bytes == hex("cd354604")));
    // INT 34h is ESC D8h: fadd st(1) (the mnemonic gets its f); INT 3Dh is fwait
    const auto fadd = x86::disasm(hex("cd34c1cd3d90"), 0, 6);
    CB_CHECK((listing(fadd) == (std::vector<std::string>{"0 fadd st(1)", "3 fwait", "5 nop"})));
    // INT 3Ch: an ESC with a segment override; the byte after it is the segment (top two bits) and the ESC opcode
    const auto seg = x86::disasm(hex("cd3c0006341290"), 0, 7);
    CB_CHECK_EQ(seg.size(), 2u);
    CB_CHECK_EQ(seg[0].mnemonic, "fadd");
    CB_CHECK_EQ(seg[0].op_str, "es: dword ptr [0x1234]");
    CB_CHECK((seg[0].bytes == hex("cd3c00063412")));
    const auto ds = x86::disasm(hex("cd3cc0063412"), 0, 6);
    CB_CHECK_EQ(ds[0].op_str, "ds: dword ptr [0x1234]");
    // not read as the emulator's: they are interrupts
    const auto raw = x86::disasm(hex("cd35460490"), 0, 5, false);
    CB_CHECK_EQ(raw[0].mnemonic, "int");
    CB_CHECK_EQ(raw[0].op_str, "0x35");
    CB_CHECK_EQ(raw[0].size(), 2u);
});

CB_TEST("exact.x86.what_cannot_be_decoded_is_a_byte", [] {
    // an INT 3Ch whose segment byte is the last of the image has nothing to decode with: left as an interrupt
    const auto cut = x86::disasm(hex("cd3c"), 0, 2);
    CB_CHECK_EQ(cut.size(), 1u);
    CB_CHECK_EQ(cut[0].mnemonic, "int");
    // 0Fh alone (a two-byte escape with no second byte) decodes to nothing
    const auto db = x86::disasm(hex("0f"), 0, 1);
    CB_CHECK((listing(db) == (std::vector<std::string>{"0 db 0x0f"})));
    // an end beyond the image is the image's end; an empty range is empty
    CB_CHECK_EQ(x86::disasm(hex("9090"), 0, 100).size(), 2u);
    CB_CHECK(x86::disasm(hex("9090"), 5, 9).empty());
    CB_CHECK(x86::disasm(hex("9090"), 1, 1).empty());
});

CB_TEST("exact.x86.jump_tables_are_found_and_stepped_over", [] {
    // jmp word ptr cs:[bx + 0xc]; seven nops; at 0xc a table of two places in the function, then a word that is not
    // one; then ret
    Bytes code = hex("2effa70c00");
    code.insert(code.end(), 7, 0x90);
    for (int b : {0x05, 0x00, 0x06, 0x00, 0x99, 0x99, 0xC3}) code.push_back(static_cast<std::uint8_t>(b));
    const std::uint32_t size = static_cast<std::uint32_t>(code.size());   // 19
    const auto first = x86::disasm(code, 0, size);
    const auto tables = x86::tables(first, code, 0, size);
    CB_CHECK_EQ(tables.size(), 1u);
    CB_CHECK_EQ(tables.at(0xC), 4u);
    // the code with the table taken out: the instructions before it, then the code after it
    const auto body = x86::body(code, 0, size);
    std::vector<std::uint32_t> at;
    for (const x86::Insn& i : body) at.push_back(i.address);
    CB_CHECK(std::ranges::find(at, 0xCu) == at.end());   // the table's words are not instructions
    CB_CHECK(std::ranges::find(at, 0xEu) == at.end());
    CB_CHECK_EQ(at.front(), 0u);
    CB_CHECK_EQ(body.back().mnemonic, "ret");
    CB_CHECK_EQ(body.back().address, 18u);
});

CB_TEST("exact.x86.jump_tables_in_a_program_image", [] {
    // The function is at image offset 0x20 and its code segment begins at 0x10 (`base`): `cs:[bx + 0x1c]` is the
    // table at image offset 0x2c, and its words are offsets in that segment (0x15 is the image's 0x25).
    Bytes image(0x20);
    for (std::uint8_t b : hex("2effa71c00")) image.push_back(b);
    image.insert(image.end(), 7, 0x90);
    for (int b : {0x15, 0x00, 0x16, 0x00, 0x99, 0x99, 0xC3}) image.push_back(static_cast<std::uint8_t>(b));
    const auto insns = x86::disasm(image, 0x20, 0x33);
    const auto tables = x86::tables(insns, image, 0x20, 0x33, 0x10);
    CB_CHECK_EQ(tables.size(), 1u);
    CB_CHECK_EQ(tables.at(0x2C), 4u);
    CB_CHECK(x86::tables(insns, image, 0x20, 0x33, 0).empty());   // with no base the table is at 0x1c: outside the function
    const auto body = x86::body(image, 0x20, 0x33, true, 0x10);
    CB_CHECK_EQ(body.back().address, 0x32u);
    CB_CHECK(std::ranges::none_of(body, [](const x86::Insn& i) { return i.address >= 0x2C && i.address < 0x30; }));
});

namespace {

// A decoder that knows two instructions: any byte 0x90 is "nop", 0xC3 is "ret"; the rest decode to nothing.
class TinyDecoder final : public x86::Decoder16 {
public:
    std::optional<x86::Insn> first(ByteView code, std::uint32_t address) const override {
        if (code.empty() || (code[0] != 0x90 && code[0] != 0xC3)) return std::nullopt;
        return x86::Insn{address, Bytes{code[0]}, code[0] == 0x90 ? "nop" : "ret", ""};
    }
};

}  // namespace

CB_TEST("exact.x86.the_decoder_is_a_seam", [] {
    const TinyDecoder tiny;
    const auto insns = x86::disasm(hex("90c3ff"), 0, 3, true, tiny);
    CB_CHECK((listing(insns) == (std::vector<std::string>{"0 nop", "1 ret", "2 db 0xff"})));
    // the default `all` repeats `first` up to the first byte that decodes to nothing
    CB_CHECK_EQ(tiny.all(hex("9090ff90"), 0).size(), 2u);
});
