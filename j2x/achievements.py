# -*- coding: utf-8 -*-
r"""
achievements.py -- analyze the program's achievements and map them onto the console's achievement system.

On a PC/Java program, achievements are just objects created and unlocked in code. On a console they are
NOT free-form: every achievement must be declared up front in the title's configuration (on Xbox 360,
an XLAST project compiled to an SPA blob and embedded in the build) and unlocked through a platform call
that writes to the signed-in profile over the platform's online service (XUserWriteAchievements on 360).
That write is a networked/telemetry operation -- it needs a Title ID, a signed-in user, and the network;
offline it queues and syncs on the next connection. None of that exists in the source program, so it has
to be synthesized. This module:

  * SCANS the converted code for the program's achievement definitions (id, key, hidden flag, parent).
  * MAPS them to a platform achievement table -- assigns a gamerscore budget (summing exactly to the
    target), flags console limits (max count, score cap), marks hidden/secret ones.
  * EMITS: a report (ACHIEVEMENTS.md), an XLAST-style achievement config skeleton to import into the
    platform's submission tool, and a code seam (Achievements.h/.cpp) that turns each in-code unlock into
    the platform write (SDK-gated), plus id constants the game code calls.

The report spells out the telemetry/networking requirement explicitly so it isn't discovered at cert.
"""

import os
import re
import zlib

try:
    from PIL import Image
    _HAS_PIL = True
except ImportError:                                       # pragma: no cover
    _HAS_PIL = False


# --- title id --------------------------------------------------------------
# A 360 title id is 32-bit: high 16 = publisher id, low 16 = title number. Microsoft assigned
# publisher ids to licensed studios; a self-published/homebrew title (no MS submission) picks its own
# and just has to avoid colliding with an existing one. We suggest a stable id in a high, unlikely-used
# publisher range and derive the title number from the name -- the user can override with an explicit id.
_HOMEBREW_PUBLISHER = 0xFFFE


def normalize_title_id(tid):
    """Accept 'FFFE1234', '0xFFFE1234', 'fffe1234' -> canonical 8-hex-digit UPPER string, or None."""
    if not tid:
        return None
    s = str(tid).strip().lower()
    if s.startswith('0x'):
        s = s[2:]
    if re.fullmatch(r'[0-9a-f]{1,8}', s):
        return '%08X' % int(s, 16)
    return None


def suggest_title_id(seed):
    """A stable, self-issued title id derived from `seed` (e.g. the game name). NOT guaranteed unique --
    the user must check it against a title-id list. Returns an 8-hex-digit UPPER string."""
    num = zlib.crc32((seed or 'title').encode('utf-8')) & 0xFFFF
    return '%04X%04X' % (_HOMEBREW_PUBLISHER, num)

# `new Achievement(<id>, "<key>", <col>, <row>, <icon>, <parent>)`, optional `->setSpecial()`.
_NEW_ACH = re.compile(r'new\s+Achievement\s*\(\s*(\d+)\s*,\s*"([^"]+)"')
_SPECIAL = re.compile(r'setSpecial\s*\(\s*\)')
_PARENT = re.compile(r'"[^"]+",[^,]+,[^,]+,[^,]+,\s*([A-Za-z_]\w*)\s*\)')  # 6th arg (parent var/NULL)
# the 5th arg is the icon source: Item::s_book / Block::s_wood / ItemStack(...) etc.
_ICON = re.compile(r'"[^"]+"\s*,[^,]+,[^,]+,\s*((?:Item|Block)::s_\w+)')


def _humanize(key):
    """camelCase / snake key -> a readable title. openInventory -> Open Inventory,
    onARail -> On A Rail."""
    s = key.replace('_', ' ')
    s = re.sub(r'([a-z])([A-Z])', r'\1 \2', s)            # openInventory -> open Inventory
    s = re.sub(r'([A-Z])([A-Z][a-z])', r'\1 \2', s)       # ARail -> A Rail
    return s[:1].upper() + s[1:] if s else key


def load_lang_names(lang_file):
    """Parse a .lang file for `achievement.<key>=Name` and `achievement.<key>.desc=Desc`. Returns
    {key: {'name':..., 'desc':...}}. Empty if the file is missing."""
    out = {}
    if not lang_file or not os.path.isfile(lang_file):
        return out
    for line in open(lang_file, 'r', encoding='utf-8', errors='replace'):
        if '=' not in line or not line.startswith('achievement.'):
            continue
        k, v = line.rstrip('\n').split('=', 1)
        rest = k[len('achievement.'):]
        if rest.endswith('.desc'):
            out.setdefault(rest[:-5], {})['desc'] = v
        else:
            out.setdefault(rest, {})['name'] = v
    return out


