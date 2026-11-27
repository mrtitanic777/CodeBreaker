// cb-unity: alone
#include <chrono>
#include <format>

#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/testing.hpp"
#include "store/function_store.hpp"

// A module the size of a big program: 20,000 functions recorded in one call, then read back. The time is printed so a
// slowdown shows up in a run; the check only guards against a store that takes minutes.
CB_TEST("store.function_store.a_big_module_is_recorded_and_read_in_seconds", [] {
    using namespace cb;
    using cb::store::Record;
    fs::ScopedTempDir tmp("function-store-scale");
    store::FunctionStore store(tmp.path() / "functions.sqlite", tmp.path() / "db");
    store.add_module("big", "big.dll", "", "pe");
    std::vector<Record> rows;
    for (int i = 0; i < 20000; ++i) {
        Record r;
        r.addr = std::format("1:{:x}", i);
        r.name = std::format("_f{}", i);
        r.source = "int f(void) { return 0; }";
        r.sig = std::format("sig{}", i % 97);
        r.verdict = i % 5 == 0 ? "divergent" : "equivalent";
        rows.push_back(std::move(r));
    }
    const auto t0 = std::chrono::steady_clock::now();
    store.record_many("big", "decompile", "r1", "p", rows);
    const auto t1 = std::chrono::steady_clock::now();
    const auto best = store.best("big", "p");
    const auto tally = store.tally("p", "big");
    const auto t2 = std::chrono::steady_clock::now();
    CB_CHECK_EQ(best.size(), 20000u);
    CB_CHECK_EQ(tally.at(store::Class::Verified), 16000);
    CB_CHECK_EQ(tally.at(store::Class::Unverified), 4000);
    CB_CHECK_EQ(store.summaries().at("big").judged, 20000);
    const double write_s = std::chrono::duration<double>(t1 - t0).count(), read_s = std::chrono::duration<double>(t2 - t1).count();
    err("function store, 20,000 functions: record_many {:.2f} s, best + tally {:.2f} s\n", write_s, read_s);
    CB_CHECK(write_s < 60 && read_s < 60);
});
