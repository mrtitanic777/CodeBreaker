// Tests of the files the port rewrites and adds (rewrite, frames, bss, headers) on a mini program: an old-style C
// file and its header. The expected text is what the Python original makes of the same input (reviewed line by line).
#include "cb/base/ctext.hpp"
#include "cb/core/testing.hpp"
#include "cb/port/bss.hpp"
#include "cb/port/frames.hpp"
#include "cb/port/headers.hpp"
#include "cb/port/original_program.hpp"
#include "cb/port/rewrite.hpp"

using namespace cb;
using namespace cb::port::pre_ansi;

namespace {

const char* const kRecH =
    "#ifndef REC_H\n"
    "#define REC_H\n"
    "struct rec { int len; char *name; struct rec *next; };\n"
    "extern int g_cnt;\n"
    "extern char g_buf[8];\n"
    "extern char g_tail[4];\n"
    "#endif\n";

const char* const kMainC =
    "#include <stdio.h>\n"
    "#include \"rec.h\"\n"
    "#define putc(c, f) _putc(c, f)\n"
    "\n"
    "static int helper(r)\n"
    "struct rec *r;\n"
    "{\n"
    "    return r->len;\n"
    "}\n"
    "\n"
    "show(fmt, args)\n"
    "char *fmt;\n"
    "int args;\n"
    "{\n"
    "    char *ap;\n"
    "    ap = &args;\n"
    "    printf(fmt, *ap);\n"
    "}\n"
    "\n"
    "struct rec *mk(n)\n"
    "int n;\n"
    "{\n"
    "    static struct rec one;\n"
    "    one.len = n;\n"
    "    return &one;\n"
    "}\n"
    "\n"
    "total(a, b)\n"
    "int a, b;\n"
    "{\n"
    "    helper(mk(a));\n"
    "    return;\n"
    "}\n"
    "\n"
    "older(x)\n"
    "{\n"
    "    return x + 1;\n"
    "}\n"
    "\n"
    "pick(flag)\n"
    "int flag;\n"
    "{\n"
    "    int v;\n"
    "    if (flag)\n"
    "        v = older(1);\n"
    "    else\n"
    "        v = older(2);\n"
    "}\n";

Profile mini_profile() {
    Profile p;
    p.libc = {"putc", "getc"};
    p.runtime_headers = {"stdio.h", "malloc.h"};
    p.runtime_internals = {"_iob"};
    p.platform_header = "dos.h";
    p.widths_pointer = 16;
    return p;
}

// The program as the port has it after preprocess and the old-style definitions
struct Mini {
    Profile profile = mini_profile();
    Files files;
    Files pre;
    OrderedMap<std::string, Fn> fns;
    Notes notes;
    Mini() {
        files.insert_or_assign("rec.h", kRecH);
        files.insert_or_assign("main.c", kMainC);
        for (const auto& kv : files) pre.insert_or_assign(kv.first, preprocess(kv.second, kv.first, profile, {}, notes));
        for (const auto& kv : pre) {
            if (!kv.first.ends_with(".c")) continue;
            const auto lines = split_lines(kv.second);
            for (Definition& d : definitions(lines, split_lines(base::ctext::blank(kv.second)))) {
                fns.try_emplace(d.fn.name, d.fn);
                fns.at(d.fn.name).file = kv.first;
            }
        }
    }
    std::string definitions_of(const std::string& file) { return rewrite_definitions(pre.at(file), file, fns, {}, notes).text; }
};

const WidthsFn kNoWidths = [](std::string_view text, std::string_view, bool) { return std::string(text); };

}  // namespace

