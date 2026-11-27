// source/processors.cpp -- the processor table (source/processors.hpp): every row but x86's reads and assembles through its SLEIGH description
// (read_described, assemble_described: the one disassembler and the one assembler); the x86 rows keep Capstone and NASM / MASM (x86asm).

#include "source/processors.hpp"

#include <algorithm>
#include <condition_variable>
#include <format>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <unordered_set>

#include "build/mips_asm.hpp"
#include "build/x86asm/x86asm.hpp"
#include "build/asm_text.hpp"
#include "base/config.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/parallel.hpp"
#include "core/text.hpp"
#include "decompile/engine/sleigh_reader.hh"
#include "platforms/decoder.hpp"
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

Image bytes_at(Bytes b) { return {0, std::move(b)}; }

std::vector<Processor> make_table() {
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
    // every row below the x86 ones is read and assembled through its SLEIGH description (the one disassembler, the one assembler)
    auto described_row = [&](std::string_view name, std::vector<std::string_view> aliases, std::string_view summary) {
        add({name, std::move(aliases), summary, "the processor's SLEIGH description (the one assembler)",
             [name](ByteView c, std::uint64_t base, std::size_t n) { return read_described(find(name), c, base, n); },
             [name](std::string_view s, Folder f) { return assemble_described(find(name), s, f); }});
    };
    described_row("6502", {"m6502", "nmos6502", "nes", "atari2600", "2600"}, "MOS 6502: NES, Atari 2600/7800, Commodore");
    described_row("65c02", {"cmos6502", "65sc02", "lynx"}, "the CMOS 6502 (65C02 / 65SC02): Atari Lynx, Apple IIc");
    described_row("huc6280", {"pce", "pcengine", "turbografx"}, "Hudson HuC6280: PC Engine / TurboGrafx-16");
    described_row("65816", {"w65816", "65c816", "snes"}, "WDC 65C816: SNES, Apple IIgs (read with 8-bit A and X; `.context m16=1 x16=1` widens them)");
    described_row("z80", {"sms", "gamegear", "msx", "zx", "spectrum", "coleco", "cpc"}, "Zilog Z80: Master System, Game Gear, MSX, Spectrum");
    described_row("sm83", {"gbz80", "gb", "gameboy", "lr35902"}, "Sharp SM83 (LR35902): Game Boy, Game Boy Color");
    described_row("m68k", {"68000", "68k", "genesis", "megadrive", "neogeo"}, "Motorola 68000: Genesis / Mega Drive, Neo Geo, Atari Jaguar's host");
    described_row("m6809", {"6809", "vectrex"}, "Motorola 6809: Vectrex");
    described_row("mips", {"r3000", "r3000a", "ps1", "psx"}, "MIPS R3000A (little-endian): PlayStation");
    described_row("r4300", {"n64", "mips3"}, "MIPS R4300i (big-endian): Nintendo 64");
    described_row("r5900", {"ee", "ps2", "emotion-engine"}, "MIPS R5900 Emotion Engine: PlayStation 2");
    described_row("arm", {"armv4t", "arm7", "arm7tdmi", "gba"}, "ARM7TDMI (ARMv4T, ARM state): Game Boy Advance, the DS's second CPU");
    described_row("thumb", {"armv4t-thumb", "gba-thumb"}, "ARM7TDMI in Thumb state");
    described_row("arm9", {"armv5te", "arm946", "nds"}, "ARM946E-S (ARMv5TE): Nintendo DS");
    described_row("thumb9", {"armv5te-thumb", "nds-thumb"}, "ARM946E-S (ARMv5TE) in Thumb state");
    described_row("arm11", {"armv6k", "3ds"}, "ARM11 MPCore (ARMv6K): Nintendo 3DS");
    described_row("armv7", {"armv7a", "cortex-a9", "vita"}, "ARMv7-A (Cortex-A9): PlayStation Vita");
    described_row("armv7t", {"armv7a-thumb", "vita-thumb"}, "ARMv7-A (Cortex-A9) in Thumb state (Thumb-2): PlayStation Vita");
    described_row("a64", {"arm64", "aarch64", "switch"}, "ARMv8-A AArch64: Nintendo Switch");
    described_row("ppc", {"powerpc", "ppc32", "gekko", "broadway", "gamecube", "wii", "xenon", "x360"}, "PowerPC (32-bit, big-endian): GameCube, Wii, Xbox 360");
    described_row("sh2", {"superh", "sh-2", "32x", "saturn"}, "Hitachi SH-2 (big-endian): Sega 32X, Saturn");
    described_row("sh4", {"sh-4", "dreamcast"}, "Hitachi SH-4 (little-endian): Dreamcast");
    described_row("v810", {"nec-v810", "virtualboy", "vb"}, "NEC V810: Virtual Boy");
    described_row("cp1610", {"intellivision", "intv"}, "General Instrument CP1610: Intellivision (16-bit words, big-endian in the file)");
    described_row("i8048", {"8048", "odyssey2", "o2"}, "Intel 8048: Magnavox Odyssey 2");
    described_row("f8", {"fairchild-f8", "channelf", "chf"}, "Fairchild F8: Channel F");
    described_row("tlcs900", {"tlcs-900h", "ngp", "neogeopocket"}, "Toshiba TLCS-900/H: Neo Geo Pocket");
    // each processor's SLEIGH description (languages/), with the context its pspec starts the processor in
    struct Described {
        std::string_view name, sla;
        std::vector<std::pair<std::string_view, int>> context;
        bool big_endian = false;
    };
    const std::map<std::string_view, std::vector<std::string_view>> halts = {
        // (a Z80 or SM83 HALT, an SH SLEEP, a 6809 SYNC wait for an interrupt and go on after it: code continues)
        {"6502", {"brk"}}, {"65c02", {"brk", "stp"}}, {"huc6280", {"brk"}}, {"65816", {"brk", "stp"}},
        {"sm83", {"stop", ".db"}}, {"m68k", {"stop"}}, {"cp1610", {"hlt"}},   // (the SM83's ".db": STOP with a second byte that is not 0)
    };
    const Described described[] = {
        {"x86-16", "x86.sla", {{"addrsize", 0}, {"opsize", 0}}}, {"x86-32", "x86.sla", {{"addrsize", 1}, {"opsize", 1}}},
        {"masm", "x86.sla", {{"addrsize", 0}, {"opsize", 0}}},   {"6502", "6502.sla", {}},
        {"65c02", "65c02.sla", {}},                               {"z80", "z80.sla", {}},
        {"m68k", "68020.sla", {}, true},                          {"m6809", "6809.sla", {}, true},
        {"mips", "mips32le.sla", {}},                             {"r4300", "mips64be.sla", {}, true},
        {"r5900", "mips64le.sla", {}},                            {"arm", "ARM4t_le.sla", {{"TMode", 0}}},
        {"thumb", "ARM4t_le.sla", {{"TMode", 1}}},                {"arm9", "ARM5t_le.sla", {{"TMode", 0}}}, {"thumb9", "ARM5t_le.sla", {{"TMode", 1}}},
        {"arm11", "ARM6_le.sla", {{"TMode", 0}}},                 {"armv7", "ARM7_le.sla", {{"TMode", 0}}}, {"armv7t", "ARM7_le.sla", {{"TMode", 1}}},
        {"a64", "AARCH64.sla", {}},                               {"ppc", "ppc_32_be.sla", {}, true},
        {"sh2", "sh-2.sla", {}, true},                            {"sh4", "SuperH4_le.sla", {}},
        {"i8048", "8048.sla", {}},                                {"cp1610", "CP1600.sla", {}, true},
        {"sm83", "sm83.sla", {}},                                 {"huc6280", "huc6280.sla", {}},
        {"65816", "65816.sla", {}},                               {"v810", "v810.sla", {}},
        {"f8", "f8.sla", {}},                                     {"tlcs900", "tlcs900.sla", {}},
    };
    const std::map<std::string_view, std::string_view> languages = {
        {"6502", "6502:LE:16:default:default"},   {"65c02", "65C02:LE:16:default:default"}, {"huc6280", "HuC6280:LE:16:default:default"},
        {"65816", "65816:LE:24:default:default"}, {"z80", "z80:LE:16:default:default"},    {"sm83", "sm83:LE:16:default:default"},
        {"m68k", "68000:BE:32:MC68020:default"},  {"m6809", "6809:BE:16:default:default"},  {"mips", "MIPS:LE:32:default:default"},
        {"r4300", "MIPS:BE:64:default:default"},  {"r5900", "MIPS:LE:64:default:default"},  {"arm", "ARM:LE:32:v4t:default"},
        {"arm9", "ARM:LE:32:v5t:default"},        {"arm11", "ARM:LE:32:v6:default"},        {"armv7", "ARM:LE:32:v7:default"},
        {"a64", "AARCH64:LE:64:v8A:default"},     {"ppc", "PowerPC:BE:32:default:default"}, {"sh2", "SuperH:BE:32:SH-2:default"},
        {"sh4", "SuperH4:LE:32:default:default"}, {"v810", "V810:LE:32:default:default"},   {"i8048", "8048:LE:16:default:default"},
        {"cp1610", "CP1600:BE:16:default:default"}, {"f8", "F8:BE:16:default:default"},     {"tlcs900", "TLCS900H:LE:24:default:default"},
    };
    for (const Described& d : described)
        for (Processor& p : t)
            if (p.name == d.name) {
                p.sla = d.sla, p.context = d.context, p.big_endian = d.big_endian;
                if (const auto h = halts.find(p.name); h != halts.end()) p.halts = h->second;
                if (p.name == "m68k") p.trap = {"trap", 0x80, 4};
                if (const auto l = languages.find(p.name); l != languages.end()) p.language = l->second;
            }
    return t;
}

