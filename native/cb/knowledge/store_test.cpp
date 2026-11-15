#include "cb/knowledge/store.hpp"

#include <thread>
#include <vector>

#include "cb/base/sqlite.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

// Behaviour pinned here:
//   * a value is a knowledge::Value (the one document model of the base); the base is a file of its own
//     (knowledge.v2.sqlite) and does not read an older one -- what it held is made again;
//   * the digests of the name store and of a whole kind are hashes of the values' encodings;
//   * a store is an object the caller owns.

using namespace cb;
using namespace cb::knowledge;

namespace {

std::filesystem::path checkout() { return std::filesystem::path(CB_SOURCE_DIR).parent_path(); }

struct TestStore {
    fs::ScopedTempDir dir{"cb-knowledge-store"};
    Store store{dir.path() / "db" / "knowledge.v2.sqlite", DataFolders::under(checkout())};
};

}  // namespace

CB_TEST("knowledge.store.put_get_has_forget", [] {
    TestStore t;
    CB_CHECK(!t.store.get("win32", "argument_bytes.code", "USER32.FindWindowA"));
    CB_CHECK(!t.store.has("win32", "argument_bytes.code", "USER32.FindWindowA"));
    t.store.put("win32", "argument_bytes.code", "USER32.FindWindowA", Value(8), "user32.dll's code");
    CB_CHECK(t.store.has("win32", "argument_bytes.code", "USER32.FindWindowA"));
    CB_CHECK_EQ(*t.store.get("win32", "argument_bytes.code", "USER32.FindWindowA")->integer(), 8);
    // the same key on another platform or kind is another piece
    CB_CHECK(!t.store.get("win16", "argument_bytes.code", "USER32.FindWindowA"));
    t.store.put("win32", "argument_bytes.code", "USER32.FindWindowA", Value(12), "again");   // replaces
    CB_CHECK_EQ(*t.store.get("win32", "argument_bytes.code", "USER32.FindWindowA")->integer(), 12);
    t.store.forget("win32", "argument_bytes.code", "USER32.FindWindowA");
    CB_CHECK(!t.store.has("win32", "argument_bytes.code", "USER32.FindWindowA"));
    t.store.forget("win32", "argument_bytes.code", "never there");   // forgetting nothing is not an error
});

CB_TEST("knowledge.store.every_kind_of_value_round_trips", [] {
    TestStore t;
    const Value structured(Value::Map{{"returns", Value("HWND")},
                                      {"parameters", Value(Value::List{Value(Value::List{Value("int"), Value("")}), Value(true)})},
                                      {"size", Value(-3)},
                                      {"ratio", Value(0.5)},
                                      {"none", Value()}});
    t.store.put("dos", "interrupt", "21", structured, "test", Origin::Given);
    const std::optional<Value> back = t.store.get("dos", "interrupt", "21");
    CB_CHECK(back && *back == structured);
});

CB_TEST("knowledge.store.provenance_says_where_a_piece_came_from", [] {
    TestStore t;
    t.store.put("win16", "argument_bytes", "USER.171", Value(6), "USER.EXE's code", Origin::Learned);
    const auto p = t.store.provenance("win16", "argument_bytes", "USER.171");
    CB_CHECK(p.has_value());
    CB_CHECK_EQ(p->source, "USER.EXE's code");
    CB_CHECK_EQ(p->origin, "learned");
    CB_CHECK(p->made_at && *p->made_at > 1.6e9);
    CB_CHECK_EQ(p->hash, Value(6).digest());
    CB_CHECK(!t.store.provenance("win16", "argument_bytes", "nothing"));
    CB_CHECK(parse_origin("given") == Origin::Given);
    CB_CHECK(!parse_origin("invented"));
    CB_CHECK_EQ(origin_name(Origin::Derived), "derived");
});

