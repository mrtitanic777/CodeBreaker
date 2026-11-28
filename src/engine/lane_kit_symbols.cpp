// engine/lane_kit_symbols.cpp -- what the decompiler and the lanes ask of the symbol tables, the analyses and the format readers, answered
// from the corpus and the knowledge base (see lane_kit.hpp).
#include <algorithm>
#include <mutex>

#include "store/knowledge_proposals.hpp"
#include "analyze/call_graph.hpp"
#include "analyze/classes.hpp"
#include "analyze/conventions.hpp"
#include "analyze/image.hpp"
#include "analyze/structures.hpp"
#include "build/flat_thunk.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "decompile/describe.hpp"
#include "decompile/environment.hpp"
#include "formats/powerpc_image.hpp"
#include "symbols/names.hpp"
#include "symbols/ordinals.hpp"
#include "engine/lane_kit_internal.hpp"
#include "formats/detect.hpp"
#include "formats/ne.hpp"
#include "formats/omf.hpp"
#include "formats/pe_functions.hpp"
#include "platforms/decoder.hpp"
#include "symbols/identify.hpp"
#include "symbols/runtime_library.hpp"

namespace cb::engine::kit_detail {

namespace stdfs = std::filesystem;
namespace aa = symbols::api_arguments;

std::shared_ptr<base::Corpus> corpus_of(const config::Config& config) {
    base::FormatProbes probes;
    probes.lane = [](const stdfs::path& p) -> std::optional<base::Lane> {
        const std::optional<formats::detect::Lane> l = formats::detect::lane(p);
        if (!l) return std::nullopt;
        switch (*l) {
        case formats::detect::Lane::Pe: return base::Lane::Pe;
        case formats::detect::Lane::Ne: return base::Lane::Ne;
        case formats::detect::Lane::Le: return base::Lane::Le;
        case formats::detect::Lane::Dos: return base::Lane::Dos;
        }
        return std::nullopt;
    };
    return std::make_shared<base::Corpus>(config.products(), std::move(probes));
}

namespace {

std::string kit_lower(std::string_view s) {
    std::string r(s);
    for (char& c : r)
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c - 'A' + 'a');
    return r;
}

// The 32-bit modules of the corpus as the stack walk asks for them: by stem, the first of the usual extensions the corpus has.
class KitModules final : public aa::ModuleSource {
public:
    explicit KitModules(base::Corpus& corpus) : corpus_(corpus) {}

    std::shared_ptr<const aa::ExportingModule> find(std::string_view stem) override {
        const std::string key(stem);
        if (const auto it = found_.find(key); it != found_.end()) return it->second;
        std::shared_ptr<const aa::ExportingModule> module;
        for (const std::string_view ext : aa::module_extensions()) {
            const std::optional<stdfs::path> path = corpus_.retail(kit_lower(stem) + std::string(ext));
            if (!path || path->empty()) continue;
            try {
                module = std::make_shared<aa::ExportingModule>(formats::pe::Image::open(*path));
                break;
            } catch (const Error&) {
                module = nullptr;
            }
        }
        return found_[key] = module;
    }

private:
    base::Corpus& corpus_;
    std::map<std::string, std::shared_ptr<const aa::ExportingModule>> found_;
};

// How the exports of the corpus's 16-bit modules return: KERNEL is KRNL386.EXE, DISPLAY an display driver ...
class KitExports16 final : public aa::Win16Exports {
public:
    explicit KitExports16(base::Corpus& corpus) : corpus_(corpus) {}