CB_TEST("port.rewrite.preprocess_points_the_old_library_at_port_h", [] {
    Mini m;
    CB_CHECK_EQ(m.pre.at("main.c").substr(0, 36), "#include \"port.h\"\n#include \"rec.h\"\n\n");
    CB_CHECK(m.pre.at("main.c").find("stdio.h") == std::string::npos);
    CB_CHECK(m.pre.at("main.c").find("_putc") == std::string::npos);   // (the program's own putc macro: the C library has it)
    CB_CHECK_EQ(m.pre.at("rec.h"), kRecH);                            // (a header gets no port.h)
    CB_CHECK(m.notes.at("#include of the old library's header -> port.h") == std::vector<std::string>{"main.c:1"});
    CB_CHECK(m.notes.at("macro of the old library's dropped (the C library of today's stands in)") == std::vector<std::string>{"main.c:3"});
    // a header the port left out of its tree is the old library's too
    Notes notes;
    const std::string with_left_out = preprocess("#include \"gone.h\"\nint x;\n", "a.c", mini_profile(), {"gone.h"}, notes);
    CB_CHECK_EQ(with_left_out, "#include \"port.h\"\nint x;\n");
    // and a file that already includes port.h gets no second one
    CB_CHECK_EQ(preprocess("#include \"port.h\"\nint y;\n", "b.c", mini_profile(), {}, notes), "#include \"port.h\"\nint y;\n");
});

CB_TEST("port.rewrite.old_style_definitions_become_prototypes", [] {
    Mini m;
    const std::string out = m.definitions_of("main.c");
    CB_CHECK_EQ(out,
                "#include \"port.h\"\n"
                "#include \"rec.h\"\n"
                "static int helper(struct rec *r);\n"   // (a static function: declared at the file's head)
                "\n"
                "static int helper(struct rec *r)\n"
                "{\n"
                "    return r->len;\n"
                "}\n"
                "\n"
                "void show(char *fmt, int args)\n"      // (it never returns a value: void)
                "{\n"
                "    char *ap;\n"
                "    ap = &args;\n"
                "    printf(fmt, *ap);\n"
                "}\n"
                "\n"
                "struct rec *mk(int n)\n"
                "{\n"
                "    static struct rec one;\n"
                "    one.len = n;\n"
                "    return &one;\n"
                "}\n"
                "\n"
                "void total(int a, int b)\n"
                "{\n"
                "    helper(mk(a));\n"
                "    return;\n"
                "}\n"
                "\n"
                "int older(int x)\n"                    // (a parameter never declared: an int)
                "{\n"
                "    return x + 1;\n"
                "}\n"
                "\n"
                "void pick(int flag)\n"
                "{\n"
                "    int v;\n"
                "    if (flag)\n"
                "        v = older(1);\n"
                "    else\n"
                "        v = older(2);\n"
                "}\n");
    CB_CHECK_EQ(m.fns.at("show").proto(), "void show(char *fmt, int args)");
    CB_CHECK(m.fns.at("show").returns_void);
    CB_CHECK(!m.fns.at("older").returns_void);
    CB_CHECK_EQ(m.fns.at("older").body.substr(0, 1), "{");
    CB_CHECK_EQ(m.notes.at("old-style definition -> prototype").size(), 6u);
    CB_CHECK_EQ(m.notes.at("old-style definition -> prototype").front(), "main.c:39");   // (the last function first: the rewrite goes from the end)
});

CB_TEST("port.rewrite.the_result_names_the_functions_it_updated", [] {
    Mini m;
    const RewrittenFile r = rewrite_definitions(m.pre.at("main.c"), "main.c", m.fns, {}, m.notes);
    CB_CHECK(r.functions == std::vector<std::string>{"helper", "show", "mk", "total", "older", "pick"});
    // a function the program lists but the first pass did not is added
    OrderedMap<std::string, Fn> none;
    rewrite_definitions(m.pre.at("main.c"), "main.c", none, {}, m.notes);
    CB_CHECK_EQ(none.size(), 6u);
});

CB_TEST("port.rewrite.the_loops_parameter_types_win", [] {
    Mini m;
    ParamTypes types;
    types[{"mk", 0}] = "long n";
    const std::string out = rewrite_definitions(m.pre.at("main.c"), "main.c", m.fns, types, m.notes).text;
    CB_CHECK(out.find("struct rec *mk(long n)") != std::string::npos);
});

