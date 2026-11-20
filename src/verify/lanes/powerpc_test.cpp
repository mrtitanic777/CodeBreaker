// Tests of the PowerPC kind over functions written word by word (the interpreter runs them; nothing is compiled): the inputs, what a
// run shows of a function, and the verdict on a function and its rebuilt twin. Where a whole module is decompiled, compiled and
// linked is the product check's (it needs the Xbox 360 compiler).

#include "verify/lanes/powerpc.hpp"

#include "base/config.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"

namespace cb::verify::lanes {
namespace {

namespace epp = emulate::powerpc;
using Lane = PowerPcLane;
using Vector = Lane::Vector;

constexpr std::uint32_t kLi5 = 0x38600005;       // li r3, 5
constexpr std::uint32_t kBlr = 0x4E800020;
constexpr std::uint32_t kOriginal = 0x82000000;
constexpr std::uint32_t kRebuilt = kOriginal + Lane::kRebuiltOffset;

epp::Region code(std::uint32_t at, std::initializer_list<std::uint32_t> words) {
    epp::Region r;
    r.address = at;
    for (const std::uint32_t w : words) {
        for (int shift = 24; shift >= 0; shift -= 8) r.data.push_back(static_cast<std::uint8_t>(w >> shift));
    }
    return r;
}

// if (r3 == 5) return 2; return 1;   (r3 is the first argument; `one` is what it returns otherwise)
epp::Region branching(std::uint32_t at, std::uint32_t one = 0x38600001) {
    return code(at, {0x2C030005,    // cmpwi r3, 5
                     0x4182000C,    // beq +0xC
                     one,           // li r3, 1
                     kBlr,
                     0x38600002,    // li r3, 2
                     kBlr});
}

std::function<std::optional<std::uint32_t>(std::uint32_t)> words_of(const epp::Region& r) {
    return [r](std::uint32_t va) -> std::optional<std::uint32_t> {
        if (va < r.address || va + 4 > r.address + r.data.size()) return std::nullopt;
        const std::size_t at = va - r.address;
        return (std::uint32_t{r.data[at]} << 24) | (std::uint32_t{r.data[at + 1]} << 16) | (std::uint32_t{r.data[at + 2]} << 8) | r.data[at + 3];
    };
}

std::shared_ptr<Lane::Context> context_of(const epp::Region& original, const epp::Region& rebuilt) {
    auto c = std::make_shared<Lane::Context>();
    const std::vector<epp::Region> o = {original}, r = {rebuilt};
    c->memory = Lane::memory_of(o, r);
    c->original_regions = o;
    return c;
}

Lane::Item item_of(const epp::Region& original, std::uint32_t rebuilt_va) {
    Lane::Item i;
    i.key = "82000000";
    i.va = original.address;
    i.span = {original.address, original.address + static_cast<std::uint32_t>(original.data.size())};
    i.insns = static_cast<std::int64_t>(original.data.size() / 4);
    i.rebuilt_va = rebuilt_va;
    return i;
}

CB_TEST("verify.lanes.powerpc.the_fixed_inputs_are_eight_argument_registers_each", [] {
    const std::vector<Vector> v = Lane::vectors();
    CB_CHECK_EQ(v.size(), 14u);
    for (const Vector& in : v) CB_CHECK_EQ(in.args.size(), 8u);
    const std::uint64_t a = epp::kArgsBase;
    CB_CHECK((v[0].args == std::vector<std::int64_t>(8, 0)));
    CB_CHECK((v[1].args == std::vector<std::int64_t>{1, 2, 3, 4, 5, 6, 7, 8}));          // every argument a different value
    CB_CHECK(v[2].args[0] == static_cast<std::int64_t>(a) && v[2].args[1] == static_cast<std::int64_t>(a + 0x1000));
    CB_CHECK(v[3].stub_ret == std::optional<std::int64_t>(1));
    CB_CHECK(v[4].stub_ret == std::optional<std::int64_t>(-1));
    CB_CHECK(v[7].args[0] == -1 && v[7].args[1] == -2);                                    // descending from all ones
    CB_CHECK(v[8].stub_ret == std::optional<std::int64_t>(0x103));
    CB_CHECK(v[9].fill == epp::Fill::Pointers && v[10].fill == epp::Fill::Counts && v[11].fill == epp::Fill::Text);
    CB_CHECK(!v[0].fill && !v[0].mem_init);
    // alternating: a pointer in the even places and a small number in the odd (v[5]), the other way round (v[6])
    CB_CHECK(v[5].args[0] == static_cast<std::int64_t>(a) && v[5].args[1] == 0x40 && v[5].args[2] == static_cast<std::int64_t>(a + 0x1000));
    CB_CHECK(v[6].args[0] == 0x20 && v[6].args[1] == static_cast<std::int64_t>(a + 0x800));
});

CB_TEST("verify.lanes.powerpc.directed_inputs_aim_at_the_constants_the_code_compares_with", [] {
    const epp::Region fn = code(kOriginal, {0x2C030005,      // cmpwi r3, 5
                                            0x28030080,      // cmplwi r3, 0x80
                                            0x2C03FFF0,      // cmpwi r3, -16
                                            0x2C030000,      // cmpwi r3, 0 (not aimed at: zero and one are in the fixed inputs)
                                            0x2C030001,      // cmpwi r3, 1
                                            0x2C030005,      // cmpwi r3, 5 again
                                            0x60000000,      // nop
                                            kBlr});
    const Lane::Span span{kOriginal, kOriginal + 32};
    const std::vector<Vector> d = Lane::directed(words_of(fn), span);
    CB_CHECK_EQ(d.size(), 6u);                                // three constants, two inputs each
    CB_CHECK((d[0].args == std::vector<std::int64_t>(8, 5)) && d[0].stub_ret == std::optional<std::int64_t>(5));
    CB_CHECK(d[1].args[0] == static_cast<std::int64_t>(epp::kArgsBase) && d[1].args[1] == 5 && d[1].args[2] == static_cast<std::int64_t>(epp::kArgsBase + 0x2000));
    CB_CHECK(d[1].fill == epp::Fill::Counts);
    CB_CHECK(d[2].args[0] == 0x80);
    CB_CHECK(d[4].args[0] == -16);                            // cmpi's constant is signed
    CB_CHECK_EQ(Lane::directed(words_of(fn), span, 2).size(), 4u);   // at most so many constants
    CB_CHECK(Lane::directed(words_of(fn), {kOriginal + 24, kOriginal + 32}).empty());   // none in a span with none
});

CB_TEST("verify.lanes.powerpc.an_input_goes_to_the_guided_search_and_back", [] {
    Vector v = Lane::vectors()[10];
    v.mem_init = std::vector<epp::Region>{code(0x60001000, {0xDEADBEEF})};
    const guided::Input g = Lane::to_guided(v);
    CB_CHECK_EQ(g.args.size(), 8u);
    CB_CHECK(g.stub_ret == std::optional<std::uint64_t>(1));
    CB_CHECK(g.mem_init && g.mem_init->size() == 1 && g.mem_init->front().address == 0x60001000);
    CB_CHECK(std::get<std::int64_t>(g.extra.at("fill")) == 2);
    const Vector back = Lane::from_guided(g);
    CB_CHECK(back.args == v.args && back.stub_ret == v.stub_ret && back.fill == v.fill);
    CB_CHECK(back.mem_init && back.mem_init->front().data == v.mem_init->front().data);
    // an input with no fill and no memory of its own has neither on the way back
    const Vector plain = Lane::from_guided(Lane::to_guided(Lane::vectors()[0]));
    CB_CHECK(!plain.fill && !plain.mem_init && !plain.stub_ret);
});

CB_TEST("verify.lanes.powerpc.an_input_is_derived_from_values_the_solver_gave", [] {
    const Vector v = Lane::vectors()[1];
    concolic::Vars names;
    names["a0"] = {concolic::InputVar::Place::Arg, 0, 64, 1};
    names["a3"] = {concolic::InputVar::Place::Arg, 3, 64, 4};
    names["m60000010"] = {concolic::InputVar::Place::Mem, 0x60000010, 8, 0};
    names["other"] = {concolic::InputVar::Place::Arg, 99, 64, 0};           // beyond the arguments: ignored
    // nothing that differs: nothing
    CB_CHECK(!Lane::derive(v, {{"a0", 1}}, names));
    CB_CHECK(!Lane::derive(v, {{"unknown", 7}, {"other", 7}}, names));
    // an argument set
    const auto set = Lane::derive(v, {{"a0", 5}, {"a3", 0xFFFFFFFFFFFFFFFFULL}}, names);
    CB_CHECK(set.has_value());
    CB_CHECK(set->args[0] == 5 && set->args[3] == -1 && set->args[1] == 2);
    CB_CHECK(!set->mem_init);                                               // (the original is untouched, too)
    CB_CHECK(v.args[0] == 1);
    // a byte of memory, with nothing under it: a region of that byte
    const auto byte = Lane::derive(v, {{"m60000010", 0x1AB}}, names);        // (only the low byte is a byte)
    CB_CHECK(byte && byte->mem_init && byte->mem_init->size() == 1);
    CB_CHECK(byte->mem_init->front().address == 0x60000010 && (byte->mem_init->front().data == Bytes{0xAB}));
    // under a fill, the whole page of the fill is given, the byte changed in it
    Vector filled = v;
    filled.fill = epp::Fill::Counts;
    const auto page = Lane::derive(filled, {{"m60000010", 0x7F}}, names);
    CB_CHECK(page && page->mem_init && page->mem_init->size() == 1);
    CB_CHECK(page->mem_init->front().address == 0x60000000 && page->mem_init->front().data.size() == epp::kPageSize);
    CB_CHECK_EQ(page->mem_init->front().data[0x10], 0x7F);
    CB_CHECK_EQ(page->mem_init->front().data[0x14], epp::filled(epp::Fill::Counts)[0x14]);
    // the same value again is no change
    CB_CHECK(!Lane::derive(*byte, {{"m60000010", 0xAB}}, names));
});

CB_TEST("verify.lanes.powerpc.the_images_are_in_memory_each_with_its_extent", [] {
    const epp::Region a = code(kOriginal, {kLi5, kBlr});
    const epp::Region b = code(kRebuilt + 0x100, {kLi5, kBlr, kBlr});
    const std::vector<epp::Region> original = {a}, rebuilt = {b};
    const Lane::Memory both = Lane::memory_of(original, rebuilt);
    CB_CHECK(both.images.size() == 2 && both.images[0].lo == kOriginal && both.images[0].hi == kOriginal + 8);
    CB_CHECK(both.images[1].lo == kRebuilt + 0x100 && both.images[1].hi == kRebuilt + 0x100 + 12);
    CB_CHECK(both.alone.find(kOriginal >> 12) != nullptr && both.alone.find((kRebuilt + 0x100) >> 12) == nullptr);
    CB_CHECK(both.both.find((kRebuilt + 0x100) >> 12) != nullptr);
    CB_CHECK_EQ(Lane::memory_of(original, {}).images.size(), 1u);
});

CB_TEST("verify.lanes.powerpc.a_run_is_a_fingerprint_of_how_it_ended_what_it_returned_and_what_ran", [] {
    const epp::Region fn = code(kOriginal, {kLi5, kBlr});
    const auto c = context_of(fn, code(kRebuilt, {kLi5, kBlr}));
    const compare::Fingerprint fp = Lane::fingerprint(c->memory.alone, kOriginal, Lane::vectors()[1], {}, {}, std::span<const Lane::Span>(c->memory.images).first(1),
                                                      Lane::Span{kOriginal, kOriginal + 8});
    CB_CHECK(!fp.err);
    CB_CHECK_EQ(fp.exit, "ret");
    CB_CHECK(fp.ret == std::optional<std::int64_t>(5));
    CB_CHECK(fp.deps.empty());
    CB_CHECK((fp.cov == std::vector<std::int64_t>{0, 4}));      // the function's own instructions, by offset
    CB_CHECK(fp.saved == std::optional<bool>(true));            // r14-r31 and the stack pointer as they were
    // only the low 32 bits of a result are the result
    const epp::Region neg = code(kOriginal, {0x3860FFFF, kBlr});   // li r3, -1
    const auto n = context_of(neg, neg);
    CB_CHECK(Lane::fingerprint(n->memory.alone, kOriginal, Lane::vectors()[0], {}, {}, n->memory.images, std::nullopt).ret == std::optional<std::int64_t>(0xFFFFFFFF));
    // a function that returns nothing has no result to compare
    CB_CHECK(!Lane::fingerprint(n->memory.alone, kOriginal, Lane::vectors()[0], {}, {}, n->memory.images, std::nullopt, true).ret);
    // where there is no code, the run ends so: nothing to return from
    const compare::Fingerprint lost = Lane::fingerprint(n->memory.alone, 0x10000000, Lane::vectors()[0], {}, {}, n->memory.images);
    CB_CHECK_EQ(lost.exit, "fault");
    CB_CHECK(!lost.ret);
});

CB_TEST("verify.lanes.powerpc.a_call_out_is_in_the_fingerprint_by_name_and_with_arguments_where_their_number_is_known", [] {
    // mflr r12; bl +0xFC (the extern); mtlr r12; blr
    const epp::Region fn = code(kOriginal, {0x7D8802A6, 0x480000FD, 0x7D8803A6, kBlr});
    const auto c = context_of(fn, fn);
    const epp::ExternTable externs = {{kOriginal + 0x100, "puts"}};
    const std::span<const Lane::Span> images = std::span<const Lane::Span>(c->memory.images).first(1);
    const compare::Fingerprint unknown = Lane::fingerprint(c->memory.alone, kOriginal, Lane::vectors()[1], externs, {}, images);
    CB_CHECK_EQ(unknown.exit, "ret");
    CB_CHECK((unknown.deps == std::vector<std::string>{"puts"}));           // compared by name alone: how many arguments is not known
    const compare::Fingerprint known = Lane::fingerprint(c->memory.alone, kOriginal, Lane::vectors()[1], externs, {{"puts", 2}}, images);
    CB_CHECK_EQ(known.deps.size(), 1u);
    CB_CHECK(known.deps[0].starts_with("puts(") && known.deps[0].ends_with(")"));
    // the arguments are the first two of the input
    const compare::Fingerprint other = Lane::fingerprint(c->memory.alone, kOriginal, Lane::vectors()[0], externs, {{"puts", 2}}, images);
    CB_CHECK(other.deps[0] != known.deps[0]);
});

CB_TEST("verify.lanes.powerpc.an_argument_is_shown_as_what_it_is_where_the_builds_differ", [] {
    const epp::Region a = code(kOriginal, {0x11223344, 0x55667788, 0x99AABBCC, 0xDDEEFF00});
    const auto c = context_of(a, code(kRebuilt, {kLi5, kBlr}));
    epp::Machine machine(std::span<const epp::Region>(&a, 1));
    CB_CHECK_EQ(Lane::argument(machine, epp::kStackLow + 0x80, c->memory.images), "stack");
    CB_CHECK_EQ(Lane::argument(machine, kOriginal, c->memory.images), "data:112233445566778899aabbcc");   // what it points at
    CB_CHECK_EQ(Lane::argument(machine, kOriginal + 0x100000000ULL, c->memory.images), "data:112233445566778899aabbcc");   // (a 32-bit address)
    CB_CHECK_EQ(Lane::argument(machine, 0x1234, c->memory.images), lane_common::argument(0x1234));
});

CB_TEST("verify.lanes.powerpc.a_function_and_its_twin_are_equivalent_and_one_that_returns_another_value_is_not", [] {
    const epp::Region original = branching(kOriginal);
    {
        const auto c = context_of(original, branching(kRebuilt));
        const Judgement j = Lane::judge_function(*c, item_of(original, kRebuilt));
        CB_CHECK(j.verdict == Verdict::Equivalent);
        CB_CHECK(j.reason.empty());
    }
    {
        const auto c = context_of(original, branching(kRebuilt, 0x38600007));   // li r3, 7 where the original has li r3, 1
        const Judgement j = Lane::judge_function(*c, item_of(original, kRebuilt));
        CB_CHECK(j.verdict == Verdict::Divergent);
        CB_CHECK_EQ(j.reason, "ret-differs");
        CB_CHECK(!j.detail.empty());
    }
    {
        // a twin that differs only on the branch the fixed inputs do not take (r3 == 5) is found by the inputs aimed at its constants
        const epp::Region twin = code(kRebuilt, {0x2C030005, 0x4182000C, 0x38600001, kBlr, 0x38600009, kBlr});   // li r3, 9 on that side
        const auto c = context_of(original, twin);
        CB_CHECK(Lane::judge_function(*c, item_of(original, kRebuilt)).verdict == Verdict::Divergent);
    }
});

CB_TEST("verify.lanes.powerpc.the_directed_inputs_of_a_span_are_made_once", [] {
    const epp::Region original = branching(kOriginal);
    const auto c = context_of(original, branching(kRebuilt));
    (void)Lane::directed_and_guided(*c, kOriginal, {kOriginal, kOriginal + 24});
    CB_CHECK_EQ(c->directed_by_span.size(), 1u);
    (void)Lane::directed_and_guided(*c, kOriginal, {kOriginal, kOriginal + 24});
    CB_CHECK_EQ(c->directed_by_span.size(), 1u);
    (void)Lane::directed_and_guided(*c, kOriginal, {kOriginal, kOriginal + 16});
    CB_CHECK_EQ(c->directed_by_span.size(), 2u);
});

CB_TEST("verify.lanes.powerpc.the_guided_search_finds_the_input_that_takes_the_other_branch", [] {
    const epp::Region original = branching(kOriginal);
    const auto c = context_of(original, branching(kRebuilt));
    const std::vector<Vector> found = Lane::directed_and_guided(*c, kOriginal, {kOriginal, kOriginal + 24});
    // some input makes r3 equal 5 (the directed ones do), which takes the branch the fixed ones do not
    CB_CHECK(std::ranges::any_of(found, [](const Vector& v) { return v.args[0] == 5; }));
});

// The ppc-be-64 lifter of the machine's pcodecheck (CB_PCODECHECK, or the one built beside the tests), or none.
emulate::pcode::Lifter* ppc_lifter_here() {
    static const std::optional<emulate::pcode::PcodeConfig> config = []() -> std::optional<emulate::pcode::PcodeConfig> {
        if (const auto env = fs::get_env("CB_PCODECHECK"); env && !env->empty()) {
            return emulate::pcode::PcodeConfig{fs::from_text(*env), std::filesystem::path(CB_SOURCE_DIR) / "decompile" / "engine" / "languages"};
        }
        const std::filesystem::path exe = fs::executable_path().parent_path() / "pcodecheck.exe";
        if (std::filesystem::is_regular_file(exe)) return emulate::pcode::PcodeConfig{exe, std::filesystem::path(CB_SOURCE_DIR) / "decompile" / "engine" / "languages"};
        return std::nullopt;
    }();
    static emulate::pcode::LifterSet set(config.value_or(emulate::pcode::PcodeConfig{}));
    return config ? set.get(emulate::pcode::Mode::PpcBe64) : nullptr;
}

// if (r4 == r3 + 7) return 2; return 1;   -- an input the fixed ones never make: the paths are found by following the run
epp::Region needs_a_relation(std::uint32_t at, std::uint32_t two = 0x38600002) {
    return code(at, {0x38A30007,    // addi r5, r3, 7
                     0x7C052000,    // cmpw r5, r4
                     0x4182000C,    // beq +0xC
                     0x38600001,    // li r3, 1
                     kBlr,
                     two,           // li r3, 2
                     kBlr});
}

CB_TEST("verify.lanes.powerpc.the_paths_of_the_original_are_found_by_following_the_run_with_the_lifter", [] {
    emulate::pcode::Lifter* lifter = ppc_lifter_here();
    if (lifter == nullptr) return;   // (no pcodecheck on this machine)
    const epp::Region original = needs_a_relation(kOriginal);
    const auto c = context_of(original, needs_a_relation(kRebuilt));
    c->lifter = lifter;
    const Lane::Span span{kOriginal, kOriginal + 28};
    std::shared_ptr<concolic::Stats> explored;
    const std::vector<Vector> found = Lane::directed_and_guided(*c, kOriginal, span, &explored);
    CB_CHECK(explored != nullptr);
    if (!explored) return;
    CB_CHECK(explored->paths >= 2);
    // an input that makes r4 equal r3 + 7 (in the low 32 bits, which a word compare reads)
    CB_CHECK(std::ranges::any_of(found, [](const Vector& v) { return static_cast<std::uint32_t>(v.args[1] - v.args[0] - 7) == 0; }));
    // so a twin that differs only on that path is found different
    const auto twin = context_of(original, needs_a_relation(kRebuilt, 0x38600009));
    twin->lifter = lifter;
    const Judgement j = Lane::judge_function(*twin, item_of(original, kRebuilt));
    CB_CHECK(j.verdict == Verdict::Divergent);
    // and the same twin is judged equivalent, with the paths said, when the function really is the same
    const Judgement same = Lane::judge_function(*c, item_of(original, kRebuilt));
    CB_CHECK(same.verdict == Verdict::Equivalent);
    CB_CHECK(std::ranges::any_of(same.limits, [](const std::string& l) { return l.starts_with("paths"); }));
});

CB_TEST("verify.lanes.powerpc.the_kind_says_what_it_is", [] {
    const reconstruct::KindTraits t = Lane::traits();
    CB_CHECK(t.per_function);
});

}  // namespace
}  // namespace cb::verify::lanes
