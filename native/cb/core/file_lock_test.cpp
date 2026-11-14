#include "cb/core/file_lock.hpp"

#include <atomic>
#include <thread>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;

CB_TEST("core.file_lock.a_taken_lock_refuses_the_second_and_is_let_go_with_the_first", [] {
    fs::ScopedTempDir dir("cb-lock");
    const auto file = dir.path() / "sub" / "x.lock";
    auto first = file_lock::try_acquire(file, "pid 7");
    CB_CHECK(first.has_value());
    CB_CHECK_EQ(file_lock::holder_of(file), "pid 7");   // (readable while locked)
    CB_CHECK(!file_lock::try_acquire(file).has_value());
    first->release();
    CB_CHECK(file_lock::try_acquire(file).has_value());
});

CB_TEST("core.file_lock.a_lock_file_left_by_a_dead_run_is_taken_over", [] {
    fs::ScopedTempDir dir("cb-lock");
    const auto file = dir.path() / "x.lock";
    fs::write_text(file, "pid 99999999");   // (the file of a run that was killed: the file is there, the lock is not)
    auto got = file_lock::try_acquire(file);
    CB_CHECK(got.has_value());
    CB_CHECK(file_lock::holder_of(file) != "pid 99999999");
});

CB_TEST("core.file_lock.acquire_tells_the_holder_and_waits_until_it_lets_go", [] {
    fs::ScopedTempDir dir("cb-lock");
    const auto file = dir.path() / "x.lock";
    auto first = file_lock::try_acquire(file, "pid 4242");
    std::vector<std::string> told;
    std::thread letting_go([&] {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        first->release();
    });
    auto got = file_lock::acquire(file, [&](const std::string& who) { told.push_back(who); }, std::nullopt, std::chrono::milliseconds(20));
    letting_go.join();
    CB_CHECK(got.has_value());
    CB_CHECK_EQ(told.size(), std::size_t{1});
    CB_CHECK_EQ(told[0], "pid 4242");
});

CB_TEST("core.file_lock.acquire_gives_up_at_its_timeout", [] {
    fs::ScopedTempDir dir("cb-lock");
    const auto file = dir.path() / "x.lock";
    auto first = file_lock::try_acquire(file);
    CB_CHECK(!file_lock::acquire(file, {}, std::chrono::milliseconds(100), std::chrono::milliseconds(20)).has_value());
});
