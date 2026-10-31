"""native_le.py -- le.LEFile and le.W3File answered by the C++ (native/formats/le).

While CB_NATIVE_FORMATS(_LE) is on, LEFile(data, lfa) and W3File(data, lfa) hand out these
(their __new__ picks them), and le.py's module-level readers ask formats.dll (native.native_first,
with the image as (lf.d, lf.lfa)). The same attributes; nothing is kept that is not in the bytes.
A temporary shim (docs/NATIVE.md, step 4).
"""
from codebreaker.formats import le, native


def image_args(lf, *rest):
    """(bytes, arguments) for a question about an image: its header's offset first."""
    return lf.d, (lf.lfa,) + rest


def _opened(data, pairs):
    """[(name, LEFile)] for (name, lfa) pairs found in `data`, opened in order (as the Python
    opens them: one that fails raises there)."""
    return [(name, LEFile(data, lfa)) for name, lfa in pairs]


def vxds_result(answer, data, path):
    return _opened(data, answer)


def members_result(answer, data, path):
    expanded, pairs = answer
    return _opened(data if expanded is None else expanded, pairs)


class LEFile(le.LEFile):
    """le.LEFile with every reader in C++."""

    def __init__(s, data, lfa):
        s.d = data
        s.lfa = lfa
        try:
            state = native.call("le.open", data, lfa)
        except native.Fallback:
            return le.LEFile.__init__(s, data, lfa)
        if isinstance(data, bytearray):
            state["sig"] = bytearray(state["sig"])
        s.__dict__.update(state)

    @classmethod
    def from_buffer(cls, data):
        return cls(data, native.call("le.from_buffer", data))


def _object_of(lf, answer):
    idx, off = answer
    return (lf.objects[idx - 1] if idx is not None else None), off


def _where(lf):
    return (lf.lfa,)


for _name, _question in (("page_to_obj", "le.page_to_obj"), ("fixups", "le.fixups"),
                         ("import_modules", "le.import_modules"), ("resident_name", "le.resident_name"),
                         ("header", "le.header"), ("description", "le.description"), ("ddb", "le.ddb"),
                         ("object_bytes", "le.object_bytes")):
    setattr(LEFile, _name, native.asker(_question, getattr(le.LEFile, _name), _where, data="d",
                                        result=_object_of if _name == "page_to_obj" else None))


class W3File(le.W3File):
    """le.W3File with every reader in C++."""

    def __init__(s, data, lfa):
        s.d = data
        s.lfa = lfa
        try:
            s.__dict__.update(native.call("le.w3", data, lfa))
        except native.Fallback:
            le.W3File.__init__(s, data, lfa)

    def modules(s):
        try:
            return _opened(s.d, native.call("le.w3_modules", s.d, s.lfa))
        except native.Fallback:
            return le.W3File.modules(s)


CLASSES = {le.LEFile: LEFile, le.W3File: W3File}
