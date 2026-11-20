// formats/xboxone_test_image.hpp -- a small x64 PE32+ built for the app sandbox (what a UWP app's executable is), and the packages (.msix, .msixbundle) that hold one, for the tests of
// the Xbox One / Series reader, writer and lane. Built whole here, byte by byte, so a test can edit it.
//
// The program (virtual addresses are RVAs from the image base 0x140000000):
//   0x1000 entry:   sub rsp, 0x28; mov ecx, 100; call [rip + IAT]  (KERNEL32.dll!Sleep); mov eax, [rip + value]; add rsp, 0x28; ret
//   0x1020 add:     lea eax, [rcx + rdx]; ret                       (begun by the exception directory only: nothing calls it)
//   0x2000 the import directory, 0x2050 the import address table, 0x2100 value (7), 0x2200 the exception directory (one function)
#pragma once

#include <cstdint>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

#include "core/bytes.hpp"

namespace cb::formats::xboxone_test {

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
inline void put_text(Bytes& b, std::size_t at, const std::string& s) {
    for (std::size_t k = 0; k < s.size(); ++k) b[at + k] = static_cast<std::uint8_t>(s[k]);
}

inline constexpr std::uint64_t kImageBase = 0x140000000ull;
inline constexpr std::uint32_t kEntry = 0x1000, kAdd = 0x1020, kIat = 0x2050, kValue = 0x2100;
inline constexpr std::size_t kPeSize = 0x800;

// The PE32+. `app_container` sets the DLL characteristic a UWP app has; a plain Win32 program has it clear.
inline Bytes pe(bool app_container = true) {
    Bytes b(kPeSize, 0);
    b[0] = 'M';
    b[1] = 'Z';
    put32(b, 0x3C, 0x80);
    put32(b, 0x80, 0x00004550);                // "PE\0\0"
    put16(b, 0x84, 0x8664);                    // machine
    put16(b, 0x86, 2);                         // sections
    put16(b, 0x94, 0xF0);                      // optional header size
    put16(b, 0x96, 0x0022);                    // executable, large address aware
    const std::size_t o = 0x98;                // the optional header
    put16(b, o, 0x20B);
    put32(b, o + 4, 0x200);                    // size of code
    put32(b, o + 8, 0x400);                    // initialised data
    put32(b, o + 16, kEntry);                  // entry point
    put32(b, o + 20, 0x1000);                  // base of code
    put64(b, o + 24, kImageBase);
    put32(b, o + 32, 0x1000);                  // section alignment
    put32(b, o + 36, 0x200);                   // file alignment
    put16(b, o + 40, 6);                       // OS version
    put16(b, o + 48, 6);                       // subsystem version
    put32(b, o + 56, 0x4000);                  // size of image
    put32(b, o + 60, 0x200);                   // size of headers
    put16(b, o + 68, 2);                       // subsystem: GUI
    put16(b, o + 70, static_cast<std::uint32_t>((app_container ? 0x1000 : 0) | 0x8160));   // AppContainer, high entropy ASLR, dynamic base, NX, terminal server aware
    put64(b, o + 72, 0x100000);                // stack reserve
    put64(b, o + 80, 0x1000);
    put64(b, o + 88, 0x100000);                // heap
    put64(b, o + 96, 0x1000);
    put32(b, o + 108, 16);                     // data directories
    put32(b, o + 112 + 1 * 8, 0x2000);         // the import directory
    put32(b, o + 112 + 1 * 8 + 4, 40);
    put32(b, o + 112 + 3 * 8, 0x2200);         // the exception directory
    put32(b, o + 112 + 3 * 8 + 4, 12);
    // section table, after the optional header (0x98 + 0xF0 = 0x188)
    const auto section = [&](std::size_t k, const char* name, std::uint32_t virtual_size, std::uint32_t rva, std::uint32_t raw_size, std::uint32_t raw_ptr, std::uint32_t flags) {
        const std::size_t s = 0x188 + k * 40;
        put_text(b, s, name);
        put32(b, s + 8, virtual_size);
        put32(b, s + 12, rva);
        put32(b, s + 16, raw_size);
        put32(b, s + 20, raw_ptr);
        put32(b, s + 36, flags);
    };
    section(0, ".text", 0x40, 0x1000, 0x200, 0x200, 0x60000020);
    section(1, ".rdata", 0x300, 0x2000, 0x400, 0x400, 0x40000040);
    // .text at file 0x200
    put_bytes(b, 0x200, {0x48, 0x83, 0xEC, 0x28, 0xB9, 0x64, 0x00, 0x00, 0x00, 0xFF, 0x15, 0x41, 0x10, 0x00, 0x00, 0x8B, 0x05, 0xEB, 0x10, 0x00, 0x00, 0x48, 0x83, 0xC4, 0x28, 0xC3});
    put_bytes(b, 0x220, {0x8D, 0x04, 0x11, 0xC3});
    // .rdata at file 0x400 (RVA 0x2000): the import descriptor for KERNEL32.dll, its terminator, the lookup table, the address table, the names
    put32(b, 0x400, 0x2040);                   // OriginalFirstThunk
    put32(b, 0x400 + 12, 0x2060);              // Name
    put32(b, 0x400 + 16, kIat);                // FirstThunk
    put64(b, 0x440, 0x2070);                   // lookup table: Sleep, by name
    put64(b, 0x450, 0x2070);                   // address table
    put_text(b, 0x460, "KERNEL32.dll");
    put_text(b, 0x472, "Sleep");               // after the 2-byte hint
    put32(b, 0x500, 7);                        // value
    put32(b, 0x600, kAdd);                     // the exception directory: begin, end, unwind info
    put32(b, 0x604, kAdd + 4);
    put32(b, 0x608, 0x2300);
    return b;
}

// A zip of stored (uncompressed) files: local headers, the central directory and the end record.
inline Bytes zip(const std::vector<std::pair<std::string, Bytes>>& files) {
    Bytes out;
    Bytes central;
    const auto add16 = [](Bytes& b, std::uint32_t v) {
        b.push_back(static_cast<std::uint8_t>(v));
        b.push_back(static_cast<std::uint8_t>(v >> 8));
    };
    const auto add32 = [&](Bytes& b, std::uint32_t v) {
        add16(b, v & 0xFFFF);
        add16(b, v >> 16);
    };
    for (const auto& [name, data] : files) {
        const std::uint32_t offset = static_cast<std::uint32_t>(out.size());
        add32(out, 0x04034B50);
        add16(out, 20);
        add16(out, 0);
        add16(out, 0);                          // stored
        add32(out, 0);                          // time and date
        add32(out, 0);                          // CRC (the reader does not check it)
        add32(out, static_cast<std::uint32_t>(data.size()));
        add32(out, static_cast<std::uint32_t>(data.size()));
        add16(out, static_cast<std::uint32_t>(name.size()));
        add16(out, 0);
        out.insert(out.end(), name.begin(), name.end());
        out.insert(out.end(), data.begin(), data.end());
        add32(central, 0x02014B50);
        add16(central, 20);
        add16(central, 20);
        add16(central, 0);
        add16(central, 0);
        add32(central, 0);
        add32(central, 0);
        add32(central, static_cast<std::uint32_t>(data.size()));
        add32(central, static_cast<std::uint32_t>(data.size()));
        add16(central, static_cast<std::uint32_t>(name.size()));
        add16(central, 0);
        add16(central, 0);
        add16(central, 0);
        add16(central, 0);
        add32(central, 0);
        add32(central, offset);
        central.insert(central.end(), name.begin(), name.end());
    }
    const std::uint32_t at = static_cast<std::uint32_t>(out.size());
    out.insert(out.end(), central.begin(), central.end());
    add32(out, 0x06054B50);
    add16(out, 0);
    add16(out, 0);
    add16(out, static_cast<std::uint32_t>(files.size()));
    add16(out, static_cast<std::uint32_t>(files.size()));
    add32(out, static_cast<std::uint32_t>(central.size()));
    add32(out, at);
    add16(out, 0);
    return out;
}

inline Bytes text(const std::string& s) { return Bytes(s.begin(), s.end()); }

// An .msix: the manifest, the app's executable, a DLL and a resource.
inline Bytes msix(const std::string& executable = "app.exe", const std::string& architecture = "x64") {
    const std::string manifest = "<?xml version=\"1.0\"?><Package><Identity Name=\"Demo.App\" Publisher=\"CN=Demo\" Version=\"1.2.3.0\" ProcessorArchitecture=\"" + architecture +
                                 "\"/><Applications><Application Id=\"App\" Executable=\"" + executable + "\" EntryPoint=\"Demo.App\"/></Applications></Package>";
    return zip({{"AppxManifest.xml", text(manifest)}, {"app.exe", pe()}, {"lib.dll", pe(false)}, {"resources.pri", Bytes(16, 1)}});
}

// An .msixbundle: a package for another architecture (no code) and the x64 one.
inline Bytes msixbundle() {
    return zip({{"AppxMetadata/AppxBundleManifest.xml", text("<Bundle/>")}, {"demo_scale-100.msix", zip({{"AppxManifest.xml", text("<Package/>")}})}, {"demo_x64.msix", msix()}});
}

}  // namespace cb::formats::xboxone_test
