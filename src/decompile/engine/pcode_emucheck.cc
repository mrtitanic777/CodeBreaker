// decompile/engine/pcode_emucheck.cc -- the p-code emulator's check (not a test of the suite).
//
//   pcode_emucheck run <file.sla> <binary|hex:BYTES> <base> <entry|reset:ADDR> <steps> [ctx=val ...] [--trace N] [--set REG=val ...]
//       Loads the binary at <base>, starts at <entry> (or at the 16-bit little-endian vector stored at ADDR) and runs <steps>
//       instructions with I/O reads answering 0. Prints the first N instructions as a register trace (the registers each one
//       changed), then the hottest addresses, the user operations that had no meaning, and the final registers.
//   pcode_emucheck unicorn <x86.sla> <16|32> <binary> <steps> [--unicorn path/unicorn.dll] [--quiet]
//       Runs the same x86 code on the p-code emulator and on Unicorn (CodeBreaker's x86 oracle) and compares the general registers,
//       the arithmetic flags and a window of stack and data after every instruction.
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
#include <map>
#include <string>
#include <vector>

#include "decompile/engine/pcode_emulator.hh"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma warning(push, 0)
#include <unicorn/unicorn.h>
#include <unicorn/x86.h>
#pragma warning(pop)
#endif

namespace sl = cb::decompile::sleigh;