CB_TEST("knowledge.store.table_keeps_the_order_made_then_the_order_learned", [] {
    TestStore t;
    int makes = 0;
    const auto make = [&] {
        ++makes;
        Rows r;
        r.insert_or_assign("zeta", Value(1));
        r.insert_or_assign("alpha", Value(2));
        return r;
    };
    const KindId kind{"win32", "prototype"};
    const Rows first = t.store.ensure(kind, "stamp-1", make, "the headers", Origin::Derived);
    t.store.put("win32", "prototype", "learned-late", Value(3), "by hand", Origin::Given);
    t.store.put("win32", "prototype", "learned-first", Value(4), "by hand", Origin::Given);
    std::vector<std::string> keys;
    for (const auto& [k, v] : t.store.table("win32", "prototype")) keys.push_back(k);
    CB_CHECK((keys == std::vector<std::string>{"zeta", "alpha", "learned-late", "learned-first"}));
    CB_CHECK_EQ(first.size(), std::size_t{2});
    CB_CHECK_EQ(makes, 1);
});

CB_TEST("knowledge.store.ensure_makes_a_kind_again_only_when_the_stamp_changes", [] {
    TestStore t;
    int makes = 0;
    const KindId kind{"win16", "prototype"};
    const auto make_v = [&](std::int64_t v) {
        return [&makes, v] {
            ++makes;
            Rows r;
            r.insert_or_assign("A", Value(v));
            return r;
        };
    };
    CB_CHECK_EQ(*t.store.ensure(kind, "s1", make_v(1), "src", Origin::Derived).find("A")->integer(), 1);
    CB_CHECK(t.store.stamp_of("win16", "prototype") == std::optional<std::string>("s1"));
    CB_CHECK_EQ(*t.store.ensure(kind, "s1", make_v(99), "src", Origin::Derived).find("A")->integer(), 1);   // not made again
    CB_CHECK_EQ(makes, 1);
    t.store.put("win16", "prototype", "kept", Value(7), "person", Origin::Given);
    t.store.put("win16", "prototype", "learned", Value(8), "step", Origin::Learned);
    const Rows again = t.store.ensure(kind, "s2", make_v(2), "src", Origin::Derived);
    CB_CHECK_EQ(makes, 2);
    CB_CHECK_EQ(*again.find("A")->integer(), 2);
    CB_CHECK(again.contains("kept") && again.contains("learned"));   // only authored and derived rows are replaced
    // a row an earlier make wrote and the new one does not is gone
    const Rows empty = t.store.ensure(kind, "s3", [&] { return Rows(); }, "src", Origin::Derived);
    CB_CHECK(!empty.contains("A"));
    CB_CHECK(empty.contains("kept"));
});

CB_TEST("knowledge.store.a_make_that_throws_leaves_the_kind_as_it_was", [] {
    TestStore t;
    const KindId kind{"dos", "interrupt"};
    (void)t.store.ensure(kind, "s1", [] { Rows r; r.insert_or_assign("k", Value(1)); return r; }, "src", Origin::Authored);
    CB_CHECK_THROWS_AS(cb::ParseError, (void)t.store.ensure(kind, "s2", []() -> Rows { throw cb::ParseError("x", 1, 1, "bad file"); }, "src", Origin::Authored));
    CB_CHECK(t.store.stamp_of("dos", "interrupt") == std::optional<std::string>("s1"));
    CB_CHECK(t.store.table("dos", "interrupt").contains("k"));
});

