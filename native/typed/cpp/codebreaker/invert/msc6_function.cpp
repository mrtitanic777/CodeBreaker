// codebreaker/invert/msc6_function.cpp -- C++17 port of codebreaker/invert/msc6_function.py (see msc6_function.hpp).
//
// Build with native/invert's msc6*.cc, cs.cc, pyval.cc, api.cc and native/l2/src/{pyre,pystr,value,json}.cc linked in.

#include "codebreaker/invert/msc6_function.hpp"

#include <filesystem>
#include <mutex>

#include "codebreaker/base/capstone_dll.hpp"
#include "codebreaker/invert/compilers.hpp"
#include "cs.h"
#include "flow16.h"
#include "json.hpp"
#include "msc6.h"

namespace codebreaker::invert::msc6_function {

namespace {

// native_invert._load(): the models decode with the capstone.dll Python's package carries
void ensure_capstone() {
    static std::once_flag once;
    std::call_once(once, [] {
        std::string err;
        std::wstring dll = std::filesystem::u8path(base::capstone_dll::find().utf8()).wstring();
        if (!cbinv::cs_load(dll.c_str(), err)) throw py::OSError(err);
    });
}

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

py::bytes _mask(const py::bytes& code) {
    std::vector<uint8_t> m = guarded([&] { return cbinv::msc6_mask(raw(code)); });
    return py::bytes(std::string(m.begin(), m.end()));
}

py::str invert_cleanup(const py::bytes& code, const py::dict<int64_t, py::str>& syms) {
    std::map<long long, std::string> names;
    for (const auto& kv : syms.items()) names[std::get<0>(kv)] = std::get<1>(kv).utf8();
    std::string json = guarded([&] { return cbinv::msc6_cleanup(raw(code), names); });
    return py::str(py::json_parse(json).str());
}

void _selftest() {
    namespace ora = codebreaker::invert::compilers;
    // shipped LZEXPAND Cleanup bytes + its DGROUP long-global map (lo-word offset -> name)
    py::bytes code = py::bytes::fromhex(py::str(
        "1e589045558bec1e8ed883ec02a146000b0644007411ff3646009ae70000002bc0a34600a344"
        "00a14a000b0648007411ff364a009a010100002bc0a34a00a34800a156000b0654007411ff3656"
        "009affff00002bc0a35600a354004d4d8be51f5d4dcb90"));
    py::dict<int64_t, py::str> syms{{0x44, py::str("gpA")}, {0x48, py::str("gpB")}, {0x54, py::str("gpC")}};
    py::str src = invert_cleanup(code, syms);
    py::print(py::str("=== recovered C ===\n") + src + py::str("\n"));
    py::bytes rc = ora::mc6_compile(src, py::str("/AM /G2 /Gsw /Ox"));
    bool ok = _mask(rc) == _mask(code) && rc.size() == code.size();
    py::print(py::percent(py::str("recompiled %d bytes vs shipped %d; masked-equal: %s"), rc.size(), code.size(), py::str(ok ? "True" : "False")));
    if (ok) {
        py::print(py::str("*** WHOLE-FUNCTION VERIFIED byte-exact (relocs masked) ***"));
    } else {
        py::print(py::percent(py::str("*** mismatch:\n  shipped %s\n  ours    %s"), _mask(code).hex(), _mask(rc).hex()));
    }
}

}  // namespace codebreaker::invert::msc6_function
