#!/usr/bin/env python3
r"""loops.py -- counted-loop synthesis (memset/fill class), verified by the oracle.

The first rung of multi-statement reconstruction (system B): a back-edge loop that writes
N consecutive elements through a pointer -- the memset/fill idiom. Recognized EMPIRICALLY
and faithfully: point the pointer arg at a controlled buffer, vary the count arg, and

  1. confirm the number of buffer writes SCALES with the count arg (the loop-trip signal),
  2. read the element size (store width) and the per-element VALUE across indices + counts,
     classifying it as a constant, a scalar arg (memset fill), or the index i (fill-with-i),
  3. emit `for (i = 0; i < n; i++) ((T*)p)[i] = <value>;`,
  4. DIFFERENTIALLY VERIFY across fresh counts (and, for an arg fill, fresh fill values):
     the loop must reproduce every write the binary makes.

Faithful-only, byte-checked, no gate-busting. Scope (this rung): one pointer, a count arg,
writes 1:1 with the count, value = const / scalar-arg / index. memcpy (two pointers) and
loops whose count is a struct field or a byte-length (n/elemsize) are the next rungs;
anything not confirmed is declined, not faked.

    wt behavior loops <module> [func] [--ptr N] [--count N]
"""
import os
import sys

from codebreaker.engines.behavior import tracer, struct as S
from codebreaker.cli.common import opt as _opt

MASK = 0xFFFFFFFF
BUF = S.BUF_BASE                                          # dst buffer (struct's scratch window)


def _run(module, func, arity, dst_i, cnt_i, cnt, insn_cap, fill=None, argvals=None):
    """Trace with dst->buffer and count=cnt; return writes scoped to the buffer."""
    args = [0] * arity
    args[dst_i] = BUF
    if cnt_i is not None:
        args[cnt_i] = cnt
    for i, v in (argvals or {}).items():
        args[i] = v & MASK
    t = tracer.trace_function(module, func=func, args=args, insn_cap=insn_cap,
                              mem_init=[(BUF, S._fill(fill))])
    wr = [{"off": m["addr"] - BUF, "size": m["size"], "value": m["value"]}
          for m in t.get("mem_writes", []) if S._in_buf(m["addr"])]
    return t, wr


def _find_ptr_count(module, func, arity, insn_cap):
    """(dst_i, cnt_i, elemsize) where pointing dst at the buffer and raising cnt scales the
    number of buffer writes, or None. The empirical loop-trip signal."""
    for dst_i in range(min(arity, 4)):
        for cnt_i in range(min(arity, 6)):
            if cnt_i == dst_i:
                continue
            try:
                _t4, w4 = _run(module, func, arity, dst_i, cnt_i, 4, insn_cap)
                _t8, w8 = _run(module, func, arity, dst_i, cnt_i, 8, insn_cap)
            except Exception:
                continue
            # writes scale ~1:1 with the count arg, and cover distinct offsets
            if len(w4) == 4 and len(w8) == 8 and len({w["size"] for w in w8}) == 1:
                return dst_i, cnt_i, w8[0]["size"]
    return None


SRC = BUF + 0x1000                                       # second buffer for the memcpy (src) idiom


def _try_strlen(module, func, arity, insn_cap, func_va):
    """scan-to-sentinel idiom: takes a pointer, returns the index of the first 0 byte (strlen).
    Verified empirically -- plant a 0 terminator at several positions; the return must track it."""
    for pi in range(min(arity, 4)):
        rets, ok = [], True
        for K in (3, 5, 8, 12):
            buf = bytearray(b"\xAA" * 48); buf[K] = 0
            args = [0] * arity; args[pi] = BUF
            try:
                t = tracer.trace_function(module, func=func, args=args, insn_cap=insn_cap,
                                          mem_init=[(BUF, bytes(buf))])
            except Exception:
                ok = False; break
            if t.get("exit") != "ret":
                ok = False; break
            rets.append((K, t.get("ret")))
        if ok and rets and all(r == K for K, r in rets):     # return == terminator index -> strlen
            src = ("unsigned sub_%x(const char *p)\n{\n    unsigned i = 0;\n"
                   "    while (p[i]) i++;\n    return i;\n}\n" % func_va)
            return {"kind": "strlen", "ptr_arg": pi, "elem_size": 1, "value": "strlen(scan to 0)",
                    "emitted_c": src, "verified": True, "tested": len(rets)}
    return None


