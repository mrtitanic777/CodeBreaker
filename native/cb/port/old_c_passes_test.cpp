#include "cb/port/old_c_passes.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::port;

namespace {

// the places recorded under one reason, in the order they were found
std::vector<std::string> places(const Notes& notes, std::string_view why_starts) {
    for (const auto& [why, where] : notes) {
        if (why.starts_with(why_starts))
            return where;
    }
    return {};
}

} // namespace

CB_TEST("port.passes.rules", [] {
    CB_CHECK(Rules().on("agree_wide_defs"));
    CB_CHECK(!Rules("-agree_wide_defs").on("agree_wide_defs"));
    CB_CHECK(Rules("-agree_wide_defs").on("stored_wide_cast"));
    CB_CHECK(!Rules("-all").on("anything"));
    CB_CHECK(Rules("-all, +x").on("x")); // in order; spaces do not count
    CB_CHECK(!Rules("+x,-x").on("x"));
    CB_CHECK(Rules("x").on("x"));
    CB_CHECK(Rules("-x,+all").on("x"));
    {
        fs::ScopedEnv env("CB_PORT_RULES", "-stored_wide_cast");
        CB_CHECK(!Rules::from_environment().on("stored_wide_cast"));
    }
    {
        fs::ScopedEnv env("CB_PORT_RULES", std::nullopt);
        CB_CHECK(Rules::from_environment().on("stored_wide_cast"));
    }
});

CB_TEST("port.passes.block_heads", [] {
    const std::string text =
        "void *get(int n)\n{\n    char **b;\n    b = (char **)malloc(n + 4);\n    return (char *)b + 4;\n}\n"
        "void *next(struct blk *b)\n{\n    return (char *)b->link + 2;\n}\n"
        "struct blk row = { (char **)pool, 1016, pool + 4 };\n"
        "char *odd = (char *)q + 3;\n";
    Notes notes;
    // a block's link is a char ** (by its cast), or a member that holds one (ptr2); an odd head is not a head
    const std::string out = block_heads(text, notes, "heads.c", {"link"});
    CB_CHECK(out.find("malloc(n + DOS_HEAD(4))") != std::string::npos);
    CB_CHECK(out.find("(char *)b->link + DOS_HEAD(2)") != std::string::npos);
    CB_CHECK(out.find("pool + DOS_HEAD(4) }") != std::string::npos);
    CB_CHECK(out.find("(char *)b + 4;") != std::string::npos); // b is no known link: left
    CB_CHECK(out.find("(char *)q + 3") != std::string::npos);
    CB_CHECK(places(notes, "a block chained") == (std::vector<std::string>{"heads.c (3)"}));
    // without the member, only the two the cast itself proves
    Notes none;
    const std::string less = block_heads(text, none, "heads.c", {});
    CB_CHECK(less.find("(char *)b->link + 2") != std::string::npos);
    CB_CHECK(places(none, "a block chained") == (std::vector<std::string>{"heads.c (2)"}));
    // nothing to do: the text as it was, no note
    Notes quiet;
    CB_CHECK_EQ(block_heads("int x;\n", quiet, "a.c", {}), "int x;\n");
    CB_CHECK(quiet.empty());
    // text in a string or comment is not code
    CB_CHECK_EQ(block_heads("char *s = \"(char **)p + 4\";\n", quiet, "a.c", {}), "char *s = \"(char **)p + 4\";\n");
});

CB_TEST("port.passes.stdio_inits", [] {
    Notes notes;
    const std::string out =
        stdio_inits("#include <stdio.h>\nFILE *out = stdout;\nstatic FILE *in = stdin;\nint f(void);\n", notes, "a.c");
    CB_CHECK_EQ(out, "#include <stdio.h>\nFILE *out;\nstatic FILE *in;\nint f(void);\n"
                     "DOS_INIT(dos_init_out) { out = stdout; }\nDOS_INIT(dos_init_in) { in = stdin; }\n");
    CB_CHECK(places(notes, "a variable given stdin") == (std::vector<std::string>{"a.c (out)", "a.c (in)"}));
    // only a .c file has a place to run the routine from
    Notes header;
    CB_CHECK_EQ(stdio_inits("FILE *h = stdout;\n", header, "a.h"), "FILE *h = stdout;\n");
    CB_CHECK(header.empty());
    // inside a function the assignment is just an assignment: it is not at the start of a line with FILE
    Notes inner;
    CB_CHECK_EQ(stdio_inits("void f() { FILE *g; g = stderr; }\n", inner, "a.c"),
                "void f() { FILE *g; g = stderr; }\n");
});

