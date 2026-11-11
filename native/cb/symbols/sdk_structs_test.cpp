// Tests of cb/symbols/sdk_structs on testdata/sdk_structs/layouts.h: structures written in the shapes the SDK headers
// use (nested structures, packing, arrays sized by constants, pointers, a declaration with a macro in it, nested braces,
// a preprocessor line in a body). Every expected layout was also what the Python module read from the same file. The
// reader was run over the real headers too (OpenWatcom's h/nt, the Win32 SDK's and the DDK's INC32: 8,514 structures and
// 6,526 pointer types) and gave the Python module's table for every one but eight, listed below.
//
// DIVERGES from the Python module, by design (and listed as a bug fixed in state/work/typed-port/_bugs/LIST.md): a
// declaration of one word in a structure's body -- a macro the header expands to fields, as PROPSHEETPAGEA_V1_FIELDS --
// is not a field, and the structure it is in cannot be laid out. The Python read the macro's name as a type and a
// field name cut from it (`PROPSHEETPAGEA_V1_FIELD` `S`) and gave the eight PROPSHEET structures of commctrl.h an
// invented 4-byte layout with a field called S.
#include "cb/symbols/sdk_structs.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/knowledge/reads.hpp"

namespace ss = cb::symbols::sdk_structs;

namespace {

ss::StructTable table() {
    static const ss::HeaderStructs read = [] {
        const std::vector<std::filesystem::path> folders = {cb::testing::data_path("symbols/testdata/sdk_structs")};
        return ss::read_headers(folders, {{"MAX_NAME", 32}});
    }();
    return ss::StructTable(read);
}

using Fields = std::vector<ss::Field>;

ss::Field F(std::uint32_t offset, std::uint32_t size, const char* name, const char* ctype, bool pointer = false) {
    return {offset, size, name, ctype, pointer};
}

}  // namespace

CB_TEST("symbols.sdk_structs.plain_and_nested_layouts", [] {
    const ss::StructTable t = table();
    const ss::Layout* point = t.layout("POINT");
    CB_CHECK(point != nullptr);
    CB_CHECK_EQ(point->size, 8u);
    CB_CHECK_EQ(point->alignment, 4u);
    CB_CHECK(point->fields == (Fields{F(0, 4, "x", "int4"), F(4, 4, "y", "int4")}));   // LONG is signed
    CB_CHECK(t.layout("tagPOINT") != nullptr);                                         // by its tag as well
    CB_CHECK(*t.layout("tagPOINT") == *point);

    const ss::Layout* rect = t.layout("RECT");
    CB_CHECK(rect != nullptr);
    CB_CHECK_EQ(rect->size, 16u);

    const ss::Layout* pair = t.layout("SIZEPAIR");
    CB_CHECK(pair != nullptr);
    CB_CHECK_EQ(pair->size, 28u);   // 25 bytes of fields, rounded to the biggest alignment
    CB_CHECK(pair->fields == (Fields{F(0, 8, "a", "struct POINT"), F(8, 16, "r", "struct RECT"), F(24, 1, "flag", "uint1")}));

    const ss::Layout* nested = t.layout("NESTED2");   // a structure inside a structure inside a structure
    CB_CHECK(nested != nullptr);
    CB_CHECK_EQ(nested->size, 32u);
    CB_CHECK(nested->fields == (Fields{F(0, 28, "s", "struct SIZEPAIR"), F(28, 2, "tail", "uint2")}));
});

CB_TEST("symbols.sdk_structs.packing_and_alignment", [] {
    const ss::StructTable t = table();
    // #include <pshpack1.h> ... <poppack.h>: no padding at all
    CB_CHECK(t.layout("PACKED")->fields == (Fields{F(0, 1, "b", "uint1"), F(1, 4, "d", "uint4"), F(5, 2, "w", "uint2")}));
    CB_CHECK_EQ(t.layout("PACKED")->size, 7u);
    // after the pop, natural alignment again
    CB_CHECK(t.layout("NATURAL")->fields == (Fields{F(0, 1, "b", "uint1"), F(4, 4, "d", "uint4"), F(8, 2, "w", "uint2")}));
    CB_CHECK_EQ(t.layout("NATURAL")->size, 12u);
    // an eight-byte field is aligned to eight
    CB_CHECK(t.layout("WITHDOUBLE")->fields == (Fields{F(0, 1, "b", "uint1"), F(8, 8, "d", "uint8")}));
    CB_CHECK_EQ(t.layout("WITHDOUBLE")->size, 16u);
    CB_CHECK_EQ(t.layout("WITHDOUBLE")->alignment, 8u);
});

