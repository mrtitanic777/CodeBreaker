// cb-unity: alone (helpers named for what they do here)
#include "decompile/environment.hpp"

#include <algorithm>
#include <set>

#include "analyze/call_graph.hpp"
#include "analyze/classes.hpp"
#include "analyze/conventions.hpp"
#include "analyze/image.hpp"
#include "analyze/structures.hpp"
#include "base/watcom.hpp"
#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "formats/powerpc_image.hpp"
#include "symbols/name_data.hpp"

namespace cb::decompile::environment {

namespace api = symbols::api_arguments;
namespace dsc = describe;
namespace ppc = formats::powerpc_image;

namespace {

// The names a 16-bit module goes by when its file is not named after it: the reading of an export's code is by the module's own name.
// (a driver exports under its interface's name, not its file's: Setup's display driver is VGA.DRV, module DISPLAY)
std::vector<std::string> files_of_module(std::string_view upper_name) {
    if (upper_name == "KERNEL") return {"krnl386.exe", "kernel.exe"};
    if (upper_name == "USER") return {"user.exe"};
    if (upper_name == "GDI") return {"gdi.exe"};
    if (upper_name == "DISPLAY") return {"vga.drv", "display.drv"};
    if (upper_name == "MOUSE") return {"msmous31.drv", "lmouse31.drv", "mouse.drv"};
    return {};
}

std::string lower_ascii(std::string_view s) { return text::lower(s); }

// The version of the reading of an export (api_arguments' arg_bytes, describe's returns and forwards): a table made by another reading
// is not used again. Changed with any of them.
constexpr std::string_view kReading = "api_arguments.arg_bytes/describe.returns/describe.forwards:typed-cpp-1";

}  // namespace

std::string win16_scope(const base::Corpus& corpus) {
    // (the product's folder as Windows names a path: absolute, lower case, backslashes)
    std::string root = fs::to_text(std::filesystem::absolute(corpus.root()));
    std::replace(root.begin(), root.end(), '/', '\\');
    const std::string source = std::string(kReading) + lower_ascii(root);
    const crypto::Sha1Digest digest = crypto::sha1(ByteView(reinterpret_cast<const std::uint8_t*>(source.data()), source.size()));
    return corpus.current() + ":" + crypto::to_hex(digest).substr(0, 10) + "|";
}

// ---- the corpus's modules ---------------------------------------------------------------------------------------------------------

struct CorpusModules::Impl {
    explicit Impl(const base::Corpus& c) : corpus(c) {}

    // A 16-bit module as the reading of exports needs it.
    struct Ne {
        formats::ne::Module module;
        std::map<std::uint32_t, formats::ne::Entry> entries;   // fixed and moveable, by ordinal
        std::map<std::uint16_t, formats::ne::Segment> segments;
        std::string file;
    };

    const base::Corpus& corpus;
    std::mutex lock;
    std::map<std::string, std::shared_ptr<const api::ExportingModule>> pe_modules;   // by stem; null: none reads as a PE
    std::map<std::string, std::shared_ptr<const Ne>> ne_modules;                     // by name in capitals; null: none
    std::optional<std::map<std::string, std::filesystem::path>> ne_by_name;          // by the name a module gives itself

    std::shared_ptr<const Ne> read_ne(const std::filesystem::path& path) const {
        auto ne = std::make_shared<Ne>(Ne{formats::ne::Module::open_file(path), {}, {}, fs::to_text(path)});
        for (const formats::ne::Entry& e : ne->module.entries())
            if (e.kind == formats::ne::Entry::Kind::Fixed || e.kind == formats::ne::Entry::Kind::Moveable) ne->entries.insert_or_assign(e.ordinal, e);
        for (const formats::ne::Segment& s : ne->module.segments()) ne->segments.insert_or_assign(s.index, s);
        return ne;
    }

    bool is_ne(const std::filesystem::path& path) const { return corpus.lane(path) == std::optional<base::Lane>(base::Lane::Ne); }

