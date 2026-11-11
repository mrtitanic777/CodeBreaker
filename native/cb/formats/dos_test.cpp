// Tests of cb/formats/dos.hpp on real DOS programs from Windows 95 (testdata/): an EXEPACK-packed program (FIND.EXE, XCOPY.EXE),
// a PKLITE-packed one run in the emulator (EXTRACT.EXE), a device driver (ASPI2HLP.SYS), an EXE that carries a driver header
// (SETVER.EXE), a .COM file, and a DOS stub in front of a 32-bit PE (XCOPY32.EXE).
//
// DIVERGES from the Python reader (see state/work/typed-port/_bugs/LIST.md): EXEPACK data that runs out (an index before the
// start of the buffer) is a FormatError (the Python wrapped around to the end of the buffer); a relocation below the image in
// `unpacked_file` is a FormatError.
#include "cb/formats/dos.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
namespace dos = cb::formats::dos;

namespace {

Bytes testdata(const char* name) { return fs::read_file(testing::data_path(std::string("formats/testdata/") + name)); }

}  // namespace

CB_TEST("formats.dos.classification", [] {
    CB_CHECK(dos::kind_of(testdata("find.exe"), "find.exe") == dos::Kind::Mz);
    CB_CHECK(dos::kind_of(testdata("setver.exe"), "setver.exe") == dos::Kind::Mz);
    CB_CHECK(dos::kind_of(testdata("aspi2hlp.sys"), "aspi2hlp.sys") == dos::Kind::Driver);
    CB_CHECK(dos::kind_of(testdata("choice.com"), "CHOICE.COM") == dos::Kind::Com);  // told by the name: it has no header
    CB_CHECK(!dos::kind_of(testdata("choice.com"), "choice.dat").has_value());
    CB_CHECK(dos::kind_of(testdata("xcopy32.exe"), "xcopy32.exe") == dos::Kind::DosWithWindows);
    CB_CHECK(!dos::kind_of(testdata("lzexpand.dll"), "lzexpand.dll").has_value());  // a Windows module is not a DOS program
    CB_CHECK(!dos::kind_of(Bytes(100, 0x90), "x.bin").has_value());
    CB_CHECK_EQ(std::string(dos::kind_name(dos::Kind::DriverWithVxd)), "driver+vxd");
});

CB_TEST("formats.dos.mz_header", [] {
    const Bytes d = testdata("find.exe");
    const auto h = dos::mz_header(d);
    CB_CHECK(h.has_value());
    CB_CHECK_EQ(h->size, std::uint64_t{d.size()});
    CB_CHECK_EQ(h->relocation_count, 0);
    CB_CHECK(h->header_size >= 0x20 && h->header_size % 16 == 0);
    CB_CHECK(!dos::mz_header(Bytes{'M', 'Z', 0}).has_value());  // too short
    CB_CHECK(!dos::mz_header(testdata("lzexpand.dll")).has_value());  // a Windows module...
    CB_CHECK(dos::mz_header(testdata("lzexpand.dll"), true).has_value());  // ... unless only its stub is asked for
    CB_CHECK(dos::dos_and_windows(testdata("xcopy32.exe")).has_value());
    CB_CHECK(!dos::dos_and_windows(d).has_value());
    CB_CHECK(!dos::driver_and_vxd(d).has_value());
    CB_CHECK(!dos::kernel_body(d, *h).has_value());
});

CB_TEST("formats.dos.exepack_is_undone_statically", [] {
    const Bytes d = testdata("find.exe");
    const auto h = *dos::mz_header(d);
    CB_CHECK(dos::packer_of(d, h) == dos::Packer::ExePack);
    const dos::Program p = dos::read_program(d, "find.exe");
    CB_CHECK(p.packer == dos::Packer::ExePack);
    CB_CHECK(p.unpacked == dos::Unpacked::Static);
    CB_CHECK_EQ(p.image.size(), std::size_t{12016});
    CB_CHECK_EQ(p.cs, 0x1000u);
    CB_CHECK_EQ(p.ip, 0x18DAu);
    CB_CHECK_EQ(p.ss, 0x12EFu);
    CB_CHECK_EQ(p.sp, 0x4A8u);
    CB_CHECK_EQ(p.ds.value(), 0x1000u);
    CB_CHECK(p.relocations.empty());
    CB_CHECK_EQ(p.entries.size(), std::size_t{1});
    CB_CHECK_EQ(p.entries[0].address, 0x118DAu);
    CB_CHECK_EQ(p.entries[0].name, "entry");
    CB_CHECK(p.has_segment(0x118DA));
    CB_CHECK(p.inside(0x10000));
    CB_CHECK(!p.inside(0x10000 + 12016));
    // the unpacker's own record, read directly
    const auto u = dos::unpack_exepack(ByteView(d).subspan(h.header_size), h.cs, h.ip);
    CB_CHECK_EQ(u.module.size(), std::size_t{12016});
    CB_CHECK_EQ(u.ip, 0x18DAu);
    CB_CHECK(u.module == p.image);
    // a program with relocations
    const dos::Program x = dos::read_program(testdata("xcopy.exe"), "xcopy.exe");
    CB_CHECK_EQ(x.relocations.size(), std::size_t{2});
    CB_CHECK_EQ(x.image.size(), std::size_t{3936});
    CB_CHECK_EQ(x.ds.value(), 4265u);
    for (std::uint32_t r : x.relocations) CB_CHECK(r >= 0x10000 && r < 0x10000 + x.image.size());
});

