// Tests of what running a function shows: the buffers and where an address in one points, the digest of a run record into an
// observation, the contents of a buffer after a run, the choice and order of the functions to run, the runs themselves and the
// idiom battery, and the analysis of the runs against the static recovery.
//
// The synthetic tests build run records by hand and read the observation against the rule that made it. The real tests run the
// functions of two modules of the Windows 95 corpus (testdata/observe/crtdll.dll and lz32.dll, copied from WINDOWS\SYSTEM) in
// the emulator and compare with what the live Python (codebreaker.analyze.observe) made of the same modules:
// testdata/observe/observe.golden.json, made by testdata/gen_observe.py, holds for each module the functions the Python chose to run,
// every observation, and its report. The import-argument resolver answers from the table the Python's runs recorded, and the
// emulator's "an import gives something back through a pointer" table is empty on both sides, so the two runs see the same machine.
//
// DIVERGES from the Python:
//   * The lists `fields_confirmed` and `fields_added` of a structure_observed fact are in offset order (the Python sorted the
//     "+%x" texts: +10 before +8); the tests compare them as sets.
//   * The Python's worker could not run a function at all when the emulator raised; here the error types the emulator throws for a
//     function it cannot lay out (FormatError, EmulationError) become the same fault observation.
#include "cb/analyze/observe.hpp"

#include <algorithm>
#include <cstdio>
#include <format>
#include <map>
#include <set>

#include <nlohmann/json.hpp>

#include "cb/analyze/call_graph.hpp"
#include "cb/analyze/image.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/pe.hpp"

namespace {

using namespace cb::analyze;
using cb::Json;
using cb::knowledge::Value;
namespace emu = cb::emulate::emulator;
namespace nt = cb::emulate::native_trace;
namespace ob = cb::analyze::observe;

void expect(bool ok, const std::string& what) {
    cb::testing::count_check();
    if (!ok) cb::testing::record_failure(__FILE__, __LINE__, what);
}

// ---- hand-made run records --------------------------------------------------------------------------------------------------

constexpr std::uint64_t kA0 = 0x40000000;             // argument 0's buffer
constexpr std::uint64_t kA1 = 0x40010000;             // argument 1's buffer
constexpr std::uint64_t kField = 0x50000000;          // the value argument 0's dword at +0 holds
constexpr std::uint64_t field_of(int k, std::uint64_t o) { return 0x50000000 + (static_cast<std::uint64_t>(k) << 24) + (o << 12); }

emu::Trace returned_trace() {
    emu::Trace t;
    t.exit = emu::Exit::Returned;
    t.ret = 0;
    t.esp_delta = 4;
    return t;
}

nt::MemRead read_at(std::uint64_t addr, std::uint64_t size) {
    nt::MemRead m;
    m.addr = addr;
    m.size = size;
    return m;
}
nt::MemWrite write_at(std::uint64_t addr, std::uint64_t size, std::uint64_t value) {
    nt::MemWrite m;
    m.addr = addr;
    m.size = size;
    m.value = value;
    return m;
}

// The text of a Value with every map's keys in order, so that two documents compare as documents.
std::string canon(const Value& v) {
    switch (v.kind()) {
        case Value::Kind::Null: return "null";
        case Value::Kind::Bool: return *v.boolean() ? "true" : "false";
        case Value::Kind::Int: return std::to_string(*v.integer());
        case Value::Kind::Double: return std::to_string(*v.number());
        case Value::Kind::Text: return "\"" + *v.text() + "\"";
        case Value::Kind::List: {
            std::string out = "[";
            for (const Value& x : *v.list()) out += canon(x) + ",";
            return out + "]";
        }
        case Value::Kind::Map: {
            std::map<std::string, std::string> sorted;
            for (const auto& [k, x] : *v.map()) sorted[k] = canon(x);
            std::string out = "{";
            for (const auto& [k, x] : sorted) out += k + ":" + x + ",";
            return out + "}";
        }
    }
    return "";
}

std::string canon(const Json& j) {
    if (j.is_null()) return "null";
    if (j.is_boolean()) return j.get<bool>() ? "true" : "false";
    if (j.is_number_integer()) return std::to_string(j.get<std::int64_t>());
    if (j.is_string()) return "\"" + j.get<std::string>() + "\"";
    if (j.is_array()) {
        std::string out = "[";
        for (const Json& x : j) out += canon(x) + ",";
        return out + "]";
    }
    std::map<std::string, std::string> sorted;
    for (auto it = j.begin(); it != j.end(); ++it) sorted[it.key()] = canon(it.value());
    std::string out = "{";
    for (const auto& [k, x] : sorted) out += k + ":" + x + ",";
    return out + "}";
}

}  // namespace

