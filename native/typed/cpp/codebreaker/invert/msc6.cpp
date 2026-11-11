// codebreaker/invert/msc6.cpp -- C++17 port of codebreaker/invert/msc6.py (see msc6.hpp).
//
// Build with native/invert's msc6*.cc, cs.cc, pyval.cc, api.cc and native/l2/src/{pyre,pystr,value,json}.cc linked in.

#include "codebreaker/invert/msc6.hpp"

#include <filesystem>
#include <mutex>

#include "codebreaker/base/capstone_dll.hpp"
#include "codebreaker/invert/compilers.hpp"
#include "codebreaker/invert/msc6_function.hpp"
#include "cs.h"
#include "flow16.h"
#include "json.hpp"
#include "msc6.h"

namespace codebreaker::invert::msc6 {

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

}  // namespace

Inverted invert(const py::bytes& code, const py::dict<int64_t, py::str>& imports,
                const std::optional<py::dict<int64_t, std::tuple<py::str, py::str>>>& syms, const py::str& rtype,
                const std::optional<py::list<std::tuple<py::str, py::str>>>& params,
                const std::optional<py::dict<int64_t, py::str>>& nearmap, int64_t func_base) {
    cbinv::Msc6In in;
    in.code.assign(code.raw().begin(), code.raw().end());
    for (const auto& kv : imports.items()) in.imports[std::get<0>(kv)] = std::get<1>(kv).utf8();
    if (syms)
        for (const auto& kv : syms->items())
            in.syms[std::get<0>(kv)] = {std::get<0>(std::get<1>(kv)).utf8(), std::get<1>(std::get<1>(kv)).utf8()};
    in.rtype = rtype.utf8();
    if (params) {
        cbinv::Params ps;
        for (const auto& p : params->vec()) ps.push_back({std::get<0>(p).utf8(), std::get<1>(p).utf8()});
        in.params = ps;
    }
    if (nearmap)
        for (const auto& kv : nearmap->items()) in.nearmap[std::get<0>(kv)] = std::get<1>(kv).utf8();
    in.func_base = func_base;
    std::string json = guarded([&] { return cbinv::msc6_invert(in); });
    py::V a = py::json_parse(json);
    Inverted out;
    if (a.get("t").is_none()) {
        out.why = py::str(a.get("w").str());
        return out;
    }
    out.text = py::str(a.get("t").str());
    const py::V& m = a.get("m");
    out.meta.calls = m.get("calls").as_int();
    out.meta.regvars = strs(m.get("regvars"));
    for (const auto& kv : m.get("globals").items()) out.meta.globals[py::str(kv.first.str())] = py::str(kv.second.str());
    out.meta.cdecl_ = m.get("cdecl").as_int() != 0;
    out.meta.has_nearcall = m.get("has_nearcall").as_int() != 0;
    out.meta.nears = strs(m.get("nears"));
    return out;
}

namespace {
py::str tf(bool b) { return py::str(b ? "True" : "False"); }
using Mask = py::bytes (*)(const py::bytes&);
}  // namespace

