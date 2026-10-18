# -*- coding: utf-8 -*-
"""
statements/controlflow.py -- control-flow lowering that has no C++ equivalent:
  * labeled break/continue -> goto + emitted target labels (_translate_labels)
  * each switch case/default body wrapped in `{ }` so a decl cannot cross a case
    label (C2360/C2361), fallthrough + break preserved (_wrap_switch_cases)
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
