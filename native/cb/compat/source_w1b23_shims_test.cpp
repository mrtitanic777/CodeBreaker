// The shims at the old header paths of source/ne_asm/{fixups,segment} and source/vxd/{coalesce,disassembly} keep the old API (the old
// NEFile and LEFile, py::dict rows, py::bytes) over cb/source. Builds at /W3 (the old py:: layer is not warning-clean). What the shims
// need of the old corpus (the DDK's folders: the disassembly's default names) is only compiled, by shim_check_source_w1b23.cpp; the
// disassembly shim is run here with the names handed to it.
#include "codebreaker/formats/le.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/source/ne_asm/fixups.hpp"
#include "codebreaker/source/ne_asm/segment.hpp"
#include "codebreaker/source/vxd/coalesce.hpp"
#include "codebreaker/source/vxd/disassembly.hpp"

#include "cb/core/process.hpp"
#include "cb/core/testing.hpp"

namespace old_fixups = codebreaker::source::ne_asm::fixups;
namespace old_segment = codebreaker::source::ne_asm::segment;
namespace old_coalesce = codebreaker::source::vxd::coalesce;
namespace old_dis = codebreaker::source::vxd::disassembly;
namespace old_le = codebreaker::formats::le;
namespace old_ne = codebreaker::formats::ne;

namespace {

py::str testdata(const char* name) { return py::str(cb::testing::data_path(std::string("formats/testdata/") + name).string()); }

bool objdump_installed() {
    try {
        return cb::process::run({.argv = {"objdump", "--version"}}).ok();
    } catch (const cb::ToolError&) {
        return false;
    }
}

}  // namespace

CB_TEST("compat.source_w1b23.fixups_shim", [] {
    const old_ne::NEFile ne = old_fixups::open_ne(testdata("lzexpand.dll"));
    CB_CHECK_EQ(ne.module_name().utf8(), "LZEXPAND");
    const auto segments = ne.segments();
    const py::dict<int64_t, py::str> map = old_fixups::build_fixup_map(ne, segments[0], ne.module_refs());
    CB_CHECK_EQ(map.size(), 13);
    CB_CHECK_EQ(map.at(37).utf8(), "KERNEL.15");
    CB_CHECK_EQ(map.at(439).utf8(), "KERNEL.81");
    CB_CHECK_EQ(old_fixups::build_fixup_map(ne, segments[1], ne.module_refs()).at(25).utf8(), "LZEXPAND.16");
    CB_CHECK_EQ(old_fixups::build_fixup_map(ne, segments[2], ne.module_refs()).size(), 0);
    // a file that is neither MZ nor NE is the ValueError the Python raised
    bool value_error = false;
    try {
        (void)old_fixups::open_ne(py::str(cb::testing::data_path("source/ne_asm/testdata/ORDINALS.md").string()));
    } catch (const py::ValueError&) {
        value_error = true;
    }
    CB_CHECK(value_error);
    // a command line that is misused is a ValueError too (the shim maps the usage error)
    bool usage = false;
    try {
        (void)old_fixups::main(py::list<py::str>{testdata("lzexpand.dll"), py::str("--start"), py::str("zz")});
    } catch (const py::ValueError&) {
        usage = true;
    }
    CB_CHECK(usage);
});

CB_TEST("compat.source_w1b23.segment_shim_tables_and_entries", [] {
    CB_CHECK_EQ(old_segment::WM.get(0x0F, py::str("")).utf8(), "WM_PAINT");
    CB_CHECK_EQ(old_segment::WM.size(), 55);
    CB_CHECK_EQ(old_segment::DOS21.get(0x3D, py::str("")).utf8(), "OPEN");
    CB_CHECK_EQ(old_segment::DOS21.size(), 32);
    const py::dict<py::str, py::str> ordinals = old_segment::load_ordinals(py::str(cb::testing::data_path("source/ne_asm/testdata/ORDINALS.md").string()));
    CB_CHECK_EQ(ordinals.size(), 13);
    CB_CHECK_EQ(ordinals.get(py::str("KERNEL.15"), py::str("")).utf8(), "GlobalAlloc");
    CB_CHECK_EQ(old_segment::load_ordinals(py::str("no-such-file.md")).size(), 0);
    const old_ne::NEFile ne = old_fixups::open_ne(testdata("lzexpand.dll"));
    const auto entry = old_segment::find_seg_of_entry(ne, 3);
    CB_CHECK(entry.has_value());
    CB_CHECK_EQ(std::get<0>(*entry), 2);
    CB_CHECK_EQ(std::get<1>(*entry), 0);
    CB_CHECK(!old_segment::find_seg_of_entry(ne, 99).has_value());
});

