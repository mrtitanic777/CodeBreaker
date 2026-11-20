// Tests of cb/emulate/native_shadow.hpp: the recorder's tables read back as a Run (on tables written out by hand, whose meaning is checked
// word by word against what the recorder writes: tracer/cbtrace.h), and a shadow of a real run -- machine code on the prebuilt Unicorn,
// lifted by the built pcodecheck (they say so and pass when this machine has not built it).
//
// Notes:
//  - the recorder is linked in (cb/emulate/tracer), not found as a DLL, so there is no enabled()/why_not()/require(): a shadow is
//    always there to make;
//  - the conditions are nodes of an ExprPool the Run owns (a shared_ptr<const Run> is held for as long as they are used), not shared
//    expression trees;
//  - tables that lie (an operation, kind or string that is not there, a node made of one after it, a constant wider than 64 bits) are a
//    FormatError;
//  - the place and the name of an input are typed (`Place`).
#include "cb/emulate/native_shadow.hpp"

#include <filesystem>
#include <iostream>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/emulate/pcode.hpp"
#include "cb/emulate/unicorn_uc.hpp"

using namespace cb;
using namespace cb::emulate;
using namespace cb::emulate::native_shadow;
namespace sfs = std::filesystem;

namespace {

constexpr std::uint64_t kNone = ~std::uint64_t{0};
using Words = std::vector<std::uint64_t>;

Words node(symbolic::Op op, std::uint64_t bits, std::uint64_t k0 = kNone, std::uint64_t k1 = kNone, std::uint64_t k2 = kNone, std::uint64_t lo = 0,
           std::uint64_t value_low = 0, std::uint64_t value_high = 0) {
    return {static_cast<std::uint64_t>(op), bits, k0, k1, k2, lo, value_low, value_high};
}
Words join(std::initializer_list<Words> parts) {
    Words out;
    for (const Words& p : parts) out.insert(out.end(), p.begin(), p.end());
    return out;
}

RawRun raw_of(const Words& nodes, const Words& path, const Words& trail, const Words& vars, const Words& lost, std::string_view strings,
              std::int64_t string_count) {
    RawRun r;
    r.nodes = nodes;
    r.path = path;
    r.trail = trail;
    r.vars = vars;
    r.lost = lost;
    r.strings = strings;
    r.string_count = string_count;
    return r;
}

}  // namespace

CB_TEST("emulate.native_shadow.decode_run_reads_the_tables", [] {
    using symbolic::Op;
    // node 0: x (32 bits, the name is string 0); node 1: 5; node 2: x == 5; node 3: not that
    const Words nodes = join({node(Op::Var, 32, kNone, kNone, kNone, 0, 0), node(Op::Const, 32, kNone, kNone, kNone, 0, 5),
                              node(Op::Eq, 0, 0, 1), node(Op::BNot, 0, 2)});
    const Words path = {0x401003, 1, 2, 0,    // a jump at 401003, taken, on node 2
                        0x401009, 0, 3, 2};   // a pin at 401009, not taken, on node 3
    const Words trail = {0x401003, 1, 0x401020, 0};
    // the input `a0` (an argument: where tag 1, its number), the register `EAX` (tag 2, string 2), what a call returned (no place)
    const Words vars = {1, 0, 1, 0, 32, 7, 0,    // a0
                        2, 1, 2, 2, 32, 9, 0,    // EAX in EAX
                        3, 3, 0, 0, 32, 4, 0};   // ret0
    const Words lost = {4, 4};
    const std::string strings = std::string("x") + '\0' + "a0" + '\0' + "EAX" + '\0' + "ret0" + '\0' + "why: a call was not modelled";
    // (the table of strings has "x", "a0", "EAX", "ret0" -- the lost string is the fifth)
    const RawRun raw = raw_of(nodes, path, trail, vars, lost, std::string_view(strings.data(), strings.size()), 5);
    const auto run = decode_run(raw);
    CB_CHECK_EQ(run->path.size(), std::size_t(2));
    CB_CHECK_EQ(run->path[0].site, 0x401003u);
    CB_CHECK(run->path[0].taken);
    CB_CHECK(run->path[0].kind == PathKind::Jump);
    CB_CHECK(run->path[0].condition->op == Op::Eq);
    CB_CHECK_EQ(run->path[0].condition->bits, 0u);
    CB_CHECK(run->path[0].condition->arg[0]->op == Op::Var);
    CB_CHECK_EQ(std::string(run->path[0].condition->arg[0]->name), "x");
    CB_CHECK_EQ(run->path[0].condition->arg[0]->bits, 32u);
    CB_CHECK_EQ(run->path[0].condition->arg[1]->value, 5u);
    CB_CHECK(!run->path[1].taken);
    CB_CHECK(run->path[1].kind == PathKind::Pin);
    CB_CHECK(run->path[1].condition->op == Op::BNot || run->path[1].condition->op == Op::Eq);   // (the pool may fold a double negation)
    CB_CHECK_EQ(run->trail.size(), std::size_t(2));
    CB_CHECK(run->trail[0].taken && !run->trail[1].taken);
    CB_CHECK_EQ(run->trail[1].site, 0x401020u);
    CB_CHECK_EQ(run->vars.size(), std::size_t(3));
    CB_CHECK(run->vars.at("a0").kind == VarKind::Arg);
    CB_CHECK_EQ(std::get<std::uint64_t>(run->vars.at("a0").where), 0u);
    CB_CHECK_EQ(run->vars.at("a0").value, 7u);
    CB_CHECK(run->vars.at("EAX").kind == VarKind::Reg);
    CB_CHECK_EQ(std::get<std::string>(run->vars.at("EAX").where), "EAX");
    CB_CHECK(run->vars.at("ret0").kind == VarKind::Carry || run->vars.at("ret0").kind == VarKind::Ret);
    CB_CHECK(std::holds_alternative<std::monostate>(run->vars.at("ret0").where));
    CB_CHECK_EQ(run->lost.size(), std::size_t(1));   // (the same string twice: one reason)
    CB_CHECK(run->lost.contains("why: a call was not modelled"));
    // the vars keep the order the inputs were made in
    CB_CHECK_EQ(run->vars.begin()->first, "a0");
});

