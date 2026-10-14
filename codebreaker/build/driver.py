"""driver.py -- build a virtual device driver from assembly source, and check it against the original.

    python -m codebreaker driver-build <source folder or .asm> [--original FILE] [--out DIR]

    from codebreaker.build import driver
    r = driver.build(r"..\\Windows95\\vmm32\\vkd", original="vkd.vxd")
    r["assembled"], r["linked"], r["path"], r["checks"], r["notes"]

The source is one .asm written with the driver kit's macros (Declare_Virtual_Device,
Begin_Service_Table, VMMcall ...). It is assembled with OpenWatcom's wasm against the kit's
include files and linked with wlink into an LE image whose one export is the device descriptor
block. Segments get the attributes the kit's own module-definition file gives each class of
segment (locked code is preloaded, initialisation code is discardable, ...); whether the driver
is loaded at start-up or on demand is taken from the original when there is one.

The check is structural -- does the rebuilt file have what the original has:

    objects      the same number of objects, each of the same size and attributes
    descriptor   the same device descriptor block (name, numbers, versions, flags, order, what
                 it points at)
    services     the same service table (as many services, the same ones left empty, each
                 starting with the same code)
    exports      the same exported names and ordinals, pointing at the descriptor block
    module       the same kind of module (start-up, on demand, old style)

and, where objects pair up, how many of their bytes are equal outside the places the loader
patches. Whether the two BEHAVE alike is verify/driver.py's question (`driver-judge`).
Everything is written under the state folder; the source folder is only read.
"""
import os
import re
import shutil
import subprocess

from codebreaker.base import corpus, settings
from codebreaker.build import watcom
from codebreaker.formats import le
from codebreaker.symbols import driver_services

# What the kit's module-definition file says of each class of segment, as wlink spells it.
SEGMENT_CLASSES = (("LCODE", "preload"), ("MCODE", "preload iopl"), ("ICODE", "discardable"),
                   ("PCODE", "loadoncall"), ("PDATA", "loadoncall shared"), ("SCODE", "resident"),
                   ("DBOCODE", "preload conforming"), ("16ICODE", "preload discardable"))
_DECLARE = re.compile(r"^\s*Declare_Virtual_Device\s+(\w+)", re.I | re.M)
_TIMEOUT = 300


def tool(name):
    """The assembler or linker to use. Drivers need the builds of wasm and wlink that
    understand the kit's macros and write the driver flavour of LE; in the OpenWatcom tree
    those are the ones in binnt. Absolute path, or '' when there is none."""
    exe = os.path.join(watcom.openwatcom_root() or "", "binnt", name + ".exe")
    return exe if os.path.isfile(exe) else watcom.ow_tool(name)


def kit_folder():
    """The kit's include files in a folder whose path has no spaces (the assembler cannot be
    given one that has): the kit's own folder if it has none, else a copy under state/."""
    src = driver_services.include_dir()
    if not src:
        return ""
    if " " not in src:
        return src
    dst = settings.state("driver_kit")
    if not os.path.isfile(os.path.join(dst, "VMM.INC")):
        os.makedirs(dst, exist_ok=True)
        for fn in os.listdir(src):
            if fn.upper().endswith(".INC"):
                shutil.copyfile(os.path.join(src, fn), os.path.join(dst, fn))
    return dst if " " not in dst else ""


def source_of(path):
    """The .asm to build: `path` itself, or in a folder the file named after the folder."""
    if os.path.isdir(path):
        name = os.path.basename(os.path.normpath(path))
        cand = os.path.join(path, name + ".asm")
        if not os.path.isfile(cand):
            raise ValueError("%s: no %s.asm in it" % (path, name))
        return cand
    return path


def original_of(source):
    """The corpus driver a source file was made from: the one with the same name, or None."""
    stem = os.path.splitext(os.path.basename(source))[0].lower()
    for path in le.corpus_files():
        if os.path.splitext(os.path.basename(path))[0].lower() == stem:
            return path
    return None


def _run(cmd, cwd):
    try:
        p = subprocess.run(cmd, cwd=cwd, env=watcom.ow_env("pe"), capture_output=True, timeout=_TIMEOUT)
        return p.returncode, (p.stdout + p.stderr).decode("latin1", "replace").strip()
    except subprocess.TimeoutExpired:
        return -1, "timed out after %d s" % _TIMEOUT
    except OSError as e:
        return -1, str(e)


def assemble(source, work):
    """-> (object file or None, the assembler's messages)"""
    wasm, kit = tool("wasm"), kit_folder()
    if not wasm or not kit:
        return None, "no assembler" if not wasm else "the driver kit's include files were not found"
    obj = os.path.join(work, os.path.splitext(os.path.basename(source))[0] + ".obj")
    if os.path.exists(obj):
        os.remove(obj)
    code, log = _run([wasm, "-zq", "-dIS_32=1", "-i=" + kit, "-fo=" + obj, os.path.abspath(source)], work)
    return (obj if code == 0 and os.path.isfile(obj) else None), log


