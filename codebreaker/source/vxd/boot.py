"""boot.py -- does Windows 95 start with these drivers? A rebuilt driver's real test.

    python -m codebreaker vxd boot [--drivers NAME,...] [--vmm32] [--retail] [--bisect]
                                   [--disk IMAGE] [--wait SECONDS] [--tag NAME]

Each driver named (vtd, vpicd, drvspacx ...: a file of the corpus) is built from the recovered
disassembly with le_writer.build_le, and with --vmm32 Windows' own VMM32.VXD with build_w3; they
are put into a copy of an installed Windows 95 disk, and the copy is booted in QEMU with no
window. Windows reaching its desktop (or a logon box over it) is a pass; a blue screen, a screen
that stays black or in text (a "Windows protection error" is one), or no desktop in --wait
seconds (default 300) is a fail. --retail puts the corpus originals in instead of the rebuilt
files: the baseline a rebuilt run is measured against. --bisect, when the run fails, boots
halves of the set until it finds the drivers that fail by themselves.

Where each file goes: a driver kept in WINDOWS\\SYSTEM of the corpus goes into
WINDOWS\\SYSTEM\\VMM32 -- Windows loads a driver found there in place of the copy inside VMM32.VXD
-- and one kept in a folder below it (IOSUBSYS) or with another extension (.386) where the corpus
has it. VMM32.VXD goes into WINDOWS\\SYSTEM. The corpus VMM32.VXD holds VMM alone, where the
installed one (W4, compressed) holds 43 drivers; so with --vmm32 every other driver of the
installed bundle the disk has no file of in VMM32 is put there from the corpus too (Windows takes
a driver it does not find in the bundle from there).

The disk is never changed: `--disk`, or the setting "win95_installed_hd" (a raw image of a hard
disk Windows 95 is installed on; default the one the Phase 3 install left in WSL), is copied into
a run folder of its own (base/runs.py), which is deleted when the run ends -- and a killed run's,
by the next run, once it is a day old. BootLog=1 is set in the copy's MSDOS.SYS; after the run the
copy's BOOTLOG.TXT says which of the files Windows loaded, and it is kept with the last screen,
one of each per tag (state/vxdboot/shots/<tag>.png and <tag>.bootlog.txt; the tag is "rebuilt"
or "retail" unless --tag names another, and a run overwrites the last of its tag). That
image has FIX95CPU's drivers in VMM32 already: without them a fast host fails at IOS, whatever
the drivers (the classic timing-loop fault of Windows 95), and those files are left alone.

QEMU runs at below-normal priority, one machine at a time; its process number is printed.
"""
import os
import shutil
import subprocess
import sys
import time

from codebreaker.base import corpus, runs, settings
from codebreaker.formats import le, media

DEFAULT_DISK = r"\\wsl$\Ubuntu\home\mrtit\win95_install.img"
GREYS = ((192, 192, 192), (192, 199, 200))          # the taskbar: the palette's grey, and on QEMU's VGA


# What goes where ---------------------------------------------------------------------------------
def corpus_file(name):
    """The corpus original of a driver name ("vtd", "vtd.vxd", "vdhcp", "drvspacx") -> its path."""
    names = [name] if "." in name else [name + ext for ext in (".vxd", ".386", ".pdr")]
    for n in names:
        path = corpus.retail(n)
        if path:
            return path
    raise ValueError("no driver %r in the corpus" % name)


def guest_path(path):
    """Where on an installed disk a corpus driver goes: WINDOWS/SYSTEM/x.vxd into VMM32/, any other
    under the corpus' own folder."""
    rel = os.path.relpath(path, corpus.root()).replace(os.sep, "/")
    folder, name = os.path.split(rel)
    if folder.upper() == "WINDOWS/SYSTEM" and name.upper().endswith(".VXD") and name.upper() != "VMM32.VXD":
        folder = "WINDOWS/SYSTEM/VMM32"
    return (folder + "/" + name).upper()


