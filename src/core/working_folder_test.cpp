// Tests of the process's working folder held for one tool (working_folder.hpp).
#include "core/working_folder.hpp"

#include <atomic>
#include <chrono>
#include <string>
#include <thread>
#include <vector>

#include "core/testing.hpp"

namespace cb {
namespace {

namespace stdfs = std::filesystem;

CB_TEST("core.working_folder.given_back_at_the_end", [] {
    const stdfs::path before = stdfs::current_path();
    const stdfs::path folder = stdfs::temp_directory_path() / "cb_working_folder_back";
    stdfs::create_directories(folder);
    {
        const WorkingFolder in(folder);
        CB_CHECK(stdfs::equivalent(stdfs::current_path(), folder));
    }
    CB_CHECK(stdfs::equivalent(stdfs::current_path(), before));
    stdfs::remove_all(folder);
});

CB_TEST("core.working_folder.threads_do_not_move_each_other", [] {
    const stdfs::path before = stdfs::current_path();
    const stdfs::path root = stdfs::temp_directory_path() / "cb_working_folder_threads";
    std::atomic<int> moved{0};
    std::vector<std::thread> threads;
    for (int t = 0; t < 4; ++t) {
        const stdfs::path folder = root / std::to_string(t);
        stdfs::create_directories(folder);
        threads.emplace_back([folder, &moved] {
            for (int k = 0; k < 40; ++k) {
                const WorkingFolder in(folder);
                std::this_thread::sleep_for(std::chrono::microseconds(200));
                if (!stdfs::equivalent(stdfs::current_path(), folder)) ++moved;
            }
        });
    }
    for (std::thread& t : threads) t.join();
    CB_CHECK_EQ(moved.load(), 0);
    CB_CHECK(stdfs::equivalent(stdfs::current_path(), before));
    stdfs::remove_all(root);
});

}  // namespace
}  // namespace cb