CB_TEST("knowledge.store.digests_follow_the_piece", [] {
    TestStore t;
    const Read piece{"win32", "argument_bytes.code", "A.f"};
    CB_CHECK_EQ(t.store.digest(piece), std::string(kAbsent));
    t.store.put("win32", "argument_bytes.code", "A.f", Value(4), "x");
    const std::string d1 = t.store.digest(piece);
    CB_CHECK_EQ(d1, Value(4).digest());
    t.store.put("win32", "argument_bytes.code", "A.f", Value(8), "x");
    CB_CHECK(t.store.digest(piece) != d1);
    // a whole kind: absent while empty, then a hash of every piece that moves with each
    const Read whole{"win32", "argument_bytes.code", std::string(kWholeKind)};
    const std::string w1 = t.store.digest(whole);
    t.store.put("win32", "argument_bytes.code", "A.g", Value(0), "x");
    CB_CHECK(t.store.digest(whole) != w1);
    CB_CHECK_EQ(t.store.digest({"win32", "nothing", std::string(kWholeKind)}), std::string(kAbsent));
});

CB_TEST("knowledge.store.digests_see_a_write_another_connection_made", [] {
    fs::ScopedTempDir dir("cb-knowledge-store");
    const auto file = dir.path() / "k.sqlite";
    Store a(file, DataFolders::under(checkout()));
    Store b(file, DataFolders::under(checkout()));
    const Read piece{"win16", "argument_bytes", "USER.1"};
    CB_CHECK_EQ(a.digest(piece), std::string(kAbsent));
    b.put("win16", "argument_bytes", "USER.1", Value(2), "b");
    CB_CHECK_EQ(a.digest(piece), Value(2).digest());   // `a` read the kind before; the file changed under it
    CB_CHECK_EQ(*a.get("win16", "argument_bytes", "USER.1")->integer(), 2);
});

CB_TEST("knowledge.store.bodies_and_names", [] {
    TestStore t;
    t.store.add_module("sha-a", "lz32.dll", "win32");
    t.store.add_module("sha-b", "other.dll", "win32");
    t.store.set_bodies("sha-a", std::vector<BodyRow>{{"1:0010", "body-1", std::string("LZStart"), std::string("export")},
                                                    {"1:0020", "body-2", std::nullopt, std::nullopt}});
    t.store.set_bodies("sha-b", std::vector<BodyRow>{{"1:0100", "body-2", std::string("helper"), std::string("export")}});
    CB_CHECK_EQ(t.store.bodies("sha-a").size(), std::size_t{2});
    // one module names the body: the others get the name
    const std::optional<BodyName> n = t.store.name("body-2");
    CB_CHECK(n && n->name == "helper" && n->source == "the same body in other.dll");
    // two modules that disagree: no name
    t.store.set_bodies("sha-a", std::vector<BodyRow>{{"1:0020", "body-2", std::string("other_name"), std::string("export")}});
    CB_CHECK(!t.store.name("body-2"));
    CB_CHECK(!t.store.name("body-unknown"));
    // a name given by hand wins, and holds in every module
    t.store.name_body("body-2", "Chosen", "named by hand");
    CB_CHECK(t.store.name("body-2") == std::optional<BodyName>(BodyName{"Chosen", "named by hand"}));
    CB_CHECK_EQ(t.store.modules_with_body("body-2"), std::size_t{2});
    CB_CHECK_EQ(t.store.modules_with_body("body-1"), std::size_t{0});   // sha-a no longer has it
    const show::SharedBodies s = t.store.shared_bodies();
    CB_CHECK_EQ(s.recorded, 2);
    CB_CHECK_EQ(s.distinct, 1);
    CB_CHECK_EQ(s.in_several_modules, 1);
});

