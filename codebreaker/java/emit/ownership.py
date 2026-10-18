# -*- coding: utf-8 -*-
"""
ownership.py -- which instance fields a class owns on the heap.

Shared by header.py (to declare the Rule-of-Three destructor + disabled copy)
and source.py (to define the destructor). A field is "owned heap" iff it is a
POINTER field (per the resolver) that a constructor assigns via `new`. Value-type
and static fields are never owned here -- deleting those would be wrong.

This is the heap-corruption defense: an owning class with a destructor but a
default copy constructor double-frees when copied (the classic 0xC0000018). We
detect owners here so emit can free them once AND forbid copying.
"""

import re

from codebreaker.java import typemap

# `new T[]{a, b, c}` -- a static array with an aggregate (brace) initializer
_AGG_ARRAY = re.compile(r'^new\s+[\w.]+\s*\[\s*\]\s*\{(.*)\}\s*$', re.S)


def nullable_value_params(method, resolver):
    """Names of value-struct PARAMS used as nullable references in the body (compared/assigned
    null). A C++ value can't hold null, so such a param is really a reference and must be a
    pointer -- and callers in this corpus already pass the (promoted) pointer for these
    raytrace-style dual-use types (Vec3D / AxisAlignedBB / MovingObjectPosition). Excludes
    primitives, String and containers (those have their own handling)."""
    text = '\n'.join(t for _ln, t in method.body)
    out = set()
    for p in method.params:
        jt = p.jtype
        if (jt.is_array or resolver.is_pointer(jt) or jt.name in typemap.PRIMITIVES
                or jt.name in typemap.VALUE_STDLIB or jt.name in typemap.CONTAINERS):
            continue
        nm = re.escape(p.name)
        if (re.search(r'\b%s\s*(?:==|!=)\s*(?:null|NULL)\b' % nm, text)
                or re.search(r'\b(?:null|NULL)\s*(?:==|!=)\s*%s\b' % nm, text)
                or re.search(r'\b%s\s*=\s*(?:null|NULL)\b' % nm, text)):
            out.add(p.name)
    return out


def _reassigned_params(method):
    """Param names that the body REASSIGNS (`p = ...`, `p += ...`, `p++`). Java lets a method reassign
    its parameter (the param is a local copy of the reference/value); a C++ `const X&` param cannot be
    reassigned -> "no operator = for const JavaString". Such params must be passed BY VALUE (mutable)."""
    body = '\n'.join(t for _ln, t in getattr(method, 'body', []) or [])
    out = set()
    for p in method.params:
        if re.search(r'(?<![\w.])' + re.escape(p.name) + r'\b\s*(?:[-+*/%|&^]?=(?!=)|\+\+|--)', body):
            out.add(p.name)
    return out


def params_str(method, resolver):
    """The C++ parameter list for a method, promoting nullable value-struct params to pointers.
    Single source of truth so the header declaration and the source definition always agree."""
    nullable = nullable_value_params(method, resolver)
    reassigned = _reassigned_params(method)
    parts = []
    for p in method.params:
        t = resolver.param_type(p.jtype)
        if p.name in nullable and not t.endswith('*'):
            t += '*'
        elif p.name in reassigned and t.startswith('const ') and t.endswith('&'):
            t = t[len('const '):-1].strip()          # const X& -> X: a reassigned param is by-value & mutable
        # Java collection out-param (pipeline.compute_collection_out_params): pass the value-struct
        # container by reference so the callee's add()/remove()/put() reach the caller's collection.
        if getattr(p, 'by_ref', False) and not t.endswith('*') and not t.endswith('&'):
            t += '&'
        parts.append('%s %s' % (t, p.name))
    return ', '.join(parts)


def aggregate_array_body(field):
    """For a static field initialized as `new T[]{...}`, the raw brace contents; else None.
    C++03 cannot `new T[]{...}`, so emit (header + source) declare a fixed C array instead."""
    init = (field.initializer or '').strip()
    m = _AGG_ARRAY.match(init)
    return m.group(1) if m else None
