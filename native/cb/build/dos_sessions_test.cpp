// Tests of the DOSBox+MASM session pool. The pool is tested over fake sessions; the real DOSBox-X worker is
// tested when CB_TEST_DOSBOX_X and CB_TEST_MASM_ROOT name the emulator and a MASM folder (BIN/MASM.EXE) --
// the machine's own, never assumed.
#include "cb/build/dos_sessions.hpp"

#include <atomic>
#include <thread>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {

using namespace cb;
using namespace cb::build::dos_sessions;

class FakeSession : public Session {
public:
    explicit FakeSession(int id, std::atomic<int>& live) : id_(id), live_(live) { ++live_; }
    ~FakeSession() override { --live_; }
    Outcome assemble_many(std::span<const Source>, std::optional<std::chrono::milliseconds>, const std::function<bool()>&) override {
        return Dead{};
    }
    bool alive() override { return alive_; }
    void close() override {
        alive_ = false;
        closed = true;
    }
    int id() const { return id_; }
    bool alive_ = true;
    bool closed = false;

private:
    int id_;
    std::atomic<int>& live_;
};

struct PoolFixture {
    std::atomic<int> made{0};
    std::atomic<int> live{0};
    int fail_next = 0;
    std::unique_ptr<WorkerPool> pool;
    explicit PoolFixture(std::size_t max) {
        pool = std::make_unique<WorkerPool>(
            [this]() -> std::shared_ptr<Session> {
                if (fail_next > 0) {
                    --fail_next;
                    throw ToolError("dosbox", 1, "", "no boot");
                }
                return std::make_shared<FakeSession>(++made, live);
            },
            max);
    }
};

}  // namespace

CB_TEST("build.dos_sessions.worker_script_is_a_crlf_dispatch_loop", [] {
    const std::string s = worker_script();
    CB_CHECK(s.starts_with("echo hi > C:\\BOOTED\r\n:loop\r\n"));
    CB_CHECK(s.find('\n') != std::string::npos);
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '\n') CB_CHECK(i > 0 && s[i - 1] == '\r');   // every line ends CRLF
    }
    // four fast polls, then the 1-second keyboard wait, then back
    const std::string poll = "rescan >nul\r\nif exist C:\\GO goto work\r\nwait\r\n";
    CB_CHECK(s.find(poll + poll + poll + poll + "choice /C:YN /T:Y,1 /N >nul\r\ngoto loop\r\n") != std::string::npos);
    CB_CHECK(s.ends_with(":work\r\ndel C:\\GO\r\nrescan >nul\r\ncall C:\\WORK.BAT\r\necho done > C:\\ACK\r\ngoto loop\r\n"));
    // WAIT.COM: mov ah,86h; xor cx,cx style delay then int 20h
    CB_CHECK_EQ(kWaitCom.size(), 12u);
    CB_CHECK_EQ(kWaitCom[0], 0xB4);
    CB_CHECK_EQ(kWaitCom[11], 0x20);
});

CB_TEST("build.dos_sessions.config_mounts_the_work_folder", [] {
    const std::string c = dosbox_config("C:/temp/dosw-1");
    CB_CHECK(c.find("mount c \"C:/temp/dosw-1\"\r\n") != std::string::npos);
    CB_CHECK(c.find("[cpu]\r\ncputype=386\r\ncore=normal\r\ncycles=max\r\n") != std::string::npos);
    CB_CHECK(c.ends_with("call WORKER.BAT\r\n"));
    CB_CHECK(c.find("\r\r") == std::string::npos);   // (the port wrote CR CR LF here)
});

CB_TEST("build.dos_sessions.pool_reuses_idle_sessions", [] {
    PoolFixture f(2);
    auto a = f.pool->lease();
    auto b = f.pool->lease();
    CB_CHECK_EQ(f.made.load(), 2);
    f.pool->release(a);   // kept: alive
    auto c = f.pool->lease();
    CB_CHECK(c == a);     // the same warm session comes back
    CB_CHECK_EQ(f.made.load(), 2);
    f.pool->release(b);
    f.pool->release(c);
    f.pool->close();
    CB_CHECK(static_cast<FakeSession&>(*a).closed);
    CB_CHECK(static_cast<FakeSession&>(*b).closed);
});

