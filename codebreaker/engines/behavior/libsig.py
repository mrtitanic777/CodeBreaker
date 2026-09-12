#!/usr/bin/env python3
r"""libsig.py -- FLIRT-style identification of statically-linked LIBRARY functions.

crtsig catches the tiny compiler helpers. This is the same idea one level up: the C runtime, MFC,
OLE and common-controls helper functions are compiled into a program from .LIB/.OBJ files, so they
appear in the binary as the author's code but are NOT -- naming and skipping them keeps the recovery
number honest and hands the reader real symbols (`_fprintf`, `CString::operator+`) instead of
`sub_x`. The mechanism is FLIRT's: a signature is the function's leading bytes with every
relocated/relative operand wildcarded (so it matches wherever the linker placed it), plus a CRC of
the tail. Signatures are BUILT from the period libraries themselves -- an OMF .OBJ carries both the
code and, via its FIXUPP records, exactly which bytes are relocations to mask (see formats.objextract)
-- so the DB is generated from ground truth, not guessed.

    from codebreaker.engines.behavior import libsig
    db = libsig.build_from_lib("LIBC.LIB", "msvcrt")      # ground-truth signatures
    libsig.save(db, "libc.sig"); libsig.load("libc.sig")  # persist / reuse
    libsig.match_module("shell32.dll")                    # {va: (name, library)}
"""
import json
import struct
import zlib

_PREFIX = 32
_VAR_MNEM = ("call", "jmp", "je", "jne", "jz", "jnz", "jl", "jle", "jg", "jge", "jb", "jbe",
             "ja", "jae", "js", "jns", "jo", "jno", "jp", "jnp", "loop")
_DB = []                                     # loaded signatures: [{name,library,pattern,mask,crc,len}]


# --------------------------------------------------------------- build from period libraries (OMF)
def _split_omf_lib(data):
    """Yield each member OBJ of an OMF .LIB (THEADR/LHEADR .. MODEND). A bare .OBJ yields itself."""
    THEADR, LHEADR, MODEND, MODEND32 = 0x80, 0x82, 0x8A, 0x8B
    starts = []
    i = 0
    n = len(data)
    while i + 3 <= n:
        rtype = data[i]
        rlen = data[i + 1] | (data[i + 2] << 8)
        if rtype in (THEADR, LHEADR):
            starts.append(i)
        end = i + 3 + rlen
        if rtype in (MODEND, MODEND32) and starts:
            yield bytes(data[starts[-1]:end])
        if rlen == 0 or end <= i:
            break
        i = end
    if not starts:                                          # not OMF-record-shaped: treat as one blob
        yield bytes(data)


def build_from_obj(data, library):
    """Signatures for every public symbol in one OMF object (code + FIXUPP mask = ground truth)."""
    from codebreaker.formats import objextract
    obj = objextract.Obj(data)
    sigs = []
    for name, (seg, off) in obj.pubs.items():
        code = bytes(obj.seg_bytes.get(seg, b""))[off:]
        if len(code) < 8:
            continue
        mask_off = obj.seg_mask.get(seg, set())
        pat = bytearray(code[:_PREFIX])
        mask = bytearray(b"\xff" * len(pat))
        for rel in mask_off:                                # wildcard relocated bytes (and the field)
            for k in range(4):
                j = (rel - off) + k
                if 0 <= j < len(mask):
                    mask[j] = 0
        tail = code[_PREFIX:]
        sigs.append({"name": name.lstrip("_") or name, "library": library,
                     "pattern": bytes(pat).hex(), "mask": bytes(mask).hex(),
                     "crc": (zlib.crc32(tail) & 0xffffffff) if tail else None, "len": len(code)})
    return sigs


def build_from_lib(path, library):
    with open(path, "rb") as f:
        data = f.read()
    out = []
    for member in _split_omf_lib(data):
        try:
            out += build_from_obj(member, library)
        except Exception:
            continue
    return out


def build_from_labeled(module, names, library, prefix=_PREFIX):
    """Signatures from an already-named binary ({va: name}) -- when a lib's symbols are known."""
    from codebreaker.engines.behavior import crtsig
    out = []
    for va, name in names.items():
        try:
            sig = crtsig.learn(module, va, name, category=library, prefix=prefix)
        except Exception:
            continue
        out.append({"name": sig["name"], "library": library,
                    "pattern": sig["pattern"].hex(), "mask": sig["mask"].hex(),
                    "crc": sig["crc"], "len": sig["len"]})
    return out


# --------------------------------------------------------------- persist / load
def save(sigs, path):
    with open(path, "w") as f:
        json.dump(sigs, f)


def load(path):
    global _DB
    with open(path) as f:
        _DB = json.load(f)
    return _DB


def register(sigs):
    _DB.extend(sigs)


# --------------------------------------------------------------- match
def _match_bytes(code):
    for sig in _DB:
        p = bytes.fromhex(sig["pattern"]); m = bytes.fromhex(sig["mask"])
        if len(code) < len(p):
            continue
        if all((code[i] & m[i]) == (p[i] & m[i]) for i in range(len(p))):
            if sig["crc"] is not None:
                tail = code[len(p):sig["len"]]
                if len(tail) != sig["len"] - len(p) or (zlib.crc32(tail) & 0xffffffff) != sig["crc"]:
                    continue
            return sig["name"], sig["library"]
    return None


def match(module, va):
    """(name, library) if the function matches a library signature, else None."""
    from codebreaker.engines.behavior import tracer
    import capstone
    try:
        _, va0, code, _s, funcs, _sha, _imp = tracer._load(module)
    except Exception:
        return None
    off = va - va0
    end = funcs[off]["end"] if off in funcs else min((o for o in funcs if o > off), default=off + 0x400)
    return _match_bytes(bytes(code[off:end]))


def match_module(module):
    """{va: (name, library)} for every carved function that matches a loaded signature."""
    from codebreaker.engines.behavior import tracer
    _, va0, code, _s, funcs, _sha, _imp = tracer._load(module)
    out = {}
    for off in funcs:
        end = funcs[off]["end"]
        hit = _match_bytes(bytes(code[off:end]))
        if hit:
            out[va0 + off] = hit
    return out


def match_bytes(code):
    """Match a raw code blob against the loaded DB -- for testing."""
    return _match_bytes(bytes(code))
