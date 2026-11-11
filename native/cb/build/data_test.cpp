// Tests of build/data: a module's initialised data as C. The real-module cases compare with what the Python
// build/data.py wrote for the same files (testdata/make_data_golden.py made the goldens; their contents were read
// through by hand: the object layouts, the pointers, the unresolved list). The rest is written: the text of an object,
// the cut of pieces, the rewrites of a header and a function, the layout check, and damaged input.
//
// DIVERGES from the Python:
//   * relocate_header: a constant of more than 64 bits is left alone and not listed in `lost` (the Python listed it as
//   a
//     number of any size; no real header has one).
//   * a far-pointer relocation in the last two bytes of a segment cannot be written past the segment's end (the
//   Python's
//     slice assignment grew the segment by two bytes, which nothing could use): the bytes there are cleared and no
//     more.
//   * layout_holds is false for an object file that cannot be read (the Python's hand parser took what it could).
#include "cb/build/data.hpp"

#include <format>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

// (a container is compared with ==; the failure message of CB_CHECK_EQ cannot render one)
#define CHECK_SAME(a, ...) CB_CHECK((a) == (__VA_ARGS__))

namespace {

using namespace cb;
using namespace cb::build::data;
namespace pe = cb::formats::pe;
namespace ne = cb::formats::ne;

std::string golden(const std::string& name) {
    return fs::read_text(testing::data_path("build/testdata/" + name));
}

pe::Image image(const std::string& name) {
    return pe::Image::open(testing::data_path("formats/testdata/" + name));
}
ne::Module module(const std::string& name) {
    return ne::Module::open_file(testing::data_path("formats/testdata/" + name));
}

// The stand-in for the reconstructed functions that the goldens were made with: every address in .text has one.
FunctionAt32 functions_in_text(const pe::Image& img) {
    std::uint64_t lo = 0, hi = 0;
    for (const pe::Section& s : img.sections()) {
        if (s.name == ".text") {
            lo = img.header().image_base + s.virtual_address;
            hi = lo + s.virtual_size;
        }
    }
    return [lo, hi](std::uint64_t va) -> std::optional<std::string> {
        if (lo <= va && va < hi)
            return std::format("fn_{:x}", va);
        return std::nullopt;
    };
}

// In a 16-bit module the code segments have functions, and the even segments a symbol.
Names16 names_of(const ne::Module& m) {
    std::set<std::uint16_t> code;
    for (const ne::Segment& s : m.segments()) {
        if (!s.is_data())
            code.insert(s.index);
    }
    Names16 names;
    names.function_at = [code](Address16 a) -> std::optional<std::string> {
        if (code.contains(a.segment))
            return std::format("fn_{}_{:04x}", a.segment, a.offset);
        return std::nullopt;
    };
    names.segment_symbol = [](std::uint16_t seg) -> std::optional<std::string> {
        if (seg % 2 == 0)
            return std::format("seg_{}", seg);
        return std::nullopt;
    };
    return names;
}

// The one-per-line description of a Data that the goldens hold.
std::string summary(const Data& d) {
    std::string out = std::format("pointers={} size={} movable={}\n", d.pointers, d.size, d.movable ? "True" : "False");
    for (const Section& s : d.sections)
        out += std::format("section {} {} {:x} {}\n", s.number, s.name, s.va, s.size);
    for (const Part& p : d.parts)
        out += std::format("part {} {:x} {} {}\n", p.symbol, p.va, p.size, p.section);
    for (const Unresolved& u : d.unresolved)
        out += std::format("unresolved {} | {}\n", u.where, u.why);
    for (const auto& [symbol, import] : d.imports) {
        const std::string by = std::holds_alternative<std::uint16_t>(import.by)
                                   ? "#" + std::to_string(std::get<std::uint16_t>(import.by))
                                   : std::get<std::string>(import.by);
        out += std::format("import {} {} {}\n", symbol, import.module, by);
    }
    for (const auto& [at, name] : d.exported)
        out += std::format("exported {} {:x} {}\n", at.first, at.second, name);
    for (const auto& [off, name] : d.cuts)
        out += std::format("cut {:x} {}\n", off, name);
    return out;
}

void check_data(const Data& d, const std::string& stem, bool with_declarations) {
    CB_CHECK_EQ(d.source, golden(stem + ".golden.c"));
    CB_CHECK_EQ(summary(d), golden(stem + ".golden.txt"));
    if (with_declarations)
        CB_CHECK_EQ(d.declarations, golden(stem + ".golden.decl"));
}

} // namespace

