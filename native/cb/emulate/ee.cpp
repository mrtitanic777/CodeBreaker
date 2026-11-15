// cb-unity: alone (file-local names that other files in a batch also use)
#include "cb/emulate/ee.hpp"

#include <algorithm>
#include <format>

#include "cb/formats/ps2.hpp"

namespace cb::emulate::ee {

namespace {

constexpr std::uint32_t kIopRam = 0x1C000000, kIopRamEnd = 0x1C1FFFFF;
constexpr std::uint32_t kBios = 0x1FC00000;

bool is_register(std::uint32_t p) { return (p >= 0x10000000 && p < 0x10010000) || (p >= 0x12000000 && p < 0x12002000); }

}  // namespace

std::string BusAccess::text() const { return std::format("{}{} {}", kind == Kind::Read ? 'r' : 'w', size, name); }

std::string Syscall::text() const { return std::format("{}({:x},{:x},{:x},{:x})", name, args[0], args[1], args[2], args[3]); }

std::optional<std::uint32_t> Bus::physical(std::uint32_t a) {
    if (a >= 0x80000000u && a < 0xC0000000u) return a & 0x1FFFFFFFu;   // KSEG0 and KSEG1
    if (a >= 0xC0000000u) return std::nullopt;
    if (a >= 0x20000000u && a < 0x22000000u) return a - 0x20000000u;   // uncached RAM
    if (a >= 0x30100000u && a < 0x32000000u) return a - 0x30000000u;   // uncached accelerated RAM
    return a;
}

bool Bus::read(std::uint32_t address, unsigned size, std::uint64_t& value) {
    if (address >= kScratchpad && address - kScratchpad + size <= kScratchpadSize) {
        value = 0;
        for (unsigned i = 0; i < size; ++i) value |= static_cast<std::uint64_t>(scratch_[address - kScratchpad + i]) << (8 * i);
        return true;
    }
    const std::optional<std::uint32_t> p = physical(address);
    if (!p) return false;
    if (*p < kRamSize || memory_.covers(address)) {
        value = memory_.read(*p < kRamSize ? *p : address, size);
        return true;
    }
    if (is_register(*p)) {
        std::uint64_t v = 0;
        const std::uint32_t key = *p & ~7u;
        for (const auto& [a, x] : latched_)
            if (a == key) v = x;
        v >>= 8 * (*p & 7);
        const std::string name = formats::ps2::hardware_name(*p).value_or("register");
        if (name == "GS_CSR") v |= 0x0A;                                       // vertical blank and finish
        else if (name.size() > 5 && name.starts_with("D") && name.ends_with("_CHCR")) v &= ~0x100u;   // the transfer is done
        else if (name.ends_with("_COUNT")) v = ++timer_;
        if (size < 8) v &= (std::uint64_t{1} << (8 * size)) - 1;
        log_.push_back({BusAccess::Kind::Read, static_cast<std::uint8_t>(size), *p, v, name});
        value = v;
        return true;
    }
    if (*p >= kIopRam && *p <= kIopRamEnd) {
        value = memory_.read(address, size);
        return true;
    }
    if (*p >= kBios) {
        value = 0;
        return true;
    }
    return false;
}

bool Bus::write(std::uint32_t address, unsigned size, std::uint64_t value) {
    if (address >= kScratchpad && address - kScratchpad + size <= kScratchpadSize) {
        for (unsigned i = 0; i < size; ++i) scratch_[address - kScratchpad + i] = static_cast<std::uint8_t>(value >> (8 * i));
        return true;
    }
    const std::optional<std::uint32_t> p = physical(address);
    if (!p) return false;
    if (*p < kRamSize) {
        memory_.write(*p, size, value);
        return true;
    }
    if (memory_.covers(address)) {
        memory_.write(address, size, value);
        return true;
    }
    if (is_register(*p)) {
        const std::string name = formats::ps2::hardware_name(*p).value_or("register");
        log_.push_back({BusAccess::Kind::Write, static_cast<std::uint8_t>(size), *p, size < 8 ? value & ((std::uint64_t{1} << (8 * size)) - 1) : value, name});
        const std::uint32_t key = *p & ~7u;
        const unsigned shift = 8 * (*p & 7);
        const std::uint64_t mask = (size >= 8 ? ~std::uint64_t{0} : ((std::uint64_t{1} << (8 * size)) - 1)) << shift;
        for (auto& [a, x] : latched_)
            if (a == key) {
                x = (x & ~mask) | ((value << shift) & mask);
                return true;
            }
        latched_.emplace_back(key, (value << shift) & mask);
        return true;
    }
    if (*p >= kIopRam && *p <= kIopRamEnd) {
        memory_.write(address, size, value);
        return true;
    }
    return *p >= kBios;   // (writes to the BIOS ROM go nowhere)
}

bool Bus::syscall(mipsle::Cpu& cpu, std::uint32_t, std::string& stopped) {
    Syscall call;
    call.number = static_cast<std::int32_t>(static_cast<std::uint32_t>(cpu.r[3]));
    call.name = formats::ps2::syscall_name(call.number);
    for (std::size_t i = 0; i < 4; ++i) call.args[i] = static_cast<std::uint32_t>(cpu.r[4 + i]);
    syscalls_.push_back(call);
    switch (call.number) {
        case 0x04: case 0x06: case 0x07: case 0x23: case 0x24:   // Exit LoadExecPS2 ExecPS2 ExitThread ExitDeleteThread
            stopped = call.name;
            return false;
        case 0x7F: cpu.r[2] = kRamSize; break;                      // GetMemorySize
        case 0x3C: cpu.r[2] = static_cast<std::uint64_t>(static_cast<std::int64_t>(static_cast<std::int32_t>(call.args[1]))); break;   // SetupThread: the stack it was given
        case 0x3D: cpu.r[2] = static_cast<std::uint64_t>(static_cast<std::int64_t>(static_cast<std::int32_t>(call.args[0]))); break;   // SetupHeap
        default: cpu.r[2] = 0; break;
    }
    return true;
}

std::string RunResult::exit() const {
    switch (end) {
        case End::Return: return "return";
        case End::Stopped: return "stopped: " + (fault ? fault->text() : std::string("stopped"));
        case End::Limit: return "limit";
    }
    return "limit";
}

RunResult run(Bus& bus, std::uint32_t entry, const RunOptions& options) {
    mipsle::Cpu cpu(mipsle::Model::R5900, bus);
    const auto sx = [](std::uint32_t v) { return static_cast<std::uint64_t>(static_cast<std::int64_t>(static_cast<std::int32_t>(v))); };
    for (std::size_t i = 0; i < 4; ++i) cpu.r[4 + i] = sx(options.a[i]);
    for (std::size_t i = 0; i < 8; ++i) {
        cpu.r[16 + i] = sx(options.s[i]);
        cpu.r[8 + i] = sx(options.t[i]);
    }
    cpu.r[28] = sx(options.gp);
    cpu.r[29] = sx(options.sp);
    cpu.r[31] = sx(kReturnAddress);
    cpu.cop0[12] = 0x70000000;   // Status as the kernel leaves it for a program: the coprocessors usable
    cpu.jump_to(entry);
    const std::size_t io_before = bus.log().size(), calls_before = bus.syscalls().size();
    RunResult result;
    while (true) {
        if (cpu.pc == kReturnAddress) {
            result.end = End::Return;
            break;
        }
        if (cpu.steps >= options.max_steps) {
            result.end = End::Limit;
            break;
        }
        if (std::optional<mipsle::Fault> fault = cpu.step()) {
            result.end = End::Stopped;
            result.fault = std::move(fault);
            break;
        }
    }
    result.r = cpu.r;
    result.rh = cpu.rh;
    result.hi = cpu.hi;
    result.lo = cpu.lo;
    result.f0 = cpu.fpr[0];
    result.steps = cpu.steps;
    result.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(io_before), bus.log().end());
    result.syscalls.assign(bus.syscalls().begin() + static_cast<std::ptrdiff_t>(calls_before), bus.syscalls().end());
    return result;
}

}  // namespace cb::emulate::ee
