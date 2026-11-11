// cb/formats/xbox/fatx.hpp -- the FATX filesystem: reader, writer, deleted-file recovery and signature carver.
//
// Reads Xbox 360 (big-endian) and original Xbox (little-endian) FATX partitions, disk images, and raw partition dumps.
// The on-disk layout, dirent format, FAT handling, timestamp packing, orphan-recovery and file-carving approach follow
// **FATX-Tools** by aerosoul94 (MIT) -- read for format facts and reimplemented here as original code driven by this
// project's own XEX/STFS readers.
//
// Authoritative facts (per FATX-Tools):
//   - volume header  'LLLL' in the volume's byte order: magic, serial, sectors_per_cluster, root_cluster
//   - dirent         'BB42sLLLLL' (0x40 bytes): name_len, attr, name[42], first_cluster, size, created, modified, accessed
//   - timestamp      year=((t>>25)&0x7F)+epoch, month, day, hour, minute, second*2; epoch 1980 (Xbox 360) / 2000 (Xbox)
//   - FAT            16-bit if max_clusters<0xFFF0 else 32-bit; FAT aligned to 0x1000
//   - offsets        fat @ vol+0x1000 ; data @ vol+0x1000+align(max_clusters*sz,0x1000)
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/random_file.hpp"

namespace cb::formats::xbox::fatx {

inline constexpr std::uint32_t kSectorSize = 0x200;
inline constexpr std::uint32_t kPageSize = 0x1000;
inline constexpr std::uint32_t kDirentSize = 0x40;
inline constexpr std::uint32_t kMaxFilename = 42;

inline constexpr std::uint8_t kDirentNeverUsed = 0x00;
inline constexpr std::uint8_t kDirentNeverUsed2 = 0xFF;
inline constexpr std::uint8_t kDirentDeleted = 0xE5;

inline constexpr std::uint8_t kAttrDirectory = 0x10;
inline constexpr std::uint8_t kAttrArchive = 0x20;
inline constexpr std::uint8_t kValidAttributes = 0x37;   // RO | HIDDEN | SYSTEM | DIRECTORY | ARCHIVE

// Whether a byte may appear in a real FATX file name (plus 0xFF padding).
bool is_valid_name_byte(std::uint8_t b);

// A partition of a disk as the standard maps place it (size 0 = to the end).
struct PartitionDef {
    std::string_view name;
    std::uint64_t offset;
    std::uint64_t size;
};
// The Xbox 360 retail HDD map, and an Xbox 360 USB stick's single partition.
extern const std::array<PartitionDef, 5> kPartitions;
extern const std::array<PartitionDef, 1> kUsbPartitions;

enum class ByteOrder { Big, Little };   // Xbox 360 / original Xbox

// A date and time as FATX stores them (a real calendar date; seconds are even).
struct Timestamp {
    int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
    friend bool operator==(const Timestamp&, const Timestamp&) = default;
    std::string to_string() const;   // "2005-06-15 12:30"
};
// A packed FATX timestamp; nullopt if it is 0 or not a valid date.
std::optional<Timestamp> decode_timestamp(std::uint32_t packed, int epoch);
std::uint32_t encode_timestamp(const Timestamp& t, int epoch);

struct Partition {
    std::string name;
    std::uint64_t offset = 0;
    std::uint64_t size = 0;
    ByteOrder order = ByteOrder::Big;
    int epoch = 1980;
    std::uint32_t sectors_per_cluster = 32;
    std::uint32_t bytes_per_cluster = 0x4000;
    std::uint32_t root_cluster = 1;
    std::uint64_t max_clusters = 0;
    bool fat16 = true;                  // otherwise 32-bit entries
    std::uint64_t fat_offset = 0;
    std::uint64_t data_offset = 0;
};

struct Entry {
    std::string name;
    std::uint8_t attributes = 0;
    std::uint32_t first_cluster = 0;
    std::uint32_t file_size = 0;
    std::optional<Timestamp> created, modified, accessed;
    bool is_directory = false;
    bool is_deleted = false;
    std::string path;                   // from the partition root, '/'-separated
    std::uint32_t cluster = 0;          // the physical cluster the dirent was found in
    std::vector<Entry> children;        // of a directory (list_directory fills them; recover leaves them empty)
};

// What the carver found.
struct Carved {
    std::uint64_t offset = 0;           // in the data area
    std::string kind;                   // "XEX", "XBE", "STFS", "PE"
    std::uint64_t length = 0;
    std::filesystem::path path;         // where it was written
};

class Volume {
public:
    // Open an image (or a raw device file) read only; the first write operation reopens it read/write.
    explicit Volume(const std::filesystem::path& path);

