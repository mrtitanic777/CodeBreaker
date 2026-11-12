# -*- coding: utf-8 -*-
"""
rules.py -- structural crash-class detectors for converted C++.

These encode the bug classes that crash when porting Java to C++ across any
target platform. Each rule is platform-aware: messages, severity, and
applicability adapt to the selected target (Xbox 360, PC Windows, PS3, etc.)
using metadata from platforms.py.

Each rule is a small, named, self-describing object so it's individually
testable and the report can explain *why* it fired. A rule looks at a converted
file (and, when needed, its sibling header) and yields Events.

Covered here (the cases plain text-marker / lint scanning misses):
  uninit-array        new T[n] without () -- Java zero-fills, C++ does not
  ctor-stub           empty / delegation-only constructor body
  rule-of-three       owns heap (new in ctor) but missing dtor/copy control
  value-copy-owner    a heavy/owning type passed or returned BY VALUE
  virtual-call-in-ctor  ctor calls own virtual -> dispatches to base, not override
  static-init-order   eager static init with cross-TU dependency
  endianness          raw multi-byte cast in binary-IO file (big-endian only)
  null-deref-getter   getter()->member where getter can return NULL
  null-deref-field    known-nullable field deref without null check
"""

import os
import re

from codebreaker.java.ledger import Category, Severity


# ---------------------------------------------------------------------------
# Platform context: built once per scan_rules() call, passed to every rule.
# ---------------------------------------------------------------------------
class _PlatCtx(object):
    __slots__ = ('label', 'big_endian', 'null_page_protected', 'null_deref_msg', 'override_pairs')
    def __init__(self, platform_id):
        from codebreaker.java.platforms import PLATFORMS
        plat = PLATFORMS.get(platform_id, {})
        self.label = plat.get('label', platform_id or 'unknown')
        self.big_endian = plat.get('big_endian', False)
        self.null_page_protected = plat.get('null_page_protected', True)
        self.null_deref_msg = plat.get('null_deref_msg',
            'NULL dereference -- undefined behaviour in C++ (Java would throw NPE)')
        # {(base_class, method)} that a DESCENDANT overrides -- built once by scan_rules() over the whole
        # tree so rule_virtual_call_in_ctor can flag ONLY genuinely-overridden calls. None => whole-tree
        # hierarchy is unknown (a per-file/unit-test call): fall back to the conservative "always flag".
        self.override_pairs = None


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
from codebreaker.java import profiles as _profiles
_HEAVY_TYPES = tuple(_profiles.active().heavy_types)
_HEAVY_HIGH = tuple(_profiles.active().heavy_types_high)


def _reseed(p):
    """Re-derive the heavy-types tuple when the active target profile changes (profiles.on_change)."""
    global _HEAVY_TYPES, _HEAVY_HIGH
    _HEAVY_TYPES = tuple(p.heavy_types)
    _HEAVY_HIGH = tuple(p.heavy_types_high)


_profiles.on_change(_reseed)


from codebreaker.java.textutil import read_text as _read_text


def _read(path):
    return _read_text(path, missing_ok=False)   # errors='replace', but raise on missing


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
def rule_uninit_array(fname, text, header_text, plat):
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
                   "Use new %s[N]()." % (t, t),
                   raw.strip()[:160])
            continue
        mp = _NEW_PTR_ARRAY.search(code)
        if mp and not mp.group(1):
            yield (i, Category.RISK, Severity.HIGH, 'uninit-array',
                   "new <Type>*[] without () -- uninitialized pointer array = garbage vtables -> "
                   "deref crash. Use new T*[N]() and NULL-check before deref.",
                   raw.strip()[:160])


# ---------------------------------------------------------------------------
# Rule: empty / delegation-only constructor bodies
# ---------------------------------------------------------------------------
def rule_ctor_stub(fname, text, header_text, plat):
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
def rule_rule_of_three(fname, text, header_text, plat):
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
def rule_value_copy_owner(fname, text, header_text, plat):
    if not (fname.endswith('.cpp') or fname.endswith('.h')):
        return
    # Blank comments to spaces (length- and newline-preserving) so a type name inside a comment like
    # "// glDrawArrays (Tessellator path)" is NOT mistaken for a by-value parameter. Offsets stay valid.
    src = re.sub(r'//[^\n]*', lambda m: ' ' * len(m.group(0)), text)
    src = re.sub(r'/\*.*?\*/', lambda m: re.sub(r'[^\n]', ' ', m.group(0)), src, flags=re.S)
    for t in _HEAVY_TYPES:
        # by-value param:  (Type name  |  , Type name   -- but NOT Type& / Type*
        # (the terminator is a lookahead so it stays the introducer of the NEXT parameter: `(Type a, Type b)` flags both)
        param_pat = re.compile(r'[(,]\s*%s\s+(?!&|\*)\w+(?=\s*[,)])' % re.escape(t))
        # by-value return: ^Type Class::method(   or   Type method(   (group 1 is the type, on the line it is reported at)
        ret_pat = re.compile(r'(?:^|\n)\s*(%s)\s+(?!&|\*)\w[\w:]*\s*\(' % re.escape(t))
        for m in param_pat.finditer(src):
            line = text.count('\n', 0, m.start()) + 1
            sev = Severity.HIGH if t in _HEAVY_HIGH else Severity.MED
            yield (line, Category.GUESSED, sev, 'value-copy-owner',
                   "%s passed BY VALUE -- copies its whole buffer every call. Take it by reference: "
                   "const %s& / %s&." % (t, t, t),
                   text.split('\n')[line-1].strip()[:160])
        for m in ret_pat.finditer(src):
            line = text.count('\n', 0, m.start(1)) + 1
            sev = Severity.HIGH if t in _HEAVY_HIGH else Severity.MED
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


