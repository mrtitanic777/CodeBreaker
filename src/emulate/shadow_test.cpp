// emulate/shadow_test.cpp -- the PowerPC run's symbolic shadow, on real runs of the interpreter with the decompiler's own p-code (the
// real pcodecheck, when this machine has it built: the tests that need it return quietly when it is not there).
//
// What is checked is the meaning of what the shadow made, not its shape: each condition of a path is evaluated for chosen values of
// its inputs (by the small evaluator below, independent of the shadow's code) and must hold exactly when the interpreter takes that
// way. Programs are encoded by hand from the Power ISA.
//
// Notes:
//  - the constructor takes no `span`.
//  - a SUBPIECE that takes nothing of its input is a constant 0.
//  - a user operation, an instruction without a decoding or an operation without a symbolic meaning say so in `lost` in their own words
//; only whether `lost` is empty is read by the explorer.
#include "emulate/shadow.hpp"

#include <cstdlib>
#include <filesystem>
#include <map>
#include <stdexcept>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"

using namespace cb;
using namespace cb::emulate;
namespace sfs = std::filesystem;
namespace ns = cb::emulate::native_shadow;
namespace sym = cb::emulate::symbolic;

namespace {

sfs::path sh_find_exe() {
    if (auto env = cb::fs::get_env("CB_PCODECHECK"); env && !env->empty()) return *env;
    // (the one build puts pcodecheck.exe beside the test program)
    const sfs::path beside = cb::fs::executable_path().parent_path() / "pcodecheck.exe";
    return sfs::exists(beside) ? beside : sfs::path();
}

// The ppc-be-64 lifter, or null when this machine has none.
pcode::Lifter* sh_lifter() {
    static pcode::LifterSet set(pcode::PcodeConfig{sh_find_exe(), sfs::path(CB_SOURCE_DIR) / "decompile" / "engine" / "languages"});
    static const bool have = !sh_find_exe().empty();
    return have ? set.get(pcode::Mode::PpcBe64) : nullptr;
}

pcode::Lifter* sh_x86_lifter() {
    static pcode::LifterSet set(pcode::PcodeConfig{sh_find_exe(), sfs::path(CB_SOURCE_DIR) / "decompile" / "engine" / "languages"});
    static const bool have = !sh_find_exe().empty();
    return have ? set.get(pcode::Mode::X86Flat32) : nullptr;
}

// ---- the programs ----------------------------------------------------------------------------------------------------

std::uint32_t sh_d(unsigned op, unsigned rt, unsigned ra, std::int32_t imm) {
    return op << 26 | rt << 21 | ra << 16 | (static_cast<std::uint32_t>(imm) & 0xFFFF);
}
std::uint32_t sh_x(unsigned op, unsigned rt, unsigned ra, unsigned rb, unsigned xo) { return op << 26 | rt << 21 | ra << 16 | rb << 11 | xo << 1; }
std::uint32_t sh_b(std::int32_t li, bool lk = false) { return 18u << 26 | (static_cast<std::uint32_t>(li) & 0x03FFFFFC) | (lk ? 1u : 0u); }
std::uint32_t sh_bc(unsigned bo, unsigned bi, std::int32_t bd) { return 16u << 26 | bo << 21 | bi << 16 | (static_cast<std::uint32_t>(bd) & 0xFFFC); }
constexpr std::uint32_t kBlr = 0x4E800020;

powerpc::Region sh_code(std::uint32_t at, std::initializer_list<std::uint32_t> words) {
    powerpc::Region r;
    r.address = at;
    for (std::uint32_t w : words) {
        for (int s = 24; s >= 0; s -= 8) r.data.push_back(static_cast<std::uint8_t>(w >> s));
    }
    return r;
}

constexpr std::uint32_t kBase = 0x10000000;

// if (r3 < r4) return r3 + 1; else return r4;         (unsigned)
//   0 cmplw cr0,r3,r4   4 blt 0x10   8 mr r3,r4   C blr   10 addi r3,r3,1   14 blr
const std::initializer_list<std::uint32_t> kBelow = {sh_x(31, 0, 3, 4, 32), sh_bc(12, 0, 12), sh_x(31, 4, 3, 4, 444), kBlr, sh_d(14, 3, 3, 1), kBlr};

// ---- an evaluator of conditions, independent of the shadow ---------------------------------------------------------------

std::uint64_t sh_mask(std::uint32_t bits) { return bits >= 64 ? ~0ull : (1ull << bits) - 1; }

std::int64_t sh_signed(std::uint64_t v, std::uint32_t bits) {
    if (bits >= 64) return static_cast<std::int64_t>(v);
    return (v >> (bits - 1)) & 1 ? static_cast<std::int64_t>(v | ~sh_mask(bits)) : static_cast<std::int64_t>(v);
}

std::uint64_t sh_eval(const sym::Expr* e, const std::map<std::string, std::uint64_t>& env) {
    auto arg = [&](int i) { return sh_eval(e->arg[i], env); };
    const std::uint64_t m = sh_mask(e->bits);
    switch (e->op) {
        case sym::Op::Var: return env.at(std::string(e->name)) & m;
        case sym::Op::Const: return e->value & m;
        case sym::Op::Add: return (arg(0) + arg(1)) & m;
        case sym::Op::Sub: return (arg(0) - arg(1)) & m;
        case sym::Op::And: return arg(0) & arg(1);
        case sym::Op::Or: return arg(0) | arg(1);
        case sym::Op::Xor: return arg(0) ^ arg(1);
        case sym::Op::Mul: return (arg(0) * arg(1)) & m;
        case sym::Op::Not: return ~arg(0) & m;
        case sym::Op::Neg: return (0 - arg(0)) & m;
        case sym::Op::Shl: return arg(1) >= e->bits ? 0 : (arg(0) << arg(1)) & m;
        case sym::Op::LShr: return arg(1) >= e->bits ? 0 : arg(0) >> arg(1);
        case sym::Op::ZExt: return arg(0);
        case sym::Op::SExt: return static_cast<std::uint64_t>(sh_signed(arg(0), e->arg[0]->bits)) & m;
        case sym::Op::Extract: return (arg(0) >> e->lo) & m;
        case sym::Op::Concat: return ((arg(0) << e->arg[1]->bits) | arg(1)) & m;
        case sym::Op::Ite: return arg(0) != 0 ? arg(1) : arg(2);
        case sym::Op::Bool: return arg(0) != 0 ? 1 : 0;
        case sym::Op::Eq: return arg(0) == arg(1);
        case sym::Op::ULt: return arg(0) < arg(1);
        case sym::Op::ULe: return arg(0) <= arg(1);
        case sym::Op::SLt: return sh_signed(arg(0), e->arg[0]->bits) < sh_signed(arg(1), e->arg[1]->bits);
        case sym::Op::SLe: return sh_signed(arg(0), e->arg[0]->bits) <= sh_signed(arg(1), e->arg[1]->bits);
        case sym::Op::BNot: return arg(0) == 0;
        case sym::Op::BAnd: return arg(0) != 0 && arg(1) != 0;
        case sym::Op::BOr: return arg(0) != 0 || arg(1) != 0;
        case sym::Op::BXor: return (arg(0) != 0) != (arg(1) != 0);
        case sym::Op::Nz: return arg(0) != 0;
        case sym::Op::True: return 1;
        default: throw std::runtime_error("the evaluator does not know this operation");
    }
}

// Run `words` at kBase with a shadow whose inputs are `places` (r3.. as arguments); the run and what the shadow made.
struct ShadowRun {
    powerpc::RunResult result;
    std::shared_ptr<const ns::Run> shadow;
};

ShadowRun sh_run_with(pcode::Lifter& lifter, std::initializer_list<std::uint32_t> words, std::vector<std::uint64_t> args, bool symbolic_args,
                      shadow::PowerPcShadow::InputFn is_input = [](std::uint64_t) { return false; }, std::vector<powerpc::Region> more = {},
                      const powerpc::ExternTable* externs = nullptr) {
    shadow::PowerPcShadow shadow(lifter, std::move(is_input));
    if (symbolic_args) {
        std::vector<ns::Place> places;
        for (std::size_t k = 0; k < args.size(); ++k) places.push_back(ns::Place{std::format("r{}", 3 + k), args[k], static_cast<std::int64_t>(k)});
        shadow.enter(places, 64);
    }
    std::vector<powerpc::Region> regions = {sh_code(kBase, words)};
    for (auto& r : more) regions.push_back(std::move(r));
    powerpc::RunOptions options;
    options.shadow = &shadow;
    options.externs = externs;
    ShadowRun out;
    out.result = powerpc::run(regions, kBase, args, options);
    out.shadow = shadow.shared_run();
    return out;
}

}  // namespace

