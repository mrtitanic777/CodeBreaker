// codebreaker/formats/xbox/god.hpp -- typed C++ from codebreaker.formats.xbox.god.
//
// GoD / SVOD -- Xbox 360 Games-on-Demand container read + write.
//
// A GoD (a.k.a. SVOD) package is a CON/LIVE header plus a `<mediaid>.data/` directory of `Data####` "part" files.
// Each part interleaves hash blocks among the data: a 4096-byte master hash block, then 0xCB subparts, each a
// 4096-byte subpart hash block (204 SHA-1s) followed by 204 data blocks of 0x1000. The concatenated, de-interleaved
// data blocks form an XDVDFS/GDF disc volume.
//
// Clean-room Python read (de-interleave) + write (ISO -> GoD) of that format. The block/hash layout and CON-header
// field map were learned from iso2god-rs by Ilia Pozdnyakov (MIT), reimplemented here as original Python; format
// constants and offsets are facts, not copied code.
//
// Commands:
//     god info    <god-dir|con-header>     summary of a GoD package
//     god list    <god-dir>                on-disc file listing (via GDF)
//     iso2god     <image.iso> <out-dir> [--title NAME]   convert an ISO to GoD
//
// Typing and runtime notes:
//   * The reader (native/formats/xbox/discs.cc) is asked through gdf::native::ask, as the Python asked formats.dll.
//   * write_god's `volume_read` is a std::function; `template` and the master hash are std::optional<py::bytes>
//     (the Python passes None through to the reader, which raises); `title_id`, `media_id`, `content_type` and
//     `title` are the Python's keyword-only arguments, in that order, after `template`.
//   * _template() finds empty_live.bin beside god.py: codebreaker/formats/xbox/ under the checkout root
//     (py::settings::checkout_root(), CB_CHECKOUT_ROOT).
//   * main() takes sys.argv[1:] and prints with py::print; sys.exit(n) is py::SystemExit(n).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "codebreaker/formats/xbox/gdf.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::xbox::god {

extern const py::str DESCRIPTION;     // "Xbox 360 GoD/SVOD: read (de-interleave) and convert ISO -> GoD"

inline constexpr int64_t BLOCK = 0x1000;
inline constexpr int64_t BLOCKS_PER_SUBPART = 0xCC;          // 204 data blocks per subpart
inline constexpr int64_t BLOCKS_PER_PART = 0xA1C4;           // = 0xCB * 0xCC data blocks per part
inline constexpr int64_t SUBPART_SIZE = BLOCK * BLOCKS_PER_SUBPART;

// De-interleaves a GoD `.data` directory into the logical disc volume, exposing read_at(logical_offset, length) so
// a GDF reader can walk it.
class SvodReader : public gdf::Device {
  public:
    py::list<py::str> paths;          // the part files, in order
    explicit SvodReader(const py::str& data_dir);
};

// Write a GoD package: `<out_dir>/<mediaid>.data/Data####` parts plus the CON header file `<out_dir>/<mediaid>`.
// `volume_read(off, len)` streams the de-interleaved (logical) disc volume to pack. (The block hashes and the part
// files' layout are formats.dll's; this reads the volume a part at a time and writes the files.)
// -> (the header file's path, the .data directory, the number of parts)
std::tuple<py::str, py::str, int64_t> write_god(const std::function<py::bytes(int64_t, int64_t)>& volume_read, int64_t volume_size,
                                                const py::str& out_dir, const std::optional<py::bytes>& template_,
                                                int64_t title_id, int64_t media_id, int64_t content_type = 0x7000,
                                                const py::str& title = py::str(""));

// The empty LIVE header template (from iso2god-rs, MIT). Ships alongside.
std::optional<py::bytes> _template();

// ---- NXE-installed STFS package -> GoD/LIVE header (NXE2GOD transform) ----
// Convert an NXE-installed game's STFS/CON header into a GoD/LIVE header (the NXE2GOD 'MakeGOD' transform): retype the
// package to LIVE / content-type 0x7000 and recompute the header hash. `data` is the full package-header file
// (>= 0xB000); the sibling .data block folder is copied unchanged by the user. Facts reimplemented clean-room from
// NXE2GOD; on modded consoles this needs no RSA signature (the header hash is what is checked).
py::bytes install_to_god(const py::bytes& data);

// True if the header looks like an NXE-installed game (content type 0x4000).
bool is_nxe_install(const py::bytes& data);

// the command: `argv` is sys.argv[1:]
void main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::xbox::god
