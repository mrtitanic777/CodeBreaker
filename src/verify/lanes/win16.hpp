// verify/lanes/win16.hpp -- what is particular to reconstructing a 16-bit Windows module.
//
//     Win16Lane lane(spec);                                       // a Kind (kind.hpp), filed under "ne"
//     Win16Judge judge(lane, module, rebuilt_dll, ...);           // one function judged beside the original (judge_function)
//
// The module is decompiled and made compilable (decompile/c_text), every function is compiled and a 16-bit DLL linked (build/functions), and
// each function is run beside the original in the 16-bit emulator (emulate/windows16).
//
// Both sides are entered the way the function is really called (far or near) with the same words on the stack, and the rebuilt module's data
// segment is given the original's data, so a global is at the same offset and starts with the same value on both sides. Compared: how the run
// ends, the Windows functions called and the UART and interrupt controller ports read and written (emulate/ports16), in order and with their
// arguments, the result in AX (DX:AX when the function returns four bytes), what is left in the data segment and on the far heap, how many bytes
// of arguments were removed, and whether SI, DI and DS were preserved.
//
// How many bytes of arguments a Windows function takes is read from the module that exports it (the runner's `argument_bytes`). A call whose size
// could not be established is compared by name only, and a match involving one is marked weak.
//
// Besides the fixed inputs and the directed ones (the constants the code compares with), each function is run on inputs found path by path
// (verify/concolic): the original is run with its symbolic shadow (emulate/native_shadow: the decompiler's own p-code over the arguments, the words
// of the data segment and of the scratch segment, and what an unmodelled Windows call returns), and z3 is asked for inputs that turn each condition
// of its path -- one input kept per new path, and the verdict's limits say how many paths there were (`paths=N`) and whether those are all
// (`paths-all`). Where the lifter is not there (the lane is given none), or with CB_NO_CONCOLIC=1, the inputs are guessed instead (verify/guided).
//
// What runs a function (the runner's environment), what compiles the C and links the rebuild, and what makes the module compilable are given to
// the lane (`Win16LaneSpec`); the platform's own parts live in platforms/win16, build/functions and decompile/c_text.
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "build/functions.hpp"
#include "core/bytes.hpp"
#include "core/ordered_map.hpp"
#include "decompile/c_text.hpp"
#include "emulate/pcode.hpp"
#include "emulate/windows16.hpp"
#include "formats/ne.hpp"
#include "platforms/decoder.hpp"
#include "verify/compare.hpp"
#include "verify/concolic.hpp"
#include "verify/guided.hpp"
#include "verify/lane_common.hpp"
#include "verify/lanes/kind.hpp"

namespace cb::verify::lanes {

inline constexpr std::uint32_t kWin16Leftover = 0x7EF707E5;   // stands for "AX held no result" in a fingerprint
inline constexpr std::int64_t kWin16Retry = 8;                // times the emulator's step limit a rebuild still going is given
// A helper that works in its caller's frame is entered with BP at kWin16CallerBp -- a caller's frame, well below the stack the run itself uses -- and
// its places start out holding kWin16FrameFill on both sides.
inline constexpr std::uint16_t kWin16CallerBp = 0x8000;
inline constexpr std::uint8_t kWin16FrameFill = 0xA5;

// ---- inputs ---------------------------------------------------------------------------------------------------------------------------

// One input to a function: eight words of arguments (compare::Input::args), what an unmodelled Windows call returns (stub_ret), memory set before
// the run, and how the input came to be.
struct Vector16 : compare::Input {
    std::vector<emulate::windows16::MemoryInit> mem;
    bool guided = false;                                            // found by the explorer
    bool noisy = false;                                             // run over a patterned data segment
    std::shared_ptr<const emulate::windows16::RecordedCall> context;   // a call recorded in the running system, replayed (the arguments are then not pushed)