CB_TEST("emulate.shadow.lift_mode_follows_the_processor_language", [] {
    CB_CHECK(shadow::lift_mode("x86:LE:16:Real Mode:default") == pcode::Mode::X86Real16);
    CB_CHECK(shadow::lift_mode("x86:LE:32:default:windows") == pcode::Mode::X86Flat32);
    CB_CHECK(shadow::lift_mode("PowerPC:BE:64:Xenon:homed") == pcode::Mode::PpcBe64);
    CB_CHECK(!shadow::lift_mode("x86:LE:64:default").has_value());
    CB_CHECK(!shadow::lift_mode("MIPS:BE:32").has_value());
    CB_CHECK(!shadow::lift_mode("").has_value());
    CB_CHECK_EQ(shadow::real_mode_base(0x1234), 0x12340u);
});

CB_TEST("emulate.shadow.a_compare_and_branch_is_a_condition_of_the_path", [] {
    pcode::Lifter* lifter = sh_lifter();
    if (lifter == nullptr) return;
    struct Case {
        std::uint64_t a, b;
        bool taken;
        std::uint64_t ret;
    };
    // r3 < r4 (unsigned): jump taken, returns r3 + 1; else returns r4
    const Case cases[] = {{2, 5, true, 3}, {7, 5, false, 5}, {5, 5, false, 5}, {0, 0xFFFFFFFFFFFFFFFF, true, 1}};
    for (const Case& c : cases) {
        const ShadowRun r = sh_run_with(*lifter, kBelow, {c.a, c.b}, true);
        CB_CHECK_EQ(r.result.ret, c.ret);
        const ns::Run& s = *r.shadow;
        CB_CHECK(!s.off);
        CB_CHECK(s.lost.empty());
        CB_CHECK_EQ(s.trail.size(), 1u);
        CB_CHECK_EQ(s.trail[0].site, std::uint64_t{kBase + 4});
        CB_CHECK_EQ(s.trail[0].taken, c.taken);
        CB_CHECK_EQ(s.path.size(), 1u);
        const ns::PathEntry& e = s.path[0];
        CB_CHECK(e.kind == ns::PathKind::Jump);
        CB_CHECK_EQ(e.site, std::uint64_t{kBase + 4});
        CB_CHECK_EQ(e.taken, c.taken);
        // the condition holds for the values this run had ...
        std::map<std::string, std::uint64_t> env = {{"a0", c.a}, {"a1", c.b}};
        CB_CHECK_EQ(sh_eval(e.condition, env), c.taken ? 1u : 0u);
        // ... and for no others than those that go the same way
        const std::pair<std::uint64_t, std::uint64_t> probes[] = {{1, 2}, {2, 1}, {3, 3}, {0, 0xFFFFFFFFFFFFFFFF}, {0xFFFFFFFFFFFFFFFF, 0}, {1ull << 63, 1}};
        for (const auto& [x, y] : probes) {
            env = {{"a0", x}, {"a1", y}};
            CB_CHECK_EQ(sh_eval(e.condition, env) != 0, static_cast<std::uint32_t>(x) < static_cast<std::uint32_t>(y));   // taken exactly when the low words compare so (cmplw), whichever way this run went
        }
        // the inputs, in the order they were made
        CB_CHECK_EQ(s.vars.size(), 2u);
        const ns::Var& a0 = s.vars.at("a0");
        CB_CHECK(a0.kind == ns::VarKind::Arg);
        CB_CHECK_EQ(a0.bits, 64u);
        CB_CHECK_EQ(a0.value, c.a);
        CB_CHECK_EQ(std::get<std::uint64_t>(a0.where), 0u);
        CB_CHECK_EQ(s.vars.at("a1").value, c.b);
        CB_CHECK_EQ(s.pins, 0);
        CB_CHECK(s.steps >= 3);
    }
});

