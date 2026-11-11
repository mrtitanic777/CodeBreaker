// codebreaker/invert/msc6_registers.cpp -- C++17 port of codebreaker/invert/msc6_registers.py (see msc6_registers.hpp).
//
// Build with native/invert's msc6*.cc, cs.cc, pyval.cc, api.cc and native/l2/src/{pyre,pystr,value,json}.cc linked in.

#include "codebreaker/invert/msc6_registers.hpp"

#include <filesystem>
#include <mutex>

#include "codebreaker/base/capstone_dll.hpp"
#include "codebreaker/invert/compilers.hpp"
#include "codebreaker/invert/msc6_function.hpp"
#include "cs.h"
#include "flow16.h"
#include "json.hpp"
#include "msc6.h"

namespace codebreaker::invert::msc6_registers {

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

py::list<py::str> strs(const py::V& v) {
    py::list<py::str> out;
    for (const py::V& x : v.seq()) out.append(py::str(x.str()));
    return out;
}

Inverted answer(const std::string& json) {
    py::V a = py::json_parse(json);
    Inverted out;
    if (a.get("t").is_none()) {
        out.why = py::str(a.get("w").str());
        return out;
    }
    out.text = py::str(a.get("t").str());
    const py::V& m = a.get("m");
    if (m.has("saved")) out.meta.saved = strs(m.get("saved"));
    if (m.has("regvars")) out.meta.regvars = strs(m.get("regvars"));
    if (m.has("stmts")) out.meta.stmts = strs(m.get("stmts"));
    if (m.has("counter")) out.meta.counter = py::str(m.get("counter").str());
    if (m.has("limit")) out.meta.limit = py::str(m.get("limit").str());
    if (m.has("body_calls")) out.meta.body_calls = strs(m.get("body_calls"));
    return out;
}

std::map<long long, std::string> import_names(const py::dict<int64_t, py::str>& imports) {
    std::map<long long, std::string> m;
    for (const auto& kv : imports.items()) m[std::get<0>(kv)] = std::get<1>(kv).utf8();
    return m;
}

}  // namespace

Inverted invert_reg(const py::bytes& code, const py::dict<int64_t, py::str>& imports) {
    return answer(guarded([&] { return cbinv::msc6_invert_reg(code.raw(), import_names(imports)); }));
}

Inverted invert_loop(const py::bytes& code, const py::dict<int64_t, py::str>& imports) {
    return answer(guarded([&] { return cbinv::msc6_invert_loop(code.raw(), import_names(imports)); }));
}

void _selftest() {
    namespace ora = codebreaker::invert::compilers;
    py::bytes code = py::bytes::fromhex(py::str(
        "1e589045558bec1e8ed883ec02568b7606569a00000000508bf09a000000008bc65e4d4d8be51f5d4dca0200"));
    py::dict<int64_t, py::str> imports{{0, py::str("G")}, {1, py::str("H")}};
    Inverted r = invert_reg(code, imports);
    if (!r.text) {
        py::print(py::str("DECLINED:"), r.why);
        return;
    }
    py::print(*r.text);
    py::bytes rc = ora::mc6_compile(*r.text, py::str("/AM /G2 /Gsw /Ox"));
    bool same = msc6_function::_mask(rc) == msc6_function::_mask(code) && rc.size() == code.size();
    py::print(py::percent(py::str("\n// register-var tracker: masked-equal = %s (%d vs %d B)"), py::str(same ? "True" : "False"), rc.size(), code.size()));
}

}  // namespace codebreaker::invert::msc6_registers
