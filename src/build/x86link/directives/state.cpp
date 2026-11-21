// build/x86link/directives/state.cpp -- see state.hpp.
#include "build/x86link/directives/state.hpp"

#include "build/x86link/directives/common.hpp"

namespace cb::x86link::directives {

namespace {
// The link being run. The in-process host runs one link at a time (build/x86link/x86link.cpp holds a lock).
std::unique_ptr<State> the_state;
}  // namespace

State& state() {
    if (!the_state) the_state = std::make_unique<State>();
    return *the_state;
}

void begin_link(Link& link) {
    the_state = std::make_unique<State>();
    the_state->reader.set_link(link);
}
void end_link() { the_state.reset(); }

Reader& reader() { return state().reader; }
Flag& cmd_flags() { return state().flags; }
char*& exe_name() { return state().exe_name; }

}  // namespace cb::x86link::directives

namespace dx = cb::x86link::directives;

const char* DirectiveSourceName() {
    const dx::Source* s = dx::reader().current();
    return s == nullptr || s->name().empty() ? nullptr : s->name().c_str();
}

DirectiveSourceKindValue DirectiveSourceKind() {
    const dx::Source* s = dx::reader().current();
    if (s == nullptr) return DIRECTIVE_SOURCE_OTHER;
    if (s->kind() == dx::SourceKind::system) return DIRECTIVE_SOURCE_SYSTEM;
    if (s->kind() == dx::SourceKind::environment) return DIRECTIVE_SOURCE_ENVIRONMENT;
    return DIRECTIVE_SOURCE_OTHER;
}

unsigned DirectiveLine() { return dx::reader().line(); }

bool DirectivesHaveRealBreak() { return dx::any(dx::cmd_flags() & dx::Flag::have_realbreak); }
