// Tests of the C back end's data (backends/c): the pass sets the writer does on the tree, the decompiler's scalar sizes, and a
// parameter's name. The sets are those of codebreaker/translate/backends/c.py (MOVED 52, IN_MODULE 43 with open_arguments listed
// twice, FLAT 13, POWERPC 8), checked by their sizes and by what each is a subset of.
//
// DIVERGES from c.py: IN_MODULE listed `open_arguments` twice; the set has it once (42 passes). A parameter's name is its
// symbol's (a typed Param has no name of its own to fall back to).
#include "cb/translate/backends/c.hpp"

#include <algorithm>
#include <set>

#include "cb/core/testing.hpp"

namespace {

namespace cback = cb::translate::backends::c;
namespace ir = cb::translate::ir;

std::set<std::string_view> as_set(std::span<const std::string_view> passes) { return {passes.begin(), passes.end()}; }

bool is_subset(std::span<const std::string_view> sub, std::span<const std::string_view> whole) {
    const auto all = as_set(whole);
    return std::all_of(sub.begin(), sub.end(), [&](std::string_view p) { return all.contains(p); });
}

}  // namespace

CB_TEST("translate.backends.c.moved_passes_are_every_pass_once", [] {
    CB_CHECK_EQ(cback::moved_passes().size(), std::size_t{52});
    CB_CHECK_EQ(as_set(cback::moved_passes()).size(), std::size_t{52});   // no pass twice
    CB_CHECK_EQ(cback::moved_passes().front(), std::string_view("far"));
    CB_CHECK_EQ(cback::moved_passes().back(), std::string_view("uint8_result"));
});

CB_TEST("translate.backends.c.path_sets_are_subsets_of_the_moved_passes", [] {
    CB_CHECK_EQ(cback::module_passes().size(), std::size_t{42});
    CB_CHECK_EQ(cback::flat_passes().size(), std::size_t{13});
    CB_CHECK_EQ(cback::powerpc_passes().size(), std::size_t{8});
    CB_CHECK_EQ(as_set(cback::module_passes()).size(), cback::module_passes().size());
    CB_CHECK(is_subset(cback::module_passes(), cback::moved_passes()));
    CB_CHECK(is_subset(cback::flat_passes(), cback::moved_passes()));
    CB_CHECK(is_subset(cback::powerpc_passes(), cback::moved_passes()));
    // what the module needs and the first-run C does without: the facts' passes are not in the flat set
    CB_CHECK(as_set(cback::module_passes()).contains("heading_facts"));
    CB_CHECK(!as_set(cback::flat_passes()).contains("heading_facts"));
    CB_CHECK(as_set(cback::flat_passes()).contains("null_is_zero"));
    CB_CHECK(as_set(cback::powerpc_passes()).contains("homed_arguments"));
});

CB_TEST("translate.backends.c.scalar_sizes", [] {
    CB_CHECK_EQ(cback::scalar_size("int1").value(), std::size_t{1});
    CB_CHECK_EQ(cback::scalar_size("xunknown2").value(), std::size_t{2});
    CB_CHECK_EQ(cback::scalar_size("uint4").value(), std::size_t{4});
    CB_CHECK_EQ(cback::scalar_size("float8").value(), std::size_t{8});
    CB_CHECK_EQ(cback::scalar_size("float10").value(), std::size_t{10});
    CB_CHECK(!cback::scalar_size("int3"));
    CB_CHECK(!cback::scalar_size(""));
    CB_CHECK(!cback::scalar_size("code"));   // a type of the decompiler's, but not a scalar with a size
    // the table and the lookup agree
    CB_CHECK_EQ(cback::scalar_sizes().size(), std::size_t{22});
    for (const cback::ScalarSize& s : cback::scalar_sizes()) CB_CHECK_EQ(cback::scalar_size(s.name).value(), s.bytes);
});

CB_TEST("translate.backends.c.param_name", [] {
    cb::Arena arena;
    ir::Node* named = ir::make(arena, ir::Param{.sym = ir::make_sym(ir::SymKind::Param, "count")});
    ir::Node* anonymous = ir::make(arena, ir::Param{});
    CB_CHECK_EQ(cback::param_name(*ir::get<ir::Param>(named)), std::string("count"));
    CB_CHECK_EQ(cback::param_name(*ir::get<ir::Param>(anonymous)), std::string());
});

CB_TEST("translate.backends.c.decline_is_the_library_s", [] {
    static_assert(std::is_same_v<cback::Declined, cb::translate::native_l2::Declined>);
    CB_CHECK(true);
});
