#!/usr/bin/env python3
r"""emit.py -- write a reconstructed source tree (Option 3: the hybrid deliverable).

Takes an analyze() report and writes a real, per-segment C source tree: the functions
msc6dec recovered byte-exact are inlined; the ones it can't (buffer/string/far-ptr code,
and intra-segment-call functions pending the whole-DLL link) are emitted as
@RECON-UNKNOWN stubs carrying everything a human/agent needs to finish them fast --
the shipped bytes, a disassembly, the detected signature, and the first blocking
instruction. Each stub is completed with the refine.check() oracle loop.

    from engine.sdk import emit
    emit.write_tree(report, out_dir, bin_path)      # or: wt sdk recon <bin> --emit <dir>

No fabrication: a stub is never a guess -- it is the raw evidence plus a TODO. The
module .def and makefile reproduce the shipped ordinals and the MSC 6.0a recipe.
"""
import os, struct, sys
from capstone import Cs, CS_ARCH_X86, CS_MODE_16
from engine.format_loaders import ne

_MD = Cs(CS_ARCH_X86, CS_MODE_16)


def _disasm(fb):
    return "\n".join("       %04x  %-10s %-8s %s" % (i.address, i.bytes.hex(), i.mnemonic, i.op_str)
                     for i in _MD.disasm(bytes(fb), 0))


_CT = {"far": "LPVOID", "long": "long", "int": "int", "void": "void", "unknown": "int"}


def _sdk_proto(fn, sdk_db):
    """A real C prototype for an exported function whose signature the SDK knows,
    else None. Uses the authentic parameter/return types from the SDK headers."""
    nm = fn.get("name")
    if not (nm and sdk_db):
        return None
    s = sdk_db.sig(nm)
    if not s:
        return None
    ps = ", ".join("%s a%d" % (_CT.get(k, "int"), i) for i, (_, k) in enumerate(s["params"])) or "void"
    return "%s far pascal %s(%s)" % (_CT.get(s["ret"], "int"), s["name"], ps)


def _stub(fn, fb, sdk_db=None):
    """A declined/needs-link function as a documented @RECON-UNKNOWN stub. Exported
    functions get their authentic SDK prototype in the stub signature."""
    nm = fn.get("name") or ("sub_%x" % fn["off"])
    proto = _sdk_proto(fn, sdk_db)
    head = ["/* @RECON-UNKNOWN  %s  (seg-offset 0x%x, %d bytes)" % (nm, fn["off"], fn["size"])]
    if fn["status"] == "needs-link":
        head.append(" * status: recovered, PENDING whole-DLL link (intra-segment call).")
        head.append(" * candidate C below -- verify via the link, not per-function.")
    else:
        head.append(" * status: DECLINED by msc6dec -- %s" % fn.get("reason", "unmodeled"))
        head.append(" * complete this body, then verify with refine.check(bytes, your_c).")
    if proto:
        head.append(" * SDK prototype: %s;" % proto)
    head.append(" *")
    head.append(" * shipped bytes: %s" % fb.hex())
    head.append(" * disassembly:")
    head.append(_disasm(fb))
    head.append(" */")
    body = fn.get("c", "").rstrip()
    if fn["status"] == "needs-link" and body:
        return "\n".join(head) + "\n" + body + "\n"
    sig = proto or ("int far %s()" % nm)                  # authentic sig when the SDK has it
    return "\n".join(head) + "\n" + "%s { /* TODO */ }\n" % sig


def _imported_symbols(f):
    """Every distinct imported-API name the module references (from all segments)."""
    names = set(); mods = f.module_refs()
    for seg in f.segments():
        if seg["kind"] != "CODE":
            continue
        for r in f.relocations(seg):
            if r["kind"] == "IMPORTNAME":
                names.add(f.imported_name(r["name_off"]))
    return names, mods