    std::optional<Found> find(std::string_view module, std::uint32_t ordinal) override {
        const std::shared_ptr<const formats::ne::Module> ne = module_named(module);
        if (!ne) return std::nullopt;
        Found found;
        found.file = file_of_[std::string(module)];
        const std::vector<formats::ne::Segment> segments = ne->segments();
        std::optional<formats::ne::Entry> entry;
        for (const formats::ne::Entry& e : ne->entries())
            if (e.ordinal == ordinal && e.kind != formats::ne::Entry::Kind::Unused) entry = e;
        const formats::ne::Segment* segment = nullptr;
        if (entry && entry->segment != 0)
            for (const formats::ne::Segment& s : segments)
                if (s.index == entry->segment) segment = &s;
        if (segment != nullptr && !segment->is_data() && segment->file_offset != 0) {
            const ByteView data = ne->data();
            const ByteView code = data.subspan(std::min<std::size_t>(segment->file_offset, data.size()),
                                               std::min<std::size_t>(segment->length, data.size() - std::min<std::size_t>(segment->file_offset, data.size())));
            // (a thunk that puts a word of its own under the return address and goes on: what its callers push is what the body removes less
            // what it added -- Win95 USER.471, lstrcmpi, read as its body's `retf 0Ah` gave the C two arguments too many)
            decompile::describe::NeDescription own;
            for (const formats::ne::Segment& g : segments) {
                if (g.is_data() || g.file_offset == 0) continue;
                const std::size_t from = std::min<std::size_t>(g.file_offset, data.size());
                const ByteView bytes = data.subspan(from, std::min<std::size_t>(g.length, data.size() - from));
                own.segment_bytes[g.index] = Bytes(bytes.begin(), bytes.end());
            }
            const std::optional<std::int64_t> forwarded = decompile::describe::forwards(own, entry->segment, entry->offset, decompile::describe::Distance::Far);
            const decompile::describe::Code16 code16{Bytes(code.begin(), code.end())};
            const decompile::describe::Return ret = decompile::describe::returns(code16, entry->offset);
            aa::ExportReturn r;
            if (forwarded && *forwarded >= 0) r.forwarded = static_cast<std::uint32_t>(*forwarded);
            r.far_return = ret.kind && *ret.kind == decompile::describe::Distance::Far;
            r.pop = ret.pops;
            found.what = r;
        }
        return found;
    }

private:
    // (a driver exports under its interface's name, not its file's: Setup's display driver is VGA.DRV, module DISPLAY)
    std::shared_ptr<const formats::ne::Module> module_named(std::string_view name_in) {
        const std::string name(name_in);
        if (const auto it = modules_.find(name); it != modules_.end()) return it->second;
        static const std::map<std::string, std::vector<std::string>> kFiles = {
            {"KERNEL", {"krnl386.exe", "kernel.exe"}},
            {"USER", {"user.exe"}},
            {"GDI", {"gdi.exe"}},
            {"DISPLAY", {"vga.drv", "display.drv"}},
            {"MOUSE", {"msmous31.drv", "lmouse31.drv", "mouse.drv"}},
        };
        std::vector<std::string> candidates;
        if (const auto it = kFiles.find(name); it != kFiles.end()) candidates = it->second;
        for (const char* ext : {".dll", ".exe", ".drv"}) candidates.push_back(kit_lower(name) + ext);
        std::shared_ptr<const formats::ne::Module> found;
        for (const std::string& candidate : candidates) {
            const std::optional<stdfs::path> path = corpus_.retail(candidate);
            if (!path || path->empty()) continue;
            const std::optional<base::Lane> lane = corpus_.lane(*path);
            if (!lane || *lane != base::Lane::Ne) continue;
            try {
                found = std::make_shared<formats::ne::Module>(formats::ne::Module::open_file(*path));
                file_of_[name] = kit_lower(fs::to_text(path->filename()));
                break;
            } catch (const Error&) {
                found = nullptr;
            }
        }
        return modules_[name] = found;
    }

    base::Corpus& corpus_;
    std::map<std::string, std::shared_ptr<const formats::ne::Module>> modules_;
    std::map<std::string, std::string> file_of_;
};

}  // namespace

// ---- the sizes ---------------------------------------------------------------------------------------------------------------

struct KitSizes::Impl {
    Impl(base::Corpus& c, knowledge::Knowledge& k) : corpus(c), kb(k), modules(c), scanner(modules), exports16(c) {}

