// decompile/engine/sleigh_asmcheck.cc -- the assembler's round-trip metric on real code.
//
//   sleigh_asmcheck <file.sla> <binary> <offset> <base> <count> [ctxvar=val ...]
//
// Disassembles `count` instructions linearly with sleigh::Reader, re-assembles each one's text at its address with sleigh::Assembler, and
// counts the exact byte matches. A result that differs but reads back as the same text is an "equivalent encoding".

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include "decompile/engine/sleigh_assembler.hh"
#include "decompile/engine/sleigh_reader.hh"

int main(int argc, char** argv) {
    if (argc < 6) {
        std::fprintf(stderr, "usage: sleigh_asmcheck <file.sla> <binary> <offset> <base> <count> [ctxvar=val ...]\n");
        return 2;
    }
    namespace sl = cb::decompile::sleigh;
    std::vector<std::pair<std::string, int>> ctx;
    for (int i = 6; i < argc; ++i) {
        const std::string a = argv[i];
        const auto eq = a.find('=');
        if (eq != std::string::npos) ctx.emplace_back(a.substr(0, eq), std::atoi(a.c_str() + eq + 1));
    }
    std::ifstream in(argv[2], std::ios::binary);
    if (!in) {
        std::fprintf(stderr, "cannot read %s\n", argv[2]);
        return 2;
    }
    const std::vector<std::uint8_t> file((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    const std::size_t offset = std::strtoull(argv[3], nullptr, 0);
    const std::uint64_t base = std::strtoull(argv[4], nullptr, 0);
    const std::size_t count = std::strtoull(argv[5], nullptr, 0);
    if (offset >= file.size()) return 2;
    try {
        sl::Reader reader(argv[1], ctx);
        sl::Assembler assembler(argv[1], ctx);
        if (const char* line = std::getenv("CB_ASM_LINE")) {   // one line, for diagnosis: CB_ASM_LINE="JR NZ,0x15b" CB_ASM_ADDR=0x160
            const char* a = std::getenv("CB_ASM_ADDR");
            for (const auto& g : assembler.assemble(line, a != nullptr ? std::strtoull(a, nullptr, 0) : 0)) {
                for (auto b : g) std::printf("%02x", b);
                std::printf("\n");
            }
            std::printf("parses=%zu candidates=%zu verified=%zu\n", assembler.last().parses, assembler.last().candidates, assembler.last().verified);
            return 0;
        }
        const auto t0 = std::chrono::steady_clock::now();
        const auto ins = reader.read(file.data() + offset, file.size() - offset, base, count);
        std::size_t total = 0, exact = 0, equivalent = 0, failed = 0, undecodable = 0, shown = 0, noparse = 0;
        std::size_t at = 0;
        for (const sl::Instruction& i : ins) {
            if (i.length == 0) {
                ++undecodable;
                continue;
            }
            ++total;
            const std::string text = i.operands.empty() ? i.mnemonic : i.mnemonic + " " + i.operands;
            const std::uint64_t addr = i.address;
            // the instruction's own bytes: the reader steps by length, so recompute the position
            // (positions of undecodable places step by alignment, which the reader reports as length 0)
            (void)at;
            std::size_t pos = 0;
            {
                // find this instruction's byte offset: address minus base, in bytes (byte-addressed processors)
                pos = static_cast<std::size_t>(addr - base);
            }
            const std::vector<std::uint8_t> orig(file.begin() + static_cast<std::ptrdiff_t>(offset + pos),
                                                 file.begin() + static_cast<std::ptrdiff_t>(offset + pos + i.length));
            const auto got = assembler.assemble(text, addr);
            bool hit = false;
            for (const auto& g : got) hit = hit || g == orig;
            if (hit) {
                ++exact;
            } else if (!got.empty()) {
                ++equivalent;
                if (shown < 8) {
                    ++shown;
                    std::printf("  equiv  %08llx  %s  orig=", static_cast<unsigned long long>(addr), text.c_str());
                    for (auto b : orig) std::printf("%02x", b);
                    std::printf("  got=");
                    for (auto b : got[0]) std::printf("%02x", b);
                    std::printf("\n");
                }
            } else {
                ++failed;
                if (assembler.last().parses == 0) ++noparse;
                if (failed <= 25) {
                    std::printf("  FAIL   %08llx  %s  orig=", static_cast<unsigned long long>(addr), text.c_str());
                    for (auto b : orig) std::printf("%02x", b);
                    std::printf("  parses=%zu candidates=%zu\n", assembler.last().parses, assembler.last().candidates);
                }
            }
        }
        const double secs = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
        const double pct = total == 0 ? 0.0 : 100.0 * static_cast<double>(exact + equivalent) / static_cast<double>(total);
        std::printf("%s: decoded %zu (undecodable %zu): exact %zu, equivalent %zu, failed %zu (no parse %zu); round trip %.2f%% in %.1fs\n", argv[2],
                    total, undecodable, exact, equivalent, failed, noparse, pct, secs);
    } catch (const std::exception& e) {
        std::fprintf(stderr, "error: %s\n", e.what());
        return 1;
    }
    return 0;
}