// ---- the text of an object ---------------------------------------------------------------------------------------

CB_TEST("build.data.object_text", [] {
    const std::vector<Piece> pieces{ByteRun{0, {0x41, 0x2A, 0x2F, 0x5C, 0x7F, 0x20}},
                                    PointerCell{6, "void *", "(void *)f", ""},
                                    PointerCell{10, "uint2", "(uint2)X", "a note"}, ZeroRun{14, 100}};
    const std::string c = object("layout_of_x", "x", pieces);
    CB_CHECK_EQ(c, "#pragma pack(push, 1)\n"
                   "struct layout_of_x {\n"
                   "    uint1 b_00000[6];\n"
                   "    void * p_00006;\n"
                   "    uint2 p_0000a;\n"
                   "    uint1 z_0000e[100];\n"
                   "};\n"
                   "#pragma pack(pop)\n"
                   "struct layout_of_x x = {\n"
                   "    {\n" +
                       std::format("        {:<96}/* A....  */\n", "0x41,0x2a,0x2f,0x5c,0x7f,0x20,") +
                       "    },\n"
                       "    (void *)f,\n"
                       "    (uint2)X,    /* a note */\n"
                       "};\n"); // (the bytes' comment shows the printable ones, not * / \ ; the trailing zeros have no
                                // initialiser)
});

CB_TEST("build.data.object_placement", [] {
    const std::vector<Piece> just_zeros{ZeroRun{0, 70}};
    const std::string z = object("s", "n", just_zeros, true);
    CB_CHECK(z.find("struct s __far n = {\n    { 0 },\n};\n") != std::string::npos); // alone, it keeps its initialiser
    const std::vector<Piece> some{ByteRun{0, {1}}};
    CB_CHECK(object("s", "n", some, false, "CODEBREAKER_SEGMENT_3")
                 .find("struct s __based(__segname(\"CODEBREAKER_SEGMENT_3\")) n = {") != std::string::npos);
});

CB_TEST("build.data.rows_are_sixteen_bytes", [] {
    Bytes b(40);
    for (std::size_t i = 0; i < b.size(); ++i)
        b[i] = static_cast<std::uint8_t>('a' + i);
    const std::vector<Piece> pieces{ByteRun{0, b}};
    const std::string c = object("s", "n", pieces);
    CB_CHECK_EQ(text::split(c, '\n').size(), 13u); // 3 rows of values among the lines of the struct, and the empty end
    CB_CHECK(c.find("/* abcdefghijklmnop */") != std::string::npos);
    CB_CHECK(c.find("/* qrstuvwxyz{|}~.. */") != std::string::npos); // 0x7F and 0x80 are not printable
    CB_CHECK(c.find("/* ........ */") != std::string::npos);
});

// ---- cutting an object into pieces --------------------------------------------------------------------------------

