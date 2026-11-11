// Tests of guided input search: how a comparison is turned round (flips), where a value can be put in an input
// (places, put), and the search itself against small programs whose branches depend on their arguments.

#include "cb/verify/guided.hpp"

#include <algorithm>
#include <set>

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::verify::guided;

// DIVERGES (from the Python verify/guided.py this replaces; each reviewed):
//  * the "fixed changes" stage draws from SplitMix64 seeded by Options::seed, not CPython's Mersenne Twister: the same
//    inputs are found where a comparison opens the branch (the common case), and the random picks differ;
//  * a value wider than its operand (`value` 0xFFFF for a one-byte compare) is cut to the operand's size; the Python's
//    value.to_bytes raised OverflowError. Such a pair is not produced by the lanes (values come from the operand);
//  * putting a value into a memory block wider than what is left of it grows the block (the Python's bytearray slice
//    assignment did too); a place outside any block is an InternalError, not an IndexError.

namespace {

constexpr std::uint64_t kArgsAt = 0x1000, kStackLow = 0x800, kStackHigh = 0x2000;

Compare cmp(CompareOp op, unsigned size, std::uint64_t a, std::uint64_t b,
            std::optional<std::uint64_t> a_at = std::nullopt, std::optional<std::uint64_t> b_at = std::nullopt) {
    return Compare{op, size, a, b, a_at, b_at};
}

std::vector<Flip> f(std::initializer_list<Flip> l) {
    return std::vector<Flip>(l);
}

Observation base_observation() {
    Observation o;
    o.args_at = kArgsAt;
    o.stack_low = kStackLow;
    o.stack_high = kStackHigh;
    return o;
}

// A program of three branches: arg0 == 0x1234; (arg1 & 0x80) != 0; and, only past the first, arg1 == 7.
// Each branch records what it compared and where (the argument slots at kArgsAt).
std::optional<Observation> program(const Input& in) {
    const std::uint64_t a0 = in.args.size() > 0 ? in.args[0] : 0, a1 = in.args.size() > 1 ? in.args[1] : 0;
    Observation o = base_observation();
    o.reach.insert(0x100);
    const bool first = a0 == 0x1234;
    o.branches.push_back({0x100, first, cmp(CompareOp::Other, 4, a0, 0x1234, kArgsAt)});
    o.reach.insert(0x110);
    o.branches.push_back({0x110, (a1 & 0x80) != 0, cmp(CompareOp::Test, 4, a1, 0x80, kArgsAt + 4)});
    if (first) {
        o.reach.insert(0x120);
        o.branches.push_back({0x120, a1 == 7, cmp(CompareOp::Other, 4, a1, 7, kArgsAt + 4)});
    }
    return o;
}

Input args(std::initializer_list<std::uint64_t> a) {
    Input in;
    in.args = a;
    return in;
}

} // namespace

CB_TEST("verify.guided.parse_compare_op", [] {
    CB_CHECK(parse_compare_op("test") == CompareOp::Test);
    CB_CHECK(parse_compare_op("and") == CompareOp::And);
    CB_CHECK(parse_compare_op("or") == CompareOp::Or);
    CB_CHECK(parse_compare_op("dec") == CompareOp::Dec);
    CB_CHECK(parse_compare_op("inc") == CompareOp::Inc);
    CB_CHECK(parse_compare_op("add") == CompareOp::Add);
    CB_CHECK(parse_compare_op("xor") == CompareOp::Xor);
    CB_CHECK(parse_compare_op("cmp") == CompareOp::Other);
    CB_CHECK(parse_compare_op("sub") == CompareOp::Other);
    CB_CHECK(parse_compare_op("") == CompareOp::Other);
});

CB_TEST("verify.guided.flips_zero_tests", [] {
    // a zero test: zero becomes one, anything else becomes zero
    CB_CHECK(flips(cmp(CompareOp::Test, 4, 0, 0)) == f({{0, 0, 1}}));
    CB_CHECK(flips(cmp(CompareOp::Test, 4, 5, 5)) == f({{0, 5, 0}}));
    CB_CHECK(flips(cmp(CompareOp::Or, 2, 0, 0)) == f({{0, 0, 1}}));
    CB_CHECK(flips(cmp(CompareOp::And, 1, 9, 9)) == f({{0, 9, 0}}));
});

