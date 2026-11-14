// cb/verify/lanes/driver.hpp -- what is particular to judging a rebuilt virtual device driver.
//
//     codebreaker driver-judge <rebuilt.vxd> <original.vxd>           every entry point, judged
//     codebreaker driver-judge --self-check <original.vxd>            the judge judged: the original against itself, then
//                                                                     against copies with one instruction broken
//
//     DriverLane lane(spec);
//     for (const auto& [label, judgement] : lane.judge_files(rebuilt, original)) { ... }
//
// A driver is flat 32-bit ring-0 code. It is entered at its control procedure (EAX = a system control message, EBX = a virtual
// machine, EBP = the client's registers), at each service it offers other drivers, and at its two API procedures; arguments are in
// registers, sometimes on the stack; the answer is in EAX, the carry and zero flags and whatever registers the procedure documents. It
// calls the system and other drivers with `int 20h` + service + device.
//
// Each entry point is run in the emulator, in the original and in the rebuilt driver, from the same registers, stack and memory, with
// both images laid out the same way and patched as the loader would patch them (every object at an address of its own, every internal
// reference resolved). What a run did is a fingerprint -- how it ended, EAX, the service calls in order with the registers and stack
// words they were given, the port reads and writes, the memory left behind, the flags and registers handed back, how far the stack
// moved -- and the two fingerprints go to the one comparer (verify/compare), which gives the verdict.
//
// A service call returns what the input says (EAX, carry), so both sides see the same system; the few services whose whole point is a
// register (the current virtual machine in EBX) return that. Addresses inside the driver are compared as they are when the two files
// have the same objects, and as "somewhere in the driver" when they do not (then the driver's own memory is compared as the order of
// values stored, and the verdict carries the limit `layout-differs`).
//
// The run itself is emulate/driver. What writes and links the rebuild is the platform's packager, given to the lane as `writer` and
// `build_source`; where the entry points are is the platform's loader (platforms::vxd_entry_points). The path explorer (which finds
// inputs for the paths of the original no fixed input takes) is on when the lane is given the p-code lifters.
//
// A lane is a `Kind` (kind.hpp), filed under "le". Its judge is thread-safe: the images read are kept in the lane, under a lock; each
// run has its own emulator.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/cli.hpp"
#include "cb/emulate/driver.hpp"
#include "cb/emulate/pcode.hpp"
#include "cb/symbols/driver_services.hpp"
#include "cb/verify/compare.hpp"
#include "cb/verify/lane_common.hpp"
#include "cb/verify/lanes/kind.hpp"

namespace cb::verify::lanes {

// The registers a procedure is entered with, in the order the lane lists them.
enum class DriverRegister : std::uint8_t { Eax, Ebx, Ecx, Edx, Esi, Edi, Ebp };

// One input to a procedure. `args` (the comparer's) holds the six general registers then the stack words, as the comparer prints them.
struct DriverVector : compare::Input {
    std::array<std::uint32_t, 7> regs{};                  // by DriverRegister
    std::vector<std::uint32_t> stack;                     // words above the return address
    std::vector<emulate::driver::MemoryInit> memory;      // what the registers point at
    bool stub_carry = false;                              // the carry a service call leaves
    std::uint32_t flags = 1;                              // the carry (1) and zero (0x40) flags on entry

