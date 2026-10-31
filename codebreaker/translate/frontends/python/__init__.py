"""The Python front end: CPython's `ast` into L2, typed by inference, refusing what it cannot type.

    from codebreaker.translate.frontends import python as pyfe
    prog = pyfe.program_of_tree('codebreaker', 'codebreaker')      # every module of a package
    prog.solve()                                                    # types, to a fixpoint
    results = pyfe.translate(prog, ['codebreaker.formats.ne'])      # {module: ModuleResult}
    r = results['codebreaker.formats.ne']
    r.unit                       an ir.Unit: records, classes, the module's functions and names
    r.translated / r.refused     {function: MethodDecl} / {function: Issue (class, detail, line)}

    from codebreaker.translate.backends import cpp17
    header, source = cpp17.write_unit(r.unit)

docs/PYTHON_FRONTEND.md says how types are found and what is refused.
"""
import ast

from codebreaker.translate import ir
from codebreaker.translate.frontends.python import pytypes as P
from codebreaker.translate.frontends.python.program import Program
from codebreaker.translate.frontends.python import lower as L


def program_of_tree(root, package):
    prog = Program()
    prog.add_tree(root, package)
    prog.index()
    return prog


def program_of_sources(sources):
    """{module name: source text} -> an indexed Program (tests)"""
    prog = Program()
    for name, src in sources.items():
        prog.add_module(name, '<%s>' % name, src)
    prog.index()
    return prog


class _ModFn(object):
    """a module's top level, seen as a function (to lower its names' values)"""

    def __init__(self, m):
        self.qname = m.name + '.<module>'
        self.module = m
        self.cls = None
        self.kind = 'function'
        self.params = []
        self.kwonly = []
        self.name = '<module>'
        self.node = m.tree


def translate(prog, modules=None, int_policy='strict', fns=None):
    """Lower the program's functions (those of `modules`, or all) -> {module: L.ModuleResult}."""
    from codebreaker.translate.frontends.python import infer
    ctx = L.Context(prog, int_policy)
    wanted = set(modules) if modules is not None else set(prog.modules)
    todo = [f for f in prog.fns.values() if (fns is None or f.qname in fns)]
    lowered = L.lower_functions(prog, ctx, todo)
    # module names the translations read: lowered as their modules' top level, or refused with them
    bad_globals = {}
    global_decls = {}
    needed = set()
    for q, (m, fl) in lowered.items():
        if fl is not None and not isinstance(m, L.Refused):
            needed |= fl.globals_used
    mevals = {}
    for (mod, name) in sorted(needed):
        mm = prog.modules[mod]
        values = mm.assigns.get(name, [])
        if len(values) != 1 or values[0] is None:
            bad_globals[(mod, name)] = L.Refused('dynamic', 'module name %s assigned %d times' % (name, len(values)))
            continue
        if mod not in mevals:
            mevals[mod] = infer.ModuleEval(prog, mm, record=True).run()
        fl = L.FnLower(ctx, _ModFn(mm), mevals[mod])
        try:
            t = ctx.ty(prog.global_type(mod, name), values[0].lineno, 'module name ' + name)
            init = fl.coerce(fl.expr(values[0]), t)
            if fl.pre:
                raise L.Refused('syntax', 'module name %s needs statements' % name, values[0].lineno)
            global_decls[(mod, name)] = ir.FieldDecl(sym=ir.Sym('global', name, mod, True, type=t), type=t, init=init,
                                                     mods=['static'] + (['const'] if t.kind in ('i64', 'bool', 'f64') or
                                                                        t.name in ('py.str', 'py.bytes', 'py.tuple') else []),
                                                     line=values[0].lineno)
            for g in fl.globals_used:
                if g not in needed and g != (mod, name):
                    bad_globals[(mod, name)] = L.Refused('syntax', 'module name %s refers to another' % name)
        except L.Refused as r:
            bad_globals[(mod, name)] = r
    for q, (m, fl) in list(lowered.items()):
        if fl is None or isinstance(m, L.Refused):
            continue
        for g in sorted(fl.globals_used):
            if g in bad_globals:
                lowered[q] = (L.Refused('calls-refused', 'module name %s (%s)' % (g[1], bad_globals[g].issue.reason[:60])), fl)
                break
    L.close_refusals(prog, lowered)
    used = used_types(prog, ctx, lowered, global_decls)
    results = {}
    todo = sorted(m for m in wanted if m in prog.modules)
    while todo:                                          # (and the modules whose declarations they need)
        mod = todo.pop(0)
        if mod in results:
            continue
        results[mod] = build_unit(prog, ctx, mod, lowered, global_decls, used)
        for dep in results[mod].unit.imports:
            if dep not in results and dep not in todo:
                todo.append(dep)
    return results


