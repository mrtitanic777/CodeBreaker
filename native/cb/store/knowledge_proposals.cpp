// cb/store/knowledge_proposals.cpp -- see knowledge_proposals.hpp.
#include "cb/store/knowledge_proposals.hpp"

#include <utility>

namespace cb::store {

namespace {

constexpr std::string_view kPlatform = "names";
constexpr std::string_view kKind = "proposal";

std::string piece_key(std::string_view module, std::string_view key) {
    std::string out(module);
    out += '/';
    out += key;
    return out;
}

std::string text_of(const knowledge::Value& map, std::string_view field) {
    const knowledge::Value* v = map.get(field);
    const std::string* t = v != nullptr ? v->text() : nullptr;
    return t != nullptr ? *t : std::string();
}

knowledge::Value encode(const Proposal& p) {
    knowledge::Value::Map map;
    map.insert_or_assign("module", p.module);
    map.insert_or_assign("key", p.key);
    map.insert_or_assign("name", p.name);
    map.insert_or_assign("description", p.description);
    map.insert_or_assign("source", p.source);
    map.insert_or_assign("rule", p.rule);
    map.insert_or_assign("confidence", p.confidence);
    map.insert_or_assign("evidence", p.evidence);
    map.insert_or_assign("digest", p.digest ? knowledge::Value(*p.digest) : knowledge::Value());
    map.insert_or_assign("state", std::string(state_name(p.state)));
    return knowledge::Value(std::move(map));
}

std::optional<Proposal> decode(const knowledge::Value& v) {
    if (v.map() == nullptr) return std::nullopt;
    Proposal p;
    p.module = text_of(v, "module");
    p.key = text_of(v, "key");
    p.name = text_of(v, "name");
    p.description = text_of(v, "description");
    p.source = text_of(v, "source");
    p.rule = text_of(v, "rule");
    if (const knowledge::Value* c = v.get("confidence"); c != nullptr && c->integer()) p.confidence = static_cast<int>(*c->integer());
    p.evidence = text_of(v, "evidence");
    if (const knowledge::Value* d = v.get("digest"); d != nullptr && d->text() != nullptr) p.digest = *d->text();
    p.state = parse_state(text_of(v, "state")).value_or(ProposalState::Proposed);
    return p;
}

knowledge::Origin origin_of(ProposalState state) { return state == ProposalState::Proposed ? knowledge::Origin::Learned : knowledge::Origin::Given; }

}  // namespace

std::string_view state_name(ProposalState state) {
    switch (state) {
    case ProposalState::Proposed: return "proposed";
    case ProposalState::Accepted: return "accepted";
    case ProposalState::Rejected: return "rejected";
    }
    return "proposed";
}

std::optional<ProposalState> parse_state(std::string_view text) {
    for (const ProposalState s : {ProposalState::Proposed, ProposalState::Accepted, ProposalState::Rejected})
        if (state_name(s) == text) return s;
    return std::nullopt;
}

void KnowledgeProposals::put(const Proposal& proposal) {
    store_.put(kPlatform, kKind, piece_key(proposal.module, proposal.key), encode(proposal), proposal.rule, origin_of(proposal.state));
}

std::optional<Proposal> KnowledgeProposals::get(std::string_view module, std::string_view key) {
    const std::optional<knowledge::Value> v = store_.get(kPlatform, kKind, piece_key(module, key));
    return v ? decode(*v) : std::nullopt;
}

std::vector<Proposal> KnowledgeProposals::of_module(std::string_view module) {
    std::vector<Proposal> out;
    const std::string prefix = std::string(module) + "/";
    for (const auto& [key, value] : store_.table(kPlatform, kKind))
        if (key.starts_with(prefix))
            if (std::optional<Proposal> p = decode(value)) out.push_back(std::move(*p));
    return out;
}

bool KnowledgeProposals::set_state(std::string_view module, std::string_view key, ProposalState state) {
    std::optional<Proposal> p = get(module, key);
    if (!p) return false;
    p->state = state;
    if (state == ProposalState::Accepted) p->source = "accepted proposal (" + p->rule + ")";
    put(*p);
    return true;
}

}  // namespace cb::store
