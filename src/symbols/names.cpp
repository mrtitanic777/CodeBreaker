// cb-unity: alone (file-local helpers named for this module)
#include "symbols/names.hpp"

#include <algorithm>
#include <format>
#include <memory>
#include <set>

#include "core/cli.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/text.hpp"
#include "decompile/describe.hpp"
#include "formats/pe.hpp"
#include "symbols/demangle.hpp"

namespace cb::symbols::names {

namespace describe = decompile::describe;
namespace ne = formats::ne;
namespace pef = formats::pe_functions;

namespace {

constexpr std::uint16_t kMachineI386 = 0x014C, kMachinePowerPc = 0x01F2;

// The segments a .SYM file or CodeView record names a linker puts no objects' names on: a function in one of them is not "from" a source file.
bool is_plain_segment(std::string_view name) {
    static const std::set<std::string, std::less<>> plain = {"_TEXT", "TEXT",  "DGROUP", "BSS",   "_BSS",   "RDATA", "_DATA", "DATA",
                                                             "CONST", "IGROUP", "LGROUP", "_LGROUP", "_IGROUP", "EDATA", "IDATA", "RELOC",
                                                             "RSRC",  "CODE",  "_CODE",  "AUTO",  "STACK",  "HEAP",  "_INIT", "INIT"};
    return plain.contains(text::upper(name));
}

std::string plain_upper(std::string_view name) { return text::upper(demangle::plain(name).name); }

// A name as `\W+` -> "_" makes it: each run of characters that are not letters, digits or underscore is one underscore.
std::string runs_to_underscore(std::string_view name) {
    std::string out;
    bool in_run = false;
    for (const char c : name) {
        const bool word = (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
        if (word) {
            out += c;
            in_run = false;
        } else if (!in_run) {
            out += '_';
            in_run = true;
        }
    }
    return out;
}

std::string_view first_part(std::string_view text, std::string_view separator) { return text.substr(0, text.find(separator)); }

bool is_digits(std::string_view s) { return !s.empty() && std::all_of(s.begin(), s.end(), [](char c) { return c >= '0' && c <= '9'; }); }

}  // namespace

std::string function_key(base::Lane lane, Address address) {
    if (lane == base::Lane::Ne) {
        const core16::SegOff place = describe::where(static_cast<describe::Va>(address));
        return std::format("{}:{:04x}", place.seg, place.off);
    }
    return std::format("{:x}", address);
}

struct Namer::Impl {
    Services s;
    platforms::DecoderSet decoders;
    pef::CarveCache carves;
    std::optional<std::map<std::string, std::vector<std::filesystem::path>>> sym_files;   // by stem in capitals; lookup only
    struct Debugged {
        Named names;
        std::map<Address, std::string> files;
    };
    std::map<std::string, Debugged> debugged;   // by absolute path; lookup only

    explicit Impl(Services services) : s(std::move(services)) {}

    std::optional<base::Lane> lane(const std::filesystem::path& module) const { return s.corpus.lane(module); }
    std::string lane_text(const std::filesystem::path& module) const {
        const auto l = lane(module);
        return l ? std::string(base::lane_name(*l)) : std::string();
    }
    std::string sha(const std::filesystem::path& module) const { return s.corpus.sha(module).value_or(""); }

    // The machine a PE file is for (the header only: the carve of a PowerPC image would read its code as x86).
    std::uint16_t machine(const std::filesystem::path& module) const { return formats::pe::Image::open(module).header().machine; }
    bool is_powerpc(const std::filesystem::path& module) const { return machine(module) == kMachinePowerPc; }

    // ---- bodies ----
    std::map<Address, std::string> bodies(const std::filesystem::path& module) {
        const auto l = lane(module);
        if (l == base::Lane::Pe) {
            return machine(module) == kMachineI386 ? bodies32(*carves.get(module)) : std::map<Address, std::string>{};
        }
        if (l == base::Lane::Ne) {
            const ne::Module m = ne::Module::open_file(module);
            std::vector<core16::SegOff> starts;
            for (const auto& [place, distance] : describe::find_functions(m)) starts.push_back(place);
            return bodies16(m, starts, decoders);
        }
        return {};
    }

