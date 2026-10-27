"""Boot Setup with a given SUWIN.EXE (or the original), accept the license, then press Enter
through the Wizard, saving a screenshot after each step.

    python wizard_walk.py <tag> [suwin.exe path] [steps]"""
import os
import json
import sys
import shutil
import subprocess
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import tempfile
import time

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
from codebreaker.base import settings
from codebreaker.verify.gate16 import setup16 as s

HERE = os.path.dirname(os.path.abspath(__file__))
tag = sys.argv[1]
suwin = sys.argv[2] if len(sys.argv) > 2 and sys.argv[2] != "-" else None
steps = int(sys.argv[3]) if len(sys.argv) > 3 else 8
shots = os.path.join(settings.state("record16"), "walk", tag)
os.makedirs(shots, exist_ok=True)


def app_error(img):
    """The Application Error box -- white, its blue border, its grey Close button -- and not a
    white Setup billboard (the three white points alone took "More powerful" for one)"""
    return img is not None and img.size == (640, 480) and s._error_box(img) and \
        img.getpixel((94, 240)) == (0, 0, 168) and img.getpixel((320, 163)) == (0, 0, 168) and \
        img.getpixel((300, 288)) == (192, 199, 200)


def finishing(img):
    """The Wizard's "Finishing Setup" page: its blue title, the arrow at step 3 and not at step 2."""
    return img is not None and img.size == (640, 480) and img.getpixel((320, 73)) == (0, 0, 168) and \
        all(img.getpixel(p) == (0, 0, 0) for p in ((218, 264), (218, 268), (222, 268), (218, 272))) and \
        not any(img.getpixel((x, y)) == (0, 0, 0) for x in range(216, 224) for y in range(244, 254))


def log(*a):
    print(tag, *a, flush=True)


