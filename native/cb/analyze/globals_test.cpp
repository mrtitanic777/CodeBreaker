// Tests of the globals analysis on a small hand-assembled 32-bit program whose functions use data the way each kind of
// variable is used (a written constant, a counter, a pointer that is followed, an array, a float, flags, the result of
// a system call, a table of function addresses), and on the report's summary.
#include "cb/analyze/globals.hpp"

#include "cb/analyze/test_image.hpp"
#include "cb/core/testing.hpp"

namespace {

using namespace cb::analyze;
using cb::analyze::test::Asm;

constexpr Va kCode = 0x401000, kData = 0x402000;
constexpr Va kWritten = 0x402100, kCounter = 0x402104, kRead = 0x402108, kPointer = 0x402110, kResult = 0x402120, kFloat = 0x402130,
             kArray = 0x402140, kFlags = 0x402150, kUninit = 0x402450, kTable = 0x402200, kStringAt = 0x402000, kExit = 0x402300;
constexpr Va kF1 = 0x401000, kF2 = 0x401060, kB = 0x4010A0, kF = 0x4010B0, kEnd = 0x4010C0;

struct Program {
    ImageSpec spec;
    Program() {
        Asm a(kCode);
        // f1
        a.raw({0xC7, 0x05}).dword(kWritten).dword(5);                 // mov dword [kWritten], 5
        a.raw({0xFF, 0x05}).dword(kCounter);                          // inc dword [kCounter]
        a.raw({0xA1}).dword(kRead);                                   // mov eax, [kRead]
        a.raw({0xA1}).dword(kPointer).raw({0x8B, 0x08});              // mov eax, [kPointer] ; mov ecx, [eax]   (followed)
        a.call_indirect(kExit).raw({0xA3}).dword(kResult);            // call [ExitProcess] ; mov [kResult], eax
        a.raw({0xD9, 0x05}).dword(kFloat);                            // fld dword [kFloat]
        a.raw({0x8B, 0x04, 0x8D}).dword(kArray);                      // mov eax, [ecx*4 + kArray]
        a.raw({0x83, 0x0D}).dword(kFlags).raw({0x01});                // or dword [kFlags], 1
        a.raw({0x83, 0x25}).dword(kFlags).raw({0x02});                // and dword [kFlags], 2
        a.raw({0xA3}).dword(kUninit);                                 // mov [kUninit], eax
        a.raw({0x68}).dword(kStringAt);                               // push offset "Hi"  (its address is taken)
        a.ret().pad_to(kF2);
        // f2 reads the same counter
        a.raw({0xA1}).dword(kCounter).ret().pad_to(kB);
        a.raw({0x33, 0xC0}).ret().pad_to(kF);
        a.raw({0x33, 0xC0}).ret().pad_to(kEnd);

        Span code;
        code.va = kCode;
        code.data = a.bytes();
        code.executable = true;
        code.initialised = static_cast<Va>(code.data.size());
        code.name = ".text";
        Span data;
        data.va = kData;
        data.data.assign(0x500, 0);
        data.initialised = 0x400;
        data.writable = true;
        data.name = ".data";
        const char hi[] = "Hi there";
        std::copy(hi, hi + sizeof hi, data.data.begin());
        put32(data, kCounter, 3);
        put32(data, kRead, 0x1234);
        put32(data, kTable, static_cast<std::uint32_t>(kB));
        put32(data, kTable + 4, static_cast<std::uint32_t>(kF));
        spec.kind = ImageKind::Pe;
        spec.bits = 32;
        spec.flat = true;
        spec.spans = {code, data};
        spec.functions = {{kF1, kF2}, {kF2, kB}, {kB, kF}, {kF, kEnd}};
        spec.function_names = {{kF1, "work"}, {kF2, "other"}, {kB, "b"}, {kF, "f"}};
        spec.imports = {{kExit, "KERNEL32!ExitProcess"}};
        spec.roots = {kF1};
    }
    static void put32(Span& s, Va va, std::uint32_t v) {
        for (int i = 0; i < 4; ++i) s.data[static_cast<std::size_t>(va - s.va) + i] = static_cast<std::uint8_t>(v >> (8 * i));
    }
};

struct Prepared {
    Program program;
    Image image{std::move(program.spec)};
    call_graph::Graph graph;
    strings::Table table;
    std::map<Va, globals::Variable> vars;
    Prepared() {
        graph = call_graph::build(image);
        table = strings::build_table(image, graph);
        vars = globals::variables(image, graph, table);
    }
};

}  // namespace

CB_TEST("analyze.globals.every_data_address_the_code_names_is_a_variable", [] {
    Prepared p;
    for (Va va : {kWritten, kCounter, kRead, kPointer, kResult, kFloat, kFlags, kUninit}) CB_CHECK(p.vars.contains(va));
    CB_CHECK(!p.vars.contains(kExit));      // an import's slot is not data
    CB_CHECK(!p.vars.contains(kF1));        // code is not a variable
    CB_CHECK(p.vars.contains(kArray));      // (indexed: seen through [reg*4 + address])
});

