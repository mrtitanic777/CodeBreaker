"""methods.py -- the types of Python's builtins, the methods of its builtin types and the standard
library calls the front end maps. A call not typed here is refused with its name (stdlib:<name>), so the
coverage report lists exactly what is missing. How each one is written in C++ is a row of
codebreaker/java/api_maps/python.json (the API map), keyed by the same name.
"""
import ast

from codebreaker.translate.frontends.python import pytypes as P
from codebreaker.translate.frontends.python import structfmt

# -- simple signatures: name -> (argument kinds, result). An argument kind is a type kind, '*' any of
# the scalar kinds, 'int?' an optional int; the result a T or a function of (eval, recv, args).
S, B, I, F = P.STR, P.BOOL, P.SIZE, P.FLOAT
_SIMPLE = {
    'str.upper': ((), S), 'str.lower': ((), S), 'str.title': ((), S), 'str.capitalize': ((), S),
    'str.strip': (('str?',), S), 'str.lstrip': (('str?',), S), 'str.rstrip': (('str?',), S),
    'str.startswith': (('str|tuple',), B), 'str.endswith': (('str|tuple',), B),
    'str.find': (('str', 'int?', 'int?'), P.FIND), 'str.rfind': (('str', 'int?', 'int?'), P.FIND),
    'str.index': (('str',), P.SIZE), 'str.rindex': (('str',), P.SIZE), 'str.count': (('str',), P.SIZE),
    'str.replace': (('str', 'str'), S), 'str.isdigit': ((), B), 'str.isalpha': ((), B),
    'str.isalnum': ((), B), 'str.isspace': ((), B), 'str.isupper': ((), B), 'str.islower': ((), B),
    'str.isidentifier': ((), B), 'str.ljust': (('int', 'str?'), S), 'str.rjust': (('int', 'str?'), S),
    'str.center': (('int', 'str?'), S), 'str.zfill': (('int',), S),
    'str.removeprefix': (('str',), S), 'str.removesuffix': (('str',), S),
    'str.encode': (('str?', 'str?'), P.BYTES), 'str.isprintable': ((), B), 'str.isascii': ((), B),
    'bytes.upper': ((), P.BYTES), 'bytes.lower': ((), P.BYTES),
    'bytes.strip': (('bytes?',), P.BYTES), 'bytes.lstrip': (('bytes?',), P.BYTES),
    'bytes.rstrip': (('bytes?',), P.BYTES),
    'bytes.startswith': (('bytes|tuple',), B), 'bytes.endswith': (('bytes|tuple',), B),
    'bytes.find': (('bytes|int', 'int?', 'int?'), P.FIND), 'bytes.rfind': (('bytes|int', 'int?', 'int?'), P.FIND),
    'bytes.index': (('bytes|int', 'int?', 'int?'), P.SIZE), 'bytes.count': (('bytes|int',), P.SIZE),
    'bytes.replace': (('bytes', 'bytes'), P.BYTES), 'bytes.isalnum': ((), B), 'bytes.isalpha': ((), B),
    'bytes.isdigit': ((), B), 'bytes.isspace': ((), B), 'bytes.isupper': ((), B), 'bytes.islower': ((), B),
    'bytes.hex': ((), S), 'bytes.ljust': (('int', 'bytes?'), P.BYTES), 'bytes.rjust': (('int', 'bytes?'), P.BYTES),
    'bytes.decode': (('str?', 'str?'), S),
}
_ENCODINGS = ('latin1', 'latin-1', 'iso-8859-1', 'ascii', 'utf-8', 'utf8', 'cp437', 'cp1252')
_ERRORS = ('strict', 'replace', 'ignore')


def _kind_ok(t, want):
    if t is None:
        return False
    if t.kind in ('bottom', 'top'):
        return True
    k = t.kind
    if k == 'bool':
        k = 'int'
    for w in want.rstrip('?').split('|'):
        if w == k or (w == 'bytes' and k == 'bytearray'):
            return True
    return False


def _simple(ev, n, name, args, kw):
    sig, res = _SIMPLE[name]
    if kw:
        if name in ('bytes.decode', 'str.encode') and set(kw) <= {'errors', 'encoding'}:
            pass
        else:
            return ev.issue(n, 'syntax', 'keyword argument to %s' % name)
    if len(args) > len(sig):
        return ev.issue(n, 'conflict', 'too many arguments to %s' % name)
    for i, w in enumerate(sig):
        if i >= len(args):
            if not w.endswith('?'):
                return ev.issue(n, 'conflict', 'missing argument to %s' % name)
            continue
        if not _kind_ok(args[i], w):
            return ev.issue(n, 'conflict', '%s of a %s' % (name, args[i].kind if args[i] else 'lambda'))
    if name in ('bytes.decode', 'str.encode'):
        consts = [_str(a) for a in n.args] + [_str(k.value) for k in n.keywords]
        enc = consts[0] if n.args or any(k.arg == 'encoding' for k in n.keywords) else 'utf-8'
        if enc is None or enc.lower() not in _ENCODINGS:
            return ev.issue(n, 'stdlib', '%s(%r)' % (name, enc))
        errs = [c for c in consts[1:]]
        if any(e is None or e not in _ERRORS for e in errs):
            return ev.issue(n, 'stdlib', '%s errors=%r' % (name, errs))
    if name.endswith(('.startswith', '.endswith')) and args and args[0].kind == 'tuple':
        want = name.split('.')[0]
        for a in args[0].args:
            if not _kind_ok(a, want):
                return ev.issue(n, 'conflict', '%s of a tuple holding %s' % (name, a.kind))
    if name.split('.')[0] in ('bytes',) and args and args[0] is not None and args[0].kind == 'bytearray':
        pass
    return res


