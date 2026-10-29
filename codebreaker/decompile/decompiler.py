"""decompiler.py -- CodeBreaker's decompiler: a module's functions as C.

    from codebreaker.decompile import decompiler
    module = decompiler.decompile(module_path)
    module.header                  types, declarations and globals the functions share
    module.functions               {address: (name, C text)}, compilable with c_text.CFLAGS
    module.order                   the names, in address order

One decompiler for everything. The native program (native/decompiler) knows processors, not
file formats: this module takes the file apart with the readers in codebreaker.formats and
describes it -- where the bytes go in memory, and the facts read from the binary rather than
guessed:

  * every function's start and name, and the bytes of arguments it removes on return (its own
    `ret N`), which fixes its calling convention and argument count;
  * every import slot, the function it holds, and the same for that function (from the module
    that exports it, checked against the headers).

What comes back is then made into C a compiler accepts and that means what the original meant:
base types defined, globals bound to their addresses in the original, every function and import
declared before use with its real convention, and stack locals kept where the original had them
whenever an address to one can escape.
"""
import json
import os
import re
import subprocess

from codebreaker.decompile import c_text, describe

REV = 16                                    # bump when what is fed to / taken from the native program changes


def _exe():
    from codebreaker.base import settings
    return os.path.join(settings.get("decompiler_bin"), "decompiler.exe")


def _languages():
    # (CB_LANGUAGES_DIR: another folder of language files -- an older decompiler's, run beside
    # this one to compare the two)
    from codebreaker.base import settings
    return os.environ.get("CB_LANGUAGES_DIR") or os.path.join(settings.ROOT, "native", "decompiler", "languages")


def _structs(description):
    """"1" when the native program is to recover structures for this description (its CB_STRUCTS),
    else "0". The setting `struct_recovery`: "off", "32" (32-bit x86 only), or "all" (the default:
    16-bit code too -- there the same pass makes a far pointer of a parameter the code reaches
    memory through as one, and of two that are one pointer's halves)."""
    from codebreaker.base import settings
    want = (settings.get("struct_recovery") or "off").strip().lower()
    if want == "all":
        return "1"
    if want != "32":
        return "0"
    try:
        with open(description, encoding="utf-8", errors="replace") as f:
            for line in f:
                if line.startswith("arch\t"):
                    return "1" if line.split("\t")[1].startswith("x86:LE:32") else "0"
    except OSError:
        pass
    return "0"


def available():
    return os.path.isfile(_exe()) and os.path.isdir(_languages())


LARGE = 200000          # instructions a function may have, tried again when the first limit stops it


def _l2_key():
    """What the L2 notes add to a cache key: a text made with them is another text. (Named "l2 2":
    the whole-module run once left it out, and per-function texts without notes were kept under
    the first name.)"""
    return ["l2 2"] if os.environ.get("CB_L2") else []


def _segment_images(description):
    """{segment: bytes} of a 16-bit module's code, from its description's `bytes` lines (describe16)."""
    from codebreaker.decompile import describe
    images = {}
    with open(description, encoding="utf-8", errors="replace") as f:
        for ln in f:
            p = ln.rstrip("\n").split("\t")
            if p[0] == "bytes" and len(p) >= 5:
                with open(p[2], "rb") as fh:
                    fh.seek(int(p[3], 16))
                    images[describe.where(int(p[1], 16))[0]] = fh.read(int(p[4], 16))
    return images


# What the native program says of a function besides its C: notes on lines of their own before it
# (decompiler.cc decompileOne). They are read here, once, as the native program's output is taken
# in -- the C handed on holds none of them -- and become the function's facts (split_notes):
#   /*@G <va> <bytes> <bytes pointed at> */     a global it touches     -> "globals": [[va, bytes, elem]]
#   /*@P <name> <bytes> */                      a pointer and its size  -> "pointers": {name: bytes}
#                                               (16-bit; `return` its result)
#   /*@S <name> <bytes> <off>:<bytes>:<type>... */  a structure it recovered
#                                               -> "structs": [[name, bytes, [[off, bytes, type], ...]]]
#   /*@L2 {...} */                              its L2 document (CB_L2) -> "l2"
# and, for C the back end wrote from that document, "from_l2": True.
_NOTE = re.compile(r"/\*@(G|P|S|L2) (.*) \*/")
_STRUCT_FIELD = re.compile(r"([0-9a-f]+):(\d+):([\w*^]+)")


