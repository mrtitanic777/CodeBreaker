"""program.py -- the modules being translated, their symbols, and the facts inference gathers about them.

A Program is a set of Python modules parsed with CPython's `ast`, each with its symbol table (functions,
classes, module-level names, imports resolved to the program's own modules or to outside libraries).
The facts are what inference (infer.py) learns, program-wide, to a fixpoint:

    params[fn]      the type of each parameter: its annotation, else the join of every call site's argument
    returns[fn]     the join of every value it returns
    attrs[cls]      each attribute of a class: the join of every value stored to it
    globals[mod]    each module-level name
    containers      per container object class (sites joined by union-find): a list's or set's element,
                    a dict's keys and values, a record's fields and which fields every creation sets
"""
import ast
import os

from codebreaker.translate.frontends.python import pytypes as P

STDLIB = frozenset('''abc argparse array ast base64 binascii bisect builtins bz2 calendar codecs collections
concurrent configparser contextlib copy csv ctypes dataclasses datetime decimal difflib dis email enum errno
fnmatch fractions functools gc getpass glob gzip hashlib heapq hmac html http importlib inspect io ipaddress
itertools json keyword locale logging lzma math mimetypes mmap multiprocessing numbers operator os pathlib
pickle platform pprint queue random re reprlib secrets select shlex shutil signal socket sqlite3 ssl stat
statistics string struct subprocess sys sysconfig tarfile tempfile textwrap threading time timeit tkinter
token tokenize traceback types typing unicodedata unittest urllib uuid warnings weakref winreg xml zipfile
zlib zipimport __future__ msvcrt _thread'''.split())


class Fn(object):
    """A function or method of the program."""

    def __init__(self, qname, node, module, cls=None):
        self.qname = qname
        self.node = node
        self.module = module
        self.cls = cls                       # the Class it is a method of
        self.name = node.name
        self.kind = 'function'
        decos = [_deco_name(d) for d in node.decorator_list]
        self.decorators = decos
        if cls is not None:
            self.kind = 'method'
            if 'staticmethod' in decos:
                self.kind = 'static'
            elif 'classmethod' in decos:
                self.kind = 'classmethod'
            elif 'property' in decos:
                self.kind = 'property'
        a = node.args
        self.params = [x.arg for x in a.posonlyargs + a.args]
        self.kwonly = [x.arg for x in a.kwonlyargs]
        self.annotations = dict((x.arg, x.annotation) for x in a.posonlyargs + a.args + a.kwonlyargs if x.annotation)
        nd = len(a.defaults)
        self.defaults = dict(zip(self.params[len(self.params) - nd:], a.defaults)) if nd else {}
        for x, d in zip(a.kwonlyargs, a.kw_defaults):
            if d is not None:
                self.defaults[x.arg] = d
        self.varargs = a.vararg.arg if a.vararg else None
        self.kwargs = a.kwarg.arg if a.kwarg else None
        self.lines = (node.end_lineno or node.lineno) - node.lineno + 1

    @property
    def bound(self):
        """the parameters a caller passes (a method's self, a classmethod's cls not among them)"""
        return self.params[1:] if self.kind in ('method', 'property', 'classmethod') else self.params

    def __repr__(self):
        return '<Fn %s>' % self.qname


class Class(object):
    def __init__(self, qname, node, module):
        self.qname = qname
        self.node = node
        self.module = module
        self.name = node.name
        self.base_nodes = node.bases
        self.bases = []                      # qualified names of the program's classes it derives from
        self.exception = None                # the builtin exception it derives from, if it is one
        self.methods = {}
        self.class_attrs = {}                # name -> the value node assigned in the class body
        self.subclasses = []
        self.decorators = [_deco_name(d) for d in node.decorator_list]
        self.foreign_base = None             # a base the program does not have (a library class)


class Module(object):
    def __init__(self, name, path, source):
        self.name = name
        self.path = path
        self.source = source
        self.tree = ast.parse(source)
        self.functions = {}                  # name -> Fn
        self.classes = {}                    # name -> Class
        self.assigns = {}                    # name -> [value nodes] at module level
        self.imports = {}                    # local name -> ('module', qname) | ('obj', module, name) | ('extern', name)
        self.statements = []                 # module-level statements that are not defs, imports or names
        self.is_package = os.path.basename(path or '') == '__init__.py'


