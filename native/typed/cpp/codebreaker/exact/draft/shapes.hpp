// codebreaker/exact/draft/shapes.hpp -- compat shim for cb/exact/draft/shapes.hpp (docs/CPP_DESIGN.md 6.3). The
// functions convert and forward; the passes that learn something about the function (a byte widened with zeros, a
// pointer stepped through by words, a long's two words) record it in the old layer's module tables of
// codebreaker/exact/draft/text.hpp (UCHAR, PTRSIZE, LONGINFO) after the call, and range_switches reads RANGES from them,
// until forms/candidates/prove hold a cb::exact::draft::text::FunctionFacts instead. Deleted with its last old-layer
// consumer.
//
// shapes.py -- the control flow a compiler gives a statement, read back into the statement: a comparison's value, the
// returns, the loops and their tests, a switch's arms, the gotos the model leaves.
//
// C++ spelling notes:
//  * a pass that Python gives back "the text, or None when it changes nothing" returns std::optional<py::str>;
//  * a pass that gives back a list of variants returns py::list<py::str>;
//  * `most` (unsigned_fields, adjacent_swaps, goto_tails) keeps Python's defaults.
#pragma once

#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/exact/draft/shapes.hpp"
#include "codebreaker/exact/draft/text.hpp"
#include "py.hpp"

