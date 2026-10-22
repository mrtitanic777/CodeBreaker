#include "xex.h"

#include "log.h"
#include "memory.h"

#include <cstring>
#include <fstream>
#include <iterator>
#include <map>

namespace xex
{
    namespace
    {
        constexpr uint32_t kHeaderFileFormatInfo = 0x000003FF;
        constexpr uint32_t kHeaderEntryPoint = 0x00010100;
        constexpr uint32_t kHeaderImageBase = 0x00010201;
        constexpr uint32_t kHeaderImportLibraries = 0x000103FF;
        constexpr uint32_t kHeaderTlsInfo = 0x00020104;
        constexpr uint32_t kHeaderDefaultStackSize = 0x00020200;
        constexpr uint32_t kHeaderExecutionInfo = 0x00040006;
        constexpr uint32_t kHeaderResourceInfo = 0x000002FF;

        uint32_t be32(const std::vector<uint8_t>& d, size_t o)
        {
            if (o + 4 > d.size())
                Fatal("XEX truncated reading offset {:X}", o);
            return uint32_t(d[o]) << 24 | uint32_t(d[o + 1]) << 16 | uint32_t(d[o + 2]) << 8 | d[o + 3];
        }

        uint16_t be16(const std::vector<uint8_t>& d, size_t o)
        {
            return uint16_t(d[o] << 8 | d[o + 1]);
        }
    }

