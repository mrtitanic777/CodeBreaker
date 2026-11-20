// source/gekko_test_images.hpp -- small GameCube, Wii and Wii U programs the tests build themselves (nothing here is a real program).
//
// The program is three functions and some data, assembled by decompile/ppc from the text below at the addresses it is loaded at:
//
//     entry (0x80003100)   builds the address of a string (`lis`/`addi`), calls `helper`, counts to five with a conditional branch back,
//                          reads the video interface's first register (`lis r5,0xCC00` / `lwz r6,0x2000(r5)`) and returns
//     helper (0x80003150)  a frame (`stwu`, `mflr`, `stw`, ... `blr`) that adds the string's first byte to r3
//     callback (0x80003190) `li r3,7; blr`: no prologue, named only by a pointer in the data
//     data0 (0x80004000)   "Hello, Cube!", then a table holding the addresses of callback and helper
//
// `dol()` is those in a DOL, `elf()` in a GameCube / Wii ELF, `rpx()` in a Wii U RPX whose code section is stored compressed (as a stored zlib
// block: the tests need an inflater's input, not a compressor's output). `wii_dol()` is the DOL with a load of the Hollywood's register block
// (`lis r7,0xCD80`) in its code, which is what makes it a Wii program.
#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "decompile/ppc.hpp"

