#!/usr/bin/env python3
r"""srctree.py — classify the flat src/ into a real Windows source tree.

The reconstruction lands FLAT: src/generated/<module>.<ext>.c (one machine-C file
per shipped module) and src/reconstructed/<base>_segNN.c (hand-C, grouped by module
base). That's a decompiler dump, not a source tree. Microsoft laid Windows out BY
COMPONENT (core/gdi/user/shell/net/mmedia/setup + drivers/<class> for drivers); this
tool assigns every module a component path so the files can be moved into that shape.

    wt --target win95 srctree --dry-run     # show where each module would go (no move)
    wt --target win95 srctree --manifest    # write src/TREE.map  (module -> component)
    wt --target win95 srctree --fallbacks   # just the unclassified remainder to eyeball
    wt --target win95 srctree --apply       # MOVE files into src/<kind>/<component>/
    wt --target win95 srctree --unapply      # flatten back (undo --apply)

The manifest (src/TREE.map) is the single source of truth the mover + layout.py read.
Classification is deterministic and ordered: curated overrides > extension > name
heuristics > honest fallback (drivers/misc or apps, always LOGGED, never hidden).
"""
import os
from engine.foundation import cores
import re
import sys

from engine.foundation import layout

# ---------------------------------------------------------------------------
# 1. Curated overrides — the modules that DEFINE a component. Hand-pinned by
#    base name (no extension). These win over every heuristic below.
# ---------------------------------------------------------------------------
CURATED = {
    # core: kernel, VMM, config manager, the load-bearing system spine
    "core": """krnl386 kernel32 kernel vmm vmm32 configmg vxdldr ntkern sysinit
               vcache vwin32 vpicd vdmad vtd vmcpd vpowerd dynapage pagefile
               vfbackup vshare vcond int13 biosxlat ifsmgr""".split(),
    # gdi: graphics device interface + the display grinders that ride with it
    "gdi": "gdi gdi32 dibeng dciman dciman32 fontext".split(),
    # user: window manager, common controls
    "user": "user user32 comctl32 commctrl ddeml".split(),
    # shell: explorer, the shell namespace, launcher plumbing
    "shell": """shell shell32 explorer cabinet rundll rundll32 shdocvw shlwapi
                welcome mkcompat""".split(),
    # net: the whole networking stack (protocols, redirectors, sockets, NDIS)
    "net": """ndis ndis2sup vnetsup vnbt vip vtcp vtdi vdhcp vredir vserver
              nwlink nwnblink nwredir nwnet nwnp32 msnp32 mspp32 wsock wsock32
              winsock ws2_32 mstcp nscl netbios netapi netapi32 rpcltc rpcrt4
              vpmtd snmp dhcpcsvc pppmac ndiswan vnetbios""".split(),
    # mmedia: multimedia core + audio/video
    "mmedia": """mmsystem mmtask winmm msvideo msvfw32 msacm msacm32 dsound
                 midimap sndblst mmdevldr avifile avicap mciwave mciseq mcicda
                 mciavi mciqtz mciole quartz dxapi""".split(),
    # setup: the installer + hardware detection (our RE'd setup source lands here)
    "setup": """setup setupx suwin netdi sysdetmg sysdm sysclass net infinst
                w95inf16 w95inf32 detect setup4""".split(),
    # inet: dial-up + internet plumbing
    "inet": "wininet urlmon url dialer rnaui rnaapp rnanp pppmac dunzip".split(),
    # ctlpanel: control-panel applets that aren't just *.cpl by extension
    "ctlpanel": "main desk intl joy modem multimon password sysdm timedate".split(),
}
# invert to base -> component
_OVERRIDE = {b: comp for comp, bases in CURATED.items() for b in bases}

# ---------------------------------------------------------------------------
# 2. Extension -> component (only when no curated override matched).
# ---------------------------------------------------------------------------
EXT_MAP = {
    "cpl": "ctlpanel",
    "mpd": "drivers/disk",
    "scr": "apps",
}

