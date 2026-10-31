"""ir.py -- the translator's intermediate representation: one typed tree for every source language.

Front ends build it, passes rewrite it, back ends read it (docs/TRANSLATOR_DESIGN.md). It keeps the
source's intent -- a lambda stays a lambda, a for-each stays a for-each, string concatenation is a
Concat -- and leaves lowering to the back end, which knows what its target can say.

Every name is resolved: a Name or a Call carries the Sym it refers to, as the source language's own
compiler decided (overloads, implicit `this`, static or not). Every expression carries its Type. Every
node carries the line and column it came from, so ledgers and the judge point at the original.

A construct a front end cannot lower yet becomes Unsupported (with its kind), never a guess: a back end
that meets one declines the method, and the old path handles it.
"""

# ---------------------------------------------------------------------------------------------- types
# Language-neutral primitive kinds. Java's char is u16, byte i8; C's are mapped by width and sign.
PRIMS = ('bool', 'i8', 'i16', 'i32', 'i64', 'u8', 'u16', 'u32', 'u64', 'f32', 'f64', 'f80', 'char')
# ('char' is C's plain char: its signedness is stated per target in `info['signed']`, never assumed)


class Type(object):
    """kind: one of PRIMS, or
      'void', 'null', 'any' (unresolved), 'dynamic' (known only at run time);
      'class'    a nominal type: `name` qualified, `args` its type arguments;
      'typevar', 'wildcard' (`bound`);
      'array'    `elem`, and info['count'] for a fixed-size array (C);
      and the binary-facing kinds of docs/L2_BINARY_TYPES.md (agreed with the decompiler, 2026-10-19):
      'ptr'      `elem` what it points at; info: space (SPACES), seg (a near pointer's segment),
                 ref (True: a C++ reference -- its reads and bindings explicit, as for a pointer);
      'selector' info['of']: what segment it names;
      'fn'       info: conv, distance, pops, loadds, params [{name, type, at}], result {type, at},
                 preserves, variadic;
      'fnptr'    info: space, fn (an 'fn' Type);
      'record'   an explicit layout: `name`, info: size, packed, role (struct union frame segment),
                 fields [{name, offset, type, pad}].
    Kind-specific details live in `info` (a dict), with the names the schema uses
    (codebreaker/translate/l2.schema.json)."""
    __slots__ = ('kind', 'name', 'args', 'elem', 'bound', 'info')

    def __init__(self, kind, name=None, args=(), elem=None, bound=None, **info):
        self.kind = kind
        self.name = name
        self.args = tuple(args)
        self.elem = elem
        self.bound = bound
        self.info = info

    def _key(self):
        return (self.kind, self.name, self.args, self.elem, self.bound, repr(sorted(self.info.items())))

    def __eq__(self, o):
        return isinstance(o, Type) and self._key() == o._key()

    def __hash__(self):
        return hash((self.kind, self.name, self.args, self.elem))

    def __repr__(self):
        if self.kind == 'array':
            return '%r[]' % (self.elem,)
        if self.kind in ('class', 'typevar'):
            return self.name + ('<%s>' % ', '.join(repr(a) for a in self.args) if self.args else '')
        if self.kind == 'wildcard':
            return '?' + (' %s %r' % self.bound if self.bound else '')
        return self.kind

    @property
    def is_prim(self):
        return self.kind in PRIMS

    @property
    def is_ref(self):
        return self.kind in ('class', 'array', 'typevar', 'null', 'wildcard')

    @property
    def simple(self):
        """The unqualified name of a class (`java.util.List` -> `List`), else the kind."""
        return self.name.rsplit('.', 1)[-1] if self.name else self.kind

    def dims(self):
        n, t = 0, self
        while t.kind == 'array':
            n, t = n + 1, t.elem
        return n, t


# docs/L2_BINARY_TYPES.md: a pointer's address space, and the memory model of an allocation
SPACES = ('flat', 'near', 'far', 'huge', 'real', 'io')
MEMORY = ('gc', 'owned', 'borrowed', 'stack', 'manual', 'emulated')
OVERFLOW = ('wrap', 'saturate', 'trap')      # an operator's integer rule; None: the source language's own


def pointer_to(t, space='flat', seg=None):
    return Type('ptr', elem=t, space=space, **({'seg': seg} if seg is not None else {}))


def array_of(t, dims=1):
    for _ in range(dims):
        t = Type('array', elem=t)
    return t


