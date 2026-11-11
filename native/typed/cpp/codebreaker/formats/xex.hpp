// codebreaker/formats/xex.hpp -- compat shim over cb/formats/xex.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module itself is native/cb/formats/xex.{hpp,cpp}; this header keeps the old py:: surface for the ports that
// have not been refactored yet (xex_edit, xex_imports, god, powerpc lanes) and is deleted with the last of them.
// Conversion and forwarding only. The settings the old code read (`xex_bin`, the state folder) are read here and
// handed to the new module as `Tools`.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/xex.hpp"
#include "codebreaker/symbols/native.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::formats::xex {

inline const py::bytes MAGIC = py::bytes("XEX2");
inline constexpr int64_t FILE_FORMAT_INFO = 0x000003FF;

// Whether the file starts with XEX2.
bool is_xex(const py::str& path);

// (the entry's offset, key, value) of each entry of the header's optional-header table, in order (read
// as asked: a reader that stops at the key it wants reads no further). A value is the data itself for a
// key whose low byte is 0 or 1, else the offset of the data in the header.
using HeaderEntry = std::tuple<int64_t, int64_t, int64_t>;
py::iter<HeaderEntry> optional_headers(const py::bytes& data);

// (media id, title id) from the header's execution info, or (0, 0) for a header without one (or what is
// not an XEX2 header)
std::tuple<int64_t, int64_t> ids(const py::bytes& data);

py::str program();
py::str _sha(const py::str& path);

// The PowerPC image inside `path`, extracted once per distinct file (named by its contents,
// so a changed .xex is a new basefile and nothing is stale).
py::str basefile(const py::str& path);

// Write `path` as an unencrypted XEX with basic compression: every header byte kept (optional
// headers, security info, imports, exports, resources), the file format info replaced by one
// block holding the whole image. XenonRecomp and the Xbox 360 runtime read only this form. The
// security info's hashes no longer match; nothing that reads it checks them.
py::str basic(const py::str& path, const py::str& out);

// [library name] for a basefile written by basefile() -- the .xex header named them -- or
// None for an image that did not come from a .xex.
std::optional<symbols::native::Json> import_libraries(const py::str& image_path);

struct Header {
    std::optional<py::str> name;
    std::optional<int64_t> base, entry;
    py::list<py::str> imports;
    py::list<std::tuple<py::str, py::str>> libraries;
};

// What the .xex header states, read from the native program's listing.
Header header(const py::str& path);

// `codebreaker xex <command> <file.xex> [args]`: info, list, extract, resources, xml, idc,
// compress, decompress, binary, encrypt, decrypt, machine, remove-limits, bounding-path, patch,
// selftest (`codebreaker xex` alone lists them).
int64_t run(const py::list<py::str>& argv);

int64_t main(const py::list<py::str>& argv);

// ---- the bodies: all conversion --------------------------------------------------------------------------

namespace shim_ {

inline std::filesystem::path path_of(const py::str& p) { return cb::fs::from_text(cb::compat::from_py(p)); }

// the settings the old code read, as the new module's Tools: xex.exe in `xex_bin`, images under state/unpacked/xex
inline cb::formats::xex::Tools tools() {
    return {path_of(py::os::path::join(py::settings::get_str(py::str("xex_bin")), py::str("xex.exe"))),
            path_of(py::os::path::join(py::os::path::join(py::settings::get_str(py::str("state")), py::str("unpacked")),
                                       py::str("xex")))};
}

// the entries optional_headers gives, then the failure the reader met (thrown by the step that reaches it)
class OptionalHeadersGenerator : public py::generator<HeaderEntry> {
public:
    explicit OptionalHeadersGenerator(py::bytes data) : data_(std::move(data)), bytes_(cb::compat::from_py(data_)) {
        cb::compat::guard([&] { table_.emplace(cb::ByteView(bytes_)); });
    }
    std::optional<HeaderEntry> next() override {
        return cb::compat::guard([&]() -> std::optional<HeaderEntry> {
            const auto h = table_->next();
            if (!h) return std::nullopt;
            return HeaderEntry(static_cast<int64_t>(h->offset), h->key, h->value);
        });
    }

private:
    py::bytes data_;
    cb::Bytes bytes_;
    std::optional<cb::formats::xex::OptionalHeaders> table_;
};

}  // namespace shim_

inline bool is_xex(const py::str& path) { return cb::formats::xex::probe(shim_::path_of(path)); }

inline py::iter<HeaderEntry> optional_headers(const py::bytes& data) {
    return py::iter<HeaderEntry>(std::make_shared<shim_::OptionalHeadersGenerator>(data));
}

inline std::tuple<int64_t, int64_t> ids(const py::bytes& data) {
    const cb::formats::xex::TitleIds t = cb::compat::guard([&] { return cb::formats::xex::ids(cb::compat::from_py(data)); });
    return std::make_tuple(int64_t(t.media_id), int64_t(t.title_id));
}

inline py::str program() { return cb::compat::to_py(cb::fs::to_text(shim_::tools().xex_program)); }

inline py::str _sha(const py::str& path) {
    return cb::compat::guard([&] {
        const cb::fs::MappedFile file(shim_::path_of(path));
        return cb::compat::to_py(cb::crypto::to_hex(cb::crypto::sha1(file.view())));
    });
}

inline py::str basefile(const py::str& path) {
    return cb::compat::guard(
        [&] { return cb::compat::to_py(cb::fs::to_text(cb::formats::xex::basefile(shim_::tools(), shim_::path_of(path)))); });
}

inline py::str basic(const py::str& path, const py::str& out) {
    cb::compat::guard([&] { cb::formats::xex::write_basic(shim_::tools(), shim_::path_of(path), shim_::path_of(out)); });
    return out;
}

inline std::optional<symbols::native::Json> import_libraries(const py::str& image_path) {
    const auto names = cb::formats::xex::import_libraries(shim_::path_of(image_path));
    if (!names) return std::nullopt;
    symbols::native::Json::Array list;
    for (const std::string& n : *names) list.push_back(symbols::native::Json{cb::compat::to_py(n)});
    return symbols::native::Json{list};
}

inline Header header(const py::str& path) {
    const cb::formats::xex::Header h = cb::compat::guard([&] { return cb::formats::xex::read_header(shim_::path_of(path)); });
    Header out;
    if (h.name) out.name = cb::compat::to_py(*h.name);
    if (h.base) out.base = int64_t(*h.base);
    if (h.entry) out.entry = int64_t(*h.entry);
    for (const std::string& n : h.imports) out.imports.append(cb::compat::to_py(n));
    for (const cb::formats::xex::Library& l : h.libraries) {
        out.libraries.append(std::make_tuple(cb::compat::to_py(l.name), cb::compat::to_py(l.version)));
    }
    return out;
}

inline int64_t run(const py::list<py::str>& argv) {
    return cb::compat::guard([&]() -> int64_t { return cb::formats::xex::run(shim_::tools(), cb::compat::from_py(argv)); });
}

inline int64_t main(const py::list<py::str>& argv) { return run(argv); }

}  // namespace codebreaker::formats::xex
