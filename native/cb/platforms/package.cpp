#include "cb/platforms/package.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::platforms {

namespace {

constexpr std::uint16_t kMachine386 = 0x014C;
constexpr std::uint16_t kMachineXenon = 0x01F2;

constexpr std::array<std::string_view, 13> kIdNames = {"java", "xbox360", "xbox", "nes", "sms",   "gb", "genesis",
                                                       "zx",   "msx",     "win32", "win16", "vxd", "dos"};
static_assert(kIdNames.size() == kOrder.size());

std::size_t order_index(Id id) {
    return static_cast<std::size_t>(std::find(kOrder.begin(), kOrder.end(), id) - kOrder.begin());
}

std::vector<std::string> numbered(const char* prefix, int from, int to) {
    std::vector<std::string> out;
    for (int i = from; i < to; ++i) out.push_back(prefix + std::to_string(i));
    return out;
}

// A probe that was not wired is the program's start-up bug, not "no".
template <class Fn>
const Fn& need(const Fn& probe, std::string_view name) {
    if (!probe) throw InternalError(std::format("platform probe '{}' is not set", name));
    return probe;
}

// The first four bytes of an XBE: "XBEH". (Mapped, not read: an XBE is megabytes and four bytes decide.)
bool starts_with_xbeh(const std::filesystem::path& path) {
    const fs::MappedFile f(path);
    const ByteView v = f.view();
    return v.size() >= 4 && as_text(v.first(4)) == "XBEH";
}

}  // namespace

std::string_view id_text(Id id) { return kIdNames[order_index(id)]; }

std::optional<Id> parse_id(std::string_view text) {
    for (std::size_t i = 0; i < kIdNames.size(); ++i)
        if (kIdNames[i] == text) return kOrder[i];
    return std::nullopt;
}

std::string_view source_text(SourceLanguage s) {
    switch (s) {
        case SourceLanguage::C: return "c";
        case SourceLanguage::Asm: return "asm";
        case SourceLanguage::CorAsm: return "c or asm";
        case SourceLanguage::JavaToCxx: return "java -> c++";
    }
    return "";
}

std::string_view engine_text(Engine e) {
    switch (e) {
        case Engine::None: return "";
        case Engine::Unicorn: return "unicorn";
        case Engine::OwnInterpreter: return "CodeBreaker's own interpreter";
        case Engine::Jvm: return "the JVM";
    }
    return "";
}

std::string_view language_text(Language l) {
    switch (l) {
        case Language::C: return "c";
        case Language::Asm: return "asm";
        case Language::Cxx: return "c++";
    }
    return "";
}

// ---- the descriptors -------------------------------------------------------------------------------------------------
Platform machine(const MachineSpec& spec) {
    Platform p;
    p.id = spec.id;
    p.name = std::string(id_text(spec.id));
    p.title = spec.title;
    p.family = spec.family;
    p.source = SourceLanguage::Asm;
    p.lane = p.name;
    p.steps = Steps::Lanes;
    p.cpu = spec.cpu;
    p.loader.container = spec.container;
    p.convention.names = {"registers"};
    p.convention.arguments = InRegisters{};
    p.convention.order = CallOrder::Registers;
    p.convention.results = spec.results;
    p.convention.pops = Pops::Caller;
    p.emulator = {Engine::OwnInterpreter, true};
    p.compiler = {Language::Asm, spec.toolchain};
    return p;
}

Platform xbox360() {
    Platform p;
    p.id = Id::Xbox360;
    p.name = "powerpc";
    p.title = "Xbox 360 (PowerPC)";
    p.family = "PowerPC";
    p.source = SourceLanguage::C;
    p.machine = kMachineXenon;
    p.lane = "powerpc";
    p.steps = Steps::Lanes;
    p.loader = {"XEX2 -> PE", Readable::XexBasefile};
    p.cpu = {"PowerPC:BE:64:Xenon:homed", 64, Endian::Big, OwnDecoder::PowerPc, numbered("r", 0, 32), "r1"};
    p.convention.names = {"__cdecl"};
    p.convention.arguments = 8u;
    p.convention.word = 8;
    p.convention.order = CallOrder::Registers;
    p.convention.results = {"r3", "f1"};
    p.convention.pops = Pops::Caller;
    p.convention.preserved = numbered("r", 14, 32);
    p.emulator = {Engine::OwnInterpreter, true};
    p.compiler = {Language::C, "the Xbox 360 SDK"};
    return p;
}

Platform xbe() {
    // The program goes down the 32-bit Windows lane as the PE image the XBE reader makes of it (the same bytes at the same
    // addresses, the kernel's functions as imports of xboxkrnl.exe), so it has no lane of its own: every other slot is
    // 32-bit Windows' -- the stages are handed the PE image, which identifies as "pe".
    Platform p;
    p.id = Id::Xbox;
    p.name = "xbe";
    p.title = "original Xbox";
    p.family = "x86";
    p.source = SourceLanguage::C;
    p.loader = {"XBE -> PE", Readable::XbeToPe};
    p.cpu = {"x86:LE:32:default:windows", 32, Endian::Little, CapstoneDecoder{Arch::X86, 32}, {}, ""};
    return p;
}

Platform win32() {
    Platform p;
    p.id = Id::Win32;
    p.name = "pe";
    p.title = "32-bit Windows";
    p.family = "x86";
    p.source = SourceLanguage::C;
    p.machine = kMachine386;
    p.lane = "win32";
    p.steps = Steps::Lanes;
    p.loader.container = "PE";
    p.cpu = {"x86:LE:32:default:windows", 32, Endian::Little, CapstoneDecoder{Arch::X86, 32},
             {"EAX", "EBX", "ECX", "EDX", "ESI", "EDI", "EBP", "ESP"}, "ESP"};
    p.convention.names = {"__stdcall", "__cdecl", "__fastcall", "__thiscall"};
    p.convention.arguments = 4u;
    p.convention.word = 4;
    p.convention.order = CallOrder::C;
    p.convention.results = {"EAX", "EDX:EAX"};
    p.convention.pops = Pops::Either;
    p.convention.preserved = {"EBX", "ESI", "EDI", "EBP"};
    p.emulator = {Engine::Unicorn, true};
    p.compiler = {Language::C, "openwatcom"};
    return p;
}

