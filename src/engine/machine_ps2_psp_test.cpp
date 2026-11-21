// cb-unity: alone (file-local names that other files in a batch also use)
// The PlayStation 2 and PSP lanes over programs the test makes itself (assembled with CodeBreaker's assembler, wrapped in an ELF the way ps2sdk's and pspsdk's linkers
// do, an EBOOT.PBP around the PSP's): the formats read them, the writers give source that builds back into the same file byte for byte (a PSP module's imports named
// by NID), both lanes are registered and take their files, and the functions run in the R5900 and the Allegrex -- the same in the original and in a copy, different
// when an instruction is changed.
#include <algorithm>
#include <cstring>
#include <format>
#include <string>

#include "build/mips_asm.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "engine/machine_lanes.hpp"
#include "engine/machine_ps2.hpp"
#include "engine/machine_psp.hpp"
#include "formats/ps2.hpp"
#include "formats/psp.hpp"
#include "source/isa_mips_le.hpp"
#include "source/ps2_writer.hpp"
#include "source/psp_writer.hpp"

using namespace cb;
namespace stdfs = std::filesystem;
namespace vl = cb::verify::lanes;

namespace {

void put32(Bytes& b, std::size_t at, std::uint32_t v) {
    if (b.size() < at + 4) b.resize(at + 4, 0);
    for (unsigned i = 0; i < 4; ++i) b[at + i] = static_cast<std::uint8_t>(v >> (8 * i));
}
void put16(Bytes& b, std::size_t at, std::uint16_t v) {
    if (b.size() < at + 2) b.resize(at + 2, 0);
    b[at] = static_cast<std::uint8_t>(v);
    b[at + 1] = static_cast<std::uint8_t>(v >> 8);
}

struct Sect {
    std::string name;
    std::uint32_t flags;      // SHF_ALLOC (2), SHF_EXECINSTR (4)
    std::uint32_t address;
    Bytes data;
};

// An executable ELF with one loadable segment holding the sections at their addresses (as the linker lays them out), a symbol table and section headers.
Bytes make_elf(std::uint32_t machine_flags, std::uint32_t entry, std::uint32_t base, const std::vector<Sect>& sections, const std::vector<std::pair<std::string, std::uint32_t>>& functions) {
    constexpr std::uint32_t kStart = 0x80;
    std::uint32_t end = 0;
    for (const Sect& s : sections) end = std::max<std::uint32_t>(end, s.address - base + static_cast<std::uint32_t>(s.data.size()));
    Bytes file(kStart + end, 0);
    for (const Sect& s : sections) std::copy(s.data.begin(), s.data.end(), file.begin() + kStart + (s.address - base));
    // the symbol and string tables
    Bytes strtab(1, 0), symtab(16, 0);
    for (const auto& [name, value] : functions) {
        const std::uint32_t at = static_cast<std::uint32_t>(strtab.size());
        strtab.insert(strtab.end(), name.begin(), name.end());
        strtab.push_back(0);
        Bytes sym(16, 0);
        put32(sym, 0, at);
        put32(sym, 4, value);
        sym[12] = 0x12;   // global function
        put16(sym, 14, 1);
        symtab.insert(symtab.end(), sym.begin(), sym.end());
    }
    Bytes shstr(1, 0);
    const auto name_of = [&](const std::string& n) {
        const std::uint32_t at = static_cast<std::uint32_t>(shstr.size());
        shstr.insert(shstr.end(), n.begin(), n.end());
        shstr.push_back(0);
        return at;
    };
    struct Row {
        std::uint32_t name, type, flags, address, offset, size, link, info, align, entsize;
    };
    std::vector<Row> rows(1, Row{});
    for (const Sect& s : sections) rows.push_back({name_of(s.name), 1, s.flags, s.address, kStart + (s.address - base), static_cast<std::uint32_t>(s.data.size()), 0, 0, 4, 0});
    const std::uint32_t symtab_name = name_of(".symtab"), strtab_name = name_of(".strtab"), shstr_name = name_of(".shstrtab");
    const std::uint32_t symtab_at = static_cast<std::uint32_t>(file.size());
    file.insert(file.end(), symtab.begin(), symtab.end());
    const std::uint32_t strtab_at = static_cast<std::uint32_t>(file.size());
    file.insert(file.end(), strtab.begin(), strtab.end());
    const std::uint32_t shstr_at = static_cast<std::uint32_t>(file.size());
    rows.push_back({symtab_name, 2, 0, 0, symtab_at, static_cast<std::uint32_t>(symtab.size()), static_cast<std::uint32_t>(rows.size() + 1), 1, 4, 16});
    rows.push_back({strtab_name, 3, 0, 0, strtab_at, static_cast<std::uint32_t>(strtab.size()), 0, 0, 1, 0});
    rows.push_back({shstr_name, 3, 0, 0, shstr_at, 0, 0, 0, 1, 0});
    shstr.insert(shstr.end(), 0, 0);
    rows.back().size = static_cast<std::uint32_t>(shstr.size());
    file.insert(file.end(), shstr.begin(), shstr.end());
    while (file.size() % 4 != 0) file.push_back(0);
    const std::uint32_t shoff = static_cast<std::uint32_t>(file.size());
    file.resize(file.size() + rows.size() * 40, 0);
    for (std::size_t i = 0; i < rows.size(); ++i) {
        const Row& r = rows[i];
        const std::uint32_t v[10] = {r.name, r.type, r.flags, r.address, r.offset, r.size, r.link, r.info, r.align, r.entsize};
        for (std::size_t k = 0; k < 10; ++k) put32(file, shoff + i * 40 + k * 4, v[k]);
    }
    // the header and the one program header
    file[0] = 0x7F; file[1] = 'E'; file[2] = 'L'; file[3] = 'F'; file[4] = 1; file[5] = 1; file[6] = 1;
    put16(file, 16, 2);
    put16(file, 18, 8);
    put32(file, 20, 1);
    put32(file, 24, entry);
    put32(file, 28, 52);
    put32(file, 32, shoff);
    put32(file, 36, machine_flags);
    put16(file, 40, 52);
    put16(file, 42, 32);
    put16(file, 44, 1);
    put16(file, 46, 40);
    put16(file, 48, static_cast<std::uint16_t>(rows.size()));
    put16(file, 50, static_cast<std::uint16_t>(rows.size() - 1));
    const std::uint32_t v[8] = {1, kStart, base, base, end, end + 0x100, 7, 0x80};
    for (std::size_t k = 0; k < 8; ++k) put32(file, 52 + k * 4, v[k]);
    return file;
}

Bytes assemble(const build::Encoder& encoder, const std::string& source) { return build::mips_asm::Assembler(encoder).build(source); }

Bytes ps2_elf() {
    const std::string text = R"(
        .set noreorder
        .org 0x00100000
_start: addiu $sp, $sp, -32
        sd    $ra, 16($sp)
        lui   $a0, 0x1200
        ori   $a0, $a0, 0x1000
        lw    $v0, 0($a0)
        jal   helper
        nop
        lui   $a1, 0x0010
        ori   $a1, $a1, 0x1000
        lq    $t0, 0($a1)
        por   $t1, $t0, $t0
        pcpyld $t2, $t1, $t0
        sq    $t1, 16($a1)
        mult  $v0, $a0, $a1
        add.s $f0, $f1, $f2
        ld    $ra, 16($sp)
        jr    $ra
        addiu $sp, $sp, 32
helper: beql  $a0, $zero, done
        nop
        addu  $v0, $v0, $a1
done:   jr    $ra
        nop
)";
    const std::string data = R"(
        .org 0x00101000
        .asciz "Hello from the Emotion Engine"
        .align 2
        .word 0x00100000, 0x0010003C
)";
    const Bytes code = assemble(source::isa::r5900_encoder(), text);
    return make_elf(0x20924001, 0x00100000, 0x00100000, {{".text", 6, 0x00100000, code}, {".data", 3, 0x00101000, assemble(source::isa::r5900_encoder(), data)}},
                    {{"_start", 0x00100000}, {"helper", 0x00100000 + 0x48}});
}