CB_TEST("verify.guided.flips_flag_tests", [] {
    // the bits of a mask: set them, clear them (a already has none of b's bits here), set them in the other operand
    CB_CHECK(flips(cmp(CompareOp::Test, 1, 0x0F, 0xF0)) == f({{0, 0x0F, 0xFF}, {0, 0x0F, 0x0F}, {1, 0xF0, 0xFF}}));
    CB_CHECK(flips(cmp(CompareOp::And, 1, 0xFF, 0x80)) == f({{0, 0xFF, 0xFF}, {0, 0xFF, 0x7F}, {1, 0x80, 0xFF}}));
});

CB_TEST("verify.guided.flips_arithmetic", [] {
    CB_CHECK(flips(cmp(CompareOp::Dec, 4, 3, 1)) == f({{0, 3, 1}, {0, 3, 0}}));
    CB_CHECK(flips(cmp(CompareOp::Add, 4, 3, 1)) == f({{0, 3, 1}, {0, 3, 0}}));
    CB_CHECK(flips(cmp(CompareOp::Xor, 2, 3, 1)) == f({{0, 3, 1}, {0, 3, 0}}));
    CB_CHECK(flips(cmp(CompareOp::Or, 4, 3, 1)) == f({{0, 3, 1}, {0, 3, 0}})); // (a zero test only when both are alike)
    CB_CHECK(flips(cmp(CompareOp::Inc, 4, 3, 1)) ==
             f({{0, 3, 0xFFFFFFFF}, {0, 3, 0}})); // minus b, at the operand's width
    CB_CHECK(flips(cmp(CompareOp::Inc, 1, 3, 1)) == f({{0, 3, 0xFF}, {0, 3, 0}}));
});

CB_TEST("verify.guided.flips_comparisons", [] {
    // each operand is tried at the other's value and one either side of it, never at the value it has
    CB_CHECK(flips(cmp(CompareOp::Other, 1, 5, 7)) ==
             f({{0, 5, 7}, {0, 5, 8}, {0, 5, 6}, {1, 7, 5}, {1, 7, 6}, {1, 7, 4}}));
    // wrapping at the width: 0xFF + 1 is 0, 0 - 1 is 0xFF
    CB_CHECK(flips(cmp(CompareOp::Other, 1, 0xFF, 0xFF)) ==
             f({{0, 0xFF, 0}, {0, 0xFF, 0xFE}, {1, 0xFF, 0}, {1, 0xFF, 0xFE}}));
    CB_CHECK(flips(cmp(CompareOp::Other, 2, 0, 0)) == f({{0, 0, 1}, {0, 0, 0xFFFF}, {1, 0, 1}, {1, 0, 0xFFFF}}));
    // a full 64-bit word
    const std::uint64_t all = ~std::uint64_t{0};
    CB_CHECK(flips(cmp(CompareOp::Other, 8, 0, all)) == f({{0, 0, all}, {0, 0, all - 1}, {1, all, 0}, {1, all, 1}}));
});

CB_TEST("verify.guided.places_follow_the_operand_then_the_value", [] {
    Input in = args({0x11, 0x2222});
    in.mem_init.emplace().push_back({0x5000, {0x00, 0x11, 0x22, 0x33, 0x44, 0x22, 0x22, 0x00}});
    in.stub_ret = 0x22;
    Observation o = base_observation();
    const Machine little{ByteOrder::Little, 4};
    // read from the second argument's slot: that slot first; then the memory that holds the value (once: the
    // argument that holds it is the same place)
    Compare c = cmp(CompareOp::Other, 2, 0x2222, 0, kArgsAt + 4);
    std::vector<Place> got = places(in, o, c, 0, 0x2222, 2, little);
    CB_CHECK_EQ(got.size(), std::size_t{2});
    CB_CHECK(got[0] == (Place{PlaceKind::Arg, 1, 0}));
    CB_CHECK(got[1] == (Place{PlaceKind::Mem, 0, 5}));
    // the stub's return value holds 0x22
    got = places(in, o, cmp(CompareOp::Other, 1, 0x22, 0), 0, 0x22, 1, little);
    CB_CHECK_EQ(got.size(), std::size_t{4}); // argument 1 (its low byte), the stub's value, two hits in the block
    CB_CHECK(got[0] == (Place{PlaceKind::Arg, 1, 0}));
    CB_CHECK(got[1] == (Place{PlaceKind::StubRet, 0, 0}));
    CB_CHECK(got[2] == (Place{PlaceKind::Mem, 0, 2}));
    CB_CHECK(got[3] == (Place{PlaceKind::Mem, 0, 5}));
});

