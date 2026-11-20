// cb-unity: alone (file-local names that other files in a batch also use)
// GameCube and Wii executables (formats/dol): the header worked out by hand from the DOL layout (18 section offsets, 18 addresses, 18 sizes,
// the bss area, the entry point), the sections as segments, which console the program is for, and what is refused.
#include "formats/dol.hpp"

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "source/gekko_test_images.hpp"

namespace d = cb::formats::dol;
namespace gt = cb::source::gekko_test;

CB_TEST("formats.dol.the_header_is_read_field_by_field", [] {
    const cb::Bytes file = gt::dol();
    const d::Dol dol = d::parse(file);
    CB_CHECK_EQ(dol.entry, gt::kText);
    CB_CHECK_EQ(dol.bss_address, 0x80005000u);
    CB_CHECK_EQ(dol.bss_size, 0x200u);
    CB_CHECK_EQ(dol.offsets[0], 0x100u);
    CB_CHECK_EQ(dol.addresses[0], gt::kText);
    CB_CHECK_EQ(dol.addresses[7], gt::kData);
    CB_CHECK_EQ(dol.sizes[7], 64u);
    CB_CHECK_EQ(dol.sizes[1], 0u);
    CB_CHECK(dol.data == file);
    const auto segments = dol.segments();
    CB_CHECK_EQ(segments.size(), std::size_t{2});
    CB_CHECK_EQ(segments[0].name, std::string("text0"));
    CB_CHECK(segments[0].code);
    CB_CHECK_EQ(segments[1].name, std::string("data0"));
    CB_CHECK(!segments[1].code);
    CB_CHECK_EQ(segments[1].offset, std::size_t{dol.offsets[7]});
    const auto* found = dol.segment_at(segments, gt::kHelper);
    CB_CHECK(found != nullptr && found->name == "text0");
    CB_CHECK(dol.segment_at(segments, 0x80010000) == nullptr);
});

CB_TEST("formats.dol.the_console_is_told_by_what_the_program_touches", [] {
    CB_CHECK(d::parse(gt::dol(false)).console() == d::Console::GameCube);
    CB_CHECK(d::parse(gt::dol(true)).console() == d::Console::Wii);   // `lis r7,0xCD80`: the Hollywood registers
    CB_CHECK_EQ(std::string(d::console_text(d::Console::Wii)), std::string("Wii"));
    // a section in MEM2 is a Wii program
    cb::Bytes file = gt::dol();
    gt::set32(file, 0x48 + 0x1C, 0x90000000);
    CB_CHECK(d::parse(file).console() == d::Console::Wii);
});

CB_TEST("formats.dol.probe_reads_the_header_only", [] {
    cb::fs::ScopedTempDir dir("cb_dol");
    cb::fs::write_file(dir.path() / "a.dol", gt::dol());
    CB_CHECK(d::probe(dir.path() / "a.dol"));
    cb::fs::write_file(dir.path() / "b.bin", cb::Bytes(0x400, 0x5A));
    CB_CHECK(!d::probe(dir.path() / "b.bin"));
    CB_CHECK(!d::probe(dir.path() / "missing.dol"));
    cb::fs::write_file(dir.path() / "short.dol", cb::Bytes(0x80, 0));
    CB_CHECK(!d::probe(dir.path() / "short.dol"));
});

CB_TEST("formats.dol.refuses_what_is_not_a_dol", [] {
    const auto refused = [](const cb::Bytes& file) {
        try {
            d::parse(file);
        } catch (const cb::FormatError&) {
            return true;
        }
        return false;
    };
    CB_CHECK(refused(cb::Bytes(0x80, 0)));                      // shorter than the header
    CB_CHECK(refused(cb::Bytes(0x100, 0)));                     // no section is used
    cb::Bytes outside = gt::dol();
    gt::set32(outside, 0x90, 0x10000);                          // a section larger than the file
    CB_CHECK(refused(outside));
    cb::Bytes elsewhere = gt::dol();
    gt::set32(elsewhere, 0x48, 0x00003100);                     // loaded outside the console memory
    CB_CHECK(refused(elsewhere));
    cb::Bytes entry = gt::dol();
    gt::set32(entry, 0xE0, gt::kData);                          // the entry point is in a data section
    CB_CHECK(refused(entry));
    cb::Bytes overlap = gt::dol();
    gt::set32(overlap, 0x1C, 0x100);                            // data0 starts where text0 does
    CB_CHECK(refused(overlap));
    cb::Bytes unaligned = gt::dol();
    gt::set32(unaligned, 0x48, gt::kText + 2);
    CB_CHECK(refused(unaligned));
});
