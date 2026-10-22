"""match.py -- does a period compiler turn the reconstruction back into the original's bytes?

    python -m codebreaker match <module> [--flags "/O2 /Gz"] [--try] [--json FILE]

This is NOT the measure of a reconstruction: that is whether it behaves as the original
(`measure`). It is a second, optional signal. Where a function's reconstructed C, compiled with
a compiler of the original's time, comes out as the very bytes the original has, the C is as
close to what was written as a binary can show; where it does not, nothing follows -- the same
source gives different bytes under another compiler version or another switch.

Each reconstructed function is compiled on its own with the compiler named by `period_compiler`
in settings.json (a 32-bit Microsoft C compiler that runs natively: the folder holding CL.EXE),
its bytes are read from the object file with the places the linker fills in masked out, and
compared with the original function's bytes, masked at the same places:

    identical     every byte outside the masked places is the same
    same size     as long as the original, some bytes differ (registers, instruction choice)
    different     another length

`--try` compiles under several sets of switches and reports, per function, the best; which set
matches most functions says how the module was built. 32-bit Windows modules here; a 16-bit module
goes to match16.py, a PowerPC image (or .xex) to match_powerpc.py.
"""
import json
import os
import re
import struct
import subprocess
from concurrent.futures import ThreadPoolExecutor

FLAG_SETS = ("/O2 /Gz", "/Oxs /Gz", "/O1 /Gz", "/Ox /Gz", "/O2 /Gz /Oy-", "/Od /Gz", "/O2 /Gd")


def compiler():
    """CL.EXE of the period compiler, or '' when settings.json names none."""
    from codebreaker.base import settings
    d = settings.get("period_compiler")
    exe = os.path.join(d or "", "CL.EXE")
    return exe if d and os.path.isfile(exe) else ""


def object_functions(obj):
    """{symbol: (bytes, mask)} for the functions of a COFF object file; mask has 1 for every
    byte the linker fills in (an address, the distance of a call)."""
    _machine, nsect, _ts, symptr, nsyms, opt, _flags = struct.unpack_from("<HHIIIHH", obj, 0)
    sects = []
    for i in range(nsect):
        p = 20 + opt + 40 * i
        _vs, _va, rawsize, rawptr, relptr, _ln, nrel, _nln, chars = struct.unpack_from("<IIIIIIHHI", obj, p + 8)
        sects.append((rawptr, rawsize, relptr, nrel, chars))
    strings = symptr + 18 * nsyms
    funcs, i = {}, 0
    while i < nsyms:
        p = symptr + 18 * i
        raw, value, sect, typ, _cls, naux = struct.unpack_from("<8sIhHBB", obj, p)
        if raw[:4] == b"\0\0\0\0":
            off = strings + struct.unpack_from("<I", raw, 4)[0]
            name = obj[off:obj.index(b"\0", off)].decode("latin1")
        else:
            name = raw.rstrip(b"\0").decode("latin1")
        if sect > 0 and typ == 0x20 and sects[sect - 1][4] & 0x20:
            funcs.setdefault(sect, []).append((value, name))
        i += 1 + naux
    out = {}
    for sect, lst in funcs.items():
        rawptr, rawsize, relptr, nrel, _chars = sects[sect - 1]
        mask = bytearray(rawsize)
        for k in range(nrel):
            at, _sym, _type = struct.unpack_from("<IIH", obj, relptr + 10 * k)
            mask[at:at + 4] = b"\1\1\1\1"
        lst.sort()
        for k, (value, name) in enumerate(lst):
            end = lst[k + 1][0] if k + 1 < len(lst) else rawsize
            out[name] = (obj[rawptr + value:rawptr + end], bytes(mask[value:end]))
    return out


def _trim(code, mask):
    """Without the padding a compiler puts after a function (int 3, nop, zero bytes)."""
    n = len(code)
    while n and not mask[n - 1] and code[n - 1] in (0xCC, 0x90, 0x00) and n > 1 and code[n - 2] in (0xC3, 0xCC, 0x90, 0x00):
        n -= 1
    return code[:n], mask[:n]


