// cb/emulate/driver.hpp -- run flat 32-bit ring-0 code: a virtual device driver's entry points.
//
//     const Image image = Image::load("vkd.vxd");                       // the driver as the loader leaves it in memory
//     Environment env{.governor = &governor, .resolver = resolver};
//     RunOptions options;  options.registers.eax = 1;  options.stack = {0x1000};
//     Record r = run(env, image, image.entries.at("control"), options);
//     Run run = record(std::make_shared<const Record>(std::move(r)));    // the one run record the judge compares
//
// The flat-code counterpart of emulator::Tracer::trace: it takes memory regions and registers where that takes a module file and stack
// arguments. A call to the system or to another driver (`int 20h`, service, device) is answered with what the input says and recorded with the
// registers and stack words it was given; port reads and writes are recorded; memory is kept by the 16-bit and 32-bit recorders' own hooks
// (cb/emulate/native_driver). What a run did is judged in verify/lanes/driver.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/core16_types.hpp"
#include "cb/emulate/emulator.hpp"
#include "cb/emulate/native_driver.hpp"
#include "cb/emulate/native_shadow.hpp"
#include "cb/emulate/run.hpp"

namespace cb::formats::le {
class Module;
}

namespace cb::emulate::driver {

inline constexpr std::size_t kMaxCalls = 64;             // service calls compared per run
// EAX is the fingerprint's `ret`; these are the other registers a run's answer is made of.
inline constexpr std::array<std::string_view, 6> kResultRegisters = {"ebx", "ecx", "edx", "esi", "edi", "ebp"};
inline constexpr std::int64_t kInstructionCap = 10000;

// The stack and scratch regions of the 32-bit tracer's machine, and the places the inputs point at inside the scratch region.
inline constexpr std::uint32_t kStackLow = emulator::kStackBase, kStackHigh = emulator::kStackBase + emulator::kStackSize;
inline constexpr std::uint32_t kScratch = emulator::kScratchBase;
inline constexpr std::uint32_t kVm = kScratch + 0x1000, kClient = kScratch + 0x2000, kBuffer = kScratch + 0x3000, kObject = kScratch + 0x6000,
                               kSafe = kScratch + 0x7800;
inline constexpr std::uint32_t kReturn = emulator::kReturnAddress;   // the run ends when the entry point returns here

// A driver as the loader leaves it in memory.
struct Region {
    std::uint32_t address = 0;
    Bytes bytes;
};
struct Span {                    // an object's place: [low, high)
    std::uint32_t low = 0, high = 0;
    std::uint32_t object = 0;    // 1-based
};
struct Image {
    std::vector<Region> regions;                       // the objects that have bytes, relocated, in object order
    std::vector<Span> spans;
    OrderedMap<std::string, std::uint32_t> entries;    // everywhere the system or another driver can enter it: "control", "v86_api", "pm_api", "service_N"
    std::vector<std::uint32_t> shape;                  // the sizes of its objects
    std::string digest;                                // SHA-1 of the regions' bytes, lowercase hex

    // The image of an LE module: each object's bytes with the fix-ups that name another place in the image applied (the 32-bit address
    // itself, or its distance from the end of the field, as in a call). FormatError when it has more objects than the address plan holds.
    static Image of(const formats::le::Module& module);
    // The image of a driver file, or of one member of a container (by name, else the first). IoError / FormatError for a file that holds no
    // LE image.
    static Image load(const std::filesystem::path& file, std::optional<std::string_view> member = std::nullopt);

    bool holds(std::uint32_t address) const;
};

// What a run needs of its surroundings.
struct Environment {
    // The instruction ceiling for one function (the caller resets `n` and sets `ceil`); required, and the caller's alone.
    native_trace::Governor* governor = nullptr;
    // Which register a service answers in when it is not EAX (0 eax ... 6 ebp, native_driver's order); empty: every service answers in EAX.
    // Asked once per distinct service in a run. `answers_in_of` makes it from the driver kit's names and the knowledge base's stand-ins.
    native_driver::ServiceResolver resolver;
    // A new 32-bit x86 engine; empty: cb/core's Unicorn.
    std::function<std::unique_ptr<core16::Uc>()> open_engine;
};

// The register a service answers in when it is not EAX, from what the kit documents (the knowledge base's vxd api_model,
// knowledge/models/vxd.json): `name_of` gives a service its kit name, `answers_in` the model's register for a name ("ebx" ...).
native_driver::ServiceResolver resolver_of(std::function<std::optional<std::string>(std::uint32_t device, std::uint32_t service)> name_of,
                                           std::function<std::optional<std::string>(std::string_view service_name)> answers_in);

struct Registers {   // the registers a run's inputs set; absent: as the machine starts
    std::optional<std::uint32_t> eax, ebx, ecx, edx, esi, edi, ebp;
};
struct MemoryInit {
    std::uint32_t address = 0;
    Bytes data;
};
struct RunOptions {
    Registers registers;
    std::vector<std::uint32_t> stack;           // words above the return address
    std::vector<MemoryInit> memory;             // set before the run (pages are mapped as needed)
    std::uint32_t service_eax = 0;              // what every `int 20h` service call, and a call that leaves the image for nowhere, answers in EAX
    bool service_carry = false;                 // ... and the carry flag it leaves
    std::int64_t cap = kInstructionCap;
    std::uint32_t flags = 0;                    // the carry (1) and zero (0x40) flags on entry
    native_shadow::Shadow* shadow = nullptr;    // the path explorer's, stepped before each instruction run and told of each call answered; not owned
};

enum class Ending : std::uint8_t {
    Returned,         // reached the return address
    Truncated,        // the instruction cap, the governor or a recorder cap stopped it
    Hooked,           // the hooks ended it: `observed.exit` says how (a jump to unmapped code, a service number that could not be read, an interrupt)
    Stopped,          // the engine stopped it somewhere that is not the return address (`number` is where)
    Faulted,          // the engine failed: `fault_text`
};

struct Record {
    Ending ending = Ending::Returned;
    std::uint32_t number = 0;                   // Stopped: the address
    std::string fault_text;                     // Faulted: Unicorn's text
    std::uint32_t eax = 0;
    std::uint32_t flags = 0;                    // the carry and zero flags (0x41 of EFLAGS)
    std::array<std::uint32_t, 6> registers{};   // kResultRegisters, in order
    std::uint32_t esp_delta = 0;
    native_driver::DriverRecord observed;       // what the hooks saw: calls, ports, memory, the image's own code that ran, counters

    // "ret", "truncated", the hooks' own words ("wild", "int:21" ...) or "exception: <why>": the words the run record carries.
    std::string exit_text() const;
};

// Runs flat 32-bit code at `entry` of `image` until it returns, and records what it did. A call that leaves the image for nowhere is recorded
// and returned from the same way as a service. An exception the resolver raised comes out of here; a fault of the code is the record's ending.
// UsageError when `env.governor` is missing.
Record run(const Environment& env, const Image& image, std::uint32_t entry, const RunOptions& options = {});

// The record as the one run record (emulate/run.hpp): EAX, the carry and zero flags and the result registers; each service call (detail: its
// device and service, no device for a call out through a pointer) with its registers and stack words; each port access; the addresses of the
// driver's own code that ran, in address order. The Record is the run's `raw`.
Run record(const std::shared_ptr<const Record>& record);

// run() as a Run (the platform's runner).
Run recorded(const Environment& env, const Image& image, std::uint32_t entry, const RunOptions& options = {});

}  // namespace cb::emulate::driver