// ---- the buffers ---------------------------------------------------------------------------------------------------------------

CB_TEST("analyze.observe.a_buffer_says_where_each_of_its_dwords_came_from", [] {
    const cb::Bytes b = ob::fill(3);
    CB_CHECK_EQ(b.size(), std::size_t{0x1000});
    auto dword = [&](std::size_t o) { return b[o] | (b[o + 1] << 8) | (b[o + 2] << 16) | (static_cast<std::uint32_t>(b[o + 3]) << 24); };
    CB_CHECK_EQ(dword(0), 0x53000000u);              // buffer 3, offset 0
    CB_CHECK_EQ(dword(4), 0x53004000u);              // offset 4: o << 12
    CB_CHECK_EQ(dword(0xFFC), 0x53FFC000u);
    CB_CHECK_EQ(ob::fill(0).size(), std::size_t{0x1000});
    CB_CHECK(ob::fill(1) != ob::fill(2));
});

CB_TEST("analyze.observe.where_tells_a_buffer_from_a_value_read_out_of_one", [] {
    const auto a = ob::where(0x40020004);
    CB_CHECK(a.has_value());
    CB_CHECK(a->kind == ob::Where::Kind::Argument);
    CB_CHECK_EQ(a->k, 2);
    CB_CHECK_EQ(a->o, 4);
    const auto f = ob::where(field_of(5, 0x2A) + 3);   // the dword at offset 0x2A of buffer 5, three bytes into it
    CB_CHECK(f.has_value());
    CB_CHECK(f->kind == ob::Where::Kind::Field);
    CB_CHECK_EQ(f->k, 5);
    CB_CHECK_EQ(f->o, 0x2A);
    CB_CHECK_EQ(f->offset, 3);
    CB_CHECK(ob::where(0x40000FFF).has_value());        // the last byte of buffer 0
    CB_CHECK(!ob::where(0x40001000).has_value());       // past the buffer's size (the rest of its window is nothing)
    CB_CHECK(!ob::where(0x40080000).has_value());       // a ninth buffer
    CB_CHECK(!ob::where(0x50000000 + (std::int64_t{8} << 24)).has_value());
    CB_CHECK(!ob::where(0x1234).has_value());
    CB_CHECK(!ob::where(0x4F000000).has_value());
});

// ---- the digest ---------------------------------------------------------------------------------------------------------------------

CB_TEST("analyze.observe.digest_counts_what_a_run_does_to_each_buffer", [] {
    emu::Trace t = returned_trace();
    t.record.mem_reads = {read_at(kA0 + 8, 4), read_at(kA0 + 8, 2), read_at(kA1, 1), read_at(0x1000, 4)};   // the last is no buffer's
    t.record.mem_writes = {write_at(kA0 + 8, 8, 0), write_at(kA0 + 12, 1, 0)};
    ob::Observation o;
    ob::digest(&t, o);
    CB_CHECK_EQ(o.fields.size(), std::size_t{2});
    const ob::Cell c8 = o.fields.at(0).at(8);
    CB_CHECK(c8 == (ob::Cell{8, 2, 1}));   // the widest access (the 8-byte write), two reads, one write
    CB_CHECK((o.fields.at(0).at(12) == ob::Cell{1, 0, 1}));
    CB_CHECK((o.fields.at(1).at(0) == ob::Cell{1, 1, 0}));
    CB_CHECK(o.nested.empty());
    CB_CHECK(o.exit == ob::Exit::Returned);
});

