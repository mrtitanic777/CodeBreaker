#include "cb/decompile/struct_text.hpp"

#include <map>
#include <string>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

namespace st = cb::decompile::struct_text;
using st::Access;
using st::Root;
using st::RootKind;

// DIVERGES from the Python module (the old harness agreed on all else: 261 cases, compared as JSON so that the
// plan's key order does not count):
//  - the plan is ordered by name (function, parameter, offset), not by the order the analysis produced things;
//  - declarations() orders structures that end in the same number (or in none) by name -- Python left that to the
//    iteration order of a set -- and a name without an underscore sorts first instead of raising IndexError;
//  - a structure asked for in declarations() that the plan has no fields for is a UsageError (Python: KeyError);
//  - an "arg" root without an index is ignored (Python: IndexError); roots of any other kind were always ignored.

namespace {

// A scripted stand-in for the three collaborators: the module's structures, each function's parameter names, and the
// accesses through each (function, parameter).
struct Script {
    std::vector<st::Structure> structures;
    std::map<std::string, std::vector<std::string>> params;
    std::map<std::pair<std::string, std::string>, std::vector<Access>> accesses;
    bool fail = false;

    st::Analysis analysis() const {
        st::Analysis a;
        a.recover = [this](std::string_view) -> std::optional<std::vector<st::Structure>> {
            if (fail) return std::nullopt;
            return structures;
        };
        a.parameter_names = [this](const std::string& fn) { return params.at(fn); };
        a.struct_accesses = [this](const std::string& fn, const std::string& p) {
            auto it = accesses.find({fn, p});
            return it == accesses.end() ? std::vector<Access>() : it->second;
        };
        return a;
    }
};

Root arg(std::uint64_t va, std::int64_t index) { return {RootKind::Arg, va, index}; }
Root self(std::uint64_t va) { return {RootKind::This, va, std::nullopt}; }

}  // namespace

CB_TEST("decompile.struct_text.kind", [] {
    auto k = st::kind("int4", 0);
    CB_CHECK(k && k->type == "int4" && k->size == 4);
    k = st::kind("uint1", 1);   // one star: the pointee's own type is read, not a pointer
    CB_CHECK(k && k->type == "uint1" && k->size == 1);
    k = st::kind("float8", 0);
    CB_CHECK(k && k->size == 8);
    k = st::kind("undefined", 0);
    CB_CHECK(k && k->size == 1);
    CB_CHECK(!st::kind("foo", 0));
    CB_CHECK(!st::kind("struct_3", 1));
    CB_CHECK(!st::kind("code", 1));
    k = st::kind("code", 2);   // a pointer to a pointer: a four-byte field written "code *"
    CB_CHECK(k && k->type == "code *" && k->size == 4);
    k = st::kind("int4", 4);
    CB_CHECK(k && k->type == "int4 ***" && k->size == 4);
    k = st::kind("", 2);
    CB_CHECK(k && k->type == " *");
    CB_CHECK(!st::kind("", 0));
});

CB_TEST("decompile.struct_text.plan_votes_and_padding", [] {
    Script s;
    s.structures = {{"struct_1", {arg(0x1000, 0)}}};
    s.params = {{"f", {"a1", "a2"}}, {"g", {"a1"}}};
    // offset 0 read as int4; offset 8 read once as int4 and once as uint1 -> the tie goes to the greater type name;
    // `code` accesses are not scalar fields and add no vote
    s.accesses[{"f", "a1"}] = {{"int4", 0, 8}, {"int4", 0, 0}, {"uint1", 0, 8}, {"code", 1, 0x10}};
    s.accesses[{"g", "a1"}] = {{"int4", 0, 8}};   // g is not a root: its accesses do not count
    st::Plan p = st::plan("x.dll", {"f", "g"}, {{0x1000, "f"}, {0x2000, "g"}},
                          {{"f", "int4 f(int4 a1,int4 a2)"}, {"g", "int4 g(int4 a1)"}}, s.analysis());
    CB_CHECK_EQ(p.size(), 1u);
    CB_CHECK_EQ(p.at("f").size(), 1u);
    const st::PlannedParam& a1 = p.at("f").at("a1");
    CB_CHECK_EQ(a1.structure, "struct_1");
    CB_CHECK_EQ(a1.fields.size(), 2u);
    CB_CHECK_EQ(a1.fields.at(0).type, "int4");
    CB_CHECK_EQ(a1.fields.at(8).type, "uint1");
    CB_CHECK_EQ(st::declarations(p, {"struct_1"}),
                "typedef struct struct_1 {\n"
                "  int4 f_0;\n"
                "  uint1 _gap_4[4];\n"
                "  uint1 f_8;\n"
                "} struct_1;\n");
});

