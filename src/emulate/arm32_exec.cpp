// cb-unity: alone
// emulate/arm32_exec.cpp -- the ARMv7-A Thumb instructions of decompile/arm32_thumb.cpp (and the ARM rows of the ARMv6 media family arm.cpp's execute_ext leaves out), run
// as the ARM ARM says. The fields are those the decoder gave (see there): `ext` names the instruction, rd, rn, rm and rs are registers, imm an offset or a bit count.
#include <bit>

#include "decompile/arm_ext.hpp"
#include "emulate/arm.hpp"

namespace cb::emulate::arm {

namespace da = decompile::arm;
namespace t2 = da::t2;

namespace {

constexpr std::uint32_t kQ = 0x08000000u, kC = 0x20000000u;

std::int32_t half_of(std::uint32_t v, bool top) { return static_cast<std::int32_t>(static_cast<std::int16_t>(top ? v >> 16 : v)); }
std::int32_t byte_of(std::uint32_t v, unsigned lane) { return static_cast<std::int32_t>(static_cast<std::int8_t>(v >> (8 * lane))); }

}  // namespace

std::optional<Fault> Cpu::execute_t32(const da::Insn& i, std::uint32_t at) {
    const auto bus_error = [&](std::uint32_t address) { return Fault{Fault::Kind::BusError, at, address, {}, 0}; };
    switch (i.ext) {
        case t2::kOrn: {   // ORN, ORNS: the first operand or the complement of the second
            const std::uint32_t first = operand_register(i, i.rn, !i.imm_form && i.shift_by_reg);
            const Shifted second = shifter(i);
            const std::uint32_t result = first | ~second.value;
            if (i.s) {
                set_nz(result);
                cpsr = (cpsr & ~kC) | (second.carry ? kC : 0u);
            }
            r[i.rd] = result;
            return std::nullopt;
        }
        case t2::kPkh: {   // PKHBT: the low half of Rn, the high half of the shifted Rm; PKHTB: the high half of Rn, the low half of the arithmetically shifted Rm
            if (i.shift == da::Shift::Lsl) {
                r[i.rd] = (r[i.rn] & 0xFFFFu) | ((r[i.rm] << i.amount) & 0xFFFF0000u);
            } else {
                const std::int32_t shifted = static_cast<std::int32_t>(r[i.rm]) >> (i.amount >= 32 ? 31 : i.amount);
                r[i.rd] = (r[i.rn] & 0xFFFF0000u) | (static_cast<std::uint32_t>(shifted) & 0xFFFFu);
            }
            return std::nullopt;
        }
        case t2::kAdr: {   // the pc (the instruction's address plus 4) word-aligned, plus or minus the offset
            const std::uint32_t aligned = (r[15] + 4) & ~3u;
            r[i.rd] = i.up ? aligned + i.imm : aligned - i.imm;
            return std::nullopt;
        }
        case t2::kSsatT: case t2::kUsatT: {
            const bool is_signed = i.ext == t2::kSsatT;
            const std::int64_t operand = i.shift == da::Shift::Lsl ? static_cast<std::int32_t>(r[i.rn] << i.amount) : static_cast<std::int32_t>(r[i.rn]) >> i.amount;
            const unsigned bits = i.imm;   // (SSAT: the field plus 1)
            const std::int64_t high = is_signed ? (std::int64_t{1} << (bits - 1)) - 1 : (std::int64_t{1} << bits) - 1;
            const std::int64_t low = is_signed ? -(std::int64_t{1} << (bits - 1)) : 0;
            const std::int64_t out = operand > high ? high : operand < low ? low : operand;
            if (out != operand) cpsr |= kQ;
            r[i.rd] = static_cast<std::uint32_t>(out);
            return std::nullopt;
        }
        case t2::kSsat16T: case t2::kUsat16T: {
            const bool is_signed = i.ext == t2::kSsat16T;
            const unsigned bits = i.imm;
            const std::int64_t high = is_signed ? (std::int64_t{1} << (bits - 1)) - 1 : (std::int64_t{1} << bits) - 1;
            const std::int64_t low = is_signed ? -(std::int64_t{1} << (bits - 1)) : 0;
            std::uint32_t result = 0;
            for (unsigned lane = 0; lane < 2; ++lane) {
                const std::int64_t operand = half_of(r[i.rn], lane != 0);
                const std::int64_t out = operand > high ? high : operand < low ? low : operand;
                if (out != operand) cpsr |= kQ;
                result |= (static_cast<std::uint32_t>(out) & 0xFFFFu) << (16 * lane);
            }
            r[i.rd] = result;
            return std::nullopt;
        }
        case t2::kMls: r[i.rd] = r[i.rs] - r[i.rn] * r[i.rm]; return std::nullopt;
        case t2::kNop32: return std::nullopt;
        case t2::kLdrexT: {   // LDREX [Rn, #imm], LDREXB, LDREXH
            const unsigned size = i.size;
            const std::uint32_t address = r[i.rn] + i.imm;
            if (!bus_->mapped(address & ~(size - 1))) return bus_error(address);
            r[i.rd] = bus_->read(address & ~(size - 1), size);
            return std::nullopt;
        }
        case t2::kStrexT: {   // STREX Rd, Rt, [Rn, #imm], STREXB, STREXH: the monitor always holds (one processor runs), the status is 0
            const unsigned size = i.size;
            const std::uint32_t address = r[i.rn] + i.imm;
            if (!bus_->mapped(address & ~(size - 1))) return bus_error(address);
            bus_->write(address & ~(size - 1), size, size == 4 ? r[i.rd] : r[i.rd] & ((1u << (8 * size)) - 1));
            r[i.rs] = 0;
            return std::nullopt;
        }
        case t2::kLdrexdT: {
            const std::uint32_t first = r[i.rn] & ~3u;
            if (!bus_->mapped(first) || !bus_->mapped(first + 4)) return bus_error(r[i.rn]);
            const std::uint32_t low = bus_->read(first, 4), high = bus_->read(first + 4, 4);
            r[i.rd] = low;
            r[i.rm] = high;
            return std::nullopt;
        }
        case t2::kStrexdT: {
            const std::uint32_t first = r[i.rn] & ~3u;
            if (!bus_->mapped(first) || !bus_->mapped(first + 4)) return bus_error(r[i.rn]);
            bus_->write(first, 4, r[i.rd]);
            bus_->write(first + 4, 4, r[i.rm]);
            r[i.rs] = 0;
            return std::nullopt;
        }
        case t2::kTb: {   // TBB, TBH: forward by twice the byte or halfword at [Rn + Rm] (or [Rn + 2 Rm])
            const std::uint32_t base = i.rn == 15 ? r[15] + 4 : r[i.rn];
            const std::uint32_t address = i.size == 1 ? base + r[i.rm] : base + r[i.rm] * 2;
            if (!bus_->mapped(address & ~(i.size - 1u))) return bus_error(address);
            const std::uint32_t offset = bus_->read(address & ~(i.size - 1u), i.size);
            branch((r[15] + 4 + 2 * offset) & ~1u);
            return std::nullopt;
        }
        default: return std::nullopt;
    }
}

// The ARM rows of the ARMv6 media family that execute_ext has no case of: RBIT, the 16-bit extensions, SSAT16 and USAT16, the rounding and exchanging multiplies, the
// long dual multiplies. True: it was one of them and has run.
bool Cpu::execute_media_more(const da::Insn& i, std::uint32_t w) {
    namespace ex = da::ext;
    const std::string_view name = ex::mnemonic(i.ext);
    const unsigned rd12 = (w >> 12) & 15, rn16 = (w >> 16) & 15, rm0 = w & 15, rs8 = (w >> 8) & 15;
    const std::uint32_t rotated = da::rotate_right(r[rm0], 8 * ((w >> 10) & 3));
    if (name == "rbit") {
        std::uint32_t v = r[rm0], out = 0;
        for (unsigned k = 0; k < 32; ++k) out |= ((v >> k) & 1u) << (31 - k);
        r[rd12] = out;
        return true;
    }
    if (name == "sxtb16" || name == "uxtb16" || name == "sxtab16" || name == "uxtab16") {
        const bool is_signed = name[0] == 's', accumulate = name.size() == 7;
        std::uint32_t out = 0;
        for (unsigned lane = 0; lane < 2; ++lane) {
            const std::uint32_t v = is_signed ? static_cast<std::uint32_t>(byte_of(rotated, 2 * lane)) : (rotated >> (16 * lane)) & 0xFFu;
            const std::uint32_t sum = (accumulate ? r[rn16] >> (16 * lane) : 0u) + v;
            out |= (sum & 0xFFFFu) << (16 * lane);
        }
        r[rd12] = out;
        return true;
    }
    if (name == "ssat16" || name == "usat16") {
        const bool is_signed = name[0] == 's';
        const unsigned bits = ((w >> 16) & 15) + (is_signed ? 1 : 0);
        const std::int64_t high = is_signed ? (std::int64_t{1} << (bits - 1)) - 1 : (std::int64_t{1} << bits) - 1;
        const std::int64_t low = is_signed ? -(std::int64_t{1} << (bits - 1)) : 0;
        std::uint32_t out = 0;
        for (unsigned lane = 0; lane < 2; ++lane) {
            const std::int64_t v = half_of(r[rm0], lane != 0);
            const std::int64_t s = v > high ? high : v < low ? low : v;
            if (s != v) cpsr |= kQ;
            out |= (static_cast<std::uint32_t>(s) & 0xFFFFu) << (16 * lane);
        }
        r[rd12] = out;
        return true;
    }
    if (name == "smmulr" || name == "smmlar" || name == "smmlsr") {
        const std::int64_t product = static_cast<std::int64_t>(static_cast<std::int32_t>(r[rm0])) * static_cast<std::int32_t>(r[rs8]);
        const std::int64_t accumulator = static_cast<std::int64_t>(static_cast<std::uint64_t>(r[rd12]) << 32);
        const std::int64_t total = name == "smmulr" ? product : name == "smmlar" ? accumulator + product : accumulator - product;
        r[rn16] = static_cast<std::uint32_t>((static_cast<std::uint64_t>(total) + 0x80000000u) >> 32);
        return true;
    }
    if (name == "smuadx" || name == "smusdx" || name == "smladx" || name == "smlsdx") {
        const std::uint32_t other = da::rotate_right(r[rs8], 16);
        const std::int64_t low = static_cast<std::int64_t>(half_of(r[rm0], false)) * half_of(other, false);
        const std::int64_t high = static_cast<std::int64_t>(half_of(r[rm0], true)) * half_of(other, true);
        std::int64_t total = name == "smuadx" || name == "smladx" ? low + high : low - high;
        if (name == "smladx" || name == "smlsdx") total += static_cast<std::int32_t>(r[rd12]);
        if (total != static_cast<std::int32_t>(total)) cpsr |= kQ;
        r[rn16] = static_cast<std::uint32_t>(total);
        return true;
    }
    if (name == "smlald" || name == "smlaldx" || name == "smlsld" || name == "smlsldx") {
        const std::uint32_t other = name.back() == 'x' ? da::rotate_right(r[rs8], 16) : r[rs8];
        const std::int64_t low = static_cast<std::int64_t>(half_of(r[rm0], false)) * half_of(other, false);
        const std::int64_t high = static_cast<std::int64_t>(half_of(r[rm0], true)) * half_of(other, true);
        const std::int64_t product = name.starts_with("smlald") ? low + high : low - high;
        const std::uint64_t sum = static_cast<std::uint64_t>(static_cast<std::int64_t>(static_cast<std::uint64_t>(r[rn16]) << 32 | r[rd12]) + product);
        r[rd12] = static_cast<std::uint32_t>(sum);
        r[rn16] = static_cast<std::uint32_t>(sum >> 32);
        return true;
    }
    return false;
}

}  // namespace cb::emulate::arm
