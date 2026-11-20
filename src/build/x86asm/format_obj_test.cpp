// Tests of the OMF object format (format_obj): each case plays what the assembler core sends a format for a small source (the
// source is in the comment) and compares the object with the one the reference nasm.exe (NASM 3.02, `-f obj`) wrote, record by
// record. The reference nasm.exe of CodeBreaker has no `obj2`, so that variant's different defaults are only checked for what they
// say (segments are 32-bit in group FLAT).
#include "build/x86asm/format_obj.hpp"

#include "build/x86asm/format_test_support.hpp"
#include "core/testing.hpp"

namespace {
using namespace cb::build::x86asm;
using namespace cb::build::x86asm::fmt_test;
using cb::Bytes;

// ---- the reference objects (from `nasm -f obj`; the file name in the THEADR is the one the source had) ----
constexpr const char* kO1 =
    "800800066f312e61736d63881e0000001a546865204e65747769646520417373656d626c657220332e30328e96160000"
    "04636f646504434f444504646174610444415441da9807002808000203012b9807002802000405012d900a0000010366"
    "6f6f0000001e8c0600036261720036a00c00010000b80100e80000ebf8cf9c05008404560180a006000200000100578a"
    "0700c10001010000ac";
constexpr const char* kO2 =
    "800800066f322e61736d62881e0000001a546865204e65747769646520417373656d626c657220332e30328e96250000"
    "04636f646504434f4445046461746104444154410362737303425353066467726f7570fe980700280100020301329807"
    "00a80e00040501a1980700600a00060701e99a040008ff025990080001020178000000ec8c09000665787473796d00bb"
    "a0050001000090caa0120002000000000000000000000000000000004c9c1d00c400140102c8025501c404140102c406"
    "040203e4085601c40c1601016f8a0700c10001010000ac";
constexpr const char* kO3 =
    "800800066f332e61736d61881e0000001a546865204e65747769646520417373656d626c657220332e30328e881400c0"
    "a0010003666f6f06757365723332036261725a881200c0a001010362617a06757365723332070093881600c0a002c008"
    "6d796578706f7274055f6d61696e0100a696180000055f5445585404434f4445055f444154410444415441ee98070069"
    "0b00020301e7980700a90800040501a6900c000001055f6d61696e0000005a8c0a00075f7072696e74660071b0170004"
    "6362756600616401026e62006204026662006132023888040040a20191a00f00010000e800000000b800000000c3ed9d"
    "0a00a401060101e40654026ca00c0002000078563412000000003e9d0500e4045401218b02000073";
constexpr const char* kO4 =
    "800800066f342e61736d60881e0000001a546865204e65747769646520417373656d626c657220332e30328e96070000"
    "0473656731ef980700280a07020101248c09000265610002656200da88040040a20191a0fc0301000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "00609cfd03c4005601c4025601c4045601c4065601c4085601c40a5601c40c5601c40e5601c4105601c4125601c41456"
    "01c4165601c4185601c41a5601c41c5601c41e5601c4205601c4225601c4245601c4265601c4285601c42a5601c42c56"
    "01c42e5601c4305601c4325601c4345601c4365601c4385601c43a5601c43c5601c43e5601c4405601c4425601c44456"
    "01c4465601c4485601c44a5601c44c5601c44e5601c4505601c4525601c4545601c4565601c4585601c45a5601c45c56"
    "01c45e5601c4605601c4625601c4645601c4665601c4685601c46a5601c46c5601c46e5601c4705601c4725601c47456"
    "01c4765601c4785601c47a5601c47c5601c47e5601c4805601c4825601c4845601c4865601c4885601c48a5601c48c56"
    "01c48e5601c4905601c4925601c4945601c4965601c4985601c49a5601c49c5601c49e5601c4a05601c4a25601c4a456"
    "01c4a65601c4a85601c4aa5601c4ac5601c4ae5601c4b05601c4b25601c4b45601c4b65601c4b85601c4ba5601c4bc56"
    "01c4be5601c4c05601c4c25601c4c45601c4c65601c4c85601c4ca5601c4cc5601c4ce5601c4d05601c4d25601c4d456"
    "01c4d65601c4d85601c4da5601c4dc5601c4de5601c4e05601c4e25601c4e45601c4e65601c4e85601c4ea5601c4ec56"
    "01c4ee5601c4f05601c4f25601c4f45601c4f65601c4f85601c4fa5601c4fc5601c4fe5601c5005601c5025601c50456"
    "01c5065601c5085601c50a5601c50c5601c50e5601c5105601c5125601c5145601c5165601c5185601c51a5601c51c56"
    "01c51e5601c5205601c5225601c5245601c5265601c5285601c52a5601c52c5601c52e5601c5305601c5325601c53456"
    "01c5365601c5385601c53a5601c53c5601c53e5601c5405601c5425601c5445601c5465601c5485601c54a5601c54c56"
    "01c54e5601c5505601c5525601c5545601c5565601c5585601c55a5601c55c5601c55e5601c5605601c5625601c56456"
    "01c5665601c5685601c56a5601c56c5601c56e5601c5705601c5725601c5745601c5765601c5785601c57a5601c57c56"
    "01c57e5601c5805601c5825601c5845601c5865601c5885601c58a5601c58c5601c58e5601c5905601c5925601c59456"
    "01c5965601c5985601c59a5601c59c5601c59e5601c5a05601c5a25601c5a45601c5a65601c5a85601c5aa5601c5ac56"
    "01c5ae5601c5b05601c5b25601c5b45601c5b65601c5b85601c5ba5601c5bc5601c5be5601c5c05601c5c25601c5c456"
    "01c5c65601c5c85601c5ca5601c5cc5601c5ce5601c5d05601c5d25601c5d45601c5d65601c5d85601c5da5601c5dc56"
    "01c5de5601c5e05601c5e25601c5e45601c5e65601c5e85601c5ea5601c5ec5601c5ee5601c5f05601c5f25601c5f456"
    "01c5f65601c5f85601c5fa5601c5fc5601fe9c5502c5fe5601c6005601c6025601c6045601c6065601c6085601c60a56"
    "01c60c5601c60e5601c6105601c6125601c6145601c6165601c6185601c61a5601c61c5601c61e5601c6205601c62256"
    "01c6245601c6265601c6285601c62a5601c62c5601c62e5601c6305601c6325601c6345601c6365601c6385601c63a56"
    "01c63c5601c63e5601c6405601c6425601c6445601c6465601c6485601c64a5601c64c5601c64e5601c6505601c65256"
    "01c6545601c6565601e6585602e65c5602e6605602e6645602e6685602e66c5602e6705602e6745602e6785602e67c56"
    "02e6805602e6845602e6885602e68c5602e6905602e6945602e6985602e69c5602e6a05602e6a45602e6a85602e6ac56"
    "02e6b05602e6b45602e6b85602e6bc5602e6c05602e6c45602e6c85602e6cc5602e6d05602e6d45602e6d85602e6dc56"
    "02e6e05602e6e45602e6e85602e6ec5602e6f05602e6f45602e6f85602e6fc5602e7005602e7045602e7085602e70c56"
    "02e7105602e7145602e7185602e71c5602e7205602e7245602e7285602e72c5602e7305602e7345602e7385602e73c56"
    "02e7405602e7445602e7485602e74c5602e7505602e7545602e7585602e75c5602e7605602e7645602e7685602e76c56"
    "02e7705602e7745602e7785602e77c5602e7805602e7845602e7885602e78c5602e7905602e7945602e7985602e79c56"
    "02e7a05602e7a45602e7a85602e7ac5602e7b05602e7b45602e7b85602e7bc5602e7c05602e7c45602e7c85602e7cc56"
    "02e7d05602e7d45602e7d85602e7dc5602e7e05602e7e45602e7e85602e7ec5602e7f05602e7f45602b5a0160301f803"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
    "0000000000000000000000000000000001004a9c1103e4005602e4045602e4085602e40c5602e4105602e4145602e418"
    "5602e41c5602e4205602e4245602e4285602e42c5602e4305602e4345602e4385602e43c5602e4405602e4445602e448"
    "5602e44c5602e4505602e4545602e4585602e45c5602e4605602e4645602e4685602e46c5602e4705602e4745602e478"
    "5602e47c5602e4805602e4845602e4885602e48c5602e4905602e4945602e4985602e49c5602e4a05602e4a45602e4a8"
    "5602e4ac5602e4b05602e4b45602e4b85602e4bc5602e4c05602e4c45602e4c85602e4cc5602e4d05602e4d45602e4d8"
    "5602e4dc5602e4e05602e4e45602e4e85602e4ec5602e4f05602e4f45602e4f85602e4fc5602e5005602e5045602e508"
    "5602e50c5602e5105602e5145602e5185602e51c5602e5205602e5245602e5285602e52c5602e5305602e5345602e538"
    "5602e53c5602e5405602e5445602e5485602e54c5602e5505602e5545602e5585602e55c5602e5605602e5645602e568"
    "5602e56c5602e5705602e5745602e5785602e57c5602e5805602e5845602e5885602e58c5602e5905602e5945602e598"
    "5602e59c5602e5a05602e5a45602e5a85602e5ac5602e5b05602e5b45602e5b85602e5bc5602e5c05602e5c45602e5c8"
    "5602e5cc5602e5d05602e5d45602e5d85602e5dc5602e5e05602e5e45602e5e85602e5ec5602e5f05602e5f45602e5f8"
    "5602e5fc5602e6005602e6045602e6085602e60c5602e6105602e6145602e6185602e61c5602e6205602e6245602e628"
    "5602e62c5602e6305602e6345602e6385602e63c5602e6405602e6445602e6485602e64c5602e6505602e6545602e658"
    "5602e65c5602e6605602e6645602e6685602e66c5602e6705602e6745602e6785602e67c5602e6805602e6845602e688"
    "5602e68c5602e6905602e6945602e6985602e69c5602e6a05602e6a45602e6a85602e6ac5602e6b05602e6b45602e6b8"
    "5602e6bc5602e6c05602e6c45602e6c85602e6cc5602e6d05602e6d45602e6d85602e6dc5602e6e05602e6e45602e6e8"
    "5602e6ec5602e6f05602e6f45602e6f85602e6fc5602e7005602e7045602e7085602e70c5602fc8a02000074";
constexpr const char* kO5 =
    "800800066f352e61736d5f881e0000001a546865204e65747769646520417373656d626c657220332e30328e961a0000"
    "0373746b05535441434b0376676104636f646504434f44458198070034000102030126980a000800b800100004010188"
    "9807004803000506010a900b0000020463656c6c000000bf900f00000000000662696776616c341200a088040040a201"
    "91a00700030000010203508a02000074";
constexpr const char* kO6 =
    "800800066f362e61736d5e881e0000001a546865204e65747769646520417373656d626c657220332e30328e960f0000"
    "0c5f5f4e41534d444546534547b498070028030002010132900a00000103666f6f0000001e88040040a20191a0070001"
    "0000010000579c0500c4015401458a02000074";
constexpr const char* kO7 =
    "800800064f372e41534ddd881e0000001a544845204e45545749444520415353454d424c455220332e30328e960c0000"
    "045345473104434f44452b98070028030002030130900e000001074c4f574e414d4500000047b01b0002433100628470"
    "1101024332006288002d310102433300616403578c07000445585431004788040040a20191a00700010000010000579c"
    "0500c4015604408a02000074";
constexpr const char* kO8 =
    "800800066f382e61736d5c881e0000001a546865204e65747769646520417373656d626c657220332e30328e96100000"
    "0474657874046461746103627373a898070028030002010132980700280400030101309807002804000401012f900a00"
    "000103666f6f0000001e8c060003626172003688040040a20191a00700010000e80000709c05008401560183a0080002"
    "000000000000569c0500e4005401268a02000074";

Sim::Factory obj_factory(bool obj2 = false) {
    return [obj2](FormatHost& host) { return std::make_unique<ObjFormat>(host, obj2); };
}

void expect_same(const Bytes& got, const char* want_hex) {
    const Bytes want = from_hex(want_hex);
    CB_CHECK_EQ(dump_omf(got), dump_omf(want));
    CB_CHECK(got == want);
}

std::string messages(const Sim& s) { return s.a.error_output(); }
}  // namespace

