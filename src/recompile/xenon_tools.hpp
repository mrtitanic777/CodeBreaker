// recompile/xenon_tools.hpp -- the three commands the Xbox 360 projects of other tools run to prepare a title for XenonRecomp:
//
//     xex-basic <file.xex> <out.xex>                    the unencrypted, basic-compressed form XenonRecomp reads
//     xenon-config <basic.xex> <out.toml> [--out-dir D] [--switch-tables F] [--file-path F]
//                                                       the XenonRecomp configuration of a module (config.hpp's `make`)
//     switch-tables <basic.xex> <out.toml>              its switch tables: the finder's (switches.hpp), with XenonAnalyse's added for
//                                                       the `bctr` the finder leaves unresolved (without them such a switch is an
//                                                       indirect call into the middle of its function)
//
// They are what the Dashboard's game-preparation scripts and the like run as commands of the program. Each writes its output whole or not at
// all (to `<out>.part`, then renamed), its folder made. The state folder comes from the settings,
// XenonRecomp's own tools from the same Xenon the other recompile commands use: `switch-tables` builds XenonAnalyse the first time (src/recompile/xenonrecomp)
// and runs it into `<out>.analysed.toml`; where it cannot be built or run, the note
//
//     NOTE XenonAnalyse did not run (<why>); switch tables from the finder alone
//
// goes to stderr and the tables are the finder's.
#pragma once

#include <vector>

#include "core/cli.hpp"
#include "recompile/command.hpp"

namespace cb::recompile::xenon_tools {

using Environment = cb::recompile::command::Environment;

cli::Command xex_basic_command(const Environment& environment);
cli::Command xenon_config_command(const Environment& environment);
cli::Command switch_tables_command(const Environment& environment);

// The three, in the order above.
std::vector<cli::Command> commands(const Environment& environment);

}  // namespace cb::recompile::xenon_tools
