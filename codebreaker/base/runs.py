"""runs.py -- a run's own folder, gone with the run: copies of disks and CDs never pile up.

    from codebreaker.base import runs
    with runs.folder("phase3", tag) as work:     # state/phase3/runs/<tag>_<time>_<pid>/
        ...copy the disk and the CD into work, boot them...
    shots = runs.results("phase3")               # state/phase3/shots/: one screenshot, one log per tag

A harness that boots a machine works on copies: a 650 MB hard disk, a CD image. The copy is the
run's, in a folder of its own under state/<tool>/runs/, and is deleted when the run ends, however
it ends. A run that was killed never reaches its end; so every run, as it starts, deletes the run
folders of its tool older than a day (a run still going holds its files open, and is not deleted
under it). What a run leaves for the user -- its last screenshot, its log -- goes to the tool's
one results folder, under the run's tag, the one before it overwritten.
"""
import contextlib
import os
import re
import shutil
import time

AGE = 24 * 3600                 # a run folder older than this is a killed run's


def _runs(tool):
    from codebreaker.base import settings
    return settings.state(tool, "runs", "")


def results(tool, name="shots"):
    """The tool's one folder of what its runs leave (made if it is not there)."""
    from codebreaker.base import settings
    return settings.state(tool, name, "")


def remove(path, tries=20):
    """Delete a run folder; a machine just stopped lets go of its files a moment later."""
    for _ in range(tries):
        shutil.rmtree(path, ignore_errors=True)
        if not os.path.exists(path):
            return True
        time.sleep(0.5)
    return False


def sweep(tool, age=AGE):
    """Delete the tool's run folders older than `age` seconds (the killed runs'). -> those deleted"""
    base, gone = _runs(tool), []
    now = time.time()
    for n in os.listdir(base):
        p = os.path.join(base, n)
        try:
            if os.path.isdir(p) and now - os.path.getmtime(p) > age:
                if remove(p, tries=1):
                    gone.append(p)
        except OSError:
            pass
    return gone


def start(tool, tag="run"):
    """A new run folder of the tool's (the killed runs' swept first). -> its path; the caller
    deletes it with remove() when the run ends (folder() does both)."""
    sweep(tool)
    name = "%s_%s_%d" % (re.sub(r"[^A-Za-z0-9.-]+", "-", str(tag))[:40] or "run",
                         time.strftime("%Y%m%d-%H%M%S"), os.getpid())
    path = os.path.join(_runs(tool), name)
    n = 0
    while os.path.exists(path + ("_%d" % n if n else "")):
        n += 1
    path += "_%d" % n if n else ""
    os.makedirs(path)
    return path


@contextlib.contextmanager
def folder(tool, tag="run"):
    """A run folder for the length of a `with`, deleted at its end, however it ends."""
    path = start(tool, tag)
    try:
        yield path
    finally:
        remove(path)