CB_TEST("analyze.observe.digest_follows_a_pointer_found_in_a_buffer", [] {
    emu::Trace t = returned_trace();
    // argument 2's dword at +0x10 was used as a pointer: its field (3 bytes in) was read and written
    t.record.mem_reads = {read_at(field_of(2, 0x10) + 4, 2)};
    t.record.mem_writes = {write_at(field_of(2, 0x10) + 4, 4, 1)};
    ob::Observation o;
    ob::digest(&t, o);
    CB_CHECK(o.fields.empty());
    const auto& cells = o.nested.at({2, 0x10});
    CB_CHECK((cells.at(4) == ob::Cell{4, 1, 1}));
});

CB_TEST("analyze.observe.digest_keeps_each_indirect_call_once_and_only_those_that_stayed_in_the_module", [] {
    emu::Trace t = returned_trace();
    nt::CallRecord indirect;
    indirect.pc = 0x401000;
    indirect.resolved_target = 0x402000;
    nt::CallRecord same_again = indirect;
    nt::CallRecord direct = indirect;     // a direct call (the static graph has it)
    direct.target = 0x402000;
    direct.pc = 0x401010;
    nt::CallRecord outside = indirect;    // left the module
    outside.pc = 0x401020;
    outside.external = true;
    nt::CallRecord unresolved;            // indirect and never resolved
    unresolved.pc = 0x401030;
    nt::CallRecord to_zero = indirect;    // went to 0
    to_zero.pc = 0x401040;
    to_zero.resolved_target = 0;
    nt::CallRecord elsewhere = indirect;
    elsewhere.resolved_target = 0x403000;
    t.record.calls = {indirect, same_again, direct, outside, unresolved, to_zero, elsewhere};
    ob::Observation o;
    ob::digest(&t, o);
    CB_CHECK((o.indirect == std::vector<ob::Indirect>{{0x401000, 0x402000}, {0x401000, 0x403000}}));
});

CB_TEST("analyze.observe.digest_notes_a_field_handed_to_a_system_function", [] {
    emu::Trace t = returned_trace();
    t.record.api_calls = std::vector<nt::ApiCall>{
        {"USER32!SendMessageA", std::vector<std::int64_t>{static_cast<std::int64_t>(kA0 + 0x20), static_cast<std::int64_t>(field_of(1, 4)),
                                                          static_cast<std::int64_t>(field_of(1, 4) + 2), 7}},
        {"GDI32!SelectObject", std::vector<std::int64_t>{static_cast<std::int64_t>(kA0 + 0x20)}},   // the same pass again
        {"x", std::nullopt}};                                                                         // its arguments are not known
    ob::Observation o;
    ob::digest(&t, o);
    CB_CHECK_EQ(o.passes.size(), std::size_t{3});
    CB_CHECK((o.passes[0] == ob::Pass{"USER32!SendMessageA", 0, 0, 0x20, ob::Pass::How::Address}));   // the address of a place
    CB_CHECK((o.passes[1] == ob::Pass{"USER32!SendMessageA", 1, 1, 4, ob::Pass::How::Value}));        // a value read from a buffer
    // (the third argument points into the middle of such a value: not a value read, not an address)
    CB_CHECK((o.passes[2] == ob::Pass{"GDI32!SelectObject", 0, 0, 0x20, ob::Pass::How::Address}));
    CB_CHECK_EQ(ob::describe(o.passes[0]), "USER32!SendMessageA parameter 0 gets the address of argument 0 +20");
    CB_CHECK_EQ(ob::describe(o.passes[1]), "USER32!SendMessageA parameter 1 gets the value at argument 1 +4");
});