CB_TEST("decompile.struct_text.plan_registers_and_this", [] {
    // a __thiscall function has `this` first in its list, before the stack arguments counted from 0
    Script s;
    s.structures = {{"struct_9", {self(1)}}};
    s.params = {{"f", {"this_", "a1"}}};
    s.accesses[{"f", "this_"}] = {{"int4", 0, 0}, {"int2", 0, 4}, {"int4", 2, 8}};
    st::Plan p = st::plan("x", {"f"}, {{1, "f"}}, {{"f", "int4 __thiscall f(int4 a1)"}}, s.analysis());
    CB_CHECK_EQ(p.at("f").at("this_").structure, "struct_9");
    CB_CHECK_EQ(st::declarations(p, {"struct_9"}),
                "typedef struct struct_9 {\n"
                "  int4 f_0;\n"
                "  int2 f_4;\n"
                "  uint1 _gap_6[2];\n"
                "  int4 *f_8;\n"
                "} struct_9;\n");
    // the same `this` root on a function that is not a register-convention one: no parameter is the object
    s.structures = {{"struct_9", {self(1)}}};
    s.params = {{"f", {"a1"}}};
    s.accesses.clear();
    s.accesses[{"f", "a1"}] = {{"int4", 0, 0}, {"int4", 0, 4}};
    CB_CHECK(st::plan("x", {"f"}, {{1, "f"}}, {{"f", "int4 f(int4 a1)"}}, s.analysis()).empty());
    // an argument root on a __fastcall function counts from the argument after the register one
    Script t;
    t.structures = {{"struct_2", {arg(1, 0), arg(1, 1)}}};
    t.params = {{"f", {"ecx", "a1", "a2"}}};
    t.accesses[{"f", "ecx"}] = {{"int4", 0, 0}, {"int4", 0, 4}};
    t.accesses[{"f", "a1"}] = {{"int2", 0, 0}, {"int2", 0, 2}};
    t.accesses[{"f", "a2"}] = {{"int8", 0, 0}, {"int8", 0, 8}};
    st::Plan q = st::plan("x", {"f"}, {{1, "f"}}, {{"f", "int4 __fastcall f(int4 a1, int4 a2)"}}, t.analysis());
    // roots arg:0 -> a1 (index 0 + 1) and arg:1 -> a2 (1 + 1); the register argument itself is no root
    CB_CHECK(!q.at("f").contains("ecx"));
    CB_CHECK(q.at("f").contains("a1") && q.at("f").contains("a2"));
    CB_CHECK_EQ(q.at("f").at("a1").structure, "struct_2");
    // offset 0: int2 (a1) once, int8 (a2) once -> the tie goes to the greater name, int8; int2@2 then lies inside it
    const st::Fields& f2 = q.at("f").at("a1").fields;
    CB_CHECK_EQ(f2.size(), 2u);
    CB_CHECK_EQ(f2.at(0).type, "int8");
    CB_CHECK_EQ(f2.at(0).size, 8u);
    CB_CHECK_EQ(f2.at(8).type, "int8");
});

CB_TEST("decompile.struct_text.plan_requires_two_fields_and_known_functions", [] {
    Script s;
    s.structures = {{"struct_3", {arg(1, 0)}}};
    s.params = {{"f", {"a1"}}};
    s.accesses[{"f", "a1"}] = {{"int8", 0, 0}, {"foo", 0, 4}};   // one usable field: no structure
    CB_CHECK(st::plan("x", {"f"}, {{1, "f"}}, {}, s.analysis()).empty());
    // a root naming a function with no document, or an address with no name, plans nothing
    s.accesses[{"f", "a1"}] = {{"int4", 0, 0}, {"int4", 0, 4}};
    CB_CHECK(st::plan("x", {}, {{1, "f"}}, {}, s.analysis()).empty());
    CB_CHECK(st::plan("x", {"f"}, {}, {}, s.analysis()).empty());
    CB_CHECK(!st::plan("x", {"f"}, {{1, "f"}}, {}, s.analysis()).empty());
    // an argument index past the parameter list, or negative, is no parameter
    s.structures = {{"struct_5", {arg(1, 5), arg(1, -1)}}};
    CB_CHECK(st::plan("x", {"f"}, {{1, "f"}}, {}, s.analysis()).empty());
    // a root of another kind, or an argument root without an index, is ignored
    s.structures = {{"struct_5", {Root{RootKind::Other, 1, 0}, Root{RootKind::Arg, 1, std::nullopt}}}};
    CB_CHECK(st::plan("x", {"f"}, {{1, "f"}}, {}, s.analysis()).empty());
    // no structures at all
    s.structures.clear();
    CB_CHECK(st::plan("x", {"f"}, {{1, "f"}}, {}, s.analysis()).empty());
});

CB_TEST("decompile.struct_text.plan_analysis_failure_is_empty", [] {
    Script s;
    s.structures = {{"struct_1", {arg(1, 0)}}};
    s.params = {{"f", {"a1"}}};
    s.accesses[{"f", "a1"}] = {{"int4", 0, 0}, {"int4", 0, 4}};
    CB_CHECK(!st::plan("x", {"f"}, {{1, "f"}}, {}, s.analysis()).empty());
    s.fail = true;
    CB_CHECK(st::plan("x", {"f"}, {{1, "f"}}, {}, s.analysis()).empty());
});

