// codebreaker/formats/detect.hpp -- COMPAT shim for the old typed port of codebreaker.formats.detect (deleted when its last py::
// consumer is refactored). The detector is native/cb/formats/detect.hpp; this header only converts. No logic.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/detect.hpp"
#include "py.hpp"

namespace codebreaker::formats::detect {

inline std::filesystem::path fs_path_(const py::str& p) { return cb::fs::from_text(p.utf8()); }

// The two-byte new-header signature (b"" for a plain MZ program); None when the file is unreadable or not MZ.
inline std::optional<py::bytes> signature(const py::str& path) {
    auto sig = cb::formats::detect::new_header(fs_path_(path));
    if (!sig) return std::nullopt;
    return py::bytes(std::string(cb::formats::detect::signature_text(*sig)));
}

inline std::optional<py::str> format_of(const py::str& path) {
    auto f = cb::formats::detect::format_of(fs_path_(path));
    return f ? std::optional<py::str>(cb::compat::to_py(*f)) : std::nullopt;
}

inline std::optional<py::str> lane(const py::str& path) {
    auto l = cb::formats::detect::lane(fs_path_(path));
    return l ? std::optional<py::str>(py::str(cb::formats::detect::lane_name(*l))) : std::nullopt;
}

inline std::optional<py::str> kind(const py::str& path) {
    auto l = cb::formats::detect::kind(fs_path_(path));
    return l ? std::optional<py::str>(py::str(cb::formats::detect::lane_name(*l))) : std::nullopt;
}

inline bool flat(const std::optional<py::str>& kind) { return kind && *kind == py::str("pe"); }

inline bool wide_resources(const std::optional<py::str>& kind) { return kind && *kind == py::str("pe"); }

inline bool holds_drivers(const py::str& path, const py::list<py::bytes>& bundles = py::list<py::bytes>{py::bytes("W3"), py::bytes("W4")}) {
    bool w3 = false, w4 = false;
    for (const py::bytes& b : bundles) {
        if (b.raw() == "W3") w3 = true;
        if (b.raw() == "W4") w4 = true;
    }
    return cb::formats::detect::holds_drivers(fs_path_(path), w3, w4);
}

struct DosHeader {
    int64_t bytes_in_last_page = 0, pages = 0, relocations = 0, header_paragraphs = 0, header_size = 0, min_alloc = 0, max_alloc = 0, ss = 0, sp = 0,
            checksum = 0, ip = 0, cs = 0, relocation_table = 0, overlay = 0, new_header = 0, load_size = 0;
    py::list<std::tuple<int64_t, int64_t>> relocation_entries;  // (segment, offset)
};

inline std::optional<DosHeader> dos_header(const py::bytes& data) {
    return cb::compat::guard([&]() -> std::optional<DosHeader> {
        auto h = cb::formats::detect::dos_header(cb::compat::from_py(data));
        if (!h) return std::nullopt;
        DosHeader o;
        o.bytes_in_last_page = h->bytes_in_last_page, o.pages = h->pages, o.relocations = h->relocations, o.header_paragraphs = h->header_paragraphs;
        o.header_size = h->header_size, o.min_alloc = h->min_alloc, o.max_alloc = h->max_alloc, o.ss = h->ss, o.sp = h->sp, o.checksum = h->checksum;
        o.ip = h->ip, o.cs = h->cs, o.relocation_table = h->relocation_table, o.overlay = h->overlay, o.new_header = h->new_header;
        o.load_size = static_cast<int64_t>(h->load_size);
        for (const auto& r : h->relocation_entries) o.relocation_entries.append(std::make_tuple<int64_t, int64_t>(r.segment, r.offset));
        return o;
    });
}

struct Detected {
    py::str format, detail, family, command;
};

inline py::list<Detected> identify(const py::str& path) {
    py::list<Detected> out;
    for (const auto& d : cb::formats::detect::identify(fs_path_(path)))
        out.append(Detected{cb::compat::to_py(d.format), cb::compat::to_py(d.detail), cb::compat::to_py(d.family), cb::compat::to_py(d.command)});
    return out;
}

}  // namespace codebreaker::formats::detect
