#include "cb/core/parallel.hpp"

#include <windows.h>

#include <algorithm>

#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::parallel {

int cpu_count() {
    const unsigned n = std::thread::hardware_concurrency();
    return n == 0 ? 4 : static_cast<int>(n);
}

namespace {
bool truthy(const std::string& v) {
    const std::string s = text::lower(text::trim(v));
    return !(s.empty() || s == "0" || s == "false" || s == "no" || s == "off" || s == "none");
}
}  // namespace

int workers(const WorkerPolicy& policy) {
    int n = policy.workers;
    if (auto v = fs::get_env("CB_JOBS")) {
        if (auto parsed = text::parse_int<int>(text::trim(*v))) n = *parsed;
    }
    if (n <= 0) n = std::max(1, cpu_count() - 2);   // (two left for the machine's owner and the process handing out work)
    bool low = policy.low_load;
    if (auto v = fs::get_env("CB_LOW_LOAD")) low = truthy(*v);
    return low ? std::min(n, kLowLoad) : n;
}

double available_gb() {
    MEMORYSTATUSEX m{};
    m.dwLength = sizeof(m);
    if (!GlobalMemoryStatusEx(&m)) return -1.0;
    return static_cast<double>(std::min(m.ullAvailPhys, m.ullAvailPageFile)) / static_cast<double>(1ull << 30);
}

bool throttle(int cores, bool below_normal) {
    HANDLE self = GetCurrentProcess();
    bool ok = true;
    if (below_normal) ok = SetPriorityClass(self, BELOW_NORMAL_PRIORITY_CLASS) != 0 && ok;
    if (cores > 0) {
        const int n = std::min(cores, cpu_count());
        const DWORD_PTR mask = n >= 64 ? ~DWORD_PTR{0} : (DWORD_PTR{1} << n) - 1;
        ok = SetProcessAffinityMask(self, mask) != 0 && ok;
    }
    return ok;
}

ThreadPool::ThreadPool(std::size_t threads) {
    if (threads == 0) threads = static_cast<std::size_t>(workers());
    threads_.reserve(threads);
    for (std::size_t i = 0; i < threads; ++i) threads_.emplace_back([this] { run(); });
}

ThreadPool::~ThreadPool() {
    {
        std::lock_guard lock(mutex_);
        stopping_ = true;
    }
    wake_.notify_all();
    for (auto& t : threads_) t.join();
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::lock_guard lock(mutex_);
        queue_.push_back(std::move(task));
    }
    wake_.notify_one();
}

void ThreadPool::run() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock lock(mutex_);
            wake_.wait(lock, [this] { return stopping_ || !queue_.empty(); });
            if (queue_.empty()) return;   // stopping, and nothing left to do
            task = std::move(queue_.front());
            queue_.pop_front();
        }
        try {
            task();
        } catch (...) {
            // A task's own failure is carried by its future (submit) or is, for enqueue, deliberately dropped.
        }
    }
}

}  // namespace cb::parallel