def split_notes(text):
    """(the C, its facts) of one function as the native program wrote it (see _NOTE)."""
    facts = {"globals": [], "pointers": {}, "structs": [], "l2": None}
    lines = text.split("\n")
    k = 0
    while k < len(lines):
        m = _NOTE.fullmatch(lines[k].strip())
        if not m:
            break
        kind, rest = m.groups()
        if kind == "L2":
            facts["l2"] = json.loads(rest)
        elif kind == "S":
            name, size, *fields = rest.split()
            facts["structs"].append([name, int(size), [[int(o, 16), int(z), t] for o, z, t in
                                                       (_STRUCT_FIELD.fullmatch(f).groups() for f in fields)]])
        else:
            parts = rest.split()
            if kind == "G":
                facts["globals"].append([int(parts[0], 16), int(parts[1]), int(parts[2]) if len(parts) > 2 else 0])
            else:
                facts["pointers"].setdefault(parts[0], int(parts[1]))
        k += 1
    return "\n".join(lines[k:]), facts


class Decompiled(tuple):
    """run()'s (texts, order, by_va, stderr) -- the texts C alone -- and `notes`: {name: its facts}."""
    notes = None

    @staticmethod
    def of(texts, order, by_va, err, notes):
        r = Decompiled((texts, order, by_va, err))
        r.notes = notes
        return r


def c_from_l2(text, facts, images=None):
    """A function's C written by the hub's C back end from its L2 document (facts["l2"];
    translate/backends/c.py, docs/IR_UNIFICATION.md step 2) in place of the C the decompiler
    printed (`text`). Without a document, or one the back end declines, the printed C stays.
    With `images` ({segment: code}, CB_ASM_BLOCKS=1) a 16-bit function's interrupts are first
    made the _asm blocks its instructions say (decompile/asm_blocks.py), and facts["l2"] is the
    document so placed (the module's L2 tree is made of them: build/module.module_document).
    facts["from_l2"] says which C it is."""
    from codebreaker.translate.backends import c as cback
    doc = facts.get("l2")
    if doc is None:
        return text
    origin = doc.get("origin") or {}
    if images and origin.get("segment") in images:
        from codebreaker.decompile import asm_blocks
        code = images[origin["segment"]]
        doc, _said = asm_blocks.place(doc, code, origin["offset"], len(code), seg=origin["segment"])
        facts["l2"] = doc
    try:
        body = cback.function(doc, cback.MOVED)  # (its far pointers: L2's types say which)
    except (cback.Decline, ValueError, KeyError):
        return text
    facts["from_l2"] = True
    return "\n" + body


def run(description, only=(), timeout=3600, max_instructions=None, l2=None):
    """As _run, the notes taken apart from the C (split_notes) -> Decompiled. With `l2` each
    function's C is the hub's C back end's, from its L2 (c_from_l2): the x86 lanes that make
    compilable C ask for it (decompile16, compilable32); for the others CB_C_FROM_L2=1 says."""
    l2 = bool(os.environ.get("CB_C_FROM_L2")) if l2 is None else l2
    was = os.environ.get("CB_L2")
    if l2:
        os.environ["CB_L2"] = "1"
    try:
        out, order, by_va, err = _run(description, only, timeout, max_instructions)
    finally:
        if l2 and was is None:
            del os.environ["CB_L2"]
        elif l2:
            os.environ["CB_L2"] = was
    images = _segment_images(description) if l2 and os.environ.get("CB_ASM_BLOCKS") else None
    texts, notes = {}, {}
    for n, t in out.items():
        texts[n], notes[n] = split_notes(t)
        if l2:
            texts[n] = c_from_l2(texts[n], notes[n], images)
    return Decompiled.of(texts, order, by_va, err, notes)