def scan_achievements(code_dir, lang_file=None):
    """Find every achievement the program defines. Returns a sorted list of dicts
    {id, key, name, desc, hidden, parent}. `hidden` = the program's 'special/secret' flag."""
    names = load_lang_names(lang_file)
    found = {}
    for dp, _dirs, files in os.walk(code_dir):
        for f in files:
            if not (f.endswith('.cpp') or f.endswith('.java')):
                continue
            try:
                text = open(os.path.join(dp, f), 'r', encoding='utf-8', errors='replace').read()
            except (IOError, OSError):
                continue
            if 'new Achievement(' not in text:
                continue
            for line in text.split('\n'):
                m = _NEW_ACH.search(line)
                if not m:
                    continue
                aid, key = int(m.group(1)), m.group(2)
                pm = _PARENT.search(line)
                parent = pm.group(1) if pm else None
                if parent in ('NULL', '0', 'null'):
                    parent = None
                im = _ICON.search(line)
                nm = names.get(key, {})
                found[aid] = {
                    'id': aid, 'key': key,
                    'name': nm.get('name') or _humanize(key),
                    'desc': nm.get('desc') or ('Earn the %s achievement.' % _humanize(key)),
                    'hidden': bool(_SPECIAL.search(line)),
                    'parent': parent,
                    'icon': im.group(1) if im else None,   # e.g. 'Item::s_book' / 'Block::s_wood'
                }
    return [found[k] for k in sorted(found)]


# --- icon resolution: symbol -> atlas sprite -------------------------------
# Items place their sprite with setIconCoord(col,row) (or setIconIndex(n)) in gui/items.png. Blocks
# carry a terrain.png index either as a literal 2nd ctor int (base Block(id, tex, mat)) OR, for a
# subclass, as `this->blockIndexInTexture = N;` inside the subclass constructor. Resolve both.
_ITEM_DEF = re.compile(r'Item::(s_\w+)\s*=\s*(.+?);')
_ICON_COORD = re.compile(r'setIconCoord\(\s*(\d+)\s*,\s*(\d+)\s*\)')
_ICON_INDEX = re.compile(r'setIconIndex\(\s*(\d+)\s*\)')
_BLOCK_ASSIGN = re.compile(r'Block::(s_\w+)\s*=\s*\(new\s+(\w+)\(([^)]*)\)')
_BIT_LITERAL = re.compile(r'blockIndexInTexture\s*=\s*(\d+)\s*;')
_INT_ONLY = re.compile(r'^\s*(\d+)\s*$')


def _read(path):
    try:
        return open(path, 'r', encoding='utf-8', errors='replace').read()
    except (IOError, OSError):
        return ''


def _block_class_textures(code_dir):
    """{BlockSubclass: terrainIndex} from each subclass ctor's literal `blockIndexInTexture = N;`.
    The base `Block` sets it from a ctor arg (no literal) and is intentionally absent here."""
    out = {}
    for dp, _dirs, files in os.walk(code_dir):
        for f in files:
            if not (f.startswith('Block') and f.endswith('.cpp')) or f == 'Block.cpp':
                continue
            m = _BIT_LITERAL.search(_read(os.path.join(dp, f)))
            if m:
                out[f[:-4]] = int(m.group(1))
    return out


