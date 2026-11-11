#include "cb/decompile/l2tree.hpp"

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"

namespace t = cb::decompile::l2tree;
using cb::Json;
using std::string;

// DIVERGES from the Python module (decompile/l2tree did not exist there: frame16, shape16 and asm_blocks each carried a
// copy of _walk, _lists and _at): the tree is a Json, edited in place; origin offsets are reduced to 16 bits and sorted.

CB_TEST("decompile.l2tree.member_text_integer", [] {
    const Json node = cb::json::parse(R"({"node":"Name","n":7,"s":"text","z":null,"f":1.5,"o":{"a":1},"l":[]})");
    CB_CHECK(t::member(node, "o") != nullptr);
    CB_CHECK(t::member(node, "z") == nullptr);        // null is as good as absent
    CB_CHECK(t::member(node, "missing") == nullptr);
    CB_CHECK(t::member(static_cast<const Json*>(nullptr), "x") == nullptr);
    CB_CHECK(t::member(*t::member(node, "l"), "x") == nullptr);   // not an object
    CB_CHECK_EQ(string(t::text(&node, "s")), "text");
    CB_CHECK_EQ(string(t::text(&node, "n")), "");     // not a string
    CB_CHECK_EQ(string(t::text(&node, "missing")), "");
    CB_CHECK_EQ(t::integer(&node, "n").value(), 7);
    CB_CHECK(!t::integer(&node, "f").has_value());
    CB_CHECK(!t::integer(&node, "s").has_value());
    CB_CHECK(t::is_kind(node, "Name"));
    CB_CHECK(!t::is_kind(node, "Local"));
    CB_CHECK(!t::is_kind(cb::json::parse("[]"), "Name"));
    CB_CHECK(!t::is_kind(static_cast<const Json*>(nullptr), "Name"));
});

CB_TEST("decompile.l2tree.present", [] {
    // what `x or default` tests in the decompiler's own notes
    CB_CHECK(!t::present(nullptr));
    for (const char* empty : {"null", "false", "0", "0.0", R"("")", "[]", "{}"}) {
        const Json j = cb::json::parse(empty);
        CB_CHECK(!t::present(&j));
    }
    for (const char* full : {"true", "1", "-1", "0.5", R"("x")", "[0]", R"({"a":0})"}) {
        const Json j = cb::json::parse(full);
        CB_CHECK(t::present(&j));
    }
});

CB_TEST("decompile.l2tree.walk", [] {
    // a node before its members, depth first, lists by index; walking reads the members after the callback ran, so a node
    // the callback replaces is walked as replaced
    Json tree = cb::json::parse(R"({"node":"A","x":{"node":"B","y":[{"node":"C"},[{"node":"D"}]]},"z":{"node":"E"},"s":"leaf","n":3})");
    std::vector<string> order;
    t::walk(tree, [&](Json& n) { order.push_back(string(t::text(&n, "node"))); });
    CB_CHECK_EQ(nlohmann::json(order).dump(), R"(["A","B","C","D","E"])");

    std::vector<string> seen;
    t::walk(tree, [&](Json& n) {
        if (t::is_kind(n, "B")) n = cb::json::parse(R"({"node":"B2","k":{"node":"F"}})");
        seen.push_back(string(t::text(&n, "node")));
    });
    CB_CHECK_EQ(nlohmann::json(seen).dump(), R"(["A","B2","F","E"])");

    const Json constant = cb::json::parse(R"([{"node":"P","q":{"node":"Q"}},{"node":"R"}])");
    const auto nodes = t::nodes(constant);
    CB_CHECK_EQ(nodes.size(), static_cast<std::size_t>(3));
    CB_CHECK_EQ(string(t::text(nodes[1], "node")), "Q");
    CB_CHECK(t::nodes(cb::json::parse("3")).empty());
});

CB_TEST("decompile.l2tree.statement_lists", [] {
    // the lists under body / then / orelse / stmts, a list before the lists inside it; the document's own body is one
    Json doc = cb::json::parse(R"({"body":[
        {"node":"If","cond":{"body":"not a list"},"then":[{"node":"While","body":[{"node":"X"}]}],"orelse":[{"node":"Y"}]},
        {"node":"Block","stmts":[{"node":"Z","then":{"node":"bare"}}]},
        {"node":"Local","other":[{"node":"W","body":[]}]}]})");
    std::vector<string> lists;
    t::for_each_statement_list(doc, [&](Json& list) { lists.push_back(std::to_string(list.size())); });
    CB_CHECK_EQ(nlohmann::json(lists).dump(), R"(["3","1","1","1","1","0"])");

    // the callback may rewrite the list it gets: the lists inside are read afterwards, as rewritten
    Json nested = cb::json::parse(R"({"body":[{"node":"A","body":[{"node":"B","body":[]}]}]})");
    int count = 0;
    t::for_each_statement_list(nested, [&](Json& list) {
        ++count;
        if (list.size() == 1 && t::is_kind(list[0], "A")) list = cb::json::parse(R"([{"node":"C","body":[]},{"node":"D","body":[]}])");
    });
    CB_CHECK_EQ(count, 3);   // the document's body, then C's and D's (A and B are gone)
});

CB_TEST("decompile.l2tree.origin_offsets", [] {
    const Json stmt = cb::json::parse(R"({"origin":{"at":[20,3,65539,3,"x",null,2.5,-1]}})");
    // 16-bit offsets, sorted, repeats dropped (65539 = $10003 reduces to 3; -1 to $FFFF); non-integers are not addresses
    CB_CHECK_EQ(nlohmann::json(t::origin_offsets(stmt)).dump(), "[3,20,65535]");
    CB_CHECK_EQ(nlohmann::json(t::origin_offsets(stmt, true)).dump(), "[3,3,3,20,65535]");
    CB_CHECK(t::origin_offsets(cb::json::parse(R"({"origin":{}})")).empty());
    CB_CHECK(t::origin_offsets(cb::json::parse(R"({"origin":{"at":"x"}})")).empty());
    CB_CHECK(t::origin_offsets(cb::json::parse("{}")).empty());
    CB_CHECK(t::origin_offsets(cb::json::parse("[1]")).empty());
});
