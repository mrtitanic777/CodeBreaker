// codebreaker/build/z80_asm.cpp -- C++17 port of codebreaker.build.z80_asm (see z80_asm.hpp).

#include "codebreaker/build/z80_asm.hpp"

#include <cstdio>

#include "codebreaker/decompile/z80.hpp"

namespace codebreaker::build::z80_asm {

namespace z80 = codebreaker::decompile::z80;

namespace {

using Labels = py::dict<py::str, int64_t>;

// s.encode("latin-1"): a UnicodeEncodeError (a ValueError) worded as Python words it
py::bytes encode_latin1(const py::str& s) {
    const std::u32string& u = s.u32();
    std::string out;
    for (size_t i = 0; i < u.size(); ++i) {
        if (u[i] < 256) {
            out.push_back(static_cast<char>(u[i]));
            continue;
        }
        size_t j = i;
        while (j < u.size() && u[j] >= 256) ++j;
        if (j - i == 1) {
            char buf[16];
            std::snprintf(buf, sizeof buf, u[i] < 0x10000 ? "\\u%04x" : "\\U%08x", static_cast<unsigned>(u[i]));
            throw py::UnicodeEncodeError(py::percent("'latin-1' codec can't encode character '%s' in position %d: ordinal not in range(256)",
                                                     py::str(buf), static_cast<int64_t>(i)));
        }
        throw py::UnicodeEncodeError(py::percent("'latin-1' codec can't encode characters in position %d-%d: ordinal not in range(256)",
                                                 static_cast<int64_t>(i), static_cast<int64_t>(j - 1)));
    }
    return py::bytes(out);
}

// str(e) of the KeyError z80.value raises (the port throws the missing name itself; Python's text is its repr)
py::str key_text(const py::KeyError& e) {
    return py::repr(py::str(e.what()));
}

// re.match(r"(?i)<pattern>", line): the pattern compiled once
struct Rx {
    py::re::Pattern p;
    explicit Rx(const char* pattern, int64_t flags = 0) : p(py::re::compile(py::str(pattern), flags)) {}
    std::optional<py::re::Match> match(const py::str& s) const { return p.match(s); }
};

const Rx& re_slot() { static const Rx r("(?i)SLOT\\s+(\\d+)\\s+(\\S+)"); return r; }
const Rx& re_bankstotal() { static const Rx r("(?i)BANKSTOTAL\\s+(\\S+)"); return r; }
const Rx& re_banksize() { static const Rx r("(?i)BANKSIZE\\s+(\\S+)"); return r; }
const Rx& re_label() { static const Rx r("^([A-Za-z_.@][\\w.@]*):\\s*(.*)$"); return r; }
const Rx& re_define() { static const Rx r("(?i)^\\.DEFINE\\s+(\\S+)\\s+(.+)$"); return r; }
const Rx& re_assign() { static const Rx r("^([A-Za-z_][\\w]*)\\s*=\\s*(.+)$"); return r; }
const Rx& re_bank() { static const Rx r("(?i)^\\.BANK\\s+(\\S+)(?:\\s+SLOT\\s+(\\S+))?$"); return r; }
const Rx& re_org() { static const Rx r("(?i)^\\.ORG\\s+(.+)$"); return r; }
const Rx& re_orga() { static const Rx r("(?i)^\\.ORGA\\s+(.+)$"); return r; }
const Rx& re_db() { static const Rx r("(?i)^\\.(DB|BYTE|DW|WORD)\\s+(.+)$"); return r; }
const Rx& re_dsb() { static const Rx r("(?i)^\\.DSB\\s+([^,]+),\\s*(.+)$"); return r; }
const Rx& re_incbin() { static const Rx r("(?i)^\\.INCBIN\\s+\"([^\"]+)\""); return r; }
const Rx& re_equ() { static const Rx r("(?i)^([A-Za-z_]\\w*)\\s+EQU\\s+(.+)$"); return r; }
const Rx& re_org_flat() { static const Rx r("(?i)^ORG\\s+(.+)$"); return r; }
const Rx& re_def_flat() { static const Rx r("(?i)^(DEFB|DB|DEFM|DEFW|DW)\\s+(.+)$"); return r; }
const Rx& re_defs() { static const Rx r("(?i)^(DEFS|DS)\\s+([^,]+)(?:,\\s*(.+))?$"); return r; }
const Rx& re_incbin_flat() { static const Rx r("(?i)^INCBIN\\s+\"([^\"]+)\""); return r; }

// bytes([v & 0xFF]) * count (a count below zero gives nothing)
py::bytes repeated_byte(int64_t v, int64_t count) {
    return count > 0 ? py::bytes(std::string(static_cast<size_t>(count), static_cast<char>(v & 0xFF))) : py::bytes("");
}

// The bytes of a data directive's items (the shared body of .DB/.DW and DEFB/DEFW): strings
// as latin-1 (bytes only), numbers and labels as bytes or little-endian words. A label not
// defined yet is 0 on the first pass.
py::bytearray data_items(const py::str& items_text, bool word, const Labels& labels, int64_t base, bool final) {
    py::bytearray data;
    for (const py::str& item : _items(items_text)) {
        if (item.startswith(py::str("\"")) && item.endswith(py::str("\"")) && !word) {
            data.extend(encode_latin1(item.slice(1, py::len(item) - 1)));
            continue;
        }
        int64_t v;
        try {
            v = z80::value(item, labels, base + py::len(data));
        } catch (const py::KeyError&) {
            if (final) {
                throw;
            }
            v = 0;
        }
        if (word) {
            data.append(v & 0xFF);
            data.append((v >> 8) & 0xFF);
        } else {
            data.append(v & 0xFF);
        }
    }
    return data;
}

py::bytes read_file(const py::str& folder, const py::str& name) {
    py::file f = py::open_rb(py::os::path::join(folder, name));
    py::bytes data = f.read();
    f.close();
    return data;
}

}  // namespace

py::str _strip(const py::str& line) {
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

py::list<py::str> _items(const py::str& text) {
    py::list<py::str> out;
    std::u32string cur;
    bool quoted = false;
    int64_t depth = 0;
    for (char32_t c : text.u32()) {
        if (c == U'"') {
            quoted = !quoted;
        }
        if (!quoted) {
            depth += c == U'(';
            depth -= c == U')';
        }
        if (c == U',' && !quoted && depth == 0) {
            out.append(py::str(cur).strip());
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    if (py::truthy(py::str(cur).strip())) {
        out.append(py::str(cur).strip());
    }
    return out;
}

py::bytes build_file(const py::str& path) {
    py::file f = py::file::open(path, py::str("r"), py::str("latin-1"));
    py::str text = f.read_text();
    f.close();
    return build(text, py::os::path::dirname(py::os::path::abspath(path)));
}

py::bytes build(const py::str& source, const py::str& folder) {
    py::list<std::tuple<int64_t, py::str>> lines;
    int64_t n0 = 0;
    for (const py::str& raw : py::splitlines(source)) {
        ++n0;
        py::str line = _strip(raw).strip();
        if (py::truthy(line)) {
            lines.append(std::make_tuple(n0, line));
        }
    }
    py::dict<int64_t, int64_t> slots;
    std::optional<int64_t> banks_total;
    int64_t bank_size = 0x4000;
    for (const std::tuple<int64_t, py::str>& nl : lines) {
        const py::str& line = std::get<1>(nl);
        std::optional<py::re::Match> m = re_slot().match(line);
        if (m) {
            slots.set(py::int_from_str(m->group(1)), z80::value(m->group(2), Labels()));
        }
        m = re_bankstotal().match(line);
        if (m) {
            banks_total = z80::value(m->group(1), Labels());
        }
        m = re_banksize().match(line);
        if (m) {
            bank_size = z80::value(m->group(1), Labels());
        }
    }
    Labels labels;
    py::bytearray image;
    for (bool final : {false, true}) {
        int64_t total = (banks_total && *banks_total != 0 ? *banks_total : 1) * bank_size;
        image = py::bytearray(repeated_byte(0xFF, total));
        int64_t bank = 0;
        int64_t slot_base = slots.get(0, 0);
        int64_t pos = 0;
        bool in_map = false;
        for (const std::tuple<int64_t, py::str>& nl : lines) {
            int64_t n = std::get<0>(nl);
            py::str line = std::get<1>(nl);
            py::str u = line.upper();
            if (u.startswith(py::str(".MEMORYMAP")) || u.startswith(py::str(".ROMBANKMAP"))) {
                in_map = true;
                continue;
            }
            if (u.startswith(py::str(".ENDME")) || u.startswith(py::str(".ENDRO"))) {
                in_map = false;
                continue;
            }
            if (in_map) {
                continue;
            }
            try {
                std::optional<py::re::Match> m = re_label().match(line);
                if (m) {
                    labels.set(m->group(1), slot_base + pos);
                    line = m->group(2).strip();
                    if (!py::truthy(line)) {
                        continue;
                    }
                    u = line.upper();
                }
                m = re_define().match(line);
                if (!m) {
                    m = re_assign().match(line);
                }
                if (m) {
                    try {
                        labels.set(m->group(1), z80::value(m->group(2), labels, slot_base + pos));
                    } catch (const py::KeyError&) {
                        if (final) {
                            throw;
                        }
                    }
                    continue;
                }
                m = re_bank().match(line);
                if (m) {
                    bank = z80::value(m->group(1), labels);
                    std::optional<py::str> slot = m->group_opt(2);
                    slot_base = py::truthy(slot) ? slots.get(z80::value(*slot, labels), 0) : slots.get(0, 0);
                    pos = 0;
                    continue;
                }
                m = re_org().match(line);
                if (m) {
                    pos = z80::value(m->group(1), labels);
                    continue;
                }
                m = re_orga().match(line);
                if (m) {
                    pos = z80::value(m->group(1), labels) - slot_base;
                    continue;
                }
                m = re_db().match(line);
                if (m) {
                    py::str kind = m->group(1).upper();
                    bool word = kind == py::str("DW") || kind == py::str("WORD");
                    py::bytearray data = data_items(m->group(2), word, labels, slot_base + pos, final);
                    _put(image, bank, bank_size, pos, data.view());
                    pos += py::len(data);
                    continue;
                }
                m = re_dsb().match(line);
                if (m) {
                    int64_t count = z80::value(m->group(1), labels);
                    int64_t v = z80::value(m->group(2), labels);
                    _put(image, bank, bank_size, pos, repeated_byte(v, count));
                    pos += count;
                    continue;
                }
                m = re_incbin().match(line);
                if (m) {
                    py::bytes data = read_file(folder, m->group(1));
                    _put(image, bank, bank_size, pos, data);
                    pos += py::len(data);
                    continue;
                }
                if (u.startswith(py::str(".EMPTYFILL")) || u.startswith(py::str(".SDSCTAG")) || u.startswith(py::str(".COMPUTESMSCHECKSUM"))) {
                    continue;
                }
                if (line.startswith(py::str("."))) {
                    throw AsmError("directive not understood");
                }
                py::bytes code;
                try {
                    code = z80::assemble(line, slot_base + pos, labels);
                } catch (const py::KeyError&) {
                    if (final) {
                        throw;
                    }
                    code = _size_guess(line, slot_base + pos);
                }
                _put(image, bank, bank_size, pos, code);
                pos += py::len(code);
            } catch (const py::KeyError& e) {
                throw AsmError(py::percent("line %d: %s (%s)", n, line, key_text(e)));
            } catch (const py::ValueError& e) {
                throw AsmError(py::percent("line %d: %s (%s)", n, line, py::str(e.what())));
            }
        }
    }
    return image.view();
}

py::bytes _size_guess(const py::str& line, int64_t pc) {
    // (a dict that has every name, each worth `pc`: grown by the names the assembler asks for)
    Labels any;
    for (;;) {
        try {
            return z80::assemble(line, pc, any);
        } catch (const py::KeyError& e) {
            py::str name(e.what());
            if (any.contains(name)) {
                throw;
            }
            any.set(name, pc);
        }
    }
}

void _put(const py::bytearray& image, int64_t bank, int64_t bank_size, int64_t pos, const py::bytes& data) {
    int64_t at = bank * bank_size + pos;
    if (at < 0 || at + py::len(data) > py::len(image)) {
        throw AsmError(py::percent("bytes outside the ROM (bank %d, offset $%X)", bank, pos));
    }
    image.set_slice(at, at + py::len(data), data);
}

std::tuple<int64_t, py::bytes> build_flat(const py::str& source, const py::str& folder) {
    py::list<std::tuple<int64_t, py::str>> lines;
    int64_t n0 = 0;
    for (const py::str& raw : py::splitlines(source)) {
        ++n0;
        py::str line = _strip(raw).strip();
        if (py::truthy(line)) {
            lines.append(std::make_tuple(n0, line));
        }
    }
    Labels labels;
    py::dict<int64_t, py::bytes> chunks;
    for (bool final : {false, true}) {
        chunks = py::dict<int64_t, py::bytes>();
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
                    try {
                        labels.set(m->group(1), z80::value(m->group(2), labels, pc));
                    } catch (const py::KeyError&) {
                        if (final) {
                            throw;
                        }
                    }
                    continue;
                }
                m = re_org_flat().match(line);
                if (m) {
                    pc = z80::value(m->group(1), labels, pc);
                    continue;
                }
                m = re_def_flat().match(line);
                if (m) {
                    py::str kind = m->group(1).upper();
                    bool word = kind == py::str("DEFW") || kind == py::str("DW");
                    py::bytearray data = data_items(m->group(2), word, labels, pc, final);
                    chunks.set(pc, data.view());
                    pc += py::len(data);
                    continue;
                }
                m = re_defs().match(line);
                if (m) {
                    int64_t count = z80::value(m->group(2), labels, pc);
                    std::optional<py::str> given = m->group_opt(3);
                    int64_t v = py::truthy(given) ? z80::value(*given, labels, pc) : 0;
                    chunks.set(pc, repeated_byte(v, count));
                    pc += count;
                    continue;
                }
                m = re_incbin_flat().match(line);
                if (m) {
                    py::bytes data = read_file(folder, m->group(1));
                    chunks.set(pc, data);
                    pc += py::len(data);
                    continue;
                }
                py::bytes code;
                try {
                    code = z80::assemble(line, pc, labels);
                } catch (const py::KeyError&) {
                    if (final) {
                        throw;
                    }
                    code = _size_guess(line, pc);
                }
                chunks.set(pc, code);
                pc += py::len(code);
            } catch (const py::KeyError& e) {
                throw AsmError(py::percent("line %d: %s (%s)", n, line, key_text(e)));
            } catch (const py::ValueError& e) {
                throw AsmError(py::percent("line %d: %s (%s)", n, line, py::str(e.what())));
            }
        }
    }
    if (chunks.empty()) {
        return {0, py::bytes("")};
    }
    int64_t start = py::min_of(chunks.keys());
    int64_t end = 0;
    bool first = true;
    for (const std::tuple<int64_t, py::bytes>& ab : chunks.items()) {
        int64_t e = std::get<0>(ab) + py::len(std::get<1>(ab));
        if (first || e > end) {
            end = e;
        }
        first = false;
    }
    py::bytearray image = py::bytearray::zeros(end - start);
    for (const std::tuple<int64_t, py::bytes>& ab : chunks.items()) {
        int64_t a = std::get<0>(ab);
        image.set_slice(a - start, a - start + py::len(std::get<1>(ab)), std::get<1>(ab));
    }
    return {start, image.view()};
}

}  // namespace codebreaker::build::z80_asm
