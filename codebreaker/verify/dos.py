"""dos.py -- what is particular to reconstructing a real-mode program.

One of the kinds reconstruct.py works with (see there for what a kind provides). The program's
source is assembly written from its image (source/dos_writer.py); it is assembled and linked,
and the result is compared with the original as a whole: every byte of the load module, the
relocation table, the entry point and the stack.

Nothing is run. A program that comes back identical IS the original, so each of its functions
does what the original's does -- a stronger statement than any number of runs would give, and
the verdict says that this is how it was reached. A program that does not come back identical
leaves all of its functions unverified, with where it first differs.
"""
import hashlib
import os

KIND = "dos"
REV = 91
SAME = "the program built from this source is the original, byte for byte"


def profile(module_path=None):
    from codebreaker.build import driver as builder
    tools = []
    for name in ("wasm", "wlink"):
        try:
            st = os.stat(builder.tool(name))
            tools.append("%s:%d:%d" % (name, st.st_size, int(st.st_mtime)))
        except OSError:
            tools.append("")
    settings = repr(("dos", REV, tools))
    return hashlib.sha1(settings.encode()).hexdigest()[:12], settings


class Prepared(object):
    pass


def prepare(module_path, work):
    from codebreaker.source import dos_writer
    m = Prepared()
    m.written = r = dos_writer.reconstruct(module_path, os.path.join(work, "source"))
    text = ""
    if r.get("source") and os.path.isfile(r["source"]):
        with open(r["source"], encoding="latin1") as f:
            text = f.read()
    functions = r.get("functions") or {}
    m.header = text
    m.order = [functions[a].name for a in sorted(functions)]
    m.key = {functions[a].name: "%05x" % a for a in functions} if text else {}
    m.all_keys = sorted("%05x" % a for a in functions)
    m.texts = {n: "; %s, in %s" % (n, os.path.basename(r["source"])) for n in m.order} if text else {}
    m.sig = hashlib.sha1(("%d\0%s" % (REV, text)).encode("latin1", "replace")).hexdigest()
    return m


def build(m, module_path, work):
    """-> (context for judge, {name: item}, {name: why it is not in the build})"""
    r = m.written
    if not r.get("linked"):
        why = (r.get("notes") or ["did not build"])[-1][:200]
        return (False, why), {}, {n: why for n in m.order}
    why = "" if r["same"] else (r.get("notes") or ["the built program differs from the original"])[-1][:200]
    return (r["same"], why), {n: (m.key[n],) for n in m.order}, {}


def judge(context, item):
    same, why = context
    if same:
        return "equivalent", "", [], SAME
    return "divergent", "image-differs", [], why