def resolve_icon_atlas(code_dir):
    """Map an icon symbol (s_book / s_wood) to (atlas, col, row), atlas in {'items','terrain'}, by
    reading the game's Item/Block definitions. Unresolvable symbols are simply absent."""
    out = {}
    class_tex = _block_class_textures(code_dir)
    item_cpp = block_cpp = None
    for dp, _dirs, files in os.walk(code_dir):
        for f in files:
            if f == 'Item.cpp':
                item_cpp = os.path.join(dp, f)
            elif f == 'Block.cpp':
                block_cpp = os.path.join(dp, f)
    if item_cpp:
        text = _read(item_cpp)
        for m in _ITEM_DEF.finditer(text):
            sym, body = m.group(1), m.group(2)
            cm = _ICON_COORD.search(body)
            if cm:
                out[sym] = ('items', int(cm.group(1)), int(cm.group(2)))
                continue
            im = _ICON_INDEX.search(body)
            if im:
                n = int(im.group(1))
                out[sym] = ('items', n % 16, n // 16)
    if block_cpp:
        text = _read(block_cpp)
        for m in _BLOCK_ASSIGN.finditer(text):
            sym, cls, args = m.group(1), m.group(2), m.group(3)
            tex = class_tex.get(cls)                        # subclass ctor's literal index
            if tex is None:                                 # base Block(id, tex, mat): 2nd arg literal
                parts = args.split(',')
                if len(parts) >= 2 and _INT_ONLY.match(parts[1]):
                    tex = int(parts[1].strip())
            if tex is not None:
                out[sym] = ('terrain', tex % 16, tex // 16)
    return out


_ATLAS_FILE = {'items': os.path.join('gui', 'items.png'), 'terrain': 'terrain.png'}


def _locked_variant(img):
    """A locked-achievement tile: desaturated + darkened, alpha preserved."""
    g = img.convert('LA').convert('RGBA')
    px = g.load()
    for y in range(g.height):
        for x in range(g.width):
            r, gg, b, a = px[x, y]
            px[x, y] = (r * 40 // 100, gg * 40 // 100, b * 40 // 100, a)
    return g


def wire_icons(entries, code_dir, content_dir, out_dir, tile=64):
    """For each achievement whose icon resolves, crop its 16x16 sprite from the atlas, scale to
    `tile`x`tile` (nearest, crisp), and write unlocked + locked PNGs under out_dir/achievement_images/.
    Mutates each entry with 'image_unlocked'/'image_locked' (basenames) when wired. Returns
    {'wired': [...keys], 'unresolved': [(key, reason)]}."""
    wired, unresolved = [], []
    if not _HAS_PIL:
        return {'wired': [], 'unresolved': [(e['key'], 'Pillow not installed') for e in entries]}
    atlas_map = resolve_icon_atlas(code_dir)
    atlases = {}
    img_dir = os.path.join(out_dir, 'achievement_images')
    for e in entries:
        icon = e.get('icon')
        sym = icon.split('::')[-1] if icon else None
        if not sym or sym not in atlas_map:
            unresolved.append((e['key'], 'icon %s not resolvable to an atlas sprite' % (icon or '(none)')))
            continue
        atlas, col, row = atlas_map[sym]
        if atlas not in atlases:
            ap = os.path.join(content_dir, _ATLAS_FILE[atlas].replace('/', os.sep))
            if not os.path.isfile(ap):
                unresolved.append((e['key'], 'atlas %s missing at %s' % (atlas, ap)))
                continue
            atlases[atlas] = Image.open(ap).convert('RGBA')
        elif atlases[atlas] is None:
            unresolved.append((e['key'], 'atlas %s missing' % atlas))
            continue
        sheet = atlases[atlas]
        sprite = sheet.crop((col * 16, row * 16, col * 16 + 16, row * 16 + 16))
        sprite = sprite.resize((tile, tile), Image.NEAREST)
        if not os.path.isdir(img_dir):
            os.makedirs(img_dir)
        un = '%s_unlocked.png' % e['key']
        lk = '%s_locked.png' % e['key']
        sprite.save(os.path.join(img_dir, un))
        _locked_variant(sprite).save(os.path.join(img_dir, lk))
        e['image_unlocked'] = 'achievement_images/%s' % un
        e['image_locked'] = 'achievement_images/%s' % lk
        wired.append(e['key'])
    return {'wired': wired, 'unresolved': unresolved}


# Console achievement constraints (Xbox 360). Retail titles: up to 99 achievements totalling 1000
# gamerscore; Arcade titles: up to 12 totalling 200. Used to validate the mapping.
PROFILES = {
    'retail': {'max_count': 99, 'score': 1000},
    'arcade': {'max_count': 12, 'score': 200},
}


def _distribute_score(n, total):
    """Split `total` gamerscore across n achievements as evenly as possible in steps of 5, summing
    EXACTLY to total (the last few absorb the remainder)."""
    if n <= 0:
        return []
    base = (total // n) // 5 * 5
    scores = [base] * n
    remaining = total - base * n
    i = 0
    while remaining > 0 and i < n:
        scores[i] += 5
        remaining -= 5
        i += 1
    # if base was 0 (n huge), fall back to 1s so nothing is zero-valued
    if base == 0 and total >= n:
        scores = [total // n] * n
        scores[-1] += total - sum(scores)
    return scores


def map_to_platform(achievements, profile='retail', title_profile=None):
    """Assign a platform achievement id (1-based, as consoles expect) + gamerscore to each. Returns
    {'entries':[...], 'profile':..., 'total_score':..., 'warnings':[...]}."""
    prof = PROFILES.get(profile, PROFILES['retail'])
    n = len(achievements)
    warnings = []
    if n > prof['max_count']:
        warnings.append('%d achievements exceeds the %s limit of %d -- split across titles or raise the '
                        'profile.' % (n, profile, prof['max_count']))
    scores = _distribute_score(n, prof['score'])
    entries = []
    for i, a in enumerate(achievements):
        entries.append({
            'platform_id': i + 1,            # consoles number achievements from 1
            'source_id': a['id'],
            'key': a['key'],
            'name': a['name'],
            'desc': a['desc'],
            'gamerscore': scores[i] if i < len(scores) else 0,
            'hidden': a['hidden'],
            'icon': a.get('icon'),
        })
    total = sum(e['gamerscore'] for e in entries)
    if total != prof['score'] and n > 0:
        warnings.append('gamerscore sums to %d, not the %s target of %d.' % (total, profile, prof['score']))
    return {'entries': entries, 'profile': profile, 'total_score': total, 'warnings': warnings,
            'count': n, 'limit': prof['max_count']}


# --- emission ---------------------------------------------------------------
def _const_name(key):
    return 'ACH_' + re.sub(r'([a-z])([A-Z])', r'\1_\2', key).upper()


def render_report(mapping, delivery='online', icons=None, title_id=None, title_suggested=False):
    e = mapping['entries']
    L = ['# ACHIEVEMENTS.md -- achievement conversion', '',
         'The program defines %d achievement(s). A console does NOT let code invent achievements at '
         'runtime -- each must be declared in the title configuration and unlocked through a platform '
         'call that writes to the signed-in profile over the online service. This maps the program\'s '
         'achievements onto that model.' % mapping['count'], '',
         '- profile: **%s** (max %d, %d gamerscore)   assigned total: **%d**   delivery: **%s**'
         % (mapping['profile'], mapping['limit'], PROFILES[mapping['profile']]['score'],
            mapping['total_score'], delivery), '']
    for w in mapping['warnings']:
        L.append('> WARNING: %s' % w)
    if mapping['warnings']:
        L.append('')
    L += ['## Achievement table', '',
          '| # | Key | Name | Gamerscore | Hidden | Tile art |', '|---:|---|---|---:|:---:|---|']
    for x in e:
        art = '`%s`' % x['image_unlocked'] if x.get('image_unlocked') else 'TODO'
        L.append('| %d | `%s` | %s | %d | %s | %s |'
                 % (x['platform_id'], x['key'], x['name'], x['gamerscore'],
                    'yes' if x['hidden'] else '', art))
    if icons is not None:
        L += ['', '### Tile art', '',
              'Achievement tiles were auto-wired from the game\'s own item/block icons (cropped from the '
              'texture atlas, scaled to 64x64; a darkened/desaturated locked variant is generated). They '
              'land in `achievement_images/` and are referenced by the XLAST project.', '',
              '- auto-wired: **%d**   needs manual art: **%d**'
              % (len(icons['wired']), len(icons['unresolved']))]
        if icons['unresolved']:
            L += ['', 'Icons not auto-resolved (add art by hand in the XLAST project):', '']
            for key, reason in icons['unresolved']:
                L.append('- `%s` — %s' % (key, reason))
        L.append('')
    L += ['', '## What the console requires (and the source program does not provide)', '',
          '1. **Declared up front.** Every achievement above must be entered into the title\'s '
          'configuration tool (Xbox 360: an XLAST project, compiled to an SPA blob embedded in the '
          'build). A skeleton is emitted as `achievements.xlast` -- import it, add the locked/unlocked '
          'tile art, and build the SPA.',
          '2. **Unlocked via a networked write.** Each in-code unlock becomes a call to the platform '
          'achievement write (Xbox 360: `XUserWriteAchievements`). This is the telemetry/networking '
          'path you flagged: it needs',
          '   - a **Title ID** assigned to the build,',
          '   - a **signed-in profile** (tie this to the *User / profile* decision),',
          '   - the **network** for immediate sync -- **offline it queues and syncs on next connect**.',
          '3. **Budget + limits.** Gamerscore must sum to the profile target and the count must stay '
          'under the limit (validated above).',
          '4. **Hidden achievements.** The program\'s "special/secret" flag maps to the console '
          '"hidden" attribute (shown above).', '',
          '## Title ID', '',
          'The achievement config (SPA) and the built XEX are both keyed to a **32-bit Title ID** '
          '(high 16 bits = publisher, low 16 = title number). It must be the SAME in the XLAST project '
          'and on the XEX, or unlocks will not bind.', '',
          '- Title ID: **0x%s**%s' % (title_id or '00000000',
                                      '  _(suggested — see below)_' if title_suggested else ''),
          '',
          'Since Xbox 360 titles are no longer submitted to Microsoft, you **self-issue** this: there is '
          'no database to request one from. Pick a value and make sure it does not collide with an '
          'existing title (community title-ID lists exist to check against). The suggested id uses a high '
          'publisher prefix (`0xFFFE`) unlikely to clash and a title number derived from the name; '
          'override it with your own if you prefer.', '',
          'Set the same value in **Visual Studio -> project properties -> Xbox 360 -> Title ID** (it is '
          'written into the XEX at build), and in the XLAST project (already set in the emitted '
          '`achievements.xlast`).', '',
          '## Code seam', '',
          'Emitted `Achievements.h/.cpp` provides `Achievement_Unlock(userIndex, ACH_*)`. The converter '
          'routes each in-code unlock (e.g. the program\'s `triggerAchievement`) to it; on hardware it '
          'performs the platform write, off-hardware it is a safe no-op. The `ACH_*` id constants match '
          'the table above.', '']
    if delivery != 'online':
        L += ['> NOTE: delivery is **%s** -- unlocks are tracked locally only, no online write. Switch '
              'the *Achievements* decision to `xbox_live` for real profile achievements.' % delivery, '']
    return '\n'.join(L) + '\n'


def emit_xlast(mapping, out_path, title_id=None):
    """Write an XLAST-style achievement config skeleton (XML) to import into the submission tool."""
    tid = normalize_title_id(title_id) or '00000000'
    L = ['<?xml version="1.0" encoding="utf-8"?>',
         '<!-- Achievement config skeleton emitted by j2x. Import into the platform submission/config',
         '     tool (Xbox 360: XLAST), then build the SPA. The titleId below MUST match the Title ID set',
         '     on the XEX in the Visual Studio project (Xbox 360 property pages). -->',
         '<XLASTProject titleId="0x%s">' % tid,
         '  <Configuration>',
         '    <Achievements>']
    for x in mapping['entries']:
        L.append('      <Achievement id="%d" name="%s" gamerscore="%d" hidden="%s">'
                 % (x['platform_id'], _xml(x['name']), x['gamerscore'],
                    'true' if x['hidden'] else 'false'))
        L.append('        <LockedDescription>%s</LockedDescription>' % _xml(x['desc']))
        L.append('        <UnlockedDescription>%s</UnlockedDescription>' % _xml(x['desc']))
        if x.get('image_unlocked'):
            L.append('        <Image type="unlocked">%s</Image>' % _xml(x['image_unlocked']))
            L.append('        <Image type="locked">%s</Image>' % _xml(x['image_locked']))
        else:
            L.append('        <!-- TODO: add locked/unlocked 64x64 tile (icon not auto-resolved) -->')
        L.append('      </Achievement>')
    L += ['    </Achievements>', '  </Configuration>', '</XLASTProject>', '']
    with open(out_path, 'w', encoding='utf-8') as f:
        f.write('\n'.join(L))
    return out_path


def _xml(s):
    return (s.replace('&', '&amp;').replace('<', '&lt;').replace('>', '&gt;')
            .replace('"', '&quot;'))


def emit_code(mapping, out_dir, delivery='online'):
    """Write Achievements.h + Achievements.cpp: the ACH_* id constants and the unlock seam."""
    h = ['// Achievements.h -- j2x-emitted achievement ids + unlock seam. See ACHIEVEMENTS.md.',
         '#ifndef J2X_ACHIEVEMENTS_H', '#define J2X_ACHIEVEMENTS_H', '',
         '// Platform achievement ids (match the title config / XLAST project).']
    for x in mapping['entries']:
        h.append('#define %-28s %d' % (_const_name(x['key']), x['platform_id']))
    h += ['',
          '// Unlock an achievement for a signed-in user. On hardware this writes to the profile over',
          '// the online service (queues offline); off-hardware it is a no-op returning true.',
          'bool Achievement_Unlock(int userIndex, int achievementId);',
          '', '#endif // J2X_ACHIEVEMENTS_H', '']

    live = delivery == 'online'
    c = ['// Achievements.cpp -- j2x-emitted. Routes in-code unlocks to the platform write.',
         '#include "stdafx.h"',                              # forced PCH: must be the first include
         '#include "Achievements.h"', '',
         '#if defined(_XBOX) || defined(_XENON)', '  #include <xtl.h>', '#endif',
         '#include <string.h>', '',
         'bool Achievement_Unlock(int userIndex, int achievementId)', '{']
    if live:
        c += ['#if defined(_XBOX) || defined(_XENON)',
              '    XUSER_ACHIEVEMENT ach;',
              '    ach.dwUserIndex = (DWORD)userIndex;',
              '    ach.dwAchievementId = (DWORD)achievementId;',
              '    XOVERLAPPED ov; memset(&ov, 0, sizeof(ov));',
              '    // async LIVE write; queues + syncs later if offline.',
              '    DWORD r = XUserWriteAchievements(1, &ach, &ov);',
              '    if (r != ERROR_IO_PENDING && r != ERROR_SUCCESS) return false;',
              '    while (!XHasOverlappedIoCompleted(&ov)) { }',
              '    DWORD ext = 0; return XGetOverlappedResult(&ov, &ext, TRUE) == ERROR_SUCCESS;',
              '#else',
              '    (void)userIndex; (void)achievementId; return true;',
              '#endif']
    else:
        c += ['    // J2X-STUB[achievements-local]: %s delivery -- track locally, no online write.' % delivery,
              '    (void)userIndex; (void)achievementId; return true;']
    c += ['}', '']
    hp = os.path.join(out_dir, 'Achievements.h')
    cp = os.path.join(out_dir, 'Achievements.cpp')
    with open(hp, 'w', encoding='utf-8') as f:
        f.write('\n'.join(h))
    with open(cp, 'w', encoding='utf-8') as f:
        f.write('\n'.join(c))
    return [hp, cp]


def run(code_dir, out_dir, lang_file=None, profile='retail', delivery='online', content_dir=None,
        title_id=None, title_name=None):
    """Scan, map, wire icons, and emit ACHIEVEMENTS.md + achievements.xlast + Achievements.h/.cpp.
    `content_dir` (with the game's atlases) enables auto-wiring the achievement tile art. `title_id`
    (self-issued, no MS submission needed) is written into the XLAST; if omitted, a stable one is
    suggested from `title_name`. Returns a dict including the icon-wiring result + title id."""
    if not os.path.isdir(out_dir):
        os.makedirs(out_dir)
    achs = scan_achievements(code_dir, lang_file=lang_file)
    mapping = map_to_platform(achs, profile=profile)
    icons = {'wired': [], 'unresolved': []}
    cdir = content_dir or _guess_content_dir(out_dir)
    if cdir:
        icons = wire_icons(mapping['entries'], code_dir, cdir, out_dir)
    # Title ID default is the conventional "unset" placeholder 00000000 -- the user self-issues one
    # (Suggest button / --title-id) rather than us auto-filling it.
    tid = normalize_title_id(title_id) or '00000000'
    suggested = False
    report = os.path.join(out_dir, 'ACHIEVEMENTS.md')
    with open(report, 'w', encoding='utf-8') as f:
        f.write(render_report(mapping, delivery=delivery, icons=icons,
                              title_id=tid, title_suggested=suggested))
    xlast = emit_xlast(mapping, os.path.join(out_dir, 'achievements.xlast'), title_id=tid)
    code = emit_code(mapping, out_dir, delivery=delivery)
    return {'mapping': mapping, 'report': report, 'xlast': xlast, 'code': code,
            'count': mapping['count'], 'icons': icons, 'title_id': tid, 'title_suggested': suggested}


def _guess_content_dir(out_dir):
    """Find the game's Content folder (with the texture atlases) near out_dir."""
    for cand in (os.path.join(out_dir, 'Content'), out_dir):
        if os.path.isfile(os.path.join(cand, 'terrain.png')) or \
           os.path.isfile(os.path.join(cand, 'gui', 'items.png')):
            return cand
    return None