    base::Corpus& corpus;
    knowledge::Knowledge& kb;
    std::mutex lock;   // the scanner keeps decoders and the modules what was found in them: one walk at a time
    KitModules modules;
    aa::StackScanner scanner;
    KitExports16 exports16;
    std::unique_ptr<aa::Win32Memory> memory32;
    std::unique_ptr<aa::Win16Memory> memory16;
    std::map<stdfs::path, std::shared_ptr<const aa::ExportingModule>> by_path;
    std::map<stdfs::path, std::vector<std::uint64_t>> stubs;

    aa::Win32Memory& m32() {
        if (!memory32) memory32 = kb.win32_memory();
        return *memory32;
    }
    aa::Win16Memory& m16() {
        if (!memory16) {
            // One table per product, and per way of reading it: USER.171 is not the same function in every Windows.
            const std::string scope = corpus.current() + ":" + kit_lower(fs::to_text(corpus.root()));
            memory16 = kb.win16_memory(scope);
        }
        return *memory16;
    }
};

KitSizes::KitSizes(base::Corpus& corpus, knowledge::Knowledge& knowledge) : impl_(std::make_unique<Impl>(corpus, knowledge)) {}
KitSizes::~KitSizes() = default;

std::optional<std::uint32_t> KitSizes::stack_bytes(std::string_view dll, std::string_view name) {
    std::lock_guard hold(impl_->lock);
    return aa::Win32Arguments(impl_->m32(), impl_->scanner).stack_bytes(dll, name);
}

std::optional<std::int64_t> KitSizes::argc(std::string_view dll, std::string_view name) {
    std::lock_guard hold(impl_->lock);
    const std::optional<std::uint32_t> n = aa::Win32Arguments(impl_->m32(), impl_->scanner).argc(dll, name);
    return n ? std::optional<std::int64_t>(*n) : std::nullopt;
}

std::optional<std::uint32_t> KitSizes::header_bytes(std::string_view name) {
    std::lock_guard hold(impl_->lock);
    return impl_->m32().header_bytes(name);
}

std::optional<std::uint32_t> KitSizes::pe_function_pop(const stdfs::path& module, std::uint32_t rva) {
    std::lock_guard hold(impl_->lock);
    std::shared_ptr<const aa::ExportingModule>& m = impl_->by_path[module];
    if (!m) m = std::make_shared<aa::ExportingModule>(formats::pe::Image::open(module));
    try {
        return impl_->scanner.scan(*m, rva, 4);
    } catch (const NotFoundError&) {
        throw;   // (no capstone is not "no pop count")
    } catch (const Error&) {
        return std::nullopt;
    }
}

std::optional<std::uint32_t> KitSizes::win16_bytes(std::string_view api) {
    std::lock_guard hold(impl_->lock);
    return aa::Win16Arguments(impl_->m16(), impl_->exports16).of(api);
}

std::string KitSizes::win16_table_text() {
    std::lock_guard hold(impl_->lock);
    return aa::Win16Arguments(impl_->m16(), impl_->exports16).table_text();
}

std::vector<std::uint64_t> KitSizes::flat_thunk_stubs(const stdfs::path& module) {
    {
        std::lock_guard hold(impl_->lock);
        if (const auto it = impl_->stubs.find(module); it != impl_->stubs.end()) return it->second;
    }
    std::vector<std::uint64_t> out;
    try {
        const build::flat_thunk::Classification c = build::flat_thunk::classify(module);
        if (c.subtype == build::flat_thunk::Subtype::FlatThunk) {
            const formats::pe_functions::PeCarve carve = formats::pe_functions::PeCarve::open(module);
            std::vector<symbols::runtime_library::CodeRegion> regions;
            for (const formats::pe_functions::CodeSection& s : carve.sections()) regions.push_back({static_cast<std::uint64_t>(s.va), s.data});
            std::vector<std::uint64_t> starts;
            for (const auto& [va, function] : carve.functions()) starts.push_back(static_cast<std::uint64_t>(va));
            platforms::DecoderSet decoders;
            out = symbols::runtime_library::flat_thunk_stubs(regions, starts, decoders);
        }
    } catch (const Error&) {   // (not readable as one: not one)
        out.clear();
    }
    std::lock_guard hold(impl_->lock);
    return impl_->stubs[module] = std::move(out);
}

