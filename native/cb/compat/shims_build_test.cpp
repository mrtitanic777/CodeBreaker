// Tests of the staged shims of the build lane (compat/shims/codebreaker/build/*): the old API over the new modules. Built at
// /W3 (the old layer is not warning-clean), like shims_test.cpp. The shims that call old code not linked here (the Z80 and
// 6502 instruction tables, the NE/PE/OMF readers, dos_tools, the corpus) are compile-checked by including them; their logic
// is the new modules' own, tested in cb/build/.
#include "shims/codebreaker/build/definitions.hpp"
#include "shims/codebreaker/build/dos_sessions.hpp"
#include "shims/codebreaker/build/exepack.hpp"
#include "shims/codebreaker/build/m6502_asm.hpp"
#include "shims/codebreaker/build/msvc152.hpp"
#include "shims/codebreaker/build/mzfile.hpp"
#include "shims/codebreaker/build/period16.hpp"
#include "shims/codebreaker/build/ppc_asm.hpp"
#include "shims/codebreaker/build/stubjoin.hpp"
#include "shims/codebreaker/build/vxdfile.hpp"
#include "shims/codebreaker/build/watcom.hpp"
#include "shims/codebreaker/build/z80_asm.hpp"

#include "cb/core/testing.hpp"

namespace old_mz = codebreaker::build::mzfile;
namespace old_vxd = codebreaker::build::vxdfile;
namespace old_stub = codebreaker::build::stubjoin;
namespace old_exe = codebreaker::build::exepack;
namespace old_ppc = codebreaker::build::ppc_asm;
namespace old_z80 = codebreaker::build::z80_asm;
namespace old_6502 = codebreaker::build::m6502_asm;
namespace old_def = codebreaker::build::definitions;
namespace old_ow = codebreaker::build::watcom;
namespace old_dos = codebreaker::build::dos_sessions;

namespace {

py::bytes bytes_of(std::initializer_list<int> v) {
    std::string s;
    for (int b : v) s.push_back(static_cast<char>(b));
    return py::bytes(s);
}

}  // namespace

CB_TEST("compat.shims.build_mzfile", [] {
    std::string header(0x40, '\0');
    header[0] = 'M';
    header[1] = 'Z';
    header[0x06] = 2;     // two relocations
    header[0x18] = 0x20;  // table at 0x20
    for (int i = 0x20; i < 0x28; ++i) header[i] = static_cast<char>(i);
    header[0x30] = 'X';   // data behind the table is kept
    const py::bytes tpl = old_mz::template_(py::bytes(header));
    CB_CHECK_EQ(tpl.size(), 0x40);
    CB_CHECK_EQ(static_cast<int>(tpl.byte_(0x06)), 0);
    CB_CHECK_EQ(static_cast<int>(tpl.byte_(0x20)), 0);
    CB_CHECK_EQ(static_cast<int>(tpl.byte_(0x30)), 'X');
    py::list<std::tuple<int64_t, int64_t>> relocs;
    relocs.append(std::make_tuple(int64_t{0x10}, int64_t{0x1}));
    const py::bytes file = old_mz::compose(tpl, py::bytes(std::string(100, '\x90')), relocs, 1, 2, 3, 4, 5);
    CB_CHECK_EQ(file.size(), 164);
    CB_CHECK_EQ(static_cast<int>(file.byte_(0x06)), 1);
    CB_CHECK_EQ(static_cast<int>(file.byte_(0x14)), 2);   // ip
    CB_CHECK_EQ(static_cast<int>(file.byte_(0x16)), 1);   // cs
    CB_CHECK_EQ(static_cast<int>(file.byte_(0x20)), 0x10);
    CB_CHECK_EQ(old_mz::BUILT_.size(), 8);
    CB_CHECK_THROWS_AS(py::StructError, (void)old_mz::template_(py::bytes(std::string(4, '\0'))));
    CB_CHECK_THROWS_AS(py::StructError, (void)old_mz::compose(tpl, py::bytes(), py::list<std::tuple<int64_t, int64_t>>(), 0, 0, 0, 0, 70000));
});

CB_TEST("compat.shims.build_vxdfile", [] {
    // an LE header at 0x80 with a name table holding a description and one export
    std::string b(0x80 + 0xC0, '\0');
    b[0] = 'M';
    b[1] = 'Z';
    b[0x3C] = static_cast<char>(0x80);
    auto put32 = [&](size_t at, uint32_t v) { for (int i = 0; i < 4; ++i) b[at + i] = static_cast<char>(v >> (8 * i)); };
    const std::string names = std::string("\x05") + "Hello" + std::string("\x00\x00", 2) + "\x01" "F" + std::string("\x02\x00", 2) + std::string("\x00", 1);
    put32(0x80 + 0x80, 0x140);
    put32(0x80 + 0x88, 0x140);
    put32(0x80 + 0x8C, static_cast<uint32_t>(names.size()));
    b += names;
    CB_CHECK_EQ(old_vxd::PAGE_ALIGN, 0x200);
    CB_CHECK_EQ(old_vxd::_u(py::bytes(b), 0x3C), 0x80);
    CB_CHECK_EQ(old_vxd::definitions(py::bytes(b), 0x80).utf8(), "DESCRIPTION 'Hello'\nEXPORTS\n    F @2\n");
    const py::bytes out = old_vxd::relay(py::bytes(b), 0, py::bytes());
    CB_CHECK_EQ(out.size(), 0x200 + static_cast<int64_t>(names.size()));   // the header padded to the boundary, no pages, the names
    CB_CHECK_THROWS_AS(py::StructError, (void)old_vxd::_u(py::bytes(std::string(4, '\0')), 2));
});