def _try_memcpy(module, func, arity, insn_cap, func_va):
    """two-pointer copy idiom: dst[i] = src[i] for i in 0..n. Verified: dst writes scale 1:1 with a
    count arg AND each written value equals the planted src byte at the same offset."""
    srcbytes = bytes((0x11 + (j * 7) & 0xFF) for j in range(64))
    for di in range(min(arity, 3)):
        for si in range(min(arity, 3)):
            if si == di:
                continue
            for ci in range(min(arity, 6)):
                if ci in (di, si):
                    continue
                ok, esz = True, None
                for cnt in (4, 7):
                    args = [0] * arity; args[di] = BUF; args[si] = SRC; args[ci] = cnt
                    try:
                        t = tracer.trace_function(module, func=func, args=args, insn_cap=insn_cap,
                                                  mem_init=[(BUF, bytes(64)), (SRC, srcbytes)])
                    except Exception:
                        ok = False; break
                    wr = [m for m in t.get("mem_writes", []) if BUF <= m["addr"] < BUF + 64]
                    if len(wr) != cnt or len({w["size"] for w in wr}) != 1:
                        ok = False; break
                    esz = wr[0]["size"]
                    for w in wr:
                        o = w["addr"] - BUF
                        exp = int.from_bytes(srcbytes[o:o + w["size"]], "little")
                        if (w["value"] & ((1 << (8 * w["size"])) - 1)) != exp:
                            ok = False; break
                    if not ok:
                        break
                if ok and esz:
                    ct = {1: "unsigned char", 2: "unsigned short"}.get(esz, "unsigned")
                    src = ("void sub_%x(%s *dst, const %s *src, unsigned n)\n{\n"
                           "    for (unsigned i = 0; i < n; i++)\n        dst[i] = src[i];\n}\n"
                           % (func_va, ct, ct))
                    return {"kind": "memcpy", "ptr_arg": di, "src_arg": si, "count_arg": ci,
                            "elem_size": esz, "value": "src[i] (copy)", "emitted_c": src,
                            "verified": True, "tested": 2}
    return None


def _try_wcslen(module, func, arity, insn_cap, func_va):
    """wide scan-to-sentinel: return the index of the first 0 WCHAR (wcslen)."""
    for pi in range(min(arity, 4)):
        rets, ok = [], True
        for K in (3, 5, 8):
            buf = bytearray(b"\xAA\xAA" * 24); buf[K * 2] = 0; buf[K * 2 + 1] = 0
            args = [0] * arity; args[pi] = BUF
            try:
                t = tracer.trace_function(module, func=func, args=args, insn_cap=insn_cap,
                                          mem_init=[(BUF, bytes(buf))])
            except Exception:
                ok = False; break
            if t.get("exit") != "ret":
                ok = False; break
            rets.append((K, t.get("ret")))
        if ok and rets and all(r == K for K, r in rets):
            src = ("unsigned sub_%x(const wchar_t *p)\n{\n    unsigned i = 0;\n"
                   "    while (p[i]) i++;\n    return i;\n}\n" % func_va)
            return {"kind": "wcslen", "ptr_arg": pi, "elem_size": 2, "value": "wcslen(scan to 0)",
                    "emitted_c": src, "verified": True, "tested": len(rets)}
    return None


