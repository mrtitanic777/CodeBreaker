// cb/symbols/workbench.hpp -- the objects `identify` and `names` work over, made when a command runs.
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

#include "cb/knowledge/package.hpp"
#include "cb/knowledge/store.hpp"
#include "cb/store/knowledge_bodies.hpp"
#include "cb/symbols/environment.hpp"
#include "cb/symbols/identify.hpp"
#include "cb/symbols/names.hpp"
#include "cb/symbols/ordinals.hpp"

namespace cb::symbols {

class Workbench {
public:
    explicit Workbench(const Environment& environment);
    Workbench(const Workbench&) = delete;
    Workbench& operator=(const Workbench&) = delete;
    ~Workbench();

    const Environment& environment() const { return environment_; }
    knowledge::Store& store();
    knowledge::Knowledge& knowledge();
    const ordinals::Ordinals& ordinals();
    identify::Identifier& identifier();
    store::BodyStore& bodies();
    names::Namer& namer();

private:
    struct Parts;
    Environment environment_;
    std::unique_ptr<Parts> parts_;
};

}  // namespace cb::symbols
