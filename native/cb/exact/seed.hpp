// cb/exact/seed.hpp -- a program's functions given the sources of the same functions in another version of it.
//
//     codebreaker exact PROJECT seed --from OTHER [--write] [-v]
//
//     std::map<std::int64_t, std::string> files = seed::seeds(*project, *other);   // {this program's address: the file's text}
//
// The functions the names stage pairs with OTHER's (the same code but for its places, or found by its calls) are mostly the same source. For each
// pair whose function OTHER has in a source file, this program gets a file of its own, src/fADDR.c (ADDR this program's place): every top-level
// declaration of OTHER's file (its includes, macros, types, variables -- a function of the file that returns other than an int declared), then the
// function's text. OTHER's address-names in it (fXXXXX, g_XXXX) are made this program's where the pairs say where those are. The ledger then says
// which are this program's bytes; a file that is not is a draft to start from.
//
// A file this project has already is kept. Without --write the files are only counted.
#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/ordered_map.hpp"
#include "cb/exact/project.hpp"

namespace cb::exact::seed {

// A C source file cut into its top-level declarations and its functions.
struct Split {
    std::vector<std::string> decls;                  // the top-level declarations, in the file's order (a remark just before a function is its own)
    OrderedMap<std::string, std::string> funcs;      // function name -> its text (a name met again keeps its place, takes the later text)
    OrderedMap<std::string, std::string> types;      // function name -> the type it returns, for those that return other than an int
};

// `text` has \n line ends. A K&R function (parameter declarations between the head and the brace) is one function.
Split split(std::string_view text);

// The file each pair of the names stage gives this program, by this program's address (ascending). A pair whose function OTHER has no source of is
// left out.
std::map<std::int64_t, std::string> seeds(project::Project& project, project::Project& other);

// `exact PROJECT seed --from OTHER [--write]`: 1 without --from; a relative OTHER is from the working directory. With --write: the headers of
// OTHER's source folders this project has none of are copied, and a src/fADDR.c is written for each seed the project has no file for.
int run(project::Project& project, const std::vector<std::string>& args);

}  // namespace cb::exact::seed
