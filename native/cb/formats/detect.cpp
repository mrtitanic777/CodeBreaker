// cb/formats/detect.cpp -- what kind of file a file is (detect.hpp).
#include "cb/formats/detect.hpp"

#include <algorithm>
#include <array>
#include <format>

#include "cb/core/fs.hpp"
#include "cb/formats/dos.hpp"
#include "cb/formats/read.hpp"

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
    Magic{"WBND"sv, "XACT wave bank (XWB, big-endian)"sv, "xbox360"sv, "info"sv},
    Magic{"DNBW"sv, "XACT wave bank (XWB, little-endian)"sv, "xbox"sv, "info"sv},
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