CB_TEST("emulate.shadow.constants_make_no_condition_but_the_trail_still_has_the_jump", [] {
    pcode::Lifter* lifter = sh_lifter();
    if (lifter == nullptr) return;
    const ShadowRun r = sh_run_with(*lifter, kBelow, {2, 5}, false);   // nothing is an input
    CB_CHECK_EQ(r.result.ret, 3u);
    const ns::Run& s = *r.shadow;
    CB_CHECK(s.path.empty());
    CB_CHECK(s.vars.empty());
    CB_CHECK_EQ(s.trail.size(), 1u);
    CB_CHECK(s.trail[0].taken);
    CB_CHECK(s.lost.empty());
});

CB_TEST("emulate.shadow.memory_bytes_are_inputs_until_the_run_writes_them", [] {
    pcode::Lifter* lifter = sh_lifter();
    if (lifter == nullptr) return;
    constexpr std::uint32_t kData = powerpc::kArgsBase;
    // r4 = *(u32*)r3;  if (r4 == 10) return 1; else return 0
    //   0 lwz r4,0(r3)   4 cmplwi cr0,r4,10   8 beq 0x14   C li r3,0   10 blr   14 li r3,1   18 blr
    const std::initializer_list<std::uint32_t> load_and_test = {sh_d(32, 4, 3, 0), sh_d(10, 0, 4, 10), sh_bc(12, 2, 12), sh_d(14, 3, 0, 0), kBlr,
                                                                  sh_d(14, 3, 0, 1), kBlr};
    auto is_input = [](std::uint64_t a) { return a >= kData && a < kData + 0x100; };
    for (std::uint8_t last : {std::uint8_t{10}, std::uint8_t{11}}) {
        powerpc::Region data;
        data.address = kData;
        data.data = {0, 0, 0, last};
        const ShadowRun r = sh_run_with(*lifter, load_and_test, {kData}, false, is_input, {data});
        CB_CHECK_EQ(r.result.ret, last == 10 ? 1u : 0u);
        const ns::Run& s = *r.shadow;
        CB_CHECK(s.lost.empty());
        CB_CHECK_EQ(s.vars.size(), 4u);   // the four bytes of the word, named by their addresses
        std::map<std::string, std::uint64_t> env;
        for (std::uint32_t k = 0; k < 4; ++k) {
            const std::string name = std::format("m{:x}", kData + k);
            const ns::Var& v = s.vars.at(name);
            CB_CHECK(v.kind == ns::VarKind::Mem);
            CB_CHECK_EQ(v.bits, 8u);
            CB_CHECK_EQ(std::get<std::uint64_t>(v.where), std::uint64_t{kData + k});
            CB_CHECK_EQ(v.value, k == 3 ? last : 0u);
            env[name] = v.value;
        }
        CB_CHECK_EQ(s.path.size(), 1u);
        CB_CHECK_EQ(s.path[0].taken, last == 10);
        CB_CHECK_EQ(sh_eval(s.path[0].condition, env), last == 10 ? 1u : 0u);
        // the big-endian word is compared whole: 0x0000000a is the one value that takes the jump
        env[std::format("m{:x}", kData + 3)] = 10;
        CB_CHECK_EQ(sh_eval(s.path[0].condition, env), 1u);
        env[std::format("m{:x}", kData + 2)] = 1;   // 0x0000010a
        CB_CHECK_EQ(sh_eval(s.path[0].condition, env), 0u);
    }
});