    // The 16-bit module that exports under `name` (capitals), found by its file, or null. Caller holds the lock.
    std::shared_ptr<const Ne> ne_by_file(const std::string& name) {
        if (const auto it = ne_modules.find(name); it != ne_modules.end()) return it->second;
        std::shared_ptr<const Ne> found;
        std::vector<std::string> candidates = files_of_module(name);
        for (const char* ext : {".dll", ".exe", ".drv"}) candidates.push_back(lower_ascii(name) + ext);
        for (const std::string& candidate : candidates) {
            const std::optional<std::filesystem::path> path = corpus.retail(candidate);
            if (!path || path->empty() || !is_ne(*path)) continue;
            try {
                found = read_ne(*path);
                break;
            } catch (const Error&) {   // (a file that does not read is not the module)
            }
        }
        ne_modules[name] = found;
        return found;
    }

    // The corpus's 16-bit modules by the name each gives itself (a file called mnmnet.dll can be the module MNMNET_): all opened, once.
    const std::map<std::string, std::filesystem::path>& named() {
        if (ne_by_name) return *ne_by_name;
        std::map<std::string, std::filesystem::path> found;
        for (const std::filesystem::path& path : corpus.modules(base::Lane::Ne)) {
            try {
                found.try_emplace(text::upper(formats::ne::Module::open_file(path).module_name()), path);
            } catch (const Error&) {
            }
        }
        ne_by_name = std::move(found);
        return *ne_by_name;
    }

