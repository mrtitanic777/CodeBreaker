r"""msvc2_registers.py -- the UNIFIED register-dataflow interpreter for the MSVC 2.0 (win95) inverse.

The 32-bit sibling of msc6_registers.py. A small abstract interpreter walks a function's instruction
stream once, tracking each register's symbolic value, and proposes a statement-list node
{nargs, stmts, ret, leave} in the vocabulary msvc2_forward's emit already speaks, so every
recovery stays self-verified (emit(node) == bytes): calls whose result is dead become void-call
statements, stores through tracked pointers become stores, and the live eax is the return.
Straight-line framed bodies only; callee-saved parks decline.

The walk is C++ (native/invert/msvc2.cc; docs/NATIVE.md); this keeps the API.
"""
from codebreaker.invert import native_invert


def interp(code, nargs):
    """Walk a straight-line framed function -> a list of seq nodes {nargs, stmts, leave, ret}, or
    None if it steps outside this cut.  The caller emit-verifies each."""
    return native_invert.msvc2_interp(code, nargs)
