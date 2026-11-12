#include "cb/exact/draft/forms.hpp"

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
using namespace cb::exact::draft;
namespace dt = cb::exact::draft::text;
namespace ct = cb::text;
using Json = nlohmann::json;

// DIVERGES from the Python module (draft/forms.py), on purpose:
//   * the module's tables (ARRAYS, STRUCTARR, BYTEPTR, PTRSIZE, FILEPTR, REGSLOTS, _LIBDATA, _HEADERS, _TABLES) are a
//     FunctionFacts, a ProjectCaches and FrameOptions held by the caller; FILEPTR's "#include"/"#include2" are
//     facts.include/include2 (absent, not None)
//   * a library macro without a header no longer erases the header an earlier macro (or data macro) gave: Python
//     stored None over it (fixed in the Python too); see exact.draft.forms.keeps_the_include_a_macro_does_not_name
//   * register_temps asks for the declaration of the register variable by its whole name: Python's pattern also matched
//     a declaration whose name merely ENDED with the name (`register int r_si;` for `si`) (fixed in the Python too)
//   * a macro spec whose `as` takes a different number of arguments than the call has, or has a conversion other than
//     %s, and a data macro of element size 0, throw UsageError (Python: TypeError, ZeroDivisionError - on the first
//     match, as here); the library data of a project that has none is an empty map
//   * a `#define` mask written with a leading 0 (`010`) is read as decimal (Python's int(v, 0) raised)
//   * default_after declines when statements follow the default's return after a blank line (the original rebuilt the
//     switch without them)
//   * case_falls: a case label on the very first line of the text has no line before it (Python indexed line -1)
//   * bool_trees: labels that go round are no test (Python recursed until the interpreter gave up)
//   * header files are found by a name-ordered walk of the kit (os.walk's order was the file system's)
//   * \w \d \s are ASCII (a draft is ASCII)

namespace {

using Strings = std::vector<std::string>;

const std::map<std::string, std::uint32_t> kLibraryData = {
    {"__iob", 0x1000}, {"_ctype", 0x2000}, {"_ctype_", 0x2000}, {"_zz", 0x3000}, {"buf", 0x20}};

std::string unescape(std::string_view s) {
    std::string out;
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '\\' && i + 1 < s.size()) {
            const char c = s[++i];
            out += c == 't' ? '\t' : c == 'n' ? '\n' : c == 'r' ? '\r' : c;
        } else {
            out += s[i];
        }
    }
    return out;
}

Json json_of(const Strings& v) {
    Json a = Json::array();
    for (const std::string& s : v) a.push_back(s);
    return a;
}
Json json_of(const std::optional<std::string>& v) { return v ? Json(*v) : Json(nullptr); }

// What a case runs against: the facts the passes fill, and the project the library passes ask about.
struct World {
    dt::FunctionFacts facts;
    dt::ProjectCaches caches;
    forms::KitContext prj1{"PRJ1", "msc300", testing::data_path("exact/testdata/draft_forms_kit"), [] { return kLibraryData; }};
    forms::KitContext prj2{"PRJ2", "nokit", std::nullopt, [] { return std::map<std::string, std::uint32_t>{}; }};
    World() { caches.library_data["PRJ1"] = kLibraryData; }
};

const std::set<dt::Site> kByteSitesA = {{"data", 4660}, {"*", 4660}, {"*", 200}, {"bxw", 0}, {"siw", 0}, {"di*", 0}, {"g", 0}};

const std::vector<forms::TableMacroSpec> kTableSpecs = {{"ctype.h"}, {"missing.h"}};
const std::vector<forms::DataMacroSpec> kDataSpecs = {
    {"__iob", 8, {"stdin", "stdout", "stderr"}, {{7, "fileno(%s)"}, {6, "feof(%s)"}}, "stdio.h"},
    {"nothere", 4, {"a"}, {}, std::nullopt},
    {"buf", 4, {"q", "r"}, {}, std::nullopt}};
