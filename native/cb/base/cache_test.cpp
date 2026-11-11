#include "cb/base/cache.hpp"

#include <chrono>
#include <thread>

#include "cb/base/sqlite.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

namespace cache = cb::base::cache;
using cb::Bytes;
namespace stdfs = std::filesystem;

// A cache in a temp folder pair that goes away with the object.
struct Sandbox {
    cb::fs::ScopedTempDir dir{"cb-cache"};
    cache::Config config() const {
        cache::Config c;
        c.state_dir = dir.path() / "state";
        c.shared_dir = dir.path() / "shared";
        return c;
    }
};

Bytes pattern(std::size_t n, std::uint32_t seed) {
    Bytes out(n);
    std::uint32_t x = seed * 2654435761u + 1;
    for (auto& b : out) {
        x = x * 1664525u + 1013904223u;
        b = static_cast<std::uint8_t>(x >> 24);
    }
    return out;
}

// ---- the envelope's checksum ---------------------------------------------------------------------------------

CB_TEST("base.cache.checksum_is_crc32c_in_hardware_and_in_tables", [] {
    const std::string check = "123456789";
    CB_CHECK_EQ(cache::entry_checksum(cb::as_bytes(check)), std::uint32_t{0xE3069283});
    CB_CHECK_EQ(cache::entry_checksum_portable(cb::as_bytes(check)), std::uint32_t{0xE3069283});
    CB_CHECK_EQ(cache::entry_checksum({}), std::uint32_t{0});
    for (std::size_t n : {1u, 7u, 8u, 9u, 15u, 16u, 17u, 100u, 4097u}) {
        const Bytes data = pattern(n, static_cast<std::uint32_t>(n));
        CB_CHECK_EQ(cache::entry_checksum(data), cache::entry_checksum_portable(data));
    }
});

// ---- keys ----------------------------------------------------------------------------------------------------

// Reviewed against hashlib.sha1 over the length-prefixed parts ("1:a", "1:a2:b" ...): the algorithm is the one the
// Python cache used, so keys of the same ingredients are the same.
CB_TEST("base.cache.key_is_sha1_of_length_prefixed_parts", [] {
    CB_CHECK_EQ(cache::key({}), std::string("da39a3ee5e6b4b0d3255bfef95601890afd80709"));   // sha1 of nothing
    CB_CHECK_EQ(cache::key({""}), std::string("b44b82a4bc6c35f6ad5e9fceefef9509c17fba74"));  // sha1("0:")
    CB_CHECK_EQ(cache::key({"a"}), std::string("adfba10e74dfa3600bdefaef15349f9804c6be41"));  // sha1("1:a")
    CB_CHECK_EQ(cache::key({"a", "b"}), std::string("95253b90414f24c679779b57f9eb73d08de34a87"));
    CB_CHECK_EQ(cache::key({"ab"}), std::string("85034e5201970f2bd081b69fc135511712c1c27e"));
    CB_CHECK_EQ(cache::key({1}), std::string("3e774731d33d9224ac36af3d85ba1f81b31bc84d"));       // the text "1"
});

CB_TEST("base.cache.key_parts_that_mean_the_same_are_the_same", [] {
    const Bytes ab = {'a', 'b'};
    CB_CHECK_EQ(cache::key({cache::KeyPart(cb::ByteView(ab))}), cache::key({"ab"}));
    CB_CHECK_EQ(cache::key({std::int64_t{42}}), cache::key({"42"}));
    CB_CHECK_EQ(cache::key({1.5}), cache::key({"1.5"}));
    // a list is its items joined by 0x1f, as one part
    const std::vector<std::string> list = {"a", "b"};
    CB_CHECK_EQ(cache::key({cache::KeyPart(list)}), cache::key({"a\x1f" "b"}));
    // the length prefix keeps the parts apart
    CB_CHECK_NE(cache::key({"ab"}), cache::key({"a", "b"}));
    CB_CHECK_NE(cache::key({"a", "bc"}), cache::key({"ab", "c"}));
    CB_CHECK_NE(cache::key({true}), cache::key({false}));
});

