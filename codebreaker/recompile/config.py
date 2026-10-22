"""config.py -- the XenonRecomp configuration of a title, from what CodeBreaker finds in it.

    from codebreaker.recompile import config
    text = config.make(xex_path, out_dir="ppc", switch_tables="switch_tables.toml", hooks=None)

XenonRecomp takes a title's functions from its .pdata and follows the calls itself; what the
configuration adds is what it cannot find:

  * the compiler's register save/restore helpers (__savegprlr_14, __restgprlr_14, __savefpr_14,
    __restfpr_14, __savevmx_14/_64, __restvmx_14/_64): each run is recompiled as one function
    with an entry per register count;
  * every function .pdata leaves out (discover.py: leaf functions, those reached only through a
    pointer in the data or an address the code builds), with its length;
  * the switch tables (XenonAnalyse's, written to their own file);
  * the register settings, which must match the runtime's ppc_config.h (native/x360) since they
    change PPCContext's layout;
  * the title's hooks: `midasm_hook` tables from a hooks file, copied as they are.
"""
import bisect
import os

from codebreaker.recompile import disassemble, discover

# The registers kept as locals of each function (the runtime is built with the same settings: x360.cmake
# reads them back from the generated ppc_config.h). LR stays in the context (backtraces and unwinding
# read it), and so do the non-volatile registers: a compiler-generated __finally block is called with
# `bl` and reads its parent's r30/r31, which a local of the parent would hide.
REGISTER_SETTINGS = (("skip_lr", False), ("skip_msr", True), ("ctr_as_local", True),
                     ("xer_as_local", True), ("reserved_as_local", True), ("cr_as_local", True),
                     ("non_argument_as_local", False), ("non_volatile_as_local", False))


def helpers(img):
    """{config key: address} of the save/restore runs the image has."""
    out = {}
    runs = {name: a for a, name in img.run_names.items()}
    for key, name in (("savegprlr_14_address", "__savegprlr_14"), ("restgprlr_14_address", "__restgprlr_14"),
                      ("savefpr_14_address", "__savefpr_14"), ("restfpr_14_address", "__restfpr_14")):
        if name in runs:
            out[key] = runs[name]
    # the vector runs: li r11,-0x120 (v14) / li r11,-0x400 (v64), then stvx or lvx through r12
    lo, hi = img.code
    code = img.bytes_at(lo, hi - lo)
    for o in range(0, len(code) - 7, 4):
        w = int.from_bytes(code[o:o + 4], "big")
        if w not in (0x3960FEE0, 0x3960FC00):
            continue
        nxt = next(iter(disassemble.listing(code, lo, start=o + 4, count=1)), None)
        if nxt is None:
            continue
        mnem, ops = nxt[2], nxt[3].replace(" ", "")
        want = "v14,r11,r12" if w == 0x3960FEE0 else "v64,r11,r12"
        if ops != want:
            continue
        n = "14" if w == 0x3960FEE0 else "64"
        if mnem.startswith("stvx"):
            out.setdefault("savevmx_%s_address" % n, lo + o)
        elif mnem.startswith("lvx"):
            out.setdefault("restvmx_%s_address" % n, lo + o)
    out.update(jump_buffers(img, code))
    return out


def _start_before(code, lo, a):
    """The first word after the zero padding (or blr) that comes before `a`: a function's start."""
    o = a - lo
    while o >= 4 and int.from_bytes(code[o - 4:o], "big") not in (0, 0x4E800020):
        o -= 4
    return lo + o


def jump_buffers(img, code):
    """The CRT's setjmp and longjmp (the Xbox 360 jmp_buf: r1 at +0x90, CR at +0x130, LR at +0x134):
    setjmp stores r1 at 0x90(r3); longjmp loads LR from 0x134 and r1 from 0x90 of one register.
    XenonRecomp turns them into the host's setjmp/longjmp; without them a longjmp is a call that
    returns, and its caller goes on with registers it never restored."""
    lo, hi = img.code
    out = {}
    words = [int.from_bytes(code[o:o + 4], "big") for o in range(0, len(code) - 3, 4)]
    for i, w in enumerate(words):
        if w == 0xF8230090 and "setjmp_address" not in out:                # std r1,0x90(r3)
            out["setjmp_address"] = _start_before(code, lo, lo + 4 * i)
        # lwz rD,0x134(rA) then lwz rE,0x90(rA) (D-form, opcode 32) within a few words
        if w >> 26 == 32 and w & 0xFFFF == 0x134 and "longjmp_address" not in out:
            ra = (w >> 16) & 31
            for v in words[i + 1:i + 4]:
                if v >> 26 == 32 and v & 0xFFFF == 0x90 and (v >> 16) & 31 == ra:
                    a = lo + 4 * i
                    starts = [s for s, (n, _p, _h) in img.extents.items() if s <= a < s + n]
                    out["longjmp_address"] = starts[0] if starts else _start_before(code, lo, a)
                    break
    return out


