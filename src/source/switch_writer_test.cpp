// cb-unity: alone
// The Switch source writer: a NRO comes back byte for byte, with its labels, the address pairs, the relocated words and the relocation table, the header, MOD0 and the assets
// written readably; a NSO and an ELF come back as their loaded image; an edit to the source is a different program.
#include "source/switch_writer.hpp"

#include "build/a64_asm.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/nro_test_image.hpp"

namespace cb::source::switch_writer {
namespace {

bool has(const std::string& text, std::string_view needle) { return text.find(needle) != std::string::npos; }

}  // namespace

CB_TEST("source.switch_writer.a_nro_comes_back_byte_for_byte", [] {
    fs::ScopedTempDir dir("cb_switch_writer");
    fs::write_file(dir.path() / "app.nro", formats::nro::test_image::nro_bytes());
    const image_writer::Written r = reconstruct(dir.path() / "app.nro", dir.path() / "out");
    CB_CHECK(r.linked);
    CB_CHECK(r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK(r.built.has_value() && fs::read_file(*r.built) == formats::nro::test_image::nro_bytes());
    CB_CHECK_EQ(r.image_bytes, 0x3000u);
    CB_CHECK_EQ(r.functions.size(), 3u);
    CB_CHECK_EQ(r.functions.at(0), "entry");
    CB_CHECK_EQ(r.functions.at(0x80), "_start");
    CB_CHECK_EQ(r.functions.at(0xB0), "sub_0000B0");
    const std::string text = fs::read_text(r.source);
    CB_CHECK(has(text, "b _start"));
    CB_CHECK(has(text, "adrp x8, D_002048"));                  // the data the pair addresses
    CB_CHECK(has(text, "add x8, x8, :lo12:D_002048"));
    CB_CHECK(has(text, "adrp x1, D_001030"));                  // the string
    CB_CHECK(has(text, "bl sub_0000B0"));
    CB_CHECK(has(text, "svc #0x21  // svcSendSyncRequest"));
    CB_CHECK(has(text, ".quad sub_0000B0"));                  // the relocated word: a pointer to the function
    CB_CHECK(has(text, ".quad B_003010"));                    // one into the bss
    CB_CHECK(has(text, ".equ B_003010, 0x3010"));
    CB_CHECK(has(text, ".quad D_002040, 0x403, sub_0000B0"));   // the relocation table entries themselves
    CB_CHECK(has(text, ".ascii \"HOMEBREW\""));
    CB_CHECK(has(text, "// text size"));
    CB_CHECK(has(text, ".ascii \"MOD0\""));
    CB_CHECK(has(text, ".asciz \"hello, switch\""));
});

CB_TEST("source.switch_writer.the_asset_section_is_kept_as_a_file", [] {
    fs::ScopedTempDir dir("cb_switch_writer_assets");
    const Bytes original = formats::nro::test_image::nro_with_assets();
    fs::write_file(dir.path() / "app.nro", original);
    const image_writer::Written r = reconstruct(dir.path() / "app.nro", dir.path() / "out");
    CB_CHECK(r.linked && r.same);
    CB_CHECK(fs::read_file(*r.built) == original);
    CB_CHECK(has(fs::read_text(r.source), ".incbin \"app.assets.bin\""));
    const Bytes assets = fs::read_file(dir.path() / "out" / "app.assets.bin");
    CB_CHECK_EQ(assets.size(), 0x38u + 24);
    CB_CHECK_EQ(assets[0], static_cast<std::uint8_t>('A'));
});

CB_TEST("source.switch_writer.a_nso_and_an_elf_come_back_as_their_loaded_image", [] {
    fs::ScopedTempDir dir("cb_switch_writer_nso");
    fs::write_file(dir.path() / "m.nso", formats::nro::test_image::nso_bytes());
    const image_writer::Written n = reconstruct(dir.path() / "m.nso", dir.path() / "nso");
    CB_CHECK(n.linked && n.same);
    CB_CHECK(fs::read_file(*n.built) == formats::nro::test_image::nro_bytes());   // (the inflated program)
    CB_CHECK(has(fs::read_text(n.source), "compressed segments inflated"));
    CB_CHECK(!n.notes.empty());
    fs::write_file(dir.path() / "p.elf", formats::nro::test_image::elf_bytes());
    const image_writer::Written e = reconstruct(dir.path() / "p.elf", dir.path() / "elf");
    CB_CHECK(e.linked && e.same);
    const std::string text = fs::read_text(e.source);
    CB_CHECK(has(text, "mov x0, #7"));
    CB_CHECK(has(text, "ret"));
    CB_CHECK(has(text, ".byte 0x78, 0x56, 0x34, 0x12"));
});

CB_TEST("source.switch_writer.a_word_no_row_knows_is_data_and_the_file_is_the_same", [] {
    fs::ScopedTempDir dir("cb_switch_writer_word");
    Bytes nro = formats::nro::test_image::nro_bytes();
    // a word in the middle of the code that no row of the table knows: the flow ends there and what follows is data, but the file is the same
    nro[0xA8] = 0xFF;
    nro[0xA9] = 0xFF;
    nro[0xAA] = 0xFF;
    nro[0xAB] = 0xFF;
    fs::write_file(dir.path() / "odd.nro", nro);
    const image_writer::Written r = reconstruct(dir.path() / "odd.nro", dir.path() / "out");
    CB_CHECK(r.linked && r.same);
    CB_CHECK(has(fs::read_text(r.source), "0xFF, 0xFF, 0xFF, 0xFF"));
});

CB_TEST("source.switch_writer.an_edited_source_builds_a_different_program", [] {
    fs::ScopedTempDir dir("cb_switch_writer_edit");
    fs::write_file(dir.path() / "app.nro", formats::nro::test_image::nro_bytes());
    const image_writer::Written r = reconstruct(dir.path() / "app.nro", dir.path() / "out");
    std::string text = fs::read_text(r.source);
    const std::size_t at = text.find("add x9, x9, #1");
    CB_CHECK(at != std::string::npos);
    text.replace(at, 14, "add x9, x9, #2");
    const Bytes edited = build::a64_asm::build(text, dir.path() / "out").data;
    CB_CHECK_EQ(edited.size(), formats::nro::test_image::nro_bytes().size());
    CB_CHECK(edited != formats::nro::test_image::nro_bytes());
});

}  // namespace cb::source::switch_writer
