// The PowerPC lanes on the one emulator (engine/pcode_cpu_ppc.cpp): the same run as the interpreter on random programs, and each lane's homebrew judged the same with
// either core. Floating point runs on the interpreter in both, so no NaN is chosen by a host here.
#include <algorithm>
#include <array>
#include <bit>
#include <cstdio>
#include <format>
#include <limits>
#include <map>
#include <random>
#include <string>
#include <vector>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "emulate/powerpc.hpp"
#include "engine/pcode_cpu.hpp"
#include "engine/pcode_cpu_compare.hpp"
#include "engine/pcode_cpu_internal.hpp"
#include "engine/pcode_cpu_ppc.hpp"
#include "engine/pcode_ops_ppc.hpp"
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
    for (unsigned i = 0, shown = 0; i < 32 && shown < 3; ++i) {
        if (x.f[i] != y.f[i]) out += std::format("f{}[own {:016X} emu {:016X}] ", i, x.f[i], y.f[i]), ++shown;
        if (x.ps1[i] != y.ps1[i]) out += std::format("ps1_{}[own {:016X} emu {:016X}] ", i, x.ps1[i], y.ps1[i]), ++shown;
    }
    if (x.cr != y.cr) out += std::format("cr[{:08X}/{:08X}] ", x.cr, y.cr);
    if (x.fpscr != y.fpscr) out += std::format("fpscr[own {:08X} emu {:08X}] ", x.fpscr, y.fpscr);
    if (x.lr != y.lr) out += std::format("lr[{:X}/{:X}] ", x.lr, y.lr);
    if (x.ctr != y.ctr) out += std::format("ctr[{:X}/{:X}] ", x.ctr, y.ctr);
    if (x.ca != y.ca) out += "ca ";
    if (x.so != y.so || x.ov != y.ov) out += std::format("xer so/ov[{}{}/{}{}] ", x.so, x.ov, y.so, y.ov);
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

namespace {

namespace po = engine::pcode_ops_ppc;
using engine::pcode_cpu::emulator_named;

// Whether the description decodes the word (at address `at`).
bool decodes(decompile::sleigh::Emulator& emu, std::uint32_t at, std::uint32_t word) {
    const std::uint8_t bytes[4] = {static_cast<std::uint8_t>(word >> 24), static_cast<std::uint8_t>(word >> 16), static_cast<std::uint8_t>(word >> 8), static_cast<std::uint8_t>(word)};
    emu.write(at, bytes, 4);
    return emu.length(at) == 4;
}

// A valid encoding near `word`: the fields a form leaves reserved cleared (random reserved bits are no instruction), the opcode and extended opcode kept. 0: none found.
std::uint32_t valid_form(decompile::sleigh::Emulator& emu, std::mt19937& rng, std::uint32_t word) {
    constexpr std::uint32_t kAt = 0x10000000;
    for (unsigned record = 0; record < 2; ++record) {
        const std::uint32_t w = (word & ~1u) | record;
        if (decodes(emu, kAt, w)) return w;
        std::array<unsigned, 20> bits;
        for (unsigned i = 0; i < 20; ++i) bits[i] = 6 + i;   // the operand fields: bits 11-25 (rb, ra, rt) and 6-10 (frc, mb, ...); the extended opcode and opcode are kept
        for (unsigned attempt = 0; attempt < 6; ++attempt) {
            std::shuffle(bits.begin(), bits.end(), rng);
            std::uint32_t x = w;
            for (unsigned bit : bits) {
                if (!((x >> bit) & 1) || (bit < 11 && attempt < 3 && ((w >> 26) == 31 || (w >> 26) == 4 || (w >> 26) == 19))) continue;   // (bits 6-10 are part of an X-form's extended opcode)
                x &= ~(1u << bit);
                if (decodes(emu, kAt, x)) return x;
            }
        }
    }
    return 0;
}

// Whether the interpreter runs the word (it stops with `unsupported` for what it has no instruction for).
bool interpreter_runs(std::uint32_t word, bool paired) {
    pw::Machine m;
    m.paired_singles = paired;
    m.pc = 0x10000000;
    for (unsigned i = 0; i < 32; ++i) m.r[i] = 0x60000000 + i * 16;
    try {
        pw::step(m, word);
    } catch (const pw::Fault& f) {
        return f.reason() != pw::StopReason::Unsupported;
    }
    return true;
}

}  // namespace

