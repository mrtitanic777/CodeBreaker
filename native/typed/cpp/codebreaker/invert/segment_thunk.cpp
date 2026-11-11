// codebreaker/invert/segment_thunk.cpp -- C++17 port of codebreaker/invert/segment_thunk.py (see segment_thunk.hpp).
//
// Build with native/invert's msc6*.cc, cs.cc, pyval.cc, api.cc and native/l2/src/{pyre,pystr,value,json}.cc linked in.

#include "codebreaker/invert/segment_thunk.hpp"

#include <filesystem>
#include <mutex>

#include "codebreaker/base/capstone_dll.hpp"
#include "cs.h"
#include "flow16.h"
#include "json.hpp"
#include "msc6.h"

namespace codebreaker::invert::segment_thunk {

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

std::vector<uint8_t> raw(const py::bytes& b) { return std::vector<uint8_t>(b.raw().begin(), b.raw().end()); }

// native_invert.thunk: the model's answer as JSON text
py::V ask(const py::bytes& fb, const py::str& name, bool exact) {
    return py::json_parse(guarded([&] { return cbinv::segment_thunk(raw(fb), name.utf8(), exact); }));
}

std::optional<Thunk> thunk(const py::bytes& fb, bool exact) {
    py::V t = ask(fb, py::str("thunk"), exact);
    if (t.is_none()) return std::nullopt;
    Thunk out;
    out.fb = fb;
    out.asm_ = py::str(t.get("asm").str());
    out.kind = py::str(t.get("kind").str());
    out.pro = py::bytes::fromhex(py::str(t.get("pro").str()));
    out.seg = t.get("seg").as_int();
    for (const py::V& b : t.get("body").seq())
        out.body.append(std::make_tuple(py::str(b[0].str()), py::bytes::fromhex(py::str(b[1].str()))));
    out.epi = py::bytes::fromhex(py::str(t.get("epi").str()));
    if (!t.get("retf").is_none()) out.retf = t.get("retf").as_int();
    out.pad = py::bytes::fromhex(py::str(t.get("pad").str()));
    return out;
}

}  // namespace

std::optional<Thunk> parse(const py::bytes& fb) { return thunk(fb, false); }

std::optional<Thunk> verify(const py::bytes& fb) { return thunk(fb, true); }

py::str emit_asm(const Thunk& t, const py::str& name) {
    py::V a = ask(t.fb, name, false);
    if (a.is_none()) throw py::TypeError("'NoneType' object is not subscriptable");
    return py::str(a.get("asm").str());
}

py::str kind(const Thunk& t) { return t.kind; }

}  // namespace codebreaker::invert::segment_thunk
