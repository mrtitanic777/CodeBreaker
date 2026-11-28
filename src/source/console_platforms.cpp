// source/console_platforms.cpp -- the console writer's loaders (console_writer.hpp): for each platform, which bytes of a file are the program,
// where they sit, its entry points and its header's values by the names the description's templates use. The rest of a platform is its
// description (config/platforms/<name>.json).

#include "source/console_writer.hpp"

#include <format>
#include <memory>

#include "core/error.hpp"
#include "source/isa.hpp"
#include "formats/atari2600.hpp"
#include "formats/atari5200.hpp"
#include "formats/atari7800.hpp"
#include "formats/coleco.hpp"
#include "formats/cpc.hpp"
#include "formats/gb.hpp"
#include "formats/msx.hpp"
#include "formats/sms.hpp"
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

using Loader = Loaded (*)(const std::filesystem::path&);

constexpr std::pair<std::string_view, Loader> kLoaders[] = {
    {"atari2600", load_atari2600},
    {"atari5200", load_atari5200},
    {"atari7800", load_atari7800},
    {"coleco", load_coleco},
    {"cpc", load_cpc},
    {"gb", load_gb},
    {"msx", load_msx},
    {"sms", load_sms},
    {"zx", load_zx},
};

}  // namespace

Loaded load(std::string_view platform, const std::filesystem::path& file) {
    for (const auto& [name, loader] : kLoaders) {
        if (name == platform) return loader(file);
    }
    throw UsageError(std::format("the console writer has no loader for {}", platform));
}

}  // namespace cb::source::console_writer
