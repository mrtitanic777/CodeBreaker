// codebreaker/invert/msvc2_forward.cpp -- C++17 port of codebreaker/invert/msvc2_forward.py (see msvc2_forward.hpp).
//
// Build with native/invert's msvc2.cc, pyval.cc, cs.cc, api.cc and native/l2/src/{pyre,pystr,value,json}.cc linked in.

#include "codebreaker/invert/msvc2_forward.hpp"

#include <algorithm>
#include <filesystem>
#include <mutex>
#include <set>

#include "codebreaker/base/capstone_dll.hpp"
#include "cs.h"
#include "flow16.h"
#include "msvc2.h"
#include "pyval.h"

namespace codebreaker::invert::msvc2_forward {

const py::set<py::str> OPS = {py::str("+"), py::str("-"), py::str("&"), py::str("|"), py::str("^"), py::str("*")};

namespace {

void ensure_capstone() {
    static std::once_flag once;
    std::call_once(once, [] {
        std::string err;
        std::wstring dll = std::filesystem::u8path(base::capstone_dll::find().utf8()).wstring();
        if (!cbinv::cs_load(dll.c_str(), err)) throw py::OSError(err);
    });
}

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
    ensure_capstone();
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

// ---- the typed tree as the model's pyval (native_invert._enc): a tuple an array, a list {"L": [...]}, a dict {"D": [...]}
cbinv::V enc_expr(const ExprPtr& e);

cbinv::V enc_arg(const Arg& a) {
    struct Visit {
        cbinv::V operator()(std::monostate) const { return cbinv::V(); }
        cbinv::V operator()(bool b) const { return cbinv::Bool(b); }
        cbinv::V operator()(int64_t i) const { return cbinv::I(i); }
        cbinv::V operator()(const py::str& s) const { return cbinv::S(s.utf8()); }
        cbinv::V operator()(const ExprPtr& e) const { return enc_expr(e); }
        cbinv::V operator()(const py::list<ExprPtr>& l) const {
            std::vector<cbinv::V> items;
            for (const ExprPtr& e : l.vec()) items.push_back(enc_expr(e));
            return cbinv::L(items);
        }
    };
    return std::visit(Visit(), a);
}

cbinv::V enc_expr(const ExprPtr& e) {
    cbinv::V t;
    t.k = cbinv::V::TUP;
    t.a.push_back(cbinv::S(e->tag.utf8()));
    for (const Arg& a : e->args) t.a.push_back(enc_arg(a));
    return t;
}

cbinv::V enc_node(const Node& n) {
    cbinv::V d = cbinv::D();
    if (n.nargs) d.set("nargs", cbinv::I(*n.nargs));
    if (n.expr) d.set("expr", enc_expr(*n.expr));
    else if (n.expr_none) d.set("expr", cbinv::V());
    if (n.stmts) {
        std::vector<cbinv::V> items;
        for (const ExprPtr& e : n.stmts->vec()) items.push_back(enc_expr(e));
        d.set("stmts", cbinv::L(items));
    }
    if (n.ret) d.set("ret", enc_expr(*n.ret));
    else if (n.ret_none) d.set("ret", cbinv::V());
    if (n.leave) d.set("leave", cbinv::Bool(*n.leave));
    if (n.fpo) d.set("fpo", cbinv::Bool(*n.fpo));
    if (n.cc) d.set("cc", cbinv::S(n.cc->utf8()));
    return d;
}

std::string ask(const std::string& in, int which) { return guarded([&] { return cbinv::msvc2_call(in, which); }); }

}  // namespace

std::optional<py::bytes> emit(const Node& node) {
    std::string h = ask(cbinv::to_json(enc_node(node)), 2);
    cbinv::V v = cbinv::from_json(h);
    if (v.none()) return std::nullopt;
    return py::bytes::fromhex(py::str(v.s));
}

py::str to_c(const Node& node) { return py::str(cbinv::from_json(ask(cbinv::to_json(enc_node(node)), 3)).s); }

std::tuple<int64_t, py::dict<int64_t, int64_t>> frame_layout(const py::list<std::tuple<int64_t, bool>>& locals_) {
    std::vector<cbinv::V> items;
    for (const auto& l : locals_.vec()) items.push_back(cbinv::T({cbinv::I(std::get<0>(l)), cbinv::Bool(std::get<1>(l))}));
    cbinv::V r = cbinv::from_json(ask(cbinv::to_json(cbinv::L(items)), 4));
    py::dict<int64_t, int64_t> offs;
    for (const cbinv::V& p : r[1].a) offs[p[0].i] = p[1].i;
    return std::make_tuple(static_cast<int64_t>(r[0].i), offs);
}

// ---------------------------------------------------------------------------------------------------- the battery
namespace {

ExprPtr T(const char* tag, std::vector<Arg> args = {}) { return std::make_shared<Expr>(Expr{py::str(tag), std::move(args)}); }
Arg I(int64_t v) { return Arg(v); }
Arg S(const char* s) { return Arg(py::str(s)); }
Arg E(const ExprPtr& e) { return Arg(e); }
Arg L(std::initializer_list<ExprPtr> es) { return Arg(py::list<ExprPtr>(std::vector<ExprPtr>(es))); }
ExprPtr A(int64_t i) { return T("arg", {I(i)}); }
ExprPtr C(int64_t v) { return T("const", {I(v)}); }
ExprPtr BIN(const char* op, const ExprPtr& l, const ExprPtr& r) { return T("binop", {S(op), E(l), E(r)}); }
Node N(int64_t nargs, const ExprPtr& expr) {
    Node n;
    n.nargs = nargs;
    n.expr = expr;
    return n;
}
using Case = std::tuple<py::str, Node>;

ExprPtr chain(const char* op, std::initializer_list<int64_t> idxs) {
    auto it = idxs.begin();
    ExprPtr e = A(*it++);
    for (; it != idxs.end(); ++it) e = BIN(op, e, A(*it));
    return e;
}

}  // namespace

py::list<std::tuple<py::str, Node>> battery() {
    py::list<Case> cases;
    auto add = [&](const std::string& name, const Node& n) { cases.append(Case(py::str(name), n)); };
    add("ret0", N(0, C(0)));
    add("ret1", N(0, C(1)));
    add("retBig", N(0, C(0x1234)));
    add("retNeg", N(0, C(-7)));
    add("arg1", N(1, A(1)));
    add("arg2of2", N(2, A(2)));
    for (const char* op : {"+", "-", "&", "|", "^", "*"}) {
        add(std::string("a") + op + "c", N(1, BIN(op, A(1), C(5))));
        add(std::string("a") + op + "b", N(2, BIN(op, A(1), A(2))));
    }
    add("a*b*c", N(3, chain("*", {1, 2, 3})));
    add("a*b*c*d", N(4, chain("*", {1, 2, 3, 4})));
    add("a+b+c", N(3, BIN("+", BIN("+", A(1), A(2)), A(3))));
    add("a+b-c", N(3, BIN("-", BIN("+", A(1), A(2)), A(3))));
    add("a-b+c", N(3, BIN("+", BIN("-", A(1), A(2)), A(3))));
    add("a*b+c", N(3, BIN("+", BIN("*", A(1), A(2)), A(3))));
    add("a+b*c", N(3, BIN("+", A(1), BIN("*", A(2), A(3)))));
    add("*p", N(1, T("deref", {E(A(1))})));
    add("p[2]", N(1, T("index", {E(A(1)), E(C(2))})));
    add("p[0]", N(1, T("index", {E(A(1)), E(C(0))})));
    for (const char* o : {"<", "<=", ">", ">=", "==", "!="}) add(std::string("a") + o + "b", N(2, T("cmp", {S(o), E(A(1)), E(A(2))})));
    add("a+1", N(1, BIN("+", A(1), C(1))));
    add("a-1", N(1, BIN("-", A(1), C(1))));
    add("store", N(2, T("store", {E(A(1)), E(A(2))})));
    add("store0", N(1, T("store", {E(A(1)), E(C(0))})));
    add("a-b-c", N(3, chain("-", {1, 2, 3})));
    add("a-b-c-d", N(4, chain("-", {1, 2, 3, 4})));
    add("a-b-c-d-e", N(5, chain("-", {1, 2, 3, 4, 5})));
    add("p[i]", N(2, T("index", {E(A(1)), E(A(2))})));
    add("a+b+c+d", N(4, chain("+", {1, 2, 3, 4})));
    add("a+b+c+d+e", N(5, chain("+", {1, 2, 3, 4, 5})));
    add("(a+b)*c", N(3, BIN("*", BIN("+", A(1), A(2)), A(3))));
    add("a*b+c*d", N(4, BIN("+", BIN("*", A(1), A(2)), BIN("*", A(3), A(4)))));
    add("a&b&c", N(3, chain("&", {1, 2, 3})));
    add("a*b-c*d", N(4, BIN("-", BIN("*", A(1), A(2)), BIN("*", A(3), A(4)))));
    add("g()", N(0, T("call", {S("__stdcall"), L({})})));
    add("g(a)", N(1, T("call", {S("__stdcall"), L({A(1)})})));
    add("g(a,b)", N(2, T("call", {S("__stdcall"), L({A(1), A(2)})})));
    add("g(a,b,c)", N(3, T("call", {S("__stdcall"), L({A(1), A(2), A(3)})})));
    add("g_cdecl(a)", N(1, T("call", {S("__cdecl"), L({A(1)})})));
    add("getG", N(0, T("gread", {S("G")})));
    add("setGc", N(0, T("gwrite", {S("G"), E(C(5))})));
    add("setG", N(1, T("gwrite", {S("G"), E(A(1))})));
    add("incG", N(0, T("gincdec", {S("G"), I(1)})));
    add("P(a)", N(1, T("icall", {S("__stdcall"), L({A(1)})})));
    add("P(a,b)", N(2, T("icall", {S("__stdcall"), L({A(1), A(2)})})));
    add("empty0", N(0, T("empty")));
    add("empty1", N(1, T("empty")));
    add("empty2", N(2, T("empty")));
    add("neg", N(1, T("unary", {S("neg"), E(A(1))})));
    add("not", N(1, T("unary", {S("not"), E(A(1))})));
    add("lognot", N(1, T("unary", {S("lognot"), E(A(1))})));
    add("a*a", N(1, BIN("*", A(1), A(1))));
    add("g(a)+b", N(2, T("callbin", {S("+"), S("__stdcall"), L({A(1)}), E(A(2))})));
    add("g(a)+c", N(1, T("callbin", {S("+"), S("__stdcall"), L({A(1)}), E(C(1))})));
    add("g(a,b)&c", N(2, T("callbin", {S("&"), S("__stdcall"), L({A(1), A(2)}), E(C(7))})));
    return cases;
}

py::list<std::tuple<py::str, Node>> declined_battery() {
    return py::list<Case>{Case(py::str("a/b"), N(2, BIN("/", A(1), A(2)))), Case(py::str("a<<b"), N(2, BIN("<<", A(1), A(2))))};
}

// ---------------------------------------------------------------------------------------------------- frame layout
py::list<std::tuple<py::str, py::str, py::list<std::tuple<int64_t, bool>>>> _frame_battery() {
    using Locals = py::list<std::tuple<int64_t, bool>>;
    using Row = std::tuple<py::str, py::str, Locals>;
    const std::string G = "extern void __stdcall g(void*); ";
    auto row = [&](const char* name, const std::string& src, std::initializer_list<std::tuple<int64_t, bool>> l) {
        return Row(py::str(name), py::str(G + src), Locals(std::vector<std::tuple<int64_t, bool>>(l)));
    };
    return py::list<Row>{
        row("1int", "int __stdcall f(void){ int a; g(&a); return a; }", {{4, false}}),
        row("2int", "int __stdcall f(void){ int a,b; g(&a); g(&b); return a+b; }", {{4, false}, {4, false}}),
        row("3int", "int __stdcall f(void){ int a,b,c; g(&a); g(&b); g(&c); return a+b+c; }", {{4, false}, {4, false}, {4, false}}),
        row("2int_rev", "int __stdcall f(void){ int a,b; g(&b); g(&a); return a+b; }", {{4, false}, {4, false}}),
        row("buf8", "int __stdcall f(void){ char buf[8]; g(buf); return 0; }", {{8, true}}),
        row("buf10", "int __stdcall f(void){ char buf[10]; g(buf); return 0; }", {{10, true}}),
        row("int_buf", "int __stdcall f(void){ int a; char buf[8]; g(&a); g(buf); return a; }", {{4, false}, {8, true}}),
        row("buf_int", "int __stdcall f(void){ char buf[6]; int a; g(buf); g(&a); return a; }", {{6, true}, {4, false}}),
    };
}

std::tuple<int64_t, py::list<int64_t>> _extract_frame(const py::bytes& code) {
    ensure_capstone();
    int64_t fs = 0;
    std::set<int64_t> offs;
    py::re::Pattern pat = py::re::compile(py::str("\\[ebp - (0x[0-9a-f]+|\\d+)\\]"));
    for (const cbinv::CsIns& ins : cbinv::disasm32(reinterpret_cast<const uint8_t*>(code.raw().data()), code.raw().size(), 0)) {
        py::str op(ins.op_str);
        if (ins.mnemonic == "sub" && op.startswith(py::str("esp,"))) fs = py::int_parse(py::split(op, py::str(","))[1], 0);
        for (const py::str& x : pat.findall(op).vec()) offs.insert(-py::int_parse(x, 0));
    }
    py::list<int64_t> sorted_offs;
    for (int64_t o : offs) sorted_offs.append(o);
    return std::make_tuple(fs, sorted_offs);
}

// ---------------------------------------------------------------------------------------------------- the self-tests
namespace {

py::str ints_repr(const py::list<int64_t>& v) {
    std::string s = "[";
    for (int64_t i = 0; i < v.size(); ++i) s += (i ? ", " : "") + std::to_string(v[i]);
    return py::str(s + "]");
}

// the compiled function "_f..." of an object: its bytes and the positions the linker fills in
std::optional<std::tuple<py::bytes, py::list<int64_t>>> compiled_f(const compilers::Obj& obj) {
    for (const auto& kv : obj.functions().items())
        if (std::get<0>(kv).startswith(py::str("_f"))) return std::get<1>(kv);
    return std::nullopt;
}

}  // namespace

compilers::Msvc2 _oracle() { return compilers::msvc2(); }

bool frame_selftest(bool show) {
    compilers::Msvc2 cc32 = _oracle();
    int64_t ok = 0, tot = 0;
    for (const auto& row : _frame_battery().vec()) {
        const py::str& name = std::get<0>(row);
        ++tot;
        auto [pred_fs, pred_off] = frame_layout(std::get<2>(row));
        std::set<int64_t> pset;
        for (const auto& kv : pred_off.items()) pset.insert(std::get<1>(kv));
        py::list<int64_t> pred_set;
        for (int64_t v : pset) pred_set.append(v);
        compilers::CompileResult r = cc32.compile_c(std::get<1>(row), py::str("/c /O1 /Oy-"));
        if (!r.obj) {
            py::print(py::percent(py::str("  %-8s ORACLE-FAIL"), name));
            continue;
        }
        auto fn = compiled_f(compilers::Obj(*r.obj));
        int64_t real_fs = -1;
        py::list<int64_t> real_off;
        if (fn) std::tie(real_fs, real_off) = _extract_frame(std::get<0>(*fn));
        bool match = pred_fs == real_fs && pred_set.vec() == real_off.vec();
        ok += match;
        if (show || !match)
            py::print(py::percent(py::str("  %-8s %s  pred(fs=%d,off=%s) real(fs=%d,off=%s)"), name, py::str(match ? "OK " : "MISS"), pred_fs,
                                  ints_repr(pred_set), real_fs, ints_repr(real_off)));
    }
    py::print(py::percent(py::str("\nmsvc2 frame-layout self-test: %d/%d byte-exact  (MSVC 2.0 /O1 /Oy-)"), ok, tot));
    return ok == tot;
}

bool selftest(bool show) {
    compilers::Msvc2 cc32 = _oracle();
    int64_t ok = 0, tot = 0;
    for (const auto& c : battery().vec()) {
        const py::str& name = std::get<0>(c);
        const Node& node = std::get<1>(c);
        std::optional<py::bytes> pred = emit(node);
        ++tot;
        compilers::CompileResult r = cc32.compile_c(to_c(node), py::str("/c /O1 /Oy-"));
        if (!r.obj) {
            py::str e = r.err.value_or(py::str(""));
            py::print(py::percent(py::str("  %-8s ORACLE-FAIL %s"), name, e.slice(0, 80)));
            continue;
        }
        auto fn = compiled_f(compilers::Obj(*r.obj));
        py::bytes real = fn ? std::get<0>(*fn) : py::bytes(std::string());
        std::set<int64_t> mask;
        if (fn)
            for (int64_t m : std::get<1>(*fn).vec()) mask.insert(m);
        bool match = pred.has_value() && pred->size() == real.size();
        if (match)
            for (int64_t i = 0; i < real.size(); ++i)
                if (!mask.count(i) && (*pred)[i] != real[i]) match = false;
        ok += match;
        if (show || !match) {
            const char* tag = match ? "OK " : "MISS";
            py::print(py::percent(py::str("  %-8s %s  pred=%s  real=%s"), name, py::str(tag),
                                  (pred && pred->size()) ? pred->hex(py::str(" ")) : py::str("(declined)"), real.hex(py::str(" "))));
        }
    }
    py::print(py::percent(py::str("\nmsvc2model self-test: %d/%d byte-exact  (MSVC 2.0 /O1 /Oy-)"), ok, tot));
    int64_t dec_ok = 0, dec_tot = 0;
    for (const auto& c : declined_battery().vec()) {
        ++dec_tot;
        bool d = !emit(std::get<1>(c)).has_value();
        dec_ok += d;
        if (show || !d) py::print(py::percent(py::str("  declined %-8s %s"), std::get<0>(c), py::str(d ? "OK (None)" : "LEAK: emitted bytes!")));
    }
    py::print(py::percent(py::str("declined-correctly: %d/%d  (unmodeled shapes return None, never mis-modeled)"), dec_ok, dec_tot));
    return ok == tot && dec_ok == dec_tot;
}

int64_t main(const py::list<py::str>& argv) {
    bool show = false, frame = false;
    for (int64_t i = 1; i < argv.size(); ++i) {
        if (argv[i] == py::str("--show")) show = true;
        if (argv[i] == py::str("--frame")) frame = true;
    }
    bool ok = frame ? frame_selftest(show) : selftest(show);
    return ok ? 0 : 1;
}

}  // namespace codebreaker::invert::msvc2_forward
