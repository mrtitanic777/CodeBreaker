"""Does Windows 95 Setup run with these modules? The real system's test of a rebuilt module.

Setup's own CD is copied with WIN95\\MINI.CAB rebuilt -- the original's files, with the ones
given in their place -- and booted in QEMU from a disk Setup has already prepared and a floppy
that starts `SETUP /IS`. At the Welcome dialog Enter is pressed (Continue); Setup then shows
its License Agreement page. That page on the screen is a pass; Windows back at the DOS prompt,
an error box, or Windows' screen with nothing on it is a fail.

Everything a run copies (the CD, the disk -- 650 MB) is made in a folder of its own
(base/runs.py: state/phase3/runs/) and gone when the run ends -- a killed run's, by the next
run once it is a day old; only the last screen of each tag is kept (state/phase3/shots/<tag>.png),
for whoever looks at a failure.

    continue_test({"USER.EXE": path, ...}, tag) -> (verdict, screenshot)
      verdict: "PASS" | "FAIL(dos)" | "FAIL(hang)" | "FAIL(error box)" | "FAIL(timeout)"
               | "FAIL(error box before Welcome)"
               | "SLOW" (Setup's DOS part never finished: the run says nothing)"""
import io
import os
import shutil
import socket
import subprocess
import time

from codebreaker.base import runs, settings

LICENSE_GREY = (192, 199, 200)
LICENSE_POINTS = ((60, 350), (580, 350), (300, 395))


def _paths():
    mini = settings.state("mini")
    return {
        "qemu": settings.get("qemu") or r"C:\Program Files\qemu\qemu-system-i386.exe",
        "iso": settings.get("win95_setup_iso"),
        "original": os.path.join(mini, "original"),
        "cab": os.path.join(mini, "MINI.CAB"),
        "hd": os.path.join(mini, "prepared_hd.img"),
        "floppy": settings.state("bootfloppy", "boot95_is.img"),
    }


def _free_port():
    s = socket.socket()
    s.bind(("127.0.0.1", 0))
    port = s.getsockname()[1]
    s.close()
    return port


def make_cd(out, replace):
    """Setup's CD with MINI.CAB rebuilt from the original's files, `replace` {NAME: path} in place
    of some."""
    import pycdlib
    from codebreaker.formats import cab
    p = _paths()
    names = [e.name for e in cab.read(p["cab"]).files]
    up = {k.upper(): v for k, v in replace.items()}
    files = [(n, open(up.get(n.upper()) or os.path.join(p["original"], n), "rb").read()) for n in names]
    tmp = out + ".cab"
    cab.write(files, tmp, compress="mszip", set_id=16261)
    with open(tmp, "rb") as f:
        data = f.read()
    os.remove(tmp)
    iso = pycdlib.PyCdlib()
    iso.open(p["iso"])
    joliet = "/WIN95/MINI.CAB" if iso.has_joliet() else None
    iso.rm_file(iso_path="/WIN95/MINI.CAB;1", joliet_path=joliet)
    iso.add_fp(io.BytesIO(data), len(data), "/WIN95/MINI.CAB;1", joliet_path=joliet)
    # The rest of Setup's own programs -- SUWIN, SETUPX and the others, in PRECOPY2.CAB, and
    # SETUP.EXE loose beside the cabinets -- rebuilt from ours where `replace` names them: a
    # cabinet holding one is written again with the original's other files and its set fields
    rest = {k: v for k, v in up.items() if k not in {n.upper() for n in names}}
    if rest:
        _replace_on_cd(iso, rest, out)
    iso.write(out)
    iso.close()


def _replace_on_cd(iso, rest, out):
    """Put the files of `rest` {NAME: path} in place on the open CD under /WIN95: a loose file
    replaced, a file in a cabinet by the cabinet written again."""
    import shutil
    import tempfile
    from codebreaker.formats import cab
    work = tempfile.mkdtemp(prefix="cd_", dir=os.path.dirname(os.path.abspath(out)))
    try:
        idents = {c.file_identifier().decode().split(";")[0].upper(): c.file_identifier().decode()
                  for c in iso.list_children(iso_path="/WIN95") if c.file_identifier() not in (b".", b"..")}
        cabs = {}
        for name in idents:                                  # (every cabinet, beside the others: a file
            if name.endswith(".CAB"):                        # one cabinet continues into the next is
                raw = io.BytesIO()                           # read from both)
                iso.get_file_from_iso_fp(raw, iso_path="/WIN95/" + idents[name])
                with open(os.path.join(work, name), "wb") as f:
                    f.write(raw.getvalue())
                cabs[name] = cab.read(os.path.join(work, name))
        new = {name: open(rest[name], "rb").read() for name in rest if name in idents}   # (loose: SETUP.EXE)
        # A cabinet holding one of ours is written again -- with the whole set it belongs to:
        # PRECOPY1's last three files run on into PRECOPY2, and PRECOPY2 rewritten alone left
        # PRECOPY1 pointing into a cabinet that no longer continued them ("Could not decompress
        # this Setup (.CAB) file"). Each file is now whole in the cabinet it starts in.
        todo = set()
        for name, c in cabs.items():
            if {e.name.upper() for e in c.files} & set(rest):
                k = name
                while cabs.get(k) and cabs[k].info.get("prev"):
                    k = cabs[k].info["prev"].upper()
                while k in cabs:
                    todo.add(k)
                    k = (cabs[k].info.get("next") or "").upper()
        for name in todo:
            c = cabs[name]
            got = cab.extract(os.path.join(work, name), os.path.join(work, name + ".d"))
            files = [(e.name, open(rest.get(e.name.upper()) or got[e.name], "rb").read())
                     for e in c.files if e.folder not in (0xFFFD, 0xFFFF)]   # (continued: in the one before)
            i = c.info
            cab.write(files, os.path.join(work, name + ".new"), compress="mszip", set_id=i.get("set_id", 0),
                      index=i.get("index", 0), prev=i.get("prev"), next=i.get("next"))
            new[name] = open(os.path.join(work, name + ".new"), "rb").read()
        for name, data in new.items():
            joliet = ("/WIN95/" + name) if iso.has_joliet() else None
            iso.rm_file(iso_path="/WIN95/" + idents[name], joliet_path=joliet)
            iso.add_fp(io.BytesIO(data), len(data), "/WIN95/" + idents[name], joliet_path=joliet)
    finally:
        shutil.rmtree(work, ignore_errors=True)              # (no copies left behind)


