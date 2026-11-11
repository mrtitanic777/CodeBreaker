// codebreaker/verify/lanes/driver.hpp -- typed C++ of codebreaker.verify.lanes.driver.
//
// driver.py -- what is particular to judging a rebuilt virtual device driver.
//
//     python -m codebreaker driver-judge <rebuilt.vxd> <original.vxd>     every entry point, judged
//     python -m codebreaker driver-judge --self-check <original.vxd>      the judge judged: the original
//                                                                         against itself, then against
//                                                                         copies with one instruction broken
//
// One of the kinds reconstruct.py works with (KIND, profile, prepare, build, judge). A driver is
// flat 32-bit ring-0 code. It is entered at its control procedure (EAX = a system control
// message, EBX = a virtual machine, EBP = the client's registers), at each service it offers
// other drivers, and at its two API procedures; arguments are in registers, sometimes on the
// stack; the answer is in EAX, the carry and zero flags and whatever registers the procedure
// documents. It calls the system and other drivers with `int 20h` + service + device.
//
// Each entry point is run in the emulator, in the original and in the rebuilt driver, from the
// same registers, stack and memory, with both images laid out the same way and patched as the
// loader would patch them (every object at an address of its own, every internal reference
// resolved). What a run did is a fingerprint -- how it ended, EAX, the service calls in order with
// the registers and stack words they were given, the port reads and writes, the memory left
// behind, the flags and registers handed back, how far the stack moved -- and the two fingerprints
// go to the one comparer (verify/compare.py), which gives the verdict.
//
// A service call returns what the input says (EAX, carry), so both sides see the same system; the
// few services whose whole point is a register (the current virtual machine in EBX) return that.
// Addresses inside the driver are compared as they are when the two files have the same objects,
// and as "somewhere in the driver" when they do not (then the driver's own memory is compared as
// the order of values stored, and the verdict carries the limit `layout-differs`).
//
// The run itself is emulate/driver.py. What runs an entry point, what writes and links the
// rebuild and where the entry points are come from the platform (codebreaker/platforms/vxd.py):
// its emulator, packager and loader.
//
// Typing notes:
//   * The Python module IS a Kind. Here the free functions below are the module's and the reconstruct::Kind that forwards to them is kind() ("le").
//   * `P = platforms.of("le")` has no typed registry: its slots are what they hold -- the emulator emulate::driver::recorded, the packager
//     source::driver_writer::reconstruct (writer) / build::driver::build (functions) / platforms::vxd::tools(), the loader platforms::vxd::entries, the
//     processor's decoder match::decoder(32).
//   * An input (the Python's dict {regs, stack, memory, stub_ret, stub_carry, flags, args}) is `Vector`: a compare::Input (args, stub_ret) plus the rest.
//   * What a fingerprint carries beyond compare::Fingerprint (the Python adds `result` and `memory` to the dict) is `Detail`, which only explain() reads: it
//     is made again (an uncached run, the same run) for the two fingerprints of the last input when a verdict needs explaining.
//   * What prepare() hands reconstruct is reconstruct::Prepared whose `lane` is a std::shared_ptr<Module> (the Python's m.source / m.written).
//   * The path explorer's inputs are found with emulate::shadow::ShadowLE; a register input's `where` (a name in the Python) is the code REG_CODE of it.
//   * judge_files() runs the entry points one after another in this process (the emulators keep per-process state).
//   * main() and self_check() print / read what they print as the Python's do (stdout).
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/verify/authored/match.hpp"   // (before verify/compare.hpp: its `compare::mask` means codebreaker::compare there)
#include "codebreaker/emulate/driver.hpp"
#include "codebreaker/source/driver_writer.hpp"
#include "codebreaker/verify/compare.hpp"
#include "codebreaker/verify/concolic.hpp"
#include "codebreaker/verify/lane_common.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "py.hpp"

