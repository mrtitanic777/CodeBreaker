// verify/sweep.hpp -- reconstruct and judge every module of the corpus, smallest first, resumably.
//
//     codebreaker sweep                      every 32-bit module not yet measured
//     codebreaker sweep --only a.dll,b.dll   these
//     codebreaker sweep --only @list.txt     one module name per line
//     codebreaker sweep --16                 the 16-bit modules; --drivers the drivers;
//                                            --dos real-mode programs; --all every kind
//     codebreaker sweep --status             what has been measured; no work
//     codebreaker sweep --again              measure modules again even if already done
//
// A module counts as measured when every one of its functions has a result under the current verifier settings, so an
// interrupted sweep picks up at the module it stopped in, and changing the verifier makes the whole corpus due again
// without anyone having to say so. One module that fails outright is reported and the sweep moves on.
//
// The command's pieces for the command table: `params()` declares the options, `options_of` reads them, `run` does the
// sweep over what `Services` hands it (the reconstructor, the store, which modules there are of a kind, whether the
// build tools are set up).
#pragma once

#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "core/cli.hpp"
#include "store/function_store.hpp"
#include "verify/reconstruct.hpp"

namespace cb::verify::sweep {

// What a sweep was asked for.
struct Options {
    std::vector<std::string> kinds = {"pe"};          // which kinds of module, in the order swept: "pe", "ne", "le", "dos"
    std::optional<std::set<std::string>> only;        // lower-cased file names; nullopt: every module of the kinds
    bool status_only = false;                         // --status: report, do no work
    bool again = false;                               // --again: measure modules again even if already done
};

// The command's options (for cli::Command::params) and what they say. UsageError for a --only list that cannot be read.
std::vector<cli::Param> params();
Options options_of(const cli::Args& args);
// The names of a --only value: a comma-separated list, or `@file` with one name per line; lower-cased, no blanks.
std::set<std::string> wanted_names(std::string_view only);

struct Services {
    const reconstruct::Reconstructor& reconstruct;
    store::FunctionStore& store;
    // The modules of a kind, in the order to sweep them (smallest first): "pe", "ne", "le" (drivers), "dos" (programs).
    std::function<std::vector<std::filesystem::path>(std::string_view kind)> modules;
    // Why nothing could be built here (OpenWatcom is not set up), or nullopt when the tools are there.
    std::function<std::optional<std::string>()> tools_missing = {};
    std::function<void(std::string_view)> out;        // a line of the report (stdout)
    std::function<void(std::string_view)> err;       // a line of trouble (stderr)
};

// What has been measured of `paths` under the settings in force for each one's kind: {path: module hash} of those with
// results, in the order of `paths`. One line of the totals goes to `services.out`.
std::vector<std::pair<std::filesystem::path, std::string>> status(const Services& services, const std::vector<std::filesystem::path>& paths);

// The sweep: 0 when every module went through, 1 when one failed outright (or nothing could be built).
int run(const Options& options, const Services& services);

}  // namespace cb::verify::sweep
