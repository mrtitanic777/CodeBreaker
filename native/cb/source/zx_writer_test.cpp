// Tests of source/zx_writer on the Spectrum demo program (hand-written, kept in
// engine/testdata/lane_kit/zx_demo.asm): built with CodeBreaker's own flat Z80 assembler into a tape (a BASIC loader that calls USR 32768,
// then the code block) and into a 48K snapshot stopped at its start; the program comes back as source that builds the same bytes, and the
// source names the Spectrum's ULA port, ROM routines and system variables.
#include "cb/source/zx_writer.hpp"

#include "cb/build/z80_asm.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/zx.hpp"
#include "cb/source/isa.hpp"

namespace cb::source::zx_writer {
namespace {

Bytes tap_block(std::uint8_t flag, const Bytes& payload) {
    Bytes body{flag};
    body.insert(body.end(), payload.begin(), payload.end());
    std::uint8_t x = 0;
    for (const std::uint8_t b : body) x ^= b;
    body.push_back(x);
    Bytes out{static_cast<std::uint8_t>(body.size()), static_cast<std::uint8_t>(body.size() >> 8)};
    out.insert(out.end(), body.begin(), body.end());
    return out;
}

void put16(Bytes& out, std::uint32_t v) {
    out.push_back(static_cast<std::uint8_t>(v));
    out.push_back(static_cast<std::uint8_t>(v >> 8));
}

Bytes tap_header(std::uint8_t type, std::string name, std::uint32_t length, std::uint32_t p1, std::uint32_t p2) {
    name.resize(10, ' ');
    Bytes out{type};
    out.insert(out.end(), name.begin(), name.end());
    put16(out, length);
    put16(out, p1);
    put16(out, p2);
    return out;
}

build::z80_asm::Assembler::Flat demo() {
    const build::z80_asm::Assembler assembler(isa::z80_encoder());
    return assembler.build_flat(fs::read_text(testing::data_path("engine/testdata/lane_kit/zx_demo.asm")));
}

// 10 RANDOMIZE USR 32768, then the code block
Bytes demo_tape() {
    const auto flat = demo();
    Bytes line = {0xF9, 0xC0, '3', '2', '7', '6', '8', 0x0E, 0x00, 0x00, static_cast<std::uint8_t>(flat.start), static_cast<std::uint8_t>(flat.start >> 8), 0x00, 0x0D};
    Bytes basic = {0, 10};   // the line number is big-endian
    put16(basic, static_cast<std::uint32_t>(line.size()));
    basic.insert(basic.end(), line.begin(), line.end());
    Bytes tape = tap_block(0, tap_header(0, "demo", static_cast<std::uint32_t>(basic.size()), 10, static_cast<std::uint32_t>(basic.size())));
    for (const Bytes& block : {tap_block(0xFF, basic), tap_block(0, tap_header(3, "democode", static_cast<std::uint32_t>(flat.bytes.size()), flat.start, 32768)),
                               tap_block(0xFF, flat.bytes)}) {
        tape.insert(tape.end(), block.begin(), block.end());
    }
    return tape;
}

// a 48K snapshot stopped at the program's start: the PC is on the stack, where RETN pops it
Bytes demo_snapshot() {
    const auto flat = demo();
    Bytes mem(0x10000, 0);
    std::copy(flat.bytes.begin(), flat.bytes.end(), mem.begin() + flat.start);
    const std::uint32_t sp = 0xFF00;
    mem[sp] = static_cast<std::uint8_t>(flat.start);
    mem[sp + 1] = static_cast<std::uint8_t>(flat.start >> 8);
    Bytes data = {0x3F};
    data.resize(15, 0);       // I and the registers
    data.resize(23, 0);       // the alternate pair, the border, the interrupt flags ...
    put16(data, sp);
    data.push_back(1);        // interrupt mode
    data.push_back(7);        // border
    data.insert(data.end(), mem.begin() + 0x4000, mem.end());
    return data;
}

bool has(const std::string& text, std::string_view line) { return text.find(line) != std::string::npos; }

CB_TEST("source.zx_writer.a_tape_comes_back_as_source_that_builds_the_same_bytes", [] {
    fs::ScopedTempDir dir("cb_zx_writer");
    const std::filesystem::path tape = dir.path() / "demo.tap";
    fs::write_file(tape, demo_tape());
    const formats::zx::Program program = formats::zx::read(tape);
    CB_CHECK_EQ(program.origin, 0x8000u);
    CB_CHECK_EQ(program.entries.size(), std::size_t{1});
    CB_CHECK_EQ(program.entries[0].address, std::uint16_t{0x8000});
    CB_CHECK_EQ(program.entries[0].name, "start");
    const image_writer::Written r = reconstruct(tape, dir.path() / "out");
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    const std::string text = fs::read_text(r.source);
    for (const char* line : {"CALL CHAN_OPEN", "OUT (ULA),A", "LD HL,FRAMES", "RST $10"}) CB_CHECK(has(text, line));
});

CB_TEST("source.zx_writer.a_snapshot_stopped_at_the_start_is_the_same_program", [] {
    fs::ScopedTempDir dir("cb_zx_writer");
    const std::filesystem::path snapshot = dir.path() / "demo.sna";
    fs::write_file(snapshot, demo_snapshot());
    const formats::zx::Program program = formats::zx::read(snapshot);
    CB_CHECK(program.registers.has_value());
    CB_CHECK_EQ(program.registers->sp, std::uint16_t{0xFF02});   // the PC was popped
    bool start = false;
    for (const formats::zx::Entry& e : program.entries) start = start || (e.address == 0x8000 && e.name == "start");
    CB_CHECK(start);
    const image_writer::Written r = reconstruct(snapshot, dir.path() / "out");
    CB_CHECK(r.same);
});

}  // namespace
}  // namespace cb::source::zx_writer