namespace {

std::vector<std::uint8_t> readFile(const std::string& spec) {
    std::vector<std::uint8_t> out;
    if (spec.rfind("hex:", 0) == 0) {
        std::string digits;
        for (char c : spec.substr(4))
            if (std::isxdigit(static_cast<unsigned char>(c))) digits += c;
        for (std::size_t i = 0; i + 1 < digits.size(); i += 2) out.push_back(static_cast<std::uint8_t>(std::strtoul(digits.substr(i, 2).c_str(), nullptr, 16)));
        return out;
    }
    std::ifstream in(spec, std::ios::binary);
    if (!in) {
        std::fprintf(stderr, "cannot read %s\n", spec.c_str());
        std::exit(2);
    }
    return std::vector<std::uint8_t>((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
}

std::string hex(std::uint64_t v, unsigned size) {
    char buf[32];
    std::snprintf(buf, sizeof buf, "%0*llx", static_cast<int>(size * 2), static_cast<unsigned long long>(v));
    return buf;
}

bool g_parts = false;

std::map<std::string, std::uint64_t> snapshot(const sl::Emulator& e) {
    std::map<std::string, std::uint64_t> m;
    for (const sl::Emulator::Register& r : e.registers(g_parts)) m[r.name] = e.reg(r.name);
    return m;
}

int run(int argc, char** argv) {
    if (argc < 7) return 2;
    std::vector<std::pair<std::string, int>> ctx;
    std::vector<std::pair<std::string, std::uint64_t>> sets;
    std::uint64_t traceN = 40;
    std::vector<std::string> dumps;
    std::uint64_t loadSize = 0, bankSwitches = 0;
    bool f8 = false, skipNops = false;
    std::vector<std::uint64_t> mirrors;
    std::vector<std::pair<std::uint64_t, std::uint64_t>> mmio;
    for (int i = 7; i < argc; ++i) {
        const std::string a = argv[i];
        if (a == "--trace" && i + 1 < argc) traceN = std::strtoull(argv[++i], nullptr, 0);
        else if (a == "--parts") g_parts = true;
        else if (a == "--f8") f8 = true;
        else if (a == "--skip-nops") skipNops = true;
        else if (a == "--size" && i + 1 < argc) loadSize = std::strtoull(argv[++i], nullptr, 0);
        else if (a == "--mirror" && i + 1 < argc) mirrors.push_back(std::strtoull(argv[++i], nullptr, 0));
        else if (a == "--mmio" && i + 1 < argc) {
            const std::string r = argv[++i];
            mmio.emplace_back(std::strtoull(r.c_str(), nullptr, 0), std::strtoull(r.c_str() + r.find('-') + 1, nullptr, 0));
        }
        else if (a == "--dump" && i + 1 < argc) dumps.push_back(argv[++i]);
        else if (a == "--set" && i + 1 < argc) {
            const std::string kv = argv[++i];
            const auto eq = kv.find('=');
            sets.emplace_back(kv.substr(0, eq), std::strtoull(kv.c_str() + eq + 1, nullptr, 0));
        } else if (a.find('=') != std::string::npos)
            ctx.emplace_back(a.substr(0, a.find('=')), std::atoi(a.c_str() + a.find('=') + 1));
    }
    sl::Emulator emu(argv[2], ctx);
    const std::vector<std::uint8_t> code = readFile(argv[3]);
    const std::uint64_t base = std::strtoull(argv[4], nullptr, 0);
    emu.load(base, code.data(), loadSize && loadSize < code.size() ? loadSize : code.size());
    for (std::uint64_t m : mirrors) emu.load(m, code.data(), loadSize && loadSize < code.size() ? loadSize : code.size());
    std::string entry = argv[5];
    std::uint64_t pc;
    if (entry.rfind("reset:", 0) == 0) {
        pc = emu.readValue(std::strtoull(entry.c_str() + 6, nullptr, 0), 2);
        std::printf("reset vector -> %04llx\n", static_cast<unsigned long long>(pc));
    } else {
        pc = std::strtoull(entry.c_str(), nullptr, 0);
    }
    const std::uint64_t steps = std::strtoull(argv[6], nullptr, 0);
    emu.setPc(pc);
    for (const auto& [k, v] : sets) emu.setReg(k, v);

    // --mmio LO-HI: memory-mapped I/O: reads there answer 0 and writes there are dropped (counted); any other access of the ram
    // goes to the page map, and every access of another space (Z80's "io" ports) is counted and answers 0.
    std::map<std::string, std::uint64_t> ioReads, ioWrites;
    auto isRam = [](std::string_view s) { return s.size() == 3 && (s[0] | 32) == 'r' && (s[1] | 32) == 'a' && (s[2] | 32) == 'm'; };   // ("ram", the 6502's "RAM")
    // --f8: Atari 2600 F8 bankswitching: the 4K banks of the file, bank 0 first; an access of $1FF8/$1FF9 (on the 13-bit bus) selects one
    // and it shows at every 4K window with address bit 12 set.
    auto switchBank = [&](std::uint64_t addr) {
        const std::uint64_t low = addr & 0x1fff;
        if (!f8 || (low != 0x1ff8 && low != 0x1ff9)) return;
        const std::size_t bank = low - 0x1ff8;
        if (bank * 4096 + 4096 > code.size()) return;
        for (std::uint64_t w = 0x1000; w < 0x10000; w += 0x2000) emu.write(w, code.data() + bank * 4096, 4096);
        ++bankSwitches;
    };
    // (with --f8 the whole Atari 2600 map: a 13-bit bus; RAM at $80-$FF mirrored through the stack page; the TIA below it and the RIOT
    // above answer 0 and take writes; the cartridge where address bit 12 is set)
    emu.onRead([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t& value) {
        if (!isRam(space)) {
            ++ioReads[std::string(space)];
            value = 0;
            return true;
        }
        if (f8) {
            value = 0;
            for (unsigned i = 0; i < size; ++i) {
                const std::uint64_t a = (addr + i) & 0x1fff;
                std::uint64_t b = 0;
                if (a & 0x1000) {
                    switchBank(a);
                    b = emu.readValue(0x1000 + (a & 0xfff), 1);
                } else if (a & 0x80) {
                    if (a & 0x200) ++ioReads["RIOT"];
                    else b = emu.readValue(0x80 | (a & 0x7f), 1);
                } else {
                    ++ioReads["TIA"];
                }
                value |= b << (8 * i);
            }
            return true;
        }
        switchBank(addr);
        for (const auto& [lo, hi] : mmio)
            if (addr >= lo && addr <= hi) {
                ++ioReads["mmio"];
                value = 0;
                return true;
            }
        return false;
    });
    emu.onWrite([&](std::string_view space, std::uint64_t addr, unsigned size, std::uint64_t value) {
        if (!isRam(space)) {
            ++ioWrites[std::string(space)];
            return true;
        }
        if (f8) {
            for (unsigned i = 0; i < size; ++i) {
                const std::uint64_t a = (addr + i) & 0x1fff;
                if (a & 0x1000) switchBank(a);   // (the cartridge is read-only)
                else if (a & 0x80) {
                    if (a & 0x200) ++ioWrites["RIOT"];
                    else emu.writeValue(0x80 | (a & 0x7f), 1, (value >> (8 * i)) & 0xff);
                } else {
                    ++ioWrites["TIA"];
                }
            }
            return true;
        }
        switchBank(addr);
        for (const auto& [lo, hi] : mmio)
            if (addr >= lo && addr <= hi) {
                ++ioWrites["mmio"];
                return true;
            }
        return false;
    });

    std::map<std::uint64_t, std::uint64_t> hot;
    std::map<std::string, std::uint64_t> prev = snapshot(emu);
    std::uint64_t shown = 0;
    std::vector<std::string> tail;
    emu.onTrace([&](std::uint64_t at, const sl::Emulator& e) {
        ++hot[at];
        std::map<std::string, std::uint64_t> now = snapshot(e);
        std::string line = "  " + std::to_string(e.steps()) + "  " + hex(at, 4) + "  ";
        std::string t = e.text(at);
        line += t;
        if (line.size() < 44) line.append(44 - line.size(), ' ');
        line += " |";
        for (const auto& [k, v] : now)
            if (prev[k] != v) {
                unsigned size = 1;
                for (const sl::Emulator::Register& r : e.registers(g_parts))
                    if (r.name == k) size = r.size;
                line += " " + k + "=" + hex(v, size);
            }
        prev = std::move(now);
        if (shown < traceN) {
            std::puts(line.c_str());
            ++shown;
        }
        tail.push_back(line);
        if (tail.size() > 12) tail.erase(tail.begin());
    });
    // --skip-nops: the 6502's undocumented NOPs (04 44 64 80 82 89 C2 E2 14 34 54 74 D4 F4: two bytes; 0C: three; 1A 3A 5A 7A DA FA: one) are in no
    // description; step over them the way the processor does (counted as steps) instead of stopping.
    // Host side of --skip-nops: also the undocumented read-modify-write and load/store combinations (SLO RLA SRE RRA SAX LAX DCP ISC) in their
    // addressing modes; binary arithmetic only (the 2600 programs never set D).
    std::uint64_t undocumented = 0;
    if (skipNops) {
        emu.onBadInstruction([&](sl::Emulator& e, std::uint64_t pc) {
            const unsigned op = static_cast<unsigned>(e.readValue(pc, 1));
            const auto b1 = [&] { return static_cast<unsigned>(e.readValue(pc + 1, 1)); };
            const auto w1 = [&] { return static_cast<unsigned>(e.readValue(pc + 1, 2)); };
            unsigned len = 0;
            for (unsigned n : {0x04u, 0x44u, 0x64u, 0x80u, 0x82u, 0x89u, 0xC2u, 0xE2u, 0x14u, 0x34u, 0x54u, 0x74u, 0xD4u, 0xF4u}) if (op == n) len = 2;
            if (op == 0x0C) len = 3;
            for (unsigned n : {0x1Au, 0x3Au, 0x5Au, 0x7Au, 0xDAu, 0xFAu}) if (op == n) len = 1;
            if (len == 0 && (op & 0x1f) == 0x0b && (op == 0x0b || op == 0x2b || op == 0x4b || op == 0xcb || op == 0xeb)) {   // ANC ANC ALR AXS SBC #imm
                const unsigned v = b1(), a = static_cast<unsigned>(e.reg("A")), x = static_cast<unsigned>(e.reg("X"));
                auto nz = [&](unsigned r) { e.setReg("N", (r >> 7) & 1); e.setReg("Z", (r & 0xff) == 0); };
                if (op == 0x0b || op == 0x2b) { const unsigned r = a & v; e.setReg("A", r); nz(r); e.setReg("C", r >> 7); }
                else if (op == 0x4b) { const unsigned r = a & v; e.setReg("C", r & 1); e.setReg("A", r >> 1); nz(r >> 1); }
                else if (op == 0xcb) { const unsigned r = (a & x) - v; e.setReg("C", (a & x) >= v); e.setReg("X", r & 0xff); nz(r); }
                else { const unsigned borrow = 1 - static_cast<unsigned>(e.reg("C")); const int r = static_cast<int>(a) - static_cast<int>(v) - static_cast<int>(borrow);
                       e.setReg("V", (((a ^ v) & (a ^ static_cast<unsigned>(r))) & 0x80) != 0); e.setReg("C", r >= 0); e.setReg("A", static_cast<unsigned>(r) & 0xff); nz(static_cast<unsigned>(r)); }
                len = 2;
            }
            if (len == 0 && (op & 3) == 3 && (op & 0x1c) != 0x08) {
                const unsigned aaa = op >> 5, bbb = (op >> 2) & 7;
                std::uint64_t addr = 0;
                const unsigned x = static_cast<unsigned>(e.reg("X")), y = static_cast<unsigned>(e.reg("Y"));
                const bool yIndexed = aaa == 4 || aaa == 5;   // (SAX, LAX index by Y in the zero page and absolute-X slots)
                switch (bbb) {
                    case 0: addr = e.busRead((b1() + x) & 0xff, 1) | (e.busRead((b1() + x + 1) & 0xff, 1) << 8); len = 2; break;
                    case 1: addr = b1(); len = 2; break;
                    case 3: addr = w1(); len = 3; break;
                    case 4: addr = (e.busRead(b1(), 1) | (e.busRead((b1() + 1) & 0xff, 1) << 8)) + y; len = 2; break;
                    case 5: addr = (b1() + (yIndexed ? y : x)) & 0xff; len = 2; break;
                    case 6: addr = w1() + y; len = 3; break;
                    case 7: addr = w1() + (yIndexed ? y : x); len = 3; break;
                    default: break;
                }
                if (len == 0 || (aaa == 4 && bbb >= 4 && bbb != 5) ) return false;
                addr &= 0xffff;
                auto setNZ = [&](unsigned v) { e.setReg("N", (v >> 7) & 1); e.setReg("Z", (v & 0xff) == 0); };
                unsigned a = static_cast<unsigned>(e.reg("A")), m = 0;
                const unsigned carry = static_cast<unsigned>(e.reg("C"));
                auto adc = [&](unsigned v) {
                    const unsigned sum = a + v + static_cast<unsigned>(e.reg("C"));
                    e.setReg("V", (~(a ^ v) & (a ^ sum) & 0x80) != 0);
                    e.setReg("C", sum > 0xff);
                    a = sum & 0xff;
                    setNZ(a);
                    e.setReg("A", a);
                };
                switch (aaa) {
                    case 4: e.busWrite(addr, 1, a & x); break;
                    case 5: m = static_cast<unsigned>(e.busRead(addr, 1)); e.setReg("A", m); e.setReg("X", m); setNZ(m); break;
                    default: {
                        m = static_cast<unsigned>(e.busRead(addr, 1));
                        if (aaa == 0) { e.setReg("C", m >> 7); m = (m << 1) & 0xff; e.busWrite(addr, 1, m); a |= m; e.setReg("A", a); setNZ(a); }
                        else if (aaa == 1) { const unsigned c = m >> 7; m = ((m << 1) | carry) & 0xff; e.busWrite(addr, 1, m); e.setReg("C", c); a &= m; e.setReg("A", a); setNZ(a); }
                        else if (aaa == 2) { e.setReg("C", m & 1); m >>= 1; e.busWrite(addr, 1, m); a ^= m; e.setReg("A", a); setNZ(a); }
                        else if (aaa == 3) { const unsigned c = m & 1; m = (m >> 1) | (carry << 7); e.busWrite(addr, 1, m); e.setReg("C", c); adc(m); }
                        else if (aaa == 6) { m = (m - 1) & 0xff; e.busWrite(addr, 1, m); e.setReg("C", a >= m); setNZ(a - m); }
                        else { m = (m + 1) & 0xff; e.busWrite(addr, 1, m); adc(m ^ 0xff); }
                    }
                }
            }
            if (len == 0) return false;
            ++undocumented;
            e.setPc(pc + len);
            return true;
        });
    }
    sl::Emulator::RunResult r;
    if (skipNops) {
        // The 6502 description's SP is a 16-bit pointer (S low byte, SH high byte), so a push at S=0 carries into SH and the stack leaves
        // page 1; the chip's S wraps in 8 bits. Put SH back to 1 after each instruction (a gap of the description, reported).
        for (; r.steps < steps; ++r.steps) {
            r.status = emu.step();
            if (r.status != sl::Emulator::Status::Ok) break;
            if (emu.reg("SH") != 1) emu.setReg("SH", 1);
        }
        if (r.status == sl::Emulator::Status::Ok) r.status = sl::Emulator::Status::Ok;
    } else {
        r = emu.run(steps);
    }
    if (undocumented) std::printf("undocumented opcodes carried out by the host: %llu\n", static_cast<unsigned long long>(undocumented));
    const char* why[] = {"ran all steps", "stopped", "BAD INSTRUCTION", "ERROR"};
    std::printf("\n%llu steps, %s, final pc %s", static_cast<unsigned long long>(r.steps), why[static_cast<int>(r.status)], hex(emu.pc(), 4).c_str());
    if (r.status == sl::Emulator::Status::Error || r.status == sl::Emulator::Status::BadInstruction) std::printf(" (%s)", emu.lastError().c_str());
    std::printf("\ndistinct instructions run: %zu\n", hot.size());
    if (f8) std::printf("F8 bank switches: %llu\n", static_cast<unsigned long long>(bankSwitches));
    for (const auto& [k, n] : ioReads) std::printf("I/O reads answered 0 (%s): %llu\n", k.c_str(), static_cast<unsigned long long>(n));
    for (const auto& [k, n] : ioWrites) std::printf("I/O writes taken (%s): %llu\n", k.c_str(), static_cast<unsigned long long>(n));
    std::vector<std::pair<std::uint64_t, std::uint64_t>> top(hot.begin(), hot.end());
    std::sort(top.begin(), top.end(), [](auto& a, auto& b) { return a.second > b.second; });
    std::puts("hottest addresses:");
    for (std::size_t i = 0; i < top.size() && i < 10; ++i)
        std::printf("  %s  x%-6llu %s\n", hex(top[i].first, 4).c_str(), static_cast<unsigned long long>(top[i].second), emu.text(top[i].first).c_str());
    if (!emu.unimplementedOps().empty()) {
        std::puts("user operations with no meaning (count):");
        for (const auto& [n, c] : emu.unimplementedOps()) std::printf("  %s x%llu\n", n.c_str(), static_cast<unsigned long long>(c));
    }
    std::puts("last instructions:");
    for (const std::string& l : tail) std::puts(l.c_str());
    std::string regs = "final registers:";
    for (const auto& [k, v] : snapshot(emu))
        if (v != 0) regs += " " + k + "=" + std::to_string(v) + "(0x" + hex(v, 1) + ")";
    std::puts(regs.c_str());
    for (const std::string& d : dumps) {
        const std::uint64_t at = std::strtoull(d.c_str(), nullptr, 0);
        const std::uint64_t n = d.find(',') == std::string::npos ? 16 : std::strtoull(d.c_str() + d.find(',') + 1, nullptr, 0);
        std::string line = "memory " + hex(at, 2) + ":";
        for (std::uint64_t i = 0; i < n; ++i) line += " " + hex(emu.readValue(at + i, 1), 1);
        std::puts(line.c_str());
    }
    return 0;
}

#ifdef _WIN32
// ---------------------------------------------------------------- the x86 comparison against Unicorn
struct Uc {
    decltype(&uc_open) open = nullptr;
    decltype(&uc_close) close = nullptr;
    decltype(&uc_mem_map) mem_map = nullptr;
    decltype(&uc_mem_write) mem_write = nullptr;
    decltype(&uc_mem_read) mem_read = nullptr;
    decltype(&uc_reg_write) reg_write = nullptr;
    decltype(&uc_reg_read) reg_read = nullptr;
    decltype(&uc_emu_start) emu_start = nullptr;
    decltype(&uc_strerror) strerror_ = nullptr;
};

int unicornCompare(int argc, char** argv) {
    if (argc < 6) return 2;
    std::string dll;
    bool quiet = false;
    for (int i = 6; i < argc; ++i) {
        if (std::string(argv[i]) == "--unicorn" && i + 1 < argc) dll = argv[++i];
        else if (std::string(argv[i]) == "--quiet") quiet = true;
    }
    if (dll.empty()) dll = "src/third_party/bin/unicorn.dll";
    HMODULE lib = LoadLibraryA(dll.c_str());
    if (!lib) {
        std::fprintf(stderr, "cannot load %s\n", dll.c_str());
        return 2;
    }
    Uc u;
#define LOADF(field, name) u.field = reinterpret_cast<decltype(u.field)>(GetProcAddress(lib, #name))
    LOADF(open, uc_open); LOADF(close, uc_close); LOADF(mem_map, uc_mem_map); LOADF(mem_write, uc_mem_write); LOADF(mem_read, uc_mem_read);
    LOADF(reg_write, uc_reg_write); LOADF(reg_read, uc_reg_read); LOADF(emu_start, uc_emu_start); LOADF(strerror_, uc_strerror);
#undef LOADF
    const int bits = std::atoi(argv[3]);
    const std::vector<std::uint8_t> code = readFile(argv[4]);
    const std::uint64_t steps = std::strtoull(argv[5], nullptr, 0);

    // layout: code, a data window, a stack
    const std::uint64_t codeBase = bits == 16 ? 0x1000 : 0x401000;
    const std::uint64_t dataBase = bits == 16 ? 0x4000 : 0x500000;
    const std::uint64_t stackTop = bits == 16 ? 0xF000 : 0x600000;
    const std::uint64_t mapBase = bits == 16 ? 0 : 0x400000;
    const std::uint64_t mapSize = bits == 16 ? 0x10000 : 0x300000;

    sl::Emulator emu(argv[2], {{"addrsize", bits == 16 ? 0 : 1}, {"opsize", bits == 16 ? 0 : 1}});
    uc_engine* uc = nullptr;
    if (u.open(UC_ARCH_X86, bits == 16 ? UC_MODE_16 : UC_MODE_32, &uc) != UC_ERR_OK) {
        std::fprintf(stderr, "uc_open failed\n");
        return 2;
    }
    u.mem_map(uc, mapBase, mapSize, UC_PROT_ALL);
    u.mem_write(uc, codeBase, code.data(), code.size());
    emu.load(codeBase, code.data(), code.size());
    std::vector<std::uint8_t> data(0x800);
    for (std::size_t i = 0; i < data.size(); ++i) data[i] = static_cast<std::uint8_t>(i * 7 + 3);
    u.mem_write(uc, dataBase, data.data(), data.size());
    emu.load(dataBase, data.data(), data.size());

    struct R { const char* name; int uc; };
    const R regs[] = {{"EAX", UC_X86_REG_EAX}, {"ECX", UC_X86_REG_ECX}, {"EDX", UC_X86_REG_EDX}, {"EBX", UC_X86_REG_EBX},
                      {"ESP", UC_X86_REG_ESP}, {"EBP", UC_X86_REG_EBP}, {"ESI", UC_X86_REG_ESI}, {"EDI", UC_X86_REG_EDI}};
    const std::uint32_t init[] = {0x12345678, 0x00000005, 0x9abcdef0, 0x0000ff01, static_cast<std::uint32_t>(stackTop), static_cast<std::uint32_t>(stackTop - 0x40),
                                  static_cast<std::uint32_t>(dataBase), static_cast<std::uint32_t>(dataBase + 0x100)};
    for (int i = 0; i < 8; ++i) {
        std::uint32_t v = init[i];
        u.reg_write(uc, regs[i].uc, &v);
        emu.setReg(regs[i].name, v);
    }
    std::uint32_t eflags = 2;
    u.reg_write(uc, UC_X86_REG_EFLAGS, &eflags);
    struct F { const char* name; unsigned bit; };
    const F flags[] = {{"CF", 0}, {"PF", 2}, {"AF", 4}, {"ZF", 6}, {"SF", 7}, {"DF", 10}, {"OF", 11}};
    emu.setPc(codeBase);
    const std::uint64_t codeEnd = codeBase + code.size();
    std::uint64_t compared = 0, instructions = 0, mismatchSteps = 0, regCompares = 0, regMismatches = 0;
    std::map<std::string, std::uint64_t> byInstruction;
    std::uint64_t afOnly = 0, undefinedOnly = 0, realMismatch = 0;
    for (std::uint64_t s = 0; s < steps && emu.pc() < codeEnd && emu.pc() >= codeBase; ++s) {
        const std::uint64_t at = emu.pc();
        const std::string text = emu.text(at);
        std::uint64_t pcu = at;
        const uc_err e1 = u.emu_start(uc, pcu, 0x7ffffff0, 0, 1);
        const sl::Emulator::Status st = emu.step();
        if (e1 != UC_ERR_OK || st != sl::Emulator::Status::Ok) {
            std::printf("step %llu at %llx (%s): unicorn %s, emulator status %d %s\n", static_cast<unsigned long long>(s), static_cast<unsigned long long>(at), text.c_str(),
                        u.strerror_(e1), static_cast<int>(st), emu.lastError().c_str());
            break;
        }
        ++instructions;
        std::string diff;
        std::vector<std::string> flagBad;
        std::uint32_t ucv[8], ef = 0, eip = 0;
        for (int i = 0; i < 8; ++i) u.reg_read(uc, regs[i].uc, &ucv[i]);
        u.reg_read(uc, UC_X86_REG_EFLAGS, &ef);
        u.reg_read(uc, UC_X86_REG_EIP, &eip);
        for (int i = 0; i < 8; ++i) {
            ++regCompares;
            const std::uint32_t mine = static_cast<std::uint32_t>(emu.reg(regs[i].name));
            if (mine != ucv[i]) {
                char b[96];
                std::snprintf(b, sizeof b, " %s emu=%08x uc=%08x", regs[i].name, mine, ucv[i]);
                diff += b;
                ++regMismatches;
            }
        }
        for (const F& f : flags) {
            ++regCompares;
            const unsigned mine = static_cast<unsigned>(emu.reg(f.name)) & 1, theirs = (ef >> f.bit) & 1;
            if (mine != theirs) {
                diff += std::string(" ") + f.name + " emu=" + std::to_string(mine) + " uc=" + std::to_string(theirs);
                flagBad.push_back(f.name);
                ++regMismatches;
            }
        }
        ++regCompares;
        const std::uint32_t pcMine = static_cast<std::uint32_t>(emu.pc());
        if (pcMine != eip) {
            char b[96];
            std::snprintf(b, sizeof b, " PC emu=%x uc=%x", pcMine, eip);
            diff += b;
            ++regMismatches;
        }
        // memory: 256 bytes under the stack top, 0x800 of data
        std::vector<std::uint8_t> a(0x100), b2(0x100), c(data.size()), d(data.size());
        u.mem_read(uc, stackTop - 0x100, a.data(), a.size());
        emu.read(stackTop - 0x100, b2.data(), b2.size());
        u.mem_read(uc, dataBase, c.data(), c.size());
        emu.read(dataBase, d.data(), d.size());
        ++regCompares;
        if (a != b2 || c != d) {
            diff += " MEMORY";
            ++regMismatches;
        }
        ++compared;
        if (!diff.empty()) {
            // Sort the difference: AF (the description never computes it), flags the processor leaves undefined (the multiplies'
            // SF ZF PF; a shift's or rotate's OF unless the count is 1), anything else is real.
            const std::string mn = text.substr(0, text.find(' '));
            std::string rest;
            for (const std::string& f : flagBad)
                if (f != "AF") rest += f;
            const bool onlyFlags = diff.find("MEMORY") == std::string::npos && diff.find(" E") == std::string::npos && diff.find(" PC ") == std::string::npos;
            const bool mulUndef = (mn == "MUL" || mn == "IMUL" || mn == "DIV" || mn == "IDIV") && rest.find("OF") == std::string::npos && rest.find("CF") == std::string::npos;
            const bool shiftUndef = (mn == "SHR" || mn == "SHL" || mn == "SAR" || mn == "SAL" || mn == "ROL" || mn == "ROR" || mn == "RCL" || mn == "RCR") && rest == "OF";
            if (onlyFlags && rest.empty()) ++afOnly;
            else if (onlyFlags && (mulUndef || shiftUndef)) ++undefinedOnly;
            else ++realMismatch;
            ++mismatchSteps;
            ++byInstruction[text.substr(0, text.find(' '))];
            if (!quiet) std::printf("MISMATCH step %llu at %llx  %-28s%s\n", static_cast<unsigned long long>(s), static_cast<unsigned long long>(at), text.c_str(), diff.c_str());
            // resynchronise the emulator to Unicorn so one divergence is counted once
            for (int i = 0; i < 8; ++i) emu.setReg(regs[i].name, ucv[i]);
            for (const F& f : flags) emu.setReg(f.name, (ef >> f.bit) & 1);
            emu.setPc(eip);
            emu.write(stackTop - 0x100, a.data(), a.size());
            emu.write(dataBase, c.data(), c.size());
        }
    }
    std::printf("%llu instructions compared against Unicorn: %llu agree on every register, flag, pc and memory window; %llu differ (%llu of %llu individual values)\n",
                static_cast<unsigned long long>(compared), static_cast<unsigned long long>(compared - mismatchSteps), static_cast<unsigned long long>(mismatchSteps),
                static_cast<unsigned long long>(regMismatches), static_cast<unsigned long long>(regCompares));
    std::printf("  of the %llu differing: %llu only AF (never computed by the description), %llu only flags the processor leaves undefined, %llu REAL\n",
                static_cast<unsigned long long>(mismatchSteps), static_cast<unsigned long long>(afOnly), static_cast<unsigned long long>(undefinedOnly),
                static_cast<unsigned long long>(realMismatch));
    for (const auto& [m, n] : byInstruction) std::printf("  differing mnemonic %s x%llu\n", m.c_str(), static_cast<unsigned long long>(n));
    if (!emu.unimplementedOps().empty())
        for (const auto& [n, c] : emu.unimplementedOps()) std::printf("  unimplemented user op %s x%llu\n", n.c_str(), static_cast<unsigned long long>(c));
    u.close(uc);
    return realMismatch ? 1 : 0;
}
#endif

}  // namespace

int main(int argc, char** argv) {
    try {
        if (argc >= 2 && std::string(argv[1]) == "run") {
            if (const int rc = run(argc, argv)) {
                std::fprintf(stderr, "usage: pcode_emucheck run <file.sla> <binary|hex:..> <base> <entry|reset:ADDR> <steps> [ctx=val ...] [--trace N] [--set REG=val]\n");
                return rc;
            }
            return 0;
        }
#ifdef _WIN32
        if (argc >= 2 && std::string(argv[1]) == "unicorn") {
            if (argc < 6) {
                std::fprintf(stderr, "usage: pcode_emucheck unicorn <x86.sla> <16|32> <binary> <steps> [--unicorn dll] [--quiet]\n");
                return 2;
            }
            return unicornCompare(argc, argv);
        }
#endif
    } catch (const std::exception& e) {
        std::fprintf(stderr, "error: %s\n", e.what());
        return 2;
    }
    std::fprintf(stderr, "usage: pcode_emucheck run|unicorn ...\n");
    return 2;
}
