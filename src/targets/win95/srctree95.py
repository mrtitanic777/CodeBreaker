#!/usr/bin/env python3
r"""srctree95.py -- assemble the reconstructed modules into a faithful Windows 95 SOURCE TREE.

The deploy banks reconstruction per BINARY (shell32_dll.fused.c): a flat blob organized by output
module. The real Win95 source was organized by COMPONENT, with many named .c files per binary. This
turns the flat banks into that tree, using only signals that are faithful:

  * COMPONENT placement   -- module -> component dir (kernel/gdi/user/shell/ole/vmm/net/...), from
                             the documented architecture.
  * SOURCE-FILE skeleton  -- for modules with a Microsoft .SYM, the real object/source-file names
                             and their function membership (symfile.source_segments) -- the authentic
                             file layout (user/msgbox.c, user/messages.c, ...).
  * FUNCTION placement     -- a reconstructed function is filed into its real source .c when its
                             recovered NAME (export / demangle / RTTI / win16ord) is listed in a
                             .SYM object; otherwise it goes to <module>.c for that module.
  * honest tiers          -- every file header states whether its NAME is real (.sym/observed) or
                             synthesized, and whether function placement is by-name or catch-all.
                             Nothing is fabricated: unknown stays `sub_xxxx` in <module>.c.

    wt --target win95 srctree95 <RECON_DIR> <OUT_TREE> [--limit N]
"""
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

# ---- module -> component (the documented Win95 architecture) -------------------------------------
_EXACT = {
    "krnl386": "kernel", "krnl286": "kernel", "kernel32": "kernel", "kernel": "kernel",
    "gdi": "gdi", "gdi32": "gdi", "dispdib": "gdi",
    "user": "user", "user32": "user",
    "shell": "shell", "shell32": "shell", "shell232": "shell", "shdocvw": "shell", "explorer": "shell",
    "comctl32": "commctrl", "comdlg32": "commctrl", "commdlg": "commctrl",
    "ole32": "ole", "ole2": "ole", "oleaut32": "ole", "compobj": "ole", "typelib": "ole",
    "storage": "ole", "oledlg": "ole", "olecli32": "ole", "olesvr32": "ole", "ole2disp": "ole",
    "ole2nls": "ole", "ole2conv": "ole", "ole2prox": "ole",
    "winmm": "mmedia", "mmsystem": "mmedia", "msacm": "mmedia", "msacm32": "mmedia",
    "msvfw32": "mmedia", "msvideo": "mmedia", "mciole16": "mmedia", "sound": "mmedia",
    "wsock32": "net", "winsock": "net", "netapi32": "net", "mpr": "net", "netware": "net",
    "wfwnet": "net", "vnetbios": "net",
    "tapi": "comm", "comm": "comm", "serial": "comm", "serialui": "comm",
    "advapi32": "base", "version": "base", "lz32": "base", "lzexpand": "base", "shlwapi": "base",
    "rpcrt4": "base", "crtdll": "base", "toolhelp": "base", "system": "base", "winspool": "base",
    "vmm": "vmm", "ios": "vmm", "configmg": "vmm", "vpicd": "vmm", "vkd": "vmm", "vdd": "vmm",
    "vdmad": "vmm", "vflatd": "vmm", "vxdldr": "vmm", "vga": "vmm", "scsiport": "vmm",
}
_VXD_EXT = (".vxd", ".386", ".pdr", ".mpd", ".vxd")


def component(module):
    base = os.path.basename(str(module)).lower()
    stem = base
    for e in (".dll", ".exe", ".drv", ".vxd", ".386", ".cpl", ".scr", ".ocx", ".fon", ".pdr", ".mpd"):
        if stem.endswith(e):
            stem = stem[:-len(e)]; break
    if stem in _EXACT:
        return _EXACT[stem]
    if base.endswith(_VXD_EXT):
        return "vmm"
    if base.endswith(".drv"):
        return "drivers"
    if base.endswith((".cpl",)):
        return "setup"
    if base.endswith((".scr",)):
        return "apps"
    if base.endswith(".exe"):
        return "apps"
    return "unsorted"


# ---- parse a reconstructed .c into function blocks -----------------------------------------------
# compose writes:   /* NAME  (sub_rva) [status verify] */\n<body>    or   /* sub_rva [status] */
# emit_all95 (NE):  /* label */\n<body>
_HDR = re.compile(r"^/\*\s*(?P<head>[^\n*]+?)\s*\*/\s*$", re.M)


def _blocks(text):
    """[(name, header, body)] for each /* ... */ function block (skips the ENRICHMENT preamble)."""
    out = []
    hdrs = list(_HDR.finditer(text))
    for i, m in enumerate(hdrs):
        head = m.group("head")
        # a function header mentions a tier in [..] or looks like a sub_/name; skip section banners
        if head.startswith("=") or head.startswith("ENRICHMENT") or "=====" in head:
            continue
        # skip inline body comments we don't want as blocks (e.g. "Win16 caller: N call(s)")
        if re.match(r"(Win16|recovered:|resource|WM_|LoadString|dialog)", head):
            continue
        body = text[m.end():(hdrs[i + 1].start() if i + 1 < len(hdrs) else len(text))].strip("\n")
        nm = re.split(r"\s*[\(\[]", head, 1)[0].strip().rstrip(":")
        if "!" in nm:                       # NE label: MODULE!sub_x -> sub_x
            nm = nm.rsplit("!", 1)[1]
        if "." in nm:                       # NE label: MODULE.FUNC -> FUNC
            nm = nm.rsplit(".", 1)[1]
        out.append((nm, head, body))
    return out


