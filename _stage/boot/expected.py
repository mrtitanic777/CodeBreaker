"""expected.py -- the ORIGINAL codebreaker.source.vxd.boot on REAL VxDs of the Windows 95 corpus, small FAT disk images made here (a
floppy-size FAT12 image with the files `plan` reads), seeded screens (PPM files), seeded BOOTLOG.TXT texts, and a real QEMU run.

Run as `python expected.py` from this folder (CB_WT names the worktree whose Python is imported; unset: the main checkout).
TEMP is tmp/ and the state folder state_py/ (state_cpp/ for test.exe).
Cases (tab separated; a text field is hex of its UTF-8, '~' empty):
  consts                               DEFAULT_DISK, GREYS, BOOT_OPTIONS
  corpus_file NAME / guest_path NAME   corpus_file(name) (the corpus folder as <C>) / guest_path(corpus_file(name))
  guest_raw PATH                       guest_path(path) of any path
  build NAME                           build(corpus_file(name)): [length, sha1];  build_path FILE: build(a file of inputs/)
  members FILE                         _bundle_members(bytes of FILE)
  bootopts TEXT OPTS                   boot_options(text, options): OPTS '-' (the default) or key=value pairs joined by ','
  plan DISK NAMES VMM32 RETAIL         plan(disk, drivers, vmm32, retail, log): [the log lines, {path: [length, sha1]}]
  classify IMG / resize IMG W H / crop IMG L T R B / pixel IMG X Y / png IMG   the image functions on a PPM of inputs/ (PIL as the reference)
  loaded FILE PATH                     loaded(text of FILE, path)
  bisect UNITS RULE                    bisect(units, trial, tag, log): the trials made, the result, the log
  boot DISK TAG WAIT MODE              boot(...): errors, and with MODE run a QEMU on a disk that cannot boot (the verdict, the bootlog, the files left)
  run DISK DRIVERS RETAIL TAG WAIT     run(...) with QEMU: [verdict, the log lines (without the times)]
  main ARG...                          main(argv): [status, stdout (the times, pids and screens' lines left out)]
"""
import contextlib
import io
import os
import random
import re
import shutil
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
os.chdir(HERE)
sys.path.insert(0, os.path.join(os.path.dirname(HERE), "_ne_common"))
import vxdcommon as X  # noqa: E402
from vxdcommon import hexs, sha1  # noqa: E402

X.isolate_env(HERE)
shutil.rmtree("inputs", ignore_errors=True)
os.makedirs("inputs", exist_ok=True)

from codebreaker.base import corpus, settings  # noqa: E402
from codebreaker.formats import media  # noqa: E402
from codebreaker.source.vxd import boot as B  # noqa: E402

from PIL import Image  # noqa: E402

K = X.VC.Cases(HERE)
open("capstone.txt", "w").write(X.VC.CAPSTONE_DLL)                  # the capstone.dll test.exe loads
rnd = random.Random(1116)
CROOT = corpus.root()


def norm_path(p):
    return p.replace(CROOT, "<C>").replace("\\", "/")


def lines_norm(ls):
    out = []
    for ln in ls:
        if re.match(r"^\s+\d+s\s+\w+ screen \d+x\d+$", ln):          # (a screen's change: when and what is timing)
            continue
        ln = re.sub(r"QEMU process \d+", "QEMU process N", ln)
        out.append(ln.replace(os.environ["CB_STATE"], "<STATE>"))
    return out


K.add("consts", lambda: [B.DEFAULT_DISK, [list(g) for g in B.GREYS], [list(kv) for kv in B.BOOT_OPTIONS.items()]])

# ---- the corpus drivers
names = ["splitter", "splitter.vxd", "SPLITTER", "vtdi", "vtdi.386", "vsd", "vsd.pdr", "vdhcp", "drvspacx", "nosuchdriver", "", "vmm32", "vmm32.vxd", "vmm.vxd", "x.y", "iosubsys", "dos386",
         "diskmgmt", "javasup", "quartz"]
