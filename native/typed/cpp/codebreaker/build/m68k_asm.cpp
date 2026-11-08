// codebreaker/build/m68k_asm.cpp -- C++17 port of codebreaker.build.m68k_asm (see m68k_asm.hpp).

#include "codebreaker/build/m68k_asm.hpp"

#include <tuple>

#include "codebreaker/decompile/m68k.hpp"

namespace codebreaker::build::m68k_asm {

namespace m68k = codebreaker::decompile::m68k;
namespace z80_asm = codebreaker::build::z80_asm;

namespace {

using Labels = py::dict<py::str, int64_t>;

// str(e) of a KeyError (the port throws the missing name itself; Python's text is its repr)
py::str key_text(const py::KeyError& e) {
    return py::repr(py::str(e.what()));
}

// re.match(<pattern>, line): the pattern compiled once
struct Rx {
    py::re::Pattern p;
    explicit Rx(const char* pattern) : p(py::re::compile(py::str(pattern))) {}
    std::optional<py::re::Match> match(const py::str& s) const { return p.match(s); }
};

const Rx& re_label() { static const Rx r("^([A-Za-z_.@][\\w.@]*):\\s*(.*)$"); return r; }
const Rx& re_equ() { static const Rx r("(?i)^([A-Za-z_]\\w*)\\s+equ\\s+(.+)$"); return r; }
const Rx& re_org() { static const Rx r("(?i)^org\\s+(.+)$"); return r; }
const Rx& re_dc() { static const Rx r("(?i)^dc\\.([bwl])\\s+(.+)$"); return r; }
const Rx& re_dcb() { static const Rx r("(?i)^dcb\\.([bwl])\\s+([^,]+),\\s*(.+)$"); return r; }
const Rx& re_even() { static const Rx r("(?i)^even$"); return r; }

// {"b": 1, "w": 2, "l": 4}[c.lower()]
int64_t width_of(const py::str& c) {
    py::str k = c.lower();
    if (k == py::str("b")) return 1;
    if (k == py::str("w")) return 2;
    return 4;
}

// (v & ((1 << (8 * width)) - 1)).to_bytes(width, "big")
py::bytes big_endian(int64_t v, int64_t width) {
    int64_t mask = (int64_t(1) << (8 * width)) - 1;
    return py::to_bytes(v & mask, width, "big");
}

}  // namespace

py::str _strip(const py::str& line) {
    if (line.lstrip().startswith(py::str("*"))) {
        return py::str("");
    }
    std::u32string out;
    bool quoted = false;
    for (char32_t c : line.u32()) {
        if (c == U'"') {
            quoted = !quoted;
        }
        if (c == U';' && !quoted) {
            break;
        }
        out.push_back(c);
    }
    return py::str(out).rstrip();
}

py::bytes build(const py::str& source) {
    py::list<std::tuple<int64_t, py::str>> lines;
    int64_t n0 = 0;
    for (const py::str& raw : py::splitlines(source)) {
        ++n0;
        py::str l = _strip(raw).strip();
        if (py::truthy(l)) {
            lines.append(std::make_tuple(n0, l));
        }
    }
    Labels labels;
    py::bytearray image;
    py::dict<int64_t, int64_t> sizes;                    // line -> bytes on the first pass (branches settle on pass 2)
    for (bool final : {false, true}) {
        image = py::bytearray();
        int64_t pc = 0;
        for (const std::tuple<int64_t, py::str>& nl : lines) {
            int64_t n = std::get<0>(nl);
            py::str line = std::get<1>(nl);
            try {
                std::optional<py::re::Match> m = re_label().match(line);
                if (m) {
                    labels.set(m->group(1), pc);
                    line = m->group(2).strip();
                    if (!py::truthy(line)) {
                        continue;
                    }
                }
                m = re_equ().match(line);
                if (m) {
                    labels.set(m->group(1), m68k::_value(m->group(2), labels, pc));
                    continue;
                }
                m = re_org().match(line);
                if (m) {
                    pc = m68k::_value(m->group(1), labels, pc);
                    continue;
                }
                m = re_dc().match(line);
                if (m) {
                    int64_t width = width_of(m->group(1));
                    py::bytearray data;
                    for (const py::str& item : z80_asm::_items(m->group(2))) {
                        if (item.startswith(py::str("\"")) && item.endswith(py::str("\"")) && width == 1) {
                            data.extend(item.slice(1, py::len(item) - 1).encode("latin-1"));
                            continue;
                        }
                        int64_t v;
                        try {
                            v = m68k::_value(item, labels, pc + py::len(data));
                        } catch (const py::KeyError&) {
                            if (final) {
                                throw;
                            }
                            v = 0;
                        }
                        data.extend(big_endian(v, width));
                    }
                    _put(image, pc, data.view());
                    pc += py::len(data);
                    continue;
                }
                m = re_dcb().match(line);
                if (m) {
                    int64_t width = width_of(m->group(1));
                    int64_t count = m68k::_value(m->group(2), labels, pc);
                    int64_t v = m68k::_value(m->group(3), labels, pc);
                    _put(image, pc, py::repeat(big_endian(v, width), count));
                    pc += width * count;
                    continue;
                }
                if (re_even().match(line)) {
                    pc += pc & 1;
                    continue;
                }
                py::bytes code;
                try {
                    code = m68k::assemble(line, pc, labels);
                } catch (const py::KeyError&) {
                    if (final) {
                        throw;
                    }
                    code = _size_guess(line, pc, labels);
                }
                if (!final) {
                    sizes.set(n, py::len(code));
                } else if (py::len(code) != sizes.get(n, py::len(code))) {
                    throw AsmError("the instruction's size changed between passes");
                }
                _put(image, pc, code);
                pc += py::len(code);
            } catch (const py::KeyError& e) {
                throw AsmError(py::percent("line %d: %s (%s)", n, line, key_text(e)));
            } catch (const py::ValueError& e) {
                throw AsmError(py::percent("line %d: %s (%s)", n, line, py::str(e.what())));
            }
        }
    }
    return image.view();
}

py::bytes _size_guess(const py::str& line, int64_t pc, const py::dict<py::str, int64_t>& labels) {
    // (a dict that has every name, each unknown one worth pc + 4: grown by the names the assembler asks for)
    Labels any = labels.copy();
    for (;;) {
        try {
            return m68k::assemble(line, pc, any);
        } catch (const py::KeyError& e) {
            py::str name(e.what());
            if (any.contains(name)) {
                throw;
            }
            any.set(name, pc + 4);
        }
    }
}

void _put(const py::bytearray& image, int64_t at, const py::bytes& data) {
    if (at + py::len(data) > py::len(image)) {
        image.extend(py::bytes::zeros(at + py::len(data) - py::len(image)));
    }
    image.set_slice(at, at + py::len(data), data);
}

}  // namespace codebreaker::build::m68k_asm