Platform zx() {
    MachineSpec s;
    s.id = Id::Zx;
    s.title = "ZX Spectrum";
    s.family = "z80";
    s.container = "snapshot or tape (.sna, .z80, .tap)";
    s.cpu = {"z80:LE:16:default", 8, Endian::Little, OwnDecoder::Z80,
             {"A", "F", "BC", "DE", "HL", "IX", "IY", "SP"}, "SP"};
    s.results = {"A", "F", "BC", "DE", "HL", "IX", "IY"};
    s.toolchain = "CodeBreaker's own assembler, or sjasmplus / pasmo";
    return machine(s);
}

// ---- the registry --------------------------------------------------------------------------------------------------------
void Registry::add(Platform platform) {
    for (const Platform& p : platforms_)
        if (p.id == platform.id) throw UsageError(std::format("platform '{}' is added twice", id_text(platform.id)));
    const std::size_t at = order_index(platform.id);
    auto where = std::find_if(platforms_.begin(), platforms_.end(), [at](const Platform& p) { return order_index(p.id) > at; });
    platforms_.insert(where, std::move(platform));
}

bool Registry::recognises(const Platform& platform, const std::filesystem::path& path) const {
    const Probes& pr = probes_;
    // Probing: a file that cannot be read, or is damaged, is not this platform.
    try {
        switch (platform.id) {
            case Id::Java: {
                if (text::iends_with(fs::to_text(path), ".jar")) return true;
                if (std::filesystem::is_directory(path) || text::iends_with(fs::to_text(path), ".java"))
                    return need(pr.is_java_module, "is_java_module")(path);
                return false;
            }
            case Id::Xbox360: {
                if (need(pr.is_xex, "is_xex")(path)) return true;
                return need(pr.executable_class, "executable_class")(path) == ExeKind::Pe &&
                       need(pr.pe_machine, "pe_machine")(path) == kMachineXenon;
            }
            case Id::Xbox: return starts_with_xbeh(path);
            case Id::Nes: return need(pr.is_nes, "is_nes")(path);
            case Id::Sms: return need(pr.is_sms, "is_sms")(path);
            case Id::Gb: return need(pr.is_gb, "is_gb")(path);
            case Id::Genesis: return need(pr.is_genesis, "is_genesis")(path);
            case Id::Zx: return need(pr.is_zx, "is_zx")(path);
            case Id::Msx: return need(pr.is_msx, "is_msx")(path);
            case Id::Win32: {
                if (need(pr.executable_class, "executable_class")(path) != ExeKind::Pe) return false;
                const std::optional<std::uint16_t> m = need(pr.pe_machine, "pe_machine")(path);
                return m && *m != kMachineXenon;
            }
            case Id::Win16: return need(pr.executable_class, "executable_class")(path) == ExeKind::Ne;
            case Id::Vxd: return need(pr.holds_drivers, "holds_drivers")(path);
            case Id::Dos: return need(pr.kind, "kind")(path) == ExeKind::Dos;
        }
    } catch (const IoError&) {
        return false;
    } catch (const FormatError&) {
        return false;
    }
    return false;
}

const Platform* Registry::identify(const std::filesystem::path& path) const {
    for (const Platform& p : platforms_)
        if (recognises(p, path)) return &p;
    return nullptr;
}

const Platform* Registry::named(std::string_view name) const {
    for (const Platform& p : platforms_)
        if (p.name == name) return &p;
    for (const Platform& p : platforms_)
        if (id_text(p.id) == name) return &p;
    return nullptr;
}

const Platform& Registry::of(std::string_view kind) const {
    const Platform* p = named(kind);
    if (p == nullptr) throw UsageError(std::format("no platform '{}'", kind));
    return *p;
}

const Platform* Registry::for_machine(std::int64_t machine) const {
    for (const Platform& p : platforms_)
        if (p.machine && static_cast<std::int64_t>(*p.machine) == machine) return &p;
    return nullptr;
}

std::filesystem::path Registry::readable(const Platform& platform, const std::filesystem::path& path) const {
    switch (platform.loader.readable) {
        case Readable::Itself: return path;
        case Readable::XexBasefile: return need(probes_.is_xex, "is_xex")(path) ? need(probes_.xex_basefile, "xex_basefile")(path) : path;
        case Readable::XbeToPe: return starts_with_xbeh(path) ? need(probes_.xbe_to_pe, "xbe_to_pe")(path) : path;
    }
    return path;
}

std::vector<std::pair<std::string, std::string>> Registry::lanes() const {
    std::vector<std::pair<std::string, std::string>> out;
    for (const Platform& p : platforms_)
        if (p.lane) out.emplace_back(p.name, *p.lane);
    return out;
}

std::vector<std::string> Registry::runners() const {
    std::vector<std::string> out;
    for (const Platform& p : platforms_)
        if (p.emulator.runs_code) out.push_back(p.name);
    return out;
}

std::vector<std::pair<Id, Steps>> Registry::step_sets() const {
    std::vector<std::pair<Id, Steps>> out;
    for (const Platform& p : platforms_)
        if (p.steps != Steps::None) out.emplace_back(p.id, p.steps);
    return out;
}

}  // namespace cb::platforms
