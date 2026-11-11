// Tests of cb/invert/early_c_frame.hpp: the 24 ground-truth functions the frame rule was recovered from (the
// allocator probes of MC1/MC2: each function's temporaries and locals in declaration order, the `sub sp,N` the compiler
// emitted and the frame offsets the code referenced), the copy-reversal model, and the declarations emitted from them.
//
// DIVERGES from the Python: a copy chain that runs into a cycle ends where it would re-enter it (the Python's
// _chain_swaps looped forever); offsets are unsigned (the compiler allocates upward from bp, so they never are
// negative); there is no str-keyed variant of the copy functions (the Python's tests used one, the model never did).
#include "cb/invert/early_c_frame.hpp"

#include <set>

#include "cb/core/testing.hpp"

using namespace cb::invert::early_c_frame;

namespace {

struct Case {
    const char* name;
    std::vector<std::string> temps;
    std::vector<Local> locals;
    std::vector<Param> params;
    std::uint32_t frame_size;
    std::vector<Offset> referenced;   // the [bp+disp] offsets the compiled code used
};

Local I(const char* n, std::uint32_t count = 1) { return {n, "int", count}; }
Local L(const char* n) { return {n, "long", 1}; }
Local C(const char* n) { return {n, "char", 1}; }

const std::vector<Case>& cases() {
    static const std::vector<Case> kCases = {
        {"f01", {}, {I("a")}, {}, 2, {0}},
        {"f02", {}, {I("a"), I("b")}, {}, 4, {0, 2}},
        {"f03", {}, {I("a"), I("b"), I("c")}, {}, 6, {0, 2, 4}},
        {"f04", {}, {I("a"), I("b")}, {}, 4, {0, 2}},
        {"f08", {}, {I("a"), I("b"), I("c")}, {}, 6, {0, 4}},   // b unused
        {"f11", {}, {I("a"), I("b"), I("c"), I("d")}, {}, 8, {0, 2, 4, 6}},
        {"f12", {"int"}, {I("a"), I("b"), I("c")}, {{"p", "int"}}, 8, {2, 4, 6, 12}},
        {"f13", {"int"}, {I("a"), I("b")}, {}, 6, {2, 4}},
        {"f14", {"long"}, {I("a"), L("b")}, {{"p", "int"}}, 10, {4, 6, 8, 14}},
        {"f15", {"int"}, {I("a"), I("b")}, {}, 6, {2, 4}},
        {"g01", {"int"}, {I("a"), I("b")}, {}, 6, {2, 4}},
        {"g02", {"int", "int"}, {I("a"), I("b"), I("c")}, {}, 10, {4, 6, 8}},
        {"g03", {"long"}, {L("a"), L("b")}, {}, 12, {4, 6, 8, 10}},
        {"g04", {"long"}, {L("a"), I("b")}, {}, 10, {4, 6, 8}},
        {"g05", {}, {I("a"), L("b"), L("c")}, {}, 10, {0}},
        {"g06", {}, {C("a"), C("b")}, {}, 2, {0, 1}},
        {"g07", {}, {I("a"), C("b"), L("c")}, {}, 8, {0, 2, 3, 5}},
        {"g08", {}, {I("a", 3)}, {}, 6, {0}},
        {"g09", {}, {I("a"), I("b"), I("c")}, {}, 6, {0, 2, 4}},
        {"g10", {"int"}, {I("a")}, {{"p", "int"}}, 4, {2, 8}},
        {"g11", {"int"}, {I("a"), I("b"), I("c")}, {}, 8, {2, 4, 6}},
        {"g12", {"int"}, {I("a")}, {}, 4, {2}},
        {"g13", {"int"}, {I("a"), I("b")}, {}, 6, {2, 4}},
        {"g14", {"long"}, {L("a"), L("b"), L("c")}, {}, 16, {4, 6, 8, 10, 12, 14}},
        {"g15", {}, {I("a"), I("b"), I("c"), I("d"), I("e")}, {}, 10, {0, 2, 4, 6, 8}},
    };
    return kCases;
}

std::vector<Decl> decls(std::vector<Offset> offs, std::uint32_t fs, std::set<Offset> longs = {}, std::vector<Copy> copies = {}) {
    return emit_local_decls(offs, fs, longs, copies);
}
std::string text_of(const std::vector<Decl>& ds) {
    std::string out;
    for (const Decl& d : ds) out += (out.empty() ? "" : " ") + format_decl(d);
    return out;
}

}  // namespace

