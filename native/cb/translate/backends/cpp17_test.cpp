// Tests of the C++17 back end's client (cpp17), on real programs: the units the Java front end lowers (the cpp03 corpus's and this
// module's own, as L2) written as a whole program, and a module of the Python front end (the semantics snippet) written as header, source and function spans. The expected text is the checked-in
// goldens (testdata/cpp17); the back end itself is the L2 core library
// (native/l2, cbl2.dll), so what these check is what this module adds: the units as documents, the modern forms lowered first,
// the language a unit without one gets, the folders, the errors. They need the DLL (cmake --build native/l2/build --target
// cbl2, or CB_L2_DLL naming it); without it they say so and pass.
//
// The decline is the library's Cpp17Declined (message as the library words it); the
// folders are found from the checkout the program belongs to, not from the source file's place.
#include "cb/translate/backends/cpp17.hpp"

#include <iostream>
#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/translate/l2.hpp"

namespace {

namespace c17 = cb::translate::backends::cpp17;
namespace nl = cb::translate::native_l2;
namespace ir = cb::translate::ir;
using cb::Json;

const nl::Library* library() {
    static const std::optional<nl::Library> lib = []() -> std::optional<nl::Library> {
        std::filesystem::path dll = cb::testing::data_path("../l2/build/bin/cbl2.dll");
        if (const auto env = cb::fs::get_env("CB_L2_DLL")) dll = cb::fs::from_text(*env);
        std::error_code ec;
        if (!std::filesystem::is_regular_file(dll, ec)) {
            std::cerr << "cpp17 tests: " << dll.string() << " is not built; skipped\n";
            return std::nullopt;
        }
        return nl::Library::load(dll);
    }();
    return lib ? &*lib : nullptr;
}

std::filesystem::path data(const std::string& relative) { return cb::testing::data_path("translate/testdata/" + relative); }

void fail(const std::string& message) {
    cb::testing::count_check();
    cb::testing::record_failure(__FILE__, __LINE__, message);
}

std::vector<ir::Node*> corpus_units(cb::Arena& arena, const std::string& name) {
    std::vector<ir::Node*> units;
    for (std::size_t i = 0;; ++i) {
        const std::filesystem::path file = data("cpp03/" + name + "/units/" + std::to_string(i) + ".json");
        if (!std::filesystem::exists(file)) break;
        units.push_back(cb::translate::l2::from_json(arena, cb::json::load(file)));
    }
    CB_CHECK(!units.empty());
    return units;
}

std::vector<ir::Node*> own_units(cb::Arena& arena, const std::string& name) {
    std::vector<ir::Node*> units;
    for (const Json& doc : cb::json::load(data("c_java/" + name + "/units.l2.json"))) units.push_back(cb::translate::l2::from_json(arena, doc));
    return units;
}

void check_text(const std::string& got, const std::string& golden_file) {
    const std::string want = cb::fs::read_text(data(golden_file));
    if (got == want) return;
    const std::vector<std::string_view> a = cb::text::lines(got), b = cb::text::lines(want);
    std::size_t i = 0;
    while (i < a.size() && i < b.size() && a[i] == b[i]) ++i;
    fail(std::format("{}: line {} differs\n  got:  {}\n  want: {}", golden_file, i + 1, i < a.size() ? std::string(a[i]) : "(end)",
                     i < b.size() ? std::string(b[i]) : "(end)"));
}

void check_corpus(const std::string& name) {
    const nl::Library* lib = library();
    if (lib == nullptr) return;
    cb::Arena arena;
    std::vector<ir::Node*> units = corpus_units(arena, name);
    check_text(c17::program(*lib, arena, units), "cpp17/" + name + ".golden.cpp");
}

void check_declined(const std::string& name) {
    const nl::Library* lib = library();
    if (lib == nullptr) return;
    cb::Arena arena;
    std::vector<ir::Node*> units = corpus_units(arena, name);
    const std::string want(cb::text::trim(cb::fs::read_text(data("cpp17/" + name + ".decline.txt"))));
    try {
        (void)c17::program(*lib, arena, units);
    } catch (const c17::Decline& d) {
        CB_CHECK_EQ(std::string(d.what()), want);
        return;
    }
    fail(name + " was written, but the golden declines it");
}

}  // namespace

