#include "cb/compat/py_bridge.hpp"

#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::compat;

CB_TEST("compat.py_bridge.text_and_bytes", [] {
    py::str s = to_py(std::string_view("caf\xC3\xA9"));
    CB_CHECK_EQ(s.utf8(), "caf\xC3\xA9");
    CB_CHECK_EQ(from_py(s), "caf\xC3\xA9");
    py::bytes b = to_py(ByteView(Bytes{0, 1, 255, 0}));
    CB_CHECK_EQ(b.size(), 4);
    CB_CHECK(from_py(b) == (Bytes{0, 1, 255, 0}));    // embedded NULs and high bytes survive
    CB_CHECK_EQ(from_py(to_py(std::string("x"))), "x");
});

CB_TEST("compat.py_bridge.integers", [] {
    CB_CHECK_EQ(to_py(std::uint32_t{0xFFFFFFFFu}), 0xFFFFFFFFll);
    CB_CHECK_EQ(to_py(-5), -5);
    CB_CHECK_THROWS_AS(py::OverflowError, to_py(std::uint64_t{0xFFFFFFFFFFFFFFFFull}));
    CB_CHECK_EQ(int_from_py<std::uint16_t>(65535), 65535);
    CB_CHECK_THROWS_AS(py::OverflowError, int_from_py<std::uint16_t>(65536));
    CB_CHECK_THROWS_AS(py::OverflowError, int_from_py<std::uint32_t>(-1));
});

CB_TEST("compat.py_bridge.vectors_and_nested", [] {
    std::vector<std::string> names = {"a", "bb", ""};
    py::list<py::str> l = to_py(names);
    CB_CHECK_EQ(l.vec().size(), 3u);
    CB_CHECK(from_py(l) == names);
    std::vector<std::vector<std::int32_t>> grid = {{1, 2}, {}, {3}};
    auto pg = to_py(grid);
    CB_CHECK_EQ(pg.vec()[0].vec().size(), 2u);
    CB_CHECK(from_py(pg) == (std::vector<std::vector<std::int64_t>>{{1, 2}, {}, {3}}));
});

CB_TEST("compat.py_bridge.dicts_keep_order", [] {
    OrderedMap<std::string, std::uint32_t> m;
    m["zeta"] = 1;
    m["alpha"] = 2;
    auto d = to_py(m);
    auto items = d.items();
    CB_CHECK_EQ(items.size(), 2u);
    CB_CHECK_EQ(std::get<0>(items[0]).utf8(), "zeta");    // insertion order, not sorted
    auto back = from_py(d);
    CB_CHECK_EQ(back.entries()[0].first, "zeta");
    CB_CHECK_EQ(back.at("alpha"), 2);
    std::map<std::string, std::int64_t> sorted = {{"b", 1}, {"a", 2}};
    CB_CHECK_EQ(std::get<0>(to_py(sorted).items()[0]).utf8(), "a");   // a std::map goes over in its sorted order
    std::unordered_map<std::string, std::int64_t> hashed = {{"q", 1}, {"p", 2}};
    CB_CHECK_EQ(std::get<0>(to_py(hashed).items()[0]).utf8(), "p");   // deterministic: sorted
});

CB_TEST("compat.py_bridge.sets_optionals_tuples", [] {
    std::set<std::string> s = {"x", "y"};
    auto ps = to_py(s);
    CB_CHECK_EQ(ps.size(), 2);
    CB_CHECK(from_py(ps) == s);
    std::optional<std::string> some = "v", none;
    CB_CHECK(to_py(some).has_value());
    CB_CHECK(!to_py(none).has_value());
    CB_CHECK_EQ(from_py(to_py(some)).value(), "v");
    auto t = to_py(std::make_pair(std::string("k"), std::uint8_t{7}));
    CB_CHECK_EQ(std::get<0>(t).utf8(), "k");
    CB_CHECK_EQ(std::get<1>(t), 7);
});

CB_TEST("compat.py_bridge.errors_become_the_matching_py_exception", [] {
    CB_CHECK_THROWS_AS(py::StructError, rethrow_as_py(TruncatedError("r", 0, 4, 1)));
    CB_CHECK_THROWS_AS(py::ValueError, rethrow_as_py(FormatError("r", 0, "bad")));
    CB_CHECK_THROWS_AS(py::FileNotFoundError, rethrow_as_py(NotFoundError("p", 2)));
    CB_CHECK_THROWS_AS(py::PermissionError, rethrow_as_py(AccessError("p", 5)));
    CB_CHECK_THROWS_AS(py::OSError, rethrow_as_py(IoError("p", 1, "x")));
    CB_CHECK_THROWS_AS(py::TimeoutError, rethrow_as_py(TimeoutError("t", "")));
    CB_CHECK_THROWS_AS(py::RuntimeError, rethrow_as_py(ToolError("t", 1, "")));
    CB_CHECK_THROWS_AS(py::ValueError, rethrow_as_py(ParseError("f", 1, 1, "x")));
    CB_CHECK_THROWS_AS(py::AssertionError, rethrow_as_py(InternalError("bug")));
    CB_CHECK_THROWS_AS(py::NotImplementedError, rethrow_as_py(UnsupportedError("r", 0, "v2")));
    // guard: new code called from an old caller
    CB_CHECK_EQ(guard([] { return 41 + 1; }), 42);
    CB_CHECK_THROWS_AS(py::ValueError, guard([]() -> int { throw FormatError("r", 0, "bad"); }));
    CB_CHECK_NOTHROW(guard([] {}));
});
