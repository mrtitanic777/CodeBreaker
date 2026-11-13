// cb/engine/lane_kit_machines.cpp -- the real-mode program lane and the lanes of the cartridge machines, wired (see lane_kit.hpp).
//
// "dos" is the real-mode lane: the program written back as assembly, built, compared as a whole. "nes", "sms", "gb", "genesis", "zx" and "msx" are
// each one verify/lanes/image_lane: the cartridge (or snapshot, or tape) written back by its platform's writer and rebuilt; a program that comes back
// identical is the original, an edited one has each function run beside the original's in CodeBreaker's own interpreter of the machine's processor,
// from the same starting states, and what each did (how it ended, the registers, the memory it left, the hardware it touched) is compared. What is
// particular to a machine -- its starting states, its memory map, what answers its hardware -- is this file's one function per machine.
#include <algorithm>
#include <cstring>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/emulate/m6502.hpp"
#include "cb/emulate/m68k.hpp"
#include "cb/emulate/sm83.hpp"
#include "cb/emulate/z80.hpp"
#include "cb/engine/lane_kit_internal.hpp"
#include "cb/formats/gb.hpp"
#include "cb/formats/genesis.hpp"
#include "cb/formats/msx.hpp"
#include "cb/formats/nes.hpp"
#include "cb/formats/sms.hpp"
#include "cb/formats/zx.hpp"
#include "cb/source/dos_writer.hpp"
#include "cb/source/gb_writer.hpp"
#include "cb/source/genesis_writer.hpp"
#include "cb/source/msx_writer.hpp"
#include "cb/source/nes_writer.hpp"
#include "cb/source/sms_writer.hpp"
#include "cb/source/zx_writer.hpp"
#include "cb/verify/lanes/dos.hpp"
#include "cb/verify/lanes/image_lane.hpp"
#include "cb/verify/lanes/msx.hpp"
#include "cb/verify/lanes/zx.hpp"

namespace cb::engine {

namespace stdfs = std::filesystem;
namespace lanes = verify::lanes;

namespace {

constexpr std::uint64_t kSteps = 20000;   // instructions a function may run in a comparison
constexpr std::size_t kIoShown = 4000;    // hardware accesses kept of a run

// How RAM is filled from a seed (0: left as it is): small numbers on odd seeds, as counters and indexes hold.
std::uint8_t machine_fill(std::uint64_t i, std::int64_t seed) {
    return static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(seed) * 13) ^ (i >> 3)) & (seed % 2 != 0 ? 0x0F : 0xFF));
}

lanes::Effect number(std::uint64_t v) { return lanes::Effect(static_cast<std::int64_t>(v)); }

lanes::Effect bytes_of(ByteView v) { return lanes::Effect(Bytes(v.begin(), v.end())); }