CB_TEST("verify.guided.places_at_most_six_and_by_address", [] {
    Input in;
    in.mem_init.emplace().push_back({0x5000, Bytes(32, 0x07)}); // 32 places hold a 0x07 byte
    Observation o = base_observation();
    std::vector<Place> got = places(in, o, cmp(CompareOp::Other, 1, 7, 0), 0, 7, 1, Machine{});
    CB_CHECK_EQ(got.size(), std::size_t{2}); // at most two hits in one block
    in.mem_init->push_back({0x6000, Bytes(32, 0x07)});
    in.mem_init->push_back({0x7000, Bytes(32, 0x07)});
    in.mem_init->push_back({0x8000, Bytes(32, 0x07)});
    got = places(in, o, cmp(CompareOp::Other, 1, 7, 0), 0, 7, 1, Machine{});
    CB_CHECK_EQ(got.size(), std::size_t{6}); // and six in all

    // read from memory outside the arguments and outside the stack: that address; inside the stack: not a place
    Input none;
    got = places(none, o, cmp(CompareOp::Other, 4, 1, 0, 0x400000), 0, 1, 4, Machine{});
    CB_CHECK_EQ(got.size(), std::size_t{1});
    CB_CHECK(got[0] == (Place{PlaceKind::Addr, 0x400000, 0}));
    got =
        places(none, o, cmp(CompareOp::Other, 4, 1, 0, 0x1F00), 0, 1, 4, Machine{}); // in the stack, past the arguments
    CB_CHECK(got.empty());
    // the second operand has its own address
    got = places(none, o, cmp(CompareOp::Other, 4, 1, 2, std::nullopt, 0x400010), 1, 2, 4, Machine{});
    CB_CHECK_EQ(got.size(), std::size_t{1});
    CB_CHECK(got[0] == (Place{PlaceKind::Addr, 0x400010, 0}));
});

CB_TEST("verify.guided.places_byte_order_in_a_word", [] {
    Input in;
    Observation o = base_observation();
    // a one-byte compare of the second byte of the first argument word (offset 1)
    Compare c = cmp(CompareOp::Other, 1, 0, 0, kArgsAt + 1);
    auto little = places(in, o, c, 0, 0, 1, Machine{ByteOrder::Little, 4});
    auto big = places(in, o, c, 0, 0, 1, Machine{ByteOrder::Big, 4});
    CB_CHECK(little.size() == 1 && little[0] == (Place{PlaceKind::Arg, 0, 1}));
    CB_CHECK(big.size() == 1 &&
             big[0] == (Place{PlaceKind::Arg, 0, 2})); // byte 1 from the front is byte 2 from the low end
    // a compare wider than what is left of the word is not in the argument
    auto wide = places(in, o, cmp(CompareOp::Other, 4, 0, 0, kArgsAt + 2), 0, 0, 4, Machine{});
    CB_CHECK(wide.empty()); // (and inside the stack, so not an address either)
});

