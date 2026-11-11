#include "cb/engine/facts.hpp"

#include <chrono>
#include <limits>

#include "cb/base/sqlite.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::engine::facts;
using Doc = knowledge::Value;

namespace {

std::string text_of(const Encoded& e) { return std::string(as_text(e.data)); }

Doc document_a() {
    return Doc(Doc::Map{{"b", Doc(1)}, {"a", Doc(Doc::List{Doc(1), Doc(2.5), Doc(), Doc("\xC3\xA9")})}});
}

}  // namespace

CB_TEST("engine.facts.encode_is_what_the_python_kept", [] {
    // (each expectation is what codebreaker/engine/facts.py's encode made of the same value)
    const Encoded doc = encode(document_a());
    CB_CHECK_EQ(doc.tag, 'J');
    CB_CHECK_EQ(text_of(doc), "{\"a\":[1,2.5,null,\"\\u00e9\"],\"b\":1}");   // keys sorted, compact, ASCII (the escape is in the text)
    const Encoded text = encode(std::string("text"));
    CB_CHECK_EQ(text.tag, 'S');
    CB_CHECK_EQ(text_of(text), "text");
    const Encoded bytes = encode(Bytes{0, 1});
    CB_CHECK_EQ(bytes.tag, 'B');
    CB_CHECK_EQ(encode(Doc("text")).tag, 'S');   // a text document is kept as text
    CB_CHECK_EQ(text_of(encode(Doc(Doc::List{Doc(true), Doc(false), Doc(Doc::Map{{"z", Doc(Doc::Map{})}, {"y", Doc(Doc::List{})}})}))),
                R"([true,false,{"y":[],"z":{}}])");
    CB_CHECK_EQ(text_of(encode(Doc(12))), "12");
    CB_CHECK_EQ(text_of(encode(Doc(-7))), "-7");
    CB_CHECK_EQ(text_of(encode(Doc(0.1))), "0.1");
    CB_CHECK_THROWS_AS(UnsupportedError, (void)encode(Doc(std::numeric_limits<double>::infinity())));
    CB_CHECK_THROWS_AS(UnsupportedError, (void)encode(Doc(std::numeric_limits<double>::quiet_NaN())));
});

CB_TEST("engine.facts.decode_round_trips", [] {
    const Doc doc = document_a();
    const Encoded e = encode(doc);
    const Value back = decode(e.tag, e.data);
    // (the keys of a stored map are sorted, so it comes back in key order: a, b)
    const Doc sorted(Doc::Map{{"a", Doc(Doc::List{Doc(1), Doc(2.5), Doc(), Doc("\xC3\xA9")})}, {"b", Doc(1)}});
    CB_CHECK(std::get<Doc>(back) == sorted);
    CB_CHECK(!(std::get<Doc>(back) == doc));
    CB_CHECK(std::get<std::string>(decode('S', as_bytes("héllo"))) == "héllo");
    CB_CHECK(std::get<Bytes>(decode('B', Bytes{1, 2})) == (Bytes{1, 2}));
    CB_CHECK_THROWS_AS(UnsupportedError, (void)decode('P', Bytes{0x80, 4}));   // a pickle is Python's
    CB_CHECK_THROWS_AS(UnsupportedError, (void)decode('X', {}));
    CB_CHECK_THROWS_AS(Error, (void)decode('J', as_bytes("{not json")));
});

CB_TEST("engine.facts.hashes_are_the_sha1_of_tag_and_bytes", [] {
    fs::ScopedTempDir dir;
    auto db = Database::open(dir.path() / "a.sqlite");
    // (hashlib.sha1(b"S" + b"text") and the others, from the Python)
    CB_CHECK_EQ(db->write("k", "1", std::string("text"), "s", "v", {}), "47ff5e97ba0a894ae59639eca8261a06c7673cb7");
    CB_CHECK_EQ(db->write("k", "2", Bytes{0, 1}, "s", "v", {}), "46cb18115cf117ca4b94ac63456131e6450f3dfa");
    CB_CHECK_EQ(db->write("k", "3", document_a(), "s", "v", {}), "a6b7db146e393c9960df976f288f53db12929628");
    CB_CHECK_EQ(db->write("k", "4", Doc(12), "s", "v", {}), "739d2dbf3ad6efb86467971d53ec833e1883c3ec");
});