    namespace
    {
        std::vector<uint8_t> ReadXexFile(const std::string& path)
        {
            std::ifstream in(path, std::ios::binary);
            if (!in)
                Fatal("Cannot open {}", path);
            std::vector<uint8_t> d((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
            if (d.size() < 24 || be32(d, 0) != 0x58455832) // "XEX2"
                Fatal("{} is not an XEX2 file", path);
            return d;
        }

        // Decodes the image of an unencrypted "basic"-compressed XEX (data blocks followed by zero runs) into dst.
        void DecodeImage(const std::string& path, const std::vector<uint8_t>& d, uint32_t fileFormatInfo,
            uint32_t peOffset, uint8_t* dst, uint32_t imageSize)
        {
            const uint32_t ff = fileFormatInfo;
            const uint32_t ffSize = be32(d, ff);
            const uint16_t encryption = be16(d, ff + 4);
            const uint16_t compression = be16(d, ff + 6);
            if (encryption != 0 || compression != 1)
                Fatal("{}: unsupported XEX format (encryption {}, compression {}); expected a decrypted basic-compressed "
                    "image (codebreaker.formats.xex.basic)", path, encryption, compression);
            uint8_t* dstEnd = dst + imageSize;
            size_t src = peOffset;
            for (uint32_t o = ff + 8; o < ff + ffSize; o += 8)
            {
                const uint32_t dataSize = be32(d, o), zeroSize = be32(d, o + 4);
                if (src + dataSize > d.size() || dst + dataSize + zeroSize > dstEnd)
                    Fatal("{}: XEX image blocks exceed the file or the image size", path);
                memcpy(dst, d.data() + src, dataSize);
                memset(dst + dataSize, 0, zeroSize);
                src += dataSize;
                dst += dataSize + zeroSize;
            }
        }
    }

    HostImage ReadImage(const std::string& path)
    {
        const std::vector<uint8_t> d = ReadXexFile(path);
        const uint32_t securityOffset = be32(d, 16);
        const uint32_t headerCount = be32(d, 20);
        uint32_t base = 0, fileFormatInfo = 0, resourceInfo = 0;
        for (uint32_t i = 0; i < headerCount; i++)
        {
            const uint32_t key = be32(d, 24 + i * 8), value = be32(d, 28 + i * 8);
            if (key == kHeaderImageBase)
                base = value;
            else if (key == kHeaderFileFormatInfo)
                fileFormatInfo = value;
            else if (key == kHeaderResourceInfo)
                resourceInfo = value;
        }
        if (base == 0) // resource-only modules (huduiskin.xex) have no image base header: the security info's load address
            base = be32(d, securityOffset + 0x110);
        if (base == 0 || fileFormatInfo == 0)
            Fatal("{}: XEX without an image base or file format header", path);
        HostImage image;
        image.base = base;
        image.bytes.resize(be32(d, securityOffset + 4));
        DecodeImage(path, d, fileFormatInfo, be32(d, 8), image.bytes.data(), uint32_t(image.bytes.size()));
        if (resourceInfo)
        {
            const uint32_t count = (be32(d, resourceInfo) - 4) / 16;
            for (uint32_t i = 0; i < count; i++)
            {
                const size_t entry = resourceInfo + 4 + i * 16;
                std::string name(reinterpret_cast<const char*>(d.data() + entry), 8);
                name.resize(strnlen(name.c_str(), 8));
                image.resources[name] = { be32(d, entry + 8), be32(d, entry + 12) };
            }
        }
        return image;
    }

    Module Load(const std::string& path)
    {
        const std::vector<uint8_t> d = ReadXexFile(path);

        const uint32_t peOffset = be32(d, 8);
        const uint32_t securityOffset = be32(d, 16);
        const uint32_t headerCount = be32(d, 20);
        std::map<uint32_t, uint32_t> headers;
        for (uint32_t i = 0; i < headerCount; i++)
            headers[be32(d, 24 + i * 8)] = be32(d, 28 + i * 8);
        auto require = [&](uint32_t key)
            {
                auto it = headers.find(key);
                if (it == headers.end())
                    Fatal("XEX is missing optional header {:08X}", key);
                return it->second;
            };

        Module m;
        m.header.assign(d.begin(), d.begin() + peOffset);
        // System DLLs (bootanim.xex) have no entry point; they are entered through their exports.
        m.entryPoint = headers.count(kHeaderEntryPoint) ? headers[kHeaderEntryPoint] : 0;
        // Resource-only modules (huduiskin.xex) have no image base header: the security info's load address.
        m.imageBase = headers.count(kHeaderImageBase) ? headers[kHeaderImageBase] : be32(d, securityOffset + 0x110);
        m.imageSize = be32(d, securityOffset + 4);
        m.defaultStackSize = headers.count(kHeaderDefaultStackSize) ? headers[kHeaderDefaultStackSize] : 0x40000;
        if (headers.count(kHeaderExecutionInfo))
            m.titleId = be32(d, headers[kHeaderExecutionInfo] + 12);
        if (headers.count(kHeaderTlsInfo))
        {
            const uint32_t o = headers[kHeaderTlsInfo];
            m.tls = { be32(d, o), be32(d, o + 4), be32(d, o + 8), be32(d, o + 12) };
        }

        auto& mem = guest::g_memory;
        mem.HostCommit(m.imageBase, m.imageSize);
        // Zeroed first: an image can replace an unloaded one at the same address (XAM's app slot, 0x90100000).
        memset(mem.Translate(m.imageBase), 0, m.imageSize);
        DecodeImage(path, d, require(kHeaderFileFormatInfo), peOffset, mem.Translate(m.imageBase), m.imageSize);

        // Imports: each record is a guest address holding (type << 24 | ordinal); type != 0 marks a function thunk.
        // The header is {total size, string table size, name count, names (4-byte aligned)}, then library blocks until
        // the total size is used up. A library can have several blocks (dash.xex: xam.xex, xam.xex, xboxkrnl.exe), so
        // blocks are walked by size and each is named by its own name index (u16 at +0x24). Records are merged by
        // (library, ordinal) across blocks: in dash.xex a function's slot can be in one xam block and its thunk in the
        // other (XamNuiGetSupportString, ordinal 2274).
        if (headers.count(kHeaderImportLibraries))
        {
            const uint32_t o = headers[kHeaderImportLibraries];
            const uint32_t totalSize = be32(d, o);
            const uint32_t stringTableSize = be32(d, o + 4);
            const uint32_t libraryCount = be32(d, o + 8);
            std::vector<std::string> names;
            for (size_t p = o + 12; names.size() < libraryCount;)
            {
                std::string name(reinterpret_cast<const char*>(d.data() + p));
                p += (name.size() + 1 + 3) & ~size_t(3);
                names.push_back(name);
            }
            std::map<std::pair<std::string, uint16_t>, Import> byOrdinal;
            for (size_t lib = o + 12 + stringTableSize; lib < o + totalSize;)
            {
                const uint32_t librarySize = be32(d, lib);
                // The name index is the low byte: early devkit dashboards (2.0.2255, 2258, 2832) have other bits in
                // the high one (0x0900, 0xC301, 0x3102 for libraries 0, 1, 2).
                const uint16_t nameIndex = be16(d, lib + 0x24) & 0xFF;
                const uint16_t recordCount = be16(d, lib + 0x26);
                if (librarySize == 0 || nameIndex >= names.size())
                    Fatal("{}: bad import library block at header offset {:X}", path, lib);
                const std::string& name = names[nameIndex];
                for (uint16_t r = 0; r < recordCount; r++)
                {
                    const uint32_t record = be32(d, lib + 0x28 + r * 4);
                    const uint32_t value = mem.Read32(record);
                    Import& imp = byOrdinal[{ name, uint16_t(value & 0xFFFF) }];
                    imp.library = name;
                    imp.ordinal = uint16_t(value & 0xFFFF);
                    uint32_t& field = value >> 24 ? imp.thunk : imp.slot;
                    if (field != 0)
                        Fatal("{}: {} ordinal {} is imported twice", path, name, imp.ordinal);
                    field = record;
                }
                lib += librarySize;
            }
            for (auto& [key, imp] : byOrdinal)
                m.imports.push_back(imp);
        }

        // Exports (security info +0x160 -> export table in the image): {.., +0x20 high half of the addresses, +0x24
        // count, +0x28 ordinal base, +0x2C offsets}; an export's address is (high << 16) + offset, 0 = no export.
        // The PE section table's .edata is stale in system modules; this table is the one the loader uses.
        if (const uint32_t table = be32(d, securityOffset + 0x160))
        {
            const uint32_t high = mem.Read32(table + 0x20);
            const uint32_t count = mem.Read32(table + 0x24);
            const uint32_t ordinalBase = mem.Read32(table + 0x28);
            for (uint32_t i = 0; i < count; i++)
                if (const uint32_t offset = mem.Read32(table + 0x2C + i * 4))
                    m.exports[uint16_t(ordinalBase + i)] = (high << 16) + offset;
        }

        Log("Loaded {}: base {:08X} size {:X} entry {:08X} title {:08X} stack {:X} tls slots {} imports {} exports {}",
            path, m.imageBase, m.imageSize, m.entryPoint, m.titleId, m.defaultStackSize, m.tls.slotCount, m.imports.size(),
            m.exports.size());
        return m;
    }
}
