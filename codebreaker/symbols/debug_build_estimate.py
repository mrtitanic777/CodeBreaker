"""Best-effort naming of the internal functions a debug build's exact signature
match leaves anonymous. Where debug_names.py demands byte-identity (zero false
positives, ~46 names for KRNL386), this trades certainty for coverage in graded,
MEASURED tiers -- because debug and retail are the same source in the same
in-segment order, an order-preserving alignment names many more.

Pipeline, per code segment, anchored on the validated-exact set so a fuzzy match
can never override ground truth:

  EXACT     strict signature identical (immediates kept) within the right gap
  HIGH      skeleton identical (immediates masked), order-consistent
  ALIGNED   equal-count residual gap between anchors -> positional 1:1
  PROBABLE  fuzzy mnemonic ratio >= 0.80 in the correct gap
  GUESS     fuzzy ratio 0.55-0.80

Every tier's real accuracy is reported by running the KNOWN EXPORTS through the
same pipeline and checking predictions against their true names -- so the tiers
carry evidence, not adjectives. Measured for KRNL386: EXACT/HIGH/ALIGNED ~100%,
PROBABLE ~48%, GUESS ~9%. Only EXACT+HIGH+ALIGNED are emitted as real labels
(via confident_names); PROBABLE/GUESS are returned for a catalog as leads and
must never be applied as bare labels.
"""
import os, re, difflib
from collections import Counter

from codebreaker.formats import ne as ne_mod
from codebreaker.source.ne_asm import segment as _t
from codebreaker.symbols import debug_build as dn
from codebreaker.base import corpus as paths, settings

CONFIDENT_TIERS = ("EXACT", "HIGH", "ALIGNED")
PROBABLE_MIN, FUZZY_MIN = 0.80, 0.55


def _skel(sg, start):
    _, rows = sg.func_rows(start)
    return tuple(re.sub(r"0x[0-9a-f]+", "X", re.sub(r"\s+", " ", a).strip())
                 for _off, a in rows)


def _mnem(sg, start):
    _, rows = sg.func_rows(start)
    return [a.split()[0] if a.split() else "?" for _off, a in rows]


def _map_names(debug_map):
    names = {}
    txt = open(debug_map, errors="replace").read()
    for m in re.finditer(r"^ ([0-9A-Fa-f]{4}):([0-9A-Fa-f]{4})\s+(\w+)", txt, re.M):
        names.setdefault(int(m.group(1), 16), {})[int(m.group(2), 16)] = m.group(3)
    return names


def _align_segment(i, rsg, dsg, dbg_names, exact_map, predict_exports=False):
    """{retail_off -> (name, tier, score)} for internal (or export) functions of
    segment i, anchored on exact_map."""
    d_by_name = {}
    for doff, nm in dbg_names[i].items():
        d_by_name.setdefault(nm, doff)
    r_offs = sorted(rsg.funcs)
    d_offs = sorted(off for off in dbg_names[i] if off in dsg.valid)
    r_skel = {off: _skel(rsg, off) for off in r_offs}
    d_skel = {off: _skel(dsg, off) for off in d_offs}
    r_strict = {off: dn.norm_sig(rsg, off) for off in r_offs}
    d_strict = {off: dn.norm_sig(dsg, off) for off in d_offs}
    r_mnem = {off: _mnem(rsg, off) for off in r_offs}
    d_mnem = {off: _mnem(dsg, off) for off in d_offs}

    def is_target(off):
        nm = rsg.name_at(off)
        exp = nm and not nm.lower().startswith("sub_")
        return exp if predict_exports else (not exp)

    # order-consistent anchors from the validated-exact set
    anch = []
    for roff in r_offs:
        nm = exact_map.get("%d:%04x" % (i, roff))
        if nm and nm in d_by_name:
            anch.append((roff, d_by_name[nm]))
    keep = []
    for a in anch:
        while keep and keep[-1][1] >= a[1]:
            keep.pop()
        keep.append(a)
    anch = keep

    out = {}
    bounds = [(-1, -1)] + anch + [(1 << 30, 1 << 30)]
    for (ra, da), (rb, db) in zip(bounds, bounds[1:]):
        rgap = [x for x in r_offs if ra < x < rb]
        dgap = [x for x in d_offs if da < x < db]
        used = set()
        sm = difflib.SequenceMatcher(
            None, [r_skel[x] for x in rgap], [d_skel[x] for x in dgap], autojunk=False)
        for ai, bj, size in sm.get_matching_blocks():
            for k in range(size):
                roff, doff = rgap[ai + k], dgap[bj + k]
                if not is_target(roff):
                    continue
                tier = "EXACT" if r_strict[roff] == d_strict[doff] else "HIGH"
                out[roff] = (dbg_names[i][doff], tier, 1.0)
                used.add(doff)
        rrem = [x for x in rgap if x not in out]
        drem = [x for x in dgap if x not in used]
        if rrem and len(rrem) == len(drem):
            for roff, doff in zip(rrem, drem):
                if is_target(roff):
                    sc = difflib.SequenceMatcher(None, r_mnem[roff], d_mnem[doff]).ratio()
                    out[roff] = (dbg_names[i][doff], "ALIGNED", round(sc, 2))
                    used.add(doff)
            continue
        for roff in rgap:
            if roff in out or not is_target(roff):
                continue
            best, bs = None, 0.0
            for doff in dgap:
                if doff in used:
                    continue
                sc = difflib.SequenceMatcher(None, r_mnem[roff], d_mnem[doff]).ratio()
                if sc > bs:
                    best, bs = doff, sc
            if best is not None and bs >= FUZZY_MIN:
                out[roff] = (dbg_names[i][best],
                             "PROBABLE" if bs >= PROBABLE_MIN else "GUESS", round(bs, 2))
                used.add(best)
    return out


