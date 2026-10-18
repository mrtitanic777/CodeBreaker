# -*- coding: utf-8 -*-
"""
statements/controlflow.py -- control-flow lowering that has no C++ equivalent:
  * labeled break/continue -> goto + emitted target labels (_translate_labels)
  * each switch case/default body wrapped in `{ }` so a decl cannot cross a case
    label (C2360/C2361), fallthrough + break preserved (_wrap_switch_cases)
  * a `finally` body run before every return/break/continue that leaves its `try` (_inline_finally)
Leaf module: only `re` + its own regexes; imported by the statements package.
"""

import re


_LABEL_DEF = re.compile(r'^(\w+):\s*(.*)$')
_LABEL_JUMP = re.compile(r'\b(break|continue)\s+(\w+)\s*;')
_LOOP_START = re.compile(r'^(for|while|do)\b')


def _loop_close(lines, loop_idx):
    """Index of the line holding the `}` that closes the loop starting at loop_idx."""
    depth, started = 0, False
    for i in range(loop_idx, len(lines)):
        depth += lines[i].count('{') - lines[i].count('}')
        if '{' in lines[i]:
            started = True
        if started and depth <= 0:
            return i
    return None


_CASE_LABEL = re.compile(r'^(case\b.*?|default)\s*:(.*)$')


def _scan_braces(text, stack, pending_switch):
    """Walk `text` (skipping string/char literals and comments), pushing/popping
    brace frames on `stack`. A frame is [is_switch, case_open]. `switch (...)`
    marks the NEXT `{` as a switch body. Returns the carried pending_switch flag."""
    i, n = 0, len(text)
    while i < n:
        c = text[i]
        if c == '"' or c == "'":
            q = c
            i += 1
            while i < n:
                if text[i] == '\\':
                    i += 2
                    continue
                if text[i] == q:
                    i += 1
                    break
                i += 1
            continue
        if c == '/' and i + 1 < n and text[i + 1] == '/':
            break                                  # line comment: nothing left to scan
        if c == '/' and i + 1 < n and text[i + 1] == '*':
            j = text.find('*/', i + 2)
            i = n if j == -1 else j + 2
            continue
        if c == '{':
            stack.append([pending_switch, False])
            pending_switch = False
            i += 1
            continue
        if c == '}':
            if stack:
                stack.pop()
            i += 1
            continue
        if (c == 's' and text.startswith('switch', i)
                and (i == 0 or not (text[i - 1].isalnum() or text[i - 1] == '_'))):
            k = i + 6
            while k < n and text[k] in ' \t':
                k += 1
            if k < n and text[k] == '(':
                pending_switch = True
            i += 6
            continue
        i += 1
    return pending_switch


def _wrap_switch_cases(lines):
    """Wrap each switch case/default body in a `{ }` block so a local declared in one
    case cannot cross the next case label (C2360/C2361 -- initialization skipped by a
    case label). Fallthrough and break are preserved: the braces are pure scope, so an
    unbroken case still falls into the next one. Nested `{}` (guards, ifs, inner
    switches) are tracked so only labels/closing-brace of the CURRENT switch fire."""
    out = []
    stack = []                                     # frames: [is_switch, case_open]
    pending_switch = False
    for line in lines:
        stripped = line.lstrip()
        indent = line[:len(line) - len(stripped)]
        # A case/default label whose innermost open block is a switch body.
        if stack and stack[-1][0]:
            m = _CASE_LABEL.match(stripped)
            if m:
                if stack[-1][1]:                   # close the previous case's block
                    out.append(indent + '}')
                out.append(indent + m.group(1) + ': {')
                stack[-1][1] = True
                rest = m.group(2).strip()
                if rest:                           # rare: `case X: stmt;` on one line
                    out.append(indent + rest)
                    pending_switch = _scan_braces(rest, stack, pending_switch)
                continue
        # A `}` that closes the switch body must close the last open case first.
        if stripped[:1] == '}' and stack[-1:] and stack[-1][0] and stack[-1][1]:
            out.append(indent + '}')
            stack[-1][1] = False
        out.append(line)
        pending_switch = _scan_braces(line, stack, pending_switch)
    return out