const std::vector<forms::LibraryMacroSpec> kMacros = {{"_filbuf", "getc(%s)", "FILE", "stdio.h", {}},
                                                      {"_flsbuf", "putc(%s, %s)", "FILE", "stdio.h", {0}}};

// The tables a pass filled, as the golden file records them (each sorted).
Json tables_of(const dt::FunctionFacts& f) {
    Json t = Json::object();
    if (!f.arrays.empty()) {
        Json a = Json::array();
        for (const auto& [k, v] : f.arrays) a.push_back(Json::array({k, v}));
        t["a"] = a;
    }
    if (!f.struct_arrays.empty()) {
        Json a = Json::array();
        for (const auto& [k, v] : f.struct_arrays) a.push_back(Json::array({k, Json::array({v.size, v.first_type})}));
        t["s"] = a;
    }
    if (!f.word_pointers.empty()) {
        Json a = Json::array();
        for (const std::string& k : f.word_pointers) a.push_back(Json::array({k, 2}));
        t["p"] = a;
    }
    std::vector<std::pair<std::string, std::string>> streams;
    for (const auto& [k, v] : f.streams) streams.emplace_back(k, v);
    if (f.include) streams.emplace_back("#include", *f.include);
    if (f.include2) streams.emplace_back("#include2", *f.include2);
    std::ranges::sort(streams);
    if (!streams.empty()) {
        Json a = Json::array();
        for (const auto& [k, v] : streams) a.push_back(Json::array({k, v}));
        t["f"] = a;
    }
    return t;
}

