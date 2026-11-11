#include "cb/xna/mgfx.hpp"

#include <algorithm>
#include <cmath>
#include <format>
#include <limits>
#include <unordered_set>

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"
#include "cb/core/json.hpp"

namespace cb::xna::mgfx {
namespace {

constexpr std::string_view kReader = "mgfx";
constexpr std::string_view kSignature = "MGFX";
// signature, version byte, profile byte, the effect's 32-bit key
constexpr std::size_t kHeaderSize = 4 + 1 + 1 + 4;
// A parameter list nests (arrays of structs of arrays ...); a real effect is two or three deep.
constexpr int kMaxDepth = 32;

// Wire values of the parameter types that carry a default (MojoShader keeps every number a float).
bool has_default(std::uint8_t wire_type) { return wire_type == 1 || wire_type == 2 || wire_type == 3; }

// ---- reading ----------------------------------------------------------------------------------------------

// A length-prefixed string: its UTF-8 length as a 7-bit varint (at most 32 bits), then the bytes. The bytes are
// kept as they are; names are only compared and copied.
std::string read_string(io::ByteReader& r) {
    std::uint64_t n = 0;
    for (unsigned shift = 0;; shift += 7) {
        if (shift >= 35) throw FormatError(kReader, r.pos(), "string length is longer than 32 bits");
        const std::uint8_t b = r.u8();
        n |= static_cast<std::uint64_t>(b & 0x7F) << shift;
        if (!(b & 0x80)) break;
    }
    return std::string(as_text(r.bytes(n)));
}

// A count that may not be negative where the data goes on to skip that many things.
std::uint32_t read_size(io::ByteReader& r, std::string_view what) {
    const std::size_t at = r.pos();
    const std::int32_t n = r.i32();
    if (n < 0) throw FormatError(kReader, at, std::format("negative {}", what));
    return static_cast<std::uint32_t>(n);
}

// MonoGame's compiler writes no annotations, so a count of them is a variant this reader does not handle.
void read_no_annotations(io::ByteReader& r) {
    const std::size_t at = r.pos();
    if (r.i32() != 0) throw UnsupportedError(kReader, at, "annotations are not read");
}

// The count that starts a list, without consuming it (TruncatedError when the data ends first).
std::int32_t peek_count(io::ByteReader& r) {
    const std::optional<std::uint32_t> v = r.peek_u32();
    if (!v) return r.i32();   // too short: this read raises the error
    return static_cast<std::int32_t>(*v);
}

// Steps over a list of parameters (a count, then each one), their elements and members; the names of the top-level
// ones are appended to `names` when given.
void skip_parameters(io::ByteReader& r, std::vector<std::string>* names, int depth) {
    if (depth > kMaxDepth) throw FormatError(kReader, r.pos(), "parameters are nested too deeply");
    const std::int32_t count = r.i32();   // a negative count is an empty list
    for (std::int32_t i = 0; i < count; ++i) {
        r.u8();   // class
        const std::uint8_t type = r.u8();
        std::string name = read_string(r);
        if (names) names->push_back(std::move(name));
        read_string(r);   // semantic
        read_no_annotations(r);
        const std::uint32_t rows = r.u8();
        const std::uint32_t cols = r.u8();
        // the element and member lists each start with their own count: look at it, then step over the list
        const std::int32_t n_elements = peek_count(r);
        skip_parameters(r, nullptr, depth + 1);
        const std::int32_t n_members = peek_count(r);
        skip_parameters(r, nullptr, depth + 1);
        if (n_elements == 0 && n_members == 0 && has_default(type)) r.skip(4ull * rows * cols);
    }
}

// The reader positioned at the effect's parameter list.
io::ByteReader reader_at_parameters(ByteView effect) {
    if (effect.size() < kSignature.size() || as_text(effect.first(kSignature.size())) != kSignature)
        throw FormatError(kReader, 0, "not a MonoGame effect");
    io::ByteReader r(effect, kReader);
    r.skip(kHeaderSize);
    const std::uint32_t buffers = read_size(r, "constant buffer count");
    for (std::uint32_t i = 0; i < buffers; ++i) {   // constant buffers: name, size, parameter count, 6 bytes each
        read_string(r);
        r.i16();
        r.skip(6ull * read_size(r, "constant buffer parameter count"));
    }
    const std::uint32_t shaders = read_size(r, "shader count");
    for (std::uint32_t i = 0; i < shaders; ++i) {   // shaders: stage flag, bytecode, samplers, buffers, attributes
        r.u8();
        r.skip(read_size(r, "shader size"));
        for (unsigned samplers = r.u8(); samplers > 0; --samplers) {
            r.skip(3);
            if (r.u8() != 0) r.skip(3 + 4 + 1 + 4 + 4 + 4);   // a sampler with a state block
            read_string(r);
            r.u8();
        }
        r.skip(r.u8());   // constant buffer indices
        for (unsigned attributes = r.u8(); attributes > 0; --attributes) {
            read_string(r);
            r.skip(4);
        }
    }
    return r;
}

// ---- writing ----------------------------------------------------------------------------------------------

void write_string(io::ByteWriter& w, std::string_view s) {
    std::uint64_t n = s.size();
    do {
        const std::uint8_t b = n & 0x7F;
        n >>= 7;
        w.u8(static_cast<std::uint8_t>(b | (n ? 0x80 : 0)));
    } while (n);
    w.bytes(as_bytes(s));
}

std::uint8_t byte_of(std::uint32_t v, const Parameter& p, std::string_view what) {
    if (v > 255) throw UsageError(std::format("parameter '{}': {} {} does not fit a byte", p.name, what, v));
    return static_cast<std::uint8_t>(v);
}

float to_float(double v, const Parameter& p) {
    const float f = static_cast<float>(v);
    if (std::isinf(f) && std::isfinite(v)) throw UsageError(std::format("parameter '{}': {} is too large for a float", p.name, v));
    return f;
}

std::uint8_t wire_class(ParamClass c) {
    switch (c) {
        case ParamClass::Scalar: return 0;
        case ParamClass::Vector: return 1;
        case ParamClass::MatrixRows:
        case ParamClass::MatrixColumns: return 2;
        case ParamClass::Object: return 3;
        case ParamClass::Struct: return 4;
    }
    return 3;
}

std::uint8_t wire_type(ParamType t) {
    switch (t) {
        case ParamType::Void: return 0;
        case ParamType::Bool: return 1;
        case ParamType::Int: return 2;
        case ParamType::Float: return 3;
        case ParamType::String: return 4;
        case ParamType::Texture:
        case ParamType::Sampler: return 5;   // what MonoGame calls the texture a sampler reads
        case ParamType::Texture1D: return 6;
        case ParamType::Texture2D: return 7;
        case ParamType::Texture3D: return 8;
        case ParamType::TextureCube: return 9;
        case ParamType::VertexShader:
        case ParamType::PixelShader: return 0;
    }
    return 0;
}

// One parameter in MGFX's form (its value: the default, as floats). An array's elements are nameless (MonoGame
// indexes a collection by its names); `element` says this one is such an element.
void write_parameter(io::ByteWriter& w, const Parameter& p, bool element) {
    const std::uint8_t type = wire_type(p.type);
    w.u8(wire_class(p.cls));
    w.u8(type);
    write_string(w, element ? std::string_view() : std::string_view(p.name));
    write_string(w, element ? std::string_view() : std::string_view(p.semantic.value_or("")));
    w.i32(0);   // annotations
    const std::uint32_t rows = p.cls == ParamClass::Vector ? 1 : p.rows;
    const std::uint32_t cols = p.columns;
    w.u8(byte_of(rows, p, "rows"));
    w.u8(byte_of(cols, p, "columns"));
    if (p.elements != 0) {
        w.i32(static_cast<std::int32_t>(p.members.size()));
        for (const Parameter& m : p.members) write_parameter(w, m, true);
        w.i32(0);
        return;
    }
    if (p.cls == ParamClass::Struct) {
        w.i32(0);
        w.i32(static_cast<std::int32_t>(p.members.size()));
        for (const Parameter& m : p.members) write_parameter(w, m, false);
        return;
    }
    w.i32(0);
    w.i32(0);
    if (!has_default(type)) return;
    const std::size_t n = static_cast<std::size_t>(rows) * cols;
    std::vector<double> values(p.value.begin(), p.value.begin() + static_cast<std::ptrdiff_t>(std::min(n, p.value.size())));
    values.resize(n, 0.0);
    if (p.cls == ParamClass::MatrixColumns) {   // the Xbox's effect keeps a column at a time; MonoGame a row
        std::vector<double> by_rows;
        by_rows.reserve(n);
        for (std::uint32_t r = 0; r < rows; ++r)
            for (std::uint32_t c = 0; c < cols; ++c) by_rows.push_back(values[static_cast<std::size_t>(c) * rows + r]);
        values = std::move(by_rows);
    }
    for (double v : values) {
        const float f = to_float(v, p);
        w.u32(std::bit_cast<std::uint32_t>(f));
    }
}

bool is_addable(const Parameter& p) {
    return !p.name.empty() && p.type != ParamType::Sampler && p.type != ParamType::VertexShader &&
           p.type != ParamType::PixelShader && p.type != ParamType::String;
}

// ---- JSON -------------------------------------------------------------------------------------------------

std::uint32_t json_count(const Json& j, const char* key) {
    const auto it = j.find(key);
    if (it == j.end() || it->is_null()) return 0;
    if (!it->is_number()) throw FormatError("fx.json", 0, std::format("'{}' is not a number", key));
    // a count written as 2.0 is 2; a negative or absurd one is carried as "too big for a byte" and refused when written
    const double d = it->get<double>();
    if (!(d >= 0)) return std::numeric_limits<std::uint32_t>::max();
    return d > 4294967295.0 ? std::numeric_limits<std::uint32_t>::max() : static_cast<std::uint32_t>(d);
}

Parameter parameter_from_json(const Json& j, int depth) {
    if (depth > kMaxDepth) throw FormatError("fx.json", 0, "parameters are nested too deeply");
    if (!j.is_object()) throw FormatError("fx.json", 0, "a parameter is not an object");
    Parameter p;
    auto text = [&](const char* key) -> std::optional<std::string> {
        const auto it = j.find(key);
        if (it == j.end() || it->is_null()) return std::nullopt;
        if (!it->is_string()) throw FormatError("fx.json", 0, std::format("'{}' is not text", key));
        return it->get<std::string>();
    };
    p.name = text("name").value_or("");
    p.semantic = text("semantic");
    p.type = parse_type(text("type").value_or(""));
    p.cls = parse_class(text("cls").value_or(""));
    p.rows = json_count(j, "rows");
    p.columns = json_count(j, "columns");
    p.elements = json_count(j, "elements");
    if (const auto it = j.find("value"); it != j.end() && it->is_array()) {
        for (const Json& v : *it) {
            if (!v.is_number()) throw FormatError("fx.json", 0, "a default value is not a number");
            p.value.push_back(v.get<double>());
        }
    }
    if (const auto it = j.find("members"); it != j.end() && it->is_array())
        for (const Json& m : *it) p.members.push_back(parameter_from_json(m, depth + 1));
    return p;
}

}  // namespace

ParamClass parse_class(std::string_view text) {
    if (text == "scalar") return ParamClass::Scalar;
    if (text == "vector") return ParamClass::Vector;
    if (text == "matrix_rows") return ParamClass::MatrixRows;
    if (text == "matrix_columns") return ParamClass::MatrixColumns;
    if (text == "struct") return ParamClass::Struct;
    return ParamClass::Object;   // "object", and what is not named
}

ParamType parse_type(std::string_view text) {
    struct Named {
        std::string_view name;
        ParamType type;
    };
    static constexpr Named kNames[] = {
        {"void", ParamType::Void},           {"bool", ParamType::Bool},           {"int", ParamType::Int},
        {"float", ParamType::Float},         {"string", ParamType::String},       {"texture", ParamType::Texture},
        {"texture1D", ParamType::Texture1D}, {"texture2D", ParamType::Texture2D}, {"texture3D", ParamType::Texture3D},
        {"textureCUBE", ParamType::TextureCube}, {"vertexshader", ParamType::VertexShader},
        {"pixelshader", ParamType::PixelShader},
    };
    if (text.starts_with("sampler")) return ParamType::Sampler;
    for (const Named& n : kNames)
        if (n.name == text) return n.type;
    return ParamType::Void;
}

std::vector<Parameter> parse_parameters(std::string_view json_text) {
    const Json doc = json::parse(json_text);
    if (!doc.is_array()) throw FormatError("fx.json", 0, "expected an array of parameters");
    std::vector<Parameter> out;
    out.reserve(doc.size());
    for (const Json& j : doc) out.push_back(parameter_from_json(j, 0));
    return out;
}

std::vector<std::string> parameter_names(ByteView effect) {
    io::ByteReader r = reader_at_parameters(effect);
    std::vector<std::string> names;
    skip_parameters(r, &names, 0);
    return names;
}

Bytes add_parameters(ByteView effect, std::span<const Parameter> parameters) {
    io::ByteReader r = reader_at_parameters(effect);
    const std::size_t count_at = r.pos();
    std::vector<std::string> names;
    skip_parameters(r, &names, 0);
    const std::size_t end = r.pos();

    const std::unordered_set<std::string> have(names.begin(), names.end());   // lookup only
    std::vector<const Parameter*> extra;
    for (const Parameter& p : parameters)
        if (is_addable(p) && !have.contains(p.name)) extra.push_back(&p);
    if (extra.empty()) return Bytes(effect.begin(), effect.end());

    io::ByteWriter added;
    for (const Parameter* p : extra) write_parameter(added, *p, false);

    io::ByteReader count_reader(effect, kReader);
    count_reader.seek(count_at);
    const std::int32_t count = count_reader.i32();
    io::ByteWriter out;
    out.bytes(effect.first(count_at));
    out.i32(count + static_cast<std::int32_t>(extra.size()));
    out.bytes(effect.subspan(count_at + 4, end - (count_at + 4)));
    out.bytes(added.data());
    out.bytes(effect.subspan(end));
    return out.take();
}

}  // namespace cb::xna::mgfx