def _str(n):
    return n.value if isinstance(n, ast.Constant) and isinstance(n.value, str) else None


def _elem(ev, t):
    return ev.p.elem[ev.p.find(t.site)]


_LAZY_OK = frozenset('join extend update sort'.split())
_LAZY_BUILTINS = frozenset('sorted min max sum any all list set enumerate'.split())


def _lazy(ev, n, args, kw, ok):
    """a lambda or generator argument where this call does not take one"""
    if not ok and (any(a is None for a in args) or any(v is None for v in kw.values())):
        return ev.issue(n, 'dynamic', 'a lambda or generator as an argument')
    return None


def call(ev, n, recv, attr, args, kw, state):
    """recv.attr(args) for a builtin type's value"""
    bad = _lazy(ev, n, args, kw, attr in _LAZY_OK)
    if bad is not None:
        return bad
    k = recv.kind
    if k == 'bool':
        k = 'int'
    name = '%s.%s' % ('bytes' if k == 'bytearray' and ('bytes.' + attr) in _SIMPLE else k, attr)
    if name in _SIMPLE:
        t = _simple(ev, n, name, args, kw)
        if k == 'bytearray' and t.kind == 'bytes' and attr not in ('decode', 'hex'):
            t = P.T('bytearray')
        return t, ('bmethod', name)
    if k == 'str':
        if attr == 'split':
            if len(args) > 2 or (args and not _kind_ok(args[0], 'str|none')) or kw:
                return ev.issue(n, 'syntax', 'str.%s arguments' % attr)
            s = ev.p.new_site(n, 'list', ev.where(n))
            ev.p.grow(ev.p.elem, s, P.STR)
            return P.T('list', site=s), ('bmethod', 'str.' + attr)
        if attr == 'splitlines':
            s = ev.p.new_site(n, 'list', ev.where(n))
            ev.p.grow(ev.p.elem, s, P.STR)
            return P.T('list', site=s), ('bmethod', 'str.splitlines')
        if attr == 'join':
            a = args[0] if args else None
            if a is None and n.args and isinstance(n.args[0], ast.GeneratorExp):
                e, _st = ev.gen_elem(n.args[0], state)
            elif a is not None:
                e, _n = ev.iteration(n.args[0], a, state)
            else:
                return ev.issue(n, 'conflict', 'str.join of nothing')
            if e.kind not in ('str', 'bottom', 'top'):
                return ev.issue(n, 'conflict', 'str.join of %s items' % e.kind)
            return P.STR, ('bmethod', 'str.join')
        if attr in ('partition', 'rpartition'):
            return P.tuple_of([P.STR, P.STR, P.STR]), ('bmethod', 'str.' + attr)
        if attr == 'format':
            return ev.issue(n, 'syntax', 'str.format')
    if k in ('bytes', 'bytearray'):
        if attr == 'join':
            a = args[0] if args else None
            if a is None and n.args and isinstance(n.args[0], ast.GeneratorExp):
                e, _st = ev.gen_elem(n.args[0], state)
            elif a is not None:
                e, _n = ev.iteration(n.args[0], a, state)
            else:
                return ev.issue(n, 'conflict', 'bytes.join of nothing')
            if e.kind not in ('bytes', 'bytearray', 'bottom', 'top'):
                return ev.issue(n, 'conflict', 'bytes.join of %s items' % e.kind)
            return P.T(k), ('bmethod', 'bytes.join')
        if attr == 'split':
            s = ev.p.new_site(n, 'list', ev.where(n))
            ev.p.grow(ev.p.elem, s, P.T(k))
            return P.T('list', site=s), ('bmethod', 'bytes.split')
        if k == 'bytearray' and attr in ('append',):
            if not args or not _kind_ok(args[0], 'int'):
                return ev.issue(n, 'conflict', 'bytearray.append')
            return P.NONE, ('bmethod', 'bytearray.append')
        if k == 'bytearray' and attr == 'extend':
            if not args or args[0] is None or args[0].kind not in ('bytes', 'bytearray', 'bottom', 'top'):
                return ev.issue(n, 'syntax', 'bytearray.extend of a %s' % (args[0].kind if args and args[0] else '?'))
            return P.NONE, ('bmethod', 'bytearray.extend')
    if k == 'list':
        s = recv.site
        e = _elem(ev, recv)
        if attr == 'append':
            if len(args) != 1:
                return ev.issue(n, 'conflict', 'list.append arity')
            ev.p.grow(ev.p.elem, s, ev.fit(n, args[0], 'list item'))
            return P.NONE, ('bmethod', 'list.append')
        if attr == 'extend':
            a = args[0] if args else None
            if a is None and n.args and isinstance(n.args[0], ast.GeneratorExp):
                et, _st = ev.gen_elem(n.args[0], state)
            else:
                et, _n = ev.iteration(n.args[0], a, state) if a is not None else (P.BOTTOM, None)
            ev.p.grow(ev.p.elem, s, et)
            return P.NONE, ('bmethod', 'list.extend')
        if attr == 'insert':
            ev.p.grow(ev.p.elem, s, args[1])
            return P.NONE, ('bmethod', 'list.insert')
        if attr == 'pop':
            return e, ('bmethod', 'list.pop')
        if attr in ('index', 'count'):
            return P.SIZE, ('bmethod', 'list.' + attr)
        if attr in ('remove',):
            return P.NONE, ('bmethod', 'list.remove')
        if attr in ('reverse', 'clear'):
            return P.NONE, ('bmethod', 'list.' + attr)
        if attr == 'copy':
            s2 = ev.p.new_site(n, 'list', ev.where(n))
            ev.p.grow(ev.p.elem, s2, e)
            return P.T('list', site=s2), ('bmethod', 'list.copy')
        if attr == 'sort':
            return _sort_key(ev, n, e, kw, state, 'list.sort', P.NONE)
    if k == 'dict':
        s = ev.p.find(recv.site)
        key = _str(n.args[0]) if n.args else None
        if attr == 'get':
            dflt = args[1] if len(args) > 1 else P.NONE
            if key is not None and s not in ev.p.dynamic:
                ft = ev.p.fields[s].get(key, P.BOTTOM)
                r = ev.p.join(ft, dflt)
                if r.kind == 'top':
                    return ev.issue(n, 'conflict', 'dict.get default: ' + r.why)
                return r, ('bmethod', 'record.get')
            if key is None:
                ev.p.make_dynamic(s, 'get of a computed key')
            if args:
                ev.p.grow(ev.p.keys, s, args[0])
            r = ev.p.join(ev.p.elem[s], dflt)
            if r.kind == 'top':
                return ev.issue(n, 'conflict', 'dict.get default: ' + r.why)
            return r, ('bmethod', 'dict.get')
        ev.p.make_dynamic(s, 'dict.' + attr)
        if attr == 'setdefault':
            if len(args) != 2:
                return ev.issue(n, 'conflict', 'dict.setdefault arity')
            ev.p.grow(ev.p.keys, s, args[0])
            ev.p.grow(ev.p.elem, s, args[1])
            return ev.p.elem[s], ('bmethod', 'dict.setdefault')
        if attr == 'items':
            return P.T('iter', (P.tuple_of([ev.p.keys[s], ev.p.elem[s]]),), lo=P.MEM), ('bmethod', 'dict.items')
        if attr == 'keys':
            return P.T('iter', (ev.p.keys[s],), lo=P.MEM), ('bmethod', 'dict.keys')
        if attr == 'values':
            return P.T('iter', (ev.p.elem[s],), lo=P.MEM), ('bmethod', 'dict.values')
        if attr == 'pop':
            if args:
                ev.p.grow(ev.p.keys, s, args[0])
            r = ev.p.elem[s] if len(args) < 2 else ev.p.join(ev.p.elem[s], args[1])
            return r, ('bmethod', 'dict.pop')
        if attr == 'update':
            if len(args) == 1 and args[0] is not None and args[0].kind == 'dict':
                o = ev.p.find(args[0].site)
                ev.p.make_dynamic(o, 'dict.update')
                ev.p.grow(ev.p.keys, s, ev.p.keys[o])
                ev.p.grow(ev.p.elem, s, ev.p.elem[o])
                return P.NONE, ('bmethod', 'dict.update')
            return ev.issue(n, 'syntax', 'dict.update arguments')
        if attr == 'copy':
            s2 = ev.p.new_site(n, 'dict', ev.where(n))
            ev.p.make_dynamic(s2, 'dict.copy')
            ev.p.grow(ev.p.keys, s2, ev.p.keys[s])
            ev.p.grow(ev.p.elem, s2, ev.p.elem[s])
            return P.T('dict', site=s2), ('bmethod', 'dict.copy')
        if attr == 'clear':
            return P.NONE, ('bmethod', 'dict.clear')
    if k == 'set':
        s = recv.site
        if attr == 'add':
            ev.p.grow(ev.p.elem, s, args[0])
            return P.NONE, ('bmethod', 'set.add')
        if attr in ('discard', 'remove'):
            return P.NONE, ('bmethod', 'set.' + attr)
        if attr == 'update':
            et, _n = ev.iteration(n.args[0], args[0], state) if args and args[0] is not None else (P.BOTTOM, None)
            ev.p.grow(ev.p.elem, s, et)
            return P.NONE, ('bmethod', 'set.update')
        if attr in ('issubset', 'issuperset', 'isdisjoint'):
            return P.BOOL, ('bmethod', 'set.' + attr)
    if k == 'int':
        if attr == 'bit_length':
            return P.INT(0, 64 if recv.fits_i64() else None), ('bmethod', 'int.bit_length')
        if attr == 'to_bytes':
            return P.BYTES, ('bmethod', 'int.to_bytes')
    if k == 'fn' and recv.name.startswith('builtin:'):
        full = recv.name[8:] + '.' + attr
        if full == 'int.from_bytes':
            return from_bytes(ev, n, args, kw)
        if full == 'bytes.fromhex':
            return P.BYTES, ('std', 'bytes.fromhex')
        return ev.issue(n, 'stdlib', full)
    return ev.issue(n, 'stdlib', '%s.%s' % (k, attr))


