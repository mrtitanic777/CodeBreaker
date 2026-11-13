// Loading a module for `analyze`: its file made an Image (a PE as it is, a 16-bit module as the decompiler lays it out).
#include <algorithm>
#include <cctype>
#include <format>
#include <map>

#include "cb/analyze/image.hpp"
#include "cb/analyze/report.hpp"
#include "cb/analyze/report_internal.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/ne.hpp"
#include "cb/formats/pe.hpp"

namespace cb::analyze::report::detail {

namespace {

namespace stdfs = std::filesystem;

// A knowledge that knows nothing: the module's own tables name what they can.
class NoKnowledge final : public decompile::describe::Knowledge {};

Loaded load_pe_module(const stdfs::path& path) {
    formats::pe::Image pe = formats::pe::Image::open(path);
    Loaded out;
    out.kind = ImageKind::Pe;
    out.image = std::make_shared<Image>(image::load_pe(pe));
    return out;
}

// The import "KERNEL_17" of a description as "KERNEL!OpenFile" (the number when the module's table has no name for it).
std::string ne_import_label(const std::string& raw, const Environment& environment) {
    const std::size_t cut = raw.rfind('_');
    if (cut == std::string::npos || cut == 0 || cut + 1 >= raw.size() ||
        !std::all_of(raw.begin() + static_cast<std::ptrdiff_t>(cut) + 1, raw.end(), [](char c) { return std::isdigit(static_cast<unsigned char>(c)) != 0; }))
        return raw;
    const std::string module_name = text::upper(raw.substr(0, cut));
    const auto number = text::parse_int<std::uint32_t>(std::string_view(raw).substr(cut + 1), 10);
    std::string shown = "#" + raw.substr(cut + 1);
    if (number && environment.export_name) {
        if (const std::optional<std::string> real = environment.export_name(module_name, *number)) shown = *real;
    }
    return module_name + "!" + shown;
}

Loaded load_ne_module(const stdfs::path& path, const Environment& environment) {
    const formats::ne::Module module = formats::ne::Module::open_file(path);
    const stdfs::path work = environment.state / "analyze" / "work" / text::lower(fs::to_text(path.filename()));
    fs::make_dirs(work);
    NoKnowledge nothing;
    decompile::describe::Knowledge& knowledge = environment.knowledge != nullptr ? *environment.knowledge : nothing;
    decompile::describe::NeDescription d = decompile::describe::describe16(module, work, knowledge);

    image::NeLayout layout;
    const std::vector<formats::ne::Segment> segments = module.segments();
    std::map<int, const formats::ne::Segment*> by_index;
    for (const formats::ne::Segment& s : segments) by_index[s.index] = &s;
    for (const auto& [index, bytes] : d.images) {
        if (bytes.empty()) continue;
        const auto seg = by_index.find(static_cast<int>(index));
        if (seg == by_index.end()) continue;
        image::NeSegment s;
        s.index = static_cast<int>(index);
        s.bytes = bytes;
        s.code = !seg->second->is_data();
        s.file_length = seg->second->file_offset != 0 ? static_cast<std::int64_t>(seg->second->length) : 0;
        layout.segments.push_back(std::move(s));
    }
    for (const auto& [va, at] : d.where_map) {
        const auto name = d.names.find(va);
        layout.functions.push_back({static_cast<int>(at.seg), at.off, name != d.names.end() ? name->second : std::string()});
    }
    for (const auto& [seg, length] : d.code) layout.code_length[static_cast<int>(seg)] = static_cast<std::uint32_t>(length);
    for (const std::string& raw : d.imports) layout.imports.push_back(ne_import_label(raw, environment));

    // the exports and the program's entry are entered from outside
    std::map<std::uint32_t, formats::ne::Entry> by_ordinal;
    for (const formats::ne::Entry& e : module.entries()) {
        if (e.kind == formats::ne::Entry::Kind::Unused) continue;
        by_ordinal[e.ordinal] = e;
        const Va va = decompile::describe::address(e.segment, e.offset);
        const auto name = d.names.find(va);
        if (name == d.names.end() || !d.where_map.contains(va)) continue;
        layout.entry_points.emplace_back(static_cast<int>(e.segment), static_cast<std::uint32_t>(e.offset));
    }
    const std::uint32_t cs = module.header().cs_ip >> 16, ip = module.header().cs_ip & 0xFFFF;
    if (cs != 0 && d.names.contains(decompile::describe::address(cs, ip))) layout.entry_points.emplace_back(static_cast<int>(cs), ip);
    layout.data_segment = static_cast<int>(d.data_segment);

    // where the loader writes an address: every place a relocation record (and its chain) names
    const std::vector<std::string> refs = module.module_refs();
    std::map<std::string, std::size_t> extern_index;
    for (std::size_t k = 0; k < d.imports.size(); ++k) extern_index.emplace(d.imports[k], k);
    const ByteView file = module.data();
    for (const formats::ne::Segment& s : segments) {
        for (const formats::ne::Relocation& r : module.relocations(s)) {
            std::optional<Va> target;
            if (const auto* internal = std::get_if<formats::ne::InternalRef>(&r.target)) {
                if (internal->segment == 0xFF) {
                    const auto e = by_ordinal.find(internal->entry);
                    if (e != by_ordinal.end()) target = decompile::describe::address(e->second.segment, e->second.offset);
                } else {
                    target = decompile::describe::address(internal->segment, internal->entry);
                }
            } else if (const auto* ordinal = std::get_if<formats::ne::ImportOrdinalRef>(&r.target)) {
                const std::string mod = ordinal->module >= 1 && ordinal->module <= refs.size() ? refs[ordinal->module - 1u] : "?";
                const auto k = extern_index.find(text::replace_all(std::format("{}_{}", mod, ordinal->ordinal), "-", "_"));
                if (k != extern_index.end()) target = image::extern_address(k->second);
            } else if (const auto* named = std::get_if<formats::ne::ImportNameRef>(&r.target)) {
                std::string nm = module.imported_name(named->name_offset);
                for (char& c : nm)
                    if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_') c = '_';
                const auto k = extern_index.find(nm);
                if (k != extern_index.end()) target = image::extern_address(k->second);
            } else {
                continue;
            }
            Pointer kind;
            kind.kind = r.source == 3 ? PointerKind::Far : r.source == 2 ? PointerKind::Selector : r.source == 5 ? PointerKind::Offset : PointerKind::Byte;
            kind.target = target;
            std::uint64_t off = r.offset;
            for (int hops = 0; off != 0xFFFF && off + 1 < s.length && hops < 0x10000; ++hops) {
                layout.pointers.emplace_back(image::ne_address(s.index, static_cast<std::uint32_t>(off)), kind);
                if (r.additive()) break;
                const std::uint64_t at = s.file_offset + off;
                if (at + 2 > file.size()) break;
                off = static_cast<std::uint64_t>(file[at]) | (static_cast<std::uint64_t>(file[at + 1]) << 8);
            }
        }
    }
    Loaded out;
    out.kind = ImageKind::Ne;
    out.image = std::make_shared<Image>(image::load(image::ne_spec(layout)));
    return out;
}

}  // namespace

Loaded load_module(const stdfs::path& path, const Environment& environment) {
    const std::optional<formats::detect::Lane> lane = formats::detect::lane(path);
    if (!lane) throw UnsupportedError("analyze", 0, std::format("{}: not a module the analyses read (a PE, or an NE)", fs::to_text(path.filename())));
    switch (*lane) {
    case formats::detect::Lane::Pe: return load_pe_module(path);
    case formats::detect::Lane::Ne: return load_ne_module(path, environment);
    case formats::detect::Lane::Le:
        throw UnsupportedError("analyze", 0, std::format("{}: a virtual device driver is not read here yet", fs::to_text(path.filename())));
    case formats::detect::Lane::Dos: break;
    }
    throw UnsupportedError("analyze", 0, std::format("{}: a DOS program is not read here", fs::to_text(path.filename())));
}

}  // namespace cb::analyze::report::detail
