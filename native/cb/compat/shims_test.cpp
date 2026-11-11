// Tests of the staged shims under compat/shims/: the old API over the new modules. Built at /W3 (the old layer is
// not warning-clean), like py_bridge_test.cpp.
#include "shims/codebreaker/jobs/workers.hpp"
#include "shims/codebreaker/knowledge/reads.hpp"
#include "shims/codebreaker/platforms/decoder.hpp"

#include "cb/core/testing.hpp"

namespace old_reads = codebreaker::knowledge::reads;
namespace old_workers = codebreaker::jobs::workers;
namespace old_platforms = codebreaker::platforms;

// ---- knowledge/reads --------------------------------------------------------------------------------------------
CB_TEST("compat.shims.reads_keys_and_recording", [] {
    CB_CHECK_EQ(old_reads::KIND.utf8(), "kb");
    CB_CHECK_EQ(old_reads::ABSENT.utf8(), "absent");
    CB_CHECK_EQ(old_reads::join(py::str("win32"), py::str("prototype"), py::str("GetDlgItem")).utf8(), "win32:prototype:GetDlgItem");
    auto [p, k, key] = old_reads::split(py::str("a:b:c:d"));
    CB_CHECK_EQ(p.utf8(), "a");
    CB_CHECK_EQ(k.utf8(), "b");
    CB_CHECK_EQ(key.utf8(), "c:d");
    CB_CHECK_THROWS_AS(py::ValueError, (void)old_reads::split(py::str("a:b")));   // the old harness's ValueError

    CB_CHECK(!old_reads::active());
    old_reads::record(py::str("win32"), py::str("prototype"), py::str("outside"));      // nothing outside a recording
    {
        old_reads::recording r;
        CB_CHECK(old_reads::active());
        old_reads::record(py::str("win32"), py::str("prototype"), py::str("GetDlgItem"));
        old_reads::record(py::str("win16"), py::str("struct"), py::str("RECT"));
        old_reads::record_kind(py::str("win32"), py::str("signature"));
        old_reads::merge(py::list<py::str>{py::str("bodies:name:abc")});
        auto sorted = r.sorted_keys();
        CB_CHECK_EQ(sorted.size(), 4);
        CB_CHECK_EQ(sorted[0].utf8(), "bodies:name:abc");
        CB_CHECK_EQ(sorted[3].utf8(), "win32:signature:*");
        CB_CHECK_EQ(r.reads().size(), 4);
        CB_CHECK(r.reads().contains(py::str("win16:struct:RECT")));
        CB_CHECK_EQ(r.keys().size(), 4);
        // the captured recording is the same set, seen from another context
        auto asker = old_reads::capture();
        CB_CHECK(asker != nullptr);
        {
            old_reads::Adopt adopt(asker);
            old_reads::record(py::str("a"), py::str("k"), py::str("again"));
        }
        CB_CHECK(r.reads().contains(py::str("a:k:again")));
    }
    CB_CHECK(!old_reads::active());
});

CB_TEST("compat.shims.reads_carry_and_digest", [] {
    old_reads::Carry<int, int, int> work([](const int& ctx, const int& item) {
        old_reads::record(py::str("win16"), py::str("api_model"), py::str("GLOBALLOCK"));
        return ctx + item;
    });
    auto carried = work(40, 2);
    CB_CHECK_EQ(carried.value, 42);
    CB_CHECK_EQ(carried.keys.size(), 1);
    old_reads::recording asker;
    CB_CHECK_EQ(old_reads::unwrap(carried), 42);
    CB_CHECK(asker.reads().contains(py::str("win16:api_model:GLOBALLOCK")));
    CB_CHECK_THROWS_AS(py::RuntimeError, (void)old_reads::digest(py::str("a:b:c")));
    old_reads::digest_impl() = [](const py::str& r) { return py::str("h(" + r.utf8() + ")"); };
    CB_CHECK_EQ(old_reads::digest(py::str("a:b:c")).utf8(), "h(a:b:c)");
    old_reads::digest_impl() = nullptr;
});