def _sort_key(ev, n, e, kw, state, name, result):
    for k in kw:
        if k not in ('key', 'reverse'):
            return ev.issue(n, 'syntax', '%s(%s=)' % (name, k))
    key = next((x.value for x in n.keywords if x.arg == 'key'), None)
    kt = e
    if key is not None:
        kt = ev.lambda_type(key, [e], state)
    if kt.kind not in ('int', 'str', 'float', 'bytes', 'tuple', 'bottom', 'top', 'bool'):
        return ev.issue(n, 'conflict', 'sorting by a %s' % kt.kind)
    return result, ('builtin', 'sorted') if name == 'sorted' else ('bmethod', name)


def from_bytes(ev, n, args, kw):
    if len(args) not in (1, 2):
        return ev.issue(n, 'conflict', 'int.from_bytes arity')
    b = args[0]
    order = _str(n.args[1]) if len(n.args) > 1 else _str(next((k.value for k in n.keywords if k.arg == 'byteorder'), None))
    if len(n.args) < 2 and 'byteorder' not in kw:
        order = 'big'
    if order not in ('little', 'big'):
        return ev.issue(n, 'dynamic', 'int.from_bytes byte order')
    signed = next((k.value for k in n.keywords if k.arg == 'signed'), None)
    if signed is not None and not (isinstance(signed, ast.Constant) and isinstance(signed.value, bool)):
        return ev.issue(n, 'dynamic', 'int.from_bytes signed=<computed>')
    sg = bool(signed.value) if signed is not None else False
    if b is None or b.kind not in ('bytes', 'bytearray', 'bottom', 'top'):
        return ev.issue(n, 'conflict', 'int.from_bytes of a %s' % (b.kind if b else '?'))
    if b.kind in ('bottom', 'top'):
        return b
    if b.hi is None:
        return P.INT(None if sg else 0, None), ('std', 'int.from_bytes')
    bits = 8 * b.hi
    return (P.s(bits) if sg and bits else P.u(bits)), ('std', 'int.from_bytes')


