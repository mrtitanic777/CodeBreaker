// codebreaker/formats/xbox/chd.hpp -- compat shim over cb/formats/xbox/chd.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module itself is native/cb/formats/xbox/chd.{hpp,cpp}; this header keeps the old py:: surface for the ports that have not been
// refactored yet (formats/xbox/package) and is deleted with the last of them. Conversion and forwarding only: every failure of the new
// module reaches old code as a ChdError, as the old module raised one; `main` is the old command-line wrapper over the new command; the
// program the conversions drive (settings.tool("chdman")) is found through the old layer, as before. The private helpers of the old
// module (`_need_chdman`, `_run`) are not carried over.
#pragma once

#include <algorithm>
#include <cstdint>
#include <format>
#include <optional>
#include <string>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/cli.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/xbox/chd.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::formats::xbox::chd {

inline const py::str DESCRIPTION = py::str("CHD (MAME) Xbox disc images: native header info + chdman-driven chd<->iso/verify");

inline const py::bytes CHD_MAGIC = py::bytes("MComprHD");

struct ChdError : py::Exception {
    using py::Exception::Exception;
    const char* type() const override { return "ChdError"; }
};

// sys.exit(code): not a py::Exception in Python
using py::SystemExit;

// read_header's dict: ints and a list of str
struct Header {
    int64_t version = 0;
    int64_t header_len = 0;
    int64_t logical_bytes = 0;
    int64_t hunk_bytes = 0;
    int64_t unit_bytes = 0;
    py::list<py::str> compressors;
    int64_t meta_offset = 0;
    int64_t map_offset = 0;
};

namespace shim_ {

// The new module's errors are the old module's ChdError.
template <class F>
decltype(auto) chd_(F&& f) {
    try {
        return std::forward<F>(f)();
    } catch (const cb::Error& e) {
        throw ChdError(e.what());
    }
}

// settings.tool("chdman"): the program settings.json names, else one on PATH.
inline cb::formats::xbox::chd::Tool tool_() {
    const py::str named = py::settings::get_str(py::str("chdman"));
    if (!named.empty() && (py::os::path::exists(named) || py::shutil::which(named))) return cb::fs::from_text(named.utf8());
    if (const auto found = py::shutil::which(py::str("chdman"))) return cb::fs::from_text(found->utf8());
    return std::nullopt;
}

inline std::filesystem::path path_(const py::str& p) { return cb::fs::from_text(cb::compat::from_py(p)); }

}  // namespace shim_

// Parse a CHD v5 header (big-endian); a version that is not 5 raises (chdman copy upgrades it).
inline Header read_header(const py::str& path) {
    namespace nc = cb::formats::xbox::chd;
    const nc::Header n = shim_::chd_([&] { return nc::read_header_file(shim_::path_(path)); });
    Header h;
    h.version = n.version;
    h.header_len = n.header_len;
    h.logical_bytes = cb::compat::to_py(n.logical_bytes);
    h.hunk_bytes = n.hunk_bytes;
    h.unit_bytes = n.unit_bytes;
    h.compressors = cb::compat::to_py(n.compressors);
    h.meta_offset = cb::compat::to_py(n.meta_offset);
    h.map_offset = cb::compat::to_py(n.map_offset);
    return h;
}

// chd -> ISO (chdman extractdvd).
inline int64_t extract(const py::str& chd_path, const py::str& out_iso) {
    namespace nc = cb::formats::xbox::chd;
    return cb::compat::to_py(shim_::chd_([&] { return nc::extract(shim_::tool_(), shim_::path_(chd_path), shim_::path_(out_iso)); }));
}

// ISO -> chd (chdman createdvd).
inline int64_t build(const py::str& iso_path, const py::str& out_chd) {
    namespace nc = cb::formats::xbox::chd;
    return cb::compat::to_py(shim_::chd_([&] { return nc::build(shim_::tool_(), shim_::path_(iso_path), shim_::path_(out_chd)); }));
}

inline py::str verify(const py::str& chd_path) {
    namespace nc = cb::formats::xbox::chd;
    return cb::compat::to_py(shim_::chd_([&] { return nc::verify(shim_::tool_(), shim_::path_(chd_path)); }));
}

// `sys_argv`: sys.argv, the program name first. Throws SystemExit(2) where the command line was wrong.
inline void main(const py::list<py::str>& sys_argv) {
    namespace nc = cb::formats::xbox::chd;
    std::vector<std::string> args = {"chd"};
    const std::vector<std::string> all = cb::compat::from_py(sys_argv);
    args.insert(args.end(), all.begin() + (all.empty() ? 0 : 1), all.end());
    const int code = shim_::chd_([&] { return cb::cli::Cli("chd", {nc::command(nc::Environment{shim_::tool_()})}).run(args); });
    if (code != 0) throw SystemExit(static_cast<int64_t>(code));
}

}  // namespace codebreaker::formats::xbox::chd
