#!/usr/bin/env python3
"""
massgen.py — generate evidence-backed reconstruction skeletons for a WHOLE
SEGMENT at a time, instead of one function per hand-written file.

WHY THIS EXISTS
---------------
Hand-writing each reconstruction as prose is the bottleneck, not decoding.
Everything a faithful skeleton needs is already mechanically extractable from
the binary, and none of it involves judgement:

    the entry address and extent          the `RETF` -> the argument count
    every call it makes, in order,        the globals it reads and writes
      with names resolved                 the struct fields it touches
    the dispatch table, expanded          the constants it compares against

So this emits, per segment, one .c file containing one skeleton per function
with all of that filled in from the binary. What it CANNOT do is reconstruct
control flow or explain intent — and it does not pretend to.

An auto-generated skeleton is NOT an understood function, so it is never counted
as one: every generated file is stamped AUTO-GENERATED, every body carries an
`[AUTO]` marker, and `ledger.py` counts them in their own column. The call list
is extracted from the binary, so `verify.py` passes by construction — a clean
verify on generated code proves the extraction is faithful, not that anyone
understood it. Promotion to a real reconstruction happens when a human reads it
and removes the `[AUTO]` marker. What this buys: every function's call graph,
argument count and global usage lands in the tree at once, so the remaining work
is reading and explaining rather than transcribing.

    python massgen.py USER 1              one segment -> reconstructed/auto/
    python massgen.py USER                every segment in the module
    python massgen.py --all               every module
    python massgen.py USER 1 --stats      what would be generated, no writing
"""
import os
import re
import sys

import idord
import trace
from codebreaker.foundation import layout

WM = getattr(trace, "WM", {})
OUT = layout.generated_dir()

# Field names this project has PROVEN. Everything here traces to a specific
# finding recorded in ORDINALS.md; anything not proven stays a raw offset,
# because a wrong field name is worse than no field name.
WND_FIELDS = {
    0x04: "hClass", 0x08: "hMenu", 0x0c: "hQueue", 0x10: "hOwnDC",
    0x1e: "rc.left", 0x20: "rc.top", 0x22: "rc.right",
    0x26: "rcClient.left", 0x28: "rcClient.top",
    0x2a: "rcClient.right", 0x2c: "rcClient.bottom",
    0x2e: "bFlags", 0x30: "dwStyle", 0x32: "wStyleHi", 0x33: "bKind",
}
CLS_FIELDS = {
    0x00: "next", 0x04: "atom", 0x06: "hClassDC", 0x0a: "style",
    0x0c: "lpfnWndProc", 0x10: "cbClsExtra", 0x12: "cbWndExtra",
    0x14: "hInstance", 0x16: "hIcon", 0x18: "hCursor", 0x1a: "hbrBackground",
}
# USER globals proven by globals.py / the Phase 2 work.
GLOBALS = {
    0x16: "cTopLevelWindows", 0x14: "hClassListHead", 0x5c: "hwndUnderMouse",
    0x5e: "dcInterval", 0x62: "hwndFocus", 0x64: "hwndActive",
    0x82: "keyState[]", 0x1fc: "fInCallback", 0x3ae: "dcMessage",
    0x3b8: "dcWindow", 0x3d2: "dcRect", 0x506: "cyCaption",
    0x5ba: "hPopupList", 0x5da: "hTopLevelList", 0x63a: "hitResult",
    0x63c: "hitCode", 0x634: "hwndDesktop", 0x748: "dcDeadline",
}