CB_TEST("invert.early_c_frame.type_sizes", [] {
    CB_CHECK_EQ(type_size("char"), 1u);
    CB_CHECK_EQ(type_size("unsigned char"), 1u);
    CB_CHECK_EQ(type_size("int"), 2u);
    CB_CHECK_EQ(type_size("unsigned"), 2u);
    CB_CHECK_EQ(type_size("long"), 4u);
    CB_CHECK_EQ(type_size("unsigned long"), 4u);
    CB_CHECK_EQ(type_size("float"), 4u);
    CB_CHECK_EQ(type_size("double"), 8u);
    CB_CHECK_EQ(type_size("pointer"), 2u);
    CB_CHECK_EQ(type_size("struct whatever"), 2u);   // unknown: two bytes
    CB_CHECK_EQ(type_size("long", 3), 12u);
    CB_CHECK_EQ(type_size("char", 0), 0u);
});

CB_TEST("invert.early_c_frame.ground_truth_frames", [] {
    for (const Case& c : cases()) {
        const Layout lay = layout(c.temps, c.locals, c.params);
        CB_CHECK_EQ(lay.frame_size, c.frame_size);
        // every offset the compiled code referenced is a slot the model predicts: a local's first word (and a long's or
        // float's second word, and each element of an array), or a parameter
        std::set<Offset> predicted;
        for (const Local& l : c.locals) {
            const Offset base = lay.local_offsets.at(l.name);
            for (std::uint32_t k = 0; k < l.count; ++k) {
                predicted.insert(base + k * type_size(l.type));
                if (l.type == "long" || l.type == "float") predicted.insert(base + k * type_size(l.type) + 2);
            }
        }
        for (const Param& p : c.params) predicted.insert(lay.param_offsets.at(p.name));
        for (const Offset d : c.referenced) CB_CHECK(predicted.contains(d));
    }
});

CB_TEST("invert.early_c_frame.layout_details", [] {
    // temps first, then locals in declaration order, no padding, the frame rounded to even, params above bp+retaddr
    const Layout lay = layout(std::vector<std::string>{"int", "long"}, std::vector<Local>{I("a"), C("b"), L("c")}, std::vector<Param>{{"p", "int"}, {"q", "long"}, {"r", "char"}});
    CB_CHECK_EQ(lay.temp_bytes, 6u);
    CB_CHECK_EQ(lay.local_offsets.at("a"), 6u);
    CB_CHECK_EQ(lay.local_offsets.at("b"), 8u);
    CB_CHECK_EQ(lay.local_offsets.at("c"), 9u);
    CB_CHECK_EQ(lay.frame_size, 14u);   // 13 rounded up
    CB_CHECK_EQ(lay.param_offsets.at("p"), 18u);
    CB_CHECK_EQ(lay.param_offsets.at("q"), 20u);
    CB_CHECK_EQ(lay.param_offsets.at("r"), 24u);
    CB_CHECK(lay.local_offsets.begin()->first == "a");   // declaration order
    const Layout empty = layout({}, {}, {});
    CB_CHECK_EQ(empty.frame_size, 0u);
    CB_CHECK(empty.local_offsets.empty());
});