def rule_virtual_call_in_ctor(fname, text, header_text, plat):
    """A constructor that calls one of its own virtual methods: in C++ the call dispatches to THIS
    class's version (the vtable isn't the derived one yet during construction), NOT a subclass
    override -- unlike Java. So overridden behaviour is silently skipped and wrong data gets recorded.
    Flag it; set the value directly or finish it after construction."""
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
        for name in sorted(virt):                        # (sorted: the findings of one constructor come in one order)
            # `this->name(` or a bare `name(` (NOT obj->name / obj.name / Class::name -- those are fine)
            m = (re.search(r'\bthis\s*->\s*%s\s*\(' % re.escape(name), bstr)
                 or re.search(r'(?<![\w.>:])%s\s*\(' % re.escape(name), bstr))
            if not m:
                continue
            # PRECISION: the base-vs-override divergence only bites if a DESCENDANT actually overrides
            # this method. When the whole-tree hierarchy is known (scan_rules populates override_pairs),
            # skip calls no subclass overrides -- they dispatch identically in C++ and Java. If the index
            # is absent (per-file/unit-test call), stay conservative and flag.
            if plat.override_pairs is not None and (cls, name) not in plat.override_pairs:
                continue
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


def rule_static_init_order(fname, text, header_text, plat):
    """An EAGER static field definition that constructs a user class or reads another class's static
    (`::s_`) runs at static-init time in UNDEFINED cross-translation-unit order -- the static-init-
    order crash class. The converter defers these into initStatics(), run by j2x_initAll() in
    dependency order; this rule flags any that slipped."""
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


def rule_endianness_raw_cast(fname, text, header_text, plat):
    """Big-endian targets (Xbox 360, PS3, Wii) byte-swap multi-byte values. The NBT / save / stream
    code reads & writes one byte at a time (MSB-first) on purpose -- that is endian-agnostic. A raw
    `reinterpret_cast`, `*(int*)ptr`, or `memcpy` over a serialized byte buffer silently corrupts
    saves / desyncs the protocol on big-endian. Skipped entirely on little-endian targets."""
    if not plat.big_endian:
        return
    if not fname.endswith('.cpp') or not _BINARY_IO_FILE.search(fname):
        return
    for i, raw, code in _iter_code_lines(text):
        if not code:
            continue
        if _RAW_CAST.search(code):
            yield (i, Category.WARNING, Severity.MED, 'endianness',
                   "raw multi-byte cast/memcpy in a binary-serialization file -- on big-endian "
                   "%s this byte-swaps and corrupts saves/protocol. Read/write byte-at-a-time "
                   "(MSB-first), then bit-cast the assembled int/long to float/double."
                   % plat.label,
                   raw.strip()[:160])


# ---------------------------------------------------------------------------
# Rule: nullable getter return dereferenced without null check
# ---------------------------------------------------------------------------
from codebreaker.java import typemap as _typemap

_GETTER_DEREF = re.compile(r'(\w+)\s*\([^)]*\)\s*->\s*\w+')

def rule_null_deref_getter(fname, text, header_text, plat):
    """getter()->member where the getter can return NULL. Dereferencing without a null check
    is undefined behaviour in C++ (Java would throw NPE). The return must be captured into
    a local and null-checked before dereference."""
    if not (fname.endswith('.cpp') or fname.endswith('.h')):
        return
    for i, raw, code in _iter_code_lines(text):
        if not code or '->' not in code:
            continue
        for m in _GETTER_DEREF.finditer(code):
            fn_name = m.group(1)
            if fn_name in _typemap.KNOWN_NULLABLE_GETTERS:
                yield (i, Category.RISK, Severity.HIGH, 'null-deref-getter',
                       "%s() can return NULL -- dereferencing without a null check is a %s. "
                       "Capture into a local and check before use."
                       % (fn_name, plat.null_deref_msg),
                       raw.strip()[:160])


# ---------------------------------------------------------------------------
# Rule: known-nullable field dereferenced without a preceding null check
# ---------------------------------------------------------------------------
_FIELD_DEREF = re.compile(r'(?:this->)?(\w+)->')

def rule_null_deref_field(fname, text, header_text, plat):
    """A known-nullable field (worldObj, mc, thePlayer, etc.) dereferenced on a line
    where there is no preceding null check."""
    if not fname.endswith('.cpp'):
        return
    lines = text.split('\n')
    for i, raw_line in enumerate(lines):
        code = raw_line.split('//', 1)[0]
        if not code.strip() or '->' not in code:
            continue
        for m in _FIELD_DEREF.finditer(code):
            field = m.group(1)
            if field not in _typemap.KNOWN_NULLABLE_FIELDS:
                continue
            if re.search(r'\b%s\s*[!=]= (?:NULL|nullptr|0)\b' % re.escape(field), code):
                continue
            guarded = False
            for j in range(max(0, i - 5), i):
                prev = lines[j].split('//', 1)[0]
                if re.search(r'\b%s\s*[!=]= (?:NULL|nullptr|0)\b' % re.escape(field), prev):
                    guarded = True
                    break
            if not guarded:
                lineno = i + 1
                yield (lineno, Category.RISK, Severity.MED, 'null-deref-field',
                       "%s can be NULL -- dereferencing without a null check is a %s. "
                       "Add `if (%s != NULL)` guard."
                       % (field, plat.null_deref_msg, field),
                       raw_line.strip()[:160])
                break


# ---------------------------------------------------------------------------
# Hang / heap-exhaustion class: per-iteration heap allocation inside a loop
# ---------------------------------------------------------------------------
def _loop_body_span(text, hdr_open):
    """Given the index of a `for`/`while` keyword, return (body_start, body_end) covering its body:
    the balanced `{...}` if the next non-space after the header `)` is `{`, else up to the next `;`."""
    i, n = hdr_open, len(text)
    m = re.compile(r'\b(for|while)\b').match(text, hdr_open)
    if not m:
        return None
    p = text.find('(', m.end())
    if p < 0:
        return None
    depth, j = 0, p
    while j < n:                                          # balance the header parens
        if text[j] == '(':
            depth += 1
        elif text[j] == ')':
            depth -= 1
            if depth == 0:
                break
        j += 1
    k = j + 1
    while k < n and text[k] in ' \t\r\n':
        k += 1
    if k < n and text[k] == '{':
        depth, t = 0, k
        while t < n:
            if text[t] == '{':
                depth += 1
            elif text[t] == '}':
                depth -= 1
                if depth == 0:
                    return k, t
            t += 1
        return k, n
    end = text.find(';', k)
    return (k, end if end >= 0 else n)


