// codebreaker/source/vxd/owbuild.hpp -- typed C++ of codebreaker.source.vxd.owbuild.
//
// owbuild.py -- build a ring-0 VxD from recovered source with the OpenWatcom toolchain (no Microsoft tools): wasm (MASM-mode assemble)
// + wlink `system win_vxd`.
//
// The recovered per-segment source (vxd_tree.emit_tree) is emitted UNSPLIT (one file per LE object, so every branch target is
// internal), each object is converged to assemble by byte-preserving what wasm rejects (the probe), assembled to OMF, and linked into
// an LE VxD with the DDB as the single required export (`export VXD_DDB.1`). Produces a real loadable <name>.vxd.
//
//     from codebreaker.source.vxd import owbuild as vxd_owbuild
//     vxd_owbuild.build_vxd("vkd.vxd", "VKD", "out")     # -> out/vkd/vkd.vxd
//
// Corpus result: 211/215 standalone ring-0 VxDs build into valid LE binaries.
//
// Typing notes:
//   * The Python's import-time constants (_OW, _WASM, _WLINK, _NASM, which read settings.json) are functions here, computed when
//     first asked and kept (an import cannot fail in C++).
//   * The result dicts are the structs BuildResult ({ok, output | error, bytes, objects}) and W3Result.
//   * build_vxd removes a .vxd an earlier run left in the folder before it links: the Python took any file there for the link's result
//     (so a failed link reported ok when the folder had an old build; the Python was changed alike).
//   * main(argv, out, err): argv is sys.argv[1:]; what it prints goes to `out`; the return is the exit status. The command line is read
//     by hand (the VxD, --out DIR / --out=DIR, -h/--help): a bad one prints a one-line usage and error to `err` and gives 2; argparse's
//     abbreviations and exact wording are not reproduced.
#pragma once

#include <cstdint>
#include <iosfwd>
#include <optional>
#include <tuple>

#include "py.hpp"

namespace codebreaker::source::vxd::owbuild {

const py::str& _OW();
const py::str& _WASM();
const py::str& _WLINK();
const py::str& _NASM();

// the environment of the tools: WATCOM and PATH set
py::dict<py::str, py::str> _env();

struct BuildResult {
    bool ok = false;
    std::optional<py::str> output, error;
    int64_t bytes = 0, objects = 0;
};

// Assemble (wasm) + link (wlink system win_vxd) one VxD's recovered source into a loadable .vxd.
BuildResult build_vxd(const py::str& vxd_path, const py::str& name, const py::str& outroot, bool byte_exact = true);

// Parse a W3 bundle header -> (lfa, [(name, offset, size)]); no lfa when it is not a W3.
std::tuple<std::optional<int64_t>, py::list<std::tuple<py::str, int64_t, int64_t>>> _w3_components(const py::bytes& data);

struct W3Result {
    bool ok = false;
    std::optional<py::str> error, output;
    int64_t bytes = 0;
    py::list<py::str> components;                                    // the names built (ok)
    py::list<std::tuple<py::str, std::optional<bool>>> status;       // (name, did it build) of each component (a failed component build)
};

// Build a W3 VxD bundle (e.g. vmm32.vxd) with OpenWatcom: extract each bundled LE module, rebuild it from recovered source
// (build_vxd), and repack -- the DOS/real-mode front is byte-preserved (it is a real-mode loader, not LE-VxD source), the W3 header +
// records regenerated, each rebuilt LE placed page-aligned.
W3Result build_w3(const py::str& w3_path, const py::str& outroot);

int64_t main(const py::list<py::str>& argv, std::ostream& out, std::ostream& err);

}  // namespace codebreaker::source::vxd::owbuild
