// codebreaker/exact/habits/cmpp.hpp -- compat shim for cb/exact/habits/cmpp.hpp (docs/CPP_DESIGN.md 6.3). Conversion and
// forwarding only; deleted with its last old-layer consumer (exact/habits/newcomp, exact/pascal_join).
//
//     python cmpp.py M.EXE 0x00001 m100/p00001.pas [--proc NAME] [--asm] [--cod]
//
// What cmpp.py takes from outside, none of it with a typed port yet:
//   MD = platforms.decoder(16)  -- the 16-bit x86 decoder: the one codebreaker/exact/x86.hpp installs with set_decoder
//   import pcc                  -- tools/pcc.py, the MS-Pascal compiler driver: image(exe) and compile_pas(path)
//   HERE                        -- the folder of cmpp.py (the ledger is HERE/../ledger/<EXE>.json)
// HDRLEN is the module global pascal_join sets from the toolchain (the bytes after the entry helper's call: 3, or 2).
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/habits/cmpp.hpp"
#include "codebreaker/exact/x86.hpp"
#include "py.hpp"

namespace codebreaker::exact::habits::cmpp {

// A raised ImportError (Python's `import pcc` failing)
using py::ImportError;

inline void set_decoder(x86::Decoder d) { x86::set_decoder(std::move(d)); }

// what pcc.compile_pas(path) returns (its .code, .procs, .fix, .lst, .console, .cod)
struct CompileResult {
    std::optional<py::bytes> code;
    py::dict<py::str, std::tuple<int64_t, int64_t>> procs;
    py::list<int64_t> fix;
    py::str lst;
    py::str console;
    py::str cod;
};
struct Pcc {
    std::function<py::bytes(const py::str&)> image;
    std::function<CompileResult(const py::str&)> compile_pas;
};

inline Pcc& installed_pcc() {   // (the old layer's one `pcc` module)
    static Pcc p;
    return p;
}
inline void set_pcc(Pcc p) { installed_pcc() = std::move(p); }

inline py::str HERE;
inline int64_t HDRLEN = 3;   // what follows a procedure's call of the entry helper: a byte and a word (build.py sets 2 for MASM 3.00)

using WalkItem = std::tuple<int64_t, int64_t, py::str, py::str>;

namespace detail {
inline cb::exact::habits::cmpp::Options options() {
    cb::exact::habits::cmpp::Options o;
    o.entry_data = HDRLEN == 2 ? cb::exact::habits::cmpp::EntryData::Word : cb::exact::habits::cmpp::EntryData::ByteAndWord;
    o.decoder = &x86::detail::active();
    return o;
}

// The installed old pcc as the new seam.
class OldPcc final : public cb::exact::habits::cmpp::PascalCompiler {
public:
    cb::Bytes image(const std::string& exe) override {
        if (!installed_pcc().image) throw py::ImportError("No module named 'pcc'");
        return cb::compat::from_py(installed_pcc().image(py::str(exe)));
    }
    cb::exact::habits::cmpp::Compiled compile(const std::filesystem::path& source) override {
        const CompileResult r = installed_pcc().compile_pas(py::str(source.string()));
        cb::exact::habits::cmpp::Compiled out;
        if (r.code) out.code = cb::compat::from_py(*r.code);
        for (const auto& kv : r.procs.items()) {
            out.procedures[std::get<0>(kv).utf8()] = {cb::compat::int_from_py<std::uint32_t>(std::get<0>(std::get<1>(kv))),
                                                      cb::compat::int_from_py<std::uint32_t>(std::get<1>(std::get<1>(kv)))};
        }
        for (const int64_t f : r.fix) out.fixups.push_back(cb::compat::int_from_py<std::uint32_t>(f));
        out.listing = r.lst.utf8();
        out.console = r.console.utf8();
        out.cod = r.cod.utf8();
        return out;
    }
};
}  // namespace detail

// -> [(offset, size, text, mnemonic)]; the db/dw after the entry call are data
inline py::list<WalkItem> walk(const py::bytes& b, int64_t base = 0, bool skip_hdr = true) {
    return cb::compat::guard([&] {
        py::list<WalkItem> out;
        for (const auto& row : cb::exact::habits::cmpp::walk(cb::compat::from_py(b), cb::compat::int_from_py<std::uint32_t>(base), skip_hdr,
                                                             detail::options())) {
            out.append(WalkItem(row.offset, row.size, py::str(row.text), py::str(row.mnemonic)));
        }
        return out;
    });
}

// offsets not compared: fixups, near call displacements to outside, far pointers
inline py::set<int64_t> loose_bytes(const py::bytes& b, const py::set<int64_t>& fx = py::set<int64_t>()) {
    return cb::compat::guard([&] {
        std::set<std::uint32_t> fixups;
        for (const int64_t f : fx) fixups.insert(cb::compat::int_from_py<std::uint32_t>(f));
        py::set<int64_t> out;
        for (const std::uint32_t k : cb::exact::habits::cmpp::loose_bytes(cb::compat::from_py(b), fixups, detail::options())) out.add(k);
        return out;
    });
}

// main(): Python reads sys.argv; the caller hands it in (program name first). Returns the exit status
// (0, or what Python passes to sys.exit); exceptions propagate as Python's would.
inline int64_t main(const py::list<py::str>& argv) {
    return cb::compat::guard([&]() -> int64_t {
        namespace c = cb::exact::habits::cmpp;
        std::vector<std::string> args;
        for (int64_t i = 1; i < py::len(argv); ++i) args.push_back(argv[i].utf8());
        detail::OldPcc pcc;
        const c::Comparison result = c::compare(c::parse_request(args), pcc, std::filesystem::path(HERE.utf8()) / ".." / "ledger", detail::options());
        const std::string text = c::render(result);
        for (const std::string_view line : cb::text::lines(text)) py::print(py::str(std::string(line)));
        return c::exit_status(result);
    });
}

}  // namespace codebreaker::exact::habits::cmpp