# Internal routines this project has PROVEN, by module. massgen resolves calls
# to exports automatically from the entry table; these are the NON-exported
# routines whose identity was established by reading them. Every name added
# here improves every skeleton that calls it, retroactively — which is why this
# table is the highest-leverage thing in the file.
INTERNALS = {
    "GDI": {
        0x15f6: "GdiLocalAllocA",    # gdi_alloc.c — heap ds:0x224
        0x160f: "GdiLocalAllocB",    # heap ds:0x226
        0x1615: "GdiGlobalAlloc",    # global pool, flags ds:0x21e
        0x1a12: "ConicSetup",        # shared by GDI.306 and GDI.307
    },
    "USER": {
        0x889f: "SharedProlog",      # 142 call sites — the plain shared
                                     # far-proc prologue: pops the return
                                     # address, reads a 2-byte DESCRIPTOR that
                                     # follows the call, steps over it, carves
                                     # locals. The descriptor is DATA, which is
                                     # why Ghidra mis-decodes 34% of seg1.
        0x885c: "SharedPrologChk",   # 94 sites — the SAME idiom PLUS a stack
                                     # probe: compares the new SP against the
                                     # limit at ss:0x0a and refuses to go past
                                     # it, and tracks the low-water mark in
                                     # ss:0x0c. Two prologues, one checked.
        0x6092: "GetWndPtr",         # validated in user_defwndproc.c
        0x608d: "ValidateWindow",
        0x09e2: "FindClass",         # the class-list walker (user_core.c)
        0x2758: "WindowFromEvent",   # user_hittest.c
        0x2c92: "ProcessKeyEvent",   # user_keyboard.c
        0x298e: "InputEventToMessage",
        0x2de1: "DeliverKeyMessage",
        0x5e99: "PostToQueue",
        0x6015: "WakeQueueTask",
        0x3c8e: "AllocWindowDC",     # NOT "LinkWindow" — see user_classdc.c
        0x3cd1: "AllocDC",
        0x44d7: "PaintNonClient",
        0x5126: "NcHitTest",
        0x40b0: "SysCommand",
        0x07e5: "DestroyWindowWorker",
        0x0232: "CreateWindow",
        0x679b: "DefWindowProc",
        0x253f: "SendMessage",
        0x5317: "AllocWindow",
        0x610c: "GetCaptionRect",
        0x3bf1: "GetNcDC",
        0x3c71: "ReleaseNcDC",
        0x64de: "PtInRectFar",
        0x2706: "InRange",
        0x2725: "CanReceiveInput",
        0x4ffa: "SearchWindowTree",
        0x87a8: "SetCursorTo",
        0x1aff: "FillWindow",
    },
}


def resolve(sg, off, asm, internals=None):
    if not asm.startswith(("call", "jmp")):
        return None
    try:
        kind, d = sg.resolve_call(off, asm)
    except Exception:
        return None
    if kind == "sys":
        return d[1] or d[0]
    if kind == "far":
        return str(d)
    if kind == "int":
        if internals and d in internals:
            return internals[d]
        return "sub_%04x" % d
    return None


def analyse(sg, start, end, internals=None):
    """Everything mechanically true about one function."""
    rows = trace.disasm_whole(sg.code[start:end], base=start)
    info = {"calls": [], "reads": set(), "writes": set(), "fields": {},
            "consts": [], "retf": None, "size": end - start, "rows": len(rows)}
    for off, asm in rows:
        t = resolve(sg, off, asm, internals)
        if t and asm.startswith("call"):
            info["calls"].append((off, t))
        m = re.match(r"ret[f]?\s+0x([0-9a-f]+)", asm)
        if m:
            info["retf"] = int(m.group(1), 16)
        for m in re.finditer(r"ds:0x([0-9a-f]+)", asm):
            a = int(m.group(1), 16)
            head = asm.split()[0]
            wrote = (asm.split(",")[0].find("ds:") >= 0 and
                     head in ("mov", "inc", "dec", "add", "sub", "or", "and", "xor"))
            (info["writes"] if wrote else info["reads"]).add(a)
        for m in re.finditer(r"\[(si|bx|di)\+0x([0-9a-f]+)\]", asm):
            k = (m.group(1), int(m.group(2), 16))
            info["fields"][k] = info["fields"].get(k, 0) + 1
        m = re.match(r"cmp\s+\w+,0x([0-9a-f]+)", asm)
        if m:
            v = int(m.group(1), 16)
            if v not in info["consts"]:
                info["consts"].append(v)
    return info


def gname(a):
    """No nested comments: these strings go INSIDE a /* ... */ block."""
    return "ds:0x%x=%s" % (a, GLOBALS[a]) if a in GLOBALS else "ds:0x%x" % a


def fname(reg, off):
    """Only si and bx are named. By this binary's convention si holds the
    window and bx the class; di is used for both and for unrelated pointers,
    so naming its offsets would be a guess, and a wrong field name is worse
    than a raw offset."""
    tbl = {"bx": CLS_FIELDS, "si": WND_FIELDS}.get(reg)
    nm = tbl.get(off) if tbl else None
    return "%s+0x%02x%s" % (reg, off, "=%s" % nm if nm else "")


def cident(t):
    """A call target as a legal C identifier: USER.330 -> USER_330."""
    return re.sub(r"[^A-Za-z0-9_]", "_", t)