namespace codebreaker::verify::lanes::driver {

extern const py::str KIND;            // "le"
inline constexpr int64_t REV = 4;     // what a verdict means; raise when run() or the inputs change (4: inputs found path by path beyond the fixed ones)

// An input to a procedure.
struct Vector : compare::Input {
    py::dict<py::str, int64_t> regs;                               // eax ebx ecx edx esi edi ebp
    py::list<int64_t> stack;
    py::list<std::tuple<int64_t, py::bytes>> memory;
    int64_t stub_carry = 0;
    int64_t flags = 1;
};

// The register a shadow variable of kind "reg" names, as a number (the Python's `where` is the name): eax=1 ebx=2 ecx=3 edx=4 esi=5 edi=6 cf=7 zf=8 ebp=9
int64_t REG_CODE(const py::str& name);

// What a fingerprint says beyond compare::Fingerprint (the Python's dict keys `result` and `memory`).
using Held = std::variant<int64_t, py::str>;
struct Detail {
    std::optional<std::tuple<int64_t, py::str>> memory;    // (bytes changed, digest); nullopt: too many to record
    py::list<Held> result;                                  // (flags, the result registers): empty for a run that did not return
};

// From a run to a fingerprint ---------------------------------------------------------------------------------------------------------
// A value as two builds of the same code agree on it (lane_common.argument): an address in the stack is "S"; one in the driver is "P", somewhere in the
// driver, when the two files are not laid out alike.
py::str _value(const emulate::driver::Image& image, int64_t v, bool aligned);
py::str _service(const std::optional<int64_t>& device, int64_t service);

// What the run left in memory outside its stack: (bytes changed, digest). Memory outside the driver by address; the driver's own by address too when the two
// files are laid out alike, else as the values stored in the order they were stored. nullopt: too many to record.
std::optional<std::tuple<int64_t, py::str>> _effects(const emulate::driver::Image& image, const emulate::run::Run& r, bool aligned);

// One run as the comparer wants it (see verify/compare.py). `aligned`: the other image has the same objects, so addresses inside the driver mean the same on
// both sides.
compare::Fingerprint fingerprint(const emulate::driver::Image& image, int64_t entry, const Vector& v, bool aligned = true);
// the run itself, with what explain() reads
std::tuple<compare::Fingerprint, Detail> _run(const emulate::driver::Image& image, int64_t entry, const Vector& v, bool aligned);

// The lift mode of the platform's processor (emulate/shadow.py mode: its cpu slot).
std::optional<py::str> _mode();
bool concolic_ready();
// [ready, PATH_BUDGET, MAX_PATHS, stamp or ""] as the profile writes it
py::str _paths_profile();

// Input `v` with the named inputs (shadow.vars) set to `values`; nullopt when nothing changed.
std::optional<Vector> _derive(const Vector& v, const concolic::Values& values, const concolic::Vars& names);

// (inputs, exploration): inputs taking paths of the original no seed took, found path by path, and how far the exploration got.
std::tuple<py::list<Vector>, std::shared_ptr<concolic::Exploration<Vector>>> explore_vectors(const emulate::driver::Image& image, int64_t entry,
                                                                                         const py::list<Vector>& seeds);

// One line on what differs between two fingerprints the comparer found different.
py::str explain(const compare::Fingerprint& a, const Detail& da, const compare::Fingerprint& b, const Detail& db);

// Inputs -----------------------------------------------------------------------------------------------------------------------------------
// The objects register arguments point at: a virtual machine block, the client's registers, a buffer, and a table of pointers that each lead to a plain `ret`.
py::list<std::tuple<int64_t, py::bytes>> _memory(const std::optional<int64_t>& fill = std::nullopt);
// One input. The carry flag is SET on entry unless said otherwise: a procedure reports success by clearing it, and one that forgot to would pass unnoticed if it
// came in clear.
struct VectorArgs {
    int64_t eax = 0, ebx = emulate::driver::VM, ecx = 0, edx = 0, esi = emulate::driver::BUFFER, edi = emulate::driver::OBJECT;
    py::list<int64_t> stack;
    std::optional<int64_t> fill;
    int64_t ret = 0, carry = 0, flags = 1;
};
Vector _vector(const VectorArgs& a = VectorArgs());
// (first inputs, further inputs run only when the first all agree) for an entry point.
std::tuple<py::list<Vector>, py::list<Vector>> inputs(const py::str& label);

// The kind interface (see verify/reconstruct.py) ------------------------------------------------------------------------------------------------
// (digest, settings): everything but the source a verdict depends on.
std::tuple<py::str, py::str> profile(const std::optional<py::str>& module_path = std::nullopt);
py::str key_of(const formats::le::Place& where);
std::tuple<bool, bool, int64_t, py::str> _entry_order(const py::str& label);
// Where the assembly source of a corpus driver is: <sources>/<name>/, with <sources> from CB_DRIVER_SOURCES, else the vmm32 folder of the source tree beside the
// corpus. '' if none.
py::str source_folder(const py::str& module_path);

// What prepare() keeps of the module for build().
struct Module {
    py::str source;                                                 // the assembly folder of CB_DRIVER_SOURCES, or ""
    std::optional<source::driver_writer::Reconstruction> written;    // the driver written from the image
};
// The driver's source: the assembly file in the source tree when CB_DRIVER_SOURCES names one to judge, otherwise written from the image
// (source/driver_writer.py) and built.
reconstruct::Prepared prepare(const py::str& module_path, const py::str& work);

// What judge() is handed once for every entry point: (original file, rebuilt file or None) and (key, label).
struct Context {
    py::str original;
    std::optional<py::str> rebuilt;
};
struct Item {
    py::str key, label;
};
// -> (context for judge, {label: item}, {label: why it is not in the build})
reconstruct::Built build(const reconstruct::Prepared& m, const py::str& module_path, const py::str& work);

// Worker: judge one entry point. -> (verdict, reason, limits, detail)
reconstruct::Verdict judge(const Context& context, const Item& item);

// Judge every entry point of `original` (or those in `labels`) in `rebuilt`. -> {label: (verdict, reason, limits, detail)} in entry order.
py::list<std::tuple<py::str, reconstruct::Verdict>> judge_files(const py::str& rebuilt, const py::str& original,
                                                                const std::optional<py::list<py::str>>& labels = std::nullopt);

// the images read so far (the Python's _IMAGES); a file replaced is forgotten with this
void forget_image(const py::str& path);

// The judge judged -----------------------------------------------------------------------------------------------------------------------------
// Write a copy of `original` with one instruction of the procedure at `label` broken: the nth instruction (in address order) that runs on the first input, with one
// bit of its last byte flipped. -> (file offset changed, the instruction's text) or None when there is no such one.
std::optional<std::tuple<int64_t, py::str>> break_instruction(const py::str& original, const py::str& out, const py::str& label, int64_t nth = 0);

struct SelfCheck {
    std::vector<std::tuple<py::str, int64_t>> self;                  // {verdict: count}, in the order first met
    int64_t broken = 0, caught = 0;
    py::list<std::tuple<py::str, py::str, py::str>> missed;       // (label, instruction, verdict)
};
// Judge the judge on one driver: the original against itself must come out with nothing divergent; then, for up to `entries` entry points that came out equivalent,
// `per_entry` copies each with one executed instruction broken are judged -- a judge with teeth reports them divergent (verify/teeth.plant, the harness).
SelfCheck self_check(const py::str& original, const std::optional<py::str>& work = std::nullopt, int64_t per_entry = 3, int64_t entries = 12);

extern const py::str DOC;
// python -m codebreaker driver-judge <rebuilt.vxd> <original.vxd> | --self-check <original.vxd>
int64_t main(const py::list<py::str>& argv);

const std::shared_ptr<reconstruct::Kind>& kind();
void register_lane();

}  // namespace codebreaker::verify::lanes::driver