// segment code use16 class=CODE / ..start: / foo: mov ax,1 / call bar / jmp foo / segment data class=DATA / x: dw 1 / global foo / extern bar
CB_TEST("x86asm.obj.simple_16bit_module", [] {
    Sim s(obj_factory(), "o1.asm");
    const Bytes got = s.run([](Sim& s) {
        s.section("code use16 class=CODE");
        s.label("..start");
        s.label("foo", true);
        s.raw({0xb8, 0x01, 0x00});
        s.raw({0xe8});
        s.rel(2, s.xref("bar"), 0);
        s.raw({0xeb, 0xf8});
        s.section("data class=DATA");
        s.label("x");
        s.raw({0x01, 0x00});
        s.extern_("bar");
    });
    expect_same(got, kO1);
    CB_CHECK_EQ(messages(s), std::string());
});

// group dgroup data / data segment: dw x, dw seg x, dw x wrt dgroup, dw z wrt data, dd extsym, dw extsym wrt dgroup
CB_TEST("x86asm.obj.groups_and_wrt_fixups", [] {
    Sim s(obj_factory(), "o2.asm");
    const Bytes got = s.run([](Sim& s) {
        s.section("code use16 class=CODE");
        s.label("..start");
        s.raw({0x90});
        s.directive(D_GROUP, "dgroup data");
        s.section("data class=DATA align=4");
        s.label("x", true);
        s.addr(2, s.current(), 0);
        s.seg_base(2, s.current());
        s.addr(2, s.current(), 0, s.seg_of("dgroup"));
        s.addr(2, s.seg_of("z"), 0, s.seg_of("data"));
        s.addr(4, s.xref("extsym"), 0);
        s.addr(2, s.xref("extsym"), 0, s.seg_of("dgroup"));
        s.section("bss class=BSS private align=16");
        s.label("z");
        s.reserve(10);
        s.extern_("extsym");
    });
    expect_same(got, kO2);
});

