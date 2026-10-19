"""l2.py -- the IR as L2 JSON, the document format every producer and consumer shares.

L2 is defined by codebreaker/translate/l2.schema.json (docs/IR_UNIFICATION.md: one IR family, three
levels; L2 is the typed hub tree). ir.py is the Python implementation; the decompiler's PrintIR and the
Rust core produce and read the same JSON. A document is one Node, usually a Module or a Unit:

    {"node": "Binary", "line": 7, "col": 16, "op": ">>>", "type": {"kind": "i32"},
     "left": {...}, "right": {...}}

    to_json(node) -> dict          from_json(dict) -> node          validate(dict) (needs jsonschema)
"""
import json
import math
import os

from codebreaker.translate import ir

SCHEMA = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'l2.schema.json')
_NODES = dict((c.__name__, c) for c in vars(ir).values()
              if isinstance(c, type) and issubclass(c, ir.Node) and c not in (ir.Node, ir.Stmt, ir.Expr))


# ---------------------------------------------------------------------------------------------- types
def type_json(t):
    if t is None:
        return None
    k = t.kind
    if k in ir.PRIMS or k == 'void':
        d = {'kind': k}
        if 'signed' in t.info:
            d['signed'] = t.info['signed']
        return d
    if k in ('null', 'any', 'dynamic'):
        return {'kind': k}
    if k == 'class':
        d = {'kind': 'class', 'name': t.name}
        if t.args:
            d['args'] = [type_json(a) for a in t.args]
        return d
    if k == 'typevar':
        return {'kind': 'typevar', 'name': t.name}
    if k == 'wildcard':
        d = {'kind': 'wildcard'}
        if t.bound:
            d['bound'], d['type'] = t.bound[0], type_json(t.bound[1])
        return d
    if k == 'array':
        d = {'kind': 'array', 'elem': type_json(t.elem)}
        if 'count' in t.info:
            d['count'] = t.info['count']
        return d
    if k == 'ptr':
        d = {'kind': 'ptr', 'space': t.info.get('space', 'flat'), 'to': type_json(t.elem)}
        if 'seg' in t.info:
            d['seg'] = t.info['seg']
        return d
    if k == 'selector':
        return dict({'kind': 'selector'}, **({'of': t.info['of']} if 'of' in t.info else {}))
    if k == 'fn':
        d = {'kind': 'fn', 'conv': t.info.get('conv', 'cdecl'),
             'params': [dict(p, type=type_json(p['type'])) for p in t.info.get('params', [])],
             'result': dict(t.info.get('result', {'at': 'none'}))}
        if 'type' in d['result']:
            d['result']['type'] = type_json(d['result']['type'])
        for key in ('distance', 'pops', 'loadds', 'preserves', 'variadic'):
            if key in t.info:
                d[key] = t.info[key]
        return d
    if k == 'fnptr':
        return {'kind': 'fnptr', 'space': t.info.get('space', 'flat'), 'fn': type_json(t.info['fn'])}
    if k == 'record':
        d = {'kind': 'record', 'size': t.info['size'],
             'fields': [dict(f, type=type_json(f['type'])) for f in t.info.get('fields', [])]}
        if t.name:
            d['name'] = t.name
        for key in ('role', 'packed'):
            if key in t.info:
                d[key] = t.info[key]
        return d
    raise ValueError('no L2 form for type kind %r' % k)