def builtin(ev, n, name, args, kw, state):
    from codebreaker.translate.frontends.python.infer import DYNAMIC_BUILTINS
    if name in DYNAMIC_BUILTINS:
        return ev.issue(n, 'dynamic', name)
    bad = _lazy(ev, n, args, kw if name not in ('sorted', 'min', 'max') else {}, name in _LAZY_BUILTINS)
    if bad is not None:
        return bad
    a0 = args[0] if args else None
    if name == 'len':
        if len(args) != 1 or a0 is None:
            return ev.issue(n, 'conflict', 'len arity')
        k = a0.kind
        if k == 'dict':
            ev.p.make_dynamic(a0.site, 'len')
        if k == 'tuple':
            return P.const_int(len(a0.args)), ('builtin', 'len')
        if k in ('bytes', 'bytearray'):
            return P.INT(0, P.MEM if a0.hi is None else a0.hi), ('builtin', 'len')
        if k in ('str', 'list', 'dict', 'set', 'bottom', 'top'):
            return P.SIZE, ('builtin', 'len')
        return ev.issue(n, 'conflict', 'len of a %s' % k)
    if name == 'range':
        if kw or not 1 <= len(args) <= 3 or any(a is None or a.kind not in ('int', 'bottom', 'top', 'bool') for a in args):
            return ev.issue(n, 'conflict', 'range arguments')
        if any(a.kind == 'bottom' for a in args):
            return P.BOTTOM
        if any(a.kind == 'top' for a in args):
            return next(a for a in args if a.kind == 'top')
        args = [P.INT(0, 1) if a.kind == 'bool' else a for a in args]
        if len(args) == 1:
            lo, hi = P.const_int(0), args[0]
        else:
            lo, hi = args[0], args[1]
        step = _intc(n.args[2]) if len(args) == 3 else 1
        if step is None or step == 0:
            return ev.issue(n, 'syntax', 'range with a computed step')
        if step > 0:
            e = P.INT(lo.lo, None if hi.hi is None else hi.hi - 1)
        else:
            e = P.INT(None if hi.lo is None else hi.lo + 1, lo.hi)
        return P.T('iter', (e,), lo=ev.range_count(n)), ('builtin', 'range')
    if name == 'enumerate':
        if a0 is None:
            return ev.issue(n, 'conflict', 'enumerate arguments')
        e, cnt = ev.iteration(n.args[0], a0, state)
        start = args[1] if len(args) > 1 else kw.get('start', P.const_int(0))
        if start.kind != 'int':
            return ev.issue(n, 'conflict', 'enumerate start')
        idx = P.iadd(start, P.INT(0, (cnt if cnt is not None else P.MEM) - 1))
        return P.T('iter', (P.tuple_of([idx, e]),), lo=cnt), ('builtin', 'enumerate')
    if name == 'zip':
        if kw or any(a is None for a in args):
            return ev.issue(n, 'syntax', 'zip arguments')
        es = [ev.iteration(x, a, state)[0] for x, a in zip(n.args, args)]
        return P.T('iter', (P.tuple_of(es),), lo=P.MEM), ('builtin', 'zip')
    if name == 'reversed':
        if a0 is None:
            return ev.issue(n, 'conflict', 'reversed arguments')
        if a0.kind == 'dict':
            return ev.issue(n, 'syntax', 'reversed of a dict')
        e, cnt = ev.iteration(n.args[0], a0, state)
        return P.T('iter', (e,), lo=cnt), ('builtin', 'reversed')
    if name == 'sorted':
        if a0 is None and n.args and isinstance(n.args[0], ast.GeneratorExp):
            e, _st = ev.gen_elem(n.args[0], state)
        elif a0 is None:
            return ev.issue(n, 'conflict', 'sorted arguments')
        else:
            e, _cnt = ev.iteration(n.args[0], a0, state)
            if a0.kind == 'set':
                ev.issues = [i for i in ev.issues if not (i.detail == 'set iteration order' and i.line == n.lineno)]
        s = ev.p.new_site(n, 'list', ev.where(n))
        ev.p.grow(ev.p.elem, s, e)
        return _sort_key(ev, n, e, kw, state, 'sorted', P.T('list', site=s))
    if name in ('min', 'max'):
        for k in kw:
            if k not in ('key', 'default'):
                return ev.issue(n, 'syntax', '%s(%s=)' % (name, k))
        if 'key' in kw:
            return ev.issue(n, 'syntax', '%s with a key' % name)
        if len(args) == 1:
            if a0 is None and isinstance(n.args[0], ast.GeneratorExp):
                e, _st = ev.gen_elem(n.args[0], state)
            else:
                e, _c = ev.iteration(n.args[0], a0, state)
            if 'default' in kw:
                e = ev.p.join(e, kw['default'])
            ts = [e]
        else:
            ts = args
        r = P.BOTTOM
        for t in ts:
            if t is None:
                return ev.issue(n, 'conflict', '%s arguments' % name)
            r = ev.p.join(r, P.INT(0, 1) if t.kind == 'bool' else t)
        if r.kind == 'top':
            return ev.issue(n, 'conflict', '%s of mixed types: %s' % (name, r.why))
        if r.kind not in ('int', 'str', 'float', 'bytes', 'tuple', 'bottom'):
            return ev.issue(n, 'conflict', '%s of %s' % (name, r.kind))
        if r.kind == 'int' and len(ts) > 1:
            los = [t.lo for t in ts]
            his = [t.hi for t in ts]
            if name == 'min':
                r = P.INT(None if None in los else min(los), None if all(h is None for h in his) else min(h for h in his if h is not None))
            else:
                r = P.INT(None if all(l_ is None for l_ in los) else max(l_ for l_ in los if l_ is not None), None if None in his else max(his))
        return r, ('builtin', name + ('_iter' if len(args) == 1 else ''))
    if name == 'abs':
        if a0 is None:
            return ev.issue(n, 'conflict', 'abs')
        if a0.kind == 'int':
            if a0.lo is not None and a0.lo >= 0:
                return a0, ('builtin', 'abs')
            m = None if a0.lo is None or a0.hi is None else max(abs(a0.lo), abs(a0.hi))
            return P.INT(0, m), ('builtin', 'abs')
        if a0.kind == 'float':
            return P.FLOAT, ('builtin', 'abs')
        return a0 if a0.kind in ('bottom', 'top') else ev.issue(n, 'conflict', 'abs of %s' % a0.kind)
    if name == 'sum':
        if a0 is None and n.args and isinstance(n.args[0], ast.GeneratorExp):
            e, _st = ev.gen_elem(n.args[0], state)
            cnt = P.MEM
        elif a0 is not None:
            e, cnt = ev.iteration(n.args[0], a0, state)
        else:
            return ev.issue(n, 'conflict', 'sum')
        if e.kind == 'bool':
            e = P.INT(0, 1)
        if e.kind == 'int':
            cnt = P.MEM if cnt is None else cnt
            r = P.imul(e, P.INT(0, cnt))
            r = P.INT(min(r.lo, 0) if r.lo is not None else None, max(r.hi, 0) if r.hi is not None else None)
            if len(args) > 1:
                r = P.iadd(r, args[1])
            return r, ('builtin', 'sum')
        if e.kind == 'float':
            return P.FLOAT, ('builtin', 'sum')
        return e if e.kind in ('bottom', 'top') else ev.issue(n, 'conflict', 'sum of %s' % e.kind)
    if name in ('any', 'all'):
        if a0 is None and n.args and isinstance(n.args[0], ast.GeneratorExp):
            ev.gen_elem(n.args[0], state)
        elif a0 is not None:
            ev.iteration(n.args[0], a0, state)
        return P.BOOL, ('builtin', name)
    if name == 'int':
        if kw:
            return ev.issue(n, 'syntax', 'int keywords')
        if not args:
            return P.const_int(0), ('builtin', 'int')
        if a0.kind in ('int', 'bool'):
            return (a0 if a0.kind == 'int' else P.INT(0, 1)), ('builtin', 'int')
        if a0.kind == 'str':
            if len(args) == 2 and _intc(n.args[1]) is None:
                return ev.issue(n, 'dynamic', 'int() with a computed base')
            return P.INT(), ('builtin', 'int_str')
        if a0.kind == 'float':
            return P.INT(), ('builtin', 'int_float')
        return a0 if a0.kind in ('bottom', 'top') else ev.issue(n, 'conflict', 'int() of %s' % a0.kind)
    if name == 'float':
        if a0 is not None and a0.kind == 'int':
            ev.exact_float(n, a0)
        return P.FLOAT, ('builtin', 'float')
    if name == 'str':
        if not args:
            return P.STR, ('builtin', 'str')
        ev.check_str(n, a0, False)
        return P.STR, ('builtin', 'str')
    if name == 'repr':
        ev.check_str(n, a0, True)
        return P.STR, ('builtin', 'repr')
    if name == 'bool':
        return P.BOOL, ('builtin', 'bool')
    if name in ('bytes', 'bytearray'):
        if not args:
            return P.T(name, hi=0), ('builtin', name)
        if a0.kind == 'int':
            return P.T(name, hi=a0.hi), ('builtin', name + '_zeros')
        if a0.kind in ('bytes', 'bytearray'):
            return P.T(name, hi=a0.hi), ('builtin', name + '_copy')
        if a0.kind == 'list':
            e = _elem(ev, a0)
            if e.kind not in ('int', 'bottom', 'top'):
                return ev.issue(n, 'conflict', '%s of a list of %s' % (name, e.kind))
            return P.T(name), ('builtin', name + '_list')
        if a0.kind == 'str':
            return ev.issue(n, 'syntax', '%s(str, encoding)' % name)
        if a0.kind == 'iter':
            return P.T(name), ('builtin', name + '_iter')
        return a0 if a0.kind in ('bottom', 'top') else ev.issue(n, 'conflict', '%s of %s' % (name, a0.kind))
    if name == 'list':
        s = ev.p.new_site(n, 'list', ev.where(n))
        if args:
            if a0 is None and isinstance(n.args[0], ast.GeneratorExp):
                e, _st = ev.gen_elem(n.args[0], state)
            else:
                e, _c = ev.iteration(n.args[0], a0, state)
            ev.p.grow(ev.p.elem, s, e)
        return P.T('list', site=s), ('builtin', 'list')
    if name == 'set':
        s = ev.p.new_site(n, 'set', ev.where(n))
        if args:
            if a0 is None and isinstance(n.args[0], ast.GeneratorExp):
                e, _st = ev.gen_elem(n.args[0], state)
            else:
                e, _c = ev.iteration(n.args[0], a0, state)
                if a0 is not None and a0.kind == 'set':
                    ev.issues = [i for i in ev.issues if not (i.detail == 'set iteration order' and i.line == n.lineno)]
            ev.p.grow(ev.p.elem, s, e)
        return P.T('set', site=s), ('builtin', 'set')
    if name == 'frozenset':
        return ev.issue(n, 'syntax', 'frozenset')
    if name == 'tuple':
        if a0 is not None and a0.kind == 'tuple':
            return a0, ('builtin', 'tuple')
        return ev.issue(n, 'conflict', 'tuple() of a sequence (its length is not in the type)')
    if name == 'dict':
        s = ev.p.new_site(n, 'dict', ev.where(n))
        keys = []
        for k in n.keywords:
            if k.arg is None:
                ev.spread(s, ev.ev(k.value, state), n)
            else:
                ev.p.grow_field(s, k.arg, ev.fit(k.value, kw[k.arg], 'field ' + k.arg))
                keys.append(k.arg)
        if args:
            if a0.kind == 'dict':
                ev.spread(s, a0, n)
            else:
                return ev.issue(n, 'syntax', 'dict() of a %s' % a0.kind)
        if ev.live:
            ev.p.literal_keys[id(n)] = keys + list(ev.p.spread_keys.get(s, ()))
            if ev.builder != id(n):
                ev.p.created(s, ev.p.literal_keys[id(n)])
        return P.T('dict', site=s), ('builtin', 'dict')
    if name == 'print':
        if set(kw) - {'end', 'sep', 'file', 'flush'}:
            return ev.issue(n, 'syntax', 'print keywords')
        if 'file' in kw:
            return ev.issue(n, 'stdlib', 'print(file=)')
        for x, t in zip(n.args, args):
            if t is not None:
                ev.check_str(x, t, False)
        return P.NONE, ('builtin', 'print')
    if name == 'open':
        mode = _str(n.args[1]) if len(n.args) > 1 else _str(next((k.value for k in n.keywords if k.arg == 'mode'), None))
        if mode == 'rb':
            return P.T('extern', name='file'), ('builtin', 'open_rb')
        return ev.issue(n, 'stdlib', 'open(%r)' % mode)
    if name == 'ord':
        return P.INT(0, 0x10FFFF), ('builtin', 'ord')
    if name == 'chr':
        if a0 is not None and a0.kind not in ('int', 'bottom', 'top'):
            return ev.issue(n, 'conflict', 'chr of %s' % a0.kind)
        return P.STR, ('builtin', 'chr')
    if name in ('hex', 'oct', 'bin'):
        return P.STR, ('builtin', name)
    if name == 'divmod':
        if len(args) == 2 and all(a is not None and a.kind == 'int' for a in args):
            return P.tuple_of([P.ifloordiv(args[0], args[1]), P.imod(args[0], args[1])]), ('builtin', 'divmod')
        return ev.issue(n, 'conflict', 'divmod arguments')
    if name == 'pow':
        if len(args) == 3 and all(a is not None and a.kind == 'int' for a in args) and args[2].positive():
            return P.INT(0, None if args[2].hi is None else args[2].hi - 1), ('builtin', 'pow3')
        if len(args) == 2:
            return ev.binop(n, ast.Pow(), args[0], args[1], n.args[0], n.args[1]), ('builtin', 'pow')
        return ev.issue(n, 'conflict', 'pow arguments')
    if name == 'round':
        if len(args) == 1:
            return P.INT(), ('builtin', 'round')
        return P.FLOAT, ('builtin', 'round2')
    if name == 'format':
        spec = _str(n.args[1]) if len(n.args) > 1 else ''
        if spec is None:
            return ev.issue(n, 'dynamic', 'format with a computed spec')
        ev.check_format(n, a0, spec)
        return P.STR, ('builtin', 'format')
    if name == 'isinstance':
        return _isinstance(ev, n, a0)
    if name == 'super':
        if args:
            return ev.issue(n, 'syntax', 'super(arguments)')
        return P.T('extern', name='super'), ('builtin', 'super')
    if name in ('map', 'filter'):
        return ev.issue(n, 'dynamic', name)
    if name in ('object', 'staticmethod', 'classmethod', 'property'):
        return ev.issue(n, 'dynamic', name)
    return ev.issue(n, 'stdlib', 'builtins.' + name)


