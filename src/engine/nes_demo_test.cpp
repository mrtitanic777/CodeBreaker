// engine/nes_demo_test.cpp -- the NES lane on the repository's own demo cartridge, end to end (the NES lane checks):
// the cartridge, the program run on the 6502 interpreter, the source written back, the SLEIGH 6502 specification against the interpreter,
// and the native decompiler's C of it.
//
// The program is engine/testdata/lane_kit/nes_demo.asm, hand-written for the lane, built with the one assembler (the 6502 description). The tests that need the native decompiler (decompiler.exe and pcodecheck.exe, built beside the tests)
// print SKIP when it is not built; the lane's judge on this cartridge is
// engine.lane_kit.machines.the_nes_demo_cartridge_is_judged.
//
// Names here are file-local to this unity unit: every one starts with `nd_`.
#include <algorithm>
#include <cstdio>
#include <format>
#include <iostream>
#include <map>
#include <optional>
#include <random>
#include <string>
#include <vector>

#include "base/cache.hpp"
#include "source/processors.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/process.hpp"
#include "core/testing.hpp"
#include "engine/pcode_cpu.hpp"
#include "core/text.hpp"
#include "decompile/decompiler.hpp"
#include "decompile/decompiler_internal.hpp"
#include "decompile/m6502.hpp"
#include "emulate/m6502.hpp"
#include "engine/lane_kit.hpp"
#include "formats/nes.hpp"
#include "source/isa.hpp"
#include "source/nes_writer.hpp"

namespace dc = cb::decompile::decompiler;
namespace dsc = cb::decompile::describe;
namespace isa6502 = cb::decompile::m6502;
namespace cpu6502 = cb::emulate::m6502;
namespace nl2 = cb::translate::native_l2;
namespace stdfs = std::filesystem;
using cb::Bytes;