CB_TEST("base.cache.tool_id_names_size_and_time", [] {
    cb::fs::ScopedTempDir dir("cb-cache");
    const stdfs::path tool = dir.path() / "Tool.EXE";
    cb::fs::write_file(tool, pattern(1234, 1));
    const std::string id = cache::tool_id(tool);
    CB_CHECK(id.starts_with("tool.exe:1234:"));
    CB_CHECK_EQ(cache::tool_id(dir.path() / "No Such.EXE"), std::string("no such.exe:missing"));
    cb::fs::write_file(tool, pattern(1235, 1));   // a rebuilt tool is a different tool
    CB_CHECK_NE(cache::tool_id(tool), id);
});

CB_TEST("base.cache.tools_and_folders_by_content", [] {
    cb::fs::ScopedTempDir dir("cb-cache");
    Sandbox sb;
    cache::Cache c(sb.config());
    const stdfs::path a = dir.path() / "a.exe", b = dir.path() / "sub" / "B.EXE";
    cb::fs::write_file(a, pattern(5000, 2));
    cb::fs::write_file(b, pattern(5000, 2));     // the same bytes elsewhere, touched later
    CB_CHECK(c.tool_bytes_id(a).starts_with("a.exe:"));
    CB_CHECK_EQ(c.tool_bytes_id(a).substr(6), c.tool_bytes_id(b).substr(6));
    CB_CHECK_EQ(c.tool_bytes_id(a), c.tool_bytes_id(a));          // remembered
    CB_CHECK_EQ(c.tool_bytes_id(dir.path() / "none.exe"), std::string("none.exe:missing"));
    CB_CHECK_EQ(c.tool_bytes_id(dir.path() / "sub"), std::string("sub:missing"));   // a folder is not a tool
    cb::fs::write_file(b, pattern(5000, 3));
    CB_CHECK_NE(c.tool_bytes_id(a).substr(6), c.tool_bytes_id(b).substr(6));

    // a folder of headers: names (case-folded, relative) and bytes; creation order does not matter
    const stdfs::path h1 = dir.path() / "h1", h2 = dir.path() / "h2";
    cb::fs::write_file(h1 / "A.h", pattern(10, 4));
    cb::fs::write_file(h1 / "sub" / "c.h", pattern(20, 5));
    cb::fs::write_file(h2 / "sub" / "c.h", pattern(20, 5));
    cb::fs::write_file(h2 / "a.H", pattern(10, 4));
    const stdfs::path both1[] = {h1}, both2[] = {h2};
    CB_CHECK_EQ(c.files_bytes_id(both1), c.files_bytes_id(both2));
    cb::fs::write_file(h2 / "sub" / "c.h", pattern(20, 6));
    CB_CHECK_NE(c.files_bytes_id(both1), c.files_bytes_id(both2));
    CB_CHECK_NE(c.files_bytes_id({}), c.files_bytes_id(both1));
    const stdfs::path missing[] = {dir.path() / "nope"};
    CB_CHECK_EQ(c.files_bytes_id(missing), c.files_bytes_id({}));    // nothing to read is nothing
});

CB_TEST("base.cache.folder_id_changes_with_a_file", [] {
    cb::fs::ScopedTempDir dir("cb-cache");
    cb::fs::write_file(dir.path() / "x.txt", pattern(10, 1));
    const std::string before = cache::folder_id(dir.path());
    CB_CHECK_EQ(before, cache::folder_id(dir.path()));
    cb::fs::write_file(dir.path() / "y.txt", pattern(10, 2));
    CB_CHECK_NE(before, cache::folder_id(dir.path()));
    CB_CHECK_EQ(cache::folder_id(dir.path() / "nope"), cache::key({cache::KeyPart(std::vector<std::string>{})}));
});

// ---- results -------------------------------------------------------------------------------------------------

