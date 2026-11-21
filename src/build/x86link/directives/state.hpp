// build/x86link/directives/state.hpp -- what reading a link's directives keeps, one per link (no globals): the reader,
// the flags the directives set, and what the directives' handlers carry from one directive to the next. Made new at each
// link's start (begin_link), so nothing an earlier link in the process read stays.
#pragma once

#include <memory>

#include "build/x86link/directives/language.hpp"
#include "build/x86link/directives/reader.hpp"
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/sysblock.h"

namespace cb::x86link::directives {

// What the common directives carry from one to the next (common.cpp; wlink's cmdall.c statics).
struct CommonState {
    struct {
        union {
            file_list* file;
            member_list* member;
        } u;
    } last_file{};                          // the last object file (or member) a `file` directive added
    file_list** last_lib_file = nullptr;    // where the next `libfile` goes
    ORDER_CLASS* current_order_class = nullptr;
    ORDER_SEGMENT* current_order_segment = nullptr;
};

struct State {
    Reader reader;
    Flag flags = Flag::none;
    file_list** current_file_list = nullptr;   // where the next `file` goes (the section being read)
    char* exe_name = nullptr;                  // `name` (wlink's memory: the rest of the link frees it)
    sysblock* sys_blocks = nullptr;            // the system blocks defined (wlink.lnk's and the script's)
    sysblock* link_commands = nullptr;         // the links after this one (`startlink`/`endlink`)
    sysblock* prev_command = nullptr;          // the link command being run (freed when the next is taken)
    CommonState common;
    byte ovl_level = 0;                        // (DOS) how deep in `begin` ... `end` the overlays are
};

State& state();
void begin_link(Link& link);   // a new State for the link starting
void end_link();     // the link's State done

}  // namespace cb::x86link::directives

// ---- for wlink's modules not rewritten yet (C names, as they were)

// The location a message names: the directive source being read (none: nullptr), its kind and line (msg.cpp).
const char* DirectiveSourceName();
enum DirectiveSourceKindValue { DIRECTIVE_SOURCE_OTHER, DIRECTIVE_SOURCE_SYSTEM, DIRECTIVE_SOURCE_ENVIRONMENT };
DirectiveSourceKindValue DirectiveSourceKind();   // a system block or an environment variable: no line is named
unsigned DirectiveLine();
bool DirectivesHaveRealBreak();  // `runtime realbreak` was given (loadphar.cpp)