// The words of every opcode and extended opcode in turn (each as a valid encoding the interpreter has an instruction for), each a few times with random registers and memory, as
// the first word of a random program; then short random programs of them. What only the description knows has no interpreter to compare with.
static void random_comparison(po::Flavor flavor, unsigned default_trials) {
    std::mt19937 rng(20261121);
    std::map<std::string, unsigned> by_kind;
    std::map<std::string, std::string> example;
    unsigned runs = 0, different = 0, skipped = 0;
    const unsigned trials = [default_trials] {
        const auto given = fs::get_env("CB_PCODE_EXHAUSTIVE");
        return given && !given->empty() && *given != "0" ? static_cast<unsigned>(std::strtoul(given->c_str(), nullptr, 10)) : default_trials;
    }();
    constexpr std::uint32_t kCode = 0x10000000;
    decompile::sleigh::Emulator& probe = emulator_named(flavor == po::Flavor::Xenon ? "ppc_64_xenon_be.sla" : "ppc_64_isa_altivec_be.sla");
    const auto make_word = [&](std::uint32_t key, bool paired) {   // a valid word of (opcode, extended opcode) = key; 0: none
        for (unsigned attempt = 0; attempt < 12; ++attempt) {
            std::uint32_t w = (static_cast<std::uint32_t>(rng()) & 0x03FFF800u) | ((key >> 10) << 26) | ((key & 0x3FF) << 1) | (rng() & 1);
            w = valid_form(probe, rng, w);
            if (w != 0 && w != 0x60000000 && !((w >> 26) == 31 && ((w >> 1) & 0x3FF) == 144 && ((w >> 20) & 1)) && interpreter_runs(w, paired)) return w;   // (mtocrf with other than one field is undefined)
        }
        return std::uint32_t{0};
    };
    struct Case {
        std::uint32_t key;
        bool paired;
    };
    std::vector<Case> cases;   // the (opcode, extended opcode, machine kind)s that have a valid word the interpreter runs
    for (std::uint32_t key = 0; key < 65536; ++key) {
        const unsigned op = key >> 10;
        const bool has_xo = op == 4 || op == 19 || op == 31 || op == 59 || op == 63 || op == 30 || op == 58 || op == 62;   // (in the others bits 1 to 10 are an immediate: one case)
        if (!has_xo && (key & 0x3FF) != 0) continue;
        for (unsigned paired = 0; paired < (flavor == po::Flavor::Xenon ? 1u : 2u); ++paired)
            if (engine::pcode_cpu::ppc_interpreter_handles(((key >> 10) << 26) | ((key & 0x3FF) << 1), paired != 0) && make_word(key, paired != 0) != 0) cases.push_back({key, paired != 0});
    }
    std::printf("pcode_cpu ppc: %zu (opcode, extended opcode, machine) cases\n", cases.size());
    const std::size_t total = cases.size() * trials;
    for (std::size_t n = 0; n < total + 3000; ++n) {
        const bool single = n < total;
        const bool paired = flavor == po::Flavor::Xenon ? false : single ? cases[n % cases.size()].paired : (rng() & 1) != 0;
        Bytes code(0x100);
        for (auto& b : code) b = static_cast<std::uint8_t>(rng());
        const auto fresh_word = [&]() {
            for (unsigned attempt = 0; attempt < 40; ++attempt) {
                const std::uint32_t w = make_word(cases[rng() % cases.size()].key, paired);
                if (w != 0) return w;
            }
            return std::uint32_t{0};
        };
        const std::uint32_t first = single ? make_word(cases[n % cases.size()].key, paired) : fresh_word();
        if (first == 0) {
            ++skipped;
            continue;
        }
        for (unsigned k = 0; k < 4; ++k) code[k] = static_cast<std::uint8_t>(first >> (24 - 8 * k));
        if (!single)
            for (unsigned i = 1; i < 12; ++i) {
                const std::uint32_t w = fresh_word();
                for (unsigned k = 0; k < 4; ++k) code[4 * i + k] = static_cast<std::uint8_t>(w >> (24 - 8 * k));
            }
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
        options.max_steps = single ? 1 : 12;
        options.paired_singles = paired;
        options.fill = pw::Fill::Counts;
        for (auto& g : options.gqr) g = static_cast<std::uint32_t>(rng());
        options.fargs = {1.5, -2.25, 1e10};
        pw::RunResult rr[2];
        for (int core = 0; core < 2; ++core) rr[core] = core == 1 ? engine::pcode_cpu::run(flavor, regions, kCode, args, options) : pw::run(regions, kCode, args, options);
        ++runs;
        const std::string fields = ppc_diff(rr[0], rr[1]);
        if (fields.empty()) continue;
        ++different;
        unsigned op = first >> 26;
        std::string where;
        std::uint32_t at_word = first;
        if (!single) {   // a program: the instruction after which the two cores differ is what is at fault
            for (unsigned k = 1; k <= options.max_steps; ++k) {
                pw::RunOptions o2 = options;
                o2.max_steps = k;
                const pw::RunResult ra = pw::run(regions, kCode, args, o2), rb = engine::pcode_cpu::run(flavor, regions, kCode, args, o2);
                if (ppc_diff(ra, rb).empty()) continue;
                pw::RunOptions o3 = options;
                o3.max_steps = k - 1;
                const std::uint32_t at = pw::run(regions, kCode, args, o3).pc;
                const std::size_t offset = at - kCode;
                if (offset + 4 <= code.size()) {
                    const std::uint32_t word = static_cast<std::uint32_t>(code[offset]) << 24 | code[offset + 1] << 16 | code[offset + 2] << 8 | code[offset + 3];
                    op = word >> 26;
                    at_word = word;
                    where = std::format("[step {} at {:X}: {:08X}] ", k, at, word);
                }
                break;
            }
        }
        const unsigned xo = (at_word >> 1) & 0x3FF;
        const std::string key = std::format("{}{}{}{}", single ? "1: op " : "N: op ", op, (op == 31 || op == 19 || op == 63 || op == 59 || op == 4) ? std::format(" xo {}", xo) : std::string(), paired ? " (paired)" : "");
        ++by_kind[key];
        if (!example.count(key)) example[key] = std::format("{:08X}: {}{}", at_word, where, fields);
    }
    std::printf("pcode_cpu ppc instructions: %u runs, %u differ, %u words not made\n", runs, different, skipped);
    unsigned shown = 0;
    for (const auto& [k, c] : by_kind)
        if (++shown <= 80) std::printf("    %u x %s   e.g. %s\n", c, k.c_str(), example[k].substr(0, 260).c_str());
    CB_CHECK_EQ(different, 0u);
}