namespace {

// ---- the cartridge -------------------------------------------------------------------------------------------------------------

Bytes nd_cartridge_bytes(const stdfs::path& work) {
    return cb::source::processors::assemble_described(cb::source::processors::find("6502"),
                                                      cb::fs::read_text(cb::testing::data_path("engine/testdata/lane_kit/nes_demo.asm")), work).data;
}

cb::formats::Decoder nd_decoder() {
    return [](cb::ByteView memory, std::uint32_t pc) -> std::optional<cb::formats::DecodedInsn> {
        const std::optional<isa6502::Insn> insn = isa6502::decode(memory, pc, pc);
        if (!insn) return std::nullopt;
        cb::formats::DecodedInsn out;
        out.size = insn->size;
        if (insn->target) out.target = *insn->target;
        if (insn->mnemonic == "JSR") out.flow = cb::formats::Flow::Call;
        else if (insn->mnemonic == "JMP") out.flow = out.target ? cb::formats::Flow::Jump : cb::formats::Flow::Indirect;
        else if (insn->mnemonic == "RTS" || insn->mnemonic == "RTI") out.flow = cb::formats::Flow::Return;
        else if (insn->mnemonic == "BRK") out.flow = cb::formats::Flow::Halt;
        else out.flow = out.target ? cb::formats::Flow::Branch : cb::formats::Flow::Next;
        return out;
    };
}

// The native decompiler's folder: CB_DECOMPILER_BIN, else beside the tests (the one build puts it there); nullopt when none has what
// is asked.
std::optional<stdfs::path> nd_native_bin(const char* program) {
    std::vector<stdfs::path> candidates;
    if (const auto given = cb::fs::get_env("CB_DECOMPILER_BIN"); given && !given->empty()) candidates.push_back(cb::fs::from_text(*given));
    candidates.push_back(cb::fs::executable_path().parent_path());
    for (const stdfs::path& dir : candidates) {
        std::error_code ec;
        if (stdfs::is_regular_file(dir / program, ec)) return dir;
    }
    return std::nullopt;
}

stdfs::path nd_languages() { return stdfs::path(CB_SOURCE_DIR) / "decompile" / "engine" / "languages"; }

// ---- the SLEIGH 6502 against the interpreter -----------------------------------------------------------------------------------------

constexpr std::uint16_t kPc = 0x0400;

struct NdState {
    std::uint8_t a, x, y, s, p;
    Bytes code;   // the instruction's bytes, three of them given
};

// Memory: a pattern over the first 4 KB (the zero page, the stack, the code, the operands' pages), the rest zero, which is what the p-code
// side reads where nothing is given; the return address of an RTS or RTI is at $1FE.
Bytes nd_memory(const NdState& st) {
    Bytes mem(0x10000, 0);
    for (std::size_t i = 0; i < 0x1000; ++i) mem[i] = static_cast<std::uint8_t>((i * 37 + 11) & 0xFF);
    mem[0x1FE] = 0x34;
    mem[0x1FF] = 0x12;
    const std::size_t size = isa6502::instruction_size(isa6502::opcode_table()[st.code[0]]->mode);
    for (std::size_t i = 0; i < 3; ++i) mem[kPc + i] = i < size ? st.code[i] : std::uint8_t{0xEA};   // (what follows decodes as NOP)
    return mem;
}

struct NdOutcome {
    std::map<std::string, unsigned> regs;   // PC A X Y S N V D I Z C
    std::map<unsigned, unsigned> writes;    // address -> the new value, of the bytes that changed
    std::string error;
};

NdOutcome nd_interpret(const NdState& st) {
    Bytes mem = nd_memory(st);
    const Bytes before = mem;
    cpu6502::Bus bus = cpu6502::Bus::over(mem);
    cb::engine::pcode_cpu::M6502Cpu cpu(bus);   // (the one emulator, a step at a time: the 2A03's binary arithmetic)
    cpu.regs = {st.a, st.x, st.y, st.s, st.p, kPc};
    NdOutcome out;
    if (cpu.step()) {
        out.error = "the interpreter stopped";
        return out;
    }
    const unsigned p = cpu.regs.p;
    out.regs = {{"PC", cpu.regs.pc},  {"A", cpu.regs.a},     {"X", cpu.regs.x},     {"Y", cpu.regs.y},     {"S", cpu.regs.s},     {"N", p >> 7 & 1u},
                {"V", p >> 6 & 1u}, {"D", p >> 3 & 1u}, {"I", p >> 2 & 1u}, {"Z", p >> 1 & 1u}, {"C", p & 1u}};
    for (unsigned a = 0; a < mem.size(); ++a)
        if (mem[a] != before[a]) out.writes[a] = mem[a];
    return out;
}

// The p-code answers for every case, the program started once: "c<i> ok LIN=.. A=.. W<addr>=<byte> ..." a line each.
std::map<std::size_t, NdOutcome> nd_pcode(const stdfs::path& pcodecheck, const std::vector<NdState>& cases) {
    std::string input;
    for (std::size_t i = 0; i < cases.size(); ++i) {
        const NdState& st = cases[i];
        const unsigned p = st.p;
        input += std::format("c{} A={:x} X={:x} Y={:x} SP={:x} N={} V={} B={} D={} I={} Z={} C={} IP={:x} M0=", i, st.a, st.x, st.y, 0x100u | st.s,
                             p >> 7 & 1, p >> 6 & 1, p >> 4 & 1, p >> 3 & 1, p >> 2 & 1, p >> 1 & 1, p & 1, kPc);
        for (const std::uint8_t b : nd_memory(st)) input += std::format("{:02x}", b);
        input += '\n';
    }
    cb::process::ProcessSpec spec;
    spec.argv = {cb::fs::to_text(pcodecheck), cb::fs::to_text(nd_languages() / "6502.sla"), "6502"};
    spec.stdin_data = Bytes(input.begin(), input.end());
    spec.timeout = std::chrono::minutes(5);
    const cb::process::ProcessResult r = cb::process::run(spec);
    std::map<std::size_t, NdOutcome> out;
    const std::string text(r.out.begin(), r.out.end());
    for (const std::string_view line : cb::text::lines(text)) {
        const auto parts = cb::text::split(line, ' ');
        if (parts.size() < 2 || parts[0].size() < 2) continue;
        const std::size_t i = *cb::text::parse_int<std::size_t>(parts[0].substr(1), 10);
        NdOutcome o;
        if (parts[1] != "ok") {
            o.error = std::string(line.substr(parts[0].size() + 1));
            out[i] = std::move(o);
            continue;
        }
        const Bytes before = nd_memory(cases.at(i));
        for (std::size_t k = 2; k < parts.size(); ++k) {
            const std::string_view token = parts[k];
            const std::size_t eq = token.find('=');
            if (eq == std::string_view::npos) continue;
            const std::string key(token.substr(0, eq));
            const unsigned value = *cb::text::parse_int<unsigned>(token.substr(eq + 1), 16);
            if (key[0] == 'W') {
                const unsigned address = *cb::text::parse_int<unsigned>(std::string_view(key).substr(1), 16);
                if (address < before.size() && before[address] != value) o.writes[address] = value;   // (a store of what was there is no change)
            } else if (key == "LIN") {
                o.regs["PC"] = value & 0xFFFF;
            } else if (key == "SP") {
                o.regs["S"] = value & 0xFF;
            } else if (key == "A" || key == "X" || key == "Y" || key == "N" || key == "V" || key == "D" || key == "I" || key == "Z" || key == "C") {
                o.regs[key] = value;
            }
        }
        out[i] = std::move(o);
    }
    return out;
}

// ---- the decompiler, as the lane runs it --------------------------------------------------------------------------------------------

class NdEnvironment final : public dc::Environment {
public:
    dsc::Knowledge& knowledge() override { return knowledge_; }
    const nl2::Library& library() override { throw cb::UsageError("the NES lane asks nothing of the L2 library here"); }
    std::map<std::string, std::string> import_dlls(const stdfs::path&) override { return {}; }
    std::optional<std::uint32_t> argument_bytes(std::string_view, std::string_view) override { return std::nullopt; }
    std::optional<std::vector<cb::decompile::struct_text::Structure>> recover_structures(const stdfs::path&) override { return std::nullopt; }
    dsc::PowerPcFacts powerpc_facts(const stdfs::path&) override { return {}; }
    std::vector<dc::Va> powerpc_functions(const stdfs::path&) override { return {}; }

private:
    dsc::NoKnowledge knowledge_;
};

}  // namespace

