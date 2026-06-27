# -*- coding: utf-8 -*-
"""
rules.py -- structural crash-class detectors for converted Xbox 360 C++.

These encode the bug classes that actually crashed the console during the Alpha
port (see the project memory / RESTRUCTURE_PLAN.md). The console hard-crashes
(IP 0x0 / 0xC0000018) where Java would tolerate the same code, so the converter
must flag these, not just translate.

Each rule is a small, named, self-describing object so it's individually
testable and the report can explain *why* it fired. A rule looks at a converted
file (and, when needed, its sibling header) and yields Events.

Covered here (the cases plain text-marker / lint scanning misses):
  uninit-array        new T[n] without () -- Java zero-fills, C++ does not   (auto-jump bug)
  ctor-stub           empty / delegation-only constructor body               (blockID=0, container-flag bugs)
  rule-of-three       owns heap (new in ctor) but missing dtor/copy control  (heap-corruption / double-free)
  value-copy-owner    a heavy/owning type passed or returned BY VALUE        (Tesselator MB-buffer copy)

Null-deref and label-goto stay in converter_lint (already covered there) to
avoid double-reporting; this module is the home for the *new* structural rules.
"""

import os
import re

from .ledger import Category, Severity


# Primitive element types whose `new T[n]` is left uninitialized in C++.
_PRIM = (r'bool|char|signed char|unsigned char|wchar_t|short|unsigned short|'
         r'int|unsigned int|unsigned|long long|long|float|double|'
         r'byte|jbyte|jboolean|jint|jshort|jlong|jfloat|jdouble')

# `new <prim>[ ... ]`  (capture whether a trailing value-init () is present)
_NEW_PRIM_ARRAY = re.compile(r'\bnew\s+(%s)\s*\[[^\]]*\]\s*(\(\s*\))?' % _PRIM)
# `new <Type> * [ ... ]`  -> array of pointers, uninitialized = garbage vtables
_NEW_PTR_ARRAY = re.compile(r'\bnew\s+\w[\w:]*\s*\*\s*\[[^\]]*\]\s*(\(\s*\))?')

# A type that is expensive or owns a resource and must never be copied by value.
# Seeded with the documented offenders; the type model will grow this set later.
_HEAVY_TYPES = ('Tesselator', 'Tessellator', 'NBTTagCompound', 'NBTTagList')


def _read(path):
    with open(path, 'r', encoding='utf-8', errors='replace') as f:
        return f.read()


def _strip_block_comments(text):
    return re.sub(r'/\*.*?\*/', ' ', text, flags=re.S)


def _iter_code_lines(text):
    """Yield (lineno, raw, code-without-line-comment) skipping pure comment lines."""
    for i, raw in enumerate(text.split('\n'), 1):
        code = raw.split('//', 1)[0]
        if not code.strip():
            yield i, raw, ''  # keep index alignment; caller skips empties
        else:
            yield i, raw, code


# ---------------------------------------------------------------------------
# Rule: uninitialized primitive / pointer arrays
# ---------------------------------------------------------------------------
def rule_uninit_array(fname, text, header_text):
    if not fname.endswith('.cpp') and not fname.endswith('.h'):
        return
    for i, raw, code in _iter_code_lines(text):
        if 'new ' not in code or '[' not in code:
            continue
        m = _NEW_PRIM_ARRAY.search(code)
        if m and not m.group(2):          # no trailing ()
            t = m.group(1)
            sev = Severity.HIGH if t in ('bool',) else Severity.MED
            yield (i, Category.RISK, sev, 'uninit-array',
                   "new %s[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. "
                   "Use new %s[N]() (the new bool[10] phantom-input bug)." % (t, t),
                   raw.strip()[:160])
            continue
        mp = _NEW_PTR_ARRAY.search(code)
        if mp and not mp.group(1):
            yield (i, Category.RISK, Severity.HIGH, 'uninit-array',
                   "new <Type>*[] without () -- uninitialized pointer array = garbage vtables -> "
                   "deref crash (IP 0x0). Use new T*[N]() and NULL-check before deref.",
                   raw.strip()[:160])


# ---------------------------------------------------------------------------
# Rule: empty / delegation-only constructor bodies
# ---------------------------------------------------------------------------
_CTOR_DEF = None  # built per-file (needs class name)

