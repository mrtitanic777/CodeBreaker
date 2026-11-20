// cb/build/x86asm/floats.hpp -- floating-point constants (asm/floats.c, asm/floats.h).
//
// `dd 1.5`, `dq -0.0`, `dt 3.14159`, `__?float32?__(1.5)` and the like: a decimal, hexadecimal (0x1.8p3), octal, binary or
// packed-BCD constant is converted to the bits of one of NASM's formats -- IEEE half (16), single (32), double (64),
// quad (128), the x87 80-bit extended, bfloat16 and an 8-bit minifloat -- rounded exactly as NASM rounds it (to nearest,
// ties to even, unless `[float down|up|zero]` says otherwise), with the same warnings for overflow, underflow and denormals.
//
// The two options a program can set (`[float daz]`, `[float near|down|up|zero]`) are the state of a `Floats` object, one
// per assembly run.
#pragma once

#include <array>
#include <cstdint>
#include <string_view>

#include "cb/build/x86asm/types.hpp"

namespace cb::build::x86asm {

class Assembly;

// The rounding control (enum float_round).
enum class FloatRound { Near, Zero, Down, Up };

// A floating-point format (struct ieee_format).
struct IeeeFormat {
    int bytes;      // total bytes
    int mantissa;   // fractional bits in the mantissa
    int explicit_;  // explicit integer bit
    int exponent;   // bits in the exponent
    int offset;     // offset into the byte array for the floatize operators
};

// The formats, indexed by Floatize (fp_formats). FLOAT_ERR is the size.
extern const std::array<IeeeFormat, FLOAT_ERR> kFpFormats;

// The bytes of a constant: up to 16, little endian, the first `kFpFormats[format].bytes` meaningful.
using FloatBytes = std::array<std::uint8_t, 16>;

class Floats {
public:
    explicit Floats(Assembly& assembly) : as_(assembly) {}

    // Convert the constant `str` (the text of the token, without its sign) with sign `sign` (negative if < 0) to `format`.
    // `str` is a number as the scanner returns a floating-point token, `__?infinity?__`, `__?nan?__`, `__?qnan?__` or
    // `__?snan?__`. Returns false, with the error reported, if the constant is unusable as a packed BCD one; a malformed
    // number is reported and gives a quiet NaN (float_const).
    bool float_const(std::string_view str, int sign, FloatBytes& result, Floatize format) const;

    // The format a Dx pseudo-op of this many bytes uses by default; FLOAT_ERR if none does (float_deffmt).
    static Floatize float_deffmt(int bytes);

    // `[float ...]`: "daz", "nodaz", "near", "down", "up", "zero" or "default" (case-blind). Returns 0, or -1 for an unknown
    // option (float_option).
    int float_option(std::string_view option);

    bool daz() const { return daz_; }
    FloatRound rounding() const { return rc_; }

private:
    Assembly& as_;
    bool daz_ = false;                    // denormals as zero
    FloatRound rc_ = FloatRound::Near;    // rounding control
};

}  // namespace cb::build::x86asm
