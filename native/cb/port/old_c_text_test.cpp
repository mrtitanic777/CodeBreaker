// Tests of the text passes that rewrite one file at a time (old_c_text): widths, formats, mem_sizes, header_offsets,
// struct_sizes, old_declarations, header_data. The expected values are what the Python original (pre_ansi.py) made of
// the same text, read through by hand here; and for the real trees of MS C 3.00 every pass on every file, by CRC, from
// testdata/views/make_views_cases.py.
#include <set>

#include "cb/base/ctext.hpp"
#include "cb/core/testing.hpp"
#include "cb/port/old_c_text.hpp"
#include "cb/port/program_prep.hpp"
#include "cb/port/struct_layout.hpp"
#include "cb/port/views_case.hpp"

using namespace cb;
using namespace cb::port::pre_ansi;
using namespace cb::port::pre_ansi::vcase;

namespace {

using Want = std::vector<std::pair<std::string, std::vector<std::string>>>;

void check_notes(const Notes& got, const Want& want) {
    CB_CHECK_EQ(got.size(), want.size());
    std::size_t i = 0;
    for (const auto& [why, places] : got) {
        if (i >= want.size()) break;
        CB_CHECK_EQ(why, want[i].first);
        CB_CHECK(places == want[i].second);
        ++i;
    }
}

StructLayouts layouts_of(const std::string& text) {
    const std::vector<std::string> texts{text};
    return lay_out_structs(texts, 2, 4);
}

}  // namespace

CB_TEST("port.old_c_text.widths_long_is_int32_and_struct_members_are_words", [] {
    const std::string text =
        "struct s { int a; unsigned b; short c; unsigned char d; };\nlong x;\nunsigned long y;\n"
        "int f(p) int *p; { return (int *)p; }\nmain(argc, argv) int argc; { return 0; }\n";
    Notes notes;
    CB_CHECK_EQ(widths(text, notes, "w.c", false),
                "struct s { int16_t a; uint16_t b; int16_t c; unsigned char d; };\nint32_t x;\nuint32_t y;\n"
                "int f(p) int *p; { return (int16_t *)p; }\nmain(argc, argv) int argc; { return 0; }\n");
    check_notes(notes, {{"struct member unsigned -> uint16_t (the structure's bytes as they were)", {"w.c:1"}},
                        {"struct member int -> int16_t (the structure's bytes as they were)", {"w.c:1", "w.c:1"}},
                        {"pointer cast (int *) -> (int16_t *): a word of a buffer", {"w.c:4"}},
                        {"unsigned long -> uint32_t", {"w.c:3"}},
                        {"long -> int32_t (32 bits, as it was)", {"w.c:2"}}});
    // with int16 every int is a word, but main's own line is the system's
    Notes wide;
    CB_CHECK_EQ(widths(text, wide, "w.c", true),
                "struct s { int16_t a; uint16_t b; int16_t c; unsigned char d; };\nint32_t x;\nuint32_t y;\n"
                "int16_t f(p) int16_t *p; { return (int16_t *)p; }\nmain(argc, argv) int argc; { return 0; }\n");
    check_notes(wide, {{"struct member unsigned -> uint16_t (the structure's bytes as they were)", {"w.c:1"}},
                       {"struct member int -> int16_t (the structure's bytes as they were)", {"w.c:1", "w.c:1"}},
                       {"pointer cast (int *) -> (int16_t *): a word of a buffer", {"w.c:4"}},
                       {"unsigned long -> uint32_t", {"w.c:3"}},
                       {"long -> int32_t (32 bits, as it was)", {"w.c:2"}},
                       {"int -> int16_t (16 bits, as it was)", {"w.c:4", "w.c:4"}}});
});

CB_TEST("port.old_c_text.widths_leave_strings_and_comments_alone", [] {
    Notes notes;
    CB_CHECK_EQ(widths("char *s = \"long int\"; /* a long one */\nlong n;\n", notes, "a.c", false),
                "char *s = \"long int\"; /* a long one */\nint32_t n;\n");
});