def _translate_labels(lines):
    """Labeled break/continue -> goto. `break L` jumps past the L-loop; `continue L`
    jumps to a target at the END of the L-loop body, so a `for` runs its increment
    on fall-through (no skipped-increment infinite loop -- the shore-water freeze)."""
    # Track break- vs continue-references SEPARATELY so we emit ONLY the target(s) that are
    # actually jumped to. Emitting an unused `L_cont:`/`L_brk:` triggers C4102 (unreferenced label).
    cont_refs, brk_refs = set(), set()
    for ln in lines:
        for m in _LABEL_JUMP.finditer(ln):
            (cont_refs if m.group(1) == 'continue' else brk_refs).add(m.group(2))
    referenced = cont_refs | brk_refs
    if not referenced:
        return lines

    def_strip, cont_at, brk_at = {}, {}, {}
    for i, ln in enumerate(lines):
        m = _LABEL_DEF.match(ln)
        if not m or m.group(1) not in referenced:
            continue
        name, rest = m.group(1), m.group(2)
        # The labeled construct's body is either a LOOP (for/while/do) or a plain labeled BLOCK
        # (`label: { ... break label; }` -- a Java labeled statement, e.g. Indev's Frustrum). Find where
        # the body opens; a block only supports `break label` (`continue label` needs a loop).
        if _LOOP_START.match(rest):
            body_idx, is_loop = i, True
        elif rest.startswith('{'):
            body_idx, is_loop = i, False               # labeled block, brace on the label line
        elif i + 1 < len(lines) and _LOOP_START.match(lines[i + 1]):
            body_idx, is_loop = i + 1, True
        elif i + 1 < len(lines) and lines[i + 1].lstrip().startswith('{'):
            body_idx, is_loop = i + 1, False           # labeled block, brace on the next line
        else:
            continue
        close = _loop_close(lines, body_idx)
        if close is None:
            continue
        def_strip[i] = name
        if is_loop and name in cont_refs:              # `continue L` is only legal on a loop
            cont_at.setdefault(close, []).append('%s_cont:;' % name)
        if name in brk_refs:
            brk_at.setdefault(close, []).append('%s_brk:;' % name)
    if not def_strip:
        return lines

    def jump(m):
        return 'goto %s_%s;' % (m.group(2), 'brk' if m.group(1) == 'break' else 'cont')
    rewritten = [_LABEL_JUMP.sub(jump, ln) for ln in lines]

    out = []
    for i, ln in enumerate(rewritten):
        if i in def_strip:
            ln = re.sub(r'^\s*%s:\s*' % re.escape(def_strip[i]), '', ln)
            if not ln.strip():
                continue
        for c in cont_at.get(i, []):           # continue-target: end of loop body (before `}`)
            out.append(c)
        out.append(ln)
        for b in brk_at.get(i, []):            # break-target: just after the loop
            out.append(b)
    return out


# ---------------------------------------------------------------------------------------------------
# try/finally. C++ has no `finally`: the line translator turns `finally {` into `catch (...) {} {`, a
# block after the try, which runs when the try body FALLS THROUGH. FIXED BUG finally-skipped-on-return:
# a return/break/continue leaving the try skipped it -- World.updatingLighting()'s
# `try { ... return ...; } finally { --lightingUpdateDepth; }` never decremented, so after 50 calls
# lighting stopped updating for good (MSVC's only warning in the build, C4715, pointed at it). Here,
# on the Java lines before translation, each such jump gets the finally body in front of it:
#     return EXPR;  ->  { T j2xRetN = EXPR;  <finally body>  return j2xRetN; }
#     return; / break; / continue;  ->  { <finally body>  return; }
# Innermost try first, so nested finallys run inside-out. Left alone (and still as before): a jump
# inside the finally body itself, inside an anonymous class body, a break/continue whose loop or switch
# is inside the try, and a return split over several lines.

