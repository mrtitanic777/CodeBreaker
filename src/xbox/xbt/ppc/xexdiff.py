"""Function-level diff between two XEX builds — what actually changed.

Comparing two builds byte-for-byte is useless: every function's address moves, so
every call/branch displacement and every materialized data address differs even
where the *logic* is identical. This normalizes each function to an
address-invariant fingerprint first — masking branch displacements and the
immediate halves of `lis`+`addi`/`load`/`store` address materializations, exactly
the fields a relocation would cover — then matches functions across the two
builds by that fingerprint.

Output: how many functions are identical (same fingerprint in both), how many are
unique to each build, and a sample of the differing addresses. Handy for tracking
what changed between prerelease builds without symbols.
"""

import importlib
import struct
import sys

DESCRIPTION = "diff two XEX builds at the function level, address-invariant (what logic changed)"

WORD = 4

# D-form opcodes whose 16-bit immediate is an address low-half when the base
# register was just set by a lis (the standard 32-bit address materialization).
_DFORM_LOWHALF = {
    14: "addi", 12: "addic", 13: "addic.", 24: "ori", 26: "xori", 28: "andi",
    32: "lwz", 33: "lwzu", 34: "lbz", 35: "lbzu", 40: "lhz", 41: "lhzu",
    42: "lha", 36: "stw", 37: "stwu", 38: "stb", 39: "stbu", 44: "sth", 45: "sthu",
}


def _imp(name):
    try:
        return importlib.import_module("." + name, __package__)
    except Exception:
        return importlib.import_module(name)


def normalize(code):
    """Return an address-invariant copy of a function's bytes: branch
    displacements and lis/addi(+load/store) address immediates zeroed."""
    out = bytearray(code)
    n = len(code) - (len(code) % WORD)
    lis_reg = set()                        # regs currently holding a lis high-half
    for off in range(0, n, WORD):
        w = struct.unpack_from(">I", code, off)[0]
        op = w >> 26
        if op == 18:                       # b / bl / ba / bla — 24-bit LI
            struct.pack_into(">I", out, off, w & 0xFC000003)
            continue
        if op == 16:                       # bc / bcl — 14-bit BD
            struct.pack_into(">I", out, off, w & 0xFFFF0003)
            continue
        if op == 15:                       # addis rD,rA,imm ; lis when rA==0
            rD, rA = (w >> 21) & 0x1F, (w >> 16) & 0x1F
            if rA == 0:
                lis_reg.add(rD)
                struct.pack_into(">I", out, off, w & 0xFFFF0000)
                continue
        if op in _DFORM_LOWHALF:
            rA = (w >> 16) & 0x1F          # source/base register
            rD = (w >> 21) & 0x1F
            if rA in lis_reg:              # address low-half following a lis
                struct.pack_into(">I", out, off, w & 0xFFFF0000)
                lis_reg.discard(rD)        # rD now holds a full address, not a lis-high
                continue
            lis_reg.discard(rD)            # a normal write clears any stale flag
    return bytes(out)


def _pe_timestamp(img):
    """PE COFF TimeDateStamp (unix seconds) from an embedded PE image, or None.
    The Xbox 360 PE headers are little-endian (only the PPC code is big-endian)."""
    if len(img) < 0x40 or img[:2] != b"MZ":
        return None
    e = struct.unpack_from("<I", img, 0x3C)[0]
    if e + 12 > len(img) or img[e:e + 4] != b"PE\x00\x00":
        return None
    return struct.unpack_from("<I", img, e + 8)[0]     # COFF header +4 = TimeDateStamp


def build_time(xex_path):
    """The build's PE TimeDateStamp (unix seconds), for dating/ordering builds."""
    xex = _imp("xex")
    try:
        return _pe_timestamp(xex.open_xex(xex_path).basefile())
    except Exception:
        return None


def _fmt_time(ts):
    if not ts:
        return "unknown"
    import datetime
    try:
        return datetime.datetime.utcfromtimestamp(ts).strftime("%Y-%m-%d %H:%M:%S UTC")
    except Exception:
        return "0x%08X (invalid)" % ts


def fingerprints(xex_path):
    """{fingerprint_bytes: [addr, ...]} for every discovered function, plus the
    address->size map. Fingerprint = normalize(function bytes)."""
    ppcdiscover = _imp("ppcdiscover")
    funcs, _src, x, img, secs = ppcdiscover.discover(xex_path)
    base = x.image_base or 0

    def slice_of(a, n):
        for s in secs:
            va = base + s["vaddr"]
            if va <= a < va + s["vsize"]:
                o = s["raw_off"] + (a - va)
                return img[o:o + n]
        return b""

    fp = {}
    for a in sorted(funcs):
        size = funcs[a] or 0
        if not size:
            continue
        code = slice_of(a, size)
        if not code or code[:4] == b"\x00\x00\x00\x00":
            continue
        key = normalize(code)
        fp.setdefault(key, []).append(a)
    return fp, funcs