def _run(description, only=(), timeout=3600, max_instructions=None):
    """Run the native program on a description. -> ({name: text}, [names], {va: name}, stderr)
    Works for any processor it has a language for; `only` limits it to those addresses.

    Each function's text is kept on its own (base/cache.py) with the lines of the description
    it was made from: its own line, the calls through pointers inside it, and the line of every
    function and import its text calls (or that there was none, for a `func_0x...` it calls).
    A function whose lines are all as they were is not decompiled again: a change to the
    description -- one function found to take its arguments off by hand -- has only the
    functions it touches made again."""
    from codebreaker.base import cache
    with open(description, encoding="utf-8", errors="replace") as f:
        lines = f.read().splitlines()
    fn_line, name_line, o_lines, base = {}, {}, [], []
    for ln in lines:
        p = ln.split("\t")
        if p[0] == "F" and len(p) > 2:
            fn_line[int(p[1], 16)] = ln
            name_line[p[2]] = ln
        elif p[0] == "X" and len(p) > 2:
            name_line[p[2]] = ln
            base.append(ln)                                  # (an import: every caller's as well)
        elif p[0] == "O" and len(p) > 2:
            o_lines.append((int(p[1], 16), ln))
        elif p[0] == "bytes" and len(p) > 2:
            with open(p[2], "rb") as fh:
                base.append("bytes\t%s\t%s" % (p[1], cache.key(fh.read())))
        else:
            base.append(ln)
    if not fn_line:
        return _run_whole(description, only, timeout, max_instructions)
    common = cache.key(cache.tool(_exe()), _language_stamp(), base, max_instructions or "", _structs(description),
                       *_l2_key())   # (with the L2 notes: another text)
    starts = sorted(fn_line)
    import bisect

    def own(va):
        k = bisect.bisect_right(starts, va)
        end = starts[k] if k < len(starts) and starts[k] >> 16 == va >> 16 else ((va >> 16) + 1) << 16
        return [ln for site, ln in o_lines if va <= site < end]

    wanted = sorted(only) if only else starts
    reused, misses = {}, []
    for va in wanted:
        if va not in fn_line:
            misses.append(va)
            continue
        had = cache.get("decompiled_fn", cache.key(common, fn_line[va], own(va)))
        if had is None:
            misses.append(va)
            continue
        entry = json.loads(had.decode("utf-8"))
        if all(name_line.get(n) == ln for n, ln in entry["deps"].items()) and \
                all((int(v, 16) in fn_line) == present for v, present in entry["unknown"].items()):
            reused[va] = entry
        else:
            misses.append(va)
    out, order, by_va, stderr = ({}, [], {}, "") if not misses else \
        _run_whole(description, misses if (only or len(misses) < len(starts)) else (), timeout, max_instructions)
    for va, name in by_va.items():                           # (kept for the next time, with their lines)
        if va not in fn_line:
            continue
        text = out.get(name, "")
        deps = {n: name_line[n] for n in set(re.findall(r"\b([A-Za-z_]\w*)\s*\(", text))
                if n in name_line and n != name}
        unknown = {"%08x" % int(v, 16): False for v in re.findall(r"\bfunc_0x([0-9a-f]{8})\b", text)}
        cache.put("decompiled_fn", cache.key(common, fn_line[va], own(va)),
                  json.dumps({"name": name, "text": text, "deps": deps, "unknown": unknown}).encode("utf-8"))
    for va, entry in reused.items():
        out[entry["name"]] = entry["text"]
        by_va[va] = entry["name"]
    order = [by_va[va] for va in sorted(by_va)]
    return out, order, by_va, stderr


def _run_whole(description, only=(), timeout=3600, max_instructions=None):
    """(run(), for the functions not kept: the native program on the description as a whole)"""
    # The description holds the module's bytes and every fact given with them, so what the
    # program says of it is kept under its hash (base/cache.py): a module is decompiled once.
    from codebreaker.base import cache
    with open(description, "rb") as f:
        text = f.read()
    # (by what the files it names hold, not where they are: the same module described in
    # another build folder is the same description)
    files = {}

    def _content(mm):
        p = mm.group(2).decode("utf-8", "replace")
        if p not in files:
            with open(p, "rb") as fh:
                files[p] = cache.key(fh.read())
        return mm.group(1) + files[p].encode() + mm.group(3)
    text = re.sub(rb"(?m)^(bytes\t[0-9a-fA-F]+\t)([^\t\r\n]+)(\t)", _content, text)
    k = cache.key(cache.tool(_exe()), _language_stamp(), text, ["%x" % va for va in only],
                  max_instructions or "", _structs(description),
                  *_l2_key())   # (with the L2 notes: another text)
    had = cache.get("decompiled", k)
    if had is not None:
        stdout, stderr = had.decode("utf-8", "replace").split("\x00", 1)
    else:
        stdout, stderr, whole = _run_resuming(description, only, timeout, max_instructions)
        if whole and stdout:
            cache.put("decompiled", k, (stdout + "\x00" + stderr).encode("utf-8", "replace"))
    out, order, by_va, large = {}, [], {}, []
    for m in re.finditer(r"(?ms)^/\*@@ (\S+) ([0-9a-f]+) \*/\n(.*?)(?=^/\*@@ |\Z)", stdout):
        name, va, text = m.group(1), int(m.group(2), 16), m.group(3).strip()
        if text.startswith("/* decompile failed: Flow exceeded maximum allowable instructions"):
            large.append(va)
        if not text or text.startswith("/* decompile failed"):
            continue
        if name not in out:
            out[name] = text
            order.append(name)
            by_va[va] = name
    if large and not max_instructions:                   # longer than a wrong start would run: again, with room
        more, more_order, more_va, _e = _run(description, only=large, timeout=timeout, max_instructions=LARGE)
        out.update(more)
        order += [n for n in more_order if n not in order]
        by_va.update(more_va)
    return out, order, by_va, stderr


