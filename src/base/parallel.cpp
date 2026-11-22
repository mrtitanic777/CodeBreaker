#include "base/parallel.hpp"

#include <algorithm>
#include <charconv>
#include <cmath>
#include <format>
#include <thread>
#include <vector>

#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/file_lock.hpp"
#include "core/sys.hpp"

namespace cb::base::budget {

namespace {

namespace core = ::cb::parallel;

// One slot: a one-byte lock on a file of its own. The lock belongs to the process that took it, and the system lets
// go of it when the process ends, even if it was killed. (A byte lock, so that processes of any kind contend for the same slots.)
class Slot {
public:
    Slot() = default;
    Slot(const Slot&) = delete;
    Slot& operator=(const Slot&) = delete;
    Slot(Slot&&) noexcept = default;
    Slot& operator=(Slot&&) noexcept = default;

    bool take(const std::filesystem::path& path) {
        lock_ = file_lock::try_acquire(path, "slot");
        return lock_.has_value();
    }

    void give_back() { lock_.reset(); }

private:
    std::optional<file_lock::FileLock> lock_;
};

std::uint64_t cpu_ticks_idle_and_total(std::uint64_t& total) {
    std::uint64_t idle = 0;
    if (!sys::cpu_times(idle, total)) total = 0;
    return idle;
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

std::optional<double> total_gb() { return sys::total_memory_gb(); }

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
    Limits limits;
    int mb_each = 0;
    int cap = 0;
    int next = 0;   // the first slot file not yet tried by grow()
};

Slots::Slots(const Limits& limits, int want_slots, int mb_each, std::string_view label,
             std::chrono::milliseconds limit, std::chrono::milliseconds poll)
    : impl_(std::make_unique<Impl>()) {
    const int cap = core::workers(limits.workers);
    impl_->limits = limits;
    impl_->mb_each = mb_each;
    impl_->cap = cap;
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

bool Slots::grow(double idle_needed) {
    if (held() >= impl_->cap) return false;
    if (const auto by_memory = memory_allows(impl_->limits, impl_->mb_each); by_memory && *by_memory < 1) return false;
    if (const auto idle = idle_cores(); idle && *idle < idle_needed) return false;
    for (int tried = 0; tried < impl_->cap; ++tried) {   // (a slot this object holds is locked, so it is passed over like another's)
        const int i = (impl_->next + tried) % impl_->cap;
        Slot slot;
        if (slot.take(impl_->limits.slot_dir / std::format("slot-{}", i))) {
            impl_->held.push_back(std::move(slot));
            impl_->next = (i + 1) % impl_->cap;
            return true;
        }
    }
    return false;
}

}  // namespace cb::base::budget