CB_TEST("analyze.observe.digest_reads_the_end_of_a_run", [] {
    {
        emu::Trace t = returned_trace();
        t.esp_delta = 12;
        t.ret = 0x40000000;   // returned its argument 0
        ob::Observation o;
        ob::digest(&t, o);
        CB_CHECK(o.exit == ob::Exit::Returned);
        CB_CHECK_EQ(o.removed.value_or(-99), 8);   // ret 8: the pop minus the return address
        CB_CHECK(o.returns == (ob::Returns{ob::Returns::Kind::Argument, 0, std::nullopt}));
    }
    {
        emu::Trace t = returned_trace();
        t.ret = static_cast<std::uint32_t>(field_of(3, 0x14));   // returned what argument 3's dword at +0x14 held
        ob::Observation o;
        ob::digest(&t, o);
        CB_CHECK_EQ(o.removed.value_or(-99), 0);   // cdecl
        CB_CHECK(o.returns == (ob::Returns{ob::Returns::Kind::Field, 3, 0x14}));
    }
    {
        emu::Trace t = returned_trace();
        t.esp_delta = 0x100;               // too much to be arguments: nothing is said of the removal
        t.ret = 0x40000004;                // a place in the middle of a buffer: not the argument itself
        ob::Observation o;
        ob::digest(&t, o);
        CB_CHECK(!o.removed.has_value());
        CB_CHECK(!o.returns.has_value());
    }
    {
        emu::Trace t = returned_trace();
        t.esp_delta.reset();
        t.ret.reset();                      // no result at all reads as 0: not a buffer
        ob::Observation o;
        ob::digest(&t, o);
        CB_CHECK(!o.removed.has_value());
        CB_CHECK(!o.returns.has_value());
    }
});

CB_TEST("analyze.observe.a_run_that_did_not_return_is_cut_off_or_faulted_and_never_undoes_a_return", [] {
    emu::Trace cut;
    cut.exit = emu::Exit::Truncated;
    cut.record.truncated = true;
    emu::Trace fault;
    fault.exit = emu::Exit::Faulted;
    ob::Observation o;
    ob::digest(&cut, o);
    CB_CHECK(o.exit == ob::Exit::Cut);
    ob::digest(&fault, o);
    CB_CHECK(o.exit == ob::Exit::Fault);   // the latest of the runs that did not return
    emu::Trace ok = returned_trace();
    ob::digest(&ok, o);
    CB_CHECK(o.exit == ob::Exit::Returned);
    ob::digest(&cut, o);
    ob::digest(&fault, o);
    ob::digest(nullptr, o);                // a run that could not be made
    CB_CHECK(o.exit == ob::Exit::Returned);
    ob::Observation never;
    ob::digest(nullptr, never);
    CB_CHECK(never.exit == ob::Exit::Fault);
});

CB_TEST("analyze.observe.after_applies_the_runs_writes_to_a_buffer", [] {
    emu::Trace t = returned_trace();
    t.record.mem_writes = {write_at(0x41000002, 2, 0xBBAA), write_at(0x41000006, 4, 0x11223344),   // inside
                           write_at(0x4100003E, 4, 0xFFFFFFFF),                                      // runs off the end: not its content
                           write_at(0x40FFFFFF, 2, 0xFFFF),                                          // starts before it
                           write_at(0x41000000, 1, 0x1FF)};                                          // a value wider than the write
    const cb::Bytes initial(64, 0xEE);
    const cb::Bytes got = ob::after(t, 0x41000000, initial);
    CB_CHECK_EQ(got.size(), std::size_t{64});
    CB_CHECK_EQ(got[0], 0xFF);
    CB_CHECK_EQ(got[1], 0xEE);
    CB_CHECK_EQ(got[2], 0xAA);
    CB_CHECK_EQ(got[3], 0xBB);
    CB_CHECK_EQ(got[6], 0x44);
    CB_CHECK_EQ(got[9], 0x11);
    CB_CHECK_EQ(got[0x3E], 0xEE);
    CB_CHECK_EQ(got[0x3F], 0xEE);
    CB_CHECK_EQ(initial[2], 0xEE);   // the start is a copy
});

// ---- the functions chosen --------------------------------------------------------------------------------------------------------

namespace {

// A module of the corpus: its Image and the static analyses the observer sets itself against.
struct Module {
    std::filesystem::path path;
    cb::analyze::Image image;
    conventions::Facts facts;
    std::vector<structures::Structure> found;