constexpr std::uint32_t kPspBase = 0x08804000;

Bytes psp_elf(bool mutate = false) {
    const std::string text = R"(
        .set noreorder
        .org 0x08804000
_start: addiu $sp, $sp, -16
        sw    $ra, 0($sp)
        jal   0x08804080
        move  $a0, $a1
        ext   $v0, $a0, 4, 8
        seb   $v1, $v0
        max   $v0, $v0, $v1
        clz   $v1, $v0
        lui   $a2, 0x1D40
        lw    $a3, 256($a2)
        lw    $ra, 0($sp)
        jr    $ra
        addiu $sp, $sp, 16
vfpu:   vadd.q C000, C010, C020
        jr    $ra
        nop
)";
    Bytes code = assemble(source::isa::allegrex_encoder(), text);
    code.resize(0x80, 0);
    const Bytes stub = assemble(source::isa::allegrex_encoder(), std::string(".set noreorder\n.org 0x08804080\n jr $ra\n nop\n"));
    Bytes lib_stub(20, 0);   // name, flags, counts, NID table, stubs
    put32(lib_stub, 0, kPspBase + 0x300);
    put32(lib_stub, 4, 0x00090000);
    put32(lib_stub, 8, 0x00010005);
    put32(lib_stub, 12, kPspBase + 0x180);
    put32(lib_stub, 16, kPspBase + 0x80);
    Bytes nid(4, 0);
    put32(nid, 0, formats::psp::nid_of("sceDisplaySetMode"));
    Bytes info(52, 0);
    put16(info, 0, 0x1000);
    std::memcpy(info.data() + 4, "test", 4);
    put32(info, 36, kPspBase + 0x200);
    put32(info, 40, kPspBase + 0x200);
    put32(info, 44, kPspBase + 0x100);
    put32(info, 48, kPspBase + 0x114);
    Bytes name(12, 0);
    std::memcpy(name.data(), "sceDisplay", 10);
    Bytes elf = make_elf(0x10A23001, kPspBase, kPspBase,
                         {{".text", 6, kPspBase, code}, {".sceStub.text", 6, kPspBase + 0x80, stub}, {".lib.stub", 2, kPspBase + 0x100, lib_stub}, {".rodata.sceNid", 2, kPspBase + 0x180, nid},
                          {".rodata.sceModuleInfo", 2, kPspBase + 0x200, info}, {".rodata.sceResident", 2, kPspBase + 0x300, name}},
                         {{"_start", kPspBase}, {"vfpu", kPspBase + 0x34}});
    if (mutate) {   // `max $v0, $v0, $v1` becomes `min`
        for (std::size_t i = 0; i + 4 <= elf.size(); i += 4)
            if (elf[i] == 0x2C && elf[i + 1] == 0x10 && elf[i + 2] == 0x43 && elf[i + 3] == 0x00) elf[i] = 0x2D;
    }
    return elf;
}