p = s._paths()
work = tempfile.mkdtemp(prefix="walk_", dir=settings.state("phase3"))
q = None
try:
    iso = os.path.join(work, "cd.iso")
    s.make_cd(iso, (json.load(open(suwin)) if suwin.endswith(".json") else {"SUWIN.EXE": suwin}) if suwin else {})
    hd = os.path.join(work, "hd.img")
    shutil.copy(p["hd"], hd)
    port = s._free_port()
    q = subprocess.Popen([p["qemu"], "-M", "pc", "-m", "64",
                          "-drive", "file=%s,format=raw,if=floppy,index=0,snapshot=on" % p["floppy"],
                          "-drive", "file=%s,format=raw,if=ide,index=0" % hd, "-cdrom", iso, "-boot", "order=c,once=a",   # (the floppy once; a restart starts the disk)
                          
                          "-display", "none", "-monitor", "tcp:127.0.0.1:%d,server,nowait" % port] +
                         (["-gdb", "tcp:127.0.0.1:%s" % os.environ["COPY_WALK_GDB"]] if os.environ.get("COPY_WALK_GDB") else []),
                         stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    mon = s._Monitor(port)
    if os.environ.get("COPY_WALK_GDB"):
        # (a recorder holds the machine stopped at times: a key sent then was lost or ran on
        # -- the product key typed garbled, 2026-10-22. Keys go only while it lets the machine
        # run: "busy" asks, "busy.ack" is its answer, and the keys wait for it)
        _send = mon.send

        def _held_send(*cmds):
            flag = os.path.join(shots, "busy")
            open(flag, "w").close()
            until = time.time() + 60
            while not os.path.exists(flag + ".ack") and time.time() < until:
                time.sleep(0.2)
            try:
                _send(*cmds)
                time.sleep(1.5)
            finally:
                os.remove(flag)
                while os.path.exists(flag + ".ack") and time.time() < until + 30:
                    time.sleep(0.2)
        mon.send = _held_send
    tmp = os.path.join(shots, "_cur.png")

    def wait_for(test, secs):
        until = time.time() + secs
        while time.time() < until:
            time.sleep(3)
            try:
                img = mon.shot(tmp)
            except (OSError, ValueError):
                continue
            if test(img):
                return img
        return None

    if not wait_for(s._welcome, 240):
        log("no Welcome")
        raise SystemExit(1)
    time.sleep(8)
    mon.send("sendkey ret")
    if not wait_for(s._license, 420):
        log("no License")
        raise SystemExit(1)
    shutil.copy(tmp, os.path.join(shots, "00_license.png"))
    time.sleep(3)
    mon.send("sendkey alt-y")
    filmed, k_now, copied, out = [0], [0], [False], [False]

    def settled(png, most=1800):
        # the screen the same for 9 seconds (but for the blinking caret): the page waits for a key
        from PIL import ImageChops
        last, still, until = None, 0, time.time() + most
        time.sleep(4)
        while time.time() < until:
            try:
                img = mon.shot(png)
            except (OSError, ValueError):
                time.sleep(3)
                continue
            bb = ImageChops.difference(last, img).getbbox() if last is not None and last.size == img.size else (0, 0, 99, 99)
            still = still + 1 if bb is None or (bb[2] - bb[0] <= 2 and bb[3] - bb[1] <= 16) or                 (img.size == (720, 400) and bb[3] - bb[1] <= 3) else 0      # (and text mode's underline cursor)
            grey = (192, 199, 200)
            box = [img.getpixel((x, y)) for x in range(520, 636, 4) for y in range(456, 479, 2)] if img.size == (640, 480) else []
            wizard = img.size == (640, 480) and img.getpixel((320, 73)) == (0, 0, 168)
            if box and not wizard and not out[0] and sum(1 for p in box if p == grey) > len(box) // 2:
                copied[0] = True
                still = 0                                    # (files still copying: the strip at the bottom)
            if img.size == (640, 480) and img.getpixel((320, 73)) == (0, 0, 168) and                     not any(img.getpixel((x, y)) == (0, 0, 0) for x in range(420, 465) for y in range(386, 400)):
                still = 0                                    # (a Wizard page still working: its Next greyed --
                                                             # Enter there is Cancel)
            img.save(os.path.join(shots, "now.png"))         # (the screen as it is now -- every look, with
            with open(os.path.join(shots, "now.txt"), "w") as f:   # when: never a picture left from before)
                f.write("%s step %d look %d\n" % (time.strftime("%H:%M:%S"), k_now[0], filmed[0] + 1))
            filmed[0] += 1                                   # (every look kept, to see what happened)
            if filmed[0] % 3 == 0:
                img.save(os.path.join(shots, "film_%04d.png" % filmed[0]))
            if still >= 3 or app_error(img):
                return img
            last = img
            time.sleep(3)
        return None

    def typed(text):
        keys = {"	": "tab", " ": "spc", "-": "minus"}
        mon.send(*["sendkey " + keys.get(ch, ch) for ch in text])
    if "W95KEY" not in os.environ:                   # (the key, from settings.json: a run started from a
        from codebreaker.base import settings as _s
        os.environ["W95KEY"] = _s.get("w95_key") or ""   # (settings.json, local)
    key = os.environ["W95KEY"].split("-OEM-")
    key = [key[0]] + key[1].split("-")
    verdict = None
    for k in range(1, steps + 1):
        k_now[0] = k
        img = settled(os.path.join(shots, "%02d.png" % k), most=240 if k > 9 else 600)
        if app_error(img):
            verdict = "BAD (error box at step %d)" % k
            break
        if k > 9:
            # (a pass is copying going on: blue in the progress bar -- not merely no crash box:
            # a "Generic installer Error" dialog stops copying at 0% with no Application Error)
            last = img if img is not None else mon.shot(os.path.join(shots, "%02d.png" % k))
            bar = [last.getpixel((x, 463)) for x in range(160, 500, 4)] if last.size == (640, 480) else []
            blue = sum(1 for p in bar if p == (0, 0, 168))
            verdict = ("GOOD (copying, the bar %d/85 blue, step %d)" % (blue, k)) if blue >= 2 and img is None                 else "BAD (copying stopped: %s, step %d)" % ("a dialog" if img is not None else "no progress", k)
            if finishing(last):                               # (last: the screen as it is, settled or not --
                                                             # a strip still drawing at the bottom kept the
                                                             # page "unsettled", and its pass read as BAD, cpe2_0)
                # (copying done already -- Hyper-V copies in minutes: the Wizard's last page,
                # "Finishing Setup", the arrow at its third step. Read as "a dialog" it was a
                # false BAD, 2026-10-22, cpc1_0)
                verdict = "GOOD (copying finished: Finishing Setup, step %d)" % k
            break     # (a page shown, not one still working)
        log("step", k)
        if k == 4:                                           # Certificate of Authenticity
            typed("".join(key))                     # (each field moves on by itself when full)
            time.sleep(2)
        elif k == 5:                                         # User Information: a name
            typed("logan")
            time.sleep(2)
        if k == 8:                                           # Startup Disk: "No" (it would be written
            mon.send("sendkey down")                         # on the floppy the machine starts from)
            time.sleep(1)
        if copied[0] and not out[0]:                         # (the files copied: out with the floppy,
            mon.send("eject -f floppy0")                     # as Setup asks, so the restart starts Windows)
            out[0] = True
            log("floppy ejected")
            time.sleep(2)
        mon.send("sendkey ret")

    log("VERDICT", verdict)
    linger = int(os.environ.get("COPY_WALK_LINGER", "0"))
    if linger:
        # (a recorder slows Setup: the machine kept running past the verdict, for it -- a look
        # every 20 seconds kept, with when)
        until = time.time() + linger
        while time.time() < until:
            time.sleep(20)
            try:
                img = mon.shot(os.path.join(shots, "now.png"))
                with open(os.path.join(shots, "now.txt"), "w") as f:
                    f.write("%s lingering\n" % time.strftime("%H:%M:%S"))
                if app_error(img):
                    shutil.copy(os.path.join(shots, "now.png"), os.path.join(shots, "linger_error.png"))
                    log("an error box while lingering")
                    break
            except (OSError, ValueError):
                pass
finally:
    if q is not None:
        q.kill()
        try:
            q.wait(timeout=20)
        except subprocess.TimeoutExpired:
            pass
    try:                                                     # (the installed disk kept, to compare)
        shutil.move(os.path.join(work, "hd.img"), os.path.join(shots, "hd.img"))
    except OSError:
        pass
    for _ in range(20):
        shutil.rmtree(work, ignore_errors=True)
        if not os.path.exists(work):
            break
        time.sleep(0.5)
