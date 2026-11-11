#include "cb/decompile/ctree.hpp"

#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace ct = cb::decompile::ctree;
using ct::Node;
using std::string;

// DIVERGES from the Python module (the old harness agreed on all else: 3822 outputs over 34 snippets and every one of
// their nodes, replayed from tree-sitter's own trees):
//  - the parser is an argument of Source (Python imported tree-sitter inside the module);
//  - a query on a node that lacks a field the grammar always gives it (a binary_expression with no `operator`, a call with
//    no `function`/`arguments`) answers "not that construct" -- nullopt, false, no arguments -- where Python raised
//    AttributeError on None;
//  - replace() checks its span against the text (Python's slicing clamped), and overlapping edits are a UsageError.
//
// tree-sitter is not part of this tree, so these tests replay the trees the real tree-sitter C grammar gave for a set of
// decompiler-style snippets (testdata/ctree_trees.txt: per snippet the text, the masked text it was parsed from, and
// every node with its type, byte range, named flag, parent and field children).

namespace {

struct Recording {
    string text, masked;
    struct N {
        string type;
        bool named;
        std::uint32_t start, end;
        int parent;
    };
    std::vector<N> nodes;
    std::vector<std::tuple<std::size_t, string, std::size_t>> fields;
};

string unhex(const string& h) {
    string s;
    if (h == "-") return s;
    auto v = [](char c) { return c <= '9' ? c - '0' : c - 'a' + 10; };
    for (std::size_t i = 0; i + 1 < h.size(); i += 2) s.push_back(static_cast<char>(v(h[i]) * 16 + v(h[i + 1])));
    return s;
}

// masked text -> recording; loaded once for the test run
const std::map<string, Recording>& recordings() {
    static const std::map<string, Recording> all = [] {
        std::map<string, Recording> out;
        Recording cur;
        const string file = cb::fs::read_text(cb::testing::data_path("decompile/testdata/ctree_trees.txt"));
        for (std::string_view raw : cb::text::lines(file)) {
            if (raw.empty()) continue;
            std::vector<std::string_view> f = cb::text::split(raw, '\t');
            if (f[0] == "S") {
                cur = Recording();
                cur.text = unhex(string(f[2]));
                cur.masked = unhex(string(f[3]));
            } else if (f[0] == "N") {
                cur.nodes.push_back({unhex(string(f[2])), f[3] == "1", static_cast<std::uint32_t>(std::stoul(string(f[4]))),
                                     static_cast<std::uint32_t>(std::stoul(string(f[5]))), std::stoi(string(f[6]))});
            } else if (f[0] == "F") {
                cur.fields.emplace_back(std::stoull(string(f[1])), unhex(string(f[2])), std::stoull(string(f[3])));
            } else if (f[0] == "E") {
                out[cur.masked] = cur;
            }
        }
        return out;
    }();
    return all;
}

// The replayed parser: hands back the recording for exactly the masked text it is given.
ct::Tree replay(cb::ByteView masked) {
    auto it = recordings().find(string(cb::as_text(masked)));
    CB_ASSERT(it != recordings().end(), "no recorded tree for this masked text");
    const Recording& r = it->second;
    ct::Tree t;
    std::vector<Node*> nodes;
    for (const auto& n : r.nodes) {
        nodes.push_back(&t.add(n.type, n.start, n.end, n.named, n.parent >= 0 ? nodes[static_cast<std::size_t>(n.parent)] : nullptr));
    }
    for (const auto& [i, name, child] : r.fields) t.set_field(*nodes[i], name, *nodes[child]);
    return t;
}

// The Source of one of the recorded snippets.
struct Parsed {
    ct::Source src;
    explicit Parsed(const string& text) : src(text, replay) {}
};

string texts(const ct::Source& src, const std::vector<const Node*>& nodes) {
    string out;
    for (const Node* n : nodes) out += (out.empty() ? "" : " | ") + string(src.at(*n));
    return out;
}

}  // namespace