lanes::Written written_of(const source::image_writer::Written& w) {
    lanes::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

std::int64_t at(const lanes::State& s, const char* key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

std::string io_line(std::string_view kind, std::uint64_t where, std::uint64_t value) { return std::format("{} {:x} {:x}", kind, where, value); }

// ---- the Game Boy ------------------------------------------------------------------------------------------------------------------

lanes::State gb_state(int a, int b, int c, int d, int e, int h, int l, int fill) {
    return {{"a", a}, {"b", b}, {"c", c}, {"d", d}, {"e", e}, {"h", h}, {"l", l}, {"fill", fill}};
}

lanes::Effects gb_effects(const stdfs::path& path, std::uint32_t entry, const lanes::State& state) {
    Bytes mem = formats::gb::read(path).memory();
    const std::int64_t seed = at(state, "fill");
    if (seed != 0)
        for (std::size_t i = 0; i < 0x2000; ++i) mem[0xC000 + i] = machine_fill(i, seed);
    emulate::sm83::Bus bus(mem);
    emulate::sm83::Bus* self = &bus;
    bus.map(0xFF00, 0xFF7F, [](std::uint16_t a) -> std::uint8_t { return a == 0xFF44 ? 0x90 : 0; });   // (LY in vblank)
    bus.map(0xFFFF, 0xFFFF);
    bus.map(0x2000, 0x3FFF, [self](std::uint16_t a) -> std::uint8_t { return self->memory()[a]; });    // bank select writes, logged
    emulate::sm83::RunOptions options;
    options.max_steps = kSteps;
    options.regs.a = static_cast<std::uint8_t>(at(state, "a"));
    options.regs.b = static_cast<std::uint8_t>(at(state, "b"));
    options.regs.c = static_cast<std::uint8_t>(at(state, "c"));
    options.regs.d = static_cast<std::uint8_t>(at(state, "d"));
    options.regs.e = static_cast<std::uint8_t>(at(state, "e"));
    options.regs.h = static_cast<std::uint8_t>(at(state, "h"));
    options.regs.l = static_cast<std::uint8_t>(at(state, "l"));
    const emulate::sm83::RunResult run = emulate::sm83::run(bus, static_cast<std::uint16_t>(entry), options);
    Bytes ram(bus.memory().begin() + 0xC000, bus.memory().begin() + 0xE000);
    ram.insert(ram.end(), bus.memory().begin() + 0xFF80, bus.memory().begin() + 0xFFFF);
    std::vector<std::string> io;
    for (const emulate::z80::BusAccess& x : run.io) {
        if (x.kind == emulate::z80::BusAccess::Kind::Read && x.where >= 0x2000 && x.where <= 0x3FFF) continue;
        io.push_back(io_line(emulate::z80::to_text(x.kind), x.where, x.value));
        if (io.size() == kIoShown) break;
    }
    return {lanes::Effect(run.exit()), number(run.regs.a), number(run.regs.f), number(run.regs.bc()), number(run.regs.de()), number(run.regs.hl()),
            lanes::Effect(std::move(ram)), lanes::Effect(std::move(io))};
}

// ---- the NES -----------------------------------------------------------------------------------------------------------------------

lanes::State nes_state(int a, int x, int y, int fill) { return {{"a", a}, {"x", x}, {"y", y}, {"fill", fill}}; }

lanes::Effects nes_effects(const stdfs::path& path, std::uint32_t entry, const lanes::State& state) {
    Bytes mem = formats::nes::read(path).memory();
    const std::int64_t seed = at(state, "fill");
    if (seed != 0)
        for (std::size_t i = 0; i < 0x800; ++i) mem[i] = machine_fill(i, seed);
    emulate::m6502::Bus bus(mem);
    bus.map(0x2000, 0x401F, [](std::uint16_t a) -> std::uint8_t { return a == 0x2002 ? 0x80 : 0; });   // (the picture unit always in vblank)
    emulate::m6502::RunOptions options;
    options.a = static_cast<std::uint8_t>(at(state, "a"));
    options.x = static_cast<std::uint8_t>(at(state, "x"));
    options.y = static_cast<std::uint8_t>(at(state, "y"));
    options.max_steps = kSteps;
    const emulate::m6502::RunResult run = emulate::m6502::run(bus, static_cast<std::uint16_t>(entry), options);
    std::vector<std::string> io;
    for (const emulate::m6502::DeviceAccess& x : run.io) {
        io.push_back(io_line(x.kind == emulate::m6502::DeviceAccess::Kind::Read ? "r" : "w", x.address, x.value));
        if (io.size() == kIoShown) break;
    }
    return {lanes::Effect(run.exit()), number(run.regs.a), number(run.regs.x), number(run.regs.y), number(run.regs.p),
            bytes_of(ByteView(bus.memory().data(), 0x800)), lanes::Effect(std::move(io))};
}

// ---- the Master System and Game Gear -----------------------------------------------------------------------------------------------

lanes::Effects sms_effects(const stdfs::path& path, std::uint32_t entry, const lanes::State& state) {
    Bytes mem = formats::sms::read(path).memory();
    const std::int64_t seed = at(state, "fill");
    if (seed != 0)
        for (std::size_t i = 0; i < 0x2000; ++i) mem[0xC000 + i] = machine_fill(i, seed);
    emulate::z80::Bus bus(mem, [](std::uint16_t port) -> std::uint8_t { return (port & 0xFF) == 0xBF ? 0x80 : 0xFF; });   // (the VDP always in vblank)
    bus.map(0xFFFC, 0xFFFF);
    emulate::z80::RunOptions options;
    options.max_steps = kSteps;
    options.regs.a = static_cast<std::uint8_t>(at(state, "a"));
    options.regs.b = static_cast<std::uint8_t>(at(state, "b"));
    options.regs.c = static_cast<std::uint8_t>(at(state, "c"));
    options.regs.d = static_cast<std::uint8_t>(at(state, "d"));
    options.regs.e = static_cast<std::uint8_t>(at(state, "e"));
    options.regs.h = static_cast<std::uint8_t>(at(state, "h"));
    options.regs.l = static_cast<std::uint8_t>(at(state, "l"));
    const emulate::z80::RunResult run = emulate::z80::run(bus, static_cast<std::uint16_t>(entry), options);
    std::vector<std::string> io;
    for (const emulate::z80::BusAccess& x : run.io) {
        io.push_back(io_line(emulate::z80::to_text(x.kind), x.where, x.value));
        if (io.size() == kIoShown) break;
    }
    const emulate::z80::Registers& r = run.regs;
    return {lanes::Effect(run.exit()), number(r.a),  number(r.f),  number(r.bc()), number(r.de()),
            number(r.hl()),           number(r.ix), number(r.iy), bytes_of(ByteView(bus.memory().data() + 0xC000, 0x2000)), lanes::Effect(std::move(io))};
}

// ---- the Genesis / Mega Drive ------------------------------------------------------------------------------------------------------

lanes::State genesis_state(const std::array<std::uint32_t, 8>& d, const std::array<std::uint32_t, 7>& a, int fill) {
    lanes::State s{{"fill", fill}};
    for (std::size_t i = 0; i < 8; ++i) s["d" + std::to_string(i)] = d[i];
    for (std::size_t i = 0; i < 7; ++i) s["a" + std::to_string(i)] = a[i];
    return s;
}

lanes::Effects genesis_effects(const stdfs::path& path, std::uint32_t entry, const lanes::State& state) {
    Bytes mem = formats::genesis::read(path).memory();
    if (mem.size() < emulate::m68k::kMemorySize) mem.resize(emulate::m68k::kMemorySize);
    const std::int64_t seed = at(state, "fill");
    if (seed != 0)
        for (std::size_t i = 0; i < 0x10000; ++i) mem[0xFF0000 + i] = machine_fill(i, seed);
    emulate::m68k::Bus bus(mem);
    const auto hardware = [](std::uint32_t a, unsigned) -> std::uint32_t { return (a & ~1u) == 0xC00004 ? 0x0008 : 0; };   // (VDP status: in vblank)
    bus.map(0xA00000, 0xA1FFFF, hardware);
    bus.map(0xC00000, 0xC0001F, hardware);
    emulate::m68k::RunOptions options;
    options.max_steps = kSteps;
    std::array<std::uint32_t, 8> d{}, a{};
    for (std::size_t i = 0; i < 8; ++i) d[i] = static_cast<std::uint32_t>(at(state, ("d" + std::to_string(i)).c_str()));
    for (std::size_t i = 0; i < 7; ++i) a[i] = static_cast<std::uint32_t>(at(state, ("a" + std::to_string(i)).c_str()));
    options.d = d;
    options.a = a;
    const emulate::m68k::RunResult run = emulate::m68k::run(bus, entry, options);
    Bytes dregs, aregs;
    const auto put = [](Bytes& out, std::uint32_t v) {
        for (int shift = 24; shift >= 0; shift -= 8) out.push_back(static_cast<std::uint8_t>(v >> shift));
    };
    for (std::size_t i = 0; i < 8; ++i) put(dregs, run.d[i]);
    for (std::size_t i = 0; i < 7; ++i) put(aregs, run.a[i]);
    std::vector<std::string> io;
    for (const emulate::m68k::BusAccess& x : run.io) {
        io.push_back(io_line(x.text(), x.address, x.value));
        if (io.size() == kIoShown) break;
    }
    return {lanes::Effect(run.exit()), lanes::Effect(std::move(dregs)), lanes::Effect(std::move(aregs)), number(run.sr & 0x1F),
            bytes_of(ByteView(bus.memory().data() + 0xFF0000, 0x10000)), lanes::Effect(std::move(io))};
}

// ---- the flat Z80 programs: the ZX Spectrum and the MSX ----------------------------------------------------------------------------------

// What a flat program is run in is a file: the origin, the length of the block, then the 64 KB of memory as it is laid out (the machine's ROM every
// byte RET, the block at its origin).
Bytes flat_file(std::uint32_t origin, ByteView image, ByteView base, std::pair<std::uint32_t, std::uint32_t> rom) {
    Bytes memory(0x10000, 0);
    std::copy_n(base.begin(), std::min<std::size_t>(base.size(), 0x10000), memory.begin());
    std::fill(memory.begin() + rom.first, memory.begin() + rom.second, std::uint8_t{0xC9});
    for (std::size_t i = 0; i < image.size() && origin + i < 0x10000; ++i) memory[origin + i] = image[i];
    Bytes out;
    for (const std::uint32_t v : {origin, static_cast<std::uint32_t>(image.size())})
        for (int shift = 0; shift < 32; shift += 8) out.push_back(static_cast<std::uint8_t>(v >> shift));
    out.insert(out.end(), memory.begin(), memory.end());
    return out;
}

lanes::State flat_state(int a, int b, int c, int d, int e, int h, int l, int fill) { return gb_state(a, b, c, d, e, h, l, fill); }

lanes::Effects flat_effects(const stdfs::path& path, std::uint32_t entry, const lanes::State& state) {
    const Bytes file = fs::read_file(path);
    CB_ASSERT(file.size() == 8 + 0x10000, "a flat program file is its origin, its length and 64 KB of memory");
    const auto word = [&](std::size_t at_) { return static_cast<std::uint32_t>(file[at_] | file[at_ + 1] << 8 | file[at_ + 2] << 16 | static_cast<std::uint32_t>(file[at_ + 3]) << 24); };
    const std::uint32_t end = word(0) + word(4);
    Bytes mem(file.begin() + 8, file.end());
    const std::int64_t seed = at(state, "fill");
    if (seed != 0)
        for (std::uint32_t i = end; i < 0x10000; ++i) mem[i] = machine_fill(i, seed);   // memory past the program
    const Bytes before = mem;
    emulate::z80::Bus bus(mem, [](std::uint16_t) -> std::uint8_t { return 0xFF; });
    emulate::z80::RunOptions options;
    options.max_steps = kSteps;
    options.sp = 0xFFF0;
    options.regs.a = static_cast<std::uint8_t>(at(state, "a"));
    options.regs.b = static_cast<std::uint8_t>(at(state, "b"));
    options.regs.c = static_cast<std::uint8_t>(at(state, "c"));
    options.regs.d = static_cast<std::uint8_t>(at(state, "d"));
    options.regs.e = static_cast<std::uint8_t>(at(state, "e"));
    options.regs.h = static_cast<std::uint8_t>(at(state, "h"));
    options.regs.l = static_cast<std::uint8_t>(at(state, "l"));
    const emulate::z80::RunResult run = emulate::z80::run(bus, static_cast<std::uint16_t>(entry), options);
    // what the run changed (the program lives in this memory: an edited byte the function does not write is the source's difference, not the function's)
    std::vector<std::string> changed;
    for (std::size_t i = 0; i < 0x10000; ++i)
        if (before[i] != bus.memory()[i]) changed.push_back(std::format("{:04x}={:02x}", i, bus.memory()[i]));
    std::vector<std::string> io;
    for (const emulate::z80::BusAccess& x : run.io) {
        io.push_back(io_line(emulate::z80::to_text(x.kind), x.where, x.value));
        if (io.size() == kIoShown) break;
    }
    const emulate::z80::Registers& r = run.regs;
    return {lanes::Effect(run.exit()), number(r.a),  number(r.f),  number(r.bc()), number(r.de()),
            number(r.hl()),           number(r.ix), number(r.iy), lanes::Effect(std::move(changed)), lanes::Effect(std::move(io))};
}

// The flat machine's lane: its writer leaves the block it rebuilt in the folder beside the source, and the two programs are run from files.
struct FlatMachine {
    std::string kind;
    int rev;
    std::pair<std::uint32_t, std::uint32_t> rom;
    std::function<source::image_writer::Written(const stdfs::path&, const stdfs::path&)> reconstruct;
    // The original's block: its origin, bytes, and the memory around it.
    std::function<std::tuple<std::uint32_t, Bytes, Bytes>(const stdfs::path&)> open;
};

lanes::ImageLaneSpec flat_spec(const FlatMachine& m) {
    lanes::ImageLaneSpec spec;
    spec.kind = m.kind;
    spec.rev = m.rev;
    spec.states = {flat_state(0, 0, 0, 0, 0, 0, 0, 0), flat_state(1, 2, 3, 0xC1, 0x00, 0xC2, 0x10, 1), flat_state(0x80, 0xFF, 0x7F, 0xD0, 0x80, 0xE1, 0x00, 2)};
    spec.effects = flat_effects;
    spec.what = {"end", "A", "flags", "BC", "DE", "HL", "IX", "IY", "memory", "port accesses"};
    spec.start = [](const lanes::State& s) {
        return std::format("from A={:02X} BC={:02X}{:02X} HL={:02X}{:02X}", at(s, "a"), at(s, "b"), at(s, "c"), at(s, "h"), at(s, "l"));
    };
    spec.writer = [m](const stdfs::path& module, const stdfs::path& out) {
        const source::image_writer::Written w = m.reconstruct(module, out);
        lanes::Written r = written_of(w);
        if (w.built_image) {   // the rebuilt block, kept beside the source for the runs
            r.built = out / (m.kind + ".built.block");
            fs::write_file(r.built, *w.built_image);
        }
        return r;
    };
    spec.subjects = [m](const stdfs::path& module, const lanes::Written& written) {
        const auto [origin, image, base] = m.open(module);
        const stdfs::path folder = written.built.parent_path();
        const stdfs::path original = folder / (m.kind + ".original.flat"), rebuilt = folder / (m.kind + ".rebuilt.flat");
        fs::write_file(original, flat_file(origin, image, base, m.rom));
        fs::write_file(rebuilt, flat_file(origin, fs::read_file(written.built), base, m.rom));
        return std::make_pair(original, rebuilt);
    };
    return spec;
}

}  // namespace

// ---- the lanes ----------------------------------------------------------------------------------------------------------------------

std::shared_ptr<const verify::lanes::Kind> LaneKit::dos_lane() {
    {
        std::lock_guard hold(impl_->lock);
        if (impl_->dos_) return impl_->dos_;
    }
    Impl& i = *impl_;
    const build::watcom::OpenWatcom ow = i.openwatcom();
    if (ow.tool("wasm").empty() || ow.tool("wlink").empty()) return nullptr;
    LaneKit* kit = this;
    source::dos_writer::Options options = source::dos_writer::options_for(ow, i.config.state() / "assembly_probes.json");
    options.build_windows = [kit](const stdfs::path& program, const stdfs::path& outdir) {   // a DOS+Windows file: its Windows module built whole
        source::dos_writer::WindowsBuild w;
        try {
            const build::module::ModuleBuilder builder(kit->module_services());
            build::module::BuildOptions build_options;
            build_options.outdir = outdir / "windows";
            build_options.log = [](std::string_view) {};
            const build::module::Built b = builder.build(fs::to_text(program), build_options);
            w.linked = b.linked();
            w.functions = b.functions;
            std::set<std::string> stand_ins(b.absent.begin(), b.absent.end());
            for (const auto& [name, why] : b.rejected) stand_ins.insert(name);
            w.stand_ins.assign(stand_ins.begin(), stand_ins.end());
            w.undefined = b.undefined;
            for (const auto& x : b.imports_extra) w.extra_imports.push_back(x.module + "." + x.text());
            w.ran = b.ran.size();
            for (const build::module::RanExport& r : b.ran) w.ran_same += r.same;
            w.error = b.error;
        } catch (const Error& e) {
            w.error = e.what();
        }
        return w;
    };
    lanes::Writer writer = [options](const stdfs::path& module, const stdfs::path& out) {
        lanes::Written w;
        fs::make_dirs(out);
        const source::dos_writer::Result r = source::dos_writer::reconstruct(module, out, options);
        if (!r.sources.empty()) w.source = r.sources.front();
        for (const auto& [address, f] : r.functions) w.functions[address] = f.name;
        w.linked = r.linked;
        w.same = r.same;
        w.notes = r.notes;
        if (r.image) w.built = *r.image;
        return w;
    };
    lanes::DosLane::Tools tools = [ow] {
        std::map<std::string, stdfs::path> out;
        out["wasm"] = ow.tool("wasm");
        out["wlink"] = ow.tool("wlink");
        return out;
    };
    auto lane = std::make_shared<lanes::DosLane>(std::move(writer), std::move(tools));
    std::lock_guard hold(impl_->lock);
    return impl_->dos_ = std::move(lane);
}

std::shared_ptr<const verify::lanes::Kind> LaneKit::machine_lane(std::string_view name) {
    {
        std::lock_guard hold(impl_->lock);
        if (const auto it = impl_->machines_.find(std::string(name)); it != impl_->machines_.end()) return it->second;
    }
    lanes::ImageLaneSpec spec;
    if (name == "gb") {
        spec.kind = "gb";
        spec.rev = 1;
        spec.states = {gb_state(0, 0, 0, 0, 0, 0, 0, 0), gb_state(1, 2, 3, 0xC1, 0x00, 0xC2, 0x10, 1), gb_state(0x80, 0xFF, 0x7F, 0xC0, 0x80, 0xC1, 0x00, 2),
                       gb_state(0x55, 0x10, 0xAA, 0xD0, 0x00, 0xC8, 0x40, 3)};
        spec.effects = gb_effects;
        spec.what = {"end", "A", "flags", "BC", "DE", "HL", "RAM", "I/O and bank accesses"};
        spec.start = [](const lanes::State& s) { return std::format("from A={:02X} BC={:02X}{:02X} HL={:02X}{:02X}", at(s, "a"), at(s, "b"), at(s, "c"), at(s, "h"), at(s, "l")); };
        spec.writer = [](const stdfs::path& m, const stdfs::path& out) { return written_of(source::gb_writer::reconstruct(m, out)); };
    } else if (name == "nes") {
        spec.kind = "nes";
        spec.rev = 2;
        spec.states = {nes_state(0, 0, 0, 0), nes_state(1, 2, 3, 1), nes_state(0x80, 0xFF, 0x7F, 2), nes_state(0x55, 0x10, 0xAA, 3), nes_state(0, 0, 0, 4), nes_state(0xFF, 1, 0, 5)};
        spec.effects = nes_effects;
        spec.what = {"end", "A", "X", "Y", "flags", "RAM", "hardware accesses"};
        spec.start = [](const lanes::State& s) { return std::format("from A={:02X} X={:02X} Y={:02X}", at(s, "a"), at(s, "x"), at(s, "y")); };
        spec.writer = [](const stdfs::path& m, const stdfs::path& out) { return written_of(source::nes_writer::reconstruct(m, out)); };
    } else if (name == "sms") {
        spec.kind = "sms";
        spec.rev = 1;
        spec.states = {gb_state(0, 0, 0, 0, 0, 0, 0, 0), gb_state(1, 2, 3, 0xC1, 0x00, 0xC2, 0x10, 1), gb_state(0x80, 0xFF, 0x7F, 0xC0, 0x80, 0xC1, 0x00, 2),
                       gb_state(0x55, 0x10, 0xAA, 0xD0, 0x00, 0xC8, 0x40, 3)};
        spec.effects = sms_effects;
        spec.what = {"end", "A", "flags", "BC", "DE", "HL", "IX", "IY", "RAM", "port and mapper accesses"};
        spec.start = [](const lanes::State& s) { return std::format("from A={:02X} BC={:02X}{:02X} HL={:02X}{:02X}", at(s, "a"), at(s, "b"), at(s, "c"), at(s, "h"), at(s, "l")); };
        spec.writer = [](const stdfs::path& m, const stdfs::path& out) { return written_of(source::sms_writer::reconstruct(m, out)); };
    } else if (name == "genesis") {
        spec.kind = "genesis";
        spec.rev = 1;
        spec.states = {genesis_state({0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, 0),
                       genesis_state({1, 2, 3, 4, 5, 6, 7, 8}, {0xFF0100, 0xFF0200, 0xFF0300, 0xFF0400, 0xFF0500, 0xFF0600, 0xFF0700}, 1),
                       genesis_state({0x8000, 0xFFFF, 0x7F, 0x80000000, 0x10, 0x20, 0x30, 0x40}, {0xFF8000, 0xFF9000, 0xFFA000, 0xFFB000, 0xFFC000, 0xFFD000, 0xFFE000}, 2)};
        spec.effects = genesis_effects;
        spec.what = {"end", "D registers", "A registers", "condition codes", "RAM", "hardware accesses"};
        spec.start = [](const lanes::State& s) { return std::format("from D0={:08X} A0={:06X}", at(s, "d0"), at(s, "a0")); };
        spec.writer = [](const stdfs::path& m, const stdfs::path& out) { return written_of(source::genesis_writer::reconstruct(m, out)); };
        spec.key_digits = 6;
    } else if (name == "zx") {
        spec = flat_spec(FlatMachine{
            std::string(lanes::zx::kKind), lanes::zx::kRev, {lanes::zx::kRomLow, lanes::zx::kRomHigh},
            [](const stdfs::path& m, const stdfs::path& out) { return source::zx_writer::reconstruct(m, out); },
            [](const stdfs::path& m) {
                const formats::zx::Program p = formats::zx::read(m);
                return std::make_tuple(p.origin, p.image, p.memory);
            }});
    } else if (name == "msx") {
        spec = flat_spec(FlatMachine{
            std::string(lanes::msx::kKind), lanes::msx::kRev, {lanes::msx::kRomLow, lanes::msx::kRomHigh},
            [](const stdfs::path& m, const stdfs::path& out) { return source::msx_writer::reconstruct(m, out); },
            [](const stdfs::path& m) {
                const formats::msx::Program p = formats::msx::read(m);
                return std::make_tuple(p.origin, p.image, Bytes());
            }});
    } else {
        return nullptr;
    }
    auto lane = std::make_shared<lanes::ImageLane>(std::move(spec));
    std::lock_guard hold(impl_->lock);
    return impl_->machines_.emplace(std::string(name), std::move(lane)).first->second;
}

// The machine a file is made for, by its format (a cartridge, a snapshot, a tape), in the order the platforms are tried; empty when it is none.
std::string kit_detail::machine_of_file(const stdfs::path& path) {
    if (formats::nes::probe(path)) return "nes";
    if (formats::sms::probe(path)) return "sms";
    if (formats::gb::probe(path)) return "gb";
    if (formats::genesis::probe(path)) return "genesis";
    if (formats::zx::probe(path)) return "zx";
    if (formats::msx::probe(path)) return "msx";
    return {};
}

}  // namespace cb::engine
