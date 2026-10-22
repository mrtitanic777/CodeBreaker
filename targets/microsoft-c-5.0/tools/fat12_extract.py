import struct, os, sys, json

def parse(img):
    d = open(img, 'rb').read()
    bps = struct.unpack('<H', d[11:13])[0]
    spc = d[13]
    rsvd = struct.unpack('<H', d[14:16])[0]
    nfat = d[16]
    nroot = struct.unpack('<H', d[17:19])[0]
    spf = struct.unpack('<H', d[22:24])[0]
    fat_start = rsvd * bps
    fat = d[fat_start: fat_start + spf * bps]
    root_start = (rsvd + nfat * spf) * bps
    root_sectors = (nroot * 32 + bps - 1) // bps
    data_start = root_start + root_sectors * bps

    def fat12(n):
        off = n + n // 2
        v = fat[off] | (fat[off+1] << 8)
        return (v >> 4) if (n & 1) else (v & 0xFFF)

    def read_chain(start, size):
        out = b''
        c = start
        while 0x002 <= c < 0xFF0:
            off = data_start + (c - 2) * spc * bps
            out += d[off: off + spc * bps]
            c = fat12(c)
        return out[:size]

    def read_dir(entries_bytes, prefix):
        out = []
        n = len(entries_bytes) // 32
        for i in range(n):
            e = entries_bytes[i*32: i*32+32]
            if e[0] == 0x00: break
            if e[0] == 0xE5: continue
            attr = e[11]
            if attr & 0x08:  # volume label
                continue
            name = e[0:8].decode('latin1').rstrip()
            ext = e[8:11].decode('latin1').rstrip()
            fn = name + ('.' + ext if ext else '')
            start = struct.unpack('<H', e[26:28])[0]
            size = struct.unpack('<I', e[28:32])[0]
            if attr & 0x10:  # directory
                if name in ('.', '..'):
                    continue
                subraw = read_chain(start, spc*bps*4096)  # generous; chain-terminated
                out += read_dir(subraw, prefix + fn + '/')
            else:
                out.append((prefix + fn, size, read_chain(start, size)))
        return out

    root_bytes = d[root_start: root_start + nroot*32]
    return read_dir(root_bytes, '')

def sig(data):
    if data[:2] == b'MZ': return 'MZ-exe'
    if data[:2] == b'ZM': return 'ZM-exe'
    if data[:8] == b'!<arch>\n' or data[:8].startswith(b'!<arch'): return 'ar-lib'
    # OMF lib: first byte 0xF0 (LIBHDR)
    if data and data[0] == 0xF0: return 'OMF-lib'
    if data and data[0] == 0x80: return 'OMF-obj(THEADR)'
    return ''

if __name__ == '__main__':
    src = sys.argv[1]
    out = sys.argv[2]
    catalog = {}
    for img in sorted(os.listdir(src)):
        if not img.lower().endswith('.img'): continue
        disk = os.path.splitext(img)[0]
        files = parse(os.path.join(src, img))
        os.makedirs(os.path.join(out, disk), exist_ok=True)
        entries = []
        for fn, size, data in files:
            dst = os.path.join(out, disk, fn)
            os.makedirs(os.path.dirname(dst), exist_ok=True)
            with open(dst, 'wb') as f:
                f.write(data)
            entries.append({'name': fn, 'size': size, 'sig': sig(data)})
        catalog[disk] = entries
    print(json.dumps(catalog, indent=1))