    const std::filesystem::path& path() const { return file_.path(); }
    std::uint64_t size() const { return size_; }

    // ---- partitions ---------------------------------------------------------
    // Every FATX partition found: a raw partition at offset 0, the standard maps, or a whole-file single partition.
    std::vector<Partition> detect_partitions();
    // The partition at `def`, or nullopt if no FATX volume starts there.
    std::optional<Partition> mount(const PartitionDef& def);

    // ---- FAT and clusters ---------------------------------------------------
    std::uint64_t cluster_offset(const Partition& p, std::uint64_t cluster) const;
    Bytes read_cluster(const Partition& p, std::uint64_t cluster);
    // Follow the FAT from `first` until the end/reserved marker (at most the partition's cluster count).
    std::vector<std::uint32_t> cluster_chain(const Partition& p, std::uint32_t first);

    // ---- reading ----------------------------------------------------------------
    // The entries of a directory (the root when `cluster` is 0), each directory's children read in turn. A directory
    // tree that loops back on itself is a FormatError.
    std::vector<Entry> list_directory(const Partition& p, std::uint32_t cluster = 0, std::string_view base = {}, bool include_deleted = false);
    // One file: along its FAT chain, or (a deleted file's chain is cleared; `contiguous`) by clusters in order.
    void extract_file(const Partition& p, const Entry& e, const std::filesystem::path& out, bool contiguous = false);
    // Every live file under `out_dir` (folders made); returns the file count.
    std::size_t extract_all(const Partition& p, const std::filesystem::path& out_dir);

    // ---- deleted-file recovery -----------------------------------------------
    // Scan every cluster for orphaned and deleted dirents that look real. The entries come back flat; the path of one
    // inside a directory found the same way is "<directory>/<name>".
    std::vector<Entry> recover(const Partition& p);
    // Extract what recover() found (the deleted entries only, by default) under `out_dir`; returns the count.
    std::size_t recover_all(const Partition& p, const std::filesystem::path& out_dir, bool deleted_only = true);

    // ---- the signature carver ----------------------------------------------------
    // Step through the partition's data every `interval` bytes and carve files by their magic even where no dirent
    // survives (XEX / XBE / STFS / PE) into `<out_dir>/carved/`. `decrypt` is for measuring encrypted XEX exactly, which
    // needs the title key (native/xex has it): not done here, those are measured by the other rules or skipped.
    std::vector<Carved> carve(const Partition& p, const std::filesystem::path& out_dir, std::uint32_t interval = kPageSize, bool decrypt = false);

    // ---- writing -------------------------------------------------------------------
    // Write `data` as a new file at `dest_path` ("dir/sub/name" or "name"; '/' or '\\'); names are ASCII, cut at 42
    // bytes. NotFoundError if a folder of the path does not exist; IoError if the volume has too few free clusters.
    void add_file(const Partition& p, std::string_view dest_path, ByteView data);
    void mkdir(const Partition& p, std::string_view dest_path);
    // Delete a file or directory: mark its dirent deleted and free its chain. NotFoundError if there is none.
    void remove(const Partition& p, std::string_view path);

    // Raw access to the image (tests, tools).
    Bytes read_at(std::uint64_t offset, std::size_t count);

private:
    const std::vector<std::uint32_t>& fat(const Partition& p);
    std::optional<Entry> parse_dirent(const Partition& p, ByteView slot, std::string_view base, std::uint32_t cluster) const;
    void make_writable();
    void write_at(std::uint64_t offset, ByteView data);
    void set_fat(const Partition& p, std::uint32_t cluster, std::uint32_t value);
    std::vector<std::uint32_t> allocate(const Partition& p, std::size_t n);
    std::uint32_t resolve_dir(const Partition& p, std::string_view path);
    void write_dirent(const Partition& p, std::uint32_t dir_cluster, std::string_view name, std::uint8_t attr, std::uint32_t first, std::uint32_t size);
    std::vector<Entry> list_directory_at(const Partition& p, std::uint32_t cluster, std::string_view base, bool include_deleted,
                                         std::vector<std::uint32_t>& ancestors);

    fs::RandomFile file_;
    std::uint64_t size_ = 0;
    std::unordered_map<std::uint64_t, std::vector<std::uint32_t>> fat_cache_;   // by partition offset; lookup only
};

}  // namespace cb::formats::xbox::fatx
