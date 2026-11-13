// codebreaker/formats/xbox/xur.hpp -- compat shim over cb/formats/xbox/xur.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module itself is native/cb/formats/xbox/xur.{hpp,cpp}; this header keeps the old py:: surface for the ports that have not been
// refactored yet (formats/xbox/package) and is deleted with the last of them. Conversion and forwarding only; `main` is the old
// command-line wrapper over the new command. A section's offset is from the start of the file (the old port read it from the end of the
// section table, which made every real scene read as garbage).
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/cli.hpp"
#include "cb/formats/xbox/xur.hpp"

namespace codebreaker::formats::xbox::xur {

inline constexpr int64_t MAGIC = 0x58554942;   // 'XUIB'
inline constexpr int64_t STRN = 0x5354524E;    // 'STRN'

inline const py::str DESCRIPTION = py::str("read/edit Xbox 360 XUR (compiled XUI) scenes: info/strings + set (repack)");

inline py::str _tag(int64_t v) { return cb::compat::to_py(cb::formats::xbox::xur::tag_text(cb::compat::int_from_py<std::uint32_t>(v))); }

class XUR {
  public:
    py::bytes d;
    int64_t version = 0;
    int64_t flags = 0;
    int64_t tool_version = 0;
    int64_t file_size = 0;
    int64_t section_count = 0;
    py::list<std::tuple<int64_t, int64_t, int64_t>> sections;   // (magic, offset, length)
    int64_t data_base = 0;                                        // where the section table ends

    explicit XUR(const py::bytes& data) : d(data) {
        namespace nx = cb::formats::xbox::xur;
        const nx::Scene scene = cb::compat::guard([&] { return nx::Scene::parse(cb::compat::from_py(data)); });
        version = scene.version;
        flags = scene.flags;
        tool_version = scene.tool_version;
        file_size = scene.file_size;
        section_count = static_cast<int64_t>(scene.sections.size());
        for (const nx::Section& s : scene.sections) sections.append(std::make_tuple(int64_t{s.tag}, int64_t{s.offset}, int64_t{s.length}));
        data_base = static_cast<int64_t>(scene.table_end());
    }

    std::optional<py::bytes> section_data(int64_t magic) const {
        namespace nx = cb::formats::xbox::xur;
        const nx::Scene scene = cb::compat::guard([&] { return nx::Scene::parse(cb::compat::from_py(d)); });
        const auto data = cb::compat::guard([&] { return scene.section_data(cb::compat::int_from_py<std::uint32_t>(magic)); });
        if (!data) return std::nullopt;
        return cb::compat::to_py(*data);
    }

    py::list<py::str> strings() const {
        namespace nx = cb::formats::xbox::xur;
        return cb::compat::to_py(cb::compat::guard([&] { return nx::Scene::parse(cb::compat::from_py(d)).strings(); }));
    }
};

namespace shim_ {
// (a vector of bytes would convert as a list of integers: say it is bytes)
inline py::bytes bytes_(const cb::Bytes& b) { return cb::compat::to_py(cb::ByteView(b)); }
}  // namespace shim_

// Serialise a STRN section body (for tests / round-trips).
inline py::bytes build_strn(const py::list<py::str>& strings) {
    const std::vector<std::string> all = cb::compat::from_py(strings);
    return shim_::bytes_(cb::compat::guard([&] { return cb::formats::xbox::xur::build_strn(all); }));
}

// Re-emit an XUR with its STRN string table replaced; byte-identical to the input when the strings are unchanged.
inline py::bytes set_strings(const py::bytes& data, const py::list<py::str>& new_strings) {
    const std::vector<std::string> all = cb::compat::from_py(new_strings);
    return shim_::bytes_(cb::compat::guard([&] { return cb::formats::xbox::xur::set_strings(cb::compat::from_py(data), all); }));
}

// Replace a single string (by index) and re-emit the XUR.
inline py::bytes set_string(const py::bytes& data, int64_t index, const py::str& text) {
    if (index < 0) throw py::IndexError("string index out of range");
    return shim_::bytes_(cb::compat::guard(
        [&] { return cb::formats::xbox::xur::set_string(cb::compat::from_py(data), static_cast<std::size_t>(index), cb::compat::from_py(text)); }));
}

// main(): the command line without the program name (sys.argv[1:]); a bare path is `info`.
inline void main(const py::list<py::str>& argv) {
    std::vector<std::string> args = {"xur"};
    for (const std::string& a : cb::compat::from_py(argv)) args.push_back(a);
    if (args.size() > 1 && args[1] != "info" && args[1] != "strings" && args[1] != "set" && args[1] != "-h" && args[1] != "--help") args.insert(args.begin() + 1, "info");
    cb::compat::guard([&] { return cb::cli::Cli("xur", {cb::formats::xbox::xur::command()}).run(args); });
}

}  // namespace codebreaker::formats::xbox::xur
