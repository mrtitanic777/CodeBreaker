// The old base/cache API (codebreaker::base::cache, over py::) as the compat shim at native/typed/cpp/codebreaker/base/
// cache.hpp gives it: conversion and forwarding to cb::base::cache. Builds at /W3: the old layer is not warning-clean.
#include "codebreaker/base/cache.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

namespace old = codebreaker::base::cache;

// The settings the old code reads (py::settings) come from a hook here: a state folder and a build cache of our own
// (the old layer reads the process environment once, so CB_* variables set per test would not be seen).
struct OldWorld {
    cb::fs::ScopedTempDir dir{"cb-shim"};
    cb::fs::ScopedEnv off{"CB_NO_CACHE", std::nullopt};
    OldWorld() {
        py::settings::hook() = [root = dir.path()](const py::str& key) -> py::str {
            if (key == py::str("state")) return py::str(cb::fs::to_text(root / "state"));
            if (key == py::str("build_cache")) return py::str(cb::fs::to_text(root / "bc"));
            if (key == py::str("cache_max_gb")) return py::str("2");
            return py::str("");
        };
        old::_close();
    }
    ~OldWorld() {
        old::_close();
        py::settings::hook() = nullptr;
    }
};

old::Part P(const char* s) { return old::Part(py::str(s)); }

// Keys the Python module made, from the old harness's expected.txt (key none, a, a b, ab, bytes ab, int, True, None,
// 1.5, 0.1, list, ints, unicode, mixed): the shim keeps the algorithm, so the same ingredients file under the same key.
CB_TEST("compat.base_cache.keys_equal_the_pythons", [] {
    CB_CHECK_EQ(old::key({}).utf8(), std::string("da39a3ee5e6b4b0d3255bfef95601890afd80709"));
    CB_CHECK_EQ(old::key({P("a")}).utf8(), std::string("adfba10e74dfa3600bdefaef15349f9804c6be41"));
    CB_CHECK_EQ(old::key({P("a"), P("b")}).utf8(), std::string("95253b90414f24c679779b57f9eb73d08de34a87"));
    CB_CHECK_EQ(old::key({old::Part(py::bytes("ab"))}).utf8(), std::string("85034e5201970f2bd081b69fc135511712c1c27e"));
    CB_CHECK_EQ(old::key({old::Part(int64_t(1))}).utf8(), std::string("3e774731d33d9224ac36af3d85ba1f81b31bc84d"));
    CB_CHECK_EQ(old::key({old::Part(true)}).utf8(), std::string("5be20837e0561fda069844a45d9c8352df93766e"));
    CB_CHECK_EQ(old::key({old::Part(py::none)}).utf8(), std::string("facdd223dbe368e734ba52cd378c183f35f0d705"));
    CB_CHECK_EQ(old::key({old::Part(1.5)}).utf8(), std::string("d1a08ed51bfba07775070e7b6a6f7650711f0e93"));
    CB_CHECK_EQ(old::key({old::Part(0.1)}).utf8(), std::string("4f20e43c6654d01c559a4f96429f18c670dc2b4d"));
    py::list<py::str> ab;
    ab.append(py::str("a"));
    ab.append(py::str("b"));
    CB_CHECK_EQ(old::key({old::Part(ab)}).utf8(), std::string("7a53299428969e086b020323fbd20a948cc25ac0"));
    py::list<int64_t> ints;
    for (int64_t i : {1, 2, 3}) ints.append(i);
    CB_CHECK_EQ(old::key({old::Part(ints)}).utf8(), std::string("84daa38bd9a427e9a6668ae5ad64cace72d1b6e2"));
});

