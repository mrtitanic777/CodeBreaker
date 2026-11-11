// codebreaker/formats/xnb.hpp -- typed C++ from codebreaker.formats.xnb.
//
// xnb.py -- XNA Game Studio content (.xnb): textures and sprite fonts out as PNG (and JSON), the rest
// catalogued.
//
//     Xnb x = xnb::read(path);            x.platform ('x' Xbox 360, 'w' Windows, 'm' Phone), x.version,
//                                         x.readers [type reader names], x.data (the content stream)
//     xnb::convert(path, out_dir)         {"file", "kind", ...}: a texture -> <name>.png, a sprite font ->
//                                         <name>.png + <name>.json (glyphs, cropping, characters,
//                                         spacing, kerning), a sound effect -> <name>.wav (XMA2 decoded by
//                                         ffmpeg: formats/xbox/xma.py), an Xbox 360 effect -> <name>.fx, its
//                                         HLSL source (decompile/xbox_effect.py; the record names the sha1
//                                         of the effect's bytes, which the PC runtime finds it by);
//                                         anything else -> kind "other"
//     xnb::rename_readers(raw, rename)    the file with each type reader name passed through rename(name), as
//                                         uncompressed XNB bytes -- or None when no name changed
//
// A .xnb starts "XNB", a platform letter, the version (5 for XNA 4.0) and flags (0x80: compressed),
// then the file's size; compressed content gives its decompressed size and comes in LZX frames. The
// stream lists its type readers, then the primary object. Integers in the stream are little-endian
// on every platform, but an Xbox 360 texture's pixels are its GPU's: DXT blocks in 16-bit
// big-endian words, and a Color pixel as a big-endian dword (A, B, G, R in file order). Xbox
// textures in XNB are linear, not tiled.
//
// Typing notes:
//   * xnb.read / texture / sprite_font / effect_code / sound_effect are formats.dll's questions (native/formats/xbox/graphics.cc,
//     linked in and asked through cbf_call): the answers are the records Texture, SpriteFont, Sound and the Xnb fields.  An
//     .xnb's `shared` count is an int that can pass int64 only in a damaged file (the reader's answer then is OverflowError-class).
//   * The Pillow images are `texture::Image` (width, height, RGBA pixels); save() is save_png() here: a PNG of 8-bit RGBA written
//     with the runtime's zlib (Pillow's own bytes differ in deflate details: the pixels they decode to are the same).
//   * `short` is a C++ keyword: short_().
//   * convert()'s record is the Python's dict, in the order its keys were set: py::dict<str, RecValue> (RecValue: an int, a
//     str, a list of str (the readers) or of int (a sound's loop)).
//   * The JSON files (json.dump(..., indent=1)) are written by Json::dumps below, as Python writes them (a text file: "\r\n"
//     line ends on Windows).
//   * Replaces the hooks `xnb.convert` and `xnb_rename_readers` of xna::port: convert() / rename_readers().
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/formats/xbox/texture.hpp"
#include "py.hpp"

namespace codebreaker::formats::xnb {

extern const py::dict<int64_t, py::str> SURFACE_FORMATS;
extern const py::dict<int64_t, py::str> _DXT;
extern const py::bytes XBOX_EFFECT;       // an XNA effect's magic, as the Xbox 360 writes it

class Xnb {
  public:
    py::str platform;                      // 'x' / 'w' / 'm' (the platform byte as a character)
    int64_t version = 0;
    int64_t flags = 0;
    py::bytes data;
    py::list<py::str> readers;
    int64_t shared = 0;
    int64_t offset = 0;