# hot per-frame / per-chunk / per-entity methods where a big loop runs on the 360's tiny budget
_HOTPATH = re.compile(r'\b(generate|populate|render\w*|doRender\w*|tick|onUpdate|updateEntity|'
                      r'onLivingUpdate|updateRenderer\w*|getBlock\w*|relightBlock|updateLighting|'
                      r'func_\w+)\s*\(')
_NEW_OBJ = re.compile(r'\bnew\s+([A-Z]\w*)\s*[({]')       # heap object (not new T[]) construction


def rule_hotpath_alloc(fname, text, header_text, plat):
    """PERF/HANG (flag-only): a heap `new` INSIDE a loop allocates once PER ITERATION. Java's GC hides
    this; on the 360 a large loop (a chunk/tree/entity scan run millions of times) becomes an
    allocation storm -> heap exhaustion / multi-second stall / apparent hang. Higher severity inside a
    hot-path method. Not auto-fixed (the fix -- hoist the allocation, pool it, or stack it -- needs
    human judgement); flagged for review."""
    if not fname.endswith('.cpp'):
        return
    seen = set()
    for lm in re.finditer(r'\b(?:for|while)\b', text):
        span = _loop_body_span(text, lm.start())
        if not span:
            continue
        bstart, bend = span
        body = text[bstart:bend]
        # is this loop lexically inside a hot-path method? (cheap: scan back to the enclosing signature)
        head = text[max(0, lm.start() - 4000):lm.start()]
        hot = bool(_HOTPATH.search(head.rsplit('\n\n', 1)[-1]))
        # One-time init: a `new` in a loop inside a CONSTRUCTOR runs once per object (building fixed
        # structures -- the chunk grid, the entity roster), NOT once per frame. That is not an allocation
        # hot path, so don't flag it. The nearest `X::Y(` before the loop is the enclosing definition;
        # `X::X(` is a constructor. Per-frame churn lives in tick/render/collision methods (still flagged).
        sigm = None
        for sm in re.finditer(r'\b([A-Za-z_]\w*)\s*::\s*(~?)\s*([A-Za-z_]\w*)\s*\(', head):
            sigm = sm
        if sigm and sigm.group(2) == '' and sigm.group(1) == sigm.group(3):
            continue
        for nm in _NEW_OBJ.finditer(body):
            line = text.count('\n', 0, bstart + nm.start()) + 1
            if line in seen:
                continue
            seen.add(line)
            sev = Severity.HIGH if hot else Severity.MED
            yield (line, Category.RISK, sev, 'hotpath-alloc',
                   "heap `new %s` inside a loop allocates once PER ITERATION -- on %s this is an "
                   "allocation storm / heap-exhaustion hang when the loop is large (Java's GC hid it). "
                   "Hoist it out of the loop, pool it, or stack-allocate it."
                   % (nm.group(1), plat.label),
                   text[bstart + nm.start():bstart + nm.start() + 120].split('\n')[0].strip())


# ---------------------------------------------------------------------------
# Hang class: an unbounded loop with no exit in its body
# ---------------------------------------------------------------------------
def rule_infinite_loop(fname, text, header_text, plat):
    """HANG (flag-only): `while (true)` / `for (;;)` whose body contains no break / return / goto /
    throw -- an unconditional infinite loop. On the 360 this wedges the (often main) thread with no
    watchdog. Flagged for review (a converter mistranslation of a labeled break, or a genuinely
    unbounded loop, both surface here)."""
    if not fname.endswith('.cpp'):
        return
    for lm in re.finditer(r'\b(while\s*\(\s*(?:true|1)\s*\)|for\s*\(\s*;\s*;\s*\))', text):
        span = _loop_body_span(text, lm.start())
        if not span:
            continue
        bstart, bend = span
        body = text[bstart:bend]
        # A loop that Sleeps / waits / yields is an INTENTIONAL service/daemon loop (e.g. the HeapTrack
        # monitor `for(;;){ Sleep(2500); ... }`), not a hang -- don't flag it.
        if not re.search(r'\b(break|return|goto|throw|exit|Sleep|WaitFor\w+|SwitchToThread|MsgWaitFor\w+)\b', body):
            line = text.count('\n', 0, lm.start()) + 1
            yield (line, Category.RISK, Severity.MED, 'infinite-loop',
                   "unbounded loop (`%s`) with no break/return/goto in its body -- wedges the thread "
                   "on %s (no watchdog). Verify it has an exit; a mistranslated labeled break lands here."
                   % (lm.group(1), plat.label),
                   lm.group(0))


# ---------------------------------------------------------------------------
# Leak class: a heap local reassigned in a loop without freeing the prior value
# ---------------------------------------------------------------------------
def rule_leak_loop_reassign(fname, text, header_text, plat):
    """LEAK (flag-only): a local pointer reassigned via `new` inside a loop, with no `delete` of the
    prior value first -- every iteration leaks the previous object (the WorldGenBigTree reuse pattern
    the live tree fixed by hand). The throwaway `(new T())->m()` case is auto-fixed elsewhere; this is
    the harder reassign-in-loop case, flagged for a human `delete before reassign` + `delete after loop`."""
    if not fname.endswith('.cpp'):
        return
    for lm in re.finditer(r'\b(?:for|while)\b', text):
        span = _loop_body_span(text, lm.start())
        if not span:
            continue
        bstart, bend = span
        body = text[bstart:bend]
        for am in re.finditer(r'\b(\w+)\s*=\s*new\s+[A-Z]\w*\s*[({]', body):
            var = am.group(1)
            if re.search(r'\bdelete\b[^;]*\b%s\b' % re.escape(var), body[max(0, am.start() - 200):am.start()]):
                continue                                  # a delete of this var precedes the reassign
            if re.search(r'\bdelete\b[^;{}]*\b%s\b' % re.escape(var), text[max(0, bstart - 500):bstart]):
                continue                                  # freed BEFORE the loop (e.g. the raycast: delete at top,
                                                          # reassign once then return -- not a per-iteration leak)
            tail = body[am.end():am.end() + 60]
            if re.match(r'[^;{}]*;\s*(?:return|break)\b', tail):
                continue                                  # reassign is immediately followed by return/break
            line = text.count('\n', 0, bstart + am.start()) + 1
            yield (line, Category.RISK, Severity.MED, 'leak-loop-reassign',
                   "`%s = new ...` inside a loop with no `delete %s` first -- each iteration leaks the "
                   "previous object (Java GC'd it). Add `delete %s;` before the reassign and once after "
                   "the loop." % (var, var, var),
                   am.group(0)[:120])
            break                                         # one finding per loop is enough