def _try_strcpy(module, func, arity, insn_cap, func_va):
    """two-pointer copy-until-0 (strcpy): copy src bytes through the terminator into dst."""
    for di in range(min(arity, 3)):
        for si in range(min(arity, 3)):
            if si == di:
                continue
            ok = True
            for K in (3, 5, 8):
                src = bytearray((0x41 + (j % 26)) for j in range(32)); src[K] = 0
                args = [0] * arity; args[di] = BUF; args[si] = SRC
                try:
                    t = tracer.trace_function(module, func=func, args=args, insn_cap=insn_cap,
                                              mem_init=[(BUF, bytes(32)), (SRC, bytes(src))])
                except Exception:
                    ok = False; break
                wr = [m for m in t.get("mem_writes", []) if BUF <= m["addr"] < BUF + 32
                      and m["size"] == 1]
                if len(wr) != K + 1:                          # copies src[0..K] incl the 0 terminator
                    ok = False; break
                for w in wr:
                    o = w["addr"] - BUF
                    if o > K or (w["value"] & 0xFF) != src[o]:
                        ok = False; break
                if not ok:
                    break
            if ok:
                s = ("char *sub_%x(char *dst, const char *src)\n{\n    char *d = dst;\n"
                     "    while ((*d++ = *src++) != 0)\n        ;\n    return dst;\n}\n" % func_va)
                return {"kind": "strcpy", "ptr_arg": di, "src_arg": si, "elem_size": 1,
                        "value": "strcpy (copy to 0)", "emitted_c": s, "verified": True, "tested": 3}
    return None


def _try_strcat(module, func, arity, insn_cap, func_va):
    """append-until-0 (strcat): copy src into dst starting at dst's existing terminator."""
    for di in range(min(arity, 3)):
        for si in range(min(arity, 3)):
            if si == di:
                continue
            ok = True
            for L, K in ((3, 4), (5, 3)):
                dst = bytearray((0x61 + (j % 26)) for j in range(40)); dst[L] = 0
                src = bytearray((0x41 + (j % 26)) for j in range(32)); src[K] = 0
                args = [0] * arity; args[di] = BUF; args[si] = SRC
                try:
                    t = tracer.trace_function(module, func=func, args=args, insn_cap=insn_cap,
                                              mem_init=[(BUF, bytes(dst)), (SRC, bytes(src))])
                except Exception:
                    ok = False; break
                wr = [m for m in t.get("mem_writes", []) if BUF + L <= m["addr"] < BUF + 40
                      and m["size"] == 1]
                if len(wr) != K + 1:                          # appends src[0..K] at the old terminator
                    ok = False; break
                for w in wr:
                    o = (w["addr"] - BUF) - L
                    if not (0 <= o <= K) or (w["value"] & 0xFF) != src[o]:
                        ok = False; break
                if not ok:
                    break
            if ok:
                s = ("char *sub_%x(char *dst, const char *src)\n{\n    char *d = dst;\n"
                     "    while (*d) d++;\n    while ((*d++ = *src++) != 0)\n        ;\n"
                     "    return dst;\n}\n" % func_va)
                return {"kind": "strcat", "ptr_arg": di, "src_arg": si, "elem_size": 1,
                        "value": "strcat (append to 0)", "emitted_c": s, "verified": True, "tested": 2}
    return None


def _sign(x):
    return (x > 0) - (x < 0)


def _try_strcmp(module, func, arity, insn_cap, func_va):
    """two-pointer compare-until-0 (strcmp): 0 if equal, else the sign of the first differing byte."""
    for ai in range(min(arity, 3)):
        for bi in range(min(arity, 3)):
            if bi == ai:
                continue
            cases = [(b"ABCDE\x00", b"ABCDE\x00", 0),        # equal
                     (b"ABCDE\x00", b"ABCDF\x00", -1),       # a < b at pos 4
                     (b"ABCDX\x00", b"ABCDF\x00", 1)]        # a > b at pos 4
            ok = True
            for A, B, want in cases:
                args = [0] * arity; args[ai] = BUF; args[bi] = SRC
                try:
                    t = tracer.trace_function(module, func=func, args=args, insn_cap=insn_cap,
                                              mem_init=[(BUF, A + b"\x00" * 8), (SRC, B + b"\x00" * 8)])
                except Exception:
                    ok = False; break
                if t.get("exit") != "ret" or _sign(_signed(t.get("ret"))) != want:
                    ok = False; break
            if ok:
                s = ("int sub_%x(const char *a, const char *b)\n{\n"
                     "    while (*a && *a == *b) { a++; b++; }\n"
                     "    return (unsigned char)*a - (unsigned char)*b;\n}\n" % func_va)
                return {"kind": "strcmp", "ptr_arg": ai, "src_arg": bi, "elem_size": 1,
                        "value": "strcmp (compare to 0)", "emitted_c": s, "verified": True, "tested": 3}
    return None