Json run_case(const std::string& op, const std::optional<std::string>& text_arg, const Strings& extra) {
    World w;
    const std::string text = text_arg.value_or("");
    const std::size_t colon = op.find(':');
    const std::string name = op.substr(0, colon);
    if (colon != std::string::npos && op.substr(colon + 1) == "b") w.facts.byte_sites = kByteSitesA;
    const forms::FlatReturns no_flat = [](std::string_view) -> std::optional<std::string> { return std::nullopt; };
    const auto num = [&](std::size_t i) { return static_cast<std::size_t>(*ct::parse_int<std::int64_t>(extra.at(i))); };
    Json r;
    using Pass = std::optional<std::string> (*)(std::string_view);
    using Plain = std::string (*)(std::string_view);
    static const std::map<std::string, Pass> optional_passes = {
        {"pre_steps", forms::pre_steps}, {"and_returns", forms::and_returns}, {"assign_in_tests", forms::assign_in_tests},
        {"default_last", forms::default_last}, {"empty_cases_first", forms::empty_cases_first}, {"or_loops", forms::or_loops},
        {"compare_results", forms::compare_results}, {"register_reads", forms::register_reads}, {"switch_breaks", forms::switch_breaks},
        {"default_after", forms::default_after}, {"push_tails", forms::push_tails}, {"split_ternaries", forms::split_ternaries},
        {"guard_returns", forms::guard_returns}, {"shared_tails", forms::shared_tails}, {"frame_more", forms::frame_more},
        {"lead_filler", forms::lead_filler}, {"unsigned_registers", forms::unsigned_registers}, {"no_default", forms::no_default},
        {"unmasked", forms::unmasked}, {"high_words", forms::high_words}, {"assign_in_args", forms::assign_in_args},
        {"return_sources", forms::return_sources}, {"compound_assigns", forms::compound_assigns}, {"chain_assigns", forms::chain_assigns},
        {"register_temps", forms::register_temps}, {"inner_registers", forms::inner_registers},
        {"store_increments", forms::store_increments}};
    static const std::map<std::string, Plain> plain_passes = {
        {"pointer_flow", forms::pointer_flow}, {"unused_locals", forms::unused_locals}, {"word_constants", forms::word_constants},
        {"char_params", forms::char_params}, {"loop_gotos", forms::loop_gotos}, {"store_reloads", forms::store_reloads},
        {"fn_pointers", forms::fn_pointers}, {"scaled_index", forms::scaled_index}, {"shared_arms", forms::shared_arms},
        {"bool_trees", forms::bool_trees}, {"guard_gotos", forms::guard_gotos}, {"merge_conditions", forms::merge_conditions},
        {"case_falls", forms::case_falls}, {"char_stores", forms::char_stores}, {"register_values", forms::register_values},
        {"shapes", forms::shapes}};
    if (const auto it = optional_passes.find(name); it != optional_passes.end()) {
        r = json_of(it->second(text));
    } else if (const auto pt = plain_passes.find(name); pt != plain_passes.end()) {
        r = pt->second(text);
    } else if (name == "pre_increments") {
        r = json_of(forms::pre_increments(text, w.facts));
    } else if (name == "guard_flat") {
        r = json_of(forms::guard_flat(text, no_flat));
    } else if (name == "guard_shared") {
        r = json_of(forms::guard_shared(text, no_flat));
    } else if (name == "scaled_places") {
        r = json_of(forms::scaled_places(text, w.facts));
    } else if (name == "register_copies") {
        r = json_of(forms::register_copies(text, w.facts));
    } else if (name == "word_arrays") {
        r = forms::word_arrays(text, w.facts, extra.empty() ? 0x40u : static_cast<std::uint32_t>(num(0)));
    } else if (name == "char_param_variants") {
        r = json_of(forms::char_param_variants(text, extra.empty() ? 2 : num(0)));
    } else if (name == "pointer_widths") {
        r = json_of(forms::pointer_widths(text, extra.empty() ? 2 : num(0)));
    } else if (name == "unsigned_globals") {
        r = json_of(forms::unsigned_globals(text, extra.empty() ? 2 : num(0)));
    } else if (name == "address_variants") {
        r = Json::array();
        for (const auto& v : forms::address_variants(text, static_cast<std::uint32_t>(num(0)), num(1), static_cast<std::uint32_t>(num(2))))
            r.push_back(Json::array({v.name, v.text}));
    } else if (name == "frame_names") {
        forms::FrameRule rule;
        const Json j = Json::parse(extra.at(0));
        if (j.contains("mod")) rule.mod = j.at("mod").get<std::uint32_t>();
        r = json_of(forms::frame_names(text, rule, {extra.at(1) == "1", extra.at(2) == "1"}));
    } else if (name == "drop_decl") {
        r = forms::drop_decl(text, extra.at(0));
    } else if (name == "redeclare") {
        std::string fmt = extra.at(1);
        fmt.replace(fmt.find("%s"), 2, extra.at(0));
        r = json_of(forms::redeclare(text, extra.at(0), fmt));
    } else if (name == "library_macros") {
        r = forms::library_macros(text, extra.at(0) == "a" ? std::span<const forms::LibraryMacroSpec>(kMacros) : std::span<const forms::LibraryMacroSpec>(), w.facts);
    } else if (name == "table_macros") {
        r = forms::table_macros(w.prj1, w.caches, w.facts, text, kTableSpecs);
    } else if (name == "data_macros") {
        r = forms::data_macros(w.prj1, w.caches, w.facts, text, kDataSpecs);
    } else if (name == "header_text") {
        r = forms::header_text(extra.at(1) == "1" ? w.prj1 : w.prj2, extra.at(0));
    } else if (name == "header_names") {
        const auto names = forms::header_names(extra.at(1) == "1" ? w.prj1 : w.prj2, w.caches, extra.at(0));
        r = json_of(Strings(names.begin(), names.end()));
    } else {
        throw std::runtime_error("unknown op " + op);
    }
    Json out = Json::object();
    out["r"] = r;
    if (const Json t = tables_of(w.facts); !t.empty()) out["t"] = t;
    return out;
}

}  // namespace