# ---------------------------------------------------------------------------
# Side-effect preservation net (the RenderPig class): an impure statement inside a guarded branch
# ---------------------------------------------------------------------------
# impure = a call, an allocation, or an assignment to non-local (field/global/native) state.
# An impure statement: a call, a `new`, or an assignment to field/global/native state. Broad on purpose --
# the RenderPig bug was a plain method CALL (`loadTexture`) in a collapsed guard, so calls must count. The
# call pattern EXCLUDES control keywords (`if (x) ;`, `while (y) ;`) -- those are not function calls, and a
# guard body reduced to `if (cond) ;` must not read as an impure statement.
_IMPURE = re.compile(r'\b(?!(?:if|for|while|switch|return|sizeof|catch|do)\b)\w+\s*\([^)]*\)\s*;'
                     r'|\bnew\s+[A-Za-z_]|->\s*\w+\s*=(?!=)|::\s*\w+\s*=(?!=)|\bthis->\w+\s*=(?!=)')
# NOISE FILTER, keyed on the two real false-positive shapes (not on statement kind):
#  (1) the guard is a FAILURE / NULL / edge check -- `if (FAILED(hr) || !t->tex)`, `if (n == 0)` -- so the
#      branch is an error/early-out path where a side effect + return is expected, not the RenderPig shape;
#  (2) the body's only impure statements are CLEANUP -- Release/delete/free/close/Unlock -- ordinary teardown.
# The RenderPig shape is a *normal* guard whose body does constructive work (loadTexture) then returns.
# A guard is an early-out (error / null / bounds / state-compare) -- NOT the RenderPig shape -- when it
# contains a FAILED/SUCCEEDED call, a negation, or ANY relational/equality operator against a sentinel.
# The RenderPig collapse only threatens a *plain predicate* guard (`if (cond)`, `if (isSolidTile(...))`)
# whose branch does constructive work; those have no comparison operator and stay flagged.
_ERR_GUARD = re.compile(r'\bFAILED\s*\(|\bSUCCEEDED\s*\(|[=!<>]=|!\s*[\w(]|(?<![-\w<>])[<>](?![<>=])')
_CLEANUP_CALL = re.compile(r'\b(?:Release|delete|free|close|Unlock|EndScene|Reset|clear|dispose|destroy)\b',
                           re.IGNORECASE)


def rule_side_effect_in_branch(fname, text, header_text, plat):
    """SIDE-EFFECT PRESERVATION audit net. Flags an `if (...) { ... }` block that contains BOTH an
    impure statement (call / `new` / assignment to field/global/native state) AND a `return` -- the
    exact shape a naive 'collapse the guard into a predicate return' would corrupt by dropping or
    unconditionally hoisting the side effect (the RenderPig loadTexture bug). The converter is
    order-preserving so these are almost always intact; the tag `J2X-REVIEW[side-effect-in-branch]`
    makes every one greppable so a full-codebase pass can confirm the guard survived."""
    if not fname.endswith('.cpp'):
        return
    for m in re.finditer(r'\bif\s*\(', text):
        p = m.end() - 1                                   # balance the condition parens
        depth, j, n = 0, p, len(text)
        while j < n:
            if text[j] == '(':
                depth += 1
            elif text[j] == ')':
                depth -= 1
                if depth == 0:
                    break
            j += 1
        k = j + 1
        while k < n and text[k] in ' \t\r\n':
            k += 1
        if k >= n or text[k] != '{':                      # only braced blocks (single-stmt is low-risk)
            continue
        depth, t = 0, k
        while t < n:
            if text[t] == '{':
                depth += 1
            elif text[t] == '}':
                depth -= 1
                if depth == 0:
                    break
            t += 1
        cond = text[m.end():j]                            # the condition text, between the parens
        body = text[k + 1:t]
        if _ERR_GUARD.search(cond):                       # (1) failure / null / edge guard -> expected path
            continue
        # (2) body whose impure work is ONLY cleanup (every impure line matches a teardown verb)
        impure_lines = [ln for ln in body.split(';') if _IMPURE.search(ln + ';')]
        if impure_lines and all(_CLEANUP_CALL.search(ln) for ln in impure_lines):
            continue
        # (3) a call/construction that IS the returned value (`return f(x);`, `return std::string(s);`) is
        # not a side-effecting STATEMENT -- it just computes the result. Blank return expressions before the
        # impure test so only a real separate side effect (`this->x = new ...; return;`) still flags.
        body_side = re.sub(r'\breturn\b[^;]*;', ' ', body)
        if 'return' in body and _IMPURE.search(body_side) and 'J2X-REVIEW' not in body:
            line = text.count('\n', 0, m.start()) + 1
            yield (line, Category.RISK, Severity.LOW, 'side-effect-in-branch',
                   "J2X-REVIEW[side-effect-in-branch]: a guarded branch runs an impure statement AND "
                   "returns -- confirm the side effect still executes ONLY under this guard (it must "
                   "never be collapsed/hoisted into an unconditional prelude; the RenderPig class).",
                   text[m.start():k].replace('\n', ' ')[:120])


_DEBUG_BYPASS = re.compile(r'\b(OutputDebugString[AW]?|printf|fprintf|std::(?:cout|cerr)|puts)\s*[\(<]')


