// codebreaker/decompile/frame16.cpp -- typed C++ from codebreaker.decompile.frame16 (see the header).
//
// A 16-bit function's locals declared so that Microsoft C 8 lays them out as the original's: the instructions say what
// the frame was (every [bp-k] reached, as wide as it is reached; one whose address is taken is a buffer), and the L2
// tree is rewritten to them.
//
// Capstone: _frame reads each memory operand's *access* (capstone's `op.access & CS_AC_WRITE`), which the Insn of
// describe.hpp does not carry: it reads capstone's own records from platforms::decoder(16, detail=True).
#include "codebreaker/decompile/frame16.hpp"

#include <algorithm>
#include <map>
#include <mutex>
#include <set>
#include <string>
#include <vector>

#include <capstone/capstone.h>

#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/decompile/l2tree.hpp"
#include "codebreaker/platforms/package.hpp"
#include "py_os.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace codebreaker::decompile::frame16 {

namespace {

namespace T = l2tree;
using translate::ir::Dict;
using translate::ir::Val;
using List = Val::List;
using Pair = std::pair<int64_t, int64_t>;

// ---- types ----------------------------------------------------------------------------------------------------------
Dict kind_dict(const char* kind) {
    Dict d;
    d.set("kind", Val(kind));
    return d;
}
Dict width_type(int64_t w) {   // _WIDTH: 1 u8, 2 i16, 4 i32
    return kind_dict(w == 1 ? "u8" : w == 2 ? "i16" : "i32");
}
bool is_width(int64_t w) { return w == 1 || w == 2 || w == 4; }
// _UNSIGNED: 1 u8, 2 u16, 3 u16 (as the back end writes x._1_3_), 4 u32
bool unsigned_type(int64_t w, Dict* out) {
    if (w < 1 || w > 4) return false;
    *out = kind_dict(w == 1 ? "u8" : w == 4 ? "u32" : "u16");
    return true;
}

int64_t sizes(const py::str& kind) {   // _SIZES
    static const std::map<std::string, int64_t> table = {{"u8", 1},  {"i8", 1},  {"char", 1}, {"bool", 1},
                                                          {"u16", 2}, {"i16", 2}, {"u32", 4},  {"i32", 4}, {"ptr", 2}};
    auto it = table.find(kind.utf8());
    return it == table.end() ? 0 : it->second;
}

bool word_char(char32_t c) { return c == '_' || (c < 128 ? (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') : true); }
bool digit_char(char32_t c) { return c >= '0' && c <= '9'; }
int64_t to_int(const std::u32string& digits) {   // int(digits), saturating where Python's int would be unbounded
    int64_t v = 0;
    for (char32_t c : digits) {
        if (v > (INT64_MAX - 9) / 10) return INT64_MAX;
        v = v * 10 + static_cast<int64_t>(c - '0');
    }
    return v;
}

// A type's size in bytes (an array's elements times their size; a far pointer 4; xunknownN N), or 0 where it is not known.
int64_t type_bytes(const Val& t_in) {
    Dict t = T::dict_or_empty(t_in);
    Val k = T::get(t, "kind");
    if (T::is_text(k, "array")) {
        Val count = T::get(t, "count");
        int64_t n = 0;
        if (auto i = std::get_if<int64_t>(&count.v)) n = *i;
        else if (auto b = std::get_if<bool>(&count.v)) n = *b ? 1 : 0;
        else if (auto d = std::get_if<double>(&count.v)) n = static_cast<int64_t>(*d);
        return type_bytes(T::get(t, "elem")) * n;
    }
    if (T::is_text(k, "ptr")) return T::is_text(T::get(t, "space"), "far") ? 4 : 2;
    if (T::is_text(k, "class")) {
        // re.fullmatch(r"\w*?(\d+)", name or ""): all word characters, ending in at least one digit; the digits are the
        // longest trailing run (the lazy prefix is the shortest that leaves only digits)
        Val nv = T::get(t, "name");
        if (!T::is_str(nv)) return 0;
        const std::u32string& s = std::get<py::str>(nv.v).u32();
        for (char32_t c : s) if (!word_char(c)) return 0;
        size_t i = s.size();
        while (i > 0 && digit_char(s[i - 1])) i--;
        if (i == s.size()) return 0;
        return to_int(s.substr(i));
    }
    if (T::is_str(k)) return sizes(std::get<py::str>(k.v));
    return 0;
}

// ---- the frame ------------------------------------------------------------------------------------------------------
struct Frame {
    int64_t size = 0;
    std::map<int64_t, int64_t> places;   // bp offset -> width read or written
    std::set<int64_t> taken;             // bp offsets whose address is taken
    std::map<int64_t, Pair> writes;      // instruction address -> (bp offset, width) it writes
};

Frame frame_of(const py::bytes& code, int64_t start, int64_t end) {
    std::shared_ptr<platforms::Decoder> md = platforms::decoder(16, py::str("x86"), true);
    Frame f;
    py::bytes chunk = code.slice(start, end);
    size_t n = 0;
    cs_insn* insn = md->raw(reinterpret_cast<const uint8_t*>(chunk.raw().data()), chunk.raw().size(), start, 0, n);
    for (size_t k = 0; k < n; k++) {
        const cs_insn& i = insn[k];
        const cs_x86& x = i.detail->x86;
        std::string mn = i.mnemonic, os = i.op_str;
        if (k == 0 && mn == "enter") {
            f.size = x.operands[0].imm;
        } else if (k == 2 && mn == "sub" && os.compare(0, 4, "sp, ") == 0 && x.op_count > 1 && x.operands[1].type == X86_OP_IMM) {
            f.size = x.operands[1].imm;
        }
        for (int o = 0; o < x.op_count; o++) {
            const cs_x86_op& op = x.operands[o];
            if (op.type != X86_OP_MEM || op.mem.base != X86_REG_BP || op.mem.index != 0) continue;
            int64_t off = op.mem.disp;
            if (off >= 0) continue;   // (a parameter's: the caller's)
            if (mn == "lea") {
                f.taken.insert(off);
                continue;
            }
            int64_t w = op.size ? op.size : 2;
            auto it = f.places.find(off);
            f.places[off] = std::max<int64_t>(it == f.places.end() ? 0 : it->second, w);
            if (o == 0 && (op.access & CS_AC_WRITE)) f.writes[static_cast<int64_t>(i.address)] = {off, w};
        }
        if (mn == "ret" || mn == "retf") break;
    }
    md->release(insn, n);
    return f;
}

bool far_pointer(const Val& t) { return T::is_dict(t) && T::is_text(T::get(t, "kind"), "ptr") && T::is_text(T::get(t, "space"), "far"); }

// (the name of a node's symbol: (n.get("sym") or {}).get("name"))
Val sym_name(const Dict& n) { return T::get(T::dict_or_empty(T::get(n, "sym")), "name"); }
std::string key_of(const Val& name) {   // a dictionary key for a name (None among them)
    return T::is_str(name) ? std::get<py::str>(name.v).utf8() : std::string("\1None");
}

Val text_node(const char* node) {
    Dict d;
    d.set("node", Val(node));
    return Val(d);
}

// One piece of a buffer read through the array of its bytes, at `off`, `width` wide.
Dict piece(const py::str& buf_name, int64_t count, int64_t off, const Val& width) {
    Dict sym;
    sym.set("kind", Val("local"));
    sym.set("name", Val(buf_name));
    Dict arrt;
    arrt.set("kind", Val("array"));
    arrt.set("elem", Val(kind_dict("u8")));
    arrt.set("count", Val(count));
    Dict arr;
    arr.set("node", Val("Name"));
    arr.set("sym", Val(sym));
    arr.set("type", Val(arrt));
    Val addr;
    if (off) {
        Dict lit;
        lit.set("node", Val("Lit"));
        lit.set("value", Val(off));
        lit.set("text", Val(py::str(std::to_string(off))));
        Dict bin;
        bin.set("node", Val("Binary"));
        bin.set("op", Val("+"));
        bin.set("left", Val(arr));
        bin.set("right", Val(lit));
        addr = Val(bin);
    } else {
        addr = Val(arr);
    }
    Dict pt;
    pt.set("kind", Val("ptr"));
    pt.set("space", Val("near"));
    pt.set("seg", Val("ss"));
    pt.set("to", width);
    Dict cast;
    cast.set("node", Val("Cast"));
    cast.set("type", Val(pt));
    cast.set("x", addr);
    Dict un;
    un.set("node", Val("Unary"));
    un.set("op", Val("*"));
    un.set("x", Val(cast));
    un.set("type", width);
    return un;
}

std::string fmt_hex(int64_t v) {
    char b[32];
    std::snprintf(b, sizeof b, "%llx", static_cast<unsigned long long>(v));
    return b;
}

// the piece-of-a-buffer name _PIECE = r"_(\d+)_(\d+)_" fullmatch: (from byte, bytes)
bool piece_name(const py::str& s, int64_t* from, int64_t* bytes_) {
    const std::u32string& u = s.u32();
    size_t i = 0;
    if (i >= u.size() || u[i] != '_') return false;
    i++;
    size_t a = i;
    while (i < u.size() && digit_char(u[i])) i++;
    if (i == a || i >= u.size() || u[i] != '_') return false;
    int64_t x = to_int(u.substr(a, i - a));
    i++;
    size_t b = i;
    while (i < u.size() && digit_char(u[i])) i++;
    if (i == b || i >= u.size() || u[i] != '_' || i + 1 != u.size()) return false;
    *from = x;
    *bytes_ = to_int(u.substr(b, i - b));
    return true;
}

}  // namespace

py::list<translate::ir::Dict> _walk(const translate::ir::Val& n) { return T::nodes(n); }

std::tuple<translate::ir::Dict, py::list<py::str>> lay_out(const translate::ir::Dict& doc_in, const py::bytes& code, int64_t start,
                                                         int64_t end) {
    Dict doc = T::deepcopy(doc_in);
    py::list<py::str> said;
    Frame fr = frame_of(code, start, end);
    if (!fr.size) {
        said.append(py::str("no frame"));
        return {doc, said};
    }
    List body = T::list_or_empty(T::get(doc, "body"));
    Val body_val(body);
    // the Locals, by name (a later one of a name replaces an earlier, keeping the earlier's place in the order)
    std::vector<Dict> locals_;
    std::vector<std::string> order;                 // by_name's keys in order
    std::map<std::string, Dict> by_name;
    std::map<std::string, Val> name_of;             // key -> the name as a Val
    for (const Val& s : body.vec()) {
        if (!T::is_text(T::get(s, "node"), "Local")) continue;
        Dict d = T::as_dict(s);
        locals_.push_back(d);
        Val nm = sym_name(d);
        std::string key = key_of(nm);
        if (!by_name.count(key)) {
            order.push_back(key);
            name_of[key] = nm;
        }
        by_name.insert_or_assign(key, d);
    }
    // each local's place in the frame
    std::vector<std::pair<std::string, int64_t>> place;   // in insertion order
    auto place_find = [&](const std::string& key) -> int64_t* {
        for (auto& kv : place) if (kv.first == key) return &kv.second;
        return nullptr;
    };
    for (const std::string& key : order) {
        Val at = T::get(by_name.at(key), "at");
        if (T::is_dict(at) && T::as_dict(at).contains(py::str("stack"))) {
            Val st = T::at(T::as_dict(at), "stack");
            int64_t sv = 0;
            if (auto i = std::get_if<int64_t>(&st.v)) sv = *i;
            place.emplace_back(key, sv + 2);
        }
    }
    T::walk(body_val, [&](const Dict& st) {
        if (!T::is_text(T::get(st, "node"), "ExprStmt") || !T::is_text(T::get(T::dict_or_empty(T::get(st, "expr")), "node"), "Assign")) return;
        Dict tgt = T::dict_or_empty(T::get(T::as_dict(T::get(st, "expr")), "target"));
        Val nm = sym_name(tgt);
        std::string key = key_of(nm);
        if (!T::is_text(T::get(tgt, "node"), "Name") || !T::is_str(nm) || !by_name.count(key) || place_find(key)) return;
        std::set<Pair> hits;
        Val at = T::get(T::dict_or_empty(T::get(st, "origin")), "at");
        if (T::is_list(at)) {
            for (const Val& a : T::as_list(at).vec()) {
                if (auto i = std::get_if<int64_t>(&a.v)) {
                    auto w = fr.writes.find(*i & 0xFFFF);
                    if (w != fr.writes.end()) hits.insert(w->second);
                }
            }
        }
        if (hits.size() == 1) place.emplace_back(key, hits.begin()->first);
    });
    if (place.empty()) {
        said.append(py::str("no local the frame places"));
        return {doc, said};
    }
    // the original's places: a buffer (its address taken) runs to the next place above it
    std::set<int64_t> pts(fr.taken);
    for (auto& kv : fr.places) pts.insert(kv.first);
    for (auto& kv : place) pts.insert(kv.second);
    std::vector<int64_t> points(pts.begin(), pts.end());
    std::map<int64_t, int64_t> span;
    for (size_t k = 0; k < points.size(); k++) {
        int64_t top = k + 1 < points.size() ? points[k + 1] : 0;
        span[points[k]] = top - points[k];
    }
    std::vector<std::pair<int64_t, int64_t>> buffers;   // sorted(taken): offset -> size
    for (int64_t b : fr.taken) {
        // the places inside a buffer: from it up to the next taken place or a place written as a variable of its own (one
        // assigned by a statement: a C variable, not a piece)
        std::set<int64_t> own;
        for (auto& kv : place) {
            Val at = T::get(by_name.at(kv.first), "at");
            if (at.is_none() || T::is_text(at, "none")) own.insert(kv.second);
        }
        int64_t top = 0;
        for (int64_t x : points) if (x > b && (fr.taken.count(x) || own.count(x))) top = std::min(top, x);
        buffers.emplace_back(b, top - b);
    }
    std::vector<std::string> said_s;
    std::vector<std::pair<std::string, std::pair<int64_t, int64_t>>> piece_of;   // key -> (buffer, offset in it)
    auto piece_find = [&](const std::string& key) -> std::pair<int64_t, int64_t>* {
        for (auto& kv : piece_of) if (kv.first == key) return &kv.second;
        return nullptr;
    };
    for (auto& pl : place) {
        int64_t p = pl.second;
        for (auto& bn : buffers) {
            int64_t b = bn.first, n = bn.second;
            if (!(b <= p && p < b + n)) continue;
            int64_t inside = 0;
            for (auto& q : place) if (b <= q.second && q.second < b + n) inside++;
            auto sp = span.find(b);
            bool alone = p == b && inside == 1 && sp != span.end() && sp->second == n;
            if (!alone) {
                if (auto* e = piece_find(pl.first)) *e = {b, p - b};
                else piece_of.push_back({pl.first, {b, p - b}});
            }
        }
    }
    // a buffer one of whose pieces holds a far pointer -- declared so, or used so, or the decompiler's `_x` (x and the word
    // above it, joined: a far pointer's halves) -- is left as it was
    std::set<int64_t> held;
    for (auto& kv : piece_of) if (far_pointer(T::get(by_name.at(kv.first), "type"))) held.insert(kv.second.first);
    T::walk(body_val, [&](const Dict& n) {
        Val nmv = T::is_text(T::get(n, "node"), "Name") ? sym_name(n) : Val();
        if (T::is_str(nmv) && piece_find(key_of(nmv)) && far_pointer(T::get(n, "type"))) {
            held.insert(piece_find(key_of(nmv))->first);
        } else if (T::is_str(nmv) && std::get<py::str>(nmv.v).startswith(py::str("_")) && std::get<py::str>(nmv.v).size() > 0 &&
                   piece_find(std::get<py::str>(nmv.v).slice_from(1).utf8()) && !by_name.count(key_of(nmv))) {
            held.insert(piece_find(std::get<py::str>(nmv.v).slice_from(1).utf8())->first);
        }
    });
    // In a frame with an array local the buffers are left too, as is one another local overlaps: the back end lays such a
    // frame out itself
    for (auto& pl : place) {
        Dict t = T::dict_or_empty(T::get(by_name.at(pl.first), "type"));
        int64_t sz = type_bytes(Val(t));
        if (!sz) sz = 2;
        int64_t p = pl.second;
        for (auto& bn : buffers) {
            int64_t b = bn.first, n = bn.second;
            bool any = false;
            for (auto& kv : piece_of) if (kv.second.first == b) any = true;
            if (!any) continue;
            if (T::is_text(T::get(t, "kind"), "array") || (!piece_find(pl.first) && p < b + n && b < p + sz && p != b)) held.insert(b);
        }
    }
    {
        std::vector<std::pair<std::string, std::pair<int64_t, int64_t>>> kept;
        for (auto& kv : piece_of) if (!held.count(kv.second.first)) kept.push_back(kv);
        piece_of = kept;
    }
    if (!held.empty()) {
        std::string offs;
        for (int64_t b : held) {
            if (!offs.empty()) offs += ", ";
            offs += "[bp" + std::to_string(b) + "]";
        }
        said.append(py::str("a buffer left as it was: a far pointer in it, an array in the frame, or a local over it (" + offs + ")"));
    }
    // rewrite: pieces read through one array; scalars typed by their width
    std::vector<std::pair<int64_t, py::str>> names;   // buffer -> its array's name
    std::map<int64_t, int64_t> buf_size;
    for (auto& bn : buffers) {
        buf_size[bn.first] = bn.second;
        bool any = false;
        for (auto& kv : piece_of) if (kv.second.first == bn.first) any = true;
        if (any) names.emplace_back(bn.first, py::str("local_" + fmt_hex(-bn.first)));
    }
    auto buf_name = [&](int64_t b) -> const py::str& {
        for (auto& kv : names) if (kv.first == b) return kv.second;
        throw py::KeyError("buffer");
    };
    {
        // (a piece of one first: x._2_2_, 2 bytes at x + 2 -- written `->_2_2_` of the cast, it did not compile)
        py::list<Dict> all = T::nodes(body_val);
        for (const Dict& n : all.vec()) {
            if (!T::is_text(T::get(n, "node"), "Select")) continue;
            Dict obj = T::dict_or_empty(T::get(n, "obj"));
            Val pn = T::get(T::dict_or_empty(T::get(n, "sym")), "name");
            int64_t from = 0, bytes_ = 0;
            Val nm = sym_name(obj);
            Dict ut;
            if (T::is_str(pn) && piece_name(std::get<py::str>(pn.v), &from, &bytes_) && T::is_text(T::get(obj, "node"), "Name") &&
                T::is_str(nm) && piece_find(key_of(nm)) && unsigned_type(bytes_, &ut)) {
                auto bo = *piece_find(key_of(nm));
                n.clear();
                n.update(piece(buf_name(bo.first), buf_size.at(bo.first), bo.second + from, Val(ut)));
            }
        }
    }
    for (auto& kv : piece_of) {
        const std::string& key = kv.first;
        int64_t b = kv.second.first, off = kv.second.second;
        Val width = T::get(by_name.at(key), "type");   // (at its declared type)
        T::walk(body_val, [&](const Dict& n) {
            if (T::is_text(T::get(n, "node"), "Name") && key_of(sym_name(n)) == key && T::is_str(sym_name(n))) {
                n.clear();
                n.update(piece(buf_name(b), buf_size.at(b), off, width));
            }
        });
        T::walk(body_val, [&](const Dict& n) {
            if (!T::is_text(T::get(n, "node"), "MachineFragment")) return;
            List out;
            std::string repl = buf_name(b).utf8() + (off ? "[" + std::to_string(off) + "]" : "");
            py::str pat = py::str("\\b") + py::re::escape(std::get<py::str>(name_of.at(key).v)) + py::str("\\b");
            for (const Val& ln : T::list_or_empty(T::get(n, "asm")).vec()) {
                out.append(Val(py::re::sub(pat, py::str(repl), std::get<py::str>(ln.v))));
            }
            n.set("asm", Val(out));
        });
        said.append(std::get<py::str>(name_of.at(key).v) + py::str(": ") + buf_name(b) + py::str("'s bytes " + std::to_string(off) + ".."));
    }
    for (auto& pl : place) {
        if (piece_find(pl.first)) continue;
        int64_t p = pl.second;
        auto w = fr.places.find(p);
        Dict t = T::dict_or_empty(T::get(by_name.at(pl.first), "type"));
        // (a bool only: a local of the decompiler's unknown type, xunknownN, takes the type its uses give it in the back end)
        if (w != fr.places.end() && is_width(w->second) && T::is_text(T::get(t, "kind"), "bool")) {
            by_name.at(pl.first).set("type", Val(width_type(w->second)));
            said.append(std::get<py::str>(name_of.at(pl.first).v) + py::str(": " + std::to_string(w->second) + " bytes at [bp" + std::to_string(p) + "]"));
        }
    }
    // declarations: the frame's, nearest the frame pointer first; the rest as they were, after
    std::vector<std::pair<int64_t, Val>> decl;
    for (auto& kv : names) {
        int64_t b = kv.first;
        Dict sym;
        sym.set("kind", Val("local"));
        sym.set("name", Val(kv.second));
        Dict at;
        at.set("stack", Val(b - 2));
        Dict arrt;
        arrt.set("kind", Val("array"));
        arrt.set("elem", Val(kind_dict("u8")));
        arrt.set("count", Val(buf_size.at(b)));
        Dict l;
        l.set("node", Val("Local"));
        l.set("sym", Val(sym));
        l.set("type", Val(arrt));
        l.set("at", Val(at));
        decl.emplace_back(b, Val(l));
    }
    for (const Dict& s : locals_) {
        Val nm = sym_name(s);
        std::string key = key_of(nm);
        if (T::is_str(nm) && piece_find(key)) continue;
        int64_t* pp = place_find(key);
        decl.emplace_back(pp ? *pp : 1, Val(s));
    }
    std::stable_sort(decl.begin(), decl.end(), [](const std::pair<int64_t, Val>& a, const std::pair<int64_t, Val>& b) {
        int64_t ka = a.first <= 0 ? -a.first : 1, kb = b.first <= 0 ? -b.first : 1;
        return ka < kb;
    });
    List out;
    for (auto& d : decl) out.append(d.second);
    for (const Val& s : body.vec()) if (!T::is_text(T::get(s, "node"), "Local")) out.append(s);
    doc.set("body", Val(out));
    return {doc, said};
}

}  // namespace codebreaker::decompile::frame16
