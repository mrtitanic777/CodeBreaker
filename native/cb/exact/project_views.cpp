// The stages' views of a Project: each stage declares what it needs of "a project" (modules::Project, library::Project,
// ledger::Project ...), and these classes answer from the Project they are made from. Conversion and forwarding only.
#include <algorithm>
#include <format>

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/exact/project.hpp"

namespace cb::exact::project {

namespace stdfs = std::filesystem;

namespace {

using ProjectPtr = std::shared_ptr<Project>;

std::vector<std::string> pv_texts(const std::vector<stdfs::path>& paths) {
    std::vector<std::string> out;
    for (const stdfs::path& p : paths) out.push_back(fs::to_text(p));
    return out;
}

// The members every view that reads like modules::Project answers.
template <class Base>
class PvModules : public Base {
public:
    explicit PvModules(ProjectPtr p) : p_(std::move(p)) {}
    stdfs::path src() const override { return p_->src(); }
    stdfs::path path(std::string_view name) const override { return p_->path(name); }
    std::uint32_t code_end() const override { return p_->code_end(); }
    std::uint32_t own_end() const override { return p_->own_end(); }
    std::optional<std::string> switches() const override { return p_->switches(); }
    std::optional<std::string> config_text(std::string_view key) const override {
        const auto it = p_->config().strings.find(std::string(key));
        return it == p_->config().strings.end() ? std::nullopt : std::optional<std::string>(it->second);
    }
    std::vector<std::uint32_t> code_segments() const override { return p_->code_segments(); }
    std::shared_ptr<const modules::Toolchain> toolchain() const override { return p_->toolchain(); }
    std::map<std::string, Bytes> headers() const override { return p_->headers(); }
    std::vector<std::string> include() const override { return pv_texts(p_->include()); }
    std::vector<std::shared_ptr<const modules::Project>> also_sets() const override {
        std::vector<std::shared_ptr<const modules::Project>> out;
        const auto all = p_->sets();
        for (std::size_t k = 1; k < all.size(); ++k) out.push_back(std::make_shared<PvModules<modules::Project>>(all[k]));
        return out;
    }
    ByteView image() const override { return p_->image(); }
    SymbolTable symbols() const override { return p_->symbols(); }
    compare::Program program() const override { return p_->program(); }

protected:
    ProjectPtr p_;
};

using PvModulesView = PvModules<modules::Project>;

class PvData final : public PvModules<data::Project> {
public:
    using PvModules<data::Project>::PvModules;
    std::string data_order() const override { return p_->profile_parts()->data_order; }
};

class PvLibrary final : public PvModules<library::Project> {
public:
    using PvModules<library::Project>::PvModules;
    std::vector<LibraryFile> libraries() const override {
        const auto parts = p_->profile_parts();
        const toolchain::Locations& where = p_->services().toolchains->locations();
        std::vector<LibraryFile> out;
        if (const auto it = parts->libraries.find(p_->config().model); it != parts->libraries.end())
            for (const LibraryRef& x : it->second) out.push_back({x.tag, where.kit_path(parts->kit, x.path)});
        for (const LibraryRef& x : p_->config().libraries) out.push_back({x.tag, where.kit_path(std::nullopt, x.path, p_->dir())});
        return out;
    }
    std::vector<stdfs::path> kit_objects() const override {
        const auto parts = p_->profile_parts();
        const toolchain::Locations& where = p_->services().toolchains->locations();
        std::vector<stdfs::path> out;
        if (const auto it = parts->objects.find(p_->config().model); it != parts->objects.end())
            for (const std::string& x : it->second) out.push_back(where.kit_path(parts->kit, x));
        return out;
    }
    std::vector<stdfs::path> first_objects() const override {
        const auto parts = p_->profile_parts();
        const toolchain::Locations& where = p_->services().toolchains->locations();
        std::vector<stdfs::path> out;
        for (const std::string& x : p_->config().first_objects) out.push_back(where.kit_path(parts->kit, x));
        return out;
    }
    std::uint32_t library_gap() const override { return p_->config().library_gap; }
    std::optional<std::uint32_t> library_start() const override { return p_->config().library_start; }
    std::map<std::string, std::uint32_t> data_sizes() const override { return p_->config().data_sizes; }
    std::vector<DataPad> data_pads() const override { return p_->config().data_pads; }
};

class PvLedger final : public ledger::Project {
public:
    explicit PvLedger(ProjectPtr p) : p_(std::move(p)) {}
    stdfs::path dir() const override { return p_->dir(); }
    stdfs::path src() const override { return p_->src(); }
    std::string exe() const override { return fs::to_text(p_->exe()); }
    stdfs::path path(std::string_view name) const override { return p_->path(name); }
    std::optional<std::string> config_text(std::string_view key) const override {
        const auto it = p_->config().strings.find(std::string(key));
        return it == p_->config().strings.end() ? std::nullopt : std::optional<std::string>(it->second);
    }
    const pascal_join::Project* compilands() const override {
        if (p_->config().pascal.compilands.empty()) return nullptr;
        if (!pascal_) pascal_ = p_->pascal_project();
        return &*pascal_;
    }
    std::map<std::string, Bytes> headers() const override { return p_->headers(); }
    std::vector<std::string> include() const override { return pv_texts(p_->include()); }
    std::optional<std::string> switches() const override { return p_->switches(); }
    std::vector<std::uint32_t> starts() const override { return p_->starts().value_or(std::vector<std::uint32_t>{}); }
    bool whole(const stdfs::path& file) const override { return p_->whole(file); }
    ByteView image() const override { return p_->image(); }
    std::uint32_t code_end() const override { return p_->code_end(); }
    std::vector<ledger::LabelledToolchain> toolchains() const override {
        std::vector<ledger::LabelledToolchain> out;
        for (NamedToolchain& t : p_->toolchains()) out.push_back({std::move(t.label), std::move(t.toolchain)});
        return out;
    }
    SymbolTable symbols() const override { return p_->symbols(); }
    void save_symbols(const SymbolTable& symbols) const override { p_->save_symbols(symbols); }
    std::vector<ledger::SourceFile> sources() const override {
        std::vector<ledger::SourceFile> out;
        for (Source& s : p_->sources()) out.push_back({s.at, std::move(s.path)});
        return out;
    }
    std::vector<std::shared_ptr<const ledger::Project>> sets() const override {
        std::vector<std::shared_ptr<const ledger::Project>> out;
        for (const ProjectPtr& s : p_->sets()) out.push_back(std::make_shared<PvLedger>(s));
        return out;
    }
    compare::Program program() const override { return p_->program(); }
    const x86::Decoder16& decoder() const override {
        const x86::Decoder16* d = p_->services().decoder;
        return d ? *d : x86::capstone_decoder();
    }

private:
    ProjectPtr p_;
    mutable std::optional<pascal_join::Project> pascal_;   // (made when a stage asks)
};

class PvLinkwhole final : public linkwhole::Project {
public:
    explicit PvLinkwhole(ProjectPtr p) : p_(std::move(p)) {}
    stdfs::path dir() const override { return p_->dir(); }
    stdfs::path src() const override { return p_->src(); }
    std::vector<std::pair<std::uint32_t, stdfs::path>> sources() const override {
        std::vector<std::pair<std::uint32_t, stdfs::path>> out;
        for (Source& s : p_->sources())
            if (s.at >= 0) out.emplace_back(static_cast<std::uint32_t>(s.at), std::move(s.path));   // (a file with no place is not linked by place)
        return out;
    }
    std::shared_ptr<const modules::Toolchain> toolchain() const override { return p_->toolchain(); }
    std::map<std::string, Bytes> headers() const override { return p_->headers(); }
    std::vector<std::string> include() const override { return pv_texts(p_->include()); }
    std::optional<std::string> switches() const override { return p_->switches(); }
    std::vector<std::shared_ptr<const linkwhole::Project>> sets() const override {
        std::vector<std::shared_ptr<const linkwhole::Project>> out;
        for (const ProjectPtr& s : p_->sets()) out.push_back(std::make_shared<PvLinkwhole>(s));
        return out;
    }
    const linkwhole::LinkConfig& link() const override { return p_->config().link; }
    ByteView image() const override { return p_->image(); }
    ByteView file() const override { return p_->file(); }
    std::string exe() const override { return fs::to_text(p_->exe()); }
    SymbolTable symbols() const override { return p_->symbols(); }
    stdfs::path resolve(std::string_view spec) const override {
        if (const auto* tc = p_->services().toolchains) return tc->locations().kit_path(std::nullopt, spec, p_->dir());
        return (p_->dir() / fs::from_text(spec)).lexically_normal();
    }
    std::shared_ptr<const modules::Toolchain> load_toolchain(std::string_view name, const std::optional<std::string>& variant) const override {
        if (!p_->services().toolchains) throw UsageError("exact project: no compilers (Services::toolchains) were given");
        return p_->services().toolchains->load(std::string(name), variant);
    }

private:
    ProjectPtr p_;
};

class PvReadable final : public readable::Project {
public:
    explicit PvReadable(ProjectPtr p) : p_(std::move(p)), symbols_(p_->symbols()) {}
    const SymbolTable& symbols() const override { return symbols_; }
    std::vector<readable::SourceFile> sources() const override {
        std::vector<readable::SourceFile> out;
        for (Source& s : p_->sources())
            if (s.at >= 0) out.push_back({static_cast<std::uint32_t>(s.at), std::move(s.path)});
        return out;
    }
    const stdfs::path& source_dir() const override { return p_->src(); }
    std::vector<readable::LedgerRow> ledger() const override {
        std::vector<readable::LedgerRow> out;
        for (const ledger::Row& r : ledger::run(*p_->as_ledger()))
            out.push_back({r.file, r.function.value_or(""), std::string(pascal_join::verdict_text(r.verdict))});
        return out;
    }
    std::unique_ptr<readable::Project> with_source_dir(const stdfs::path& source_dir) const override {
        Json override_;
        override_["src"] = fs::to_text(source_dir);
        return std::make_unique<PvReadable>(project::Project::open(p_->dir(), p_->services_ptr(), json::dump(override_, json::Style::Compact)));
    }

private:
    ProjectPtr p_;
    SymbolTable symbols_;
};

constexpr std::uint32_t kNoSegment = 0xFFFFFFFFu;     // a module with no code has no first code segment
constexpr std::uint32_t kAbsoluteName = 0xFFFFFFFEu;  // a public name that is no segment's

class PvDraft final : public draft::Project {
public:
    explicit PvDraft(ProjectPtr p) : p_(std::move(p)), symbols_(p_->symbols()) {}
    std::string toolchain_name() const override { return p_->profile_parts()->name; }
    std::optional<std::string> inverse_model_name() const override { return p_->profile_parts()->inverse; }
    std::shared_ptr<draft::InverseModel> load_inverse_model(std::string_view name) override {
        if (!p_->services().inverse_model) throw UsageError("exact project: no inverse models (Services::inverse_model) were given");
        return p_->services().inverse_model(name);
    }
    std::vector<draft::LibraryPlace> library_places() const override {
        if (!map_) {
            library_ = p_->as_library();
            const x86::Decoder16* d = p_->services().decoder;
            map_ = library::locate(*library_, d ? *d : x86::capstone_decoder());
        }
        std::vector<draft::LibraryPlace> out;
        for (const auto* list : {&map_->coded, &map_->lone})
            for (const library::Placed& placed : *list) {
                draft::LibraryPlace place;
                place.code = static_cast<std::uint32_t>(placed.code.value_or(0));
                place.module = module_of(*placed.module);
                for (const library::Member* m : placed.also) place.also.push_back(module_of(*m));
                out.push_back(std::move(place));
            }
        return out;
    }
    const SymbolTable& symbols() const override { return symbols_; }
    ByteView image() const override { return p_->image(); }
    std::set<std::uint32_t> source_addresses() const override {
        std::set<std::uint32_t> out;
        for (const Source& s : p_->sources())
            if (s.at >= 0) out.insert(static_cast<std::uint32_t>(s.at));
        return out;
    }
    stdfs::path path(std::string_view name) const override { return p_->path(name); }
    std::vector<std::pair<std::uint32_t, std::uint32_t>> functions() const override {
        std::vector<std::pair<std::uint32_t, std::uint32_t>> out;
        for (const CodeFunction& f : p_->functions()) out.emplace_back(f.address, f.length);
        return out;
    }
    draft::ProveOutcome prove_texts(draft::InverseModel& model, const draft::ModelTexts& texts, std::uint32_t address, std::uint32_t length,
                                    const std::map<std::uint32_t, std::string>& names) override {
        if (p_->services().prove_texts) return p_->services().prove_texts(model, texts, address, length, names);
        if (!p_->services().prover_for) throw UsageError("exact project: no prover (Services::prove_texts or prover_for) was given");
        if (!prover_) prover_ = p_->services().prover_for(p_);
        return prover_(model, texts, address, length, names);
    }

private:
    // A library module as the draft stage reads it: its public names as (segment number, offset), the segments numbered
    // in the order the object defines them.
    static draft::LibraryModule module_of(const library::Member& m) {
        const auto id = [&](const std::optional<std::string>& segment) {
            if (!segment) return kAbsoluteName;
            const auto it = std::ranges::find(m.segments, *segment);
            return it == m.segments.end() ? kAbsoluteName : static_cast<std::uint32_t>(it - m.segments.begin());
        };
        draft::LibraryModule out;
        for (const auto& [name, place] : m.pubs) out.publics.insert_or_assign(name, std::pair{id(place.segment), place.offset});
        out.first_code_segment = m.code.empty() ? kNoSegment : id(m.code.front().segment);
        return out;
    }

