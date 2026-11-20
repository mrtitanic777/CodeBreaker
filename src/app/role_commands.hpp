// app/role_commands.hpp -- the front door of each role: disassemble, assemble, compile, link, and decompile for any input
// (app/role_commands.cpp says what each takes).
#pragma once

#include <memory>

#include "app/commands.hpp"
#include "base/corpus.hpp"

namespace cb::app::roles {

// Sets disassemble, assemble, compile and link, and puts the any-input route in front of decompile (which must be set already).
// `table` must outlive its commands.
void register_role_commands(CommandTable& table, std::shared_ptr<base::Corpus> corpus);

}  // namespace cb::app::roles