    // ---- debug symbols ----
    // The .SYM files of the reference folder by the module they are of (settings "reference", else the folder of that name beside the corpus);
    // the folders of other processors are not looked in. In the order of their paths.
    const std::vector<std::filesystem::path>& sym_files_of(const std::string& stem) {
        if (!sym_files) {
            std::map<std::string, std::vector<std::filesystem::path>> found;
            std::error_code ec;
            const std::filesystem::path reference = s.corpus.reference();
            std::filesystem::recursive_directory_iterator it(reference, std::filesystem::directory_options::skip_permission_denied, ec), end;
            for (; !ec && it != end; it.increment(ec)) {
                const std::string name = fs::to_text(it->path().filename());
                if (it->is_directory(ec)) {
                    const std::string upper = text::upper(name);
                    if (upper == "ALPHA" || upper == "MIPS" || upper == "PPC") it.disable_recursion_pending();
                } else if (text::lower(name).ends_with(".sym")) {
                    found[text::upper(name.substr(0, name.size() - 4))].push_back(it->path());
                }
            }
            for (auto& [key, paths] : found) std::sort(paths.begin(), paths.end());
            sym_files = std::move(found);
        }
        static const std::vector<std::filesystem::path> none;
        const auto at = sym_files->find(stem);
        return at == sym_files->end() ? none : at->second;
    }