# -- no-GC ownership --------------------------------------------------------------------------------
# A field is OWNED (this object must `delete` it) iff SOME method assigns it a fresh heap allocation
# (`field = new T(...)` or an owning factory `X::createY(...)`) and NO method ever aliases it to a value
# of unknown provenance (a parameter, another field, a plain getter) -- deleting a BORROWED pointer
# double-frees. `= NULL` is neutral. This is broadened from the old ctor-only/`new`-only heuristic so
# factory-allocated (BufferUtils::createIntBuffer) and init()-allocated members are freed too; the
# borrow-exclusion is what keeps a passed-in pointer (a renderer's `level`) from being double-freed.
# NOTE: method bodies here are RAW JAVA (`this.field = new T()`, `Buf.createBuf(...)`), so the patterns
# accept Java `.` as well as C++ `::`/`->` -- ownership is derived before statement translation.
_ASSIGN_STMT = re.compile(r'(?:this\s*(?:->|\.)\s*)?([A-Za-z_]\w*)\s*=\s*([^;]+);')
_RHS_NEW     = re.compile(r'^new\s+[A-Za-z_][\w:.]*\s*(\[)?')
_RHS_FACTORY = re.compile(r'^(?:[A-Za-z_]\w*\s*(?:::|\.)\s*)+(?:create|make|alloc|clone|copyOf)[A-Za-z]\w*\s*\(')
_RHS_NEUTRAL = re.compile(r'^(?:null|NULL|nullptr|0)$')
# container element adds: `c.push_back(EXPR)` / `c.add(EXPR)` / `c[i] = EXPR`
_ADD_CALL = re.compile(r'([A-Za-z_]\w*)\s*(?:->|\.)\s*(?:push_back|add|addElement|append)\s*\(\s*(.*?)\)\s*;')
_ELEM_SET = re.compile(r'([A-Za-z_]\w*)\s*\[[^\]]*\]\s*=\s*([^;]+);')


def _is_alloc_rhs(rhs):
    return bool(_RHS_NEW.match(rhs) or _RHS_FACTORY.match(rhs))


def _body_text(method):
    """A method's body as one string, line-comments stripped (the form every ownership scan wants)."""
    return re.sub(r'//[^\n]*', '', '\n'.join(t for _ln, t in (method.body or [])))


def _method_texts(cls):
    """Body text of every method (ctors included), line-comments stripped."""
    return (_body_text(m) for m in cls.methods)


def owned_pointers(cls, resolver):
    """{field_name: is_array} for POINTER instance fields this object owns and the destructor may
    safely `delete`: allocated somewhere and never aliased to a borrowed value (see module note)."""
    alloc, borrowed = {}, set()

    def classify(name, rhs):
        mnew = _RHS_NEW.match(rhs)
        if mnew:
            alloc[name] = bool(mnew.group(1)) or alloc.get(name, False)
        elif _RHS_FACTORY.match(rhs):
            alloc.setdefault(name, False)
        elif _RHS_NEUTRAL.match(rhs):
            pass                                             # = NULL: neutral, delete-of-NULL is safe
        else:
            borrowed.add(name)                               # param / alias / unknown getter -> borrowed

    # field-declaration initializers (`Timer timer = new Timer(60F);`, `= BufferUtils.createIntBuffer(16);`):
    # the emitter moves these into the constructor, but they live on Field.initializer, NOT in any scanned
    # method body -- so without this the destructor silently misses them (the RetroMCP-style leak).
    for f in cls.fields:
        if not f.is_static and f.initializer and resolver.is_pointer(f.jtype):
            classify(f.name, f.initializer.strip())
    # method-body assignments
    for text in _method_texts(cls):
        for am in _ASSIGN_STMT.finditer(text):
            fld = cls.field(am.group(1))
            if fld and not fld.is_static and resolver.is_pointer(fld.jtype):
                classify(am.group(1), am.group(2).strip())
    return {f: a for f, a in alloc.items() if f not in borrowed}


def owned_containers(cls, resolver):
    """{field_name: size_accessor} for CONTAINER fields (a Java array or List of a reference type) whose
    EVERY element is a fresh allocation and never a borrowed element -- the destructor loop-deletes them.
    accessor is '.length' (JArray) or '.size()' (JList). A single borrowed add disqualifies the field
    (e.g. a listener list that stores passed-in observers)."""
    add_alloc, add_borrow = set(), set()
    for text in _method_texts(cls):
        for pat in (_ADD_CALL, _ELEM_SET):               # `c.add(x)` / `c[i] = x` -- own iff x is a fresh alloc
            for m in pat.finditer(text):
                (add_alloc if _is_alloc_rhs(m.group(2).strip()) else add_borrow).add(m.group(1))
    out = {}
    for name in add_alloc:
        if name in add_borrow:
            continue
        fld = cls.field(name)
        if not fld or fld.is_static:
            continue
        jt = fld.jtype
        if jt.is_array and resolver.is_pointer(jt.element()):
            out[name] = '.length'
        elif jt.name in typemap.CONTAINERS and jt.args and resolver.is_pointer(jt.args[0]):
            out[name] = '.size()'
    return out