CB_TEST("compat.base_cache.put_get_and_counts", [] {
    OldWorld w;
    const py::str k = old::key({P("input")});
    CB_CHECK(!old::get(py::str("objects"), k).has_value());
    old::put(py::str("objects"), k, py::bytes(std::string("\x00\x01\xFF payload", 11)));
    const auto got = old::get(py::str("objects"), k);
    CB_CHECK(got.has_value() && got->raw() == std::string("\x00\x01\xFF payload", 11));
    const auto counts = old::counts();
    CB_CHECK_EQ(std::get<0>(counts.at(py::str("objects"))), 1);
    CB_CHECK_EQ(std::get<1>(counts.at(py::str("objects"))), 1);
    // "objects" is a shared kind: it is in the build cache, not the state folder
    CB_CHECK(std::filesystem::exists(w.dir.path() / "bc" / "objects"));
    CB_CHECK(!std::filesystem::exists(w.dir.path() / "state" / "cache" / "objects"));
    // CB_NO_CACHE turns it off
    {
        cb::fs::ScopedEnv off("CB_NO_CACHE", "1");
        CB_CHECK(old::off());
        CB_CHECK(!old::get(py::str("objects"), k).has_value());
    }
    CB_CHECK(old::get(py::str("objects"), k).has_value());
});

CB_TEST("compat.base_cache.typed_values_still_pickle", [] {
    OldWorld w;
    py::list<int64_t> xs;
    for (int64_t i : {1, 2, 300, -5}) xs.append(i);
    old::keep(py::str("values"), py::str("k1"), xs);
    const auto back = old::value<py::list<int64_t>>(py::str("values"), py::str("k1"));
    CB_CHECK(back.has_value() && back->size() == 4 && (*back)[2] == 300 && (*back)[3] == -5);
    old::keep(py::str("values"), py::str("k2"), std::make_tuple(py::str("a"), int64_t(7), 2.5, true));
    const auto t = old::value<std::tuple<py::str, int64_t, double, bool>>(py::str("values"), py::str("k2"));
    CB_CHECK(t.has_value() && std::get<0>(*t) == py::str("a") && std::get<1>(*t) == 7 && std::get<2>(*t) == 2.5 && std::get<3>(*t));
    CB_CHECK(!old::value<py::list<int64_t>>(py::str("values"), py::str("missing")).has_value());
    CB_CHECK(!old::value<py::str>(py::str("values"), py::str("k1")).has_value());          // another type
    old::put(py::str("values"), py::str("junk"), py::bytes(std::string("not a pickle")));
    CB_CHECK(!old::value<py::list<int64_t>>(py::str("values"), py::str("junk")).has_value());
});

CB_TEST("compat.base_cache.generations_come_from_the_registered_reprs", [] {
    OldWorld w;
    CB_CHECK_EQ(old::generation(py::str("anything")).value().utf8(), std::string("g0"));
    CB_CHECK(!old::generation(py::str("fingerprints")).has_value());                         // listed, no repr registered
    old::GENERATION_REPRS.set(py::str("fingerprints"), []() -> std::optional<py::str> { return py::str("42"); });
    const auto g = old::generation(py::str("fingerprints"));
    CB_CHECK(g.has_value() && g->utf8().size() == 13 && g->utf8()[0] == 'g');
    old::put(py::str("fingerprints"), py::str("k"), py::bytes(std::string("v")));
    old::GENERATION_REPRS.set(py::str("fingerprints"), []() -> std::optional<py::str> { return py::str("43"); });
    old::_close();                                                                           // (a new process)
    CB_CHECK(!old::get(py::str("fingerprints"), py::str("k")).has_value());                  // the code changed: gone
    old::GENERATION_REPRS.set(py::str("fingerprints"), []() -> std::optional<py::str> { return std::nullopt; });
});

CB_TEST("compat.base_cache.tools_folders_and_files", [] {
    OldWorld w;
    const auto tool_path = w.dir.path() / "Tool.EXE";
    cb::fs::write_file(tool_path, cb::Bytes(100, 7));
    const py::str t = old::tool(py::str(cb::fs::to_text(tool_path)));
    CB_CHECK(t.utf8().starts_with("tool.exe:100:"));
    CB_CHECK_EQ(old::tool(py::str(cb::fs::to_text(w.dir.path() / "No.EXE"))).utf8(), std::string("no.exe:missing"));
    const py::str tb = old::tool_bytes(py::str(cb::fs::to_text(tool_path)));
    CB_CHECK(tb.utf8().starts_with("tool.exe:"));
    CB_CHECK_EQ(old::tool_bytes(py::str(cb::fs::to_text(w.dir.path() / "No.EXE"))).utf8(), std::string("no.exe:missing"));
    py::list<py::str> folders;
    folders.append(py::str(cb::fs::to_text(w.dir.path() / "state")));
    CB_CHECK_EQ(old::files_bytes(folders).utf8().size(), std::size_t{40});
    CB_CHECK_EQ(old::folder(py::str(cb::fs::to_text(w.dir.path()))).utf8().size(), std::size_t{40});
});

