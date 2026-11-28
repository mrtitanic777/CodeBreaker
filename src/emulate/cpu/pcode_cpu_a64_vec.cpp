// cb-unity: alone (file-local names that other files in a batch also use)
// emulate/cpu/pcode_cpu_a64_vec.cpp -- see pcode_cpu_a64_vec.hpp.
#include "emulate/cpu/pcode_cpu_a64_vec.hpp"

#include <cstring>
#include <vector>

#include "emulate/cpu/pcode_vector.hpp"

namespace cb::emulate::pcode_cpu::a64vec {
namespace {

namespace vec = cb::emulate::vec;
using u32 = std::uint32_t;

unsigned bits_of(u32 w, unsigned pos, unsigned width) { return (w >> pos) & ((1u << width) - 1); }

}  // namespace

bool takes(std::string_view mnemonic, bool shift_class, bool scalar_class, bool by_element_class) {
    if (shift_class) return mnemonic == "ssra" || mnemonic == "usra" || mnemonic == "srsra" || mnemonic == "ursra" || (scalar_class && mnemonic == "sli");
    if (by_element_class) return mnemonic == "sqrdmulh";
    return mnemonic == "smaxp" || mnemonic == "sabd";
}

unsigned element_register(u32 w) { return bits_of(w, 22, 2) == 1 ? bits_of(w, 16, 4) : bits_of(w, 16, 5); }

bool run(u32 w, std::string_view mnemonic, const std::uint8_t* n, const std::uint8_t* m, const std::uint8_t* d, std::uint8_t* out, std::size_t& bytes) {
    const bool q = bits_of(w, 30, 1) != 0;
    if (mnemonic == "smaxp") {
        const unsigned ew = 1u << bits_of(w, 22, 2);
        bytes = q ? 16 : 8;
        vec::pairwise(out, n, m, bytes, ew, vec::Pair::MaxSigned);
        return true;
    }
    if (mnemonic == "sabd") {   // the difference is taken exactly, whatever the width of the lane
        const unsigned ew = 1u << bits_of(w, 22, 2);
        bytes = q ? 16 : 8;
        vec::absDiff(out, n, m, bytes, ew, vec::Sign::Signed);
        return true;
    }
    if (mnemonic == "sqrdmulh") {   // by element: the element of Vm is every lane of the second operand
        const bool scalar = bits_of(w, 28, 1) != 0;
        const unsigned size = bits_of(w, 22, 2);
        if (size != 1 && size != 2) return false;
        const unsigned ew = size == 1 ? 2 : 4;
        const unsigned index = size == 1 ? (bits_of(w, 11, 1) << 2) | (bits_of(w, 21, 1) << 1) | bits_of(w, 20, 1) : (bits_of(w, 11, 1) << 1) | bits_of(w, 21, 1);
        bytes = scalar ? ew : q ? 16 : 8;
        std::uint8_t splat[16];
        vec::splat(splat, bytes, ew, vec::load(m + index * ew, ew));
        vec::mulDoublingHigh(out, n, splat, bytes, ew, true);
        return true;
    }
    // shifts: immh:immb gives the lane width and the count
    const unsigned immh = bits_of(w, 19, 4), immhb = bits_of(w, 16, 7);
    const unsigned ew = (immh & 8) != 0 ? 8 : (immh & 4) != 0 ? 4 : (immh & 2) != 0 ? 2 : (immh & 1) != 0 ? 1 : 0;
    if (ew == 0) return false;
    const bool scalar = bits_of(w, 28, 1) != 0;
    bytes = scalar ? ew : q ? 16 : 8;
    if (!scalar && ew == 8 && !q) return false;
    if (mnemonic == "sli") {
        vec::shiftInsert(out, d, n, bytes, ew, true, immhb - 8 * ew);
        return true;
    }
    const bool is_unsigned = bits_of(w, 29, 1) != 0, round = mnemonic[1] == 'r';   // (srsra, ursra)
    std::vector<std::uint8_t> shifted(bytes);
    vec::shiftRightImm(shifted.data(), n, bytes, ew, is_unsigned ? vec::Sign::Unsigned : vec::Sign::Signed, 16 * ew - immhb, round);
    vec::add(out, d, shifted.data(), bytes, ew);
    return true;
}

}  // namespace cb::emulate::pcode_cpu::a64vec