CB_TEST("engine.facts.rows_keep_how_they_were_made", [] {
    fs::ScopedTempDir dir;
    auto db = Database::open(dir.path() / "a.sqlite");
    const std::vector<Input> inputs = {{"function.object", "1:0010", "aa"}, {"binary", "", "bb"}};
    const std::string hash = db->write("function.c", "1:7cd4", std::string("int f(void){return 1;}"), "function.c", "v7", inputs, 0.25);
    const auto r = db->row("function.c", "1:7cd4");
    CB_CHECK(r.has_value());
    CB_CHECK_EQ(r->hash, hash);
    CB_CHECK_EQ(r->tag, 'S');
    CB_CHECK_EQ(r->size, 22u);
    CB_CHECK_EQ(r->step, "function.c");
    CB_CHECK_EQ(r->version, "v7");
    CB_CHECK(r->inputs == inputs);
    CB_CHECK_EQ(r->took, 0.25);
    CB_CHECK(r->made > 1.7e9);
    CB_CHECK(!r->given());
    CB_CHECK(std::get<std::string>(r->value()) == "int f(void){return 1;}");
    CB_CHECK(!db->row("function.c", "1:0000").has_value());
    // a key of the whole binary is ""
    db->write("binary.sha", "", std::string("x"), "given", "", {});
    CB_CHECK(db->row("binary.sha")->given());
    // rows: by kind, then key; with a kind: by key
    db->write("function.c", "1:0010", std::string("a"), "function.c", "v7", {});
    const auto all = db->rows();
    CB_CHECK_EQ(all.size(), 3u);
    CB_CHECK_EQ(all[0].kind, "binary.sha");
    CB_CHECK_EQ(all[1].key, "1:0010");
    CB_CHECK_EQ(all[2].key, "1:7cd4");
    CB_CHECK_EQ(db->rows("function.c").size(), 2u);
    CB_CHECK(db->rows("nothing").empty());
    const auto kinds = db->kinds();
    CB_CHECK_EQ(kinds.size(), 2u);
    CB_CHECK_EQ(kinds[1].kind, "function.c");
    CB_CHECK_EQ(kinds[1].count, 2u);
    CB_CHECK_EQ(kinds[1].bytes, 23u);
    // writing the same fact again replaces it
    db->write("function.c", "1:0010", std::string("b"), "function.c", "v8", {});
    CB_CHECK_EQ(db->row("function.c", "1:0010")->version, "v8");
    CB_CHECK_EQ(db->rows("function.c").size(), 2u);
});

CB_TEST("engine.facts.a_large_value_is_kept_once_by_its_hash", [] {
    fs::ScopedTempDir dir;
    const auto file = dir.path() / "a.sqlite";
    auto db = Database::open(file);
    const Bytes big(kBlobAt + 1, 0x5a), edge(kBlobAt, 0x5a);
    const std::string h1 = db->write("k", "1", big, "s", "v", {});
    const std::string h2 = db->write("k", "2", big, "s", "v", {});
    CB_CHECK_EQ(h1, h2);
    db->write("k", "3", edge, "s", "v", {});
    base::sqlite::Database raw = base::sqlite::Database::open(file);
    const auto blobs = raw.query("SELECT COUNT(*) FROM blob");
    CB_CHECK_EQ(std::get<std::int64_t>(blobs[0][0]), 1);   // held by two facts, kept once; the value of exactly kBlobAt stays in its row
    CB_CHECK(db->row("k", "3")->data() == edge);
    CB_CHECK(db->row("k", "1")->data() == big);
    CB_CHECK_THROWS_AS(FormatError, (void)db->blob("nosuchhash"));
    // forgetting both leaves the blob until collect
    CB_CHECK_EQ(db->forget("k", "1"), 1u);
    db->collect();
    CB_CHECK_EQ(std::get<std::int64_t>(raw.query("SELECT COUNT(*) FROM blob")[0][0]), 1);
    CB_CHECK_EQ(db->forget("k", "2"), 1u);
    db->collect();
    CB_CHECK_EQ(std::get<std::int64_t>(raw.query("SELECT COUNT(*) FROM blob")[0][0]), 0);
});

