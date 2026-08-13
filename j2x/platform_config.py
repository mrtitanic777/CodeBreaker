# -*- coding: utf-8 -*-
r"""
platform_config.py -- the user-DECISION layer of the converter.

Some platform questions have no single right answer -- they are the user's call. This module turns
those into an explicit, prefilled MENU the user picks from, then EMITS the chosen implementation into
the port instead of a blind stub:

  1. Controller mapping   -- which 360 buttons/sticks drive the mouse-expected PC controls, plus a
                             stick-layout preset (Default / Southpaw / Legacy / Legacy-Southpaw),
                             Y-invert, sensitivity, deadzones. -> ControllerConfig.h (the Input layer
                             reads it).
  2. Keyboard / text input -- HOW the game gets typed text (world names, chat, server address): the
                             system virtual keyboard `XShowKeyboardUI`, an in-game on-screen keyboard,
                             or a fixed-string stub. -> Kernel_ShowKeyboard() in Kernel.cpp.
  3. Save / load storage   -- WHERE worlds persist + device selection: STFS content container
                             (`XShowDeviceSelectorUI` + `XContentCreate`), a raw dev HDD path, or
                             memory-only. -> Kernel_SelectDevice()/Kernel_OpenSave() in Kernel.cpp.

The choices live in `platform.config.json` (defaults written on first run); re-emit after editing. The
emitted code carries `// J2X-STUB[...]` markers wherever a choice degrades to a no-op, so the master
audit (`grep -rn J2X-STUB`) still finds it. A `PLATFORM_CHOICES.md` shows the full menu with the
current pick marked, so every option is visible and switchable in one place.
"""

import json
import os

from . import sdk

# Which chosen medium needs which 360 kernel symbols -- so we can (a) gate on the SDK being present
# and (b) verify each symbol actually exists in the user's installed SDK before emitting a call to it.
_MEDIUM_SYMBOLS = {
    'xshowkeyboard': ['XShowKeyboardUI'],
    'osk': [],
    'stub': [],
    'stfs': ['XShowDeviceSelectorUI', 'XContentCreate', 'XContentClose'],
    'rawhdd': [],
    'memory': [],
}


# ---------------------------------------------------------------------------
# 1. CONTROLLER MAPPING
# ---------------------------------------------------------------------------
# The mouse/keyboard controls the PC game expects, each needing a 360 binding.
CONTROLLER_ACTIONS = [
    ('look',       'Mouse look (camera)',        'RSTICK'),
    ('move',       'Move (WASD)',                'LSTICK'),
    ('attack',     'Attack / break (left mouse)', 'RTRIGGER'),
    ('use',        'Use / place (right mouse)',  'LTRIGGER'),
    ('jump',       'Jump (space)',               'A'),
    ('sneak',      'Sneak (shift)',              'LSTICK_CLICK'),
    ('inventory',  'Inventory (E)',              'Y'),
    ('drop',       'Drop item (Q)',              'B'),
    ('hotbar_prev','Hotbar left (wheel up)',     'LBUMPER'),
    ('hotbar_next','Hotbar right (wheel down)',  'RBUMPER'),
    ('pause',      'Pause / menu (esc)',         'START'),
    ('chat',       'Chat / command (T)',         'DPAD_UP'),
    ('perspective','Toggle view (F5)',           'DPAD_DOWN'),
    ('sprint',     'Sprint',                     'RSTICK_CLICK'),
    ('gui_cursor', 'Virtual cursor in menus',    'LSTICK'),
]

# The 360 controls a binding may target.
CONTROLLER_BUTTONS = ['A', 'B', 'X', 'Y', 'LBUMPER', 'RBUMPER', 'LTRIGGER', 'RTRIGGER',
                      'LSTICK', 'RSTICK', 'LSTICK_CLICK', 'RSTICK_CLICK',
                      'DPAD_UP', 'DPAD_DOWN', 'DPAD_LEFT', 'DPAD_RIGHT', 'START', 'BACK']

