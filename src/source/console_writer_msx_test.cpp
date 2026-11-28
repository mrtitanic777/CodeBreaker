// Tests of the MSX writer over a small cartridge written byte by byte: it must come back as source that builds the same bytes, with the
// MSX's names for what the code touches, and a file that is not a cartridge must be refused.

#include "source/console_writer.hpp"

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/msx.hpp"

namespace cb::source::console_writer::msx_test {
namespace {

// The platform's writer over a file (the description: config/platforms/msx.json).
image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder) {
    return console_writer::reconstruct("msx", file, folder);
}

// `data` kept in `folder` as `name` and written beside it.
image_writer::Written write(const Bytes& data, std::string_view name, const std::filesystem::path& folder) {
    fs::write_file(folder / std::string(name), data);
    return reconstruct(folder / std::string(name), folder);
}


// A 16 KB cartridge at $4000: "AB" and INIT = $4010.
//   $4010 init: CALL $4020; OUT ($98),A; RET
//   $4020 sub:  CALL $004A (RDVRM); RET
Bytes cartridge() {
    Bytes b(0x4000, 0);
    b[0] = 'A';
    b[1] = 'B';
    b[2] = 0x10;
    b[3] = 0x40;
    const Bytes init = {0xCD, 0x20, 0x40, 0xD3, 0x98, 0xC9};
    std::copy(init.begin(), init.end(), b.begin() + 0x10);
    const Bytes sub = {0xCD, 0x4A, 0x00, 0xC9};
    std::copy(sub.begin(), sub.end(), b.begin() + 0x20);
    return b;
}

CB_TEST("source.console_writer.msx.a_cartridge_comes_back_as_source_that_builds_the_same_bytes", [] {
    fs::ScopedTempDir dir("cb_msx");
    const formats::msx::Program p = formats::msx::parse(cartridge());
    const image_writer::Written r = write(cartridge(), "demo.rom", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK((r.functions == image_writer::Functions{{0x4010, "init"}, {0x4020, "sub_4020"}}));
    CB_CHECK_EQ(r.code_bytes, 10u);
    CB_CHECK_EQ(r.image_bytes, 0x4000u);
    CB_CHECK(r.source == dir.path() / "demo.asm");
    CB_CHECK(r.built_image && *r.built_image == p.image);
});

CB_TEST("source.console_writer.msx.the_source_names_the_ports_and_the_bios_by_the_handbook", [] {
    fs::ScopedTempDir dir("cb_msx");
    const image_writer::Written r = write(cartridge(), "demo.rom", dir.path());
    const std::string text = fs::read_text(r.source);
    for (const char* line : {"; MSX cartridge demo.rom; 16 KB at $4000; INIT $4010 STATEMENT $0000 DEVICE $0000 TEXT $0000", "VDP_DATA", "RDVRM",
                             "        OUT (VDP_DATA),A", "        CALL RDVRM", "        CALL sub_4020", "init:"}) {
        CB_CHECK(text.find(line) != std::string::npos);
    }
    CB_CHECK(text.find("PSG_ADDR") == std::string::npos);   // only what the code uses is defined
});

CB_TEST("source.console_writer.msx.reconstruct_reads_the_file_and_refuses_what_is_not_a_cartridge", [] {
    fs::ScopedTempDir dir("cb_msx");
    const Bytes image = cartridge();
    fs::write_file(dir.path() / "game.rom", image);
    const image_writer::Written r = reconstruct(dir.path() / "game.rom", dir.path() / "out");
    CB_CHECK(r.same);
    CB_CHECK(r.source == dir.path() / "out" / "game.asm");

    fs::write_file(dir.path() / "junk.rom", Bytes(0x4000, 0));
    CB_CHECK_THROWS_AS(FormatError, reconstruct(dir.path() / "junk.rom", dir.path() / "out2"));
    CB_CHECK_THROWS_AS(IoError, reconstruct(dir.path() / "absent.rom", dir.path() / "out3"));
});

}  // namespace
}  // namespace cb::source::console_writer::msx_test
