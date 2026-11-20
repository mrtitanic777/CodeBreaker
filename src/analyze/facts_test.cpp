#include "analyze/facts.hpp"

#include <string>

#include "analyze/image.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"

namespace {

namespace facts = cb::analyze::facts;
using cb::analyze::ImageKind;
using cb::knowledge::Value;

Value convention(const std::string& name, std::int64_t args) {
    return Value(Value::Map{{"name", Value(name)}, {"args", Value(args)}, {"convention", Value("stdcall")}});
}

CB_TEST("analyze.facts.keys_are_hex_for_flat_modules_and_segment_offset_for_16_bit", [] {
    CB_CHECK_EQ(facts::key(ImageKind::Pe, 0x401000), std::string("401000"));
    CB_CHECK_EQ(facts::key(ImageKind::Le, 0x01001234), std::string("1001234"));
    CB_CHECK_EQ(facts::key(ImageKind::Pe, std::nullopt), std::string());
    // segment 1 is selector 0x1000, segment 3 is 0x1010
    CB_CHECK_EQ(facts::key(ImageKind::Ne, cb::analyze::image::ne_address(1, 0x2a)), std::string("1:002a"));
    CB_CHECK_EQ(facts::key(ImageKind::Ne, cb::analyze::image::ne_address(3, 0xbeef)), std::string("3:beef"));
    CB_CHECK_EQ(facts::key(ImageKind::Ne, std::nullopt), std::string());
});

CB_TEST("analyze.facts.put_replaces_one_analysis_of_one_module_and_keeps_the_rest", [] {
    cb::fs::ScopedTempDir dir("cb-facts");
    auto store = facts::FactStore::open(dir.path() / "sub" / "functions.sqlite");
    const std::vector<facts::Row> first = {{"401000", "convention", convention("a", 2)},
                                           {"401020", "convention", convention("b", 0)},
                                           {"", "thunk_layer", Value(Value::Map{{"imports", Value(Value::List{Value("QT_THUNK")})}})}};
    store.put("m1", "conventions", first);
    store.put("m1", "globals", std::vector<facts::Row>{{"", "global", Value(std::int64_t{7})}});
    store.put("m2", "conventions", std::vector<facts::Row>{{"1", "convention", convention("z", 1)}});
    CB_CHECK_EQ(store.get("m1").size(), std::size_t{4});

    store.put("m1", "conventions", std::vector<facts::Row>{{"401000", "convention", convention("a", 4)}});
    const auto got = store.get("m1", {.analysis = "conventions"});
    CB_CHECK_EQ(got.size(), std::size_t{1});
    CB_CHECK_EQ(facts::to_text(got[0].value), facts::to_text(convention("a", 4)));
    CB_CHECK_EQ(store.get("m1", {.analysis = "globals"}).size(), std::size_t{1});   // untouched
    CB_CHECK_EQ(store.get("m2").size(), std::size_t{1});                           // another module: untouched
    CB_CHECK(store.get("nobody").empty());
});

CB_TEST("analyze.facts.get_narrows_by_analysis_address_and_kind_and_keeps_the_order_put", [] {
    cb::fs::ScopedTempDir dir("cb-facts");
    auto store = facts::FactStore::open(dir.path() / "f.sqlite");
    store.put("m", "conventions",
              std::vector<facts::Row>{{"b", "convention", Value(std::int64_t{1})}, {"a", "convention", Value(std::int64_t{2})},
                                      {"a", "shape", Value("leaf")}});
    store.put("m", "structures", std::vector<facts::Row>{{"a", "structure", Value(Value::List{})}});
    const auto all = store.get("m");
    CB_CHECK_EQ(all.size(), std::size_t{4});
    CB_CHECK_EQ(all[0].addr, std::string("b"));   // put order, not address order
    CB_CHECK_EQ(all[3].analysis, std::string("structures"));
    CB_CHECK_EQ(store.get("m", {.addr = "a"}).size(), std::size_t{3});
    CB_CHECK_EQ(store.get("m", {.kind = "convention"}).size(), std::size_t{2});
    CB_CHECK_EQ(store.get("m", {.analysis = "conventions", .addr = "a", .kind = "shape"}).size(), std::size_t{1});
    CB_CHECK(store.get("m", {.analysis = "conventions", .addr = "zz"}).empty());
});

CB_TEST("analyze.facts.tally_counts_kinds_per_analysis", [] {
    cb::fs::ScopedTempDir dir("cb-facts");
    auto store = facts::FactStore::open(dir.path() / "f.sqlite");
    store.put("m", "conventions",
              std::vector<facts::Row>{{"1", "convention", Value()}, {"2", "convention", Value()}, {"", "duplicates", Value()}});
    store.put("m", "globals", std::vector<facts::Row>{{"", "global", Value()}});
    const facts::Tally tally = store.tally("m");
    CB_CHECK_EQ(tally.size(), std::size_t{2});
    CB_CHECK_EQ(tally.at("conventions").at("convention"), 2);
    CB_CHECK_EQ(tally.at("conventions").at("duplicates"), 1);
    CB_CHECK_EQ(tally.at("globals").at("global"), 1);
    CB_CHECK(store.tally("other").empty());
});

CB_TEST("analyze.facts.values_of_every_kind_round_trip_through_the_table", [] {
    cb::fs::ScopedTempDir dir("cb-facts");
    auto store = facts::FactStore::open(dir.path() / "f.sqlite");
    const Value nested(Value::Map{{"null", Value()},
                                  {"yes", Value(true)},
                                  {"big", Value(std::int64_t{-0x7fffffffffffffffLL})},
                                  {"ratio", Value(0.25)},
                                  {"text", Value("caf\xC3\xA9 \"quoted\" \\ \n")},
                                  {"list", Value(Value::List{Value(1), Value("two"), Value(Value::List{})})},
                                  {"map", Value(Value::Map{{"z", Value(1)}, {"a", Value(2)}})}});
    store.put("m", "x", std::vector<facts::Row>{{"1", "k", nested}});
    const auto got = store.get("m");
    CB_CHECK_EQ(got.size(), std::size_t{1});
    CB_CHECK_EQ(facts::to_text(got[0].value), facts::to_text(nested));
    // an object comes back with its keys sorted ("a" before "z"), whatever order it was put in
    const auto* map = got[0].value.map()->find("map")->map();
    CB_CHECK_EQ(map->entries().front().first, std::string("a"));
});

CB_TEST("analyze.facts.stored_text_has_sorted_keys_and_the_usual_separators", [] {
    const Value v(Value::Map{{"b", Value(1)}, {"a", Value(Value::List{Value(2), Value(3)})}});
    CB_CHECK_EQ(facts::to_text(v), std::string(R"({"a": [2, 3], "b": 1})"));
    CB_CHECK_EQ(facts::to_text(Value()), std::string("null"));
    // the two orders of one object are one text
    const Value w(Value::Map{{"a", Value(Value::List{Value(2), Value(3)})}, {"b", Value(1)}});
    CB_CHECK_EQ(facts::to_text(v), facts::to_text(w));
});

CB_TEST("analyze.facts.text_that_is_not_json_is_a_parse_error", [] {
    CB_CHECK_THROWS_AS(cb::ParseError, facts::from_text("{\"a\": "));
    CB_CHECK_THROWS_AS(cb::ParseError, facts::from_text(""));
    CB_CHECK(facts::from_text("[1, 2]") == Value(Value::List{Value(1), Value(2)}));
});

CB_TEST("analyze.facts.facts_survive_reopening_the_file", [] {
    cb::fs::ScopedTempDir dir("cb-facts");
    const auto file = dir.path() / "f.sqlite";
    {
        auto store = facts::FactStore::open(file);
        store.put("m", "a", std::vector<facts::Row>{{"1", "k", Value("v")}});
    }
    auto again = facts::FactStore::open(file);   // the table already exists: opening is harmless
    CB_CHECK_EQ(again.get("m").size(), std::size_t{1});
    CB_CHECK(again.path() == file);
});

CB_TEST("analyze.facts.a_put_that_cannot_complete_leaves_the_old_facts", [] {
    cb::fs::ScopedTempDir dir("cb-facts");
    auto store = facts::FactStore::open(dir.path() / "f.sqlite");
    store.put("m", "a", std::vector<facts::Row>{{"1", "k", Value("old")}});
    // a row whose address holds an embedded NUL still stores; make the failure the database's: drop the table
    {
        auto other = cb::base::sqlite::Database::open(dir.path() / "f.sqlite");
        other.exec("DROP TABLE fact");
    }
    CB_CHECK_THROWS_AS(cb::IoError, store.put("m", "a", std::vector<facts::Row>{{"1", "k", Value("new")}}));
});

CB_TEST("analyze.facts.default_path_is_the_environment_first_then_the_state_folder", [] {
    cb::fs::ScopedTempDir dir("cb-facts");
    const auto config = cb::config::Config::load(dir.path());
    {
        cb::fs::ScopedEnv env("CB_FUNCTIONS_DB", std::string_view("elsewhere.sqlite"));
        CB_CHECK(facts::default_path(config) == std::filesystem::path("elsewhere.sqlite"));
    }
    cb::fs::ScopedEnv none("CB_FUNCTIONS_DB", std::nullopt);
    CB_CHECK(facts::default_path(config).filename() == std::filesystem::path("functions.sqlite"));
});

}  // namespace