    explicit Xnb(const py::bytes& raw);
};

// (value, the offset after it)
std::tuple<int64_t, int64_t> _7bit(const py::bytes& b, int64_t o);
std::tuple<py::str, int64_t> _string(const py::bytes& b, int64_t o);
py::bytes _7bit_bytes(int64_t value);

// `raw` (an .xnb's bytes) with each type reader's name passed through rename(name) -> the new file,
// uncompressed and otherwise the same, or None when rename changed none.
std::optional<py::bytes> rename_readers(const py::bytes& raw, const std::function<py::str(const py::str&)>& rename);

// Microsoft.Xna.Framework.Content.Texture2DReader -> Texture2D.
py::str short_(const py::str& reader);

Xnb read(const py::str& path);

// Texture2DReader: {format, width, height, levels, mips}
struct Texture {
    int64_t format = 0;
    int64_t width = 0;
    int64_t height = 0;
    int64_t levels = 0;
    py::list<py::bytes> mips;
};
template <class F> void for_each_field_mut(Texture& t, F&& f) {
    f("format", t.format, true);
    f("width", t.width, true);
    f("height", t.height, true);
    f("levels", t.levels, true);
    f("mips", t.mips, true);
}

// (the texture, the offset after it)
std::tuple<Texture, int64_t> _texture(const py::bytes& data, int64_t o);

// The first level as an RGBA image.
formats::xbox::texture::Image texture_image(const Texture& tex, const py::str& platform);

// XNA 4.0 SpriteFontReader: a texture, glyph and cropping rectangles, characters, line spacing, spacing, kerning.
struct SpriteFont {
    Texture texture;
    py::list<py::list<int64_t>> glyphs;
    py::list<py::list<int64_t>> cropping;
    py::list<py::str> characters;
    int64_t line_spacing = 0;
    double spacing = 0;
    py::list<py::list<double>> kerning;
};
template <class F> void for_each_field_mut(SpriteFont& s, F&& f) {
    f("texture", s.texture, true);
    f("glyphs", s.glyphs, true);
    f("cropping", s.cropping, true);
    f("characters", s.characters, true);
    f("line_spacing", s.line_spacing, true);
    f("spacing", s.spacing, true);
    f("kerning", s.kerning, true);
}
SpriteFont _sprite_font(const py::bytes& data, int64_t o);

// EffectReader: the compiled effect's bytes.
py::bytes effect_code(const py::bytes& data, int64_t o);

// SoundEffectReader: the format (a WAVEFORMATEX), the samples, the loop and the duration.
struct Sound {
    int64_t tag = 0;
    int64_t channels = 0;
    int64_t rate = 0;
    int64_t bits = 0;
    py::bytes fmt;
    py::bytes samples;
    py::list<int64_t> loop;
    int64_t duration_ms = 0;
};
template <class F> void for_each_field_mut(Sound& s, F&& f) {
    f("tag", s.tag, true);
    f("channels", s.channels, true);
    f("rate", s.rate, true);
    f("bits", s.bits, true);
    f("fmt", s.fmt, true);
    f("samples", s.samples, true);
    f("loop", s.loop, true);
    f("duration_ms", s.duration_ms, true);
}
Sound _sound_effect(const py::bytes& data, int64_t o, const py::str& platform);

py::bytes _riff(const py::bytes& fmt, const py::bytes& samples);

// A sound effect as a WAV a PC plays: PCM and ADPCM as they are, XMA2 decoded to PCM by ffmpeg.
void write_wav(const Sound& sound, const py::str& out);

// An Xbox 360 effect's bytes as <name>.fx in out_dir, its first line "// sha1 <the bytes' sha1>"
// (the PC runtime finds the compiled effect by it), and <name>.fx.json: every parameter it declares (the
// port gives the compiled effect back the ones no shader reads: xna/mgfx.py). -> sha1
py::str write_effect(const py::bytes& code, const py::str& name, const py::str& out_dir);

// A file that is an Xbox 360 effect on its own (a game reads it and makes an Effect of the bytes) -> its
// record, or None when the file is not one.
using RecValue = std::variant<int64_t, py::str, py::list<py::str>, py::list<int64_t>>;
using Record = py::dict<py::str, RecValue>;
std::optional<Record> convert_effect_file(const py::str& path, const py::str& out_dir);

// the file's record: {"file", "kind", ...} (see the module's description)
Record convert(const py::str& path, const py::str& out_dir);

// a PNG of the image (8-bit RGBA)
void save_png(const formats::xbox::texture::Image& image, const py::str& path);

}  // namespace codebreaker::formats::xnb