_ISTYPES = {'int': ('int', 'bool'), 'str': ('str',), 'bytes': ('bytes',), 'bytearray': ('bytearray',),
            'bool': ('bool',), 'float': ('float',), 'list': ('list',), 'dict': ('dict',), 'tuple': ('tuple',),
            'set': ('set',)}


def _isinstance(ev, n, t):
    """isinstance with a statically known answer (the value's type is one kind) is that answer; one
    that depends on the run-time class is refused."""
    if t is None or len(n.args) != 2:
        return ev.issue(n, 'conflict', 'isinstance arguments')
    if t.kind in ('bottom', 'top'):
        return P.BOOL
    spec = n.args[1]
    names = spec.elts if isinstance(spec, ast.Tuple) else [spec]
    answer = False
    for x in names:
        if isinstance(x, ast.Name) and x.id in _ISTYPES:
            if t.kind in _ISTYPES[x.id]:
                answer = True
            elif t.kind == 'opt' and t.args[0].kind in _ISTYPES[x.id]:
                return ev.issue(n, 'dynamic', 'isinstance of an optional value (use `is not None`)')
        elif isinstance(x, ast.Name) and t.kind == 'inst':
            cq = ev.lookup_class(x)
            if cq is None:
                return ev.issue(n, 'dynamic', 'isinstance %s' % ast.unparse(x))
            if cq in ev.p.mro(t.name):
                answer = True
            elif t.name in ev.p.mro(cq):
                return ev.issue(n, 'dynamic', 'isinstance of a subclass (run-time class)')
        elif isinstance(x, ast.Name):
            if ev.lookup_class(x) is None and x.id not in _ISTYPES:
                return ev.issue(n, 'dynamic', 'isinstance %s' % ast.unparse(x))
        else:
            return ev.issue(n, 'dynamic', 'isinstance %s' % ast.unparse(x))
    return P.BOOL, ('isinstance', answer)


