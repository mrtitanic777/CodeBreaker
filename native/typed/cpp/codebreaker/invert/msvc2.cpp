// codebreaker/invert/msvc2.cpp -- C++17 port of codebreaker/invert/msvc2.py (see msvc2.hpp).
//
// Build with native/invert's msvc2.cc, pyval.cc, cs.cc, api.cc and native/l2/src/{pyre,pystr,value,json}.cc linked in.

#include "codebreaker/invert/msvc2.hpp"

#include <filesystem>
#include <mutex>
#include <set>

#include "codebreaker/base/capstone_dll.hpp"
#include "cs.h"
#include "flow16.h"
#include "msvc2.h"
#include "pyval.h"

namespace codebreaker::invert::msvc2 {

namespace M = codebreaker::invert::msvc2_forward;

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

// the model's pyval -> the typed tree (native_invert._dec): a tuple is an Expr, a list a list of them
M::ExprPtr dec_expr(const cbinv::V& v);

M::Arg dec_arg(const cbinv::V& v) {
    switch (v.k) {
        case cbinv::V::NONE: return M::Arg(std::monostate());
        case cbinv::V::BOOL: return M::Arg(v.i != 0);
        case cbinv::V::INT: return M::Arg(static_cast<int64_t>(v.i));
        case cbinv::V::STR: return M::Arg(py::str(v.s));
        case cbinv::V::TUP: return M::Arg(dec_expr(v));
        case cbinv::V::LIST: {
            py::list<M::ExprPtr> l;
            for (const cbinv::V& x : v.a) l.append(dec_expr(x));
            return M::Arg(l);
        }
        default: throw py::TypeError("a dict inside a construct");
    }
}

M::ExprPtr dec_expr(const cbinv::V& v) {
    auto e = std::make_shared<M::Expr>();
    if (!v.a.empty() && v.a[0].k == cbinv::V::STR) e->tag = py::str(v.a[0].s);
    for (size_t i = 1; i < v.a.size(); ++i) e->args.push_back(dec_arg(v.a[i]));
    return e;
}

M::Node dec_node(const cbinv::V& d) {
    M::Node n;
    if (const cbinv::V* p = d.find("nargs")) n.nargs = p->i;
    if (const cbinv::V* p = d.find("expr")) {
        if (p->none()) n.expr_none = true;
        else n.expr = dec_expr(*p);
    }
    if (const cbinv::V* p = d.find("stmts")) {
        py::list<M::ExprPtr> l;
        for (const cbinv::V& x : p->a) l.append(dec_expr(x));
        n.stmts = l;
    }
    if (const cbinv::V* p = d.find("ret")) {
        if (p->none()) n.ret_none = true;
        else n.ret = dec_expr(*p);
    }
    if (const cbinv::V* p = d.find("leave")) n.leave = p->i != 0;
    if (const cbinv::V* p = d.find("fpo")) n.fpo = p->i != 0;
    if (const cbinv::V* p = d.find("cc")) n.cc = py::str(p->s);
    return n;
}

}  // namespace

std::optional<Inverted> invert(const py::bytes& code, const std::optional<py::set<int64_t>>& mask) {
    std::string in = "C\t" + code.hex().utf8() + "\n";
    if (mask) {
        std::vector<int64_t> ms;
        for (int64_t m : *mask) ms.push_back(m);   // (a set: its own order, as the Python's ",".join over the set)
        std::string list;
        for (size_t i = 0; i < ms.size(); ++i) list += (i ? "," : "") + std::to_string(ms[i]);
        in += "M\t" + list + "\n";
    }
    cbinv::V v = cbinv::from_json(guarded([&] { return cbinv::msvc2_call(in, 0); }));
    if (v.none()) return std::nullopt;
    Inverted r;
    r.nargs = v.at("nargs").i;
    if (!v.at("expr").none()) r.expr = dec_expr(v.at("expr"));
    r.c = py::str(v.at("c").s);
    r.node = dec_node(v.at("node"));
    return r;
}

namespace {
bool matches(const std::optional<py::bytes>& pred, const py::bytes& code, const std::set<int64_t>& mask) {
    if (!pred || pred->size() != code.size()) return false;
    for (int64_t i = 0; i < code.size(); ++i)
        if (!mask.count(i) && (*pred)[i] != code[i]) return false;
    return true;
}
}  // namespace

bool selftest(bool show) {
    compilers::Msvc2 cc32 = M::_oracle();
    int64_t ok = 0, tot = 0;
    for (const auto& c : M::battery().vec()) {
        const py::str& name = std::get<0>(c);
        const M::Node& node = std::get<1>(c);
        ++tot;
        compilers::CompileResult r = cc32.compile_c(M::to_c(node), py::str("/c /O1 /Oy-"));
        std::optional<std::tuple<py::bytes, py::list<int64_t>>> fn;
        if (r.obj)
            for (const auto& kv : compilers::Obj(*r.obj).functions().items())
                if (std::get<0>(kv).startswith(py::str("_f"))) {
                    fn = std::get<1>(kv);
                    break;
                }
        if (!fn) {
            py::print(py::percent(py::str("  %-8s ORACLE-FAIL"), name));
            continue;
        }
        py::bytes real = std::get<0>(*fn);
        py::set<int64_t> mask;
        std::set<int64_t> plain;
        for (int64_t m : std::get<1>(*fn).vec()) {
            mask.add(m);
            plain.insert(m);
        }
        std::optional<Inverted> inv = invert(real, mask);
        bool good = inv.has_value() && matches(M::emit(inv->node), real, plain);   // re-emit the full node
        ok += good;
        if (show || !good) py::print(py::percent(py::str("  %-8s %s  %s"), name, py::str(good ? "OK " : "MISS"), inv ? inv->c : py::str("(declined)")));
    }
    py::print(py::percent(py::str("\nmsvc2inv round-trip: %d/%d bytes->C->byte-exact  (MSVC 2.0 /O1 /Oy-)"), ok, tot));
    return ok == tot;
}

int64_t main(const py::list<py::str>& argv) {
    bool show = false;
    for (int64_t i = 1; i < argv.size(); ++i)
        if (argv[i] == py::str("--show")) show = true;
    return selftest(show) ? 0 : 1;
}

}  // namespace codebreaker::invert::msvc2
