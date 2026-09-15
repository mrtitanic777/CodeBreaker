#!/usr/bin/env python3
r"""dosrecon.py -- reconstruction organizer for the Win95 real-mode base (IO.SYS + COMMAND.COM).

The real Win95 IO.SYS is WINBOOT.SYS: the MS-DOS 7.0 real-mode kernel (MSLOAD + MSBIO + MSDOS)
with a Win95-specific real-mode layer bolted on -- the registry preloader, the INT 2Fh AX=1613h
"WinBootDir" provider, IFSHLP/DoubleSpace auto-mount, the LOGO, and the MSDOS.SYS [Paths]/[Options]
handling. The DOS 6.0-derived `msdos/` tree has the shared kernel but NOT that Win95 layer -- which
is exactly the registry gate the VMM32 boot hits.

win95mzdec produces a raw whole-file decode (`--tree`), but a raw dump mixes code with data (the
device-driver header table, the BPB, message/string tables) and gives every routine a `sub_<off>`
label. This pass turns that census into an organized, navigable reconstruction:

  * CODE vs DATA classification per discovered function (data = the device table, BPB, string/message
    regions, or runs the 16-bit decoder can't validate);
  * per-function characterization: the INT services it issues and the strings it references;
  * NAMING of the Win95 real-mode layer from those signals (registry_load references SYSTEM.DAT, the
    2Fh dispatcher that answers AX=1613h, the CONFIG.SYS parser, the logo, ...);
  * an organized source tree + a triage REPORT.md with real counts and the Win95-layer work-list.

    wt --target win95 dosrecon [--iosys|--command|--both] [--out DIR]

Read-only against the corpus; writes only under design/recon/.
"""
import os
import re
import sys
import csv
import subprocess

from src.core import paths

try:
    import capstone
except ImportError:
    capstone = None

# --- corpus binaries (the REAL Win95 real-mode files) ---------------------------------------
_BINS = {
    "iosys":   ("winboot.sys", "IO.SYS (WINBOOT.SYS) -- MS-DOS 7.0 real-mode kernel"),
    "command": ("command.com", "COMMAND.COM -- the Win95 command interpreter"),
}


def _design_root():
    """The Win95 design arena that actually holds the corpus. Prefer the resolver, but fall back to
    the sibling Windows95 repo (<...>/GitHub/Windows95/design) when the resolver can't find it."""
    dr = paths.design_root()
    if os.path.isdir(os.path.join(dr, "corpus")):
        return dr
    sib = os.path.join(os.path.dirname(paths.repo_root()), "Windows95", "design")
    if os.path.isdir(os.path.join(sib, "corpus")):
        return sib
    return dr


def _corpus(name):
    root = _design_root()
    for cand in (
        os.path.join(root, "corpus", "files", "setup", "inf", name),
        os.path.join(root, "corpus", "files", "WINDOWS", "COMMAND", name),
    ):
        if os.path.isfile(cand):
            return cand
    # last resort: search
    for dp, _dn, fn in os.walk(os.path.join(root, "corpus")):
        for f in fn:
            if f.lower() == name.lower():
                return os.path.join(dp, f)
    return None


def _mzdec():
    b = os.path.join(paths.repo_root(), "native", "decompiler", "build", "bin", "win95mzdec.exe")
    return b if os.path.isfile(b) else None


# --- IO.SYS layout (mirror of the native loader) --------------------------------------------
def _layout(data):
    """Return (is_iosys, body_end, load_base, image) matching win95mzdec's loader."""
    import struct
    last_page, pages, _rel, hdr_par = struct.unpack("<HHHH", data[2:10])
    hdr = hdr_par * 16
    total = pages * 512
    if last_page:
        total = (pages - 1) * 512 + last_page
    total = min(total, len(data))
    is_iosys = hdr > total // 2
    if is_iosys:
        return True, hdr, 0x0070, data[:total]
    # normal MZ: image is past the header
    return False, 0, 0, data[hdr:total]


# --- string table ---------------------------------------------------------------------------
def _string_regions(image):
    """Map linear offset -> string for printable runs >= 4, used for code->string xref."""
    strings = {}
    for m in re.finditer(rb"[\x20-\x7e]{4,}", image):
        strings[m.start()] = m.group().decode("latin1")
    return strings


def _nearest_string(strings, off, window=0):
    s = strings.get(off)
    return s


