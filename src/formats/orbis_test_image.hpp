// formats/orbis_test_image.hpp -- a small x86-64 ELF of the kind a PlayStation payload SDK links, and the fake SELF OpenOrbis's create-fself wraps one in, for the
// tests of the PlayStation 4 / 5 reader, writer and lane. Built whole here, byte by byte, so a test can edit it.
//
// The program (virtual addresses are file offsets):
//   0x1000 _start:  push rbx; mov ebx, edi; call helper; add eax, ebx; pop rbx; ret
//   0x1020 helper:  sub rsp, 8; mov edi, 100; call [rip + got]   (the import sceKernelUsleep); mov eax, [rip + value]; add rsp, 8; ret
//   0x1040 loop:    xor eax, eax; test edi, edi; je end; L: add eax, edi; dec edi; jne L; end: ret   (a function only a relocation's pointer names)
//   0x2000 got (8 bytes, filled by the loader), 0x2010 value (7), 0x2020 a pointer to the loop function (a RELATIVE relocation)
// The dynamic section needs libkernel.sprx, names sceKernelUsleep and has two relocations.
#pragma once

#include <algorithm>
#include <cstdint>
#include <initializer_list>
#include <string>
#include <vector>

#include "core/bytes.hpp"

namespace cb::formats::orbis_test {

inline void put16(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 2; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (8 * i));
}
inline void put32(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (8 * i));
}
inline void put64(Bytes& b, std::size_t at, std::uint64_t v) {
    for (int i = 0; i < 8; ++i) b[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (8 * i));
}
inline void put_bytes(Bytes& b, std::size_t at, std::initializer_list<std::uint8_t> bytes) {
    std::size_t k = 0;
    for (const std::uint8_t v : bytes) b[at + k++] = v;
}

inline constexpr std::size_t kElfSize = 0x2140;
inline constexpr std::uint64_t kStart = 0x1000, kHelper = 0x1020, kLoop = 0x1040, kSyscall = 0x1060, kGot = 0x2000, kValue = 0x2010;

// The ELF. `needed` is the library the dynamic section names (a PlayStation program names libkernel.sprx; a Linux one names libc.so.6).
inline Bytes elf(const char* needed = "libkernel.sprx") {
    Bytes b(kElfSize, 0);
    // header
    put_bytes(b, 0, {0x7F, 'E', 'L', 'F', 2, 1, 1, 0});
    put16(b, 16, 3);       // ET_DYN
    put16(b, 18, 62);      // x86-64
    put32(b, 20, 1);
    put64(b, 24, kStart);  // entry
    put64(b, 32, 0x40);    // program headers
    put16(b, 52, 64);
    put16(b, 54, 56);
    put16(b, 56, 3);
    // program headers: code, data, dynamic
    const auto phdr = [&](std::size_t k, std::uint32_t type, std::uint32_t flags, std::uint64_t offset, std::uint64_t filesz, std::uint64_t memsz) {
        const std::size_t o = 0x40 + k * 56;
        put32(b, o, type);
        put32(b, o + 4, flags);
        put64(b, o + 8, offset);
        put64(b, o + 16, offset);
        put64(b, o + 24, offset);
        put64(b, o + 32, filesz);
        put64(b, o + 40, memsz);
        put64(b, o + 48, 0x1000);
    };
    phdr(0, 1, 5, 0x1000, 0x80, 0x80);
    phdr(1, 1, 6, 0x2000, 0x140, 0x200);
    phdr(2, 2, 6, 0x2040, 0x70, 0x70);
    // _start
    put_bytes(b, 0x1000, {0x53, 0x89, 0xFB, 0xE8, 0x18, 0x00, 0x00, 0x00, 0x01, 0xD8, 0x5B, 0xC3});
    // helper
    put_bytes(b, 0x1020, {0x48, 0x83, 0xEC, 0x08, 0xBF, 0x64, 0x00, 0x00, 0x00, 0xFF, 0x15, 0xD1, 0x0F, 0x00, 0x00, 0x8B, 0x05, 0xDB, 0x0F, 0x00, 0x00, 0x48, 0x83, 0xC4, 0x08, 0xC3});
    // loop: eax = 0; while (edi) eax += edi--
    put_bytes(b, 0x1040, {0x31, 0xC0, 0x85, 0xFF, 0x74, 0x06, 0x01, 0xF8, 0xFF, 0xCF, 0x75, 0xFA, 0xC3});
    // writer: mov eax, 4 (write); syscall; ret -- reached by no flow, run by address
    put_bytes(b, 0x1060, {0xB8, 0x04, 0x00, 0x00, 0x00, 0x0F, 0x05, 0xC3});
    // data
    put32(b, 0x2010, 7);
    // dynamic section
    const auto tag = [&](std::size_t k, std::uint64_t t, std::uint64_t v) {
        put64(b, 0x2040 + k * 16, t);
        put64(b, 0x2040 + k * 16 + 8, v);
    };
    tag(0, 1, 1);          // NEEDED: "libkernel.sprx" at strtab+1
    tag(1, 5, 0x20F0);     // STRTAB
    tag(2, 6, 0x20C0);     // SYMTAB (two symbols, up to the string table)
    tag(3, 10, 0x20);      // STRSZ
    tag(4, 7, 0x2110);     // RELA
    tag(5, 8, 48);         // RELASZ
    tag(6, 0, 0);
    // symbols: the null one, then the import (undefined function, global)
    put32(b, 0x20C0 + 24, 17);
    b[0x20C0 + 24 + 4] = 0x12;
    // strings: "\0<needed>\0sceKernelUsleep\0": the library at 1, the symbol after it (the test library name is the 14 characters of libkernel.sprx, or shorter)
    const std::string lib = needed;
    const std::string strings = std::string(1, '\0') + lib + std::string(1, '\0') + "sceKernelUsleep" + std::string(1, '\0');
    for (std::size_t k = 0; k < strings.size() && k < 0x20; ++k) b[0x20F0 + k] = static_cast<std::uint8_t>(strings[k]);
    put32(b, 0x20C0 + 24, static_cast<std::uint32_t>(2 + lib.size()));   // the symbol's name offset
    // relocations: GLOB_DAT of the import into the GOT slot, RELATIVE for the pointer to the loop
    put64(b, 0x2110, kGot);
    put64(b, 0x2110 + 8, (1ull << 32) | 6);
    put64(b, 0x2110 + 24, 0x2020);
    put64(b, 0x2110 + 24 + 8, 8);
    put64(b, 0x2110 + 24 + 16, kLoop);
    return b;
}