// ---- the golden file: small real drafts the Python original was run on (reviewed), with the results it gave -----------
// One case per line: `op <TAB> S<text>|N <TAB> S<extra> ... <TAB> => <TAB> {"r": result, "t": tables}`; the text escapes
// \t \n \r and \\. The cases are the smallest texts of the old harness's corpus (8475 draft bodies) on which each pass
// changes something, plus one on which it does not; every result was read and judged right (the pass does what its
// comment says) before it was kept.
CB_TEST("exact.draft.forms.golden_cases_from_the_python_module", [] {
    const std::string file = fs::read_text(testing::data_path("exact/testdata/draft_forms.cases"));
    std::size_t cases = 0;
    for (std::string_view line : ct::lines(file)) {
        const std::vector<std::string_view> fields = ct::split(line, '\t');
        std::size_t arrow = fields.size();
        while (arrow-- > 0 && fields[arrow] != "=>") {}
        CB_CHECK(arrow != static_cast<std::size_t>(-1) && arrow + 2 == fields.size());
        std::optional<std::string> text;
        if (fields[1] != "N") text = unescape(fields[1].substr(1));
        Strings extra;
        for (std::size_t i = 2; i < arrow; ++i) extra.push_back(unescape(fields[i].substr(1)));
        const Json expected = Json::parse(fields[arrow + 1]);
        Json got;
        try {
            got = run_case(std::string(fields[0]), text, extra);
        } catch (const std::exception& e) {
            got = Json::object({{"error", e.what()}});
        }
        if (got != expected)
            testing::record_failure(__FILE__, __LINE__,
                                    std::format("{} {}\n    got:      {}\n    expected: {}", fields[0], fields[1].substr(0, 200),
                                                got.dump().substr(0, 400), expected.dump().substr(0, 400)));
        ++cases;
    }
    CB_CHECK(cases > 150);
});

// ---- the frame ---------------------------------------------------------------------------------------------------

CB_TEST("exact.draft.forms.frame_names_by_the_compilers_buckets", [] {
    // mod 16: v10 (a long) takes the bucket after v6's; its name's characters sum to a bucket not past v6's, so a
    // letter is put after it - the first that gives the next bucket and is not a name in the text
    const std::string src = "int f(x)\nint x;\n{\n    char v6[6];\n    long v10;\n    return ax;\n}";
    const auto renamed = forms::frame_names(src, {16u});
    CB_CHECK(renamed.has_value());
    CB_CHECK_EQ(*renamed, "int f(x)\nint x;\n{\n    char v6[6];\n    long v10f;\n    return ax;\n}");
    CB_CHECK(!forms::frame_names(src, {}).has_value());        // no rule
    CB_CHECK(!forms::frame_names(src, {0u}).has_value());      // a modulus of 0 is no rule
    CB_CHECK(!forms::frame_names("int x;", {16u}).has_value()); // no body
    // a filler for the gap before v6 only when the toolchain gives a register local a slot
    const std::string gap = "int f(x)\n{\n    int v5;\n    return ax;\n}";
    CB_CHECK(forms::frame_names(gap, {7u}, {.register_local_has_slot = true}).has_value());
});

CB_TEST("exact.draft.forms.declarations_are_edited_by_name", [] {
    CB_CHECK_EQ(forms::drop_decl("f(a, b)\nint a, b;\n{\n}", "a"), "f(a, b)\nint b;\n{\n}");
    CB_CHECK_EQ(forms::drop_decl("f(a)\nint a;\n{\n}", "a"), "f(a)\n{\n}");       // the whole line goes
    CB_CHECK_EQ(forms::drop_decl("f(a)\nint ab;\n{\n}", "a"), "f(a)\nint ab;\n{\n}");   // a name is not a prefix of another
    CB_CHECK_EQ(*forms::redeclare("{\n    int a, p, b;\n}", "p", "char *p"), "{\n    int a, b;\n    char *p;\n}");
    CB_CHECK_EQ(*forms::redeclare("{\n    register int p;\n}", "p", "int *p"), "{\n    register int *p;\n}");
    CB_CHECK(!forms::redeclare("{\n    return p;\n}", "p", "int p").has_value());   // `return p;` is no declaration
    CB_CHECK(!forms::redeclare("{\n    int a;\n}", "p", "int p").has_value());
});