CB_TEST("emulate.native_shadow.decode_run_empty_and_flags", [] {
    const RawRun none = raw_of({}, {}, {}, {}, {}, "", 0);
    const auto run = decode_run(none);
    CB_CHECK(run->path.empty() && run->trail.empty() && run->vars.empty() && run->lost.empty());
    RawRun counted = none;
    counted.pins = 2;
    counted.steps = 99;
    counted.off = 1;
    const auto flagged = decode_run(counted);
    CB_CHECK_EQ(flagged->pins, 2);
    CB_CHECK_EQ(flagged->steps, 99);
    CB_CHECK(flagged->off);
    CB_CHECK(!run->off);
});

CB_TEST("emulate.native_shadow.decode_run_refuses_tables_that_lie", [] {
    using symbolic::Op;
    const Words var = node(Op::Var, 32, kNone, kNone, kNone, 0, 0);
    const Words five = node(Op::Const, 32, kNone, kNone, kNone, 0, 5);
    // an operation that is not one
    CB_CHECK_THROWS_AS(FormatError, decode_run(raw_of({999, 32, kNone, kNone, kNone, 0, 0, 0}, {}, {}, {}, {}, "x\0", 1)));
    // a table that is not whole records
    CB_CHECK_THROWS_AS(FormatError, decode_run(raw_of({1, 2, 3}, {}, {}, {}, {}, "", 0)));
    // a node made of one that is not before it
    CB_CHECK_THROWS_AS(FormatError, decode_run(raw_of(join({var, node(Op::Add, 32, 0, 5)}), {}, {}, {}, {}, std::string_view("x\0", 2), 1)));
    // a node that wants a child and has none
    CB_CHECK_THROWS_AS(FormatError, decode_run(raw_of(join({var, node(Op::Neg, 32)}), {}, {}, {}, {}, std::string_view("x\0", 2), 1)));
    // a name that is not among the strings
    CB_CHECK_THROWS_AS(FormatError, decode_run(raw_of(node(Op::Var, 32, kNone, kNone, kNone, 0, 3), {}, {}, {}, {}, std::string_view("x\0", 2), 1)));
    // a constant wider than 64 bits
    CB_CHECK_THROWS_AS(FormatError, decode_run(raw_of(node(Op::Const, 128, kNone, kNone, kNone, 0, 5, 1), {}, {}, {}, {}, "", 0)));
    // a path entry on a node that is not there, of a kind that is not one
    CB_CHECK_THROWS_AS(FormatError, decode_run(raw_of(join({var, five}), {1, 1, 7, 0}, {}, {}, {}, std::string_view("x\0", 2), 1)));
    CB_CHECK_THROWS_AS(FormatError, decode_run(raw_of(join({var, five}), {1, 1, 0, 3}, {}, {}, {}, std::string_view("x\0", 2), 1)));
    // an input of a kind that is not one; a lost reason that is not a string
    CB_CHECK_THROWS_AS(FormatError, decode_run(raw_of({}, {}, {}, {0, 9, 0, 0, 32, 1, 0}, {}, std::string_view("x\0", 2), 1)));
    CB_CHECK_THROWS_AS(FormatError, decode_run(raw_of({}, {}, {}, {}, {5}, std::string_view("x\0", 2), 1)));
});