VOID = Type('void')
NULL = Type('null')
ANY = Type('any')
BOOL = Type('bool')
I32 = Type('i32')


# ---------------------------------------------------------------------------------------------- symbols
class Sym(object):
    """What a name refers to. kind: 'local', 'param', 'field', 'method', 'ctor', 'class', 'enumconst',
    'typevar', 'package', and 'property' / 'event' (C#: a member read and written through accessors).
    owner: the qualified name of the declaring type (or method, for a local).
    For a method or constructor: params (Types), ret (Type), varargs; refs: how each parameter is passed
    where any is not by value ('ref', 'out', 'in'; '' for by value). flags: what else the source says of
    it ('extension': a static method called as if on its first argument; 'indexer'; 'operator';
    'readonly'; 'virtual', 'abstract', 'override')."""
    __slots__ = ('kind', 'name', 'owner', 'static', 'type', 'params', 'ret', 'varargs', 'refs', 'flags')

    def __init__(self, kind, name, owner=None, static=False, type=None, params=(), ret=None, varargs=False,
                 refs=(), flags=()):
        self.kind = kind
        self.name = name
        self.owner = owner
        self.static = static
        self.type = type
        self.params = tuple(params)
        self.ret = ret
        self.varargs = varargs
        self.refs = tuple(refs)
        self.flags = tuple(flags)

    def __repr__(self):
        return '<%s %s.%s>' % (self.kind, self.owner, self.name)

    @property
    def owner_simple(self):
        return self.owner.rsplit('.', 1)[-1] if self.owner else None


# ---------------------------------------------------------------------------------------------- nodes
class Node(object):
    # origin: where in a binary the node came from (L2_BINARY_TYPES.md section 8):
    # {'module', 'segment', 'offset'} or {'module', 'va'}; None for source
    __slots__ = ('line', 'col', 'origin')
    fields = ()                                  # the child slots, in source order (walk() follows them)

    def __init__(self, **kw):
        self.line = kw.pop('line', 0)
        self.col = kw.pop('col', 0)
        self.origin = kw.pop('origin', None)
        for k in self.slots():
            setattr(self, k, kw.pop(k, None))
        if kw:
            raise TypeError('%s: unknown fields %s' % (type(self).__name__, sorted(kw)))

    @classmethod
    def slots(cls):
        """Every slot of the class and its bases below Node (an Expr's `type` included)."""
        out = []
        for k in reversed(cls.__mro__):
            if issubclass(k, Node) and k is not Node:
                out += [s for s in k.__dict__.get('__slots__', ()) if s not in out]
        return out

    def __repr__(self):
        return '%s(%s)' % (type(self).__name__, ', '.join('%s=%r' % (k, getattr(self, k)) for k in self.slots()))

    def children(self):
        for f in self.fields:
            v = getattr(self, f)
            if isinstance(v, Node):
                yield v
            elif isinstance(v, (list, tuple)):
                for x in v:
                    if isinstance(x, Node):
                        yield x


def walk(node):
    """Every node under (and including) `node`, depth first, in source order."""
    stack = [node]
    while stack:
        n = stack.pop()
        yield n
        stack.extend(reversed(list(n.children())))


def _node(name, slots, fields=None, base=Node):
    cls = type(name, (base,), {'__slots__': tuple(slots), 'fields': tuple(fields if fields is not None else slots)})
    return cls


# -- declarations
Module = _node('Module', ['units', 'header'], ['units'])              # a whole program, or one binary's module
# (header: its declarations as text, where they are not yet a tree -- l2.module, the one module model)
Unit = _node('Unit', ['path', 'package', 'imports', 'types', 'lang', 'target'], ['types'])
# (lang: the source language -- 'java', 'csharp', ... -- whose semantics the unit's library calls, string
# conversions (Concat) and enum and struct kinds carry; a back end maps them by it)
# (target: a C/C++ unit's target and the widths its compiler gave pointer, long and plain char -- a dict)
ClassDecl = _node('ClassDecl', ['name', 'qname', 'kind', 'mods', 'typeparams', 'extends', 'implements',
                                'members', 'outer', 'tparams', 'layout'], ['members'])
