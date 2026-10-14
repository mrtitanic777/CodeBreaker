"""
disasm.py — Annotated 16-bit disassembly of NE segments.

Extracts a segment's code, disassembles it with objdump (i8086), then walks the
NE per-segment relocation chains to label every fixup site with the real target
(module.ordinal for imports, seg:entry for internal refs). This turns the
placeholder `call 0x0:0xffff` instructions into readable `call FAR KERNEL.91`.

Usage:
    python3 disasm.py FILE [seg_index] [--start 0xNN] [--count N]
    python3 disasm.py FILE --entry ORDINAL      # disassemble one exported entry
"""
import struct, subprocess, sys, os, tempfile
from codebreaker.formats.ne import NEFile

def open_ne(path):
    """Open a normal MZ/NE file, or a raw embedded NE module (starts with 'NE')."""
    data = open(path, "rb").read()
    if data[:2] == b"MZ":
        return NEFile(path)
    if data[:2] == b"NE":
        return NEFile.from_buffer(data, 0, path)
    raise ValueError(f"{path}: not MZ or NE")

def build_fixup_map(ne, seg, mods):
    """offset -> human label, by walking each relocation chain."""
    data = ne.data
    base = seg["file_off"]
    seglen = seg["length"]
    fixups = {}
    def label(r):
        if r["kind"] == "IMPORTORD":
            m = mods[r["module"]-1] if 0 < r["module"] <= len(mods) else f"mod{r['module']}"
            return f"{m}.{r['ordinal']}"
        if r["kind"] == "IMPORTNAME":
            m = mods[r["module"]-1] if 0 < r["module"] <= len(mods) else f"mod{r['module']}"
            return f"{m}.{ne.imported_name(r['name_off'])}"
        if r["kind"] == "INTERNAL":
            # seg 0xFF is not a segment: it means "this module's OWN entry-table
            # ordinal N" — the fixup for a call into a MOVEABLE segment, which
            # must go through the entry table so the segment can be relocated.
            # Resolving it names the target as a self-call (e.g. GDI -> GDI.273)
            # instead of a meaningless "seg255:0x0111".
            if r["seg"] == 0xFF:
                return f"{ne.module_name()}.{r['entry']}"
            return f"seg{r['seg']}:0x{r['entry']:04x}"
        return f"osfixup#{r.get('ftype')}"
    for r in ne.relocations(seg):
        lbl = label(r)
        cur = r["offset"]
        seen = 0
        while cur != 0xFFFF and cur + 1 < seglen and seen < 10000:
            fixups[cur] = lbl
            nxt = struct.unpack_from("<H", data, base + cur)[0]
            if r["additive"]:
                break
            if nxt == cur:
                break
            cur = nxt
            seen += 1
    return fixups

def disasm_segment(path, seg_index=1, start=0, count=None):
    ne = open_ne(path)
    mods = ne.module_refs()
    segs = ne.segments()
    seg = segs[seg_index-1]
    if seg["kind"] != "CODE":
        print(f"(segment {seg_index} is {seg['kind']})")
    code = ne.data[seg["file_off"]:seg["file_off"]+seg["length"]]
    fixups = build_fixup_map(ne, seg, mods)

    end = seg["length"] if count is None else min(seg["length"], start+count)
    blob = code[start:end]
    # Unique temp file per process so concurrent disasm.py runs don't race.
    fd, tmp = tempfile.mkstemp(suffix="_ne_seg.bin")
    try:
        with os.fdopen(fd, "wb") as fh:
            fh.write(blob)
        out = subprocess.run(
            ["objdump","-D","-b","binary","-mi386","-Maddr16,data16,intel",
             "--adjust-vma=%d"%start, tmp],
            capture_output=True, text=True).stdout
    finally:
        try: os.remove(tmp)
        except OSError: pass

    print(f"# {os.path.basename(path)}  segment #{seg_index} ({seg['kind']}, "
          f"0x{seg['length']:x} bytes)  imports: {', '.join(mods)}")
    print(f"# {len(fixups)} fixup sites in this segment\n")
    # First pass: collect instruction offsets so we know each instruction's span.
    parsed = []  # (offset|None, raw_line)
    for line in out.splitlines():
        if ":\t" in line:
            head = line.split(":\t")[0].strip()
            try:
                parsed.append((int(head, 16), line))
                continue
            except ValueError:
                pass
        parsed.append((None, line))
    offsets = [o for o,_ in parsed if o is not None]
    nextoff = {}
    for i, o in enumerate(offsets):
        nextoff[o] = offsets[i+1] if i+1 < len(offsets) else o+8
    for off, line in parsed:
        if off is None:
            print(line); continue
        annot = ""
        for k in range(off, nextoff[off]):     # only THIS instruction's bytes
            if k in fixups:
                annot = f"   ; -> {fixups[k]}"
                break
        print(line + annot)

def disasm_entry(path, ordinal):
    ne = open_ne(path)
    ents = {e["ordinal"]: e for e in ne.entries()}
    e = ents.get(ordinal)
    if not e or e.get("kind") == "UNUSED":
        print(f"ordinal {ordinal} not exported"); return
    print(f"# entry ordinal {ordinal} -> segment {e['seg']} offset 0x{e['offset']:x}")
    disasm_segment(path, e["seg"], start=e["offset"], count=128)

if __name__ == "__main__":
    a = sys.argv[1:]
    path = a[0]
    if "--entry" in a:
        disasm_entry(path, int(a[a.index("--entry")+1], 0)); sys.exit()
    seg = int(a[1]) if len(a) > 1 and not a[1].startswith("--") else 1
    start = int(a[a.index("--start")+1], 0) if "--start" in a else 0
    count = int(a[a.index("--count")+1], 0) if "--count" in a else None
    disasm_segment(path, seg, start, count)
