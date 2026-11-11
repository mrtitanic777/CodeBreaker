// The shims at the old header paths of the console cartridge formats: the old py:: surface over cb/formats.
// (The functions() bodies call the decompiler's old decoders, which are not linked into cb_tests: they compile here,
// and run in the exe.)
#include "codebreaker/formats/gb.hpp"
#include "codebreaker/formats/genesis.hpp"
#include "codebreaker/formats/msx.hpp"
#include "codebreaker/formats/nes.hpp"
#include "codebreaker/formats/sms.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

py::bytes to_py_bytes(const cb::Bytes& b) { return cb::compat::to_py(cb::ByteView(b)); }

cb::Bytes nes_image(unsigned prg_units) {
    cb::Bytes out = {'N', 'E', 'S', 0x1A, static_cast<std::uint8_t>(prg_units), 0, 0x11, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (unsigned b = 0; b < prg_units; ++b) out.insert(out.end(), 0x4000, static_cast<std::uint8_t>(0x10 + b));
    out[16 + 0x4000 * prg_units - 6] = 0x34;   // nmi vector low byte
    out[16 + 0x4000 * prg_units - 5] = 0xC1;
    return out;
}

}  // namespace

namespace nes = codebreaker::formats::nes;

CB_TEST("compat.nes.parse_gives_the_old_record", [] {
    py::ref<nes::ROM> rom = nes::parse(to_py_bytes(nes_image(2)), py::str("demo.nes"));
    CB_CHECK_EQ(rom->path.utf8(), "demo.nes");
    CB_CHECK_EQ(rom->mapper, 1);
    CB_CHECK_EQ(rom->mirroring.utf8(), "vertical");
    CB_CHECK(!rom->four_screen);
    CB_CHECK_EQ(rom->prg.size(), 0x8000);
    CB_CHECK_EQ(rom->banks.vec().size(), 2u);
    CB_CHECK_EQ(rom->code_at, 0x8000);
    CB_CHECK_EQ(rom->vectors.at(py::str("nmi")), 0xC134);
    CB_CHECK_EQ(rom->vectors.items().size(), 3u);   // nmi, reset, irq
    CB_CHECK_EQ(rom->memory().size(), 0x10000);
    CB_CHECK_EQ(static_cast<int64_t>(rom->memory()[0xC000]), 0x11);   // 32 KB of PRG fill $8000-$FFFF
    CB_CHECK_EQ(*rom->mapped(0xC000), 0x4000);
    CB_CHECK(!rom->mapped(0x1000));
    CB_CHECK(!rom->mapped(-5));
});

CB_TEST("compat.nes.errors_are_the_old_exceptions", [] {
    CB_CHECK_THROWS_AS(py::ValueError, (void)nes::parse(py::bytes("junk"), py::str("x.nes")));
    CB_CHECK_THROWS_AS(py::OSError, (void)nes::read(py::str("no/such/file.nes")));
    CB_CHECK(!nes::is_nes(py::str("no/such/file.nes")));
});

CB_TEST("compat.sms.parse_gives_the_old_record", [] {
    cb::Bytes b(0x8000, 0x42);
    const char sig[] = "TMR SEGA";
    std::copy(sig, sig + 8, b.begin() + 0x7FF0);
    b[0x7FFF] = 0x4C;
    py::ref<codebreaker::formats::sms::ROM> rom = codebreaker::formats::sms::parse(to_py_bytes(b), py::str("g.sms"));
    CB_CHECK(rom->header.has_value());
    CB_CHECK_EQ(rom->header->at, 0x7FF0);
    CB_CHECK_EQ(rom->header->region.utf8(), "export");
    CB_CHECK_EQ(*rom->header->size, 0x8000);
    CB_CHECK_EQ(rom->system.utf8(), "Master System");
    CB_CHECK(!rom->mapper);
    CB_CHECK_EQ(rom->pages.vec().size(), 2u);
    CB_CHECK_EQ(rom->vectors.at(py::str("nmi")), 0x66);
    CB_CHECK_EQ(*rom->mapped(0x10), 0x10);
    CB_CHECK(!rom->mapped(0xC000));
    CB_CHECK_EQ(codebreaker::formats::sms::parse(to_py_bytes(cb::Bytes(0x4000, 0)), py::str("g.gg"))->system.utf8(), "Game Gear");
});

