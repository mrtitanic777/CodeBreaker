// Tests of the COFF formats (format_coff): each case plays what the assembler core sends a format for a small source (the source is
// in the comment) and compares the object with the one the reference nasm.exe (NASM 3.02, `-f win32` / `-f win64`) wrote, section
// by section, relocation by relocation and symbol by symbol. The reference's time stamp is given to the run (a real assembly
// uses the time it started). The reference nasm.exe of CodeBreaker has no `-f coff`, so the standard COFF's own rules are
// checked against what NASM's source says (no reference object).
#include "cb/build/x86asm/format_coff.hpp"

#include "cb/build/x86asm/format_test_support.hpp"
#include "cb/core/testing.hpp"

namespace {
using namespace cb::build::x86asm;
using namespace cb::build::x86asm::fmt_test;
using cb::Bytes;

// ---- the reference objects (from `nasm -f win32` and `-f win64`; the .file record has the source's name) ----
constexpr const char* kC1 =
    "4c0103009cc0ff6adb0000000f000000000000002e7465787400000000000000000000000e0000008c0000009a000000"
    "0000000002000000200050602e6461746100000000000000000000000f000000ae000000bd0000000000000003000000"
    "400030c02e6273730000000000000000000000004000000000000000000000000000000000000000800030c068000000"
    "00e80000000083c404c30100000004000000060006000000090000001400686900000000000000000000000000030000"
    "00020000000600070000000400000006000b0000000900000006002e66696c6500000000000000feff0000670163312e"
    "61736d0000000000000000000000002e74657874000000000000000100000003010e0000000200000000000000000000"
    "0000002e64617461000000000000000200000003010f00000003000000000000000000000000002e6273730000000000"
    "0000000300000003014000000000000000000000000000000000002e6162736f6c757400000000ffff000003005f7075"
    "7473000000000000000000000002005f6d61696e000000000000000100000002006d7367000000000000000000020000"
    "00030074626c00000000000300000002000000030062756600000000000000000003000000030040666561742e303001"
    "000000ffff0000030004000000";
constexpr const char* kC2 =
    "4c0106009cc0ff6a2d01000013000000000000002e74657874000000000000000000000006000000040100000a010000"
    "0000000000000000200050602e647265637476650000000000000000160000000a010000200100000000000000000000"
    "000a10002e7378646174610000000000000000000400000020010000240100000000000000000000000200002f303030"
    "3030303400000000000000000400000024010000280100000000000000000000400040402e7465787424780000000000"
    "000000000100000028010000290100000000000000000000201050602e696e666f000000000000000000000004000000"
    "290100002d0100000000000000000000000a1000c3c30f0000002d6578706f72743a5f66202d6578706f72743a5f6720"
    "0f00000001000000c3050000002e66696c6500000000000000feff0000670163322e61736d0000000000000000000000"
    "002e74657874000000000000000100000003010600000000000000000000000000000000002e64726563747665000000"
    "000200000003011600000000000000000000000000000000002e73786461746100000000000300000003010400000000"
    "000000000000000000000000002e7264617461246c000000000400000003010400000000000000000000000000000000"
    "002e746578742478000000000005000000030101000000000000000a936d020000020000002e696e666f000000000000"
    "000600000003010400000000000000000000000000000000002e6162736f6c757400000000ffff000003005f66000000"
    "000000000000000100200002005f67000000000000010000000100000003006d79636400000000000000000500000003"
    "0040666561742e303001000000ffff00000300220000002e7264617461246c6f6e675f73656374696f6e5f6e616d655f"
    "6865726500";
constexpr const char* kC3 =
    "648603009cc0ff6a040100000e000000000000002e7465787400000000000000000000001e0000008c000000aa000000"
    "0000000004000000200050602e64617461000000000000000000000010000000d2000000e20000000000000002000000"
    "400030c02e70646174610000000000000000000004000000f6000000fa000000000000000100000040003040488d0500"
    "000000e800000000833dffffffff0548b80000000000000000c303000000040000000400080000000900000004000e00"
    "000004000000040015000000040000000100010000000000000000000000000000000400000004000000030008000000"
    "0a000000010000000000000000000200000003002e66696c6500000000000000feff0000670163332e61736d00000000"
    "00000000000000002e74657874000000000000000100000003011e00000004000000000000000000000000002e646174"
    "61000000000000000200000003011000000002000000000000000000000000002e706461746100000000000003000000"
    "03010400000001000000000000000000000000002e6162736f6c757400000000ffff0000030070757473000000000000"
    "00000000000002006578740000000000000000000000000002006d61696e00000000000000000100000002006d736700"
    "000000000000000002000000030074000000000000000400000002000000030004000000";
constexpr const char* kC5 =
    "4c010200edc1ff6a6900000008000000000000002e646174610000000000000000000000050000006400000069000000"
    "0000000000000000400030c02e6273730000000000000000000000000800000000000000000000000000000000000000"
    "800030c001000000002e66696c6500000000000000feff0000670163352e61736d0000000000000000000000002e6461"
    "7461000000000000000100000003010500000000000000000000000000000000002e6273730000000000000000020000"
    "0003010800000000000000000000000000000000002e6162736f6c757400000000ffff0000030040666561742e303001"
    "000000ffff0000030004000000";
constexpr const char* kC6 =
    "4c010100b0c1ff6a3d00000007000000000000002e746578740000000000000000000000010000003c0000003d000000"
    "000000000000000020005060c32e66696c6500000000000000feff0000670163362e61736d0000000000000000000000"
    "002e74657874000000000000000100000003010100000000000000000000000000000000002e6162736f6c7574000000"
    "00ffff00000300666f6f00000000000000000001002000020040666561742e303001000000ffff0000030004000000";
constexpr const char* kC8 =
    "4c010200b2c1ff6a6900000009000000000000002e746578742461000000000000000000010000006400000065000000"
    "0000000000000000201050602e7864617461246100000000000000000400000065000000690000000000000000000000"
    "40104040c3010000002e66696c6500000000000000feff0000670163382e61736d0000000000000000000000002e7465"
    "78742461000000000001000000030101000000000000000a936d020000020000002e7864617461246100000000020000"
    "00030104000000000000006567bcb80100050000002e6162736f6c757400000000ffff00000300663100000000000000"
    "00000001000000030040666561742e303001000000ffff0000030004000000";
constexpr const char* kC9 =
    "4c010300b3c1ff6a920000000b000000000000002e746578742462000000000000000000010000008c0000008d000000"
    "0000000000000000201050602e786461746124620000000000000000040000008d000000910000000000000000000000"
    "401040402e746578742462000000000000000000010000009100000092000000000000000000000020005060c3020000"
    "00902e66696c6500000000000000feff0000670163392e61736d0000000000000000000000002e746578742462000000"
    "000001000000030101000000000000000a936d020000020000002e786461746124620000000002000000030104000000"
    "000000008bc809aa0100050000002e746578742462000000000003000000030101000000000000000000000000000000"
    "00002e6162736f6c757400000000ffff0000030066320000000000000000000001000000030040666561742e30300100"
    "0000ffff0000030004000000";
constexpr const char* kC10 =
    "4c010200b5c1ff6a6500000008000000000000002e646174610000000000000000000000010000006400000065000000"
    "0000000000000000400030c02e6273730000000000000000000000000400000000000000000000000000000000000000"
    "800030c0012e66696c6500000000000000feff000067016331302e61736d00000000000000000000002e646174610000"
    "00000000000100000003010100000000000000000000000000000000002e627373000000000000000002000000030104"
    "00000000000000000000000000000000002e6162736f6c757400000000ffff0000030040666561742e303001000000ff"
    "ff0000030004000000";

Sim::Factory coff_factory(CoffKind kind) {
    return [kind](FormatHost& host) { return std::make_unique<CoffFormat>(host, kind); };
}

std::int64_t stamp_of(const char* want_hex) { return le(from_hex(want_hex), 4, 4); }

void expect_same(const Bytes& got, const char* want_hex) {
    const Bytes want = from_hex(want_hex);
    CB_CHECK_EQ(dump_coff(got), dump_coff(want));
    CB_CHECK(got == want);
}
}  // namespace

