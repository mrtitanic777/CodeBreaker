// app/corpus_executables.hpp -- the `corpus-executables` command: every executable of the selected product, its format and its path.
//
//     corpus-executables          one line per executable, <format> <tab> <path>
//
// The executables are corpus.hpp's: every file of the selected product that is an executable whatever it is called (fonts, drivers, overlays),
// every copy of it. They are listed by file name (case ignored), the paths of one name in path order. The format is detect.hpp's name for the file
// ("pe", "ne", "le", "mz", ...); "?" when it names none. The corpus is the application's (its products from the settings).
#pragma once

#include <memory>

#include "base/corpus.hpp"
#include "core/cli.hpp"

namespace cb::app::corpus_executables {

struct Environment {
    std::shared_ptr<base::Corpus> corpus;
};

cli::Command command(const Environment& environment);

}  // namespace cb::app::corpus_executables
