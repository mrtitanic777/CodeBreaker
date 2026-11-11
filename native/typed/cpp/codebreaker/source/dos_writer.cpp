// codebreaker/source/dos_writer.cpp -- typed C++ of codebreaker.source.dos_writer.

#include "codebreaker/source/dos_writer.hpp"

#include <capstone/capstone.h>

#include <algorithm>

#include "codebreaker/base/capstone_dll.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/exepack.hpp"
#include "codebreaker/build/module.hpp"
#include "codebreaker/build/mzfile.hpp"
#include "codebreaker/build/pklite.hpp"
#include "codebreaker/build/stubjoin.hpp"
#include "codebreaker/build/vxdfile.hpp"
#include "codebreaker/compare/image.hpp"
#include "codebreaker/source/driver_writer.hpp"
#include "codebreaker/source/image_writer.hpp"
#include "codebreaker/source/pe_writer.hpp"
#include "codebreaker/symbols/dos_interrupts.hpp"
#include "json.hpp"
#include "py_json.hpp"

namespace codebreaker::source::dos_writer {

namespace w = codebreaker::source::image_writer;
namespace l2 = codebreaker::translate::l2;
namespace pe_writer = codebreaker::source::pe_writer;
namespace dos_interrupts = codebreaker::symbols::dos_interrupts;
using Log = std::function<void(const py::str&)>;

const int64_t _MARK = 0x7AB3;   // stands in for a relocated segment word while decoding
const int64_t _MAX_ROUNDS = 200;

const py::str MAIN_DOC(R"DOC(python -m codebreaker dos-source <program> [--out DIR]

Write a real-mode program's assembly source from its image, build it and compare the build
with the original.)DOC");   // (a docstring is dedented by the compiler, as Python 3.13 does)

namespace {

// a // b (floor division)
int64_t fdiv_(int64_t a, int64_t b) {
    int64_t q = a / b;
    if ((a % b != 0) && ((a < 0) != (b < 0))) {
        q -= 1;
    }
    return q;
}

py::bytes read_file_(const py::str& path) {
    py::file f = py::open_rb(path);
    py::bytes b = f.read();
    f.close();
    return b;
}

void write_file_(const py::str& path, const py::bytes& data) {
    py::file f = py::open(path, py::str("wb"));
    f.write(data);
    f.close();
}

py::str sub_nonword_(const py::str& s) {
    return py::re::sub(py::str("\\W"), py::str("_"), s);
}

py::list<int64_t> sorted_(const py::set<int64_t>& s) {
    py::list<int64_t> out = py::list<int64_t>::from(s);
    out.sort();
    return out;
}

// the first line of `lines` ... [-1][:200]
py::str last_line_(const py::str& messages) {
    py::list<py::str> lines = py::splitlines(messages.strip());
    py::str last = py::truthy(lines) ? lines[-1] : py::str("failed");
    return last.slice(0, 200);
}

// ---- json.dump(..., indent=1), as the Python writes <stem>.build.json ----------------------------------

struct JNode {
    enum class Kind { Null, Bool, Int, Str, List, Obj };
    Kind kind = Kind::Null;
    bool b = false;
    int64_t i = 0;
    py::str s;
    std::vector<JNode> list;
    std::vector<std::pair<py::str, JNode>> obj;

