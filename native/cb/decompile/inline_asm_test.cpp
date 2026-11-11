#include "cb/decompile/inline_asm.hpp"

#include <string>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

namespace ia = cb::decompile::inline_asm;
using std::string;

// DIVERGES from the Python module (the old harness agreed on all else; the expected texts below were produced by the
// original Python on the same bytes with the same Capstone 5.0.7 and read through by hand):
//  - `rep insb` / `rep insw` are written `rep ...` (Python's test `mnemonic[:4] in (..., "ins", ...)` can never be true
//    for the four-letter slice "insb", so it wrote `repe insb`: the same F3 prefix byte, the wrong word);
//  - the instructions are cb::decompile::inline_asm::Instruction values and the registers Capstone's ids (uint16), the
//    segment a plain number: Python's dict keyed by both ints and ("segment", offset) tuples is two maps;
//  - a refusal is a Result carrying NotAsmBody (Python returned (None, why)); the text is the same;
//  - a decoded stream cut short in the middle of a prologue/epilogue probe is "no frame" (Python raised IndexError).

namespace {

const std::string kPro = "558bec";
const std::string kEpi = "8be55dc3";

cb::Bytes hex(const string& s) {
    cb::Bytes out;
    for (std::size_t i = 0; i + 1 < s.size(); i += 2) out.push_back(static_cast<std::uint8_t>(std::stoi(s.substr(i, 2), nullptr, 16)));
    return out;
}

std::optional<string> name_at(std::uint32_t seg, std::uint16_t off) {
    if (seg == 2 && off == 0x0200) return "helper_a";
    if (seg == 2 && off == 0x0300) return "helper_b";
    if (seg == 3 && off == 0x0540) return "other_c";
    return std::nullopt;
}

struct Run {
    string hex_code;
    std::uint32_t start = 0;
    ia::Fixups fixups;
    std::uint32_t seg = 2;
    string result = "int2";
    string name = "f";
    std::optional<std::uint32_t> end;
};

// whole_body of `r.hex_code` placed at offset r.start of a segment; the text, or "!" + the reason.
string say(const Run& r) {
    cb::Bytes code(r.start);
    cb::Bytes body = hex(r.hex_code);
    code.insert(code.end(), body.begin(), body.end());
    auto out = ia::whole_body(r.name, code, r.start, r.end.value_or(static_cast<std::uint32_t>(code.size())), r.fixups,
                              name_at, r.seg, r.result);
    return out.ok() ? out.value() : "!" + string(out.error().what());
}

string call_to(std::uint32_t at, std::uint32_t target) {
    const auto rel = static_cast<std::uint16_t>(target - (at + 3));
    return std::format("e8{:02x}{:02x}", rel & 0xff, rel >> 8);
}

}  // namespace

// SUWIN.EXE (the Win95 Setup's 16-bit program), segment 2 offset 0x7E3C: `mov bx, h / mov ah, 68h / int 21h` in the
// frame C 8 gave it (push bp / mov bp, sp ... leave / ret 2). The bytes are the file's.
CB_TEST("decompile.inline_asm.suwin_7e3c", [] {
    Run r{"558bec8b5e04b468cd21c9c20200"};
    r.start = 0x7e3c;
    r.name = "fn_7e3c";
    CB_CHECK_EQ(say(r),
                "int2 __near __pascal fn_7e3c(a1)\nint2 a1;\n{\n  _asm {\n        mov bx, word ptr [bp + 4]\n"
                "        mov ah, 0x68\n        int 0x21\n  }\n}");
});

