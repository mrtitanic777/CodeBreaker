// formats/disc_image.hpp -- a CD image's sectors, however the image was saved: the 2048 bytes of data each sector holds.
//
// The consoles that boot from a CD (the PlayStation, PlayStation 2, Saturn, Sega CD, Dreamcast, CD-i) and Windows 95's Setup CD are
// all read through this: an ISO 9660 volume (formats/iso9660) and a console's boot area are both addressed by sector, and the image
// on disk is one of
//
//     .iso / .img   cooked: 2048 bytes a sector, the data alone
//     .bin / .img   raw: 2352 bytes a sector -- a 12-byte sync pattern, the sector's address and mode (4 bytes), then the data:
//                   mode 1: the 2048 bytes;  mode 2 form 1 (the PlayStation's, CD-XA): an 8-byte subheader, then the 2048 bytes
//     .cue          a sheet naming the image file (its first FILE) and the first track's mode (MODE1/2352, MODE2/2352, MODE1/2048)
//
// A raw image is told from a cooked one by the sync pattern at its start, or by what the .cue sheet says; the mode is read from
// each sector's own header, so a disc that mixes modes reads right. Only the first track is read (the data track: a console's
// boot area and file system are in it; audio tracks after it are not data).
//
//     const disc::Sectors cd = disc::Sectors::open("game.cue");
//     Bytes pvd = cd.read(16, 2048);                 // the volume descriptor
//     Bytes some = cd.read_at(16 * 2048 + 1, 5);     // "CD001": an offset into the data, as if the image were cooked
#pragma once

#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>

#include "core/bytes.hpp"

namespace cb::formats::disc {

inline constexpr std::size_t kSector = 2048;
inline constexpr std::size_t kRawSector = 2352;
inline constexpr std::size_t kRawHeader = 16;      // the sync pattern, the address and the mode
inline constexpr std::size_t kSubheader = 8;       // mode 2's, after the header

// The 12-byte sync pattern that starts a raw sector.
bool raw_sync(ByteView sector);

// Where a raw sector's data starts: after the header (mode 1) or after the header and the subheader (mode 2).
std::size_t data_offset(ByteView raw_sector);

// The image a `.cue` sheet names (its first FILE, beside the sheet) and the sector size of its first track (0 when the sheet
// does not say: the image is then told by its own bytes).
struct Cue {
    std::filesystem::path file;
    std::size_t sector_size = 0;
};
std::optional<Cue> read_cue(const std::filesystem::path& sheet);

// Whether a file is a CD image this reads, by its name: `.iso`, `.bin`, `.img`, `.cue`.
bool is_image_name(const std::filesystem::path& path);

// The sectors of an image.
class Sectors {
public:
    // An image held in memory (the bytes must outlive this). `sector_size`: 2048 or 2352; 0 tells it by the sync pattern.
    static Sectors of(ByteView image, std::size_t sector_size = 0);
    // An image file, read as it is asked for (a disc is hundreds of megabytes: nothing is read whole): `.iso`, `.bin`, `.img`, or a
    // `.cue` sheet's image. FormatError for a sheet that names no file; NotFoundError when the image is not there.
    static Sectors open(const std::filesystem::path& path);
    // An image file whose layout the caller knows: `sector_size` 2048 (cooked) or 2352 (raw) with the data starting `data_skip` bytes into a raw
    // sector (16 or 24), whatever the sector headers say (an image that has no sync patterns is read too).
    static Sectors open_laid_out(const std::filesystem::path& path, std::size_t sector_size, std::size_t data_skip = kRawHeader);

    Sectors(const Sectors&);
    Sectors& operator=(const Sectors&);
    Sectors(Sectors&&) noexcept;
    Sectors& operator=(Sectors&&) noexcept;
    ~Sectors();

    bool raw() const;                          // 2352 bytes a sector
    std::uint64_t count() const;               // whole sectors in the image
    bool has(std::uint64_t lba) const { return lba < count(); }
    const std::filesystem::path& file() const;   // the image file (empty for an image in memory)

    // The data of sector `lba` (2048 bytes; empty past the end).
    Bytes sector(std::uint64_t lba) const;
    // `length` bytes of data from sector `lba` on, over as many sectors as it takes; fewer when the image ends first.
    Bytes read(std::uint64_t lba, std::size_t length) const;
    // `count` bytes at `offset` into the data, as if the image were cooked (offset / 2048 is the sector); fewer when it ends first.
    Bytes read_at(std::uint64_t offset, std::size_t count) const;

private:
    struct Impl;
    explicit Sectors(std::shared_ptr<const Impl> impl);
    std::shared_ptr<const Impl> impl_;
};

}  // namespace cb::formats::disc