    Debugged compute_debug(const std::filesystem::path& module) {
        Debugged out;
        const auto l = lane(module);
        const Bytes data = fs::read_file(module);
        std::map<std::string, std::pair<std::uint32_t, std::uint32_t>> ne_exports;   // (NE) NAME -> (segment, offset)
        std::map<std::string, std::uint32_t> pe_exports;                              // (PE) NAME -> rva
        std::string stem;
        std::vector<std::uint32_t> sections;
        std::uint64_t image_base = 0;
        const bool is_ne = l == base::Lane::Ne;
        if (is_ne) {
            const ne::Module m = ne::Module::open_file(module);
            std::map<std::uint32_t, std::pair<std::uint32_t, std::uint32_t>> entries;
            for (const ne::Entry& e : m.entries())
                if (e.kind != ne::Entry::Kind::Unused) entries[e.ordinal] = {e.segment, e.offset};
            const std::vector<ne::Name> resident = m.resident_names(), nonresident = m.nonresident_names();
            for (const auto* names : {&resident, &nonresident})   // (the module's own name, the first of each, is not an export)
                for (std::size_t i = 1; i < names->size(); ++i)
                    if (const auto at = entries.find((*names)[i].ordinal); at != entries.end()) ne_exports[text::upper((*names)[i].name)] = at->second;
            stem = text::upper(m.module_name());
        } else if (l == base::Lane::Pe) {
            const formats::pe::Image image = formats::pe::Image::open(module);
            image_base = image.header().image_base;
            for (const formats::pe::Export& e : image.exports())
                if (e.name && !e.name->empty()) pe_exports[plain_upper(*e.name)] = e.rva;
            stem = text::upper(fs::to_text(module.stem()));
            for (const formats::pe::Section& sec : image.sections()) sections.push_back(sec.virtual_address);
        } else {
            return out;
        }
        const auto place = [&](std::uint32_t segment, std::uint32_t offset) -> std::optional<Address> {
            if (is_ne) return static_cast<Address>(describe::address(segment, offset));
            if (0 < segment && segment <= sections.size()) return image_base + sections[segment - 1] + offset;
            return std::nullopt;
        };

        struct Table {
            std::string where;
            std::vector<SymSegment> segments;
            std::map<std::uint32_t, std::string> objects;
        };
        std::vector<Table> tables;
        {
            Table codeview_table{"CodeView symbols in the file", {}, {}};
            for (const auto& [at, name] : codeview(data)) {   // by segment, ascending
                if (codeview_table.segments.empty() || codeview_table.segments.back().number != at.seg) codeview_table.segments.push_back({at.seg, {}});
                codeview_table.segments.back().symbols[at.off] = name;
            }
            tables.push_back(std::move(codeview_table));
        }
        for (const std::filesystem::path& p : sym_files_of(stem)) {
            std::error_code ec;
            if (!std::filesystem::is_regular_file(p, ec)) continue;
            const Bytes file = fs::read_file(p);
            tables.push_back({fs::to_text(p.filename()), read_sym(file).value_or(std::vector<SymSegment>{}), sym_objects(file)});
        }
        for (const Table& table : tables) {
            for (const SymSegment& segment : table.segments) {
                // the symbols are for this build only where its own exports sit where the symbols put them
                std::size_t agree = 0, named = 0;
                for (const auto& [offset, name] : segment.symbols) {
                    if (is_ne) {
                        const auto at = ne_exports.find(plain_upper(name));
                        if (at == ne_exports.end()) continue;
                        ++named;
                        agree += at->second == std::make_pair(segment.number, offset & 0xFFFFu);
                    } else {
                        const auto at = pe_exports.find(plain_upper(name));
                        if (at == pe_exports.end()) continue;
                        ++named;
                        const auto p = place(segment.number, offset);
                        agree += p && *p == image_base + at->second;
                    }
                }
                if (!(agree >= 2 && agree * 3 >= named * 2)) continue;
                for (const auto& [offset, name] : segment.symbols) {
                    const auto address = place(segment.number, is_ne ? (offset & 0xFFFFu) : offset);
                    if (!address) continue;
                    out.names.try_emplace(*address, NameSource{demangle::plain(name).name, table.where});
                    const auto object = table.objects.find(segment.number);
                    if (object != table.objects.end() && !object->second.empty() && !is_plain_segment(object->second)) {
                        const std::string_view trimmed = std::string_view(object->second).substr(std::min(object->second.find_first_not_of('_'), object->second.size()));
                        out.files.try_emplace(*address, text::lower(trimmed));
                    }
                }
            }
            if (!out.names.empty()) break;   // the first table that names anything is the one
        }
        return out;
    }

    const Debugged& debug(const std::filesystem::path& module) {
        const std::string key = fs::to_text(std::filesystem::absolute(module).lexically_normal());
        auto it = debugged.find(key);
        if (it == debugged.end()) it = debugged.emplace(key, compute_debug(module)).first;
        return it->second;
    }

    // ---- the names of a module ----
    std::vector<Address> starts(const std::filesystem::path& module) {
        const auto l = lane(module);
        if (l == base::Lane::Ne) {
            std::vector<Address> out;
            for (const auto& [place, distance] : describe::find_functions(ne::Module::open_file(module)))
                out.push_back(static_cast<Address>(describe::address(place.seg, place.off)));
            std::sort(out.begin(), out.end());
            return out;
        }
        if (l == base::Lane::Pe) {
            if (is_powerpc(module)) {
                const auto facts = s.powerpc ? s.powerpc(module) : std::nullopt;
                return facts ? facts->functions : std::vector<Address>{};
            }
            std::vector<Address> out;   // the functions the carve finds, in address order
            for (const auto& [start, function] : carves.get(module)->functions()) out.push_back(start);
            return out;
        }
        return {};
    }