def link(obj, device, out, dynamic=False, classes=None, named=True):
    """Link one object file into a driver exporting <device>_DDB. -> (file or None, messages)
    `classes` [(segment class, attributes)] replaces the kit's classes, for source that names
    its own (one per object of the original). `named` False exports the DDB by ordinal only,
    with no name -- which is how a member of a container (VMM in VMM32.VXD) exports it, the
    name living in the container's own directory."""
    wlink = tool("wlink")
    if not wlink:
        return None, "no linker"
    out = os.path.abspath(out)                               # the linker runs with its folder as cwd
    script = os.path.splitext(out)[0] + ".lnk"
    lines = ["format windows vxd" + (" virtdevice" if dynamic else ""), "option quiet",
             "option map=%s" % (os.path.splitext(out)[0] + ".map"), "name %s" % out,
             "export %s_DDB%s" % (device, "" if named else ".1"), "file %s" % obj]
    lines += ["segment class '%s' %s" % (cls, attrs) for cls, attrs in (classes or SEGMENT_CLASSES)]
    with open(script, "w", newline="\r\n") as f:
        f.write("\n".join(lines) + "\n")
    if os.path.exists(out):
        os.remove(out)
    code, log = _run([wlink, "@" + script], os.path.dirname(out))
    return (out if code == 0 and os.path.isfile(out) else None), log


# The structural check ------------------------------------------------------------------------
def _patched(lf):
    """{object: set of offsets the loader writes into}"""
    out = {}
    for obj, off, _kind, _to, _ta in le.references(lf):
        out.setdefault(obj, set()).update(range(off, off + 4))
    return out


def _code_at(lf, where, n=12):
    """The first bytes of code at a place, with the bytes the loader patches left out."""
    obj, off = where
    patched = _patched(lf).get(obj, ())
    data = le.object_data(lf, lf.objects[obj - 1])[off:off + n]
    return bytes(b for i, b in enumerate(data) if off + i not in patched)


def structure(lf):
    """What compare() looks at, for one image."""
    d = le.descriptor(lf)
    table = le.service_table(lf, d)
    return {"objects": [(o["vsize"], o["flags"]) for o in lf.objects],
            "descriptor": d, "services": table,
            "exports": [(n, o) for n, o, _obj, _off in le.exports(lf)],
            "export_is_descriptor": bool(d) and any((obj, off) == (d["obj"], d["offset"]) for _n, _o, obj, off in le.exports(lf)),
            "module": le.header_fields(lf)["module_flags"],
            "references": len(le.references(lf))}


def compare(built, original):
    """Check a rebuilt image against the original (two LEFile). -> (checks, notes, same):
    checks {name: True/False} for objects, descriptor, services, exports, module; notes, one
    line for each that failed; same = share of bytes equal outside patched places, over the
    objects that pair up by number and size (None when none do)."""
    a, b = structure(built), structure(original)
    checks, notes = {}, []

    def fail(name, text):
        checks[name] = False
        notes.append("%s: %s" % (name, text))

    checks["objects"] = a["objects"] == b["objects"]
    if not checks["objects"]:
        if len(a["objects"]) != len(b["objects"]):
            fail("objects", "%d objects, the original has %d" % (len(a["objects"]), len(b["objects"])))
        else:
            bad = [i + 1 for i, (x, y) in enumerate(zip(a["objects"], b["objects"])) if x != y]
            i = bad[0] - 1
            fail("objects", "%d of %d differ; object %d is %#x bytes flags %#x, original %#x flags %#x" % (
                len(bad), len(b["objects"]), bad[0], a["objects"][i][0], a["objects"][i][1],
                b["objects"][i][0], b["objects"][i][1]))
    da, db = a["descriptor"], b["descriptor"]
    checks["descriptor"] = True
    if not da or not db:
        fail("descriptor", "none in the %s" % ("rebuilt file" if not da else "original"))
    else:
        for f in ("name", "kit_version", "device_id", "major", "minor", "flags", "init_order",
                  "service_count", "size", "v86_api_address", "pm_api_address"):
            if da[f] != db[f]:
                fail("descriptor", "%s is %r, original %r" % (f, da[f], db[f]))
                break
        else:
            for f in ("control", "v86_api", "pm_api", "service_table", "reference_data"):
                if bool(da[f]) != bool(db[f]) or (da[f] and db[f] and f != "service_table" and
                                                 f != "reference_data" and
                                                 _code_at(built, da[f]) != _code_at(original, db[f])):
                    fail("descriptor", "%s %s" % (f, "starts with different code" if da[f] and db[f] else
                                                  "is %s, original %s" % ("set" if da[f] else "empty",
                                                                          "set" if db[f] else "empty")))
                    break
    ta, tb = a["services"], b["services"]
    checks["services"] = True
    if len(ta) != len(tb):
        fail("services", "%d services, the original has %d" % (len(ta), len(tb)))
    else:
        bad = [i for i, (x, y) in enumerate(zip(ta, tb))
               if bool(x) != bool(y) or (x and y and _code_at(built, x) != _code_at(original, y))]
        if bad:
            fail("services", "%d of %d differ, first service %d" % (len(bad), len(tb), bad[0]))
    checks["exports"] = a["exports"] == b["exports"] and a["export_is_descriptor"] == b["export_is_descriptor"]
    if not checks["exports"]:
        fail("exports", "%r, original %r" % (a["exports"], b["exports"]))
    checks["module"] = a["module"] == b["module"]
    if not checks["module"]:
        fail("module", "module flags %#x, original %#x" % (a["module"], b["module"]))
    equal = total = 0
    pa, pb = _patched(built), _patched(original)
    for x, y in zip(built.objects, original.objects):
        if x["vsize"] != y["vsize"]:
            continue
        ba, bb = le.object_data(built, x), le.object_data(original, y)
        skip = pa.get(x["idx"], set()) | pb.get(y["idx"], set())
        total += len(ba) - len(skip)
        equal += sum(1 for i in range(len(ba)) if ba[i] == bb[i] and i not in skip)
    if a["references"] != b["references"]:
        notes.append("places the loader patches: %d, original %d" % (a["references"], b["references"]))
    return checks, notes, (equal / total if total else None)


