#!/usr/bin/env python3
"""XDBF / GPD — Xbox 360 database format reader (gamer profiles, achievements).

XDBF (Xbox DataBase Format) backs .gpd gamer-profile files, PEC, and dashboard
databases: a header, an entry table, a free-space table, then a data heap.
Entries are keyed by (namespace, id); the namespace says what the payload is
(1 achievements, 2 images, 3 settings, 4 title/games-played, 5 strings,
6 avatar awards). This is a clean-room Python reader written to the public/
documented format — the profile-data half of what the GPL X360 library does,
as original code.

Commands:
    xdbf info    <file.gpd>          header + per-namespace entry counts
    xdbf list    <file.gpd>          every entry (namespace, id, size)
    xdbf ach     <file.gpd>          decode the achievement records
    xdbf extract <file.gpd> [outdir] dump every entry's raw payload
"""
DESCRIPTION = "read Xbox 360 XDBF/GPD (profiles, achievements): info/list/ach/extract"

import os
import struct
import sys

from codebreaker.formats import native

MAGIC = b"XDBF"
NS = {1: "achievement", 2: "image", 3: "setting", 4: "title",
      5: "string", 6: "avatar-award", 7: "avatar-award-1"}


class XDBF:
    def __init__(self, data):
        self.d = data
        (self.version, self.entry_table_len, self.entry_count, self.free_table_len, self.free_count,
         self.entries) = native.call("xdbf.parse", data)          # entries: (namespace, id, off, length)
        self.data_start = 0x18 + self.entry_table_len * 0x12 + self.free_table_len * 8

    def payload(self, ns, eid):
        for n, i, off, length in self.entries:
            if n == ns and i == eid:
                start = self.data_start + off
                return self.d[start:start + length]
        return None

    def _entry_bytes(self, off, length):
        start = self.data_start + off
        return self.d[start:start + length]

    def achievements(self):
        """Decode namespace-1 achievement records. Each record: fixed fields
        then a UTF-16BE name + unlocked-description + locked-description."""
        return native.call("xdbf.achievements", self.d, self.data_start,
                           [(ns, off, length) for ns, _eid, off, length in self.entries])

    def counts(self):
        c = {}
        for ns, _, _, _ in self.entries:
            c[ns] = c.get(ns, 0) + 1
        return c

    # ---- write side (edit + rebuild) ----------------------------------
    def _payload_map(self):
        """Current payload for every (ns, id), honouring edits made so far."""
        m = {}
        for ns, eid, off, length in self.entries:
            m[(ns, eid)] = getattr(self, "_ov", {}).get((ns, eid),
                                                         self._entry_bytes(off, length))
        return m

    def set_payload(self, ns, eid, data):
        """Replace (or add) the payload for an entry; rebuild() serialises it."""
        if not hasattr(self, "_ov"):
            self._ov = {}
        self._ov[(ns, eid)] = bytes(data)
        if not any(n == ns and i == eid for n, i, _o, _l in self.entries):
            self.entries.append((ns, eid, 0, len(data)))

    # FILETIME for a fixed, plausible unlock date (2015-06-15 12:00 UTC).
    UNLOCK_FILETIME = 130788648000000000

    def unlock_achievement(self, ach_id, unlock=True):
        """Set/clear the 'achieved' flag (0x20000) and stamp the unlock time on
        the namespace-1 record whose achievement id == ach_id."""
        for ns, eid, off, length in self.entries:
            if ns != 1:
                continue
            b = bytearray(getattr(self, "_ov", {}).get((ns, eid),
                                                        self._entry_bytes(off, length)))
            if len(b) < 0x1C or struct.unpack_from(">I", b, 4)[0] != ach_id:
                continue
            flags = struct.unpack_from(">I", b, 0x10)[0]
            flags = (flags | 0x20000) if unlock else (flags & ~0x20000)
            struct.pack_into(">I", b, 0x10, flags)
            struct.pack_into(">Q", b, 0x14, self.UNLOCK_FILETIME if unlock else 0)
            self.set_payload(ns, eid, bytes(b))
            return True
        return False

    def unlock_all(self):
        n = 0
        for a in self.achievements():
            if self.unlock_achievement(a["id"], True):
                n += 1
        return n

    def build(self):
        """Serialise back to a valid XDBF byte stream (compacted heap). Entry
        order is preserved; edited/new payloads are honoured."""
        pm = self._payload_map()
        return native.call("xdbf.build", None, self.version, self.entry_table_len, self.free_table_len,
                           [(ns, eid.to_bytes(8, "big"), pm[(ns, eid)]) for ns, eid, _off, _length in self.entries])

    def extract_all(self, outdir):
        os.makedirs(outdir, exist_ok=True)
        for ns, eid, off, length in self.entries:
            name = "%s_%016X.bin" % (NS.get(ns, "ns%d" % ns), eid)
            with open(os.path.join(outdir, name), "wb") as f:
                f.write(self._entry_bytes(off, length))
        return len(self.entries)


