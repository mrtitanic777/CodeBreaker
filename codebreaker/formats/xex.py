"""xex.py -- an Xbox 360 executable (.xex): the PowerPC image inside it, and what its header says.

    from codebreaker.formats import xex
    xex.is_xex(path)               the file starts with XEX2
    xex.basefile(path)             the decrypted, decompressed PowerPC image (a PE, machine 0x1F2),
                                   written once under state/unpacked/xex/ and reused, with the
                                   header beside it (<image>.json)
    xex.import_libraries(image)    the libraries that header named, for an image written so
    h = xex.header(path)
    h.name                         the image's own name (bootanim.dll, default.exe)
    h.base, h.entry                its load address and entry point
    h.imports                      [library name] in the order the image's import stubs number them
    h.libraries                    [(static library, version)]: what it was built with
    run(args)                      `codebreaker xex <command> ...`: the native program's commands

A .xex wraps a PE image in its own header: the image is compressed (LZX or by blocks), usually
encrypted with a per-title key, and the libraries its imports come from are named in the header,
not in the image. The native program (native/xex, built with CMake) does the decrypting and
decompressing; the PowerPC lane reads the basefile it writes, so a .xex is reconstructed like any
PowerPC image (corpus.resolve hands every stage the basefile in its place).
"""
import hashlib
import json
import os
import re
import subprocess
import sys

MAGIC = b"XEX2"


def is_xex(path):
    try:
        with open(path, "rb") as f:
            return f.read(4) == MAGIC
    except OSError:
        return False


def program():
    from codebreaker.base import settings
    exe = os.path.join(settings.get("xex_bin"), "xex.exe")
    if not os.path.isfile(exe):
        raise RuntimeError("the xex program is not built: cmake -S native/xex -B native/xex/build "
                           "&& cmake --build native/xex/build --config Release --target xex")
    return exe


def _sha(path):
    h = hashlib.sha1()
    with open(path, "rb") as f:
        for block in iter(lambda: f.read(1 << 20), b""):
            h.update(block)
    return h.hexdigest()


def basefile(path):
    """The PowerPC image inside `path`, extracted once per distinct file (named by its contents,
    so a changed .xex is a new basefile and nothing is stale)."""
    from codebreaker.base import settings
    stem = os.path.splitext(os.path.basename(path))[0].lower()
    out = settings.state("unpacked", "xex", "%s-%s.exe" % (stem, _sha(path)[:12]))
    if not os.path.isfile(out):
        tmp = out + ".part"
        r = subprocess.run([program(), "extract", path, tmp], capture_output=True, text=True)
        if r.returncode != 0 or not os.path.isfile(tmp):
            raise ValueError("%s: no basefile (%s)" % (path, (r.stderr or r.stdout).strip()))
        h = header(path)
        with open(out + ".json", "w", encoding="utf-8") as f:
            json.dump({"xex": os.path.abspath(path), "name": h.name, "base": h.base, "entry": h.entry,
                       "imports": h.imports, "libraries": h.libraries}, f, indent=1)
        os.replace(tmp, out)
    return out


def import_libraries(image_path):
    """[library name] for a basefile written by basefile() -- the .xex header named them -- or
    None for an image that did not come from a .xex."""
    try:
        with open(image_path + ".json", encoding="utf-8") as f:
            return json.load(f)["imports"]
    except (OSError, ValueError, KeyError):
        return None


class Header:
    def __init__(self):
        self.name = None
        self.base = self.entry = None
        self.imports = []
        self.libraries = []


def header(path):
    """What the .xex header states, read from the native program's listing."""
    r = subprocess.run([program(), "list", path], capture_output=True, text=True)
    if r.returncode != 0:
        raise ValueError("%s: %s" % (path, (r.stderr or r.stdout).strip()))
    h, section = Header(), None
    for line in r.stdout.splitlines():
        if line and not line.startswith(" "):
            section = line.strip()
            continue
        m = re.match(r"\s+(Original PE Name|Load Address|Entry Point):\s+(\S+)", line)
        if m:
            key, value = m.groups()
            if key == "Original PE Name":
                h.name = value
            elif key == "Load Address":
                h.base = int(value, 16)
            else:
                h.entry = int(value, 16)
            continue
        m = re.match(r"\s+\d+\)\s+(\S+)\s+v(\S+)", line)
        if m and section == "Import Libraries":
            h.imports.append(m.group(1))
        elif m and section == "Static Libraries":
            h.libraries.append((m.group(1), m.group(2)))
    return h


def run(argv):
    """`codebreaker xex <command> <file.xex> [args]`: info, list, extract, resources, xml, idc,
    compress, decompress, binary, encrypt, decrypt, machine, remove-limits, bounding-path, patch,
    selftest (`codebreaker xex` alone lists them)."""
    return subprocess.run([program()] + list(argv)).returncode


def main(argv):
    return run(argv)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