CB_TEST("emulate.shadow.a_byte_the_run_wrote_is_no_longer_an_input", [] {
    pcode::Lifter* lifter = sh_lifter();
    if (lifter == nullptr) return;
    constexpr std::uint32_t kData = powerpc::kArgsBase;
    // *(u32*)r3 = 10 first; then the same load and test as above: every byte was written by the run, so none is an input
    //   0 li r5,10  4 stw r5,0(r3)  8 lwz r4,0(r3)  C cmplwi cr0,r4,10  10 beq 0x1c  14 li r3,0  18 blr  1C li r3,1  20 blr
    const std::initializer_list<std::uint32_t> write_then_read = {sh_d(14, 5, 0, 10), sh_d(36, 5, 3, 0), sh_d(32, 4, 3, 0), sh_d(10, 0, 4, 10),
                                                                    sh_bc(12, 2, 12), sh_d(14, 3, 0, 0), kBlr, sh_d(14, 3, 0, 1), kBlr};
    const ShadowRun r = sh_run_with(*lifter, write_then_read, {kData}, false, [](std::uint64_t a) { return a >= kData && a < kData + 0x100; });
    CB_CHECK_EQ(r.result.ret, 1u);
    CB_CHECK(r.shadow->vars.empty());
    CB_CHECK(r.shadow->path.empty());
    CB_CHECK_EQ(r.shadow->trail.size(), 1u);
});

