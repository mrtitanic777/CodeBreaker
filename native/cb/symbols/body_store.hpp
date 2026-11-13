// cb/symbols/body_store.hpp -- store::BodyStore over the knowledge base (knowledge::Store).
//
// symbols/names records a module's bodies and reads the names other modules give them through the interface store::BodyStore; the
// knowledge base keeps them (its body and body_name tables). This is the thin joint between the two, kept here until the store
// area carries one of its own: drop it for that one and nothing else changes.
#pragma once

#include <map>
#include <string>
#include <string_view>

#include "cb/knowledge/store.hpp"
#include "cb/store/bodies.hpp"

namespace cb::symbols {

class KnowledgeBodies final : public store::BodyStore {
public:
    // `knowledge` outlives this.
    explicit KnowledgeBodies(knowledge::Store& knowledge) : knowledge_(knowledge) {}

    void set_bodies(const store::ModuleRef& module, const std::map<std::string, store::Body>& by_key) override;
    std::map<std::string, store::Body> bodies(std::string_view module) override;
    void name_body(std::string_view digest, std::string_view name, std::string_view source = "named by hand") override;
    std::map<std::string, store::SharedName> names_from_bodies(std::string_view module) override;
    // The knowledge base counts bodies in several modules; the functions that are such copies it does not say, so they are counted here
    // from the base's own file (read-only, on a connection of their own).
    store::BodyCounts shared_bodies() override;

private:
    knowledge::Store& knowledge_;
};

}  // namespace cb::symbols