// section .text / global _main / extern _puts / push msg / call _puts / add esp,4 / ret / section .data / msg: db 'hi',0 /
// tbl: dd _main, msg, _puts / section .bss / buf: resb 64
CB_TEST("x86asm.coff.win32_text_data_bss", [] {
    Sim s(coff_factory(CoffKind::Win32), "c1.asm", stamp_of(kC1));
    const Bytes got = s.run([](Sim& s) {
        s.section(".text");
        s.extern_("_puts");
        s.label("_main", true);
        s.raw({0x68});
        s.addr(4, s.seg_of("msg"), 0);
        s.raw({0xe8});
        s.rel(4, s.xref("_puts"), 0);
        s.raw({0x83, 0xc4, 0x04});
        s.raw({0xc3});
        s.section(".data");
        s.label("msg");
        s.raw({'h', 'i', 0});
        s.label("tbl");
        s.addr(4, s.seg_of("_main"), 0);
        s.addr(4, s.seg_of("msg"), 0);
        s.addr(4, s.xref("_puts"), 0);
        s.section(".bss");
        s.label("buf");
        s.reserve(64);
    });
    expect_same(got, kC1);
    CB_CHECK_EQ(s.a.error_output(), std::string());
});

// exports (.drectve), safeseh (.sxdata), a long section name (the string table), a COMDAT section, an info section and a
// `wrt ..symtab` value
CB_TEST("x86asm.coff.win32_exports_safeseh_comdat_symtab", [] {
    Sim s(coff_factory(CoffKind::Win32), "c2.asm", stamp_of(kC2));
    const Bytes got = s.run([](Sim& s) {
        s.section(".text align=16");
        s.directive(D_EXPORT, "_f");
        s.directive(D_EXPORT, "_g");
        s.label("_f", true);
        s.raw({0xc3});
        s.label("_g");
        s.raw({0xc3});
        s.directive(D_SAFESEH, "_f");
        s.section(".rdata$long_section_name_here rdata");
        s.raw({1, 0, 0, 0});
        s.section(".text$x code comdat=2:mycd");
        s.label("mycd");
        s.raw({0xc3});
        s.section(".info info");
        s.raw({5, 0, 0, 0});
        s.section(".text");
        s.addr(4, s.seg_of("_f"), 0, s.seg_of("..symtab"));
    });
    expect_same(got, kC2);
});