CB_TEST("invert.early_c_frame.copy_chains", [] {
    // a=11; b=a; c=b: the chain a -> b -> c swaps its endpoints a and c; b stays
    CB_CHECK(chain_swaps(std::vector<Copy>{{.dest = 2, .src = 4}, {.dest = 0, .src = 2}}) ==
             (std::vector<std::pair<Offset, Offset>>{{4, 0}}));
    // a single copy swaps src and dest
    CB_CHECK(chain_swaps(std::vector<Copy>{{.dest = 6, .src = 2}}) == (std::vector<std::pair<Offset, Offset>>{{2, 6}}));
    // independent copies are separate chains
    CB_CHECK(chain_swaps(std::vector<Copy>{{.dest = 2, .src = 0}, {.dest = 6, .src = 4}}) ==
             (std::vector<std::pair<Offset, Offset>>{{0, 2}, {4, 6}}));
    CB_CHECK(chain_swaps({}).empty());
    // a cycle has no origin: nothing swaps; a chain that runs into one ends where it would re-enter
    CB_CHECK(chain_swaps(std::vector<Copy>{{.dest = 0, .src = 2}, {.dest = 2, .src = 0}}).empty());
    CB_CHECK(chain_swaps(std::vector<Copy>{{.dest = 2, .src = 8}, {.dest = 4, .src = 2}, {.dest = 2, .src = 4}}) ==
             (std::vector<std::pair<Offset, Offset>>{{8, 4}}));
});

CB_TEST("invert.early_c_frame.apply_and_infer", [] {
    const std::vector<Offset> offs = {0, 2, 4};
    const std::vector<Copy> chain = {{.dest = 2, .src = 4}, {.dest = 0, .src = 2}};
    const auto fwd = apply_copy_swaps(offs, chain);
    CB_CHECK_EQ(fwd.at(0), 4u);
    CB_CHECK_EQ(fwd.at(2), 2u);
    CB_CHECK_EQ(fwd.at(4), 0u);
    CB_CHECK(infer_decl_order(offs, chain) == (std::vector<Offset>{4, 2, 0}));
    CB_CHECK(infer_decl_order(offs, {}) == offs);
    // swaps naming offsets that are not declared change nothing
    CB_CHECK(infer_decl_order(offs, std::vector<Copy>{{.dest = 2, .src = 100}}) == offs);
    // duplicates in the offsets are one variable
    CB_CHECK(infer_decl_order(std::vector<Offset>{4, 0, 4, 2}, {}) == offs);
});

CB_TEST("invert.early_c_frame.local_declarations", [] {
    CB_CHECK(decls({}, 10).empty());
    // consecutive ints: no fillers
    CB_CHECK_EQ(text_of(decls({0, 2, 4}, 6)), "int v0; int v2; int v4;");
    // an unused local between two used ones, and unused space after the last: char filler arrays
    CB_CHECK_EQ(text_of(decls({0, 4}, 8)), "int v0; char _p0[2]; int v4; char _p4[2];");
    CB_CHECK_EQ(text_of(decls({2, 4}, 8)), "int v2; int v4; char _p4[2];");
    // longs by the set
    CB_CHECK_EQ(text_of(decls({4, 8}, 12, {4})), "long v4; int v8; char _p8[2];");
    CB_CHECK_EQ(text_of(decls({4, 8}, 12, {4, 8})), "long v4; long v8;");   // each long covers the next slot
    // unsorted, repeated offsets are sorted and deduplicated
    CB_CHECK_EQ(text_of(decls({4, 0, 4, 2}, 6)), "int v0; int v2; int v4;");
    // copy reversal with a contiguous layout: the inferred declaration order, no fillers
    CB_CHECK_EQ(text_of(decls({0, 2, 4}, 6, {}, {{.dest = 2, .src = 4}, {.dest = 0, .src = 2}})), "int v4; int v2; int v0;");
    // copies with a layout that has gaps are ignored
    CB_CHECK_EQ(text_of(decls({0, 4}, 6, {}, {{.dest = 0, .src = 4}})), "int v0; char _p0[2]; int v4;");
    // formatting
    CB_CHECK_EQ(format_decl({"char", "buf", 12}), "char buf[12];");
    CB_CHECK_EQ(format_decl({"unsigned", "n", 1}), "unsigned n;");
});
