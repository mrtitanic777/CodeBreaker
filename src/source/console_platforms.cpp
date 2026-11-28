// source/console_platforms.cpp -- the console writer's loaders (console_writer.hpp): for each platform, which bytes of a file are the program,
// where they sit, its entry points and its header's values by the names the description's templates use. The rest of a platform is its
// description (config/platforms/<name>.json).

#include "source/console_shapes.hpp"

#include <format>
#include <algorithm>
#include <cctype>
#include <unordered_set>
#include <map>
#include <memory>
#include <set>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "source/isa.hpp"
#include "source/isa_r4300.hpp"
#include "source/isa_sh2.hpp"
#include "source/iso_disc.hpp"
#include "source/sh2_writer.hpp"
#include "formats/atari2600.hpp"
#include "formats/atari5200.hpp"
#include "formats/atari7800.hpp"
#include "formats/cdi.hpp"
#include "formats/coleco.hpp"
#include "formats/cpc.hpp"
#include "formats/dreamcast.hpp"
#include "formats/gb.hpp"
#include "formats/gba.hpp"
#include "formats/genesis.hpp"
#include "formats/jaguar.hpp"
#include "formats/lynx.hpp"
#include "formats/msx.hpp"
#include "formats/n3ds.hpp"
#include "formats/n64.hpp"
#include "formats/nds.hpp"
#include "formats/neogeo.hpp"
#include "formats/nes.hpp"
#include "formats/nro.hpp"
#include "formats/orbis.hpp"
#include "formats/ppc_program.hpp"
#include "formats/ps2.hpp"
#include "formats/ps3.hpp"
#include "formats/psp.hpp"
#include "formats/psx.hpp"
#include "formats/saturn.hpp"
#include "formats/sega32x.hpp"
#include "formats/segacd.hpp"
#include "formats/sms.hpp"
#include "formats/threedo.hpp"
#include "formats/vb.hpp"
#include "formats/vita.hpp"
#include "formats/chf.hpp"
#include "formats/intv.hpp"
#include "formats/ngp.hpp"
#include "formats/o2.hpp"
#include "formats/vectrex.hpp"
#include "formats/wonderswan.hpp"
#include "formats/xboxone.hpp"
#include "formats/zx.hpp"

