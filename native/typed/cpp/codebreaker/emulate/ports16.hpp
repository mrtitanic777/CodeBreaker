// codebreaker/emulate/ports16.hpp -- C++17 port of codebreaker.emulate.ports16.
//
// ports16.py -- the devices a 16-bit run talks to through `in` and `out`.
//
//     devices = ports16.Devices()
//     value = devices.read(port)          # an `in`
//     devices.write(port, value)          # an `out`
//     devices.log                         # [(step, "in" | "out", port, value)], in order
//
// Without them every `in` read 0 and every `out` went nowhere: COMM.DRV's functions wait for the
// UART's transmitter to be empty (`in al, dx / test al, 20h / jz`) and spun to the step limit, the
// original and the rebuild alike, and what they wrote to the chip was not looked at -- so a rebuild
// writing the wrong divisor or the wrong line control passed. Here are an 8250/16550A at each of the
// four COM ports and the two 8259 interrupt controllers, as much of them as a driver's code paths
// see: registers that read back what was written, the line status saying the transmitter is empty,
// a few bytes waiting in the receiver, the interrupt identification a handler loops on, the
// controllers' masks and in-service registers. No interrupt is ever raised. Each run starts from the
// same state, so the original and the rebuild meet the same devices; what they did to them (the
// log) is part of what is compared (verify/lanes/win16.py).
//
// A UART answers at base 0 as well: a function run alone finds its port's record as the module's
// data has it before OpenComm filled it in, port base 0 -- COMM.DRV's TRMCOM polls port 5 for the
// transmitter, its interrupt handler reads ports 2 and 6. (Ports 0-7 are the DMA controller's on a
// PC; nothing else run here reaches them.)
//
// Other ports are not modelled: an `in` reads 0 and is not logged, as before.
//
// Typing notes:
//   Python ints are int64_t here (a register value is not masked until the Python masks it, so an
//   unmasked or negative value written straight to a Uart or a Pic is kept as the Python keeps it).
//   Devices' dicts of devices (uarts, pics) are std::maps keyed by base port.
//   `clock` is a callable returning the run's step (None -> a clock that is always 0).
//   hook(uc, U, devices) hands Unicorn's `in`/`out` instruction hooks to the devices; Unicorn is a
//   Python object, so here `uc` is the InsnHooks interface (the one thing the Python asks of it:
//   hook_add for the IN and for the OUT instruction), and U/UC_X86_INS_* are not needed.
#pragma once

#include <deque>
#include <functional>
#include <map>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::emulate::ports16 {

extern const py::list<int64_t> UART_BASES;
extern const py::list<int64_t> PIC_PORTS;
extern const py::bytes RECEIVED;   // what each UART has waiting at the start ("AT\r\n")
constexpr int64_t MAX_LOG = 256;   // accesses logged per run (then "...")

// line status: data ready, transmitter holding register empty, transmitter empty
constexpr int64_t LSR_DR = 0x01, LSR_THRE = 0x20, LSR_TEMT = 0x40;
// modem status: clear to send, data set ready, carrier detect (the deltas clear)
constexpr int64_t MSR_IDLE = 0x10 | 0x20 | 0x80;

// An 8250 / 16550A. Registers (base +): 0 receive / transmit (divisor low with DLAB),
// 1 interrupt enable (divisor high), 2 interrupt identification / FIFO control, 3 line control,
// 4 modem control, 5 line status, 6 modem status, 7 scratch.
class Uart {
  public:
    int64_t dll, dlm;                    // 9600 baud
    int64_t ier, lcr, mcr, scr;
    int64_t fcr;
    std::deque<int64_t> rx;              // the receiver's bytes (a bytearray)
    int64_t msr;
    bool thre_pending;                   // (an empty transmitter, signalled)

    Uart();
    int64_t read(int64_t reg);
    void write(int64_t reg, int64_t value);

  private:
    int64_t _iir();
};

// An 8259: the mask (base + 1), and the request / in-service registers (base, as the last
// OCW3 chose); an initialisation sequence is taken in and ends with the mask cleared.
class Pic {
  public:
    int64_t imr, irr, isr;
    bool read_isr;
    int64_t icw;                         // initialisation words still to come

    explicit Pic(int64_t mask);
    int64_t read(int64_t reg);
    void write(int64_t reg, int64_t value);
};

// One entry of Devices.log: (step, "in" | "out" | "...", port, value)
using LogEntry = std::tuple<int64_t, py::str, int64_t, int64_t>;

// The ports a run can reach: the UARTs, the two interrupt controllers. Fresh for each run.
// `clock()`: the run's step, logged with each access (to order them among its other effects).
class Devices {
  public:
    std::map<int64_t, Uart> uarts;
    std::map<int64_t, Pic> pics;
    py::list<LogEntry> log;
    std::function<int64_t()> clock;

    explicit Devices(std::function<int64_t()> clock = nullptr);
    int64_t read(int64_t port);
    void write(int64_t port, int64_t value);

  private:
    // (the Uart or the Pic that answers at `port`, and the register): neither when none does
    struct Found {
        Uart* uart = nullptr;
        Pic* pic = nullptr;
        int64_t reg = 0;
    };
    Found _device(int64_t port);
    void _note(const py::str& what, int64_t port, int64_t value);
};

// What hook() asks of Unicorn: the callback of an `in` instruction (port, size -> the value read)
// and of an `out` (port, size, value).
class InsnHooks {
  public:
    virtual ~InsnHooks() = default;
    virtual void hook_in(std::function<int64_t(int64_t port, int64_t size)> on_in) = 0;
    virtual void hook_out(std::function<void(int64_t port, int64_t size, int64_t value)> on_out) = 0;
};

// Hand `uc`'s `in` and `out` instructions to `devices` (which must outlive the hooks).
void hook(InsnHooks& uc, Devices& devices);

}  // namespace codebreaker::emulate::ports16
