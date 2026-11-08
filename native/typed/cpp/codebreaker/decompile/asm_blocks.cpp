// codebreaker/decompile/asm_blocks.cpp -- typed C++ port of codebreaker.decompile.asm_blocks. See asm_blocks.hpp.

#include "codebreaker/decompile/asm_blocks.hpp"

#include <algorithm>
#include <vector>

namespace codebreaker::decompile::asm_blocks {

using inline_asm::X86_OP_IMM;
using inline_asm::X86_OP_MEM;
using inline_asm::X86_OP_REG;

const py::re::Pattern& _HELPER() {
    static const py::re::Pattern helper = py::re::compile(py::str("^int([0-9a-f]{2})(_[a-z_]+)?$"));
    return helper;
}
const py::list<py::str> _FLAGS = {py::str("int_CF"), py::str("int_ZF")};
const py::list<py::str> _SEGS = {py::str("ds"), py::str("es"), py::str("ss"), py::str("cs")};

// ---------------------------------------------------------------------------------------- the tree

bool truthy(const Json& v) {
    switch (v.kind) {
        case Json::None: return false;
        case Json::Bool: return v.b;
        case Json::Int: return v.i != 0;
        case Json::Float: return v.d != 0.0;
        case Json::Str: return !v.s.empty();
        case Json::List: return !v.list.empty();
        case Json::Dict: return !v.dict.empty();
    }
    return false;
}

bool is_str(const Json& v, const char* text) { return v.kind == Json::Str && v.s == text; }

Json deepcopy(const Json& v) {
    switch (v.kind) {
        case Json::List: {
            py::list<Json> out;
            for (const Json& x : v.list) out.append(deepcopy(x));
            return Json::make_list(out);
        }
        case Json::Dict: {
            py::dict<py::str, Json> out;
            for (const auto& kv : v.dict.items()) out.set(std::get<0>(kv), deepcopy(std::get<1>(kv)));
            return Json::make_dict(out);
        }
        default: return v;
    }
}

Json get(const Json& d, const char* key) {
    if (d.kind != Json::Dict) throw py::AttributeError("object has no attribute 'get'");
    py::str k(key);
    return d.dict.contains(k) ? d.dict.at(k) : Json();
}

Json get(const Json& d, const char* key, const Json& dflt) {
    if (d.kind != Json::Dict) throw py::AttributeError("object has no attribute 'get'");
    py::str k(key);
    return d.dict.contains(k) ? d.dict.at(k) : dflt;
}

namespace {

Json empty_dict() { return Json::make_dict(py::dict<py::str, Json>()); }

// `x or {}`
Json or_dict(const Json& x) { return truthy(x) ? x : empty_dict(); }

// a name in the tree: str, or None for no name (see the typing notes)
std::optional<py::str> name_of(const Json& v) {
    if (v.kind == Json::None) return std::nullopt;
    if (v.kind != Json::Str) throw py::TypeError("a name in the tree is a string");
    return v.s;
}

// the `sym` name of a node: (n.get("sym") or {}).get("name")
Json sym_name(const Json& n) { return get(or_dict(get(n, "sym")), "name"); }

// int(text, 16)
int64_t hex_int(const py::str& text) {
    try {
        return std::stoll(text.utf8(), nullptr, 16);
    } catch (const std::out_of_range&) {
        throw py::OverflowError("int too large for int64");
    }
}

// int(text, 0) of the digits the pattern \[bp ([+-]) (0x[0-9a-f]+|\d+)\] takes: 0x.. is hex; decimal
// digits may not start with 0 unless they are all zeros (Python: "invalid literal")
int64_t int_base0(const py::str& text) {
    std::string t = text.utf8();
    try {
        if (t.size() > 1 && t[0] == '0' && (t[1] == 'x' || t[1] == 'X')) return std::stoll(t.substr(2), nullptr, 16);
        if (t.size() > 1 && t[0] == '0' && t.find_first_not_of('0') != std::string::npos) throw py::ValueError("invalid literal for int() with base 0");
        return std::stoll(t, nullptr, 10);
    } catch (const std::out_of_range&) {
        throw py::OverflowError("int too large for int64");
    }
}

// the address as the signed number the dictionaries are keyed by
int64_t addr(const inline_asm::Instruction& i) { return static_cast<int64_t>(i.address); }

void walk_into(const Json& n, py::list<Json>& out) {
    if (n.kind == Json::Dict) {
        out.append(n);
        for (const auto& kv : n.dict.items()) walk_into(std::get<1>(kv), out);
    } else if (n.kind == Json::List) {
        for (const Json& x : n.list) walk_into(x, out);
    }
}

void lists_into(const Json& n, py::list<py::list<Json>>& out) {
    if (n.kind == Json::Dict) {
        for (const auto& kv : n.dict.items()) {
            const py::str& k = std::get<0>(kv);
            const Json& v = std::get<1>(kv);
            if ((k == "body" || k == "then" || k == "orelse" || k == "stmts") && v.kind == Json::List) out.append(v.list);
            lists_into(v, out);
        }
    } else if (n.kind == Json::List) {
        for (const Json& x : n.list) lists_into(x, out);
    }
}

}  // namespace

py::list<Json> _walk(const Json& n) {
    py::list<Json> out;
    walk_into(n, out);
    return out;
}

py::list<py::list<Json>> _lists(const Json& n) {
    py::list<py::list<Json>> out;
    lists_into(n, out);
    return out;
}

py::set<int64_t> _at(const Json& stmt) {
    Json at = get(or_dict(get(stmt, "origin")), "at");
    py::set<int64_t> out;
    if (!truthy(at)) return out;                             // (or ())
    if (at.kind != Json::List) throw py::TypeError("origin.at is a list of addresses");
    for (const Json& a : at.list) {
        if (a.kind != Json::Int) throw py::TypeError("origin.at is a list of addresses");
        out.add(a.i);
    }
    return out;
}

std::optional<int64_t> _calls_helper(const Json& stmt) {
    for (const Json& n : _walk(get(stmt, "expr"))) {
        if (is_str(get(n, "node"), "Call")) {
            Json nm = get(or_dict(get(or_dict(get(n, "target")), "sym")), "name", Json::make_str(py::str("")));
            if (nm.kind != Json::Str) throw py::TypeError("expected string or bytes-like object");
            std::optional<py::re::Match> m = _HELPER().match(nm.s);
            if (m) return hex_int(m->group(1));
        }
    }
    return std::nullopt;
}

bool _reads(const Json& stmt, const py::list<py::str>& names) {
    for (const Json& n : _walk(get(stmt, "cond"))) {
        if (!is_str(get(n, "node"), "Name")) continue;
        Json nm = sym_name(n);
        if (nm.kind == Json::Str && names.contains(nm.s)) return true;
    }
    return false;
}

Frame _frame_names(const Json& doc, bool far) {
    (void)far;
    Frame out;
    Json ft = or_dict(get(or_dict(get(doc, "sym")), "type"));
    Json params = get(ft, "params");
    if (truthy(params)) {
        if (params.kind != Json::List) throw py::TypeError("params is a list");
        for (const Json& p : params.list) {
            Json at = get(or_dict(get(p, "at")), "stack");
            if (at.kind != Json::None) {
                if (at.kind != Json::Int) throw py::TypeError("unsupported operand type(s) for +");
                if (!p.dict.contains(py::str("name"))) throw py::KeyError("'name'");
                std::optional<py::str> nm = name_of(p.dict.at(py::str("name")));
                if (!nm) throw py::TypeError("a parameter's name is a string");
                out.set(at.i + 2, *nm);                      // (BP is SP at entry less the BP pushed)
            }
        }
    }
    static const py::re::Pattern stack = py::re::compile(py::str("^[a-z]*Stack_([0-9a-f]+)$"));
    for (const Json& n : _walk(get(doc, "body"))) {
        if (is_str(get(n, "node"), "Name")) {
            Json nm = get(or_dict(get(n, "sym")), "name", Json::make_str(py::str("")));
            if (nm.kind != Json::Str) throw py::TypeError("expected string or bytes-like object");
            std::optional<py::re::Match> m = stack.match(nm.s);
            if (m) out.setdefault(2 - hex_int(m->group(1)), nm.s);
        }
    }
    return out;
}

py::str _named(const py::str& text, const Frame& frame) {
    static const py::re::Pattern bp = py::re::compile(py::str("\\[bp ([+-]) (0x[0-9a-f]+|\\d+)\\]"));
    return bp.sub_fn(
        [&](const py::re::Match& m) {
            int64_t k = int_base0(m.group(2)) * (m.group(1) == "-" ? -1 : 1);
            return frame.get(k, m.group(0));
        },
        text);
}

py::list<Instruction> _decode(const Decoder& md, const py::bytes& code, int64_t start, int64_t end) {
    return md(code.slice(start, end), static_cast<uint64_t>(start));
}

py::list<Instruction> _decode(const Decoder& md, const py::bytearray& code, int64_t start, int64_t end) {
    return _decode(md, code.view(), start, end);
}

bool _feeds(const Instruction& i) {
    if (inline_asm::_386(i)) return false;
    const py::str& mn = i.mnemonic;
    if (mn == "push" || mn == "pop")
        return _SEGS.contains(i.op_str) || i.op_str == "ax" || i.op_str == "bx" || i.op_str == "cx" || i.op_str == "dx" || i.op_str == "si" ||
               i.op_str == "di";
    if (inline_asm::_JUMPS.contains(mn) || mn == "call" || mn == "lcall" || mn == "ret" || mn == "retf" || mn == "int") return false;
    for (int64_t k = 0; k < i.operands.size(); k++)
        if (i.operands[k].type == X86_OP_MEM && (i.access[k] & CS_AC_WRITE)) return false;
    return !i.operands.empty() && i.operands[0].type == X86_OP_REG;
}

std::optional<py::str> _line(const Instruction& i, const Frame& frame, const py::dict<int64_t, py::str>& labels, const Fixups& fixups,
                             const NameAt& name_at, std::optional<int64_t> seg) {
    py::str mn = py::split(i.mnemonic)[-1];
    if (inline_asm::_JUMPS.contains(mn)) {
        int64_t to = i.operands[0].imm & 0xFFFF;
        if (labels.contains(to)) return py::percent(py::str("%s %s"), mn, labels.at(to));
        return std::nullopt;
    }
    bool site = false;
    for (int64_t a = addr(i); a < addr(i) + i.size; a++)
        if (fixups.names.contains(a)) site = true;
    if (site || mn == "call" || mn == "lcall") return std::nullopt;
    py::str ops = inline_asm::_operands(i, fixups, name_at, seg.value_or(0));
    py::str prefix;
    if ((i.prefix[0] == 0xF2 || i.prefix[0] == 0xF3) && inline_asm::_STRING.contains(mn)) prefix = i.prefix[0] == 0xF3 ? "rep " : "repne ";
    return _named(py::percent(py::str("%s%s%s"), prefix, mn, !ops.empty() ? py::str(" ") + ops : py::str("")), frame);
}

namespace {

void unused_names(const Json& body, py::set<py::str>& used, bool& none_used) {
    for (const Json& n : _walk(body)) {
        if (!is_str(get(n, "node"), "Local") && get(n, "sym").kind == Json::Dict) {
            std::optional<py::str> nm = name_of(sym_name(n));
            if (nm) used.add(*nm);
            else none_used = true;
        }
    }
}

}  // namespace

std::tuple<Json, py::list<py::str>> place(const Decoder& md, const Json& doc_in, const py::bytes& code, int64_t start, int64_t end,
                                          const Fixups& fixups_in, const NameAt& name_at_in, std::optional<int64_t> seg,
                                          const std::tuple<int64_t, int64_t>& window) {
    // fixups, name_at = fixups or {}, name_at or (lambda s, o: None)
    const Fixups& fixups = fixups_in;
    NameAt name_at = name_at_in ? name_at_in : NameAt([](int64_t, int64_t) { return std::optional<py::str>(); });
    Json doc = deepcopy(doc_in);
    py::list<Instruction> ins = _decode(md, code, start, end);
    if (ins.empty()) return {doc, py::list<py::str>{py::str("no instructions")}};
    py::dict<int64_t, int64_t> index;
    for (int64_t k = 0; k < ins.size(); k++) index.set(addr(ins[k]), k);
    // far = (doc.get("sym") or {}).get("type", {}).get("distance") == "far"
    bool far = is_str(get(get(or_dict(get(doc, "sym")), "type", empty_dict()), "distance"), "far");
    Frame frame = _frame_names(doc, far);
    py::list<py::str> said;
    py::set<int64_t> targets;
    for (const Instruction& i : ins)
        if (inline_asm::_JUMPS.contains(i.mnemonic) && !i.operands.empty() && i.operands[0].type == X86_OP_IMM) targets.add(i.operands[0].imm & 0xFFFF);
    static const py::re::Pattern int_reg = py::re::compile(py::str("^int_(?!CF|ZF)[A-Z]{2}"));
    for (const py::list<Json>& body : _lists(doc)) {
        int64_t k = 0;
        while (k < body.size()) {
            Json st = body[k];
            std::optional<int64_t> vec;
            if (is_str(get(st, "node"), "ExprStmt")) vec = _calls_helper(st);
            if (!vec) {
                k += 1;
                continue;
            }
            py::set<int64_t> at;
            for (int64_t a : _at(st)) at.add(a & 0xFFFF);
            py::list<int64_t> ints;
            {
                py::list<int64_t> sat;
                for (int64_t a : at) sat.append(a);
                sat.sort();
                for (int64_t a : sat)
                    if (index.contains(a) && ins[index.at(a)].mnemonic == "int") ints.append(a);
            }
            if (ints.empty()) {
                said.append(py::str("an interrupt helper with no INT among its instructions"));
                k += 1;
                continue;
            }
            int64_t p = index.at(ints[0]);
            int64_t lo = p;
            while (lo > 0 && _feeds(ins[lo - 1]) && !targets.contains(addr(ins[lo - 1]))) lo -= 1;
            int64_t hi = p;
            while (hi + 1 < ins.size() && (ins[hi + 1].mnemonic == "pop" || ins[hi + 1].mnemonic == "push") && _SEGS.contains(ins[hi + 1].op_str)) hi += 1;
            lo = std::max<int64_t>(0, lo + std::get<0>(window));
            hi = std::min<int64_t>(ins.size() - 1, hi + std::get<1>(window));
            py::set<int64_t> span;
            for (int64_t x = lo; x <= hi; x++) span.add(addr(ins[x]));
            py::list<std::optional<py::str>> lines;
            for (int64_t x = lo; x <= hi; x++) lines.append(_line(ins[x], frame, py::dict<int64_t, py::str>(), fixups, name_at, seg));
            {
                int64_t none_at = -1;
                for (int64_t q = 0; q < lines.size(); q++)
                    if (!lines[q]) {
                        none_at = q;
                        break;
                    }
                if (none_at >= 0) {
                    said.append(py::percent(py::str("an instruction of the block the inline assembler cannot say (%04x)"), addr(ins[lo + none_at])));
                    k += 1;
                    continue;
                }
            }
            // what the interrupt leaves in registers read by the C after it (int_BX, int_DS): not a block
            // of this shape -- the C would read registers no variable holds
            std::optional<py::str> later;
            for (int64_t x = k + 1; x < body.size(); x++) {
                for (const Json& n : _walk(body[x])) {
                    if (!is_str(get(n, "node"), "Name")) continue;
                    Json nm = get(or_dict(get(n, "sym")), "name", Json::make_str(py::str("")));
                    if (nm.kind != Json::Str) throw py::TypeError("expected string or bytes-like object");
                    if (int_reg.match(nm.s) && !later) later = nm.s;
                }
            }
            if (later) {
                said.append(py::percent(py::str("INT %02Xh: the C after it reads %s"), *vec, *later));
                k += 1;
                continue;
            }
            // the statements made only of the block's instructions go; the block stands where the first was
            int64_t first = k;
            auto only_span = [&](const Json& s) {
                for (int64_t a : _at(s))
                    if (!span.contains(a & 0xFFFF)) return false;
                return true;
            };
            while (first > 0 && is_str(get(body[first - 1], "node"), "ExprStmt") && !_at(body[first - 1]).empty() && only_span(body[first - 1])) first -= 1;
            int64_t length = 0;
            for (int64_t x = lo; x <= hi; x++) length += ins[x].size;
            py::list<Json> asm_lines;
            for (const std::optional<py::str>& l : lines) asm_lines.append(Json::make_str(*l));
            py::list<Json> span_sorted;
            {
                py::list<int64_t> sv;
                for (int64_t a : span) sv.append(a);
                sv.sort();
                for (int64_t a : sv) span_sorted.append(Json::make_int(a));
            }
            py::dict<py::str, Json> frag;
            frag.set(py::str("node"), Json::make_str(py::str("MachineFragment")));
            frag.set(py::str("reason"), Json::make_str(py::str("inline")));
            frag.set(py::str("length"), Json::make_int(length));
            frag.set(py::str("why"), Json::make_str(py::percent(py::str("INT %02Xh"), *vec)));
            frag.set(py::str("asm"), Json::make_list(asm_lines));
            py::dict<py::str, Json> origin;
            origin.set(py::str("at"), Json::make_list(span_sorted));
            frag.set(py::str("origin"), Json::make_dict(origin));
            body.set_slice(first, k + 1, py::list<Json>{Json::make_dict(frag)});
            said.append(py::percent(py::str("INT %02Xh: an _asm block %04x-%04x"), *vec, addr(ins[lo]), addr(ins[hi])));
            k = first + 1;
            // its carry: the first If after it reading int_CF / int_ZF
            for (int64_t j = k, n = body.size(); j < n; j++) {
                Json cand = body[j];
                if (!is_str(get(cand, "node"), "If") || !_reads(cand, _FLAGS)) continue;
                py::list<int64_t> cat;
                for (int64_t a : _at(cand)) cat.append(a & 0xFFFF);
                cat.sort();
                Json then_j = get(cand, "then");
                py::list<Json> then;
                if (then_j.kind == Json::List) then = then_j.list;
                else if (truthy(then_j)) then = py::list<Json>{then_j};
                py::list<int64_t> then_at;
                for (const Json& s : then)
                    for (int64_t a : _at(s)) then_at.append(a & 0xFFFF);
                then_at.sort();
                if (cat.empty() || then_at.empty() || !index.contains(cat[0])) {
                    said.append(py::str("its carry: an If the instructions do not place"));
                    break;
                }
                int64_t a = index.at(cat[0]);
                if (!index.contains(then_at[0]) || index.at(then_at[0]) <= a) {
                    said.append(py::str("its carry: a then-part before its test"));
                    break;
                }
                int64_t b = index.at(then_at[0]);
                std::optional<int64_t> after;
                if (index.get(then_at[-1], ins.size()) + 1 < ins.size()) after = addr(ins[index.at(then_at[-1]) + 1]);
                py::dict<int64_t, py::str> labels;
                labels.set(then_at[0], py::percent(py::str("L_%04x"), then_at[0]));
                if (after) labels.set(*after, py::percent(py::str("L_%04x"), *after));
                py::set<int64_t> inner;
                for (int64_t x = a; x < b; x++) inner.add(addr(ins[x]));
                for (int64_t t : targets)
                    if (inner.contains(t)) labels.set(t, py::percent(py::str("L_%04x"), t));
                py::list<std::optional<py::str>> test;
                for (int64_t x = a; x < b; x++) {
                    if (inner.contains(addr(ins[x])) && targets.contains(addr(ins[x])) && x != a)
                        test.append(py::percent(py::str("L_%04x:"), addr(ins[x])));
                    test.append(_line(ins[x], frame, labels, fixups, name_at, seg));
                }
                bool has_none = false;
                for (const std::optional<py::str>& l : test)
                    if (!l) has_none = true;
                if (has_none) {
                    said.append(py::str("its carry: an instruction the block cannot say"));
                    break;
                }
                int64_t tlen = 0;
                for (int64_t x = a; x < b; x++) tlen += ins[x].size;
                py::list<Json> test_lines;
                for (const std::optional<py::str>& l : test) test_lines.append(Json::make_str(*l));
                py::list<Json> inner_sorted;
                {
                    py::list<int64_t> sv;
                    for (int64_t v : inner) sv.append(v);
                    sv.sort();
                    for (int64_t v : sv) inner_sorted.append(Json::make_int(v));
                }
                py::dict<py::str, Json> carry;
                carry.set(py::str("node"), Json::make_str(py::str("MachineFragment")));
                carry.set(py::str("reason"), Json::make_str(py::str("inline")));
                carry.set(py::str("why"), Json::make_str(py::percent(py::str("the carry INT %02Xh left"), *vec)));
                carry.set(py::str("length"), Json::make_int(tlen));
                carry.set(py::str("asm"), Json::make_list(test_lines));
                py::dict<py::str, Json> carry_origin;
                carry_origin.set(py::str("at"), Json::make_list(inner_sorted));
                carry.set(py::str("origin"), Json::make_dict(carry_origin));
                py::dict<py::str, Json> labeled;
                labeled.set(py::str("node"), Json::make_str(py::str("Labeled")));
                labeled.set(py::str("label"), Json::make_str(labels.at(then_at[0])));
                labeled.set(py::str("body"), Json::make_list(then));
                py::list<Json> parts{Json::make_dict(carry), Json::make_dict(labeled)};
                if (after) {
                    py::dict<py::str, Json> tail;
                    tail.set(py::str("node"), Json::make_str(py::str("Labeled")));
                    tail.set(py::str("label"), Json::make_str(labels.at(*after)));
                    tail.set(py::str("body"), Json::make_list(py::list<Json>()));
                    parts.append(Json::make_dict(tail));
                }
                body.set_slice(j, j + 1, parts);
                said.append(py::percent(py::str("its carry: an _asm block %04x-%04x, the then-part at %s"), addr(ins[a]), addr(ins[b - 1]),
                                        labels.at(then_at[0])));
                break;
            }
        }
    }
    bool any_block = false;
    for (const py::str& s : said)
        if (s.contains(py::str("block"))) any_block = true;
    if (any_block) _unused(doc);
    return {doc, said};
}

std::tuple<Json, py::list<py::str>> place(const Decoder& md, const Json& doc, const py::bytearray& code, int64_t start, int64_t end,
                                          const Fixups& fixups, const NameAt& name_at, std::optional<int64_t> seg,
                                          const std::tuple<int64_t, int64_t>& window) {
    return place(md, doc, code.view(), start, end, fixups, name_at, seg, window);
}

void _unused(const Json& doc) {
    Json body_j = get(doc, "body");
    Json body = truthy(body_j) ? body_j : Json::make_list(py::list<Json>());
    py::set<py::str> used;
    bool none_used = false;
    unused_names(body, used, none_used);
    py::list<py::str> asm_text;
    for (const Json& n : _walk(body)) {
        if (!is_str(get(n, "node"), "MachineFragment")) continue;
        Json a = get(n, "asm");
        py::list<py::str> parts;
        if (truthy(a)) {
            if (a.kind != Json::List) throw py::TypeError("asm is a list of lines");
            for (const Json& l : a.list) {
                if (l.kind != Json::Str) throw py::TypeError("sequence item: expected str instance");
                parts.append(l.s);
            }
        }
        py::str joined;
        for (int64_t q = 0; q < parts.size(); q++) joined += (q ? py::str(" ") : py::str("")) + parts[q];
        asm_text.append(joined);
    }
    py::str text;
    for (int64_t q = 0; q < asm_text.size(); q++) text += (q ? py::str(" ") : py::str("")) + asm_text[q];
    static const py::re::Pattern ident = py::re::compile(py::str("[A-Za-z_]\\w*"));
    for (const py::str& w : ident.findall(text)) used.add(w);
    // (the decompiler's `_x` is x joined with the word above it, a far pointer's halves: the back end
    // declares it in x's place, so x is used where `_x` is -- SUWIN 8:1704's _pcStack_10c)
    py::list<py::str> bare;
    for (const py::str& nm : used)
        if (!nm.empty() && nm.startswith(py::str("_"))) bare.append(nm.slice_from(1));
    for (const py::str& nm : bare) used.add(nm);
    if (body.kind != Json::List) {
        // (Python goes on to `lst[:] = [s for s in lst ...]` over the body: the keys of a dict and the characters
        // of a string have no .get; a number is not iterable)
        if (body.kind == Json::Dict || body.kind == Json::Str) throw py::AttributeError("object has no attribute 'get'");
        throw py::TypeError("object is not iterable");
    }
    py::list<py::list<Json>> all{body.list};
    all.extend(_lists(body));
    for (const py::list<Json>& lst : all) {
        std::vector<Json> kept;
        for (const Json& s : lst) {
            bool drop = false;
            if (is_str(get(s, "node"), "Local")) {
                Json nm = sym_name(s);
                std::optional<py::str> name = name_of(nm);
                drop = name ? !used.contains(*name) : !none_used;
            }
            if (!drop) kept.push_back(s);
        }
        lst.vec() = std::move(kept);
    }
}

}  // namespace codebreaker::decompile::asm_blocks
