// codebreaker/symbols/libraries/omf.cpp -- typed C++ from codebreaker.symbols.libraries.omf.

#include "codebreaker/symbols/libraries/omf.hpp"

#include <optional>

namespace codebreaker::symbols::libraries::omf {

const py::dict<int64_t, int64_t> _WIDTH = {{0, 1}, {1, 2}, {2, 2}, {3, 4}, {4, 1}, {5, 2}, {9, 4}, {11, 6}, {13, 4}};   // bytes a 16-bit fix-up covers, by kind

std::tuple<int64_t, int64_t> _number(const py::bytes& b, int64_t i) {
    if (b[i] & 0x80) return {((b[i] & 0x7F) << 8) | b[i + 1], i + 2};
    return {b[i], i + 1};
}

py::bytes fields_only(const py::bytes& mask) {
    std::string out;
    for (int64_t m : mask) out.push_back(m == 1 ? 1 : 0);
    return py::bytes(out);
}

py::bytearray _mask(const py::dict<int64_t, int64_t>& masked, int64_t start, int64_t size) {
    py::bytearray out = py::bytearray::zeros(size);
    for (const auto& [o, v] : masked.items()) {
        if (start <= o && o < start + size) out[o - start] = v;
    }
    return out;
}

py::list<Function> functions(const py::bytes& data) {
    py::list<Function> out;
    int64_t at = 0, page = 1;
    std::optional<int64_t> start;
    if (data.slice(0, 1) == py::bytes("\xF0", 1)) {
        page = py::unpack_from1<int64_t>("<H", data, 1) + 3;
        at = page;
    }
    while (at + 3 <= py::len(data) && data[at] != 0xF1) {
        int64_t kind = data[at], size = py::unpack_from1<int64_t>("<H", data, at + 1);
        if (!start) start = at;
        at += 3 + size;
        if (kind == 0x8A || kind == 0x8B) {   // the end of one object module
            try {
                out.extend(module_functions(data.slice(*start, at)));
            } catch (const py::StructError&) {
            } catch (const py::IndexError&) {
            }
            start.reset();
            at += py::mod(-at, page);
        }
    }
    return out;
}

py::list<Function> module_functions(const py::bytes& module) {
    using Segment = std::tuple<py::str, py::bytearray, py::dict<int64_t, int64_t>>;   // (class, bytes, masked offsets)
    using Piece = std::tuple<bool, py::bytearray, py::dict<int64_t, int64_t>>;        // (is code, bytes, masked offsets)
    py::list<py::str> names{py::str("")};
    py::list<Segment> segments;
    py::list<std::tuple<int64_t, int64_t, py::str>> publics;   // (segment, offset, name)
    py::dict<int64_t, Piece> pieces;
    // last: (bytes, {masked offset: 1|2}, offset) fix-ups apply to
    std::optional<std::tuple<py::bytearray, py::dict<int64_t, int64_t>, int64_t>> last;
    int64_t at = 0;
    while (at + 3 <= py::len(module)) {
        int64_t kind = module[at], size = py::unpack_from1<int64_t>("<H", module, at + 1);
        py::bytes b = module.slice(at + 3, at + 3 + size - 1);
        at += 3 + size;
        int64_t wide = kind & 1;   // the 32-bit form of a record
        if (kind == 0x96) {
            int64_t i = 0;
            while (i < py::len(b)) {
                names.append(b.slice(i + 1, i + 1 + b[i]).decode("latin1"));
                i += 1 + b[i];
            }
        } else if (kind == 0x98 || kind == 0x99) {
            int64_t i = 1 + ((b[0] >> 5) == 0 ? 3 : 0) + (wide ? 4 : 2);
            int64_t _nm, cls;
            std::tie(_nm, i) = _number(b, i);
            std::tie(cls, i) = _number(b, i);
            segments.append({cls < py::len(names) ? names[cls] : py::str(""), py::bytearray(),
                             py::dict<int64_t, int64_t>()});
        } else if (kind == 0xA0 || kind == 0xA1 || kind == 0xC2 || kind == 0xC3) {
            py::bytearray buf;
            py::dict<int64_t, int64_t> masked;
            int64_t off;
            py::bytes chunk;
            if (kind >= 0xC2) {   // a piece of its own
                int64_t flags = b[0], how = b[1] & 0x0F;
                off = py::unpack_from1<int64_t>(wide ? "<I" : "<H", b, 3);
                int64_t _type, i;
                std::tie(_type, i) = _number(b, 3 + (wide ? 4 : 2));
                bool code = how == 1 || how == 3;
                if (how == 0) {
                    int64_t _grp, seg;
                    std::tie(_grp, i) = _number(b, i);
                    std::tie(seg, i) = _number(b, i);
                    i += seg == 0 ? 2 : 0;
                    code = 1 <= seg && seg <= py::len(segments) &&
                           std::get<0>(segments[seg - 1]).upper().contains(py::str("CODE"));
                }
                int64_t name;
                std::tie(name, i) = _number(b, i);
                if (!pieces.contains(name)) pieces.set(name, Piece{code && !(flags & 2), py::bytearray(), {}});
                const Piece& where = pieces.at(name);
                buf = std::get<1>(where);
                masked = std::get<2>(where);
                chunk = b.slice_from(i);
            } else {
                int64_t seg, i;
                std::tie(seg, i) = _number(b, 0);
                off = py::unpack_from1<int64_t>(wide ? "<I" : "<H", b, i);
                chunk = b.slice_from(i + (wide ? 4 : 2));
                if (!(1 <= seg && seg <= py::len(segments))) {
                    last.reset();
                    continue;
                }
                buf = std::get<1>(segments[seg - 1]);
                masked = std::get<2>(segments[seg - 1]);
            }
            buf.extend(py::bytes::zeros(std::max<int64_t>(0, off + py::len(chunk) - py::len(buf))));
            buf.set_slice(off, off + py::len(chunk), chunk);
            last = std::make_tuple(buf, masked, off);
        } else if ((kind == 0x9C || kind == 0x9D) && last) {
            py::bytearray buf = std::get<0>(*last);
            py::dict<int64_t, int64_t> masked = std::get<1>(*last);
            int64_t base = std::get<2>(*last);
            int64_t i = 0;
            while (i < py::len(b)) {
                int64_t first = b[i];
                if (!(first & 0x80)) {   // a thread: says how later fix-ups are framed
                    i += 1;
                    if (((first >> 2) & 7) < 3) {
                        int64_t _x;
                        std::tie(_x, i) = _number(b, i);
                    }
                    continue;
                }
                int64_t where = ((first & 3) << 8) | b[i + 1];
                int64_t loc = (first >> 2) & 0xF, fix = b[i + 2];
                i += 3;
                if (!(fix & 0x80) && ((fix >> 4) & 7) < 3) {
                    int64_t _x;
                    std::tie(_x, i) = _number(b, i);
                }
                if (!(fix & 0x08)) {
                    int64_t _x;
                    std::tie(_x, i) = _number(b, i);
                }
                if (!(fix & 0x04)) i += wide ? 4 : 2;
                int64_t start = base + where, n = _WIDTH.get(loc, 2);
                if (loc == 3 && start && buf[start - 1] == 0x9A) {
                    masked.setdefault(start - 1, 2);   // a far call: the linker may rewrite all of it
                }
                for (int64_t o = start; o < start + n; ++o) masked.set(o, 1);
            }
        } else if (kind == 0x90 || kind == 0x91) {
            int64_t _grp, seg, i;
            std::tie(_grp, i) = _number(b, 0);
            std::tie(seg, i) = _number(b, i);
            if (seg == 0) i += 2;
            while (i < py::len(b)) {
                py::str name = b.slice(i + 1, i + 1 + b[i]).decode("latin1");
                i += 1 + b[i];
                int64_t off = py::unpack_from1<int64_t>(wide ? "<I" : "<H", b, i);
                i += wide ? 4 : 2;
                int64_t _type;
                std::tie(_type, i) = _number(b, i);
                publics.append({seg, off, name});
            }
        }
    }
    py::list<Function> out;
    for (const auto& [n, piece] : pieces.items()) {
        const auto& [code, buf, masked] = piece;
        if (code && n < py::len(names))
            out.append(Function{names[n], py::bytes(buf.raw()), _mask(masked, 0, py::len(buf)), 0});
    }
    publics.sort();
    for (int64_t k = 0; k < py::len(publics); ++k) {
        auto [seg, start, name] = publics[k];
        if (!(1 <= seg && seg <= py::len(segments)) ||
            !std::get<0>(segments[seg - 1]).upper().contains(py::str("CODE")))
            continue;
        py::bytearray code = std::get<1>(segments[seg - 1]);
        py::dict<int64_t, int64_t> masked = std::get<2>(segments[seg - 1]);
        // the next public of the same segment that starts after this one, else the segment's end
        int64_t end = py::len(code);
        for (int64_t j = k + 1; j < py::len(publics); ++j) {
            const auto& [s, o, _n] = publics[j];
            if (s == seg && o > start) {
                end = o;
                break;
            }
        }
        py::bytes piece = code.view().slice(start, end);
        out.append(Function{name, piece, _mask(masked, start, py::len(piece)), start});
    }
    return out;
}

}  // namespace codebreaker::symbols::libraries::omf
