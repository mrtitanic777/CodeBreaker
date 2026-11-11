// cb/recompile/test_ppc.hpp -- helpers of the recompile tests: PowerPC word encoders for hand-made switch code, and
// the reader of testdata/switch_samples.txt (real tables of Minecraft with what the original found in each).
#pragma once

#include <algorithm>
#include <cstdint>
#include <initializer_list>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace cb::recompile::test {

// ---- encoders ------------------------------------------------------------------------------------------------------
constexpr std::uint32_t kBctrWord = 0x4E800420;
constexpr std::uint32_t lis(unsigned rd, std::uint32_t imm) { return (15u << 26) | (rd << 21) | (imm & 0xFFFF); }
constexpr std::uint32_t addi(unsigned rd, unsigned ra, std::uint32_t imm) { return (14u << 26) | (rd << 21) | (ra << 16) | (imm & 0xFFFF); }
constexpr std::uint32_t ori(unsigned ra, unsigned rs, std::uint32_t imm) { return (24u << 26) | (rs << 21) | (ra << 16) | (imm & 0xFFFF); }
constexpr std::uint32_t mtctr(unsigned rs) { return 0x7C0903A6u | (rs << 21); }
constexpr std::uint32_t x31(unsigned rt, unsigned ra, unsigned rb, unsigned xo) { return (31u << 26) | (rt << 21) | (ra << 16) | (rb << 11) | (xo << 1); }
constexpr std::uint32_t lwzx(unsigned rt, unsigned ra, unsigned rb) { return x31(rt, ra, rb, 23); }
constexpr std::uint32_t lbzx(unsigned rt, unsigned ra, unsigned rb) { return x31(rt, ra, rb, 87); }
constexpr std::uint32_t lhzx(unsigned rt, unsigned ra, unsigned rb) { return x31(rt, ra, rb, 279); }
constexpr std::uint32_t add(unsigned rt, unsigned ra, unsigned rb) { return x31(rt, ra, rb, 266); }
constexpr std::uint32_t rlwinm(unsigned ra, unsigned rs, unsigned sh, unsigned mb, unsigned me) {
    return (21u << 26) | (rs << 21) | (ra << 16) | (sh << 11) | (mb << 6) | (me << 1);
}
constexpr std::uint32_t slwi(unsigned ra, unsigned rs, unsigned n) { return rlwinm(ra, rs, n, 0, 31 - n); }
constexpr std::uint32_t clrlwi(unsigned ra, unsigned rs, unsigned n) { return rlwinm(ra, rs, 0, n, 31); }
constexpr std::uint32_t cmplwi(unsigned crf, unsigned ra, std::uint32_t imm) { return (10u << 26) | ((crf * 4) << 21) | (ra << 16) | (imm & 0xFFFF); }
// bgt crN, +offset (relative to the branch itself)
constexpr std::uint32_t bgt(unsigned crf, std::int32_t offset) { return (16u << 26) | (12u << 21) | ((crf * 4 + 1) << 16) | (static_cast<std::uint32_t>(offset) & 0xFFFC); }
constexpr std::uint32_t blr() { return 0x4E800020; }
constexpr std::uint32_t nop() { return 0x60000000; }
constexpr std::uint32_t b(std::int32_t offset) { return (18u << 26) | (static_cast<std::uint32_t>(offset) & 0x03FFFFFC); }

// A big-endian image: words and bytes written at addresses, the rest zero.
struct Mem {
    std::uint32_t base = 0;
    std::vector<std::uint8_t> bytes;
    Mem(std::uint32_t base_, std::size_t size) : base(base_), bytes(size, 0) {}
    void u32(std::uint32_t addr, std::uint32_t v) {
        for (int i = 0; i < 4; ++i) bytes.at(addr - base + i) = static_cast<std::uint8_t>(v >> (24 - 8 * i));
    }
    void u16(std::uint32_t addr, std::uint16_t v) {
        bytes.at(addr - base) = static_cast<std::uint8_t>(v >> 8);
        bytes.at(addr - base + 1) = static_cast<std::uint8_t>(v);
    }
    void u8(std::uint32_t addr, std::uint8_t v) { bytes.at(addr - base) = v; }
    // code(addr, {w0, w1, ...}) writes consecutive words and returns the address after the last
    std::uint32_t code(std::uint32_t addr, std::initializer_list<std::uint32_t> words) {
        for (const std::uint32_t w : words) {
            u32(addr, w);
            addr += 4;
        }
        return addr;
    }
};