def string_set(src, min_len=5):
    """Set of strings in a build (address-invariant). `src` is a XEX path or raw
    bytes."""
    strings = _imp("strings")
    if isinstance(src, (bytes, bytearray)):
        data = bytes(src)
    else:
        data = _imp("xex").open_xex(src).basefile()
    return {s for _a, s in strings.scan_strings(data, 0, min_len)}


def import_set(xex_path):
    """Set of (library, ordinal, name) imports in a build (address-invariant)."""
    x = _imp("xex").open_xex(xex_path)
    try:
        ordinals = _imp("ordinals")
    except Exception:
        ordinals = None
    out = set()
    for lib, entries in x.imports().items():
        mod = lib.rsplit(".", 1)[0] if "." in lib else lib
        for _addr, ordinal, _kind in entries:
            name = None
            if ordinals is not None:
                try:
                    r = ordinals.lookup(mod, ordinal)
                    name = r[0] if r else None
                except Exception:
                    pass
            out.add((lib, ordinal, name or "ord_%d" % ordinal))
    return out


def _diff_sets(a, b):
    return sorted(a - b), sorted(b - a)


def _norm_c(text):
    """Normalize a decompilation for diffing: collapse build-specific addresses
    (sub_/L_/loc_ names and bare 0x82xxxxxx-style code addresses) so a unified
    diff shows only real logic changes, not the fact that everything moved."""
    import re
    text = re.sub(r"\b(sub|L|loc)_[0-9A-Fa-f]{8}\b", r"\1_ADDR", text)
    text = re.sub(r"\b0x8[0-9A-Fa-f]{7}\b", "0xADDR", text)
    return text


def c_diff(old_c, new_c, old_name="OLD", new_name="NEW"):
    """Unified diff of two decompilations, address-normalized. Returns a string
    (empty if they differ only in addresses)."""
    import difflib
    la = _norm_c(old_c).splitlines(keepends=True)
    lb = _norm_c(new_c).splitlines(keepends=True)
    return "".join(difflib.unified_diff(la, lb, old_name, new_name, n=2))


def _words_of(key_bytes):
    return [struct.unpack_from(">I", key_bytes, i)[0]
            for i in range(0, len(key_bytes) - 3, WORD)]