// 32-bit segments (FIXU32), commons (default, near, far with element size), export and import
CB_TEST("x86asm.obj.use32_common_export_import", [] {
    Sim s(obj_factory(), "o3.asm");
    const Bytes got = s.run([](Sim& s) {
        s.section("_TEXT use32 class=CODE align=16 public");
        s.extern_("_printf");
        s.label("_main", true);
        s.raw({0xe8});
        s.rel(4, s.xref("_printf"), 0);
        s.raw({0xb8});
        s.addr(4, s.seg_of("_data1"), 0);
        s.raw({0xc3});
        s.section("_DATA use32 class=DATA align=4");
        s.label("_data1");
        s.raw({0x78, 0x56, 0x34, 0x12});
        s.addr(4, s.seg_of("_main"), 0);
        s.common("cbuf", 100);
        s.common("nb", 4, "near");
        s.common("fb", 100, "far 2");
        s.directive(D_EXPORT, "_main myexport 1 resident");
        s.directive(D_IMPORT, "foo user32 bar");
        s.directive(D_IMPORT, "baz user32 7");
    });
    expect_same(got, kO3);
});

// times 300 dw ea / times 300 dd eb: LEDATA and FIXUPP records continue into new ones, with the fixup size flag changing
CB_TEST("x86asm.obj.long_records_continue", [] {
    Sim s(obj_factory(), "o4.asm");
    const Bytes got = s.run([](Sim& s) {
        s.section("seg1 use16");
        s.extern_("ea");
        s.extern_("eb");
        for (int i = 0; i < 300; ++i) s.addr(2, s.xref("ea"), 0);
        for (int i = 0; i < 300; ++i) s.addr(4, s.xref("eb"), 0);
        s.raw({0x01, 0x00});
    });
    expect_same(got, kO4);
});