def emit(mod, seg, sg, starts, names, fh, internals):
    fh.write('''/*
 * %s segment %d — AUTO-GENERATED RECONSTRUCTION SKELETONS
 *
 * *** GENERATED BY massgen.py. NOT A HAND-VERIFIED RECONSTRUCTION. ***
 *
 * Every fact below is extracted mechanically from the 1985 binary: entry
 * address, extent, argument count from the `RETF`, the calls each function
 * makes in the order it makes them, the globals it touches, the structure
 * fields it reads. All of that is evidence and can be relied on.
 *
 * What is NOT here is control flow, intent, or meaning. These are skeletons.
 * A function still carrying its `[AUTO]` marker has NOT been read by a human.
 * Removing that marker is what promotes it to a real reconstruction, and
 * `ledger.py` counts the two separately so this file can never inflate the
 * project's completion figure.
 *
 * Because the call lists are extracted FROM the binary, `verify.py` passes on
 * this file by construction. That proves the extraction is faithful. It proves
 * nothing whatever about understanding.
 */

#include <windows.h>
#include "../win1.h"

''' % (mod, seg))

    n = 0
    for i, s in enumerate(starts):
        end = starts[i + 1] if i + 1 < len(starts) else len(sg.code)
        if end - s < 6:
            continue
        info = analyse(sg, s, min(end, s + 0x600), internals)
        label = names.get(s)
        fn = (label or "sub_%04x" % s).replace(".", "_")
        args = info["retf"] // 2 if info["retf"] else 0

        fh.write("/* [AUTO] %s  %s seg%d:0x%04x  %d bytes%s */\n"
                 % (fn, mod, seg, s, info["size"],
                    "  retf %d = %d args" % (info["retf"], args) if info["retf"] else ""))
        if info["writes"]:
            fh.write(" /* writes:  %s */\n"
                     % ", ".join(gname(a) for a in sorted(info["writes"])))
        if info["reads"]:
            r = ", ".join(gname(a) for a in sorted(info["reads"]))
            fh.write(" /* reads:   %s */\n" % (r[:400]))
        if info["fields"]:
            top = sorted(info["fields"].items(), key=lambda kv: -kv[1])[:8]
            fh.write(" /* fields:  %s */\n"
                     % ", ".join("%s x%d" % (fname(k[0], k[1]), c) for k, c in top))
        if info["consts"]:
            cs = info["consts"][:10]
            fh.write(" /* consts:  %s */\n"
                     % ", ".join("0x%x%s" % (c, "=%s" % WM[c] if c in WM else "")
                                 for c in cs))

        sig = ", ".join("WORD a%d" % (k + 1) for k in range(args)) or "void"
        fh.write("LONG %s(%s)\n{\n" % (fn, sig))
        if info["calls"]:
            for off, t in info["calls"][:40]:
                c = cident(t)       # USER.477 -> USER_477: a legal identifier
                fh.write("    %s();%s/* 0x%04x */\n"
                         % (c, " " * max(1, 28 - len(c)), off))
            if len(info["calls"]) > 40:
                fh.write("    /* ... %d further calls */\n" % (len(info["calls"]) - 40))
        else:
            fh.write("    /* makes no calls */\n")
        fh.write("    return 0L;   /* [AUTO] control flow not reconstructed */\n}\n\n")
        n += 1
    return n


def do_segment(mod, seg, ords_, ne, stats=False):
    try:
        sg = trace.Seg(ne, seg, ords_)
    except Exception:
        return 0
    if not getattr(sg, "code", None):
        return 0
    starts = sorted(sg.funcs)
    if not starts:
        return 0
    names = {}
    for e in ne.entries():
        if e.get("seg") == seg and "offset" in e:
            nm = ords_.get("%s.%d" % (mod, e["ordinal"]))
            names[e["offset"]] = nm or "%s_%d" % (mod, e["ordinal"])
    if stats:
        print("  %s seg%-2d  %4d functions  %6d bytes" % (mod, seg, len(starts), len(sg.code)))
        return len(starts)
    if not os.path.isdir(OUT):
        os.makedirs(OUT)
    path = os.path.join(OUT, "%s_seg%d.c" % (mod.lower(), seg))
    with open(path, "w", encoding="utf-8") as fh:
        # exported entry points in THIS segment resolve by address too, so a
        # near call to CopyRect prints CopyRect and not sub_648a
        internals = dict(INTERNALS.get(mod, {}))
        for off_, nm_ in names.items():
            internals.setdefault(off_, nm_)
        n = emit(mod, seg, sg, starts, names, fh, internals)
    print("  %-22s %4d skeletons" % (path, n))
    return n


def main():
    a = [x for x in sys.argv[1:] if not x.startswith("--")]
    stats = "--stats" in sys.argv
    ords_ = trace.load_ordinals("ORDINALS.md")
    mods = ([a[0].upper()] if a else
            ["KERNEL", "GDI", "USER", "SYSTEM", "KEYBOARD", "MOUSE",
             "DISPLAY", "SOUND", "COMM"])
    total = 0
    for mod in mods:
        try:
            ne = idord.open_module(mod)
        except Exception as ex:
            print("  %s: %s" % (mod, ex))
            continue
        segs = [int(a[1])] if len(a) > 1 else range(1, 40)
        for seg in segs:
            total += do_segment(mod, seg, ords_, ne, stats)
    print("\n%d functions %s" % (total, "surveyed" if stats else "generated"))


if __name__ == "__main__":
    main()