CB_TEST("port.rewrite.finish_applies_the_rules_in_order", [] {
    Mini m;
    const std::string defs = m.definitions_of("main.c");
    FinishRules rules;
    rules.renames["printf"] = "dos_printf";
    Finished f = finish(rules, "main.c", defs);
    CB_CHECK(f.text.find("    dos_printf(fmt, *ap);") != std::string::npos);
    CB_CHECK_EQ(f.origin.size(), split_lines(defs).size());
    CB_CHECK_EQ(f.origin.front().text, "#include \"port.h\"");
    // a name in an #include line is the header's: not renamed
    rules.renames["stdio"] = "dos_stdio";
    CB_CHECK_EQ(finish(rules, "x.c", "#include <stdio.h>\nint stdio;\n").text, "#include <stdio.h>\nint dos_stdio;\n");
    // a declaration rule, an array's size and a line fix
    FinishRules more;
    more.decl_rules.emplace_back(R"(\bextern\s+int\s+g_cnt\b)", "extern long g_cnt");
    more.array_sizes["g_tail"] = 4;
    more.line_fixes[LineFixKey{"rec.h", "#define REC_H", 0}] = "#define REC_H 1";
    const Finished h = finish(more, "rec.h", "#ifndef REC_H\n#define REC_H\nextern int g_cnt;\nextern char g_tail[];\nextern char g_buf[8];\n#endif\n");
    CB_CHECK_EQ(h.text, "#ifndef REC_H\n#define REC_H 1\nextern long g_cnt;\nextern char g_tail[4];\nextern char g_buf[8];\n#endif\n");
    CB_CHECK_EQ(h.origin[1].text, "#define REC_H");   // (the line as it was before the fix)
    CB_CHECK_EQ(h.origin[1].nth, 0u);
    // the n-th of two alike lines
    FinishRules second;
    second.line_fixes[LineFixKey{"a.c", "x;", 1}] = "y;";
    CB_CHECK_EQ(finish(second, "a.c", "x;\nx;\nx;").text, "x;\ny;\nx;");
    FinishRules every;
    every.line_fixes[LineFixKey{"a.c", "x;", std::nullopt}] = "z;";
    CB_CHECK_EQ(finish(every, "a.c", "x;\nw;\nx;").text, "z;\nw;\nz;");
});

CB_TEST("port.rewrite.finish_retypes_a_local_that_holds_a_pointer", [] {
    FinishRules rules;
    rules.local_retypes.push_back({"f.c", "g", "p", "char *"});
    const std::string text =
        "int g(int n, int p)\n"
        "{\n"
        "    int p2;\n"
        "    return n;\n"
        "}\n"
        "int h(void)\n"
        "{\n"
        "    int p;\n"
        "    return p;\n"
        "}\n";
    const std::string out = finish(rules, "f.c", text).text;
    CB_CHECK(out.find("int g(int n, char *p)") != std::string::npos);   // the parameter of g
    CB_CHECK(out.find("    int p;") != std::string::npos);                // h's own p is an int
    CB_CHECK_EQ(apply_local_retypes("int g(a, p)\nint p;\n{\n}\n", "f.c", {{"f.c", "g", "p", "char *"}}), "int g(a, p)\nchar *p;\n{\n}\n");
    CB_CHECK_EQ(apply_local_retypes("int g(p)\n{\n int a, p, b;\n}\n", "f.c", {{"f.c", "g", "p", "char *"}}),
                "int g(p)\n{\n int a, b; char *p;\n}\n");
    CB_CHECK_EQ(apply_local_retypes("int g(p)\n", "other.c", {{"f.c", "g", "p", "char *"}}), "int g(p)\n");   // (another file's rule)
});

