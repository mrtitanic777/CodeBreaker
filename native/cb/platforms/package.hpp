// cb/platforms/package.hpp -- one description per platform, which every stage asks.
//
//     platforms::Registry registry(probes);                       // probes: what recognising a file asks of the formats
//     registry.add(platforms::win32());  registry.add(platforms::dos()); ...
//     const Platform* p = registry.identify(path);                // the Platform a file belongs to, or nullptr
//     p->name, p->title, p->cpu.language, p->source              // "pe", "32-bit Windows", "x86:LE:32:default:windows", C
//     registry.named("powerpc");  registry.for_machine(0x1F2);    // by the name results are filed under; by PE machine
//
// A platform is what a program was made for: the processor its code is for (the decompiler's language for it), the
// container the code comes in, the language its functions come back as, the lane that reconstructs and judges it, and
// what runs its code beside the original's. A platform is five records -- loader, cpu, convention, emulator, compiler
// -- and a new platform adds a descriptor, not branches in every stage (docs/PLATFORM_DESCRIPTOR.md). `name` is what
// results are filed under in the function store, so it keeps the name each lane has always had ("pe" is 32-bit
// Windows); `id` says which descriptor it is (win16, win32, vxd ...), which the engine's steps are filed under.
// Platforms are tried in kOrder: the first that recognises a file is its platform.
//
// A descriptor is DATA. The functions that do a platform's work -- open its container, run its code, compile and link
// it, write its source -- belong to the layers that own them (the formats, the emulators, the build and verify lanes),
// which key their own tables on `Id`; this module names no function of another area, so describing a platform costs
// nothing and depends on nothing. What the platform layer does itself is recognise a file, which asks the formats
// layer through `Probes`.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

#include "cb/platforms/decoder.hpp"

