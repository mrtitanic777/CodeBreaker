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
// translator's temporaries'; a hook that returns false leaves the access to the page map. A varnode may be wider than 8 bytes (up
// to 256: vector registers, the 128-bit temporaries of a widening multiply, the x87's 10 bytes): an instruction that has one runs by
// a separate wide path chosen when it is decoded, with every integer operation at any width and the extended float. Nothing of the
// decompiler core shows through this header.
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
    void clearMemory(std::string_view space = {});   // every page of the space forgotten (it reads zeros again); decoded instructions dropped
    void read(std::uint64_t addr, std::uint8_t* data, std::size_t size, std::string_view space = {}) const;
    // Where the bytes of a page the map does not hold come from, for a memory that is mostly an image written once and read a little (a page is made the first time a run
    // looks at it, instead of all of them when the image is written). The source fills `out` with the bytes of [address, address + size) (zeros where it has none) and says
    // whether it had any; with `out` null it only says whether it has any. The pages written since are the map's: write() merges into a page the source gave, writeHeld()
    // writes only the pages the map holds already, read() takes a page the map does not hold from the source without making it. {} switches the source off.
    using PageSource = std::function<bool(std::uint64_t address, std::uint8_t* out, std::size_t size)>;
    void setPageSource(PageSource source, std::string_view space = {});
    void writeHeld(std::uint64_t addr, const std::uint8_t* data, std::size_t size, std::string_view space = {});
    std::uint64_t readValue(std::uint64_t addr, unsigned size, std::string_view space = {}) const;   // in the processor's byte order
    void writeValue(std::uint64_t addr, unsigned size, std::uint64_t value, std::string_view space = {});

    // Memory-mapped I/O: called for every read/write the p-code makes (not for load/write/read above) in a processor space other
    // than the registers'. addr is the byte address. Return true when the hook served the access (a read's `value` is then the
    // result, a write is not stored).
    //
    // A p-code access wider than 8 bytes (a vector register's LOAD or STORE, the x87's 10 bytes) reaches the same hooks as the accesses of at most 8 bytes that make it up, in
    // address order: the pieces are 8 bytes each from the lowest address and a last one for what is left (16 bytes: two of 8; 10 bytes: 8 then 2). Each piece is valued as an
    // access of its size in the space's own byte order is: in a little-endian space the lowest address holds the least significant byte of `value`, in a big-endian one the
    // most significant (so a big-endian vector's first piece is its high half). Each piece is served or declined on its own; a declined piece is read from or written to the
    // page map. The hooks never see a size above 8.
    using ReadHook = std::function<bool(std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value)>;
    using WriteHook = std::function<bool(std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t value)>;
    void onRead(ReadHook hook);
    void onWrite(WriteHook hook);

    // ---- registers, by the description's names ("EAX", "A", "SP", "CF" ...; a sub-register like "AX" reads its part of "EAX").
    bool hasReg(const std::string& name) const;
    std::uint64_t reg(const std::string& name) const;                 // runtime_error for an unknown name; at most 8 bytes of a wider one
    void setReg(const std::string& name, std::uint64_t value);
    // A register found once by name and then read and written without looking the name up (what an adapter does for the registers it reads and writes on every
    // step or every instruction it hands to another interpreter). Valid for the Emulator it came from.
    struct RegRef {
        std::size_t space = 0;
        std::uint64_t offset = 0;
        unsigned size = 0;
    };
    RegRef regRef(const std::string& name) const;
    std::uint64_t reg(const RegRef& ref) const;
    void setReg(const RegRef& ref, std::uint64_t value);
    // `size` bytes of the register space from the register's place on (the registers that lie after it too), in one copy: what an adapter saves and puts back around an
    // instruction it may have to undo.
    void readRegs(const RegRef& from, std::uint8_t* out, std::size_t size) const;
    void writeRegs(const RegRef& to, const std::uint8_t* data, std::size_t size);
    // A register (or the bytes from it on) of any size as a value: `ref.size` bytes, LEAST significant byte first whatever the register space's byte order (readRegs and
    // writeRegs copy the bytes as the space holds them: reversed for a big-endian space). What an adapter uses for a vector register.
    void readRegWide(const RegRef& ref, std::uint8_t* out) const;
    void writeRegWide(const RegRef& ref, const std::uint8_t* data);
    // Every register of at most 8 bytes that is not part of a bigger one (the ones a trace shows), in the description's order.
    // parts = true: every register of at most 8 bytes, the parts too (Z80's A and F as well as AF, x86's flags).
    const std::vector<Register>& registers(bool parts = false) const;

    // Program counter: the next instruction's address in the code space's units (a word-addressed space counts words).
    std::uint64_t pc() const;
    void setPc(std::uint64_t pc);

    // ---- running
    Status step();                                    // one instruction
    // Blocks: up to `maxSteps` instructions run as that many step() calls would run them (registers, memory, hooks, step counts and the pc at every exit are the same), but
    // the straight-line instructions are translated once and run together. The call returns early, after the instruction, when a hook (memory or user operation) was
    // called, when bytes code was decoded from changed, or when the pc reached `stopPc`: wherever the caller could have looked at the state. `status` is that of the
    // instruction that could not run (BadInstruction or Error: it is not counted), else Ok. With CB_PCODE_BLOCKS=0 (or setBlocksOverride(0)), with a trace callback or a stop
    // address set, one instruction runs per call.
    struct BlockResult {
        std::uint64_t executed = 0;
        Status status = Status::Ok;
    };
    BlockResult runBlocks(std::uint64_t maxSteps, bool useStop = false, std::uint64_t stopPc = 0);
    // A register put back to `value` after every instruction run by runBlocks (an adapter fixing a width the description does not keep), and a register that counts the
    // instructions: before each instruction its bits under `mask` are advanced by amount(pc). Both apply in runBlocks only, not to a plain step(). Cleared with clearPin / clearCounter.
    void setPin(const RegRef& ref, std::uint64_t value);
    void clearPin();
    void setCounter(const RegRef& ref, std::uint64_t mask, std::function<std::uint64_t(std::uint64_t pc)> amount);
    void clearCounter();
    // Forces blocks on (1) or off (0) for every Emulator, or leaves it to the environment (-1): what the tests compare.
    static void setBlocksOverride(int mode);
    // One p-code operation on values, by the form blocks run (writtenOut) or by the description's behaviour table: what the test compares. False: no such form.
    bool evaluateOp(unsigned opcode, int sizeOut, int sizeIn, std::uint64_t a, std::uint64_t c, std::uint64_t& result, bool writtenOut) const;
    // The same for the wide path (values of up to 256 bytes, least significant byte first; a float operation has 10-byte, x87, values on at least one side): the operation `opcode`
    // with inputs `a` (aSize bytes) and `c` (cSize bytes; a shift count or a SUBPIECE's byte offset) into `out` (outSize bytes). False: no such operation or size. A division by
    // zero is reported in `divideByZero` (the output is then untouched).
    static bool evaluateWideOp(unsigned opcode, std::uint8_t* out, std::size_t outSize, const std::uint8_t* a, std::size_t aSize, const std::uint8_t* c, std::size_t cSize,
                               bool& divideByZero);
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
    // The same for a user operation with a value wider than 8 bytes (a vector register as an argument or the result): `args` are the inputs as byte arrays, LEAST
    // significant byte first, each of its varnode's size; `result` is sized to the output (empty without one) and is to be filled the same way. When this hook is set it
    // serves every user operation that has such a value (a return of false counts the operation in unimplementedOps()); operations whose values are all of at most 8 bytes
    // go to onUserOp as before. Without it a wide value reaches onUserOp as its low 8 bytes and a wide result is that value zero extended.
    using WideUserOpHook = std::function<bool(Emulator& self, const std::string& name, const std::vector<std::vector<std::uint8_t>>& args, std::vector<std::uint8_t>& result)>;
    void onUserOpWide(WideUserOpHook hook);

    // A user operation served by its number, with no name looked at while a program runs (what an instruction set's table of lane operations is bound to, once, when the
    // emulator is made: engine/pcode_ops.hpp resolves each name of the description to a handler and calls bindUserOp). A bound operation is served before onUserOp and
    // onUserOpWide are asked, whatever the sizes of its values (a value of at most 8 bytes is given as that many bytes too, least significant first), so there is one form
    // of handler for narrow and wide arguments. It costs no allocation per call. An operation with more than kMaxArgs inputs, or a value over 256 bytes, is not served this
    // way (it goes to the hooks as before). A handler that returns false counts the operation in unimplementedOps() as a hook that declined.
    struct UserOpCall {
        static constexpr unsigned kMaxArgs = 8;
        unsigned nIn = 0;                                 // inputs, not counting the operation's own number
        unsigned inSize[kMaxArgs] = {};                   // bytes of each input
        const std::uint8_t* in[kMaxArgs] = {};            // each input's bytes, least significant first
        unsigned outSize = 0;                             // bytes of the output; 0: the operation has none
        std::uint8_t* out = nullptr;                      // the output, cleared before the call, to be filled the same way
    };
    using UserOpHandler = bool (*)(Emulator& self, UserOpCall& call, void* context);
    std::size_t userOpCount() const;                      // the description's user operations, numbered from 0
    const std::string& userOpName(std::size_t index) const;
    void bindUserOp(std::size_t index, UserOpHandler handler, void* context = nullptr);   // a null handler unbinds
    void clearUserOpBindings();

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
    // Whether the context a decoded instruction sets for other addresses (SLEIGH's globalset: ARM's Thumb state at a BLX's target, the "mov lr,pc" before a call) is kept
    // for the decodes that follow (the default, as a disassembler wants it) or dropped after each one (an emulator that runs the state itself: a run must not be
    // decoded under what another run's flow left).
    void keepAddressContext(bool keep);
    // Drop decoded instructions (writes to a page code was decoded from do this by themselves).
    void flushDecoded();

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace cb::decompile::sleigh