_MARK = re.compile(r"(?m)^/\*@@ \S+ ([0-9a-f]+) \*/")


def _run_resuming(description, only, timeout, max_instructions=None):
    """Run the native program; when it dies part way (a crash that depends on what it did
    before -- the function it died on decompiles on its own), start it again from the next
    function it had not written, and so on. A function a fresh start dies on at once is left
    out, with a note on stderr. -> (stdout, stderr, whether every function was attempted)"""
    from codebreaker.base import settings
    # CB_THREADS (setting "threads"): functions analysed at once in the one process, by hand;
    # unset, one thread, and the functions are shared among processes instead (below)
    env = dict(os.environ, CB_LANGUAGES=_languages(), CB_THREADS=str(settings.get("threads") or 1),
               CB_STRUCTS=_structs(description))      # (structures: see _structs)
    if max_instructions:
        env["CB_MAX_INSTRUCTIONS"] = str(max_instructions)
    if only:
        wanted = sorted(only)
    else:
        with open(description, encoding="utf-8", errors="replace") as f:
            wanted = sorted({int(line.split("\t")[1], 16) for line in f if line.startswith("F\t")})
    if not wanted:                                       # (no code: a font or resource-only module)
        return "", "", True
    if settings.get("threads"):                          # (set by hand: as it says)
        return _resuming(description, only, timeout, env, wanted)
    if len(wanted) <= _CHUNK:
        return _resuming(description, only, timeout, env, wanted)
    # More than a chunk: the functions in fixed chunks of _CHUNK, in address order, each chunk
    # a process of its own on one thread, as many at once as the governor gives slots and memory
    # for (base/parallel.py). Not the native program's threads: each keeps its own analysis, and
    # the globals one function types stay typed for the next -- so with threads a function's text
    # depended on which thread it fell to (HC31's sub_1_AF04 came out two ways in two runs). A
    # chunk is always the same functions in the same order, so the texts are the same however
    # many run at once, on any machine. (Against the whole on one thread, as before: HC31 1 of 635
    # functions typed otherwise, sub_1_AF04; COMPOBJ and P3 none. 2-3 times as fast on 4 slots.)
    from codebreaker.base import parallel
    from codebreaker.jobs import pool
    chunks = [wanted[i:i + _CHUNK] for i in range(0, len(wanted), _CHUNK)]
    with parallel.slots(want=len(chunks), mb_each=_PROCESS_MB, label="decompile") as n:
        parts = pool.threads(lambda c: _resuming(description, c, timeout, env, c), chunks, jobs=n)
    blocks = sorted((int(m.group(1), 16), m.group(0)) for p in parts for m in _BLOCK.finditer(p[0]))
    return "".join(b for _va, b in blocks), "".join(p[1] for p in parts)[-2000:], all(p[2] for p in parts)


_BLOCK = re.compile(r"(?ms)^/\*@@ \S+ ([0-9a-f]+) \*/\n.*?(?=^/\*@@ |\Z)")
_CHUNK = 64             # functions to a process
_PROCESS_MB = 1536      # what the governor counts for each: the native program's own budget (CB_MEMORY_MB) --
                        # mostly ~60 MB, but a wrong function start can run to it


def _resuming(description, only, timeout, env, wanted):
    """(_run_resuming, once the threads are settled)"""
    first = True
    outs, errs, left = [], [], list(wanted)
    while True:
        cmd = [_exe(), description] + (["--at"] + ["%x" % va for va in left] if (only or not first) else [])
        r = subprocess.run(cmd, capture_output=True, text=True, errors="replace", timeout=timeout, env=env)
        out = r.stdout or ""
        outs.append(out)
        errs.append((r.stderr or "")[-1000:])
        if r.returncode == 0:
            return "".join(outs), "".join(errs)[-2000:], True
        done = {int(m.group(1), 16) for m in _MARK.finditer(out)}
        # (the last one written may be cut off by the crash: it is done again)
        last = max(done) if done else None
        if int(env.get("CB_THREADS") or 1) > 1:
            # Several threads at work: what ended the process was any of the functions in hand,
            # not the last one written -- P3's 1BCE4 went over its budget while 1BC01 was being
            # written, and the rule below, taking 1BC01 for the culprit, left it out. What is left
            # is done on one thread, where the last written is the one in hand.
            env = dict(env, CB_THREADS="1")
            if not os.environ.get("CB_MEMORY_MB"):
                env.pop("CB_MEMORY_MB", None)
            left = [va for va in left if va not in done or va == last]
            if done:
                outs[-1] = out[:list(_MARK.finditer(out))[-1].start()]
            first = False
            continue
        # the governor stopped it on one function (over its memory budget or its time): that one
        # is done -- failed, as written -- and is not tried again
        governed = re.search(r"/\* decompile failed: over the (?:memory budget|time limit)[^*]*\*/\s*$", out)
        if governed:
            errs.append("\n[%x stopped by the governor: %s]\n" % (last, re.search(r"over the [^*]*", governed.group(0)).group(0).strip()))
        left = [va for va in left if va not in done or (va == last and not governed)]
        if done and last is not None and not governed:
            outs[-1] = out[:list(_MARK.finditer(out))[-1].start()]
        if not done or done == {last} and left and left[0] == last:
            dead = left.pop(0)                               # dies at once on this one: left out
            errs.append("\n[decompiler died on %x; left out]\n" % dead)
        first = False
        if not left:
            return "".join(outs), "".join(errs)[-2000:], False