_FINALLY_LINE = re.compile(r'^\s*\}\s*finally\s*\{\s*$')
_TRY_LINE = re.compile(r'^\s*try\s*\{\s*$')
_RETURN_LINE = re.compile(r'^(\s*)return\b\s*(.*?)\s*;\s*$')
_JUMP_LINE = re.compile(r'^(\s*)(break|continue)\s*(\w*)\s*;\s*$')
_BREAKABLE = re.compile(r'^\s*(?:(\w+)\s*:\s*)?(for|while|do|switch)\b')
_ANON_OPEN = re.compile(r'\bnew\s+[\w.<>]+\s*\([^;]*\)\s*\{\s*$')
_DONE = '  /*j2x-finally*/'


def _code(line):
    """The line without string/char literals and // comments (for counting braces)."""
    out, i, n = [], 0, len(line)
    while i < n:
        c = line[i]
        if c in '"\'':
            j = i + 1
            while j < n and line[j] != c:
                j += 2 if line[j] == '\\' else 1
            i = j + 1
            continue
        if line.startswith('//', i):
            break
        out.append(c)
        i += 1
    return ''.join(out)


def _depths(lines):
    """Brace depth before each line."""
    out, d = [], 0
    for ln in lines:
        out.append(d)
        c = _code(ln)
        d += c.count('{') - c.count('}')
    return out


def _close_of(lines, depths, i):
    """Index of the line whose `}` closes the block opened on line i."""
    for k in range(i + 1, len(lines)):
        if depths[k] <= depths[i] and _code(lines[k]).lstrip().startswith('}'):
            return k
    return None


def _inline_finally(lines, return_type):
    """Run each `finally` body before the jumps that leave its try (see above). `return_type`: the
    method's Java return type (an ir.JType, or None / void)."""
    lines = list(lines)
    rtype = None if (return_type is None or repr(return_type) == 'void') else repr(return_type)
    counter = [0]
    while True:
        depths = _depths(lines)
        todo = [k for k, ln in enumerate(lines) if _FINALLY_LINE.match(ln) and not ln.endswith(_DONE)]
        if not todo:
            break
        f = max(todo, key=lambda k: (depths[k], k))                # innermost (deepest) first
        level = depths[f] - 1                                      # the depth the `try {` line sits at
        t = next((k for k in range(f - 1, -1, -1) if depths[k] == level and _TRY_LINE.match(lines[k])), None)
        end = _close_of(lines, depths, f)
        lines[f] = lines[f] + _DONE
        if t is None or end is None:
            continue
        body = [ln for ln in lines[f + 1:end]]
        if any(_RETURN_LINE.match(ln) or _JUMP_LINE.match(ln) for ln in body):
            continue                                               # a jump in the finally itself: leave it
        new, inner = lines[:t + 1], []                             # inner: blocks opened inside the try
        for k in range(t + 1, f):
            ln = lines[k]
            while inner and depths[k] <= inner[-1][1]:
                inner.pop()
            in_anon = any(kind == 'anon' for kind, _d, _l in inner)
            rm, jm = _RETURN_LINE.match(ln), _JUMP_LINE.match(ln)
            exits = False
            if rm and not in_anon:
                exits = True
            elif jm and not in_anon:
                word, label = jm.group(2), jm.group(3)
                loops = [l for kind, _d, l in inner if kind == 'loop' and (word == 'break' or l[1] != 'switch')]
                exits = not (any(l[0] == label for l in loops) if label else bool(loops))
            if exits:
                ind = (rm or jm).group(1)
                if rm and rm.group(2):
                    if rtype is None:                              # `return x;` in a void method: not Java
                        new.append(ln)
                        continue
                    counter[0] += 1
                    var = 'j2xRet%d' % counter[0]
                    new += [ind + '{', ind + '%s %s = %s;' % (rtype, var, rm.group(2))]
                    new += body
                    new += [ind + 'return %s;' % var, ind + '}']
                else:
                    new += [ind + '{'] + body + [ln.rstrip(), ind + '}']
                continue
            new.append(ln)
            code = _code(ln)
            if code.count('{') > code.count('}'):
                bm = _BREAKABLE.match(ln)
                if _ANON_OPEN.search(code):
                    inner.append(('anon', depths[k], None))
                elif bm:
                    inner.append(('loop', depths[k], (bm.group(1) or '', bm.group(2))))
                else:
                    inner.append(('block', depths[k], None))
        lines = new + lines[f:]
    return [ln[:-len(_DONE)] if ln.endswith(_DONE) else ln for ln in lines]

