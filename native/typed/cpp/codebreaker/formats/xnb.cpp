// codebreaker/formats/xnb.cpp -- see xnb.hpp.

#include "codebreaker/formats/xnb.hpp"

#include <cmath>

#include "codebreaker/decompile/xbox_effect.hpp"
#include "codebreaker/formats/native_ask.hpp"
#include "codebreaker/formats/xbox/effect.hpp"
#include "codebreaker/formats/xbox/xma.hpp"
#include "py_json.hpp"
#include "py_os.hpp"
#include "py_zlib.hpp"

namespace codebreaker::formats::xnb {

namespace na = native_ask;
namespace tex = formats::xbox::texture;

const py::dict<int64_t, py::str> SURFACE_FORMATS = {
    {0, py::str("Color")},        {1, py::str("Bgr565")},     {2, py::str("Bgra5551")},      {3, py::str("Bgra4444")},    {4, py::str("Dxt1")},
    {5, py::str("Dxt3")},         {6, py::str("Dxt5")},       {7, py::str("NormalizedByte2")}, {8, py::str("NormalizedByte4")}, {9, py::str("Rgba1010102")},
    {10, py::str("Rg32")},        {11, py::str("Rgba64")},    {12, py::str("Alpha8")},       {13, py::str("Single")},     {14, py::str("Vector2")},
    {15, py::str("Vector4")},     {16, py::str("HalfSingle")}, {17, py::str("HalfVector2")}, {18, py::str("HalfVector4")}, {19, py::str("HdrBlendable")}};
const py::dict<int64_t, py::str> _DXT = {{4, py::str("dxt1")}, {5, py::str("dxt3")}, {6, py::str("dxt5")}};
const py::bytes XBOX_EFFECT("\xbc\xf0\x0b\xcf", 4);

namespace {

// the platform's line end in a text file
const char* const NL =
#ifdef _WIN32
    "\r\n";
#else
    "\n";
#endif

// a 7-bit encoded int that does not fit int64 is OverflowError (Python goes on with a big integer)
[[noreturn]] void too_big() { throw py::OverflowError("a 7-bit encoded integer of more than 63 bits"); }

// ---- json.dump(..., indent=1) -------------------------------------------------------------------------------------
struct Json {
    enum Kind { Null, Int, Float, Str, List, Dict } kind = Null;
    int64_t i = 0;
    double f = 0;
    py::str s;
    std::vector<Json> items;                               // a List's items; a Dict's values
    std::vector<py::str> keys;                             // a Dict's keys

