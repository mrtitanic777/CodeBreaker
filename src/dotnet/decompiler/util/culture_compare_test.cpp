// Tests of util::culture_compare against .NET itself: culture_compare_golden.inc holds about ten thousand strings in the order .NET 9
// (ICU, culture en-US) sorts them by string.CompareTo -- ASCII (all of it, controls too), accented Latin, Greek, Cyrillic, CJK, kana, Hangul,
// combining marks, symbols, fullwidth forms, surrogate pairs, short strings over a mixed alphabet and namespace and attribute-like names. No two
// of them compare equal, so the order is strict. The program that writes the list (work/ilspy-port/il4/CC, `dotnet run -- golden`) is the oracle.
#include "dotnet/decompiler/util/culture_compare.hpp"

#include <algorithm>
#include <random>
#include <string>
#include <string_view>
#include <vector>

#include "core/testing.hpp"

namespace cb::dotnet::decompiler::util {
namespace {

using namespace std::string_view_literals;

const std::string_view kGolden[] = {
#include "dotnet/decompiler/util/culture_compare_golden.inc"
};
constexpr std::size_t kGoldenCount = sizeof(kGolden) / sizeof(kGolden[0]);

CB_TEST("dotnet.decompiler.util.culture_compare.golden_adjacent", [] {
    // every string compares below the next one, and the next one above it
    for (std::size_t i = 0; i + 1 < kGoldenCount; ++i) {
        CB_CHECK_EQ(culture_compare(kGolden[i], kGolden[i + 1]), -1);
        CB_CHECK_EQ(culture_compare(kGolden[i + 1], kGolden[i]), 1);
    }
});

CB_TEST("dotnet.decompiler.util.culture_compare.golden_sort", [] {
    // sorting a shuffled copy with culture_compare gives the golden order back
    std::vector<std::string_view> v(std::begin(kGolden), std::end(kGolden));
    std::mt19937 rng(12345);
    std::shuffle(v.begin(), v.end(), rng);
    std::stable_sort(v.begin(), v.end(), [](std::string_view a, std::string_view b) { return culture_compare(a, b) < 0; });
    bool same = true;
    for (std::size_t i = 0; i < kGoldenCount && same; ++i) same = v[i] == kGolden[i];
    CB_CHECK(same);
});

CB_TEST("dotnet.decompiler.util.culture_compare.golden_random_pairs", [] {
    std::mt19937 rng(987);
    for (int k = 0; k < 20000; ++k) {
        std::size_t i = rng() % kGoldenCount, j = rng() % kGoldenCount;
        int want = i < j ? -1 : i > j ? 1 : 0;
        CB_CHECK_EQ(culture_compare(kGolden[i], kGolden[j]), want);
    }
});

CB_TEST("dotnet.decompiler.util.culture_compare.known_orders", [] {
    // what .NET answers for the names ILSpy sorts
    CB_CHECK_EQ(culture_compare("a", "A"), -1);                    // lower case first (ordinal says the opposite)
    CB_CHECK_EQ(culture_compare("A", "b"), -1);                    // case is the last thing that counts
    CB_CHECK_EQ(culture_compare("a-b", "ab"), -1);                 // punctuation has a weight and sorts before letters
    CB_CHECK_EQ(culture_compare("a_b", "a-b"), -1);                // '_' before '-'
    CB_CHECK_EQ(culture_compare("1", "a"), -1);                    // digits before letters
    CB_CHECK_EQ(culture_compare("\xC3\xA9", "f"), -1);             // e acute sorts with e
    CB_CHECK_EQ(culture_compare("e\xCC\x81", "\xC3\xA9"), 0);      // decomposed and precomposed are equal
    CB_CHECK_EQ(culture_compare("ab", "ab"), 0);
    CB_CHECK_EQ(culture_compare("", ""), 0);
    CB_CHECK_EQ(culture_compare("", "a"), -1);
    CB_CHECK_EQ(culture_compare("System", "system"), 1);
    CB_CHECK_EQ(culture_compare("System.IO", "System.Collections"), 1);
    CB_CHECK_EQ(culture_compare("a\xC2\xAD" "b", "ab"), 0);        // a soft hyphen is ignorable
});

CB_TEST("dotnet.decompiler.util.culture_compare.ignore_case", [] {
    CB_CHECK_EQ(culture_compare_ignore_case("a", "A"), 0);
    CB_CHECK_EQ(culture_compare_ignore_case("List", "list"), 0);
    CB_CHECK_EQ(culture_compare_ignore_case("a", "B"), -1);
    CB_CHECK_EQ(culture_compare_ignore_case("B", "a"), 1);
    CB_CHECK_EQ(culture_compare_ignore_case("\xC3\xA9", "E"), 1);  // an accent still counts
});

}  // namespace
}  // namespace cb::dotnet::decompiler::util
