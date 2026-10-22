// STFS format helpers (stfs/format.h): header fields, volume descriptor, directory entries, block layout, SHA-1.

#include "stfs/format.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <bcrypt.h>

#include <algorithm>
#include <cstring>
#include <format>

namespace stfs
{
    const char* MagicName(Magic magic)
    {
        switch (magic)
        {
        case Magic::Con: return "CON";
        case Magic::Live: return "LIVE";
        case Magic::Pirs: return "PIRS";
        }
        return "?";
    }

    Sha1Digest Sha1(std::span<const uint8_t> data)
    {
        static BCRYPT_ALG_HANDLE algorithm = []
        {
            BCRYPT_ALG_HANDLE h = nullptr;
            BCryptOpenAlgorithmProvider(&h, BCRYPT_SHA1_ALGORITHM, nullptr, 0);
            return h;
        }();
        Sha1Digest digest{};
        BCryptHash(algorithm, nullptr, 0, const_cast<PUCHAR>(data.data()), ULONG(data.size()), digest.data(),
                   ULONG(digest.size()));
        return digest;
    }

    namespace
    {
        std::u16string ReadUtf16(const uint8_t* p, size_t bytes)
        {
            std::u16string s;
            for (size_t i = 0; i + 1 < bytes; i += 2)
            {
                const char16_t c = char16_t(Be16(p + i));
                if (!c)
                    break;
                s.push_back(c);
            }
            return s;
        }

        // Writes s (truncated, NUL-terminated when it fits) and zero-fills the rest of the field.
        void WriteUtf16(uint8_t* p, size_t bytes, const std::u16string& s)
        {
            std::memset(p, 0, bytes);
            const size_t n = std::min(s.size(), bytes / 2 - 1);
            for (size_t i = 0; i < n; ++i)
                PutBe16(p + i * 2, uint16_t(s[i]));
        }
    }

    VolumeDescriptor ParseVolumeDescriptor(const uint8_t* p)
    {
        VolumeDescriptor v;
        v.length = p[0];
        v.version = p[1];
        v.flags = p[2];
        v.fileTableBlocks = Le16(p + 3);
        v.fileTableBlock = Le24(p + 5);
        std::memcpy(v.topHash.data(), p + 8, 20);
        v.totalBlocks = Be32(p + 0x1C);
        v.freeBlocks = Be32(p + 0x20);
        return v;
    }

    void WriteVolumeDescriptor(const VolumeDescriptor& v, uint8_t* p)
    {
        p[0] = v.length;
        p[1] = v.version;
        p[2] = v.flags;
        PutLe16(p + 3, v.fileTableBlocks);
        PutLe24(p + 5, v.fileTableBlock);
        std::memcpy(p + 8, v.topHash.data(), 20);
        PutBe32(p + 0x1C, v.totalBlocks);
        PutBe32(p + 0x20, v.freeBlocks);
    }

    bool ParseMetadata(std::span<const uint8_t> header, Metadata& m, std::string* error)
    {
        auto fail = [&](std::string message)
        {
            if (error)
                *error = std::move(message);
            return false;
        };
        if (header.size() < offsets::kMetadataEnd)
            return fail("header too short");
        const uint8_t* h = header.data();
        const uint32_t magic = Be32(h);
        if (magic != uint32_t(Magic::Con) && magic != uint32_t(Magic::Live) && magic != uint32_t(Magic::Pirs))
            return fail(std::format("not an STFS package (magic {:08X})", magic));
        m.magic = Magic(magic);
        m.headerSize = Be32(h + offsets::kHeaderSize);
        std::memcpy(m.headerHash.data(), h + offsets::kHeaderHash, 20);
        m.contentType = Be32(h + offsets::kContentType);
        m.metadataVersion = Be32(h + offsets::kMetadataVersion);
        m.contentSize = Be64(h + offsets::kContentSize);
        m.mediaId = Be32(h + offsets::kMediaId);
        m.version = Be32(h + offsets::kVersion);
        m.baseVersion = Be32(h + offsets::kBaseVersion);
        m.titleId = Be32(h + offsets::kTitleId);
        m.platform = h[offsets::kPlatform];
        m.executableType = h[offsets::kExecutableType];
        m.discNumber = h[offsets::kDiscNumber];
        m.discsInSet = h[offsets::kDiscsInSet];
        m.saveGameId = Be32(h + offsets::kSaveGameId);
        std::memcpy(m.consoleId.data(), h + offsets::kConsoleId, 5);
        m.profileId = Be64(h + offsets::kProfileId);
        m.volume = ParseVolumeDescriptor(h + offsets::kVolumeDescriptor);
        m.dataFileCount = Be32(h + offsets::kDataFileCount);
        m.dataFileSize = Be64(h + offsets::kDataFileSize);
        m.volumeType = Be32(h + offsets::kVolumeType);
        m.onlineCreator = Be64(h + offsets::kOnlineCreator);
        m.category = Be32(h + offsets::kCategory);
        std::memcpy(m.deviceId.data(), h + offsets::kDeviceId, 20);
        const bool v2 = m.metadataVersion >= 2;
        for (uint32_t i = 0; i < kLanguages; ++i)
        {
            if (i < kLanguagesV1)
            {
                m.displayNames[i] = ReadUtf16(h + offsets::kDisplayNames + i * 0x100, 0x100);
                m.descriptions[i] = ReadUtf16(h + offsets::kDescriptions + i * 0x100, 0x100);
            }
            else
            {
                m.displayNames[i] = v2 ? ReadUtf16(h + offsets::kDisplayNamesV2 + (i - kLanguagesV1) * 0x100, 0x100) : u"";
                m.descriptions[i] = v2 ? ReadUtf16(h + offsets::kDescriptionsV2 + (i - kLanguagesV1) * 0x100, 0x100) : u"";
            }
        }
        m.publisher = ReadUtf16(h + offsets::kPublisher, 0x80);
        m.titleName = ReadUtf16(h + offsets::kTitleName, 0x80);
        m.transferFlags = h[offsets::kTransferFlags];
        const uint32_t maxThumb = v2 ? 0x3D00 : 0x4000;
        const uint32_t thumb = std::min(Be32(h + offsets::kThumbnailSize), maxThumb);
        const uint32_t titleThumb = std::min(Be32(h + offsets::kTitleThumbnailSize), maxThumb);
        m.thumbnail.assign(h + offsets::kThumbnail, h + offsets::kThumbnail + thumb);
        m.titleThumbnail.assign(h + offsets::kTitleThumbnail, h + offsets::kTitleThumbnail + titleThumb);
        return true;
    }

