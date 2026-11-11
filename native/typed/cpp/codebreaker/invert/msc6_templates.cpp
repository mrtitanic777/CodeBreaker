// codebreaker/invert/msc6_templates.cpp -- C++17 port of codebreaker/invert/msc6_templates.py (see msc6_templates.hpp).
//
// Build with native/invert's msc6*.cc, cs.cc, pyval.cc, api.cc and native/l2/src/{pyre,pystr,value,json}.cc linked in.

#include "codebreaker/invert/msc6_templates.hpp"

#include <filesystem>
#include <mutex>

#include "codebreaker/base/capstone_dll.hpp"
#include "codebreaker/invert/compilers.hpp"
#include "cs.h"
#include "flow16.h"
#include "json.hpp"
#include "msc6.h"

namespace codebreaker::invert::msc6_templates {

const py::str DOC = py::str(
    "msc6_templates.py -- the leaf templates of Microsoft C 6.00a (medium model, optimized): T0 the\n"
    "loadds far frame, T1 return-constant, T2 return-param, T3 return-global, T4 store-constant, T5 one far\n"
    "call, T6 an if-branch of leaves; anything else declined.  C++ (native/invert/msc6_small.cc;\n"
    "docs/NATIVE.md); this keeps the API and the command line.\n"
    "\n"
    "Usage:  python -m codebreaker invert --tool msc6-templates <hexbytes> [--verify]\n");

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

py::list<py::str> strs(const py::V& v) {
    py::list<py::str> out;
    for (const py::V& x : v.seq()) out.append(py::str(x.str()));
    return out;
}

}  // namespace

Inverted invert(const py::bytes& code) {
    std::string json = guarded([&] { return cbinv::msc6_templates(raw(code)); });
    py::V a = py::json_parse(json);
    Inverted out;
    if (a.get("t").is_none()) {
        out.why = py::str(a.get("w").str());
        return out;
    }
    out.text = py::str(a.get("t").str());
    const py::V& m = a.get("m");
    out.meta.nargs = m.get("nargs").as_int();
    if (m.has("frame")) out.meta.frame = m.get("frame").as_int();
    if (m.has("trailing_nop")) out.meta.trailing_nop = m.get("trailing_nop").as_int() != 0;
    if (m.has("rtype")) out.meta.rtype = py::str(m.get("rtype").str());
    if (m.has("globals")) out.meta.globals = strs(m.get("globals"));
    out.meta.template_ = py::str(m.get("template").str());
    if (m.has("call_args")) out.meta.call_args = strs(m.get("call_args"));
    if (m.has("lines")) out.meta.lines = strs(m.get("lines"));
    return out;
}

int64_t main(const py::list<py::str>& argv) {
    namespace ora = codebreaker::invert::compilers;
    py::list<py::str> args;
    for (int64_t i = 1; i < argv.size(); ++i)
        if (!argv[i].startswith(py::str("--"))) args.append(argv[i]);
    if (args.empty()) {
        py::print(DOC);
        return 2;
    }
    py::bytes code = py::bytes::fromhex(args[0]);
    Inverted r = invert(code);
    if (!r.text) {
        py::print(py::percent(py::str("DECLINED: %s"), r.why));
        return 1;
    }
    py::print(*r.text);
    bool verify = false;
    for (const py::str& a : argv.vec())
        if (a == py::str("--verify")) verify = true;
    if (verify) {
        for (const char* flags : {"/AM /G2 /Gsw /Ox", "/AM /G2 /Gsw /Os"}) {
            py::bytes rc = ora::mc6_compile(*r.text, py::str(flags));   // compile the emitted source as-is
            if (rc == code) {
                py::print(py::percent(py::str("// VERIFIED byte-exact (%s): recompiles to the identical %d bytes"), py::str(flags), code.size()));
                return 0;
            }
        }
        py::print(py::str("// NOT byte-exact under the tried flags -- template incomplete for this function"));
    }
    return 0;
}

}  // namespace codebreaker::invert::msc6_templates
