// The shims at the old header paths (native/typed/cpp/codebreaker/exact/habits/*.hpp and exact/emu/*.hpp) over the
// modules of this folder: they must compile with the old layer and forward to the new code. Built at /W3 (the old
// layer is not warning-clean).
#include <string>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "codebreaker/exact/emu/isis.hpp"
#include "codebreaker/exact/emu/xenix.hpp"
#include "codebreaker/exact/habits/boolx.hpp"
#include "codebreaker/exact/habits/cmpp.hpp"
#include "codebreaker/exact/habits/oldcomp.hpp"
#include "codebreaker/exact/habits/symx.hpp"

namespace {

namespace habits = codebreaker::exact::habits;
using cb::compat::from_py;

cb::Bytes bytes_of(std::initializer_list<int> values) {
    cb::Bytes out;
    for (const int v : values) out.push_back(static_cast<std::uint8_t>(v));
    return out;
}

py::bytes py_bytes(std::initializer_list<int> values) { return cb::compat::to_py(cb::ByteView(bytes_of(values))); }

CB_TEST("exact.habits.shims.oldcomp", [] {
    // a call whose displacement differs next to a conditional jump the kit compiler made long
    const py::bytes want = py_bytes({0xe8, 0x10, 0x00, 0x72, 0x05, 0x90, 0x90, 0x90, 0x90, 0x90});
    const py::bytes got = py_bytes({0xe8, 0x12, 0x00, 0x73, 0x03, 0xe9, 0x05, 0x00, 0x90, 0x90, 0x90, 0x90, 0x90});
    const auto [ok, notes] = habits::oldcomp::equivalent(want, got, py::set<int64_t>{});
    CB_CHECK(ok);
    CB_CHECK_EQ(notes.size(), int64_t{1});
    CB_CHECK_EQ(from_py(notes[0]), "long jump at +0003");
    const auto [ok2, why] = habits::oldcomp::equivalent(py_bytes({0x01}), py_bytes({0x02}), py::set<int64_t>{});
    CB_CHECK(!ok2);
    CB_CHECK_EQ(from_py(why[0]), "no pattern at +0000 (original 01, compiled 02)");
    // a fixup stands for the byte
    const auto [ok3, why3] = habits::oldcomp::equivalent(py_bytes({0x01}), py_bytes({0x02}), py::set<int64_t>{0});
    CB_CHECK(!ok3);
    CB_CHECK_EQ(from_py(why3[0]), "differs only in jump displacements?");
});

CB_TEST("exact.habits.shims.symx", [] {
    using Ins = habits::symx::Ins;
    const py::list<Ins> want{Ins(py::str("mov ax, 0x1234"), py::bytes()), Ins(py::str("push ax"), py::bytes())};
    const py::list<Ins> got{Ins(py::str("mov ax, 0x9999"), py::bytes()), Ins(py::str("push ax"), py::bytes())};
    const auto [same1, why1] = habits::symx::same(want, got, py::dict<int64_t, py::set<int64_t>>{});
    CB_CHECK(!same1);
    CB_CHECK(from_py(why1).starts_with("other values pushed"));
    py::dict<int64_t, py::set<int64_t>> fixed;
    fixed.set(0, py::set<int64_t>{0x9999});
    const auto [same2, why2] = habits::symx::same(want, got, fixed);
    CB_CHECK(same2);
    CB_CHECK_EQ(from_py(why2), "");
});

CB_TEST("exact.habits.shims.boolx", [] {
    namespace bx = habits::boolx;
    const py::list<bx::Instruction> direct{bx::Instruction(0, py::str("test byte ptr [bp - 2], 1")), bx::Instruction(4, py::str("je 0x20"))};
    const py::list<bx::Instruction> masked{bx::Instruction(0, py::str("mov al, byte ptr [bp - 2]")), bx::Instruction(3, py::str("and al, 1")),
                                           bx::Instruction(5, py::str("je 0x20"))};
    const bx::SameResult r = bx::same(direct, 0, 6, masked, 0, 7);
    CB_CHECK(r.same);
    CB_CHECK(r.elses.has_value());
    if (r.elses) {
        CB_CHECK_EQ(std::get<0>(*r.elses), int64_t{0x20});
        CB_CHECK_EQ(std::get<1>(*r.elses), int64_t{0x20});
    }
    const bx::SameResult bad = bx::same(direct, 0, 6, py::list<bx::Instruction>{bx::Instruction(0, py::str("call 0x10"))}, 0, 3);
    CB_CHECK(!bad.same);
    CB_CHECK_EQ(from_py(bad.why), "instruction call 0x10");
});

CB_TEST("exact.emu.shims.isis", [] {
    namespace isis = codebreaker::exact::emu::isis;
    cb::fs::ScopedTempDir dir("shim_isis");
    // MVI C,'H' / CALL CO / HLT as an ISIS object file: a content record at 0100h (9 bytes + checksum), an entry record, the end
    cb::Bytes object = bytes_of({0x06, 0x0A, 0x00, 0x01, 0x00, 0x01, 0x0E, 0x48, 0xCD, 0x09, 0xF8, 0x76, 0x55});
    for (const int b : {0x04, 0x05, 0x00, 0x00, 0x00, 0x00, 0x01, 0x55, 0x0E, 0x01, 0x00, 0x55}) object.push_back(static_cast<std::uint8_t>(b));
    cb::fs::write_file(dir.path() / "PROG.OBJ", object);
    py::dict<int64_t, py::str> drives;
    drives.set(0, cb::compat::to_py(cb::fs::to_text(dir.path())));
    const isis::Result r = isis::run(cb::compat::to_py(cb::fs::to_text(dir.path() / "PROG.OBJ")), py::str(""), drives);
    CB_CHECK_EQ(from_py(r.console), "H");
    CB_CHECK_EQ(r.status, int64_t{0});
    CB_CHECK_EQ(r.steps, int64_t{3});
    CB_CHECK_THROWS_AS(py::FileNotFoundError, isis::run(py::str("no/such/file.obj"), py::str(""), drives));
});

CB_TEST("exact.emu.shims.xenix", [] {
    namespace xenix = codebreaker::exact::emu::xenix;
    cb::fs::ScopedTempDir dir("shim_xenix");
    // a.out: header (0109h, 10 bytes of text, 0 data), then mov bx,7 / mov dx,8800h / mov ax,1 / out dx,ax
    cb::Bytes image = bytes_of({0x09, 0x01, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0xBB, 0x07, 0x00, 0xBA, 0x00, 0x88, 0xB8, 0x01, 0x00, 0xEF});
    cb::fs::write_file(dir.path() / "root" / "bin" / "p", image);
    cb::fs::make_dirs(dir.path() / "work");
    const py::str root = cb::compat::to_py(cb::fs::to_text(dir.path() / "root"));
    const py::str work = cb::compat::to_py(cb::fs::to_text(dir.path() / "work"));
    const xenix::Result r = xenix::run(root, work, py::str("bin/p"), py::list<xenix::Arg>{}, py::bytes(), true);
    CB_CHECK(r.status.has_value());
    CB_CHECK_EQ(r.status.value_or(-1), int64_t{7});
    CB_CHECK_EQ(r.calls.size(), int64_t{1});
    // an old hook: it names registers by Unicorn's numbers, and changes BX just before the out
    py::dict<int64_t, xenix::Hook> hooks;
    hooks.set(9, [](xenix::Machine& m, int64_t cs, int64_t ds) {
        CB_CHECK_EQ(cs, int64_t{0x1000});
        CB_CHECK_EQ(ds, int64_t{0x3000});
        CB_CHECK_EQ(m.reg_read(UC_X86_REG_AX), int64_t{1});
        m.reg_write(UC_X86_REG_BX, 42);
        CB_CHECK_THROWS_AS(xenix::UcError, m.mem_read(0xFFFFFFF0, 64));   // (the engine's own error, as the old layer has it)
    });
    CB_CHECK_EQ(xenix::run(root, work, py::str("bin/p"), py::list<xenix::Arg>{}, py::bytes(), false, 1000, hooks).status.value_or(-1), int64_t{42});
});

}  // namespace