# Stick-layout presets: which physical stick does move vs look, and whether attack/place sit on the
# triggers (default) or the bumpers (legacy). Each preset overrides the relevant action binds.
CONTROLLER_LAYOUTS = {
    'default':         {'move': 'LSTICK', 'look': 'RSTICK', 'attack': 'RTRIGGER', 'use': 'LTRIGGER'},
    'southpaw':        {'move': 'RSTICK', 'look': 'LSTICK', 'attack': 'RTRIGGER', 'use': 'LTRIGGER'},
    'legacy':          {'move': 'LSTICK', 'look': 'RSTICK', 'attack': 'RBUMPER',  'use': 'LBUMPER'},
    'legacy_southpaw': {'move': 'RSTICK', 'look': 'LSTICK', 'attack': 'RBUMPER',  'use': 'LBUMPER'},
}


# ---------------------------------------------------------------------------
# 2. KEYBOARD / TEXT-INPUT medium
# ---------------------------------------------------------------------------
KEYBOARD_OPTIONS = {
    'xshowkeyboard': {
        'label': 'System virtual keyboard (XShowKeyboardUI) -- RECOMMENDED',
        'desc':  'The platform\'s built-in on-screen keyboard. Blocks while open; returns the typed '
                 'string. The standard way any console app takes text input.',
    },
    'osk': {
        'label': 'In-game on-screen keyboard',
        'desc':  'A custom controller-driven keyboard you render yourself. More work, fully styled, '
                 'no system overlay.',
    },
    'stub': {
        'label': 'Stub (fixed strings, no text entry)',
        'desc':  'Text fields return a fixed placeholder. Boots, but the user cannot type. Bring-up '
                 'only.',
    },
}

# ---------------------------------------------------------------------------
# 3. SAVE / LOAD storage medium
# ---------------------------------------------------------------------------
STORAGE_OPTIONS = {
    'stfs': {
        'label': 'STFS content container (XShowDeviceSelectorUI + XContentCreate) -- RECOMMENDED',
        'desc':  'Persistent per-title saves. The device selector picks HDD/USB once; XContentCreate '
                 'mounts a writable "Save:" root the save files live under. Survives reboots, passes '
                 'cert.',
    },
    'rawhdd': {
        'label': 'Raw HDD dev path (game:\\Save\\)',
        'desc':  'Writes straight to a dev-kit path. Simple, persists on devkit, but NOT a content '
                 'container (no device select, will not pass retail cert).',
    },
    'memory': {
        'label': 'Memory-only (no persistence)',
        'desc':  'Save data lives in RAM and is lost on exit. Boots and plays, but nothing persists.',
    },
}


