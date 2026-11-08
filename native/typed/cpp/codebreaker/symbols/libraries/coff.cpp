// codebreaker/symbols/libraries/coff.cpp -- typed C++ port of codebreaker.symbols.libraries.coff (see coff.hpp).

#include "codebreaker/symbols/libraries/coff.hpp"

#include <algorithm>
#include <limits>

#include "codebreaker/compare/mask.hpp"

namespace codebreaker::symbols::libraries::coff {

namespace masks = codebreaker::compare::mask;

const int64_t I386 = 0x014C;
const int64_t POWERPC = 0x01F2;
// IMAGE_REL_PPC_* (the low byte of a relocation's type)
const int64_t REL_ADDR32 = 0x2, REL_ADDR24 = 0x3, REL_ADDR16 = 0x4, REL_ADDR14 = 0x5;
const int64_t REL_REL24 = 0x6, REL_REL14 = 0x7, REL_ADDR32NB = 0xA;
const int64_t REL_SECREL = 0xB, REL_REFHI = 0x10, REL_REFLO = 0x11, REL_PAIR = 0x12;
const int64_t REL_SECRELLO = 0x13, REL_SECRELHI = 0x14, REL_GPREL = 0x15;

namespace {

bool is_space(int64_t c) {   // the whitespace bytes.split() splits on
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == 0x0B || c == 0x0C;
}

bool is_digit(int64_t c) { return c >= '0' && c <= '9'; }

// int(token) for a non-empty run of non-whitespace bytes: an optional sign, then digits with single
// underscores between them. Raises ValueError otherwise, as int() does. A number beyond int64 is
// saturated towards +/- int64's end (see the header: a negative one raises OverflowError instead).
int64_t py_int(const py::bytes& token) {
    int64_t n = py::len(token);
    int64_t i = 0;
    bool neg = false;
    if (n > 0 && (token[0] == '+' || token[0] == '-')) {
        neg = token[0] == '-';
        i = 1;
    }
    if (i >= n) throw py::ValueError("invalid literal for int() with base 10");
    const int64_t limit = std::numeric_limits<int64_t>::max() / 4;
    int64_t value = 0;
    bool prev_digit = false, overflow = false;
    for (; i < n; ++i) {
        int64_t c = token[i];
        if (is_digit(c)) {
            if (!overflow) {
                value = value * 10 + (c - '0');
                if (value > limit) overflow = true;
            }
            prev_digit = true;
        } else if (c == '_' && prev_digit && i + 1 < n && is_digit(token[i + 1])) {
            prev_digit = false;
        } else {
            throw py::ValueError("invalid literal for int() with base 10");
        }
    }
    if (overflow) {
        if (neg) throw py::OverflowError("int too large for int64");
        return limit;
    }
    return neg ? -value : value;
}

// the last whitespace-separated token of `b` (b.split()[-1]); IndexError if there is none
py::bytes last_token(const py::bytes& b) {
    int64_t end = py::len(b);
    while (end > 0 && is_space(b[end - 1])) --end;
    if (end == 0) throw py::IndexError("list index out of range");
    int64_t start = end;
    while (start > 0 && !is_space(b[start - 1])) --start;
    return b.slice(start, end);
}

}  // namespace

py::list<std::tuple<py::str, py::bytes>> members(const py::bytes& data) {
    py::list<std::tuple<py::str, py::bytes>> out;
    int64_t at = 8;
    py::bytes long_names("");
    while (at + 60 <= py::len(data)) {
        py::bytes head = data.slice(at, at + 60);
        int64_t size;
        try {   // the size is the last number of the header
            size = py_int(last_token(head.slice(16, 58).replace(py::bytes("\0", 1), py::bytes(" "))));   // (the first 32-bit tools misplaced it)
        } catch (const py::ValueError&) {
            return out;
        } catch (const py::IndexError&) {
            return out;
        }
        py::bytes body = data.slice(at + 60, at + 60 + size);
        at += 60 + size + (size & 1);
        py::bytes label = head.slice(0, 16).strip();
        if (label == py::bytes("//")) {
            long_names = body;
        } else if (label != py::bytes("/") && label != py::bytes("")) {
            if (label.startswith(py::bytes("/")) && label.slice(1, py::len(label)).isdigit()) {
                int64_t from = py_int(label.slice(1, py::len(label)));
                py::bytes name = long_names.slice(from, py::len(long_names));
                label = py::split(py::split(name, py::bytes("\0", 1))[0], py::bytes("\n"))[0];
            }
            out.append(std::make_tuple(label.rstrip(py::bytes("/")).decode("latin1"), body));
        }
    }
    return out;
}

Coff::Coff(const py::bytes& data_in) : data(data_in) {
    int64_t nsec, nsym, opt;
    {
        auto [machine_, nsec_, _t, symptr_, nsym_, opt_, _c] =
            py::unpack_from<int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t>("<HHIIIHH", data, 0);
        machine = machine_;
        nsec = nsec_;
        symptr = symptr_;
        nsym = nsym_;
        opt = opt_;
    }
    for (int64_t i = 0; i < nsec; ++i) {
        int64_t o = 20 + opt + i * 40;
        auto [_vs, _va, rawsize, rawptr, relptr, _l, nrel, _n, chars] =
            py::unpack_from<int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t>("<IIIIIIHHI", data, o + 8);
        Section s;
        s.size = rawsize;
        s.ptr = rawptr;
        s.relptr = relptr;
        s.nrel = nrel;
        s.chars = chars;
        sections.append(s);
    }
    int64_t strtab = symptr + nsym * 18;
    int64_t i = 0;
    while (i < nsym && symptr) {
        int64_t o = symptr + i * 18;
        py::bytes raw = data.slice(o, o + 18);
        py::str name;
        if (py::len(raw) < 18) {   // (a record cut short: the table ends)
            break;
        }
        if (raw.slice(0, 4) == py::bytes("\0\0\0\0", 4)) {
            int64_t so = strtab + py::unpack_from1<int64_t>("<I", raw, 4);
            name = data.slice(so, data.find(py::bytes("\0", 1), so)).decode("latin-1");
        } else {
            name = raw.slice(0, 8).rstrip(py::bytes("\0", 1)).decode("latin-1");
        }
        auto [value, secnum, typ, cls, naux] = py::unpack_from<int64_t, int64_t, int64_t, int64_t, int64_t>("<IhHBB", raw, 8);
        Symbol sym;
        sym.name = name;
        sym.value = value;
        sym.sec = secnum;
        sym.type = typ;
        sym.cls = cls;
        symbols.append(sym);
        for (int64_t k = 0; k < naux; ++k) symbols.append(std::nullopt);
        i += 1 + naux;
    }
}

py::list<Relocation> Coff::relocations(const Section& sec) const {
    py::list<Relocation> out;
    for (int64_t k = 0; k < sec.nrel; ++k) {
        out.append(py::unpack_from<int64_t, int64_t, int64_t>("<IIH", data, sec.relptr + k * 10));
    }
    return out;
}

py::list<Function> functions(const py::bytes& data) {
    if (py::len(data) < 20) {
        return py::list<Function>();
    }
    Coff obj(data);
    if (obj.machine == I386 && obj.symptr) {
        return _x86(obj);
    }
    if (obj.machine == POWERPC && !obj.symbols.empty()) {
        return _powerpc(obj);
    }
    return py::list<Function>();
}

py::list<Function> _x86(const Coff& obj) {
    // code section number -> (its bytes, the offsets a relocation covers, its function symbols)
    using Sym = std::tuple<int64_t, py::str, int64_t>;   // (offset, name, storage class)
    struct Code {
        int64_t number;
        py::bytes code;
        py::set<int64_t> masked;
        std::vector<Sym> symbols;
    };
    std::vector<Code> sections;
    for (int64_t i = 0; i < obj.sections.size(); ++i) {
        const Section& sec = obj.sections[i];
        if ((sec.chars & 0x20) && sec.size && sec.ptr) {   // holds code
            py::set<int64_t> masked;
            for (const Relocation& r : obj.relocations(sec)) {
                int64_t at = std::get<0>(r), kind = std::get<2>(r);
                int64_t width = (kind == 1 || kind == 2) ? 2 : 4;
                for (int64_t o = at; o < at + width; ++o) masked.add(o);
            }
            sections.push_back(Code{i + 1, obj.data.slice(sec.ptr, sec.ptr + sec.size), masked, {}});
        }
    }
    for (const std::optional<Symbol>& s : obj.symbols) {
        // a function: external, or static and typed as a function (jump-table labels are neither)
        if (s) {
            for (Code& c : sections) {
                if (c.number == s->sec && (s->cls == 2 || (s->cls == 3 && s->type == 0x20))) {
                    c.symbols.push_back(Sym(s->value, s->name, s->cls));
                    break;
                }
            }
        }
    }
    py::list<Function> out;
    for (Code& c : sections) {
        std::sort(c.symbols.begin(), c.symbols.end());
        const int64_t total = py::len(c.code);
        for (size_t k = 0; k < c.symbols.size(); ++k) {
            int64_t start = std::get<0>(c.symbols[k]);
            const py::str& name = std::get<1>(c.symbols[k]);
            int64_t storage = std::get<2>(c.symbols[k]);
            int64_t end = total;
            for (size_t j = k + 1; j < c.symbols.size(); ++j) {
                if (std::get<0>(c.symbols[j]) > start) {
                    end = std::get<0>(c.symbols[j]);
                    break;
                }
            }
            if (storage == 2) {
                py::bytes piece = c.code.slice(start, end);
                py::list<int64_t> inside;
                for (int64_t o : c.masked) {
                    if (start <= o && o < end) inside.append(o - start);
                }
                out.append(Function{name, piece, masks::of(inside, py::len(piece)), start});
            }
        }
    }
    return out;
}

std::optional<int64_t> _field(int64_t typ) {
    int64_t t = typ & 0xFF;
    if (t == REL_REL24 || t == REL_ADDR24) {
        return 0x03FFFFFC;
    }
    if (t == REL_REL14 || t == REL_ADDR14) {
        return 0x0000FFFC;
    }
    if (t == REL_REFHI || t == REL_REFLO || t == REL_ADDR16 || t == REL_SECRELLO || t == REL_SECRELHI || t == REL_GPREL) {
        return 0x0000FFFF;
    }
    return std::nullopt;
}

py::list<Function> _powerpc(const Coff& obj) {
    py::list<Function> out;
    for (int64_t si = 1; si <= obj.sections.size(); ++si) {
        const Section& sec = obj.sections[si - 1];
        if (!(sec.chars & 0x20) || sec.size < 8) {   // IMAGE_SCN_CNT_CODE
            continue;
        }
        py::bytes code = obj.data.slice(sec.ptr, sec.ptr + sec.size);
        int64_t n = py::len(code) / 4;
        py::list<int64_t> ignore;
        for (int64_t k = 0; k < n; ++k) ignore.append(0);
        bool skip = false;
        for (const Relocation& r : obj.relocations(sec)) {
            int64_t va = std::get<0>(r), typ = std::get<2>(r);
            if (skip) {   // a PAIR carries data, not a place
                skip = false;
                continue;
            }
            if ((typ & 0xFF) == REL_PAIR) {
                continue;
            }
            if ((typ & 0xFF) == REL_REFHI) {
                skip = true;
            }
            std::optional<int64_t> m = _field(typ);
            int64_t w = (va & ~int64_t(3)) / 4;
            if (w >= n) {
                continue;
            }
            if (!m && (va & 3)) {   // an unaligned 32-bit field: two words
                ignore[w] = 0xFFFFFFFF;
                if (w + 1 < n) {
                    ignore[w + 1] = 0xFFFFFFFF;
                }
            } else {
                ignore[w] = ignore[w] | (m ? *m : 0xFFFFFFFF);
            }
        }
        std::vector<Symbol> syms;
        for (const std::optional<Symbol>& s : obj.symbols) {
            if (s && s->sec == si && (s->cls == 2 || s->cls == 3) &&
                !s->name.startswith(std::make_tuple(py::str("."), py::str("$"))) && ((s->type & 0x20) || s->cls == 2)) {
                syms.push_back(*s);
            }
        }
        std::stable_sort(syms.begin(), syms.end(), [](const Symbol& a, const Symbol& b) { return a.value < b.value; });
        for (size_t k = 0; k < syms.size(); ++k) {
            int64_t start = syms[k].value;
            int64_t end = k + 1 < syms.size() ? syms[k + 1].value : py::len(code);
            if (end - start >= 8 && !(start & 3)) {
                out.append(Function{syms[k].name, code.slice(start, end), ignore.slice(start / 4, end / 4), start});
            }
        }
    }
    return out;
}

}  // namespace codebreaker::symbols::libraries::coff