    static JNode null() { return JNode(); }
    static JNode boolean(bool v) { JNode n; n.kind = Kind::Bool; n.b = v; return n; }
    static JNode integer(int64_t v) { JNode n; n.kind = Kind::Int; n.i = v; return n; }
    static JNode str(const py::str& v) { JNode n; n.kind = Kind::Str; n.s = v; return n; }
    static JNode array() { JNode n; n.kind = Kind::List; return n; }
    static JNode object() { JNode n; n.kind = Kind::Obj; return n; }
    JNode& add(const char* key, const JNode& v) { obj.emplace_back(py::str(key), v); return *this; }
};

void dump_(std::string& o, const JNode& n, int level) {
    switch (n.kind) {
        case JNode::Kind::Null: o += "null"; break;
        case JNode::Kind::Bool: o += n.b ? "true" : "false"; break;
        case JNode::Kind::Int: o += std::to_string(n.i); break;
        case JNode::Kind::Str: py::json::put(o, n.s); break;
        case JNode::Kind::List:
            if (n.list.empty()) {
                o += "[]";
                break;
            }
            o += "[";
            for (size_t k = 0; k < n.list.size(); ++k) {
                o += k ? ",\n" : "\n";
                o += std::string(static_cast<size_t>(level + 1), ' ');
                dump_(o, n.list[k], level + 1);
            }
            o += "\n" + std::string(static_cast<size_t>(level), ' ') + "]";
            break;
        case JNode::Kind::Obj:
            if (n.obj.empty()) {
                o += "{}";
                break;
            }
            o += "{";
            for (size_t k = 0; k < n.obj.size(); ++k) {
                o += k ? ",\n" : "\n";
                o += std::string(static_cast<size_t>(level + 1), ' ');
                py::json::put(o, n.obj[k].first);
                o += ": ";
                dump_(o, n.obj[k].second, level + 1);
            }
            o += "\n" + std::string(static_cast<size_t>(level), ' ') + "}";
            break;
    }
}

py::str dumps_(const JNode& n) {
    std::string o;
    dump_(o, n, 0);
    return py::str(o);
}

JNode facts_node_(const Facts& f) {
    JNode order = JNode::array();
    if (std::holds_alternative<py::list<int64_t>>(f.order)) {
        for (int64_t v : std::get<py::list<int64_t>>(f.order)) {
            order.list.push_back(JNode::integer(v));
        }
    } else {
        for (const auto& [a, b] : std::get<py::list<std::tuple<int64_t, int64_t>>>(f.order)) {
            JNode pair = JNode::array();
            pair.list.push_back(JNode::integer(a));
            pair.list.push_back(JNode::integer(b));
            order.list.push_back(pair);
        }
    }
    JNode n = JNode::object();
    n.add("file", JNode::str(f.file));
    n.add("packer", f.packer.has_value() ? JNode::str(*f.packer) : JNode::null());
    n.add("overlay", JNode::boolean(f.overlay));
    if (f.pklite_order) {
        n.add("order", order);
        n.add("min_alloc", JNode::integer(*f.min_alloc));
        return n;
    }
    n.add("min_alloc", JNode::integer(*f.min_alloc));
    n.add("full_last", JNode::boolean(*f.full_last));
    if (f.words.has_value()) {
        n.add("words", JNode::integer(*f.words));
    }
    n.add("order", order);
    return n;
}

// the facts as a JSON document holds them (the other way round)
Facts facts_from_(const py::V& j) {
    Facts f;
    f.file = py::str(j.at("file").str());
    const py::V& pk = j.get("packer");
    if (!pk.is_none()) {
        f.packer = py::str(pk.str());
    }
    f.overlay = py::truthy(j.get("overlay"));
    const py::V& ma = j.get("min_alloc");
    if (!ma.is_none()) {
        f.min_alloc = ma.as_int();
    }
    const py::V& fl = j.get("full_last");
    if (!fl.is_none()) {
        f.full_last = py::truthy(fl);
    }
    const py::V& wd = j.get("words");
    if (!wd.is_none()) {
        f.words = wd.as_int();
    }
    const py::V& od = j.at("order");
    if (f.packer.has_value() && *f.packer == py::str("EXEPACK")) {
        py::list<int64_t> ints;
        for (const py::V& v : od.seq()) {
            ints.append(v.as_int());
        }
        f.order = ints;
    } else {
        py::list<std::tuple<int64_t, int64_t>> pairs;
        for (const py::V& v : od.seq()) {
            pairs.append(std::make_tuple(v[0].as_int(), v[1].as_int()));
        }
        f.order = pairs;
    }
    return f;
}

std::optional<dos::Mz> need_mz_(const std::optional<dos::Mz>& h) {
    if (!h.has_value()) {
        throw py::TypeError("'NoneType' object is not subscriptable");
    }
    return h;
}

}  // namespace

// ---- Writer -----------------------------------------------------------------------------------------------

Writer::Writer(const dos::Program& program) : p(program), analysis(dos_functions::discover(program)) {
    img = p.image;
    origin = p.kind == py::str("com") ? 0x100 : 0;   // a .COM file sits behind its program prefix
    for (int64_t a : p.relocations) {
        rel.add(a - p.base);
    }
    int64_t load = dos::LOAD;
    py::set<int64_t> segs;
    for (int64_t a : rel) {
        if (a + 2 <= py::len(img)) {
            segs.add((py::unpack_from1<uint16_t>("<H", img, a) - load) & 0xFFFF);
        }
    }
    // a program with a header of its own (a driver may be linked as one too), or bytes as they load
    flat = !(p.kind != py::str("com") && py::truthy(p.spans) && std::get<2>(p.spans[0]) == py::str("program"));
    if (!flat) {
        segs.add((p.cs - load) & 0xFFFF);
        if (p.sp && ((p.ss - load) & 0xFFFF) < fdiv_(py::len(img) + 15, 16)) {
            stack_in = (p.ss - load) & 0xFFFF;
            segs.add(*stack_in);
        }
    }
    top = fdiv_(py::len(img) + 15, 16);
    py::set<int64_t> below;
    for (int64_t s : segs) {
        if (s < top) {
            below.add(s);
        }
    }
    below.add(0);
    segments = sorted_(below);
    for (const py::ref<dos_functions::Function>& f : analysis.functions.values()) {
        names.set(f->address - p.base, f->name);
    }
    if (!flat) {
        entry = (((p.cs - load) & 0xFFFF) << 4) + p.ip;
    }
    for (int64_t k : names.keys()) {
        labels.add(k);
    }
    if (entry.has_value()) {
        labels.add(*entry);
    }
    for (const dos_functions::Interrupt& i : analysis.interrupts) {
        dos_interrupts::Service s = dos_interrupts::name(i.number, i.ah, i.al);
        calls.set(i.address - p.base, s.name + (py::truthy(s.arguments) && s.exact ? py::percent("(%s)", s.arguments) : py::str("")));
    }
    _decode();
    _divide();
}

void Writer::_divide() {
    int64_t img_end = py::len(img);
    std::vector<std::pair<int64_t, int64_t>> reach;
    for (const auto& [at, t] : ins.items()) {
        if (t.target.has_value()) {
            reach.emplace_back(std::min(at, *t.target), std::max(at, *t.target));
        }
    }
    py::set<int64_t> inside;
    for (const auto& [at, t] : ins.items()) {
        for (int64_t x = at + 1; x < at + py::len(t.data); ++x) {
            inside.add(x);
        }
    }
    bool again = true;
    while (again) {
        again = false;
        for (int64_t k = 0; k < py::len(segments); ++k) {
            int64_t s = segments[k];
            int64_t lo = s * 16;
            int64_t hi = k + 1 < py::len(segments) ? segments[k + 1] * 16 : img_end;
            if (hi - lo <= 0xFFF0) {
                continue;
            }
            std::optional<int64_t> cut, fewest;
            for (int64_t b = (lo + 0xFFF0) & ~static_cast<int64_t>(15); b > lo + 0x1000; b -= 16) {
                if (inside.contains(b) || rel.contains(b) || rel.contains(b - 1)) {
                    continue;
                }
                int64_t n = 0;
                for (const auto& [a, z] : reach) {
                    if (a < b && b <= z && lo <= a && z < hi) {
                        n += 1;
                    }
                }
                if (!fewest.has_value() || n < *fewest) {
                    cut = b;
                    fewest = n;
                }
                if (!n) {
                    break;
                }
            }
            if (!cut.has_value()) {
                return;   // it cannot be divided: left as it is
            }
            if (*fewest) {
                // every place is jumped across (the DOS kernel's code runs in a segment of its
                // own at run time): the few jumps across the place chosen are written as bytes
                for (const auto& [at, t] : ins.items()) {
                    if (t.target.has_value() && std::min(at, *t.target) < *cut && *cut <= std::max(at, *t.target)) {
                        Ins u = t;
                        u.target = std::nullopt;
                        ins.set(at, u);
                        crossing.add(at);
                    }
                }
                reach.clear();
                for (const auto& [at, t] : ins.items()) {
                    if (t.target.has_value()) {
                        reach.emplace_back(std::min(at, *t.target), std::max(at, *t.target));
                    }
                }
            }
            py::set<int64_t> next = py::set<int64_t>::from(segments);
            next.add(*cut / 16);
            segments = sorted_(next);
            again = true;
            break;
        }
    }
}

int64_t Writer::segment_of(int64_t at) const {
    int64_t best = 0;
    for (int64_t s : segments) {
        if (s * 16 <= at) {
            best = s;
        }
    }
    return best;
}

py::str Writer::label(int64_t at) const {
    std::optional<py::str> n = names.get(at);
    if (n.has_value() && py::truthy(*n)) {
        return *n;
    }
    return py::percent("L_%05X", at);
}

void Writer::_decode() {
    int64_t n_img = py::len(img);
    py::bytes marks = analysis._mark_bytes();
    for (int64_t at = 0; at < n_img; ++at) {
        if (marks[at] != 1) {
            continue;
        }
        std::string window = img.slice(at, at + 16).raw();
        std::optional<int64_t> where;
        for (int64_t x = at; x < std::min(at + 15, n_img); ++x) {
            if (rel.contains(x)) {
                int64_t k = x - at;
                if (k + 2 > static_cast<int64_t>(window.size())) {
                    throw py::StructError(py::str(py::percent("pack_into requires a buffer of at least %d bytes", k + 2)).utf8());
                }
                window[static_cast<size_t>(k)] = static_cast<char>(_MARK & 0xFF);
                window[static_cast<size_t>(k) + 1] = static_cast<char>((_MARK >> 8) & 0xFF);
                where = x;
                break;
            }
        }
        std::optional<pe_writer::Insn> one = pe_writer::decode(16, py::bytes(window), at & 0xFFFF);
        if (!one.has_value() || at + one->size > n_img) {
            continue;
        }
        if (where.has_value() && !(at <= *where && *where + 2 <= at + one->size)) {
            where = std::nullopt;
        }
        bool more = false;
        for (int64_t x = at; x < at + one->size; ++x) {
            if (x != where && rel.contains(x)) {
                more = true;
                break;
            }
        }
        if (more) {
            continue;   // more than one relocated word: written as bytes
        }
        std::optional<int64_t> target;
        bool jump_or_call = one->groups.contains(X86_GRP_JUMP) || one->groups.contains(X86_GRP_CALL);
        if (jump_or_call && one->op_count == 1 && one->op0_type == X86_OP_IMM && !where.has_value() && one->bytes[0] != 0x9A &&
            one->bytes[0] != 0xEA) {
            int64_t delta = (one->op0_imm - ((at + one->size) & 0xFFFF)) & 0xFFFF;
            if (delta & 0x8000) {
                delta -= 0x10000;
            }
            int64_t t = at + one->size + delta;
            if (0 <= t && t < n_img && segment_of(t) == segment_of(at)) {
                target = t;
                labels.add(t);
            }
        }
        Ins rec;
        rec.data = img.slice(at, at + one->size);
        rec.mnemonic = one->mnemonic;
        rec.ops = one->op_str;
        rec.target = target;
        rec.where = where;
        ins.set(at, rec);
    }
    // an instruction something jumps into the middle of is not written as one
    for (int64_t at : sorted_(py::set<int64_t>::from(ins.keys()))) {
        int64_t n = py::len(ins.at(at).data);
        for (int64_t x = at + 1; x < at + n; ++x) {
            if (labels.contains(x)) {
                ins.pop(at);
                break;
            }
        }
    }
    bool again = true;
    while (again) {
        again = false;
        py::set<int64_t> inside;
        for (int64_t at : ins.keys()) {
            for (int64_t x = at + 1; x < at + py::len(ins.at(at).data); ++x) {
                inside.add(x);
            }
        }
        for (int64_t at : ins.keys()) {
            const std::optional<int64_t>& t = ins.at(at).target;
            if (t.has_value() && inside.contains(*t)) {
                ins.pop(at);
                again = true;
            }
        }
    }
}

py::str Writer::seg_name(int64_t value) const {
    int64_t base = 0;
    bool any = false;
    for (int64_t s : segments) {
        if (s <= value) {
            base = any ? std::max(base, s) : s;
            any = true;
        }
    }
    if (!any) {
        base = 0;
    }
    return py::percent("seg S%04X%s", base, value != base ? py::percent(" + %d", value - base) : py::str(""));
}

py::str Writer::_seg_at(int64_t at) const {
    return seg_name((py::unpack_from1<uint16_t>("<H", img, at) - dos::LOAD) & 0xFFFF);
}

std::optional<py::str> Writer::mnemonic(int64_t at) const {
    static const py::re::Pattern string_ops = py::re::compile(assembly::STRING_OPS);
    static const py::re::Pattern bare = py::re::compile(py::str("(?<![a-z]:)\\[(0x[0-9a-f]+)\\]"));
    const Ins& i = ins.at(at);
    const py::str& mnem = i.mnemonic;
    if (crossing.contains(at)) {
        return std::nullopt;
    }
    std::optional<py::re::Match> m = string_ops.match(mnem + py::str(" "));
    if (m.has_value() && !i.where.has_value() && !i.ops.replace(py::str("es:[di]"), py::str("")).contains(py::str(":"))) {
        std::optional<py::str> g1 = m->group_opt(1);
        return ((g1.has_value() ? *g1 : py::str("")) + m->group(2) + m->group(3)).strip();
    }
    if (i.target.has_value()) {
        if (mnem == py::str("loop") || mnem == py::str("loope") || mnem == py::str("loopne") || mnem == py::str("jcxz") ||
            mnem == py::str("jecxz")) {
            return py::percent("%-6s %s", mnem, label(*i.target));
        }
        return py::percent("%-6s %s %s", mnem, py::str(py::len(i.data) == 2 ? "short" : "near ptr"), label(*i.target));
    }
    py::str o = i.ops;
    if (i.where.has_value()) {
        py::str marker = py::percent("0x%x", _MARK);
        if (!o.contains(marker)) {
            return std::nullopt;
        }
        o = o.replace(marker, _seg_at(*i.where));
    }
    // a bare number in brackets is an address in the data segment, which the assembler
    // reads as a number unless the segment is said
    o = bare.sub(py::str("ds:[\\1]"), o);
    return py::percent("%-6s %s", assembly::mnemonic(mnem), assembly::operands(o)).rstrip();
}

py::list<py::str> Writer::_raw(int64_t at) const {
    const Ins& i = ins.at(at);
    py::list<py::str> lines;
    std::vector<int64_t> run;
    auto flush = [&]() {
        if (!run.empty()) {
            std::vector<py::str> parts;
            for (int64_t b : run) {
                parts.push_back(py::percent("0%02Xh", b));
            }
            lines.append(py::str("        db   ") + py::join(py::str(", "), parts));
            run.clear();
        }
    };
    int64_t x = at;
    while (x < at + py::len(i.data)) {
        if (rel.contains(x)) {
            flush();
            lines.append(py::str("        dw   ") + _seg_at(x));
            x += 2;
        } else {
            run.push_back(i.data[x - at]);
            x += 1;
        }
    }
    flush();
    std::optional<py::str> m = mnemonic(at);
    py::str text = m.has_value() ? *m : py::percent("%s %s", i.mnemonic, i.ops).strip();
    if (py::truthy(lines)) {
        lines[0] = py::percent("%-44s ; %s", lines[0], text);
    }
    return lines;
}

std::tuple<py::str, Writer::Counts> Writer::source(const std::optional<py::set<int64_t>>& good) {
    w::Assembly out(py::list<py::str>{py::percent("; %s -- reconstructed from the program image by CodeBreaker.", p.name),
                                      py::str(";"),
                                      py::str("; One segment for each segment the program names. A word holding a segment is written as"),
                                      py::str("; that segment, so the program can be loaded anywhere; offsets inside a segment are the"),
                                      py::str("; original's numbers. Labels are L_<position in the load module>. Instructions written as"),
                                      py::str("; bytes are ones the assembler would encode differently; their mnemonic is in the comment."),
                                      py::str(""),
                                      py::str(".386p"),
                                      py::str("")},
                    py::str("masm"), p.name);   // (the file as one L2 tree, as it is written)
    Counts counts;
    int64_t n_img = py::len(img);
    int64_t done = 0;   // how far the segments before have got
    for (int64_t k = 0; k < py::len(segments); ++k) {
        int64_t s = segments[k];
        int64_t lo = s * 16;
        int64_t hi = k + 1 < py::len(segments) ? segments[k + 1] * 16 : n_img;
        // The original's linker packed segments end to end, so the last instruction of one
        // may run a few bytes into the paragraph the next is named by. That next segment then
        // starts where the one before stopped (byte-aligned), and its offsets count from its
        // paragraph all the same.
        int64_t over = std::max(done, lo) - lo;
        {
            l2::Dict origin;
            origin.set(py::str("module"), l2::Val(p.name));
            origin.set(py::str("segment"), l2::Val(s));
            origin.set(py::str("offset"), l2::Val(static_cast<int64_t>(0)));
            out.part(py::percent("S%04X", s), py::str("segment"), origin, hi - lo);
        }
        const char* align = over ? "byte" : "para";
        if (stack_in.has_value() && s == *stack_in && hi == n_img) {   // the stack, in the program's last segment
            out.append(py::percent("S%04X segment %s stack use16 'STACK'", s, py::str(align)));
        } else {
            out.append(py::percent("S%04X segment %s public use16 'C%04X'", s, py::str(align), s));
        }
        out.append(py::percent("        assume cs:S%04X, ds:nothing, es:nothing, ss:nothing", s));
        if (origin && s == 0) {
            out.append(py::percent("        org  %d", origin));
        }
        int64_t at = std::max(lo + over, s == 0 ? origin : static_cast<int64_t>(0));   // (DOS supplies what is before a .COM file's origin)
        std::string run;
        auto flush = [&]() {
            out.extend(assembly::data_lines(py::bytes(run)));
            run.clear();
        };
        while (at < hi) {
            if (labels.contains(at)) {
                flush();
                out.append(py::percent("%s:", label(at)));
            }
            if (ins.contains(at)) {
                flush();
                std::optional<py::str> text = (!good.has_value() || good->contains(at)) ? mnemonic(at) : std::nullopt;
                counts.instructions += 1;
                std::optional<py::str> note = calls.get(at);
                if (!text.has_value()) {
                    counts.raw += 1;
                    out.extend(_raw(at));
                } else {
                    out.append(py::str("        ") + *text + (note.has_value() && py::truthy(*note) ? py::str("    ; ") + *note : py::str("")));
                }
                at += py::len(ins.at(at).data);
            } else if (rel.contains(at)) {
                flush();
                out.append(py::str("        dw   ") + _seg_at(at));
                at += 2;
            } else {
                run.push_back(static_cast<char>(img[at]));
                counts.data += 1;
                at += 1;
            }
        }
        flush();
        done = at;
        if (labels.contains(hi) && hi == n_img) {
            out.append(py::percent("%s:", label(hi)));
        }
        // (the stack's room behind the file's bytes is not written: a linker puts it in the
        // file as zeros; it is memory the header asks for -- the stack is set there after linking)
        out.append(py::percent("S%04X ends", s));
        out.append(py::str(""));
    }
    out.part(py::str("end"), py::str("end"));
    if (!flat) {
        int64_t stack = (p.ss - dos::LOAD) & 0xFFFF;
        if (stack >= top) {   // the stack lies behind the load module
            if (stack > top) {
                out.append(py::str("GAP segment para public use16 'GAP'"));
                out.append(py::percent("        db   %d dup (?)", (stack - top) * 16));
                out.append(py::str("GAP ends"));
            }
            out.append(py::str("STK segment para stack use16 'STACK'"));
            out.append(py::percent("        db   %d dup (?)", p.sp));
            out.append(py::str("STK ends"));
            out.append(py::str(""));
        }
        // (an entry point outside the load module -- COMMAND.COM's is in its program prefix --
        // has no label to name; it is set in the file's header after linking)
        out.append(0 <= *entry && *entry < n_img ? py::percent("end %s", label(*entry)) : py::str("end"));
    } else if (p.kind == py::str("com") && labels.contains(origin)) {
        out.append(py::percent("end %s", label(origin)));   // where it starts: the linker leaves out what is before
    } else {
        out.append(py::str("end"));
    }
    l2 = out.document();
    return std::make_tuple(w::text(out) + py::str("\n"), counts);
}

py::list<assembly::Piece<int64_t>> Writer::pieces(const py::set<int64_t>& good) const {
    py::list<assembly::Piece<int64_t>> out;
    int64_t at = origin, start = origin, n = py::len(img);
    auto data = [&](int64_t end) {
        if (end > start) {
            out.append(std::make_tuple(start - origin, end - start, std::optional<py::bytes>(), std::optional<int64_t>(), false));
        }
    };
    while (at < n) {
        if (ins.contains(at)) {
            const Ins& i = ins.at(at);
            int64_t size = py::len(i.data);
            data(at);
            std::optional<py::bytes> mask;
            if (i.where.has_value()) {
                std::string m(static_cast<size_t>(size), '\0');
                m[static_cast<size_t>(*i.where - at)] = 1;
                m[static_cast<size_t>(*i.where - at) + 1] = 1;
                mask = py::bytes(m);
            }
            out.append(std::make_tuple(at - origin, size, mask, good.contains(at) ? std::optional<int64_t>(at) : std::nullopt,
                                       i.target.has_value()));
            at += size;
            start = at;
        } else if (rel.contains(at)) {
            data(at);
            out.append(std::make_tuple(at - origin, static_cast<int64_t>(2), std::optional<py::bytes>(py::bytes("\1\1", 2)),
                                       std::optional<int64_t>(), false));
            at += 2;
            start = at;
        } else {
            at += 1;
        }
    }
    data(n);
    return out;
}

py::dict<int64_t, assembly::Candidate> Writer::candidates() const {
    py::dict<int64_t, assembly::Candidate> out;
    for (const auto& [at, i] : ins.items()) {
        if (i.target.has_value()) {
            continue;
        }
        std::optional<py::str> text = mnemonic(at);
        if (!text.has_value()) {
            continue;
        }
        std::string mask(static_cast<size_t>(py::len(i.data)), '\0');
        if (i.where.has_value()) {
            mask[static_cast<size_t>(*i.where - at)] = 1;
            mask[static_cast<size_t>(*i.where - at) + 1] = 1;
        }
        std::string kept;
        for (int64_t k = 0; k < py::len(i.data); ++k) {
            if (!mask[static_cast<size_t>(k)]) {
                kept.push_back(static_cast<char>(i.data[k]));
            }
        }
        out.set(at, std::make_tuple(*text, py::bytes(kept), py::bytes(mask)));
    }
    return out;
}

py::list<py::str> Writer::link_lines(const py::str& obj) const {
    if (p.kind == py::str("com")) {
        return py::list<py::str>{py::str("format dos com"), py::str("option quiet, nodefaultlibs"), py::percent("file %s", obj)};
    }
    if (flat) {
        return py::list<py::str>{py::str("format raw bin"), py::str("option quiet, nodefaultlibs"), py::percent("file %s", obj)};
    }
    // the stack's size is said outright: the linker would give a small one 4096 bytes
    py::str stack = p.sp ? py::percent(", stack=%d", p.sp) : py::str("");
    return py::list<py::str>{py::str("format dos"), py::str("option quiet, nodefaultlibs, packcode=0, packdata=0") + stack,
                             py::percent("file %s", obj)};
}

// ---- results --------------------------------------------------------------------------------------------

py::str Result::source_text() const {
    if (std::holds_alternative<py::str>(source)) {
        return std::get<py::str>(source);
    }
    if (std::holds_alternative<py::list<py::str>>(source)) {
        std::vector<py::str> parts;
        for (const py::str& s : std::get<py::list<py::str>>(source)) {
            parts.push_back(py::repr(s));
        }
        return py::str("[") + py::join(py::str(", "), parts) + py::str("]");
    }
    return py::str("None");
}

namespace {

Part part_of_(const Result& r) {
    Part part;
    part.source = std::holds_alternative<py::str>(r.source) ? std::get<py::str>(r.source) : py::str("");
    part.image = r.image;
    part.linked = r.linked;
    part.same = r.same;
    part.rounds = r.rounds;
    part.instructions = r.instructions;
    part.raw = r.raw;
    part.data = r.data;
    part.notes = r.notes;
    return part;
}

Part part_of_(const driver_writer::Reconstruction& r) {
    Part part;
    part.source = r.source;
    part.image = r.image;
    part.linked = r.linked;
    part.same = r.same.has_value() && *r.same != 0.0;
    part.rounds = r.rounds;
    part.instructions = r.instructions;
    part.raw = r.raw;
    part.data = r.data;
    part.notes = r.notes;
    return part;
}

}  // namespace

std::tuple<py::bytes, py::set<int64_t>, std::optional<std::tuple<int64_t, int64_t, int64_t, int64_t>>> _load_module(const py::str& path,
                                                                                                                  bool kind) {
    if (kind) {
        // read plainly, as stored: the built file IS the program as it runs. (Read the way an
        // original is, a program that moves or unpacks itself at start would be undone twice.)
        py::bytes data = read_file_(path);
        dos::Mz h = *need_mz_(dos::_mz(data));
        std::string img = data.slice(h.header, h.size).raw();
        py::set<int64_t> relocs;
        for (int64_t i = 0; i < h.nreloc; ++i) {
            auto [off, seg] = py::unpack_from<int64_t, int64_t>("<HH", data, h.reloc_off + 4 * i);
            int64_t at = seg * 16 + off;
            if (at + 2 <= static_cast<int64_t>(img.size())) {
                relocs.add(at);
                int64_t v = (static_cast<unsigned char>(img[static_cast<size_t>(at)]) |
                             (static_cast<unsigned char>(img[static_cast<size_t>(at) + 1]) << 8)) + dos::LOAD;
                v &= 0xFFFF;
                img[static_cast<size_t>(at)] = static_cast<char>(v & 0xFF);
                img[static_cast<size_t>(at) + 1] = static_cast<char>(v >> 8);
            }
        }
        return std::make_tuple(py::bytes(img), relocs,
                               std::optional<std::tuple<int64_t, int64_t, int64_t, int64_t>>(std::make_tuple(
                                   (dos::LOAD + h.cs) & 0xFFFF, h.ip, (dos::LOAD + h.ss) & 0xFFFF, h.sp)));
    }
    return std::make_tuple(read_file_(path), py::set<int64_t>(), std::optional<std::tuple<int64_t, int64_t, int64_t, int64_t>>());
}

Result reconstruct(const py::str& module_path, const py::str& outdir, const Log& log) {
    std::optional<py::str> kind = dos::kind(module_path);
    if (kind.has_value() && *kind == py::str("kernel")) {
        return _kernel(module_path, outdir, log);
    }
    if (kind.has_value() && *kind == py::str("driver+vxd")) {
        return _driver_and_vxd(module_path, outdir, log);
    }
    if (kind.has_value() && *kind == py::str("dos+windows")) {
        return _dos_and_windows(module_path, outdir, log);
    }
    dos::Program p = dos::read(module_path, false);   // moving itself is the program's own code
    return _program(p, outdir, sub_nonword_(std::get<0>(py::os::path::splitext(p.name))), log);
}

Result _dos_and_windows(const py::str& module_path, const py::str& outdir, const Log& log) {
    py::bytes original = read_file_(module_path);
    std::optional<std::tuple<dos::Mz, int64_t>> hl = dos::dos_and_windows(original);
    if (!hl.has_value()) {
        throw py::TypeError("cannot unpack non-iterable NoneType object");
    }
    int64_t lfa = std::get<1>(*hl);
    dos::Program p = dos::read(module_path, false);
    py::str stem = sub_nonword_(std::get<0>(py::os::path::splitext(py::os::path::basename(module_path))).lower());
    Result result = _program(p, outdir, stem, log);
    result.kind = py::str("dos+windows");
    if (!result.same) {
        return result;
    }
    if (!result.file.has_value()) {
        throw py::KeyError("file");
    }
    py::bytes program = read_file_(*result.file);
    build::module::Built b = build::module::build(module_path, py::os::path::join(outdir, py::str("windows")), true, [](const py::str&) {});
    py::set<py::str> stand_set = py::set<py::str>::from(b.rejected.keys());
    stand_set.update(b.absent);
    py::list<py::str> stand_ins = py::sorted(stand_set);
    Windows win;
    win.linked = b.linked();
    win.functions = b.functions;
    win.stand_ins = stand_ins;
    win.undefined = b.undefined;
    win.imports_extra = b.imports_extra;
    win.ran = py::len(b.ran);
    for (const auto& r : b.ran) {
        if (std::get<3>(r)) {
            win.ran_same += 1;
        }
    }
    result.windows = win;
    py::str built = py::os::path::join(outdir, py::str("windows"), py::os::path::basename(module_path).lower());
    if (!b.linked() || !py::os::path::isfile(built)) {
        result.notes.append(py::percent("the Windows module did not build: %s", py::truthy(b.error) ? b.error : py::str("not linked")));
        return result;
    }
    py::bytes joined = build::stubjoin::join(program, read_file_(built), lfa);
    write_file_(*result.file, joined);
    result.file_same = joined == original;
    py::list<py::str> remarks;
    for (const py::str& r : result.remarks) {
        if (!r.startswith(py::str("the file differs"))) {
            remarks.append(r);
        }
    }
    result.remarks = remarks;
    if (!*result.file_same) {
        result.remarks.append(py::percent("the file is %d bytes, the original %d: the module is OpenWatcom's build of its reconstruction",
                                          py::len(joined), py::len(original)));
    }
    py::str extra;
    if (py::truthy(stand_ins)) {
        py::list<py::str> head;
        for (int64_t k = 0; k < std::min<int64_t>(6, py::len(stand_ins)); ++k) {
            head.append(stand_ins[k]);
        }
        extra = py::percent(", %d of them stand-ins: %s", py::len(stand_ins), py::join(py::str(", "), head));
    }
    result.remarks.append(py::percent("the Windows module (%d functions%s) put behind the DOS program", b.functions, extra));
    return result;
}

Result _driver_and_vxd(const py::str& module_path, const py::str& outdir, const Log& log) {
    py::bytes original = read_file_(module_path);
    std::optional<std::tuple<int64_t, int64_t>> hl = dos::driver_and_vxd(original);
    if (!hl.has_value()) {
        throw py::TypeError("cannot unpack non-iterable NoneType object");
    }
    auto [hdr, lfa] = *hl;
    py::str stem = sub_nonword_(std::get<0>(py::os::path::splitext(py::os::path::basename(module_path))).lower());
    Result result;
    result.source = py::list<py::str>();
    result.linked = true;
    result.kind = py::str("driver+vxd");
    py::os::makedirs(outdir, 0777, true);
    Result r = _program(dos::driver_part(module_path), outdir, stem, log);
    {   // (the Python's `import capstone` in the driver lane: here the DLL is named once, where the shared lookup finds it)
        static const bool named = (driver_writer::set_capstone_dll(base::capstone_dll::find()), true);
        (void)named;
    }
    driver_writer::Reconstruction v = driver_writer::reconstruct(module_path, py::os::path::join(outdir, py::str("vxd")), 0, log);
    const std::pair<py::str, Part> both[2] = {{stem, part_of_(r)}, {py::str("vxd"), part_of_(v)}};
    for (const auto& [name, part] : both) {
        result.parts.emplace_back(name, part);
        std::get<py::list<py::str>>(result.source).append(part.source);
        result.instructions += part.instructions;
        result.raw += part.raw;
        result.data += part.data;
        result.rounds = std::max(result.rounds, part.rounds);
        bool ok = part.same;
        if (log) {
            log(py::percent("  %s: %d instructions, %d as bytes; %s", name, part.instructions, part.raw,
                            ok ? py::str("builds back the same")
                               : py::truthy(part.notes) ? py::join(py::str("; "), part.notes) : py::str("differs")));
        }
        if (!part.linked) {
            result.linked = false;
            result.notes.append(py::percent("%s: %s", name, py::truthy(part.notes) ? py::join(py::str("; "), part.notes) : py::str("not built")));
        }
    }
    if (!result.linked) {
        return result;
    }
    if (!v.image.has_value()) {
        throw py::TypeError("expected str, bytes or os.PathLike object, not NoneType");
    }
    py::str vstem = std::get<0>(py::os::path::splitext(py::os::path::basename(*v.image)));
    py::bytes head = _beside(py::os::path::join(outdir, stem + py::str(".hdr")), [&]() { return original.slice(0, hdr); });
    auto [res_at, res_len] = py::unpack_from<int64_t, int64_t>("<II", original, lfa + 0xB8);
    py::bytes resource = _beside(py::os::path::join(outdir, py::str("vxd"), vstem + py::str(".ver")),
                                 [&]() { return original.slice(res_at, res_at + res_len); });
    py::bytes tail = _beside(py::os::path::join(outdir, stem + py::str(".tail")), [&]() { return original.slice(res_at + res_len, py::len(original)); });
    if (!r.image.has_value()) {
        throw py::TypeError("expected str, bytes or os.PathLike object, not NoneType");
    }
    py::bytes driver = read_file_(*r.image);
    py::bytes built_vxd = read_file_(*v.image);
    int64_t at = py::len(head) + py::len(driver);
    std::string body = head.raw() + driver.raw() + build::vxdfile::relay(built_vxd, at, resource).raw() + tail.raw();
    if (body.size() < 0x40) {
        throw py::StructError("pack_into requires a buffer of at least 64 bytes");
    }
    for (int k = 0; k < 4; ++k) {   // struct.pack_into("<I", body, 0x3C, at)
        body[0x3C + static_cast<size_t>(k)] = static_cast<char>((at >> (8 * k)) & 0xFF);
    }
    int64_t total = static_cast<int64_t>(body.size());
    int64_t last = total % 512, pages = (total + 511) / 512;   // struct.pack_into("<HH", body, 2, ...)
    body[2] = static_cast<char>(last & 0xFF);
    body[3] = static_cast<char>((last >> 8) & 0xFF);
    body[4] = static_cast<char>(pages & 0xFF);
    body[5] = static_cast<char>((pages >> 8) & 0xFF);
    py::os::makedirs(py::os::path::join(outdir, py::str("file")), 0777, true);
    py::str out = py::os::path::join(outdir, py::str("file"), py::os::path::basename(module_path).upper());
    write_file_(out, py::bytes(body));
    result.image = out;
    result.file = out;
    result.same = py::bytes(body) == original;
    if (!result.same) {
        int64_t first = *compare::image::first_difference(py::bytes(body), original);
        result.notes.append(py::percent("the file differs from the original at %#x (%d bytes, original %d)", first, total, py::len(original)));
        result.equivalent = r.same && v.same.has_value() && *v.same != 0.0;
        if (*result.equivalent) {
            result.notes.append(py::str("the driver is the original's byte for byte and the VxD the same module (objects, "
                                        "descriptor, services, exports); laid out by OpenWatcom's linker, its tables differ"));
        }
    }
    return result;
}

py::str _fills_text(const build::exepack::Layout& layout) {
    py::list<py::str> parts{py::str("; the stretches of SYSINIT that LINK /EXEPACK stored as fills: its iterated data\n"),
                            py::str("; (`dup`s), as offset length byte, in hex\n")};
    for (const auto& [offset, length, byte] : layout.fills) {
        parts.append(py::percent("%05X %04X %02X\n", offset, length, byte));
    }
    return py::join(py::str(""), parts);
}

py::list<build::exepack::Fill> _read_fills(const py::str& path) {
    py::file f = py::open(path);
    py::str text = f.read_text();
    f.close();
    py::list<build::exepack::Fill> out;
    for (const py::str& line : py::splitlines(text)) {
        if (py::truthy(line.strip()) && !line.startswith(py::str(";"))) {
            py::list<py::str> words = py::split(line);
            out.append(std::make_tuple(py::int_parse(words[0], 16), py::int_parse(words[1], 16), py::int_parse(words[2], 16)));
        }
    }
    return out;
}

Result _kernel(const py::str& module_path, const py::str& outdir, const Log& log) {
    py::bytes original = read_file_(module_path);
    Result result;
    result.source = py::list<py::str>();
    result.linked = true;
    result.kind = py::str("kernel");
    py::os::makedirs(outdir, 0777, true);
    auto [parts, layout] = dos::kernel_parts(module_path);
    py::str fills = py::os::path::join(outdir, py::str("sysinit.pack"));
    if (!py::os::path::isfile(fills)) {
        py::file f = py::open(fills, py::str("w"));
        f.write(_fills_text(layout));
        f.close();
    }
    py::dict<py::str, py::str> built;
    for (const auto& [stem, p] : parts) {
        Result r = _program(p, outdir, stem, log);
        result.parts.emplace_back(stem, part_of_(r));
        std::get<py::list<py::str>>(result.source).append(std::holds_alternative<py::str>(r.source) ? std::get<py::str>(r.source) : py::str(""));
        result.instructions += r.instructions;
        result.raw += r.raw;
        result.data += r.data;
        result.rounds = std::max(result.rounds, r.rounds);
        if (log) {
            log(py::percent("  %s: %d instructions, %d as bytes; %s", stem, r.instructions, r.raw,
                            r.same ? py::str("builds back the same")
                                   : py::truthy(r.notes) ? py::join(py::str("; "), r.notes) : py::str("differs")));
        }
        if (!r.linked) {
            result.linked = false;
            result.notes.append(py::percent("%s: %s", stem, py::truthy(r.notes) ? py::join(py::str("; "), r.notes) : py::str("not built")));
            continue;
        }
        if (r.image.has_value()) {
            built.set(stem, *r.image);
        }
    }
    if (!result.linked) {
        return result;
    }
    py::bytes stub_code = read_file_(built.at(py::str("exepack")));
    py::bytes exe = read_file_(built.at(py::str("sysinit")));
    dos::Mz h = *need_mz_(dos::_mz(exe));
    py::bytes image = exe.slice(h.header, h.size);
    py::list<std::tuple<int64_t, int64_t>> relocs;
    for (int64_t i = 0; i < h.nreloc; ++i) {
        relocs.append(py::unpack_from<int64_t, int64_t>("<HH", exe, h.reloc_off + 4 * i));
    }
    py::list<int64_t> sites;   // (the table in the order the original's lists it)
    for (const auto& [off, seg] : relocs) {
        sites.append(seg * 16 + off);
    }
    try {
        dos::Mz oh = *need_mz_(dos::_mz(original));
        py::list<int64_t> theirs = std::get<1>(dos::unpack_exepack(original.slice(oh.header, oh.size), oh.cs, oh.ip));
        if (py::sorted(theirs) == py::sorted(sites)) {
            sites = theirs;
        }
    } catch (const py::ValueError&) {
    } catch (const py::KeyError&) {
    } catch (const py::StructError&) {
    }
    auto [packed, regs] = build::exepack::pack(image, sites, std::make_tuple(h.cs, h.ip), std::make_tuple(h.ss, h.sp), _read_fills(fills), stub_code,
                                               layout.words);
    auto [cs, ip, _ss, _sp] = regs;
    py::bytes loader = read_file_(built.at(py::str("msload")));
    py::bytes kernel = read_file_(built.at(py::str("msdos")));
    // the header the loader's first bytes hold: what it says must be what was built
    py::bytes joined0 = py::bytes(loader.raw() + kernel.raw() + packed.raw());
    dos::Mz head = *need_mz_(dos::_mz(joined0));
    std::vector<std::tuple<py::str, int64_t, int64_t>> wrong;   // (name, header says, built) sorted by name
    const std::tuple<const char*, int64_t, int64_t> want[5] = {{"cs", head.cs, cs},
                                                               {"header", head.header, py::len(loader) + py::len(kernel)},
                                                               {"ip", head.ip, ip},
                                                               {"nreloc", head.nreloc, 0},
                                                               {"size", head.size, py::len(loader) + py::len(kernel) + py::len(packed)}};
    for (const auto& [k, a, b] : want) {
        if (a != b) {
            wrong.emplace_back(py::str(k), a, b);
        }
    }
    if (!wrong.empty()) {
        std::vector<py::str> msgs;
        for (const auto& [k, a, b] : wrong) {
            msgs.push_back(py::percent("%s %#x, built %#x", k, a, b));
        }
        result.notes.append(py::percent("the MZ header in msload does not describe what was built: %s", py::join(py::str(", "), msgs)));
    }
    py::bytes joined = joined0;
    {   // (what rebuild() joins, from the folder)
        JNode doc = JNode::object();
        doc.add("file", JNode::str(py::os::path::basename(module_path).upper()));
        doc.add("words", JNode::integer(layout.words));
        JNode order = JNode::array();
        for (int64_t v : sites) {
            order.list.push_back(JNode::integer(v));
        }
        doc.add("order", order);
        py::file f = py::open(py::os::path::join(outdir, py::str("kernel.build.json")), py::str("w"));
        f.write(dumps_(doc));
        f.close();
    }
    py::str out = py::os::path::join(outdir, py::os::path::basename(module_path).lower());
    write_file_(out, joined);
    result.image = out;
    result.same = joined == original && wrong.empty();
    if (!result.same) {
        int64_t n = std::min(py::len(joined), py::len(original));
        int64_t first = n;
        for (int64_t a = 0; a < n; ++a) {
            if (joined[a] != original[a]) {
                first = a;
                break;
            }
        }
        result.notes.append(py::percent("the joined file differs from the original at %#x (%d bytes, original %d)", first, py::len(joined),
                                        py::len(original)));
        // what the difference is: the same programs, relocation table in another order?
        dos::Mz oh = *need_mz_(dos::_mz(original));
        if (joined.slice(0, oh.header) == original.slice(0, oh.header) && py::len(joined) == py::len(original)) {
            dos::Unpacked mine = dos::unpack_exepack(packed, cs, ip);
            dos::Unpacked theirs = dos::unpack_exepack(original.slice(oh.header, oh.size), oh.cs, oh.ip);
            if (std::get<0>(mine) == std::get<0>(theirs) && std::get<2>(mine) == std::get<2>(theirs) &&
                std::get<3>(mine) == std::get<3>(theirs) && std::get<4>(mine) == std::get<4>(theirs) &&
                std::get<5>(mine) == std::get<5>(theirs) && py::sorted(std::get<1>(mine)) == py::sorted(std::get<1>(theirs))) {
                result.notes.append(py::str("loader, kernel and SYSINIT are the original's, byte for byte, unpacked; "
                                            "only the order of SYSINIT's relocation table differs (LINK listed them "
                                            "in its fixups' order)"));
                result.equivalent = true;
            }
        }
    }
    return result;
}

py::bytes _beside(const py::str& path, const std::function<py::bytes()>& make) {
    if (!py::os::path::isfile(path)) {
        write_file_(path, make());
    }
    return read_file_(path);
}

void _as_file(const dos::Program& p, const py::str& outdir, const py::str& stem, const py::str& linked, Result& result, const Log& log) {
    py::bytes data = read_file_(p.path);
    py::bytes exe = read_file_(linked);
    py::os::makedirs(py::os::path::join(outdir, py::str("file")), 0777, true);
    py::str out = py::os::path::join(outdir, py::str("file"), py::os::path::basename(p.path).upper());
    if (p.packer.has_value() && *p.packer == py::str("PKLITE")) {
        dos::Mz oh = *need_mz_(dos::_mz(data, true));
        dos::Mz h = *need_mz_(dos::_mz(exe));
        _as_pklite_file(p, data, oh, exe, h, exe.slice(h.header, h.size), outdir, stem, out, result, log);
        return;
    }
    if (p.packer.has_value() && *p.packer != py::str("EXEPACK") && *p.packer != py::str("moves itself")) {
        // stored by a packer this does not write (PKLITE ...): the program as linked
        write_file_(out, py::bytes(exe.raw() + p.overlay.raw()));
        result.file = out;
        result.file_same = false;
        result.remarks.append(py::percent("the original is stored packed by %s; the file is the program unpacked", *p.packer));
        return;
    }
    Facts facts = _facts(p, data, outdir, stem, log);
    result.facts = facts;
    py::bytes body = _compose(outdir, stem, exe, facts, result);
    write_file_(out, body);
    result.file = out;
    result.file_same = body == data;
    if (!*result.file_same) {
        int64_t first = *compare::image::first_difference(body, data);
        result.remarks.append(py::percent("the file differs from the original's at %#x (%d bytes, original %d)", first, py::len(body), py::len(data)));
    }
}

Facts _facts(const dos::Program& p, const py::bytes& data, const py::str& outdir, const py::str& stem, const Log& log) {
    dos::Mz oh = *need_mz_(dos::_mz(data, true));   // (a Windows module's program stub too)
    _beside(py::os::path::join(outdir, stem + py::str(".hdr")), [&]() { return build::mzfile::template_(data.slice(0, oh.header)); });
    if (py::truthy(p.overlay)) {
        _beside(py::os::path::join(outdir, stem + py::str(".ovl")), [&]() { return p.overlay; });
    }
    Facts facts;
    facts.file = py::os::path::basename(p.path).upper();
    facts.packer = p.packer;
    facts.overlay = py::truthy(p.overlay);
    facts.min_alloc = oh.min_alloc;
    facts.full_last = py::unpack_from1<uint16_t>("<H", data, 2) == 512;
    if (p.packer.has_value() && *p.packer == py::str("EXEPACK")) {
        build::exepack::Layout layout = build::exepack::layout(data.slice(oh.header, oh.size), oh.cs, oh.ip);
        _beside(py::os::path::join(outdir, stem + py::str(".pack")), [&]() {
            return _fills_text(layout).replace(py::str("SYSINIT"), p.name).encode("utf-8");
        });
        py::str stub_dir = py::os::path::join(outdir, py::str("exepack"));
        if (!py::os::path::isfile(py::os::path::join(stub_dir, py::str("exepack.bin")))) {
            Result r = _program(dos::exepack_stub(p.path, layout), stub_dir, py::str("exepack"), log);
            if (!r.same) {
                throw py::ValueError("the EXEPACK unpacker does not build back");
            }
        }
        facts.words = layout.words;
        facts.order = std::get<1>(dos::unpack_exepack(data.slice(oh.header, oh.size), oh.cs, oh.ip));
    } else {
        py::list<std::tuple<int64_t, int64_t>> order;
        for (int64_t i = 0; i < oh.nreloc; ++i) {
            order.append(py::unpack_from<int64_t, int64_t>("<HH", data, oh.reloc_off + 4 * i));
        }
        facts.order = order;
    }
    return facts;
}

py::bytes _compose(const py::str& outdir, const py::str& stem, const py::bytes& exe, const Facts& facts, Result& result) {
    if (facts.packer.has_value() && *facts.packer == py::str("PKLITE")) {
        return _compose_pklite(outdir, stem, exe, facts, result);
    }
    dos::Mz h = *need_mz_(dos::_mz(exe));
    py::bytes module = exe.slice(h.header, h.size);
    py::list<std::tuple<int64_t, int64_t>> relocs;   // (offset, segment)
    for (int64_t i = 0; i < h.nreloc; ++i) {
        relocs.append(py::unpack_from<int64_t, int64_t>("<HH", exe, h.reloc_off + 4 * i));
    }
    // the memory the program needs beyond its load module: as far as its stack reaches (the
    // linker's own figure goes wrong -- FFFF -- for a stack that lies inside the file)
    int64_t beyond = std::max<int64_t>(0, fdiv_(h.ss * 16 + h.sp - py::len(module) + 15, 16));
    py::bytes tpl = read_file_(py::os::path::join(outdir, stem + py::str(".hdr")));
    py::bytes overlay;
    if (facts.overlay) {
        overlay = read_file_(py::os::path::join(outdir, stem + py::str(".ovl")));
    }
    py::bytes body;
    try {
        if (facts.packer.has_value() && *facts.packer == py::str("EXEPACK")) {
            py::bytes stub_code = read_file_(py::os::path::join(outdir, py::str("exepack"), py::str("exepack.bin")));
            py::list<int64_t> sites;
            for (const auto& [off, seg] : relocs) {
                sites.append(seg * 16 + off);
            }
            const py::list<int64_t>& theirs = std::get<py::list<int64_t>>(facts.order);
            if (py::sorted(theirs) == py::sorted(sites) && !(theirs == sites)) {
                sites = theirs;
                result.remarks.append(py::str("the relocation table is written in the original linker's order"));
            }
            auto [packed, regs] = build::exepack::pack(module, sites, std::make_tuple(h.cs, h.ip), std::make_tuple(h.ss, h.sp),
                                                       _read_fills(py::os::path::join(outdir, stem + py::str(".pack"))), stub_code, *facts.words);
            auto [cs, ip, ss, sp] = regs;
            // the memory it asks for beyond the stored module: the unpacked program and its stack
            // (EXEPACK counted both down to whole paragraphs), or the unpacker's copy and stack
            // behind it (counted from the stored module rounded up) -- whichever is more
            int64_t program = fdiv_(py::len(module) + 15, 16) + fdiv_(std::max<int64_t>(0, h.ss * 16 + h.sp - py::len(module)), 16) -
                              fdiv_(py::len(packed), 16);
            int64_t unpacker = ss + fdiv_(sp, 16) - fdiv_(py::len(packed) + 15, 16);
            int64_t memory = std::max(program, unpacker);
            if (py::abs(*facts.min_alloc - memory) == 1) {
                // (LINK rounded this count down for some programs and up for others -- CL.EXE of
                // MSC 6.0a one way, C1.EXE the other -- by a rule not yet found: the original's)
                result.remarks.append(py::percent("the memory it asks for is the original's (%d paragraphs; the rule gives %d)", *facts.min_alloc, memory));
                memory = *facts.min_alloc;
            }
            body = build::mzfile::compose(tpl, packed, py::list<std::tuple<int64_t, int64_t>>(), cs, ip, ss, sp, memory, facts.full_last.value_or(false));
        } else {
            // the relocation table as the original's linker wrote it -- each entry in the frame of
            // the segment it was recorded in, in the order of its records -- when it names the
            // same places as the build's; and the memory the original asks for beyond its stack
            // (a segment of no bytes behind it) when that is more than the stack needs
            const py::list<std::tuple<int64_t, int64_t>>& theirs = std::get<py::list<std::tuple<int64_t, int64_t>>>(facts.order);
            py::list<int64_t> mine_pos, their_pos;
            for (const auto& [off, seg] : relocs) {
                mine_pos.append(seg * 16 + off);
            }
            for (const auto& [off, seg] : theirs) {
                their_pos.append(seg * 16 + off);
            }
            if (py::sorted(mine_pos) == py::sorted(their_pos) && !(theirs == relocs)) {
                relocs = theirs;
                result.remarks.append(py::str("the relocation table is written in the original linker's frames and order"));
            }
            if (*facts.min_alloc > beyond) {
                result.remarks.append(py::percent("the original asks for %d bytes more memory than its stack needs", 16 * (*facts.min_alloc - beyond)));
                beyond = *facts.min_alloc;
            }
            body = build::mzfile::compose(tpl, module, relocs, h.cs, h.ip, h.ss, h.sp, beyond, facts.full_last.value_or(false));
        }
    } catch (const py::ValueError& e) {
        result.notes.append(py::percent("not laid out as the original file: %s", py::str(e.what())));
        body = exe;
    }
    return py::bytes(body.raw() + overlay.raw());
}

void _as_pklite_file(const dos::Program& p, const py::bytes& data, const dos::Mz& oh, const py::bytes& exe, const dos::Mz& h,
                     const py::bytes& module, const py::str& outdir, const py::str& stem, const py::str& out, Result& result, const Log& log) {
    (void)h;
    (void)module;
    (void)exe;
    _beside(py::os::path::join(outdir, stem + py::str(".hdr")), [&]() { return build::mzfile::template_(data.slice(0, oh.header)); });
    if (py::truthy(p.overlay)) {
        _beside(py::os::path::join(outdir, stem + py::str(".ovl")), [&]() { return p.overlay; });
    }
    py::str head_dir = py::os::path::join(outdir, py::str("pklite"));
    if (!py::os::path::isfile(py::os::path::join(head_dir, py::str("pklite.bin")))) {
        int64_t start = build::pklite::stream_start(data.slice(oh.header, oh.size));
        Result r = _program(dos::pklite_head(p.path, start), head_dir, py::str("pklite"), log);
        if (!r.same) {
            throw py::ValueError("PKLITE's decompressor does not build back");
        }
    }
    build::pklite::Stored stored = build::pklite::read(data.slice(oh.header, oh.size));
    Facts facts;
    facts.file = py::os::path::basename(p.path).upper();
    facts.packer = py::str("PKLITE");
    facts.overlay = py::truthy(p.overlay);
    py::list<std::tuple<int64_t, int64_t>> order;
    for (const auto& [seg, offs] : stored.relocs) {
        for (int64_t off : offs) {
            order.append(std::make_tuple(seg, off));
        }
    }
    facts.order = order;
    facts.min_alloc = oh.min_alloc;
    facts.pklite_order = true;
    result.facts = facts;
    py::bytes body = _compose(outdir, stem, exe, facts, result);
    write_file_(out, body);
    result.file = out;
    result.file_same = body == data;
    if (!*result.file_same) {
        int64_t first = *compare::image::first_difference(body, data);
        result.remarks.append(py::percent("the file differs from the original's at %#x (%d bytes, original %d)", first, py::len(body), py::len(data)));
    }
}

py::bytes _compose_pklite(const py::str& outdir, const py::str& stem, const py::bytes& exe, const Facts& facts, Result& result) {
    dos::Mz h = *need_mz_(dos::_mz(exe));
    py::bytes module = exe.slice(h.header, h.size);
    py::bytes tpl = read_file_(py::os::path::join(outdir, stem + py::str(".hdr")));
    py::bytes head = read_file_(py::os::path::join(outdir, py::str("pklite"), py::str("pklite.bin")));
    int64_t at = tpl.find(py::bytes("Reserved"));
    if (at < 0) {
        throw py::ValueError(py::percent("no copy of the program's header behind PKLITE's notice in %s.hdr", stem).utf8());
    }
    int64_t memory = py::unpack_from1<uint16_t>("<H", tpl, at + 8 + 4 + 8);
    py::list<std::tuple<int64_t, int64_t>> relocs;   // (segment, offset)
    for (int64_t i = 0; i < h.nreloc; ++i) {
        auto [off, seg] = py::unpack_from<int64_t, int64_t>("<HH", exe, h.reloc_off + 4 * i);
        relocs.append(std::make_tuple(seg, off));
    }
    const py::list<std::tuple<int64_t, int64_t>>& theirs = std::get<py::list<std::tuple<int64_t, int64_t>>>(facts.order);
    py::list<int64_t> mine_pos, their_pos;
    for (const auto& [seg, off] : relocs) {
        mine_pos.append(seg * 16 + off);
    }
    for (const auto& [seg, off] : theirs) {
        their_pos.append(seg * 16 + off);
    }
    if (py::sorted(mine_pos) == py::sorted(their_pos) && !(theirs == relocs)) {
        relocs = theirs;
        result.remarks.append(py::str("the relocation table is written in the original's frames and order"));
    }
    auto [packed, hd] = build::pklite::pack(module, relocs, std::make_tuple(h.cs, h.ip, h.ss, h.sp), head, memory);
    memory = hd.min_alloc;
    if (facts.min_alloc.has_value() && *facts.min_alloc != memory) {
        // (PKLITE's own figure for the memory it asks for, by a rule not yet found: the original's)
        result.remarks.append(py::percent("the memory it asks for is the original's (%d paragraphs; the rule gives %d)", *facts.min_alloc, memory));
        memory = *facts.min_alloc;
    }
    py::list<std::tuple<int64_t, int64_t>> hrel;
    for (const auto& [off, seg] : hd.relocs) {
        hrel.append(std::make_tuple(off, seg));
    }
    py::bytes body = build::mzfile::compose(tpl, packed, hrel, hd.cs, hd.ip, hd.ss, hd.sp, memory);
    py::bytes overlay;
    if (facts.overlay) {
        overlay = read_file_(py::os::path::join(outdir, stem + py::str(".ovl")));
    }
    return py::bytes(body.raw() + overlay.raw());
}

Result _program(const dos::Program& p, const py::str& outdir, const py::str& stem, const Log& log) {
    Result result;
    result.kind = p.kind;
    Writer wr(p);
    for (const py::ref<dos_functions::Function>& f : wr.analysis.functions.values()) {
        result.functions.set(f->address - p.base, f);
    }
    py::os::makedirs(outdir, 0777, true);
    py::str src = py::os::path::join(outdir, stem + py::str(".asm"));
    py::str out = py::os::path::join(outdir, stem + py::str(p.kind == py::str("com") ? ".com" : !wr.flat ? ".exe" : ".bin"));
    py::dict<int64_t, assembly::Candidate> cand = wr.candidates();
    py::set<py::str> seg_labels;
    for (int64_t s : wr.segments) {
        seg_labels.add(py::percent("S%04X", s));
    }
    py::list<assembly::Candidate> cand_list;
    for (const assembly::Candidate& c : cand.values()) {
        cand_list.append(c);
    }
    py::set<assembly::Candidate> ok = assembly::probe(cand_list, seg_labels, outdir, py::str("real16"));
    py::set<int64_t> good;
    for (const auto& [at, c] : cand.items()) {
        if (ok.contains(c)) {
            good.add(at);
        }
    }
    for (const auto& [at, t] : wr.ins.items()) {
        if (t.target.has_value()) {
            good.add(at);
        }
    }
    py::bytes original = wr.flat ? wr.img.slice(wr.origin, py::len(wr.img)) : wr.img;
    result.source = src;
    for (int64_t rounds = 1; rounds <= _MAX_ROUNDS; ++rounds) {
        auto [text, counts] = wr.source(good);
        {
            py::file f = py::open(src, py::str("w"), py::str("latin1"), std::nullopt, py::str("\r\n"));
            f.write(text);
            f.close();
        }
        result.rounds = rounds;
        result.instructions = counts.instructions;
        result.raw = counts.raw;
        result.data = counts.data;
        auto [obj, messages] = assembly::assemble(src, outdir);
        if (!obj.has_value()) {
            py::list<py::str> lines = py::split(text, py::str("\n"));
            static const py::re::Pattern error_line = py::re::compile(py::str("\\((\\d+)\\)\\s*:\\s*Error"));
            py::set<py::str> failed;
            for (const py::re::Match& m : error_line.finditer(messages)) {
                int64_t n = py::int_parse(m.group(1));
                if (n <= py::len(lines)) {
                    failed.add(py::split(lines[n - 1], py::str("    ;"))[0].strip());
                }
            }
            py::set<int64_t> drop;
            for (int64_t at : good) {
                std::optional<py::str> m = wr.mnemonic(at);
                if (m.has_value() && failed.contains(*m)) {
                    drop.add(at);
                }
            }
            // a jump to a place that is not the start of anything written (the middle of a word
            // that holds a segment, say) has no label to go to: it stays bytes
            py::set<py::str> lost = py::set<py::str>::from(py::re::findall(py::str("Symbol (\\w+) is not defined"), messages));
            for (int64_t at : good) {
                const std::optional<int64_t>& t = wr.ins.at(at).target;
                if (t.has_value() && lost.contains(wr.label(*t))) {
                    drop.add(at);
                }
            }
            if (!py::truthy(drop)) {
                result.notes = py::list<py::str>{py::str("assembler: ") + last_line_(messages)};
                break;
            }
            good = good - drop;
            continue;
        }
        auto [image, messages2] = assembly::link(wr.link_lines(*obj), out, outdir);
        if (!image.has_value()) {
            result.notes = py::list<py::str>{py::str("linker: ") + last_line_(messages2)};
            break;
        }
        result.image = image;
        result.linked = true;
        auto [built, relocs, regs] = _load_module(*image, !wr.flat);
        int64_t n = std::min(py::len(built), py::len(original));
        std::optional<int64_t> first;
        for (int64_t a = 0; a < n; ++a) {
            if (built[a] != original[a]) {
                first = a;
                break;
            }
        }
        if (!first.has_value() && py::len(built) == py::len(original)) {
            py::list<py::str> notes;
            if (!wr.flat) {
                if (!(relocs == wr.rel)) {
                    notes.append(py::percent("the relocation table differs (%d entries, original %d)", py::len(relocs), py::len(wr.rel)));
                }
                const std::tuple<int64_t, int64_t, int64_t, int64_t> want = std::make_tuple(p.cs, p.ip, p.ss, p.sp);
                if (regs != std::optional<std::tuple<int64_t, int64_t, int64_t, int64_t>>(want) && relocs == wr.rel) {
                    // Where a program starts and where its stack is are four words of the file's
                    // header. A linker fills them in from the source's entry label and stack
                    // segment, and cannot be told a stack that lies inside the program or has
                    // no size, or an entry before the program's first byte; the code and data
                    // being the original's, the four words are written as the original has them.
                    {
                        py::bytes img = read_file_(*image);
                        std::string raw = img.raw();
                        const int64_t words[4] = {(p.ss - dos::LOAD) & 0xFFFF, p.sp, p.ip, (p.cs - dos::LOAD) & 0xFFFF};
                        if (raw.size() >= 0x18) {
                            for (int k = 0; k < 2; ++k) {
                                raw[0x0E + 2 * static_cast<size_t>(k)] = static_cast<char>(words[k] & 0xFF);
                                raw[0x0E + 2 * static_cast<size_t>(k) + 1] = static_cast<char>((words[k] >> 8) & 0xFF);
                                raw[0x14 + 2 * static_cast<size_t>(k)] = static_cast<char>(words[2 + k] & 0xFF);
                                raw[0x14 + 2 * static_cast<size_t>(k) + 1] = static_cast<char>((words[2 + k] >> 8) & 0xFF);
                            }
                            write_file_(*image, py::bytes(raw));
                        }
                    }
                    regs = std::get<2>(_load_module(*image, true));
                    result.remarks.append(py::str("entry and stack set in the file's header after linking"));
                }
                if (regs != std::optional<std::tuple<int64_t, int64_t, int64_t, int64_t>>(want)) {
                    auto [rcs, rip, rss, rsp] = *regs;
                    notes.append(py::percent("entry or stack differ: %04x:%04x %04x:%04x, original %04x:%04x %04x:%04x", rcs, rip, rss, rsp, p.cs, p.ip,
                                             p.ss, p.sp));
                }
            }
            result.same = !py::truthy(notes);
            result.notes = notes;
            if (!wr.flat && result.same && !p.in_kernel) {
                _as_file(p, outdir, stem, *image, result, log);
            }
            if (result.same) {   // (SYSINIT of IO.SYS too: joined by _rebuild_kernel)
                bool patched = false;
                for (const py::str& i : result.remarks) {
                    if (i.contains(py::str("entry and stack set"))) {
                        patched = true;
                    }
                }
                JNode desc = JNode::object();
                desc.add("stem", JNode::str(stem));
                desc.add("product", JNode::str(py::os::path::basename(out)));
                desc.add("flat", JNode::boolean(wr.flat));
                if (patched) {
                    JNode regs_node = JNode::array();
                    for (int64_t v : {(p.ss - dos::LOAD) & 0xFFFF, p.sp, p.ip, (p.cs - dos::LOAD) & 0xFFFF}) {
                        regs_node.list.push_back(JNode::integer(v));
                    }
                    desc.add("regs", regs_node);
                } else {
                    desc.add("regs", JNode::null());
                }
                desc.add("facts", result.facts.has_value() ? facts_node_(*result.facts) : JNode::null());
                py::file f = py::open(py::os::path::join(outdir, stem + py::str(".build.json")), py::str("w"));
                f.write(dumps_(desc));
                f.close();
            }
            break;
        }
        // which instructions did not come back as they were -- found instruction by instruction,
        // so one that came back a byte longer is blamed itself and not everything behind it
        py::set<int64_t> blamed = assembly::culprits<int64_t>(original, built, wr.pieces(good)) & good;
        if (py::truthy(blamed)) {
            good = good - blamed;
            if (log) {
                log(py::percent("  round %d: %d instructions did not come back as they were; they go back to bytes", rounds, py::len(blamed)));
            }
            continue;
        }
        int64_t at = (first.has_value() ? *first : n) + wr.origin;
        std::vector<int64_t> before;
        for (int64_t a : good) {
            if (a <= at) {
                before.push_back(a);
            }
        }
        std::vector<int64_t> hit;
        for (int64_t a : before) {
            if (a <= at && at < a + py::len(wr.ins.at(a).data)) {
                hit.push_back(a);
            }
        }
        if (hit.empty() && !before.empty()) {
            hit.push_back(*std::max_element(before.begin(), before.end()));
        }
        if (hit.empty()) {
            result.notes = py::list<py::str>{py::percent("the built program differs at %#x and no instruction accounts for it", at)};
            break;
        }
        for (int64_t a : hit) {
            good.discard(a);
        }
        if (log) {
            std::optional<py::str> m = wr.mnemonic(hit[0]);
            log(py::percent("  round %d: differs at %#x; %s goes back to bytes", rounds, at, m.has_value() ? *m : py::str("None")));
        }
    }
    return result;
}

py::str _rebuild_kernel(const py::str& srcdir, const Log& log) {
    py::file jf = py::open(py::os::path::join(srcdir, py::str("kernel.build.json")));
    py::str text = jf.read_text();
    jf.close();
    py::V k = py::json_parse(text.utf8());
    py::dict<py::str, py::str> made;
    for (const char* stem : {"exepack", "msload", "msdos", "sysinit"}) {
        made.set(py::str(stem), rebuild(srcdir, log, py::str(stem) + py::str(".build.json")));
    }
    py::bytes stub_code = read_file_(made.at(py::str("exepack")));
    py::bytes exe = read_file_(made.at(py::str("sysinit")));
    dos::Mz h = *need_mz_(dos::_mz(exe));
    py::list<int64_t> order;
    for (const py::V& v : k.at("order").seq()) {
        order.append(v.as_int());
    }
    auto [packed, _regs] = build::exepack::pack(exe.slice(h.header, h.size), order, std::make_tuple(h.cs, h.ip), std::make_tuple(h.ss, h.sp),
                                                _read_fills(py::os::path::join(srcdir, py::str("sysinit.pack"))), stub_code, k.at("words").as_int());
    py::bytes loader = read_file_(made.at(py::str("msload")));
    py::bytes kernel = read_file_(made.at(py::str("msdos")));
    py::os::makedirs(py::os::path::join(srcdir, py::str("file")), 0777, true);
    py::str out = py::os::path::join(srcdir, py::str("file"), py::str(k.at("file").str()));
    write_file_(out, py::bytes(loader.raw() + kernel.raw() + packed.raw()));
    return out;
}

py::str rebuild(const py::str& srcdir_, const Log& log, const std::optional<py::str>& desc_name) {
    py::str srcdir = py::os::path::abspath(srcdir_);
    if (!desc_name.has_value() && py::os::path::isfile(py::os::path::join(srcdir, py::str("kernel.build.json")))) {
        return _rebuild_kernel(srcdir, log);
    }
    std::optional<py::str> desc_path;
    if (desc_name.has_value() && py::truthy(*desc_name)) {
        desc_path = py::os::path::join(srcdir, *desc_name);
    } else {
        py::list<py::str> names = py::sorted(py::os::listdir(srcdir));
        for (const py::str& f : names) {
            if (f.endswith(py::str(".build.json"))) {
                desc_path = py::os::path::join(srcdir, f);
                break;
            }
        }
    }
    if (!desc_path.has_value()) {
        throw py::SystemExit(py::percent("%s: no .build.json (write it with dos-source)", srcdir));
    }
    py::file jf = py::open(*desc_path);
    py::str text = jf.read_text();
    jf.close();
    py::V desc = py::json_parse(text.utf8());
    py::str stem = py::str(desc.at("stem").str());
    const py::V& facts_j = desc.get("facts");
    std::optional<Facts> facts;
    if (!facts_j.is_none() && py::truthy(facts_j)) {
        facts = facts_from_(facts_j);
    }
    if (facts.has_value() && facts->packer.has_value() && (*facts->packer == py::str("EXEPACK") || *facts->packer == py::str("PKLITE"))) {
        // (its unpacker first: a program of its own)
        rebuild(py::os::path::join(srcdir, facts->packer->lower()), log);
    }
    auto [obj, messages] = assembly::assemble(py::os::path::join(srcdir, stem + py::str(".asm")), srcdir);
    if (!obj.has_value()) {
        throw py::SystemExit(py::str("assembler: ") + messages.slice_from(py::len(messages) > 500 ? py::len(messages) - 500 : 0));
    }
    py::str product = py::str(desc.at("product").str());
    py::str out = py::os::path::join(srcdir, product);
    py::file lf = py::open(py::os::path::join(srcdir, std::get<0>(py::os::path::splitext(product)) + py::str(".lnk")));
    py::str lnk = lf.read_text();
    lf.close();
    py::list<py::str> lines;
    for (const py::str& l : py::splitlines(lnk)) {
        if (py::truthy(l.strip()) && !l.lower().startswith(py::str("name "))) {
            lines.append(l);
        }
    }
    auto [image, messages2] = assembly::link(lines, out, srcdir);
    if (!image.has_value()) {
        throw py::SystemExit(py::str("linker: ") + messages2.slice_from(py::len(messages2) > 500 ? py::len(messages2) - 500 : 0));
    }
    const py::V& regs = desc.get("regs");
    if (!regs.is_none() && py::truthy(regs)) {
        int64_t ss = regs[0].as_int(), sp = regs[1].as_int(), ip = regs[2].as_int(), cs = regs[3].as_int();
        std::string raw = read_file_(*image).raw();
        if (raw.size() >= 0x18) {
            const int64_t words[4] = {ss, sp, ip, cs};
            for (int k = 0; k < 2; ++k) {
                raw[0x0E + 2 * static_cast<size_t>(k)] = static_cast<char>(words[k] & 0xFF);
                raw[0x0E + 2 * static_cast<size_t>(k) + 1] = static_cast<char>((words[k] >> 8) & 0xFF);
                raw[0x14 + 2 * static_cast<size_t>(k)] = static_cast<char>(words[2 + k] & 0xFF);
                raw[0x14 + 2 * static_cast<size_t>(k) + 1] = static_cast<char>((words[2 + k] >> 8) & 0xFF);
            }
            write_file_(*image, py::bytes(raw));
        }
    }
    if (!facts.has_value()) {
        return *image;
    }
    py::bytes exe = read_file_(*image);
    Result scratch;
    py::bytes body = _compose(srcdir, stem, exe, *facts, scratch);
    py::os::makedirs(py::os::path::join(srcdir, py::str("file")), 0777, true);
    py::str made = py::os::path::join(srcdir, py::str("file"), facts->file);
    write_file_(made, body);
    return made;
}

int64_t main(const py::list<py::str>& argv) {
    std::vector<py::str> args;
    for (const py::str& a : argv) {
        if (!a.startswith(py::str("--"))) {
            args.push_back(a);
        }
    }
    if (args.empty()) {
        py::print(MAIN_DOC);
        return 2;
    }
    py::str path;
    if (py::os::path::isfile(args[0])) {
        path = args[0];
    } else {
        std::optional<py::str> found = base::corpus::retail(args[0]);
        if (!found.has_value() || !py::truthy(*found)) {
            throw py::SystemExit(py::percent("no program %r", args[0]));
        }
        path = *found;
    }
    py::str outdir;
    if (argv.contains(py::str("--out"))) {
        outdir = argv[argv.index(py::str("--out")) + 1];
    } else {
        py::str base = std::get<0>(py::os::path::splitext(py::os::path::basename(path))).lower();
        outdir = py::os::path::join(py::settings::get_str(py::str("state")), py::str("dos_source"), base);
        py::os::makedirs(py::os::path::dirname(outdir), 0777, true);
    }
    Result r = reconstruct(path, outdir, [](const py::str& s) { py::print(s); });
    int64_t n = std::max<int64_t>(1, r.instructions);
    py::print(py::percent("%s: %d instructions, %d (%d%%) written as bytes; %d bytes of data; %d round%s; %s", py::os::path::basename(path), r.instructions,
                          r.raw, 100 * r.raw / n, r.data, r.rounds, py::str(r.rounds == 1 ? "" : "s"),
                          r.same ? py::str("builds back THE SAME")
                                 : r.equivalent.value_or(false) ? py::str("builds back the same program (see below)")
                                                                : r.linked ? py::str("built, differs") : py::str("not built")));
    for (const py::str& note : r.notes) {
        py::print(py::str("  ") + note);
    }
    if (std::holds_alternative<py::list<py::str>>(r.source)) {
        for (const py::str& s : std::get<py::list<py::str>>(r.source)) {
            py::print(py::percent("  source: %s", s));
        }
    } else {
        py::print(py::percent("  source: %s", r.source_text()));
    }
    return r.same || r.equivalent.value_or(false) ? 0 : 1;
}

}  // namespace codebreaker::source::dos_writer
