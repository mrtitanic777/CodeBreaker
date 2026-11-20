// cb-unity: alone (file-local names that other files in a batch also use)
#include "formats/gekko_map.hpp"

#include <format>

namespace cb::formats::gekko_map {

namespace {

using ppc_program::Console;

struct Reg {
    std::uint32_t offset;
    const char* name;
};

// A register block: [base, base + size), its prefix for unnamed registers, and the registers it names.
struct Block {
    std::uint32_t base, size;
    const char* prefix;
    std::vector<Reg> regs;
};

// The GameCube's blocks (and the Wii's, which keeps them), at their uncached addresses.
const std::vector<Block>& gamecube_blocks() {
    static const std::vector<Block> blocks = {
        {0xCC000000, 0x1000, "CP",
         {{0x00, "CP_SR"},          {0x02, "CP_CR"},           {0x04, "CP_CLEAR"},        {0x20, "CP_FIFO_START_LO"}, {0x22, "CP_FIFO_START_HI"}, {0x24, "CP_FIFO_END_LO"},
          {0x26, "CP_FIFO_END_HI"}, {0x28, "CP_FIFO_HI_MARK_LO"}, {0x2A, "CP_FIFO_HI_MARK_HI"}, {0x2C, "CP_FIFO_LO_MARK_LO"}, {0x2E, "CP_FIFO_LO_MARK_HI"},
          {0x30, "CP_FIFO_COUNT_LO"}, {0x32, "CP_FIFO_COUNT_HI"}, {0x34, "CP_FIFO_WP_LO"},   {0x36, "CP_FIFO_WP_HI"},    {0x38, "CP_FIFO_RP_LO"},   {0x3A, "CP_FIFO_RP_HI"},
          {0x3C, "CP_FIFO_BP_LO"},  {0x3E, "CP_FIFO_BP_HI"}}},
        {0xCC001000, 0x1000, "PE",
         {{0x00, "PE_ZCONF"}, {0x02, "PE_ALPHACONF"}, {0x04, "PE_DSTALPHACONF"}, {0x06, "PE_ALPHAMODE"}, {0x08, "PE_ALPHAREAD"}, {0x0A, "PE_STATUS"}, {0x0E, "PE_TOKEN"}}},
        {0xCC002000, 0x1000, "VI",
         {{0x00, "VI_VTR"},  {0x02, "VI_DCR"},   {0x04, "VI_HTR0"},  {0x08, "VI_HTR1"},  {0x0C, "VI_VTO"},   {0x10, "VI_VTE"},   {0x14, "VI_BBEI"},  {0x18, "VI_BBOI"},
          {0x1C, "VI_TFBL"}, {0x20, "VI_TFBR"},  {0x24, "VI_BFBL"},  {0x28, "VI_BFBR"},  {0x2C, "VI_DPV"},   {0x2E, "VI_DPH"},   {0x30, "VI_DI0"},   {0x34, "VI_DI1"},
          {0x38, "VI_DI2"},  {0x3C, "VI_DI3"},   {0x48, "VI_HSW"},   {0x4A, "VI_HSR"},   {0x6C, "VI_CLK"},   {0x6E, "VI_SEL"},   {0x70, "VI_HBE"},   {0x72, "VI_HBS"}}},
        {0xCC003000, 0x1000, "PI",
         {{0x00, "PI_INTSR"}, {0x04, "PI_INTMSK"}, {0x0C, "PI_FIFO_BASE"}, {0x10, "PI_FIFO_END"}, {0x14, "PI_FIFO_WP"}, {0x24, "PI_RESET"}, {0x2C, "PI_CHIP_REV"}}},
        {0xCC004000, 0x1000, "MI", {{0x1C, "MI_INTMSK"}, {0x1E, "MI_INTSR"}, {0x20, "MI_UNKNOWN_20"}, {0x28, "MI_COUNTER_28"}}},
        {0xCC005000, 0x1000, "DSP",
         {{0x00, "DSP_CPU_MAILBOX_HI"}, {0x02, "DSP_CPU_MAILBOX_LO"}, {0x04, "DSP_MAILBOX_HI"}, {0x06, "DSP_MAILBOX_LO"}, {0x0A, "DSP_CSR"},
          {0x12, "AR_SIZE"},           {0x16, "AR_MODE"},            {0x1A, "AR_REFRESH"},     {0x20, "AR_DMA_MMADDR"}, {0x24, "AR_DMA_ARADDR"},
          {0x28, "AR_DMA_CNT"},        {0x30, "AI_DMA_START_HI"},    {0x32, "AI_DMA_START_LO"}, {0x36, "AI_DMA_LEN"},   {0x3A, "AI_DMA_BYTES_LEFT"}}},
        {0xCC006000, 0x400, "DI",
         {{0x00, "DI_STATUS"}, {0x04, "DI_COVER"}, {0x08, "DI_CMDBUF0"}, {0x0C, "DI_CMDBUF1"}, {0x10, "DI_CMDBUF2"}, {0x14, "DI_MAR"}, {0x18, "DI_LENGTH"},
          {0x1C, "DI_CR"},     {0x20, "DI_IMMBUF"}, {0x24, "DI_CFG"}}},
        {0xCC006400, 0x400, "SI",
         {{0x00, "SI_CH0_OUTBUF"}, {0x04, "SI_CH0_INBUF_H"}, {0x08, "SI_CH0_INBUF_L"}, {0x0C, "SI_CH1_OUTBUF"}, {0x10, "SI_CH1_INBUF_H"}, {0x14, "SI_CH1_INBUF_L"},
          {0x18, "SI_CH2_OUTBUF"}, {0x1C, "SI_CH2_INBUF_H"}, {0x20, "SI_CH2_INBUF_L"}, {0x24, "SI_CH3_OUTBUF"}, {0x28, "SI_CH3_INBUF_H"}, {0x2C, "SI_CH3_INBUF_L"},
          {0x30, "SI_POLL"},       {0x34, "SI_COMCSR"},      {0x38, "SI_SR"},          {0x3C, "SI_EXILK"},       {0x80, "SI_IOBUF"}}},
        {0xCC006800, 0x400, "EXI",
         {{0x00, "EXI0_CSR"}, {0x04, "EXI0_MAR"}, {0x08, "EXI0_LEN"}, {0x0C, "EXI0_CR"}, {0x10, "EXI0_DATA"}, {0x14, "EXI1_CSR"}, {0x18, "EXI1_MAR"}, {0x1C, "EXI1_LEN"},
          {0x20, "EXI1_CR"},  {0x24, "EXI1_DATA"}, {0x28, "EXI2_CSR"}, {0x2C, "EXI2_MAR"}, {0x30, "EXI2_LEN"}, {0x34, "EXI2_CR"},   {0x38, "EXI2_DATA"}}},
        {0xCC006C00, 0x400, "AI", {{0x00, "AI_CONTROL"}, {0x04, "AI_VOLUME"}, {0x08, "AI_SAMPLE_COUNTER"}, {0x0C, "AI_INT_TIMING"}}},
        {0xCC008000, 0x1000, "WGPIPE", {{0x00, "WGPIPE"}}},
    };
    return blocks;
}

// What the Wii adds: the mailbox with its ARM processor (IPC) and the Hollywood registers.
const std::vector<Block>& wii_blocks() {
    static const std::vector<Block> blocks = {
        {0xCD000000, 0x40, "IPC", {{0x00, "IPC_PPCMSG"}, {0x04, "IPC_PPCCTRL"}, {0x08, "IPC_ARMMSG"}, {0x0C, "IPC_ARMCTRL"}}},
        {0xCD800000, 0x1000, "HW",
         {{0x30, "HW_PPCIRQFLAG"}, {0x34, "HW_PPCIRQMASK"}, {0x38, "HW_ARMIRQFLAG"}, {0x3C, "HW_ARMIRQMASK"}, {0x60, "HW_MEMMIRR"}, {0x64, "HW_AHBPROT"},
          {0xC0, "HW_GPIOB_OUT"}, {0xC4, "HW_GPIOB_DIR"}, {0xC8, "HW_GPIOB_IN"}, {0xE0, "HW_GPIO_OUT"}, {0xE4, "HW_GPIO_DIR"}, {0xE8, "HW_GPIO_IN"},
          {0x194, "HW_RESETS"}}},
        {0xCD806000, 0x400, "DI", {}},
        {0xCD806400, 0x400, "SI", {}},
        {0xCD806800, 0x400, "EXI", {}},
        {0xCD806C00, 0x400, "AI", {}},
        {0xCD8B4000, 0x1000, "MEM", {}},
        {0xCD8B8000, 0x1000, "AHB", {}},
    };
    return blocks;
}

// What the Wii U's programs see of the Latte chip: a window named by offset.
const std::vector<Block>& wiiu_blocks() {
    static const std::vector<Block> blocks = {{0x0D000000, 0x01000000, "LATTE", {}}};
    return blocks;
}

// The cached physical view (0x0Cxxxxxx, 0x0Dxxxxxx) of a block is the same register as its uncached 0xCCxxxxxx, 0xCDxxxxxx view.
std::uint32_t uncached(std::uint32_t a) { return (a >> 24) == 0x0C || (a >> 24) == 0x0D ? a | 0xC0000000u : a; }

std::optional<std::string> find_in(const std::vector<Block>& blocks, std::uint32_t a) {
    for (const Block& b : blocks) {
        if (a < b.base || a - b.base >= b.size) continue;
        const std::uint32_t offset = a - b.base;
        for (const Reg& r : b.regs)
            if (r.offset == offset) return std::string(r.name);
        return std::format("{}_{:X}", b.prefix, offset);
    }
    return std::nullopt;
}

}  // namespace

std::optional<std::string> hardware_name(Console console, std::uint32_t address) {
    if (console == Console::WiiU) return find_in(wiiu_blocks(), address);
    const std::uint32_t a = uncached(address);
    if (console == Console::Wii) {
        if (auto n = find_in(wii_blocks(), a)) return n;
    }
    return find_in(gamecube_blocks(), a);
}

std::vector<Range> devices(Console console) {
    switch (console) {
        case Console::GameCube: return {{0xCC000000, 0xCC010000}};
        case Console::Wii: return {{0xCC000000, 0xCC010000}, {0xCD000000, 0xCD010000}, {0xCD800000, 0xCD900000}};
        case Console::WiiU: return {{0x0D000000, 0x0E000000}};
    }
    return {};
}

}  // namespace cb::formats::gekko_map
