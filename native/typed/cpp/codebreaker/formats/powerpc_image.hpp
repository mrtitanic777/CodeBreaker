// codebreaker/formats/powerpc_image.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3) over cb/formats/powerpc_image.hpp, which holds the module and its
// documentation: what a PowerPC PE image states about its own code (its functions, import stubs, register save/restore runs, exception table
// and the virtual functions its run-time type information names). This keeps the old py:: surface for the ports that still use it: `Image`
// with py containers, `read` (cached per path), `virtual_methods` (cached on the image) and `regions`. Conversion and forwarding only.
//
//   * `read` raises the Python's ValueError for a file that is not PowerPC (the new reader's FormatError, through the compat guard).
//   * `base`: the sidecar `<file>.json`'s `"base"` when it is a number, else the header's; the old reader carried any other kind of value on
//     to fail later (a TypeError), the new one reads the header's.
#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/powerpc_image.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::powerpc_image {

inline constexpr int64_t MACHINE = 0x01F2;

// The image: the attributes of the new module's Image, as the old callers read them.
struct Image {
    py::str path;
    int64_t base = 0;
    py::bytes data;
    bool memory_layout = false;
    py::list<std::tuple<int64_t, int64_t, int64_t>> spans;               // (address, file offset, size)
    py::list<std::tuple<py::str, int64_t, int64_t, int64_t>> sections;   // (name, address, size, characteristics)
    py::list<int64_t> functions;
    py::dict<int64_t, py::str> imports;
    py::dict<int64_t, py::str> inline_;                                  // `inline` in the Python
    std::tuple<int64_t, int64_t> code{0, 0};
    py::dict<int64_t, std::tuple<int64_t, int64_t, bool>> extents;       // start -> (size, prologue size, has a handler)
    int64_t entry = 0;
    std::optional<py::dict<int64_t, py::str>> run_names;                 // {address: run name}; None without code
    std::optional<py::dict<int64_t, std::tuple<py::str, int64_t>>> _virtual;   // virtual_methods' cache
    std::shared_ptr<const ::cb::formats::powerpc_image::Image> native;   // the module's image this was made from

    // The file offset of an address, or None when no span holds it.
    std::optional<int64_t> offset(int64_t va) const {
        const auto found = va >= 0 && va <= 0xFFFFFFFFLL ? native->offset(static_cast<uint32_t>(va)) : std::nullopt;
        return found ? std::optional<int64_t>(static_cast<int64_t>(*found)) : std::nullopt;
    }
    // One big-endian instruction word, or None.
    std::optional<int64_t> word(int64_t va) const {
        const auto w = va >= 0 && va <= 0xFFFFFFFFLL ? native->word(static_cast<uint32_t>(va)) : std::nullopt;
        return w ? std::optional<int64_t>(static_cast<int64_t>(*w)) : std::nullopt;
    }
    py::bytes bytes_at(int64_t va, int64_t size) const {
        if (va < 0 || va > 0xFFFFFFFFLL || size < 0) return py::bytes();
        return ::cb::compat::to_py(native->bytes_at(static_cast<uint32_t>(va), static_cast<std::size_t>(size)));
    }
};

namespace shim_detail {

inline py::ref<Image> old_image(const py::str& path, std::shared_ptr<const ::cb::formats::powerpc_image::Image> n) {
    py::ref<Image> img = std::make_shared<Image>();
    img->path = path;
    img->base = n->base();
    img->data = ::cb::compat::to_py(n->file());
    img->memory_layout = n->memory_layout();
    for (const auto& s : n->spans()) img->spans.append(std::make_tuple(int64_t{s.va}, static_cast<int64_t>(s.file_offset), static_cast<int64_t>(s.size)));
    for (const auto& s : n->sections())
        img->sections.append(std::make_tuple(::cb::compat::to_py(s.name), int64_t{s.va}, int64_t{s.size}, int64_t{s.characteristics}));
    for (const auto a : n->functions()) img->functions.append(int64_t{a});
    for (const auto& [a, name] : n->imports()) img->imports.set(int64_t{a}, ::cb::compat::to_py(name));
    for (const auto& [a, name] : n->inlined()) img->inline_.set(int64_t{a}, ::cb::compat::to_py(name));
    img->code = std::make_tuple(int64_t{n->code_begin()}, int64_t{n->code_end()});
    for (const auto& [a, e] : n->extents()) img->extents.set(int64_t{a}, std::make_tuple(int64_t{e.size}, int64_t{e.prologue_size}, e.has_handler));
    img->entry = n->entry();
    if (n->has_code()) {
        py::dict<int64_t, py::str> runs;
        for (const auto& [a, name] : n->run_names()) runs.set(int64_t{a}, ::cb::compat::to_py(name));
        img->run_names = runs;
    }
    img->native = std::move(n);
    return img;
}

inline std::mutex& cache_lock() {
    static std::mutex m;
    return m;
}
inline std::map<std::string, py::ref<Image>>& cache() {
    static std::map<std::string, py::ref<Image>> c;
    return c;
}

}  // namespace shim_detail

// -> Image for a PowerPC PE file (cached per path).
inline py::ref<Image> read(const py::str& path_) {
    const py::str path = py::os::path::abspath(path_);
    std::lock_guard<std::mutex> hold(shim_detail::cache_lock());
    const auto found = shim_detail::cache().find(path.utf8());
    if (found != shim_detail::cache().end()) return found->second;
    py::ref<Image> img = ::cb::compat::guard([&] {
        return shim_detail::old_image(path, std::make_shared<const ::cb::formats::powerpc_image::Image>(
                                                ::cb::formats::powerpc_image::Image::read(::cb::fs::from_text(path.utf8()))));
    });
    shim_detail::cache()[path.utf8()] = img;
    return img;
}

// {address: (class, slot)} -- each virtual function whose table still links, through its locator, to the class's type descriptor.
inline py::dict<int64_t, std::tuple<py::str, int64_t>> virtual_methods(Image& img) {
    if (img._virtual) return *img._virtual;
    py::dict<int64_t, std::tuple<py::str, int64_t>> out;
    for (const auto& m : ::cb::formats::powerpc_image::virtual_methods(*img.native))
        out.set(int64_t{m.address}, std::make_tuple(::cb::compat::to_py(m.class_name), int64_t{m.slot}));
    img._virtual = out;
    return out;
}

// [(address, bytes)] -- the image as it sits in memory, for running it.
inline py::list<std::tuple<int64_t, py::bytes>> regions(const py::str& path) {
    py::ref<Image> img = read(path);
    py::list<std::tuple<int64_t, py::bytes>> out;
    for (const auto& r : img->native->regions()) out.append(std::make_tuple(int64_t{r.va}, ::cb::compat::to_py(r.bytes)));
    return out;
}

}  // namespace codebreaker::formats::powerpc_image