# ---------------------------------------------------------------------------
# 4. OTHER SUBSYSTEM decisions (audio / video / networking / memory / user / debug)
# ---------------------------------------------------------------------------
# Each is an either/or the user picks. Mirrors the option menus in platform_coverage.SUBSYSTEMS,
# surfaced as pick-one choices the GUI renders as button groups and the emitter records.
SUBSYSTEM_OPTIONS = {
    'audio': {
        'default': 'xact3',
        'options': {
            'xact3':   {'label': 'XACT3 (wave/sound banks)', 'desc': 'Authored wave/sound banks, cue-driven. A bank-authoring pipeline instead of hand-written mixing.'},
            'xaudio2': {'label': 'XAudio2 (source voices)',  'desc': 'Low-level source voices; you own mixing, streaming, and format decode. More code, no authoring pipeline.'},
            'silent':  {'label': 'Silent (no audio)',        'desc': 'No sound output. Boots quietly — bring-up only.'},
        },
    },
    'video': {
        'default': 'gl_emulator',
        'options': {
            'gl_emulator': {'label': 'GL→D3D9 emulation layer', 'desc': 'Map each OpenGL call onto Direct3D 9 through a translation layer — keeps the existing render code.'},
            'd3d9_fresh':  {'label': 'Native Direct3D 9 backend', 'desc': 'Rewrite the renderer directly against D3D9. Most control, most work.'},
            'stub':        {'label': 'Stub (renders nothing)',   'desc': 'No-op graphics — the game runs but draws nothing. Bring-up only.'},
        },
    },
    'network': {
        'default': 'sockets',
        'options': {
            'sockets': {'label': 'Sockets (networked multiplayer)', 'desc': 'BSD-style sockets over the console network stack. Off-subnet play may need a relay.'},
            'offline': {'label': 'Offline (single-player only)',    'desc': 'No networking — single-player only.'},
        },
    },
    'memory': {
        'default': 'real_budget',
        'options': {
            'real_budget': {'label': 'Real console budget', 'desc': 'Report the platform\'s true free/total memory (via the console memory API).'},
            'fixed':       {'label': 'Fixed constants',      'desc': 'Free/total memory return a nominal number — fine unless game logic branches on it.'},
        },
    },
    'user': {
        'default': 'profile',
        'options': {
            'profile': {'label': 'Signed-in profile name', 'desc': 'Read the console user/profile name for the on-screen identity.'},
            'stub':    {'label': 'Fixed name',              'desc': 'Hardcode a placeholder identity.'},
        },
    },
    'debug': {
        'default': 'on',
        'options': {
            'on':  {'label': 'On (emit debug file)',   'desc': 'Emit the single toggleable debug file (master switch, greppable markers, rate-limited output).'},
            'off': {'label': 'Off (no debug support)', 'desc': 'Do not emit any debug facility.'},
        },
    },
    'achievements': {
        'default': 'online',
        'options': {
            'online': {'label': 'Online profile achievements', 'desc': 'Real achievements written to the signed-in profile over the online service. Needs a Title ID + network (queues offline).'},
            'local':  {'label': 'Local-only (no online write)', 'desc': 'Track unlocks locally; nothing is written to the online profile. Bring-up / no-LIVE builds.'},
            'off':    {'label': 'No achievements',              'desc': 'Skip achievement handling entirely.'},
        },
    },
}


def default_config():
    binds = dict((a[0], a[2]) for a in CONTROLLER_ACTIONS)
    return {
        'controller': {
            'layout': 'default',
            'invert_y': False,
            'look_sensitivity': 1.0,
            'stick_deadzone': 0.24,
            'trigger_threshold': 0.30,
            'binds': binds,
        },
        'keyboard': 'xshowkeyboard',
        'storage': 'stfs',
        'subsystems': dict((k, v['default']) for k, v in SUBSYSTEM_OPTIONS.items()),
    }


