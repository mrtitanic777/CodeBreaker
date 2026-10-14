"""classes_writer.py -- the C++ classes of a module, written as class declarations.

    classes_writer.write(module_path, kind, out_dir, stem, name_at, status_at)
        -> the file written (<stem>_classes.hpp), or None when the module shows no class

A compiled C++ program keeps little of its classes, but not nothing: every class with a virtual
function has a table of them, the code that installs a table in an object is the class's
constructor or destructor, and MFC programs and programs built with run-time type information
carry the classes' names and bases outright (analyze/classes.py finds all of this, and says for
each fact where it came from). This writes what was found as C++:

    class CMainFrame : public CFrameWnd {      // 92 virtual functions, object of 0x1a8 bytes
    public:
        virtual int4 vf_0();                    // GetRuntimeClass    [VERIFIED]
        virtual int4 vf_1();                    // sub_004A10         [UNVERIFIED]
        ...
    };

Each virtual function is tied to the reconstructed function that implements it (in <stem>.c)
and carries that function's verdict. The file is a description in C++ syntax, to read beside the
C: it is not compiled, the functions' real parameter lists are in the C, and a name the binary
does not state is a placeholder (`class_<address of its table>`, `vf_<slot>`). What is inferred
rather than stated is marked "probably".

32-bit Windows modules get everything analyze/classes.py finds. A PowerPC image gets its virtual
tables (tables, in its data, of addresses of its own functions): the image carries no names.
"""
import os
import struct


def _powerpc_tables(path):
    """[{vtable, methods}] -- runs of two or more words in a PowerPC image's data that are each
    the address of one of its functions."""
    from codebreaker.formats import powerpc_image
    img = powerpc_image.read(path)
    starts = set(img.functions)
    code = img.code or (0, 0)
    # The addresses kept in an extracted image's data are for the base it was linked at, which
    # need not be where it is read to here. The table of function entries (.pdata) says by how
    # much they differ: its first word is the first function's address, as linked.
    delta = 0
    for name, va, n, _flags in img.sections:
        if name == ".pdata" and n >= 8 and starts:
            raw = img.bytes_at(va, n)
            linked = sorted(set(struct.unpack_from(">I", raw, 8 * i)[0] for i in range(len(raw) // 8)) - {0})
            best = (0, 0)
            for s in sorted(starts)[:8]:                     # which of the first few is the table's first
                for w in linked[:8]:
                    d = (s - w) & 0xFFFFFFFF
                    hits = sum(1 for x in linked if ((x + d) & 0xFFFFFFFF) in starts)
                    best = max(best, (hits, d))
            if best[0] * 2 >= len(linked):
                delta = best[1]
    starts = {(s - delta) & 0xFFFFFFFF for s in starts}
    out = []
    for name, va, n, _flags in img.sections:
        if n <= 0 or (code[0] <= va < code[1]) or name in (".pdata", ".reloc", ".edata", ".idata"):
            continue
        data = img.bytes_at(va, n)
        words = struct.unpack_from(">%dI" % (len(data) // 4), data, 0) if len(data) >= 8 else ()
        run = []
        for i, w in enumerate(list(words) + [0]):
            if w in starts:
                run.append(w)
                continue
            if len(run) >= 2:
                out.append({"vtable": va + 4 * (i - len(run)), "methods": [(w + delta) & 0xFFFFFFFF for w in run],
                            "name": "class_%x" % (va + 4 * (i - len(run))),
                            "bases": [], "evidence": ["a table of %d of the image's own functions, in %s: a virtual table, or another table of "
                                         "functions (callbacks) -- the image does not say which" % (len(run), name)],
                            "size": None, "installed_by": []})
            run = []
    return out


_IMPORTS = {}


def import_behind(path, address):
    """The import a 32-bit module's function is nothing but a jump to (`jmp [slot]`), as
    LIBRARY.name or LIBRARY.number -- or None. A class whose functions are another library's
    (MFC's) has such jumps in its table."""
    try:
        from codebreaker.formats.pe import open_pe
        from codebreaker.symbols import ordinals
        if path not in _IMPORTS:
            pe = open_pe(path)
            _IMPORTS[path] = (pe, dict(pe.imports()))
        pe, slots = _IMPORTS[path]
        off = pe.file_offset(address - pe.image_base)
        if off is None or pe.data[off:off + 2] != bytes([0xFF, 0x25]):
            return None
        dll, name = slots.get(struct.unpack_from("<I", pe.data, off + 2)[0], (None, None))
        if not dll:
            return None
        stem = os.path.splitext(dll)[0].upper()
        if name and name.startswith("#") and name[1:].isdigit():
            return "%s.%s" % (stem, ordinals.identifier(stem, int(name[1:])) or name[1:])
        return "%s.%s" % (stem, name)
    except Exception:
        return None


def found(path, kind):
    """The classes of a module, as analyze/classes.py's hierarchy gives them (or the PowerPC tables)."""
    if kind == "powerpc":
        return _powerpc_tables(path)
    if kind != "pe":
        return []
    from codebreaker.analyze import classes, image
    return classes.hierarchy(image.load(path))


def write(path, kind, out_dir, stem, name_at, status_at):
    """`name_at(address)` -> the name a function has in the written source; `status_at(address)`
    -> its verdict's title, or "". -> the path written, or None."""
    try:
        classes = found(path, kind)
    except Exception:
        return None
    if not classes:
        return None
    names = {c["vtable"]: c["name"] for c in classes}
    lines = ["// %s -- the C++ classes this module shows, as class declarations." % os.path.basename(path), "//",
             "// Read beside %s.c. Not compiled: each virtual function is the reconstructed function named" % stem,
             "// in its comment, where its real parameters are. A class or function name the binary does",
             "// not state is a placeholder (class_<address of its table>, vf_<slot>).", "//",
             "// %d classes; %d named by the binary itself." % (
                 len(classes), sum(1 for c in classes if not c["name"].startswith("class_"))), ""]
    for c in classes:
        # (a base known only as a numbered import of another library is written as that)
        bases = [b if b.replace("_", "a").isalnum() and not b[0].isdigit() else "imported_class_" + "".join(
            ch for ch in b if ch.isalnum()) for b in c.get("bases") or [] if b]
        facts = ["%d virtual functions" % len(c["methods"]), "table at %x" % c["vtable"]]
        if c.get("size"):
            facts.append("object of %#x bytes%s" % (c["size"], "" if c.get("size_exact", True) else " at least"))
        if c.get("com"):
            facts.append("has the shape of a COM interface (%s)" % c["com"])
        if c.get("message_map"):
            facts.append("message map at %x" % c["message_map"])
        lines.append("// " + "; ".join(facts))
        for e in c.get("evidence") or []:
            lines.append("//   %s" % e)
        if c.get("installed_by"):
            lines.append("//   constructed or destroyed in: %s" % ", ".join(name_at(f) for f in c["installed_by"][:8]))
        direct = bases[:1] if bases else []
        lines.append("class %s%s {" % (c["name"], " : public " + ", public ".join(direct) if direct else ""))
        if len(bases) > 1:
            lines.append("    // further up: %s" % " : ".join(bases[1:]))
        lines.append("public:")
        for slot, f in enumerate(c["methods"]):
            status = status_at(f)
            lines.append("    virtual int4 vf_%d();%s// %s%s" % (
                slot, " " * max(1, 6 - len(str(slot))), name_at(f), "  [%s]" % status if status else ""))
        lines += ["};", ""]
    out = os.path.join(out_dir, stem + "_classes.hpp")
    with open(out, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(lines))
    return out