CB_TEST("port.rewrite.null_reads_through_the_null_segment", [] {
    FinishRules rules;
    rules.null_reads.push_back({"f.c", "l", "struct n *", NullRead::Kind::Var});
    rules.null_reads.push_back({"f.c", "q", "struct n *", NullRead::Kind::Assign});
    rules.null_reads.push_back({"f.c", "a", "struct n *", NullRead::Kind::Plain});
    rules.null_reads.push_back({"g.c", "z", "struct n *", NullRead::Kind::Var});   // (another file: not applied)
    const std::string out =
        finish(rules, "f.c", "x = l->op; y = (q = next(p))->kind; z = a[0]->op; w = z->k; v = \"l->op\";").text;
    CB_CHECK_EQ(out,
                "x = ((struct n *)dos_np(l))->op; y = ((struct n *)dos_np(q = next(p)))->kind; "
                "z = ((struct n *)dos_np(a[0]))->op; w = z->k; v = \"l->op\";");
    FinishRules mem;
    mem.null_reads.push_back({"f.c", "kind", "struct n *", NullRead::Kind::Mem});
    // (the link read through the null segment where a member is read from a member: p->kind->z, not p->next->kind = q)
    CB_CHECK_EQ(finish(mem, "f.c", "p->next->kind = q; r = (x)->kind->y; s = p->kind->z;").text,
                "p->next->kind = q; r = (x)->kind->y; s = ((struct n *)dos_np(p->kind))->z;");
});

CB_TEST("port.frames.locals_a_call_reads_past_an_array_are_one_structure", [] {
    const std::string text =
        "struct dir { char name[8]; int attr; int size; };\n"
        "\n"
        "static int rd(d)\n"
        "struct dir *d;\n"
        "{\n"
        "    return d->attr + d->size;\n"
        "}\n"
        "\n"
        "f()\n"
        "{\n"
        "    int size;\n"
        "    int attr;\n"
        "    char buf[8];\n"
        "    return rd(buf);\n"
        "}\n"
        "\n"
        "g(n)\n"
        "int n;\n"
        "{\n"
        "    char big[64];\n"
        "    return rd(big);\n"
        "}\n";
    Profile profile = mini_profile();
    Notes notes;
    OrderedMap<std::string, Fn> fns;
    Files files;
    files.insert_or_assign("f.c", rewrite_definitions(preprocess(text, "f.c", profile, {}, notes), "f.c", fns, {}, notes).text);
    // (the definitions were found on the text the preprocessor left)
    for (const Definition& d : definitions(split_lines(text), split_lines(base::ctext::blank(text)))) fns.try_emplace(d.fn.name, d.fn);
    files.insert_or_assign("f.c", rewrite_definitions(preprocess(text, "f.c", profile, {}, notes), "f.c", fns, {}, notes).text);
    Notes frame_notes;
    frames(files, fns, profile.old_pointer_bytes(), {}, frame_notes);
    CB_CHECK_EQ(files.at("f.c"),
                "#include \"port.h\"\n"
                "static int rd(struct dir *d);\n"
                "struct dir { char name[8]; int attr; int size; };\n"
                "\n"
                "static int rd(struct dir *d)\n"
                "{\n"
                "    return d->attr + d->size;\n"
                "}\n"
                "\n"
                "int f(void)\n"
                "{\n"
                "\tstruct {\t\t\t/* side by side, as the original's frame had them: rd reads past buf */\n"
                "\t\tchar buf[8];\n"
                "\t\tint attr;\n"
                "\t\tint size;\n"
                "\t} frame;\n"
                "    return rd(frame.buf);\n"
                "}\n"
                "\n"
                "int g(int n)\n"   // (big[64] is long enough: left alone)
                "{\n"
                "    char big[64];\n"
                "    return rd(big);\n"
                "}\n");
    CB_CHECK_EQ(frame_notes.size(), 1u);
    CB_CHECK(frame_notes.entries().front().second == std::vector<std::string>{"f.c (rd: buf, attr, size)"});
});

