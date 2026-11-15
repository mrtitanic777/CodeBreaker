// cb-unity: alone
#include "cb/formats/ngp.hpp"

#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/formats/probe.hpp"

namespace cb::formats::ngp {

namespace {

constexpr std::string_view kReader = "ngp";

bool has_licence(ByteView d) {
    if (d.size() < kHeaderSize) return false;
    const std::string_view text = as_text(d.first(28));
    return text == "COPYRIGHT BY SNK CORPORATION" || text == " LICENSED BY SNK CORPORATION";
}

std::uint32_t le32(ByteView d, std::size_t at) { return d[at] | d[at + 1] << 8 | d[at + 2] << 16 | static_cast<std::uint32_t>(d[at + 3]) << 24; }

}  // namespace

const std::map<std::uint32_t, std::string>& hardware() {
    static const std::map<std::uint32_t, std::string> table = {
        {0x0020, "HW_TRUN"},      {0x0022, "HW_TREG0"},    {0x0023, "HW_TREG1"},     {0x0024, "HW_T01MOD"},   {0x0025, "HW_TFFCR"},    {0x0026, "HW_TREG2"},
        {0x0027, "HW_TREG3"},     {0x0028, "HW_T23MOD"},   {0x006F, "HW_WATCHDOG"},  {0x0073, "HW_ROM_BANK"}, {0x007C, "HW_DMA0V"},    {0x007D, "HW_DMA1V"},
        {0x007E, "HW_DMA2V"},     {0x007F, "HW_DMA3V"},
        {0x6F80, "BIOS_BAT_VOLT"}, {0x6F82, "BIOS_JOYPAD"}, {0x6F84, "BIOS_USR_BOOT"}, {0x6F85, "BIOS_USR_SHUTDOWN"}, {0x6F86, "BIOS_USR_ANSWER"},
        {0x6F87, "BIOS_LANGUAGE"}, {0x6F91, "BIOS_OS_VERSION"},
        {0x8000, "K1GE_CONTROL"}, {0x8002, "K1GE_TRANSPARENCY"}, {0x8008, "K1GE_SCR1_SCROLL_X"}, {0x8009, "K1GE_SCR1_SCROLL_Y"}, {0x800A, "K1GE_SCR2_SCROLL_X"},
        {0x800B, "K1GE_SCR2_SCROLL_Y"}, {0x8010, "K1GE_BG_COLOUR"}, {0x8012, "K1GE_WIN_X1"}, {0x8013, "K1GE_WIN_Y1"}, {0x8014, "K1GE_WIN_X2"}, {0x8015, "K1GE_WIN_Y2"},
        {0x8102, "K1GE_LED"},     {0x8800, "K1GE_SPRITE_VRAM"}, {0x8C00, "K1GE_SPRITE_PALETTE_INDEX"}, {0x9000, "K1GE_SCR1_MAP"}, {0x9800, "K1GE_SCR2_MAP"}, {0xA000, "K1GE_TILE_RAM"}};
    return table;
}

Rom parse(ByteView d) {
    if (d.size() < kHeaderSize || d.size() > kMaxSize) throw FormatError(kReader, 0, std::format("{} bytes is not a Neo Geo Pocket cartridge (64 bytes to 4 MB)", d.size()));
    if (!has_licence(d)) throw FormatError(kReader, 0, "the file does not start with the SNK licence text");
    Rom r;
    r.data.assign(d.begin(), d.end());
    Header& h = r.header;
    h.licensed = d[0] == ' ';
    h.start = le32(d, 0x1C);
    h.catalog = static_cast<std::uint16_t>(d[0x20] | d[0x21] << 8);
    h.sub_catalog = d[0x22];
    h.colour = d[0x23] == 0x10;
    std::string title(reinterpret_cast<const char*>(d.data() + 0x24), 12);
    while (!title.empty() && (title.back() == ' ' || title.back() == '\0')) title.pop_back();
    h.title = title;
    if (h.start < kRomBase || h.start - kRomBase >= d.size()) throw FormatError(kReader, 0x1C, std::format("the start address {:06X} is outside the ROM", h.start));
    return r;
}

Rom read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    if (!has_extension(path, {".ngp", ".ngc", ".npc"})) return false;
    const std::optional<Bytes> head = read_prefix(path, 0, kHeaderSize);
    return head && has_licence(*head);
}

}  // namespace cb::formats::ngp