CB_TEST("decompile.ctree.mask_qualifiers", [] {
    using ct::mask_for_parser;
    CB_CHECK_EQ(mask_for_parser("void __cdecl near g(int __far *p, char __near *q)"),
                "void         near g(int       *p, char        *q)");
    // whole words only, in the lengths the grammar must not see: the text keeps every byte's place
    CB_CHECK_EQ(mask_for_parser("x__far __far_x __farx"), "x__far __far_x __farx");
    // (the prefixes __cdecl, __stdcall and __pascal take any word characters after them)
    CB_CHECK_EQ(mask_for_parser("__cdeclfoo __stdcall16 __pascal __fastcall __thiscall"), string(53, ' '));
    CB_CHECK_EQ(mask_for_parser("__huge __export __loadds __saveregs __interrupt _Packed int"), string(56, ' ') + "int");
    CB_CHECK_EQ(mask_for_parser("_Packed2 _packed __Far"), "_Packed2 _packed __Far");
    CB_CHECK_EQ(mask_for_parser("static void __pascal far fw(int a)").size(), string("static void __pascal far fw(int a)").size());
    CB_CHECK_EQ(mask_for_parser(""), "");
});

CB_TEST("decompile.ctree.mask_end_of_block_labels", [] {
    using ct::mask_for_parser;
    // a label that is the last thing in a block is read as `L;`
    CB_CHECK_EQ(mask_for_parser("{\n  foo(1, 2);\n done:\n}\n"), "{\n  foo(1, 2);\n done;\n}\n");
    CB_CHECK_EQ(mask_for_parser("{\n  L2:\n  }\n}"), "{\n  L2;\n  }\n}");
    CB_CHECK_EQ(mask_for_parser("{\n  if (x) { L1: }\n  else { L2:\n  }\n}"), "{\n  if (x) { L1: }\n  else { L2:\n  }\n}");   // not first on a line
    CB_CHECK_EQ(mask_for_parser("{\r\n  lbl :  \r\n}\r\n"), "{\r\n  lbl ;  \r\n}\r\n");
    // only the one next to the closing brace; labels before a statement are fine
    CB_CHECK_EQ(mask_for_parser("{\n  a:\n  b:\n  x = 1;\n  c: }\n"), "{\n  a:\n  b:\n  x = 1;\n  c; }\n");
    CB_CHECK_EQ(mask_for_parser("a:\n b:\n}"), "a:\n b;\n}");
    // case and default are not labels; a label must be the first thing on its line
    CB_CHECK_EQ(mask_for_parser("switch (x) {\n  case 1:\n    break;\n  default:\n  }\n"),
                "switch (x) {\n  case 1:\n    break;\n  default:\n  }\n");
    CB_CHECK_EQ(mask_for_parser("{ x = 1; L:\n}"), "{ x = 1; L:\n}");
    CB_CHECK_EQ(mask_for_parser("case:\n}"), "case:\n}");
    CB_CHECK_EQ(mask_for_parser("cases:\n}"), "cases;\n}");
    // something other than space between the colon and the brace
    CB_CHECK_EQ(mask_for_parser("{\n end: ;\n}"), "{\n end: ;\n}");
    CB_CHECK_EQ(mask_for_parser("{\n 1abc:\n}"), "{\n 1abc:\n}");
});

