"""XUIZ (.xzp) container parser / extractor -- Xbox 360 UI / texture packs.

Two layouts share the 'XUIZ' magic, told apart by the version word at 0x04:

  v1  Minecraft LCE:                       v3  Xbox dashboard / XUI:
    0x00 'XUIZ'                              0x00 'XUIZ'
    0x04 uint32be version = 1                0x04 uint32be version = 3
    0x08 uint32be file_size                  0x08 uint32be file_size
    0x0C uint32be ? (1)                       0x0C uint32be ? (0)
    0x10 uint32be base_rel (rel 0x16)         0x10 uint32be base_rel (rel 0x16)
    0x14 uint16   ?                           0x14 uint16   entry_count
    0x17 table (count implied):               0x16 table (entry_count entries):
       uint24be size                             uint32be size
       uint32be offset (rel 0x16+base_rel)       uint32be offset (rel 0x16+base_rel)
       uint16le name_len (UTF-16 units)          uint8    name_len (bytes)
       char16le name  (LITTLE-endian)            char     name  (latin-1)
    payloads                                  payloads

Both put the payload base at 0x16 + (uint32be @0x10). v1 has a one-byte skew
(table @0x17, offsets @0x16) that must be preserved; v3 has no skew and an
explicit entry count. v3 names carry backslash paths with leading '..\\' -- the
extractor strips '..'/absolute components so extraction can't escape the outdir.

Verified: v1 Minecraft TexturePack.xzp (PNG sig + IEND exact); v3 dashboard
packs controlp/xam/skin/gamercrd (18/29/33/9 PNG signatures all valid).
"""

DESCRIPTION = "unpack XUIZ .xzp containers (Minecraft v1 + dashboard v3)"
import os, sys, struct

from codebreaker.formats import native

BASE_REL = 0x16     # payload base is relative to this
TBL = 0x17          # entry table starts here
PNG = bytes.fromhex('89504e470d0a1a0a')
IEND = bytes.fromhex('49454e44ae426082')


# XUIZ comes in two layouts, distinguished by the version word at 0x04:
#   v1  Minecraft LCE      table @0x17, [u24be size][u32be off][u16le nlen][utf16le name]
#   v3  Xbox dashboard/XUI table @0x16, [u32be size][u32be off][u8 nlen][latin1 name],
#                          with an explicit u16 entry count at 0x14
# Both put the payload base at 0x16 + (u32be @0x10).
def parse(path):
    d = path if isinstance(path, (bytes, bytearray)) else open(path, 'rb').read()
    r = native.call("xzp.parse", d, path if isinstance(path, str) else '<bytes>')
    r['data'] = d
    return r


def verify(r):
    d, base = r['data'], r['base']
    png = [e for e in r['entries'] if e['name'].lower().endswith('.png')]
    hit = sum(1 for e in png if d[base + e['offset']:base + e['offset'] + 8] == PNG)
    tail = sum(1 for e in png
               if d[base + e['offset'] + e['size'] - 8:base + e['offset'] + e['size']] == IEND)
    return len(png), hit, tail


def payloads(path):
    """Every stored file as (relative path, bytes), names made safe (no '..', no absolute parts)."""
    d = path if isinstance(path, (bytes, bytearray)) else open(path, 'rb').read()
    return native.call("xzp.payloads", d, path if isinstance(path, str) else '<bytes>')


def extract(path, outdir):
    n = 0
    for rel, blob in payloads(path):
        dest = os.path.join(outdir, rel)
        os.makedirs(os.path.dirname(dest) or outdir, exist_ok=True)
        with open(dest, 'wb') as f:
            f.write(blob)
        n += 1
    return n


def build(folder, out_path, template):
    """Pack a folder of files into a XUIZ (.xzp) container. Clones the 0x16-byte
    header from a template .xzp (preserving its unknown fields), then rebuilds
    the entry table and payloads. Inverse of parse()."""
    tpl = template if isinstance(template, (bytes, bytearray)) else open(template, 'rb').read()
    if tpl[:4] != b'XUIZ':
        raise ValueError('template is not a XUIZ container')
    hdr = bytearray(tpl[:TBL])                           # 0x17: header incl. the skew byte
    # Recurse: v1 Minecraft entry names are nested, backslash-separated and
    # carry their directory prefix (e.g. 'media\\skin_Minecraft.xur',
    # 'media\\de-DE\\skin_Minecraft.xus'). Name each entry by its path relative
    # to `folder`, with '\\' separators, so packing folder=".../XZP" reproduces
    # the 'media\\...' layout the game loads.
    files = []
    for root, _dirs, fnames in os.walk(folder):
        for fn in sorted(fnames):
            fp = os.path.join(root, fn)
            rel = os.path.relpath(fp, folder).replace('/', '\\').replace(os.sep, '\\')
            files.append((rel, open(fp, 'rb').read()))
    files.sort(key=lambda x: x[0].lower())
    table = bytearray()
    payloads = bytearray()
    offset = 0
    for nm, data in files:
        name = nm.encode('utf-16-le')
        table += int(len(data)).to_bytes(3, 'big')     # uint24be size
        table += int(offset).to_bytes(4, 'big')         # uint32be offset
        table += struct.pack('<H', len(name) // 2)       # uint16le name length
        table += name                                    # utf-16le name
        payloads += data
        offset += len(data)
    base_rel = (TBL + len(table)) - BASE_REL
    struct.pack_into('>I', hdr, 0x10, base_rel)
    out = bytearray(hdr) + table + payloads
    struct.pack_into('>I', out, 0x08, len(out))          # file_size
    open(out_path, 'wb').write(bytes(out))
    return out_path


if __name__ == '__main__':
    if sys.argv[1:2] == ['pack']:
        if len(sys.argv) < 5:
            print('usage: xzp pack <folder> <out.xzp> <template.xzp>'); sys.exit(2)
        out = build(sys.argv[2], sys.argv[3], sys.argv[4])
        r = parse(out)
        print('packed %d entries -> %s' % (len(r['entries']), out))
        sys.exit(0)
    args = [a for a in sys.argv[1:] if a != '-x']
    doext = '-x' in sys.argv[1:]
    for path in args:
        r = parse(path)
        npng, hit, tail = verify(r)
        used = max((r['base'] + e['offset'] + e['size'] for e in r['entries']), default=0)
        print(os.path.basename(path))
        print('  version=%d  declared_size=%d  actual=%d  %s'
              % (r['version'], r['file_size'], len(r['data']),
                 'OK' if r['file_size'] == len(r['data']) else 'MISMATCH'))
        print('  entries=%d  table_end=0x%X  payload_base=0x%X'
              % (len(r['entries']), r['table_end'], r['base']))
        print('  png payloads: %d   valid signature: %d   valid IEND: %d' % (npng, hit, tail))
        print('  consumed %d of %d bytes (%d trailing)' % (used, len(r['data']), len(r['data']) - used))
        for e in r['entries'][:6]:
            print('     %-9d @%-9d %s' % (e['size'], e['offset'], e['name']))
        if len(r['entries']) > 6:
            print('     ... %d more' % (len(r['entries']) - 6))
        if doext:
            out = os.path.splitext(path)[0] + '_extracted'
            print('  extracted %d files -> %s' % (extract(path, out), out))
        print('')
