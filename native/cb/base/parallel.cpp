#include "cb/base/parallel.hpp"

#include <windows.h>

#include <algorithm>
#include <charconv>
#include <cmath>
#include <format>
#include <thread>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/win.hpp"

namespace cb::base::budget {

namespace {

namespace core = ::cb::parallel;

// One slot: a one-byte lock on a file of its own. The lock belongs to the process that took it, and the system lets
// go of it when the process ends, even if it was killed. (A byte lock, like the Python's msvcrt.locking, so that
// processes of either kind contend for the same slots.)
class Slot {
public:
    Slot() = default;
    Slot(const Slot&) = delete;
    Slot& operator=(const Slot&) = delete;
    Slot(Slot&& o) noexcept : file_(o.file_) { o.file_ = INVALID_HANDLE_VALUE; }
    ~Slot() { give_back(); }

    bool take(const std::filesystem::path& path) {
        HANDLE h = CreateFileW(win::to_wide(fs::to_text(path)).c_str(), GENERIC_READ | GENERIC_WRITE,
                               FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (h == INVALID_HANDLE_VALUE) return false;
        if (!LockFile(h, 0, 0, 1, 0)) {
            CloseHandle(h);
            return false;
        }
        file_ = h;
        return true;
    }

    void give_back() {
        if (file_ == INVALID_HANDLE_VALUE) return;
        UnlockFile(file_, 0, 0, 1, 0);
        CloseHandle(file_);
        file_ = INVALID_HANDLE_VALUE;
    }

private:
    HANDLE file_ = INVALID_HANDLE_VALUE;
};

std::uint64_t cpu_ticks_idle_and_total(std::uint64_t& total) {
    FILETIME idle{}, kernel{}, user{};
    GetSystemTimes(&idle, &kernel, &user);
    const auto value = [](const FILETIME& f) { return (static_cast<std::uint64_t>(f.dwHighDateTime) << 32) | f.dwLowDateTime; };
    total = value(kernel) + value(user);   // (kernel time includes idle)
    return value(idle);
}

}  // namespace

double min_free_gb(const Limits& limits) {
    if (auto env = fs::get_env("CB_MIN_FREE_GB")) {
        double v = 0;
        const auto [end, ec] = std::from_chars(env->data(), env->data() + env->size(), v);
        if (ec == std::errc() && end == env->data() + env->size()) return v;
    }
    return limits.min_free_gb;
}

std::chrono::milliseconds wait_for_memory(const Limits& limits, std::string_view label, std::chrono::milliseconds poll,
                                          std::chrono::milliseconds limit) {
    const double need = min_free_gb(limits);
    std::chrono::milliseconds waited{0};
    while (true) {
        const double available = core::available_gb();
        if (available < 0 || available >= need || waited >= limit) return waited;
        if (waited.count() == 0)
            cb::err("(waiting: {:.1f} GB free, {:.1f} wanted{})\n", available, need,
                    label.empty() ? std::string() : " - " + std::string(label));
        std::this_thread::sleep_for(poll);
        waited += poll;
    }
}

std::optional<double> total_gb() {
    ULONGLONG kilobytes = 0;
    if (!GetPhysicallyInstalledSystemMemory(&kilobytes)) return std::nullopt;
    return static_cast<double>(kilobytes) / (1024.0 * 1024.0);
}

double reserve_gb(const Limits& limits) {
    const double need = min_free_gb(limits);
    const auto total = total_gb();
    return total && *total > 0 ? std::min(need, *total / 4) : need;
}

std::optional<double> idle_cores(std::chrono::milliseconds sample) {
    std::uint64_t total0 = 0, total1 = 0;
    const std::uint64_t idle0 = cpu_ticks_idle_and_total(total0);
    std::this_thread::sleep_for(sample);
    const std::uint64_t idle1 = cpu_ticks_idle_and_total(total1);
    if (total1 <= total0) return std::nullopt;
    return core::cpu_count() * static_cast<double>(idle1 - idle0) / static_cast<double>(total1 - total0);
}

std::optional<int> memory_allows(const Limits& limits, int mb_each) {
    const double available = core::available_gb();
    if (available < 0) return std::nullopt;
    const double spare = std::max(0.0, available - reserve_gb(limits));
    return static_cast<int>(std::floor(spare * 1024.0 / std::max(1, mb_each)));
}

struct Slots::Impl {
    std::vector<Slot> held;
};

Slots::Slots(const Limits& limits, int want_slots, int mb_each, std::string_view label,
             std::chrono::milliseconds limit, std::chrono::milliseconds poll)
    : impl_(std::make_unique<Impl>()) {
    const int cap = core::workers(limits.workers);
    int want = std::min(std::max(1, want_slots), cap);
    if (const auto by_memory = memory_allows(limits, mb_each)) want = std::min(want, std::max(1, *by_memory));
    if (want > 1) {   // (and no more than the processors free)
        if (const auto idle = idle_cores()) want = std::min(want, std::max(1, static_cast<int>(*idle + 0.5)));
    }
    fs::make_dirs(limits.slot_dir);
    std::chrono::milliseconds waited{0};
    while (true) {
        for (int i = 0; i < cap && static_cast<int>(impl_->held.size()) < want; ++i) {
            Slot slot;
            if (slot.take(limits.slot_dir / std::format("slot-{}", i))) impl_->held.push_back(std::move(slot));
        }
        if (!impl_->held.empty() || waited >= limit) return;
        if (waited.count() == 0)
            cb::err("(waiting for a free slot{})\n", label.empty() ? std::string() : " - " + std::string(label));
        std::this_thread::sleep_for(poll);
        waited += poll;
    }
}

Slots::~Slots() = default;

int Slots::held() const { return static_cast<int>(impl_->held.size()); }

int Slots::count() const { return std::max(1, held()); }

}  // namespace cb::base::budget