def _deco_name(d):
    if isinstance(d, ast.Name):
        return d.id
    if isinstance(d, ast.Attribute):
        return d.attr
    if isinstance(d, ast.Call):
        return _deco_name(d.func)
    return '?'


class Program(object):
    def __init__(self):
        self.modules = {}                    # qualified module name -> Module
        self.fns = {}                        # qualname -> Fn
        self.classes = {}                    # qualname -> Class
        # facts
        self.params = {}                     # fn qname -> {param: T}
        self.returns = {}                    # fn qname -> T
        self.attrs = {}                      # class qname -> {attr: T}
        self.globals = {}                    # module -> {name: T}
        self.raises = {}                     # fn qname -> True when it can raise (C++ needs nothing more)
        # containers: union-find over sites
        self.parent = {}
        self.ckind = {}                      # root -> 'list' | 'dict' | 'set'
        self.elem = {}                       # list/set element; dict values when used as a dict
        self.keys = {}                       # dict keys
        self.fields = {}                     # root -> {key: T} (insertion order)
        self.required = {}                   # root -> frozenset of keys every creation sets (None: no creation seen)
        self.dynamic = {}                    # root -> why it is a dict and not a record
        self.hints = {}                      # root -> [name hints]
        self.site_of_node = {}
        self.site_where = {}
        self.site_node = {}                  # site -> the AST node that is it
        self.fixed = set()                   # sites from an annotation: their element types do not grow
        self.changed = False
        self._keep = []
        self.index_params = {}
        self.literal_keys = {}               # dict display node -> its constant keys
        self.spread_keys = {}                # site -> keys a ** spread brought, set by every creation
        self.spread_from = {}
        self.round = 0
        self.class_attr_types = {}           # (class, name) -> the type of a class-level value
        self.default_types = {}              # (fn, param) -> the type of its default
        self.ann_cache = {}
        self._blocks = {}
        self._assigned_attrs = {}
        self.crashes = {}                    # fn -> the front end error that stopped it
        self.builder_targets = set()         # item stores that fill a record right after its creation
        self.mutated = set()
        self.param_poison = {}               # (fn, param) -> (a caller, why) that passed a value of no type
        self.poisoned = {}                   # (class, attribute) -> a function that stores it a value of no type
        self.poisoned_sites = {}             # container root -> a function that adds it an item of no type
        self.int_policy = 'strict'          # 'checked': ints beyond int64 raise OverflowError in C++
        self.reads = None                    # the facts the running evaluation reads (worklist solving)
        self.dirty = set()                   # the facts changed this round
        self._fn_locals = {}
        self._access = {}
        self.suspend = 0                     # > 0 while a loop body runs toward its fixpoint                 # records changed after they were built (reference semantics)               # fn qname -> {param} it indexes with at once (a successful call bounds it)

    # ------------------------------------------------------------------------------------ loading
    def add_module(self, name, path=None, source=None):
        if source is None:
            with open(path, encoding='utf-8') as f:
                source = f.read()
        m = Module(name, path, source)
        self.modules[name] = m
        return m

    def add_tree(self, root, package):
        """Every .py file under `root` (the folder of package `package`)."""
        for dp, dirs, files in os.walk(root):
            dirs[:] = sorted(d for d in dirs if d != '__pycache__')
            for f in sorted(files):
                if not f.endswith('.py'):
                    continue
                p = os.path.join(dp, f)
                rel = os.path.relpath(p, root)[:-3].replace(os.sep, '.')
                if rel.endswith('__init__'):
                    rel = rel[:-len('__init__')].rstrip('.')
                name = package + ('.' + rel if rel else '')
                try:
                    self.add_module(name, p)
                except SyntaxError:
                    pass

    def index(self):
        """Build every module's symbol table, then resolve class bases and imports."""
        for m in self.modules.values():
            for st in m.tree.body:
                self._index_stmt(m, st)
        for c in self.classes.values():
            for b in c.base_nodes:
                r = self.resolve_expr_name(c.module, b)
                if r and r[0] == 'class':
                    c.bases.append(r[1])
                    self.classes[r[1]].subclasses.append(c.qname)
                elif isinstance(b, ast.Name) and b.id in EXCEPTIONS:
                    c.exception = b.id
                elif isinstance(b, ast.Name) and b.id == 'object':
                    pass
                else:
                    c.foreign_base = ast.unparse(b)
        for c in self.classes.values():                          # exceptions derived from exceptions
            k = c
            while k.exception is None and k.bases:
                k = self.classes[k.bases[0]]
            c.exception = k.exception if c.exception is None else c.exception
        for f in self.fns.values():
            self.index_params[f.qname] = _indexing_params(f)
            f.none_aware = _none_aware(f)

    # ------------------------------------------------------------------------------------ type lookups
    def annotation_type(self, f, name):
        node = f.annotations.get(name)
        if node is None:
            return P.BOTTOM
        return self.ann_to_type(f.module, node, node)

    def ann_to_type(self, m, a, site_node):
        key = id(a)
        if key in self.ann_cache:
            return self.ann_cache[key]
        t = self._ann(m, a)
        self.ann_cache[key] = t
        self._keep.append(a)
        return t

    def _ann(self, m, a):
        if isinstance(a, ast.Constant) and isinstance(a.value, str):
            try:
                inner = ast.parse(a.value, mode='eval').body
            except SyntaxError:
                return P.BOTTOM
            self._keep.append(inner)
            return self._ann(m, inner)
        if isinstance(a, ast.Constant) and a.value is None:
            return P.NONE
        name = a.id if isinstance(a, ast.Name) else (a.attr if isinstance(a, ast.Attribute) else None)
        if name is not None:
            simple = {'int': P.INT(), 'str': P.STR, 'bytes': P.BYTES, 'bytearray': P.BYTEARRAY, 'bool': P.BOOL,
                      'float': P.FLOAT, 'None': P.NONE}
            if name in simple:
                return simple[name]
            if name in ('list', 'List', 'dict', 'Dict', 'set', 'Set'):
                kind = name.lower()
                s = self.new_site(a, kind, (m.name, getattr(a, 'lineno', 0)))
                if kind == 'dict':
                    self.make_dynamic(s, 'annotated as a dict')
                return P.T(kind, site=s)
            if isinstance(a, ast.Name):
                r = self.resolve_global(m, name)
                if r and r[0] == 'class':
                    return P.T('inst', name=r[1])
            return P.BOTTOM
        if isinstance(a, ast.BinOp) and isinstance(a.op, ast.BitOr):
            return self.join(self._ann(m, a.left), self._ann(m, a.right))
        if isinstance(a, ast.Subscript):
            v = a.value
            head = v.id if isinstance(v, ast.Name) else (v.attr if isinstance(v, ast.Attribute) else '')
            args = a.slice.elts if isinstance(a.slice, ast.Tuple) else [a.slice]
            if head == 'Optional':
                return P.opt(self._ann(m, args[0]))
            if head == 'Union':
                t = P.BOTTOM
                for x in args:
                    t = self.join(t, self._ann(m, x))
                return t
            if head in ('list', 'List', 'set', 'Set', 'Sequence', 'Iterable'):
                kind = 'set' if head in ('set', 'Set') else 'list'
                s = self.new_site(a, kind, (m.name, getattr(a, 'lineno', 0)))
                self.grow(self.elem, s, self._ann(m, args[0]))
                return P.T(kind, site=s)
            if head in ('dict', 'Dict', 'Mapping'):
                s = self.new_site(a, 'dict', (m.name, getattr(a, 'lineno', 0)))
                self.make_dynamic(s, 'annotated as a dict')
                if len(args) == 2:
                    self.grow(self.keys, s, self._ann(m, args[0]))
                    self.grow(self.elem, s, self._ann(m, args[1]))
                return P.T('dict', site=s)
            if head in ('tuple', 'Tuple'):
                if any(isinstance(x, ast.Constant) and x.value is Ellipsis for x in args):
                    return P.top('variable-length tuple annotation')
                return P.tuple_of([self._ann(m, x) for x in args])
        return P.BOTTOM

    def param_type(self, f, name):
        self.read(('p', f.qname))
        self.read(('d',))
        t = self.params.get(f.qname, {}).get(name, P.BOTTOM)
        if name in f.annotations:
            at = self.annotation_type(f, name)
            if at.kind == 'int':
                t = t if t.kind == 'int' else at
            elif at.kind != 'bottom':
                t = at
        if name in f.defaults:
            d = self.default_types.get((f.qname, name))
            if d is not None:
                t = self.join(t, d)
        return t

    def global_type(self, mod, name):
        self.read(('g', mod))
        return self.globals.get(mod, {}).get(name, P.BOTTOM)

    def find_method(self, cq, name):
        for c in self.mro(cq):
            f = self.classes[c].methods.get(name)
            if f is not None:
                return f
        return None

    def assigned_attrs(self, cq):
        got = self._assigned_attrs.get(cq)
        if got is None:
            got = set()
            for f in self.classes[cq].methods.values():
                if not f.params or f.kind == 'static':
                    continue
                me = f.params[0]
                for n in ast.walk(f.node):
                    if isinstance(n, ast.Attribute) and isinstance(n.ctx, ast.Store) and isinstance(n.value, ast.Name) \
                            and n.value.id == me:
                        got.add(n.attr)
            self._assigned_attrs[cq] = got
        return got

    def assigned_attrs_any(self, cq):
        """attributes assigned on instances by the class or any class derived from it"""
        out = set(self.assigned_attrs(cq))
        for d in self.descendants(cq):
            out |= self.assigned_attrs(d)
        return out

    def attr_owner(self, cq, attr):
        """the most basic class that has the attribute (where C++ declares the member)"""
        if cq not in self.classes:
            return None
        for c in reversed(self.mro(cq)):
            if attr in self.classes[c].class_attrs or attr in self.assigned_attrs(c) or attr in self.attrs.get(c, {}):
                return c
        return None

    def attr_type(self, cq, attr):
        for c in self.mro(cq):
            self.read(('a', c))
        self.read(('ca',))
        owner = self.attr_owner(cq, attr)
        if owner is None:
            for d in self.descendants(cq):
                if attr in self.assigned_attrs(d):
                    return P.top('attribute %s only set by a subclass' % attr)
            return None
        if (owner, attr) in self.poisoned:
            return P.top('untyped:attribute %s is also stored a value of no single type (by %s)'
                         % (attr, self.poisoned[(owner, attr)].rsplit('.', 1)[-1]))
        t = self.attrs.get(owner, {}).get(attr, P.BOTTOM)
        for c in self.mro(cq):
            if attr in self.classes[c].class_attrs:
                t = self.join(t, self.class_attr_types.get((c, attr), P.BOTTOM))
        return t

    def class_attr_type(self, cq, attr):
        self.read(('ca',))
        return self.class_attr_types.get((cq, attr), P.BOTTOM)

    def block_of(self, st):
        return self._blocks.get(id(st))

    def map_blocks(self, node):
        for n in ast.walk(node):
            for field in ('body', 'orelse', 'finalbody'):
                lst = getattr(n, field, None)
                if isinstance(lst, list):
                    for s in lst:
                        self._blocks[id(s)] = lst

    # ------------------------------------------------------------------------------------ solving
    def solve(self, max_rounds=40, log=None):
        """Run every function, then round after round only those that read a fact the last round
        changed, until none changed. -> rounds run"""
        from codebreaker.translate.frontends.python import infer
        for f in self.fns.values():
            self.map_blocks(f.node)
        deps = {}                                            # fn or module -> the facts its last run read
        prev = None
        for r in range(max_rounds):
            self.round = r
            self.changed = False
            self.dirty = set()
            ran = 0
            for m in self.modules.values():
                key = ('mod', m.name)
                if prev is not None and not (deps.get(key, set()) & prev):
                    continue
                self.reads = set()
                self.suspend = 0
                infer.ModuleEval(self, m).run()
                deps[key] = self.reads
                self.reads = None
            for f in self.fns.values():
                if f.qname in self.crashes:
                    continue
                if prev is not None and not (deps.get(f.qname, set()) & prev):
                    continue
                ran += 1
                self.reads = set()
                self.suspend = 0
                try:
                    infer.Eval(self, f).run()
                except RecursionError:
                    self.crashes[f.qname] = 'recursion'
                except Exception as e:                       # a front end bug: the function is refused
                    self.crashes[f.qname] = '%s: %s' % (type(e).__name__, e)
                deps[f.qname] = self.reads
                self.reads = None
            if log:
                log('round %d: ran %d, %d facts changed' % (r, ran, len(self.dirty)))
            if not self.dirty:
                return r + 1
            prev = self.dirty
            # a merged container's old root is read under its new root from now on
            prev |= set(('s', self.find(k[1])) for k in list(prev) if k[0] == 's')
        return max_rounds

    def evaluate(self, f):
        """the recording pass of one function -> infer.Eval (types, resolutions, issues)"""
        from codebreaker.translate.frontends.python import infer
        self.suspend = 0
        try:
            return infer.Eval(self, f, record=True).run()
        finally:
            self.suspend = 0

    def _index_stmt(self, m, st):
        if isinstance(st, (ast.FunctionDef, ast.AsyncFunctionDef)):
            f = Fn(m.name + '.' + st.name, st, m)
            if isinstance(st, ast.AsyncFunctionDef):
                f.kind = 'async'
            m.functions[st.name] = f
            self.fns[f.qname] = f
        elif isinstance(st, ast.ClassDef):
            c = Class(m.name + '.' + st.name, st, m)
            m.classes[st.name] = c
            self.classes[c.qname] = c
            for b in st.body:
                if isinstance(b, (ast.FunctionDef, ast.AsyncFunctionDef)):
                    f = Fn(c.qname + '.' + b.name, b, m, c)
                    c.methods[b.name] = f
                    self.fns[f.qname] = f
                elif isinstance(b, ast.Assign) and len(b.targets) == 1 and isinstance(b.targets[0], ast.Name):
                    c.class_attrs[b.targets[0].id] = b.value
                elif isinstance(b, ast.AnnAssign) and isinstance(b.target, ast.Name) and b.value is not None:
                    c.class_attrs[b.target.id] = b.value
        elif isinstance(st, ast.Import):
            for a in st.names:
                if a.asname:
                    m.imports[a.asname] = self._module_ref(a.name)
                else:
                    top = a.name.split('.')[0]
                    m.imports[top] = self._module_ref(top)
        elif isinstance(st, ast.ImportFrom):
            base = self._from_base(m, st)
            for a in st.names:
                if a.name == '*':
                    continue
                local = a.asname or a.name
                full = base + '.' + a.name if base else a.name
                if full in self.modules:
                    m.imports[local] = ('module', full)
                elif base in self.modules:
                    m.imports[local] = ('obj', base, a.name)
                else:
                    m.imports[local] = ('extern', full)
        elif isinstance(st, ast.Assign):
            for t in st.targets:
                for n in _names(t):
                    m.assigns.setdefault(n, []).append(st.value if isinstance(t, ast.Name) else None)
        elif isinstance(st, ast.AnnAssign) and isinstance(st.target, ast.Name):
            m.assigns.setdefault(st.target.id, []).append(st.value)
        elif isinstance(st, (ast.If, ast.Try)):
            # `if TYPE_CHECKING:` / `try: import x except ImportError:` -- index what they define
            for b in getattr(st, 'body', []) + getattr(st, 'orelse', []):
                self._index_stmt(m, b)
            for h in getattr(st, 'handlers', []):
                for b in h.body:
                    self._index_stmt(m, b)
            m.statements.append(st)
        elif isinstance(st, ast.Expr) and isinstance(st.value, ast.Constant):
            pass                                                 # a docstring
        else:
            m.statements.append(st)

    def _module_ref(self, name):
        if name in self.modules:
            return ('module', name)
        top = name.split('.')[0]
        return ('stdmod', name) if top in STDLIB else ('extern', name)

    def _from_base(self, m, st):
        if not st.level:
            return st.module or ''
        parts = m.name.split('.')
        keep = len(parts) - st.level + (1 if m.is_package else 0)
        base = '.'.join(parts[:keep])
        return base + ('.' + st.module if st.module else '')

    def resolve_global(self, m, name):
        """What a module-level name refers to: ('fn', qname) ('class', qname) ('global', module, name)
        ('module', qname) ('stdmod', name) ('extern', name), or None."""
        if name in m.functions:
            return ('fn', m.functions[name].qname)
        if name in m.classes:
            return ('class', m.classes[name].qname)
        if name in m.assigns:
            return ('global', m.name, name)
        imp = m.imports.get(name)
        if imp is None:
            return None
        if imp[0] == 'obj':
            other = self.modules[imp[1]]
            if imp[2] in other.functions or imp[2] in other.classes or imp[2] in other.assigns or imp[2] in other.imports:
                if imp[2] in other.imports and not (imp[2] in other.functions or imp[2] in other.classes or imp[2] in other.assigns):
                    return self.resolve_global(other, imp[2])
                return self.resolve_global(other, imp[2])
            return ('missing', imp[1] + '.' + imp[2])
        if imp[0] == 'extern':
            top = imp[1].split('.')[0]
            if top in STDLIB:
                return ('stdobj', imp[1])
        return imp

    def resolve_expr_name(self, m, node):
        if isinstance(node, ast.Name):
            return self.resolve_global(m, node.id)
        if isinstance(node, ast.Attribute):
            r = self.resolve_expr_name(m, node.value)
            if r and r[0] == 'module':
                return self.resolve_global(self.modules[r[1]], node.attr)
        return None

    # ------------------------------------------------------------------------------------ containers
    def find(self, s):
        p = self.parent
        root = s
        while p[root] != root:
            root = p[root]
        while p[s] != root:
            p[s], s = root, p[s]
        if self.reads is not None:
            self.reads.add(('s', root))
        return root

    def settle(self, old, new, depth=0):
        """A fact that keeps growing round after round: its integer bounds widened (to thresholds), a type
        that keeps nesting deeper given up -- so the rounds end."""
        if self.round <= 3 or new == old:
            return new
        if new.kind == 'int' and old.kind == 'int':
            return P.widen_int(old, new)
        if new.kind in ('tuple', 'opt') and old.kind == new.kind and len(new.args) == len(old.args):
            if depth > 6:
                return P.top('a type that keeps growing')
            return P.T(new.kind, [self.settle(a, b, depth + 1) for a, b in zip(old.args, new.args)])
        return new

    def touch(self, key):
        """a fact changed: what read it runs again"""
        self.changed = True
        self.dirty.add(key)

    def read(self, key):
        if self.reads is not None:
            self.reads.add(key)

    def ret(self, fq):
        self.read(('r', fq))
        return self.returns.get(fq, P.BOTTOM)

    def fn_locals(self, f):
        got = self._fn_locals.get(f.qname)
        if got is None:
            from codebreaker.translate.frontends.python.infer import assigned_names
            got = self._fn_locals[f.qname] = assigned_names(f.node) | set(f.params) | set(f.kwonly)
        return got

    def access_nodes(self, node):
        """the subscripts and calls of an expression or statement, outside nested scopes (cached)"""
        got = self._access.get(id(node))
        if got is None:
            got = []
            todo = [node]
            while todo:
                n = todo.pop()
                if isinstance(n, (ast.Lambda, ast.ListComp, ast.GeneratorExp, ast.SetComp, ast.DictComp)):
                    continue
                if isinstance(n, (ast.Subscript, ast.Call)):
                    got.append(n)
                todo.extend(ast.iter_child_nodes(n))
            got.reverse()
            self._access[id(node)] = got
        return got

    def new_site(self, node, kind, where=None):
        """The container site of an AST node (the same node is the same site every round)."""
        key = id(node)
        s = self.site_of_node.get(key)
        if s is None:
            s = len(self.parent) + 1
            self.site_of_node[key] = s
            self.parent[s] = s
            self.ckind[s] = kind
            self.elem[s] = P.BOTTOM
            self.keys[s] = P.BOTTOM
            self.fields[s] = {}
            self.required[s] = None
            self.site_where[s] = where
            self.site_node[s] = node
            self._keep.append(node)                          # (id() is only unique while the node lives)
            self.touch(('s', s))
        return s

    def union(self, a, b):
        a, b = self.find(a), self.find(b)
        if a == b:
            return a
        if a > b:
            a, b = b, a
        self.parent[b] = a
        self.touch(('s', a))
        self.touch(('s', b))
        if self.ckind[a] != self.ckind[b]:
            self.dynamic[a] = 'a %s and a %s flow together' % (self.ckind[a], self.ckind[b])
        self.elem[a] = self.join(self.elem[a], self.elem[b])
        self.keys[a] = self.join(self.keys[a], self.keys[b])
        fa = self.fields[a]
        for k, t in self.fields[b].items():
            fa[k] = self.join(fa.get(k, P.BOTTOM), t)
        ra, rb = self.required[a], self.required[b]
        self.required[a] = rb if ra is None else (ra if rb is None else ra & rb)
        if b in self.dynamic and a not in self.dynamic:
            self.dynamic[a] = self.dynamic[b]
        self.hints.setdefault(a, []).extend(self.hints.pop(b, []))
        if b in self.fixed:
            self.fixed.add(a)
        if b in self.mutated:
            self.mutated.add(a)
        if b in self.poisoned_sites and a not in self.poisoned_sites:
            self.poisoned_sites[a] = self.poisoned_sites[b]
        return a

    def mark_mutated(self, s):
        s = self.find(s)
        if s not in self.mutated:
            self.mutated.add(s)
            self.touch(('s', s))

    def grow(self, table, s, t):
        """Join `t` into a container fact; note a change. (Not while a loop is still being run to its
        fixpoint: its heads are not final, and a widened bound would stick.)"""
        s = self.find(s)
        if t.kind == 'top' and not self.suspend and s not in self.poisoned_sites:
            self.poisoned_sites[s] = t.why or '?'            # its readers cannot know what its items are
            self.touch(('s', s))
        if self.suspend or t.kind == 'top':
            return s
        old = table[s]
        new = self.settle(old, self.join(old, t))
        if new != old:
            table[s] = new
            self.touch(('s', s))
        return s

    def grow_field(self, s, key, t):
        s = self.find(s)
        if t.kind == 'top':
            if key not in self.fields[s]:
                self.fields[s][key] = P.BOTTOM
            if not self.suspend and s not in self.poisoned_sites:
                self.poisoned_sites[s] = t.why or '?'
                self.touch(('s', s))
            return
        if self.suspend:
            if key not in self.fields[s]:
                self.fields[s][key] = P.BOTTOM
            return
        f = self.fields[s]
        old = f.get(key, P.BOTTOM)
        new = self.settle(old, self.join(old, t))
        if key not in f or new != old:
            f[key] = new
            self.touch(('s', s))
        self.grow(self.elem, s, t)
        self.grow(self.keys, s, P.STR)

    def created(self, s, keys):
        s = self.find(s)
        if self.suspend:
            return
        keys = frozenset(keys)
        old = self.required[s]
        new = keys if old is None else old & keys
        if new != old:
            self.required[s] = new
            self.touch(('s', s))

    def make_dynamic(self, s, why):
        s = self.find(s)
        if s not in self.dynamic:
            self.dynamic[s] = why
            self.touch(('s', s))

    def is_record(self, s):
        s = self.find(s)
        return self.ckind[s] == 'dict' and s not in self.dynamic

    def hint(self, s, name):
        s = self.find(s)
        h = self.hints.setdefault(s, [])
        if name not in h:
            h.append(name)

    # ------------------------------------------------------------------------------------ the join
    def join(self, a, b):
        if a is b or a == b:
            return a
        ka, kb = a.kind, b.kind
        if ka == 'bottom':
            return b
        if kb == 'bottom':
            return a
        if ka == 'top':
            return a
        if kb == 'top':
            return b
        if ka == 'none' or kb == 'none':
            other = b if ka == 'none' else a
            if other.kind == 'opt':
                return other
            return P.opt(other)
        if ka == 'opt' or kb == 'opt':
            ia = P.strip_opt(a)
            ib = P.strip_opt(b)
            j = self.join(ia, ib)
            return j if j.kind == 'top' else P.opt(j)
        if ka != kb:
            if {ka, kb} == {'bytes', 'bytearray'}:
                return P.top('bytes and bytearray mixed')
            return P.top('%s and %s mixed' % (_tname(a), _tname(b)))
        if ka == 'int':
            return P.int_join(a, b)
        if ka in ('bytes', 'bytearray'):
            return P.T(ka, hi=None if a.hi is None or b.hi is None else max(a.hi, b.hi))
        if ka == 'tuple':
            if len(a.args) != len(b.args):
                return P.top('tuples of %d and %d items mixed' % (len(a.args), len(b.args)))
            parts = [self.join(x, y) for x, y in zip(a.args, b.args)]
            for p_ in parts:
                if p_.kind == 'top':
                    return p_
            return P.tuple_of(parts)
        if ka in ('list', 'dict', 'set'):
            return P.T(ka, site=self.union(a.site, b.site))
        if ka == 'inst':
            c = self.common_base(a.name, b.name)
            return P.T('inst', name=c) if c else P.top('instances of %s and %s mixed' % (a.name, b.name))
        return P.top('%s and %s mixed' % (_tname(a), _tname(b)))

    def common_base(self, x, y):
        anc = self.mro(x)
        for c in self.mro(y):
            if c in anc:
                return c
        return None

    def mro(self, cq):
        out, todo = [], [cq]
        while todo:
            c = todo.pop(0)
            if c in out or c not in self.classes:
                continue
            out.append(c)
            todo.extend(self.classes[c].bases)
        return out

    def descendants(self, cq):
        out, todo = [], [cq]
        while todo:
            c = todo.pop()
            for s in self.classes[c].subclasses:
                if s not in out:
                    out.append(s)
                    todo.append(s)
        return out

    def canon(self, t):
        """`t` with every container named by its root site (for comparing facts between rounds)."""
        if t.kind in ('list', 'dict', 'set'):
            r = self.find(t.site)
            return t if r == t.site else P.T(t.kind, site=r)
        if t.args:
            return P.T(t.kind, [self.canon(a) for a in t.args], t.lo, t.hi, t.name, t.site, t.why)
        return t