CB_TEST("port.old_c_text.formats_give_a_long_arguments_the_cast_its_format_says", [] {
    const std::string text =
        "printf(\"%ld %lx %d %Fs %x\\n\", x, y, z, s, w);\nfprintf(f, \"%lu\\n\", a + b);\nsprintf(buf, \"%x %u\", (long)k, m);\n";
    Notes notes;
    CB_CHECK_EQ(formats(text, notes, "f.c", false, false),
                "printf(\"%ld %lx %d %s %x\\n\", (long)x, (long)y, z, s, w);\nfprintf(f, \"%lu\\n\", (long)(a + b));\n"
                "sprintf(buf, \"%x %u\", (long)k, m);\n");
    check_notes(notes, {{"%Fs / %Np -> %s / %p (Microsoft's printf: a far or near pointer; one flat memory now)", {"f.c:1"}},
                        {"argument of a %l. format given as (long) (a long is int32_t here)", {"f.c:1", "f.c:1", "f.c:2"}}});
    Notes wide;   // (int16 and the old printf's capitals)
    CB_CHECK_EQ(formats(text, wide, "f.c", true, true),
                "printf(\"%ld %lX %d %s %X\\n\", (long)x, (long)y, z, s, (uint16_t)w);\nfprintf(f, \"%lu\\n\", (long)(a + b));\n"
                "sprintf(buf, \"%X %u\", (uint16_t)((long)k), (uint16_t)m);\n");
    check_notes(wide, {{"%Fs / %Np -> %s / %p (Microsoft's printf: a far or near pointer; one flat memory now)", {"f.c:1"}},
                       {"%x -> %X (the old printf gave its hexadecimal in capitals)", {"f.c:1", "f.c:3"}},
                       {"argument of a %l. format given as (long) (a long is int32_t here)", {"f.c:1", "f.c:1", "f.c:2"}},
                       {"argument of a %x / %u format given as (uint16_t) (16 bits, as the old printf showed)", {"f.c:1", "f.c:3", "f.c:3"}}});
});

CB_TEST("port.old_c_text.formats_read_the_text_they_are_given_not_an_earlier_reading_of_it", [] {
    // DIVERGES: the Python was handed the code of the text as it was before the passes in between had changed it (offsets
    // of a different text); the code is the text's own here. A pass that grew the line before the call put the old
    // offsets inside the wrong argument.
    Notes notes;
    const std::string grown = "uint32_t x = 1; uint32_t y = 2;\nprintf(\"%ld\", x);\n";
    CB_CHECK_EQ(formats(grown, notes, "g.c", false, false), "uint32_t x = 1; uint32_t y = 2;\nprintf(\"%ld\", (long)x);\n");
});

CB_TEST("port.old_c_text.mem_sizes_give_a_global_arrays_bytes_as_sizeof", [] {
    const OrderedMap<std::string, std::uint64_t> olds{{"g_seg", 24}, {"g_tab", 20}};
    const OrderedMap<std::string, std::uint64_t> elems{{"g_seg", 4}};
    Notes notes;
    CB_CHECK_EQ(mem_sizes("memset(g_seg, 0, 24);\nmemcpy(&g_seg, p, 24);\nmemset(other, 0, 24);\nmemset(g_seg, 0, 12);\nmemset(g_tab, 0, 20);\n",
                          olds, elems, notes, "m.c"),
                "memset(g_seg, 0, 6 * sizeof(g_seg[0]));\nmemcpy(&g_seg, p, 6 * sizeof(g_seg[0]));\nmemset(other, 0, 24);\n"
                "memset(g_seg, 0, 12);\nmemset(g_tab, 0, sizeof(g_tab));\n");
    check_notes(notes, {{"the bytes of a global array given as the number it had: sizeof it now",
                         {"m.c:1 (g_seg: 24)", "m.c:2 (g_seg: 24)", "m.c:5 (g_tab: 20)"}}});
});

