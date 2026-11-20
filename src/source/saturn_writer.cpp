// source/saturn_writer.cpp -- a Sega Saturn disc's boot sectors and first program as assembly source (saturn_writer.hpp).

#include "source/saturn_writer.hpp"

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
    static const std::map<std::uint32_t, std::string> table = [] {
        std::map<std::uint32_t, std::string> t;
        const auto add = [&](std::uint32_t base, const char* prefix, std::initializer_list<std::pair<std::uint32_t, const char*>> regs) {
            for (const auto& [offset, name] : regs) t[base + offset] = std::string(prefix) + name;
        };
        // the SMPC: its registers are the odd bytes of $00100000-$0010007F
        for (unsigned i = 0; i < 7; ++i) t[0x00100001 + 2 * i] = std::format("SMPC_IREG{}", i);
        for (unsigned i = 0; i < 32; ++i) t[0x00100021 + 2 * i] = std::format("SMPC_OREG{}", i);
        t[0x0010001F] = "SMPC_COMREG";
        t[0x00100061] = "SMPC_SR";
        t[0x00100063] = "SMPC_SF";
        t[0x00100075] = "SMPC_PDR1";
        t[0x00100077] = "SMPC_PDR2";
        t[0x00100079] = "SMPC_DDR1";
        t[0x0010007B] = "SMPC_DDR2";
        t[0x0010007D] = "SMPC_IOSEL";
        t[0x0010007F] = "SMPC_EXLE";
        add(0x05FE0000, "SCU_", {{0x00, "D0R"}, {0x04, "D0W"}, {0x08, "D0C"}, {0x0C, "D0AD"}, {0x10, "D0EN"}, {0x14, "D0MD"}, {0x20, "D1R"},
                                 {0x24, "D1W"}, {0x28, "D1C"}, {0x2C, "D1AD"}, {0x30, "D1EN"}, {0x34, "D1MD"}, {0x40, "D2R"}, {0x44, "D2W"},
                                 {0x48, "D2C"}, {0x4C, "D2AD"}, {0x50, "D2EN"}, {0x54, "D2MD"}, {0x60, "DSTP"}, {0x7C, "DSTA"}, {0x80, "PPAF"},
                                 {0x84, "PPD"}, {0x88, "PDA"}, {0x8C, "PDD"}, {0x90, "T0C"}, {0x94, "T1S"}, {0x98, "T1MD"}, {0xA0, "IMS"},
                                 {0xA4, "IST"}, {0xA8, "AIACK"}, {0xB0, "ASR0"}, {0xB4, "ASR1"}, {0xB8, "AREF"}, {0xC4, "RSEL"}, {0xC8, "VER"}});
        add(0x05D00000, "VDP1_", {{0x00, "TVMR"}, {0x02, "FBCR"}, {0x04, "PTMR"}, {0x06, "EWDR"}, {0x08, "EWLR"}, {0x0A, "EWRR"}, {0x0C, "ENDR"},
                                  {0x10, "EDSR"}, {0x12, "LOPR"}, {0x14, "COPR"}, {0x16, "MODR"}});
        add(0x05F80000, "VDP2_", {{0x00, "TVMD"},   {0x02, "EXTEN"},  {0x04, "TVSTAT"}, {0x06, "VRSIZE"}, {0x08, "HCNT"},   {0x0A, "VCNT"},   {0x0E, "RAMCTL"},
                                  {0x10, "CYCA0L"}, {0x12, "CYCA0U"}, {0x14, "CYCA1L"}, {0x16, "CYCA1U"}, {0x18, "CYCB0L"}, {0x1A, "CYCB0U"}, {0x1C, "CYCB1L"},
                                  {0x1E, "CYCB1U"}, {0x20, "BGON"},   {0x22, "MZCTL"},  {0x24, "SFSEL"},  {0x26, "SFCODE"}, {0x28, "CHCTLA"}, {0x2A, "CHCTLB"},
                                  {0x2C, "BMPNA"},  {0x2E, "BMPNB"},  {0x30, "PNCN0"},  {0x32, "PNCN1"},  {0x34, "PNCN2"},  {0x36, "PNCN3"},  {0x38, "PNCR"},
                                  {0x3A, "PLSZ"},   {0x3C, "MPOFN"},  {0x3E, "MPOFR"},  {0x70, "SCXIN0"}, {0x72, "SCXDN0"}, {0x74, "SCYIN0"}, {0x76, "SCYDN0"},
                                  {0x78, "ZMXIN0"}, {0x7A, "ZMXDN0"}, {0x7C, "ZMYIN0"}, {0x7E, "ZMYDN0"}, {0x80, "SCXIN1"}, {0x82, "SCXDN1"}, {0x84, "SCYIN1"},
                                  {0x86, "SCYDN1"}, {0x88, "ZMXIN1"}, {0x8A, "ZMXDN1"}, {0x8C, "ZMYIN1"}, {0x8E, "ZMYDN1"}, {0x90, "SCXN2"},  {0x92, "SCYN2"},
                                  {0x94, "SCXN3"},  {0x96, "SCYN3"},  {0x98, "ZMCTL"},  {0x9A, "SCRCTL"}, {0xE0, "SPCTL"},  {0xE2, "SDCTL"},  {0xE4, "CRAOFA"},
                                  {0xE6, "CRAOFB"}, {0xE8, "LNCLEN"}, {0xEA, "SFPRMD"}, {0xEC, "CCCTL"},  {0xEE, "SFCCMD"}, {0xF0, "PRISA"},  {0xF2, "PRISB"},
                                  {0xF4, "PRISC"},  {0xF6, "PRISD"},  {0xF8, "PRINA"},  {0xFA, "PRINB"},  {0xFC, "PRIR"}});
        add(0x05890000, "CDB_", {{0x08, "HIRQ"}, {0x0C, "HIRQMSK"}, {0x18, "CR1"}, {0x1C, "CR2"}, {0x20, "CR3"}, {0x24, "CR4"}});
        add(0x05B00000, "SCSP_", {{0x400, "MVOL"}, {0x402, "RBL"}, {0x404, "MIDI_IN"}, {0x408, "MSLC"}});
        t[0x01000000] = "MINIT";
        t[0x01800000] = "SINIT";
        t[0x05818000] = "CDB_DATATRNS";
        t[0x05A00000] = "SCSP_RAM";
        t[0x05C00000] = "VDP1_VRAM";
        t[0x05C80000] = "VDP1_FRAMEBUFFER";
        t[0x05E00000] = "VDP2_VRAM";
        t[0x05F00000] = "VDP2_CRAM";
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
    image.head = {std::format("! {}.s -- written by CodeBreaker from {}", stem, base_name),
                  std::format("! Saturn disc \"{}\" ({}, areas {}): IP.BIN of {} bytes at 0x{:08X}, first program {} of {} bytes at 0x{:08X}",
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
                lines.push_back(std::format("        .long 0x{:08X}    ! {}", n, f.what));
            } else if (all_printable(v)) {
                lines.push_back(std::format("        .ascii \"{}\"    ! {}", as_text(v), f.what));
            } else if (std::ranges::all_of(v, [](std::uint8_t c) { return c == 0; })) {
                lines.push_back(std::format("        .space {}    ! {}", f.length, f.what));
            } else {
                std::vector<std::string> items;
                for (const std::uint8_t c : v) items.push_back(std::format("0x{:02X}", c));
                lines.push_back(std::format("        .byte {}    ! {}", text::join(items, ", "), f.what));
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
