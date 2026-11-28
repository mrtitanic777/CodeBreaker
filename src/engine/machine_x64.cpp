// cb-unity: alone
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "engine/machine_x64.hpp"

#include <algorithm>
#include <format>
#include <optional>
#include <map>
#include <memory>
#include <mutex>
#include <set>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/unicorn_api.hpp"
#include "emulate/unicorn_uc.hpp"
#include "engine/lane_registry.hpp"
#include "engine/machine_lanes.hpp"
#include "emulate/cpu/pcode_cpu_x86.hpp"
#include "formats/read.hpp"
#include "platforms/ps4.hpp"
#include "platforms/ps5.hpp"
#include "platforms/xboxone.hpp"
#include "formats/orbis.hpp"
#include "formats/xboxone.hpp"
#include "source/orbis_writer.hpp"
#include "source/xboxone_writer.hpp"

#pragma warning(push, 0)
#include <unicorn/x86.h>
#pragma warning(pop)

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_ps4(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (const auto kind = orbis::kind_of(head); kind != orbis::Kind::None && size >= 64) {   // the PlayStation 4 and 5: an x86-64 ELF, OpenOrbis's fake SELF, a PS5 SELF or a package
        if (kind == orbis::Kind::Ps5Self) return one("PlayStation 5 SELF", "not handled: signed and encrypted, CodeBreaker does not decrypt", "ps5", "gap");
        if (kind == orbis::Kind::Pkg) return one("PlayStation 4 package (.pkg)", "not handled: recognised only", "ps4", "gap");
        if (size <= (64u << 20)) {
            try {
                const orbis::Program program = orbis::parse(file);
                if (kind == orbis::Kind::Fself || orbis::is_playstation(program)) {
                    const bool ps5 = orbis::console_of(std::filesystem::path(std::string(name)), program) == orbis::Console::Ps5;
                    return one(std::format("PlayStation {} {}", ps5 ? 5 : 4, kind == orbis::Kind::Fself ? "SELF (fake-signed)" : program.orbis ? "executable (Orbis ELF)" : "executable (ELF)"),
                               std::format("{}, {} bytes, {} imports", ps5 ? "AMD Zen 2 x86-64" : "AMD Jaguar x86-64", size, program.imports.size()), ps5 ? "ps5" : "ps4", "reach");
                }
            } catch (const FormatError&) {
                if (kind == orbis::Kind::Fself) return one("PlayStation 4 SELF (encrypted)", "not handled: signed or encrypted, CodeBreaker does not decrypt", "ps4", "gap");
            }
        }
    }
    return std::nullopt;
}
}  // namespace
}  // namespace cb::formats::detect

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_xboxone_1(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (starts_with(head, 0, "PK") && name_ends_with(name, {".appx", ".msix", ".appxbundle", ".msixbundle"})) {   // an Xbox One / Series dev-mode package: a zip with an x64 PE32+ in it
        try {
            const xboxone::Program program = xboxone::parse(file, std::string(name));
            return one(std::format("Xbox One / Series {}", program.container), std::format("{} {}, {}, {} bytes", program.identity, program.version, program.member, size), "xboxone", "reach");
        } catch (const Error&) {
        }
    }
    return std::nullopt;
}
}  // namespace
}  // namespace cb::formats::detect

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_xboxone_2(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (name_ends_with(name, {".xvc", ".msixvc", ".eappx", ".eappxbundle"}))   // a retail package: encrypted with the console's keys
        return one("Xbox One / Series package (encrypted)", "not handled: CodeBreaker does not decrypt", "xboxone", "gap");
    return std::nullopt;
}
}  // namespace
}  // namespace cb::formats::detect

