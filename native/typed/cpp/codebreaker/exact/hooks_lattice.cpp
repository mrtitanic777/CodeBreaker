// codebreaker/exact/hooks_lattice.cpp -- C++ port of codebreaker.exact.hooks_lattice.
//
// hooks_lattice.py -- answers a pass of the Lattice-lineage compilers is given at a place (profile hooks with "call").
//
// cmp_test: the code generator of MS C 1.04's own executables stood between the two shipped ones. When it
// compared two longs it loaded both words of the left one first only if neither operand was reached through
// a pointer (type flag 2000h); the shipped MC2.EXE 1.04 does so for every long - the test in the function that
// generates comparisons (image 0x4D79). There is no room to patch a longer test in, so the pass is told the
// outcome at 0x4D7F: go on at 0x4D85 (load) or 0x4DA1 (do not).

#include "codebreaker/exact/hooks_lattice.hpp"

namespace codebreaker::exact::hooks_lattice {

int64_t cmp_test(const Reader& rd, int64_t bp, int64_t ss, int64_t ds) {
    int64_t n = rd(ss, bp + 0x1C);   // the two operand nodes
    int64_t m = rd(ss, bp + 0x1E);
    bool eager = rd(ss, bp + 6) == 4 && !(rd(ds, n + 2) & 0x2000) && !(rd(ds, m + 2) & 0x2000);
    return eager ? 0x4D85 : 0x4DA1;
}

}  // namespace codebreaker::exact::hooks_lattice
