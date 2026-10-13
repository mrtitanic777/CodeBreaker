"""observe.py -- what running a function shows that reading it does not.

    from codebreaker.analyze import image, observe
    seen = observe.run(image.load("comctl32.dll"), limit=400)     # {function: observation}

Each function is run in the emulator (emulate/emulator.py) with every argument pointing at a
buffer of its own, filled so that any value read from it says which buffer and which offset it
came from; and once more with only the arguments the static analysis took for pointers. From
those runs:

    fields      {argument: {offset: [size, reads, writes]}}   what it touches through each pointer
    nested      {(argument, offset): {offset: [size, reads, writes]}}   ... and through the
                pointers it finds there
    indirect    [(call site, function reached)]   where `call [reg+8]` and the like really went
                (addresses of the module's own functions the static call graph cannot have)
    passes      [(import, parameter, argument, offset, "address" | "value")]   a field, or the
                address of one, handed to a system function -- which says what the field is
    removed     bytes of arguments the function removed on return (None when it did not return)
    returns     ("argument", k) or ("field", k, offset) when the result is one of those
    idiom       "strlen", "memcpy", ... when the function behaves as that one on test inputs

The idiom check runs the small functions that loop and call nothing on a battery of strings and
buffers and compares what they return and leave behind with what strlen, wcslen, strcpy, strcat,
strcmp, stricmp, strchr, strrchr, strncpy, memcpy, memset, memcmp, memicmp and strncmp would. A function that passes every case of one of them
behaves as it on those cases; it is evidence for a name, not a proof.

32-bit modules only: the 16-bit emulator is entered with far pointers and a data segment, and
nothing here drives it yet. Runs go through the job pool (a native crash loses one function,
not the analysis), at most `jobs` at a time.
"""
import struct

from codebreaker.analyze import conventions, structures
from codebreaker.jobs import pool

BUFFERS, BUFFER_SIZE = 0x40000000, 0x1000        # argument k points at BUFFERS + k * 0x10000
MARK = 0x50000000                                # the dword at offset o of buffer k is MARK + (k << 24) + (o << 12)
TEXT_A, TEXT_B = 0x41000000, 0x41010000          # the idiom battery's two buffers
MAX_ARGS, INSNS = 8, 2500


def _fill(k):
    return b"".join(struct.pack("<I", MARK + (k << 24) + (o << 12)) for o in range(0, BUFFER_SIZE, 4))


def _argc(dll, name):
    from codebreaker.symbols import api_arguments
    return api_arguments.argc(dll, name)


def _trace(path, va, args, mem):
    from codebreaker.emulate import emulator
    try:
        return emulator.trace_function(path, func=va, args=args, mem_init=mem, stub_externals=True,
                                       sig_resolver=_argc, insn_cap=INSNS)
    except ImportError:
        raise
    except Exception as e:                          # the emulator could not run it at all
        return {"exit": "error: %s" % type(e).__name__, "mem_reads": [], "mem_writes": [], "calls": []}


def _where(addr):
    """What an address in one of our buffers is: ("arg", k, offset) or ("field", k, o, offset)."""
    if BUFFERS <= addr < BUFFERS + MAX_ARGS * 0x10000 and (addr & 0xFFFF) < BUFFER_SIZE:
        return ("arg", (addr - BUFFERS) >> 16, addr & 0xFFFF)
    if MARK <= addr < MARK + (MAX_ARGS << 24):
        return ("field", (addr - MARK) >> 24, (addr >> 12) & 0xFFF, addr & 0xFFF)
    return None


def _digest(rec, out):
    for bucket, col in (("mem_reads", 1), ("mem_writes", 2)):
        for m in rec.get(bucket) or ():
            w = _where(m["addr"])
            if w is None:
                continue
            if w[0] == "arg":
                cell = out["fields"].setdefault(w[1], {}).setdefault(w[2], [0, 0, 0])
            else:
                cell = out["nested"].setdefault((w[1], w[2]), {}).setdefault(w[3], [0, 0, 0])
            cell[0] = max(cell[0], m["size"])
            cell[col] += 1
    for c in rec.get("calls") or ():
        t = c.get("resolved_target")
        if c.get("indirect") and t and not c.get("external") and (c["pc"], t) not in out["indirect"]:
            out["indirect"].append((c["pc"], t))
    for api, args in rec.get("api_calls") or ():
        for j, v in enumerate(args or ()):
            w = _where(v)
            if w and w[0] == "arg":
                row = (api, j, w[1], w[2], "address")
            elif w and w[3] == 0:
                row = (api, j, w[1], w[2], "value")
            else:
                continue
            if row not in out["passes"]:
                out["passes"].append(row)
    if rec.get("exit") == "ret":
        out["exit"] = "ret"
        if rec.get("esp_delta") is not None and rec["esp_delta"] < 0x100:
            out["removed"] = rec["esp_delta"] - 4
        w = _where(rec.get("ret") or 0)
        if w and w[0] == "arg" and w[2] == 0:
            out["returns"] = ("argument", w[1])
        elif w and w[0] == "field" and w[3] == 0:
            out["returns"] = ("field", w[1], w[2])
    elif out.get("exit") != "ret":
        out["exit"] = "cut" if rec.get("truncated") else "fault"


