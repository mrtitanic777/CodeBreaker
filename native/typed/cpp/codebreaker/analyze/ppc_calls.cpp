// codebreaker/analyze/ppc_calls.cpp -- COMPAT SHIM over cb/analyze/ppc_calls.hpp (see ppc_calls.hpp).

#include "codebreaker/analyze/ppc_calls.hpp"

#include <algorithm>
#include <vector>

#include "cb/compat/py_bridge.hpp"
#include "codebreaker/formats/xex.hpp"
#include "codebreaker/formats/xex_imports.hpp"
#include "codebreaker/recompile/disassemble.hpp"
#include "codebreaker/recompile/discover.hpp"
#include "py_os.hpp"

namespace codebreaker::analyze::ppc_calls {

namespace ns = ::cb::analyze::ppc_calls;
namespace powerpc_image = codebreaker::formats::powerpc_image;

const int64_t VOLATILE_MASK = ns::kVolatileMask;
const int64_t FP_VOLATILE_MASK = ns::kFpVolatileMask;
const py::list<int64_t> ARG_REGS = {3, 4, 5, 6, 7, 8, 9, 10};
const py::list<int64_t> VOLATILE = {0, 4, 5, 6, 7, 8, 9, 10, 11, 12};

namespace {

// the module's docstring (what main() prints without arguments)
const char* const DOC =
    "ppc_calls.py -- what a PowerPC program's calls show about its functions, from the binary alone.\n"
    "\n"
    "    python -m codebreaker ppc-calls <module.xex or image> [-o calls.tsv]\n"
    "    from codebreaker.analyze import ppc_calls\n"
    "    r = ppc_calls.analyse(path)\n"
    "    r.arity        {address: argument registers r3.. its callers set up}   (internal functions)\n"
    "    r.imports      {name: argument registers}                            (imported functions)\n"
    "    r.clobber      {address: mask of r0, r4-r12 it may change}           (only those that keep some)\n"
    "    r.fpclobber    {address: mask of f0-f13 it may change}               (only those that keep some)\n"
    "    r.variadic     {address: fixed arguments before the variable ones}\n"
    "\n"
    "Each is evidence, never a guess:\n"
    "\n"
    "  * Arity: before a `bl`, the argument registers r3, r4, ... set within the 14 instructions back\n"
    "    (stopping at an earlier call), counted while they run on without a gap. A function's arity is\n"
    "    the count most of its call sites agree on: at least 3 sites, 60 % agreeing (85 % for 8, which\n"
    "    a longer argument list also gives), and 1-7 (0 is a pass-through as often as no arguments). An\n"
    "    import is a stub with no body to pass through, so 0 and 8 count there; it needs 3 sites and\n"
    "    85 %, and the printf family, whose count varies by call, is left out.\n"
    "  * Clobber: the volatile registers a function may change -- those it writes, and those every\n"
    "    function it calls may change, to a fixed point over the call graph. A call to an import, through\n"
    "    a pointer or out of the known functions may change them all. The compiler's register save and\n"
    "    restore runs (__savegprlr_N ...) change nothing the caller sees, and neither does a register\n"
    "    reloaded from where the function stored it on its frame. What is reported is what a function\n"
    "    provably keeps: a caller may read those registers across the call.\n"
    "  * Variadic: a function that stores r8, r9 and r10 to its caller's frame before its own `stwu`\n"
    "    is homing the va_list area; its fixed arguments are the registers below the lowest one homed.\n"
    "\n"
    "These passes come from the clean-room xam decompile (../xam, tools/infer_*.py), where they turned\n"
    "thousands of refused functions into faithful C; here they run on CodeBreaker's own listing\n"
    "(recompile/disassemble.py) and function discovery (recompile/discover.py), for any module.\n";

py::list<py::str> _ops(const py::str& text) {
    py::list<py::str> out;
    for (const std::string& o : ns::split_operands(text.utf8())) out.append(py::str(o));
    return out;
}

}  // namespace

std::tuple<py::dict<int64_t, Body>, py::ref<powerpc_image::Image>> _functions(const py::str& path) {
    auto [funcs, sources, img] = recompile::discover::discover(path);
    (void)sources;
    const int64_t lo = std::get<0>(img->code), hi = std::get<1>(img->code);
    const py::bytes code = img->bytes_at(lo, hi - lo);
    std::vector<int64_t> starts;
    for (const auto& kv : funcs.items()) {
        const int64_t a = std::get<0>(kv);
        if (!(lo <= a && a < hi) || img->imports.contains(a)) continue;
        if (!img->run_names) throw py::AttributeError("'Image' object has no attribute 'run_names'");
        if (img->run_names->contains(a)) continue;
        starts.push_back(a);
    }
    std::sort(starts.begin(), starts.end());
    py::dict<int64_t, Body> out;
    for (size_t i = 0; i < starts.size(); ++i) {
        const int64_t a = starts[i];
        const int64_t size = funcs.at(a);
        int64_t end = size ? a + size : (i + 1 < starts.size() ? starts[i + 1] : hi);
        if (i + 1 < starts.size()) end = std::min(end, starts[i + 1]);
        end = std::min(end, hi);
        Body body;
        for (const auto& row : recompile::disassemble::listing(code, lo, a - lo, py::floordiv(end - a, 4)))
            body.append(Row(std::get<0>(row), std::get<2>(row), _ops(std::get<3>(row))));
        out.set(a, body);
    }
    return {out, img};
}

Result analyse(const py::str& path) {
    auto [funcs, img] = _functions(path);
    ns::Module module;
    if (formats::xex::is_xex(path)) {
        for (const auto& imp : formats::xex_imports::imports(path)) {
            const auto& thunk = std::get<5>(imp);
            if (thunk && *thunk) module.import_names[static_cast<uint64_t>(*thunk)] = std::get<2>(imp).utf8();
        }
    }
    for (const auto& kv : img->imports.items()) module.import_names.emplace(static_cast<uint64_t>(std::get<0>(kv)), std::get<1>(kv).utf8());
    if (!img->run_names) throw py::AttributeError("'Image' object has no attribute 'run_names'");
    for (int64_t a : img->run_names->vec()) module.transparent.insert(static_cast<uint64_t>(a));
    for (const auto& kv : funcs.items()) {
        ns::Body body;
        for (const Row& row : std::get<1>(kv).vec()) {
            ns::Instruction ins;
            ins.address = static_cast<uint64_t>(std::get<0>(row));
            ins.mnemonic = std::get<1>(row).utf8();
            for (const py::str& o : std::get<2>(row)) ins.operands.push_back(o.utf8());
            body.push_back(std::move(ins));
        }
        module.functions[static_cast<uint64_t>(std::get<0>(kv))] = std::move(body);
    }
    const ns::Result n = ns::analyse(module);
    Result r;
    r.functions = static_cast<int64_t>(n.functions);
    for (const auto& [a, v] : n.arity) r.arity.set(static_cast<int64_t>(a), v);
    for (const auto& [name, v] : n.imports) r.imports.set(py::str(name), v);
    for (const auto& [a, m] : n.clobber) r.clobber.set(static_cast<int64_t>(a), m);
    for (const auto& [a, m] : n.fpclobber) r.fpclobber.set(static_cast<int64_t>(a), m);
    for (const auto& [a, v] : n.variadic) r.variadic.set(static_cast<int64_t>(a), v);
    return r;
}

void write_tsv(const Result& r, const py::str& out) {
    ns::Result n;
    n.functions = static_cast<std::size_t>(r.functions);
    for (const auto& kv : r.arity.items()) n.arity[static_cast<uint64_t>(std::get<0>(kv))] = static_cast<int>(std::get<1>(kv));
    for (const auto& kv : r.imports.items()) n.imports[std::get<0>(kv).utf8()] = static_cast<int>(std::get<1>(kv));
    for (const auto& kv : r.clobber.items()) n.clobber[static_cast<uint64_t>(std::get<0>(kv))] = static_cast<uint32_t>(std::get<1>(kv));
    for (const auto& kv : r.fpclobber.items()) n.fpclobber[static_cast<uint64_t>(std::get<0>(kv))] = static_cast<uint32_t>(std::get<1>(kv));
    for (const auto& kv : r.variadic.items()) n.variadic[static_cast<uint64_t>(std::get<0>(kv))] = static_cast<int>(std::get<1>(kv));
    py::file f = py::open(out, py::str("w"), py::str("utf-8"));
    f.write(py::str(ns::to_tsv(n)));
    f.close();
}

int64_t main(const py::list<py::str>& argv) {
    if (argv.empty() || argv[0] == py::str("-h") || argv[0] == py::str("--help")) {
        py::print(py::str(DOC));
        return argv.empty() ? 2 : 0;
    }
    std::optional<py::str> out;
    if (argv.contains(py::str("-o"))) out = argv[argv.index(py::str("-o")) + 1];
    const Result r = analyse(argv[0]);
    py::print(py::percent(py::str("%d functions: arity for %d, import arity for %d, %d variadic, %d keep a volatile register, "
                                  "%d keep a volatile FP register"),
                          r.functions, r.arity.size(), r.imports.size(), r.variadic.size(), r.clobber.size(), r.fpclobber.size()));
    if (out) {
        write_tsv(r, *out);
        py::print(py::percent(py::str("written: %s"), *out));
    }
    return 0;
}

}  // namespace codebreaker::analyze::ppc_calls
