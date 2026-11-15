// cb-unity: alone (file-local names that other files in a batch also use)
// PowerPC ELF programs of the Nintendo consoles (formats/ppc_elf), a Wii U RPX with its compressed sections inflated into a file that is
// laid out again, and the three formats as one program (formats/ppc_program): the regions that tile the file, the console, the hardware map.
#include "cb/formats/ppc_elf.hpp"

#include <algorithm>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/gekko_map.hpp"
#include "cb/formats/ppc_program.hpp"
#include "cb/source/gekko_test_images.hpp"

namespace e = cb::formats::ppc_elf;
namespace pp = cb::formats::ppc_program;
namespace gm = cb::formats::gekko_map;
namespace gt = cb::source::gekko_test;

CB_TEST("formats.ppc_elf.the_headers_are_read", [] {
    const cb::Bytes file = gt::elf();
    const e::Elf elf = e::parse(file);
    CB_CHECK(elf.data == file);
    CB_CHECK(!elf.wiiu);
    CB_CHECK_EQ(elf.type, std::uint16_t{2});
    CB_CHECK_EQ(elf.machine, std::uint16_t{20});
    CB_CHECK_EQ(elf.entry, gt::kText);
    CB_CHECK_EQ(elf.program_headers.size(), std::size_t{2});
    CB_CHECK_EQ(elf.program_headers[1].vaddr, gt::kData);
    CB_CHECK_EQ(elf.sections.size(), std::size_t{4});
    CB_CHECK_EQ(elf.sections[1].name, std::string(".text"));
    CB_CHECK_EQ(elf.sections[2].name, std::string(".data"));
    CB_CHECK_EQ(elf.sections[3].name, std::string(".shstrtab"));
    CB_CHECK_EQ(elf.inflated_sections, std::size_t{0});
    const auto segments = elf.segments();
    CB_CHECK_EQ(segments.size(), std::size_t{2});
    CB_CHECK(segments[0].code);
    CB_CHECK_EQ(segments[0].address, gt::kText);
    CB_CHECK(!segments[1].code);
    CB_CHECK(elf.console() == cb::formats::dol::Console::GameCube);
    CB_CHECK(e::parse(gt::elf(false, true)).console() == cb::formats::dol::Console::Wii);
});

CB_TEST("formats.ppc_elf.an_rpx_is_inflated_and_laid_out_again", [] {
    const cb::Bytes file = gt::elf(true);
    const e::Elf elf = e::parse(file);
    CB_CHECK(elf.wiiu);
    CB_CHECK_EQ(elf.type, std::uint16_t{0xFE01});
    CB_CHECK_EQ(elf.os_abi, std::uint8_t{0xCA});
    CB_CHECK_EQ(elf.inflated_sections, std::size_t{1});
    const cb::Bytes code = gt::code();
    const e::SectionHeader& text = elf.sections[1];
    CB_CHECK_EQ(std::size_t{text.size}, code.size());
    CB_CHECK_EQ(text.flags & e::kFlagDeflated, 0u);        // the flag is cleared: the bytes are what they are
    CB_CHECK(std::equal(code.begin(), code.end(), elf.data.begin() + text.offset));
    // what came after the section moved by what it grew: the data section's bytes are where its header says
    const cb::Bytes data = gt::data();
    const e::SectionHeader& d = elf.sections[2];
    CB_CHECK(std::equal(data.begin(), data.end(), elf.data.begin() + d.offset));
    CB_CHECK_EQ(elf.sections[3].name, std::string(".shstrtab"));
    CB_CHECK(elf.data.size() != file.size());
    // reading it again changes nothing: the file is its own decompressed form
    CB_CHECK(e::parse(elf.data).data == elf.data);
});

CB_TEST("formats.ppc_elf.refuses_what_is_not_a_ppc_elf", [] {
    const auto refused = [](const cb::Bytes& file) {
        try {
            e::parse(file);
        } catch (const cb::FormatError&) {
            return true;
        }
        return false;
    };
    CB_CHECK(refused(cb::Bytes(10, 0)));
    cb::Bytes little = gt::elf();
    little[5] = 1;                                          // little-endian
    CB_CHECK(refused(little));
    cb::Bytes arm = gt::elf();
    arm[19] = 40;                                           // EM_ARM
    CB_CHECK(refused(arm));
    cb::Bytes damaged = gt::elf(true);
    damaged[0x80 + 8] ^= 0xFF;                              // inside the zlib stream
    CB_CHECK(refused(damaged));
    cb::fs::ScopedTempDir dir("cb_ppc_elf");
    cb::fs::write_file(dir.path() / "a.elf", gt::elf());
    CB_CHECK(e::probe(dir.path() / "a.elf"));
    cb::fs::write_file(dir.path() / "b.elf", little);
    CB_CHECK(!e::probe(dir.path() / "b.elf"));
});