class _Monitor(object):
    def __init__(self, port):
        self.port = port

    def send(self, *cmds):
        s = socket.create_connection(("127.0.0.1", self.port), timeout=5)
        s.settimeout(2)
        try:
            s.recv(4096)
        except socket.timeout:
            pass
        for c in cmds:
            s.sendall((c + "\n").encode())
            time.sleep(0.3)
        s.close()

    def shot(self, png):
        from PIL import Image
        ppm = png[:-4] + ".ppm"
        self.send("screendump " + ppm.replace("\\", "/"))
        time.sleep(1.2)
        img = Image.open(ppm).convert("RGB")
        img.save(png)
        os.remove(ppm)
        return img


def _pixel(img, xy):
    """The pixel at xy, or None where the screen has none: a 640-wide screen shorter than the page's lowest
    point (a 640x350 one) is not that page, and asking it for the point ended the whole run with IndexError."""
    return img.getpixel(xy) if xy[0] < img.size[0] and xy[1] < img.size[1] else None


def _license(img):
    return img.size[0] == 640 and all(_pixel(img, xy) == LICENSE_GREY for xy in LICENSE_POINTS)


def _error_box(img):
    if img.size[0] != 640:
        return False
    for xy in ((97, 310), (320, 175), (540, 310)):
        p = _pixel(img, xy)
        if p is None or sum(p) <= 700:
            return False
    return True


def _welcome(img):
    if img.size[0] != 640:
        return False
    small = img.resize((160, 120))
    return sum(1 for r, g, b in small.getdata() if r > 200 and g > 200 and b > 200) > 2000


def _black(img):
    px = list(img.resize((90, 50)).getdata())
    return sum(1 for r, g, b in px if r + g + b < 60) / len(px) > 0.9


def continue_test(replace, tag, wait=150, keep_shot=None):
    """Boot Setup with `replace` {NAME: path} and press Continue. -> (verdict, screenshot path)"""
    p = _paths()
    shots = keep_shot or runs.results("phase3")
    os.makedirs(shots, exist_ok=True)
    png = os.path.join(shots, tag + ".png")
    work = runs.start("phase3", tag)                         # (the disk and CD copies: this run's alone)
    q = None
    verdict = "SLOW"
    try:
        iso = os.path.join(work, "cd.iso")
        make_cd(iso, replace)
        hd = os.path.join(work, "hd.img")
        shutil.copy(p["hd"], hd)
        port = _free_port()
        q = subprocess.Popen([p["qemu"], "-M", "pc", "-m", "64",
                              "-drive", "file=%s,format=raw,if=floppy,index=0,snapshot=on" % p["floppy"],
                              "-drive", "file=%s,format=raw,if=ide,index=0" % hd, "-cdrom", iso, "-boot", "a",
                              "-display", "none", "-monitor", "tcp:127.0.0.1:%d,server,nowait" % port],
                             stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        mon = _Monitor(port)
        img = None
        for i in range(60):                                  # (to the Welcome dialog)
            time.sleep(4)
            if q.poll() is not None:
                break
            try:
                img = mon.shot(png)
            except (OSError, ValueError):
                continue
            if _error_box(img):                              # (white like the Welcome dialog: not it)
                verdict = "FAIL(error box before Welcome)"
                break
            if _welcome(img):
                time.sleep(8)
                mon.send("sendkey ret")
                verdict, bad, until = "FAIL(timeout)", 0, time.time() + wait
                while time.time() < until:
                    time.sleep(3)
                    try:
                        img = mon.shot(png)
                    except (OSError, ValueError):
                        continue
                    if img.size[0] == 720:
                        verdict = "FAIL(dos)"
                        break
                    if _license(img):
                        verdict = "PASS"
                        break
                    bad = bad + 1 if _error_box(img) else 0
                    if bad >= 2:
                        verdict = "FAIL(error box)"
                        break
                break
            # (no keys while Setup's DOS part runs: it asks for none from a prepared disk, and an
            # Enter it did not need reached Windows and hid a fault -- USER's 1:007F)
        else:
            if img is not None:
                verdict = "FAIL(dos)" if _black(img) else "FAIL(hang)" if img.size[0] == 640 else "SLOW"
    finally:
        if q is not None:
            q.kill()
            try:
                q.wait(timeout=20)
            except subprocess.TimeoutExpired:
                pass
        runs.remove(work)                                    # (QEMU lets go of its files a moment later)
    return verdict, png