CB_TEST("base.cache.put_then_get_and_a_second_cache_sees_it", [] {
    Sandbox sb;
    const std::string k = cache::key({"input"});
    const Bytes data = pattern(70000, 7);
    {
        cache::Cache c(sb.config());
        CB_CHECK(!c.get("objects", k).has_value());
        c.put("objects", k, data);
        const auto got = c.get("objects", k);
        CB_CHECK(got.has_value() && *got == data);
        CB_CHECK(!c.get("objects", cache::key({"other"})).has_value());
        c.put("objects", cache::key({"empty"}), Bytes{});
        CB_CHECK(c.get("objects", cache::key({"empty"}))->empty());
    }
    cache::Cache again(sb.config());                       // a new process, in effect
    const auto got = again.get("objects", k);
    CB_CHECK(got.has_value() && *got == data);
    // objects is a shared kind: it is in the shared folder, not the state folder
    CB_CHECK(stdfs::exists(sb.config().shared_dir / "objects"));
    CB_CHECK(!stdfs::exists(sb.config().state_dir / "objects"));
});

CB_TEST("base.cache.put_replaces", [] {
    Sandbox sb;
    cache::Cache c(sb.config());
    c.put("k", "x", Bytes{1});
    c.put("k", "x", Bytes{2, 3});
    CB_CHECK(*c.get("k", "x") == (Bytes{2, 3}));
});

CB_TEST("base.cache.counts_found_and_missed_per_kind", [] {
    Sandbox sb;
    cache::Cache c(sb.config());
    c.get("a", "1");
    c.put("a", "1", Bytes{1});
    c.get("a", "1");
    c.get("a", "1");
    c.get("b", "1");
    const auto counts = c.counts();
    CB_CHECK_EQ(counts.at("a").found, std::uint64_t{2});
    CB_CHECK_EQ(counts.at("a").missed, std::uint64_t{1});
    CB_CHECK_EQ(counts.at("b").missed, std::uint64_t{1});
});

CB_TEST("base.cache.disabled_misses_and_keeps_nothing", [] {
    Sandbox sb;
    cache::Config config = sb.config();
    config.disabled = true;
    cache::Cache c(config);
    c.put("k", "x", Bytes{1});
    CB_CHECK(!c.get("k", "x").has_value());
    CB_CHECK(!stdfs::exists(config.state_dir));
    cb::fs::ScopedEnv env("CB_NO_CACHE", "1");
    CB_CHECK(cache::Config::disabled_by_environment());
    cb::fs::ScopedEnv off("CB_NO_CACHE", std::nullopt);
    CB_CHECK(!cache::Config::disabled_by_environment());
});

struct Verdict {
    std::uint8_t code = 0;
    std::string note;
    std::vector<std::pair<std::string, std::int64_t>> scores;
    std::optional<double> ratio;
    friend bool operator==(const Verdict&, const Verdict&) = default;
};

}  // namespace

template <>
struct cb::base::cache::Codec<Verdict> {
    static constexpr std::uint32_t kSchema = 3;
    static void write(io::ByteWriter& w, const Verdict& v) {
        w.u8(v.code);
        Codec<std::string>::write(w, v.note);
        Codec<decltype(v.scores)>::write(w, v.scores);
        Codec<decltype(v.ratio)>::write(w, v.ratio);
    }
    static Verdict read(io::ByteReader& r) {
        Verdict v;
        v.code = r.u8();
        v.note = Codec<std::string>::read(r);
        v.scores = Codec<decltype(v.scores)>::read(r);
        v.ratio = Codec<decltype(v.ratio)>::read(r);
        return v;
    }
};

