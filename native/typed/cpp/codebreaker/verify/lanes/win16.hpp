// codebreaker/verify/lanes/win16.hpp -- interface of codebreaker.verify.lanes.win16 (typed C++; implementation pending).
//
// win16.py -- what is particular to reconstructing a 16-bit Windows module.
//
// One of the kinds reconstruct.py works with (see reconstruct.hpp for what a kind provides). Here: the module is
// decompiled and made compilable by decompile/c_text, every function is compiled and a 16-bit DLL linked by
// build/functions, and each function is run beside the original in the 16-bit emulator (emulate/windows16).
//
// Both sides are entered the way the function is really called (far or near) with the same words on the stack, and the
// rebuilt module's data segment is given the original's data.  Compared: how the run ends, the Windows calls and device
// ports used (in order, with arguments), the result in AX (DX:AX), what is left in the data segment and on the far heap,
// the bytes of arguments removed, and whether SI, DI and DS were preserved.  Inputs: the fixed ones (vectors()), the
// directed ones (the constants the code compares with) and inputs found path by path (verify/concolic) or guessed
// (verify/guided).
//
// Typing notes:
//   * An input (the Python dict {"args", "mem", "stub_ret", "guided", "noisy", "context"}) is the record `Vector`, which is a
//     compare::Input (args, stub_ret) plus the lane's fields.  `mem` is a list of (where, bytes) with `where` a linear
//     address or ("D", offset in the module's data segment, wherever this build has it): windows16::Where.
//   * A fingerprint is compare::Fingerprint plus the lane's `trail` (the path of the function's own jumps): `Fingerprint16`.
//     A run that was not an observation is Fingerprint::err set (the Python's {"err": type name}).
//   * `P` (platforms.of("ne")) is the platform record `P()`; the emulator and the packager it names are called directly
//     (emulate::windows16, build::functions::build16) by the implementation.
//   * emulate/windows16's record of a run (probe()'s dict), a Windows call it made (`hit`) and a replayed call context are
//     emulate::windows16::{Trace16, Hit, Context}; the Run's `raw` holds the ProbeRecord (the Python: r.raw).
//     "**probe" (more options for every run: callees, data_end, insn_cap, ports ...) is emulate::windows16::ProbeOptions.
//   * The lane's judge context ((module_path, dll, data_image, own, order) tuple) and item (a tuple of 9-10) are records.
//   * "Prepared" is the c_text Module16 plus the three attributes prepare() adds (key, all_keys, sig).
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/decompile/c_text.hpp"
#include "codebreaker/emulate/run.hpp"
#include "codebreaker/emulate/windows16.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/platforms/win16.hpp"
#include "codebreaker/verify/compare.hpp"
#include "codebreaker/verify/concolic.hpp"
#include "codebreaker/verify/guided.hpp"
#include "codebreaker/verify/lane_common.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py.hpp"