// The cartridge: the program's vectors and the functions the flow from them finds.
CB_TEST("engine.nes_demo.the_cartridge_and_its_functions", [] {
    cb::fs::ScopedTempDir dir("cb-nes-demo");
    const Bytes image = nd_cartridge_bytes(dir.path());
    const cb::formats::nes::Rom rom = cb::formats::nes::parse(image);
    CB_CHECK_EQ(rom.mapper, 0);
    CB_CHECK(rom.mirroring == cb::formats::nes::Mirroring::Vertical);
    CB_CHECK_EQ(rom.prg.size(), std::size_t{0x4000});
    CB_CHECK_EQ(rom.chr.size(), std::size_t{0x2000});
    CB_CHECK_EQ(rom.vectors.nmi, 0xC05C);
    CB_CHECK_EQ(rom.vectors.reset, 0xC000);
    CB_CHECK_EQ(rom.vectors.irq, 0xC076);
    CB_CHECK((rom.functions(nd_decoder()) == std::vector<std::uint32_t>{0xC000, 0xC04F, 0xC05C, 0xC076}));
    const Bytes mem = rom.memory();   // 16 KB: mirrored at $8000 and $C000
    CB_CHECK(std::equal(mem.begin() + 0x8000, mem.begin() + 0xC000, rom.prg.begin()));
    CB_CHECK(std::equal(mem.begin() + 0xC000, mem.end(), rom.prg.begin()));
});

