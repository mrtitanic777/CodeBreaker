#!/usr/bin/env python3
"""wthunk -- a from-scratch reimplementation of the MS Thunk Compiler (thunk.exe),
reverse-engineered from its output so the .thk thunk-script becomes a first-class
source type for a maintainable toolchain (OpenWatcom's wasm assembles the result).

thunk.exe reads a .thk and emits ONE .asm carrying BOTH the 32-bit and 16-bit
flat-thunk halves, gated by -DIS_32 / -DIS_16, then ml/wasm assembles each side.
This tool reproduces that codegen. The 16<->32 transition itself lives in the
Win95 kernel's flat-thunk runtime (QT_Thunk / FT_Thunk / SMapLS / SUnMapLS /
ThunkConnect); the generated glue only CALLS it.

    wthunk.py -o lz.asm lz.thk        # like: thunk -o lz.asm lz.thk

Scope: the direct-mapped 32->16 flat-thunk subset the Win95 base DLLs use
(enablemapdirect3216). Return/args are marshalled by size; pointer args are
SMapLS'd down and SUnMapLS'd back. The ThunkData checksum is emitted
self-consistently across both halves (which is what ThunkConnect verifies);
matching thunk.exe's exact hash is only needed to interface a *retail* half and
is left as a documented refinement.
"""
import re
import sys


# ---- .thk parsing -------------------------------------------------------------

def strip_comments(s):
    s = re.sub(r"/\*.*?\*/", "", s, flags=re.S)
    s = re.sub(r"//[^\n]*", "", s)
    return s


# argument / return marshalling class
WORD, DWORD, PTR, VOID = "WORD", "DWORD", "PTR", "VOID"


def build_types(text):
    """Map every type name to a marshalling class, resolving typedef chains."""
    base = {
        "void": VOID,
        "char": WORD, "unsigned char": WORD, "signed char": WORD,
        "short": WORD, "unsigned short": WORD,
        "int": WORD, "unsigned int": WORD,
        "long": DWORD, "unsigned long": DWORD,
    }
    aliases = {}   # name -> (base_string, is_pointer)
    structs = set()

    # typedef struct { ... } NAME;   and   typedef struct _T {...} NAME;
    for m in re.finditer(r"typedef\s+struct\s+\w*\s*\{.*?\}\s*(\w+)\s*;", text, flags=re.S):
        structs.add(m.group(1))
    # strip struct bodies so the simple typedef scan below is clean
    text = re.sub(r"typedef\s+struct\s+\w*\s*\{.*?\}\s*\w+\s*;", "", text, flags=re.S)

    # typedef BASE [*] NAME;
    for m in re.finditer(r"typedef\s+([\w ]+?)\s*(\*?)\s*(\w+)\s*;", text):
        b, star, name = m.group(1).strip(), m.group(2), m.group(3)
        aliases[name] = (b, star == "*")

    def classify(tystr):
        tystr = tystr.strip()
        star = tystr.endswith("*")
        core = tystr.rstrip("* ").strip()
        if star:
            return PTR
        if core in base:
            return base[core]
        if core in structs:
            return PTR   # only ever used via pointer here
        if core in aliases:
            b, isptr = aliases[core]
            if isptr:
                return PTR
            return classify(b)
        # unknown -> assume a 16-bit scalar
        return WORD

    return classify


def parse_thk(text):
    text = strip_comments(text)
    classify = build_types(text)
    funcs = []
    # RET NAME ( PARAMS ) { BODY }
    for m in re.finditer(r"([\w ]+?\*?)\s+(\w+)\s*\(([^)]*)\)\s*\{([^}]*)\}", text):
        ret, name, params, body = m.group(1).strip(), m.group(2), m.group(3), m.group(4)
        if name in ("struct",):
            continue
        args = []
        for p in params.split(","):
            p = p.strip()
            if not p or p.lower() == "void":
                continue
            # split trailing identifier from the type
            pm = re.match(r"(.+?)\s*(\*?)\s*(\w+)$", p)
            tystr = (pm.group(1) + pm.group(2)).strip()
            pname = pm.group(3)
            args.append({"name": pname, "cls": classify(tystr)})
        # pointer direction annotations: name = input|output|inout;
        dirs = dict(re.findall(r"(\w+)\s*=\s*(input|output|inout)\s*;", body))
        for a in args:
            a["dir"] = dirs.get(a["name"], "input")
        funcs.append({"name": name, "ret": classify(ret), "args": args})
    return funcs


