// cb/emulate/package.hpp -- emulate: run a piece of the original (or of a rebuild) and record what it did.
//
// One runner per kind of code, each returning the one run record (cb/emulate/run.hpp) -- how the run ended, the
// result registers, the memory it wrote, the calls and device accesses out of it in order, how the stack moved, the
// registers it had to keep, which of its instructions ran:
//
//     kind       runner                                  the code it runs
//     pe         the 32-bit tracer (trace_function)      a function of a 32-bit Windows module
//     ne         the 16-bit tracer (probe)               a function of a 16-bit Windows module
//     le         the driver runner                       an entry point of a virtual device driver (flat ring 0)
//     powerpc    the PowerPC interpreter                 a function of a PowerPC image
//     nes ...    the 6502 / Z80 / SM83 / 68000 machines  a function of a console or home-computer program
//
// The x86 runners are one engine (Unicorn) with one set of memory hooks (what was read and written, pages given on
// demand, runaway runs cut off); they differ in how the code is laid out and entered, which is what a kind is.
// PowerPC is a processor Unicorn does not run usefully in this mode, so it is an interpreter of its own, written
// to give the same record.
//
// What a record means for a reconstruction -- two runs compared -- is not here: the lanes in cb/verify turn a record
// into a fingerprint and the comparer compares two.
//
// Each platform names the runner of its kind when it registers (the platform registry, cb/platforms). A runner has
// its own typed signature -- the tracer takes a module and an address, a console machine a ROM -- so no one type
// holds them all: this table is the *name* of the runner each kind has, which the registry fills and the tools that
// list or select by kind read.
#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace cb::emulate {

struct RunnerName {
    std::string module;      // the emulator ("emulator", "windows16", "driver", "powerpc", "m6502" ...)
    std::string function;    // its entry that returns a Run ("recorded")
};

// {platform kind: the runner of its code}, in the order the kinds were added. Owned by whoever builds the platform
// registry; not global.
class RunnerTable {
public:
    // Add `kind`'s runner; a kind added twice keeps the later one.
    void add(std::string kind, RunnerName runner);

    // The runner of `kind`: absent when no platform runs code of that kind.
    const RunnerName* find(std::string_view kind) const;
    // The same where absence is a mistake: throws UsageError naming the kind.
    const RunnerName& at(std::string_view kind) const;

    std::vector<std::string> kinds() const;

private:
    std::vector<std::pair<std::string, RunnerName>> entries_;
};

}  // namespace cb::emulate