// test_interpreter: the multiply subroutine (6 * 7), and the reset routine against a PPU that is in vblank at once.
CB_TEST("engine.nes_demo.the_program_runs_on_the_interpreter", [] {
    cb::fs::ScopedTempDir dir("cb-nes-demo");
    const cb::formats::nes::Rom rom = cb::formats::nes::parse(nd_cartridge_bytes(dir.path()));
    {
        cpu6502::Bus bus{cb::ByteView(rom.memory())};
        bus.memory()[0x10] = 6;
        bus.memory()[0x11] = 7;
        const cpu6502::RunResult run = cb::engine::pcode_cpu::run(bus, 0xC04F);   // multiply
        CB_CHECK(run.end == cpu6502::End::Return);
        CB_CHECK_EQ(run.regs.a, 42);
    }
    cpu6502::Bus bus{cb::ByteView(rom.memory())};
    bus.map(0x2000, 0x401F, [](std::uint16_t) -> std::uint8_t { return 0x80; });   // in vblank at once
    const cpu6502::RunResult run = cb::engine::pcode_cpu::run(bus, 0xC000, {.max_steps = 4000});   // reset: ends in its loop
    std::vector<std::pair<unsigned, unsigned>> writes;
    for (const cpu6502::DeviceAccess& io : bus.log())
        if (io.kind == cpu6502::DeviceAccess::Kind::Write) writes.emplace_back(io.address, io.value);
    CB_CHECK(writes.size() >= 2);
    if (writes.size() >= 2) {
        CB_CHECK((writes[0] == std::pair<unsigned, unsigned>{0x2000, 0}));
        CB_CHECK((writes[1] == std::pair<unsigned, unsigned>{0x2001, 0}));
    }
    CB_CHECK(std::ranges::find(writes, std::pair<unsigned, unsigned>{0x2006, 0x3F}) != writes.end());
    std::vector<unsigned> palette;
    for (const auto& [address, value] : writes)
        if (address == 0x2007) palette.push_back(value);
    CB_CHECK((palette == std::vector<unsigned>{0x0F, 0x16, 0x27, 0x30}));
    CB_CHECK_EQ(bus.memory()[0x12], 42);
    CB_CHECK(run.end == cpu6502::End::Limit);
});

// test_source_rebuilds: the cartridge as source, the source built again as the cartridge, and the source says what the program does.
CB_TEST("engine.nes_demo.the_source_rebuilds_the_cartridge", [] {
    cb::fs::ScopedTempDir dir("cb-nes-demo");
    const stdfs::path cartridge = dir.path() / "demo.nes";
    cb::fs::write_file(cartridge, nd_cartridge_bytes(dir.path()));
    const cb::source::image_writer::Written r = cb::source::nes_writer::reconstruct(cartridge, dir.path() / "src");
    CB_CHECK(r.linked && r.same);
    CB_CHECK(r.notes.empty());
    const std::string text = cb::fs::read_text(r.source);
    for (const char* want : {"reset:", "nmi:", "JSR sub_C04F", "BIT PPUSTATUS", "STA PPUADDR", "LDA D_C077,x", ".word nmi, reset, irq"})
        CB_CHECK(text.find(want) != std::string::npos);
});