def _pair_changed(only_a, only_b, fa, fb, threshold=0.55, cap=3000):
    """Pair unmatched A/B functions by normalized-instruction similarity so a
    function that was *edited* (not added/removed) is reported as changed. Size
    bucketing keeps it fast; a hard cap disables fuzzy pairing on huge diffs.
    Returns (changed[(addrA, addrB, ratio)], removed_keys, added_keys)."""
    if not only_a or not only_b or len(only_a) > cap or len(only_b) > cap:
        return [], set(only_a), set(only_b)
    import difflib
    from collections import defaultdict
    bybucket = defaultdict(list)
    for k in only_b:
        bybucket[len(k) // 16].append(k)
    changed, used_b = [], set()
    for ka in sorted(only_a, key=len):
        la, wa = len(ka), _words_of(ka)
        best, best_r = None, threshold
        cand = []
        for bkt in range(la // 16 - 1, la // 16 + 2):
            cand.extend(bybucket.get(bkt, []))
        for kb in cand:
            if kb in used_b or abs(len(kb) - la) > max(16, la * 0.35):
                continue
            r = difflib.SequenceMatcher(None, wa, _words_of(kb)).ratio()
            if r > best_r:
                best_r, best = r, kb
        if best is not None:
            used_b.add(best)
            changed.append((fa[ka][0], fb[best][0], best_r))
    removed = {k for k in only_a if fa[k][0] not in {c[0] for c in changed}}
    added = set(only_b) - used_b
    return changed, removed, added


def diff(a_path, b_path, fuzzy=True):
    """Compare two XEX builds. Returns a dict: identical (address-invariant),
    changed (fuzzy-paired edits, with similarity), added, removed."""
    fa, _fna = fingerprints(a_path)
    fb, _fnb = fingerprints(b_path)
    ka, kb = set(fa), set(fb)
    common = ka & kb
    only_a, only_b = ka - kb, kb - ka
    matched_fns = sum(min(len(fa[k]), len(fb[k])) for k in common)
    changed, removed_k, added_k = ([], only_a, only_b)
    if fuzzy:
        changed, removed_k, added_k = _pair_changed(only_a, only_b, fa, fb)
    return {
        "functions_a": sum(len(v) for v in fa.values()),
        "functions_b": sum(len(v) for v in fb.values()),
        "identical_fingerprints": len(common),
        "matched_functions": matched_fns,
        "changed": sorted(changed, key=lambda c: c[2]),   # least similar first
        "only_in_a": sorted(a for k in removed_k for a in fa[k]),
        "only_in_b": sorted(a for k in added_k for a in fb[k]),
    }


# ── self-verification ──────────────────────────────────────────────────────
def _selfcheck():
    ppcasm = _imp("ppcasm")
    # same logic at two different addresses: the bl target and a lis/addi address
    # both move, but the normalized fingerprint must be identical.
    def build(baseaddr, tgt, dataaddr):
        src = ("mflr r0\n"
               "bl 0x%X\n" % tgt +
               "lis r3, 0x%X\n" % ((dataaddr >> 16) & 0xFFFF) +
               "addi r3, r3, 0x%X\n" % (dataaddr & 0xFFFF) +
               "lwz r4, 0(r3)\n"
               "blr\n")
        return ppcasm.assemble(src, baseaddr)
    a = build(0x82000000, 0x82010000, 0x82500000)
    b = build(0x83000000, 0x83020000, 0x82600000)   # different addr, target, data
    if a == b:
        return False, "test inputs identical before normalization (bad test)"
    if normalize(a) != normalize(b):
        return False, "normalize did not make address-varying code equal:\n%s\n%s" % (
            normalize(a).hex(), normalize(b).hex())
    # a genuine logic change (a non-address opcode) must NOT normalize away.
    # offset 16 is the `lwz r4, 0(r3)`; swap it for an unrelated `add`.
    c = bytearray(build(0x82000000, 0x82010000, 0x82500000))
    c[16:20] = struct.pack(">I", ppcasm.assemble_one("add r4, r5, r6", 0x82000010))
    if normalize(a) == normalize(bytes(c)):
        return False, "normalize erased a real opcode change"
    # the set-diff bookkeeping
    fa = {normalize(a): [0x82000000]}
    fb = {normalize(b): [0x83000000], b"zzzz": [0x83001000]}
    ka, kb = set(fa), set(fb)
    if len(ka & kb) != 1 or (kb - ka) != {b"zzzz"}:
        return False, "fingerprint set math wrong"
    # PE TimeDateStamp read (little-endian) from a synthetic embedded PE header
    img = bytearray(0x100)
    img[:2] = b"MZ"
    struct.pack_into("<I", img, 0x3C, 0x80)                # e_lfanew
    img[0x80:0x84] = b"PE\x00\x00"
    struct.pack_into("<I", img, 0x88, 0x4F75C3C7)          # TimeDateStamp @ e+8
    if _pe_timestamp(bytes(img)) != 0x4F75C3C7:
        return False, "PE timestamp read wrong: 0x%X" % (_pe_timestamp(bytes(img)) or 0)
    # fuzzy pairing: an edited function pairs as "changed"; an unrelated one does not
    f0 = ("mflr r0\nstw r0, -0x8(r1)\nadd r3, r3, r4\nmullw r3, r3, r5\n"
          "lwz r0, -0x8(r1)\nmtlr r0\nblr\n")
    f1 = f0.replace("mullw r3, r3, r5", "add r3, r3, r5")     # one-op edit
    f2 = ("li r3, 0x0\ncmpwi r4, 0x0\nbeq 0x8200000C\nli r3, 0x1\nblr\n")  # unrelated
    ka = normalize(ppcasm.assemble(f0, 0x82000000))
    kb = normalize(ppcasm.assemble(f1, 0x83000000))
    kc = normalize(ppcasm.assemble(f2, 0x84000000))
    fa2 = {ka: [0x82000000]}
    fb2 = {kb: [0x83000000], kc: [0x84000000]}
    changed, removed, added = _pair_changed(set(fa2), set(fb2), fa2, fb2)
    if len(changed) != 1 or changed[0][0] != 0x82000000 or changed[0][1] != 0x83000000:
        return False, "fuzzy pairing did not match the edited function: %r" % changed
    if changed[0][2] < 0.6 or added != {kc}:
        return False, "fuzzy pairing wrong (ratio %.2f / added %r)" % (
            changed[0][2] if changed else 0, added)
    # C diff: address-only differences vanish; a real logic change shows
    ca = "unsigned int sub_82001000(...)\n{\n    r3 = sub_82002000(r3);\n    return r3;\n}\n"
    cb = "unsigned int sub_83004000(...)\n{\n    r3 = sub_83005000(r3);\n    return r3;\n}\n"
    if c_diff(ca, cb).strip():
        return False, "c_diff flagged an address-only difference:\n" + c_diff(ca, cb)
    cc = cb.replace("return r3;", "return r3 + 1;")
    if "+ 1" not in c_diff(ca, cc):
        return False, "c_diff missed a real logic change"
    # string set extraction + diff
    sa = string_set(b"alpha\x00beta\x00hi\x00gamma", min_len=4)
    sb = string_set(b"beta\x00gamma\x00delta", min_len=4)
    if sa != {"alpha", "beta", "gamma"}:              # "hi" too short
        return False, "string_set extraction wrong: %r" % sa
    removed, added = _diff_sets(sa, sb)
    if removed != ["alpha"] or added != ["delta"]:
        return False, "string diff wrong: -%r +%r" % (removed, added)
    return True, ("address-invariant normalize + real-change detect + set math + "
                  "PE build-time + fuzzy pairing + normalized C diff + string diff")


def main():
    a = sys.argv[1:]
    if a and a[0] == "--selftest":
        ok, msg = _selfcheck()
        print(("PASS: " if ok else "FAIL: ") + msg)
        sys.exit(0 if ok else 1)
    if len(a) < 2:
        print("usage:\n"
              "  xexdiff <old.xex> <new.xex> [--show N]   function-level diff (+ optional C diff)\n"
              "  xexdiff <old.xex> <new.xex> --strings     added/removed strings\n"
              "  xexdiff <old.xex> <new.xex> --imports     added/removed imports")
        return
    if "--strings" in a:
        removed, added = _diff_sets(string_set(a[0]), string_set(a[1]))
        print("strings: %d removed, %d added" % (len(removed), len(added)))
        for tag, lst in (("- ", removed), ("+ ", added)):
            for s in lst[:60]:
                print("%s%s" % (tag, s if len(s) < 100 else s[:100] + "..."))
            if len(lst) > 60:
                print("  (+%d more)" % (len(lst) - 60))
        return
    if "--imports" in a:
        removed, added = _diff_sets(import_set(a[0]), import_set(a[1]))
        print("imports: %d removed, %d added" % (len(removed), len(added)))
        for tag, lst in (("- ", removed), ("+ ", added)):
            for lib, ordn, name in lst:
                print("%s%s  ord %d  %s" % (tag, lib, ordn, name))
        return
    ta, tb = build_time(a[0]), build_time(a[1])
    print("A  %s  built %s" % (a[0], _fmt_time(ta)))
    print("B  %s  built %s" % (a[1], _fmt_time(tb)))
    if ta and tb:
        print("   (%s is the newer build)" % ("B" if tb >= ta else "A"))
    show = int(a[a.index("--show") + 1]) if "--show" in a else 0
    r = diff(a[0], a[1])
    print("A: %d functions   B: %d functions" % (r["functions_a"], r["functions_b"]))
    print("identical (address-invariant): %d functions" % r["matched_functions"])
    print("changed (edited): %d   added: %d   removed: %d"
          % (len(r["changed"]), len(r["only_in_b"]), len(r["only_in_a"])))
    if r["changed"]:
        print("\nchanged functions (least similar first):")
        for addr_a, addr_b, ratio in r["changed"][:40]:
            print("  A sub_%08X  ->  B sub_%08X   %.0f%% similar" % (addr_a, addr_b, ratio * 100))
    for label, lst in (("added (B only)", r["only_in_b"]), ("removed (A only)", r["only_in_a"])):
        if lst:
            shown = " ".join("0x%08X" % x for x in lst[:12])
            more = "  (+%d more)" % (len(lst) - 12) if len(lst) > 12 else ""
            print("%s: %s%s" % (label, shown, more))
    if show and r["changed"]:
        decomp = _imp("ppcdecomp")
        for addr_a, addr_b, ratio in r["changed"][:show]:
            print("\n" + "=" * 70)
            print("CHANGED  A sub_%08X -> B sub_%08X  (%.0f%% similar)"
                  % (addr_a, addr_b, ratio * 100))
            old_c = decomp.from_xex(a[0], addr_a)
            new_c = decomp.from_xex(a[1], addr_b)
            d = c_diff(old_c, new_c, "sub_%08X (A)" % addr_a, "sub_%08X (B)" % addr_b)
            print(d if d.strip() else "  (differ only in addresses — no logic change shown)")


if __name__ == "__main__":
    main()
