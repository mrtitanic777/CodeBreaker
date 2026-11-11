// Tests of cb/port/old_c_passes (the data passes of the port of pre-ANSI C) and cb/port/old_c_scan.
//
// The golden data in testdata/old_c/<case>/ is what the original Python (codebreaker.port.pre_ansi.Port) leaves on the
// same program: each pass alone and the chain Port.modernize runs, as the digests of the files each changed, the note
// lists, aliases, extra declarations and declaration rules (testdata/old_c/make_cases.py makes it, run with
// PYTHONHASHSEED=0: the port's own passes iterate sets of names). Two real trees (the MS C 3.00 compiler's own sources,
// ports msc-3.00-p0 and msc-3.00-p3) fire most passes; the hand-written `syn-*` cases fire the ones they do not
// (array_bounds, fixed_allocators) and the edge shapes. The 16 trees of the ports folder that have a modern/ folder were
// run through the same comparison while this was written (CB_P2_EXTRA names the folder make_cases.py wrote them to).
//
// DIVERGES:
//   * A constant with a leading zero (`010`) is octal, as in C. The Python's int(x, 0) raised ValueError and ended the whole
//     port; fixed there too (pre_ansi.c_int, test_octal_constant_in_a_pass). A constant that does not fit 64 bits is not
//     edited (the Python's integers had no end).
//   * A table's rows end at its own closing brace. The Python's ended at the next `};` of the file (the file's last byte when
//     there was none), so a table written `}` and `;` apart took the rows of what came after it; fixed there too
//     (pre_ansi.table_end, test_size_table_ends_at_its_own_brace). A table that never closes has no rows.
//   * `\w \d \s` are ASCII (the Python's matched Unicode letters in the latin-1 text, which carries none in an identifier).
//   * The pascal names a file calls in other letters are visited sorted (their notes); the Python's order was its string
//     hash's.
//   * add_missing_includes puts the headers at the top of a file that has no #include (Port.modernize cannot make one: the
//     preprocessor always leaves port.h in every file).
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <functional>
#include <cstdlib>
#include <format>
#include <fstream>
#include <list>

#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/text.hpp"
#include "cb/core/testing.hpp"
#include "cb/port/old_c_passes.hpp"
#include "cb/port/old_c_scan.hpp"

using namespace cb;
using namespace cb::port::pre_ansi;