CB_TEST("engine.pcode_cpu.ppc_instructions_run_as_the_interpreter_does", [] { random_comparison(po::Flavor::AltiVec, 40); });

// The Xbox 360's description runs the interpreter's instructions the same (VMX128's opcodes are no instruction the interpreter has).
CB_TEST("engine.pcode_cpu.ppc_the_xenon_description_runs_the_interpreters_instructions_the_same", [] { random_comparison(po::Flavor::Xenon, 6); });

// The words the adapter stops on, with `unsupported`, are the ones the interpreter has no instruction for (what only the description knows is not run).
CB_TEST("engine.pcode_cpu.ppc_the_instructions_the_adapter_stops_on_are_the_interpreters", [] {
    std::mt19937 rng(77);
    unsigned checked = 0, different = 0;
    std::map<std::string, unsigned> classes;
    for (unsigned paired = 0; paired < 2; ++paired)
        for (std::uint32_t key = 0; key < 65536; ++key)
            for (unsigned variant = 0; variant < 3; ++variant) {
                const std::uint32_t w = ((key >> 10) << 26) | ((key & 0x3FF) << 1) | (static_cast<std::uint32_t>(rng()) & 0x03FFF801u);
                ++checked;
                if (engine::pcode_cpu::ppc_interpreter_handles(w, paired != 0) == interpreter_runs(w, paired != 0)) continue;
                ++different;
                ++classes[std::format("op {} xo {} (paired {}): the interpreter {}", w >> 26, (w >> 1) & 0x3FF, paired, interpreter_runs(w, paired != 0) ? "runs it" : "stops")];
            }
    std::printf("pcode_cpu ppc stops: %u words, %u differ\n", checked, different);
    unsigned shown = 0;
    for (const auto& [text, count] : classes)
        if (++shown <= 40) std::printf("    %u x %s\n", count, text.c_str());
    CB_CHECK_EQ(different, 0u);
});

namespace {

// What a shadow is told, as text: the registers it is shown before each instruction and after each call out.
class Recorder final : public pw::Shadow {
public:
    std::vector<std::string> log;
    void step(pw::Machine& m, std::uint32_t pc) override {
        log.push_back(std::format("step {:X} r3={:X} r4={:X} r5={:X} r9={:X} f1={:X} cr={:X} lr={:X} ctr={:X} ca={}", pc, m.r[3], m.r[4], m.r[5], m.r[9], m.f[1], m.cr, m.lr, m.ctr, m.ca));
    }
    void after_call(pw::Machine& m, bool known, std::uint64_t ret) override { log.push_back(std::format("call {} {:X} r3={:X} pc={:X}", known, ret, m.r[3], m.pc)); }
};

std::string judge_diff(const pw::RunResult& a, const pw::RunResult& b, const Recorder& ra, const Recorder& rb) {
    std::string out = ppc_diff(a, b);
    if (a.calls.size() != b.calls.size()) out += std::format("calls[{}/{}] ", a.calls.size(), b.calls.size());
    else
        for (std::size_t i = 0; i < a.calls.size(); ++i)
            if (a.calls[i].name != b.calls[i].name || a.calls[i].args != b.calls[i].args) {
                out += std::format("call {} ", i);
                break;
            }
    if (a.path != b.path) out += std::format("path[{}/{}] ", a.path.size(), b.path.size());
    if (a.truncated != b.truncated) out += "truncated ";
    if (a.branches.has_value() != b.branches.has_value()) out += "branches-present ";
    else if (a.branches) {
        if (a.branches->size() != b.branches->size()) out += std::format("branches[{}/{}] ", a.branches->size(), b.branches->size());
        else
            for (std::size_t i = 0; i < a.branches->size(); ++i) {
                const pw::BranchEvent &x = (*a.branches)[i], &y = (*b.branches)[i];
                const bool same = x.address == y.address && x.taken == y.taken && x.compare.has_value() == y.compare.has_value() &&
                                  (!x.compare || (x.compare->size == y.compare->size && x.compare->a == y.compare->a && x.compare->b == y.compare->b && x.compare->a_from == y.compare->a_from &&
                                                  x.compare->b_from == y.compare->b_from));
                if (!same) {
                    out += std::format("branch {} at {:X} ", i, x.address);
                    break;
                }
            }
    }
    if (ra.log != rb.log) {
        std::size_t i = 0;
        while (i < ra.log.size() && i < rb.log.size() && ra.log[i] == rb.log[i]) ++i;
        out += std::format("shadow[{}/{}] first at {}: [{}] / [{}] ", ra.log.size(), rb.log.size(), i, i < ra.log.size() ? ra.log[i] : "-", i < rb.log.size() ? rb.log[i] : "-");
    }
    return out;
}

// Instruction words for the judge programs.
constexpr std::uint32_t d_form(unsigned op, unsigned rt, unsigned ra, std::uint32_t imm) { return (op << 26) | (rt << 21) | (ra << 16) | (imm & 0xFFFF); }
constexpr std::uint32_t branch_to(std::int32_t displacement, bool link) { return (18u << 26) | (static_cast<std::uint32_t>(displacement) & 0x03FFFFFC) | (link ? 1u : 0u); }
constexpr std::uint32_t branch_if(unsigned bo, unsigned bi, std::int32_t displacement) { return (16u << 26) | (bo << 21) | (bi << 16) | (static_cast<std::uint32_t>(displacement) & 0xFFFC); }

}  // namespace