CB_TEST("port.frames.local_decls_in_order_and_their_old_sizes", [] {
    const std::vector<std::string> code = {"f()", "{", "    register char *p, q[4];", "    int w;", "    struct s *sp;", "    long x = 1;", "    return;", "}"};
    const auto locals = local_decls(code, 1, 7);
    CB_CHECK_EQ(locals.size(), 4u);   // (the declaration with an initializer ends the head)
    CB_CHECK_EQ(locals.at("p").base, "register char");
    CB_CHECK(locals.at("p").is_register);
    CB_CHECK_EQ(locals.at("p").size.value_or(0), 2u);
    CB_CHECK_EQ(locals.at("q").size.value_or(0), 4u);
    CB_CHECK_EQ(locals.at("w").line, 3u);
    CB_CHECK_EQ(locals.at("sp").size.value_or(0), 2u);   // (a pointer: 2 bytes in the original)
    CB_CHECK_EQ(local_decls(code, 1, 7, 4).at("sp").size.value_or(0), 4u);
    CB_CHECK_EQ(locals.entries().front().first, "p");
});

CB_TEST("port.bss.the_list_is_defined_one_array_for_arrays_that_were_one", [] {
    Mini m;
    BssSizes sizes;
    sizes.insert_or_assign("g_cnt", 2);
    sizes.insert_or_assign("g_buf", 8);
    sizes.insert_or_assign("g_tail", 4);
    sizes.insert_or_assign("_iob", 5);
    Files originals;
    originals.insert_or_assign("rec.h", kRecH);
    originals.insert_or_assign("main.c", kMainC);
    OriginalProgram original(originals, 2);
    BssPlan plan(original, m.profile.runtime_internals);
    plan.set_sizes(sizes);
    Aliases aliases;
    std::vector<DeclRule> rules;
    CB_CHECK(plan.join_bss("g_buf", aliases, rules));   // g_buf[8] and g_tail[4]: arrays of one element type, one after the other
    CB_CHECK_EQ(aliases.size(), 1u);
    CB_CHECK_EQ(aliases.at("g_tail").where, "(&g_buf[8])");
    CB_CHECK_EQ(aliases.at("g_tail").declaration, "char g_buf[28];");
    CB_CHECK_EQ(rules.size(), 1u);
    CB_CHECK_EQ(rules[0].apply("extern char g_buf[8];\nextern char g_tail[4];\n"), "extern char g_buf[28];\nextern char g_tail[4];\n");
    CB_CHECK(plan.joined().at("g_buf") == std::vector<std::string>{"g_tail"});
    CB_CHECK(plan.definitions(aliases) ==
             (std::vector<std::string>{"int g_cnt;", "char g_buf[28];\t/* (12 bytes then, and room) */"}));
    // g_cnt is an int (the extern says so): it is no array and not joined; _iob is the old runtime's
    CB_CHECK(!plan.join_bss("g_cnt", aliases, rules));
});

CB_TEST("port.bss.a_variable_after_an_array_of_another_shape_is_not_joined", [] {
    Files originals;
    originals.insert_or_assign("a.h", "extern char a[4];\nextern int b[4];\n");
    OriginalProgram original(originals, 2);
    BssPlan plan(original, {});
    BssSizes sizes;
    sizes.insert_or_assign("a", 4);
    sizes.insert_or_assign("b", 8);
    plan.set_sizes(sizes);
    Aliases aliases;
    std::vector<DeclRule> rules;
    CB_CHECK(!plan.join_bss("a", aliases, rules));   // (char and int arrays: not one element type)
    CB_CHECK(!plan.join_bss("b", aliases, rules));   // (nothing after b)
    CB_CHECK(aliases.empty() && rules.empty());
    // and with no BSS list there is nothing to define
    BssPlan empty(original, {});
    CB_CHECK(empty.definitions(aliases).empty());
});