// ---- what the decompiler asks ------------------------------------------------------------------------------------------------------

namespace {

// What the describer asks of the symbol tables.
class KitKnowledge final : public decompile::describe::Knowledge {
public:
    KitKnowledge(KitSizes& sizes, knowledge::Knowledge& kb, base::Corpus& corpus, base::watcom::ImportMaps& imports, KitEnvironment& env)
        : sizes_(sizes), kb_(kb), corpus_(corpus), imports_(imports), env_(env) {}

    std::optional<std::int64_t> win16_argument_bytes(std::string_view api) override { return widen(sizes_.win16_bytes(api)); }
    std::optional<std::int64_t> win32_argument_bytes(std::string_view module_stem, std::string_view api) override {
        return widen(sizes_.stack_bytes(module_stem, api));
    }
    std::optional<std::int64_t> declared_argument_bytes(std::string_view name) override { return widen(sizes_.header_bytes(name)); }
    std::optional<decompile::describe::Declared> declared(std::string_view name) override { return decompile::describe::declared_of(kb_.prototype("win64", name)); }
    std::optional<std::string> import_name(std::string_view module, std::uint32_t ordinal) override {
        // the export's name as the module's own tables give it (ordinal_names.json: {module: {from, names: {ordinal: name}}})
        const std::string stem = aa::stem(module);
        const std::optional<knowledge::Value> row = kb_.known("windows", "ordinal_names", stem);
        if (!row) return std::nullopt;
        const knowledge::Value* names = row->get("names");
        if (names == nullptr) return std::nullopt;
        const knowledge::Value* name = names->get(std::to_string(ordinal));
        if (name == nullptr || name->text() == nullptr) return std::nullopt;
        const std::string spelled = kb_.spelling(*name->text());
        return is_identifier(spelled) ? std::optional<std::string>(spelled) : std::nullopt;
    }
    std::string driver_service_label(std::uint32_t device, std::uint32_t service) override {
        return decompile::describe::Knowledge::driver_service_label(device, service);
    }
    std::set<std::string> imported_names(const stdfs::path& module) override {
        std::set<std::string> out;
        for (const auto& [name, dll] : *imports_.of(module)) out.insert(name);
        return out;
    }
    Conventions conventions(const stdfs::path& module) override {
        Conventions c;
        analyze::Image image = analyze::image::load_pe(formats::pe::Image::open(module));
        const analyze::call_graph::Graph graph = analyze::call_graph::build(image);
        for (const auto& [va, fact] : analyze::conventions::functions(image, graph)) {
            if (fact.convention && *fact.convention == "thiscall") c.thiscall.insert(va);
            if (fact.convention && *fact.convention == "fastcall") c.fastcall.insert(va);
        }
        return c;
    }
    std::optional<std::int64_t> pe_function_pop(const stdfs::path& module, std::uint32_t rva) override {
        return widen(sizes_.pe_function_pop(module, rva));
    }
    std::map<core16::Va, std::string> class_method_names(const stdfs::path& module) override { return analyze::classes::module_method_names(module); }

private:
    static std::optional<std::int64_t> widen(const std::optional<std::uint32_t>& v) { return v ? std::optional<std::int64_t>(*v) : std::nullopt; }
    static bool is_identifier(std::string_view s) {
        if (s.empty() || (s[0] >= '0' && s[0] <= '9')) return false;
        return std::all_of(s.begin(), s.end(), [](char c) { return c == '_' || (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); });
    }

    KitSizes& sizes_;
    knowledge::Knowledge& kb_;
    base::Corpus& corpus_;
    base::watcom::ImportMaps& imports_;
    KitEnvironment& env_;
};

}  // namespace

struct KitEnvironment::Impl {
    Impl(const config::Config& c, base::Corpus& k, knowledge::Knowledge& kn)
        : config(c), corpus(k), kb(kn), sizes(k, kn), imports([](const stdfs::path& module) {
              std::vector<base::watcom::ImportedName> out;
              for (const formats::pe::Import& i : formats::pe::Image::open(module).imports()) out.push_back({i.dll, i.name});
              return out;
          }) {}