// bits 64 / lea rax,[msg] / call puts / cmp dword [msg],5 / mov rax,msg / dd msg wrt ..imagebase / dq ext / .pdata: dd main
CB_TEST("x86asm.coff.win64_relocations_imagebase_pdata", [] {
    Sim s(coff_factory(CoffKind::Win64), "c3.asm", stamp_of(kC3));
    const Bytes got = s.run([](Sim& s) {
        s.bits = 64;
        s.section(".text");
        s.extern_("puts");
        s.extern_("ext");
        s.label("main", true);
        s.raw({0x48, 0x8d, 0x05});
        s.rel(4, s.seg_of("msg"), 0);
        s.raw({0xe8});
        s.rel(4, s.xref("puts"), 0);
        s.raw({0x83, 0x3d});
        s.rel(4, s.seg_of("msg"), 0, 1);
        s.raw({0x05});
        s.raw({0x48, 0xb8});
        s.addr(8, s.seg_of("msg"), 0);
        s.raw({0xc3});
        s.section(".data");
        s.label("msg");
        s.raw({1, 0, 0, 0});
        s.label("t");
        s.addr(4, s.seg_of("msg"), 0, s.seg_of("..imagebase"));
        s.addr(8, s.xref("ext"), 0);
        s.section(".pdata");
        s.addr(4, s.seg_of("main"), 0);
    });
    expect_same(got, kC3);
});