CB_TEST("compat.shims.build_exepack_roundtrip", [] {
    std::string image;
    for (int i = 0; i < 100; ++i) image.push_back(static_cast<char>(i));
    image += std::string(200, '\0');
    image += std::string("tail-bytes-of-the-program");
    py::list<old_exe::Fill> fills;
    fills.append(old_exe::Fill{100, 200, 0});
    py::list<int64_t> relocs;
    relocs.append(0x10);
    relocs.append(0x20);
    const py::bytes stub = py::bytes(std::string(20, '\x90') + "Packed file is corrupt");
    auto [stored, regs] = old_exe::pack(py::bytes(image), relocs, std::make_tuple(int64_t{3}, int64_t{0x44}), std::make_tuple(int64_t{0x50}, int64_t{0x200}),
                                        fills, stub);
    CB_CHECK_EQ(std::get<1>(regs), 16);
    CB_CHECK_EQ(std::get<3>(regs), 0x80);
    const old_exe::Layout l = old_exe::layout(stored, std::get<0>(regs), std::get<1>(regs));
    CB_CHECK_EQ(l.fills.size(), 1);
    CB_CHECK_EQ(std::get<0>(l.fills[0]), 100);
    CB_CHECK_EQ(std::get<1>(l.fills[0]), 200);
    CB_CHECK_EQ(l.words, 8);
    CB_CHECK_EQ(l.stub_code.size(), stub.size());
    CB_CHECK_EQ(old_exe::MESSAGE.size(), 22);
    CB_CHECK_THROWS_AS(py::ValueError, (void)old_exe::layout(stored, std::get<0>(regs), 0x14));
    CB_CHECK_THROWS_AS(py::ValueError, (void)old_exe::pack(py::bytes(image), relocs, std::make_tuple(int64_t{3}, int64_t{0x44}),
                                                           std::make_tuple(int64_t{0x50}, int64_t{0x200}), py::list<old_exe::Fill>{old_exe::Fill{0, 10, 0}}, stub));
});

CB_TEST("compat.shims.build_stubjoin_errors_are_the_old_ones", [] {
    std::string stub(0x40, '\0');
    stub[0] = 'M';
    std::string neither(0x100, '\0');
    neither[0x3C] = static_cast<char>(0x80);
    neither[0x80] = 'L';
    CB_CHECK_THROWS_AS(py::ValueError, (void)old_stub::join(py::bytes(stub), py::bytes(neither)));   // "not a PE or NE module"
    CB_CHECK_THROWS_AS(py::StructError, (void)old_stub::join(py::bytes(stub), py::bytes(std::string(8, '\0'))));
});

CB_TEST("compat.shims.build_ppc_asm", [] {
    const py::bytes code = old_ppc::assemble(py::str("li r3, 1\nblr\n"), 0x82000000);
    CB_CHECK_EQ(code.size(), 8);
    CB_CHECK_EQ(static_cast<int>(code.byte_(0)), 0x38);
    CB_CHECK_EQ(static_cast<int>(code.byte_(3)), 0x01);
    CB_CHECK_EQ(static_cast<int>(code.byte_(4)), 0x4E);
    CB_CHECK_EQ(*old_ppc::assemble_one(py::str("b 0x82001000"), 0x82000000), 0x48001000);
    CB_CHECK(!old_ppc::assemble_one(py::str("# nothing")).has_value());
    py::dict<py::str, int64_t> labels;
    labels.set(py::str("top"), 0x82000000);
    CB_CHECK_EQ(*old_ppc::assemble_one(py::str("b top"), 0x82000004, labels), 0x4BFFFFFC);
    CB_CHECK_THROWS_AS(old_ppc::AsmError, (void)old_ppc::assemble(py::str("frobnicate r1")));
    CB_CHECK_EQ(old_ppc::MASK32, 0xFFFFFFFF);
});