CB_TEST("symbols.sdk_structs.arrays_and_pointers", [] {
    const ss::StructTable t = table();
    // name[MAX_NAME] with MAX_NAME from the constants; a handle or a pointer typedef is four bytes
    CB_CHECK(t.layout("WITHARRAY")->fields ==
             (Fields{F(0, 32, "name", "int1[32]"), F(32, 8, "w", "uint2[4]"), F(40, 4, "count", "int4"), F(44, 4, "p", "uint4")}));
    CB_CHECK_EQ(t.layout("WITHARRAY")->size, 48u);
    // stars: a pointer, whatever it points at, and `h, h2` share the type
    CB_CHECK(t.layout("TWOPTRS")->fields == (Fields{F(0, 4, "pr", "uint4", true), F(4, 4, "pp", "uint4", true), F(8, 4, "ppp", "uint4", true),
                                                    F(12, 4, "h", "uint4"), F(16, 4, "h2", "uint4")}));
});

CB_TEST("symbols.sdk_structs.what_cannot_be_laid_out", [] {
    const ss::StructTable t = table();
    CB_CHECK(t.layout("UNKNOWNFIELD") == nullptr);   // a field of a type of unknown size
    CB_CHECK(t.layout("BADARRAY") == nullptr);       // a length no constant gives
    CB_CHECK(t.layout("MACRO") == nullptr);          // a macro where a field should be (DIVERGES, see above)
    CB_CHECK(t.layout("WITHUNION") == nullptr);      // braces inside: the declaration is not read
    CB_CHECK(t.layout("DIRECTIVE") == nullptr);      // a preprocessor line inside
    CB_CHECK(t.layout("NoSuchStructure") == nullptr);
    CB_CHECK(t.data().structs.contains("UNKNOWNFIELD") && !t.data().structs.at("UNKNOWNFIELD").has_value());   // met, unknown
    CB_CHECK(!t.data().structs.contains("WITHUNION"));                                                         // never met
    CB_CHECK(t.layout("ANONYMOUS") != nullptr);   // `typedef struct { ... } NAME;`
});

CB_TEST("symbols.sdk_structs.pointed_to", [] {
    const ss::StructTable t = table();
    CB_CHECK(t.pointed_to("LPPOINT") == std::optional<std::string>("POINT"));     // `FAR *LPPOINT` in the declaration
    CB_CHECK(t.pointed_to("PPOINT") == std::optional<std::string>("POINT"));
    CB_CHECK(t.pointed_to("LPCRECT") == std::optional<std::string>("RECT"));      // `typedef const RECT FAR *LPCRECT;`
    CB_CHECK(t.pointed_to("CONST RECT *") == std::optional<std::string>("RECT"));
    CB_CHECK(t.pointed_to("const POINT FAR *") == std::optional<std::string>("POINT"));
    CB_CHECK(t.pointed_to("  RECT  * ") == std::optional<std::string>("RECT"));
    CB_CHECK(!t.pointed_to("RECT").has_value());              // not a pointer
    CB_CHECK(!t.pointed_to("UNKNOWNFIELD *").has_value());    // a structure that is not laid out
    CB_CHECK(!t.pointed_to("LPUNKNOWNTHING").has_value());
    CB_CHECK(!t.pointed_to("").has_value());
});

CB_TEST("symbols.sdk_structs.fields_by_offset", [] {
    const ss::StructTable t = table();
    const ss::Layout& pair = *t.layout("SIZEPAIR");
    CB_CHECK(t.field_at(pair, 24, 1) == std::optional<std::string>("flag"));
    CB_CHECK(t.field_at(pair, 0, 8) == std::optional<std::string>("a"));        // the whole nested field
    CB_CHECK(t.field_at(pair, 0, 4) == std::optional<std::string>("a.x"));      // into it
    CB_CHECK(t.field_at(pair, 16, 4) == std::optional<std::string>("r.right"));
    CB_CHECK(!t.field_at(pair, 1, 4).has_value());                               // nothing starts there
    CB_CHECK(!t.field_at(pair, 24, 4).has_value());                              // wrong size
    const ss::Layout& deep = *t.layout("NESTED2");
    CB_CHECK(t.field_at(deep, 4, 4) == std::optional<std::string>("s.a.y"));   // a structure in a structure in a structure
    CB_CHECK(t.field_at(deep, 28, 2) == std::optional<std::string>("tail"));

    CB_CHECK(t.typed_field(pair, 24, 1)->path == "flag");
    CB_CHECK(t.typed_field(pair, 24, 1)->ctype == "uint1");
    CB_CHECK(t.typed_field(pair, 16, 4)->path == "r.right");
    CB_CHECK(t.typed_field(pair, 16, 4)->ctype == "int4");
    CB_CHECK(!t.typed_field(pair, 0, 8).has_value());   // a structure, not an integer
    const ss::Layout& arrays = *t.layout("WITHARRAY");
    CB_CHECK(!t.typed_field(arrays, 0, 32).has_value());   // an array is not one integer
    CB_CHECK(t.field_at(arrays, 0, 32) == std::optional<std::string>("name"));
});