CB_TEST("decompile.ctree.function_facts", [] {
    Parsed fn("uint4 FUN_1000_0010(int x, uint2 y)\n{\n  uint4 local_c;\n  local_c = x + y;\n  return local_c;\n}\n");
    CB_CHECK_EQ(string(fn.src.at(*fn.src.function_name())), "FUN_1000_0010");
    CB_CHECK_EQ(string(fn.src.at(*fn.src.parameter_list())), "(int x, uint2 y)");
    auto params = fn.src.parameters();
    CB_CHECK(params && *params == std::vector<string>({"int x", "uint2 y"}));
    CB_CHECK_EQ(string(fn.src.at(*fn.src.body())).substr(0, 4), "{\n  ");
    CB_CHECK_EQ(string(fn.src.at(*fn.src.body())).back(), '}');
    // the qualifiers are masked for the parser but the text as written is what a node says
    Parsed far("void __cdecl near g(int __far *p, char __near *q)\n{\n  *p = 1;\n  return;\n}\n");
    auto far_params = far.src.parameters();
    CB_CHECK(far_params && far_params->size() == 2 && (*far_params)[0] == "int __far *p");
    CB_CHECK_EQ(string(far.src.at(*far.src.function_name())), "g");
    // a function that returns a pointer: its own parameter list, not the pointee's
    Parsed ptr("int *ptr_ret(int p) { return &p; }\n");
    CB_CHECK_EQ(string(ptr.src.at(*ptr.src.function_name())), "ptr_ret");
    CB_CHECK_EQ(string(ptr.src.at(*ptr.src.parameter_list())), "(int p)");
    // no function at all
    Parsed none("int x;");
    CB_CHECK(!none.src.function_name() && !none.src.parameter_list() && !none.src.body() && !none.src.parameters());
    Parsed empty("");
    CB_CHECK_EQ(empty.src.root().type, "translation_unit");
    CB_CHECK(empty.src.find({"identifier"}).empty());
    CB_CHECK_EQ(empty.src.text(), "");
    // a parameter list with a function pointer in it is split by the grammar, not at commas
    Parsed pointer("code *code_ptr;\nvoid cb(code *p) { (*p)(); }\n");
    auto cb_params = pointer.src.parameters();
    CB_CHECK(cb_params && cb_params->size() == 1);
    Parsed no_params("void f(void) { }");
    auto void_params = no_params.src.parameters();
    CB_CHECK(void_params && *void_params == std::vector<string>({"void"}));
});

CB_TEST("decompile.ctree.calls_and_arguments", [] {
    Parsed p("void call(void)\n{\n  uint4 v = (uint4)(x);\n  f(a, b, c);\n  g();\n  h(  1 ,2  );\n  (*fn)(1);\n  uint2 (*p);\n  i = (uint2)(uint1)x;\n}\n");
    auto calls = p.src.find({"call_expression"});
    CB_CHECK_EQ(texts(p.src, calls), "(uint4)(x) | f(a, b, c) | g() | h(  1 ,2  ) | (*fn)(1) | uint2 (*p)");
    // the grammar reads `(uint4)(x)` as a call and `uint2 (*p)` too; neither is one
    std::vector<bool> is_call;
    for (const Node* c : calls) is_call.push_back(p.src.is_call(*c));
    CB_CHECK(is_call == std::vector<bool>({false, true, true, true, true, false}));
    // arguments as written, with what precedes each
    auto args = [&](const Node* c) {
        std::vector<string> out;
        for (std::string_view a : p.src.raw_arguments(*c)) out.emplace_back(a);
        return out;
    };
    CB_CHECK(args(calls[1]) == std::vector<string>({"a", " b", " c"}));
    CB_CHECK(args(calls[2]).empty());
    CB_CHECK(args(calls[3]) == std::vector<string>({"  1 ", "2  "}));
    CB_CHECK(args(calls[0]) == std::vector<string>({"x"}));
    // the line a node is on
    CB_CHECK_EQ(string(p.src.line(*calls[1])), "  f(a, b, c);");
    auto span = p.src.line_span(*calls[1]);
    CB_CHECK_EQ(string(p.src.at(*calls[1])), "f(a, b, c)");
    CB_CHECK_EQ(p.src.original().substr(span.start, span.end - span.start), "  f(a, b, c);\n");
});