CB_TEST("emulate.shadow.a_pointer_made_from_an_input_is_held_at_its_value", [] {
    pcode::Lifter* lifter = sh_lifter();
    if (lifter == nullptr) return;
    constexpr std::uint32_t kData = powerpc::kArgsBase;
    // r3 is an argument and the address the next instruction loads from: lwz r3,0(r3); blr
    powerpc::Region data;
    data.address = kData;
    data.data = {0, 0, 0, 7};
    const ShadowRun r = sh_run_with(*lifter, {sh_d(32, 3, 3, 0), kBlr}, {kData}, true, [](std::uint64_t a) { return a >= kData && a < kData + 0x100; }, {data});
    CB_CHECK_EQ(r.result.ret, 7u);
    const ns::Run& s = *r.shadow;
    CB_CHECK_EQ(s.pins, 1);
    CB_CHECK_EQ(s.path.size(), 1u);   // the pointer held at its value (the return through lr is not made from an input: no condition)
    const ns::PathEntry& pin = s.path[0];
    CB_CHECK(pin.kind == ns::PathKind::Pin);
    CB_CHECK(pin.taken);
    CB_CHECK_EQ(pin.site, std::uint64_t{kBase});
    CB_CHECK_EQ(sh_eval(pin.condition, {{"a0", kData}}), 1u);
    CB_CHECK_EQ(sh_eval(pin.condition, {{"a0", kData + 4}}), 0u);
    CB_CHECK(s.vars.contains("a0"));
    CB_CHECK(s.vars.contains(std::format("m{:x}", kData + 3)));
});

CB_TEST("emulate.shadow.a_call_out_returns_a_new_input", [] {
    pcode::Lifter* lifter = sh_lifter();
    if (lifter == nullptr) return;
    constexpr std::uint32_t kExtern = kBase + 0x1000;
    // 0 mflr r0   4 bl kExtern   8 mtlr r0   C cmpwi cr0,r3,0   10 beq 0x18   14 li r3,5   18 blr
    const std::initializer_list<std::uint32_t> after_the_call = {sh_x(31, 0, 8, 0, 339), sh_b(0x1000 - 4, true), sh_x(31, 0, 8, 0, 467),
                                                                   sh_d(11, 0, 3, 0), sh_bc(12, 2, 8), sh_d(14, 3, 0, 5), kBlr};
    powerpc::ExternTable externs = {{kExtern, "ext"}};
    for (std::uint64_t returned : {std::uint64_t{0}, std::uint64_t{9}}) {
        shadow::PowerPcShadow shadow(*lifter, [](std::uint64_t) { return false; });
        powerpc::RunOptions options;
        options.shadow = &shadow;
        options.externs = &externs;
        options.stub_ret = returned;
        const powerpc::Region r = sh_code(kBase, after_the_call);
        const powerpc::RunResult result = powerpc::run(std::span<const powerpc::Region>(&r, 1), kBase, {}, options);
        CB_CHECK_EQ(result.calls.size(), 1u);
        CB_CHECK_EQ(result.ret, returned == 0 ? 0u : 5u);
        const ns::Run& s = shadow.run();
        CB_CHECK(s.lost.empty());
        const ns::Var& ret = s.vars.at("ret");
        CB_CHECK(ret.kind == ns::VarKind::Ret);
        CB_CHECK(std::holds_alternative<std::monostate>(ret.where));
        CB_CHECK_EQ(ret.bits, 64u);
        CB_CHECK_EQ(ret.value, returned);
        CB_CHECK_EQ(s.path.size(), 1u);   // beq on what the call returned
        CB_CHECK_EQ(s.path[0].taken, returned == 0);
        CB_CHECK_EQ(sh_eval(s.path[0].condition, {{"ret", returned}}), returned == 0 ? 1u : 0u);
        CB_CHECK_EQ(sh_eval(s.path[0].condition, {{"ret", 0}}), 1u);
        CB_CHECK_EQ(sh_eval(s.path[0].condition, {{"ret", 1}}), 0u);
        CB_CHECK_EQ(sh_eval(s.path[0].condition, {{"ret", 1ull << 40}}), 1u);   // cmpwi compares the low word only
        CB_CHECK_EQ(sh_eval(s.path[0].condition, {{"ret", (1ull << 32) | 1}}), 0u);
    }
});

