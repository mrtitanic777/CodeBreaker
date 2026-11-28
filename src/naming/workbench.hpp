// naming/workbench.hpp -- the objects `identify` and `names` work over, made when a command runs.
//
//     Workbench bench(environment);
//     bench.identifier().identify(path)          the runtime library's functions of a module
//     bench.namer().of_module(path)              every function and import of a module, named
//
// What each needs of the rest of the program, and where it comes from: the knowledge base of the state folder (the signatures, the
// ordinal tables, the console's import names, the names given to bodies), the corpus's modules (the names their own tables give their
// numbered exports; which modules the corpus imports by number), the headers (how a name is spelled), and the readers of each kind of
// file. Each is opened the first time it is asked for: `names --shared` opens the knowledge base and nothing else.
#pragma once

#include <memory>

#include "knowledge/package.hpp"
#include "knowledge/store.hpp"
#include "store/knowledge_bodies.hpp"
#include "store/knowledge_proposals.hpp"
#include "symbols/environment.hpp"
#include "naming/identify.hpp"
#include "naming/names.hpp"
#include "symbols/ordinals.hpp"
#include "naming/proposals.hpp"

namespace cb::naming {

class Workbench {
public:
    explicit Workbench(const symbols::Environment& environment);
    Workbench(const Workbench&) = delete;
    Workbench& operator=(const Workbench&) = delete;
    ~Workbench();

    const symbols::Environment& environment() const { return environment_; }
    knowledge::Store& store();
    knowledge::Knowledge& knowledge();
    const symbols::ordinals::Ordinals& ordinals();
    identify::Identifier& identifier();
    store::BodyStore& bodies();
    store::ProposalStore& proposal_store();
    proposals::Proposals& proposer();
    names::Namer& namer();

private:
    struct Parts;
    symbols::Environment environment_;
    std::unique_ptr<Parts> parts_;
};

}  // namespace cb::naming