CB_TEST("build.data.pieces_long_zero_runs_and_pointers", [] {
    Bytes blob(300, 0);
    blob[0] = 7;
    blob[10] = 8;
    blob[200] = 9;
    // bytes 1..9 (9 zeros) are no run; 11..149 (139) is; the pointer cuts it; 154..199 (46) is no run; 201..299 (99) is
    const Pointers pointers{{150, {4, "void *", "p", ""}}};
    const std::vector<Piece> out = cut_pieces(blob, 300, pointers);
    // bytes 0..10, zeros 11..149, the pointer at 150, bytes 154..200, zeros 201..299
    CB_CHECK_EQ(out.size(), 5u);
    CB_CHECK(std::get<ByteRun>(out[0]).bytes.size() == 11 && std::get<ByteRun>(out[0]).offset == 0);
    CB_CHECK(std::get<ZeroRun>(out[1]).offset == 11 && std::get<ZeroRun>(out[1]).count == 139);
    CB_CHECK(std::get<PointerCell>(out[2]).offset == 150);
    CB_CHECK(std::get<ByteRun>(out[3]).offset == 154 &&
             std::get<ByteRun>(out[3]).bytes.size() == 47); // 46 zeros and the 9
    CB_CHECK(std::get<ZeroRun>(out[4]).offset == 201 && std::get<ZeroRun>(out[4]).count == 99);
});

CB_TEST("build.data.pieces_pad_a_short_blob_and_skip_overlaps", [] {
    // the blob is shorter than the object: the rest is zeros, as long runs
    const std::vector<Piece> padded = cut_pieces(Bytes{1, 2, 3}, 200, {});
    CB_CHECK_EQ(padded.size(), 2u);
    CB_CHECK(std::get<ByteRun>(padded[0]).bytes == (Bytes{1, 2, 3}));
    CB_CHECK(std::get<ZeroRun>(padded[1]).offset == 3 && std::get<ZeroRun>(padded[1]).count == 197);
    // a pointer that begins inside the one before is left out
    const Pointers overlapping{{4, {4, "T", "a", ""}}, {6, {4, "T", "b", ""}}, {8, {4, "T", "c", ""}}};
    const std::vector<Piece> out = cut_pieces(Bytes(12, 1), 12, overlapping);
    std::vector<std::string> names;
    for (const Piece& p : out) {
        if (const auto* c = std::get_if<PointerCell>(&p))
            names.push_back(c->expression);
    }
    CHECK_SAME(names, (std::vector<std::string>{"a", "c"}));
    CB_CHECK(cut_pieces({}, 0, {}).empty());
    // a pointer that runs past the end of the object
    CB_CHECK_EQ(cut_pieces(Bytes(6, 1), 6, Pointers{{4, {4, "T", "a", ""}}}).size(), 2u);
});

// ---- 32-bit modules ------------------------------------------------------------------------------------------------

CB_TEST("build.data.data32_lz32", [] {
    const pe::Image img = image("lz32.dll");
    check_data(data32(img, "LZ32.DLL", functions_in_text(img)), "data_lz32_dll", true);
});

CB_TEST("build.data.data32_desk_cpl", [] {
    const pe::Image img = image("desk.cpl");
    check_data(data32(img, "desk.cpl", functions_in_text(img)), "data_desk_cpl", true);
});

// comcat.dll has a .rdata of tables of pointers (66 of them) and a .data: two carried sections, and pointers that point
// into the other one
CB_TEST("build.data.data32_comcat", [] {
    const pe::Image img = image("comcat.dll");
    const Data d = data32(img, "comcat.dll", functions_in_text(img));
    check_data(d, "data_comcat_dll", true);
    CB_CHECK_EQ(d.sections.size(), 2u);
    CB_CHECK(d.movable);
    CB_CHECK(d.unresolved.empty());
});

CB_TEST("build.data.data32_exported_names_cut_a_section", [] {
    const pe::Image img = image("lz32.dll");
    const std::uint64_t base = img.header().image_base;
    const Data d =
        data32(img, "lz32.dll", functions_in_text(img), {{base + 0x2010, "g_exported"}, {base + 0x2040, "g_second"}});
    check_data(d, "data_lz32_named", true);
    CB_CHECK_EQ(d.parts.size(), 3u);
    // the cut at the very start of a section names that part too; one outside every section is ignored
    const Data e =
        data32(img, "lz32.dll", functions_in_text(img), {{base + 0x2000, "g_first"}, {base + 0x9000, "g_nowhere"}});
    CB_CHECK_EQ(e.parts.size(), 1u);
    CB_CHECK_EQ(e.parts[0].symbol, "g_first");
});