def type_from(d):
    if d is None:
        return None
    k = d['kind']
    if k in ir.PRIMS or k == 'void':
        return ir.Type(k, **({'signed': d['signed']} if 'signed' in d else {}))
    if k in ('null', 'any', 'dynamic'):
        return ir.Type(k)
    if k == 'class':
        return ir.Type('class', d['name'], [type_from(a) for a in d.get('args', [])])
    if k == 'typevar':
        return ir.Type('typevar', d['name'])
    if k == 'wildcard':
        return ir.Type('wildcard', bound=(d['bound'], type_from(d['type'])) if 'bound' in d else None)
    if k == 'array':
        return ir.Type('array', elem=type_from(d['elem']), **({'count': d['count']} if 'count' in d else {}))
    if k == 'ptr':
        return ir.Type('ptr', elem=type_from(d['to']), space=d['space'], **({'seg': d['seg']} if 'seg' in d else {}))
    if k == 'selector':
        return ir.Type('selector', **({'of': d['of']} if 'of' in d else {}))
    if k == 'fn':
        info = dict((key, d[key]) for key in ('conv', 'distance', 'pops', 'loadds', 'preserves', 'variadic') if key in d)
        info['params'] = [dict(p, type=type_from(p['type'])) for p in d['params']]
        res = dict(d['result'])
        if 'type' in res:
            res['type'] = type_from(res['type'])
        info['result'] = res
        return ir.Type('fn', **info)
    if k == 'fnptr':
        return ir.Type('fnptr', space=d['space'], fn=type_from(d['fn']))
    if k == 'record':
        info = dict((key, d[key]) for key in ('size', 'role', 'packed') if key in d)
        info['fields'] = [dict(f, type=type_from(f['type'])) for f in d['fields']]
        return ir.Type('record', d.get('name'), **info)
    raise ValueError('no IR form for L2 type kind %r' % k)


# ---------------------------------------------------------------------------------------------- symbols
def sym_json(s):
    if s is None:
        return None
    d = {'kind': s.kind, 'name': s.name}
    if s.owner is not None:
        d['owner'] = s.owner
    if s.static:
        d['static'] = True
    if s.type is not None:
        d['type'] = type_json(s.type)
    if s.params or s.kind in ('method', 'ctor', 'function'):
        d['params'] = [type_json(p) for p in s.params]
    if s.ret is not None:
        d['ret'] = type_json(s.ret)
    if s.varargs:
        d['varargs'] = True
    return d


def sym_from(d):
    if d is None:
        return None
    return ir.Sym(d['kind'], d['name'], d.get('owner'), d.get('static', False), type_from(d.get('type')),
                  [type_from(p) for p in d.get('params', [])], type_from(d.get('ret')), d.get('varargs', False))


# ---------------------------------------------------------------------------------------------- nodes
def _value(v):
    if isinstance(v, ir.Node):
        return to_json(v)
    if isinstance(v, ir.Type):
        return type_json(v)
    if isinstance(v, ir.Sym):
        return sym_json(v)
    if isinstance(v, (list, tuple)):
        return [_value(x) for x in v]
    if isinstance(v, float) and not math.isfinite(v):
        return repr(v)                              # JSON has no NaN/inf: a Lit keeps them as text
    return v


def to_json(node):
    d = {'node': type(node).__name__}
    if node.line:
        d['line'] = node.line
    if node.col:
        d['col'] = node.col
    if node.origin is not None:
        d['origin'] = node.origin
    for slot in node.slots():
        v = getattr(node, slot)
        if v is None or (slot not in node.fields and v == [] and slot not in ('params', 'args')):
            continue
        d[slot] = _value(v)
    return d


_TYPED = frozenset(('type', 'extends', 'ret', 'elem', 'test', 'of', 'qualifier'))
_TYPE_LISTS = frozenset(('implements', 'throws', 'types', 'typeargs'))


def from_json(d):
    cls = _NODES[d['node']]
    kw = {'line': d.get('line', 0), 'col': d.get('col', 0), 'origin': d.get('origin')}
    for slot in cls.slots():
        if slot not in d:
            continue
        v = d[slot]
        if slot == 'sym' or slot == 'ctor':
            kw[slot] = sym_from(v)
        elif isinstance(v, dict) and 'node' in v:             # a node first: a ClassDecl has a 'kind' too
            kw[slot] = from_json(v)
        elif isinstance(v, list) and v and all(isinstance(x, dict) and 'node' in x for x in v):
            kw[slot] = [from_json(x) for x in v]
        elif slot in _TYPED and isinstance(v, dict) and 'kind' in v:
            kw[slot] = type_from(v)
        elif slot in _TYPE_LISTS and isinstance(v, list) and all(isinstance(x, dict) and 'kind' in x for x in v):
            kw[slot] = [type_from(x) for x in v]
        else:
            kw[slot] = v
    return cls(**kw)


def schema():
    with open(SCHEMA, encoding='utf-8') as f:
        return json.load(f)


def validate(doc):
    """Raise jsonschema.ValidationError if `doc` is not L2."""
    import jsonschema
    jsonschema.Draft202012Validator(schema()).validate(doc)
