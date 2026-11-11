// codebreaker/java/console/xdk_scan.hpp -- SHIM over cb/java/console/xdk_scan.hpp (docs/CPP_DESIGN.md 6.3); deleted with
// its last py:: consumer (platform_coverage).
#pragma once

#include <optional>

#include "codebreaker/java/console/_shim.hpp"
#include "cb/java/console/xdk_scan.hpp"
#include "py.hpp"

namespace codebreaker::java::console::xdk_scan {

// One entry of PATTERNS as the old code read it: strings for kind, severity and effort.
struct Pattern {
    py::str needs;
    py::str family;
    py::str kind;
    py::str sub;
    std::optional<py::str> sev;
    std::optional<py::str> effort;
    py::str title;
    py::str why;
};

// A PATTERNS entry as detect() reports it: the pattern plus what the scan found.
struct Detection : Pattern {
    bool present = false;
    py::list<py::str> functions;
    int64_t func_count = 0;
    py::str header;
};

struct ScanResult {
    std::optional<py::str> include;
    py::dict<py::str, py::str> funcs;
    py::list<Detection> found;
    py::list<Detection> missing;
};

namespace detail_ {
namespace nw = cb::java::console::xdk_scan;

inline py::str kind_text(nw::Kind k) {
    return py::str(k == nw::Kind::Requirement ? "requirement" : k == nw::Kind::Equivalent ? "equivalent" : "opportunity");
}
inline std::optional<py::str> severity_text(const std::optional<nw::Severity>& s) {
    if (!s) return std::nullopt;
    return py::str(*s == nw::Severity::Correctness ? "correctness" : *s == nw::Severity::Cert ? "cert" : "compat");
}
inline std::optional<py::str> effort_text(const std::optional<nw::Effort>& e) {
    if (!e) return std::nullopt;
    return py::str(*e == nw::Effort::Small ? "S" : "M");
}
inline Pattern to_old(const nw::Pattern& p) {
    Pattern o;
    o.needs = cb::compat::to_py(p.needs);
    o.family = cb::compat::to_py(p.family);
    o.kind = kind_text(p.kind);
    o.sub = cb::compat::to_py(p.sub);
    o.sev = severity_text(p.sev);
    o.effort = effort_text(p.effort);
    o.title = cb::compat::to_py(p.title);
    o.why = cb::compat::to_py(p.why);
    return o;
}
}  // namespace detail_

// The patterns, built on first use.
inline const py::list<Pattern>& PATTERNS() {
    static const py::list<Pattern> list = [] {
        py::list<Pattern> out;
        for (const auto& p : cb::java::console::xdk_scan::patterns()) out.append(detail_::to_old(p));
        return out;
    }();
    return list;
}

inline py::dict<py::str, py::str> scan_functions(const std::optional<py::str>& include_dir) {
    return cb::compat::guard([&] {
        const auto dir = shim::given(include_dir);
        if (!dir) return py::dict<py::str, py::str>();
        return cb::compat::to_py(cb::java::console::xdk_scan::scan_functions(*dir));
    });
}

inline std::optional<py::str> sdk_include_dir(const std::optional<py::str>& sdk_root) {
    const auto root = shim::given(sdk_root);
    if (!root) return std::nullopt;
    return shim::native(cb::java::console::xdk_scan::sdk_include_dir(*root));
}

inline ScanResult detect(const std::optional<py::str>& sdk_root) {
    return cb::compat::guard([&] {
        const auto found = cb::java::console::xdk_scan::detect(shim::given(sdk_root));
        ScanResult out;
        out.include = shim::native(found.include);
        out.funcs = cb::compat::to_py(found.funcs);
        auto convert = [](const cb::java::console::xdk_scan::Detection& d) {
            Detection o;
            static_cast<Pattern&>(o) = detail_::to_old(d);
            o.present = d.present;
            o.functions = cb::compat::to_py(d.functions);
            o.func_count = static_cast<int64_t>(d.func_count);
            o.header = cb::compat::to_py(d.header);
            return o;
        };
        for (const auto& d : found.found) out.found.append(convert(d));
        for (const auto& d : found.missing) out.missing.append(convert(d));
        return out;
    });
}

}  // namespace codebreaker::java::console::xdk_scan