namespace codebreaker::verify::lanes::win16 {

using Hit = emulate::windows16::Hit;                    // one Windows call a run made
using ProbeRecord = emulate::windows16::Trace16;        // windows16.probe()'s record of a run (Run.raw holds it)
using ProbeOptions = emulate::windows16::ProbeOptions;  // the optional keywords of probe() (callees, data_end, insn_cap, ports ...)
using Context = emulate::windows16::Context;            // a call recorded in the running system, replayed

// 16-bit Windows: the six slots this lane asks (platforms.of("ne"))
const platforms::win16::Platform& P();

// the judge's words (verify/verdicts.py)
const verdicts::NotRun& NOT_RUN();

inline constexpr int64_t LEFTOVER = 0x7EF707E5;   // stands for "AX held no result" in a fingerprint
inline constexpr int64_t REV = 23;                // bump when what is built or compared changes
inline constexpr int64_t RETRY = 8;               // times the emulator's step limit a rebuild still going is given
inline constexpr int64_t CALLER_BP = 0x8000;      // BP of a caller-frame helper's run (a caller's frame, below the run's own stack)
inline constexpr int64_t FRAME_FILL = 0xA5;       // what the helper's places start out holding on both sides
extern const py::str KIND;                        // "ne"
inline constexpr bool PER_FUNCTION = true;        // a verdict holds while the function and what it reaches are unchanged

// ---- inputs -------------------------------------------------------------------------------------------------------
// memory an input sets before the run: (linear address | ("D", offset), bytes)
using MemInit = emulate::windows16::MemInit;

struct Vector : compare::Input {   // args: eight words; stub_ret: what an unmodelled Windows call returns
    std::optional<MemInit> mem;
    bool guided = false;                          // found by the explorer (branch-guided / path by path)
    bool noisy = false;                           // run over a patterned data segment
    std::shared_ptr<Context> context;             // a recorded call to replay (isolated16's `inputs`), else null
};

// The inputs every function is run on: eight words of arguments each.
py::list<Vector> vectors();

// Inputs aimed at what the function's own code tests: the constants it compares its values with.
// span: (start, end) offsets of the function in its segment.
py::list<Vector> directed(const py::str& module_path, int64_t seg, const std::tuple<int64_t, int64_t>& span, int64_t most = 8);

// ---- a caller-frame helper ----------------------------------------------------------------------------------------
// [(BP less, bytes)] of a caller-frame helper's parameters, in their order
using Frame = py::list<std::tuple<int64_t, int64_t>>;
// from its C; or None
std::optional<Frame> frame_slots(const py::str& text, const py::str& name);

// ---- fingerprints -------------------------------------------------------------------------------------------------
struct Fingerprint16 : compare::Fingerprint {
    py::list<std::tuple<int64_t, bool>> trail;    // [(site, taken)] of the function's own jumps (empty without a span)
};

// One Windows call as text (lane_common.call): its name, and its arguments when their size is known.
// `inputs`: the input's words; `entry`: the registers on entry ({"DS", "SS" ...}); `segs`: selector -> the original's segment name.
py::str _call(const Hit& hit, const py::list<int64_t>& inputs, const py::dict<py::str, int64_t>& entry,
              const std::optional<py::dict<int64_t, py::str>>& segs = std::nullopt);

// What one run of one function did, in a form two builds can be compared on.
//   wide: None = the function returns nothing; own: ranges of the data segment that are the build's own (writes left out);
//   frame: a caller-frame helper's places; ran: the record of this very run, made already; span: also the path
//   ("trail"); segs: the module's code segment selectors as _call names them; steps: the instructions a run may take.
Fingerprint16 fingerprint(const py::str& path, int64_t seg, int64_t off, bool near_, const std::optional<bool>& wide,
                          const Vector& v, const std::optional<py::bytes>& data_image = std::nullopt,
                          const std::optional<std::tuple<int64_t, int64_t>>& span = std::nullopt,
                          const py::list<std::tuple<int64_t, int64_t>>& own = py::list<std::tuple<int64_t, int64_t>>(),
                          const std::optional<Frame>& frame = std::nullopt,
                          const std::optional<emulate::run::Run>& ran = std::nullopt,
                          const std::optional<py::dict<int64_t, py::str>>& segs = std::nullopt,
                          const std::optional<int64_t>& steps = std::nullopt);

// The run's effects outside itself, in the order it had them: each Windows call (`call(hit)`: the hits are the run's raw
// ProbeRecord.imports_hit, parallel to r.calls) and each access to a device -- `out(3fb,83)`, `in(3fd)`.
py::list<py::str> _effects(const emulate::run::Run& r, const std::function<py::str(const Hit&)>& call);

// ---- the explorer's inputs ----------------------------------------------------------------------------------------
bool concolic_ready();

// [(site, taken)] -- the function's own conditional jumps in the order a run took them, from the run's instructions
// (the run's linear addresses).
concolic::Trail trail(const py::str& module_path, int64_t seg, const std::tuple<int64_t, int64_t>& span,
                      const py::list<int64_t>& path);

// The explorer reads an input's words the other way round, and its memory as linear addresses.  ds: the data segment's base.
guided::Input _to_explorer(const Vector& v, const std::optional<int64_t>& ds);
Vector _from_explorer(const guided::Input& g, const std::optional<int64_t>& ds);

// The original run on `v` with its branches logged, as the explorer reads it, or None when the run is not an observation.
std::optional<guided::Observation> _branch_run(const py::str& module_path, int64_t seg, int64_t off, bool near_,
                                               const std::tuple<int64_t, int64_t>& span, const Vector& v,
                                               const std::optional<ProbeOptions>& probe = std::nullopt);

// Inputs that take branches of the original no seed took (verify/guided.py).
py::list<Vector> guided_vectors(const py::str& module_path, int64_t seg, int64_t off, bool near_,
                                const std::tuple<int64_t, int64_t>& span, const py::list<Vector>& seeds,
                                const std::optional<ProbeOptions>& probe = std::nullopt);

// Input `v` with the named inputs (shadow.vars) set to `values`; None when nothing changes.
std::optional<Vector> _derive(const Vector& v, const concolic::Values& values, const concolic::Vars& names,
                              const std::optional<int64_t>& ds);

// What explore_vectors answers: (inputs, exploration, [(input, record)]).  `exploration` is None where the explorer cannot
// be used (guided_vectors then guessed the inputs).
struct Explored {
    py::list<Vector> inputs;                                        // inputs taking paths of the original no seed took
    std::shared_ptr<concolic::Exploration<Vector>> exploration;     // how far it got (.paths, .complete ...), null for None
    int64_t guessed = 0;                                            // the Python sets got.guessed on the exploration
    py::list<std::tuple<Vector, ProbeRecord>> made;                 // every run it made
};
// known: {seed index: trail} of seeds already run.  probe: how the original is run, when not as the judge runs it.
Explored explore_vectors(const py::str& module_path, int64_t seg, int64_t off, bool near_,
                         const std::tuple<int64_t, int64_t>& span, const py::list<Vector>& seeds,
                         const std::optional<py::dict<int64_t, concolic::Trail>>& known = std::nullopt,
                         const std::optional<ProbeOptions>& probe = std::nullopt);

// `probe` (the Python's **probe): how the original is run when not as the judge runs it.  Absent: the judge's own
// (ports=True); a given one is used as it is (its `ports` too), apart from the fields the call sets itself.

// ---- the module ---------------------------------------------------------------------------------------------------
// {address: ((start, end), instruction count)} -- each function runs to the next one in its segment (or the segment's end).
py::dict<int64_t, std::tuple<std::tuple<int64_t, int64_t>, int64_t>> _spans(const formats::ne::NEFile& nef,
                                                                           const py::dict<int64_t, core16::SegOff>& where);

// (digest, settings) of everything other than the source that a verdict depends on.
std::tuple<py::str, py::str> profile(const std::optional<py::str>& module_path = std::nullopt);

// "seg:offset" (%d:%04x)
py::str key(int64_t seg, int64_t off);

// The decompiled module with the three attributes prepare() gives it.
struct Prepared : decompile::c_text::Module16 {
    py::dict<py::str, py::str> key;     // {function name: "seg:off"}
    py::list<py::str> all_keys;         // sorted
    py::str sig;                        // sha1 of the header and the texts in order
    using decompile::c_text::Module16::Module16;
};
Prepared prepare(const py::str& module_path, const py::str& work);
// prepare() from a module already made compilable: its key, all_keys and sig added
Prepared prepared_from(const decompile::c_text::Module16& made);
// what reconstruct::Kind::prepare hands back for a Prepared (the lane's own is in `lane`)
reconstruct::Prepared as_prepared(const std::shared_ptr<Prepared>& m);

// What judge() is handed once for every function: (module_path, dll, data_image, own, order)
struct JudgeContext {
    py::str module_path;
    py::str dll;                                            // the rebuilt module
    py::bytes data_image;                                   // the original's data segment bytes
    py::list<std::tuple<int64_t, int64_t>> own;             // the rebuild's own places in its data segment (_own_data)
    py::dict<int64_t, int64_t> order;                       // rebuilt code segment -> the original's
};
// One function to judge: (key, seg, off, near, wide, span, insns, rebuilt seg, rebuilt off[, frame])
struct JudgeItem {
    py::str key;
    int64_t seg = 0, off = 0;
    bool near_ = false;
    std::optional<bool> wide;                               // None: the function returns nothing
    std::tuple<int64_t, int64_t> span;
    int64_t insns = 0;
    int64_t bseg = 0, boff = 0;                             // where the build has it
    std::optional<Frame> frame;                             // a caller-frame helper's places
};
struct Built {
    JudgeContext context;
    py::dict<py::str, JudgeItem> items;
    py::dict<py::str, py::str> dropped;                     // {name: why it is not in the build}
};
Built build(const Prepared& m, const py::str& module_path, const py::str& work);

// {selector: "seg<n>"} for a build's code segments, n the original's number of the segment each holds: the original's own
// (`order` None; original_segments lists them), or a rebuild's through `order` (its segment -> the original's).
py::dict<int64_t, py::str> _named(const py::list<int64_t>& original_segments,
                                  const std::optional<py::dict<int64_t, int64_t>>& order = std::nullopt);

// {segment: the original's bytes of it} for each code segment some of whose C still reads it through CS at the original's offsets.
py::dict<int64_t, py::bytes> _code_images(const Prepared& m, const formats::ne::NEFile& nef);

// The offsets in the rebuild's data segment of what the build itself keeps there and the original does not.
py::list<std::tuple<int64_t, int64_t>> _own_data(const py::str& work);

// (verdict, reason, limits, detail) -- runs in a worker process.
std::tuple<py::str, py::str, py::list<py::str>, py::str> _judge(const JudgeContext& context, const JudgeItem& item);
inline std::tuple<py::str, py::str, py::list<py::str>, py::str> judge(const JudgeContext& context, const JudgeItem& item) {
    return _judge(context, item);
}

// The lane as a reconstruct::Kind (what the Python module is to reconstruct.py), and its registration under "ne".
// Prepared::lane holds a std::shared_ptr<decompile::c_text::Module16> (the base of a win16::Prepared, static_pointer_cast back); Built::context a std::shared_ptr<JudgeContext>; each item a
// std::shared_ptr<JudgeItem>.
const std::shared_ptr<reconstruct::Kind>& kind();
void register_lane();

}  // namespace codebreaker::verify::lanes::win16