def rule_ctor_stub(fname, text, header_text):
    if not fname.endswith('.cpp'):
        return
    cls = fname[:-4]
    # ClassName::ClassName( ... ) [: init list] { body }
    pat = re.compile(r'\b%s::%s\s*\(([^)]*)\)\s*(:[^{]*)?\{' % (re.escape(cls), re.escape(cls)))
    for m in pat.finditer(text):
        params = m.group(1).strip()
        init_list = (m.group(2) or '').strip()       # `: Base(args), member(0), ...`
        # extract balanced body
        depth, j = 1, m.end()
        while j < len(text) and depth:
            if text[j] == '{':
                depth += 1
            elif text[j] == '}':
                depth -= 1
            j += 1
        body = text[m.end():j-1]
        line = text.count('\n', 0, m.start()) + 1
        # strip comments + whitespace to see if there's any real statement
        stripped = _strip_block_comments(body)
        stripped = '\n'.join(ln.split('//', 1)[0] for ln in stripped.split('\n')).strip()
        has_delegation_marker = 'constructor delegation' in body or 'was: this(' in body
        # an empty body WITH an init list (base/member init) is valid C++, not a stub
        if params and not init_list and (not stripped or has_delegation_marker):
            yield (line, Category.STUBBED, Severity.HIGH, 'ctor-stub',
                   "constructor %s(%s) has an empty/delegation-only body -- field init and static "
                   "REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer "
                   "bugs). Replicate the real ctor body." % (cls, params[:60]),
                   ('%s(%s) {...}' % (cls, params))[:160])


# ---------------------------------------------------------------------------
# Rule: Rule of Three -- owns heap but missing destructor / copy control
# ---------------------------------------------------------------------------
def rule_rule_of_three(fname, text, header_text):
    if not fname.endswith('.cpp') or header_text is None:
        return
    cls = fname[:-4]
    ctor_pat = re.compile(r'\b%s::%s\s*\([^)]*\)\s*(?::[^{]*)?\{' % (re.escape(cls), re.escape(cls)))
    owns_heap = False
    ctor_line = 0
    for m in ctor_pat.finditer(text):
        depth, j = 1, m.end()
        while j < len(text) and depth:
            if text[j] == '{':
                depth += 1
            elif text[j] == '}':
                depth -= 1
            j += 1
        body = text[m.end():j-1]
        if re.search(r'=\s*new\s+\w', body) or re.search(r'\bnew\s+\w[\w:]*\s*\[', body):
            owns_heap = True
            ctor_line = text.count('\n', 0, m.start()) + 1
            break
    if not owns_heap:
        return
    h = _strip_block_comments(header_text)
    has_dtor = re.search(r'~%s\s*\(' % re.escape(cls), h) is not None
    has_copy_ctor = re.search(r'\b%s\s*\(\s*const\s+%s\s*&' % (re.escape(cls), re.escape(cls)), h) is not None
    has_copy_assign = re.search(r'operator\s*=\s*\(\s*const\s+%s\s*&' % re.escape(cls), h) is not None
    copy_disabled = ('private:' in h and has_copy_ctor) or 'DISALLOW_COPY' in h
    if not has_dtor:
        yield (ctor_line, Category.RISK, Severity.HIGH, 'rule-of-three',
               "%s allocates with `new` in its constructor but declares NO destructor -> leak "
               "(Java GC'd this; C++ does not)." % cls, '')
    elif not (has_copy_ctor and has_copy_assign) and not copy_disabled:
        yield (ctor_line, Category.RISK, Severity.MED, 'rule-of-three',
               "%s owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: "
               "a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). "
               "Add them or make copying private." % cls, '')