# ---- behaving like a library function --------------------------------------------------------
def _after(rec, base, initial):
    """Buffer contents after a run: the initial bytes with the run's writes applied."""
    buf = bytearray(initial)
    for m in rec.get("mem_writes") or ():
        off = m["addr"] - base
        if 0 <= off and off + m["size"] <= len(buf):
            buf[off:off + m["size"]] = (m["value"] & ((1 << (8 * m["size"])) - 1)).to_bytes(m["size"], "little")
    return bytes(buf)


def _s32(v):
    v &= 0xFFFFFFFF
    return v - (1 << 32) if v & 0x80000000 else v


def _case(path, va, args, a, b):
    """Run once with buffer A holding `a` (padded with 0xEE) and B holding `b` (padded with
    0xDD, so a byte copied too far shows). -> (result or None when it did not return, A
    afterwards, B afterwards)"""
    a, b = a + b"\xEE" * (64 - len(a)), b + b"\xDD" * (64 - len(b))
    rec = _trace(path, va, args, [(TEXT_A, a), (TEXT_B, b)])
    if rec.get("exit") != "ret":
        return None, a, b
    return rec.get("ret") or 0, _after(rec, TEXT_A, a), _after(rec, TEXT_B, b)


def _sign(x):
    return (x > 0) - (x < 0)


def _idiom(path, va, nargs):
    """The library function `va` behaves as on the battery, or None."""
    A, B = TEXT_A, TEXT_B
    run = lambda args, a=b"", b=b"": _case(path, va, args, a, b)        # noqa: E731

    def unchanged(r, a, b):
        return r[1][:len(a)] == a and r[2][:len(b)] == b

    tests = []
    if nargs in (1, None):
        def strlen():
            return all((lambda r: r[0] == n and unchanged(r, s, b""))(run([A], s))
                       for s, n in ((b"hello\0", 5), (b"\0", 0), (b"abcdefghijk\0", 11)))

        def wcslen():
            return all((lambda r: r[0] == n)(run([A], s.encode("utf-16-le") + b"\0\0"))
                       for s, n in (("hey", 3), ("", 0), ("abcdefg", 7)))
        tests += [("strlen", strlen), ("wcslen", wcslen)]
    if nargs in (2, None):
        def strcpy():
            return all((lambda r: r[0] is not None and r[2][:len(s) + 1] == s + b"\xDD")(run([B, A], s))
                       for s in (b"hello\0", b"\0", b"a longer string\0"))

        def strcat():
            return all((lambda r: r[0] is not None and r[2][:len(d) + len(s)] == d[:-1] + s + b"\xDD")(run([B, A], s, d))
                       for s, d in ((b"cd\0", b"ab\0"), (b"xyz\0", b"\0"), (b"\0", b"q\0")))

        def compares(cases):
            return all((lambda r: r[0] is not None and _sign(_s32(r[0])) == want and unchanged(r, a, b))(run([A, B], a, b))
                       for a, b, want in cases)
        ordered = ((b"abc\0", b"abc\0", 0), (b"abc\0", b"abd\0", -1), (b"abd\0", b"abc\0", 1), (b"ab\0", b"abc\0", -1),
                   (b"\0", b"\0", 0))

        def strcmp():
            return compares(ordered + ((b"ABC\0", b"abc\0", -1),))

        def stricmp():
            return compares(ordered + ((b"ABC\0", b"abc\0", 0), (b"aBd\0", b"Abc\0", 1)))

        def strchr():
            return all((lambda r: r[0] == want)(run([A, c], b"hello\0"))
                       for c, want in ((ord("l"), A + 2), (ord("h"), A), (ord("z"), 0)))

        def strrchr():
            return all((lambda r: r[0] == want)(run([A, c], b"hello\0"))
                       for c, want in ((ord("l"), A + 3), (ord("h"), A), (ord("z"), 0)))
        tests += [("strcpy", strcpy), ("strcat", strcat), ("strcmp", strcmp), ("stricmp", stricmp),
                  ("strchr", strchr), ("strrchr", strrchr)]
    if nargs in (3, None):
        src = bytes(range(1, 33))

        def memcpy():
            return all((lambda r: r[0] is not None and r[2][:n + 1] == s[:n] + b"\xDD")(run([B, A, n], s))
                       for s, n in ((src, 7), (src, 1), (src, 16), (src, 0), (b"ab\0cdef", 6)))

        def memset():
            return all((lambda r: r[0] is not None and r[2][:n + 1] == bytes([c]) * n + b"\xDD")(run([B, c, n]))
                       for c, n in ((0x5A, 9), (0, 4), (0x11, 1), (0x7F, 0)))

        def counted(cases):
            return all((lambda r: r[0] is not None and _sign(_s32(r[0])) == want)(run([A, B, n], a, b))
                       for a, b, n, want in cases)
        sized = ((b"abcd", b"abcd", 4, 0), (b"abcd", b"abce", 4, -1), (b"abce", b"abcd", 4, 1), (b"abcd", b"abce", 3, 0))

        def memcmp():
            return counted(sized + ((b"\0b", b"\0c", 2, -1), (b"ABCD", b"abcd", 4, -1)))

        def memicmp():
            return counted(sized + ((b"\0b", b"\0c", 2, -1), (b"ABCD", b"abcd", 4, 0)))

        def strncmp():
            return counted(sized + ((b"\0b", b"\0c", 2, 0), (b"ABCD", b"abcd", 4, -1)))

        def strncpy():
            return all((lambda r: r[0] is not None and r[2][:n + 1] == (s[:n] + b"\0" * n)[:n] + b"\xDD")(run([B, A, n], s))
                       for s, n in ((b"hello\0", 3), (b"hi\0", 6), (b"abc\0", 4)))
        tests += [("strncpy", strncpy), ("memcpy", memcpy), ("memset", memset), ("memcmp", memcmp),
                  ("memicmp", memicmp), ("strncmp", strncmp)]
    for name, test in tests:
        try:
            if test():
                return name
        except ImportError:
            raise
        except Exception:
            pass
    return None