CB_TEST("build.data.data32_without_function_names", [] {
    // no function_at: every pointer into the code is unresolved
    const pe::Image img = image("lz32.dll");
    const Data d = data32(img, "lz32.dll", nullptr);
    CB_CHECK_EQ(d.pointers, 0u);
    CB_CHECK_EQ(d.unresolved.size(), 2u);
    CB_CHECK(d.unresolved[0].why.find("neither a reconstructed function nor carried data") != std::string::npos);
    // an empty answer is no answer
    const Data e = data32(img, "lz32.dll", [](std::uint64_t) -> std::optional<std::string> { return std::string(); });
    CB_CHECK_EQ(e.unresolved.size(), 2u);
});

CB_TEST("build.data.relocated_words_and_address_constants", [] {
    const pe::Image img = image("lz32.dll");
    const RelocatedWords words = relocated_words(img);
    std::string text;
    for (const auto& [va, value] : words)
        text += std::format("word {:x} {:x}\n", va, value);
    const std::uint64_t lo = img.header().image_base + 0x1000;
    const std::vector<std::uint64_t> starts{lo + 0x300, lo, lo + 0x200, lo + 0x100}; // any order
    const auto constants = address_constants(img, starts);
    CB_CHECK_EQ(constants.size(), 4u);
    for (const std::uint64_t s : {lo, lo + 0x100, lo + 0x200, lo + 0x300}) {
        text += std::format("constants {:x}", s);
        for (const std::uint64_t a : constants.at(s))
            text += std::format(" {:x}", a);
        text += "\n";
    }
    CB_CHECK_EQ(text, golden("data_lz32_words.golden.txt"));
    CB_CHECK(address_constants(img, {}).empty());
});

// ---- rewriting the header and the functions ---------------------------------------------------------------------

CB_TEST("build.data.relocate_header_and_text_of_lz32", [] {
    const pe::Image img = image("lz32.dll");
    const std::uint64_t base = img.header().image_base;
    const FunctionAt32 functions = functions_in_text(img);
    const Data d = data32(img, "lz32.dll", functions, {{base + 0x2010, "g_exported"}, {base + 0x2040, "g_second"}});
    const std::uint64_t va0 = d.parts[0].va, lo = base + 0x1000;

    const std::string header =
        std::format("#define g_a (*(int *)0x{:x})\n#define g_b (*(char *)0x{:x})\nint helper(void);\n"
                    "#define g_far (*(int *)0x{:x})\n#define g_upper (*(int *)0x{:X})\n",
                    va0 + 4, va0 + 0x44, 0x7fff0000, va0);
    const Relocated h = relocate_header(header, d);
    std::string shown = h.text + "LOST";
    for (const std::uint64_t x : h.lost)
        shown += std::format(" {:x}", x);
    CB_CHECK_EQ(shown + "\n", golden("data_lz32_header.golden.txt"));

    const std::string body = std::format(
        "int f(void) {{ int a = *(int *)0x{:x}; int b = x-0x{:x}; int c = -0x{:x}; int d = 0x{:x}; int e = 0x{:x}U; "
        "int g = 0x{:x}; return v.0x{:x} + 0x{:x}; }}\n",
        va0 + 4, va0 + 8, (1ull << 32) - (lo + 8), lo + 0x10, va0 + 0x44, 0x12345, va0, lo + 0x20);
    const std::string text = std::format("static int pre = 0x{:x};\n", va0 + 4) + body;
    const std::set<std::uint64_t> addresses{va0 + 4,   va0 + 8,   va0 + 0x44, lo + 8,
                                            lo + 0x10, lo + 0x20, 0x12345,    0x55550000};
    const Relocated t = relocate_text(text, addresses, d, functions);
    std::string both = "INPUT\n" + text + "OUTPUT\n" + t.text + "LOST";
    for (const std::uint64_t x : t.lost)
        both += std::format(" {:x}", x);
    CB_CHECK_EQ(both + "\n", golden("data_lz32_text.golden.txt"));
});

