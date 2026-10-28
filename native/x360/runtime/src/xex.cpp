#include "xex.h"

#include "log.h"
#include "memory.h"

// CodeBreaker's XEX reader (native/xex/reader.cmake): the headers, security info, imports and the decoded image
#include "basefile.h"
#include "xex_file.h"

#include <cstring>
#include <exception>
#include <fstream>
#include <iterator>
#include <map>

namespace xex
{
    namespace
    {
        XexFile ReadXexFile(const std::string& path)
        {
            std::ifstream in(path, std::ios::binary);
            if (!in)
                Fatal("Cannot open {}", path);
            std::vector<uint8_t> d((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
            try
            {
                return XexFile::parse(std::move(d));
            }
            catch (const std::exception& e)
            {
                Fatal("{} is not a readable XEX2 file: {}", path, e.what());
            }
        }

        // The image of a module (security info image size bytes): decrypted and decompressed by the reader. Patches
        // (delta compression) are not images.
        std::vector<uint8_t> DecodeImage(const std::string& path, const XexFile& x)
        {
            const uint16_t compression = x.compression_type().value_or(0);
            if (!x.find_header(kHdrFileFormatInfo) || (compression != kCompBasic && compression != kCompNormal))
                Fatal("{}: unsupported XEX format (encryption {}, compression {})", path,
                    x.encryption_type().value_or(0), compression);
            std::vector<uint8_t> image;
            try
            {
                image = reconstruct_basefile(x);
            }
            catch (const std::exception& e)
            {
                Fatal("{}: cannot decode the XEX image: {}", path, e.what());
            }
            if (image.size() != x.security().image_size)
                Fatal("{}: XEX image blocks exceed the image size", path);
            return image;
        }

        // Resource-only modules (huduiskin.xex) have no image base header: the security info's load address.
        uint32_t ImageBase(const XexFile& x)
        {
            return x.image_base().value_or(x.security().load_address);
        }
    }

    HostImage ReadImage(const std::string& path)
    {
        const XexFile x = ReadXexFile(path);
        HostImage image;
        image.base = ImageBase(x);
        if (image.base == 0)
            Fatal("{}: XEX without an image base", path);
        image.bytes = DecodeImage(path, x);
        for (const Resource& r : x.resources())
            image.resources[r.name] = { r.address, r.size };
        return image;
    }

    Module Load(const std::string& path)
    {
        const XexFile x = ReadXexFile(path);
        auto header = [&](uint32_t key, uint32_t absent)
            {
                const OptionalHeader* h = x.find_header(key);
                return h ? h->value : absent;
            };

        Module m;
        m.header.assign(x.raw().begin(), x.raw().begin() + x.data_offset());
        // System DLLs (bootanim.xex) have no entry point; they are entered through their exports.
        m.entryPoint = header(kHdrEntryPoint, 0);
        m.imageBase = ImageBase(x);
        m.imageSize = x.security().image_size;
        m.defaultStackSize = header(kHdrDefaultStackSize, 0x40000);
        if (const auto id = x.execution_id())
            m.titleId = id->title_id;
        if (const auto tls = x.tls_info())
            m.tls = { tls->slot_count, tls->raw_data_address, tls->data_size, tls->raw_data_size };

        // The whole image is written: it can replace an unloaded one at the same address (XAM's app slot, 0x90100000).
        const std::vector<uint8_t> image = DecodeImage(path, x);
        auto& mem = guest::g_memory;
        mem.HostCommit(m.imageBase, m.imageSize);
        memcpy(mem.Translate(m.imageBase), image.data(), m.imageSize);

        // Imports: each record is a guest address holding (type << 24 | ordinal); type != 0 marks a function thunk.
        // A library can have several import blocks (dash.xex: xam.xex, xam.xex, xboxkrnl.exe), so records are merged by
        // (library, ordinal) across blocks: in dash.xex a function's slot can be in one xam block and its thunk in the
        // other (XamNuiGetSupportString, ordinal 2274).
        std::map<std::pair<std::string, uint16_t>, Import> byOrdinal;
        for (const ImportLibrary& lib : x.import_libraries())
        {
            if (lib.name.empty())
                Fatal("{}: an import library block names no library", path);
            for (const uint32_t record : lib.records)
            {
                const uint32_t value = mem.Read32(record);
                Import& imp = byOrdinal[{ lib.name, uint16_t(value & 0xFFFF) }];
                imp.library = lib.name;
                imp.ordinal = uint16_t(value & 0xFFFF);
                uint32_t& field = value >> 24 ? imp.thunk : imp.slot;
                if (field != 0)
                    Fatal("{}: {} ordinal {} is imported twice", path, lib.name, imp.ordinal);
                field = record;
            }
        }
        for (auto& [key, imp] : byOrdinal)
            m.imports.push_back(imp);

        // Exports (security info +0x160 -> export table in the image): {.., +0x20 high half of the addresses, +0x24
        // count, +0x28 ordinal base, +0x2C offsets}; an export's address is (high << 16) + offset, 0 = no export.
        // The PE section table's .edata is stale in system modules; this table is the one the loader uses.
        if (const uint32_t table = x.security().export_table)
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