    std::vector<std::uint16_t> words() const;                       // `args` as the words pushed
};

// The inputs every function is run on: eight words of arguments each. Plain numbers alone go nowhere in 16-bit code -- most functions take far
// pointers, and the first use of a pointer whose selector word is 3 or 0x400 is a protection fault, on both sides, before anything has been compared.
// So half the inputs are (offset, selector) pairs naming a real, writable segment, in both orders (which word is the selector depends on where in the
// argument list a pointer sits). The last four are scattered: every word a different one spread over the segment (the same on every run).
std::vector<Vector16> win16_vectors();

// A function's place in its segment: [start, end) as offsets.
struct CodeSpan {
    std::uint32_t start = 0, end = 0;
    friend bool operator==(const CodeSpan&, const CodeSpan&) = default;
};

// Inputs aimed at what the function's own code tests: the constants it compares its values with (alone, and as the offset of a valid far pointer).
// A switch written as a chain of subtractions tests the running total, not each step's number: the totals are the cases. `module` is the module's
// NE image and `segment` the function's (1-based).
std::vector<Vector16> win16_directed(const formats::ne::Module& module, std::uint32_t segment, CodeSpan span, std::size_t most = 8);

// A caller-frame helper's parameters, in their order, from its C: (BP less, bytes) of each; nullopt for a function that is not one.
using Frame16 = std::vector<std::pair<std::uint32_t, std::uint32_t>>;
std::optional<Frame16> win16_frame_slots(std::string_view text, std::string_view name);

// ---- the code of one segment, as the path of a run names its jumps ----------------------------------------------------------------------------

// The conditional jumps of a code segment, decoded the first time an offset is asked about.
class JumpTable {
public:
    JumpTable(Bytes segment_code, std::uint32_t segment);
    // [(site, taken)] -- the function's own conditional jumps in the order a run took them, from the run's instructions (linear addresses): the path it
    // went, as the explorer names one (verify/concolic), for a run made without a shadow.
    concolic::Trail trail(CodeSpan span, std::span<const std::uint64_t> path);

private:
    struct Jump {
        std::uint32_t target = 0, next = 0;
    };
    Bytes code_;
    std::uint32_t segment_;
    platforms::DecoderSet decoders_;
    std::map<std::uint32_t, std::optional<Jump>> known_;   // by offset: lookup only
};

// ---- what a run is asked, and what it did -----------------------------------------------------------------------------------------------------

// A function of a module and how it is entered.
struct Win16Target {
    std::filesystem::path module;
    std::uint32_t segment = 0, offset = 0;
    bool near_entry = false;
    std::optional<bool> wide;                   // returns four bytes (DX:AX); nullopt: nothing
    std::optional<CodeSpan> span;               // with it the print carries the path of the function's own jumps
};

// What the side of a comparison is given besides the target.
struct Win16Side {
    Bytes data_image;                           // written over the start of the data segment (a rebuilt module gets the original's data)
    std::vector<emulate::windows16::OffsetRange> own;   // ranges of the data segment that are the build's own: their writes are left out
    std::optional<Frame16> frame;               // a caller-frame helper's places, whose writes are compared
    std::map<std::uint16_t, std::string> segs;  // the module's code segment selectors as a call's text names them
    std::optional<std::int64_t> steps;          // the instructions a run may take, when not the emulator's own limit
    JumpTable* jumps = nullptr;                 // for the trail (with a span)
};

// A fingerprint, and the path of the function's own jumps the run took (empty without a span).
struct Win16Print {
    compare::Fingerprint fp;
    concolic::Trail trail;
};

// The options of a run of `target` on `v` over `base` (the judge's own: ports on; the gate's: the callees answered, the data's end ...).
emulate::windows16::ProbeOptions win16_run_options(const emulate::windows16::ProbeOptions& base, const Win16Target& target, const Vector16& v,
                                                   const Win16Side& side);
// The judge's own base: devices reachable.
emulate::windows16::ProbeOptions win16_judge_base();

// One Windows call as text (lane_common::call): its name, and its arguments when their size is known. An argument is written as what it IS rather
// than its bit pattern wherever the two builds would legitimately differ: the data or stack selector, an address in the stack, one of the function's
// own inputs (above 0xFF: a small one is as likely a constant), the selector of one of the module's code segments (`segs`: selector -> the original's
// segment it holds -- a build orders its segments its own way), and a far pointer into one of them as what it points at.
std::string win16_call_text(const emulate::ImportHit& hit, std::span<const std::int64_t> inputs, const emulate::windows16::EntryRegisters& entry,
                            const std::map<std::uint16_t, std::string>& segs);

// What one run of one function did, in a form two builds can be compared on. `ran`: the record of this very run, made already (by the path explorer);
// else the run is made in `runner`.
Win16Print win16_fingerprint(emulate::windows16::Runner& runner, const emulate::windows16::ProbeOptions& base, const Win16Target& target,
                             const Vector16& v, const Win16Side& side, const emulate::Run* ran = nullptr);

// ---- the path explorer --------------------------------------------------------------------------------------------------------------------------

struct Win16Explored {
    std::vector<Vector16> inputs;                                        // inputs taking paths of the original no seed took
    std::shared_ptr<concolic::Exploration<Vector16>> exploration;        // how far it got; null where the inputs were guessed
    std::vector<std::pair<Vector16, emulate::Run>> made;                 // every run it made, input and record (a judge running the original as `base`
                                                                         // does need not run those again)
};

// Inputs taking paths of the original no seed took, found path by path, how far the exploration got and every run it made. `known`: {seed index: trail}
// of seeds already run. Falls back to guessing (verify/guided) where there are no lifters.
//
// Where the paths found are not all there are and a jump of the function's own was still seen going one way only, what is left of the guessing
// explorer's budget goes to it, seeded with the runs made: a condition over what the shadow does not follow -- the made-up answer of a callee not run, a
// Windows call modelled from its arguments -- is no question for the solver, and guessing still reaches some of them.
Win16Explored win16_explore(emulate::windows16::Runner& runner, emulate::pcode::LifterSet* lifters, const emulate::windows16::ProbeOptions& base,
                            const Win16Target& target, std::span<const Vector16> seeds, const std::map<std::size_t, concolic::Trail>& known = {});

// Whether the path explorer can be used: the lane has its lifters, and CB_NO_GUIDED / CB_NO_CONCOLIC are not set.
bool win16_concolic_ready(emulate::pcode::LifterSet* lifters);

// ---- the lane ----------------------------------------------------------------------------------------------------------------------------------------

struct Win16LaneSpec {
    // The module decompiled and made compilable, in `work` (decompile/c_text::compilable16 over the program's services).
    std::function<std::shared_ptr<decompile::c_text::Module16>(const std::filesystem::path& module, const std::filesystem::path& work)> compilable;
    // What a 16-bit build gave: the DLL (nullopt: nothing was linked), where each function is, and why each rejected one was.
    struct Rebuilt {
        std::optional<std::filesystem::path> dll;
        std::map<std::string, build::functions::SegmentOffset> where;
        OrderedMap<std::string, std::string> dropped;
    };
    // The functions compiled and linked (build/functions build16): `interrupts`, `code_data` and `code_image` as the module wants them.
    std::function<Rebuilt(const decompile::c_text::Module16& module, const std::filesystem::path& work, const std::map<int, Bytes>& code_images)> build_dll;
    // The compiler's options (part of the profile).
    std::string compiler_options;
    // The runner's surroundings: the models of Windows' functions and their argument sizes. The governor is the caller's, one thread to one.
    std::function<emulate::windows16::Environment(emulate::native_trace::Governor&)> environment;
    // The p-code lifters of the path explorer; none: the inputs are guessed.
    //
    // (The original's runs are kept within one judgement only: a fingerprint of the store carries no path of the function's own jumps, and the explorer
    // needs the paths of the fixed inputs.)
    emulate::pcode::LifterSet* lifters = nullptr;
};

class Win16Lane final : public Kind {
public:
    // Bump when what is built or compared changes. 23: the branch-guided inputs found path by path; a fingerprint carries its path; paths= limits; what
    // the deeper inputs showed of the judge itself. 24: the C++ lane's own digests and guessing.
    static constexpr int kRev = 24;