def _attach_sdk(f, out_dir, mod, bits=16):
    """Discover the SDK/DDK kits, map every imported + exported symbol to its header,
    and write a shared <mod>.h (the #includes) plus a build.mk with real /I and lib
    paths -- so the reconstructed skeletons compile against the AUTHENTIC SDK."""
    try:
        from engine.sdk import kits
        reg = kits.Registry.discover()
    except Exception:
        return {}
    imports, mods = _imported_symbols(f)
    exports = [nm for nm, o in f.resident_names()[1:] + f.nonresident_names()]
    used = set(x for x in (set(imports) | set(exports)) if x and all(32 <= ord(c) < 127 for c in x))
    headers, unresolved = set(), set()
    for sym in used:
        h = reg.header_for(sym, bits)
        (headers.add(h) if h else unresolved.add(sym))
    headers = sorted(headers) or ["windows.h"]
    # shared header: the #include block for every segment .c
    hdr = "/* %s.h -- SDK headers for the reconstructed %s (auto-attached from the kits). */\n" % (mod.lower(), mod)
    for h in headers:
        hdr += "#include <%s>\n" % h.lower()
    if unresolved:
        hdr += "\n/* symbols with no SDK header (module-private or DDK): %s */\n" % \
               ", ".join(sorted(unresolved)[:40])
    open(os.path.join(out_dir, "%s.h" % mod.lower()), "w", encoding="utf-8").write(hdr)
    # build.mk: authentic include + lib paths for this arch
    incs = reg.include_dirs(bits); libs = reg.lib_dirs(bits)
    mk = "# build.mk -- reconstruct %s against the authentic %d-bit SDK/DDK kits.\n" % (mod, bits)
    mk += "INCLUDE=%s\n" % ";".join(incs)
    mk += "LIB=%s\n" % ";".join(libs)
    mk += "CFLAGS=/c /AM /G2 /Gsw /Ox %s\n" % " ".join('/I"%s"' % d for d in incs)
    open(os.path.join(out_dir, "build.mk"), "w", encoding="utf-8").write(mk)
    return {"headers": headers, "unresolved": len(unresolved), "apidb": reg.apidb(bits),
            "include_dirs": incs, "lib_dirs": libs, "kits": [k.key for k in reg.for_bits(bits)]}