CB_TEST("port.passes.file_internals", [] {
    Notes notes;
    const std::string out =
        file_internals("int a(FILE *fp) { return fp->_file; }\nint b(FILE *fp) { return fp->_flag & 0x10; }\n"
                       "void c(FILE *fp) { fp->_flag = 0; fp->_file = 3; fp->_flag |= 1; }\n"
                       "int d(struct t *t) { return t->files[2]->_file + t->cur->_flag; }\n",
                       notes, "io.c");
    CB_CHECK_EQ(out,
                "int a(FILE *fp) { return fileno(fp); }\nint b(FILE *fp) { return dos_fflag(fp) & 0x10; }\n"
                "void c(FILE *fp) { fp->_flag = 0; fp->_file = 3; fp->_flag |= 1; }\n" // (stores are not reads)
                "int d(struct t *t) { return fileno(t->files[2]) + dos_fflag(t->cur); }\n");
    CB_CHECK(places(notes, "the old library's FILE") == (std::vector<std::string>{"io.c (4)"}));
    // `==` after the member is a read, `=` is a store
    Notes eq;
    CB_CHECK_EQ(file_internals("x = fp->_flag == 0;", eq, "a.c"), "x = dos_fflag(fp) == 0;");
});

CB_TEST("port.passes.word_unions_and_copies", [] {
    const std::string text = "union tokval { int16_t i; char *s; struct sym *sym; };\n"
                             "union num { int16_t i; int32_t l; };\n"
                             "union pair { int16_t val; char *name; };\n"
                             "union empty_ptr { char *a; char *b; };\n"
                             "union both { unsigned n; char *p; double d; };\n"
                             "struct holder { union { int16_t v; char *s; } u; int16_t w; };\n";
    Notes notes;
    // a number and pointers only: the number as wide as a pointer
    const std::string out = word_unions(text, notes, "u.c");
    CB_CHECK(out.find("union tokval { intptr_t i; char *s; struct sym *sym; };") != std::string::npos);
    CB_CHECK(out.find("union num { int16_t i; int32_t l; };") != std::string::npos); // no pointer
    CB_CHECK(out.find("union pair { intptr_t val; char *name; };") != std::string::npos);
    CB_CHECK(out.find("union both { unsigned n; char *p; double d; };") != std::string::npos); // a double too
    CB_CHECK(out.find("union { intptr_t v; char *s; } u;") != std::string::npos);
    CB_CHECK(places(notes, "a union of a number") == (std::vector<std::string>{"u.c (i)", "u.c (val)", "u.c (v)"}));
    // unions that keep their old layout are left
    Notes olds;
    const std::string kept = word_unions(text, olds, "u.c", {"tokval", "pair"});
    CB_CHECK(kept.find("union tokval { int16_t i;") != std::string::npos);
    CB_CHECK(places(olds, "a union of a number") == (std::vector<std::string>{"u.c (v)"}));
    // an int copied from one union to another carried a pointer: it is made one, in a union that has pointers
    Notes copies;
    const std::string copied = word_copies(text, copies, "u.c", {"i", "l"});
    CB_CHECK(copied.find("union tokval { intptr_t i; char *s;") != std::string::npos);
    CB_CHECK(copied.find("union num { int16_t i; int32_t l; };") != std::string::npos); // no pointer in num
    CB_CHECK(places(copies, "an int of a union") == (std::vector<std::string>{"u.c (i)"}));
    Notes quiet;
    CB_CHECK_EQ(word_copies(text, quiet, "u.c", {}), text);
});