def used_types(prog, ctx, lowered, global_decls):
    """the qualified names of the records and classes every translated function (and every module name
    they read) uses -- and the types of those records' fields and classes' attributes, to a fixpoint"""
    used = set()
    for q, (md, fl) in lowered.items():
        if fl is not None and not isinstance(md, L.Refused):
            used |= _types_in(md)
    for d in global_decls.values():
        used |= _types_of(d.type)
    recs = dict(('%s.%s' % (ctx.records.module[s], n), s) for s, n in ctx.records.names.items())
    todo = list(used)
    while todo:
        q = todo.pop()
        more = set()
        if q in recs:
            for key, ft, req in ctx.records.fields(recs[q]):
                try:
                    more |= _types_of(ctx.ty(ft, 0, 'field ' + key))
                except L.Refused:
                    pass
        elif q in prog.classes:
            for attr in prog.attrs.get(q, {}):
                try:
                    more |= _types_of(ctx.ty(prog.attr_type(q, attr), 0, 'attribute ' + attr))
                except L.Refused:
                    pass
            for b in prog.classes[q].bases:
                more.add(b)
        for x in more - used:
            used.add(x)
            todo.append(x)
    return used


def build_unit(prog, ctx, mod, lowered, global_decls, used_all=None):
    m = prog.modules[mod]
    r = L.ModuleResult(mod)
    fns = [f for f in prog.fns.values() if f.module is m]
    for f in fns:
        got = lowered.get(f.qname)
        if got is None:
            continue
        md, fl = got
        r.lines[f.qname] = f.lines
        if isinstance(md, L.Refused):
            r.refused[f.qname] = md.issue
        else:
            r.translated[f.qname] = md
            r.calls[f.qname] = fl.calls
    types = []
    # records declared here that the translation uses
    used = set()
    for md in r.translated.values():
        used |= _types_in(md)
    for (gm, gname), d in global_decls.items():
        if gm == mod:
            used |= _types_of(d.type)
    everywhere = used_all if used_all is not None else used
    recs = []
    for s, name in ctx.records.names.items():
        q = '%s.%s' % (ctx.records.module[s], name)
        if ctx.records.module[s] == mod and (q in everywhere or q in used or _any_ref(prog, ctx, s, used)):
            recs.append((s, q, name))
    for s, q, name in recs:
        members = []
        try:
            for key, ft, req in ctx.records.fields(s):
                t = ctx.ty(ft, 0, 'field ' + key)
                members.append(ir.FieldDecl(sym=ir.Sym('field', key, q, type=t), type=t, init=None,
                                            mods=[] if req else ['optional']))
        except L.Refused as e:
            members = [ir.Comment(text='a field has no type: %s' % e.issue.reason)]
        types.append(ir.ClassDecl(name=name, qname=q, kind='record', mods=['ref'] if ctx.records.is_ref(s) else [],
                                  typeparams=[], extends=None, implements=[], members=members, outer=None))
    # classes
    for c in m.classes.values():
        methods = [r.translated[f.qname] for f in c.methods.values() if f.qname in r.translated]
        if not methods and c.qname not in used and c.qname not in everywhere:
            continue
        members = []
        attrs = dict(prog.attrs.get(c.qname, {}))
        for name in c.class_attrs:                       # a class value an instance assigns is the instance's
            if name in prog.assigned_attrs_any(c.qname) and name not in attrs:
                attrs[name] = prog.class_attr_types.get((c.qname, name), P.BOTTOM)
        for attr, t in sorted(attrs.items(), key=lambda kv: _attr_order(prog, c, kv[0])):
            try:
                it = ctx.ty(prog.attr_type(c.qname, attr), 0, 'attribute ' + attr)
            except L.Refused as e:
                members.append(ir.Comment(text='%s: not translated (%s)' % (attr, e.issue.reason)))
                continue
            init = None
            v = c.class_attrs.get(attr)
            if isinstance(v, ast.Constant) and v.value is not None and not isinstance(v.value, bytes):
                init = L.lit(v.value, it)
            members.append(ir.FieldDecl(sym=ir.Sym('field', attr, c.qname, type=it), type=it, init=init, mods=[]))
        for name, v in c.class_attrs.items():
            if name in attrs or prog.attr_owner(c.qname, name) != c.qname:
                continue
            t = prog.class_attr_types.get((c.qname, name))
            if t is None or not isinstance(v, ast.Constant):
                continue
            try:
                it = ctx.ty(t, 0, 'class attribute ' + name)
            except L.Refused:
                continue
            members.append(ir.FieldDecl(sym=ir.Sym('field', name, c.qname, True, type=it), type=it,
                                        init=L.lit(v.value, it), mods=['static', 'const']))
        members += methods
        base = ir.Type('class', c.bases[0]) if c.bases else None
        mods = []
        if c.exception:
            mods.append('exception:' + c.exception)
        for f in c.methods.values():
            if f.qname in r.refused:
                members.append(ir.Comment(text='not translated: %s -- %s' % (f.name, r.refused[f.qname].reason)))
        types.append(ir.ClassDecl(name=c.name, qname=c.qname, kind='class', mods=mods, typeparams=[], extends=base,
                                  implements=[], members=members, outer=None))
    # the module's functions and names
    members = []
    for (gm, gname), d in sorted(global_decls.items(), key=lambda kv: kv[1].line):
        if gm == mod:
            members.append(d)
    for f in m.functions.values():
        if f.qname in r.translated:
            members.append(r.translated[f.qname])
        elif f.qname in r.refused:
            members.append(ir.Comment(text='not translated: %s -- %s' % (f.name, r.refused[f.qname].reason)))
    types.append(ir.ClassDecl(name=mod.rsplit('.', 1)[-1], qname=mod, kind='module', mods=[], typeparams=[],
                              extends=None, implements=[], members=members, outer=None))
    deps = set()
    for q in used:
        owner = q if q in prog.modules else q.rsplit('.', 1)[0]      # (a module's own names: the module)
        if owner != mod and owner in prog.modules:
            deps.add(owner)
    for md in r.translated.values():
        for x in ir.walk(md):
            s = getattr(x, 'sym', None)
            if isinstance(s, ir.Sym) and s.owner and s.owner != 'py':
                own = s.owner if s.owner in prog.modules else s.owner.rsplit('.', 1)[0]
                if own in prog.modules and own != mod:
                    deps.add(own)
    for decl in types:                                   # what the declarations name: their modules too
        names = set()
        if decl.extends is not None:
            names.add(decl.extends.name)
        for x in decl.members:
            if isinstance(x, ir.FieldDecl):
                names |= _types_of(x.type)
        for q in names:
            owner = q if q in prog.modules else q.rsplit('.', 1)[0]
            if owner != mod and owner in prog.modules:
                deps.add(owner)
    r.unit = ir.Unit(path=m.path, package=mod, imports=sorted(deps), types=types, lang='python')
    return r