// ---- the library --------------------------------------------------------------------------------------------------

CB_TEST("exact.draft.forms.keeps_the_include_a_macro_does_not_name", [] {
    const std::string stream_read =
        "{\n    stdin->f2--;\n    if ((stdin->f2 >= 0)) {\n        x = (*stdin->f0++ = *p);\n    } else {\n        x = _filbuf(stdin);\n    }\n}";
    dt::FunctionFacts facts;
    facts.include = "stdio.h";   // (given by an earlier pass)
    const std::vector<forms::LibraryMacroSpec> without_header = {{"_filbuf", "getc(%s)", "FILE", std::nullopt, {}}};
    const std::string t = forms::library_macros(stream_read, without_header, facts);
    CB_CHECK_EQ(t, "{\n    ax = getc(stdin);\n}");
    CB_CHECK(facts.include.has_value() && *facts.include == "stdio.h");   // Python stored None over it
    CB_CHECK_EQ(facts.streams.at("stdin"), "FILE");
});

CB_TEST("exact.draft.forms.library_macros_error_paths", [] {
    const std::string stream_read =
        "{\n    stdin->f2--;\n    if ((stdin->f2 >= 0)) {\n        x = _filbuf(stdin);\n    }\n}";
    dt::FunctionFacts facts;
    const std::vector<forms::LibraryMacroSpec> too_many = {{"_filbuf", "getc(%s, %s)", "FILE", std::nullopt, {}}};
    const std::vector<forms::LibraryMacroSpec> too_few = {{"_filbuf", "getc()", "FILE", std::nullopt, {}}};
    const std::vector<forms::LibraryMacroSpec> bad_conversion = {{"_filbuf", "getc(%d)", "FILE", std::nullopt, {}}};
    CB_CHECK_THROWS_AS(UsageError, (void)forms::library_macros(stream_read, too_many, facts));
    CB_CHECK_THROWS_AS(UsageError, (void)forms::library_macros(stream_read, too_few, facts));
    CB_CHECK_THROWS_AS(UsageError, (void)forms::library_macros(stream_read, bad_conversion, facts));
    // a spec for a routine the text does not call is never asked
    CB_CHECK_EQ(forms::library_macros("{\n    return 1;\n}", too_many, facts), "{\n    return 1;\n}");
    // `%%` is a percent sign
    const std::vector<forms::LibraryMacroSpec> percent = {{"_filbuf", "100%% sure(%s)", "FILE", std::nullopt, {}}};
    CB_CHECK_EQ(forms::library_macros(stream_read, percent, facts), "{\n    ax = 100% sure(stdin);\n}");
    // no stream count stepped before the call: the call stays
    CB_CHECK_EQ(forms::library_macros("{\n    x = _filbuf(f);\n}", percent, facts), "{\n    x = _filbuf(f);\n}");
});