def owns_heap(cls, resolver):
    """True if the object owns ANY heap (a scalar pointer field or a container of owned pointers). Such a
    class must forbid copying, else a by-value copy double-frees on destruction (Rule of Three)."""
    return bool(owned_pointers(cls, resolver)) or bool(owned_containers(cls, resolver))


# -- returned ownership (caller frees) --------------------------------------------------------------
# Some methods BUILD heap objects and return them for the caller to own+free (Level.getCubes returns a
# fresh List of `new AABB`). Java GC'd those; on the 360 the caller must delete them. We detect such
# methods CONSERVATIVELY (fresh local container, every element a fresh allocation, then returned) so the
# emitter can drop an RAII element-deleter at the call site. A method that returns a field/param (a
# borrowed view) never qualifies.
def _returns_container(rt):
    # Pre-generics Minecraft returns RAW containers (`ArrayList`, no <T>), so the element type is NOT on
    # the return signature -- it comes from the `new T` adds and the caller's `JList<T*>` decl. So the gate
    # is just "returns a container type"; ownership is proven by every add being a fresh allocation, and the
    # pointer-ness by the caller-side guard only firing on a `JList<T*>` local.
    if rt is None or rt.name == 'void':
        return False
    return rt.name in typemap.CONTAINERS or rt.is_array


def returns_owned_container(method, resolver):
    """True iff the method returns a FRESHLY-built container whose every element is a fresh allocation --
    so the caller owns those elements. Conservative: any borrowed add, or returning a non-locally-built
    container, disqualifies it (we then simply don't emit cleanup and the leak stays flagged)."""
    if not _returns_container(method.return_type):
        return False
    text = _body_text(method)
    returned = set(re.findall(r'\breturn\s+([A-Za-z_]\w*)\s*;', text))
    if not returned:
        return False
    for var in returned:
        v = re.escape(var)
        # freshly constructed local container: `var = new ArrayList()` or generic `new ArrayList<T>()`
        if not re.search(r'\b%s\s*=\s*new\s+[A-Za-z_][\w.]*\s*(?:<[^>]*>)?\s*\(' % v, text):
            return False
        adds = re.findall(r'\b%s\s*\.\s*(?:add|addElement|push_back|append)\s*\(\s*(.*?)\)\s*;' % v, text)
        if not adds or any(not _is_alloc_rhs(a.strip()) for a in adds):
            return False                                 # no adds, or a borrowed element added
    return True


def owned_return_container_methods(classes, resolver):
    """Program-wide set of method NAMES that return a freshly-built owned container. UNANIMOUS: a name is
    included only if some definition qualifies AND no same-named definition returns a container that is
    NOT freshly-owned -- so matching a call site by method name alone stays safe."""
    owned, ambiguous = set(), set()
    for c in classes:
        for m in c.methods:
            if m.is_ctor or not _returns_container(m.return_type):
                continue
            (owned if returns_owned_container(m, resolver) else ambiguous).add(m.name)
    return frozenset(owned - ambiguous)


# -- owned SCALAR return + argument-retention (Pattern B: `getCubes(bb.expand(...))`) ----------------
# A method that returns a fresh heap SCALAR (`AABB.expand` -> `return new AABB(...)`) hands the caller an
# object it must free. When such a call is used INLINE as an argument and its result is never captured, it
# leaks. We can hoist it to a temp and RAII-delete it -- but ONLY if the receiving method does not RETAIN
# the argument (store it in a field, return it, add it to a container); otherwise the delete would dangle.
def _returns_scalar_pointer(method, resolver):
    """Gate for scalar-ownership analysis: the return is a single reference/pointer object -- not void,
    an array, or a container."""
    rt = method.return_type
    if rt is None or rt.name == 'void' or rt.is_array or rt.name in typemap.CONTAINERS:
        return False
    return resolver.is_pointer(rt) or returns_pointer(method, resolver)


def returns_owned_scalar(method, resolver):
    """True iff EVERY non-null return of the method is a fresh `new X(...)` (the caller owns the result)."""
    if not _returns_scalar_pointer(method, resolver):
        return False
    rets = re.findall(r'\breturn\b([^;]*);', _body_text(method))
    if not rets:
        return False
    saw_new = False
    for e in (r.strip() for r in rets):
        if e == '' or _RHS_NEUTRAL.match(e):
            continue                                     # bare / null / 0 return -> temp may be null; delete NULL is safe
        if _RHS_NEW.match(e):
            saw_new = True
        else:
            return False                                 # returns a field/param/other -> not caller-owned
    return saw_new


