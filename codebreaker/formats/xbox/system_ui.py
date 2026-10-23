"""system_ui.py -- the system UI's assets the Xbox 360 runtime draws the Guide and its popups from,
taken from the owner's own console system update.

    python -m codebreaker system-ui <system update package> <out folder> [<online modules folder>]

A system update (su20076000_00000000, dashboard 2.0.17559) is an STFS package of the console's
flash files; its system modules carry XUI packages (XUIZ) as XEX resources. Written: every XUI
package of the modules the Guide uses, as <module>.<resource>.xzp, and the system fonts -- the
folder native/x360's xui::Assets loads (system/xui/xui/assets.h).

<online modules folder> (optional): the Guide's LIVE-delivered modules as a console keeps them in
its SystemAux partition (online/<version>/Guide.Friends.xex ...; no system update carries them);
their XUI packages are written too. Modules missing there are skipped.

These are Microsoft's files: the output folder is the owner's and is never committed or distributed.
"""
import os
import shutil
import subprocess
import sys
import tempfile

from codebreaker.formats import xex
from codebreaker.formats.xbox import stfs

MODULES = ["xam", "huduiskin", "hud", "signin", "vk", "gamerprofile", "createprofile", "deviceselector"]
ONLINE_MODULES = ["Guide.Friends", "Guide.ChatAndMessenger", "Guide.Voicemail"]
FONTS = {"SegoeXbox-Light.xtt": "SegoeXbox-Light.xtt", "xenonclatin.xttp": "xenonclatin.xtt",
         "xenonjklatin.xttp": "xenonjklatin.xtt"}


def _xui_packages(module_bytes):
    """{resource name: bytes} of the XUI packages a module's XEX carries."""
    work = tempfile.mkdtemp(prefix="cb_sysui_")
    try:
        path = os.path.join(work, "module.xex")
        with open(path, "wb") as f:
            f.write(module_bytes)
        out = os.path.join(work, "res")
        os.makedirs(out)
        subprocess.run([xex.program(), "resources", path, out], capture_output=True)
        found = {}
        for name in os.listdir(out):
            with open(os.path.join(out, name), "rb") as f:
                blob = f.read()
            if blob[:4] == b"XUIZ":
                found[name] = blob
        return found
    finally:
        shutil.rmtree(work, ignore_errors=True)


def extract(package, out_dir, online=None, report=print):
    os.makedirs(out_dir, exist_ok=True)
    with open(package, "rb") as f:
        pkg = stfs.STFS(f.read())
    files = {name.rsplit("/", 1)[-1]: name for name in pkg.files}
    written = 0
    for module in MODULES:
        name = files.get("$flash_%s.xex" % module)
        if not name:
            raise ValueError("%s: no $flash_%s.xex (not a system update?)" % (package, module))
        for res, blob in sorted(_xui_packages(pkg.read_file(name)).items()):
            target = os.path.join(out_dir, "%s.%s.xzp" % (module, res))
            with open(target, "wb") as f:
                f.write(blob)
            report("%-40s %8d" % (os.path.basename(target), len(blob)))
            written += 1
    for source, target in FONTS.items():
        name = files.get("$flash_" + source)
        if not name:
            raise ValueError("%s: no $flash_%s" % (package, source))
        data = pkg.read_file(name)
        with open(os.path.join(out_dir, target), "wb") as f:
            f.write(data)
        report("%-40s %8d" % (target, len(data)))
        written += 1
    if online:
        found = {}
        for root, _dirs, names in os.walk(online):
            for n in names:
                found.setdefault(n.lower(), os.path.join(root, n))
        for module in ONLINE_MODULES:
            path = found.get(module.lower() + ".xex")
            if not path:
                report("%-40s (not in %s)" % (module + ".xex", online))
                continue
            with open(path, "rb") as f:
                packages = _xui_packages(f.read())
            for res, blob in sorted(packages.items()):
                target = os.path.join(out_dir, "%s.%s.xzp" % (module.lower(), res))
                with open(target, "wb") as f:
                    f.write(blob)
                report("%-40s %8d" % (os.path.basename(target), len(blob)))
                written += 1
    return written


def main(argv):
    if len(argv) < 2:
        print(__doc__.split("\n\n")[0] + "\n\n" + __doc__.split("\n\n")[1])
        return 2
    n = extract(argv[0], argv[1], argv[2] if len(argv) > 2 else None)
    print("%d files -> %s" % (n, argv[1]))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
