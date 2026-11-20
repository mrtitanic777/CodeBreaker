// build/floppy.hpp -- a FAT12 boot floppy made from rebuilt files, laid out as an original one is.
//
//     const floppy::Disk disk = floppy::read(img_bytes);            disk.boot (512 bytes), disk.entries, disk.bpb
//     const Bytes img = floppy::make(disk, boot, contents);         contents {NAME.EXT: bytes}
//
// The original floppy says what goes on the disk and how: its boot parameters (the BPB in the boot sector), its root directory -- the
// files, in their order, with their attributes and times, and the volume label. make() keeps all of that and puts in the boot sector
// given and each file's contents given (a file not given keeps the original's), each file in one run of clusters, in the order of the
// directory, both FATs written to match.
//
// The command builds the Windows 95 Setup boot floppy (setup/media/boot.img of the corpus) from what CodeBreaker rebuilt: the boot sector
// from design/recon/bootdisk/msboot.asm (nasm), every real-mode program from its source (`dos-source`, <state>/bootfloppy/src/<NAME>),
// and the text files (CONFIG.SYS, AUTOEXEC.BAT, MSDOS.SYS ...) as they are -- they are source. A file with no rebuilt program is the
// original's, and the command says which:
//
//     boot-floppy [--image ORIGINAL.img] [--out FILE.img] [--extract DIR]
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "core/cli.hpp"
#include "core/error.hpp"
#include "source/dos_writer.hpp"

namespace cb::build::floppy {

inline constexpr std::size_t kSectorSize = 512;
inline constexpr std::size_t kEntrySize = 32;   // a root directory entry

// The boot sector's parameter block (the fields at 11..), and the places they give.
struct Bpb {
    std::uint16_t bytes_per_sector = 0;
    std::uint8_t sectors_per_cluster = 0;
    std::uint16_t reserved = 0;
    std::uint8_t fats = 0;
    std::uint16_t root_entries = 0;
    std::uint16_t total_sectors = 0;
    std::uint8_t media = 0;
    std::uint16_t sectors_per_fat = 0;
    std::size_t root_at = 0;   // byte offset of the root directory
    std::size_t data_at = 0;   // byte offset of the first cluster

    // The parameters that make a floppy the same layout (not the media byte).
    bool same_layout(const Bpb& other) const;
};
// FormatError for a boot sector too short to hold one.
Bpb bpb_of(ByteView boot);

struct Entry {
    std::array<std::uint8_t, kEntrySize> raw{};   // the 32-byte directory entry
    std::optional<Bytes> data;                    // the file's bytes (none for the volume label and folders)

    // "NAME.EXT" from the 8.3 fields.
    std::string name() const;
    // A file: not a volume label, a folder or a long-name part.
    bool is_file() const;
};

struct Disk {
    Bytes boot;                  // the first 512 bytes
    std::vector<Entry> entries;  // the root directory up to its end marker (a boot floppy has no folders)
    Bpb bpb;
};

// A FAT12 floppy image. FormatError when it is too short to be one.
Disk read(ByteView image);

// The contents do not fit the floppy's clusters.
class DoNotFitError : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(DoNotFitError)
};

// The image: `boot` as the boot sector, the directory of `disk` with `contents` in it. UsageError when the boot sector's parameters are not
// the disk's; DoNotFitError when the files are too big.
Bytes make(const Disk& disk, ByteView boot, const std::map<std::string, Bytes>& contents);

struct Environment {
    std::filesystem::path corpus_root;   // the original floppy is <corpus_root>/setup/media/boot.img
    std::filesystem::path design_dir;    // the boot sector's source is <design_dir>/recon/bootdisk/msboot.asm
    std::filesystem::path state;         // <this>/bootfloppy (the work folder's win95_boot, else the state folder): original/, src/, msboot.bin, boot95.img
    source::dos_writer::Options writer;  // how a program of the floppy is written back as source and built
};

// `boot-floppy [--image ORIGINAL.img] [--out FILE.img] [--extract DIR]`.
cli::Command command(const Environment& environment);

}  // namespace cb::build::floppy
