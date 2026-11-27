// emulate/rsp.hpp -- the Nintendo 64's RSP, CodeBreaker's own interpreter of it: the microcode a program puts in IMEM, run over DMEM, as the
// console's RSP runs it when the CPU clears SP_STATUS's halt bit.
//
//     rsp::Rsp sp(bus.sp_memory(), bus.rdram());     // DMEM and IMEM (8 KB, DMEM first) and the RDRAM its DMA reads and writes
//     rsp::RunResult r = sp.run(0x000);              // from IMEM's first word to its BREAK (or the step limit)
//
// What it runs: the scalar unit (the R4000's 32-bit integer instructions but multiply and divide, with its branch delay slots; loads and stores
// wrap inside DMEM's 4 KB and may be unaligned) and the vector unit: 32 registers of eight 16-bit lanes, the 48-bit accumulator of each lane,
// the flags VCO, VCC and VCE, and the divide unit's DIV_IN / DIV_OUT with its reciprocal and inverse square root tables (made as the hardware's
// ROM holds them); every computational instruction with its element selection, the moves MFC2 MTC2 CFC2 CTC2, and the twelve vector loads and
// twelve stores. COP0 is the SP's registers (0-7: the DMA's addresses and lengths, SP_STATUS, the semaphore) and the RDP's (8-15: DPC_START,
// DPC_END, ...): a DMA is done at once when its length is written; SP_STATUS's set and clear bits work as the CPU's writes do; a write to
// DPC_END is handed to `on_dpc` (the RDP: what the display list is, and where).
//
// What is approximate: time (a DMA and the RDP take none; the busy and full bits read 0); SFV, the odd VMACQ and the inverse square root
// table are written as the published behaviour says, not checked on a console; an instruction the RSP does not have ends the run as a fault.
#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>

#include "core/bytes.hpp"

namespace cb::emulate::rsp {

inline constexpr std::size_t kMemSize = 0x1000;     // DMEM, and IMEM after it
inline constexpr std::uint64_t kMaxSteps = 1000000;

// SP_STATUS's bits, as read.
inline constexpr std::uint32_t kHalt = 0x001, kBroke = 0x002, kSingleStep = 0x020, kInterruptOnBreak = 0x040;

struct RunResult {
    std::uint64_t steps = 0;
    bool broke = false;                 // it ran to a BREAK (the end of a task)
    std::optional<std::string> fault;   // what stopped it otherwise: "step limit", "not an RSP instruction 0x...", a write to SP_STATUS halting it
};

// One lane-wise vector register.
using Vector = std::array<std::uint16_t, 8>;

class Rsp {
public:
    // `sp` is DMEM then IMEM (8 KB), `rdram` the console's RDRAM (the DMA's other side).
    Rsp(ByteSpan sp, ByteSpan rdram);

    // Run from IMEM address `pc` (0-0xFFF) until BREAK, a halt, or `max_steps` instructions.
    RunResult run(std::uint32_t pc, std::uint64_t max_steps = kMaxSteps);
    // One instruction (with its delay slot's bookkeeping); false once halted.
    bool step();

    // The state, for tests and for the judge.
    std::uint32_t& gpr(unsigned n) { return r_[n & 31]; }
    Vector& vr(unsigned n) { return v_[n & 31]; }
    std::uint64_t acc(unsigned lane) const { return acc_[lane & 7]; }   // the 48 bits, sign-extended
    std::uint16_t vco() const { return vco_; }
    std::uint16_t vcc() const { return vcc_; }
    std::uint8_t vce() const { return vce_; }
    std::uint32_t pc() const { return pc_; }
    std::uint32_t status() const { return status_; }
    void set_status(std::uint32_t s) { status_ = s; }

    // COP0 as the CPU sees it too: SP registers 0-7, DP registers 8-15.
    std::uint32_t cop0(unsigned reg) const;
    void set_cop0(unsigned reg, std::uint32_t value);

    // Called when DPC_END is written: the display list runs from DPC_CURRENT to DPC_END, in DMEM when `xbus` (DPC_STATUS bit 0), else in RDRAM.
    std::function<void(std::uint32_t start, std::uint32_t end, bool xbus)> on_dpc;

    // The tables of the divide unit (512 entries each), as the hardware's ROM holds them.
    static const std::array<std::uint16_t, 512>& reciprocals();
    static const std::array<std::uint16_t, 512>& inverse_roots();

private:
    std::uint8_t dmem(std::uint32_t a) const { return sp_[a & 0xFFF]; }
    void set_dmem(std::uint32_t a, std::uint8_t v) { sp_[a & 0xFFF] = v; }
    std::uint32_t load(std::uint32_t a, unsigned size, bool sign) const;
    std::uint32_t fetch(std::uint32_t pc) const;   // the instruction word at IMEM address pc
    void store(std::uint32_t a, unsigned size, std::uint32_t v);
    void branch(bool taken, std::uint32_t target);
    bool execute(std::uint32_t word);
    void vector_op(std::uint32_t word);
    void vector_load(std::uint32_t word);
    void vector_store(std::uint32_t word);
    void dma(bool to_rdram, std::uint32_t length);

    std::uint8_t byte(unsigned reg, unsigned b) const;
    void set_byte(unsigned reg, unsigned b, std::uint8_t value);

    ByteSpan sp_;
    ByteSpan rdram_;
    std::array<std::uint32_t, 32> r_{};
    std::array<Vector, 32> v_{};
    std::array<std::uint64_t, 8> acc_{};
    std::uint16_t vco_ = 0, vcc_ = 0;
    std::uint8_t vce_ = 0;
    std::int16_t div_in_ = 0, div_out_ = 0;
    bool div_dp_ = false;
    std::uint32_t pc_ = 0;
    std::optional<std::uint32_t> next_;   // a branch's target, taken after the delay slot
    bool in_delay_ = false;
    std::uint32_t status_ = kHalt;
    std::uint32_t mem_addr_ = 0, dram_addr_ = 0, semaphore_ = 0;
    std::array<std::uint32_t, 8> dp_{};   // DPC_START END CURRENT STATUS CLOCK BUFBUSY PIPEBUSY TMEM
};

}  // namespace cb::emulate::rsp
