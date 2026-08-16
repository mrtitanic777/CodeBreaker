# -*- coding: utf-8 -*-
r"""
ui_mark.py -- tag every UI function with a greppable // J2X-UI[Class::method] marker + catalogue.

After a UI merge (a new UI layered over the converted menus), you need to find EVERY UI function fast
-- which screens/menus/buttons exist, where each method lives, what got layered where. This tool scans
the converted C++, identifies the UI classes (Gui*/*Screen/*Menu + anything extending a GUI base), and
inserts `// J2X-UI[Class::method]` at each UI method definition, so `grep -rn "J2X-UI" .` enumerates
the entire UI surface. It also writes UI_INDEX.md: every UI class -> its methods with file:line.

Idempotent: a definition already carrying a J2X-UI marker is left alone, so you can re-run after each
merge without doubling markers. `--dry-run` reports what it WOULD mark without editing.
"""

import os
import re


# UI base classes: a class extending one of these is a UI class even if its name doesn't match.
UI_BASES = {'Gui', 'GuiScreen', 'GuiButton', 'GuiContainer', 'GuiSlot', 'GuiTextField',
            'GuiSmallButton', 'GuiHandler', 'Screen', 'GuiComponent'}


def is_ui_class_name(name):
    """Name-based heuristic: Minecraft UI classes are Gui* or end in Screen/Menu."""
    return name.startswith('Gui') or name.endswith('Screen') or name.endswith('Menu')


_CLASS_DECL = re.compile(r'\bclass\s+([A-Za-z_]\w*)\s*(?::\s*([^\{]+))?\{', re.DOTALL)
_EXTENDS = re.compile(r'\b(?:public|protected|private)?\s*([A-Za-z_]\w*)')


def discover_ui_classes(src_dir, extra=None):
    """Return the set of UI class names: name-pattern matches PLUS classes that extend a UI base
    (transitively within this tree). Scans .h files for `class X : public Base`."""
    ui = set(extra or ())
    bases_by_class = {}
    names = set()
    for dp, _dirs, files in os.walk(src_dir):
        for f in files:
            if not (f.endswith('.h') or f.endswith('.hpp')):
                continue
            try:
                text = open(os.path.join(dp, f), 'r', encoding='utf-8', errors='replace').read()
            except (IOError, OSError):
                continue
            for m in _CLASS_DECL.finditer(text):
                cls, bases = m.group(1), m.group(2) or ''
                names.add(cls)
                bases_by_class[cls] = set(_EXTENDS.findall(bases)) - {'public', 'protected', 'private'}
    # seed from name pattern
    for cls in names:
        if is_ui_class_name(cls):
            ui.add(cls)
    # transitive closure over inheritance into a UI base or an already-UI class
    changed = True
    while changed:
        changed = False
        for cls, bases in bases_by_class.items():
            if cls in ui:
                continue
            if bases & (UI_BASES | ui):
                ui.add(cls)
                changed = True
    return ui


# a method DEFINITION line: `RetType Class::method(...)` (not a call: `Class::` sits after a return
# type / at statement start, and the line is not a `;`-terminated prototype).
def _def_regex(cls):
    return re.compile(r'(^|[^\w:])%s::(~?[A-Za-z_]\w*)\s*\(' % re.escape(cls))


def mark(src_dir, dry_run=False, extra_classes=None):
    """Insert `// J2X-UI[Class::method]` at each UI method definition. Returns
    {'classes': set, 'marked': {class: [(file,line,method)]}, 'skipped': int}."""
    ui = discover_ui_classes(src_dir, extra=extra_classes)
    regexes = [(c, _def_regex(c)) for c in sorted(ui)]
    marked, skipped = {}, 0
    for dp, _dirs, files in os.walk(src_dir):
        for f in sorted(files):
            if not f.endswith('.cpp'):
                continue
            path = os.path.join(dp, f)
            try:
                lines = open(path, 'r', encoding='utf-8', errors='replace').read().split('\n')
            except (IOError, OSError):
                continue
            changed = False
            for i, line in enumerate(lines):
                stripped = line.strip()
                # only definition-ish lines: contains '::method(' and opens a body (has '{' now or is
                # a signature continued onto the next line) -- skip pure prototypes/calls.
                if '::' not in line or stripped.endswith(';'):
                    continue
                for cls, rx in regexes:
                    m = rx.search(line)
                    if not m:
                        continue
                    method = m.group(2)
                    # must look like a definition: this line or a later line has '{' before any ';'
                    tail = '\n'.join(lines[i:i + 4])
                    brace = tail.find('{')
                    semi = tail.find(';')
                    if brace == -1 or (semi != -1 and semi < brace):
                        continue
                    if 'J2X-UI[' in line:
                        skipped += 1
                        marked.setdefault(cls, []).append((f, i + 1, method))
                        break
                    if not dry_run:
                        lines[i] = line.rstrip() + '  // J2X-UI[%s::%s]' % (cls, method)
                    marked.setdefault(cls, []).append((f, i + 1, method))
                    changed = True
                    break
            if changed and not dry_run:
                with open(path, 'w', encoding='utf-8') as out:
                    out.write('\n'.join(lines))
    return {'classes': ui, 'marked': marked, 'skipped': skipped}


def render_index(result):
    marked = result['marked']
    total = sum(len(v) for v in marked.values())
    L = ['# UI_INDEX.md -- catalogue of every UI function', '',
         'Every UI method carries a greppable `// J2X-UI[Class::method]` marker; '
         '`grep -rn "J2X-UI" .` lists them all. Regenerate after each UI merge.', '',
         '- UI classes: **%d**   UI methods marked: **%d**' % (len(result['classes']), total), '']
    for cls in sorted(marked):
        L.append('## %s (%d)' % (cls, len(marked[cls])))
        L.append('')
        L.append('| Method | Location |')
        L.append('|---|---|')
        for fname, line, method in sorted(marked[cls]):
            L.append('| `%s::%s` | `%s:%d` |' % (cls, method, fname, line))
        L.append('')
    unmarked = sorted(result['classes'] - set(marked))
    if unmarked:
        L += ['## UI classes with no marked methods (header-only or all inline)', '',
              ', '.join('`%s`' % c for c in unmarked), '']
    return '\n'.join(L) + '\n'


def run(src_dir, dry_run=False, extra_classes=None, index_dir=None):
    """Mark the tree (unless dry_run) and write UI_INDEX.md. Returns the result dict + index path."""
    result = mark(src_dir, dry_run=dry_run, extra_classes=extra_classes)
    index_path = None
    if not dry_run:
        index_path = os.path.join(index_dir or src_dir, 'UI_INDEX.md')
        with open(index_path, 'w', encoding='utf-8') as f:
            f.write(render_index(result))
    result['index'] = index_path
    return result