def rule_debug_bypass(fname, text, header_text, plat):
    """Flags raw debug output (printf / OutputDebugString / cout) that does NOT go through the single
    Debug.h `DBG_*` macros. The user's rule: all debug instrumentation must route through the one
    master switch (DEBUG_ENABLED) so toggling it off silences everything. A stray printf stays hot in
    a retail build and spams the console -- tag it `J2X-REVIEW[debug-bypass]` so it is greppable and
    can be converted to a DBG_* macro (or removed)."""
    if not fname.endswith('.cpp'):
        return
    if 'Debug.cpp' in fname:                               # the backend itself legitimately uses printf
        return
    for m in _DEBUG_BYPASS.finditer(text):
        line = text.count('\n', 0, m.start()) + 1
        # skip lines already routed/annotated
        ls = text.rfind('\n', 0, m.start()) + 1
        le = text.find('\n', m.start())
        linetext = text[ls:le if le != -1 else len(text)]
        if 'DBG_' in linetext or 'J2X-REVIEW' in linetext or 'Debug_Printf' in linetext:
            continue
        yield (line, Category.RISK, Severity.LOW, 'debug-bypass',
               "J2X-REVIEW[debug-bypass]: raw debug output not routed through Debug.h -- convert to a "
               "DBG_* macro so the master DEBUG_ENABLED switch governs it (else it stays hot in retail "
               "and spams the console).",
               linetext.strip()[:120])


_FUNC_DEF = re.compile(r'(?m)^([A-Za-z_][\w:<>,\*&\s]*?\b[\w:~]+)\s*\(([^;{}]*)\)\s*(?:const\s*)?\{')

def _iter_functions(text):
    """Yield (signature, body_text, body_start_offset) for each top-level function DEFINITION (brace-matched,
    string/char-literal aware). Shared by the leak detectors, which are function-scoped."""
    for m in _FUNC_DEF.finditer(text):
        ob = m.end() - 1
        depth, j, n = 0, ob, len(text)
        while j < n:
            c = text[j]
            if c == '"' or c == "'":
                q = c; j += 1
                while j < n:
                    if text[j] == '\\': j += 2; continue
                    if text[j] == q: break
                    j += 1
            elif c == '{': depth += 1
            elif c == '}':
                depth -= 1
                if depth == 0: break
            j += 1
        yield (m.group(1).strip(), text[ob + 1:j], ob + 1)


def rule_leak_temp_new(fname, text, header_text, plat):
    """No-GC TEMP LEAK: a local `T* v = new ...` that is never delete'd, returned, stored to a field/local,
    or handed to a container in its function -> leaks on the GC-less 360 heap. The per-call Random /
    comparator / temp-point class (renderstars-random-leak, entitysorter-comparator-leak, noisegen-random-
    temp-leak, pathfinder-dims-point-leak). NOTE: a receiver-only use like `(new Random())->nextInt()` or
    passing to `Arrays::sort` does NOT transfer ownership -- still a leak."""
    if not fname.endswith('.cpp'):
        return
    for sig, body, off in _iter_functions(text):
        for am in re.finditer(r'\b[A-Za-z_][\w:]*\s*\*\s*(\w+)\s*=\s*new\b', body):
            v = re.escape(am.group(1))
            rest = body[am.end():]
            nr = r'(?!\s*(?:->|\.|\(|\[))'          # var used as a VALUE, not a receiver (`v->m()`) or `v[i]`
            escaped = (
                re.search(r'\bdelete\b[^;{}]*\b' + v + r'\b', rest) or               # freed
                re.search(r'\breturn\b[^;{}]*\b' + v + r'\b' + nr, rest) or          # returned (the value itself)
                re.search(r'(?<![=<>!])=\s*' + v + r'\b' + nr, rest) or              # stored: x = v / field = v
                re.search(r'(?:\.|->)(?:add\w*|push_back|push_front|put|insert|append|offer|set\w*|register\w*)'
                          r'\s*\([^;{}]*\b' + v + r'\b', rest))                       # added to a container/registry
            if escaped:
                continue
            line = text.count('\n', 0, off + am.start()) + 1
            yield (line, Category.RISK, Severity.MED, 'leak-temp-new',
                   "local `%s = new ...` is never delete'd, returned, stored, or added to a container in this "
                   "function -> leaks on the GC-less heap (Java GC'd it). `delete %s;` on every exit path."
                   % (am.group(1), am.group(1)),
                   am.group(0)[:120])
            break                                     # one finding per function is enough


def rule_leak_dtor_member(fname, text, header_text, plat):
    """No-GC DTOR-MEMBER LEAK: a pointer field assigned `new` in the constructor but NOT `delete`d in the
    destructor -> leaks for the object's whole lifetime. Stronger than rule-of-three (which only checks the
    dtor EXISTS): this names each unfreed member. The ledger's chunkprovider-mapgen-dtor-leak /
    mapgenbase-rand-dtor-leak / noiseoctaves-subgen-dtor-leak class."""
    if not fname.endswith('.cpp'):
        return
    cls = fname[:-4]
    ctor_new, dtor_body, ctor_line = set(), None, {}
    for sig, body, off in _iter_functions(text):
        if re.search(r'\b' + re.escape(cls) + r'::~' + re.escape(cls) + r'\b', sig):
            dtor_body = (dtor_body or '') + body
        elif re.search(r'\b' + re.escape(cls) + r'::' + re.escape(cls) + r'\b', sig):
            # the constructor's own locals (`T* v = NULL;` first, `v = new ...` later: the back end's ordered-
            # operand temporaries j2xoN, a decompiler local) are not members
            local = set(re.findall(r'\b[A-Za-z_][\w:<>]*\s*\*+\s*(\w+)\s*(?:=|;)', body))
            for mm in re.finditer(r'(?:this->)?(\w+)\s*=\s*new\b', body):
                if mm.group(1) in local and not mm.group(0).startswith('this->'):
                    continue
                ctor_new.add(mm.group(1))
                ctor_line.setdefault(mm.group(1), text.count('\n', 0, off + mm.start()) + 1)
    if not ctor_new:
        return
    if dtor_body is None:
        return                                        # no dtor at all -> rule-of-three already reports it
    for mem in sorted(ctor_new):
        if re.search(r'\bdelete\b[^;{}]*\b' + re.escape(mem) + r'\b', dtor_body):
            continue
        yield (ctor_line[mem], Category.RISK, Severity.HIGH, 'leak-dtor-member',
               "member '%s' is `new`'d in the constructor but never `delete`d in ~%s() -> leaks for the "
               "object's whole lifetime on the GC-less heap. Add `delete %s;` to the destructor."
               % (mem, cls, mem),
               ("%s = new ..." % mem))


