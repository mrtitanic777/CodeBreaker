"""sources.py -- where each kind of the knowledge base comes from, and how it is loaded.

    AUTHORED    files people write and keep in the repository: each is the knowledge, the rows
                in the base its copy, loaded again when the file's hash changes
    authored(platform, kind)      the kind's rows, loaded first when the file changed

The derived and learned kinds are made by the modules that know how (symbols/prototypes.py reads
the headers, symbols/api_arguments.py an export's code, ...): they keep what they make here
through store.ensure() and store.put(), and nothing of it is kept anywhere else.
"""
import hashlib
import json
import os

from codebreaker.knowledge import store

_HERE = os.path.dirname(os.path.abspath(__file__))
_SYMBOLS = os.path.join(os.path.dirname(_HERE), "symbols")
_MODELS = os.path.join(_HERE, "models")


def _json(path):
    with open(path, encoding="utf-8") as f:
        return json.load(f)


def _signatures(bits):
    def make(path):
        data = _json(path)
        labels = data.get("libraries") or []
        return {"%06d" % i: [lead, size, mask, digest, name, labels[lib]]
                for i, (lead, size, mask, digest, name, lib) in enumerate(data.get(bits) or ())}
    return make


def _part(name, keyed=lambda k: k):
    def make(path):
        return {keyed(k): v for k, v in (_json(path).get(name) or {}).items()}
    return make


def _whole(path):
    return dict(_json(path))


# (platform, kind) -> (file, how its rows are read from it, what it is)
AUTHORED = {
    ("windows", "ordinal_names"): (os.path.join(_SYMBOLS, "ordinal_names.json"), _whole,
                                   "names of numbered exports from their makers' definition files and import "
                                   "libraries: {module: {from, names: {ordinal: name}}}"),
    ("win32", "argument_bytes.header"): (os.path.join(_SYMBOLS, "win32_arguments.json"), _whole,
                                         "bytes of arguments as the Win32 headers declare them (tools/make_api_table.py)"),
    ("win32", "signature"): (os.path.join(_SYMBOLS, "runtime_signatures.json"), _signatures("32"),
                             "32-bit runtime-library functions: [leading bytes, size, mask, digest, name, library]"),
    ("win16", "signature"): (os.path.join(_SYMBOLS, "runtime_signatures.json"), _signatures("16"),
                             "16-bit runtime-library functions: [leading bytes, size, mask, digest, name, library]"),
    ("vxd", "device"): (os.path.join(_SYMBOLS, "driver_services.json"), _part("devices"),
                        "device numbers the driver kit names"),
    ("vxd", "service_table"): (os.path.join(_SYMBOLS, "driver_services.json"), _part("services"),
                               "each device's services in order (service N is the Nth)"),
    ("vxd", "driver"): (os.path.join(_SYMBOLS, "driver_services.json"), _part("drivers"),
                        "device numbers corpus drivers give themselves"),
    ("vxd", "message"): (os.path.join(_SYMBOLS, "driver_services.json"), _part("messages"),
                         "messages a driver's control procedure is sent"),
    ("vxd", "api_model"): (os.path.join(_MODELS, "vxd.json"), _part("services"),
                           "what a service does that the emulator's stand-in must do too"),
    ("dos", "interrupt"): (os.path.join(_SYMBOLS, "dos_interrupts.json"), _whole,
                           "DOS and BIOS interrupt functions: names and arguments"),
    ("xbox360", "ordinal_names"): (os.path.join(_SYMBOLS, "console_ordinals.json"), _whole,
                                   "numbered exports of the console's system libraries"),
    ("win16", "api_model"): (os.path.join(_MODELS, "win16.json"), _part("functions"),
                             "what the emulator's stand-in for a Windows function returns and removes"),
    ("win32", "api_model"): (os.path.join(_MODELS, "win32.json"), _part("functions"),
                             "documented argument counts for the emulator's stand-in of a common import"),
    ("win16", "kernel_ordinal"):(os.path.join(_MODELS, "win16.json"), _part("kernel_ordinals"),
                                  "KERNEL's numbers for the functions the stand-ins model"),
}


def file_stamp(path):
    try:
        with open(path, "rb") as f:
            return hashlib.sha1(f.read()).hexdigest()
    except OSError:
        return "missing"


_stamps = {}


def changed(path):
    """A file of AUTHORED was written by this process: its kinds are loaded again at the next ask."""
    path = os.path.normcase(os.path.abspath(path))
    from codebreaker.knowledge import store
    kb = store.open()
    for (p, k), (f, _read, _what) in AUTHORED.items():
        if os.path.normcase(f) == path:
            _stamps.pop(f, None)
            kb._ensured.pop((p, k), None)


def authored(platform, kind, kb=None):
    """{key: value} of an authored kind, loaded from its file first when the file changed."""
    path, read, _what = AUTHORED[(platform, kind)]
    kb = kb or store.open()
    stamp = _stamps.get(path)
    if stamp is None:
        stamp = _stamps[path] = file_stamp(path)

    def make():
        try:
            return read(path)
        except (OSError, ValueError):
            return {}
    return kb.ensure(platform, kind, stamp, make, os.path.relpath(path, os.path.dirname(os.path.dirname(_HERE))))
