#!/usr/bin/env python3
"""RETool -- inventory, diff and patch Xbox 360 PE images.

Built around one finding: what you can usefully diff between builds is exactly
what survives compilation.

  survives : RTTI class names, XUI scene names, platform imports, string
             literals -- all source-derived and position-independent.
  does not : function bodies. PPC code is full of position-dependent branch
             targets and address-forming immediates, so a relink regenerates
             essentially everything. Measured across one title's lineage:

               same binary, string-patched .... 100%  of functions identical
               adjacent versions, same XDK ....   0.02%
               version jump + XDK change ......   0%
               +27 functions, same class list .   0.2%

             There is no gradient: either the image was patched in place, or
             every function moved. So `funcs` is only meaningful for detecting
             patched variants of ONE build; use `classes`/`scenes` across
             rebuilds.

Commands
    info       <image>...          metrics table
    classes    <image> [<image2>]  RTTI class inventory / diff
    scenes     <image> [<image2>]  XUI scene + .xur resource inventory / diff
    imports    <image> [<image2>]  platform (kernel/XAM) symbol inventory / diff
    strings    <image> [<image2>]  UTF-16 text inventory / diff
    funcs      <old> <new>         function-level diff (see caveat above)
    timeline   <image>...          first-seen / persistent-core matrix
    patchdiff  <old> <new>         capture an in-place toggle as a portable patch
    patchapply <image> <patch>     re-apply a captured toggle

Global flags: --json FILE  --csv FILE  --limit N  --all  -o FILE
"""

DESCRIPTION = "diff class/scene/import inventories & timelines across Xbox 360 builds; capture/apply byte patches"
import os, re, json, csv, struct, argparse, collections

BS = chr(92)
LIBRARY_TAGS = ('@std@@', '@tr1@', '@stlp')
CTX = 24
MAXGAP = 64