# ---- checksum (self-consistent; see module docstring) -------------------------

def checksum(funcs):
    h = 0x1505
    for f in funcs:
        for ch in f["name"]:
            h = ((h * 33) ^ ord(ch)) & 0xFFFFFFFF
        for a in f["args"]:
            h = ((h * 33) ^ ord(a["cls"][0])) & 0xFFFFFFFF
    return h & 0xFFFFFFFF


# ---- 32-bit emit --------------------------------------------------------------

FT_EXITS = list(range(0, 57, 4))
EBP_MAPS = list(range(8, 41, 4))


def emit32(pfx, funcs, cs, out):
    w = out.append
    w("\t.386\n\t.model FLAT,STDCALL\n\n")
    w(";-- Import common flat thunk routines (in k32)\n")
    for r in ("MapHInstLS", "MapHInstLS_PN", "MapHInstSL", "MapHInstSL_PN",
              "FT_Prolog", "FT_Thunk", "QT_Thunk"):
        w(f"externdef {r}\t:near\n")
    for n in FT_EXITS:
        w(f"externdef FT_Exit{n}\t:near\n")
    w("externdef SMapLS\t:near\n")
    w("externdef SUnMapLS\t:near\n")
    for n in EBP_MAPS:
        w(f"externdef SMapLS_IP_EBP_{n}\t:near\n")
        w(f"externdef SUnMapLS_IP_EBP_{n}\t:near\n")
    w("\nexterndef MapSL:near\n\n\t.code \n\n")
    w("\t.data\n\n")
    w(f"public {pfx}_ThunkData32\t;This symbol must be exported.\n")
    w(f"{pfx}_ThunkData32 label dword\n")
    w("\tdd\t3130534ch\t;Protocol 'LS01'\n")
    w(f"\tdd\t0{cs:x}h\t;Checksum\n")
    w("\tdd\t0\t;Jump table address.\n")
    w("\tdd\t3130424ch\t;'LB01'\n")
    w("\tdd\t0\t;Flags\n\tdd\t0\t;Reserved (MUST BE 0)\n\tdd\t0\t;Reserved (MUST BE 0)\n")
    w(f"\tdd\toffset QT_Thunk_{pfx} - offset {pfx}_ThunkData32\n")
    w(f"\tdd\toffset FT_Prolog_{pfx} - offset {pfx}_ThunkData32\n\n\t.code \n\n")
    w("externdef ThunkConnect32@24:near\n")
    w(f"public {pfx}_ThunkConnect32@16\n{pfx}_ThunkConnect32@16:\n")
    w(f"\tpop\tedx\n\tpush\toffset {pfx}_ThkData16\n\tpush\toffset {pfx}_ThunkData32\n")
    w(f"\tpush\tedx\n\tjmp\tThunkConnect32@24\n")
    w(f"{pfx}_ThkData16 label byte\n\tdb\t\"{pfx}_ThunkData16\",0\n\n")
    w(f"pfnQT_Thunk_{pfx}\tdd offset QT_Thunk_{pfx}\n")
    w(f"pfnFT_Prolog_{pfx}\tdd offset FT_Prolog_{pfx}\n\t.data\n")
    w(f"QT_Thunk_{pfx} label byte\n\tdb\t32 dup(0cch)\t;Patch space.\n\n")
    w(f"FT_Prolog_{pfx} label byte\n\tdb\t32 dup(0cch)\t;Patch space.\n\n\t.code \n\n")
    w(";************************ START OF THUNK BODIES************************\n\n")

    n = len(funcs)
    for i, f in enumerate(funcs):
        cl = (n - 1) - i
        nm = f["name"]
        stackb = 4 * len(f["args"])
        w(f"\npublic {nm}@{stackb}\n{nm}@{stackb}:\n\tmov\tcl,{cl}\n")
        w(f"public II{nm}@{stackb}\nII{nm}@{stackb}:\n")
        w("\tpush\tebp\n\tmov\tebp,esp\n\tpush\tecx\n\tsub\tesp,60\n")
        off = 8
        unmaps = []
        for a in f["args"]:
            if a["cls"] == PTR:
                w(f"\tcall\tSMapLS_IP_EBP_{off}\n\tpush\teax\n")
                unmaps.append(off)
            elif a["cls"] == DWORD:
                w(f"\tpush\tdword ptr [ebp+{off}]\t;{a['name']}: dword->dword\n")
            else:
                w(f"\tpush\tword ptr [ebp+{off}]\t;{a['name']}: dword->word\n")
            off += 4
        w(f"\tcall\tdword ptr [pfnQT_Thunk_{pfx}]\n")
        if f["ret"] == DWORD:
            w("\tshl\teax,16\n\tshrd\teax,edx,16\n")
        elif f["ret"] == WORD:
            w("\tcwde\n")
        for o in unmaps:
            w(f"\tcall\tSUnMapLS_IP_EBP_{o}\n")
        w("\tleave\n")
        w(f"\tretn\t{stackb}\n" if stackb else "\tretn\n")


