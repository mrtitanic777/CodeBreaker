// codebreaker/formats/xbox/gdf.cpp -- see gdf.hpp.

#include "codebreaker/formats/xbox/gdf.hpp"

#include "codebreaker/formats/xbox/god.hpp"

namespace codebreaker::formats::xbox::gdf {

const py::str DESCRIPTION("read Xbox 360 disc images / SVOD (XDVDFS/GDF): info/list/extract");
const py::bytes MAGIC("MICROSOFT*XBOX*MEDIA");

using native::ask;

namespace {

// print("text") (py::print of a char* would print a bool)
void say(const char* utf8) { py::print(py::str(utf8)); }

// the device id as an argument: None for a closed device
std::optional<int64_t> id_of(const Device& d) { return d._id; }

}  // namespace

// ---- Device ----------------------------------------------------------------------------------------------------------
Device::~Device() {
    try {
        close();
    } catch (...) {
    }
}

py::bytes Device::read_at(int64_t offset, int64_t length) {
    return ask<py::bytes>("xdev.read", py::bytes(), id_of(*this), offset, length);
}

void Device::close() {
    if (_id) {
        ask<std::optional<int64_t>>("xdev.close", py::bytes(), *_id);
        _id.reset();
    }
}

FileDevice::FileDevice(const py::str& path) { _id = ask<int64_t>("xdev.open_file", py::bytes(), path); }

std::tuple<int64_t, py::str, int64_t, int64_t, int64_t> native_open_image(const py::str& path) {
    return ask<std::tuple<int64_t, py::str, int64_t, int64_t, int64_t>>("xdev.open_image", py::bytes(), path);
}

// ---- GDF -------------------------------------------------------------------------------------------------------------
GDF::GDF(std::shared_ptr<Device> dev_, std::optional<int64_t> base_, std::optional<int64_t> descriptor) : dev(std::move(dev_)) {
    if (descriptor) _descriptor = descriptor;                // (where the volume descriptor is, when not at the volume's sector 32)
    auto r = ask<std::tuple<int64_t, int64_t, int64_t>>("gdf.open", py::bytes(), id_of(*dev), base_, descriptor);
    base = std::get<0>(r);
    root_sector = std::get<1>(r);
    root_size = std::get<2>(r);
}

py::bytes GDF::_sector(int64_t sec, int64_t length) { return dev->read_at(base + sec * SECTOR, length); }

py::list<WalkEntry> GDF::_walk_dir(int64_t sector, int64_t size, const py::str& prefix) {
    return ask<py::list<WalkEntry>>("gdf.walk", py::bytes(), id_of(*dev), base, sector, size, prefix);
}

py::list<std::tuple<py::str, int64_t>> GDF::files() {
    return ask<py::list<std::tuple<py::str, int64_t>>>("gdf.files", py::bytes(), id_of(*dev), base, root_sector, root_size);
}

py::bytes GDF::read(const py::str& path) {
    auto data = ask<std::optional<py::bytes>>("gdf.read", py::bytes(), id_of(*dev), base, root_sector, root_size, path);
    if (!data) throw py::KeyError(py::repr(path).utf8());
    return *data;
}

py::bytes build_xdvdfs(const py::dict<py::str, py::bytes>& files) { return ask<py::bytes>("gdf.build_xdvdfs", py::bytes(), files); }

std::shared_ptr<GDF> _open(const py::str& path) {
    if (py::os::path::isdir(path)) {
        // a GoD/SVOD .data directory: use the hash-block-aware deinterleaver
        return std::make_shared<GDF>(std::make_shared<god::SvodReader>(path), 0);
    }
    return std::make_shared<GDF>(std::make_shared<FileDevice>(path));
}

// ------------------------------------------------------------------------ CLI
void main(const py::list<py::str>& a) {
    if (a.empty() || a[0] == py::str("-h") || a[0] == py::str("--help")) {
        say("usage: gdf {info|list|extract} <image.iso|svod-dir> [path] [out]");
        return;
    }
    py::str op = a[0];
    if (a.size() < 2) {
        py::print(py::percent(py::str("usage: gdf %s <image|svod-dir>"), op));
        throw py::SystemExit(2);
    }
    std::shared_ptr<GDF> g = _open(a[1]);
    if (op == py::str("info")) {
        auto fs = g->files();
        int64_t total = 0;
        for (const auto& f : fs) total += std::get<1>(f);
        py::print(py::percent(py::str("volume base : 0x%X"), g->base));
        py::print(py::percent(py::str("root sector : %d  size 0x%X"), g->root_sector, g->root_size));
        py::print(py::percent(py::str("files       : %d, %d bytes total"), fs.size(), total));
    } else if (op == py::str("list")) {
        auto fs = g->files();
        for (const auto& f : fs) py::print(py::percent(py::str("  %12d  %s"), std::get<1>(f), std::get<0>(f)));
        py::print(py::percent(py::str("total: %d files"), fs.size()));
    } else if (op == py::str("extract")) {
        if (a.size() < 3) {
            say("usage: gdf extract <image> <path-on-disc> [out]");
            throw py::SystemExit(2);
        }
        py::bytes data = g->read(a[2]);
        py::str out = a.size() > 3 ? a[3] : py::os::path::basename(a[2].replace(py::str("/"), py::str("\\")));
        py::file f = py::file::open(out, py::str("wb"));
        f.write(data);
        f.close();
        py::print(py::percent(py::str("extracted %s (%d bytes) -> %s"), a[2], data.size(), out));
    } else {
        py::print(py::percent(py::str("unknown op %r"), op));
        throw py::SystemExit(2);
    }
}

}  // namespace codebreaker::formats::xbox::gdf
