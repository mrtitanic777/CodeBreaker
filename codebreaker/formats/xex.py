"""xex.py -- an Xbox 360 executable (.xex): the PowerPC image inside it, and what its header says.

    from codebreaker.formats import xex
    xex.is_xex(path)               the file starts with XEX2
    xex.optional_headers(data)     (entry's offset, key, value) of each entry of the header's table:
                                   the one reader of it (xex_edit, xex_imports, god, fatx's carver)
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
import struct
import subprocess
import sys

from codebreaker.formats import native


# ---- the C++ reader (native/formats/xex): its Python is gone (docs/NATIVE.md, step 4) ----------
def _headers_result(answer, data, *_):
    entries, error = answer

    def entries_then_error():
        yield from entries
        if error is not None:
            raise native._exception(*error)
    return entries_then_error()


def _header_result(answer, data, path):
    h = Header()
    h.name, h.base, h.entry = answer["name"], answer["base"], answer["entry"]
    h.imports, h.libraries = answer["imports"], [tuple(lib) for lib in answer["libraries"]]
    return h

MAGIC = b"XEX2"
FILE_FORMAT_INFO = 0x000003FF


@native.answered("xex.is_xex", data_arg=None)
def is_xex(path):
    """Whether the file starts with XEX2."""


@native.answered("xex.optional_headers", result=_headers_result)
def optional_headers(data):
    """(the entry's offset, key, value) of each entry of the header's optional-header table, in order (read
    as asked: a reader that stops at the key it wants reads no further). A value is the data itself for a
    key whose low byte is 0 or 1, else the offset of the data in the header."""


@native.answered("xex.ids")
def ids(data):
    """(media id, title id) from the header's execution info, or (0, 0) for a header without one (or what is
    not an XEX2 header)"""


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


def basic(path, out):
    """Write `path` as an unencrypted XEX with basic compression: every header byte kept (optional
    headers, security info, imports, exports, resources), the file format info replaced by one
    block holding the whole image. XenonRecomp and the Xbox 360 runtime read only this form. The
    security info's hashes no longer match; nothing that reads it checks them."""
    with open(path, "rb") as f:
        data = f.read()
    if data[:4] != MAGIC:
        raise ValueError("%s: not an XEX2 module" % path)
    pe_offset, = struct.unpack(">I", data[8:12])
    info = None
    for _at, key, value in optional_headers(data):
        if key == FILE_FORMAT_INFO:
            info = value
    if info is None:
        raise ValueError("%s: no file format info header" % path)
    info_size, = struct.unpack(">I", data[info:info + 4])
    if info_size < 16:
        raise ValueError("%s: file format info too small for one block" % path)
    with open(basefile(path), "rb") as f:
        image = f.read()
    header = bytearray(data[:pe_offset])
    # one block, all of the image as data, no zero run (the block count comes from info_size)
    struct.pack_into(">IHHII", header, info, 16, 0, 1, len(image), 0)
    with open(out, "wb") as f:
        f.write(bytes(header) + image)
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


@native.answered("xex.header", data_arg=None, result=_header_result)
def header(path):
    """What the .xex header states, read from the native program's listing."""


def run(argv):
    """`codebreaker xex <command> <file.xex> [args]`: info, list, extract, resources, xml, idc,
    compress, decompress, binary, encrypt, decrypt, machine, remove-limits, bounding-path, patch,
    selftest (`codebreaker xex` alone lists them)."""
    return subprocess.run([program()] + list(argv)).returncode


def main(argv):
    return run(argv)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
