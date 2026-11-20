// cb-unity: alone (file-local names that other files in a batch also use)
#include "formats/ps2.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <array>
#include <map>
#include <unordered_map>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "formats/iso9660.hpp"
#include "formats/probe.hpp"

namespace cb::formats::ps2 {

namespace {

std::string upper(std::string_view s) {
    std::string out(s);
    for (char& c : out) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return out;
}

bool is_r5900_elf(ByteView head) {
    return mips_elf::probe_header(head) && head.size() >= 40 && mips_elf::machine_of(static_cast<std::uint32_t>(head[36]) | static_cast<std::uint32_t>(head[37]) << 8 |
                                                                                    static_cast<std::uint32_t>(head[38]) << 16 | static_cast<std::uint32_t>(head[39]) << 24) == mips_elf::kMach5900;
}

// The registers of the Emotion Engine's units by physical address.
const std::map<std::uint32_t, std::string>& registers() {
    static const std::map<std::uint32_t, std::string> table = [] {
        std::map<std::uint32_t, std::string> t;
        for (unsigned n = 0; n < 4; ++n) {   // the four timers
            const std::uint32_t base = 0x10000000 + n * 0x800;
            t[base] = std::format("T{}_COUNT", n);
            t[base + 0x10] = std::format("T{}_MODE", n);
            t[base + 0x20] = std::format("T{}_COMP", n);
            if (n < 2) t[base + 0x30] = std::format("T{}_HOLD", n);
        }
        t[0x10002000] = "IPU_CMD";   t[0x10002010] = "IPU_CTRL";  t[0x10002020] = "IPU_BP";    t[0x10002030] = "IPU_TOP";
        const char* gif[] = {"GIF_CTRL", "GIF_MODE", "GIF_STAT"};
        for (unsigned i = 0; i < 3; ++i) t[0x10003000 + i * 0x10] = gif[i];
        t[0x10003040] = "GIF_TAG0";  t[0x10003050] = "GIF_TAG1";  t[0x10003060] = "GIF_TAG2";  t[0x10003070] = "GIF_TAG3";
        t[0x10003080] = "GIF_CNT";   t[0x10003090] = "GIF_P3CNT"; t[0x100030A0] = "GIF_P3TAG";
        const char* vif[] = {"STAT", "FBRST", "ERR", "MARK", "CYCLE", "MODE", "NUM", "MASK", "CODE", "ITOPS"};
        for (unsigned n = 0; n < 2; ++n) {
            const std::uint32_t base = 0x10003800 + n * 0x400;
            for (unsigned i = 0; i < 10; ++i) t[base + 0x00 + i * 0x10] = std::format("VIF{}_{}", n, vif[i]);
            if (n == 1) {
                t[base + 0xA0] = "VIF1_BASE"; t[base + 0xB0] = "VIF1_OFST"; t[base + 0xC0] = "VIF1_TOPS"; t[base + 0xD0] = "VIF1_ITOP"; t[base + 0xE0] = "VIF1_TOP";
            } else {
                t[base + 0xD0] = "VIF0_ITOP";
            }
            for (unsigned i = 0; i < 4; ++i) {
                t[base + 0x100 + i * 0x10] = std::format("VIF{}_R{}", n, i);
                t[base + 0x200 + i * 0x10] = std::format("VIF{}_C{}", n, i);
            }
        }
        t[0x10004000] = "VIF0_FIFO"; t[0x10005000] = "VIF1_FIFO"; t[0x10006000] = "GIF_FIFO"; t[0x10007000] = "IPU_OUT_FIFO"; t[0x10007010] = "IPU_IN_FIFO";
        constexpr std::uint32_t channels[10] = {0x10008000, 0x10009000, 0x1000A000, 0x1000B000, 0x1000B400, 0x1000C000, 0x1000C400, 0x1000C800, 0x1000D000, 0x1000D400};
        const std::pair<unsigned, const char*> dma_regs[] = {{0x00, "CHCR"}, {0x10, "MADR"}, {0x20, "QWC"}, {0x30, "TADR"}, {0x40, "ASR0"}, {0x50, "ASR1"}, {0x80, "SADR"}};
        for (unsigned n = 0; n < 10; ++n)
            for (const auto& [off, name] : dma_regs) t[channels[n] + off] = std::format("D{}_{}", n, name);
        t[0x1000E000] = "D_CTRL";  t[0x1000E010] = "D_STAT";  t[0x1000E020] = "D_PCR";   t[0x1000E030] = "D_SQWC";
        t[0x1000E040] = "D_RBSR";  t[0x1000E050] = "D_RBOR";  t[0x1000E060] = "D_STADR"; t[0x1000F520] = "D_ENABLER"; t[0x1000F590] = "D_ENABLEW";
        t[0x1000F000] = "I_STAT";  t[0x1000F010] = "I_MASK";
        t[0x1000F100] = "SIO_LCR"; t[0x1000F110] = "SIO_LSR"; t[0x1000F120] = "SIO_IER"; t[0x1000F130] = "SIO_ISR"; t[0x1000F140] = "SIO_FCR";
        t[0x1000F150] = "SIO_BGR"; t[0x1000F180] = "SIO_TXFIFO"; t[0x1000F1C0] = "SIO_RXFIFO";
        t[0x1000F200] = "SB_MSCOM"; t[0x1000F210] = "SB_SMCOM"; t[0x1000F220] = "SB_MSFLG"; t[0x1000F230] = "SB_SMFLG"; t[0x1000F240] = "SB_CTRL";
        t[0x1000F260] = "SB_?";    t[0x1000F430] = "MCH_RICM"; t[0x1000F440] = "MCH_DRD";
        t.erase(0x1000F260);
        const char* gs[] = {"GS_PMODE", "GS_SMODE1", "GS_SMODE2", "GS_SRFSH", "GS_SYNCH1", "GS_SYNCH2", "GS_SYNCV", "GS_DISPFB1", "GS_DISPLAY1",
                            "GS_DISPFB2", "GS_DISPLAY2", "GS_EXTBUF", "GS_EXTDATA", "GS_EXTWRITE", "GS_BGCOLOR"};
        for (unsigned i = 0; i < 15; ++i) t[0x12000000 + i * 0x10] = gs[i];
        t[0x12001000] = "GS_CSR"; t[0x12001010] = "GS_IMR"; t[0x12001040] = "GS_BUSDIR"; t[0x12001080] = "GS_SIGLBLID";
        return t;
    }();
    return table;
}

}  // namespace

std::optional<std::string> boot_file(std::string_view system_cnf, std::string* boot_value) {
    for (const std::string_view line : text::lines(system_cnf)) {
        const std::string_view t = text::trim(line);
        if (!upper(t.substr(0, 5)).starts_with("BOOT2")) continue;
        const std::size_t eq = t.find('=');
        if (eq == std::string_view::npos) continue;
        std::string value(text::trim(t.substr(eq + 1)));
        if (boot_value) *boot_value = value;
        std::string path = value;
        if (const std::size_t colon = path.find(':'); colon != std::string::npos) path = path.substr(colon + 1);
        if (const std::size_t semi = path.find(';'); semi != std::string::npos) path = path.substr(0, semi);
        for (char& c : path)
            if (c == '\\') c = '/';
        if (path.empty()) return std::nullopt;
        if (path.front() != '/') path.insert(path.begin(), '/');
        return path;
    }
    return std::nullopt;
}

Program parse_elf(ByteView data) {
    if (!is_r5900_elf(data)) throw FormatError("ps2", 0, "not a PlayStation 2 executable (a 32-bit little-endian MIPS ELF with the R5900 flags)");
    Program p;
    p.kind = Kind::Elf;
    p.elf = mips_elf::parse(data);
    return p;
}

std::optional<Program> read(const std::filesystem::path& path) {
    try {
        const std::string ext = text::lower(path.extension().string());
        const std::optional<Bytes> head = read_prefix(path, 0, 64);
        if (!head) return std::nullopt;
        if (is_r5900_elf(*head)) return parse_elf(fs::read_file(path));
        if (ext != ".iso" && ext != ".bin" && ext != ".img") return std::nullopt;
        iso9660::Volume volume(path);
        const Bytes cnf = volume.read("/SYSTEM.CNF");
        std::string boot;
        const std::optional<std::string> file = boot_file(as_text(cnf), &boot);
        if (!file) return std::nullopt;
        const Bytes elf = volume.read(*file);
        if (!is_r5900_elf(elf)) return std::nullopt;
        Program p = parse_elf(elf);
        p.kind = Kind::Disc;
        p.boot = boot;
        p.boot_file = *file;
        return p;
    } catch (const Error&) {
        return std::nullopt;
    }
}

bool probe(const std::filesystem::path& path) { return read(path).has_value(); }

std::optional<std::uint32_t> register_address(std::uint32_t address) {
    std::uint32_t p = address;
    if (p >= 0x80000000u && p < 0xC0000000u) p &= 0x1FFFFFFFu;
    if ((p >= 0x10000000u && p < 0x20000000u) || (p >= 0x12000000u && p < 0x12002000u)) return p;
    return std::nullopt;
}

std::optional<std::string> hardware_name(std::uint32_t address) {
    const std::optional<std::uint32_t> p = register_address(address);
    if (!p) return std::nullopt;
    const auto& table = registers();
    if (const auto it = table.find(*p); it != table.end()) return it->second;
    // a register of an unnamed offset inside a named unit's block: the block and the offset
    const struct {
        std::uint32_t lo, hi;
        const char* block;
    } blocks[] = {{0x10000000, 0x10001FFF, "TIMERS"}, {0x10002000, 0x10002FFF, "IPU"}, {0x10003000, 0x100037FF, "GIF"}, {0x10003800, 0x10003BFF, "VIF0"},
                  {0x10003C00, 0x10003FFF, "VIF1"},   {0x10004000, 0x10007FFF, "FIFO"}, {0x10008000, 0x1000EFFF, "DMAC"}, {0x1000F000, 0x1000FFFF, "SYS"},
                  {0x12000000, 0x12001FFF, "GS"}};
    for (const auto& b : blocks)
        if (*p >= b.lo && *p <= b.hi) return std::format("{}_{:04X}", b.block, *p - b.lo);
    return std::nullopt;
}

std::string syscall_name(std::int32_t number) {
    static const std::unordered_map<std::int32_t, std::string_view> names = {
        {0x01, "ResetEE"}, {0x02, "SetGsCrt"}, {0x04, "Exit"}, {0x06, "LoadExecPS2"}, {0x07, "ExecPS2"}, {0x10, "AddIntcHandler"},
        {0x11, "RemoveIntcHandler"}, {0x12, "AddDmacHandler"}, {0x13, "RemoveDmacHandler"}, {0x14, "EnableIntc"}, {0x15, "DisableIntc"},
        {0x16, "EnableDmac"}, {0x17, "DisableDmac"}, {0x18, "SetAlarm"}, {0x19, "ReleaseAlarm"}, {0x1A, "iEnableIntc"}, {0x1B, "iDisableIntc"},
        {0x1C, "iEnableDmac"}, {0x1D, "iDisableDmac"}, {0x1E, "iSetAlarm"}, {0x1F, "iReleaseAlarm"}, {0x20, "CreateThread"}, {0x21, "DeleteThread"},
        {0x22, "StartThread"}, {0x23, "ExitThread"}, {0x24, "ExitDeleteThread"}, {0x25, "TerminateThread"}, {0x26, "iTerminateThread"},
        {0x29, "ChangeThreadPriority"}, {0x2A, "iChangeThreadPriority"}, {0x2B, "RotateThreadReadyQueue"}, {0x2C, "iRotateThreadReadyQueue"},
        {0x2D, "ReleaseWaitThread"}, {0x2E, "iReleaseWaitThread"}, {0x2F, "GetThreadId"}, {0x30, "ReferThreadStatus"}, {0x31, "iReferThreadStatus"},
        {0x32, "SleepThread"}, {0x33, "WakeupThread"}, {0x34, "iWakeupThread"}, {0x35, "CancelWakeupThread"}, {0x36, "iCancelWakeupThread"},
        {0x37, "SuspendThread"}, {0x38, "iSuspendThread"}, {0x39, "ResumeThread"}, {0x3A, "iResumeThread"}, {0x3C, "SetupThread"}, {0x3D, "SetupHeap"},
        {0x3E, "EndOfHeap"}, {0x40, "CreateSema"}, {0x41, "DeleteSema"}, {0x42, "SignalSema"}, {0x43, "iSignalSema"}, {0x44, "WaitSema"},
        {0x45, "PollSema"}, {0x46, "iPollSema"}, {0x47, "ReferSemaStatus"}, {0x48, "iReferSemaStatus"}, {0x64, "FlushCache"}, {0x70, "GsGetIMR"},
        {0x71, "GsPutIMR"}, {0x73, "SetVSyncFlag"}, {0x74, "SetSyscall"}, {0x76, "SifDmaStat"}, {0x77, "iSifDmaStat"}, {0x78, "SifSetDma"},
        {0x79, "iSifSetDma"}, {0x7A, "SifSetDChain"}, {0x7B, "iSifSetDChain"}, {0x7C, "Deci2Call"}, {0x7D, "PSMode"}, {0x7E, "MachineType"},
        {0x7F, "GetMemorySize"}};
    const auto it = names.find(number);
    if (it != names.end()) return std::string(it->second);
    return std::format("syscall_{}", number);
}

}  // namespace cb::formats::ps2
