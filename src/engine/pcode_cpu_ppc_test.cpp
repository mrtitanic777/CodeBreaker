// The PowerPC lanes on the one emulator (engine/pcode_cpu_ppc.cpp): the same run as the interpreter on random programs, and each lane's homebrew judged the same with
// either core. Floating point runs on the interpreter in both, so no NaN is chosen by a host here.
#include <algorithm>
#include <array>
#include <cstdio>
#include <format>
#include <map>
#include <random>
#include <string>
#include <vector>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "emulate/powerpc.hpp"
#include "engine/pcode_cpu.hpp"
#include "engine/pcode_cpu_compare.hpp"
#include "formats/ppc_program.hpp"

using namespace cb;
namespace pw = cb::emulate::powerpc;

namespace {

std::string ppc_diff(const pw::RunResult& a, const pw::RunResult& b) {
    std::string out;
    if (a.exit_text() != b.exit_text() || a.truncated != b.truncated) out += std::format("exit('{}' / '{}') ", a.exit_text(), b.exit_text());
    const pw::Machine &x = *a.machine, &y = *b.machine;
    for (unsigned i = 0; i < 32; ++i)
        if (x.r[i] != y.r[i]) out += std::format("r{}[own {:X} emu {:X}] ", i, x.r[i], y.r[i]);
    for (unsigned i = 0; i < 32; ++i)
        if (x.f[i] != y.f[i] || x.ps1[i] != y.ps1[i]) {
            out += std::format("f{} ", i);
            break;
        }
    if (x.cr != y.cr) out += std::format("cr[{:08X}/{:08X}] ", x.cr, y.cr);
    if (x.lr != y.lr) out += std::format("lr[{:X}/{:X}] ", x.lr, y.lr);
    if (x.ctr != y.ctr) out += std::format("ctr[{:X}/{:X}] ", x.ctr, y.ctr);
    if (x.ca != y.ca) out += "ca ";
    if (a.pc != b.pc) out += std::format("pc[{:X}/{:X}] ", a.pc, b.pc);
    if (a.steps != b.steps) out += std::format("steps[{}/{}] ", a.steps, b.steps);
    if (a.writes.size() != b.writes.size()) {
        out += std::format("writes[{}/{}] ", a.writes.size(), b.writes.size());
        for (const auto* list : {&a.writes, &b.writes}) {
            out += list == &a.writes ? "own{" : "emu{";
            for (std::size_t i = 0; i < list->size() && i < 8; ++i) out += std::format("{:X}={:02X} ", (*list)[i].address, (*list)[i].after);
            out += "} ";
        }
    }
    else
        for (std::size_t i = 0; i < a.writes.size(); ++i)
            if (a.writes[i].address != b.writes[i].address || a.writes[i].after != b.writes[i].after) {
                out += std::format("write@{:X} ", a.writes[i].address);
                break;
            }
    if (x.io().size() != y.io().size()) out += std::format("io[{}/{}] ", x.io().size(), y.io().size());
    return out;
}

}  // namespace

// One instruction word at a time as the first word of a random program (every major opcode and extended opcode in turn), from random registers and memory.
CB_TEST("engine.pcode_cpu.ppc_instructions_run_as_the_interpreter_does", [] {
    std::mt19937 rng(20261121);
    std::map<std::string, unsigned> by_kind;
    std::map<std::string, std::string> example;
    unsigned runs = 0, different = 0;
    const unsigned total = [] {
        const auto given = fs::get_env("CB_PCODE_EXHAUSTIVE");
        return given && !given->empty() && *given != "0" ? static_cast<unsigned>(std::strtoul(given->c_str(), nullptr, 10)) : 6000u;
    }();
    constexpr std::uint32_t kCode = 0x10000000;
    for (unsigned n = 0; n < 2 * total; ++n) {
        const bool single = n < total;
        Bytes code(0x100);
        for (auto& b : code) b = static_cast<std::uint8_t>(rng());
        std::uint32_t first = static_cast<std::uint32_t>(rng());
        if (single) {   // (the opcode and the extended opcode in turn)
            first = (first & 0x03FFF800u) | ((n % 64) << 26) | (((n / 64) * 7 % 1024) << 1) | (rng() & 1);
        }
        for (unsigned k = 0; k < 4; ++k) code[k] = static_cast<std::uint8_t>(first >> (24 - 8 * k));
        pw::Region region;
        region.address = kCode;
        region.data = code;
        pw::Region data;
        data.address = pw::kArgsBase;
        data.data = Bytes(0x4000);
        for (auto& b : data.data) b = static_cast<std::uint8_t>(rng());
        const std::array<pw::Region, 2> regions = {region, data};
        std::array<std::uint64_t, 8> args{};
        for (auto& v : args) v = (rng() & 1) ? pw::kArgsBase + (rng() & 0x3FF8) : (rng() & 3) == 0 ? static_cast<std::uint32_t>(rng()) : rng() & 0xFF;
        pw::RunOptions options;
        options.max_steps = single ? 1 : 30;
        options.paired_singles = (rng() & 1) != 0;
        options.fill = pw::Fill::Counts;
        pw::RunResult rr[2];
        for (int core = 0; core < 2; ++core) rr[core] = core == 1 ? engine::pcode_cpu::run(regions, kCode, args, options) : pw::run(regions, kCode, args, options);
        ++runs;
        const std::string fields = ppc_diff(rr[0], rr[1]);
        if (fields.empty()) continue;
        ++different;
        unsigned op = first >> 26;
        std::string where;
        if (!single) {   // a program: the instruction after which the two cores differ is what is at fault
            for (unsigned k = 1; k <= options.max_steps; ++k) {
                pw::RunOptions o2 = options;
                o2.max_steps = k;
                const pw::RunResult ra = pw::run(regions, kCode, args, o2), rb = engine::pcode_cpu::run(regions, kCode, args, o2);
                if (ppc_diff(ra, rb).empty()) continue;
                pw::RunOptions o3 = options;
                o3.max_steps = k - 1;
                const std::uint32_t at = pw::run(regions, kCode, args, o3).pc;
                const std::size_t offset = at - kCode;
                if (offset + 4 <= code.size()) {
                    const std::uint32_t word = static_cast<std::uint32_t>(code[offset]) << 24 | code[offset + 1] << 16 | code[offset + 2] << 8 | code[offset + 3];
                    op = word >> 26;
                    where = std::format("[step {} at {:X}: {:08X}] ", k, at, word);
                }
                break;
            }
        }
        const std::string key = std::format("{}{}", single ? "1: op " : "N: op ", op);
        ++by_kind[key];
        if (!example.count(key)) example[key] = std::format("{:08X}: {}{}", first, where, fields);
    }
    std::printf("pcode_cpu ppc instructions: %u runs, %u differ\n", runs, different);
    unsigned shown = 0;
    for (const auto& [k, c] : by_kind)
        if (++shown <= 40) std::printf("    %u x %s   e.g. %s\n", c, k.c_str(), example[k].c_str());
    CB_CHECK_EQ(different, 0u);
});