    void WriteMetadata(const Metadata& m, std::vector<uint8_t>& header)
    {
        if (header.size() < offsets::kMetadataEnd)
            header.resize(offsets::kMetadataEnd);
        uint8_t* h = header.data();
        PutBe32(h, uint32_t(m.magic));
        PutBe32(h + offsets::kHeaderSize, m.headerSize);
        PutBe32(h + offsets::kContentType, m.contentType);
        PutBe32(h + offsets::kMetadataVersion, m.metadataVersion);
        PutBe64(h + offsets::kContentSize, m.contentSize);
        PutBe32(h + offsets::kMediaId, m.mediaId);
        PutBe32(h + offsets::kVersion, m.version);
        PutBe32(h + offsets::kBaseVersion, m.baseVersion);
        PutBe32(h + offsets::kTitleId, m.titleId);
        h[offsets::kPlatform] = m.platform;
        h[offsets::kExecutableType] = m.executableType;
        h[offsets::kDiscNumber] = m.discNumber;
        h[offsets::kDiscsInSet] = m.discsInSet;
        PutBe32(h + offsets::kSaveGameId, m.saveGameId);
        std::memcpy(h + offsets::kConsoleId, m.consoleId.data(), 5);
        PutBe64(h + offsets::kProfileId, m.profileId);
        WriteVolumeDescriptor(m.volume, h + offsets::kVolumeDescriptor);
        PutBe32(h + offsets::kDataFileCount, m.dataFileCount);
        PutBe64(h + offsets::kDataFileSize, m.dataFileSize);
        PutBe32(h + offsets::kVolumeType, m.volumeType);
        PutBe64(h + offsets::kOnlineCreator, m.onlineCreator);
        PutBe32(h + offsets::kCategory, m.category);
        std::memcpy(h + offsets::kDeviceId, m.deviceId.data(), 20);
        const bool v2 = m.metadataVersion >= 2;
        for (uint32_t i = 0; i < kLanguages; ++i)
        {
            if (i < kLanguagesV1)
            {
                WriteUtf16(h + offsets::kDisplayNames + i * 0x100, 0x100, m.displayNames[i]);
                WriteUtf16(h + offsets::kDescriptions + i * 0x100, 0x100, m.descriptions[i]);
            }
            else if (v2)
            {
                WriteUtf16(h + offsets::kDisplayNamesV2 + (i - kLanguagesV1) * 0x100, 0x100, m.displayNames[i]);
                WriteUtf16(h + offsets::kDescriptionsV2 + (i - kLanguagesV1) * 0x100, 0x100, m.descriptions[i]);
            }
        }
        WriteUtf16(h + offsets::kPublisher, 0x80, m.publisher);
        WriteUtf16(h + offsets::kTitleName, 0x80, m.titleName);
        h[offsets::kTransferFlags] = m.transferFlags;
        const size_t maxThumb = v2 ? 0x3D00 : 0x4000;
        const size_t thumb = std::min(m.thumbnail.size(), maxThumb);
        const size_t titleThumb = std::min(m.titleThumbnail.size(), maxThumb);
        PutBe32(h + offsets::kThumbnailSize, uint32_t(thumb));
        PutBe32(h + offsets::kTitleThumbnailSize, uint32_t(titleThumb));
        std::memset(h + offsets::kThumbnail, 0, maxThumb);
        std::memset(h + offsets::kTitleThumbnail, 0, maxThumb);
        std::memcpy(h + offsets::kThumbnail, m.thumbnail.data(), thumb);
        std::memcpy(h + offsets::kTitleThumbnail, m.titleThumbnail.data(), titleThumb);
    }

