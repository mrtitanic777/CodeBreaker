// Tests of the port's mending (mend.hpp) and of the passes that work on the whole program (stored_wide, passed_on,
// union_member_for, visible_tags). The golden tests next to this file (mend_golden_test.cpp) hold the same code to what
// the Python gave over two real programs and a dozen small ones; these read as the rules they are.
//
// DIVERGES from the Python (pre_ansi.py), each on purpose:
//   * stored_wide, passed_on and whole_word_stores no longer raise IndexError for a definition whose braces never close
//     (a truncated file); they read it to the last line. Fixed in the Python too (tests/test_port.py).
//   * stored_wide reads each file's text afresh: the Python took the texts once at the start, so a file mended after an
//     earlier file had changed its declarations wrote the old text back over them. Fixed in the Python too.
//   * object_before with an unbalanced bracket (`a) ->x`) starts the object at the line's start; the Python indexed from
//     the end of the line.
//   * the notes and rules for pointers to functions (the last loop of mend) come in the order of the pointers' names, the
//     Python's in the order the pointers were first met; no output reads that order.
//   * a program with no library functions named has no "given to a library function" use of a parameter; the Python
//     built an empty alternative that matched any call.
//   * text is bytes: the Python's code-point offsets are byte offsets here (a Latin-1 program is one byte a character).
#include "cb/port/mend.hpp"

#include "cb/core/regex.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::port;

namespace {

Function fn(std::string name, std::string ret, std::vector<Param> params, std::string body = "") {
    Function f;
    f.name = std::move(name);
    f.ret = std::move(ret);
    f.params = std::move(params);
    f.body = std::move(body);
    f.file = "a.c";
    return f;
}

MendState state_of(std::vector<Function> functions, Sources files = {}) {
    MendState st;
    for (Function& f : functions)
        st.functions[f.name] = std::move(f);
    st.files = std::move(files);
    st.libc = {"strlen", "strcpy", "printf", "fopen"};
    return st;
}

Diag diag(std::string file, std::size_t line, std::size_t col, std::string text, std::vector<std::string> notes = {}) {
    Diag d;
    d.file = std::move(file);
    d.line = line;
    d.col = col;
    d.text = std::move(text);
    d.notes = std::move(notes);
    return d;
}

std::size_t reasons(const MendState& st, std::string_view starts) {
    for (const auto& [why, where] : st.mended) {
        if (why.starts_with(starts))
            return where.size();
    }
    return 0;
}

const std::string& fixed(const MendState& st, const std::string& file, const std::string& original) {
    return st.line_fixes.at(LineFixKey{file, original, -1});
}

} // namespace

CB_TEST("port.mend.a_cast_on_the_line", [] {
    // an int given for a pointer parameter, and a pointer given to a pointer of another type: casts, kept as line fixes
    Sources out;
    out["a.c"] = "char *buf;\nint16_t f_first(char *s, int16_t n);\nvoid go(void)\n{\n    f_first(count, 2);\n    head "
                 "= buf;\n}\n";
    MendState st = state_of({fn("f_first", "int16_t", {{"s", "char *s"}, {"n", "int16_t n"}})}, out);
    const std::vector<Diag> diags = {
        diag("a.c", 5, 13, "passing argument 1 of 'f_first' makes pointer from integer without a cast",
             {"expected 'char *' but argument is of type 'int'"}),
        diag("a.c", 6, 8, "assignment to 'struct node *' from incompatible pointer type 'char *'")};
    CB_CHECK(mend(st, out, diags, Rules()));
    CB_CHECK_EQ(fixed(st, "a.c", "    f_first(count, 2);"), "    f_first((char *)count, 2);");
    CB_CHECK_EQ(fixed(st, "a.c", "    head = buf;"), "    head = (struct node *)buf;");
    CB_CHECK_EQ(out["a.c"].find("(char *)"), std::string::npos); // the text is not changed: the line is a rule
    CB_CHECK_EQ(reasons(st, "argument of another type than the parameter"), 1u);
    CB_CHECK_EQ(reasons(st, "pointer given to a pointer of another type"), 1u);
});

