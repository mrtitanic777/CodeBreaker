#include "cb/core/json.hpp"

#include "cb/core/json_struct.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;

// If json_fwd.hpp drifts from the vendored nlohmann header, this fails to compile.
static_assert(std::is_same_v<cb::Json, nlohmann::ordered_json>);
static_assert(std::is_same_v<nlohmann::ordered_json, nlohmann::basic_json<nlohmann::ordered_map>>);

CB_TEST("core.json.parse_and_dump", [] {
    Json j = json::parse(R"({"b": 1, "a": [1, 2.5, "x"], "c": null, "d": true})");
    CB_CHECK_EQ(j["b"].get<int>(), 1);
    CB_CHECK_EQ(j["a"].size(), 3u);
    CB_CHECK_EQ(json::dump(j, json::Style::Compact), R"({"b":1,"a":[1,2.5,"x"],"c":null,"d":true})");   // key order kept
    CB_CHECK_EQ(json::dump(Json::array({1, 2}), json::Style::Pretty), "[\n  1,\n  2\n]");
});

CB_TEST("core.json.parse_errors_are_parse_errors", [] {
    CB_CHECK_THROWS_AS(ParseError, json::parse("{"));
    CB_CHECK_THROWS_AS(ParseError, json::parse(""));
    try {
        (void)json::parse("{\n  \"a\": ,\n}");
    } catch (const ParseError& e) {
        CB_CHECK_EQ(e.line(), 2u);
        CB_CHECK(e.column() > 1);
    }
    Json out;
    CB_CHECK(json::try_parse("[1]", out));
    CB_CHECK(!json::try_parse("[1", out));
});

CB_TEST("core.json.python_compatible_layout", [] {
    Json j = json::parse(R"({"name": "café 😀", "n": [1, 2], "f": 1.0, "e": [], "o": {}})");
    CB_CHECK_EQ(json::dump(j, json::Style::PythonCompatible),
                "{\"name\": \"caf\\u00e9 \\ud83d\\ude00\", \"n\": [1, 2], \"f\": 1.0, \"e\": [], \"o\": {}}");   // non-ASCII is escaped
    Json p = json::parse(R"({"a": [1, {"b": 2}]})");
    CB_CHECK_EQ(json::dump(p, json::Style::PythonCompatiblePretty),
                "{\n  \"a\": [\n    1,\n    {\n      \"b\": 2\n    }\n  ]\n}");
    CB_CHECK_EQ(json::dump(Json("tab\tquote\"\n"), json::Style::PythonCompatible), R"("tab\tquote\"\n")");
});

CB_TEST("core.json.file_roundtrip", [] {
    fs::ScopedTempDir dir("cbjson");
    Json j = Json::object();
    j["z"] = 1;
    j["a"] = "caf\xC3\xA9";
    json::save(dir.path() / "x" / "doc.json", j);
    Json back = json::load(dir.path() / "x" / "doc.json");
    CB_CHECK_EQ(back["a"].get<std::string>(), "caf\xC3\xA9");
    CB_CHECK_EQ(json::dump(back, json::Style::Compact), json::dump(j, json::Style::Compact));
    CB_CHECK_THROWS_AS(NotFoundError, json::load(dir.path() / "missing.json"));
    fs::write_text(dir.path() / "bad.json", "{oops");
    try {
        (void)json::load(dir.path() / "bad.json");
        CB_CHECK(false);
    } catch (const ParseError& e) {
        CB_CHECK(e.file().find("bad.json") != std::string::npos);
    }
    fs::write_text(dir.path() / "bom.json", "\xEF\xBB\xBF{\"ok\": true}");
    CB_CHECK(json::load(dir.path() / "bom.json")["ok"].get<bool>());
});

namespace {
struct Point {
    int x = 0;
    int y = 0;
};
CB_JSON_STRUCT(Point, x, y)
}  // namespace

CB_TEST("core.json.typed_struct_edge", [] {
    Point p = json::parse(R"({"x": 3, "y": -4})").get<Point>();
    CB_CHECK_EQ(p.x, 3);
    CB_CHECK_EQ(p.y, -4);
    CB_CHECK_EQ(json::dump(Json(p), json::Style::Compact), R"({"x":3,"y":-4})");
});
