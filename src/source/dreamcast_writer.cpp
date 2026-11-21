// cb-unity: alone (file-local names that other files in a batch also use)
// source/dreamcast_writer.cpp -- a Sega Dreamcast disc's IP.BIN and first program as assembly source (dreamcast_writer.hpp).

#include "source/dreamcast_writer.hpp"
#include "knowledge/package.hpp"

#include <algorithm>
#include <format>

#include "core/fs.hpp"
#include "core/text.hpp"
#include "source/isa_sh2.hpp"
#include "source/sh2_writer.hpp"

namespace cb::source::dreamcast_writer {

namespace iw = image_writer;
namespace fm = formats::dreamcast;

namespace {

constexpr std::uint32_t kHeaderBytes = 0x100;

// A field of IP.BIN's header: where it is, how long, and what it is.
struct Field {
    std::uint32_t at, length;
    const char* what;
};
constexpr Field kFields[] = {{0x00, 16, "hardware ID"},   {0x10, 16, "maker ID"},        {0x20, 16, "device information: CRC, medium, disc number"},
                             {0x30, 8, "area symbols"},   {0x38, 8, "peripherals"},      {0x40, 10, "product number"},
                             {0x4A, 6, "version"},        {0x50, 16, "release date"},    {0x60, 16, "boot file"},
                             {0x70, 16, "software maker"}, {0x80, 128, "title"}};

bool all_printable(ByteView b) {
    return std::ranges::all_of(b, [](std::uint8_t c) { return c >= 32 && c < 127 && c != '"' && c != '\\'; });
}

}  // namespace

Bytes minimal_disc(ByteView image, const std::string& name, bool scrambled, std::uint32_t volume_base) {
    constexpr std::size_t kS = fm::kSector;
    constexpr std::uint32_t kRootSector = 17, kProgramSector = 18;
    const Bytes program_plain(image.begin() + static_cast<std::ptrdiff_t>(fm::kSystemBytes), image.end());
    const Bytes program = scrambled ? fm::scramble(program_plain) : program_plain;
    Bytes disc((kProgramSector + (program.size() + kS - 1) / kS) * kS, 0);
    std::copy_n(image.begin(), fm::kSystemBytes, disc.begin());
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
    text_at(pvd + 8, "SEGA DREAMCAST", 32);
    text_at(pvd + 40, "CODEBREAKER", 32);
    both32(pvd + 80, static_cast<std::uint32_t>(disc.size() / kS));
    disc[pvd + 128] = 0;   // the logical block size: 2048, both-endian
    disc[pvd + 129] = 8;
    disc[pvd + 130] = 8;
    disc[pvd + 131] = 0;
    record(pvd + 156, kRootSector + volume_base, kS, true, std::string(1, '\0'));
    std::size_t at = kRootSector * kS;
    at += record(at, kRootSector + volume_base, kS, true, std::string(1, '\0'));
    at += record(at, kRootSector + volume_base, kS, true, std::string(1, '\1'));
    record(at, kProgramSector + volume_base, static_cast<std::uint32_t>(program.size()), false, name + ";1");
    std::copy(program.begin(), program.end(), disc.begin() + static_cast<std::ptrdiff_t>(kProgramSector * kS));
    return disc;
}

const std::map<std::uint32_t, std::string>& hardware() {
    // config/knowledge/dreamcast/registers.json
    static const std::map<std::uint32_t, std::string> table = knowledge::standard().hardware_names("dreamcast", "hardware");
    return table;
}

iw::Written write(const fm::Disc& disc, std::string_view file_name, const std::filesystem::path& folder) {
    const std::string stem = std::filesystem::path(file_name).stem().string();
    const std::string base_name = std::filesystem::path(file_name).filename().string();
    const std::uint32_t system_size = static_cast<std::uint32_t>(disc.system.size());
    const std::uint32_t program_size = static_cast<std::uint32_t>(disc.program.size());

    sh2_writer::Image image;
    image.sh4 = true;
    image.bytes = disc.system;
    image.bytes.insert(image.bytes.end(), disc.program.begin(), disc.program.end());
    image.parts.push_back({"header", 0, kHeaderBytes, fm::kIpBase, false, true, false});
    image.parts.push_back({"ip", kHeaderBytes, system_size - kHeaderBytes, fm::kIpBase + kHeaderBytes, true, true});
    if (program_size != 0) image.parts.push_back({"first_read", system_size, program_size, fm::kFirstReadBase, true, true});

    if (fm::kIpEntry + 2 <= system_size) {
        image.entries.push_back(fm::kIpBase + fm::kIpEntry);
        image.names[fm::kIpBase + fm::kIpEntry] = "ip_bootstrap";
    }
    if (program_size != 0) {
        image.entries.push_back(fm::kFirstReadBase);
        image.names[fm::kFirstReadBase] = "first_read";
    }
    for (const std::uint32_t a : disc.functions(isa::sh4_decoder())) image.entries.push_back(a);

    image.hardware = &hardware();
    const std::uint32_t load_end = fm::kFirstReadBase + program_size;
    image.is_variable = [load_end](std::uint32_t a) {
        const bool in_ram = a >= fm::kRamBase && a < fm::kRamBase + fm::kRamSize && !(a >= fm::kIpBase && a < load_end);
        const bool in_video_or_sound = (a >= 0x04000000 && a < 0x04800000) || (a >= 0x05000000 && a < 0x05800000) || (a >= 0x00800000 && a < 0x00A00000);
        return in_ram || in_video_or_sound;
    };
    image.module = std::string(file_name);
    image.head = {std::format("; {}.s -- written by CodeBreaker from {}", stem, base_name),
                  std::format("; Dreamcast disc \"{}\" ({}, areas {}): IP.BIN of {} bytes at 0x{:08X} (cached view 0x{:08X}), {} of {} bytes at 0x{:08X}{}", disc.header.title,
                              disc.header.product, disc.header.areas, system_size, fm::kIpBase, fm::kIpBase + fm::kCachedView,
                              disc.first_read_name.empty() ? std::string("(no first program)") : disc.first_read_name, program_size, fm::kFirstReadBase,
                              disc.scrambled ? ", scrambled on the disc and written here descrambled" : ""),
                  "; (addresses are physical: a program's pointer 0x8Cxxxxxx is written `label + 0x80000000`)"};
    // IP.BIN's header, field by field
    const Bytes& bytes = image.bytes;
    image.special = [&bytes](std::uint32_t off, std::vector<std::string>& lines) -> std::uint32_t {
        if (off >= kHeaderBytes) return 0;
        for (const Field& f : kFields) {
            if (f.at != off) continue;
            const ByteView v = ByteView(bytes).subspan(f.at, f.length);
            if (all_printable(v)) {
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
    if (r.linked && r.built && program_size != 0 && !disc.first_read_name.empty()) {
        const Bytes built = fs::read_file(*r.built);
        const std::filesystem::path iso = folder / (stem + ".built.iso");
        fs::write_file(iso, minimal_disc(built, disc.first_read_name, disc.scrambled));
        r.built = iso;
    }
    return r;
}

iw::Written reconstruct(const std::filesystem::path& image_file, const std::filesystem::path& folder) {
    return write(fm::read(image_file), image_file.filename().string(), folder);
}

}  // namespace cb::source::dreamcast_writer
