#!/usr/bin/env python3
"""
libnames.py — recover the AUTHORITATIVE ordinal -> name map from the Windows
1.01 SDK's own import library.

This is the document the whole Level-3a naming effort was reconstructing by
hand. `SLIBW.LIB` (SDK disk 2) is the import library applications linked
against, and an import library exists precisely to say "the function called
INITTASK is ordinal 91 of KERNEL". It is not a cross-check and not a
hypothesis: it is the mapping the linker used to build the very binaries in
files/WF.

FORMAT. Each import is an OMF COMENT record carrying an IMPDEF:

        A0 01 <flag> <len>NAME <len>MODULE <ordinal:word>
        ^^ ^^  ^^
        |  |   +-- 01 = import by ordinal
        |  +------ IMPDEF subtype
        +--------- Microsoft OMF extension

so a scan for that signature yields every (module, ordinal, name) triple.

    python libnames.py                 the whole map
    python libnames.py --check         compare against ORDINALS.md
    python libnames.py --md            emit ORDINALS.md table rows

The names are UPPERCASE in the library (the linker upper-cases them); the
mixed-case spelling comes from WINDOWS.H, which this tool consults so the
output reads as the SDK wrote it.
"""
import glob, os, re, struct, sys
import trace

LIBS = ["refs/sdk101/d02/SLIBW.LIB", "refs/sdk101/d03/MLIBW.LIB",
        "refs/sdk101/d04/LWLIBC.LIB", "refs/sdk101/d05/PASLIBW.LIB"]
HDR = "refs/sdk101/d06/WINDOWS.H"


def parse_lib(path):
    """[(module, ordinal, NAME)] from every IMPDEF in an OMF import library."""
    d = open(path, "rb").read()
    out, i = [], 0
    while True:
        i = d.find(b"\xa0\x01", i)
        if i < 0:
            break
        p = i + 2
        try:
            flag = d[p]; p += 1
            ln = d[p]; p += 1
            name = d[p:p + ln].decode("latin-1"); p += ln
            lm = d[p]; p += 1
            mod = d[p:p + lm].decode("latin-1"); p += lm
            if flag == 1:                      # by ordinal
                ordv = struct.unpack_from("<H", d, p)[0]
                if name.isidentifier() and mod.isidentifier() and 0 < ordv < 1000:
                    out.append((mod, ordv, name))
        except Exception:
            pass
        i += 2
    return out


def sdk_spelling():
    """UPPERCASE -> the mixed-case spelling WINDOWS.H uses."""
    m = {}
    if os.path.exists(HDR):
        src = open(HDR, encoding="latin-1", errors="replace").read()
        for w in re.findall(r"\bFAR\s+PASCAL\s+(\w+)", src):
            m[w.upper()] = w
        for w in re.findall(r"#define\s+(\w+)", src):
            m.setdefault(w.upper(), w)
    return m


def collect():
    seen, spell = {}, sdk_spelling()
    for lib in LIBS:
        if not os.path.exists(lib):
            continue
        for mod, ordv, name in parse_lib(lib):
            key = f"{mod}.{ordv}"
            seen.setdefault(key, spell.get(name, name))
    return seen


def main():
    seen = collect()
    if not seen:
        print("no import library found under refs/sdk101/ — extract the SDK disks first")
        sys.exit(1)

    if "--md" in sys.argv:
        print("| Ordinal | Name | Conf | Evidence |")
        print("|--------:|------|:----:|----------|")
        for k in sorted(seen, key=lambda x: (x.split(".")[0], int(x.split(".")[1]))):
            print(f"| {k} | `{seen[k]}` | ★★★ | the SDK's own import library "
                  f"(SLIBW.LIB) binds this name to this ordinal |")
        return

    if "--check" in sys.argv:
        ours = {k: v for k, v in trace.load_ordinals("ORDINALS.md").items()
                if v and re.match(r"^[A-Za-z_]\w*$", v)}
        agree = differ = onlyus = 0
        for k, v in sorted(ours.items()):
            if k in seen:
                if seen[k].lower() == v.lower():
                    agree += 1
                else:
                    differ += 1
                    print(f"  {k:12s} project says {v:24s} library says {seen[k]}")
            else:
                onlyus += 1
        print(f"\n{agree} of the project's names CONFIRMED by the import library")
        print(f"{differ} disagree")
        print(f"{onlyus} name ordinals the library does not export "
              f"(internal ordinals, or names this project coined)")
        print(f"{len(seen)} ordinals in the library overall; "
              f"{len(seen) - agree - differ} of them still unnamed here")
        return

    mods = {}
    for k, v in seen.items():
        mods.setdefault(k.split(".")[0], []).append(k)
    for m in sorted(mods):
        print(f"{m}: {len(mods[m])} exported names")
    print(f"total: {len(seen)}")


if __name__ == "__main__":
    main()