CB_TEST("port.mend.nothing_to_mend", [] {
    Sources out;
    out["a.c"] = "int x;\nvoid f(void) { x = 1; }\n";
    MendState st = state_of({}, out);
    CB_CHECK(!mend(st, out, {}, Rules()));
    // a complaint nobody knows, one in a file the program lacks, one past the end of its file: nothing happens
    CB_CHECK(!mend(st, out,
                   {diag("a.c", 2, 1, "implicit declaration of function 'zzz'"),
                    diag("none.c", 1, 1, "void value not ignored"), diag("a.c", 99, 1, "void value not ignored"),
                    diag("a.c", 0, 1, "void value not ignored")},
                   Rules()));
    CB_CHECK(st.line_fixes.empty() && st.mended.empty() && st.decl_rules.empty());
});

CB_TEST("port.mend.integer_that_holds_a_pointer", [] {
    // a local given a pointer is a pointer; a global too, in every file, by rules over the declarations
    Sources out;
    out["a.c"] =
        "char *buf;\nint16_t count;\nvoid go(void)\n{\n    int16_t loc;\n    loc = buf;\n    count = buf;\n}\n";
    MendState st = state_of({fn("go", "", {})}, out);
    const std::string said = "assignment to 'int' from 'char *' makes integer from pointer without a cast";
    CB_CHECK(mend(st, out, {diag("a.c", 6, 9, said), diag("a.c", 7, 11, said)}, Rules()));
    CB_CHECK_EQ(st.local_retypes.at(VariableKey{"a.c", "go", "loc"}), "char *");
    CB_CHECK_EQ(st.retyped.at(VariableKey{"", "", "count"}), "char *");
    CB_CHECK_EQ(st.decl_rules.size(), 2u); // the declaration, and the declaration among several names
    CB_CHECK_EQ(st.decl_rules[0].replacement, "$1char *count$2;");
    CB_CHECK(st.decl_rules[1].split.has_value());
    CB_CHECK_EQ(st.decl_rules[1].split->name, "count");
    CB_CHECK_EQ(reasons(st, "integer variable that holds a pointer"), 2u);
    // the same complaint again changes nothing more
    MendState again = st;
    again.mended.clear();
    CB_CHECK(!mend(again, out, {diag("a.c", 6, 9, said)}, Rules()));
});

CB_TEST("port.mend.pointer_cast_to_int_and_negative_constant", [] {
    Sources out;
    out["a.c"] =
        "uint16_t u;\nchar *buf;\nvoid go(void)\n{\n    int16_t n;\n    n = (int)buf;\n    if (u == -1) n = 0;\n}\n";
    MendState st = state_of({fn("go", "", {})}, out);
    CB_CHECK(mend(st, out,
                  {diag("a.c", 6, 9, "cast from pointer to integer of different size"),
                   diag("a.c", 7, 10, "comparison is always false due to limited range of data type")},
                  Rules()));
    CB_CHECK_EQ(fixed(st, "a.c", "    n = (int)buf;"), "    n = (intptr_t)buf;");
    CB_CHECK_EQ(fixed(st, "a.c", "    if (u == -1) n = 0;"), "    if (u == (uint16_t)-1) n = 0;");
});