CB_TEST("build.data.relocate_text_rules", [] {
    Data d;
    d.parts = {{"codebreaker_data_2", 0x1000, 0x100, 2}};
    const FunctionAt32 fn = [](std::uint64_t va) -> std::optional<std::string> {
        if (va == 0x2000)
            return "f2";
        if (va == 0x3000)
            return std::string(); // an empty name is none
        return std::nullopt;
    };
    const std::set<std::uint64_t> addresses{0x1010, 0x2000, 0x3000, 0xFFFFE000, 0x4000, 0x1ABC + 0x100};
    const std::string in =
        "void f() { a = 0x1010; b = 0x2000; c = -0x1010; d = 0x1010L; e = 0x1010 + 0x1ABC; g = 0X1010; h = x0x1010; "
        "k = 0x00001010; m = 0x100001010; n = -0x100000000; p = -0xFFFFEFF0; q = (0x2000); r = 0x3000; s=.0x2000; "
        "t=-0x2000;"
        " u = 0x1010,0x1010;}";
    const std::string place = "((int4)((char *)&codebreaker_data_2 + 0x10))";
    const Relocated r = relocate_text(in, addresses, d, fn);
    CB_CHECK_EQ(r.text, "void f() { a = " + place + "; b = ((int4)f2); c = -0x1010; d = 0x1010L; e = " + place +
                            " + 0x1ABC; g = 0X1010; h = x0x1010; k = " + place +
                            "; m = 0x100001010; n = -0x100000000; p = " + place +
                            "; q = (((int4)f2)); r = 0x3000; s=.0x2000; t=-0x2000; u = " + place + "," + place + ";}");
    CHECK_SAME(r.lost,
               (std::vector<std::uint64_t>{0x3000, 0xFFFFE000})); // addresses that are neither functions nor data
    // only the text after the first `{` is rewritten; without one nothing is
    CB_CHECK_EQ(relocate_text("no braces 0x1010", addresses, d, fn).text, "no braces 0x1010");
    CB_CHECK_EQ(relocate_text("x { }", addresses, d, fn).text, "x { }");
    CB_CHECK_EQ(relocate_text("int x = 0x1010; { y = 0x1010; }", addresses, d, fn).text,
                "int x = 0x1010; { y = " + place + "; }");
    CB_CHECK(relocate_text("", addresses, d, fn).text.empty());
    // an address not among the function's own is not an address
    CB_CHECK_EQ(relocate_text("{ 0x1010 }", {}, d, fn).text, "{ 0x1010 }");
});

CB_TEST("build.data.relocate_header_rules", [] {
    Data d;
    d.parts = {{"codebreaker_data_2", 0x1000, 0x100, 2}};
    d.declarations = "DECL\n";
    const std::string in =
        "#define A (*(int *)0x1010)\n#define B (*(char **)0x2000)\n#define  C (*(int *)0x1010)\n#define D (*(int "
        "*)0x1010) \n"
        "#define E (*(int*)0x1010)\n#define F (*(unsigned int *)0xdeadbeefdeadbeefdead)\n#define G (*( *)0x1010)\n"
        "#define H (*(*)0x1010)\n#define I (*(int *)0x01010)\r\n#define J (*(int *)0x1010)\r\nlast #define K (*(int "
        "*)0x10ff)";
    const Relocated r = relocate_header(in, d);
    const std::string place = "((char *)&codebreaker_data_2 + 0x10)";
    CB_CHECK_EQ(r.text,
                "DECL\n#define A (*(int *)" + place +
                    ")\n#define B (*(char **)0x2000)\n#define  C (*(int *)0x1010)\n"
                    "#define D (*(int *)0x1010) \n#define E (*(int*)" +
                    place +
                    ")\n"
                    "#define F (*(unsigned int *)0xdeadbeefdeadbeefdead)\n#define G (*( *)" +
                    place + ")\n#define H (*(*)" + place +
                    ")\n#define I (*(int *)0x01010)\r\n#define J (*(int *)0x1010)\r\nlast #define K (*(int *)0x10ff)");
    CHECK_SAME(r.lost,
               (std::vector<std::uint64_t>{0x2000})); // (a constant past 64 bits is left alone, not listed: DIVERGES)
    CB_CHECK_EQ(relocate_header("", d).text, "DECL\n");
});

