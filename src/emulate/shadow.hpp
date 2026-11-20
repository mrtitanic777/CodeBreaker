// emulate/shadow.hpp -- which values of a PowerPC run are made from its inputs, and from which: the run's symbolic shadow, kept up
// beside the interpreter instruction by instruction.
//
//     PowerPcShadow shadow(lifter, [](std::uint64_t address) { return in_the_arguments_memory(address); });
//     shadow.enter(places, 64);                      // argument k is in r3 + k ...
//     powerpc::RunOptions options;  options.shadow = &shadow;
//     powerpc::run(regions, entry, args, options);   // the interpreter steps it before each instruction
//     const native_shadow::Run& run = shadow.run();  // run.path, run.trail, run.vars, run.lost
//
// The run itself is the interpreter's: nothing here changes it. Before each instruction runs, its p-code (emulate/pcode, the
// decompiler's own specification) is evaluated twice over at once -- concretely, from the registers and memory as they are at that
// moment, and symbolically for the values that come from an input. Only the symbolic half is kept: which register bytes and memory
// bytes hold an expression over the inputs. A conditional jump whose condition is such an expression is a condition of the path; its
// direction is the one the interpreter then takes (the next instruction says which).
//
// The inputs are the function's argument words (in the registers they are passed in), any byte of memory the `is_input` test names
// that the run has not written yet (`m<address>`), and what a call the environment answers returns (`ret`). An address made from an
// input is held at the value it had (a Pin in the path: the pointer as given). A value made from an input that leaves through
// something not followed (a user operation, an operation without a meaning here, an instruction that does not decode) makes the path's
// conditions possibly short of all there are, and says so in `run().lost`. Whatever goes wrong inside the shadow ends the shadow (`off`),
// never the run.
//
// The x86 shadows are the recorder's (cb/emulate/native_shadow over emulate/tracer): a run in Unicorn steps them from the hooks. Their
// results are the same types (native_shadow::Run), so the path explorer reads either kind alike.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "emulate/native_shadow.hpp"
#include "emulate/pcode.hpp"
#include "emulate/powerpc.hpp"
#include "emulate/symbolic.hpp"

namespace cb::emulate::shadow {

// Conditions kept per run; past it the run is followed no further (`lost` says "path-capped").
inline constexpr std::size_t kPathCap = 400;

// The lift mode (emulate/pcode) for a platform's processor slot (the language of platforms: its SLEIGH language id), or nothing where
// there is no lift: "x86:LE:16..." is Real16, "x86:LE:32..." Flat32, "PowerPC:BE..." PpcBe64.
std::optional<pcode::Mode> lift_mode(std::string_view cpu_language);

// A selector's base in real mode (what a 16-bit shadow of a real-mode run uses): selector * 16.
constexpr std::uint32_t real_mode_base(std::uint32_t selector) { return selector * 16u; }

// The shadow of one PowerPC run (the interpreter of emulate/powerpc): a call out of the code returns `ret` (64 bits) in r3 and changes
// nothing else; arguments are r3-r10 (enter). One shadow is for one run on one thread.
class PowerPcShadow final : public powerpc::Shadow {
public:
    // Whether a byte of memory is one of the run's inputs until the run writes it.
    using InputFn = std::function<bool(std::uint64_t address)>;

    // `lifter` (the ppc-be-64 mode's) must outlive the shadow. UsageError for a lifter of another mode.
    PowerPcShadow(pcode::Lifter& lifter, InputFn is_input);
    PowerPcShadow(const PowerPcShadow&) = delete;
    PowerPcShadow& operator=(const PowerPcShadow&) = delete;
    ~PowerPcShadow() override;

    // The run is about to start: each place, `bits` wide, is an input -- an argument (named a<k>) or a register's own value. Places are
    // registers by name ("r3"); a memory place is an address. UsageError for a register the lifter has not.
    void enter(std::span<const native_shadow::Place> places, int bits = 64);

    // Before the instruction at `pc` runs.
    void step(powerpc::Machine& machine, std::uint32_t pc) override;
    // A call the environment answered: r3 was set to `ret`, an input when `symbolic_result`; nothing else changed.
    void after_call(powerpc::Machine& machine, bool symbolic_result, std::uint64_t ret) override;

    // Registers the environment set (not an instruction): no longer made from anything.
    void forget(std::span<const std::string> names);
    // Memory the environment wrote (not an instruction): no longer an input, nor made from one.
    void written(std::uint64_t at, std::uint64_t size);

    // What the run made so far. The conditions are nodes of the run's own pool: hold the pointer for as long as they are used.
    const native_shadow::Run& run() const { return *run_; }
    std::shared_ptr<const native_shadow::Run> shared_run() const { return run_; }

    struct Impl;

private:
    std::shared_ptr<native_shadow::Run> run_;
    std::unique_ptr<Impl> impl_;
};

}  // namespace cb::emulate::shadow