CB_TEST("port.mend.function_types", [] {
    Sources out;
    out["a.c"] = "int16_t f_int(void)\n{\n    return (int)buf;\n}\nchar *f_str(void)\n{\n    return 0;\n}\n"
                 "void f_void(void)\n{\n}\nvoid go(void)\n{\n    n = f_void();\n}\n";
    MendState st =
        state_of({fn("f_int", "int16_t", {}), fn("f_str", "char *", {}), fn("f_void", "", {}), fn("go", "", {})}, out);
    st.functions.at("f_void").returns_void = true;
    CB_CHECK(mend(
        st, out,
        {diag("a.c", 3, 12,
              "returning 'char *' from a function with return type 'int' makes integer from pointer without a cast"),
         diag("a.c", 7, 12,
              "returning 'int' from a function with return type 'char *' makes pointer from integer without a cast"),
         diag("a.c", 14, 9, "void value not ignored as it ought to be")},
        Rules()));
    CB_CHECK_EQ(st.functions.at("f_int").ret, "char *"); // the pointer it gives
    CB_CHECK(!st.functions.at("f_int").returns_void && st.functions.at("f_int").keep_int);
    CB_CHECK_EQ(st.functions.at("f_str").ret, "int"); // an integer after all, its callers cast
    CB_CHECK(st.functions.at("f_str").int_ret && st.functions.at("f_str").ret_set);
    CB_CHECK(!st.functions.at("f_void").returns_void); // its value is used
});

CB_TEST("port.mend.calls_with_other_numbers_of_arguments", [] {
    Sources out;
    out["a.c"] = "void go(void)\n{\n    fa(1, 2, 3);\n    fb(1);\n}\n";
    MendState st = state_of(
        {fn("fa", "int16_t", {{"a", "int16_t a"}, {"b", "int16_t b"}}), fn("fb", "int16_t", {{"a", "int16_t a"}})},
        out);
    st.functions.at("fb").varargs = true;
    CB_CHECK(mend(st, out,
                  {diag("a.c", 3, 5, "too many arguments to function 'fa'"),
                   diag("a.c", 4, 5, "too few arguments to function call")},
                  Rules()));
    CB_CHECK(st.functions.at("fa").loose);  // old-style definition kept: its calls are not checked
    CB_CHECK(!st.functions.at("fb").loose); // a function with a variable number of arguments is as it is
});

CB_TEST("port.mend.sequence_points_follow_the_old_compilers_order", [] {
    Sources out;
    out["a.c"] = "void go(void)\n{\n    fa((a -= 2) == 0, a);\n}\n";
    const std::vector<Diag> diags = {diag("a.c", 3, 9, "operation on 'a' may be undefined")};
    MendState left = state_of({}, out);
    left.arg_order_left = true;
    CB_CHECK(mend(left, out, diags, Rules()));
    CB_CHECK_EQ(fixed(left, "a.c", "    fa((a -= 2) == 0, a);"), "    a -= 2, fa(a == 0, a);");
    MendState right = state_of({}, out); // right to left: the compilers' own order is today's, nothing to write out
    CB_CHECK(!mend(right, out, diags, Rules()));
});

CB_TEST("port.mend.function_pointers", [] {
    Sources out;
    out["a.c"] = "int16_t (*handler)();\nvoid go(void)\n{\n    handler = f_one;\n    handler = f_two;\n}\n";
    MendState st =
        state_of({fn("f_one", "int16_t", {{"x", "int16_t x"}}), fn("f_two", "int16_t", {{"s", "char *s"}})}, out);
    const std::string said = "assignment to 'int (*)(void)' from incompatible pointer type '";
    CB_CHECK(mend(st, out, {diag("a.c", 4, 13, said + "int (*)(int)'"), diag("a.c", 5, 13, said + "int (*)(char *)'")},
                  Rules()));
    // two prototypes given to one pointer: mixed, each cast to the pointer's own type later
    CB_CHECK(st.fptr_mixed.contains("handler"));
    CB_CHECK_EQ(st.fptr.at("handler").size(), 2u);
    CB_CHECK(st.fptr_done.empty());
    // one prototype: the pointer is declared with it
    Sources out2;
    out2["a.c"] = "int16_t (*handler)();\nvoid go(void)\n{\n    handler = f_one;\n}\n";
    MendState one = state_of({fn("f_one", "int16_t", {{"x", "int16_t x"}})}, out2);
    CB_CHECK(mend(one, out2, {diag("a.c", 4, 13, said + "int (*)(int)'")}, Rules()));
    CB_CHECK(one.fptr_done.contains("handler"));
    CB_CHECK_EQ(one.decl_rules.size(), 1u);
    CB_CHECK_EQ(one.decl_rules[0].replacement, "int (*handler)(int)");
    // a rule's pattern is a real regular expression that finds the old declaration
    CB_CHECK(Regex(one.decl_rules[0].pattern).test("int16_t (*handler)();"));
});

