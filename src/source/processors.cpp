// source/processors.cpp -- the processor table (source/processors.hpp): each row joins the decompiler's instruction tables
// (decompile/<cpu>), the assemblers (build/<cpu>_asm, build/x86asm) and the encoders the source writers use (source/isa*).

#include "source/processors.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <stdexcept>

#include "build/a64_asm.hpp"
#include "build/arm_asm.hpp"
#include "build/cp1610_asm.hpp"
#include "build/dasm_asm.hpp"
#include "build/f8_asm.hpp"
#include "build/i8048_asm.hpp"
#include "build/m6809_asm.hpp"
#include "build/m68k_asm.hpp"
#include "build/mips_asm.hpp"
#include "build/ppc_asm.hpp"
#include "build/sh_asm.hpp"
#include "build/sm83_asm.hpp"
#include "build/tlcs900_asm.hpp"
#include "build/v810_asm.hpp"
#include "build/x86asm/x86asm.hpp"
#include "build/z80_asm.hpp"
#include "build/asm_text.hpp"
#include "base/config.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/a64.hpp"
#include "decompile/engine/sleigh_reader.hh"
#include "decompile/arm.hpp"
#include "decompile/cp1610.hpp"
#include "decompile/f8.hpp"
#include "decompile/huc6280.hpp"
#include "decompile/i8048.hpp"
#include "decompile/m6502.hpp"
#include "decompile/m6809.hpp"
#include "decompile/m68k.hpp"
#include "decompile/mips.hpp"
#include "decompile/ppc.hpp"
#include "decompile/sh2.hpp"
#include "decompile/sm83.hpp"
#include "decompile/tlcs900.hpp"
#include "decompile/v810.hpp"
#include "decompile/w65816.hpp"
#include "decompile/z80.hpp"
#include "platforms/decoder.hpp"
#include "source/isa.hpp"
#include "source/isa_65xx.hpp"
#include "source/isa_mips_le.hpp"
#include "source/isa_r4300.hpp"
#include "source/isa_sh2.hpp"
#include "source/isa_sleigh.hpp"