CB_TEST("formats.dos.unpacked_file_reads_back", [] {
    const dos::Program p = dos::read_program(testdata("xcopy.exe"), "xcopy.exe");
    const Bytes plain = dos::unpacked_file(p, "xcopy.exe");
    const dos::Program q = dos::read_program(plain, "xcopy.exe");
    CB_CHECK(!q.packer.has_value());
    CB_CHECK(q.image == p.image);
    CB_CHECK(q.relocations == p.relocations);
    CB_CHECK_EQ(q.cs, p.cs);
    CB_CHECK_EQ(q.ip, p.ip);
    CB_CHECK_EQ(q.ss, p.ss);
    CB_CHECK_EQ(q.sp, p.sp);
    CB_CHECK_THROWS_AS(FormatError, dos::unpacked_file(dos::read_program(testdata("choice.com"), "choice.com"), "choice.com"));
});

CB_TEST("formats.dos.pklite_is_unpacked_by_running_it", [] {
    const dos::Program p = dos::read_program(testdata("extract.exe"), "extract.exe");
    CB_CHECK(p.packer == dos::Packer::PkLite);
    CB_CHECK(p.unpacked == dos::Unpacked::Emulated);
    CB_CHECK_EQ(p.image.size(), std::size_t{86150});
    CB_CHECK_EQ(p.relocations.size(), std::size_t{431});
    CB_CHECK_EQ(p.cs, 0x18D4u);
    CB_CHECK_EQ(p.ip, 0x1Eu);
    CB_CHECK_EQ(p.ss, 0x25C0u);
    CB_CHECK_EQ(p.sp, 0x1400u);
    CB_CHECK(p.notes.empty());
    // the two runs at two load segments agree on where the segment words are
    CB_CHECK(std::is_sorted(p.relocations.begin(), p.relocations.end()));
});

CB_TEST("formats.dos.emulated_unpack_reports_failure", [] {
    // a program that just exits never reaches an entry point
    const Bytes exits{0xCD, 0x20};
    CB_CHECK_THROWS_AS(EmulationError, dos::unpack_emulated(exits, {}, 0, 0, 0, 0x100, false, 1000000));
});

CB_TEST("formats.dos.drivers", [] {
    const dos::Program p = dos::read_program(testdata("aspi2hlp.sys"), "aspi2hlp.sys");
    CB_CHECK(p.kind == dos::Kind::Driver);
    CB_CHECK_EQ(p.devices.size(), std::size_t{1});
    CB_CHECK_EQ(p.devices[0].name, "ASPI2HL$");
    CB_CHECK(p.devices[0].character());
    CB_CHECK_EQ(p.devices[0].strategy, 0x28);
    CB_CHECK_EQ(p.devices[0].interrupt, 0x31);
    CB_CHECK(!p.devices[0].next.has_value());
    CB_CHECK_EQ(p.entries.size(), std::size_t{2});
    CB_CHECK_EQ(p.entries[0].name, "ASPI2HL_strategy");
    CB_CHECK_EQ(p.entries[0].address, 0x10028u);
    CB_CHECK_EQ(p.entries[1].name, "ASPI2HL_interrupt");
    CB_CHECK_EQ(p.entries[1].address, 0x10031u);
    // an EXE that is also a driver
    const dos::Program s = dos::read_program(testdata("setver.exe"), "setver.exe");
    CB_CHECK(s.kind == dos::Kind::Mz);
    CB_CHECK_EQ(s.devices.size(), std::size_t{1});
    CB_CHECK_EQ(s.devices[0].name, "SETVERXX");
    CB_CHECK_EQ(s.entries.size(), std::size_t{3});  // the program's entry, and the driver's two routines
    // devices() on hand-made headers: two devices chained in one segment
    Bytes img(0x40, 0);
    auto put16 = [&](std::size_t at, std::uint16_t v) { img[at] = static_cast<std::uint8_t>(v), img[at + 1] = static_cast<std::uint8_t>(v >> 8); };
    put16(0, 0x20), put16(2, 0xFFFF), put16(4, 0x8000), put16(6, 0x30), put16(8, 0x34);  // first device, next at 0x20
    std::copy_n("DEVONE  ", 8, img.begin() + 10);
    put16(0x20, 0xFFFF), put16(0x22, 0), put16(0x24, 0x0000), put16(0x26, 0x30), put16(0x28, 0x34);  // a block device, last
    img[0x2A] = 3;
    const auto devices = dos::devices(img);
    CB_CHECK_EQ(devices.size(), std::size_t{2});
    CB_CHECK_EQ(devices[0].name, "DEVONE");
    CB_CHECK_EQ(devices[0].next.value(), 0x20);
    CB_CHECK_EQ(devices[1].name, "block device (3 units)");
    CB_CHECK(!devices[1].character());
    // a chain that never ends is no driver
    put16(0x20, 0x00);
    CB_CHECK(dos::devices(img).empty());
    // a strategy routine outside the image is no driver either
    put16(0x20, 0xFFFF), put16(6, 0x5000);
    CB_CHECK(dos::devices(img).empty());
});

