// cb/formats/inspect.cpp -- describe(): one description for every executable format, and the document it becomes.
// (The strings, hex and resource views, the text reports and the `info` command are inspect_text.cpp.)
#include "cb/formats/inspect.hpp"

#include <algorithm>
#include <chrono>
#include <format>
#include <map>
#include <set>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/xex.hpp"

namespace cb::formats::inspect {

namespace {

struct FlagName {
    std::uint32_t bit;
    const char* name;
};

constexpr FlagName kFileFlags[] = {{0x0001, "NO_RELOCS"}, {0x0002, "EXECUTABLE"}, {0x0020, "LARGE_ADDRESS"},
                                   {0x0100, "32BIT"},     {0x0200, "NO_DEBUG"},   {0x1000, "SYSTEM"},
                                   {0x2000, "DLL"}};
constexpr FlagName kSectionFlags[] = {{0x20, "CODE"},          {0x40, "INITDATA"},       {0x80, "UNINITDATA"},
                                      {0x02000000, "DISCARDABLE"}, {0x04000000, "NOTCACHED"}, {0x08000000, "NOTPAGED"},
                                      {0x10000000, "SHARED"},  {0x20000000, "EXECUTE"},  {0x40000000, "READ"},
                                      {0x80000000, "WRITE"}};
constexpr FlagName kNeFlags[] = {{0x0001, "SINGLE_DATA"},    {0x0002, "MULTIPLE_DATA"}, {0x0004, "GLOBAL_INIT"},
                                 {0x0008, "PROTECTED_ONLY"}, {0x0800, "SELF_LOADING"},  {0x2000, "LINK_ERRORS"},
                                 {0x8000, "LIBRARY"}};
constexpr FlagName kSegmentFlags[] = {{0x0001, "DATA"},      {0x0010, "MOVABLE"},   {0x0020, "SHAREABLE"},
                                      {0x0040, "PRELOAD"},   {0x0080, "READ_ONLY"}, {0x0100, "RELOCINFO"},
                                      {0x1000, "DISCARDABLE"}};
constexpr FlagName kObjectFlags[] = {{0x0001, "READ"},      {0x0002, "WRITE"},   {0x0004, "EXECUTE"},
                                     {0x0008, "RESOURCE"},  {0x0010, "DISCARDABLE"}, {0x0020, "SHARED"},
                                     {0x0040, "PRELOAD"},   {0x2000, "32BIT"}};

template <std::size_t N>
std::vector<std::string> flags_of(std::uint32_t value, const FlagName (&table)[N]) {
    std::vector<std::string> out;
    for (const FlagName& f : table)
        if (value & f.bit) out.emplace_back(f.name);
    return out;
}

// The operating system an NE or LE header names (the same numbers in both).
std::string system_name(std::uint32_t id, bool ne_style) {
    switch (id) {
        case 0: if (ne_style) return "unknown"; break;
        case 1: return "OS/2";
        case 2: return "Windows";
        case 3: return "DOS 4";
        case 4: return "Windows 386";
        default: break;
    }
    return std::to_string(id);
}

std::string cpu_name(std::uint32_t c) {
    switch (c) {
        case 1: return "80286";
        case 2: return "80386";
        case 3: return "80486";
        case 4: return "Pentium";
        default: return std::to_string(c);
    }
}

std::string debug_type_name(std::uint32_t t) {
    switch (t) {
        case 0: return "Unknown";
        case 1: return "COFF";
        case 2: return "CodeView";
        case 3: return "FPO";
        case 4: return "Misc";
        case 5: return "Exception";
        case 6: return "Fixup";
        case 9: return "Borland";
        case 12: return "VC feature";
        case 16: return "Repro";
        default: return std::to_string(t);
    }
}

std::string subsystem_name(std::uint16_t s) {
    if (const char* label = pe::subsystem_label(s)) return label;
    return std::to_string(s);
}

std::string version_text(unsigned major, unsigned minor) { return std::format("{}.{}", major, minor); }

// A link timestamp as text, or "" when it is not a date (0, or a hash as newer linkers write).
std::string when(std::uint32_t stamp) {
    if (stamp == 0 || stamp >= 0x7FFFFFFFu) return "";
    return std::format("{:%Y-%m-%d %H:%M:%S}", std::chrono::sys_seconds{std::chrono::seconds{stamp}});
}

Preview preview_of(std::int32_t type_id) {
    switch (type_id) {
        case 1: case 2: case 3: return Preview::Image;
        case 6: case 10: case 16: case 23: case 24: return Preview::Text;
        default: return Preview::Hex;
    }
}

// A part that fails to read is noted, not thrown: a damaged table must not hide the rest. This is the isolation
// boundary (docs/CPP_DESIGN.md 3.6); the readers' strict errors stop here.
template <class F>
bool part(std::vector<std::string>& notes, std::string_view key, F&& make) {
    try {
        make();
        return true;
    } catch (const std::exception& e) {
        notes.push_back(std::format("{}: could not be read ({})", key, std::string(e.what()).substr(0, 80)));
        return false;
    }
}

std::size_t sum_symbols(const std::vector<ImportModule>& im) {
    std::size_t n = 0;
    for (const ImportModule& m : im) n += m.symbols.size();
    return n;
}

// ---- 32-bit Windows ------------------------------------------------------------------------------------------------
void describe_pe(Bytes data, const std::string& path, Description& d) {
    const pe::Image f = pe::Image::parse(std::move(data), path);
    const pe::Header& h = f.header();
    std::vector<std::string>& notes = d.notes;

    d.headers = PeHeaders{.read = false};
    std::optional<pe::OptionalHeader> opt;
    part(notes, "headers", [&] {
        PeHeaders ph;
        ph.machine = f.machine_name();
        ph.machine_id = h.machine;
        ph.sections = h.n_sections;
        ph.timestamp = h.timestamp;
        ph.optional_header_size = h.opt_size;
        ph.characteristics = flags_of(h.characteristics, kFileFlags);
        ph.optional = f.optional_header();
        opt = ph.optional;
        d.headers = std::move(ph);
    });

    PeExtras x;
    part(notes, "directories", [&] {
        std::vector<DirectoryRow> rows;
        const auto dirs = f.data_directories();
        for (std::size_t i = 0; i < dirs.size() && i < kDirectoryNames.size(); ++i)
            rows.push_back({std::string(kDirectoryNames[i]), dirs[i].rva, dirs[i].size});
        x.directories = std::move(rows);
    });
    part(notes, "sections", [&] {
        std::vector<Section> out;
        for (const pe::Section& s : f.sections())
            out.push_back({.name = s.name,
                           .address = std::uint64_t{h.image_base} + s.virtual_address,
                           .size = s.virtual_size,
                           .file_offset = s.raw_ptr,
                           .file_size = s.raw_size,
                           .flags = flags_of(s.characteristics, kSectionFlags),
                           .code = s.is_code()});
        d.sections = std::move(out);
    });
    part(notes, "exports", [&] {
        const pe::ExportTable t = f.export_table();
        Exports e;
        e.module = t.module;
        e.ordinal_base = t.ordinal_base;
        for (const pe::ExportSymbol& s : t.symbols)
            e.symbols.push_back({.name = s.name,
                                 .ordinal = s.ordinal,
                                 .hint = s.hint,
                                 .address = std::uint64_t{h.image_base} + s.rva,
                                 .forward = s.forward,
                                 .ne = std::nullopt});
        d.exports = std::move(e);
    });
    part(notes, "imports", [&] {
        std::vector<ImportModule> out;
        for (const bool delay : {false, true})
            for (const pe::ImportModule& m : f.import_table(delay)) {
                ImportModule r{.module = m.module, .delay = delay, .symbols = {}};
                for (const pe::ImportSymbol& s : m.symbols)
                    r.symbols.push_back({.name = s.name, .ordinal = s.ordinal, .hint = s.hint, .slot = s.slot_rva});
                out.push_back(std::move(r));
            }
        d.imports = std::move(out);
    });
    part(notes, "resources", [&] {
        Resources out;
        for (const pe::ResourceType& t : f.resource_tree()) {
            ResourceType r;
            const std::int32_t tid = t.id ? static_cast<std::int32_t>(*t.id) : -1;
            r.id = t.id ? std::optional<std::int32_t>(tid) : std::nullopt;
            if (t.id) {
                const std::string_view known = resource_type_name(*t.id);
                r.type = known.empty() ? std::to_string(*t.id) : std::string(known);
            } else {
                r.type = t.name.value_or("");
            }
            for (const pe::ResourceItem& it : t.items)
                // an item that goes by number is named "#<number>", as the resource editors list it
                r.items.push_back({.name = it.name ? it.name : it.id ? std::optional<std::string>("#" + std::to_string(*it.id)) : std::nullopt,
                                   .id = it.id,
                                   .language = it.language,
                                   .rva = it.rva,
                                   .offset = it.file_offset,
                                   .size = it.size,
                                   .type_id = tid,
                                   .preview = preview_of(tid),
                                   .wide = true,
                                   .from_pe = true});
            out.types.push_back(std::move(r));
        }
        d.resources = std::move(out);
    });
    part(notes, "relocations", [&] {
        RelocationSummary r;
        const auto blocks = f.reloc_blocks();
        r.blocks = blocks.size();
        for (const pe::RelocBlock& b : blocks) r.entries += b.entries;
        r.addresses = f.base_relocs().size();
        x.relocations = r;
    });
    part(notes, "tls", [&] { x.tls = f.tls(); });
    part(notes, "debug", [&] {
        std::vector<DebugRow> rows;
        for (const pe::DebugEntry& e : f.debug_entries())
            rows.push_back({debug_type_name(e.type), e.timestamp, e.size, e.offset, when(e.timestamp)});
        x.debug = std::move(rows);
    });
    part(notes, "certificates", [&] { x.certificates = f.certificates(); });
    part(notes, "clr", [&] { x.clr = f.clr(); });
    part(notes, "rich", [&] { x.rich = f.rich(); });
    part(notes, "load_config", [&] { x.load_config = f.directory(10).size > 0; });
    part(notes, "symbols_file", [&] { x.symbols_file = f.debug_codeview(); });
    d.extras = std::move(x);

    Summary s;
    s.format = h.is_plus ? "PE32+" : "PE32";
    s.type = h.is_dll() ? "DLL" : "EXE";
    s.machine = f.machine_name();
    s.module = d.exports.module;
    s.subsystem = opt ? subsystem_name(opt->subsystem) : "";
    s.linker = opt ? version_text(opt->linker_major, opt->linker_minor) : "";
    s.linked = when(h.timestamp);
    s.image_base = h.image_base;
    if (const auto entry = f.entry_va()) s.entry = *entry;
    s.code_bytes = f.code_bytes();
    s.sections = f.sections().size();
    s.exports = d.exports.symbols.size();
    s.import_modules = d.imports.size();
    s.imports = sum_symbols(d.imports);
    s.resources = d.resources.count();
    d.summary = std::move(s);
}

// ---- 16-bit Windows ------------------------------------------------------------------------------------------------
void describe_ne(Bytes data, Description& d) {
    const ne::Module f = ne::Module::open(std::move(data));
    const ne::Header& h = f.header();
    std::vector<std::string>& notes = d.notes;
    d.headers = NeHeaders{h};

    part(notes, "sections", [&] {
        std::vector<Section> out;
        for (const ne::Segment& s : f.segments())
            out.push_back({.name = std::format("seg {}", s.index),
                           .address = SegmentAddress{s.index, 0, SegmentAddress::Style::NeSegment},
                           .size = s.min_alloc,
                           .file_offset = s.file_offset,
                           .file_size = s.length,
                           .flags = flags_of(s.flags, kSegmentFlags),
                           .code = !s.is_data()});
        d.sections = std::move(out);
    });

    // The module's name and description are the first entries of the name tables; the rest are exported names.
    std::string module, description;
    std::map<std::uint32_t, std::string> by_ordinal;  // a name met twice for one ordinal keeps the later; sorted use
    part(notes, "names", [&] {
        const auto resident = f.resident_names();
        const auto nonresident = f.nonresident_names();
        std::string m = resident.empty() ? "" : resident.front().name;
        std::string desc = nonresident.empty() ? "" : nonresident.front().name;
        std::map<std::uint32_t, std::string> names;
        for (std::size_t i = 1; i < resident.size(); ++i) names[resident[i].ordinal] = resident[i].name;
        for (std::size_t i = 1; i < nonresident.size(); ++i) names[nonresident[i].ordinal] = nonresident[i].name;
        module = std::move(m);
        description = std::move(desc);
        by_ordinal = std::move(names);
    });

    d.exports.module = module;
    part(notes, "exports", [&] {
        Exports out;
        std::set<std::uint32_t> seen;
        for (const ne::Entry& e : f.entries()) {
            if (e.kind == ne::Entry::Kind::Unused) continue;
            seen.insert(e.ordinal);
            const auto name = by_ordinal.find(e.ordinal);
            out.symbols.push_back(
                {.name = name == by_ordinal.end() ? std::optional<std::string>{} : std::optional<std::string>(name->second),
                 .ordinal = e.ordinal,
                 .hint = std::nullopt,
                 .address = SegmentAddress{e.segment, e.offset, SegmentAddress::Style::NeSegment},
                 .forward = std::nullopt,
                 .ne = NeExportInfo{.movable = e.kind == ne::Entry::Kind::Moveable, .exported = (e.flags & 1) != 0}});
        }
        for (const auto& [ordinal, name] : by_ordinal)  // named, but with no entry of its own
            if (!seen.contains(ordinal))
                out.symbols.push_back({.name = name,
                                       .ordinal = ordinal,
                                       .hint = std::nullopt,
                                       .address = std::monostate{},
                                       .forward = std::nullopt,
                                       .ne = NeExportInfo{}});
        out.module = module;
        d.exports = std::move(out);
    });

    part(notes, "imports", [&] {
        const std::vector<std::string> mods = f.module_refs();
        struct Used {
            std::set<std::uint32_t> ordinals;
            std::set<std::string> names;
        };
        std::vector<Used> used(mods.size());
        for (const ne::Segment& s : f.segments())
            for (const ne::Relocation& r : f.relocations(s)) {
                if (const auto* o = std::get_if<ne::ImportOrdinalRef>(&r.target)) {
                    if (o->module >= 1 && o->module <= mods.size()) used[o->module - 1u].ordinals.insert(o->ordinal);
                } else if (const auto* n = std::get_if<ne::ImportNameRef>(&r.target)) {
                    if (n->module >= 1 && n->module <= mods.size())
                        used[n->module - 1u].names.insert(f.imported_name(n->name_offset));
                }
            }
        std::vector<ImportModule> out;
        for (std::size_t i = 0; i < mods.size(); ++i) {
            ImportModule m{.module = mods[i], .delay = false, .symbols = {}};
            for (const std::uint32_t o : used[i].ordinals) m.symbols.push_back({.ordinal = o});
            for (const std::string& n : used[i].names) m.symbols.push_back({.name = n});
            out.push_back(std::move(m));
        }
        d.imports = std::move(out);
    });

    part(notes, "resources", [&] {
        // grouped by type name, in the order the types first appear
        std::vector<ResourceType> types;
        std::unordered_map<std::string, std::size_t> index;  // lookup only
        for (const ne::Resource& r : f.resources()) {
            const std::string tname = r.type.empty() ? "?" : r.type;
            std::int32_t tid = -1;
            for (std::uint32_t n = 1; n <= 24; ++n)
                if (resource_type_name(n) == tname) {
                    tid = static_cast<std::int32_t>(n);
                    break;
                }
            if (tid < 0 && tname.size() > 1 && tname[0] == '#') {
                if (const auto n = text::parse_int<std::int32_t>(std::string_view(tname).substr(1)); n && *n >= 0) tid = *n;
            }
            auto [slot, inserted] = index.try_emplace(tname, types.size());
            if (inserted) types.push_back({.type = tname, .id = std::nullopt, .items = {}});
            types[slot->second].items.push_back({.name = r.id,
                                                 .language = 0,
                                                 .offset = r.file_offset,
                                                 .size = r.length,
                                                 .type_id = tid,
                                                 .preview = preview_of(tid),
                                                 .wide = false,
                                                 .from_pe = false});
        }
        for (ResourceType& t : types)
            if (t.items.front().type_id >= 0) t.id = t.items.front().type_id;
        d.resources.types = std::move(types);
    });

    NeExtras x;
    part(notes, "relocations", [&] {
        std::size_t n = 0;
        for (const ne::Segment& s : f.segments()) n += f.relocations(s).size();
        x.relocation_records = n;
    });
    d.extras = x;

    Summary s;
    s.format = "NE";
    s.type = (h.flags & 0x8000) ? "DLL" : "EXE";
    s.machine = "i286/i386 (16-bit)";
    s.module = module;
    s.description = description;
    s.subsystem = system_name(h.target_os, true);
    s.linker = version_text(h.linker_version, h.linker_revision);
    s.entry = SegmentAddress{h.cs_ip >> 16, h.cs_ip & 0xFFFF, SegmentAddress::Style::NeSegment};
    for (const Section& sec : d.sections)
        if (sec.code) s.code_bytes += sec.file_size;
    s.sections = d.sections.size();
    s.exports = d.exports.symbols.size();
    s.import_modules = d.imports.size();
    s.imports = sum_symbols(d.imports);
    s.resources = d.resources.count();
    d.summary = std::move(s);
}

// ---- drivers -------------------------------------------------------------------------------------------------------
// One LE/LX image: a driver file, or one member of a W3 bundle. Parts that cannot be read are noted and left empty.
DriverDescription describe_driver(const le::Module& f, const std::string& name, std::vector<std::string>& notes) {
    DriverDescription out;
    const le::Header& h = f.header();
    out.headers = {std::string(f.signature()), h};

    part(notes, "sections", [&] {
        std::vector<Section> secs;
        for (const le::Object& o : f.objects()) {
            // an object with no pages of its own (first_page 0) has no file offset; the first data page is the floor
            const std::int64_t first = (static_cast<std::int64_t>(o.first_page) - 1) * h.page_size;
            secs.push_back({.name = std::format("object {}", o.index),
                            .address = std::uint64_t{o.base},
                            .size = o.vsize,
                            .file_offset = static_cast<std::uint64_t>(std::max<std::int64_t>(0, h.data_pages + first)),
                            .file_size = std::uint64_t{o.page_count} * h.page_size,
                            .flags = flags_of(o.flags, kObjectFlags),
                            .code = o.executable()});
        }
        out.sections = std::move(secs);
    });
    part(notes, "imports", [&] {
        const std::vector<std::string> mods = f.import_modules();
        std::vector<std::set<std::uint32_t>> used(mods.size());
        try {
            for (const auto& [place, fixup] : f.fixups())
                if (const auto* ix = std::get_if<le::ImportFixup>(&fixup))
                    if (ix->module >= 1 && ix->module <= mods.size()) used[ix->module - 1u].insert(ix->ordinal);
        } catch (const std::exception& e) {  // the fixups read so far stand
            notes.push_back(std::format("imports: the fixup records could not all be read ({})", e.what()));
        }
        std::vector<ImportModule> im;
        for (std::size_t i = 0; i < mods.size(); ++i) {
            ImportModule m{.module = mods[i], .delay = false, .symbols = {}};
            for (const std::uint32_t o : used[i]) m.symbols.push_back({.ordinal = o});
            im.push_back(std::move(m));
        }
        out.imports = std::move(im);
    });
    part(notes, "driver", [&] { out.driver = f.ddb(); });
    std::string module, description;
    part(notes, "name", [&] { module = f.resident_name(); });
    part(notes, "description", [&] { description = f.description(); });

    out.exports.module = module;
    Summary& s = out.summary;
    s.format = std::string(f.signature());
    s.type = "driver";
    s.machine = cpu_name(h.cpu);
    s.module = out.driver && !out.driver->name.empty() ? out.driver->name : !module.empty() ? module : name;
    s.description = description;
    s.subsystem = system_name(h.os, false);
    if (h.eip_object != 0) s.entry = SegmentAddress{h.eip_object, h.eip, SegmentAddress::Style::Driver};
    for (const Section& o : out.sections)
        if (o.code) s.code_bytes += o.size;
    s.sections = out.sections.size();
    s.import_modules = out.imports.size();
    s.imports = sum_symbols(out.imports);
    return out;
}

void describe_le(Bytes data, Description& d) {
    const le::Module f = le::Module::from_buffer(std::move(data));
    DriverDescription dd = describe_driver(f, d.name, d.notes);
    d.headers = dd.headers;
    d.sections = std::move(dd.sections);
    d.imports = std::move(dd.imports);
    d.extras = DriverExtras{dd.driver};
    d.exports = std::move(dd.exports);
    d.resources = std::move(dd.resources);
    d.summary = std::move(dd.summary);
}

// A W3 file (vmm32.vxd): several drivers packed one after another.
void describe_bundle(Bytes data, Description& d) {
    const std::optional<le::Container> c = le::container(data);
    if (!c || c->kind != le::Container::Kind::W3) throw FormatError("inspect", 0, "not a W3 container");
    const auto shared = std::make_shared<const Bytes>(std::move(data));
    BundleExtras extras;
    for (const le::ContainerMember& e : c->members) {
        BundleMember m{.name = e.name, .offset = e.offset, .size = e.size, .notes = {}, .driver = std::nullopt};
        const std::uint64_t end = std::uint64_t{e.offset} + 2;
        if (end <= shared->size() && (*shared)[e.offset] == 'L' && ((*shared)[e.offset + 1] == 'E' || (*shared)[e.offset + 1] == 'X')) {
            try {
                m.driver = describe_driver(le::Module::parse(shared, e.offset), e.name, m.notes);
            } catch (const std::exception& err) {
                m.notes.push_back(std::string(err.what()).substr(0, 80));
            }
        }
        extras.members.push_back(std::move(m));
    }
    d.headers = W3Headers{c->version, extras.members.size()};
    for (const BundleMember& m : extras.members)
        d.sections.push_back({.name = m.name,
                              .address = std::uint64_t{0},
                              .size = m.size,
                              .file_offset = m.offset,
                              .file_size = m.size,
                              .flags = {},
                              .code = true});
    Summary s;
    s.format = "W3";
    s.type = "driver bundle";
    s.machine = "80386";
    s.module = d.name;
    s.description = std::format("{} drivers in one file", extras.members.size());
    s.subsystem = "Windows 386";
    for (const BundleMember& m : extras.members)
        if (m.driver) {
            s.code_bytes += m.driver->summary.code_bytes;
            s.imports += m.driver->summary.imports;
        }
    s.sections = extras.members.size();
    d.extras = std::move(extras);
    d.summary = std::move(s);
}

// ---- DOS -----------------------------------------------------------------------------------------------------------
void describe_dos(std::uint64_t file_size, const std::optional<detect::DosHeader>& h, Description& d) {
    const std::uint64_t start = h ? h->header_size : 0, size = h ? h->load_size : 0;
    if (h) d.headers = *h;
    d.sections.push_back({.name = "program",
                          .address = SegmentAddress{0, 0, SegmentAddress::Style::RealMode},
                          .size = size + (h ? std::uint64_t{h->min_alloc} * 16 : 0),
                          .file_offset = start,
                          .file_size = size,
                          .flags = {},
                          .code = true});
    if (file_size > start + size) {
        const std::uint64_t extra = file_size - start - size;
        d.sections.push_back({.name = "after the program",
                              .address = std::monostate{},
                              .size = extra,
                              .file_offset = start + size,
                              .file_size = extra,
                              .flags = {},
                              .code = false});
    }
    DosExtras x;
    x.relocation_count = h ? h->relocations : 0;
    if (h)
        for (std::size_t i = 0; i < h->relocation_entries.size() && i < 512; ++i)
            x.entries.push_back(std::format("{:04x}:{:04x}", h->relocation_entries[i].segment, h->relocation_entries[i].offset));
    d.extras = std::move(x);
    Summary s;
    s.format = "MZ";
    s.type = "DOS program";
    s.machine = "8086 (16-bit)";
    s.module = d.name;
    s.subsystem = "DOS";
    s.entry = SegmentAddress{h ? h->cs : 0u, h ? h->ip : 0u, SegmentAddress::Style::RealMode};
    s.code_bytes = size;
    s.sections = 1;
    d.summary = std::move(s);
}

// A file that is not an MZ executable: what detect::identify says it is, with what the format's reader adds.
void describe_other(const std::filesystem::path& path, const DescribeHooks& hooks, Description& d) {
    try {
        d.size = fs::file_size(path);
    } catch (const std::exception& e) {
        d.notes.push_back(std::format("cannot be read: {}", e.what()));
        return;
    }
    const std::vector<detect::Detected> found = detect::identify(path);
    if (found.empty()) {
        d.notes.emplace_back("not a format CodeBreaker recognises");
        return;
    }
    const detect::Detected& m = found.front();
    d.format = m.format;
    d.kind = m.family;
    d.facts = {{"format", m.format},
               {"type", m.detail},
               {"family", m.family},
               {"read with", m.command.empty() ? "(nothing yet)" : "codebreaker " + m.command}};
    if ((m.command == "media" || m.family == "xbox" || m.family == "xbox360") && hooks.xbox_summary) {
        try {
            for (auto& fact : hooks.xbox_summary(path, m)) {
                auto same = std::find_if(d.facts.begin(), d.facts.end(), [&](const auto& f) { return f.first == fact.first; });
                if (same != d.facts.end()) same->second = std::move(fact.second);
                else d.facts.push_back(std::move(fact));
            }
        } catch (const std::exception& e) {  // a damaged container: the format line stands
            d.notes.push_back(std::string(e.what()).substr(0, 120));
        }
    }
    d.ok = true;
}

}  // namespace

// ---- the vocabulary ------------------------------------------------------------------------------------------------
const std::vector<std::string_view> kSections = {"summary", "dos",         "headers",   "directories", "sections",
                                                 "exports", "imports",     "resources", "relocations", "tls",
                                                 "debug",   "certificates", "clr",      "rich",        "driver",
                                                 "members", "strings",     "hex"};
const std::vector<std::string_view> kDirectoryNames = {"Export",       "Import",     "Resource",   "Exception",
                                                       "Security",     "BaseReloc",  "Debug",      "Architecture",
                                                       "GlobalPtr",    "TLS",        "LoadConfig", "BoundImport",
                                                       "IAT",          "DelayImport", "CLR",       "Reserved"};

std::string_view resource_type_name(std::uint32_t id) {
    switch (id) {
        case 1: return "CURSOR";
        case 2: return "BITMAP";
        case 3: return "ICON";
        case 4: return "MENU";
        case 5: return "DIALOG";
        case 6: return "STRING";
        case 7: return "FONTDIR";
        case 8: return "FONT";
        case 9: return "ACCELERATOR";
        case 10: return "RCDATA";
        case 11: return "MESSAGETABLE";
        case 12: return "GROUP_CURSOR";
        case 14: return "GROUP_ICON";
        case 16: return "VERSION";
        case 17: return "DLGINCLUDE";
        case 19: return "PLUGPLAY";
        case 20: return "VXD";
        case 21: return "ANICURSOR";
        case 22: return "ANIICON";
        case 23: return "HTML";
        case 24: return "MANIFEST";
        default: return "";
    }
}

const char* preview_name(Preview p) {
    switch (p) {
        case Preview::Image: return "image";
        case Preview::Text: return "text";
        case Preview::Hex: return "hex";
    }
    return "hex";
}

std::size_t Resources::count() const {
    std::size_t n = 0;
    for (const ResourceType& t : types) n += t.items.size();
    return n;
}

std::string address_text(const Address& a) {
    if (std::holds_alternative<std::monostate>(a)) return "";
    if (const auto* linear = std::get_if<std::uint64_t>(&a)) return std::format("0x{:x}", *linear);
    const SegmentAddress& s = std::get<SegmentAddress>(a);
    switch (s.style) {
        case SegmentAddress::Style::NeSegment: return std::format("{}:{:04x}", s.segment, s.offset);
        case SegmentAddress::Style::RealMode: return std::format("{:04x}:{:04x}", s.segment, s.offset);
        case SegmentAddress::Style::Driver: return std::format("{}:{:08x}", s.segment, s.offset);
    }
    return "";
}

// ---- describe ------------------------------------------------------------------------------------------------------
Description describe(const std::filesystem::path& path, const DescribeHooks& hooks) {
    Description d;
    d.path = fs::to_text(path);
    d.name = text::lower(fs::to_text(path.filename()));
    std::error_code ec;
    if (!std::filesystem::is_regular_file(path, ec)) {
        if (std::filesystem::exists(path, ec)) describe_other(path, hooks, d);  // a directory or device: not a format
        else d.notes.emplace_back("cannot be read: not found");
        return d;
    }

    Bytes data;
    try {
        data = fs::read_file(path);
    } catch (const std::exception& e) {
        d.notes.push_back(std::format("cannot be read: {}", e.what()));
        return d;
    }
    const std::optional<detect::NewHeader> sig = detect::new_header(ByteView(data));
    if (!sig) {  // not an executable: what it is, from its header
        data.clear();
        data.shrink_to_fit();
        describe_other(path, hooks, d);
        return d;
    }
    d.size = data.size();
    d.format = *sig == detect::NewHeader::None ? "MZ" : detect::signature_text(*sig);
    d.kind = detect::lane_name(detect::lane_of(*sig));
    try {
        const std::optional<detect::DosHeader> dos = detect::dos_header(data);
        d.dos = dos;
        switch (*sig) {
            case detect::NewHeader::Pe: describe_pe(std::move(data), d.name, d); break;
            case detect::NewHeader::Ne: describe_ne(std::move(data), d); break;
            case detect::NewHeader::Le:
            case detect::NewHeader::Lx: describe_le(std::move(data), d); break;
            case detect::NewHeader::W3: describe_bundle(std::move(data), d); break;
            default: describe_dos(data.size(), dos, d); break;  // a plain DOS program (and a W4 bundle, as the Python did)
        }
        d.ok = true;
    } catch (const std::exception& e) {  // the header itself is damaged
        d.notes.push_back(std::string(e.what()).substr(0, 120));
    }
    return d;
}

// ---- the document ----------------------------------------------------------------------------------------------------
namespace {

using Json = cb::Json;

Json strings_json(const std::vector<std::string>& v) {
    Json a = Json::array();
    for (const std::string& s : v) a.push_back(s);
    return a;
}

Json address_json(const Address& a) {
    if (const auto* linear = std::get_if<std::uint64_t>(&a)) return *linear;
    return address_text(a);  // "" for none
}

Json opt_json(const std::optional<std::uint32_t>& v) { return v ? Json(*v) : Json(nullptr); }

Json section_json(const Section& s) {
    Json j = Json::object();
    j["name"] = s.name;
    j["address"] = address_json(s.address);
    j["size"] = s.size;
    j["file_offset"] = s.file_offset;
    j["file_size"] = s.file_size;
    j["flags"] = strings_json(s.flags);
    j["code"] = s.code;
    return j;
}

Json sections_json(const std::vector<Section>& v) {
    Json a = Json::array();
    for (const Section& s : v) a.push_back(section_json(s));
    return a;
}

Json export_json(const ExportSymbol& e) {
    Json j = Json::object();
    j["name"] = e.name.value_or("");
    j["ordinal"] = e.ordinal;
    j["hint"] = e.hint ? Json(*e.hint) : Json(-1);
    if (e.ne) {
        j["address"] = address_json(e.address);
        j["forward"] = e.forward.value_or("");
        j["movable"] = e.ne->movable;
        j["exported"] = e.ne->exported;
    } else {
        j["forward"] = e.forward.value_or("");
        j["address"] = address_json(e.address);
    }
    return j;
}

Json exports_json(const Exports& e) {
    Json j = Json::object();
    j["module"] = e.module;
    j["count"] = e.symbols.size();
    if (e.ordinal_base) j["ordinal_base"] = *e.ordinal_base;
    Json syms = Json::array();
    for (const ExportSymbol& s : e.symbols) syms.push_back(export_json(s));
    j["symbols"] = std::move(syms);
    return j;
}

Json imports_json(const std::vector<ImportModule>& v) {
    Json a = Json::array();
    for (const ImportModule& m : v) {
        Json j = Json::object();
        j["module"] = m.module;
        j["count"] = m.symbols.size();
        j["delay"] = m.delay;
        Json syms = Json::array();
        for (const ImportSymbol& s : m.symbols) {
            Json sj = Json::object();
            sj["name"] = s.name.value_or("");
            sj["ordinal"] = s.ordinal ? Json(*s.ordinal) : Json(-1);
            sj["hint"] = s.hint ? Json(*s.hint) : Json(-1);
            if (s.slot) sj["slot"] = *s.slot;
            syms.push_back(std::move(sj));
        }
        j["symbols"] = std::move(syms);
        a.push_back(std::move(j));
    }
    return a;
}

Json resources_json(const Resources& r) {
    Json j = Json::object();
    j["count"] = r.count();
    Json types = Json::array();
    for (const ResourceType& t : r.types) {
        Json tj = Json::object();
        tj["type"] = t.type;
        tj["id"] = t.id ? Json(*t.id) : Json(nullptr);
        tj["count"] = t.items.size();
        Json items = Json::array();
        for (const ResourceItem& it : t.items) {
            Json ij = Json::object();
            ij["name"] = it.name ? Json(*it.name) : Json(nullptr);
            if (it.from_pe) ij["id"] = opt_json(it.id);
            ij["language"] = it.language;
            if (it.from_pe) ij["rva"] = opt_json(it.rva);
            ij["offset"] = it.offset ? Json(*it.offset) : Json(nullptr);
            ij["size"] = it.size;
            ij["type_id"] = it.type_id;
            ij["preview"] = preview_name(it.preview);
            ij["wide"] = it.wide;
            items.push_back(std::move(ij));
        }
        tj["items"] = std::move(items);
        types.push_back(std::move(tj));
    }
    j["types"] = std::move(types);
    return j;
}

Json summary_json(const Summary& s) {
    Json j = Json::object();
    j["format"] = s.format;
    j["type"] = s.type;
    j["machine"] = s.machine;
    j["module"] = s.module;
    j["description"] = s.description;
    j["subsystem"] = s.subsystem;
    j["linker"] = s.linker;
    j["linked"] = s.linked;
    j["image_base"] = s.image_base;
    j["entry"] = std::holds_alternative<std::monostate>(s.entry) ? Json(0) : address_json(s.entry);
    j["code_bytes"] = s.code_bytes;
    j["sections"] = s.sections;
    j["exports"] = s.exports;
    j["import_modules"] = s.import_modules;
    j["imports"] = s.imports;
    j["resources"] = s.resources;
    return j;
}

Json facts_json(const Facts& f) {
    Json j = Json::object();
    for (const auto& [k, v] : f) j[k] = v;
    return j;
}

Json dos_header_json(const detect::DosHeader& h) {
    Json j = Json::object();
    j["bytes_in_last_page"] = h.bytes_in_last_page;
    j["pages"] = h.pages;
    j["relocations"] = h.relocations;
    j["header_paragraphs"] = h.header_paragraphs;
    j["header_size"] = h.header_size;
    j["min_alloc"] = h.min_alloc;
    j["max_alloc"] = h.max_alloc;
    j["ss"] = h.ss;
    j["sp"] = h.sp;
    j["checksum"] = h.checksum;
    j["ip"] = h.ip;
    j["cs"] = h.cs;
    j["relocation_table"] = h.relocation_table;
    j["overlay"] = h.overlay;
    j["new_header"] = h.new_header;
    j["load_size"] = h.load_size;
    return j;
}

Json le_headers_json(const LeHeaders& l) {
    const le::Header& h = l.header;
    Json j = Json::object();
    j["signature"] = l.signature;
    j["cpu"] = h.cpu;
    j["os"] = h.os;
    j["module_version"] = h.version;
    j["module_flags"] = h.module_flags;
    j["pages"] = h.pages;
    j["entry_object"] = h.eip_object;
    j["entry"] = h.eip;
    j["stack_object"] = h.esp_object;
    j["stack"] = h.esp;
    j["page_size"] = h.page_size;
    j["last_page_bytes"] = h.last_page_bytes;
    j["fixup_section_size"] = h.fixup_size;
    j["loader_section_size"] = h.loader_size;
    j["objects"] = h.objects;
    j["imported_modules"] = h.import_module_count;
    j["data_pages_offset"] = h.data_pages;
    j["device_id"] = h.device_id;
    j["ddk_version"] = h.kit_version;
    j["cpu_name"] = cpu_name(h.cpu);
    j["os_name"] = system_name(h.os, false);
    return j;
}

Json ddb_json(const std::optional<le::Ddb>& d) {
    if (!d) return nullptr;
    Json j = Json::object();
    j["obj"] = d->object;
    j["rva"] = d->rva;
    j["version"] = d->version;
    j["device_id"] = d->device_id;
    j["major"] = d->major;
    j["minor"] = d->minor;
    j["flags"] = d->flags;
    j["name"] = d->name;
    j["init_order"] = d->init_order;
    j["control_proc"] = d->control_proc;
    return j;
}

Json pe_headers_json(const PeHeaders& p) {
    Json file = Json::object(), opt = Json::object();
    if (p.read) {
        file["machine"] = p.machine;
        file["machine_id"] = p.machine_id;
        file["sections"] = p.sections;
        file["timestamp"] = p.timestamp;
        file["linked"] = when(p.timestamp);
        file["optional_header_size"] = p.optional_header_size;
        file["characteristics"] = strings_json(p.characteristics);
        const pe::OptionalHeader& o = p.optional;
        opt["magic"] = o.magic;
        opt["linker_version"] = version_text(o.linker_major, o.linker_minor);
        opt["code_size"] = o.code_size;
        opt["initialized_data_size"] = o.initialized_data_size;
        opt["uninitialized_data_size"] = o.uninitialized_data_size;
        opt["entry_point"] = o.entry_point;
        opt["code_base"] = o.code_base;
        opt["image_base"] = o.image_base;
        opt["section_alignment"] = o.section_alignment;
        opt["file_alignment"] = o.file_alignment;
        opt["os_version"] = version_text(o.os_version.major, o.os_version.minor);
        opt["image_version"] = version_text(o.image_version.major, o.image_version.minor);
        opt["subsystem_version"] = version_text(o.subsystem_version.major, o.subsystem_version.minor);
        opt["image_size"] = o.image_size;
        opt["headers_size"] = o.headers_size;
        opt["checksum"] = o.checksum;
        opt["subsystem"] = o.subsystem;
        opt["subsystem_name"] = subsystem_name(o.subsystem);
        opt["dll_characteristics"] = o.dll_characteristics;
    }
    Json j = Json::object();
    j["file"] = std::move(file);
    j["optional"] = std::move(opt);
    return j;
}

Json ne_headers_json(const ne::Header& h) {
    Json j = Json::object();
    j["linker_version"] = version_text(h.linker_version, h.linker_revision);
    j["flags"] = strings_json(flags_of(h.flags, kNeFlags));
    j["flags_value"] = h.flags;
    j["automatic_data_segment"] = h.autodata_segment;
    j["heap"] = h.heap_size;
    j["stack"] = h.stack_size;
    j["entry_segment"] = h.cs_ip >> 16;
    j["entry_offset"] = h.cs_ip & 0xFFFF;
    j["stack_segment"] = h.ss_sp >> 16;
    j["stack_pointer"] = h.ss_sp & 0xFFFF;
    j["segments"] = h.segment_count;
    j["module_references"] = h.module_ref_count;
    j["movable_entries"] = h.movable_entries;
    j["alignment_shift"] = h.alignment_shift;
    j["target_system"] = system_name(h.target_os, true);
    j["expected_windows"] = h.expected_windows_version && *h.expected_windows_version != 0
                                ? version_text(*h.expected_windows_version >> 8, *h.expected_windows_version & 0xFF)
                                : "";
    j["checksum"] = h.crc;
    return j;
}

Json headers_json(const Headers& h) {
    struct Visit {
        Json operator()(const std::monostate&) const { return Json::object(); }
        Json operator()(const PeHeaders& p) const { return pe_headers_json(p); }
        Json operator()(const NeHeaders& n) const { return ne_headers_json(n.header); }
        Json operator()(const LeHeaders& l) const { return le_headers_json(l); }
        Json operator()(const W3Headers& w) const {
            Json j = Json::object();
            j["version"] = w.version;
            j["members"] = w.members;
            return j;
        }
        Json operator()(const detect::DosHeader& d) const { return dos_header_json(d); }
    };
    return std::visit(Visit{}, h);
}

Json driver_json(const DriverDescription& d, const std::vector<std::string>& notes, const BundleMember& m) {
    Json j = Json::object();
    j["name"] = m.name;
    j["offset"] = m.offset;
    j["size"] = m.size;
    j["notes"] = strings_json(notes);
    j["headers"] = le_headers_json(d.headers);
    j["sections"] = sections_json(d.sections);
    j["imports"] = imports_json(d.imports);
    j["driver"] = ddb_json(d.driver);
    j["exports"] = exports_json(d.exports);
    j["resources"] = resources_json(d.resources);
    j["summary"] = summary_json(d.summary);
    return j;
}

Json tuple3(std::uint32_t a, std::uint32_t b, std::uint32_t c) {
    Json j = Json::array();
    j.push_back(a);
    j.push_back(b);
    j.push_back(c);
    return j;
}

// The parts only some formats have, appended to the document in the order the format fills them.
void extras_json(Json& doc, const Extras& extras) {
    if (const auto* x = std::get_if<PeExtras>(&extras)) {
        Json dirs = Json::array();
        for (const DirectoryRow& r : x->directories) {
            Json rj = Json::object();
            rj["name"] = r.name;
            rj["rva"] = r.rva;
            rj["size"] = r.size;
            dirs.push_back(std::move(rj));
        }
        doc["directories"] = std::move(dirs);
        Json rel = Json::object();
        rel["blocks"] = x->relocations.blocks;
        rel["entries"] = x->relocations.entries;
        rel["addresses"] = x->relocations.addresses;
        doc["relocations"] = std::move(rel);
        if (x->tls) {
            Json t = Json::object();
            t["start_va"] = x->tls->start_va;
            t["end_va"] = x->tls->end_va;
            t["index_va"] = x->tls->index_va;
            t["callbacks_va"] = x->tls->callbacks_va;
            doc["tls"] = std::move(t);
        } else {
            doc["tls"] = nullptr;
        }
        Json debug = Json::array();
        for (const DebugRow& r : x->debug) {
            Json rj = Json::object();
            rj["type"] = r.type;
            rj["timestamp"] = r.timestamp;
            rj["size"] = r.size;
            rj["offset"] = r.offset;
            rj["linked"] = r.linked;
            debug.push_back(std::move(rj));
        }
        doc["debug"] = std::move(debug);
        Json certs = Json::array();
        for (const pe::Certificate& c : x->certificates) {
            Json cj = Json::object();
            cj["length"] = c.length;
            cj["revision"] = c.revision;
            cj["type"] = c.type;
            certs.push_back(std::move(cj));
        }
        doc["certificates"] = std::move(certs);
        if (x->clr) {
            Json c = Json::object();
            c["runtime_version"] = version_text(x->clr->runtime.major, x->clr->runtime.minor);
            c["flags"] = x->clr->flags;
            c["entry_token"] = x->clr->entry_token;
            doc["clr"] = std::move(c);
        } else {
            doc["clr"] = nullptr;
        }
        if (x->rich) {
            Json r = Json::object();
            r["key"] = x->rich->key;
            Json entries = Json::array();
            for (const pe::RichEntry& e : x->rich->entries) entries.push_back(tuple3(e.product, e.build, e.count));
            r["entries"] = std::move(entries);
            doc["rich"] = std::move(r);
        } else {
            doc["rich"] = nullptr;
        }
        doc["load_config"] = x->load_config;
        if (x->symbols_file) {
            const pe::CodeView& cv = *x->symbols_file;
            Json c = Json::object();
            c["format"] = cv.kind == pe::CodeView::Kind::Rsds ? "RSDS" : "NB10";
            if (cv.kind == pe::CodeView::Kind::Rsds) c["guid"] = cv.guid;
            else c["signature"] = cv.signature;
            c["age"] = cv.age;
            c["pdb"] = cv.pdb;
            doc["symbols_file"] = std::move(c);
        } else {
            doc["symbols_file"] = nullptr;
        }
    } else if (const auto* n = std::get_if<NeExtras>(&extras)) {
        Json r = Json::object();
        r["records"] = n->relocation_records;
        doc["relocations"] = std::move(r);
    } else if (const auto* dx = std::get_if<DosExtras>(&extras)) {
        Json r = Json::object();
        r["records"] = dx->relocation_count;
        r["entries"] = strings_json(dx->entries);
        doc["relocations"] = std::move(r);
    } else if (const auto* drv = std::get_if<DriverExtras>(&extras)) {
        doc["driver"] = ddb_json(drv->driver);
    } else if (const auto* b = std::get_if<BundleExtras>(&extras)) {
        Json members = Json::array();
        for (const BundleMember& m : b->members) {
            if (m.driver) {
                members.push_back(driver_json(*m.driver, m.notes, m));
            } else {
                Json j = Json::object();
                j["name"] = m.name;
                j["offset"] = m.offset;
                j["size"] = m.size;
                j["notes"] = strings_json(m.notes);
                members.push_back(std::move(j));
            }
        }
        doc["members"] = std::move(members);
    }
}

}  // namespace

Json to_json(const Description& d) {
    Json j = Json::object();
    j["path"] = d.path;
    j["name"] = d.name;
    j["size"] = d.size;
    j["format"] = d.format ? Json(*d.format) : Json(nullptr);
    j["kind"] = d.kind ? Json(*d.kind) : Json(nullptr);
    j["ok"] = d.ok;
    j["notes"] = strings_json(d.notes);
    j["summary"] = d.summary ? summary_json(*d.summary) : facts_json(d.facts);
    if (!d.dos) j["dos"] = nullptr;
    else j["dos"] = *d.dos ? dos_header_json(**d.dos) : Json::object();
    j["headers"] = headers_json(d.headers);
    j["sections"] = sections_json(d.sections);
    j["exports"] = exports_json(d.exports);
    j["imports"] = imports_json(d.imports);
    j["resources"] = resources_json(d.resources);
    extras_json(j, d.extras);
    if (!d.xex.empty()) j["xex"] = facts_json(d.xex);
    return j;
}

// ---- folders -------------------------------------------------------------------------------------------------------
namespace {

struct SourceKind {
    std::string_view extension, what;
};
constexpr SourceKind kSource[] = {
    {".c", "C"}, {".h", "C/C++ header"}, {".cc", "C++"}, {".cpp", "C++"}, {".cxx", "C++"}, {".hpp", "C++ header"},
    {".hh", "C++ header"}, {".cs", "C#"}, {".java", "Java"}, {".py", "Python"}, {".asm", "assembly"}, {".s", "assembly"},
    {".inc", "assembly include"}, {".mac", "assembly macros"}, {".js", "JavaScript"}, {".ts", "TypeScript"},
    {".rs", "Rust"}, {".go", "Go"}, {".lua", "Lua"}, {".hlsl", "HLSL shader"}, {".fx", "effect shader"},
    {".ps1", "PowerShell"}, {".bat", "batch"}, {".cmd", "batch"}, {".sh", "shell"}, {".pl", "Perl"},
    {".md", "Markdown"}, {".txt", "text"}, {".json", "JSON"}, {".xml", "XML"}, {".toml", "TOML"}, {".yml", "YAML"},
    {".yaml", "YAML"}, {".ini", "INI"}, {".cfg", "config"}, {".csv", "CSV"}, {".html", "HTML"}, {".css", "CSS"},
    {".def", "module definition"}, {".mak", "makefile"}, {".cmake", "CMake"}, {".gradle", "Gradle"},
    {".csproj", "C# project"}, {".vcxproj", "C++ project"}, {".sln", "Visual Studio solution"},
    {".obj", "Wavefront OBJ model"}, {".mtl", "Wavefront material"}, {".resx", ".NET resources"}};

std::uint64_t size_or_zero(const std::filesystem::path& p, bool& ok) {
    std::error_code ec;
    const auto n = std::filesystem::file_size(p, ec);
    ok = !ec;
    return ok ? n : 0;
}

}  // namespace

std::optional<detect::Detected> text_kind(const std::filesystem::path& path) {
    const std::string name = fs::to_text(path.filename());
    const std::string ext = text::lower(fs::to_text(path.extension()));
    std::string_view what;
    if (name == "Makefile" || name == "CMakeLists.txt" || name == "LICENSE" || name == "NOTICE" || name == "README" ||
        name == "AUTHORS") {
        what = name == "Makefile" ? "makefile" : name == "CMakeLists.txt" ? "CMake" : "text";
    } else {
        for (const SourceKind& s : kSource)
            if (s.extension == ext) what = s.what;
        if (what.empty()) return std::nullopt;
    }
    try {  // text has no NUL in its first 4 KB
        const fs::MappedFile file(path);
        const ByteView head = file.view().first(std::min<std::size_t>(4096, file.view().size()));
        if (std::find(head.begin(), head.end(), std::uint8_t{0}) != head.end()) return std::nullopt;
    } catch (const Error&) {
        return std::nullopt;
    }
    return detect::Detected{"source / text: " + std::string(what), "", "source", ""};
}

namespace {

// What a plain executable's format is called in a folder map, for the ones identify() has nothing to add to.
detect::Detected executable_kind(std::string_view format) {
    std::string name;
    if (format == "PE") name = "PE executable";
    else if (format == "NE") name = "16-bit Windows (NE)";
    else if (format == "LE") name = "virtual device driver (LE)";
    else if (format == "LX") name = "OS/2 / VxD (LX)";
    else if (format == "MZ") name = "DOS program (MZ)";
    else name = std::string(format);
    return {name, "", format == "MZ" ? "dos" : "windows", "info"};
}

constexpr std::string_view kSkippedFolders[] = {".git", "__pycache__", "node_modules", ".gradle", ".vs"};

struct Tally {
    std::vector<FolderFormat> formats;
    std::unordered_map<std::string, std::size_t> format_index;  // lookup only; `formats` keeps first-seen order
    std::vector<FolderOther> others;
    std::unordered_map<std::string, std::size_t> other_index;
    std::uint64_t files = 0, bytes = 0;
};

void classify(const std::filesystem::path& p, std::uint64_t size, Tally& t) {
    std::optional<detect::Detected> m;
    if (const auto fmt = detect::format_of(p)) {
        m = executable_kind(*fmt);
        if (*fmt == "PE" || *fmt == "W3" || *fmt == "W4") {  // the CPU decides the platform; a bundle of drivers
            const auto found = detect::identify(p);
            if (!found.empty()) m = found.front();
        }
    } else {
        const auto found = detect::identify(p);
        if (!found.empty()) m = found.front();
    }
    if (!m) m = text_kind(p);
    const std::string example = fs::to_text(p);
    if (!m) {
        std::string ext = text::lower(fs::to_text(p.extension()));
        if (ext.empty()) ext = "(none)";
        auto [slot, inserted] = t.other_index.try_emplace(ext, t.others.size());
        if (inserted) t.others.push_back({ext, 0, 0, example});
        t.others[slot->second].files++;
        t.others[slot->second].bytes += size;
        return;
    }
    const std::string detail = m->format == "PE executable" ? m->detail : "";
    const std::string key = m->format + '\n' + detail;
    auto [slot, inserted] = t.format_index.try_emplace(key, t.formats.size());
    if (inserted)
        t.formats.push_back({detail.empty() ? m->format : m->format + " (" + detail + ")", m->family, m->command, 0, 0, example});
    t.formats[slot->second].files++;
    t.formats[slot->second].bytes += size;
}

void walk(const std::filesystem::path& dir, Tally& t, const std::function<void(std::string_view)>& progress) {
    std::vector<std::string> dirs, files;  // names, sorted: the walk is deterministic
    std::error_code ec;
    for (std::filesystem::directory_iterator it(dir, ec), end; !ec && it != end; it.increment(ec)) {
        const std::string name = fs::to_text(it->path().filename());
        std::error_code type_ec;
        if (it->is_symlink(type_ec)) continue;  // a link is not followed (it may lead out of the folder or loop)
        if (it->is_directory(type_ec)) {
            if (std::find(std::begin(kSkippedFolders), std::end(kSkippedFolders), name) == std::end(kSkippedFolders))
                dirs.push_back(name);
        } else {
            files.push_back(name);
        }
    }
    std::sort(files.begin(), files.end());
    std::sort(dirs.begin(), dirs.end());
    for (const std::string& name : files) {
        const std::filesystem::path p = dir / fs::from_text(name);
        bool ok = false;
        const std::uint64_t size = size_or_zero(p, ok);
        if (!ok) continue;
        t.files++;
        t.bytes += size;
        if (progress && t.files % 2000 == 0) progress(std::format("  {} files...", t.files));
        if (size == 0) continue;
        classify(p, size, t);
    }
    for (const std::string& name : dirs) walk(dir / fs::from_text(name), t, progress);
}

template <class Row>
void largest_first(std::vector<Row>& rows) {
    std::stable_sort(rows.begin(), rows.end(), [](const Row& a, const Row& b) {
        return a.bytes != b.bytes ? a.bytes > b.bytes : a.files > b.files;
    });
}

}  // namespace

FolderMap folder_map(const std::filesystem::path& folder, const std::function<void(std::string_view)>& progress) {
    Tally t;
    walk(folder, t, progress);
    largest_first(t.formats);
    largest_first(t.others);
    return {fs::to_text(folder), t.files, t.bytes, std::move(t.formats), std::move(t.others)};
}

Json to_json(const FolderMap& m) {
    Json j = Json::object();
    j["folder"] = m.folder;
    j["files"] = m.files;
    j["bytes"] = m.bytes;
    Json formats = Json::array();
    for (const FolderFormat& f : m.formats) {
        Json fj = Json::object();
        fj["format"] = f.format;
        fj["family"] = f.family;
        fj["command"] = f.command;
        fj["files"] = f.files;
        fj["bytes"] = f.bytes;
        fj["example"] = f.example;
        formats.push_back(std::move(fj));
    }
    j["formats"] = std::move(formats);
    Json others = Json::array();
    for (const FolderOther& o : m.unrecognised) {
        Json oj = Json::object();
        oj["extension"] = o.extension;
        oj["files"] = o.files;
        oj["bytes"] = o.bytes;
        oj["example"] = o.example;
        others.push_back(std::move(oj));
    }
    j["unrecognised"] = std::move(others);
    return j;
}

// ---- an .xex's own header ---------------------------------------------------------------------------------------------
Facts xex_facts(const std::filesystem::path& path) {
    if (!xex::probe(path)) return {};
    const xex::Header h = xex::read_header(path);
    Facts out;
    out.emplace_back("image name", h.name.value_or(""));
    out.emplace_back("load address", std::format("0x{:08X}", h.base.value_or(0)));
    out.emplace_back("entry point", std::format("0x{:08X}", h.entry.value_or(0)));
    out.emplace_back("import libraries", text::join(h.imports, ", "));
    if (!h.libraries.empty()) {
        std::vector<std::string> built;
        for (const xex::Library& l : h.libraries) built.push_back(l.name + " " + l.version);
        out.emplace_back("built with", text::join(built, ", "));
    }
    return out;
}

}  // namespace cb::formats::inspect
