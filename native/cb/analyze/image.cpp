// cb/analyze/image.cpp -- see image.hpp.
#include "cb/analyze/image.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"

namespace cb::analyze::image {

std::string auto_name(ImageKind kind, Va image_base, Va va) {
    switch (kind) {
        case ImageKind::Pe: return std::format("sub_{:06X}", va - image_base);
        case ImageKind::Ne: return std::format("sub_{}_{:04X}", ((va >> 16) - kFirstSelector) / 8 + 1, va & 0xFFFF);
        case ImageKind::Le: {
            const Va rel = va - kLeBase;
            return std::format("sub_{}_{:05X}", rel / kLeStride + 1, rel % kLeStride);
        }
    }
    return {};
}

namespace {

constexpr std::uint32_t kScnCode = 0x20000000 | 0x00000020, kScnWrite = 0x80000000;

// The start of the function each name is, and where its bytes may run to: the next start, or the end of its span.
void set_functions(ImageSpec& spec, const std::set<Va>& starts) {
    std::vector<Va> sorted(starts.begin(), starts.end());
    for (std::size_t k = 0; k < sorted.size(); ++k) {
        const Va va = sorted[k];
        Va end = va;
        for (const Span& s : spec.spans) {
            if (va >= s.va && va < s.va + static_cast<Va>(s.data.size())) end = s.va + static_cast<Va>(s.data.size());
        }
        const Va next = k + 1 < sorted.size() ? sorted[k + 1] : end;
        spec.functions.emplace_back(va, next < end ? next : end);
    }
}

// A name a C or C++ symbol can have, as the exports give them.
bool is_symbol_name(std::string_view name) {
    auto first = [](char c) { return std::isalpha(static_cast<unsigned char>(c)) || c == '_' || c == '?' || c == '@' || c == '$'; };
    auto rest = [&](char c) { return first(c) || std::isdigit(static_cast<unsigned char>(c)); };
    return !name.empty() && first(name[0]) && std::all_of(name.begin(), name.end(), rest);
}

std::string import_name(const formats::pe::Import& imp) {
    std::string dll = imp.dll;
    if (const std::size_t dot = dll.rfind('.'); dot != std::string::npos) dll.resize(dot);
    return text::upper(dll) + "!" + imp.name;
}

const Span* span_at(const ImageSpec& spec, Va va) {
    for (const Span& s : spec.spans) {
        if (va >= s.va && va < s.va + static_cast<Va>(s.data.size())) return &s;
    }
    return nullptr;
}

std::optional<std::uint32_t> u32_at(const ImageSpec& spec, Va va) {
    const Span* s = span_at(spec, va);
    if (s == nullptr || va + 4 > s->va + static_cast<Va>(s->data.size())) return std::nullopt;
    std::uint32_t v = 0;
    for (int i = 0; i < 4; ++i) v |= static_cast<std::uint32_t>(s->data[static_cast<std::size_t>(va - s->va) + i]) << (8 * i);
    return v;
}

}  // namespace

ImageSpec pe_spec(const formats::pe::Image& pe) {
    if (pe.header().machine != formats::pe::kMachineI386) throw UnsupportedError("analyze::image", 0, "not an x86 module");
    const Va base = static_cast<Va>(pe.header().image_base);
    ImageSpec spec;
    spec.kind = ImageKind::Pe;
    spec.bits = 32;
    spec.flat = true;
    const ByteView file = pe.data();
    for (const formats::pe::Section& s : pe.sections()) {
        const std::uint64_t size = std::max(s.virtual_size, s.raw_size);
        if (size == 0) continue;
        std::uint64_t held = s.raw_ptr != 0 ? std::min<std::uint64_t>(s.raw_size, s.virtual_size != 0 ? s.virtual_size : s.raw_size) : 0;
        held = s.raw_ptr < file.size() ? std::min<std::uint64_t>(held, file.size() - s.raw_ptr) : 0;   // (the file is cut short: what it has)
        Span span;
        span.va = base + s.virtual_address;
        span.data.assign(file.begin() + static_cast<std::ptrdiff_t>(s.raw_ptr), file.begin() + static_cast<std::ptrdiff_t>(s.raw_ptr + held));
        span.initialised = static_cast<Va>(held);
        span.data.resize(static_cast<std::size_t>(size), 0);
        span.executable = (s.characteristics & kScnCode) != 0;
        span.writable = (s.characteristics & kScnWrite) != 0;
        span.name = s.name;
        spec.spans.push_back(std::move(span));
    }
    std::sort(spec.spans.begin(), spec.spans.end(), [](const Span& a, const Span& b) { return a.va < b.va; });

    for (const formats::pe::Import& imp : pe.imports()) spec.imports[static_cast<Va>(imp.slot_va)] = import_name(imp);

    // function starts: the exports (not the ones passed on to another module), the entry point, the standard prologues
    std::set<Va> starts;
    std::set<Va> roots;
    for (const formats::pe::ExportSymbol& e : pe.export_table().symbols) {
        if (e.forward || e.rva == 0) continue;
        const Va va = base + e.rva;
        const Span* s = span_at(spec, va);
        if (s == nullptr || !s->executable) continue;
        starts.insert(va);
        roots.insert(va);
        if (e.name && is_symbol_name(*e.name)) spec.function_names[va] = *e.name;
    }
    if (const auto entry = pe.entry_va()) {
        roots.insert(static_cast<Va>(*entry));
        if (span_at(spec, static_cast<Va>(*entry)) != nullptr) starts.insert(static_cast<Va>(*entry));
    }
    for (const Span& s : spec.spans) {
        if (!s.executable) continue;
        const Bytes& d = s.data;
        for (std::size_t i = 0; i + 3 <= d.size(); ++i) {
            if (d[i] == 0x55 && d[i + 1] == 0x8B && d[i + 2] == 0xEC) {
                const bool hot_patch = i >= 2 && d[i - 2] == 0x8B && d[i - 1] == 0xFF;      // mov edi,edi first
                starts.insert(s.va + static_cast<Va>(hot_patch ? i - 2 : i));
            }
        }
    }
    for (Va va : starts) {
        if (!spec.function_names.contains(va)) spec.function_names[va] = auto_name(ImageKind::Pe, base, va);
    }
    set_functions(spec, starts);
    spec.roots.assign(roots.begin(), roots.end());

    // where the loader writes an address: every base relocation, and what it holds
    const std::vector<std::uint64_t> relocs = pe.base_relocs();
    spec.has_pointers = !relocs.empty();
    for (std::uint64_t r : relocs) {
        const Va site = base + static_cast<Va>(r);
        spec.pointers.emplace_back(site, Pointer{PointerKind::Abs32, u32_at(spec, site) ? std::optional<Va>(*u32_at(spec, site)) : std::nullopt});
    }
    return spec;
}

Image load_pe(const formats::pe::Image& pe) {
    return load(pe_spec(pe), static_cast<Va>(pe.header().image_base));
}

ImageSpec ne_spec(const NeLayout& layout) {
    ImageSpec spec;
    spec.kind = ImageKind::Ne;
    spec.bits = 16;
    spec.flat = false;
    spec.has_pointers = true;
    std::vector<const NeSegment*> by_index;
    for (const NeSegment& s : layout.segments) {
        if (s.bytes.empty()) continue;
        Span span;
        span.va = ne_address(s.index, 0);
        span.data = s.bytes;
        span.executable = s.code;
        span.writable = !s.code;
        span.initialised = s.file_length;
        span.name = std::format("{}{}", s.code ? "code" : "data", s.index);
        spec.spans.push_back(std::move(span));
    }
    std::sort(spec.spans.begin(), spec.spans.end(), [](const Span& a, const Span& b) { return a.va < b.va; });
    std::map<int, std::vector<std::uint32_t>> offsets;
    for (const NeFunction& f : layout.functions) {
        offsets[f.segment].push_back(f.offset);
        spec.function_names[ne_address(f.segment, f.offset)] = f.name;
    }
    for (auto& [seg, offs] : offsets) {
        std::sort(offs.begin(), offs.end());
        const auto code = layout.code_length.find(seg);
        for (std::size_t k = 0; k < offs.size(); ++k) {
            const std::uint32_t end = k + 1 < offs.size() ? offs[k + 1] : (code == layout.code_length.end() ? 0 : code->second);
            spec.functions.emplace_back(ne_address(seg, offs[k]), ne_address(seg, end));
        }
    }
    for (std::size_t k = 0; k < layout.imports.size(); ++k) spec.imports[extern_address(k)] = layout.imports[k];
    std::set<Va> roots;
    for (const auto& [seg, off] : layout.entry_points) {
        const Va va = ne_address(seg, off);
        if (spec.function_names.contains(va)) roots.insert(va);
    }
    spec.roots.assign(roots.begin(), roots.end());
    spec.data_segment = layout.data_segment != 0 ? ne_address(layout.data_segment, 0) : 0;
    spec.pointers = layout.pointers;
    return spec;
}

ImageSpec le_spec(const LeLayout& layout) {
    ImageSpec spec;
    spec.kind = ImageKind::Le;
    spec.bits = 32;
    spec.flat = false;
    for (const LeObject& o : layout.objects) {
        Span span;
        span.va = le_address(o.index, 0);
        span.data = o.bytes;
        span.executable = o.code;
        span.writable = o.writable || !o.code;
        span.initialised = static_cast<Va>(o.bytes.size());
        span.name = std::format("object{}", o.index);
        spec.spans.push_back(std::move(span));
    }
    std::sort(spec.spans.begin(), spec.spans.end(), [](const Span& a, const Span& b) { return a.va < b.va; });
    for (const auto& [va, name] : layout.functions) spec.function_names[va] = name;
    spec.imports.insert(layout.imports.begin(), layout.imports.end());
    spec.has_pointers = layout.has_pointers;
    spec.pointers = layout.pointers;
    // what the device descriptor block names -- the control procedure, the API procedures and the service table -- is
    // entered from outside
    std::set<Va> roots;
    if (layout.device_descriptor) {
        const Va at = *layout.device_descriptor;
        std::vector<std::uint32_t> entries;
        for (std::uint32_t o : {0x18u, 0x1Cu, 0x20u}) entries.push_back(u32_at(spec, at + o).value_or(0));
        const std::uint32_t table = u32_at(spec, at + 0x30).value_or(0), count = u32_at(spec, at + 0x34).value_or(0);
        if (table != 0 && count != 0 && count < 0x400 && span_at(spec, table) != nullptr) {
            for (std::uint32_t k = 0; k < count; ++k) entries.push_back(u32_at(spec, table + 4 * static_cast<Va>(k)).value_or(0));
        }
        for (std::uint32_t va : entries) {
            const Span* s = va != 0 ? span_at(spec, va) : nullptr;
            if (s == nullptr || !s->executable) continue;
            spec.function_names.try_emplace(va, auto_name(ImageKind::Le, 0, va));
            roots.insert(va);
        }
    }
    std::set<Va> starts;
    for (const auto& [va, name] : spec.function_names) starts.insert(va);
    set_functions(spec, starts);
    spec.roots.assign(roots.begin(), roots.end());
    return spec;
}

Image load(ImageSpec spec, Va image_base) {
    const ImageKind kind = spec.kind;
    Image image(std::move(spec));
    for (Va va : image.discover()) image.set_function_name(va, auto_name(kind, image_base, va));
    return image;
}

}  // namespace cb::analyze::image