namespace cb::source::console_writer {

namespace {

using Num = std::int64_t;

// (each entry assigned in order: a later name for an address replaces an earlier one, as the formats list them)
template <class Entries>
image_writer::Names entry_names(const Entries& entries) {
    image_writer::Names names;
    for (const auto& e : entries) names[e.address] = std::string(e.name);
    return names;
}

Loaded load_coleco(const std::filesystem::path& file) {
    formats::coleco::Program p = formats::coleco::read(file);
    const formats::coleco::Header& h = p.header;
    Loaded l{.image = std::move(p.image), .origin = formats::coleco::kOrigin, .entries = entry_names(p.entries), .values = {}};
    l.values = {{"title_screen", Num{h.title_screen}}, {"sprite_names", Num{h.sprite_names}}, {"sprite_order", Num{h.sprite_order}},
                {"work_buffer", Num{h.work_buffer}},   {"controllers", Num{h.controllers}},   {"start", Num{h.start}},
                {"title", h.title}};
    return l;
}

Loaded load_msx(const std::filesystem::path& file) {
    formats::msx::Program p = formats::msx::read(file);
    const formats::msx::Header& h = p.header;
    Loaded l{.image = std::move(p.image), .origin = p.origin, .entries = entry_names(p.entries), .values = {}};
    l.values = {{"init", Num{h.init}}, {"statement", Num{h.statement}}, {"device", Num{h.device}}, {"text", Num{h.text}}};
    return l;
}

Loaded load_zx(const std::filesystem::path& file) {
    formats::zx::Program p = formats::zx::read(file);
    Loaded l{.image = std::move(p.image), .origin = p.origin, .entries = entry_names(p.entries), .values = {}};
    l.values = {{"kind", std::string(p.kind == formats::zx::Kind::Tape ? "tape" : "snapshot")}, {"registers", Num{p.registers.has_value()}}};
    if (p.registers) {
        const formats::zx::Registers& r = *p.registers;
        for (const auto& [name, v] : {std::pair<const char*, Num>{"a", r.a}, {"b", r.b}, {"c", r.c}, {"d", r.d}, {"e", r.e}, {"f", r.f}, {"h", r.h}, {"i", r.i},
                                      {"im", r.im}, {"ix", r.ix}, {"iy", r.iy}, {"l", r.l}, {"sp", r.sp}}) {
            l.values.insert_or_assign(name, v);
        }
    }
    return l;
}

Loaded load_cpc(const std::filesystem::path& file) {
    formats::cpc::Program p = formats::cpc::read(file);
    using Kind = formats::cpc::Kind;
    Loaded l{.image = std::move(p.image), .origin = p.origin, .entries = entry_names(p.entries), .values = {}};
    l.values = {{"kind", std::string(p.kind == Kind::Snapshot ? "snapshot" : p.kind == Kind::Disk ? "disk" : "binary")},
                {"disk", Num{p.kind == Kind::Disk}},
                {"name", p.name},
                {"files", p.files},
                {"registers", Num{p.registers.has_value()}}};
    if (p.registers) {
        const formats::cpc::Registers& r = *p.registers;
        for (const auto& [name, v] : {std::pair<const char*, Num>{"a", r.a}, {"b", r.b}, {"c", r.c}, {"d", r.d}, {"e", r.e}, {"f", r.f}, {"h", r.h}, {"i", r.i},
                                      {"im", r.im}, {"ix", r.ix}, {"iy", r.iy}, {"l", r.l}, {"sp", r.sp}, {"pc", r.pc}}) {
            l.values.insert_or_assign(name, v);
        }
    }
    return l;
}

Loaded load_gb(const std::filesystem::path& file) {
    formats::gb::Rom rom = formats::gb::read(file);
    Loaded l{.image = {}, .origin = 0, .entries = {}, .values = {}, .starts = rom.functions(isa::sm83_decoder()), .memory = rom.memory()};
    using Color = formats::gb::ColorSupport;
    l.values = {{"title", rom.title},
                {"mbc", rom.mbc()},
                {"color", std::string(rom.color == Color::None ? "" : rom.color == Color::Also ? "also" : "only")},
                {"sgb", Num{rom.sgb}}};
    l.image = std::move(rom.data);
    return l;
}

Loaded load_sms(const std::filesystem::path& file) {
    formats::sms::Rom rom = formats::sms::read(file);
    Loaded l{.image = {}, .origin = 0, .entries = {}, .values = {}, .starts = rom.functions(isa::z80_decoder()), .memory = rom.memory()};
    l.values = {{"system", std::string(formats::sms::to_string(rom.system))}, {"header", Num{rom.header.has_value()}}};
    if (rom.header) {
        l.values.insert_or_assign("product", rom.header->product);
        l.values.insert_or_assign("version", Num{rom.header->version});
        l.values.insert_or_assign("region", rom.header->region);
    }
    l.image = std::move(rom.data);
    return l;
}

Loaded load_atari2600(const std::filesystem::path& file) {
    const auto rom = std::make_shared<const formats::atari2600::Rom>(formats::atari2600::read(file));
    Loaded l;
    l.image = rom->data;
    const bool banked = rom->bank_count() > 1;
    for (std::size_t b = 0; b < rom->bank_count(); ++b) {
        l.cart.banks.push_back({static_cast<std::uint32_t>(b * rom->bank_size), rom->bank_size, rom->window, rom->memory(b)});
        // each bank's reset and IRQ vectors (the start bank's unprefixed)
        const std::string prefix = banked && b != rom->start_bank() ? std::format("bank{}_", b) : "";
        for (const auto& [vector, label] : {std::pair{rom->vectors[b].reset, "reset"}, std::pair{rom->vectors[b].irq, "irq"}}) {
            if (const auto folded = rom->fold(vector)) {
                const std::uint32_t at = static_cast<std::uint32_t>(b) << 16 | *folded;
                l.starts.push_back(at);
                l.entries.try_emplace(at, prefix + label);
            }
        }
    }
    l.cart.inside = [rom](std::size_t, std::uint32_t a) { return a >= rom->window && a < rom->window + rom->bank_size; };
    l.cart.fits = [rom](std::size_t, std::uint32_t a, std::uint32_t size) { return a + size <= rom->window + rom->bank_size; };
    l.cart.target = [rom](std::size_t b, std::uint32_t a) -> std::optional<std::uint32_t> {   // (a mirror folded in)
        const auto folded = rom->fold(a);
        if (!folded) return std::nullopt;
        return static_cast<std::uint32_t>(b) << 16 | *folded;
    };
    l.cart.data_at = [rom](std::size_t b, std::uint32_t a) -> std::optional<std::uint32_t> {   // (exactly: a mirror's address is left as it is)
        if (rom->fold(a) != std::optional<std::uint32_t>(a)) return std::nullopt;
        return static_cast<std::uint32_t>(b) << 16 | a;
    };
    l.cart.offset = [rom](std::size_t b, std::uint32_t a) { return rom->offset(b, a); };
    l.values = {{"scheme", std::string(formats::atari2600::to_string(rom->scheme))}, {"banks", Num(rom->bank_count())}, {"banked", Num{banked}}};
    return l;
}

Loaded load_atari5200(const std::filesystem::path& file) {
    const auto rom = std::make_shared<const formats::atari5200::Rom>(formats::atari5200::read(file));
    Loaded l;
    l.image = rom->data;
    l.cart.banks.push_back({0, static_cast<std::uint32_t>(rom->data.size()), rom->base, rom->memory()});
    l.starts = {rom->start};
    l.entries = {{rom->start, "start"}};
    l.cart.inside = [rom](std::size_t, std::uint32_t a) { return rom->offset(a).has_value(); };
    l.cart.fits = [rom](std::size_t, std::uint32_t a, std::uint32_t size) { return rom->offset(a) && rom->offset(a + size - 1); };
    l.cart.target = [rom](std::size_t, std::uint32_t a) { return rom->offset(a) ? std::optional<std::uint32_t>(a) : std::nullopt; };
    l.cart.data_at = [](std::size_t, std::uint32_t a) { return std::optional<std::uint32_t>(a); };
    l.cart.offset = [rom](std::size_t, std::uint32_t a) { return *rom->offset(a); };
    l.values = {{"base", Num{rom->base}}, {"flag_a", Num{rom->flag_a}}, {"flag_b", Num{rom->flag_b}}, {"start", Num{rom->start}}};
    return l;
}

Loaded load_atari7800(const std::filesystem::path& file) {
    const auto rom = std::make_shared<const formats::atari7800::Rom>(formats::atari7800::read(file));
    Loaded l;
    l.image = rom->data;
    l.cart.header = rom->header;
    if (!rom->supergame) {
        l.cart.banks.push_back({0, static_cast<std::uint32_t>(rom->data.size()), rom->base, rom->memory(0)});
    } else {   // (16 KB banks: the last fixed at $C000, the others switched in at $8000)
        for (std::size_t b = 0; b < rom->bank_count(); ++b) {
            l.cart.banks.push_back({static_cast<std::uint32_t>(b * formats::atari7800::kBankSize), static_cast<std::uint32_t>(formats::atari7800::kBankSize),
                                    b == rom->fixed_bank() ? 0xC000u : 0x8000u, rom->memory(b)});
        }
    }
    const auto flow = [](std::size_t b, std::uint32_t a) { return static_cast<std::uint32_t>(b) << 16 | a; };
    // the vectors: where each stands, and the bank its code is in
    for (const auto& [vector, label] : {std::pair{rom->vectors.reset, "reset"}, std::pair{rom->vectors.nmi, "nmi"}, std::pair{rom->vectors.irq, "irq"}}) {
        const std::uint32_t at = flow(rom->supergame && vector >= 0xC000 ? rom->fixed_bank() : 0, vector);
        if (!rom->offset(at >> 16, vector)) continue;
        l.starts.push_back(at);
        l.entries.try_emplace(at, label);
    }
    l.cart.inside = [rom](std::size_t b, std::uint32_t a) { return rom->offset(b, a).has_value(); };
    l.cart.fits = [rom](std::size_t b, std::uint32_t a, std::uint32_t size) {
        const auto first = rom->offset(b, a);
        const auto last = rom->offset(b, a + size - 1);
        return first && last && *last == *first + size - 1;
    };
    l.cart.target = [rom, flow](std::size_t b, std::uint32_t a) -> std::optional<std::uint32_t> {
        if (!rom->supergame) return rom->offset(0, a) ? std::optional<std::uint32_t>(a) : std::nullopt;
        if (a >= 0xC000) return flow(rom->fixed_bank(), a);
        if (a < 0x8000 || b == rom->fixed_bank()) return std::nullopt;   // (which bank the fixed bank's code selected is a run-time fact)
        return flow(b, a);
    };
    l.cart.data_at = [rom, flow](std::size_t b, std::uint32_t a) { return std::optional<std::uint32_t>(flow(rom->supergame && a >= 0xC000 ? rom->fixed_bank() : b, a)); };
    l.cart.offset = [rom](std::size_t b, std::uint32_t a) { return *rom->offset(b, a); };
    l.values = {{"version", Num{rom->version}}, {"title", rom->title}, {"declared", Num{rom->declared_size}}, {"cart_type", Num{rom->cart_type}},
                {"controller1", Num{rom->controller1}}, {"controller2", Num{rom->controller2}}, {"tv", Num{rom->tv}}, {"save", Num{rom->save}}};
    return l;
}

// The NES: the 16-byte header, a trainer, the PRG (16 KB at $C000; up to 32 KB at $8000; with a mapper, 16 KB banks, the last fixed at $C000 and
// the others at $8000, as at power-on) and the CHR, which goes beside the source. Anything after the CHR is not the cartridge's.
Loaded load_nes(const std::filesystem::path& file) {
    const auto rom = std::make_shared<const formats::nes::Rom>(formats::nes::read(file));
    constexpr std::uint32_t kBank = 0x4000;
    Loaded l;
    l.image = rom->header;
    l.image.insert(l.image.end(), rom->trainer.begin(), rom->trainer.end());
    const std::uint32_t prg_at = static_cast<std::uint32_t>(l.image.size());
    l.image.insert(l.image.end(), rom->prg.begin(), rom->prg.end());
    l.image.insert(l.image.end(), rom->chr.begin(), rom->chr.end());
    Segments& s = l.segments;
    s.pieces.push_back({"HEADER", "HEADER", "header", 0, static_cast<std::uint32_t>(rom->header.size()), 0, false});
    if (!rom->trainer.empty()) s.pieces.push_back({"TRAINER", "TRAINER", "trainer", static_cast<std::uint32_t>(rom->header.size()), static_cast<std::uint32_t>(rom->trainer.size()), 0x7000, false});
    const auto bank = [&](std::size_t i, std::uint32_t base, std::uint32_t offset, std::uint32_t size) {
        s.pieces.push_back({std::format("BANK{}", i), std::format("PRG{}", i), "", prg_at + offset, size, base, true});
    };
    const std::uint32_t prg_size = static_cast<std::uint32_t>(rom->prg.size());
    if (prg_size == kBank) {
        bank(0, 0xC000, 0, kBank);
    } else if (rom->mapper == 0 || prg_size <= 0x8000) {
        bank(0, 0x8000, 0, prg_size);
    } else {
        const std::size_t count = rom->bank_count();
        for (std::size_t i = 0; i < count; ++i) bank(i, i == count - 1 ? 0xC000u : 0x8000u, static_cast<std::uint32_t>(i * kBank), kBank);
    }
    if (!rom->chr.empty()) s.pieces.push_back({"CHARS", "CHR", "chr", prg_at + prg_size, static_cast<std::uint32_t>(rom->chr.size()), 0, false});
    s.offset_of = [rom, prg_at](std::uint32_t a) -> std::optional<std::uint32_t> {
        const auto mapped = rom->mapped(a);
        if (!mapped) return std::nullopt;
        return prg_at + *mapped;
    };
    s.measured = rom->prg.size();
    l.memory = rom->memory();
    l.starts = rom->functions(isa::m6502_decoder());
    l.entries = {{rom->vectors.reset, "reset"}};
    l.entries.try_emplace(rom->vectors.nmi, "nmi");
    l.entries.try_emplace(rom->vectors.irq, "irq");
    l.values = {{"mapper", Num{rom->mapper}}, {"prg_kb", Num(rom->prg.size() / 1024)}, {"chr_kb", Num(rom->chr.size() / 1024)},
                {"mirroring", std::string(formats::nes::to_string(rom->mirroring))}, {"battery", Num{rom->battery}}};
    return l;
}

// The Jaguar: a cartridge (its boot signature and header ahead of the ROM) or a COFF executable, its segments written in address order and
// put back where the file has them.
Loaded load_jaguar(const std::filesystem::path& file) {
    namespace jag = formats::jaguar;
    const auto program = std::make_shared<const jag::Program>(jag::read(file));
    const bool cartridge = program->kind == jag::Kind::Cartridge;
    Loaded l;
    l.image = program->file;
    l.memory = program->memory();
    l.starts = {program->entry};
    l.entries = {{program->entry, "start"}};
    std::vector<jag::Segment> order = program->segments;
    std::sort(order.begin(), order.end(), [](const jag::Segment& a, const jag::Segment& b) { return a.address < b.address; });
    Segments& s = l.segments;
    for (const jag::Segment& seg : order) {
        Piece piece{seg.name.empty() ? "segment" : seg.name, "", "", seg.offset, seg.size, seg.address, true};
        piece.org = true;
        s.pieces.push_back(std::move(piece));
        s.code_ranges.push_back({seg.address, seg.address + seg.size});
    }
    s.measured = program->file.size();
    s.rebuilt = [program, order](const Bytes& image) {   // (the segments' bytes follow one another in address order; a COFF is the file with each section's replaced)
        Bytes out = program->file;
        std::size_t at = 0;
        for (const jag::Segment& seg : order) {
            for (std::uint32_t i = 0; i < seg.size; ++i, ++at) out[seg.offset + i] = at < image.size() ? image[at] : 0;
        }
        return out;
    };
    l.values = {{"cartridge", Num{cartridge}}, {"ext", std::string(cartridge ? "j64" : "cof")}, {"entry", Num{program->entry}}};
    return l;
}

Loaded load_genesis_file(const std::filesystem::path& file) { return load_genesis(formats::genesis::read(file)); }

// The PlayStation: a PS-X EXE (or the program a disc boots, named as the disc names it): the 2048-byte header, then the program at its load
// address. An executable's extension goes from its name; a disc's SLUS_000.01 keeps its whole name (".01" is no extension).
Loaded load_ps1(const std::filesystem::path& path) {
    const formats::psx::Exe exe = formats::psx::read(path);
    const std::filesystem::path named = exe.boot_file.empty() ? path.filename() : std::filesystem::path(exe.boot_file).filename();
    const bool known = std::ranges::any_of(formats::psx::kExtensions, [&](std::string_view e) { return text::iequals(named.extension().string(), e); });
    Loaded l;
    l.image = exe.data;
    l.starts = exe.functions(isa::mips_decoder());
    l.entries = {{exe.header.pc0, "entry"}};
    const std::uint32_t header = static_cast<std::uint32_t>(formats::psx::kHeaderSize);
    Segments& s = l.segments;
    s.pieces.push_back({"header", "header", "header", 0, header, 0, false});
    Piece text{"text", "text", "", header, static_cast<std::uint32_t>(exe.data.size() - header), exe.header.t_addr, true};
    text.org = true;
    s.pieces.push_back(std::move(text));
    s.measured = exe.data.size();
    l.values = {{"file", named.string()}, {"stem", known || !named.has_extension() ? named.stem().string() : named.string()}, {"pc0", Num{exe.header.pc0}}};
    return l;
}

// The Nintendo 64: the header and the boot code (IPL3) as data, the program at its load address (what the boot code copies to RDRAM, up to
// 1 MB), and what is past it as data at its cartridge address. The source is the ROM in the console's byte order; the rebuilt file is made in
// the file's own.
Loaded load_n64(const std::filesystem::path& path) {
    namespace n64 = formats::n64;
    const auto rom = std::make_shared<const n64::Rom>(n64::read(path));
    constexpr std::uint32_t kCartKseg1 = 0xB0000000;   // the cartridge as a program reads it: ROM offset + this
    Loaded l;
    l.image = rom->data;
    l.starts = rom->functions(isa::r4300_decoder());
    const std::uint32_t base = rom->load_address();
    l.entries = {{base, "entry"}};
    const ByteView text = rom->program();
    Segments& s = l.segments;
    s.pieces.push_back({"header", "header", "header", 0, static_cast<std::uint32_t>(n64::kBootSize), 0, false});
    Piece program{"text", "text", "", static_cast<std::uint32_t>(n64::kBootSize), static_cast<std::uint32_t>(text.size()), base, true};
    program.org = true;
    s.pieces.push_back(std::move(program));
    const std::size_t rest_at = n64::kBootSize + text.size();
    if (rest_at < rom->data.size()) {
        Piece rest{"cartridge", "cartridge", "data", static_cast<std::uint32_t>(rest_at), static_cast<std::uint32_t>(rom->data.size() - rest_at), kCartKseg1 + static_cast<std::uint32_t>(rest_at), false};
        rest.org = true;
        s.pieces.push_back(std::move(rest));
    }
    s.measured = rom->data.size();
    s.original = rom->in_order();
    s.rebuilt = [order = rom->order](const Bytes& image) { return n64::Rom::convert(image, n64::ByteOrder::Z64, order); };
    const n64::Header& h = rom->header;
    const auto sums = rom->checksums();
    std::string crc_note;
    if (!sums) crc_note = "CRCs not checked (the ROM is shorter than the megabyte they cover)";
    else if (rom->checksums_match()) crc_note = "CRCs match the program's";
    else crc_note = std::format("CRCs do not match the program's ({:08X} {:08X}); kept as they are", sums->first, sums->second);
    l.values = {{"name", h.name}, {"game_code", h.game_code}, {"country", std::string(1, h.country)}, {"order", std::string(n64::order_text(rom->order))},
                {"entry", Num{h.entry}}, {"cic", std::string(n64::cic_text(rom->cic))}, {"crc_note", crc_note}, {"z64", Num{rom->order == n64::ByteOrder::Z64}}};
    return l;
}

// The Game Boy Advance: the ROM at $08000000, its header field by field; flow from the entry branch and what the code calls or points at.
Loaded load_gba(const std::filesystem::path& path) {
    const formats::gba::Rom rom = formats::gba::read(path);
    Loaded l;
    l.image = rom.data;
    l.starts = rom.functions(isa::arm_decoder());
    l.segments.pieces.push_back({"rom", "rom", "", 0, static_cast<std::uint32_t>(rom.data.size()), formats::gba::kRomBase, true});
    l.segments.measured = rom.data.size();
    l.values = {{"title", rom.header.title}, {"game_code", rom.header.game_code}};
    return l;
}

// The Neo Geo's P-ROM: the vector table, the header from $100 field by field, the program in the first megabyte (flow from the header's callbacks
// and jump table), the second megabyte written at the bank window as data. A handler is named for the callback or the vector that names it; one
// many vectors share is the program's catch-all. A .neo container's Z80 sound program is written beside it (the `neogeo_sound` platform).
Loaded load_neogeo(const std::filesystem::path& path) {
    namespace ng = formats::neogeo;
    const auto rom = std::make_shared<const ng::Rom>(ng::read(path));
    const Bytes& data = rom->data;
    const std::uint32_t first = static_cast<std::uint32_t>(std::min(data.size(), ng::kRomWindow));       // the bytes the flow runs through
    const std::uint32_t second = static_cast<std::uint32_t>(std::min(data.size(), ng::kRomWindow * 2));  // ... and those written as the bank
    Loaded l;
    l.image = data;
    l.memory.assign(data.begin(), data.begin() + first);
    const std::vector<std::uint32_t> function_starts = rom->functions(isa::m68k_decoder());
    l.starts = function_starts;   // flow also starts from the header's JMPs, which are not functions of their own
    for (const std::uint32_t jump : rom->callback_jumps()) l.starts.push_back(jump);
    const std::set<std::uint32_t> starts(function_starts.begin(), function_starts.end());
    std::vector<std::pair<std::string, std::uint32_t>> vector_list = {{"ssp", rom->initial_ssp}, {"reset", rom->reset}};
    for (const ng::Vector& v : rom->vectors) vector_list.emplace_back(v.name, v.address);
    std::map<std::uint32_t, std::vector<std::string>> sharing;
    for (const auto& [name, at] : vector_list) sharing[at].push_back(name);
    image_writer::Names& names = l.entries;
    for (std::size_t i = 0; i < rom->header.callbacks.size(); ++i) {   // (where a callback jumps to is the function; two callbacks to one place take the first's name)
        const std::uint32_t target = rom->header.callbacks[i];
        if (target != 0 && starts.contains(target)) names.try_emplace(target, std::string(ng::kCallbackNames[i]));
    }
    if (rom->header.security != 0 && starts.contains(rom->header.security)) names.try_emplace(rom->header.security, "security");
    for (const auto& [at, vecs] : sharing) {
        if (!starts.contains(at) || names.contains(at) || (vecs.size() == 1 && vecs[0] == "ssp")) continue;
        std::string main_name;
        for (const std::string& v : vecs) {
            if (v == "reset" || v == "vblank" || v == "timer") {
                main_name = v;
                break;
            }
        }
        names[at] = vecs.size() > 2 ? "exception" : !main_name.empty() ? main_name : vecs[0];
    }
    Segments& s = l.segments;
    s.functions = function_starts;
    Piece vectors{"vectors", "vectors", "vectors", 0, 0x100, 0, false};
    vectors.org = true;
    s.pieces.push_back(std::move(vectors));
    s.pieces.push_back({"rom", "rom", "", 0x100, first - 0x100, 0x100, true});
    if (second > ng::kRomWindow) {
        Piece window{"window", "window", "", static_cast<std::uint32_t>(ng::kRomWindow), second - static_cast<std::uint32_t>(ng::kRomWindow), ng::Rom::address_of(static_cast<std::uint32_t>(ng::kRomWindow)), true};
        window.org = window.blank_before = true;
        window.org_note = "the bank window: the P-ROM's second megabyte";
        s.pieces.push_back(std::move(window));
    }
    s.code_ranges.push_back({static_cast<std::uint32_t>(ng::kFirstCode), first});
    s.measured = data.size();
    s.original.assign(data.begin(), data.begin() + second);
    s.after = [rom, second](image_writer::Written& r, const Values& values, const std::filesystem::path& folder) {
        if (rom->data.size() > second) {
            r.same = false;
            r.notes.push_back(std::format("the P-ROM holds {} bytes: the banks after the second megabyte are not written", rom->data.size()));
        }
        if (rom->sound.empty()) return;   // (a .neo container: its Z80 program is written beside the 68000's)
        const std::string& file = std::get<std::string>(values.at("file"));
        const image_writer::Written sound = write(describe("neogeo_sound"), load_neogeo_sound(rom->sound, file), file, folder);
        if (!sound.same) {
            r.same = false;
            r.notes.push_back("the Z80 sound program (M-ROM) did not rebuild" + (sound.notes.empty() ? std::string() : ": " + sound.notes.front()));
        }
    };
    std::vector<std::string> what = {"initial stack pointer", "reset"};
    for (unsigned i = 2; i < 64; ++i) what.push_back(ng::vector_name(i));
    l.values = {{"ngh", Num{rom->header.ngh}}, {"layout", std::string(ng::layout_text(rom->layout))}, {"vector_names", what}};
    return l;
}

Loaded load_neogeo_sound_file(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return load_neogeo_sound(file.view(), path.filename().string());
}

// The Virtual Boy: the ROM at $07000000, its header $220 bytes from the end, the reset slot and the interrupt vectors that hold code.
Loaded load_vb(const std::filesystem::path& path) {
    const formats::vb::Rom rom = formats::vb::read(path);
    Loaded l;
    l.image = rom.data;
    for (const formats::vb::Rom::Entry& e : rom.entries()) {
        const std::uint32_t a = rom.address(e.offset);
        l.starts.push_back(a);
        l.entries.try_emplace(a, e.name == "reset" ? std::string("reset") : "vector_" + std::string(e.name));
    }
    l.segments.pieces.push_back({"ROM", "ROM", "", 0, static_cast<std::uint32_t>(rom.data.size()), formats::vb::kBase, true});
    l.segments.measured = rom.data.size();
    l.values = {{"title", rom.header.title}, {"reset", Num{rom.reset_address()}}, {"header_at", Num(rom.header_offset())}, {"ext", path.extension().string()}};
    return l;
}

// The PlayStation 2: an R5900 ELF, or the BOOT2 program of a disc image (written under the disc's name).
Loaded load_ps2(const std::filesystem::path& path) {
    std::optional<formats::ps2::Program> program = formats::ps2::read(path);
    if (!program) throw FormatError("ps2", 0, "not a PlayStation 2 program (an R5900 ELF, or a disc image whose BOOT2 file is one)");
    const bool disc = program->kind == formats::ps2::Kind::Disc;
    Loaded l;
    l.image = program->elf.data;
    l.contained.mips_elf = std::make_shared<const formats::mips_elf::Elf>(std::move(program->elf));
    l.contained.hardware = [](std::uint32_t a) { return formats::ps2::hardware_name(a); };
    l.values = {{"disc", Num{disc}}};
    if (disc) l.values.insert_or_assign("file", path.stem().string() + ".elf");
    return l;
}

// The PSP: an ELF, a PRX module or an EBOOT.PBP holding one; the exports and the imports' stubs named (by NID when the name is not known).
Loaded load_psp(const std::filesystem::path& path) {
    std::optional<formats::psp::Program> program = formats::psp::read(path);
    if (!program) throw FormatError("psp", 0, "not an unencrypted PSP program (an ELF, a PRX, or an EBOOT.PBP holding one)");
    const auto clean = [](std::string_view text) {
        std::string out;
        for (const char c : text) out += (std::isalnum(static_cast<unsigned char>(c)) || c == '_') ? c : '_';
        return out;
    };
    Loaded l;
    image_writer::Names& names = l.contained.names;
    std::unordered_set<std::string> used;
    const auto unique = [&](std::string name, std::uint32_t address) {
        if (!used.insert(name).second) name = std::format("{}_{:08X}", name, address);
        used.insert(name);
        return name;
    };
    for (const formats::psp::Export& e : formats::psp::exports(program->elf)) {
        if (e.variable || e.address == 0) continue;
        const std::string name = e.name.empty() ? std::format("export_{}_{:08X}", e.library.empty() ? "system" : clean(e.library), e.nid) : e.name;
        if (!names.contains(e.address)) names[e.address] = unique(name, e.address);
    }
    for (const formats::psp::Import& i : formats::psp::imports(program->elf)) {
        if (i.variable) continue;
        const std::string name = i.name.empty() ? std::format("{}_{:08X}", clean(i.library), i.nid) : i.name;
        if (!names.contains(i.stub)) names[i.stub] = unique(name, i.stub);
    }
    l.values = {{"pbp", Num{program->kind == formats::psp::Kind::Pbp}}, {"prx", Num{program->prx()}}, {"title", program->title}};
    l.image = std::move(program->file);
    l.contained.elf_offset = program->elf_offset;
    l.contained.mips_elf = std::make_shared<const formats::mips_elf::Elf>(std::move(program->elf));
    l.contained.hardware = [](std::uint32_t a) { return formats::psp::hardware_name(a); };
    return l;
}

// The GameCube, the Wii and the Wii U: a DOL, an ELF or an RPX (decompressed), whichever of the three consoles it is for.
Loaded load_gekko(const std::filesystem::path& path) {
    namespace pp = formats::ppc_program;
    std::optional<pp::Program> read = pp::read(path);
    if (!read) throw FormatError("gekko", 0, "not a GameCube, Wii or Wii U program (a DOL, an ELF or an RPX)");
    auto program = std::make_shared<pp::Program>(std::move(*read));
    std::vector<std::string> loaded;
    for (const formats::ppc_segment::Segment& s : program->segments) loaded.push_back(std::format("{} at 0x{:08X} ({} bytes)", s.name, s.address, s.size));
    Loaded l;
    l.image = program->file;
    l.values = {{"console", std::string(pp::console_text(program->console))},
                {"cpu", std::string(pp::cpu_text(program->console))},
                {"kind", std::string(pp::kind_text(program->kind))},
                {"dol", Num{program->kind == pp::Kind::Dol}},
                {"rpx", Num{program->kind == pp::Kind::Rpx}},
                {"decompressed", Num{program->kind == pp::Kind::Rpx && program->elf && program->elf->inflated_sections != 0}},
                {"entry", Num{program->entry}},
                {"loaded", text::join(loaded, ", ")}};
    l.contained.gekko = std::move(program);
    return l;
}

// The flat shape's cartridges: the ROM at the address it is seen at, the values the head names; `ext` the file's extension (the rebuilt file's).
std::string extension_of(const std::filesystem::path& path) { return path.extension().string(); }

// The Odyssey 2: up to 3 KB at $0400.
Loaded load_o2(const std::filesystem::path& path) {
    Loaded l;
    l.image = formats::o2::read(path).data;
    l.origin = formats::o2::kBase;
    l.values = {{"ext", extension_of(path)}};
    return l;
}

// The Channel F: the cartridge at $0800.
Loaded load_chf(const std::filesystem::path& path) {
    Loaded l;
    l.image = formats::chf::read(path).data;
    l.origin = formats::chf::kBase;
    l.values = {{"ext", extension_of(path)}};
    return l;
}

// The Neo Geo Pocket: the cartridge at $200000, its header's colour, title and start.
Loaded load_ngp(const std::filesystem::path& path) {
    auto rom = std::make_shared<const formats::ngp::Rom>(formats::ngp::read(path));
    Loaded l;
    l.image = rom->data;
    l.origin = formats::ngp::kRomBase;
    l.values = {{"ext", extension_of(path)}, {"colour", Num{rom->header.colour}}, {"title", rom->header.title}, {"start", Num{rom->header.start}}};
    l.contained.program = std::shared_ptr<const formats::ngp::Rom>(std::move(rom));
    return l;
}

// The Vectrex: the cartridge at $0000, its copyright text and where the program starts after the header.
Loaded load_vectrex(const std::filesystem::path& path) {
    auto rom = std::make_shared<const formats::vectrex::Rom>(formats::vectrex::read(path));
    Loaded l;
    l.image = rom->data;
    l.values = {{"ext", extension_of(path)}, {"copyright", rom->copyright}, {"entry", Num{rom->entry}}};
    l.contained.program = std::shared_ptr<const formats::vectrex::Rom>(std::move(rom));
    return l;
}

// The Intellivision: an Intellicart .rom or a raw .int, its segments of 16-bit words (`image` is the file).
Loaded load_intv(const std::filesystem::path& path) {
    auto rom = std::make_shared<const formats::intv::Rom>(formats::intv::read(path));
    Loaded l;
    l.image = fs::read_file(path);
    l.values = {{"ext", extension_of(path)}, {"segments", static_cast<Num>(rom->segments.size())}, {"words", static_cast<Num>(rom->word_count())}, {"title", rom->title()}};
    l.contained.program = std::shared_ptr<const formats::intv::Rom>(std::move(rom));
    return l;
}

// The WonderSwan: the cartridge, its footer's values.
Loaded load_wonderswan(const std::filesystem::path& path) {
    auto rom = std::make_shared<const formats::wonderswan::Rom>(formats::wonderswan::read(path));
    Loaded l;
    l.image = rom->data;
    l.values = {{"color", Num{rom->is_color()}}, {"reset_segment", Num{rom->reset_segment}}, {"reset_offset", Num{rom->reset_offset}}, {"developer", Num{rom->developer}},
                {"game", Num{rom->game_id}}, {"version", Num{rom->version}}, {"save", Num{rom->save}}, {"checksum", Num{rom->checksum}},
                {"checksum_ok", Num{rom->checksum_ok}}};
    l.contained.program = std::shared_ptr<const formats::wonderswan::Rom>(std::move(rom));
    return l;
}

// The Sega CD: a disc image's system area (the boot header, the security code, the IP and the SP; 2048- or 2352-byte sectors).
Loaded load_segacd(const std::filesystem::path& path) {
    auto disc = std::make_shared<const formats::segacd::Disc>(formats::segacd::read(path));
    const formats::segacd::Header& h = disc->header;
    Loaded l;
    l.image = disc->boot;
    l.values = {{"domestic", h.domestic_name}, {"overseas", h.overseas_name}, {"serial", h.serial}, {"sector", static_cast<Num>(disc->sector_size)}};
    l.contained.program = std::shared_ptr<const formats::segacd::Disc>(std::move(disc));
    return l;
}

// The 32X: the cartridge (its 68000 side the Genesis's), the SH-2 program its header names.
Loaded load_sega32x(const std::filesystem::path& path) {
    namespace f32 = formats::sega32x;
    auto rom = std::make_shared<const f32::Rom>(f32::read(path));
    const f32::Header& h = rom->header;
    Loaded l;
    l.image = rom->data;
    l.values = {{"module", h.module_name}, {"program_size", static_cast<Num>(rom->has_sh2_program() ? rom->sh2_program().size() : 0)}, {"sh2_offset", Num{h.sh2_offset}},
                {"sdram", Num{f32::kSdram}}, {"master", Num{h.master_start}}, {"slave", Num{h.slave_start}}};
    l.contained.program = std::shared_ptr<const f32::Rom>(std::move(rom));
    return l;
}

// The 3DO: an Opera disc's LaunchMe, or a bare AIF.
Loaded load_threedo(const std::filesystem::path& path) {
    auto program = std::make_shared<const formats::threedo::Program>(formats::threedo::read(path));
    Loaded l;
    l.image = program->file;
    l.values = {{"disc", Num{program->kind == formats::threedo::Kind::Disc}}, {"name", program->name}, {"image_size", static_cast<Num>(program->image_size)}, {"entry", static_cast<Num>(program->aif.entry)}};
    l.contained.program = std::shared_ptr<const formats::threedo::Program>(std::move(program));
    return l;
}

// The CD-i: a disc's CD-RTOS modules, or a module file.
Loaded load_cdi(const std::filesystem::path& path) {
    auto program = std::make_shared<const formats::cdi::Program>(formats::cdi::read(path));
    Loaded l;
    l.image = program->file;
    l.values = {{"ext", path.extension().string()}, {"disc", Num{program->disc}}, {"modules", static_cast<Num>(program->modules.size())},
                {"placed", static_cast<Num>(program->placed().size())}};
    l.contained.program = std::shared_ptr<const formats::cdi::Program>(std::move(program));
    return l;
}

// The PlayStation 4 and 5: an ELF, or an unencrypted SELF holding one.
Loaded load_orbis(const std::filesystem::path& path) {
    auto program = std::make_shared<const formats::orbis::Program>(formats::orbis::read(path));
    std::string needed;
    for (const std::string& n : program->needed) needed += (needed.empty() ? "" : ", ") + n;
    Loaded l;
    l.image = program->file;
    l.values = {{"container", program->container}, {"ps4", Num{program->orbis || program->self}}, {"entry", static_cast<Num>(program->entry)},
                {"imports", static_cast<Num>(program->imports.size())}, {"needed", needed}};
    l.contained.program = std::shared_ptr<const formats::orbis::Program>(std::move(program));
    return l;
}

Loaded load_xboxone_file(const std::filesystem::path& path) { return load_xboxone(path, ""); }

// The PlayStation 3: a PSL1GHT ELF or an unencrypted SELF holding one; the image is its loadable segments end to end.
Loaded load_ps3(const std::filesystem::path& path) {
    auto program = std::make_shared<formats::ps3::Program>(formats::ps3::read(path));
    Loaded l;
    l.image = program->image;
    l.values = {{"self", Num{program->self}}, {"segments", static_cast<Num>(program->segments.size())}, {"entry", static_cast<Num>(program->entry_code)},
                {"toc", static_cast<Num>(program->toc)}};
    l.contained.ps3 = std::move(program);
    return l;
}

// The Nintendo 3DS: a `.3dsx`, or the ARM ELF devkitARM links it from; the image is the file.
Loaded load_n3ds(const std::filesystem::path& path) {
    auto program = std::make_shared<formats::n3ds::Program>(formats::n3ds::read(path));
    Loaded l;
    l.image = program->file;
    l.values = {{"x3dsx", Num{program->kind == formats::n3ds::Kind::X3dsx}}, {"entry", Num{program->entry}}};
    l.contained.n3ds = std::move(program);
    return l;
}

// The Nintendo DS: a cartridge ROM, its two programs (the ARM9's and the ARM7's) and its file system in one source.
Loaded load_nds(const std::filesystem::path& path) {
    auto rom = std::make_shared<formats::nds::Rom>(formats::nds::read(path));
    Loaded l;
    l.image = rom->data;
    l.values = {{"title", rom->header.title}, {"game_code", rom->header.game_code}};
    l.contained.nds = std::move(rom);
    return l;
}

// The Nintendo Switch: a homebrew `.nro`, a `.nso` or the AArch64 ELF devkitA64 links; the image is what is loaded (a NRO's file).
Loaded load_switch(const std::filesystem::path& path) {
    namespace nro = formats::nro;
    auto program = std::make_shared<nro::Program>(nro::read(path));
    Loaded l;
    l.image = program->flat;
    l.values = {{"nro", Num{program->kind == nro::Kind::Nro}}, {"nso", Num{program->kind == nro::Kind::Nso}}, {"entry", static_cast<Num>(program->entry)}, {"note", program->note}};
    l.contained.nswitch = std::move(program);
    return l;
}

// The PlayStation Vita: an unencrypted SELF (a `.vpk`'s eboot.bin) or the ELF inside it; the image is its loadable segments inflated, end to end.
Loaded load_vita(const std::filesystem::path& path) {
    auto program = std::make_shared<formats::vita::Program>(formats::vita::read(path));
    Loaded l;
    l.image = program->image;
    l.values = {{"self", Num{program->self}}, {"module", program->module_name}, {"segments", static_cast<Num>(program->segments.size())}};
    l.contained.vita = std::move(program);
    return l;
}

// The Saturn: a disc's IP.BIN (the header, the initial program, the rest of the system area, which is never loaded) and its first program, one
// source; the four area-code entries, the initial program and the first program's start named. The lane runs a disc of its own holding them.
Loaded load_saturn(const std::filesystem::path& path) {
    namespace st = formats::saturn;
    const st::Disc disc = st::read(path);
    const std::uint32_t ip = disc.ip_length();
    const auto system_size = static_cast<std::uint32_t>(disc.system.size());
    const auto first_size = static_cast<std::uint32_t>(disc.first_read.size());
    const std::uint32_t load = disc.load_address();
    constexpr std::uint32_t kHeader = 0x100;
    auto image = std::make_shared<sh2_writer::Image>();
    image->parts.push_back({"header", 0, kHeader, st::kIpBase, false, true});
    image->parts.push_back({"ip", kHeader, ip - kHeader, st::kIpBase + kHeader, true, true});
    if (ip < system_size) image->parts.push_back({"system_tail", ip, system_size - ip, st::kIpBase + ip, false, false});
    if (first_size != 0) image->parts.push_back({"first_read", system_size, first_size, load, true, true});
    static constexpr const char* kAreas[] = {"ip_area_jp", "ip_area_tw", "ip_area_us", "ip_area_eu"};
    for (std::uint32_t i = 0; i < 4; ++i) {
        const std::uint32_t at = st::kIpBase + st::kAreaEntries + 0x20 * i;
        if (st::kAreaEntries + 0x20 * i + 2 <= ip) {
            image->entries.push_back(at);
            image->names[at] = kAreas[i];
        }
    }
    if (st::kInitAt + 2 <= ip) {
        image->entries.push_back(st::kIpBase + st::kInitAt);
        image->names[st::kIpBase + st::kInitAt] = "ip_init";
    }
    if (first_size != 0) {
        image->entries.push_back(load);
        image->names[load] = "first_read";
    }
    for (const std::uint32_t a : disc.functions(isa::sh2_decoder())) image->entries.push_back(a);
    Loaded l;
    l.image = disc.system;
    l.image.insert(l.image.end(), disc.first_read.begin(), disc.first_read.end());
    l.contained.sh2 = std::move(image);
    l.contained.own = {{load, load + first_size}};
    l.values = {{"title", disc.header.title}, {"product", disc.header.product}, {"areas", disc.header.areas}, {"ip", Num{ip}},
                {"first", disc.first_read_name}, {"first_size", Num{first_size}}, {"load", Num{load}}};
    l.contained.after = [name = disc.first_read_name, first_size](image_writer::Written& r, const Values& values, const std::filesystem::path& folder) {
        if (!r.linked || !r.built || first_size == 0 || name.empty()) return;
        const Bytes built = fs::read_file(*r.built);
        const std::filesystem::path iso = folder / (std::get<std::string>(values.at("stem")) + ".built.iso");
        fs::write_file(iso, iso_disc::minimal(ByteView(built).first(st::kSystemBytes), ByteView(built).subspan(st::kSystemBytes), name, "SEGA SATURN"));
        r.built = iso;
    };
    return l;
}

// The Dreamcast: a disc's IP.BIN (the header, then the bootstrap at $300) and its first program, descrambled, one source at physical
// addresses. The lane runs a disc of its own holding them (the program scrambled again when it was).
Loaded load_dreamcast(const std::filesystem::path& path) {
    namespace dc = formats::dreamcast;
    const dc::Disc disc = dc::read(path);
    const auto system_size = static_cast<std::uint32_t>(disc.system.size());
    const auto program_size = static_cast<std::uint32_t>(disc.program.size());
    constexpr std::uint32_t kHeader = 0x100;
    auto image = std::make_shared<sh2_writer::Image>();
    image->parts.push_back({"header", 0, kHeader, dc::kIpBase, false, true, false});
    image->parts.push_back({"ip", kHeader, system_size - kHeader, dc::kIpBase + kHeader, true, true});
    if (program_size != 0) image->parts.push_back({"first_read", system_size, program_size, dc::kFirstReadBase, true, true});
    if (dc::kIpEntry + 2 <= system_size) {
        image->entries.push_back(dc::kIpBase + dc::kIpEntry);
        image->names[dc::kIpBase + dc::kIpEntry] = "ip_bootstrap";
    }
    if (program_size != 0) {
        image->entries.push_back(dc::kFirstReadBase);
        image->names[dc::kFirstReadBase] = "first_read";
    }
    for (const std::uint32_t a : disc.functions(isa::sh4_decoder())) image->entries.push_back(a);
    Loaded l;
    l.image = disc.system;
    l.image.insert(l.image.end(), disc.program.begin(), disc.program.end());
    l.contained.sh2 = std::move(image);
    l.contained.own = {{dc::kIpBase, dc::kFirstReadBase + program_size}};
    l.values = {{"title", disc.header.title}, {"product", disc.header.product}, {"areas", disc.header.areas}, {"ip", Num{system_size}},
                {"first", disc.first_read_name}, {"first_size", Num{program_size}}, {"scrambled", Num{disc.scrambled}}};
    l.contained.after = [name = disc.first_read_name, program_size, scrambled = disc.scrambled](image_writer::Written& r, const Values& values,
                                                                                               const std::filesystem::path& folder) {
        if (!r.linked || !r.built || program_size == 0 || name.empty()) return;
        const Bytes built = fs::read_file(*r.built);
        const std::filesystem::path iso = folder / (std::get<std::string>(values.at("stem")) + ".built.iso");
        fs::write_file(iso, dreamcast_disc(built, name, scrambled));
        r.built = iso;
    };
    return l;
}

Loaded load_lynx_file(const std::filesystem::path& file) { return load_lynx(formats::lynx::read(file)); }

using Loader = Loaded (*)(const std::filesystem::path&);

constexpr std::pair<std::string_view, Loader> kLoaders[] = {
    {"atari2600", load_atari2600},
    {"atari5200", load_atari5200},
    {"atari7800", load_atari7800},
    {"chf", load_chf},
    {"cdi", load_cdi},
    {"coleco", load_coleco},
    {"cpc", load_cpc},
    {"dreamcast", load_dreamcast},
    {"gamecube", load_gekko},
    {"gb", load_gb},
    {"gba", load_gba},
    {"genesis", load_genesis_file},
    {"intv", load_intv},
    {"jaguar", load_jaguar},
    {"lynx", load_lynx_file},
    {"msx", load_msx},
    {"n3ds", load_n3ds},
    {"n64", load_n64},
    {"nds", load_nds},
    {"neogeo", load_neogeo},
    {"neogeo_sound", load_neogeo_sound_file},
    {"nes", load_nes},
    {"ngp", load_ngp},
    {"o2", load_o2},
    {"pce", pce_code::load_pce_file},
    {"ps1", load_ps1},
    {"ps2", load_ps2},
    {"ps3", load_ps3},
    {"ps4", load_orbis},
    {"ps5", load_orbis},
    {"psp", load_psp},
    {"saturn", load_saturn},
    {"sega32x", load_sega32x},
    {"segacd", load_segacd},
    {"sms", load_sms},
    {"snes", snes_code::load_snes_file},
    {"switch", load_switch},
    {"threedo", load_threedo},
    {"vb", load_vb},
    {"vectrex", load_vectrex},
    {"vita", load_vita},
    {"wii", load_gekko},
    {"wiiu", load_gekko},
    {"wonderswan", load_wonderswan},
    {"xboxone", load_xboxone_file},
    {"zx", load_zx},
};

}  // namespace

// The Lynx: the file in order, each stretch a cartridge's loaders copy to RAM (or a BS93 object's one block) a code piece linked at its RAM
// address, the rest data: the header, the encrypted first stage (written as the bytes it is), the directory, padding, unloaded blocks.
Loaded load_lynx(const formats::lynx::Program& program) {
    namespace lynx = formats::lynx;
    const bool cartridge = program.kind == lynx::Kind::Cartridge;
    Loaded l;
    l.image = program.file;
    Segments& s = l.segments;
    const std::uint32_t header = static_cast<std::uint32_t>(cartridge ? lynx::kHeaderSize : lynx::kBs93Header);
    s.pieces.push_back({"HEADER", "HEADER", cartridge ? "lnx-header" : "bs93-header", 0, header, 0, false});
    std::uint32_t at = header;
    int gaps = 0, codes = 0;
    const auto gap_to = [&](std::uint32_t end, bool trailer) {
        if (end > at) {
            const std::string name = trailer ? std::string("TRAILER") : std::format("GAP{}", gaps++);
            s.pieces.push_back({name, name, at == lynx::kHeaderSize && program.encrypted_blocks > 0 ? "stage1" : "", at, end - at, 0, false});
        }
        at = std::max(at, end);
    };
    for (const lynx::Segment& seg : program.segments) {
        if (seg.offset < at || seg.offset + seg.size > program.file.size()) continue;   // (set_loaded sorts them; one that overlaps another or runs off is data)
        gap_to(seg.offset, false);
        const std::string name = std::format("SEG{}", codes++);
        s.pieces.push_back({name, name, "", seg.offset, seg.size, seg.address, true});
        at = seg.offset + seg.size;
    }
    gap_to(static_cast<std::uint32_t>(program.file.size()), !cartridge);
    // the file offset each RAM address shows (an address two segments load to shows the later one)
    const auto shown = std::make_shared<std::vector<std::int64_t>>(0x10000, -1);
    for (const Piece& piece : s.pieces) {
        if (!piece.code) continue;
        for (std::uint32_t i = 0; i < piece.size && piece.address + i < lynx::kRamEnd; ++i) (*shown)[piece.address + i] = static_cast<std::int64_t>(piece.offset + i);
    }
    s.offset_of = [shown](std::uint32_t a) -> std::optional<std::uint32_t> {
        if (a >= lynx::kRamEnd || (*shown)[a] < 0) return std::nullopt;
        return static_cast<std::uint32_t>((*shown)[a]);
    };
    s.measured = program.file.size();
    l.memory = program.memory();
    l.starts = program.functions(isa::m65sc02_decoder());
    if (!program.entries.empty() && s.offset_of(program.entries.front())) l.entries[program.entries.front()] = "start";
    l.values = {{"cartridge", Num{cartridge}},
                {"container", std::string(cartridge ? "lnx" : "o")},
                {"name", program.name},
                {"manufacturer", program.manufacturer},
                {"encrypted_blocks", Num{program.encrypted_blocks}},
                {"loaded", Num(program.segments.size())},
                {"bank0_block", Num{program.bank0_block}},
                {"bank1_block", Num{program.bank1_block}},
                {"version", Num{program.version}},
                {"rotation", Num{program.rotation}},
                {"load_address", Num{program.load_address}},
                {"block_length", Num{program.block_length}},
                {"body", Num{program.block_length} - Num{lynx::kBs93Header}}};
    return l;
}

// The Genesis: the 64-long vector table, then from $100 the header (its fields) and the program; code from $200 on. A handler is named for its
// vector when one vector names it (or it is the reset, vblank or hblank handler); one many vectors share is the program's catch-all.
Loaded load_genesis(const formats::genesis::Rom& rom) {
    constexpr std::uint32_t kFirstCode = 0x200, kHeaderFrom = 0x100;
    Loaded l;
    l.image = rom.data;
    l.memory = rom.data;
    l.starts = rom.functions(isa::m68k_decoder());
    const std::set<std::uint32_t> starts(l.starts.begin(), l.starts.end());
    std::vector<std::pair<std::string, std::uint32_t>> vector_list = {{"ssp", rom.initial_ssp}, {"reset", rom.reset}};
    for (const formats::genesis::Vector& v : rom.vectors) vector_list.emplace_back(v.name, v.address);
    std::map<std::uint32_t, std::vector<std::string>> sharing;
    for (const auto& [name, at] : vector_list) sharing[at].push_back(name);
    for (const auto& [at, vecs] : sharing) {
        if (!starts.contains(at) || (vecs.size() == 1 && vecs[0] == "ssp")) continue;
        std::string main_name;
        for (const std::string& v : vecs) {
            if (v == "reset" || v == "vblank" || v == "hblank" || v == "external") {
                main_name = v;
                break;
            }
        }
        l.entries[at] = vecs.size() > 2 ? "exception" : !main_name.empty() ? main_name : vecs[0];
    }
    const std::uint32_t size = static_cast<std::uint32_t>(rom.data.size());
    Segments& s = l.segments;
    Piece vectors{"vectors", "vectors", "vectors", 0, kHeaderFrom, 0, false};
    vectors.org = true;
    s.pieces.push_back(std::move(vectors));
    s.pieces.push_back({"rom", "rom", "", kHeaderFrom, size - kHeaderFrom, kHeaderFrom, true});
    s.code_ranges.push_back({kFirstCode, size});
    s.measured = rom.data.size();
    std::vector<std::string> what = {"initial stack pointer", "reset"};
    for (unsigned i = 2; i < 64; ++i) what.push_back(formats::genesis::vector_name(i));
    const formats::genesis::Header& h = rom.header;
    l.values = {{"domestic", h.domestic_name}, {"overseas", h.overseas_name}, {"serial", h.serial}, {"smd", Num{rom.smd}}, {"vector_names", what}};
    return l;
}

// The Neo Geo's Z80 sound program: the first 32 KB (the Z80's fixed window) as one block at 0 with its reset and interrupt entries; the rest of
// an M-ROM is banked into $8000-$FFFF and is not written (the result says so). The source is named `<name>.m1`.
Loaded load_neogeo_sound(ByteView sound, std::string_view file_name) {
    constexpr std::size_t kFixed = 0x8000;
    Loaded l;
    l.image.assign(sound.begin(), sound.begin() + static_cast<std::ptrdiff_t>(std::min(sound.size(), kFixed)));
    l.origin = 0;
    l.entries = {{0x0000, "reset"}, {0x0038, "irq"}, {0x0066, "nmi"}};
    const std::size_t size = sound.size();
    l.values = {{"bytes", Num(size)}, {"banked", Num{size > kFixed}}, {"stem", std::filesystem::path(file_name).stem().string() + ".m1"}};
    l.segments.after = [size](image_writer::Written& r, const Values&, const std::filesystem::path&) {
        if (size <= kFixed) return;
        r.same = false;
        r.notes.push_back(std::format("the M-ROM holds {} bytes: the banks after the first 32 KB are not written", size));
    };
    return l;
}

Loaded load_xboxone(const std::filesystem::path& file, const std::string& member) {
    auto program = std::make_shared<const formats::xboxone::Program>(formats::xboxone::read(file, member));
    Loaded l;
    l.image = program->file;
    l.values = {{"container", program->container}, {"identity", program->identity}, {"version", program->version}};
    l.contained.program = std::shared_ptr<const formats::xboxone::Program>(std::move(program));
    return l;
}

Bytes dreamcast_disc(ByteView image, const std::string& name, bool scrambled, std::uint32_t volume_base) {
    namespace dc = formats::dreamcast;
    const Bytes program_plain(image.begin() + static_cast<std::ptrdiff_t>(dc::kSystemBytes), image.end());
    const Bytes program = scrambled ? dc::scramble(program_plain) : program_plain;
    return iso_disc::minimal(image.first(dc::kSystemBytes), program, name, "SEGA DREAMCAST", volume_base);
}

Loaded load(std::string_view platform, const std::filesystem::path& file) {
    for (const auto& [name, loader] : kLoaders) {
        if (name == platform) return loader(file);
    }
    throw UsageError(std::format("the console writer has no loader for {}", platform));
}

}  // namespace cb::source::console_writer