    Layout Layout::For(uint32_t headerSize, const VolumeDescriptor& volume)
    {
        Layout l;
        l.base = (uint64_t(headerSize) + kBlockSize - 1) & ~uint64_t(kBlockSize - 1);
        l.tablesPerBlock = volume.TablesPerHashBlock();
        return l;
    }

    uint64_t Layout::DataBlockToBacking(uint32_t block) const
    {
        // One level-0 table per 170 data blocks so far (including the one in front of this block), one level-1
        // table per 28900 once past the first 170, one level-2 table once past the first 28900.
        uint64_t backing = block;
        uint64_t base = kBlocksPerLevel[0];
        for (int level = 0; level < 3; ++level)
        {
            backing += ((uint64_t(block) + base) / base) * tablesPerBlock;
            if (block < base)
                break;
            base *= kBlocksPerLevel[0];
        }
        return backing;
    }

    uint64_t Layout::HashTableBacking(uint32_t block, int level) const
    {
        const uint64_t step0 = kBlocksPerLevel[0] + tablesPerBlock;          // one level-0 group
        const uint64_t step1 = kBlocksPerLevel[1] + (kBlocksPerLevel[0] + 1) * tablesPerBlock; // one level-1 group
        if (level == 2)
            return step1;
        if (block < kBlocksPerLevel[level])
            return level == 0 ? 0 : step0;
        uint64_t backing = (block / kBlocksPerLevel[level]) * (level == 0 ? step0 : step1);
        if (level == 0)
        {
            backing += (block / kBlocksPerLevel[1] + 1) * tablesPerBlock;
            if (block < kBlocksPerLevel[1])
                return backing;
        }
        return backing + tablesPerBlock;
    }

    int TopHashLevel(uint32_t totalBlocks)
    {
        if (totalBlocks <= kBlocksPerLevel[0])
            return 0;
        if (totalBlocks <= kBlocksPerLevel[1])
            return 1;
        return 2;
    }

    uint32_t HashTableCount(uint32_t totalBlocks, int level)
    {
        if (level > TopHashLevel(totalBlocks))
            return 0;
        if (totalBlocks == 0)
            return level == 0 ? 1 : 0;
        return (totalBlocks + kBlocksPerLevel[level] - 1) / kBlocksPerLevel[level];
    }

    uint64_t BackingSize(const Layout& layout, uint32_t totalBlocks)
    {
        if (totalBlocks == 0)
            return uint64_t(layout.tablesPerBlock) * kBlockSize;
        const uint32_t last = totalBlocks - 1;
        uint64_t end = layout.DataBlockToBacking(last) + 1;
        for (int level = 0; level <= TopHashLevel(totalBlocks); ++level)
            end = std::max(end, layout.HashTableBacking(last, level) + layout.tablesPerBlock);
        return end * kBlockSize;
    }

    bool ParseDirEntry(const uint8_t* p, DirEntry& e)
    {
        const uint8_t flags = p[0x28];
        const uint32_t length = flags & 0x3F;
        if (length == 0 || p[0] == 0)
            return false;
        e.name.assign(reinterpret_cast<const char*>(p), std::min(length, kMaxNameLength));
        e.directory = flags & 0x80;
        e.contiguous = flags & 0x40;
        e.validBlocks = Le24(p + 0x29);
        e.allocatedBlocks = Le24(p + 0x2C);
        e.startBlock = Le24(p + 0x2F);
        const uint16_t parent = Be16(p + 0x32);
        e.parent = parent == 0xFFFF ? -1 : int32_t(parent);
        e.size = Be32(p + 0x34);
        e.updated = Be32(p + 0x38);
        e.accessed = Be32(p + 0x3C);
        return true;
    }

    void WriteDirEntry(const DirEntry& e, uint8_t* p)
    {
        std::memset(p, 0, kDirEntrySize);
        const size_t length = std::min<size_t>(e.name.size(), kMaxNameLength);
        std::memcpy(p, e.name.data(), length);
        p[0x28] = uint8_t(length | (e.directory ? 0x80 : 0) | (e.contiguous ? 0x40 : 0));
        PutLe24(p + 0x29, e.validBlocks);
        PutLe24(p + 0x2C, e.allocatedBlocks);
        PutLe24(p + 0x2F, e.startBlock);
        PutBe16(p + 0x32, e.parent < 0 ? 0xFFFF : uint16_t(e.parent));
        PutBe32(p + 0x34, e.size);
        PutBe32(p + 0x38, e.updated);
        PutBe32(p + 0x3C, e.accessed);
    }
}