CB_TEST("port.bss.a_routine_that_takes_a_structure_makes_the_run_after_the_variable_its_members", [] {
    Files originals;
    originals.insert_or_assign("t.h", "struct slot { int top; int v; };\nextern int s_a;\nextern int s_b;\n");
    originals.insert_or_assign("t.c", "#include \"t.h\"\nstatic int use(p)\nstruct slot *p;\n{\n    return p->top;\n}\nf()\n{\n    return use(&s_a);\n}\n");
    OriginalProgram original(originals, 2);
    BssPlan plan(original, {});
    BssSizes sizes;
    sizes.insert_or_assign("s_a", 2);   // (smaller than the structure: it was the first member of one)
    sizes.insert_or_assign("s_b", 2);
    plan.set_sizes(sizes);
    OrderedMap<std::string, Fn> fns;
    for (const auto& kv : originals) {
        for (Definition& d : definitions(split_lines(kv.second), split_lines(base::ctext::blank(kv.second)))) fns.try_emplace(d.fn.name, d.fn);
    }
    Aliases aliases;
    std::set<std::string> view_headers;
    Notes notes;
    plan.bss_views(originals, fns, aliases, view_headers, notes);
    CB_CHECK_EQ(aliases.at("s_a").where, "(s_a_s.top)");
    CB_CHECK_EQ(aliases.at("s_b").where, "(s_a_s.v)");
    CB_CHECK_EQ(aliases.at("s_a").declaration, "struct slot s_a_s;");
    CB_CHECK(view_headers.contains("t.h"));
    CB_CHECK(plan.structs().at("s_a_s").names == std::vector<std::string>{"s_a", "s_b"});
    CB_CHECK_EQ(plan.definitions(aliases).front(), "struct slot s_a_s;\t/* (s_a, s_b: one place, as the old linker kept them) */");
    // done once: a second call changes nothing
    const std::size_t notes_before = notes.size();
    plan.bss_views(originals, fns, aliases, view_headers, notes);
    CB_CHECK_EQ(notes.size(), notes_before);
});

CB_TEST("port.headers.port_h_proto_h_bss_c_and_data_c", [] {
    Mini m;
    for (auto& kv : m.pre) {
        if (kv.first.ends_with(".c")) m.definitions_of(kv.first);
    }
    Files originals;
    originals.insert_or_assign("rec.h", kRecH);
    originals.insert_or_assign("main.c", kMainC);
    OriginalProgram original(originals, 2);
    BssPlan plan(original, m.profile.runtime_internals);
    BssSizes sizes;
    sizes.insert_or_assign("g_cnt", 2);
    sizes.insert_or_assign("g_buf", 8);
    sizes.insert_or_assign("g_tail", 4);
    sizes.insert_or_assign("_iob", 5);
    plan.set_sizes(sizes);
    Aliases aliases;
    std::vector<DeclRule> rules;
    plan.join_bss("g_buf", aliases, rules);
    const std::vector<std::pair<std::string, std::string>> data = {{"rec.h", "int g_cnt;"}, {"rec.h", "char g_buf[28];"}};
    const std::map<std::string, std::string> asm_protos = {{"zz_asm", "int zz_asm();"}, {"aa_asm", "void aa_asm();"}};
    const std::set<std::string> view_headers = {"rec.h"};
    const HeaderInputs in{m.fns, m.files, data, {}, asm_protos, m.profile, view_headers, false};
    const Headers h = make_headers(in, aliases, plan, FinishRules{}, kNoWidths);
    CB_CHECK_EQ(h.files.size(), 4u);
    CB_CHECK_EQ(*h.files.at("port.h"),
                "/* port.h -- what every file of the port includes first (made by CodeBreaker's port) */\n"
                "#ifndef PORT_H\n"
                "#define PORT_H\n"
                "#include <stdio.h>\n"
                "#include <stdlib.h>\n"
                "#include <string.h>\n"
                "#include <stdint.h>\n"
                "#include <ctype.h>\n"
                "#include <signal.h>\n"
                "#include <errno.h>\n"
                "#include <time.h>\n"
                "#include \"dos.h\"\n"
                "#include \"rec.h\"\n"
                "\n"
                "/* the names the old linker gave the places of an array's elements */\n"
                "extern char g_buf[28];\n"
                "#define g_tail\t(&g_buf[8])\n"
                "#include \"proto.h\"\n"
                "#endif\n");
    CB_CHECK_EQ(*h.files.at("proto.h"),
                "/* proto.h -- every function of the program, declared (made by CodeBreaker's port) */\n"
                "#ifndef PROTO_H\n"
                "#define PROTO_H\n"
                "struct rec;\n"
                "\n"
                "void show(char *fmt, int args);\n"
                "struct rec *mk(int n);\n"
                "void total(int a, int b);\n"
                "int older(int x);\n"
                "void pick(int flag);\n"
                "\n"
                "/* the program's routines in assembly language (their *64.asm) */\n"
                "void aa_asm();\n"
                "int zz_asm();\n"
                "\n"
                "#endif\n");
    CB_CHECK_EQ(*h.files.at("bss.c"),
                "/* bss.c -- the variables the old build defined beginning without a value (its BSS list), as the program declares them "
                "(made by CodeBreaker's port) */\n"
                "#include \"port.h\"\n"
                "\n"
                "int g_cnt;\n"
                "char g_buf[28];\t/* (12 bytes then, and room) */\n");
    CB_CHECK_EQ(*h.files.at("data.c"),
                "/* data.c -- the variables the headers share, defined once (made by CodeBreaker's port) */\n"
                "#include \"port.h\"\n"
                "#include \"rec.h\"\n"
                "\n"
                "int g_cnt;\n"
                "char g_buf[28];\n");
    CB_CHECK_EQ(h.origins.at("port.h").size(), 20u);
    CB_CHECK_EQ(h.origins.at("data.c").size(), 7u);
    CB_CHECK_EQ(h.files.entries()[0].first, "port.h");   // (in the order the files are written)
});