for nm in names:
    K.add("corpus_file\t" + hexs(nm), lambda nm=nm: norm_path(B.corpus_file(nm)))
    K.add("guest_path\t" + hexs(nm), lambda nm=nm: B.guest_path(B.corpus_file(nm)))
for rel_ in ["WINDOWS/SYSTEM/X.VXD", "WINDOWS/SYSTEM/vmm32.vxd", "WINDOWS/SYSTEM/IOSUBSYS/a.pdr", "WINDOWS/SYSTEM/b.386", "WINDOWS\\SYSTEM\\c.vxd", "WINDOWS/system/d.VXD",
             "WINDOWS/x.vxd", "setup/e.vxd", "f.vxd", "WINDOWS/SYSTEM/sub/g.vxd", "WINDOWS/SYSTEM/.vxd", "WINDOWS/SYSTEM/h.VXd"]:
    K.add("guest_raw\t" + hexs(rel_), lambda rel_=rel_: B.guest_path(os.path.join(CROOT, rel_.replace("/", os.sep))))
other = "D:\\other\\drive\\x.vxd" if os.path.splitdrive(CROOT)[0].upper() == "C:" else "C:\\other\\drive\\x.vxd"      # (a path on another drive: relpath raises)
K.add("guest_raw\t" + hexs(other), lambda: B.guest_path(other))

drivers = X.small(10)
for p, i, nm, v in drivers[:5]:
    base_ = nm.split(".")[0]
    K.add("build\t" + hexs(base_), lambda nm=nm: (lambda b: [len(b), sha1(b)])(B.build(B.corpus_file(nm))))
K.add("build\t" + hexs("vmm32"), lambda: (lambda b: [len(b), sha1(b)])(B.build(B.corpus_file("vmm32"))))


# ---- W3 bundles and the disks
def w3_bundle(paths, names_=None):
    datas = [open(p, "rb").read() for p in paths]
    n = len(datas)
    front = bytearray(0x80)
    front[0:2] = b"MZ"
    struct.pack_into("<I", front, 0x3C, 0x80)
    hdr = b"W3" + struct.pack("<HI", 0x0400, n) + b"\0" * 8
    body_start = (0x80 + 16 + 16 * n + 0x3FF) & ~0x3FF
    out = bytearray(front) + hdr + b"\0" * (16 * n)
    while len(out) < body_start:
        out += b"\0"
    recs = []
    for i, d in enumerate(datas):
        off = len(out)
        out += d
        while len(out) % 0x400:
            out += b"\0"
        recs.append(((names_[i] if names_ else os.path.basename(paths[i]).split(".")[0].upper()).encode("latin1")[:8].ljust(8, b" "), off, len(d)))
    for i, (nm, off, size) in enumerate(recs):
        e = 0x90 + i * 16
        out[e:e + 8] = nm
        struct.pack_into("<II", out, e + 8, off, size)
    return bytes(out)


bundle = w3_bundle([drivers[0][0], drivers[1][0], drivers[2][0]])
with open("inputs/bundle3.vxd", "wb") as f:
    f.write(bundle)
with open("inputs/bundle1.vxd", "wb") as f:
    f.write(w3_bundle([drivers[3][0]]))
with open("inputs/notbundle.bin", "wb") as f:
    f.write(b"MZ" + bytes(rnd.randrange(256) for _ in range(300)))
for fn in ("bundle3.vxd", "bundle1.vxd", "notbundle.bin"):
    K.add("members\tinputs/" + fn, lambda fn=fn: B._bundle_members(open("inputs/" + fn, "rb").read()))
K.add("members\t" + norm_path(B.corpus_file("vmm32")), lambda: B._bundle_members(open(B.corpus_file("vmm32"), "rb").read()))
K.add("build_path\tinputs/bundle3.vxd", lambda: B.build("inputs/bundle3.vxd"))                   # a bundle of three: ValueError
K.add("build_path\tinputs/bundle1.vxd", lambda: (lambda b: [len(b), sha1(b)])(B.build("inputs/bundle1.vxd")))
K.add("build_path\tinputs/notbundle.bin", lambda: B.build("inputs/notbundle.bin"))