CB_TEST("decompile.ctree.casts_misread_as_operators", [] {
    Parsed p("int h(int a)\n{\n  int x = (uint4)*p;\n  y = (uint2)&x;\n  z = (uint2)(uint1)*p;\n  w = (xunknown1 [4])*r;\n  return x - (uint4)-y + (uint2)+z;\n}\n");
    // the grammar does not know uint4 is a type: `(uint4)*p` is a product to it. misread() tells which binary nodes are
    // really a cast followed by a unary operator, and unary() the operand's operator and where it starts
    std::vector<std::pair<string, string>> misread;
    for (const Node* n : p.src.find({"binary_expression"})) {
        if (auto op = p.src.misread(*n)) misread.emplace_back(string(p.src.at(*n)), string(*op));
    }
    CB_CHECK_EQ(misread.size(), 4u);
    CB_CHECK(misread[0] == std::make_pair(string("(uint4)*p"), string("*")));
    CB_CHECK(misread[1] == std::make_pair(string("(uint2)&x"), string("&")));
    CB_CHECK(misread[2] == std::make_pair(string("(uint2)(uint1)*p"), string("*")));
    CB_CHECK(misread[3] == std::make_pair(string("(xunknown1 [4])*r"), string("*")));
    // a real product is not one
    Parsed product("void arr(void)\n{\n  x = a[i] + b[i + 1][j];\n  p->q = r.s;\n  t = sizeof(int) + (int)u;\n  v = cond ? w : z;\n}\n");
    CB_CHECK(!product.src.find({"binary_expression"}).empty());
    for (const Node* n : product.src.find({"binary_expression"})) CB_CHECK(!product.src.misread(*n));
    // the operand p of `(uint4)*p`: operator `*` at byte 32 (counted in the text)
    const Node* operand = nullptr;
    for (const Node* n : p.src.find({"identifier"})) {
        if (p.src.at(*n) == "p") {
            operand = n;
            break;
        }
    }
    CB_CHECK(operand != nullptr);
    auto u = p.src.unary(*operand);
    CB_CHECK(u && u->op == "*" && u->start == 32);
    CB_CHECK_EQ(p.src.original().substr(32, 1), "*");
    // the cast leads `(uint4)*p`: leading() finds the outermost node of a kind that starts where the node does
    const Node* first = p.src.find({"binary_expression"})[0];
    CB_CHECK(p.src.leading(*first, "binary_expression") == first);
    CB_CHECK(p.src.leading(*first, "if_statement") == nullptr);
    for (const Node* n : p.src.find({"binary_expression"})) {
        if (p.src.at(*n).starts_with("x - (uint4)-y")) {          // `x` is the first thing in the whole sum
            const Node* lead = p.src.leading(*n, "identifier");
            CB_CHECK(lead != nullptr && p.src.at(*lead) == "x");
        }
    }
});

CB_TEST("decompile.ctree.text_is_not_code", [] {
    Parsed p("void s(void)\n{\n  puts(\"a.b._1_2_ x p\");\n  c = 'x';\n  /* x p */\n  // x p\n  x = p;\n}\n");
    // strings, characters and comments are not code, wherever the cursor is in them
    for (const Node* n : p.src.find({"string_literal", "comment"})) CB_CHECK(ct::Source::in_text(*n));
    int in_text_nodes = 0, code_identifiers = 0;
    for (const Node* n : p.src.find({"identifier"})) {
        CB_CHECK(!ct::Source::in_text(*n));
        ++code_identifiers;
    }
    for (const Node* n : p.src.find({"string_content", "character"})) in_text_nodes += ct::Source::in_text(*n);
    CB_CHECK(in_text_nodes >= 2);
    CB_CHECK_EQ(code_identifiers, 5);   // s, puts, c, x, p: what is in the string and the comments is not an identifier
    // a pass that renames identifiers touches `x = p;` and nothing in the strings or comments
    ct::Source& src = p.src;
    for (const Node* n : src.find({"identifier"})) {
        if (src.at(*n) == "x") src.replace(*n, "X");
        if (src.at(*n) == "p") src.replace(*n, "P");
    }
    CB_CHECK_EQ(src.text(), "void s(void)\n{\n  puts(\"a.b._1_2_ x p\");\n  c = 'x';\n  /* x p */\n  // x p\n  X = P;\n}\n");
    CB_CHECK_EQ(src.edit_count(), 2u);
    CB_CHECK_EQ(src.original(), "void s(void)\n{\n  puts(\"a.b._1_2_ x p\");\n  c = 'x';\n  /* x p */\n  // x p\n  x = p;\n}\n");
});