# The full, ordered list of decisions the UI renders as button groups. Each: id, label, an ordered
# list of (key, label, desc) options, and where the value lives in the config (a dotted path).
# The QUESTION each decision poses to the user + WHY it must be answered a certain way. Each decision
# is a dict: id, label, path (where the value lives in cfg), question, why (the stakes/guidance),
# recommend (the key we suggest + a one-line reason), and options [(key, label, consequence)].
# NOTE: keep all text UNIVERSAL — describe the platform's general trade-offs, not any specific game,
# port, or this tool's internals. Other people port other Java programs with this; the questions must
# read the same for all of them.
_QUESTION = {
    'controller_layout': (
        'Which stick layout should the gamepad default to?',
        'A console gamepad has no mouse — the sticks are the camera and movement. This sets where each '
        'lives; an unfamiliar default feels backwards. Users can still remap in-game.',
        ('default', 'most players expect movement on the left stick and camera on the right')),
    'keyboard': (
        'How does the user enter text (names, chat, addresses)?',
        'A console has no attached keyboard by default. Any text field needs an input method, or the '
        'user is stuck at that prompt — required whenever the program asks for typed input.',
        ('xshowkeyboard', 'the built-in system keyboard is familiar and needs no custom UI')),
    'storage': (
        'Where does save data live, and must it survive a reboot?',
        'This decides whether saves persist across sessions and whether the build can pass the '
        'platform\'s certification. The wrong choice loses data on exit or fails cert.',
        ('stfs', 'a proper content container persists across reboots and passes certification')),
    'audio': (
        'Which audio backend plays music and sound effects?',
        'The platform offers a high-level banked audio system and a low-level voice API. The banked '
        'system provides an authoring pipeline; the low-level one makes you handle mixing and decode.',
        ('xact3', 'the banked system saves you writing mixing, streaming, and format decode by hand')),
    'video': (
        'How is the program\'s OpenGL rendering realized on this platform?',
        'The console has no OpenGL — every GL call must reach the native graphics API (Direct3D 9). '
        'A translation layer keeps the existing render code; a native rewrite is far more work.',
        ('gl_emulator', 'translating GL→D3D9 reuses the existing render code instead of a full rewrite')),
    'network': (
        'Does the program need networked multiplayer — and how?',
        'This gates whether the networking code is live. Sockets enable networked play; offline builds '
        'skip it entirely. Play across different subnets may need a relay you provide.',
        ('sockets', 'sockets keep multiplayer working; offline disables it completely')),
    'memory': (
        'How should the program report how much memory is free?',
        'Some program logic branches on available memory. Reporting the platform\'s true budget keeps '
        'that behavior honest; a fixed number is only safe if nothing reads it.',
        ('real_budget', 'report the true budget unless you know no code depends on the value')),
    'user': (
        'How is the user\'s identity / profile obtained?',
        'Used for the on-screen name and any per-user data. The real name comes from the console '
        'profile API; the stub hardcodes a placeholder.',
        ('profile', 'reading the signed-in profile makes the on-screen name correct')),
    'debug': (
        'Include the debug instrumentation in this build?',
        'Emits one toggleable debug file (a single master switch, greppable markers, rate-limited '
        'output). Keep it on while bringing the port up; turn it off for a clean release build.',
        ('on', 'keep it on during bring-up so crashes and leaks are traceable')),
    'achievements': (
        'How are the program\'s achievements delivered?',
        'A console can\'t invent achievements at runtime — each must be declared in the title config '
        'and unlocked by a networked write to the signed-in profile (needs a Title ID, a profile, and '
        'network; offline it queues). Local-only skips the online write.',
        ('online', 'online writes give real profile achievements; needs a Title ID + signed-in user')),
}

_SUB_LABELS = {'audio': 'Audio', 'video': 'Video', 'network': 'Networking',
               'memory': 'Memory', 'user': 'User / profile', 'debug': 'Debug support',
               'achievements': 'Achievements'}


def _decisions():
    def entry(did, label, path, opts):
        q, why, rec = _QUESTION[did]
        return {'id': did, 'label': label, 'path': path, 'question': q, 'why': why,
                'recommend': rec, 'options': opts}
    D = [
        entry('controller_layout', 'Controller layout', 'controller.layout',
              [(k, {'default': 'Default', 'southpaw': 'Southpaw', 'legacy': 'Legacy',
                    'legacy_southpaw': 'Legacy Southpaw'}[k],
                {'default': 'Move on the left stick, camera on the right; primary/secondary actions on the triggers.',
                 'southpaw': 'Swaps the sticks: move on the right, camera on the left (left-handed aim).',
                 'legacy': 'Primary/secondary actions move from the triggers to the bumpers (older layout).',
                 'legacy_southpaw': 'Southpaw sticks AND primary/secondary actions on the bumpers.'}[k])
               for k in CONTROLLER_LAYOUTS]),
        entry('keyboard', 'Text input', 'keyboard',
              [(k, o['label'], o['desc']) for k, o in KEYBOARD_OPTIONS.items()]),
        entry('storage', 'Save / load storage', 'storage',
              [(k, o['label'], o['desc']) for k, o in STORAGE_OPTIONS.items()]),
    ]
    for sid, spec in SUBSYSTEM_OPTIONS.items():
        D.append(entry(sid, _SUB_LABELS[sid], 'subsystems.%s' % sid,
                       [(k, o['label'], o['desc']) for k, o in spec['options'].items()]))
    return D


DECISIONS = _decisions()


