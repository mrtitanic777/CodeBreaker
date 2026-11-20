// decompile/engine/pcode_emulator.hh -- CodeBreaker's ONE emulator: a p-code interpreter that runs any processor from its SLEIGH
// description (languages/<cpu>.sla), the description the decompiler lifts and sleigh_reader reads instructions from.
//
//     sleigh::Emulator z80(languages / "z80.sla", {});
//     z80.load(0, rom.data(), rom.size());
//     z80.setPc(0);
//     z80.onRead([](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value) { ...; return true; });
//     z80.run(10000);
//     std::uint64_t a = z80.reg("A");
//
// Memory is a sparse page map per address space (ram, io ... whatever the description declares), byte addressed. Reads of memory
// never written give 0. The hooks (memory-mapped I/O) see every access of a processor space except the registers' and the
// translator's temporaries'; a hook that returns false leaves the access to the page map. Nothing of the decompiler core shows
// through this header.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace cb::decompile::sleigh {

class Emulator {
public:
    enum class Status {
        Ok,              // one instruction executed
        Stopped,         // run() reached a stop address (before executing it)
        BadInstruction,  // the bytes at pc are no instruction of the processor's (nothing executed)
        Error,           // the p-code failed (message in lastError()); state may be half-updated
    };
    struct RunResult {
        std::uint64_t steps = 0;
        Status status = Status::Ok;   // Ok: ran all n steps
    };
    struct Register {
        std::string name;
        unsigned size = 0;            // bytes
    };

    // `sla`: a compiled description; `context`: the description's context variables to start from (x86's "addrsize"/"opsize": 0 is
    // 16-bit real mode, 1 is 32-bit flat; ARM's "TMode"). std::runtime_error for a file that is not a description.
    Emulator(const std::filesystem::path& sla, const std::vector<std::pair<std::string, int>>& context);
    ~Emulator();
    Emulator(const Emulator&) = delete;
    Emulator& operator=(const Emulator&) = delete;

    // ---- memory (space: the default code space, "ram", unless named). Raw: hooks are not called.
    void load(std::uint64_t addr, const std::uint8_t* data, std::size_t size, std::string_view space = {});
    void write(std::uint64_t addr, const std::uint8_t* data, std::size_t size, std::string_view space = {});
    void read(std::uint64_t addr, std::uint8_t* data, std::size_t size, std::string_view space = {}) const;
    std::uint64_t readValue(std::uint64_t addr, unsigned size, std::string_view space = {}) const;   // in the processor's byte order
    void writeValue(std::uint64_t addr, unsigned size, std::uint64_t value, std::string_view space = {});

    // Memory-mapped I/O: called for every read/write the p-code makes (not for load/write/read above) in a processor space other
    // than the registers'. addr is the byte address. Return true when the hook served the access (a read's `value` is then the
    // result, a write is not stored).
    using ReadHook = std::function<bool(std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value)>;
    using WriteHook = std::function<bool(std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t value)>;
    void onRead(ReadHook hook);
    void onWrite(WriteHook hook);

    // ---- registers, by the description's names ("EAX", "A", "SP", "CF" ...; a sub-register like "AX" reads its part of "EAX").
    bool hasReg(const std::string& name) const;
    std::uint64_t reg(const std::string& name) const;                 // runtime_error for an unknown name; at most 8 bytes of a wider one
    void setReg(const std::string& name, std::uint64_t value);
    // Every register of at most 8 bytes that is not part of a bigger one (the ones a trace shows), in the description's order.
    // parts = true: every register of at most 8 bytes, the parts too (Z80's A and F as well as AF, x86's flags).
    const std::vector<Register>& registers(bool parts = false) const;

    // Program counter: the next instruction's address in the code space's units (a word-addressed space counts words).
    std::uint64_t pc() const;
    void setPc(std::uint64_t pc);

    // ---- running
    Status step();                                    // one instruction
    RunResult run(std::uint64_t n);                   // up to n instructions; stops early at a stop address, a bad instruction or an error
    void addStop(std::uint64_t pc);                   // run() stops before executing an instruction at pc (a step() never does)
    void clearStops();
    std::uint64_t steps() const;                      // instructions executed so far
    const std::string& lastError() const;

    // Called after each executed instruction with its address (pc() is then the next one's). Pass {} to switch off.
    using Trace = std::function<void(std::uint64_t pc, const Emulator& self)>;
    void onTrace(Trace trace);

    // A user operation (the description's CALLOTHER: "in", "out", "interrupt", "SetThumbMode" ...). `args` are the input values;
    // set `result` for an operation that has an output. Return true when served. An operation without a hook (or whose hook returns
    // false) is counted in unimplementedOps() and gives 0; nothing throws. "segment" (x86 real mode) is built in.
    using UserOpHook = std::function<bool(Emulator& self, const std::string& name, const std::vector<std::uint64_t>& args, std::uint64_t& result)>;
    void onUserOp(UserOpHook hook);

    // The bytes at pc are no instruction of the description (undocumented opcodes ...): the host may carry the instruction out itself
    // (it sets the registers and pc, via bus access below if it needs memory-mapped I/O) and return true; the step counts as executed.
    using BadInstructionHook = std::function<bool(Emulator& self, std::uint64_t pc)>;
    void onBadInstruction(BadInstructionHook hook);
    // Memory access as the p-code makes it: through the hooks (default space: the code space).
    std::uint64_t busRead(std::uint64_t addr, unsigned size, std::string_view space = {});
    void busWrite(std::uint64_t addr, unsigned size, std::uint64_t value, std::string_view space = {});
    const std::map<std::string, std::uint64_t>& unimplementedOps() const;   // name -> times reached

    // ---- decoding
    std::string text(std::uint64_t pc) const;         // the instruction at pc as the reader prints it ("" if none)
    std::size_t length(std::uint64_t pc) const;       // its bytes (0: not an instruction)
    // Change a context variable (ARM's "TMode" ...) for instructions decoded from now on.
    void setContext(const std::string& name, int value);
    // Drop decoded instructions (writes to a page code was decoded from do this by themselves).
    void flushDecoded();

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace cb::decompile::sleigh