CB_TEST("analyze.globals.a_written_constant_and_who_wrote_it", [] {
    Prepared p;
    const globals::Variable& g = p.vars.at(kWritten);
    CB_CHECK_EQ(g.size, 4);
    CB_CHECK(g.writers == std::set<Va>{kF1});
    CB_CHECK(g.readers.empty());
    CB_CHECK(g.values == std::set<std::int64_t>{5});
    CB_CHECK(g.writable);
    CB_CHECK_EQ(g.type, std::string("unsigned long"));
    CB_CHECK(g.initial.kind == globals::Initial::Kind::Value);
    CB_CHECK_EQ(g.initial.value, std::uint64_t{0});
});

CB_TEST("analyze.globals.a_counter_has_readers_and_its_initial_value", [] {
    Prepared p;
    const globals::Variable& g = p.vars.at(kCounter);
    CB_CHECK(g.notes.contains("counter"));
    CB_CHECK_EQ(g.type, std::string("counter (4 bytes)"));
    CB_CHECK(g.readers == (std::set<Va>{kF1, kF2}) || g.readers == std::set<Va>{kF2});
    CB_CHECK(g.writers.contains(kF1));
    CB_CHECK_EQ(g.initial.value, std::uint64_t{3});
});

CB_TEST("analyze.globals.a_value_that_is_dereferenced_is_a_pointer", [] {
    Prepared p;
    const globals::Variable& g = p.vars.at(kPointer);
    CB_CHECK(g.notes.contains("pointer"));
    CB_CHECK_EQ(g.type, std::string("pointer"));
});

CB_TEST("analyze.globals.the_result_of_a_system_call_is_named_for_it", [] {
    Prepared p;
    const globals::Variable& g = p.vars.at(kResult);
    CB_CHECK(g.notes.contains("result of ExitProcess"));
    CB_CHECK_EQ(g.type, std::string("result of ExitProcess"));
});

CB_TEST("analyze.globals.floats_arrays_and_flags", [] {
    Prepared p;
    CB_CHECK_EQ(p.vars.at(kFloat).type, std::string("float"));
    const globals::Variable& array = p.vars.at(kArray);
    CB_CHECK(array.notes.contains("array"));
    CB_CHECK_EQ(array.element, 4);
    CB_CHECK_EQ(array.type, std::string("unsigned long[]"));
    const globals::Variable& flags = p.vars.at(kFlags);
    CB_CHECK(flags.notes.contains("flags"));
    CB_CHECK(flags.values == (std::set<std::int64_t>{1, 2}));
    CB_CHECK_EQ(flags.type, std::string("flags (4 bytes)"));
});

CB_TEST("analyze.globals.memory_the_file_only_reserves_is_uninitialised", [] {
    Prepared p;
    CB_CHECK(p.vars.at(kUninit).initial.kind == globals::Initial::Kind::Uninitialised);
    CB_CHECK(p.vars.at(kWritten).initial.kind == globals::Initial::Kind::Value);
});

CB_TEST("analyze.globals.a_string_whose_address_is_taken_is_typed_as_one", [] {
    Prepared p;
    const globals::Variable& g = p.vars.at(kStringAt);
    CB_CHECK_EQ(g.taken, 1);
    CB_CHECK_EQ(g.type, std::string("char[9]"));       // "Hi there" and its terminator
    CB_CHECK(g.extent > 0 && g.extent <= 0x10000);
});

CB_TEST("analyze.globals.the_extent_runs_to_the_next_thing_known", [] {
    Prepared p;
    CB_CHECK_EQ(p.vars.at(kWritten).extent, kCounter - kWritten);
    CB_CHECK_EQ(p.vars.at(kUninit).extent, 0x402500 - kUninit);      // to the end of the span
});

CB_TEST("analyze.globals.a_run_of_function_addresses_is_a_table", [] {
    Prepared p;
    const std::vector<globals::PointerTable> tables = globals::pointer_tables(p.image, p.table);
    CB_CHECK_EQ(tables.size(), std::size_t{1});
    CB_CHECK_EQ(tables[0].at, kTable);
    CB_CHECK_EQ(tables[0].count, 2);
    CB_CHECK_EQ(tables[0].stride, 4);
    CB_CHECK(tables[0].shape == globals::PointerTable::Shape::Run);
    CB_CHECK((tables[0].targets == std::vector<Va>{kB, kF}));
    CB_CHECK(tables[0].kinds[0] == globals::SlotKind::Function && tables[0].kinds[1] == globals::SlotKind::Function);
    CB_CHECK_EQ(std::string(globals::name_of(tables[0].kinds[0])), "function");
});