# --- per-function analysis ------------------------------------------------------------------
def _analyze_fn(image, fstart, fend, strings, md):
    """Disassemble one function; return (kind, ninsn, nbad, ints:set, strs:list)."""
    ints, strs = set(), []
    ninsn = nbad = 0
    a = fstart
    seglen = min(fend, len(image))
    while a < seglen:
        chunk = bytes(image[a:min(a + 15, seglen)])
        got = list(md.disasm(chunk, a))
        if not got:
            nbad += 1
            a += 1
            continue
        ins = got[0]
        ninsn += 1
        mn = ins.mnemonic
        if mn == "int":
            op = ins.op_str.strip()
            ints.add(op)
        # string xref: any immediate/displacement that lands on a string start
        for tok in re.findall(r"0x[0-9a-fA-F]+", ins.op_str):
            v = int(tok, 16)
            if v in strings and 4 <= len(strings[v]) <= 64:
                strs.append(strings[v])
        a += ins.size
    # classify code vs data: data regions decode as mostly-bad or trivial
    total = ninsn + nbad
    if total == 0:
        kind = "empty"
    elif nbad > ninsn:
        kind = "data"
    elif ninsn <= 2 and not ints and not strs:
        kind = "stub"
    else:
        kind = "code"
    return kind, ninsn, nbad, ints, strs


# --- naming heuristics for the Win95 real-mode layer ----------------------------------------
_NAME_RULES = [
    (re.compile(r"SYSTEM\.DA", re.I),      "registry"),
    (re.compile(r"\.DAT\b", re.I),          "registry"),
    (re.compile(r"CONFIG\.SYS", re.I),      "config_parse"),
    (re.compile(r"AUTOEXEC", re.I),         "autoexec"),
    (re.compile(r"COMMAND\.COM", re.I),     "load_command"),
    (re.compile(r"WINBOOT|\\WINDOWS", re.I),"winbootdir"),
    (re.compile(r"IFSHLP", re.I),           "ifshlp"),
    (re.compile(r"HIMEM|XMS", re.I),        "xms_himem"),
    (re.compile(r"DBLSPACE|DRVSPACE", re.I),"cvf_mount"),
    (re.compile(r"Windows 95|MS-DOS", re.I),"banner"),
    (re.compile(r"LOGO", re.I),             "logo"),
    (re.compile(r"MSDOS\.SYS", re.I),       "msdossys_cfg"),
]


def _name_for(fstart, kind, ints, strs, region):
    if kind != "code":
        return None
    tags = []
    for s in strs:
        for rx, tag in _NAME_RULES:
            if rx.search(s) and tag not in tags:
                tags.append(tag)
    if tags:
        return "%s_%06X" % (tags[0], fstart)
    # int-service based
    if "0x2f" in ints:
        return "int2f_%06X" % fstart
    if "0x21" in ints:
        return "dos21_%06X" % fstart
    if "0x13" in ints:
        return "disk13_%06X" % fstart
    if "0x10" in ints:
        return "video10_%06X" % fstart
    return None