CB_TEST("verify.guided.put", [] {
    const Machine little{ByteOrder::Little, 4}, big{ByteOrder::Big, 4};
    // an argument one past those given
    Input one = args({1});
    Input out = put(one, Place{PlaceKind::Arg, 2, 0}, 5, 4, little);
    CB_CHECK((out.args == std::vector<std::uint64_t>{1, 0, 5}));
    CB_CHECK((one.args == std::vector<std::uint64_t>{1})); // the original is not touched
    // one byte of a word
    out = put(args({0xAABBCCDD}), Place{PlaceKind::Arg, 0, 1}, 0xEE, 1, little);
    CB_CHECK_EQ(out.args[0], std::uint64_t{0xAABBEEDD});
    // the value is cut to the size: only its low byte goes in
    out = put(args({0xAABBCCDD}), Place{PlaceKind::Arg, 0, 0}, 0x1FF, 1, little);
    CB_CHECK_EQ(out.args[0], std::uint64_t{0xAABBCCFF});
    // a 16-bit machine's word keeps to 16 bits
    out = put(args({0x1234}), Place{PlaceKind::Arg, 0, 0}, 0xFFFF, 2, Machine{ByteOrder::Little, 2});
    CB_CHECK_EQ(out.args[0], std::uint64_t{0xFFFF});
    // the stub's return value
    CB_CHECK(put(Input{}, Place{PlaceKind::StubRet, 0, 0}, 7, 1, little).stub_ret == std::optional<std::uint64_t>(7));
    Input stub;
    stub.stub_ret = 0x1234;
    CB_CHECK(put(stub, Place{PlaceKind::StubRet, 0, 0}, 5, 1, little).stub_ret == std::optional<std::uint64_t>(0x1205));
    // memory the input did not set: a block is added, in the machine's byte order
    out = put(Input{}, Place{PlaceKind::Addr, 0x4000, 0}, 0x1234, 2, little);
    CB_CHECK(out.mem_init && out.mem_init->size() == 1 && (*out.mem_init)[0].address == 0x4000 &&
             ((*out.mem_init)[0].data == Bytes{0x34, 0x12}));
    out = put(Input{}, Place{PlaceKind::Addr, 0x4000, 0}, 0x1234, 2, big);
    CB_CHECK(((*out.mem_init)[0].data == Bytes{0x12, 0x34}));
    // inside a block the input already sets
    Input mem;
    mem.mem_init.emplace().push_back({0x5000, Bytes(8, 0)});
    out = put(mem, Place{PlaceKind::Mem, 0, 3}, 0xAABBCCDD, 4, big);
    CB_CHECK(((*out.mem_init)[0].data == Bytes{0, 0, 0, 0xAA, 0xBB, 0xCC, 0xDD, 0}));
    CB_CHECK(((*mem.mem_init)[0].data == Bytes(8, 0)));
    // extra keys travel with the copy
    Input with_extra = args({1});
    with_extra.extra["ecx"] = std::int64_t{9};
    CB_CHECK(put(with_extra, Place{PlaceKind::Arg, 0, 0}, 2, 4, little).extra.at("ecx") == Extra(std::int64_t{9}));
});

CB_TEST("verify.guided.put_grows_a_block_and_checks_its_place", [] {
    Input mem;
    mem.mem_init.emplace().push_back({0x5000, Bytes{1, 2, 3, 4, 5}});
    Input out = put(mem, Place{PlaceKind::Mem, 0, 3}, 0xAABBCCDD, 4, Machine{ByteOrder::Little, 4});
    CB_CHECK(((*out.mem_init)[0].data ==
              Bytes{1, 2, 3, 0xDD, 0xCC, 0xBB, 0xAA})); // four bytes at 3: two replaced, two added
    CB_CHECK_THROWS_AS(InternalError, put(mem, Place{PlaceKind::Mem, 1, 0}, 1, 1, Machine{})); // no such block
    CB_CHECK_THROWS_AS(InternalError, put(mem, Place{PlaceKind::Mem, 0, 9}, 1, 1, Machine{})); // past its end
    CB_CHECK_THROWS_AS(InternalError,
                       put(Input{}, Place{PlaceKind::Mem, 0, 0}, 1, 1, Machine{})); // the input sets no memory
    // a value wider than its operand is cut to it
    Input in;
    Observation o = base_observation();
    auto got = places(in, o, cmp(CompareOp::Other, 1, 0, 0, 0x400000), 0, 0xFFFF, 1, Machine{});
    CB_CHECK(got.size() == 1 && got[0] == (Place{PlaceKind::Addr, 0x400000, 0}));
});

CB_TEST("verify.guided.finds_the_branches_the_seeds_miss", [] {
    std::vector<Input> seeds = {args({0, 0})};
    int runs = 0;
    Run run = [&](const Input& in) {
        ++runs;
        return program(in);
    };
    std::vector<Input> got = guided_inputs(run, seeds);
    // the three one-sided jumps, in the order found: the first equality, the flag test, then the jump behind the first
    CB_CHECK_EQ(got.size(), std::size_t{3});
    CB_CHECK((got[0].args == std::vector<std::uint64_t>{0x1234, 0}));
    CB_CHECK((got[1].args == std::vector<std::uint64_t>{0, 0x80}));
    CB_CHECK((got[2].args == std::vector<std::uint64_t>{0x1234, 7}));
    // every direction of every branch is now taken by the seeds or the kept inputs
    std::set<std::pair<std::uint64_t, bool>> edges;
    for (const Input& in : {seeds[0], got[0], got[1], got[2]}) {
        const Observation o = *program(in);
        for (const Branch& b : o.branches)
            edges.insert({b.address, b.taken});
    }
    CB_CHECK_EQ(edges.size(), std::size_t{6});
    CB_CHECK(runs < 30); // found without searching blindly
    // the same again: a run is reproducible
    CB_CHECK(guided_inputs(run, seeds) == got);
});