def _observe(context, item):
    """Worker: one function. item = (address, argument count, arguments known to be pointers,
    try the idiom battery)."""
    path = context
    va, nargs, pointers, battery = item
    n = min(nargs if nargs is not None else 4, MAX_ARGS)
    out = {"fields": {}, "nested": {}, "indirect": [], "passes": [], "exit": None, "removed": None, "returns": None,
           "idiom": None}
    mem = [(BUFFERS + k * 0x10000, _fill(k)) for k in range(n)]
    _digest(_trace(path, va, [BUFFERS + k * 0x10000 for k in range(n)], mem), out)
    if n and 0 < len(pointers) < n:
        _digest(_trace(path, va, [BUFFERS + k * 0x10000 if k in pointers else 0 for k in range(n)], mem), out)
    if not n or out["exit"] != "ret":
        _digest(_trace(path, va, [0] * n, []), out)
    if battery:
        out["idiom"] = _idiom(path, va, nargs)
    return out


def _run(img, limit, jobs):
    if img.kind != "pe":
        return {}
    facts = conventions.functions(img)
    pointer_args = {}
    for s in structures.recover(img):
        for r in s["roots"]:
            if r[0] == "arg":
                pointer_args.setdefault(r[1], set()).add(r[2])
    items = []
    for f in img.functions:
        fact = facts[f]
        if fact.get("shape") in ("import thunk", "jump thunk", "empty", "not code") or \
                any(r not in ("ecx",) for r in fact.get("reg_args") or ()):
            continue
        # worth the battery: small, calls nothing, loops (or is a few instructions in front of
        # the function after it, as hand-written entry points are), and takes one to three arguments
        battery = fact.get("shape") == "leaf" and not fact.get("reg_args") \
            and (fact.get("loops") or fact.get("string_ops") or len(img.function(f).insns) <= 3) \
            and (fact.get("args") is None or 1 <= fact["args"] <= 3)
        items.append((f, fact.get("args"), tuple(sorted(pointer_args.get(f, ()))), bool(battery)))
    items.sort(key=lambda it: (not it[3], not it[2], it[0]))        # the informative ones first
    if limit is not None:
        items = items[:limit]
    return pool.run(_observe, items, context=img.path, jobs=jobs, small=12)