def owned_scalar_return_methods(classes, resolver):
    """{method_name: cpp_element_type} for methods that return a fresh heap scalar. UNANIMOUS across defs
    of the same name (conflicting element type or any non-owning def drops the name)."""
    owned, ambiguous = {}, set()
    for c in classes:
        for m in c.methods:
            if m.is_ctor or not _returns_scalar_pointer(m, resolver):
                continue
            if returns_owned_scalar(m, resolver):
                elem = return_cpp_type(m, resolver).rstrip('*').strip()
                if owned.get(m.name, elem) != elem:
                    ambiguous.add(m.name)
                owned[m.name] = elem
            else:
                ambiguous.add(m.name)
    return {k: v for k, v in owned.items() if k not in ambiguous}


def _param_retained(name, text):
    """Conservative: a param is RETAINED unless every occurrence in the body is a pure read (`name.` or
    `name[`). Anything else -- stored, returned, passed on, compared -- counts as retained, so we only
    ever free an argument the callee provably just reads."""
    v = re.escape(name)
    for mo in re.finditer(r'\b%s\b' % v, text):
        if not re.match(r'\s*[.\[]', text[mo.end():]):
            return True
    return False


def retained_param_positions(classes):
    """{method_name: set(param_index)} for parameters that SOME definition retains -- so a temp passed at
    such a position must NOT be auto-freed. A name absent here retains nothing in any definition."""
    retained = {}
    for c in classes:
        for m in c.methods:
            if m.is_ctor:
                continue
            text = _body_text(m)
            brace = text.find('{')                       # drop the signature line -- the param name in
            text = text[brace + 1:] if brace >= 0 else text   # `f(T p)` is a decl, not a retention use
            for i, p in enumerate(m.params):
                if _param_retained(p.name, text):
                    retained.setdefault(m.name, set()).add(i)
    return retained


def returns_pointer(method, resolver):
    """Whether the method's return should be a C++ pointer.

    Reference types are always pointers. A VALUE type (e.g. Vec3D) is promoted to
    a pointer when the body `return new <ReturnType>(...)` -- the dual-use case
    where a struct is stored by value as a field but heap-allocated on return.
    """
    rt = method.return_type
    if rt is None or rt.name == 'void':
        return False
    # arrays are by-value JArray<T> (their own null handling: `return null` -> empty JArray).
    # Never promote to pointer, or `return theField;` (a value JArray) can't convert.
    if rt.is_array:
        return False
    # collections are by-value (JList/JMap/JSet), with their own null handling (`= null` / `return
    # null` -> empty via the long-long ctor) exactly like arrays. NEVER promote a container return to
    # a pointer: a sibling method / local / field of the SAME interface type is a value, and a promoted
    # pointer return can't interconvert with it (the StatsSyncher/StatFileWriter JMap*<->JMap collisions).
    if rt.name in typemap.CONTAINERS:
        return False
    if resolver.is_pointer(rt):
        return True
    # std::string (and other value-stdlib types) is NEVER a pointer: it has dedicated
    # by-value handling (return-by-value, `return null` -> empty string). Promoting it
    # both reintroduces the dangling-pointer risk and breaks virtual-override return
    # unification (base `return null` -> string*, derived `return "x"` -> string).
    if rt.name in typemap.VALUE_STDLIB:
        return False
    text = '\n'.join(t for _ln, t in method.body)
    # value-type return promoted to pointer if the body new's it in a return (incl. a
    # ternary) OR returns null -- both impossible for a by-value return
    if re.search(r'\breturn\b[^;]*\bnew\s+%s\b' % re.escape(rt.name), text):
        return True
    if re.search(r'\breturn\s+(?:null|NULL)\b', text):
        return True
    return False


def return_cpp_type(method, resolver):
    """The C++ return type for a method, applying dual-use promotion. Shared by
    header.py and source.py so the declaration and definition always agree."""
    if method.return_type is None or method.return_type.name == 'void':
        return 'void'
    rt = resolver.return_type(method.return_type)
    if returns_pointer(method, resolver) and not rt.endswith('*') and not rt.endswith('&'):
        rt += '*'
    return rt