_STAMP = []


def _language_stamp():
    if not _STAMP:
        from codebreaker.base import cache
        _STAMP.append(cache.folder(_languages()))
    return _STAMP[0]


def generation():
    """The version of what makes decompiled C, as the cache keeps its results apart
    (base/cache.py GENERATIONS): REV, the native program and its languages. (Not those of
    CB_LANGUAGES_DIR: a run comparing another folder of them shares the generation of now.)"""
    from codebreaker.base import cache
    from codebreaker.base import settings
    languages = _language_stamp() if not os.environ.get("CB_LANGUAGES_DIR") else \
        cache.folder(os.path.join(settings.ROOT, "native", "decompiler", "languages"))
    return REV, cache.tool(_exe()), languages


def raw(module_path, funcs, outdir, timeout=3600, l2=None):
    """The native program's own output for a PE module, before it is made compilable (`l2`: run's)."""
    description, _names = describe.describe32(module_path, funcs, outdir)
    return run(description, timeout=timeout, l2=l2)


def decompile(module_path, funcs=None, outdir=None):
    """Decompile a module (every function, or the addresses in `funcs`). -> Module"""
    from codebreaker.build import watcom as B
    from codebreaker.verify.lanes import win32 as pipeline
    if funcs is None:
        funcs = pipeline.LANES["pe"].functions(module_path)
    outdir = outdir or B._scratch("decompiler", os.path.basename(module_path).lower())
    return c_text.Module(*c_text.compilable32(module_path, funcs, outdir))


# 16-bit modules ---------------------------------------------------------------------------------
_CALLED = re.compile(r"\bfunc_0x([0-9a-f]{8})\s*\(")


