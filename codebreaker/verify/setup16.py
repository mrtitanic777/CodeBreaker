"""Does Windows 95 Setup run with these modules? The real system's test of a rebuilt module.

Setup's own CD is copied with WIN95\\MINI.CAB rebuilt -- the original's files, with the ones
given in their place -- and booted in QEMU from a disk Setup has already prepared and a floppy
that starts `SETUP /IS`. At the Welcome dialog Enter is pressed (Continue); Setup then shows
its License Agreement page. That page on the screen is a pass; Windows back at the DOS prompt,
an error box, or Windows' screen with nothing on it is a fail.

Everything a run copies (the CD, the disk -- 650 MB) is made in a folder of its own and gone
when the run ends; only the last screen is kept, for whoever looks at a failure.

    continue_test({"USER.EXE": path, ...}, tag) -> (verdict, screenshot)
      verdict: "PASS" | "FAIL(dos)" | "FAIL(hang)" | "FAIL(error box)" | "FAIL(timeout)"
               | "FAIL(error box before Welcome)"
               | "SLOW" (Setup's DOS part never finished: the run says nothing)"""
import io
import os
import shutil
import socket
import subprocess
import tempfile
import time

from codebreaker.base import settings

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
    iso.write(out)
    iso.close()


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


def _license(img):
    return img.size[0] == 640 and all(img.getpixel(xy) == LICENSE_GREY for xy in LICENSE_POINTS)


def _error_box(img):
    return img.size[0] == 640 and all(sum(img.getpixel(xy)) > 700 for xy in ((97, 310), (320, 175), (540, 310)))


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
    shots = keep_shot or settings.state("phase3", "shots")
    os.makedirs(shots, exist_ok=True)
    png = os.path.join(shots, tag + ".png")
    work = tempfile.mkdtemp(prefix="setup16_", dir=settings.state("phase3"))
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
        for _ in range(20):                                  # (QEMU lets go of its files a moment later)
            shutil.rmtree(work, ignore_errors=True)
            if not os.path.exists(work):
                break
            time.sleep(0.5)
    return verdict, png