    const config::Config& config;
    base::Corpus& corpus;
    knowledge::Knowledge& kb;
    KitSizes sizes;
    base::watcom::ImportMaps imports;
    std::unique_ptr<KitKnowledge> describing;
    std::mutex lock;
    std::optional<translate::native_l2::Library> library;
    std::optional<symbols::runtime_library::SignatureTable> signatures;   // the runtime library's functions by their leading bytes, from the knowledge base
    std::optional<symbols::runtime_library::SignatureTable> signatures16;   // the same of a 16-bit module's
    std::optional<symbols::runtime_library::SignatureTable> signatures64;   // and of an x64 module's
    std::map<stdfs::path, std::unordered_map<std::string, base::watcom::NumberedImport>> numbered;   // by module: what the description said

    // PowerPC images read (the last few: the facts, the functions and the names are asked in turn for one), and what names their functions.
    std::mutex ppc_lock;
    std::map<stdfs::path, std::shared_ptr<const formats::powerpc_image::Image>> ppc_images;
    std::shared_ptr<decompile::environment::CorpusModules> ppc_modules;
    std::unique_ptr<symbols::ordinals::Ordinals> ppc_ordinals;
    std::unique_ptr<symbols::names::Namer> ppc_namer;

    std::shared_ptr<const formats::powerpc_image::Image> ppc_image(const stdfs::path& module) {
        std::lock_guard hold(ppc_lock);
        auto& slot = ppc_images[module];
        if (!slot) {
            if (ppc_images.size() > 4) {
                for (auto it = ppc_images.begin(); it != ppc_images.end();) it = (it->first != module) ? ppc_images.erase(it) : std::next(it);
            }
            slot = std::make_shared<const formats::powerpc_image::Image>(formats::powerpc_image::Image::read(module));
        }
        return slot;
    }

    // The namer, made when first asked (naming a PowerPC image's functions needs no ordinal table, but the namer holds one).
    symbols::names::Namer& namer() {
        std::lock_guard hold(ppc_lock);
        if (!ppc_namer) {
            ppc_modules = std::make_shared<decompile::environment::CorpusModules>(corpus);
            knowledge::Knowledge& base = kb;
            ppc_ordinals = std::make_unique<symbols::ordinals::Ordinals>(*ppc_modules, decompile::environment::shipped_tables(base),
                                                                         [&base](std::string_view name) { return base.spelling(name); });
            symbols::names::Namer::Services services{corpus, *ppc_ordinals, nullptr};
            services.powerpc = [this](const stdfs::path& module) -> std::optional<symbols::names::PowerPcNames> {
                const std::shared_ptr<const formats::powerpc_image::Image> image = ppc_image(module);   // (ppc_lock is not held when the namer asks)
                symbols::names::PowerPcNames out;
                if (image->entry() != 0) out.entry = image->entry();
                out.functions.assign(image->functions().begin(), image->functions().end());
                for (const formats::powerpc_image::VirtualMethod& m : formats::powerpc_image::virtual_methods(*image))
                    out.methods.push_back({m.address, m.class_name, m.slot});
                return out;
            };
            ppc_namer = std::make_unique<symbols::names::Namer>(std::move(services));
        }
        return *ppc_namer;
    }
};

KitEnvironment::KitEnvironment(const config::Config& config, base::Corpus& corpus, knowledge::Knowledge& kb)
    : impl_(std::make_unique<Impl>(config, corpus, kb)) {}
KitEnvironment::~KitEnvironment() = default;

KitSizes& KitEnvironment::sizes() { return impl_->sizes; }

decompile::describe::Knowledge& KitEnvironment::knowledge() {
    std::lock_guard hold(impl_->lock);
    if (!impl_->describing)
        impl_->describing = std::make_unique<KitKnowledge>(impl_->sizes, impl_->kb, impl_->corpus, impl_->imports, *this);
    return *impl_->describing;
}

const translate::native_l2::Library& KitEnvironment::library() {
    std::lock_guard hold(impl_->lock);
    if (!impl_->library) impl_->library = translate::native_l2::Library::builtin();
    return *impl_->library;
}