CB_TEST("exact.draft.forms.data_and_table_macros", [] {
    World w;
    const std::string source = "{\n    foo(g_1007, 4096, 4104, 99999);\n    return g_1006 + g_2000;\n}";
    const std::string out = forms::data_macros(w.prj1, w.caches, w.facts, source, kDataSpecs);
    CB_CHECK_EQ(out, "{\n    foo(fileno(stdin), stdin, stdout, 99999);\n    return feof(stdin) + g_2000;\n}");
    CB_CHECK(w.facts.include.has_value() && *w.facts.include == "stdio.h");
    // no specs: nothing read, nothing asked
    dt::FunctionFacts none;
    CB_CHECK_EQ(forms::data_macros(w.prj1, w.caches, none, source, {}), source);
    CB_CHECK(!none.include.has_value());
    // an element of size 0 is a spec error - only when something matches
    const std::vector<forms::DataMacroSpec> zero = {{"__iob", 0, {"a"}, {}, std::nullopt}};
    CB_CHECK_EQ(forms::data_macros(w.prj1, w.caches, none, "{\n    return 1;\n}", zero), "{\n    return 1;\n}");
    CB_CHECK_THROWS_AS(UsageError, (void)forms::data_macros(w.prj1, w.caches, none, source, zero));
    // a format without a place for the name is a spec error too
    const std::vector<forms::DataMacroSpec> no_slot = {{"__iob", 8, {"stdin"}, {{7, "fileno()"}}, std::nullopt}};
    CB_CHECK_THROWS_AS(UsageError, (void)forms::data_macros(w.prj1, w.caches, none, source, no_slot));
    // a project whose library places could not be read: nothing found, text as it was
    CB_CHECK_EQ(forms::data_macros(w.prj2, w.caches, none, source, kDataSpecs), source);

    // ctype.h: the macros' table place and masks come from the kit's header, the place from the library
    dt::FunctionFacts tf;
    const std::string t = "{\n    if (((c->f8193 & 4) != 0)) x = 1;\n    if (((c->f8193 & 4) == 0)) x = 2;\n    x = (c->f8193 & 3);\n    y = (c->f1 & 4);\n}";
    CB_CHECK_EQ(forms::table_macros(w.prj1, w.caches, tf, t, kTableSpecs),
                "{\n    if (isdigit(c)) x = 1;\n    if (!isdigit(c)) x = 2;\n    x = isalpha(c);\n    y = (c->f1 & 4);\n}");
    CB_CHECK(tf.include.has_value() && *tf.include == "ctype.h" && tf.include2.has_value());
    dt::FunctionFacts untouched;
    CB_CHECK_EQ(forms::table_macros(w.prj2, w.caches, untouched, t, kTableSpecs), t);   // no kit: no macros
    CB_CHECK(!untouched.include.has_value());
});

CB_TEST("exact.draft.forms.the_kits_headers", [] {
    World w;
    CB_CHECK(forms::header_text(w.prj1, "CTYPE.H").starts_with("#define _UPPER"));    // any case
    CB_CHECK(forms::header_text(w.prj1, "stdio.h").starts_with("int fopen();"));        // in a subfolder
    CB_CHECK(forms::header_text(w.prj1, "nothing.h").empty());
    CB_CHECK(forms::header_text(w.prj2, "ctype.h").empty());                            // no kit
    const auto names = forms::header_names(w.prj1, w.caches, "STDIO.H");
    CB_CHECK((names == std::set<std::string>{"fclose", "fgetc", "fopen", "getc", "printf", "putc"}));
    CB_CHECK(w.caches.headers.contains({"msc300", "stdio.h"}));   // cached under the lower-case name
    // two headers of the same name: the first of a name-ordered walk, files before subfolders
    fs::ScopedTempDir dir("forms_kit");
    fs::write_text(dir.path() / "z.h", "int top();\n");
    fs::write_text(dir.path() / "inc" / "z.h", "int nested();\n");
    forms::KitContext kit{"PRJ3", "tc", dir.path(), nullptr};
    CB_CHECK_EQ(forms::header_text(kit, "z.h"), "int top();\n");
});

// ---- tests, stores, control flow -----------------------------------------------------------------------------------

CB_TEST("exact.draft.forms.register_temps_names_the_whole_variable", [] {
    // `si` is not declared: `r_si` ends with it, and Python took that for its declaration
    const std::string src = "int f(x)\nint x;\n{\n    register int r_si;\n    si = foo(a);\n    x = (si + 1);\n}";
    CB_CHECK(!forms::register_temps(src).has_value());
    const std::string good = "int f(x)\nint x;\n{\n    register int r_si;\n    r_si = foo(a);\n    x = (strlen(b) + r_si);\n}";
    CB_CHECK_EQ(*forms::register_temps(good), "int f(x)\nint x;\n{\n    x = (strlen(b) + foo(a));\n}");
});