def make_fat12(path, files=None):
    img = bytearray(1474560)
    bs = bytearray(512)
    bs[0:3] = b"\xEB\x3C\x90"
    bs[3:11] = b"MSDOS5.0"
    struct.pack_into("<HBHBHHBHHHII", bs, 11, 512, 1, 1, 2, 224, 2880, 0xF0, 9, 18, 2, 0, 0)
    bs[38] = 0x29
    struct.pack_into("<I", bs, 39, 0x12345678)
    bs[43:54] = b"NO NAME    "
    bs[54:62] = b"FAT12   "
    bs[510:512] = b"\x55\xAA"
    img[0:512] = bs
    fat = bytearray(9 * 512)
    fat[0:3] = b"\xF0\xFF\xFF"
    img[512:512 + len(fat)] = fat
    img[512 + len(fat):512 + 2 * len(fat)] = fat
    with open(path, "wb") as f:
        f.write(img)
    if files:
        media.put(path, files)


MSDOS = b"[Paths]\r\nWinDir=C:\\WINDOWS\r\n[Options]\r\nBootGUI=1\r\nautoscan=1\r\nBootLog=0\r\nNetwork=0\r\n"
make_fat12("inputs/disk_plain.img")
make_fat12("inputs/disk_msdos.img", {"MSDOS.SYS": MSDOS})
make_fat12("inputs/disk_bundle.img", {"MSDOS.SYS": MSDOS, "WINDOWS/SYSTEM/VMM32.VXD": bundle})
make_fat12("inputs/disk_vmm32own.img", {"WINDOWS/SYSTEM/VMM32.VXD": bundle, "WINDOWS/SYSTEM/VMM32/" + drivers[1][2].split(".")[0].upper() + ".VXD": b"x"})
with open("inputs/disk_garbage.img", "wb") as f:
    f.write(bytes(rnd.randrange(256) for _ in range(4096)))

# boot_options
texts = [MSDOS.decode("latin1"), "", "[Options]", "[options]\r\nBootLog=5\r\n", "[Paths]\r\nx=1\r\n", "A=1\r\nbootlog = 2\r\n[Options]\r\nAUTOSCAN=7\r\nb=2",
         "[Options]\r\n[Options]\r\nBootLog=1\r\n", "BootLog=1\r\nAutoScan=0", "  [OPTIONS]  \r\nx=1", "line1\nline2\r\n[Options]\r\nz=9\r\n"]
for t in texts:
    K.add("bootopts\t%s\t-" % hexs(t), lambda t=t: B.boot_options(t))
    K.add("bootopts\t%s\tFoo=bar,BootLog=9,x=y" % hexs(t), lambda t=t: B.boot_options(t, {"Foo": "bar", "BootLog": "9", "x": "y"}))
K.add("bootopts\t%s\t~" % hexs("[Options]\r\nA=1"), lambda: B.boot_options("[Options]\r\nA=1", {}))

# plan: disks with MSDOS.SYS / a bundle
for disk in ("inputs/disk_plain.img", "inputs/disk_msdos.img", "inputs/disk_bundle.img", "inputs/disk_vmm32own.img"):
    for ds, vmm32, retail in ((["splitter"], False, True), (["splitter", "srammtd"], False, False), ([], True, True), (["vtdi"], True, True), ([], False, False)):
        def f(disk=disk, ds=ds, vmm32=vmm32, retail=retail):
            log = []
            res = B.plan(disk, ds, vmm32, retail, log=log.append)
            return [log, [[k, len(v), sha1(v)] for k, v in res.items()]]
        K.add("plan\t%s\t%s\t%d\t%d" % (disk, ",".join(ds) or "-", vmm32, retail), f)
for disk, ds in (("inputs/disk_garbage.img", ["splitter"]), ("inputs/nosuch.img", []), ("inputs/disk_msdos.img", ["nosuchdriver"])):
    K.add("plan\t%s\t%s\t0\t1" % (disk, ",".join(ds) or "-"), lambda disk=disk, ds=ds: B.plan(disk, ds, False, True, log=lambda s: None))