CB_TEST("emulate.shadow.an_instruction_that_does_not_decode_is_a_loss_not_an_error", [] {
    pcode::Lifter* lifter = sh_lifter();
    if (lifter == nullptr) return;
    // primary opcode 1 is no instruction of the Xenon's: the interpreter stops the run on it, the shadow only says it lost the thread
    const ShadowRun r = sh_run_with(*lifter, {0x04000000u, kBlr}, {1}, true);
    CB_CHECK(r.result.exit == powerpc::Exit::Exception);
    CB_CHECK(!r.shadow->lost.empty());
    CB_CHECK(!r.shadow->off);   // (an undecoded instruction forgets the registers; the shadow goes on)
});

CB_TEST("emulate.shadow.bounds_and_wrong_use", [] {
    pcode::Lifter* ppc = sh_lifter();
    if (ppc == nullptr) return;
    // a register the lifter does not have
    {
        shadow::PowerPcShadow shadow(*ppc, [](std::uint64_t) { return false; });
        const ns::Place bad[] = {{std::string("r99"), 1, std::nullopt}};
        CB_CHECK_THROWS_AS(UsageError, shadow.enter(bad, 64));
    }
    // a lifter of another mode
    if (pcode::Lifter* x86 = sh_x86_lifter()) CB_CHECK_THROWS_AS(UsageError, (void)shadow::PowerPcShadow(*x86, [](std::uint64_t) { return false; }));
    // forgetting an input register, and memory the environment wrote
    {
        shadow::PowerPcShadow shadow(*ppc, [](std::uint64_t) { return false; });
        const ns::Place places[] = {{std::string("r3"), 4, std::int64_t{0}}, {std::string("r4"), 9, std::string("flag")}, {std::uint64_t{0x1000}, 7, std::int64_t{2}}};
        shadow.enter(places, 32);
        CB_CHECK_EQ(shadow.run().vars.size(), 3u);
        CB_CHECK(shadow.run().vars.at("flag").kind == ns::VarKind::Reg);
        CB_CHECK_EQ(std::get<std::string>(shadow.run().vars.at("flag").where), "flag");
        CB_CHECK_EQ(shadow.run().vars.at("a0").bits, 32u);
        const std::string gone[] = {"r3", "no-such-register"};
        shadow.forget(gone);
        shadow.written(0x1000, 4);
    }
});

// The conditions kept per run are bounded: a loop over an input goes on being followed until the cap, then the shadow goes off.
CB_TEST("emulate.shadow.the_path_is_capped", [] {
    pcode::Lifter* lifter = sh_lifter();
    if (lifter == nullptr) return;
    // r3 counts down from an input: 0 addi r3,r3,-1  4 cmpwi cr0,r3,0  8 bne 0 (back)  C blr
    const std::initializer_list<std::uint32_t> countdown = {sh_d(14, 3, 3, -1), sh_d(11, 0, 3, 0), sh_bc(4, 2, -8), kBlr};
    const ShadowRun r = sh_run_with(*lifter, countdown, {1000}, true);
    const ns::Run& s = *r.shadow;
    CB_CHECK(s.off);
    CB_CHECK(s.lost.contains("path-capped"));
    CB_CHECK(s.path.size() >= shadow::kPathCap);
    CB_CHECK(s.path.size() <= shadow::kPathCap + 2);
    CB_CHECK(r.result.exit == powerpc::Exit::Ret || r.result.truncated);   // the run itself went on as it would have
});
