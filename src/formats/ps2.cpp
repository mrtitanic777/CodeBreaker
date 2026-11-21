// cb-unity: alone (file-local names that other files in a batch also use)
#include "formats/ps2.hpp"
#include "knowledge/package.hpp"

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
    // config/knowledge/ps2/registers.json
    static const std::map<std::uint32_t, std::string> table = knowledge::standard().hardware_names("ps2", "registers");
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
    static const std::vector<knowledge::HardwareRegion> blocks = knowledge::standard().hardware_regions("ps2", "blocks");   // (config/knowledge/ps2/registers.json)
    for (const auto& b : blocks)
        if (*p >= b.address && *p - b.address < b.size) return std::format("{}_{:04X}", b.name, *p - b.address);
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
