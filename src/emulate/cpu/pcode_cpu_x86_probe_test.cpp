// cb-unity: alone (file-local names that other files in a batch also use)
// What Unicorn does with the instructions the adapter has to carry out itself (HLT, INT, CPUID, far transfers, invalid opcodes ...): printed, for the adapter to match.
#include <cstdio>
#include <format>
#include <string>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "emulate/unicorn_uc.hpp"

namespace {

using namespace cb;
namespace uu = cb::emulate::unicorn_uc;
using u64 = std::uint64_t;

int reg(const char* name) { return *uu::x86_register(name); }

}  // namespace

CB_TEST("emulate.pcode_cpu.x86_probe_unicorn", [] {
    const auto filter = fs::get_env("CB_X86_PROBE");
    if (!filter || filter->empty()) return;
    // CB_X86_PROBE = programs separated by ';', each hex bytes with '+' between instructions
    std::string list = *filter;
    std::size_t at = 0;
    while (at < list.size()) {
        std::size_t end = list.find(';', at);
        if (end == std::string::npos) end = list.size();
        const std::string text = list.substr(at, end - at);
        at = end + 1;
        Bytes code;
        u64 count = 1;
        for (std::size_t i = 0; i + 1 < text.size();) {
            if (text[i] == '+') {
                ++count;
                ++i;
                continue;
            }
            code.push_back(static_cast<std::uint8_t>(std::stoul(text.substr(i, 2), nullptr, 16)));
            i += 2;
        }
        code.resize(16, 0x90);
        for (const int bits : {64, 16}) {
            std::unique_ptr<core16::Uc> uc = uu::open_x86(bits);
            u64 base = bits == 64 ? 0x400000 : 0x1100, until = 0;
            uc->mem_map(bits == 64 ? 0x400000 : 0, bits == 64 ? 0x10000 : 0x110000);
            if (bits == 64) {
                uc->mem_map(0x600000, 0x10000);
                uc->reg_write(reg("RSP"), 0x600000 + 0x8000);
                for (const char* r : {"RAX", "RBX", "RCX", "RDX", "RSI", "RDI"}) uc->reg_write(reg(r), 0x1111111122223333ull);
                until = 0x7FFFFFF0;
            } else {
                uc->reg_write(reg("CS"), 0x100);
                base = 0x1000 + 0x100;
                uc->reg_write(reg("SP"), 0x3F00);
                until = 0x1000 + 0xFFFE;
            }
            uc->mem_write(base, code);
            int fault = 0;
            std::string message;
            try {
                uc->emu_start(base, until, 0, count);
            } catch (const uu::UcFault& f) {
                fault = f.code();
                message = f.what();
            }
            const u64 ip = bits == 64 ? uc->reg_read(reg("RIP")) : (uc->reg_read(reg("CS")) << 4) + uc->reg_read(reg("IP"));
            std::printf("%s %d-bit: fault %d (%s) ip+%lld", text.c_str(), bits, fault, message.c_str(), static_cast<long long>(ip - base));
            for (const char* r : {"RAX", "RBX", "RCX", "RDX", "RSP"}) std::printf(" %s=%llX", r, static_cast<unsigned long long>(uc->reg_read(reg(bits == 64 ? r : r))));
            std::printf("\n");
        }
    }
});

// Unicorn's CPUID, leaf by leaf and sub-leaf by sub-leaf, printed as rows for the adapter's table (CB_X86_CPUID_DUMP=1).
CB_TEST("emulate.pcode_cpu.x86_probe_unicorn_cpuid_table", [] {
    const auto on = fs::get_env("CB_X86_CPUID_DUMP");
    if (!on || on->empty()) return;
    for (const bool extended : {false, true})
        for (unsigned index = 0; index <= 0x40; ++index)
            for (unsigned sub = 0; sub < 8; ++sub) {
                const unsigned leaf = (extended ? 0x80000000u : 0u) + index;
                std::unique_ptr<core16::Uc> uc = uu::open_x86(64);
                uc->mem_map(0x400000, 0x1000);
                // mov eax, leaf ; mov ecx, sub ; cpuid
                Bytes code = {0xB8, 0, 0, 0, 0, 0xB9, 0, 0, 0, 0, 0x0F, 0xA2};
                for (int k = 0; k < 4; ++k) {
                    code[1 + k] = static_cast<std::uint8_t>(leaf >> (8 * k));
                    code[6 + k] = static_cast<std::uint8_t>(sub >> (8 * k));
                }
                uc->mem_write(0x400000, code);
                uc->emu_start(0x400000, 0x400000 + code.size(), 0, 0);
                std::printf("CPUID %08X %u: %08llX %08llX %08llX %08llX\n", leaf, sub, static_cast<unsigned long long>(uc->reg_read(reg("RAX"))), static_cast<unsigned long long>(uc->reg_read(reg("RBX"))),
                            static_cast<unsigned long long>(uc->reg_read(reg("RCX"))), static_cast<unsigned long long>(uc->reg_read(reg("RDX"))));
            }
});

// Which instructions Unicorn refuses when a LOCK prefix is put in front of them (CB_X86_LOCK_DUMP=1): by opcode map (one byte, 0F) and whether the ModRM operand is a register.
CB_TEST("emulate.pcode_cpu.x86_probe_unicorn_lock", [] {
    const auto on = fs::get_env("CB_X86_LOCK_DUMP");
    if (!on || on->empty()) return;
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    const auto run = [&](const Bytes& code) {
        std::unique_ptr<core16::Uc> uc = uu::open_x86(64);
        uc->mem_map(0x400000, 0x1000);
        uc->mem_map(0x500000, 0x10000);
        uc->mem_map(0x600000, 0x10000);
        uc->mem_write(0x400000, code);
        for (const char* r : {"RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP"}) uc->reg_write(reg(r), 0x500100);
        uc->reg_write(reg("RSP"), 0x608000);
        try {
            uc->emu_start(0x400000, 0x7FFFFFF0, 0, 1);
        } catch (const uu::UcFault& f) {
            return f.code();
        }
        return 0;
    };
    for (const int map : {0, 1})
        for (const int mod3 : {0, 1}) {
            std::printf("LOCK map %d mod3 %d:", map, mod3);
            for (unsigned op = 0; op < 256; ++op) {
                Bytes plain;
                if (map == 1) plain.push_back(0x0F);
                plain.push_back(static_cast<std::uint8_t>(op));
                plain.push_back(mod3 != 0 ? 0xC1 : 0x08);   // modrm: reg 1, rm 1 (rcx) or [rax]
                plain.resize(16, 0);
                Bytes locked = plain;
                locked.insert(locked.begin(), 0xF0);
                if (const auto skip = fs::get_env("CB_X86_LOCK_SKIP"); skip && (map == 1) && (" " + *skip + " ").find(std::format(" {:02x}:{} ", op, mod3)) != std::string::npos) continue;
                if (fs::get_env("CB_X86_LOCK_TRACE")) std::fprintf(stderr, "[%d %d %02x]\n", map, mod3, op);
                const int a = run(plain), b = run(locked);
                if (b == 10 && a != 10) std::printf(" %02x", op);
            }
            std::printf("\n");
        }
});