    Named own(const std::filesystem::path& module) {
        const auto l = lane(module);
        Named out;
        std::string stem;
        std::map<std::uint32_t, Address> numbered;   // ordinal -> address: what the ordinal table can name
        if (l == base::Lane::Ne) {
            const ne::Module m = ne::Module::open_file(module);
            std::map<std::uint32_t, Address> entries;
            for (const ne::Entry& e : m.entries())
                if (e.kind != ne::Entry::Kind::Unused) entries[e.ordinal] = static_cast<Address>(describe::address(e.segment, e.offset));
            const std::vector<ne::Name> nonresident = m.nonresident_names(), resident = m.resident_names();
            for (const auto* names : {&nonresident, &resident})   // (the module's own name, the first of each, is not an export)
                for (std::size_t i = 1; i < names->size(); ++i)
                    if (const auto at = entries.find((*names)[i].ordinal); at != entries.end()) out.try_emplace(at->second, NameSource{(*names)[i].name, "export"});
            stem = text::upper(m.module_name());
            numbered = std::move(entries);
        } else if (l == base::Lane::Pe && is_powerpc(module)) {
            return powerpc(module);
        } else if (l == base::Lane::Pe) {
            const formats::pe::Image image = formats::pe::Image::open(module);
            for (const formats::pe::Export& e : image.exports()) {
                const Address va = image.header().image_base + e.rva;
                numbered[static_cast<std::uint32_t>(e.ordinal)] = va;
                if (e.name && !e.name->empty()) out.try_emplace(va, NameSource{*e.name, "export"});
            }
            stem = text::upper(fs::to_text(module.stem()));
        } else {
            return out;
        }
        for (const auto& [address, found] : debug(module).names) out.try_emplace(address, NameSource{found.name, "debug symbols (" + found.source + ")"});
        for (const auto& [ordinal, address] : numbered) {
            if (out.contains(address)) continue;
            const auto linker = s.ordinals.linker_name(stem, ordinal);
            if (linker && !linker->empty()) out[address] = {*linker, "ordinal table (" + s.ordinals.source(stem, ordinal) + ")"};
        }
        return out;
    }

    Named powerpc(const std::filesystem::path& module) {
        Named out;
        const auto facts = s.powerpc ? s.powerpc(module) : std::nullopt;
        if (!facts) return out;
        if (facts->entry && *facts->entry) out[*facts->entry] = {"entry", "entry point"};
        std::set<std::string> taken = {"entry"};
        std::vector<PowerPcNames::VirtualMethod> methods = facts->methods;
        std::stable_sort(methods.begin(), methods.end(), [](const auto& a, const auto& b) { return a.address < b.address; });
        for (const auto& m : methods) {
            const std::string name = std::format("{}_vf{}", runs_to_underscore(m.class_name), m.slot);
            if (out.contains(m.address) || taken.contains(name)) continue;
            taken.insert(name);
            out[m.address] = {name, std::format("run-time type information ({}, virtual function {})", m.class_name, m.slot)};
        }
        return out;
    }

    std::size_t index(const std::filesystem::path& module, const std::optional<Named>& own_names_given) {
        if (!s.bodies) throw UsageError("there is no body store to record bodies in");
        const auto l = lane(module);
        const base::Lane lane_of = l.value_or(base::Lane::Dos);
        const Named names = own_names_given ? *own_names_given : own(module);
        std::map<std::string, store::Body> rows;
        for (const auto& [address, digest] : bodies(module)) {
            const auto found = names.find(address);
            rows[function_key(lane_of, address)] = {digest, found == names.end() ? std::nullopt : std::optional<std::string>(found->second.name),
                                                     found == names.end() ? std::nullopt : std::optional<std::string>(found->second.source)};
        }
        s.bodies->set_bodies({sha(module), fs::to_text(module.filename()), fs::to_text(module), lane_text(module)}, rows);
        return rows.size();
    }

    base::Lane lane_or_dos(const std::filesystem::path& module) const { return lane(module).value_or(base::Lane::Dos); }

