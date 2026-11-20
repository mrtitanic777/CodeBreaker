// store/knowledge_proposals.hpp -- the proposal store (proposals.hpp) over the knowledge base (knowledge/store.hpp).
//
// A proposal is a piece of knowledge: platform "names", kind "proposal", key "<module sha>/<function key>", the value a map of its fields. A
// proposal not yet accepted is Learned; an accepted or rejected one is Given (a person decided it: never dropped). It lives beside, and apart
// from, the knowledge base's body names (`body_name`), which only an accepted proposal reaches.
#pragma once

#include "knowledge/store.hpp"
#include "store/proposals.hpp"

namespace cb::store {

class KnowledgeProposals : public ProposalStore {
public:
    // `store` is not owned and must outlive this object.
    explicit KnowledgeProposals(knowledge::Store& store) : store_(store) {}

    void put(const Proposal& proposal) override;
    std::optional<Proposal> get(std::string_view module, std::string_view key) override;
    std::vector<Proposal> of_module(std::string_view module) override;
    bool set_state(std::string_view module, std::string_view key, ProposalState state) override;

private:
    knowledge::Store& store_;
};

}  // namespace cb::store