def _intc(n):
    from codebreaker.translate.frontends.python.infer import _int_const
    return _int_const(n)


# ------------------------------------------------------------------------------------------ stdlib
_PATH = {'os.path.join': ('str+', P.STR), 'os.path.basename': (('str',), P.STR), 'os.path.dirname': (('str',), P.STR),
         'os.path.splitext': (('str',), P.tuple_of([P.STR, P.STR])), 'os.path.exists': (('str',), P.BOOL),
         'os.path.isfile': (('str',), P.BOOL), 'os.path.isdir': (('str',), P.BOOL),
         'os.path.getsize': (('str',), P.SIZE), 'os.path.split': (('str',), P.tuple_of([P.STR, P.STR])),
         'os.path.normcase': (('str',), P.STR)}


def std_call(ev, n, name, args, kw, state):
    bad = _lazy(ev, n, args, kw, False)
    if bad is not None:
        return bad
    if name.startswith('struct.'):
        return _struct(ev, n, name, args, kw)
    if name in _PATH:
        sig, res = _PATH[name]
        if kw:
            return ev.issue(n, 'syntax', name + ' keywords')
        if sig == 'str+':
            if not args or any(a is None or a.kind not in ('str', 'bottom', 'top') for a in args):
                return ev.issue(n, 'conflict', name + ' arguments')
        elif len(args) != len(sig) or any(a is None or a.kind not in ('str', 'bottom', 'top') for a in args):
            return ev.issue(n, 'conflict', name + ' arguments')
        return res, ('std', name)
    if name == 'os.listdir':
        s = ev.p.new_site(n, 'list', ev.where(n))
        ev.p.grow(ev.p.elem, s, P.STR)
        return P.T('list', site=s), ('std', name)
    if name in ('binascii.crc32', 'zlib.crc32'):
        if not args or args[0] is None or args[0].kind not in ('bytes', 'bytearray', 'bottom', 'top'):
            return ev.issue(n, 'conflict', name + ' arguments')
        if len(args) > 1 and (args[1] is None or not args[1].fits_i64()):
            return ev.issue(n, 'int-range', name + ' start value')
        return P.u(32), ('std', name)
    if name == 'bisect.bisect_left' or name == 'bisect.bisect_right' or name == 'bisect.bisect':
        if len(args) != 2 or kw or args[0] is None or args[0].kind != 'list':
            return ev.issue(n, 'syntax', name + ' arguments')
        return P.SIZE, ('std', name)
    if name == 'math.sqrt':
        return P.FLOAT, ('std', name)
    if name in ('math.floor', 'math.ceil'):
        return P.INT(), ('std', name)
    top = name.split('.')[0]
    return ev.issue(n, 'stdlib', name if top != 're' else 're.' + name.split('.')[-1])