Bytes pbp_of(const Bytes& module) {
    Bytes sfo(0x40, 0);
    std::memcpy(sfo.data(), "\0PSF", 4);
    put32(sfo, 4, 0x101);
    put32(sfo, 8, 0x24);   // key table
    put32(sfo, 12, 0x30);  // data table
    put32(sfo, 16, 1);
    put16(sfo, 20, 0);
    put16(sfo, 22, 0x0204);
    put32(sfo, 24, 8);
    put32(sfo, 28, 8);
    put32(sfo, 32, 0);
    std::memcpy(sfo.data() + 0x24, "TITLE", 6);
    std::memcpy(sfo.data() + 0x30, "TESTPRG", 8);
    Bytes pbp(40, 0);
    std::memcpy(pbp.data(), "\0PBP", 4);
    put32(pbp, 4, 0x10000);
    const std::uint32_t sfo_at = 40, data_at = 40 + static_cast<std::uint32_t>(sfo.size()), end = data_at + static_cast<std::uint32_t>(module.size());
    const std::uint32_t offsets[8] = {sfo_at, data_at, data_at, data_at, data_at, data_at, data_at, end};
    for (std::size_t i = 0; i < 8; ++i) put32(pbp, 8 + 4 * i, offsets[i]);
    pbp.insert(pbp.end(), sfo.begin(), sfo.end());
    pbp.insert(pbp.end(), module.begin(), module.end());
    return pbp;
}

bool contains(const std::string& text, std::string_view part) { return text.find(part) != std::string::npos; }

}  // namespace