# ---------------------------------------------------------------------------
# Rule: heavy/owning type passed or returned BY VALUE
# ---------------------------------------------------------------------------
def rule_value_copy_owner(fname, text, header_text):
    if not (fname.endswith('.cpp') or fname.endswith('.h')):
        return
    for t in _HEAVY_TYPES:
        # by-value param:  (Type name  |  , Type name   -- but NOT Type& / Type*
        param_pat = re.compile(r'[(,]\s*%s\s+(?!&|\*)\w+\s*[,)]' % re.escape(t))
        # by-value return: ^Type Class::method(   or   Type method(
        ret_pat = re.compile(r'(?:^|\n)\s*%s\s+(?!&|\*)\w[\w:]*\s*\(' % re.escape(t))
        for m in param_pat.finditer(text):
            line = text.count('\n', 0, m.start()) + 1
            sev = Severity.HIGH if t in ('Tesselator', 'Tessellator') else Severity.MED
            yield (line, Category.GUESSED, sev, 'value-copy-owner',
                   "%s passed BY VALUE -- copies its whole buffer every call (Tesselator holds an MB-scale "
                   "vertex buffer). Take it by reference: const %s& / %s&." % (t, t, t),
                   text.split('\n')[line-1].strip()[:160])
        for m in ret_pat.finditer(text):
            line = text.count('\n', 0, m.start()) + 1
            sev = Severity.HIGH if t in ('Tesselator', 'Tessellator') else Severity.MED
            yield (line, Category.GUESSED, sev, 'value-copy-owner',
                   "%s returned BY VALUE -- returns a full copy. getInstance() must return a reference." % t,
                   text.split('\n')[line-1].strip()[:160])


# ---------------------------------------------------------------------------
# Rule: virtual method called from a constructor (dispatches to base, not override)
# ---------------------------------------------------------------------------
def _ctor_bodies(text, cls):
    """Yield (line, body) for each ClassName::ClassName(...) definition."""
    pat = re.compile(r'\b%s::%s\s*\([^)]*\)\s*(?::[^{]*)?\{' % (re.escape(cls), re.escape(cls)))
    for m in pat.finditer(text):
        depth, j = 1, m.end()
        while j < len(text) and depth:
            if text[j] == '{':
                depth += 1
            elif text[j] == '}':
                depth -= 1
            j += 1
        yield text.count('\n', 0, m.start()) + 1, text[m.end():j-1]


def rule_virtual_call_in_ctor(fname, text, header_text):
    """A constructor that calls one of its own virtual methods: in C++ the call dispatches to THIS
    class's version (the vtable isn't the derived one yet during construction), NOT a subclass
    override -- unlike Java. So overridden behaviour is silently skipped and wrong data gets recorded
    (the old port's Block opacity bug: Block ctor -> setLightOpacity dispatched to Block, not the
    subclass). Flag it; set the value directly or finish it after construction."""
    if not fname.endswith('.cpp') or header_text is None:
        return
    cls = fname[:-4]
    h = _strip_block_comments(header_text)
    virt = set(re.findall(r'\bvirtual\b[^;{]*?\b(\w+)\s*\(', h))
    virt.discard(cls)                                    # not the ctor itself
    virt = set(n for n in virt if not n.startswith('~'))  # not the dtor
    if not virt:
        return
    for line0, body in _ctor_bodies(text, cls):
        bstr = _strip_block_comments(body)
        for name in virt:
            # `this->name(` or a bare `name(` (NOT obj->name / obj.name / Class::name -- those are fine)
            m = (re.search(r'\bthis\s*->\s*%s\s*\(' % re.escape(name), bstr)
                 or re.search(r'(?<![\w.>:])%s\s*\(' % re.escape(name), bstr))
            if m:
                yield (line0, Category.RISK, Severity.MED, 'virtual-call-in-ctor',
                       "%s's constructor calls virtual %s() -- C++ dispatches to %s's own version, NOT "
                       "a subclass override (Java would call the override). Wrong data may be recorded "
                       "(the Block light-opacity bug). Set the field directly or finish init post-ctor."
                       % (cls, name, cls), '')


# ---------------------------------------------------------------------------
# Rule: eager static init with a cross-translation-unit dependency (init-order crash class)
# ---------------------------------------------------------------------------
# A static field DEFINITION with an initializer: `<type> <Class>::s_<name> = <init>;`
_STATIC_DEF = re.compile(r'^[A-Za-z_][\w:<>\*&,\s]*?\b(\w+::s_\w+)\s*=\s*(.+);\s*$', re.M)
# constructing a user class (not a safe runtime value type) -> its ctor may read other statics
_NEW_USER = re.compile(r'\bnew\s+(?!JavaString\b|JArray\b|JList\b|JObject\b|JCat\b)[A-Z]\w*')


