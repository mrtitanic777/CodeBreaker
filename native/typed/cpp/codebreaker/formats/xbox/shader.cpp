// codebreaker/formats/xbox/shader.cpp -- typed C++ from codebreaker.formats.xbox.shader.
// (_u32 and _cstring are the front end's proven functions, kept as they were.)

#include "codebreaker/formats/xbox/shader.hpp"

namespace codebreaker::formats::xbox::shader {

const py::dict<int64_t, py::str> USAGES = {
    {0, py::str("POSITION")}, {1, py::str("BLENDWEIGHT")}, {2, py::str("BLENDINDICES")}, {3, py::str("NORMAL")},
    {4, py::str("PSIZE")}, {5, py::str("TEXCOORD")}, {6, py::str("TANGENT")}, {7, py::str("BINORMAL")},
    {8, py::str("TESSFACTOR")}, {9, py::str("POSITIONT")}, {10, py::str("COLOR")}, {11, py::str("FOG")},
    {12, py::str("DEPTH")}, {13, py::str("SAMPLE")}};
const py::dict<int64_t, py::str> REGISTER_SETS = {
    {0, py::str("bool")}, {1, py::str("int4")}, {2, py::str("float4")}, {3, py::str("sampler")}};
const py::dict<int64_t, py::str> CLASSES = {
    {0, py::str("scalar")}, {1, py::str("vector")}, {2, py::str("matrix_rows")}, {3, py::str("matrix_columns")},
    {4, py::str("object")}, {5, py::str("struct")}};
const py::dict<int64_t, py::str> TYPES = {
    {0, py::str("void")}, {1, py::str("bool")}, {2, py::str("int")}, {3, py::str("float")}, {4, py::str("string")},
    {5, py::str("texture")}, {6, py::str("texture1D")}, {7, py::str("texture2D")}, {8, py::str("texture3D")},
    {9, py::str("textureCUBE")}, {10, py::str("sampler")}, {11, py::str("sampler1D")}, {12, py::str("sampler2D")},
    {13, py::str("sampler3D")}, {14, py::str("samplerCUBE")}};

py::str Constant::__repr__() const {
    return py::percent(py::str("<%s %s %s%d x%d>"), set, name,
                       set == py::str("float4") ? py::str("c") : set.slice(0, 1), register_, count);
}

std::tuple<std::optional<Constant>, int64_t> Shader::constant_at(const py::str& register_set, int64_t register_) const {
    for (const Constant& c : constants) {
        if (c.set == register_set && c.register_ <= register_ && register_ < c.register_ + c.count)
            return {c, register_ - c.register_};
    }
    return {std::nullopt, 0};
}

int64_t _u32(const py::bytes& b, int64_t at) {
    return py::unpack_from1<int64_t>(">I", b, at);
}

py::str _cstring(const py::bytes& b, int64_t at) {
    int64_t end = b.index(py::bytes("\000", 1), at);
    return b.slice(at, end).decode("latin1", "strict");
}

// struct.unpack_from(">%df" % n, b, at) / (">%dI" % n, b, at): n values of one kind, all or a struct.error
static void check_room(const py::bytes& b, int64_t at, int64_t n) {
    if (n < 0) throw py::StructError("bad char in struct format");
    if (at > py::len(b) || n * 4 > py::len(b) - at) throw py::StructError("unpack_from requires a larger buffer");
}

static py::list<double> unpack_floats(const py::bytes& b, int64_t at, int64_t n) {
    check_room(b, at, n);
    py::list<double> out;
    for (int64_t i = 0; i < n; ++i) out.append(py::unpack_from1<double>(">f", b, at + 4 * i));
    return out;
}

static py::list<int64_t> unpack_words(const py::bytes& b, int64_t at, int64_t n) {
    check_room(b, at, n);
    py::list<int64_t> out;
    for (int64_t i = 0; i < n; ++i) out.append(py::unpack_from1<int64_t>(">I", b, at + 4 * i));
    return out;
}

std::tuple<py::list<Constant>, py::str> _ctab(const py::bytes& b, int64_t base) {
    int64_t n = _u32(b, base + 12), info = _u32(b, base + 16), target = _u32(b, base + 24);
    py::list<Constant> out;
    for (int64_t i = 0; i < n; ++i) {
        int64_t at = base + info + 20 * i;
        int64_t name_at = py::unpack_from1<int64_t>(">I", b, at);
        auto [rset, reg, count, unused] = py::unpack_from<int64_t, int64_t, int64_t, int64_t>(">4H", b, at + 4);
        (void)unused;
        auto [type_at, default_at] = py::unpack_from<int64_t, int64_t>(">II", b, at + 12);
        auto [cls, typ, rows, cols, elements, members] =
            py::unpack_from<int64_t, int64_t, int64_t, int64_t, int64_t, int64_t>(">6H", b, base + type_at);
        std::optional<py::list<double>> default_;
        if (default_at) default_ = unpack_floats(b, base + default_at, 4 * count);
        Constant c;
        c.name = _cstring(b, base + name_at);
        c.set = REGISTER_SETS.get(rset, py::to_str(rset));
        c.register_ = reg;
        c.count = count;
        c.cls = CLASSES.get(cls, py::to_str(cls));
        c.type = TYPES.get(typ, py::to_str(typ));
        c.rows = rows;
        c.columns = cols;
        c.elements = elements;
        c.members = members;
        c.default_ = default_;
        out.append(c);
    }
    return {out, _cstring(b, base + target)};
}

std::tuple<py::str, int64_t> _semantic(int64_t entry) {
    return {USAGES.get((entry >> 4) & 0xF, py::percent(py::str("USAGE%d"), (entry >> 4) & 0xF)), entry & 0xF};
}

Shader read(const py::bytes& blob) {
    int64_t magic = _u32(blob, 0);
    if (magic != PIXEL && magic != VERTEX)
        throw py::ValueError(py::percent(py::str("not an Xbox 360 shader (magic %08X)"), magic));
    Shader s;
    s.stage = magic == PIXEL ? py::str("pixel") : py::str("vertex");
    int64_t physical = _u32(blob, 4), _size = _u32(blob, 8), ctab_at = _u32(blob, 16), literals_at = _u32(blob, 20),
            info_at = _u32(blob, 24);
    (void)_size;
    std::tie(s.constants, s.target) = _ctab(blob, ctab_at + 4);
    int64_t literal_bytes = _u32(blob, info_at), microcode_bytes = _u32(blob, info_at + 4);
    // the literal records: four words, the records' size, then the records -- a float one is register << 16 |
    // count, and two words (its floats are at the physical part's start, in order); a loop constant one is
    // 0x2320 + 4 * index << 16 | count, its values (count | start << 8 | step << 16) and a word. The SDK's fxc
    // listing (def / defi) shows the same constants.
    if (literals_at) {
        int64_t size = _u32(blob, literals_at + 16);
        int64_t at = literals_at + 20, end = literals_at + 16 + size, pos = physical;
        int64_t base = s.stage == py::str("pixel") ? 256 : 0;   // pixel shader registers follow the vertex shader's
        while (at + 4 <= end) {
            int64_t word = _u32(blob, at);
            int64_t reg = word >> 16, count = word & 0xFFFF;
            if (word == 0) break;
            if (reg >= 0x2320) {
                for (int64_t i = 0; i < count; ++i)
                    s.loop_literals.set(py::floordiv(reg - 0x2320, 4) + i, _u32(blob, at + 4 + 4 * i));
                at += 4 * (count + 2);
            } else {
                for (int64_t i = 0; i < py::floordiv(count, 4); ++i) {
                    py::list<double> f = unpack_floats(blob, pos + 16 * i, 4);
                    s.literals.set(reg - base + i, {f[0], f[1], f[2], f[3]});
                }
                pos += 4 * count;
                at += 12;
            }
        }
    }
    int64_t start = physical + literal_bytes;
    s.words = unpack_words(blob, start, py::floordiv(microcode_bytes, 4));
    py::list<int64_t> info = unpack_words(blob, info_at, py::floordiv(physical - info_at, 4));
    if (s.stage == py::str("vertex")) {
        int64_t n_in = info[7], n_writers = info[8];
        py::list<int64_t> entries = info.slice_from(10);
        py::list<VertexInput> inputs;
        for (int64_t e : entries.slice_to(n_in)) {
            py::str usage = USAGES.get((e >> 12) & 0xF, py::percent(py::str("USAGE%d"), (e >> 12) & 0xF));
            inputs.append({e & 0xFFF, usage, (e >> 16) & 0xF});
        }
        s.inputs = inputs;
        py::list<int64_t> outputs = entries.slice(n_in, py::len(entries) - n_writers);
        py::list<int64_t> writers = entries.slice_from(py::len(entries) - n_writers);
        for (int64_t e : outputs) {
            auto [usage, index] = _semantic(e);
            s.outputs.append({(e >> 8) & 0xF, usage, index, (e >> 12) & 0xF});
        }
        s.output_writers = py::list<int64_t>{};
        for (int64_t w : writers) s.output_writers.append(w & 0xFFF);
    } else {
        s.interpolator_control = info[6];
        s.output_mask = info[7];
        py::list<Interpolator> inputs;
        for (int64_t e : info.slice_from(8)) {
            if (e == 0) break;
            auto [usage, index] = _semantic(e);
            inputs.append({(e >> 8) & 0xF, usage, index, (e >> 12) & 0xF});
        }
        s.inputs = inputs;
    }
    return s;
}

}  // namespace codebreaker::formats::xbox::shader
