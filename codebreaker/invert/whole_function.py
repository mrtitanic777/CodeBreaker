r"""whole_function.py -- whole-function templates: a structural matcher over the instructions and an
emitter that fills the holes, so a recurring shape is read once and reused on every module.  The
compiler then proves each candidate exact or rejects it.

Template so far: the pure forwarding thunk (loadds prologue; pushes of zeros and [bp+off] params;
push cs; call a sibling; [add sp,N]; the epilogue), in both argument orders.  C++
(native/invert/msc6_helpers.cc; docs/NATIVE.md); this keeps the API.

    from codebreaker.invert import whole_function as templates
    c = templates.synthesize(func_bytes)     # -> candidate C str, or None
"""
from codebreaker.invert import native_invert


def candidates(func_bytes):
    """Yield (template, candidate C) for `func_bytes` from every matching template."""
    yield from native_invert.candidates(func_bytes)


def synthesize(func_bytes, verify=None):
    """The first candidate C that `verify(func_bytes, c)` accepts ({'exact': True}), or -- with no
    verifier -- the first candidate."""
    for _name, c in candidates(func_bytes):
        if verify is None:
            return c
        if verify(bytes(func_bytes), c).get("exact"):
            return c
    return None
