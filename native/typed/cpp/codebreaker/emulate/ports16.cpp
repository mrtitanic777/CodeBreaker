// codebreaker/emulate/ports16.cpp -- C++17 port of codebreaker.emulate.ports16 (see the header).

#include "codebreaker/emulate/ports16.hpp"

namespace codebreaker::emulate::ports16 {

const py::list<int64_t> UART_BASES = {0x3F8, 0x2F8, 0x3E8, 0x2E8, 0x000};
const py::list<int64_t> PIC_PORTS = {0x20, 0x21, 0xA0, 0xA1};
const py::bytes RECEIVED = py::bytes(std::string("\x41\x54\x0d\x0a", 4));   // "AT\r\n"

// ------------------------------------------------------------------------------------------ Uart

Uart::Uart()
    : dll(0x0C), dlm(0x00),   // 9600 baud
      ier(0x00), lcr(0x03), mcr(0x00), scr(0x00),
      fcr(0x00),
      msr(MSR_IDLE),
      thre_pending(false) {
    for (unsigned char c : RECEIVED.raw()) rx.push_back(c);
}

int64_t Uart::_iir() {
    int64_t fifo = (fcr & 1) ? 0xC0 : 0x00;
    if ((ier & 0x01) && !rx.empty()) {
        return fifo | 0x04;                    // received data available
    }
    if ((ier & 0x02) && thre_pending) {
        thre_pending = false;                  // (reading it clears it)
        return fifo | 0x02;                    // transmitter holding register empty
    }
    if ((ier & 0x08) && (msr & 0x0F)) {
        return fifo | 0x00;                    // modem status
    }
    return fifo | 0x01;                        // nothing pending
}

int64_t Uart::read(int64_t reg) {
    int64_t dlab = lcr & 0x80;
    if (reg == 0) {
        if (dlab) {
            return dll;
        }
        if (!rx.empty()) {
            int64_t b = rx.front();            // rx.pop(0)
            rx.pop_front();
            return b;
        }
        return 0x00;
    }
    if (reg == 1) {
        return dlab ? dlm : ier;
    }
    if (reg == 2) {
        return _iir();
    }
    if (reg == 3) {
        return lcr;
    }
    if (reg == 4) {
        return mcr;
    }
    if (reg == 5) {
        return LSR_THRE | LSR_TEMT | (!rx.empty() ? LSR_DR : 0);
    }
    if (reg == 6) {
        int64_t v = msr;
        msr = msr & 0xF0;                      // (the deltas clear on reading)
        if (mcr & 0x10) {                      // loopback: the outputs read back
            v = ((mcr & 0x02) << 3) | ((mcr & 0x01) << 5) | ((mcr & 0x04) << 4) | ((mcr & 0x08) << 4);
        }
        return v;
    }
    return scr;
}

void Uart::write(int64_t reg, int64_t value) {
    int64_t dlab = lcr & 0x80;
    if (reg == 0) {
        if (dlab) {
            dll = value;
        } else {
            thre_pending = true;               // (sent at once: empty again)
        }
    } else if (reg == 1) {
        if (dlab) {
            dlm = value;
        } else {
            if ((value & 0x02) && !(ier & 0x02)) {
                thre_pending = true;           // (enabling it with the transmitter empty)
            }
            ier = value & 0x0F;
        }
    } else if (reg == 2) {
        fcr = value & 0xC9;
        if (value & 0x02) {
            rx.clear();                        // receiver FIFO reset
        }
    } else if (reg == 3) {
        lcr = value;
    } else if (reg == 4) {
        mcr = value & 0x1F;
    } else if (reg == 7) {
        scr = value;
    }
}

// -------------------------------------------------------------------------------------------- Pic

Pic::Pic(int64_t mask) : imr(mask), irr(0x00), isr(0x00), read_isr(false), icw(0) {}

int64_t Pic::read(int64_t reg) {
    if (reg == 1) {
        return imr;
    }
    return read_isr ? isr : irr;
}

void Pic::write(int64_t reg, int64_t value) {
    if (reg == 0) {
        if (value & 0x10) {                    // ICW1: two or three words follow
            icw = (value & 0x01) ? 3 : 2;
            imr = 0;
            isr = 0;
            read_isr = false;
        } else if (value & 0x08) {             // OCW3
            if (value & 0x02) {
                read_isr = (value & 0x01) != 0;
            }
        } else if (value & 0x20) {             // OCW2: end of interrupt
            isr &= !(value & 0x40) ? isr - 1 : ~(int64_t(1) << (value & 7)) & 0xFF;
        }
    } else if (icw) {
        icw -= 1;
    } else {
        imr = value;
    }
}

// ----------------------------------------------------------------------------------------- Devices

Devices::Devices(std::function<int64_t()> clock_) {
    for (int64_t b : UART_BASES) {
        uarts.emplace(b, Uart());
    }
    pics.emplace(0x20, Pic(0xB8));             // (COM1, COM2 masked at the start)
    pics.emplace(0xA0, Pic(0x8F));
    clock = clock_ ? clock_ : std::function<int64_t()>([]() -> int64_t { return 0; });
}

Devices::Found Devices::_device(int64_t port) {
    int64_t base = port & ~int64_t(7);
    Found f;
    auto u = uarts.find(base);
    if (u != uarts.end()) {
        f.uart = &u->second;
        f.reg = port & 7;
        return f;
    }
    if (PIC_PORTS.contains(port)) {
        f.pic = &pics.at(port & ~int64_t(1));
        f.reg = port & 1;
        return f;
    }
    return f;
}

void Devices::_note(const py::str& what, int64_t port, int64_t value) {
    if (log.size() < MAX_LOG) {
        log.append(LogEntry(clock(), what, port, value));
    } else if (log.size() == MAX_LOG) {
        log.append(LogEntry(clock(), py::str("..."), 0, 0));
    }
}

int64_t Devices::read(int64_t port) {
    Found d = _device(port & 0xFFFF);
    if (!d.uart && !d.pic) {
        return 0;
    }
    int64_t v = (d.uart ? d.uart->read(d.reg) : d.pic->read(d.reg)) & 0xFF;
    _note(py::str("in"), port & 0xFFFF, v);
    return v;
}

void Devices::write(int64_t port, int64_t value) {
    Found d = _device(port & 0xFFFF);
    if (!d.uart && !d.pic) {
        return;
    }
    if (d.uart) {
        d.uart->write(d.reg, value & 0xFF);
    } else {
        d.pic->write(d.reg, value & 0xFF);
    }
    _note(py::str("out"), port & 0xFFFF, value & 0xFF);
}

// -------------------------------------------------------------------------------------------- hook

void hook(InsnHooks& uc, Devices& devices) {
    uc.hook_in([&devices](int64_t port, int64_t size) -> int64_t {
        int64_t v = devices.read(port);
        if (size >= 2) {                       // (a word: the next port's byte above)
            v |= devices.read(port + 1) << 8;
        }
        return v;
    });
    uc.hook_out([&devices](int64_t port, int64_t size, int64_t value) {
        devices.write(port, value);
        if (size >= 2) {
            devices.write(port + 1, value >> 8);
        }
    });
}

}  // namespace codebreaker::emulate::ports16
