// cb-unity: alone
#include "cb/formats/intv.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/intv_test_image.hpp"

namespace cb::formats::intv {

namespace ti = test_image;

CB_TEST("formats.intv.a_rom_file_is_read_segment_by_segment", [] {
    const Bytes file = ti::file();
    CB_CHECK_EQ(file.size(), 3u + 2 + 512 + 2 + 4);
    const Rom rom = parse(file);
    CB_CHECK(!rom.raw);
    CB_CHECK_EQ(rom.segments.size(), 1u);
    CB_CHECK_EQ(rom.segments[0].start, 0x5000u);
    CB_CHECK_EQ(rom.segments[0].end(), 0x5100u);
    CB_CHECK_EQ(rom.word_count(), 256u);
    CB_CHECK_EQ(rom.entry().value_or(0), 0x5020u);
    CB_CHECK_EQ(rom.title(), "TEST");
    CB_CHECK_EQ(rom.word_at(0x5020).value_or(0), 0x2B8u);
    CB_CHECK(!rom.word_at(0x5100).has_value());
    CB_CHECK((rom.tail == Bytes{0, 0x11, 0x22, 0x33}));
    CB_CHECK(rom.serialize(false) == file);
    CB_CHECK_EQ(rom.segments[0].crc, crc16(ByteView(file).subspan(3, 2 + 512)));
});

CB_TEST("formats.intv.a_raw_file_is_the_words_from_5000", [] {
    Rom rom = ti::rom();
    rom.raw = true;
    rom.tail.clear();
    const Bytes raw = rom.serialize();
    CB_CHECK_EQ(raw.size(), 512u);
    const Rom back = parse(raw, true);
    CB_CHECK(back.raw);
    CB_CHECK_EQ(back.entry().value_or(0), 0x5020u);
    CB_CHECK(back.serialize() == raw);
});

CB_TEST("formats.intv.refuses_what_is_not_one", [] {
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(2000, 0)));
    Bytes bad = ti::file();
    bad[2] = 0x00;   // the complement is wrong
    CB_CHECK_THROWS_AS(FormatError, (void)parse(bad));
    Bytes cut = ti::file();
    cut.resize(300);
    CB_CHECK_THROWS_AS(FormatError, (void)parse(cut));
    CB_CHECK_THROWS_AS(FormatError, (void)parse(Bytes(512, 0), true));   // no program pointer
});

CB_TEST("formats.intv.probe", [] {
    fs::ScopedTempDir dir("cb_intv_probe");
    fs::write_file(dir.path() / "a.rom", ti::file());
    Rom raw = ti::rom();
    raw.raw = true;
    raw.tail.clear();
    fs::write_file(dir.path() / "b.int", raw.serialize());
    fs::write_file(dir.path() / "c.rom", Bytes(2000, 0xFF));
    fs::write_file(dir.path() / "d.bin", ti::file());
    CB_CHECK(probe(dir.path() / "a.rom"));
    CB_CHECK(probe(dir.path() / "b.int"));
    CB_CHECK(!probe(dir.path() / "c.rom"));
    CB_CHECK(!probe(dir.path() / "d.bin"));
    CB_CHECK(!probe(dir.path() / "missing.rom"));
});

}  // namespace cb::formats::intv