CB_TEST("port.mend.library_names_and_headers", [] {
    Sources out;
    out["proto.h"] = "int16_t abs();\n";
    out["a.c"] = "int16_t abs(a)\nint a;\n{\n    return a;\n}\n";
    MendState st = state_of({fn("abs", "int16_t", {{"a", "int a"}})}, out);
    CB_CHECK(mend(st, out, {diag("proto.h", 1, 1, "conflicting types for 'abs'")}, Rules()));
    CB_CHECK_EQ(st.renames.at("abs"), "abs_");
    // asked again it is not renamed twice
    MendState again = st;
    again.mended.clear();
    CB_CHECK(!mend(again, out, {diag("proto.h", 1, 1, "conflicting types for 'abs'")}, Rules()));
    // an incomplete structure given by value: its header included
    Sources withs;
    withs["a.c"] = "void go(void)\n{\n    fs(tok);\n}\n";
    withs["s.h"] = "struct big { int16_t k; };\n";
    MendState inc = state_of({fn("fs", "int16_t", {{"s", "struct big s"}})}, withs);
    CB_CHECK(mend(inc, withs, {diag("a.c", 3, 5, "argument type 'struct big' is incomplete")}, Rules()));
    CB_CHECK(inc.need_headers.at("a.c").contains("s.h"));
});

CB_TEST("port.mend.parameters_that_carried_a_pointer", [] {
    // intptr_t parameters: a pointer given to one, and an integer parameter given on to one
    Sources out;
    out["w.c"] = "void push(intptr_t n)\n{\n    struct stk *s;\n    s->v = n;\n}\nint16_t pop(void)\n{\n    int16_t "
                 "v;\n    struct stk *s;\n"
                 "    v = s->v;\n    return v;\n}\nvoid relay(int16_t a, int16_t b)\n{\n    f_wide(1, a);\n    "
                 "push(b);\n    f_wide(2, 3);\n}\n";
    out["s.h"] = "struct stk { int16_t v; };\nstruct unrelated { int16_t v; };\n";
    MendState st = state_of({fn("push", "", {{"n", "intptr_t n"}}), fn("pop", "int16_t", {}),
                             fn("relay", "", {{"a", "int16_t a"}, {"b", "int16_t b"}}),
                             fn("f_wide", "int16_t", {{"p", "int16_t p"}, {"q", "intptr_t q"}})},
                            out);
    st.files = out;
    CB_CHECK(passed_on(st, out));
    CB_CHECK_EQ(st.functions.at("relay").params[0].type, "intptr_t a"); // `a` goes to f_wide's second: wide
    CB_CHECK_EQ(st.functions.at("relay").params[1].type, "intptr_t b"); // `b` goes to push's wide parameter
    CB_CHECK_EQ(st.param_types.at({"relay", 0}), "intptr_t a");
    CB_CHECK_EQ(st.param_types.at({"relay", 1}), "intptr_t b");
    CB_CHECK(stored_wide(st, out, Rules()));
    // the member push stores its parameter in is wide, in the program's text and the port's own; pop gives it back
    CB_CHECK(out["s.h"].find("struct stk { intptr_t v; };") != std::string::npos);
    CB_CHECK(out["s.h"].find("struct unrelated { int16_t v; };") != std::string::npos);
    CB_CHECK(st.files["s.h"].find("struct stk { intptr_t v; };") != std::string::npos);
    CB_CHECK_EQ(st.functions.at("pop").ret, "intptr_t");
    CB_CHECK(out["w.c"].find("intptr_t pop(void)") != std::string::npos);
    CB_CHECK(out["w.c"].find("    intptr_t v;\n    struct stk *s;\n    v = s->v;") != std::string::npos);
    CB_CHECK_EQ(st.local_retypes.at(VariableKey{"w.c", "pop", "v"}), "intptr_t");
    CB_CHECK(!stored_wide(st, out, Rules())); // nothing left to do the second time
});