namespace cb::source::processors {
namespace {

// A byte (or a fixed-width unit) that is no instruction, as data the assembler reads back.
std::string data_text(ByteView bytes) {
    std::string out = bytes.size() == 1 ? ".db " : bytes.size() == 2 ? ".dw " : ".db ";
    if (bytes.size() == 2) return out + std::format("${:02X}{:02X}", bytes[0], bytes[1]);
    for (std::size_t k = 0; k < bytes.size(); ++k) out += std::format("{}${:02X}", k ? "," : "", bytes[k]);
    return out;
}

// The instructions of `code` read straight on: `decode(code, at, pc)` gives one (or nullopt), `size(insn)` its length in bytes,
// `text(insn)` its text; a place that does not decode is `unit` bytes of data.
template <class Decode, class Size, class Text>
std::vector<Line> linear(ByteView code, std::uint64_t base, std::size_t count, std::size_t unit, Decode decode, Size size, Text text) {
    std::vector<Line> out;
    for (std::size_t at = 0; at < code.size() && (count == 0 || out.size() < count);) {
        const std::uint64_t pc = base + at;
        auto insn = decode(code, at, pc);
        std::size_t n = insn ? static_cast<std::size_t>(size(*insn)) : 0;
        Line line;
        line.address = pc;
        if (insn && n != 0 && at + n <= code.size()) {
            line.text = text(*insn);
        } else {
            n = std::min(unit, code.size() - at);
            line.text = data_text(code.subspan(at, n));
        }
        line.bytes.assign(code.begin() + static_cast<std::ptrdiff_t>(at), code.begin() + static_cast<std::ptrdiff_t>(at + n));
        out.push_back(std::move(line));
        at += n;
    }
    return out;
}

std::uint32_t pc32(std::uint64_t pc) { return static_cast<std::uint32_t>(pc); }

// x86 through Capstone, at 16, 32 or 64 bits.
std::vector<Line> x86_lines(unsigned bits, ByteView code, std::uint64_t base, std::size_t count) {
    const platforms::Decoder decoder = platforms::Decoder::open(platforms::Arch::X86, bits, false);
    std::vector<Line> out;
    for (std::size_t at = 0; at < code.size() && (count == 0 || out.size() < count);) {
        const std::vector<platforms::Insn> one = decoder.disasm(code.subspan(at), base + at, 1);
        Line line;
        line.address = base + at;
        std::size_t n = 1;
        if (!one.empty() && one[0].size != 0) {
            n = one[0].size;
            line.text = one[0].op_str.empty() ? one[0].mnemonic : one[0].mnemonic + " " + one[0].op_str;
        } else {
            line.text = data_text(code.subspan(at, 1));
        }
        line.bytes.assign(code.begin() + static_cast<std::ptrdiff_t>(at), code.begin() + static_cast<std::ptrdiff_t>(at + n));
        out.push_back(std::move(line));
        at += n;
    }
    return out;
}

// x86 through x86asm (NASM's syntax, or MASM's with `masm`): `source` as a flat binary, `bits` the mode it starts in.
Image x86_image(unsigned bits, bool masm, std::string_view source, const std::filesystem::path& folder) {
    build::x86asm::Request r;
    r.args = {"-f", "bin", "-o", "out.bin", "in.asm"};
    if (masm) r.args.insert(r.args.begin(), "--masm");
    r.cwd = "/";
    const std::string text = (masm ? std::string() : std::format("bits {}\n", bits)) + std::string(source);
    r.files["/in.asm"] = Bytes(text.begin(), text.end());
    r.read = [folder](const std::string& name) -> std::optional<Bytes> {
        std::filesystem::path p = folder / std::filesystem::path(name).relative_path();
        std::error_code ec;
        if (!std::filesystem::is_regular_file(p, ec)) p = folder / std::filesystem::path(name).filename();
        if (!std::filesystem::is_regular_file(p, ec)) return std::nullopt;
        return cb::fs::read_file(p);
    };
    build::x86asm::Result out = build::x86asm::assemble(r);
    if (out.exit != 0 || !out.files.contains("out.bin")) throw build::AsmError(std::string(text::trim(out.messages)));
    return {0, std::move(out.files.at("out.bin"))};
}

// A two-byte-wide processor's words, big-endian (the CP1610's decles as the Intellivision's ROM files hold them).
std::vector<std::uint16_t> words_be(ByteView code) {
    std::vector<std::uint16_t> w(code.size() / 2);
    for (std::size_t k = 0; k < w.size(); ++k) w[k] = static_cast<std::uint16_t>(code[2 * k] << 8 | code[2 * k + 1]);
    return w;
}

std::vector<Line> cp1610_lines(ByteView code, std::uint64_t base, std::size_t count) {
    const std::vector<std::uint16_t> words = words_be(code);
    std::vector<Line> out;
    bool sdbd = false;
    for (std::size_t at = 0; at < words.size() && (count == 0 || out.size() < count);) {
        const std::uint32_t pc = static_cast<std::uint32_t>(base + at);
        const std::optional<decompile::cp1610::Insn> insn = decompile::cp1610::decode(words, at, pc, sdbd);
        Line line;
        line.address = pc;
        std::size_t n = insn && insn->size != 0 && at + insn->size <= words.size() ? insn->size : 1;
        line.text = insn && insn->size != 0 ? decompile::cp1610::text(*insn) : std::format(".dw ${:04X}", words[at]);
        sdbd = line.text == "SDBD";
        line.bytes.assign(code.begin() + static_cast<std::ptrdiff_t>(2 * at), code.begin() + static_cast<std::ptrdiff_t>(2 * (at + n)));
        out.push_back(std::move(line));
        at += n;
    }
    return out;
}

Image bytes_at(Bytes b) { return {0, std::move(b)}; }

std::vector<Processor> make_table() {
    namespace d = decompile;
    namespace b = build;
    using Folder = const std::filesystem::path&;
    std::vector<Processor> t;
    auto add = [&](Processor p) { t.push_back(std::move(p)); };

    add({"x86-16", {"x86", "8086", "i86", "real-mode"}, "Intel 8086 and up in real mode: DOS, Win16, VxD real-mode parts", "NASM (`--masm` for MASM)",
         [](ByteView c, std::uint64_t base, std::size_t n) { return x86_lines(16, c, base, n); },
         [](std::string_view s, Folder f) { return x86_image(16, false, s, f); }});
    add({"x86-32", {"i386", "x86_32", "ia32", "win32"}, "Intel 80386 and up, 32-bit: Win32, VxD, DOS extenders", "NASM",
         [](ByteView c, std::uint64_t base, std::size_t n) { return x86_lines(32, c, base, n); },
         [](std::string_view s, Folder f) { return x86_image(32, false, s, f); }});
    add({"x86-64", {"amd64", "x64", "x86_64"}, "AMD64 / Intel 64", "NASM",
         [](ByteView c, std::uint64_t base, std::size_t n) { return x86_lines(64, c, base, n); },
         [](std::string_view s, Folder f) { return x86_image(64, false, s, f); }});
    add({"masm", {"x86-masm"}, "x86 in MASM's syntax (x86asm's MASM mode)", "MASM",
         [](ByteView c, std::uint64_t base, std::size_t n) { return x86_lines(16, c, base, n); },
         [](std::string_view s, Folder f) { return x86_image(16, true, s, f); }});
    add({"6502", {"m6502", "nmos6502", "nes", "atari2600", "2600"}, "MOS 6502: NES, Atari 2600/7800, Commodore", "ca65 to read, DASM to assemble",
         [](ByteView c, std::uint64_t base, std::size_t n) {
             return linear(c, base, n, 1, [](ByteView c, std::size_t at, std::uint64_t pc) { return d::m6502::decode(c, at, pc32(pc)); },
                           [](const auto& i) { return i.size; }, [](const auto& i) { return d::m6502::text(i); });
         },
         [](std::string_view s, Folder f) { return bytes_at(b::dasm_asm::Assembler(isa::m6502_encoder()).build(s, f)); }});
    add({"65c02", {"cmos6502", "65sc02", "lynx"}, "the CMOS 6502 (65C02 / 65SC02): Atari Lynx, Apple IIc", "ca65 to read, DASM to assemble",
         [](ByteView c, std::uint64_t base, std::size_t n) {
             return linear(c, base, n, 1, [](ByteView c, std::size_t at, std::uint64_t pc) { return d::m6502::decode(c, at, pc32(pc), d::m6502::Variant::Cmos); },
                           [](const auto& i) { return i.size; }, [](const auto& i) { return d::m6502::text(i); });
         },
         [](std::string_view s, Folder f) { return bytes_at(b::dasm_asm::Assembler(isa::m65sc02_encoder()).build(s, f)); }});
    add({"huc6280", {"pce", "pcengine", "turbografx"}, "Hudson HuC6280: PC Engine / TurboGrafx-16", "ca65",
         [](ByteView c, std::uint64_t base, std::size_t n) {
             return linear(c, base, n, 1, [](ByteView c, std::size_t at, std::uint64_t pc) { return d::huc6280::decode(c, at, pc32(pc)); },
                           [](const auto& i) { return i.size; }, [](const auto& i) { return d::huc6280::text(i); });
         },
         [](std::string_view s, Folder f) { return bytes_at(b::dasm_asm::Assembler(isa::huc6280_encoder()).build(s, f)); }});
    add({"65816", {"w65816", "65c816", "snes"}, "WDC 65C816: SNES, Apple IIgs (read with 8-bit A and X)", "ca65",
         [](ByteView c, std::uint64_t base, std::size_t n) {
             return linear(c, base, n, 1, [](ByteView c, std::size_t at, std::uint64_t pc) { return d::w65816::decode(c, at, pc32(pc), false, false); },
                           [](const auto& i) { return i.size; }, [](const auto& i) { return d::w65816::text(i); });
         },
         [](std::string_view s, Folder f) { return bytes_at(b::dasm_asm::Assembler(isa::w65816_encoder()).build(s, f)); }});
    add({"z80", {"sms", "gamegear", "msx", "zx", "spectrum", "coleco", "cpc"}, "Zilog Z80: Master System, Game Gear, MSX, Spectrum", "WLA-DX / sjasmplus",
         [](ByteView c, std::uint64_t base, std::size_t n) {
             return linear(c, base, n, 1, [](ByteView c, std::size_t at, std::uint64_t pc) { return d::z80::decode(c, at, pc32(pc)); },
                           [](const auto& i) { return i.size; }, [](const auto& i) { return d::z80::text(i); });
         },
         [](std::string_view s, Folder f) {
             // a cartridge's source (WLA-DX's memory map) is the whole ROM; anything else is the bytes it writes, from where they go
             const b::z80_asm::Assembler assembler(isa::z80_encoder());
             const std::string up = text::upper(s);
             if (up.find(".MEMORYMAP") != std::string::npos || up.find(".ROMBANK") != std::string::npos) return bytes_at(assembler.build(s, f));
             b::z80_asm::Assembler::Flat flat = assembler.build_flat(s, f);
             return Image{flat.start, std::move(flat.bytes)};
         }});
    add({"sm83", {"gbz80", "gb", "gameboy", "lr35902"}, "Sharp SM83 (LR35902): Game Boy, Game Boy Color", "RGBDS",
         [](ByteView c, std::uint64_t base, std::size_t n) {
             return linear(c, base, n, 1, [](ByteView c, std::size_t at, std::uint64_t pc) { return d::sm83::decode(c, at, pc32(pc)); },
                           [](const auto& i) { return i.size; }, [](const auto& i) { return d::sm83::text(i); });
         },
         [](std::string_view s, Folder f) { return bytes_at(b::sm83_asm::build(s, 0x8000, f)); }});
    add({"m68k", {"68000", "68k", "genesis", "megadrive", "neogeo"}, "Motorola 68000: Genesis / Mega Drive, Neo Geo, Atari Jaguar's host", "vasm (Motorola)",
         [](ByteView c, std::uint64_t base, std::size_t n) {
             return linear(c, base, n, 2, [](ByteView c, std::size_t at, std::uint64_t pc) { return d::m68k::decode(c, at, pc32(pc)); },
                           [](const auto& i) { return i.length; }, [](const auto& i) { return d::m68k::text(i); });
         },
         [](std::string_view s, Folder) { return bytes_at(b::m68k_asm::Assembler(isa::m68k_encoder()).build(s)); }});
    add({"m6809", {"6809", "vectrex"}, "Motorola 6809: Vectrex", "asm6809",
         [](ByteView c, std::uint64_t base, std::size_t n) {
             return linear(c, base, n, 1, [](ByteView c, std::size_t at, std::uint64_t pc) { return d::m6809::decode(c, at, pc32(pc)); },
                           [](const auto& i) { return i.size; }, [](const auto& i) { return d::m6809::text(i); });
         },
         [](std::string_view s, Folder) { return bytes_at(b::m6809_asm::build(s)); }});
    add({"mips", {"r3000", "r3000a", "ps1", "psx"}, "MIPS R3000A (little-endian): PlayStation", "GNU as",
         [](ByteView c, std::uint64_t base, std::size_t n) {
             return linear(c, base, n, 4, [](ByteView c, std::size_t at, std::uint64_t pc) { return d::mips::decode(c, at, pc32(pc)); },
                           [](const auto&) { return 4; }, [](const auto& i) { return d::mips::text(i); });
         },
         [](std::string_view s, Folder) { return bytes_at(b::mips_asm::Assembler(isa::mips_encoder()).build(s)); }});
    add({"r4300", {"n64", "mips3"}, "MIPS R4300i (big-endian): Nintendo 64", "GNU as",
         [](ByteView c, std::uint64_t base, std::size_t n) {
             return linear(c, base, n, 4, [](ByteView c, std::size_t at, std::uint64_t pc) { return d::mips::decode(c, at, pc32(pc), d::mips::Variant::R4300); },
                           [](const auto&) { return 4; }, [](const auto& i) { return d::mips::text(i); });
         },
         [](std::string_view s, Folder) { return bytes_at(b::mips_asm::Assembler(isa::r4300_encoder(), b::mips_asm::ByteOrder::Big).build(s)); }});
    add({"r5900", {"ee", "ps2", "emotion-engine"}, "MIPS R5900 Emotion Engine: PlayStation 2", "GNU as",
         [](ByteView c, std::uint64_t base, std::size_t n) {
             return linear(c, base, n, 4, [](ByteView c, std::size_t at, std::uint64_t pc) { return d::mips::decode(c, at, pc32(pc), d::mips::Variant::R5900); },
                           [](const auto&) { return 4; }, [](const auto& i) { return d::mips::text(i); });
         },
         [](std::string_view s, Folder) { return bytes_at(b::mips_asm::Assembler(isa::r5900_encoder()).build(s)); }});
    add({"arm", {"armv4t", "arm7", "arm7tdmi", "gba"}, "ARM7TDMI (ARMv4T, ARM state): Game Boy Advance, the DS's second CPU", "GNU as",
         [](ByteView c, std::uint64_t base, std::size_t n) {
             return linear(c, base, n, 4, [](ByteView c, std::size_t at, std::uint64_t pc) { return d::arm::decode(c, at, pc32(pc)); },
                           [](const auto& i) { return i.length; }, [](const auto& i) { return d::arm::text(i); });
         },
         [](std::string_view s, Folder) { return bytes_at(b::arm_asm::Assembler(isa::arm_encoder()).build(s)); }});
    add({"thumb", {"armv4t-thumb", "gba-thumb"}, "ARM7TDMI in Thumb state", "GNU as",
         [](ByteView c, std::uint64_t base, std::size_t n) {
             return linear(c, base, n, 2, [](ByteView c, std::size_t at, std::uint64_t pc) { return d::arm::decode(c, at, pc32(pc) | 1u); },
                           [](const auto& i) { return i.length; }, [](const auto& i) { return d::arm::text(i); });
         },
         {}});
    add({"arm9", {"armv5te", "arm946", "nds"}, "ARM946E-S (ARMv5TE): Nintendo DS", "GNU as",
         [](ByteView c, std::uint64_t base, std::size_t n) {
             return linear(c, base, n, 4, [](ByteView c, std::size_t at, std::uint64_t pc) { return d::arm::decode(c, at, pc32(pc), d::arm::Arch::V5TE); },
                           [](const auto& i) { return i.length; }, [](const auto& i) { return d::arm::text(i); });
         },
         [](std::string_view s, Folder) { return bytes_at(b::arm_asm::Assembler(isa::arm9_encoder()).build(s)); }});
    add({"arm11", {"armv6k", "3ds"}, "ARM11 MPCore (ARMv6K): Nintendo 3DS", "GNU as",
         [](ByteView c, std::uint64_t base, std::size_t n) {
             return linear(c, base, n, 4, [](ByteView c, std::size_t at, std::uint64_t pc) { return d::arm::decode(c, at, pc32(pc), d::arm::Arch::V6K); },
                           [](const auto& i) { return i.length; }, [](const auto& i) { return d::arm::text(i); });
         },
         [](std::string_view s, Folder) { return bytes_at(b::arm_asm::Assembler(isa::arm11_encoder()).build(s)); }});
    add({"armv7", {"armv7a", "cortex-a9", "vita"}, "ARMv7-A (Cortex-A9): PlayStation Vita", "GNU as",
         [](ByteView c, std::uint64_t base, std::size_t n) {
             return linear(c, base, n, 4, [](ByteView c, std::size_t at, std::uint64_t pc) { return d::arm::decode(c, at, pc32(pc), d::arm::Arch::V7A); },
                           [](const auto& i) { return i.length; }, [](const auto& i) { return d::arm::text(i); });
         },
         [](std::string_view s, Folder) { return bytes_at(b::arm_asm::Assembler(isa::armv7_encoder()).build(s)); }});
    add({"a64", {"arm64", "aarch64", "switch"}, "ARMv8-A AArch64: Nintendo Switch", "GNU as",
         [](ByteView c, std::uint64_t base, std::size_t n) {
             return linear(c, base, n, 4, [](ByteView c, std::size_t at, std::uint64_t pc) { return d::a64::decode(c, at, pc); },
                           [](const auto&) { return 4; }, [](const auto& i) { return d::a64::text(i); });
         },
         [](std::string_view s, Folder f) {
             b::a64_asm::Image i = b::a64_asm::build(s, f);
             return Image{i.base, std::move(i.data)};
         }});
    add({"ppc", {"powerpc", "ppc32", "gekko", "broadway", "gamecube", "wii", "xenon", "x360"}, "PowerPC (32-bit, big-endian): GameCube, Wii, Xbox 360", "GNU as",
         [](ByteView c, std::uint64_t base, std::size_t n) {
             return linear(c, base, n, 4, [](ByteView c, std::size_t at, std::uint64_t pc) { return d::ppc::decode(c, at, pc32(pc)); },
                           [](const auto&) { return 4; }, [](const auto& i) { return d::ppc::text(i); });
         },
         [](std::string_view s, Folder) { return bytes_at(b::ppc_asm::assemble(s)); }});
    add({"sh2", {"superh", "sh-2", "32x", "saturn"}, "Hitachi SH-2 (big-endian): Sega 32X, Saturn", "GNU as",
         [](ByteView c, std::uint64_t base, std::size_t n) {
             return linear(c, base, n, 2, [](ByteView c, std::size_t at, std::uint64_t pc) { return d::sh2::decode(c, at, pc32(pc)); },
                           [](const auto&) { return 2; }, [](const auto& i) { return d::sh2::text(i); });
         },
         [](std::string_view s, Folder) { return bytes_at(b::sh_asm::Assembler(isa::sh2_encoder()).build(s)); }});
    add({"sh4", {"sh-4", "dreamcast"}, "Hitachi SH-4 (little-endian): Dreamcast", "GNU as",
         [](ByteView c, std::uint64_t base, std::size_t n) {
             return linear(c, base, n, 2, [](ByteView c, std::size_t at, std::uint64_t pc) { return d::sh2::decode(c, at, pc32(pc), d::sh2::Variant::Sh4); },
                           [](const auto&) { return 2; }, [](const auto& i) { return d::sh2::text(i); });
         },
         [](std::string_view s, Folder) { return bytes_at(b::sh_asm::Assembler(isa::sh4_encoder(), true).build(s)); }});
    add({"v810", {"nec-v810", "virtualboy", "vb"}, "NEC V810: Virtual Boy", "GNU as (V810)",
         [](ByteView c, std::uint64_t base, std::size_t n) {
             return linear(c, base, n, 2, [](ByteView c, std::size_t at, std::uint64_t pc) { return d::v810::decode(c, at, pc32(pc)); },
                           [](const auto& i) { return i.size; }, [](const auto& i) { return d::v810::text(i); });
         },
         [](std::string_view s, Folder) {
             b::v810_asm::Image i = b::v810_asm::build(s);
             return Image{i.base, std::move(i.data)};
         }});
    add({"cp1610", {"intellivision", "intv"}, "General Instrument CP1610: Intellivision (16-bit words, big-endian in the file)", "as1600",
         cp1610_lines, [](std::string_view s, Folder) { return bytes_at(b::cp1610_asm::build(s)); }});
    add({"i8048", {"8048", "odyssey2", "o2"}, "Intel 8048: Magnavox Odyssey 2", "asl",
         [](ByteView c, std::uint64_t base, std::size_t n) {
             return linear(c, base, n, 1, [](ByteView c, std::size_t at, std::uint64_t pc) { return d::i8048::decode(c, at, pc32(pc)); },
                           [](const auto& i) { return i.size; }, [](const auto& i) { return d::i8048::text(i); });
         },
         [](std::string_view s, Folder) {
             b::byte_asm::Image i = b::i8048_asm::build(s);
             return Image{i.base, std::move(i.data)};
         }});
    add({"f8", {"fairchild-f8", "channelf", "chf"}, "Fairchild F8: Channel F", "dasm (F8)",
         [](ByteView c, std::uint64_t base, std::size_t n) {
             return linear(c, base, n, 1, [](ByteView c, std::size_t at, std::uint64_t pc) { return d::f8::decode(c, at, pc32(pc)); },
                           [](const auto& i) { return i.size; }, [](const auto& i) { return d::f8::text(i); });
         },
         [](std::string_view s, Folder) {
             b::byte_asm::Image i = b::f8_asm::build(s);
             return Image{i.base, std::move(i.data)};
         }});
    add({"tlcs900", {"tlcs-900h", "ngp", "neogeopocket"}, "Toshiba TLCS-900/H: Neo Geo Pocket", "asl",
         [](ByteView c, std::uint64_t base, std::size_t n) {
             return linear(c, base, n, 1, [](ByteView c, std::size_t at, std::uint64_t pc) { return d::tlcs900::decode(c, at, pc32(pc)); },
                           [](const auto& i) { return i.size; }, [](const auto& i) { return d::tlcs900::text(i); });
         },
         [](std::string_view s, Folder) {
             b::byte_asm::Image i = b::tlcs900_asm::build(s);
             return Image{i.base, std::move(i.data)};
         }});
    // each processor's SLEIGH description (languages/), with the context its pspec starts the processor in
    struct Described {
        std::string_view name, sla;
        std::vector<std::pair<std::string_view, int>> context;
        bool big_endian = false;
    };
    const Described described[] = {
        {"x86-16", "x86.sla", {{"addrsize", 0}, {"opsize", 0}}}, {"x86-32", "x86.sla", {{"addrsize", 1}, {"opsize", 1}}},
        {"masm", "x86.sla", {{"addrsize", 0}, {"opsize", 0}}},   {"6502", "6502.sla", {}},
        {"65c02", "65c02.sla", {}},                               {"z80", "z80.sla", {}},
        {"m68k", "68020.sla", {}, true},                          {"m6809", "6809.sla", {}, true},
        {"mips", "mips32le.sla", {}},                             {"r4300", "mips64be.sla", {}, true},
        {"r5900", "mips64le.sla", {}},                            {"arm", "ARM4t_le.sla", {{"TMode", 0}}},
        {"thumb", "ARM4t_le.sla", {{"TMode", 1}}},                {"arm9", "ARM5t_le.sla", {{"TMode", 0}}},
        {"arm11", "ARM6_le.sla", {{"TMode", 0}}},                 {"armv7", "ARM7_le.sla", {{"TMode", 0}}},
        {"a64", "AARCH64.sla", {}},                               {"ppc", "ppc_32_be.sla", {}, true},
        {"sh2", "sh-2.sla", {}, true},                            {"sh4", "SuperH4_le.sla", {}},
        {"i8048", "8048.sla", {}},                                {"cp1610", "CP1600.sla", {}, true},
        {"sm83", "sm83.sla", {}},                                 {"huc6280", "huc6280.sla", {}},
        {"65816", "65816.sla", {}},
    };
    for (const Described& d : described)
        for (Processor& p : t)
            if (p.name == d.name) p.sla = d.sla, p.context = d.context, p.big_endian = d.big_endian;
    return t;
}

// The decompiler core's language files: CB_LANGUAGES_DIR, or the checkout's (decompile/decompiler.cpp finds them the same way).
std::filesystem::path languages() {
    if (const std::optional<std::string> env = cb::fs::get_env("CB_LANGUAGES_DIR"); env && !env->empty()) return cb::fs::from_text(*env);
    return config::current().checkout_root() / "src" / "decompile" / "engine" / "languages";
}

}  // namespace

std::vector<Line> read_described(const Processor& processor, ByteView code, std::uint64_t base, std::size_t count) {
    if (processor.sla.empty()) throw UsageError(std::format("{} has no SLEIGH description yet", processor.name));
    // one reader per description, kept (reading a description costs more than any listing)
    static std::mutex lock;
    static std::map<std::string, std::unique_ptr<decompile::sleigh::Reader>> readers;
    decompile::sleigh::Reader* reader = nullptr;
    {
        std::lock_guard hold(lock);
        std::unique_ptr<decompile::sleigh::Reader>& r = readers[std::string(processor.name)];
        if (!r) {
            std::vector<std::pair<std::string, int>> context;
            for (const auto& [k, v] : processor.context) context.emplace_back(std::string(k), v);
            try {
                r = std::make_unique<decompile::sleigh::Reader>(languages() / processor.sla, context);
            } catch (const std::runtime_error& e) {
                throw NotFoundError(languages() / processor.sla, 0, e.what());
            }
        }
        reader = r.get();
    }
    std::vector<Line> out;
    const std::size_t unit = processor.name == "cp1610" ? 2 : 1;   // (a word-addressed processor: addresses count words)
    for (const decompile::sleigh::Instruction& i : reader->read(code.data(), code.size(), base, count)) {
        Line line;
        line.address = i.address;
        const std::size_t at = static_cast<std::size_t>((i.address - base) * unit);
        const std::size_t n = i.length != 0 ? i.length : std::min(std::max<std::size_t>(reader->alignment(), 1) * unit, code.size() - at);
        line.bytes.assign(code.begin() + static_cast<std::ptrdiff_t>(at), code.begin() + static_cast<std::ptrdiff_t>(at + n));
        line.text = i.length != 0 ? (i.operands.empty() ? i.mnemonic : i.mnemonic + " " + i.operands) : data_text(code.subspan(at, n));
        out.push_back(std::move(line));
    }
    return out;
}

Image assemble_described(const Processor& processor, std::string_view source, const std::filesystem::path& folder) {
    if (processor.sla.empty()) throw UsageError(std::format("{} has no SLEIGH description yet", processor.name));
    std::vector<std::pair<std::string, int>> context;
    for (const auto& [k, v] : processor.context) context.emplace_back(std::string(k), v);
    const build::Encoder& encoder = isa::sleigh_encoder(languages() / processor.sla, context);
    (void)folder;
    return bytes_at(build::mips_asm::Assembler(encoder, processor.big_endian ? build::mips_asm::ByteOrder::Big : build::mips_asm::ByteOrder::Little, 1, ';').build(source));
}

std::string source_described(const Processor& processor, ByteView code, std::uint64_t base) {
    std::vector<std::pair<std::string, int>> context;
    for (const auto& [k, v] : processor.context) context.emplace_back(std::string(k), v);
    const build::Encoder& encoder = isa::sleigh_encoder(languages() / processor.sla, context);
    const build::Labels none;
    auto raw = [](const Line& l) {
        std::string s = "\t.byte ";
        for (std::size_t k = 0; k < l.bytes.size(); ++k) s += std::format("{}0x{:02x}", k ? "," : "", l.bytes[k]);
        return s;
    };
    std::string out = std::format("; {} ({}), {} bytes from 0x{:x}\n\t.org 0x{:x}\n", processor.name, processor.sla, code.size(), base, base);
    for (const Line& l : read_described(processor, code, base, 0)) {
        if (l.text.starts_with(".d")) {
            out += raw(l) + "\n";
            continue;
        }
        const Result<Bytes> again = encoder.assemble(l.text, static_cast<std::int64_t>(l.address), none);
        if (again.ok() && again.value() == l.bytes) out += "\t" + l.text + "\n";
        else out += raw(l) + "\t; " + l.text + "\n";
    }
    return out;
}

std::span<const Processor> all() {
    static const std::vector<Processor> table = make_table();
    return table;
}

const Processor* lookup(std::string_view name) {
    const std::string want = text::lower(name);
    for (const Processor& p : all()) {
        if (p.name == want) return &p;
        if (std::find(p.aliases.begin(), p.aliases.end(), want) != p.aliases.end()) return &p;
    }
    return nullptr;
}

const Processor& find(std::string_view name) {
    if (const Processor* p = lookup(name)) return *p;
    std::string names;
    for (const Processor& p : all()) names += (names.empty() ? "" : ", ") + std::string(p.name);
    throw UsageError(std::format("no processor is named '{}' (the processors: {})", name, names));
}

}  // namespace cb::source::processors