// The fake SELF create-fself makes of `elf` (its layout, OpenOrbis/create-fself pkg/fself): the header, two entries per loaded segment (a meta entry and the data),
// the ELF header and program headers, the extended info (`program_type` 1 is "fake"), the NPDRM block, the meta blocks, the footer and signature, then the segments.
inline Bytes fself(ByteView elf, std::uint64_t program_type = 1, std::uint64_t data_props_extra = 0) {
    const std::size_t phnum = elf[56];
    struct Load {
        std::size_t index;
        std::uint64_t offset, size;
    };
    std::vector<Load> loads;
    for (std::size_t k = 0; k < phnum; ++k) {
        const std::size_t o = 0x40 + k * 56;
        const std::uint32_t type = elf[o] | elf[o + 1] << 8 | elf[o + 2] << 16 | static_cast<std::uint32_t>(elf[o + 3]) << 24;
        if (type != 1) continue;
        std::uint64_t offset = 0, size = 0;
        for (int i = 7; i >= 0; --i) {
            offset = offset << 8 | elf[o + 8 + static_cast<std::size_t>(i)];
            size = size << 8 | elf[o + 32 + static_cast<std::size_t>(i)];
        }
        loads.push_back({k, offset, size});
    }
    const std::size_t entries = loads.size() * 2;
    const std::size_t elf_at = 0x20 + entries * 0x20;
    const std::size_t ext_at = (elf_at + 0x40 + phnum * 56 + 15) & ~std::size_t{15};
    std::size_t cursor = ext_at + 0x40 + 0x30 + entries * 0x20 + 0x50 + 0x100;   // extended info, NPDRM block, meta blocks, footer, signature
    cursor = (cursor + 15) & ~std::size_t{15};
    Bytes out(cursor, 0);
    put_bytes(out, 0, {0x4F, 0x15, 0x3D, 0x1D, 0, 1, 1, 0x12});
    put32(out, 8, 0x101);
    put16(out, 12, static_cast<std::uint32_t>(ext_at + 0x70));
    put16(out, 14, static_cast<std::uint32_t>(entries * 0x20 + 0x150));
    put16(out, 0x18, static_cast<std::uint32_t>(entries));
    put16(out, 0x1A, 0x22);
    std::copy_n(elf.begin(), 0x40 + phnum * 56, out.begin() + static_cast<std::ptrdiff_t>(elf_at));
    put64(out, ext_at, 0x3800000000000011ull);
    put64(out, ext_at + 8, program_type);
    std::size_t e = 0;
    for (const Load& l : loads) {
        const std::size_t meta_entry = 0x20 + e * 0x20, data_entry = meta_entry + 0x20;
        put64(out, meta_entry, (1ull << 2) | (1ull << 16) | static_cast<std::uint64_t>(e + 1) << 20);
        put64(out, data_entry, (1ull << 2) | (1ull << 11) | (2ull << 12) | static_cast<std::uint64_t>(l.index) << 20 | data_props_extra);
        put64(out, data_entry + 8, cursor);
        put64(out, data_entry + 16, l.size);
        put64(out, data_entry + 24, l.size);
        out.insert(out.end(), elf.begin() + static_cast<std::ptrdiff_t>(l.offset), elf.begin() + static_cast<std::ptrdiff_t>(l.offset + l.size));
        cursor = (out.size() + 15) & ~std::size_t{15};
        out.resize(cursor, 0);
        e += 2;
    }
    put64(out, 16, out.size());
    return out;
}

// What `fself` gives back as an ELF: the header and program headers, each loaded segment at its offset, nothing else.
inline Bytes elf_of_self(ByteView elf) {
    const std::size_t phnum = elf[56];
    Bytes out(0x40 + phnum * 56, 0);
    std::copy_n(elf.begin(), out.size(), out.begin());
    for (std::size_t k = 0; k < phnum; ++k) {
        const std::size_t o = 0x40 + k * 56;
        const std::uint32_t type = elf[o];
        if (type != 1) continue;
        std::uint64_t offset = 0, size = 0;
        for (int i = 7; i >= 0; --i) {
            offset = offset << 8 | elf[o + 8 + static_cast<std::size_t>(i)];
            size = size << 8 | elf[o + 32 + static_cast<std::size_t>(i)];
        }
        if (out.size() < offset + size) out.resize(offset + size, 0);
        std::copy_n(elf.begin() + static_cast<std::ptrdiff_t>(offset), size, out.begin() + static_cast<std::ptrdiff_t>(offset));
    }
    return out;
}

}  // namespace cb::formats::orbis_test