# ---------------------------------------------------------------------------
# 3. Name heuristics — regex on the base name, first match wins. Subdivides the
#    driver mass (drivers/<class>) and catches components the curated list misses.
# ---------------------------------------------------------------------------
NAME_RULES = [
    (r"(disk|scsi|esdi|ide|atapi|cdfs|cdrom|cdtsd|cdvsd|ios|disktsd|aha|ncr|"
     r"btdosm|fdc|floppy|voltrack|drvspac|dblspace|dsktrace)", "drivers/disk"),
    (r"(disp|vga|svga|s3|cirrus|ati|mga|tseng|trident|weitek|framebuf|vflatd|"
     r"vdd|super|pnp|ega|8514|mach|w32|cirr)", "drivers/display"),
    (r"(print|unidrv|pscript|hppcl|deskjet|epson|canon|lexmark|hpdj|dmcolor|"
     r"finstall|plotter)", "drivers/printer"),
    (r"(mouse|kbd|keyboard|joy|vjoyd|vmouse|vkd|vmd|msmouse|gameport)", "drivers/input"),
    (r"(pccard|pcmcia|socket|csmapper|cbss|pccardsv|carddrv)", "drivers/pcmcia"),
    (r"(serial|modem|vcomm|vmodem|unimodem|comm|vcd)", "drivers/comm"),
    (r"(usb|uhcd|openhci|usbd|hid)", "drivers/usb"),
    (r"(sound|audio|wave|midi|sb16|sbpro|mpu401|opl3|es\d|ymf|cs4|ad1|wss)", "mmedia"),
    # ActiveMovie / video capture family (avifil32, avicap32, amovie, avcapt, ...)
    (r"(^av[a-z]|amovie|activemovie|quartz|indeo|iyuv|msh26|vidc)", "mmedia"),
    (r"(ndis|vnet|tcp|nw[a-z]|winsock|wsock|pppp|vnbt|nbt|dhcp|redir|netbeui)", "net"),
    # Microsoft At Work Fax subsystem (awfax32, awcodc32, awkrnl32, ... — ~30 modules)
    (r"(^aw[a-z0-9]*32$|atwork|awfax)", "fax"),
    (r"(font|fnt|type1|atmfd|ttf)", "gdi"),
    (r"(setup|detect|instal|inf[0-9])", "setup"),
]

# fallback bucket by extension family
DRIVER_EXTS = {"vxd", "386", "drv", "sys", "mpd", "pdr"}
APP_EXTS = {"exe", "scr"}


def _base_and_ext(fname):
    """('kernel32', 'dll') from 'kernel32.dll.c'  |  ('gdi', '') from 'gdi_seg01.c'."""
    stem = fname[:-2] if fname.endswith(".c") else fname          # drop .c
    m = re.match(r"^(.*)\.([A-Za-z0-9_]+)$", stem)
    if m and m.group(2).lower() in (
            DRIVER_EXTS | APP_EXTS | {"dll", "cpl", "ocx", "cnt", "tsp", "flt"}):
        return m.group(1).lower(), m.group(2).lower()
    # hand-C form: <base>_segNN / <base>_pN / <base>_lstr -> group by base
    base = re.split(r"_(seg\d+|p\d+|part\d+|[a-z]+\d*)$", stem, maxsplit=1)[0]
    return base.lower(), ""


# tier (from win95's target.tier_of) -> the component a module falls to when no
# curated override or name rule places it more precisely. This is the honest coarse
# router: a driver with no recognized class is drivers/misc, NOT apps.
TIER_FALLBACK = {
    "drivers": "drivers/misc",
    "apps": "apps",
    "core": "core",
    "system": "system",           # leftover system modules (most are curated away)
    "thirdparty": "thirdparty",
}


