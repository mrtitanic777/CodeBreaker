#include "cb/formats/xbox/shader.hpp"

#include <bit>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"

namespace cb::formats::xbox::shader {

namespace {

constexpr std::string_view kShaderReader = "xbox shader";

const char* const kUsages[] = {"POSITION", "BLENDWEIGHT", "BLENDINDICES", "NORMAL", "PSIZE", "TEXCOORD", "TANGENT",
                               "BINORMAL", "TESSFACTOR", "POSITIONT", "COLOR", "FOG", "DEPTH", "SAMPLE"};

std::string usage_name(std::uint32_t number) {
    return number < std::size(kUsages) ? kUsages[number] : std::format("USAGE{}", number);
}

// Random access over a big-endian container; every read is bounds-checked (TruncatedError).
class Container {
public:
    explicit Container(ByteView data) : data_(data) {}
    ByteView data() const { return data_; }
    std::uint32_t u32(std::uint64_t at) const {
        io::ByteReader r(data_, kShaderReader);
        r.seek(at);
        return r.u32_be();
    }
    std::uint16_t u16(std::uint64_t at) const {
        io::ByteReader r(data_, kShaderReader);
        r.seek(at);
        return r.u16_be();
    }
    float f32(std::uint64_t at) const { return std::bit_cast<float>(u32(at)); }
    std::string cstring(std::uint64_t at) const {
        io::ByteReader r(data_, kShaderReader);
        r.seek(at);
        return text::from_latin1(r.cstr());
    }
    // `count` big-endian dwords from `at`; the range must lie inside the container.
    std::vector<std::uint32_t> words(std::uint64_t at, std::uint64_t count) const {
        const ByteView v = io::checked_range(data_, at, count * 4);
        io::ByteReader r(v, kShaderReader);
        std::vector<std::uint32_t> out;
        out.reserve(static_cast<std::size_t>(count));
        for (std::uint64_t i = 0; i < count; ++i) out.push_back(r.u32_be());
        return out;
    }

private:
    ByteView data_;
};

RegisterSet register_set_of(std::uint16_t number) {
    return number <= 3 ? static_cast<RegisterSet>(number) : RegisterSet::Other;
}

// The Direct3D constant table at `base` (big-endian here): its constants and the compile target.
void read_ctab(const Container& c, std::uint64_t base, Shader& s) {
    const std::uint32_t n = c.u32(base + 12), info = c.u32(base + 16), target = c.u32(base + 24);
    for (std::uint32_t i = 0; i < n; ++i) {
        const std::uint64_t at = base + info + 20ull * i;
        Constant k;
        k.name = c.cstring(base + c.u32(at));
        k.set = register_set_of(c.u16(at + 4));
        k.first_register = c.u16(at + 6);
        k.count = c.u16(at + 8);
        const std::uint32_t type_at = c.u32(at + 12), default_at = c.u32(at + 16);
        k.cls = d3dx::class_of(c.u16(base + type_at));
        k.type = d3dx::type_of(c.u16(base + type_at + 2));
        k.rows = c.u16(base + type_at + 4);
        k.columns = c.u16(base + type_at + 6);
        k.elements = c.u16(base + type_at + 8);
        k.members = c.u16(base + type_at + 10);
        if (default_at != 0) {
            const std::uint64_t floats = 4ull * k.count;
            const ByteView v = io::checked_range(c.data(), base + default_at, floats * 4);
            (void)v;
            for (std::uint64_t f = 0; f < floats; ++f) k.defaults.push_back(c.f32(base + default_at + 4 * f));
        }
        s.constants.push_back(std::move(k));
    }
    s.target = c.cstring(base + target);
}

// The literal records: four words, the records' size, then the records. A float one is register << 16 | count, and two
// words (its floats are at the physical part's start, in order); a loop constant one is 0x2320 + 4 * index << 16 | count,
// its values (count | start << 8 | step << 16) and a word. The SDK's fxc listing (def / defi) shows the same constants.
void read_literals(const Container& c, std::uint64_t literals_at, std::uint64_t physical, Shader& s) {
    const std::uint32_t size = c.u32(literals_at + 16);
    std::uint64_t at = literals_at + 20;
    const std::uint64_t end = literals_at + 16 + size;
    std::uint64_t pos = physical;
    const std::uint32_t base = s.stage == Stage::Pixel ? 256 : 0;   // pixel shader registers follow the vertex shader's
    while (at + 4 <= end) {
        const std::uint32_t word = c.u32(at);
        const std::uint32_t reg = word >> 16, count = word & 0xFFFF;
        if (word == 0) break;
        if (reg >= 0x2320) {
            for (std::uint32_t i = 0; i < count; ++i) s.loop_literals[(reg - 0x2320) / 4 + i] = c.u32(at + 4 + 4ull * i);
            at += 4ull * (count + 2);
        } else {
            for (std::uint32_t i = 0; i < count / 4; ++i) {
                std::array<float, 4> v{};
                for (std::size_t k = 0; k < 4; ++k) v[k] = c.f32(pos + 16ull * i + 4 * k);
                s.literals[reg - base + i] = v;
            }
            pos += 4ull * count;
            at += 12;
        }
    }
}

}  // namespace

bool is_container(ByteView data) {
    if (data.size() < 4) return false;
    const std::uint32_t magic = (std::uint32_t{data[0]} << 24) | (std::uint32_t{data[1]} << 16) | (std::uint32_t{data[2]} << 8) | data[3];
    return magic == kPixelMagic || magic == kVertexMagic;
}

Shader read(ByteView blob) {
    const Container c(blob);
    const std::uint32_t magic = c.u32(0);
    if (magic != kPixelMagic && magic != kVertexMagic) {
        throw FormatError(kShaderReader, 0, std::format("not an Xbox 360 shader (magic {:08X})", magic));
    }
    Shader s;
    s.stage = magic == kPixelMagic ? Stage::Pixel : Stage::Vertex;
    const std::uint32_t physical = c.u32(4), ctab_at = c.u32(16), literals_at = c.u32(20), info_at = c.u32(24);
    read_ctab(c, std::uint64_t{ctab_at} + 4, s);
    const std::uint32_t literal_bytes = c.u32(info_at), microcode_bytes = c.u32(std::uint64_t{info_at} + 4);
    if (literals_at != 0) read_literals(c, literals_at, physical, s);
    s.words = c.words(std::uint64_t{physical} + literal_bytes, microcode_bytes / 4);
    if (physical < info_at) throw FormatError(kShaderReader, info_at, "the program information lies past the physical part");
    const std::vector<std::uint32_t> info = c.words(info_at, (physical - info_at) / 4);

    const auto semantic = [](std::uint32_t entry) { return std::pair{usage_name((entry >> 4) & 0xF), entry & 0xF}; };
    if (s.stage == Stage::Vertex) {
        if (info.size() < 10) throw FormatError(kShaderReader, info_at, "the vertex shader's program information is cut short");
        const std::size_t n_in = info[7], n_writers = info[8];
        const std::size_t entries = info.size() - 10;
        if (n_in > entries || n_writers > entries - n_in) {
            throw FormatError(kShaderReader, info_at, "the vertex shader lists more inputs and writers than its program information holds");
        }
        for (std::size_t i = 0; i < n_in; ++i) {
            const std::uint32_t e = info[10 + i];
            s.vertex_inputs.push_back({e & 0xFFF, usage_name((e >> 12) & 0xF), (e >> 16) & 0xF});
        }
        for (std::size_t i = n_in; i < entries - n_writers; ++i) {
            const std::uint32_t e = info[10 + i];
            const auto [usage, index] = semantic(e);
            s.outputs.push_back({(e >> 8) & 0xF, usage, index, (e >> 12) & 0xF});
        }
        for (std::size_t i = entries - n_writers; i < entries; ++i) s.output_writers.push_back(info[10 + i] & 0xFFF);
    } else {
        if (info.size() < 8) throw FormatError(kShaderReader, info_at, "the pixel shader's program information is cut short");
        s.interpolator_control = info[6];
        s.output_mask = info[7];
        for (std::size_t i = 8; i < info.size() && info[i] != 0; ++i) {
            const std::uint32_t e = info[i];
            const auto [usage, index] = semantic(e);
            s.pixel_inputs.push_back({(e >> 8) & 0xF, usage, index, (e >> 12) & 0xF});
        }
    }
    return s;
}

Shader::Found Shader::constant_at(RegisterSet set, std::uint32_t reg) const {
    for (const Constant& c : constants) {
        if (c.set == set && c.first_register <= reg && reg < std::uint32_t{c.first_register} + c.count) {
            return {&c, reg - c.first_register};
        }
    }
    return {};
}

std::string_view register_set_name(RegisterSet set) {
    switch (set) {
        case RegisterSet::Bool: return "bool";
        case RegisterSet::Int4: return "int4";
        case RegisterSet::Float4: return "float4";
        case RegisterSet::Sampler: return "sampler";
        case RegisterSet::Other: return "other";
    }
    return "other";
}

}  // namespace cb::formats::xbox::shader