CB_TEST("decompile.ctree.edits", [] {
    const string text = "void s(void)\n{\n  puts(\"a.b._1_2_ x p\");\n  c = 'x';\n  /* x p */\n  // x p\n  x = p;\n}\n";
    // inserts at one place keep the order they were made in; a replaced span and an insert before it combine
    Parsed p(text);
    p.src.insert(0, "// head\n");
    p.src.insert(0, "// head2\n");
    const Node* body = p.src.body();
    p.src.replace(ct::Source::Span{body->start_byte + 1, body->start_byte + 2}, "");   // drop the newline after `{`
    CB_CHECK_EQ(p.src.text().substr(0, 41), "// head\n// head2\nvoid s(void)\n{  puts(\"a.");
    // edits made out of order are applied in order
    Parsed q(text);
    const Node* name = q.src.function_name();
    q.src.insert(static_cast<std::uint32_t>(text.size()), "// end\n");
    q.src.replace(*name, "renamed");
    CB_CHECK_EQ(q.src.text(), "void renamed(void)\n{\n  puts(\"a.b._1_2_ x p\");\n  c = 'x';\n  /* x p */\n  // x p\n  x = p;\n}\n// end\n");
    // overlapping edits are the pass's bug
    Parsed r(text);
    r.src.replace(*r.src.body(), "{}");
    const Node* inside = nullptr;
    for (const Node* n : r.src.find({"identifier"}, r.src.body())) {
        inside = n;
        break;
    }
    CB_CHECK(inside != nullptr);
    r.src.replace(*inside, "q");
    CB_CHECK_THROWS_AS(cb::UsageError, (void)r.src.text());
    // a span outside the text is refused when it is recorded
    Parsed s(text);
    CB_CHECK_THROWS_AS(cb::UsageError, s.src.replace(ct::Source::Span{0, static_cast<std::uint32_t>(text.size() + 1)}, "x"));
    CB_CHECK_THROWS_AS(cb::UsageError, s.src.insert(static_cast<std::uint32_t>(text.size() + 1), "x"));
    CB_CHECK_THROWS_AS(cb::UsageError, s.src.replace(ct::Source::Span{5, 2}, "x"));
    CB_CHECK_NOTHROW(s.src.insert(static_cast<std::uint32_t>(text.size()), "x"));
    // no edits: the text comes back as it was
    Parsed t(text);
    CB_CHECK_EQ(t.src.text(), text);
});

CB_TEST("decompile.ctree.rewrite", [] {
    const string text = "int a = 1;\nint f1(void) { return a; }\nint f2(void) { return f1(); }\n";
    string out = ct::rewrite(text, replay, [](ct::Source& s) {
        for (const Node* n : s.find({"number_literal"})) s.replace(*n, "Z");
    });
    CB_CHECK_EQ(out, "int a = Z;\nint f1(void) { return a; }\nint f2(void) { return f1(); }\n");
    CB_CHECK_EQ(ct::rewrite(text, replay, [](ct::Source&) {}), text);
    // unparsable text still keeps every byte's place
    const string junk = "}}{{ )( ;;; int int ((";
    Parsed p(junk);
    CB_CHECK(!p.src.find({"ERROR"}).empty());
    CB_CHECK_EQ(p.src.text(), junk);
});

