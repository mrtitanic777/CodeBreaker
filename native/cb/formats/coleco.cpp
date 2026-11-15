#include "cb/formats/coleco.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/formats/probe.hpp"

namespace cb::formats::coleco {

namespace {

constexpr std::string_view kReader = "coleco";
constexpr std::size_t kVectors = 0x0C;       // the first vector's offset: RST $08
constexpr std::size_t kNmi = 0x21;
constexpr std::size_t kTitle = 0x24;

bool has_header(ByteView d) { return d.size() >= kTitle && ((d[0] == 0xAA && d[1] == 0x55) || (d[0] == 0x55 && d[1] == 0xAA)); }

// Does a vector hold something the BIOS may jump to: a JMP, a RET, RETI or RETN?
bool used(ByteView d, std::size_t at) {
    if (at + 3 > d.size()) return false;
    if (d[at] == 0xC3 || d[at] == 0xC9) return true;
    return d[at] == 0xED && (d[at + 1] == 0x4D || d[at + 1] == 0x45);
}

}  // namespace

Program parse(ByteView data) {
    if (data.size() > kMaxSize) {
        throw UnsupportedError(kReader, 0, std::format("a {} KB cartridge switches banks (a mapper), not read yet", data.size() / 1024));
    }
    if (!has_header(data)) throw FormatError(kReader, 0, "no ColecoVision cartridge header");
    io::ByteReader r(data, kReader);
    r.seek(2);
    Program p;
    p.header.title_screen = data[0] == 0xAA;
    p.header.sprite_names = r.u16();
    p.header.sprite_order = r.u16();
    p.header.work_buffer = r.u16();
    p.header.controllers = r.u16();
    p.header.start = r.u16();
    for (std::size_t i = kTitle; i < data.size() && i < kTitle + 0x40 && data[i] >= 0x20 && data[i] < 0x7F; ++i) p.header.title.push_back(static_cast<char>(data[i]));
    p.image.assign(data.begin(), data.end());
    const std::uint32_t end = kOrigin + static_cast<std::uint32_t>(data.size());
    if (p.header.start >= kOrigin + kTitle && p.header.start < end) p.entries.push_back({p.header.start, "start"});
    static constexpr const char* kNames[] = {"rst08", "rst10", "rst18", "rst20", "rst28", "rst30", "irq"};
    for (std::size_t i = 0; i < 7; ++i) {
        const std::size_t at = kVectors + 3 * i;
        if (used(data, at)) p.entries.push_back({static_cast<std::uint16_t>(kOrigin + at), kNames[i]});
    }
    if (used(data, kNmi)) p.entries.push_back({static_cast<std::uint16_t>(kOrigin + kNmi), "nmi"});
    return p;
}

Program read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    const bool col = has_extension(path, {".col"});
    if (!col && !has_extension(path, {".rom", ".bin"})) return false;
    const std::optional<std::uint64_t> size = size_of(path);
    if (!size || *size < kTitle || (!col && *size > kMaxSize)) return false;
    const std::optional<Bytes> head = read_prefix(path, 0, 2);
    return head && head->size() == 2 && ((*head)[0] == 0xAA || (*head)[0] == 0x55) && ((*head)[0] ^ (*head)[1]) == 0xFF;
}

const std::map<std::uint32_t, std::string>& bios_names() {
    static const std::map<std::uint32_t, std::string> kNames = {
        {0x1F61, "DO_SOUND"},       {0x1F6A, "FLIP_RL"},          {0x1F6D, "FLIP_UD"},         {0x1F70, "ROTATE"},          {0x1F73, "EXPAND"},
        {0x1F76, "READ_CTL_RAW"},   {0x1F79, "READ_CTL"},         {0x1F7C, "GAME_OPT"},        {0x1F7F, "INIT_FONT"},       {0x1F82, "FILL_VRAM"},
        {0x1F85, "INIT_VDP"},       {0x1F88, "READ_SPINNER"},     {0x1F8B, "P_BASE_LOAD"},     {0x1F8E, "P_BLK_READ_VRAM"}, {0x1F91, "P_BLK_WRT_VRAM"},
        {0x1F94, "P_INIT_RAM_SPRT"}, {0x1F97, "P_COPY_RAM_SPRT"}, {0x1F9A, "P_INIT_TIMERS"},   {0x1F9D, "P_STOP_TIMER"},    {0x1FA0, "P_START_TIMER"},
        {0x1FA3, "P_TEST_TIMER"},   {0x1FA6, "P_WRITE_REG"},      {0x1FA9, "P_WRT_VRAM"},      {0x1FAC, "P_READ_VRAM"},     {0x1FB2, "P_INIT_SOUND"},
        {0x1FB5, "P_ADD_SOUND"},    {0x1FB8, "BASE_LOAD"},        {0x1FBB, "BLK_READ_VRAM"},   {0x1FBE, "BLK_WRT_VRAM"},    {0x1FC1, "INIT_RAM_SPRT"},
        {0x1FC4, "COPY_RAM_SPRT"},  {0x1FC7, "INIT_TIMERS"},      {0x1FCA, "STOP_TIMER"},      {0x1FCD, "START_TIMER"},     {0x1FD0, "TEST_TIMER"},
        {0x1FD3, "RUN_TIMERS"},     {0x1FD6, "NO_SOUND"},         {0x1FD9, "WRITE_REG"},       {0x1FDC, "READ_REG"},        {0x1FDF, "WRT_VRAM"},
        {0x1FE2, "READ_VRAM"},      {0x1FEB, "READ_CTL_STATE"},   {0x1FEE, "INIT_SOUND"},      {0x1FF1, "ADD_SOUND"},      {0x1FF4, "UPDATE_SOUND"},
        {0x1FFD, "RANDOM"},         {0x73B9, "STACK"},            {0x73C3, "VDP0_SHAD"},       {0x73C4, "VDP1_SHAD"},       {0x73C8, "RAND_SEED"}};
    return kNames;
}

}  // namespace cb::formats::coleco