# ---- 16-bit emit --------------------------------------------------------------

def emit16(pfx, funcs, cs, out):
    w = out.append
    w("\t.286\n\t.model LARGE,PASCAL\n\n\t.code\t\n\n")
    for f in reversed(funcs):
        w(f"externdef {f['name']}:far\n")
    w(f"\n\nFT_{pfx}TargetTable label word\n")
    for f in reversed(funcs):
        w(f"\tdw\toffset {f['name']}\n\tdw\t   seg {f['name']}\n")
    w("\n\n\t.data\n\n")
    w(f"public {pfx}_ThunkData16\t;This symbol must be exported.\n")
    w(f"{pfx}_ThunkData16\tdd\t3130534ch\t;Protocol 'LS01'\n")
    w(f"\tdd\t0{cs:x}h\t;Checksum\n")
    w(f"\tdw\toffset FT_{pfx}TargetTable\n\tdw\tseg    FT_{pfx}TargetTable\n")
    w("\tdd\t0\t;First-time flag.\n\n\n\t.code \n\n")
    w("externdef ThunkConnect16:far\n")
    w(f"public {pfx}_ThunkConnect16\n{pfx}_ThunkConnect16:\n")
    w(f"\tpop\tax\n\tpop\tdx\n\tpush\tseg    {pfx}_ThunkData16\n\tpush\toffset {pfx}_ThunkData16\n")
    w(f"\tpush\tseg    {pfx}_ThkData32\n\tpush\toffset {pfx}_ThkData32\n")
    w("\tpush\tcs\n\tpush\tdx\n\tpush\tax\n\tjmp\tThunkConnect16\n")
    w(f"{pfx}_ThkData32 label byte\n\tdb\t\"{pfx}_ThunkData32\",0\n")


def generate(pfx, funcs):
    cs = checksum(funcs)
    out = ["\tpage\t,132\n\n;wthunk -- MS Thunk Compiler reimplementation (OpenWatcom-targeted)\n\n"]
    out.append(f"\tTITLE\t${pfx}.asm\n\n")   # .386/.286 emitted per-branch (wasm-friendly)
    out.append("IFNDEF IS_16\nIFNDEF IS_32\n%out command line error: specify one of -DIS_16, -DIS_32\n.err\nENDIF  ;IS_32\nENDIF  ;IS_16\n\n\n")
    out.append("IFDEF IS_32\nIFDEF IS_16\n%out command line error: you can't specify both -DIS_16 and -DIS_32\n.err\nENDIF ;IS_16\n")
    out.append(";************************* START OF 32-BIT CODE *************************\n\n\n")
    emit32(pfx, funcs, cs, out)
    out.append("\n\n\nELSE\n;************************* START OF 16-BIT CODE *************************\n\n\n\n")
    emit16(pfx, funcs, cs, out)
    out.append("\n\nENDIF\nEND\n")
    return "".join(out)


def main(argv):
    out_path, in_path = None, None
    i = 0
    while i < len(argv):
        if argv[i] == "-o":
            out_path = argv[i + 1]; i += 2
        else:
            in_path = argv[i]; i += 1
    if not in_path:
        print("usage: wthunk.py -o out.asm in.thk", file=sys.stderr); return 2
    pfx = re.sub(r"\.asm$", "", out_path.replace("\\", "/").split("/")[-1]) if out_path else "thk"
    funcs = parse_thk(open(in_path).read())
    asm = generate(pfx, funcs)
    if out_path:
        open(out_path, "w", newline="\n").write(asm)
        print(f"wthunk: {in_path} -> {out_path}  ({len(funcs)} thunks, prefix '{pfx}')", file=sys.stderr)
    else:
        sys.stdout.write(asm)
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
