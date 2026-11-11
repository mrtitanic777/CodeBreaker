// The shims at the old header paths of the executable-format readers (native/typed/cpp/codebreaker/formats/*.hpp) keep the old API
// over cb::formats. Builds at /W3 (the old py:: layer is not warning-clean).
#include "codebreaker/formats/detect.hpp"
#include "codebreaker/formats/dos.hpp"
#include "codebreaker/formats/le.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/formats/omf.hpp"
#include "codebreaker/formats/pe.hpp"

#include "cb/core/testing.hpp"

using namespace cb;

// The omf shim hands a.out objects to codebreaker/exact/aout (the old port, not part of cb_tests): stand-ins that say "not a.out".
namespace codebreaker::exact::aout {
bool is_aout(const py::bytes&) { return false; }
Obj read(const py::bytes&) { throw py::ValueError("a.out is not part of cb_tests"); }
}  // namespace codebreaker::exact::aout

namespace {
py::str testdata(const char* name) { return py::str(testing::data_path(std::string("formats/testdata/") + name).string()); }
py::bytes file_bytes(const char* name) {
    py::file f = py::open_rb(testdata(name));
    py::bytes b = f.read();
    f.close();
    return b;
}
}  // namespace

CB_TEST("compat.formats.pe_shim", [] {
    namespace pe = codebreaker::formats::pe;
    pe::PEFile f(testdata("lz32.dll"));
    CB_CHECK_EQ(f.machine, 0x14C);
    CB_CHECK_EQ(f.entry, 0x1000);
    CB_CHECK_EQ(f.image_base, uint64_t{0xBFEA0000});
    CB_CHECK_EQ(f.sections.size(), 6);
    CB_CHECK_EQ(f.sections[0].name.utf8(), ".text");
    CB_CHECK_EQ(f.module_name().utf8(), "LZ32.dll");
    CB_CHECK_EQ(f.exports().size(), 12);
    CB_CHECK_EQ(f.export_addrs().get(py::str("LZCopy"), 0), 0x11AA);
    CB_CHECK_EQ(f.imports().size(), 17);
    CB_CHECK_EQ(f.build_meta().linker_version.utf8(), f.optional_header().linker_version.utf8());
    CB_CHECK_EQ(f.build_meta().subsystem.utf8(), "GUI");
    CB_CHECK_EQ(f.export_table().symbols.size(), 12);
    CB_CHECK_EQ(f.import_table().size(), 1);
    CB_CHECK_EQ(f.import_table()[0].symbols[0].ordinal, 21);
    CB_CHECK_EQ(f.import_table()[0].symbols[0].hint, -1);
    CB_CHECK_EQ(f.resource_tree()[0].items[0].name.utf8().substr(0, 1), "#");
    CB_CHECK_EQ(f._u(py::str("H"), 0), 0x5A4D);
    CB_CHECK_EQ(f._text(0, 2).utf8(), "MZ");
    CB_CHECK_EQ(std::get<0>(f.directory(0)), 0x4000);
    CB_CHECK(!f.debug_codeview().has_value());
    CB_CHECK(!pe::is_dotnet(testdata("lz32.dll")));
    CB_CHECK_THROWS_AS(py::ValueError, pe::PEFile(testdata("lzexpand.dll")));  // an NE module: not a PE image
});

CB_TEST("compat.formats.ne_shim", [] {
    namespace ne = codebreaker::formats::ne;
    ne::NEFile f(testdata("lzexpand.dll"));
    CB_CHECK_EQ(f.ne_off, 128);
    CB_CHECK_EQ(f.h.n_segs, 12);
    CB_CHECK_EQ(f.module_name().utf8(), "LZEXPAND");
    CB_CHECK_EQ(f.segments().size(), 12);
    CB_CHECK_EQ(f.segments()[0].kind.utf8(), "CODE");
    CB_CHECK_EQ(f.relocations(f.segments()[0]).size(), 6);
    CB_CHECK_EQ(f.entries()[0].kind.utf8(), "MOVEABLE");
    CB_CHECK_EQ(f.entries().size(), 30);
    CB_CHECK_EQ(f.module_refs().size(), 2);
    CB_CHECK_EQ(f.nonresident_names().size(), 11);
    ne::NEFile c(testdata("charmap.exe"));
    CB_CHECK_EQ(c.resources().size(), 8);
    CB_CHECK_EQ(c.resources()[0].type.utf8(), "GROUP_ICON");
    CB_CHECK_EQ(ne::_tables_end(c), 438);
    CB_CHECK(ne::standalone(c).size() > 14000);
    CB_CHECK_EQ(ne::embedded(file_bytes("lzexpand.dll")).size(), 1);
    CB_CHECK_THROWS_AS(py::ValueError, ne::NEFile(testdata("lz32.dll")));
});

