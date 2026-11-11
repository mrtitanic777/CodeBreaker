// cb/emulate/package_test.cpp -- the runner table.
#include "cb/emulate/package.hpp"

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::emulate;

CB_TEST("emulate.package.runner_table_finds_by_kind", [] {
    RunnerTable t;
    t.add("pe", {"emulator", "recorded"});
    t.add("ne", {"windows16", "recorded"});
    CB_CHECK_EQ(t.at("pe").module, "emulator");
    CB_CHECK_EQ(t.at("ne").function, "recorded");
    CB_CHECK(t.find("le") == nullptr);
    CB_CHECK_THROWS_AS(UsageError, (void)t.at("le"));
});

CB_TEST("emulate.package.kinds_keep_the_order_they_were_added_in", [] {
    RunnerTable t;
    t.add("pe", {"emulator", "recorded"});
    t.add("le", {"driver", "recorded"});
    t.add("pe", {"emulator", "recorded2"});                       // added again: replaced in place
    CB_CHECK(t.kinds() == (std::vector<std::string>{"pe", "le"}));
    CB_CHECK_EQ(t.at("pe").function, "recorded2");
    CB_CHECK(RunnerTable{}.kinds().empty());
});