EXCEPTIONS = frozenset('''Exception BaseException ValueError KeyError IndexError TypeError RuntimeError
OSError IOError LookupError ArithmeticError ZeroDivisionError OverflowError NotImplementedError
AssertionError StopIteration EOFError FileNotFoundError AttributeError UnicodeDecodeError
UnicodeEncodeError PermissionError TimeoutError'''.split())


def _tname(t):
    if t.kind in ('list', 'dict', 'set'):
        return t.kind
    if t.kind == 'inst':
        return t.name.rsplit('.', 1)[-1]
    return t.kind


def _names(t):
    if isinstance(t, ast.Name):
        return [t.id]
    if isinstance(t, (ast.Tuple, ast.List)):
        out = []
        for e in t.elts:
            out += _names(e)
        return out
    return []


def _indexing_params(f):
    """The parameters a function indexes a sequence with in its first statement, before anything else
    can happen: once a call returns, the argument is within the sequence's bounds."""
    body = [s for s in f.node.body if not (isinstance(s, ast.Expr) and isinstance(s.value, ast.Constant))]
    if not body:
        return set()
    first = body[0]
    if not isinstance(first, (ast.Return, ast.Assign, ast.Expr)):
        return set()
    out = set()
    for n in ast.walk(first):
        if isinstance(n, ast.Subscript) and isinstance(n.slice, ast.Name):
            out.add(n.slice.id)
        if isinstance(n, ast.Call) and isinstance(n.func, ast.Attribute) and n.func.attr in ('unpack_from',) \
                and len(n.args) >= 3 and isinstance(n.args[2], ast.Name):
            out.add(n.args[2].id)
        if isinstance(n, ast.Call) and isinstance(n.func, ast.Attribute) and n.func.attr in ('unpack_from',) \
                and len(n.args) >= 3 and isinstance(n.args[2], ast.BinOp) and isinstance(n.args[2].left, ast.Name) \
                and isinstance(n.args[2].right, ast.Constant):
            pass
    return out & set(f.params)


def _none_aware(f):
    """The parameters a function is ready to get None for: a default of None, or a test of it against
    None or of its truth. Any other parameter is not None by contract -- a caller that may pass None is
    refused there, where the None comes from."""
    out = set(n for n, d in f.defaults.items() if isinstance(d, ast.Constant) and d.value is None)
    params = set(f.params) | set(f.kwonly)
    for n in ast.walk(f.node):
        if isinstance(n, ast.Compare):
            names = [x for x in [n.left] + n.comparators if isinstance(x, ast.Name) and x.id in params]
            nones = any(isinstance(x, ast.Constant) and x.value is None for x in [n.left] + n.comparators)
            if nones:
                out.update(x.id for x in names)
        elif isinstance(n, (ast.If, ast.While, ast.IfExp, ast.Assert)) and isinstance(n.test, ast.Name) \
                and n.test.id in params:
            out.add(n.test.id)
        elif isinstance(n, ast.UnaryOp) and isinstance(n.op, ast.Not) and isinstance(n.operand, ast.Name) \
                and n.operand.id in params:
            out.add(n.operand.id)
        elif isinstance(n, ast.BoolOp):
            out.update(v.id for v in n.values if isinstance(v, ast.Name) and v.id in params)
    return out