    ModuleNames of_module(const std::filesystem::path& module) {
        const auto l = lane(module);
        const std::string sha_text = sha(module);
        const Named own_found = own(module);
        if (s.bodies && s.bodies->bodies(sha_text).empty()) index(module, own_found);
        const std::map<Address, runtime_library::Identified> library = s.runtime_library ? s.runtime_library(module) : std::map<Address, runtime_library::Identified>{};
        const std::map<std::string, store::SharedName> from_bodies = s.bodies ? s.bodies->names_from_bodies(sha_text) : std::map<std::string, store::SharedName>{};
        std::map<std::string, store::Proposal> accepted_here, proposed_here;   // by function key: the proposals of this module not carried by a body
        if (s.proposals) {
            for (store::Proposal& p : s.proposals->of_module(sha_text)) {
                if (p.state == store::ProposalState::Accepted && !p.digest) accepted_here[p.key] = std::move(p);
                else if (p.state == store::ProposalState::Proposed) proposed_here[p.key] = std::move(p);
            }
        }

        ModuleNames result;
        const base::Lane lane_of = lane_or_dos(module);
        for (const Address address : starts(module)) {
            const std::string key = function_key(lane_of, address);
            const auto shared = from_bodies.find(key);
            const auto mine = own_found.find(address);
            FunctionName f{address, key, {}, {}};
            if (mine != own_found.end() && mine->second.source == "export") {
                f.name = mine->second.name;
                f.source = mine->second.source;
            } else if (shared != from_bodies.end() && !shared->second.source.starts_with("the same body")) {
                f.name = shared->second.name;
                f.source = "named body (" + shared->second.source + ")";
            } else if (const auto local = accepted_here.find(key); local != accepted_here.end()) {
                f.name = local->second.name;
                f.source = local->second.source;
            } else if (mine != own_found.end()) {
                f.name = mine->second.name;
                f.source = mine->second.source;
            } else if (const auto lib = library.find(address); lib != library.end()) {
                f.name = lib->second.name;
                f.source = "runtime library: " + lib->second.library;
            } else if (shared != from_bodies.end()) {
                f.name = shared->second.name;
                f.source = shared->second.source;
            } else {
                f.name = "sub_" + text::replace_all(key, ":", "_");
                f.source = "(none)";
                if (const auto proposal = proposed_here.find(key); proposal != proposed_here.end())
                    f.proposed = ProposedName{proposal->second.name, proposal->second.description, proposal->second.rule, proposal->second.confidence};
            }
            result.functions.push_back(std::move(f));
        }

        if (l == base::Lane::Ne) {
            const ne::Module m = ne::Module::open_file(module);
            const std::vector<std::string> refs = m.module_refs();
            std::set<std::pair<std::string, std::string>> seen;   // (module, name) already listed
            std::set<std::pair<std::string, std::uint32_t>> by_number;   // the functions imported by number, sorted
            for (const ne::Segment& segment : m.segments()) {
                for (const ne::Relocation& r : m.relocations(segment)) {
                    if (const auto* by_name = std::get_if<ne::ImportNameRef>(&r.target); by_name && 0 < by_name->module && by_name->module <= refs.size()) {
                        std::string imported = text::upper(refs[by_name->module - 1]);
                        std::string name = m.imported_name(by_name->name_offset);
                        if (seen.emplace(imported, name).second) result.imports.push_back({std::move(imported), std::nullopt, std::move(name), "imported by name"});
                    } else if (const auto* ordinal = std::get_if<ne::ImportOrdinalRef>(&r.target); ordinal && 0 < ordinal->module && ordinal->module <= refs.size()) {
                        by_number.emplace(text::upper(refs[ordinal->module - 1]), ordinal->ordinal);
                    }
                }
            }
            for (const auto& [imported, ordinal] : by_number) result.imports.push_back(numbered_import(imported, ordinal));
        } else if (l == base::Lane::Pe && is_powerpc(module)) {
            if (s.console_import_names) {
                for (const auto& [symbol, name] : s.console_import_names(module)) {
                    // `import<library>_ord_<number>`: the library and the ordinal the console's system libraries number it by
                    ImportName item{"?", std::nullopt, name, "the console's system libraries (symbols/console_ordinals.json)"};
                    if (symbol.starts_with("import")) {
                        const std::size_t middle = symbol.find("_ord_");
                        const std::string_view number_of_library = middle == std::string::npos ? std::string_view() : std::string_view(symbol).substr(6, middle - 6);
                        const std::string_view number = middle == std::string::npos ? std::string_view() : std::string_view(symbol).substr(middle + 5);
                        if (is_digits(number_of_library) && is_digits(number)) {
                            item.module = "library " + std::string(number_of_library);
                            item.ordinal = text::parse_int<std::uint32_t>(number, 10);
                        }
                    }
                    result.imports.push_back(std::move(item));
                }
            }
        } else if (l == base::Lane::Pe) {
            std::set<std::pair<std::string, std::string>> uniq;   // (dll, name), sorted
            for (const formats::pe::Import& i : formats::pe::Image::open(module).imports()) uniq.emplace(i.dll, i.name);
            for (const auto& [dll, name] : uniq) {
                const std::string imported = text::upper(fs::to_text(std::filesystem::path(dll).stem()));
                if (name.starts_with('#') && is_digits(std::string_view(name).substr(1))) {
                    result.imports.push_back(numbered_import(imported, *text::parse_int<std::uint32_t>(std::string_view(name).substr(1), 10)));
                } else {
                    result.imports.push_back({imported, std::nullopt, name, "imported by name"});
                }
            }
        }
        return result;
    }

