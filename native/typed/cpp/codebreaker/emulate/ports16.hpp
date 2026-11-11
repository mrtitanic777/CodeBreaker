// codebreaker/emulate/ports16.hpp -- SHIM over cb/emulate/ports16.hpp (docs/CPP_DESIGN.md 6.3): the old Devices
// handle, forwarding to the new one. No logic; deleted with its last py:: consumer (emulate/windows16).
//
// The old Python-shaped extras (public Uart/Pic registers as int64_t, `devices.log` as a live list) are not kept: no
// caller reads them. `log` is a proxy that converts to the old list of (step, "in" | "out" | "...", port, value) and
// can be cleared.
#pragma once

#include <cstdint>
#include <functional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/emulate/ports16.hpp"

namespace codebreaker::emulate::ports16 {

constexpr int64_t MAX_LOG = static_cast<int64_t>(cb::emulate::ports16::kMaxLog);

// One entry of Devices.log: (step, "in" | "out" | "...", port, value)
using LogEntry = std::tuple<int64_t, py::str, int64_t, int64_t>;

class Devices {
  public:
    explicit Devices(std::function<int64_t()> clock = nullptr)
        : impl_(clock ? std::function<std::uint64_t()>([clock] { return static_cast<std::uint64_t>(clock()); })
                      : std::function<std::uint64_t()>()),
          log(&impl_) {}

    int64_t read(int64_t port) { return impl_.read(static_cast<std::uint32_t>(port)); }
    void write(int64_t port, int64_t value) { impl_.write(static_cast<std::uint32_t>(port), static_cast<std::uint32_t>(value)); }

    // `devices.log`: converts to the old list, and clears.
    struct LogProxy {
        cb::emulate::ports16::Devices* impl;
        explicit LogProxy(cb::emulate::ports16::Devices* d) : impl(d) {}
        operator py::list<LogEntry>() const {
            py::list<LogEntry> out;
            for (const auto& e : impl->log()) {
                const char* what = e.kind == cb::emulate::ports16::Access::In ? "in" : e.kind == cb::emulate::ports16::Access::Out ? "out" : "...";
                out.append(LogEntry(static_cast<int64_t>(e.step), py::str(what), e.port, e.value));
            }
            return out;
        }
        void clear() { impl->clear_log(); }
    };

    cb::emulate::ports16::Devices& impl() { return impl_; }

  private:
    cb::emulate::ports16::Devices impl_;

  public:
    LogProxy log;
};

// What hook() asks of Unicorn: the callback of an `in` instruction (port, size -> the value read) and of an `out`
// (port, size, value).
class InsnHooks {
  public:
    virtual ~InsnHooks() = default;
    virtual void hook_in(std::function<int64_t(int64_t port, int64_t size)> on_in) = 0;
    virtual void hook_out(std::function<void(int64_t port, int64_t size, int64_t value)> on_out) = 0;
};

// Hand `uc`'s `in` and `out` instructions to `devices` (which must outlive the hooks).
inline void hook(InsnHooks& uc, Devices& devices) {
    cb::emulate::ports16::Devices* d = &devices.impl();
    uc.hook_in([d](int64_t port, int64_t size) -> int64_t { return d->in(static_cast<std::uint32_t>(port), static_cast<int>(size)); });
    uc.hook_out([d](int64_t port, int64_t size, int64_t value) {
        d->out(static_cast<std::uint32_t>(port), static_cast<int>(size), static_cast<std::uint32_t>(value));
    });
}

}  // namespace codebreaker::emulate::ports16
