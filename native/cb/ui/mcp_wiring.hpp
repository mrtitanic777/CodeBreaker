// cb/ui/mcp_wiring.hpp -- what standard_services() fills in beyond the folders and the inspector: the parts of the program the MCP tools
// read, made when a tool first asks (the symbols' workbench, the decompiler's surroundings), each filling the Services it answers.
//
//     auto context = std::make_shared<wiring::Context>(config, corpus);
//     wiring::fill_code(services, context);       // mcp_code.cpp:    disasm, xrefs, locate           (a module as the analyses see it)
//     wiring::fill_work(services, context);       // mcp_work.cpp:    decompile_function, compare, diff
//     wiring::fill_symbols(services, context);    // mcp_symbols.cpp: names, identified, ordinal_name, set_name, the command table, the
//                                                 //                  ledger, the source tree, what a file is
//
// The Services' functions are called from the server's worker threads, one slow answer at a time per key; every part that is not
// thread-safe is used under the context's lock.
#pragma once

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>

#include "cb/base/config.hpp"
#include "cb/base/corpus.hpp"
#include "cb/decompile/environment.hpp"
#include "cb/engine/lane_kit.hpp"
#include "cb/symbols/workbench.hpp"
#include "cb/ui/mcp_server.hpp"

namespace cb::ui::mcp::wiring {

// The verifier settings in force now per kind of module (the ledger's `verifier_profiles`), worked out when first asked.
std::function<std::map<std::string, std::optional<std::string>>()> verifier_profiles(const config::Config& config);

class Context {
public:
    Context(const config::Config& config, const base::Corpus& corpus);

    const config::Config& config() const { return *config_; }
    const base::Corpus& corpus() const { return *corpus_; }
    // The symbols' workbench: the names of a module, the runtime libraries it holds, the ordinals' names, the body store.
    symbols::Workbench& bench();
    // The corpus's modules as the symbol readers ask for them (the names a 16-bit module's own tables give its exports).
    decompile::environment::CorpusModules& modules();
    // The decompiler and what it reads (its own knowledge base, cache and surroundings).
    engine::LaneKit& kit();
    // The verifier settings in force now (one kit for all the ledger's readings).
    const std::function<std::map<std::string, std::optional<std::string>>()>& profiles() const { return profiles_; }
    // Held while a part that is not thread-safe is used (the workbench, the images).
    std::mutex& lock() { return lock_; }

private:
    const config::Config* config_;
    const base::Corpus* corpus_;
    std::mutex lock_, make_;
    std::unique_ptr<symbols::Workbench> bench_;
    std::unique_ptr<engine::LaneKit> kit_;
    std::unique_ptr<decompile::environment::CorpusModules> modules_;
    std::function<std::map<std::string, std::optional<std::string>>()> profiles_;
};

void fill_code(Services& services, const std::shared_ptr<Context>& context);
void fill_work(Services& services, const std::shared_ptr<Context>& context);
void fill_symbols(Services& services, const std::shared_ptr<Context>& context);

}  // namespace cb::ui::mcp::wiring