def build(path):
    """A driver rebuilt from its recovered disassembly: build_le, or build_w3 for a W3 bundle."""
    from codebreaker.source.vxd import le_writer
    from codebreaker.source.vxd.disassembly import VxDDisasm
    with open(path, "rb") as f:
        data = f.read()
    v = le.open_vxds(path)[0][1]
    d = VxDDisasm(v).run()
    d.v = v
    c = le.container(data)
    if c and c["kind"] == "W3":
        if len(c["members"]) != 1:
            raise ValueError("%s: a bundle of %d drivers; build_w3 rebuilds one" % (path, len(c["members"])))
        return le_writer.build_w3(d, v, c["members"][0][0])
    return le_writer.build_le(d, v, d.ddb["name"] if d.ddb else "DEVICE")


def _original(path):
    with open(path, "rb") as f:
        return f.read()


def _bundle_members(data):
    """The driver names inside an installed VMM32.VXD (W4 or W3)."""
    c = le.container(data)
    if c and c["kind"] == "W4":
        c = le.container(le.expand(data))
    return [m[0] for m in (c or {}).get("members", [])]


BOOT_OPTIONS = {"BootLog": "1",          # C:\BOOTLOG.TXT: which drivers loaded, where it stopped
                "AutoScan": "0"}         # no ScanDisk first: every run stops the machine unclean


def boot_options(text, options=None):
    """MSDOS.SYS with `options` (default BOOT_OPTIONS) set under [Options]."""
    options = options or BOOT_OPTIONS
    lines = [ln for ln in text.split("\r\n")
             if ln.split("=")[0].strip().lower() not in [k.lower() for k in options]]
    at = next((i + 1 for i, ln in enumerate(lines) if ln.strip().lower() == "[options]"), None)
    if at is None:
        lines[0:0], at = ["[Options]"], 1
    lines[at:at] = ["%s=%s" % kv for kv in options.items()]
    return "\r\n".join(lines)


def plan(disk, drivers=(), vmm32=False, retail=False, log=print):
    """{path on the disk: bytes} for a run: the drivers (rebuilt, or the originals with `retail`),
    VMM32.VXD with `vmm32` and the bundle's other drivers it then needs, MSDOS.SYS with BOOT_OPTIONS."""
    vol = media.volumes(disk)[0]
    files = {e.path.upper(): e for e in vol.files()}
    out = {}
    for name in list(drivers) + (["vmm32.vxd"] if vmm32 else []):
        path = corpus_file(name)
        out[guest_path(path)] = _original(path) if retail else build(path)
        log("  %-34s %s" % (guest_path(path), "original" if retail else "rebuilt"))
    if vmm32:
        own = files.get("WINDOWS/SYSTEM/VMM32.VXD")
        fallback = []
        for m in _bundle_members(vol.read(own)) if own else []:
            at = "WINDOWS/SYSTEM/VMM32/%s.VXD" % m.upper()
            if m.upper() == "VMM" or at in out or at in files:
                continue
            out[at] = _original(corpus_file(m.lower() + ".vxd"))
            fallback.append(m)
        if fallback:
            log("  + %d drivers of the installed bundle as their own files (originals): %s"
                % (len(fallback), " ".join(fallback)))
    if "MSDOS.SYS" in files:
        out["MSDOS.SYS"] = boot_options(vol.read(files["MSDOS.SYS"]).decode("latin1")).encode("latin1")
    return out


# The machine --------------------------------------------------------------------------------------
def _teal(p):
    """Windows 95's desktop colour: (0, 128, 128) in the palette, (87, 168, 168) on QEMU's VGA."""
    r, g, b = p
    return abs(g - b) < 12 and 110 < g < 190 and r < g - 50


def _pixels(img):
    raw = img.tobytes()
    return [tuple(raw[i:i + 3]) for i in range(0, len(raw), 3)]