    std::shared_ptr<const api::ExportingModule> pe_by_stem(const std::string& stem) {
        if (const auto it = pe_modules.find(stem); it != pe_modules.end()) return it->second;
        std::shared_ptr<const api::ExportingModule> found;
        for (const std::string_view ext : api::module_extensions()) {
            const std::optional<std::filesystem::path> path = corpus.retail(lower_ascii(stem) + std::string(ext));
            if (!path || path->empty()) continue;
            try {
                found = std::make_shared<const api::ExportingModule>(formats::pe::Image::open(*path));
                break;
            } catch (const Error&) {   // (not a PE)
            }
        }
        pe_modules[stem] = found;
        return found;
    }
};

CorpusModules::CorpusModules(const base::Corpus& corpus) : impl_(std::make_unique<Impl>(corpus)) {}
CorpusModules::~CorpusModules() = default;

std::shared_ptr<const api::ExportingModule> CorpusModules::find(std::string_view stem) {
    std::lock_guard hold(impl_->lock);
    return impl_->pe_by_stem(std::string(stem));
}

std::optional<api::Win16Exports::Found> CorpusModules::find(std::string_view module, std::uint32_t ordinal) {
    std::shared_ptr<const Impl::Ne> ne;
    {
        std::lock_guard hold(impl_->lock);
        ne = impl_->ne_by_file(text::upper(module));
    }
    if (!ne) return std::nullopt;
    Found found;
    found.file = ne->file;
    const auto entry = ne->entries.find(ordinal);
    if (entry == ne->entries.end()) return found;
    const auto segment = ne->segments.find(entry->second.segment);
    if (segment == ne->segments.end() || segment->second.is_data() || segment->second.file_offset == 0) return found;
    const ByteView file = ne->module.data();
    const auto code_of = [&](const formats::ne::Segment& s) -> Bytes {
        if (s.file_offset >= file.size()) return {};
        const ByteView part = file.subspan(s.file_offset, std::min<std::uint64_t>(s.length, file.size() - s.file_offset));
        return Bytes(part.begin(), part.end());
    };
    // (a thunk that puts a word of its own under the return address and goes on: what its callers push is what the body removes less
    // what it added -- Win95 USER.471, lstrcmpi, read as its body's `retf 0Ah` gave the C two arguments too many)
    dsc::NeDescription own;
    for (const auto& [index, s] : ne->segments)
        if (!s.is_data() && s.file_offset != 0) own.segment_bytes[index] = code_of(s);
    const std::int64_t offset = entry->second.offset;
    api::ExportReturn how;
    if (const std::optional<std::int64_t> forwarded = dsc::forwards(own, entry->second.segment, offset, dsc::Distance::Far); forwarded && *forwarded >= 0)
        how.forwarded = static_cast<std::uint32_t>(*forwarded);
    const dsc::Return returned = dsc::returns(dsc::Code16(code_of(segment->second)), offset);
    how.far_return = returned.kind == std::optional<dsc::Distance>(dsc::Distance::Far);
    how.pop = returned.pops;
    found.what = how;
    return found;
}

std::optional<symbols::ordinals::ModuleExports::Table> CorpusModules::table(std::string_view stem_in) const {
    const std::string stem(stem_in);
    std::shared_ptr<const Impl::Ne> ne;
    std::shared_ptr<const api::ExportingModule> pe;
    std::optional<std::filesystem::path> pe_file;
    {
        std::lock_guard hold(impl_->lock);
        ne = impl_->ne_by_file(stem);
        if (!ne) {
            // a 32-bit module found by its file (before the 16-bit modules are searched by the names they give themselves, which opens every one)
            for (const std::string_view ext : api::module_extensions()) {
                const std::optional<std::filesystem::path> path = impl_->corpus.retail(lower_ascii(stem) + std::string(ext));
                if (path && !path->empty() && impl_->corpus.lane(*path) == std::optional<base::Lane>(base::Lane::Pe)) {
                    pe_file = *path;
                    break;
                }
            }
        }
        if (!ne && !pe_file) {
            const auto named = impl_->named().find(stem);
            if (named != impl_->named().end() && !named->second.empty()) {
                try {
                    ne = impl_->read_ne(named->second);
                } catch (const Error&) {
                }
            }
        }
    }
    Table out;
    if (ne) {
        // (the first name of each table is the module's own)
        const auto resident = ne->module.resident_names();
        const auto nonresident = ne->module.nonresident_names();
        for (std::size_t i = 1; i < nonresident.size(); ++i) out.names[nonresident[i].ordinal] = nonresident[i].name;
        for (std::size_t i = 1; i < resident.size(); ++i) out.names[resident[i].ordinal] = resident[i].name;
        out.file = lower_ascii(fs::to_text(std::filesystem::path(ne->file).filename()));
        return out;
    }
    if (pe_file) {
        try {
            for (const formats::pe::Export& e : formats::pe::Image::open(*pe_file).exports())
                if (e.name && !e.name->empty()) out.names[static_cast<std::uint32_t>(e.ordinal)] = *e.name;
            out.file = lower_ascii(fs::to_text(pe_file->filename()));
            return out;
        } catch (const Error&) {
        }
    }
    return std::nullopt;
}

// ---- the base's tables ------------------------------------------------------------------------------------------------------------

symbols::ordinals::ShippedTables shipped_tables(knowledge::Knowledge& knowledge) {
    symbols::ordinals::ShippedTables out;
    for (const auto& [module, entry] : knowledge.table("windows", "ordinal_names")) {
        if (entry.map() == nullptr) continue;
        symbols::ordinals::ShippedModule m;
        if (const knowledge::Value* from = entry.get("from"); from != nullptr && from->text() != nullptr) m.from = *from->text();
        if (const knowledge::Value* checked = entry.get("checked"); checked != nullptr && checked->list() != nullptr && checked->list()->size() == 2) {
            m.checked_yes = static_cast<std::uint32_t>((*checked->list())[0].integer().value_or(0));
            m.checked_no = static_cast<std::uint32_t>((*checked->list())[1].integer().value_or(0));
        }
        if (const knowledge::Value* names = entry.get("names"); names != nullptr && names->map() != nullptr)
            for (const auto& [number, name] : *names->map())
                if (const auto n = text::parse_int<std::uint32_t>(number, 10); n && name.text() != nullptr) m.names[*n] = *name.text();
        out[module] = std::move(m);
    }
    return out;
}

symbols::driver_services::Table driver_table(knowledge::Knowledge& knowledge) {
    return symbols::driver_services::Table::from_rows(knowledge.table("vxd", "device"), knowledge.table("vxd", "service_table"),
                                                      knowledge.table("vxd", "driver"), knowledge.table("vxd", "message"));
}

// ---- the environment ----------------------------------------------------------------------------------------------------------------

namespace {

std::vector<base::watcom::ImportedName> read_imports(const std::filesystem::path& module) {
    std::vector<base::watcom::ImportedName> out;
    for (const formats::pe::Import& i : formats::pe::Image::open(module).imports()) out.push_back({i.dll, i.name});
    return out;
}

// What the analyses find of a 32-bit module: its image, the calls between its functions, how each is called.
struct Analysis {
    analyze::Image image;
    analyze::call_graph::Graph graph;
    analyze::conventions::Facts facts;
    explicit Analysis(const formats::pe::Image& pe)
        : image(analyze::image::load_pe(pe)), graph(analyze::call_graph::build(image)), facts(analyze::conventions::functions(image, graph)) {}
};

}  // namespace

struct CorpusEnvironment::Impl {
    // What describing a module asks of the symbol tables.
    class Known final : public dsc::Knowledge {
    public:
        explicit Known(Impl& owner) : owner_(owner) {}
        std::optional<std::int64_t> win16_argument_bytes(std::string_view api_name) override {
            std::lock_guard hold(owner_.args_lock);
            return widen(api::Win16Arguments(owner_.s.win16, *owner_.modules).of(api_name));
        }
        std::optional<std::int64_t> win32_argument_bytes(std::string_view module, std::string_view api_name) override {
            std::lock_guard hold(owner_.args_lock);
            return widen(api::Win32Arguments(owner_.s.win32, owner_.scanner).stack_bytes(module, api_name));
        }
        std::optional<std::int64_t> declared_argument_bytes(std::string_view name) override {
            std::lock_guard hold(owner_.args_lock);
            return widen(owner_.s.win32.header_bytes(name));
        }
        std::optional<dsc::Declared> declared(std::string_view name) override {
            if (owner_.s.knowledge == nullptr) return std::nullopt;
            return dsc::declared_of(owner_.s.knowledge->prototype("win64", name));
        }
        std::optional<std::string> import_name(std::string_view module, std::uint32_t ordinal) override {
            return owner_.s.ordinals.identifier(module, ordinal);
        }
        std::string driver_service_label(std::uint32_t device, std::uint32_t service) override {
            return owner_.s.drivers != nullptr ? owner_.s.drivers->label(device, service) : dsc::Knowledge::driver_service_label(device, service);
        }
        std::set<std::string> imported_names(const std::filesystem::path& module) override {
            std::set<std::string> out;
            for (const auto& [name, dll] : *owner_.imports.of(module)) out.insert(name);
            return out;
        }
        Conventions conventions(const std::filesystem::path& module) override {
            Conventions out;
            std::lock_guard hold(owner_.analysis_lock);
            Analysis& a = owner_.analysis_of(module);
            for (const auto& [va, fact] : a.facts) {
                if (fact.convention == "thiscall") out.thiscall.insert(va);
                if (fact.convention == "fastcall") out.fastcall.insert(va);
            }
            return out;
        }
        std::optional<std::int64_t> pe_function_pop(const std::filesystem::path& module, std::uint32_t rva) override {
            std::lock_guard hold(owner_.args_lock);
            try {
                return widen(owner_.scanner.scan(*owner_.module_being_described(module), rva, 4));
            } catch (const Error&) {   // (code the scan cannot read: no pop count)
                return std::nullopt;
            }
        }
        std::map<dsc::Va, std::string> class_method_names(const std::filesystem::path& module) override { return analyze::classes::module_method_names(module); }

