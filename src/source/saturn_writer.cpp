// source/saturn_writer.cpp -- a Sega Saturn disc's boot sectors and first program as assembly source (saturn_writer.hpp).

#include "source/saturn_writer.hpp"
#include "knowledge/package.hpp"

#include <algorithm>
#include <format>

#include "core/fs.hpp"
#include "core/text.hpp"
#include "source/isa_sh2.hpp"
#include "source/sh2_writer.hpp"

namespace cb::source::saturn_writer {

namespace iw = image_writer;
namespace fs_ = formats::saturn;

namespace {

constexpr std::uint32_t kHeaderBytes = 0x100;

// A field of the header: where it is, how long, and what it is.
struct Field {
    std::uint32_t at, length;
    const char* what;
    bool number;   // a longword written as a number (not as text)
};
constexpr Field kFields[] = {
    {0x00, 16, "hardware ID", false},       {0x10, 16, "maker ID", false},           {0x20, 10, "product number", false},
    {0x2A, 6, "version", false},            {0x30, 8, "release date", false},        {0x38, 8, "device information", false},
    {0x40, 10, "area symbols", false},      {0x4A, 6, "(padding)", false},           {0x50, 16, "compatible peripherals", false},
    {0x60, 112, "game title", false},       {0xD0, 16, "(reserved)", false},         {0xE0, 4, "IP size", true},
    {0xE4, 4, "(reserved)", true},          {0xE8, 4, "master SH-2 stack pointer", true}, {0xEC, 4, "slave SH-2 stack pointer", true},
    {0xF0, 4, "first program: load address", true}, {0xF4, 4, "first program: size (0: the whole file)", true}, {0xF8, 8, "(reserved)", false}};

bool all_printable(ByteView b) {
    return std::ranges::all_of(b, [](std::uint8_t c) { return c >= 32 && c < 127 && c != '"' && c != '\\'; });
}

// A disc of 2048-byte sectors that holds `image` (the 32 KB system area, then the first program): the system area, an ISO 9660 volume whose
// root directory has the one file `name`, and the program in the sectors after. The reader (formats/saturn) finds the same two parts in it.
Bytes minimal_disc(ByteView image, const std::string& name) {
    constexpr std::size_t kS = fs_::kSector;
    constexpr std::uint32_t kRootSector = 17, kProgramSector = 18;
    const ByteView program = image.subspan(fs_::kSystemBytes);
    Bytes disc((kProgramSector + (program.size() + kS - 1) / kS) * kS, 0);
    std::copy_n(image.begin(), fs_::kSystemBytes, disc.begin());
    const auto both32 = [&](std::size_t at, std::uint32_t v) {   // a 32-bit field written little-endian and then big-endian
        for (std::size_t i = 0; i < 4; ++i) {
            disc[at + i] = static_cast<std::uint8_t>(v >> (8 * i));
            disc[at + 4 + i] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
        }
    };
    const auto record = [&](std::size_t at, std::uint32_t sector, std::uint32_t size, bool directory, const std::string& id) {
        const std::size_t length = 33 + id.size() + (id.size() % 2 == 0 ? 1 : 0);
        disc[at] = static_cast<std::uint8_t>(length);
        both32(at + 2, sector);
        both32(at + 10, size);
        disc[at + 25] = directory ? 2 : 0;
        disc[at + 28] = 1;   // the volume sequence number, both-endian
        disc[at + 31] = 1;
        disc[at + 32] = static_cast<std::uint8_t>(id.size());
        std::copy(id.begin(), id.end(), disc.begin() + static_cast<std::ptrdiff_t>(at + 33));
        return length;
    };
    const std::size_t pvd = 16 * kS;
    disc[pvd] = 1;
    std::copy_n("CD001", 5, disc.begin() + static_cast<std::ptrdiff_t>(pvd + 1));
    disc[pvd + 6] = 1;
    const auto text_at = [&](std::size_t at, std::string_view s, std::size_t width) {
        for (std::size_t i = 0; i < width; ++i) disc[at + i] = i < s.size() ? static_cast<std::uint8_t>(s[i]) : ' ';
    };
    text_at(pvd + 8, "SEGA SATURN", 32);
    text_at(pvd + 40, "CODEBREAKER", 32);
    both32(pvd + 80, static_cast<std::uint32_t>(disc.size() / kS));
    disc[pvd + 128] = 0;   // the logical block size: 2048, both-endian
    disc[pvd + 129] = 8;
    disc[pvd + 130] = 8;
    disc[pvd + 131] = 0;
    record(pvd + 156, kRootSector, kS, true, std::string(1, '\0'));
    std::size_t at = kRootSector * kS;
    at += record(at, kRootSector, kS, true, std::string(1, '\0'));
    at += record(at, kRootSector, kS, true, std::string(1, '\1'));
    record(at, kProgramSector, static_cast<std::uint32_t>(program.size()), false, name + ";1");
    std::copy(program.begin(), program.end(), disc.begin() + static_cast<std::ptrdiff_t>(kProgramSector * kS));
    return disc;
}

}  // namespace

const std::map<std::uint32_t, std::string>& hardware() {
    // the Saturn's own registers (config/knowledge/saturn/registers.json) and the SH-2's (sh2/registers.json), which name an address both do
    static const std::map<std::uint32_t, std::string> table = [] {
        std::map<std::uint32_t, std::string> t = knowledge::standard().hardware_names("saturn", "hardware");
        for (const auto& [a, n] : sh2_writer::chip_registers()) t[a] = n;
        return t;
    }();
    return table;
}

iw::Written write(const fs_::Disc& disc, std::string_view file_name, const std::filesystem::path& folder) {
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const std::string base_name = std::filesystem::path(file_name).filename().string();
    const std::uint32_t ip = disc.ip_length();
    const std::uint32_t system_size = static_cast<std::uint32_t>(disc.system.size());
    const std::uint32_t load = disc.load_address();

    sh2_writer::Image image;
    image.bytes = disc.system;
    image.bytes.insert(image.bytes.end(), disc.first_read.begin(), disc.first_read.end());
    image.parts.push_back({"header", 0, kHeaderBytes, fs_::kIpBase, false, true});
    image.parts.push_back({"ip", kHeaderBytes, ip - kHeaderBytes, fs_::kIpBase + kHeaderBytes, true, true});
    if (ip < system_size) image.parts.push_back({"system_tail", ip, system_size - ip, fs_::kIpBase + ip, false, false});   // (the rest of the system area is never loaded)
    if (!disc.first_read.empty()) image.parts.push_back({"first_read", system_size, static_cast<std::uint32_t>(disc.first_read.size()), load, true, true});

    static constexpr const char* kAreas[] = {"ip_area_jp", "ip_area_tw", "ip_area_us", "ip_area_eu"};
    for (unsigned i = 0; i < 4; ++i) {
        const std::uint32_t at = fs_::kIpBase + fs_::kAreaEntries + 0x20 * i;
        if (fs_::kAreaEntries + 0x20 * i + 2 <= ip) {
            image.entries.push_back(at);
            image.names[at] = kAreas[i];
        }
    }
    if (fs_::kInitAt + 2 <= ip) {
        image.entries.push_back(fs_::kIpBase + fs_::kInitAt);
        image.names[fs_::kIpBase + fs_::kInitAt] = "ip_init";
    }
    if (!disc.first_read.empty()) {
        image.entries.push_back(load);
        image.names[load] = "first_read";
    }
    for (const std::uint32_t a : disc.functions(isa::sh2_decoder())) image.entries.push_back(a);

    image.hardware = &hardware();
    const std::uint32_t load_end = load + static_cast<std::uint32_t>(disc.first_read.size());
    image.is_variable = [load, load_end](std::uint32_t a) {
        const bool in_work_ram = (a >= 0x00200000 && a < 0x00300000) || (a >= fs_::kHighRam && a < fs_::kHighRam + fs_::kHighRamSize);
        const bool in_video_or_sound = (a >= 0x05A00000 && a < 0x05A80000) || (a >= 0x05C00000 && a < 0x05CC0000) || (a >= 0x05E00000 && a < 0x05E80000) ||
                                       (a >= 0x05F00000 && a < 0x05F01000);
        return (in_work_ram && !(a >= load && a < load_end)) || in_video_or_sound;
    };
    image.module = std::string(file_name);
    image.head = {std::format("; {}.s -- written by CodeBreaker from {}", stem, base_name),
                  std::format("; Saturn disc \"{}\" ({}, areas {}): IP.BIN of {} bytes at 0x{:08X}, first program {} of {} bytes at 0x{:08X}",
                              disc.header.title, disc.header.product, disc.header.areas, ip, fs_::kIpBase,
                              disc.first_read_name.empty() ? std::string("(none)") : disc.first_read_name, disc.first_read.size(), load)};
    // the header, field by field
    const Bytes& bytes = image.bytes;
    image.special = [&bytes](std::uint32_t off, std::vector<std::string>& lines) -> std::uint32_t {
        if (off >= kHeaderBytes) return 0;
        for (const Field& f : kFields) {
            if (f.at != off) continue;
            const ByteView v = ByteView(bytes).subspan(f.at, f.length);
            if (f.number) {
                const std::uint32_t n = static_cast<std::uint32_t>(v[0]) << 24 | static_cast<std::uint32_t>(v[1]) << 16 | static_cast<std::uint32_t>(v[2]) << 8 | v[3];
                lines.push_back(std::format("        .long 0x{:08X}    ; {}", n, f.what));
            } else if (all_printable(v)) {
                lines.push_back(std::format("        .ascii \"{}\"    ; {}", as_text(v), f.what));
            } else if (std::ranges::all_of(v, [](std::uint8_t c) { return c == 0; })) {
                lines.push_back(std::format("        .space {}    ; {}", f.length, f.what));
            } else {
                std::vector<std::string> items;
                for (const std::uint8_t c : v) items.push_back(std::format("0x{:02X}", c));
                lines.push_back(std::format("        .byte {}    ; {}", text::join(items, ", "), f.what));
            }
            return f.length;
        }
        return 0;
    };

    iw::Written r = sh2_writer::write(image, stem, folder, stem + ".built.bin");
    // what the lane runs beside the original: the rebuilt system area and program as a disc of their own, which the reader takes like any
    if (r.linked && r.built && !disc.first_read.empty() && !disc.first_read_name.empty()) {
        const Bytes built = fs::read_file(*r.built);
        const std::filesystem::path iso = folder / (stem + ".built.iso");
        fs::write_file(iso, minimal_disc(built, disc.first_read_name));
        r.built = iso;
    }
    return r;
}

iw::Written reconstruct(const std::filesystem::path& image_file, const std::filesystem::path& folder) {
    return write(fs_::read(image_file), image_file.filename().string(), folder);
}

}  // namespace cb::source::saturn_writer