std::map<std::string, std::string> KitEnvironment::import_dlls(const stdfs::path& module) {
    std::map<std::string, std::string> out;
    for (const auto& [name, dll] : *impl_->imports.of(module)) out[name] = dll;
    return out;
}

std::optional<std::uint32_t> KitEnvironment::argument_bytes(std::string_view dll, std::string_view name) { return impl_->sizes.stack_bytes(dll, name); }

std::optional<std::vector<decompile::struct_text::Structure>> KitEnvironment::recover_structures(const stdfs::path& module) {
    namespace st = decompile::struct_text;
    try {   // (the analysis may throw: the plan is then empty)
        analyze::Image image = analyze::image::load_pe(formats::pe::Image::open(module));
        const analyze::call_graph::Graph graph = analyze::call_graph::build(image);
        const analyze::ConventionFacts conventions = analyze::conventions::summary(analyze::conventions::functions(image, graph));
        std::vector<st::Structure> out;
        for (const analyze::structures::Structure& s : analyze::structures::recover(image, graph, conventions)) {
            st::Structure t;
            t.name = s.name;
            for (const analyze::structures::Root& r : s.roots) {
                if (r.kind == analyze::structures::Root::Kind::Arg) t.roots.push_back({st::RootKind::Arg, static_cast<std::uint64_t>(r.at), r.index});
                else if (r.kind == analyze::structures::Root::Kind::This) t.roots.push_back({st::RootKind::This, static_cast<std::uint64_t>(r.at), std::nullopt});
            }
            out.push_back(std::move(t));
        }
        return out;
    } catch (const Error&) {
        return std::nullopt;
    }
}

decompile::describe::PowerPcFacts KitEnvironment::powerpc_facts(const stdfs::path& module) {
    const std::shared_ptr<const formats::powerpc_image::Image> image = impl_->ppc_image(module);
    decompile::describe::PowerPcFacts out;
    for (const formats::powerpc_image::Span& s : image->spans()) out.spans.push_back({s.va, s.file_offset, s.size});
    out.imports.insert(image->imports().begin(), image->imports().end());
    out.inlined.insert(image->inlined().begin(), image->inlined().end());
    // what run-time type information and the entry point name for certain
    for (const auto& [address, named] : impl_->namer().powerpc_names(module)) out.names[static_cast<decompile::decompiler::Va>(address)] = named.name;
    return out;
}

std::vector<decompile::decompiler::Va> KitEnvironment::powerpc_functions(const stdfs::path& module) {
    const std::shared_ptr<const formats::powerpc_image::Image> image = impl_->ppc_image(module);
    return std::vector<decompile::decompiler::Va>(image->functions().begin(), image->functions().end());
}

// what the builders ask of a module that was decompiled: the imports it has by number
void KitEnvironment::described(const stdfs::path& module, const decompile::describe::PeDescription& d) {
    std::unordered_map<std::string, base::watcom::NumberedImport> numbered;
    for (const auto& [name, imp] : d.by_number) {
        base::watcom::NumberedImport n;
        n.dll = imp.module;
        n.number = static_cast<std::uint32_t>(imp.number);
        if (imp.pop && *imp.pop >= 0) n.argument_bytes = static_cast<std::uint32_t>(*imp.pop);
        numbered[name] = std::move(n);
    }
    std::lock_guard hold(impl_->lock);
    impl_->numbered[module] = std::move(numbered);
}

