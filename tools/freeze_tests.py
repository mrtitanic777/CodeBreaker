"""Record the frozen cases the tests replay (tests/frozen.py, tests/test_frozen.py).

    python tools/freeze_tests.py c_text [MODULE ...]   run the C text's passes on retail modules
                                                       and keep, for each pass, a few of the real
                                                       inputs it was given with what it made
    python tools/freeze_tests.py subjects [NAME ...]   the format readers and descriptions
                                                       (tests/frozen_subjects.py)
    python tools/freeze_tests.py all

Recording again replaces what was kept: do it only after a change meant to change the output,
and say in the commit which cases changed and why. Needs the corpus; c_text also needs the
native decompiler (native/decompiler/build/bin)."""
import functools
import json
import os
import sys
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)
sys.path.insert(0, os.path.join(ROOT, "tests"))

import frozen                                           # noqa: E402
import frozen_subjects                                  # noqa: E402
from codebreaker.decompile import c_text                # noqa: E402

# Every pass of the C text that is a function of its arguments (the ones that take the module's
# file or its description are covered by the whole-module cases below)
PASSES = ["_more_helpers",
          "_far_loads",
          "_open_arguments", "declaration", "_global_type", "_loads_dgroup",
          "_near_to_far",
          "interrupt_helper_name",
          "interrupt_helpers", "_ram_type", "call_counts", "interrupt_routines"]

# Passes handed a whole segment's bytes read only a window of them: (buffer, start, end) argument
# positions, or (buffer, start, None, length)
WINDOWS = {"_far_loads": (0, 1, 2), "_loads_dgroup": (0, 1, None, 24)}

# retail modules the passes are run on: 16-bit (with interrupts, far pointers, stack records), 32-bit
MODULES16 = ["lzexpand.dll", "keyboard.drv", "comm.drv", "system.drv", "ver.dll", "commdlg.dll", "mmsystem.dll",
             "shell.dll"]
MODULES32 = ["lz32.dll", "notepad.exe"]
WHOLE = {"lzexpand.dll", "lz32.dll"}                    # kept whole as well: every function's final text

CHANGED_PER_PASS = 10          # inputs a pass changed, kept per pass (spread over their sizes)
SAME_PER_PASS = 2              # ... and inputs it left as they were
MAX_CASE = 12000               # characters of one case's arguments, at most
LARGE = {"call_counts": 120000}  # (a pass given every function of the module at once)


def _windowed(name, args):
    spec = WINDOWS.get(name)
    if not spec:
        return args
    args = list(args)
    buf, s = spec[0], args[spec[1]]
    e = args[spec[2]] if spec[2] is not None else s + spec[3]
    args[buf] = frozen.Window(s, bytes(args[buf][s:e]))
    return args


def _record(store, name, fn):
    @functools.wraps(fn)
    def wrapper(*args, **kw):
        try:
            before = frozen.encode(list(_windowed(name, args)))
            kw_before = frozen.encode(kw)
        except TypeError:
            return fn(*args, **kw)
        result = fn(*args, **kw)
        try:
            case = {"args": before, "kwargs": kw_before, "result": frozen.encode(result)}
            after = frozen.encode(list(_windowed(name, args)))
            if after != before:
                case["args_after"] = after
        except TypeError:
            return result
        size = len(json.dumps(case))
        if size <= LARGE.get(name, MAX_CASE):
            store.setdefault(name, {}).setdefault(json.dumps(case, sort_keys=True), size)
        return result
    return wrapper


def _changed(case):
    a = case["args"]
    first = a[0] if isinstance(a, list) and a else None
    if case.get("args_after"):
        return True
    return not (isinstance(first, str) and first == case["result"])


def _choose(cases):
    """A few cases: inputs the pass changed spread over their sizes, and the smallest it left."""
    changed = sorted((sz, c) for c, sz in cases.items() if _changed(json.loads(c)))
    same = sorted((sz, c) for c, sz in cases.items() if not _changed(json.loads(c)))
    keep = []
    if changed:
        n = min(CHANGED_PER_PASS, len(changed))
        idx = sorted({round(i * (len(changed) - 1) / max(n - 1, 1)) for i in range(n)})
        keep += [changed[i][1] for i in idx]
    keep += [c for _sz, c in same[:SAME_PER_PASS]]
    return [json.loads(c) for c in keep], len(changed), len(same)


