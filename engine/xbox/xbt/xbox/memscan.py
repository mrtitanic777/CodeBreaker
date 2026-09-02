"""Memory scanner & trainer toolkit — Cheat-Engine-style search over Xbox 360
console memory (via JRPC2), plus pointer-chain and code-cave finders.

The console RTE (`jrpc2` peek/poke/freeze) can read and write an address, but
finding the address is the hard part. This adds the search: an initial scan of a
range for a value, then successive filter passes (equal / changed / increased /
decreased / …) that narrow millions of candidates to the one that is your
health/ammo/score. A pointer-scan then finds a *stable* base+offset chain so the
cheat survives a reload, and a cave-scan finds unused space to host a hook.

The search engine is a pure library over a `read(addr, size) -> bytes` callback
(a live JRPC2 reader in the field, a snapshot in tests), so it is fully
verifiable offline.
"""

import struct
import sys

DESCRIPTION = "Xbox 360 memory scanner + pointer-chain / code-cave finder (trainers over JRPC2)"

# big-endian value codecs
_TYPES = {
    "u8": (1, ">B"), "i8": (1, ">b"), "u16": (2, ">H"), "i16": (2, ">h"),
    "u32": (4, ">I"), "i32": (4, ">i"), "u64": (8, ">Q"), "i64": (8, ">q"),
    "f32": (4, ">f"), "f64": (8, ">d"),
}
PPC_NOP = 0x60000000


def _decode(data, typ):
    size, fmt = _TYPES[typ]
    return struct.unpack(fmt, data[:size])[0]


def _encode(value, typ):
    size, fmt = _TYPES[typ]
    return struct.pack(fmt, value)


class Scanner:
    def __init__(self, read):
        self.read = read                 # read(addr, size) -> bytes
        self.typ = "u32"
        self.hits = {}                   # addr -> last-seen value

    # -- first pass: scan a range for a value ------------------------------
    def first_scan(self, lo, hi, value=None, typ="u32", step=None, chunk=0x10000):
        self.typ = typ
        size = _TYPES[typ][0]
        step = step or size
        self.hits = {}
        addr = lo
        while addr < hi:
            n = min(chunk, hi - addr)
            buf = self.read(addr, n)
            if not buf:
                break
            for off in range(0, len(buf) - size + 1, step):
                v = _decode(buf[off:off + size], typ)
                if value is None or v == value:
                    self.hits[addr + off] = v
            addr += len(buf)
        return len(self.hits)

    # -- successive passes: filter by comparison to the previous value -----
    def next_scan(self, op, value=None):
        size = _TYPES[self.typ][0]
        keep = {}
        for a, prev in self.hits.items():
            buf = self.read(a, size)
            if len(buf) < size:
                continue
            cur = _decode(buf, self.typ)
            ok = {
                "eq": cur == value, "ne": cur != value,
                "gt": cur > value if value is not None else cur > prev,
                "lt": cur < value if value is not None else cur < prev,
                "changed": cur != prev, "unchanged": cur == prev,
                "inc": cur > prev, "dec": cur < prev,
            }.get(op, False)
            if ok:
                keep[a] = cur
        self.hits = keep
        return len(self.hits)

    # -- code caves: runs of NOP / zero big enough to host a hook ----------
    def find_caves(self, lo, hi, min_bytes=0x40, chunk=0x10000):
        caves = []
        run_start = None
        addr = lo
        while addr < hi:
            buf = self.read(addr, min(chunk, hi - addr))
            if not buf:
                break
            for off in range(0, len(buf) - 3, 4):
                word = struct.unpack_from(">I", buf, off)[0]
                a = addr + off
                if word in (0, PPC_NOP):
                    if run_start is None:
                        run_start = a
                elif run_start is not None:
                    if a - run_start >= min_bytes:
                        caves.append((run_start, a - run_start))
                    run_start = None
            addr += len(buf)
        if run_start is not None and (hi - run_start) >= min_bytes:
            caves.append((run_start, hi - run_start))
        return caves

    # -- pointer scan: static base + offset chain that resolves to target --
    def pointer_paths(self, target, static_lo, static_hi, max_depth=2,
                      max_off=0x800, align=4):
        """Find (base_addr, [offsets]) chains so that following the pointer
        chain from base_addr lands within `max_off` of `target`. Depth-limited
        breadth search; returns the shortest chains first."""
        results = []

        def pointers_to(dst_lo, dst_hi):
            """addresses in [static_lo,static_hi) holding a pointer into the
            window [dst_lo, dst_hi)."""
            out = []
            a = static_lo
            while a < static_hi:
                buf = self.read(a, min(0x10000, static_hi - a))
                if not buf:
                    break
                for off in range(0, len(buf) - 3, align):
                    p = struct.unpack_from(">I", buf, off)[0]
                    if dst_lo <= p <= dst_hi:
                        out.append((a + off, p))
                a += len(buf)
            return out

        # depth 1: a static slot pointing near target
        for src, p in pointers_to(target - max_off, target):
            results.append((src, [target - p]))
        if max_depth >= 2:
            for src, p in pointers_to(target - max_off, target):
                base_off = target - p
                for src2, p2 in pointers_to(src - max_off, src):
                    results.append((src2, [src - p2, base_off]))
        results.sort(key=lambda r: (len(r[1]), r[0]))
        return results[:64]


# ── live reader over XBDM (getmemex) ───────────────────────────────────
def console_reader(ip):
    import xbdm
    cli = xbdm.XbdmClient(ip)
    cli.connect()

    def read(addr, size):
        try:
            return cli.memory_read(addr, size)
        except Exception:
            return b""
    return read


# ── CLI ────────────────────────────────────────────────────────────────
def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage:\n"
              "  memscan caves   <ip> <lo> <hi> [--min N]        find code caves (NOP/zero runs)\n"
              "  memscan pointer <ip> <target> <lo> <hi>         find static pointer chains\n"
              "  memscan scan    <ip> <lo> <hi> <value> [--type u32]\n"
              "     (live search needs a dev console; the Scanner class is the library)")
        return
    op = a[0]
    ip = a[1]
    typ = a[a.index("--type") + 1] if "--type" in a else "u32"
    sc = Scanner(console_reader(ip))
    if op == "caves":
        lo, hi = int(a[2], 0), int(a[3], 0)
        mn = int(a[a.index("--min") + 1], 0) if "--min" in a else 0x40
        for addr, ln in sc.find_caves(lo, hi, mn):
            print("  cave @0x%08X  %d bytes" % (addr, ln))
        return
    if op == "pointer":
        target, lo, hi = int(a[2], 0), int(a[3], 0), int(a[4], 0)
        for base, offs in sc.pointer_paths(target, lo, hi):
            print("  [0x%08X]%s" % (base, "".join("+0x%X" % o for o in offs)))
        return
    if op == "scan":
        lo, hi, val = int(a[2], 0), int(a[3], 0), int(a[4], 0)
        n = sc.first_scan(lo, hi, val, typ)
        print("%d match(es) for %s == %d in [0x%X,0x%X)" % (n, typ, val, lo, hi))
        return
    print("unknown/short command; run 'memscan' for usage"); sys.exit(2)


if __name__ == "__main__":
    main()