# ------------------------------------------------------------------------ CLI
def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: xdbf {info|list|ach|extract} <file.gpd> [outdir]\n"
              "       xdbf unlock <file.gpd> <ach_id> [out.gpd]\n"
              "       xdbf unlock-all <file.gpd> [out.gpd]\n"
              "       xdbf set <file.gpd> <ns> <id> <payload.bin> [out.gpd]")
        return
    op = a[0]
    if len(a) < 2:
        print("usage: xdbf %s <file.gpd>" % op); sys.exit(2)
    x = XDBF(open(a[1], "rb").read())
    if op == "info":
        print("file        : %s" % os.path.basename(a[1]))
        print("version     : 0x%X" % x.version)
        print("entries     : %d (table %d)  free: %d"
              % (x.entry_count, x.entry_table_len, x.free_count))
        print("data start  : 0x%X" % x.data_start)
        print("by namespace:")
        for ns, n in sorted(x.counts().items()):
            print("   %-14s %d" % (NS.get(ns, "ns%d" % ns), n))
    elif op == "list":
        for ns, eid, off, length in x.entries:
            print("  %-14s id=%016X  %8d bytes" % (NS.get(ns, "ns%d" % ns), eid, length))
        print("total: %d entries" % len(x.entries))
    elif op == "ach":
        ach = x.achievements()
        for a2 in ach:
            mark = "*" if a2["unlocked"] else " "
            print("  [%s] %5dG  %-40s | %s"
                  % (mark, a2["score"], a2["name"][:40], a2["unlocked_desc"][:50]))
        print("total: %d achievements, %d unlocked, %dG"
              % (len(ach), sum(1 for a2 in ach if a2["unlocked"]),
                 sum(a2["score"] for a2 in ach)))
    elif op == "extract":
        outdir = a[2] if len(a) > 2 else os.path.splitext(a[1])[0] + "_xdbf"
        print("extracted %d entries -> %s" % (x.extract_all(outdir), outdir))
    elif op in ("unlock", "unlock-all", "set"):
        out = None
        if op == "set":                          # xdbf set <gpd> <ns> <id> <payload> [out]
            if len(a) < 5:
                print("usage: xdbf set <file.gpd> <ns> <id> <payload.bin> [out.gpd]"); sys.exit(2)
            x.set_payload(int(a[2], 0), int(a[3], 0), open(a[4], "rb").read())
            out = a[5] if len(a) > 5 else a[1]
            print("set ns=%s id=%s (%d bytes)" % (a[2], a[3], os.path.getsize(a[4])))
        elif op == "unlock":                     # xdbf unlock <gpd> <ach_id> [out]
            if len(a) < 3:
                print("usage: xdbf unlock <file.gpd> <ach_id> [out.gpd]"); sys.exit(2)
            ok = x.unlock_achievement(int(a[2], 0), True)
            out = a[3] if len(a) > 3 else a[1]
            print("unlocked achievement %s: %s" % (a[2], "ok" if ok else "NOT FOUND"))
        else:                                    # unlock-all
            n = x.unlock_all()
            out = a[2] if len(a) > 2 else a[1]
            print("unlocked %d achievements" % n)
        open(out, "wb").write(x.build())
        print("wrote %s" % out)
    else:
        print("unknown op %r" % op); sys.exit(2)


if __name__ == "__main__":
    main()