CB_TEST("port.headers.no_data_no_bss_gives_empty_files", [] {
    Files files;
    OrderedMap<std::string, Fn> fns;
    const Profile profile = mini_profile();
    Files originals;
    OriginalProgram original(originals, 2);
    BssPlan plan(original, {});
    Aliases aliases;
    const std::vector<std::pair<std::string, std::string>> none;
    const HeaderInputs in{fns, files, none, none, {}, profile, {}, false};
    const Headers h = make_headers(in, aliases, plan, FinishRules{}, kNoWidths);
    CB_CHECK(!h.files.at("bss.c").has_value());
    CB_CHECK(!h.files.at("data.c").has_value());
    CB_CHECK(h.files.at("port.h").has_value());
});

CB_TEST("port.headers.a_tag_defined_two_ways_makes_a_data_file_each", [] {
    Files files;
    files.insert_or_assign("a.h", "struct t { int x; };\n");
    files.insert_or_assign("b.h", "struct t { long y; };\n");
    OrderedMap<std::string, Fn> fns;
    const Profile profile = mini_profile();
    Files originals;
    OriginalProgram original(originals, 2);
    BssPlan plan(original, {});
    Aliases aliases;
    const std::vector<std::pair<std::string, std::string>> data = {{"a.h", "struct t ga;"}, {"b.h", "struct t gb;"}};
    const std::vector<std::pair<std::string, std::string>> none;
    const HeaderInputs in{fns, files, data, none, {}, profile, {}, false};
    const Headers h = make_headers(in, aliases, plan, FinishRules{}, kNoWidths);
    CB_CHECK(h.files.at("data.c")->find("struct t ga;") != std::string::npos);
    CB_CHECK(h.files.at("data.c")->find("struct t gb;") == std::string::npos);
    CB_CHECK(h.files.at("data2.c")->find("struct t gb;") != std::string::npos);
    // one header including the other defines nothing twice
    CB_CHECK(same_definition(files, "struct t", "a.h", "a.h"));
    CB_CHECK(!same_definition(files, "struct t", "a.h", "b.h"));
    files.insert_or_assign("c.h", "#include \"a.h\"\n");
    CB_CHECK(same_definition(files, "struct t", "a.h", "c.h"));
});
