// build/x86asm/encode.cpp -- the encoder: the port of NASM's asm/assemble.c (see encode.hpp).
//
// The structure, the choices and the message texts are NASM's. The byte-code programs the templates carry are
// interpreted twice: `calcsize` (the length, and every decision that depends on the operands: REX, VEX/EVEX, prefixes,
// the effective address) and `gencode` (the bytes, sent to the output format piece by piece through `out`).
#include "build/x86asm/encode.hpp"

#include <algorithm>
#include <array>
#include <cstring>
#include <format>

namespace cb::build::x86asm {

namespace encode_impl {

// ---- small helpers (nasmlib.h, assemble.c) ----

// Sign-extend a value to `bits` bits (sext); zero-extend (zext).
constexpr std::int64_t sext(std::int64_t value, unsigned bits) {
    if (bits == 8) return static_cast<std::int8_t>(value);
    if (bits == 16) return static_cast<std::int16_t>(value);
    if (bits == 32) return static_cast<std::int32_t>(value);
    bits = bits ? 64 - bits : 63;
    return static_cast<std::int64_t>(static_cast<std::uint64_t>(value) << bits) >> bits;
}
constexpr std::uint64_t zext(std::uint64_t value, unsigned bits) {
    if (bits == 8) return static_cast<std::uint8_t>(value);
    if (bits == 16) return static_cast<std::uint16_t>(value);
    if (bits == 32) return static_cast<std::uint32_t>(value);
    if (bits >= 64) return value;
    return value & ((std::uint64_t{1} << bits) - 1);
}
constexpr bool overflow_signed(std::int64_t value, unsigned bytes) { return sext(value, bytes << 3) != value; }
constexpr bool overflow_unsigned(std::uint64_t value, unsigned bytes) { return zext(value, bytes << 3) != value; }
// Very conservative, but otherwise things like ~0x80 break.
constexpr bool overflow_general(std::int64_t value, unsigned bytes) {
    return overflow_unsigned(static_cast<std::uint64_t>(value), bytes) && overflow_unsigned(std::uint64_t{0} - static_cast<std::uint64_t>(value), bytes);
}
constexpr bool is_power2(std::uint64_t v) { return v && (v & (v - 1)) == 0; }

// A bit field of a word (getfield, fieldval).
constexpr std::uint32_t getfield(const Field& f, std::uint32_t from) { return (from >> f.pos) & f.basemask; }
constexpr std::uint32_t fieldval(const Field& f, std::uint32_t val) { return (val & f.basemask) << f.pos; }

constexpr int GEN_SIB(int scale, int index, int base) { return (scale << 6) | (index << 3) | base; }
constexpr int GEN_MODRM(int mod, int reg, int rm) { return (mod << 6) | ((reg & 7) << 3) | (rm & 7); }

// Convert operand/address/mode size to a BITS opflag constant. Not valid for 80+ bits.
constexpr OpFlags mode_to_op(unsigned bits) { return static_cast<OpFlags>(bits) << (SIZE_SHIFT - 3); }

// Any of REX_[BXR]1 corresponding to non-GPR registers, by masking them with the REX_[BXR]V flags.
constexpr std::uint32_t rex_highvec(std::uint32_t rexflags) { return rexflags & (rexflags >> 4) & REX_BXR1; }

// The shift for a compressed displacement (disp8 << shift) (disp8.h).
unsigned get_disp8_shift(const Insn& ins) {
    const Ttypes tuple = ins.evex_tuple;
    if (!tuple) return 0;
    const bool evex_b = (ins.evex & EVEX_B.mask) != 0;
    const unsigned evex_w = (ins.evex & EVEX_W.mask) != 0;
    // XXX (NASM): consider RC/SAE here?!
    const unsigned vectlen = getfield(EVEX_LL, ins.evex);
    switch (tuple) {
    case FV: return evex_b ? 2 + evex_w : vectlen + 4;
    case HV: return evex_b ? 2 + evex_w : vectlen + 3;
    case FVM: return vectlen + 4;
    case T1S8: return 0;
    case T1S16: return 1;
    case T1F32: return 2;
    case T1F64: return 3;
    case M128: return 4;
    case T1S: return 2 + evex_w;
    case T2: return 3 + evex_w;
    case T4: return 4 + evex_w;
    case T8: return 5 + evex_w;
    case HVM: return vectlen + 3;
    case QVM: return vectlen + 2;
    case OVM: return vectlen + 1;
    case DUP:
        // The 128-bit vector case doesn't follow the same formula as 256- and 512-bit vectors.
        if (!vectlen) return 3;
        return vectlen + 4;
    default: return 0;
    }
}

// The operand n of the instruction if it exists (get_operand).
const Operand* get_operand(const Insn& ins, unsigned n) { return n >= static_cast<unsigned>(ins.operands) ? nullptr : &ins.oprs[n]; }
Operand* get_operand(Insn& ins, unsigned n) { return n >= static_cast<unsigned>(ins.operands) ? nullptr : &ins.oprs[n]; }

bool absolute_op(const Operand* o) { return o && o->segment == NO_SEG && o->wrt == NO_SEG && !(o->opflags & OPFLAG_RELATIVE); }

bool has_prefix(const Insn& ins, int pos, int prefix) { return ins.prefixes[static_cast<std::size_t>(pos)] == prefix; }

const char* size_name(int size) {
    switch (size) {
    case 1: return "byte";
    case 2: return "word";
    case 4: return "dword";
    case 8: return "qword";
    case 10: return "tword";
    case 16: return "oword";
    case 32: return "yword";
    case 64: return "zword";
    default: return "???";
    }
}

// The pad that brings `len` to a multiple of `align`; must handle non-power-of-2 alignments (pad_bytes).
std::size_t pad_bytes(std::size_t len, std::size_t align) {
    const std::size_t partial = len % align;
    return partial ? align - partial : 0;
}

// The pseudo-opcode of a RESB-family or DB-family instruction's data type, for the debug info (eops_typeinfo).
std::int32_t eops_typeinfo(const std::vector<ExtOp>& eops) {
    std::int32_t typeinfo = 0;
    for (const ExtOp& e : eops) {
        switch (e.type) {
        case EOT_NOTHING: break;
        case EOT_EXTOP: typeinfo |= eops_typeinfo(e.subexpr); break;
        case EOT_DB_FLOAT:
            switch (e.elem) {
            case 1: typeinfo |= TY_BYTE; break;
            case 2: typeinfo |= TY_WORD; break;
            case 4: typeinfo |= TY_FLOAT; break;
            case 8: typeinfo |= TY_QWORD; break;    // double?
            case 10: typeinfo |= TY_TBYTE; break;   // long double?
            case 16: typeinfo |= TY_YWORD; break;
            case 32: typeinfo |= TY_ZWORD; break;
            default: break;
            }
            break;
        default:
            switch (e.elem) {
            case 1: typeinfo |= TY_BYTE; break;
            case 2: typeinfo |= TY_WORD; break;
            case 4: typeinfo |= TY_DWORD; break;
            case 8: typeinfo |= TY_QWORD; break;
            case 10: typeinfo |= TY_TBYTE; break;
            case 16: typeinfo |= TY_YWORD; break;
            case 32: typeinfo |= TY_ZWORD; break;
            default: break;
            }
            break;
        }
    }
    return typeinfo;
}

// The value of a prefix as a byte, or a PrefixErr when it has none (prefix_byte); may depend on the mode.
int prefix_byte(int pfx, int bits) {
    switch (pfx) {
    case P_WAIT_: return 0x9B;
    case P_LOCK: return 0xF0;
    case P_REPNE:
    case P_REPNZ:
    case P_XACQUIRE:
    case P_BND: return 0xF2;
    case P_REPE:
    case P_REPZ:
    case P_REP:
    case P_XRELEASE: return 0xF3;
    case R_CS:
    case P_PN: return 0x2E;
    case R_DS:
    case P_PT: return 0x3E;
    case R_ES: return 0x26;
    case R_FS: return 0x64;
    case R_GS: return 0x65;
    case R_SS: return 0x36;
    case P_A16:
        if (bits == 64) return PFE_ERR;
        if (bits == 32) return 0x67;
        return PFE_NULL;
    case P_A32: return bits != 32 ? 0x67 : PFE_NULL;
    case P_ASP: return 0x67;
    case P_O16: return bits != 16 ? 0x66 : PFE_NULL;
    case P_O32: return bits == 16 ? 0x66 : PFE_NULL;
    case P_O64: return bits == 64 ? PFE_NULL : PFE_ERR;
    case P_OSP: return 0x66;
    case P_REX:
    case P_VEX:
    case P_EVEX:
    case P_VEX3:
    case P_VEX2:
    case P_REX2: return PFE_MULTI;
    case P_NF:
    case P_NOBND:
    case P_ZU:
    case P_none: return PFE_NULL;
    default: return PFE_WHAT;
    }
}

// The EVEX flags a decorator word carries (evexflags).
std::uint32_t evexflags(DecoFlags deco_flags, std::uint32_t mask) {
    std::uint32_t evex = 0;
    if (deco_flags & deco::Z) evex |= EVEX_Z.mask;
    if (deco_flags & deco::OPMASK_MASK) evex |= (static_cast<std::uint32_t>(deco_flags) << 24) & EVEX_AAA.mask;
    return evex & mask;
}

// The REX flags of a register value and flag word under `mask` (rexflags).
std::uint32_t rexflags(int val, OpFlags flags, std::uint32_t mask) {
    std::uint32_t rex = 0;
    if (val < 0 || !is_class(opf::REGISTER, flags)) return 0;   // not a register
    if (val & 8) rex |= REX_B | REX_X | REX_R;
    if (val & 16) rex |= REX_B1 | REX_X1 | REX_R1;
    if (flags & opf::REG_CLASS_VECTOR) {
        rex |= REX_BV | REX_XV | REX_RV;
    } else if (is_class(opf::REG8, flags)) {
        if (is_class(opf::REG_HIGH, flags)) {
            rex |= REX_H;   // AH, CH, DH, BH: REX/REX2/VEX/EVEX forbidden
        } else if (val >= 4) {
            rex |= REX_P;   // SPL, BPL, SIL, DIL, or extended: prefix required
        }
    }
    return rex & mask;
}

// The number of broadcast elements an operand takes (get_broadcast_num).
std::uint8_t get_broadcast_num(OpFlags opflags, OpFlags brsize) {
    const unsigned opsize = static_cast<unsigned>((opflags & SIZE_MASK) >> SIZE_SHIFT);
    // The shift term takes care of the extra BITS80 inserted between BITS64 and BITS128.
    return static_cast<std::uint8_t>(((opsize / (opf::BITS64 >> SIZE_SHIFT)) * (opf::BITS64 / brsize)) >> static_cast<unsigned>(opsize > (opf::BITS64 >> SIZE_SHIFT)));
}

// A zero buffer for OUT_ZERODATA.
const std::array<std::uint8_t, kZeroBufSize> kZeroBuffer{};

}  // namespace encode_impl

using namespace encode_impl;

// ---- the output path ----

// Add the entries of struct out_data for the legacy backend interface, and submit to the backend (nasm_ofmt_output).
//
// The legacy `data` points to the address of the target for an address record, or to the bytes for OUT_RAWDATA.
// OUT_RELxADR denote an x-byte relocation of a relative jump; its size is the distance from the start of the relocated
// record to the end of the containing instruction. OUT_RESERVE reserves N bytes of BSS space.
void Encoder::nasm_ofmt_output(OutData& data) {
    const void* dptr = data.data;
    OutType type = data.type;
    std::int32_t tsegment = data.tsegment;
    std::int32_t twrt = data.twrt;
    std::uint64_t size = data.size;

    switch (data.type) {
    case OUT_RELADDR:
        switch (data.size) {
        case 1: type = OUT_REL1ADR; break;
        case 2: type = OUT_REL2ADR; break;
        case 4: type = OUT_REL4ADR; break;
        case 8: type = OUT_REL8ADR; break;
        default: as_.panic_here();
        }
        dptr = &data.toffset;
        size = static_cast<std::uint64_t>(data.relbase - data.loc.offset);
        break;

    case OUT_SEGMENT:
        type = OUT_ADDRESS;
        if (tsegment != NO_SEG && tsegment < SEG_ABS) tsegment |= 1;
        [[fallthrough]];

    case OUT_ADDRESS:
        dptr = &data.toffset;
        size = (data.flags & OUT_SIGNED) ? static_cast<std::uint64_t>(-static_cast<std::int64_t>(data.size)) : data.size;
        break;

    case OUT_RAWDATA:
    case OUT_RESERVE:
        tsegment = twrt = NO_SEG;
        break;

    case OUT_ZERODATA:
        tsegment = twrt = NO_SEG;
        type = OUT_RAWDATA;
        dptr = kZeroBuffer.data();
        break;

    default:
        as_.panic_here();
    }

    data.legacy.data = dptr;
    data.legacy.type = type;
    data.legacy.size = size;
    data.legacy.tsegment = tsegment;
    data.legacy.twrt = twrt;

    ofmt_.output(data);
}

void Encoder::warn_overflow(int size, const char* prefix, const char* suffix) {
    const char* pfxsp = prefix ? " " : "";
    const char* sufsp = suffix ? " " : "";
    as_.warn(WARN_NUMBER_OVERFLOW | ERR_PASS2, "{}{}{}{}{} exceeds bounds", prefix ? prefix : "", pfxsp, size_name(size), sufsp,
             suffix ? suffix : "");
}

void Encoder::warn_overflow_const(std::int64_t data, int size) {
    if (overflow_general(data, static_cast<unsigned>(size))) warn_overflow(size, nullptr, nullptr);
}

void Encoder::warn_overflow_out(std::int64_t data, int size, int flags, const char* what) {
    bool err;
    const char* prefix;

    if (flags & OUT_NOWARN) return;

    if (flags & OUT_SIGNED) {
        prefix = "signed";
        err = overflow_signed(data, static_cast<unsigned>(size));
    } else if (flags & OUT_UNSIGNED) {
        prefix = "unsigned";
        err = overflow_unsigned(static_cast<std::uint64_t>(data), static_cast<unsigned>(size));
    } else {
        prefix = nullptr;
        err = overflow_general(data, static_cast<unsigned>(size));
    }

    if (err) warn_overflow(size, prefix, what);
}

// This routine wrappers the real output format's output routine, in order to pass a copy of the data off to the listing
// file generator at the same time, flatten unnecessary relocations, and verify backend compatibility (out).
void Encoder::out(OutData& data) {
    std::array<std::uint8_t, 8> xdata{};
    std::size_t asize, amax;
    std::uint64_t zeropad = 0;
    std::int64_t addrval;
    std::int32_t fixseg;   // segment for which to produce fixed data

    if (!data.size) return;   // nothing to do

    const std::uint64_t real_size = data.size;

    // Convert addresses to RAWDATA if possible.
    // XXX (NASM): not all backends want this for global symbols!!!!
    switch (data.type) {
    case OUT_ADDRESS:
        addrval = static_cast<std::int64_t>(data.toffset);
        fixseg = NO_SEG;   // absolute address is fixed data
        goto address;

    case OUT_RELADDR:
        addrval = static_cast<std::int64_t>(data.toffset) - data.relbase;
        fixseg = data.loc.segment;   // our own segment is fixed data
        goto address;

    address:
        if (data.size > 8) as_.assert_failed("data->size <= 8");
        asize = data.size;
        amax = static_cast<std::size_t>(ofmt_.maxbits()) >> 3;   // maximum address size in bytes
        if (data.tsegment == fixseg && data.twrt == NO_SEG) {
            if (!(ofmt_.flags() & OFMT_KEEP_ADDR)) {
                if (asize >= static_cast<std::size_t>(data.bits >> 3)) {
                    // Support address space wrapping for low-bit modes.
                    data.flags &= ~OUT_SIGNMASK;
                }
                warn_overflow_out(addrval, static_cast<int>(asize), data.flags, data.what);
                const std::uint64_t q = static_cast<std::uint64_t>(addrval);
                for (int i = 0; i < 8; ++i) xdata[static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(q >> (8 * i));
                data.data = xdata.data();
                data.type = OUT_RAWDATA;
                asize = amax = 0;   // no longer an address
            }
        } else {
            ErrFlags warn;
            const char* type;

            switch (data.type) {
            case OUT_ADDRESS:
                type = "absolute";
                switch (asize) {
                case 1: warn = WARN_RELOC_ABS_BYTE; break;
                case 2: warn = WARN_RELOC_ABS_WORD; break;
                case 4: warn = WARN_RELOC_ABS_DWORD; break;
                case 8: warn = WARN_RELOC_ABS_QWORD; break;
                default: as_.panic_here();
                }
                break;
            case OUT_RELADDR:
                type = "relative";
                switch (asize) {
                case 1: warn = WARN_RELOC_REL_BYTE; break;
                case 2: warn = WARN_RELOC_REL_WORD; break;
                case 4: warn = WARN_RELOC_REL_DWORD; break;
                case 8: warn = WARN_RELOC_REL_QWORD; break;
                default: as_.panic_here();
                }
                break;
            default:
                type = "";
                warn = 0;
            }

            if (warn) as_.warn(warn, "{}-bit {} section-crossing relocation", static_cast<unsigned>(asize << 3), type);
        }
        break;

    case OUT_SEGMENT:
        if (data.size > 8) as_.assert_failed("data->size <= 8");
        asize = data.size;
        amax = 2;
        break;

    default:
        asize = amax = 0;   // not an address
        break;
    }

    // If the source location or output segment has changed, let the debug backend know. Some backends really don't like
    // being given a NULL filename as can happen if we use -Lb and expand a macro, so filter out that case.
    data.where = as_.src.where();
    if (!data.where.filename.empty() &&
        (!last_debug_.any || !(data.where == last_debug_.where) || data.loc.segment != last_debug_.segment)) {
        last_debug_.where = data.where;
        last_debug_.segment = data.loc.segment;
        last_debug_.any = true;
        if (dfmt_) dfmt_->linenum(last_debug_.where.filename, last_debug_.where.lineno, data.loc.segment);
    }

    if (asize > amax) {
        if (data.type == OUT_RELADDR || (data.flags & OUT_SIGNED)) {
            as_.nonfatal("{}-bit signed relocation unsupported by output format {}", static_cast<unsigned>(asize << 3), ofmt_.shortname());
        } else {
            as_.warn(WARN_ZEXT_RELOC, "{}-bit {} relocation zero-extended from {} bits", static_cast<unsigned>(asize << 3),
                     data.type == OUT_SEGMENT ? "segment" : "unsigned", static_cast<unsigned>(amax << 3));
        }
        zeropad = data.size - amax;
        data.size = amax;
    }
    if (lfmt_) lfmt_->output(data);

    if (data.loc.segment != NO_SEG) {
        if (data.type == OUT_ZERODATA && !(ofmt_.flags() & OFMT_ZERODATA)) {
            // Break OUT_ZERODATA up into ZERO_BUF_SIZE chunks unless the backend has indicated it can handle arbitrary
            // sizes by setting the OFMT_ZERODATA flag.
            std::uint64_t size = data.size;
            while (size > kZeroBufSize) {
                data.type = OUT_ZERODATA;   // help the compiler?
                data.size = kZeroBufSize;
                nasm_ofmt_output(data);
                size -= kZeroBufSize;
                data.loc.offset += static_cast<std::int64_t>(kZeroBufSize);
                data.insoffs += static_cast<int>(kZeroBufSize);
            }
            data.size = size;
        }
        nasm_ofmt_output(data);
    } else {
        // Outputting to ABSOLUTE section - only reserve is permitted.
        if (data.type != OUT_RESERVE) as_.nonfatal("attempt to assemble code in [ABSOLUTE] space");
        // No need to push to the backend.
    }

    // Prepare for the next instruction.
    data.loc.offset += static_cast<std::int64_t>(data.size);
    data.insoffs += static_cast<int>(data.size);

    // Note: this is never called with zeropad > ZERO_BUF_SIZE.
    if (zeropad) {
        data.type = OUT_ZERODATA;
        data.size = zeropad;
        if (lfmt_) lfmt_->output(data);
        nasm_ofmt_output(data);
        data.loc.offset += static_cast<std::int64_t>(zeropad);
        data.insoffs += static_cast<int>(zeropad);
    }

    // Restore the real data size in case the transaction was broken up.
    data.size = real_size;

    // Avoid confusion when this OutData is reused.
    data.what = nullptr;
}

void Encoder::out_rawdata(OutData& data, const void* rawdata, std::size_t size) {
    if (!size) return;
    data.type = OUT_RAWDATA;
    data.data = rawdata;
    data.size = size;
    out(data);
}

void Encoder::out_rawbyte(OutData& data, std::uint8_t byte) {
    data.type = OUT_RAWDATA;
    data.data = &byte;
    data.size = 1;
    out(data);
}

void Encoder::out_rawdword(OutData& data, std::uint32_t value) {
    const std::array<std::uint8_t, 4> buf = {static_cast<std::uint8_t>(value), static_cast<std::uint8_t>(value >> 8),
                                             static_cast<std::uint8_t>(value >> 16), static_cast<std::uint8_t>(value >> 24)};
    data.type = OUT_RAWDATA;
    data.data = buf.data();
    data.size = 4;
    out(data);
}

void Encoder::out_reserve(OutData& data, std::uint64_t size) {
    data.type = OUT_RESERVE;
    data.size = size;
    out(data);
}

// Emit a segment from a far expression. In this case, the segment offset is always zero. out_imm() handles explicit
// segment references, which may have addends.
void Encoder::out_farseg(OutData& data, const Operand& opx) {
    if (opx.opflags & OPFLAG_RELATIVE) as_.nonfatal("segment references cannot be relative");

    data.type = OUT_SEGMENT;
    data.flags = OUT_UNSIGNED;
    data.size = 2;
    data.toffset = 0;
    data.tsegment = ofmt_.segbase(opx.segment | 1);
    data.twrt = opx.wrt;
    out(data);
}

void Encoder::out_imm(OutData& data, const Operand& opx, int size, int sign) {
    if (opx.segment != NO_SEG && (opx.segment & 1)) {
        // This is actually a segment reference, but eval() has already called ofmt->segbase() for us.
        if (size < 2) as_.nonfatal("segment reference must be 16 bits");
        data.type = OUT_SEGMENT;
    } else {
        data.type = (opx.opflags & OPFLAG_RELATIVE) ? OUT_RELADDR : OUT_ADDRESS;
    }
    data.flags = sign;
    data.toffset = static_cast<std::uint64_t>(opx.offset);
    data.tsegment = opx.segment;
    data.twrt = opx.wrt;
    // XXX (NASM): improve this if at some point in the future we can distinguish the subtrahend in expressions like
    // [foo - bar] where bar is a symbol in the current segment. At present, if OPFLAG_RELATIVE is set that subtraction has
    // already occurred.
    data.relbase = 0;
    data.size = static_cast<std::uint64_t>(size);
    out(data);
}

void Encoder::out_reladdr(OutData& data, const Operand& opx, int size) {
    if (opx.opflags & OPFLAG_RELATIVE) as_.nonfatal("invalid use of self-relative expression");

    data.type = OUT_RELADDR;
    data.flags = OUT_SIGNED;
    data.size = static_cast<std::uint64_t>(size);
    data.toffset = static_cast<std::uint64_t>(opx.offset);
    data.tsegment = opx.segment;
    data.twrt = opx.wrt;
    data.relbase = data.loc.offset + (data.inslen - data.insoffs);
    out(data);
}

// Issue an error message on match failure.
void Encoder::no_match_error(MatchResult m, const Insn& ins) {
    switch (m) {
    case MERR_INVALOP: as_.nonfatalf(ERR_HOLD, "invalid combination of opcode and operands"); break;
    case MERR_OPSIZEINVAL: as_.nonfatalf(ERR_HOLD, "invalid operand sizes for instruction"); break;
    case MERR_OPSIZEMISSING: as_.nonfatalf(ERR_HOLD, "operation size not specified"); break;
    case MERR_OPSIZEMISMATCH: as_.nonfatalf(ERR_HOLD, "mismatch in operand sizes"); break;
    case MERR_BRNOTHERE: as_.nonfatalf(ERR_HOLD, "broadcast not permitted on this operand"); break;
    case MERR_BRNUMMISMATCH: as_.nonfatalf(ERR_HOLD, "mismatch in the number of broadcasting elements"); break;
    case MERR_MASKNOTHERE: as_.nonfatalf(ERR_HOLD, "mask not permitted on this operand"); break;
    case MERR_DECONOTHERE: as_.nonfatalf(ERR_HOLD, "unsupported mode decorator for instruction"); break;
    case MERR_BADCPU: as_.nonfatalf(ERR_HOLD, "no instruction for this cpu level"); break;
    case MERR_BADMODE: as_.nonfatalf(ERR_HOLD, "instruction not supported in {}-bit mode", ins.bits); break;
    case MERR_ENCMISMATCH:
        if (!ins.prefixes[PPS_REX]) {
            as_.nonfatalf(ERR_HOLD, "instruction not encodable without explicit prefix");
        } else {
            as_.nonfatalf(ERR_HOLD, "instruction not encodable with {} prefix", prefix_name(ins.prefixes[PPS_REX]));
        }
        break;
    case MERR_BADBND:
    case MERR_BADREPNE: as_.nonfatalf(ERR_HOLD, "{} prefix is not allowed", prefix_name(ins.prefixes[PPS_REP])); break;
    case MERR_REGSETSIZE: as_.nonfatalf(ERR_HOLD, "invalid register set size"); break;
    case MERR_REGSET: as_.nonfatalf(ERR_HOLD, "register set not valid for operand"); break;
    case MERR_WRONGIMM: as_.nonfatalf(ERR_HOLD, "operand/operator invalid for this instruction"); break;
    case MERR_BADZU: as_.nonfatalf(ERR_HOLD, "{{zu}} not applicable to this instruction"); break;
    case MERR_MEMZU: as_.nonfatalf(ERR_HOLD, "{{zu}} invalid for non-register destination"); break;
    case MERR_BADNF: as_.nonfatalf(ERR_HOLD, "{{nf}} not available for this instruction"); break;
    case MERR_REQNF: as_.nonfatalf(ERR_HOLD, "{{nf}} required for this instruction"); break;
    default:
        if (m < MOK_GOOD) as_.nonfatalf(ERR_HOLD, "invalid use of instruction");
        break;
    }
}

// This is a real hack. The jcc8 or jmp8 byte code must come first.
MatchResult Encoder::jmp_match(const Insn& ins, const ItemTemplate& temp) {
    const Operand* const op0 = get_operand(ins, 0);

    if (op0->type & opf::STRICT_) return MERR_INVALOP;

    if (ins.opt & (OPTIM_NO_Jcc_RELAX | OPTIM_NO_JMP_RELAX)) {
        const std::uint8_t c = temp.code[0];
        switch (c) {
        case 0370:
            if (ins.opt & OPTIM_NO_Jcc_RELAX) return MERR_INVALOP;
            break;
        case 0371:
            if (ins.opt & OPTIM_NO_JMP_RELAX) return MERR_INVALOP;
            break;
        default: return MERR_INVALOP;
        }
    }

    if (op0->opflags & OPFLAG_UNKNOWN) return MOK_GOOD;   // be optimistic in pass 1

    if (op0->segment != ins.loc.segment) return MERR_INVALOP;   // cross-segment jump

    // An instruction with a rel8 operand can only range between 2 and 15 bytes. If that is guaranteed true or false,
    // there is no reason to go through the process of calculating the exact instruction size. Note that the instruction
    // size is to be *subtracted* from the initial (beginning-of-instruction) delta value, or *added* to the limiting
    // value compared to.
    std::int64_t delta = op0->offset - ins.loc.offset;
    if (delta < -128 + 2 || delta > 127 + 15) {
        return MERR_INVALOP;   // this cannot be a byte-sized jump
    } else if (delta >= -128 + 15 && delta <= 127 + 2) {
        // It is guaranteed to be a valid byte-sized jump, no need to test.
    } else {
        // Borderline: need to do this the hard way...
        const std::int64_t isize = calcsize_speculative(ins, temp);
        if (isize < 0) return MERR_INVALOP;
        delta -= isize;
        if (static_cast<std::int8_t>(delta) != delta) return MERR_INVALOP;
    }

    return MOK_GOOD;
}

std::int64_t Encoder::merge_resb(Insn& ins, std::int64_t isize) {
    const int nbytes = resb_bytes(ins.opcode);

    if (!nbytes) return isize;

    if (isize != nbytes * ins.oprs[0].offset) return isize;   // has prefixes of some sort

    ins.oprs[0].offset *= ins.times;
    isize *= ins.times;
    ins.times = 1;
    return isize;
}

void Encoder::out_eops(OutData& data, const std::vector<ExtOp>& eops) {
    for (const ExtOp& e : eops) {
        std::size_t dup = e.dup;

        switch (e.type) {
        case EOT_NOTHING: break;

        case EOT_EXTOP:
            while (dup--) out_eops(data, e.subexpr);
            break;

        case EOT_DB_NUMBER:
            if (e.elem > 8) {
                as_.nonfatal("integer supplied as {}-bit data", e.elem << 3);
            } else {
                while (dup--) {
                    data.insoffs = 0;
                    data.inslen = e.elem;
                    data.size = static_cast<std::uint64_t>(e.elem);
                    data.tsegment = e.segment;
                    data.toffset = static_cast<std::uint64_t>(e.offset);
                    data.twrt = e.wrt;
                    data.relbase = 0;
                    if (e.segment != NO_SEG && (e.segment & 1)) {
                        data.type = OUT_SEGMENT;
                        data.flags = OUT_UNSIGNED;
                    } else {
                        data.type = e.relative ? OUT_RELADDR : OUT_ADDRESS;
                        data.flags = OUT_WRAP;
                    }
                    out(data);
                }
            }
            break;

        case EOT_DB_FLOAT:
        case EOT_DB_STRING:
        case EOT_DB_STRING_FREE: {
            const std::size_t pad = pad_bytes(e.string.size(), static_cast<std::size_t>(e.elem));
            const std::size_t len = e.string.size() + pad;

            while (dup--) {
                data.insoffs = 0;
                data.inslen = static_cast<int>(len);
                out_rawdata(data, e.string.data(), e.string.size());
                if (pad) out_rawdata(data, kZeroBuffer.data(), pad);
            }
            break;
        }

        case EOT_DB_RESERVE:
            data.insoffs = 0;
            data.inslen = static_cast<int>(dup * static_cast<std::size_t>(e.elem));
            out_reserve(data, static_cast<std::uint64_t>(data.inslen));
            break;
        }
    }
}

// Process an EQU directive.
void Encoder::define_equ(Insn& ins) {
    if (ins.label.empty()) {
        as_.nonfatal("EQU not preceded by label");
    } else if (ins.operands == 1 && (ins.oprs[0].type & opf::IMMEDIATE) && ins.oprs[0].wrt == NO_SEG) {
        if (hooks_.define_label) hooks_.define_label(ins.label, ins.oprs[0].segment, ins.oprs[0].offset, false);
    } else if (ins.operands == 2 && (ins.oprs[0].type & opf::IMMEDIATE) && (ins.oprs[0].type & opf::COLON) &&
               ins.oprs[0].segment == NO_SEG && ins.oprs[0].wrt == NO_SEG && (ins.oprs[1].type & opf::IMMEDIATE) &&
               ins.oprs[1].segment == NO_SEG && ins.oprs[1].wrt == NO_SEG) {
        if (hooks_.define_label) hooks_.define_label(ins.label, static_cast<std::int32_t>(ins.oprs[0].offset | SEG_ABS), ins.oprs[1].offset, false);
    } else {
        as_.nonfatal("bad syntax for EQU");
    }
}

void Encoder::debug_set_db_type(const Insn& ins) {
    std::int32_t typeinfo = static_cast<std::int32_t>(tys_elements(ins.operands));
    typeinfo |= eops_typeinfo(ins.eops);
    if (dfmt_) dfmt_->typevalue(typeinfo);
}

void Encoder::debug_set_type(const Insn& ins) {
    std::int32_t typeinfo;

    if (opcode_is_resb(ins.opcode)) {
        typeinfo = static_cast<std::int32_t>(tys_elements(static_cast<unsigned>(ins.oprs[0].offset)));

        switch (ins.opcode) {
        case I_RESB: typeinfo |= TY_BYTE; break;
        case I_RESW: typeinfo |= TY_WORD; break;
        case I_RESD: typeinfo |= TY_DWORD; break;
        case I_RESQ: typeinfo |= TY_QWORD; break;
        case I_REST: typeinfo |= TY_TBYTE; break;
        case I_RESO: typeinfo |= TY_OWORD; break;
        case I_RESY: typeinfo |= TY_YWORD; break;
        case I_RESZ: typeinfo |= TY_ZWORD; break;
        default: as_.panic_here();
        }
    } else {
        typeinfo = TY_LABEL;
    }

    if (dfmt_) dfmt_->typevalue(typeinfo);
}

std::int64_t Encoder::len_extops(const std::vector<ExtOp>& eops) {
    std::int64_t isize = 0;

    for (const ExtOp& e : eops) {
        switch (e.type) {
        case EOT_NOTHING: break;

        case EOT_EXTOP: isize += static_cast<std::int64_t>(e.dup) * len_extops(e.subexpr); break;

        case EOT_DB_STRING:
        case EOT_DB_STRING_FREE:
        case EOT_DB_FLOAT: {
            const std::size_t pad = pad_bytes(e.string.size(), static_cast<std::size_t>(e.elem));
            isize += static_cast<std::int64_t>(e.dup * (e.string.size() + pad));
            break;
        }

        case EOT_DB_NUMBER:
            warn_overflow_const(e.offset, e.elem);
            isize += static_cast<std::int64_t>(e.dup) * e.elem;
            break;

        case EOT_DB_RESERVE: isize += static_cast<std::int64_t>(e.dup) * e.elem; break;
        }
    }

    return isize;
}

// The length of an instruction for the passes before the last (insn_size).
std::int64_t Encoder::insn_size(Insn& ins) {
    if (ins.opcode == I_none) {
        return 0;
    } else if (ins.opcode == I_EQU) {
        define_equ(ins);
        return 0;
    } else if (opcode_is_db(ins.opcode)) {
        const std::int64_t isize = len_extops(ins.eops);
        debug_set_db_type(ins);
        return isize;
    } else if (ins.opcode == I_INCBIN) {
        const std::string& fname = ins.eops[0].string;
        std::optional<std::vector<std::uint8_t>> file;
        if (hooks_.read_file) file = hooks_.read_file(fname);
        if (!file) {
            as_.nonfatal("`incbin': unable to get length of file `{}'", fname);
            return 0;
        }
        std::int64_t len = static_cast<std::int64_t>(file->size());

        if (ins.eops.size() > 1) {
            const ExtOp& e = ins.eops[1];
            if (len <= e.offset) {
                len = 0;
            } else {
                len -= e.offset;
                if (ins.eops.size() > 2 && len > ins.eops[2].offset) len = ins.eops[2].offset;
            }
        }

        len *= ins.times;
        ins.times = 1;   // tell the upper layer to not iterate

        return len;
    } else {
        // Normal instruction, or RESx.

        // Pre-matching setup.
        insn_early_setup(ins);

        const MatchResult m = find_match(ins);
        if (m < MOK_GOOD) {
            no_match_error(m, ins);
            return -1;   // no match
        }

        std::int64_t isize = calcsize(ins);
        debug_set_type(ins);
        isize = merge_resb(ins, isize);

        return isize;
    }
}

// Generate the code of an instruction for the last pass (assemble).
std::int64_t Encoder::assemble(Insn& ins) {
    OutData data;
    const std::int64_t start = ins.loc.offset;

    if (ins.opcode == I_none) return 0;

    data.loc = ins.loc;
    data.bits = ins.bits;

    if (opcode_is_db(ins.opcode)) {
        out_eops(data, ins.eops);
    } else if (ins.opcode == I_INCBIN) {
        const std::string& fname = ins.eops[0].string;
        std::size_t t = static_cast<std::size_t>(ins.times);   // INCBIN handles TIMES by itself
        bool need_downlevel_times = false;
        std::int64_t base = 0;

        if (t) {
            std::optional<std::vector<std::uint8_t>> file;
            if (hooks_.read_file) file = hooks_.read_file(fname);
            if (!file) {
                as_.nonfatal("`incbin': unable to open file `{}'", fname);
            } else {
                std::int64_t len = static_cast<std::int64_t>(file->size());

                if (ins.eops.size() > 1) {
                    base = ins.eops[1].offset;
                    if (base >= len) {
                        len = 0;
                    } else {
                        len -= base;
                        if (ins.eops.size() > 2 && len > ins.eops[2].offset) len = ins.eops[2].offset;
                    }
                }

                if (lfmt_) lfmt_->set_offset(data.loc.offset);

                if (len) {
                    while (t--) {
                        // Consider these irrelevant for INCBIN, since it is fully possible that these might be (way)
                        // bigger than an int can hold. data.inslen == 0 signals to the backend that these fields are
                        // meaningless, if at all needed.
                        data.insoffs = 0;
                        data.inslen = 0;

                        if (lfmt_) lfmt_->uplevel(ListType::INCBIN, len);
                        out_rawdata(data, file->data() + base, static_cast<std::size_t>(len));
                        if (lfmt_) lfmt_->downlevel(ListType::INCBIN);

                        if (t && !need_downlevel_times) {
                            if (lfmt_) lfmt_->uplevel(ListType::TIMES, ins.times);
                            need_downlevel_times = true;
                        }
                    }
                    if (need_downlevel_times && lfmt_) lfmt_->downlevel(ListType::TIMES);
                }
            }
        }
        ins.times = 1;   // tell the upper layer not to iterate
    } else {
        // "Real" instruction.

        // Pre-match instruction structure update.
        insn_early_setup(ins);

        const MatchResult m = find_match(ins);

        if (m >= MOK_GOOD) {
            const ItemTemplate& temp = *ins.itemp;

            // Matches!
            if (lfmt_ && lfmt_->option('X')) {
                lfmt_->line(ListType::INFO, -1,
                            std::format(" ;;; used template {}:{} from insns.xda:{}", kInsnNames[static_cast<std::size_t>(ins.opcode)],
                                        ins.itempindex, ins.itemp->xdaline));
            }

            if (itemp_has(temp, IF_OBSOLETE)) {
                ErrFlags warning;
                const char* validity;
                const bool never = itemp_has(temp, IF_NEVER);

                // If IF_OBSOLETE is set, warn the user. Different warning classes for "obsolete but valid for this
                // specific CPU" and "obsolete and gone." (NASM: this currently doesn't really happen correctly; it
                // requires better information in insns.dat.)
                const char* whathappened = never ? "never implemented" : "obsolete";

                if (!never && !iflag_cmp_cpu_level(itemp_flags(temp), as_.cpu)) {
                    warning = WARN_OBSOLETE_VALID;
                    validity = "but valid on";
                } else if (itemp_has(temp, IF_NOP)) {
                    warning = WARN_OBSOLETE_NOP;
                    validity = "and is a noop on";
                } else {
                    warning = WARN_OBSOLETE_REMOVED;
                    validity = never ? "and invalid on" : "and removed from";
                }

                as_.warn(warning, "instruction {} {} the target CPU", whathappened, validity);
            }

            data.inslen = static_cast<int>(calcsize(ins));

            // This can happen if the instruction generated an error.
            if (data.inslen <= 0) return 0;

            data.inslen = static_cast<int>(merge_resb(ins, data.inslen));

            data.insoffs = 0;
            gencode(data, ins);
            if (data.insoffs != data.inslen) {
                as_.nonfatal("instruction length changed during code generation ({} -> {})", static_cast<unsigned>(data.insoffs),
                             static_cast<unsigned>(data.inslen));
            }

            if (data.loc.offset - start != data.inslen) as_.assert_failed("data.loc.offset - start == data.inslen");
        } else {
            no_match_error(m, ins);
            ins.times = 1;   // avoid repeated error messages
        }
    }
    return data.loc.offset - start;
}

void Encoder::bad_hle_warn(const Insn& ins, std::uint8_t hleok) {
    enum WhatWarn { w_none, w_lock, w_inval };
    static constexpr WhatWarn warn[2][4] = {
        {w_inval, w_inval, w_none, w_lock},   // XACQUIRE
        {w_inval, w_none, w_none, w_lock},    // XRELEASE
    };

    const int rep_pfx = ins.prefixes[PPS_REP];
    const unsigned n = static_cast<unsigned>(rep_pfx) - static_cast<unsigned>(P_XACQUIRE);
    if (n > 1) return;   // not XACQUIRE/XRELEASE

    WhatWarn ww = warn[n][hleok];
    if (!is_class(opf::MEMORY, ins.oprs[0].type)) ww = w_inval;   // HLE requires operand 0 to be memory

    switch (ww) {
    case w_none: break;
    case w_lock:
        if (ins.prefixes[PPS_LOCK] != P_LOCK) as_.warn(WARN_PREFIX_HLE, "{} with this instruction requires lock", prefix_name(rep_pfx));
        break;
    case w_inval: as_.warn(WARN_PREFIX_HLE, "{} invalid with this instruction", prefix_name(rep_pfx)); break;
    }
}

// Handle EVEX flags at the time of EA processing.
int Encoder::ea_evex_flags(Insn& ins, const Operand& opy) {
    // EVEX.b1: evex_brerop contains the operand position.
    const Operand* op_er_sae = ins.evex_brerop >= 0 ? &ins.oprs[static_cast<std::size_t>(ins.evex_brerop)] : nullptr;
    const DecoFlags decof = op_er_sae ? op_er_sae->decoflags : DecoFlags{0};

    if (decof & (deco::ER | deco::SAE)) {
        // EVEX_LL overlays EVEX_RC, so LL must = 2 or be ignored.
        if (!itemp_has(*ins.itemp, IF_LIG)) {
            const unsigned vlen = getfield(EVEX_LL, ins.evex);

            if (vlen != 2) {
                const char* what = (decof & deco::ER) ? "embedded rounding" : "suppress all exceptions";
                as_.nonfatal("{} not possible for {}-bit vectors", what, 128 << vlen);
                return -1;
            }
        }

        ins.evex &= ~EVEX_RC.mask;
        ins.evex ^= EVEX_B.mask;
        if (op_er_sae->decoflags & deco::ER) {
            // Set EVEX.RC (rounding control).
            ins.evex ^= fieldval(EVEX_RC, static_cast<std::uint32_t>(ins.evex_rm - BRC_RN));
        }
    } else if (opy.decoflags & deco::BRDCAST_MASK) {
        // Set EVEX.b but don't EVEX.L/RC.
        ins.evex ^= EVEX_B.mask;
    }

    return 0;
}

// Call calcsize() without modifying the source instruction, and without generating errors. This answers the question "how
// long would this instruction be if it were to be generated at this point in the code" (used by jmp_match()). Returns < 0
// if generating the instruction would throw an error.
std::int64_t Encoder::calcsize_speculative(const Insn& ins, const ItemTemplate& itemp) {
    Insn tmpins = ins;
    tmpins.dummy = true;
    tmpins.itemp = &itemp;
    const ErrHold hold = as_.error_hold_push();

    const std::int64_t isize = calcsize(tmpins);

    if (as_.error_hold_pop(hold, false) >= ERR_NONFATAL) return -1;

    return isize;
}

// Common constructs of the byte-code switch: a code and the next one, two or three after it (case3, case4).
#define CB_CASE3(x) case (x): case (x) + 1: case (x) + 2
#define CB_CASE4(x) CB_CASE3(x): case (x) + 3

std::int64_t Encoder::calcsize(Insn& ins_) {
    Insn* const ins = &ins_;
    const ItemTemplate& temp = *ins->itemp;
    const int bits = ins->bits;
    const std::uint8_t* codes = temp.code;
    std::int64_t length = 0;
    std::uint8_t c;
    int op1, op2;
    Operand *opx, *opy;
    std::uint8_t opex = 0;
    EaType eat;
    std::uint8_t hleok = 0;
    bool lockcheck = true;
    int mib_index = R_none;   // for a separate index reg form
    std::string errmsg;
    int opt_opsize = 0;

    ins->rex = 0;      // ensure REX is reset
    ins->evex = 0;     // ensure EVEX is reset
    ins->vexreg = 0;   // no V register
    ins->vex_cm = 0;   // no implicit map
    eat = EA_SCALAR;   // expect a scalar EA

    // Default operand size (prefixes are handled in the byte code).
    ins->op_size = bits != 16 ? 32 : 16;

    while (*codes) {
        c = *codes++;
        op1 = (c & 3) + ((opex & 1) << 2);
        opx = get_operand(*ins, static_cast<unsigned>(op1));
        op2 = ((c >> 3) & 3) + ((opex & 2) << 1);
        opy = nullptr;
        opex = 0;   // for the next iteration

        switch (c) {
        CB_CASE4(01):
            codes += c, length += c;
            break;

        CB_CASE3(05):
            opex = c;
            break;

        CB_CASE4(010):
            ins->rex |= [&] {
                if (!is_register(opx->basereg)) as_.panic("invalid operand passed to op_rexflags()");
                return rexflags(kRegVals[static_cast<std::size_t>(opx->basereg)], kRegFlags[static_cast<std::size_t>(opx->basereg)], REX_rB);
            }();
            codes++, length++;
            break;

        CB_CASE4(014):
            // This is an index reg of a split SIB operand.
            mib_index = opx->basereg;
            break;

        CB_CASE4(020):
        CB_CASE4(024):
            length++;
            break;

        CB_CASE4(030):
            length += 2;
            break;

        CB_CASE4(034):
            if (opx->type & (opf::BITS16 | opf::BITS32 | opf::BITS64))
                length += (opx->type & opf::BITS16) ? 2 : 4;
            else
                length += (ins->op_size == 16) ? 2 : 4;
            break;

        CB_CASE4(040):
            length += 4;
            break;

        CB_CASE4(044):
            length += ins->addr_size >> 3;
            break;

        CB_CASE4(050):
            length++;
            break;

        CB_CASE4(054):
            length += 8;   // MOV reg64/imm
            break;

        CB_CASE4(060):
            length += 2;
            break;

        CB_CASE4(064):
            if (opx->type & (opf::BITS16 | opf::BITS32 | opf::BITS64))
                length += (opx->type & opf::BITS16) ? 2 : 4;
            else
                length += (ins->op_size == 16) ? 2 : 4;
            break;

        CB_CASE4(070):
            length += 4;
            break;

        CB_CASE4(074):
            length += 2;
            break;

        case 0171:
            c = *codes++;
            op2 = (op2 & ~3) | ((c >> 3) & 3);
            opy = get_operand(*ins, static_cast<unsigned>(op2));
            if (!is_register(opy->basereg)) as_.panic("invalid operand passed to op_rexflags()");
            ins->rex |= rexflags(kRegVals[static_cast<std::size_t>(opy->basereg)], kRegFlags[static_cast<std::size_t>(opy->basereg)], REX_rR);
            length++;
            break;

        case 0172:
        case 0173:
            codes++;
            length++;
            break;

        CB_CASE4(0174):
            length++;
            break;

        CB_CASE4(0240):
            if (is_class(opx->type, opf::IMMEDIATE)) {
                ins->veximm = static_cast<std::uint8_t>(opx->offset);
            } else {
                if (!is_register(opx->basereg)) as_.panic("invalid operand passed to regval()");
                ins->vexreg = static_cast<std::uint8_t>(kRegVals[static_cast<std::size_t>(opx->basereg)]);
            }
            goto evex_common;

        case 0250:
            ins->vexreg = 0;
            goto evex_common;

        evex_common:
            ins->rex |= REX_EV;
            ins->evex = 0x62;
            ins->evex += static_cast<std::uint32_t>(*codes++) << 8;
            ins->evex += static_cast<std::uint32_t>(*codes++) << 16;
            ins->evex += static_cast<std::uint32_t>(*codes++) << 24;
            ins->evex_tuple = static_cast<Ttypes>(*codes++ - 0300);
            ins->vex_cm = static_cast<std::uint8_t>(((ins->evex >> 8) & 7) | (RV_EVEX << 6));
            break;

        CB_CASE4(0254):
        CB_CASE4(0264):
            length += 4;
            break;

        CB_CASE4(0260):
            if (is_class(opx->type, opf::IMMEDIATE)) {
                ins->veximm = static_cast<std::uint8_t>(opx->offset);
            } else {
                if (!is_register(opx->basereg)) as_.panic("invalid operand passed to regval()");
                ins->vexreg = static_cast<std::uint8_t>(kRegVals[static_cast<std::size_t>(opx->basereg)]);
            }
            goto vex_common;

        case 0270:
            ins->vexreg = 0;
            goto vex_common;

        vex_common:
            ins->rex |= REX_V;
            ins->vex_cm = *codes++;
            ins->vex_wlp = *codes++;
            break;

        CB_CASE3(0271):
            hleok = c & 3;
            break;

        CB_CASE4(0274):
            length++;
            break;

        CB_CASE4(0300):
            length++;
            break;

        CB_CASE4(0304):
            length++;
            codes += 2;
            break;

        case 0310: {
            const int pfx = ins->prefixes[PPS_ASIZE];

            ins->addr_size = 16;

            if (bits == 64) return -1;
            if (pfx) {
                if (!(pfx == P_A16 || (bits == 32 && pfx == P_ASP))) return -1;
            } else {
                ins->prefixes[PPS_ASIZE] = P_A16;
            }
            break;
        }

        case 0311: {
            const int pfx = ins->prefixes[PPS_ASIZE];

            ins->addr_size = 32;

            if (pfx) {
                if (!(pfx == P_A32 || (bits != 32 && pfx == P_ASP))) return -1;
            } else {
                ins->prefixes[PPS_ASIZE] = P_A32;
            }
            break;
        }

        case 0312:
            break;

        case 0313: {
            const int pfx = ins->prefixes[PPS_ASIZE];

            ins->addr_size = 64;

            if (bits != 64 || (pfx && pfx != P_A64))
                return -1;
            else
                ins->prefixes[PPS_ASIZE] = P_A64;
            break;
        }

        CB_CASE4(0314):
            break;

        case 0320:
        is_o16 : {
            const int pfx = ins->prefixes[PPS_OSIZE];
            ins->op_size = 16;
            if (bits != 16 && pfx == P_OSP) {
                // Allow osp prefix as is.
            } else if (pfx != P_none && pfx != P_O16) {
                as_.warn(WARN_PREFIX_OPSIZE, "invalid operand size prefix {}, must be o16", prefix_name(pfx));
            } else if (!(opt_opsize & 16)) {
                ins->prefixes[PPS_OSIZE] = P_O16;
            }
            break;
        }

        case 0321:
        is_o32 : {
            const int pfx = ins->prefixes[PPS_OSIZE];
            ins->op_size = 32;
            if (ins->rex & REX_NW) {
                as_.nonfatalf(ERR_PASS2, "instruction not valid with 32-bit operand size");
                return -1;
            } else if (bits == 16 && pfx == P_OSP) {
                // Allow osp prefix as is.
            } else if (pfx != P_none && pfx != P_O32) {
                as_.warn(WARN_PREFIX_OPSIZE, "invalid operand size prefix {}, must be o32", prefix_name(pfx));
            } else if (!(opt_opsize & 32)) {
                ins->prefixes[PPS_OSIZE] = P_O32;
            }
            break;
        }

        case 0322:
            break;

        case 0327:
            if (bits == 64) {
                ins->rex |= REX_NW;
                if (ins->op_size == 32) ins->op_size = 64;
            }
            break;

        case 0323:
            ins->rex |= REX_NW;
            [[fallthrough]];

        case 0324:
        is_o64 : {
            const int pfx = ins->prefixes[PPS_OSIZE];
            ins->op_size = 64;
            if (pfx == P_OSP) {
                // If osp is specified, leave it, but force REX.W.
                if (!(ins->rex & REX_NW)) ins->rex |= REX_W;
            } else if (pfx != P_none && pfx != P_O64) {
                as_.warn(WARN_PREFIX_OPSIZE, "invalid operand size prefix {}, must be o64", prefix_name(pfx));
            } else if (!(opt_opsize & 64)) {
                ins->prefixes[PPS_OSIZE] = P_O64;
            }
            break;
        }

        case 0325:
            ins->rex |= REX_NH;
            break;

        case 0326:
            break;

        case 0330:
            switch (ins->prefixes[PPS_OSIZE]) {
            case P_none:
                switch (ins->op_size) {
                case 16: goto is_o16;
                case 32: goto is_o32;
                case 64: goto is_o64;
                default: as_.panic_here();
                }
                break;
            case P_OSP:
                if (bits == 16)
                    goto is_o32;
                else
                    goto is_o16;
            case P_O16: goto is_o16;
            case P_O32: goto is_o32;
            case P_O64: goto is_o64;
            default: as_.panic_here();
            }
            break;

        case 0331:
            ins->need_pfx[PPS_REP] = PFE_NULL;
            break;

        case 0332:
            ins->need_pfx[PPS_REP] = 0xf2;
            break;

        case 0333:
            ins->need_pfx[PPS_REP] = 0xf3;
            break;

        case 0334:
            ins->rex |= REX_L;   // ignored in 64-bit mode
            break;

        case 0335:
            break;

        case 0336:
            if (!(as_.optimizing & OPTIM_STRICT_OSIZE)) opt_opsize = 16 | 32 | 64;
            break;

        case 0337:
            if (!(as_.optimizing & OPTIM_STRICT_OSIZE)) opt_opsize = 64;
            break;

        case 0340:
            // The byte code ends in 0, so opx points to operand 0.
            if (!absolute_op(opx)) {
                as_.nonfatal("attempt to reserve non-constant quantity of memory");
                return -1;
            } else if (opx->opflags & OPFLAG_FORWARD) {
                as_.warn(WARN_FORWARD, "forward reference in {} can have unpredictable results", kInsnNames[static_cast<std::size_t>(ins->opcode)]);
            }

            length += opx->offset * resb_bytes(ins->opcode);
            break;

        case 0341:
            if (!ins->prefixes[PPS_WAIT]) ins->prefixes[PPS_WAIT] = P_WAIT_;
            break;

        case 0342:
            switch (bits) {
            case 16: goto is_o16;
            case 32: goto is_o32;
            case 64: goto is_o64;
            default: as_.panic_here();
            }
            break;

        case 0344:
            ins->rex |= REX_P | REX_B;
            break;

        case 0345:
            ins->rex |= REX_P | REX_X;
            break;

        case 0346:
            ins->rex |= REX_P | REX_R;
            break;

        case 0347:
            ins->rex |= REX_P | REX_W;
            break;

        case 0350:
            ins->rex |= REX_P | REX_2;
            break;

        case 0351:
            ins->rex |= REX_P | REX_2 | REX_W;
            break;

        CB_CASE3(0355):
            // Set opmap for legacy and REX2 encodings.
            ins->vex_cm = c & 3;
            break;

        case 0360:
            ins->need_pfx[PPS_REP] = PFE_NULL;
            ins->need_pfx[PPS_OSIZE] = PFE_NULL;
            break;

        case 0361:
            ins->need_pfx[PPS_REP] = PFE_NULL;
            [[fallthrough]];
        case 0366:
            ins->need_pfx[PPS_OSIZE] = 0x66;
            break;

        case 0364:
            ins->need_pfx[PPS_OSIZE] = PFE_NULL;
            break;

        case 0365:
            ins->need_pfx[PPS_ASIZE] = PFE_NULL;
            break;

        case 0367:
            ins->need_pfx[PPS_ASIZE] = 0x67;
            break;

        case 0370:
            break;

        case 0371:
            if (ins->prefixes[PPS_REP] == P_BND) {
                // jmp short (opcode eb) cannot be used with the bnd prefix.
                ins->prefixes[PPS_REP] = P_none;
                if (!ins->dummy) as_.warn(WARN_PREFIX_BND, "jmp short does not init bnd regs - bnd prefix dropped");
            }
            break;

        case 0373:
            length++;
            break;

        case 0374:
            eat = EA_XMMVSIB;
            break;

        case 0375:
            eat = EA_YMMVSIB;
            break;

        case 0376:
            eat = EA_ZMMVSIB;
            break;

        CB_CASE4(0100):
        CB_CASE4(0110):
        CB_CASE4(0120):
        CB_CASE4(0130):
        CB_CASE4(0200):
        CB_CASE4(0204):
        CB_CASE4(0210):
        CB_CASE4(0214):
        CB_CASE4(0220):
        CB_CASE4(0224):
        CB_CASE4(0230):
        CB_CASE4(0234): {
            int rfield;
            OpFlags rflags;

            opy = get_operand(*ins, static_cast<unsigned>(op2));

            ins->ea.rex = 0;   // ensure ea.REX is initially 0

            if (c <= 0177) {
                // Pick rfield from operand b (opx).
                if (!is_register(opx->basereg)) as_.panic("invalid operand passed to regflag()");
                rflags = kRegFlags[static_cast<std::size_t>(opx->basereg)];
                rfield = kRegVals[static_cast<std::size_t>(opx->basereg)];
            } else {
                rflags = 0;
                rfield = c & 7;
                opx = nullptr;
            }

            if (itemp_has(temp, IF_MIB)) {
                opy->eaflags |= EAF_MIB;
                // If a separate form of MIB (ICC style) is used, the index reg info is merged into the mem operand.
                if (mib_index != R_none) {
                    opy->indexreg = mib_index;
                    opy->scale = 1;
                    opy->hintbase = mib_index;
                    opy->hinttype = EAH_NOTBASE;
                }
            }

            // SIB encoding required.
            if (itemp_has(temp, IF_SIB)) opy->eaflags |= EAF_SIB;

            // ea_evex_flags() must come before process_ea().
            if (ea_evex_flags(*ins, *opy)) return -1;

            if (process_ea(*opy, rfield, rflags, *ins, eat, errmsg)) {
                as_.nonfatal("{}", errmsg);
                return -1;
            }

            ins->rex |= ins->ea.rex;
            length += ins->ea.size;
        } break;

        default:
            as_.panic("internal instruction table corrupt: instruction code \\{:o} (0x{:02X}) given", c, c);
        }
    }

    if (ins->rex & REX_NH) {
        if (ins->rex & REX_H) {
            as_.nonfatal("instruction cannot use high registers");
            return -1;
        }
        ins->rex &= ~REX_P;   // don't force REX prefix due to high reg
    }

    if (ins->prefixes[PPS_OSIZE] == P_O64) ins->rex |= !(ins->rex & REX_NW) ? REX_W : 0;

    switch (ins->prefixes[PPS_REX]) {
    case P_EVEX:
        if (!(ins->rex & REX_EV)) return -1;
        break;
    case P_VEX:
    case P_VEX3:
    case P_VEX2:
        if (!(ins->rex & REX_V)) return -1;
        break;
    case P_REX:
        if (bits != 64) {
            as_.nonfatal("REX encoding not supported in {}-bit mode", bits);
            return -1;
        }
        if (ins->rex & (REX_V | REX_EV | REX_2)) return -1;
        ins->rex |= REX_P;   // force REX prefix
        break;
    case P_REX2:
        if (bits != 64) {
            as_.nonfatal("REX2 encoding not supported in {}-bit mode", bits);
            return -1;
        }
        if ((ins->rex & (REX_V | REX_EV)) || rex_highvec(ins->rex)) return -1;
        ins->rex |= REX_P | REX_2;   // force REX2 prefix
        break;
    default:
        break;
    }

    if (ins->rex & (REX_V | REX_EV)) {
        std::uint32_t bad32 = REX_BXR;

        if (ins->rex & REX_H) {
            as_.nonfatal("cannot use high byte register in this instruction");
            return -1;
        }
        if (itemp_has(temp, IF_WW)) {
            bad32 |= REX_W;
        } else {
            ins->rex = (ins->rex & ~REX_W) | ((static_cast<std::uint32_t>(ins->vex_wlp) >> (7 - 3)) & REX_W);
        }

        if (bits != 64 && ((ins->rex & bad32) || ins->vexreg > 7)) {
            as_.nonfatal("invalid operands in non-64-bit mode");
            return -1;
        }

        if (ins->rex & REX_EV) {
            // EVEX.
            length += 4;
        } else {
            // VEX.
            if (ins->vexreg > 15 || (ins->rex & REX_BXR1)) {
                as_.nonfatal("invalid high-16 register in VEX encoded instruction");
                return -1;
            }

            if (ins->vex_cm != 1 || (ins->rex & (REX_B | REX_X | REX_W)) || ins->prefixes[PPS_REX] == P_VEX3) {
                // VEX3 required.
                if (ins->prefixes[PPS_REX] == P_VEX2) as_.nonfatal("instruction not encodable with {{vex2}} prefix");
                length += 3;
            } else {
                // VEX2 available.
                length += 2;
            }
        }
    } else if (ins->rex & (REX_BXR1 | REX_2)) {
        // REX2 prefix needed.
        if (bits != 64) {
            as_.nonfatal("invalid operands in {}-bit mode", bits);
            return -1;
        }
        if (!as_.cpu.test(IF_APX)) {
            as_.nonfatal("invalid operands in non-APX mode");
            return -1;
        }
        if (itemp_has(temp, IF_NOAPX) || rex_highvec(ins->rex)) {
            as_.nonfatal("this use of registers 16-31 not supported for this instruction");
            return -1;
        }
        if (ins->rex & REX_H) {
            as_.nonfatal("cannot use high byte register in this instruction");
            return -1;
        }

        ins->rex |= REX_2 | REX_P;
        length += 2;
    } else if (ins->rex & REX_MASK) {
        if (ins->rex & REX_H) {
            as_.nonfatal("cannot use high byte register in this instruction");
            return -1;
        } else if (bits == 64) {
            ins->rex &= ~REX_L;
            ins->rex |= REX_P;
        } else if ((ins->rex & (REX_L | REX_W | REX_BXR)) == (REX_L | REX_R) && iflag_cpu_level_ok(as_.cpu, IF_X86_64)) {
            // LOCK-as-REX.R.
            if (assert_no_prefix(*ins, PPS_LOCK)) return -1;
            lockcheck = false;   // already errored, no need for warning
            ins->rex &= ~REX_P;
        } else {
            as_.nonfatal("invalid operands in {}-bit mode", bits);
            return -1;
        }
        length++;
    }

    if (!(ins->rex & (REX_2 | REX_V | REX_EV))) {
        // Opmap legacy encoding: none, 0f, 0f 38, 0f 3a.
        length += (ins->vex_cm > 0) + (ins->vex_cm > 1);
    }

    if (lockcheck && has_prefix(*ins, PPS_LOCK, P_LOCK)) {
        if ((!itemp_has(temp, IF_LOCK) || !is_class(opf::MEMORY, ins->oprs[0].type)) &&
            (!itemp_has(temp, IF_LOCK1) || !is_class(opf::MEMORY, ins->oprs[1].type))) {
            as_.warn(WARN_PREFIX_LOCK_ERROR, "instruction is not lockable");
        } else if (temp.opcode == I_XCHG) {
            as_.warn(WARN_PREFIX_LOCK_XCHG, "superfluous LOCK prefix on XCHG instruction");
        }
    }

    bad_hle_warn(*ins, hleok);

    // When the BND prefix is set by the DEFAULT directive, the BND prefix is added to every appropriate instruction line
    // unless it is overridden by the NOBND prefix.
    if (as_.globl.bnd && (itemp_has(temp, IF_BND) && !has_prefix(*ins, PPS_REP, P_NOBND))) ins->prefixes[PPS_REP] = P_BND;

    // Warn on {pt} or {pn} on a nonhintable instruction.
    if (ins->prefixes[PPS_SEG] == P_PT || ins->prefixes[PPS_SEG] == P_PN) {
        if (!itemp_has(temp, IF_JCC_HINT)) {
            as_.warn(WARN_PREFIX_HINT_DROPPED, "invalid branch hint prefix dropped");
            ins->prefixes[PPS_SEG] = 0;
        }
    }

    // Add the length of legacy prefixes.
    length += emit_prefixes(nullptr, *ins);

    return length;
}

// Emit REX and REX2 prefixes, and if necessary legacy opmap bytes (emit_rex).
void Encoder::emit_rex(OutData& data, Insn& ins) {
    std::array<std::uint8_t, 4> buf{};
    std::size_t n = 0;

    if (ins.rex_done) return;

    ins.rex_done = true;

    if (ins.rex & (REX_V | REX_EV)) return;   // handled elsewhere

    if (ins.rex & REX_2) {
        buf[0] = 0xd5;
        buf[1] = static_cast<std::uint8_t>(ins.rex & (REX_BXR0 | REX_W));
        buf[1] |= static_cast<std::uint8_t>((ins.rex & REX_BXR1) >> 8);
        buf[1] |= static_cast<std::uint8_t>(ins.vex_cm << 7);
        n = 2;
    } else {
        const std::uint8_t rex = static_cast<std::uint8_t>(ins.rex & (REX_MASK | REX_L));

        if (rex == (REX_L | REX_R)) {
            buf[n++] = 0xf0;
        } else if (rex & REX_P) {
            buf[n++] = rex;
        } else if (rex & ~REX_L) {
            as_.assert_failed("!(rex & ~REX_L)");
        }

        if (ins.vex_cm) {
            buf[n++] = 0x0f;
            if (ins.vex_cm > 1) {
                // Map 2 = 0F 38, map 3 = 0F 3A.
                buf[n++] = static_cast<std::uint8_t>(0x34 + (ins.vex_cm << 1));
            }
        }
    }

    out_rawdata(data, buf.data(), n);
}

// Emit the legacy prefixes (emit_prefixes); returns how many bytes. With no `data` only the length is wanted.
int Encoder::emit_prefixes(OutData* data, const Insn& ins) {
    const int bits = ins.bits;
    std::array<std::uint8_t, MAXPREFIX> buf{};
    int bytes = 0;

    // Do not issue warnings if data is null during the data-generation pass, or the warnings will be issued twice.
    // Warnings are still issued on previous passes, in case we terminate with an error.
    const bool do_warn = !as_.pass_final() || data;

    for (int j = 0; j < MAXPREFIX; j++) {
        const int pfx = ins.prefixes[static_cast<std::size_t>(j)];
        const int r = ins.need_pfx[static_cast<std::size_t>(j)];
        int c = prefix_byte(pfx, bits);

        if (r && r != c && !(r <= 0 && c <= 0)) {
            switch (pfx) {
            case P_none:
            case P_O16:
            case P_O32:
            case P_O64:
            case P_A16:
            case P_A32:
            case P_A64:
                // In these cases, allow the instruction to simply override.
                c = r;
                break;
            default:
                if (do_warn) as_.warn(WARN_PREFIX_INVALID, "invalid prefix {} for instruction, result may be unexpected", prefix_name(pfx));
                break;
            }
        }

        // Various warnings and error conditions.
        switch (pfx) {
        case R_CS:
        case R_DS:
            // Hintable Jcc instructions OK even in 64-bit mode.
            if (itemp_has(*ins.itemp, IF_JCC_HINT)) break;
            [[fallthrough]];
        case R_ES:
        case R_SS:
            if (do_warn && bits == 64) as_.warn(WARN_PREFIX_SEG, "{} segment override will be ignored in 64-bit mode", prefix_name(pfx));
            break;

        case P_A16:
            if (bits == 64) as_.nonfatal("16-bit addressing is not supported in 64-bit mode");
            break;

        case P_A64:
            if (bits != 64) as_.nonfatal("64-bit addressing is only supported in 64-bit mode");
            break;

        case P_O64:
            if (bits != 64) as_.nonfatal("64-bit operand size is only supported in 64-bit mode");
            break;

        default:
            switch (c) {
            case 0x66:
            case 0xf2:
            case 0xf3:
                // These prefixes are embedded in a VEX/EVEX prefix.
                if (ins.rex & (REX_V | REX_EV)) c = PFE_NULL;
                break;
            case PFE_ERR: as_.nonfatal("invalid use of {} prefix", prefix_name(pfx)); break;
            case PFE_WHAT: as_.nonfatal("{} cannot be used as a prefix", prefix_name(pfx)); break;
            default: break;
            }
        }

        if (c >= 0) buf[static_cast<std::size_t>(bytes++)] = static_cast<std::uint8_t>(c);
    }

    if (data && bytes) out_rawdata(*data, buf.data(), static_cast<std::size_t>(bytes));

    return bytes;
}

void Encoder::gencode(OutData& data, Insn& ins_) {
    Insn* const ins = &ins_;
    std::uint8_t c;
    std::array<std::uint8_t, 4> bytes{};
    std::int64_t size;
    int op1, op2;
    Operand *opx, *opy;
    const std::uint8_t* codes = ins->itemp->code;
    std::uint8_t opex = 0;
    int r;
    const int bits = ins->bits;

    data.itemp = ins->itemp;
    data.bits = ins->bits;

    ins->rex_done = false;

    emit_prefixes(&data, *ins);

    while (*codes) {
        c = *codes++;
        op1 = (c & 3) + ((opex & 1) << 2);
        opx = get_operand(*ins, static_cast<unsigned>(op1));
        op2 = ((c >> 3) & 3) + ((opex & 2) << 1);
        opy = nullptr;
        opex = 0;   // for the next iteration

        switch (c) {
        case 01:
        case 02:
        case 03:
        case 04:
            emit_rex(data, *ins);
            out_rawdata(data, codes, c);
            codes += c;
            break;

        case 05:
        case 06:
        case 07:
            opex = c;
            break;

        CB_CASE4(010): {
            emit_rex(data, *ins);
            if (opx->type & opf::IMMEDIATE) {
                r = static_cast<std::int32_t>(opx->offset);
            } else {
                if (!is_register(opx->basereg)) as_.panic("invalid operand passed to regval()");
                r = kRegVals[static_cast<std::size_t>(opx->basereg)];
            }
            out_rawbyte(data, static_cast<std::uint8_t>(*codes++ + (r & 7)));
            break;
        }

        CB_CASE4(014):
            break;

        CB_CASE4(020):
            out_imm(data, *opx, 1, OUT_WRAP);
            break;

        CB_CASE4(024):
            out_imm(data, *opx, 1, OUT_UNSIGNED);
            break;

        CB_CASE4(030):
            out_imm(data, *opx, 2, OUT_WRAP);
            break;

        CB_CASE4(034):
            if (opx->type & (opf::BITS16 | opf::BITS32))
                size = (opx->type & opf::BITS16) ? 2 : 4;
            else
                size = (ins->op_size == 16) ? 2 : 4;
            out_imm(data, *opx, static_cast<int>(size), OUT_WRAP);
            break;

        CB_CASE4(040):
            out_imm(data, *opx, 4, OUT_WRAP);
            break;

        CB_CASE4(044):
            size = ins->addr_size >> 3;
            out_imm(data, *opx, static_cast<int>(size), OUT_WRAP);
            break;

        CB_CASE4(050):
            if (opx->segment == data.loc.segment) {
                const std::int64_t delta = sext(opx->offset - data.loc.offset - (data.inslen - data.insoffs), ins->op_size);
                if (delta != static_cast<std::int8_t>(delta)) as_.nonfatal("short jump is out of range");
            }
            out_reladdr(data, *opx, 1);
            break;

        CB_CASE4(054):
            out_imm(data, *opx, 8, OUT_WRAP);
            break;

        CB_CASE4(060):
            out_reladdr(data, *opx, 2);
            break;

        CB_CASE4(064):
            if (opx->type & (opf::BITS16 | opf::BITS32 | opf::BITS64))
                size = (opx->type & opf::BITS16) ? 2 : 4;
            else
                size = (ins->op_size == 16) ? 2 : 4;

            out_reladdr(data, *opx, static_cast<int>(size));
            break;

        CB_CASE4(070):
            out_reladdr(data, *opx, 4);
            break;

        CB_CASE4(074):
            if (opx->segment == NO_SEG) as_.nonfatal("value referenced by FAR is not relocatable");
            out_farseg(data, *opx);
            break;

        case 0171:
            c = *codes++;
            op2 = (op2 & ~3) | ((c >> 3) & 3);
            opx = &ins->oprs[static_cast<std::size_t>(op2)];
            r = kRegVals[static_cast<std::size_t>(opx->basereg)];
            c = static_cast<std::uint8_t>((c & ~070) | ((r & 7) << 3));
            out_rawbyte(data, c);
            break;

        case 0172: {
            const int mask = ins->prefixes[PPS_REX] == P_EVEX ? 7 : 15;
            const Operand* opz;

            c = *codes++;
            opx = get_operand(*ins, static_cast<unsigned>(op1 = (c >> 3) & 7));
            opz = get_operand(*ins, static_cast<unsigned>(op2 = c & 7));
            if (!absolute_op(opz))
                as_.nonfatal("non-absolute expression not permitted as argument {}", op2);
            else if (opz->offset & ~static_cast<std::int64_t>(mask))
                as_.warn(WARN_NUMBER_OVERFLOW, "is4 argument exceeds bounds");
            c = static_cast<std::uint8_t>(opz->offset & mask);
            goto emit_is4;
        }

        case 0173:
            c = *codes++;
            opx = get_operand(*ins, static_cast<unsigned>(op1 = c >> 4));
            c &= 15;
            goto emit_is4;

        CB_CASE4(0174):
            c = 0;
        emit_is4:
            r = kRegVals[static_cast<std::size_t>(opx->basereg)];
            out_rawbyte(data, static_cast<std::uint8_t>((r << 4) | ((r & 0x10) >> 1) | c));
            break;

        CB_CASE4(0240):
        case 0250: {
            const std::uint8_t vregbits = ins->vexreg | ins->veximm;

            codes += 4;
            ins->evex ^= evexflags(ins->oprs[0].decoflags, EVEX_Z.mask | EVEX_AAA.mask);
            ins->evex ^= (ins->rex & REX_BXR0) << 13;   // R0 X0 B0
            if (ins->rex & REX_B1) ins->evex ^= (ins->rex & REX_BV) ? EVEX_X3.mask : EVEX_B4.mask;
            if (ins->rex & REX_X1) ins->evex ^= (ins->rex & REX_XV) ? EVEX_V4.mask : EVEX_X4.mask;
            if (ins->rex & REX_R1) ins->evex ^= EVEX_R4.mask;
            if (ins->rex & REX_W) ins->evex |= EVEX_W.mask;   // OR is correct here
            ins->evex ^= static_cast<std::uint32_t>(vregbits & 15) << 19;
            ins->evex ^= static_cast<std::uint32_t>(vregbits & 16) << (27 - 4);
            if (ins->prefixes[PPS_NF] == P_NF) {
                // Set NF if {nd} and this is applicable.
                if (itemp_has(*ins->itemp, IF_NF_E)) ins->evex ^= EVEX_NF.mask;

                // Clear NF if the instruction forbids it.
                if (itemp_has(*ins->itemp, IF_NF_N)) as_.warn(WARN_OTHER, "this instruction doesn't allow {{nf}}");
            }
            // Set ND if {zu} and this is applicable.
            if (ins->prefixes[PPS_ZU] == P_ZU) {
                if (itemp_has(*ins->itemp, IF_ZU_E)) ins->evex ^= EVEX_ND.mask;
            }
            out_rawdword(data, ins->evex);
            break;
        }

        CB_CASE4(0254):
            out_imm(data, *opx, 4, OUT_SIGNED);
            break;

        CB_CASE4(0264):
            out_imm(data, *opx, 4, OUT_UNSIGNED);
            break;

        CB_CASE4(0260):
        case 0270:
            codes += 2;
            if (ins->vex_cm != 1 || (ins->rex & (REX_W | REX_X | REX_B)) || ins->prefixes[PPS_REX] == P_VEX3) {
                bytes[0] = (ins->vex_cm >> 6) ? 0x8f : 0xc4;
                bytes[1] = static_cast<std::uint8_t>((ins->vex_cm & 31) | ((~ins->rex & REX_BXR0) << 5));
                bytes[2] = static_cast<std::uint8_t>(((ins->rex & REX_W) << (7 - 3)) | ((~static_cast<unsigned>(ins->vexreg) & 15) << 3) |
                                                      (ins->vex_wlp & 07));
                out_rawdata(data, bytes.data(), 3);
            } else {
                bytes[0] = 0xc5;
                bytes[1] = static_cast<std::uint8_t>(((~ins->rex & REX_R) << (7 - 2)) | ((~static_cast<unsigned>(ins->vexreg) & 15) << 3) |
                                                      (ins->vex_wlp & 07));
                out_rawdata(data, bytes.data(), 2);
            }
            break;

        case 0271:
        case 0272:
        case 0273:
            break;

        CB_CASE4(0274): {
            if (absolute_op(opx)) {
                const int s = ins->op_size;

                const std::uint64_t um = std::uint64_t{2} << (s - 1);
                const std::uint64_t uv = static_cast<std::uint64_t>(opx->offset);

                if (uv > 127 && uv < static_cast<std::uint64_t>(-128) && (uv < um - 128 || uv > um - 1)) {
                    // If this wasn't explicitly byte-sized, warn as though we had fallen through to the imm16/32/64 case.
                    as_.warn(WARN_NUMBER_OVERFLOW | ERR_PASS2, "{} exceeds bounds",
                             (opx->type & opf::BITS8) ? "signed byte" : s == 16 ? "word" : s == 32 ? "dword" : "signed dword");
                }

                // Output as a raw byte to avoid the byte overflow check.
                out_rawbyte(data, static_cast<std::uint8_t>(uv));
            } else {
                out_imm(data, *opx, 1, OUT_WRAP);   // XXX (NASM): OUT_SIGNED?
            }
            break;
        }

        CB_CASE4(0300): {
            emit_rex(data, *ins);
            if (absolute_op(opx)) {
                if (!is_hint_nop(static_cast<std::uint64_t>(opx->offset))) as_.warn(WARN_NONE, "not a valid hint-NOP opcode (0D, 18-1F)");
                out_rawbyte(data, static_cast<std::uint8_t>(opx->offset));
            } else {
                out_imm(data, *opx, 1, OUT_UNSIGNED);
            }
            break;
        }

        CB_CASE4(0304): {
            const std::uint8_t type = *codes++;
            const std::uint8_t modifier = *codes++;
            const int flags = type & 3;   // signed or unsigned

            if (type > 2) as_.assert_failed("type <= 2");
            if (!absolute_op(opx)) as_.assert_failed("absolute_op(opx)");

            warn_overflow_out(opx->offset, 1, flags, "byte");
            out_rawbyte(data, static_cast<std::uint8_t>(opx->offset ^ modifier));
            break;
        }

        case 0310:
        case 0311:
            break;

        case 0312:
            break;

        case 0313:
            break;

        CB_CASE4(0314):
            break;

        case 0320:
        case 0321:
        case 0322:
        case 0323:
        case 0324:
        case 0327:
        case 0342:
            break;

        case 0325:
            break;

        case 0326:
            break;

        case 0330:
        case 0331:
        case 0332:
        case 0333:
            break;

        case 0334:
            break;

        case 0335:
            break;

        case 0336:
        case 0337:
            break;

        case 0340:
            if (opx->segment != NO_SEG) as_.panic("non-constant BSS size in pass two");

            out_reserve(data, static_cast<std::uint64_t>(ins->oprs[0].offset * resb_bytes(ins->opcode)));
            break;

        case 0341:
            break;

        CB_CASE4(0344):
            break;

        case 0350:
        case 0351:
            break;

        CB_CASE3(0355):
            break;

        case 0360:
            break;

        case 0361:
            break;

        case 0364:
        case 0365:
            break;

        case 0366:
        case 0367:
            break;

        CB_CASE3(0370):
            break;

        case 0373:
            out_rawbyte(data, bits == 16 ? 3 : 5);
            break;

        case 0374:
        case 0375:
        case 0376:
            break;

        CB_CASE4(0100):
        CB_CASE4(0110):
        CB_CASE4(0120):
        CB_CASE4(0130):
        CB_CASE4(0200):
        CB_CASE4(0204):
        CB_CASE4(0210):
        CB_CASE4(0214):
        CB_CASE4(0220):
        CB_CASE4(0224):
        CB_CASE4(0230):
        CB_CASE4(0234): {
            std::array<std::uint8_t, 4> modrm{};
            std::size_t n = 0;
            bool overflow;

            opy = get_operand(*ins, static_cast<unsigned>(op2));

            modrm[n++] = ins->ea.modrm;
            if (ins->ea.sib_present) modrm[n++] = ins->ea.sib;
            out_rawdata(data, modrm.data(), n);

            // Make sure the address gets the right offset in case the line breaks in the .lst file (BR 1197827).
            switch (ins->ea.bytes) {
            case 0:
                // No displacement.
                overflow = false;
                break;
            case 1:
                // 8-bit displacement, which may be compressed.
                out_rawbyte(data, static_cast<std::uint8_t>(ins->ea.disp8));
                overflow = !ins->ea.disp8_ok;
                break;
            default:
                if (ins->ea.rip) {
                    out_reladdr(data, *opy, ins->ea.bytes);
                    overflow = false;
                } else {
                    const unsigned asize = static_cast<unsigned>(ins->addr_size >> 3);

                    out_imm(data, *opy, ins->ea.bytes, (asize > ins->ea.bytes) ? OUT_SIGNED | OUT_NOWARN : OUT_WRAP | OUT_NOWARN);

                    overflow = overflow_general(opy->offset, asize) ||
                               sext(opy->offset, static_cast<unsigned>(ins->addr_size)) != sext(opy->offset, static_cast<unsigned>(ins->ea.bytes) << 3);
                }
            }

            if (overflow) warn_overflow(ins->ea.bytes, nullptr, "displacement");
        } break;

        default:
            as_.panic("internal instruction table corrupt: instruction code \\{:o} (0x{:02X}) given", c, c);
        }
    }
}

#undef CB_CASE3
#undef CB_CASE4

// MASM encoding preference (--masm): for a two-register ALU/MOV instruction MASM emits the "reg, r/m" (direction bit = 1)
// form -- the first operand goes in the ModRM reg field -- whereas NASM defaults to the "r/m, reg" form. Both encode the
// same operation, so this only changes the emitted bytes, never the semantics; it is what makes reg,reg bodies
// byte-identical to ML output.
//
// When find_match settled on the r/m,reg orientation for an all-register instruction, switch to the reg,r/m counterpart
// template if the opcode has one that also matches. The r/m operand slot is the template operand lacking the REGISTER bit
// (RM_GPR); the pure-register slot (REG_GPR) has it.
const ItemTemplate* Encoder::masm_regdir_pref(const TemplateList& list, const ItemTemplate* best, const Insn& ins, int& besti) {
    if (ins.operands != 2) return best;
    if (!(ins.oprs[0].type & opf::REGISTER) || !(ins.oprs[1].type & opf::REGISTER)) return best;
    // best is r/m,reg? opd[0] is the r/m slot (no REGISTER), opd[1] the reg.
    if ((best->opd[0] & opf::REGISTER) || !(best->opd[1] & opf::REGISTER)) return best;

    const ItemTemplate* t = list.temp;
    for (unsigned i = 0; i < list.ntemp; i++, t++) {
        if (t == best || t->operands != 2) continue;
        // The counterpart is reg,r/m: opd[0] the reg slot, opd[1] the r/m slot.
        if (!(t->opd[0] & opf::REGISTER) || (t->opd[1] & opf::REGISTER)) continue;
        if (matches(*t, ins) == MOK_GOOD) {
            besti = static_cast<int>(i);
            return t;
        }
    }
    return best;
}

// MASM encoding preference (--masm): for a 16-bit `<alu> AX, imm' ML emits the accumulator-special opcode with a 16-bit
// immediate (AND AX,imm -> 25 iw, etc). That is the same length as NASM's sign-extended-imm8 group form (83 /r ib) -- and
// ML picks the accumulator on that tie. (For 8-bit the accumulator form is already shorter and for 32-bit the imm8 group
// form is shorter, so both assemblers agree; only the 16-bit accumulator ties.) When find_match settled on the group r/m,imm
// form for a 16-bit accumulator destination, switch to the accumulator template. matches() confirms the operand really is AX.
const ItemTemplate* Encoder::masm_accum_imm_pref(const TemplateList& list, const ItemTemplate* best, const Insn& ins, int& besti) {
    if (ins.operands != 2) return best;
    if (!(ins.oprs[1].type & opf::IMMEDIATE)) return best;
    // best is the group r/m,imm form (opd[0] the r/m slot), 16-bit only.
    if (best->opd[0] & opf::REGISTER) return best;
    if (!(best->opd[0] & opf::BITS16)) return best;

    const ItemTemplate* t = list.temp;
    for (unsigned i = 0; i < list.ntemp; i++, t++) {
        if (t == best || t->operands != 2) continue;
        // Accumulator form: opd[0] is a specific register, not an r/m slot.
        if (!(t->opd[0] & opf::REGISTER)) continue;
        if (matches(*t, ins) == MOK_GOOD) {
            besti = static_cast<int>(i);
            return t;
        }
    }
    return best;
}

MatchResult Encoder::find_match(Insn& ins) {
    const int bits = ins.bits;
    const int rex = ins.prefixes[PPS_REX];
    int besti;
    const ItemTemplate* best = nullptr;

    ins.itemp = nullptr;
    ins.itempindex = besti = -1;

    // Impossible encoding request?
    if (bits != 64) {
        if (rex == P_REX || rex == P_REX2) return MERR_ENCMISMATCH;
    }

    MatchResult merr = MERR_INVALOP;

    const TemplateList templist = templates_of(ins.opcode);
    const ItemTemplate* temp = templist.temp;

    for (unsigned i = 0; i < templist.ntemp; i++) {
        const MatchResult m = matches(*temp, ins);
        if (m > merr) {
            best = temp;
            besti = static_cast<int>(i);
            merr = m;
            if (merr == MOK_GOOD) break;
        }
        temp++;
    }

    if (merr >= MOK_FIRST) {
        // If this was a fuzzy match it is confirmed now.
        merr = MOK_GOOD;
        if (as_.masm_mode) {
            best = masm_regdir_pref(templist, best, ins, besti);
            best = masm_accum_imm_pref(templist, best, ins, besti);
        }
        ins.itemp = best;
        ins.itempindex = besti;
    }
    return merr;
}

MatchResult Encoder::matches(const ItemTemplate& itemp, const Insn& ins) {
    const int bits = ins.bits;
    const int oprs = ins.operands;
    std::uint32_t arflag;
    std::uint32_t armask, smmask;
    bool if_anysize, if_sx;
    int op_size;
    OpFlags opsize, arsize, smsize;
    std::array<OpFlags, MAX_OPERANDS> isize{};   // adjusted instruction operand sizes
    std::array<OpFlags, MAX_OPERANDS> tsize{};   // adjusted template operand sizes
    std::array<OpFlags, MAX_OPERANDS> itype{};   // adjusted instruction flags
    bool sm_missing;

    // Jump size/type declarators are more like types than sizes.
    constexpr OpFlags jsize_mask = opf::NEAR_ | opf::FAR_ | opf::SHORT | opf::ABS_;
    constexpr OpFlags msize_mask = SIZE_MASK & ~jsize_mask;

    // Check the opcode.
    if (itemp.opcode != ins.opcode) return MERR_INVALOP;

    // Count the operands.
    if (itemp.operands != oprs) return MERR_INVALOP;

    // Is it legal?
    if (ins.opt & OPTIM_STRICT_INSTR) {
        if (itemp_has(itemp, IF_OPT)) return MERR_INVALOP;
    }

    // {rex/vexn/evex} available?
    switch (ins.prefixes[PPS_REX]) {
    case P_EVEX:
        if (!itemp_has(itemp, IF_EVEX)) return MERR_ENCMISMATCH;
        break;
    case P_VEX:
    case P_VEX3:
    case P_VEX2:
        if (!itemp_has(itemp, IF_VEX)) return MERR_ENCMISMATCH;
        break;
    case P_REX:
        if (bits != 64 || itemp_has(itemp, IF_NOREX) || itemp_has(itemp, IF_VEX) || itemp_has(itemp, IF_EVEX) || itemp_has(itemp, IF_REX2))
            return MERR_ENCMISMATCH;
        break;
    case P_REX2:
        if (bits != 64 || itemp_has(itemp, IF_NOAPX) || itemp_has(itemp, IF_EVEX)) return MERR_ENCMISMATCH;
        break;
    default:
        if (itemp_has(itemp, IF_EVEX)) {
            if (!as_.cpu.test(IF_EVEX)) return MERR_BADCPU;
        } else if (itemp_has(itemp, IF_VEX)) {
            if (!as_.cpu.test(IF_VEX)) {
                return MERR_BADCPU;
            } else if (itemp_has(itemp, IF_LATEVEX)) {
                if (!as_.cpu.test(IF_LATEVEX) && as_.cpu.test(IF_EVEX)) return MERR_BADCPU;
            } else if (itemp_has(itemp, IF_REX2)) {
                if (bits != 64 || !as_.cpu.test(IF_APX)) return MERR_BADCPU;
            }
        }
        break;
    }

    // First, cursory operand filtering and initialize itype[] and isize[].
    for (int i = 0; i < oprs; i++) {
        const std::size_t k = static_cast<std::size_t>(i);
        const Operand& op = ins.oprs[k];
        const OpFlags ttype = itemp.opd[k];

        isize[k] = op.type & msize_mask;
        itype[k] = op.type - isize[k];
        if (op.type & ~ttype & (opf::COLON | opf::TO)) return MERR_INVALOP;
        if (op.iflag && !itemp_has(itemp, static_cast<unsigned>(op.iflag))) return MERR_WRONGIMM;
    }

    // "Default" operand size (from mode and prefixes only).
    op_size = ins.op_size;
    if (bits == 64 && itemp_has(itemp, IF_NWSIZE) && op_size == 32) {
        // If this is an nw instruction, default to 64 bits in 64-bit mode.
        op_size = bits;
    }
    opsize = mode_to_op(static_cast<unsigned>(op_size));

    // Some key flags.
    if_anysize = itemp_has(itemp, IF_ANYSIZE);
    if_sx = itemp_has(itemp, IF_SX);

    // Compare various operand flags that don't depend on sizes, and compute the "true" template operand sizes.
    for (int i = 0; i < oprs; i++) {
        const std::size_t k = static_cast<std::size_t>(i);
        const OpFlags ttype = itemp.opd[k];
        const DecoFlags decof = ins.oprs[k].decoflags;
        const DecoFlags ideco = itemp.deco[k];
        const bool is_broadcast = (decof & deco::BRDCAST_MASK) != 0;

        if (!is_broadcast) {
            tsize[k] = ttype & msize_mask;
        } else {
            const DecoFlags ideco_brsize = ideco & deco::BRSIZE_MASK;

            if (~ideco & deco::BRDCAST_MASK) return MERR_BRNOTHERE;

            // When broadcasting, the element size depends on the instruction type. The decorator flag should match.
            if (ideco_brsize)
                tsize[k] = brsize_to_size(ideco_brsize);
            else
                tsize[k] = 0;   // is this even possible?
        }

        // Handle implied SHORT or NEAR.
        if (ttype & (opf::NEAR_ | opf::SHORT)) {
            // Treat BYTE as an alias for SHORT, ignoring size.
            if (isize[k] == opf::BITS8) {
                itype[k] |= opf::SHORT;
                isize[k] = 0;
            }
            // An explicit SHORT or BITS8 cancels NEAR; are synonyms.
            if (itype[k] & opf::SHORT) itype[k] &= ~opf::NEAR_;
            // NEAR is implicit unless otherwise specified.
            if (!(itype[k] & (opf::FAR_ | opf::SHORT))) itype[k] |= ttype & opf::NEAR_;
            if ((ttype & (opf::NEAR_ | opf::SHORT)) == (opf::NEAR_ | opf::SHORT)) {
                // Only a short form exists; this is specially coded.
                if (!(itype[k] & (opf::FAR_ | opf::ABS_))) itype[k] |= opf::NEAR_ | opf::SHORT;
            }
        }

        // Check to see if we need to coerce an explicitly sized immediate.
        if (itype[k] & ttype & opf::IMMEDIATE) {
            // If this is an *explicitly* sized immediate, allow it to match an extending pattern.
            if (isize[k] == opf::BITS8) {
                if (ttype & opf::BYTEEXTMASK) {
                    isize[k] = tsize[k];
                    itype[k] |= opf::BYTEEXTMASK;
                }
            } else if (isize[k] == opf::BITS32) {
                if (ttype & opf::DWORDEXTMASK) isize[k] = tsize[k];
            }

            // MOST instructions which take an sdword64 are the only form; set the SDWORD flag to be strict about sdword64
            // matching (use when a true imm64 pattern exists).
            if (!itemp_has(itemp, IF_SDWORD)) itype[k] |= opf::SDWORD;
        }

        if (ttype & ~itype[k] & ~(msize_mask | REGSET_MASK)) return MERR_INVALOP;

        if (~ideco & decof & deco::OPMASK_MASK) return MERR_MASKNOTHERE;

        if (~ideco & decof & (deco::Z_MASK | deco::STATICRND_MASK | deco::SAE_MASK)) return MERR_DECONOTHERE;

        if (~ttype & itype[k] & REGSET_MASK) return (ttype & REGSET_MASK) ? MERR_REGSETSIZE : MERR_REGSET;
    }

    // Process the operand sizes.
    arflag = itemp_smask(itemp);

    // (NASM computes this in 32 bits: the true-size flags shifted up to the BITS* positions fall out of the word, so
    // only an OSIZE or ASIZE flag gives the ARx operands a default size. Kept as it is.)
    arsize = static_cast<OpFlags>(static_cast<std::uint32_t>((arflag & IF_TSMASK) << (SIZE_SHIFT - IF_SB)));

    if (arflag & IFM_OSIZE)
        arsize = opsize;
    else if (arflag & IFM_ASIZE)
        arsize = mode_to_op(ins.addr_size);

    // Flags for which the AR and SM flags apply.
    armask = itemp_arx(itemp);
    smmask = itemp_smx(itemp);

    // Apply ARx and register default sizes.
    if (!if_sx) {
        for (int i = 0; i < oprs; i++) {
            const std::size_t k = static_cast<std::size_t>(i);
            const unsigned bit = 1U << i;

            if (!isize[k]) {
                if (is_class(opf::REGISTER, itype[k]) && tsize[k])
                    isize[k] = tsize[k];
                else if (armask & bit)
                    isize[k] = arsize;
            }
        }
    }

    // Look the common subset for the size-matched operands. However, defer the error messages to until after the final
    // operand checking loop, to get a better order of message priorities.
    smsize = msize_mask;
    sm_missing = false;
    if (smmask) {
        unsigned nosizemask = 0;
        for (int i = 0; i < oprs; i++) {
            const std::size_t k = static_cast<std::size_t>(i);
            const unsigned bit = 1U << i;
            if (isize[k]) {
                if (smmask & bit) smsize &= isize[k];
            } else if (tsize[k] && !is_class(opf::REGISTER, itype[k])) {
                nosizemask |= bit;
            }
        }
        sm_missing = !(smmask & ~nosizemask);
    }

    // Check that the operand sizes actually match, and look for other kinds of mismatches, e.g. REG_HIGH when REX is
    // specified.
    for (int i = 0; i < oprs; i++) {
        const std::size_t k = static_cast<std::size_t>(i);
        const OpFlags type = itype[k];
        const DecoFlags decof = ins.oprs[k].decoflags;
        const DecoFlags ideco = itemp.deco[k];
        const bool is_broadcast = (decof & deco::BRDCAST_MASK) != 0;
        const bool has_ar = ((armask >> i) & 1) != 0;
        const bool has_sm = ((smmask >> i) & 1) != 0;
        const bool is_reg = is_class(opf::REGISTER, type);

        // Operand sizes are considered matching at this stage if any one of these is true:
        //
        // 1. The template size is undefined. XXX (NASM): apply ARx|Sx flags here?
        // 2. The operand size matches the template size.
        // 3. There is an ARx|ANYSIZE flag in the template.
        // 4. The operand size is unspecified and the template does not carry an ARx|SX flag.
        if (tsize[k]) {
            if (isize[k] != tsize[k]) {
                if (has_ar) {
                    if (if_anysize) goto isize_ok;
                    if (if_sx && !is_reg) return MERR_OPSIZEINVAL;
                }

                if (isize[k]) return is_reg ? MERR_INVALOP : MERR_OPSIZEINVAL;
            }

        isize_ok:
            if (has_sm) smsize &= tsize[k];
        } else {
            // SX with an unsized operand means only an unsized operand is OK.
            if (if_sx && has_ar)
                if (isize[k]) return MERR_OPSIZEINVAL;
        }

        if (is_class(opf::REG_HIGH, type) && ins.prefixes[PPS_REX]) {
            // High registers cannot be combined with any REX type.
            return MERR_INVALOP;
        }

        if (is_broadcast) {
            // Calculate the proper number: {1to<brcast_num>}.
            const DecoFlags ideco_brsize = ideco & deco::BRSIZE_MASK;
            unsigned brcast_num = 0;

            if (ideco_brsize) brcast_num = get_broadcast_num(itemp.opd[k], tsize[k]);

            if (brcast_num != (2U << ((decof & deco::BRNUM_MASK) >> deco::BRNUM_SHIFT))) {
                // Broadcasting opsize matches but the number of repeated memory elements does not match. If a 64b double
                // precision float is broadcast to ymm (256b), the broadcasting decorator must be {1to4}.
                return MERR_BRNUMMISMATCH;
            }
        }
    }

    // Make sure that our size match set, if any, did not get exhausted, and contains exactly one valid combination still...
    if (smmask) {
        if (!smsize)
            return MERR_OPSIZEMISMATCH;
        else if (!is_power2(smsize) || (if_sx && sm_missing))
            return MERR_OPSIZEMISSING;
    }

    // Check the template is okay at the set cpu level.
    if (iflag_cmp_cpu_level(itemp_flags(itemp), as_.cpu) > 0) return MERR_BADCPU;

    // Verify the appropriate long mode flag.
    if (itemp_has(itemp, bits == 64 ? IF_NOLONG : IF_LONG)) return MERR_BADMODE;

    // If we have a HLE prefix, look for the NOHLE flag.
    if (itemp_has(itemp, IF_NOHLE) && (has_prefix(ins, PPS_REP, P_XACQUIRE) || has_prefix(ins, PPS_REP, P_XRELEASE))) return MERR_BADHLE;

    // {nf} or {zu} prefixes used? Must be permitted.
    if (has_prefix(ins, PPS_NF, P_NF)) {
        if (!itemp_has(itemp, IF_NF) && (itemp_has(itemp, IF_FL) || (ins.opt & OPTIM_STRICT_INSTR))) return MERR_BADNF;
    } else if (itemp_has(itemp, IF_NF_R)) {
        return MERR_REQNF;
    }

    if (has_prefix(ins, PPS_ZU, P_ZU)) {
        if (!itemp_has(itemp, IF_ZU))
            return MERR_BADZU;
        else if (!(ins.oprs[0].type & opf::REGISTER))
            return MERR_MEMZU;
    }

    // Check if the BND prefix is allowed. The other 0xF2 (REPNE/REPNZ) prefix is prohibited.
    if (!itemp_has(itemp, IF_BND) && (has_prefix(ins, PPS_REP, P_BND) || has_prefix(ins, PPS_REP, P_NOBND)))
        return MERR_BADBND;
    else if (itemp_has(itemp, IF_BND) && (has_prefix(ins, PPS_REP, P_REPNE) || has_prefix(ins, PPS_REP, P_REPNZ)))
        return MERR_BADREPNE;

    // Check if special handling is needed for a relaxable jump.
    if (itemp_has(itemp, IF_JMP_RELAX)) return jmp_match(ins, itemp);

    return MOK_GOOD;
}

// Select the mod part of modr/m for a memory operand with displacement. zerook should be clear for the forbidden BP
// encodings; such instructions must be coded with a +0 displacement.
//
// 0 = no displacement, 1 = 8-bit displacement, 2 = 16/32-bit displacement.
unsigned Encoder::memory_mod(int eaflags, Insn& ins, std::int64_t o, bool known, bool zerook) {
    EaData& output = ins.ea;

    // Explicitly requested by user.
    if (eaflags & EAF_WORDOFFS) return 2;

    output.disp8_shift = static_cast<std::uint8_t>(get_disp8_shift(ins));
    output.disp8 = static_cast<std::int8_t>(o >> output.disp8_shift);
    output.disp8_ok = known && (static_cast<std::int64_t>(output.disp8) << output.disp8_shift) == sext(o, static_cast<unsigned>(ins.addr_size));

    // Explicitly requested by user.
    if (eaflags & EAF_BYTEOFFS) return 1;

    // Not knowable at this time.
    if (!known) return 2;

    // No displacement needed?
    if (o == 0 && zerook) return 0;

    // 8-bit displacement possible?
    return output.disp8_ok ? 1 : 2;
}

// Process an effective address (the memory operand or the register in the r/m field) into the ModRM/SIB bytes and the
// displacement size. Returns 0, or -1 with `errmsg` set.
int Encoder::process_ea(Operand& input, int rfield, OpFlags rflags, Insn& ins, EaType expected, std::string& errmsg) {
    EaData& output = ins.ea;
    const bool long_mode = ins.bits == 64;
    const int addrbits = ins.addr_size;
    const int eaflags = input.eaflags;

    errmsg.clear();

    output = EaData{};
    output.type = EA_SCALAR;

    // REX flags for the rfield operand.
    output.rex |= rexflags(rfield, rflags, REX_rR);

    if (is_class(opf::REGISTER, input.type)) {
        // It's a direct register.
        if (!is_register(input.basereg)) goto err;

        if (!is_reg_class(opf::REG_EA, input.basereg)) goto err;

        // Broadcasting is not available with a direct register operand.
        if (input.decoflags & deco::BRDCAST_MASK) {
            errmsg = "broadcast not allowed with register operand";
            goto err;
        }

        output.rex |= rexflags(kRegVals[static_cast<std::size_t>(input.basereg)], kRegFlags[static_cast<std::size_t>(input.basereg)], REX_rB);
        output.sib_present = false;   // no SIB necessary
        output.bytes = 0;             // no offset necessary either
        output.modrm = static_cast<std::uint8_t>(GEN_MODRM(3, rfield, kRegVals[static_cast<std::size_t>(input.basereg)]));
    } else {
        // It's a memory reference.

        // Embedded rounding or SAE is not available with a mem ref operand.
        if (input.decoflags & (deco::ER | deco::SAE)) {
            errmsg = "embedded rounding is available only with register-register operations";
            goto err;
        }

        if (input.basereg == -1 && (input.indexreg == -1 || input.scale == 0)) {
            // It's a pure offset. If it is an IMMEDIATE, it is a pattern in insns.dat which allows an immediate to be
            // used as a memory address, in which case apply the default REL/ABS.
            if (long_mode) {
                if (is_class(opf::IMMEDIATE, input.type)) {
                    if (!(input.eaflags & EAF_ABS) && ((input.eaflags & EAF_REL) || as_.globl.rel)) input.type |= opf::IP_REL;
                }
                if ((input.type & opf::IP_REL) == opf::IP_REL) {
                    if (!as_.pass_first() && absolute_op(&input)) {
                        as_.warn(WARN_EA_ABSOLUTE, "absolute address can not be RIP-relative");
                        input.type &= ~opf::IP_REL;
                        input.type |= opf::MEMORY;
                    }
                }
            }

            if (long_mode && !(opf::IP_REL & ~input.type) && (eaflags & EAF_SIB)) {
                errmsg = "instruction requires SIB encoding, cannot be RIP-relative";
                goto err;
            }

            if ((eaflags & EAF_BYTEOFFS) || ((eaflags & EAF_WORDOFFS) && input.disp_size != (addrbits != 16 ? 32 : 16))) {
                as_.warn(WARN_EA_DISPSIZE, "displacement size ignored on absolute address");
            }

            if ((eaflags & EAF_SIB) || (long_mode && (~input.type & opf::IP_REL))) {
                output.sib_present = true;
                output.sib = static_cast<std::uint8_t>(GEN_SIB(0, 4, 5));
                output.bytes = 4;
                output.modrm = static_cast<std::uint8_t>(GEN_MODRM(0, rfield, 4));
                output.rip = false;
            } else {
                output.sib_present = false;
                output.bytes = addrbits != 16 ? 4 : 2;
                output.modrm = static_cast<std::uint8_t>(GEN_MODRM(0, rfield, addrbits != 16 ? 5 : 6));
                output.rip = long_mode;
            }
        } else {
            // It's an indirection.
            int i = input.indexreg, b = input.basereg, s = input.scale;
            const bool known = !(input.opflags & OPFLAG_UNKNOWN) && (input.segment == NO_SEG);
            const int hb = input.hintbase;
            const int ht = input.hinttype;
            int t, it, bt;        // register numbers
            OpFlags x, ix, bx;    // register flags

            if (s == 0) i = -1;   // make this easy, at least

            if (is_register(i)) {
                it = kRegVals[static_cast<std::size_t>(i)];
                ix = kRegFlags[static_cast<std::size_t>(i)];
            } else {
                it = -1;
                ix = 0;
            }

            if (is_register(b)) {
                bt = kRegVals[static_cast<std::size_t>(b)];
                bx = kRegFlags[static_cast<std::size_t>(b)];
            } else {
                bt = -1;
                bx = 0;
            }

            // If either one is a vector register...
            if ((ix | bx) & (opf::XMMREG | opf::YMMREG | opf::ZMMREG) & ~opf::REG_EA) {
                OpFlags sok = opf::BITS32 | opf::BITS64;
                const std::int32_t o = static_cast<std::int32_t>(input.offset);
                int mod, scale, index, base;

                // For a vector SIB, one has to be a vector and the other, if present, a GPR. The vector must be the
                // index operand.
                if (it == -1 || (bx & (opf::XMMREG | opf::YMMREG | opf::ZMMREG) & ~opf::REG_EA)) {
                    if (s == 0)
                        s = 1;
                    else if (s != 1)
                        goto err;

                    t = bt, bt = it, it = t;
                    x = bx, bx = ix, ix = x;
                }

                if (bt != -1) {
                    if (opf::REG_GPR & ~bx) goto err;
                    if (!(opf::REG64 & ~bx) || !(opf::REG32 & ~bx))
                        sok &= bx;
                    else
                        goto err;
                }

                // While we're here, ensure the user didn't specify WORD or QWORD.
                if (input.disp_size == 16 || input.disp_size == 64) goto err;

                if (addrbits == 16 || (addrbits == 32 && !(sok & opf::BITS32)) || (addrbits == 64 && !(sok & opf::BITS64))) goto err;

                output.type = ((ix & opf::ZMMREG & ~opf::REG_EA) ? EA_ZMMVSIB : ((ix & opf::YMMREG & ~opf::REG_EA) ? EA_YMMVSIB : EA_XMMVSIB));

                output.rex |= rexflags(it, ix, REX_rX);
                output.rex |= rexflags(bt, bx, REX_rB);

                index = it & 7;   // it is known to be != -1

                switch (s) {
                case 1: scale = 0; break;
                case 2: scale = 1; break;
                case 4: scale = 2; break;
                case 8: scale = 3; break;
                default:   // then what the smeg is it?
                    goto err;   // panic
                }

                if (bt == -1) {
                    base = 5;
                    mod = 0;
                } else {
                    base = bt & 7;
                    mod = static_cast<int>(memory_mod(eaflags, ins, o, known, base != REG_NUM_EBP));
                }

                output.sib_present = true;
                output.bytes = static_cast<std::uint8_t>(bt == -1 || mod == 2 ? 4 : mod);
                output.modrm = static_cast<std::uint8_t>(GEN_MODRM(mod, rfield, 4));
                output.sib = static_cast<std::uint8_t>(GEN_SIB(scale, index, base));
            } else if ((ix | bx) & (opf::BITS32 | opf::BITS64)) {
                // It must be a 32/64-bit memory reference. Firstly we have to check that all registers involved are
                // type E/Rxx.
                OpFlags sok = opf::BITS32 | opf::BITS64;
                const std::int32_t o = static_cast<std::int32_t>(input.offset);

                if (it != -1) {
                    if (!(opf::REG64 & ~ix) || !(opf::REG32 & ~ix))
                        sok &= ix;
                    else
                        goto err;
                }

                if (bt != -1) {
                    if (opf::REG_GPR & ~bx) goto err;   // invalid register
                    if (~sok & bx & SIZE_MASK) goto err;   // invalid size
                    sok &= bx;
                }

                // While we're here, ensure the user didn't specify WORD or QWORD.
                if (input.disp_size == 16 || input.disp_size == 64) goto err;

                if (addrbits == 16 || (addrbits == 32 && !(sok & opf::BITS32)) || (addrbits == 64 && !(sok & opf::BITS64))) goto err;

                // Now reorganize base/index.
                if (s == 1 && bt != it && bt != -1 && it != -1 && ((hb == b && ht == EAH_NOTBASE) || (hb == i && ht == EAH_MAKEBASE))) {
                    // Swap if hints say so.
                    t = bt, bt = it, it = t;
                    x = bx, bx = ix, ix = x;
                }

                if (bt == -1 && s == 1 && !(hb == i && ht == EAH_NOTBASE)) {
                    // Make a single reg base, unless hint.
                    bt = it, bx = ix, it = -1, ix = 0;
                }
                if (eaflags & EAF_MIB) {
                    // MIB/split-SIB encoding.
                    if (it == -1 && (hb == b && ht == EAH_NOTBASE)) {
                        // Make a single reg index [reg*1]; gas uses this form for an explicit index register.
                        it = bt, ix = bx, bt = -1, bx = 0, s = 1;
                    }
                    if ((ht == EAH_SUMMED) && bt == -1) {
                        // Separate once-summed index into [base, index].
                        bt = it, bx = ix, s--;
                    }
                } else {
                    if (((s == 2 && it != REG_NUM_ESP && ((ht == EAH_SUMMED) || (!as_.masm_mode && !(eaflags & EAF_TIMESTWO)))) || s == 3 || s == 5 ||
                         s == 9) &&
                        bt == -1) {
                        // Convert 3*EAX to EAX+2*EAX (and normally [reg*2] to [reg+reg]). Under --masm the [reg*2] fold
                        // is skipped: ML keeps the index*2+disp32 SIB form the disassembly corpus was written from. The
                        // s==3/5/9 decompositions are mandatory (those scales cannot be encoded) and so apply in either
                        // mode.
                        bt = it, bx = ix, s--;
                    }
                    if (it == -1 && (bt & 7) != REG_NUM_ESP && (eaflags & EAF_TIMESTWO) && (hb == b && ht == EAH_NOTBASE)) {
                        // Convert [NOSPLIT EAX*1] to sib format with 0x0 displacement - [EAX*1+0].
                        it = bt, ix = bx, bt = -1, bx = 0, s = 1;
                    }
                }
                if (s == 1 && it == REG_NUM_ESP) {
                    // Swap ESP into base if scale is 1.
                    t = it, it = bt, bt = t;
                    x = ix, ix = bx, bx = x;
                }
                if (it == REG_NUM_ESP || (s != 1 && s != 2 && s != 4 && s != 8 && it != -1)) goto err;   // wrong, for various reasons

                output.rex |= rexflags(it, ix, REX_rX);
                output.rex |= rexflags(bt, bx, REX_rB);

                if (it == -1 && (bt & 7) != REG_NUM_ESP && !(eaflags & EAF_SIB)) {
                    // No SIB needed.
                    int mod, rm;

                    if (bt == -1) {
                        rm = 5;
                        mod = 0;
                    } else {
                        rm = bt & 7;
                        mod = static_cast<int>(memory_mod(eaflags, ins, o, known, rm != REG_NUM_EBP));
                    }

                    output.sib_present = false;
                    output.bytes = static_cast<std::uint8_t>(bt == -1 || mod == 2 ? 4 : mod);
                    output.modrm = static_cast<std::uint8_t>(GEN_MODRM(mod, rfield, rm));
                } else {
                    // We need a SIB.
                    int mod, scale, index, base;

                    if (it == -1)
                        index = 4, s = 1;
                    else
                        index = it & 7;

                    switch (s) {
                    case 1: scale = 0; break;
                    case 2: scale = 1; break;
                    case 4: scale = 2; break;
                    case 8: scale = 3; break;
                    default:   // then what the smeg is it?
                        goto err;   // panic
                    }

                    if (bt == -1) {
                        base = 5;
                        mod = 0;
                    } else {
                        base = bt & 7;
                        mod = static_cast<int>(memory_mod(eaflags, ins, o, known, base != REG_NUM_EBP));
                    }

                    output.sib_present = true;
                    output.bytes = static_cast<std::uint8_t>(bt == -1 || mod == 2 ? 4 : mod);
                    output.modrm = static_cast<std::uint8_t>(GEN_MODRM(mod, rfield, 4));
                    output.sib = static_cast<std::uint8_t>(GEN_SIB(scale, index, base));
                }
            } else {   // it's 16-bit
                int mod, rm;
                const std::int16_t o = static_cast<std::int16_t>(input.offset);

                // Check for 64-bit long mode.
                if (addrbits == 64) goto err;

                // Check all registers are BX, BP, SI or DI.
                if ((b != -1 && b != R_BP && b != R_BX && b != R_SI && b != R_DI) || (i != -1 && i != R_BP && i != R_BX && i != R_SI && i != R_DI))
                    goto err;

                // Ensure the user didn't specify DWORD/QWORD.
                if (input.disp_size == 32 || input.disp_size == 64) goto err;

                if (s != 1 && i != -1) goto err;   // no can do, in 16-bit EA
                if (b == -1 && i != -1) {
                    const int tmp = b;
                    b = i;
                    i = tmp;
                }   // swap
                if ((b == R_SI || b == R_DI) && i != -1) {
                    const int tmp = b;
                    b = i;
                    i = tmp;
                }
                // Have BX/BP as base, SI/DI index.
                if (b == i) goto err;   // shouldn't ever happen, in theory
                if (i != -1 && b != -1 && (i == R_BP || i == R_BX || b == R_SI || b == R_DI)) goto err;   // invalid combinations
                if (b == -1)   // pure offset: handled above
                    goto err;  // so if it gets to here, panic!

                rm = -1;
                if (i != -1) {
                    switch (i * 256 + b) {
                    case R_SI * 256 + R_BX: rm = 0; break;
                    case R_DI * 256 + R_BX: rm = 1; break;
                    case R_SI * 256 + R_BP: rm = 2; break;
                    case R_DI * 256 + R_BP: rm = 3; break;
                    }
                } else {
                    switch (b) {
                    case R_SI: rm = 4; break;
                    case R_DI: rm = 5; break;
                    case R_BP: rm = 6; break;
                    case R_BX: rm = 7; break;
                    }
                }
                if (rm == -1)   // can't happen, in theory
                    goto err;   // so panic if it does

                mod = static_cast<int>(memory_mod(eaflags, ins, o, known, rm != 6));
                output.sib_present = false;   // no SIB - it's 16-bit
                output.bytes = static_cast<std::uint8_t>(mod);   // bytes of offset needed
                output.modrm = static_cast<std::uint8_t>(GEN_MODRM(mod, rfield, rm));
            }

            if (eaflags & EAF_REL) {
                // Explicit REL reference with indirect memory.
                as_.warn(WARN_OTHER, "indirect address displacements cannot be RIP-relative");
            }
        }
    }

    output.size = static_cast<std::uint8_t>(1 + output.sib_present + output.bytes);
    // The type parsed might not match one supplied by a caller. In this case exit with error and let the caller decide
    // how critical it is.
    if (output.type != expected) goto err_set_msg;

    return 0;

err:
    output.type = EA_INVALID;

err_set_msg:
    if (errmsg.empty()) {
        // Default error message.
        errmsg = std::format("invalid {}-bit effective address", addrbits);
    }
    return -1;
}

void Encoder::add_asp(Insn& ins) {
    const int bits = ins.bits;
    int valid = (bits == 64) ? 64 | 32 : 32 | 16;

    switch (ins.prefixes[PPS_ASIZE]) {
    case P_A16: valid &= 16; break;
    case P_A32: valid &= 32; break;
    case P_A64: valid &= 64; break;
    case P_ASP: valid &= (bits == 32) ? 16 : 32; break;
    default: break;
    }

    for (int j = 0; j < ins.operands; j++) {
        const Operand& op = ins.oprs[static_cast<std::size_t>(j)];
        if (is_class(opf::MEMORY, op.type)) {
            OpFlags i, b;

            // Verify as register.
            if (!is_register(op.indexreg))
                i = 0;
            else
                i = kRegFlags[static_cast<std::size_t>(op.indexreg)];

            // Verify as register.
            if (!is_register(op.basereg))
                b = 0;
            else
                b = kRegFlags[static_cast<std::size_t>(op.basereg)];

            if (op.scale == 0) i = 0;

            if (!i && !b) {
                const int ds = op.disp_size;
                if ((bits != 64 && ds > 8) || (bits == 64 && ds == 16)) valid &= ds;
            } else {
                if (!(opf::REG16 & ~b)) valid &= 16;
                if (!(opf::REG32 & ~b)) valid &= 32;
                if (!(opf::REG64 & ~b)) valid &= 64;

                if (!(opf::REG16 & ~i)) valid &= 16;
                if (!(opf::REG32 & ~i)) valid &= 32;
                if (!(opf::REG64 & ~i)) valid &= 64;
            }
        }
    }

    const int asp_bits = (bits == 32) ? 16 : 32;

    if (valid & bits) {
        ins.addr_size = static_cast<std::uint8_t>(bits);
    } else if (valid & asp_bits) {
        // Add an address size prefix.
        ins.prefixes[PPS_ASIZE] = (bits == 32) ? P_A16 : P_A32;
        ins.addr_size = static_cast<std::uint8_t>(asp_bits);
    } else {
        // Impossible...
        as_.nonfatal("impossible combination of address sizes");
        ins.addr_size = static_cast<std::uint8_t>(bits);   // error recovery
    }

    const int defdisp = ins.addr_size == 16 ? 16 : 32;

    for (int j = 0; j < ins.operands; j++) {
        Operand& op = ins.oprs[static_cast<std::size_t>(j)];
        if (!is_class(opf::MEM_OFFS, op.type)) continue;

        int disp_size = op.disp_size;
        if (!disp_size) disp_size = defdisp;

        if (disp_size != ins.addr_size) {
            // mem_offs sizes must match the address size; if not, strip the MEM_OFFS bit and match only EA instructions.
            op.type &= ~(opf::MEM_OFFS & ~opf::MEMORY);
        }
    }
}

// Set the initial operand size, based only on the mode and any prefixes. The actual operand size may change after
// instruction pattern selection.
void Encoder::set_initial_opsize(Insn& ins) {
    const int bits = ins.bits;

    switch (ins.prefixes[PPS_OSIZE]) {
    case P_OSP: ins.op_size = bits == 16 ? 32 : 16; break;
    case P_O16: ins.op_size = 16; break;
    case P_O32: ins.op_size = 32; break;
    case P_O64: ins.op_size = 64; break;
    default: ins.op_size = bits == 16 ? 16 : 32; break;
    }
}

void Encoder::insn_early_setup(Insn& ins) {
    // Check to see if we need an address-size prefix.
    add_asp(ins);

    // Set default/prefix-controlled operand size.
    set_initial_opsize(ins);
}

bool Encoder::assert_no_prefix(Insn& ins, int pos) {
    if (ins.prefixes[static_cast<std::size_t>(pos)]) {
        as_.nonfatal("invalid {} prefix", prefix_name(ins.prefixes[static_cast<std::size_t>(pos)]));
        return true;
    }
    return false;
}

void Encoder::list_nonfinal_pass(std::int64_t start) {
    if (lfmt_ && lfmt_->option('p')) {
        OutData dummy;
        // OUT_RAWDATA with .data is special.
        dummy.type = OUT_RAWDATA;
        dummy.loc = as_.location;
        dummy.size = static_cast<std::uint64_t>(as_.location.offset - start);
        dummy.loc.offset = start;
        lfmt_->output(dummy);
    }
}

// Process a single instruction without TIMES; this is a common case so it is optimized.
void Encoder::process_one_insn(Insn& ins) {
    ins.loc = as_.location;
    std::int64_t l;
    if (!as_.pass_final()) {
        l = insn_size(ins);
        if (l < 0) return;   // invalid instruction
        list_nonfinal_pass(ins.loc.offset);
    } else {
        l = assemble(ins);
        if (l < 0) as_.assert_failed("l >= 0");   // invalid instruction here: bad
    }
    as_.increment_offset(l);
}

// Process an instruction with TIMES handling. As this instruction may end up getting processed multiple times and it is
// permitted for the intervening layers to change the contents of the instruction structure, it is necessary to keep the
// original and re-initialize the structure on each iteration.
void Encoder::process_times_insn(Insn& ins) {
    std::int64_t l;

    ins.loc = as_.location;

    // NOTE: insn_size() and therefore assemble() can change ins.times (usually to 1) when called due to merging certain
    // operations (e.g. TIMES x RESB y). Therefore, it is necessary to check the returned times value for each iteration.
    if (!as_.pass_final()) {
        std::int64_t times = ins.times;
        while (times > 0) {
            Insn tmpins = ins;
            tmpins.loc.offset = as_.location.offset;
            tmpins.times = static_cast<std::int32_t>(times);
            l = insn_size(tmpins);
            if (l < 0) break;   // invalid instruction
            as_.increment_offset(l);
            times = tmpins.times - 1;
        }
        list_nonfinal_pass(ins.loc.offset);   // the original starting offset
    } else {
        Insn tmpins = ins;
        l = assemble(tmpins);
        if (l < 0) as_.assert_failed("l >= 0");   // invalid instruction here: bad
        as_.increment_offset(l);

        std::int64_t times = tmpins.times;
        if (times <= 1) return;

        if (lfmt_) lfmt_->uplevel(ListType::TIMES, times);
        times--;
        while (times) {
            tmpins = ins;
            tmpins.loc.offset = as_.location.offset;
            tmpins.times = static_cast<std::int32_t>(times);
            l = assemble(tmpins);
            if (l < 0) as_.assert_failed("l >= 0");
            as_.increment_offset(l);
            times = tmpins.times - 1;
        }
        if (lfmt_) lfmt_->downlevel(ListType::TIMES);
    }
}

// The entry point of this module for a line (process_insn).
void Encoder::process_insn(Insn& ins) {
    if (ins.times == 1) {
        process_one_insn(ins);
    } else if (!ins.times) {
        // TIMES 0 = nothing to do.
    } else if (ins.times > 0) {
        process_times_insn(ins);
    } else {
        as_.nonfatalf(ERR_PASS2, "TIMES value {} is negative", ins.times);
    }
}

}  // namespace cb::build::x86asm
