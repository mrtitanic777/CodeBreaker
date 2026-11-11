#include "cb/base/sqlite.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

using cb::Bytes;
using cb::base::sqlite::Database;
using cb::base::sqlite::Row;
using cb::base::sqlite::Value;

CB_TEST("base.sqlite.every_storage_class_round_trips", [] {
    cb::fs::ScopedTempDir dir("cb-sqlite");
    Database db = Database::open(dir.path() / "t.db");
    db.exec("CREATE TABLE t(i INTEGER, r REAL, s TEXT, b BLOB, n)");
    const Bytes blob = {0, 1, 2, 0xFF, 0};
    db.exec("INSERT INTO t VALUES(?,?,?,?,?)",
            {Value(std::int64_t{-5}), Value(2.5), Value(std::string("h\xC3\xA9llo")), Value(blob), Value(std::monostate{})});
    const std::vector<Row> rows = db.query("SELECT i, r, s, b, n FROM t");
    CB_CHECK_EQ(rows.size(), std::size_t{1});
    CB_CHECK_EQ(std::get<std::int64_t>(rows[0][0]), -5);
    CB_CHECK_EQ(std::get<double>(rows[0][1]), 2.5);
    CB_CHECK_EQ(std::get<std::string>(rows[0][2]), std::string("h\xC3\xA9llo"));
    CB_CHECK(std::get<Bytes>(rows[0][3]) == blob);
    CB_CHECK(std::holds_alternative<std::monostate>(rows[0][4]));
    CB_CHECK_EQ(db.changes(), 1);
});

CB_TEST("base.sqlite.text_with_embedded_zero_and_empty_blob", [] {
    cb::fs::ScopedTempDir dir("cb-sqlite");
    Database db = Database::open(dir.path() / "t.db");
    db.exec("CREATE TABLE t(s TEXT, b BLOB)");
    db.exec("INSERT INTO t VALUES(?,?)", {Value(std::string("a\0b", 3)), Value(Bytes{})});
    const std::vector<Row> rows = db.query("SELECT s, b FROM t");
    CB_CHECK_EQ(std::get<std::string>(rows[0][0]).size(), std::size_t{3});
    CB_CHECK(std::get<Bytes>(rows[0][1]).empty());
});

CB_TEST("base.sqlite.persists_across_open_and_makes_its_folder", [] {
    cb::fs::ScopedTempDir dir("cb-sqlite");
    const auto file = dir.path() / "a" / "b" / "t.db";
    {
        Database db = Database::open(file);
        db.exec("CREATE TABLE t(x)");
        db.exec_many("INSERT INTO t VALUES(?)", {{Value(std::int64_t{1})}, {Value(std::int64_t{2})}, {Value(std::int64_t{3})}});
    }
    Database again = Database::open(file);
    CB_CHECK_EQ(std::get<std::int64_t>(again.query("SELECT sum(x) FROM t")[0][0]), 6);
});

CB_TEST("base.sqlite.transaction_rolls_back_unless_committed", [] {
    cb::fs::ScopedTempDir dir("cb-sqlite");
    Database db = Database::open(dir.path() / "t.db");
    db.exec("CREATE TABLE t(x)");
    {
        Database::Transaction tx(db);
        db.exec("INSERT INTO t VALUES(1)");
    }   // no commit
    CB_CHECK_EQ(std::get<std::int64_t>(db.query("SELECT count(*) FROM t")[0][0]), 0);
    {
        Database::Transaction tx(db);
        db.exec("INSERT INTO t VALUES(1)");
        tx.commit();
    }
    CB_CHECK_EQ(std::get<std::int64_t>(db.query("SELECT count(*) FROM t")[0][0]), 1);
});

CB_TEST("base.sqlite.exec_many_is_all_or_nothing", [] {
    cb::fs::ScopedTempDir dir("cb-sqlite");
    Database db = Database::open(dir.path() / "t.db");
    db.exec("CREATE TABLE t(x INTEGER PRIMARY KEY)");
    CB_CHECK_THROWS_AS(cb::IoError,
                       db.exec_many("INSERT INTO t VALUES(?)", {{Value(std::int64_t{1})}, {Value(std::int64_t{1})}}));
    CB_CHECK_EQ(std::get<std::int64_t>(db.query("SELECT count(*) FROM t")[0][0]), 0);
});

CB_TEST("base.sqlite.errors_are_io_errors_naming_the_file", [] {
    cb::fs::ScopedTempDir dir("cb-sqlite");
    Database db = Database::open(dir.path() / "t.db");
    CB_CHECK_THROWS_AS(cb::IoError, db.exec("NOT SQL AT ALL"));
    CB_CHECK_THROWS_AS(cb::IoError, db.query("SELECT * FROM missing_table"));
    try {
        db.exec("SELECT nope FROM nowhere");
        CB_CHECK(false);
    } catch (const cb::IoError& e) {
        CB_CHECK(cb::fs::same_path(e.path(), dir.path() / "t.db"));
    }
    // a file that is not a database
    cb::fs::write_text(dir.path() / "junk.db", "this is not a sqlite file, not even close, 0123456789abcdef");
    CB_CHECK_THROWS_AS(cb::IoError, Database::open(dir.path() / "junk.db").query("SELECT 1 FROM sqlite_master"));
});

}  // namespace