// stack and absolute segments, an absolute global (bigval equ 0x1234), a global in an absolute segment
CB_TEST("x86asm.obj.stack_absolute_and_far_publics", [] {
    Sim s(obj_factory(), "o5.asm");
    const Bytes got = s.run([](Sim& s) {
        s.section("stk stack class=STACK");
        s.reserve(256);
        s.section("vga absolute=0xB800");
        s.label("cell", true);
        s.reserve(16);
        s.section("code use16 class=CODE align=2");
        s.equ_global("bigval", 0x1234);
        s.raw({1, 2, 3});
    });
    expect_same(got, kO5);
});

// no segment directive: the default segment __NASMDEFSEG
CB_TEST("x86asm.obj.default_segment", [] {
    Sim s(obj_factory(), "o6.asm");
    const Bytes got = s.run([](Sim& s) {
        s.label("foo", true);
        s.raw({0x01});
        s.addr(2, s.current(), 0);
    });
    expect_same(got, kO6);
});

// uppercase, long common sizes (3- and 4-byte values), a far common with an element size
CB_TEST("x86asm.obj.uppercase_and_common_values", [] {
    Sim s(obj_factory(), "o7.asm");
    const Bytes got = s.run([](Sim& s) {
        s.directive(D_UPPERCASE, "");
        s.section("seg1 use16 class=Code");
        s.label("lowName", true);
        s.raw({0x01});
        s.common("c1", 70000, "near");
        s.common("c2", 20000000, "near");
        s.common("c3", 300, "far 3");
        s.extern_("ext1");
        s.addr(2, s.xref("ext1"), 0);
    });
    expect_same(got, kO7);
});

