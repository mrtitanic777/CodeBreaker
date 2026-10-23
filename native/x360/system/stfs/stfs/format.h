#pragma once

// STFS on-disk format (Shared/docs/stfs.md): constants, the XContent header's metadata, the volume descriptor and the
// block layout (where data blocks and hash tables sit in the file). Everything here is pure arithmetic on bytes; the
// file access is in stfs/package.h, writing in stfs/builder.h.

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace stfs
{
    constexpr uint32_t kBlockSize = 0x1000;
    constexpr uint32_t kHashesPerTable = 170;    // 24-byte entries in one 4 KiB hash table
    constexpr uint32_t kHashEntrySize = 24;      // SHA-1 (20) + info (BE u32)
    constexpr uint32_t kBlocksPerLevel[3] = {170, 28900, 4913000}; // data blocks covered by one table of each level
    constexpr uint32_t kMaxBlocks = 4913000;     // one level-2 table
    constexpr uint32_t kEndOfChain = 0xFFFFFF;
    constexpr uint32_t kDirEntrySize = 0x40;
    constexpr uint32_t kDirEntriesPerBlock = kBlockSize / kDirEntrySize; // 64
    constexpr uint32_t kMaxNameLength = 0x28;

    // Header offsets (from the start of the package).
    namespace offsets
    {
        constexpr uint32_t kMagic = 0x000;
        constexpr uint32_t kCertificate = 0x004;     // CON: console certificate, 0x1A8 bytes
        constexpr uint32_t kConSignature = 0x1AC;    // CON: console signature, 0x80 bytes (byte-reversed)
        constexpr uint32_t kLiveSignature = 0x004;   // LIVE / PIRS: Microsoft's signature, 0x100 bytes
        constexpr uint32_t kLicenses = 0x22C;        // 16 x 0x10; the signed region starts here
        constexpr uint32_t kHeaderHash = 0x32C;      // SHA-1 of [0x344, first hash table)
        constexpr uint32_t kHeaderSize = 0x340;
        constexpr uint32_t kSignedEnd = 0x344;       // signed region = [0x22C, 0x344)
        constexpr uint32_t kContentType = 0x344;
        constexpr uint32_t kMetadataVersion = 0x348;
        constexpr uint32_t kContentSize = 0x34C;
        constexpr uint32_t kMediaId = 0x354;
        constexpr uint32_t kVersion = 0x358;
        constexpr uint32_t kBaseVersion = 0x35C;
        constexpr uint32_t kTitleId = 0x360;
        constexpr uint32_t kPlatform = 0x364;
        constexpr uint32_t kExecutableType = 0x365;
        constexpr uint32_t kDiscNumber = 0x366;
        constexpr uint32_t kDiscsInSet = 0x367;
        constexpr uint32_t kSaveGameId = 0x368;
        constexpr uint32_t kConsoleId = 0x36C;       // 5 bytes
        constexpr uint32_t kProfileId = 0x371;       // BE u64
        constexpr uint32_t kVolumeDescriptor = 0x379; // 0x24 bytes
        constexpr uint32_t kDataFileCount = 0x39D;
        constexpr uint32_t kDataFileSize = 0x3A1;    // BE u64
        constexpr uint32_t kVolumeType = 0x3A9;      // 0 STFS, 1 SVOD
        constexpr uint32_t kOnlineCreator = 0x3AD;   // BE u64
        constexpr uint32_t kCategory = 0x3B5;
        constexpr uint32_t kSeriesId = 0x3D9;        // metadata v2: series (0x10), season (0x10), season / episode u16
        constexpr uint32_t kDeviceId = 0x3FD;        // 0x14 bytes
        constexpr uint32_t kDisplayNames = 0x411;    // 9 x 0x100 (UTF-16BE, 128 chars)
        constexpr uint32_t kDescriptions = 0xD11;    // 9 x 0x100
        constexpr uint32_t kPublisher = 0x1611;      // 0x80
        constexpr uint32_t kTitleName = 0x1691;      // 0x80
        constexpr uint32_t kTransferFlags = 0x1711;
        constexpr uint32_t kThumbnailSize = 0x1712;
        constexpr uint32_t kTitleThumbnailSize = 0x1716;
        constexpr uint32_t kThumbnail = 0x171A;      // 0x4000 (v2: 0x3D00, then 3 more display names)
        constexpr uint32_t kTitleThumbnail = 0x571A; // 0x4000 (v2: 0x3D00, then 3 more descriptions)
        constexpr uint32_t kDisplayNamesV2 = 0x541A;
        constexpr uint32_t kDescriptionsV2 = 0x941A;
        constexpr uint32_t kMetadataEnd = 0x971A;
    }

    constexpr uint32_t kCertificateSize = 0x1A8;
    constexpr uint32_t kConSignatureSize = 0x80;
    constexpr uint32_t kLanguagesV1 = 9;
    constexpr uint32_t kLanguages = 12;          // v2 adds 3
    constexpr uint32_t kDefaultHeaderSize = 0x971A; // CON packages the library creates (first hash table at 0xA000)

    enum class Magic : uint32_t
    {
        Con = 0x434F4E20,  // "CON " signed by a console
        Live = 0x4C495645, // "LIVE" signed by Microsoft (Xbox LIVE downloads)
        Pirs = 0x50495253, // "PIRS" signed by Microsoft (system / offline content)
    };
    const char* MagicName(Magic magic);

    struct VolumeDescriptor
    {
        uint8_t length = 0x24;
        uint8_t version = 0;
        uint8_t flags = 0;              // bit 0 read-only format (one table per hash block), bit 1 root active index
        uint16_t fileTableBlocks = 0;   // LE
        uint32_t fileTableBlock = 0;    // LE24
        std::array<uint8_t, 20> topHash{};
        uint32_t totalBlocks = 0;       // BE: data blocks allocated
        uint32_t freeBlocks = 0;        // BE

        bool ReadOnlyFormat() const { return flags & 1; }
        bool RootActiveIndex() const { return flags & 2; }
        uint32_t TablesPerHashBlock() const { return ReadOnlyFormat() ? 1 : 2; }
    };

    struct Metadata
    {
        Magic magic = Magic::Con;
        uint32_t headerSize = kDefaultHeaderSize;
        uint32_t contentType = 0;
        uint32_t metadataVersion = 2;
        uint64_t contentSize = 0;
        uint32_t mediaId = 0, version = 0, baseVersion = 0, titleId = 0;
        uint8_t platform = 0, executableType = 0, discNumber = 0, discsInSet = 0;
        uint32_t saveGameId = 0;
        std::array<uint8_t, 5> consoleId{};
        uint64_t profileId = 0;
        VolumeDescriptor volume;
        uint32_t dataFileCount = 0;
        uint64_t dataFileSize = 0;
        uint32_t volumeType = 0;
        uint64_t onlineCreator = 0;
        uint32_t category = 0;
        std::array<uint8_t, 20> deviceId{};
        std::array<std::u16string, kLanguages> displayNames, descriptions; // [9..11] only with metadata v2
        std::u16string publisher, titleName;
        uint8_t transferFlags = 0;
        std::vector<uint8_t> thumbnail, titleThumbnail;
        std::array<uint8_t, 20> headerHash{};
    };

    // Content types (Xenia's XContentType names; the values the dashboard cares about).
    namespace content_type
    {
        constexpr uint32_t kSavedGame = 0x00000001;
        constexpr uint32_t kMarketplace = 0x00000002;
        constexpr uint32_t kPublisher = 0x00000003;
        constexpr uint32_t kInstalledGame = 0x00004000;
        constexpr uint32_t kTheme = 0x00030000;
        constexpr uint32_t kGamerPicture = 0x00020000;
        constexpr uint32_t kProfile = 0x00010000;
        constexpr uint32_t kGameTitle = 0x000A0000;
        constexpr uint32_t kAvatarItem = 0x00009000;
        constexpr uint32_t kCacheFile = 0x00040000;
        constexpr uint32_t kStoragePack = 0x00008000; // the system update's packages (FFFE07DF*)
        constexpr uint32_t kInstaller = 0x000B0000;  // su* (system update)
    }

    // Big / little-endian access to byte buffers.
    inline uint16_t Be16(const uint8_t* p) { return uint16_t(p[0] << 8 | p[1]); }
    inline uint32_t Be24(const uint8_t* p) { return uint32_t(p[0]) << 16 | uint32_t(p[1]) << 8 | p[2]; }
    inline uint32_t Be32(const uint8_t* p) { return uint32_t(p[0]) << 24 | uint32_t(p[1]) << 16 | uint32_t(p[2]) << 8 | p[3]; }
    inline uint64_t Be64(const uint8_t* p) { return uint64_t(Be32(p)) << 32 | Be32(p + 4); }
    inline uint16_t Le16(const uint8_t* p) { return uint16_t(p[0] | p[1] << 8); }
    inline uint32_t Le24(const uint8_t* p) { return p[0] | uint32_t(p[1]) << 8 | uint32_t(p[2]) << 16; }
    inline void PutBe16(uint8_t* p, uint16_t v) { p[0] = uint8_t(v >> 8); p[1] = uint8_t(v); }
    inline void PutBe32(uint8_t* p, uint32_t v) { p[0] = uint8_t(v >> 24); p[1] = uint8_t(v >> 16); p[2] = uint8_t(v >> 8); p[3] = uint8_t(v); }
    inline void PutBe64(uint8_t* p, uint64_t v) { PutBe32(p, uint32_t(v >> 32)); PutBe32(p + 4, uint32_t(v)); }
    inline void PutLe16(uint8_t* p, uint16_t v) { p[0] = uint8_t(v); p[1] = uint8_t(v >> 8); }
    inline void PutLe24(uint8_t* p, uint32_t v) { p[0] = uint8_t(v); p[1] = uint8_t(v >> 8); p[2] = uint8_t(v >> 16); }

    // SHA-1 through Windows bcrypt.
    using Sha1Digest = std::array<uint8_t, 20>;
    Sha1Digest Sha1(std::span<const uint8_t> data);

    // Header <-> Metadata. ParseMetadata needs at least kMetadataEnd bytes. WriteMetadata writes every field above
    // except headerHash into header (resized to at least kMetadataEnd); bytes it has no field for (licenses,
    // signature, reserved areas, anything past 0x971A) are left as they are.
    bool ParseMetadata(std::span<const uint8_t> header, Metadata& out, std::string* error = nullptr);
    void WriteMetadata(const Metadata& meta, std::vector<uint8_t>& header);
    VolumeDescriptor ParseVolumeDescriptor(const uint8_t* p);
    void WriteVolumeDescriptor(const VolumeDescriptor& volume, uint8_t* p);

    // Where blocks sit. A "backing block" is a 4 KiB block counted from the first hash table (the header rounded up
    // to 4 KiB); data blocks are numbered without the hash tables between them. Layout (Xenia's
    // StfsContainerDevice, BSD-3): level-0 table 0, data 0..169, level-1 table 0, level-0 table 1, data 170..339,
    // level-0 table 2, ...; after the first 28900 data blocks the level-2 table, then level-1 table 1, ...
    // Read-write packages keep two copies of every table (tablesPerBlock 2), side by side.
    struct Layout
    {
        uint64_t base = 0;            // file offset of backing block 0
        uint32_t tablesPerBlock = 1;  // 1 read-only format, 2 read-write

        static Layout For(uint32_t headerSize, const VolumeDescriptor& volume);
        uint64_t DataBlockToBacking(uint32_t block) const;
        // Backing block of the (first copy of the) level-<level> table that covers data block <block>.
        uint64_t HashTableBacking(uint32_t block, int level) const;
        uint64_t Offset(uint64_t backing) const { return base + backing * kBlockSize; }
    };

    // Highest hash level present for a package of totalBlocks data blocks (the table the top hash covers).
    int TopHashLevel(uint32_t totalBlocks);
    // Record of data block <block> in its level-<level> table.
    inline uint32_t HashRecord(uint32_t block, int level)
    {
        return level == 0 ? block % kHashesPerTable : (block / kBlocksPerLevel[level - 1]) % kHashesPerTable;
    }
    // Number of level-<level> tables a package of totalBlocks data blocks has.
    uint32_t HashTableCount(uint32_t totalBlocks, int level);
    // Bytes from backing block 0 to the end of the last block (the header's content size).
    uint64_t BackingSize(const Layout& layout, uint32_t totalBlocks);

    // Directory entry (0x40 bytes in the file table).
    struct DirEntry
    {
        std::string name;             // ASCII / CP-1252 bytes as stored
        bool directory = false;
        bool contiguous = false;
        uint32_t validBlocks = 0;     // LE24 at 0x29
        uint32_t allocatedBlocks = 0; // LE24 at 0x2C
        uint32_t startBlock = 0;      // LE24 at 0x2F
        int32_t parent = -1;          // BE16 at 0x32, 0xFFFF = root
        uint32_t size = 0;            // BE32 at 0x34
        uint32_t updated = 0;         // BE32 at 0x38, FAT date (high 16) and time (low 16)
        uint32_t accessed = 0;        // BE32 at 0x3C
    };
    bool ParseDirEntry(const uint8_t* p, DirEntry& out); // false for an empty slot
    void WriteDirEntry(const DirEntry& entry, uint8_t* p);
}
