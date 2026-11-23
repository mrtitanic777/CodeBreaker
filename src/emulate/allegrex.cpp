// cb-unity: alone (file-local names that other files in a batch also use)
#include "emulate/allegrex.hpp"

#include <algorithm>
#include <format>

#include "formats/psp.hpp"

namespace cb::emulate::allegrex {

namespace {

bool ends_the_program(const std::string& name) {
    return name == "sceKernelExitGame" || name == "sceKernelExitGameWithStatus" || name == "sceKernelExitThread" || name == "sceKernelExitDeleteThread" ||
           name == "sceKernelSelfStopUnloadModule" || name == "sceKernelStopUnloadSelfModule";
}

}  // namespace

std::string BusAccess::text() const { return std::format("{}{} {}", kind == Kind::Read ? 'r' : 'w', size, name); }

std::string Call::text() const { return std::format("{}({:x},{:x},{:x},{:x})", name, args[0], args[1], args[2], args[3]); }

Bus::Bus(paged_memory::Regions regions, BusOptions options) : memory_(std::move(regions)), options_(std::move(options)) {
    for (const auto& [address, name] : options_.imports) {
        stub_low_ = std::min(stub_low_, address);
        stub_high_ = std::max(stub_high_, address);
    }
}

std::optional<std::uint32_t> Bus::physical(std::uint32_t a) {
    if (a >= 0x80000000u && a < 0xC0000000u) return a & 0x1FFFFFFFu;   // the kernel's views
    if (a >= 0xC0000000u) return std::nullopt;
    if (a >= 0x40000000u && a < 0x60000000u) return a - 0x40000000u;   // uncached
    return a;
}

bool Bus::read(std::uint32_t address, unsigned size, std::uint64_t& value) {
    const std::optional<std::uint32_t> p = physical(address);
    if (!p) return false;
    if (options_.scratchpad && *p >= kScratchpad && *p - kScratchpad + size <= kScratchpadSize) {
        value = 0;
        for (unsigned i = 0; i < size; ++i) value |= static_cast<std::uint64_t>(scratch_[*p - kScratchpad + i]) << (8 * i);
        return true;
    }
    if ((*p >= kRamBase && *p < kRamBase + kRamSize) || (*p >= kVram && *p < kVram + kVramSize) || memory_.covers(*p)) {
        value = memory_.read(*p, size);
        return true;
    }
    if (*p >= 0x1C000000u && *p < 0x20000000u) {
        std::uint32_t v = 0;
        for (const auto& [a, x] : latched_)
            if (a == (*p & ~3u)) v = x;
        v >>= 8 * (*p & 3);
        if (size < 4) v &= (1u << (8 * size)) - 1;
        log_.push_back({BusAccess::Kind::Read, static_cast<std::uint8_t>(size), *p, v, formats::psp::hardware_name(*p).value_or("register")});
        value = v;
        return true;
    }
    return false;
}

bool Bus::peek(std::uint32_t address, unsigned size, std::uint64_t& value) const {
    const std::optional<std::uint32_t> p = physical(address);
    if (!p) return false;
    if (options_.scratchpad && *p >= kScratchpad && *p - kScratchpad + size <= kScratchpadSize) {
        value = 0;
        for (unsigned i = 0; i < size; ++i) value |= static_cast<std::uint64_t>(scratch_[*p - kScratchpad + i]) << (8 * i);
        return true;
    }
    if ((*p >= kRamBase && *p < kRamBase + kRamSize) || (*p >= kVram && *p < kVram + kVramSize) || memory_.covers(*p)) {
        value = memory_.read(*p, size);
        return true;
    }
    return false;
}

bool Bus::write(std::uint32_t address, unsigned size, std::uint64_t value) {
    const std::optional<std::uint32_t> p = physical(address);
    if (!p) return false;
    if (options_.scratchpad && *p >= kScratchpad && *p - kScratchpad + size <= kScratchpadSize) {
        for (unsigned i = 0; i < size; ++i) scratch_[*p - kScratchpad + i] = static_cast<std::uint8_t>(value >> (8 * i));
        return true;
    }
    if ((*p >= kRamBase && *p < kRamBase + kRamSize) || (*p >= kVram && *p < kVram + kVramSize)) {
        memory_.write(*p, size, value);
        return true;
    }
    if (memory_.covers(*p)) {
        memory_.write(*p, size, value);
        return true;
    }
    if (*p >= 0x1C000000u && *p < 0x20000000u) {
        const std::uint32_t v = static_cast<std::uint32_t>(size < 4 ? value & ((1u << (8 * size)) - 1) : value);
        log_.push_back({BusAccess::Kind::Write, static_cast<std::uint8_t>(size), *p, v, formats::psp::hardware_name(*p).value_or("register")});
        const std::uint32_t key = *p & ~3u;
        const unsigned shift = 8 * (*p & 3);
        const std::uint32_t mask = (size >= 4 ? ~0u : ((1u << (8 * size)) - 1)) << shift;
        for (auto& [a, x] : latched_)
            if (a == key) {
                x = (x & ~mask) | ((v << shift) & mask);
                return true;
            }
        latched_.emplace_back(key, (v << shift) & mask);
        return true;
    }
    return false;
}

bool Bus::syscall(mipsle::Cpu& cpu, std::uint32_t code, std::string&) {
    Call call;
    call.name = std::format("syscall {}", code);
    for (std::size_t i = 0; i < 4; ++i) call.args[i] = static_cast<std::uint32_t>(cpu.r[4 + i]);
    calls_.push_back(call);
    cpu.r[2] = 0;
    return true;
}

mipsle::Port::Intercept Bus::intercept(mipsle::Cpu& cpu, std::string& stopped) {
    if (options_.imports.empty() || cpu.pc < stub_low_ || cpu.pc > stub_high_) return Intercept::None;
    const auto it = options_.imports.find(cpu.pc);
    if (it == options_.imports.end()) return Intercept::None;
    Call call;
    call.name = it->second;
    for (std::size_t i = 0; i < 4; ++i) call.args[i] = static_cast<std::uint32_t>(cpu.r[4 + i]);
    calls_.push_back(call);
    if (ends_the_program(call.name)) {
        stopped = call.name;
        return Intercept::Stop;
    }
    cpu.r[2] = 0;
    const std::uint32_t back = static_cast<std::uint32_t>(cpu.r[31]);
    cpu.pc = back;
    cpu.next_pc = back + 4;
    return Intercept::Handled;
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
    mipsle::Cpu cpu(mipsle::Model::Allegrex, bus);
    const auto sx = [](std::uint32_t v) { return static_cast<std::uint64_t>(static_cast<std::int64_t>(static_cast<std::int32_t>(v))); };
    for (std::size_t i = 0; i < 4; ++i) cpu.r[4 + i] = sx(options.a[i]);
    for (std::size_t i = 0; i < 8; ++i) {
        cpu.r[16 + i] = sx(options.s[i]);
        cpu.r[8 + i] = sx(options.t[i]);
    }
    cpu.r[28] = sx(options.gp);
    cpu.r[29] = sx(options.sp);
    cpu.r[31] = sx(kReturnAddress);
    cpu.jump_to(entry);
    const std::size_t io_before = bus.log().size(), calls_before = bus.calls().size();
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
    result.hi = cpu.hi;
    result.lo = cpu.lo;
    result.f0 = cpu.fpr[0];
    result.steps = cpu.steps;
    result.io.assign(bus.log().begin() + static_cast<std::ptrdiff_t>(io_before), bus.log().end());
    result.calls.assign(bus.calls().begin() + static_cast<std::ptrdiff_t>(calls_before), bus.calls().end());
    return result;
}

}  // namespace cb::emulate::allegrex