    explicit Win16Lane(Win16LaneSpec spec);

    const std::string& name() const override;
    Profile profile(const std::filesystem::path& module_path) const override;

    // What prepare() carries to build(): the module made compilable, and the keys of its functions ("seg:off").
    struct PreparedWin16 final : Prepared {
        std::shared_ptr<decompile::c_text::Module16> module;
        void set_text(const std::string& name, std::string text) override {
            module->made.set_text(name, text);        // (the build reads the module's)
            texts[name] = std::move(text);
        }
    };
    std::unique_ptr<Prepared> prepare(const std::filesystem::path& module_path, const std::filesystem::path& work) const override;
    Built build(const Prepared& prepared, const std::filesystem::path& module_path, const std::filesystem::path& work) const override;

    // One function to judge.
    struct Item {
        std::string key;                          // "seg:off" in the original
        std::uint32_t segment = 0, offset = 0;
        bool near_entry = false;
        std::optional<bool> wide;
        CodeSpan span;
        std::int64_t insns = 0;                   // the function's instructions
        std::uint32_t built_segment = 0, built_offset = 0;
        std::optional<Frame16> frame;
    };
    // What judge_function is handed for every function of a build.
    struct Context {
        std::filesystem::path module, dll;
        Bytes data_image;                         // the original's data segment, as the file holds it
        std::vector<emulate::windows16::OffsetRange> own;   // the rebuild's own places in its data segment
        std::map<std::uint32_t, std::uint32_t> order;       // rebuilt code segment -> the original's
    };

