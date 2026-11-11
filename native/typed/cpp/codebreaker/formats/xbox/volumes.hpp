// codebreaker/formats/xbox/volumes.hpp -- typed C++ from codebreaker.formats.xbox.volumes.
//
// volumes.py -- Xbox and Xbox 360 containers as media volumes.
//
//     auto found = volumes::volumes(path);   [volume, ...] when `path` is an Xbox container, else None
//
// Each volume has the shape formats/media.py gives every volume (kind, label, where, boot, files() -> [Entry],
// read(entry) -> bytes), so `codebreaker media list|extract` reads these as it reads a floppy or a CD:
//
//     stfs       an STFS package (CON / LIVE / PIRS): saves, DLC, title updates, installs
//     xdvdfs     an Xbox / Xbox 360 disc image (XISO, redump XGD), plain or CSO / CCI compressed
//     god        a Games-on-Demand folder (its Data#### parts are the disc, de-interleaved)
//     fatx       each FATX partition of a hard-disk or memory-unit image
//     nand       the flash file system of a console's NAND dump (16 MB, with spare bytes)
//     heavyiron  a Heavy Iron GoodEngine archive (.ho): each asset by layer, id and type
//     zar        a ZArchive (.zar)
//     xzp        a XUI package (XUIZ)
//     pck        a Minecraft Legacy Console Edition .pck
//
// Typing and runtime notes:
//   * "[volume, ...] or None" is std::optional<py::list<std::shared_ptr<media::Volume>>>; the volume kinds are
//     the classes _Listed (a volume whose files are a list of (path, size) and a way to read one, with a std::function
//     for each) and FatxVolume, both media::Volume. `_god`, `_stfs`, ... return the optional list too.
//   * formats.xbox.ciso and formats.xbox.xzp have no typed port yet; what volumes() takes from them is given here: the
//     magic numbers CSO_MAGIC / CCI_MAGIC, and two functions to set -- `ciso_open_image` (ciso.open_image: a path -> the
//     reader, a gdf::Device) and `xzp_payloads` (xzp.payloads: a path -> [(name, bytes)]). Unset, a use of one is an
//     ImportError (as a missing module would be).
//   * A FATX volume shares its FatxReader with the others of the same drive: std::shared_ptr<FatxReader>.
//   * `with open(...)` and os.walk are done in place; os.walk's order is the directory listing's, as Python's.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>

#include "codebreaker/formats/media.hpp"
#include "codebreaker/formats/xbox/fatx.hpp"
#include "codebreaker/formats/xbox/gdf.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::xbox::volumes {

using VolumeList = py::list<std::shared_ptr<media::Volume>>;

extern const py::bytes CSO_MAGIC;            // ciso.CSO_MAGIC: b"CISO"
extern const py::bytes CCI_MAGIC;            // ciso.CCI_MAGIC: b"CCIM"

// ciso.open_image(path): the reader of a CSO / CCI image
extern std::function<std::shared_ptr<gdf::Device>(const py::str&)> ciso_open_image;
// xzp.payloads(path): every stored file as (relative path, bytes)
extern std::function<py::list<std::tuple<py::str, py::bytes>>(const py::str&)> xzp_payloads;

// A volume whose files are a list of (path, size) and a way to read one.
class _Listed : public media::Volume {
  public:
    _Listed(const py::str& kind, const py::str& label, const py::str& where, const media::Boot& boot,
            std::function<py::list<std::tuple<py::str, int64_t>>()> listing, std::function<py::bytes(const py::str&)> reader);
    py::list<media::Entry> files() override;
    py::bytes read(const media::Entry& entry) override;

  private:
    std::function<py::list<std::tuple<py::str, int64_t>>()> _listing;
    std::function<py::bytes(const py::str&)> _reader;
};

// An STFS package (CON / LIVE / PIRS): saves, DLC, title updates, installs
std::shared_ptr<media::Volume> _stfs(const py::str& path);

// A disc image, over its device. `base` and `descriptor` place the volume as the GoD header says.
std::shared_ptr<media::Volume> _xdvdfs(const std::shared_ptr<gdf::Device>& device, const py::str& label, const py::str& where,
                                       const py::str& kind = py::str("xdvdfs"), std::optional<int64_t> base = std::nullopt,
                                       std::optional<int64_t> descriptor = std::nullopt);

// A Games-on-Demand folder: the `<media id>.data` folder of Data#### parts, or the folder of the whole package (the one
// that holds it). With its `header` (the package file beside the .data folder), the volume is placed as the header's SVOD
// descriptor says: the disc sectors before its first data block are not in the parts, and in the "enhanced GDF" layout
// the volume descriptor comes first, the volume's sectors a 4 KB block after it.
std::optional<VolumeList> _god(const py::str& folder, const std::optional<py::str>& header = std::nullopt);

// The Data#### files of a package's .data folder, as they are.
VolumeList _parts(const py::str& folder);

// One FATX partition of a drive or memory-unit image.
class FatxVolume : public media::Volume {
  public:
    FatxVolume(const std::shared_ptr<fatx::FatxReader>& reader, const fatx::PartitionRef& partition);
    py::list<media::Entry> files() override;
    py::bytes read(const media::Entry& entry) override;

  private:
    std::shared_ptr<fatx::FatxReader> _r;
    fatx::PartitionRef _p;
    std::optional<py::dict<py::str, fatx::EntryRef>> _entries;
    const py::dict<py::str, fatx::EntryRef>& _all();
};

std::optional<VolumeList> _fatx(const py::str& path);
VolumeList _nand(const py::str& path);
VolumeList _zar(const py::str& path);
VolumeList _payloads(const py::str& kind, const py::str& path, const py::list<std::tuple<py::str, py::bytes>>& found);

// The volumes of `path` when it is an Xbox container, else None (it is then some other kind of image, or not one).
// `long_names` is media's option; Xbox names are always long.
std::optional<VolumeList> volumes(const py::str& path, bool long_names = true);

}  // namespace codebreaker::formats::xbox::volumes