CB_TEST("port.old_c_text.header_offsets_name_the_head_a_trailing_array_follows", [] {
    const std::string text =
        "struct blk { int next; int len; char data[2]; };\nf(n, c, i)\n{\n\tchar *p;\n\tstruct blk *b;\n\tp = alloc(n + 4);\n"
        "\tp[i + 4] = c;\n\tb = (struct blk *)p;\n\treturn 0;\n}\n";
    Notes notes;
    CB_CHECK_EQ(header_offsets(text, layouts_of(text), notes, "h.c"),
                "struct blk { int next; int len; char data[2]; };\nf(n, c, i)\n{\n\tchar *p;\n\tstruct blk *b;\n"
                "\tp = alloc(n + offsetof(struct blk, data));\n\tp[i + offsetof(struct blk, data)] = c;\n\tb = (struct blk *)p;\n\treturn 0;\n}\n");
    check_notes(notes, {{"a structure's head given as the bytes it was (its array after it): offsetof",
                         {"h.c:6 (offsetof(struct blk, data))", "h.c:7 (offsetof(struct blk, data))"}}});
});

CB_TEST("port.old_c_text.struct_sizes_give_a_structures_size_as_sizeof", [] {
    const std::string text =
        "struct s { int a; char *b; int c; };\nf()\n{\n\tstruct s *p, *q;\n\tchar *r;\n\tp = (struct s *)malloc(6);\n"
        "\tq = (struct s *)xalloc(6 + 2);\n\tr = (char **)malloc(8);\n\treturn 0;\n}\n";
    Notes notes;
    AllocatorSizes pools;
    Allocators allocs;
    allocs.insert_or_assign("xalloc", std::set<std::size_t>{0});
    CB_CHECK_EQ(struct_sizes(text, layouts_of(text), notes, "s.c", allocs, pools),
                "struct s { int a; char *b; int c; };\nf()\n{\n\tstruct s *p, *q;\n\tchar *r;\n"
                "\tp = (struct s *)malloc(sizeof(struct s));\n\tq = (struct s *)xalloc(sizeof(struct s) + 2);\n"
                "\tr = (char **)malloc( 4 * sizeof(char *));\n\treturn 0;\n}\n");
    check_notes(notes, {{"a table of pointers given by its bytes then (two a pointer): as many of today's", {"s.c:8 (char **: 8)"}},
                        {"a structure's size given as the number it had (16-bit layout): sizeof and the rest",
                         {"s.c:6 (struct s: 6)", "s.c:7 (struct s: 6)"}}});
    // the program's own allocator that was given the structure's whole old size is remembered
    CB_CHECK_EQ(pools.size(), 1u);
    CB_CHECK(pools.at("xalloc").contains({"struct s", 6}));
});

CB_TEST("port.old_c_text.old_extern_declarations_go_when_the_program_or_library_declares_them", [] {
    Notes notes;
    const std::set<std::string, std::less<>> known{"f", "h", "lookup"};
    CB_CHECK_EQ(old_declarations("extern int f(), g();\nextern int h(),\n\tk();\nextern char *name, *lookup();\nint own();\nextern int ipn_core;\n",
                                 known, notes, "d.h", {"ipn_core"}),
                "extern int g();\nextern int k();\nextern char *name;\nint own();\n");
    check_notes(notes, {{"old declaration removed (proto.h / the C library declare it)", {"d.h:1", "d.h:2", "d.h:3", "d.h:5"}}});
});

CB_TEST("port.old_c_text.a_headers_variables_become_extern_with_a_definition_for_data_c", [] {
    Notes notes;
    const HeaderData h = header_data("int g_count;\nchar buf[10];\nextern int seen;\nstatic int hidden;\nint (*hook)();\nint f();\nstruct s { int a; } v;\n", "h.h", notes);
    CB_CHECK_EQ(h.text, "extern int g_count;\nextern char buf[10];\nextern int seen;\nstatic int hidden;\nextern int (*hook)();\nint f();\nstruct s { int a; } v;\n");
    CB_CHECK(h.definitions == (std::vector<std::string>{"int g_count;", "char buf[10];", "int (*hook)();"}));
    check_notes(notes, {{"shared variable of a header: extern there, defined once in data.c", {"h.h:1", "h.h:2", "h.h:5"}}});
});

