# -*- coding: utf-8 -*-
"""
parse.py -- Module 2: Java source text -> IR (codebreaker.java.ir).

This is the front-end. It extracts STRUCTURE only -- it does not translate
anything. Decompiled-Minecraft Java is a constrained, well-formatted subset
(tab-indented, obfuscated `var1` locals, one declarator per line), so a
brace/paren-aware scan is enough; no full Java grammar required.

Design notes:
  * Comments are blanked (replaced with spaces, newlines kept) rather than
    deleted, so every character offset still maps to its ORIGINAL line number --
    method bodies carry real line numbers for ledger attribution downstream.
  * The body walk separates fields (end in `;` at depth 0) from methods (have a
    `{ }` body at depth 0). Abstract/interface methods (`...);` with params) are
    methods with an empty body.
  * Deferred on purpose (added when `emit` needs them): inner types and
    static-initializer blocks are recognized and skipped, not mis-parsed.
"""

import re

from codebreaker.java import ir

_MODIFIERS = {'public', 'protected', 'private', 'static', 'final', 'abstract',
              'native', 'synchronized', 'transient', 'volatile', 'strictfp'}


# ---------------------------------------------------------------------------
# Lexical helpers
# ---------------------------------------------------------------------------
def _blank_comments(src):
    """Return src with // and /* */ comments replaced by spaces (newlines kept),
    so character offsets and line numbers are preserved. String/char literals
    are respected."""
    out = []
    i, n = 0, len(src)
    in_str = in_chr = False
    while i < n:
        c = src[i]
        nxt = src[i + 1] if i + 1 < n else ''
        if in_str:
            out.append(c)
            if c == '\\' and nxt:
                out.append(nxt); i += 2; continue
            if c == '"':
                in_str = False
            i += 1; continue
        if in_chr:
            out.append(c)
            if c == '\\' and nxt:
                out.append(nxt); i += 2; continue
            if c == "'":
                in_chr = False
            i += 1; continue
        if c == '"':
            in_str = True; out.append(c); i += 1; continue
        if c == "'":
            in_chr = True; out.append(c); i += 1; continue
        if c == '/' and nxt == '/':
            while i < n and src[i] != '\n':
                out.append(' '); i += 1
            continue
        if c == '/' and nxt == '*':
            while i < n and not (src[i] == '*' and i + 1 < n and src[i + 1] == '/'):
                out.append('\n' if src[i] == '\n' else ' '); i += 1
            out.append('  '); i += 2; continue
        out.append(c); i += 1
    return ''.join(out)


def _match_brace(s, open_idx):
    """Index just past the `}` matching the `{` at open_idx (literal-aware)."""
    depth, i, n = 0, open_idx, len(s)
    in_str = in_chr = False
    while i < n:
        c = s[i]
        if in_str:
            if c == '\\':
                i += 2; continue
            if c == '"':
                in_str = False
        elif in_chr:
            if c == '\\':
                i += 2; continue
            if c == "'":
                in_chr = False
        elif c == '"':
            in_str = True
        elif c == "'":
            in_chr = True
        elif c == '{':
            depth += 1
        elif c == '}':
            depth -= 1
            if depth == 0:
                return i + 1
        i += 1
    return n


def _split_top_commas(s):
    """Split on commas not nested in (), <>, [] or literals."""
    parts, depth, buf = [], 0, []
    in_str = in_chr = False
    for c in s:
        if in_str:
            buf.append(c)
            if c == '"':
                in_str = False
            continue
        if in_chr:
            buf.append(c)
            if c == "'":
                in_chr = False
            continue
        if c == '"':
            in_str = True; buf.append(c); continue
        if c == "'":
            in_chr = True; buf.append(c); continue
        if c in '(<[':
            depth += 1
        elif c in ')>]':
            depth -= 1
        if c == ',' and depth == 0:
            parts.append(''.join(buf)); buf = []
        else:
            buf.append(c)
    if ''.join(buf).strip():
        parts.append(''.join(buf))
    return [p.strip() for p in parts if p.strip()]


# ---------------------------------------------------------------------------
# Type / parameter parsing
# ---------------------------------------------------------------------------
def parse_type(s):
    """'int[][]' / 'List<ItemStack>' / 'net.minecraft.Block' -> ir.JType."""
    s = s.strip()
    dims = 0
    while s.endswith('[]'):
        dims += 1
        s = s[:-2].rstrip()
    args = []
    lt = s.find('<')
    if lt != -1 and s.endswith('>'):
        inner = s[lt + 1:-1]
        args = [parse_type(a) for a in _split_top_commas(inner)]
        s = s[:lt].rstrip()
    name = s.split('.')[-1]          # drop package qualifier; emit uses simple names
    return ir.JType(name, dims, args)


