"""LZ4 raw-block codec (no frame, no checksum, no embedded size).

CCI and CSO store each 2048-byte ISO sector as a raw LZ4 block per
lz4_Block_format.md. Decoding is pure-Python (a faithful port of the format
spec) so reading a .cci/.cso never needs a third-party package. Encoding uses
python-lz4's LZ4-HC when installed (smaller, matches the reference tools) and
otherwise falls back to a pure-Python greedy encoder — LZ4 is lossless, so any
valid block is interoperable; the fallback just compresses a little less.
"""

import struct

try:
    import lz4.block as _lz4block
except Exception:
    _lz4block = None

HAVE_LZ4 = _lz4block is not None
HC_LEVEL = 12                       # XGDTool CCI + stellar CSO both use HC level 12


class Lz4Error(Exception):
    pass


# ── decode (pure-Python, spec-faithful) ────────────────────────────────
def decode_block(data, uncompressed_size):
    """Decode one raw LZ4 block to exactly `uncompressed_size` bytes."""
    if HAVE_LZ4:
        try:
            out = _lz4block.decompress(bytes(data), uncompressed_size=uncompressed_size)
            if len(out) == uncompressed_size:
                return out
        except Exception:
            pass
    out = _decode_py(bytes(data), uncompressed_size)
    if len(out) != uncompressed_size:
        raise Lz4Error("LZ4 block decoded to %d bytes, expected %d" % (len(out), uncompressed_size))
    return out


def _decode_py(src, uncompressed_size):
    if uncompressed_size == 0:
        return b""
    n = len(src)
    if n == 0:
        raise Lz4Error("empty LZ4 block")
    dst = bytearray()
    i = 0
    try:
        while i < n:
            token = src[i]
            i += 1
            length = token >> 4                 # literals
            if length == 15:
                while True:
                    b = src[i]; i += 1
                    length += b
                    if b != 255:
                        break
            if length:
                dst += src[i:i + length]
                i += length
            if i == n:
                break                            # last sequence: literals only
            offset = src[i] | (src[i + 1] << 8)  # match
            i += 2
            if offset == 0:
                raise Lz4Error("invalid zero match offset")
            length = (token & 0x0F) + 4
            if (token & 0x0F) == 15:
                while True:
                    b = src[i]; i += 1
                    length += b
                    if b != 255:
                        break
            start = len(dst) - offset
            if start < 0:
                raise Lz4Error("match before start of output")
            if offset >= length:
                dst += dst[start:start + length]
            else:
                pattern = bytes(dst[start:])     # overlapping match
                reps = (length // offset) + 2
                dst += (pattern * reps)[:length]
    except IndexError:
        raise Lz4Error("truncated LZ4 block")
    return bytes(dst)


# ── encode ─────────────────────────────────────────────────────────────
_MIN_MATCH = 4
_MF_LIMIT = 12          # last match must start >= 12 bytes before end
_LAST_LITERALS = 5      # final 5 bytes are always literals
_MAX_OFFSET = 0xFFFF


def compress_block(data):
    """Compress one raw block to a raw LZ4 block. Prefers python-lz4 HC (matches
    the reference encoders); else a pure-Python greedy encoder."""
    data = bytes(data)
    if HAVE_LZ4:
        return _lz4block.compress(data, mode="high_compression",
                                  compression=HC_LEVEL, store_size=False)
    return _compress_py(data)


def _emit_length(out, n):
    while n >= 255:
        out.append(255); n -= 255
    out.append(n)


def _compress_py(src):
    n = len(src)
    out = bytearray()
    if n < _MF_LIMIT + 1:                        # too short to hold any match
        token = (min(n, 15)) << 4
        out.append(token if n < 15 else 0xF0)
        if n >= 15:
            _emit_length(out, n - 15)
        out += src
        return bytes(out)
    htab = {}
    anchor = 0
    i = 0
    limit = n - _MF_LIMIT
    while i < limit:
        seq = src[i:i + 4]
        cand = htab.get(seq)
        htab[seq] = i
        if cand is not None and (i - cand) <= _MAX_OFFSET and src[cand:cand + 4] == seq:
            # extend the match
            mlen = 4
            maxm = n - _LAST_LITERALS - i
            while mlen < maxm and src[cand + mlen] == src[i + mlen]:
                mlen += 1
            litlen = i - anchor
            token = (min(litlen, 15) << 4) | min(mlen - _MIN_MATCH, 15)
            out.append(token)
            if litlen >= 15:
                _emit_length(out, litlen - 15)
            out += src[anchor:i]
            out += struct.pack("<H", i - cand)
            if mlen - _MIN_MATCH >= 15:
                _emit_length(out, mlen - _MIN_MATCH - 15)
            # seed the hash for a couple of positions inside the match
            for j in range(i + 1, min(i + mlen, limit)):
                htab.setdefault(src[j:j + 4], j)
            i += mlen
            anchor = i
        else:
            i += 1
    # trailing literals (always includes the last 5 bytes)
    litlen = n - anchor
    out.append(min(litlen, 15) << 4 if litlen < 15 else 0xF0)
    if litlen >= 15:
        _emit_length(out, litlen - 15)
    out += src[anchor:]
    return bytes(out)