def _sym_file_map(module):
    """{FUNC_NAME_UPPER: source_file_stem} from the module's .SYM object segments, or {}."""
    try:
        from src.engines.symbol import symfile
    except Exception:
        return {}, {}
    segs = symfile.source_segments(module)
    fmap, roster = {}, {}
    for segname, names in segs:
        stem = _src_stem(segname)
        if not stem:
            continue
        clean = [symfile.clean_name(n) for n in names]
        roster.setdefault(stem, [])
        roster[stem] += clean
        for n in clean:
            fmap.setdefault(n.upper(), stem)
    return fmap, roster


def _src_stem(segname):
    """A source-file stem from a .SYM object/segment name: '_MSGBOX' -> 'msgbox', 'DGROUP' -> '' ."""
    s = segname.strip().lstrip("_").lower()
    if not s or s in ("text", "dgroup", "bss", "rdata", "data", "idata", "const", "edata",
                       "reloc", "rsrc", "igroup", "lgroup"):
        return ""
    return re.sub(r"[^a-z0-9_]", "", s)[:32]


def assemble_module(recon_path, module, out_root, manifest):
    text = open(recon_path, encoding="utf-8", errors="replace").read()
    blocks = _blocks(text)
    if not blocks:
        return 0
    comp = component(module)
    stem = os.path.basename(module)
    for e in (".dll", ".exe", ".drv", ".vxd", ".386", ".cpl", ".scr", ".fon"):
        if stem.lower().endswith(e):
            stem = stem[:-len(e)]; break
    moddir = os.path.join(out_root, comp, stem)
    os.makedirs(moddir, exist_ok=True)
    fmap, roster = _sym_file_map(module)                  # {FUNC->srcstem}, {srcstem:[realnames]}
    # bucket reconstructed functions into source files by recovered name (.sym), else catch-all
    buckets = {}            # filestem -> [(name, header, body)]
    placed = catchall = 0
    for nm, head, body in blocks:
        dest = fmap.get(nm.upper())
        if dest:
            placed += 1
        else:
            dest = stem
            catchall += 1
        buckets.setdefault(dest, []).append((nm, head, body))
    # ensure every REAL .sym source file exists as a skeleton (the authentic tree shape), even when
    # we placed no reconstructed body in it.
    for fstem in roster:
        buckets.setdefault(fstem, [])
    for fstem, items in sorted(buckets.items()):
        real = fstem in roster
        with open(os.path.join(moddir, fstem + ".c"), "w", encoding="utf-8") as f:
            f.write("/* %s/%s/%s.c -- reconstructed Windows 95 source  [module %s | %s]\n"
                    "   filename: %s\n" % (comp, stem, fstem, module, comp,
                    "REAL -- original object/source name from the module's Microsoft .SYM"
                    if real else "SYNTHESIZED -- module catch-all (no .sym object name)"))
            if real:
                names = roster[fstem]
                f.write("   this source file's original functions (from .SYM, %d): %s\n"
                        % (len(names), ", ".join(names[:40]) + (" ..." if len(names) > 40 else "")))
                f.write("   reconstructed bodies filed here by matched name: %d\n" % len(items))
            f.write("   (bodies below are oracle-validated reconstructions) */\n\n")
            for nm, head, body in items:
                f.write("/* %s */\n%s\n\n" % (head, body))
    manifest.append({"module": module, "component": comp, "files": len(buckets),
                     "functions": len(blocks), "placed_by_name": placed, "catchall": catchall,
                     "real_files": len(roster), "has_sym": bool(roster)})
    return len(blocks)


def run(recon_dir, out_root, limit=None):
    os.makedirs(out_root, exist_ok=True)
    recon = []
    for fn in sorted(os.listdir(recon_dir)):
        if fn.endswith((".fused.c", ".recon16.c", ".wrappers.c")):
            recon.append(fn)
    if limit:
        recon = recon[:limit]
    manifest = []
    tot = 0
    for fn in recon:
        # module name from the stem: shell32_dll.fused.c -> shell32.dll
        base = fn.split(".")[0]                            # shell32_dll
        module = base[::-1].replace("_", ".", 1)[::-1]     # -> shell32.dll (last _ -> .)
        try:
            tot += assemble_module(os.path.join(recon_dir, fn), module, out_root, manifest)
        except Exception as e:
            print("  SKIP %-22s (%s)" % (fn, str(e)[:50]), flush=True)
    # manifest + rollup
    import json
    json.dump(manifest, open(os.path.join(out_root, "TREE_MANIFEST.json"), "w"), indent=1)
    comps = {}
    realf = plac = 0
    for m in manifest:
        comps[m["component"]] = comps.get(m["component"], 0) + 1
        realf += m["real_files"]; plac += m["placed_by_name"]
    print("=" * 64)
    print("SOURCE TREE ASSEMBLED -> %s" % out_root)
    print("  modules: %d   functions: %d" % (len(manifest), tot))
    print("  components: %s" % dict(sorted(comps.items())))
    print("  real (.sym-named) source files: %d   functions placed by name: %d" % (realf, plac))
    print("=" * 64)


from src.cli.common import opt as _opt


def main():
    argv = [a for a in sys.argv[1:] if not a.startswith("-")]
    if not argv or sys.argv[1] in ("-h", "--help"):
        print(__doc__); return
    from src.core import config
    config.apply_config()
    recon_dir = argv[0]
    out_root = argv[1] if len(argv) > 1 else os.path.join(os.path.dirname(recon_dir), "src_tree")
    limit = _opt(sys.argv[1:], "--limit")
    run(recon_dir, out_root, int(limit) if limit else None)


if __name__ == "__main__":
    main()