CB_TEST("decompile.inline_asm.frames_and_conventions", [] {
    CB_CHECK_EQ(say({"558bec83ec04b801008be55dc3"}),
                "int2 __near __cdecl f()\n{\n  char codebreaker_frame[4];\n  _asm {\n        mov ax, 1\n  }\n}");
    CB_CHECK_EQ(say({"c8080000fafbc9cb"}),   // enter 8,0 ... leave / retf
                "int2 __far __cdecl f()\n{\n  char codebreaker_frame[8];\n  _asm {\n        cli\n        sti\n  }\n}");
    // __loadds: push ds / mov ax, DGROUP / mov ds, ax ... pop ds before the frame is torn down; retf 6 pops three arguments
    CB_CHECK_EQ(say({"558bec1eb834128ed8a10a00a30c001f8be55dca0600"}),
                "int2 __far __pascal __loadds f(a1, a2, a3)\nint2 a1;\nint2 a2;\nint2 a3;\n{\n  _asm {\n"
                "        mov ax, word ptr ds:[0xa]\n        mov word ptr ds:[0xc], ax\n  }\n}");
    CB_CHECK_EQ(say({"558bec908be55dc20400"}), "int2 __near __pascal f(a1, a2)\nint2 a1;\nint2 a2;\n{\n  _asm {\n        nop\n  }\n}");
    CB_CHECK_EQ(say({"558bec908be55dcb"}), "int2 __far __cdecl f()\n{\n  _asm {\n        nop\n  }\n}");
    Run typed{"558bec908be55dc3"};
    typed.result = "void";
    typed.name = "g_name";
    CB_CHECK_EQ(say(typed), "void __near __cdecl g_name()\n{\n  _asm {\n        nop\n  }\n}");
    CB_CHECK_EQ(say({"558bec5dc3"}), "int2 __near __cdecl f()\n{\n  _asm {\n  }\n}");   // nothing between the frames
    // an end past the segment is the part that exists
    Run overrun{"558bec908be55dc3"};
    overrun.end = 500;
    CB_CHECK_EQ(say(overrun), "int2 __near __cdecl f()\n{\n  _asm {\n        nop\n  }\n}");
});

CB_TEST("decompile.inline_asm.jumps_and_labels", [] {
    // 0x100: xor ax,ax / inc ax / cmp ax,3 / jl back / jmp to the frame's end / pop bp / ret
    Run labels{"558bec33c0403d03007cfaeb005dc3"};
    labels.start = 0x100;
    CB_CHECK_EQ(say(labels),
                "int2 __near __cdecl f()\n{\n  _asm {\n        xor ax, ax\nL_0105:\n        inc ax\n        cmp ax, 3\n"
                "        jl L_0105\n        jmp L_010d\nL_010d:\n  }\n}");
    Run to_end{"558beceb008be55dc3"};
    to_end.start = 0x40;
    CB_CHECK_EQ(say(to_end), "int2 __near __cdecl f()\n{\n  _asm {\n        jmp L_0045\nL_0045:\n  }\n}");
    Run loops{"558bece3fee2fe8be55dc3"};
    loops.start = 0x100;
    CB_CHECK_EQ(say(loops), "int2 __near __cdecl f()\n{\n  _asm {\nL_0103:\n        jcxz L_0103\nL_0105:\n        loop L_0105\n  }\n}");
});

CB_TEST("decompile.inline_asm.calls", [] {
    Run near_named{kPro + call_to(0x103, 0x200) + kEpi};
    near_named.start = 0x100;
    CB_CHECK_EQ(say(near_named), "int2 __near __cdecl f()\n{\n  _asm {\n        call near ptr helper_a\n  }\n}");
    Run near_unnamed{kPro + call_to(0x103, 0x250) + kEpi};
    near_unnamed.start = 0x100;
    CB_CHECK_EQ(say(near_unnamed), "!a near call it cannot name (0103)");
    // a far call: the fix-up on its offset word names the target
    Run named{kPro + "9a00000000" + kEpi};
    named.start = 0x100;
    named.fixups.names[0x104] = "KERNEL_Thing";
    CB_CHECK_EQ(say(named), "int2 __near __cdecl f()\n{\n  _asm {\n        call KERNEL_Thing\n  }\n}");
    named.fixups.names[0x104] = std::nullopt;
    CB_CHECK_EQ(say(named), "!a far call it cannot name (0103)");
    named.fixups.names.clear();
    CB_CHECK_EQ(say(named), "!a far lcall without a fix-up (0103)");
    // a selector-only fix-up: the offset is in the instruction, the segment is the fix-up's
    Run selector{kPro + "9a40050000" + kEpi};
    selector.start = 0x100;
    selector.fixups.names[0x106] = std::nullopt;
    selector.fixups.segments[0x106] = 3;
    CB_CHECK_EQ(say(selector), "int2 __near __cdecl f()\n{\n  _asm {\n        call other_c\n  }\n}");
    selector.fixups.segments[0x106] = std::nullopt;
    CB_CHECK_EQ(say(selector), "!a far call it cannot name (0103)");
    selector.fixups.segments[0x106] = 4;   // a segment that has nothing at that offset
    CB_CHECK_EQ(say(selector), "!a far call it cannot name (0103)");
    Run far_jump{kPro + "ea00000000" + kEpi};
    far_jump.start = 0x100;
    CB_CHECK_EQ(say(far_jump), "!a far ljmp without a fix-up (0103)");
    Run in_mov{kPro + "b80000" + kEpi};
    in_mov.start = 0x100;
    in_mov.fixups.names[0x104] = "x";
    CB_CHECK_EQ(say(in_mov), "!a fix-up in mov (0103)");
});