CB_TEST("formats.ppc_program.the_regions_tile_the_file", [] {
    for (const cb::Bytes& bytes : {gt::dol(), gt::elf(), gt::elf(true)}) {
        const auto program = pp::parse(bytes);
        CB_CHECK(program.has_value());
        std::size_t at = 0;
        for (const pp::Region& r : program->regions) {
            CB_CHECK_EQ(r.offset, at);
            at += r.size;
        }
        CB_CHECK_EQ(at, program->file.size());
        CB_CHECK_EQ(program->entry, gt::kText);
        const pp::Region* code = program->code_at(gt::kHelper);
        CB_CHECK(code != nullptr);
        CB_CHECK_EQ(code->address, gt::kText);
        CB_CHECK_EQ(program->loaded().size(), std::size_t{2});
    }
    const auto dol = pp::parse(gt::dol());
    CB_CHECK(dol->kind == pp::Kind::Dol);
    CB_CHECK(dol->console == pp::Console::GameCube);
    CB_CHECK_EQ(dol->regions[0].name, std::string("dol header"));
    CB_CHECK(dol->regions[0].role == pp::Role::Header);
    CB_CHECK_EQ(dol->regions[1].name, std::string("text0"));
    CB_CHECK_EQ(dol->regions[1].offset, std::size_t{0x100});
    CB_CHECK(pp::parse(gt::dol(true))->console == pp::Console::Wii);
    CB_CHECK(pp::parse(gt::elf())->kind == pp::Kind::Elf);
    const auto rpx = pp::parse(gt::elf(true));
    CB_CHECK(rpx->kind == pp::Kind::Rpx);
    CB_CHECK(rpx->console == pp::Console::WiiU);
    CB_CHECK(!pp::parse(cb::Bytes(500, 7)).has_value());
    // an ELF header tables are regions of their own
    const auto elf = pp::parse(gt::elf());
    CB_CHECK_EQ(elf->regions[0].name, std::string("elf header"));
    CB_CHECK_EQ(elf->regions[1].name, std::string("program headers"));
    CB_CHECK_EQ(elf->regions.back().name, std::string("section headers"));
});

CB_TEST("formats.ppc_program.probe_tells_the_console", [] {
    cb::fs::ScopedTempDir dir("cb_ppc_program");
    cb::fs::write_file(dir.path() / "gc.dol", gt::dol(false));
    cb::fs::write_file(dir.path() / "wii.dol", gt::dol(true));
    cb::fs::write_file(dir.path() / "u.rpx", gt::elf(true));
    CB_CHECK(pp::probe(dir.path() / "gc.dol", pp::Console::GameCube));
    CB_CHECK(!pp::probe(dir.path() / "gc.dol", pp::Console::Wii));
    CB_CHECK(pp::probe(dir.path() / "wii.dol", pp::Console::Wii));
    CB_CHECK(pp::probe(dir.path() / "u.rpx", pp::Console::WiiU));
    CB_CHECK(!pp::probe(dir.path() / "u.rpx", pp::Console::GameCube));
    CB_CHECK_EQ(std::string(pp::lane_name(pp::Console::WiiU)), std::string("wiiu"));
    CB_CHECK_EQ(std::string(pp::cpu_text(pp::Console::GameCube)), std::string("Gekko"));
});

CB_TEST("formats.gekko_map.hardware_by_name", [] {
    using pp::Console;
    CB_CHECK_EQ(*gm::hardware_name(Console::GameCube, 0xCC006000), std::string("DI_STATUS"));
    CB_CHECK_EQ(*gm::hardware_name(Console::GameCube, 0x0C006000), std::string("DI_STATUS"));   // the cached physical view
    CB_CHECK_EQ(*gm::hardware_name(Console::GameCube, 0xCC002000), std::string("VI_VTR"));
    CB_CHECK_EQ(*gm::hardware_name(Console::GameCube, 0xCC00206C), std::string("VI_CLK"));
    CB_CHECK_EQ(*gm::hardware_name(Console::GameCube, 0xCC002072), std::string("VI_HBS"));
    CB_CHECK_EQ(*gm::hardware_name(Console::GameCube, 0xCC002076), std::string("VI_76"));        // a register the table does not name
    CB_CHECK_EQ(*gm::hardware_name(Console::GameCube, 0xCC008000), std::string("WGPIPE"));
    CB_CHECK_EQ(*gm::hardware_name(Console::GameCube, 0xCC006C00), std::string("AI_CONTROL"));
    CB_CHECK(!gm::hardware_name(Console::GameCube, 0x80003100).has_value());
    CB_CHECK(!gm::hardware_name(Console::GameCube, 0xCD000000).has_value());                    // the Wii mailbox is not the GameCube
    CB_CHECK_EQ(*gm::hardware_name(Console::Wii, 0xCD000004), std::string("IPC_PPCCTRL"));
    CB_CHECK_EQ(*gm::hardware_name(Console::Wii, 0xCD800064), std::string("HW_AHBPROT"));
    CB_CHECK_EQ(*gm::hardware_name(Console::Wii, 0xCC006000), std::string("DI_STATUS"));        // (the Wii keeps the GameCube blocks)
    CB_CHECK_EQ(*gm::hardware_name(Console::WiiU, 0x0D000010), std::string("LATTE_10"));
    CB_CHECK(!gm::hardware_name(Console::WiiU, 0xCC006000).has_value());
    CB_CHECK_EQ(gm::devices(Console::GameCube).size(), std::size_t{1});
    CB_CHECK(gm::devices(Console::Wii).size() > 1);
});