// ---- a shadow of a real run ------------------------------------------------------------------------------------------------

namespace {

sfs::path find_pcodecheck() {
    if (auto env = fs::get_env("CB_PCODECHECK"); env && !env->empty()) return *env;
    // (the one build puts pcodecheck.exe beside the test program)
    const sfs::path beside = cb::fs::executable_path().parent_path() / "pcodecheck.exe";
    return sfs::exists(beside) ? beside : sfs::path();
}

Place register_input(const std::string& name, std::uint64_t value) {
    Place p;
    p.where = name;
    p.value = value;
    p.input = std::variant<std::int64_t, std::string>(name);
    return p;
}

// cmp eax, 5 ; jne +2 ; nop ; nop ; nop -- run with eax = `eax`, the shadow stepped before each instruction (a jump is told its way
// by the step after it). -> what the run made.
std::shared_ptr<const Run> follow(pcode::Lifter& lifter, std::uint64_t eax, std::uint64_t* final_address = nullptr) {
    constexpr std::uint64_t kBase = 0x401000;
    const Bytes code = {0x83, 0xF8, 0x05, 0x75, 0x02, 0x90, 0x90, 0x90};
    auto uc = unicorn_uc::open_x86(32);
    Shadow shadow(Setup::for_lifter(Kind::Flat32, lifter));
    uc->mem_map(kBase, 0x1000);
    uc->mem_write(kBase, code);
    uc->reg_write(*unicorn_uc::x86_register("EAX"), eax);
    const std::vector<Place> inputs = {register_input("EAX", eax)};
    shadow.enter(inputs, 32);
    std::uint64_t at = kBase;
    for (int i = 0; i < 8 && at < kBase + code.size(); ++i) {
        shadow.step(*uc, at);
        uc->emu_start(at, 0, 0, 1);
        at = uc->reg_read(*unicorn_uc::x86_register("EIP"));
    }
    if (final_address != nullptr) *final_address = at;
    return shadow.run();
}

}  // namespace

CB_TEST("emulate.native_shadow.follows_a_compare_and_jump", [] {
    const sfs::path exe = find_pcodecheck();
    const sfs::path languages = sfs::path(CB_SOURCE_DIR) / "cb" / "decompile" / "engine" / "languages";
    if (exe.empty() || !sfs::exists(languages / "x86.sla")) {
        std::cerr << "native_shadow tests: pcodecheck is not built here; the live run is skipped\n";
        return;
    }
    pcode::LifterSet lifters(pcode::PcodeConfig{exe, languages});
    pcode::Lifter* lifter = lifters.get(pcode::Mode::X86Flat32);
    if (lifter == nullptr) {
        std::cerr << "native_shadow tests: this pcodecheck does not lift; the live run is skipped\n";
        return;
    }
    // eax = 7: 7 != 5, the jump is taken and the first two nops are not run
    std::uint64_t end = 0;
    const auto taken = follow(*lifter, 7, &end);
    CB_CHECK_EQ(end, 0x401008u);
    CB_CHECK_EQ(taken->path.size(), std::size_t(1));
    if (taken->path.empty()) return;
    CB_CHECK(taken->path[0].kind == PathKind::Jump);
    CB_CHECK_EQ(taken->path[0].site, 0x401003u);
    CB_CHECK(taken->path[0].taken);
    CB_CHECK_EQ(taken->trail.size(), std::size_t(1));
    CB_CHECK(taken->trail[0].taken);
    symbolic::InputsMemo memo;
    const auto& over = symbolic::inputs(taken->path[0].condition, memo);
    CB_CHECK_EQ(over.size(), std::size_t(1));   // the condition is over the one input, EAX
    CB_CHECK(over.contains("EAX"));
    CB_CHECK(taken->vars.contains("EAX"));
    CB_CHECK(taken->vars.at("EAX").kind == VarKind::Reg);
    CB_CHECK_EQ(taken->vars.at("EAX").value, 7u);
    CB_CHECK(taken->steps >= 2);
    CB_CHECK(!taken->off);
    // eax = 5: equal, the jump is not taken and the run falls through to the nops
    const auto fell = follow(*lifter, 5, &end);
    CB_CHECK_EQ(end, 0x401008u);
    CB_CHECK_EQ(fell->path.size(), std::size_t(1));
    if (fell->path.empty() || fell->trail.empty()) return;
    CB_CHECK(!fell->path[0].taken);
    CB_CHECK(!fell->trail[0].taken);
});
