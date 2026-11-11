// codebreaker/invert/msc6_walk.cpp -- C++17 port of codebreaker/invert/msc6_walk.py (see msc6_walk.hpp).
//
// Build with native/invert's msc6*.cc, cs.cc, pyval.cc, api.cc and native/l2/src/{pyre,pystr,value,json}.cc linked in.

#include "codebreaker/invert/msc6_walk.hpp"

#include <filesystem>
#include <mutex>

#include "codebreaker/base/capstone_dll.hpp"
#include "codebreaker/invert/compilers.hpp"
#include "codebreaker/invert/msc6_function.hpp"
#include "cs.h"
#include "flow16.h"
#include "json.hpp"
#include "msc6.h"

namespace codebreaker::invert::msc6_walk {

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

std::vector<uint8_t> raw(const py::bytes& b) {
    std::vector<uint8_t> v(static_cast<size_t>(b.size()));
    for (int64_t i = 0; i < b.size(); ++i) v[static_cast<size_t>(i)] = static_cast<uint8_t>(b[i]);
    return v;
}

}  // namespace

Inverted invert(const py::bytes& code, const py::dict<int64_t, std::tuple<py::str, py::str>>& syms, const py::str& callee) {
    std::map<long long, std::pair<std::string, std::string>> table;
    for (const auto& kv : syms.items())
        table[std::get<0>(kv)] = {std::get<0>(std::get<1>(kv)).utf8(), std::get<1>(std::get<1>(kv)).utf8()};
    std::string json = guarded([&] { return cbinv::msc6_walk(raw(code), table, callee.utf8()); });
    py::V a = py::json_parse(json);
    Inverted out;
    if (a.get("t").is_none()) {
        out.why = py::str(a.get("w").str());
        return out;
    }
    out.text = py::str(a.get("t").str());
    const py::V& m = a.get("m");
    out.meta.stmts = m.get("stmts").as_int();
    for (const auto& kv : m.get("globals").items()) out.meta.globals[py::str(kv.first.str())] = py::str(kv.second.str());
    return out;
}

// regression: Cleanup must invert + verify through the generalized walker
int64_t main() {
    namespace ora = codebreaker::invert::compilers;
    py::bytes code = py::bytes::fromhex(py::str(
        "1e589045558bec1e8ed883ec02a146000b0644007411ff3646009ae70000002bc0a34600a344"
        "00a14a000b0648007411ff364a009a010100002bc0a34a00a34800a156000b0654007411ff3656"
        "009affff00002bc0a35600a354004d4d8be51f5d4dcb90"));
    py::dict<int64_t, std::tuple<py::str, py::str>> syms{{0x44, std::make_tuple(py::str("gpA"), py::str("long"))},
                                                          {0x48, std::make_tuple(py::str("gpB"), py::str("long"))},
                                                          {0x54, std::make_tuple(py::str("gpC"), py::str("long"))}};
    Inverted r = invert(code, syms, py::str("GlobalFree"));
    if (!r.text) {
        py::print(py::str("DECLINED:"), r.why);
        return 1;
    }
    py::print(*r.text);
    py::bytes rc = ora::mc6_compile(r.text->replace(py::str("void far F"), py::str("void far Cleanup")), py::str("/AM /G2 /Gsw /Ox"));
    bool same = msc6_function::_mask(rc) == msc6_function::_mask(code) && rc.size() == code.size();
    py::print(py::percent(py::str("\n// generalized walker: masked-equal = %s (%d stmts)"), py::str(same ? "True" : "False"), r.meta.stmts));
    return 0;
}

}  // namespace codebreaker::invert::msc6_walk