CB_TEST("port.passes.whole_word_stores", [] {
    Notes notes;
    const std::string text = "void store(int16_t *q, intptr_t n, int m)\n{\n    intptr_t k;\n    *(int *)q = n;\n    "
                             "*(int *)q = m;\n    *(int *)q = k;\n}\n"
                             "void plain(int16_t *q, int n)\n{\n    *(int *)q = n;\n}\n";
    const std::string out = whole_word_stores(text, notes, "w.c");
    CB_CHECK_EQ(
        out,
        "void store(int16_t *q, intptr_t n, int m)\n{\n    intptr_t k;\n    *(intptr_t *)q = n;\n    *(int *)q = m;\n"
        "    *(intptr_t *)q = k;\n}\nvoid plain(int16_t *q, int n)\n{\n    *(int *)q = n;\n}\n");
    CB_CHECK(places(notes, "a word stored") == (std::vector<std::string>{"w.c:4", "w.c:6"}));
    // a definition that never closes is read to the last line (it was an IndexError in the Python)
    Notes open;
    CB_CHECK_EQ(whole_word_stores("void f(int16_t *q, intptr_t p)\n{\n    *(int *)q = p;\n", open, "a.c"),
                "void f(int16_t *q, intptr_t p)\n{\n    *(intptr_t *)q = p;\n");
});

CB_TEST("port.passes.old_c_ways", [] {
    Sources out;
    out["a.c"] = "char *g = \"xLIBCE\";\nstatic char *h = \"abc\\n\";\nlong t1;\n"
                 "void go()\n{\n    char *loc = \"inside\";\n    long when;\n    time(&when);\n    time((long "
                 "*)&when);\n    time(&t1);\n"
                 "    *(int16_t *)&p->size = dos_ph(q);\n    r = (struct blk *)(*(int16_t *)&p->size);\n"
                 "    s = (char *)(*(int16_t *)&p->other);\n}\n";
    out["b.h"] = "char *g = \"in a header\";\n";
    Notes notes;
    old_c_ways(out, notes);
    // a string's text written to: an array of its own, for the one at the top of the file only; time into a long: a
    // cast
    CB_CHECK(out["a.c"].find("char *g = (char []){\"xLIBCE\"};") != std::string::npos);
    CB_CHECK(out["a.c"].find("static char *h = (char []){\"abc\\n\"};") != std::string::npos);
    CB_CHECK(out["a.c"].find("char *loc = \"inside\";") != std::string::npos); // inside a function: left
    CB_CHECK(
        out["a.c"].find(
            "    (when = (int32_t)time(NULL));\n    (when = (int32_t)time(NULL));\n    (t1 = (int32_t)time(NULL));") !=
        std::string::npos);
    // the word that held a handle (stored through dos_ph) is read through dos_hp, the other is not
    CB_CHECK(out["a.c"].find("r = (struct blk *)dos_hp(*(int16_t *)&p->size);") != std::string::npos);
    CB_CHECK(out["a.c"].find("s = (char *)(*(int16_t *)&p->other);") != std::string::npos);
    CB_CHECK_EQ(out["b.h"], "char *g = \"in a header\";\n"); // only .c files
    CB_CHECK(places(notes, "a handle kept").size() == 1u);
    CB_CHECK(places(notes, "what old C allowed") == (std::vector<std::string>{"a.c (5)"}));
});

CB_TEST("port.passes.numbered_places", [] {
    Sources out;
    out["a.c"] = "#include \"p.h\"\nvoid go()\n{\n    f06988(f021ff, (struct hsym **)0x3a20);\n    f(2, (char "
                 "*)(0x3a20));\n    use((char **)0x4444);\n}\n";
    out["p.h"] = "struct hsym { int a; };\n";
    Notes notes;
    numbered_places(out, {"struct hsym *g_3a20;", "static int g_3a21 = 3;"}, notes);
    // the number of a variable the program defines is the variable, its extern declared where it is not
    CB_CHECK_EQ(
        out["a.c"],
        "#include \"p.h\"\nextern struct hsym *g_3a20;\nvoid go()\n{\n    f06988(f021ff, (struct hsym **)g_3a20);\n"
        "    f(2, (char *)(g_3a20));\n    use((char **)0x4444);\n}\n");
    CB_CHECK(places(notes, "a variable's place") == (std::vector<std::string>{"a.c (g_3a20)"}));
    // nothing defined: nothing to do
    Sources empty;
    empty["a.c"] = "void f() { g((char **)0x3a20); }\n";
    Notes quiet;
    numbered_places(empty, {}, quiet);
    CB_CHECK_EQ(empty["a.c"], "void f() { g((char **)0x3a20); }\n");
});