def rule_ptr_container_compare(fname, text, header_text, plat):
    """A std::set<T*> / std::map<T*,..> / JSet<T*> compares & keys by POINTER ADDRESS, not by value. Where
    the game means value-equality (Java HashSet/HashMap use .equals/.hashCode), two logically-equal objects
    both count as 'present' (duplicates) and lookups by an equal-but-different pointer MISS. The ledger's
    scheduledtick-pointer-compare-explosion / regionfile-cache-never-hits class. Flags a set/map whose KEY
    is a non-primitive pointer."""
    if not (fname.endswith('.h') or fname.endswith('.cpp')):
        return
    prim = {'int', 'char', 'short', 'long', 'float', 'double', 'bool', 'unsigned', 'size_t', 'void',
            'int8_t', 'uint8_t', 'int16_t', 'uint16_t', 'int32_t', 'uint32_t', 'int64_t', 'uint64_t', 'wchar_t'}
    for i, raw, code in _iter_code_lines(text):
        for m in re.finditer(r'\b(?:std::set|std::unordered_set|JSet)\s*<\s*([\w:]+)\s*\*\s*>', code):
            if m.group(1) not in prim:
                yield (i, Category.RISK, Severity.MED, 'ptr-container-compare',
                       "std::set<%s*> compares by POINTER ADDRESS, not value (Java HashSet uses .equals/"
                       ".hashCode) -- logically-equal entries duplicate and lookups miss. Use a value "
                       "comparator or key by a value type. (scheduledtick-pointer-compare class.)" % m.group(1),
                       code.strip()[:150])
        for m in re.finditer(r'\b(?:std::map|std::unordered_map|JMap)\s*<\s*([\w:]+)\s*\*\s*,', code):
            if m.group(1) not in prim:
                yield (i, Category.RISK, Severity.MED, 'ptr-container-compare',
                       "std::map<%s*, ...> keys by POINTER ADDRESS, not value -- a lookup with an equal-but-"
                       "different key MISSES. Key by a value type or add a comparator. "
                       "(regionfile-cache-never-hits class.)" % m.group(1),
                       code.strip()[:150])


def rule_map_read_insert(fname, text, header_text, plat):
    """C++ `map[key]` INSERTS a default on READ, where Java's HashMap.get() returns null. This is the
    map-get-inserts / datawatcher-null-deref-hang class from the ledger: a lookup like `if (chunks[key])`
    silently grows the map with a null entry -> chunkExists() lies -> infinite generation recursion ->
    FREEZE. Flags operator[] READS on map-typed variables. Writes (`map[k] = v`) are the legitimate insert
    and are left alone."""
    if not fname.endswith('.cpp'):
        return
    hay = text + (('\n' + header_text) if header_text else '')
    maps = set(re.findall(
        r'\b(?:std::map|std::unordered_map|JMap|HashMap\w*|TreeMap\w*|LinkedHashMap\w*)\s*<[^;{}]*>\s*\*?\s*(\w+)', hay))
    if not maps:
        return
    for i, raw, code in _iter_code_lines(text):
        for name in sorted(maps):                         # (sorted: the findings of one line come in one order)
            m = re.search(r'\b' + re.escape(name) + r'\s*\[[^\]]+\]', code)
            if not m:
                continue
            after = code[m.end():].lstrip()
            if after.startswith('=') and not after.startswith('=='):
                continue                              # a write: map[k] = v  (intended insert)
            yield (i, Category.RISK, Severity.HIGH, 'map-read-insert',
                   "%s[...] READ: C++ map::operator[] INSERTS a default on read (Java .get() returns null). "
                   "The silent insert makes a 'contains?' check lie -- the chunkExists->gen-recursion FREEZE "
                   "class. Use .count()/.find() for lookups; reserve [] for writes." % name,
                   raw.strip()[:150])
            break


def rule_throw_fallthrough(fname, text, header_text, plat):
    """On this console C++ `throw` is a NO-OP that does NOT unwind -- execution FALLS THROUGH past it. When
    Java relied on the throw to abort, the code after it now runs -> state corruption / garbage returns.
    The ledger's tessellator-state-throw / path-addpoint-state-throw / root-tag-not-compound-throw class
    (and readfully-eof). Flags a `throw` that is followed by more code in its block (not right before the
    block's `}`)."""
    if not fname.endswith('.cpp'):
        return
    rows = [(i, code.strip()) for i, raw, code in _iter_code_lines(text) if code.strip()]
    for idx in range(len(rows) - 1):
        i, s = rows[idx]
        if not s.startswith('throw'):
            continue
        if len(s) > 5 and (s[5].isalnum() or s[5] == '_'):
            continue                                  # 'throws'/'throwFoo', not a throw statement
        nxt = rows[idx + 1][1]
        if not nxt or nxt[0] in '})' or nxt.startswith(('case ', 'default:', '#')):
            continue                                  # throw is last in its block -> abort == return here
        yield (i, Category.RISK, Severity.MED, 'throw-fallthrough',
               "`throw` does NOT unwind on this console -- it is a no-op and the code after it STILL RUNS "
               "(Java aborted here). Replace with an early `return` / guarded skip so the fall-through can't "
               "corrupt state or return garbage. (The _TRACE_THROWS runtime tracer names any that fire.)",
               s[:150])