def _struct(ev, n, name, args, kw):
    fn = name.split('.', 1)[1]
    if kw:
        return ev.issue(n, 'syntax', name + ' keywords')
    if fn == 'error':
        return P.T('inst', name='exc:struct.error'), ('exc', 'struct.error')
    fmt = _str(n.args[0]) if n.args else None
    if fmt is None:
        return ev.issue(n, 'dynamic', name + ' with a computed format')
    items = structfmt.parse(fmt)
    if items is None:
        return ev.issue(n, 'stdlib', '%s(%r)' % (name, fmt))
    vals = structfmt.values(items)
    if fn == 'calcsize':
        return P.const_int(structfmt.size(items)), ('std', name)
    if fn in ('unpack', 'unpack_from'):
        want = 2 if fn == 'unpack' else (2, 3)
        if (fn == 'unpack' and len(args) != 2) or (fn == 'unpack_from' and len(args) not in (2, 3)):
            return ev.issue(n, 'conflict', name + ' arity')
        b = args[1]
        if b is None or b.kind not in ('bytes', 'bytearray', 'bottom', 'top'):
            return ev.issue(n, 'conflict', '%s of a %s' % (name, b.kind if b else '?'))
        if len(args) == 3 and args[2] is not None and args[2].kind not in ('int', 'bottom', 'top'):
            return ev.issue(n, 'conflict', name + ' offset')
        ts = []
        for code, size, kind, lo, hi in vals:
            if kind == 'int':
                ts.append(P.INT(lo, hi))
            elif kind == 'bytes':
                ts.append(P.bytes_of(size))
            elif kind == 'bool':
                ts.append(P.BOOL)
            else:
                ts.append(P.FLOAT)
        return P.tuple_of(ts), ('std', name, fmt)
    if fn in ('pack', 'pack_into'):
        k0 = 1 if fn == 'pack' else 3
        given = args[k0:]
        if len(given) != len(vals):
            return ev.issue(n, 'conflict', '%s: %d values for %r' % (name, len(given), fmt))
        if fn == 'pack_into' and (args[1] is None or args[1].kind not in ('bytearray', 'bottom', 'top')):
            return ev.issue(n, 'conflict', 'pack_into a %s' % (args[1].kind if args[1] else '?'))
        for t, (code, size, kind, lo, hi) in zip(given, vals):
            if t is None:
                return ev.issue(n, 'conflict', name + ' value')
            want = {'int': ('int', 'bool'), 'bytes': ('bytes', 'bytearray'), 'bool': ('bool', 'int'),
                    'float': ('float', 'int')}[kind]
            if t.kind not in want + ('bottom', 'top'):
                return ev.issue(n, 'conflict', '%s of a %s for %s' % (name, t.kind, code))
        return (P.bytes_of(structfmt.size(items)) if fn == 'pack' else P.NONE), ('std', name, fmt)
    return ev.issue(n, 'stdlib', name)