def estimate_module(retail_exe, debug_exe, debug_map, ordinals):
    """Return (tiers, accuracy): tiers = {"seg:off" -> (name, tier, score)} over
    internal functions; accuracy = {tier -> (correct, total)} scored on exports."""
    dbg_names = _map_names(debug_map)
    dbg_ne = ne_mod.NEFile(debug_exe)
    ret_ne = ne_mod.NEFile(retail_exe)
    idx = dn.build_debug_index(debug_exe, debug_map, ordinals)
    exact_map, _ = dn.match_module(retail_exe, idx, ordinals)

    tiers, acc = {}, {t: [0, 0] for t in ("EXACT", "HIGH", "ALIGNED", "PROBABLE", "GUESS")}
    for i, s in enumerate(ret_ne.segments(), 1):
        if s.get("kind") != "CODE" or i not in dbg_names:
            continue
        rsg, dsg = _t.Seg(ret_ne, i, ordinals), _t.Seg(dbg_ne, i, ordinals)
        for roff in rsg.funcs:                       # lock validated anchors
            nm = exact_map.get("%d:%04x" % (i, roff))
            if nm:
                tiers["%d:%04x" % (i, roff)] = (nm, "EXACT", 1.0)
        for roff, v in _align_segment(i, rsg, dsg, dbg_names, exact_map).items():
            tiers.setdefault("%d:%04x" % (i, roff), v)
        for roff, v in _align_segment(i, rsg, dsg, dbg_names, exact_map, True).items():
            real = rsg.name_at(roff)
            acc[v[1]][1] += 1
            acc[v[1]][0] += (v[0].upper() == real.upper())
    return tiers, acc


def confident_names(tiers, retail_exe, ordinals):
    """The EXACT+HIGH+ALIGNED subset, guarded against export-name collisions and
    duplicate names -- safe to apply as real labels."""
    ne = ne_mod.NEFile(retail_exe)
    exp_at = {}
    for i, s in enumerate(ne.segments(), 1):
        if s.get("kind") != "CODE":
            continue
        for off, n in _t.Seg(ne, i, ordinals).export_names.items():
            exp_at[n.upper()] = (i, off)
    conf = {}
    for k, (name, tier, _score) in tiers.items():
        if tier not in CONFIDENT_TIERS:
            continue
        si, off = int(k.split(":")[0]), int(k.split(":")[1], 16)
        clash = exp_at.get(name.upper())
        if clash and clash != (si, off):
            continue
        conf[k] = name
    cnt = Counter(conf.values())
    dup = {n for n, c in cnt.items() if c > 1}
    return {k: v for k, v in conf.items() if v not in dup}


if __name__ == "__main__":
    import json
    ordinals = _t.load_ordinals("ORDINALS.md")
    retail = paths.retail("krnl386.exe")
    dbg = paths.reference("WIN32SDK", "SCT", "DEBUG16", "I386")
    tiers, acc = estimate_module(
        retail, os.path.join(dbg, "KRNL386.EXE"), os.path.join(dbg, "KRNL386.MAP"), ordinals)
    conf = confident_names(tiers, retail, ordinals)
    out = settings.state("idiomatic", "krnl386")
    os.makedirs(out, exist_ok=True)
    json.dump(conf, open(os.path.join(out, "names.json"), "w"), indent=0, sort_keys=True)
    for t in ("EXACT", "HIGH", "ALIGNED", "PROBABLE", "GUESS"):
        c, tot = acc[t]
        n = sum(1 for v in tiers.values() if v[1] == t)
        a = ("%d/%d=%.0f%%" % (c, tot, 100.0 * c / tot)) if tot else "-"
        print("  %-9s %4d   export-accuracy %s" % (t, n, a))
    print("confident names.json (EXACT+HIGH+ALIGNED, guarded): %d" % len(conf))
