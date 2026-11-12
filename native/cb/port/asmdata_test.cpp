// Tests of the data of an assembler module as C (asmdata). The expected values are what the Python original (asmdata.py)
// made of the same sources, read through by hand: the bytes of a small data module, its relocations and labels, the typed
// C of its tables, and the numbered messages of the C library's tables.
#include <format>

#include "cb/core/testing.hpp"
#include "cb/port/asmdata.hpp"

using namespace cb;
using namespace cb::port;
using namespace cb::port::asmdata;

namespace {

const char* const kData =
    "        PUBLIC  _names, _tab, _msgtext\n"
    "DGROUP  GROUP   _DATA\n"
    "_DATA   SEGMENT WORD PUBLIC 'DATA'\n"
    "s1      DB      'one', 0\n"
    "s2      DB      'two', 0\n"
    "_names  DW      offset s1\n"
    "        DW      offset s2\n"
    "        DW      0\n"
    "_tab    DW      1, offset _names\n"
    "        DW      2, offset _names+2\n"
    "_msgtext DB     'it''s ', \"ok\", 0\n"
    "pad     DB      3 DUP (7)\n"
    "wide    DD      0ffffh, 0fffffffeh\n"
    "fill    DW      2 DUP (?)\n"
    "_DATA   ENDS\n"
    "        END\n";

const char* const kProgramC = "struct ent { int n; char **p; };\nstruct ent tab[2];\nchar *names[3];\nchar msgtext[10];\n";

std::string hex(const Bytes& b) {
    std::string out;
    for (const std::uint8_t c : b) out += std::format("{:02x}", c);
    return out;
}

}  // namespace

CB_TEST("port.asmdata.the_image_of_a_data_module_has_its_bytes_relocations_labels_and_items", [] {
    const Image img = image(kData);
    CB_CHECK_EQ(img.size(), 1u);
    const Segment& s = img.at("_DATA");
    CB_CHECK_EQ(s.cls.value_or(""), "DATA");
    CB_CHECK_EQ(hex(s.data), "6f6e650074776f00000000000000010000000200000069742773206f6b00070707ffff0000feffffff00000000");
    CB_CHECK_EQ(s.reloc.size(), 4u);
    CB_CHECK_EQ(s.reloc.at(8).symbol, "s1");
    CB_CHECK_EQ(s.reloc.at(10).symbol, "s2");
    CB_CHECK_EQ(s.reloc.at(16).symbol, "_names");
    CB_CHECK_EQ(s.reloc.at(16).displacement, 0);
    CB_CHECK_EQ(s.reloc.at(20).displacement, 2);   // (_names+2)
    std::vector<std::pair<std::string, std::size_t>> labels(s.labels.begin(), s.labels.end());
    const std::vector<std::pair<std::string, std::size_t>> want_labels = {{"s1", 0},  {"s2", 4},  {"_names", 8}, {"_tab", 14},
                                                                         {"_msgtext", 22}, {"pad", 30}, {"wide", 33}, {"fill", 41}};
    CB_CHECK(labels == want_labels);
    CB_CHECK_EQ(s.items.size(), 18u);
    CB_CHECK_EQ(s.items[11].offset, 22u);   // ('it''s ' : the doubled quote is one)
    CB_CHECK_EQ(s.items[11].size, 5u);
    CB_CHECK_EQ(s.items[14].size, 3u);      // (3 DUP (7): three bytes)
    CB_CHECK_EQ(s.items.back().offset, 41u);
    CB_CHECK_EQ(s.items.back().size, 4u);   // (2 DUP (?): two words)
});

CB_TEST("port.asmdata.a_data_item_that_is_not_understood_is_an_error", [] {
    CB_CHECK_THROWS_AS(asm16::Asm16Error, image("_D SEGMENT\nx DW foo+bar\n_D ENDS\n"));
    CB_CHECK_THROWS_AS(asm16::Asm16Error, image("_D SEGMENT\nx DB 1+\n_D ENDS\n"));
});

CB_TEST("port.asmdata.the_publics_are_the_names_a_module_declares_public", [] {
    const std::set<std::string> p = publics(kData);
    CB_CHECK(p == (std::set<std::string>{"_msgtext", "_names", "_tab"}));
    CB_CHECK(publics("x DW 1\n").empty());
});

CB_TEST("port.asmdata.a_c_string_closes_an_octal_escape_a_digit_follows", [] {
    auto bytes = [](std::initializer_list<int> v) {
        Bytes b;
        for (const int x : v) b.push_back(static_cast<std::uint8_t>(x));
        return b;
    };
    CB_CHECK_EQ(c_string(bytes({'h', 'e', 'l', 'l', 'o'})), "\"hello\"");
    CB_CHECK_EQ(c_string(bytes({'a', '"', 'b', '\\', 'c', '\n', '\t', '\r'})), "\"a\\\"b\\\\c\\n\\t\\r\"");
    CB_CHECK_EQ(c_string(bytes({1, 0x1b, 200, '1'})), "\"\\001\\033\\310\"\"1\"");   // (\310 then 1: not \3101)
    CB_CHECK_EQ(c_string(bytes({7, '1', '2'})), "\"\\007\"\"12\"");
    CB_CHECK_EQ(c_string(Bytes{}), "\"\"");
});