CB_TEST("source.ps2_writer.the_source_builds_back_into_the_elf", [] {
    fs::ScopedTempDir dir("cb_ps2_writer");
    fs::write_file(dir.path() / "game.elf", ps2_elf());
    CB_CHECK(formats::ps2::probe(dir.path() / "game.elf"));
    CB_CHECK(!formats::psp::probe(dir.path() / "game.elf"));
    const source::image_writer::Written r = source::ps2_writer::reconstruct(dir.path() / "game.elf", dir.path() / "out");
    CB_CHECK(r.notes.empty());
    CB_CHECK(r.linked);
    CB_CHECK(r.same);
    CB_CHECK_EQ(r.functions.size(), std::size_t{2});
    const std::string text = fs::read_text(r.source);
    CB_CHECK(contains(text, "_start:"));
    CB_CHECK(contains(text, "lq $t0, 0($a1)"));
    CB_CHECK(contains(text, "pcpyld $t2, $t1, $t0"));
    CB_CHECK(contains(text, ".equ GS_CSR, 0x12001000"));   // the register the code builds the address of is named
    CB_CHECK(contains(text, "%lo(GS_CSR)"));
    CB_CHECK(contains(text, "; section 1 .text"));
    CB_CHECK(contains(text, "beql a0, zero, L_"));
});

CB_TEST("source.psp_writer.the_source_builds_back_into_the_module_and_the_package", [] {
    fs::ScopedTempDir dir("cb_psp_writer");
    const Bytes elf = psp_elf();
    fs::write_file(dir.path() / "game.elf", elf);
    fs::write_file(dir.path() / "EBOOT.PBP", pbp_of(elf));
    CB_CHECK(formats::psp::probe(dir.path() / "game.elf"));
    CB_CHECK(formats::psp::probe(dir.path() / "EBOOT.PBP"));
    CB_CHECK(!formats::ps2::probe(dir.path() / "game.elf"));
    const std::optional<formats::psp::Program> program = formats::psp::read(dir.path() / "EBOOT.PBP");
    CB_CHECK(program && program->title == "TESTPRG" && program->kind == formats::psp::Kind::Pbp);
    const std::vector<formats::psp::Import> imports = formats::psp::imports(program->elf);
    CB_CHECK_EQ(imports.size(), std::size_t{1});
    CB_CHECK_EQ(imports[0].name, std::string("sceDisplaySetMode"));
    CB_CHECK_EQ(imports[0].library, std::string("sceDisplay"));
    for (const char* file : {"game.elf", "EBOOT.PBP"}) {
        const source::image_writer::Written r = source::psp_writer::reconstruct(dir.path() / file, dir.path() / (std::string("out_") + file));
        CB_CHECK(r.notes.empty());
        CB_CHECK(r.linked);
        CB_CHECK(r.same);
        const std::string text = fs::read_text(r.source);
        CB_CHECK(contains(text, "sceDisplaySetMode:"));   // the import's stub, named by its NID
        CB_CHECK(contains(text, "ext v0, a0, 0x4, 0x8"));
        CB_CHECK(contains(text, "seb v1, v0"));
        CB_CHECK(contains(text, ".byte 0x80,0x81,0x02,0x60  ; vadd.q C000, C010, C020"));
        CB_CHECK(contains(text, "jal sceDisplaySetMode"));
    }
    // an encrypted module is recognised and not handled
    Bytes encrypted = pbp_of(elf);
    std::memcpy(encrypted.data() + 40 + 0x40, "~PSP", 4);
    fs::write_file(dir.path() / "enc.PBP", encrypted);
    CB_CHECK(!formats::psp::probe(dir.path() / "enc.PBP"));
    CB_CHECK(formats::psp::encrypted(dir.path() / "enc.PBP"));
});

CB_TEST("engine.machine_ps2_psp.are_registered_once_and_take_their_files", [] {
    engine::register_ps2_lane();
    engine::register_ps2_lane();
    engine::register_psp_lane();
    CB_CHECK(engine::machine_lanes().find("ps2") != nullptr);
    CB_CHECK(engine::machine_lanes().find("psp") != nullptr);
    fs::ScopedTempDir dir("cb_ps2_psp_lane");
    fs::write_file(dir.path() / "a.elf", ps2_elf());
    fs::write_file(dir.path() / "b.elf", psp_elf());
    fs::write_file(dir.path() / "other.bin", Bytes(0x2000, 0x41));
    CB_CHECK_EQ(engine::machine_lanes().name_of_file(dir.path() / "a.elf"), "ps2");
    CB_CHECK_EQ(engine::machine_lanes().name_of_file(dir.path() / "b.elf"), "psp");
    CB_CHECK_EQ(engine::machine_lanes().name_of_file(dir.path() / "other.bin"), "");
});