namespace {

std::string fnv(std::string_view s) {
    std::uint64_t h = 0xcbf29ce484222325ull;
    for (const char c : s) h = (h ^ static_cast<std::uint8_t>(c)) * 0x100000001b3ull;
    return std::format("{:016x}", h);
}

std::string read_text(const std::filesystem::path& p) {
    std::ifstream in(p, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(in), {});
}

// One program of the golden data: what the Port held when the passes started, and what each pass left.
struct Case {
    std::string name;
    Files originals, initial;
    Json manifest, expected;
    unsigned pointer_bytes = 2;
};

Case load_case(const std::string& name, const std::filesystem::path& root = testing::data_path("port/testdata/old_c")) {
    const std::filesystem::path dir = root / name;
    Case c;
    c.name = name;
    c.manifest = json::load(dir / "manifest.json");
    c.expected = json::load(dir / "expected.json");
    c.pointer_bytes = c.manifest.at("pointer_bits").get<unsigned>() / 8;
    for (const auto& entry : std::filesystem::directory_iterator(dir / "originals"))
        c.originals.insert_or_assign(entry.path().filename().string(), read_text(entry.path()));
    // (the files in the manifest's order: the changed ones from files/, the rest as the originals)
    for (const Json& f : c.manifest.at("files")) {
        const std::string n = f.get<std::string>();
        const std::filesystem::path changed = dir / "files" / n;
        c.initial.insert_or_assign(n, std::filesystem::exists(changed) ? read_text(changed) : c.originals.at(n));
    }
    return c;
}

struct Pass {
    const char* name;
    void (*run)(ProgramState&);
};
// In the order Port.modernize runs them.
const Pass kChain[] = {{"word_copies", first_word_copies}, {"shared_blocks", shared_blocks}, {"byte_places", byte_places},
                       {"local_copies", local_copies},     {"pool_blocks", pool_blocks},     {"size_tables", size_tables},
                       {"numbered_tables", numbered_tables}, {"array_bounds", array_bounds}, {"word_stacks", word_stacks},
                       {"merge_tables", merge_tables},     {"fixed_allocators", fixed_allocators}, {"agree_globals", agree_globals}};
const Pass kEach[] = {{"agree_globals", agree_globals},     {"fixed_allocators", fixed_allocators}, {"array_bounds", array_bounds},
                      {"word_stacks", word_stacks},         {"merge_tables", merge_tables},         {"numbered_tables", numbered_tables},
                      {"size_tables", size_tables},         {"pool_blocks", pool_blocks},           {"local_copies", local_copies},
                      {"byte_places", byte_places},         {"shared_blocks", shared_blocks},       {"word_copies", first_word_copies}};

void start(const Case& c, ProgramState& st) {
    st.files = c.initial;
    st.originals = c.originals;
    st.pointer_bytes = c.pointer_bytes;
    for (const Json& kv : c.manifest.at("ds")) st.ds.insert_or_assign(kv.at(0).get<std::string>(), kv.at(1).get<std::uint64_t>());
    for (const auto& [name, size] : c.manifest.at("bss").items()) st.bss[name] = size.get<std::uint64_t>();
    for (const Json& n : c.manifest.at("notes"))
        for (const Json& place : n.at(1)) note(st.notes, n.at(0).get<std::string>(), place.get<std::string>());
    for (const Json& a : c.manifest.at("aliases"))
        st.aliases.insert_or_assign(a.at(0).get<std::string>(), Alias{a.at(1).get<std::string>(), a.at(2).get<std::string>()});
    for (const auto& [f, decls] : c.manifest.at("extra").items())
        for (const Json& d : decls) st.extra_decls[f].insert(d.get<std::string>());
    for (const Json& r : c.manifest.at("rules"))
        st.decl_rules.emplace_back(r.at(0).get<std::string>(), text::replace_all(r.at(1).get<std::string>(), "\1", "${1}"));
}

std::vector<std::string> strings_of(const Json& j) {
    std::vector<std::string> out;
    for (const Json& x : j) out.push_back(x.get<std::string>());
    return out;
}

// Compares the state with the golden record of one tag; failures are reported against the tag.
void check_record(const ProgramState& st, const Json& rec, const Files& before, const std::string& tag) {
    std::map<std::string, std::string> changed;
    for (const auto& [n, t] : st.files) {
        const std::string* was = before.find(n);
        if (!was || *was != t) changed[n] = fnv(t);
    }
    std::map<std::string, std::string> want;
    for (const auto& [n, h] : rec.at("changed").items()) want[n] = h.get<std::string>();
    if (changed != want) {
        std::string differing;
        for (const auto& [n, h] : changed)
            if (!want.contains(n) || want[n] != h) differing += " " + n;
        for (const auto& [n, h] : want)
            if (!changed.contains(n)) differing += " -" + n;
        CB_CHECK_EQ(differing, std::string());
        if (const auto dump = fs::get_env("CB_P2_DUMP")) {   // (the actual files, to compare with the Python's)
            for (const auto& [n, t] : st.files) {
                const std::filesystem::path p = std::filesystem::path(*dump) / tag / n;
                std::filesystem::create_directories(p.parent_path());
                std::ofstream(p, std::ios::binary) << t;
            }
        }
    }
    const PassNotes& l = st.lists;
    const std::pair<const char*, const std::vector<std::string>*> lists[] = {
        {"agree_notes", &l.agreed},           {"long_notes", &l.longs},          {"alloc_notes", &l.allocators},
        {"table_notes", &l.tables},           {"notes_word_copies", &l.word_copies}, {"notes_shared", &l.shared_blocks},
        {"notes_byte_places", &l.byte_places}, {"notes_local_copies", &l.local_copies}, {"notes_pools", &l.pools},
        {"notes_size_tables", &l.size_tables}};
    for (const auto& [key, got] : lists) CB_CHECK_EQ(text::join(*got, "\n"), text::join(strings_of(rec.at("lists").at(key)), "\n"));

    std::vector<std::string> notes, want_notes;
    for (const auto& [why, places] : st.notes) notes.push_back(why + " => " + text::join(places, " | "));
    for (const Json& n : rec.at("notes")) want_notes.push_back(n.at(0).get<std::string>() + " => " + text::join(strings_of(n.at(1)), " | "));
    CB_CHECK_EQ(text::join(notes, "\n"), text::join(want_notes, "\n"));

    std::vector<std::string> aliases, want_aliases;
    for (const auto& [name, a] : st.aliases) aliases.push_back(name + " " + a.where + " " + a.declaration);
    for (const Json& a : rec.at("aliases")) want_aliases.push_back(a.at(0).get<std::string>() + " " + a.at(1).get<std::string>() + " " + a.at(2).get<std::string>());
    CB_CHECK_EQ(text::join(aliases, "\n"), text::join(want_aliases, "\n"));

    std::vector<std::string> extra, want_extra;
    for (const auto& [f, ds] : st.extra_decls) extra.push_back(f + ": " + text::join(std::vector<std::string>(ds.begin(), ds.end()), " | "));
    for (const auto& [f, ds] : rec.at("extra").items()) want_extra.push_back(f + ": " + text::join(strings_of(ds), " | "));
    CB_CHECK_EQ(text::join(extra, "\n"), text::join(want_extra, "\n"));

    std::vector<std::string> rules, want_rules;
    for (const DeclRule& r : st.decl_rules) rules.push_back(r.pattern() + " -> " + r.replacement());
    for (const Json& r : rec.at("rules")) {
        std::string repl = text::replace_all(r.at(1).get<std::string>(), "\\1", "${1}");
        want_rules.push_back(r.at(0).get<std::string>() + " -> " + repl);
    }
    CB_CHECK_EQ(text::join(rules, "\n"), text::join(want_rules, "\n"));

    std::vector<std::string> bss, want_bss;
    for (const auto& [name, size] : st.bss) bss.push_back(std::format("{}={}", name, size));
    for (const auto& [name, size] : rec.at("bss").items()) want_bss.push_back(std::format("{}={}", name, size.get<std::uint64_t>()));
    CB_CHECK_EQ(text::join(bss, ","), text::join(want_bss, ","));
}

const Json& record_of(const Case& c, const std::string& tag) {
    for (const Json& r : c.expected)
        if (r.at("tag").get<std::string>() == tag) return r;
    throw InternalError("no golden record " + tag);
}

// (CB_P2_TIME set: the time the passes took on each case, to stderr -- the speed record of docs/TYPED_CPP_SPEED.md)
double timed(const std::function<void()>& f) {
    const auto start = std::chrono::steady_clock::now();
    f();
    return std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
}

void run_case(const std::string& name, const std::filesystem::path& root = testing::data_path("port/testdata/old_c")) {
    const Case c = load_case(name, root);
    double pass_seconds = 0;
    std::list<fs::ScopedEnv> settings;   // (the rules a case turns off)
    if (c.manifest.contains("env"))
        for (const auto& [key, value] : c.manifest.at("env").items()) settings.emplace_back(key, value.get<std::string>());
    for (const Pass& pass : kEach) {   // each pass alone, on a program as the Port holds it when they start
        OriginalProgram original(c.originals, c.pointer_bytes);
        ProgramState st(original);
        start(c, st);
        pass_seconds += timed([&] { pass.run(st); });
        check_record(st, record_of(c, name + "." + pass.name), c.initial, name + "." + pass.name);
    }
    OriginalProgram original(c.originals, c.pointer_bytes);   // and the chain, each pass on what the one before left
    ProgramState st(original);
    start(c, st);
    for (const Pass& pass : kChain) {
        pass_seconds += timed([&] { pass.run(st); });
        check_record(st, record_of(c, name + ".chain." + pass.name), c.initial, name + ".chain." + pass.name);
    }
    if (fs::get_env("CB_P2_TIME")) std::fprintf(stderr, "old_c_passes %s: %.3f s in the passes\n", name.c_str(), pass_seconds);
}

}  // namespace