def decompile16(module_path, outdir=None, rounds=4):
    """Decompile a 16-bit Windows module. -> ({name: text}, [names], {address: name}, Description)

    Function starts come first from what the file states (entry table, relocations, agreed
    near-call targets). Decompiling then shows calls to places not yet known as functions
    (`func_0x...`); those are added and the module is done again, until no new ones appear."""
    from codebreaker.build import watcom as B
    outdir = outdir or B._scratch("decompiler16", os.path.basename(module_path).lower())
    d = describe.describe16(module_path, outdir)
    got = run(d.path, l2=True)
    out, order, by_va, _err = got
    notes = dict(got.notes)                                  # {name: its facts} (split_notes)
    for _ in range(rounds):
        new = {int(x, 16) for t in out.values() for x in _CALLED.findall(t)} - set(d.names)
        new = {va for va in new if describe.inside(d, va)}
        if not new:
            break
        for va in sorted(new):
            seg, off = describe.where(va)
            d.names[va], d.where[va] = "sub_%d_%04X" % (seg, off), (seg, off)
            d.lines.append(describe.model_line(d, va, d.names[va]))
        d.write()
        # the new ones -- and again every function that calls one: it was made while the callee
        # was unknown, a default near function (a far call's pushed CS read as its first
        # argument, the last one lost); now its prototype is known
        callers = {va for va, n in by_va.items()
                   if any("func_0x%08x" % c in out.get(n, "") for c in new)}
        more = run(d.path, only=sorted(new | callers), l2=True)
        out.update(more[0])
        notes.update(more.notes)
        by_va.update(more[2])
    # A function its callers clean up after (cdecl) has its arguments left to the analysis, which
    # decides at each call on its own: SETUPX's allocator 24:0F1E takes a size, and 11 of its 25
    # callers were made to pass none (sub_3_7E58's block got whatever the stack held: Setup's
    # SU99405). Where a call passes fewer than the function itself takes, the function's own
    # argument words are stated, and it and its callers are done again. (Stated for every such
    # function, all 578 of SETUPX's texts changed, their pointers made words, and new faults.)
    lost = _arguments_lost(out, by_va, d, notes)
    if lost:
        _state_arguments(d, lost)
        names = {by_va[va] for va in lost}
        callers = {va for va, n in by_va.items()
                   if any(re.search(r"\b%s\s*\(" % re.escape(c), out.get(n, "")) for c in names if c != n)}
        more = run(d.path, only=sorted(set(lost) | callers), l2=True)
        # (and of what is done again, only the callers that passed too few are taken: the rest --
        # the stated functions themselves, callers that passed all -- kept as they were. Taken
        # whole, the analysis's second look made other mistakes: 25:0F96 lost one of its own two
        # arguments, 30:00F6 wrote through a pointer it had read right before)
        short = {n for va in callers for n in [by_va[va]]
                 if any(any(k < own for k in _call_arguments(out.get(n, ""), by_va[c]))
                        for c, own in ((c, len(lost[c])) for c in lost) if by_va[c] != n)}
        # (and one whose own arguments the second look changed is not taken either: 25:0F96, two
        # words become one, faulted -- its calls passed in full, its own frame read wrong)
        taken = {n for n, tx in more[0].items() if n in short and
                 _own_arguments(tx, n, more.notes[n]) == _own_arguments(out.get(n, ""), n, notes.get(n))}
        out.update({n: more[0][n] for n in taken})
        notes.update({n: more.notes[n] for n in taken})
    # a helper made part of its callers (describe.register_helper: `inline`) is still a function of
    # the module, at its place, for whatever else reaches it: its own text, from the description
    # with it as an ordinary function
    if getattr(d, "inlined", None):
        own = describe.Description(os.path.join(os.path.dirname(d.path), "description_own.txt"), {}, {}, [])
        for ln in d.lines:
            p = ln.split("\t")
            if p[0] == "F" and len(p) > 4 and p[4].strip() == "inline" and int(p[1], 16) in d.inlined:
                ln = describe.model_line(d, int(p[1], 16), p[2])
            own.lines.append(ln)
        more = run(own.write(), only=sorted(d.inlined), l2=True)
        out.update(more[0])
        notes.update(more.notes)
        by_va.update(more[2])
    out = _by_their_names(out, d.names)
    for facts in notes.values():                             # (its L2 document's calls by those names too)
        if facts.get("l2") is not None:
            facts["l2"] = _by_their_names({"": json.dumps(facts["l2"])}, d.names)[""]
            facts["l2"] = json.loads(facts["l2"])
    d.notes = {n: notes[n] for n in out if n in notes}
    order = [by_va[va] for va in sorted(by_va)]
    return out, order, by_va, d


def _call_arguments(text, name):
    """The number of arguments each call to `name` in `text` passes."""
    out = []
    for m in re.finditer(r"\b%s\s*\(" % re.escape(name), text):
        depth, k, n, seen = 1, m.end(), 0, False
        while k < len(text) and depth:
            ch = text[k]
            if ch in "([":
                depth += 1
            elif ch in ")]":
                depth -= 1
            elif ch == "," and depth == 1:
                n += 1
            if depth and not ch.isspace():
                seen = True
            k += 1
        out.append(n + 1 if seen else 0)
    return out


def _own_arguments(text, name, facts=None):
    """(count, [bytes of each]) of the arguments a function's own text declares, or None
    (`facts`: its facts, split_notes -- which pointers are four bytes)."""
    head = text.split("{", 1)[0]
    m = re.search(r"\b%s\(([^)]*)\)" % re.escape(name), head)
    if not m:
        return None
    # (the analysis's own text: `type name` in the list, and a far pointer as its facts say --
    # `char *param_1` alone reads as a near one)
    pointers = (facts or {}).get("pointers") or {}
    params = [p.strip() for p in m.group(1).split(",") if p.strip() and p.strip() != "void"]
    sizes = []
    for p in params:
        name_ = re.findall(r"\w+", p)[-1]
        far = pointers.get(name_) == 4
        wide = re.search(r"\b(u?int4|xunknown4|float4|undefined4)\b", p)
        # (a far pointer stated as one: `4p` -- stated as a 4-byte number, 34:0146's string and
        # 34:016A's copy were dereferenced as near pointers, into the data segment)
        sizes.append("4p" if far or ("__far" in p and "*" in p) else "4" if wide else "2")
    return len(params), sizes


