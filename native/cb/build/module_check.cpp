// cb/build/module_check.cpp -- compare a built image with the original, call its exports beside the original's, and the module as one
// L2 tree.

#include <algorithm>
#include <format>
#include <map>
#include <set>

#include "cb/build/module_internal.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/detect.hpp"
#include "cb/translate/l2.hpp"

namespace cb::build::module {

using namespace internal;

namespace {

namespace ir = translate::ir;
namespace l2 = translate::l2;
namespace ct = decompile::c_text;

bool same_import(const definitions::Import& a, const definitions::Import& b) { return a.module == b.module && a.what == b.what; }

Address address_of(const definitions::Export& e) {
    if (const auto* flat = std::get_if<std::uint64_t>(&e.address)) return *flat;
    const Far16 far = std::get<Far16>(e.address);
    return core16::SegOff{far.seg, far.off};
}

std::string quoted_name(const std::optional<std::string>& name) { return name ? "'" + *name + "'" : "None"; }

// The L2 document the decompiler made of a function, if it made one.
const ir::Node* document_of(const ct::Module16& m, const std::string& name) {
    if (!m.description) return nullptr;
    const auto note = m.description->notes.find(name);
    return note == m.description->notes.end() ? nullptr : note->second.l2;
}

}  // namespace

// ---- the checks ---------------------------------------------------------------------------------------------------------------

void ModuleBuilder::check(Built& b) const {
    const stdfs::path& image = *b.image;
    const definitions::Interface built = services_.interface_of(image), original = services_.interface_of(b.original);
    b.format_same = formats::detect::format_of(image) == formats::detect::format_of(b.original) && built.library == original.library;
    std::map<std::uint32_t, const definitions::Export*> mine;
    for (const definitions::Export& e : built.exports) mine[e.ordinal] = &e;
    b.exports_total = original.exports.size();
    for (const definitions::Export& e : original.exports) {
        const auto got = mine.find(e.ordinal);
        // an export that has no name in the original cannot be made without one by this linker
        if (got != mine.end() && (got->second->name == e.name || (!e.name && got->second->name && *got->second->name == std::format("ordinal_{}", e.ordinal)))) {
            ++b.exports_same;
            if (b.stand_in_addresses.contains(address_of(e))) ++b.exports_on_stand_ins;
        } else {
            const bool listed = std::any_of(b.exports_missing.begin(), b.exports_missing.end(),
                                            [&](const definitions::MissingExport& x) { return x.what.ordinal == e.ordinal; });
            if (!listed) b.exports_missing.push_back({e, "in the build as " + quoted_name(got != mine.end() ? got->second->name : std::nullopt)});
        }
    }
    b.imports_original = original.imports.size();
    b.imports_built = built.imports.size();
    {
        std::vector<std::pair<std::string, definitions::Import>> extra;
        for (const definitions::Import& i : built.imports) {
            if (!std::any_of(original.imports.begin(), original.imports.end(), [&](const definitions::Import& w) { return same_import(w, i); }))
                extra.emplace_back(i.module + "." + i.text(), i);
        }
        std::sort(extra.begin(), extra.end(), [](const auto& x, const auto& y) { return x.first < y.first; });
        b.imports_extra.clear();
        for (auto& [text, i] : extra) b.imports_extra.push_back(std::move(i));
    }
    const resources::Module theirs = resources::read_module(b.original);
    if (!theirs.resources.empty()) {
        resources::Rebuilt r;
        resources::compare(theirs.resources, resources::read_module(image).resources, b.kind == "pe", r);
        b.resources_same = r.same();
        b.resources_line = std::format("{} of {} the same in the image ({} identical, {} equivalent)", r.identical + r.equivalent, r.total, r.identical, r.equivalent);
        if (const auto& w = b.resources; w && w->error.empty()) {
            b.resources_line += std::format("; written as text {}, image files {}, opaque payload {}, fallback payload {}", w->as_text, w->as_files, w->opaque, w->fallback);
        } else if (w) {
            b.resources_line += "; NOT COMPILED: " + w->error;
        }
    }
}

void ModuleBuilder::run(Built& b, std::size_t limit) const {
    if (!services_.call_export) throw UnsupportedError("build/module", 0, "no emulator is given to call the exports in");
    const std::string kind = b.kind.value_or("");
    std::map<std::uint32_t, definitions::Export> mine;
    for (const definitions::Export& e : services_.interface_of(*b.image).exports) {
        if (e.kind == definitions::ExportKind::Code) mine[e.ordinal] = e;
    }
    std::vector<definitions::Export> todo;
    for (const definitions::Export& x : services_.interface_of(b.original).exports) {
        if (x.kind == definitions::ExportKind::Code && mine.contains(x.ordinal)) todo.push_back(x);
    }
    for (std::size_t i = 0; i < todo.size() && i < limit; ++i) {
        const definitions::Export& e = todo[i];
        const CallOutcome got = services_.call_export(*b.image, kind, mine.at(e.ordinal)), want = services_.call_export(b.original, kind, e);
        b.ran.push_back({e.has_name() ? *e.name : std::format("@{}", e.ordinal), got.ended, want.ended, got.ended == want.ended && got.result == want.result});
    }
}

std::vector<Dispute> ModuleBuilder::disputes(const decompile::describe::NeDescription& d) const {
    try {
        const auto say = decompile::describe::callers_say(d);
        std::vector<Dispute> out;
        for (const std::string& line : d.lines) {
            const std::vector<std::string_view> p = text::split(line, '\t');
            if (p.size() < 4 || p[0] != "F") continue;
            const auto address = text::parse_int<std::int64_t>(p[1], 16);
            const auto pops = text::parse_int<std::int64_t>(p[3]);
            if (!address || !pops) continue;
            const auto who = say.find(decompile::describe::where(*address));
            if (who == say.end()) continue;
            const std::string said = who->second == decompile::describe::Remover::Callee ? "callee" : "caller";
            if (said != (*pops > 0 ? "callee" : "caller")) out.push_back({std::string(p[2]), said});
        }
        return out;
    } catch (const std::exception& e) {   // (a report, not the build: never stops it)
        return {{"(not checked)", std::string(e.what()).substr(0, 80)}};
    }
}

// ---- the small ones -----------------------------------------------------------------------------------------------------------

std::optional<std::string> name_of_stem(std::string_view stem, const std::map<std::string, std::string>& cstem) {
    for (const auto& [name, s] : cstem) {
        if (s == stem) return name;
    }
    return std::nullopt;
}

OrderedMap<std::string, std::string> kept_as_asm(std::string_view text, std::string_view module) {
    OrderedMap<std::string, std::string> kept;
    for (std::string_view line : text::split(text, '\n')) {
        const std::string_view code = line.substr(0, line.find('#'));
        // module, function, then the rest of the line as the reason
        std::vector<std::string_view> parts;
        std::string_view rest = text::ltrim(code);
        while (parts.size() < 2 && !rest.empty()) {
            const std::size_t end = std::find_if(rest.begin(), rest.end(), [](char c) { return text::is_space(c); }) - rest.begin();
            parts.push_back(rest.substr(0, end));
            rest = text::ltrim(rest.substr(end));
        }
        if (parts.size() == 2 && !rest.empty()) parts.push_back(rest);
        if (parts.size() >= 2 && text::iequals(parts[0], module)) kept.insert_or_assign(std::string(parts[1]), parts.size() > 2 ? std::string(text::trim(parts[2])) : std::string());
    }
    return kept;
}

void whole_file_image(const stdfs::path& original, const stdfs::path& image) {
    const Bytes head = fs::read_file(original);
    if (head.size() < 6) return;
    const std::int64_t pages = static_cast<std::int64_t>(head[4]) | static_cast<std::int64_t>(head[5]) << 8;
    // (signed: a file shorter than 511 bytes is a stub alone)
    if (pages * 512 < static_cast<std::int64_t>(head.size()) - 511) return;   // (a stub alone: nothing to do)
    Bytes file = fs::read_file(image);
    if (file.size() < 6) return;
    const std::uint16_t last = static_cast<std::uint16_t>(file.size() % 512), count = static_cast<std::uint16_t>((file.size() + 511) / 512);
    file[2] = static_cast<std::uint8_t>(last & 0xFF);
    file[3] = static_cast<std::uint8_t>(last >> 8);
    file[4] = static_cast<std::uint8_t>(count & 0xFF);
    file[5] = static_cast<std::uint8_t>(count >> 8);
    fs::write_file(image, file);
}

// ---- the module as one L2 tree --------------------------------------------------------------------------------------------------

ir::Node* module_document(Arena& arena, std::string_view module, const ct::Module16& m, const OrderedMap<std::string, std::string>& assembled,
                          const std::map<std::string, std::string>& stems, const stdfs::path& folder, const std::map<std::int64_t, std::int64_t>& lengths) {
    std::vector<std::pair<core16::SegOff, std::string>> places;
    for (const auto& [va, name] : m.made.by_va()) {
        if (const auto where = m.where.find(static_cast<core16::Va>(va)); where != m.where.end()) places.emplace_back(where->second, name);
    }
    std::sort(places.begin(), places.end());
    std::vector<ir::Node*> units;
    std::map<std::string, std::string> holder;   // asm stem -> the fragment that holds its text
    std::optional<std::string> holder_without_stem;   // (and the one held under no stem)
    for (std::size_t k = 0; k < places.size(); ++k) {
        const auto& [place, name] = places[k];
        std::optional<std::int64_t> end;
        if (k + 1 < places.size() && places[k + 1].first.seg == place.seg) {
            end = places[k + 1].first.off;
        } else if (const auto len = lengths.find(place.seg); len != lengths.end()) {
            end = len->second;
        }
        const ir::Origin origin = ir::OriginSegment{std::string(module), place.seg, place.off};
        if (assembled.contains(name)) {
            ir::MachineFragment frag;
            frag.name = name;
            frag.reason = assembled.at(name);
            frag.syntax = "wasm";
            if (end && *end > static_cast<std::int64_t>(place.off)) frag.length = static_cast<std::uint64_t>(*end - place.off);
            const auto stem = stems.find(name);
            std::optional<std::string> held = holder_without_stem;
            if (stem != stems.end()) {
                const auto h = holder.find(stem->second);
                held = h != holder.end() ? std::optional<std::string>(h->second) : std::nullopt;
            }
            if (held) {
                frag.within = *held;
            } else {
                std::error_code ec;
                if (stem != stems.end() && stdfs::is_regular_file(folder / (stem->second + ".asm"), ec)) frag.assembly = read_text_lossy(folder / (stem->second + ".asm"));
                if (stem != stems.end()) holder[stem->second] = name;
                else holder_without_stem = name;
            }
            units.push_back(l2::fragment_node(arena, std::move(frag), origin));
        } else if (const ir::Node* doc = document_of(m, name)) {
            ir::Node* copy = ir::clone(arena, doc);
            copy->origin = std::make_shared<const ir::Origin>(origin);
            units.push_back(copy);
        } else {
            units.push_back(l2::function_node(arena, {.name = name, .text = std::nullopt, .origin = origin}));
        }
    }
    return l2::module_node(arena, units);
}

ir::Node* module_document32(Arena& arena, std::string_view module, const ct::Compilable32& m) {
    std::vector<ir::Node*> units;
    for (const auto& [va, name] : m.by_va) {   // (ascending addresses)
        const ir::Origin origin = ir::OriginVa{std::string(module), static_cast<std::uint64_t>(va)};
        if (const auto doc = m.docs.find(name); doc != m.docs.end() && doc->second != nullptr) {
            ir::Node* copy = ir::clone(arena, doc->second);
            copy->origin = std::make_shared<const ir::Origin>(origin);
            units.push_back(copy);
        } else {
            units.push_back(l2::function_node(arena, {.name = name, .text = std::nullopt, .origin = origin}));
        }
    }
    return l2::module_node(arena, units);
}

}  // namespace cb::build::module