CB_TEST("port.old_c_passes.msc_3_00_p3_each_pass_and_the_chain", [] { run_case("msc-3.00-p3"); });
CB_TEST("port.old_c_passes.msc_3_00_p0_each_pass_and_the_chain", [] { run_case("msc-3.00-p0"); });
CB_TEST("port.old_c_passes.agree_globals_cases", [] { run_case("syn-agree"); });
CB_TEST("port.old_c_passes.array_bounds_case", [] { run_case("syn-bounds"); });
CB_TEST("port.old_c_passes.fixed_allocators_case", [] { run_case("syn-allocs"); });
CB_TEST("port.old_c_passes.merge_tables_case", [] { run_case("syn-tables"); });
CB_TEST("port.old_c_passes.word_stacks_case", [] { run_case("syn-words"); });
CB_TEST("port.old_c_passes.word_stacks_in_declarations_with_other_names", [] { run_case("syn-words2"); });
CB_TEST("port.old_c_passes.tables_with_strings_and_the_places_they_are_read_at", [] { run_case("syn-tables2"); });
CB_TEST("port.old_c_passes.a_size_table_ends_at_its_own_brace", [] { run_case("syn-tables3"); });
CB_TEST("port.old_c_passes.a_table_that_never_closes_has_no_rows", [] { run_case("syn-tables3b"); });
CB_TEST("port.old_c_passes.the_agree_rules_can_be_turned_off", [] { run_case("syn-agree-rules-off"); });

// (more trees, made by make_cases.py with P2_EXTRA, to try the passes on: CB_P2_EXTRA is their folder)
CB_TEST("port.old_c_passes.more_trees_in_the_folder_the_environment_names", [] {
    const auto extra = fs::get_env("CB_P2_EXTRA");
    if (!extra) return;
    for (const auto& entry : std::filesystem::directory_iterator(*extra)) run_case(entry.path().filename().string(), *extra);
});
