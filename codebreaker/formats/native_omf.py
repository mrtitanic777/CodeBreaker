"""native_omf.py -- formats/omf.py answered by the C++ (native/formats/omf).

While CB_NATIVE_FORMATS(_OMF) is on, omf.py's readers ask formats.dll (native.native_first); this
module turns the answers back into what they returned: read()'s omf.Obj, walk()'s generator.
patched (its spec read by a regular expression) and rename_symbols (a Python function applied to
every name) stay Python. A temporary shim (docs/NATIVE.md, step 4).
"""
from codebreaker.formats import native, omf


def obj(answer, data, *_):
    o = omf.Obj()
    o.__dict__.update(answer)
    o.raw = bytes(data)
    return o


def walk(answer, data, *_):
    items, error = answer
    if isinstance(data, bytearray):
        items = [(i, t, bytearray(body)) for i, t, body in items]

    def records():
        yield from items
        if error is not None:
            raise native._exception(*error)
    return records()


def holes_args(o, seg):
    return None, (o.fix, seg)


def code_segments_args(o):
    return None, (o.segs,)


def functions_args(o, seg="_TEXT"):
    return None, (o.pubs, len(o.seg.get(seg, b"")), o.filled.get(seg, 0), seg)