CB_TEST("port.asmdata.typed_tables_take_the_c_programs_types_and_what_points_to_them", [] {
    const std::vector<std::string> c{kProgramC};
    const asm16::CTypes types(c, c);
    const std::optional<TypedData> t = typed(kData, types);
    CB_CHECK(t.has_value());
    CB_CHECK_EQ(t->text,
                "/* the tables of an assembler module, with the addresses in them (made by CodeBreaker's port: typed by the C "
                "program's declarations and what points where) */\n"
                "static char s1[4];\nstatic char s2[4];\nchar *names[3];\nstruct ent tab[2];\nchar msgtext[8];\n\n"
                "static char s1[4] = \"one\";\nstatic char s2[4] = \"two\";\nchar *names[3] = {\n\t&s1[0],\n\t&s2[0],\n\t0\n};\n"
                "struct ent tab[2] = {\n\t{ 1, &names[0] },\n\t{ 2, &names[1] }\n};\nchar msgtext[8] = \"it's ok\";");
    CB_CHECK(t->notes.empty());
    // a module with no address in it has nothing to type
    CB_CHECK(!typed("_DATA SEGMENT\nx DW 1\n_DATA ENDS\n END\n", types));
});

CB_TEST("port.asmdata.a_table_whose_type_has_no_size_is_noted_and_left_out", [] {
    // DIVERGES: the Python divided by the missing size and raised TypeError for the whole module; the table is noted
    // ("no size for ...") and the others are made.
    const std::string c = "struct unknown_here;\nstruct unknown_here hook;\nchar *names[3];\n";
    const std::vector<std::string> program{c};
    const asm16::CTypes types(program, program);
    const std::optional<TypedData> t = typed(
        "        PUBLIC  _names, _hook\n_DATA SEGMENT WORD PUBLIC 'DATA'\ns1 DB 'a', 0\n_names DW offset s1\n DW offset s1\n DW 0\n"
        "_hook DW offset s1\n_DATA ENDS\n END\n",
        types);
    CB_CHECK(t.has_value());
    CB_CHECK_EQ(t->text,
                "/* the tables of an assembler module, with the addresses in them (made by CodeBreaker's port: typed by the C "
                "program's declarations and what points where) */\n/* (not typed: _hook: no size for struct unknown_here) */\n"
                "static char s1[2];\nchar *names[3];\n\nstatic char s1[2] = \"a\";\nchar *names[3] = {\n\t&s1[0],\n\t&s1[0],\n\t0\n};");
    CB_CHECK(t->notes == (std::vector<std::string>{"_hook: no size for struct unknown_here"}));
});

CB_TEST("port.asmdata.the_numbered_messages_of_the_library_become_two_tables_and_two_lookups", [] {
    const char* const msgs =
        "_NMSG   SEGMENT BYTE PUBLIC 'MSG'\n        DW      1\n        DB      'Null pointer', 0\n        DW      2\n"
        "        DB      'Divide', 0\n        DW      0ffffh\n_NMSG   ENDS\n_FMSG   SEGMENT BYTE PUBLIC 'FAR_MSG'\n        DW      9\n"
        "        DB      'far one', 0\n        DW      0ffffh\n_FMSG   ENDS\n        END\n";
    const std::optional<std::string> m = messages(msgs);
    CB_CHECK(m.has_value());
    CB_CHECK_EQ(*m,
                "/* the numbered messages of the C library's __NMSG_TEXT (near) and __FMSG_TEXT (far) */\n"
                "static const struct { int16_t num; const char *text; } dos_nmsgs[] = {\n\t{ 1, \"Null pointer\" },\n\t{ 2, \"Divide\" },\n"
                "\t{ -1, 0 }\n};\n\n"
                "static const struct { int16_t num; const char *text; } dos_fmsgs[] = {\n\t{ 9, \"far one\" },\n\t{ -1, 0 }\n};\n\n"
                "char *__NMSG_TEXT(int32_t n)\t\t/* (an int: as the callers give it) */\n{\n\tint32_t i;\n"
                "\tfor (i = 0; dos_nmsgs[i].text; i++)\n\t\tif (dos_nmsgs[i].num == n) return (char *)dos_nmsgs[i].text;\n"
                "\treturn (char *)\"\";\n}\n\n"
                "char *__FMSG_TEXT(int32_t n)\t\t/* (an int: as the callers give it) */\n{\n\tint32_t i;\n"
                "\tfor (i = 0; dos_fmsgs[i].text; i++)\n\t\tif (dos_fmsgs[i].num == n) return (char *)dos_fmsgs[i].text;\n"
                "\treturn (char *)\"\";\n}\n");
    CB_CHECK(!messages(kData));   // (no MSG segment)
});