    explicit Module(const std::string& name)
        : path(cb::testing::data_path("analyze/testdata/observe/" + name)),
          image(image::load_pe(cb::formats::pe::Image::open(path))) {
        const call_graph::Graph graph = call_graph::build(image);
        facts = conventions::functions(image, graph);
        found = structures::recover(image, graph, conventions::summary(facts));
    }

    // The observer's setting: the imports' argument counts the Python's runs recorded, and nothing given back through pointers.
    ob::Setting setting(const Json& argc_table, std::optional<std::size_t> limit) const {
        ob::Setting s;
        s.module = path;
        s.limit = limit;
        s.jobs = 4;
        s.argc = [argc_table](const nt::ImportName& import) -> std::optional<std::int64_t> {
            const std::string key = import.dll + "!" + import.name;
            if (!argc_table.contains(key) || argc_table.at(key).is_null()) return std::nullopt;
            return argc_table.at(key).get<std::int64_t>();
        };
        return s;
    }
};

const Json& golden() {
    static const Json doc = cb::json::load(cb::testing::data_path("analyze/testdata/observe/observe.golden.json"));
    return doc;
}

}  // namespace

CB_TEST("analyze.observe.the_functions_chosen_are_the_ones_the_python_chose_in_its_order", [] {
    for (const char* name : {"crtdll.dll", "lz32.dll"}) {
        Module m(name);
        const Json& g = golden().at(name);
        const std::vector<ob::Candidate> got = ob::candidates(m.image, m.facts, m.found, g.at("limit").get<std::size_t>());
        const Json& want = g.at("candidates");
        expect(got.size() == want.size(), std::string(name) + ": the number of functions chosen");
        for (std::size_t i = 0; i < got.size() && i < want.size(); ++i) {
            const Json& w = want.at(i);
            const std::string where = std::format("{}: candidate {}", name, i);
            expect(got[i].function == w.at(0).get<std::int64_t>(), where + " function");
            expect(got[i].args == (w.at(1).is_null() ? std::nullopt : std::optional<std::int64_t>(w.at(1).get<std::int64_t>())), where + " argument count");
            expect(got[i].pointers == w.at(2).get<std::vector<int>>(), where + " pointer arguments");
            expect(got[i].battery == w.at(3).get<bool>(), where + " battery");
        }
    }
});

CB_TEST("analyze.observe.the_limit_cuts_the_list_after_the_informative_ones", [] {
    Module m("crtdll.dll");
    auto& image = m.image;
    const auto all = ob::candidates(image, m.facts, m.found, std::nullopt);
    const auto ten = ob::candidates(image, m.facts, m.found, 10);
    CB_CHECK(all.size() > 100);
    CB_CHECK_EQ(ten.size(), std::size_t{10});
    for (std::size_t i = 0; i < 10; ++i) CB_CHECK_EQ(ten[i].function, all[i].function);
    // the battery's functions first, then those with pointer arguments, then the rest by address
    bool seen_plain = false, seen_no_pointers = false;
    for (const ob::Candidate& c : all) {
        if (!c.battery) seen_plain = true;
        else CB_CHECK(!seen_plain);
        if (!c.battery && c.pointers.empty()) seen_no_pointers = true;
        else if (!c.battery) CB_CHECK(!seen_no_pointers);
    }
    // no thunk, no empty function, no function that wants a register but ecx
    for (const ob::Candidate& c : all) {
        const conventions::Fact& f = m.facts.at(c.function);
        CB_CHECK(f.shape != conventions::Shape::ImportThunk && f.shape != conventions::Shape::JumpThunk && f.shape != conventions::Shape::Empty);
        for (const std::string& r : f.reg_args) CB_CHECK_EQ(r, "ecx");
    }
});

// ---- running the functions ------------------------------------------------------------------------------------------------------