def decision_by_id(dec_id):
    for d in DECISIONS:
        if d['id'] == dec_id:
            return d
    return None


def _get_path(cfg, path):
    node = cfg
    for part in path.split('.'):
        node = node.get(part) if isinstance(node, dict) else None
    return node


def _set_path(cfg, path, value):
    parts = path.split('.')
    node = cfg
    for part in parts[:-1]:
        node = node.setdefault(part, {})
    node[parts[-1]] = value


def decision_value(cfg, dec_id):
    d = decision_by_id(dec_id)
    return _get_path(cfg, d['path']) if d else None


def set_decision(cfg, dec_id, key):
    d = decision_by_id(dec_id)
    if d and key in [o[0] for o in d['options']]:
        _set_path(cfg, d['path'], key)


def _apply_layout(cfg):
    """Return a copy of the binds with the chosen layout preset applied over the base binds."""
    binds = dict(cfg['controller']['binds'])
    binds.update(CONTROLLER_LAYOUTS.get(cfg['controller'].get('layout', 'default'), {}))
    return binds


def load_config(path):
    if os.path.isfile(path):
        with open(path, 'r', encoding='utf-8') as f:
            cfg = json.load(f)
        # fill any missing keys from defaults (forward-compatible)
        d = default_config()
        for k, v in d.items():
            if k not in cfg:
                cfg[k] = v
            elif isinstance(v, dict):
                for kk, vv in v.items():
                    cfg[k].setdefault(kk, vv)
        return cfg
    return default_config()


def save_config(path, cfg):
    with open(path, 'w', encoding='utf-8') as f:
        json.dump(cfg, f, indent=2, sort_keys=True)


# ---------------------------------------------------------------------------
# emission
# ---------------------------------------------------------------------------
def _controller_header(cfg):
    c = cfg['controller']
    binds = _apply_layout(cfg)
    L = ['// ControllerConfig.h -- j2x-emitted controller mapping (edit platform.config.json + re-emit).',
         '// The Input platform layer (Input.reference) reads these to map the 360 pad to the PC controls.',
         '#ifndef J2X_CONTROLLER_CONFIG_H', '#define J2X_CONTROLLER_CONFIG_H', '',
         '// Stick layout: %s   (Default / Southpaw / Legacy / Legacy-Southpaw)' % c.get('layout'),
         '#define CTRL_LAYOUT_%s 1' % c.get('layout', 'default').upper(), '',
         '#define CTRL_INVERT_Y %d' % (1 if c.get('invert_y') else 0),
         '#define CTRL_LOOK_SENSITIVITY %.3ff' % float(c.get('look_sensitivity', 1.0)),
         '#define CTRL_STICK_DEADZONE %.3ff' % float(c.get('stick_deadzone', 0.24)),
         '#define CTRL_TRIGGER_THRESHOLD %.3ff' % float(c.get('trigger_threshold', 0.30)), '',
         '// action -> 360 control binding:']
    for key, label, _default in CONTROLLER_ACTIONS:
        L.append('#define CTRL_BIND_%-12s %-14s // %s' % (key.upper(), binds.get(key, 'NONE'), label))
    L += ['', '#endif // J2X_CONTROLLER_CONFIG_H', '']
    return '\n'.join(L)