# (kind also: struct -- a value type, copied on assignment --, 'record struct', delegate -- a named function
# type whose one member is its Invoke --; tparams: the type parameters with their bounds, as TypeParam)
# (layout: a C/C++ struct or union whose bytes the source can see -- its 'record' Type: offsets, size)
# (a C struct, a frame, a data segment with a fixed layout is a 'record' Type, beside ClassDecl)     # kind: class interface enum record annotation
FieldDecl = _node('FieldDecl', ['sym', 'type', 'init', 'mods'], ['init'])
MethodDecl = _node('MethodDecl', ['sym', 'name', 'kind', 'params', 'ret', 'body', 'mods', 'throws', 'typeparams',
                                  'comments', 'text', 'tparams'], ['params', 'body'])
# kind: method ctor static_init init, or function (a free function: C, a decompiled binary). A function
# whose type carries a calling convention (an 'fn' Type) has it on sym.type, where callers read it too.
# comments: the text of each comment written above it (a decompiler's warnings). text: its source as
# text where its tree is not yet written (the C a function store holds).
Param = _node('Param', ['sym', 'type', 'mods'], [])
EnumConst = _node('EnumConst', ['sym', 'args', 'body', 'ctor', 'value'], ['args', 'body'])
# (value: an enum that is a named integer -- C, C# -- gives each constant's number; Java's are objects)
Property = _node('Property', ['sym', 'type', 'mods', 'params', 'accessors', 'init'], ['params', 'accessors', 'init'])
# a member read and written through accessors (C# properties and indexers): accessors are MethodDecls of
# kind get / set / init, each with body None where the compiler supplies it (an auto-property's field)
TypeParam = _node('TypeParam', ['name', 'bounds', 'flags', 'variance'], [])
# a type parameter: bounds (Types it must extend or implement), flags (class, struct, new)


# -- statements
class Stmt(Node):
    __slots__ = ()


def _stmt(name, slots, fields=None):
    return _node(name, slots, fields, Stmt)


Block = _stmt('Block', ['stmts'])
Local = _stmt('Local', ['sym', 'type', 'init', 'mods'], ['init'])
ExprStmt = _stmt('ExprStmt', ['expr'])
If = _stmt('If', ['cond', 'then', 'orelse'])
While = _stmt('While', ['cond', 'body'])
DoWhile = _stmt('DoWhile', ['body', 'cond'])
For = _stmt('For', ['init', 'cond', 'update', 'body'])
ForEach = _stmt('ForEach', ['var', 'iterable', 'body'])               # var: a Local without init
Switch = _stmt('Switch', ['expr', 'cases'])
Case = _stmt('Case', ['labels', 'default', 'body'], ['labels', 'body'])   # body: [Stmt]; falls through as in C
Break = _stmt('Break', ['label'], [])
Continue = _stmt('Continue', ['label'], [])
Return = _stmt('Return', ['expr'])
Throw = _stmt('Throw', ['expr'])
Try = _stmt('Try', ['resources', 'body', 'catches', 'final'])
Catch = _stmt('Catch', ['sym', 'types', 'body'], ['body'])          # several types: a multi-catch
Labeled = _stmt('Labeled', ['label', 'body'], ['body'])
Goto = _stmt('Goto', ['label'], [])                  # C, and whatever structuring leaves in a binary
# What stays assembly (L2_BINARY_TYPES.md section 7), as a statement, a member or a whole function:
# origin, length in bytes, reason (hand-written, register-arguments, ds-left-changed, ...), why, asm;
# name (the function it stands for), syntax (the assembler: wasm, masm, nasm), within (the fragment
# whose asm holds it too: a run of functions assembled as one file).
MachineFragment = _stmt('MachineFragment', ['length', 'reason', 'why', 'asm', 'name', 'syntax', 'within'], [])
Sync = _stmt('Sync', ['lock', 'body'])
Empty = _stmt('Empty', [], [])
Comment = _stmt('Comment', ['text'], [])             # a comment at its place among the statements (a warning)
LocalClass = _stmt('LocalClass', ['decl'])
Assert = _stmt('Assert', ['cond', 'detail'])
CtorCall = _stmt('CtorCall', ['kind', 'sym', 'args', 'target'], ['args', 'target'])   # this(...) / super(...)
Yield = _stmt('Yield', ['expr'])                     # a switch expression's arm gives its value


# -- expressions
class Expr(Node):
    __slots__ = ('type',)


def _expr(name, slots, fields=None):
    return _node(name, slots, fields, Expr)