    ProjectPtr p_;
    SymbolTable symbols_;
    project::ProveTexts prover_;   // (made from Services::prover_for at the first proof)
    mutable std::shared_ptr<const library::Project> library_;   // (what the map points into the project through)
    mutable std::optional<library::Map> map_;                   // (the library stage's answer, kept: finding the modules costs)
};

}  // namespace

std::shared_ptr<const modules::Project> Project::as_modules() { return std::make_shared<PvModulesView>(shared_from_this()); }
std::shared_ptr<const library::Project> Project::as_library() { return std::make_shared<PvLibrary>(shared_from_this()); }
std::shared_ptr<const data::Project> Project::as_data() { return std::make_shared<PvData>(shared_from_this()); }
std::shared_ptr<const ledger::Project> Project::as_ledger() { return std::make_shared<PvLedger>(shared_from_this()); }
std::shared_ptr<const linkwhole::Project> Project::as_linkwhole() { return std::make_shared<PvLinkwhole>(shared_from_this()); }
std::shared_ptr<const readable::Project> Project::as_readable() { return std::make_shared<PvReadable>(shared_from_this()); }
std::shared_ptr<draft::Project> Project::as_draft() { return std::make_shared<PvDraft>(shared_from_this()); }

identify::Services Project::identify_services() {
    if (!services_->toolchains) throw UsageError("exact project: no compilers (Services::toolchains) were given");
    identify::Services s;
    s.locations = services_->toolchains->locations();
    const auto self = shared_from_this();
    s.set_switches = [self](const std::optional<std::string>& switches) { self->set_switches(switches); };
    toolchain::Toolchains* tcs = services_->toolchains;
    s.load = [tcs](const std::string& name, const std::optional<std::string>& variant) -> std::shared_ptr<const ledger::Toolchain> {
        return tcs->load(name, variant);
    };
    return s;
}

pascal_join::Project Project::pascal_project() const {
    if (config_.pascal.compilands.empty()) throw UsageError("exact project: the project has no \"compilands\" (it is not a Pascal program in pieces)");
    const std::shared_ptr<toolchain::Toolchain> tc = toolchain();
    const auto* pascal = dynamic_cast<const pascal_join::Toolchain*>(tc.get());
    if (!pascal) throw UsageError(std::format("exact project: the toolchain {} does not compile Pascal", tc->name()));
    pascal_join::Project q;
    q.dir = dir_;
    q.config = config_.pascal;
    q.image = image_;
    q.toolchain = pascal;
    q.symbols = symbols();
    q.program = program();
    q.equivalents = services_->equivalents;
    q.decoder = services_->decoder;
    return q;
}

}  // namespace cb::exact::project