CB_TEST("compat.source_w1b23.segment_shim_with_objdump", [] {
    if (!objdump_installed()) return;   // the shim's Seg runs the real objdump
    const old_ne::NEFile ne = old_fixups::open_ne(testdata("lzexpand.dll"));
    const py::dict<py::str, py::str> ordinals = old_segment::load_ordinals(py::str(cb::testing::data_path("source/ne_asm/testdata/ORDINALS.md").string()));
    const old_segment::Seg seg(ne, 7, ordinals);
    CB_CHECK_EQ(seg.si, 7);
    CB_CHECK_EQ(seg.code.size(), 0x2f0);
    CB_CHECK(seg.rows.size() > 50);
    CB_CHECK_EQ(seg.fixups.size(), 19);
    CB_CHECK(seg.valid.contains(0));
    CB_CHECK(seg.funcs.size() >= 1);
    CB_CHECK_EQ(seg.name_at(0xF4).utf8(), "sub_00f4");   // ordinal 21 is exported at 0xf4, without a name
    const auto [func, rows] = seg.func_rows(0);
    CB_CHECK_EQ(std::get<0>(rows[0]), 0);
    CB_CHECK_EQ(func.start, 0);
    // a function that calls KERNEL.81: the call kind and its text
    bool saw_kernel = false;
    for (const auto& [start, f] : seg.funcs.items()) {
        for (const auto& [kind, text] : seg.calls_in(start)) saw_kernel = saw_kernel || (kind == old_segment::CallKind::Sys && text.utf8().find("KERNEL.") == 0);
    }
    CB_CHECK(saw_kernel);
});

CB_TEST("compat.source_w1b23.coalesce_shim", [] {
    const py::bytes file = [&] {
        py::file f = py::open_rb(testdata("splitter.vxd"));
        py::bytes b = f.read();
        f.close();
        return b;
    }();
    const old_le::LEFile original = old_le::LEFile::from_buffer(file);
    CB_CHECK(old_coalesce::coalesce(file) == file);                      // retail's form is folded already
    CB_CHECK(old_coalesce::restore_entry_point(file, original) == file);
    CB_CHECK(old_coalesce::restore_metadata(file, original) == file);
    CB_CHECK(old_coalesce::restore_version_resource(file, original) == file);
    CB_CHECK(old_coalesce::finalize(file, original) == file);
    // what is not a driver is the error the old callers see
    bool threw = false;
    try {
        (void)old_coalesce::coalesce(py::bytes(std::string(100, 'x')));
    } catch (const py::Exception&) {
        threw = true;
    }
    CB_CHECK(threw);
    // finalize does not raise: a module the steps cannot read comes back as it was
    CB_CHECK(old_coalesce::finalize(py::bytes(std::string(100, 'x')), original) == py::bytes(std::string(100, 'x')));
});

CB_TEST("compat.source_w1b23.disassembly_shim_rows_and_labels", [] {
    const old_le::LEFile splitter = old_le::LEFile::from_file(testdata("splitter.vxd"));
    old_dis::VxDDisasm D(splitter);
    CB_CHECK_EQ(D.code.size(), 1);
    CB_CHECK_EQ(D.objs.size(), 1);
    CB_CHECK(D.ddb.has_value() && D.ddb->name.utf8() == "Splitter");
    CB_CHECK(D.insns.size() == 0);   // nothing is recovered before run()
    D.run(old_dis::cbv::DdkNames());
    CB_CHECK_EQ(D.insns.size(), 501);
    CB_CHECK_EQ(D.labels.size(), 64);
    CB_CHECK_EQ(D.jtables.size(), 0);
    CB_CHECK(D.svctab.has_value() && std::get<1>(*D.svctab) == 1416 && std::get<2>(*D.svctab) == 5);
    CB_CHECK_EQ(D.labels.at(old_dis::Place(1, 153)).utf8(), "Splitter_Control");
    const old_dis::Row& call = D.insns.at(old_dis::Place(1, 7));
    CB_CHECK_EQ(call.mnem.utf8(), "int");
    CB_CHECK_EQ(call.ops.utf8(), "20h");
    CB_CHECK_EQ(call.size, 6);
    CB_CHECK(call.vxdcall.has_value() && std::get<0>(*call.vxdcall) == 0x40 && std::get<1>(*call.vxdcall) == 0x0d && std::get<2>(*call.vxdcall).utf8() == "inline");
    const old_dis::Row& load = D.insns.at(old_dis::Place(1, 0x15));
    CB_CHECK(load.fx.has_value() && std::holds_alternative<old_le::InternalFixup>(*load.fx));
    CB_CHECK(D.fsite(1, 0x17).has_value());
    // a branch to a labelled place reads as the label; the operand of a direct branch is a number
    const old_dis::Row& jump = D.insns.at(old_dis::Place(1, 0x10));
    CB_CHECK_EQ(jump.mnem.utf8(), "je");
    CB_CHECK_EQ(*old_dis::_direct_target_row(jump), 0x15);
    const auto [text, resolved] = old_dis::_render_ops(D, 1, jump);
    CB_CHECK(resolved);
    CB_CHECK_EQ(text.utf8(), "loc_1_0015");
    CB_CHECK(!old_dis::_direct_target_row(D.insns.at(old_dis::Place(1, 0))).has_value());
    CB_CHECK_EQ(*old_dis::_direct_target(py::str("0x20")), 0x20);
    CB_CHECK(!old_dis::_direct_target(py::str("eax")).has_value());
    CB_CHECK_EQ(old_dis::VXD_DEVICES.get(0x40, py::str("")).utf8(), "IFSMgr");
    CB_CHECK(old_dis::_BRANCHES.contains(py::str("jecxz")) && old_dis::_TERMS.contains(py::str("iretd")));
    CB_CHECK_EQ(old_dis::DDB_SERVICE_TABLE, 0x30);
    const auto ranges = old_dis::_string_ranges(py::bytes(std::string("\x01Hello Wo\x00", 10)));
    CB_CHECK_EQ(ranges.size(), 1);
});