// global foo:function (the symbol type)
CB_TEST("x86asm.coff.function_symbol_type", [] {
    Sim s(coff_factory(CoffKind::Win32), "c6.asm", stamp_of(kC6));
    const Bytes got = s.run([](Sim& s) {
        s.section(".text");
        s.label("foo", true, "function");
        s.raw({0xc3});
    });
    expect_same(got, kC6);
});

// COMDAT: an associative section after its target, and before it (a placeholder section is made and then turned into the target)
CB_TEST("x86asm.coff.comdat_associative_and_placeholder", [] {
    {
        Sim s(coff_factory(CoffKind::Win32), "c8.asm", stamp_of(kC8));
        const Bytes got = s.run([](Sim& s) {
            s.section(".text$a code comdat=2:f1");
            s.label("f1");
            s.raw({0xc3});
            s.section(".xdata$a rdata comdat=5:f1");
            s.raw({1, 0, 0, 0});
        });
        expect_same(got, kC8);
    }
    {
        Sim s(coff_factory(CoffKind::Win32), "c9.asm", stamp_of(kC9));
        const Bytes got = s.run([](Sim& s) {
            s.section(".xdata$b rdata comdat=5:f2");
            s.raw({2, 0, 0, 0});
            s.section(".text$b code comdat=2:f2");
            s.label("f2");
            s.raw({0xc3});
            s.section(".text$b");
            s.raw({0x90});
        });
        expect_same(got, kC9);
    }
});

// space reserved in a data section is zeroed (with a warning); data in a BSS section is ignored (with a warning)
CB_TEST("x86asm.coff.bss_and_zeroing_messages", [] {
    {
        Sim s(coff_factory(CoffKind::Win32), "c5.asm", stamp_of(kC5));
        const Bytes got = s.run([](Sim& s) {
            s.at(1);
            s.section(".data");
            s.raw({1});
            s.at(3);
            s.reserve(4);
            s.at(4);
            s.section(".bss");
            s.reserve(8);
        });
        expect_same(got, kC5);
        CB_CHECK_EQ(s.a.error_output(), std::string("c5.asm:3: warning: uninitialised space declared in non-BSS section `.data': zeroing [-w+zeroing]\n"));
    }
    {
        Sim s(coff_factory(CoffKind::Win32), "c10.asm", stamp_of(kC10));
        const Bytes got = s.run([](Sim& s) {
            s.at(1);
            s.section(".data");
            s.raw({1});
            s.at(3);
            s.section(".bss");
            s.at(4);
            s.raw({2});
            s.reserve(3);
        });
        expect_same(got, kC10);
        CB_CHECK_EQ(s.a.error_output(), std::string("c10.asm:4: warning: attempt to initialize memory in BSS section `.bss': ignored [-w+other]\n"));
    }
});

