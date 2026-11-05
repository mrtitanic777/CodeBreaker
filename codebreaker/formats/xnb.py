"""xnb.py -- XNA Game Studio content (.xnb): textures and sprite fonts out as PNG (and JSON), the rest
catalogued.

    from codebreaker.formats import xnb
    x = xnb.read(path)                  x.platform ('x' Xbox 360, 'w' Windows, 'm' Phone), x.version,
                                        x.readers [type reader names], x.data (the content stream)
    xnb.convert(path, out_dir)          {"file", "kind", ...}: a texture -> <name>.png, a sprite font ->
                                        <name>.png + <name>.json (glyphs, cropping, characters,
                                        spacing, kerning), a sound effect -> <name>.wav (XMA2 decoded by
                                        ffmpeg: formats/xbox/xma.py), an Xbox 360 effect -> <name>.fx, its
                                        HLSL source (decompile/xbox_effect.py; the record names the sha1
                                        of the effect's bytes, which the PC runtime finds it by);
                                        anything else -> kind "other"
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

from codebreaker.formats import native

SURFACE_FORMATS = {0: "Color", 1: "Bgr565", 2: "Bgra5551", 3: "Bgra4444", 4: "Dxt1", 5: "Dxt3", 6: "Dxt5",
                   7: "NormalizedByte2", 8: "NormalizedByte4", 9: "Rgba1010102", 10: "Rg32", 11: "Rgba64",
                   12: "Alpha8", 13: "Single", 14: "Vector2", 15: "Vector4", 16: "HalfSingle",
                   17: "HalfVector2", 18: "HalfVector4", 19: "HdrBlendable"}
_DXT = {4: "dxt1", 5: "dxt3", 6: "dxt5"}


class Xnb:
    def __init__(self, raw):
        (self.platform, self.version, self.flags, self.data, self.readers, self.shared,
         self.offset) = native.call("xnb.read", raw)           # (formats.dll's: the header, the LZX frames, the manifest)


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


def short(reader):
    """Microsoft.Xna.Framework.Content.Texture2DReader -> Texture2D."""
    return reader.rsplit(".", 1)[-1].split("`")[0].replace("Reader", "")


def read(path):
    with open(path, "rb") as f:
        return Xnb(f.read())


@native.answered("xnb.texture")
def _texture(data, o):
    """Texture2DReader: ({format, width, height, levels, mips}, the offset after it)."""


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


@native.answered("xnb.sprite_font")
def _sprite_font(data, o):
    """XNA 4.0 SpriteFontReader: a texture, glyph and cropping rectangles, characters, line spacing,
    spacing, kerning and the default character."""


@native.answered("xnb.effect_code")
def effect_code(data, o):
    """EffectReader: the compiled effect's bytes."""


@native.answered("xnb.sound_effect")
def _sound_effect(data, o, platform):
    """SoundEffectReader: the format (a WAVEFORMATEX), the samples, the loop and the duration."""


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


XBOX_EFFECT = b"\xbc\xf0\x0b\xcf"                        # an XNA effect's magic, as the Xbox 360 writes it


def write_effect(code, name, out_dir):
    """An Xbox 360 effect's bytes as <name>.fx in out_dir, its first line "// sha1 <the bytes' sha1>"
    (the PC runtime finds the compiled effect by it), and <name>.fx.json: every parameter it declares (the
    port gives the compiled effect back the ones no shader reads: xna/mgfx.py). -> sha1"""
    import hashlib
    from codebreaker.decompile import xbox_effect
    from codebreaker.formats.xbox import effect
    digest = hashlib.sha1(code).hexdigest()
    text = xbox_effect.to_fx(code, name)
    os.makedirs(out_dir, exist_ok=True)
    with open(os.path.join(out_dir, name + ".fx"), "w", encoding="utf-8", newline="\n") as f:
        f.write("// sha1 %s\n%s" % (digest, text))
    with open(os.path.join(out_dir, name + ".fx.json"), "w", encoding="utf-8") as f:
        json.dump([_parameter(p) for p in effect.read(code).parameters], f, indent=1)
    return digest


def _parameter(p):
    """An effect parameter's declaration as data (its value when it is numbers)."""
    numbers = isinstance(p.value, list) and all(isinstance(v, (int, float)) for v in p.value)
    return {"name": p.name, "semantic": p.semantic, "type": p.type, "cls": p.cls, "rows": p.rows,
            "columns": p.columns, "elements": p.elements, "value": p.value if numbers and not p.elements else None,
            "members": [_parameter(m) for m in p.members]}


def convert_effect_file(path, out_dir):
    """A file that is an Xbox 360 effect on its own (a game reads it and makes an Effect of the bytes) -> its
    record, or None when the file is not one."""
    with open(path, "rb") as f:
        code = f.read()
    if code[:4] != XBOX_EFFECT:
        return None
    name = os.path.splitext(os.path.basename(path))[0]
    rec = {"file": os.path.basename(path), "kind": "effect"}
    try:
        rec["sha1"] = write_effect(code, name, out_dir)
    except Exception as e:
        rec["error"] = "%s: %s" % (type(e).__name__, e)
    return rec


def convert(path, out_dir):
    name = os.path.splitext(os.path.basename(path))[0]
    rec = {"file": os.path.basename(path)}
    if os.path.getsize(path) == 0:
        rec["kind"] = "empty"                                # the package has it, with nothing in it
        return rec
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
        elif primary == "Effect" and x.platform == "x":
            rec["kind"] = "effect"
            rec["sha1"] = write_effect(effect_code(x.data, x.offset), name, out_dir)
        else:
            rec["kind"] = "other"
    except Exception as e:                                   # one bad file is reported, the rest go on
        rec["error"] = "%s: %s" % (type(e).__name__, e)
    return rec