CB_TEST("engine.facts.forget_remake_about", [] {
    fs::ScopedTempDir dir;
    auto db = Database::open(dir.path() / "a.sqlite");
    db->write("k", "1", Doc(1), "s", "v1", {});
    db->write("k", "2", Doc(2), "s", "v1", {});
    db->write("j", "1", Doc(3), "s", "v1", {});
    db->remake("k", "1", "v2", {{"a", "b", "c"}});
    CB_CHECK_EQ(db->row("k", "1")->version, "v2");
    CB_CHECK(db->row("k", "1")->inputs == (std::vector<Input>{{"a", "b", "c"}}));
    CB_CHECK_EQ(db->forget("k"), 2u);
    CB_CHECK(!db->row("k", "1") && !db->row("k", "2") && db->row("j", "1"));
    CB_CHECK_EQ(db->forget("k"), 0u);
    CB_CHECK(!db->about("name").has_value());
    db->set_about("name", "lz32.dll");
    db->set_about("name", "other.dll");
    CB_CHECK_EQ(*db->about("name"), "other.dll");
});

CB_TEST("engine.facts.other_versions_of_a_step_are_dropped_once", [] {
    fs::ScopedTempDir dir;
    auto db = Database::open(dir.path() / "a.sqlite");
    db->write("c", "1", Doc(1), "compile", "old", {});
    db->write("c", "2", Doc(2), "compile", "new", {});
    db->write("d", "1", std::string("typed by hand"), std::string(kGiven), "", {});
    CB_CHECK(!db->cleaned("compile"));
    CB_CHECK_EQ(db->drop_other_versions("compile", "new"), 1u);
    CB_CHECK(db->cleaned("compile"));
    CB_CHECK(!db->row("c", "1") && db->row("c", "2"));
    // the step is clean for the rest of the process: a later call does nothing, even for facts that are of another version
    db->write("c", "3", Doc(3), "compile", "older", {});
    CB_CHECK_EQ(db->drop_other_versions("compile", "new"), 0u);
    CB_CHECK(db->row("c", "3").has_value());
    // what a person gave is never dropped
    CB_CHECK_EQ(db->drop_other_versions(kGiven, "anything"), 0u);
    CB_CHECK(db->row("d", "1")->given());
});

CB_TEST("engine.facts.write_raw_keeps_the_hash_it_is_given", [] {
    fs::ScopedTempDir dir;
    auto db = Database::open(dir.path() / "a.sqlite");
    db->write_raw("binary", "", "feedface", 'B', Bytes{1, 2, 3}, "given", "v", R"([["a", "b", "c"]])", 1.5);
    const auto r = db->row("binary");
    CB_CHECK_EQ(r->hash, "feedface");
    CB_CHECK(r->data() == (Bytes{1, 2, 3}));
    CB_CHECK_EQ(r->took, 1.5);
    CB_CHECK(r->inputs == (std::vector<Input>{{"a", "b", "c"}}));
});

CB_TEST("engine.facts.databases_are_per_binary_and_one_open_per_file", [] {
    fs::ScopedTempDir dir;
    Databases dbs({.root = dir.path() / "db", .max_bytes = kDefaultMaxBytes});
    const auto binary = dir.path() / "prog.exe";
    fs::write_file(binary, as_bytes("MZ not really a program"));
    std::error_code ec;
    std::filesystem::last_write_time(binary, std::filesystem::file_time_type::clock::now() - std::chrono::hours(1), ec);
    // (the SHA-256 of those 23 bytes)
    const std::string sha = dbs.sha_of(binary);
    CB_CHECK_EQ(sha, crypto::to_hex(crypto::sha256(as_bytes("MZ not really a program"))));
    CB_CHECK_EQ(dbs.sha_of(binary), sha);
    auto db = dbs.open_for(binary);
    CB_CHECK_EQ(db->sha, sha);
    CB_CHECK_EQ(db->path().filename().string(), sha + ".sqlite");
    CB_CHECK_EQ(*db->about("name"), "prog.exe");
    CB_CHECK(db->about("path")->find("prog.exe") != std::string::npos);
    CB_CHECK(dbs.open_for(binary) == db);   // the same open database
    CB_CHECK(dbs.open_at(db->path()) == db);
    CB_CHECK_EQ(Databases::work_folder(*db, "objs").filename().string(), "objs");
    CB_CHECK(std::filesystem::is_directory(Databases::work_folder(*db, "objs")));
    CB_CHECK_EQ(Databases::work_folder(*db).filename().string(), sha + ".work");
    CB_CHECK_THROWS_AS(NotFoundError, (void)dbs.sha_of(dir.path() / "missing.exe"));
    // another binary, another database
    const auto other = dir.path() / "other.exe";
    fs::write_file(other, as_bytes("something else"));
    CB_CHECK(dbs.open_for(other) != db);
    CB_CHECK_EQ(dbs.databases().size(), 2u);
});