    private:
        static std::optional<std::int64_t> widen(const std::optional<std::uint32_t>& v) { return v ? std::optional<std::int64_t>(*v) : std::nullopt; }
        Impl& owner_;
    };

    explicit Impl(Sources sources)
        : s(std::move(sources)),
          modules(s.modules ? s.modules : std::make_shared<CorpusModules>(s.corpus)),
          imports(read_imports),
          scanner(*modules),
          known(*this) {}

    Sources s;
    std::shared_ptr<CorpusModules> modules;
    base::watcom::ImportMaps imports;
    std::mutex args_lock;   // the stack scan and the argument memories: one at a time (the scanner keeps decoders, the memories a base)
    api::StackScanner scanner;
    std::map<std::filesystem::path, std::shared_ptr<const api::ExportingModule>> described_modules;
    Known known;

    // The analyses of the last 32-bit module asked about (the structures and the conventions are asked in turn for one module).
    std::mutex analysis_lock;
    std::filesystem::path analysed;
    std::unique_ptr<Analysis> analysis;

    Analysis& analysis_of(const std::filesystem::path& module) {
        if (!analysis || analysed != module) {
            analysis.reset();
            analysis = std::make_unique<Analysis>(formats::pe::Image::open(module));
            analysed = module;
        }
        return *analysis;
    }

