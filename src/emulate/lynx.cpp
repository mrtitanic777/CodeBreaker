#include "emulate/lynx.hpp"

#include <algorithm>
#include <format>

#include "decompile/m6502.hpp"

namespace cb::emulate::lynx {

namespace dec = cb::decompile::m6502;
namespace fl = cb::formats::lynx;

namespace {

constexpr std::uint16_t kRcart0 = 0xFCB2;
constexpr std::uint16_t kRcart1 = 0xFCB3;
constexpr std::uint16_t kShift = 0xFE00;       // the boot ROM's shift routine: select cartridge block A
constexpr int kStage1Owner = -2;

// What the devices answer a read (see the header).
std::uint8_t device_read(std::uint16_t address) {
    if (address == 0xFD81) return 0xFF;                                          // INTSET: every interrupt pending
    if (address == 0xFD8C) return 0xA0;                                          // SERCTL: ready to send, send register empty
    if (address >= 0xFD00 && address < 0xFD20 && (address & 3) == 3) return 0x08;   // a timer's CTLB: timer done
    return 0;
}

// The effective address a store instruction writes (a pointer in page zero wraps there), read from memory without going through the bus.
std::optional<std::uint16_t> store_address(const dec::Insn& insn, const m6502::Registers& regs, ByteView mem) {
    const std::uint16_t v = insn.operand.value_or(0);
    const auto pointer = [&](std::uint8_t at) { return static_cast<std::uint16_t>(mem[at] | (mem[static_cast<std::uint8_t>(at + 1)] << 8)); };
    switch (insn.mode) {
        case dec::Mode::Zp: return v;
        case dec::Mode::Zpx: return static_cast<std::uint8_t>(v + regs.x);
        case dec::Mode::Abs: return v;
        case dec::Mode::Abx: return static_cast<std::uint16_t>(v + regs.x);
        case dec::Mode::Aby: return static_cast<std::uint16_t>(v + regs.y);
        case dec::Mode::Izp: return pointer(static_cast<std::uint8_t>(v));
        case dec::Mode::Izx: return pointer(static_cast<std::uint8_t>(v + regs.x));
        case dec::Mode::Izy: return static_cast<std::uint16_t>(pointer(static_cast<std::uint8_t>(v)) + regs.y);
        default: return std::nullopt;
    }
}

struct Copy {
    std::uint32_t cart = 0;      // the byte's place in the cartridge
    std::uint16_t address = 0;   // where it was stored
};

}  // namespace

m6502::Bus make_bus(ByteView image) {
    m6502::Bus bus(image.first(std::min<std::size_t>(image.size(), 0x10000)));
    bus.map(0xFC00, 0xFDFF, [](std::uint16_t a) { return device_read(a); });   // Suzy and Mikey: logged
    bus.map(0xFFF9, 0xFFF9);                                                   // the memory map control: logged
    return bus;
}

Boot boot(const fl::Program& program) {
    Boot out;
    if (program.kind == fl::Kind::Bs93) {
        out.segments = program.segments;
        out.entries = program.entries;
        out.ended = "running";
        return out;
    }
    if (program.stage1.empty()) {
        out.ended = "no first stage";
        return out;
    }
    const ByteView cart = program.cart();
    const std::size_t block_size = program.bank0_block != 0 ? program.bank0_block : 1024;
    std::size_t position = 1 + fl::kEncryptedBlock * program.encrypted_blocks;   // the boot ROM has read the first stage
    std::size_t last_read = 0;
    std::uint8_t last_value = 0;
    bool read_now = false;

    m6502::Bus bus;
    std::copy(program.stage1.begin(), program.stage1.end(), bus.memory().begin() + fl::kStage1Address);
    const auto cartridge = [&](std::uint16_t) -> std::uint8_t {
        last_read = position;
        last_value = position < cart.size() ? cart[position] : 0;
        ++position;
        read_now = true;
        return last_value;
    };
    bus.map_quiet(kRcart0, kRcart0, cartridge);
    bus.map_quiet(kRcart1, kRcart1, [](std::uint16_t) -> std::uint8_t { return 0; });
    bus.map_quiet(0xFC00, 0xFDFF, [](std::uint16_t a) { return device_read(a); }, [](std::uint16_t, std::uint8_t) {});

    std::vector<int> owner(0x10000, -1);
    std::vector<char> running(0x10000, 0);   // executed since last loaded
    std::vector<char> ever(0x10000, 0);
    for (std::size_t i = 0; i < program.stage1.size(); ++i) owner[fl::kStage1Address + i] = kStage1Owner;

    std::vector<Copy> copies;
    std::vector<int> run_of;                 // the run each copy belongs to
    int run = -1;
    struct Pending {
        bool valid = false;
        std::size_t cart = 0;
        std::uint8_t value = 0;
    } pending;

    m6502::Cpu cpu(bus, true, m6502::Variant::Cmos);
    cpu.regs.pc = fl::kStage1Address;
    cpu.regs.s = 0xFF;
    cpu.regs.y = 2;
    std::uint64_t idle = 0;
    std::vector<std::uint32_t> entries;
    out.ended = "limit";

    for (std::uint64_t step = 0; step < kBootSteps; ++step) {
        const std::uint16_t pc = cpu.regs.pc;
        if (pc >= kShift) {
            if (pc != kShift) {
                out.ended = "rom";
                break;
            }
            // the shift routine: the cartridge address becomes block A, and it returns
            position = static_cast<std::size_t>(cpu.regs.a) * block_size;
            const std::uint16_t lo = bus.memory()[0x100 | static_cast<std::uint8_t>(cpu.regs.s + 1)];
            const std::uint16_t hi = bus.memory()[0x100 | static_cast<std::uint8_t>(cpu.regs.s + 2)];
            cpu.regs.s = static_cast<std::uint8_t>(cpu.regs.s + 2);
            cpu.regs.pc = static_cast<std::uint16_t>(((hi << 8) | lo) + 1);
            cpu.regs.a = 0;
            cpu.regs.x = 0;
            cpu.regs.y = 2;
            continue;
        }
        const std::optional<dec::Insn> insn = dec::decode(bus.memory(), pc, pc, dec::Variant::Cmos);
        if (insn && insn->mnemonic == "STA" && pending.valid && cpu.regs.a == pending.value) {
            if (const auto at = store_address(*insn, cpu.regs, bus.memory())) {
                const bool continues = !copies.empty() && copies.back().cart + 1 == pending.cart && copies.back().address + 1 == *at;
                if (!continues) ++run;
                copies.push_back({static_cast<std::uint32_t>(pending.cart), *at});
                run_of.push_back(run);
                owner[*at] = run;
                running[*at] = 0;
                pending.valid = false;
            }
        }
        ever[pc] = 1;
        running[pc] = 1;
        read_now = false;
        if (const auto fault = cpu.step()) {
            out.ended = "fault: " + fault->text();
            break;
        }
        ++out.steps;
        if (read_now) {
            pending = {true, last_read, last_value};
            idle = 0;
        } else if (!copies.empty() && ++idle > kIdleSteps) {
            out.ended = "running";
            break;
        }
        // a jump out of one loaded stretch into another that has not run yet: an entry
        const std::uint16_t next = cpu.regs.pc;
        if (insn && next != static_cast<std::uint16_t>(pc + insn->size) && next < kShift && owner[next] >= 0 && owner[next] != owner[pc] && !running[next]) {
            if (std::find(entries.begin(), entries.end(), next) == entries.end()) entries.push_back(next);
        }
    }
    out.steps = cpu.steps;

    // the runs of copies that are contiguous in both the cartridge and RAM are the segments
    std::vector<fl::Segment> found;
    for (std::size_t i = 0; i < copies.size();) {
        std::size_t j = i + 1;
        while (j < copies.size() && run_of[j] == run_of[i]) ++j;
        found.push_back({static_cast<std::uint32_t>(fl::kHeaderSize + copies[i].cart), copies[i].address, static_cast<std::uint32_t>(j - i)});
        i = j;
    }
    // keep what is a program: not a few bytes read into page zero (a directory entry, a counter) that never ran
    std::vector<fl::Segment> kept;
    std::sort(found.begin(), found.end(), [](const fl::Segment& a, const fl::Segment& b) { return a.offset < b.offset; });
    for (const fl::Segment& s : found) {
        bool executed = false;
        for (std::uint32_t a = s.address; a < s.address + s.size && !executed; ++a) executed = ever[a & 0xFFFF] != 0;
        if (!executed && (s.address < 0x100 || s.size < 4)) continue;
        if (!kept.empty() && s.offset < kept.back().offset + kept.back().size) continue;   // the cart's bytes loaded twice: the first stays
        kept.push_back(s);
    }
    out.segments = kept;
    const auto loaded = [&](std::uint32_t address) {
        return std::any_of(kept.begin(), kept.end(), [&](const fl::Segment& s) { return address >= s.address && address < s.address + s.size; });
    };
    for (const std::uint32_t e : entries) {
        if (loaded(e)) out.entries.push_back(e);
    }
    for (const std::uint16_t vector : {std::uint16_t{0x0314}, std::uint16_t{0x0318}}) {   // the IRQ and NMI vectors the loaded program may have set
        const std::uint32_t at = bus.memory()[vector] | (bus.memory()[vector + 1] << 8);
        if (loaded(at) && std::find(out.entries.begin(), out.entries.end(), at) == out.entries.end()) out.entries.push_back(at);
    }
    return out;
}

}  // namespace cb::emulate::lynx
