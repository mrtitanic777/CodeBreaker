// store/proposals.hpp -- proposed names: what a rule or an assistant suggests for a function, kept apart from the names people accepted.
//
// A proposal is (module, function key) -> a name, a one-line description, where it came from, how sure it is, and the evidence it rests on.
// It is not a name: nothing that reads names (the `names` listing, the source writer, the version matcher) sees it until it is accepted. A person
// (or a rule that is certain) accepts it, and then the name goes where every name goes -- on the function's body, when it has one that tells it
// apart (store/bodies.hpp), so every module with that body has it -- and the record stays, as the provenance and the description. A function too
// small to have a body keeps the accepted name in its record, in its own module. A rejected proposal is kept too, so a rule does not propose
// it again.
//
// The knowledge base keeps them (store/knowledge_proposals.hpp); tests use a memory one.
#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cb::store {

enum class ProposalState { Proposed, Accepted, Rejected };

std::string_view state_name(ProposalState state);
std::optional<ProposalState> parse_state(std::string_view text);

struct Proposal {
    std::string module;                        // the module's content hash
    std::string key;                           // the function store's key for the address ("1:04a2", "bfea1123")
    std::string name;
    std::string description;                   // one line: what the function does
    std::string source = "proposal";           // "proposal" for everything not yet accepted
    std::string rule;                          // who proposed it: "import-wrapper", "string-reference", "assistant"
    int confidence = 0;                        // percent, 0 to 100
    std::string evidence;                      // what it rests on, one line ("jumps to KERNEL32!CloseHandle")
    std::optional<std::string> digest;         // the function's body, when it has one that tells it apart
    ProposalState state = ProposalState::Proposed;
};

class ProposalStore {
public:
    virtual ~ProposalStore() = default;

    // Record a proposal for the function (replacing any the function has).
    virtual void put(const Proposal& proposal) = 0;
    virtual std::optional<Proposal> get(std::string_view module, std::string_view key) = 0;
    // Every proposal of a module, by key.
    virtual std::vector<Proposal> of_module(std::string_view module) = 0;
    // Move a proposal to another state; false when there is none.
    virtual bool set_state(std::string_view module, std::string_view key, ProposalState state) = 0;
};

}  // namespace cb::store