CB_TEST("decompile.inline_asm.operands", [] {
    // string instructions: no operands written, the prefix as the assembler spells it
    CB_CHECK_EQ(say({kPro + "f3a5f3aaf2aef3a6f36cf36faca4" + kEpi}),
                "int2 __near __cdecl f()\n{\n  _asm {\n        rep movsw\n        rep stosb\n        repne scasb\n"
                "        repe cmpsb\n        rep insb\n        rep outsw\n        lodsb\n        movsb\n  }\n}");
    // `les`/`lds` take `dword ptr`; a direct memory operand is the module's data, ds:[n]
    CB_CHECK_EQ(say({kPro + "c41fc57704c43e0a00" + kEpi}),
                "int2 __near __cdecl f()\n{\n  _asm {\n        les bx, dword ptr [bx]\n        lds si, dword ptr [bx + 4]\n"
                "        les di, dword ptr ds:[0xa]\n  }\n}");
    CB_CHECK_EQ(say({kPro + "a10a008b1e0c00c706140034128b4604" + kEpi}),
                "int2 __near __cdecl f()\n{\n  _asm {\n        mov ax, word ptr ds:[0xa]\n        mov bx, word ptr ds:[0xc]\n"
                "        mov word ptr ds:[0x14], 0x1234\n        mov ax, word ptr [bp + 4]\n  }\n}");
    // the helper on its own
    ia::Instruction les;
    les.mnemonic = "les";
    les.op_str = "bx, ptr [bx]";
    CB_CHECK_EQ(ia::operands_text(les), "bx, dword ptr [bx]");
    les.op_str = "ptr [bx]";
    CB_CHECK_EQ(ia::operands_text(les), "dword ptr [bx]");
    ia::Instruction rep;
    rep.mnemonic = "rep movsw";
    rep.op_str = "word ptr es:[di], word ptr [si]";
    CB_CHECK_EQ(rep.base_mnemonic(), "movsw");
    CB_CHECK_EQ(ia::operands_text(rep), "");
    ia::Instruction plain;
    plain.mnemonic = "mov";
    plain.op_str = "ax, word ptr [0x1234]";
    ia::Operand mem;
    mem.kind = ia::OperandKind::Mem;
    mem.mem.disp = 0x1234;
    plain.operands.push_back(mem);
    CB_CHECK_EQ(ia::operands_text(plain), "ax, word ptr ds:[0x1234]");
    mem.mem.base = 11;   // a base register: not a direct reference
    plain.operands = {mem};
    CB_CHECK_EQ(ia::operands_text(plain), "ax, word ptr [0x1234]");
});

CB_TEST("decompile.inline_asm.refusals", [] {
    CB_CHECK_EQ(say({kPro + "66b801000000" + kEpi}), "!386 code, which the compiler made (0003 mov eax, 1)");
    CB_CHECK_EQ(say({kPro + "0f840000" + kEpi}), "!386 code, which the compiler made (0003 je 7)");
    CB_CHECK_EQ(say({kPro + "ffe3" + kEpi}), "!a jump through a register or memory (0003)");
    Run out{kPro + "e9f00f" + kEpi};
    out.start = 0x100;
    CB_CHECK_EQ(say(out), "!a jump out of the function (0103)");
    CB_CHECK_EQ(say({kPro + "cf" + kEpi}), "!a return in its middle (0003)");
    CB_CHECK_EQ(say({kPro + "90909090"}), "!no return found");
    CB_CHECK_EQ(say({""}), "!no return found");
    CB_CHECK_EQ(say({"90c3"}), "!no frame of C 8's at its start");
    CB_CHECK_EQ(say({kPro + "90c3"}), "!no frame of C 8's at its end");
    // the error is a NotAsmBody, and value() of a refusal throws it
    auto refused = ia::whole_body("f", hex("90c3"), 0, 2, {}, name_at, 2);
    CB_CHECK(!refused.ok());
    CB_CHECK_THROWS_AS(ia::NotAsmBody, (void)refused.value());
});