def read_switch_tables(path):
    """{table base (the bctr): [case labels]} from a switch-table file (XenonAnalyse's format)."""
    import tomllib
    with open(path, "rb") as f:
        data = tomllib.load(f)
    return {s["base"]: list(s["labels"]) for s in data.get("switch", [])}


def write_switch_tables(path, xex_path, analysed=None):
    """The title's switch-table file: switches.py's tables (it walks back from every bctr), with
    XenonAnalyse's (`analysed`, its file's text) added for any bctr switches.py did not resolve.
    -> (switches.py's count, XenonAnalyse's added)."""
    import tomllib
    from codebreaker.recompile import switches
    found, _report = switches.find(xex_path)
    have = {t["bctr"] for t in found}
    img = discover._image(xex_path)
    lo, hi = img.code
    code = img.bytes_at(lo, hi - lo)
    added = 0
    for s in (tomllib.loads(analysed).get("switch", []) if analysed else []):
        bctr = s["base"]                     # XenonAnalyse's base is the sequence's start: find its bctr
        while bctr < hi and int.from_bytes(code[bctr - lo:bctr - lo + 4], "big") != 0x4E800420:
            bctr += 4
        if bctr in have or bctr >= hi:
            continue
        have.add(bctr)
        found.append({"bctr": s["base"], "r": s["r"], "default": s.get("default", 0), "labels": s["labels"],
                      "kind": "XenonAnalyse", "table": 0, "guard": 0, "inline": False})
        added += 1
    switches.write(path, found)
    return len(found) - added, added


def _end_of_run(code, lo, a):
    """The address after the first blr, bctr or plain branch at or after `a`."""
    for o in range(a - lo, len(code) - 3, 4):
        w = int.from_bytes(code[o:o + 4], "big")
        if w in (0x4E800020, 0x4E800420) or (w >> 26 == 18 and not w & 3):
            return lo + o + 4
    return lo + len(code)