// ---- jobs/workers -------------------------------------------------------------------------------------------------
CB_TEST("compat.shims.workers_text", [] {
    CB_CHECK_EQ(old_workers::hex(""), "-");
    CB_CHECK_EQ(old_workers::hex("ab"), "6162");
    CB_CHECK_EQ(old_workers::unhex("-"), "");
    CB_CHECK_EQ(old_workers::unhex("6162"), "ab");
    old_workers::Enc e;
    e.i(-3).s("hi ").s("");
    CB_CHECK_EQ(e.text(), "-3 686920 -");
    old_workers::Dec d(e.text());
    CB_CHECK_EQ(d.i(), -3);
    CB_CHECK_EQ(d.s(), "hi ");
    CB_CHECK_EQ(d.s(), "");
    const std::string dir = old_workers::scratch_folder();
    old_workers::write_file(dir + "\\x.txt", "payload");
    CB_CHECK_EQ(old_workers::read_file(dir + "\\x.txt"), "payload");
    std::filesystem::remove_all(dir);
});

CB_TEST("compat.shims.workers_run_a_real_worker", [] {
    CB_CHECK(!old_workers::enabled());
    const auto self = cb::jobs::WorkerLauncher::for_this_program();
    old_workers::launcher() = cb::jobs::WorkerLauncher(self.exe().parent_path() / "cb_worker_probe.exe");
    CB_CHECK(old_workers::enabled());
    CB_CHECK_EQ(old_workers::run({"exit", "3"}), 3);
    CB_CHECK_EQ(old_workers::run({"exit", "0"}), 0);
    old_workers::launcher() = cb::jobs::WorkerLauncher(std::filesystem::path());
    CB_CHECK(!old_workers::enabled());
});

// ---- platforms/decoder --------------------------------------------------------------------------------------------
CB_TEST("compat.shims.decoder", [] {
    auto md = old_platforms::decoder(16, py::str("x86"), false);
    CB_CHECK(md.get() == old_platforms::decoder(16, py::str("x86"), false).get());     // one per thread and option set
    CB_CHECK(md.get() != old_platforms::decoder(32, py::str("x86"), false).get());
    auto insns = md->disasm(py::bytes(std::string("\x55\x8B\xEC\xCB", 4)), 0x100);
    CB_CHECK_EQ(insns.size(), 3);
    CB_CHECK_EQ(insns[0].mnemonic.utf8(), "push");
    CB_CHECK_EQ(insns[0].address, 0x100);
    CB_CHECK_EQ(insns[1].op_str.utf8(), "bp, sp");
    CB_CHECK_EQ(insns[2].bytes.size(), 1);
    CB_CHECK_EQ(md->bits, 16);
    CB_CHECK_EQ(md->arch.utf8(), "x86");

    auto detailed = old_platforms::decoder(32, py::str("x86"), true);
    int seen = 0;
    detailed->each(std::string("\x8B\x44\x24\x04", 4), 0, 0, [&](const cs_insn& c) {
        ++seen;
        CB_CHECK_EQ(c.detail->x86.op_count, 2);
        auto acc = detailed->regs_access(c);
        CB_CHECK(acc.has_value());
        CB_CHECK_EQ(std::string(detailed->reg_name(std::get<1>(*acc)[0])), "eax");
        return true;
    });
    CB_CHECK_EQ(seen, 1);
    // the raw records, walked by the caller
    size_t n = 0;
    const uint8_t code[] = {0x90, 0x90};
    cs_insn* raw = detailed->raw(code, 2, 0, 0, n);
    CB_CHECK_EQ(n, 2u);
    detailed->release(raw, n);

    CB_CHECK_EQ(old_platforms::decoder(32, py::str("ppc"), false)->disasm(py::bytes(std::string("\x4E\x80\x00\x20", 4)), 0)[0].mnemonic.utf8(), "blr");
    CB_CHECK_THROWS_AS(py::ValueError, (void)old_platforms::decoder(16, py::str("mips"), false));
    CB_CHECK_THROWS_AS(py::KeyError, (void)old_platforms::decoder(7, py::str("x86"), false));
});