class Image:
    def __init__(self, path):
        self.path = path
        self.name = os.path.basename(path)
        self.d = open(path, 'rb').read()
        d = self.d
        e = struct.unpack_from('<I', d, 0x3c)[0]
        if d[e:e + 2] != b'PE':
            raise ValueError('%s: not a PE image' % path)
        nsec = struct.unpack_from('<H', d, e + 6)[0]
        opt = e + 24
        self.base = struct.unpack_from('<I', d, opt + 28)[0]
        self.entry = struct.unpack_from('<I', d, opt + 16)[0]
        so = opt + struct.unpack_from('<H', d, e + 20)[0]
        self.secs = []
        for i in range(nsec):
            o = so + i * 40
            nm = d[o:o + 8].rstrip(b'\0').decode('ascii', 'replace')
            vs, va, rs, rp = struct.unpack_from('<IIII', d, o + 8)
            self.secs.append((nm, va, vs, rp, rs))

    def sec(self, name):
        for s in self.secs:
            if s[0] == name:
                return s
        return None

    def v2o(self, va):
        r = va - self.base
        for nm, sva, vs, rp, rs in self.secs:
            if sva <= r < sva + max(vs, rs):
                o = r - sva
                if o < rs:
                    return rp + o
        return None

    def functions(self):
        """{begin_va: size_bytes} decoded from .pdata RUNTIME_FUNCTION entries."""
        pd = self.sec('.pdata')
        if not pd:
            return {}
        out = {}
        for i in range(pd[2] // 8):
            a, x = struct.unpack_from('>II', self.d, pd[3] + i * 8)
            if a or x:
                out[a] = ((x >> 8) & 0x3FFFFF) * 4
        return out

    def utf16(self, lo=4, hi=200):
        pat = ('(?:[' + chr(32) + '-' + chr(126) + ']' + BS + 'x00){%d,%d}') % (lo, hi)
        return {m.group().decode('utf-16-le')
                for m in re.finditer(pat.encode('ascii'), self.d)}

    def classes(self):
        out = set()
        for m in re.finditer(rb'\.\?A[VU][\w@?$]{1,400}?\x00', self.d):
            n = m.group()[:-1].decode('ascii', 'replace')
            if any(t in n for t in LIBRARY_TAGS):
                continue
            n = n[4:]
            out.add(n[:-2] if n.endswith('@@') else n)
        return out

    def scenes(self):
        return {s for s in self.utf16(4, 120)
                if s.startswith(('xuiscene_', 'CScene_', 'CXuiScene')) or s.endswith('.xur')}

    def imports(self):
        pre = (b'Xam', b'Xe', b'Nt', b'Rtl', b'Ke', b'Ob', b'Mm', b'Db', b'Xbox', b'Xps', b'Vd')
        return {m.group().decode('ascii')
                for m in re.finditer(rb'[A-Za-z_][A-Za-z0-9_]{3,60}', self.d)
                if m.group().startswith(pre)}

    def texts(self):
        """UTF-16 strings that read as real text rather than stray data."""
        ok = set()
        for s in self.utf16(6, 160):
            t = s.strip()
            if len(t) < 6:
                continue
            if sum(c.isalpha() for c in t) * 2 < len(t):
                continue
            ok.add(s)
        return ok

    def toolchain(self):
        xdk = sorted({m.group().decode('ascii', 'replace')
                      for m in re.finditer(rb'xdk[-a-z0-9_]{2,20}', self.d, re.I)})
        pdb = [m.group().decode('ascii', 'replace')
               for m in re.finditer(rb'[A-Za-z0-9_.]{1,40}\.pdb', self.d)]
        return (','.join(x for x in xdk if any(c.isdigit() for c in x)) or '-',
                pdb[0] if pdb else '-')


def is_meta(n):
    """Hand-rolled reflection artefacts, not real types."""
    return n.startswith('Class@') or n.startswith('?$Descriptive')


THEMES = [
    ('AI / mob behaviour',  lambda n: n.endswith(('Goal', 'Control', 'Constraint'))),
    ('Enchanting',          lambda n: 'Enchant' in n),
    ('Brewing / potions',   lambda n: any(k in n for k in ('Brewing', 'Potion', 'Bottle', 'MobEffect'))),
    ('World gen: biomes',   lambda n: 'Biome' in n),
    ('World gen: layers',   lambda n: n.endswith(('Layer', 'Zoom'))),
    ('World gen: features', lambda n: n.endswith(('Feature', 'LevelSource')) or 'Pieces' in n or 'Structure' in n),
    ('Dimensions',          lambda n: 'Dimension' in n or 'Portal' in n),
    ('DLC subsystem',       lambda n: n.startswith('DLC') or 'TexturePack' in n),
    ('Networking',          lambda n: n.endswith('Packet') or 'Net' in n),
    ('Rendering / models',  lambda n: n.endswith(('Renderer', 'Model'))),
    ('Textures / icons',    lambda n: 'Texture' in n or 'Icon' in n),
    ('Particles',           lambda n: n.endswith('Particle')),
    ('Blocks / tiles',      lambda n: n.endswith(('Tile', 'TileEntity'))),
    ('Items',               lambda n: n.endswith('Item')),
    ('UI scenes / menus',   lambda n: n.startswith(('CScene_', 'CXui', 'IUIScene', 'xuiscene_'))
                                      or n.endswith(('Menu', 'Screen', '.xur'))),
    ('Containers / slots',  lambda n: n.endswith(('Container', 'Slot'))),
    ('Commands',            lambda n: n.endswith(('Command', 'Dispatcher'))),
    ('Game rules',          lambda n: 'GameRule' in n or 'Ruleset' in n or 'RuleDefinition' in n),
    ('Trading',             lambda n: 'Merchant' in n or 'Villager' in n or n == 'Villages'),
    ('Platform / console',  lambda n: n.startswith(('Console', 'CPlatform', 'Xbox', 'CXbox'))
                                      or 'Leaderboard' in n),
    ('Game modes',          lambda n: n.endswith('Mode')),
]


def categorise(names):
    seen, out = set(), collections.OrderedDict()
    for label, test in THEMES:
        grp = sorted(n for n in names if n not in seen and test(n))
        seen |= set(grp)
        if grp:
            out[label] = grp
    rest = sorted(n for n in names if n not in seen)
    if rest:
        out['(uncategorised)'] = rest
    return out


def show_groups(title, names, limit, show_all):
    print('')
    print('=== %s: %d ===' % (title, len(names)))
    for label, grp in categorise(names).items():
        print('  %-22s %5d' % (label, len(grp)))
        for n in (grp if show_all else grp[:limit]):
            print('        ' + n)
        if not show_all and len(grp) > limit:
            print('        ... %d more' % (len(grp) - limit))


EXTRACTORS = {
    'classes': lambda im: im.classes(),
    'scenes':  lambda im: im.scenes(),
    'imports': lambda im: im.imports(),
    'strings': lambda im: im.texts(),
}


def emit(a, obj):
    if getattr(a, 'json', None):
        json.dump(obj, open(a.json, 'w'), indent=1)
        print('')
        print('[json] -> %s' % a.json)
    if getattr(a, 'csv', None):
        rows = obj if isinstance(obj, list) else [obj]
        flat = [{k: v for k, v in r.items() if not isinstance(v, (list, dict))} for r in rows]
        if flat:
            with open(a.csv, 'w', newline='') as f:
                w = csv.DictWriter(f, fieldnames=sorted(flat[0]))
                w.writeheader()
                w.writerows(flat)
            print('[csv]  -> %s' % a.csv)


def cmd_set(a, kind):
    get = EXTRACTORS[kind]
    if len(a.images) == 1:
        im = Image(a.images[0])
        s = get(im)
        real = {n for n in s if not is_meta(n)}
        print('%s: %d %s (%d real, %d reflection)'
              % (im.name, len(s), kind, len(real), len(s) - len(real)))
        show_groups('inventory', real, a.limit, a.all)
        emit(a, {'image': im.name, kind: sorted(s)})
        return
    old, new = Image(a.images[0]), Image(a.images[1])
    A, B = get(old), get(new)
    added, removed, common = B - A, A - B, A & B
    m_rm = {n for n in removed if is_meta(n)}
    m_ad = {n for n in added if is_meta(n)}
    print('OLD %-30s %d' % (old.name, len(A)))
    print('NEW %-30s %d' % (new.name, len(B)))
    print('  unchanged : %d  (%.1f%% of old survive)'
          % (len(common), 100 * len(common) / max(len(A), 1)))
    print('  added     : %d  (%d real, %d reflection)'
          % (len(added), len(added) - len(m_ad), len(m_ad)))
    print('  removed   : %d  (%d real, %d reflection)'
          % (len(removed), len(removed) - len(m_rm), len(m_rm)))
    if m_rm:
        tgt = [n[6:] for n in m_rm if n.startswith('Class@')]
        kept = [t for t in tgt if t in B or t.split('@')[0] in B]
        print('  NOTE: %d/%d removed Class@X still have X present -- reflection'
              ' layer dropped, types kept' % (len(kept), len(tgt)))
    show_groups('ADDED', added - m_ad, a.limit, a.all)
    show_groups('REMOVED', removed - m_rm, a.limit, a.all)
    emit(a, {'old': old.name, 'new': new.name, 'added': sorted(added),
             'removed': sorted(removed), 'unchanged': len(common)})


def cmd_info(a):
    rows = []
    print('%-26s %11s %10s %8s %7s %7s  %-14s %s'
          % ('image', 'size', '.text', 'funcs', 'cls', 'real', 'xdk', 'pdb'))
    for p in a.images:
        im = Image(p)
        cl = im.classes()
        real = {c for c in cl if not is_meta(c)}
        t = im.sec('.text')
        xdk, pdb = im.toolchain()
        r = dict(image=im.name, size=len(im.d), text=t[2] if t else 0,
                 funcs=len(im.functions()), classes=len(cl), real=len(real),
                 xdk=xdk, pdb=pdb, entry=im.entry)
        rows.append(r)
        print('%-26s %11d %10X %8d %7d %7d  %-14s %s'
              % (r['image'], r['size'], r['text'], r['funcs'], r['classes'],
                 r['real'], xdk, pdb))
    emit(a, rows)


def cmd_funcs(a):
    old, new = Image(a.images[0]), Image(a.images[1])
    fa, fb = old.functions(), new.functions()
    ta, tb = old.sec('.text'), new.sec('.text')
    sh = set(fa) & set(fb)
    same = diff = 0
    changed = []
    for va in sh:
        if fa[va] != fb[va]:
            diff += 1
            continue
        x, y = old.v2o(va), new.v2o(va)
        if x is None or y is None:
            continue
        if old.d[x:x + fa[va]] == new.d[y:y + fb[va]]:
            same += 1
        else:
            diff += 1
            changed.append((va, fa[va]))
    print('%s -> %s' % (old.name, new.name))
    print('  .text base   : 0x%08X -> 0x%08X %s'
          % (old.base + ta[1], new.base + tb[1], '(same)' if ta[1] == tb[1] else '(MOVED)'))
    print('  functions    : %d -> %d (%+d)' % (len(fa), len(fb), len(fb) - len(fa)))
    print('  shared addrs : %d (%.1f%% of old)' % (len(sh), 100 * len(sh) / max(len(fa), 1)))
    pct = 100 * same / max(same + diff, 1)
    if sh:
        print('  of shared    : %d byte-identical, %d changed (%.1f%%)' % (same, diff, pct))
        if pct < 50:
            print('  VERDICT: separate compiles -- function diffing is not meaningful')
            print('           here. Use `classes` / `scenes` instead.')
        else:
            print('  VERDICT: same build lineage -- the differences below are real edits.')
            for va, sz in sorted(changed)[:a.limit]:
                print('      0x%08X  size=%d' % (va, sz))
    n = min(len(old.d), len(new.d))
    rawd = sum(1 for i in range(n) if old.d[i] != new.d[i])
    print('  raw bytes differing: %d / %d (%.3f%%)' % (rawd, n, 100 * rawd / n))
    emit(a, dict(old=old.name, new=new.name, shared=len(sh), identical=same,
                 changed=diff, raw_diff=rawd))


def cmd_timeline(a):
    ims = [Image(p) for p in a.images]
    labels = [i.name for i in ims]
    out = {}
    for kind in ('classes', 'scenes', 'imports'):
        sets = [{n for n in EXTRACTORS[kind](i) if not is_meta(n)} for i in ims]
        core = set.intersection(*sets)
        ever = set.union(*sets)
        first = collections.Counter()
        for n in ever:
            for l, s in zip(labels, sets):
                if n in s:
                    first[l] += 1
                    break
        print('')
        print('### %s' % kind)
        print('  per build      : %s' % ', '.join('%s=%d' % (l, len(s))
                                                  for l, s in zip(labels, sets)))
        print('  present in ALL : %d  (%.1f%% of first, %.1f%% of last)'
              % (len(core), 100 * len(core) / max(len(sets[0]), 1),
                 100 * len(core) / max(len(sets[-1]), 1)))
        print('  ever seen      : %d' % len(ever))
        print('  first appearance:')
        for l in labels:
            print('      %-28s %5d' % (l, first[l]))
        out[kind] = dict(core=sorted(core),
                         per_build={l: len(s) for l, s in zip(labels, sets)},
                         first={l: first[l] for l in labels})
    emit(a, out)


def _show(bs):
    try:
        t = bs.decode('utf-16-le')
        if t and all(c == '\0' or 32 <= ord(c) < 127 for c in t):
            return repr(t.replace('\0', '.'))
    except Exception:
        pass
    return bs.hex()


def cmd_patchdiff(a):
    A, B = Image(a.images[0]).d, Image(a.images[1]).d
    d = [i for i in range(min(len(A), len(B))) if A[i] != B[i]]
    if not d:
        print('images are identical')
        return
    runs, s, p = [], d[0], d[0]
    for i in d[1:]:
        if i - p <= MAXGAP:
            p = i
            continue
        runs.append((s, p))
        s = p = i
    runs.append((s, p))
    edits = []
    for s, e in runs:
        lo, hi = max(0, s - CTX), min(len(A), e + 1 + CTX)
        before, after, old, new = A[lo:s], A[e + 1:hi], A[s:e + 1], B[s:e + 1]
        if A.count(before + old + after) != 1:
            print('warning: anchor at 0x%X is not unique -- patch may not port' % s)
        edits.append(dict(offset_hint=s, before=before.hex(), old=old.hex(),
                          new=new.hex(), after=after.hex()))
    out = a.out or 'patch.json'
    json.dump(dict(edits=edits), open(out, 'w'), indent=1)
    print('%d edit region(s), %d bytes -> %s'
          % (len(edits), sum(len(bytes.fromhex(e['old'])) for e in edits), out))
    for e in edits:
        print('   0x%08X  %-30s -> %s'
              % (e['offset_hint'], _show(bytes.fromhex(e['old'])),
                 _show(bytes.fromhex(e['new']))))


def cmd_patchapply(a):
    img = bytearray(open(a.images[0], 'rb').read())
    patch = json.load(open(a.images[1]))
    applied = skipped = 0
    for e in patch['edits']:
        before, old = bytes.fromhex(e['before']), bytes.fromhex(e['old'])
        new, after = bytes.fromhex(e['new']), bytes.fromhex(e['after'])
        hits = [m.start() for m in re.finditer(re.escape(before + old + after), bytes(img))]
        if len(hits) == 1:
            at = hits[0] + len(before)
            img[at:at + len(old)] = new
            applied += 1
            print('   applied at 0x%08X  %s -> %s' % (at, _show(old), _show(new)))
        elif not hits and (before + new + after) in bytes(img):
            print('   already applied: %s' % _show(new))
            skipped += 1
        elif not hits:
            print('   NOT FOUND: %s' % _show(old))
            skipped += 1
        else:
            print('   AMBIGUOUS (%d matches): %s' % (len(hits), _show(old)))
            skipped += 1
    out = a.out or (a.images[0] + '.patched')
    open(out, 'wb').write(img)
    print('%d applied, %d skipped -> %s' % (applied, skipped, out))


def main():
    ap = argparse.ArgumentParser(prog='retool', description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('cmd', choices=['info', 'classes', 'scenes', 'imports', 'strings',
                                    'funcs', 'timeline', 'patchdiff', 'patchapply'])
    ap.add_argument('images', nargs='+')
    ap.add_argument('--json')
    ap.add_argument('--csv')
    ap.add_argument('-o', dest='out')
    ap.add_argument('--limit', type=int, default=5)
    ap.add_argument('--all', action='store_true')
    a = ap.parse_args()

    if a.cmd in EXTRACTORS:
        cmd_set(a, a.cmd)
    else:
        {'info': cmd_info, 'funcs': cmd_funcs, 'timeline': cmd_timeline,
         'patchdiff': cmd_patchdiff, 'patchapply': cmd_patchapply}[a.cmd](a)


if __name__ == "__main__":
    main()
