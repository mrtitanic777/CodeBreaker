#include "cb/analyze/strings.hpp"

#include <nlohmann/json.hpp>

#include "cb/analyze/test_image.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"

namespace {

using namespace cb::analyze;
using cb::Json;
using cb::analyze::test::Sample;
using strings::Encoding;

struct Prepared {
    Sample s;
    Image image{make()};
    call_graph::Graph graph;
    strings::Table table;

    ImageSpec make() {
        ImageSpec spec = s.spec();
        spec.function_names = {{s.A, "main"}, {s.P, "format"}, {s.H, "log"}, {s.I, "log_x"}, {s.J, "log_y"}, {s.G, "stack"}};
        return spec;
    }
    Prepared() {
        graph = call_graph::build(image);
        table = strings::build_table(image, graph);
    }
};

// ---- string_at ---------------------------------------------------------------------------------------------------

CB_TEST("analyze.strings.string_at_reads_ascii_and_wide", [] {
    const Prepared p;
    const auto hello = strings::string_at(p.image, p.s.hello);
    CB_CHECK(hello.has_value() && hello->text == "Hello" && hello->encoding == Encoding::Ascii);
    const auto wide = strings::string_at(p.image, p.s.wide_string);
    CB_CHECK(wide.has_value() && wide->text == "Wide!" && wide->encoding == Encoding::Wide);
    CB_CHECK(!strings::string_at(p.image, p.s.format, 6).has_value());      // "%s=%d" is five characters
    CB_CHECK(strings::string_at(p.image, p.s.format, 5).has_value());
    CB_CHECK(!strings::string_at(p.image, p.s.hello + 5).has_value());      // the terminator itself
    CB_CHECK(!strings::string_at(p.image, 0x500000).has_value());           // nowhere
    CB_CHECK(!strings::string_at(p.image, p.s.buffer).has_value());         // memory the file only reserves
    CB_CHECK(!strings::string_at(p.image, p.s.A).has_value());              // code
});

CB_TEST("analyze.strings.the_tail_of_a_longer_string_is_not_a_short_string", [] {
    const Prepared p;
    // "ello" inside "Hello" is four characters: believed. "llo" is three and follows a printable byte: it is the tail
    // of a longer one, not a string of its own.
    CB_CHECK(strings::string_at(p.image, p.s.hello + 1).has_value());
    CB_CHECK(!strings::string_at(p.image, p.s.hello + 2).has_value());
    CB_CHECK(!strings::string_at(p.image, p.s.hello + 3).has_value());
    CB_CHECK(strings::string_at(p.image, p.s.hello + 3, 1).has_value() == false);   // (the rule holds for any `least`)
});

CB_TEST("analyze.strings.tab_newline_and_carriage_return_are_text", [] {
    Sample s;
    ImageSpec spec = s.spec();
    const char text[] = "a\tb\r\nc";
    for (std::size_t i = 0; i < sizeof(text); ++i) spec.spans[1].data[0x50 + i] = static_cast<std::uint8_t>(text[i]);
    Image image(std::move(spec));
    const auto found = strings::string_at(image, s.kData + 0x50);
    CB_CHECK(found.has_value() && found->text == std::string(text));
    spec = s.spec();
    spec.spans[1].data[0x50] = 'a';
    spec.spans[1].data[0x51] = 0x01;   // a control character ends the run without a zero: not a string
    spec.spans[1].data[0x52] = 0;
    Image image2(std::move(spec));
    CB_CHECK(!strings::string_at(image2, s.kData + 0x50).has_value());
});

// ---- the table ---------------------------------------------------------------------------------------------------

CB_TEST("analyze.strings.the_table_holds_referenced_and_unreferenced_strings", [] {
    const Prepared p;
    CB_CHECK_EQ(p.table.size(), std::size_t{6});
    CB_CHECK(p.table.at(p.s.hello).referenced && p.table.at(p.s.hello).text == "Hello");
    CB_CHECK(p.table.at(p.s.format).referenced);
    CB_CHECK(p.table.at(p.s.format_x).referenced && p.table.at(p.s.format_x).text == "x=%d");   // four characters: referenced
    CB_CHECK(p.table.at(p.s.format_y).referenced);
    // nothing refers to these two: listed because they are long enough and outside the code
    CB_CHECK(!p.table.at(p.s.unreferenced).referenced);
    CB_CHECK_EQ(p.table.at(p.s.unreferenced).text, std::string("A string nothing refers to"));
    CB_CHECK(!p.table.at(p.s.wide_string).referenced && p.table.at(p.s.wide_string).encoding == Encoding::Wide);
    CB_CHECK_EQ(p.table.at(p.s.wide_string).text, std::string("Wide!"));
});

CB_TEST("analyze.strings.an_unreferenced_string_must_be_long_enough", [] {
    Sample s;
    ImageSpec spec = s.spec();
    const char text[] = "four";   // nothing refers to it: four characters is too few in 32-bit code
    for (std::size_t i = 0; i < sizeof(text); ++i) spec.spans[1].data[0x90 + i] = static_cast<std::uint8_t>(text[i]);
    Image image(std::move(spec));
    const call_graph::Graph g = call_graph::build(image);
    const strings::Table t = strings::build_table(image, g);
    CB_CHECK(!t.contains(s.kData + 0x90));
    CB_CHECK_EQ(t.size(), std::size_t{6});
});

CB_TEST("analyze.strings.code_and_resource_sections_are_not_searched", [] {
    Sample s;
    ImageSpec spec = s.spec();
    // a string-looking run in the padding of the code span (after A), and a data span called .rsrc with another
    const char text[] = "code text";
    for (std::size_t i = 0; i < sizeof(text); ++i) spec.spans[0].data[0x15 + i] = static_cast<std::uint8_t>(text[i]);
    Span rsrc;
    rsrc.va = 0x403000;
    rsrc.data.assign(64, 0);
    for (std::size_t i = 0; i < sizeof(text); ++i) rsrc.data[i] = static_cast<std::uint8_t>(text[i]);
    rsrc.initialised = 64;   // (".rsrc" holds resources, not strings)
    rsrc.name = ".rsrc";
    spec.spans.push_back(rsrc);
    Image image(std::move(spec));
    const call_graph::Graph g = call_graph::build(image);
    const strings::Table t = strings::build_table(image, g);
    CB_CHECK_EQ(t.size(), std::size_t{6});
    CB_CHECK(!t.contains(0x403000));
});

// ---- uses --------------------------------------------------------------------------------------------------------

CB_TEST("analyze.strings.uses_by_function_in_the_order_they_first_appear", [] {
    Prepared p;
    const auto uses = strings::find_uses(p.image, p.graph, p.table);
    CB_CHECK_EQ(uses.size(), std::size_t{4});
    CB_CHECK_EQ(uses[0].function, p.s.A);
    CB_CHECK_EQ(uses[0].uses.size(), std::size_t{1});
    CB_CHECK_EQ(uses[0].uses[0].site, p.s.A + 5);
    CB_CHECK_EQ(uses[0].uses[0].address, p.s.hello);
    CB_CHECK_EQ(uses[0].uses[0].text, std::string("Hello"));
    CB_CHECK_EQ(uses[1].function, p.s.P);                    // the buffer address is not a string: only the format
    CB_CHECK_EQ(uses[1].uses.size(), std::size_t{1});
    CB_CHECK_EQ(uses[1].uses[0].text, std::string("%s=%d"));
    CB_CHECK_EQ(uses[2].function, p.s.I);
    CB_CHECK_EQ(uses[3].function, p.s.J);
});

// ---- strings built on the stack ----------------------------------------------------------------------------------

CB_TEST("analyze.strings.a_string_stored_by_immediates_is_found", [] {
    Prepared p;
    const auto stack = strings::stack_strings(p.image);
    CB_CHECK_EQ(stack.size(), std::size_t{1});
    CB_CHECK_EQ(stack[0].function, p.s.G);
    CB_CHECK_EQ(stack[0].where, std::string("ebp-0x10"));
    CB_CHECK_EQ(stack[0].text, std::string("Hello World"));
    CB_CHECK(!stack[0].wide);
});

strings::Stores stores_of(std::initializer_list<std::tuple<const char*, std::int64_t, int, int>> rows) {
    strings::Stores s;
    for (const auto& [base, off, byte, number] : rows) s[{base, off}] = strings::Store{static_cast<std::uint8_t>(byte), number};
    return s;
}

CB_TEST("analyze.strings.assemble_needs_letters_length_and_two_stores", [] {
    // "Hello" written by two stores
    auto hello = stores_of({{"ebp", -8, 'H', 1}, {"ebp", -7, 'e', 1}, {"ebp", -6, 'l', 1}, {"ebp", -5, 'l', 2}, {"ebp", -4, 'o', 2}});
    auto pieces = strings::assemble(hello);
    CB_CHECK_EQ(pieces.size(), std::size_t{1});
    CB_CHECK(pieces[0].base == "ebp" && pieces[0].offset == -8 && pieces[0].text == "Hello" && !pieces[0].wide);
    // one store only: not a string, a dword constant that happens to be text
    auto one = stores_of({{"ebp", -8, 'H', 1}, {"ebp", -7, 'e', 1}, {"ebp", -6, 'l', 1}, {"ebp", -5, 'l', 1}});
    CB_CHECK(strings::assemble(one).empty());
    // too short
    auto shorter = stores_of({{"ebp", -8, 'H', 1}, {"ebp", -7, 'e', 2}, {"ebp", -6, 'y', 3}});
    CB_CHECK(strings::assemble(shorter).empty());
    CB_CHECK_EQ(strings::assemble(shorter, 3).size(), std::size_t{1});      // unless `least` says three will do
    // no letters
    auto digits = stores_of({{"esp", 0, '1', 1}, {"esp", 1, '2', 1}, {"esp", 2, '3', 2}, {"esp", 3, '4', 2}});
    CB_CHECK(strings::assemble(digits).empty());
    // a gap in the offsets splits the run; each half is judged alone
    auto gap = stores_of({{"ebp", 0, 'a', 1}, {"ebp", 1, 'b', 2}, {"ebp", 5, 'c', 3}, {"ebp", 6, 'd', 4}});
    CB_CHECK(strings::assemble(gap, 2).size() == 2);
    // a different base is a different run
    auto bases = stores_of({{"ebp", 0, 'H', 1}, {"ebp", 1, 'i', 2}, {"esp", 2, 'x', 3}, {"esp", 3, 'y', 4}});
    CB_CHECK(strings::assemble(bases).empty());
});

CB_TEST("analyze.strings.assemble_reads_sixteen_bit_characters", [] {
    // "Abc" as 16-bit characters: A,0,b,0,c,0
    auto wide = stores_of({{"ebp", 0, 'A', 1}, {"ebp", 1, 0, 1}, {"ebp", 2, 'b', 2}, {"ebp", 3, 0, 2}, {"ebp", 4, 'c', 3}, {"ebp", 5, 0, 3}});
    const auto pieces = strings::assemble(wide);
    CB_CHECK_EQ(pieces.size(), std::size_t{1});
    CB_CHECK(pieces[0].wide && pieces[0].text == "Abc" && pieces[0].offset == 0);
    auto two = stores_of({{"ebp", 0, 'A', 1}, {"ebp", 1, 0, 1}, {"ebp", 2, 'b', 2}, {"ebp", 3, 0, 2}});
    CB_CHECK(strings::assemble(two).empty());                   // two wide characters are too few
});

CB_TEST("analyze.strings.assemble_matches_the_engine_it_replaces", [] {
    const Json cases = cb::json::load(cb::testing::data_path("analyze/testdata/assemble.json"));
    std::size_t with_pieces = 0;
    for (const Json& c : cases) {
        strings::Stores stores;
        for (const Json& s : c.at("stores"))
            stores[{s.at(0).get<std::string>(), s.at(1).get<std::int64_t>()}] =
                strings::Store{static_cast<std::uint8_t>(s.at(2).get<int>()), s.at(3).get<int>()};
        const auto got = strings::assemble(stores, c.at("least").get<std::size_t>());
        const Json& want = c.at("pieces");
        bool same = got.size() == want.size();
        for (std::size_t i = 0; same && i < got.size(); ++i)
            same = got[i].base == want[i].at(0).get<std::string>() && got[i].offset == want[i].at(1).get<std::int64_t>() &&
                   got[i].text == want[i].at(2).get<std::string>() && got[i].wide == want[i].at(3).get<bool>();
        CB_CHECK(same);
        if (!want.empty()) ++with_pieces;
    }
    CB_CHECK(with_pieces > 200);                                // the cases are not vacuous
});

// ---- format strings ----------------------------------------------------------------------------------------------

CB_TEST("analyze.strings.conversions_type_the_arguments", [] {
    CB_CHECK((strings::conversions("%s = %ld") == std::vector<std::string>{"char *", "long"}));
    CB_CHECK((strings::conversions("%*.*f") == std::vector<std::string>{"int", "int", "double"}));
    CB_CHECK((strings::conversions("%I64d %I64u") == std::vector<std::string>{"__int64", "unsigned __int64"}));
    CB_CHECK((strings::conversions("%s", true) == std::vector<std::string>{"wchar_t *"}));      // a wide format: %s is wide
    CB_CHECK((strings::conversions("%S", true) == std::vector<std::string>{"char *"}));
    CB_CHECK((strings::conversions("%hs %ws", true) == std::vector<std::string>{"wchar_t *", "wchar_t *"}));
    CB_CHECK((strings::conversions("100%% done %d") == std::vector<std::string>{"int"}));
    CB_CHECK(strings::conversions("").empty());
    CB_CHECK(strings::conversions("no conversions here").empty());
    CB_CHECK(strings::conversions("%.f").empty());                 // a precision needs digits or '*'
    CB_CHECK(strings::conversions("trailing %").empty());
});

CB_TEST("analyze.strings.conversions_match_the_engine_it_replaces", [] {
    const Json cases = cb::json::load(cb::testing::data_path("analyze/testdata/conversions.json"));
    CB_CHECK(cases.size() >= 90);
    for (const Json& c : cases) {
        const auto got = strings::conversions(c.at("format").get<std::string>(), c.at("wide").get<bool>());
        const auto want = c.at("types").get<std::vector<std::string>>();
        if (got != want) cb::testing::record_failure(__FILE__, __LINE__, "conversions differ for \"" + c.at("format").get<std::string>() + "\"");
        else cb::testing::count_check();
    }
});

CB_TEST("analyze.strings.a_printf_import_is_read_for_its_format", [] {
    Prepared p;
    const strings::Formats f = strings::find_formats(p.image, p.graph, p.table);
    CB_CHECK_EQ(f.sites, 1);
    CB_CHECK_EQ(f.calls.size(), std::size_t{3});
    const strings::FormatCall& w = f.calls[0];
    CB_CHECK_EQ(w.function, p.s.P);
    CB_CHECK_EQ(w.api, std::string("USER32!wsprintfA"));
    CB_CHECK(w.imported && w.lists_arguments && !w.wide);
    CB_CHECK_EQ(w.format, std::string("%s=%d"));
    CB_CHECK_EQ(w.site, p.s.P + 0x0C);                                  // the call through the import's slot
});

CB_TEST("analyze.strings.a_function_handed_formats_from_several_places_is_printf_like", [] {
    Prepared p;
    const strings::Formats f = strings::find_formats(p.image, p.graph, p.table);
    CB_CHECK_EQ(f.own.size(), std::size_t{1});
    CB_CHECK_EQ(f.own.at(p.s.H), 0);                                    // its format is its first argument
    CB_CHECK(!f.calls[1].imported && !f.calls[2].imported);
    CB_CHECK_EQ(f.calls[1].function, p.s.I);
    CB_CHECK_EQ(f.calls[1].api, std::string("log"));                    // the label of the printf-like function
    CB_CHECK_EQ(f.calls[1].format, std::string("x=%d"));
    CB_CHECK_EQ(f.calls[2].function, p.s.J);
    CB_CHECK_EQ(f.calls[2].format, std::string("y=%s"));
});

CB_TEST("analyze.strings.a_function_given_one_format_is_not_printf_like", [] {
    // J's call is removed: H is handed a format from one place only
    Sample s;
    ImageSpec spec = s.spec();
    for (std::size_t i = 0; i < 5; ++i) spec.spans[0].data[s.J - s.kCode + i] = 0x90;   // nop the push
    Image image(std::move(spec));
    const call_graph::Graph g = call_graph::build(image);
    const strings::Table t = strings::build_table(image, g);
    const strings::Formats f = strings::find_formats(image, g, t);
    CB_CHECK(f.own.empty());
    CB_CHECK_EQ(f.calls.size(), std::size_t{1});
});

// ---- the report --------------------------------------------------------------------------------------------------

CB_TEST("analyze.strings.the_report_counts_and_summarises", [] {
    Prepared p;
    const strings::Report r = strings::analyse(p.image, p.graph);
    const strings::Counts& c = r.counts;
    CB_CHECK_EQ(c.strings, 6);
    CB_CHECK_EQ(c.referenced, 4);
    CB_CHECK_EQ(c.wide, 1);
    CB_CHECK_EQ(c.functions_using_strings, 4);
    CB_CHECK_EQ(c.string_uses, 4);
    CB_CHECK_EQ(c.stack_strings, 1);
    CB_CHECK_EQ(c.formatter_call_sites, 1);
    CB_CHECK_EQ(c.format_strings_read, 1);
    CB_CHECK_EQ(c.own_printf_like_functions, 1);
    CB_CHECK_EQ(c.format_strings_at_own_functions, 2);
    CB_CHECK_EQ(c.arguments_typed_by_formats, 4);                        // (char *, int) (int) (char *)
    CB_CHECK_EQ(r.strings.size(), std::size_t{6});
    CB_CHECK(r.strings.front().at < r.strings.back().at);                // by address
    CB_CHECK_EQ(r.string_uses.size(), std::size_t{4});
    CB_CHECK((r.string_uses[0].strings == std::vector<std::string>{"Hello"}));
    CB_CHECK_EQ(r.formats.size(), std::size_t{3});
    CB_CHECK((*r.formats[0].arguments == std::vector<std::string>{"char *", "int"}));
    CB_CHECK_EQ(r.printf_like.size(), std::size_t{1});
    CB_CHECK_EQ(r.printf_like[0].format_argument, 0);
    // the summary: the stack string, the formats and the printf-like function
    CB_CHECK_EQ(r.text[0], std::string("stack string in stack at ebp-0x10: \"Hello World\""));
    CB_CHECK_EQ(r.text[1], std::string("wsprintfA(\"%s=%d\") in format -> char *, int"));
    CB_CHECK_EQ(r.text[4], std::string("printf-like: log, format is argument 0"));
});

CB_TEST("analyze.strings.a_va_list_formatter_leaves_the_arguments_elsewhere", [] {
    // vsprintf-like imports take a va_list: the format is read, the argument types are not
    Sample s;
    ImageSpec spec = s.spec();
    spec.imports[s.wsprintf_slot] = "USER32!wvsprintfA";
    Image image(std::move(spec));
    const call_graph::Graph g = call_graph::build(image);
    const strings::Table t = strings::build_table(image, g);
    const strings::Formats f = strings::find_formats(image, g, t);
    CB_CHECK(!f.calls.empty() && !f.calls[0].lists_arguments);
    CB_CHECK_EQ(f.calls[0].format, std::string("%s=%d"));
    const strings::Report r = strings::analyse(image, g);
    CB_CHECK(!r.formats[0].arguments.has_value());
});

}  // namespace