Lit = _expr('Lit', ['value', 'text'], [])            # value: int, float, bool, str, None (null); text: as spelled
Name = _expr('Name', ['sym'], [])                    # a local or parameter
Select = _expr('Select', ['obj', 'sym'], ['obj'])    # a field: obj is an Expr, This, or TypeName for a static
TypeName = _expr('TypeName', [], [])                 # a type used as a qualifier (Block.stone); .type says which
This = _expr('This', ['qualifier'], [])
Super = _expr('Super', ['qualifier'], [])
Call = _expr('Call', ['target', 'sym', 'args', 'typeargs'], ['target', 'args'])   # target None: own method, implicit
New = _expr('New', ['sym', 'args', 'body', 'outer', 'memory'], ['outer', 'args', 'body'])   # memory: MEMORY   # body: an anonymous ClassDecl
NewArray = _expr('NewArray', ['elem', 'dims', 'init'], ['dims', 'init'])        # elem: the element Type
Index = _expr('Index', ['arr', 'index', 'sym'], ['arr', 'index'])   # sym: an indexer (C#), when not an array
Length = _expr('Length', ['arr'])                    # an array's length
Unary = _expr('Unary', ['op', 'x', 'postfix', 'overflow'], ['x'])     # op: - + ! ~ ++ --
Binary = _expr('Binary', ['op', 'left', 'right', 'overflow', 'signed'], ['left', 'right'])
# (signed: for a comparison in a binary, from the instruction (jl vs jb), not the operand types)        # op: + - * / % << >> >>> & | ^ && || == != < <= > >=
# (and ??: the left operand unless it is null, the right evaluated only then)
Concat = _expr('Concat', ['parts'])                      # string concatenation, flattened
Assign = _expr('Assign', ['op', 'target', 'value', 'overflow'], ['target', 'value'])   # op None, or the compound operator
Cond = _expr('Cond', ['cond', 'then', 'orelse'])
Cast = _expr('Cast', ['x', 'overflow', 'implicit'], ['x'])              # .type is the target type
# (implicit: True for a conversion the source language makes by itself -- C's promotions, array decay)
InstanceOf = _expr('InstanceOf', ['x', 'test', 'binding'], ['x'])   # test: a Type; binding: a pattern's Local
ClassLit = _expr('ClassLit', ['of'], [])             # X.class; of: a Type
Lambda = _expr('Lambda', ['params', 'body'])         # body: an Expr or a Block
MethodRef = _expr('MethodRef', ['target', 'sym', 'mode'], ['target'])   # mode: invoke / new
SwitchExpr = _expr('SwitchExpr', ['expr', 'cases'])  # cases: [Case]; each arm ends in a Yield or a Throw
AddressOf = _expr('AddressOf', ['sym'], [])            # a constant code or data address: by symbol, never a number
FarAddress = _expr('FarAddress', ['selector', 'offset'])   # selector:offset, typed ptr far (MK_FP)
Piece = _expr('Piece', ['x', 'byte', 'width'], ['x'])   # `width` bytes of x from byte `byte`
Join = _expr('Join', ['hi', 'lo'])                    # hi:lo as one wider value
Paren = _expr('Paren', ['x'])                        # kept: back ends print grouping the source chose
Unsupported = _expr('Unsupported', ['kind', 'raw'], [])   # a construct the front end cannot lower yet

OPERATORS = {
    'PLUS': '+', 'MINUS': '-', 'MULTIPLY': '*', 'DIVIDE': '/', 'REMAINDER': '%',
    'LEFT_SHIFT': '<<', 'RIGHT_SHIFT': '>>', 'UNSIGNED_RIGHT_SHIFT': '>>>',
    'AND': '&', 'OR': '|', 'XOR': '^', 'CONDITIONAL_AND': '&&', 'CONDITIONAL_OR': '||',
    'EQUAL_TO': '==', 'NOT_EQUAL_TO': '!=', 'LESS_THAN': '<', 'LESS_THAN_EQUAL': '<=',
    'GREATER_THAN': '>', 'GREATER_THAN_EQUAL': '>=',
}
UNARY = {'UNARY_MINUS': ('-', False), 'UNARY_PLUS': ('+', False), 'LOGICAL_COMPLEMENT': ('!', False),
         'BITWISE_COMPLEMENT': ('~', False), 'PREFIX_INCREMENT': ('++', False), 'PREFIX_DECREMENT': ('--', False),
         'POSTFIX_INCREMENT': ('++', True), 'POSTFIX_DECREMENT': ('--', True)}


def unsupported(node):
    """The Unsupported nodes under `node` -- what keeps it from going through the new path."""
    return [n for n in walk(node) if isinstance(n, Unsupported)]
