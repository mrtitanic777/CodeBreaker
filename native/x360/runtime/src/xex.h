#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

// Minimal XEX2 loader for decrypted, "basic"-compressed modules (titles; system modules converted by
// codebreaker.formats.xex.basic).
namespace xex
{
    struct Import
    {
        std::string library; // "xboxkrnl.exe" or "xam.xex"
        uint16_t ordinal = 0;
        uint32_t slot = 0;   // 4-byte import slot (.rdata) the loader fills
        uint32_t thunk = 0;  // 16-byte code thunk (.text) for function imports; 0 for variables
    };

    struct TlsInfo
    {
        uint32_t slotCount = 0;
        uint32_t rawDataAddress = 0;
        uint32_t dataSize = 0;
        uint32_t rawDataSize = 0;
    };

    struct Module
    {
        std::vector<uint8_t> header; // the XEX header, up to the PE data
        uint32_t imageBase = 0;
        uint32_t imageSize = 0;
        uint32_t entryPoint = 0;
        uint32_t defaultStackSize = 0;
        uint32_t titleId = 0;
        TlsInfo tls;
        std::vector<Import> imports;
        std::map<uint16_t, uint32_t> exports; // ordinal -> guest address
    };

    // Reads the XEX and copies its image into guest memory at its base address. Exits on any format it cannot load.
    Module Load(const std::string& path);

    // A module's image decoded into host memory, for reading data out of a module the runtime does not load (the
    // runtime replaces xam.xex, but some XAM data, like its online schema, is handed to titles as is).
    struct HostImage
    {
        uint32_t base = 0;
        std::vector<uint8_t> bytes;
        // XEX resources (header 0x2FF: name[8], address, size), the sections of XexGetModuleSection.
        std::map<std::string, std::pair<uint32_t, uint32_t>> resources;
        const uint8_t* At(uint32_t address, uint32_t size) const
        {
            return address >= base && size <= bytes.size() && address - base <= bytes.size() - size
                ? bytes.data() + (address - base) : nullptr;
        }
    };
    HostImage ReadImage(const std::string& path);
}