// The SLEIGH cross-check: every documented opcode, in states of every register, one instruction on the SLEIGH specification's p-code and on
// the interpreter; A X Y S, the flags, the next address and the bytes written must agree.
CB_TEST("engine.nes_demo.sleigh_6502_agrees_with_the_interpreter", [] {
    CB_WINDOWS_ONLY("it needs the p-code lifting program (a Windows executable)");
    const std::optional<stdfs::path> bin = nd_native_bin("pcodecheck.exe");
    if (!bin) {
        std::cout << "SKIP pcodecheck is not built (decompile/engine/CMakeLists.txt)\n";
        return;
    }
    std::mt19937 rnd(6502);
    const auto below = [&](unsigned n) { return static_cast<unsigned>(rnd() % n); };
    std::vector<NdState> cases;
    const auto& table = isa6502::opcode_table();
    for (unsigned op = 0; op < 256; ++op) {
        if (!table[op]) continue;
        for (unsigned k = 0; k < 8; ++k) {
            NdState st;
            st.a = static_cast<std::uint8_t>(below(256));
            st.x = static_cast<std::uint8_t>(below(256));
            st.y = static_cast<std::uint8_t>(below(256));
            const unsigned stacks[] = {0xFD, 0xF0, 0x80, 4 + below(248)};
            st.s = static_cast<std::uint8_t>(stacks[below(4)]);
            st.p = static_cast<std::uint8_t>((below(256) & ~0x08u) | 0x20u);   // (decimal off: the 2A03's case)
            if (k % 6 == 0) {
                st.a = 0x7F;
                st.x = 0xFF;   // overflow and wrap edges
            }
            std::uint8_t b1 = static_cast<std::uint8_t>(below(256)), b2 = static_cast<std::uint8_t>(below(256));
            if (k % 5 == 1) b1 = 0xFF;                                      // zero-page and page-end wraps
            if (table[op]->mode == isa6502::Mode::Rel && b1 == 0xFF) b1 = 0xFE;   // (not a branch into its own operand)
            st.code = {static_cast<std::uint8_t>(op), b1, b2};
            cases.push_back(std::move(st));
        }
    }
    const std::map<std::size_t, NdOutcome> got = nd_pcode(*bin / "pcodecheck.exe", cases);
    std::map<std::string, std::pair<int, int>> per_form;   // form -> {agree, cases}
    std::map<std::string, std::string> example;
    for (std::size_t i = 0; i < cases.size(); ++i) {
        const auto& entry = *table[cases[i].code[0]];
        const std::string form = std::format("{} {}", entry.mnemonic, isa6502::mode_name(entry.mode));
        const NdOutcome mine = nd_interpret(cases[i]);
        const auto theirs = got.find(i);
        std::string why;
        if (theirs == got.end()) {
            why = "no answer";
        } else if (!theirs->second.error.empty()) {
            why = theirs->second.error;
        } else {
            for (const auto& [name, value] : mine.regs) {
                const auto it = theirs->second.regs.find(name);
                if (it == theirs->second.regs.end() || it->second != value) why += name + " ";
            }
            if (mine.writes != theirs->second.writes) why += "memory ";
        }
        auto& [agree, total] = per_form[form];
        ++total;
        if (why.empty()) {
            ++agree;
        } else if (!example.contains(form)) {
            example[form] = std::format("{} differs in {} (code {:02x} {:02x} {:02x})", form, why, cases[i].code[0], cases[i].code[1], cases[i].code[2]);
        }
    }
    CB_CHECK(per_form.size() > 50);
    for (const auto& [form, counts] : per_form) {
        if (counts.first != counts.second) std::cout << "  " << example[form] << "\n";
        CB_CHECK_EQ(counts.first, counts.second);
    }
});

// test_decompiles: the native decompiler's C of the cartridge: every function found is decompiled, the PPU's registers are volatile accesses,
// and the multiply's loop adds the zero-page operand.
CB_TEST("engine.nes_demo.the_native_decompiler_gives_the_c", [] {
    CB_WINDOWS_ONLY("it needs the decompiler program (a Windows executable)");
    const std::optional<stdfs::path> bin = nd_native_bin("decompiler.exe");
    if (!bin) {
        std::cout << "SKIP decompiler.exe is not built (decompile/engine/CMakeLists.txt)\n";
        return;
    }
    cb::fs::ScopedTempDir dir("cb-nes-demo");
    const stdfs::path cartridge = dir.path() / "demo.nes";
    cb::fs::write_file(cartridge, nd_cartridge_bytes(dir.path()));
    cb::base::cache::Config cache_config;
    cache_config.state_dir = dir.path() / "cache";
    cache_config.shared_dir = dir.path() / "shared";
    cb::base::cache::Cache cache(cache_config);
    NdEnvironment environment;
    dc::Context context;
    context.settings.checkout = stdfs::path(CB_SOURCE_DIR).parent_path();
    context.settings.program = *bin / "decompiler.exe";
    context.settings.languages = nd_languages();
    context.settings.struct_recovery = "off";
    context.cache = &cache;
    context.environment = &environment;
    context.limits.workers.workers = 1;
    context.limits.min_free_gb = 0.0;
    context.limits.slot_dir = dir.path() / "slots";
    context.scratch = dir.path() / "scratch";
    dc::Decompiler decompiler(context);
    const dc::AsC c = decompiler.as_c(cartridge);
    CB_CHECK_EQ(c.decompiled, std::size_t{4});
    CB_CHECK_EQ(c.found, std::size_t{4});
    CB_CHECK(c.text.find("RAM2006 = 0x3f;") != std::string::npos);   // both writes: volatile
    CB_CHECK(c.text.find("RAM2006 = 0;") != std::string::npos);
    CB_CHECK(c.text.find("+ cRAM0010") != std::string::npos);        // the multiply's loop
});