// The functions of a real-mode program that came with its compiler, by signature. The 16-bit table is shared with the Windows 3 modules': a library's
// bytes are the same wherever the function was linked, and the rows name the library each came from.
std::map<decompile::decompiler::Va, decompile::decompiler::LibraryFunction> KitEnvironment::dos_library(
    const formats::dos::Program& program, std::span<const decompile::decompiler::Va> starts) {
    symbols::runtime_library::DosImage image{program.image, {}, static_cast<std::uint32_t>(program.base), {}};
    for (const formats::dos::Entry& e : program.entries)
        if (e.address >= program.base) image.entries.push_back(static_cast<std::uint32_t>(e.address - program.base));
    const auto base = static_cast<decompile::decompiler::Va>(program.base);
    for (const decompile::decompiler::Va va : starts)
        if (va >= base && va - base < static_cast<decompile::decompiler::Va>(program.image.size())) image.function_starts.push_back(static_cast<std::uint32_t>(va - base));
    std::sort(image.function_starts.begin(), image.function_starts.end());
    std::map<decompile::decompiler::Va, decompile::decompiler::LibraryFunction> out;
    std::lock_guard hold(impl_->lock);
    if (!impl_->signatures16) impl_->signatures16 = symbols::runtime_library::SignatureTable::from_rows(impl_->kb.signatures("win16"));
    for (const auto& [offset, found] : symbols::runtime_library::identify_dos(*impl_->signatures16, image)) out[base + offset] = {found.name, found.library, found.bytes};
    return out;
}

// The names accepted for a program's functions (names MODULE --suggest ... --accept): proposals the knowledge base keeps under the
// program's hash, by the namer's key (a real-mode program's: its function's linear address in hex).
std::map<decompile::decompiler::Va, std::string> KitEnvironment::program_names(const stdfs::path& module) {
    std::map<decompile::decompiler::Va, std::string> out;
    const std::optional<std::string> sha = impl_->corpus.sha(module);
    if (!sha) return out;
    store::KnowledgeProposals proposals(impl_->kb.store());
    for (const store::Proposal& p : proposals.of_module(*sha)) {
        if (p.state != store::ProposalState::Accepted || p.key.find(':') != std::string::npos) continue;
        if (const auto va = text::parse_int<decompile::decompiler::Va>(p.key, 16)) out[*va] = p.name;
    }
    return out;
}

std::function<std::vector<std::string>(ByteView)> KitEnvironment::import_lines(const stdfs::path& module, const std::vector<std::string>& used,
                                                                                const base::watcom::OpenWatcom&) {
    const std::shared_ptr<const base::watcom::ImportMap> map = impl_->imports.of(module);
    std::unordered_map<std::string, base::watcom::NumberedImport> numbered;
    {
        std::lock_guard hold(impl_->lock);
        if (const auto it = impl_->numbered.find(module); it != impl_->numbered.end()) numbered = it->second;
    }
    return [this, map, numbered = std::move(numbered), used, module](ByteView object) {
        const std::vector<std::string> externs = formats::omf::externs(object);
        const std::string text = base::watcom::import_lines(externs, used, *map, impl_->imports, &numbered);
        std::vector<std::string> lines;
        for (const std::string_view line : text::lines(text::trim(text))) lines.emplace_back(line);
        return lines;
    };
}

// {function key: {name, library}} of the functions of a 32-bit module that belong to a runtime library: by signature, the thunk compiler's stubs, and for a
// module that is itself a runtime library its exports. A 16-bit module's are found among the functions its describer finds, keyed as the 16-bit lane keys them.
std::set<decompile::decompiler::Va> KitEnvironment::runtime_functions(const stdfs::path& module) {
    std::set<decompile::decompiler::Va> out;
    for (const auto& [address, what] : identify(module))
        if (address.find(':') == std::string::npos) out.insert(static_cast<decompile::decompiler::Va>(std::stoul(address, nullptr, 16)));
    return out;
}

std::map<decompile::decompiler::Va, decompile::decompiler::LibraryFunction> KitEnvironment::pe_library(const stdfs::path& module) {
    std::map<decompile::decompiler::Va, decompile::decompiler::LibraryFunction> out;
    try {
        const formats::pe::Image image = formats::pe::Image::open(module);
        if (image.header().machine != formats::pe::kMachineX64) return out;
        std::lock_guard hold(impl_->lock);
        if (!impl_->signatures64) impl_->signatures64 = symbols::runtime_library::SignatureTable::from_rows(impl_->kb.signatures("win64"));
        for (const auto& [va, found] : symbols::identify::identify_x64(image, aa::stem(fs::to_text(module.filename())), *impl_->signatures64))
            out[static_cast<decompile::decompiler::Va>(va)] = {found.name, found.library, found.bytes};
    } catch (const Error&) {   // (a module that does not read as a PE has nothing identified)
        out.clear();
    }
    return out;
}