    // Judge one function of a build: the original and the rebuilt DLL run on the same inputs, and compared.
    Judgement judge_function(const Context& context, const Item& item) const;

    // The key of a function: "seg:off" (segment in decimal, offset as four hex digits).
    static std::string key(std::uint32_t segment, std::uint32_t offset);
    // {function address: (its span, its instruction count)} -- each function runs to the next one in its segment (or the segment's end).
    static std::map<std::uint64_t, std::pair<CodeSpan, std::int64_t>> spans(const formats::ne::Module& module,
                                                                           const std::map<std::uint64_t, core16::SegOff>& where);
    // {selector: "seg<n>"} for a build's code segments, n the original's number of the segment each holds: the original's own (`order` null;
    // `original_segments` lists them), or a rebuild's through `order` (its segment -> the original's).
    static std::map<std::uint16_t, std::string> named_segments(const std::vector<std::uint32_t>& original_segments,
                                                               const std::map<std::uint32_t, std::uint32_t>* order = nullptr);
    // {segment: the original's bytes of it} for each code segment some of whose C still reads it through CS at the original's offsets -- what c_text
    // could not name, or, with the naming turned off (CB_CODE_DATA=0), every segment whose C reads through CS.
    static std::map<int, Bytes> code_images(const decompile::c_text::Module16& module, const formats::ne::Module& ne);
    // The offsets in the rebuild's data segment of what the build itself keeps there and the original does not: the registers an interrupt helper
    // hands its routine. Writing them is the build's bookkeeping, not the function's effect.
    static std::vector<emulate::windows16::OffsetRange> own_data(const std::filesystem::path& work);

    bool concolic_ready() const;
    const Win16LaneSpec& spec() const { return spec_; }

private:
    struct State;
    std::shared_ptr<State> state_;
    Win16LaneSpec spec_;
};

}  // namespace cb::verify::lanes
