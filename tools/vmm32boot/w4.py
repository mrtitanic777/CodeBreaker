#!/usr/bin/env python3
r"""Byte-exact codec for the Win95 W4 (compressed VMM32) container.

An installed Win95 builds vmm32.vxd as a **W4**: an MZ stub whose real-mode loader decompresses a
body of 8 KB pages when it maps the image. Each page is an MRCI/DoubleSpace-family bit code. This
module implements that code directly in Python -- decode AND encode -- so it needs no emulator and
reproduces Microsoft's own compressed bytes exactly (verified on every page of a real vmm32.vxd:
227/227 blocks decode to the stub decompressor's output and re-encode to the original bytes).

The W4 header (at e_lfanew): 'W4', version(2), page size(2, = 0x2000), page count(2), 'DS', then a
page-count array of DWORD file offsets to each compressed block. The decompressed pages concatenate
to the W3 container (which starts 'W3' and lists the bundled VxD modules).

Per-page grammar (bits are read LSB-first) -- literals and LZ back-references, closed into 512-byte
chunks:
  token flag = first two bits b0,b1:
    b0 != b1                -> LITERAL byte V: V>>7 == b0, then the low 7 bits follow (LSB-first).
    b0 == b1 == 0           -> MATCH, class "00" : 6-bit field  = distance      (1..63).
    b0 == b1 == 1, b2 == 0  -> MATCH, class "110": 8-bit field, distance = f+64  (64..319).
    b0 == b1 == 1, b2 == 1  -> MATCH, class "111": 12-bit field, distance = f+320 (320..4414);
                               field == 0xfff is the END-OF-512-BYTE-CHUNK marker (16 per 8 KB page).
  match length = Elias gamma of n = length-1 (k leading 0 bits, a 1 bit, then k mantissa bits;
                 length = (1<<k) + mantissa + 1, minimum 2).
A match never crosses a 512-byte output boundary; every chunk (including the last) ends with 0xfff.

    from w4 import decompress_w4, build_w4
    w3  = decompress_w4(open('vmm32.vxd','rb').read())        # W4 -> uncompressed W3 payload
    out = build_w4(open('vmm32.vxd','rb').read(), w3)         # W3 payload -> a valid W4 (reuses the stub)

CLI:
    python w4.py decompress vmm32.vxd [out.w3]     list the bundled modules; write the W3 if given
    python w4.py reproduce  vmm32.vxd out.vxd      rebuild the input W4 byte-for-byte (identity)
    python w4.py compress   vmm32.vxd in.w3 out    build a W4 from in.w3, reusing vmm32.vxd's stub
    python w4.py verify     vmm32.vxd              cross-check the Python decoder against the stub's
                                                   own decompressor under emulation (needs `unicorn`)
"""
import struct, sys

PAGE = 0x2000            # 8192-byte pages
CHUNK = 512             # matches never cross a 512-byte output boundary
MAXDIST = 4414          # 320 + 0xffe; 320 + 0xfff is the chunk marker


# --- LSB-first bit I/O ----------------------------------------------------------------------------
class _BR:
    def __init__(s, buf): s.buf = buf; s.pos = 0
    def bit(s):
        b = (s.buf[s.pos >> 3] >> (s.pos & 7)) & 1; s.pos += 1; return b
    def val(s, n):
        v = 0
        for i in range(n): v |= s.bit() << i
        return v


class _BW:
    def __init__(s): s.acc = 0; s.k = 0; s.out = bytearray()
    def bit(s, b):
        s.acc |= (b & 1) << s.k; s.k += 1
        if s.k == 8: s.out.append(s.acc); s.acc = 0; s.k = 0
    def val(s, v, n):
        for i in range(n): s.bit((v >> i) & 1)
    def bytes(s):
        return bytes(s.out + (bytes([s.acc]) if s.k else b""))


# --- per-page codec -------------------------------------------------------------------------------
def _glen(br):
    k = 0
    while br.bit() == 0: k += 1
    return (1 << k) + br.val(k) + 1


def decode_block(buf, page=PAGE):
    """A compressed page -> (tokens, decoded bytes). tokens: ('L', byte) | ('M', dist, length)."""
    br = _BR(buf); toks = []; out = bytearray()
    while len(out) < page:
        b0 = br.bit(); b1 = br.bit()
        if b0 != b1:
            V = (b0 << 7) | br.val(7); toks.append(("L", V)); out.append(V)
            continue
        if b0 == 0:
            d = br.val(6)
        elif br.bit() == 0:
            d = br.val(8) + 64
        else:
            f = br.val(12)
            if f == 0xfff:                       # end-of-chunk marker: no output
                continue
            d = f + 320
        L = _glen(br); toks.append(("M", d, L))
        for _ in range(L): out.append(out[-d])
    return toks, bytes(out)


def _emit_len(bw, L):
    n = L - 1; k = n.bit_length() - 1
    for _ in range(k): bw.bit(0)
    bw.bit(1); bw.val(n - (1 << k), k)