def extra_functions(xex_path, img, switches=None):
    """[(address, size)] of the functions .pdata leaves out, sized to the next known start (a run
    of save/restore entries, and the import thunks, count as starts). A switch's case labels are
    never starts (an absolute switch table is data holding code addresses, which the pointer
    scan would take for functions), and the function that holds a switch runs to the end of
    its last case."""
    switches = switches or {}
    labels = {l for ls in switches.values() for l in ls}
    funcs, _sources, _img = discover.discover(xex_path)
    lo, hi = img.code
    code = img.bytes_at(lo, hi - lo)
    # XenonRecomp emits each save/restore run itself, an entry per register: nothing inside one is extra
    runs = [(a, _end_of_run(code, lo, a)) for a in helpers(img).values()]
    starts = [a for a in funcs if a not in labels and not any(s <= a < e for s, e in runs)]
    bounds = sorted(set(starts) | set(img.imports) | set(img.run_names) | set(img.extents) |
                    {s for s, _e in runs})
    pdata = sorted((s, s + n) for s, (n, _p, _h) in img.extents.items())
    pdata_starts = [s for s, _e in pdata]

    def containing(a):
        i = bisect.bisect_right(pdata_starts, a) - 1
        return pdata[i] if i >= 0 and pdata[i][0] < a < pdata[i][1] else None

    def size(a):
        inside = containing(a)
        if inside:                       # a second entry of a .pdata function: runs to its end
            return inside[1] - a
        i = bisect.bisect_right(bounds, a)
        end = min(bounds[i] if i < len(bounds) else hi, a + 0x4000, hi)
        for base, cases in switches.items():          # its switches' cases are inside it
            if a <= base < end:
                end = max(end, min(_end_of_run(code, lo, max(cases)), hi))
        while end - 4 > a and code[end - 4 - lo:end - lo] == b"\0\0\0\0":   # trailing padding
            end -= 4
        return end - a

    found = {}
    for a in sorted(starts):
        if a in img.extents or a in img.run_names or not lo <= a < hi:
            continue
        found[a] = size(a)
    # A switch outside every known function (between .pdata functions): its function runs from the start of
    # that gap (after the previous function's end and any padding) to the end of its last case; left to
    # XenonRecomp, the gap's analysis stops at the bctr and the cases fall outside it.
    covered = sorted([(s, e) for s, e in pdata] + [(a, a + n) for a, n in found.items()])
    covered_starts = [s for s, _e in covered]
    for bctr, cases in switches.items():
        i = bisect.bisect_right(covered_starts, bctr) - 1
        if i >= 0 and covered[i][0] <= bctr < covered[i][1]:
            continue
        start = covered[i][1] if i >= 0 else lo
        while start < bctr and code[start - lo:start - lo + 4] == b"\0\0\0\0":
            start += 4
        if start in img.extents or not lo <= start < hi:
            continue
        end = max(_end_of_run(code, lo, max(cases)), bctr + 4)
        j = bisect.bisect_right(pdata_starts, start)
        if j < len(pdata_starts):
            end = min(end, pdata_starts[j])
        found[start] = end - start
    # Every branch (b, bc; no link) out of a function is a tail call: its target must be a function too, or
    # XenonRecomp has nothing to call there. New functions have branches of their own: to a fixed point.
    known = set(img.extents) | set(found) | set(img.run_names) | set(img.imports) | {s for s, _e in runs}
    todo = [(s, e) for s, e in pdata] + [(a, a + n) for a, n in found.items()]
    while todo:
        new = []
        for s, e in todo:
            for o in range(s - lo, min(e, hi) - lo, 4):
                w = int.from_bytes(code[o:o + 4], "big")
                op = w >> 26
                if op == 18 and not w & 3:                    # b
                    d = w & 0x03FFFFFC
                    d -= 0x04000000 if d & 0x02000000 else 0
                elif op == 16 and not w & 3:                  # bc
                    d = w & 0xFFFC
                    d -= 0x10000 if d & 0x8000 else 0
                else:
                    continue
                t = lo + o + d
                if s <= t < e or t in known or t in labels or not lo <= t < hi or \
                        any(rs <= t < re for rs, re in runs):
                    continue
                known.add(t)
                found[t] = size(t)
                new.append((t, t + found[t]))
        todo = new
    return sorted(found.items())


def make(xex_path, out_dir="ppc", switch_tables="switch_tables.toml", hooks=None, file_path=None):
    """The configuration's text. `file_path` is the .xex as the configuration names it
    (default: xex_path); `hooks` is TOML text appended as it is (midasm_hook tables)."""
    img = discover._image(xex_path)
    lines = ["# XenonRecomp configuration, written by CodeBreaker (codebreaker/recompile/config.py)",
             "[main]",
             'file_path = "%s"' % (file_path or xex_path).replace("\\", "/"),
             'out_directory_path = "%s"' % out_dir.replace("\\", "/"),
             'switch_table_file_path = "%s"' % switch_tables.replace("\\", "/"),
             ""]
    lines += ["%s = %s" % (k, "true" if v else "false") for k, v in REGISTER_SETTINGS]
    lines.append("")
    for key, a in sorted(helpers(img).items()):
        lines.append("%s = 0x%08X" % (key, a))
    switch_path = os.path.join(os.path.dirname(os.path.abspath(xex_path)), switch_tables)
    extra = extra_functions(xex_path, img,
                            read_switch_tables(switch_path) if os.path.isfile(switch_path) else None)
    lines += ["", "# functions .pdata leaves out (%d)" % len(extra), "functions = ["]
    lines += ["    { address = 0x%08X, size = 0x%X }," % (a, n) for a, n in extra]
    lines += ["]", ""]
    if hooks:
        lines += [hooks.strip(), ""]
    return "\n".join(lines)