# ---- the images
def save_ppm(path, w, h, data):
    with open(path, "wb") as f:
        f.write(b"P6\n%d %d\n255\n" % (w, h) + data)


def fill(w, h, c):
    return bytes(c) * (w * h)


imgs = {}
imgs["blue"] = (640, 480, fill(640, 480, (0, 0, 170)))
imgs["blue_edge"] = (320, 200, fill(320, 200, (30, 30, 150)))
imgs["black"] = (640, 480, fill(640, 480, (0, 0, 0)))
imgs["black_text"] = (720, 400, fill(720, 400, (0, 0, 0)))
imgs["text_noise"] = (720, 400, bytes(rnd.randrange(256) for _ in range(720 * 400 * 3)))
imgs["teal"] = (640, 480, fill(640, 480, (0, 128, 128)))
imgs["teal_qemu"] = (800, 600, fill(800, 600, (87, 168, 168)))
imgs["teal_small"] = (320, 240, fill(320, 240, (0, 128, 128)))
imgs["white"] = (1024, 768, fill(1024, 768, (255, 255, 255)))
rows = []
for y in range(480):
    rows.append(bytes([192, 192, 192]) * 640 if y >= 456 else bytes([10, 200, 10]) * 640)
imgs["taskbar"] = (640, 480, b"".join(rows))
rows = []
for y in range(600):
    rows.append(bytes([192, 199, 200]) * 800 if y >= 576 else bytes([200, 10, 10]) * 800)
imgs["taskbar2"] = (800, 600, b"".join(rows))
rows = []
for y in range(480):
    rows.append((bytes([192, 192, 192]) * 640) if y >= 466 else (bytes([5, 5, 5]) * 640))
imgs["taskbar_dark"] = (640, 480, b"".join(rows))
for k, (w, h) in enumerate([(1, 1), (2, 3), (5, 5), (160, 120), (161, 119), (100, 50), (640, 480), (641, 481), (1024, 768), (800, 600), (720, 400), (639, 480), (3, 800), (1000, 7)]):
    imgs["noise%d_%dx%d" % (k, w, h)] = (w, h, bytes(rnd.randrange(256) for _ in range(w * h * 3)))
