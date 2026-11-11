// cb/emulate/ports16.hpp -- the devices a 16-bit run talks to through `in` and `out`.
//
//     Devices devices;
//     std::uint8_t value = devices.read(0x3FD);       // an `in`
//     devices.write(0x3F8, 'A');                      // an `out`
//     devices.log();                                  // [(step, in | out, port, value)], in order
//
// Without them every `in` read 0 and every `out` went nowhere: COMM.DRV's functions wait for the UART's
// transmitter to be empty (`in al, dx / test al, 20h / jz`) and spun to the step limit, the original and the
// rebuild alike, and what they wrote to the chip was not looked at -- so a rebuild writing the wrong divisor or the
// wrong line control passed. Here are an 8250/16550A at each of the four COM ports and the two 8259 interrupt
// controllers, as much of them as a driver's code paths see: registers that read back what was written, the line
// status saying the transmitter is empty, a few bytes waiting in the receiver, the interrupt identification a
// handler loops on, the controllers' masks and in-service registers. No interrupt is ever raised. Each run starts
// from the same state, so the original and the rebuild meet the same devices; what they did to them (the log) is
// part of what is compared (verify/lanes/win16).
//
// A UART answers at base 0 as well: a function run alone finds its port's record as the module's data has it
// before OpenComm filled it in, port base 0 -- COMM.DRV's TRMCOM polls port 5 for the transmitter, its interrupt
// handler reads ports 2 and 6. (Ports 0-7 are the DMA controller's on a PC; nothing else run here reaches them.)
//
// Other ports are not modelled: an `in` reads 0 and is not logged.
//
// One Devices belongs to one run, on one thread: the emulator it is hooked to is single-threaded.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <functional>
#include <span>
#include <vector>

struct uc_struct;   // Unicorn's engine (uc_engine); the header stays out of ours

namespace cb::emulate::ports16 {

inline constexpr std::array<std::uint16_t, 5> kUartBases = {0x3F8, 0x2F8, 0x3E8, 0x2E8, 0x000};
inline constexpr std::array<std::uint16_t, 4> kPicPorts = {0x20, 0x21, 0xA0, 0xA1};
inline constexpr std::array<std::uint8_t, 4> kReceived = {0x41, 0x54, 0x0D, 0x0A};   // each UART starts with "AT\r\n" waiting
inline constexpr std::size_t kMaxLog = 256;                                          // accesses logged per run (then a marker)

// An 8250 / 16550A. Registers (base +): 0 receive / transmit (divisor low with DLAB), 1 interrupt enable (divisor
// high), 2 interrupt identification / FIFO control, 3 line control, 4 modem control, 5 line status, 6 modem status,
// 7 scratch.
class Uart {
public:
    Uart();
    std::uint8_t read(unsigned reg);
    void write(unsigned reg, std::uint8_t value);

    std::uint8_t divisor_low() const { return dll_; }
    std::uint8_t divisor_high() const { return dlm_; }
    std::uint8_t line_control() const { return lcr_; }
    std::uint8_t modem_control() const { return mcr_; }
    std::uint8_t interrupt_enable() const { return ier_; }
    std::size_t waiting() const { return rx_.size(); }

private:
    std::uint8_t interrupt_id();

    std::uint8_t dll_ = 0x0C, dlm_ = 0x00;                       // 9600 baud
    std::uint8_t ier_ = 0x00, lcr_ = 0x03, mcr_ = 0x00, scr_ = 0x00;
    std::uint8_t fcr_ = 0x00;
    std::deque<std::uint8_t> rx_;                                // the receiver's bytes
    std::uint8_t msr_;
    bool thre_pending_ = false;                                  // an empty transmitter, signalled
};

// An 8259: the mask (base + 1), and the request / in-service registers (base, as the last OCW3 chose); an
// initialisation sequence is taken in and ends with the mask cleared.
class Pic {
public:
    explicit Pic(std::uint8_t mask) : imr_(mask) {}
    std::uint8_t read(unsigned reg);
    void write(unsigned reg, std::uint8_t value);

    std::uint8_t mask() const { return imr_; }
    std::uint8_t in_service() const { return isr_; }

private:
    std::uint8_t imr_, irr_ = 0x00, isr_ = 0x00;
    bool read_isr_ = false;
    unsigned icw_ = 0;                                           // initialisation words still to come
};

enum class Access { In, Out, Truncated };                        // Truncated: the log is full; the one marker entry

struct LogEntry {
    std::uint64_t step = 0;                                      // the run's clock when it happened
    Access kind = Access::In;
    std::uint16_t port = 0;
    std::uint8_t value = 0;
    friend bool operator==(const LogEntry&, const LogEntry&) = default;
};

// The ports a run can reach: the UARTs, the two interrupt controllers. Fresh for each run. `clock` gives the run's
// step, logged with each access (to order them among its other effects); without one every step is 0.
class Devices {
public:
    explicit Devices(std::function<std::uint64_t()> clock = {});

    // An `in` / an `out` at `port` (the low 16 bits of it count); a port no device answers reads 0, writes nothing,
    // and is not logged.
    std::uint8_t read(std::uint32_t port);
    void write(std::uint32_t port, std::uint32_t value);

    // What the CPU does for an `in` / an `out` instruction of `size` bytes (1, 2 or 4): a word is two byte accesses,
    // the next port's byte above the first's.
    std::uint32_t in(std::uint32_t port, int size);
    void out(std::uint32_t port, int size, std::uint32_t value);

    std::span<const LogEntry> log() const { return log_; }
    void clear_log() { log_.clear(); }                                       // (the devices keep their state)
    const Uart& uart(std::size_t index) const { return uarts_.at(index); }   // by place in kUartBases
    const Pic& pic(std::size_t index) const { return pics_.at(index); }      // 0: master (0x20), 1: slave (0xA0)

private:
    struct Found {
        Uart* uart = nullptr;
        Pic* pic = nullptr;
        unsigned reg = 0;
    };
    Found find(std::uint16_t port);
    void note(Access what, std::uint16_t port, std::uint8_t value);

    std::array<Uart, kUartBases.size()> uarts_;
    std::array<Pic, 2> pics_;
    std::vector<LogEntry> log_;
    std::function<std::uint64_t()> clock_;
};

// Hands an engine's `in` and `out` instructions to a Devices for as long as it lives. The engine and the devices
// must outlive it; destroying it (or detach()) removes the hooks.
class PortHooks {
public:
    PortHooks(uc_struct* engine, Devices& devices);   // throws EmulationError when Unicorn refuses the hook
    PortHooks(const PortHooks&) = delete;
    PortHooks& operator=(const PortHooks&) = delete;
    ~PortHooks();
    void detach();

private:
    uc_struct* engine_;
    std::uintptr_t in_hook_ = 0, out_hook_ = 0;
};

}  // namespace cb::emulate::ports16