def original_functions(path, addresses):
    """{address: (bytes, mask)} for functions of a 32-bit module; the mask marks relocated words
    and the distances of direct calls and jumps, which depend on where things are."""
    import capstone
    from codebreaker.formats import pe_functions
    from codebreaker.formats.pe import open_pe
    pe = open_pe(path)
    spans = pe_functions.function_spans(path)
    relocated = set()
    va, size = pe.directory(5)
    off = pe.file_offset(va) if va else None
    end = (off or 0) + size
    while off is not None and off + 8 <= min(end, len(pe.data)):
        page, block = struct.unpack_from("<II", pe.data, off)
        if block < 8:
            break
        for k in range((block - 8) // 2):
            e = struct.unpack_from("<H", pe.data, off + 8 + 2 * k)[0]
            if e >> 12 == 3:
                relocated.add(pe.image_base + page + (e & 0xFFF))
        off += block
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    out = {}
    for start in addresses:
        if start not in spans:
            continue
        stop = spans[start][0]
        o = pe.file_offset(start - pe.image_base)
        if o is None:
            continue
        code = pe.data[o:o + (stop - start)]
        mask = bytearray(len(code))
        for a in range(start, stop):
            if a in relocated:
                mask[a - start:a - start + 4] = b"\1\1\1\1"
        for addr, n, mnemonic, _ops in md.disasm_lite(code, start):
            first = code[addr - start]
            if first in (0xE8, 0xE9) and n == 5:
                mask[addr - start + 1:addr - start + 5] = b"\1\1\1\1"
        out[start] = _trim(code, bytes(mask[:len(code)]))
    return out


def compare(built, original):
    """-> ("identical" | "same size" | "different", share of bytes equal outside masked places)"""
    (a, ma), (b, mb) = _trim(*built), original
    if len(a) != len(b):
        import difflib
        return "different", difflib.SequenceMatcher(None, a, b, autojunk=False).ratio()
    free = [i for i in range(len(a)) if not ma[i] and not mb[i]]
    same = sum(1 for i in free if a[i] == b[i])
    return ("identical" if same == len(free) else "same size"), (same / len(free) if free else 1.0)


def distance(built, original):
    """How far a compiled function's bytes are from the original's, outside the places the linker
    fills in. -> (verdict, number of bytes that differ, total bytes compared, first differing
    offset or -1). When the lengths differ the whole tail counts as differing, so a longer or
    shorter function is always farther than one of the right length."""
    (a, ma), (b, mb) = _trim(*built), original
    n = min(len(a), len(b))
    diff = first = 0
    saw = False
    for i in range(n):
        if ma[i] or mb[i]:
            continue
        if a[i] != b[i]:
            diff += 1
            if not saw:
                first, saw = i, True
    diff += abs(len(a) - len(b))
    verdict = "identical" if diff == 0 else "same size" if len(a) == len(b) else "different"
    return verdict, diff, max(len(a), len(b)), (first if saw else (n if len(a) != len(b) else -1))


def relocation_targets(path):
    """The addresses the module's own relocations point at: every place in it that a word of
    code or data names by address."""
    from codebreaker.formats.pe import open_pe
    pe = open_pe(path)
    out = set()
    va, size = pe.directory(5)
    off = pe.file_offset(va) if va else None
    end = (off or 0) + size
    while off is not None and off + 8 <= min(end, len(pe.data)):
        page, block = struct.unpack_from("<II", pe.data, off)
        if block < 8:
            break
        for k in range((block - 8) // 2):
            e = struct.unpack_from("<H", pe.data, off + 8 + 2 * k)[0]
            if e >> 12 == 3:
                o = pe.file_offset(page + (e & 0xFFF))
                if o is not None and o + 4 <= len(pe.data):
                    out.add(struct.unpack_from("<I", pe.data, o)[0])
        off += block
    return out


_BOUND = re.compile(r"(?m)^#define (\w+) \(\*\(([^()]*?) ?\*\)0x([0-9a-fA-F]+)\)[ \t]*$")
_CONSTANT = re.compile(r"(-\s*)?\b0x([0-9a-fA-F]{5,8})\b")
_RESULT_OF_NOTHING = re.compile(r"(?m)^[ \t]*return in_EAX;[ \t]*\n")


def period_source(header, texts, imports, targets):
    """(header, {name: text}) as they are given to a period compiler.

    For judging behaviour the reconstruction names the original's storage by its address and
    calls imports as plain functions: that is what lets it run beside the original. The original
    was not written that way, and a compiler shows the difference in every instruction that
    touches a global or calls Windows. Here the same source is given the declarations its author
    had, which change no meaning:

      * an imported function is `__declspec(dllimport)` -- called through its slot;
      * a global is a variable, not a number cast to a pointer, and an address taken as a number
        (`0xbff10070`, where the module's relocations show that number IS an address) is the
        address of a variable -- so the compiler leaves it to the linker, as the original's did;
      * a function whose every `return` hands back "whatever was in EAX" returns nothing (when
        no caller uses a result).
    """
    lines, bound = [], {}
    for m in _BOUND.finditer(header):
        if int(m.group(3), 16) in targets:
            bound[m.group(1)] = m.group(2)
    names = {}

    def at(address):
        names[address] = "codebreaker_at_%08x" % address
        return names[address]

    def constant(m, text):
        value = int(m.group(2), 16)
        if m.group(1):
            value = (-value) & 0xFFFFFFFF
        if value not in targets:
            return m.group(0)
        before = text[:m.start()].rstrip()[-1:]
        joined = m.group(1) and (before.isalnum() or before in "_)]")
        return ("+ " if joined else "") + "(int4)&" + at(value)
    out, nothing = {}, set()
    bodies = "\n".join(t.partition("{")[2] for t in texts.values())

    def used(name):
        """Is the function's result used anywhere (a call that is not a statement of its own)?"""
        for m in re.finditer(r"\b%s\s*\(" % re.escape(name), bodies):
            before = bodies[:m.start()].rstrip()
            if before[-1:] not in ";{})" and not before.endswith("else"):
                return True
        return False
    for name, text in texts.items():
        text = _CONSTANT.sub(lambda m, t=text: constant(m, t), text)
        returns = re.findall(r"\breturn\b[^;]*;", text)
        if returns and all(r.replace(" ", "") == "returnin_EAX;" for r in returns) and not used(name):
            head, brace, body = text.partition("{")
            body = _RESULT_OF_NOTHING.sub("", re.sub(r"(?m)^[ \t]*\w+ in_EAX;[ \t]*\n", "", body))
            if "in_EAX" not in body:                     # (given a value somewhere: it is a result)
                text = re.sub(r"^\s*\w+\b", "void", head, count=1) + brace + body
                nothing.add(name)
        out[name] = text
    for line in header.split("\n"):
        m = _BOUND.match(line)
        if m and m.group(1) in bound:
            line = "extern %s %s;" % (bound[m.group(1)], m.group(1))
        else:
            f = re.match(r"^(\w+)((?: __\w+)?) (\w+)\((.*)\);[ \t]*$", line)
            if f and f.group(3) in imports and f.group(4) == "":
                line = "__declspec(dllimport) " + line
            elif f and f.group(3) in nothing:
                line = "void%s %s(%s);" % (f.group(2), f.group(3), f.group(4))
        lines.append(line)
    lines += ["extern char %s;" % n for _a, n in sorted(names.items())]
    return "\n".join(lines), out


_DECLARES = re.compile(r"^(?:__declspec\(dllimport\) )?(?:extern )?[\w ]*?[ *](\w+)(?:\(.*\)|\[\d*\])?;[ \t]*$|^#define (\w+) \(\*")


def needed_header(header, text):
    """The header with only the declarations `text` uses (types and macros are all kept).

    What this compiler makes of a function depends on what was declared before it -- a variable
    the function never mentions, declared a thousand lines earlier, can turn `cmp eax, ecx` into
    `cmp ecx, eax`. The author's file had its own declarations, not the whole module's; a function
    given just what it uses is one more context to try, and often the nearer one."""
    words = set(re.findall(r"[A-Za-z_]\w*", text))
    keep, inside = [], False
    for line in header.split("\n"):
        if inside or line.startswith("typedef struct"):          # a structure, whole or not at all
            if line.startswith("typedef struct"):
                inside = True
                wanted = line.split()[2] in words
            if wanted:
                keep.append(line)
            inside = not line.startswith("}")
            continue
        m = _DECLARES.match(line)
        if m and (m.group(1) or m.group(2)) not in words and not line.startswith("typedef"):
            continue
        keep.append(line)
    return "\n".join(keep)


def compile_function(cl, env, header, source, name, work, flags):
    """Compile one function's C with the period compiler. -> (bytes, mask) of the function, or
    None when it does not compile. `header` is the shared types/declarations. What the compiler
    made of a text is kept (base/cache.py): a search tries the same text many times."""
    import pickle
    from codebreaker.base import cache
    k = cache.key(cache.tool(cl), flags, header, source, name)
    had = cache.get("period", k)
    if had is not None:
        return pickle.loads(had)
    built = _compile_function(cl, env, header, source, name, work, flags)
    cache.put("period", k, pickle.dumps(built))
    return built


def _compile_function(cl, env, header, source, name, work, flags):
    os.makedirs(work, exist_ok=True)
    with open(os.path.join(work, "m.h"), "w", newline="\n", encoding="utf-8") as f:
        f.write(header.replace("unsigned long long", "unsigned __int64").replace("long long", "__int64"))
    stem = "one"
    for ext in (".obj", ".c"):
        try:
            os.remove(os.path.join(work, stem + ext))
        except OSError:
            pass
    with open(os.path.join(work, stem + ".c"), "w", newline="\n", encoding="utf-8") as f:
        f.write('#include "m.h"\n\n' + source + "\n")
    r = subprocess.run([cl, "/nologo", "/c", "/W0"] + flags.split() + [stem + ".c"], cwd=work, env=env,
                       capture_output=True, text=True, errors="replace", timeout=300)
    if r.returncode or not os.path.isfile(os.path.join(work, stem + ".obj")):
        return None
    funcs = object_functions(open(os.path.join(work, stem + ".obj"), "rb").read())
    return next((v for k, v in funcs.items() if k.lstrip("_@").split("@")[0] == name), None) or \
        (next(iter(funcs.values())) if len(funcs) == 1 else None)


def compiler_env(cl):
    return dict(os.environ, PATH=os.path.dirname(cl) + os.pathsep + os.environ.get("PATH", ""),
                INCLUDE=os.path.join(os.path.dirname(os.path.dirname(cl)), "INCLUDE"))


def run(module_path, flags, work, jobs=4):
    """Compile every reconstructed function of a module with the period compiler and compare.
    -> {function name: (verdict, share, compiled size, original size)}; a function the compiler
    rejects has verdict "does not compile"."""
    from codebreaker.verify import win32
    cl = compiler()
    if not cl:
        raise SystemExit("no period compiler (settings.json: period_compiler -- the folder holding CL.EXE)")
    m = win32.prepare(module_path, os.path.join(work, "source"))
    out_dir = os.path.join(work, re.sub(r"\W+", "_", flags).strip("_") or "default")
    os.makedirs(out_dir, exist_ok=True)
    with open(os.path.join(out_dir, "m.h"), "w", newline="\n", encoding="utf-8") as f:
        f.write(m.header.replace("unsigned long long", "unsigned __int64").replace("long long", "__int64"))
    env = dict(os.environ, PATH=os.path.dirname(cl) + os.pathsep + os.environ.get("PATH", ""),
               INCLUDE=os.path.join(os.path.dirname(os.path.dirname(cl)), "INCLUDE"))
    names = list(m.order)

    def one(i):
        stem = "f%04d" % i
        with open(os.path.join(out_dir, stem + ".c"), "w", newline="\n", encoding="utf-8") as f:
            f.write('#include "m.h"\n\n' + m.texts[names[i]] + "\n")
        r = subprocess.run([cl, "/nologo", "/c", "/W0"] + flags.split() + [stem + ".c"], cwd=out_dir, env=env,
                           capture_output=True, text=True, errors="replace", timeout=300)
        if r.returncode:
            return None
        with open(os.path.join(out_dir, stem + ".obj"), "rb") as f:
            return object_functions(f.read())
    with ThreadPoolExecutor(max_workers=jobs) as ex:
        objects = list(ex.map(one, range(len(names))))
    address = {name: va for va, name in m.by_va.items()}
    originals = original_functions(module_path, sorted(address.values()))
    results = {}
    for name, funcs in zip(names, objects):
        orig = originals.get(address[name])
        if funcs is None or orig is None:
            results[name] = ("does not compile" if funcs is None else "no original extent", 0.0, 0, len(orig[0]) if orig else 0)
            continue
        mine = next((v for k, v in funcs.items() if k.lstrip("_@").split("@")[0] == name), None) or \
            (next(iter(funcs.values())) if len(funcs) == 1 else None)
        if mine is None:
            results[name] = ("does not compile", 0.0, 0, len(orig[0]))
            continue
        verdict, share = compare(mine, orig)
        results[name] = (verdict, share, len(_trim(*mine)[0]), len(orig[0]))
    return results


def main(argv):
    """python -m codebreaker match <module> [--flags "/O2 /Gz"] [--try] [--json FILE]"""
    from codebreaker.base import corpus, settings
    from codebreaker.formats.pe import open_pe
    args = [a for a in argv if not a.startswith("--")]
    flags = argv[argv.index("--flags") + 1] if "--flags" in argv else FLAG_SETS[0]
    args = [a for a in args if a != flags and not a.endswith(".json")]
    if not args:
        print(__doc__)
        return 2
    path = args[0] if os.path.isfile(args[0]) else corpus.retail(args[0])
    if args[0].lower().endswith(".xex") or (path and corpus.lane(path) == "pe" and
                                            open_pe(path).machine == 0x01F2):   # PowerPC: the Xbox 360 compiler
        from codebreaker.verify import match_powerpc
        return match_powerpc.main(argv)
    if path and corpus.lane(path) == "ne":                   # a 16-bit module: Microsoft C 6
        from codebreaker.verify import match16
        return match16.main(argv)
    if not path or corpus.lane(path) != "pe":
        raise SystemExit("%s: a 32-bit or 16-bit Windows module is needed" % args[0])
    work = settings.state("match", os.path.basename(path).lower())
    sets = FLAG_SETS if "--try" in argv else (flags,)
    best, per_set = {}, {}
    for fl in sets:
        r = run(path, fl, work)
        per_set[fl] = sum(1 for v in r.values() if v[0] == "identical")
        for name, v in r.items():
            rank = ("identical", "same size", "different", "no original extent", "does not compile").index(v[0])
            if name not in best or (rank, -v[1]) < best[name][0]:
                best[name] = ((rank, -v[1]), v, fl)
        if len(sets) > 1:
            print("  %-16s %d identical" % (fl, per_set[fl]), flush=True)
    counts = {}
    for _rank, v, _fl in best.values():
        counts[v[0]] = counts.get(v[0], 0) + 1
    print("%s: %d functions compiled with the period compiler: %s" % (
        os.path.basename(path), len(best), ", ".join("%s %d" % (k, counts[k]) for k in (
            "identical", "same size", "different", "does not compile", "no original extent") if counts.get(k))))
    close = sorted(((v[1], n, v) for n, (_r, v, _f) in best.items() if v[0] != "identical"), reverse=True)[:8]
    for share, n, v in close:
        print("  %-28s %-9s %3d%% of bytes equal  (%d bytes, original %d)" % (n, v[0], 100 * share, v[2], v[3]))
    if "--json" in argv:
        with open(argv[argv.index("--json") + 1], "w", encoding="utf-8") as f:
            json.dump({n: {"verdict": v[0], "share": v[1], "size": v[2], "original": v[3], "flags": fl}
                       for n, (_r, v, fl) in best.items()}, f, indent=1, sort_keys=True)
    return 0
