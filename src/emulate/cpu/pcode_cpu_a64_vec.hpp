// emulate/cpu/pcode_cpu_a64_vec.hpp -- the few Advanced SIMD instructions that the adapter (engine/pcode_cpu_a64.cpp) runs itself, over the lane library, because the description makes
// them wrongly: SSRA, USRA, SRSRA and URSRA (the rounding of the shift is lost), the scalar SLI (the shift count is read from 5 bits of its 6 bit field), SMAXP (the 16 byte form
// is written as SMAX), SABD (the difference wraps in the lane), SQRDMULH by element (the rounding is lost).
#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>

namespace cb::emulate::pcode_cpu::a64vec {

// Whether the adapter runs this mnemonic of this class itself. `scalar_class`: the scalar group of the class (SimdScalarShift, SimdScalarByElem).
bool takes(std::string_view mnemonic, bool shift_class, bool scalar_class, bool by_element_class);

// Compute the result of `word` (16 bytes of Vn, Vm and the old Vd; Vm is the register the instruction names, whole: the caller reads the right one) into out; `bytes` is the
// width of the result (the rest of Vd is zero). False: not an instruction it takes.
bool run(std::uint32_t word, std::string_view mnemonic, const std::uint8_t* n, const std::uint8_t* m, const std::uint8_t* d, std::uint8_t* out, std::size_t& bytes);

// The register number of Vm of a by-element instruction (the M bit is part of the index for 16 bit lanes).
unsigned element_register(std::uint32_t word);

}  // namespace cb::emulate::pcode_cpu::a64vec