for k in range(6):                                                      # mostly teal with noise
    w, h = rnd.choice([(640, 480), (800, 600), (1024, 768)])
    d = bytearray(fill(w, h, (0, 128, 128)))
    for _ in range(w * h // rnd.choice([3, 5, 20])):
        at = rnd.randrange(w * h) * 3
        d[at:at + 3] = bytes(rnd.randrange(256) for _ in range(3))
    imgs["tealnoise%d" % k] = (w, h, bytes(d))
for k in range(4):                                                      # mostly blue with noise
    w, h = rnd.choice([(640, 480), (800, 600)])
    d = bytearray(fill(w, h, (0, 0, 170)))
    for _ in range(w * h // rnd.choice([2, 3, 4])):
        at = rnd.randrange(w * h) * 3
        d[at:at + 3] = bytes(rnd.randrange(256) for _ in range(3))
    imgs["bluenoise%d" % k] = (w, h, bytes(d))
for nm, (w, h, data) in imgs.items():
    save_ppm("inputs/%s.ppm" % nm, w, h, data)


def load(nm):
    return Image.open("inputs/%s.ppm" % nm).convert("RGB")


def rgbd(img):
    return [img.size[0], img.size[1], sha1(img.tobytes())]


for nm in imgs:
    K.add("classify\t" + nm, lambda nm=nm: B.classify(load(nm)))
    for (w, h) in ((160, 120), (80, 6), (1, 1), (3, 7), (200, 100)):
        K.add("resize\t%s\t%d\t%d" % (nm, w, h), lambda nm=nm, w=w, h=h: rgbd(load(nm).resize((w, h), 0)))
    w, h = imgs[nm][0], imgs[nm][1]
    for box in ((0, max(0, h - 24), w, h), (0, 0, w // 2, h // 2), (1, 1, w, h)):
        K.add("crop\t%s\t%d\t%d\t%d\t%d" % ((nm,) + box), lambda nm=nm, box=box: rgbd(load(nm).crop(box)))
    for _ in range(3):
        x, y = rnd.randrange(0, w), rnd.randrange(0, h)
        K.add("pixel\t%s\t%d\t%d" % (nm, x, y), lambda nm=nm, x=x, y=y: list(load(nm).getpixel((x, y))))
    K.add("pixel\t%s\t%d\t%d" % (nm, 0, h - 14), lambda nm=nm, h=h: list(load(nm).getpixel((0, h - 14))) if h > 14 else None)
    K.add("pixel\t%s\t%d\t%d" % (nm, w, h), lambda nm=nm, w=w, h=h: list(load(nm).getpixel((w, h))))


def png_case(nm):
    buf = io.BytesIO()
    load(nm).save(buf, "PNG")
    back = Image.open(io.BytesIO(buf.getvalue())).convert("RGB")
    return rgbd(back)


for nm in list(imgs)[:12]:
    K.add("png\t" + nm, lambda nm=nm: png_case(nm))
K.add("teal_all", lambda: [B._teal((r, g, b)) for r in (0, 10, 87, 100) for g in (100, 110, 111, 128, 168, 189, 190) for b in (100, 128, 140, 168, 180, 200)])

# ---- BOOTLOG.TXT
logs = {
    "log1": "[0010] Loading Vxd = vmm32\r\n[0020] LoadStart = C:\\WINDOWS\\SYSTEM\\VMM32\\VSPLIT.VXD\r\n[0021] LoadSuccess = C:\\WINDOWS\\SYSTEM\\VMM32\\VSPLIT.VXD\r\n"
            "[0030] SysCritInit = VSPLIT\r\n[0031] SysCritInitSuccess = VSPLIT\r\n[0040] DeviceInit = VSPLIT\r\n[0041] DeviceInitSuccess = VSPLIT\r\n[0050] Loading Vxd = SPLITTER\r\n"
            "[0060] InitComplete = SPLITTER\r\n",
    "log2": "no brackets = splitter.vxd\r\n  = SPLITTER\r\n[x] LoadFail = splitter\r\nLoadStart=\\SPLITTER.VXD\r\n[1] DeviceInit = SPLITTER.VXD\r\n[2] DeviceInitFail = splitter.vxd\r\n",
    "log3": "",
    "log4": "[1] A = splitter]x\r\n[2] B] = C:\\X\\SPLITTER.VXD\r\n[3] LoadStart = SPLITTER\r\n[4] LoadSuccess = SPLITTER\r\n[5] LoadStart = SPLITTER\r\n",
}
for nm, t in logs.items():
    with open("inputs/%s.txt" % nm, "wb") as f:
        f.write(t.encode("latin1"))
    for path in ("WINDOWS/SYSTEM/VMM32/SPLITTER.VXD", "WINDOWS/SYSTEM/VSPLIT.VXD", "splitter.386", "SPLITTER", "WINDOWS/SYSTEM/VMM32/VSPLIT.VXD", "x/y/z"):
        K.add("loaded\tinputs/%s.txt\t%s" % (nm, hexs(path)), lambda nm=nm, path=path: B.loaded(open("inputs/%s.txt" % nm, "rb").read().decode("latin1"), path))

# ---- bisect (a fake trial: a unit fails by itself / two fail only together)
for units, rule in ((["a"], "a"), (["a", "b"], "a"), (["a", "b"], "ab"), (["a", "b", "c", "d"], "c"), (["a", "b", "c", "d"], "bd"), (["a", "b", "c", "d", "e", "f", "g"], "ae"),
                    (["a", "b", "c", "d", "e"], "xyz"), (["a", "VMM32", "c"], "VMM32"), ([], "a")):
    def f(units=units, rule=rule):
        trials, log = [], []

        def trial(part, name):
            trials.append([list(part), name])
            return "FAIL(x)" if ("&" not in rule and rule in [u for u in part]) or (rule == "ab" and "a" in part and "b" in part) or (rule == "bd" and "b" in part and "d" in part) \
                or (rule == "ae" and ("a" in part or "e" in part)) else "PASS"
        res = B.bisect(units, trial, "tg", log=log.append)
        return [trials, res, log]
    K.add("bisect\t%s\t%s" % (",".join(units) or "-", rule), f)

# ---- boot / run
STATE = os.environ["CB_STATE"]


def files_state(tag):
    # (whether a screen was saved depends on whether QEMU's monitor was up at the first look: not compared; the bootlog file is)
    p = os.path.join(STATE, "vxdboot", "shots", tag + ".bootlog.txt")
    return [None if not os.path.exists(p) else sha1(open(p, "rb").read())]


def boot_err(disk, tag, wait):
    try:
        B.boot(disk, {"X.TXT": b"hi"}, tag, wait, log=lambda s: None)
    except Exception as e:  # noqa: BLE001
        return [type(e).__name__, str(e).replace(os.path.abspath("."), "<H>") if type(e).__name__ == "RuntimeError" else ""]
    return None


K.add("boot\tinputs/nosuch.img\tt_none\t5\terr", lambda: boot_err("inputs/nosuch.img", "t_none", 5))
K.add("boot\tinputs/disk_garbage.img\tt_garb\t5\terr", lambda: boot_err("inputs/disk_garbage.img", "t_garb", 5))


def nospace():
    os.environ["CB_MIN_FREE_GB"] = "100000000"
    try:
        return boot_err("inputs/disk_plain.img", "t_space", 5)
    finally:
        del os.environ["CB_MIN_FREE_GB"]


K.add("boot\tinputs/disk_plain.img\tt_space\t5\tspace", nospace)


def boot_floppy(tag, wait):
    log = []
    r = B.boot("inputs/disk_plain.img", {"X.TXT": b"hi"}, tag, wait, log=log.append)
    return [r[0], r[2], os.path.basename(r[1]), files_state(tag), lines_norm(log)]


K.add("boot\tinputs/disk_plain.img\tt_floppy\t10\tfloppy", lambda: boot_floppy("t_floppy", 10))
K.add("boot\tinputs/disk_plain.img\tt_floppy2\t1\tfloppy", lambda: boot_floppy("t_floppy2", 1))


def run_case(disk, ds, retail, tag, wait):
    log = []
    v = B.run(disk, ds, False, retail, tag, wait, log=log.append)
    return [v, lines_norm(log)]


K.add("run\tinputs/disk_msdos.img\t-\t1\tt_run1\t8", lambda: run_case("inputs/disk_msdos.img", [], True, "t_run1", 8))
K.add("run\tinputs/disk_msdos.img\tsplitter,srammtd\t1\tt_run2\t8", lambda: run_case("inputs/disk_msdos.img", ["splitter", "srammtd"], True, "t_run2", 8))


def main_case(args):
    old = sys.stdout
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf):
        try:
            status = B.main(args)
        except Exception as e:  # noqa: BLE001
            return ["ERR", type(e).__name__]
    return [status, lines_norm(buf.getvalue().splitlines()), files_state(args[args.index("--tag") + 1]) if "--tag" in args else None]


for args in ([], ["-h"], ["--help"], ["--nosuch"], ["--drivers"], ["--vmm32", "--wat"], ["--disk", "inputs/nosuch.img"], ["--disk", "inputs/disk_plain.img", "--wait", "x"],
             ["--disk", "inputs/disk_garbage.img", "--drivers", "splitter"], ["--disk", "inputs/disk_msdos.img", "--wait", "6", "--tag", "t_m1", "--retail"],
             ["--disk", "inputs/disk_msdos.img", "--drivers", "splitter,srammtd", "--wait", "6", "--tag", "t_m2", "--bisect", "--retail"],
             ["--disk", "inputs/disk_msdos.img", "--drivers", " splitter , ,VSD", "--wait", "6", "--tag", "t_m3"]):
    K.add("\t".join(["main"] + [hexs(a) for a in args]), lambda args=args: main_case(args))
K.close()
print(K.n, "cases")