// the error messages of the Win32 format
CB_TEST("x86asm.coff.messages", [] {
    {
        Sim s(coff_factory(CoffKind::Win32), "w1.asm");   // call far foo
        s.run([](Sim& s) {
            s.at(1);
            s.section(".text");
            s.extern_("foo");
            s.at(4);
            s.addr(2, s.xref("foo"), 0);
        });
        CB_CHECK_EQ(s.a.diagnostics().front().text, std::string("w1.asm:4: error: COFF format does not support non-32-bit relocations"));
    }
    {
        Sim s(coff_factory(CoffKind::Win32), "w3.asm");   // bad alignments, bad export lines
        s.run([](Sim& s) {
            s.at(1);
            s.section(".text foo=1 align=3");
            s.at(2);
            s.section(".xx align=16384");
            s.at(7);
            s.directive(D_EXPORT, "a b");
        });
        CB_CHECK_EQ(s.a.diagnostics()[0].text, std::string("w3.asm:1: error: argument to `align' is not a power of two"));
        CB_CHECK_EQ(s.a.diagnostics()[1].text, std::string("w3.asm:2: error: maximum alignment in COFF is 8192 bytes"));
        CB_CHECK_EQ(s.a.diagnostics()[2].text, std::string("w3.asm:7: error: unrecognized export qualifier `b'"));
    }
    {
        Sim s(coff_factory(CoffKind::Win32), "w4.asm");   // global bar:bogus
        s.run([](Sim& s) {
            s.at(1);
            s.section(".text");
            s.at(4);
            s.label("bar", true, "bogus");
        });
        CB_CHECK_EQ(s.a.diagnostics().front().text, std::string("w4.asm:4: error: unrecognised symbol type `bogus' on `bar'"));
    }
    {
        Sim s(coff_factory(CoffKind::Win32), "w5.asm");   // ..foo: nop
        s.run([](Sim& s) {
            s.at(1);
            s.section(".text");
            s.at(2);
            s.label("..foo");
        });
        CB_CHECK_EQ(s.a.diagnostics().front().text, std::string("w5.asm:2: error: unrecognized special symbol `..foo'"));
    }
});

// -f coff (no reference object: nasm.exe is built without it): the DOS-style flags, header flags 0x104, the common variable fix,
// and relative relocations counted from the start of the section
CB_TEST("x86asm.coff.standard_coff_rules", [] {
    Sim s(coff_factory(CoffKind::Coff), "c4.asm", 0x12345678);
    const Bytes got = s.run([](Sim& s) {
        s.section(".text");
        s.extern_("ext");
        s.label("main", true);
        s.raw({0xe8});
        s.rel(4, s.xref("ext"), 0);          // call ext
        s.raw({0x8b, 0x05});
        s.addr(4, s.xref("ext"), 0);         // mov eax,[ext]
        s.raw({0x8b, 0x05});
        s.addr(4, s.seg_of("cvar"), 0);      // mov eax,[cvar]
        s.section(".data");
        s.common("cvar", 4);
        s.label("x");
        s.addr(4, s.seg_of("main"), 0);
    });
    const std::string d = dump_coff(got);
    CB_CHECK_EQ(le(got, 0, 2), 0x014cu);
    CB_CHECK_EQ(le(got, 4, 4), 0x12345678u);
    CB_CHECK_EQ(le(got, 18, 2), 0x104u);                  // header flags: 32-bit, no line numbers
    CB_CHECK(d.find("flags 00000020") != std::string::npos);   // .text: code only
    CB_CHECK(d.find("flags 00000040") != std::string::npos);   // .data: initialized data only
    // call ext: relative to the section start in standard COFF: 0 - (4 + 1) = -5
    CB_CHECK(d.find("data e8fbffffff") != std::string::npos);
    // the .text data: call (-5), mov eax,[ext] (+0), mov eax,[cvar] (the common variable's size, 4, as the fix of a standard COFF)
    CB_CHECK(d.find("data e8fbffffff8b05000000008b0504000000") != std::string::npos);
});

// sectalign raises an alignment, align=0 means the default, a section keeps its flags on redeclaration
CB_TEST("x86asm.coff.alignment_rules", [] {
    Sim s(coff_factory(CoffKind::Win32), "t.asm", 1);
    const Bytes got = s.run([](Sim& s) {
        s.section(".text align=64");
        s.raw({0xc3});
        s.section(".data");
        s.raw({1});
        s.format->sectalign(s.current(), 256);
        s.section(".text align=8");   // narrower: ignored
    });
    const std::string d = dump_coff(got);
    // .text: align=64 (0x00700000 | code | exec | read); .data: raised to 256 by sectalign (0x00900000)
    CB_CHECK(d.find("flags 60700020") != std::string::npos);
    CB_CHECK(d.find("flags c0900040") != std::string::npos);
});