def _own_if_read(out, va, name, d, notes=None):
    """A cdecl function's own arguments' sizes, when they come to the bytes its instructions
    read through BP (describe.args_read); otherwise None -- its own text is wrong too (SETUPX's
    25:0F96 once declared one word and read a far pointer)."""
    text = out.get(name, "")
    if "__cdecl16" not in text.split("{", 1)[0]:
        return None
    own = _own_arguments(text, name, (notes or {}).get(name))
    if not own or not own[0]:
        return None
    if d is not None:
        seg, off = describe.where(va)
        code = d.segment_bytes.get(seg, b"")
        later = [o for v in d.names for (sg, o) in [describe.where(v)] if sg == seg and o > off]
        read = describe.args_read(code, off, min(later) if later else len(code),
                                  "far" if "16far" in text.split("{", 1)[0] else "near")
        if read is None or read != sum(int(s.rstrip("p")) for s in own[1]):
            return None
    return own


def _arguments_lost(out, by_va, d=None, notes=None):
    """{address: [bytes of each argument]} of the cdecl functions some call passes fewer
    arguments than they themselves take (and whose own arguments are what they read: stated
    from a wrong text, the function was made worse)."""
    lost = {}
    for va, name in by_va.items():
        own = _own_if_read(out, va, name, d, notes)
        if own is None:
            continue
        for other, t in out.items():
            if other != name and name in t and any(n < own[0] for n in _call_arguments(t, name)):
                lost[va] = own[1]
                break
    return lost


def _state_arguments(d, lost):
    """The description's lines for these functions given their arguments' sizes (its 7th field,
    `4,2,2`: a far pointer one argument of four bytes, not two words -- as words, 34:0146's
    pointer was put back together as a near one, and SUWIN faulted in it)."""
    lines = d.lines
    for i, line in enumerate(lines):
        parts = line.split("\t")
        if parts[0] == "F" and len(parts) >= 5 and int(parts[1], 16) in lost:
            while len(parts) < 6:
                parts.append("0")
            parts[6:] = [",".join(str(s) for s in lost[int(parts[1], 16)])]
            lines[i] = "\t".join(parts)
    d.write()


def _by_their_names(out, names):
    """A function found only because another called it was decompiled after its callers, whose
    text still calls it `func_0x<address>` -- a name nothing has. The callers are given its name."""
    known = {"%08x" % va: name for va, name in names.items()}
    if not known:
        return out
    placeholder = re.compile(r"\bfunc_0x([0-9a-f]{8})\b")
    return {n: placeholder.sub(lambda m: known.get(m.group(1), m.group(0)), t) for n, t in out.items()}


def _grow(d, line_for, rounds=4):
    """Decompile a description; then, while the output calls places not yet known as functions
    (`func_0x...`), add those inside the module's code and decompile them too."""
    out, order, by_va, _err = run(d.path)
    for _ in range(rounds):
        new = {int(x, 16) for t in out.values() for x in _CALLED.findall(t)} - set(d.names)
        new = {va for va in new if any(lo <= va < lo + n for lo, n in d.spans.values())}
        if not new:
            break
        for va in sorted(new):
            name, line = line_for(va)
            d.names[va] = name
            d.lines.append(line)
        d.write()
        callers = {va for va, n in by_va.items()                 # (made while these were unknown)
                   if any("func_0x%08x" % c in out.get(n, "") for c in new)}
        more, _o, more_va, _err = run(d.path, only=sorted(new | callers))
        out.update(more)
        by_va.update(more_va)
    out = _by_their_names(out, d.names)
    return out, [by_va[va] for va in sorted(by_va)], by_va


