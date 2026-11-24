// cb-unity: alone
// emulate/a64.cpp -- the AArch64 interpreter (emulate/a64.hpp).
#include "emulate/a64.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>
#include <format>
#include <limits>

#include "decompile/a64.hpp"

namespace cb::emulate::a64 {

namespace da = decompile::a64;

namespace detail {
std::optional<std::string> run_simd(Registers& R, Memory& mem, const da::Insn& insn);
}  // namespace detail

using u64 = std::uint64_t;
using i64 = std::int64_t;
using u32 = std::uint32_t;

// ---- memory ---------------------------------------------------------------------------------------------------------------------------------------------------

Memory::Memory(Regions regions) : shared_(std::move(regions)) {
    if (shared_)
        for (const Region& r : *shared_) ranges_.push_back({r.address, r.address + r.bytes.size(), r.writable});
}

const Memory::Range* Memory::find(u64 address) const {
    for (const Range& r : ranges_)
        if (address >= r.lo && address < r.hi) return &r;
    return nullptr;
}

void Memory::map_writable(u64 address, u64 size) { ranges_.push_back({address, address + size, true}); }
void Memory::map_zero(u64 address, u64 size) { ranges_.push_back({address, address + size, false}); }

std::uint8_t Memory::base_byte(u64 address) const {
    if (shared_) {
        for (const Region& r : *shared_)
            if (address >= r.address && address - r.address < r.bytes.size()) return r.bytes[static_cast<std::size_t>(address - r.address)];
    }
    return 0;
}

std::uint8_t Memory::byte(u64 address) const {
    const auto it = pages_.find(address >> 12);
    if (it != pages_.end()) return (*it->second)[address & 0xFFF];
    return base_byte(address);
}

Memory::Page& Memory::page_for_write(u64 address) {
    std::unique_ptr<Page>& p = pages_[address >> 12];
    if (!p) {
        p = std::make_unique<Page>();
        const u64 base = address & ~0xFFFull;
        for (unsigned i = 0; i < 4096; ++i) (*p)[i] = base_byte(base + i);
    }
    return *p;
}

bool Memory::mapped(u64 address) const { return find(address) != nullptr; }
bool Memory::writable(u64 address) const {
    const Range* r = find(address);
    return r && r->writable;
}

bool Memory::read_bytes(u64 address, std::size_t n, std::uint8_t* out) const {
    for (std::size_t i = 0; i < n; ++i) {
        if (!find(address + i)) return false;
        out[i] = byte(address + i);
    }
    return true;
}

bool Memory::write_bytes(u64 address, const std::uint8_t* data, std::size_t n) {
    for (std::size_t i = 0; i < n; ++i)
        if (!writable(address + i)) return false;
    for (std::size_t i = 0; i < n; ++i) page_for_write(address + i)[(address + i) & 0xFFF] = data[i];
    return true;
}

bool Memory::read(u64 address, unsigned size, u64& value) const {
    std::uint8_t b[8] = {};
    if (!read_bytes(address, size, b)) return false;
    value = 0;
    for (unsigned i = 0; i < size; ++i) value |= static_cast<u64>(b[i]) << (8 * i);
    return true;
}

bool Memory::write(u64 address, unsigned size, u64 value) {
    std::uint8_t b[8];
    for (unsigned i = 0; i < size; ++i) b[i] = static_cast<std::uint8_t>(value >> (8 * i));
    return write_bytes(address, b, size);
}

std::vector<std::pair<u64, std::uint8_t>> Memory::changes(std::size_t limit) const {
    std::vector<std::pair<u64, std::uint8_t>> out;
    for (const auto& [page, data] : pages_) {
        for (unsigned i = 0; i < 4096 && out.size() < limit; ++i) {
            const u64 a = (page << 12) + i;
            if ((*data)[i] != base_byte(a)) out.emplace_back(a, (*data)[i]);
        }
        if (out.size() >= limit) break;
    }
    return out;
}

// ---- the kernel's calls ---------------------------------------------------------------------------------------------------------------------------------------

std::string_view svc_name(std::uint32_t n) {
    static const std::map<std::uint32_t, const char*> names = {
        {0x01, "SetHeapSize"}, {0x02, "SetMemoryPermission"}, {0x03, "SetMemoryAttribute"}, {0x04, "MapMemory"}, {0x05, "UnmapMemory"}, {0x06, "QueryMemory"},
        {0x07, "ExitProcess"}, {0x08, "CreateThread"}, {0x09, "StartThread"}, {0x0A, "ExitThread"}, {0x0B, "SleepThread"}, {0x0C, "GetThreadPriority"},
        {0x0D, "SetThreadPriority"}, {0x0E, "GetThreadCoreMask"}, {0x0F, "SetThreadCoreMask"}, {0x10, "GetCurrentProcessorNumber"}, {0x11, "SignalEvent"},
        {0x12, "ClearEvent"}, {0x13, "MapSharedMemory"}, {0x14, "UnmapSharedMemory"}, {0x15, "CreateTransferMemory"}, {0x16, "CloseHandle"}, {0x17, "ResetSignal"},
        {0x18, "WaitSynchronization"}, {0x19, "CancelSynchronization"}, {0x1A, "ArbitrateLock"}, {0x1B, "ArbitrateUnlock"}, {0x1C, "WaitProcessWideKeyAtomic"},
        {0x1D, "SignalProcessWideKey"}, {0x1E, "GetSystemTick"}, {0x1F, "ConnectToNamedPort"}, {0x20, "SendSyncRequestLight"}, {0x21, "SendSyncRequest"},
        {0x22, "SendSyncRequestWithUserBuffer"}, {0x23, "SendAsyncRequestWithUserBuffer"}, {0x24, "GetProcessId"}, {0x25, "GetThreadId"}, {0x26, "Break"},
        {0x27, "OutputDebugString"}, {0x28, "ReturnFromException"}, {0x29, "GetInfo"}, {0x2A, "FlushEntireDataCache"}, {0x2B, "FlushDataCache"}, {0x2C, "MapPhysicalMemory"},
        {0x2D, "UnmapPhysicalMemory"}, {0x2E, "GetFutureThreadInfo"}, {0x2F, "GetLastThreadInfo"}, {0x30, "GetResourceLimitLimitValue"},
        {0x31, "GetResourceLimitCurrentValue"}, {0x32, "SetThreadActivity"}, {0x33, "GetThreadContext3"}, {0x34, "WaitForAddress"}, {0x35, "SignalToAddress"},
        {0x36, "SynchronizePreemptionState"}, {0x37, "GetResourceLimitPeakValue"}, {0x40, "CreateSession"}, {0x41, "AcceptSession"}, {0x42, "ReplyAndReceiveLight"},
        {0x43, "ReplyAndReceive"}, {0x44, "ReplyAndReceiveWithUserBuffer"}, {0x45, "CreateEvent"}, {0x4B, "CreateCodeMemory"}, {0x4C, "ControlCodeMemory"},
        {0x4D, "SleepSystem"}, {0x4E, "ReadWriteRegister"}, {0x4F, "SetProcessActivity"}, {0x50, "CreateSharedMemory"}, {0x51, "MapTransferMemory"},
        {0x52, "UnmapTransferMemory"}, {0x53, "CreateInterruptEvent"}, {0x54, "QueryPhysicalAddress"}, {0x55, "QueryIoMapping"}, {0x56, "CreateDeviceAddressSpace"},
        {0x57, "AttachDeviceAddressSpace"}, {0x58, "DetachDeviceAddressSpace"}, {0x59, "MapDeviceAddressSpaceByForce"}, {0x5A, "MapDeviceAddressSpaceAligned"},
        {0x5C, "UnmapDeviceAddressSpace"}, {0x5D, "InvalidateProcessDataCache"}, {0x5E, "StoreProcessDataCache"}, {0x5F, "FlushProcessDataCache"},
        {0x60, "DebugActiveProcess"}, {0x61, "BreakDebugProcess"}, {0x62, "TerminateDebugProcess"}, {0x63, "GetDebugEvent"}, {0x64, "ContinueDebugEvent"},
        {0x65, "GetProcessList"}, {0x66, "GetThreadList"}, {0x67, "GetDebugThreadContext"}, {0x68, "SetDebugThreadContext"}, {0x69, "QueryDebugProcessMemory"},
        {0x6A, "ReadDebugProcessMemory"}, {0x6B, "WriteDebugProcessMemory"}, {0x6C, "SetHardwareBreakPoint"}, {0x6D, "GetDebugThreadParam"}, {0x6F, "GetSystemInfo"},
        {0x70, "CreatePort"}, {0x71, "ManageNamedPort"}, {0x72, "ConnectToPort"}, {0x73, "SetProcessMemoryPermission"}, {0x74, "MapProcessMemory"},
        {0x75, "UnmapProcessMemory"}, {0x76, "QueryProcessMemory"}, {0x77, "MapProcessCodeMemory"}, {0x78, "UnmapProcessCodeMemory"}, {0x79, "CreateProcess"},
        {0x7A, "StartProcess"}, {0x7B, "TerminateProcess"}, {0x7C, "GetProcessInfo"}, {0x7D, "CreateResourceLimit"}, {0x7E, "SetResourceLimitLimitValue"},
        {0x7F, "CallSecureMonitor"}};
    const auto it = names.find(n);
    return it == names.end() ? std::string_view() : std::string_view(it->second);
}

std::string describe_ipc(Cpu& cpu) {
    const u64 tls = cpu.regs.tpidrro_el0;
    if (tls == 0) return {};
    u64 h0 = 0, h1 = 0;
    if (!cpu.memory().read(tls, 4, h0) || !cpu.memory().read(tls + 4, 4, h1)) return {};
    const unsigned type = static_cast<unsigned>(h0 & 0xFFFF);
    if (type != 4 && type != 6 && type != 0 && type != 2 && type != 1 && type != 5) return {};
    const unsigned x = (h0 >> 16) & 15, a = (h0 >> 20) & 15, b = (h0 >> 24) & 15, w = (h0 >> 28) & 15;
    std::size_t at = 8;
    if (h1 & 0x80000000u) {
        u64 hd = 0;
        if (!cpu.memory().read(tls + at, 4, hd)) return {};
        at += 4;
        if (hd & 1) at += 8;                    // process id
        at += 4 * ((hd >> 1) & 15);             // copy handles
        at += 4 * ((hd >> 5) & 15);             // move handles
    }
    at += 8 * x + 12 * (a + b + w);
    at = (at + 15) & ~std::size_t{15};
    u64 magic = 0, cmd = 0;
    if (!cpu.memory().read(tls + at, 4, magic) || !cpu.memory().read(tls + at + 8, 4, cmd)) return {};
    if (type == 2) return "ipc close";
    if (magic != 0x49434653u) return std::format("ipc type {}", type);
    std::string out = std::format("ipc request cmd {}", cmd);
    if (cmd == 1 && ((h1 & 0x3FF) >= 6)) {   // sm: GetService takes a name of 8 characters
        std::uint8_t name[8] = {};
        if (cpu.memory().read_bytes(tls + at + 16, 8, name)) {
            std::string n;
            for (const std::uint8_t c : name)
                if (c >= 32 && c < 127) n += static_cast<char>(c);
            if (!n.empty()) out += " '" + n + "'";
        }
    }
    return out;
}

bool horizon_svc(Cpu& cpu, SvcCall& call) {
    Registers& r = cpu.regs;
    r.x[0] = 0;
    switch (call.number) {
        case 0x07:
        case 0x0A:
        case 0x26: return false;
        case 0x01: {
            const u64 size = std::max<u64>(call.args[1], 0x200000);
            cpu.memory().map_writable(kHeapBase, (size + 0x1FFFFF) & ~u64{0x1FFFFF});
            r.x[1] = kHeapBase;
            break;
        }
        case 0x1E: {
            cpu.counter += 1000;
            r.x[0] = cpu.counter;
            break;
        }
        case 0x21: call.note = describe_ipc(cpu); break;
        case 0x1F: {
            std::uint8_t name[12] = {};
            if (cpu.memory().read_bytes(call.args[1], 11, name)) {
                std::string n;
                for (const std::uint8_t c : name)
                    if (c >= 32 && c < 127) n += static_cast<char>(c);
                call.note = "port '" + n + "'";
            }
            r.x[1] = 0xD0000001;
            break;
        }
        case 0x08:
        case 0x15:
        case 0x45:
        case 0x50: r.x[1] = 0xD0000002; break;
        case 0x18: r.x[1] = 0; break;
        case 0x29: r.x[1] = 0; break;
        default: break;
    }
    return true;
}

std::string RunResult::exit() const {
    switch (end) {
        case End::Return: return "return";
        case End::Stopped: return "stopped: " + reason;
        default: return "limit";
    }
}

// ---- helpers ----------------------------------------------------------------------------------------------------------------------------------------------------

namespace {

u64 sx(u64 v, unsigned width) {
    if (width >= 64) return v;
    const u64 m = 1ull << (width - 1);
    return ((v & ((1ull << width) - 1)) ^ m) - m;
}

u64 mask_of(bool is64) { return is64 ? ~0ull : 0xFFFFFFFFull; }

u64 ror_width(u64 v, unsigned r, unsigned width) {
    r %= width;
    if (r == 0) return v;
    const u64 m = width == 64 ? ~0ull : (1ull << width) - 1;
    return ((v >> r) | (v << (width - r))) & m;
}

u64 shift_value(u64 v, unsigned type, unsigned amount, bool is64) {
    const unsigned width = is64 ? 64 : 32;
    v &= mask_of(is64);
    switch (type) {
        case 0: return amount >= width ? 0 : (v << amount) & mask_of(is64);
        case 1: return amount >= width ? 0 : v >> amount;
        case 2: return static_cast<u64>(static_cast<i64>(sx(v, width)) >> std::min(amount, width - 1)) & mask_of(is64);
        default: return ror_width(v, amount, width);
    }
}

struct Sum {
    u64 result;
    u32 nzcv;
};

Sum add_with_carry(u64 x, u64 y, unsigned carry, bool is64) {
    const unsigned width = is64 ? 64 : 32;
    x &= mask_of(is64);
    y &= mask_of(is64);
    u64 r;
    bool c;
    if (is64) {
        const u64 s1 = x + y;
        const bool c1 = s1 < x;
        r = s1 + carry;
        c = c1 || r < s1;
    } else {
        const u64 s = x + y + carry;
        r = s & 0xFFFFFFFFull;
        c = (s >> 32) & 1;
    }
    const bool n = (r >> (width - 1)) & 1;
    const bool z = r == 0;
    const bool v = (((x ^ r) & (y ^ r)) >> (width - 1)) & 1;
    return {r, (n ? 8u : 0u) << 28 | (z ? 4u : 0u) << 28 | (c ? 2u : 0u) << 28 | (v ? 1u : 0u) << 28};
}

bool condition_holds(u32 nzcv, unsigned cond) {
    const bool n = nzcv >> 31 & 1, z = nzcv >> 30 & 1, c = nzcv >> 29 & 1, v = nzcv >> 28 & 1;
    bool r = false;
    switch (cond >> 1) {
        case 0: r = z; break;
        case 1: r = c; break;
        case 2: r = n; break;
        case 3: r = v; break;
        case 4: r = c && !z; break;
        case 5: r = n == v; break;
        case 6: r = n == v && !z; break;
        default: r = true; break;
    }
    return (cond & 1) && cond != 15 ? !r : r;
}

u64 umulh(u64 a, u64 b) {
    const u64 a0 = a & 0xFFFFFFFFull, a1 = a >> 32, b0 = b & 0xFFFFFFFFull, b1 = b >> 32;
    const u64 p00 = a0 * b0, p01 = a0 * b1, p10 = a1 * b0, p11 = a1 * b1;
    const u64 mid = (p00 >> 32) + (p01 & 0xFFFFFFFFull) + (p10 & 0xFFFFFFFFull);
    return p11 + (p01 >> 32) + (p10 >> 32) + (mid >> 32);
}

u64 reverse_bits(u64 v, unsigned width) {
    u64 r = 0;
    for (unsigned i = 0; i < width; ++i) r |= ((v >> i) & 1) << (width - 1 - i);
    return r;
}

u64 reverse_bytes_in(u64 v, unsigned container_bits, unsigned width) {
    u64 r = 0;
    const unsigned bytes = container_bits / 8;
    for (unsigned base = 0; base < width / 8; base += bytes)
        for (unsigned i = 0; i < bytes; ++i) r |= ((v >> (8 * (base + i))) & 0xFF) << (8 * (base + bytes - 1 - i));
    return r;
}

std::string abort_text(const char* what, u64 address) { return std::format("data abort: {} at {:#x}", what, address); }

// ---- floating point ----

double to_double(u64 bits_, unsigned ftype) {
    if (ftype == 0) return std::bit_cast<float>(static_cast<u32>(bits_));
    return std::bit_cast<double>(bits_);
}
u64 from_double(double d, unsigned ftype) {
    if (ftype == 0) return std::bit_cast<u32>(static_cast<float>(d));
    return std::bit_cast<u64>(d);
}

float half_to_float(std::uint16_t h) {
    const unsigned sign = h >> 15, exp = (h >> 10) & 31, frac = h & 0x3FF;
    float v;
    if (exp == 0) v = std::ldexp(static_cast<float>(frac), -24);
    else if (exp == 31) v = frac ? std::numeric_limits<float>::quiet_NaN() : std::numeric_limits<float>::infinity();
    else v = std::ldexp(static_cast<float>(frac | 0x400), static_cast<int>(exp) - 25);
    return sign ? -v : v;
}

std::uint16_t float_to_half(double d) {
    if (std::isnan(d)) return 0x7E00;
    const unsigned sign = std::signbit(d) ? 0x8000u : 0u;
    d = std::fabs(d);
    if (std::isinf(d) || d >= 65520.0) return static_cast<std::uint16_t>(sign | 0x7C00);
    if (d < std::ldexp(1.0, -14)) {   // subnormal: a multiple of 2^-24
        const double q = std::nearbyint(std::ldexp(d, 24));
        return static_cast<std::uint16_t>(sign | static_cast<unsigned>(q));
    }
    int e;
    const double m = std::frexp(d, &e);   // d = m * 2^e, m in [0.5, 1)
    double frac = std::nearbyint(std::ldexp(m, 11));   // 11 significant bits, in [1024, 2048]
    if (frac >= 2048.0) {
        frac /= 2;
        ++e;
    }
    const int exp = e + 14;
    if (exp >= 31) return static_cast<std::uint16_t>(sign | 0x7C00);
    return static_cast<std::uint16_t>(sign | (static_cast<unsigned>(exp) << 10) | (static_cast<unsigned>(frac) & 0x3FF));
}

double round_mode(double d, unsigned mode) {
    switch (mode) {
        case 0: return std::nearbyint(d);   // to nearest, ties to even
        case 1: return std::ceil(d);
        case 2: return std::floor(d);
        case 3: return std::trunc(d);
        default: return std::round(d);      // to nearest, ties away
    }
}

// A floating-point value to an integer of `bits` bits, saturating; NaN is zero.
u64 to_integer(double d, unsigned mode, bool is_signed, unsigned bits_) {
    if (std::isnan(d)) return 0;
    const double r = round_mode(d, mode);
    if (is_signed) {
        const double lim = std::ldexp(1.0, static_cast<int>(bits_) - 1);
        if (r >= lim) return bits_ == 64 ? 0x7FFFFFFFFFFFFFFFull : 0x7FFFFFFFull;
        if (r < -lim) return bits_ == 64 ? 0x8000000000000000ull : 0x80000000ull;
        return static_cast<u64>(static_cast<i64>(r)) & mask_of(bits_ == 64);
    }
    const double lim = std::ldexp(1.0, static_cast<int>(bits_));
    if (r >= lim) return mask_of(bits_ == 64);
    if (r <= 0) return 0;
    return static_cast<u64>(r);
}

}  // namespace

// ---- the CPU ------------------------------------------------------------------------------------------------------------------------------------------------

std::optional<std::string> Cpu::step() {
    Registers& R = regs;
    std::uint8_t fetch[4];
    if (!mem_->read_bytes(R.pc, 4, fetch)) return std::format("instruction fetch from unmapped address {:#x}", R.pc);
    const u32 w = static_cast<u32>(fetch[0]) | static_cast<u32>(fetch[1]) << 8 | static_cast<u32>(fetch[2]) << 16 | static_cast<u32>(fetch[3]) << 24;
    const std::optional<da::Insn> decoded = da::decode_word(w, R.pc);
    if (!decoded) return std::format("undefined instruction {:08x} at {:#x}", w, R.pc);
    const da::Insn& insn = *decoded;
    ++steps;

    const auto bits = [&](unsigned pos, unsigned width) -> u32 { return (w >> pos) & (width >= 32 ? 0xFFFFFFFFu : ((1u << width) - 1)); };
    const bool sf = bits(31, 1) != 0;
    const unsigned rd = bits(0, 5), rn = bits(5, 5), rm = bits(16, 5), ra = bits(10, 5);
    const auto X = [&](unsigned n) -> u64 { return n == 31 ? 0 : R.x[n]; };
    const auto XSP = [&](unsigned n) -> u64 { return n == 31 ? R.sp : R.x[n]; };
    const auto setX = [&](unsigned n, u64 v, bool is64) {
        if (n != 31) R.x[n] = v & mask_of(is64);
    };
    const auto setXSP = [&](unsigned n, u64 v, bool is64) {
        if (n == 31) R.sp = v & mask_of(is64);
        else R.x[n] = v & mask_of(is64);
    };
    u64 next = R.pc + 4;
    const auto stop = [&](const std::string& why) -> std::optional<std::string> { return why; };
    const auto unsupported = [&](const char* what) { return stop(std::format("{}: {} ({}) at {:#x}", what, std::string(insn.mnemonic), da::text(insn), R.pc)); };

    // loads and stores share these
    const auto load = [&](u64 addr, unsigned size, u64& v) -> bool { return mem_->read(addr, size, v); };
    const auto fault_read = [&](u64 addr) { return stop(abort_text("read", addr)); };
    const auto fault_write = [&](u64 addr) { return stop(abort_text(mem_->mapped(addr) ? "write to read-only memory" : "write", addr)); };

    switch (insn.op) {
        case da::Op::Adr: {
            const i64 imm = static_cast<i64>(sx((bits(5, 19) << 2) | bits(29, 2), 21));
            setX(rd, R.pc + static_cast<u64>(imm), true);
            break;
        }
        case da::Op::Adrp: {
            const i64 imm = static_cast<i64>(sx((bits(5, 19) << 2) | bits(29, 2), 21));
            setX(rd, (R.pc & ~0xFFFull) + static_cast<u64>(imm * 4096), true);
            break;
        }
        case da::Op::AddSubImm: {
            const bool sub = bits(30, 1), set = bits(29, 1);
            const u64 imm = static_cast<u64>(bits(10, 12)) << (bits(22, 1) ? 12 : 0);
            const Sum s = add_with_carry(XSP(rn), sub ? ~imm : imm, sub ? 1 : 0, sf);
            if (set) {
                R.nzcv = s.nzcv;
                setX(rd, s.result, sf);
            } else {
                setXSP(rd, s.result, sf);
            }
            break;
        }
        case da::Op::AddSubShift: {
            const bool sub = bits(30, 1), set = bits(29, 1);
            const u64 op2 = shift_value(X(rm), bits(22, 2), bits(10, 6), sf);
            const Sum s = add_with_carry(X(rn), sub ? ~op2 : op2, sub ? 1 : 0, sf);
            if (set) R.nzcv = s.nzcv;
            setX(rd, s.result, sf);
            break;
        }
        case da::Op::AddSubExt: {
            const bool sub = bits(30, 1), set = bits(29, 1);
            const unsigned option = bits(13, 3), amount = bits(10, 3);
            u64 v = X(rm);
            switch (option & 3) {
                case 0: v = option & 4 ? sx(v, 8) : v & 0xFF; break;
                case 1: v = option & 4 ? sx(v, 16) : v & 0xFFFF; break;
                case 2: v = option & 4 ? sx(v, 32) : v & 0xFFFFFFFFull; break;
                default: break;
            }
            v <<= amount;
            const Sum s = add_with_carry(XSP(rn), sub ? ~v : v, sub ? 1 : 0, sf);
            if (set) {
                R.nzcv = s.nzcv;
                setX(rd, s.result, sf);
            } else {
                setXSP(rd, s.result, sf);
            }
            break;
        }
        case da::Op::AddSubCarry: {
            const bool sub = bits(30, 1), set = bits(29, 1);
            const u64 y = sub ? ~X(rm) : X(rm);
            const Sum s = add_with_carry(X(rn), y, (R.nzcv >> 29) & 1, sf);
            if (set) R.nzcv = s.nzcv;
            setX(rd, s.result, sf);
            break;
        }
        case da::Op::LogicImm:
        case da::Op::LogicShift: {
            const unsigned opc = bits(29, 2);
            u64 op2;
            if (insn.op == da::Op::LogicImm) {
                const std::optional<u64> m = da::decode_bitmask(bits(22, 1), bits(10, 6), bits(16, 6), sf ? 64 : 32);
                if (!m) return unsupported("bad bitmask");
                op2 = *m;
            } else {
                op2 = shift_value(X(rm), bits(22, 2), bits(10, 6), sf);
                if (bits(21, 1)) op2 = ~op2;
            }
            const u64 a = X(rn);
            u64 r = opc == 0 || opc == 3 ? (a & op2) : opc == 1 ? (a | op2) : (a ^ op2);
            r &= mask_of(sf);
            if (opc == 3) {
                R.nzcv = ((r >> (sf ? 63 : 31)) & 1 ? 8u << 28 : 0) | (r == 0 ? 4u << 28 : 0);
                setX(rd, r, sf);
            } else if (insn.op == da::Op::LogicImm) {
                setXSP(rd, r, sf);
            } else {
                setX(rd, r, sf);
            }
            break;
        }
        case da::Op::MoveWide: {
            const unsigned opc = bits(29, 2), shift = 16 * bits(21, 2);
            const u64 imm = static_cast<u64>(bits(5, 16)) << shift;
            if (opc == 0) setX(rd, ~imm, sf);
            else if (opc == 2) setX(rd, imm, sf);
            else setX(rd, (X(rd) & ~(0xFFFFull << shift)) | imm, sf);
            break;
        }
        case da::Op::Bitfield: {
            const unsigned opc = bits(29, 2), r = bits(16, 6), s = bits(10, 6), width = sf ? 64 : 32;
            const u64 src = X(rn) & mask_of(sf);
            const auto low = [](unsigned n) { return n >= 64 ? ~0ull : (1ull << n) - 1; };
            u64 field;
            unsigned sign_at;
            if (s >= r) {
                field = (src >> r) & low(s - r + 1);
                sign_at = s - r;
            } else {
                field = (src & low(s + 1)) << (width - r);
                sign_at = s + width - r;
            }
            if (opc == 0) {   // sbfm
                setX(rd, sx(field, sign_at + 1), sf);
            } else if (opc == 2) {   // ubfm
                setX(rd, field, sf);
            } else {   // bfm
                u64 dst = X(rd);
                u64 mask;
                if (s >= r) mask = low(s - r + 1);
                else mask = low(s + 1) << (width - r);
                dst = (dst & ~mask) | (field & mask);
                setX(rd, dst, sf);
            }
            break;
        }
        case da::Op::Extract: {
            const unsigned lsb = bits(10, 6);
            const u64 hi = X(rn), lo = X(rm);
            if (sf) setX(rd, lsb == 0 ? lo : (lo >> lsb) | (hi << (64 - lsb)), true);
            else setX(rd, ((hi << 32 | (lo & 0xFFFFFFFFull)) >> lsb) & 0xFFFFFFFFull, false);
            break;
        }
        case da::Op::CondCmp: {
            const unsigned cond = bits(12, 4);
            if (condition_holds(R.nzcv, cond)) {
                const bool sub = bits(30, 1);
                const u64 y = bits(11, 1) ? rm : X(rm);
                R.nzcv = add_with_carry(X(rn), sub ? ~y : y, sub ? 1 : 0, sf).nzcv;
            } else {
                R.nzcv = bits(0, 4) << 28;
            }
            break;
        }
        case da::Op::CondSel: {
            const bool o = bits(30, 1), o2 = bits(10, 1);
            u64 v;
            if (condition_holds(R.nzcv, bits(12, 4))) {
                v = X(rn);
            } else {
                v = X(rm);
                if (!o && o2) v += 1;
                else if (o && !o2) v = ~v;
                else if (o && o2) v = ~v + 1;
            }
            setX(rd, v, sf);
            break;
        }
        case da::Op::Dp3: {
            const unsigned op31 = bits(21, 3);
            const bool sub = bits(15, 1);
            const u64 a = X(ra);
            if (op31 == 0) {
                const u64 p = X(rn) * X(rm);
                setX(rd, sub ? a - p : a + p, sf);
            } else if (op31 == 1 || op31 == 5) {
                const u64 n = op31 == 1 ? sx(X(rn), 32) : X(rn) & 0xFFFFFFFFull, m = op31 == 1 ? sx(X(rm), 32) : X(rm) & 0xFFFFFFFFull;
                const u64 p = n * m;
                setX(rd, sub ? a - p : a + p, true);
            } else if (op31 == 6) {
                setX(rd, umulh(X(rn), X(rm)), true);
            } else {
                const u64 n = X(rn), m = X(rm);
                u64 h = umulh(n, m);
                if (static_cast<i64>(n) < 0) h -= m;
                if (static_cast<i64>(m) < 0) h -= n;
                setX(rd, h, true);
            }
            break;
        }
        case da::Op::Dp2: {
            const unsigned opcode = bits(10, 6);
            const u64 a = X(rn) & mask_of(sf), b = X(rm) & mask_of(sf);
            switch (opcode) {
                case 2: setX(rd, b == 0 ? 0 : a / b, sf); break;
                case 3: {
                    const i64 x = static_cast<i64>(sx(a, sf ? 64 : 32)), y = static_cast<i64>(sx(b, sf ? 64 : 32));
                    if (y == 0) setX(rd, 0, sf);
                    else if (y == -1) setX(rd, static_cast<u64>(0) - static_cast<u64>(x), sf);
                    else setX(rd, static_cast<u64>(x / y), sf);
                    break;
                }
                case 8: case 9: case 10: case 11: setX(rd, shift_value(a, opcode - 8, static_cast<unsigned>(b % (sf ? 64 : 32)), sf), sf); break;
                default: return unsupported("crc32 is not run");
            }
            break;
        }
        case da::Op::Dp1: {
            const unsigned opcode = bits(10, 6), width = sf ? 64 : 32;
            const u64 v = X(rn) & mask_of(sf);
            switch (opcode) {
                case 0: setX(rd, reverse_bits(v, width), sf); break;
                case 1: setX(rd, reverse_bytes_in(v, 16, width), sf); break;
                case 2: setX(rd, reverse_bytes_in(v, 32, width), sf); break;
                case 3: setX(rd, reverse_bytes_in(v, 64, width), sf); break;
                case 4: setX(rd, static_cast<u64>(width - static_cast<unsigned>(std::bit_width(v))), sf); break;
                default: {   // cls: the number of bits after the sign bit equal to it
                    const u64 sign = (v >> (width - 1)) & 1;
                    unsigned n = 0;
                    for (int i = static_cast<int>(width) - 2; i >= 0 && ((v >> i) & 1) == sign; --i) ++n;
                    setX(rd, n, sf);
                    break;
                }
            }
            break;
        }
        case da::Op::B: next = R.pc + sx(bits(0, 26), 26) * 4; break;
        case da::Op::Bl:
            R.x[30] = R.pc + 4;
            next = R.pc + sx(bits(0, 26), 26) * 4;
            break;
        case da::Op::BCond:
            if (condition_holds(R.nzcv, bits(0, 4))) next = R.pc + sx(bits(5, 19), 19) * 4;
            break;
        case da::Op::Cbz: {
            const bool zero = (X(rd) & mask_of(sf)) == 0;
            if (zero == (bits(24, 1) == 0)) next = R.pc + sx(bits(5, 19), 19) * 4;
            break;
        }
        case da::Op::Tbz: {
            const unsigned bit = (bits(31, 1) << 5) | bits(19, 5);
            const bool set = (X(rd) >> bit) & 1;
            if (set == (bits(24, 1) == 1)) next = R.pc + sx(bits(5, 14), 14) * 4;
            break;
        }
        case da::Op::Br: next = X(rn); break;
        case da::Op::Blr: {
            const u64 target = X(rn);
            R.x[30] = R.pc + 4;
            next = target;
            break;
        }
        case da::Op::Ret: next = X(rn); break;
        case da::Op::Svc: {
            SvcCall call;
            call.number = bits(5, 16);
            for (unsigned i = 0; i < 8; ++i) call.args[i] = R.x[i];
            const SvcHandler& handler = svc ? svc : SvcHandler(horizon_svc);
            const bool go = handler(*this, call);
            calls.push_back(call);
            if (!go) {
                const std::string_view name = svc_name(call.number);
                return stop("svc " + (name.empty() ? std::format("{:#x}", call.number) : std::string(name)));
            }
            R.pc = next;
            return std::nullopt;
        }
        case da::Op::Brk: case da::Op::Hlt: case da::Op::Udf: case da::Op::Hvc: case da::Op::Smc: case da::Op::Eret:
            return stop(std::format("{} at {:#x}", std::string(insn.mnemonic), R.pc));
        case da::Op::Hint: case da::Op::Dsb: case da::Op::Dmb: case da::Op::Isb: break;
        case da::Op::Clrex: monitor_set_ = false; break;
        case da::Op::MsrImm: return unsupported("msr pstate is not run");
        case da::Op::Mrs: {
            const std::string_view name = da::system_register_name(bits(5, 15));
            u64 v;
            if (name == "nzcv") v = R.nzcv;
            else if (name == "fpcr") v = R.fpcr;
            else if (name == "fpsr") v = R.fpsr;
            else if (name == "tpidr_el0") v = R.tpidr_el0;
            else if (name == "tpidrro_el0") v = R.tpidrro_el0;
            else if (name == "cntvct_el0" || name == "cntpct_el0") {
                counter += 1000;
                v = counter;
            } else if (name == "cntfrq_el0") v = 19200000;
            else if (name == "dczid_el0") v = 4;
            else if (name == "ctr_el0") v = 0x8444C004;
            else return unsupported("system register not modelled");
            setX(rd, v, true);
            break;
        }
        case da::Op::Msr: {
            const std::string_view name = da::system_register_name(bits(5, 15));
            const u64 v = X(rd);
            if (name == "nzcv") R.nzcv = static_cast<u32>(v) & 0xF0000000u;
            else if (name == "fpcr") R.fpcr = static_cast<u32>(v);
            else if (name == "fpsr") R.fpsr = static_cast<u32>(v);
            else if (name == "tpidr_el0") R.tpidr_el0 = v;
            else return unsupported("system register not modelled");
            break;
        }
        case da::Op::Sys: {
            if (insn.mnemonic == "dc" && da::text(insn).starts_with("dc zva")) {
                const u64 base = X(rd) & ~u64{63};
                const std::uint8_t zero[64] = {};
                if (!mem_->write_bytes(base, zero, 64)) return fault_write(base);
            }
            break;
        }
        case da::Op::LdStImm:
        case da::Op::LdStReg: {
            const unsigned size = bits(30, 2), opc = bits(22, 2);
            const bool simd = bits(26, 1);
            u64 addr = XSP(rn);
            bool writeback = false;
            u64 new_base = 0;
            if (insn.op == da::Op::LdStReg) {
                const unsigned option = bits(13, 3);
                u64 off = X(rm);
                if ((option & 3) == 2) off = option & 4 ? sx(off, 32) : off & 0xFFFFFFFFull;
                if (bits(12, 1)) off <<= (simd && opc >= 2 ? 4 : size);
                addr += off;
            } else if (bits(24, 1)) {
                addr += static_cast<u64>(bits(10, 12)) << (simd && opc >= 2 ? 4 : size);
            } else {
                const i64 imm = static_cast<i64>(sx(bits(12, 9), 9));
                const unsigned mode = bits(10, 2);
                if (mode == 1) {
                    new_base = addr + static_cast<u64>(imm);
                    writeback = true;
                } else if (mode == 3) {
                    addr += static_cast<u64>(imm);
                    new_base = addr;
                    writeback = true;
                } else {
                    addr += static_cast<u64>(imm);
                }
            }
            bool is_load, signed_load = false, to64 = true;
            unsigned bytes = 1u << size;
            if (simd) {
                is_load = opc & 1;
                if (opc >= 2) bytes = 16;
            } else {
                is_load = opc != 0;
                if (opc >= 2) {
                    if (size == 3) break;   // prfm: no effect
                    signed_load = true;
                    to64 = opc == 2;
                }
            }
            if (is_load) {
                if (bytes == 16) {
                    std::uint8_t b[16];
                    if (!mem_->read_bytes(addr, 16, b)) return fault_read(addr);
                    std::memcpy(&R.v[rd].lo, b, 8);
                    std::memcpy(&R.v[rd].hi, b + 8, 8);
                } else {
                    u64 v;
                    if (!load(addr, bytes, v)) return fault_read(addr);
                    if (simd) R.v[rd] = {v, 0};
                    else setX(rd, signed_load ? sx(v, 8 * bytes) : v, signed_load ? to64 : size == 3);
                }
            } else {
                if (bytes == 16) {
                    std::uint8_t b[16];
                    std::memcpy(b, &R.v[rd].lo, 8);
                    std::memcpy(b + 8, &R.v[rd].hi, 8);
                    if (!mem_->write_bytes(addr, b, 16)) return fault_write(addr);
                } else {
                    const u64 v = simd ? R.v[rd].lo : X(rd);
                    if (!mem_->write(addr, bytes, v)) return fault_write(addr);
                }
            }
            if (writeback) setXSP(rn, new_base, true);
            break;
        }
        case da::Op::LdLit: {
            const u64 addr = R.pc + sx(bits(5, 19), 19) * 4;
            const unsigned opc = bits(30, 2);
            if (bits(26, 1)) {
                if (opc == 2) {
                    std::uint8_t b[16];
                    if (!mem_->read_bytes(addr, 16, b)) return fault_read(addr);
                    std::memcpy(&R.v[rd].lo, b, 8);
                    std::memcpy(&R.v[rd].hi, b + 8, 8);
                } else {
                    u64 v;
                    if (!load(addr, opc == 0 ? 4 : 8, v)) return fault_read(addr);
                    R.v[rd] = {v, 0};
                }
            } else if (opc != 3) {
                u64 v;
                if (!load(addr, opc == 0 ? 4 : 8, v)) return fault_read(addr);
                setX(rd, opc == 2 ? sx(v, 32) : v, opc != 0);
            }
            break;
        }
        case da::Op::LdStPair: {
            const unsigned opc = bits(30, 2), mode = bits(23, 2);
            const bool simd = bits(26, 1), is_load = bits(22, 1);
            const unsigned scale = simd ? 2 + opc : (opc == 2 ? 3 : 2);
            const unsigned bytes = 1u << scale;
            const unsigned rt2 = bits(10, 5);
            const i64 imm = static_cast<i64>(sx(bits(15, 7), 7)) * static_cast<i64>(bytes);
            u64 addr = XSP(rn);
            const u64 base_after = addr + static_cast<u64>(imm);
            if (mode != 1) addr += static_cast<u64>(imm);
            const auto one = [&](unsigned reg, u64 at) -> std::optional<std::string> {
                if (is_load) {
                    if (bytes == 16) {
                        std::uint8_t b[16];
                        if (!mem_->read_bytes(at, 16, b)) return fault_read(at);
                        std::memcpy(&R.v[reg].lo, b, 8);
                        std::memcpy(&R.v[reg].hi, b + 8, 8);
                    } else {
                        u64 v;
                        if (!load(at, bytes, v)) return fault_read(at);
                        if (simd) R.v[reg] = {v, 0};
                        else setX(reg, opc == 1 ? sx(v, 32) : v, opc != 0);
                    }
                } else if (bytes == 16) {
                    std::uint8_t b[16];
                    std::memcpy(b, &R.v[reg].lo, 8);
                    std::memcpy(b + 8, &R.v[reg].hi, 8);
                    if (!mem_->write_bytes(at, b, 16)) return fault_write(at);
                } else {
                    if (!mem_->write(at, bytes, simd ? R.v[reg].lo : X(reg))) return fault_write(at);
                }
                return std::nullopt;
            };
            if (auto e = one(rd, addr)) return e;
            if (auto e = one(rt2, addr + bytes)) return e;
            if (mode == 1 || mode == 3) setXSP(rn, mode == 1 ? base_after : addr, true);
            break;
        }
        case da::Op::LdStExcl: {
            const unsigned size = bits(30, 2);
            const bool o2 = bits(23, 1), is_load = bits(22, 1), pair = bits(21, 1);
            const unsigned bytes = 1u << size, rs = bits(16, 5), rt2 = bits(10, 5);
            const u64 addr = XSP(rn);
            const unsigned count = pair ? 2 : 1;
            if (is_load) {
                for (unsigned i = 0; i < count; ++i) {
                    u64 v;
                    if (!load(addr + i * bytes, bytes, v)) return fault_read(addr + i * bytes);
                    setX(i ? rt2 : rd, v, size == 3);
                }
                if (!o2 || pair) {
                    monitor_ = addr;
                    monitor_set_ = true;
                }
            } else {
                bool ok = true;
                if (!o2 || pair) {
                    ok = monitor_set_ && monitor_ == addr;
                    monitor_set_ = false;
                }
                if (ok) {
                    for (unsigned i = 0; i < count; ++i)
                        if (!mem_->write(addr + i * bytes, bytes, X(i ? rt2 : rd))) return fault_write(addr + i * bytes);
                }
                if (!o2 || pair) setX(rs, ok ? 0 : 1, false);
            }
            break;
        }
        case da::Op::LdStAtomic: {   // CAS{A}{L}{B,H} and CASP{A}{L}: compare Rs (and Rs+1) with memory, store Rt (and Rt+1) if equal, and put what memory held in Rs (and Rs+1)
            const bool pair = bits(23, 1) == 0;
            const unsigned size = pair ? 2 + bits(30, 1) : bits(30, 2);
            const unsigned bytes = 1u << size, rs = bits(16, 5), count = pair ? 2 : 1;
            if (pair && (((rs | rd) & 1) != 0)) return stop(std::format("undefined instruction {:08x} at {:#x}", w, R.pc));
            const u64 mask = bytes == 8 ? ~0ull : (1ull << (8 * bytes)) - 1;
            const u64 addr = XSP(rn);
            u64 old[2] = {0, 0};
            bool equal = true;
            for (unsigned i = 0; i < count; ++i) {
                if (!load(addr + i * bytes, bytes, old[i])) return fault_read(addr + i * bytes);
                equal = equal && (X(rs + i) & mask) == old[i];
            }
            if (equal)
                for (unsigned i = 0; i < count; ++i)
                    if (!mem_->write(addr + i * bytes, bytes, X(rd + i) & mask)) return fault_write(addr + i * bytes);
            for (unsigned i = 0; i < count; ++i) setX(rs + i, old[i], size == 3);
            break;
        }
        // ---- scalar floating point ----
        case da::Op::Fp1Src: {
            const unsigned ftype = bits(22, 2), opcode = bits(15, 6);
            if (ftype == 3) {
                const std::uint16_t h = static_cast<std::uint16_t>(R.v[rn].lo);
                if (opcode == 0) { R.v[rd] = {h, 0}; break; }
                if (opcode == 1) { R.v[rd] = {static_cast<u64>(h & 0x7FFF), 0}; break; }
                if (opcode == 2) { R.v[rd] = {static_cast<u64>(h ^ 0x8000), 0}; break; }
                if (opcode == 4 || opcode == 5) {
                    const double d = half_to_float(h);
                    R.v[rd] = {from_double(d, opcode == 4 ? 0 : 1), 0};
                    break;
                }
                return unsupported("half-precision arithmetic is not run");
            }
            const u64 raw = ftype == 0 ? (R.v[rn].lo & 0xFFFFFFFFull) : R.v[rn].lo;
            const double d = to_double(raw, ftype);
            switch (opcode) {
                case 0: R.v[rd] = {raw, 0}; break;
                case 1: R.v[rd] = {ftype == 0 ? (raw & 0x7FFFFFFFull) : (raw & 0x7FFFFFFFFFFFFFFFull), 0}; break;
                case 2: R.v[rd] = {ftype == 0 ? (raw ^ 0x80000000ull) : (raw ^ 0x8000000000000000ull), 0}; break;
                case 3: R.v[rd] = {from_double(ftype == 0 ? static_cast<double>(std::sqrt(static_cast<float>(d))) : std::sqrt(d), ftype), 0}; break;
                case 4: R.v[rd] = {from_double(d, 0), 0}; break;
                case 5: R.v[rd] = {from_double(d, 1), 0}; break;
                case 7: R.v[rd] = {float_to_half(d), 0}; break;
                case 8: case 9: case 10: case 11: case 12: case 14: case 15: {
                    const unsigned mode = opcode == 8 ? 0 : opcode == 9 ? 1 : opcode == 10 ? 2 : opcode == 11 ? 3 : opcode == 12 ? 4 : 0;
                    R.v[rd] = {from_double(round_mode(d, mode), ftype), 0};
                    break;
                }
                default: return unsupported("floating-point operation not run");
            }
            break;
        }
        case da::Op::Fp2Src: {
            const unsigned ftype = bits(22, 2), opcode = bits(12, 4);
            if (ftype == 3) return unsupported("half-precision arithmetic is not run");
            const double a = to_double(R.v[rn].lo, ftype), b = to_double(R.v[rm].lo, ftype);
            double r;
            switch (opcode) {
                case 0: r = a * b; break;
                case 1: r = a / b; break;
                case 2: r = a + b; break;
                case 3: r = a - b; break;
                case 4: r = std::isnan(a) || std::isnan(b) ? std::numeric_limits<double>::quiet_NaN() : std::fmax(a, b); break;
                case 5: r = std::isnan(a) || std::isnan(b) ? std::numeric_limits<double>::quiet_NaN() : std::fmin(a, b); break;
                case 6: r = std::fmax(a, b); break;   // (maxnm and minnm: a quiet NaN operand loses)
                case 7: r = std::fmin(a, b); break;
                default: r = -(a * b); break;
            }
            if (ftype == 0) {
                // single precision: compute in single so rounding is single's
                const float fa = static_cast<float>(a), fb = static_cast<float>(b);
                float fr;
                switch (opcode) {
                    case 0: fr = fa * fb; break;
                    case 1: fr = fa / fb; break;
                    case 2: fr = fa + fb; break;
                    case 3: fr = fa - fb; break;
                    case 8: fr = -(fa * fb); break;
                    default: fr = static_cast<float>(r); break;
                }
                R.v[rd] = {std::bit_cast<u32>(fr), 0};
            } else {
                R.v[rd] = {std::bit_cast<u64>(r), 0};
            }
            break;
        }
        case da::Op::Fp3Src: {
            const unsigned ftype = bits(22, 2);
            if (ftype == 3) return unsupported("half-precision arithmetic is not run");
            const bool o1 = bits(21, 1), o0 = bits(15, 1);
            if (ftype == 0) {
                const float n = std::bit_cast<float>(static_cast<u32>(R.v[rn].lo)), m = std::bit_cast<float>(static_cast<u32>(R.v[rm].lo)), a = std::bit_cast<float>(static_cast<u32>(R.v[ra].lo));
                const float r = !o1 && !o0 ? std::fma(n, m, a) : !o1 && o0 ? std::fma(-n, m, a) : o1 && !o0 ? std::fma(-n, m, -a) : std::fma(n, m, -a);
                R.v[rd] = {std::bit_cast<u32>(r), 0};
            } else {
                const double n = std::bit_cast<double>(R.v[rn].lo), m = std::bit_cast<double>(R.v[rm].lo), a = std::bit_cast<double>(R.v[ra].lo);
                const double r = !o1 && !o0 ? std::fma(n, m, a) : !o1 && o0 ? std::fma(-n, m, a) : o1 && !o0 ? std::fma(-n, m, -a) : std::fma(n, m, -a);
                R.v[rd] = {std::bit_cast<u64>(r), 0};
            }
            break;
        }
        case da::Op::FpCmp:
        case da::Op::FpCCmp: {
            const unsigned ftype = bits(22, 2);
            if (ftype == 3) return unsupported("half-precision arithmetic is not run");
            if (insn.op == da::Op::FpCCmp && !condition_holds(R.nzcv, bits(12, 4))) {
                R.nzcv = bits(0, 4) << 28;
                break;
            }
            const double a = to_double(R.v[rn].lo, ftype);
            const bool zero = insn.op == da::Op::FpCmp && bits(3, 1);
            const double b = zero ? 0.0 : to_double(R.v[rm].lo, ftype);
            if (std::isnan(a) || std::isnan(b)) R.nzcv = 0x30000000u;
            else if (a == b) R.nzcv = 0x60000000u;
            else if (a < b) R.nzcv = 0x80000000u;
            else R.nzcv = 0x20000000u;
            break;
        }
        case da::Op::FpCSel: {
            const unsigned ftype = bits(22, 2);
            const V128 pick = condition_holds(R.nzcv, bits(12, 4)) ? R.v[rn] : R.v[rm];
            R.v[rd] = {ftype == 0 ? pick.lo & 0xFFFFFFFFull : ftype == 3 ? pick.lo & 0xFFFF : pick.lo, 0};
            break;
        }
        case da::Op::FpImm: {
            const unsigned ftype = bits(22, 2);
            const double d = da::expand_fp_imm8(bits(13, 8));
            R.v[rd] = {ftype == 3 ? static_cast<u64>(float_to_half(d)) : from_double(d, ftype), 0};
            break;
        }
        case da::Op::FpInt: {
            const unsigned ftype = bits(22, 2), rmode = bits(19, 2), opcode = bits(16, 3);
            if (opcode == 6 || opcode == 7) {   // fmov between the files
                if (opcode == 6) {
                    if (rmode == 1) setX(rd, R.v[rn].hi, true);
                    else setX(rd, ftype == 0 ? R.v[rn].lo & 0xFFFFFFFFull : ftype == 3 ? R.v[rn].lo & 0xFFFF : R.v[rn].lo, sf);
                } else if (rmode == 1) {
                    R.v[rd].hi = X(rn);
                } else {
                    R.v[rd] = {ftype == 0 ? X(rn) & 0xFFFFFFFFull : ftype == 3 ? X(rn) & 0xFFFF : X(rn), 0};
                }
                break;
            }
            if (opcode == 2 || opcode == 3) {   // scvtf, ucvtf
                if (ftype == 3) return unsupported("half-precision arithmetic is not run");
                const u64 v = X(rn) & mask_of(sf);
                double d;
                float f;
                if (opcode == 2) {
                    const i64 s = static_cast<i64>(sx(v, sf ? 64 : 32));
                    d = static_cast<double>(s);
                    f = static_cast<float>(s);
                } else {
                    d = static_cast<double>(v);
                    f = static_cast<float>(v);
                }
                R.v[rd] = {ftype == 0 ? static_cast<u64>(std::bit_cast<u32>(f)) : std::bit_cast<u64>(d), 0};
                break;
            }
            if (ftype == 3) return unsupported("half-precision arithmetic is not run");
            const double d = to_double(R.v[rn].lo, ftype);
            const bool is_signed = !(opcode & 1);
            unsigned mode = rmode == 0 ? 0 : rmode == 1 ? 1 : rmode == 2 ? 2 : 3;
            if (opcode >= 4) mode = 4;
            setX(rd, to_integer(d, mode, is_signed, sf ? 64 : 32), sf);
            break;
        }
        case da::Op::FpFixed: {
            const unsigned ftype = bits(22, 2), opcode = bits(16, 3), fbits = 64 - bits(10, 6);
            if (ftype == 3) return unsupported("half-precision arithmetic is not run");
            if (opcode == 2 || opcode == 3) {
                const u64 v = X(rn) & mask_of(sf);
                const double s = opcode == 2 ? static_cast<double>(static_cast<i64>(sx(v, sf ? 64 : 32))) : static_cast<double>(v);
                const double d = std::ldexp(s, -static_cast<int>(fbits));
                R.v[rd] = {from_double(d, ftype), 0};
            } else {
                const double d = std::ldexp(to_double(R.v[rn].lo, ftype), static_cast<int>(fbits));
                setX(rd, to_integer(d, 3, opcode == 0, sf ? 64 : 32), sf);
            }
            break;
        }
        case da::Op::SimdThree: case da::Op::SimdThreeDiff: case da::Op::SimdTwoMisc: case da::Op::SimdAcross: case da::Op::SimdCopy: case da::Op::SimdModImm: case da::Op::SimdShift:
        case da::Op::SimdExt: case da::Op::SimdPermute: case da::Op::SimdByElem: case da::Op::SimdTable: case da::Op::SimdLdStMulti: case da::Op::SimdLdStSingle:
        case da::Op::SimdScalarThree: case da::Op::SimdScalarTwoMisc: case da::Op::SimdScalarPair: case da::Op::SimdScalarCopy: case da::Op::SimdScalarShift:
            if (const std::optional<std::string> why = detail::run_simd(R, *mem_, insn)) return why;
            break;
        default: return unsupported("not run");
    }
    R.pc = next;
    return std::nullopt;
}

RunResult run(Memory& memory, std::uint64_t entry, const RunOptions& options) {
    Cpu cpu(memory);
    cpu.regs.pc = entry;
    for (unsigned i = 0; i < 8; ++i) cpu.regs.x[i] = options.x[i];
    cpu.regs.x[18] = options.x18;
    cpu.regs.x[19] = options.x19;
    cpu.regs.x[29] = options.x29;
    cpu.regs.x[30] = kReturnAddress;
    cpu.regs.sp = options.sp;
    cpu.regs.tpidr_el0 = options.tpidr_el0;
    cpu.regs.tpidrro_el0 = options.tpidrro_el0;
    cpu.svc = options.svc ? options.svc : SvcHandler(horizon_svc);
    RunResult out;
    while (true) {
        if (cpu.regs.pc == kReturnAddress) {
            out.end = End::Return;
            break;
        }
        if (cpu.steps >= options.max_steps) {
            out.end = End::Limit;
            break;
        }
        if (options.on_instruction) options.on_instruction(cpu.regs);
        if (const std::optional<std::string> why = cpu.step()) {
            out.end = End::Stopped;
            out.reason = *why;
            break;
        }
    }
    out.regs = cpu.regs;
    out.steps = cpu.steps;
    out.calls = std::move(cpu.calls);
    return out;
}

}  // namespace cb::emulate::a64
