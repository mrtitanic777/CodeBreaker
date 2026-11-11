// codebreaker/invert/whole_function.cpp -- C++17 port of codebreaker/invert/whole_function.py (see whole_function.hpp).
//
// Build with native/invert's msc6*.cc, cs.cc, pyval.cc, api.cc and native/l2/src/{pyre,pystr,value,json}.cc linked in.

#include "codebreaker/invert/whole_function.hpp"

#include <filesystem>
#include <mutex>

#include "codebreaker/base/capstone_dll.hpp"
#include "cs.h"
#include "flow16.h"
#include "json.hpp"
#include "msc6.h"

namespace codebreaker::invert::whole_function {

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

}  // namespace

py::list<std::tuple<py::str, py::str>> candidates(const py::bytes& func_bytes) {
    std::vector<uint8_t> fb(func_bytes.raw().begin(), func_bytes.raw().end());
    std::string json = guarded([&] { return cbinv::whole_function_candidates(fb); });
    py::list<std::tuple<py::str, py::str>> out;
    py::V answer = py::json_parse(json);   // (kept: its items live in it)
    for (const py::V& c : answer.seq()) out.append(std::make_tuple(py::str(c[0].str()), py::str(c[1].str())));
    return out;
}

std::optional<py::str> synthesize(const py::bytes& func_bytes, const Verify& verify) {
    for (const auto& nc : candidates(func_bytes).vec()) {
        const py::str& c = std::get<1>(nc);
        if (!verify) return c;
        py::dict<py::str, bool> r = verify(func_bytes, c);
        if (r.contains(py::str("exact")) && r.at(py::str("exact"))) return c;
    }
    return std::nullopt;
}

}  // namespace codebreaker::invert::whole_function
