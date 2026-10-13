"""win32.py -- what is particular to reconstructing a 32-bit Windows module.

One of the kinds reconstruct.py works with (see there for what a kind provides). Here: the
module is decompiled by decompile/decompiler.py, built as one unit by build/whole_module.py, and
each function is run beside the original in the 32-bit emulator and compared by verify/oracle.py
(calls and their arguments, result, memory changed, stack, saved registers, coverage).
"""
import hashlib

KIND = "pe"


def profile(module_path):
    from codebreaker.verify import pipeline
    return pipeline.profile("pe")


def prepare(module_path, work):
    from codebreaker.decompile import decompiler
    from codebreaker.verify import pipeline
    pipeline._arm()
    lane = pipeline.LANES["pe"]
    addrs = lane.functions(module_path)
    m = decompiler.decompile(module_path, addrs, work)
    m.key = {name: lane.key(va) for va, name in m.by_va.items()}
    m.va = {name: va for va, name in m.by_va.items()}
    m.all_keys = [lane.key(va) for va in addrs]
    # The module is one translation unit: any change to it may change any function's build.
    m.sig = hashlib.sha1(("%s\0%s\0%s" % (decompiler.REV, m.header, "".join(m.texts[n] for n in m.order)))
                         .encode("utf-8", "replace")).hexdigest()
    return m


def build(m, module_path, work):
    """-> (context for judge, {name: item}, {name: why it is not in the build})"""
    from codebreaker.build import whole_module
    from codebreaker.decompile import decompiler
    from codebreaker.formats.pe import open_pe
    # Only functions with a known address are built. The rest are library routines recognised
    # by signature (__allshl, _memcpy): they belong to the C runtime, which is linked.
    placed = set(m.by_va.values())
    dll, _built, dropped = whole_module.build(module_path, m.header, m.texts, [n for n in m.order if n in placed],
                                              m.imports, work, cflags=decompiler.CFLAGS)
    at, base = {}, 0
    if dll:
        pb = open_pe(dll)
        at, base = pb.export_addrs(), pb.image_base
    items, dropped = {}, dict(dropped)
    for fn in placed:
        if fn in dropped:
            continue
        if fn not in at:
            dropped[fn] = "not in the build"
        else:
            items[fn] = (m.key[fn], m.va[fn], dll, base + at[fn])
    return module_path, items, dropped


def judge(module_path, item):
    """Worker: judge one function. item = (key, address, dll, entry)."""
    from codebreaker.verify import oracle as V
    from codebreaker.verify import pipeline
    pipeline._arm()
    _key, va, dll, entry = item
    info = {}
    try:
        verdict, reason, limits = V.compare(
            lambda v: V._fp_pe(module_path, va, v),
            lambda v: V._fp_pe(dll, entry, v, rebuilt=module_path),
            V._vectors(4, "pe"), V._confirm_vectors(4, "pe"),
            insns=V.insn_count(module_path, va),
            directed=lambda cov, rch: V.directed_vectors(module_path, va, cov, reached=rch),
            info=info)
    except ImportError:
        raise
    except Exception as e:                                  # one function must not sink the rest
        verdict, reason, limits = "inconclusive", "judge: %s" % type(e).__name__, []
    return verdict, reason, list(limits), info.get("detail", "")
