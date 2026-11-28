// store/knowledge_bodies.hpp -- the body store (bodies.hpp) over the knowledge base (knowledge/store.hpp).
//
//     knowledge::Store kb(knowledge::Store::default_path(state), knowledge::DataFolders::from_environment());
//     store::KnowledgeBodies bodies(kb);
//     naming::names::Namer::Services services{.bodies = &bodies};     // the `names` command's shared names
//
// The knowledge base keeps a row per body per module (`body`) and a name given to a body (`body_name`); this is the interface the name
// readers use, over them. A module's path is not kept (the knowledge base knows a module by its hash, its name and its kind).
#pragma once

#include "knowledge/store.hpp"
#include "store/bodies.hpp"

namespace cb::store {

class KnowledgeBodies : public BodyStore {
public:
    // `store` is not owned and must outlive this object.
    explicit KnowledgeBodies(knowledge::Store& store) : store_(store) {}

    void set_bodies(const ModuleRef& module, const std::map<std::string, Body>& by_key) override;
    std::map<std::string, Body> bodies(std::string_view module) override;
    void name_body(std::string_view digest, std::string_view name, std::string_view source = "named by hand") override;
    std::map<std::string, SharedName> names_from_bodies(std::string_view module) override;
    BodyCounts shared_bodies() override;

private:
    knowledge::Store& store_;
};

}  // namespace cb::store