    // A function imported by number: the name the tables give it and where they have it from, else "MODULE.number" and "(none)".
    ImportName numbered_import(const std::string& imported, std::uint32_t ordinal) {
        const auto real = s.ordinals.name(imported, ordinal);
        if (real && !real->empty()) return {imported, ordinal, *real, std::format("{}'s exports ({})", imported, s.ordinals.source(imported, ordinal))};
        return {imported, ordinal, std::format("{}.{}", imported, ordinal), "(none)"};
    }
};

Namer::Namer(Services services) : impl_(std::make_unique<Impl>(std::move(services))) {}
Namer::~Namer() = default;

std::map<Address, std::string> Namer::bodies(const std::filesystem::path& module) { return impl_->bodies(module); }
const Named& Namer::debug_names(const std::filesystem::path& module) { return impl_->debug(module).names; }
const std::map<Address, std::string>& Namer::source_files(const std::filesystem::path& module) { return impl_->debug(module).files; }
Named Namer::own_names(const std::filesystem::path& module) { return impl_->own(module); }
Named Namer::powerpc_names(const std::filesystem::path& module) { return impl_->powerpc(module); }
std::size_t Namer::index(const std::filesystem::path& module, const std::optional<Named>& own) { return impl_->index(module, own); }
ModuleNames Namer::of_module(const std::filesystem::path& module) { return impl_->of_module(module); }

std::string Namer::module_sha(const std::filesystem::path& module) const { return impl_->sha(module); }
std::string Namer::key_of(const std::filesystem::path& module, Address address) const { return function_key(impl_->lane_or_dos(module), address); }

std::optional<std::string> Namer::set_name(const std::filesystem::path& module, Address address, std::string_view name, std::string_view source) {
    if (!impl_->s.bodies) throw UsageError("there is no body store to name a body in");
    const std::string sha = impl_->sha(module);
    if (impl_->s.bodies->bodies(sha).empty()) impl_->index(module, std::nullopt);
    const auto rows = impl_->s.bodies->bodies(sha);
    const auto row = rows.find(function_key(impl_->lane_or_dos(module), address));
    if (row == rows.end()) return std::nullopt;
    impl_->s.bodies->name_body(row->second.digest, name, source);
    return row->second.digest;
}

// ---- the command -----------------------------------------------------------------------------------------------------------------

// The address a person writes: "segment:offset" (decimal segment, hex offset) of a 16-bit module, or the address in hex.
std::optional<Address> parse_address(std::string_view text_in) {
    const std::size_t colon = text_in.find(':');
    if (colon == std::string_view::npos) return text::parse_int<Address>(text_in, 16);
    const auto segment = text::parse_int<std::uint32_t>(text_in.substr(0, colon), 10);
    const auto offset = text::parse_int<std::uint32_t>(text_in.substr(colon + 1), 16);
    if (!segment || !offset) return std::nullopt;
    return static_cast<Address>(describe::address(*segment, *offset));
}

namespace {

// "name count, name count" with the counts as they were met, the largest first (ties keep the order they were met in).
using Counts = std::vector<std::pair<std::string, std::size_t>>;
void bump(Counts& counts, const std::string& key) {
    for (auto& [name, n] : counts)
        if (name == key) {
            ++n;
            return;
        }
    counts.emplace_back(key, 1);
}
std::string joined(const Counts& counts) {
    std::vector<std::string> parts;
    for (const auto& [name, n] : counts) parts.push_back(std::format("{} {}", name, n));
    return text::join(parts, ", ");
}

}  // namespace

cli::Command declare_command(std::function<int(const cli::Args&)> run) {
    return cli::Command{
        "names",
        "the best name for every function and import of a module, and where it came from",
        {cli::Option{"--shared", "", "how many bodies the indexed modules share", cli::Arg::Flag},
         cli::Option{"--ordinals", "", "remake the table of names for numbered exports from definition files and import libraries (the arguments)",
                     cli::Arg::Flag},
         cli::Option{"--propose", "", "run the naming rules over the module and store what they propose (nothing is named until it is accepted)", cli::Arg::Flag},
         cli::Option{"--measure", "", "how well the rules name the functions the module itself names (its own names hidden from them): recall and precision; --rules basic: the first rules only", cli::Arg::Flag},
         cli::Option{"--rules", "", "with --measure: `basic` for the phase 1 rules", cli::Arg::Value, "SET"},
         cli::Option{"--packets", "", "for each unnamed function, the evidence an assistant needs to propose a name (JSON, a line each); --c adds its C, --limit N stops after N", cli::Arg::Flag},
         cli::Option{"--proposals", "", "the module's proposed, accepted and rejected names", cli::Arg::Flag},
         cli::Option{"--suggest", "", "store an assistant's proposal for the function at ADDRESS; the name is the second argument, with --describe and --confidence", cli::Arg::Value, "ADDRESS"},
         cli::Option{"--describe", "", "the one-line description of a suggested name", cli::Arg::Value, "TEXT"},
         cli::Option{"--confidence", "", "how sure a suggestion is, in percent (default 50)", cli::Arg::Value, "PERCENT"},
         cli::Option{"--accept", "", "make the proposal for ADDRESS its function's name; `all` takes every proposal of at least --min-confidence percent (default 80)", cli::Arg::Value, "ADDRESS"},
         cli::Option{"--min-confidence", "", "with --accept all: the lowest confidence taken, in percent", cli::Arg::Value, "PERCENT"},
         cli::Option{"--reject", "", "turn down the proposal for ADDRESS (it is not proposed again)", cli::Arg::Value, "ADDRESS"},
         cli::Option{"--c", "", "with --packets: include each function's decompiled C", cli::Arg::Flag},
         cli::Option{"--limit", "", "with --packets: at most this many functions", cli::Arg::Value, "N"},
         cli::Option{"--set", "", "name the body of the function at ADDRESS (hex, or segment:offset); the name is the second argument", cli::Arg::Value,
                     "ADDRESS"},
         cli::Positional{"arguments", "MODULE [NAME], or FILE... with --ordinals", false, true}},
        std::move(run), {}, {}, {}};
}

int Namer::execute(const cli::Args& a) {
    Impl& self = *impl_;
    const std::vector<std::string> arguments = a.values("arguments");
    if (a.flag("--ordinals")) {
        if (!self.s.collect_ordinals) throw UsageError("remaking the ordinal table is not available here");
        std::vector<std::filesystem::path> files;
        for (const std::string& f : arguments) files.push_back(fs::from_text(f));
        self.s.collect_ordinals(files);
        return 0;
    }
    if (a.flag("--shared")) {
        if (!self.s.bodies) throw UsageError("there is no body store to count bodies in");
        const store::BodyCounts c = self.s.bodies->shared_bodies();
        cb::out("{} bodies recorded, {} distinct; {} bodies are in two or more modules ({} functions are such copies)\n", c.recorded, c.distinct,
                c.in_several, c.copies);
        return 0;
    }
    for (const char* option : {"--propose", "--measure", "--packets", "--proposals", "--suggest", "--accept", "--reject"}) {
        if (!a.flag(option)) continue;
        if (!self.s.proposals_command) throw UsageError("proposed names are not available here");
        return self.s.proposals_command(a);
    }
    if (arguments.empty()) throw UsageError("names needs a module, --shared or --ordinals");
    const auto found = self.s.corpus.retail(arguments[0]);
    if (!found) throw NotFoundError(std::filesystem::path(arguments[0]), 0, "no such module in the corpus");
    const std::filesystem::path path = *found;
    if (const auto address_text = a.value("--set")) {
        if (arguments.size() < 2) throw UsageError("--set needs the address and then the name");
        const auto address = parse_address(*address_text);
        if (!address) throw UsageError("'" + *address_text + "' is not an address (hex, or segment:offset)");
        const auto digest = set_name(path, *address, arguments[1]);
        cb::out("{}\n", digest ? std::format("body {} is now {}", *digest, arguments[1]) : "that function has no body worth naming");
        return digest ? 0 : 1;
    }
    const ModuleNames names = of_module(path);
    Counts by_source;
    for (const FunctionName& f : names.functions) {
        const std::string shown = f.name.starts_with('?') ? demangle::readable(f.name) : f.name;
        cb::out("{:<10} {:<48} {}\n", f.key, shown.substr(0, 48), f.source);
        bump(by_source, std::string(first_part(first_part(f.source, " ("), ":")));
    }
    Counts by_kind;
    for (const ImportName& i : names.imports) {
        const std::string shown = i.name.starts_with('?') ? demangle::readable(i.name) : i.name;
        cb::out("import {:<12} {:<6} {:<40} {}\n", i.module, i.ordinal ? std::to_string(*i.ordinal) : std::string(), shown.substr(0, 40), i.source);
        bump(by_kind, !i.ordinal ? "by name" : i.source != "(none)" ? "by number, named" : "by number, no name");
    }
    std::stable_sort(by_source.begin(), by_source.end(), [](const auto& x, const auto& y) { return x.second > y.second; });
    std::sort(by_kind.begin(), by_kind.end());
    cb::out("{}: {} functions -- {}\n", text::lower(fs::to_text(path.filename())), names.functions.size(), joined(by_source));
    cb::out("{} imports -- {}\n", names.imports.size(), joined(by_kind));
    const auto l = self.lane(path);
    if ((l == base::Lane::Pe || l == base::Lane::Ne) && self.s.compiler) {
        const runtime_library::CompilerInfo info = self.s.compiler(path);
        cb::out("built with: {} (linker {})\n", info.compiler, info.linker.empty() ? "?" : info.linker);
    } else if (l != base::Lane::Pe && l != base::Lane::Ne) {
        cb::out("\n");   // (a file of another kind has no "built with" line, and the report ends with an empty one)
    }
    return 0;
}

int Namer::run(std::span<const std::string> args) {
    const cli::Cli program("names", {declare_command([this](const cli::Args& a) { return execute(a); })});
    std::vector<std::string> line = {"names"};
    line.insert(line.end(), args.begin(), args.end());
    return program.run(line);
}

}  // namespace cb::symbols::names