    std::uint32_t& reg(DriverRegister r) { return regs[static_cast<std::size_t>(r)]; }
    std::uint32_t reg(DriverRegister r) const { return regs[static_cast<std::size_t>(r)]; }
};

// One input as `driver_vector` is asked for it: the registers, stack words and what the service calls answer. The carry flag is SET on
// entry unless said otherwise: a procedure reports success by clearing it, and one that forgot to would pass unnoticed if it came in clear.
struct DriverVectorSpec {
    std::uint32_t eax = 0, ebx = emulate::driver::kVm, ecx = 0, edx = 0, esi = emulate::driver::kBuffer, edi = emulate::driver::kObject;
    std::vector<std::uint32_t> stack;
    std::optional<std::uint32_t> fill;                    // what the objects the registers point at are filled with (default: a plain `ret`'s address)
    std::uint32_t ret = 0;                                // what a service call answers in EAX
    bool carry = false;                                   // the carry it leaves
    std::uint32_t flags = 1;
};
DriverVector driver_vector(const DriverVectorSpec& spec = {});

// The objects register arguments point at: a virtual machine block, the client's registers, a buffer, and a table of pointers that each
// lead to a plain `ret`.
std::vector<emulate::driver::MemoryInit> driver_memory(std::optional<std::uint32_t> fill = std::nullopt);

// The inputs of an entry point: the first ones, and further ones run only when the first all agree.
struct DriverInputs {
    std::vector<DriverVector> first, more;
};

// What a fingerprint says beyond compare::Fingerprint, for the sentence that explains a difference (the comparer does not carry it).
struct DriverDetail {
    std::optional<std::pair<std::size_t, std::string>> memory;   // (bytes changed, digest); nullopt: too many to record
    std::vector<std::string> result;                              // the flags, then the result registers as the comparer sees them; empty: did not return
};

// A break made by break_instruction: where in the file, and the instruction.
struct DriverBreak {
    std::uint64_t offset = 0;
    std::string instruction;
};

// What self_check found.
struct DriverSelfCheck {
    std::vector<std::pair<Verdict, std::size_t>> self;            // verdict -> count, in the order first met
    std::size_t broken = 0, caught = 0;
    struct Missed {
        std::string label, instruction;
        Verdict verdict = Verdict::Inconclusive;
    };
    std::vector<Missed> missed;
};

struct DriverLaneSpec {
    // The driver written back as assembly source and built (the platform's packager); Written::built is the file.
    Writer writer;
    // A driver's assembly source folder built against the original (the driver builder); empty: a source folder is never judged.
    struct Rebuilt {
        bool linked = false;
        std::string log;
        std::filesystem::path image;
    };
    std::function<Rebuilt(const std::filesystem::path& source, const std::filesystem::path& original, const std::filesystem::path& out)> build_source;
    // The assembler and linker, by name: their size and date are part of the profile. Empty: none.
    std::function<std::map<std::string, std::filesystem::path>()> tools;
    // What the builder says of the classes of segments (part of the profile), as text.
    std::string segment_classes;
    // The names of services and messages; required.
    const symbols::driver_services::Table* services = nullptr;
    // Which register a service answers in when it is not EAX; empty: every service answers in EAX.
    emulate::native_driver::ServiceResolver resolver;
    // The p-code lifters of the path explorer; none: only the fixed inputs.
    emulate::pcode::LifterSet* lifters = nullptr;
    // Fingerprints kept across runs; unset: none are.
    std::optional<lane_common::FingerprintStore> store;
    // Where the assembly source of a corpus driver is when it is not under CB_DRIVER_SOURCES: the vmm32 folder of the source tree.
    std::optional<std::filesystem::path> source_tree;
    // Where the judge's own files go (copies with an instruction broken): <state>/driver_judge/<driver>.
    std::filesystem::path state;
    // A corpus name or a file as the driver's file; nullopt when there is none.
    std::function<std::optional<std::filesystem::path>(std::string_view)> retail;
};

class DriverLane final : public Kind {
public:
    // What a verdict means; raise when run() or the inputs change. 4: inputs found path by path beyond the fixed ones. 5: the lane's own
    // digest of what a run left in memory.
    static constexpr int kRev = 5;

    explicit DriverLane(DriverLaneSpec spec);

    const std::string& name() const override;
    Profile profile(const std::filesystem::path& module_path) const override;

    // What prepare() carries to build().
    struct PreparedDriver final : Prepared {
        std::filesystem::path source;                     // the assembly folder of CB_DRIVER_SOURCES, or empty
        std::optional<Written> written;                   // the driver written from the image
    };
    // The driver's source: the assembly file in the source tree when CB_DRIVER_SOURCES names one to judge, otherwise written from the
    // image and built. The units judged are the entry points; their text is the one assembly file (in the header).
    std::unique_ptr<Prepared> prepare(const std::filesystem::path& module_path, const std::filesystem::path& work) const override;
    Built build(const Prepared& prepared, const std::filesystem::path& module_path, const std::filesystem::path& work) const override;

