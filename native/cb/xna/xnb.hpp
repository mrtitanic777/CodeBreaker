// cb/xna/xnb.hpp -- XNA Game Studio content (.xnb): textures and sprite fonts out as PNG (and JSON), sounds as WAV, Xbox 360 effects as HLSL,
// the rest catalogued.
//
//     xnb::Xnb x = xnb::read(raw);                      x.platform ('x' Xbox 360, 'w' Windows, 'm' Phone), x.version, x.readers, x.data
//     xnb::Record r = xnb::convert(file, out_dir, env); the record of one file: kind, what it made, or its error
//     xnb::rename_readers(raw, rename)                  the file with each type reader's name passed through `rename`
//
// A .xnb starts "XNB", a platform letter, the version (5 for XNA 4.0) and flags (0x80: compressed), then the file's size; compressed content
// gives its decompressed size and comes in LZX frames. The stream lists its type readers, then the primary object. Integers in the stream are
// little-endian on every platform, but an Xbox 360 texture's pixels are its GPU's: DXT blocks in 16-bit big-endian words, and a Color pixel
// as a big-endian dword (A, B, G, R in file order). Xbox textures in XNB are linear, not tiled.
//
// What `convert` writes, by the file's primary type reader:
//     Texture2D     <name>.png
//     SpriteFont    <name>.png and <name>.json (glyphs, cropping, characters, spacing, kerning)
//     SoundEffect   <name>.wav (PCM and ADPCM as they are; XMA2 decoded by ffmpeg, which the environment names)
//     Effect (Xbox) <name>.fx -- its HLSL (decompile/xbox_effect), its first line "// sha1 <the effect's bytes' SHA-1>", which the PC runtime
//                   finds the compiled effect by -- and <name>.fx.json, every parameter the effect declares (xna/mgfx gives the compiled effect
//                   back the ones no shader reads)
// anything else is kind "other". A file the converter cannot read is reported in its record ("error"); the rest go on.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/json_fwd.hpp"

namespace cb::xna::xnb {

// The surface formats a texture is stored in, by the number the file gives.
std::string_view surface_format_name(std::int32_t format);

struct Xnb {
    char platform = 'w';
    std::uint8_t version = 0;
    std::uint8_t flags = 0;
    Bytes data;                         // the content stream, decompressed
    std::vector<std::string> readers;   // the type readers' names, as far as the first comma
    std::size_t offset = 0;             // where the primary object starts: after the readers and the shared-resource count
};

// FormatError when the bytes are not an XNB (or a compressed one that does not decompress).
Xnb read(ByteView raw);
Xnb read_file(const std::filesystem::path& file);

// `Microsoft.Xna.Framework.Content.Texture2DReader` -> `Texture2D`.
std::string short_name(std::string_view reader);

// `raw` with each type reader's name (the whole name, with its assembly) passed through `rename`: the new file, uncompressed and otherwise the
// same -- or nothing when `rename` changed none.
std::optional<Bytes> rename_readers(ByteView raw, const std::function<std::string(const std::string&)>& rename);

// ---- the content ------------------------------------------------------------------------------------------------------------

struct Texture {
    std::int32_t format = 0;
    std::uint32_t width = 0, height = 0, levels = 0;
    std::vector<Bytes> mips;
    std::size_t end = 0;                // where the stream goes on
};

// Texture2DReader at `offset` (the type reader's index comes first).
Texture read_texture(ByteView data, std::size_t offset);

// The first level as straight RGBA. UnsupportedError for a format that is not converted.
struct Pixels {
    std::uint32_t width = 0, height = 0;
    Bytes rgba;
};
Pixels texture_pixels(const Texture& texture, char platform);

struct Rectangle {
    std::int32_t x = 0, y = 0, width = 0, height = 0;
};
struct Kerning {
    float left = 0, width = 0, right = 0;
};
struct SpriteFont {
    Texture texture;
    std::vector<Rectangle> glyphs, cropping;
    std::vector<std::string> characters;   // one character each (UTF-8)
    std::int32_t line_spacing = 0;
    float spacing = 0;
    std::vector<Kerning> kerning;
};
// XNA 4.0's SpriteFontReader. FormatError when the lists disagree in length.
SpriteFont read_sprite_font(ByteView data, std::size_t offset);

struct SoundEffect {
    std::uint32_t tag = 0, channels = 0, rate = 0, bits = 0;
    Bytes format;                          // the WAVEFORMATEX, little-endian
    Bytes samples;
    std::int32_t loop_start = 0, loop_length = 0, duration_ms = 0;
};
// SoundEffectReader (an Xbox 360's format and 16-bit PCM are byte-swapped to the PC's).
SoundEffect read_sound_effect(ByteView data, std::size_t offset, char platform);

// EffectReader: the compiled effect's bytes.
Bytes read_effect_code(ByteView data, std::size_t offset);

// ---- converting ---------------------------------------------------------------------------------------------------------------

// What conversion needs besides the file: the ffmpeg program that decodes XMA2 (nothing: such a sound is an error).
struct Environment {
    std::optional<std::filesystem::path> ffmpeg;
};

// A sound effect as a WAV a PC plays: PCM and ADPCM as they are, XMA2 decoded to PCM by ffmpeg. ToolError when ffmpeg is missing or fails.
void write_wav(const SoundEffect& sound, const std::filesystem::path& out, const Environment& env);

// An Xbox 360 effect's bytes as <name>.fx and <name>.fx.json in `out_dir`. -> the SHA-1 (hex) of the bytes.
std::string write_effect(ByteView code, std::string_view name, const std::filesystem::path& out_dir);

// The magic an XNA effect starts with, as the Xbox 360 writes it.
inline constexpr std::uint8_t kEffectMagic[4] = {0xBC, 0xF0, 0x0B, 0xCF};

// The record of one .xnb ("file", "platform", "readers", "kind" ... or "error"), as JSON text. An empty file is kind "empty".
std::string convert(const std::filesystem::path& file, const std::filesystem::path& out_dir, const Environment& env = {});
// A file that is an Xbox 360 effect on its own (a game reads it and makes an Effect of the bytes): its record, or nothing when it is not one.
std::optional<std::string> convert_effect_file(const std::filesystem::path& file, const std::filesystem::path& out_dir);

}  // namespace cb::xna::xnb