CB_TEST("exact.draft.forms.constants_and_steps", [] {
    CB_CHECK_EQ(forms::word_constants("x = 65535; y = 32768; z = 32767; w = 100000; v = 1.50000; u = a50000;"),
                "x = -1; y = -32768; z = 32767; w = 100000; v = 1.50000; u = a50000;");
    dt::FunctionFacts facts;
    CB_CHECK_EQ(*forms::pre_increments("    p = p + 2;\n    x = *p;", facts), "    x = *++p;");
    CB_CHECK(facts.word_pointers.contains("p"));
    CB_CHECK(!forms::pre_increments("    p = p + 2;\n    x = *p++;", facts).has_value());
    CB_CHECK(!forms::pre_steps("    x--;\n    y = z;").has_value());
    CB_CHECK_EQ(*forms::pre_steps("    x--;\n    if ((x == 0)) {"), "    if ((--x == 0)) {");
});

CB_TEST("exact.draft.forms.control_flow_corners", [] {
    // a default that falls into a case that returns: the statements after the switch (and the original's blank-line loss
    // is not reproduced: such a text is left alone)
    const std::string sw = "    switch (x) {\n    default:\n        foo(bx);\n    case 3:\n        return 0;\n    }";
    CB_CHECK_EQ(*forms::default_after(sw), "    switch (x) {\n    case 3:\n        return 0;\n    }\n    foo(bx);\n    return 0;");
    const std::string lossy = "    switch (x) {\n    default:\n        foo(bx);\n    case 3:\n        return 0;\n\n        bar();\n    }";
    CB_CHECK(!forms::default_after(lossy).has_value());
    // a case label on the first line: no line before it
    CB_CHECK_EQ(forms::case_falls("case 1:\nL_1:\n    return 0;"), "case 1:\nL_1:\n    return 0;");
    // an if-tree whose labels go round is no test (and does not recurse for ever)
    const std::string round = "    if ((a)) {\n        L_1:\n        goto L_1;\n    } else {\n        ax = 0;\n    }\n    return ax;";
    CB_CHECK_EQ(forms::bool_trees(round), round);
    // text without a function body: the passes that read the body leave it
    CB_CHECK_EQ(forms::unused_locals("int x;"), "int x;");
    CB_CHECK(!forms::inner_registers("int x;").has_value());
    CB_CHECK(!forms::frame_more("int x;").has_value());
});

CB_TEST("exact.draft.forms.guards_take_the_flat_returns_they_are_handed", [] {
    const std::string src = "f(x)\n{\n    if ((x < 3)) {\n        y = 1;\n    } else {\n        return 1;\n    }\n}";
    const auto none = forms::guard_flat(src, [](std::string_view) { return std::optional<std::string>(); });
    CB_CHECK(none.has_value());
    CB_CHECK_EQ(*none, "f(x)\n{\n    if (x >= 3) return 1;\n    y = 1;\n}");
    const auto flat = forms::guard_flat(src, [](std::string_view t) { return std::optional<std::string>(std::string(t) + "\n/*flat*/"); });
    CB_CHECK(flat.has_value() && flat->ends_with("/*flat*/"));
});

CB_TEST("exact.draft.forms.variants_for_the_beam", [] {
    const std::string src = "int f(a, b)\nint a, b;\n{\n    return (a + b);\n}";
    const auto chars = forms::char_param_variants(src);
    CB_CHECK_EQ(chars.size(), 3u);
    CB_CHECK_EQ(chars[0], "int f(a, b)\nint b;\nchar a;\n{\n    return (a + b);\n}");
    CB_CHECK_EQ(chars[2], "int f(a, b)\nchar a, b;\n{\n    return (a + b);\n}");
    CB_CHECK_EQ(forms::char_param_variants(src, 1).size(), 2u);   // `most` limits both
    CB_CHECK_EQ(forms::address_variants("    char *q;\n    q = 4660;", 0x2000)[0].name, "g_1234");
    CB_CHECK(forms::address_variants("    char *q;\n    q = 4660;", 0x1000).empty());   // past the data
});
