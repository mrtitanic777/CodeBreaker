// codebreaker/exact/linkemu.cpp -- C++17 port of codebreaker.exact.linkemu (see linkemu.hpp).

#include "codebreaker/exact/linkemu.hpp"

#include <algorithm>
#include <string>
#include <vector>

#include "codebreaker/exact/aout.hpp"
#include "codebreaker/formats/omf.hpp"

namespace codebreaker::exact::linkemu {

namespace omf = codebreaker::formats::omf;
namespace aout = codebreaker::exact::aout;

const py::dict<int64_t, int64_t> ALIGN = {{0, 1}, {1, 1}, {2, 2}, {3, 16}, {4, 256}, {5, 4}};

py::list<Module> link24_mods;
int64_t link24_dgroup = 0;

int64_t W(const py::bytes& b, int64_t k) { return py::unpack_from1<int64_t>("<H", b, k); }

namespace {

// a piece's place; None (not placed) added to a number is a TypeError
int64_t at_of(const Piece& p) {
    if (!p.at) throw py::TypeError("unsupported operand type(s) for +: 'NoneType' and 'int'");
    return *p.at;
}

// a name given by an object as text: the str the Python's .lower() / .upper() needs
const py::str& text_of(const omf::Target& tgt) {
    const auto& v = std::get<1>(tgt);
    if (!std::holds_alternative<py::str>(v)) throw py::AttributeError("object has no attribute 'upper'");
    return std::get<py::str>(v);
}

}  // namespace

Loaded load(const py::bytes& raw, bool odd, const std::optional<py::dict<py::str, py::str>>& real) {
    omf::Obj o = omf::read(raw);
    bool even = aout::is_aout(raw);   // (a C object's pieces each begin at an even address)
    auto name = [&](const py::str& s) -> py::str { return real ? real->setdefault(s.upper(), s) : s; };
    Pieces pieces;
    for (const omf::SegDef& sd : o.segs) {
        const py::str& s = std::get<0>(sd);
        const py::str& cls = std::get<1>(sd);
        int64_t ln = std::get<2>(sd);
        int64_t al = std::get<3>(sd);
        if (even) al = 2;
        py::bytes d = o.seg.get(s, py::bytes()).slice_to(o.filled.get(s, 0));
        if (odd && cls.upper() == py::str("CODE") && !d.empty()) {
            d = d.slice_to(-1);   // (the compiler's filler, which is not there)
            ln = py::len(d);
        }
        PieceRef p = std::make_shared<Piece>(name(s), ALIGN.get(al, 1), d, ln, o.filled.get(s, 0) != 0);
        p->stack = (o.combine.contains(s) && o.combine.at(s) == 5) || cls.upper() == py::str("STACK");
        pieces.set(name(s), p);
    }
    Defined defined;
    for (const omf::Public& pb : o.pubs) {
        const auto& s = std::get<1>(pb);
        if (s && !s->empty()) {
            defined.set(std::get<0>(pb).lower(), std::make_tuple(name(*s), std::get<2>(pb)));
        }
    }
    Common common;
    for (const auto& [n, v] : o.comm.items()) {
        common.set(n.lower(), std::get<0>(v));
    }
    Fixes fixes;
    for (const omf::Fixup& f : o.fix) {
        const py::str& s = std::get<0>(f);
        int64_t off = std::get<1>(f);
        const py::str& kind = std::get<2>(f);
        const omf::Target& tgt = std::get<3>(f);
        int64_t disp = std::get<4>(f);
        int64_t add = (W(o.seg.at(s), off) + disp) & 0xFFFF;
        Target tg = std::get<0>(tgt) == py::str("ext")   ? Target(py::str("sym"), text_of(tgt).lower())
                    : std::get<0>(tgt) == py::str("seg") ? Target(py::str("seg"), name(text_of(tgt)))
                                                         : Target(py::str("grp"), py::str(""));
        if (kind == py::str("base")) {
            fixes.append(std::make_tuple(name(s), off, py::str("seg"), tg, add));
        } else if (kind == py::str("rel")) {
            fixes.append(std::make_tuple(name(s), off, py::str("rel"), tg, (add - off - 2) & 0xFFFF));
        } else {
            fixes.append(std::make_tuple(name(s), off, py::str("abs"), tg, add));
        }
    }
    Start start;
    if (o.start) {
        start = std::make_tuple(name(std::get<0>(*o.start)), std::get<1>(*o.start));
    }
    return std::make_tuple(pieces, defined, common, fixes, start);
}

py::bytes link24(const py::list<Object>& objects, const Symbols& symbols, const std::function<void(const py::str&)>& log) {
    auto say = [&](const py::str& s) {
        if (log) log(s);
    };
    py::dict<py::str, py::str> real;
    py::list<Module> mods;
    for (const Object& ob : objects) {
        const py::str& n = std::get<0>(ob);
        auto [pieces, defined, common, fixes, start] = load(std::get<1>(ob), std::get<2>(ob), real);
        mods.append(Module{n, pieces, defined, common, fixes, start});
    }
    py::list<py::str> order;   // (the segments: a group's in the order the group lists
    for (const Object& ob : objects) {   //  them - an assembler writes its SEGDEFs in the order of
        omf::Obj g = omf::read(std::get<1>(ob));   //  its names, its GRPDEF as the source says - the others
        for (const auto& [gname, mem] : g.groups.items()) {   //  in the order the modules name them)
            (void)gname;
            for (const py::str& sg : mem) {
                if (!order.contains(real.get(sg.upper(), sg))) {
                    order.append(real.get(sg.upper(), sg));
                }
            }
        }
    }
    for (const Module& m : mods) {
        for (const py::str& sg : m.pieces) {
            if (!order.contains(sg)) {
                if (sg.upper().endswith(py::str("TEXT"))) {
                    order.insert(0, sg);
                } else {
                    order.append(sg);
                }
            }
        }
    }
    py::list<py::str> code;
    for (const py::str& sg : order) {
        bool any = false;
        for (const Module& m : mods) {
            PieceRef p = m.pieces.get(sg);
            if (p && p->hasdata && sg.upper().endswith(py::str("TEXT"))) {
                any = true;
                break;
            }
        }
        if (any) code.append(sg);
    }
    if (code.empty()) code = order.slice_to(1);
    py::dict<py::str, int64_t> segat;

    auto place = [&](const py::str& seg, int64_t base) -> int64_t {
        int64_t at = base;
        bool first = true;
        for (const Module& m : mods) {
            PieceRef p = m.pieces.get(seg);
            if (!p) continue;
            at = py::floordiv(at + p->align - 1, p->align) * p->align;
            if (first) {
                segat.set(seg, at);
                first = false;
            }
            p->at = at;
            at += p->length;
        }
        if (first) segat.set(seg, at);
        return at;
    };
    int64_t at = 0;
    for (const py::str& sg : code) {
        at = place(sg, at);
    }
    int64_t end = at;
    int64_t dgroup = (end + 15) & ~static_cast<int64_t>(15);
    at = dgroup;
    py::list<py::str> rest;
    for (const py::str& sg : order) {
        if (!code.contains(sg)) rest.append(sg);
    }
    std::optional<py::str> comm_seg;
    for (const py::str& sg : rest) {
        if (sg.lower() == py::str("c_common")) {
            comm_seg = sg;
            break;
        }
    }
    py::list<py::str> before = comm_seg ? rest.slice_to(rest.index(*comm_seg)) : rest;
    for (const py::str& sg : before) {
        if (sg.upper() != py::str("STACK")) {
            at = place(sg, at);
        }
    }
    // the communal variables: in the order of their places in SYMBOLS, one after another
    py::dict<py::str, int64_t> common;
    py::set<py::str> names;
    py::dict<py::str, int64_t> gdef;
    py::dict<py::str, int64_t> known;
    for (const auto& [k, v] : symbols.items()) {
        known.set(k.lower(), std::get<1>(v));
    }
    for (const Module& m : mods) {
        for (const auto& [k, v] : m.common.items()) {
            common.set(k, py::max(common.get(k, 0), v));
        }
        for (const py::str& k : m.defined) {
            if (names.contains(k) && !k.startswith(py::str("l"))) {
                throw py::SystemExit(py::percent("%s is defined twice (%s)", k, m.name));
            }
            names.add(k);
        }
    }
    {
        py::dict<py::str, int64_t> kept;
        for (const auto& [k, v] : common.items()) {
            if (!names.contains(k)) kept.set(k, v);
        }
        common = kept;
    }
    py::list<py::str> missing;
    for (const py::str& k : common) {
        if (!known.contains(k)) missing.append(k);
    }
    if (!missing.empty()) {
        throw py::SystemExit(py::str("communal variables SYMBOLS has no place for: ") + py::join(py::str(" "), py::sorted(missing)));
    }
    if (comm_seg) {
        PieceRef p;
        bool found = false;
        for (const Module& m : mods) {
            if (m.pieces.contains(*comm_seg)) {
                p = m.pieces.at(*comm_seg);
                found = true;
                break;
            }
        }
        if (!found) throw py::StopIteration();
        at = py::floordiv(at + p->align - 1, p->align) * p->align;
        segat.set(*comm_seg, at);
        p->at = at;
        std::vector<py::str> ks;
        for (const py::str& k : common) ks.push_back(k);
        std::stable_sort(ks.begin(), ks.end(), [&](const py::str& a, const py::str& b) { return known.at(a) < known.at(b); });
        for (const py::str& k : ks) {
            if (known.at(k) != at - dgroup) {
                say(py::percent("  %s: SYMBOLS has it at %04x; the variables before it end at %04x", k, known.at(k), at - dgroup));
            }
            gdef.set(k, at - dgroup);
            at += (common.at(k) + 1) & ~static_cast<int64_t>(1);
        }
        for (const py::str& sg : rest.slice_from(rest.index(*comm_seg) + 1)) {
            if (sg.upper() != py::str("STACK")) {
                at = place(sg, at);
            }
        }
    }
    std::optional<py::str> stseg;
    for (const py::str& sg : rest) {
        if (sg.upper() == py::str("STACK")) {
            stseg = sg;
            break;
        }
    }
    if (stseg) {
        at = place(*stseg, at);
    }
    int64_t top = at;
    for (const Module& m : mods) {
        for (const auto& [k, so] : m.defined.items()) {
            const py::str& seg = std::get<0>(so);
            gdef.set(k, at_of(*m.pieces.at(seg)) + std::get<1>(so) - (!code.contains(seg) ? dgroup : 0));
        }
    }
    PieceRef stack;
    for (const Module& m : mods) {
        for (const py::str& sg : m.pieces) {
            if (!stack && m.pieces.at(sg)->stack) stack = m.pieces.at(sg);
        }
    }
    // the image goes as far as the last segment that has data in it does - to its end
    py::set<py::str> withdata;
    for (const Module& m : mods) {
        for (const PieceRef& p : m.pieces.values()) {
            if (p->hasdata && p->length) withdata.add(p->seg);
        }
    }
    std::optional<int64_t> lastdata;
    for (const Module& m : mods) {
        for (const PieceRef& p : m.pieces.values()) {
            if (withdata.contains(p->seg) && p->at) {
                int64_t v = *p->at + p->length;
                if (!lastdata || v > *lastdata) lastdata = v;
            }
        }
    }
    if (!lastdata) throw py::ValueError("max() arg is an empty sequence");
    py::bytearray image = py::bytearray::zeros(*lastdata);
    py::list<int64_t> relocs;
    std::optional<int64_t> entry;
    auto frame = [&](const py::str& seg) -> int64_t { return !code.contains(seg) ? dgroup : 0; };
    for (const Module& m : mods) {
        for (const PieceRef& p : m.pieces.values()) {
            if (p->hasdata) {
                int64_t pa = at_of(*p);
                image.set_slice(pa, pa + py::len(p->data), p->data);
            }
        }
        if (m.start) {
            entry = at_of(*m.pieces.at(std::get<0>(*m.start))) + std::get<1>(*m.start);
        }
        for (const Fix& fx : m.fixes) {
            const py::str& seg = std::get<0>(fx);
            int64_t off = std::get<1>(fx);
            const py::str& how = std::get<2>(fx);
            const Target& tg = std::get<3>(fx);
            int64_t add = std::get<4>(fx);
            int64_t loc = at_of(*m.pieces.at(seg)) + off;
            int64_t v;
            if (std::get<0>(tg) == py::str("sym")) {
                if (!gdef.contains(std::get<1>(tg))) {
                    throw py::SystemExit(py::percent("%s: %s is not defined", m.name, std::get<1>(tg)));
                }
                v = gdef.at(std::get<1>(tg));
            } else if (std::get<0>(tg) == py::str("seg")) {
                v = at_of(*m.pieces.at(std::get<1>(tg))) - frame(std::get<1>(tg));
            } else {
                v = 0;
            }
            if (how == py::str("seg")) {
                v = dgroup >> 4;
                relocs.append(loc);
            } else if (how == py::str("rel")) {
                v = v - (at_of(*m.pieces.at(seg)) - frame(seg)) + add;
            } else {
                v = v + add;
            }
            py::pack_into("<H", image, loc, v & 0xFFFF);
        }
    }
    auto far = [&](int64_t r) -> std::tuple<int64_t, int64_t> {
        if (r < dgroup) return std::make_tuple(r, int64_t(0));
        return std::make_tuple(r - dgroup, dgroup >> 4);
    };
    int64_t cbrlc = 30 + 4 * py::len(relocs);
    int64_t hdrlen = (cbrlc + 511) & ~static_cast<int64_t>(511);
    int64_t size = hdrlen + py::len(image);
    int64_t e0 = entry ? *entry : 0;   // (`entry or 0`)
    int64_t stack_seg = stack ? (at_of(*stack) >> 4) : 0;
    int64_t stack_len = stack ? stack->length : 0;
    py::bytes hdr = py::pack("<15H", int64_t(0x5A4D), size & 511, (size + 511) >> 9, py::len(relocs), hdrlen >> 4,
                             (top - py::len(image) + 15) >> 4, int64_t(0xFFFF), stack_seg, stack_len, int64_t(0),
                             std::get<0>(far(e0)), std::get<1>(far(e0)), int64_t(30), int64_t(0), int64_t(1));
    py::bytearray exe(hdr);
    for (int64_t r : relocs) {
        auto [fo, fs] = far(r);
        exe += py::pack("<HH", fo, fs);
    }
    exe += py::bytes::zeros(hdrlen - py::len(exe));
    exe += image.view();
    {
        // ~sum(struct.unpack("<%dH", exe + bytes(len(exe) & 1))) & 0xFFFF
        // (an odd-length file: the format counts len // 2 words, the padded buffer holds one byte more -- struct.error)
        const std::string& raw = exe.raw();
        if (raw.size() & 1) {
            throw py::StructError("unpack requires a buffer of " + std::to_string(raw.size() - 1) + " bytes");
        }
        int64_t sum = 0;
        for (size_t i = 0; i + 1 < raw.size(); i += 2) {
            sum += static_cast<unsigned char>(raw[i]) | (static_cast<int64_t>(static_cast<unsigned char>(raw[i + 1])) << 8);
        }
        py::pack_into("<H", exe, 18, ~sum & 0xFFFF);
    }
    link24_mods = mods;   // (where each module's pieces went: a map)
    link24_dgroup = dgroup;
    say(py::percent("  the text ends at %05x, DGROUP is at %05x; the image is %04x bytes, the program ends at %05x", end, dgroup,
                    py::len(image), top));
    return py::bytes(exe.view());
}

}  // namespace codebreaker::exact::linkemu