    // The module as the stack scan reads it (the module being described, not one of the corpus's).
    std::shared_ptr<const api::ExportingModule> module_being_described(const std::filesystem::path& module) {
        auto& slot = described_modules[module];
        if (!slot) slot = std::make_shared<const api::ExportingModule>(formats::pe::Image::open(module));
        return slot;
    }

    // PowerPC images read (the last few: the facts, the functions and the names are asked in turn for one).
    std::mutex ppc_lock;
    std::map<std::filesystem::path, std::shared_ptr<const ppc::Image>> ppc_images;

    std::shared_ptr<const ppc::Image> ppc_image(const std::filesystem::path& module) {
        std::lock_guard hold(ppc_lock);
        auto& slot = ppc_images[module];
        if (!slot) {
            if (ppc_images.size() > 4) {
                for (auto it = ppc_images.begin(); it != ppc_images.end();) it = (it->first != module) ? ppc_images.erase(it) : std::next(it);
            }
            slot = std::make_shared<const ppc::Image>(ppc::Image::read(module));
        }
        return slot;
    }

    symbols::names::PowerPcNames ppc_names(const std::filesystem::path& module) {
        const std::shared_ptr<const ppc::Image> image = ppc_image(module);
        symbols::names::PowerPcNames out;
        if (image->entry() != 0) out.entry = image->entry();
        out.functions.assign(image->functions().begin(), image->functions().end());
        for (const ppc::VirtualMethod& m : ppc::virtual_methods(*image)) out.methods.push_back({m.address, m.class_name, m.slot});
        return out;
    }
};

CorpusEnvironment::CorpusEnvironment(Sources sources) : impl_(std::make_unique<Impl>(std::move(sources))) {}
CorpusEnvironment::~CorpusEnvironment() = default;

dsc::Knowledge& CorpusEnvironment::knowledge() { return impl_->known; }
const translate::native_l2::Library& CorpusEnvironment::library() { return impl_->s.library; }

std::map<std::string, std::string> CorpusEnvironment::import_dlls(const std::filesystem::path& module) {
    const auto found = impl_->imports.of(module);
    return std::map<std::string, std::string>(found->begin(), found->end());
}

std::optional<std::uint32_t> CorpusEnvironment::argument_bytes(std::string_view dll, std::string_view name) {
    std::lock_guard hold(impl_->args_lock);
    return api::Win32Arguments(impl_->s.win32, impl_->scanner).stack_bytes(dll, name);
}

std::optional<std::vector<struct_text::Structure>> CorpusEnvironment::recover_structures(const std::filesystem::path& module) {
    namespace st = struct_text;
    try {
        std::lock_guard hold(impl_->analysis_lock);
        Analysis& a = impl_->analysis_of(module);
        std::vector<st::Structure> out;
        for (const analyze::structures::Structure& found : analyze::structures::recover(a.image, a.graph, analyze::conventions::summary(a.facts))) {
            st::Structure t;
            t.name = found.name;
            for (const analyze::structures::Root& r : found.roots) {
                if (r.kind == analyze::structures::Root::Kind::Arg) t.roots.push_back({st::RootKind::Arg, static_cast<std::uint64_t>(r.at), r.index});
                else if (r.kind == analyze::structures::Root::Kind::This) t.roots.push_back({st::RootKind::This, static_cast<std::uint64_t>(r.at), std::nullopt});
            }
            out.push_back(std::move(t));
        }
        return out;
    } catch (const Error&) {   // (a module the analysis cannot read: no structures)
        return std::nullopt;
    }
}

dsc::PowerPcFacts CorpusEnvironment::powerpc_facts(const std::filesystem::path& module) {
    const std::shared_ptr<const ppc::Image> image = impl_->ppc_image(module);
    dsc::PowerPcFacts out;
    for (const ppc::Span& s : image->spans()) out.spans.push_back({s.va, s.file_offset, s.size});
    out.imports.insert(image->imports().begin(), image->imports().end());
    out.inlined.insert(image->inlined().begin(), image->inlined().end());
    // what run-time type information and the entry point name for certain
    for (const auto& [address, named] : symbols::names::powerpc_names(impl_->ppc_names(module))) out.names[static_cast<decompiler::Va>(address)] = named.name;
    return out;
}

std::vector<decompiler::Va> CorpusEnvironment::powerpc_functions(const std::filesystem::path& module) {
    const std::shared_ptr<const ppc::Image> image = impl_->ppc_image(module);
    return std::vector<decompiler::Va>(image->functions().begin(), image->functions().end());
}

void CorpusEnvironment::described(const std::filesystem::path& module, const dsc::PeDescription& description) {
    if (on_described) on_described(module, description);
}

// ---- over the knowledge base ------------------------------------------------------------------------------------------------------

namespace {

// What an environment over a knowledge base is made from, kept as long as the environment.
struct KnowledgeParts {
    std::unique_ptr<api::Win32Memory> win32;
    std::unique_ptr<api::Win16Memory> win16;
    std::shared_ptr<CorpusModules> modules;
    symbols::ordinals::Ordinals ordinals;
    symbols::driver_services::Table drivers;
    knowledge::Knowledge* base;

