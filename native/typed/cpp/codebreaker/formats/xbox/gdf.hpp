// codebreaker/formats/xbox/gdf.hpp -- typed C++ from codebreaker.formats.xbox.gdf.
//
// GDF / XDVDFS -- Xbox 360 disc filesystem reader (and SVOD device).
//
// XDVDFS (a.k.a. GDF, "Game Disc Format") is the filesystem on Xbox 360 discs and inside SVOD (games-on-demand /
// installed) content. It has a volume descriptor at sector 32 and a directory of binary-tree entries; files are
// runs of 0x800-byte sectors. A clean-room Python reader written to the public/documented format -- the disc-image
// half of what the GPL X360 library does -- as original code.
//
// Reads are streamed from the file (disc images are gigabytes), never loaded whole. The XDVDFS volume sits at a
// base offset that depends on the disc layout; common XGD offsets are auto-detected. SVOD content splits the same
// filesystem across Data#### files at a fixed base; a SvodDevice presents them as one stream.
//
// Commands:
//     gdf list    <image.iso|svod-dir>          the on-disc file tree
//     gdf extract <image.iso|svod-dir> <path> [out]   extract one file
//     gdf info    <image.iso|svod-dir>          volume descriptor summary
//
// Typing and runtime notes:
//   * The Python asked formats.dll (native/formats/xbox/discs.cc: the device table, the directory walk) through
//     native.call; here the same questions go to the reader linked in (cbf_call, answered in typed values by
//     py::native). A Device keeps the id the reader gave it (`_id`, None once closed); __del__ is the destructor.
//   * Devices are shared (a GDF, a volume and its caller all hold one): std::shared_ptr<Device>.
//   * `class _FileDevice` is FileDevice (a name with a leading underscore stays out of this namespace's API in C++
//     only by custom; the Python name is kept as an alias below).
//   * main() takes sys.argv[1:] and prints with py::print; sys.exit(n) is py::SystemExit(n).
//   * formats.xbox.ciso has no typed port yet: the two exception classes the reader raises for a damaged CSO / CCI
//     image (CisoError, Lz4Error: native.RAISES) are declared here.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_answers.hpp"
#include "py_os.hpp"
#include "py_stdlib.hpp"

// the reader's one entry point (native/formats/api.cc, linked in)
extern "C" int cbf_call(const char* question, const uint8_t* data, size_t size, const uint8_t* args, size_t args_size,
                        const uint8_t** out, size_t* out_size);

namespace codebreaker::formats::xbox::gdf {

extern const py::str DESCRIPTION;     // "read Xbox 360 disc images / SVOD (XDVDFS/GDF): info/list/extract"

inline constexpr int64_t SECTOR = 0x800;
extern const py::bytes MAGIC;         // b"MICROSOFT*XBOX*MEDIA"

// formats.xbox.ciso's exceptions (the reader names them in an answer)
struct CisoError : py::Exception {
    using py::Exception::Exception;
    const char* type() const override { return "CisoError"; }
};
struct Lz4Error : py::Exception {
    using py::Exception::Exception;
    const char* type() const override { return "Lz4Error"; }
};

// ---- the reader's questions, as native.call asks them --------------------------------------------------------------------
namespace native {
// native.call(question, data, *args), the answer read as R; what the reader raises is raised as native._exception(...)
template <class R, class... A> R ask(const char* question, const py::bytes& data, const A&... args) {
    namespace nat = py::native;
    nat::Args a;
    (a.put(args), ...);
    const std::string blob = a.done();
    const uint8_t* out = nullptr;
    size_t n = 0;
    const std::string& raw = data.raw();
    int status = ::cbf_call(question, reinterpret_cast<const uint8_t*>(raw.data()), raw.size(),
                            reinterpret_cast<const uint8_t*>(blob.data()), blob.size(), &out, &n);
    nat::Val v = nat::read_pickle(out, n);
    if (status == 1) {
        std::string name = v.items.size() > 0 ? v.items[0].s : "RuntimeError", msg = v.items.size() > 1 ? v.items[1].s : "";
        if (name == "CisoError") throw CisoError(msg);
        if (name == "Lz4Error") throw Lz4Error(msg);
        if (name == "KeyError") {
            while (!msg.empty() && msg.front() == '\'') msg.erase(msg.begin());
            while (!msg.empty() && msg.back() == '\'') msg.pop_back();
            throw py::KeyError(py::repr(py::str(msg)).utf8());
        }
        if (name == "UnicodeDecodeError") throw py::UnicodeDecodeError("'ascii' codec can't decode bytes in position 0-0: " + msg);
        if (name == "UnicodeEncodeError") throw py::UnicodeEncodeError("'latin-1' codec can't encode characters in position 0-0: " + msg);
        nat::raise_as(v);
    }
    if (status != 0) throw py::RuntimeError(std::string("formats.dll does not answer ") + question);
    if constexpr (std::is_void_v<R>) return;
    else return nat::read<R>(v);
}
}  // namespace native

// A byte source held by the reader (native/formats/xbox/discs.cc): `read_at(offset, length)`, `close()`.
// FileDevice, god::SvodReader and ciso's readers are the three kinds.
class Device {
  public:
    std::optional<int64_t> _id;       // the reader's id for it; None once closed
    Device() = default;
    Device(const Device&) = delete;
    Device& operator=(const Device&) = delete;
    virtual ~Device();                // __del__: a best-effort close

    py::bytes read_at(int64_t offset, int64_t length);
    void close();
};

// A seekable byte source over a single disc-image file.
class FileDevice : public Device {
  public:
    explicit FileDevice(const py::str& path);
};
using _FileDevice = FileDevice;

// the formats.dll answer of xdev.open_image: (id, kind "CSO" / "CCI", uncompressed size, blocks, compressed blocks),
// for the ciso reader classes a caller derives from Device
std::tuple<int64_t, py::str, int64_t, int64_t, int64_t> native_open_image(const py::str& path);

// one directory entry of a walk: (path, is folder, sector, size)
using WalkEntry = std::tuple<py::str, bool, int64_t, int64_t>;

class GDF {
  public:
    std::shared_ptr<Device> dev;
    std::optional<int64_t> _descriptor;      // where the volume descriptor is, when not at the volume's sector 32
    int64_t base = 0, root_sector = 0, root_size = 0;

    GDF(std::shared_ptr<Device> dev, std::optional<int64_t> base = std::nullopt, std::optional<int64_t> descriptor = std::nullopt);

    py::bytes _sector(int64_t sec, int64_t length);
    // A directory's entries and, under each folder, its own: [(path, is folder, sector, size)]. The entries of a
    // directory are a binary tree -- each names the offsets (in 4-byte units) of the entries before and after it in
    // name order, 0xFFFF for none -- so it is walked as one, from the first entry (a folder of a large
    // Games-on-Demand title once "held" itself, without end, when the sectors were read in a row).
    py::list<WalkEntry> _walk_dir(int64_t sector, int64_t size, const py::str& prefix = py::str(""));
    // Return [(path, size)] for every file (not directory).
    py::list<std::tuple<py::str, int64_t>> files();
    // The file's bytes; KeyError for a path that is not on the disc.
    py::bytes read(const py::str& path);
};

// Build a minimal XDVDFS/GDF disc volume (bytes) from {name: data}. Volume descriptor at sector 32, a flat root
// directory, then the file data. Enough for wrapping a default.xex (xex2god); readable by GDF above.
py::bytes build_xdvdfs(const py::dict<py::str, py::bytes>& files);

std::shared_ptr<GDF> _open(const py::str& path);

// the command: `argv` is sys.argv[1:]
void main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::gdf