namespace {

Json obs_json(const ob::Observation& o) {
    auto cells = [](const ob::Cells& c) {
        Json j = Json::object();
        for (const auto& [off, cell] : c) j[std::to_string(off)] = Json::array({cell.size, cell.reads, cell.writes});
        return j;
    };
    Json j = Json::object();
    j["fields"] = Json::object();
    for (const auto& [k, c] : o.fields) j["fields"][std::to_string(k)] = cells(c);
    j["nested"] = Json::object();
    for (const auto& [k, c] : o.nested) j["nested"][std::format("{},{}", k.first, k.second)] = cells(c);
    j["indirect"] = Json::array();
    for (const ob::Indirect& i : o.indirect) j["indirect"].push_back(Json::array({i.site, i.target}));
    j["passes"] = Json::array();
    for (const ob::Pass& p : o.passes)
        j["passes"].push_back(Json::array({p.import, p.parameter, p.argument, p.offset, p.how == ob::Pass::How::Address ? "address" : "value"}));
    j["exit"] = o.exit ? Json(std::string(ob::name_of(*o.exit))) : Json(nullptr);
    j["removed"] = o.removed ? Json(*o.removed) : Json(nullptr);
    if (o.returns) {
        j["returns"] = Json::array({o.returns->kind == ob::Returns::Kind::Argument ? "argument" : "field", o.returns->k});
        if (o.returns->offset) j["returns"].push_back(*o.returns->offset);
    } else {
        j["returns"] = nullptr;
    }
    j["idiom"] = o.idiom ? Json(std::string(ob::name_of(*o.idiom))) : Json(nullptr);
    return j;
}

}  // namespace

CB_TEST("analyze.observe.running_a_module_shows_what_the_python_saw", [] {
    for (const char* name : {"crtdll.dll", "lz32.dll"}) {
        Module m(name);
        const Json& g = golden().at(name);
        const ob::Seen seen = ob::run(m.image, m.facts, m.found, m.setting(golden().at("argc"), g.at("limit").get<std::size_t>()));
        const Json& want = g.at("seen");
        expect(seen.size() == want.size(), std::string(name) + ": the number of functions run");
        int idioms = 0;
        for (const auto& [f, maybe] : seen) {
            const std::string key = std::to_string(f);
            expect(want.contains(key), std::format("{}: {:x} was run", name, f));
            if (!want.contains(key)) continue;
            expect(maybe.has_value(), std::format("{}: {:x} was not lost", name, f));
            if (!maybe) continue;
            const Json got = obs_json(*maybe);
            expect(canon(got) == canon(want.at(key)), std::format("{}: the observation of {:x}\n  got  {}\n  want {}", name, f, canon(got), canon(want.at(key))));
            idioms += maybe->idiom.has_value();
        }
        if (std::string(name) == "crtdll.dll") CB_CHECK(idioms >= 15);   // the battery named functions of the C library
    }
});

CB_TEST("analyze.observe.the_idiom_battery_names_the_library_functions", [] {
    // every function of crtdll.dll the battery was run on (the first 400 chosen), with what the Python's battery answered
    Module m("crtdll.dll");
    const ob::Observer observer(m.setting(golden().at("argc"), std::nullopt));
    std::map<std::string, int> by_name;   // idiom -> how many functions it was found in
    int none = 0;
    for (const Json& row : golden().at("crtdll.dll@battery").at("battery")) {
        const std::int64_t f = row.at(0).get<std::int64_t>();
        const std::optional<std::int64_t> nargs = row.at(1).is_null() ? std::nullopt : std::optional<std::int64_t>(row.at(1).get<std::int64_t>());
        const auto got = observer.idiom(f, nargs);
        const std::string want = row.at(2).is_null() ? "" : row.at(2).get<std::string>();
        expect((got ? std::string(ob::name_of(*got)) : "") == want, std::format("the idiom of {:x}: want '{}'", f, want));
        if (want.empty()) ++none;
        else ++by_name[want];
    }
    // all fourteen are named somewhere in the C library, and the battery also turns functions down
    for (const char* n : {"strlen", "wcslen", "strcpy", "strcat", "strcmp", "stricmp", "strchr", "strrchr", "strncpy", "memcpy", "memset", "memcmp", "memicmp", "strncmp"})
        expect(by_name.contains(n), std::string("an idiom of ") + n + " in the golden");
    CB_CHECK(none > 0);
});

