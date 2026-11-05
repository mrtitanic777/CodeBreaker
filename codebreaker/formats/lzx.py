"""lzx.py -- Microsoft LZX (LZXD), the codec of CAB, WIM, XEX and XNA's compressed content
(formats.dll answers: native/formats/lzx, docs/NATIVE.md step 4).

    from codebreaker.formats import lzx
    lzx.lzxd_decompress(data, window_bits, out_len)       a raw LZXD stream (XNB: window_bits 16)
    lzx.lzx_decompress_xex(buf, window_size, first_block, out_len)   a XEX's normal-compressed image:
                                                          its block chain (each block's SHA-1 checked)

An LZ77 window and canonical Huffman trees (main, length, aligned offset), the algorithm libmspack's
lzxd.c implements, written from the format (reset interval 0, no delta). Decompressing a retail
default.xex gives byte for byte the image an uncompressed copy holds. The native xex program has its
own decoder for whole modules; this one is for what the readers hold (XNA content).
"""
from codebreaker.formats import native


def _bytes_or_none(v):
    return None if v is None else bytes(v)


def _block(first_block):
    """(block size, SHA-1) as the C++ reads it: a tuple, the hash as bytes."""
    if not first_block:
        return None
    first_block = tuple(first_block)
    return (first_block[0], bytes(first_block[1])) if len(first_block) == 2 else first_block


@native.answered("lzx.lzxd_decompress",
                 args_of=lambda data, window_bits, out_len, ref_data=None:
                 (data, (window_bits, out_len, _bytes_or_none(ref_data))))
def lzxd_decompress(data, window_bits, out_len, ref_data=None):
    """Decode a raw (de-blocked) LZXD stream to `out_len` bytes.

    Output is produced in 32 KB frames; the bitstream is realigned to a 16-bit boundary at each
    frame boundary, while block state (type, trees, repeated offsets) persists across frames and
    blocks -- mirroring the reference XEX LZX decoder exactly.

    `ref_data` enables the LZX-DELTA mode used by .xexp patches: the window is pre-seeded with
    [zero padding][ref_data] up to window_size, so matches may reference the old image; frame/size
    counting stays relative to the seed end and only the newly produced bytes are returned."""


@native.answered("lzx.lzx_decompress_xex",
                 args_of=lambda buf, window_size, first_block, out_len:
                 (buf, (window_size, _block(first_block), out_len)))
def lzx_decompress_xex(buf, window_size, first_block, out_len):
    """Full XEX 'normal compression' decode: de-block then LZXD. `first_block` is (block_size,
    block_hash20) from the file-format header."""