def _kernel_files(cfg):
    kb, st = cfg['keyboard'], cfg['storage']
    h = ['// KernelConfig.h -- j2x-emitted 360 kernel-call selection (edit platform.config.json + re-emit).',
         '#ifndef J2X_KERNEL_CONFIG_H', '#define J2X_KERNEL_CONFIG_H', '',
         '// text-input medium: %s' % kb,
         '#define KERNEL_KEYBOARD_%s 1' % kb.upper(),
         '// storage medium: %s' % st,
         '#define KERNEL_STORAGE_%s 1' % st.upper(), '',
         '// Route ALL text entry and save/load through these -- the one seam the medium choice swaps.',
         'bool Kernel_ShowKeyboard(const wchar_t* title, const wchar_t* deflt, wchar_t* out, int outCount);',
         'bool Kernel_SelectDevice();               // pick + remember the storage device (once)',
         'bool Kernel_OpenSave(const char* rootName); // mount the writable save root',
         'void Kernel_CloseSave();',
         '', '#endif // J2X_KERNEL_CONFIG_H', '']

    cpp = ['// Kernel.cpp -- j2x-emitted 360 kernel wrappers for the chosen mediums. See KernelConfig.h.',
           '#include "KernelConfig.h"', '',
           '#if defined(_XBOX) || defined(_XENON)', '  #include <xtl.h>', '#endif',
           '#include <string.h>', '#include <wchar.h>', '']

    # --- keyboard ---
    cpp += ['// ---- text input -------------------------------------------------------']
    if kb == 'xshowkeyboard':
        cpp += [
            'bool Kernel_ShowKeyboard(const wchar_t* title, const wchar_t* deflt, wchar_t* out, int outCount)',
            '{', '#if defined(_XBOX) || defined(_XENON)',
            '    XOVERLAPPED ov; memset(&ov, 0, sizeof(ov));',
            '    DWORD r = XShowKeyboardUI(0, VKBD_DEFAULT, deflt ? deflt : L"", title ? title : L"",',
            '                              L"", out, (DWORD)outCount, &ov);',
            '    if (r != ERROR_IO_PENDING) return false;',
            '    while (!XHasOverlappedIoCompleted(&ov)) { /* pump: */ }',
            '    DWORD ext = 0; return XGetOverlappedResult(&ov, &ext, TRUE) == ERROR_SUCCESS;',
            '#else',
            '    if (out && outCount > 0) { if (deflt) wcsncpy(out, deflt, outCount); else out[0]=0; }',
            '    return true;', '#endif', '}', '']
    elif kb == 'osk':
        cpp += [
            'bool Kernel_ShowKeyboard(const wchar_t* title, const wchar_t* deflt, wchar_t* out, int outCount)',
            '{', '    // In-game OSK: implemented in OSK.reference (controller-driven keyboard).',
            '    extern bool OSK_Show(const wchar_t*, const wchar_t*, wchar_t*, int);',
            '    return OSK_Show(title, deflt, out, outCount);', '}', '']
    else:  # stub
        cpp += [
            'bool Kernel_ShowKeyboard(const wchar_t*, const wchar_t* deflt, wchar_t* out, int outCount)',
            '{   // J2X-STUB[kernel-keyboard]: no text entry -- returns the default string unchanged.',
            '    if (out && outCount > 0) { if (deflt) wcsncpy(out, deflt, outCount); else out[0]=0; }',
            '    return true;', '}', '']

    # --- storage ---
    cpp += ['// ---- save / load storage ---------------------------------------------']
    if st == 'stfs':
        cpp += [
            '#if defined(_XBOX) || defined(_XENON)',
            'static DWORD g_deviceId = 0;',
            '#endif',
            'bool Kernel_SelectDevice()',
            '{', '#if defined(_XBOX) || defined(_XENON)',
            '    XOVERLAPPED ov; memset(&ov, 0, sizeof(ov));',
            '    DWORD r = XShowDeviceSelectorUI(0, XCONTENTTYPE_SAVEDGAME, XCONTENTFLAG_MANAGESTORAGE,',
            '                                    &g_deviceId, &ov);',
            '    if (r != ERROR_IO_PENDING) return false;',
            '    while (!XHasOverlappedIoCompleted(&ov)) { }',
            '    DWORD ext = 0; return XGetOverlappedResult(&ov, &ext, TRUE) == ERROR_SUCCESS;',
            '#else', '    return true;', '#endif', '}', '',
            'bool Kernel_OpenSave(const char* rootName)',
            '{', '#if defined(_XBOX) || defined(_XENON)',
            '    XCONTENT_DATA data; memset(&data, 0, sizeof(data));',
            '    data.DeviceID = g_deviceId; data.dwContentType = XCONTENTTYPE_SAVEDGAME;',
            '    strncpy(data.szFileName, rootName, XCONTENT_MAX_FILENAME_LENGTH - 1);',
            '    DWORD disp = 0;',
            '    DWORD r = XContentCreate(0, "Save", &data, XCONTENTFLAG_OPENALWAYS,',
            '                             &disp, NULL, NULL);',
            '    return r == ERROR_SUCCESS;',
            '#else', '    (void)rootName; return true;', '#endif', '}', '',
            'void Kernel_CloseSave()',
            '{', '#if defined(_XBOX) || defined(_XENON)', '    XContentClose("Save", NULL);',
            '#endif', '}', '']
    elif st == 'rawhdd':
        cpp += [
            'bool Kernel_SelectDevice() { return true; }   // raw path: no device selector',
            'bool Kernel_OpenSave(const char* rootName)',
            '{   // dev HDD path (game:\\Save\\); not a content container -- devkit only.',
            '    (void)rootName; return true;', '}',
            'void Kernel_CloseSave() {}', '']
    else:  # memory
        cpp += [
            'bool Kernel_SelectDevice() { return true; }',
            'bool Kernel_OpenSave(const char*)',
            '{   // J2X-STUB[kernel-storage]: memory-only -- worlds do NOT persist across reboots.',
            '    return true; }',
            'void Kernel_CloseSave() {}', '']
    return '\n'.join(h), '\n'.join(cpp)