def _emit_asm(which, outroot, image, is_iosys, body_end, starts, ends, strings, md):
    r"""Emit annotated MASM-style reconstruction per region, with DS-tracked string xref
    resolution and INT-service comments. This is the faithful authored form for 16-bit real-mode
    code (MS-DOS/COMMAND.COM were written in assembly). The body loads at real-mode segment 0x0070,
    so a runtime DS value D addresses file offset (D*16 + disp) - 0x0700."""
    LOAD_LIN = 0x0070 * 16 if is_iosys else 0

    def region_of(a):
        if not is_iosys:
            return "command"
        if a < 0x0800:
            return "msload"
        if a < body_end:
            return "msbio"
        return "sysinit"

    def resolve_str(ds_base, disp):
        if ds_base is None:
            return None
        lin = ds_base * 16 + disp
        off = lin - LOAD_LIN
        if off in strings and 4 <= len(strings[off]) <= 96:
            return strings[off]
        return None

    outdir = os.path.join(outroot, which)
    os.makedirs(outdir, exist_ok=True)
    rf = {}

    def region_file(r):
        if r in rf:
            return rf[r]
        f = open(os.path.join(outdir, r + ".asm"), "w")
        f.write("; %s.asm -- reconstructed from the REAL Win95 %s (%s region).\n"
                "; 16-bit real-mode. Annotated by CodeBreaker dosrecon: DS-tracked string xrefs,\n"
                "; INT-service comments, sub_<fileoffset> labels (authored names where triaged).\n\n"
                "        .8086\n_TEXT   SEGMENT byte public 'CODE'\n        ASSUME cs:_TEXT\n\n"
                % (r, which.upper(), r))
        rf[r] = f
        return f

    start_set = set(starts)
    _JCC = {"jmp", "je", "jne", "jz", "jnz", "jb", "jnb", "jbe", "ja", "jae", "jl", "jle",
            "jg", "jge", "js", "jns", "jc", "jnc", "jo", "jno", "jp", "jnp", "jcxz",
            "loop", "loope", "loopne", "call"}

    def fn_insns(s, e):
        """Decode a function once; return list of (addr, ins)."""
        out = []
        a = s
        seglen = min(e, len(image))
        n = 0
        while a < seglen and n < 6000:
            got = list(md.disasm(bytes(image[a:min(a + 15, seglen)]), a))
            if not got:
                out.append((a, None)); a += 1; n += 1; continue
            out.append((a, got[0])); a += got[0].size; n += 1
        return out

    for s in starts:
        e = ends[s]
        r = region_of(s)
        f = region_file(r)
        insns = fn_insns(s, e)
        seglen = min(e, len(image))
        # pass 1: collect intra-function jump targets -> local labels
        labels = {}
        for a, ins in insns:
            if ins and ins.mnemonic in _JCC and ins.op_str.startswith("0x"):
                try: t = int(ins.op_str, 16)
                except ValueError: continue
                if ins.mnemonic != "call" and s <= t < seglen:
                    labels.setdefault(t, "L%06X" % t)
        f.write("\n%s_%06X proc near\n" % ("sub", s))
        ds_base = None
        pend_ax = None
        for a, ins in insns:
            if a in labels:
                f.write("%s:\n" % labels[a])
            if ins is None:
                f.write("        db 0%02Xh\n" % image[a]); continue
            mn, ops = ins.mnemonic, ins.op_str
            cmt = ""
            # branch/call operand -> label or sub_ name
            if mn in _JCC and ops.startswith("0x"):
                try:
                    t = int(ops, 16)
                    if mn == "call":
                        ops = ("sub_%06X" % t) if t in start_set else ("near ptr 0%06Xh" % t)
                    elif t in labels:
                        ops = labels[t]
                    else:
                        ops = ("sub_%06X" % t) if t in start_set else ("0%06Xh" % t)
                except ValueError:
                    pass
            # DS tracking for string xref
            if mn == "mov" and ops.startswith(("ax,", "bx,")) and "0x" in ops:
                try: pend_ax = int(ops.split(",")[1], 16)
                except ValueError: pend_ax = None
            elif mn == "mov" and ops.startswith("ds,"):
                ds_base = pend_ax
            if mn == "int":
                cmt = _int_comment(ins.op_str)
            for tok in re.findall(r"0x[0-9a-fA-F]+", ins.op_str):
                v = int(tok, 16)
                st = strings.get(v - LOAD_LIN) or resolve_str(ds_base, v)
                if st and 4 <= len(st) <= 96:
                    cmt = '"%s"' % st.replace('"', "'"); break
            txt = ("%s %s" % (mn, ops)).rstrip()
            if cmt:
                f.write("        %-34s ; %s\n" % (txt, cmt))
            else:
                f.write("        %s\n" % txt)
        f.write("sub_%06X endp\n" % s)
    for f in rf.values():
        f.write("\n_TEXT   ENDS\n        END\n"); f.close()
    return list(rf.keys())


def _int_comment(op):
    m = {"0x21": "DOS", "0x2f": "multiplex", "0x13": "BIOS disk", "0x10": "BIOS video",
         "0x16": "BIOS kbd", "0x15": "BIOS misc", "0x25": "set int vec", "0x26": "abs disk write",
         "0x19": "reboot", "0x11": "equip", "0x12": "memsize", "0x2a": "network"}
    return "INT %s (%s)" % (op, m.get(op, "?"))


