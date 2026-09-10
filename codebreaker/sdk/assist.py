#!/usr/bin/env python3
r"""assist.py -- SDK-assisted inversion (Phase 3 payoff).

Ties the API database (apidb) to the MSC-6.0a inverse (msc6dec): given a function
name known to the SDK, it hands msc6dec the REAL typed signature (return type +
[(name,'int'|'far'|'long')] params) instead of the coarse guess msc6dec would make
from `retf n` alone. Correct far/long param layout is exactly what makes the
recovered C recompile byte-exact -- so SDK symbol/API data directly raises accuracy.

    from codebreaker.sdk import apidb, assist
    db = apidb.ApiDb.from_paths([hdr], [deff])
    src, meta = assist.invert(db, "LZSeek", code_bytes, imports={0:"_llseek"})
"""
import os, sys
from codebreaker.inverse_engine import msc6dec


def signature_for(db, name):
    """(rtype, params) in msc6dec form, or (None, None) if the SDK doesn't know it."""
    f = db.sig(name)
    if not f:
        return None, None
    rt = f["ret"] if f["ret"] in ("int", "long", "void") else "int"
    return rt, list(f["params"])


def invert(db, name, code, imports=None, syms=None):
    """Invert `code` using the SDK-known signature for `name`. Falls back to a bare
    guess (declining) if the SDK has no prototype -- and says so in meta."""
    rt, params = signature_for(db, name)
    if rt is None:
        src, meta = msc6dec.invert(code, imports=imports or {}, syms=syms)
        if isinstance(meta, dict):
            meta["sdk"] = "no prototype -- guessed from retf"
        return src, meta
    src, meta = msc6dec.invert(code, imports=imports or {}, syms=syms,
                               rtype=rt, params=params)
    if isinstance(meta, dict):
        meta["sdk"] = {"ret": rt, "params": params}
    return src, meta


def _selftest():
    """Prove header -> signature -> byte-exact inversion end-to-end.

    A synthetic SDK header declares a far-pointer store; apidb parses it; assist
    feeds msc6dec the far/int signature; the recovered C recompiles to the shipped
    bytes. WITHOUT the SDK signature, msc6dec would mislabel the params."""
    from codebreaker.sdk import apidb
    from codebreaker.inverse_engine.msc6fn import _mask
    from codebreaker.inverse_engine import msc6_oracle as ora

    header = "void far pascal SetWord(int far *p, int v);\n"
    dfile = "LIBRARY DEMO\nEXPORTS\n    SETWORD  @1\n"
    db = apidb.ApiDb().add_header(header).add_def(dfile)
    print("SDK signature SetWord:", db.sig("SETWORD"))

    # shipped bytes for  void far pascal SetWord(int far *p, int v){ *p = v; }
    code = bytes.fromhex("1e589045558bec1e8ed883ec028b4606c45e082689074d4d8be51f5d4dca0600")
    src, meta = invert(db, "SetWord", code, imports={})
    print("\n--- recovered via SDK signature ---\n" + src)
    rc = ora.mc6_compile(src, "/AM /G2 /Gsw /Ox")
    ok = _mask(rc) == _mask(code) and len(rc) == len(code)
    print("\nSDK-assisted byte-exact = %s  (sdk=%s)" % (ok, meta.get("sdk")))


if __name__ == "__main__":
    _selftest()
