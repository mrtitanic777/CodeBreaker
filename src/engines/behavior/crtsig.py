#!/usr/bin/env python3
r"""crtsig.py -- identify CRT / compiler-helper functions so we DON'T reconstruct the toolchain.

Core methodology (triage by ORIGIN first): a large share of a binary's functions are not the
author's code at all -- they are the C runtime and the compiler's own helpers (`_chkstk`, `__allmul`,
`__aulldiv`, `__ftol`, `_EH_prolog`, the intrinsic `memcpy`/`memset` expansions, ...). Reconstructing
those is wasted effort and pollutes the recovery number. This identifies them by signature so recon
can mark them `origin=toolchain` and skip reconstruction -- they get a one-line declaration with
their real CRT name, not a fabricated body.

Two mechanisms, most-confident first:
  1. STRUCTURAL idioms -- version-independent instruction fingerprints (a `fnstcw`/`fldcw`/`fistp`
     float->long, a 0x1000 page-probe loop, a leaf `ret 0x10` with `mul`/`div` and no call). These
     don't depend on exact bytes, so they survive across MSVC 1.x-4.x (the Win95-era compilers).
  2. A FLIRT-style masked-byte pattern DB -- exact leading bytes with operand/relocation bytes
     wildcarded, plus a tail CRC. `learn()` extracts such a pattern from a LABELLED function, so the
     DB grows from any corpus where a helper's name is known (an export, a CodeView symbol).
"""
import struct
from src.engines.behavior import tracer


def _rows(module, va):
    import capstone
    _, va0, code, _s, funcs, _sha, _imp = tracer._load(module)
    off = va - va0
    end = funcs[off]["end"] if off in funcs else (
        min((o for o in funcs if o > off), default=off + 0x400))
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    return [(i.address, i.mnemonic, i.op_str, i.size) for i in md.disasm(code[off:end], va0 + off)], \
        code[off:end]


# --------------------------------------------------------------- structural idiom detectors
def _structural(rows):
    """Return (name, category) for a recognizable CRT/compiler helper, or None. rows: [(a,mn,ops,sz)]."""
    mnems = [r[1] for r in rows]
    ops = [r[2] for r in rows]
    has = lambda m: m in mnems
    ncall = mnems.count("call")
    ret_imm = _ret_imm(rows)

    # float -> long/int: the classic __ftol / __ftol2 (round-toward-zero via control word)
    if has("fnstcw") and has("fldcw") and (has("fistp") or has("fist")):
        return "__ftol", "crt-fp"
    # stack probe: _chkstk / _alloca_probe -- a 0x1000 page constant + esp walk, leaf
    if ncall == 0 and any("0x1000" in o for o in ops) and any("esp" in o for o in ops) \
            and any(m in ("sub", "or", "test", "cmp") for m in mnems):
        return "_chkstk", "compiler-helper"
    # 64-bit arithmetic helpers: leaf, `ret 0x10` (four dword args), no calls
    if ncall == 0 and ret_imm == 0x10:
        if has("div") or has("idiv"):
            return "__aulldiv_family", "compiler-helper"     # alldiv/aulldiv/allrem/aullrem
        if has("mul") or has("imul"):
            return "__allmul", "compiler-helper"
        if has("shl") or has("shr") or has("sar") or has("shld") or has("shrd"):
            return "__allshl_family", "compiler-helper"      # allshl/allshr/aullshr
    # intrinsic block ops the compiler inlines then shares (capstone folds `rep` into the mnemonic)
    joined = " ".join("%s %s" % (m, o) for m, o in zip(mnems, ops))
    if ncall == 0 and ("movs" in joined or "stos" in joined):
        if "movs" in joined:
            return "memcpy", "crt-intrinsic"
        if "stos" in joined:
            return "memset", "crt-intrinsic"
    # _EH_prolog: tiny thunk that installs the C++ EH frame via fs:[0], then rets
    if len(rows) <= 14 and any("fs:" in o for o in ops) and ret_imm in (0, None) \
            and any(m == "push" for m in mnems):
        return "_EH_prolog", "compiler-helper"
    return None


def _ret_imm(rows):
    for _a, mn, ops, _s in rows:
        if mn in ("ret", "retn"):
            try:
                return int(ops, 0) if ops.strip() else 0
            except ValueError:
                return 0
    return None


# --------------------------------------------------------------- FLIRT-style masked-byte DB
# each entry: {"name","category","pattern": bytes, "mask": bytes (1=match,0=wildcard), "crc": int|None,
#              "len": total-bytes-CRCd}. Seeded empty; grow with learn() from labelled corpora.
_DB = []
_VAR_MNEM = ("call", "jmp", "je", "jne", "jz", "jnz", "jl", "jle", "jg", "jge", "jb", "jbe",
             "ja", "jae", "js", "jns", "jo", "jno", "jp", "jnp", "loop")


def learn(module, va, name, category="crt", prefix=32):
    """Extract a masked signature from a LABELLED function (name known from an export/symbol).

    Wildcards the operand bytes of relocatable/relative instructions (calls, jumps, rip/abs refs)
    so the pattern matches the same helper wherever it is linked; CRCs the bytes after the prefix."""
    import zlib
    rows, code = _rows(module, va)
    pat = bytearray(code[:prefix])
    mask = bytearray(b"\xff" * len(pat))
    pos = 0
    for a, mn, ops, sz in rows:
        if pos >= len(pat):
            break
        if mn in _VAR_MNEM or "0x" in ops and sz >= 5:       # relative/absolute operand -> wildcard it
            for k in range(1, sz):                            # keep the opcode byte, wildcard operand
                if pos + k < len(mask):
                    mask[pos + k] = 0
        pos += sz
    tail = code[prefix:]
    crc = zlib.crc32(tail) & 0xffffffff if tail else None
    return {"name": name, "category": category, "pattern": bytes(pat), "mask": bytes(mask),
            "crc": crc, "len": len(code)}


def register(sig):
    _DB.append(sig)


def _match_db(code):
    import zlib
    for sig in _DB:
        p, m = sig["pattern"], sig["mask"]
        if len(code) < len(p):
            continue
        if all((code[i] & m[i]) == (p[i] & m[i]) for i in range(len(p))):
            if sig["crc"] is not None:
                tail = code[len(p):sig["len"]]
                if len(tail) != sig["len"] - len(p) or (zlib.crc32(tail) & 0xffffffff) != sig["crc"]:
                    continue
            return sig["name"], sig["category"]
    return None


# --------------------------------------------------------------- public API
def identify(module, va):
    """{'name','category','confidence','via'} if the function is CRT/compiler code, else None."""
    try:
        rows, code = _rows(module, va)
    except Exception:
        return None
    if not rows:
        return None
    hit = _structural(rows)
    if hit:
        return {"name": hit[0], "category": hit[1], "confidence": "high", "via": "structural"}
    hit = _match_db(bytes(code))
    if hit:
        return {"name": hit[0], "category": hit[1], "confidence": "high", "via": "pattern"}
    return None


def classify_rows(rows):
    """Structural classification directly from rows -- for testing without a loaded module."""
    return _structural(rows)