// What the lane judge asks of a run -- calls out to named functions, the addresses that ran, the log of conditional branches with what each compared, calls into code that is not
// there, a shadow that is shown the registers before every instruction -- comes out the same on the emulator as in the interpreter.
CB_TEST("engine.pcode_cpu.ppc_the_judges_runs_match_the_interpreters", [] {
    std::mt19937 rng(424242);
    constexpr std::uint32_t kCode = 0x10000000, kExtern = 0x10000400, kAbsent = 0x10000800;
    unsigned runs = 0, different = 0, calls = 0, events = 0;
    for (unsigned n = 0; n < 600; ++n) {
        std::vector<std::uint32_t> words;
        const auto at = [&] { return static_cast<std::int32_t>(4 * words.size()); };
        const unsigned count = 24 + rng() % 24;
        while (words.size() < count) {
            const unsigned kind = rng() % 14;
            const unsigned ra = 3 + rng() % 6, rb = 3 + rng() % 6, field = rng() % 8, target = 5 + rng() % 4;   // (r3 and r4 stay what they were given: the bases of the loads and stores)
            switch (kind) {
                case 0: words.push_back(d_form(11, field << 2, ra, rng() % 9)); break;                                       // cmpwi
                case 1: words.push_back(d_form(10, field << 2, ra, rng() % 9)); break;                                       // cmplwi
                case 2: words.push_back((31u << 26) | ((field << 2) << 21) | (ra << 16) | (rb << 11) | (0u << 1)); break;   // cmpw
                case 3: words.push_back((31u << 26) | ((field << 2) << 21) | (ra << 16) | (rb << 11) | (32u << 1)); break;  // cmplw
                case 4: words.push_back(d_form(14, target, rb, rng() % 7)); break;                                                // addi
                case 5: words.push_back(d_form(32, target, 3, 4 * (rng() % 16))); break;                                         // lwz from r3's block
                case 6: words.push_back(d_form(34, target, 4, rng() % 32)); break;                                               // lbz
                case 7: words.push_back(d_form(36, rb, 3, 4 * (rng() % 16))); break;                                         // stw
                case 8: words.push_back(branch_if((rng() & 1) ? 12 : 4, field * 4 + rng() % 4, 4 * (1 + rng() % 4))); break;   // a conditional forward branch
                case 9: words.push_back(branch_to(static_cast<std::int32_t>(kExtern + 4 * (rng() % 3)) - static_cast<std::int32_t>(kCode) - at(), true)); break;   // bl to an extern
                case 10: words.push_back(branch_to(static_cast<std::int32_t>(kAbsent + 4 * (rng() % 2)) - static_cast<std::int32_t>(kCode) - at(), true)); break;   // bl to code not there
                case 11: words.push_back((31u << 26) | (ra << 21) | (9u << 16) | (467u << 1)); break;                       // mtctr
                case 12: words.push_back(branch_if(16, 0, 4 * (1 + rng() % 3))); break;                                      // bdnz forward
                default: words.push_back(0x7C0802A6 | (target << 21)); break;                                                     // mflr
            }
        }
        words.push_back(0x4E800020);   // blr
        pw::Region code;
        code.address = kCode;
        for (std::uint32_t w : words)
            for (unsigned k = 0; k < 4; ++k) code.data.push_back(static_cast<std::uint8_t>(w >> (24 - 8 * k)));
        pw::Region data;
        data.address = pw::kArgsBase;
        data.data = Bytes(0x1000);
        for (auto& b : data.data) b = static_cast<std::uint8_t>(rng() % 6);
        const std::array<pw::Region, 2> regions = {code, data};
        std::array<std::uint64_t, 8> args{};
        for (unsigned i = 0; i < 8; ++i) args[i] = i < 2 ? pw::kArgsBase + 16 * i : rng() % 6;
        pw::ExternTable externs;
        externs[kExtern] = "ext_a";
        externs[kExtern + 4] = "ext_b";
        externs[kExtern + 8] = "ext_c";
        pw::RunOptions options;
        options.externs = &externs;
        options.span = pw::AddressRange{kCode, kCode + 0x1000};
        options.branches = (n & 1) == 0;
        options.absent = pw::AddressRange{kAbsent, kAbsent + 0x10};
        options.stub_ret = rng() % 100;
        options.max_steps = 300;
        options.paired_singles = (n & 2) != 0;
        pw::RunResult rr[2];
        Recorder rec[2];
        for (int core = 0; core < 2; ++core) {
            options.shadow = (n % 3 == 0) ? &rec[core] : nullptr;
            rr[core] = core == 1 ? engine::pcode_cpu::run(regions, kCode, args, options) : pw::run(regions, kCode, args, options);
        }
        ++runs;
        calls += static_cast<unsigned>(rr[0].calls.size());
        events += rr[0].branches ? static_cast<unsigned>(rr[0].branches->size()) : 0;
        const std::string fields = judge_diff(rr[0], rr[1], rec[0], rec[1]);
        if (fields.empty()) continue;
        if (++different <= 6) std::printf("  program %u: %s\n", n, fields.substr(0, 400).c_str());
    }
    std::printf("pcode_cpu ppc judge runs: %u runs, %u calls out, %u logged branches, %u differ\n", runs, calls, events, different);
    CB_CHECK(calls > 100);
    CB_CHECK(events > 100);
    CB_CHECK_EQ(different, 0u);
});