CB_TEST("decompile.struct_text.plan_shared_structure_and_first_owner", [] {
    // one structure reached through arguments of two functions: all their accesses vote; a parameter claimed by
    // the first structure listed keeps it
    Script s;
    s.structures = {{"struct_5", {arg(1, 0), self(2), arg(3, 0)}}, {"struct_6", {arg(3, 0), arg(1, 1)}}};
    s.params = {{"f", {"a1", "a2"}}, {"g", {"p"}}, {"h", {"q"}}};
    s.accesses[{"f", "a1"}] = {{"int4", 0, 0}, {"int4", 0, 4}};
    s.accesses[{"g", "p"}] = {{"int4", 0, 0}, {"int2", 0, 4}};
    s.accesses[{"h", "q"}] = {{"int1", 0, 0}, {"int1", 0, 1}};
    s.accesses[{"f", "a2"}] = {{"int8", 0, 0}, {"int8", 0, 8}};
    st::Plan p = st::plan("x", {"f", "g", "h"}, {{1, "f"}, {2, "g"}, {3, "h"}}, {{"g", "int __thiscall g()"}},
                          s.analysis());
    CB_CHECK_EQ(p.at("f").at("a1").structure, "struct_5");
    CB_CHECK_EQ(p.at("g").at("p").structure, "struct_5");
    CB_CHECK_EQ(p.at("h").at("q").structure, "struct_5");   // struct_6 also named h's q but came second
    CB_CHECK_EQ(p.at("f").at("a2").structure, "struct_6");
    // struct_5's field at 0: int4 (f.a1, g.p) twice vs int1 (h.q) once -> int4; at 4: int4 once vs int2 once -> a tie,
    // which goes to the greater name ("int4" > "int2"); int1@1 lies inside the int4 at 0
    const st::Fields& f5 = p.at("f").at("a1").fields;
    CB_CHECK_EQ(f5.at(0).type, "int4");
    CB_CHECK_EQ(f5.at(4).type, "int4");
    CB_CHECK_EQ(f5.size(), 2u);   // int1@1 lies inside the int4 at 0
    // two structures in one declarations() call come out ordered by their number, not by name or plan order
    CB_CHECK_EQ(st::declarations(p, {"struct_6", "struct_5"}),
                "typedef struct struct_5 {\n  int4 f_0;\n  int4 f_4;\n} struct_5;\n"
                "typedef struct struct_6 {\n  int8 f_0;\n  int8 f_8;\n} struct_6;\n");
});

CB_TEST("decompile.struct_text.declarations_layout", [] {
    st::Plan p;
    p["f"]["a1"] = {"struct_2", {{0, {"int4", 4}}, {8, {"int4 *", 4}}, {12, {"uint1", 1}}}};
    p["g"]["a1"] = {"struct_10", {{3, {"int2", 2}}, {0x20, {"code **", 4}}}};
    // numeric order (2 before 10), gaps as padding, pointer stars written against the name
    CB_CHECK_EQ(st::declarations(p, {"struct_10", "struct_2"}),
                "typedef struct struct_2 {\n"
                "  int4 f_0;\n"
                "  uint1 _gap_4[4];\n"
                "  int4 *f_8;\n"
                "  uint1 f_c;\n"
                "} struct_2;\n"
                "typedef struct struct_10 {\n"
                "  uint1 _gap_0[3];\n"
                "  int2 f_3;\n"
                "  uint1 _gap_5[27];\n"
                "  code **f_20;\n"
                "} struct_10;\n");
    CB_CHECK_EQ(st::declarations(p, {}), "\n");   // no structures: just the line end
    CB_CHECK_THROWS_AS(cb::UsageError, (void)st::declarations(p, {"struct_2", "struct_3"}));
    // a negative offset prints as such
    st::Plan q;
    q["f"]["a1"] = {"s_100", {{-8, {"int4", 4}}, {0x1f, {"uint8", 8}}}};
    CB_CHECK_EQ(st::declarations(q, {"s_100"}),
                "typedef struct s_100 {\n"
                "  int4 f_-8;\n"
                "  uint1 _gap_-4[35];\n"
                "  uint8 f_1f;\n"
                "} s_100;\n");
    // names ending in a number sort by it; others (no digits, no underscore) first, among themselves by name
    st::Plan r;
    r["f"]["a1"] = {"k_99999999999", {{0, {"int1", 1}}}};
    r["f"]["a2"] = {"a_b_12", {{0, {"int4", 4}}}};
    r["f"]["a3"] = {"struct_x", {{0, {"int4", 4}}}};
    r["f"]["a4"] = {"z_3", {{0, {"int1", 1}}}};
    r["f"]["a5"] = {"s_1", {{0, {"int4 ***", 4}}}};
    std::string out = st::declarations(r, {"a_b_12", "z_3", "k_99999999999", "s_1", "struct_x"});
    auto at = [&](const char* name) { return out.find(std::string("typedef struct ") + name + " "); };
    CB_CHECK(at("struct_x") < at("s_1") && at("s_1") < at("z_3") && at("z_3") < at("a_b_12") &&
             at("a_b_12") < at("k_99999999999"));
});