namespace codebreaker::exact::draft::shapes {

namespace ns = cb::exact::draft::shapes;
namespace nt = cb::exact::draft::text;

namespace detail {

using cb::compat::guard;

inline py::str text_of(const std::string& s) { return py::str(s); }
inline std::optional<py::str> maybe_text(const std::optional<std::string>& s) {
    return s ? std::optional<py::str>(py::str(*s)) : std::nullopt;
}

// The facts the passes read: the ranges a loaded register is tested against.
inline nt::FunctionFacts facts_from_tables() {
    nt::FunctionFacts f;
    for (const auto& r : text::RANGES) f.ranges.insert(nt::Range{std::get<0>(r), std::get<1>(r), std::get<2>(r)});
    return f;
}

// What a pass learned, added to the old layer's tables (the passes only ever add).
inline void tables_from_facts(const nt::FunctionFacts& f) {
    for (const nt::Site& s : f.unsigned_sites)
        text::UCHAR.add(std::variant<std::tuple<py::str, int64_t>, py::str>(std::in_place_index<0>, std::make_tuple(py::str(s.pointer), s.offset)));
    for (const std::string& g : f.unsigned_globals)
        text::UCHAR.add(std::variant<std::tuple<py::str, int64_t>, py::str>(std::in_place_index<1>, py::str(g)));
    for (const std::string& p : f.word_pointers) text::PTRSIZE[py::str(p)] = 2;
    if (!f.longs.fields.empty()) {
        if (!text::LONGINFO.fields) text::LONGINFO.fields = py::set<int64_t>();
        for (std::int64_t v : f.longs.fields) text::LONGINFO.fields->add(v);
    }
    if (!f.longs.globals.empty()) {
        if (!text::LONGINFO.globals) text::LONGINFO.globals = py::set<py::str>();
        for (const std::string& g : f.longs.globals) text::LONGINFO.globals->add(py::str(g));
    }
}

// A pass that records what it learned.
template <class F>
auto learning(F&& pass) {
    nt::FunctionFacts facts = facts_from_tables();
    auto result = guard([&] { return pass(facts); });
    tables_from_facts(facts);
    return result;
}

}  // namespace detail

#define CB_SHIM_TEXT_PASS(NAME) \
    inline py::str NAME(const py::str& text) { return detail::text_of(detail::guard([&] { return ns::NAME(text.utf8()); })); }
#define CB_SHIM_OPT_PASS(NAME)                                                                                  \
    inline std::optional<py::str> NAME(const py::str& text) {                                                   \
        return detail::maybe_text(detail::guard([&] { return ns::NAME(text.utf8()); }));                        \
    }
#define CB_SHIM_LEARNING_PASS(NAME)                                                                             \
    inline py::str NAME(const py::str& text) {                                                                  \
        return detail::text_of(detail::learning([&](nt::FunctionFacts& facts) { return ns::NAME(text.utf8(), facts); })); \
    }

CB_SHIM_TEXT_PASS(compare_values)
CB_SHIM_TEXT_PASS(logical_values)
CB_SHIM_TEXT_PASS(step_tests)
CB_SHIM_TEXT_PASS(conditional_values)
CB_SHIM_TEXT_PASS(branch_returns)
CB_SHIM_TEXT_PASS(push_returns)
CB_SHIM_TEXT_PASS(chain_returns)
CB_SHIM_LEARNING_PASS(zero_extends)
CB_SHIM_LEARNING_PASS(high_zero)
CB_SHIM_TEXT_PASS(widen_bytes)
CB_SHIM_TEXT_PASS(post_increments)
CB_SHIM_TEXT_PASS(loop_registers)
CB_SHIM_OPT_PASS(const_tests)
CB_SHIM_OPT_PASS(loop_guards)
CB_SHIM_TEXT_PASS(first_breaks)
CB_SHIM_OPT_PASS(loop_tests)
CB_SHIM_TEXT_PASS(call_tests)
CB_SHIM_TEXT_PASS(fall_dups)
CB_SHIM_TEXT_PASS(fall_gotos)
CB_SHIM_TEXT_PASS(tested_zero)
CB_SHIM_TEXT_PASS(continue_loops)
CB_SHIM_TEXT_PASS(empty_loops)
CB_SHIM_TEXT_PASS(stored_reads)
CB_SHIM_TEXT_PASS(stored_tests)
CB_SHIM_OPT_PASS(flat_returns)
CB_SHIM_TEXT_PASS(switch_follow)
CB_SHIM_OPT_PASS(return_cases_first)
CB_SHIM_TEXT_PASS(selector_reads)
CB_SHIM_TEXT_PASS(word_offsets)
CB_SHIM_TEXT_PASS(byte_heads)
CB_SHIM_TEXT_PASS(common_tails)
CB_SHIM_TEXT_PASS(common_heads)
CB_SHIM_OPT_PASS(call_pairs)
CB_SHIM_TEXT_PASS(lvalue_choices)
CB_SHIM_OPT_PASS(label_sinks)
CB_SHIM_OPT_PASS(shared_exit)
CB_SHIM_LEARNING_PASS(pointer_index)
CB_SHIM_OPT_PASS(heads_variant)
CB_SHIM_OPT_PASS(chain_tails)
CB_SHIM_TEXT_PASS(goto_returns)
CB_SHIM_OPT_PASS(char_widens)
CB_SHIM_TEXT_PASS(tail_continues)
CB_SHIM_OPT_PASS(assign_loops)
CB_SHIM_TEXT_PASS(store_joins)
CB_SHIM_TEXT_PASS(loop_collapse)
CB_SHIM_TEXT_PASS(switch_ifs)
CB_SHIM_TEXT_PASS(ternary_joins)
CB_SHIM_TEXT_PASS(stored_values)
CB_SHIM_TEXT_PASS(two_case_switches)
CB_SHIM_LEARNING_PASS(long_tests)
CB_SHIM_LEARNING_PASS(sunk_pairs)
CB_SHIM_TEXT_PASS(sunk_tails)
CB_SHIM_TEXT_PASS(register_chains)
CB_SHIM_TEXT_PASS(store_loops)
CB_SHIM_TEXT_PASS(copy_props)
CB_SHIM_TEXT_PASS(self_updates)
CB_SHIM_TEXT_PASS(post_steps)
CB_SHIM_TEXT_PASS(copy_tests)
CB_SHIM_TEXT_PASS(test_first_loops)
CB_SHIM_TEXT_PASS(stored_calls)
CB_SHIM_OPT_PASS(for_loops)
CB_SHIM_TEXT_PASS(temporaries)
CB_SHIM_TEXT_PASS(fold_registers)

#undef CB_SHIM_TEXT_PASS
#undef CB_SHIM_OPT_PASS
#undef CB_SHIM_LEARNING_PASS

// a switch whose cases are one run of values - LO..HI - the compiler tests as a range (reads text::RANGES)
inline py::str range_switches(const py::str& text) {
    const nt::FunctionFacts facts = detail::facts_from_tables();
    return detail::text_of(detail::guard([&] { return ns::range_switches(text.utf8(), facts); }));
}

// C ? A : B - one test's two values
inline py::str _ternary(const py::str& c, const py::str& a, const py::str& b) {
    return detail::text_of(detail::guard([&] { return ns::ternary(c.utf8(), a.utf8(), b.utf8()); }));
}

// the long two word expressions are, the low and the high, or None (fills text::LONGINFO)
inline std::optional<py::str> _pair_long(const py::str& lo, const py::str& hi) {
    return detail::maybe_text(detail::learning([&](nt::FunctionFacts& facts) { return ns::pair_long(lo.utf8(), hi.utf8(), facts); }));
}

inline std::optional<py::str> goto_tails(const py::str& text, int64_t most = 3) {
    return detail::maybe_text(detail::guard([&] { return ns::goto_tails(text.utf8(), cb::compat::int_from_py<std::size_t>(most)); }));
}

inline py::list<py::str> unsigned_fields(const py::str& text, int64_t most = 3) {
    return cb::compat::to_py(detail::guard([&] { return ns::unsigned_fields(text.utf8(), cb::compat::int_from_py<std::size_t>(most)); }));
}

inline py::list<py::str> adjacent_swaps(const py::str& text, int64_t most = 4) {
    return cb::compat::to_py(detail::guard([&] { return ns::adjacent_swaps(text.utf8(), cb::compat::int_from_py<std::size_t>(most)); }));
}

inline py::list<py::str> case_guards(const py::str& text) {
    return cb::compat::to_py(detail::guard([&] { return ns::case_guards(text.utf8()); }));
}

}  // namespace codebreaker::exact::draft::shapes