CB_TEST("engine.machine_ps2.functions_run_in_the_r5900_and_a_changed_one_differs", [] {
    fs::ScopedTempDir dir("cb_ps2_lane");
    const Bytes elf = ps2_elf();
    fs::write_file(dir.path() / "a.elf", elf);
    Bytes changed = elf;
    for (std::size_t i = 0; i + 4 <= changed.size(); i += 4)
        if (changed[i] == 0x21 && changed[i + 1] == 0x10 && changed[i + 2] == 0x45 && changed[i + 3] == 0x00) changed[i] = 0x23;   // addu $v0,$v0,$a1 becomes subu
    CB_CHECK(changed != elf);
    fs::write_file(dir.path() / "b.elf", changed);
    const vl::ImageLaneSpec spec = engine::ps2_lane_spec();
    CB_CHECK_EQ(spec.kind, "ps2");
    CB_CHECK_EQ(spec.what.size(), std::size_t{12});
    const vl::Effects entry = spec.effects(dir.path() / "a.elf", 0x00100000, spec.states[0]);
    CB_CHECK_EQ(entry.size(), spec.what.size());
    CB_CHECK_EQ(std::get<std::string>(entry[0]), std::string("return"));
    const auto& io = std::get<std::vector<std::string>>(entry[10]);
    CB_CHECK(!io.empty());
    CB_CHECK_EQ(io[0], std::string("r4 GS_CSR 12001000 a"));   // the vertical-blank and finish flags read set
    CB_CHECK(spec.effects(dir.path() / "a.elf", 0x00100000, spec.states[1]) == spec.effects(dir.path() / "a.elf", 0x00100000, spec.states[1]));
    const std::uint32_t helper = 0x00100048;
    CB_CHECK(spec.effects(dir.path() / "a.elf", helper, spec.states[1]) != spec.effects(dir.path() / "b.elf", helper, spec.states[1]));
    CB_CHECK(spec.effects(dir.path() / "a.elf", helper, spec.states[0]) == spec.effects(dir.path() / "b.elf", helper, spec.states[0]));   // (a0 = 0: the branch skips it)
    const vl::Written w = spec.writer(dir.path() / "a.elf", dir.path() / "out");
    CB_CHECK(w.same);
});

CB_TEST("engine.machine_psp.functions_run_in_the_allegrex_and_imports_are_logged", [] {
    fs::ScopedTempDir dir("cb_psp_lane");
    fs::write_file(dir.path() / "a.elf", psp_elf());
    fs::write_file(dir.path() / "b.elf", psp_elf(true));
    const vl::ImageLaneSpec spec = engine::psp_lane_spec();
    const vl::Effects entry = spec.effects(dir.path() / "a.elf", kPspBase, spec.states[0]);
    CB_CHECK_EQ(std::get<std::string>(entry[0]), std::string("return"));
    const auto& calls = std::get<std::vector<std::string>>(entry[10]);
    CB_CHECK_EQ(calls.size(), std::size_t{1});
    CB_CHECK_EQ(calls[0], std::string("sceDisplaySetMode(0,0,0,0)"));
    const auto& io = std::get<std::vector<std::string>>(entry[9]);
    CB_CHECK_EQ(io.size(), std::size_t{1});
    CB_CHECK(contains(io[0], "GE_0100"));   // the graphics engine's block, by name
    CB_CHECK(spec.effects(dir.path() / "a.elf", kPspBase, spec.states[2]) != spec.effects(dir.path() / "b.elf", kPspBase, spec.states[2]));
    // the VFPU is decoded but not run: both programs stop the same way
    const vl::Effects vfpu = spec.effects(dir.path() / "a.elf", kPspBase + 0x34, spec.states[0]);
    CB_CHECK_EQ(std::get<std::string>(vfpu[0]), std::string("stopped: not run: vadd.q"));
    const vl::Written w = spec.writer(dir.path() / "a.elf", dir.path() / "out");
    CB_CHECK(w.same);
});
