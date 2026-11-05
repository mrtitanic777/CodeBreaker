r"""segment_thunk.py -- the whole-function template for 16-bit segment-switch thunks.

Much of the framed corpus (the printer and device drivers especially) is generated assembly that
switches DS to a fixed data segment and forwards to a handler, or returns 0:

    <far loadds prologue>          ; mov ax,ds | push ds;pop ax; nop; inc bp; push bp; mov bp,sp; push ds; mov ds,ax
    mov ax, <seg> ; mov ds, ax     ; switch DS to the fixed data segment
    <body>                         ; forwarder: push args; call far <target>   stub: xor ax,ax [; cdq]
    <lea/std epilogue> ; retf <N>

parse() reads that grammar (None the instant a byte is not explained), verify() also re-lays the
bytes from the parse and demands they are the function's, and emit_asm() writes the MASM.  C++
(native/invert/msc6_helpers.cc; docs/NATIVE.md); this keeps the API.

    from codebreaker.invert import segment_thunk as seg_thunk
    t = seg_thunk.verify(func_bytes)        # None if not a pure thunk
    text = seg_thunk.emit_asm(t, "name")
"""
from codebreaker.invert import native_invert


def _thunk(fb, exact):
    t = native_invert.thunk(fb, "thunk", exact)
    if t is None:
        return None
    t["fb"] = bytes(fb)
    for k in ("pro", "epi", "pad"):
        t[k] = bytes.fromhex(t[k])
    t["body"] = [(n, bytes.fromhex(r)) for n, r in t["body"]]
    return t


def parse(fb):
    """{pro, seg, body [(name, raw)], epi, retf, pad} or None."""
    return _thunk(fb, False)


def verify(fb):
    """The parse, when re-laying it gives exactly these bytes; else None."""
    return _thunk(fb, True)


def emit_asm(t, name="thunk"):
    """The thunk as a 16-bit MASM procedure."""
    return native_invert.thunk(t["fb"], name, False)["asm"]


def kind(t):
    return t["kind"]
