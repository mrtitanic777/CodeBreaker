// codebreaker/formats/xbox/ciso.cpp -- see ciso.hpp.

#include "codebreaker/formats/xbox/ciso.hpp"

#include "codebreaker/formats/native_ask.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::xbox::ciso {

namespace {

namespace na = native_ask;

// native.RAISES["CisoError"], native.RAISES["Lz4Error"]
const na::Own& own() {
    static const na::Own o = [](const std::string& kind, const std::string& message) -> bool {
        if (kind == "CisoError") throw CisoError(message);
        if (kind == "Lz4Error") throw Lz4Error(message);
        return false;
    };
    return o;
}

// print("text") (py::print of a char* would print a bool)
void say(const char* utf8) { py::print(py::str(utf8)); }

}  // namespace

const py::str DESCRIPTION("Xbox CSO/CCI compressed disc images: info/list/extract + convert to/from ISO");
const py::bytes CSO_MAGIC("CISO");
const py::bytes CCI_MAGIC("CCIM");

_CompReader::_CompReader(const py::str& path, const char* kind) {
    auto r = gdf::native_open_image(path);                       // os.fspath(path), no data
    _id = std::get<0>(r);
    size = std::get<2>(r);
    _nblocks = std::get<3>(r);
    compressed = std::get<4>(r);
    if (std::get<1>(r) != py::str(kind)) {
        close();
        throw CisoError(py::percent(py::str("not a %s image"), py::str(kind)));
    }
}

std::shared_ptr<_CompReader> open_image(const py::str& path) {
    py::bytes magic = py::open_rb(path).read(4);
    if (magic == CSO_MAGIC) return std::make_shared<CsoReader>(path);
    if (magic == CCI_MAGIC) return std::make_shared<CciReader>(path);
    throw CisoError(py::percent(py::str("%s is not a CSO or CCI image"), path));
}

int64_t write_cso(const py::str& iso_path, const py::str& out_path) {
    return na::ask<int64_t>("ciso.write_cso", py::bytes(), own(), iso_path, out_path);
}

int64_t write_cci(const py::str& iso_path, const py::str& out_path) {
    return na::ask<int64_t>("ciso.write_cci", py::bytes(), own(), iso_path, out_path);
}

int64_t to_iso(const py::str& image_path, const py::str& out_path) {
    return na::ask<int64_t>("ciso.to_iso", py::bytes(), own(), image_path, out_path);
}

void main(const py::list<py::str>& a) {
    auto S = [](const char* s) { return py::str(s); };
    if (a.empty() || a[0] == S("-h") || a[0] == S("--help")) {
        say("usage:\n"
            "  ciso info    <image.cso|.cci>\n"
            "  ciso list    <image.cso|.cci>\n"
            "  ciso extract <image.cso|.cci> <outdir>\n"
            "  ciso toiso   <image.cso|.cci> <out.iso>\n"
            "  ciso tocso   <in.iso> <out.cso>\n"
            "  ciso tocci   <in.iso> <out.cci>");
        return;
    }
    py::str op = a[0];
    if (op == S("tocso")) {
        py::print(py::percent(S("wrote CSO (%d MB uncompressed) -> %s"), py::floordiv(write_cso(a[1], a[2]), int64_t(1) << 20), a[2]));
        return;
    }
    if (op == S("tocci")) {
        py::print(py::percent(S("wrote CCI (%d MB uncompressed) -> %s"), py::floordiv(write_cci(a[1], a[2]), int64_t(1) << 20), a[2]));
        return;
    }
    if (op == S("toiso")) {
        py::print(py::percent(S("wrote %d bytes -> %s"), to_iso(a[1], a[2]), a[2]));
        return;
    }
    std::shared_ptr<_CompReader> r = open_image(a[1]);
    if (op == S("info")) {
        py::str kind = S(r->KIND());
        int64_t comp = r->compressed;
        py::print(py::percent(S("%s image: %s"), kind, a[1]));
        py::print(py::percent(S("uncompressed: %d bytes (%d blocks); %d compressed, %d raw; on-disk %d bytes"), r->size, r->_nblocks, comp,
                              r->_nblocks - comp, py::os::path::getsize(a[1])));
    } else if (op == S("list") || op == S("extract")) {
        gdf::GDF g(r);
        if (op == S("list")) {
            for (const auto& ps : g.files()) py::print(py::percent(S("  %12d  %s"), std::get<1>(ps), std::get<0>(ps)));
        } else {
            py::str outdir = a[2];
            for (const auto& ps : g.files()) {
                py::str dst = py::os::path::join(outdir, std::get<0>(ps).replace(S("/"), S("\\")));
                py::str folder = py::os::path::dirname(dst);
                py::os::makedirs(folder.empty() ? S(".") : folder, 0777, true);
                py::file f = py::open(dst, S("wb"));
                f.write(g.read(std::get<0>(ps)));
                f.close();
            }
            py::print(py::percent(S("extracted -> %s"), outdir));
        }
    }
    r->close();
}

}  // namespace codebreaker::formats::xbox::ciso