def write_tree(rep, out_dir, bin_path, verify=False):
    """Emit the tree. If verify=True, each msc6dec-produced function is recompiled with
    the oracle first, so only functions that REPRODUCE the shipped bytes are labelled
    byte-exact; unverified msc6dec output is labelled CANDIDATE (not a guarantee)."""
    if verify:
        from engine.sdk import reconstruct as R
        R.verify_inverted(rep)
    d = open(bin_path, "rb").read(); ne_off = struct.unpack_from("<I", d, 0x3c)[0]
    f = ne.NEFile.from_buffer(d, ne_off, bin_path); segs = {s["idx"]: s for s in f.segments()}
    mod = rep["module"]
    os.makedirs(out_dir, exist_ok=True)
    sdk = _attach_sdk(f, out_dir, mod, bits=16)              # <mod>.h + build.mk from the kits
    inc_line = '#include "%s.h"\n' % mod.lower() if sdk else ""
    sdk_db = sdk.get("apidb") if sdk else None               # authentic prototypes for stubs
    made = []
    exact = cand = link = todo = 0
    for s in rep["segments"]:
        seg = segs[s["idx"]]; segb = d[seg["file_off"]:seg["file_off"] + seg["length"]]
        parts = ["/* %s -- segment %d  (%d bytes, %d functions)\n"
                 "   Reconstructed by CodeBreaker msc6dec (MSC 6.0a). VERIFIED byte-exact\n"
                 "   functions are inlined; CANDIDATE = msc6dec output NOT yet oracle-verified\n"
                 "   (verify/complete with engine.sdk.refine.check). */\n" % (mod, s["idx"], s["length"], s["n"]),
                 inc_line]                                    # authentic SDK headers, auto-attached
        for fn in s["funcs"]:
            fb = segb[fn["off"]:fn["off"] + fn["size"]]
            nm = fn.get("name") or ("sub_%x" % fn["off"])
            if fn["status"] == "inverted" and fn.get("byte_exact"):
                parts.append("/* %s @0x%x -- VERIFIED byte-exact (recipe %s) */\n%s\n" %
                             (nm, fn["off"], fn.get("recipe", "?"), fn.get("c", "").rstrip()))
                exact += 1
            elif fn["status"] == "inverted":                 # produced but unverified/miss
                note = "CANDIDATE (msc6dec output -- NOT verified byte-exact; check with refine)"
                parts.append("/* %s @0x%x -- %s */\n%s\n" % (nm, fn["off"], note, fn.get("c", "").rstrip()))
                cand += 1
            elif fn["status"] == "needs-link":
                parts.append(_stub(fn, fb, sdk_db)); link += 1
            elif fn["status"] == "declined":
                parts.append(_stub(fn, fb, sdk_db)); todo += 1
        path = os.path.join(out_dir, "%s_seg%d.c" % (mod.lower(), s["idx"]))
        open(path, "w", encoding="utf-8").write("\n".join(parts)); made.append(path)
    _write_def(f, out_dir, mod)
    open(os.path.join(out_dir, "STATUS.md"), "w", encoding="utf-8").write(
        "# %s reconstruction status\n\n"
        "- VERIFIED byte-exact: %d%s\n- CANDIDATE (msc6dec output, unverified): %d\n"
        "- pending link: %d\n- @RECON-UNKNOWN (to complete): %d\n\n"
        "Byte-exact requires oracle verification (`refine.check`); CANDIDATE C is a\n"
        "starting point, NOT a guarantee.\n"
        % (mod, exact, "" if verify else " (run with verify=True to populate)", cand, link, todo))
    if sdk:
        with open(os.path.join(out_dir, "STATUS.md"), "a", encoding="utf-8") as fh:
            fh.write("\n## SDK/DDK auto-attached (%s)\n" % ", ".join(sdk["kits"]))
            fh.write("- headers (%s.h): %s\n" % (mod.lower(), ", ".join(sdk["headers"])))
            fh.write("- unresolved symbols (module-private): %d\n" % sdk["unresolved"])
            fh.write("- build.mk carries %d include dirs + %d lib dirs\n"
                     % (len(sdk["include_dirs"]), len(sdk["lib_dirs"])))
    return {"files": made, "exact": exact, "candidate": cand, "link": link, "todo": todo,
            "dir": out_dir, "sdk": sdk}


def _write_def(f, out_dir, mod):
    ords = {}
    for nm, o in f.resident_names()[1:] + f.nonresident_names():
        ords[o] = nm
    lines = ["LIBRARY     %s" % mod, "EXETYPE     WINDOWS", "", "EXPORTS"]
    for o in sorted(ords):
        lines.append("    %-16s @%d" % (ords[o], o))
    open(os.path.join(out_dir, "%s.def" % mod.lower()), "w", encoding="utf-8").write("\n".join(lines) + "\n")


if __name__ == "__main__":
    import sys
    from engine.sdk import reconstruct as R
    from engine.sdk import w95paths
    binp = sys.argv[1] if len(sys.argv) > 1 else w95paths.input_dll("setup", "inf", "lzexpand.dll")
    out = sys.argv[2] if len(sys.argv) > 2 else "recon_out"
    verify = "--verify" in sys.argv
    r = write_tree(R.analyze(binp), out, binp, verify=verify)
    print("wrote %d files to %s: %d verified byte-exact, %d candidate, %d pending-link, %d @RECON-UNKNOWN"
          % (len(r["files"]), r["dir"], r["exact"], r["candidate"], r["link"], r["todo"]))
