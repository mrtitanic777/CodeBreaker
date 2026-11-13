// cb/xna/xnb.cpp -- see xnb.hpp.
#include "cb/xna/xnb.hpp"

#include <algorithm>
#include <bit>
#include <format>

#include <nlohmann/json.hpp>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/json.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/xbox_effect.hpp"
#include "cb/formats/lzx.hpp"
#include "cb/formats/xbox/effect.hpp"
#include "cb/formats/xbox/texture.hpp"
#include "cb/java/rgba_png.hpp"

namespace cb::xna::xnb {

namespace {

constexpr std::string_view kReader = "xnb";

// A 7-bit encoded integer (the content stream's length prefix). A value past what a file can hold saturates.
std::uint64_t read_7bit(io::ByteReader& r) {
    std::uint64_t value = 0;
    unsigned shift = 0;
    for (;;) {
        const std::uint8_t x = r.u8();
        if (shift >= 56) value = std::uint64_t{1} << 60;
        else value |= std::uint64_t{x & 0x7Fu} << shift;
        if ((x & 0x80) == 0) return value;
        shift += 7;
    }
}

void write_7bit(Bytes& out, std::uint64_t value) {
    for (;;) {
        const std::uint8_t x = static_cast<std::uint8_t>(value & 0x7F);
        value >>= 7;
        out.push_back(static_cast<std::uint8_t>(x | (value != 0 ? 0x80 : 0)));
        if (value == 0) return;
    }
}

std::string read_string(io::ByteReader& r) {
    const std::uint64_t n = read_7bit(r);
    const ByteView bytes = r.bytes(n);
    return std::string(bytes.begin(), bytes.end());
}

float read_f32(io::ByteReader& r) { return std::bit_cast<float>(r.u32()); }

// The content of a compressed file: its frames put together, then decompressed.
Bytes deframe(ByteView raw, std::uint32_t size) {
    io::ByteReader r(raw, kReader);
    r.seek(14);
    Bytes stream;
    while (r.pos() < size) {
        const std::uint32_t hi = r.u8(), lo = r.u8();
        std::uint32_t block = hi << 8 | lo, frame = 0x8000;
        if (hi == 0xFF) {
            frame = lo << 8 | r.u8();
            const std::uint32_t b1 = r.u8(), b2 = r.u8();
            block = b1 << 8 | b2;
        }
        if (block == 0 || frame == 0) break;
        const ByteView chunk = r.bytes(block);
        stream.insert(stream.end(), chunk.begin(), chunk.end());
    }
    return stream;
}

}  // namespace

std::string_view surface_format_name(std::int32_t format) {
    static constexpr std::string_view names[] = {"Color", "Bgr565", "Bgra5551", "Bgra4444", "Dxt1", "Dxt3", "Dxt5", "NormalizedByte2", "NormalizedByte4",
                                                 "Rgba1010102", "Rg32", "Rgba64", "Alpha8", "Single", "Vector2", "Vector4", "HalfSingle",
                                                 "HalfVector2", "HalfVector4", "HdrBlendable"};
    return format >= 0 && static_cast<std::size_t>(format) < std::size(names) ? names[static_cast<std::size_t>(format)] : std::string_view();
}

Xnb read(ByteView raw) {
    if (raw.size() < 3 || raw[0] != 'X' || raw[1] != 'N' || raw[2] != 'B') throw FormatError(kReader, 0, "not an XNB file");
    io::ByteReader r(raw, kReader);
    r.seek(3);
    Xnb x;
    x.platform = static_cast<char>(r.u8());
    x.version = r.u8();
    x.flags = r.u8();
    const std::uint32_t size = r.u32();
    if ((x.flags & 0x80) != 0) {
        const std::uint32_t out_len = r.u32();
        const Bytes stream = deframe(raw, size);
        x.data = formats::lzx::decompress(stream, 16, out_len);
    } else {
        if (size < 10 || size > raw.size()) throw FormatError(kReader, 6, "the size the header gives is not the file's");
        x.data.assign(raw.begin() + 10, raw.begin() + size);
    }
    io::ByteReader d(x.data, kReader);
    const std::uint64_t count = read_7bit(d);
    for (std::uint64_t k = 0; k < count; ++k) {
        const std::string name = read_string(d);
        d.skip(4);   // the reader's version
        x.readers.push_back(name.substr(0, name.find(',')));
    }
    read_7bit(d);   // the number of shared resources
    x.offset = d.pos();
    return x;
}

Xnb read_file(const std::filesystem::path& file) { return read(fs::read_file(file)); }

std::string short_name(std::string_view reader) {
    const std::size_t dot = reader.rfind('.');
    std::string name(dot == std::string_view::npos ? reader : reader.substr(dot + 1));
    name = name.substr(0, name.find('`'));
    return text::replace_all(name, "Reader", "");
}

std::optional<Bytes> rename_readers(ByteView raw, const std::function<std::string(const std::string&)>& rename) {
    const Xnb x = read(raw);
    io::ByteReader d(x.data, kReader);
    const std::uint64_t count = read_7bit(d);
    Bytes manifest;
    write_7bit(manifest, count);
    bool changed = false;
    for (std::uint64_t k = 0; k < count; ++k) {
        const std::string name = read_string(d);
        const std::string renamed = rename(name);
        changed = changed || renamed != name;
        write_7bit(manifest, renamed.size());
        manifest.insert(manifest.end(), renamed.begin(), renamed.end());
        const ByteView version = d.bytes(4);   // and the reader's version
        manifest.insert(manifest.end(), version.begin(), version.end());
    }
    if (!changed) return std::nullopt;
    Bytes body = std::move(manifest);
    body.insert(body.end(), x.data.begin() + static_cast<std::ptrdiff_t>(d.pos()), x.data.end());
    Bytes out{'X', 'N', 'B', static_cast<std::uint8_t>(x.platform), x.version, static_cast<std::uint8_t>(x.flags & ~0x80u)};
    const std::uint32_t total = static_cast<std::uint32_t>(10 + body.size());
    for (int i = 0; i < 4; ++i) out.push_back(static_cast<std::uint8_t>(total >> (8 * i)));
    out.insert(out.end(), body.begin(), body.end());
    return out;
}

// ---- the content ------------------------------------------------------------------------------------------------------------

Texture read_texture(ByteView data, std::size_t offset) {
    io::ByteReader r(data, kReader);
    r.seek(offset);
    read_7bit(r);   // the type reader of the object
    Texture t;
    t.format = r.i32();
    t.width = r.u32();
    t.height = r.u32();
    t.levels = r.u32();
    for (std::uint32_t k = 0; k < t.levels; ++k) {
        const std::uint32_t size = r.u32();
        const ByteView mip = r.bytes(size);
        t.mips.emplace_back(mip.begin(), mip.end());
    }
    t.end = r.pos();
    return t;
}

Pixels texture_pixels(const Texture& tex, char platform) {
    namespace tx = formats::xbox::texture;
    if (tex.mips.empty()) throw FormatError(kReader, 0, "the texture has no levels");
    Pixels out;
    out.width = tex.width;
    out.height = tex.height;
    const ByteView level = tex.mips[0];
    if (tex.format >= 4 && tex.format <= 6) {
        const tx::Format format = tex.format == 4 ? tx::Format::Dxt1 : tex.format == 5 ? tx::Format::Dxt3 : tx::Format::Dxt5;
        tx::Image image = tx::decode_dxt(level, tex.width, tex.height, format, {.tiled = false, .swapped = platform == 'x'});
        out.rgba = std::move(image.rgba);
        return out;
    }
    if (tex.format == 0) {
        const std::size_t n = std::size_t{tex.width} * tex.height * 4;
        if (level.size() < n) throw FormatError(kReader, 0, "the texture's pixels are cut short");
        out.rgba.assign(level.begin(), level.begin() + static_cast<std::ptrdiff_t>(n));
        if (platform == 'x') {   // A, B, G, R in file order
            for (std::size_t i = 0; i < n; i += 4) {
                const std::uint8_t a = out.rgba[i], b = out.rgba[i + 1], g = out.rgba[i + 2], rr = out.rgba[i + 3];
                out.rgba[i] = rr;
                out.rgba[i + 1] = g;
                out.rgba[i + 2] = b;
                out.rgba[i + 3] = a;
            }
        }
        return out;
    }
    const std::string_view name = surface_format_name(tex.format);
    throw UnsupportedError(kReader, 0, "texture format " + (name.empty() ? std::to_string(tex.format) : std::string(name)) + " is not converted");
}

namespace {

std::vector<Rectangle> read_rectangles(io::ByteReader& r) {
    read_7bit(r);   // the list's type reader
    const std::uint32_t n = r.u32();
    std::vector<Rectangle> out;
    for (std::uint32_t i = 0; i < n; ++i) {
        Rectangle rect;
        rect.x = r.i32();
        rect.y = r.i32();
        rect.width = r.i32();
        rect.height = r.i32();
        out.push_back(rect);
    }
    return out;
}

// One UTF-8 character at the cursor (a character in a sprite font's list is stored as UTF-8 bytes, not length-prefixed).
std::string read_character(io::ByteReader& r) {
    const std::uint8_t b0 = r.peek_u8().value_or(0);
    const std::size_t size = b0 < 0x80 ? 1 : b0 < 0xE0 ? 2 : b0 < 0xF0 ? 3 : 4;
    const ByteView bytes = r.bytes(size);
    return std::string(bytes.begin(), bytes.end());
}

}  // namespace

SpriteFont read_sprite_font(ByteView data, std::size_t offset) {
    io::ByteReader r(data, kReader);
    r.seek(offset);
    read_7bit(r);   // the type reader of the object
    SpriteFont font;
    font.texture = read_texture(data, r.pos());
    r.seek(font.texture.end);
    font.glyphs = read_rectangles(r);
    font.cropping = read_rectangles(r);
    read_7bit(r);
    const std::uint32_t n = r.u32();
    if (!(font.glyphs.size() == font.cropping.size() && font.cropping.size() == n))
        throw FormatError(kReader, r.pos(), std::format("sprite font lists disagree: {} glyphs, {} cropping, {} characters", font.glyphs.size(),
                                                        font.cropping.size(), n));
    for (std::uint32_t k = 0; k < n; ++k) font.characters.push_back(read_character(r));
    font.line_spacing = r.i32();
    font.spacing = read_f32(r);
    read_7bit(r);
    const std::uint32_t kerns = r.u32();
    for (std::uint32_t i = 0; i < kerns; ++i) {
        Kerning k;
        k.left = read_f32(r);
        k.width = read_f32(r);
        k.right = read_f32(r);
        font.kerning.push_back(k);
    }
    return font;
}

SoundEffect read_sound_effect(ByteView data, std::size_t offset, char platform) {
    io::ByteReader r(data, kReader);
    r.seek(offset);
    read_7bit(r);
    SoundEffect s;
    const std::uint32_t format_size = r.u32();
    const ByteView format = r.bytes(format_size);
    s.format.assign(format.begin(), format.end());
    const std::uint32_t sample_size = r.u32();
    const ByteView samples = r.bytes(sample_size);
    s.samples.assign(samples.begin(), samples.end());
    s.loop_start = r.i32();
    s.loop_length = r.i32();
    s.duration_ms = r.i32();
    if (platform == 'x') {
        // an Xbox 360's WAVEFORMATEX (and, for XMA2, its extension) is big-endian field by field
        std::vector<int> widths = {2, 2, 4, 4, 2, 2, 2};
        if (s.format.size() >= 2 && ((s.format[0] << 8) | s.format[1]) == 0x166)
            for (const int w : {2, 4, 4, 4, 4, 4, 4, 4, 1, 1, 2}) widths.push_back(w);
        std::size_t total = 0;
        for (const int w : widths) total += static_cast<std::size_t>(w);
        std::size_t take = s.format.size() >= total ? total : 18;
        if (take == 18) widths.resize(7);
        if (s.format.size() < take) throw TruncatedError(kReader, offset, take, s.format.size());
        std::size_t at = 0;
        Bytes swapped;
        for (const int w : widths) {
            for (int k = w - 1; k >= 0; --k) swapped.push_back(s.format[at + static_cast<std::size_t>(k)]);
            at += static_cast<std::size_t>(w);
        }
        std::copy(swapped.begin(), swapped.end(), s.format.begin());
    }
    io::ByteReader f(s.format, kReader);
    s.tag = f.u16();
    s.channels = f.u16();
    s.rate = f.u32();
    f.u32();
    f.u16();
    s.bits = f.u16();
    if (platform == 'x' && s.tag == 1 && s.bits == 16) {
        for (std::size_t i = 0; i + 1 < s.samples.size(); i += 2) std::swap(s.samples[i], s.samples[i + 1]);
    }
    return s;
}

Bytes read_effect_code(ByteView data, std::size_t offset) {
    io::ByteReader r(data, kReader);
    r.seek(offset);
    read_7bit(r);
    const std::uint32_t n = r.u32();
    return r.bytes_copy(n);
}

// ---- converting ---------------------------------------------------------------------------------------------------------------

namespace {

void put_u16(Bytes& b, std::uint32_t v) {
    b.push_back(static_cast<std::uint8_t>(v));
    b.push_back(static_cast<std::uint8_t>(v >> 8));
}
void put_u32(Bytes& b, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b.push_back(static_cast<std::uint8_t>(v >> (8 * i)));
}
void put_chunk(Bytes& out, std::string_view tag, ByteView body) {
    out.insert(out.end(), tag.begin(), tag.end());
    put_u32(out, static_cast<std::uint32_t>(body.size()));
    out.insert(out.end(), body.begin(), body.end());
    if (body.size() % 2 != 0) out.push_back(0);
}

// The RIFF file of a format and its samples.
Bytes riff(ByteView format, ByteView samples) {
    Bytes body{'W', 'A', 'V', 'E'};
    put_chunk(body, "fmt ", format);
    put_chunk(body, "data", samples);
    Bytes out{'R', 'I', 'F', 'F'};
    put_u32(out, static_cast<std::uint32_t>(body.size()));
    out.insert(out.end(), body.begin(), body.end());
    return out;
}

Json parameter_json(const formats::xbox::effect::Parameter& p) {
    Json j = Json::object();
    j["name"] = p.name;
    j["semantic"] = p.semantic;
    j["type"] = std::string(formats::xbox::d3dx::type_name(p.type));
    j["cls"] = std::string(formats::xbox::d3dx::class_name(p.cls));
    j["rows"] = p.rows;
    j["columns"] = p.columns;
    j["elements"] = p.elements;
    j["value"] = nullptr;
    if (p.elements == 0) {
        if (!p.floats().empty()) j["value"] = std::vector<float>(p.floats().begin(), p.floats().end());
        else if (!p.ints().empty()) j["value"] = std::vector<std::int32_t>(p.ints().begin(), p.ints().end());
        else if (formats::xbox::d3dx::is_sampler_type(p.type) && p.states.empty()) j["value"] = Json::array();   // a sampler that sets nothing
    }
    Json members = Json::array();
    for (const auto& m : p.members) members.push_back(parameter_json(m));
    j["members"] = members;
    return j;
}

}  // namespace

void write_wav(const SoundEffect& sound, const std::filesystem::path& out, const Environment& env) {
    const Bytes file = riff(sound.format, sound.samples);
    if (sound.tag != 0x166 && sound.tag != 0x165) {
        fs::write_file(out, file);
        return;
    }
    if (!env.ffmpeg) throw ToolError("ffmpeg", 0, "ffmpeg not found: it decodes XMA2 (settings.json: ffmpeg)");
    fs::ScopedTempDir folder{"cb-xma"};
    const std::filesystem::path in = folder.path() / "in.xma.wav";
    fs::write_file(in, file);
    // 16-bit PCM in a plain WAVEFORMATEX: with the decoder's own layout ffmpeg writes WAVE_FORMAT_EXTENSIBLE, which MonoGame's
    // (and many players') WAV readers do not take
    const process::ProcessResult r = process::run({.argv = {fs::to_text(*env.ffmpeg), "-y", "-hide_banner", "-loglevel", "error", "-i", fs::to_text(in),
                                                            "-c:a", "pcm_s16le", "-ac", std::to_string(std::min<std::uint32_t>(sound.channels, 2)),
                                                            fs::to_text(out)},
                                                   .err = process::Capture::Merge});
    std::error_code ec;
    if (r.exit_code != 0 || !std::filesystem::exists(out, ec))
        throw ToolError("ffmpeg", r.exit_code, "XMA2 decode failed: " + std::string(r.out.begin(), r.out.end()));
}

std::string write_effect(ByteView code, std::string_view name, const std::filesystem::path& out_dir) {
    const std::string digest = crypto::to_hex(crypto::sha1(code));
    const std::string hlsl = decompile::xbox_effect::to_fx(code, name);
    fs::make_dirs(out_dir);
    fs::write_text(out_dir / (std::string(name) + ".fx"), "// sha1 " + digest + "\n" + hlsl);
    Json parameters = Json::array();
    for (const auto& p : formats::xbox::effect::read(code).parameters) parameters.push_back(parameter_json(p));
    json::save(out_dir / (std::string(name) + ".fx.json"), parameters);
    return digest;
}

std::optional<std::string> convert_effect_file(const std::filesystem::path& file, const std::filesystem::path& out_dir) {
    const Bytes code = fs::read_file(file);
    if (code.size() < 4 || !std::equal(code.begin(), code.begin() + 4, std::begin(kEffectMagic))) return std::nullopt;
    Json rec = Json::object();
    rec["file"] = fs::to_text(file.filename());
    rec["kind"] = "effect";
    try {
        rec["sha1"] = write_effect(code, fs::to_text(file.stem()), out_dir);
    } catch (const Error& e) {
        rec["error"] = e.what();
    }
    return json::dump(rec, json::Style::Compact);
}

std::string convert(const std::filesystem::path& file, const std::filesystem::path& out_dir, const Environment& env) {
    const std::string name = fs::to_text(file.stem());
    Json rec = Json::object();
    rec["file"] = fs::to_text(file.filename());
    std::error_code ec;
    if (std::filesystem::file_size(file, ec) == 0) {
        rec["kind"] = "empty";   // the package has it, with nothing in it
        return json::dump(rec, json::Style::Compact);
    }
    try {
        const Xnb x = read_file(file);
        std::vector<std::string> kinds;
        for (const std::string& reader : x.readers) kinds.push_back(short_name(reader));
        rec["platform"] = std::string(1, x.platform);
        rec["readers"] = kinds;
        const std::string primary = kinds.empty() ? "?" : kinds[0];
        fs::make_dirs(out_dir);
        const auto save_png = [&](const Texture& tex) {
            const Pixels pixels = texture_pixels(tex, x.platform);
            java::rgba_png::Image image;
            image.width = pixels.width;
            image.height = pixels.height;
            image.rgba = pixels.rgba;
            fs::write_file(out_dir / (name + ".png"), java::rgba_png::encode(image));
        };
        if (primary == "Texture2D") {
            const Texture tex = read_texture(x.data, x.offset);
            save_png(tex);
            rec["kind"] = "texture";
            const std::string_view format = surface_format_name(tex.format);
            rec["format"] = format.empty() ? Json(tex.format) : Json(std::string(format));
            rec["width"] = tex.width;
            rec["height"] = tex.height;
            rec["levels"] = tex.levels;
        } else if (primary == "SpriteFont") {
            const SpriteFont font = read_sprite_font(x.data, x.offset);
            save_png(font.texture);
            Json doc = Json::object();
            const auto rects = [](const std::vector<Rectangle>& list) {
                Json out = Json::array();
                for (const Rectangle& r : list) out.push_back(Json::array({r.x, r.y, r.width, r.height}));
                return out;
            };
            doc["glyphs"] = rects(font.glyphs);
            doc["cropping"] = rects(font.cropping);
            doc["characters"] = font.characters;
            doc["line_spacing"] = font.line_spacing;
            doc["spacing"] = font.spacing;
            Json kerning = Json::array();
            for (const Kerning& k : font.kerning) kerning.push_back(Json::array({k.left, k.width, k.right}));
            doc["kerning"] = kerning;
            json::save(out_dir / (name + ".json"), doc);
            rec["kind"] = "font";
            rec["width"] = font.texture.width;
            rec["height"] = font.texture.height;
            rec["glyphs"] = font.glyphs.size();
        } else if (primary == "SoundEffect") {
            const SoundEffect sound = read_sound_effect(x.data, x.offset, x.platform);
            write_wav(sound, out_dir / (name + ".wav"), env);
            rec["kind"] = "sound";
            switch (sound.tag) {
            case 1: rec["codec"] = "PCM"; break;
            case 2: rec["codec"] = "ADPCM"; break;
            case 0x166: rec["codec"] = "XMA2"; break;
            case 0x165: rec["codec"] = "XMA"; break;
            default: rec["codec"] = sound.tag; break;
            }
            rec["channels"] = sound.channels;
            rec["rate"] = sound.rate;
            rec["duration_ms"] = sound.duration_ms;
            rec["loop"] = Json::array({sound.loop_start, sound.loop_length});
        } else if (primary == "Effect" && x.platform == 'x') {
            rec["kind"] = "effect";
            rec["sha1"] = write_effect(read_effect_code(x.data, x.offset), name, out_dir);
        } else {
            rec["kind"] = "other";
        }
    } catch (const std::exception& e) {   // one bad file is reported, the rest go on
        rec["error"] = e.what();
    }
    return json::dump(rec, json::Style::Compact);
}

}  // namespace cb::xna::xnb