CB_TEST("analyze.globals.the_report_counts_and_summarises", [] {
    Prepared p;
    const globals::Report r = globals::analyse(p.image, p.graph, p.table);
    CB_CHECK_EQ(r.counts.globals, static_cast<std::int64_t>(p.vars.size()));
    CB_CHECK_EQ(r.counts.counters, 1);
    CB_CHECK_EQ(r.counts.flags, 1);
    CB_CHECK_EQ(r.counts.floats, 1);
    CB_CHECK_EQ(r.counts.strings, 1);
    CB_CHECK_EQ(r.counts.results_of_system_calls, 1);
    CB_CHECK_EQ(r.counts.pointers, 1);
    CB_CHECK_EQ(r.counts.arrays, 1);                       // (the string is not an array)
    CB_CHECK_EQ(r.counts.uninitialised, 1);
    CB_CHECK_EQ(r.counts.pointer_tables, 1);
    CB_CHECK_EQ(r.counts.table_entries, 2);
    CB_CHECK_EQ(r.counts.function_table_entries, 2);
    CB_CHECK(r.counts.written_never_read >= 1);            // kWritten, kResult, kUninit ...
    CB_CHECK_EQ(r.globals.size(), p.vars.size());
    CB_CHECK_EQ(r.tables.size(), std::size_t{1});
    CB_CHECK_EQ(r.tables[0].holds.at("function"), 2);
    CB_CHECK_EQ(r.tables[0].first.size(), std::size_t{2});
    CB_CHECK_EQ(r.tables[0].first[0], std::string("b"));
    CB_CHECK(r.text.front().starts_with("types: "));
    CB_CHECK(r.text.back().starts_with("  table 402200: 2 function, e.g. b, f"));
    // the busiest variable first: the counter is read by two functions and written by one
    CB_CHECK(r.text[1].starts_with("  402104  counter (4 bytes)"));
    CB_CHECK(r.text[1].find("read by 2, written by 1") != std::string::npos || r.text[1].find("read by 1, written by 1") != std::string::npos);
});

CB_TEST("analyze.globals.facts_report_counts_not_sets_and_at_most_24_values", [] {
    std::map<Va, globals::Variable> vars;
    globals::Variable v;
    v.at = 0x500000;
    v.type = "unsigned long";
    v.readers = {1, 2, 3};
    v.writers = {1};
    for (int i = 0; i < 40; ++i) v.values.insert(i);
    v.notes = {"b", "a"};
    vars[v.at] = v;
    const globals::Report r = globals::make_report(vars, {}, [](Va a) { return std::to_string(a); });
    CB_CHECK_EQ(r.globals.size(), std::size_t{1});
    CB_CHECK_EQ(r.globals[0].readers, 3);
    CB_CHECK_EQ(r.globals[0].writers, 1);
    CB_CHECK_EQ(r.globals[0].values.size(), std::size_t{24});
    CB_CHECK((r.globals[0].notes == std::vector<std::string>{"a", "b"}));
    CB_CHECK(r.text[1].find("values 0,1,2,3,4,5,6,7") != std::string::npos);
}
);

CB_TEST("analyze.globals.arrays_are_indexed_types_not_strings", [] {
    std::map<Va, globals::Variable> vars;
    for (const char* t : {"unsigned char[]", "unsigned long[]", "char[12]", "wchar_t[4]", "unsigned long"}) {
        globals::Variable v;
        v.at = static_cast<Va>(0x500000 + vars.size() * 0x10);
        v.type = t;
        vars[v.at] = v;
    }
    const globals::Report r = globals::make_report(vars, {}, [](Va) { return std::string(); });
    CB_CHECK_EQ(r.counts.arrays, 2);
    CB_CHECK_EQ(r.counts.strings, 2);
});

// ---- against what the engine it replaces answered on real modules ---------------------------------------------------
//
// testdata/<module>.globals.json is what cbanalyze.dll's globals.cc answered (variables and tables) for the fifteen
// modules of golden_image.hpp, and the report the live Python made of them (made by testdata/gen_globals.py).

#include <nlohmann/json.hpp>

#include "cb/analyze/golden_image.hpp"
#include "cb/core/json.hpp"