std::map<std::string, std::pair<std::string, std::string>> KitEnvironment::identify(const stdfs::path& module) {
    std::map<std::string, std::pair<std::string, std::string>> out;
    const std::optional<base::Lane> lane = impl_->corpus.lane(module);
    if (lane && *lane == base::Lane::Ne) {   // a 16-bit module: the functions the describer finds, by their leading bytes (keys "seg:off", the 16-bit lane's)
        try {
            const formats::ne::Module ne = formats::ne::Module::open_file(module);
            const Bytes file(ne.data().begin(), ne.data().end());
            symbols::runtime_library::Ne16Module m;
            m.module_name = ne.module_name();
            for (const formats::ne::Segment& s : ne.segments()) {
                symbols::runtime_library::Ne16Segment seg;
                seg.index = s.index;
                seg.code = !s.is_data();
                if (s.file_offset != 0 && s.file_offset <= file.size()) seg.bytes = ByteView(file).subspan(s.file_offset, std::min<std::uint64_t>(s.length, file.size() - s.file_offset));
                m.segments.push_back(seg);
            }
            for (const auto& [at, distance] : decompile::describe::find_functions(ne)) {
                (void)distance;
                m.function_starts.push_back({static_cast<std::uint16_t>(at.seg), static_cast<std::uint16_t>(at.off)});
            }
            std::ranges::sort(m.function_starts);
            for (const formats::ne::Entry& e : ne.entries()) {
                if (e.kind == formats::ne::Entry::Kind::Unused) continue;
                m.entries.push_back({static_cast<std::uint16_t>(e.ordinal), e.segment, e.offset});
            }
            std::lock_guard hold(impl_->lock);
            if (!impl_->signatures16) impl_->signatures16 = symbols::runtime_library::SignatureTable::from_rows(impl_->kb.signatures("win16"));
            for (const auto& [at, found] : symbols::runtime_library::identify_ne(*impl_->signatures16, m)) out[std::format("{}:{:04x}", at.segment, at.offset)] = {found.name, found.library};
        } catch (const Error&) {   // (a module that does not read as an NE has nothing identified)
            out.clear();
        }
        return out;
    }
    if (!lane || *lane != base::Lane::Pe) return out;
    try {
        if (formats::pe::Image::open(module).header().machine == formats::pe::kMachineX64) {   // (x64: the functions .pdata states)
            for (const auto& [va, f] : pe_library(module)) out[std::format("{:x}", va)] = {f.name, f.library};
            return out;
        }
        const formats::pe_functions::PeCarve carve = formats::pe_functions::PeCarve::open(module);
        symbols::runtime_library::Pe32Module m;
        m.stem = aa::stem(fs::to_text(module.filename()));
        m.image_base = static_cast<std::uint64_t>(carve.image_base());
        for (const formats::pe_functions::CodeSection& s : carve.sections()) m.code.push_back({static_cast<std::uint64_t>(s.va), s.data});
        for (const auto& [va, function] : carve.functions()) m.function_starts.push_back(static_cast<std::uint64_t>(va));
        for (const formats::pe::ExportSymbol& e : carve.image().export_table().symbols) m.exports.push_back({static_cast<std::uint32_t>(e.ordinal), e.name, e.rva});
        m.flat_thunk = build::flat_thunk::classify(module).subtype == build::flat_thunk::Subtype::FlatThunk;
        std::lock_guard hold(impl_->lock);
        if (!impl_->signatures) impl_->signatures = symbols::runtime_library::SignatureTable::from_rows(impl_->kb.signatures("win32"));
        platforms::DecoderSet decoders;
        for (const auto& [va, found] : symbols::runtime_library::identify_pe(*impl_->signatures, m, decoders)) out[std::format("{:x}", va)] = {found.name, found.library};
    } catch (const Error&) {   // (a module that does not read as a PE has nothing identified)
        out.clear();
    }
    return out;
}

}  // namespace cb::engine::kit_detail