def rule_leak_drop_no_delete(fname, text, header_text, plat):
    """No-GC DROP LEAK: an OWNING container of pointers (JList<T*>, std::vector<T*>, JMap<..,T*>, set...) has
    an element removed (.clear/.erase/.remove/.pop*) with no `delete` of that element nearby -> the object
    is dropped but never freed on the GC-less heap. The ledger's dead-particle-leak / clear-effects-leak /
    client-chunk-drop-leak / net-*-packets-leak class."""
    if not fname.endswith('.cpp'):
        return
    hay = text + (('\n' + header_text) if header_text else '')
    owners = set(re.findall(
        r'\b(?:JList|std::vector|JSet|std::set|JMap|std::map|std::deque)\s*<[^;{}]*\*\s*>\s*\*?\s*(\w+)', hay))
    if not owners:
        return
    # Observer/callback lists hold NON-owned references (registered from outside) -- removing does not free.
    owners = {o for o in owners if not re.search(r'(?i)(listener|observer|callback|handler|watcher|subscriber)s?$', o)}
    rows = [(i, code) for i, raw, code in _iter_code_lines(text)]
    for idx, (i, code) in enumerate(rows):
        for name in sorted(owners):                       # (sorted: the findings of one line come in one order)
            m = re.search(r'\b' + re.escape(name) + r'(?:\.|->)(clear|erase|remove|pop_back|pop_front|pop)\s*\(', code)
            if not m:
                continue
            window = ' '.join(c for _i, c in rows[max(0, idx - 6):idx + 1])
            if 'delete' in window:
                continue                              # a delete of the removed element is nearby
            yield (i, Category.RISK, Severity.MED, 'leak-drop-no-delete',
                   "%s.%s() removes from an OWNING pointer container with no `delete` -- the object is dropped "
                   "but never freed on the GC-less heap. Capture the removed pointer and delete it first. "
                   "(dead-particle / clear-effects / chunk-drop / packet leak class.)" % (name, m.group(1)),
                   code.strip()[:150])
            break


_DISPATCH_IF = re.compile(r'\bif\s*\(\s*(\w+)\s*==\s*[A-Z][A-Z0-9_]{2,}')

def rule_silent_dispatch(fname, text, header_text, plat):
    """FAIL-LOUD DOCTRINE. An enum DISPATCHER that can fall through SILENTLY on a value it doesn't handle --
    exactly how the fog bug hid: `glFogf`/`glFogi` were `if(pname==GL_FOG_START) else if(pname==GL_FOG_END)`
    chains with no final else, so GL_FOG_DENSITY / GL_FOG_MODE matched nothing and were dropped with no
    error. RULE: a dispatcher must END in a loud report (J2X_UNHANDLED) or an explicit J2X_NOOP. This flags
    `if(x==CONST) else if(x==CONST) ...` chains (2+ branches on the same var) that have neither a terminal
    else nor a marker. Chain-scoped (gathers only the if + following `else*` lines) to avoid bleeding into
    neighbouring code."""
    if not (fname.endswith('.cpp') or fname.endswith('.h')):
        return
    lines = text.split('\n')
    n = len(lines)
    for i in range(n):
        ln = lines[i]
        m = _DISPATCH_IF.search(ln)
        if not m:
            continue
        if ln[:m.start()].rstrip().endswith('else'):
            continue                                  # this `if` is an `else if` -- not a chain start
        var = m.group(1)
        chain = [ln]                                  # gather the chain: this line + following `else*` lines
        k = i + 1
        while k < n and (k - i) < 40:                 # skip interior comment/blank lines so an explanatory
            s = lines[k].strip()                      # comment between two else-ifs doesn't truncate the chain
            if s.startswith('else') or s == '' or s.startswith(('//', '/*', '*')):
                chain.append(lines[k]); k += 1
            else:
                break
        region = '\n'.join(chain)
        elseifs = len(re.findall(r'\belse\s+if\s*\(\s*' + re.escape(var) + r'\s*==\s*[A-Z][A-Z0-9_]{2,}', region))
        if elseifs < 1:
            continue                                  # a lone `if` (e.g. `if (rc==A || rc==B)`) -- not a
                                                      # multi-branch else-if DISPATCH; nothing to fall through
        branches = elseifs + 1
        if 'J2X_UNHANDLED' in region or 'J2X_NOOP' in region:
            continue                                  # reports the gap, or explicitly blessed
        if re.search(r'\belse\b(?!\s*if)', region):
            continue                                  # has a terminal else (assume it handles/reports)
        yield (i + 1, Category.RISK, Severity.HIGH, 'silent-dispatch',
               "enum dispatch on '%s' (%d cases) falls through SILENTLY on an unhandled value -- the "
               "fog-bug class. End the chain with J2X_UNHANDLED(\"<fn>\", %s), or J2X_NOOP(\"why\") if the "
               "silent fall-through is deliberate." % (var, branches, var),
               ln.strip()[:160])