CB_TEST("port.passes.agree_wide", [] {
    Sources out;
    out["a.c"] = "extern intptr_t g_74be;\nint16_t g_8672 = 0;\nint16_t g_0cbc;\nvoid f() { int16_t g_74be; }\n";
    out["b.c"] = "extern int16_t g_74be, g_1;\nextern int g_8672;\nint16_t table_place = 0;\nstruct row tab[] = { { "
                 "'a', 1, &g_8672 } };\n";
    Notes notes;
    agree_wide(out, {"intptr_t g_8672;"}, notes, Rules());
    CB_CHECK_EQ(out["b.c"],
                "extern int16_t g_1; extern intptr_t g_74be;\nextern intptr_t g_8672;\nint16_t table_place = 0;\n"
                "struct row tab[] = { { 'a', 1, (void *)&g_8672 } };\n");
    // with the rule that makes definitions agree, the definition is made so too
    CB_CHECK(out["a.c"].find("intptr_t g_8672 = 0;") != std::string::npos);
    CB_CHECK(out["a.c"].find("int16_t g_0cbc;") != std::string::npos);              // a variable of its own
    CB_CHECK(out["a.c"].find("void f() { int16_t g_74be; }") != std::string::npos); // a local, not the global
    CB_CHECK(places(notes, "a variable intptr_t in some files").size() == 2u);
    // without it only the extern declarations are
    Sources off;
    off["a.c"] = "extern intptr_t g_74be;\nint16_t g_74be2;\n";
    off["b.c"] = "extern int16_t g_74be;\nint16_t g_74be = 0;\n";
    Notes none;
    agree_wide(off, {}, none, Rules("-agree_wide_defs"));
    CB_CHECK_EQ(off["b.c"], "extern intptr_t g_74be;\nint16_t g_74be = 0;\n");
    Sources nothing;
    nothing["a.c"] = "extern int16_t g_1;\n";
    Notes quiet;
    agree_wide(nothing, {}, quiet, Rules());
    CB_CHECK(quiet.empty());
});

CB_TEST("port.passes.widen_member", [] {
    const std::string text = "struct list { int s4, s6, s8; char *name; };\n"
                             "struct node { int16_t a; unsigned b; /* (a handle) */ int16_t c; };\n"
                             "struct handle { int16_t h; /* (a handle) */ };\n"
                             "struct two { char c; short t, u; int32_t big; };\n";
    CB_CHECK_EQ(*widen_member(text, "list", "s6"), "struct list { int s4; intptr_t s6; int s8; char *name; };\n" +
                                                       text.substr(text.find("struct node")).insert(0, ""));
    CB_CHECK(widen_member(text, "list", "s4")->starts_with("struct list { intptr_t s4; int s6, s8; char *name; };"));
    CB_CHECK(widen_member(text, "list", "s8")->starts_with("struct list { int s4, s6; intptr_t s8; char *name; };"));
    CB_CHECK(!widen_member(text, "list", "name")); // a pointer already
    CB_CHECK(!widen_member(text, "handle", "h"));  // a pointer's handle: left
    CB_CHECK(!widen_member(text, "node", "b"));    // (a handle) is on its line
    CB_CHECK(widen_member(text, "two", "t")->find("struct two { char c; intptr_t t; short u; int32_t big; };") !=
             std::string::npos);
    CB_CHECK(!widen_member(text, "two", "big")); // not a 16-bit integer
    CB_CHECK(!widen_member(text, "none", "x"));
    CB_CHECK(!widen_member("struct a { int x;", "a", "x")); // no closing brace
    CB_CHECK(!widen_member("", "a", "x"));
});