// ---- the layout check --------------------------------------------------------------------------------------------

namespace {

// A tiny OMF object: one segment and the publics `names` (name, offset); `segment` for each (1-based; 0: absolute).
struct Pub {
    std::string name;
    int offset;
    int segment;
};
Bytes omf_object(const std::vector<Pub>& publics) {
    Bytes out;
    auto record = [&](std::uint8_t type, const Bytes& body) {
        Bytes r{type, static_cast<std::uint8_t>((body.size() + 1) & 0xFF),
                static_cast<std::uint8_t>((body.size() + 1) >> 8)};
        r.insert(r.end(), body.begin(), body.end());
        std::uint8_t sum = 0;
        for (const std::uint8_t b : r)
            sum = static_cast<std::uint8_t>(sum + b);
        r.push_back(static_cast<std::uint8_t>(0 - sum));
        out.insert(out.end(), r.begin(), r.end());
    };
    auto name = [](const std::string& s) {
        Bytes b{static_cast<std::uint8_t>(s.size())};
        b.insert(b.end(), s.begin(), s.end());
        return b;
    };
    record(0x80, name("data.c"));
    Bytes lnames = name("");
    for (const std::string s : {"_DATA", "DATA", "_BSS"}) {
        const Bytes n = name(s);
        lnames.insert(lnames.end(), n.begin(), n.end());
    }
    record(0x96, lnames);
    record(0x98, {0x48, 0x00, 0x01, 2, 3, 1}); // SEGDEF: _DATA, class DATA, 0x100 bytes
    record(0x98, {0x48, 0x00, 0x01, 4, 3, 1}); // SEGDEF: _BSS
    for (const auto& [n, offset, segment] : publics) {
        const auto off = static_cast<std::uint16_t>(offset);
        const auto seg = static_cast<std::uint8_t>(segment);
        Bytes body{0, seg};
        if (seg == 0) {
            body.push_back(0);
            body.push_back(0);
        }
        const Bytes nm = name(n);
        body.insert(body.end(), nm.begin(), nm.end());
        body.push_back(static_cast<std::uint8_t>(off & 0xFF));
        body.push_back(static_cast<std::uint8_t>(off >> 8));
        body.push_back(0); // type index
        record(0x90, body);
    }
    record(0x8A, {0x00}); // MODEND
    return out;
}

Data parts_of(std::initializer_list<Part> parts) {
    Data d;
    d.parts = parts;
    return d;
}

} // namespace

CB_TEST("build.data.layout_holds", [] {
    const Data d = parts_of({{"a", 0x1000, 16, 2}, {"b", 0x1010, 48, 2}, {"c", 0x1040, 8, 2}, {"x", 0x9000, 4, 3}});
    // each part where the original has it, relative to the first part of its section
    CB_CHECK(layout_holds(omf_object({{"_a", 0x20, 1}, {"_b", 0x30, 1}, {"_c", 0x60, 1}, {"_x", 0x4, 2}}), d));
    // a part moved (a gap the compiler put in)
    CB_CHECK(!layout_holds(omf_object({{"_a", 0x20, 1}, {"_b", 0x34, 1}, {"_c", 0x64, 1}, {"_x", 0, 2}}), d));
    // in another segment than the first part of its section
    CB_CHECK(!layout_holds(omf_object({{"_a", 0x20, 1}, {"_b", 0x30, 2}, {"_c", 0x60, 1}, {"_x", 0, 2}}), d));
    // a part the object does not define
    CB_CHECK(!layout_holds(omf_object({{"_a", 0x20, 1}, {"_b", 0x30, 1}, {"_x", 0, 2}}), d));
    // the last definition of a name wins
    CB_CHECK(layout_holds(
        omf_object({{"_a", 0x99, 1}, {"_a", 0x20, 1}, {"_b", 0x30, 1}, {"_c", 0x60, 1}, {"_x", 7, 1}}), d));
    // no parts: nothing to disprove
    CB_CHECK(layout_holds(omf_object({}), Data{}));
    // absolute symbols (no segment) compare as a segment of their own
    CB_CHECK(layout_holds(omf_object({{"_a", 0x20, 0}, {"_b", 0x30, 0}, {"_c", 0x60, 0}, {"_x", 1, 1}}), d));
});