CB_TEST("verify.guided.limits", [] {
    std::vector<Input> seeds = {args({0, 0})};
    Run run = program;
    Options one;
    one.keep = 1;
    CB_CHECK_EQ(guided_inputs(run, seeds, one).size(), std::size_t{1});
    Options none;
    none.budget = 0;
    CB_CHECK(guided_inputs(run, seeds, none).empty());
    Options two;
    two.budget = 2; // two attempts: the first equality flip, then the flag test
    CB_CHECK_EQ(guided_inputs(run, seeds, two).size(), std::size_t{2});
});

CB_TEST("verify.guided.unobservable_and_empty", [] {
    std::vector<Input> seeds = {args({0, 0})};
    int runs = 0;
    Run blind = [&](const Input&) -> std::optional<Observation> {
        ++runs;
        return std::nullopt;
    };
    CB_CHECK(guided_inputs(blind, seeds).empty());
    CB_CHECK_EQ(runs, 1);                         // nothing to start from: the seed was run once and nothing else tried
    CB_CHECK(guided_inputs(program, {}).empty()); // no seeds and nothing already seen
});

CB_TEST("verify.guided.observed_inputs_are_not_run_again", [] {
    int runs = 0;
    Run run = [&](const Input& in) {
        ++runs;
        return program(in);
    };
    Seen seen{args({0, 0}), *program(args({0, 0}))};
    Options o;
    o.observed = std::span<const Seen>(&seen, 1);
    std::vector<Input> got = guided_inputs(run, {}, o);
    CB_CHECK_EQ(got.size(), std::size_t{3});
    // the seed itself was never run again (it would have been the first run)
    CB_CHECK(runs <= 30);
    Input zero = args({0, 0});
    CB_CHECK(std::find(got.begin(), got.end(), zero) == got.end());
});

CB_TEST("verify.guided.fixed_changes_when_no_comparison_helps", [] {
    // a branch that records no comparison (the lane could not tell): only the fixed changes can open it. It opens when
    // an argument is 0x10000 (one of the interesting values) -- a program whose jump at 0x100 depends on arg0 alone.
    Run run = [](const Input& in) -> std::optional<Observation> {
        Observation o = base_observation();
        const std::uint64_t a0 = in.args.empty() ? 0 : in.args[0];
        o.reach.insert(0x100);
        o.branches.push_back({0x100, a0 == 0x10000, std::nullopt});
        if (a0 == 0x10000)
            o.reach.insert(0x200);
        return o;
    };
    Options o;
    o.budget = 400;
    bool found = false;
    for (std::uint64_t seed = 0; seed < 64 && !found; ++seed) { // the draws are random: some seed finds it
        o.seed = seed;
        for (const Input& in : guided_inputs(run, std::vector<Input>{args({1})}, o))
            found |= in.args[0] == 0x10000;
    }
    CB_CHECK(found);
    // and the same seed gives the same inputs
    o.seed = 3;
    CB_CHECK(guided_inputs(run, std::vector<Input>{args({1})}, o) ==
             guided_inputs(run, std::vector<Input>{args({1})}, o));
});

CB_TEST("verify.guided.extra_keys_ride_along", [] {
    Input seed = args({0, 0});
    seed.extra["ecx"] = std::int64_t{3};
    seed.extra["given"] = std::vector<std::int64_t>{1, 2};
    std::vector<Input> got = guided_inputs(program, std::vector<Input>{seed});
    CB_CHECK(!got.empty());
    for (const Input& in : got) {
        CB_CHECK(in.extra.at("ecx") == Extra(std::int64_t{3}));
        CB_CHECK((in.extra.at("given") == Extra(std::vector<std::int64_t>{1, 2})));
    }
});