def rule_static_init_order(fname, text, header_text):
    """An EAGER static field definition that constructs a user class or reads another class's static
    (`::s_`) runs at static-init time in UNDEFINED cross-translation-unit order -- the static-init-
    order crash class (Block initialized before Material -> null vtable). The converter defers these
    into initStatics(), run by j2x_initAll() in dependency order; this rule flags any that slipped
    (e.g. in a hand-written override) so they get deferred too."""
    if not fname.endswith('.cpp'):
        return
    for m in _STATIC_DEF.finditer(text):
        init = m.group(2)
        if '::s_' in init or _NEW_USER.search(init):
            line = text.count('\n', 0, m.start()) + 1
            yield (line, Category.RISK, Severity.MED, 'static-init-order',
                   "eager static init `%s = ...` runs in UNDEFINED cross-translation-unit order "
                   "(it constructs a class or reads another class's statics) -- the static-init-order "
                   "crash class. Defer it into initStatics() (run in dependency order by j2x_initAll)."
                   % m.group(1), m.group(0).strip()[:160])


# ---------------------------------------------------------------------------
# Rule: raw multi-byte cast/copy in a binary-serialization file (big-endian hazard)
# ---------------------------------------------------------------------------
# Files that read/write the on-disk binary formats (NBT, save/region, streams, compressed IO).
_BINARY_IO_FILE = re.compile(r'(NBT|Stream|Save|Region|Compressed|Chunk.*(Loader|Provider))', re.I)
# A raw reinterpret over a >1-byte scalar, or memcpy -- byte-swaps on big-endian PowerPC.
_RAW_CAST = re.compile(r'reinterpret_cast\s*<|memcpy\s*\(|\*\s*\(\s*(?:int|short|long|long long|float|double|unsigned)\b[\w ]*\*\s*\)')


def rule_endianness_raw_cast(fname, text, header_text):
    """Xbox 360 is BIG-ENDIAN PowerPC. The NBT / save / stream code reads & writes multi-byte values
    one byte at a time (MSB-first) on purpose -- that is endian-agnostic and matches Java. A raw
    `reinterpret_cast`, `*(int*)ptr`, or `memcpy` over a serialized byte buffer is an "optimization"
    that BYTE-SWAPS on the 360 and silently corrupts saves / desyncs the protocol. Flag such casts in
    binary-IO files; keep the manual byte assembly (assemble the int/long, THEN bit-cast to float)."""
    if not fname.endswith('.cpp') or not _BINARY_IO_FILE.search(fname):
        return
    for i, raw, code in _iter_code_lines(text):
        if not code:
            continue
        if _RAW_CAST.search(code):
            yield (i, Category.WARNING, Severity.MED, 'endianness',
                   "raw multi-byte cast/memcpy in a binary-serialization file -- on the big-endian "
                   "360 this byte-swaps and corrupts saves/protocol. Read/write byte-at-a-time "
                   "(MSB-first), then bit-cast the assembled int/long to float/double.",
                   raw.strip()[:160])


# Registry -- (id, description, function). Add new rules here.
RULES = [
    ('uninit-array',    'new T[n] without () leaves primitives/pointers uninitialized', rule_uninit_array),
    ('ctor-stub',       'empty or delegation-only constructor body (missing registration)', rule_ctor_stub),
    ('rule-of-three',   'owns heap but missing destructor / copy control', rule_rule_of_three),
    ('value-copy-owner','heavy/owning type passed or returned by value', rule_value_copy_owner),
    ('virtual-call-in-ctor', 'constructor calls own virtual method (dispatches to base, not override)',
     rule_virtual_call_in_ctor),
    ('static-init-order', 'eager static init with a cross-TU dependency (undefined init order)',
     rule_static_init_order),
]


def scan_rules(ledger, output_dir):
    """Run every structural rule over the converted tree, appending Events. Returns count."""
    n = 0
    files = sorted(f for f in os.listdir(output_dir) if f.endswith('.cpp') or f.endswith('.h'))
    for fname in files:
        text = _read(os.path.join(output_dir, fname))
        header_text = None
        if fname.endswith('.cpp'):
            hp = os.path.join(output_dir, fname[:-4] + '.h')
            if os.path.isfile(hp):
                header_text = _read(hp)
        for _rid, _desc, fn in RULES:
            for line, cat, sev, rule, msg, snip in fn(fname, text, header_text):
                ledger.add(fname, cat, msg, line=line, severity=sev,
                           source='rule', rule=rule, snippet=snip)
                n += 1
    return n