CB_TEST("build.dos_sessions.pool_drops_dead_and_unkept_sessions", [] {
    PoolFixture f(1);
    auto a = f.pool->lease();
    static_cast<FakeSession&>(*a).alive_ = false;
    f.pool->release(a);   // dead: closed and dropped
    CB_CHECK(static_cast<FakeSession&>(*a).closed);
    auto b = f.pool->lease();
    CB_CHECK(b != a);
    CB_CHECK_EQ(f.made.load(), 2);
    f.pool->release(b, false);   // not to be kept: closed
    CB_CHECK(static_cast<FakeSession&>(*b).closed);
    auto c = f.pool->lease();
    CB_CHECK_EQ(f.made.load(), 3);
    f.pool->release(c);
});

CB_TEST("build.dos_sessions.pool_failed_boot_gives_the_slot_back", [] {
    PoolFixture f(1);
    f.fail_next = 1;
    CB_CHECK_THROWS_AS(ToolError, (void)f.pool->lease());
    auto a = f.pool->lease();   // the single slot is free again: this would block forever otherwise
    CB_CHECK(a != nullptr);
    f.pool->release(a);
});

CB_TEST("build.dos_sessions.pool_caps_concurrent_leases", [] {
    PoolFixture f(2);
    std::atomic<int> holding{0};
    std::atomic<int> most{0};
    std::vector<std::thread> threads;
    for (int t = 0; t < 6; ++t) {
        threads.emplace_back([&] {
            for (int i = 0; i < 20; ++i) {
                auto s = f.pool->lease();
                const int now = ++holding;
                int seen = most.load();
                while (now > seen && !most.compare_exchange_weak(seen, now)) {}
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                --holding;
                f.pool->release(s);
            }
        });
    }
    for (auto& t : threads) t.join();
    CB_CHECK(most.load() <= 2);
    CB_CHECK(f.made.load() <= 2);   // never more sessions than the cap
    f.pool->close();
});

CB_TEST("build.dos_sessions.worker_refuses_a_stem_that_is_not_a_dos_name", [] {
    // exercised through a real worker only when one can be started; the check itself needs no emulator
    const auto dosbox = fs::get_env("CB_TEST_DOSBOX_X");
    const auto masm = fs::get_env("CB_TEST_MASM_ROOT");
    if (!dosbox || !masm) return;   // no emulator on this machine: nothing to run
    DosboxWorker worker(fs::from_text(*masm), fs::from_text(*dosbox));
    const std::vector<Source> bad{{"a b", "x"}};
    CB_CHECK_THROWS_AS(UsageError, (void)worker.assemble_many(bad));
    const std::vector<Source> bad2{{"x&del", "x"}};
    CB_CHECK_THROWS_AS(UsageError, (void)worker.assemble_many(bad2));
    CB_CHECK_THROWS_AS(UsageError, (void)worker.assemble_many(std::vector<Source>{{"", "x"}}));
});

CB_TEST("build.dos_sessions.real_worker_assembles_batches", [] {
    const auto dosbox = fs::get_env("CB_TEST_DOSBOX_X");
    const auto masm = fs::get_env("CB_TEST_MASM_ROOT");
    if (!dosbox || !masm) return;   // no emulator on this machine: nothing to run
    DosboxWorker worker(fs::from_text(*masm), fs::from_text(*dosbox));
    CB_CHECK(worker.alive());
    const std::vector<Source> batch{{"ONE", ".model small\n.code\nstart: mov ax,1\nret\nend start\n"},
                                    {"TWO", ".model small\n.code\nmov ax,2\nend\n"}};
    for (int round = 0; round < 2; ++round) {   // the second round is the warm path
        const Outcome o = worker.assemble_many(batch);
        const auto* done = std::get_if<Finished>(&o);
        CB_CHECK(done != nullptr);
        if (!done) return;
        CB_CHECK_EQ(done->results.size(), 2u);
        CB_CHECK_EQ(done->results[0].first, "ONE");
        CB_CHECK(done->results[0].second.obj.has_value());
        CB_CHECK(done->results[1].second.lst.has_value());
    }
    worker.close();
    CB_CHECK(!worker.alive());
});
