#pragma once
// A translator of single instructions that the recompiler asks first (CodeBreaker's p-code translator: src/recompile/pcode_emit).
// The recompiler keeps everything around an instruction: functions, labels, branches, calls, switch tables, the flush mode, the
// local variables. For any other instruction it asks the translator; when it declines (no value), its own translation is written.
//
// Set it for the runs of one thread (the in-process engine runs one recompile at a time); unset, nothing changes.
#include <cstdint>
#include <functional>
#include <optional>
#include <string>

namespace xenon_translate {

// How the recompiler names each register in the function being written: a context field or a local, as its configuration says.
struct Names {
    std::function<std::string(std::size_t)> r, f, v, cr;      // "ctx.r3" or "r3" ... (the field's members are the translator's)
    std::function<std::string()> ctr, xer, lr;                // lr: empty when the configuration does not keep LR
};

struct Translated {
    std::string text;      // the statements, each line starting with a tab and ending with a newline
    bool fpu = false;      // it computes with the floating-point unit: the recompiler sets the FPU's flush mode first
};

using Translator = std::function<std::optional<Translated>(std::uint32_t address, std::uint32_t word, std::uint32_t next_word, const Names& names)>;

// The translator asked for every instruction from now on, on this thread; an empty one switches it off.
void set(Translator translator);
const Translator& current();

// How many instructions the translator wrote and declined since the last reset.
struct Counts {
    std::size_t translated = 0;
    std::size_t declined = 0;
};
Counts& counts();

}  // namespace xenon_translate
