"""xnb.py -- XNA Game Studio content (.xnb): textures and sprite fonts out as PNG (and JSON), the rest
catalogued.

    from codebreaker.formats import xnb
    x = xnb.read(path)                  x.platform ('x' Xbox 360, 'w' Windows, 'm' Phone), x.version,
                                        x.readers [type reader names], x.data (the content stream)
    xnb.convert(path, out_dir)          {"file", "kind", ...}: a texture -> <name>.png, a sprite font ->
                                        <name>.png + <name>.json (glyphs, cropping, characters,
                                        spacing, kerning), a sound effect -> <name>.wav (XMA2 decoded by
                                        ffmpeg: formats/xbox/xma.py); anything else -> kind "other"
    xnb.rename_readers(raw, rename)     the file with each type reader name passed through rename(name), as
                                        uncompressed XNB bytes -- or None when no name changed

A .xnb starts "XNB", a platform letter, the version (5 for XNA 4.0) and flags (0x80: compressed),
then the file's size; compressed content gives its decompressed size and comes in LZX frames. The
stream lists its type readers, then the primary object. Integers in the stream are little-endian
on every platform, but an Xbox 360 texture's pixels are its GPU's: DXT blocks in 16-bit
big-endian words, and a Color pixel as a big-endian dword (A, B, G, R in file order). Xbox
textures in XNB are linear, not tiled.
"""
import json
import os
import struct

from codebreaker.formats import lzx

SURFACE_FORMATS = {0: "Color", 1: "Bgr565", 2: "Bgra5551", 3: "Bgra4444", 4: "Dxt1", 5: "Dxt3", 6: "Dxt5",
                   7: "NormalizedByte2", 8: "NormalizedByte4", 9: "Rgba1010102", 10: "Rg32", 11: "Rgba64",
                   12: "Alpha8", 13: "Single", 14: "Vector2", 15: "Vector4", 16: "HalfSingle",
                   17: "HalfVector2", 18: "HalfVector4", 19: "HdrBlendable"}
_DXT = {4: "dxt1", 5: "dxt3", 6: "dxt5"}


class Xnb:
    def __init__(self, raw):
        if raw[:3] != b"XNB":
            raise ValueError("not an XNB file")
        self.platform, self.version, self.flags = chr(raw[3]), raw[4], raw[5]
        size, = struct.unpack_from("<I", raw, 6)
        if self.flags & 0x80:
            out_len, = struct.unpack_from("<I", raw, 10)
            self.data = lzx.lzxd_decompress(_deframe(raw, size), 16, out_len)
        else:
            self.data = bytes(raw[10:size])
        self.readers, self.shared, self.offset = _manifest(self.data)


def _deframe(raw, size):
    """The LZX frames of compressed content as one stream: each is a 16-bit big-endian length, or
    0xFF, a 16-bit frame size and then the length."""
    pos, chunks = 14, []
    while pos < size:
        hi, lo = raw[pos], raw[pos + 1]
        pos += 2
        block, frame = hi << 8 | lo, 0x8000
        if hi == 0xFF:
            frame = lo << 8 | raw[pos]
            block = raw[pos + 1] << 8 | raw[pos + 2]
            pos += 3
        if block == 0 or frame == 0:
            break
        chunks.append(raw[pos:pos + block])
        pos += block
    return b"".join(chunks)


def _7bit(b, o):
    value = shift = 0
    while True:
        x = b[o]
        o += 1
        value |= (x & 0x7F) << shift
        if not x & 0x80:
            return value, o
        shift += 7


def _string(b, o):
    n, o = _7bit(b, o)
    return b[o:o + n].decode("utf-8", "replace"), o + n


def _7bit_bytes(value):
    out = bytearray()
    while True:
        x = value & 0x7F
        value >>= 7
        out.append(x | (0x80 if value else 0))
        if not value:
            return bytes(out)