// Words the description has no instruction for but the interpreter runs (it ignores the fields a form leaves reserved): the judge's edited copies have bytes turned over, which makes
// such words, and the verdict on the copy must not change with the core. Survey: what flipping each of the first three bytes of a valid word makes.
CB_TEST("engine.pcode_cpu.ppc_survey_of_words_only_the_interpreter_runs", [] {
    std::mt19937 rng(31337);
    decompile::sleigh::Emulator& probe = emulator_named("ppc_64_isa_altivec_be.sla");
    std::map<std::string, unsigned> classes;
    std::map<std::string, std::uint32_t> example;
    unsigned words = 0, only_interpreter = 0, disagreeing = 0;
    std::map<std::string, std::string> disagree_example;
    for (std::uint32_t key = 0; key < 65536; ++key) {
        const unsigned op = key >> 10;
        const bool has_xo = op == 4 || op == 19 || op == 31 || op == 59 || op == 63 || op == 30 || op == 58 || op == 62;
        if (!has_xo && (key & 0x3FF) != 0) continue;
        if (!engine::pcode_cpu::ppc_interpreter_handles(((key >> 10) << 26) | ((key & 0x3FF) << 1), false)) continue;
        for (unsigned n = 0; n < 6; ++n) {
            std::uint32_t w = (static_cast<std::uint32_t>(rng()) & 0x03FFF800u) | ((key >> 10) << 26) | ((key & 0x3FF) << 1) | (rng() & 1);
            w = valid_form(probe, rng, w);
            if (w == 0) continue;
            for (unsigned byte = 0; byte < 3; ++byte) {
                const std::uint32_t flipped = w ^ (0xFFu << (24 - 8 * byte));
                ++words;
                constexpr std::uint32_t kAt = 0x10000000;
                if (decodes(probe, kAt, flipped) || !interpreter_runs(flipped, false) || !engine::pcode_cpu::ppc_interpreter_handles(flipped, false)) continue;
                ++only_interpreter;
                {   // the word as the first instruction of a program, on both cores
                    Bytes code(0x40);
                    for (auto& b8 : code) b8 = static_cast<std::uint8_t>(rng());
                    for (unsigned k = 0; k < 4; ++k) code[k] = static_cast<std::uint8_t>(flipped >> (24 - 8 * k));
                    pw::Region region, data;
                    region.address = kAt;
                    region.data = code;
                    data.address = pw::kArgsBase;
                    data.data = Bytes(0x4000);
                    for (auto& b8 : data.data) b8 = static_cast<std::uint8_t>(rng());
                    const std::array<pw::Region, 2> regions = {region, data};
                    std::array<std::uint64_t, 8> args{};
                    for (auto& v : args) v = (rng() & 1) ? pw::kArgsBase + (rng() & 0x3FF8) : rng() & 0xFF;
                    pw::RunOptions options;
                    options.max_steps = 1;
                    options.fill = pw::Fill::Counts;
                    const std::string fields = ppc_diff(pw::run(regions, kAt, args, options), engine::pcode_cpu::run(regions, kAt, args, options));
                    if (!fields.empty()) {
                        ++disagreeing;
                        disagree_example.emplace(std::format("op {} xo {}", flipped >> 26, (flipped >> 1) & 0x3FF), std::format("{:08X}: {}", flipped, fields));
                    }
                }
                const std::string name = std::format("op {} xo {}", flipped >> 26, (flipped >> 26) == 31 || (flipped >> 26) == 19 || (flipped >> 26) == 63 || (flipped >> 26) == 59 ? ((flipped >> 1) & 0x3FF) : 0u);
                ++classes[name];
                example.emplace(name, flipped);
            }
        }
    }
    std::printf("pcode_cpu ppc: %u flipped words, %u that only the interpreter runs\n", words, only_interpreter);
    std::printf("pcode_cpu ppc: %u of them run differently on the emulator\n", disagreeing);
    for (const auto& [name, text] : disagree_example) std::printf("    %s   %s\n", name.c_str(), text.substr(0, 300).c_str());
    CB_CHECK_EQ(disagreeing, 0u);
});

