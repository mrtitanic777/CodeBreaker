#!/usr/bin/env python3
"""
idord.py — Behavioral ordinal identifier for the Windows 1.0 system modules.

The boot image binds by ordinal and ships no export names, so names must be
recovered from evidence. This tool gathers that evidence per ordinal, from two
sides:

  MODULE side (what the ordinal's own code does), from modules/<MOD>.ne:
    - INT 21h DOS function numbers it issues (names KERNEL file/mem wrappers)
    - which OTHER modules/ordinals it far-calls (a USER ordinal that calls
      DISPLAY is a screen op; one that calls KEYBOARD is input; one that leans
      on GDI is graphics; one that calls KERNEL does memory/file work)
    - body size and internal-call count (leaf vs orchestrator)

  APP side (how the applications call it), from files/*.EXE:
    - how many apps use it, and under which window messages (consensus)
    - typical pushed-argument count at the call sites
    - whether the return value is kept (returns a handle/value) or discarded

None of this is read from a symbol table; it is all derived from the binaries.
High-signal combinations (DOS AH, Create/Delete & Get/Release pairing, message
consensus) yield confident names; the rest are printed as evidence to weigh.

Usage:
    python3 idord.py MODULE [--ord N] [--used] [--apps]
      MODULE  = KERNEL | GDI | USER | SYSTEM | KEYBOARD | MOUSE | DISPLAY | COMM
      --ord N = focus on one ordinal
      --used  = only ordinals that at least one app actually calls
      --apps  = include the app-side call-site evidence (slower)
"""
import subprocess, sys, os, re, tempfile, glob
from src.core import cores
from src.formats.ne import NEFile
from src.disasm.disasm import open_ne, build_fixup_map
target = cores.get('target')


def open_module(modname):
    """Open a Windows 3.1 system module by name or path.

    Retargeted from Windows 1.0, where this function carried the project's
    hardest-won piece of knowledge: 1.01's system modules were eleven NE
    modules concatenated into WIN100.BIN, with their non-PRELOAD segments
    living in a *second* file (WIN100.OVL) at overlapping offsets. Opening
    one meant carving it out of a synthetic two-file flat image and rewriting
    every overlay segment's file offset.

    None of that applies here. Windows 3.1 ships each module as an ordinary
    MZ/NE file, so this delegates to target.open_module and the whole
    bundle/overlay apparatus — including segfix.py — is simply not ported.
    That simplification is the single biggest structural change between the
    two releases from a tooling standpoint.

    Note that 3.1 module names are NOT unique (twelve files call themselves
    DISPLAY, six call themselves MOUSE), so prefer passing a filename.
    """
    return target.open_module(modname)

DOS = {
 0x00:"Terminate",0x0E:"SetDefaultDrive",0x19:"GetDefaultDrive",0x1A:"SetDTA",
 0x25:"SetIntVec",0x29:"ParseFilename",0x2A:"GetDate",0x2B:"SetDate",0x2C:"GetTime",
 0x2F:"GetDTA",0x30:"GetDOSVersion",0x33:"CtrlBreak",0x35:"GetIntVec",
 0x36:"GetDiskFreeSpace",0x39:"MkDir",0x3A:"RmDir",0x3B:"ChDir",0x3C:"CreateFile",
 0x3D:"OpenFile",0x3E:"CloseFile",0x3F:"ReadFile",0x40:"WriteFile",0x41:"DeleteFile",
 0x42:"LSeek",0x43:"GetSetFileAttr",0x44:"IOCTL",0x45:"DupHandle",0x47:"GetCurDir",
 0x48:"AllocMem",0x49:"FreeMem",0x4A:"SetBlock",0x4B:"EXEC",0x4C:"ExitProcess",
 0x4E:"FindFirst",0x4F:"FindNext",0x56:"RenameFile",0x57:"FileDateTime",0x5B:"CreateNew",
}

def objdump(blob, vma):
    fd, tmp = tempfile.mkstemp(suffix="_id.bin")
    try:
        with os.fdopen(fd, "wb") as f:
            f.write(blob)
        return subprocess.run(
            ["objdump", "-D", "-b", "binary", "-mi386", "-Maddr16,data16,intel",
             "--adjust-vma=%d" % vma, tmp], capture_output=True, text=True).stdout
    finally:
        try: os.remove(tmp)
        except OSError: pass