namespace cb::source::gekko_test {

inline constexpr std::uint32_t kText = 0x80003100;
inline constexpr std::uint32_t kHelper = 0x80003150;
inline constexpr std::uint32_t kCallback = 0x80003190;
inline constexpr std::uint32_t kData = 0x80004000;

inline void put32(Bytes& b, std::uint32_t v) {
    b.push_back(static_cast<std::uint8_t>(v >> 24));
    b.push_back(static_cast<std::uint8_t>(v >> 16));
    b.push_back(static_cast<std::uint8_t>(v >> 8));
    b.push_back(static_cast<std::uint8_t>(v));
}
inline void put16(Bytes& b, std::uint32_t v) {
    b.push_back(static_cast<std::uint8_t>(v >> 8));
    b.push_back(static_cast<std::uint8_t>(v));
}
inline void set32(Bytes& b, std::size_t at, std::uint32_t v) {
    b[at] = static_cast<std::uint8_t>(v >> 24);
    b[at + 1] = static_cast<std::uint8_t>(v >> 16);
    b[at + 2] = static_cast<std::uint8_t>(v >> 8);
    b[at + 3] = static_cast<std::uint8_t>(v);
}

// The code, assembled at kText (with an extra line when `wii`).
inline Bytes code(bool wii = false) {
    const decompile::NameLabels labels = {{"entry", kText}, {"helper", kHelper}, {"callback", kCallback}, {"msg", kData}};
    std::vector<std::string> lines = {"stwu r1,-16(r1)", "mflr r0", "stw r0,20(r1)", "lis r3,msg@ha", "addi r3,r3,msg@l", "bl helper", "li r4,5", "cmpwi r4,5", "bne entry", "lis r5,0xCC00", "lwz r6,0x2000(r5)"};
    if (wii) lines.push_back("lis r7,0xCD80");
    for (const char* l : {"lwz r0,20(r1)", "addi r1,r1,16", "mtlr r0", "blr"}) lines.push_back(l);
    Bytes out;
    for (const std::string& line : lines) {
        const Bytes b = decompile::ppc::assemble(line, kText + static_cast<std::uint32_t>(out.size()), labels);
        out.insert(out.end(), b.begin(), b.end());
    }
    while (kText + out.size() < kHelper) {   // padding between functions: nops
        put32(out, 0x60000000);
    }
    for (const std::string& line : {"stwu r1,-16(r1)", "mflr r0", "stw r0,20(r1)", "lbz r4,0(r3)", "add r3,r3,r4", "lwz r0,20(r1)", "mtlr r0", "addi r1,r1,16", "blr"}) {
        const Bytes b = decompile::ppc::assemble(line, kText + static_cast<std::uint32_t>(out.size()), labels);
        out.insert(out.end(), b.begin(), b.end());
    }
    while (kText + out.size() < kCallback) put32(out, 0x60000000);
    for (const std::string& line : {"li r3,7", "blr"}) {
        const Bytes b = decompile::ppc::assemble(line, kText + static_cast<std::uint32_t>(out.size()), labels);
        out.insert(out.end(), b.begin(), b.end());
    }
    return out;
}

inline Bytes data() {
    Bytes out;
    for (const char c : std::string_view("Hello, Cube!")) out.push_back(static_cast<std::uint8_t>(c));
    while (out.size() < 16) out.push_back(0);
    put32(out, kCallback);
    put32(out, kHelper);
    for (int i = 0; i < 4; ++i) put32(out, 0);
    return out;
}

// A DOL: text0 at 0x100, data0 after it (both padded to 32 bytes), the bss area past that, entry at kText.
inline Bytes dol(bool wii = false) {
    Bytes text = code(wii), data_bytes = data();
    while (text.size() % 32) text.push_back(0);
    while (data_bytes.size() % 32) data_bytes.push_back(0);
    Bytes out(0x100, 0);
    const std::uint32_t text_at = 0x100, data_at = 0x100 + static_cast<std::uint32_t>(text.size());
    set32(out, 0x00, text_at);                    // text0
    set32(out, 0x1C, data_at);                    // data0 (slot 7)
    set32(out, 0x48, kText);
    set32(out, 0x48 + 0x1C, kData);
    set32(out, 0x90, static_cast<std::uint32_t>(text.size()));
    set32(out, 0x90 + 0x1C, static_cast<std::uint32_t>(data_bytes.size()));
    set32(out, 0xD8, 0x80005000);                 // bss
    set32(out, 0xDC, 0x200);
    set32(out, 0xE0, kText);
    out.insert(out.end(), text.begin(), text.end());
    out.insert(out.end(), data_bytes.begin(), data_bytes.end());
    return out;
}

// A zlib stream holding `data` as one stored (uncompressed) block.
inline Bytes zlib_stored(ByteView data) {
    Bytes out = {0x78, 0x01, 0x01, static_cast<std::uint8_t>(data.size()), static_cast<std::uint8_t>(data.size() >> 8), static_cast<std::uint8_t>(~data.size()),
                 static_cast<std::uint8_t>(~(data.size() >> 8))};
    out.insert(out.end(), data.begin(), data.end());
    std::uint32_t a = 1, b = 0;
    for (const std::uint8_t byte : data) {
        a = (a + byte) % 65521;
        b = (b + a) % 65521;
    }
    put32(out, b << 16 | a);
    return out;
}

// An ELF: header, two program headers, .text, .data, .shstrtab, the section header table (null, .text, .data, .shstrtab). With `rpx` it is
// the Wii U's flavour (OS/ABI 0xCA, type 0xFE01) with .text stored compressed.
inline Bytes elf(bool rpx = false, bool wii = false) {
    const Bytes text = code(wii), data_bytes = data();
    Bytes text_stored = text;
    if (rpx) {
        text_stored.clear();
        put32(text_stored, static_cast<std::uint32_t>(text.size()));
        const Bytes z = zlib_stored(text);
        text_stored.insert(text_stored.end(), z.begin(), z.end());
    }
    const std::string names = std::string("\0.text\0.data\0.shstrtab\0", 23);
    const std::uint32_t text_at = 0x80, data_at = (text_at + static_cast<std::uint32_t>(text_stored.size()) + 3) & ~3u;
    const std::uint32_t strings_at = data_at + static_cast<std::uint32_t>(data_bytes.size());
    const std::uint32_t sections_at = (strings_at + static_cast<std::uint32_t>(names.size()) + 3) & ~3u;
    Bytes out = {0x7F, 'E', 'L', 'F', 1, 2, 1, static_cast<std::uint8_t>(rpx ? 0xCA : 0), static_cast<std::uint8_t>(rpx ? 0xFE : 0), 0, 0, 0, 0, 0, 0, 0};
    put16(out, rpx ? 0xFE01 : 2);
    put16(out, 20);
    put32(out, 1);
    put32(out, kText);
    put32(out, rpx ? 0 : 52);    // program headers (an RPX has none)
    put32(out, sections_at);
    put32(out, 0);
    put16(out, 52);
    put16(out, 32);
    put16(out, rpx ? 0 : 2);
    put16(out, 40);
    put16(out, 4);
    put16(out, 3);
    // program headers: type, offset, vaddr, paddr, filesz, memsz, flags, align
    if (!rpx) {
        for (const std::uint32_t w : {1u, text_at, kText, kText, static_cast<std::uint32_t>(text.size()), static_cast<std::uint32_t>(text.size()), 5u, 4u}) put32(out, w);
        for (const std::uint32_t w : {1u, data_at, kData, kData, static_cast<std::uint32_t>(data_bytes.size()), static_cast<std::uint32_t>(data_bytes.size()), 6u, 4u}) put32(out, w);
    }
    out.resize(text_at, 0);
    out.insert(out.end(), text_stored.begin(), text_stored.end());
    out.resize(data_at, 0);
    out.insert(out.end(), data_bytes.begin(), data_bytes.end());
    out.insert(out.end(), names.begin(), names.end());
    out.resize(sections_at, 0);
    out.resize(sections_at + 40, 0);   // the null section
    // name, type, flags, addr, offset, size, link, info, align, entsize
    const std::uint32_t text_flags = 2 | 4 | (rpx ? 0x08000000u : 0u);
    for (const std::uint32_t w : {1u, 1u, text_flags, kText, text_at, static_cast<std::uint32_t>(text_stored.size()), 0u, 0u, 4u, 0u}) put32(out, w);
    for (const std::uint32_t w : {7u, 1u, 3u, kData, data_at, static_cast<std::uint32_t>(data_bytes.size()), 0u, 0u, 4u, 0u}) put32(out, w);
    for (const std::uint32_t w : {13u, 3u, 0u, 0u, strings_at, static_cast<std::uint32_t>(names.size()), 0u, 0u, 1u, 0u}) put32(out, w);
    return out;
}

}  // namespace cb::source::gekko_test