def _try_memcmp(module, func, arity, insn_cap, func_va):
    """two-pointer counted compare (memcmp): 0 if the first n bytes are equal, else nonzero."""
    for ai in range(min(arity, 3)):
        for bi in range(min(arity, 3)):
            if bi == ai:
                continue
            for ci in range(min(arity, 6)):
                if ci in (ai, bi):
                    continue
                base = bytes((0x11 + j) & 0xFF for j in range(16))
                diff = bytearray(base); diff[3] ^= 0xFF
                ok = True
                for A, B, n, eqwant in ((base, base, 8, True), (base, bytes(diff), 8, False),
                                        (base, bytes(diff), 2, True)):   # diff at 3 is outside n=2
                    args = [0] * arity; args[ai] = BUF; args[bi] = SRC; args[ci] = n
                    try:
                        t = tracer.trace_function(module, func=func, args=args, insn_cap=insn_cap,
                                                  mem_init=[(BUF, A), (SRC, B)])
                    except Exception:
                        ok = False; break
                    if t.get("exit") != "ret":
                        ok = False; break
                    if (t.get("ret") == 0) != eqwant:
                        ok = False; break
                if ok:
                    s = ("int sub_%x(const void *a, const void *b, unsigned n)\n{\n"
                         "    const unsigned char *p = a, *q = b;\n"
                         "    for (unsigned i = 0; i < n; i++)\n"
                         "        if (p[i] != q[i]) return p[i] - q[i];\n    return 0;\n}\n" % func_va)
                    return {"kind": "memcmp", "ptr_arg": ai, "src_arg": bi, "count_arg": ci,
                            "elem_size": 1, "value": "memcmp (counted compare)", "emitted_c": s,
                            "verified": True, "tested": 3}
    return None


def _signed(v):
    if v is None:
        return 0
    v &= 0xFFFFFFFF
    return v - 0x100000000 if v >= 0x80000000 else v