def analyze_module(modname):
    """Return {ordinal: evidence} for every exported entry of a module."""
    ne = open_module(modname)
    mods = ne.module_refs()
    segs = ne.segments()
    ents = [e for e in ne.entries() if e.get("kind") in ("FIXED", "MOVEABLE") and "offset" in e]
    # bound each function by the next entry in the same segment
    by_seg = {}
    for e in ents:
        by_seg.setdefault(e["seg"], []).append(e["offset"])
    for s in by_seg:
        by_seg[s] = sorted(set(by_seg[s]))

    out = {}
    fixcache = {}
    for e in ents:
        seg = segs[e["seg"]-1]
        if seg["kind"] != "CODE":
            continue
        if e["seg"] not in fixcache:
            try:
                fixcache[e["seg"]] = build_fixup_map(ne, seg, mods)
            except Exception:
                fixcache[e["seg"]] = {}          # truncated reloc table -> no fixups
        fixups = fixcache[e["seg"]]
        offs = by_seg[e["seg"]]
        start = e["offset"]
        nxt = next((o for o in offs if o > start), seg["length"])
        code = ne.data[seg["file_off"]:seg["file_off"]+seg["length"]]
        blob = code[start:nxt]
        asm = objdump(blob, start)

        dos, calls = [], {}
        # DOS AH: an `int 0x21` preceded by the nearest mov ah,/mov ax, load
        lines = [l for l in asm.splitlines() if ":\t" in l]
        for i, l in enumerate(lines):
            if "int    0x21" in l:
                for back in range(i, max(-1, i-8), -1):
                    ah = re.search(r"mov    a[hx],0x([0-9a-f]{1,4})$", lines[back].split("\t")[-1])
                    if ah:
                        v = int(ah.group(1), 16)
                        dos.append(v >> 8 if v > 0xff else v)
                        break
        # cross-module far calls, via the fixups landing in this span. Restrict
        # to this module's real imports so misparsed relocs don't leak noise.
        for site, lbl in fixups.items():
            if start <= site < nxt and not lbl.startswith("seg"):
                m = lbl.split(".")[0]
                if m in mods:
                    calls[m] = calls.get(m, 0) + 1
        out[e["ordinal"]] = dict(size=nxt-start, seg=e["seg"], off=start,
                                 dos=sorted(set(dos)), calls=calls)
    return ne.resident_names()[0][0], out


def app_usage(ord_prefix, apps_glob="files/*.EXE"):
    """{ 'USER.66': {'apps': n, 'ret_kept': k, 'ret_void': v} } from call sites."""
    usage = {}
    for path in glob.glob(apps_glob):
        try:
            ne = open_ne(path)
        except Exception:
            continue
        mods = ne.module_refs()
        seen_here = set()
        for seg in ne.segments():
            if seg["kind"] != "CODE":
                continue
            fixups = build_fixup_map(ne, seg, mods)
            code = ne.data[seg["file_off"]:seg["file_off"]+seg["length"]]
            asm = objdump(code, 0)
            rows = [l.split("\t")[-1].strip() for l in asm.splitlines() if ":\t" in l]
            offs = [int(l.split(":\t")[0].strip(), 16) for l in asm.splitlines() if ":\t" in l]
            pos = {o: i for i, o in enumerate(offs)}
            for site, lbl in fixups.items():
                if not lbl.startswith(ord_prefix):
                    continue
                seen_here.add(lbl)
                u = usage.setdefault(lbl, dict(apps=set(), ret_kept=0, ret_void=0))
                # return kept? next instruction after the call stores ax
                near = min((o for o in offs if o >= site), default=None)
                if near is not None and pos[near]+1 < len(rows):
                    nxtasm = rows[pos[near]+1]
                    if re.match(r"(mov    (WORD PTR |)[^,]+,ax|push   ax|or     ax,ax|test   ax,ax)", nxtasm):
                        u["ret_kept"] += 1
                    else:
                        u["ret_void"] += 1
        for lbl in seen_here:
            usage[lbl]["apps"].add(os.path.basename(path))
    for lbl in usage:
        usage[lbl]["apps"] = len(usage[lbl]["apps"])
    return usage


def main():
    a = sys.argv[1:]
    if not a:
        print(__doc__); sys.exit(1)
    mod = a[0].upper()
    name, ev = analyze_module(mod)
    focus = int(a[a.index("--ord")+1]) if "--ord" in a else None
    usage = app_usage(mod + ".") if "--apps" in a else {}

    print(f"# {name}: {len(ev)} exported ordinals analyzed from {BUNDLE}")
    if usage:
        print(f"# app-side usage gathered for {len(usage)} called ordinals\n")

    for ordn in sorted(ev):
        if focus and ordn != focus:
            continue
        e = ev[ordn]
        lbl = f"{mod}.{ordn}"
        u = usage.get(lbl)
        if "--used" in a and not u:
            continue
        tags = []
        if e["dos"]:
            tags.append("DOS[" + ",".join(DOS.get(d, f"AH{d:02x}") for d in e["dos"]) + "]")
        if e["calls"]:
            tags.append("calls " + ",".join(f"{m}x{c}" for m, c in sorted(e["calls"].items(), key=lambda x:-x[1])))
        if not e["calls"] and not e["dos"]:
            tags.append("leaf (no syscalls/subcalls)")
        us = ""
        if u:
            us = f"  | apps={u['apps']} ret_kept={u['ret_kept']} ret_void={u['ret_void']}"
        print(f"  {lbl:<10} sz={e['size']:<4} {'; '.join(tags)}{us}")


if __name__ == "__main__":
    main()
