// cb/emulate/ports16.cpp -- see ports16.hpp.
#include "cb/emulate/ports16.hpp"

#include <algorithm>

#include "cb/core/unicorn_api.hpp"

#pragma warning(push, 0)
#include <unicorn/x86.h>
#pragma warning(pop)

namespace cb::emulate::ports16 {

namespace {

// line status: data ready, transmitter holding register empty, transmitter empty
constexpr std::uint8_t kLsrDataReady = 0x01, kLsrThre = 0x20, kLsrTemt = 0x40;
// modem status: clear to send, data set ready, carrier detect (the deltas clear)
constexpr std::uint8_t kMsrIdle = 0x10 | 0x20 | 0x80;

}  // namespace

// ------------------------------------------------------------------------------------------------------ Uart

Uart::Uart() : rx_(kReceived.begin(), kReceived.end()), msr_(kMsrIdle) {}

std::uint8_t Uart::interrupt_id() {
    const std::uint8_t fifo = (fcr_ & 1) ? 0xC0 : 0x00;
    if ((ier_ & 0x01) && !rx_.empty()) return fifo | 0x04;       // received data available
    if ((ier_ & 0x02) && thre_pending_) {
        thre_pending_ = false;                                   // (reading it clears it)
        return fifo | 0x02;                                      // transmitter holding register empty
    }
    if ((ier_ & 0x08) && (msr_ & 0x0F)) return fifo | 0x00;      // modem status
    return fifo | 0x01;                                          // nothing pending
}

std::uint8_t Uart::read(unsigned reg) {
    const bool dlab = (lcr_ & 0x80) != 0;
    switch (reg) {
        case 0:
            if (dlab) return dll_;
            if (rx_.empty()) return 0x00;
            {
                const std::uint8_t b = rx_.front();
                rx_.pop_front();
                return b;
            }
        case 1: return dlab ? dlm_ : ier_;
        case 2: return interrupt_id();
        case 3: return lcr_;
        case 4: return mcr_;
        case 5: return static_cast<std::uint8_t>(kLsrThre | kLsrTemt | (rx_.empty() ? 0 : kLsrDataReady));
        case 6: {
            std::uint8_t v = msr_;
            msr_ &= 0xF0;                                        // (the deltas clear on reading)
            if (mcr_ & 0x10) {                                   // loopback: the outputs read back
                v = static_cast<std::uint8_t>(((mcr_ & 0x02) << 3) | ((mcr_ & 0x01) << 5) | ((mcr_ & 0x04) << 4) |
                                              ((mcr_ & 0x08) << 4));
            }
            return v;
        }
        default: return scr_;
    }
}

void Uart::write(unsigned reg, std::uint8_t value) {
    const bool dlab = (lcr_ & 0x80) != 0;
    switch (reg) {
        case 0:
            if (dlab) dll_ = value;
            else thre_pending_ = true;                           // (sent at once: empty again)
            break;
        case 1:
            if (dlab) {
                dlm_ = value;
            } else {
                if ((value & 0x02) && !(ier_ & 0x02)) thre_pending_ = true;   // (enabling it with the transmitter empty)
                ier_ = value & 0x0F;
            }
            break;
        case 2:
            fcr_ = value & 0xC9;
            if (value & 0x02) rx_.clear();                       // receiver FIFO reset
            break;
        case 3: lcr_ = value; break;
        case 4: mcr_ = value & 0x1F; break;
        case 7: scr_ = value; break;
        default: break;                                          // the line and modem status are read-only
    }
}

// -------------------------------------------------------------------------------------------------------- Pic

std::uint8_t Pic::read(unsigned reg) {
    if (reg == 1) return imr_;
    return read_isr_ ? isr_ : irr_;
}

void Pic::write(unsigned reg, std::uint8_t value) {
    if (reg == 0) {
        if (value & 0x10) {                                      // ICW1: two or three words follow
            icw_ = (value & 0x01) ? 3 : 2;
            imr_ = 0;
            isr_ = 0;
            read_isr_ = false;
        } else if (value & 0x08) {                               // OCW3
            if (value & 0x02) read_isr_ = (value & 0x01) != 0;
        } else if (value & 0x20) {                               // OCW2: end of interrupt
            if (value & 0x40) isr_ &= static_cast<std::uint8_t>(~(1u << (value & 7)));   // specific: that level
            else isr_ &= static_cast<std::uint8_t>(isr_ - 1);                            // non-specific: the lowest
        }
    } else if (icw_ != 0) {
        --icw_;
    } else {
        imr_ = value;
    }
}

// --------------------------------------------------------------------------------------------------- Devices

Devices::Devices(std::function<std::uint64_t()> clock)
    : pics_{Pic(0xB8), Pic(0x8F)},                               // (COM1, COM2 masked at the start)
      clock_(clock ? std::move(clock) : [] { return std::uint64_t{0}; }) {}

Devices::Found Devices::find(std::uint16_t port) {
    const std::uint16_t base = port & ~std::uint16_t{7};
    for (std::size_t i = 0; i < kUartBases.size(); ++i) {
        if (kUartBases[i] == base) return {&uarts_[i], nullptr, port & 7u};
    }
    if (std::find(kPicPorts.begin(), kPicPorts.end(), port) != kPicPorts.end()) {
        return {nullptr, &pics_[(port & ~std::uint16_t{1}) == 0x20 ? 0 : 1], port & 1u};
    }
    return {};
}

void Devices::note(Access what, std::uint16_t port, std::uint8_t value) {
    if (log_.size() < kMaxLog) {
        log_.push_back({clock_(), what, port, value});
    } else if (log_.size() == kMaxLog) {
        log_.push_back({clock_(), Access::Truncated, 0, 0});     // one marker, then nothing more
    }
}

std::uint8_t Devices::read(std::uint32_t port) {
    const auto p = static_cast<std::uint16_t>(port & 0xFFFF);
    const Found d = find(p);
    if (!d.uart && !d.pic) return 0;
    const std::uint8_t v = d.uart ? d.uart->read(d.reg) : d.pic->read(d.reg);
    note(Access::In, p, v);
    return v;
}

void Devices::write(std::uint32_t port, std::uint32_t value) {
    const auto p = static_cast<std::uint16_t>(port & 0xFFFF);
    const Found d = find(p);
    if (!d.uart && !d.pic) return;
    const auto v = static_cast<std::uint8_t>(value & 0xFF);
    if (d.uart) d.uart->write(d.reg, v);
    else d.pic->write(d.reg, v);
    note(Access::Out, p, v);
}

std::uint32_t Devices::in(std::uint32_t port, int size) {
    std::uint32_t v = read(port);
    if (size >= 2) v |= static_cast<std::uint32_t>(read(port + 1)) << 8;   // (a word: the next port's byte above)
    return v;
}

void Devices::out(std::uint32_t port, int size, std::uint32_t value) {
    write(port, value);
    if (size >= 2) write(port + 1, value >> 8);
}

// ------------------------------------------------------------------------------------------------ PortHooks

namespace {

std::uint32_t on_in(uc_engine*, std::uint32_t port, int size, void* user) {
    return static_cast<Devices*>(user)->in(port, size);
}

void on_out(uc_engine*, std::uint32_t port, int size, std::uint32_t value, void* user) {
    static_cast<Devices*>(user)->out(port, size, value);
}

}  // namespace

PortHooks::PortHooks(uc_struct* engine, Devices& devices) : engine_(engine) {
    const auto& uc = emu::unicorn::api();
    uc_hook in = 0, out = 0;
    emu::unicorn::check(uc.uc_hook_add(engine, &in, UC_HOOK_INSN, reinterpret_cast<void*>(&on_in), &devices, 1, 0,
                                       UC_X86_INS_IN),
                        "uc_hook_add(in)");
    in_hook_ = in;
    try {
        emu::unicorn::check(uc.uc_hook_add(engine, &out, UC_HOOK_INSN, reinterpret_cast<void*>(&on_out), &devices, 1, 0,
                                           UC_X86_INS_OUT),
                            "uc_hook_add(out)");
    } catch (...) {
        uc.uc_hook_del(engine, in);
        throw;
    }
    out_hook_ = out;
}

void PortHooks::detach() {
    if (engine_ == nullptr) return;
    const auto& uc = emu::unicorn::api();
    uc.uc_hook_del(engine_, in_hook_);
    uc.uc_hook_del(engine_, out_hook_);
    engine_ = nullptr;
}

PortHooks::~PortHooks() { detach(); }

}  // namespace cb::emulate::ports16