CB_TEST("build.data.layout_holds_damaged_object", [] {
    const Data d = parts_of({{"a", 0x1000, 16, 2}});
    const Bytes good = omf_object({{"_a", 0x20, 1}});
    CB_CHECK(layout_holds(good, d));
    CB_CHECK(!layout_holds({}, d));
    CB_CHECK(!layout_holds(Bytes{0x80, 0xFF, 0xFF, 1}, d));
    for (std::size_t cut = 0; cut + 1 < good.size(); cut += 3) { // truncated at many places: never a crash
        (void)layout_holds(ByteView(good).first(cut), d);
    }
});

// ---- 16-bit modules ------------------------------------------------------------------------------------------------

CB_TEST("build.data.data16_lzexpand", [] {
    const ne::Module m = module("lzexpand.dll");
    check_data(data16(m, names_of(m)), "data_lzexpand_dll", false);
});

CB_TEST("build.data.data16_charmap_with_and_without_carried_segments", [] {
    const ne::Module m = module("charmap.exe");
    check_data(data16(m, names_of(m)), "data_charmap_exe", false);
    check_data(data16(m, names_of(m), {1}), "data_charmap_exe_far", false);
});

CB_TEST("build.data.far_segments_import_everything", [] {
    const ne::Module m = module("lzexpand.dll");
    std::map<std::uint16_t, Bytes> segs;
    for (const ne::Segment& s : m.segments()) {
        if (s.index == 2 || s.index == 4)
            segs[s.index] = Bytes(m.data().begin() + s.file_offset, m.data().begin() + s.file_offset + s.length);
    }
    CB_CHECK_EQ(far_segments(segs, &m, names_of(m)), golden("data_lzexpand_dll_segments.golden.c"));
    // without the module there is nothing to fix up: the bytes as given
    CB_CHECK_EQ(far_segments(segs), golden("data_lzexpand_dll_segments_bare.golden.c"));
    CB_CHECK(far_segments({}).empty());
});

CB_TEST("build.data.far_segments_of_code_with_internal_fixups", [] {
    const ne::Module m = module("charmap.exe");
    std::map<std::uint16_t, Bytes> segs;
    for (const ne::Segment& s : m.segments())
        segs[s.index] = Bytes(m.data().begin() + s.file_offset, m.data().begin() + s.file_offset + s.length);
    CB_CHECK_EQ(far_segments(segs, &m, names_of(m)), golden("data_charmap_exe_segments.golden.c"));
});