CB_TEST("port.old_c_text.prefix_member_names_the_member_a_block_of_that_many_bytes_ended_at", [] {
    const StructLayouts lays = layouts_of("struct s { int a; char *b; int c; };\n");
    const StructLayout& s = lays.at("struct s");
    CB_CHECK_EQ(prefix_member(s, 2).value_or(""), "b");
    CB_CHECK_EQ(prefix_member(s, 4).value_or(""), "c");
    CB_CHECK(!prefix_member(s, 0));       // (not its start)
    CB_CHECK(!prefix_member(s, s.size));  // (not its end)
    CB_CHECK(!prefix_member(s, 3));       // (no member begins there)
});

namespace {

// Every pass on every file of a real tree, by CRC, as the Python answered them.
void check_real(const std::string& name) {
    const Case c = load(name);
    const Profile profile = profile_of(c);
    OrderedMap<std::string, std::string> members;
    for (const auto& [k, v] : c.expected["members"].items()) members.insert_or_assign(k, v.get<std::string>());
    const PreparedProgram p = prepare_program(c.originals, profile, members);
    OriginalProgram original(p.originals, profile.old_pointer_bytes());
    std::set<std::string, std::less<>> known(profile.libc.begin(), profile.libc.end());
    for (const auto& kv : p.files)
        for (const Definition& d : definitions(split_lines(kv.second), split_lines(base::ctext::blank(kv.second)))) known.insert(d.fn.name);
    const OrderedMap<std::string, std::uint64_t> var_sizes = original.var_sizes({}), elems = original.elem_sizes();
    const Allocators allocs = original.allocators(p.files);
    std::vector<std::string> names;
    for (const auto& kv : p.files) names.push_back(kv.first);
    std::sort(names.begin(), names.end());
    std::size_t compared = 0;
    for (const std::string& n : names) {
        const std::string& t = p.files.at(n);
        const J& want = c.expected["text_passes"][n];
        auto check = [&](const char* pass, const std::string& text, const Notes& notes) {
            CHECK_J(crc(text), want[pass][0]);
            CHECK_J(notes_json(notes), want[pass][1]);
            ++compared;
        };
        {
            Notes notes;
            const std::string r = widths(t, notes, n, p.int16);
            check("widths", r, notes);
        }
        {
            Notes notes;
            const std::string r = formats(t, notes, n, p.int16, profile.printf_hex_upper);
            check("formats", r, notes);
        }
        if (n.ends_with(".h")) {
            Notes notes;
            const HeaderData h = header_data(t, n, notes);
            CHECK_J(crc(h.text), want["header_data"][0]);
            CHECK_J(J(h.definitions), want["header_data"][1]);
            CHECK_J(notes_json(notes), want["header_data"][2]);
        }
        {
            Notes notes;
            const std::string r = old_declarations(t, known, notes, n, profile.platform_calls);
            check("old_declarations", r, notes);
        }
        {
            Notes notes;
            const std::string r = mem_sizes(t, var_sizes, elems, notes, n);
            check("mem_sizes", r, notes);
        }
        {
            Notes notes;
            const std::string r = header_offsets(t, original.layouts_for(n), notes, n);
            check("header_offsets", r, notes);
        }
        {
            Notes notes;
            AllocatorSizes pools;
            const std::string r = struct_sizes(t, original.layouts_for(n), notes, n, allocs, pools);
            check("struct_sizes", r, notes);
            J sizes = J::array();
            for (const auto& kv : pools) {
                J list = J::array();
                for (const auto& [tag, size] : kv.second) list.push_back(J::array({tag, size}));
                sizes.push_back(J::array({kv.first, list}));
            }
            std::vector<J> sorted(sizes.begin(), sizes.end());
            std::sort(sorted.begin(), sorted.end(), [](const J& a, const J& b) { return a.dump() < b.dump(); });
            CHECK_J(J(sorted), want["struct_sizes"][2]);
        }
    }
    CB_CHECK(compared >= names.size() * 6);
}

}  // namespace

CB_TEST("port.old_c_text.every_pass_on_every_file_of_the_synthetic_programs", [] {
    for (const std::string& name : synthetic_names()) check_real(name);
});

CB_TEST("port.old_c_text.every_pass_on_every_file_of_the_ms_c_3_00_trees", [] {
    for (const std::string& name : real_names()) check_real(name);
});