CB_TEST("port.mend.stored_wide_keeps_declarations_changed_for_an_earlier_file", [] {
    // The Python wrote the stale text of b.c back over the declaration an earlier file's mending had changed.
    Sources out;
    out["a.c"] = "#include \"s.h\"\nvoid push(intptr_t n)\n{\n    struct stk *s;\n    s->v = n;\n}\n"
                 "int16_t pop(void)\n{\n    int16_t v;\n    struct stk *s;\n    v = s->v;\n    return v;\n}\n";
    out["b.c"] = "#include \"s.h\"\nint16_t pop(void);\nint16_t other(void)\n{\n    int16_t w;\n    struct stk *s;\n   "
                 " w = s->v;\n    return w;\n}\n";
    out["s.h"] = "struct stk { int16_t v; };\n";
    MendState st =
        state_of({fn("push", "", {{"n", "intptr_t n"}}), fn("pop", "int16_t", {}), fn("other", "int16_t", {})}, out);
    CB_CHECK(stored_wide(st, out, Rules()));
    CB_CHECK(out["b.c"].find("intptr_t pop(void);") != std::string::npos);
    CB_CHECK(out["b.c"].find("intptr_t other(void)") != std::string::npos &&
             out["b.c"].find("intptr_t w;") != std::string::npos);
    // the cast form is a rule of its own that can be switched off
    Sources cast;
    cast["a.c"] = "void set(struct stk *r, char *x)\n{\n    r->w = (intptr_t)x;\n}\n";
    cast["s.h"] = "struct stk { int16_t v; int16_t w; };\n";
    MendState with = state_of({fn("set", "", {{"r", "struct stk *r"}, {"x", "char *x"}})}, cast);
    MendState without = with;
    Sources cast_off = cast;
    CB_CHECK(stored_wide(with, cast, Rules()));
    CB_CHECK(cast["s.h"].find("intptr_t w;") != std::string::npos);
    CB_CHECK(!stored_wide(without, cast_off, Rules("-stored_wide_cast")));
});

CB_TEST("port.mend.unions_and_tags", [] {
    Sources out;
    out["u.h"] = "union tokval { int32_t l; double d; char *s; struct sym *y; };\nunion plain { int16_t i; int32_t l; "
                 "char c; };\n";
    out["a.c"] = "#include \"u.h\"\nstruct local { int a; };\n";
    CB_CHECK_EQ(*union_member_for(out, "union tokval", "long"), "l");
    CB_CHECK_EQ(*union_member_for(out, "union tokval", "int"), "l"); // today's int is 32 bits
    CB_CHECK_EQ(*union_member_for(out, "union tokval", "char *"), "s");
    CB_CHECK_EQ(*union_member_for(out, "union tokval", "struct foo *"), "s"); // the first pointer: char * by preference
    CB_CHECK_EQ(*union_member_for(out, "union tokval", "double"), "d");
    CB_CHECK_EQ(*union_member_for(out, "union plain", "short"), "i");
    CB_CHECK_EQ(*union_member_for(out, "union plain", "char"), "c");
    CB_CHECK_EQ(*union_member_for(out, "union plain", "unsigned char"), "i"); // the first of the 16-bit kinds
    CB_CHECK(!union_member_for(out, "union plain", "double"));
    CB_CHECK(!union_member_for(out, "union nosuch", "int"));
    CB_CHECK(!union_member_for(out, "", "int"));
    const auto tags = visible_tags(out, "a.c");
    CB_CHECK(tags.contains("local") && tags.contains("tokval") && tags.contains("plain")); // its own and its header's
    CB_CHECK(visible_tags(out, "u.h").size() == 2u && !visible_tags(out, "u.h").contains("local"));
    CB_CHECK(visible_tags(out, "missing.c").empty());
});