def freeze_c_text(modules):
    from codebreaker.base import corpus
    from codebreaker.decompile import decompiler
    if not decompiler.available():
        sys.exit("the native decompiler is not built (native/decompiler/build/bin)")
    store, vectors = {}, set()
    originals = {n: getattr(c_text, n) for n in PASSES}
    for n in PASSES:
        setattr(c_text, n, _record(store, n, originals[n]))
    try:
        for mod in modules:
            path = corpus.retail(mod)
            if not path:
                print("%-14s not in the corpus" % mod)
                continue
            t = time.time()
            whole = (frozen_subjects.c_text32 if mod in MODULES32 else frozen_subjects.c_text16)(path)
            if mod in WHOLE:
                frozen.save("c_text_module", mod.replace(".", "_"),
                            {"file": mod, "sha256": frozen_subjects.sha256(path), "output": whole})
            print("%-14s %4d functions  %5.1fs" % (mod, len(whole["texts"]), time.time() - t))
            vectors.update(v for v, _regs in whole.get("interrupts", ()))
        # passes the modules above do not reach, on the inputs their real callers give them:
        c_text._more_helpers(c_text._HELPERS)                 # (made once, when c_text is imported)
        if vectors:
            c_text.interrupt_routines(vectors)                # (build/module.py: the interrupts a module takes)
        _powerpc()                                            # call_counts, _homed_arguments
    finally:
        for n, fn in originals.items():
            setattr(c_text, n, fn)
    for n in PASSES:
        cases, changed, same = _choose(store.get(n, {}))
        if not cases and os.path.isfile(frozen.path("c_text", n)):
            # not reached this time (a pass the C text no longer calls itself, still called from
            # elsewhere): its earlier cases stand -- it is a function of its arguments
            print("  %-24s not reached: its earlier cases kept" % n)
            continue
        frozen.save("c_text", n, {"pass": n, "modules": modules, "cases": cases})
        print("  %-24s kept %2d of %4d changed, %4d unchanged%s" % (n, len(cases), changed, same,
                                                                    "   <- NEVER CHANGED ANYTHING" if not changed else ""))


def _powerpc():
    """The PowerPC lane, on the functions tests/test_powerpc.py compiles with the Xbox 360 compiler
    (there is no PowerPC image in the corpus)."""
    from codebreaker.build import functions
    if not functions.powerpc_tools():
        print("powerpc        no PowerPC compiler in settings.json: its passes are not recorded")
        return
    import tempfile
    import test_powerpc as T
    with tempfile.TemporaryDirectory(prefix="cbfreeze_ppc_") as work:
        order = list(T.CASES)
        dll, _where, dropped = functions.build_powerpc(T.HEADER, {n: T.CASES[n][0] for n in order}, order,
                                                       ["outside"], work, 0x93000000)
        if not dll or dropped:
            print("powerpc        build failed: %s" % (dropped or "no image"))
            return
        m = c_text.compilable_powerpc(dll, os.path.join(work, "c"))
        print("powerpc        %4d functions" % len(m.texts))
        # call_counts is given the image's imports, and this image has none it can see: given its
        # own functions instead, on the texts that call them (the smallest twenty)
        names = set(m.texts)
        callers = sorted((t for t in m.texts.values() if any(n + "(" in t.split("{", 1)[-1] for n in names)), key=len)
        c_text.call_counts({"f%d" % k: t for k, t in enumerate(callers[:20])}, names)


def freeze_subjects(names):
    for group, name, where, fn in frozen_subjects.SUBJECTS + frozen_subjects.MODULE_SUBJECTS:
        if names and name not in names and "%s/%s" % (group, name) not in names:
            continue
        path = where()
        if not path or not os.path.isfile(path):
            print("%-9s %-22s no file" % (group, name))
            continue
        t = time.time()
        frozen.save(group, name, {"file": os.path.basename(path), "sha256": frozen_subjects.sha256(path),
                                  "output": fn(path)})
        print("%-9s %-22s %5.1fs" % (group, name, time.time() - t))


def main(argv):
    os.environ["CB_NO_CACHE"] = "1"            # recorded from the code as it is, not a kept result
    if not argv or argv[0] not in ("c_text", "subjects", "all"):
        print(__doc__)
        return 2
    if argv[0] in ("c_text", "all"):
        freeze_c_text(argv[1:] if argv[0] == "c_text" and argv[1:] else MODULES16 + MODULES32)
    if argv[0] in ("subjects", "all"):
        freeze_subjects(argv[1:] if argv[0] == "subjects" else [])
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
