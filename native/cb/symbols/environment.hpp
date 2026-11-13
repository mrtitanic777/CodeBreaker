// cb/symbols/environment.hpp -- what the commands of this area (`identify`, `names`) need of the rest of the program.
//
// The corpus (which file is which kind, where the modules are) and the configuration (the state folder holds the knowledge base, the
// checkout holds the authored files). Everything a command opens -- the knowledge base, the corpus's modules -- is opened when the
// command runs, not when it is declared: the program declares every command at start-up.
#pragma once

#include <filesystem>

#include "cb/base/config.hpp"
#include "cb/base/corpus.hpp"

namespace cb::symbols {

struct Environment {
    const base::Corpus* corpus = nullptr;
    const config::Config* config = nullptr;
    // Where `identify --learn` writes the signatures; empty: codebreaker/symbols/runtime_signatures.json of the checkout.
    std::filesystem::path signature_file;
    // Where `names --ordinals` writes the table; empty: codebreaker/symbols/ordinal_names.json of the checkout.
    std::filesystem::path ordinal_file;
    // The knowledge base; empty: the one of the state folder (CB_KNOWLEDGE_DB overrides, as everywhere).
    std::filesystem::path knowledge_file;
};

}  // namespace cb::symbols
