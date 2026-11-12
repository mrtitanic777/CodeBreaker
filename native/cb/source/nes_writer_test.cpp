// Tests of the NES writer over a small program of its own: one 16 KB PRG bank with a reset, an NMI and an IRQ handler, a subroutine, a
// loop, an indexed read of a table, and the PPU's registers named. The program is machine code, written out byte by byte here, and
// the writer is held to what it is for: the source it writes, built again, is the cartridge it came from.

#include "cb/source/nes_writer.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace cb::source::nes_writer {
namespace {

// reset at $C000: SEI, LDA #0, STA $2000 (PPUCTRL); loop: JSR sub; JMP loop
// sub at $C00C: LDA $2002 (PPUSTATUS); BPL sub; LDA $C020,X; RTS
// nmi at $C015: RTI    irq at $C016: RTI    the table at $C020: "SCORE", 1, 2, 3    the vectors at $FFFA
Bytes prg_bank() {
    Bytes prg(0x4000, 0xFF);
    const Bytes code = {0x78, 0xA9, 0x00, 0x8D, 0x00, 0x20, 0x20, 0x0C, 0xC0, 0x4C, 0x06, 0xC0, 0xAD, 0x02, 0x20, 0x10, 0xFB,
                        0xBD, 0x20, 0xC0, 0x60, 0x40, 0x40};
    std::copy(code.begin(), code.end(), prg.begin());
    const Bytes table = {'S', 'C', 'O', 'R', 'E', 1, 2, 3};
    std::copy(table.begin(), table.end(), prg.begin() + 0x20);
    const Bytes vectors = {0x15, 0xC0, 0x00, 0xC0, 0x16, 0xC0};   // nmi, reset, irq
    std::copy(vectors.begin(), vectors.end(), prg.begin() + 0x3FFA);
    return prg;
}

Bytes ines(const Bytes& prg, const Bytes& chr, std::uint8_t mapper = 0, bool trainer = false) {
    Bytes out = {'N', 'E', 'S', 0x1A, static_cast<std::uint8_t>(prg.size() / 0x4000), static_cast<std::uint8_t>(chr.size() / 0x2000),
                 static_cast<std::uint8_t>(((mapper & 0x0F) << 4) | (trainer ? 4 : 0)), static_cast<std::uint8_t>(mapper & 0xF0), 0, 0, 0, 0, 0, 0, 0, 0};
    if (trainer) out.insert(out.end(), 0x200, 0x77);
    out.insert(out.end(), prg.begin(), prg.end());
    out.insert(out.end(), chr.begin(), chr.end());
    return out;
}

Bytes chr_bank() {
    Bytes chr(0x2000);
    for (std::size_t i = 0; i < chr.size(); ++i) chr[i] = static_cast<std::uint8_t>(i % 251);
    return chr;
}

std::vector<std::string> lines_of(const std::filesystem::path& p) {
    std::vector<std::string> out;
    const std::string content = fs::read_text(p);   // (the lines are views into it)
    for (const std::string_view l : text::lines(content)) out.emplace_back(l);
    return out;
}

CB_TEST("source.nes_writer.the_program_comes_back_as_source_that_builds_the_same_rom", [] {
    fs::ScopedTempDir dir("cb_nes");
    const Bytes image = ines(prg_bank(), chr_bank());
    const image_writer::Written r = write(formats::nes::parse(image), "demo.nes", dir.path());
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    CB_CHECK((r.functions == image_writer::Functions{{0xC000, "reset"}, {0xC00C, "sub_C00C"}, {0xC015, "nmi"}, {0xC016, "irq"}}));
    CB_CHECK_EQ(r.code_bytes, 23u);        // every byte the flow reaches, and no other
    CB_CHECK_EQ(r.image_bytes, 0x4000u);   // the PRG
    CB_CHECK(r.source == dir.path() / "demo.s");
    CB_CHECK(r.built && fs::read_file(*r.built) == image);
    CB_CHECK(fs::read_file(dir.path() / "demo.chr") == chr_bank());   // the CHR beside the source, which names it

    // the head: the hardware by name, labels at the targets, the first name a vector gives
    const std::vector<std::string> lines = lines_of(r.source);
    const std::vector<std::string> head = {
        "; demo.s -- written by CodeBreaker from demo.nes",
        "; mapper 0, 16 KB PRG, 8 KB CHR, horizontal mirroring",
        "",
        "PPUCTRL    = $2000",
        "PPUSTATUS  = $2002",
        "",
        ".segment \"HEADER\"",
        "        .byte $4E, $45, $53, $1A, $01, $01, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00",
        "",
        ".segment \"BANK0\"",
        "",
        "reset:",
        "        SEI",
        "        LDA #$00",
        "        STA PPUCTRL",
        "L_C006:",
        "        JSR sub_C00C",
        "        JMP L_C006",
        "",
        "sub_C00C:",
        "        LDA PPUSTATUS",
        "        BPL sub_C00C",
        "        LDA D_C020,x",   // an absolute operand that names a line's start gets that line's label
        "        RTS",
        "",
        "nmi:",
        "        RTI",
        "",
        "irq:",
        "        RTI",
        "        .byte $FF, $FF, $FF, $FF, $FF, $FF, $FF, $FF, $FF",
        "D_C020:",
        "        .byte $53, $43, $4F, $52, $45, $01, $02, $03, $FF, $FF, $FF, $FF, $FF, $FF, $FF, $FF"};
    CB_CHECK(lines.size() > head.size());
    CB_CHECK(std::equal(head.begin(), head.end(), lines.begin()));
    // the tail: the last six bytes of the bank as the vectors, then the CHR taken in from its own file
    const std::vector<std::string> tail = {"        .byte $FF, $FF, $FF, $FF, $FF, $FF, $FF, $FF, $FF, $FF",
                                           "        .word nmi, reset, irq",
                                           "",
                                           ".segment \"CHARS\"",
                                           "        .incbin \"demo.chr\"",
                                           ""};
    CB_CHECK(lines.size() >= tail.size() && std::equal(tail.begin(), tail.end(), lines.end() - static_cast<std::ptrdiff_t>(tail.size())));

    // the linker configuration says where each segment goes
    CB_CHECK(r.config && *r.config == dir.path() / "demo.cfg");
    CB_CHECK_EQ(fs::read_text(*r.config),
                "MEMORY {\n    HEADER: start = $0000, size = $0010, fill = yes, file = %O;\n    PRG0: start = $C000, size = $4000, fill = yes, file = %O;\n"
                "    CHR: start = $0000, size = $2000, fill = yes, file = %O;\n}\n"
                "SEGMENTS {\n    HEADER: load = HEADER, type = ro;\n    BANK0: load = PRG0, type = ro;\n    CHARS: load = CHR, type = ro;\n}\n");
});

CB_TEST("source.nes_writer.writing_twice_gives_the_same_source", [] {
    fs::ScopedTempDir dir("cb_nes");
    const formats::nes::Rom rom = formats::nes::parse(ines(prg_bank(), chr_bank()));
    const image_writer::Written a = write(rom, "demo.nes", dir.path() / "a");
    const image_writer::Written b = write(rom, "demo.nes", dir.path() / "b");
    CB_CHECK(fs::read_file(a.source) == fs::read_file(b.source));
});

CB_TEST("source.nes_writer.a_cartridge_without_chr_or_with_a_trainer", [] {
    fs::ScopedTempDir dir("cb_nes");
    const Bytes image = ines(prg_bank(), Bytes{}, 0, true);
    const image_writer::Written r = write(formats::nes::parse(image), "plain.nes", dir.path());
    CB_CHECK(r.same);
    CB_CHECK(!std::filesystem::exists(dir.path() / "plain.chr"));   // no CHR: no file, no segment
    const std::string source = fs::read_text(r.source);
    CB_CHECK(source.find("CHARS") == std::string::npos);
    CB_CHECK(source.find(".segment \"TRAINER\"") != std::string::npos);   // the trainer is written ahead of the PRG, and the build brings it back
    const std::string config = fs::read_text(*r.config);
    CB_CHECK(config.find("TRAINER: start = $7000, size = $0200") != std::string::npos);
    CB_CHECK(config.find("CHR:") == std::string::npos);
    CB_CHECK(fs::read_file(*r.built) == image);
});

CB_TEST("source.nes_writer.banks_of_a_cartridge", [] {
    const auto shape = [](const Bytes& prg, std::uint8_t mapper) {
        std::string out;
        for (const Bank& b : banks(formats::nes::parse(ines(prg, Bytes{}, mapper)))) out += std::format("{} ${:04X} +{:X} {:X};", b.segment, b.base, b.offset, b.size);
        return out;
    };
    CB_CHECK_EQ(shape(Bytes(0x4000, 0), 0), "BANK0 $C000 +0 4000;");                      // 16 KB sits at the top
    CB_CHECK_EQ(shape(Bytes(0x8000, 0), 0), "BANK0 $8000 +0 8000;");                      // 32 KB fills the two slots
    CB_CHECK_EQ(shape(Bytes(0x8000, 0), 1), "BANK0 $8000 +0 8000;");                      // as much, whatever the board
    CB_CHECK_EQ(shape(Bytes(0x10000, 0), 1), "BANK0 $8000 +0 4000;BANK1 $8000 +4000 4000;BANK2 $8000 +8000 4000;BANK3 $C000 +C000 4000;");   // the last bank is fixed at the top
    CB_CHECK_EQ(shape(Bytes(0x10000, 0), 0), "BANK0 $8000 +0 10000;");                    // (a board with no mapper cannot hold 64 KB, but a file may say so)
});

CB_TEST("source.nes_writer.a_banked_cartridge_is_written_one_segment_a_bank", [] {
    fs::ScopedTempDir dir("cb_nes");
    Bytes prg(0x4000 * 3, 0xEE);
    const Bytes last = prg_bank();   // the fixed bank at $C000 holds the program
    prg.insert(prg.end(), last.begin(), last.end());
    const Bytes image = ines(prg, Bytes{}, 1);
    const image_writer::Written r = write(formats::nes::parse(image), "banked.nes", dir.path());
    CB_CHECK(r.same);
    const std::string source = fs::read_text(r.source);
    for (const char* bank : {"BANK0", "BANK1", "BANK2", "BANK3"}) CB_CHECK(source.find(std::format(".segment \"{}\"", bank)) != std::string::npos);
    CB_CHECK(source.find("        .word nmi, reset, irq") != std::string::npos);   // only the bank at the top has the vectors
    CB_CHECK_EQ(r.functions.size(), 4u);
    const std::string config = fs::read_text(*r.config);
    CB_CHECK(config.find("PRG3: start = $C000, size = $4000") != std::string::npos);
    CB_CHECK(config.find("PRG1: start = $8000, size = $4000") != std::string::npos);
});

CB_TEST("source.nes_writer.a_file_that_is_not_a_cartridge_is_refused", [] {
    fs::ScopedTempDir dir("cb_nes");
    fs::write_file(dir.path() / "junk.nes", Bytes(64, 0x41));
    CB_CHECK_THROWS_AS(FormatError, reconstruct(dir.path() / "junk.nes", dir.path() / "out"));
    CB_CHECK_THROWS_AS(IoError, reconstruct(dir.path() / "missing.nes", dir.path() / "out"));
});

CB_TEST("source.nes_writer.reconstruct_reads_the_file_and_names_the_output_for_it", [] {
    fs::ScopedTempDir dir("cb_nes");
    fs::write_file(dir.path() / "Game One.nes", ines(prg_bank(), chr_bank()));
    const image_writer::Written r = reconstruct(dir.path() / "Game One.nes", dir.path() / "out");
    CB_CHECK(r.same);
    CB_CHECK(r.source == dir.path() / "out" / "Game One.s");
    CB_CHECK(std::filesystem::exists(dir.path() / "out" / "Game One.cfg"));
});

CB_TEST("source.nes_writer.the_registers_are_the_documented_ones", [] {
    CB_CHECK_EQ(hardware().size(), 30u);
    CB_CHECK_EQ(hardware().at(0x2007), "PPUDATA");
    CB_CHECK_EQ(hardware().at(0x4014), "OAMDMA");
    CB_CHECK_EQ(hardware().at(0x4017), "JOY2");
});

}  // namespace
}  // namespace cb::source::nes_writer
