// decompile/environment.hpp -- what the decompiler asks of the rest of CodeBreaker, answered by cb/ modules only.
//
//     auto environment = CorpusEnvironment::over_knowledge(corpus, library, knowledge);
//     Context context;  context.environment = environment.get();  ...  decompiler::Decompiler decompiler(context);
//
// `decompiler::Environment` (decompiler.hpp) is the interface: the L2 library, the symbol tables describing a module reads (what an API
// removes from the stack, the names of numbered imports, a driver's services, the conventions the analysis finds), the structures
// recovered for a module, and the PowerPC reader. This is the implementation that needs nothing old:
//
//   * the sizes of arguments: symbols/api_arguments (the headers' table and the exporting module's own code, found in the corpus), the
//     memories behind it the knowledge base's;
//   * the names of imports by number: symbols/ordinals, its tables the base's, the corpus's exports its own;
//   * a driver's service names: symbols/driver_services over the base's rows;
//   * the imports of a module by name: build/watcom's import maps;
//   * conventions and structures: analyze/ (the image, its call graph, the calling-convention pass, structure recovery);
//   * a PowerPC image: formats/powerpc_image, its names from symbols/names.
//
// What a part is made from can be given (`Sources`): a caller that keeps its own memories (the base, its tables) hands them over;
// `over_knowledge` makes them from one `knowledge::Knowledge`.
#pragma once

#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>

#include "base/corpus.hpp"
#include "decompile/decompiler.hpp"
#include "formats/ne.hpp"
#include "formats/pe.hpp"
#include "knowledge/package.hpp"
#include "symbols/api_arguments.hpp"
#include "symbols/driver_services.hpp"
#include "symbols/ordinals.hpp"
#include "translate/native_l2.hpp"

namespace cb::decompile::environment {

// The key prefix the 16-bit argument sizes of a product are kept under: the product, and the way the sizes are read (a product's
// folder can be pointed elsewhere: Setup's MINI.CAB has other USER and KRNL386 files than Windows' own, and the sizes read from
// one must not be handed to the other). Ends with `|`: `knowledge::Knowledge::win16_memory(win16_scope(corpus))`.
std::string win16_scope(const base::Corpus& corpus);

// The modules of the corpus as the symbol readers ask for them. One object answers all three questions, over the one set of modules
// it has opened: which PE module exports what (the stack scan), how an export of a 16-bit module returns, and what names a module's
// own tables give its numbered exports. Safe to use from several threads.
class CorpusModules final : public symbols::api_arguments::ModuleSource,
                            public symbols::api_arguments::Win16Exports,
                            public symbols::ordinals::ModuleExports {
public:
    explicit CorpusModules(const base::Corpus& corpus);
    ~CorpusModules() override;

    // ModuleSource: the PE module with this stem ("USER32"), or null when the corpus has none that reads as one.
    std::shared_ptr<const symbols::api_arguments::ExportingModule> find(std::string_view stem) override;
    // Win16Exports: how export `ordinal` of the 16-bit module `module` (capitals) returns.
    std::optional<Found> find(std::string_view module, std::uint32_t ordinal) override;
    // ModuleExports: the names the module's own tables give its exports. A 16-bit module is found by its file (KERNEL is krnl386.exe;
    // DISPLAY is vga.drv); a 32-bit one by its file; a 16-bit one that is called by the name it gives itself, last (the corpus's
    // 16-bit modules are then all opened, once).
    std::optional<Table> table(std::string_view stem) const override;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

// What the environment is made from. Everything given outlives it.
struct Sources {
    const base::Corpus& corpus;
    const translate::native_l2::Library& library;
    symbols::api_arguments::Win32Memory& win32;   // the headers' sizes, and what scans of exports found
    symbols::api_arguments::Win16Memory& win16;   // the sizes found for 16-bit exports (under win16_scope)
    const symbols::ordinals::Ordinals& ordinals;
    const symbols::driver_services::Table* drivers = nullptr;   // none: a service is `vxd_<device>_<service>`
    // The modules the sizes and names are read from (the ordinals were made over them, too); none: the environment opens its own.
    std::shared_ptr<CorpusModules> modules;
    knowledge::Knowledge* knowledge = nullptr;   // the headers' prototypes (a 64-bit module's imports are typed from them); none: untyped
};

class CorpusEnvironment : public decompiler::Environment {
public:
    explicit CorpusEnvironment(Sources sources);
    ~CorpusEnvironment() override;

    // An environment over the knowledge base: its memories of argument sizes, the ordinals' tables, the driver kit's services.
    static std::unique_ptr<CorpusEnvironment> over_knowledge(const base::Corpus& corpus, const translate::native_l2::Library& library,
                                                             knowledge::Knowledge& knowledge);

    // ---- decompiler::Environment
    describe::Knowledge& knowledge() override;
    const translate::native_l2::Library& library() override;
    std::map<std::string, std::string> import_dlls(const std::filesystem::path& module) override;
    std::optional<std::uint32_t> argument_bytes(std::string_view dll, std::string_view name) override;
    std::optional<std::vector<struct_text::Structure>> recover_structures(const std::filesystem::path& module) override;
    describe::PowerPcFacts powerpc_facts(const std::filesystem::path& module) override;
    std::vector<decompiler::Va> powerpc_functions(const std::filesystem::path& module) override;
    // A PE module has been described: handed on to `on_described` when one is set (whoever wants to know what the description said of it).
    void described(const std::filesystem::path& module, const describe::PeDescription& description) override;
    std::function<void(const std::filesystem::path&, const describe::PeDescription&)> on_described;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

// The ordinals' tables of the base ("windows"/"ordinal_names"), as symbols/ordinals reads them.
symbols::ordinals::ShippedTables shipped_tables(knowledge::Knowledge& knowledge);
// The driver kit's services of the base ("vxd": device, service_table, driver, message).
symbols::driver_services::Table driver_table(knowledge::Knowledge& knowledge);

}  // namespace cb::decompile::environment