CB_TEST("compat.gb.parse_gives_the_old_record", [] {
    cb::Bytes b(0x8000, 0);
    std::copy(cb::formats::gb::kLogo.begin(), cb::formats::gb::kLogo.end(), b.begin() + 0x104);
    b[0x134] = 'A';
    b[0x143] = 0x80;
    b[0x147] = 0x03;
    py::ref<codebreaker::formats::gb::ROM> rom = codebreaker::formats::gb::parse(to_py_bytes(b), py::str("a.gb"));
    CB_CHECK(rom->logo_ok);
    CB_CHECK_EQ(rom->title.utf8(), "A");
    CB_CHECK_EQ(rom->cgb.utf8(), "also");
    CB_CHECK_EQ(rom->mbc.utf8(), "MBC1+RAM+BATTERY");
    CB_CHECK_EQ(rom->banks.vec().size(), 2u);
    CB_CHECK_EQ(rom->vectors.at(py::str("vblank")), 0x40);
    CB_CHECK_EQ(rom->vectors.items().size(), 6u);
    CB_CHECK_EQ(codebreaker::formats::gb::MBC.at(0x13).utf8(), "MBC3+RAM+BATTERY");
    CB_CHECK_EQ(codebreaker::formats::gb::MBC.items().size(), 24u);
    CB_CHECK_EQ(codebreaker::formats::gb::LOGO.size(), 48);
    CB_CHECK_THROWS_AS(py::ValueError, (void)codebreaker::formats::gb::parse(py::bytes("short"), py::str("s.gb")));
});

CB_TEST("compat.genesis.parse_gives_the_old_record", [] {
    cb::Bytes b(0x400, 0);
    const char sega[] = "SEGA MEGA DRIVE ";
    std::copy(sega, sega + 16, b.begin() + 0x100);
    b[0x1A7] = 0xFF;   // rom end low byte
    b[4 + 3] = 0x00;
    b[5] = 0x00;
    b[6] = 0x02;       // reset $0200
    py::ref<codebreaker::formats::genesis::ROM> rom = codebreaker::formats::genesis::parse(to_py_bytes(b), py::str("g.md"));
    CB_CHECK_EQ(rom->header.system.utf8(), "SEGA MEGA DRIVE");
    CB_CHECK_EQ(std::get<1>(rom->header.rom), 0xFF);
    CB_CHECK_EQ(rom->vectors.at(py::str("reset")), 0x200);
    CB_CHECK_EQ(rom->vectors.at(py::str("ssp")), 0);
    CB_CHECK_EQ(rom->memory().size(), 0x1000000);
    CB_CHECK_EQ(rom->checksum(), 0);
    CB_CHECK_EQ(codebreaker::formats::genesis::VECTOR_NAMES.at(30).utf8(), "vblank");
    CB_CHECK_EQ(codebreaker::formats::genesis::VECTOR_NAMES.at(47).utf8(), "trap15");
    CB_CHECK_EQ(codebreaker::formats::genesis::VECTOR_NAMES.items().size(), 34u);
    CB_CHECK_THROWS_AS(py::ValueError, (void)codebreaker::formats::genesis::parse(py::bytes("x"), py::str("x.md")));
});

CB_TEST("compat.msx.read_gives_the_old_record", [] {
    cb::fs::ScopedTempDir dir("msx-shim");
    cb::Bytes b(0x4000, 0);
    b[0] = 'A';
    b[1] = 'B';
    b[2] = 0x10;
    b[3] = 0x40;
    cb::fs::write_file(dir.path() / "g.rom", b);
    const py::str path = cb::compat::to_py(cb::fs::to_text(dir.path() / "g.rom"));
    CB_CHECK(codebreaker::formats::msx::is_msx(path));
    py::ref<codebreaker::formats::msx::Program> p = codebreaker::formats::msx::read(path);
    CB_CHECK_EQ(p->origin, 0x4000);
    CB_CHECK_EQ(p->header.init, 0x4010);
    CB_CHECK_EQ(p->entries.items().size(), 1u);
    CB_CHECK_EQ(p->entries.at(0x4010).utf8(), "init");
    CB_CHECK_THROWS_AS(py::OSError, (void)codebreaker::formats::msx::read(cb::compat::to_py(cb::fs::to_text(dir.path() / "missing.rom"))));
});