// ---- the real samples -----------------------------------------------------------------------------------------------
struct ExpectedTable {
    std::uint32_t bctr = 0, r = 0, default_target = 0, table = 0;
    std::string kind;
    std::optional<std::uint32_t> guard;
    bool inline_table = false;
    std::optional<std::pair<std::uint32_t, std::uint32_t>> func;
    std::vector<std::uint32_t> labels;
};
struct ExpectedJump {
    std::uint32_t bctr = 0, reg = 0;
    std::vector<std::uint32_t> targets;
};
struct Sample {
    std::string name;
    std::vector<std::pair<std::uint32_t, std::vector<std::uint8_t>>> windows;   // the bytes of the image that were kept
    std::uint32_t code_lo = 0, code_hi = 0, exec_lo = 0, exec_hi = 0;
    std::vector<std::pair<std::uint32_t, std::uint32_t>> funcs;
    std::vector<ExpectedTable> tables;
    std::vector<std::uint32_t> unresolved, outside, pointers;
    std::vector<ExpectedJump> jumps;

    // the lowest and one past the highest address of any window
    std::pair<std::uint32_t, std::uint32_t> span() const {
        std::uint32_t lo = 0xFFFFFFFF, hi = 0;
        for (const auto& [a, b] : windows) {
            lo = std::min(lo, a);
            hi = std::max(hi, static_cast<std::uint32_t>(a + b.size()));
        }
        return {lo, hi};
    }
    // the windows laid out from span().first (the gaps zero)
    std::vector<std::uint8_t> flat() const {
        const auto [lo, hi] = span();
        std::vector<std::uint8_t> out(hi - lo, 0);
        for (const auto& [a, b] : windows) std::copy(b.begin(), b.end(), out.begin() + (a - lo));
        return out;
    }
};

inline std::uint32_t hex32(std::string_view s) { return *text::parse_int<std::uint32_t>(s, 16); }
inline std::vector<std::uint32_t> hex_list(std::string_view s) {
    std::vector<std::uint32_t> out;
    if (s == "-") return out;
    for (const auto piece : text::split(s, ',')) out.push_back(hex32(piece));
    return out;
}

inline std::vector<Sample> load_samples() {
    std::vector<Sample> out;
    const std::string text = fs::read_text(testing::data_path("recompile/testdata/switch_samples.txt"));
    for (const std::string_view raw : text::lines(text)) {
        const std::string_view line = text::trim(raw);
        if (line.empty() || line[0] == '#') continue;
        const auto parts = text::split(line, ' ');
        const std::string_view key = parts[0];
        if (key == "case") {
            out.emplace_back();
            out.back().name = std::string(parts[1]);
            continue;
        }
        Sample& s = out.back();
        if (key == "window") {
            std::vector<std::uint8_t> bytes;
            for (std::size_t i = 0; i + 1 < parts[2].size(); i += 2) bytes.push_back(static_cast<std::uint8_t>(*text::parse_int<unsigned>(parts[2].substr(i, 2), 16)));
            s.windows.emplace_back(hex32(parts[1]), std::move(bytes));
        } else if (key == "code") {
            s.code_lo = hex32(parts[1]);
            s.code_hi = hex32(parts[2]);
        } else if (key == "exec") {
            s.exec_lo = hex32(parts[1]);
            s.exec_hi = hex32(parts[2]);
        } else if (key == "func") {
            s.funcs.emplace_back(hex32(parts[1]), hex32(parts[2]));
        } else if (key == "table") {
            ExpectedTable t;
            t.bctr = hex32(parts[1]);
            for (std::size_t i = 2; i < parts.size(); ++i) {
                const auto eq = parts[i].find('=');
                const std::string_view k = parts[i].substr(0, eq), v = parts[i].substr(eq + 1);
                if (k == "r") t.r = *text::parse_int<std::uint32_t>(v);
                else if (k == "kind") t.kind = std::string(v);
                else if (k == "default") t.default_target = hex32(v);
                else if (k == "guard") { if (v != "-") t.guard = hex32(v); }
                else if (k == "table") t.table = hex32(v);
                else if (k == "inline") t.inline_table = v == "1";
                else if (k == "func") { if (v != "-") t.func = {hex32(v.substr(0, v.find('-'))), hex32(v.substr(v.find('-') + 1))}; }
                else if (k == "labels") t.labels = hex_list(v);
            }
            s.tables.push_back(std::move(t));
        } else if (key == "report") {
            for (std::size_t i = 1; i < parts.size(); ++i) {
                const auto eq = parts[i].find('=');
                const std::string_view k = parts[i].substr(0, eq), v = parts[i].substr(eq + 1);
                if (k == "unresolved") s.unresolved = hex_list(v);
                else if (k == "outside") s.outside = hex_list(v);
                else if (k == "pointers") s.pointers = hex_list(v);
            }
        } else if (key == "jt") {
            ExpectedJump j;
            j.bctr = hex32(parts[1]);
            j.reg = *text::parse_int<std::uint32_t>(parts[2].substr(4));
            j.targets = hex_list(parts[3].substr(8));
            s.jumps.push_back(std::move(j));
        }
    }
    return out;
}

}  // namespace cb::recompile::test