CB_TEST("translate.cpp17.corpus.bank", [] { check_corpus("bank"); });
CB_TEST("translate.cpp17.corpus.calc", [] { check_corpus("calc"); });
CB_TEST("translate.cpp17.corpus.generics", [] { check_corpus("generics"); });
CB_TEST("translate.cpp17.corpus.sample", [] { check_corpus("sample"); });
CB_TEST("translate.cpp17.corpus.shapes", [] { check_corpus("shapes"); });
CB_TEST("translate.cpp17.corpus.streams", [] { check_corpus("streams"); });   // switch expressions and for-each are kept for C++17
CB_TEST("translate.cpp17.corpus.t03", [] { check_corpus("t03"); });
CB_TEST("translate.cpp17.corpus.t17", [] { check_corpus("t17"); });
CB_TEST("translate.cpp17.corpus.textstats", [] { check_corpus("textstats"); });

CB_TEST("translate.cpp17.own.zoo", [] {
    const nl::Library* lib = library();
    if (lib == nullptr) return;
    cb::Arena arena;
    std::vector<ir::Node*> units = own_units(arena, "zoo");
    check_text(c17::program(*lib, arena, units, std::string("Main")), "cpp17/zoo.golden.cpp");
});
CB_TEST("translate.cpp17.own.mix", [] {
    const nl::Library* lib = library();
    if (lib == nullptr) return;
    cb::Arena arena;
    std::vector<ir::Node*> units = own_units(arena, "mix");
    check_text(c17::program(*lib, arena, units), "cpp17/mix.golden.cpp");
});
CB_TEST("translate.cpp17.own.names", [] {
    const nl::Library* lib = library();
    if (lib == nullptr) return;
    cb::Arena arena;
    std::vector<ir::Node*> units = own_units(arena, "names");
    check_text(c17::program(*lib, arena, units), "cpp17/names.golden.cpp");
});

// What the library does not write is declined, with its reason.
CB_TEST("translate.cpp17.declines.feat", [] { check_declined("feat"); });
CB_TEST("translate.cpp17.declines.minecraft", [] { check_declined("minecraft"); });
CB_TEST("translate.cpp17.declines.modernlib", [] { check_declined("modernlib"); });

// A module of the Python front end: header, source, and where each function's body is.
CB_TEST("translate.cpp17.write_unit", [] {
    const nl::Library* lib = library();
    if (lib == nullptr) return;
    cb::Arena arena;
    ir::Node* unit = cb::translate::l2::from_json(arena, cb::json::load(data("cpp17/semantics.unit.l2.json")));
    const c17::UnitText m = c17::write_unit(*lib, *unit);
    check_text(m.header, "cpp17/semantics.golden.hpp");
    check_text(m.source, "cpp17/semantics.golden.cpp");
    const Json spans = cb::json::load(data("cpp17/semantics.spans.json"));
    CB_CHECK_EQ(m.spans.size(), spans.size());
    for (const auto& [name, span] : spans.items()) {
        const auto* got = m.spans.find(name);
        CB_CHECK(got != nullptr);
        if (got == nullptr) continue;
        CB_CHECK_EQ(got->first, span[0].get<std::int64_t>());
        CB_CHECK_EQ(got->second, span[1].get<std::int64_t>());
    }
    // a unit that does not say its language is the Python front end's: the same text with the language said
    Json doc = cb::translate::l2::to_json(*unit);
    CB_CHECK(!doc.contains("lang") || doc["lang"].is_null() || doc["lang"] == "python");
});

CB_TEST("translate.cpp17.paths", [] {
    CB_CHECK_EQ(c17::header_path("pkg.mod"), "pkg/mod.hpp");
    CB_CHECK_EQ(c17::source_path("pkg.sub.mod"), "pkg/sub/mod.cpp");
    CB_CHECK_EQ(c17::header_path("plain"), "plain.hpp");
    CB_CHECK_EQ(std::string(c17::kFlags[0]), "-std=c++17");
});