namespace cb::engine {

namespace stdfs = std::filesystem;
namespace vl = verify::lanes;
namespace fo = formats::orbis;

namespace x64_lane_detail {

constexpr std::uint64_t kSteps = 200000;       // instructions a function may run in a comparison
constexpr std::size_t kShown = 4000;           // changes and calls kept of a run
constexpr std::size_t kCacheSize = 6;
constexpr std::uint64_t kStubs = 0x70000000;   // the imported functions' stubs, 16 bytes each
constexpr std::uint64_t kThread = 0x71000000;  // the thread block (fs or gs points into it)
constexpr std::uint64_t kThreadBytes = 0x10000;
constexpr std::uint64_t kWork = 0x72000000;    // free memory a run may be handed pointers into, seeded from `fill`
constexpr std::uint64_t kWorkBytes = 0x10000;
constexpr std::uint64_t kStackTop = 0x7F000000;
constexpr std::uint64_t kStackBytes = 0x100000;
constexpr std::uint64_t kStackWatched = 0x4000;   // the top of the stack compared after a run
constexpr std::uint64_t kReturn = 0x6F000000;     // where a function returns to: nothing is mapped there
constexpr std::uint64_t kLargest = 512ull << 20;  // an image larger than this is not run

std::uint8_t fill_byte(std::uint64_t i, std::int64_t seed) {
    return static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(seed) * 13) ^ (i >> 3)) & (seed % 2 != 0 ? 0x0F : 0xFF));
}