def _parse_params(s):
    out = []
    for raw in _split_top_commas(s):
        toks = raw.replace('...', '[]').split()    # varargs -> array
        is_final = 'final' in toks
        toks = [t for t in toks if t != 'final']
        if len(toks) < 2:
            continue
        name = toks[-1]
        type_str = ' '.join(toks[:-1])
        out.append(ir.Param(name, parse_type(type_str), is_final))
    return out


def _split_mods(tokens):
    mods, rest = set(), []
    for t in tokens:
        if t in _MODIFIERS:
            mods.add(t)
        else:
            rest.append(t)
    return mods, rest


def _access_of(mods):
    if 'public' in mods:
        return ir.PUBLIC
    if 'protected' in mods:
        return ir.PROTECTED
    if 'private' in mods:
        return ir.PRIVATE
    return ir.PACKAGE


# ---------------------------------------------------------------------------
# Member parsing
# ---------------------------------------------------------------------------
def _line_of(src, offset):
    return src.count('\n', 0, offset) + 1


def _parse_field_decl(decl, class_name, line):
    """`[mods] Type name [= init]` -> [Field]. One declarator per line (the MCP shape)."""
    eq = _toplevel_index(decl, '=')
    head = (decl[:eq] if eq != -1 else decl).strip()
    init = decl[eq + 1:].strip() if eq != -1 else None
    mods, rest = _split_mods(head.split())
    if len(rest) < 2:
        return []
    type_str = ' '.join(rest[:-1])          # everything before the declarator name
    name = rest[-1].rstrip(',')
    return [ir.Field(name, parse_type(type_str), _access_of(mods),
                     'static' in mods, 'final' in mods, init, line)]


def _toplevel_index(s, ch):
    depth = 0
    in_str = in_chr = False
    for i, c in enumerate(s):
        if in_str:
            if c == '"':
                in_str = False
            continue
        if in_chr:
            if c == "'":
                in_chr = False
            continue
        if c == '"':
            in_str = True; continue
        if c == "'":
            in_chr = True; continue
        if c in '(<[':
            depth += 1
        elif c in ')>]':
            depth -= 1
        elif c == ch and depth == 0:
            return i
    return -1


def _parse_signature(header, class_name):
    """`public final Vec3D subtract(Vec3D var1)` -> Method (no body yet).
    The first '(' opens the param list; return type / name never contain one."""
    lp = header.find('(')
    rp = header.rfind(')')
    params = _parse_params(header[lp + 1:rp]) if lp != -1 and rp > lp else []
    pre = header[:lp].split()
    mods, rest = _split_mods(pre)
    name = rest[-1]
    ret = ' '.join(rest[:-1])
    is_ctor = (name == class_name and not ret)
    return ir.Method(
        name,
        return_type=(None if is_ctor else parse_type(ret) if ret else ir.JType('void')),
        params=params,
        access=_access_of(mods),
        is_static='static' in mods,
        is_abstract='abstract' in mods,
        is_final='final' in mods,
        is_ctor=is_ctor,
    )


def _looks_like_method(decl):
    """A member decl is a method (vs field) if its first '(' is not preceded by '='.
    (A field initializer's '(' always follows the '='.)"""
    lp = decl.find('(')
    if lp == -1:
        return False
    eq = _toplevel_index(decl, '=')
    return eq == -1 or lp < eq


def _iter_members(body, body_offset, src):
    """Yield ('field', decl, line) and ('method', header, body_text, line)."""
    i, n, start = 0, len(body), 0
    in_str = in_chr = False
    seen_eq = False                                    # a '=' since the member start -> initializer
    while i < n:
        c = body[i]
        if in_str:
            if c == '\\':
                i += 2; continue
            if c == '"':
                in_str = False
            i += 1; continue
        if in_chr:
            if c == '\\':
                i += 2; continue
            if c == "'":
                in_chr = False
            i += 1; continue
        if c == '"':
            in_str = True; i += 1; continue
        if c == "'":
            in_chr = True; i += 1; continue
        if c == '=':
            seen_eq = True; i += 1; continue
        if c == ';':
            decl = body[start:i].strip()
            if decl:
                yield ('decl', decl, _line_of(src, body_offset + start))
            i += 1; start = i; seen_eq = False; continue
        if c == '{':
            if seen_eq:                                # field initializer `= ... { ... }` (array/aggregate),
                i = _match_brace(body, i); continue    # not a method body -- skip the braces, keep the field
            header = body[start:i].strip()
            close = _match_brace(body, i)
            inner = body[i + 1:close - 1]
            htoks = set(header.split())
            if htoks & {'class', 'interface', 'enum'}:
                pass                                   # inner type -- skip (deferred)
            elif 'static' in htoks and htoks <= _MODIFIERS:
                yield ('static_init', inner, _line_of(src, body_offset + i + 1))   # static { ... }
            elif not header:
                yield ('inst_init', inner, _line_of(src, body_offset + i + 1))     # instance { ... }
            elif htoks <= _MODIFIERS:
                pass                                   # other modifier-only block -- skip
            else:
                yield ('method', header, inner, _line_of(src, body_offset + start),
                       _line_of(src, body_offset + i + 1))
            i = close; start = i; seen_eq = False; continue
        i += 1


