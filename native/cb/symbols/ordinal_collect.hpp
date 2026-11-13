// cb/symbols/ordinal_collect.hpp -- `names --ordinals FILE...`: remake the table of names for numbered exports from the corpus.
//
// ordinals::collect reads definition files and import libraries and keeps the names the corpus can bear out. What it asks of the corpus --
// which numbers a module exports, whether the code of an export takes the arguments a name says, which modules the corpus imports from by
// number -- is answered here over the corpus's modules.
#pragma once

#include <filesystem>
#include <functional>
#include <span>
#include <string_view>

#include "cb/base/corpus.hpp"
#include "cb/decompile/environment.hpp"
#include "cb/symbols/ordinals.hpp"

namespace cb::symbols {

// The table made from `paths` (files or folders; earlier ones win), `log` told of each table used or refused. `modules` is the corpus's modules as
// the ordinal tables read them.
ordinals::ShippedTables remake_ordinal_table(const base::Corpus& corpus, decompile::environment::CorpusModules& modules,
                                             std::span<const std::filesystem::path> paths, const std::function<void(std::string_view)>& log);

}  // namespace cb::symbols
