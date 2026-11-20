// cb-unity: alone
#include "formats/ngp.hpp"

#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "formats/format_builtin.hpp"
#include "formats/probe.hpp"

namespace cb::formats::ngp {

namespace {

constexpr std::string_view kReader = "ngp";

bool has_licence(ByteView d) {
    if (d.size() < kHeaderSize) return false;
    const std::string_view text = as_text(d.first(28));
    return text == "COPYRIGHT BY SNK CORPORATION" || text == " LICENSED BY SNK CORPORATION";
}

}  // namespace

const std::map<std::uint32_t, std::string>& hardware() {
    static const std::map<std::uint32_t, std::string> table = [] {
        std::map<std::uint32_t, std::string> t = {
            // the TLCS-900's own I/O: timers, watchdog, DMA vectors, the Z80 mailbox
            {0x0020, "HW_TRUN"},       {0x0022, "HW_TREG0"},     {0x0023, "HW_TREG1"},      {0x0024, "HW_T01MOD"},    {0x0025, "HW_TFFCR"},     {0x0026, "HW_TREG2"},
            {0x0027, "HW_TREG3"},      {0x0028, "HW_T23MOD"},    {0x006E, "HW_WDMOD"},      {0x006F, "HW_WATCHDOG"},  {0x0073, "HW_ROM_BANK"},  {0x007C, "HW_DMA0V"},
            {0x007D, "HW_DMA1V"},      {0x007E, "HW_DMA2V"},     {0x007F, "HW_DMA3V"},      {0x00B8, "HW_SOUNDCPU_CTRL"}, {0x00BA, "HW_Z80_NMI"}, {0x00BC, "HW_Z80_COMM"},
            // the BIOS's work RAM: the system variables and the interrupt vectors
            {0x6F80, "BIOS_BAT_VOLT"}, {0x6F82, "BIOS_JOYPAD"},  {0x6F84, "BIOS_USR_BOOT"}, {0x6F85, "BIOS_USR_SHUTDOWN"}, {0x6F86, "BIOS_USR_ANSWER"}, {0x6F87, "BIOS_LANGUAGE"},
            {0x6F91, "BIOS_OS_VERSION"},
            // the K2GE graphics chip
            {0x8000, "K2GE_DISP_CTL"}, {0x8002, "K2GE_WIN_X"},   {0x8003, "K2GE_WIN_Y"},    {0x8004, "K2GE_WIN_W"},   {0x8005, "K2GE_WIN_H"},    {0x8006, "K2GE_FRAME_RATE"},
            {0x8008, "K2GE_RAS_H"},    {0x8009, "K2GE_RAS_V"},   {0x8010, "K2GE_STATUS"},   {0x8012, "K2GE_LCD_CTL"}, {0x8020, "K2GE_PO_H"},     {0x8021, "K2GE_PO_V"},
            {0x8030, "K2GE_SCR_PRIO"}, {0x8032, "K2GE_SCR1_OFS_X"}, {0x8033, "K2GE_SCR1_OFS_Y"}, {0x8034, "K2GE_SCR2_OFS_X"}, {0x8035, "K2GE_SCR2_OFS_Y"}, {0x8118, "K2GE_BG_CTL"},
            {0x87E2, "K2GE_GE_MODE"},  {0x8200, "K2GE_PAL_SPR"}, {0x8280, "K2GE_PAL_SCR1"}, {0x8300, "K2GE_PAL_SCR2"}, {0x83E0, "K2GE_PAL_BG"}, {0x83F0, "K2GE_PAL_WIN"},
            {0x8800, "K2GE_SPR_VRAM"}, {0x8C00, "K2GE_SPR_PAL_IDX"}, {0x9000, "K2GE_SCR1_MAP"}, {0x9800, "K2GE_SCR2_MAP"}, {0xA000, "K2GE_TILE_RAM"}};
        // the interrupt vectors: 32-bit pointers the program stores for the BIOS to jump through
        const char* vectors[] = {"SWI3", "SWI4", "SWI5", "SWI6", "RTC", "VBL", "Z80", "TIM0", "TIM1", "TIM2", "TIM3", "SER_TX", "SER_RX"};
        for (unsigned k = 0; k < 13; ++k) t[0x6FB8 + 4 * k] = std::string("BIOS_INT_") + vectors[k];
        t[0x6FF0] = "BIOS_INT_DMA0";
        t[0x6FF4] = "BIOS_INT_DMA1";
        t[0x6FF8] = "BIOS_INT_DMA2";
        t[0x6FFC] = "BIOS_INT_DMA3";
        return t;
    }();
    return table;
}

Rom parse(ByteView d) {
    const engine::Tree tree = engine::read_builtin(kReader, d);   // config/formats/ngp.fmt
    const engine::Node& n = tree.root.get("header");
    Rom r;
    r.data.assign(d.begin(), d.end());
    Header& h = r.header;
    h.licensed = n.u("licensed") != 0;
    h.start = static_cast<std::uint32_t>(n.u("start"));
    h.catalog = static_cast<std::uint16_t>(n.u("catalog"));
    h.sub_catalog = static_cast<std::uint8_t>(n.u("sub_catalog"));
    h.colour = n.u("colour") != 0;
    std::string title = n.get("title").text;
    while (!title.empty() && (title.back() == ' ' || title.back() == '\0')) title.pop_back();
    h.title = title;
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
