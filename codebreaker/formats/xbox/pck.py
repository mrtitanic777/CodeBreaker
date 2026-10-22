"""Minecraft LCE .pck container parser / extractor.

Layout (all integers big-endian, all names UTF-16BE):

    uint32 version                       observed: 3
    uint32 property_count
      property_count x {
          uint32 id
          uint32 name_len                in UTF-16 code units
          char16 name[name_len]
          uint32 zero
      }
    uint32 entry_count
      entry_count x {
          uint32 size                    payload length in bytes
          uint32 type                    content-type tag
          uint32 name_len
          char16 name[name_len]
          uint32 zero
      }
    payload blobs, in table order

Tutorial.pck has property_count = 0, which is why a parser that assumes the
property table is absent still appears to work on it.
"""

DESCRIPTION = "unpack Minecraft LCE .pck containers (PNG/loc/grf/col payloads)"
import struct, sys, os

def _rec(d, p):
    """Read {uint32 a, uint32 len, utf16be name, uint32 zero} at p."""
    a, nl = struct.unpack_from('>II', d, p)
    p += 8
    if nl > 8192 or p + nl * 2 > len(d):
        raise ValueError('bad name length %d at 0x%X' % (nl, p))
    name = d[p:p + nl * 2].decode('utf-16-be', 'replace')
    p += nl * 2 + 4
    return a, name, p

def parse(path):
    d = open(path, 'rb').read()
    ver, nprop = struct.unpack_from('>II', d, 0)
    p = 8
    props = []
    for _ in range(nprop):
        pid, name, p = _rec(d, p)
        props.append((pid, name))
    (ncount,) = struct.unpack_from('>I', d, p)
    p += 4
    entries = []
    for _ in range(ncount):
        size, typ = struct.unpack_from('>II', d, p)
        (nl,) = struct.unpack_from('>I', d, p + 8)
        q = p + 12
        name = d[q:q + nl * 2].decode('utf-16-be', 'replace')
        p = q + nl * 2 + 4
        entries.append(dict(size=size, type=typ, name=name))
    return dict(version=ver, props=props, entries=entries, table_end=p, data=d)

def payloads(path):
    """Every payload as (name, bytes), and where reading stopped. -> ([(name, blob)], end, file size)

    Entry sizes in the table are exact (verified 122/122 against PNG spans),
    but payloads are separated by variable padding -- observed 4, 16 and 316
    bytes, not a simple alignment.  So rather than predict the padding, walk
    forward from the previous payload and skip the gap: PNG payloads are
    located by signature, everything else by skipping NUL padding.
    """
    r = parse(path)
    d, p = r['data'], r['table_end'] + 12      # 12-byte gap before first payload
    PNG = bytes.fromhex('89504e470d0a1a0a')
    out = []
    for e in r['entries']:
        if not e['size']:
            continue
        if e['name'].lower().endswith('.png'):
            at = d.find(PNG, p)                # exact: PNG is self-delimiting
            if at < 0:
                continue
        else:
            at = p
            while at < len(d) and d[at] == 0 and at - p < 512 and p != r['table_end'] + 12:
                at += 1
        blob = d[at:at + e['size']]
        p = at + e['size']
        safe = e['name'].replace('/', '_').replace(chr(92), '_').strip(chr(0))
        out.append((safe or ('entry%d' % len(out)), blob))
    return out, p, len(d)


def extract(path, outdir):
    """Write every payload under outdir. -> (files written, end, file size)"""
    found, end, size = payloads(path)
    os.makedirs(outdir, exist_ok=True)
    for name, blob in found:
        open(os.path.join(outdir, name), 'wb').write(blob)
    return len(found), end, size

if __name__ == '__main__':
    args = [a for a in sys.argv[1:] if not a.startswith('-x')]
    doext = any(a.startswith('-x') for a in sys.argv[1:])
    for path in args:
        try:
            r = parse(path)
        except Exception as ex:
            print('%s: PARSE FAILED (%s)' % (os.path.basename(path), ex)); continue
        tot = sum(e['size'] for e in r['entries'])
        print('%s' % os.path.basename(path))
        print('  version=%d  properties=%s  entries=%d'
              % (r['version'], r['props'] or '-', len(r['entries'])))
        print('  table_end=0x%X  sum(sizes)=%d  table+payload=%d  file=%d  delta=%d'
              % (r['table_end'], tot, r['table_end'] + tot, len(r['data']),
                 len(r['data']) - (r['table_end'] + tot)))
        byt = {}
        for e in r['entries']:
            byt[e['type']] = byt.get(e['type'], 0) + 1
        print('  type histogram: %s' % ', '.join('t%d x%d' % (k, v) for k, v in sorted(byt.items())))
        for e in r['entries'][:10]:
            print('     %-9d t%-3d %s' % (e['size'], e['type'], e['name']))
        if len(r['entries']) > 10:
            print('     ... %d more' % (len(r['entries']) - 10))
        if doext:
            outdir = os.path.splitext(path)[0] + '_extracted'
            n, end, sz = extract(path, outdir)
            print('  extracted %d files -> %s  (consumed %d of %d)' % (n, outdir, end, sz))
        print()