def classify(img):
    """What a screen shows: "desktop", "blue" (a fatal exception, or ScanDisk), "text" (the text
    mode: a protection error, "Press a key", DOS), "black" (a graphics mode with nothing on it),
    "other" (Windows' logo, a box with no desktop behind it ...)."""
    w, h = img.size
    px = _pixels(img.convert("RGB").resize((160, 120), 0))
    share = lambda test: sum(1 for p in px if test(p)) / len(px)                # noqa: E731
    if share(lambda p: p[2] > 140 and p[0] < 40 and p[1] < 40) > 0.6:
        return "blue"
    if (w, h) == (720, 400):
        return "text"
    if share(lambda p: sum(p) < 40) > 0.97:
        return "black"
    # (the desktop: mostly teal, with whatever box is on it -- the logon, "Welcome", a clock
    # message -- and the taskbar once Explorer has it, in a 640x480 or larger mode)
    if w >= 640 and h >= 480 and share(_teal) > 0.4:
        return "desktop"
    if w >= 640 and h >= 480:                                   # (a wallpaper: the taskbar, then)
        bar = _pixels(img.convert("RGB").crop((0, h - 24, w, h)).resize((80, 6), 0))
        if sum(1 for p in bar if p in GREYS) > len(bar) * 0.7 and img.convert("RGB").getpixel((0, h - 14)) in GREYS:
            return "desktop"
    return "other"


def boot(disk, files, tag, wait=300, log=print):
    """Copy `disk`, write `files` into the copy, boot it. -> (verdict, screenshot, bootlog text)
    verdict: "PASS" | "FAIL(blue screen)" | "FAIL(text screen)" | "FAIL(black screen)" | "FAIL(timeout)"
    | "FAIL(qemu exited)" """
    from codebreaker.verify.gate16 import setup16
    qemu = settings.get("qemu") or r"C:\Program Files\qemu\qemu-system-i386.exe"
    shots = runs.results("vxdboot")
    os.makedirs(shots, exist_ok=True)
    png = os.path.join(shots, tag + ".png")
    need = os.path.getsize(disk) + float(settings.get("min_free_gb") or 6) * 2 ** 30
    if shutil.disk_usage(settings.state("vxdboot")).free < need:
        raise RuntimeError("not enough free disk space for a copy of %s" % disk)
    work = runs.start("vxdboot", tag)                           # (the disk copy: this run's alone)
    q, verdict, bootlog = None, "FAIL(timeout)", ""
    try:
        hd = os.path.join(work, "hd.img")
        shutil.copyfile(disk, hd)
        media.put(hd, files)
        port = setup16._free_port()
        q = subprocess.Popen([qemu, "-M", "pc", "-m", "64", "-drive", "file=%s,format=raw,if=ide,index=0" % hd,
                              "-boot", "c", "-display", "none", "-monitor", "tcp:127.0.0.1:%d,server,nowait" % port],
                             stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
                             creationflags=getattr(subprocess, "BELOW_NORMAL_PRIORITY_CLASS", 0))
        log("  QEMU process %d (below-normal priority), up to %ds" % (q.pid, wait))
        mon = setup16._Monitor(port)
        seen, same, last, until = [], 0, None, time.time() + wait
        while time.time() < until:
            time.sleep(4)
            if q.poll() is not None:
                verdict = "FAIL(qemu exited)"
                break
            try:
                img = mon.shot(png)
            except (OSError, ValueError):
                continue
            kind = classify(img)
            seen.append(kind)
            if kind != last:
                log("    %3ds  %s screen %dx%d" % (wait - (until - time.time()), kind, img.size[0], img.size[1]))
            same = same + 1 if kind == last else 0
            last = kind
            if kind == "desktop" and same >= 2:                 # (three screens running: settled)
                verdict = "PASS"
                break
            if kind == "blue" and same >= 8:                    # (ScanDisk is blue too, for a while)
                verdict = "FAIL(blue screen)"
                break
            # a text screen or a black one after Windows' own graphic screens is a stop (the
            # protection error; a fault with the screen left dark), but only once it has stayed
            if kind in ("text", "black") and "other" in seen and same >= 15:
                verdict = "FAIL(%s screen)" % kind
                break
    finally:
        if q is not None:
            q.kill()
            try:
                q.wait(timeout=20)
            except subprocess.TimeoutExpired:
                pass
        try:
            bootlog = _read(os.path.join(work, "hd.img"), "BOOTLOG.TXT")
        except (OSError, ValueError, KeyError):
            pass
        runs.remove(work)                                       # (QEMU lets go of its files a moment later)
    with open(os.path.join(shots, tag + ".bootlog.txt"), "w", encoding="latin1", newline="") as f:      # (a text-mode write made each "\r\n" "\r\r\n")
        f.write(bootlog)
    return verdict, png, bootlog