// `.text`, `.data` and `.bss` sections: the leading dot is dropped from the OMF name
CB_TEST("x86asm.obj.dotted_section_names", [] {
    Sim s(obj_factory(), "o8.asm");
    const Bytes got = s.run([](Sim& s) {
        s.section(".text");
        s.extern_("bar");
        s.label("foo", true);
        s.raw({0xe8});
        s.rel(2, s.xref("bar"), 0);
        s.section(".data");
        s.label("x");
        s.addr(4, s.seg_of("foo"), 0);
        s.section(".bss");
        s.reserve(4);
    });
    expect_same(got, kO8);
});

// the messages: bad and rounded alignments, a group naming a segment that never appears, `extern foo:far`, bad export lines
CB_TEST("x86asm.obj.messages", [] {
    {
        Sim s(obj_factory(), "m1.asm");
        s.run([](Sim& s) {
            s.at(1);
            s.section("a align=3");
            s.raw({1});
        });
        CB_CHECK_EQ(s.a.diagnostics().front().text, std::string("m1.asm:1: error: invalid alignment value 3"));
    }
    {
        Sim s(obj_factory(), "m2.asm");
        s.run([](Sim& s) {
            s.at(1);
            s.section("a align=8");
            s.raw({1});
            s.at(3);
            s.section("b align=64");
            s.raw({1});
        });
        CB_CHECK_EQ(s.a.error_output(),
                    std::string("m2.asm:1: warning: alignment of 8 not supported, using 16 [-w+section-alignment-rounded]\n"
                                "m2.asm:3: warning: alignment of 64 not supported, using 256 [-w+section-alignment-rounded]\n"));
    }
    {
        Sim s(obj_factory(), "m3.asm");
        s.run([](Sim& s) {
            s.directive(D_GROUP, "g nothere");
            s.section("s");
            s.raw({1});
        });
        CB_CHECK_EQ(s.a.error_output(), std::string("m3.asm: error: group `g' contains undefined segment `nothere'\n"));
    }
    {
        Sim s(obj_factory(), "m4.asm");
        s.run([](Sim& s) {
            s.at(2);
            s.section("s");
            s.extern_("foo", "far");
        });
        CB_CHECK_EQ(s.a.diagnostics().front().text,
                    std::string("m4.asm:2: error: `foo': `far' keyword may only be applied to common variables\n"));
    }
    {
        Sim s(obj_factory(), "m8.asm");
        s.run([](Sim& s) {
            s.at(1);
            s.section("s use16 align=4096");
            s.raw({1});
            s.at(3);
            s.section("t align=8192");
            s.raw({2});
        });
        CB_CHECK_EQ(s.a.diagnostics().front().text, std::string("m8.asm:3: error: invalid alignment value 8192"));
    }
    {
        Sim s(obj_factory(), "m9.asm");
        s.run([](Sim& s) {
            s.at(1);
            s.directive(D_IMPORT, "foo");
            s.at(3);
            s.directive(D_EXPORT, "a b parm=x");
            s.at(4);
            s.directive(D_EXPORT, "a b zz");
            s.section("s");
            s.raw({1});
        });
        CB_CHECK_EQ(s.a.diagnostics()[0].text, std::string("m9.asm:1: error: `import' directive requires symbol name and library name"));
        CB_CHECK_EQ(s.a.diagnostics()[1].text, std::string("m9.asm:3: error: value `x' for `parm' is non-numeric"));
        CB_CHECK_EQ(s.a.diagnostics()[2].text, std::string("m9.asm:4: error: unrecognised export qualifier `zz'"));
    }
});