void _selftest() {
    namespace ora = codebreaker::invert::compilers;
    using msc6_function::_mask;
    const py::str flags("/AM /G2 /Gsw /Ox");
    py::bytes code = py::bytes::fromhex(py::str(
        "1e589045558bec1e8ed883ec0256ff76069a000000008bf00bf675042bc0eb08569a000000008bc65e4d4d8be51f5d4dca020090"));
    Inverted r = invert(code, py::dict<int64_t, py::str>{{0, py::str("G")}, {1, py::str("H")}});
    if (!r.text) {
        py::print(py::str("DECLINED:"), r.why);
        return;
    }
    py::print(*r.text);
    py::bytes rc = ora::mc6_compile(*r.text, flags);
    py::print(py::percent(py::str("\n// INTEGRATION driver: masked-equal = %s (%d vs %d B)"), tf(_mask(rc) == _mask(code) && rc.size() == code.size()),
                          rc.size(), code.size()));
    // far-pointer battery: branch, deref load/pass/store (pointee width from usage)
    struct Far {
        const char* label;
        const char* hex;
        const char* rt;
        std::vector<std::pair<const char*, const char*>> params;
    };
    const std::vector<Far> far = {
        {"fptr-branch", "1e589045558bec1e8ed883ec028b4e068b46080bc175052bc0eb0b908b460850519a000000004d4d8be51f5d4dca0400", "int", {{"name", "far"}}},
        {"deref-return", "1e589045558bec1e8ed883ec02c45e06268b074d4d8be51f5d4dca040090", "int", {{"p", "far"}}},
        {"deref-pass", "1e589045558bec1e8ed883ec02c45e0626ff379a000000004d4d8be51f5d4dca0400", "int", {{"p", "far"}}},
        {"deref-store", "1e589045558bec1e8ed883ec028b4606c45e082689074d4d8be51f5d4dca0600", "void", {{"p", "far"}, {"v", "int"}}},
        {"dword-shl4", "1e589045558bec1e8ed883ec028b46068b5608d1e0d1d2d1e0d1d2d1e0d1d2d1e0d1d24d4d8be51f5d4dca040090", "long", {{"a", "long"}}},
        {"dword-add", "1e589045558bec1e8ed883ec028b46068b560803460a13560c4d4d8be51f5d4dca080090", "long", {{"a", "long"}, {"b", "long"}}},
        {"dword-frword", "1e589045558bec1e8ed883ec028b56062bc04d4d8be51f5d4dca0200", "long", {{"a", "int"}}},
        // short-circuit OR-of-ANDs converging on a JOIN: (f==1&&g==0)||(f==2&&g==0)||(f==3&&g==0)
        {"shortcircuit-or-and",
         "1e589045558bec1e8ed883ec028b4e088b560683f90175040bd2741283f90275040bd2740983f903750c0bd27508519a00000000eb03b80d004d4d8be51f5d4dca040090",
         "int", {{"f", "int"}, {"g", "int"}}},
        // materialized boolean: MSC builds (a||b) into a 0/1 register then tests it (olecli sub_28aa)
        {"bool-materialize",
         "1e589045558bec1e8ed883ec02c45e0626837f7f00750b26837f7d0075042bd2eb03ba01000bd274052bc0eb0490b805004d4d8be51f5d4dca040090", "int",
         {{"p", "far"}}},
    };
    for (const Far& f : far) {
        py::bytes c = py::bytes::fromhex(py::str(f.hex));
        py::list<std::tuple<py::str, py::str>> pms;
        for (const auto& p : f.params) pms.append(std::make_tuple(py::str(p.first), py::str(p.second)));
        Inverted s2 = invert(c, py::dict<int64_t, py::str>{{0, py::str("G")}}, std::nullopt, py::str(f.rt), pms);
        if (!s2.text) {
            py::print(py::percent(py::str("// %s: DECLINED"), py::str(f.label)));
            continue;
        }
        py::bytes r2 = ora::mc6_compile(*s2.text, flags);
        py::print(py::percent(py::str("// %-12s masked-equal = %s (%d vs %d B)"), py::str(f.label), tf(_mask(r2) == _mask(c) && r2.size() == c.size()),
                              r2.size(), c.size()));
    }
    // real retail LZEXPAND functions (cdecl + param-inference + cmp-consumes-call);
    // padding-tolerant compare since a standalone recompile appends an alignment nop.
    struct Real {
        const char* label;
        const char* hex;
        py::dict<int64_t, py::str> imp;
        std::optional<py::dict<int64_t, std::tuple<py::str, py::str>>> sy;
    };
    using Sy = py::dict<int64_t, std::tuple<py::str, py::str>>;
    const std::vector<Real> real = {
        {"retail-cdecl(seg11)",
         "1e589045558bec1e8ed883ec02568b760cff7606ff760aff7608569a210500003bc67506b8ffffeb03902bc05e4d4d8be51f5d4dcb", {{0, py::str("G")}}, std::nullopt},
        {"retail-wstore(seg10)", "1e589045558bec1e8ed883ec02b8ffffa382004d4d8be51f5d4dcb", {}, std::nullopt},
        {"retail-free(seg11)",
         "1e589045558bec1e8ed883ec02833e7e00007420ff367e009affff0000ff367e009affff0000c7067e0000002bc0a3ee00a3ec00c606800000"
         "2bc0a3ee00a3ec00a3f200a3f000a3f600a3f400a3fa00a3f800a3fe00a3fc00a30201a300014d4d8be51f5d4dcb",
         {{0, py::str("G")}, {1, py::str("H")}}, Sy{{0xec, std::make_tuple(py::str("gL_ec"), py::str("long"))}}},
        {"retail-cdecl2(seg12)",
         "1e589045558bec1e8ed883ec029affff0000833e8c01007412ff368c019a9b09000083c402c7068c010000833e8a01007412ff368a019affff000083c402c7068a010000"
         "833e8201007420ff3682019af4060000ff3682019afd060000c706820100002bc0a39801a396012bc0a38401a386014d4d8be51f5d4dcb",
         {}, Sy{{0x196, std::make_tuple(py::str("gL_196"), py::str("long"))}, {0x186, std::make_tuple(py::str("gL_186"), py::str("long"))}}},
        {"retail-addrglob(seg10)",
         "1e589045558bec1e8ed883ec02688203688003ff76069affff000083c4060bc07406b8ffffeb2090ff368203ff368003ff76089affff000083c4060bc07405b8feffeb03b801004d4d8be51f5d4dcb",
         {{0, py::str("G0")}, {1, py::str("G1")}}, std::nullopt},
    };
    auto rstrip00 = [](const py::bytes& b) { return b.rstrip(py::bytes(std::string("\x90\x00", 2))); };
    for (const Real& f : real) {
        py::bytes c = py::bytes::fromhex(py::str(f.hex));
        Inverted s3 = invert(c, f.imp, f.sy);
        if (!s3.text) {
            py::print(py::percent(py::str("// %s: DECLINED"), py::str(f.label)));
            continue;
        }
        py::bytes r3 = ora::mc6_compile(*s3.text, flags);
        bool eq = rstrip00(_mask(r3)) == rstrip00(_mask(c));
        py::print(py::percent(py::str("// %-20s masked-equal = %s"), py::str(f.label), tf(eq)));
    }
}

}  // namespace codebreaker::invert::msc6
