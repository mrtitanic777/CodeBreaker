#include "formats/pce.hpp"

#include <bit>
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "formats/format_builtin.hpp"
#include "formats/probe.hpp"

namespace cb::formats::pce {

namespace {

constexpr std::string_view kReader = "pce";

std::uint16_t word_of(ByteView data, std::size_t at) { return static_cast<std::uint16_t>(data[at] | data[at + 1] << 8); }

// The header of the image, if its size says there is one.
std::size_t header_size_of(std::uint64_t size) {
    if (size % kBankSize == kHeaderSize && size > kHeaderSize) return kHeaderSize;
    return 0;
}

bool whole_banks(std::uint64_t size, std::size_t header) { return size > header && (size - header) % kBankSize == 0; }

}  // namespace

std::optional<std::size_t> Rom::rom_bank(std::uint32_t physical) const {
    const std::size_t count = bank_count();
    if (physical > 0x7F || count == 0) return std::nullopt;
    if (std::has_single_bit(count)) return physical & (count - 1);
    if (physical < count) return physical;
    return std::nullopt;
}

ByteView Rom::bank(std::size_t index) const { return ByteView(data).subspan(index * kBankSize, kBankSize); }

std::optional<std::uint8_t> Rom::at(std::uint32_t physical, std::uint32_t offset) const {
    const std::optional<std::size_t> index = rom_bank(physical);
    if (!index || offset >= kBankSize) return std::nullopt;
    return data[*index * kBankSize + offset];
}

Rom parse(ByteView data) {
    const engine::Tree tree = engine::read_builtin(kReader, data);   // config/formats/pce.fmt
    const engine::Node& d = tree.root;
    Rom rom;
    const ByteView copier = engine::bytes_of(tree, d.get("copier")), banks = engine::bytes_of(tree, d.get("banks"));
    rom.header.assign(copier.begin(), copier.end());
    rom.data.assign(banks.begin(), banks.end());
    const engine::Node& v = d.get("vectors");
    rom.vectors = {static_cast<std::uint16_t>(v.u("irq2")), static_cast<std::uint16_t>(v.u("irq1")), static_cast<std::uint16_t>(v.u("timer")),
                   static_cast<std::uint16_t>(v.u("nmi")), static_cast<std::uint16_t>(v.u("reset"))};
    return rom;
}

Rom read(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse(file.view());
}

bool probe(const std::filesystem::path& path) {
    if (!has_extension(path, {".pce", ".tg16", ".sgx"})) return false;
    const std::optional<std::uint64_t> size = size_of(path);
    if (!size) return false;
    const std::size_t header = header_size_of(*size);
    if (!whole_banks(*size, header)) return false;
    const std::optional<Bytes> first = read_prefix(path, header + kBankSize - 10, 10);
    if (!first || first->size() < 10) return false;
    return word_of(*first, 8) >= kResetBase;
}

const std::map<std::uint32_t, std::string>& registers_read() {
    static const std::map<std::uint32_t, std::string> table = {
        {0x0000, "VDC_STATUS"}, {0x0002, "VDC_DATA_L"}, {0x0003, "VDC_DATA_H"},
        {0x0402, "VCE_CTA_L"},  {0x0403, "VCE_CTA_H"},  {0x0404, "VCE_CTD_L"},  {0x0405, "VCE_CTD_H"},
        {0x0C00, "TIMER_COUNTER"}, {0x0C01, "TIMER_CTRL"}, {0x1000, "IO_PORT"}, {0x1402, "IRQ_DISABLE"}, {0x1403, "IRQ_STATUS"},
    };
    return table;
}

const std::map<std::uint32_t, std::string>& registers_write() {
    static const std::map<std::uint32_t, std::string> table = {
        {0x0000, "VDC_SELECT"}, {0x0002, "VDC_DATA_L"}, {0x0003, "VDC_DATA_H"},
        {0x0400, "VCE_CR"},     {0x0402, "VCE_CTA_L"},  {0x0403, "VCE_CTA_H"},  {0x0404, "VCE_CTD_L"},  {0x0405, "VCE_CTD_H"},
        {0x0800, "PSG_CH_SEL"}, {0x0801, "PSG_MAIN_VOL"}, {0x0802, "PSG_FREQ_L"}, {0x0803, "PSG_FREQ_H"}, {0x0804, "PSG_CH_CTRL"},
        {0x0805, "PSG_CH_PAN"}, {0x0806, "PSG_WAVE"},   {0x0807, "PSG_NOISE"},  {0x0808, "PSG_LFO_FREQ"}, {0x0809, "PSG_LFO_CTRL"},
        {0x0C00, "TIMER_RELOAD"}, {0x0C01, "TIMER_CTRL"}, {0x1000, "IO_PORT"}, {0x1402, "IRQ_DISABLE"}, {0x1403, "IRQ_ACK"},
    };
    return table;
}

std::string register_name(std::uint16_t offset, bool write) {
    std::uint32_t canonical = offset;
    if (offset < 0x0400) canonical = offset & 3;                    // VDC: four registers repeated
    else if (offset < 0x0800) canonical = 0x0400 | (offset & 7);    // VCE: eight
    else if (offset < 0x0C00) canonical = 0x0800 | (offset & 15);   // PSG: sixteen
    else if (offset < 0x1000) canonical = 0x0C00 | (offset & 1);    // timer: two
    else if (offset < 0x1400) canonical = 0x1000;                   // I/O: one
    else if (offset < 0x1800) canonical = 0x1400 | (offset & 3);    // interrupt control: four
    const auto& table = write ? registers_write() : registers_read();
    if (const auto it = table.find(canonical); it != table.end()) return it->second;
    return std::format("IO_{:04X}", offset);
}

std::string vdc_register_name(unsigned index) {
    static constexpr std::string_view kNames[] = {"MAWR", "MARR", "VWR", "R3", "R4", "CR", "RCR", "BXR", "BYR", "MWR", "HSR", "HDR", "VPR", "VDW", "VCR", "DCR", "SOUR", "DESR", "LENR", "SATB"};
    return index < std::size(kNames) ? std::string(kNames[index]) : std::format("R{}", index);
}

}  // namespace cb::formats::pce
