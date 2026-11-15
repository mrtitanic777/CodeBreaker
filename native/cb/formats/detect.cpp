// cb/formats/detect.cpp -- what kind of file a file is (detect.hpp).
#include "cb/formats/detect.hpp"

#include <algorithm>
#include <array>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/atari5200.hpp"
#include "cb/formats/cdi.hpp"
#include "cb/formats/dos.hpp"
#include "cb/formats/jaguar.hpp"
#include "cb/formats/n64.hpp"
#include "cb/formats/pce.hpp"
#include "cb/formats/ppc_program.hpp"
#include "cb/formats/read.hpp"
#include "cb/formats/dreamcast.hpp"
#include "cb/formats/saturn.hpp"
#include "cb/formats/sega32x.hpp"
#include "cb/formats/segacd.hpp"
#include "cb/formats/snes.hpp"
#include "cb/formats/threedo.hpp"
#include "cb/formats/wonderswan.hpp"

namespace cb::formats::detect {

namespace {

constexpr std::uint64_t kMaxNewHeaderOffset = 0x10000000;

// Whether `file` can be mapped and is not a directory; the map is empty for an empty file.
std::optional<fs::MappedFile> map_file(const std::filesystem::path& file) {
    std::error_code ec;
    if (!std::filesystem::is_regular_file(file, ec)) return std::nullopt;
    try {
        return fs::MappedFile(file);
    } catch (const IoError&) {  // probing: an unreadable file is an ordinary "unknown"
        return std::nullopt;
    }
}

NewHeader signature_from(ByteView s) {
    if (starts_with(s, 0, "PE")) return NewHeader::Pe;
    if (starts_with(s, 0, "NE")) return NewHeader::Ne;
    if (starts_with(s, 0, "LE")) return NewHeader::Le;
    if (starts_with(s, 0, "LX")) return NewHeader::Lx;
    if (starts_with(s, 0, "W3")) return NewHeader::W3;
    if (starts_with(s, 0, "W4")) return NewHeader::W4;
    return NewHeader::None;
}

}  // namespace

const char* signature_text(NewHeader sig) {
    switch (sig) {
        case NewHeader::Pe: return "PE";
        case NewHeader::Ne: return "NE";
        case NewHeader::Le: return "LE";
        case NewHeader::Lx: return "LX";
        case NewHeader::W3: return "W3";
        case NewHeader::W4: return "W4";
        case NewHeader::None: return "";
    }
    return "";
}

const char* lane_name(Lane l) {
    switch (l) {
        case Lane::Pe: return "pe";
        case Lane::Ne: return "ne";
        case Lane::Le: return "le";
        case Lane::Dos: return "dos";
    }
    return "";
}

std::optional<NewHeader> new_header(ByteView d) {
    if (d.size() < 0x40 || !starts_with(d, 0, "MZ")) return std::nullopt;
    const std::uint64_t off = get<std::uint32_t>(d, 0x3C, "detect");
    if (off >= kMaxNewHeaderOffset) return NewHeader::None;
    return signature_from(clamped(d, off, 2));
}

std::optional<NewHeader> new_header(const std::filesystem::path& file) {
    auto map = map_file(file);
    return map ? new_header(map->view()) : std::nullopt;
}

Lane lane_of(NewHeader sig) {
    switch (sig) {
        case NewHeader::Pe: return Lane::Pe;
        case NewHeader::Ne: return Lane::Ne;
        case NewHeader::Le:
        case NewHeader::Lx: return Lane::Le;
        default: return Lane::Dos;
    }
}

std::optional<Lane> lane(const std::filesystem::path& file) {
    auto sig = new_header(file);
    return sig ? std::optional<Lane>(lane_of(*sig)) : std::nullopt;
}

std::optional<std::string> format_of(const std::filesystem::path& file) {
    auto sig = new_header(file);
    if (!sig) return std::nullopt;
    return *sig == NewHeader::None ? std::string("MZ") : std::string(signature_text(*sig));
}

std::optional<Lane> kind(const std::filesystem::path& file) {
    if (auto l = lane(file)) return l;
    auto map = map_file(file);
    if (!map) return std::nullopt;
    if (dos::kind_of(map->view(), file.filename().string())) return Lane::Dos;
    return std::nullopt;
}

bool holds_drivers(const std::filesystem::path& file, bool w3_bundles, bool w4_bundles) {
    auto sig = new_header(file);
    if (!sig) return false;
    return *sig == NewHeader::Le || *sig == NewHeader::Lx || (w3_bundles && *sig == NewHeader::W3) || (w4_bundles && *sig == NewHeader::W4);
}

std::optional<DosHeader> dos_header(ByteView d) {
    if (d.size() < 0x1C || !starts_with(d, 0, "MZ")) return std::nullopt;
    std::uint16_t f[13];
    for (int k = 0; k < 13; ++k) f[k] = get<std::uint16_t>(d, 2 + 2 * static_cast<std::uint64_t>(k), "detect");
    DosHeader h;
    const std::int64_t last = f[0], pages = f[1];
    h.bytes_in_last_page = f[0];
    h.pages = f[1];
    h.relocations = f[2];
    h.header_paragraphs = f[3];
    h.header_size = std::uint32_t{f[3]} * 16;
    h.min_alloc = f[4];
    h.max_alloc = f[5];
    h.ss = f[6];
    h.sp = f[7];
    h.checksum = f[8];
    h.ip = f[9];
    h.cs = f[10];
    h.relocation_table = f[11];
    h.overlay = f[12];
    h.new_header = d.size() >= 0x40 ? get<std::uint32_t>(d, 0x3C, "detect") : 0;
    const std::int64_t image = std::max<std::int64_t>(0, pages * 512 - (last ? 512 - last : 0) - std::int64_t{h.header_size});
    const std::int64_t rest = std::max<std::int64_t>(0, static_cast<std::int64_t>(d.size()) - std::int64_t{h.header_size});
    h.load_size = static_cast<std::uint64_t>(std::min(image, rest));
    for (std::uint64_t i = 0; i < f[2]; ++i) {
        const std::uint64_t at = std::uint64_t{f[11]} + i * 4;
        if (at + 4 > d.size()) break;
        h.relocation_entries.push_back({get<std::uint16_t>(d, at + 2, "detect"), get<std::uint16_t>(d, at, "detect")});
    }
    return h;
}

// ---- identify -------------------------------------------------------------------------------------------------

namespace {

struct Magic {
    std::string_view magic, format, family, command;
};
using namespace std::string_view_literals;
constexpr std::array kMagic = {
    Magic{"XEX2"sv, "Xbox 360 executable (XEX2)"sv, "xbox360"sv, "xex"sv},
    Magic{"XEX1"sv, "Xbox 360 executable (XEX1, early kit)"sv, "xbox360"sv, "xex"sv},
    Magic{"XEX%"sv, "Xbox 360 executable (XEX%, prototype)"sv, "xbox360"sv, "xex"sv},
    Magic{"XBEH"sv, "original Xbox executable (XBE)"sv, "xbox"sv, "info"sv},
    Magic{"FATX"sv, "FATX filesystem (Xbox drive / memory unit)"sv, "xbox"sv, "media"sv},
    Magic{"CISO"sv, "compressed disc image (CSO)"sv, "xbox"sv, "media"sv},
    Magic{"CCIM"sv, "compressed disc image (CCI)"sv, "xbox"sv, "media"sv},
    Magic{"XUIZ"sv, "XUI package (XZP)"sv, "xbox360"sv, "media"sv},
    Magic{"XPR0"sv, "Xbox resource bundle (XPR0)"sv, "xbox"sv, "info"sv},
    Magic{"XPR2"sv, "Xbox 360 resource bundle (XPR2)"sv, "xbox360"sv, "info"sv},
    Magic{"XDBF"sv, "Xbox 360 database (XDBF: GPD / SPA)"sv, "xbox360"sv, "info"sv},
    Magic{"WBND"sv, "XACT wave bank (XWB, little-endian)"sv, "xbox"sv, "info"sv},
    Magic{"DNBW"sv, "XACT wave bank (XWB, big-endian)"sv, "xbox360"sv, "info"sv},
    Magic{"SDBK"sv, "XACT sound bank (XSB, little-endian)"sv, "xbox"sv, ""sv},
    Magic{"KBDS"sv, "XACT sound bank (XSB, big-endian)"sv, "xbox360"sv, ""sv},
    Magic{"XUIB"sv, "compiled XUI scene (XUR)"sv, "xbox360"sv, "info"sv},
    Magic{"MComprHD"sv, "CHD compressed disc image"sv, "disc"sv, "info"sv},
    Magic{"BIK"sv, "Bink video"sv, "media"sv, "info"sv},
    Magic{"DDS "sv, "DirectDraw Surface texture (DDS)"sv, "media"sv, ""sv},
    Magic{"MSCF"sv, "Microsoft cabinet (CAB)"sv, "windows"sv, "cab"sv},
    Magic{"\x7f" "ELF"sv, "ELF executable / object"sv, "unix"sv, ""sv},
    Magic{"dex\n"sv, "Android DEX bytecode"sv, "android"sv, ""sv},
    Magic{"\xfe\xed\xfa\xce"sv, "Mach-O (32-bit)"sv, "apple"sv, ""sv},
    Magic{"\xfe\xed\xfa\xcf"sv, "Mach-O (64-bit)"sv, "apple"sv, ""sv},
    Magic{"\xce\xfa\xed\xfe"sv, "Mach-O (32-bit)"sv, "apple"sv, ""sv},
    Magic{"\xcf\xfa\xed\xfe"sv, "Mach-O (64-bit)"sv, "apple"sv, ""sv},
    Magic{"NES\x1a"sv, "NES ROM (iNES)"sv, "nes"sv, ""sv},
    Magic{"\xff\x4f"sv, "Xbox 360 NAND flash image"sv, "xbox360"sv, ""sv},
    Magic{"PK\x03\x04"sv, "ZIP archive (or .jar / .apk / .xlsx)"sv, "archive"sv, ""sv},
    Magic{"RIFF"sv, "RIFF container (WAV / AVI / XMA)"sv, "media"sv, "info"sv},
    Magic{"XNBw"sv, "XNA asset (XNB, Windows)"sv, "xna"sv, ""sv},
    Magic{"XNBx"sv, "XNA asset (XNB, Xbox 360)"sv, "xna"sv, ""sv},
    Magic{"XNBm"sv, "XNA asset (XNB, Windows Phone)"sv, "xna"sv, ""sv},
    Magic{"Microsoft C/C++ MSF 7.00"sv, "program database (PDB)"sv, "windows"sv, ""sv},
    Magic{"Microsoft C/C++ program database 2.00"sv, "program database (PDB 2.0)"sv, "windows"sv, ""sv},
    Magic{"\x89PNG"sv, "PNG image"sv, "media"sv, ""sv},
    Magic{"\xff\xd8\xff"sv, "JPEG image"sv, "media"sv, ""sv},
    Magic{"GIF8"sv, "GIF image"sv, "media"sv, ""sv},
    Magic{"OggS"sv, "Ogg audio / video"sv, "media"sv, ""sv},
    Magic{"fLaC"sv, "FLAC audio"sv, "media"sv, ""sv},
    Magic{"ID3"sv, "MP3 audio"sv, "media"sv, ""sv},
};

struct Content {
    std::uint32_t type;
    std::string_view name;
};
constexpr std::array kContent = {
    Content{0x00000001, "saved game"sv},        Content{0x00000002, "marketplace / DLC content"sv},
    Content{0x00000003, "publisher"sv},         Content{0x00001000, "Xbox 360 title (installer)"sv},
    Content{0x00002000, "IPTV DVR"sv},          Content{0x00004000, "Xbox 360 title"sv},
    Content{0x00005000, "original Xbox title"sv}, Content{0x00007000, "Games on Demand"sv},
    Content{0x00009000, "avatar item"sv},       Content{0x00010000, "profile"sv},
    Content{0x00020000, "gamer picture"sv},     Content{0x00030000, "theme"sv},
    Content{0x00040000, "cache file"sv},        Content{0x00050000, "storage download"sv},
    Content{0x00070000, "installer"sv},         Content{0x000B0000, "community game"sv}};

const char* coff_name(std::uint32_t m) {
    switch (m) {
        case 0x014C: return "x86";
        case 0x8664: return "x86-64";
        case 0x01F2: return "PowerPC big-endian (Xbox 360)";
        case 0x01F0: return "PowerPC";
        case 0x01C0: return "ARM";
        case 0x01C4: return "ARM Thumb-2";
        case 0xAA64: return "ARM64";
        default: return nullptr;
    }
}

std::vector<Detected> one(std::string format, std::string detail, std::string_view family, std::string_view command) {
    return {Detected{std::move(format), std::move(detail), std::string(family), std::string(command)}};
}

// The Bink codec letter in a detail string: a printable ASCII letter as it is, anything else as \xNN.
std::string codec_text(std::uint8_t ch) {
    if (ch >= 0x20 && ch < 0x7F && ch != '\'' && ch != '\\') return std::format("'{}'", static_cast<char>(ch));
    return std::format("'\\x{:02x}'", ch);
}

// Does the file name end in one of the (lower-case) extensions, whatever its case?
bool name_ends_with(std::string_view name, std::initializer_list<std::string_view> extensions) {
    for (const std::string_view e : extensions) {
        if (name.size() >= e.size() && std::equal(e.begin(), e.end(), name.end() - static_cast<std::ptrdiff_t>(e.size()), [](char a, char b) { return a == (b >= 'A' && b <= 'Z' ? b + 32 : b); })) return true;
    }
    return false;
}

std::uint32_t be32_or_zero(ByteView d, std::uint64_t off) {
    auto v = try_get<std::uint32_t>(d, off);
    if (!v) return 0;
    return ((*v & 0xFF) << 24) | ((*v & 0xFF00) << 8) | ((*v >> 8) & 0xFF00) | (*v >> 24);
}

}  // namespace

std::vector<Detected> identify(ByteView file, std::string_view name) {
    const std::uint64_t size = file.size();
    const ByteView head = clamped(file, 0, 0x400);
    if (starts_with(head, 0, "MZ")) {
        if (auto sig = new_header(head.size() >= 0x40 ? file : ByteView())) {
            if (*sig == NewHeader::Pe) {
                const std::uint64_t e = get<std::uint32_t>(file, 0x3C, "detect");
                const ByteView m = clamped(file, e + 4, 2);
                const std::uint32_t machine = m.size() == 2 ? get<std::uint16_t>(m, 0, "detect") : 0;
                const char* cpu = coff_name(machine);
                return one("PE executable", cpu ? std::string(cpu) : std::format("machine 0x{:04X}", machine),
                           machine == 0x01F2 ? "xbox360" : "windows", "info");
            }
            if (*sig == NewHeader::W3 || *sig == NewHeader::W4)
                return one(std::string("driver bundle (") + signature_text(*sig) + ")", "VxDs stored together", "windows", "info");
            const char* what = *sig == NewHeader::Ne   ? "16-bit Windows (NE)"
                               : *sig == NewHeader::Le ? "virtual device driver (LE)"
                               : *sig == NewHeader::Lx ? "OS/2 / VxD (LX)"
                                                       : "DOS program (MZ)";
            return one(what, "", *sig == NewHeader::None ? "dos" : "windows", "info");
        }
    }
    const ByteView h4 = clamped(head, 0, 4);
    if (starts_with(h4, 0, "CON ") || starts_with(h4, 0, "LIVE") || starts_with(h4, 0, "PIRS")) {
        const ByteView t = clamped(file, 0x344, 4);
        Bytes padded(4 - t.size(), 0);  // a short read is padded on the left
        append(padded, t);
        const std::uint32_t type = be32_or_zero(padded, 0);
        std::string detail;
        for (const Content& k : kContent)
            if (k.type == type) detail = k.name;
        if (detail.empty()) detail = std::format("content type 0x{:X}", type);
        std::string tag(as_text(h4));
        while (!tag.empty() && tag.back() == ' ') tag.pop_back();
        return one("STFS package (" + tag + ")", detail, "xbox360", "media");
    }
    if (starts_with(h4, 0, "\xca\xfe\xba\xbe")) {
        const std::uint32_t n = head.size() >= 8 ? be32_or_zero(head, 4) : 0;
        if (0 < n && n < 30) return one("Mach-O universal binary", std::format("{} architectures", n), "apple", "");
        return one("Java class file", "", "java", "java");
    }
    if (starts_with(head, 0, "PS-X EXE") && size >= 0x800) {   // a PlayStation executable: the header says where it starts and loads
        return one("PlayStation executable (PS-X EXE)",
                   std::format("entry 0x{:08X}, program at 0x{:08X}, {} bytes", get<std::uint32_t>(file, 0x10, "detect"),
                               get<std::uint32_t>(file, 0x18, "detect"), size - 0x800),
                   "playstation", "reach");
    }
    if (size >= 52 && head.size() >= 52 && head[0] == 0x7F && head[1] == 'E' && head[2] == 'L' && head[3] == 'F' && head[4] == 1 && head[5] == 1 && (head[18] | head[19] << 8) == 8) {   // a little-endian MIPS ELF: the flags say which console
        const std::uint32_t flags = get<std::uint32_t>(file, 36, "detect");
        const std::uint32_t entry = get<std::uint32_t>(file, 24, "detect");
        if ((flags & 0x00FF0000u) == 0x00920000u) return one("PlayStation 2 executable (ELF)", std::format("Emotion Engine R5900, entry 0x{:08X}, {} bytes", entry, size), "ps2", "reach");
        if ((flags & 0x00FF0000u) == 0x00A20000u)
            return one((head[16] | head[17] << 8) == 0xFFA0 ? "PSP module (PRX)" : "PSP executable (ELF)", std::format("Allegrex, entry 0x{:08X}, {} bytes", entry, size), "psp", "reach");
    }
    if (size >= 40 && head.size() >= 40 && starts_with(head, 0, std::string_view(" PBP", 4))) {   // an EBOOT.PBP: its DATA.PSP is an ELF, or an encrypted ~PSP module (recorded, not handled)
        const std::uint32_t at = get<std::uint32_t>(file, 8 + 4 * 6, "detect");
        if (at + 4 <= size && starts_with(file, at, "~PSP")) return one("PSP EBOOT.PBP (encrypted ~PSP module)", "not handled: the module is encrypted", "psp", "gap");
        if (at + 4 <= size && starts_with(file, at, "" "ELF")) return one("PSP EBOOT.PBP", std::format("DATA.PSP at 0x{:X}, {} bytes", at, size), "psp", "reach");
    }
    if (const std::optional<n64::ByteOrder> order = size >= n64::kBootSize ? n64::order_of(head) : std::nullopt; order && head.size() >= n64::kHeaderSize) {   // a Nintendo 64 ROM: the PI configuration word $80371240, in one of three byte orders
        const Bytes z64 = n64::Rom::convert(head.first(n64::kHeaderSize), *order, n64::ByteOrder::Z64);
        const auto word = [&](std::size_t at) { return static_cast<std::uint32_t>(z64[at]) << 24 | static_cast<std::uint32_t>(z64[at + 1]) << 16 | static_cast<std::uint32_t>(z64[at + 2]) << 8 | z64[at + 3]; };
        std::string title(as_text(ByteView(z64).subspan(0x20, 20)));
        while (!title.empty() && (title.back() == ' ' || title.back() == '\0')) title.pop_back();
        return one("Nintendo 64 ROM", std::format("\"{}\" ({}{}), {} ({}), {} bytes, entry 0x{:08X}", title, as_text(ByteView(z64).subspan(0x3C, 2)), static_cast<char>(z64[0x3E]), std::string(n64::order_text(*order)).substr(1),
                                                    *order == n64::ByteOrder::Z64 ? "big-endian" : *order == n64::ByteOrder::V64 ? "byte-swapped" : "little-endian", size, word(8)),
                   "n64", "reach");
    }
    if (head.size() >= 0x200 && head[0x15C] == 0x56 && head[0x15D] == 0xCF && head[0xC0] == 0x24 && head[0xC1] == 0xFF) {   // a Nintendo DS ROM: the logo, and its checksum $CF56 at $15C
        std::string title(as_text(clamped(head, 0, 12)));
        while (!title.empty() && title.back() == '\0') title.pop_back();
        std::string code(as_text(clamped(head, 0x0C, 4)));
        return one("Nintendo DS cartridge", std::format("\"{}\" ({}), {} bytes, ARM9 {} bytes at 0x{:08X}, ARM7 {} bytes at 0x{:08X}", title, code, size, get<std::uint32_t>(file, 0x2C, "detect"), get<std::uint32_t>(file, 0x28, "detect"), get<std::uint32_t>(file, 0x3C, "detect"), get<std::uint32_t>(file, 0x38, "detect")), "nds", "reach");
    }
    if (head.size() >= 0xC0 && head[0xB2] == 0x96 && (head[3] & 0x0E) == 0x0A && (head[3] >> 4) != 0xF) {   // a Game Boy Advance cartridge: $96 at $B2 and the entry branch
        std::string title(as_text(clamped(head, 0xA0, 12)));
        while (!title.empty() && title.back() == '\0') title.pop_back();
        std::string code(as_text(clamped(head, 0xAC, 4)));
        return one("Game Boy Advance cartridge", std::format("\"{}\" ({}), {} bytes", title, code, size), "gba", "reach");
    }
    if (name_ends_with(name, {".pce", ".tg16", ".sgx"})) {   // a PC Engine / TurboGrafx-16 HuCard: whole 8 KB banks, a RESET vector into the top page
        try {
            const formats::pce::Rom rom = formats::pce::parse(file);
            return one("PC Engine HuCard", std::format("{} banks of 8 KB{}, RESET ${:04X}", rom.bank_count(), rom.header.empty() ? "" : " behind a 512-byte header", rom.vectors.reset), "pce", "reach");
        } catch (const FormatError&) {
        }
    }
    if (name_ends_with(name, {".sfc", ".smc", ".swc", ".fig"})) {   // a Super Nintendo cartridge: an internal header at $7FC0 or $FFC0 that reads as one
        try {
            const formats::snes::Rom rom = formats::snes::parse(file);
            return one("Super Nintendo cartridge", std::format("\"{}\", {}, {} bytes{}, RESET ${:04X}", rom.title, formats::snes::to_string(rom.mapping), rom.data.size(),
                                                              rom.copier_header.empty() ? "" : " behind a 512-byte header", rom.emulation.reset), "snes", "reach");
        } catch (const FormatError&) {
        }
    }
    if (name_ends_with(name, {".ws", ".wsc"})) {   // a WonderSwan cartridge: a power-of-two ROM whose last 16 bytes start with the far jump
        try {
            const formats::wonderswan::Rom rom = formats::wonderswan::parse(file);
            return one(rom.is_color() ? "WonderSwan Color cartridge" : "WonderSwan cartridge",
                       std::format("{} KB, start {:04X}:{:04X}, developer {:02X}, game {:02X}, checksum {}", size / 1024, rom.reset_segment, rom.reset_offset, rom.developer, rom.game_id,
                                   rom.checksum_ok ? "right" : "wrong"),
                       "wonderswan", "reach");
        } catch (const FormatError&) {
        }
    }
    if (name_ends_with(name, {".j64", ".jag", ".rom", ".cof", ".abs"})) {   // an Atari Jaguar cartridge (the start address at $404) or COFF executable
        try {
            const formats::jaguar::Program program = formats::jaguar::parse(file);
            return one(program.kind == formats::jaguar::Kind::Cartridge ? "Atari Jaguar cartridge" : "Atari Jaguar COFF executable",
                       std::format("{} bytes, start ${:06X}, {} segment(s)", size, program.entry, program.segments.size()), "jaguar", "reach");
        } catch (const FormatError&) {
        }
    }
    if (starts_with(head, 0, std::string_view("\x01\x5A\x5A\x5A\x5A\x5A\x01", 7)) || name_ends_with(name, {".aif"})) {   // a 3DO disc (the Opera volume header) or an AIF executable
        try {
            const formats::threedo::Program program = formats::threedo::parse(file);
            return one(program.kind == formats::threedo::Kind::Disc ? "3DO disc" : "3DO AIF executable",
                       std::format("{}\"{}\", entry 0x{:06X}, {} bytes of AIF", program.label.empty() ? "" : "\"" + program.label + "\", ", program.name, program.aif.entry, program.image_size),
                       "threedo", "reach");
        } catch (const FormatError&) {
        }
    }
    if (name_ends_with(name, {".a52", ".bin", ".rom"})) {   // an Atari 5200 cartridge: 4, 8, 16 or 32 KB ending with a start address inside the ROM
        try {
            const formats::atari5200::Rom rom = formats::atari5200::parse(file);
            const std::uint32_t first = *rom.offset(rom.start);
            if (name_ends_with(name, {".a52"}) || (rom.data[first] != 0x00 && rom.data[first] != 0xFF)) {
                return one("Atari 5200 cartridge", std::format("{} KB at ${:04X}, start ${:04X}", size / 1024, rom.base, rom.start), "atari5200", "reach");
            }
        } catch (const FormatError&) {
        }
    }
    if (name_ends_with(name, {".iso", ".bin", ".cdi", ".img", ".mod", ".rtf"})) {   // a Philips CD-i disc (the Disc Label "CD-I " in an early sector) or a CD-RTOS module
        bool label = false;
        for (std::size_t at = 0; at + 7 <= file.size() && at <= 16 * formats::cdi::kSector; at += formats::cdi::kSector) label = label || (file[at] == 1 && starts_with(file, at + 1, "CD-I ") && file[at + 6] == 1);
        if (label || formats::cdi::module_at(file, 0)) {
            try {
                const formats::cdi::Program program = formats::cdi::parse(file);
                return one(program.disc ? "Philips CD-i disc" : "CD-RTOS module file", std::format("{} CD-RTOS module(s), {} executable", program.modules.size(), program.placed().size()), "cdi", "reach");
            } catch (const FormatError&) {
            }
        }
    }
    if (head.size() >= 0x108 && (starts_with(head, 0x100, "NEO-GEO") || starts_with(head, 0x100, "EN-OEG"))) {   // a Neo Geo P-ROM: "NEO-GEO" at $100, or with its words swapped
        const bool swapped = starts_with(head, 0x100, "EN-OEG");
        const unsigned ngh = swapped ? head[0x109] << 8 | head[0x108] : head[0x108] << 8 | head[0x109];
        return one("Neo Geo cartridge (P-ROM)", std::format("NGH {:04X}, {} KB{}", ngh, size / 1024, swapped ? ", words swapped" : ""), "neogeo", "reach");
    }
    if (starts_with(h4, 0, "NEO") && size >= 4096) {   // a .neo file: a 4 KB header, then the P, S, M, V and C ROMs
        return one("Neo Geo .neo file", std::format("{} KB P-ROM, {} KB sound program, {} KB sprites", get<std::uint32_t>(file, 4, "detect") / 1024,
                                                      get<std::uint32_t>(file, 12, "detect") / 1024, get<std::uint32_t>(file, 24, "detect") / 1024),
                   "neogeo", "reach");
    }
    if (segacd::is_disc_start(head)) {   // a Sega CD disc: "SEGADISCSYSTEM" starts its first sector, which holds the boot header
        try {
            const segacd::Disc disc = segacd::parse(file);
            return one("Sega CD disc", std::format("\"{}\", {}-byte sectors, IP {} bytes at 0x{:X}, SP {} bytes at 0x{:X}", disc.header.domestic_name, disc.sector_size,
                                                 disc.header.ip.size, disc.header.ip.start, disc.header.sp.size, disc.header.sp.start),
                       "segacd", "reach");
        } catch (const Error&) {   // a damaged boot header is no disc of this machine
        }
    }
    if (dreamcast::is_disc_start(head) || name_ends_with(name, {".gdi", ".cdi"})) {   // a Dreamcast disc: "SEGA SEGAKATANA " starts the boot track's first sector (a .gdi names its tracks; a .cdi is searched)
        try {
            const dreamcast::Disc disc = name_ends_with(name, {".gdi"}) ? dreamcast::read(std::filesystem::path(std::string(name))) : dreamcast::parse(file);
            const char* container = disc.container == dreamcast::Container::Gdi ? "GD-ROM image" : disc.container == dreamcast::Container::Cdi ? "DiscJuggler image" : disc.container == dreamcast::Container::Raw ? "raw sectors" : "2048-byte sectors";
            return one("Sega Dreamcast disc", std::format("\"{}\", {}, {}, IP.BIN {} bytes, first program {}{} at 0x{:08X}", disc.header.title, disc.header.product, container, disc.ip_length(),
                                                        disc.first_read_name.empty() ? std::string("(none)") : disc.first_read_name, disc.scrambled ? " (scrambled)" : "", dreamcast::kFirstReadBase + dreamcast::kCachedView),
                       "dreamcast", "reach");
        } catch (const Error&) {   // not a disc of this machine
        }
    }
    if (saturn::is_disc_start(head)) {   // a Saturn disc: "SEGA SEGASATURN " starts its first sector, which holds IP.BIN's header
        try {
            const saturn::Disc disc = saturn::parse(file);
            return one("Sega Saturn disc", std::format("\"{}\", {}, IP {} bytes, first program {} at 0x{:08X}", disc.header.title, disc.header.product, disc.ip_length(),
                                                     disc.first_read_name.empty() ? std::string("(none)") : disc.first_read_name, disc.load_address()),
                       "saturn", "reach");
        } catch (const Error&) {   // a damaged boot header is no disc of this machine
        }
    }
    {   // a Sega 32X cartridge: a Genesis ROM whose header at $3C0 names the SH-2 programs
        const bool dot_32x = name.size() >= 4 && (name.substr(name.size() - 4) == ".32x" || name.substr(name.size() - 4) == ".32X");
        if (sega32x::is_rom_start(head, dot_32x)) {
            try {
                const sega32x::Rom rom = sega32x::parse(file);
                return one("Sega 32X cartridge", std::format("\"{}\", {} KB, master SH-2 at 0x{:08X}, slave at 0x{:08X}", rom.header.domestic_name, size / 1024,
                                                           rom.header.master_start, rom.header.slave_start),
                           "sega32x", "reach");
            } catch (const Error&) {
            }
        }
    }
    // a GameCube, Wii or Wii U program: a DOL (a header that reads as one), or a PowerPC ELF (devkitPPC's output, a Wii U RPX or RPL)
    if (ppc_elf::is_ppc_elf(head) || name_ends_with(name, {".dol"})) {
        if (const std::optional<ppc_program::Program> program = ppc_program::parse(file)) {
            return one(std::format("Nintendo {} {}", ppc_program::console_text(program->console), ppc_program::kind_text(program->kind)),
                       std::format("{} ({}), entry 0x{:08X}, {} bytes, {} loaded sections{}", ppc_program::cpu_text(program->console), "PowerPC 750 family", program->entry, size,
                                   program->segments.size(), program->elf && program->elf->inflated_sections != 0 ? std::format(" ({} compressed)", program->elf->inflated_sections) : std::string()),
                       std::string(ppc_program::lane_name(program->console)), "reach");
        }
    }
    for (const Magic& m : kMagic) {
        if (starts_with(head, 0, m.magic)) {
            std::string detail;
            if (m.magic == "BIK" && head.size() > 3) detail = "codec " + codec_text(head[3]);
            return one(std::string(m.format), detail, m.family, m.command);
        }
    }
    for (std::uint64_t base : {std::uint64_t{0}, std::uint64_t{0x10000}, std::uint64_t{0xFD90000}, std::uint64_t{0x2080000},
                               std::uint64_t{0xFB20000}, std::uint64_t{0x18300000}})
        if (base + 0x10000 + 20 <= size && starts_with(file, base + 0x10000, "MICROSOFT*XBOX*MEDIA"))
            return one("Xbox disc image (XDVDFS)", std::format("volume at 0x{:X}", base), "xbox", "media");
    for (const auto [sector, skip] : {std::pair{2048, 0}, std::pair{2352, 16}, std::pair{2352, 24}})
        if (starts_with(file, 16 * static_cast<std::uint64_t>(sector) + static_cast<std::uint64_t>(skip) + 1, "CD001"))
            return one("CD image (ISO-9660)", std::format("{}-byte sectors", sector), "disc", "media");
    if (head.size() >= 512 && head[510] == 0x55 && head[511] == 0xAA) {
        bool fat = false;
        if (head[0] == 0xEB || head[0] == 0xE9) {
            const std::uint16_t bps = get<std::uint16_t>(head, 11, "detect");
            fat = bps == 512 || bps == 1024 || bps == 2048 || bps == 4096;
        }
        if (!fat) {  // a partition table: boot flags are 0 or 0x80, and some partition has a type
            bool flags = true, any = false;
            for (int i = 0; i < 4; ++i) {
                const std::uint8_t f = head[0x1BE + 16 * static_cast<std::size_t>(i)];
                if (f != 0 && f != 0x80) flags = false;
            }
            if (flags)
                for (int i = 0; i < 4; ++i)
                    if (head[0x1BE + 16 * static_cast<std::size_t>(i) + 4]) any = true;
            fat = flags && any;
        }
        if (fat) return one("disk image (FAT)", size <= 2949120 ? "floppy image" : "hard-disk image", "disc", "media");
    }
    if (!head.empty() && (head[0] == 0x80 || head[0] == 0x82) && head.size() > 3) return one("OMF object", "", "dos", "");
    if (!head.empty() && head[0] == 0xF0) return one("OMF library", "", "dos", "");
    if (head.size() >= 20) {
        const std::uint32_t machine = get<std::uint16_t>(head, 0, "detect"), nsec = get<std::uint16_t>(head, 2, "detect");
        const char* cpu = coff_name(machine);
        if (cpu && 0 < nsec && nsec < 200)
            return one("COFF object", std::format("{}, {} sections", cpu, nsec), machine == 0x01F2 ? "xbox360" : "windows", "");
    }
    const std::string lower_name = text::lower(name);
    if (lower_name.ends_with(".zar")) return one("ZArchive (ZAR)", "", "archive", "media");
    if (lower_name.ends_with(".pck")) return one("Minecraft LCE package (PCK)", "", "xbox360", "media");
    return {};
}

std::vector<Detected> identify(const std::filesystem::path& file) {
    auto map = map_file(file);
    if (!map) return {};
    return identify(map->view(), file.generic_string());
}

}  // namespace cb::formats::detect