// The decompiler core's language files: CB_LANGUAGES_DIR, or the checkout's (decompile/decompiler.cpp finds them the same way).
std::filesystem::path languages() {
    if (const std::optional<std::string> env = cb::fs::get_env("CB_LANGUAGES_DIR"); env && !env->empty()) return cb::fs::from_text(*env);
    return config::current().checkout_root() / "src" / "decompile" / "engine" / "languages";
}

}  // namespace

std::vector<Line> read_described(const Processor& processor, ByteView code, std::uint64_t base, std::size_t count) {
    return read_described(processor, code, base, count, {});
}

std::vector<Line> read_described(const Processor& processor, ByteView code, std::uint64_t base, std::size_t count,
                                 const std::vector<std::pair<std::string_view, int>>& over) {
    if (processor.sla.empty()) throw UsageError(std::format("{} has no SLEIGH description yet", processor.name));
    // readers per description, kept (reading a description costs more than any listing). A reader holds the translator's state between
    // instructions, so it reads for one caller at a time: each thread that reads at once has one of its own (made when first needed, up to the
    // worker count), and a writer's threads list side by side.
    struct Pool {
        std::mutex lock;
        std::condition_variable wake;
        std::vector<std::unique_ptr<decompile::sleigh::Reader>> all;
        std::vector<decompile::sleigh::Reader*> free;
        std::size_t made = 0;
    };
    static std::mutex pools_lock;
    static std::map<std::string, std::unique_ptr<Pool>> pools;
    Pool* pool = nullptr;
    {
        std::lock_guard hold(pools_lock);
        std::unique_ptr<Pool>& p = pools[std::string(processor.name)];
        if (!p) p = std::make_unique<Pool>();
        pool = p.get();
    }
    const std::size_t limit = static_cast<std::size_t>(std::max(1, parallel::workers()));
    decompile::sleigh::Reader* reader = nullptr;
    {
        std::unique_lock hold(pool->lock);
        for (;;) {
            if (!pool->free.empty()) {
                reader = pool->free.back();
                pool->free.pop_back();
                break;
            }
            if (pool->made < limit) {
                ++pool->made;
                hold.unlock();
                std::unique_ptr<decompile::sleigh::Reader> fresh;
                try {
                    std::vector<std::pair<std::string, int>> context;
                    for (const auto& [k, v] : processor.context) context.emplace_back(std::string(k), v);
                    std::lock_guard one(isa::loading_lock());
                    fresh = std::make_unique<decompile::sleigh::Reader>(languages() / processor.sla, context);
                } catch (const std::runtime_error& e) {
                    std::lock_guard again(pool->lock);
                    --pool->made;
                    pool->wake.notify_one();
                    throw NotFoundError(languages() / processor.sla, 0, e.what());
                } catch (...) {
                    std::lock_guard again(pool->lock);
                    --pool->made;
                    pool->wake.notify_one();
                    throw;
                }
                hold.lock();
                pool->all.push_back(std::move(fresh));
                reader = pool->all.back().get();
                break;
            }
            pool->wake.wait(hold);
        }
    }
    struct Return {   // (the reader goes back to the pool however this function ends)
        Pool& pool;
        decompile::sleigh::Reader* reader;
        ~Return() {
            {
                std::lock_guard hold(pool.lock);
                pool.free.push_back(reader);
            }
            pool.wake.notify_one();
        }
    } give_back{*pool, reader};
    std::vector<Line> out;
    const std::size_t unit = processor.name == "cp1610" ? 2 : 1;   // (a word-addressed processor: addresses count words)
    // the context set over the bytes read: the processor's own, then the caller's (what a read before left there, such as the state an ARM `blx` switches to
    // from its target on, is replaced)
    std::vector<std::pair<std::string, int>> here;
    for (const auto& [k, v] : processor.context) here.emplace_back(std::string(k), v);
    for (const auto& [k, v] : over) {
        const auto same = std::find_if(here.begin(), here.end(), [&](const auto& h) { return h.first == k; });
        if (same != here.end()) same->second = v;
        else here.emplace_back(std::string(k), v);
    }
    for (const decompile::sleigh::Instruction& i : reader->read(code.data(), code.size(), base, count, here)) {
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
    return bytes_at(build::mips_asm::Assembler(encoder, processor.big_endian ? build::mips_asm::ByteOrder::Big : build::mips_asm::ByteOrder::Little, (processor.name == "sh2" || processor.name == "sh4") ? 2 : 1, ';').build(source));
}

std::optional<Bytes> assemble_line(const Processor& processor, std::string_view line, std::uint64_t pc, const decompile::NameLabels& symbols) {
    if (processor.sla.empty()) throw UsageError(std::format("{} has no SLEIGH description yet", processor.name));
    std::vector<std::pair<std::string, int>> context;
    for (const auto& [k, v] : processor.context) context.emplace_back(std::string(k), v);
    const build::Encoder& encoder = isa::sleigh_encoder(languages() / processor.sla, context);
    const build::Labels& labels = symbols;
    Result<Bytes> done = encoder.assemble(line, static_cast<std::int64_t>(pc), labels);
    if (!done.ok()) return std::nullopt;
    return done.value();
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
    // (a text and the bytes it came from that assembled back to those bytes once do so at another address too: see described_source's
    // checked_text; the lines of this listing that repeat an earlier one are not assembled again)
    std::unordered_set<std::string> verified;
    for (const Line& l : read_described(processor, code, base, 0)) {
        if (l.text.starts_with(".d")) {
            out += raw(l) + "\n";
            continue;
        }
        std::string key = l.text + '|';
        for (const std::uint8_t b : l.bytes) key += std::format("{:02x}", b);
        if (verified.contains(key)) {
            out += "\t" + l.text + "\n";
            continue;
        }
        const Result<Bytes> again = encoder.assemble(l.text, static_cast<std::int64_t>(l.address), none);
        if (again.ok() && again.value() == l.bytes) {
            out += "\t" + l.text + "\n";
            verified.insert(std::move(key));
        } else {
            out += raw(l) + "\t; " + l.text + "\n";
        }
    }
    return out;
}

formats::Decoder described_decoder(const Processor& processor) {
    if (processor.sla.empty()) throw UsageError(std::format("{} has no SLEIGH description yet", processor.name));
    std::vector<std::pair<std::string, int>> context;
    for (const auto& [k, v] : processor.context) context.emplace_back(std::string(k), v);
    auto reader = std::make_shared<decompile::sleigh::Reader>(languages() / processor.sla, context);
    std::vector<std::string> halts;
    for (std::string_view h : processor.halts) halts.emplace_back(h);
    const Processor::Trap trap = processor.trap;
    const bool big = processor.big_endian;
    return [reader, halts, trap, big](ByteView mem, std::uint32_t pc) -> std::optional<formats::DecodedInsn> {
        if (pc >= mem.size()) return std::nullopt;
        // (the image is indexed by address: the reader is handed the whole of it, read at `pc`)
        const std::vector<decompile::sleigh::Instruction> one{reader->at(mem.data(), mem.size(), 0, pc)};
        if (one[0].length == 0) return std::nullopt;
        formats::DecodedInsn d;
        d.size = static_cast<std::uint32_t>(one[0].length);
        using F = decompile::sleigh::Flow;
        switch (one[0].flow) {
            case F::Next: d.flow = formats::Flow::Next; break;
            case F::CallIndirect: d.flow = formats::Flow::Call; break;   // (a call through a register: a call with no target known)
            case F::Branch: d.flow = formats::Flow::Branch; break;
            case F::Jump: d.flow = formats::Flow::Jump; break;
            case F::Call: d.flow = formats::Flow::Call; break;
            case F::Return: d.flow = formats::Flow::Return; break;
            case F::Indirect: d.flow = formats::Flow::Indirect; break;
        }
        if (one[0].target) d.target = static_cast<std::uint32_t>(*one[0].target);
        const std::string mnemonic = text::lower(one[0].mnemonic);
        if (std::find(halts.begin(), halts.end(), mnemonic) != halts.end()) d.flow = formats::Flow::Halt, d.target.reset();
        if (!trap.mnemonic.empty() && mnemonic == trap.mnemonic) {   // TRAP #n: a branch to the handler the vector table names
            const std::string_view op = text::trim(one[0].operands);
            const std::optional<std::int64_t> n = build::parse_number(op.starts_with('#') ? op.substr(1) : op);
            d.flow = formats::Flow::Branch;
            d.target.reset();
            if (n && *n >= 0) {
                const std::uint64_t at = trap.table + static_cast<std::uint64_t>(*n) * trap.entry;
                if (at + trap.entry <= mem.size()) {
                    std::uint32_t v = 0;
                    for (unsigned k = 0; k < trap.entry; ++k) v = big ? (v << 8 | mem[at + k]) : (v | static_cast<std::uint32_t>(mem[at + k]) << (8 * k));
                    d.target = v;
                }
            }
        }
        return d;
    };
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
