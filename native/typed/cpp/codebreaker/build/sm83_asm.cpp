// codebreaker/build/sm83_asm.cpp -- C++ from codebreaker.build.sm83_asm.

#include "codebreaker/build/sm83_asm.hpp"

#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "py_os.hpp"

namespace codebreaker::build::sm83_asm {

using codebreaker::build::z80_asm::AsmError;
namespace z80 = codebreaker::decompile::z80;
namespace sm83 = codebreaker::decompile::sm83;
using Labels = py::dict<py::str, int64_t>;

const int64_t BANK = 0x4000;

namespace {

// The patterns of build's line forms (matched from the start of the line).
const py::re::Pattern& label_re() {
    static const py::re::Pattern p = py::re::compile(py::str("^([A-Za-z_.][\\w.]*):{1,2}\\s*(.*)$"));
    return p;
}
const py::re::Pattern& equ_re() {
    static const py::re::Pattern p = py::re::compile(py::str("(?i)^(?:DEF\\s+)?([A-Za-z_]\\w*)\\s+EQU\\s+(.+)$"));
    return p;
}
const py::re::Pattern& section_re() {
    static const py::re::Pattern p = py::re::compile(
        py::str("(?i)^SECTION\\s+\"[^\"]*\"\\s*,\\s*(ROM0|ROMX)\\s*\\[\\s*([^\\]]+)\\]\\s*(?:,\\s*BANK\\s*\\[\\s*([^\\]]+)\\])?$"));
    return p;
}
const py::re::Pattern& data_re() {
    static const py::re::Pattern p = py::re::compile(py::str("(?i)^(DB|DW)\\s+(.+)$"));
    return p;
}
const py::re::Pattern& ds_re() {
    static const py::re::Pattern p = py::re::compile(py::str("(?i)^DS\\s+([^,]+)(?:,\\s*(.+))?$"));
    return p;
}
const py::re::Pattern& incbin_re() {
    static const py::re::Pattern p = py::re::compile(py::str("(?i)^INCBIN\\s+\"([^\"]+)\""));
    return p;
}
// the shape of a name in an expression (as decompile/z80.py's value() reads one)
const py::re::Pattern& name_re() {
    static const py::re::Pattern p = py::re::compile(py::str("[A-Za-z_.@][\\w.@]*"));
    return p;
}

// bytes([v & 0xFF])
py::bytes low_byte(int64_t v) { return py::bytes(std::string(1, static_cast<char>(v & 0xFF))); }
// bytes([v & 0xFF, (v >> 8) & 0xFF])
py::bytes little_word(int64_t v) {
    std::string s;
    s.push_back(static_cast<char>(v & 0xFF));
    s.push_back(static_cast<char>((v >> 8) & 0xFF));
    return py::bytes(s);
}

}  // namespace

py::bytes build(const py::str& source, int64_t size, const py::str& folder) {
    std::vector<std::pair<int64_t, py::str>> lines;
    {
        int64_t n = 0;
        for (const py::str& raw : py::splitlines(source)) {
            n = py::checked_add(n, 1);
            py::str l = z80_asm::_strip(raw).strip();
            if (!l.empty()) lines.emplace_back(n, l);
        }
    }
    Labels labels;
    py::bytearray image;
    for (bool final : {false, true}) {
        image = py::bytearray::zeros(size < 0 ? 0 : size);          // bytearray(b"\xFF" * size)
        for (char& c : image.raw_mut()) c = static_cast<char>(0xFF);
        int64_t bank = 0, base = 0, pos = 0;
        for (const auto& entry : lines) {
            int64_t n = entry.first;
            py::str line = entry.second;
            try {
                std::optional<py::re::Match> m = label_re().match(line);
                if (m) {
                    labels.set(m->group(1), py::checked_add(base, pos));
                    line = m->group(2).strip();
                    if (line.empty()) continue;
                }
                m = equ_re().match(line);
                if (m) {
                    try {
                        labels.set(m->group(1), z80::value(m->group(2), labels, py::checked_add(base, pos)));
                    } catch (const py::KeyError&) {
                        if (final) throw;
                    }
                    continue;
                }
                m = section_re().match(line);
                if (m) {
                    int64_t addr = z80::value(m->group(2), labels);
                    std::optional<py::str> bank_text = m->group_opt(3);
                    bank = bank_text ? z80::value(*bank_text, labels) : (m->group(1).upper() == "ROM0" ? 0 : 1);
                    base = bank == 0 ? 0 : BANK;
                    pos = py::checked_sub(addr, base);
                    continue;
                }
                m = data_re().match(line);
                if (m) {
                    bool word = m->group(1).upper() == "DW";
                    py::bytearray data;
                    for (const py::str& item : z80_asm::_items(m->group(2))) {
                        if (item.startswith(py::str("\"")) && item.endswith(py::str("\"")) && !word) {
                            data += item.slice(1, -1).encode("latin-1");
                            continue;
                        }
                        int64_t v;
                        try {
                            v = z80::value(item, labels, py::checked_add(py::checked_add(base, pos), data.size()));
                        } catch (const py::KeyError&) {
                            if (final) throw;
                            v = 0;
                        }
                        data += word ? little_word(v) : low_byte(v);
                    }
                    _put(image, bank, pos, data);
                    pos = py::checked_add(pos, data.size());
                    continue;
                }
                m = ds_re().match(line);
                if (m) {
                    int64_t count = z80::value(m->group(1), labels);
                    std::optional<py::str> fill = m->group_opt(2);
                    int64_t v = fill ? z80::value(*fill, labels) : 0;
                    // bytes([v & 0xFF]) * count: nothing for a count below 1.  A count past the ROM can
                    // only end in _put's AsmError, so that run is not built.
                    py::bytes run;
                    if (count > 0) {
                        if (count > image.size()) throw AsmError(py::percent("bytes outside the ROM (bank %d, offset $%X)", bank, pos));
                        run = py::bytes(std::string(static_cast<size_t>(count), static_cast<char>(v & 0xFF)));
                    }
                    _put(image, bank, pos, run);
                    pos = py::checked_add(pos, count);
                    continue;
                }
                m = incbin_re().match(line);
                if (m) {
                    py::bytes data;
                    {
                        auto f = py::open_rb(py::os::path::join(folder, m->group(1)));
                        data = f.read();
                        f.close();
                    }
                    _put(image, bank, pos, data);
                    pos = py::checked_add(pos, data.size());
                    continue;
                }
                py::bytes code;
                try {
                    code = sm83::assemble(line, py::checked_add(base, pos), labels);
                } catch (const py::KeyError&) {
                    if (final) throw;
                    code = _size_guess(line, py::checked_add(base, pos));
                }
                _put(image, bank, pos, code);
                pos = py::checked_add(pos, code.size());
            } catch (const py::KeyError& e) {
                throw AsmError(py::percent("line %d: %s (%s)", n, line, py::str(e.what())));
            } catch (const py::ValueError& e) {
                throw AsmError(py::percent("line %d: %s (%s)", n, line, py::str(e.what())));
            }
        }
    }
    return py::bytes(image.raw());
}

// The size an instruction line takes while its labels are not all known: assembled as if every name
// in it were an address (the Python's dict subclass that claims to hold any name; here a dict holding
// every name the line can be read to contain, each at that one address).
py::bytes _size_guess(const py::str& line, int64_t pc) {
    const int64_t any = line.upper().contains(py::str("LDH")) ? 0xFF80 : pc;
    Labels names;
    for (int64_t i = 0; i < line.size(); ++i) {
        std::optional<py::re::Match> m = name_re().match(line, i);
        if (m) names.set(m->group(0), any);
    }
    return sm83::assemble(line, pc, names);
}

void _put(const py::bytearray& image, int64_t bank, int64_t pos, const py::bytes& data) {
    int64_t at = py::checked_add(py::checked_mul(bank, BANK), pos);
    if (at < 0 || py::checked_add(at, data.size()) > image.size()) {
        throw AsmError(py::percent("bytes outside the ROM (bank %d, offset $%X)", bank, pos));
    }
    image.set_slice(at, py::checked_add(at, data.size()), data);
}

}  // namespace codebreaker::build::sm83_asm