namespace cb::platforms {

// ---- the descriptors' vocabulary --------------------------------------------------------------------------------
enum class Id { Java, Xbox360, Xbox, Nes, Sms, Gb, Genesis, Zx, Msx, Win32, Win16, Vxd, Dos };

// The order platforms are tried in.
inline constexpr std::array<Id, 13> kOrder = {Id::Java, Id::Xbox360, Id::Xbox, Id::Nes,   Id::Sms,   Id::Gb, Id::Genesis,
                                              Id::Zx,   Id::Msx,     Id::Win32, Id::Win16, Id::Vxd, Id::Dos};

// The descriptor's name ("win32", "xbox360", ...), and back.
std::string_view id_text(Id id);
std::optional<Id> parse_id(std::string_view text);

enum class Endian { Little, Big };
// What the functions come back as.
enum class SourceLanguage { C, Asm, CorAsm, JavaToCxx };
std::string_view source_text(SourceLanguage s);   // "c", "asm", "c or asm", "java -> c++"

// Own decoders (CodeBreaker's, not Capstone's): the decompiler's module for the processor.
enum class OwnDecoder { Z80, Sm83, M6502, M68k, Jar, PowerPc };
struct CapstoneDecoder {
    Arch arch = Arch::X86;
    unsigned bits = 16;
};
// What disassembles it outside the decompiler: nothing declared, Capstone, or CodeBreaker's own.
using DecoderSpec = std::variant<std::monostate, CapstoneDecoder, OwnDecoder>;

// The processor.
struct Cpu {
    std::string language;                  // the SLEIGH language id the decompiler reads the code as; empty: none
    std::optional<unsigned> bits;          // the word the code works in (8, 16, 32, 64)
    std::optional<Endian> endian;
    DecoderSpec decoder;
    std::vector<std::string> registers;    // the registers a run starts from and is compared on
    std::string stack;                     // the stack pointer's name
};

enum class CallOrder { Pascal, C, Registers };   // first pushed first | first pushed last | in registers
enum class Pops { Callee, Caller, Either };      // who removes the arguments ("either": the function says)
struct InRegisters {};                           // the arguments are in registers
// How many arguments (words) the judge gives a function whose count is not known; or "in registers".
using ArgumentCount = std::variant<std::monostate, unsigned, InRegisters>;

// How a function is called, as the judge enters it and the decompiler writes it.
struct Convention {
    std::vector<std::string> names;        // the conventions the decompiler writes its prototypes with
    ArgumentCount arguments;
    std::optional<unsigned> word;          // bytes per argument word
    std::optional<CallOrder> order;
    std::vector<std::string> results;      // where the result is: one register, or the pair a wide one is in ("DX:AX")
    std::optional<Pops> pops;
    std::vector<std::string> preserved;    // the registers a function must hand back as it found them
};

// The file the stages read: the file itself, or the image inside its container.
enum class Readable { Itself, XexBasefile, XbeToPe };

struct Loader {
    std::string container;                 // what the code comes in ("NE", "XEX2 -> PE", "MZ, COM, SYS" ...)
    Readable readable = Readable::Itself;
};

enum class Engine { None, Unicorn, OwnInterpreter, Jvm };
std::string_view engine_text(Engine e);

struct Emulator {
    Engine engine = Engine::None;
    bool runs_code = false;                // CodeBreaker can run this platform's code (the judge has a runner)
};

enum class Language { C, Asm, Cxx };
std::string_view language_text(Language l);

struct Compiler {
    std::optional<Language> language;      // what the source is
    std::string toolchain;                 // whose tools ("openwatcom", "the Xbox 360 SDK", "cc65" ...)
};

// The engine's steps a platform's stages are filed under.
enum class Steps { None, Lanes, Win16 };

struct Platform {
    Id id = Id::Dos;
    std::string name;                      // what results are filed under: "pe", "ne", "le", "powerpc", "xbe", "dos" ...
    std::string title;                     // "32-bit Windows"
    std::string family;                    // "x86", "z80", "PowerPC", "bytecode" ...
    std::optional<SourceLanguage> source;
    std::optional<std::string> lane;       // the verify lane that reconstructs and judges it ("win32", "driver" ...)
    std::optional<std::uint16_t> machine;  // the PE machine type, for the platforms a PE image can be of
    Steps steps = Steps::None;             // (a platform with a lane is on the engine: Lanes, unless it says otherwise)
    Cpu cpu;
    Loader loader;
    Convention convention;
    Emulator emulator;
    Compiler compiler;
};

// A machine whose programs were written in assembly and come back as assembly (the console and home-computer lanes):
// its processor, its registers in and out, CodeBreaker's own interpreter, the assembler that rebuilds it.
struct MachineSpec {
    Id id = Id::Nes;                       // the lane, the name and the id are all this machine's
    std::string title;
    std::string family;
    std::string container;
    Cpu cpu;
    std::vector<std::string> results;      // the registers a run is compared on
    std::string toolchain = "CodeBreaker's own assembler";
};
Platform machine(const MachineSpec& spec);

// The descriptors this module holds (the others are in their own files: dos, msx, java, win16, vxd, nes, sms, gb, genesis).
Platform xbox360();   // PowerPC code (Xenon) in a big-endian PE image inside a XEX2 container
Platform xbe();       // the original Xbox: x86 code in an XBE (it goes down the 32-bit Windows lane as a PE image)
Platform win32();     // PE images for the 386 and later (machine type 0x14C)
Platform zx();        // the ZX Spectrum: Z80 programs in snapshots and on tapes

// ---- recognising a file ------------------------------------------------------------------------------------------
// The class of an executable by its header.
enum class ExeKind { Pe, Ne, Le, Dos };

// What recognising a file asks of the formats layer. Each answers "no" (false / nullopt) for a file that is not that
// format, unreadable or damaged; a probe that throws an Error of the IoError or FormatError family is taken as "no"
// too. A probe that is not set, asked, is an InternalError: the program's start-up forgot to wire it.
struct Probes {
    using Path = std::filesystem::path;
    std::function<std::optional<ExeKind>(const Path&)> executable_class;   // by the MZ header (cached by its owner)
    std::function<std::optional<ExeKind>(const Path&)> kind;                // that, and DOS for a headerless .COM or driver
    std::function<std::optional<std::uint16_t>(const Path&)> pe_machine;    // a PE image's machine type
    std::function<bool(const Path&)> holds_drivers;                         // an LE/LX image, or a W3 bundle (vmm32.vxd)
    std::function<bool(const Path&)> is_xex, is_nes, is_sms, is_gb, is_genesis, is_msx, is_zx;
    std::function<bool(const Path&)> is_java_module;                        // a folder or .java file the Java lane takes
    std::function<Path(const Path&)> xex_basefile;                          // the image a XEX2 container holds
    std::function<Path(const Path&)> xbe_to_pe;                            // the PE image an XBE becomes
};

// ---- the registry --------------------------------------------------------------------------------------------------
class Registry {
public:
    explicit Registry(Probes probes = {}) : probes_(std::move(probes)) {}

    // Add a descriptor. Throws UsageError for an id added twice.
    void add(Platform platform);
    bool complete() const { return platforms_.size() == kOrder.size(); }

    // Every platform added, in kOrder.
    std::span<const Platform> all() const { return platforms_; }

    bool recognises(const Platform& platform, const std::filesystem::path& path) const;
    // The platform `path` belongs to, or nullptr when no platform recognises it.
    const Platform* identify(const std::filesystem::path& path) const;
    // The platform results are filed under `name`, else the descriptor called `name` ("win32" is "pe"), or nullptr.
    const Platform* named(std::string_view name) const;
    // named(), which must exist (UsageError).
    const Platform& of(std::string_view kind) const;
    // The platform of a PE image with this machine type, or nullptr.
    const Platform* for_machine(std::int64_t machine) const;
    // The file the stages read: the file itself, or the image inside its container.
    std::filesystem::path readable(const Platform& platform, const std::filesystem::path& path) const;

    // {name, lane} of every platform that has a lane, in kOrder.
    std::vector<std::pair<std::string, std::string>> lanes() const;
    // The names of the platforms whose code CodeBreaker can run, in kOrder.
    std::vector<std::string> runners() const;
    // {id, steps} of every platform on the engine, in kOrder.
    std::vector<std::pair<Id, Steps>> step_sets() const;

private:
    Probes probes_;
    std::vector<Platform> platforms_;   // in kOrder
};

}  // namespace cb::platforms
