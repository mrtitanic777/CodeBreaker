// cb-unity: alone (file-local names that other files in a batch also use)
// cb/source/dreamcast_writer.cpp -- a Sega Dreamcast disc's IP.BIN and first program as assembly source (dreamcast_writer.hpp).

#include "cb/source/dreamcast_writer.hpp"

#include <algorithm>
#include <format>

#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/source/isa_sh2.hpp"
#include "cb/source/sh2_writer.hpp"

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
    static const std::map<std::uint32_t, std::string> table = [] {
        std::map<std::uint32_t, std::string> t;
        const auto add = [&](std::uint32_t base, std::initializer_list<std::pair<std::uint32_t, const char*>> regs) {
            for (const auto& [offset, name] : regs) t[base + offset] = name;
        };
        t[0x00000000] = "BIOS_ROM";
        t[0x00200000] = "FLASH_ROM";
        t[0x00600000] = "MODEM";
        t[0x00800000] = "AICA_WAVERAM";
        t[0x01000000] = "G2_EXTERNAL";
        t[0x04000000] = "PVR_VRAM64";
        t[0x05000000] = "PVR_VRAM32";
        t[0x10000000] = "TA_FIFO_POLY";
        t[0x10800000] = "TA_FIFO_YUV";
        t[0x11000000] = "TA_FIFO_TEXTURE";
        t[0xE0000000] = "SH4_SQ0";
        t[0xE0000020] = "SH4_SQ1";

        // the Holly's system ASIC, the DMA to the PowerVR, the interrupt controller
        add(0x005F6800, {{0x00, "SB_C2DSTAT"}, {0x04, "SB_C2DLEN"}, {0x08, "SB_C2DST"}, {0x10, "SB_SDSTAW"}, {0x14, "SB_SDBAAW"}, {0x18, "SB_SDWLT"},
                         {0x1C, "SB_SDLAS"}, {0x20, "SB_SDST"}, {0x30, "SB_DBREQM"}, {0x34, "SB_BAVLWC"}, {0x38, "SB_C2DPRYC"}, {0x3C, "SB_C2DMAXL"},
                         {0x40, "SB_TFREM"}, {0x44, "SB_LMMODE0"}, {0x48, "SB_LMMODE1"}, {0x4C, "SB_FFST"}, {0x50, "SB_SFRES"}, {0x80, "SB_SBREV"},
                         {0x84, "SB_RBSPLT"}, {0x100, "SB_ISTNRM"}, {0x104, "SB_ISTEXT"}, {0x108, "SB_ISTERR"}, {0x110, "SB_IML2NRM"},
                         {0x114, "SB_IML2EXT"}, {0x118, "SB_IML2ERR"}, {0x120, "SB_IML4NRM"}, {0x124, "SB_IML4EXT"}, {0x128, "SB_IML4ERR"},
                         {0x130, "SB_IML6NRM"}, {0x134, "SB_IML6EXT"}, {0x138, "SB_IML6ERR"}, {0x140, "SB_PDTNRM"}, {0x144, "SB_PDTEXT"},
                         {0x150, "SB_G2DTNRM"}, {0x154, "SB_G2DTEXT"}});
        // the Maple bus
        add(0x005F6C00, {{0x04, "SB_MDSTAR"}, {0x10, "SB_MDTSEL"}, {0x14, "SB_MDEN"}, {0x18, "SB_MDST"}, {0x80, "SB_MSYS"}, {0x84, "SB_MST"},
                         {0x88, "SB_MSHTCL"}, {0x8C, "SB_MDAPRO"}, {0xE8, "SB_MMSEL"}, {0xF4, "SB_MTXDAD"}, {0xF8, "SB_MRXDAD"}, {0xFC, "SB_MRXDBD"}});
        // the G1 bus (the GD-ROM drive's DMA and timing) and the drive's ATA registers
        add(0x005F7000, {{0x18, "GD_ALTSTATUS"}, {0x80, "GD_DATA"}, {0x84, "GD_ERROR"}, {0x88, "GD_INTREASON"}, {0x8C, "GD_SECTNUM"},
                         {0x90, "GD_BYCTLLO"}, {0x94, "GD_BYCTLHI"}, {0x98, "GD_DRVSEL"}, {0x9C, "GD_STATUS"}, {0x404, "SB_GDSTAR"},
                         {0x408, "SB_GDLEN"}, {0x40C, "SB_GDDIR"}, {0x414, "SB_GDEN"}, {0x418, "SB_GDST"}, {0x480, "SB_G1RRC"}, {0x484, "SB_G1RWC"},
                         {0x488, "SB_G1FRC"}, {0x48C, "SB_G1FWC"}, {0x490, "SB_G1CRC"}, {0x494, "SB_G1CWC"}, {0x4A0, "SB_G1GDRC"},
                         {0x4A4, "SB_G1GDWC"}, {0x4B0, "SB_G1SYSM"}, {0x4B4, "SB_G1CRDYC"}, {0x4B8, "SB_GDAPRO"}, {0x4F4, "SB_GDSTARD"},
                         {0x4F8, "SB_GDLEND"}});
        // the G2 bus: four DMA channels (the AICA, two external, the development port) and its timing
        static constexpr const char* kChannels[] = {"AD", "E1", "E2", "DD"};
        for (unsigned c = 0; c < 4; ++c) {
            static constexpr const char* kSuffixes[] = {"STAG", "STAR", "LEN", "DIR", "TSEL", "EN", "ST", "SUSP"};
            for (unsigned k = 0; k < 8; ++k) t[0x005F7800 + 0x20 * c + 4 * k] = std::string("SB_") + kChannels[c] + kSuffixes[k];
        }
        add(0x005F7800, {{0x80, "SB_G2ID"}, {0x90, "SB_G2DSTO"}, {0x94, "SB_G2TRTO"}, {0x98, "SB_G2MDMTO"}, {0x9C, "SB_G2MDMW"}, {0xBC, "SB_G2APRO"}});
        // the PowerVR2: the core, the display, the tile accelerator
        add(0x005F8000, {{0x00, "PVR_ID"}, {0x04, "PVR_REVISION"}, {0x08, "PVR_SOFTRESET"}, {0x14, "PVR_STARTRENDER"}, {0x20, "PVR_PARAM_BASE"},
                         {0x2C, "PVR_REGION_BASE"}, {0x30, "PVR_SPAN_SORT_CFG"}, {0x40, "PVR_VO_BORDER_COL"}, {0x44, "PVR_FB_R_CTRL"},
                         {0x48, "PVR_FB_W_CTRL"}, {0x4C, "PVR_FB_W_LINESTRIDE"}, {0x50, "PVR_FB_R_SOF1"}, {0x54, "PVR_FB_R_SOF2"},
                         {0x5C, "PVR_FB_R_SIZE"}, {0x60, "PVR_FB_W_SOF1"}, {0x64, "PVR_FB_W_SOF2"}, {0x68, "PVR_FB_X_CLIP"}, {0x6C, "PVR_FB_Y_CLIP"},
                         {0x74, "PVR_FPU_SHAD_SCALE"}, {0x78, "PVR_FPU_CULL_VAL"}, {0x7C, "PVR_FPU_PARAM_CFG"}, {0x80, "PVR_HALF_OFFSET"},
                         {0x84, "PVR_FPU_PERP_VAL"}, {0x88, "PVR_ISP_BACKGND_D"}, {0x8C, "PVR_ISP_BACKGND_T"}, {0x98, "PVR_ISP_FEED_CFG"},
                         {0xA0, "PVR_SDRAM_REFRESH"}, {0xA4, "PVR_SDRAM_ARB_CFG"}, {0xA8, "PVR_SDRAM_CFG"}, {0xB0, "PVR_FOG_COL_RAM"},
                         {0xB4, "PVR_FOG_COL_VERT"}, {0xB8, "PVR_FOG_DENSITY"}, {0xBC, "PVR_FOG_CLAMP_MAX"}, {0xC0, "PVR_FOG_CLAMP_MIN"},
                         {0xC4, "PVR_SPG_TRIGGER_POS"}, {0xC8, "PVR_SPG_HBLANK_INT"}, {0xCC, "PVR_SPG_VBLANK_INT"}, {0xD0, "PVR_SPG_CONTROL"},
                         {0xD4, "PVR_SPG_HBLANK"}, {0xD8, "PVR_SPG_LOAD"}, {0xDC, "PVR_SPG_VBLANK"}, {0xE0, "PVR_SPG_WIDTH"},
                         {0xE4, "PVR_TEXT_CONTROL"}, {0xE8, "PVR_VO_CONTROL"}, {0xEC, "PVR_VO_STARTX"}, {0xF0, "PVR_VO_STARTY"},
                         {0xF4, "PVR_SCALER_CTL"}, {0x108, "PVR_PAL_RAM_CTRL"}, {0x10C, "PVR_SPG_STATUS"}, {0x110, "PVR_FB_BURSTCTRL"},
                         {0x114, "PVR_FB_C_SOF"}, {0x118, "PVR_Y_COEFF"}, {0x11C, "PVR_PT_ALPHA_REF"}, {0x124, "PVR_TA_OL_BASE"},
                         {0x128, "PVR_TA_ISP_BASE"}, {0x12C, "PVR_TA_OL_LIMIT"}, {0x130, "PVR_TA_ISP_LIMIT"}, {0x134, "PVR_TA_NEXT_OPB"},
                         {0x138, "PVR_TA_ITP_CURRENT"}, {0x13C, "PVR_TA_GLOB_TILE_CLIP"}, {0x140, "PVR_TA_ALLOC_CTRL"}, {0x144, "PVR_TA_LIST_INIT"},
                         {0x148, "PVR_TA_YUV_TEX_BASE"}, {0x14C, "PVR_TA_YUV_TEX_CTRL"}, {0x150, "PVR_TA_YUV_TEX_CNT"}, {0x160, "PVR_TA_LIST_CONT"},
                         {0x164, "PVR_TA_NEXT_OPB_INIT"}, {0x200, "PVR_FOG_TABLE"}, {0x1000, "PVR_PALETTE_RAM"}});
        // the AICA sound processor: 64 channels of 0x80 bytes, then its common registers, and the real-time clock
        for (unsigned ch = 0; ch < 64; ++ch)
            for (unsigned reg = 0; reg < 0x50; reg += 4) t[0x00700000 + 0x80 * ch + reg] = std::format("AICA_CH{}_{:02X}", ch, reg);
        add(0x00702800, {{0x00, "AICA_MASTER_VOL"}, {0x04, "AICA_RING_BUFFER"}, {0x08, "AICA_MIDI_IN"}, {0x10, "AICA_CHANNEL_INFO_REQ"},
                         {0x14, "AICA_PLAY_STATUS"}, {0x18, "AICA_PLAY_POS"}, {0x80, "AICA_TIMER_A"}, {0x84, "AICA_TIMER_B"}, {0x88, "AICA_TIMER_C"},
                         {0x400, "AICA_ARM_RESET"}, {0x500, "AICA_INT_STATUS_SH4"}, {0x504, "AICA_INT_CLEAR_SH4"}, {0x508, "AICA_INT_ENABLE_SH4"}});
        add(0x00710000, {{0x00, "RTC_DATA_H"}, {0x04, "RTC_DATA_L"}, {0x08, "RTC_WRITE_ENABLE"}});
        // the SH7750's own: the cache, MMU and exception registers, the bus state controller, the DMAC, clocks, interrupts, timers, serial ports
        add(0xFF000000, {{0x00, "PTEH"}, {0x04, "PTEL"}, {0x08, "TTB"}, {0x0C, "TEA"}, {0x10, "MMUCR"}, {0x14, "BASRA"}, {0x18, "BASRB"},
                         {0x1C, "CCR"}, {0x20, "TRA"}, {0x24, "EXPEVT"}, {0x28, "INTEVT"}, {0x34, "PTEA"}, {0x38, "QACR0"}, {0x3C, "QACR1"}});
        add(0xFF200000, {{0x00, "UBC_BARA"}, {0x04, "UBC_BAMRA"}, {0x08, "UBC_BBRA"}, {0x0C, "UBC_BARB"}, {0x10, "UBC_BAMRB"}, {0x14, "UBC_BDRB"},
                         {0x18, "UBC_BDMRB"}, {0x1C, "UBC_BRCR"}});
        add(0xFF800000, {{0x00, "BSC_BCR1"}, {0x04, "BSC_BCR2"}, {0x08, "BSC_WCR1"}, {0x0C, "BSC_WCR2"}, {0x10, "BSC_WCR3"}, {0x14, "BSC_MCR"},
                         {0x18, "BSC_PCR"}, {0x1C, "BSC_RTCSR"}, {0x20, "BSC_RTCNT"}, {0x24, "BSC_RTCOR"}, {0x28, "BSC_RFCR"}, {0x2C, "BSC_PCTRA"},
                         {0x30, "BSC_PDTRA"}, {0x40, "BSC_PCTRB"}, {0x44, "BSC_PDTRB"}, {0x48, "BSC_GPIOIC"}});
        add(0xFFA00000, {{0x00, "DMAC_SAR0"}, {0x04, "DMAC_DAR0"}, {0x08, "DMAC_DMATCR0"}, {0x0C, "DMAC_CHCR0"}, {0x10, "DMAC_SAR1"}, {0x14, "DMAC_DAR1"},
                         {0x18, "DMAC_DMATCR1"}, {0x1C, "DMAC_CHCR1"}, {0x20, "DMAC_SAR2"}, {0x24, "DMAC_DAR2"}, {0x28, "DMAC_DMATCR2"},
                         {0x2C, "DMAC_CHCR2"}, {0x30, "DMAC_SAR3"}, {0x34, "DMAC_DAR3"}, {0x38, "DMAC_DMATCR3"}, {0x3C, "DMAC_CHCR3"},
                         {0x40, "DMAC_DMAOR"}});
        add(0xFFC00000, {{0x00, "CPG_FRQCR"}, {0x04, "CPG_STBCR"}, {0x08, "CPG_WTCNT"}, {0x0C, "CPG_WTCSR"}, {0x10, "CPG_STBCR2"}});
        add(0xFFC80000, {{0x00, "RTC_R64CNT"}, {0x04, "RTC_RSECCNT"}, {0x08, "RTC_RMINCNT"}, {0x0C, "RTC_RHRCNT"}, {0x10, "RTC_RWKCNT"},
                         {0x14, "RTC_RDAYCNT"}, {0x18, "RTC_RMONCNT"}, {0x1C, "RTC_RYRCNT"}, {0x40, "RTC_RCR1"}, {0x44, "RTC_RCR2"}});
        add(0xFFD00000, {{0x00, "INTC_ICR"}, {0x04, "INTC_IPRA"}, {0x08, "INTC_IPRB"}, {0x0C, "INTC_IPRC"}});
        add(0xFFD80000, {{0x00, "TMU_TOCR"}, {0x04, "TMU_TSTR"}, {0x08, "TMU_TCOR0"}, {0x0C, "TMU_TCNT0"}, {0x10, "TMU_TCR0"}, {0x14, "TMU_TCOR1"},
                         {0x18, "TMU_TCNT1"}, {0x1C, "TMU_TCR1"}, {0x20, "TMU_TCOR2"}, {0x24, "TMU_TCNT2"}, {0x28, "TMU_TCR2"}, {0x2C, "TMU_TCPR2"}});
        add(0xFFE00000, {{0x00, "SCI_SCSMR1"}, {0x04, "SCI_SCBRR1"}, {0x08, "SCI_SCSCR1"}, {0x0C, "SCI_SCTDR1"}, {0x10, "SCI_SCSSR1"},
                         {0x14, "SCI_SCRDR1"}, {0x18, "SCI_SCSCMR1"}, {0x1C, "SCI_SCSPTR1"}});
        add(0xFFE80000, {{0x00, "SCIF_SCSMR2"}, {0x04, "SCIF_SCBRR2"}, {0x08, "SCIF_SCSCR2"}, {0x0C, "SCIF_SCFTDR2"}, {0x10, "SCIF_SCFSR2"},
                         {0x14, "SCIF_SCFRDR2"}, {0x18, "SCIF_SCFCR2"}, {0x1C, "SCIF_SCFDR2"}, {0x20, "SCIF_SCSPTR2"}, {0x24, "SCIF_SCLSR2"}});
        return t;
    }();
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
    image.head = {std::format("! {}.s -- written by CodeBreaker from {}", stem, base_name),
                  std::format("! Dreamcast disc \"{}\" ({}, areas {}): IP.BIN of {} bytes at 0x{:08X} (cached view 0x{:08X}), {} of {} bytes at 0x{:08X}{}", disc.header.title,
                              disc.header.product, disc.header.areas, system_size, fm::kIpBase, fm::kIpBase + fm::kCachedView,
                              disc.first_read_name.empty() ? std::string("(no first program)") : disc.first_read_name, program_size, fm::kFirstReadBase,
                              disc.scrambled ? ", scrambled on the disc and written here descrambled" : ""),
                  "! (addresses are physical: a program's pointer 0x8Cxxxxxx is written `label + 0x80000000`)"};
    // IP.BIN's header, field by field
    const Bytes& bytes = image.bytes;
    image.special = [&bytes](std::uint32_t off, std::vector<std::string>& lines) -> std::uint32_t {
        if (off >= kHeaderBytes) return 0;
        for (const Field& f : kFields) {
            if (f.at != off) continue;
            const ByteView v = ByteView(bytes).subspan(f.at, f.length);
            if (all_printable(v)) {
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
