#include "cb/core/ordered_map.hpp"

#include <map>
#include <string>

#include "cb/core/intmath.hpp"
#include "cb/core/testing.hpp"

using namespace cb;

CB_TEST("core.ordered_map.keeps_insertion_order", [] {
    OrderedMap<std::string, int> m;
    m["zeta"] = 1;
    m["alpha"] = 2;
    m["mid"] = 3;
    std::string order;
    for (const auto& [k, v] : m) order += k + std::to_string(v) + " ";
    CB_CHECK_EQ(order, "zeta1 alpha2 mid3 ");
    CB_CHECK_EQ(m.size(), 3u);
});

CB_TEST("core.ordered_map.reassign_keeps_position", [] {
    OrderedMap<std::string, int> m;
    m.insert_or_assign("a", 1);
    m.insert_or_assign("b", 2);
    m.insert_or_assign("a", 10);
    CB_CHECK_EQ(m.size(), 2u);
    CB_CHECK_EQ(m.entries()[0].first, "a");
    CB_CHECK_EQ(m.entries()[0].second, 10);
    auto [it, fresh] = m.try_emplace("b", 99);
    CB_CHECK(!fresh);
    CB_CHECK_EQ(it->second, 2);   // try_emplace leaves an existing entry alone
});

CB_TEST("core.ordered_map.lookup", [] {
    OrderedMap<std::string, int> m{{"x", 1}, {"y", 2}};
    CB_CHECK(m.contains("x"));
    CB_CHECK(!m.contains("z"));
    CB_CHECK_EQ(*m.find("y"), 2);
    CB_CHECK(m.find("z") == nullptr);
    CB_CHECK_EQ(m.at("x"), 1);
    CB_CHECK_THROWS_AS(std::out_of_range, m.at("z"));   // absent is a bug when at() is used
    CB_CHECK_EQ(get_or(m, std::string("z"), 7), 7);
    CB_CHECK_EQ(get_or(m, std::string("x"), 7), 1);
    CB_CHECK(find_ptr(m, std::string("x")) != nullptr);
    CB_CHECK(find_ptr(m, std::string("q")) == nullptr);
});

CB_TEST("core.ordered_map.erase_renumbers", [] {
    OrderedMap<int, std::string> m;
    for (int i = 0; i < 5; ++i) m[i] = std::to_string(i);
    CB_CHECK(m.erase(1));
    CB_CHECK(!m.erase(1));
    CB_CHECK(!m.contains(1));
    CB_CHECK_EQ(m.at(4), "4");          // later keys still find the right slot
    CB_CHECK_EQ(m.at(2), "2");
    m[9] = "nine";
    std::string order;
    for (const auto& kv : m) order += std::to_string(kv.first);
    CB_CHECK_EQ(order, "02349");
    m.clear();
    CB_CHECK(m.empty());
});

CB_TEST("core.ordered_map.get_or_on_std_map", [] {
    std::map<std::string, int> m{{"a", 1}};
    CB_CHECK_EQ(get_or(m, std::string("a"), 0), 1);
    CB_CHECK_EQ(get_or(m, std::string("b"), 5), 5);
    CB_CHECK(find_ptr(m, std::string("a")) != nullptr);
});

CB_TEST("core.intmath.floor_div_mod", [] {
    CB_CHECK_EQ(floor_div(7, 2), 3);
    CB_CHECK_EQ(floor_div(-7, 2), -4);
    CB_CHECK_EQ(floor_div(7, -2), -4);
    CB_CHECK_EQ(floor_div(-7, -2), 3);
    CB_CHECK_EQ(floor_div(-8, 2), -4);
    CB_CHECK_EQ(floor_mod(7, 3), 1);
    CB_CHECK_EQ(floor_mod(-7, 3), 2);
    CB_CHECK_EQ(floor_mod(7, -3), -2);
    CB_CHECK_EQ(floor_mod(-6, 3), 0);
    for (int a = -20; a <= 20; ++a) {
        for (int b : {-7, -3, -1, 1, 2, 5}) CB_CHECK_EQ(floor_div(a, b) * b + floor_mod(a, b), a);
    }
});

CB_TEST("core.intmath.checked", [] {
    CB_CHECK_EQ(*checked_add<std::uint32_t>(1, 2), 3u);
    CB_CHECK(!checked_add<std::uint32_t>(0xFFFFFFFFu, 1).has_value());
    CB_CHECK(!checked_add<std::int32_t>(INT32_MAX, 1).has_value());
    CB_CHECK(!checked_add<std::int32_t>(INT32_MIN, -1).has_value());
    CB_CHECK_EQ(*checked_add<std::int32_t>(INT32_MIN, 1), INT32_MIN + 1);
    CB_CHECK_EQ(*checked_mul<std::uint64_t>(1ull << 31, 2), 1ull << 32);
    CB_CHECK(!checked_mul<std::uint64_t>(1ull << 32, 1ull << 32).has_value());
    CB_CHECK(!checked_mul<std::int32_t>(INT32_MIN, -1).has_value());
    CB_CHECK(!checked_mul<std::int32_t>(65536, 65536).has_value());
    CB_CHECK_EQ(*checked_mul<std::int32_t>(-3, 5), -15);
    CB_CHECK_EQ(*checked_mul<std::int32_t>(0, INT32_MAX), 0);
    CB_CHECK_EQ(*checked_mul<std::int32_t>(-1, INT32_MAX), -INT32_MAX);
});