// The arithmetic with OE set records overflow in XER[OV] and the summary XER[SO], and CR0 and the compares carry the summary: the edge values of every OE form, on both cores.
CB_TEST("engine.pcode_cpu.ppc_overflow_is_recorded_the_same_on_both_cores", [] {
    constexpr std::uint32_t kCode = 0x10000000;
    const std::uint64_t edges[] = {0, 1, 2, 0xFFFFFFFFFFFFFFFFull, 0x7FFFFFFFFFFFFFFFull, 0x8000000000000000ull, 0x8000000000000001ull, 0x7FFFFFFFull, 0x80000000ull, 0xFFFFFFFFull, 0xFFFFFFFF80000000ull, 0x100000000ull, 0x40000000ull};
    const unsigned xos[] = {266, 40, 10, 8, 138, 136, 202, 200, 234, 232, 104, 235, 233, 491, 459, 489, 457};
    unsigned runs = 0, different = 0, overflowed = 0;
    for (unsigned xo : xos)
        for (std::uint64_t a : edges)
            for (std::uint64_t b : edges)
                for (unsigned carry = 0; carry < 2; ++carry) {
                    // mtspr XER sets the carry first (r6 holds it), then the instruction, then mfspr XER and a compare to see the summary in CR1
                    std::vector<std::uint32_t> program = {
                        (31u << 26) | (6u << 21) | (1u << 16) | (0u << 11) | (467u << 1),                                                   // mtspr XER,r6 (spr 1: 0x0020 split)
                        (31u << 26) | (5u << 21) | (3u << 16) | (4u << 11) | 0x400 | (xo << 1) | 1,                                          // the OE form, with the record bit
                        (31u << 26) | (7u << 21) | (1u << 16) | (0u << 11) | (339u << 1),                                                    // mfspr r7,XER
                        (31u << 26) | (2u << 23) | (3u << 16) | (4u << 11) | (0u << 1),                                                      // cmpw cr2,r3,r4 (carries the summary)
                        0x4E800020,
                    };
                    program[0] = (31u << 26) | (6u << 21) | (1u << 16) | (467u << 1);   // the SPR field of XER (1) is split: low five bits in 16-20, high in 11-15
                    program[2] = (31u << 26) | (7u << 21) | (1u << 16) | (339u << 1);
                    pw::Region region;
                    region.address = kCode;
                    for (std::uint32_t w : program)
                        for (unsigned k = 0; k < 4; ++k) region.data.push_back(static_cast<std::uint8_t>(w >> (24 - 8 * k)));
                    const std::array<pw::Region, 1> regions = {region};
                    const std::array<std::uint64_t, 4> args = {a, b, 0, 0};   // r3, r4, r5, r6
                    std::array<std::uint64_t, 4> given = args;
                    given[3] = std::uint64_t{carry} << 29;
                    pw::RunOptions options;
                    options.max_steps = 20;
                    const pw::RunResult x = pw::run(regions, kCode, given, options), y = engine::pcode_cpu::run(regions, kCode, given, options);
                    ++runs;
                    overflowed += (x.machine->ov ? 1 : 0);
                    if (const std::string d = ppc_diff(x, y); !d.empty() && ++different <= 6) std::printf("  xo %u a=%llX b=%llX carry %u: %s\n", xo, static_cast<unsigned long long>(a), static_cast<unsigned long long>(b), carry, d.c_str());
                }
    std::printf("pcode_cpu ppc overflow: %u runs, %u that overflow, %u differ\n", runs, overflowed, different);
    CB_CHECK(overflowed > 200);
    CB_CHECK_EQ(different, 0u);
});


// The NaN of an invalid operation is the positive default one and a NaN operand comes out quieted (the manual's rule, not the host's), on both cores.
CB_TEST("engine.pcode_cpu.ppc_nan_results_follow_the_manual_on_both_cores", [] {
    constexpr std::uint32_t kCode = 0x10000000;
    const double inf = std::numeric_limits<double>::infinity();
    const double snan = pw::to_double(0x7FF0000000000001ull), neg_nan = pw::to_double(0xFFF8000000000123ull);
    struct Case {
        std::uint32_t word;   // f3 = op(f1, f2) (fmul: f1 * f2 in the C field)
        double a, b;
        std::uint64_t expect;
    };
    const Case cases[] = {
        {0xFC611028, inf, inf, 0x7FF8000000000000ull},        // fsub f3,f1,f2: inf - inf
        {0xFC6100B2, 0.0, inf, 0x7FF8000000000000ull},   // fmul f3,f1,f2(C): 0 * inf
        {0xFC61102A, snan, 1.0, 0x7FF8000000000001ull},       // fadd: a signaling NaN comes out quiet
        {0xFC61102A, 1.0, neg_nan, 0xFFF8000000000123ull},    // fadd: a negative quiet NaN keeps its sign
        {0xFC611024, 0.0, 0.0, 0x7FF8000000000000ull},        // fdiv: 0 / 0
    };
    for (const Case& c : cases) {
        pw::Region region;
        region.address = kCode;
        for (std::uint32_t w : {c.word, 0x4E800020u})
            for (unsigned k = 0; k < 4; ++k) region.data.push_back(static_cast<std::uint8_t>(w >> (24 - 8 * k)));
        const std::array<pw::Region, 1> regions = {region};
        pw::RunOptions options;
        options.fargs = {c.a, c.b};
        options.max_steps = 4;
        const pw::RunResult x = pw::run(regions, kCode, {}, options), y = engine::pcode_cpu::run(regions, kCode, {}, options);
        CB_CHECK_EQ(x.machine->f[3], c.expect);
        CB_CHECK_EQ(y.machine->f[3], c.expect);
    }
});


