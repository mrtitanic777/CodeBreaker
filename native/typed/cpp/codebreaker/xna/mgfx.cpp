// codebreaker/xna/mgfx.cpp -- C++ from codebreaker.xna.mgfx.

#include "codebreaker/xna/mgfx.hpp"

#include <cmath>
#include <string>
#include <vector>

namespace codebreaker::xna::mgfx {

const py::bytes SIGNATURE = py::bytes("MGFX");
const py::dict<py::str, int64_t> CLASSES{{py::str("scalar"), 0}, {py::str("vector"), 1}, {py::str("matrix_rows"), 2},
                                         {py::str("matrix_columns"), 2}, {py::str("object"), 3}, {py::str("struct"), 4}};
const py::dict<py::str, int64_t> TYPES{{py::str("void"), 0},      {py::str("bool"), 1},      {py::str("int"), 2},       {py::str("float"), 3},
                                       {py::str("string"), 4},    {py::str("texture"), 5},   {py::str("texture1D"), 6}, {py::str("texture2D"), 7},
                                       {py::str("texture3D"), 8}, {py::str("textureCUBE"), 9}};

int64_t Reader::take_i() {
    int64_t v = py::unpack_from1<int64_t>("<i", d, at);
    at += 4;
    return v;
}

int64_t Reader::take_h() {
    int64_t v = py::unpack_from1<int64_t>("<h", d, at);
    at += 2;
    return v;
}

int64_t Reader::take_B() {
    int64_t v = py::unpack_from1<int64_t>("<B", d, at);
    at += 1;
    return v;
}

bool Reader::take_flag() {
    bool v = py::unpack_from1<bool>("<?", d, at);
    at += 1;
    return v;
}

py::str Reader::string() {
    // The python's n is unbounded. Once it passes 2**62 it is held at 2**62 (and above): every use of it is
    // as an offset into the data (a slice end clamped to the data, a position that then fails every read),
    // so a number that large behaves as any other that large.
    const int64_t HUGE_N = int64_t(1) << 62;
    int64_t n = 0, shift = 0;
    for (;;) {
        int64_t b = d[at];
        at += 1;
        int64_t payload = b & 0x7F;
        if (payload != 0) {
            if (shift >= 62 || payload > (HUGE_N >> shift)) n = HUGE_N | n;
            else n |= payload << shift;
        }
        shift += 7;
        if (!(b & 0x80)) break;
    }
    int64_t end = py::checked_add(at, n);
    py::str s = d.slice(at, end).decode("utf-8");
    at = end;
    return s;
}

void Reader::annotations() {
    if (take_i() != 0) {
        throw py::ValueError("MGFX annotations are not read");   // MonoGame's compiler writes none
    }
}

py::bytes _string(const py::str& s) {
    py::bytes raw = s.encode("utf-8");
    int64_t n = raw.size();
    std::string out;
    for (;;) {
        int64_t b = n & 0x7F;
        n >>= 7;
        out.push_back(static_cast<char>(b | (n ? 0x80 : 0)));
        if (!n) {
            return py::bytes(out) + raw;
        }
    }
}

void _skip_parameters(Reader& r, py::set<py::str>* names) {
    int64_t count = r.take_i();
    for (int64_t k = 0; k < count; k++) {
        r.take_B();   // the class
        int64_t typ = r.take_B();
        py::str name = r.string();
        if (names != nullptr) {
            names->add(name);
        }
        r.string();
        r.annotations();
        int64_t rows = r.take_B();
        int64_t cols = r.take_B();
        int64_t elements_at = r.at;
        int64_t n_elements = r.take_i();
        r.at = elements_at;
        _skip_parameters(r);
        int64_t members_at = r.at;
        int64_t n_members = r.take_i();
        r.at = members_at;
        _skip_parameters(r);
        if (!n_elements && !n_members && (typ == 1 || typ == 2 || typ == 3)) {
            r.at += 4 * rows * cols;
        }
    }
}

namespace {

// struct.pack("f", v): the float32, OverflowError where a finite double is too large for one
void append_float(std::string& out, double v) {
    float f = static_cast<float>(v);
    if (std::isinf(f) && !std::isinf(v)) {
        throw py::OverflowError("float too large to pack with f format");
    }
    uint32_t bits;
    std::memcpy(&bits, &f, 4);
    for (int k = 0; k < 4; k++) {
        out.push_back(static_cast<char>((bits >> (8 * k)) & 0xFF));
    }
}

}  // namespace

py::bytes _parameter(const Parameter& p, bool element) {
    int64_t cls = CLASSES.get(p.cls, 3);
    int64_t typ = TYPES.get(p.type, 0);
    if (p.type.startswith(py::str("sampler"))) {
        typ = TYPES.at(py::str("texture"));   // what MonoGame calls the texture a sampler reads
    }
    std::string out = py::pack("<BB", cls, typ).raw();
    // an array's elements are nameless (MonoGame indexes a collection by its names)
    out += (_string(element ? py::str("") : p.name) + _string(element ? py::str("") : (p.semantic && !p.semantic->empty() ? *p.semantic : py::str("")))).raw();
    out += py::pack("<i", int64_t(0)).raw();
    int64_t rows = p.rows ? *p.rows : 0;
    int64_t cols = p.columns ? *p.columns : 0;
    if (p.cls == "vector") {
        rows = 1;
        if (!p.columns) {
            throw py::StructError("required argument is not an integer");   // the python packs None
        }
        cols = *p.columns;
    }
    out += py::pack("<BB", rows, cols).raw();
    if (p.elements && *p.elements != 0) {
        out += py::pack("<i", p.members.size()).raw();
        for (const Parameter& m : p.members) {
            out += _parameter(m, true).raw();
        }
        out += py::pack("<i", int64_t(0)).raw();
        return py::bytes(out);
    }
    if (p.cls == "struct") {
        out += py::pack("<i", int64_t(0)).raw();
        out += py::pack("<i", p.members.size()).raw();
        for (const Parameter& m : p.members) {
            out += _parameter(m).raw();
        }
        return py::bytes(out);
    }
    out += py::pack("<ii", int64_t(0), int64_t(0)).raw();
    if (typ == 1 || typ == 2 || typ == 3) {
        std::vector<double> values;
        if (p.value) {
            values.assign(p.value->vec().begin(), p.value->vec().end());
        }
        int64_t n = rows * cols;
        values.resize(static_cast<size_t>(n), 0.0);   // (values + [0] * n)[:n]
        if (p.cls == "matrix_columns") {   // the Xbox's effect keeps a column at a time; MonoGame a row
            std::vector<double> rowwise;
            for (int64_t r = 0; r < rows; r++) {
                for (int64_t c = 0; c < cols; c++) {
                    rowwise.push_back(values[static_cast<size_t>(c * rows + r)]);
                }
            }
            values = rowwise;
        }
        for (double v : values) {
            append_float(out, v);
        }
    }
    return py::bytes(out);
}

py::bytes add_parameters(const py::bytes& data, const py::list<Parameter>& parameters) {
    if (data.slice_to(4) != SIGNATURE) {
        throw py::ValueError("not a MonoGame effect");
    }
    Reader r(data, 10);   // signature, version, profile, effect key
    {
        int64_t buffers = r.take_i();   // constant buffers
        for (int64_t k = 0; k < buffers; k++) {
            r.string();
            r.take_h();
            int64_t n = r.take_i();
            r.at += 6 * n;
        }
    }
    {
        int64_t shaders = r.take_i();   // shaders
        for (int64_t k = 0; k < shaders; k++) {
            r.take_flag();
            int64_t n = r.take_i();
            r.at += n;
            int64_t samplers = r.take_B();
            for (int64_t j = 0; j < samplers; j++) {
                r.at += 3;
                if (r.take_flag()) {
                    r.at += 3 + 4 + 1 + 4 + 4 + 4;
                }
                r.string();
                r.take_B();
            }
            n = r.take_B();
            r.at += n;
            int64_t attributes = r.take_B();
            for (int64_t j = 0; j < attributes; j++) {
                r.string();
                r.at += 4;
            }
        }
    }
    int64_t count_at = r.at;
    py::set<py::str> names;
    _skip_parameters(r, &names);
    int64_t end = r.at;
    py::list<Parameter> extra;
    for (const Parameter& p : parameters) {
        if (!p.name.empty() && !names.contains(p.name) && !p.type.startswith(py::str("sampler")) && p.type != py::str("vertexshader") &&
            p.type != py::str("pixelshader") && p.type != py::str("string")) {
            extra.append(p);
        }
    }
    if (extra.empty()) {
        return data;
    }
    int64_t count = py::unpack_from1<int64_t>("<i", data, count_at);
    py::bytes added;
    for (const Parameter& p : extra) {
        added = added + _parameter(p);
    }
    return data.slice_to(count_at) + py::pack("<i", count + extra.size()) + data.slice(count_at + 4, end) + added + data.slice_from(end);
}

}  // namespace codebreaker::xna::mgfx