def _run_one(which, outroot):
    fname, desc = _BINS[which]
    path = _corpus(fname)
    if not path:
        print("  MISSING corpus file: %s" % fname)
        return None
    data = open(path, "rb").read()
    is_iosys, body_end, load_base, image = _layout(data)
    # for iosys the image is the whole file; for MZ it's the load module
    strings = _string_regions(image if not is_iosys else data)
    if is_iosys:
        image = data  # whole-file flat, matches loader linear==fileoffset
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)

    # function starts from win95mzdec --names
    mz = _mzdec()
    starts = []
    if mz:
        out = subprocess.run([mz, path, "--names"], capture_output=True, text=True).stdout
        for line in out.splitlines():
            p = line.split("\t")
            if p and p[0]:
                try:
                    starts.append(int(p[0], 16))
                except ValueError:
                    pass
    starts = sorted(set(starts))
    ends = {s: (starts[i + 1] if i + 1 < len(starts) else len(image)) for i, s in enumerate(starts)}

    def region_of(a):
        if not is_iosys:
            return "command"
        if a < 0x0800:
            return "msload"
        if a < body_end:
            return "msbio"
        return "sysinit"

    rows = []
    counts = {"code": 0, "data": 0, "stub": 0, "empty": 0}
    win95_layer = []
    for s in starts:
        e = ends[s]
        if e - s > 0x4000:      # cap runaway (data gaps); analyze the head
            e = s + 0x4000
        kind, ni, nb, ints, strs = _analyze_fn(image, s, e, strings, md)
        counts[kind] = counts.get(kind, 0) + 1
        nm = _name_for(s, kind, ints, strs, region_of(s))
        rows.append({
            "addr": "0x%06X" % s, "end": "0x%06X" % ends[s], "size": ends[s] - s,
            "region": region_of(s), "kind": kind, "ninsn": ni,
            "ints": " ".join(sorted(ints)), "name": nm or "",
            "strings": " | ".join(dict.fromkeys(strs))[:200],
        })
        if nm and any(t in nm for t in ("registry", "config", "winbootdir", "ifshlp",
                                        "load_command", "cvf_mount", "msdossys", "logo",
                                        "banner", "autoexec", "int2f")):
            win95_layer.append((s, nm, sorted(ints), list(dict.fromkeys(strs))[:6]))

    outdir = os.path.join(outroot, which)
    os.makedirs(outdir, exist_ok=True)
    with open(os.path.join(outdir, "triage.csv"), "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=["addr", "end", "size", "region", "kind",
                                          "ninsn", "ints", "name", "strings"])
        w.writeheader()
        w.writerows(rows)
    asm_regions = _emit_asm(which, outroot, image, is_iosys, body_end, starts, ends, strings, md)
    print("  emitted annotated asm: %s" % ", ".join("%s.asm" % r for r in asm_regions))
    return {
        "which": which, "path": path, "desc": desc, "is_iosys": is_iosys,
        "body_end": body_end, "nfuncs": len(starts), "counts": counts,
        "win95_layer": win95_layer, "outdir": outdir,
    }


def main(argv):
    if capstone is None:
        print("dosrecon: needs capstone (pip install capstone)")
        return 2
    which = "both"
    outroot = os.path.join(_design_root(), "recon")
    i = 0
    while i < len(argv):
        a = argv[i]
        if a == "--iosys":
            which = "iosys"
        elif a == "--command":
            which = "command"
        elif a == "--both":
            which = "both"
        elif a == "--out" and i + 1 < len(argv):
            outroot = argv[i + 1]; i += 1
        i += 1

    targets = ["iosys", "command"] if which == "both" else [which]
    results = []
    for t in targets:
        print("== dosrecon %s ==" % t)
        r = _run_one(t, outroot)
        if r:
            results.append(r)
            c = r["counts"]
            print("  %d functions: code=%d data=%d stub=%d empty=%d"
                  % (r["nfuncs"], c.get("code", 0), c.get("data", 0),
                     c.get("stub", 0), c.get("empty", 0)))
            print("  Win95 real-mode layer candidates: %d" % len(r["win95_layer"]))

    # combined REPORT.md
    rep = os.path.join(outroot, "REPORT.md")
    with open(rep, "w") as f:
        f.write("# Win95 real-mode base reconstruction -- triage report\n\n")
        f.write("Reverse-engineered from the REAL corpus binaries (not the 6.0-derived tree).\n\n")
        for r in results:
            c = r["counts"]
            f.write("## %s\n\n%s\n\n" % (r["which"].upper(), r["desc"]))
            f.write("- source: `%s`\n" % os.path.basename(r["path"]))
            f.write("- layout: %s\n" % ("IO.SYS (body+SYSINIT, body_end=0x%06X)" % r["body_end"]
                                        if r["is_iosys"] else "normal MZ"))
            f.write("- functions: **%d** (code=%d, data=%d, stub=%d, empty=%d)\n"
                    % (r["nfuncs"], c.get("code", 0), c.get("data", 0),
                       c.get("stub", 0), c.get("empty", 0)))
            f.write("- **Win95 real-mode layer candidates: %d**\n\n" % len(r["win95_layer"]))
            if r["win95_layer"]:
                f.write("| addr | name | INT | strings |\n|---|---|---|---|\n")
                for s, nm, ints, strs in r["win95_layer"]:
                    f.write("| 0x%06X | %s | %s | %s |\n"
                            % (s, nm, " ".join(ints), "; ".join(strs).replace("|", "\\|")[:80]))
                f.write("\n")
    print("\nwrote %s" % rep)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
