// cb-unity: alone (file-local names that other files in a batch also use)
#include "cb/platforms/n64.hpp"

#include "cb/core/testing.hpp"

namespace cb::platforms {

CB_TEST("platforms.n64.descriptor", [] {
    const Platform p = n64();
    CB_CHECK(p.id == Id::Machine);
    CB_CHECK_EQ(p.name, "n64");
    CB_CHECK_EQ(p.title, "Nintendo 64");
    CB_CHECK_EQ(p.family, "mips");
    CB_CHECK_EQ(p.cpu.language, "MIPS:BE:64:default");
    CB_CHECK_EQ(*p.cpu.bits, 64u);
    CB_CHECK(*p.cpu.endian == Endian::Big);
    CB_CHECK(std::get<OwnDecoder>(p.cpu.decoder) == OwnDecoder::Mips);
    CB_CHECK_EQ(p.cpu.stack, "SP");
    CB_CHECK(p.lane && *p.lane == "n64");
    CB_CHECK(p.emulator.runs_code);
    CB_CHECK_EQ(p.loader.container, "cartridge ROM (.z64, .v64, .n64)");
});

}  // namespace cb::platforms