CB_TEST("decompile.inline_asm.prologue_and_epilogue_search", [] {
    auto decode = [](const string& h) { return ia::decode16(hex(h), 0); };
    auto ins = decode("558bec83ec0c" "1eb83412" "8ed8" "90" "1f8be55dcb");
    auto pro = ia::find_prologue(ins);
    CB_CHECK(pro && pro->taken == 6 && pro->frame == 12 && pro->loads_ds);
    auto epi = ia::find_epilogue(ins, true);
    CB_CHECK(epi && epi->taken == 4 && epi->far && epi->pops == 0);   // pop ds, mov sp bp, pop bp, retf
    epi = ia::find_epilogue(ins, false);
    CB_CHECK(epi && epi->taken == 3);                                  // without __loadds the pop ds is not the frame's
    auto enter = decode("c8100000" "90" "c9c20800");
    pro = ia::find_prologue(enter);
    CB_CHECK(pro && pro->taken == 1 && pro->frame == 16 && !pro->loads_ds);
    epi = ia::find_epilogue(enter, false);
    CB_CHECK(epi && epi->taken == 2 && !epi->far && epi->pops == 8);
    CB_CHECK(!ia::find_prologue(decode("90c3")));
    CB_CHECK(!ia::find_prologue(decode("55")));
    CB_CHECK(!ia::find_prologue({}));
    CB_CHECK(!ia::find_epilogue(decode("90"), false));
    CB_CHECK(!ia::find_epilogue(decode("c3"), false));    // too short for any frame
    CB_CHECK(!ia::find_epilogue({}, false));
    // the 386 test and the instruction tables
    CB_CHECK(ia::is_386(decode("66b801000000")[0]));
    CB_CHECK(!ia::is_386(decode("b80100")[0]));
    CB_CHECK(ia::is_386(decode("0f840000")[0]));   // a near conditional jump: 0f 8x
    CB_CHECK(ia::is_jump("loopne") && ia::is_jump("jcxz") && !ia::is_jump("call"));
    CB_CHECK(ia::is_string_op("outsw") && !ia::is_string_op("xlat"));
});

CB_TEST("decompile.inline_asm.decoder_is_an_argument", [] {
    // the instructions may come from anywhere: here a decoder that hands back a fixed stream, and sees where the
    // function starts
    std::uint32_t seen_address = 0;
    std::size_t seen_size = 0;
    ia::Decoder fake = [&](cb::ByteView code, std::uint32_t address) {
        seen_address = address;
        seen_size = code.size();
        std::vector<ia::Instruction> ins;
        auto add = [&](const char* mnemonic, const char* op_str, std::uint8_t size = 1) {
            ia::Instruction i;
            i.address = address + static_cast<std::uint32_t>(ins.empty() ? 0 : ins.back().address - address + ins.back().size);
            i.size = size;
            i.mnemonic = mnemonic;
            i.op_str = op_str;
            ins.push_back(i);
        };
        add("push", "bp");
        add("mov", "bp, sp", 2);
        add("cli", "");
        add("pop", "bp");
        add("ret", "");
        return ins;
    };
    cb::Bytes code(20);
    auto out = ia::whole_body("g", code, 5, 12, {}, name_at, 1, "int2", fake);
    CB_CHECK(out.ok());
    if (out.ok()) CB_CHECK_EQ(out.value(), "int2 __near __cdecl g()\n{\n  _asm {\n        cli\n  }\n}");
    CB_CHECK_EQ(seen_address, 5u);
    CB_CHECK_EQ(seen_size, 7u);
    // an instruction stream that stops at the first return: nothing after it is looked at
});
