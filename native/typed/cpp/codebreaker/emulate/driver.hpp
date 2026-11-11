// codebreaker/emulate/driver.hpp -- typed C++ port of codebreaker.emulate.driver.
//
// driver.py -- run flat 32-bit ring-0 code: a virtual device driver's entry points.
//
//     auto image = driver::load("vkd.vxd");                    // the driver as the loader leaves it in memory
//     auto r = driver::run(image, image.entries.at("control"), {{"eax", 1}}, stack, memory);
//
// The flat-code counterpart of emulator.trace_function: it takes memory regions and registers
// where that takes a module file and stack arguments. A call to the system or to another driver
// (`int 20h`, service, device) is answered with what the input says and recorded with the
// registers and stack words it was given; port reads and writes are recorded; memory is kept by
// the emulator's own hooks. What a run did is judged in verify/lanes/driver.py.
//
// Typing notes:
//   * Unicorn is the typed engine of emulate/unicorn_uc.hpp (core16::Uc): the host loads its DLL
//     (unicorn_uc::load_unicorn) and cbtrace.dll's engine table (native_trace::set_engine), as for the other
//     emulators. UcError is native_trace::UcError; its text is Unicorn's (unicorn_uc::strerror).
//   * The rec dict run() returns is `Record`, one field a key. The keys the DLL's hooks fill are those of
//     native_driver::DriverRecord (a field the hooks left alone keeps the Python's initial value).
//   * Image(lf) takes the typed LE reader's LEFile (the Python's `lf`); load() reads a file, as the Python's.
//   * `registers` is {register name: value}: a name the machine has no register for is a KeyError, as in Python.
//     `stack` is the words above the return address; `memory` is [(address, bytes)].
//   * `shadow` is the native shadow (emulate/native_shadow.hpp) stepped beside the run, or null (None). The Python's
//     inner answer() function is dead code (the hooks are the DLL's) and is not carried.
//   * record() turns a Record into the one run record (emulate/run.hpp). run::Call's detail is a typed variant that
//     has no integer device: the (device, service) of a driver call is (the device's decimal text, the service), the
//     device None when the call went through a pointer; the integers are in Run::raw (the Record, a shared_ptr<Record>).
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "codebreaker/emulate/emulator.hpp"
#include "codebreaker/emulate/native_driver.hpp"
#include "codebreaker/emulate/run.hpp"
#include "codebreaker/formats/le.hpp"
#include "py.hpp"

namespace codebreaker::emulate::native_shadow {
class Shadow;
}

namespace codebreaker::emulate::driver {

inline constexpr int64_t MAX_CALLS = 64;   // service calls compared per run
extern const py::list<py::str> RESULT_REGISTERS;   // EAX is the fingerprint's `ret`
inline constexpr int64_t INSN_CAP = 10000;
extern const py::list<py::str> REGISTERS;

// emulator._STACK_BASE ... (the stack and scratch regions of the 32-bit tracer's machine)
inline constexpr int64_t STACK_LO = emulator::_STACK_BASE, STACK_HI = emulator::_STACK_BASE + emulator::_STACK_SIZE;

// Places the inputs point at, inside the emulator's scratch region.
inline constexpr int64_t SCRATCH = emulator::_SCRATCH_BASE;
inline constexpr int64_t VM = SCRATCH + 0x1000, CLIENT = SCRATCH + 0x2000, BUFFER = SCRATCH + 0x3000, OBJECT = SCRATCH + 0x6000,
                         SAFE = SCRATCH + 0x7800;
inline constexpr int64_t RETURN = emulator::_RET;

// The register a service answers in when it is not EAX, or None: without the answer the code
// after it has nothing to work on. (What the kit documents -- the knowledge base's vxd
// api_model, knowledge/models/vxd.json; everything else returns EAX and carry.)
std::optional<py::str> answers_in(const std::optional<py::str>& service);

// A driver as the loader leaves it in memory: regions [(address, bytes)], spans
// [(address, end, object)], entries {label: address}, shape (sizes of its objects).
class Image {
  public:
    explicit Image(const formats::le::LEFile& lf);

    py::list<std::tuple<int64_t, py::bytes>> regions;
    py::list<std::tuple<int64_t, int64_t, int64_t>> spans;
    py::dict<py::str, int64_t> entries;
    py::list<int64_t> shape;
    py::str digest;

    bool holds(int64_t address) const;
};

// The Image of a driver file (of one member of a container: by name, else the first).
Image load(const py::str& path, const std::optional<py::str>& member = std::nullopt);

// The rec dict of one run: {"exit", "eax", "flags", "registers", "esp_delta", "calls" [(device, service, registers, stack words)],
// "ports" [...], "mem_reads", "mem_writes", "cover" (addresses run inside the image), "truncated", "insns", "outside"
// and what the hooks add: governed, lazy_maps, memops, writes_seen, _stuck}.
struct Record {
    std::optional<py::str> exit;
    std::optional<int64_t> eax;
    py::list<native_driver::Call> calls;
    py::list<native_driver::Port> ports;
    py::list<native_trace::MemRead> mem_reads;
    py::list<native_trace::MemWrite> mem_writes;
    bool truncated = false;
    int64_t insns = 0;
    py::set<uint64_t> cover;
    int64_t outside = 0;
    int64_t flags = 0;                     // the carry and zero flags
    py::list<int64_t> registers;           // RESULT_REGISTERS, in order
    int64_t esp_delta = 0;
    std::optional<bool> governed;
    std::optional<py::list<native_trace::LazyMap>> lazy_maps;
    std::optional<int64_t> memops, writes_seen, _stuck;
};

// Run flat 32-bit code at `entry` of `image` until it returns, and record what it did.
// `registers` {name: value}; `stack` words above the return address; `memory` [(address, bytes)] set before the run;
// `flags` the carry (1) and zero (0x40) flags on entry. Every `int 20h` service call is recorded and
// returns EAX = service_eax, carry = service_carry; a call that leaves the image for
// nowhere is recorded and returned from the same way. `shadow`: a native shadow kept up beside the run (the path
// explorer's): stepped before each instruction run, told of each call answered. The run is the same run with it or without.
Record run(const Image& image, int64_t entry, const std::optional<py::dict<py::str, int64_t>>& registers = std::nullopt,
           const py::list<int64_t>& stack = py::list<int64_t>(),
           const py::list<std::tuple<int64_t, py::bytes>>& memory = py::list<std::tuple<int64_t, py::bytes>>(), int64_t service_eax = 0,
           int64_t service_carry = 0, int64_t cap = INSN_CAP, int64_t flags = 0, native_shadow::Shadow* shadow = nullptr);

// run's record as the one run record (emulate/run.py): EAX, the carry and zero flags and the
// result registers; each service call (detail: (device, service), or (None, address) for a call
// out through a pointer) with its registers and stack words; each port access; the addresses
// of the driver's own code that ran.
run::Run record(const Record& rec);

// The platform's runner (platforms/vxd.py emulator.run): run(...) as a Run.
run::Run recorded(const Image& image, int64_t entry, const std::optional<py::dict<py::str, int64_t>>& registers = std::nullopt,
                  const py::list<int64_t>& stack = py::list<int64_t>(),
                  const py::list<std::tuple<int64_t, py::bytes>>& memory = py::list<std::tuple<int64_t, py::bytes>>(),
                  int64_t service_eax = 0, int64_t service_carry = 0, int64_t cap = INSN_CAP, int64_t flags = 0,
                  native_shadow::Shadow* shadow = nullptr);

}  // namespace codebreaker::emulate::driver