namespace {

void expect(bool ok, const std::string& what) {
    cb::testing::count_check();
    if (!ok) cb::testing::record_failure(__FILE__, __LINE__, what);
}

std::set<std::int64_t> set_of(const cb::Json& j) {
    std::set<std::int64_t> s;
    for (const cb::Json& x : j) s.insert(x.get<std::int64_t>());
    return s;
}

bool same_variable(const globals::Variable& g, const cb::Json& row) {
    std::set<std::string> notes;
    for (const cb::Json& x : row.at("notes")) notes.insert(x.get<std::string>());
    bool same = g.size == row.at("size").get<std::int64_t>() && g.taken == row.at("taken").get<std::int64_t>() &&
                g.writable == row.at("writable").get<bool>() && g.element == row.at("element").get<std::int64_t>() &&
                g.extent == row.at("extent").get<std::int64_t>() && g.notes == notes && g.type == row.at("type").get<std::string>() &&
                g.values == set_of(row.at("values"));
    same = same && std::set<std::int64_t>(g.readers.begin(), g.readers.end()) == set_of(row.at("readers")) &&
           std::set<std::int64_t>(g.writers.begin(), g.writers.end()) == set_of(row.at("writers"));
    const cb::Json& initial = row.at("initial");
    if (initial.is_number()) return same && g.initial.kind == globals::Initial::Kind::Value && g.initial.value == initial.get<std::uint64_t>();
    if (initial.get<std::string>() == "uninitialised") return same && g.initial.kind == globals::Initial::Kind::Uninitialised;
    return same && g.initial.kind == globals::Initial::Kind::Address && g.initial.label == initial.get<std::string>();
}

bool same_counts(const globals::Counts& n, const cb::Json& c) {
    auto v = [&](const char* k) { return c.at(k).get<std::int64_t>(); };
    // (the live Python's `arrays` was always 0: see _bugs/LIST.md; it is not compared)
    return n.globals == v("globals") && n.typed_beyond_size == v("typed_beyond_size") && n.pointers == v("pointers") &&
           n.function_pointers == v("function_pointers") && n.strings == v("strings") && n.counters == v("counters") &&
           n.flags == v("flags") && n.floats == v("floats") && n.results_of_system_calls == v("results_of_system_calls") &&
           n.uninitialised == v("uninitialised") && n.written_never_read == v("written_never_read") &&
           n.read_never_written == v("read_never_written") && n.pointer_tables == v("pointer_tables") &&
           n.record_tables == v("record_tables") && n.table_entries == v("table_entries") &&
           n.function_table_entries == v("function_table_entries");
}

}  // namespace

CB_TEST("analyze.globals.golden_the_variables_and_tables_of_fifteen_real_modules", [] {
    for (const char* m : cb::analyze::test::kGoldenModules) {
        const std::string module = m;
        const cb::Json want = cb::json::load(cb::testing::data_path("analyze/testdata/" + module + ".globals.json"));
        Image image(cb::analyze::test::read_golden_image(module));
        image.discover();
        const call_graph::Graph graph = call_graph::build(image);
        const strings::Table table = strings::build_table(image, graph);
        const std::map<Va, globals::Variable> vars = globals::variables(image, graph, table);

        expect(vars.size() == want.at("variables").size(), std::format("{}: {} variables, wanted {}", module, vars.size(), want.at("variables").size()));
        for (const cb::Json& row : want.at("variables")) {
            const Va at = row.at("at").get<std::int64_t>();
            const auto it = vars.find(at);
            if (it == vars.end()) {
                expect(false, std::format("{}: no variable at {:x}", module, at));
                continue;
            }
            expect(same_variable(it->second, row), std::format("{}: variable {:x} differs ({})", module, at, row.dump().substr(0, 300)));
        }

        const std::vector<globals::PointerTable> tables = globals::pointer_tables(image, table);
        expect(tables.size() == want.at("tables").size(), std::format("{}: {} tables, wanted {}", module, tables.size(), want.at("tables").size()));
        for (std::size_t i = 0; i < std::min(tables.size(), want.at("tables").size()); ++i) {
            const cb::Json& t = want.at("tables")[i];
            std::vector<Va> targets;
            for (const cb::Json& x : t.at("targets")) targets.push_back(x.get<std::int64_t>());
            std::vector<std::string> kinds, want_kinds;
            for (const globals::SlotKind k : tables[i].kinds) kinds.emplace_back(globals::name_of(k));
            for (const cb::Json& x : t.at("kinds")) want_kinds.push_back(x.get<std::string>());
            expect(tables[i].at == t.at("at").get<std::int64_t>() && tables[i].count == t.at("count").get<std::int64_t>() &&
                       tables[i].stride == t.at("stride").get<std::int64_t>() &&
                       std::string(globals::name_of(tables[i].shape)) == t.at("shape").get<std::string>() &&
                       tables[i].targets == targets && kinds == want_kinds,
                   std::format("{}: table {} differs", module, i));
        }

        const globals::Report report = globals::make_report(vars, tables, [&](Va a) { return image.label(a); });
        expect(same_counts(report.counts, want.at("counts")), std::format("{}: counts differ", module));
        expect(report.text.size() == want.at("text").size(), std::format("{}: summary has {} lines, wanted {}", module, report.text.size(), want.at("text").size()));
    }
});
