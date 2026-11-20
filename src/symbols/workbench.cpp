// symbols/workbench.cpp -- see workbench.hpp.
#include "symbols/workbench.hpp"

#include <set>

#include "build/watcom.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/text.hpp"
#include "decompile/environment.hpp"
#include "formats/powerpc_image.hpp"
#include "formats/xex.hpp"
#include "source/module_writer.hpp"
#include "symbols/ordinal_collect.hpp"
#include "symbols/prototypes.hpp"

namespace cb::symbols {

namespace ppc = formats::powerpc_image;

struct Workbench::Parts {
    std::unique_ptr<knowledge::Store> store;
    std::unique_ptr<knowledge::Knowledge> knowledge;
    std::shared_ptr<decompile::environment::CorpusModules> modules;
    std::unique_ptr<ordinals::Ordinals> ordinals;
    std::unique_ptr<identify::Identifier> identifier;
    std::unique_ptr<store::KnowledgeBodies> bodies;
    std::unique_ptr<store::KnowledgeProposals> proposals;
    std::unique_ptr<proposals::Proposals> proposer;
    std::unique_ptr<names::Namer> namer;
};

Workbench::Workbench(const Environment& environment) : environment_(environment), parts_(std::make_unique<Parts>()) {
    if (environment_.corpus == nullptr || environment_.config == nullptr) throw UsageError("the symbols commands need a corpus and a configuration");
}
Workbench::~Workbench() = default;

knowledge::Store& Workbench::store() {
    if (!parts_->store) {
        std::filesystem::path file = environment_.knowledge_file;
        if (file.empty()) file = knowledge::Store::default_path(environment_.config->state());
        parts_->store = std::make_unique<knowledge::Store>(file, knowledge::DataFolders::under(environment_.config->checkout_root()));
    }
    return *parts_->store;
}

knowledge::Knowledge& Workbench::knowledge() {
    if (!parts_->knowledge) {
        std::filesystem::path reference;
        try {
            reference = environment_.corpus->reference();
        } catch (const Error&) {   // (no reference folder: the headers of the compiler alone spell the names)
        }
        const std::filesystem::path openwatcom = build::watcom::OpenWatcom::from_settings(environment_.config->text("openwatcom")).root();
        parts_->knowledge = std::make_unique<knowledge::Knowledge>(store(), prototypes::HeaderFolders::standard(openwatcom, reference));
    }
    return *parts_->knowledge;
}

const ordinals::Ordinals& Workbench::ordinals() {
    if (!parts_->ordinals) {
        if (!parts_->modules) parts_->modules = std::make_shared<decompile::environment::CorpusModules>(*environment_.corpus);
        knowledge::Knowledge& kb = knowledge();
        parts_->ordinals = std::make_unique<ordinals::Ordinals>(*parts_->modules, decompile::environment::shipped_tables(kb),
                                                                [&kb](std::string_view name) { return kb.spelling(name); });
    }
    return *parts_->ordinals;
}

identify::Identifier& Workbench::identifier() {
    if (!parts_->identifier)
        parts_->identifier = std::make_unique<identify::Identifier>(identify::Identifier::Sources{*environment_.corpus, knowledge(), ordinals()});
    return *parts_->identifier;
}

store::BodyStore& Workbench::bodies() {
    if (!parts_->bodies) parts_->bodies = std::make_unique<store::KnowledgeBodies>(store());
    return *parts_->bodies;
}

store::ProposalStore& Workbench::proposal_store() {
    if (!parts_->proposals) parts_->proposals = std::make_unique<store::KnowledgeProposals>(store());
    return *parts_->proposals;
}

proposals::Proposals& Workbench::proposer() {
    if (!parts_->proposer) {
        proposals::Proposals::Services services{namer(), proposal_store(), *environment_.corpus, environment_.evidence, environment_.decompile};
        if (!services.evidence) services.evidence = [](const std::filesystem::path&) -> std::vector<FunctionEvidence> { throw UsageError("the facts that name a function are not available here"); };
        parts_->proposer = std::make_unique<proposals::Proposals>(std::move(services));
    }
    return *parts_->proposer;
}

namespace {

// What a PowerPC image says that names or finds functions.
names::PowerPcNames wb_powerpc_names(const std::filesystem::path& module) {
    const ppc::Image image = ppc::Image::read(module);
    names::PowerPcNames out;
    if (image.entry() != 0) out.entry = image.entry();
    out.functions.assign(image.functions().begin(), image.functions().end());
    for (const ppc::VirtualMethod& m : ppc::virtual_methods(image)) out.methods.push_back({m.address, m.class_name, m.slot});
    return out;
}

// {import symbol: name} for a PowerPC image's imports: the libraries it imports from are in the .xex header when the image came from one,
// else the kernel and the system library, in the order every image seen has them; the numbers of the console's system libraries are the base's.
std::map<std::string, std::string> wb_console_import_names(knowledge::Knowledge& kb, const std::filesystem::path& module) {
    const knowledge::Rows table = kb.table("xbox360", "ordinal_names");
    const std::vector<std::string> libraries =
        formats::xex::import_libraries(module).value_or(std::vector<std::string>{"xboxkrnl.exe", "xam.xex"});
    std::vector<std::map<std::string, std::string>> numbers;
    for (const std::string& library : libraries) {
        std::map<std::string, std::string> names;
        if (const knowledge::Value* entry = table.find(text::lower(library)); entry != nullptr && entry->map() != nullptr)
            for (const auto& [number, name] : *entry->map())
                if (name.text() != nullptr) names[number] = *name.text();
        numbers.push_back(std::move(names));
    }
    std::set<std::string> distinct;
    const ppc::Image image = ppc::Image::read(module);   // (named: its imports are walked below)
    for (const auto& [address, symbol] : image.imports()) distinct.insert(symbol);
    const std::vector<std::string> symbols(distinct.begin(), distinct.end());
    return source::module_writer::console_import_names(symbols, numbers, {});
}

}  // namespace

names::Namer& Workbench::namer() {
    if (!parts_->namer) {
        // (each of the services is made when the Namer first asks: opening the base, the corpus's modules and the headers costs, and a
        // module with nothing to name by them does not pay)
        names::Namer::Services services{*environment_.corpus, ordinals(), &bodies()};
        services.powerpc = [](const std::filesystem::path& module) -> std::optional<names::PowerPcNames> { return wb_powerpc_names(module); };
        services.console_import_names = [this](const std::filesystem::path& module) { return wb_console_import_names(knowledge(), module); };
        services.runtime_library = [this](const std::filesystem::path& module) { return identifier().identify(module); };
        services.compiler = [this](const std::filesystem::path& module) { return identifier().compiler(module); };
        services.collect_ordinals = [this](std::span<const std::filesystem::path> files) {
            const ordinals::ShippedTables tables = remake_ordinal_table(*environment_.corpus, *parts_->modules, files, [](std::string_view line) { cb::out("{}\n", line); });
            std::filesystem::path file = environment_.ordinal_file;
            if (file.empty()) file = environment_.config->checkout_root() / "config" / "symbols" / "ordinal_names.json";
            const std::string text = ordinals::format_shipped(tables);
            fs::write_file(file, ByteView(reinterpret_cast<const std::uint8_t*>(text.data()), text.size()));
        };
        services.proposals = &proposal_store();
        services.proposals_command = [this](const cli::Args& args) { return proposer().execute(args); };
        parts_->namer = std::make_unique<names::Namer>(std::move(services));
    }
    return *parts_->namer;
}

}  // namespace cb::symbols