def _choices_md(cfg, sdk_info=None, unverified=None):
    def menu(title, options, chosen):
        L = ['## %s' % title, '']
        for key, o in options.items():
            mark = 'x' if key == chosen else ' '
            L.append('- [%s] **%s** (`%s`)  ' % (mark, o['label'], key))
            L.append('  %s' % o['desc'])
        L.append('')
        return L

    c = cfg['controller']
    binds = _apply_layout(cfg)
    L = ['# PLATFORM_CHOICES.md -- user decisions the converter emitted', '',
         'These are the questions only you can answer. The current pick is checked; change it in',
         '`platform.config.json` (or via `j2x platform-config`) and re-emit. Everything below is wired',
         'into the port -- ControllerConfig.h, KernelConfig.h, Kernel.cpp.', '']
    L += ['## Controller mapping', '',
          '- Layout: **%s** (options: %s)' % (c.get('layout'), ', '.join(CONTROLLER_LAYOUTS)),
          '- Invert Y: **%s**   Sensitivity: **%.2f**   Stick deadzone: **%.2f**   Trigger: **%.2f**'
          % (c.get('invert_y'), c.get('look_sensitivity', 1.0), c.get('stick_deadzone', 0.24),
             c.get('trigger_threshold', 0.30)), '',
          '| Action | Bound to |', '|---|---|']
    for key, label, _d in CONTROLLER_ACTIONS:
        L.append('| %s | `%s` |' % (label, binds.get(key, 'NONE')))
    L += ['', 'Available controls to rebind to: `%s`.' % '`, `'.join(CONTROLLER_BUTTONS), '']
    # Every decision, each posed as a QUESTION with its stakes + the recommended answer, then the
    # options as a checked multiple-choice list.
    L += ['## Subsystem decisions', '',
          'Each of these is a real question you must answer for the port. The recommended answer and '
          'why are given; the current pick is checked.', '']
    for d in DECISIONS:
        if d['id'] == 'controller_layout':
            continue                                      # already shown above under Controller mapping
        chosen = _get_path(cfg, d['path'])
        L.append('### %s — %s' % (d['label'], d['question']))
        L.append('')
        L.append('*Why it matters:* %s' % d['why'])
        L.append('')
        L.append('*Recommended:* `%s` — %s' % (d['recommend'][0], d['recommend'][1]))
        L.append('')
        for key, olabel, odesc in d['options']:
            mark = 'x' if key == chosen else ' '
            rec = '  ⭐' if key == d['recommend'][0] else ''
            L.append('- [%s] **%s** (`%s`)%s — %s' % (mark, olabel, key, rec, odesc))
        L.append('')
    # SDK provenance: the kernel code was emitted against the user's own installed SDK, none shipped.
    L += ['## SDK', '']
    if sdk_info is not None and sdk_info.root:
        L.append('Kernel code emitted against **%s** located at `%s` (via %s). The converter ships no '
                 'SDK headers or signatures -- the real declarations come from your `<xtl.h>` at build '
                 'time.' % (sdk_info.name, sdk_info.root, sdk_info.source))
        if unverified:
            L += ['', '> WARNING: these symbols were NOT found in the located SDK headers -- verify the '
                  'medium/SDK version: `%s`.' % '`, `'.join(unverified)]
    else:
        L.append('Chosen mediums need no SDK-gated kernel symbols (or a portable target).')
    L.append('')
    return '\n'.join(L) + '\n'