CB_TEST("analyze.observe.a_module_that_is_not_there_is_one_error_not_a_lost_function_each", [] {
    ob::Setting s;
    s.module = cb::testing::data_path("analyze/testdata/observe/nowhere.dll");
    CB_CHECK_THROWS_AS(cb::Error, (void)ob::Observer(s));
    cb::fs::ScopedTempDir dir("cbobs");
    cb::fs::write_text(dir.path() / "text.dll", "this is not a program");
    s.module = dir.path() / "text.dll";
    CB_CHECK_THROWS_AS(cb::FormatError, (void)ob::Observer(s));
});

CB_TEST("analyze.observe.an_image_that_is_not_a_flat_32_bit_module_is_not_run", [] {
    ImageSpec spec;
    spec.kind = ImageKind::Ne;
    spec.bits = 16;
    spec.flat = false;
    Image ne(spec);
    CB_CHECK(ob::run(ne, {}, {}, ob::Setting{}).empty());
    CB_CHECK(ob::candidates(ne, {}, {}, 5).empty());
});

// ---- the analysis -----------------------------------------------------------------------------------------------------------------

CB_TEST("analyze.observe.the_analysis_is_the_pythons_report", [] {
    for (const char* name : {"crtdll.dll", "lz32.dll"}) {
        Module m(name);
        const Json& g = golden().at(name);
        const ob::Seen seen = ob::run(m.image, m.facts, m.found, m.setting(golden().at("argc"), g.at("limit").get<std::size_t>()));
        const ob::Report report = ob::analyse(m.image, m.facts, m.found, seen);
        const Json& want = g.at("report");
        // the counts
        const ob::Counts& c = report.counts;
        const std::map<std::string, std::int64_t> got = {
            {"functions_run", c.functions_run}, {"returned", c.returned}, {"faulted", c.faulted}, {"cut_off", c.cut_off},
            {"with_pointer_arguments", c.with_pointer_arguments}, {"fields_observed", c.fields_observed},
            {"nested_pointers_followed", c.nested_pointers_followed}, {"static_fields_confirmed", c.static_fields_confirmed},
            {"fields_only_seen_running", c.fields_only_seen_running},
            {"structures_with_a_field_confirmed", c.structures_with_a_field_confirmed},
            {"indirect_calls_resolved", c.indirect_calls_resolved}, {"function_starts_found", c.function_starts_found},
            {"fields_passed_to_system_calls", c.fields_passed_to_system_calls},
            {"removed_bytes_agree_with_static", c.removed_bytes_agree_with_static}, {"removed_bytes_differ", c.removed_bytes_differ},
            {"library_idioms", c.library_idioms}};
        expect(want.at("counts").size() == got.size(), std::string(name) + ": the number of counts");
        for (const auto& [k, v] : got) expect(want.at("counts").value(k, std::int64_t{-1}) == v, std::format("{}: count {} = {}", name, k, v));
        // the text
        expect(report.text == want.at("text").get<std::vector<std::string>>(), std::string(name) + ": the report's text");
        // the facts, as the documents the fact table keeps
        const auto rows = ob::fact_rows(report, ImageKind::Pe);
        std::size_t structure_facts = 0, observed_facts = 0;
        for (const Json& w : want.at("facts")) {
            if (w.at("kind").get<std::string>() == "observed") {
                ++observed_facts;
                const std::string key = std::format("{:x}", w.at("addr").get<std::int64_t>());
                const auto row = std::find_if(rows.begin(), rows.end(), [&](const auto& r) { return r.kind == "observed" && r.addr == key; });
                expect(row != rows.end(), std::string(name) + ": a row for " + key);
                if (row != rows.end()) expect(canon(row->value) == canon(w), std::format("{}: the fact of {}\n  got  {}\n  want {}", name, key, canon(row->value), canon(w)));
            } else {
                ++structure_facts;
                const auto row = std::find_if(rows.begin(), rows.end(), [&](const auto& r) {
                    return r.kind == "structure_observed" && r.value.get("name") && *r.value.get("name")->text() == w.at("name").get<std::string>();
                });
                expect(row != rows.end(), std::string(name) + ": a row for " + w.at("name").get<std::string>());
                if (row == rows.end()) continue;
                // (the offsets are compared as sets: DIVERGES, see the top of the file)
                for (const char* list : {"fields_confirmed", "fields_added"}) {
                    std::set<std::string> a, b;
                    for (const Value& x : *row->value.get(list)->list()) a.insert(*x.text());
                    for (const Json& x : w.at(list)) b.insert(x.get<std::string>());
                    expect(a == b, std::format("{}: {} of {}", name, list, w.at("name").get<std::string>()));
                }
                expect(*row->value.get("fields_not_reached")->integer() == w.at("fields_not_reached").get<std::int64_t>(), std::string(name) + ": fields_not_reached");
                expect(row->value.get("addr") != nullptr && row->value.get("addr")->is_null(), std::string(name) + ": a structure fact has no address");
            }
        }
        expect(report.observed.size() == observed_facts && report.structures.size() == structure_facts, std::string(name) + ": the number of facts");
        expect(rows.size() == observed_facts + structure_facts, std::string(name) + ": the number of rows");
        if (std::string(name) == "crtdll.dll") CB_CHECK(structure_facts > 0);
    }
});