CB_TEST("engine.facts.the_cap_removes_the_oldest_closed_database", [] {
    fs::ScopedTempDir dir;
    Databases dbs({.root = dir.path(), .max_bytes = kDefaultMaxBytes});
    const Bytes payload(200 * 1024, 7);
    const auto make = [&](const char* name, int hours_ago) {
        {
            auto db = Database::open(dir.path() / name);
            db->write("k", "1", payload, "s", "v", {});
        }
        std::error_code ec;
        std::filesystem::last_write_time(dir.path() / name, std::filesystem::file_time_type::clock::now() - std::chrono::hours(hours_ago), ec);
    };
    make("old.sqlite", 30);
    make("middle.sqlite", 20);
    make("new.sqlite", 10);
    make("knowledge.sqlite", 40);   // never counted, never removed
    std::filesystem::create_directories(dir.path() / "old.work");
    const auto listed = dbs.databases();
    CB_CHECK_EQ(listed.size(), 3u);
    CB_CHECK_EQ(listed[0].path.filename().string(), "old.sqlite");   // the one opened longest ago first
    CB_CHECK_EQ(listed[2].path.filename().string(), "new.sqlite");
    // under the cap: nothing goes
    CB_CHECK(dbs.hold(std::uint64_t{100} << 20).empty());
    // a cap that holds only about one: the oldest go until four fifths of it is used
    const std::uint64_t one = listed[2].bytes;
    const auto gone = dbs.hold(one * 2);
    CB_CHECK_EQ(gone.size(), 2u);
    CB_CHECK_EQ(gone[0].filename().string(), "old.sqlite");
    CB_CHECK(!std::filesystem::exists(dir.path() / "old.sqlite"));
    CB_CHECK(!std::filesystem::exists(dir.path() / "old.work"));
    CB_CHECK(!std::filesystem::exists(dir.path() / "middle.sqlite"));
    CB_CHECK(std::filesystem::exists(dir.path() / "new.sqlite"));
    CB_CHECK(std::filesystem::exists(dir.path() / "knowledge.sqlite"));
});

CB_TEST("engine.facts.a_database_open_here_is_never_removed", [] {
    fs::ScopedTempDir dir;
    Databases dbs({.root = dir.path(), .max_bytes = kDefaultMaxBytes});
    auto db = dbs.open_at(dir.path() / "a.sqlite");
    db->write("k", "1", Bytes(100 * 1024, 1), "s", "v", {});
    CB_CHECK(dbs.hold(1).empty());
    CB_CHECK(std::filesystem::exists(dir.path() / "a.sqlite"));
    dbs.close(dir.path() / "a.sqlite");
    db.reset();
    CB_CHECK_EQ(dbs.hold(1).size(), 1u);
});

CB_TEST("engine.facts.a_database_with_the_function_stores_tables_loses_only_its_facts", [] {
    fs::ScopedTempDir dir;
    Databases dbs({.root = dir.path(), .max_bytes = kDefaultMaxBytes});
    const auto file = dir.path() / "a.sqlite";
    {
        auto db = Database::open(file);
        db->write("k", "1", Bytes(300 * 1024, 1), "s", "v", {});
        base::sqlite::Database raw = base::sqlite::Database::open(file);
        raw.exec("CREATE TABLE verdict(function TEXT, v TEXT)");
        raw.exec("INSERT INTO verdict VALUES('f', 'verified')");
    }
    const auto gone = dbs.hold(1);
    CB_CHECK_EQ(gone.size(), 1u);
    CB_CHECK(std::filesystem::exists(file));
    auto db = Database::open(file);
    CB_CHECK(db->rows().empty());
    base::sqlite::Database raw = base::sqlite::Database::open(file);
    CB_CHECK_EQ(raw.query("SELECT COUNT(*) FROM verdict").size(), 1u);
    CB_CHECK_EQ(std::get<std::int64_t>(raw.query("SELECT COUNT(*) FROM verdict")[0][0]), 1);
});
