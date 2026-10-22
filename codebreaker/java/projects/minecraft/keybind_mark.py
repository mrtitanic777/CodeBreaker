# -*- coding: utf-8 -*-
r"""
keybind_mark.py -- find every HARDCODED keyboard key-press handler in the converted C++ and tag it with a
greppable `// J2X-KEYBIND[KEY -> action]` marker + write KEYBIND_INDEX.md.

Minecraft drives a lot of actions off physical keyboard keys (F1 hide-GUI, F2 screenshot, F3 debug overlay,
F5 third-person, F8 cinematic camera, F11 fullscreen, ESC pause, number keys = hotbar, shift = sneak...).
There is NO keyboard on the console, so every `Keyboard::getEventKey() == KEY_x` / `Keyboard::isKeyDown(
KEY_x)` site is DEAD -- the action is unreachable until it is re-routed. This tool catalogues them all so
each can be deliberately turned into EITHER a controller button press OR a menu (GameSettings) toggle -- the
user's choice per key -- instead of hand-wiring them one at a time (e.g. the Show-FPS -> debug-panel merge).

It does NOT rewrite the branch logic (the route decision is deferred): it only annotates + indexes, exactly
like ui_mark. Idempotent -- a line already carrying a J2X-KEYBIND marker is left alone. Runs automatically at
the end of every xbox360 conversion, and is also exposed as `codebreaker java keybind-mark`.
"""

import os
import re

# Hardcoded, non-rebindable keys and what they do in Beta. (The rebindable movement/inventory/hotbar binds
# go through GameSettings.keyBindings + the controller config already; these function keys do not.)
KEY_ACTIONS = {
    'F1': 'hide GUI / HUD',
    'F2': 'take screenshot',
    'F3': 'toggle debug overlay (F3 panel)',
    'F5': 'toggle third-person view',
    'F7': 'debug / profiler timing',
    'F8': 'toggle smooth (cinematic) camera',
    'F11': 'toggle fullscreen',
    'ESCAPE': 'pause / open in-game menu',
    'S': 'reload chunks (with F3 held)',
    'LSHIFT': 'sneak', 'RSHIFT': 'sneak',
    'LCONTROL': 'sprint', 'RCONTROL': 'sprint',
    'TAB': 'player list',
    '1': 'hotbar slot 1', '2': 'hotbar slot 2', '3': 'hotbar slot 3', '4': 'hotbar slot 4',
    '5': 'hotbar slot 5', '6': 'hotbar slot 6', '7': 'hotbar slot 7', '8': 'hotbar slot 8',
    '9': 'hotbar slot 9',
}

# `Keyboard::getEventKey() == Keyboard::KEY_x`  OR  `Keyboard::isKeyDown(Keyboard::KEY_x)`
_KEY_SITE = re.compile(
    r'Keyboard::(?:getEventKey\s*\(\s*\)\s*==\s*Keyboard::KEY_(\w+)|isKeyDown\s*\(\s*Keyboard::KEY_(\w+))')
_MARKER = 'J2X-KEYBIND['


def _keys_on_line(line):
    """Every KEY_x referenced on a line (a line may test two, e.g. F3+S)."""
    keys = []
    for m in _KEY_SITE.finditer(line):
        keys.append(m.group(1) or m.group(2))
    return keys


def _action(key):
    return KEY_ACTIONS.get(key, '(unknown action -- inspect the branch)')


def mark(src_dir, dry_run=False):
    """Append `// J2X-KEYBIND[KEY_x -> action | ROUTE: controller button OR menu toggle]` to each keyboard
    key-press line. Returns {'sites': [(file,line,keys,action)], 'skipped': int}."""
    sites, skipped = [], 0
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
                keys = _keys_on_line(line)
                if not keys:
                    continue
                label = ' '.join('KEY_%s=%s' % (k, _action(k)) for k in keys)
                sites.append((f, i + 1, keys, label))
                if _MARKER in line:
                    skipped += 1
                    continue
                if not dry_run:
                    lines[i] = (line.rstrip() +
                                '   // J2X-KEYBIND[%s | keyboard is DEAD on console -- ROUTE TO: controller'
                                ' button OR menu toggle]' % label)
                    changed = True
            if changed and not dry_run:
                with open(path, 'w', encoding='utf-8') as out:
                    out.write('\n'.join(lines))
    return {'sites': sites, 'skipped': skipped}


def render_index(result):
    sites = result['sites']
    # unique keys across all sites
    keys = []
    for _f, _ln, ks, _lab in sites:
        for k in ks:
            if k not in keys:
                keys.append(k)
    L = ['# KEYBIND_INDEX.md -- every hardcoded keyboard action (route to controller OR menu)', '',
         'Minecraft binds these actions to physical keys. The console has NO keyboard, so each site is DEAD '
         'until re-routed. Every site carries a greppable `// J2X-KEYBIND[...]` marker '
         '(`grep -rn "J2X-KEYBIND" .`). Decide, per key, whether it becomes a **controller button** or a '
         '**menu toggle** (a GameSettings option, like Show FPS -> the debug panel), then wire it and clear '
         'the marker.', '',
         '- key-press sites: **%d**   distinct keys: **%d**' % (len(sites), len(keys)), '',
         '| Key | Action | Site | Route decision |', '|---|---|---|---|']
    for f, ln, ks, _lab in sorted(sites, key=lambda s: (s[0], s[1])):
        keydesc = ', '.join('KEY_%s' % k for k in ks)
        action = ' + '.join(_action(k) for k in ks)
        L.append('| `%s` | %s | `%s:%d` | _controller button / menu toggle_ |' % (keydesc, action, f, ln))
    L.append('')
    return '\n'.join(L) + '\n'


def run(src_dir, dry_run=False, index_dir=None):
    """Mark the tree (unless dry_run) and write KEYBIND_INDEX.md. Returns the result dict + index path."""
    result = mark(src_dir, dry_run=dry_run)
    index_path = None
    if not dry_run:
        index_path = os.path.join(index_dir or src_dir, 'KEYBIND_INDEX.md')
        with open(index_path, 'w', encoding='utf-8') as f:
            f.write(render_index(result))
    result['index'] = index_path
    return result