def run(img, limit=400, jobs=4):
    """{function: observation} for up to `limit` functions (None: all of them)."""
    return img.memo(("observe", limit), lambda i: _run(i, limit, jobs))


def analyse(img, limit=400, jobs=4):
    seen = {f: o for f, o in run(img, limit, jobs).items() if o}
    facts_c = conventions.functions(img)
    funcs = set(img.functions)
    facts, new_targets, edges = [], set(), 0
    agree = differ = 0
    confirmed = added = 0
    static_fields = {}
    for s in structures.recover(img):
        for r in s["roots"]:
            if r[0] == "arg":
                static_fields[(r[1], r[2])] = s
    for f, o in sorted(seen.items()):
        fact = {"addr": f, "kind": "observed", "exit": o["exit"]}
        if o["fields"]:
            fact["fields"] = {"argument %d" % k: {("+%x" % off): c for off, c in sorted(v.items())}
                              for k, v in sorted(o["fields"].items())}
        if o["nested"]:
            fact["nested"] = {"argument %d +%x" % k: {("+%x" % off): c for off, c in sorted(v.items())}
                              for k, v in sorted(o["nested"].items())}
        if o["passes"]:
            fact["passes"] = ["%s parameter %d gets %s argument %d +%x" % (a, j, "the address of" if how == "address" else "the value at", k, off)
                              for a, j, k, off, how in o["passes"]]
        ind = []
        for pc, t in o["indirect"]:
            owner = img.owner(pc)
            if owner is None or not img.is_code(t):
                continue
            ind.append("%s at %x -> %s" % (img.label(owner), pc, img.label(t)))
            edges += 1
            if t not in funcs:
                new_targets.add(t)
        if ind:
            fact["indirect_calls"] = ind
        if o["removed"] is not None:
            fact["removed"] = o["removed"]
            want = facts_c[f].get("removes")
            if want is not None:
                agree += want == o["removed"]
                differ += want != o["removed"]
        if o["returns"]:
            fact["returns"] = o["returns"]
        if o["idiom"]:
            fact["behaves_like"] = o["idiom"]
        for k, offs in o["fields"].items():
            s = static_fields.get((f, k))
            if s:
                confirmed += sum(1 for off in offs if off in s["fields"])
                added += sum(1 for off in offs if off not in s["fields"])
        facts.append(fact)
    by_structure = {}                                   # what the runs say about each recovered structure
    for (f, k), s in static_fields.items():
        offs = seen.get(f, {}).get("fields", {}).get(k)
        if offs:
            by_structure.setdefault(s["name"], [s, set()])[1].update(offs)
    for name, (s, offs) in sorted(by_structure.items()):
        facts.append({"addr": None, "kind": "structure_observed", "name": name,
                      "fields_confirmed": sorted("+%x" % o for o in offs if o in s["fields"]),
                      "fields_added": sorted("+%x" % o for o in offs if o not in s["fields"]),
                      "fields_not_reached": sum(1 for o in s["fields"] if o not in offs)})
    idioms = sorted((o["idiom"], f) for f, o in seen.items() if o["idiom"])
    counts = {"functions_run": len(seen), "returned": sum(1 for o in seen.values() if o["exit"] == "ret"),
              "faulted": sum(1 for o in seen.values() if o["exit"] == "fault"),
              "cut_off": sum(1 for o in seen.values() if o["exit"] == "cut"),
              "with_pointer_arguments": sum(1 for o in seen.values() if o["fields"]),
              "fields_observed": sum(len(v) for o in seen.values() for v in o["fields"].values()),
              "nested_pointers_followed": sum(len(o["nested"]) for o in seen.values()),
              "static_fields_confirmed": confirmed, "fields_only_seen_running": added,
              "structures_with_a_field_confirmed": sum(1 for s, offs in by_structure.values()
                                                       if any(o in s["fields"] for o in offs)),
              "indirect_calls_resolved": edges, "function_starts_found": len(new_targets),
              "fields_passed_to_system_calls": sum(len(o["passes"]) for o in seen.values()),
              "removed_bytes_agree_with_static": agree, "removed_bytes_differ": differ,
              "library_idioms": len(idioms)}
    text = ["behaves like %s: %s" % (n, img.label(f)) for n, f in idioms[:16]]
    text += [x for fact in facts for x in fact.get("indirect_calls", [])][:8]
    if new_targets:
        text.append("function starts reached only by running: " + ", ".join("%x" % t for t in sorted(new_targets)[:8]))
    return {"counts": counts, "facts": facts, "text": text}
