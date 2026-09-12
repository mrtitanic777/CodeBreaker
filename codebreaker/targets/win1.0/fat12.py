#!/usr/bin/env python3
"""Minimal FAT12 floppy-image reader/extractor. Written for the Win1.0 RE project."""
import struct, sys, os

class FAT12:
    def __init__(self, path):
        self.data = open(path, 'rb').read()
        bs = self.data
        # BIOS Parameter Block
        self.oem = bs[3:11].decode('latin1').strip()
        self.bytes_per_sector    = struct.unpack('<H', bs[11:13])[0]
        self.sectors_per_cluster = bs[13]
        self.reserved_sectors    = struct.unpack('<H', bs[14:16])[0]
        self.num_fats            = bs[16]
        self.root_entries        = struct.unpack('<H', bs[17:19])[0]
        self.total_sectors       = struct.unpack('<H', bs[19:21])[0]
        self.media               = bs[21]
        self.sectors_per_fat     = struct.unpack('<H', bs[22:24])[0]
        self.sectors_per_track   = struct.unpack('<H', bs[24:26])[0]
        self.num_heads           = struct.unpack('<H', bs[26:28])[0]

        self.fat_start   = self.reserved_sectors * self.bytes_per_sector
        self.root_start  = self.fat_start + self.num_fats * self.sectors_per_fat * self.bytes_per_sector
        self.root_bytes  = self.root_entries * 32
        self.data_start  = self.root_start + self.root_bytes
        self._load_fat()

    def _load_fat(self):
        fat = self.data[self.fat_start:self.fat_start + self.sectors_per_fat*self.bytes_per_sector]
        self.fat = fat

    def fat_entry(self, cluster):
        # 12-bit packed
        off = (cluster * 3) // 2
        val = self.fat[off] | (self.fat[off+1] << 8)
        if cluster & 1:
            return val >> 4
        return val & 0x0FFF

    def cluster_bytes(self, cluster):
        cs = self.sectors_per_cluster * self.bytes_per_sector
        start = self.data_start + (cluster - 2) * cs
        return self.data[start:start+cs]

    def read_chain(self, first_cluster, size):
        out = b''
        c = first_cluster
        while 2 <= c < 0xFF0:
            out += self.cluster_bytes(c)
            c = self.fat_entry(c)
        return out[:size]

    def _parse_dir(self, raw, path_prefix=''):
        entries = []
        for i in range(0, len(raw), 32):
            e = raw[i:i+32]
            if len(e) < 32: break
            first = e[0]
            if first == 0x00: break        # no more entries
            if first == 0xE5: continue      # deleted
            attr = e[11]
            if attr == 0x0F: continue       # LFN entry, skip
            name = e[0:8].decode('latin1').rstrip()
            ext  = e[8:11].decode('latin1').rstrip()
            if first == 0x2E: # '.' or '..'
                fname = e[0:11].decode('latin1').rstrip()
            else:
                fname = name + ('.'+ext if ext else '')
            cluster = struct.unpack('<H', e[26:28])[0]
            size    = struct.unpack('<I', e[28:32])[0]
            is_dir  = bool(attr & 0x10)
            is_vol  = bool(attr & 0x08)
            entries.append(dict(name=fname, attr=attr, cluster=cluster,
                                size=size, is_dir=is_dir, is_vol=is_vol))
        return entries

    def root_dir(self):
        raw = self.data[self.root_start:self.root_start+self.root_bytes]
        return self._parse_dir(raw)

    def subdir(self, cluster):
        raw = self.read_chain(cluster, 0x10000)
        # trim at first 0x00 dir entry handled in parse
        return self._parse_dir(raw)

    def info(self):
        print(f"OEM             : {self.oem!r}")
        print(f"Bytes/sector    : {self.bytes_per_sector}")
        print(f"Sectors/cluster : {self.sectors_per_cluster}")
        print(f"Reserved sectors: {self.reserved_sectors}")
        print(f"FAT count       : {self.num_fats}")
        print(f"Root entries    : {self.root_entries}")
        print(f"Total sectors   : {self.total_sectors}  ({self.total_sectors*self.bytes_per_sector} bytes)")
        print(f"Sectors/FAT     : {self.sectors_per_fat}")
        print(f"Media descriptor: 0x{self.media:02X}")

def listing(img):
    fs = FAT12(img)
    print(f"\n########## {os.path.basename(img)} ##########")
    fs.info()
    print("\n--- ROOT ---")
    def show(entries, fs, depth=0, extract_to=None, path=''):
        for e in entries:
            if e['is_vol']:
                print("  "*depth + f"[VOL] {e['name']}")
                continue
            if e['name'] in ('.','..'): continue
            tag = 'DIR ' if e['is_dir'] else f"{e['size']:>7}"
            print("  "*depth + f"{tag}  {e['name']}")
            if e['is_dir']:
                show(fs.subdir(e['cluster']), fs, depth+1, extract_to,
                     path+e['name']+'/')
            elif extract_to is not None:
                data = fs.read_chain(e['cluster'], e['size'])
                op = os.path.join(extract_to, path, e['name'])
                os.makedirs(os.path.dirname(op), exist_ok=True)
                open(op,'wb').write(data)
    show(fs.root_dir(), fs, extract_to=(sys.argv[2] if len(sys.argv)>2 else None))

if __name__ == '__main__':
    listing(sys.argv[1])