def rename_readers(raw, rename):
    """`raw` (an .xnb's bytes) with each type reader's name passed through rename(name) -> the new file,
    uncompressed and otherwise the same, or None when rename changed none."""
    x = Xnb(raw)
    count, o = _7bit(x.data, 0)
    manifest, changed = [_7bit_bytes(count)], False
    for _ in range(count):
        name, o2 = _string(x.data, o)
        new = rename(name)
        changed |= new != name
        encoded = new.encode("utf-8")
        manifest += [_7bit_bytes(len(encoded)), encoded, x.data[o2:o2 + 4]]   # and the reader's version
        o = o2 + 4
    if not changed:
        return None
    body = b"".join(manifest) + x.data[o:]
    return b"XNB" + x.platform.encode() + bytes([x.version, x.flags & ~0x80]) + struct.pack("<I", 10 + len(body)) + body


def _manifest(data):
    count, o = _7bit(data, 0)
    readers = []
    for _ in range(count):
        name, o = _string(data, o)
        o += 4                                               # the reader's version
        readers.append(name.split(",")[0])
    shared, o = _7bit(data, o)
    return readers, shared, o


def short(reader):
    """Microsoft.Xna.Framework.Content.Texture2DReader -> Texture2D."""
    return reader.rsplit(".", 1)[-1].split("`")[0].replace("Reader", "")


def read(path):
    with open(path, "rb") as f:
        return Xnb(f.read())


def _texture(data, o):
    _reader, o = _7bit(data, o)
    fmt, w, h, levels = struct.unpack_from("<iIII", data, o)
    o += 16
    mips = []
    for _ in range(levels):
        size, = struct.unpack_from("<I", data, o)
        o += 4
        mips.append(data[o:o + size])
        o += size
    return {"format": fmt, "width": w, "height": h, "levels": levels, "mips": mips}, o


def texture_image(tex, platform):
    """The first level as a Pillow RGBA image."""
    from PIL import Image
    from codebreaker.formats.xbox import texture
    fmt, w, h, pixels = tex["format"], tex["width"], tex["height"], tex["mips"][0]
    if fmt in _DXT:
        return texture.decode_dxt(pixels, w, h, _DXT[fmt], tiled=False, swapped=platform == "x")
    if fmt == 0:
        img = Image.frombytes("RGBA", (w, h), bytes(pixels[:w * h * 4]))
        if platform == "x":                                  # A, B, G, R in file order
            a, b, g, r = img.split()
            img = Image.merge("RGBA", (r, g, b, a))
        return img
    raise ValueError("texture format %s is not converted" % SURFACE_FORMATS.get(fmt, fmt))


def _sprite_font(data, o):
    """XNA 4.0 SpriteFontReader: a texture, glyph and cropping rectangles, characters, line spacing,
    spacing, kerning and the default character. Each list is a ReadObject: its reader index first."""
    _reader, o = _7bit(data, o)
    tex, o = _texture(data, o)

    def rects(o):
        _idx, o = _7bit(data, o)
        n, = struct.unpack_from("<I", data, o)
        o += 4
        return [list(struct.unpack_from("<4i", data, o + 16 * i)) for i in range(n)], o + 16 * n

    glyphs, o = rects(o)
    cropping, o = rects(o)
    _idx, o = _7bit(data, o)
    n, = struct.unpack_from("<I", data, o)
    o += 4
    if not len(glyphs) == len(cropping) == n:
        raise ValueError("sprite font lists disagree: %d glyphs, %d cropping, %d characters"
                         % (len(glyphs), len(cropping), n))
    chars = []
    for _ in range(n):                                       # each a UTF-8 character (BinaryReader.ReadChar)
        b0 = data[o]
        size = 1 if b0 < 0x80 else 2 if b0 < 0xE0 else 3 if b0 < 0xF0 else 4
        chars.append(data[o:o + size].decode("utf-8", "replace")[:1])
        o += size
    line_spacing, spacing = struct.unpack_from("<if", data, o)
    o += 8
    _idx, o = _7bit(data, o)
    k, = struct.unpack_from("<I", data, o)
    o += 4
    kerning = [list(struct.unpack_from("<3f", data, o + 12 * i)) for i in range(k)]
    return {"texture": tex, "glyphs": glyphs, "cropping": cropping, "characters": chars,
            "line_spacing": line_spacing, "spacing": spacing, "kerning": kerning}


# WAVEFORMATEX and XMA2WAVEFORMATEX fields, in order (the Xbox 360 writes them big-endian)
_WAVEFORMATEX = "HHIIHHH"
_XMA2_EXTRA = "HIIIIIIIBBH"


