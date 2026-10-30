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


# ------------------------------------------------------------------------------------ the module model
# A module, of any lane and whatever each part of it came back as, is one L2 Module document: its units
# in order -- a function rebuilt as a tree (a MethodDecl with a body), one whose source is still text
# (a MethodDecl with `text`), what stays machine code (a MachineFragment: its assembly, or only its name
# and place), a comment at its place. Every lane's module is this one model (docs/ARCHITECTURE.md, "one
# Module model"): the decompiler's (build/module.module_document), the readable pass's
# (source/readable.Readable), the 16-bit port's (port/asm16.Port), a library's members
# (exact/library.Member), and every assembly writer's (l2.Assembly, written by backends/asm.py).
def module(units=(), header=None, origin=None):
    """A Module document. -> {"node": "Module", "units": [...]}"""
    d = {"node": "Module", "units": list(units)}
    if header is not None:
        d["header"] = header
    if origin is not None:
        d["origin"] = origin
    return d


def function(name, text=None, origin=None, comments=None, **fields):
    """A function of a module: a MethodDecl (kind "function"), its source as `text` while it is not a tree."""
    d = {"node": "MethodDecl", "name": name, "kind": "function"}
    if origin is not None:
        d["origin"] = origin
    if comments is not None:
        d["comments"] = list(comments)
    if text is not None:
        d["text"] = text
    d.update(fields)
    return d


def fragment(asm=None, name=None, syntax=None, origin=None, reason=None, why=None, length=None, within=None):
    """What stays machine code: a MachineFragment (L2_BINARY_TYPES.md section 7)."""
    d = {"node": "MachineFragment"}
    for k, v in (("origin", origin), ("name", name), ("reason", reason), ("why", why), ("length", length),
                 ("syntax", syntax), ("asm", asm), ("within", within)):
        if v is not None:
            d[k] = v
    return d


def comment(text):
    return {"node": "Comment", "text": text}


def units(doc, node=None):
    """The units of a Module document (those of one kind, given `node`)."""
    us = doc.get("units") or []
    us = us if isinstance(us, list) else [us]
    return [u for u in us if node is None or u.get("node") == node]


def texts(doc):
    """{name: text} of a module's functions whose source is text, in their order."""
    return {u["name"]: u["text"] for u in units(doc, "MethodDecl") if u.get("text") is not None}


class Made(object):
    """A module made compilable for a lane's build (decompile/c_text: compilable16, compilable32,
    compilable_powerpc), as the one module model: an L2 Module document, `doc` -- each function a
    MethodDecl with its C as `text` and its place as `origin` ({module, va}), in the module's
    order, the header the Module's -- and, beside it, what the lane keeps
    of the module as attributes (16-bit: near, wide, novalue, interrupts, selector_imports, ...;
    32-bit: imports, in_ecx, in_edx; PowerPC: externs, argc, image).

        m = l2.Made("lzexpand.dll", header, texts, order, by_va, where=...)
        m.texts / m.order / m.by_va / m.header / m.functions               views of m.doc
        m.texts[name] = text, m.texts = {...}, m.header = ...              rewrite m.doc (a copy of
                                                                           the module keeps its own)
        m.add(name, text, va)                                              one more function, last

    (A change makes a new document rather than changing the one there: copy.copy(m) and m share
    it until one of them changes.)"""

    def __init__(self, name="", header="", texts=None, order=None, by_va=None, **facts):
        self.name = name
        self.doc = module(header=header)
        texts = dict(texts or {})
        place = {n: va for va, n in (by_va or {}).items()}
        order = list(order) if order is not None else list(texts)
        listed = set(order)
        self.doc["units"] = [function(n, texts.get(n), origin={"module": name, "va": place[n]} if n in place else None)
                             for n in order + [n for n in texts if n not in listed]]
        self.__dict__.update(facts)

    def add(self, name, text, va=None):
        origin = {"module": self.name, "va": va} if va is not None else None
        self.doc = dict(self.doc, units=units(self.doc) + [function(name, text, origin=origin)])

    def _functions(self):
        return units(self.doc, "MethodDecl")

    def _view(self, key, make):
        """a view of the document as it is now (kept until the document is another)"""
        cache = self.__dict__.setdefault("_views", {})
        got = cache.get(key)
        if got is None or got[0] is not self.doc or got[1] is not self:
            got = cache[key] = (self.doc, self, make())
        return got[2]

    def __getstate__(self):
        return {k: v for k, v in self.__dict__.items() if k != "_views"}

    def __copy__(self):
        new = object.__new__(type(self))
        new.__dict__.update(self.__getstate__())
        return new

    @property
    def texts(self):
        return self._view("texts", lambda: _Texts(self))

    @texts.setter
    def texts(self, new):
        new = dict(new)
        us = [dict(u, text=new[u["name"]]) if u.get("node") == "MethodDecl" and u["name"] in new else u
              for u in units(self.doc)]
        have = {u["name"] for u in us if u.get("node") == "MethodDecl"}
        us += [function(n, t) for n, t in new.items() if n not in have]
        self.doc = dict(self.doc, units=us)

    @property
    def header(self):
        return self.doc.get("header") or ""

    @header.setter
    def header(self, text):
        self.doc = dict(self.doc, header=text)

    @property
    def order(self):
        return self._view("order", lambda: [u["name"] for u in self._functions()])

    @property
    def by_va(self):
        return self._view("by_va", lambda: {u["origin"]["va"]: u["name"] for u in self._functions()
                                            if "va" in (u.get("origin") or {})})

    @property
    def functions(self):
        """{address: (name, text)}"""
        return self._view("functions", lambda: {u["origin"]["va"]: (u["name"], u.get("text")) for u in self._functions()
                                                if "va" in (u.get("origin") or {})})


class _Texts(dict):
    """Made.texts: {name: C text}, in the module's order; setting one rewrites the module's document."""

    def __init__(self, made):
        dict.__init__(self, texts(made.doc))
        self._made = made

    def __setitem__(self, name, text):
        dict.__setitem__(self, name, text)
        self._made.texts = {name: text}

    def update(self, *args, **kw):
        new = dict(*args, **kw)
        dict.update(self, new)
        self._made.texts = new

    def __reduce__(self):                                # (pickled as the plain dict it is)
        return dict, (dict(self),)


class Assembly(list):
    """A module being written as assembly language, built as its L2 Module as it goes.

        L = l2.Assembly(["; head ..."], syntax="nasm", module="lzexpand.dll")
        L.append(...); L.extend(...)                 # lines, as to a list (every writer's helpers take one)
        L.part("seg1", origin={"module": ..., "segment": 1, "offset": 0})   # the next fragment begins here
        doc = L.document()                           # a Module of MachineFragments: backends/asm.py writes it

    Each part() begins a MachineFragment at the next line; the lines before the first part are the
    head's. A part with no lines is left out, so the fragments joined by a line break are the lines
    joined, as they were written."""

    def __init__(self, lines=(), syntax=None, module=None, name="head", reason="head"):
        list.__init__(self, lines)
        self.syntax, self.module = syntax, module
        self._parts = [(0, name, reason, None, None)]

    def part(self, name, reason="section", origin=None, length=None):
        self._parts.append((len(self), name, reason, origin, length))

    def document(self):
        out = []
        ends = [p[0] for p in self._parts[1:]] + [len(self)]
        for (start, name, reason, origin, length), end in zip(self._parts, ends):
            if end > start:
                out.append(fragment("\n".join(self[start:end]), name=name, syntax=self.syntax, origin=origin,
                                    reason=reason, length=length))
        return module(out)
