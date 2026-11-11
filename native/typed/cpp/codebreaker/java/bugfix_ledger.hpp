// codebreaker/java/bugfix_ledger.hpp -- SHIM over cb/java/bugfix_ledger.hpp (docs/CPP_DESIGN.md 6.3); deleted with its
// last py:: consumer. (The port's private scanners _MARK, _clean_comment, _scan_file ... had no consumer.)
#pragma once

#include <cstdint>
#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/java/bugfix_ledger.hpp"
#include "codebreaker/java/_paths.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::java::bugfix_ledger {

namespace cbb = cb::java::bugfix_ledger;

struct Entry {
    py::str slug;
    int64_t line = 0;
    py::str desc;
    py::str layer;
    py::str file;
};

namespace conv_ {
inline std::string_view layer_name(cbb::Layer l) {
    switch (l) {
        case cbb::Layer::Canonical: return "canonical";
        case cbb::Layer::Reference: return "reference";
        case cbb::Layer::Companion: return "companion";
        case cbb::Layer::Generator: return "generator";
    }
    return "generator";
}
inline cbb::Layer layer_of(const py::str& s) {
    for (cbb::Layer l : {cbb::Layer::Canonical, cbb::Layer::Reference, cbb::Layer::Companion, cbb::Layer::Generator}) {
        if (layer_name(l) == cb::compat::from_py(s)) return l;
    }
    throw py::ValueError("not a ledger layer: " + cb::compat::from_py(s));
}
}  // namespace conv_

// Return [{'layer','file','slug','line','desc'}] across every authored layer.
inline py::list<Entry> collect(const py::str& repo_root) {
    return cb::compat::guard([&] {
        py::list<Entry> out;
        for (const cbb::MarkerEntry& e : cbb::collect(cb::fs::from_text(cb::compat::from_py(repo_root)))) {
            out.append(Entry{cb::compat::to_py(e.slug), static_cast<int64_t>(e.line), cb::compat::to_py(e.description),
                             cb::compat::to_py(conv_::layer_name(e.layer)), cb::compat::to_py(e.file)});
        }
        return out;
    });
}

// Render the collected entries as BUGFIX_LEDGER.md text.
inline py::str render(const py::list<Entry>& entries) {
    return cb::compat::guard([&] {
        std::vector<cbb::MarkerEntry> converted;
        for (const Entry& e : entries) {
            converted.push_back({conv_::layer_of(e.layer), cb::compat::from_py(e.file), cb::compat::from_py(e.slug),
                                 cb::compat::int_from_py<std::uint32_t>(e.line), cb::compat::from_py(e.desc)});
        }
        return cb::compat::to_py(cbb::render(converted));
    });
}

// Collect markers and write BUGFIX_LEDGER.md (default: state/java/BUGFIX_LEDGER.md). `repo_root` is the folder whose
// layers are scanned (default: this package). Returns the entry list.
inline py::list<Entry> run(const std::optional<py::str>& repo_root = std::nullopt,
                           const std::optional<py::str>& out_path = std::nullopt) {
    const py::str root = repo_root ? *repo_root : codebreaker::java::_paths::_HERE();
    const py::str out = out_path ? *out_path
                                 : py::os::path::join(py::settings::get_str(py::str("state")), py::str("java"),
                                                      py::str("BUGFIX_LEDGER.md"));
    return cb::compat::guard([&] {
        py::list<Entry> out_entries;
        for (const cbb::MarkerEntry& e : cbb::run(cb::fs::from_text(cb::compat::from_py(root)),
                                                  cb::fs::from_text(cb::compat::from_py(out)))) {
            out_entries.append(Entry{cb::compat::to_py(e.slug), static_cast<int64_t>(e.line), cb::compat::to_py(e.description),
                                     cb::compat::to_py(conv_::layer_name(e.layer)), cb::compat::to_py(e.file)});
        }
        return out_entries;
    });
}

}  // namespace codebreaker::java::bugfix_ledger
