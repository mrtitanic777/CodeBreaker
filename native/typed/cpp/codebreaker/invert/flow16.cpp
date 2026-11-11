// codebreaker/invert/flow16.cpp -- C++17 port of codebreaker/invert/flow16.py (see flow16.hpp).
//
// Build with native/invert's flow16.cc linked in (the model proper lives there) and native/l2/src/{pyre,pystr,value}.cc.

#include "codebreaker/invert/flow16.hpp"

#include <map>

#include "flow16.h"   // native/invert: cbinv::build_cfg, cbinv::structure, cbinv::PyError
#include "py_os.hpp"

namespace codebreaker::invert::flow16 {

SwitchHook SWITCH;
bool CMPSWITCH = false;

Block::Block(int64_t bid, int64_t lo_, int64_t hi_, const py::list<Insn>& insns_, int64_t addr_)
    : id(bid), lo(lo_), hi(hi_), insns(insns_), addr(addr_) {}

py::str Block::repr() const {
    int64_t last = insns.empty() ? addr : insns[-1].address;
    std::string succs = "[";
    for (int64_t i = 0; i < succ.size(); ++i) succs += (i ? ", " : "") + std::to_string(succ[i]);
    succs += "]";
    return py::percent(py::str("B%d[%05x..%05x %s->%s]"), id, addr, last, term ? *term : py::str("None"), py::str(succs));
}

namespace {

// The exception the model raised (native_invert's _RAISE table), as the Python exception type.
[[noreturn]] void raise_kind(int kind, const std::string& msg) {
    switch (kind) {
        case 1: throw py::IndexError(msg);
        case 2: throw py::ValueError(msg);
        case 4: throw py::KeyError(msg);
        case 5: throw py::AssertionError(msg);
        case 6: throw py::TypeError(msg);
        case 7: throw py::AttributeError(msg);
        default: throw py::RuntimeError(msg);
    }
}

template <class F> auto guarded(F f) -> decltype(f()) {
    try {
        return f();
    } catch (const cbinv::PyError& e) {
        raise_kind(e.kind, e.msg);
    } catch (const py::Exception&) {
        throw;
    } catch (const std::exception& e) {
        throw py::RuntimeError(e.what());
    }
}

// _switch_targets(ins): the SWITCH hook's answer for an indirect jmp (None: none)
std::optional<py::list<int64_t>> switch_targets(const Insn& ins) {
    if (!SWITCH || ins.mnemonic != py::str("jmp") || !ins.op_str.contains(py::str("["))) return std::nullopt;
    try {
        return SWITCH(ins.address);
    } catch (const std::exception&) {   // (py::Exception is one: any exception from the hook)
        return std::nullopt;
    }
}

std::optional<int64_t> opt(int v) {
    return v == cbinv::NONE ? std::nullopt : std::optional<int64_t>(v);
}

py::list<Node> node_list(const cbinv::Seq& s);

Node node_of(const cbinv::Node& n) {
    Node out;
    switch (n.kind) {
        case cbinv::Node::BLOCK:
            out.kind = py::str("block");
            out.bid = n.a;
            break;
        case cbinv::Node::BREAK: out.kind = py::str("break"); break;
        case cbinv::Node::CONTINUE: out.kind = py::str("continue"); break;
        case cbinv::Node::GOTO:
            out.kind = py::str("goto");
            out.target = n.a;
            break;
        case cbinv::Node::IF:
        case cbinv::Node::WHILE:
        case cbinv::Node::DOWHILE:
            out.kind = py::str(n.kind == cbinv::Node::IF ? "if" : n.kind == cbinv::Node::WHILE ? "while" : "dowhile");
            out.bid = n.a;
            out.body = node_list(n.s1);
            out.join = opt(n.join);
            out.neg = n.flag;
            break;
        case cbinv::Node::IFELSE:
            out.kind = py::str("ifelse");
            out.bid = n.a;
            out.body = node_list(n.s1);
            out.else_body = node_list(n.s2);
            out.join = opt(n.join);
            break;
        case cbinv::Node::LOOP:
            out.kind = py::str("loop");
            out.bid = n.a;
            out.body = node_list(n.s1);
            out.join = opt(n.join);
            break;
        case cbinv::Node::SWITCH:
            out.kind = py::str("switch");
            out.bid = n.a;
            for (const auto& c : n.cases) {
                py::list<CaseValue> vals;
                for (const cbinv::CaseVal& v : c.first)
                    vals.append(v.is_default ? CaseValue(py::str("default")) : CaseValue(static_cast<int64_t>(v.v)));
                out.arms.append(std::make_tuple(vals, node_list(c.second)));
            }
            out.join = opt(n.join);
            break;
    }
    return out;
}

py::list<Node> node_list(const cbinv::Seq& s) {
    py::list<Node> out;
    for (const cbinv::Node& n : s) out.append(node_of(n));
    return out;
}

}  // namespace

std::tuple<py::list<Block>, py::dict<int64_t, int64_t>, int64_t> build_cfg(const py::list<Insn>& insns, int64_t lo_addr, int64_t hi_addr) {
    (void)lo_addr;
    (void)hi_addr;
    if (insns.empty()) throw py::IndexError("list index out of range");
    std::vector<cbinv::Ins> rows;
    std::map<long long, std::vector<long long>> sw;
    for (const Insn& i : insns.vec()) rows.push_back({i.address, i.mnemonic.utf8(), i.op_str.utf8()});
    for (const Insn& i : insns.vec()) {
        if (i.mnemonic == py::str("jmp") && i.op_str.contains(py::str("["))) {
            std::optional<py::list<int64_t>> t = switch_targets(i);
            if (t) {
                std::vector<long long>& v = sw[i.address];
                v.clear();
                for (int64_t x : t->vec()) v.push_back(x);
            }
        }
    }
    std::vector<cbinv::Block> cfg = guarded([&] { return cbinv::build_cfg(rows, sw, CMPSWITCH); });
    py::list<Block> blocks;
    py::dict<int64_t, int64_t> idx2blk;
    int64_t bi = 0;
    for (const cbinv::Block& c : cfg) {
        py::list<Insn> body;
        for (int64_t k = c.lo; k < c.hi; ++k) body.append(insns[k]);
        Block b(bi, c.lo, c.hi, body, insns[c.lo].address);
        b.term = py::str(c.term);
        for (int s : c.succ) b.succ.append(s);
        b.cond_idx = opt(c.cond_idx);
        if (c.has_table) {
            py::list<int64_t> t;
            for (int x : c.table) t.append(x);
            b.table = t;
        }
        b.default_ = opt(c.def);
        if (!c.selreg.empty()) b.selreg = py::str(c.selreg);
        b.selbase = opt(c.selbase);
        if (c.has_cases) {
            py::list<std::tuple<int64_t, int64_t>> cs;
            for (const auto& vc : c.cases) cs.append(std::make_tuple(static_cast<int64_t>(vc.first), static_cast<int64_t>(vc.second)));
            b.cases = cs;
        }
        blocks.append(b);
        for (int64_t k = c.lo; k < c.hi; ++k) idx2blk[k] = bi;
        ++bi;
    }
    return std::make_tuple(blocks, idx2blk, int64_t(0));
}

py::list<Node> structure(const py::list<Block>& blocks, int64_t entry) {
    std::vector<cbinv::Block> rows;
    for (const Block& b : blocks.vec()) {
        cbinv::Block c;
        c.id = static_cast<int>(rows.size());
        c.term = b.term ? b.term->utf8() : std::string("None");
        c.lo = static_cast<int>(b.lo);
        c.hi = static_cast<int>(b.hi);
        for (int64_t s : b.succ.vec()) c.succ.push_back(static_cast<int>(s));
        if (b.cases) {
            c.has_cases = true;
            for (const auto& vc : b.cases->vec()) c.cases.push_back({std::get<0>(vc), static_cast<int>(std::get<1>(vc))});
        }
        if (b.default_) c.def = static_cast<int>(*b.default_);
        if (b.table) {
            c.has_table = true;
            for (int64_t t : b.table->vec()) c.table.push_back(static_cast<int>(t));
        }
        rows.push_back(c);
    }
    std::optional<py::str> nocap = py::os::environ_::get(py::str("CFG_NOCAP"));
    cbinv::Seq tree = guarded([&] { return cbinv::structure(rows, static_cast<int>(entry), nocap && !nocap->empty()); });
    return node_list(tree);
}

}  // namespace codebreaker::invert::flow16