def _emit_dist(bw, d):
    if d <= 63:    bw.bit(0); bw.bit(0); bw.val(d, 6)
    elif d <= 319: bw.bit(1); bw.bit(1); bw.bit(0); bw.val(d - 64, 8)
    else:          bw.bit(1); bw.bit(1); bw.bit(1); bw.val(d - 320, 12)


def _marker(bw):
    bw.bit(1); bw.bit(1); bw.bit(1); bw.val(0xfff, 12)


def encode_block(toks, page=PAGE):
    """tokens -> compressed page bytes, closing every 512-byte chunk with the 0xfff marker."""
    bw = _BW(); outn = 0
    for t in toks:
        if t[0] == "L":
            V = t[1]; v7 = (V >> 7) & 1
            bw.bit(v7); bw.bit(1 - v7); bw.val(V & 0x7f, 7); outn += 1
        else:
            _emit_dist(bw, t[1]); _emit_len(bw, t[2]); outn += t[2]
        if outn % CHUNK == 0 and outn < page: _marker(bw)
    _marker(bw)
    return bw.bytes()


def compress_page(pb, page=PAGE):
    """Greedy longest-match LZ over one page; matches never cross a 512-byte boundary. -> tokens."""
    n = len(pb); heads = {}; prev = [-1] * n
    def key(p): return pb[p] | (pb[p + 1] << 8)
    toks = []; i = 0
    while i < n:
        maxlen = ((i // CHUNK) + 1) * CHUNK - i          # room left in this chunk
        best_l = 1; best_d = 0
        if maxlen >= 2 and i + 1 < n:
            p = heads.get(key(i), -1); lo = max(0, i - MAXDIST)
            while p >= lo:
                if pb[p + best_l - 1] == pb[i + best_l - 1]:    # cheap reject on current best tail
                    l = 0
                    while l < maxlen and pb[p + l] == pb[i + l]: l += 1
                    if l > best_l or (l == best_l and (i - p) < best_d):
                        best_l = l; best_d = i - p
                        if l == maxlen: break
                p = prev[p]
        adv = best_l if (best_d and best_l >= 2) else 1
        toks.append(("M", best_d, best_l) if adv >= 2 else ("L", pb[i]))
        for j in range(i, i + adv):
            if j + 1 < n:
                k = key(j); prev[j] = heads.get(k, -1); heads[k] = j
        i += adv
    return toks


# --- container ------------------------------------------------------------------------------------
def _table(data, lfa):
    page, npages = struct.unpack_from("<HH", data, lfa + 4)
    tab = lfa + 0x10
    offs = [struct.unpack_from("<I", data, tab + i * 4)[0] for i in range(npages)] + [len(data)]
    return page, npages, offs


def decompress_w4(data):
    """Return the uncompressed W3 payload of a W4 vmm32.vxd (or `data` unchanged if already W3/LE)."""
    lfa = struct.unpack_from("<I", data, 0x3c)[0]
    if data[lfa:lfa + 2] != b"W4":
        return bytes(data)                                # already uncompressed (W3) or a plain module
    page, npages, offs = _table(data, lfa)
    out = bytearray()
    for p in range(npages):
        out += decode_block(data[offs[p]:offs[p + 1]], page)[1]
    return bytes(out)


def build_w4(template_w4, w3_payload, tokfn=compress_page):
    """Build a W4 container for `w3_payload`, reusing the MZ stub + W4 header of `template_w4`
    (its real-mode loader/decompressor). `tokfn(page_bytes)` chooses the parse (default: compress)."""
    lfa = struct.unpack_from("<I", template_w4, 0x3c)[0]
    if template_w4[lfa:lfa + 2] != b"W4":
        raise ValueError("template is not a W4 container")
    page = struct.unpack_from("<H", template_w4, lfa + 4)[0]
    head = template_w4[:lfa]; hdr = bytearray(template_w4[lfa:lfa + 0x10])
    pages = [w3_payload[i:i + page].ljust(page, b"\x00") for i in range(0, len(w3_payload), page)]
    blocks = [encode_block(tokfn(pg), page) for pg in pages]
    struct.pack_into("<H", hdr, 6, len(pages))
    data_off = lfa + 0x10 + len(pages) * 4
    offtab = bytearray(); pos = data_off
    for b in blocks:
        offtab += struct.pack("<I", pos); pos += len(b)
    return bytes(head) + bytes(hdr) + bytes(offtab) + b"".join(blocks)


def reproduce_w4(data):
    """Rebuild a W4 by decoding each block to tokens and re-encoding -- byte-identical to `data`."""
    lfa = struct.unpack_from("<I", data, 0x3c)[0]
    page, npages, offs = _table(data, lfa)
    def tok(pg, _i=[0]):
        t = decode_block(data[offs[_i[0]]:offs[_i[0] + 1]], page)[0]; _i[0] += 1; return t
    return build_w4(data, decompress_w4(data), tokfn=tok)


def modules(w3):
    """[(name, offset, size), ...] of the VxDs bundled in a W3 payload."""
    if w3[:2] != b"W3":
        return []
    cnt = struct.unpack_from("<I", w3, 4)[0]
    out = []
    for i in range(cnt):
        p = 0x10 + i * 16
        nm = w3[p:p + 8].rstrip(b" \x00").decode("latin1")
        mo, ms = struct.unpack_from("<II", w3, p + 8)
        out.append((nm, mo, ms))
    return out


# --- optional: run the stub's OWN decompressor under emulation, to re-verify the Python codec ------
_DEC_SIG = b"\x60\x1e\x06\x2e\x89\x36"          # pushaw; push ds; push es; mov word cs:[imm], si


def decompress_w4_emulated(data):
    """Ground-truth decompressor: locate the routine inside the W4's real-mode stub and run it under
    the Unicorn CPU emulator. Used to validate `decode_block`; needs `pip install unicorn`."""
    lfa = struct.unpack_from("<I", data, 0x3c)[0]
    if data[lfa:lfa + 2] != b"W4":
        return bytes(data)
    from unicorn import Uc, UC_ARCH_X86, UC_MODE_16, UcError
    from unicorn.x86_const import (UC_X86_REG_CS, UC_X86_REG_DS, UC_X86_REG_ES, UC_X86_REG_SS,
                                   UC_X86_REG_SI, UC_X86_REG_DI, UC_X86_REG_SP,
                                   UC_X86_REG_AX, UC_X86_REG_CX)
    page, npages, offs = _table(data, lfa)
    stub = data[0:lfa]
    dec = stub.find(_DEC_SIG)
    if dec < 0:
        raise ValueError("W4 decompressor routine not found in the stub")
    cs_base = struct.unpack_from("<H", data, 8)[0] * 16
    CS = cs_base >> 4; SRC, DST, STK = 0x2000, 0x2400, 0x2800

    def one(block):
        uc = Uc(UC_ARCH_X86, UC_MODE_16); uc.mem_map(0, 0x40000)
        uc.mem_write(0, stub); uc.mem_write(SRC * 16, block)
        uc.mem_write(DST * 16, b"\x00" * (page + 64))
        uc.reg_write(UC_X86_REG_CS, CS)
        uc.reg_write(UC_X86_REG_DS, SRC); uc.reg_write(UC_X86_REG_SI, 0)
        uc.reg_write(UC_X86_REG_ES, DST); uc.reg_write(UC_X86_REG_DI, 0)
        uc.reg_write(UC_X86_REG_SS, STK); uc.reg_write(UC_X86_REG_SP, 0x2000)
        uc.reg_write(UC_X86_REG_AX, page); uc.reg_write(UC_X86_REG_CX, len(block))
        uc.mem_write(STK * 16 + 0x2000, struct.pack("<H", 0x3000))
        try: uc.emu_start(cs_base + (dec - cs_base), cs_base + 0x3000, count=4_000_000)
        except UcError: pass
        return bytes(uc.mem_read(DST * 16, page))

    out = bytearray()
    for p in range(npages):
        blk = data[offs[p]:offs[p + 1]]
        out += blk if len(blk) == page else one(blk)
    return bytes(out)


# --- CLI ------------------------------------------------------------------------------------------
def _main(argv):
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__.strip()); return 0
    cmd = argv[0]
    if cmd == "decompress":
        w3 = decompress_w4(open(argv[1], "rb").read())
        mods = modules(w3)
        print("decompressed %d bytes; %d bundled modules:" % (len(w3), len(mods)))
        print("  " + " ".join(n for n, _, _ in mods))
        if len(argv) > 2:
            open(argv[2], "wb").write(w3); print("wrote", argv[2])
    elif cmd == "reproduce":
        data = open(argv[1], "rb").read(); out = reproduce_w4(data)
        open(argv[2], "wb").write(out)
        print("reproduced %s -> %s : byte-exact %s" % (argv[1], argv[2], out == data))
    elif cmd == "compress":
        tmpl = open(argv[1], "rb").read(); payload = open(argv[2], "rb").read()
        out = build_w4(tmpl, payload)
        open(argv[3], "wb").write(out)
        rt = decompress_w4(out) == payload
        print("built %s (%d bytes, %.1f%% of template); round-trips: %s"
              % (argv[3], len(out), 100.0 * len(out) / len(tmpl), rt))
    elif cmd == "verify":
        data = open(argv[1], "rb").read()
        py = decompress_w4(data); em = decompress_w4_emulated(data)
        print("python decoder == emulated stub decompressor: %s (%d bytes)" % (py == em, len(py)))
    else:
        print("unknown command %r (see --help)" % cmd); return 2
    return 0


if __name__ == "__main__":
    sys.exit(_main(sys.argv[1:]))
