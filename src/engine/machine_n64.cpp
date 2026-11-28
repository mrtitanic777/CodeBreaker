// cb-unity: alone (file-local names that other files in a batch also use)
#include "engine/machine_n64.hpp"

#include <format>
#include <optional>
#include <memory>

#include "core/error.hpp"
#include "emulate/r4300.hpp"
#include "emulate/rdp.hpp"
#include "emulate/rsp.hpp"
#include "engine/lane_registry.hpp"
#include "engine/machine_lanes.hpp"
#include "emulate/cpu/pcode_cpu.hpp"
#include "formats/read.hpp"
#include "formats/n64.hpp"
#include "platforms/package.hpp"
#include "source/console_writer.hpp"

// What `info` says of the lane's files (formats/detect.hpp), as it was written in formats/detect.cpp.
namespace cb::formats::detect {
namespace {
std::optional<std::vector<Detected>> info_n64(const DetectInput& in) {
    [[maybe_unused]] const ByteView file = in.file;
    [[maybe_unused]] const ByteView head = in.head;
    [[maybe_unused]] const ByteView h4 = clamped(head, 0, 4);
    [[maybe_unused]] const std::uint64_t size = in.size;
    [[maybe_unused]] const std::string_view name = in.name;
    if (const std::optional<n64::ByteOrder> order = size >= n64::kBootSize ? n64::order_of(head) : std::nullopt; order && head.size() >= n64::kHeaderSize) {   // a Nintendo 64 ROM: the PI configuration word $80371240, in one of three byte orders
        const Bytes z64 = n64::Rom::convert(head.first(n64::kHeaderSize), *order, n64::ByteOrder::Z64);
        const auto word = [&](std::size_t at) { return static_cast<std::uint32_t>(z64[at]) << 24 | static_cast<std::uint32_t>(z64[at + 1]) << 16 | static_cast<std::uint32_t>(z64[at + 2]) << 8 | z64[at + 3]; };
        std::string title(as_text(ByteView(z64).subspan(0x20, 20)));
        while (!title.empty() && (title.back() == ' ' || title.back() == '\0')) title.pop_back();
        return one("Nintendo 64 ROM", std::format("\"{}\" ({}{}), {} ({}), {} bytes, entry 0x{:08X}", title, as_text(ByteView(z64).subspan(0x3C, 2)), static_cast<char>(z64[0x3E]), std::string(n64::order_text(*order)).substr(1),
                                                    *order == n64::ByteOrder::Z64 ? "big-endian" : *order == n64::ByteOrder::V64 ? "byte-swapped" : "little-endian", size, word(8)),
                   "n64", "reach");
    }
    return std::nullopt;
}
}  // namespace
}  // namespace cb::formats::detect