# Registry -- (id, description, function). Add new rules here.
RULES = [
    ('silent-dispatch', 'enum dispatcher can fall through silently on an unhandled value (fail-loud doctrine)', rule_silent_dispatch),
    ('map-read-insert',  'map[key] READ inserts a default (Java .get() returns null) -> chunkExists lies -> freeze', rule_map_read_insert),
    ('throw-fallthrough','throw is a no-op that does not unwind on the 360 -> code after it still runs', rule_throw_fallthrough),
    ('leak-drop-no-delete','element removed from an owning pointer container without delete (no-GC drop leak)', rule_leak_drop_no_delete),
    ('leak-temp-new',    'local new never freed/returned/stored/added -> no-GC temp leak', rule_leak_temp_new),
    ('leak-dtor-member', 'member new-d in ctor but not deleted in dtor -> no-GC lifetime leak', rule_leak_dtor_member),
    ('ptr-container-compare','std::set/map keyed by a non-primitive pointer -> compares by address, not value', rule_ptr_container_compare),
    ('uninit-array',    'new T[n] without () leaves primitives/pointers uninitialized', rule_uninit_array),
    ('ctor-stub',       'empty or delegation-only constructor body (missing registration)', rule_ctor_stub),
    ('rule-of-three',   'owns heap but missing destructor / copy control', rule_rule_of_three),
    ('value-copy-owner','heavy/owning type passed or returned by value', rule_value_copy_owner),
    ('virtual-call-in-ctor', 'constructor calls own virtual method (dispatches to base, not override)',
     rule_virtual_call_in_ctor),
    ('static-init-order', 'eager static init with a cross-TU dependency (undefined init order)',
     rule_static_init_order),
    ('endianness',      'raw multi-byte cast in binary-IO file (big-endian hazard)',
     rule_endianness_raw_cast),
    ('null-deref-getter', 'getter return dereferenced without null check',
     rule_null_deref_getter),
    ('null-deref-field', 'known-nullable field dereferenced without null check',
     rule_null_deref_field),
    ('hotpath-alloc',   'heap allocation inside a loop (per-iteration alloc storm / hang)',
     rule_hotpath_alloc),
    ('infinite-loop',   'unbounded loop with no break/return in body (thread wedge)',
     rule_infinite_loop),
    ('leak-loop-reassign', 'heap local reassigned in a loop without freeing the prior value',
     rule_leak_loop_reassign),
    ('side-effect-in-branch', 'impure statement in a guarded branch that returns (RenderPig class)',
     rule_side_effect_in_branch),
    ('debug-bypass',    'raw debug output not routed through the single Debug.h master switch',
     rule_debug_bypass),
]


_CLASS_HDR = re.compile(r'\b(?:class|struct)\s+([A-Za-z_]\w*)\s*(?:final\s*)?(?::([^{;()]*))?\{')
_NOT_METHODS = frozenset(('if', 'for', 'while', 'switch', 'return', 'sizeof', 'catch', 'new', 'delete'))


def _match_brace(src, open_idx):
    """Index of the '}' matching the '{' at open_idx (len(src) if unbalanced)."""
    depth = 0
    for j in range(open_idx, len(src)):
        if src[j] == '{':
            depth += 1
        elif src[j] == '}':
            depth -= 1
            if depth == 0:
                return j
    return len(src)


def _scan_odr(ledger, output_dir):
    """CROSS-FILE ODR: a class/struct DEFINED with a body in MORE THAN ONE header -> one-definition-rule
    violation (the ledger's outputstream-odr / inputstream-odr / file-odr / stream-odr C4743 class). The
    per-file rules can't see this; it needs the whole tree. Keep ONE canonical definition per type."""
    defs = {}    # class name -> { file: normalized-body }
    for fname in sorted(f for f in os.listdir(output_dir) if f.endswith('.h')):
        src = _strip_block_comments(_read(os.path.join(output_dir, fname)))
        for m in _CLASS_HDR.finditer(src):
            ob = m.end() - 1
            norm = re.sub(r'\s+', ' ', src[ob:_match_brace(src, ob)]).strip()   # whitespace-insensitive body
            defs.setdefault(m.group(1), {})[fname] = norm
    n = 0
    for name, filemap in sorted(defs.items()):
        # Identical definitions across guarded headers are LEGAL (ODR permits it) and compile fine -- only
        # DIFFERING definitions of the same name are a real violation (the C4743 / mismatched-layout class).
        bodies = set(filemap.values())
        if len(filemap) > 1 and len(bodies) > 1:
            fs = sorted(filemap)
            ledger.add(fs[0], Category.RISK,
                       "class '%s' has DIFFERING definitions across %d headers: %s -- one-definition-rule "
                       "violation (mismatched layout / C4743 class). Keep ONE canonical definition; the rest "
                       "must #include it, not redefine it differently." % (name, len(fs), ', '.join(fs)),
                       severity=Severity.HIGH, source='rule', rule='odr-duplicate-def')
            n += 1
    return n


def _build_override_pairs(output_dir):
    """{(ancestor_class, method_name)} for every method a DESCENDANT class declares -- i.e. the calls
    that a base constructor invoking `this->method()` would REALLY dispatch wrong (Java calls the
    override; C++ calls the base). Over-capturing a name only makes the rule MORE conservative (flag),
    never less, so a loose method-name scan is safe. Whole-tree; the per-file rule can't see subclasses."""
    parents, methods = {}, {}
    for fname in sorted(f for f in os.listdir(output_dir) if f.endswith('.h')):
        src = _strip_block_comments(_read(os.path.join(output_dir, fname)))
        for m in _CLASS_HDR.finditer(src):
            name = m.group(1)
            bases = []
            if m.group(2):
                for b in m.group(2).split(','):
                    bm = re.search(r'([A-Za-z_]\w*)\s*$', b.strip())
                    if bm:
                        bases.append(bm.group(1))
            ob = m.end() - 1
            body = src[ob + 1:_match_brace(src, ob)]
            mset = set(re.findall(r'(?<![\w>])([A-Za-z_]\w*)\s*\(', body)) - _NOT_METHODS
            mset.discard(name)                                   # the constructor itself
            parents.setdefault(name, []).extend(bases)
            methods.setdefault(name, set()).update(mset)

    def ancestors(c, seen):
        for p in parents.get(c, ()):
            if p not in seen:
                seen.add(p)
                ancestors(p, seen)
        return seen

    pairs = set()
    for cls, mset in methods.items():
        for anc in ancestors(cls, set()):
            for meth in mset:
                pairs.add((anc, meth))
    return frozenset(pairs)


def scan_rules(ledger, output_dir, platform=None):
    """Run every structural rule over the converted tree, appending Events. Returns count."""
    plat = _PlatCtx(platform)
    plat.override_pairs = _build_override_pairs(output_dir)   # cross-file hierarchy for virtual-call-in-ctor
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
            for line, cat, sev, rule, msg, snip in fn(fname, text, header_text, plat):
                ledger.add(fname, cat, msg, line=line, severity=sev,
                           source='rule', rule=rule, snippet=snip)
                n += 1
    n += _scan_odr(ledger, output_dir)          # cross-file ODR pass (per-file rules can't see it)
    return n