def _read(image, name):
    vol = media.volumes(image)[0]
    entry = {e.path.upper(): e for e in vol.files()}[name.upper()]
    return vol.read(entry).decode("latin1")


def loaded(bootlog, path):
    """What BOOTLOG.TXT says of a driver file put in: its load and the phases of its start-up
    ("LoadSuccess SYSCRITINITSUCCESS DEVICEINITSUCCESS ..."), the last one where it stopped; ""
    when the log never names it (not a driver this machine loads)."""
    name = path.rsplit("/", 1)[-1].upper()
    stem = name.split(".")[0]
    steps = []
    for ln in bootlog.splitlines():
        what, _, value = ln.split("]", 1)[-1].partition("=")
        value = value.strip().upper()
        if value.endswith("\\" + name) or value == name or value == stem:
            step = what.strip()
            if step != "Loading Vxd" and step not in steps:
                steps.append(step)
    return " ".join(s for s in steps if s + "SUCCESS" not in steps)       # (a phase that ended: once)


def run(disk, drivers, vmm32, retail, tag, wait, log=print):
    label = ", ".join(list(drivers) + (["VMM32"] if vmm32 else [])) or "(nothing put in)"
    log("%s: %s %s" % (tag, "originals of" if retail else "rebuilt", label))
    files = plan(disk, drivers, vmm32, retail, log)
    verdict, png, bootlog = boot(disk, files, tag, wait, log)
    for d in drivers:
        p = guest_path(corpus_file(d))
        log("    %-34s %s" % (p, loaded(bootlog, p) or "(not in BOOTLOG.TXT: not loaded)"))
    log("  %s   screen: %s" % (verdict, png))
    if verdict != "PASS" and bootlog:
        log("  BOOTLOG.TXT ends:\n    " + "\n    ".join(bootlog.strip().splitlines()[-6:]))
    return verdict


def bisect(units, trial, tag, log=print):
    """The units (driver names, "VMM32") of a failing set that fail by themselves, found by
    booting halves: trial(units, tag) -> verdict."""
    def go(group, n):
        if len(group) == 1:
            return group
        found = []
        for i, part in enumerate((group[:len(group) // 2], group[len(group) // 2:])):
            if trial(part, "%s_b%s%d" % (tag, n, i)) != "PASS":
                found += go(part, n + str(i))
        if not found:
            log("  %s: no half fails alone -- they fail together" % ", ".join(group))
            return group
        return found
    return go(list(units), "")


def main(argv):
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__)
        return 0 if argv else 2
    opts = {"--drivers": "", "--disk": settings.get("win95_installed_hd") or DEFAULT_DISK, "--wait": "300", "--tag": ""}
    flags, i = set(), 0
    while i < len(argv):
        a = argv[i]
        if a in opts and i + 1 < len(argv):
            opts[a] = argv[i + 1]
            i += 2
            continue
        if a in ("--vmm32", "--retail", "--bisect"):
            flags.add(a)
        else:
            print("unknown argument %r\n%s" % (a, __doc__))
            return 2
        i += 1
    drivers = [d.strip().lower() for d in opts["--drivers"].split(",") if d.strip()]
    disk = opts["--disk"]
    if disk.lower().startswith(("\\\\wsl$", "\\\\wsl.localhost")) and not os.path.isfile(disk) and shutil.which("wsl"):
        subprocess.run(["wsl", "-e", "true"], capture_output=True, timeout=120)     # (WSL stops when idle)
    if not os.path.isfile(disk):
        print("no installed Windows 95 disk at %s (set win95_installed_hd in settings.json, or --disk)" % disk)
        return 1
    tag = opts["--tag"] or ("retail" if "--retail" in flags else "rebuilt")     # (the last of a tag is kept)
    verdict = run(disk, drivers, "--vmm32" in flags, "--retail" in flags, tag, int(opts["--wait"]))
    if verdict != "PASS" and "--bisect" in flags:
        units = drivers + (["VMM32"] if "--vmm32" in flags else [])
        if len(units) > 1:
            culprits = bisect(units, lambda part, name: run(disk, [u for u in part if u != "VMM32"], "VMM32" in part,
                                                            "--retail" in flags, name, int(opts["--wait"])), tag)
            print("fails: %s" % ", ".join(culprits))
    return 0 if verdict == "PASS" else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