CB_TEST("build.data.places_follow_chains", [] {
    auto reloc = [](std::uint16_t offset, std::uint8_t source, bool additive) {
        ne::Relocation r;
        r.source = source;
        r.flags = additive ? 4 : 0;
        r.offset = offset;
        r.target = ne::InternalRef{1, 0};
        return r;
    };
    Bytes blob(32, 0);
    blob[4] = 10; // 4 -> 10 -> 20 -> end
    blob[10] = 20;
    blob[20] = 0xFF, blob[21] = 0xFF;
    blob[24] = 0x34, blob[25] = 0x12; // an additive place: its word is the addend, and it ends the chain
    const std::vector<ne::Relocation> relocs{reloc(4, 3, false), reloc(24, 5, true), reloc(0xFFFF, 2, false),
                                             reloc(31, 2, false)};
    const std::vector<RelocationPlace> out = places(relocs, blob);
    CB_CHECK_EQ(out.size(), 4u);
    CB_CHECK(out[0].offset == 4 && out[0].source == 3 && out[0].addend == 0 && out[0].record == 0);
    CB_CHECK(out[1].offset == 10 && out[1].record == 0);
    CB_CHECK(out[2].offset == 20 && out[2].record == 0);
    CB_CHECK(out[3].offset == 24 && out[3].source == 5 && out[3].addend == 0x1234 && out[3].record == 1);
    // (the record at 0xFFFF is empty; the one at 31 has no second byte in the segment)
});

CB_TEST("build.data.places_survive_a_cyclic_chain", [] {
    ne::Relocation r;
    r.offset = 0;
    r.target = ne::InternalRef{1, 0};
    Bytes blob(4, 0); // offset 0 holds 0: a place that points at itself
    const std::vector<ne::Relocation> relocs{r};
    const std::vector<RelocationPlace> out = places(relocs, blob);
    CB_CHECK_EQ(out.size(), 0x10000u); // the chain is cut at 64 Ki hops, as a damaged file must not hang the tool
});

// ---- no data -----------------------------------------------------------------------------------------------------

CB_TEST("build.data.data16_without_an_automatic_data_segment", [] {
    // a PE-like module has none: charmap's second segment is data; here use a module whose autodata is 0 by clearing it
    // in a copy of the file
    Bytes file = fs::read_file(testing::data_path("formats/testdata/lzexpand.dll"));
    const ne::Module original = ne::Module::open(file);
    // the NE header's automatic data segment word is at 0x0E of the header
    file[original.ne_offset() + 0x0E] = 0;
    file[original.ne_offset() + 0x0F] = 0;
    const ne::Module m = ne::Module::open(file);
    CB_CHECK_EQ(m.header().autodata_segment, 0);
    const Data d = data16(m, Names16{});
    CB_CHECK_EQ(d.source,
                std::format("/* {} has no automatic data segment. */\nchar codebreaker_data = 1;\n", m.module_name()));
    CB_CHECK_EQ(d.pointers, 0u);
});

// ---- cut-off modules ---------------------------------------------------------------------------------------------

CB_TEST("build.data.data32_of_a_module_cut_in_its_data", [] {
    // the file ends inside .data
    const Bytes file = fs::read_file(testing::data_path("formats/testdata/lz32.dll"));
    const pe::Image whole = pe::Image::parse(file);
    std::uint32_t raw_ptr = 0;
    for (const pe::Section& s : whole.sections()) {
        if (s.name == ".data")
            raw_ptr = s.raw_ptr;
    }
    const Bytes cut(file.begin(), file.begin() + raw_ptr + 0x60);
    const pe::Image img = pe::Image::parse(cut);
    // the relocation table is gone with the rest of the file: the loader's reader says so (nothing is guessed)
    CB_CHECK_THROWS_AS(FormatError, (void)data32(img, "lz32.dll", functions_in_text(img)));
});

CB_TEST("build.data.data16_of_a_module_cut_in_its_data", [] {
    Bytes file = fs::read_file(testing::data_path("formats/testdata/lzexpand.dll"));
    const ne::Module whole = ne::Module::open(file);
    // cut the file in the middle of the automatic data segment (the last segment)
    const ne::Segment last = whole.segments().back();
    file.resize(last.file_offset + 100);
    const ne::Module m = ne::Module::open(file);
    const Data d = data16(m, names_of(m));
    CB_CHECK_EQ(d.size, 644u); // the segment keeps its size; what the file lacks is zeros
    CB_CHECK(d.source.find("(0x64 of them in the file)") != std::string::npos);
});
