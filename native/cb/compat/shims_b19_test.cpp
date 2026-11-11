// Tests of the staged shims for cb/jobs/pool and cb/platforms/{win16,vxd}: the old API over the new modules. Built at /W3
// (the old layer is not warning-clean), like shims_test.cpp.
#include "shims/codebreaker/jobs/pool.hpp"
#include "shims/codebreaker/platforms/vxd.hpp"
#include "shims/codebreaker/platforms/win16.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace old_pool = codebreaker::jobs::pool;
namespace old_win16 = codebreaker::platforms::win16;
namespace old_vxd = codebreaker::platforms::vxd;

namespace {
int64_t sq(const int64_t& ctx, const int64_t& i) { return i * i - ctx; }
}  // namespace

CB_TEST("compat.shims.pool_run_threads_as_done", [] {
    py::list<int64_t> items;
    for (int64_t i = 0; i < 40; ++i) items.append(i);
    old_pool::Options<int64_t, int64_t> o;
    o.jobs = 4;
    o.small = 1;
    o.size = 3;
    auto got = old_pool::run_first(sq, items, int64_t(7), o);
    CB_CHECK_EQ(got.size(), 40);
    CB_CHECK_EQ(got.at(6), 29);

    auto squares = old_pool::threads([](const int64_t& n) { return n * n; }, items, 4);
    CB_CHECK_EQ(squares.size(), 40);
    CB_CHECK_EQ(squares[7], 49);

    int64_t sum = 0;
    old_pool::as_done([](const int64_t& n) { return n; }, items, [&](const int64_t&, const int64_t& r) { sum += r; });
    CB_CHECK_EQ(sum, 780);

    // a work function that throws on one item: crashed(item)
    old_pool::Options<int64_t, int64_t> c = o;
    c.crashed = [](const int64_t& i) { return -i; };
    auto crashed = old_pool::run_first([](const int64_t&, const int64_t& i) -> int64_t { if (i == 13) throw py::ValueError("bad 13"); return i * 2; }, items, int64_t(0), c);
    CB_CHECK_EQ(crashed.at(13), -13);
    CB_CHECK_EQ(crashed.at(14), 28);

    // an ImportError is a broken setup: it leaves run()
    CB_CHECK_THROWS_AS(py::ImportError, (void)old_pool::run_first([](const int64_t&, const int64_t& i) -> int64_t { if (i == 5) throw py::ImportError("no module"); return i; }, items, int64_t(0), c));

    // threads rethrows an item's exception
    CB_CHECK_THROWS_AS(py::ValueError, (void)old_pool::threads([](const int64_t& n) -> int64_t { if (n == 3) throw py::ValueError("three"); return n; }, items, 2));
});

CB_TEST("compat.shims.pool_isolated_old_api", [] {
    cb::fs::ScopedTempDir dir("cbshimpool");
    const std::string script = cb::fs::to_text(dir.path() / "child.cmd");
    const std::string text = "@echo off\r\nif \"%1\"==\"crash\" exit /b 3\r\necho {\"item\":\"%1\"}\r\n";
    cb::fs::write_file(script, cb::as_bytes(text));
    py::list<py::str> items;
    for (const char* n : {"ok1", "crash"}) items.append(py::str(n));
    std::function<py::list<py::str>(const py::str&)> argv_for = [&](const py::str& it) {
        py::list<py::str> argv;
        for (const std::string& a : {std::string("cmd"), std::string("/c"), script, it.utf8()}) argv.append(py::str(a));
        return argv;
    };
    old_pool::IsolatedOptions<py::str> o;
    o.key = [](const py::str& s) { return s; };
    o.timeout = 5;
    o.jobs = 2;
    int seen = 0;
    o.on_result = [&](const py::str&, const old_pool::Result&, bool) { ++seen; };
    auto out = old_pool::isolated<py::str>(argv_for, items, o);
    CB_CHECK_EQ(seen, 2);
    CB_CHECK_EQ(out.ran, 2);
    CB_CHECK_EQ(out.done.size(), 1);
    CB_CHECK(out.done.contains(py::str("ok1")));
    CB_CHECK_EQ(out.done.at(py::str("ok1")).at(py::str("item")).s.utf8(), "ok1");
    CB_CHECK_EQ(out.failed.at(py::str("crash")).at(py::str("error")).s.utf8(), "no-output");
    CB_CHECK_EQ(out.failed.at(py::str("crash")).at(py::str("tries")).i, 1);
});