def emit(out_dir, cfg, sdk_info=None, unverified=None):
    """Write ControllerConfig.h, KernelConfig.h, Kernel.cpp, and PLATFORM_CHOICES.md into out_dir.
    Returns the list of paths written."""
    kh, kc = _kernel_files(cfg)
    files = {
        'ControllerConfig.h': _controller_header(cfg),
        'KernelConfig.h': kh,
        'Kernel.cpp': kc,
        'PLATFORM_CHOICES.md': _choices_md(cfg, sdk_info=sdk_info, unverified=unverified),
    }
    written = []
    for name, content in files.items():
        p = os.path.join(out_dir, name)
        with open(p, 'w', encoding='utf-8') as f:
            f.write(content if content.endswith('\n') else content + '\n')
        written.append(p)
    return written


def required_symbols(cfg):
    """The 360 kernel symbols the chosen mediums will call."""
    syms = []
    for medium in (cfg.get('keyboard'), cfg.get('storage')):
        syms += _MEDIUM_SYMBOLS.get(medium, [])
    return syms


def gate_sdk(cfg, platform='xbox360', sdk_override=None):
    """Enforce the SDK gate for the chosen mediums. If a chosen medium needs kernel symbols and the
    platform is SDK-gated, require the SDK (raises sdk.SdkNotFound if absent) and verify each symbol
    exists in the located SDK. Returns (sdk_info_or_None, unverified_symbols)."""
    syms = required_symbols(cfg)
    if not syms or not sdk.requires_sdk(platform):
        return None, []
    info = sdk.require(platform, override=sdk_override)      # raises SdkNotFound -> caller reports
    unverified = [s for s in syms if not info.has_symbol(s)]
    return info, unverified


def run(out_dir, config_path=None, updates=None, list_only=False,
        platform='xbox360', sdk_override=None):
    """Load (or default) the config, apply `updates` (dict of dotted keys), save it, and emit -- unless
    list_only. The SDK gate runs before any 360 kernel code is written: a medium needing XDK symbols
    is refused unless the SDK is located (env/override/standard path). Returns
    (cfg, written_paths, sdk_info, unverified_symbols). `updates` keys: layout, invert_y,
    look_sensitivity, keyboard, storage, bind.<action>."""
    config_path = config_path or os.path.join(out_dir, 'platform.config.json')
    cfg = load_config(config_path)
    for k, v in (updates or {}).items():
        if k in ('keyboard', 'storage'):
            cfg[k] = v
        elif k in ('layout', 'invert_y', 'look_sensitivity', 'stick_deadzone', 'trigger_threshold'):
            cfg['controller'][k] = v
        elif k.startswith('bind.'):
            cfg['controller']['binds'][k[len('bind.'):]] = v
        elif k.startswith('subsystem.'):
            cfg.setdefault('subsystems', {})[k[len('subsystem.'):]] = v
    if list_only:
        return cfg, [], None, []
    info, unverified = gate_sdk(cfg, platform=platform, sdk_override=sdk_override)
    save_config(config_path, cfg)
    written = emit(out_dir, cfg, sdk_info=info, unverified=unverified)
    return cfg, written, info, unverified
