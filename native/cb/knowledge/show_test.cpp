#include "cb/knowledge/show.hpp"

#include "cb/core/testing.hpp"

namespace {

namespace sh = cb::knowledge::show;
using cb::knowledge::Value;

// A knowledge base in memory: two kinds, a few keys, and the calls it was asked.
class FakeBase : public sh::Base {
public:
    std::string path() const override { return "state/db/knowledge.sqlite"; }
    std::string platform(std::string_view name) const override { return name == "ne" ? "win16" : std::string(name); }
    void load_authored() override { ++loads; }
    std::vector<sh::KindRow> kinds(std::optional<std::string_view> platform) override {
        asked_platform = platform ? std::string(*platform) : "(all)";
        std::vector<sh::KindRow> rows;
        rows.push_back({"win32", "prototype", 42, 12800, {{"learned", 2}, {"authored", 40}}, "headers/x.h"});
        rows.push_back({"win16", "api_model", 1000, 0, {}, ""});
        if (platform) std::erase_if(rows, [&](const sh::KindRow& r) { return r.platform != *platform; });
        return rows;
    }
    sh::SharedBodies shared_bodies() override { return {120, 80, 15}; }
    std::vector<std::string> first_keys(std::string_view, std::string_view, std::size_t limit, std::size_t& total) override {
        total = keys.size();
        return {keys.begin(), keys.begin() + static_cast<std::ptrdiff_t>(std::min(limit, keys.size()))};
    }
    std::optional<sh::Stored> value(std::string_view, std::string_view, std::string_view key) override {
        if (key == "GetDlgItem") {
            sh::Stored s;
            s.structured = Value(Value::Map{{"ret", Value("HWND")}, {"args", Value(Value::List{Value(1), Value(2.5), Value(true), Value()})}});
            return s;
        }
        if (key == "Blob") return sh::Stored{sh::Stored::Kind::Bytes, {0x0a, 0xff, 0x00}, {}, {}};
        if (key == "Text") return sh::Stored{sh::Stored::Kind::Text, {}, "caf\xC3\xA9 \"quoted\"", {}};
        if (key == "Long") {
            sh::Stored s;
            s.kind = sh::Stored::Kind::Text;
            s.text = std::string(30000, 'x');
            return s;
        }
        return std::nullopt;
    }
    std::optional<sh::Provenance> about(std::string_view, std::string_view, std::string_view key) override {
        if (key == "Unknown") return std::nullopt;
        return sh::Provenance{"headers/x.h", "authored", 86400.0 * 365, "abc123"};
    }