CB_TEST("compat.formats.detect_shim", [] {
    namespace detect = codebreaker::formats::detect;
    CB_CHECK_EQ(detect::lane(testdata("lz32.dll"))->utf8(), "pe");
    CB_CHECK_EQ(detect::kind(testdata("choice.com"))->utf8(), "dos");
    CB_CHECK(!detect::lane(testdata("choice.com")).has_value());
    CB_CHECK_EQ(detect::format_of(testdata("splitter.vxd"))->utf8(), "LE");
    CB_CHECK_EQ(detect::signature(testdata("lzexpand.dll"))->raw(), "NE");
    CB_CHECK(detect::holds_drivers(testdata("splitter.vxd")));
    CB_CHECK(detect::flat(std::optional<py::str>(py::str("pe"))));
    CB_CHECK_EQ(detect::identify(testdata("lz32.dll")).size(), 1);
    CB_CHECK_EQ(detect::identify(testdata("lz32.dll"))[0].format.utf8(), "PE executable");
    CB_CHECK_EQ(detect::dos_header(file_bytes("find.exe"))->relocations, 0);
});

CB_TEST("compat.formats.le_shim", [] {
    namespace le = codebreaker::formats::le;
    le::LEFile f = le::LEFile::from_file(testdata("splitter.vxd"));
    CB_CHECK_EQ(f.lfa, 128);
    CB_CHECK_EQ(f.objects.size(), 1);
    CB_CHECK_EQ(f.resident_name().utf8(), "Splitter");
    CB_CHECK_EQ(f.header().signature.utf8(), "LE");
    CB_CHECK_EQ(le::exports(f).size(), 1);
    CB_CHECK_EQ(f.fixups().size(), 30);
    CB_CHECK_EQ(le::fixup_records(f).size(), 13);
    CB_CHECK_EQ(std::get<1>(le::fixup_records(f)[0]).kind.utf8(), "internal");
    CB_CHECK(le::descriptor(f).has_value());
    CB_CHECK_EQ(le::descriptor(f)->name.utf8(), "Splitter");
    CB_CHECK_EQ(le::service_table(f).size(), 5);
    CB_CHECK_EQ(le::header_fields(f).get(py::str("page_size"), 0), 4096);
    CB_CHECK_EQ(le::open_vxds(testdata("bios.vxd")).size(), 1);
    // take apart and put back: the same file
    le::Image image = le::read_image(f);
    CB_CHECK(le::write_image(image) == file_bytes("splitter.vxd"));
    CB_CHECK_THROWS_AS(py::ValueError, le::LEFile::from_buffer(file_bytes("lz32.dll")));
});

CB_TEST("compat.formats.dos_shim", [] {
    namespace dos = codebreaker::formats::dos;
    dos::Program p = dos::read(testdata("find.exe"));
    CB_CHECK_EQ(p.kind.utf8(), "mz");
    CB_CHECK_EQ(p.packer->utf8(), "EXEPACK");
    CB_CHECK_EQ(p.unpacked->utf8(), "static");
    CB_CHECK_EQ(py::len(p.image), 12016);
    CB_CHECK_EQ(p.cs, 0x1000);
    CB_CHECK_EQ(p.entries.size(), 1);
    CB_CHECK(p.inside(0x10000));
    CB_CHECK_EQ(dos::kind(testdata("choice.com"))->utf8(), "com");
    CB_CHECK_EQ(dos::packer(testdata("find.exe"))->utf8(), "EXEPACK");
    dos::Program d = dos::read(testdata("aspi2hlp.sys"));
    CB_CHECK_EQ(d.devices.size(), 1);
    CB_CHECK_EQ(d.devices[0].name.utf8(), "ASPI2HL$");
    CB_CHECK(d.devices[0].character);
    CB_CHECK_EQ(dos::unpacked_file(dos::read(testdata("xcopy.exe"))).raw().substr(0, 2), "MZ");
    CB_CHECK_THROWS_AS(py::ValueError, dos::read(testdata("lzexpand.dll")));
});

CB_TEST("compat.formats.omf_shim", [] {
    namespace omf = codebreaker::formats::omf;
    py::bytes d = file_bytes("fastadd.obj");
    omf::Obj o = omf::read(d);
    CB_CHECK_EQ(o.segs.size(), 2);
    CB_CHECK_EQ(std::get<0>(o.pubs[0]).utf8(), "__FASTLDADD");
    CB_CHECK_EQ(std::get<2>(o.pubs[0]), 6);
    CB_CHECK_EQ(o.seg.at(py::str("_TEXT")).size(), 386);
    CB_CHECK_EQ(omf::library(d).size(), 1);
    CB_CHECK_EQ(omf::publics(d).size(), 1);
    CB_CHECK_EQ(omf::records(d).size() > 3, true);
    auto fns = omf::functions(o, py::str("_TEXT"));
    CB_CHECK_EQ(std::get<0>(fns[0]).utf8(), "(module start)");
    // a writer and its reader
    py::dict<py::str, int64_t> pubs;
    pubs.set(py::str("_f"), 0);
    py::bytes obj = omf::raw_object(py::str("t"), py::bytes("\x90\xC3", 2), pubs);
    CB_CHECK_EQ(omf::read(obj).pubs.size(), 1);
    CB_CHECK_THROWS_AS(py::ValueError, omf::name(py::str(std::string(300, 'x'))));
});