def _body_lines(src, start_line, end_line):
    lines = src.split('\n')
    return [(ln, lines[ln - 1]) for ln in range(start_line, min(end_line, len(lines)) + 1)]


# ---------------------------------------------------------------------------
# Top-level entry
# ---------------------------------------------------------------------------
_TYPE_DECL = re.compile(
    r'(?P<mods>(?:\b(?:public|final|abstract|strictfp)\b\s+)*)'
    r'(?P<kind>class|interface|enum)\s+(?P<name>\w+)(?P<rest>[^{]*)\{')


def parse_source(src, source_path=None):
    blanked = _blank_comments(src)

    pkg_m = re.search(r'\bpackage\s+([\w.]+)\s*;', blanked)
    package = pkg_m.group(1) if pkg_m else None

    m = _TYPE_DECL.search(blanked)
    if not m:
        raise ValueError('no type declaration found' +
                         (' in %s' % source_path if source_path else ''))

    mods = m.group('mods') or ''
    kind = m.group('kind')
    name = m.group('name')
    rest = m.group('rest') or ''

    parent = None
    pm = re.search(r'\bextends\s+([\w.]+)', rest)
    if pm:
        parent = pm.group(1).split('.')[-1]
    interfaces = []
    im = re.search(r'\bimplements\s+([\w.,\s]+)', rest)
    if im:
        interfaces = [x.split('.')[-1].strip() for x in im.group(1).split(',') if x.strip()]

    cls = ir.JClass(
        name, package=package, parent=parent, interfaces=interfaces,
        is_interface=(kind == 'interface'),
        is_abstract=('abstract' in mods),
        is_enum=(kind == 'enum'),
        source_path=source_path,
    )

    open_brace = m.end() - 1
    close = _match_brace(blanked, open_brace)
    body = blanked[open_brace + 1:close - 1]
    body_offset = open_brace + 1

    if cls.is_enum:
        _parse_enum_body(cls, body, body_offset, blanked, src)
        return cls

    for item in _iter_members(body, body_offset, blanked):
        if item[0] == 'decl':
            _, decl, line = item
            if _looks_like_method(decl):                # abstract / interface method
                meth = _parse_signature(decl, name)
                if cls.is_interface:
                    meth.is_abstract = True
                cls.methods.append(meth)
            else:
                cls.fields.extend(_parse_field_decl(decl, name, line))
        elif item[0] in ('static_init', 'inst_init'):   # static { } / instance { } block
            _, inner, bline = item
            block = _body_lines(src, bline, bline + inner.count('\n'))
            (cls.static_inits if item[0] == 'static_init' else cls.inst_inits).append(block)
        else:
            _, header, inner, hline, bline = item
            meth = _parse_signature(header, name)
            meth.line = hline
            # body lines from the ORIGINAL source (real text + real line numbers)
            meth.body = _body_lines(src, bline, bline + inner.count('\n'))
            cls.methods.append(meth)
    return cls


def _parse_enum_body(cls, body, body_offset, blanked, src):
    semi = _toplevel_index(body, ';')
    consts_part = body[:semi] if semi != -1 else body
    for raw in _split_top_commas(consts_part):
        lp = raw.find('(')
        if lp != -1 and raw.rstrip().endswith(')'):
            nm = raw[:lp].strip()
            args = _split_top_commas(raw[lp + 1:raw.rindex(')')])
        else:
            nm, args = raw.strip(), []
        if nm:
            cls.enum_consts.append(ir.EnumConst(nm, args))
    # members after the ';' (enum methods/fields) -- parse like a class body. Bodies are captured
    # from the ORIGINAL src (real text + line numbers) so typed-enum ctors/methods translate; a
    # constant-only enum ignores them (header-only int constants).
    if semi != -1:
        tail = body[semi + 1:]
        for item in _iter_members(tail, body_offset + semi + 1, blanked):
            if item[0] == 'decl':
                _, decl, line = item
                if _looks_like_method(decl):
                    cls.methods.append(_parse_signature(decl, cls.name))
                else:
                    cls.fields.extend(_parse_field_decl(decl, cls.name, line))
            elif item[0] in ('static_init', 'inst_init'):
                _, inner, bline = item
                block = _body_lines(src, bline, bline + inner.count('\n'))
                (cls.static_inits if item[0] == 'static_init' else cls.inst_inits).append(block)
            else:
                _, header, inner, hline, bline = item
                meth = _parse_signature(header, cls.name)
                meth.line = hline
                meth.body = _body_lines(src, bline, bline + inner.count('\n'))
                cls.methods.append(meth)


def parse_file(path):
    with open(path, 'r', encoding='utf-8', errors='replace') as f:
        src = f.read()
    return parse_source(src, source_path=path)