// The floating-point status register on both cores: programs that set FPSCR (rounding mode, enables, sticky bits chosen at random) through mtfsf, run one to three floating
// instructions on edge and random operands (scalar, or paired singles on the Gekko's machine), read FPSCR back with mffs, and must leave the same registers, condition register
// (CR1 of the record forms) and FPSCR on the interpreter and on the p-code emulator.
static void fpscr_comparison(po::Flavor flavor, bool paired, unsigned programs) {
    std::mt19937_64 rng(20261124 + (paired ? 1 : 0) + (flavor == po::Flavor::Xenon ? 2 : 0));
    constexpr std::uint32_t kCode = 0x10000000;
    const auto operand = [&]() -> std::uint64_t {
        static const std::uint64_t specials[] = {0, 1ull << 63, 0x3FF0000000000000ull, 0xBFF0000000000000ull, 0x4000000000000000ull, 0x7FF0000000000000ull, 0xFFF0000000000000ull, 0x7FF0000000000001ull,
                                                 0x7FF8000000000000ull, 0xFFF8000000000123ull, 0x7FEFFFFFFFFFFFFFull, 0x0010000000000000ull, 0x000FFFFFFFFFFFFFull, 0x0000000000000001ull,
                                                 0x3FE0000000000000ull, 0x4008000000000000ull, 0x3810000000000000ull, 0x47EFFFFFE0000000ull, 0x41E0000000000000ull, 0xC1E0000000000000ull, 0x43E0000000000000ull};
        std::uint64_t v;
        switch (rng() % 5) {
            case 0: v = specials[rng() % (sizeof specials / sizeof *specials)]; break;
            case 1: v = (rng() & 0x800FFFFFFFFFFFFFull) | (static_cast<std::uint64_t>(rng() % 2047) << 52); break;
            case 2: v = (rng() & 0x800FFFFFFFFFFFFFull) | (static_cast<std::uint64_t>(1015 + rng() % 16) << 52); break;
            case 3: v = std::bit_cast<std::uint64_t>(static_cast<double>(static_cast<std::int64_t>(rng() % 4001) - 2000) / 8.0); break;
            default: v = rng(); break;
        }
        return v;
    };
    const auto fpscr_value = [&]() -> std::uint32_t {
        std::uint32_t v = static_cast<std::uint32_t>(rng() & 3);                                // RN
        if (rng() % 3 == 0) v |= (static_cast<std::uint32_t>(rng()) & 0x1F) << 3;               // XE NI .. VE OE UE ZE XE
        if (rng() % 3 == 0) v |= static_cast<std::uint32_t>(rng()) & 0x0FF80000u;               // some sticky bits
        return v;
    };
    unsigned runs = 0, different = 0;
    unsigned long long executed = 0;
    for (unsigned n = 0; n < programs; ++n) {
        std::vector<std::uint32_t> words;
        std::array<double, 13> fargs{};
        for (unsigned i = 0; i < 5; ++i) fargs[i] = std::bit_cast<double>(operand());
        fargs[5] = std::bit_cast<double>(std::uint64_t{fpscr_value()});   // f6: the FPSCR value, in the low word
        Bytes memory(0x100);
        if (paired) {
            for (unsigned i = 0; i < 5; ++i) {   // psq_l f(i+1), 8*i(r3), 0, 0: both halves
                const float fa = static_cast<float>(std::bit_cast<double>(operand())), fb = static_cast<float>(std::bit_cast<double>(operand()));
                const std::uint32_t wa = std::bit_cast<std::uint32_t>(fa), wb = std::bit_cast<std::uint32_t>(fb);
                for (unsigned k = 0; k < 4; ++k) memory[8 * i + k] = static_cast<std::uint8_t>(wa >> (24 - 8 * k)), memory[8 * i + 4 + k] = static_cast<std::uint8_t>(wb >> (24 - 8 * k));
                words.push_back((56u << 26) | ((i + 1) << 21) | (3u << 16) | (8 * i));
            }
        }
        words.push_back((63u << 26) | (0xFFu << 17) | (6u << 11) | (711u << 1));   // mtfsf 0xFF, f6
        const unsigned count = 1 + rng() % 3;
        for (unsigned k = 0; k < count; ++k) {
            std::uint32_t w = 0;
            for (unsigned attempt = 0; attempt < 50; ++attempt) {
                const unsigned frt = 7 + rng() % 3, fra = 1 + rng() % 5, frb = 1 + rng() % 5, frc = 1 + rng() % 5;
                const unsigned rc = rng() & 1;
                if (paired) {
                    static const unsigned xo5s[] = {18, 20, 21, 23, 24, 25, 26, 28, 29, 30, 31, 10, 11, 12, 13, 14, 15};
                    if (rng() % 6 == 0) {   // ps_cmpu0 / ps_cmpo0 / ps_cmpu1 / ps_cmpo1 into a random field
                        static const unsigned compares[] = {0, 32, 64, 96};
                        w = (4u << 26) | ((rng() % 8) << 23) | (fra << 16) | (frb << 11) | (compares[rng() % 4] << 1);
                    } else {
                        w = (4u << 26) | (frt << 21) | (fra << 16) | (frb << 11) | (frc << 6) | (xo5s[rng() % (sizeof xo5s / sizeof *xo5s)] << 1) | rc;
                    }
                } else {
                    static const unsigned xo5s[] = {18, 20, 21, 22, 24, 25, 26, 28, 29, 30, 31};
                    static const unsigned xo10s[] = {12, 14, 15, 814, 815, 846, 0, 32, 583, 134, 70, 38, 64, 711, 23, 72, 40, 264, 136};
                    if (rng() % 3 != 0) {
                        const unsigned op = rng() & 1 ? 59 : 63;
                        w = (op << 26) | (frt << 21) | (fra << 16) | (frb << 11) | (frc << 6) | (xo5s[rng() % (sizeof xo5s / sizeof *xo5s)] << 1) | rc;
                    } else {
                        const unsigned xo = xo10s[rng() % (sizeof xo10s / sizeof *xo10s)];
                        const bool no_record = xo == 0 || xo == 32 || xo == 64;
                        w = (63u << 26) | (frt << 21) | (fra << 16) | (frb << 11) | (xo == 23 ? frc << 6 : 0u) | (xo << 1) | (no_record ? 0 : rc);
                        if (xo == 0 || xo == 32) w = (w & ~(7u << 23)) | ((rng() % 8) << 23);
                        if (xo == 134) w = (w & ~(0xFu << 12)) | ((rng() & 0xF) << 12) | ((rng() % 8) << 23);
                        if (xo == 70 || xo == 38) w = (w & ~(31u << 21)) | ((rng() % 32) << 21);
                        if (xo == 711) w = (w & ~(0xFFu << 17)) | ((rng() & 0xFF) << 17);
                        if (xo == 64) w = (w & ~((7u << 23) | (7u << 18))) | ((rng() % 8) << 23) | ((rng() % 8) << 18);
                    }
                }
                if (engine::pcode_cpu::ppc_interpreter_handles(w, paired)) break;
                w = 0;
            }
            if (w != 0) words.push_back(w);
        }
        words.push_back((63u << 26) | (11u << 21) | (583u << 1));   // mffs f11
        words.push_back(0x4E800020);
        pw::Region region;
        region.address = kCode;
        for (std::uint32_t w : words)
            for (unsigned k = 0; k < 4; ++k) region.data.push_back(static_cast<std::uint8_t>(w >> (24 - 8 * k)));
        pw::Region data;
        data.address = pw::kArgsBase;
        data.data = memory;
        const std::array<pw::Region, 2> regions = {region, data};
        const std::array<std::uint64_t, 1> args = {pw::kArgsBase};
        pw::RunOptions options;
        options.max_steps = 40;
        options.paired_singles = paired;
        options.fargs.assign(fargs.begin(), fargs.end());
        const pw::RunResult a = pw::run(regions, kCode, args, options), b = engine::pcode_cpu::run(flavor, regions, kCode, args, options);
        ++runs;
        executed += a.steps;
        if (const std::string d = ppc_diff(a, b); !d.empty() && ++different <= 8) {
            std::printf("  fpscr program %u:", n);
            for (std::uint32_t w : words) std::printf(" %08X", w);
            std::printf("\n    %s\n", d.c_str());
        }
    }
    std::printf("pcode_cpu ppc fpscr (%s%s): %u programs, %llu instructions, %u differ\n", paired ? "paired" : "scalar", flavor == po::Flavor::Xenon ? ", xenon" : "", runs, executed, different);
    CB_CHECK_EQ(different, 0u);
}

CB_TEST("engine.pcode_cpu.ppc_floating_point_status_is_the_same_on_both_cores", [] { fpscr_comparison(po::Flavor::AltiVec, false, 8000); });
CB_TEST("engine.pcode_cpu.ppc_paired_single_status_is_the_same_on_both_cores", [] { fpscr_comparison(po::Flavor::AltiVec, true, 5000); });
CB_TEST("engine.pcode_cpu.ppc_the_xenon_floating_point_status_is_the_same_on_both_cores", [] { fpscr_comparison(po::Flavor::Xenon, false, 1500); });

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

// The PlayStation 3's lane: its programs are encrypted or packed unless the sample says otherwise, so no function bytes can be found in the file (no edited copies are judged).
CB_TEST("engine.pcode_cpu.the_ps3_lane_runs_homebrew_the_same_on_both_cores", [] {
    engine::pcode_compare::compare_homebrew("ps3", "ps3", ".sprx", [](const std::filesystem::path&, std::uint32_t) { return SIZE_MAX; });
    engine::pcode_compare::compare_homebrew("ps3", "ps3", ".BIN", [](const std::filesystem::path&, std::uint32_t) { return SIZE_MAX; });
});