CB_TEST("symbols.sdk_structs.definitions", [] {
    const ss::StructTable t = table();
    CB_CHECK_EQ(t.definition("POINT"),
                "#pragma pack(push, 1)\ntypedef struct POINT {\n    int4 x;\n    int4 y;\n} POINT;\n#pragma pack(pop)\n");
    // gaps are padding, written at the offsets they are at
    CB_CHECK_EQ(t.definition("NATURAL"),
                "#pragma pack(push, 1)\ntypedef struct NATURAL {\n    uint1 b;\n    uint1 codebreaker_pad_1[3];\n    uint4 d;\n    uint2 w;\n"
                "    uint1 codebreaker_pad_a[2];\n} NATURAL;\n#pragma pack(pop)\n");
    // a structure in a structure is defined first, once
    const std::string pair = t.definition("SIZEPAIR");
    CB_CHECK(pair.find("typedef struct POINT {") < pair.find("typedef struct RECT {"));
    CB_CHECK(pair.find("typedef struct RECT {") < pair.find("typedef struct SIZEPAIR {"));
    CB_CHECK(pair.find("    POINT a;\n    RECT r;\n    uint1 flag;\n    uint1 codebreaker_pad_19[3];\n} SIZEPAIR;") != std::string::npos);
    // what is already defined is left out
    const std::string rest = t.definition("SIZEPAIR", {"POINT", "RECT"});
    CB_CHECK(rest.find("typedef struct POINT") == std::string::npos);
    CB_CHECK(rest.find("typedef struct SIZEPAIR {") != std::string::npos);
    // arrays keep their length
    CB_CHECK(t.definition("WITHARRAY").find("    int1 name[32];\n    uint2 w[4];\n") != std::string::npos);
    // a structure with no layout is a bug of the caller
    CB_CHECK_THROWS_AS(cb::InternalError, (void)t.definition("UNKNOWNFIELD"));
});

CB_TEST("symbols.sdk_structs.lookups_are_recorded_as_knowledge_reads", [] {
    const ss::StructTable t = table();
    cb::knowledge::Recording recording;
    (void)t.layout("RECT");
    (void)t.pointed_to("LPPOINT");
    (void)t.pointed_to("CONST RECT *");
    std::vector<std::string> keys;
    for (const auto& r : recording.reads()->sorted()) keys.push_back(r.text());
    CB_CHECK(keys == (std::vector<std::string>{"win32:struct:POINT", "win32:struct:RECT", "win32:struct.pointer:LPPOINT"}));
});

CB_TEST("symbols.sdk_structs.reading_folders", [] {
    cb::fs::ScopedTempDir tmp("cb-sdk");
    // the first declaration of a name wins, across headers and folders; only .h files are read; line ends of every kind
    cb::fs::write_text(tmp.path() / "a" / "one.h", "typedef struct tagTHING { BYTE x; } THING;\r\ntypedef struct tagA { WORD w; } A;\r");
    cb::fs::write_text(tmp.path() / "a" / "two.h", "typedef struct tagTHING { DWORD y; DWORD z; } THING;\n");
    cb::fs::write_text(tmp.path() / "a" / "notes.txt", "typedef struct tagTEXT { BYTE x; } TEXT;\n");
    cb::fs::write_text(tmp.path() / "b" / "three.h", "typedef struct tagTHING { WORD q; } THING;\ntypedef struct tagB { THING t; } B;\n");
    const std::vector<std::filesystem::path> folders = {tmp.path() / "a", tmp.path() / "missing", tmp.path() / "b"};
    const ss::StructTable t(ss::read_headers(folders, {}));
    CB_CHECK(t.layout("THING")->fields == (Fields{F(0, 1, "x", "uint1")}));
    CB_CHECK(t.layout("A") != nullptr);
    CB_CHECK(t.layout("TEXT") == nullptr);
    CB_CHECK(t.layout("B")->fields == (Fields{F(0, 1, "t", "struct THING")}));   // B is laid out with the first THING
    // a structure that contains itself is unknown, not an infinite loop
    cb::fs::write_text(tmp.path() / "c" / "loop.h", "typedef struct tagLOOP { struct tagLOOP inner; } LOOP;\n");
    const std::vector<std::filesystem::path> loop = {tmp.path() / "c"};
    CB_CHECK(ss::StructTable(ss::read_headers(loop, {})).layout("LOOP") == nullptr);
    // comments are not code
    cb::fs::write_text(tmp.path() / "d" / "comments.h",
                       "/* typedef struct tagHIDDEN { BYTE x; } HIDDEN; */\n// typedef struct tagTOO { BYTE x; } TOO;\n"
                       "typedef struct tagSHOWN { BYTE /* c */ x; // trailing\n WORD y; } SHOWN;\n");
    const std::vector<std::filesystem::path> comments = {tmp.path() / "d"};
    const ss::StructTable ct(ss::read_headers(comments, {}));
    CB_CHECK(ct.layout("HIDDEN") == nullptr && ct.layout("TOO") == nullptr);
    CB_CHECK(ct.layout("SHOWN")->fields == (Fields{F(0, 1, "x", "uint1"), F(2, 2, "y", "uint2")}));
});