CB_TEST("knowledge.store.names_from_bodies_prefers_what_was_given_by_hand", [] {
    TestStore t;
    t.store.add_module("m1", "one.dll", "win32");
    t.store.add_module("m2", "two.dll", "win32");
    t.store.set_bodies("m1", std::vector<BodyRow>{{"a", "d1", std::nullopt, std::nullopt},
                                                  {"b", "d2", std::nullopt, std::nullopt},
                                                  {"c", "d3", std::nullopt, std::nullopt}});
    t.store.set_bodies("m2", std::vector<BodyRow>{{"x", "d1", std::string("Alpha"), std::string("s")},
                                                  {"y", "d2", std::string("Beta"), std::string("s")},
                                                  {"z", "d2", std::string("Gamma"), std::string("s")}});
    t.store.name_body("d3", "ByHand", "me");
    const auto names = t.store.names_from_bodies("m1");
    CB_CHECK_EQ(names.size(), std::size_t{2});   // d2 is named two ways by m2: undecided
    CB_CHECK_EQ(names[0].first, "a");
    CB_CHECK_EQ(names[0].second.name, "Alpha");
    CB_CHECK_EQ(names[0].second.source, "the same body in two.dll");
    CB_CHECK_EQ(names[1].first, "c");
    CB_CHECK_EQ(names[1].second.source, "me");
    // the name store's digests: the answer's hash, absent when there is none
    CB_CHECK(t.store.digest({"bodies", "name", "d1"}) != std::string(kAbsent));
    CB_CHECK_EQ(t.store.digest({"bodies", "name", "d2"}), std::string(kAbsent));
    CB_CHECK(t.store.digest({"bodies", "names", "m1"}) != std::string(kAbsent));
    CB_CHECK_EQ(t.store.digest({"bodies", "names", "nobody"}), std::string(kAbsent));
    CB_CHECK_EQ(t.store.digest({"bodies", "else", "x"}), std::string(kAbsent));
});

CB_TEST("knowledge.store.carried_notes", [] {
    TestStore t;
    CB_CHECK(!t.store.carried("copy of x"));
    t.store.set_carried("copy of x", "12:3");
    CB_CHECK(t.store.carried("copy of x") == std::optional<std::string>("12:3"));
    t.store.set_carried("copy of x", "13:3");
    CB_CHECK(t.store.carried("copy of x") == std::optional<std::string>("13:3"));
});

CB_TEST("knowledge.store.a_reopened_file_has_what_was_written", [] {
    fs::ScopedTempDir dir("cb-knowledge-store");
    const auto file = dir.path() / "nested" / "k.sqlite";
    {
        Store s(file, DataFolders::under(checkout()));
        s.put("vxd", "device", "1", Value("VMM"), "me", Origin::Given);
        s.name_body("d", "N", "by hand");
    }
    Store s(file, DataFolders::under(checkout()));
    CB_CHECK_EQ(*s.get("vxd", "device", "1")->text(), "VMM");
    CB_CHECK(s.name("d").has_value());
    CB_CHECK_EQ(fs::to_text(s.file()), fs::to_text(file));
});

CB_TEST("knowledge.store.default_path_and_beside", [] {
    {
        fs::ScopedEnv env("CB_KNOWLEDGE_DB", std::nullopt);
        CB_CHECK_EQ(fs::to_text(Store::default_path(testing::rooted("state"))), testing::rooted("state/db/knowledge.v2.sqlite"));
    }
    {
        fs::ScopedEnv env("CB_KNOWLEDGE_DB", testing::rooted("elsewhere/k.sqlite"));
        CB_CHECK_EQ(fs::to_text(Store::default_path(testing::rooted("state"))), testing::rooted("elsewhere/k.sqlite"));
    }
    const auto beside = Store::beside(std::filesystem::path(testing::rooted("work/state/functions.sqlite")));
    CB_CHECK_EQ(fs::to_text(beside.lexically_normal().generic_string()), testing::rooted("work/state/db/knowledge.v2.sqlite"));
});

CB_TEST("knowledge.store.authored_kinds_load_from_the_checkouts_files", [] {
    TestStore t;
    const Rows api = t.store.authored_table({"win16", "api_model"});
    CB_CHECK_EQ(api.size(), std::size_t{14});
    CB_CHECK(api.contains("GLOBALLOCK"));
    // loaded once: the kind is in the base now, with where it came from
    CB_CHECK_EQ(t.store.table("win16", "api_model").size(), std::size_t{14});
    CB_CHECK(t.store.stamp_of("win16", "api_model").has_value());
    const auto p = t.store.about("win16", "api_model", "GLOBALLOCK");
    CB_CHECK(p && p->origin == "authored" && p->source == "data/knowledge/models/win16.json");
    // an unknown kind is just the base's rows
    CB_CHECK(t.store.authored_table({"win16", "no-such-kind"}).empty());
});

