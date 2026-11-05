r"""msc6.py -- the Microsoft C 6.0a model: one recursive pass that runs register-value tracking,
control-flow recovery (if/else, do-while, short-circuit and materialised conditions) and the
statement templates (calls far, near, cdecl and through a COM vtable; far pointers; longs; globals;
stack locals) together over a whole function, and declines at the first instruction it does not
model.  It reads the frames of Microsoft C 6.0a and of Visual C++ 1.52c (mov ax,ds and near-loadds
prologues, the lea / sub bp teardowns).

The model is C++ (native/invert/msc6.cc, cbinvert.dll; docs/NATIVE.md, "the compiler models"); this
keeps its Python API.

    python -m codebreaker invert --tool msc6      # the self-test against the real compiler
"""
from codebreaker.invert import native_invert
from codebreaker.invert.msc6_function import _mask


def invert(code, imports=None, syms=None, rtype="int", params=None, nearmap=None, func_base=0):
    """(C text, meta) for a function's bytes, or (None, the reason it declined).  imports {k: name} names
    the k-th far call, syms {offset: (name, 'long'|'word')} the DGROUP variables, params [(name,
    'int'|'far'|'long')] the signature (None: read from the body and retf N), nearmap {offset: name} the
    same-segment callees, func_base the function's offset in its segment."""
    return native_invert.msc6(code, imports or {}, syms, rtype, params, nearmap, func_base)


def _selftest():
    from codebreaker.invert import compilers as ora
    code = bytes.fromhex("1e589045558bec1e8ed883ec0256ff76069a000000008bf00bf675042bc0eb08569a000000008bc65e4d4d8be51f5d4dca020090")
    src, meta = invert(code, imports={0: "G", 1: "H"})
    if src is None:
        print("DECLINED:", meta); return
    print(src)
    rc = ora.mc6_compile(src, "/AM /G2 /Gsw /Ox")
    print("\n// INTEGRATION driver: masked-equal = %s (%d vs %d B)"
          % (_mask(rc) == _mask(code) and len(rc) == len(code), len(rc), len(code)))
    # far-pointer battery: branch, deref load/pass/store (pointee width from usage)
    far = [
        ("fptr-branch", "1e589045558bec1e8ed883ec028b4e068b46080bc175052bc0eb0b908b460850519a000000004d4d8be51f5d4dca0400", "int", [("name", "far")]),
        ("deref-return", "1e589045558bec1e8ed883ec02c45e06268b074d4d8be51f5d4dca040090", "int", [("p", "far")]),
        ("deref-pass", "1e589045558bec1e8ed883ec02c45e0626ff379a000000004d4d8be51f5d4dca0400", "int", [("p", "far")]),
        ("deref-store", "1e589045558bec1e8ed883ec028b4606c45e082689074d4d8be51f5d4dca0600", "void", [("p", "far"), ("v", "int")]),
        ("dword-shl4", "1e589045558bec1e8ed883ec028b46068b5608d1e0d1d2d1e0d1d2d1e0d1d2d1e0d1d24d4d8be51f5d4dca040090", "long", [("a", "long")]),
        ("dword-add", "1e589045558bec1e8ed883ec028b46068b560803460a13560c4d4d8be51f5d4dca080090", "long", [("a", "long"), ("b", "long")]),
        ("dword-frword", "1e589045558bec1e8ed883ec028b56062bc04d4d8be51f5d4dca0200", "long", [("a", "int")]),
        # short-circuit OR-of-ANDs converging on a JOIN: (f==1&&g==0)||(f==2&&g==0)||(f==3&&g==0)
        ("shortcircuit-or-and", "1e589045558bec1e8ed883ec028b4e088b560683f90175040bd2741283f90275040bd2740983f903750c0bd27508519a00000000eb03b80d004d4d8be51f5d4dca040090", "int", [("f", "int"), ("g", "int")]),
        # materialized boolean: MSC builds (a||b) into a 0/1 register then tests it (olecli sub_28aa)
        ("bool-materialize", "1e589045558bec1e8ed883ec02c45e0626837f7f00750b26837f7d0075042bd2eb03ba01000bd274052bc0eb0490b805004d4d8be51f5d4dca040090", "int", [("p", "far")]),
    ]
    for label, hx, rt, pms in far:
        c = bytes.fromhex(hx)
        s2, _ = invert(c, imports={0: "G"}, rtype=rt, params=pms)
        if not s2:
            print("// %s: DECLINED" % label); continue
        r2 = ora.mc6_compile(s2, "/AM /G2 /Gsw /Ox")
        print("// %-12s masked-equal = %s (%d vs %d B)"
              % (label, _mask(r2) == _mask(c) and len(r2) == len(c), len(r2), len(c)))
    # real retail LZEXPAND functions (cdecl + param-inference + cmp-consumes-call);
    # padding-tolerant compare since a standalone recompile appends an alignment nop.
    real = [
        ("retail-cdecl(seg11)", "1e589045558bec1e8ed883ec02568b760cff7606ff760aff7608569a210500003bc67506b8ffffeb03902bc05e4d4d8be51f5d4dcb", {0: "G"}, None),
        ("retail-wstore(seg10)", "1e589045558bec1e8ed883ec02b8ffffa382004d4d8be51f5d4dcb", {}, None),
        ("retail-free(seg11)", "1e589045558bec1e8ed883ec02833e7e00007420ff367e009affff0000ff367e009affff0000c7067e0000002bc0a3ee00a3ec00c606800000" "2bc0a3ee00a3ec00a3f200a3f000a3f600a3f400a3fa00a3f800a3fe00a3fc00a30201a300014d4d8be51f5d4dcb", {0: "G", 1: "H"}, {0xec: ("gL_ec", "long")}),
        ("retail-cdecl2(seg12)", "1e589045558bec1e8ed883ec029affff0000833e8c01007412ff368c019a9b09000083c402c7068c010000833e8a01007412ff368a019affff000083c402c7068a010000833e8201007420ff3682019af4060000ff3682019afd060000c706820100002bc0a39801a396012bc0a38401a386014d4d8be51f5d4dcb", {}, {0x196: ("gL_196", "long"), 0x186: ("gL_186", "long")}),
        ("retail-addrglob(seg10)", "1e589045558bec1e8ed883ec02688203688003ff76069affff000083c4060bc07406b8ffffeb2090ff368203ff368003ff76089affff000083c4060bc07405b8feffeb03b801004d4d8be51f5d4dcb", {0: "G0", 1: "G1"}, None),
    ]
    for label, hx, imp, sy in real:
        c = bytes.fromhex(hx)
        s3, _ = invert(c, imports=imp, syms=sy)
        if not s3:
            print("// %s: DECLINED" % label); continue
        r3 = ora.mc6_compile(s3, "/AM /G2 /Gsw /Ox")
        eq = _mask(r3).rstrip(b"\x90\x00") == _mask(c).rstrip(b"\x90\x00")
        print("// %-20s masked-equal = %s" % (label, eq))


if __name__ == "__main__":
    _selftest()
