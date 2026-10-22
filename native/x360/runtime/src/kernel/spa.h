#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <vector>

// The title's SPA: the XEX resource named after the title ID, an XDBF database with achievements, strings and images.
//
// XDBF header (24 bytes): magic "XDBF", version, entry count, entries used, free-list count, free entries used; then
// entries {u16 section, u64 id, u32 offset, u32 size} (18 bytes, packed) and free entries {u32 offset, u32 size};
// data offsets are relative to the end of those tables. Section 1 (metadata) id 'XACH' holds achievements:
// {magic, version, size, u16 count} then 0x24-byte entries {u16 id, label string, description string, unachieved
// string, u32 image id, u16 gamerscore, u16, u32 flags, 16 bytes}. Section 2 holds images (PNG) by id (0x8000 = the
// title image). Section 3 holds one string table per language (id = XC_LANGUAGE): {'XSTR', version 1, size, u16
// count} then {u16 id, u16 length, UTF-8 bytes}; metadata 'XSTC' {magic, version, size, u32 default language} names
// the language used when the console's has no table. Layouts cross-checked against Xenia's netplay fork
// (xdbf/spa_info.h, xdbf_io.h).
namespace kernel::spa
{
    struct Achievement
    {
        uint16_t id = 0;
        uint16_t labelId = 0, descriptionId = 0, unachievedId = 0;
        uint32_t imageId = 0;
        uint16_t gamerscore = 0;
        uint32_t flags = 0;
    };

    constexpr uint64_t kTitleImageId = 0x8000;

    const std::vector<Achievement>& Achievements();
    // A metadata entry (section 1) by id ('XSRC' = 0x58535243, the XLAST); empty if absent.
    std::span<const uint8_t> Metadata(uint64_t id);
    // An image (section 2) by id; empty if absent.
    std::span<const uint8_t> Image(uint64_t id);
    // A string in the console's language (the SPA's default language if it has no table for it); empty if absent.
    std::u16string String(uint16_t id);
    // The title's name (string 0x8000).
    std::u16string TitleName();
}
