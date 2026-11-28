// Tests of the console writer's own parts: the head templates, and a platform with no description.

#include "source/console_writer.hpp"

#include "core/error.hpp"
#include "core/testing.hpp"

namespace cb::source::console_writer {
namespace {

CB_TEST("source.console_writer.a_template_says_values_numbers_and_choices", [] {
    const Values v = {{"file", std::string("game.col")}, {"start", std::int64_t{0x8030}}, {"on", std::int64_t{1}}, {"off", std::int64_t{0}},
                      {"name", std::string("DEMO")}};
    CB_CHECK_EQ(fill("cart {file} start ${start:04X}", v), "cart game.col start $8030");
    CB_CHECK_EQ(fill("{on?$AA55: yes|no}/{off?yes|$55AA: no}", v), "$AA55: yes/$55AA: no");
    CB_CHECK_EQ(fill("x{on? (file {name})|}y{off? (file {name})|}", v), "x (file DEMO)y");
    CB_CHECK_THROWS_AS(UsageError, fill("{absent}", v));
    CB_CHECK_THROWS_AS(UsageError, fill("{file", v));
});

CB_TEST("source.console_writer.a_platform_with_no_description_is_refused", [] {
    CB_CHECK_THROWS_AS(UsageError, describe("no-such-console"));
    CB_CHECK_EQ(describe("coleco").shape, "z80-block");
    CB_CHECK_EQ(describe("coleco").dialect, "sjasmplus");
});

}  // namespace
}  // namespace cb::source::console_writer
