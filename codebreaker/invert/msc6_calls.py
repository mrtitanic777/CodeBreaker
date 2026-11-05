r"""msc6_calls.py -- the parameters of a module's internal functions, from how they are called.

Internal functions have no SDK prototype, and one body alone misreads them (a far pointer only passed
through is never `les`'d).  A segment-level pass merges three signals: each callee's `retf N` (the
exact parameter bytes), the parameters its body loads far or long, and the far pointers pushed (a
segment word, then an offset) at every near call site.  C++ (native/invert/msc6_helpers.cc;
docs/NATIVE.md); this keeps the API.

    from codebreaker.invert import msc6_calls
    sigs = msc6_calls.infer_segment_sigs(nefile, seg, segbytes)   # {func_off: [(name,type),...]}
"""
from codebreaker.invert import native_invert


def infer_segment_sigs(f, seg, segb):
    """{func_off: [(name, 'int'|'far'|'long'), ...]} for every framed function in the segment's bytes
    (`f` and `seg` are not needed; kept for the callers)."""
    return native_invert.segment_sigs(segb)
