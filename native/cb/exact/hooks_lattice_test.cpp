#include "cb/exact/hooks_lattice.hpp"

#include <map>

#include "cb/core/testing.hpp"

using namespace cb::exact;

namespace {

// Memory of the generating function's frame (segment 0x10) and its operand nodes (segment 0x20).
struct Machine {
    std::map<std::pair<std::uint32_t, std::uint32_t>, std::uint16_t> words;
    hooks_lattice::WordReader reader() const {
        return [this](std::uint32_t seg, std::uint32_t off) {
            const auto it = words.find({seg, off});
            return it == words.end() ? std::uint16_t{0} : it->second;
        };
    }
};

Machine long_compare(std::uint16_t left_flags, std::uint16_t right_flags, std::uint16_t kind = 4) {
    Machine m;
    constexpr std::uint32_t bp = 0x100, ss = 0x10, ds = 0x20;
    m.words[{ss, bp + 0x1C}] = 0x40;   // the left operand node
    m.words[{ss, bp + 0x1E}] = 0x60;   // the right
    m.words[{ss, bp + 6}] = kind;      // what is compared: a long is 4
    m.words[{ds, 0x40 + 2}] = left_flags;
    m.words[{ds, 0x60 + 2}] = right_flags;
    return m;
}

}  // namespace

CB_TEST("exact.hooks_lattice.cmp_test_loads_both_words_unless_reached_through_a_pointer", [] {
    const auto go = [](const Machine& m) { return hooks_lattice::cmp_test(m.reader(), 0x100, 0x10, 0x20); };
    CB_CHECK_EQ(go(long_compare(0, 0)), 0x4D85u);                  // neither: load
    CB_CHECK_EQ(go(long_compare(0x2000, 0)), 0x4DA1u);             // the left one through a pointer: do not
    CB_CHECK_EQ(go(long_compare(0, 0x2000)), 0x4DA1u);             // the right one
    CB_CHECK_EQ(go(long_compare(0x2000, 0x2000)), 0x4DA1u);
    CB_CHECK_EQ(go(long_compare(0x0FFF | 0x4000, 0x8000)), 0x4D85u);   // other flag bits mean nothing here
    CB_CHECK_EQ(go(long_compare(0, 0, 2)), 0x4DA1u);               // not a long being compared
});

CB_TEST("exact.hooks_lattice.cmp_test_reads_only_what_it_needs", [] {
    // for a compare that is not of longs, the operand nodes are not touched at all
    int reads = 0;
    const auto count = [&](std::uint32_t, std::uint32_t) {
        ++reads;
        return std::uint16_t{2};   // every word reads 2: node pointers, and a kind that is not 4
    };
    CB_CHECK_EQ(hooks_lattice::cmp_test(count, 0, 0, 0), 0x4DA1u);
    CB_CHECK_EQ(reads, 3);   // the two node pointers and the kind
});
