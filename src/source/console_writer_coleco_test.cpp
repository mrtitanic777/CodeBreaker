// Tests of the ColecoVision writer over a small cartridge written byte by byte: it must come back as source that builds the same bytes, with the console's
// names for what the code touches, and a file that is not a cartridge must be refused.

#include "source/console_writer.hpp"

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/coleco.hpp"

namespace cb::source::console_writer::coleco_test {
namespace {

// The platform's writer over a file (the description: config/platforms/coleco.json).
image_writer::Written reconstruct(const std::filesystem::path& file, const std::filesystem::path& folder) {
    return console_writer::reconstruct("coleco", file, folder);
}

// `data` kept in `folder` as `name` and written beside it.
image_writer::Written write(const Bytes& data, std::string_view name, const std::filesystem::path& folder) {
    fs::write_file(folder / std::string(name), data);
    return reconstruct(folder / std::string(name), folder);
}


// A 16 KB cartridge: the start at $8030, RETN as the NMI.
//   $8030 start: CALL $8040; RET
//   $8040 sub:   CALL $1FBE (BLK_WRT_VRAM); OUT ($BF),A; LD ($73C8),HL (RAND_SEED); RET
Bytes cartridge() {
    Bytes b(0x4000, 0);
    b[0] = 0x55;
    b[1] = 0xAA;
    b[0x0A] = 0x30;
    b[0x0B] = 0x80;
    b[0x21] = 0xED;
    b[0x22] = 0x45;
    const Bytes title = {'D', 'E', 'M', 'O'};
    std::copy(title.begin(), title.end(), b.begin() + 0x24);
    const Bytes start = {0xCD, 0x40, 0x80, 0xC9};
    std::copy(start.begin(), start.end(), b.begin() + 0x30);
    const Bytes sub = {0xCD, 0xBE, 0x1F, 0xD3, 0xBF, 0x22, 0xC8, 0x73, 0xC9};
    std::copy(sub.begin(), sub.end(), b.begin() + 0x40);
    return b;
}

CB_TEST("source.console_writer.coleco.a_cartridge_comes_back_as_source_that_builds_the_same_bytes", [] {
    fs::ScopedTempDir dir("cb_coleco");
    const formats::coleco::Program p = formats::coleco::parse(cartridge());
    const image_writer::Written r = write(cartridge(), "demo.col", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK_EQ(r.functions.at(0x8030), "start");
    CB_CHECK_EQ(r.functions.at(0x8021), "nmi");
    CB_CHECK_EQ(r.functions.at(0x8040), "sub_8040");
    CB_CHECK_EQ(r.image_bytes, 0x4000u);
    CB_CHECK(r.source == dir.path() / "demo.asm");
    CB_CHECK(r.built_image && *r.built_image == p.image);
});

CB_TEST("source.console_writer.coleco.the_source_names_the_ports_and_the_bios_by_its_listing", [] {
    fs::ScopedTempDir dir("cb_coleco");
    const image_writer::Written r = write(cartridge(), "demo.col", dir.path());
    const std::string text = fs::read_text(r.source);
    for (const char* line : {"; ColecoVision cartridge demo.col; 16 KB at $8000; $55AA: the game starts at once", "; title: DEMO", "VDP_CTRL", "BLK_WRT_VRAM",
                             "RAND_SEED", "        OUT (VDP_CTRL),A", "        CALL BLK_WRT_VRAM", "        CALL sub_8040", "start:"}) {
        CB_CHECK(text.find(line) != std::string::npos);
    }
    CB_CHECK(text.find("VDP_DATA") == std::string::npos);   // only what the code uses is defined
});

CB_TEST("source.console_writer.coleco.reconstruct_reads_the_file_and_refuses_what_is_not_a_cartridge", [] {
    fs::ScopedTempDir dir("cb_coleco");
    fs::write_file(dir.path() / "game.col", cartridge());
    const image_writer::Written r = reconstruct(dir.path() / "game.col", dir.path() / "out");
    CB_CHECK(r.same);
    fs::write_file(dir.path() / "junk.col", Bytes(0x4000, 0));
    CB_CHECK_THROWS_AS(FormatError, reconstruct(dir.path() / "junk.col", dir.path() / "out2"));
    CB_CHECK_THROWS_AS(IoError, reconstruct(dir.path() / "absent.col", dir.path() / "out3"));
});

}  // namespace
}  // namespace cb::source::console_writer::coleco_test
