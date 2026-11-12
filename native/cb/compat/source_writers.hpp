// cb/compat/source_writers.hpp -- what the shims of the console writers (nes, genesis, gb, sms, z80) share: a new writer's result as
// the old record the ports and the dynamic exe read. Conversion only. Deleted with the last shim that includes it.
#pragma once

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/source/image_writer.hpp"
#include "codebreaker/source/image_writer.hpp"
#include "py.hpp"

namespace cb::compat::source {

namespace old = ::codebreaker::source::image_writer;

inline std::filesystem::path path_of(const py::str& s) { return ::cb::fs::from_text(s.utf8()); }
inline py::str py_path(const std::filesystem::path& p) { return py::str(::cb::fs::to_text(p)); }

// A writer's result as the old record. `unit` names the count of the image's own size ("rom bytes", "prg bytes", "bytes").
inline old::Result old_result(const ::cb::source::image_writer::Written& w, const char* unit) {
    old::Result r;
    r.source = py_path(w.source);
    for (const auto& [address, name] : w.functions) r.functions.set(static_cast<int64_t>(address), old::Function(py::str(name)));
    for (const std::string& n : w.notes) r.notes.append(py::str(n));
    r.linked = w.linked;
    r.same = w.same;
    r.code_bytes = static_cast<int64_t>(w.code_bytes);
    if (w.built) r.built = py_path(*w.built);
    r.more.set(py::str(unit), old::Extra(static_cast<int64_t>(w.image_bytes)));
    if (w.config) r.more.set(py::str("config"), old::Extra(py_path(*w.config)));
    return r;
}

}  // namespace cb::compat::source