namespace cb::engine {

namespace stdfs = std::filesystem;
namespace vl = verify::lanes;

namespace n64_lane_detail {

constexpr std::uint64_t kSteps = 20000;   // instructions a function may run in a comparison
constexpr std::uint64_t kRspSteps = 200000;   // instructions a microcode task the function starts may run
constexpr std::size_t kListsShown = 4000;     // RDP commands kept of a run
constexpr std::size_t kShown = 4000;      // register accesses and memory changes kept of a run
// RDRAM a run may be handed pointers into: seeded from `fill`, above the program (1 MB from the load address) and the stack.
constexpr std::uint32_t kWork = 0x80600000;
constexpr std::size_t kWorkBytes = 0x10000;

std::uint8_t fill_byte(std::uint64_t i, std::int64_t seed) {
    return static_cast<std::uint8_t>(((i * 7 + static_cast<std::uint64_t>(seed) * 13) ^ (i >> 3)) & (seed % 2 != 0 ? 0x0F : 0xFF));
}

std::int64_t at(const vl::State& s, const char* key) {
    const auto it = s.find(key);
    return it == s.end() ? 0 : it->second;
}

vl::State make_state(std::uint32_t a0, std::uint32_t a1, std::uint32_t a2, std::uint32_t a3, std::uint32_t s0, int fill) {
    return {{"a0", a0}, {"a1", a1}, {"a2", a2}, {"a3", a3}, {"s0", s0}, {"fill", fill}};
}

vl::Effects effects(const stdfs::path& path, std::uint32_t entry, const vl::State& state) {
    const formats::n64::Rom rom = formats::n64::read(path);
    Bytes ram = rom.memory();
    if (const std::int64_t seed = at(state, "fill"); seed != 0) {
        const std::size_t work = kWork & 0x1FFFFFFF;
        for (std::size_t i = 0; i < kWorkBytes; ++i) ram[work + i] = fill_byte(i, seed);
    }
    emulate::r4300::Bus bus(ram, rom.data);
    // The RSP and the RDP's command interface: SP_STATUS cleared of its halt bit runs the microcode in IMEM from SP_PC to its BREAK
    // (emulate/rsp), and every display list handed over by a write to DPC_END (by the microcode, or by the CPU itself) is read as its
    // commands (emulate/rdp), which are compared.
    emulate::rsp::Rsp rsp(bus.sp_memory(), bus.rdram());
    std::uint32_t sp_pc = 0;
    std::vector<std::string> rdp;
    std::vector<std::string> rsp_runs;
    rsp.on_dpc = [&](std::uint32_t start, std::uint32_t end, bool xbus) {
        const ByteView memory = xbus ? ByteView(bus.sp_memory()).first(emulate::rsp::kMemSize) : ByteView(bus.rdram());
        for (const emulate::rdp::Command& c : emulate::rdp::commands(memory, xbus ? start & 0xFFF : start, xbus ? end & 0xFFF : end))
            if (rdp.size() < kListsShown) rdp.push_back(c.text());
    };
    bus.map(0x04040000, 0x0404001F, [&](std::uint32_t a, unsigned) -> std::uint64_t { return rsp.cop0((a >> 2) & 7); },
            [&](std::uint32_t a, unsigned, std::uint64_t v) {
                const unsigned reg = (a >> 2) & 7;
                rsp.set_cop0(reg, static_cast<std::uint32_t>(v));
                if (reg == 4 && !(rsp.status() & emulate::rsp::kHalt)) {   // the CPU started the RSP
                    const emulate::rsp::RunResult r = rsp.run(sp_pc, kRspSteps);
                    rsp_runs.push_back(std::format("{:03x} {} {}", sp_pc, r.steps, r.broke ? std::string("break") : r.fault.value_or("")));
                    sp_pc = rsp.pc();
                    rsp.set_status(rsp.status() | emulate::rsp::kHalt);
                }
            });
    bus.map(0x04080000, 0x04080007, [&](std::uint32_t, unsigned) -> std::uint64_t { return sp_pc; },
            [&](std::uint32_t, unsigned, std::uint64_t v) { sp_pc = static_cast<std::uint32_t>(v) & 0xFFC; });
    bus.map(0x04100000, 0x0410001F, [&](std::uint32_t a, unsigned) -> std::uint64_t { return rsp.cop0(8 + ((a >> 2) & 7)); },
            [&](std::uint32_t a, unsigned, std::uint64_t v) { rsp.set_cop0(8 + ((a >> 2) & 7), static_cast<std::uint32_t>(v)); });
    const Bytes ram_before = ram;
    const Bytes sp_before(bus.sp_memory().begin(), bus.sp_memory().end());
    emulate::r4300::RunOptions options;
    options.max_steps = kSteps;
    options.a = {static_cast<std::uint32_t>(at(state, "a0")), static_cast<std::uint32_t>(at(state, "a1")), static_cast<std::uint32_t>(at(state, "a2")),
                 static_cast<std::uint32_t>(at(state, "a3"))};
    options.s[0] = static_cast<std::uint32_t>(at(state, "s0"));
    const emulate::r4300::RunResult run = cb::emulate::pcode_cpu::call(bus, entry, options);
    // what the run changed in RDRAM and SP memory (the program is in RDRAM: its own bytes are not an effect unless the run rewrote them)
    std::vector<std::string> changed;
    for (std::size_t i = 0; i < ram.size() && changed.size() < kShown; ++i)
        if (ram_before[i] != bus.rdram()[i]) changed.push_back(std::format("{:08x}={:02x}", i, bus.rdram()[i]));
    for (std::size_t i = 0; i < sp_before.size() && changed.size() < kShown; ++i)
        if (sp_before[i] != bus.sp_memory()[i]) changed.push_back(std::format("{:08x}={:02x}", emulate::r4300::kDmem + i, bus.sp_memory()[i]));
    Bytes saved;
    for (std::size_t i = 16; i < 24; ++i)
        for (int shift = 56; shift >= 0; shift -= 8) saved.push_back(static_cast<std::uint8_t>(run.r[i] >> shift));
    std::vector<std::string> io;
    for (const emulate::r4300::BusAccess& x : run.io) {
        io.push_back(std::format("{} {:x} {:x}", x.text(), x.address, x.value));
        if (io.size() == kShown) break;
    }
    const auto number = [](std::uint64_t v) { return vl::Effect(static_cast<std::int64_t>(v)); };
    return {vl::Effect(run.exit()), number(run.r[2]), number(run.r[3]), vl::Effect(std::move(saved)), number(run.r[29]), number(run.hi), number(run.lo), number(run.fpr[0]),
            vl::Effect(std::move(changed)), vl::Effect(std::move(io)), vl::Effect(std::move(rsp_runs)), vl::Effect(std::move(rdp))};
}

vl::Written written_by(const stdfs::path& module, const stdfs::path& out) {
    const source::image_writer::Written w = source::console_writer::reconstruct("n64", module, out);
    vl::Written r;
    r.source = w.source;
    r.functions = w.functions;
    r.linked = w.linked;
    r.notes = w.notes;
    r.same = w.same;
    if (w.built) r.built = *w.built;
    return r;
}

}  // namespace n64_lane_detail

namespace d = n64_lane_detail;

vl::ImageLaneSpec n64_lane_spec() {
    vl::ImageLaneSpec spec;
    spec.kind = "n64";
    spec.rev = 2;   // 2: the RSP run and the RDP's commands
    spec.states = {d::make_state(0, 0, 0, 0, 0, 0), d::make_state(d::kWork, d::kWork + 0x1000, 5, 0x10, 0, 1),
                   d::make_state(d::kWork + 0x2000, 0xFFFFFFFF, 0x7F, 0x80000000, 3, 2), d::make_state(2, 3, d::kWork + 0x100, d::kWork + 0x200, 7, 3)};
    spec.effects = d::effects;
    spec.what = {"end", "V0", "V1", "S0-S7", "SP", "HI", "LO", "F0", "memory changes", "hardware accesses", "RSP tasks", "RDP commands"};
    spec.start = [](const vl::State& s) { return std::format("from A0={:08X} A1={:08X} A2={:X}", d::at(s, "a0"), d::at(s, "a1"), d::at(s, "a2")); };
    spec.writer = d::written_by;
    spec.key_digits = 8;
    return spec;
}

void register_n64_lane() {
    register_lane({
        .name = "n64",
        .platform = [] { return platforms::described("n64"); },
        .probe = [](const stdfs::path& p) { return formats::n64::probe(p); },
        .make_spec = n64_lane_spec,
        .detectors = {{.priority = 190, .detect = formats::detect::info_n64}},
        .program_extensions = {".z64", ".n64", ".v64"},
    });
}

}  // namespace cb::engine