    KnowledgeParts(const base::Corpus& corpus, knowledge::Knowledge& kb)
        : win32(kb.win32_memory()),
          win16(kb.win16_memory(win16_scope(corpus))),
          modules(std::make_shared<CorpusModules>(corpus)),
          ordinals(*modules, shipped_tables(kb), [&kb](std::string_view name) { return kb.spelling(name); }),
          drivers(driver_table(kb)),
          base(&kb) {}
};

struct PartsHolder {
    std::unique_ptr<KnowledgeParts> parts;
};

class KnowledgeEnvironment final : private PartsHolder, public CorpusEnvironment {
public:
    KnowledgeEnvironment(std::unique_ptr<KnowledgeParts> p, const base::Corpus& corpus, const translate::native_l2::Library& library)
        : PartsHolder{std::move(p)},
          CorpusEnvironment(Sources{corpus, library, *parts->win32, *parts->win16, parts->ordinals, &parts->drivers, parts->modules, parts->base}) {}
};

}  // namespace

std::unique_ptr<CorpusEnvironment> CorpusEnvironment::over_knowledge(const base::Corpus& corpus, const translate::native_l2::Library& library,
                                                                    knowledge::Knowledge& knowledge) {
    return std::make_unique<KnowledgeEnvironment>(std::make_unique<KnowledgeParts>(corpus, knowledge), corpus, library);
}

}  // namespace cb::decompile::environment