// -f obj2: every segment is 32-bit and in group FLAT, with the Unix section names mapped by the format's definitions
CB_TEST("x86asm.obj.obj2_defaults", [] {
    Sim s(obj_factory(true), "t2.asm");
    const Bytes got = s.run([](Sim& s) {
        s.section("TEXT32");
        s.label("foo", true);
        s.raw({0xc3});
    });
    CB_CHECK_EQ(s.predefined.size(), std::size_t{4});
    CB_CHECK_EQ(s.predefined[0], std::string(".text=TEXT32"));
    const std::string d = dump_omf(got);
    // LNAMES: "", TEXT32, CODE (the default class), FLAT; SEGDEF is 32-bit (P bit), aligned 16 (0x60), public (2 << 2);
    // GRPDEF: the group FLAT (LNAME 4) names no segment (a segment is in it by default only)
    CB_CHECK(d.find("LNAMES 96 len=18 ok 000654455854333204434f444504464c4154") != std::string::npos);
    CB_CHECK(d.find("SEGDEF 98 len=6 ok 690100020301") != std::string::npos);
    CB_CHECK(d.find("GRPDEF 9a len=1 ok 04") != std::string::npos);
});

// The record builder itself.
CB_TEST("x86asm.obj.record_builder_encodings", [] {
    Assembly a;
    Bytes out;
    ObjWriter w(a, out);
    auto orp = w.make();
    ObjRecord* r = orp.get();
    r->type = omf::LNAMES;
    // index: one byte below 128, two bytes (high byte first, bit 7 set) from 128
    w.index(r, 5);
    w.index(r, 127);
    w.index(r, 128);
    w.index(r, 300);
    // value: 1 byte up to 128, then 129 + word, 132 + 3 bytes, 136 + dword
    w.value(r, 128);
    w.value(r, 129);
    w.value(r, 0x10000);
    w.value(r, 0x1000000);
    w.emit2(r);
    CB_CHECK_EQ(dump_omf(out), std::string("LNAMES 96 len=19 ok 057f8080812c80" "818100" "84000001" "8800000001\n"));
});

// A name longer than 255 bytes is cut with a warning; an upper-case writer upper-cases names
CB_TEST("x86asm.obj.record_builder_names", [] {
    Assembly a;
    Bytes out;
    ObjWriter w(a, out);
    auto orp = w.make();
    ObjRecord* r = orp.get();
    r->type = omf::THEADR;
    w.name(r, std::string(300, 'a'));
    w.emit2(r);
    CB_CHECK_EQ(out.size(), std::size_t{3 + 1 + 255 + 1});
    CB_CHECK_EQ(out[3], std::uint8_t{255});
    CB_CHECK_EQ(a.error_output(), "nasm: warning: truncating object name `" + std::string(64, 'a') + "...' to 255 bytes [-w+other]\n");
    Bytes out2;
    ObjWriter up(a, out2);
    up.uppercase = true;
    auto o2 = up.make();
    o2->type = omf::THEADR;
    up.name(o2.get(), "MixEd_1");
    up.emit2(o2.get());
    CB_CHECK_EQ(dump_omf(out2).substr(0, 23), std::string("THEADR 80 len=8 ok 074d"));
});
