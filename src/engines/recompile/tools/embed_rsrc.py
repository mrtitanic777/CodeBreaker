#!/usr/bin/env python3
r"""embed_rsrc.py -- graft the ORIGINAL module's .rsrc (icons/dialogs/strings) into the
full-fidelity mingw DLL so Win95's FindResource/LoadIcon/LoadString work.

The mingw build has an EMPTY resource directory, so resource-heavy DLLs (shell32/comctl32/
comdlg32) crash when their (or explorer's) code loads a resource. This appends the original
.rsrc bytes as a new section, relocates the resource tree's leaf OffsetToData RVAs to the new
location, and points the PE's resource data directory at it.

    python embed_rsrc.py <ff_dll> <original_dll>      (rewrites ff_dll in place)
"""
import sys, struct


def u16(b, o): return struct.unpack_from("<H", b, o)[0]
def u32(b, o): return struct.unpack_from("<I", b, o)[0]
def p32(b, o, v): struct.pack_into("<I", b, o, v)
def p16(b, o, v): struct.pack_into("<H", b, o, v)


def sections(b):
    e = u32(b, 0x3c)
    nsec = u16(b, e + 6)
    opt = u16(b, e + 20)
    sh = e + 24 + opt
    secs = []
    for i in range(nsec):
        o = sh + i * 40
        secs.append(dict(name=b[o:o + 8], vsize=u32(b, o + 8), vaddr=u32(b, o + 12),
                         rawsize=u32(b, o + 16), rawptr=u32(b, o + 20), chars=u32(b, o + 36), hdr=o))
    return e, nsec, opt, sh, secs


def reloc_rsrc_leaves(rsrc, delta):
    """Walk the resource tree (rooted at offset 0 of `rsrc`), add `delta` to every leaf
    IMAGE_RESOURCE_DATA_ENTRY.OffsetToData (an RVA)."""
    seen = set()

    def walk(off):
        if off in seen or off + 16 > len(rsrc):
            return
        seen.add(off)
        nnamed = u16(rsrc, off + 12)
        nid = u16(rsrc, off + 14)
        n = nnamed + nid
        for i in range(n):
            eo = off + 16 + i * 8
            if eo + 8 > len(rsrc):
                return
            child = u32(rsrc, eo + 4)
            if child & 0x80000000:                 # subdirectory
                walk(child & 0x7fffffff)
            else:                                  # leaf -> IMAGE_RESOURCE_DATA_ENTRY
                de = child
                if de + 4 <= len(rsrc):
                    p32(rsrc, de, u32(rsrc, de) + delta)   # OffsetToData += delta
    walk(0)


def main():
    ff, orig = sys.argv[1], sys.argv[2]
    fb = bytearray(open(ff, "rb").read())
    ob = open(orig, "rb").read()
    _e = u32(fb, 0x3c)
    if u32(fb, _e + 24 + 96 + 2 * 8):          # resource data dir already set -> idempotent skip
        print("already has resources -- skip %s" % ff); return
    # original .rsrc
    oe, on, oopt, osh, osecs = sections(ob)
    rs = next((s for s in osecs if s["name"].rstrip(b"\0") == b".rsrc"), None)
    if not rs:
        print("no .rsrc in original -- nothing to do"); return
    orsrc_rva = rs["vaddr"]
    rsrc = bytearray(ob[rs["rawptr"]:rs["rawptr"] + rs["rawsize"]])
    # ff pe geometry
    e, n, opt, sh, secs = sections(fb)
    file_align = u32(fb, e + 24 + 36)
    sect_align = u32(fb, e + 24 + 32)
    last = max(secs, key=lambda s: s["vaddr"])
    new_rva = (last["vaddr"] + max(last["vsize"], last["rawsize"]) + sect_align - 1) & ~(sect_align - 1)
    new_rawptr = (len(fb) + file_align - 1) & ~(file_align - 1)
    fb += b"\0" * (new_rawptr - len(fb))
    # relocate the resource leaves to the new RVA, then append
    reloc_rsrc_leaves(rsrc, new_rva - orsrc_rva)
    raw = bytes(rsrc)
    raw_aligned = raw + b"\0" * ((-len(raw)) % file_align)
    fb += raw_aligned
    # add a section header for .rsrc
    if u16(fb, e + 20) < 24 + (n + 1) * 40 + (sh - (e + 24 + opt)):
        pass  # header space check (mingw leaves slack; assume OK)
    ho = sh + n * 40
    fb[ho:ho + 8] = b".rsrc\0\0\0"
    p32(fb, ho + 8, len(raw))          # VirtualSize
    p32(fb, ho + 12, new_rva)          # VirtualAddress
    p32(fb, ho + 16, len(raw_aligned))  # SizeOfRawData
    p32(fb, ho + 20, new_rawptr)       # PointerToRawData
    p32(fb, ho + 24, 0); p32(fb, ho + 28, 0); p16(fb, ho + 32, 0); p16(fb, ho + 34, 0)
    p32(fb, ho + 36, 0x40000040)       # INITIALIZED_DATA | MEM_READ
    p16(fb, e + 6, n + 1)              # NumberOfSections++
    # SizeOfImage = new_rva + aligned virtual size
    p32(fb, e + 24 + 56, (new_rva + ((len(raw) + sect_align - 1) & ~(sect_align - 1))))
    # resource data directory [2] (opt hdr: after 96 bytes for PE32, dir[2] at 96+2*8)
    dd = e + 24 + 96
    p32(fb, dd + 2 * 8, new_rva)       # Resource dir RVA
    p32(fb, dd + 2 * 8 + 4, len(raw))  # size
    open(ff, "wb").write(fb)
    print("embedded .rsrc: %d bytes at RVA 0x%x (was 0x%x) -> %s" % (len(raw), new_rva, orsrc_rva, ff))


if __name__ == "__main__":
    main()
