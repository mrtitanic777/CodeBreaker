// engine/pcode_cpu_a64_fp.hpp -- the Switch lane's scalar floating point where the description does not give the lane's result (engine/pcode_cpu_a64.cpp calls these).
//
// The lane's floating point is the host's: a result is what C++ float and double arithmetic gives (round to nearest even, the host's NaN rules, no flags). The description's p-code is
// right for the arithmetic of numbers but not for a NaN result (the sign and payload of the NaN come out of its host float operations differently), and it has no meaning for the
// float-to-integer conversions, the fused multiply-adds, the maximum and minimum, or the rounding instructions. The functions here compute those the way the lane defines them, from
// the manual's instruction fields, so that the lane's results do not change when a floating-point instruction moves from the own core to the emulator.
#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>

namespace cb::engine::pcode_cpu::a64fp {

// FADD ... FNMUL (the two-source group), FMADD ... FNMSUB (three sources), FMOV ... FRINTI (one source, from a single, a double or a half). `n`, `m` and `a` are the low 64
// bits of Vn, Vm and Va; the result is the low bits of Vd (the rest of Vd is zero). Half-precision forms the lane stops at never come here.
std::uint64_t scalar_result(std::uint32_t word, std::uint64_t n, std::uint64_t m, std::uint64_t a);

// SCVTF, UCVTF (integer to float) and FCVT{N,P,M,Z,A}{S,U} (float to integer), by register. `x` is Xn, `v` the low 64 bits of Vn. Returns where the result goes (true: Xd, false:
// the low bits of Vd) and writes its value.
bool int_conversion(std::uint32_t word, std::uint64_t x, std::uint64_t v, std::uint64_t& out);

// The fixed-point forms (SCVTF, UCVTF with #fbits, FCVTZS, FCVTZU with #fbits).
bool fixed_conversion(std::uint32_t word, std::uint64_t x, std::uint64_t v, std::uint64_t& out);

// The vector and scalar FRINT* and FCVT{N,M,A,P,Z}{S,U} of a register: `n` is Vn (16 bytes), the result is Vd's low `bytes` bytes. `scalar`: the scalar form (one lane). Returns
// false for a mnemonic or size it does not take.
bool round_convert(std::uint32_t word, std::string_view mnemonic, bool scalar, const std::uint8_t* n, std::uint8_t* out, std::size_t& bytes);

// FCVTZS and FCVTZU with fractional bits (vector and scalar, singles and doubles): the same arguments as round_convert.
bool fixed_convert(std::uint32_t word, std::string_view mnemonic, bool scalar, const std::uint8_t* n, std::uint8_t* out, std::size_t& bytes);

// FMLA and FMLS (vector: `form` 0, by element: 1, scalar by element: 2) on singles and doubles with the host's fused multiply-add. `n`, `m`, `d` are Vn, Vm, Vd (16 bytes each).
bool fused_lanes(std::uint32_t word, std::string_view mnemonic, int form, const std::uint8_t* n, const std::uint8_t* m, const std::uint8_t* d, std::uint8_t* out, std::size_t& bytes);

// FADD, FSUB, FMUL, FDIV, FMULX, FABD on the lanes of a vector (`form` 0), by element (1) or as a scalar (2: of the registers' low lanes, 3: with an element), as the host computes
// them (what the description gets wrong is a NaN result: its sign and payload). False: not one of these.
bool host_lanes(std::uint32_t word, std::string_view mnemonic, int form, const std::uint8_t* n, const std::uint8_t* m, std::uint8_t* out, std::size_t& bytes);

// Whether any lane of `bytes` bytes of `ew` byte floats (4 or 8) is a NaN.
bool any_nan_lane(const std::uint8_t* v, std::size_t bytes, unsigned ew);

// Whether a result of the given float type (0 single, 1 double, 3 half) is a NaN.
bool is_nan_result(unsigned ftype, std::uint64_t value);

}  // namespace cb::engine::pcode_cpu::a64fp