def _sound_effect(data, o, platform):
    """SoundEffectReader: the format (a WAVEFORMATEX), the samples, the loop and the duration."""
    _reader, o = _7bit(data, o)
    n, = struct.unpack_from("<I", data, o)
    fmt = bytearray(data[o + 4:o + 4 + n])
    o += 4 + n
    size, = struct.unpack_from("<I", data, o)
    samples = data[o + 4:o + 4 + size]
    o += 4 + size
    loop_start, loop_length, duration = struct.unpack_from("<iii", data, o)
    if platform == "x":                                       # the format's fields are big-endian there
        fields = _WAVEFORMATEX + (_XMA2_EXTRA if struct.unpack_from(">H", fmt, 0)[0] == 0x166 else "")
        size_of = struct.calcsize(">" + fields)
        if len(fmt) >= size_of:
            fmt[:size_of] = struct.pack("<" + fields, *struct.unpack_from(">" + fields, fmt, 0))
        else:
            fmt[:18] = struct.pack("<" + _WAVEFORMATEX, *struct.unpack_from(">" + _WAVEFORMATEX, fmt, 0))
    tag, channels, rate, _avg, _align, bits = struct.unpack_from("<HHIIHH", fmt, 0)
    if platform == "x" and tag == 1 and bits == 16:            # big-endian PCM samples
        samples = b"".join(samples[i + 1:i + 2] + samples[i:i + 1] for i in range(0, len(samples) - 1, 2))
    return {"tag": tag, "channels": channels, "rate": rate, "bits": bits, "fmt": bytes(fmt), "samples": samples,
            "loop": [loop_start, loop_length], "duration_ms": duration}


def _riff(fmt, samples):
    chunk = lambda tag, body: tag + struct.pack("<I", len(body)) + body + (bytes(1) if len(body) % 2 else b"")
    body = b"WAVE" + chunk(b"fmt ", fmt) + chunk(b"data", samples)
    return b"RIFF" + struct.pack("<I", len(body)) + body


def write_wav(sound, out):
    """A sound effect as a WAV a PC plays: PCM and ADPCM as they are, XMA2 decoded to PCM by ffmpeg."""
    riff = _riff(sound["fmt"], sound["samples"])
    if sound["tag"] in (0x166, 0x165):
        from codebreaker.formats.xbox import xma
        xma.decode_to_wav(riff, out, sound["channels"])
    else:
        with open(out, "wb") as f:
            f.write(riff)


def convert(path, out_dir):
    name = os.path.splitext(os.path.basename(path))[0]
    rec = {"file": os.path.basename(path)}
    try:
        x = read(path)
        kinds = [short(r) for r in x.readers]
        rec["platform"], rec["readers"] = x.platform, kinds
        primary = kinds[0] if kinds else "?"
        os.makedirs(out_dir, exist_ok=True)
        if primary == "Texture2D":
            tex, _o = _texture(x.data, x.offset)
            texture_image(tex, x.platform).save(os.path.join(out_dir, name + ".png"))
            rec.update(kind="texture", format=SURFACE_FORMATS.get(tex["format"], tex["format"]),
                       width=tex["width"], height=tex["height"], levels=tex["levels"])
        elif primary == "SpriteFont":
            font = _sprite_font(x.data, x.offset)
            tex = font.pop("texture")
            texture_image(tex, x.platform).save(os.path.join(out_dir, name + ".png"))
            with open(os.path.join(out_dir, name + ".json"), "w", encoding="utf-8") as f:
                json.dump(font, f, ensure_ascii=False, indent=1)
            rec.update(kind="font", width=tex["width"], height=tex["height"], glyphs=len(font["glyphs"]))
        elif primary == "SoundEffect":
            sound = _sound_effect(x.data, x.offset, x.platform)
            write_wav(sound, os.path.join(out_dir, name + ".wav"))
            rec.update(kind="sound", codec={1: "PCM", 2: "ADPCM", 0x166: "XMA2", 0x165: "XMA"}.get(sound["tag"], sound["tag"]),
                       channels=sound["channels"], rate=sound["rate"], duration_ms=sound["duration_ms"], loop=sound["loop"])
        else:
            rec["kind"] = "other"
    except Exception as e:                                   # one bad file is reported, the rest go on
        rec["error"] = "%s: %s" % (type(e).__name__, e)
    return rec