CB_TEST("knowledge.store.kinds_listing_counts_by_origin", [] {
    TestStore t;
    (void)t.store.authored_table({"win16", "api_model"});
    t.store.put("win16", "api_model", "EXTRA", Value(Value::Map{{"ax", Value(0)}}), "person", Origin::Given);
    t.store.put("win32", "argument_bytes.code", "A.f", Value(4), "code");
    const auto all = t.store.kinds(std::nullopt);
    CB_CHECK_EQ(all.size(), std::size_t{2});
    CB_CHECK_EQ(all[0].platform, "win16");
    CB_CHECK_EQ(all[0].kind, "api_model");
    CB_CHECK_EQ(all[0].keys, 15);
    CB_CHECK_EQ(all[0].origins.at("authored"), 14);
    CB_CHECK_EQ(all[0].origins.at("given"), 1);
    CB_CHECK_EQ(all[0].source, "data/knowledge/models/win16.json");
    CB_CHECK(all[0].bytes > 0);
    CB_CHECK_EQ(t.store.kinds("win32").size(), std::size_t{1});
    CB_CHECK(t.store.kinds("dos").empty());
});

CB_TEST("knowledge.store.the_knowledge_command_runs_over_it", [] {
    TestStore t;
    t.store.put("dos", "note", "21", Value(Value::Map{{"name", Value("DOS")}}), "me", Origin::Given);
    const std::vector<std::string> list = {"dos", "note"};
    const show::Outcome out = show::run(t.store, list);
    CB_CHECK_EQ(out.exit_code, 0);
    CB_CHECK(out.text.find("21") != std::string::npos);
    const std::vector<std::string> piece = {"dos", "note", "21"};
    const show::Outcome one = show::run(t.store, piece);
    CB_CHECK_EQ(one.exit_code, 0);
    CB_CHECK(one.text.find("DOS") != std::string::npos && one.text.find("given") != std::string::npos);
    const std::vector<std::string> missing = {"dos", "note", "no-such"};
    CB_CHECK_EQ(show::run(t.store, missing).exit_code, 1);
    // the lanes' names work: pe is win32
    CB_CHECK_EQ(t.store.platform("pe"), "win32");
});

CB_TEST("knowledge.store.threads_may_share_one_store", [] {
    TestStore t;
    std::vector<std::thread> threads;
    for (int n = 0; n < 4; ++n) {
        threads.emplace_back([&t, n] {
            for (int i = 0; i < 50; ++i) {
                const std::string key = "k" + std::to_string(n) + "-" + std::to_string(i);
                t.store.put("win32", "argument_bytes.code", key, Value(i), "thread");
                CB_CHECK_EQ(t.store.digest({"win32", "argument_bytes.code", key}), Value(i).digest());
            }
        });
    }
    for (std::thread& th : threads) th.join();
    CB_CHECK_EQ(t.store.table("win32", "argument_bytes.code").size(), std::size_t{200});
});

CB_TEST("knowledge.store.a_damaged_value_is_a_format_error", [] {
    TestStore t;
    t.store.put("dos", "interrupt", "1", Value(1), "x");
    // write a row the base did not make: a value that is not an encoded Value
    cb::base::sqlite::Database db = cb::base::sqlite::Database::open(t.dir.path() / "db" / "knowledge.v2.sqlite");
    db.exec("UPDATE knowledge SET value = X'0102' WHERE key = '1'");
    CB_CHECK_THROWS_AS(cb::FormatError, (void)t.store.get("dos", "interrupt", "1"));
});