CB_TEST("exact.habits.shims.cmpp", [] {
    namespace cmpp = codebreaker::exact::habits::cmpp;
    // walk and loose_bytes over the old layer's types
    const py::bytes proc = py_bytes({0xE8, 0x10, 0x00, 0x07, 0x34, 0x12, 0xE8, 0x40, 0x00, 0xC3});
    cmpp::HDRLEN = 3;
    const py::list<cmpp::WalkItem> rows = cmpp::walk(proc, 0);
    CB_CHECK_EQ(rows.size(), int64_t{4});
    CB_CHECK_EQ(from_py(std::get<3>(rows[1])), "hdr");
    CB_CHECK_EQ(from_py(std::get<2>(rows[1])), "db 0x07 ; dw 0x1234");
    const py::set<int64_t> loose = cmpp::loose_bytes(proc);
    CB_CHECK(loose.contains(7) && loose.contains(8));   // the call outside the procedure
    cmpp::HDRLEN = 2;   // (pascal_join sets it from the toolchain)
    CB_CHECK_EQ(from_py(std::get<2>(cmpp::walk(proc, 0)[1])), "dw 0x3407");
    cmpp::HDRLEN = 3;
    // main over a fake pcc and a ledger on disk
    cb::fs::ScopedTempDir dir;
    cb::fs::make_dirs(dir.path() / "habits");
    cb::fs::make_dirs(dir.path() / "ledger");
    cb::fs::write_file(dir.path() / "ledger" / "M.EXE.json", cb::to_bytes(R"({"0x00000": {"size": 10}})"));
    cmpp::HERE = cb::compat::to_py(cb::path_text(dir.path() / "habits"));
    cmpp::Pcc pcc;
    pcc.image = [&](const py::str&) { return proc; };
    pcc.compile_pas = [&](const py::str&) {
        cmpp::CompileResult r;
        r.code = proc;
        r.procs.set(py::str("P00000"), std::make_tuple(int64_t{0}, int64_t{10}));
        return r;
    };
    cmpp::set_pcc(pcc);
    CB_CHECK_EQ(cmpp::main(py::list<py::str>{py::str("cmpp.py"), py::str("M.EXE"), py::str("0x0"), py::str("x.pas")}), int64_t{0});
    // a procedure the compiler did not make: exit 2
    cmpp::Pcc none;
    none.image = pcc.image;
    none.compile_pas = [](const py::str&) { return cmpp::CompileResult(); };
    cmpp::set_pcc(none);
    CB_CHECK_EQ(cmpp::main(py::list<py::str>{py::str("cmpp.py"), py::str("M.EXE"), py::str("0x0"), py::str("x.pas")}), int64_t{2});
    cmpp::set_pcc(cmpp::Pcc{});   // (no pcc module: the import fails)
    CB_CHECK_THROWS_AS(py::ImportError, cmpp::main(py::list<py::str>{py::str("cmpp.py"), py::str("M.EXE"), py::str("0x0"), py::str("x.pas")}));
});