    // ---- the pieces, for the tests and the command -------------------------------------------------------------------------

    // The inputs of an entry point ("control", "service_N", "pm_api" ...): a control procedure gets every system control message; a
    // service or API procedure registers and stack words of each sort -- zeros, small numbers, pointers to plain memory, pointers to
    // objects, boundary values -- with service calls answering 0, 1 and failure.
    DriverInputs inputs(std::string_view label) const;

    // The driver at `path`, read once.
    std::shared_ptr<const emulate::driver::Image> image(const std::filesystem::path& path) const;
    // A file replaced is read again.
    void forget_image(const std::filesystem::path& path) const;

    // One run as the comparer wants it. `aligned`: the other image has the same objects, so addresses inside the driver mean the same on
    // both sides.
    compare::Fingerprint fingerprint(const emulate::driver::Image& image, std::uint32_t entry, const DriverVector& v, bool aligned = true) const;
    // The same run, with what explain() reads (made again, not from the cache).
    std::pair<compare::Fingerprint, DriverDetail> run_detailed(const emulate::driver::Image& image, std::uint32_t entry, const DriverVector& v,
                                                               bool aligned) const;
    // One line on what differs between two fingerprints the comparer found different (their details, as run_detailed gives them).
    static std::string explain(const compare::Fingerprint& a, const DriverDetail& da, const compare::Fingerprint& b, const DriverDetail& db);

    // Inputs taking paths of the original no seed took, found path by path, and how far the exploration got. Empty when the lane has no lifters.
    std::pair<std::vector<DriverVector>, std::shared_ptr<concolic::Exploration<DriverVector>>> explore_vectors(
        const emulate::driver::Image& image, std::uint32_t entry, std::span<DriverVector> seeds) const;
    bool concolic_ready() const;

    // Judge one entry point of `original` in `rebuilt`.
    Judgement judge_entry(const std::filesystem::path& original, const std::filesystem::path& rebuilt, std::string_view label) const;
    // Judge every entry point of `original` (or those in `labels`) in `rebuilt`, in entry order.
    std::vector<std::pair<std::string, Judgement>> judge_files(const std::filesystem::path& rebuilt, const std::filesystem::path& original,
                                                               const std::vector<std::string>& labels = {}) const;

    // Where the assembly source of a corpus driver is: <sources>/<name>/, with <sources> from CB_DRIVER_SOURCES, else the lane's source
    // tree. Empty when there is none.
    std::filesystem::path source_folder(const std::filesystem::path& module_path) const;

    // ---- the judge judged ----------------------------------------------------------------------------------------------------

    // Write a copy of `original` with one instruction of the procedure at `label` broken: the nth instruction (in address order) that
    // runs on the first inputs, with one bit of its last byte flipped. nullopt when there is no such one, or the file is a member of a
    // container.
    std::optional<DriverBreak> break_instruction(const std::filesystem::path& original, const std::filesystem::path& out, std::string_view label,
                                                 int nth = 0) const;
    // The original against itself must come out with nothing divergent; then, for up to `entries` entry points that came out equivalent,
    // `per_entry` copies each with one executed instruction broken are judged -- a judge with teeth reports them divergent.
    DriverSelfCheck self_check(const std::filesystem::path& original, const std::optional<std::filesystem::path>& work = std::nullopt,
                               int per_entry = 3, int entries = 12) const;

    // `driver-judge <rebuilt> <original> | --self-check <original>`.
    cli::Command command() const;

private:
    struct State;
    std::shared_ptr<State> state_;
    DriverLaneSpec spec_;
};

// The key a driver's entry point is filed under: "<object>:<offset in hex>".
std::string driver_key(std::uint32_t object, std::int64_t offset);
// The order the entry points are judged in: control, then the services, then the rest; shorter labels first.
bool driver_entry_before(std::string_view a, std::string_view b);

}  // namespace cb::verify::lanes
