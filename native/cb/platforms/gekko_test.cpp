// cb-unity: alone (file-local names that other files in a batch also use)
#include "cb/platforms/gamecube.hpp"
#include "cb/platforms/wii.hpp"
#include "cb/platforms/wiiu.hpp"

#include "cb/core/testing.hpp"

namespace cb::platforms {

CB_TEST("platforms.gekko.descriptors", [] {
    const Platform gc = gamecube(), w = wii(), u = wiiu();
    CB_CHECK(gc.id == Id::Machine && w.id == Id::Machine && u.id == Id::Machine);
    CB_CHECK_EQ(gc.name, "gamecube");
    CB_CHECK_EQ(w.name, "wii");
    CB_CHECK_EQ(u.name, "wiiu");
    CB_CHECK_EQ(gc.title, "Nintendo GameCube");
    CB_CHECK_EQ(w.title, "Nintendo Wii");
    CB_CHECK_EQ(u.title, "Nintendo Wii U");
    for (const Platform* p : {&gc, &w, &u}) {
        CB_CHECK_EQ(p->family, "powerpc");
        CB_CHECK_EQ(p->cpu.language, "PowerPC:BE:32:default");
        CB_CHECK_EQ(*p->cpu.bits, 32u);
        CB_CHECK(*p->cpu.endian == Endian::Big);
        CB_CHECK(std::get<OwnDecoder>(p->cpu.decoder) == OwnDecoder::PowerPc);
        CB_CHECK_EQ(p->cpu.stack, "r1");
        CB_CHECK(p->lane && *p->lane == p->name);
        CB_CHECK(p->emulator.runs_code);
        CB_CHECK(p->source && *p->source == SourceLanguage::Asm);
        CB_CHECK_EQ(p->convention.results.size(), std::size_t{22});
    }
    CB_CHECK_EQ(gc.loader.container, "DOL executable (.dol), ELF (.elf)");
    CB_CHECK_EQ(u.loader.container, "RPX executable (.rpx), RPL library (.rpl): an ELF with compressed sections");
});

}  // namespace cb::platforms
