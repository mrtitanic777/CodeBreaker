// engine/pcode_ops_x86.hpp -- the x86 description's named user operations (x86.sla, x86-64.sla): SSE, SSE2 to SSE4.2, SSSE3, AVX, AVX2, F16C, AES-NI, CLMUL, CRC32 and the x87's
// transcendental functions, as host C++ over the shared lane library (engine/pcode_vector.hpp).
//
//     pcode_ops_x86::Context context = pcode_ops_x86::make_context(emu);
//     const pcode_ops::Binding b = pcode_ops_x86::bind(emu, context);   // b.unbound: the operations of the description that nothing serves (they go to the hooks)
//
// The Context holds what the operations read and write besides their arguments (MXCSR: the rounding mode, flush-to-zero, denormals-are-zero and the exception flags) as register
// references, found once. It must outlive the emulator's use of the binding.
#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "engine/pcode_ops.hpp"
#include "engine/pcode_vector.hpp"

namespace cb::engine::pcode_ops_x86 {

struct Context {
    pcode_ops::Emu::RegRef mxcsr;
    bool has_mxcsr = false;
    pcode_ops::Emu::RegRef cf, pf, af, zf, sf, of;   // the flags the string compares of SSE4.2 set
    pcode_ops::Emu::RegRef fs_base, gs_base;   // the segment bases RDFSBASE and WRGSBASE read and write
    bool invalid_instruction = false;   // set by an operation of an instruction the machine refuses (#UD: CMPPS with a predicate over 7 outside VEX): the adapter's fault
};

// The library's environment for an MXCSR value (rounding mode, flush to zero, denormals are zero) and the MXCSR bits for the exception flags an operation raised.
vec::FpEnv environment(std::uint64_t mxcsr);
std::uint64_t exception_bits(unsigned flags);

Context make_context(const pcode_ops::Emu& emu);
pcode_ops::Binding bind(pcode_ops::Emu& emu, Context& context);
const pcode_ops::Table& table();

// The names this table serves and the ones of the description it does not (for the report and the tests): the second is what is given to bind()'s caller as `unbound`.
std::vector<std::string> served_names();

}  // namespace cb::engine::pcode_ops_x86