def build(source, original=None, out=None):
    """Assemble and link the driver whose source is `source` (a folder or an .asm). With
    `original` (a file or a corpus name; found by name when not given) the result is checked
    against it. -> {"name", "source", "assembled", "linked", "path", "log", "original",
    "checks", "notes", "same"}"""
    source = source_of(source)
    name = os.path.splitext(os.path.basename(source))[0].lower()
    work = out or settings.state("driver_build", name)
    os.makedirs(work, exist_ok=True)
    original = corpus.retail(original) if original else original_of(source)
    r = {"name": name, "source": source, "assembled": False, "linked": False, "path": None, "log": "",
         "original": original, "checks": {}, "notes": [], "same": None}
    with open(source, encoding="latin1") as f:
        m = _DECLARE.search(f.read())
    if not m:
        r["log"] = "the source declares no device (no Declare_Virtual_Device)"
        return r
    obj, r["log"] = assemble(source, work)
    if not obj:
        return r
    r["assembled"] = True
    olf = None
    if original:
        mem = le.members(original)
        olf = next((lf for n, lf in mem if n.lower() == name), mem[0][1])
    dynamic = bool(olf) and le.header_fields(olf)["module_flags"] == le.MODULE_DYNAMIC
    ext = os.path.splitext(original)[1] if original and len(le.members(original)) == 1 else ".vxd"
    path, log = link(obj, m.group(1), os.path.join(work, name + ext), dynamic)
    r["log"] = (r["log"] + "\n" + log).strip()
    if not path:
        return r
    r["linked"], r["path"] = True, path
    if olf is not None:
        try:
            r["checks"], r["notes"], r["same"] = compare(le.LEFile.from_file(path), olf)
        except Exception as e:                            # a file the reader cannot take apart
            r["notes"] = ["the rebuilt file could not be read: %s: %s" % (type(e).__name__, e)]
    return r


def report(r):
    """One line for a build result."""
    if not r["assembled"]:
        return "%-12s does not assemble: %s" % (r["name"], (r["log"].splitlines() or ["?"])[0][:120])
    if not r["linked"]:
        return "%-12s assembles, does not link: %s" % (r["name"], (r["log"].splitlines() or ["?"])[-1][:120])
    if not r["checks"]:
        return "%-12s built (%s)" % (r["name"], r["notes"][0] if r["notes"] else "no original to check against")
    marks = " ".join("%s %s" % (k, "same" if v else "DIFFERS") for k, v in r["checks"].items())
    same = "" if r["same"] is None else "  %.1f%% of paired bytes equal" % (100 * r["same"])
    return "%-12s built  %s%s" % (r["name"], marks, same)


def main(argv):
    """python -m codebreaker driver-build <source folder or .asm> ... [--original FILE] [--out DIR]"""
    opts, args = {}, []
    it = iter(argv)
    for a in it:
        if a in ("--original", "--out"):
            opts[a[2:]] = next(it, None)
        else:
            args.append(a)
    if not args:
        print(main.__doc__)
        return 2
    bad = 0
    for src in args:
        try:
            r = build(src, opts.get("original"), opts.get("out"))
        except ValueError as e:
            print(e)
            bad += 1
            continue
        print(report(r))
        for note in r["notes"]:
            print("    " + note)
        if r["path"]:
            print("    -> " + r["path"])
        bad += not (r["linked"] and all(r["checks"].values()))
    return 1 if bad else 0
