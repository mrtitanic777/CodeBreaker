// formats/format_compare_tables.cpp -- the tables behind the headers (PE imports, exports, resources, relocations; NE name
// tables, module references, segment relocations, resources; LE objects, page map, names, entries, fix-ups): the engine's
// tree of a file, with every part of the description read, against what the hand-written reader says of the same table.
// (The same tree is what the readers of pe.cpp, ne.cpp and le.cpp now answer from: format_check then proves the
// answers equal the tree's own values, read here a second way.)
#include <algorithm>
#include <format>
#include <map>
#include <unordered_map>

#include "core/error.hpp"
#include "formats/cab.hpp"
#include "core/text.hpp"
#include "formats/detect.hpp"
#include "formats/disc_image.hpp"
#include "formats/dos.hpp"
#include "formats/format_compare_kit.hpp"
#include "formats/iso9660.hpp"
#include "formats/le.hpp"
#include "formats/mips_elf.hpp"
#include "formats/ne.hpp"
#include "formats/nes.hpp"
#include "formats/nro.hpp"
#include "formats/omf.hpp"
#include "formats/orbis.hpp"
#include "formats/pe.hpp"
#include "formats/ppc_elf.hpp"
#include "formats/ps3.hpp"
#include "formats/vita.hpp"
#include "formats/xbox/pck.hpp"
#include "formats/xbox/xbe.hpp"
#include "formats/xbox/xdbf.hpp"
#include "formats/xbox/xpr.hpp"
#include "formats/xbox/xur.hpp"
#include "formats/xbox/xzp.hpp"

