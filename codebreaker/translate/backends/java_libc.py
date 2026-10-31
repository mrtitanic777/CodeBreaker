"""java_libc.py -- a C library call as Java: which method (codebreaker/java/api_maps/libc.json, the API
map's C rows) and how its arguments travel.

crt.LibC's methods take C's parameters by their Java types (a pointer is an int address, size_t a
long), except that a `char *` parameter is an Object: a string literal travels as a Java String, so
`printf("%d\\n", x)` stays `printf("%d\\n", x)`; any other char pointer is its address. Arguments past a
variadic prototype's last parameter go as they are (boxed), and LibC's printf reads them as the format
says, as C's does. A row of kind 'direct' names the JDK's own method (Math.sqrt); 'unsupported' and
missing rows are refused with the reason.
"""
from codebreaker.translate import ir
from codebreaker.translate.backends import java as jb

_REG = []


def registry():
    if not _REG:
        from codebreaker.java import apimap
        _REG.append(apimap.load())
    return _REG[0]


def _string_literal(a):
    a = jb.strip(a)
    if isinstance(a, ir.Cast) and a.x is not None:
        x = jb.strip(a.x)
        if isinstance(x, ir.Lit) and isinstance(x.value, str):
            return x
    return None


def _char_ptr(t):
    return t is not None and t.kind == 'ptr' and t.elem is not None and jb._kind(t.elem) in ('i8', 'u8', 'u16')


def call(w, e, name, args):
    """w: the FnWriter; e: the ir.Call; -> jb.J."""
    if name == '__acrt_iob_func':                          # MSVC's stdin/stdout/stderr
        return jb.J('stream(%s)' % w.value(args[0]).code, jb.P_POST, 'int')
    m = registry().resolve(name, 'c', 'java')
    if m is None:
        raise jb.Refused('library function %s: not in the C runtime for Java (api_maps/libc.json)' % name)
    if m.kind == 'unsupported':
        raise jb.Refused('library function %s: %s' % (name, m.note or 'not carried'))
    target = m.target
    if target.startswith('crt.LibC.'):
        target = target[len('crt.LibC.'):]
    params = list(e.sym.params or [])
    out = []
    for i, a in enumerate(args):
        pt = params[i] if i < len(params) else None
        lit = _string_literal(a)
        if lit is not None and (pt is None or _char_ptr(pt)):
            out.append(jb.string_lit(lit))
            continue
        if pt is not None and pt.kind == 'fnptr':
            out.append(_callback(w, a, pt))
            continue
        if pt is None:                                     # a variadic argument: as C passed it
            v = w.as_value(w.value(a), a.type)
            out.append(v.code)
        else:
            out.append(w.conv(w.value(a), a.type, pt).code)
    jt = w.p.jt(e.type) if e.type is not None else 'void'
    return jb.J('%s(%s)' % (target, ', '.join(out)), jb.P_POST, jt)


def _callback(w, a, pt):
    """A function pointer given to the library (qsort's comparator): a Java functional value."""
    x = jb.strip(a)
    if isinstance(x, ir.Cast):
        x = jb.strip(x.x)
    if isinstance(x, ir.AddressOf) and x.sym.name in w.p.functions:
        return '%s::%s' % (w.p.name, jb.jname(x.sym.name))
    sig = w.p.fn_sig_of_type(pt)
    iface = w.p.iface(sig)
    names = ['a', 'b', 'c', 'd'][:len(sig[0])]
    f = w.value(a).code
    return '(%s) -> ((%s) Fn.at(%s)).call(%s)' % (', '.join(names), iface, f, ', '.join(names))