CB_TEST("compat.shims.build_z80_text_helpers_and_error_type", [] {
    CB_CHECK_EQ(old_z80::_strip(py::str("LD A,5   ; load")).utf8(), "LD A,5");
    CB_CHECK_EQ(old_z80::_strip(py::str("  .DB \"a;b\" ; c")).utf8(), "  .DB \"a;b\"");
    auto items = old_z80::_items(py::str("1, (2,3), \"x,y\""));
    CB_CHECK_EQ(items.size(), 3);
    CB_CHECK_EQ(items[1].utf8(), "(2,3)");
    const old_z80::AsmError e("boom");
    CB_CHECK_EQ(std::string(e.type()), "AsmError");
    CB_CHECK_THROWS_AS(py::ValueError, throw old_z80::AsmError("boom"));   // it is-a ValueError, as the old one was
});

CB_TEST("compat.shims.build_m6502_config", [] {
    auto [areas, segments] = old_6502::config(py::str("MEMORY { PRG: start = $8000, size = $4000, fill = yes; } SEGMENTS { CODE: load = PRG; }"));
    CB_CHECK_EQ(areas.size(), 1);
    CB_CHECK_EQ(std::get<0>(areas[0]).utf8(), "PRG");
    CB_CHECK_EQ(std::get<1>(areas[0]), 0x8000);
    CB_CHECK_EQ(std::get<2>(areas[0]), 0x4000);
    CB_CHECK(std::get<3>(areas[0]));
    CB_CHECK_EQ(segments.at(py::str("CODE")).utf8(), "PRG");
});

CB_TEST("compat.shims.build_definitions_types", [] {
    old_def::Export e(7, std::optional<py::str>(py::str("Foo")), uint64_t{0x1000}, py::str("code"));
    CB_CHECK_EQ(e.repr().utf8(), "<export Foo @7 code>");
    const auto n = old_def::detail::to_new(e);
    CB_CHECK_EQ(n.ordinal, 7u);
    CB_CHECK_EQ(n.name.value(), "Foo");
    CB_CHECK(n.kind == cb::build::definitions::ExportKind::Code);
    const auto back = old_def::detail::to_old(n);
    CB_CHECK_EQ(back.ordinal, 7);
    CB_CHECK_EQ(back.kind.utf8(), "code");
    old_def::Export seg(2, std::nullopt, std::make_tuple(int64_t{3}, int64_t{0x40}), py::str("data"), true, py::str(""), false);
    const auto sn = old_def::detail::to_new(seg);
    CB_CHECK(sn.kind == cb::build::definitions::ExportKind::Data);
    CB_CHECK_EQ(std::get<cb::Far16>(sn.address).off, 0x40);
    CB_CHECK(sn.resident);
    CB_CHECK(!sn.shared_data);
});

CB_TEST("compat.shims.build_watcom_small_helpers", [] {
    CB_CHECK_EQ(old_ow::IMAGE_BASE, 0x6E000000);
    CB_CHECK_EQ(old_ow::_BASE.utf8(), "option offset=0x6E000000\n");
    CB_CHECK_EQ(old_ow::_RTL.utf8(), "library clib3r.lib\n");
    CB_CHECK_EQ(old_ow::_first(py::list<py::str>{py::str(""), py::str("Z:/definitely/not/here"), py::str(".")}).utf8(), ".");
    CB_CHECK_EQ(old_ow::_first(py::list<py::str>{py::str("Z:/nope")}).utf8(), "");
    const py::str s = old_ow::_scratch(py::list<py::str>{py::str("a"), py::str("b")});
    CB_CHECK(s.utf8().find("bodyverify") != std::string::npos);
});

CB_TEST("compat.shims.build_dos_sessions_popen_and_constants", [] {
    CB_CHECK_EQ(old_dos::_WAIT_COM.size(), 12);
    CB_CHECK(old_dos::_WORKER_BAT.utf8().starts_with("echo hi > C:\\BOOTED\r\n:loop\r\n"));
    old_dos::Popen p(py::list<py::str>{py::str("cmd"), py::str("/c"), py::str("exit"), py::str("7")}, py::dict<py::str, py::str>());
    CB_CHECK_EQ(p.wait(30.0), 7);
    CB_CHECK_EQ(*p.poll(), 7);
    old_dos::Popen slow(py::list<py::str>{py::str("cmd"), py::str("/c"), py::str("ping"), py::str("-n"), py::str("30"), py::str("127.0.0.1")},
                        py::dict<py::str, py::str>());
    CB_CHECK(!slow.poll().has_value());
    CB_CHECK_THROWS_AS(py::TimeoutError, (void)slow.wait(0.05));
    slow.kill();
    (void)slow.wait(10.0);
    CB_CHECK_THROWS_AS(py::FileNotFoundError, old_dos::Popen(py::list<py::str>{py::str("Z:\\no\\such\\program.exe")}, py::dict<py::str, py::str>()));
});

CB_TEST("compat.shims.build_driver_shims_compile", [] {
    CB_CHECK(!codebreaker::build::msvc152::DOC.empty());
    CB_CHECK(!codebreaker::build::period16::DOC.empty());
    CB_CHECK_EQ(codebreaker::build::period16::_win(py::str("a/b")).utf8(), "a\\b");
});
