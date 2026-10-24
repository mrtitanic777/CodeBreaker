#!/usr/bin/env python3
"""Microsoft C 8.00c's behaviours, against OpenWatcom's: how far an OpenWatcom is from generating
what Visual C++ 1.52c's compiler did (tests/msc8/cases.py).

    python tests/msc8/suite.py --expect        compile the cases with MS C 8 itself (settings:
                                               msvc152; run on Windows, no emulator) and keep what
                                               each function's code is in expected/ -- once
    python tests/msc8/suite.py [--wcc PATH]    compile them with OpenWatcom (the fork by default)
                                               and say, case by case, whether the code is the same

Bytes a fix-up names are not compared (where the linker puts things is not the compiler's). The
switches are given as Microsoft's; OpenWatcom is given what comes closest (WATCOM_FLAGS), and
where no switch of OpenWatcom's does a behaviour, that is what the fork has to learn."""
import json
import os
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
# after each case's functions another, so what follows a function (its padding) is compared too
AFTER = "\nint FAR CDECL zz_after(void) { return 12345; }\n"   # (not one OpenWatcom can fall into: an
#                                                                    empty one was, its RETF taken for the case's own)
ROOT = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, ROOT)
sys.path.insert(0, HERE)
from cases import CASES, HEADER                                   # noqa: E402

EXPECTED = os.path.join(HERE, "expected")


def watcom_flags(ms):
    """OpenWatcom's switches for Microsoft's"""
    out = ["-bt=dos", "-zq", "-w0", "-zl", "-ecc", "-zu", "-zmsc", "-j"]           # (cdecl unless a function says; SS != DS;
                                                                    # dos: no Windows prolog -- /Gw gives -zW;
                                                                    # -zmsc: the fork's Microsoft C 8 shapes)
    for f in ms.split():
        f = f.lstrip("/-")
        if f.startswith("A"):
            out.append({"AS": "-ms", "AM": "-mm", "AC": "-mc", "AL": "-ml"}.get(f, "-mm"))
        elif f.startswith("G"):
            for c in f[1:]:
                out += {"2": ["-2"], "3": ["-3"], "s": ["-s"], "w": ["-zW"]}.get(c, [])
        elif f.startswith("O"):
            opts = f[1:]
            # (/Ox is OpenWatcom's -ox but for e: Microsoft's does not put a function's body inside
            # its callers -- push_cs_call's helper is called, not copied in)
            out.append("-od" if opts == "d" else "-obmilr" if "x" in opts else "-o" + opts)
            if "s" in opts and "x" in opts:
                out.append("-os")
    return out


def code_of(obj, name):
    from codebreaker.build import msc6
    got = msc6.function_code(obj, name)
    return None if got is None else (got[0].hex(), got[1].hex())


def expect():
    from codebreaker.build import msc6
    os.makedirs(EXPECTED, exist_ok=True)
    if not msc6._native("msvc152"):
        raise SystemExit("MS C 8 is not set up to run on Windows (settings: msvc152, native16)")
    for case in CASES:
        work = tempfile.mkdtemp(prefix="msc8_")
        msc6._session(work, HEADER, [("C", case["src"] + AFTER)], case["flags"], "msvc152", 300)
        obj = os.path.join(work, "C.OBJ")
        if not os.path.isfile(obj):
            print("%-20s MS C refused it: %s" % (case["name"], open(os.path.join(work, "C.TXT"), "rb").read()[:300]))
            continue
        data = open(obj, "rb").read()
        res = {f: code_of(data, f) for f in case["funcs"]}
        with open(os.path.join(EXPECTED, case["name"] + ".json"), "w") as fh:
            json.dump({"flags": case["flags"], "what": case["what"], "code": res}, fh, indent=1)
        print("%-20s %s" % (case["name"], ", ".join("%s %d bytes" % (f, len(c[0]) // 2) if c else "%s MISSING" % f
                                                   for f, c in res.items())))


def disasm(hexcode):
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_16)
    return ["%-7s %s" % (i.mnemonic, i.op_str) for i in md.disasm(bytes.fromhex(hexcode), 0)]


def same(a, b):
    """the code alike, a fix-up's bytes (either side's) aside"""
    (ca, ma), (cb, mb) = a, b
    ca, ma, cb, mb = (bytes.fromhex(x) for x in (ca, ma, cb, mb))
    if len(ca) != len(cb):
        return False
    return all(x == y or ma[i] or mb[i] for i, (x, y) in enumerate(zip(ca, cb)))


def against(wcc, verbose=False):
    from codebreaker.build import watcom
    env = watcom._ow_env(watcom._ow())
    work = tempfile.mkdtemp(prefix="msc8_ow_")
    with open(os.path.join(work, "m.h"), "w") as f:
        f.write(HEADER)
    rows, alike = [], 0
    for case in CASES:
        path = os.path.join(EXPECTED, case["name"] + ".json")
        if not os.path.isfile(path):
            rows.append((case["name"], "no expected code (run --expect)", case["what"]))
            continue
        want = json.load(open(path))["code"]
        src = os.path.join(work, case["name"] + ".c")
        with open(src, "w") as f:
            f.write('#include "m.h"\n' + case["src"] + AFTER)
        r = subprocess.run([wcc] + watcom_flags(case["flags"]) + ["-fo=" + case["name"] + ".obj", src],
                           cwd=work, env=env, capture_output=True, text=True)
        obj = os.path.join(work, case["name"] + ".obj")
        if r.returncode or not os.path.isfile(obj):
            rows.append((case["name"], "OpenWatcom refused it", (r.stdout + r.stderr).strip()[:200]))
            continue
        data = open(obj, "rb").read()
        verdicts = []
        for f in case["funcs"]:
            w, mine = want.get(f), code_of(data, f)
            if not w or not mine:
                verdicts.append("%s: %s" % (f, "missing" if not mine else "no expected"))
            elif same(w, mine):
                verdicts.append("%s: SAME" % f)
            else:
                verdicts.append("%s: differs (%d bytes, MS %d)" % (f, len(mine[0]) // 2, len(w[0]) // 2))
                if verbose:
                    a, b = disasm(w[0]), disasm(mine[0])
                    verdicts.append("\n      MS C 8          | OpenWatcom\n" + "\n".join(
                        "      %-16s| %s" % (a[i] if i < len(a) else "", b[i] if i < len(b) else "")
                        for i in range(max(len(a), len(b)))))
        ok = all(v.endswith("SAME") for v in verdicts if not v.startswith("\n"))
        alike += ok
        rows.append((case["name"], "; ".join(verdicts), case["what"]))
    for name, verdict, what in rows:
        print("%-18s %s\n%18s (%s)" % (name, verdict, "", what))
    print("\n%d of %d behaviours as MS C 8 does them" % (alike, len(CASES)))
    return alike


def main(argv):
    if "--expect" in argv:
        expect()
        return 0
    from codebreaker.build import watcom
    wcc = argv[argv.index("--wcc") + 1] if "--wcc" in argv else watcom.ow_tool("wcc")
    against(wcc, verbose="-v" in argv)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