CB_TEST("decompile.ctree.declarations_and_rendering", [] {
    Parsed p("void decls(void)\n{\n  int a, *b, c[4], (*d)(void);\n  char e = 1, *f = 0;\n  unsigned long g;\n}\n");
    std::vector<string> names;
    for (const Node* n : p.src.find({"declaration"})) {
        for (const Node* c : n->named_children()) {
            if (auto name = p.src.declared_name(*c)) names.emplace_back(*name);
        }
    }
    // through pointers, arrays, parentheses and initialisers
    // (`(*d)(void)` is a function declarator: the grammar's own node for what the name belongs to, not asked here)
    CB_CHECK(names == std::vector<string>({"a", "b", "c", "e", "f", "g"}));
    const auto parens = p.src.find({"parenthesized_declarator"});
    CB_CHECK(parens.size() == 1 && p.src.declared_name(*parens[0]) == "d");
    // render: the text of a node with replacements inside it, the rest as written
    Parsed q("void s(void)\n{\n  puts(\"a.b._1_2_ x p\");\n  c = 'x';\n  /* x p */\n  // x p\n  x = p;\n}\n");
    const Node* body = q.src.body();
    string rendered = q.src.render(*body, [&](const Node& c) -> std::optional<string> {
        if (c.type == "identifier" && (q.src.at(c) == "x" || q.src.at(c) == "p")) return "<" + string(q.src.at(c)) + ">";
        if (c.type == "string_literal") return string("\"\"");
        return std::nullopt;
    });
    CB_CHECK_EQ(rendered, "{\n  puts(\"\");\n  c = 'x';\n  /* x p */\n  // x p\n  <x> = <p>;\n}");
    // ancestors, nearest first, optionally of some types
    const Node* id = nullptr;
    for (const Node* n : q.src.find({"identifier"})) {
        if (q.src.at(*n) == "puts") id = n;
    }
    CB_CHECK(id != nullptr);
    auto up = q.src.ancestors(*id);
    CB_CHECK_EQ(up.front()->type, "call_expression");
    CB_CHECK_EQ(up.back()->type, "translation_unit");
    auto only = q.src.ancestors(*id, {"compound_statement", "function_definition"});
    CB_CHECK_EQ(only.size(), 2u);
    CB_CHECK_EQ(only[0]->type, "compound_statement");
    CB_CHECK_EQ(only[1]->type, "function_definition");
});

CB_TEST("decompile.ctree.crlf_lines", [] {
    Parsed p("void crlf(void)\r\n{\r\n  x = 1;\r\n  lbl:\r\n}\r\n");
    const Node* x = nullptr;
    for (const Node* n : p.src.find({"identifier"})) {
        if (p.src.at(*n) == "x") x = n;
    }
    CB_CHECK(x != nullptr);
    CB_CHECK_EQ(string(p.src.line(*x)), "  x = 1;");   // the line end (\r\n) is not part of it
    auto span = p.src.line_span(*x);
    CB_CHECK_EQ(p.src.original().substr(span.start, span.end - span.start), "  x = 1;\r\n");
    // the label at the end of the block was read as a statement; the text is still as written
    CB_CHECK_EQ(p.src.text(), "void crlf(void)\r\n{\r\n  x = 1;\r\n  lbl:\r\n}\r\n");
});

CB_TEST("decompile.ctree.parser_must_cover_the_text", [] {
    // an empty tree is the parser's bug, reported when the Source is made
    ct::Parser empty = [](cb::ByteView) { return ct::Tree(); };
    CB_CHECK_THROWS_AS(cb::InternalError, ct::Source("int x;", empty));
    // the parser is given the masked bytes, nothing else
    string seen;
    ct::Parser spy = [&](cb::ByteView masked) {
        seen = string(cb::as_text(masked));
        ct::Tree t;
        t.add("translation_unit", 0, static_cast<std::uint32_t>(masked.size()), true);
        return t;
    };
    ct::Source src("int __far x;\nL:\n}", spy);
    CB_CHECK_EQ(seen, "int       x;\nL;\n}");
    CB_CHECK_EQ(string(src.at(src.root())), "int __far x;\nL:\n}");
});