CB_TEST("analyze.observe.the_report_is_made_of_hand_made_observations", [] {
    Module m("lz32.dll");
    auto& image = m.image;
    const Va f = image.functions().front();
    ob::Observation o;
    o.exit = ob::Exit::Returned;
    o.fields[1][8] = {4, 2, 1};
    o.nested[{1, 8}][0] = {1, 1, 0};
    o.passes.push_back({"KERNEL32!CloseHandle", 0, 1, 8, ob::Pass::How::Value});
    o.removed = 8;
    o.returns = ob::Returns{ob::Returns::Kind::Field, 1, 8};
    o.idiom = ob::Idiom::Memcpy;
    ob::Seen seen;
    seen[f] = o;
    seen[f + 1] = std::nullopt;   // the pool lost it: it is not in the report
    const ob::Report report = ob::analyse(image, {}, {}, seen);
    CB_CHECK_EQ(report.counts.functions_run, 1);
    CB_CHECK_EQ(report.counts.returned, 1);
    CB_CHECK_EQ(report.counts.library_idioms, 1);
    CB_CHECK_EQ(report.counts.fields_observed, 1);
    CB_CHECK_EQ(report.counts.fields_passed_to_system_calls, 1);
    CB_CHECK_EQ(report.observed.size(), std::size_t{1});
    CB_CHECK_EQ(report.text.size(), std::size_t{1});
    CB_CHECK_EQ(report.text[0], "behaves like memcpy: " + image.label(f));
    const Value v = ob::to_value(report.observed[0]);
    CB_CHECK_EQ(canon(v), std::format("{{addr:{},behaves_like:\"memcpy\",exit:\"ret\",fields:{{argument 1:{{+8:[4,2,1,],}},}},kind:\"observed\","
                                      "nested:{{argument 1 +8:{{+0:[1,1,0,],}},}},passes:[\"KERNEL32!CloseHandle parameter 0 gets the value at argument 1 +8\",],"
                                      "removed:8,returns:[\"field\",1,8,],}}", f));
    // the rows: one `observed` row addressed by the function
    const auto rows = ob::fact_rows(report, ImageKind::Pe);
    CB_CHECK_EQ(rows.size(), std::size_t{1});
    CB_CHECK_EQ(rows[0].kind, "observed");
    CB_CHECK_EQ(rows[0].addr, std::format("{:x}", f));
    // an empty seen is an empty report
    const ob::Report none = ob::analyse(image, {}, {}, {});
    CB_CHECK(none.observed.empty() && none.structures.empty() && none.text.empty());
    CB_CHECK_EQ(none.counts.functions_run, 0);
});