CB_TEST("formats.dos.com_file_and_dos_stub_of_a_windows_module", [] {
    const Bytes d = testdata("choice.com");
    const dos::Program c = dos::read_program(d, "CHOICE.COM");
    CB_CHECK(c.kind == dos::Kind::Com);
    CB_CHECK_EQ(c.image.size(), d.size() + 0x100);  // the program prefix in front
    CB_CHECK_EQ(c.image[0], 0xCD);
    CB_CHECK_EQ(c.image[1], 0x20);
    CB_CHECK_EQ(c.cs, 0x1000u);
    CB_CHECK_EQ(c.ip, 0x100u);
    CB_CHECK_EQ(c.sp, 0xFFFEu);
    CB_CHECK_EQ(c.entries[0].address, 0x10100u);
    CB_CHECK(c.entries[0].call == dos::Entry::Call::Near);
    const dos::Program w = dos::read_program(testdata("xcopy32.exe"), "xcopy32.exe");
    CB_CHECK(w.kind == dos::Kind::DosWithWindows);
    CB_CHECK(w.windows_at.has_value());
    CB_CHECK(w.overlay.empty());
    CB_CHECK_EQ(w.image.size(), std::size_t{21200});
});

CB_TEST("formats.dos.chain_of_programs", [] {
    const Bytes a = testdata("find.exe"), b = testdata("xcopy.exe");
    CB_CHECK(dos::chain(a).empty());  // one program is not a chain
    Bytes both = a;
    both.insert(both.end(), b.begin(), b.end());
    const auto parts = dos::chain(both);
    CB_CHECK_EQ(parts.size(), std::size_t{2});
    CB_CHECK_EQ(parts[0].first, std::uint64_t{0});
    CB_CHECK_EQ(parts[1].first, std::uint64_t{a.size()});
    CB_CHECK_EQ(parts[1].second, std::uint64_t{b.size()});
});

CB_TEST("formats.dos.errors", [] {
    CB_CHECK_THROWS_AS(FormatError, dos::read_program(Bytes(200, 0x90), "junk.bin"));
    CB_CHECK_THROWS_AS(FormatError, dos::read_driver_part(testdata("find.exe")));
    CB_CHECK_THROWS_AS(FormatError, dos::read_pklite_head(Bytes{1, 2, 3}, 4));
    // not EXEPACK: no RB before the entry point
    CB_CHECK_THROWS_AS(FormatError, dos::unpack_exepack(testdata("choice.com"), 0, 0x10));
    // damaged EXEPACK data runs out instead of reading outside the buffer
    const Bytes d = testdata("find.exe");
    const auto h = *dos::mz_header(d);
    Bytes module(d.begin() + h.header_size, d.end());
    for (std::size_t k = 0; k < 64 && k < module.size(); ++k) module[k] = 0xB0;
    try {
        (void)dos::unpack_exepack(module, h.cs, h.ip);
    } catch (const FormatError&) {
    }
    // every truncation of a packed program reads without crashing
    for (std::size_t n = 0; n < d.size(); n += 97) {
        try {
            (void)dos::read_program(ByteView(d).first(n), "find.exe", false);
        } catch (const Error&) {
        }
    }
});