def _attr_order(prog, c, attr):
    """attributes in the order the class's methods first assign them (__init__ first)"""
    order = []
    meths = sorted(c.methods.values(), key=lambda f: (f.name != '__init__', f.node.lineno))
    for f in meths:
        for n in ast.walk(f.node):
            if isinstance(n, ast.Attribute) and isinstance(n.ctx, ast.Store) and n.attr not in order:
                order.append(n.attr)
    return order.index(attr) if attr in order else len(order)


def _types_of(t, out=None):
    out = set() if out is None else out
    if t is None:
        return out
    if t.kind == 'class':
        if not t.name.startswith('py.'):
            out.add(t.name)
        for a in t.args:
            _types_of(a, out)
    return out


def _types_in(md):
    out = set()
    for x in ir.walk(md):
        t = getattr(x, 'type', None)
        if isinstance(t, ir.Type):
            _types_of(t, out)
        if isinstance(x, (ir.Local, ir.Param)):
            _types_of(x.type, out)
    _types_of(md.ret, out)
    for p in md.params or []:
        _types_of(p.type, out)
    return out


def _any_ref(prog, ctx, s, used):
    """a record whose type appears inside another used type (a field of a record that is used)"""
    q = '%s.%s' % (ctx.records.module[s], ctx.records.names[s])
    for s2, name in ctx.records.names.items():
        q2 = '%s.%s' % (ctx.records.module[s2], name)
        if q2 in used and s2 != s:
            for key, ft, req in ctx.records.fields(s2):
                try:
                    if q in _types_of(ctx.ty(ft, 0)):
                        return True
                except L.Refused:
                    pass
    return False