def _load_tier_map():
    """{ 'KERNEL32.DLL': 'system', ... } from win95's own tier classifier, or {} if
    target.py isn't importable (non-win95 / no corpus). Keyed by BASENAME.upper()."""
    try:
        target = cores.get('target')
    except Exception:
        return {}
    tm = {}
    for tier, paths in target.tiers().items():
        for p in paths:
            tm[os.path.basename(p).upper()] = tier
    return tm


def classify(fname, tier_map=None):
    """Return (component_path, reason) for a source file name. `tier_map` (base+ext
    upper -> tier) supplies the coarse fallback router when the precise rules miss."""
    base, ext = _base_and_ext(fname)
    if base in _OVERRIDE:
        return _OVERRIDE[base], "curated"
    # a couple of families key off the exact base+ext (cpl/mpd/scr)
    if ext in EXT_MAP:
        return EXT_MAP[ext], "ext:%s" % ext
    for pat, comp in NAME_RULES:
        if re.search(pat, base):
            return comp, "name"
    # coarse fallback via the module's TIER — a driver lands in drivers/, an app in apps/,
    # never the other way round. Only when even the tier is unknown do we guess by ext.
    if tier_map is not None and ext:
        tier = tier_map.get(("%s.%s" % (base, ext)).upper())
        if tier == "drivers":
            # target.py's "drivers" tier is really "not core/system/app": it sweeps in
            # every loose DLL too. A source tree must keep true device drivers (.vxd/
            # .drv/.sys/...) apart from libraries (.dll/.ocx) — file them separately.
            return ("drivers/misc" if ext in DRIVER_EXTS else "lib/misc"), "tier:driver"
        if tier:
            return TIER_FALLBACK[tier], "tier:%s" % tier
    if ext in DRIVER_EXTS:
        return "drivers/misc", "guess:driver"
    if ext in APP_EXTS:
        return "apps", "guess:app"
    return "lib/misc" if ext in ("dll", "ocx") else "apps", "guess:other"


def _scan():
    """[(relpath_within_src, filename, component, reason)] for every source file."""
    out = []
    tier_map = _load_tier_map()
    roots = [("generated", layout.generated_dir()),
             ("reconstructed", layout.recon_dir())]
    for kind, root in roots:
        if not os.path.isdir(root):
            continue
        for fn in sorted(os.listdir(root)):
            if not fn.endswith(".c"):
                continue
            comp, reason = classify(fn, tier_map)
            out.append((kind, fn, comp, reason))
    return out


def _roots():
    return {"generated": layout.generated_dir(), "reconstructed": layout.recon_dir()}


def _prune_empty(root):
    for dp, _dns, _fns in os.walk(root, topdown=False):
        if os.path.abspath(dp) != os.path.abspath(root) and not os.listdir(dp):
            os.rmdir(dp)


def _apply_asm(undo=False):
    """Mirror the component tree into asm/ — move each module's asm DIR (asm/<mod>)
    into asm/<component>/<mod>, or flatten back with undo=True. Same classification
    as the .c (matched by dir name or stem via layout.asm_component_map)."""
    aroot = layout.asm_dir()
    if not os.path.isdir(aroot):
        return 0
    moved = 0
    dirs = layout.asm_module_dirs()                      # captured before we mutate
    if undo:
        for d in dirs:
            dst = os.path.join(aroot, os.path.basename(d))
            if os.path.abspath(d) != os.path.abspath(dst) and not os.path.exists(dst):
                os.rename(d, dst)
                moved += 1
        _prune_empty(aroot)
        return moved
    cm = layout.asm_component_map()
    for d in dirs:
        if os.path.dirname(os.path.abspath(d)) != os.path.abspath(aroot):
            continue                                     # already nested
        name = os.path.basename(d)
        comp = cm.get(name.lower()) or cm.get(name.rsplit(".", 1)[0].lower(), "")
        if not comp:
            continue                                     # unmapped -> leave at root
        dst_dir = os.path.join(aroot, comp)
        dst = os.path.join(dst_dir, name)
        if os.path.abspath(dst) == os.path.abspath(d):
            continue                                     # already in place
        # collision: a module named exactly like its component (module `gdi` -> comp
        # `gdi`) would rename asm/gdi -> asm/gdi/gdi, i.e. a dir into itself. Stage it
        # through a temp name at the root so the component dir can be created first.
        if os.path.abspath(dst).startswith(os.path.abspath(d) + os.sep):
            tmp = os.path.join(aroot, "__srctree_tmp__")
            os.rename(d, tmp)
            os.makedirs(dst_dir, exist_ok=True)
            os.rename(tmp, dst)
        else:
            os.makedirs(dst_dir, exist_ok=True)
            os.rename(d, dst)
        moved += 1
    return moved