    static Json null() { return Json(); }
    static Json integer(int64_t v) { Json j; j.kind = Int; j.i = v; return j; }
    static Json real(double v) { Json j; j.kind = Float; j.f = v; return j; }
    static Json str(const py::str& v) { Json j; j.kind = Str; j.s = v; return j; }
    static Json list() { Json j; j.kind = List; return j; }
    static Json dict() { Json j; j.kind = Dict; return j; }
    void push(const Json& v) { items.push_back(v); }
    void set(const char* k, const Json& v) { keys.push_back(py::str(k)); items.push_back(v); }
};

// json's string: ensure_ascii as asked
void put_str(std::string& o, const py::str& s, bool ensure_ascii) {
    if (ensure_ascii) {
        py::json::put(o, s);
        return;
    }
    o += '"';
    std::string utf8;
    for (char32_t c : s.u32()) {
        char buf[16];
        if (c == '"') o += "\\\"";
        else if (c == '\\') o += "\\\\";
        else if (c == '\n') o += "\\n";
        else if (c == '\r') o += "\\r";
        else if (c == '\t') o += "\\t";
        else if (c == '\b') o += "\\b";
        else if (c == '\f') o += "\\f";
        else if (c < 0x20) { std::snprintf(buf, sizeof buf, "\\u%04x", unsigned(c)); o += buf; }
        else o += py::str(c).utf8();
    }
    o += '"';
}

void dumps(std::string& o, const Json& v, bool ensure_ascii, int level) {
    auto pad = [&](int n) { o += std::string(size_t(n), ' '); };
    switch (v.kind) {
        case Json::Null: o += "null"; break;
        case Json::Int: o += std::to_string(v.i); break;
        case Json::Float:
            if (std::isnan(v.f)) o += "NaN";
            else if (std::isinf(v.f)) o += v.f > 0 ? "Infinity" : "-Infinity";
            else o += py::to_str(v.f).utf8();
            break;
        case Json::Str: put_str(o, v.s, ensure_ascii); break;
        case Json::List:
            if (v.items.empty()) { o += "[]"; break; }
            o += "[\n";
            for (size_t k = 0; k < v.items.size(); ++k) {
                pad(level + 1);
                dumps(o, v.items[k], ensure_ascii, level + 1);
                o += k + 1 < v.items.size() ? ",\n" : "\n";
            }
            pad(level);
            o += "]";
            break;
        case Json::Dict:
            if (v.items.empty()) { o += "{}"; break; }
            o += "{\n";
            for (size_t k = 0; k < v.items.size(); ++k) {
                pad(level + 1);
                put_str(o, v.keys[k], true);
                o += ": ";
                dumps(o, v.items[k], ensure_ascii, level + 1);
                o += k + 1 < v.items.size() ? ",\n" : "\n";
            }
            pad(level);
            o += "}";
            break;
    }
}

// a text file as Python writes it: "w", a new line is the platform's unless newline="\n"
void write_text(const py::str& path, const std::string& text, bool platform_newline) {
    std::string out;
    if (platform_newline && std::string(NL) != "\n") {
        for (char c : text) {
            if (c == '\n') out += NL;
            else out += c;
        }
    } else out = text;
    py::file f = py::open(path, py::str("wb"));
    f.write(py::bytes(out));
    f.close();
}

// the effect parameter as data (its value when it is numbers)
Json parameter_json(const formats::xbox::effect::Parameter& p) {
    namespace fx = formats::xbox::effect;
    Json d = Json::dict();
    d.set("name", Json::str(p.name));
    d.set("semantic", Json::str(p.semantic));
    d.set("type", Json::str(p.type));
    d.set("cls", Json::str(p.cls));
    d.set("rows", Json::integer(p.rows));
    d.set("columns", Json::integer(p.columns));
    d.set("elements", Json::integer(p.elements));
    // numbers = isinstance(p.value, list) and all(isinstance(v, (int, float)) for v in p.value)
    Json value = Json::null();
    if (!p.elements) {
        const fx::ValueData& v = p.value.data;
        if (std::holds_alternative<py::list<int64_t>>(v)) {
            value = Json::list();
            for (int64_t x : std::get<py::list<int64_t>>(v)) value.push(Json::integer(x));
        } else if (std::holds_alternative<py::list<double>>(v)) {
            value = Json::list();
            for (double x : std::get<py::list<double>>(v)) value.push(Json::real(x));
        } else if (std::holds_alternative<py::list<fx::State>>(v)) {
            if (std::get<py::list<fx::State>>(v).empty()) value = Json::list();          // (all() of nothing)
        } else if (std::holds_alternative<py::list<fx::Value>>(v)) {
            if (std::get<py::list<fx::Value>>(v).empty()) value = Json::list();
        }
    }
    d.set("value", value);
    Json members = Json::list();
    for (const fx::Parameter& m : p.members) members.push(parameter_json(m));
    d.set("members", members);
    return d;
}

// "%s: %s" % (type(e).__name__, e)
py::str error_text(const py::Exception& e) { return py::str(e.type()) + py::str(": ") + py::str(e.what()); }

}  // namespace

// ---- Xnb -----------------------------------------------------------------------------------------------------------
Xnb::Xnb(const py::bytes& raw) {
    // (the header, the LZX frames, the manifest: formats.dll's)
    auto r = na::ask<std::tuple<py::str, int64_t, int64_t, py::bytes, py::list<py::str>, int64_t, int64_t>>("xnb.read", raw, na::no_own);
    std::tie(platform, version, flags, data, readers, shared, offset) = r;
}

std::tuple<int64_t, int64_t> _7bit(const py::bytes& b, int64_t o) {
    int64_t value = 0, shift = 0;
    for (;;) {
        int64_t x = b[o];
        o += 1;
        if (shift >= 63) {
            if (x & 0x7F) too_big();
        } else {
            if (shift > 56 && ((x & 0x7F) >> (63 - shift))) too_big();
            value |= (x & 0x7F) << shift;
        }
        if (!(x & 0x80)) return std::make_tuple(value, o);
        shift += 7;
    }
}

std::tuple<py::str, int64_t> _string(const py::bytes& b, int64_t o) {
    auto nv = _7bit(b, o);
    int64_t n = std::get<0>(nv);
    o = std::get<1>(nv);
    return std::make_tuple(b.slice(o, o + n).decode("utf-8", "replace"), o + n);
}

py::bytes _7bit_bytes(int64_t value) {
    std::string out;
    for (;;) {
        int64_t x = value & 0x7F;
        value >>= 7;
        out.push_back(char(x | (value ? 0x80 : 0)));
        if (!value) return py::bytes(out);
    }
}

std::optional<py::bytes> rename_readers(const py::bytes& raw, const std::function<py::str(const py::str&)>& rename) {
    Xnb x(raw);
    auto cv = _7bit(x.data, 0);
    int64_t count = std::get<0>(cv), o = std::get<1>(cv);
    std::string manifest = _7bit_bytes(count).raw();
    bool changed = false;
    for (int64_t k = 0; k < count; ++k) {
        auto so = _string(x.data, o);
        py::str name = std::get<0>(so);
        int64_t o2 = std::get<1>(so);
        py::str new_ = rename(name);
        changed = changed | (new_ != name);
        py::bytes encoded = new_.encode("utf-8");
        manifest += _7bit_bytes(encoded.size()).raw() + encoded.raw() + x.data.slice(o2, o2 + 4).raw();   // and the reader's version
        o = o2 + 4;
    }
    if (!changed) return std::nullopt;
    py::bytes body = py::bytes(manifest) + x.data.slice(o, x.data.size());
    std::string head = "XNB" + x.platform.encode("utf-8").raw();
    head.push_back(char(x.version));
    head.push_back(char(x.flags & ~0x80));
    return py::bytes(head) + py::pack("<I", 10 + body.size()) + body;
}

py::str short_(const py::str& reader) {
    // reader.rsplit(".", 1)[-1].split("`")[0].replace("Reader", "")
    int64_t dot = reader.rfind(py::str("."));
    py::str last = dot < 0 ? reader : reader.slice(dot + 1, reader.size());
    int64_t tick = last.find(py::str("`"));
    py::str first = tick < 0 ? last : last.slice(0, tick);
    return first.replace(py::str("Reader"), py::str(""));
}

Xnb read(const py::str& path) { return Xnb(py::open_rb(path).read()); }

std::tuple<Texture, int64_t> _texture(const py::bytes& data, int64_t o) {
    // (Texture2DReader: ({format, width, height, levels, mips}, the offset after it))
    auto r = na::ask<std::tuple<Texture, int64_t>>("xnb.texture", data, na::no_own, o);
    return r;
}

tex::Image texture_image(const Texture& t, const py::str& platform) {
    int64_t fmt = t.format, w = t.width, h = t.height;
    py::bytes pixels = t.mips[0];
    if (_DXT.contains(fmt)) return tex::decode_dxt(pixels, w, h, _DXT.at(fmt), false, platform == py::str("x"));
    if (fmt == 0) {
        // Image.frombytes("RGBA", (w, h), bytes(pixels[:w * h * 4]))
        if (w < 0 || h < 0 || (h && w > INT64_MAX / 4 / h)) throw py::MemoryError("");
        int64_t need = w * h * 4;
        py::bytes raw = pixels.slice(0, need);
        if (raw.size() < need) throw py::ValueError("not enough image data");
        tex::Image img;
        img.width = w;
        img.height = h;
        if (platform == py::str("x")) {                         // A, B, G, R in file order
            std::string s = raw.raw();
            for (int64_t k = 0; k < w * h; ++k) {
                char a = s[size_t(4 * k)], b = s[size_t(4 * k + 1)], g = s[size_t(4 * k + 2)], r = s[size_t(4 * k + 3)];
                s[size_t(4 * k)] = r;
                s[size_t(4 * k + 1)] = g;
                s[size_t(4 * k + 2)] = b;
                s[size_t(4 * k + 3)] = a;
            }
            img.pixels = py::bytes(s);
        } else {
            img.pixels = raw;
        }
        return img;
    }
    throw py::ValueError(py::percent(py::str("texture format %s is not converted"),
                                     SURFACE_FORMATS.contains(fmt) ? SURFACE_FORMATS.at(fmt) : py::to_str(fmt)));
}

SpriteFont _sprite_font(const py::bytes& data, int64_t o) { return na::ask<SpriteFont>("xnb.sprite_font", data, na::no_own, o); }

py::bytes effect_code(const py::bytes& data, int64_t o) { return na::ask<py::bytes>("xnb.effect_code", data, na::no_own, o); }

Sound _sound_effect(const py::bytes& data, int64_t o, const py::str& platform) { return na::ask<Sound>("xnb.sound_effect", data, na::no_own, o, platform); }

py::bytes _riff(const py::bytes& fmt, const py::bytes& samples) {
    auto chunk = [](const char* tag, const py::bytes& body) {
        return py::bytes(tag) + py::pack("<I", body.size()) + body + (body.size() % 2 ? py::bytes::zeros(1) : py::bytes());
    };
    py::bytes body = py::bytes("WAVE") + chunk("fmt ", fmt) + chunk("data", samples);
    return py::bytes("RIFF") + py::pack("<I", body.size()) + body;
}

void write_wav(const Sound& sound, const py::str& out) {
    py::bytes riff = _riff(sound.fmt, sound.samples);
    if (sound.tag == 0x166 || sound.tag == 0x165) {
        formats::xbox::xma::decode_to_wav(riff, out, sound.channels);
    } else {
        py::file f = py::open(out, py::str("wb"));
        f.write(riff);
        f.close();
    }
}

py::str write_effect(const py::bytes& code, const py::str& name, const py::str& out_dir) {
    py::str digest = py::hashlib::sha1(code).hexdigest();
    py::str text = decompile::xbox_effect::to_fx(code, name);
    py::os::makedirs(out_dir, 0777, true);
    {
        py::file f = py::open(py::os::path::join(out_dir, name + py::str(".fx")), py::str("w"), py::str("utf-8"), std::nullopt, py::str("\n"));
        f.write(py::str("// sha1 ") + digest + py::str("\n") + text);
        f.close();
    }
    Json params = Json::list();
    for (const auto& p : formats::xbox::effect::read(code).parameters) params.push(parameter_json(p));
    std::string json;
    dumps(json, params, true, 0);
    write_text(py::os::path::join(out_dir, name + py::str(".fx.json")), json, true);
    return digest;
}

std::optional<Record> convert_effect_file(const py::str& path, const py::str& out_dir) {
    py::bytes code = py::open_rb(path).read();
    if (code.slice(0, 4) != XBOX_EFFECT) return std::nullopt;
    py::str name = std::get<0>(py::os::path::splitext(py::os::path::basename(path)));
    Record rec;
    rec.set(py::str("file"), RecValue(py::os::path::basename(path)));
    rec.set(py::str("kind"), RecValue(py::str("effect")));
    try {
        rec.set(py::str("sha1"), RecValue(write_effect(code, name, out_dir)));
    } catch (const py::Exception& e) {
        rec.set(py::str("error"), RecValue(error_text(e)));
    }
    return rec;
}

Record convert(const py::str& path, const py::str& out_dir) {
    py::str name = std::get<0>(py::os::path::splitext(py::os::path::basename(path)));
    Record rec;
    rec.set(py::str("file"), RecValue(py::os::path::basename(path)));
    if (py::os::path::getsize(path) == 0) {
        rec.set(py::str("kind"), RecValue(py::str("empty")));      // the package has it, with nothing in it
        return rec;
    }
    try {
        Xnb x = read(path);
        py::list<py::str> kinds;
        for (const py::str& r : x.readers) kinds.append(short_(r));
        rec.set(py::str("platform"), RecValue(x.platform));
        rec.set(py::str("readers"), RecValue(kinds));
        py::str primary = kinds.empty() ? py::str("?") : kinds[0];
        py::os::makedirs(out_dir, 0777, true);
        if (primary == py::str("Texture2D")) {
            auto te = _texture(x.data, x.offset);
            const Texture& t = std::get<0>(te);
            save_png(texture_image(t, x.platform), py::os::path::join(out_dir, name + py::str(".png")));
            rec.set(py::str("kind"), RecValue(py::str("texture")));
            rec.set(py::str("format"), SURFACE_FORMATS.contains(t.format) ? RecValue(SURFACE_FORMATS.at(t.format)) : RecValue(t.format));
            rec.set(py::str("width"), RecValue(t.width));
            rec.set(py::str("height"), RecValue(t.height));
            rec.set(py::str("levels"), RecValue(t.levels));
        } else if (primary == py::str("SpriteFont")) {
            SpriteFont font = _sprite_font(x.data, x.offset);
            Texture t = font.texture;                              // (font.pop("texture"))
            save_png(texture_image(t, x.platform), py::os::path::join(out_dir, name + py::str(".png")));
            Json j = Json::dict();
            Json glyphs = Json::list(), cropping = Json::list();
            for (const auto& g : font.glyphs) {
                Json r = Json::list();
                for (int64_t v : g) r.push(Json::integer(v));
                glyphs.push(r);
            }
            for (const auto& g : font.cropping) {
                Json r = Json::list();
                for (int64_t v : g) r.push(Json::integer(v));
                cropping.push(r);
            }
            Json chars = Json::list();
            for (const py::str& c : font.characters) chars.push(Json::str(c));
            Json kerning = Json::list();
            for (const auto& k : font.kerning) {
                Json r = Json::list();
                for (double v : k) r.push(Json::real(v));
                kerning.push(r);
            }
            j.set("glyphs", glyphs);
            j.set("cropping", cropping);
            j.set("characters", chars);
            j.set("line_spacing", Json::integer(font.line_spacing));
            j.set("spacing", Json::real(font.spacing));
            j.set("kerning", kerning);
            std::string text;
            dumps(text, j, false, 0);
            write_text(py::os::path::join(out_dir, name + py::str(".json")), text, true);
            rec.set(py::str("kind"), RecValue(py::str("font")));
            rec.set(py::str("width"), RecValue(t.width));
            rec.set(py::str("height"), RecValue(t.height));
            rec.set(py::str("glyphs"), RecValue(font.glyphs.size()));
        } else if (primary == py::str("SoundEffect")) {
            Sound sound = _sound_effect(x.data, x.offset, x.platform);
            write_wav(sound, py::os::path::join(out_dir, name + py::str(".wav")));
            rec.set(py::str("kind"), RecValue(py::str("sound")));
            if (sound.tag == 1) rec.set(py::str("codec"), RecValue(py::str("PCM")));
            else if (sound.tag == 2) rec.set(py::str("codec"), RecValue(py::str("ADPCM")));
            else if (sound.tag == 0x166) rec.set(py::str("codec"), RecValue(py::str("XMA2")));
            else if (sound.tag == 0x165) rec.set(py::str("codec"), RecValue(py::str("XMA")));
            else rec.set(py::str("codec"), RecValue(sound.tag));
            rec.set(py::str("channels"), RecValue(sound.channels));
            rec.set(py::str("rate"), RecValue(sound.rate));
            rec.set(py::str("duration_ms"), RecValue(sound.duration_ms));
            rec.set(py::str("loop"), RecValue(sound.loop));
        } else if (primary == py::str("Effect") && x.platform == py::str("x")) {
            rec.set(py::str("kind"), RecValue(py::str("effect")));
            rec.set(py::str("sha1"), RecValue(write_effect(effect_code(x.data, x.offset), name, out_dir)));
        } else {
            rec.set(py::str("kind"), RecValue(py::str("other")));
        }
    } catch (const py::Exception& e) {                             // one bad file is reported, the rest go on
        rec.set(py::str("error"), RecValue(error_text(e)));
    }
    return rec;
}

// ---- PNG -----------------------------------------------------------------------------------------------------------
namespace {
void put_be32(std::string& o, int64_t v) {
    for (int k = 3; k >= 0; --k) o.push_back(char((v >> (8 * k)) & 0xFF));
}
void chunk(std::string& o, const char* tag, const std::string& body) {
    put_be32(o, int64_t(body.size()));
    std::string t = std::string(tag, 4) + body;
    o += t;
    put_be32(o, py::crc32(py::bytes(t)));
}
}  // namespace

void save_png(const tex::Image& image, const py::str& path) {
    std::string png("\x89PNG\r\n\x1a\n", 8);
    std::string ihdr;
    put_be32(ihdr, image.width);
    put_be32(ihdr, image.height);
    ihdr += std::string("\x08\x06\x00\x00\x00", 5);                 // 8 bits, RGBA, deflate, no filter method, no interlace
    chunk(png, "IHDR", ihdr);
    std::string raw;
    const std::string& px = image.pixels.raw();
    for (int64_t y = 0; y < image.height; ++y) {
        raw.push_back('\0');                                        // filter type 0 (None) for each row
        raw.append(px, size_t(y * image.width * 4), size_t(image.width * 4));
    }
    chunk(png, "IDAT", py::zlib::compress(py::bytes(raw)).raw());
    chunk(png, "IEND", "");
    py::file f = py::open(path, py::str("wb"));
    f.write(py::bytes(png));
    f.close();
}

}  // namespace codebreaker::formats::xnb
