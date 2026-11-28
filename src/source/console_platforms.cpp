// source/console_platforms.cpp -- the console writer's loaders (console_writer.hpp): for each platform, which bytes of a file are the program,
// where they sit, its entry points and its header's values by the names the description's templates use. The rest of a platform is its
// description (config/platforms/<name>.json).

#include "source/console_writer.hpp"

#include <format>

#include "core/error.hpp"
#include "source/isa.hpp"
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

using Loader = Loaded (*)(const std::filesystem::path&);

constexpr std::pair<std::string_view, Loader> kLoaders[] = {
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