CB_TEST("compat.shims.win16_platform_record", [] {
    const old_win16::Platform& p = old_win16::PLATFORM();
    CB_CHECK_EQ(p.name.utf8(), "ne");
    CB_CHECK_EQ(p.title.utf8(), "16-bit Windows");
    CB_CHECK_EQ(p.source.utf8(), "c or asm");
    CB_CHECK_EQ(p.lane.utf8(), "codebreaker.verify.lanes.win16");
    CB_CHECK_EQ(p.steps.utf8(), "codebreaker.engine.win16");
    CB_CHECK_EQ(p.cpu.language.utf8(), "x86:LE:16:Protected Mode:default");
    CB_CHECK_EQ(p.cpu.bits, 16);
    CB_CHECK_EQ(p.cpu.registers.size(), 12);
    CB_CHECK_EQ(p.convention.arguments, 8);
    CB_CHECK_EQ(p.convention.word, 2);
    CB_CHECK_EQ(p.convention.order.utf8(), "pascal");
    CB_CHECK_EQ(p.convention.pops.utf8(), "either");
    CB_CHECK_EQ(p.convention.preserved.size(), 3);
    CB_CHECK_EQ(p.loader.container.utf8(), "NE");
    CB_CHECK_EQ(p.emulator.run.module.utf8(), "codebreaker.emulate.windows16");
    CB_CHECK_EQ(p.packager.link.name.utf8(), "_build16");
    CB_CHECK_EQ(p.compiler.toolchain.utf8(), "openwatcom");
});

CB_TEST("compat.shims.win16_call", [] {
    std::vector<int64_t> seen;
    old_win16::Runner ok = [&](const py::str&, int64_t, const py::list<int64_t>& args) {
        for (int64_t i = 0; i < args.size(); ++i) seen.push_back(args[i]);
        old_win16::RunRecord r;
        r.ended = py::str("returned");
        r.results.set(py::str("AX"), 9);
        return r;
    };
    auto [ended, ax] = old_win16::call(py::str("x.dll"), 3, ok);
    CB_CHECK_EQ(ended.utf8(), "returned");
    CB_CHECK_EQ(*ax, 9);
    CB_CHECK_EQ(seen.size(), 8);
    old_win16::Runner fails = [](const py::str&, int64_t, const py::list<int64_t>&) -> old_win16::RunRecord { throw py::ValueError("x"); };
    auto [ended2, ax2] = old_win16::call(py::str("x.dll"), 3, fails);
    CB_CHECK_EQ(ended2.utf8(), "error: ValueError");
    CB_CHECK(!ax2);
    old_win16::Runner no_import = [](const py::str&, int64_t, const py::list<int64_t>&) -> old_win16::RunRecord { throw py::ImportError("m"); };
    CB_CHECK_THROWS_AS(py::ImportError, (void)old_win16::call(py::str("x.dll"), 3, no_import));
});

CB_TEST("compat.shims.vxd_platform_record_and_entries", [] {
    const old_vxd::Platform& p = old_vxd::PLATFORM();
    CB_CHECK_EQ(p.name.utf8(), "le");
    CB_CHECK_EQ(p.lane.utf8(), "codebreaker.verify.lanes.driver");
    CB_CHECK_EQ(p.cpu.language.utf8(), "x86:LE:32:default:windows");
    CB_CHECK_EQ(p.convention.arguments.utf8(), "registers");
    CB_CHECK_EQ(p.convention.results.size(), 3);
    CB_CHECK_EQ(p.convention.pops.utf8(), "callee");
    CB_CHECK_EQ(p.compiler.toolchain.utf8(), "openwatcom wasm, the DDK's macros");
    CB_CHECK_EQ(p.packager.writer.module.utf8(), "codebreaker.source.driver_writer");
    auto entries = old_vxd::entries(py::str(cb::fs::to_text(cb::testing::data_path("formats/testdata/splitter.vxd"))));
    CB_CHECK(entries.contains(py::str("control")));
    CB_CHECK_EQ(std::get<1>(entries.at(py::str("control"))), 0x99);
    CB_CHECK_THROWS_AS(py::ValueError, (void)old_vxd::entries(py::str(cb::fs::to_text(cb::testing::data_path("formats/testdata/lzexpand.dll")))));
});