namespace {

CB_TEST("base.cache.typed_values_round_trip", [] {
    Sandbox sb;
    cache::Cache c(sb.config());
    const Verdict v{7, "verified \xC3\xA9", {{"a", -1}, {"b", 1LL << 40}}, 0.25};
    c.keep("verdicts", "k1", v);
    c.keep("verdicts", "k2", Verdict{});
    CB_CHECK(c.value<Verdict>("verdicts", "k1") == v);
    CB_CHECK(c.value<Verdict>("verdicts", "k2") == Verdict{});
    CB_CHECK(!c.value<Verdict>("verdicts", "k3").has_value());
    c.keep("lists", "k", std::vector<std::string>{"x", "", "yz"});
    CB_CHECK((c.value<std::vector<std::string>>("lists", "k") == std::vector<std::string>{"x", "", "yz"}));
    c.keep("maps", "k", std::map<std::string, std::vector<std::int32_t>>{{"a", {1, -2}}, {"b", {}}});
    const auto m = c.value<std::map<std::string, std::vector<std::int32_t>>>("maps", "k");
    CB_CHECK(m.has_value() && m->at("a") == (std::vector<std::int32_t>{1, -2}) && m->at("b").empty());
    c.keep("scalars", "d", 3.5);
    CB_CHECK_EQ(*c.value<double>("scalars", "d"), 3.5);
    c.keep("scalars", "b", true);
    CB_CHECK_EQ(*c.value<bool>("scalars", "b"), true);
});

CB_TEST("base.cache.a_value_of_another_layout_is_a_miss", [] {
    Sandbox sb;
    cache::Cache c(sb.config());
    c.keep("v", "k", std::vector<std::int32_t>{1, 2, 3});
    CB_CHECK(!c.value<std::vector<std::int64_t>>("v", "k").has_value());   // a different schema number
    CB_CHECK(!c.value<std::string>("v", "k").has_value());
    CB_CHECK(c.value<std::vector<std::int32_t>>("v", "k").has_value());
    // raw bytes are schema 0
    c.put("v", "raw", Bytes{1, 2});
    CB_CHECK(!c.get("v", "raw", 5).has_value());
    CB_CHECK(c.get("v", "raw", 0).has_value());
});

CB_TEST("base.cache.damaged_entries_are_misses", [] {
    Sandbox sb;
    {
        cache::Cache c(sb.config());
        c.put("d", "good", Bytes{1, 2, 3});
        c.put("d", "bad_crc", Bytes{1, 2, 3});
        c.put("d", "short", Bytes{1, 2, 3});
    }
    // Reach into the file as another program or a bad disk might: flip a payload byte, cut one entry short, and
    // store something that is not an envelope at all.
    {
        const auto file = [&] {
            for (const auto& e : stdfs::directory_iterator(sb.config().state_dir / "d"))
                if (e.path().extension() == ".cbc") return e.path();
            return stdfs::path();
        }();
        auto db = cb::base::sqlite::Database::open(file);
        db.exec("UPDATE entry SET v = substr(v, 1, 16) || x'FF0203' WHERE k='bad_crc'");
        db.exec("UPDATE entry SET v = substr(v, 1, 17) WHERE k='short'");
        db.exec("INSERT INTO entry(k, v) VALUES('junk', x'0102')");
    }
    cache::Cache c(sb.config());
    CB_CHECK(c.get("d", "good").has_value());
    CB_CHECK(!c.get("d", "bad_crc").has_value());
    CB_CHECK(!c.get("d", "short").has_value());
    CB_CHECK(!c.get("d", "junk").has_value());
});

CB_TEST("base.cache.a_file_of_another_format_is_replaced", [] {
    Sandbox sb;
    cache::Config config = sb.config();
    {
        cache::Cache c(config);
        c.put("f", "k", Bytes{9});
    }
    const stdfs::path file = [&] {
        for (const auto& e : stdfs::directory_iterator(config.state_dir / "f")) return e.path();
        return stdfs::path();
    }();
    {   // a future (or foreign) format version
        auto db = cb::base::sqlite::Database::open(file);
        db.exec("PRAGMA user_version=99");
    }
    cache::Cache c(config);
    CB_CHECK(!c.get("f", "k").has_value());      // not read ...
    c.put("f", "k", Bytes{8});                   // ... and usable again
    CB_CHECK(*c.get("f", "k") == Bytes{8});
});

CB_TEST("base.cache.a_file_that_is_not_a_database_is_replaced", [] {
    Sandbox sb;
    cache::Config config = sb.config();
    cache::Cache probe(config);
    probe.put("g", "k", Bytes{1});
    probe.close();
    const stdfs::path file = [&] {
        for (const auto& e : stdfs::directory_iterator(config.state_dir / "g"))
            if (e.path().extension() == ".cbc") return e.path();
        return stdfs::path();
    }();
    cb::fs::write_text(file, "not a database, just text that is long enough to be mistaken for a header .....");
    cache::Cache c(config);
    // The kind is unusable or rebuilt; either way no exception reaches the caller.
    CB_CHECK_NOTHROW(c.get("g", "k"));
    CB_CHECK_NOTHROW(c.put("g", "k", Bytes{2}));
});

// ---- generations, scopes, files -----------------------------------------------------------------------------

CB_TEST("base.cache.another_generation_goes_when_the_kind_is_first_used", [] {
    Sandbox sb;
    cache::Config config = sb.config();
    {
        cache::Cache c(config);
        c.declare_kind("fp", {.generation = "rev 1"});
        c.put("fp", "k", Bytes{1});
    }
    {
        cache::Cache c(config);
        c.declare_kind("fp", {.generation = "rev 1"});
        CB_CHECK(c.get("fp", "k").has_value());    // the same code: still there
    }
    {
        cache::Cache c(config);
        c.declare_kind("fp", {.generation = "rev 2"});
        CB_CHECK(!c.get("fp", "k").has_value());   // the code changed: the old results are gone ...
        std::size_t files = 0;
        for (const auto& e : stdfs::directory_iterator(config.state_dir / "fp"))
            if (e.path().extension() == ".cbc") ++files;
        CB_CHECK_EQ(files, std::size_t{1});        // ... and so is their file
    }
});

CB_TEST("base.cache.shared_kinds_keep_other_generations", [] {
    Sandbox sb;
    cache::Config config = sb.config();
    {
        cache::Cache c(config);
        c.declare_kind("masm", {.scope = cache::Scope::Shared, .generation = "v1"});
        c.put("masm", "k", Bytes{1});
    }
    {
        cache::Cache c(config);
        c.declare_kind("masm", {.scope = cache::Scope::Shared, .generation = "v2"});   // another checkout's code
        CB_CHECK(!c.get("masm", "k").has_value());
        std::size_t files = 0;
        for (const auto& e : stdfs::directory_iterator(config.shared_dir / "masm"))
            if (e.path().extension() == ".cbc") ++files;
        CB_CHECK_EQ(files, std::size_t{2});
    }
});

CB_TEST("base.cache.path_kinds_have_a_folder_of_their_own", [] {
    Sandbox sb;
    cache::Cache c(sb.config());
    c.declare_kind("port-objects", {.generation = "g"});
    const std::string k = cache::key({"x"});
    const stdfs::path p = c.path("port-objects", k);
    CB_CHECK_EQ(p.filename().string(), k);
    CB_CHECK_EQ(p.parent_path().filename().string(), k.substr(0, 2));
    CB_CHECK(!stdfs::exists(p));                    // made when the tool writes it, not before
    CB_CHECK(c.path("port-objects", k) == p);
});

// ---- the cap -------------------------------------------------------------------------------------------------

CB_TEST("base.cache.hold_drops_the_least_recently_used_first", [] {
    Sandbox sb;
    cache::Config config = sb.config();
    config.max_bytes = 400 * 1000;
    cache::Cache c(config);
    // Fill past the cap without the cap being looked at (the first put looks once; later looks wait for 32 MB).
    c.put("lru", "first", Bytes{});
    c.hold();
    for (int i = 0; i < 8; ++i) c.put("lru", "k" + std::to_string(i), pattern(100000, i));
    // Say which were used when: k0 oldest ... k7 newest.
    const stdfs::path file = [&] {
        for (const auto& e : stdfs::directory_iterator(config.state_dir / "lru"))
            if (e.path().extension() == ".cbc") return e.path();
        return stdfs::path();
    }();
    {
        auto db = cb::base::sqlite::Database::open(file);
        for (int i = 0; i < 8; ++i)
            db.exec("UPDATE usage SET used=? WHERE k=?",
                    {cb::base::sqlite::Value(std::int64_t{1000 + i}), cb::base::sqlite::Value("k" + std::to_string(i))});
    }
    const std::uint64_t freed = c.hold();
    CB_CHECK(freed > 0);
    CB_CHECK(!c.get("lru", "k0").has_value());
    CB_CHECK(!c.get("lru", "k1").has_value());
    CB_CHECK(c.get("lru", "k7").has_value());
    CB_CHECK(c.get("lru", "k6").has_value());
    std::uint64_t total = 0;
    for (const auto& s : c.stats()) total += s.bytes;
    CB_CHECK_EQ(std::max(total, config.max_bytes), config.max_bytes);   // at or under the cap (shows the total if not)
});

CB_TEST("base.cache.hold_also_counts_files_a_tool_wrote", [] {
    Sandbox sb;
    cache::Config config = sb.config();
    config.max_bytes = 100 * 1000;
    cache::Cache c(config);
    c.declare_kind("objs", {.generation = "g"});
    for (int i = 0; i < 5; ++i) {
        const std::string k = cache::key({i});
        cb::fs::write_file(c.path("objs", k), pattern(60000, i));
    }
    CB_CHECK(c.hold() > 0);
    std::size_t left = 0;
    for (const auto& e : stdfs::recursive_directory_iterator(config.state_dir / "objs"))
        if (e.is_regular_file()) ++left;
    CB_CHECK(left < 5);
});

CB_TEST("base.cache.prune_stats_and_clear", [] {
    Sandbox sb;
    cache::Config config = sb.config();
    {
        cache::Cache c(config);
        c.declare_kind("fp", {.generation = "rev 1"});
        c.put("fp", "a", pattern(1000, 1));
        c.put("fp", "b", pattern(1000, 2));
    }
    cache::Cache c(config);
    c.declare_kind("fp", {.generation = "rev 2"});
    c.declare_kind("other", {});
    c.put("other", "x", Bytes{1});
    // The old generation's file is still there until the kind is used or pruned.
    auto before = c.stats();
    bool saw_old = false;
    for (const auto& s : before)
        if (s.kind == "fp") saw_old = s.generations.size() == 1 && s.current && s.generations[0] != *s.current;
    CB_CHECK(saw_old);
    const auto dropped = c.prune();
    CB_CHECK_EQ(dropped.size(), std::size_t{1});
    CB_CHECK_EQ(dropped[0].kind, std::string("fp"));
    CB_CHECK(!stdfs::exists(config.state_dir / "_dropped"));
    for (const auto& s : c.stats())
        if (s.kind == "other") CB_CHECK_EQ(s.results, std::uint64_t{1});
    const std::string report = cache::describe(c.stats(), config.max_bytes);
    CB_CHECK(report.find("kind") != std::string::npos);
    CB_CHECK(report.find("other") != std::string::npos);
    CB_CHECK(report.find("(cap 2.00 GB)") != std::string::npos);
    CB_CHECK(c.clear() > 0);
    CB_CHECK(c.stats().empty());
    CB_CHECK(!c.get("other", "x").has_value());   // and it works again afterwards
    c.put("other", "x", Bytes{1});
    CB_CHECK(c.get("other", "x").has_value());
});

CB_TEST("base.cache.workers_share_one_cache", [] {
    Sandbox sb;
    cache::Cache c(sb.config());
    std::vector<std::thread> threads;
    for (int t = 0; t < 4; ++t) {
        threads.emplace_back([&c, t] {
            for (int i = 0; i < 50; ++i) {
                const std::string k = "k" + std::to_string(t) + "-" + std::to_string(i);
                c.put("shared-use", k, pattern(200, static_cast<std::uint32_t>(i)));
                const auto got = c.get("shared-use", k);
                CB_CHECK(got.has_value() && got->size() == 200);
            }
        });
    }
    for (auto& th : threads) th.join();
    std::uint64_t results = 0;
    for (const auto& s : c.stats())
        if (s.kind == "shared-use") results = s.results;
    CB_CHECK_EQ(results, std::uint64_t{200});
});

}  // namespace
