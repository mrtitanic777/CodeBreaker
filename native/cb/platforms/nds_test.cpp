#include "cb/platforms/nds.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/detect.hpp"
#include "cb/source/nds_test_image.hpp"

namespace cb::platforms {

CB_TEST("platforms.nds.descriptor", [] {
    const Platform p = nds();
    CB_CHECK(p.id == Id::Nds);
    CB_CHECK_EQ(p.name, "nds");
    CB_CHECK_EQ(p.title, "Nintendo DS");
    CB_CHECK_EQ(p.family, "arm");
    CB_CHECK_EQ(p.cpu.language, "ARM:LE:32:v5t");
    CB_CHECK_EQ(*p.cpu.bits, 32u);
    CB_CHECK(*p.cpu.endian == Endian::Little);
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::Arm);
    CB_CHECK_EQ(p.cpu.stack, "SP");
    CB_CHECK(p.lane && *p.lane == "nds");
    CB_CHECK(p.emulator.runs_code);
});

CB_TEST("formats.detect.identify_a_nintendo_ds_cartridge", [] {
    fs::ScopedTempDir dir("cb_nds_detect");
    const Bytes rom = source::nds_test::test_rom();
    fs::write_file(dir.path() / "game.nds", rom);
    const std::vector<formats::detect::Detected> found = formats::detect::identify(dir.path() / "game.nds");
    CB_CHECK(!found.empty());
    CB_CHECK_EQ(found.front().format, "Nintendo DS cartridge");
    CB_CHECK_EQ(found.front().family, "nds");
    CB_CHECK_EQ(found.front().command, "reach");
    CB_CHECK(found.front().detail.starts_with("\"NDSTEST\" (ATSE)"));
});

}  // namespace cb::platforms