    int loads = 0;
    std::string asked_platform;
    std::vector<std::string> keys;
};

sh::Outcome run(FakeBase& base, std::vector<std::string> args) { return sh::run(base, args); }

CB_TEST("knowledge.show.listing_of_every_kind", [] {
    FakeBase base;
    const sh::Outcome out = run(base, {});
    CB_CHECK_EQ(out.exit_code, 0);
    // the format "  %-8s %-24s %7d  %8.1f KB  %-28s %s" (checked against it), origins in name order
    CB_CHECK_EQ(out.text,
                "state/db/knowledge.sqlite\n"
                "  win32    prototype                     42      12.5 KB  authored 40, learned 2       headers/x.h\n"
                "  win16    api_model                   1000       0.0 KB                               \n"
                "  (bodies) 120 recorded, 80 distinct, 15 in two or more modules\n");
    CB_CHECK_EQ(base.asked_platform, "(all)");
});

CB_TEST("knowledge.show.listing_of_one_platform_uses_the_base_name", [] {
    FakeBase base;
    const sh::Outcome out = run(base, {"ne"});
    CB_CHECK_EQ(base.asked_platform, "win16");   // the lane's name ne is the base's win16
    CB_CHECK(out.text.find("api_model") != std::string::npos);
    CB_CHECK(out.text.find("prototype") == std::string::npos);
});

CB_TEST("knowledge.show.a_long_source_is_clipped", [] {
    class LongSource : public FakeBase {
        std::vector<sh::KindRow> kinds(std::optional<std::string_view>) override {
            return {{"win32", "k", 1, 1024, {}, std::string(80, 's')}};
        }
    } base;
    const sh::Outcome out = run(base, {});
    CB_CHECK(out.text.find(std::string(72, 's') + "...\n") != std::string::npos);
    CB_CHECK(out.text.find(std::string(73, 's')) == std::string::npos);
});

CB_TEST("knowledge.show.load_loads_first_and_is_not_an_argument", [] {
    FakeBase base;
    const sh::Outcome out = run(base, {"--load", "ne"});
    CB_CHECK_EQ(base.loads, 1);
    CB_CHECK_EQ(base.asked_platform, "win16");
    CB_CHECK_EQ(out.exit_code, 0);
});

CB_TEST("knowledge.show.keys_of_a_kind_are_the_first_200_and_the_count", [] {
    FakeBase base;
    for (int i = 0; i < 250; ++i) base.keys.push_back("key" + std::to_string(i));
    const sh::Outcome out = run(base, {"win32", "prototype"});
    CB_CHECK(out.text.starts_with("key0\nkey1\n"));
    CB_CHECK(out.text.ends_with("key199\n(250)\n"));
    CB_CHECK(out.text.find("key200") == std::string::npos);
    FakeBase small;
    small.keys = {"a", "b"};
    CB_CHECK_EQ(run(small, {"win32", "prototype"}).text, "a\nb\n(2)\n");
    FakeBase none;
    CB_CHECK_EQ(run(none, {"win32", "prototype"}).text, "(0)\n");
});

CB_TEST("knowledge.show.one_piece", [] {
    FakeBase base;
    const sh::Outcome out = run(base, {"win32", "prototype", "GetDlgItem"});
    CB_CHECK_EQ(out.exit_code, 0);
    // 86400 * 365 seconds after the epoch is 1971-01-01 in UTC; the local time depends on the zone, so it is built the same way
    CB_CHECK_EQ(out.text,
                "{\n"
                " \"ret\": \"HWND\",\n"
                " \"args\": [\n"
                "  1,\n"
                "  2.5,\n"
                "  true,\n"
                "  null\n"
                " ]\n"
                "}\n"
                "source: headers/x.h\n"
                "origin: authored\n"
                "made:   " + sh::local_time_text(86400.0 * 365) + "\n"
                "hash:   abc123\n");
});

CB_TEST("knowledge.show.nothing_known_is_exit_1", [] {
    FakeBase base;
    const sh::Outcome out = run(base, {"win32", "prototype", "Unknown"});
    CB_CHECK_EQ(out.exit_code, 1);
    CB_CHECK_EQ(out.text, "nothing known: win32 prototype Unknown\n");
});

CB_TEST("knowledge.show.a_piece_with_provenance_but_no_value_shows_null", [] {
    FakeBase base;
    const sh::Outcome out = run(base, {"win32", "prototype", "Other"});
    CB_CHECK(out.text.starts_with("null\nsource: headers/x.h\n"));
});

CB_TEST("knowledge.show.dump_forms", [] {
    FakeBase base;
    CB_CHECK_EQ(sh::dump(*base.value("", "", "Blob")), "\"<3 bytes: 0aff00>\"");
    CB_CHECK_EQ(sh::dump(*base.value("", "", "Text")), "\"caf\xC3\xA9 \\\"quoted\\\"\"");   // UTF-8 kept as is, quotes escaped
    sh::Stored empty;
    empty.structured = Value(Value::List{});
    CB_CHECK_EQ(sh::dump(empty), "[]");
    empty.structured = Value(Value::Map{});
    CB_CHECK_EQ(sh::dump(empty), "{}");
    sh::Stored big;
    big.kind = sh::Stored::Kind::Bytes;
    big.bytes.assign(40, 0xab);
    std::string hex;
    for (int i = 0; i < 32; ++i) hex += "ab";   // only the first 32 bytes are previewed
    CB_CHECK_EQ(sh::dump(big), "\"<40 bytes: " + hex + "...>\"");
});

CB_TEST("knowledge.show.a_long_value_is_cut_at_20000_bytes", [] {
    FakeBase base;
    const sh::Outcome out = run(base, {"win32", "prototype", "Long"});
    // the value line is the first 20000 bytes of the JSON text, then the provenance on the next lines
    CB_CHECK(out.text.starts_with("\"" + std::string(19999, 'x') + "\nsource: "));
});

CB_TEST("knowledge.show.local_time_text_shape", [] {
    const std::string t = sh::local_time_text(0.0);
    CB_CHECK_EQ(t.size(), std::size_t{19});   // 1970-01-01 12:00 AM
    CB_CHECK(t[4] == '-' && t[7] == '-' && t[10] == ' ' && t[13] == ':' && t[16] == ' ');
    CB_CHECK(t.ends_with("AM") || t.ends_with("PM"));
});

}  // namespace
