#!/usr/bin/env python3
"""
identify.py — Behavioral identification of KERNEL entry points.

Many KERNEL ordinals are thin wrappers over MS-DOS INT 21h services. For every
exported ordinal we disassemble its body and, for each `int 0x21` we find, look
back for the nearest AH/AX load to recover the DOS function number — which names
the wrapper. Purely derived from the code.
"""
import subprocess, sys, re
from codebreaker.format_loaders.ne import NEFile
from codebreaker.disasm_engine.disasm import open_ne, build_fixup_map

# MS-DOS INT 21h AH -> service (the ones a windowing kernel actually wraps)
DOS = {
 0x00:"Terminate",0x0E:"SetDefaultDrive",0x19:"GetDefaultDrive",0x1A:"SetDTA",
 0x25:"SetInterruptVector",0x29:"ParseFilename",0x2A:"GetDate",0x2B:"SetDate",
 0x2C:"GetTime",0x2F:"GetDTA",0x30:"GetDOSVersion",0x35:"GetInterruptVector",
 0x36:"GetDiskFreeSpace",0x39:"MkDir",0x3A:"RmDir",0x3B:"ChDir",0x3C:"CreateFile",
 0x3D:"OpenFile",0x3E:"CloseFile",0x3F:"ReadFile",0x40:"WriteFile",0x41:"DeleteFile",
 0x42:"LSeek",0x43:"GetSetFileAttr",0x44:"IOCTL",0x45:"DupHandle",0x47:"GetCurDir",
 0x48:"DOSAllocMem",0x49:"DOSFreeMem",0x4A:"DOSSetBlock",0x4B:"EXEC",0x4C:"ExitProcess",
 0x4E:"FindFirst",0x4F:"FindNext",0x56:"RenameFile",0x57:"GetSetFileDateTime",
}

def objdump(blob, vma):
    open("/tmp/_id.bin","wb").write(blob)
    return subprocess.run(
        ["objdump","-D","-b","binary","-mi386","-Maddr16,data16,intel",
         "--adjust-vma=%d"%vma,"/tmp/_id.bin"],
        capture_output=True, text=True).stdout

def scan(path):
    # IMPORTANT: the carved modules/*.ne files have BUNDLE-RELATIVE segment sector
    # offsets, so open_ne() on them reads the WRONG bytes (this once produced a
    # completely bogus KERNEL map). Read system modules from the full WIN100.BIN
    # bundle with base=0 instead. See idord.py open_module().
    import re as _re, os as _os
    m = _re.match(r".*modules[\\/](\w+)\.ne$", path, _re.I)
    if m and _os.path.exists("files/WF/WIN100.BIN"):
        from idord import open_module
        ne = open_module(m.group(1).upper())
    else:
        ne = open_ne(path)
    modname = ne.resident_names()[0][0]
    segs = ne.segments()
    ents = [e for e in ne.entries() if e.get("kind") in ("FIXED","MOVEABLE")]
    # Per-segment sorted entry offsets, so each function is bounded by the next.
    seg_offsets = {}
    for e in ents:
        seg_offsets.setdefault(e["seg"], set()).add(e["offset"])
    seg_offsets = {s: sorted(v) for s, v in seg_offsets.items()}
    print(f"# {modname}: scanning {len(ents)} entry points for INT 21h wrappers\n")
    found = {}
    for e in ents:
        seg = segs[e["seg"]-1]
        if seg["kind"] != "CODE":
            continue
        code = ne.data[seg["file_off"]:seg["file_off"]+seg["length"]]
        start = e["offset"]
        offs = seg_offsets[e["seg"]]
        nxt = next((o for o in offs if o > start), seg["length"])
        span = min(nxt - start, 512)          # bound by the next entry point
        blob = code[start:start+span]
        text = objdump(blob, start)
        lines = [l for l in text.splitlines() if ":\t" in l]
        # track last AH/AX immediate before each int 0x21
        last_ah = None
        for l in lines:
            m = re.search(r"mov    ah,0x([0-9a-f]+)", l)
            if m: last_ah = int(m.group(1),16)
            m2 = re.search(r"mov    ax,0x([0-9a-f]{1,4})", l)
            if m2:
                v = int(m2.group(1),16)
                last_ah = (v >> 8) if v > 0xff else last_ah
            if "int    0x21" in l and last_ah is not None:
                found.setdefault(e["ordinal"], set()).add(last_ah)
    # report
    named = {}
    for ordv in sorted(found):
        ahs = sorted(found[ordv])
        names = [DOS.get(a, f"AH={a:02X}h") for a in ahs]
        # heuristic: if two DOS calls, the wrapper is usually named for the primary
        label = "/".join(names)
        named[ordv] = label
        print(f"  {modname}.{ordv:<3}  DOS {['%02Xh'%a for a in ahs]}  -> {label}")
    print(f"\n{len(named)} of {len(ents)} entry points are DOS INT 21h wrappers")
    return named

if __name__ == "__main__":
    scan(sys.argv[1] if len(sys.argv) > 1 else "modules/KERNEL.ne")
