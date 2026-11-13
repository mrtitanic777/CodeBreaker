#include "cb/store/versions_command.hpp"

#include <optional>

#include "cb/compare/versions.hpp"
#include "cb/engine/function_store.hpp"
#include "cb/engine/lane_kit.hpp"
#include "cb/knowledge/package.hpp"
#include "cb/store/knowledge_bodies.hpp"
#include "cb/store/versions_ledger.hpp"
#include "cb/verify/reconstruct.hpp"

namespace cb::store::versions_command {

namespace {

// Everything the ledger stands on, made in the order it is needed and let go in reverse.
class Surroundings {
public:
    Surroundings(const Environment& environment, bool writable)
        : kit_(environment.config, environment.corpus),
          store_(environment.config.state("functions.sqlite"), environment.config.state("db"), !writable) {
        kit_.add_lanes(kinds_);
        reconstructor_.emplace(kit_.reconstruct_services(kinds_, store_));
        bodies_.emplace(kit_.knowledge().store());
    }

    base::Corpus& corpus() { return kit_.corpus(); }

    VersionsLedger ledger() {
        return VersionsLedger({.corpus = kit_.corpus(),
                               .store = store_,
                               .identity = verify::reconstruct::identity_hook<compare::versions::Ledger::Identity>(*reconstructor_),
                               .bodies = &*bodies_});
    }

private:
    engine::LaneKit kit_;
    verify::reconstruct::Kinds kinds_;
    engine::FunctionStore store_;
    std::optional<verify::reconstruct::Reconstructor> reconstructor_;
    std::optional<KnowledgeBodies> bodies_;
};

}  // namespace

cli::Command command(const Environment& environment) {
    cli::Command declared = compare::versions::declaration();
    declared.handler = [environment](const cli::Args& args) {
        Surroundings world(environment, args.flag("carry"));
        VersionsLedger ledger = world.ledger();
        return compare::versions::run(args, world.corpus(), ledger);
    };
    return declared;
}

}  // namespace cb::store::versions_command