// A load or store that runs out of pages stops the run with the instruction undone: its target, its base and the pc as the interpreter leaves them.
CB_TEST("engine.pcode_cpu.ppc_a_memory_fault_leaves_the_state_the_interpreter_leaves", [] {
    constexpr std::uint32_t kCode = 0x10000000;
    struct Program {
        std::vector<std::uint32_t> words;
        std::uint64_t r3;
    };
    const Program programs[] = {
        {{0x94831000, 0x4BFFFFFC}, 0x20000000},               // stwu r4,0x1000(r3); loop
        {{0x84831000, 0x4BFFFFFC}, 0x20000000},               // lwzu r4,0x1000(r3); loop
        {{0x80830000, 0x38631000, 0x4BFFFFF8}, 0x20000000},   // lwz r4,0(r3); addi r3,r3,0x1000; loop
        {{0xBFC30000, 0x38631000, 0x4BFFFFF8}, 0x20000FFC},   // stmw r30,0(r3) across a page; addi; loop
        {{0xBBC30000, 0x38631000, 0x4BFFFFF8}, 0x20000FFC},   // lmw r30,0(r3) across a page; addi; loop
    };
    for (const Program& program : programs) {
        pw::Region region;
        region.address = kCode;
        for (const std::uint32_t word : program.words)
            for (unsigned k = 0; k < 4; ++k) region.data.push_back(static_cast<std::uint8_t>(word >> (24 - 8 * k)));
        const std::array<pw::Region, 1> regions = {region};
        const std::array<std::uint64_t, 2> args = {program.r3, 0x11223344};
        pw::RunOptions options;
        options.max_steps = 20000;
        const pw::RunResult own = pw::run(regions, kCode, args, options), emulator = engine::pcode_cpu::run(regions, kCode, args, options);
        CB_CHECK(own.stopped == pw::StopReason::Memory);
        CB_CHECK_EQ(ppc_diff(own, emulator), std::string{});
    }
});

namespace {

// Where a function's first bytes are in the file: found by their bytes (a section the file holds as it loads).
std::size_t ppc_file_offset(const std::filesystem::path& path, std::uint32_t address) {
    const std::optional<formats::ppc_program::Program> program = formats::ppc_program::read(path);
    if (!program) return SIZE_MAX;
    const Bytes file = fs::read_file(path);
    for (const auto& piece : program->loaded()) {
        if (address < piece.address || address - piece.address + 16 > piece.bytes.size()) continue;
        const auto from = piece.bytes.begin() + (address - piece.address);
        const auto found = std::search(file.begin(), file.end(), from, from + 16);
        return found == file.end() ? SIZE_MAX : static_cast<std::size_t>(found - file.begin());
    }
    return SIZE_MAX;
}

}  // namespace

CB_TEST("engine.pcode_cpu.the_gamecube_lane_runs_homebrew_the_same_on_both_cores", [] { engine::pcode_compare::compare_homebrew("gamecube", "gamecube", ".dol", ppc_file_offset); });

CB_TEST("engine.pcode_cpu.the_wii_lane_runs_homebrew_the_same_on_both_cores", [] { engine::pcode_compare::compare_homebrew("wii", "wii", ".dol", ppc_file_offset); });

CB_TEST("engine.pcode_cpu.the_wiiu_lane_runs_homebrew_the_same_on_both_cores", [] { engine::pcode_compare::compare_homebrew("wiiu", "wiiu", ".rpx", ppc_file_offset); });