std::int64_t at(const vl::State& s, const char* key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

vl::State make_state(std::uint64_t a0, std::uint64_t a1, std::uint64_t a2, std::uint64_t a3, std::uint64_t a4, std::uint64_t a5, int fill) {
    return {{"a0", static_cast<std::int64_t>(a0)}, {"a1", static_cast<std::int64_t>(a1)}, {"a2", static_cast<std::int64_t>(a2)},
            {"a3", static_cast<std::int64_t>(a3)}, {"a4", static_cast<std::int64_t>(a4)}, {"a5", static_cast<std::int64_t>(a5)}, {"fill", fill}};
}

// What a loader leaves for the engine: the memory as the system's loader would have it.
struct Loaded {
    std::uint64_t base = 0;                                         // added to every address a function's key names
    std::uint64_t low = 0, high = 0;                                // the span to map (whole pages)
    struct Part {
        std::uint64_t address = 0;
        Bytes bytes;
    };
    std::vector<Part> parts;                                        // what is written, absolute
    std::vector<std::pair<std::uint64_t, std::string>> imports;     // slot address (absolute) and the function it will hold the stub of
    std::vector<std::pair<std::uint64_t, std::uint64_t>> patches;   // an address and the 64-bit value the loader writes there
    std::vector<std::pair<std::uint64_t, std::uint64_t>> writable;  // ranges compared for what a run changed
    bool ms_abi = false;                                            // arguments in RCX RDX R8 R9 (else RDI RSI RDX RCX R8 R9)
    std::set<std::string> ending;                                   // imports that end the process
    std::function<std::string(std::uint64_t)> syscall_name;
};

std::uint64_t page_down(std::uint64_t v) { return v & ~0xFFFull; }
std::uint64_t page_up(std::uint64_t v) { return (v + 0xFFF) & ~0xFFFull; }

std::shared_ptr<const Loaded> load_orbis(const stdfs::path& path) {
    const fo::Program p = fo::read(path);
    auto loaded = std::make_shared<Loaded>();
    const bool shared = p.type == 3 || p.type == 0xFE10 || p.type == 0xFE18;
    loaded->base = shared ? 0x10000000 : 0;
    std::uint64_t low = UINT64_MAX, high = 0;
    for (const fo::Segment& s : p.segments) {
        if (!s.loaded() || s.memory_size == 0) continue;
        low = std::min(low, s.address);
        high = std::max(high, s.address + s.memory_size);
    }
    if (low == UINT64_MAX || high - low > kLargest) throw FormatError("orbis lane", 0, "the program has no loaded segment, or one too large to run");
    loaded->low = page_down(low + loaded->base);
    loaded->high = page_up(high + loaded->base);
    for (const fo::Segment& s : p.segments) {
        if (!s.loaded() || s.file_size == 0 || s.offset >= p.file.size()) continue;
        const std::uint64_t n = std::min<std::uint64_t>(s.file_size, p.file.size() - s.offset);
        loaded->parts.push_back({s.address + loaded->base, Bytes(p.file.begin() + static_cast<std::ptrdiff_t>(s.offset), p.file.begin() + static_cast<std::ptrdiff_t>(s.offset + n))});
        if ((s.flags & 2) != 0) loaded->writable.emplace_back(s.address + loaded->base, std::min<std::uint64_t>(s.memory_size, 0x40000));
    }
    std::map<std::uint64_t, std::string> slots;
    for (const fo::Import& im : p.imports) slots[im.slot] = im.name.empty() ? std::format("{}_{}", im.library.empty() ? "nid" : im.library, im.nid) : im.name;
    for (const auto& [slot, name] : slots) loaded->imports.emplace_back(slot + loaded->base, name);
    for (const fo::Relocation& r : p.relocations) {
        if (r.type == 8) {   // R_X86_64_RELATIVE
            loaded->patches.emplace_back(r.offset + loaded->base, static_cast<std::uint64_t>(r.addend) + loaded->base);
        } else if ((r.type == 1 || r.type == 6 || r.type == 7) && r.symbol < p.symbols.size() && p.symbols[r.symbol].defined()) {   // a symbol the program defines
            loaded->patches.emplace_back(r.offset + loaded->base, p.symbols[r.symbol].value + static_cast<std::uint64_t>(r.type == 1 ? r.addend : 0) + loaded->base);
        }
    }
    loaded->ending = {"exit", "_exit", "abort", "sceKernelExitProcess", "sceKernelExitThread", "scePthreadExit", "pthread_exit", "__stack_chk_fail"};
    loaded->syscall_name = [](std::uint64_t n) { return fo::syscall_name(static_cast<std::uint32_t>(n)); };
    return loaded;
}


// A PE32+ at its preferred image base, as the system's loader leaves it before relocations (none are needed there): the headers and each section's bytes at
// their RVAs, every import address-table slot to be filled with a stub's address, the dynamic-link names kept as `dll!function`.
std::shared_ptr<const Loaded> load_xboxone(const stdfs::path& path) {
    const formats::xboxone::Program p = formats::xboxone::read(path);
    const formats::pe::Image img = formats::xboxone::image_of(p);
    const formats::pe::OptionalHeader opt = img.optional_header();
    auto loaded = std::make_shared<Loaded>();
    loaded->base = opt.image_base;
    std::uint64_t high = opt.headers_size;
    for (const formats::pe::Section& s : img.sections()) high = std::max<std::uint64_t>(high, static_cast<std::uint64_t>(s.virtual_address) + std::max(s.virtual_size, s.raw_size));
    high = std::max<std::uint64_t>(high, opt.image_size);
    if (high > kLargest) throw FormatError("xboxone lane", 0, "the image is too large to run");
    loaded->low = page_down(loaded->base);
    loaded->high = page_up(loaded->base + high);
    const ByteView file = img.data();
    loaded->parts.push_back({loaded->base, Bytes(file.begin(), file.begin() + static_cast<std::ptrdiff_t>(std::min<std::uint64_t>(opt.headers_size, file.size())))});
    for (const formats::pe::Section& s : img.sections()) {
        const std::uint64_t n = std::min<std::uint64_t>({s.raw_size, s.virtual_size == 0 ? s.raw_size : s.virtual_size, file.size() > s.raw_ptr ? file.size() - s.raw_ptr : 0});
        if (n != 0) loaded->parts.push_back({loaded->base + s.virtual_address, Bytes(file.begin() + s.raw_ptr, file.begin() + s.raw_ptr + static_cast<std::ptrdiff_t>(n))});
        if ((s.characteristics & 0x80000000u) != 0) loaded->writable.emplace_back(loaded->base + s.virtual_address, std::min<std::uint64_t>(std::max(s.virtual_size, s.raw_size), 0x40000));
    }
    try {
        for (const formats::pe::Import& im : img.imports()) loaded->imports.emplace_back(im.slot_va, im.dll + "!" + im.name);
    } catch (const Error&) {   // (a damaged import table: the functions that call nothing still run)
    }
    loaded->ms_abi = true;
    loaded->ending = {"ExitProcess", "TerminateProcess", "ExitThread", "RaiseFailFastException", "abort", "exit", "_exit", "quick_exit", "FatalExit"};
    return loaded;
}

using Loader = std::function<std::shared_ptr<const Loaded>(const stdfs::path&)>;

std::shared_ptr<const Loaded> cached(const std::string& kind, const Loader& loader, const stdfs::path& path) {
    static std::mutex lock;
    static std::map<std::string, std::shared_ptr<const Loaded>> cache;
    std::error_code ec;
    const auto size = stdfs::file_size(path, ec);
    const auto time = stdfs::last_write_time(path, ec).time_since_epoch().count();
    const std::string key = std::format("{}|{}|{}|{}", kind, path.generic_string(), static_cast<std::uint64_t>(size), static_cast<std::int64_t>(time));
    {
        const std::lock_guard guard(lock);
        if (const auto it = cache.find(key); it != cache.end()) return it->second;
    }
    std::shared_ptr<const Loaded> loaded = loader(path);
    const std::lock_guard guard(lock);
    if (cache.size() >= kCacheSize) cache.erase(cache.begin());
    cache[key] = loaded;
    return loaded;
}

int reg(const char* name) { return *emulate::unicorn_uc::x86_register(name); }

vl::Effects effects(const std::string& kind, const Loader& loader, const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const std::shared_ptr<const Loaded> loaded = cached(kind, loader, path);
    namespace uu = emulate::unicorn_uc;
    const std::unique_ptr<core16::Uc> uc = emulate::pcode_cpu::open_x86(64);   // (Unicorn, unless the one emulator is asked for: pcode_cpu_x86.hpp)
    uc->mem_map(loaded->low, loaded->high - loaded->low);
    for (const Loaded::Part& part : loaded->parts) uc->mem_write(part.address, part.bytes);
    uc->mem_map(kStubs, page_up(std::max<std::size_t>(loaded->imports.size(), 1) * 16));
    {
        Bytes returns(page_up(std::max<std::size_t>(loaded->imports.size(), 1) * 16), 0xC3);   // `ret`: a stub gives back the call it was made by
        uc->mem_write(kStubs, returns);
    }
    uc->mem_map(kThread, kThreadBytes);
    uc->mem_map(kWork, kWorkBytes);
    uc->mem_map(kStackTop - kStackBytes, kStackBytes);

    std::vector<std::string> stub_names;
    for (std::size_t k = 0; k < loaded->imports.size(); ++k) {
        stub_names.push_back(loaded->imports[k].second);
        Bytes value(8);
        for (int b = 0; b < 8; ++b) value[static_cast<std::size_t>(b)] = static_cast<std::uint8_t>((kStubs + k * 16) >> (8 * b));
        uc->mem_write(loaded->imports[k].first, value);
    }
    for (const auto& [address, value] : loaded->patches) {
        Bytes v(8);
        for (int b = 0; b < 8; ++b) v[static_cast<std::size_t>(b)] = static_cast<std::uint8_t>(value >> (8 * b));
        uc->mem_write(address, v);
    }
    // the thread block: its own address at offset 0 (the thread pointer's target), a stack guard value at 0x28
    {
        const std::uint64_t tcb = kThread + kThreadBytes / 2;
        Bytes block(16, 0);
        for (int b = 0; b < 8; ++b) block[static_cast<std::size_t>(b)] = static_cast<std::uint8_t>(tcb >> (8 * b));
        uc->mem_write(tcb, block);
        Bytes guard(8, 0x5A);
        uc->mem_write(tcb + 0x28, guard);
        uc->reg_write(reg(loaded->ms_abi ? "GS_BASE" : "FS_BASE"), tcb);
    }
    if (const std::int64_t seed = at(state, "fill"); seed != 0) {
        Bytes fill(kWorkBytes);
        for (std::size_t i = 0; i < fill.size(); ++i) fill[i] = fill_byte(i, seed);
        uc->mem_write(kWork, fill);
    }

    // what is compared of memory afterwards: the writable segments, the scratch area and the top of the stack
    std::vector<std::pair<std::uint64_t, std::uint64_t>> watched = loaded->writable;
    watched.emplace_back(kWork, kWorkBytes);
    watched.emplace_back(kStackTop - kStackWatched, kStackWatched);
    std::vector<Bytes> before;
    for (const auto& [address, size] : watched) before.push_back(uc->mem_read(address, size));

    std::vector<std::string> calls;
    bool ended = false;
    uc->hook_add_code(
        [&](core16::Uc& u, std::uint64_t address, std::uint32_t) {
            const std::size_t index = static_cast<std::size_t>((address - kStubs) / 16);
            if (index >= stub_names.size()) return;
            const std::string& name = stub_names[index];
            if (calls.size() < kShown) {
                const bool plain = loaded->ms_abi;
                calls.push_back(std::format("{}({:x}, {:x}, {:x})", name, u.reg_read(reg(plain ? "RCX" : "RDI")), u.reg_read(reg(plain ? "RDX" : "RSI")), u.reg_read(reg(plain ? "R8" : "RDX"))));
            }
            u.reg_write(reg("RAX"), 0);
            const std::string bare = name.substr(name.find('!') == std::string::npos ? 0 : name.find('!') + 1);
            if (loaded->ending.contains(bare)) {
                ended = true;
                u.emu_stop();
            }
        },
        kStubs, kStubs + stub_names.size() * 16);

    const std::shared_ptr<void> syscall_hook = emulate::pcode_cpu::hook_syscall(*uc, [&](core16::Uc& u) {
        const std::uint64_t rax = u.reg_read(reg("RAX")), rdi = u.reg_read(reg("RDI")), rsi = u.reg_read(reg("RSI")), rdx = u.reg_read(reg("RDX"));
        if (calls.size() < kShown) {
            const std::string name = loaded->syscall_name ? loaded->syscall_name(rax) : std::string();
            calls.push_back(std::format("syscall {}({:x}, {:x}, {:x})", name.empty() ? std::format("#{}", rax) : name, rdi, rsi, rdx));
        }
        u.reg_write(reg("RAX"), 0);
    });

    // the arguments and the stack: a return address on the top, which is where the run ends
    const char* const sysv[] = {"RDI", "RSI", "RDX", "RCX", "R8", "R9"};
    const char* const win[] = {"RCX", "RDX", "R8", "R9"};
    const char* const* names = loaded->ms_abi ? win : sysv;
    const std::size_t count = loaded->ms_abi ? 4 : 6;
    for (std::size_t k = 0; k < count; ++k) uc->reg_write(reg(names[k]), static_cast<std::uint64_t>(at(state, std::format("a{}", k).c_str())));
    std::uint64_t sp = kStackTop - 0x1000;
    if (loaded->ms_abi) sp -= 0x20;   // the shadow space the caller leaves
    sp -= 8;
    Bytes ret(8);
    for (int b = 0; b < 8; ++b) ret[static_cast<std::size_t>(b)] = static_cast<std::uint8_t>(kReturn >> (8 * b));
    uc->mem_write(sp, ret);
    uc->reg_write(reg("RSP"), sp);

    std::string end = "returned";
    try {
        uc->emu_start(entry + loaded->base, kReturn, 0, kSteps);
        const std::uint64_t rip = uc->reg_read(reg("RIP"));
        if (ended) end = "exit";
        else if (rip != kReturn) end = "steps";
    } catch (const emulate::unicorn_uc::UcFault& fault) {
        end = ended ? "exit" : std::format("fault {}", fault.code());
    }

    std::vector<std::string> changed;
    for (std::size_t w = 0; w < watched.size() && changed.size() < kShown; ++w) {
        const Bytes after = uc->mem_read(watched[w].first, watched[w].second);
        for (std::size_t i = 0; i < after.size() && changed.size() < kShown; ++i) {
            if (after[i] != before[w][i]) changed.push_back(std::format("{:x}={:02x}", watched[w].first + i, after[i]));
        }
    }
    Bytes saved;
    for (const char* r : {"RBX", "RBP", "R12", "R13", "R14", "R15"}) {
        const std::uint64_t v = uc->reg_read(reg(r));
        for (int b = 0; b < 8; ++b) saved.push_back(static_cast<std::uint8_t>(v >> (8 * b)));
    }
    const auto number = [](std::uint64_t v) { return vl::Effect(static_cast<std::int64_t>(v)); };
    return {vl::Effect(end), number(uc->reg_read(reg("RAX"))), number(uc->reg_read(reg("RDX"))), vl::Effect(std::move(saved)), number(uc->reg_read(reg("RSP"))),
            vl::Effect(std::move(changed)), vl::Effect(std::move(calls))};
}

vl::Written written_by(const source::image_writer::Written& w) {
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

vl::ImageLaneSpec spec_for(const std::string& kind, Loader loader, std::function<source::image_writer::Written(const stdfs::path&, const stdfs::path&)> writer) {
    vl::ImageLaneSpec spec;
    spec.kind = kind;
    spec.rev = 1;
    constexpr std::uint64_t work = kWork;
    spec.states = {make_state(0, 0, 0, 0, 0, 0, 0), make_state(work, work + 0x1000, 5, 0x10, work + 0x2000, 1, 1),
                   make_state(work + 0x800, 0xFFFFFFFFFFFFFFFFull, 0x7F, 0x80000000, 3, work + 0x100, 2), make_state(2, 3, work + 0x100, work + 0x200, 7, 0, 3)};
    spec.effects = [kind, loader](const stdfs::path& subject, std::uint32_t entry, const vl::State& state) { return effects(kind, loader, subject, entry, state); };
    spec.what = {"end", "RAX", "RDX", "RBX RBP R12-R15", "RSP", "memory changes", "calls"};
    spec.start = [](const vl::State& s) { return std::format("from a0={:x} a1={:x} a2={:x}", static_cast<std::uint64_t>(at(s, "a0")), static_cast<std::uint64_t>(at(s, "a1")), static_cast<std::uint64_t>(at(s, "a2"))); };
    spec.writer = [writer](const stdfs::path& module, const stdfs::path& out) { return written_by(writer(module, out)); };
    spec.key_digits = 6;
    return spec;
}

}  // namespace x64_lane_detail

vl::ImageLaneSpec ps4_lane_spec() {
    namespace d = x64_lane_detail;
    return d::spec_for("ps4", d::load_orbis, [](const stdfs::path& m, const stdfs::path& o) { return source::orbis_writer::reconstruct(m, o); });
}

vl::ImageLaneSpec ps5_lane_spec() {
    namespace d = x64_lane_detail;
    return d::spec_for("ps5", d::load_orbis, [](const stdfs::path& m, const stdfs::path& o) { return source::orbis_writer::reconstruct(m, o); });
}

void register_ps4_lane() {
    register_lane({
        .name = "ps4",
        .platform = platforms::ps4,
        .probe = [](const stdfs::path& p) { return fo::probe(p, fo::Console::Ps4); },
        .make_spec = ps4_lane_spec,
        .detectors = {{.priority = 120, .detect = formats::detect::info_ps4}},
    });
}

void register_ps5_lane() {
    register_lane({
        .name = "ps5",
        .platform = platforms::ps5,
        .probe = [](const stdfs::path& p) { return fo::probe(p, fo::Console::Ps5); },
        .make_spec = ps5_lane_spec,
    });
}

vl::ImageLaneSpec xboxone_lane_spec() {
    namespace d = x64_lane_detail;
    return d::spec_for("xboxone", d::load_xboxone, [](const stdfs::path& m, const stdfs::path& o) { return source::xboxone_writer::reconstruct(m, o); });
}

void register_xboxone_lane() {
    register_lane({
        .name = "xboxone",
        .platform = platforms::xboxone,
        .probe = [](const stdfs::path& p) { return formats::xboxone::probe(p); },
        .make_spec = xboxone_lane_spec,
        .detectors = {{.priority = 130, .detect = formats::detect::info_xboxone_1}, {.priority = 140, .detect = formats::detect::info_xboxone_2}},
    });
}

}  // namespace cb::engine
