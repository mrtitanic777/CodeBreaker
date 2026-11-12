// cb/formats/xbox/god.hpp -- Xbox 360 Games on Demand (GoD / SVOD) packages: reading one back into the disc volume it holds, and
// writing one from a volume.
//
// A GoD package is a header file in the CON/LIVE format plus a `<mediaid>.data/` folder of `Data0000`, `Data0001` ... part files.
// A part interleaves hash blocks among the data: a 4096-byte master hash block, then up to 0xCB subparts, each a 4096-byte hash
// block (the SHA-1 of each of its 204 data blocks, 20 bytes apiece) followed by those 204 data blocks of 4096 bytes. The master
// block holds the SHA-1 of each subpart's hash block; the header file holds the SHA-1 of the first part's master block. Taking
// the data blocks of all parts in order gives the logical volume, an XDVDFS disc image.
//
// The block and hash layout and the header's field map were learned from iso2god-rs by Ilia Pozdnyakov (MIT); the format's
// constants and offsets are facts, and this is original code.
//
//     SvodReader volume = SvodReader::open("00004000/0000ABCD.data");
//     Bytes sector = volume.read_at(0x10000, 2048);
//
//     WriteResult done = write_god(read_volume, volume_size, out_dir, template_header, {.title_id = tid, .media_id = mid});
//
// Not here: finding the volume's title in a disc image and building a volume from files is the XDVDFS reader's (xbox/gdf); the
// writer takes the volume as a callback and the title ids as parameters.
//
// Errors: NotFoundError for a missing folder or file, FormatError for a folder with no part files and for a header too short to
// be one, UsageError for values that do not fit the header's fields.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"

namespace cb::formats::xbox::god {

inline constexpr std::uint64_t kBlock = 0x1000;
inline constexpr std::uint64_t kBlocksPerSubpart = 0xCC;                      // 204 data blocks per subpart
inline constexpr std::uint64_t kSubpartsPerPart = 0xCB;
inline constexpr std::uint64_t kBlocksPerPart = kSubpartsPerPart * kBlocksPerSubpart;   // 0xA1C4 data blocks per part
inline constexpr std::uint64_t kSubpartSize = kBlock * kBlocksPerSubpart;     // the data of one subpart

inline constexpr std::uint32_t kDefaultContentType = 0x7000;   // "Games on Demand"

// ---- reading -------------------------------------------------------------------------------------------------------------

// The logical disc volume of a package: its parts, de-interleaved on demand. Not thread-safe (the part files are opened as
// they are first read).
class SvodReader {
public:
    // The `Data####` part files of the `.data` folder (the files whose names start with "data", in name order). NotFoundError if the
    // folder is missing, FormatError if it holds none.
    static SvodReader open(const std::filesystem::path& data_dir);

    const std::vector<std::filesystem::path>& parts() const { return parts_; }
    // The bytes of the logical volume the parts hold. A part that is shorter than its hash layout says contributes what it holds.
    std::uint64_t size() const;
    // Up to `length` bytes of the volume from `offset`: fewer where the parts end (none at or past the end).
    Bytes read_at(std::uint64_t offset, std::size_t length) const;

    SvodReader(SvodReader&&) noexcept;
    SvodReader& operator=(SvodReader&&) noexcept;
    ~SvodReader();

private:
    SvodReader();
    struct Impl;
    std::unique_ptr<Impl> impl_;
    std::vector<std::filesystem::path> parts_;
};

// ---- writing -------------------------------------------------------------------------------------------------------------

// The volume to pack: `read(offset, length)` returns the bytes at that place; a short answer reads as zeros for the rest.
using VolumeReader = std::function<Bytes(std::uint64_t offset, std::size_t length)>;

// What goes into the package header besides the sizes.
struct Title {
    std::uint32_t title_id = 0;
    std::uint32_t media_id = 0;
    std::uint32_t content_type = kDefaultContentType;
    std::string title;   // UTF-8; the header keeps the first 63 UTF-16 units, in its display-name and title-name places
};

struct WriteResult {
    std::filesystem::path header;     // `<out>/<TITLEID>/<CONTENTTYPE>/<MEDIAID>`
    std::filesystem::path data_dir;   // `<header>.data`
    std::size_t part_count = 0;
};

// Write the package of a volume of `volume_size` bytes under `out_dir`: the `Data####` parts (each part's blocks hashed as above)
// and the header, made from `header_template` (the empty LIVE header, 0xB000 bytes) with the title, sizes and the hash of the first
// part's master block filled in and its own hash recomputed. UsageError for an empty volume or sizes the header cannot hold;
// FormatError for a template too short.
WriteResult write_god(const VolumeReader& read, std::uint64_t volume_size, const std::filesystem::path& out_dir,
                      ByteView header_template, const Title& title);

// The header of a package: `header_template` with the fields set. `blocks_allocated` is the volume's block count, `parts_total_size`
// the bytes of all the part files, `first_master_hash` the SHA-1 of part 0's master hash block.
Bytes make_header(ByteView header_template, const Title& title, std::uint64_t blocks_allocated, std::uint64_t part_count,
                  std::uint64_t parts_total_size, ByteView first_master_hash);

// ---- NXE-installed games -------------------------------------------------------------------------------------------------

// An NXE-installed game's STFS/CON header as a GoD/LIVE header (the NXE2GOD transform): retyped as LIVE with content type 0x7000
// and the header hash recomputed. `header` is the package-header file (0xB000 bytes); the sibling `.data` folder is copied
// unchanged by the user. FormatError if it is shorter than 0x413 bytes. (On a modded console the header hash is what is checked,
// no RSA signature.)
Bytes install_to_god(ByteView header);
// Whether a header looks like an NXE-installed game's (content type 0x4000).
bool is_nxe_install(ByteView header);

// ---- the command ---------------------------------------------------------------------------------------------------------

// The tool's usage text.
std::string_view usage();
// `god god2iso <data-dir> <out.iso>` and `god install2god <header> <out-header>`: the report on stdout, the exit code. The tool's
// other operations (info, list, iso2god, xex2god) read and build XDVDFS volumes and are the disc-image tool's; they are UsageErrors
// here. No arguments, `-h` or `--help` print the usage and return 0.
int command(std::span<const std::string> args);

}  // namespace cb::formats::xbox::god