CB_TEST("compat.base_cache.path_hold_stats_prune_clear_and_main", [] {
    OldWorld w;
    const py::str k = old::key({P("x")});
    const std::filesystem::path p = cb::fs::from_text(old::path(py::str("port-objects"), k).utf8());
    cb::fs::write_file(p, cb::Bytes(1000, 1));
    CB_CHECK(old::hold() >= 0);
    old::put(py::str("kept"), py::str("a"), py::bytes(std::string(500, 'x')));
    bool saw = false;
    for (const auto& row : old::stats().vec()) saw = saw || std::get<0>(row) == py::str("kept");
    CB_CHECK(saw);
    CB_CHECK(old::sizes().contains(py::str("kept")));
    CB_CHECK(old::prune().empty());
    CB_CHECK_EQ(old::root().utf8(), cb::fs::to_text(w.dir.path() / "state" / "cache"));
    CB_CHECK_EQ(old::root(py::str("masm")).utf8(), cb::fs::to_text(w.dir.path() / "bc"));
    CB_CHECK_EQ(old::max_bytes(), int64_t(2) << 30);
    CB_CHECK_EQ(old::roots().size(), int64_t(2));
    py::list<py::str> stats_arg;
    stats_arg.append(py::str("--stats"));
    cb::testing::CapturedStd cap;
    CB_CHECK_EQ(old::main(stats_arg), 0);
    CB_CHECK(cap.out().find("kept") != std::string::npos);
    py::list<py::str> bad;
    bad.append(py::str("--bogus"));
    CB_CHECK_EQ(old::main(bad), 2);
    CB_CHECK(old::clear() > 0);
    CB_CHECK(!old::get(py::str("kept"), py::str("a")).has_value());
});

// ---- Db: a Python-sqlite3 look-alike over cb::base::sqlite ----------------------------------------------------------

CB_TEST("compat.base_cache.db_begins_a_transaction_before_a_write_and_commit_ends_it", [] {
    OldWorld w;
    const py::str file(cb::fs::to_text(w.dir.path() / "d" / "facts.db"));
    {
        auto db = old::Db::connect(file, 5.0);
        db->execute("CREATE TABLE t(a INTEGER, b TEXT, c BLOB)");
        db->execute("INSERT INTO t VALUES(?,?,?)", {int64_t(1), py::str("x"), py::bytes(std::string("\x00\x01", 2))});
        db->commit();
        db->executemany("INSERT INTO t VALUES(?,?,?)", {{int64_t(2), py::str("y"), std::monostate{}}, {int64_t(3), 2.5, py::str("z")}});
        // not committed: close rolls it back, as sqlite3 does
        db->close();
        CB_CHECK_THROWS_AS(old::SqliteError, db->execute("SELECT 1"));
    }
    auto db = old::Db::connect(file);
    const auto rows = db->query("SELECT a, b, c FROM t ORDER BY a");
    CB_CHECK_EQ(rows.size(), std::size_t{1});
    CB_CHECK_EQ(std::get<int64_t>(rows[0][0]), 1);
    CB_CHECK(std::get<py::str>(rows[0][1]) == py::str("x"));
    CB_CHECK_EQ(std::get<py::bytes>(rows[0][2]).raw(), std::string("\x00\x01", 2));
    db->executemany("INSERT INTO t VALUES(?,?,?)", {{int64_t(2), py::str("y"), std::monostate{}}});
    db->commit();
    CB_CHECK_EQ(db->query("SELECT count(*) FROM t")[0][0].index(), size_t(1));
    CB_CHECK_THROWS_AS(old::SqliteError, db->execute("NOT SQL"));
    CB_CHECK(cb::fs::same_path(cb::fs::from_text(db->filename().utf8()), cb::fs::from_text(file.utf8())));
});

}  // namespace