def reconstruct(module, func=None, ptr_i=None, cnt_i=None, insn_cap=8000):
    base = tracer.trace_function(module, func=func, insn_cap=insn_cap)
    res = {"schema": "cb.behavior.loops/1", "module": os.path.basename(base["module"]),
           "func_va": base["func_va"], "func_off": base["func_off"], "reconstructed": False}
    from codebreaker.engines.behavior import perturb
    try:
        _b, rep = perturb.run_experiment(module, func=func, insn_cap=insn_cap)
        arity = max(rep["arity"], 6)
    except Exception:
        arity = 6
    if ptr_i is not None and cnt_i is not None:
        _t8, w8 = _run(module, func, arity, ptr_i, cnt_i, 8, insn_cap)
        found = (ptr_i, cnt_i, w8[0]["size"]) if len(w8) == 8 and len({w["size"] for w in w8}) == 1 else None
    else:
        found = _find_ptr_count(module, func, arity, insn_cap)
    if not found:
        # memset/fill declined -> try the other counted-loop idioms (most specific first).
        for rec in (_try_strlen(module, func, arity, insn_cap, res["func_va"]),
                    _try_wcslen(module, func, arity, insn_cap, res["func_va"]),
                    _try_memcpy(module, func, arity, insn_cap, res["func_va"]),
                    _try_strcpy(module, func, arity, insn_cap, res["func_va"]),
                    _try_strcat(module, func, arity, insn_cap, res["func_va"]),
                    _try_strcmp(module, func, arity, insn_cap, res["func_va"]),
                    _try_memcmp(module, func, arity, insn_cap, res["func_va"])):
            if rec:
                res.update(reconstructed=True, **rec)
                return res
        res["reason"] = "no counted write-loop (buffer writes don't scale 1:1 with an arg)"
        return res
    dst_i, cnt_i, esz = found
    ctype = {1: "unsigned char", 2: "unsigned short"}.get(esz, "unsigned")

    # classify the per-element value across indices AND counts
    runs = []
    for cnt in (3, 5, 6, 9):
        av = {i: 0 for i in range(arity)}
        for fillarg in range(arity):                     # give each scalar arg a distinct probe value
            if fillarg not in (dst_i, cnt_i):
                av[fillarg] = 0xA0 + fillarg
        _t, wr = _run(module, func, arity, dst_i, cnt_i, cnt, insn_cap, argvals=av)
        runs.append((cnt, av, {w["off"] // (esz or 1): (w["value"], w["off"]) for w in wr}))
    mask = (1 << (8 * esz)) - 1

    def const_val():
        vals = {v[0] & mask for _c, _a, wm in runs for v in wm.values()}
        return vals.pop() if len(vals) == 1 else None

    def arg_fill():
        for fa in range(arity):
            if fa in (dst_i, cnt_i):
                continue
            if all((v[0] & mask) == (av[fa] & mask) for _c, av, wm in runs for v in wm.values()):
                return fa
        return None

    def index_fill():
        if all((v[0] & mask) == ((idx * (1 if esz == 1 else 1)) & mask)
               for _c, _a, wm in runs for idx, v in wm.items()):
            return True
        return False

    cv, fa = const_val(), arg_fill()
    if fa is not None:
        expr, desc = ("a%d" % fa, "a%d (fill arg)" % fa)
    elif cv is not None:
        expr, desc = ("0x%xU" % cv, "0x%x (const)" % cv)
    elif index_fill():
        expr, desc = ("i", "i (index)")
    else:
        res["reason"] = "loop writes a value not yet modeled (not const/arg/index)"
        return res

    # verify on fresh counts (+ fresh fill value if arg-filled)
    ok, tested = True, 0
    for cnt in (2, 7, 11, 13):
        av = {i: 0 for i in range(arity)}
        if fa is not None:
            av[fa] = 0x1234 + cnt
        _t, wr = _run(module, func, arity, dst_i, cnt_i, cnt, insn_cap, argvals=av)
        if len(wr) != cnt:
            ok = False; break
        for w in wr:
            idx = w["off"] // (esz or 1)
            exp = (av[fa] if fa is not None else cv if cv is not None else idx) & mask
            if (w["value"] & mask) != exp:
                ok = False; break
        tested += 1
        if not ok:
            break

    params = ", ".join(("%s *p" % ctype if i == dst_i else "unsigned n" if i == cnt_i
                        else "unsigned a%d" % i) for i in range(max(dst_i, cnt_i) + 1))
    body = "    for (unsigned i = 0; i < n; i++)\n        p[i] = %s;" % expr
    csrc = "void sub_%x(%s)\n{\n%s\n}\n" % (res["func_va"], params, body)
    res.update(reconstructed=True, verified=ok, ptr_arg=dst_i, count_arg=cnt_i, elem_size=esz,
               value=desc, tested_counts=tested, emitted_c=csrc)
    return res


def main():
    argv = sys.argv[1:]
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__); return
    module = argv[0]
    func = next((a for a in argv[1:] if not a.startswith("-")), None)
    ptr = _opt(argv, "--ptr"); ptr = int(ptr) if ptr is not None else None
    cnt = _opt(argv, "--count"); cnt = int(cnt) if cnt is not None else None
    res = reconstruct(module, func=func, ptr_i=ptr, cnt_i=cnt)
    print("loop synthesis: %s  func VA 0x%x" % (res["module"], res["func_va"]))
    if not res["reconstructed"]:
        print("  not reconstructed: %s" % res["reason"]); return
    print("  ptr=arg%d  count=arg%d  elem=%dB  value=%s" %
          (res["ptr_arg"], res["count_arg"], res["elem_size"], res["value"]))
    print("  differential verification vs binary: %s" %
          ("VERIFIED" if res["verified"] else "PARTIAL"))
    print("\n  %s" % res["emitted_c"].replace("\n", "\n  ").rstrip())


if __name__ == "__main__":
    main()