namespace cb::formats::engine {

namespace {

// The text of a Str node up to its first NUL, decoded as the readers decode it.
std::string tbl_latin1(const Node& n) {
    const std::size_t z = n.text.find('\0');
    return text::from_latin1(as_bytes(z == std::string::npos ? n.text : std::string_view(n.text).substr(0, z)));
}

// A length-prefixed name keeps every byte it holds (NULs too).
std::string tbl_whole(const Node& parent, std::string_view child) {
    const Node* n = parent.child(child);
    return n ? text::from_latin1(as_bytes(n->text)) : std::string();
}

std::string tbl_latin1_at(const Node& parent, std::string_view child) {
    const Node* n = parent.child(child);
    return n ? tbl_latin1(*n) : std::string();
}

std::uint64_t tbl_or0(const Node& n, std::string_view child) {
    const Node* c = n.child(child);
    return c ? c->value : 0;
}

constexpr std::uint64_t kHigh = 0x80000000u;

// ---- PE ----

struct TblSymbol {
    std::optional<std::string> name;
    std::uint64_t ordinal = 0, rva = 0;
    std::optional<std::uint32_t> hint;
    std::optional<std::string> forward;
};

void tbl_pe_exports(Checker& k, const Node& d, const pe::Image& image) {
    const pe::ExportTable t = image.export_table();
    const Node* ex = d.find("exports");
    if (!ex) {
        k.eq("exports: none", t.symbols.size() + t.module.size(), 0);
        return;
    }
    const std::uint64_t base = ex->u("ordinal_base"), n_funcs = ex->u("n_functions"), n_names = ex->u("n_names");
    k.eq("export_table.ordinal_base", base, t.ordinal_base);
    k.eq("export_table.timestamp", ex->u("timestamp"), t.timestamp);
    k.eq_text("export_table.module", tbl_latin1_at(*ex, "name"), t.module);
    // name -> (slot, index in the name table); a later name for a slot wins
    std::unordered_map<std::uint64_t, std::pair<std::string, std::uint32_t>> named;
    const Node* names = ex->child("names");
    const Node* ords = ex->child("ordinals");
    if (names && ords) {
        const std::size_t n = std::min<std::size_t>({std::min<std::uint64_t>(n_names, 0x10000), names->children.size(), ords->children.size()});
        for (std::size_t i = 0; i < n; ++i) named[ords->children[i].value] = {tbl_latin1_at(names->children[i], "name"), static_cast<std::uint32_t>(i)};
    }
    std::vector<TblSymbol> mine;
    if (const Node* funcs = ex->child("functions")) {
        const std::uint64_t dir_rva = d.u("nt.optional.directories[0].rva"), dir_size = d.u("nt.optional.directories[0].size");
        for (std::size_t i = 0; i < std::min<std::uint64_t>(n_funcs, std::min<std::size_t>(funcs->children.size(), 0x10000)); ++i) {
            const std::uint64_t rva = funcs->children[i].u("rva");
            if (!rva) continue;
            TblSymbol s;
            s.ordinal = base + i;
            s.rva = rva;
            if (auto it = named.find(i); it != named.end()) {
                s.name = it->second.first;
                s.hint = it->second.second;
            }
            if (dir_rva <= rva && rva < dir_rva + dir_size) s.forward = tbl_latin1_at(funcs->children[i], "forward");
            mine.push_back(std::move(s));
        }
    }
    k.eq("export_table.symbols", mine.size(), t.symbols.size());
    for (std::size_t i = 0; i < std::min(mine.size(), t.symbols.size()); ++i) {
        const std::string p = std::format("export[{}].", i);
        k.eq(p + "ordinal", mine[i].ordinal, t.symbols[i].ordinal);
        k.eq(p + "rva", mine[i].rva, t.symbols[i].rva);
        k.eq(p + "has name", mine[i].name.has_value(), t.symbols[i].name.has_value());
        if (mine[i].name && t.symbols[i].name) k.eq_text(p + "name", *mine[i].name, *t.symbols[i].name);
        k.eq(p + "hint", mine[i].hint.value_or(0xFFFFFFFFu), t.symbols[i].hint.value_or(0xFFFFFFFFu));
        k.eq(p + "has forward", mine[i].forward.has_value(), t.symbols[i].forward.has_value());
        if (mine[i].forward && t.symbols[i].forward) k.eq_text(p + "forward", *mine[i].forward, *t.symbols[i].forward);
    }
}

// The loader's view of the exports and the module name (a damaged table throws there: nothing to compare then).
void tbl_pe_loader_exports(Checker& k, const Node& d, const pe::Image& image) {
    std::vector<pe::Export> theirs;
    std::string module;
    try {
        theirs = image.exports();
        module = image.module_name();
    } catch (const Error&) {
        return;
    }
    const Node* ex = d.find("exports");
    k.eq_text("module_name", ex ? tbl_latin1_at(*ex, "name") : std::string(), module);
    struct Mine {
        std::uint64_t ordinal, rva;
        std::optional<std::string> name;
    };
    std::vector<Mine> mine;
    if (ex) {
        const Node* funcs = ex->child("functions");
        if (funcs) {
            const std::uint64_t base = ex->u("ordinal_base");
            for (std::size_t i = 0; i < funcs->children.size(); ++i) mine.push_back({base + i, funcs->children[i].u("rva"), std::nullopt});
            const Node* names = ex->child("names");
            const Node* ords = ex->child("ordinals");
            if (names && ords) {
                const std::size_t n = std::min(names->children.size(), ords->children.size());
                for (std::size_t i = 0; i < n; ++i) {
                    const Node* nm = names->children[i].child("name");
                    const std::uint64_t index = ords->children[i].value;
                    if (nm && index < mine.size()) mine[index].name = tbl_latin1(*nm);
                }
            }
            std::erase_if(mine, [](const Mine& m) { return m.rva == 0; });
        }
    }
    k.eq("exports.count", mine.size(), theirs.size());
    for (std::size_t i = 0; i < std::min(mine.size(), theirs.size()); ++i) {
        const std::string p = std::format("exports[{}].", i);
        k.eq(p + "ordinal", mine[i].ordinal, theirs[i].ordinal);
        k.eq(p + "rva", mine[i].rva, theirs[i].rva);
        k.eq(p + "has name", mine[i].name.has_value(), theirs[i].name.has_value());
        if (mine[i].name && theirs[i].name) k.eq_text(p + "name", *mine[i].name, *theirs[i].name);
    }
}

struct TblThunk {
    bool by_ordinal = false;
    std::uint64_t ordinal = 0, hint = 0;
    bool has_name = false;
    std::string name;
};

std::vector<TblThunk> tbl_thunks(const Node& descriptor) {
    std::vector<TblThunk> out;
    const Node* thunks = descriptor.child("thunks");
    if (!thunks) return out;
    for (const Node& t : thunks->children) {
        if (t.u("value") == 0) break;
        TblThunk x;
        x.by_ordinal = t.u("by_ordinal") != 0;
        x.ordinal = t.u("ordinal");
        if (const Node* hn = t.child("hint_name")) {
            x.has_name = true;
            x.hint = hn->u("hint");
            x.name = tbl_latin1_at(*hn, "name");
        }
        out.push_back(std::move(x));
    }
    return out;
}

void tbl_pe_imports(Checker& k, const Node& d, const pe::Image& image) {
    const bool plus = d.u("nt.optional.magic") == 0x20B;
    const std::uint64_t width = plus ? 8 : 4, image_base = d.u("nt.optional.image_base");
    const Node* imports = d.find("imports");
    // the tolerant table (imports and delay-load imports)
    for (int delay = 0; delay < 2; ++delay) {
        const Node* table = delay ? d.find("delay_imports") : imports;
        const std::vector<pe::ImportModule> theirs = image.import_table(delay != 0);
        std::vector<pe::ImportModule> mine;
        if (table) {
            for (std::size_t i = 0; i < std::min<std::size_t>(table->children.size(), 4096); ++i) {
                const Node& desc = table->children[i];
                if (desc.u("is_end")) break;
                pe::ImportModule m;
                for (const TblThunk& t : tbl_thunks(desc)) {
                    pe::ImportSymbol s;
                    s.slot_rva = desc.u(delay ? "iat_rva" : "iat") + m.symbols.size() * width;
                    if (t.by_ordinal) {
                        s.ordinal = static_cast<std::uint16_t>(t.ordinal);
                    } else if (t.has_name) {
                        s.name = t.name;
                        s.hint = static_cast<std::uint32_t>(t.hint);
                    } else {
                        break;   // an address (bound), not an RVA: the module's list ends
                    }
                    m.symbols.push_back(std::move(s));
                }
                m.module = tbl_latin1_at(desc, "name");
                if (m.module.empty()) m.module = "?";
                mine.push_back(std::move(m));
            }
        }
        const std::string w = delay ? "delay_imports" : "import_table";
        k.eq(w + ".modules", mine.size(), theirs.size());
        for (std::size_t i = 0; i < std::min(mine.size(), theirs.size()); ++i) {
            k.eq_text(std::format("{}[{}].module", w, i), mine[i].module, theirs[i].module);
            k.eq(std::format("{}[{}].symbols", w, i), mine[i].symbols.size(), theirs[i].symbols.size());
            for (std::size_t j = 0; j < std::min(mine[i].symbols.size(), theirs[i].symbols.size()); ++j) {
                const pe::ImportSymbol& a = mine[i].symbols[j];
                const pe::ImportSymbol& b = theirs[i].symbols[j];
                const std::string p = std::format("{}[{}][{}].", w, i, j);
                k.eq(p + "slot_rva", a.slot_rva, b.slot_rva);
                k.eq(p + "ordinal", a.ordinal.value_or(0xFFFFFFu), b.ordinal.value_or(0xFFFFFFu));
                k.eq(p + "hint", a.hint.value_or(0xFFFFFFFFu), b.hint.value_or(0xFFFFFFFFu));
                k.eq(p + "has name", a.name.has_value(), b.name.has_value());
                if (a.name && b.name) k.eq_text(p + "name", *a.name, *b.name);
            }
        }
    }

    // the loader's list: the import address table slots, a slot met again keeping its place
    std::vector<pe::Import> theirs;
    try {
        theirs = image.imports();
    } catch (const Error&) {
        return;
    }
    std::vector<pe::Import> mine;
    std::unordered_map<std::uint64_t, std::size_t> slot_index;
    if (imports) {
        for (const Node& desc : imports->children) {
            if (desc.u("lookup") == 0 && desc.u("iat") == 0) break;
            std::string dll = tbl_latin1_at(desc, "name");
            std::uint64_t j = 0;
            for (const TblThunk& t : tbl_thunks(desc)) {
                std::string name = t.by_ordinal ? "#" + std::to_string(t.ordinal) : (t.has_name ? t.name : "?");
                const std::uint64_t slot = image_base + desc.u("iat") + j++ * width;
                auto [it, fresh] = slot_index.try_emplace(slot, mine.size());
                if (fresh) {
                    mine.push_back({slot, dll, std::move(name)});
                } else {
                    mine[it->second].dll = dll;
                    mine[it->second].name = std::move(name);
                }
            }
        }
    }
    k.eq("imports.count", mine.size(), theirs.size());
    for (std::size_t i = 0; i < std::min(mine.size(), theirs.size()); ++i) {
        const std::string p = std::format("import[{}].", i);
        k.eq(p + "slot", mine[i].slot_va, theirs[i].slot_va);
        k.eq_text(p + "dll", mine[i].dll, theirs[i].dll);
        k.eq_text(p + "name", mine[i].name, theirs[i].name);
    }
}

std::string tbl_resource_label(const Node& entry) {
    const Node* name = entry.child("name");
    const Node* chars = name ? name->child("text") : nullptr;
    return chars ? text::from_utf16le(as_bytes(chars->text)) : std::string();
}

void tbl_pe_resources(Checker& k, const Node& d, const pe::Image& image) {
    const std::vector<pe::ResourceType> theirs = image.resource_tree();
    std::vector<pe::ResourceType> mine;
    if (const Node* dir = d.find("resources")) {
        if (const Node* types = dir->child("entries")) {
            for (const Node& te : types->children) {
                pe::ResourceType rt;
                if (te.u("ident") & kHigh) rt.name = tbl_resource_label(te);
                else rt.id = static_cast<std::uint32_t>(te.u("ident"));
                const Node* names = te.child("names");
                const Node* name_entries = names ? names->child("entries") : nullptr;
                if (name_entries) {
                    for (const Node& ne : name_entries->children) {
                        const Node* langs = ne.child("languages");
                        const Node* lang_entries = langs ? langs->child("entries") : nullptr;
                        if (!lang_entries) continue;
                        for (const Node& le : lang_entries->children) {
                            const Node* data = le.child("data");
                            if (!data) continue;
                            pe::ResourceItem it;
                            if (ne.u("ident") & kHigh) it.name = tbl_resource_label(ne);
                            else it.id = static_cast<std::uint32_t>(ne.u("ident"));
                            it.language = (le.u("ident") & kHigh) ? 0 : static_cast<std::uint32_t>(le.u("ident"));
                            it.rva = static_cast<std::uint32_t>(data->u("rva"));
                            it.file_offset = image.file_offset(it.rva);
                            it.size = static_cast<std::uint32_t>(data->u("size"));
                            rt.items.push_back(std::move(it));
                        }
                    }
                }
                mine.push_back(std::move(rt));
            }
        }
    }
    k.eq("resource types", mine.size(), theirs.size());
    for (std::size_t i = 0; i < std::min(mine.size(), theirs.size()); ++i) {
        const std::string p = std::format("resource[{}].", i);
        k.eq(p + "has name", mine[i].name.has_value(), theirs[i].name.has_value());
        if (mine[i].name && theirs[i].name) k.eq_text(p + "name", *mine[i].name, *theirs[i].name);
        k.eq(p + "id", mine[i].id.value_or(0xFFFFFFFFu), theirs[i].id.value_or(0xFFFFFFFFu));
        k.eq(p + "items", mine[i].items.size(), theirs[i].items.size());
        for (std::size_t j = 0; j < std::min(mine[i].items.size(), theirs[i].items.size()); ++j) {
            const pe::ResourceItem& a = mine[i].items[j];
            const pe::ResourceItem& b = theirs[i].items[j];
            const std::string q = std::format("{}item[{}].", p, j);
            k.eq(q + "has name", a.name.has_value(), b.name.has_value());
            if (a.name && b.name) k.eq_text(q + "name", *a.name, *b.name);
            k.eq(q + "id", a.id.value_or(0xFFFFFFFFu), b.id.value_or(0xFFFFFFFFu));
            k.eq(q + "language", a.language, b.language);
            k.eq(q + "rva", a.rva, b.rva);
            k.eq(q + "file_offset", a.file_offset.value_or(~std::uint64_t{0}), b.file_offset.value_or(~std::uint64_t{0}));
            k.eq(q + "size", a.size, b.size);
        }
    }
}

void tbl_pe_relocs(Checker& k, const Node& d, const pe::Image& image) {
    const Node* blocks = d.find("relocs");
    std::vector<pe::RelocBlock> mine;
    if (blocks) {
        for (const Node& b : blocks->children) {
            if (b.u("block_size") < 8) break;
            mine.push_back({static_cast<std::uint32_t>(b.u("page_rva")), static_cast<std::uint32_t>((b.u("block_size") - 8) / 2)});
        }
    }
    const std::vector<pe::RelocBlock> theirs = image.reloc_blocks();
    k.eq("reloc blocks", mine.size(), theirs.size());
    for (std::size_t i = 0; i < std::min(mine.size(), theirs.size()); ++i) {
        k.eq(std::format("reloc[{}].page_rva", i), mine[i].page_rva, theirs[i].page_rva);
        k.eq(std::format("reloc[{}].entries", i), mine[i].entries, theirs[i].entries);
    }
    std::vector<std::uint64_t> all;
    bool damaged = false;
    std::vector<std::uint64_t> expected;
    try {
        expected = image.base_relocs();
    } catch (const Error&) {
        damaged = true;
    }
    if (damaged) return;
    if (blocks) {
        for (const Node& b : blocks->children) {
            if (b.u("block_size") < 8) break;
            if (const Node* entries = b.child("entries"))
                for (const Node& e : entries->children)
                    if ((e.value >> 12) == 3) all.push_back(b.u("page_rva") + (e.value & 0x0FFF));
        }
    }
    std::sort(all.begin(), all.end());
    all.erase(std::unique(all.begin(), all.end()), all.end());
    k.eq("base relocations", all.size(), expected.size());
    for (std::size_t i = 0; i < std::min(all.size(), expected.size()); ++i) k.eq(std::format("base_reloc[{}]", i), all[i], expected[i]);
}

// ---- NE ----

void tbl_ne_names(Checker& k, const Node& ne, std::string_view what, const Node* table, const std::vector<ne::Name>& theirs) {
    std::vector<ne::Name> mine;
    if (table)
        for (const Node& e : table->children)
            if (e.u("length") != 0) mine.push_back({tbl_whole(e, "name"), static_cast<std::uint16_t>(e.u("ordinal"))});
    (void)ne;
    k.eq(std::format("{} count", what), mine.size(), theirs.size());
    for (std::size_t i = 0; i < std::min(mine.size(), theirs.size()); ++i) {
        k.eq_text(std::format("{}[{}].name", what, i), mine[i].name, theirs[i].name);
        k.eq(std::format("{}[{}].ordinal", what, i), mine[i].ordinal, theirs[i].ordinal);
    }
}

std::string tbl_pstring(const Node& parent) {
    const Node* n = parent.child("name");
    return n ? tbl_whole(*n, "text") : std::string();
}

void tbl_ne_tables(Checker& k, const Node& d, const ne::Module& module) {
    const Node& n = d.get("ne");
    try {
        tbl_ne_names(k, n, "resident name", n.child("resident_names"), module.resident_names());
        tbl_ne_names(k, n, "nonresident name", n.child("nonresident_names"), module.nonresident_names());
    } catch (const Error&) {   // a damaged table: the reader refuses it
    }
    try {
        const std::vector<std::string> refs = module.module_refs();
        const Node* mine = n.child("module_refs");
        k.eq("module refs", mine ? mine->children.size() : 0, refs.size());
        if (mine)
            for (std::size_t i = 0; i < std::min(mine->children.size(), refs.size()); ++i)
                k.eq_text(std::format("module_ref[{}]", i), tbl_pstring(mine->children[i]), refs[i]);
    } catch (const Error&) {
    }

    // the relocation records of every segment
    const std::vector<ne::Segment> segments = module.segments();
    const Node& segs = n.get("segments");
    for (std::size_t i = 0; i < std::min(segs.children.size(), segments.size()); ++i) {
        const std::vector<ne::Relocation> theirs = module.relocations(segments[i]);
        const Node* table = segs.children[i].child("relocs");
        const Node* records = table ? table->child("records") : nullptr;
        const std::string p = std::format("segment[{}].", i);
        k.eq(p + "relocations", records ? records->children.size() : 0, theirs.size());
        if (!records) continue;
        for (std::size_t j = 0; j < std::min(records->children.size(), theirs.size()); ++j) {
            const Node& r = records->children[j];
            const ne::Relocation& t = theirs[j];
            const std::string q = std::format("{}reloc[{}].", p, j);
            k.eq(q + "source", r.u("source") & 0x0F, t.source);
            k.eq(q + "flags", r.u("flags"), t.flags);
            k.eq(q + "offset", r.u("offset"), t.offset);
            switch (r.u("flags") & 3) {
                case 0:
                    k.eq(q + "internal.segment", r.u("b4") & 0xFF, std::get<ne::InternalRef>(t.target).segment);
                    k.eq(q + "internal.entry", r.u("b6"), std::get<ne::InternalRef>(t.target).entry);
                    break;
                case 1:
                    k.eq(q + "ordinal.module", r.u("b4"), std::get<ne::ImportOrdinalRef>(t.target).module);
                    k.eq(q + "ordinal.ordinal", r.u("b6"), std::get<ne::ImportOrdinalRef>(t.target).ordinal);
                    break;
                case 2:
                    k.eq(q + "name.module", r.u("b4"), std::get<ne::ImportNameRef>(t.target).module);
                    k.eq(q + "name.offset", r.u("b6"), std::get<ne::ImportNameRef>(t.target).name_offset);
                    break;
                default: k.eq(q + "os.type", r.u("b4"), std::get<ne::OsFixupRef>(t.target).fixup_type); break;
            }
        }
    }

    // the resource table
    std::vector<ne::Resource> theirs;
    try {
        theirs = module.resources();
    } catch (const Error&) {
        return;
    }
    std::vector<ne::Resource> mine;
    if (const Node* table = n.child("resources")) {
        const std::uint64_t shift = table->u("shift");
        for (const Node& type : table->child("types")->children) {
            const std::uint64_t id = type.u("type_id");
            if (id == 0) break;
            std::string type_name;
            std::optional<std::uint16_t> type_number;
            if (id & 0x8000) {
                type_number = static_cast<std::uint16_t>(id & 0x7FFF);
                const char* known = ne::resource_type_name(*type_number);
                type_name = known ? known : "#" + std::to_string(*type_number);
            } else {
                type_name = tbl_pstring(type);
            }
            for (const Node& item : type.get("items").children) {
                ne::Resource r;
                r.type = type_name;
                r.type_number = type_number;
                r.file_offset = item.u("offset") << shift;
                r.length = item.u("length") << shift;
                r.flags = static_cast<std::uint16_t>(item.u("flags"));
                const std::uint64_t rid = item.u("id");
                if (rid & 0x8000) {
                    r.id_number = static_cast<std::uint16_t>(rid & 0x7FFF);
                    r.id = "#" + std::to_string(*r.id_number);
                } else {
                    r.id = tbl_pstring(item);
                }
                mine.push_back(std::move(r));
            }
        }
    }
    k.eq("resources", mine.size(), theirs.size());
    for (std::size_t i = 0; i < std::min(mine.size(), theirs.size()); ++i) {
        const std::string p = std::format("resource[{}].", i);
        k.eq_text(p + "type", mine[i].type, theirs[i].type);
        k.eq_text(p + "id", mine[i].id, theirs[i].id);
        k.eq(p + "type_number", mine[i].type_number.value_or(0xFFFFFF), theirs[i].type_number.value_or(0xFFFFFF));
        k.eq(p + "id_number", mine[i].id_number.value_or(0xFFFFFF), theirs[i].id_number.value_or(0xFFFFFF));
        k.eq(p + "file_offset", mine[i].file_offset, theirs[i].file_offset);
        k.eq(p + "length", mine[i].length, theirs[i].length);
        k.eq(p + "flags", mine[i].flags, theirs[i].flags);
    }
}

// ---- LE ----

void tbl_le_tables(Checker& k, const Node& d, ByteView data, const le::Module& module) {
    const Node& n = d.get("le");
    const le::Header& h = module.header();
    k.eq("signature", n.str("signature") == "LX", module.signature() == "LX");
    k.eq("byte_order", n.u("byte_order"), h.byte_order);
    k.eq("word_order", n.u("word_order"), h.word_order);
    k.eq("level", n.u("level"), h.level);
    k.eq("cpu", n.u("cpu"), h.cpu);
    k.eq("os", n.u("os"), h.os);
    k.eq("version", n.u("version"), h.version);
    k.eq("module_flags", n.u("module_flags"), h.module_flags);
    k.eq("pages", n.u("pages"), h.pages);
    k.eq("eip_object", n.u("eip_object"), h.eip_object);
    k.eq("eip", n.u("eip"), h.eip);
    k.eq("esp_object", n.u("esp_object"), h.esp_object);
    k.eq("esp", n.u("esp"), h.esp);
    k.eq("page_size", n.u("page_size"), h.page_size);
    k.eq("last_page_bytes", n.u("last_page_bytes"), h.last_page_bytes);
    k.eq("fixup_size", n.u("fixup_size"), h.fixup_size);
    k.eq("loader_size", n.u("loader_size"), h.loader_size);
    k.eq("object_table", n.u("object_table"), h.object_table);
    k.eq("objects", n.u("objects"), h.objects);
    k.eq("page_map", n.u("page_map"), h.page_map);
    k.eq("resources", n.u("resources"), h.resources);
    k.eq("resident_names", n.u("resident_names"), h.resident_names);
    k.eq("entry_table", n.u("entry_table"), h.entry_table);
    k.eq("fixup_pages", n.u("fixup_pages"), h.fixup_pages);
    k.eq("fixup_records", n.u("fixup_records"), h.fixup_records);
    k.eq("import_modules", n.u("import_modules"), h.import_modules);
    k.eq("import_module_count", n.u("import_module_count"), h.import_module_count);
    k.eq("import_names", n.u("import_names"), h.import_names);
    k.eq("data_pages", n.u("data_pages"), h.data_pages);
    k.eq("preload_pages", n.u("preload_pages"), h.preload_pages);
    k.eq("nonresident_names", n.u("nonresident_names"), h.nonresident_names);
    k.eq("nonresident_size", n.u("nonresident_size"), h.nonresident_size);
    k.eq("debug_info", n.u("debug_info"), h.debug_info);
    k.eq("debug_size", n.u("debug_size"), h.debug_size);
    k.eq("resource_data", n.u("resource_data"), h.resource_data);
    k.eq("resource_size", n.u("resource_size"), h.resource_size);
    k.eq("device_id", n.u("device_id"), h.device_id);
    k.eq("kit_version", n.u("kit_version"), h.kit_version);

    const auto objects = module.objects();
    const Node& objs = n.get("object_entries");
    k.eq("object count", objs.children.size(), objects.size());
    for (std::size_t i = 0; i < std::min(objs.children.size(), objects.size()); ++i) {
        const Node& o = objs.children[i];
        const std::string p = std::format("object[{}].", i);
        k.eq(p + "vsize", o.u("vsize"), objects[i].vsize);
        k.eq(p + "base", o.u("base"), objects[i].base);
        k.eq(p + "flags", o.u("flags"), objects[i].flags);
        k.eq(p + "first_page", o.u("first_page"), objects[i].first_page);
        k.eq(p + "page_count", o.u("page_count"), objects[i].page_count);
    }

    try {
        const std::vector<le::PageEntry> pm = module.page_map();
        const Node* mine = n.child("page_entries");
        k.eq("page map", mine ? mine->children.size() : 0, pm.size());
        if (mine)
            for (std::size_t i = 0; i < std::min(mine->children.size(), pm.size()); ++i) {
                k.eq(std::format("page[{}].number", i), mine->children[i].u("number"), pm[i].number);
                k.eq(std::format("page[{}].kind", i), mine->children[i].u("kind"), pm[i].kind);
            }
    } catch (const Error&) {
    }

    try {
        const auto [res, non] = module.names();
        const Node* r = n.child("resident");
        std::vector<le::NameOrdinal> mine_res, mine_non;
        if (r)
            for (const Node& e : r->children)
                if (e.u("length") != 0) mine_res.push_back({tbl_whole(e, "name"), static_cast<std::uint16_t>(e.u("ordinal"))});
        k.eq("resident names", mine_res.size(), res.size());
        for (std::size_t i = 0; i < std::min(mine_res.size(), res.size()); ++i) {
            k.eq_text(std::format("resident[{}].name", i), mine_res[i].name, res[i].name);
            k.eq(std::format("resident[{}].ordinal", i), mine_res[i].ordinal, res[i].ordinal);
        }
        if (!module.is_member())
            if (const Node* nr = n.child("nonresident"))
                for (const Node& e : nr->children)
                    if (e.u("length") != 0) mine_non.push_back({tbl_whole(e, "name"), static_cast<std::uint16_t>(e.u("ordinal"))});
        k.eq("nonresident names", mine_non.size(), non.size());
        for (std::size_t i = 0; i < std::min(mine_non.size(), non.size()); ++i) {
            k.eq_text(std::format("nonresident[{}].name", i), mine_non[i].name, non[i].name);
            k.eq(std::format("nonresident[{}].ordinal", i), mine_non[i].ordinal, non[i].ordinal);
        }
    } catch (const Error&) {
    }

    try {
        const std::vector<std::string> modules = module.import_modules();
        const Node* mine = n.child("import_module_names");
        k.eq("import modules", mine ? mine->children.size() : 0, modules.size());
        if (mine)
            for (std::size_t i = 0; i < std::min(mine->children.size(), modules.size()); ++i)
                k.eq_text(std::format("import_module[{}]", i), tbl_whole(mine->children[i], "text"), modules[i]);
    } catch (const Error&) {
    }

    // the entry table: the bundles flattened to numbered entries
    {
        const std::vector<le::EntryPoint> theirs = module.entries();
        std::vector<le::EntryPoint> mine;
        std::uint32_t ordinal = 1;
        const Node* table = n.child("entries");
        if (table) {
            for (const Node& b : table->children) {
                const std::uint64_t count = b.u("count");
                if (count == 0) break;
                const std::uint64_t kind = b.u("kind");
                if (kind == 0) {
                    ordinal += static_cast<std::uint32_t>(count);
                    continue;
                }
                for (const Node& e : b.get("entries").children)
                    mine.push_back({ordinal++, kind != 4 ? static_cast<std::uint32_t>(b.u("object")) : 0u, static_cast<std::uint32_t>(e.u("offset")),
                                    static_cast<std::uint8_t>(e.u("flags"))});
            }
        }
        k.eq("entry count", mine.size(), theirs.size());
        for (std::size_t i = 0; i < std::min(mine.size(), theirs.size()); ++i) {
            const std::string p = std::format("entry[{}].", i);
            k.eq(p + "ordinal", mine[i].ordinal, theirs[i].ordinal);
            k.eq(p + "object", mine[i].object, theirs[i].object);
            k.eq(p + "offset", mine[i].offset, theirs[i].offset);
            k.eq(p + "flags", mine[i].flags, theirs[i].flags);
        }
        if (table && !table->children.empty() && table->children.back().u("count") == 0)
            k.eq("entry table size", table->size, module.entry_table_size());
    }

    // the fix-up records
    try {
        const std::vector<le::FixupRecord> theirs = module.fixup_records();
        std::vector<const Node*> mine;
        if (const Node* pages = n.child("fixup_page_records"))
            for (const Node& page : pages->children)
                if (const Node* recs = page.child("records"))
                    for (const Node& r : recs->children) mine.push_back(&r);
        k.eq("fixup records", mine.size(), theirs.size());
        std::size_t page = 0, in_page = 0;
        const Node* pages = n.child("fixup_page_records");
        for (std::size_t i = 0; i < std::min(mine.size(), theirs.size()); ++i) {
            const Node& r = *mine[i];
            const le::FixupRecord& t = theirs[i];
            const std::string p = std::format("fixup[{}].", i);
            while (pages && page < pages->children.size() &&
                   (!pages->children[page].child("records") || in_page >= pages->children[page].get("records").children.size())) {
                ++page;
                in_page = 0;
            }
            ++in_page;
            k.eq(p + "page", page + 1, t.page);
            k.eq(p + "source", r.u("source") & 0x0F, t.source);
            k.eq(p + "source_flags", r.u("source") & 0xF0, t.source_flags);
            k.eq(p + "flags", r.u("flags"), t.flags);
            k.eq(p + "object", r.u("object"), t.first);
            const std::uint64_t kind = r.u("flags") & 3;
            k.eq(p + "kind", kind, static_cast<std::uint64_t>(t.kind));
            if (kind == 0 && r.find("target")) k.eq(p + "target", r.u("target"), t.target.value_or(~std::uint32_t{0}));
            if (kind == 1) k.eq(p + "ordinal", r.u("ordinal"), t.ordinal);
            if (kind == 2) k.eq(p + "name", r.u("name_offset"), t.name);
            k.eq(p + "has addend", r.find("addend") != nullptr, t.addend.has_value());
            if (r.find("addend") && t.addend) k.eq(p + "addend", r.u("addend"), *t.addend);
            std::vector<std::int16_t> sources;
            if (r.find("first_source")) sources.push_back(static_cast<std::int16_t>(r.i("first_source")));
            if (const Node* list = r.child("sources"))
                for (const Node& s : list->children) sources.push_back(static_cast<std::int16_t>(s.as_signed()));
            k.eq(p + "sources", sources.size(), t.sources.size());
            for (std::size_t j = 0; j < std::min(sources.size(), t.sources.size()); ++j) k.eq(p + "source offset", static_cast<std::uint16_t>(sources[j]), static_cast<std::uint16_t>(t.sources[j]));
            k.eq_bytes(p + "bytes", data.subspan(r.offset, r.size), t.raw);
        }
    } catch (const Error&) {
    }
}

}  // namespace

void compare_pe_tables(Checker& k, const Tree& t, ByteView data) {
    const auto image = pe::Image::try_parse(data);
    if (!image) return;
    tbl_pe_exports(k, t.root, *image);
    tbl_pe_loader_exports(k, t.root, *image);
    tbl_pe_imports(k, t.root, *image);
    tbl_pe_resources(k, t.root, *image);
    tbl_pe_relocs(k, t.root, *image);
}

void compare_ne_tables(Checker& k, const Tree& t, ByteView data) {
    const auto module = ne::Module::try_open(data);
    if (module) tbl_ne_tables(k, t.root, *module);
}

void compare_le_tables(Checker& k, const Tree& t, ByteView data) {
    try {
        const le::Module module = le::Module::from_buffer(Bytes(data.begin(), data.end()));
        tbl_le_tables(k, t.root, data, module);
    } catch (const Error&) {
    }
}

// ---- a digest of what the hand-written readers answer (format_check --readers: the same text before and after the
// readers moved onto the engine proves they answer the same) ----

namespace {

struct Digest {
    std::string text;
    bool verbose = false;   // --readers-text: an error's words too (the hash leaves them out)
    template <class... A>
    void line(std::format_string<A...> f, A&&... a) {
        text += std::format(f, std::forward<A>(a)...);
        text += '\n';
    }
    template <class F>
    void guard(std::string_view what, F&& body) {   // an answer that throws is an answer too
        try {
            body();
        } catch (const TruncatedError& e) {
            line("{}: truncated{}", what, said(e));
        } catch (const FormatError& e) {
            line("{}: format error{}", what, said(e));
        } catch (const Error& e) {
            line("{}: error{}", what, said(e));
        }
    }
    std::string said(const Error& e) const { return verbose ? std::string(" (") + e.what() + ")" : std::string();
    }
};

std::uint64_t dig_hash(const Bytes& b) {
    std::uint64_t hash = 1469598103934665603ull;
    for (std::uint8_t c : b) hash = (hash ^ c) * 1099511628211ull;
    return hash;
}

long long dig_opt(const auto& v) { return v ? static_cast<long long>(*v) : -1; }

void digest_pe(Digest& g, ByteView data) {
    const auto image = pe::Image::try_parse(data);
    if (!image) return;
    const pe::Header& h = image->header();
    g.line("header {} {} {} {} {} {} {} {} {}", h.e_lfanew, h.machine, h.n_sections, h.opt_size, h.characteristics, h.magic, h.entry_rva, h.n_datadirs, h.image_base);
    const pe::OptionalHeader o = image->optional_header();
    g.line("optional {} {} {} {} {} {} {} {} {} {} {} {} {}", o.linker_major, o.linker_minor, o.code_size, o.initialized_data_size, o.uninitialized_data_size,
           o.code_base, o.section_alignment, o.file_alignment, o.image_size, o.headers_size, o.checksum, o.subsystem, o.dll_characteristics);
    g.line("versions {} {} {} {} {} {}", o.os_version.major, o.os_version.minor, o.image_version.major, o.image_version.minor, o.subsystem_version.major, o.subsystem_version.minor);
    for (const pe::DataDirectory& d : image->data_directories()) g.line("dir {} {}", d.rva, d.size);
    for (const pe::Section& s : image->sections()) g.line("section {} {} {} {} {} {}", s.name, s.virtual_size, s.virtual_address, s.raw_size, s.raw_ptr, s.characteristics);
    g.guard("module_name", [&] { g.line("module {}", image->module_name()); });
    g.guard("exports", [&] {
        for (const pe::Export& e : image->exports()) g.line("export {} {} {}", e.ordinal, e.rva, e.name ? *e.name : "-");
    });
    g.guard("imports", [&] {
        for (const pe::Import& i : image->imports()) g.line("import {} {} {}", i.slot_va, i.dll, i.name);
    });
    g.guard("base_relocs", [&] {
        for (std::uint64_t r : image->base_relocs()) g.line("reloc {}", r);
    });
    const pe::ExportTable et = image->export_table();
    g.line("export_table {} {} {}", et.module, et.ordinal_base, et.timestamp);
    for (const pe::ExportSymbol& s : et.symbols)
        g.line("sym {} {} {} {} {}", s.ordinal, s.rva, s.name ? *s.name : "-", dig_opt(s.hint), s.forward ? *s.forward : "-");
    for (int delay = 0; delay < 2; ++delay)
        for (const pe::ImportModule& m : image->import_table(delay != 0)) {
            g.line("module {} {}", delay, m.module);
            for (const pe::ImportSymbol& s : m.symbols) g.line("isym {} {} {} {}", s.slot_rva, s.name ? *s.name : "-", dig_opt(s.ordinal), dig_opt(s.hint));
        }
    for (const pe::ResourceType& t : image->resource_tree()) {
        g.line("rtype {} {}", t.name ? *t.name : "-", dig_opt(t.id));
        for (const pe::ResourceItem& i : t.items) g.line("ritem {} {} {} {} {} {}", i.name ? *i.name : "-", dig_opt(i.id), i.language, i.rva, dig_opt(i.file_offset), i.size);
    }
    for (const pe::RelocBlock& b : image->reloc_blocks()) g.line("block {} {}", b.page_rva, b.entries);
}

void digest_ne(Digest& g, ByteView data) {
    const auto module = ne::Module::try_open(data);
    if (!module) return;
    const ne::Header& h = module->header();
    g.line("header {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {}", h.linker_version, h.linker_revision, h.entry_table_offset, h.entry_table_length,
           h.crc, h.flags, h.autodata_segment, h.heap_size, h.stack_size, h.cs_ip, h.ss_sp, h.segment_count, h.module_ref_count, h.nonresident_size, h.segment_table,
           h.resource_table, h.resident_names_table, h.module_ref_table, h.imported_names_table, h.nonresident_offset, h.movable_entries, h.alignment_shift,
           h.resource_segments, h.target_os);
    g.line("expected {}", dig_opt(h.expected_windows_version));
    g.guard("resident", [&] {
        for (const ne::Name& n : module->resident_names()) g.line("rname {} {}", n.name, n.ordinal);
        g.line("module_name {}", module->module_name());
    });
    g.guard("nonresident", [&] {
        for (const ne::Name& n : module->nonresident_names()) g.line("nname {} {}", n.name, n.ordinal);
    });
    g.guard("refs", [&] {
        for (const std::string& r : module->module_refs()) g.line("ref {}", r);
    });
    g.guard("segments", [&] {
        for (const ne::Segment& s : module->segments()) {
            g.line("segment {} {} {} {} {}", s.index, s.file_offset, s.length, s.flags, s.min_alloc);
            for (const ne::Relocation& r : module->relocations(s)) {
                g.line("reloc {} {} {} {}", r.source, r.flags, r.offset, r.target.index());
                if (const auto* a = std::get_if<ne::InternalRef>(&r.target)) g.line("  internal {} {}", a->segment, a->entry);
                else if (const auto* b = std::get_if<ne::ImportOrdinalRef>(&r.target)) g.line("  ordinal {} {}", b->module, b->ordinal);
                else if (const auto* c = std::get_if<ne::ImportNameRef>(&r.target)) g.line("  name {} {} {}", c->module, c->name_offset, module->imported_name(c->name_offset));
                else g.line("  os {}", std::get<ne::OsFixupRef>(r.target).fixup_type);
            }
        }
    });
    g.guard("entries", [&] {
        for (const ne::Entry& e : module->entries()) g.line("entry {} {} {} {} {}", e.ordinal, static_cast<int>(e.kind), e.segment, e.flags, e.offset);
        for (std::uint32_t o : module->export_ordinals()) g.line("export {}", o);
    });
    g.guard("resources", [&] {
        for (const ne::Resource& r : module->resources())
            g.line("resource {} {} {} {} {} {} {}", r.type, r.id, dig_opt(r.type_number), dig_opt(r.id_number), r.file_offset, r.length, r.flags);
    });
    g.guard("tables_end", [&] { g.line("tables_end {}", module->tables_end()); });
    g.guard("standalone", [&] {
        const Bytes b = module->standalone();
        g.line("standalone {} {:x}", b.size(), dig_hash(b));
    });
}

void digest_le(Digest& g, ByteView data) {
    std::optional<le::Module> module;
    try {
        module = le::Module::from_buffer(Bytes(data.begin(), data.end()));
    } catch (const Error&) {
        return;
    }
    const le::Header& h = module->header();
    g.line("header {} {} {} {} {} {} {} {} {} {} {} {} {} {} {}", h.byte_order, h.word_order, h.level, h.cpu, h.os, h.version, h.module_flags, h.pages, h.eip_object, h.eip,
           h.esp_object, h.esp, h.page_size, h.last_page_bytes, h.fixup_size);
    g.line("header2 {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {}", h.loader_size, h.object_table, h.objects, h.page_map, h.resources,
           h.resident_names, h.entry_table, h.fixup_pages, h.fixup_records, h.import_modules, h.import_module_count, h.import_names, h.data_pages, h.preload_pages,
           h.nonresident_names, h.nonresident_size, h.debug_info, h.debug_size, h.resource_data, h.resource_size, h.device_id, h.kit_version,
           module->signature(), module->is_member());
    for (const le::Object& o : module->objects()) g.line("object {} {} {} {} {} {}", o.index, o.vsize, o.base, o.flags, o.first_page, o.page_count);
    g.guard("page_map", [&] {
        for (const le::PageEntry& p : module->page_map()) g.line("page {} {}", p.number, p.kind);
    });
    g.guard("names", [&] {
        const auto [res, non] = module->names();
        for (const le::NameOrdinal& n : res) g.line("rname {} {}", n.name, n.ordinal);
        for (const le::NameOrdinal& n : non) g.line("nname {} {}", n.name, n.ordinal);
        g.line("resident_name {} description {}", module->resident_name(), module->description());
    });
    g.guard("import_modules", [&] {
        for (const std::string& m : module->import_modules()) g.line("import {}", m);
    });
    g.guard("entries", [&] {
        for (const le::EntryPoint& e : module->entries()) g.line("entry {} {} {} {}", e.ordinal, e.object, e.offset, e.flags);
        g.line("entry_table_size {}", module->entry_table_size());
        for (const le::Export& e : module->exports()) g.line("export {} {} {} {}", e.ordinal, e.object, e.offset, e.name ? *e.name : "-");
    });
    g.guard("fixups", [&] {
        for (const auto& [place, fx] : module->fixups()) {
            g.line("fixup {} {}", place.object, place.offset);
            if (const auto* a = std::get_if<le::InternalFixup>(&fx)) g.line("  internal {} {} {}", a->object, dig_opt(a->target), a->source_type);
            else g.line("  import {} {}", std::get<le::ImportFixup>(fx).module, std::get<le::ImportFixup>(fx).ordinal);
        }
    });
    g.guard("fixup_records", [&] {
        for (const le::FixupRecord& r : module->fixup_records()) {
            g.line("record {} {} {} {} {} {} {} {} {}", r.page, r.source, r.source_flags, r.flags, static_cast<int>(r.kind), r.first, dig_opt(r.target), r.ordinal, r.name);
            g.line("  addend {} sources {} raw {}", dig_opt(r.addend), r.sources.size(), r.raw.size());
            for (std::int16_t s : r.sources) g.line("  source {}", s);
        }
        for (const le::Reference& r : module->references()) g.line("ref {} {} {} {} {}", r.at.object, r.at.offset, r.source, r.target_object, r.target_offset);
    });
    g.guard("parts", [&] {
        const le::Parts parts = module->parts();
        g.line("parts {} {} {} {} {} {} {}", parts.stub.size(), parts.header.size(), parts.objects.size(), parts.pages.size(), parts.entry_table.size(),
               parts.fixups.size(), parts.after.size());
        const Bytes rebuilt = le::write_parts(parts);
        g.line("write_parts {} {:x}", rebuilt.size(), dig_hash(rebuilt));
    });
}

void digest_mz_header(Digest& g, std::string_view what, const std::optional<dos::MzHeader>& h) {
    if (!h) return g.line("{} none", what);
    g.line("{} {} {} {} {} {} {} {} {} {} {}", what, h->relocation_count, h->header_size, h->min_alloc, h->max_alloc, h->ss, h->sp, h->ip, h->cs,
           h->relocation_offset, h->size);
}

void digest_dos(Digest& g, ByteView data) {
    digest_mz_header(g, "mz", dos::mz_header(data));
    digest_mz_header(g, "mz_stub", dos::mz_header(data, true));
    if (const auto d = detect::dos_header(data)) {
        g.line("dos_header {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {}", d->bytes_in_last_page, d->pages, d->relocations, d->header_paragraphs, d->header_size,
               d->min_alloc, d->max_alloc, d->ss, d->sp, d->checksum, d->ip, d->cs, d->relocation_table, d->overlay, d->new_header, d->load_size);
        for (const auto& r : d->relocation_entries) g.line("  r {} {}", r.segment, r.offset);
    }
    if (const auto k = dos::kind_of(data, "file.exe")) g.line("kind {}", dos::kind_name(*k));
    for (const auto& [at, size] : dos::chain(data)) g.line("chain {} {}", at, size);
    if (const auto dv = dos::driver_and_vxd(data)) g.line("driver_and_vxd {} {}", dv->first, dv->second);
    if (const auto dw = dos::dos_and_windows(data)) g.line("dos_and_windows {} {}", dw->first.size, dw->second);
    if (const auto h = dos::mz_header(data, true))
        if (const auto pk = dos::packer_of(data, *h)) g.line("packer {}", dos::packer_name(*pk));
    g.guard("program", [&] {
        const dos::Program p = dos::read_program(data, "file.exe", false);
        g.line("program {} {} {} {} {} {} {} {} {:x} {} {}", dos::kind_name(p.kind), p.packer ? dos::packer_name(*p.packer) : "-", p.base, p.cs, p.ip, p.ss, p.sp,
               dig_opt(p.ds), dig_hash(p.image), p.image.size(), p.overlay.size());
        for (std::uint32_t r : p.relocations) g.line("  reloc {}", r);
        for (const dos::Span& s : p.spans) g.line("  span {} {} {}", s.address, s.length, s.label);
        for (const dos::Entry& e : p.entries) g.line("  entry {} {}", e.address, e.name);
        for (const dos::Device& d : p.devices) g.line("  device {} {} {}", d.offset, d.attributes, d.name);
    });
}

template <class E>
void digest_elf_tables(Digest& g, const E& elf) {
    g.line("elf {} {} {} {} {} {} {} {} {} {} {} {} {} {:x}", elf.type, elf.machine, elf.version, elf.entry, elf.phoff, elf.shoff, elf.flags, elf.ehsize, elf.phentsize,
           elf.phnum, elf.shentsize, elf.shnum, elf.shstrndx, dig_hash(elf.data));
    for (const auto& p : elf.program_headers) g.line("  ph {} {} {} {} {} {} {} {}", p.type, p.offset, p.vaddr, p.paddr, p.filesz, p.memsz, p.flags, p.align);
    for (const auto& s : elf.sections)
        g.line("  sh {} {} {} {} {} {} {} {} {} {} {}", s.name_offset, s.type, s.flags, s.address, s.offset, s.size, s.link, s.info, s.align, s.entsize, s.name);
}

void digest_elf(Digest& g, ByteView data) {
    for (const std::uint16_t machine : {std::uint16_t{8}, std::uint16_t{40}})
        if (mips_elf::probe_header(data, machine)) g.guard("mips_elf", [&] {
            const mips_elf::Elf elf = mips_elf::parse(data, machine);
            digest_elf_tables(g, elf);
            for (const mips_elf::Symbol& s : elf.symbols()) g.line("  sym {} {} {} {} {} {}", s.name, s.value, s.size, s.type, s.bind, s.section);
        });
    if (ppc_elf::is_ppc_elf(data)) g.guard("ppc_elf", [&] {
        const ppc_elf::Elf elf = ppc_elf::parse(data);
        g.line("ppc {} {} {} {} {}", elf.wiiu, elf.os_abi, elf.abi_version, elf.inflated_sections, static_cast<int>(elf.console()));
        digest_elf_tables(g, elf);
    });
}

void digest_xbe(Digest& g, ByteView data) {
    namespace xbe = xbox::xbe;
    const xbe::Xbe x = xbe::Xbe::parse(data);
    g.line("xbe {} {} {} {} {} {} {} {} {}", x.base, x.size_of_image, x.size_of_headers, x.cert_addr, x.section_count, x.section_header_addr, x.init_flags, x.entry_raw,
           x.thunk_raw);
    g.line("  cert {} {} {} {} {} {} {} {}", x.cert_size, x.title_id, x.allowed_media, x.region, x.ratings, x.disk_number, x.version, x.title_name);
    g.line("  entry {} {} thunk {} {}", x.entry_point().address, xbe::key_set_name(x.entry_point().keys), x.kernel_thunk().address, xbe::key_set_name(x.kernel_thunk().keys));
    g.line("  text {} {} {}", x.title_id_text(), x.region_text(), x.media_text());
    g.guard("sections", [&] {
        for (const xbe::Section& s : x.sections()) g.line("  section {} {} {} {} {} {}", s.name, s.flags, s.vaddr, s.vsize, s.raw_addr, s.raw_size);
    });
    g.guard("pe_image", [&] {
        const Bytes pe = x.pe_image();
        g.line("  pe {} {:x}", pe.size(), dig_hash(pe));
    });
}

void digest_cab(Digest& g, ByteView data, const std::filesystem::path& file) {
    const cab::Cabinet c = cab::parse(data, file);
    g.line("cab {} {} {} {} {} {} {} {}", c.size, c.set_id, c.index, c.flags, c.version_major, c.version_minor, c.block_reserve, c.folders.size());
    if (c.prev) g.line("  prev {} {}", c.prev->cabinet, c.prev->disk);
    if (c.next) g.line("  next {} {}", c.next->cabinet, c.next->disk);
    for (const cab::FolderEntry& f : c.folders) g.line("  folder {} {} {}", f.data_offset, f.block_count, f.type);
    for (const cab::FileEntry& e : c.files) g.line("  file {} {} {} {} {} {} {}", e.name, e.size, e.offset, e.folder, e.date, e.time, e.attributes);
    if (file.empty()) return;
    for (std::size_t k = 0; k < c.folders.size(); ++k)
        g.guard("folder", [&] {
            const Bytes stream = cab::folder_stream(c, k);
            g.line("  stream {} {} {:x}", k, stream.size(), dig_hash(stream));
        });
}

void digest_xdbf(Digest& g, ByteView data) {
    namespace xdbf = xbox::xdbf;
    const xdbf::Database db = xdbf::Database::parse(data);
    g.line("xdbf {} {} {} {} {} {}", db.version, db.entry_table_len, db.free_table_len, db.free_count, db.entries.size(), db.data_start());
    for (const xdbf::Entry& e : db.entries) g.line("  entry {} {} {} {:x}", e.ns, e.id, e.payload.size(), dig_hash(e.payload));
    for (const xdbf::Achievement& a : db.achievements()) g.line("  ach {} {} {} {} {} {}", a.id, a.image, a.score, a.flags, a.name, a.locked_desc);
}

void digest_pck(Digest& g, ByteView data) {
    namespace pck = xbox::pck;
    const pck::Archive a = pck::parse(data);
    g.line("pck {} {} {} {}", a.version, a.properties.size(), a.entries.size(), a.table_end);
    for (const auto& [id, name] : a.properties) g.line("  prop {} {}", id, name);
    for (const pck::Entry& e : a.entries) g.line("  entry {} {} {}", e.size, e.type, e.name);
    const pck::Payloads p = pck::payloads(data);
    g.line("  payloads {} {}", p.end, p.file_size);
    for (const pck::Payload& f : p.found) g.line("  payload {} {} {}", f.name, f.offset, f.size);
}

void digest_nes(Digest& g, ByteView data) {
    const nes::Rom rom = nes::parse(data);
    g.line("nes {} {} {} {} {} {:x} {} {:x} {:x} {:x} {} {} {}", rom.nes2, rom.mapper, rom.submapper, static_cast<int>(rom.mirroring), rom.battery, dig_hash(rom.header),
           rom.trainer.size(), dig_hash(rom.prg), dig_hash(rom.chr), dig_hash(rom.extra), rom.vectors.nmi, rom.vectors.reset, rom.vectors.irq);
    g.line("  sizes {} {} {} code_at {}", rom.prg.size(), rom.chr.size(), rom.extra.size(), rom.code_at);
}

void digest_ps3(Digest& g, ByteView data) {
    const ps3::Program p = ps3::parse(data);
    g.line("ps3 {} {} {} {} {} {} {} {} {} {} {} {:x}", p.self, p.elf_type, p.os_abi, p.abi_version, p.entry, p.entry_code, p.toc, p.elf_flags, p.prx_param,
           p.prx_param_size, p.image.size(), dig_hash(p.image));
    for (const ps3::Segment& s : p.segments) g.line("  segment {} {} {} {} {}", s.image_offset, s.size, s.address, s.memory_size, s.flags);
    for (const ps3::Import& i : p.imports) g.line("  import {} {} {} {}", i.library, i.nid, i.name, i.descriptor);
    const Bytes elf = p.to_elf(p.image);
    g.line("  to_elf {} {:x}", elf.size(), dig_hash(elf));
}

void digest_vita(Digest& g, ByteView data) {
    const vita::Program p = vita::parse(data);
    g.line("vita {} {} {} {} {} {} {:x}", p.self, p.elf_type, p.elf_entry, p.elf_flags, p.module_name, p.image.size(), dig_hash(p.image));
    for (const vita::Segment& s : p.segments) g.line("  segment {} {} {} {} {}", s.image_offset, s.size, s.address, s.memory_size, s.flags);
    for (const vita::Import& i : p.imports) g.line("  import {} {} {} {} {}", i.library, i.library_nid, i.nid, i.name, i.stub);
    for (std::uint32_t e : p.entries) g.line("  entry {}", e);
    const Bytes elf = p.to_elf(p.image);
    g.line("  to_elf {} {:x}", elf.size(), dig_hash(elf));
}

void digest_orbis(Digest& g, ByteView data) {
    const orbis::Program p = orbis::parse(data);
    g.line("orbis {} {} {} {} {} {} {} {} {:x} {}", p.self, p.orbis, p.container, p.type, p.entry, p.program_type, p.paid, p.file.size(), dig_hash(p.file), p.image_end());
    for (const orbis::Segment& s : p.segments) g.line("  segment {} {} {} {} {} {} {}", s.type, s.flags, s.offset, s.address, s.file_size, s.memory_size, s.align);
    for (const std::string& n : p.needed) g.line("  needed {}", n);
    for (const orbis::Symbol& s : p.symbols) g.line("  sym {} {} {} {} {}", s.name, s.value, s.size, s.info, s.section);
    for (const orbis::Symbol& s : p.functions) g.line("  fn {} {} {}", s.name, s.value, s.size);
    for (const orbis::Relocation& r : p.relocations) g.line("  rel {} {} {} {}", r.offset, r.type, r.symbol, r.addend);
    for (const orbis::Import& i : p.imports) g.line("  import {} {} {} {}", i.name, i.nid, i.library, i.slot);
    for (std::uint64_t i : p.initializers) g.line("  init {}", i);
}

void digest_nro(Digest& g, ByteView data) {
    const nro::Program p = nro::parse(data);
    g.line("nro {} {} {:x} {} {} {} {} {} {} {}", static_cast<int>(p.kind), p.flat.size(), dig_hash(p.flat), p.flat_is_file, p.entry, p.image_size, p.mod_offset,
           p.dynamic, p.asset_offset, p.build_id);
    g.line("  assets {} {} {} {} {} {} note {}", p.icon.offset, p.icon.size, p.nacp.offset, p.nacp.size, p.romfs.offset, p.romfs.size, p.note);
    for (const nro::Segment& s : p.segments)
        g.line("  segment {} {} {} {} {} {} {}", s.address, s.size, s.memory_size, s.file_offset, s.code, s.writable, s.name);
    for (const nro::Relocation& r : p.relocations) g.line("  rel {} {} {} {}", r.offset, r.type, r.symbol, r.addend);
    for (const auto& [at, size] : p.relocation_tables) g.line("  table {} {}", at, size);
    for (const auto& [at, name] : p.names) g.line("  name {} {}", at, name);
    for (const nro::Symbol& s : p.imports) g.line("  import {} {} {} {} {}", s.name, s.value, s.size, s.info, s.section);
}

void digest_omf_object(Digest& g, ByteView data) {
    const omf::Object o = omf::read(data);
    for (const auto& [name, bytes] : o.segments) g.line("  seg {} {} {:x}", name, bytes.size(), dig_hash(bytes));
    for (const omf::SegDef& s : o.segdefs) g.line("  segdef {} {} {} {}", s.name, s.class_name, s.length, s.alignment);
    for (const auto& [name, members] : o.groups) {
        g.line("  group {}", name);
        for (const std::string& m : members) g.line("    {}", m);
    }
    for (const omf::Public& p : o.publics) g.line("  public {} {} {}", p.name, p.segment ? *p.segment : "-", p.offset);
    for (const std::string& e : o.externals) g.line("  extern {}", e);
    for (const auto& [name, c] : o.communals) g.line("  communal {} {} {}", name, c.size, c.far_pointer);
    for (const omf::Fixup& f : o.fixups)
        g.line("  fixup {} {} {} {} {} {} {} {}", f.segment, f.at, static_cast<int>(f.kind), static_cast<int>(f.target.kind), f.target.name ? *f.target.name : "-",
               dig_opt(f.target.number), f.displacement, f.location);
    for (const auto& [name, c] : o.combine) g.line("  combine {} {}", name, c);
    if (o.start) g.line("  start {} {}", o.start->segment, o.start->offset);
    for (const auto& [name, n] : o.filled) g.line("  filled {} {}", name, n);
}

void digest_omf(Digest& g, ByteView data) {
    g.line("omf aout {}", omf::is_aout(data));
    for (const omf::Record& r : omf::records(data)) g.line("  record {} {} {:x}", r.type, r.payload.size(), dig_hash(Bytes(r.payload.begin(), r.payload.end())));
    g.guard("omf_library", [&] {
        for (const auto& [name, module] : omf::library(data)) {
            g.line("module {} {}", name, module.size());
            g.guard("omf_read", [&] { digest_omf_object(g, module); });
        }
    });
    g.guard("omf_publics", [&] {
        for (const std::string& p : omf::publics(data)) g.line("  pub {}", p);
    });
    g.guard("omf_externs", [&] {
        for (const std::string& e : omf::externs(data)) g.line("  ext {}", e);
    });
    const omf::Walk w = omf::walk(data);
    for (const omf::RawRecord& r : w.records) g.line("  walk {} {} {}", r.offset, r.type, r.payload.size());
    if (w.error) g.line("  walk error");
}

void digest_iso_tree(Digest& g, const iso9660::Volume& v, iso9660::Names names, const iso9660::Entry& dir, int depth, std::size_t& seen, std::size_t& files) {
    if (depth > 12 || seen > 20000) return;
    const std::vector<iso9660::Entry> entries = v.list(dir, names);
    for (const iso9660::Entry& e : entries) {
        ++seen;
        g.line("  {}{} {} {} {} {:x}", std::string(static_cast<std::size_t>(depth), ' '), e.name, e.directory, e.size, e.extent,
               dig_hash(Bytes(e.recorded.begin(), e.recorded.end())));
        for (const iso9660::Extent& x : e.extents) g.line("    extent {} {}", x.sector, x.size);
        if (e.directory) {
            g.guard("dir", [&] { digest_iso_tree(g, v, names, e, depth + 1, seen, files); });
        } else if (files < 64 && e.size < (8u << 20)) {
            ++files;
            g.guard("file", [&] {
                const Bytes b = v.read(e);
                g.line("    data {} {:x}", b.size(), dig_hash(b));
            });
        }
    }
}

void digest_iso(Digest& g, ByteView data) {
    const iso9660::Volume v(disc::Sectors::of(data));
    g.line("iso {} {}", v.has_joliet(), v.sector_count());
    for (const iso9660::Names names : {iso9660::Names::Primary, iso9660::Names::Joliet}) {
        if (names == iso9660::Names::Joliet && !v.has_joliet()) break;
        const iso9660::Info i = v.info(names);
        g.line(" info {} {} {} {} {} {} {} {} {} {}", i.system, i.label, i.created, i.tree_label, i.blocks, i.block_size, i.has_joliet, i.root_sector, i.root_size,
               dig_opt(i.boot_catalog));
        const iso9660::Entry root = v.root(names);
        g.line(" root {} {}", root.size, root.extent);
        std::size_t seen = 0, files = 0;
        g.guard("tree", [&] { digest_iso_tree(g, v, names, root, 0, seen, files); });
        g.guard("lookup", [&] {
            for (const iso9660::Entry& e : v.list("/", names))
                if (!e.directory) {
                    const Bytes b = v.read("/" + e.name, names);
                    g.line(" read /{} {} {:x}", e.name, b.size(), dig_hash(b));
                    break;
                }
        });
    }
    g.guard("sectors", [&] {
        const Bytes b = v.read_sectors(16, 2);
        g.line(" sectors {} {:x}", b.size(), dig_hash(b));
    });
}

void digest_xzp(Digest& g, ByteView data) {
    namespace xzp = xbox::xzp;
    const xzp::Pack p = xzp::parse(data);
    g.line("xzp {} {} {} {} {}", p.version, p.file_size, p.base, p.table_end, p.entries.size());
    for (const xzp::Entry& e : p.entries) g.line("  entry {} {} {}", e.size, e.offset, e.name);
    for (const xzp::Payload& f : xzp::payloads(data)) g.line("  payload {} {} {}", f.path, f.offset, f.size);
}

void digest_xpr(Digest& g, ByteView data) {
    namespace xpr = xbox::xpr;
    const xpr::Bundle b = xpr::parse(data);
    g.line("xpr {} {} {} {} {} {} {} {} {} {} {} {} {} {:x}", static_cast<int>(b.container), b.xfmt, b.format_name, b.dds_format ? static_cast<int>(*b.dds_format) : -1,
           b.width, b.height, b.mipmaps, b.cubemap, b.total, b.header_size, b.data_size, b.data_offset, b.pixels.size(), dig_hash(b.pixels));
}

void digest_xur(Digest& g, ByteView data) {
    namespace xur = xbox::xur;
    const xur::Scene s = xur::Scene::parse(data);
    g.line("xur {} {} {} {} {}", s.version, s.flags, s.tool_version, s.file_size, s.table_end());
    for (const xur::Section& sec : s.sections) g.line("  section {} {} {}", sec.tag, sec.offset, sec.length);
    g.guard("strings", [&] {
        for (const std::string& t : s.strings()) g.line("  string {}", t);
    });
}

}  // namespace

std::string reader_digest(ByteView data, const std::filesystem::path& file, bool text) {
    Digest g;
    g.verbose = text;
    g.guard("pe", [&] { digest_pe(g, data); });
    g.guard("ne", [&] { digest_ne(g, data); });
    g.guard("le", [&] { digest_le(g, data); });
    g.guard("dos", [&] { digest_dos(g, data); });
    if (data.size() >= 4 && data[0] == 0x7F && data[1] == 0x45) g.guard("elf", [&] { digest_elf(g, data); });
    if (file.extension() == ".pck" || file.extension() == ".PCK") g.guard("pck", [&] { digest_pck(g, data); });
    if (data.size() >= 4 && data[0] == 0x58 && data[1] == 0x44 && data[2] == 0x42 && data[3] == 0x46) g.guard("xdbf", [&] { digest_xdbf(g, data); });
    if (data.size() >= 4 && data[0] == 0x4D && data[1] == 0x53 && data[2] == 0x43 && data[3] == 0x46) g.guard("cab", [&] { digest_cab(g, data, file); });
    if (data.size() >= 4 && data[0] == 0x58 && data[1] == 0x42 && data[2] == 0x45 && data[3] == 0x48) g.guard("xbe", [&] { digest_xbe(g, data); });
    if (data.size() >= 4 && data[0] == 0x4E && data[1] == 0x45 && data[2] == 0x53) g.guard("nes", [&] { digest_nes(g, data); });
    const bool elf_or_sce = data.size() >= 4 && ((data[0] == 0x7F && data[1] == 0x45) || (data[0] == 0x53 && data[1] == 0x43 && data[2] == 0x45 && data[3] == 0));
    if (elf_or_sce) g.guard("ps3", [&] { digest_ps3(g, data); });
    if (elf_or_sce) g.guard("vita", [&] { digest_vita(g, data); });
    if (orbis::kind_of(data.first(std::min<std::size_t>(data.size(), 64))) != orbis::Kind::None) g.guard("orbis", [&] { digest_orbis(g, data); });
    const bool switch_file = (data.size() >= 0x14 && data[0x10] == 0x4E && data[0x11] == 0x52 && data[0x12] == 0x4F && data[0x13] == 0x30) ||
                             (data.size() >= 4 && ((data[0] == 0x4E && data[1] == 0x53 && data[2] == 0x4F && data[3] == 0x30) || (data[0] == 0x7F && data[1] == 0x45)));
    if (switch_file) g.guard("nro", [&] { digest_nro(g, data); });
    if (data.size() >= 1 && (data[0] == 0x80 || data[0] == 0xF0)) g.guard("omf", [&] { digest_omf(g, data); });
    const bool cd001 = (data.size() >= 0x8006 && data[0x8001] == 0x43 && data[0x8002] == 0x44 && data[0x8003] == 0x30) ||
                       (data.size() >= 16 * 2352 + 32 && disc::raw_sync(data.subspan(16 * 2352)));
    if (cd001) g.guard("iso", [&] { digest_iso(g, data); });
    const auto magic = [&](std::string_view m) { return data.size() >= m.size() && as_text(data.first(m.size())) == m; };
    if (magic("XUIZ")) g.guard("xzp", [&] { digest_xzp(g, data); });
    if (magic("XPR0") || magic("XPR2")) g.guard("xpr", [&] { digest_xpr(g, data); });
    if (magic("XUIB")) g.guard("xur", [&] { digest_xur(g, data); });
    if (text) return g.text;
    std::uint64_t hash = 1469598103934665603ull;
    for (char c : g.text) hash = (hash ^ static_cast<std::uint8_t>(c)) * 1099511628211ull;
    return std::format("{:016x} {} bytes", hash, g.text.size());
}

}  // namespace cb::formats::engine