def apply_tree(undo=False, asm=True):
    """Move each source file into src/<kind>/<component>/ per the classification
    (or, with undo=True, flatten every component subdir back to the root). Uses
    plain os.rename so git records renames; idempotent — re-running is a no-op.
    asm=True also mirrors the tree into asm/ (module dirs)."""
    roots = _roots()
    moved = 0
    if undo:
        for kind, root in roots.items():
            for dp, _dns, fns in os.walk(root, topdown=False):
                if os.path.abspath(dp) == os.path.abspath(root):
                    continue
                for fn in fns:
                    if fn.endswith(".c"):
                        dst = os.path.join(root, fn)
                        if not os.path.exists(dst):
                            os.rename(os.path.join(dp, fn), dst)
                            moved += 1
                if not os.listdir(dp):
                    os.rmdir(dp)
        amoved = _apply_asm(undo=True) if asm else 0
        print("flattened %d src files + %d asm dirs back to the roots" % (moved, amoved))
        return
    rows = _scan()                                       # flat files only (non-recursive)
    for kind, fn, comp, _reason in rows:
        root = roots[kind]
        src = os.path.join(root, fn)
        if not os.path.isfile(src):                      # already moved / absent
            continue
        dst_dir = os.path.join(root, comp)
        os.makedirs(dst_dir, exist_ok=True)
        os.rename(src, os.path.join(dst_dir, fn))
        moved += 1
    amoved = _apply_asm(undo=False) if asm else 0
    print("moved %d src files into src/<kind>/<component>/ + %d asm module dirs into asm/<component>/"
          % (moved, amoved))


def main():
    args = set(sys.argv[1:]) or {"--dry-run"}
    if "--apply" in args or "--unapply" in args:
        apply_tree(undo="--unapply" in args)
        return
    rows = _scan()

    if "--manifest" in args:
        src = os.path.dirname(layout.generated_dir())          # .../src
        path = os.path.join(src, "TREE.map")
        with open(path, "w", encoding="utf-8") as f:
            f.write("# module-source -> component path  (generated by srctree.py)\n")
            f.write("# kind/filename\tcomponent\treason\n")
            for kind, fn, comp, reason in rows:
                f.write("%s/%s\t%s\t%s\n" % (kind, fn, comp, reason))
        print("wrote %s  (%d entries)" % (os.path.relpath(path), len(rows)))
        return

    # tally by component
    from collections import Counter
    bycomp = Counter(c for _k, _f, c, _r in rows)
    fallbacks = [(k, f, c, r) for k, f, c, r in rows if r.startswith("fallback")]

    if "--fallbacks" in args:
        print("UNCLASSIFIED (%d) — landed in fallback buckets, review these:\n"
              % len(fallbacks))
        for kind, fn, comp, reason in fallbacks:
            print("  %-28s -> %-10s (%s)" % (fn, comp, reason))
        return

    # default: --dry-run summary
    print("=" * 62)
    print("src-tree classification  (%d source files)" % len(rows))
    print("=" * 62)
    for comp in sorted(bycomp, key=lambda c: (-bycomp[c], c)):
        print("  %-14s %4d" % (comp, bycomp[comp]))
    print("-" * 62)
    print("  fallbacks: %d  (run --fallbacks to review)" % len(fallbacks))
    print("  next: --manifest to write src/TREE.map")


if __name__ == "__main__":
    main()