def decompile_le(module_path, outdir=None):
    """Decompile a virtual device driver file: one LE image, or every member of a container.
    -> [(member name, {name: text}, [names], Description)]"""
    from codebreaker.build import watcom as B
    from codebreaker.formats import le
    outdir = outdir or B._scratch("decompiler_le", os.path.basename(module_path).lower())
    results = []
    for name, lf in le.open_vxds(module_path):
        d = describe.describe_le(lf, name, os.path.join(outdir, re.sub(r"\W", "_", name)))

        def line(va):
            n = "sub_%d_%05X" % ((va - describe.LE_BASE) // describe.LE_STRIDE + 1, (va - describe.LE_BASE) % describe.LE_STRIDE)
            return n, "F\t%x\t%s\t-1" % (va, n)
        out, order, _by_va = _grow(d, line)
        results.append((d.device, out, order, d))
    return results


def decompile_dos(module_path, outdir=None):
    """Decompile a DOS program. -> ({name: text}, [names], Description)"""
    from codebreaker.build import watcom as B
    outdir = outdir or B._scratch("decompiler_dos", os.path.basename(module_path).lower())
    d = describe.describe_dos(module_path, outdir)

    def line(va):
        kind, _pop = describe.returns(d.image, va - d.base)
        n = "sub_%05X" % (va - d.base)
        return n, "F\t%x\t%s\t-1\t__cdecl16%s" % (va, n, kind or "near")
    out, order, _by_va = _grow(d, line, rounds=8)
    from codebreaker.symbols import dos_interrupts
    out = {n: dos_interrupts.annotate(t, d.interrupts.get(n, [])) for n, t in out.items()}
    return out, order, d


def _is_x86(module_path):
    from codebreaker.formats.pe import open_pe
    return open_pe(module_path).machine == 0x014C


def as_c(path):
    """Any executable as C: (text, functions decompiled, functions found). Raises ValueError for
    a file that is not an executable this can read."""
    from codebreaker.formats import detect, nes
    kind = "nes" if nes.is_nes(path) else detect.kind(path)
    if kind == "ne":
        texts, order, _by_va, d = decompile16(path)
        header, total = "", len(d.names)
    elif kind == "pe" and _is_x86(path):
        m = decompile(path)
        texts, order, header, total = m.texts, m.order, m.header, len(m.by_va)
    elif kind == "pe":                                       # PowerPC: as the decompiler gives it
        from codebreaker.build import watcom as B
        starts = describe.function_starts(path)
        texts, order, _by_va, _err = raw(path, starts, B._scratch("decompiler", os.path.basename(path).lower()))
        header, total = "", len(starts)
    elif kind == "nes":                                      # the 6502, as the decompiler gives it
        from codebreaker.build import watcom as B
        description, by_va = describe.describe_nes(path, B._scratch("decompiler_nes", os.path.basename(path).lower()))
        texts, order, _by_va, _err = run(description)
        header, total = "", len(by_va)
    elif kind == "le":
        texts, order, header, total = {}, [], "", 0
        for member, out, names, d in decompile_le(path):
            total += len(d.names)
            for n in names:
                key = "%s.%s" % (member, n)
                texts[key] = "/* driver %s */\n%s" % (member, out[n])
                order.append(key)
    elif kind == "dos":
        from codebreaker.formats import dos
        with open(path, "rb") as f:
            data = f.read()
        parts = dos.chain(data)
        if not parts:
            texts, order, d = decompile_dos(path)
            header, total = "", len(d.names)
        else:                                                # programs one after another: each its own
            from codebreaker.build import watcom as B
            texts, order, header, total = {}, [], "", 0
            stem = os.path.basename(path).lower()
            for k, (at, size) in enumerate(parts, 1):
                part = B._scratch("decompiler_dos", "%s.part%d" % (stem, k), "%s.part%d.exe" % (stem, k))
                os.makedirs(os.path.dirname(part), exist_ok=True)
                with open(part, "wb") as f:
                    f.write(data[at:at + size])
                out, names, d = decompile_dos(part)
                total += len(d.names)
                for n in names:
                    key = "part%d.%s" % (k, n)
                    texts[key] = "/* part %d of %d: offset %Xh, %d bytes */\n%s" % (k, len(parts), at, size, out[n])
                    order.append(key)
    else:
        raise ValueError("%s: not an executable this can read" % path)
    texts = {n: t.strip() for n, t in texts.items()}
    text = "/* %s -- decompiled by CodeBreaker; not built, not judged. */\n\n" % os.path.basename(path) + \
        (header + "\n\n" if header else "") + "\n\n".join(texts[n] for n in order) + "\n"
    return text, len(order), total


def main(argv):
    """python -m codebreaker decompile <module> [--out FILE]

    Decompile any module the corpus or a path names -- a 32-bit or 16-bit Windows module, a
    virtual device driver, a DOS program, a PowerPC image, a NES cartridge -- and write its functions as C. Nothing is built or judged; `module` does that."""
    if not argv:
        print(main.__doc__)
        return 2
    from codebreaker.base import corpus
    path = corpus.retail(argv[0])
    if not path:
        raise SystemExit("no module %r" % argv[0])
    try:
        text, decompiled, total = as_c(path)
    except ValueError as e:
        raise SystemExit(str(e))
    proj = None
    if "--out" in argv:
        out = argv[argv.index("--out") + 1]
    else:                                    # the binary's project folder: Converted/<name>/src/
        from codebreaker.base import converted
        proj = converted.project(path)
        out = os.path.join(proj.src, os.path.splitext(os.path.basename(path))[0].lower() + ".c")
    with open(out, "w", encoding="utf-8", newline="\n") as f:
        f.write(text)
    if proj:
        proj.record("decompile", [out], notes="%d of %d functions as C (readable; not built or judged)"
                    % (decompiled, total))
    print("%s: %d of %d functions -> %s" % (os.path.basename(path), decompiled, total, out))
    return 0
