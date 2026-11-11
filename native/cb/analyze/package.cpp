#include "cb/analyze/package.hpp"

namespace cb::analyze {

std::string_view overview() {
    return R"(What can be worked out ABOUT a module, beside decompiling it.

    image          a module as the analyses see it: its bytes at their addresses, its functions
                   decoded by following the code, its imports
    native         the address space and the decoder an analysis runs on, and what is held at a call
    call_graph     who calls whom, references to data and functions, what exports reach
    conventions    how each function is called, its arguments, frame, result and shape
    globals        global variables and initialised tables of pointers
    strings        strings, strings built on the stack, format strings and their arguments
    structures     structures recovered from how pointers are used
    classes        virtual tables, run-time type information, MFC classes and message maps,
                   exception-handling tables
    dispatch       jump tables, compare chains, message and command dispatch, system calls
    resources      the resources code asks for
    observe        what running the functions shows: fields touched, indirect call targets,
                   functions that behave as strlen, memcpy, ...
    facts          the fact table of the function store

Every analysis takes the image (and what it needs of the analyses before it) and returns a typed
report: counts, the facts it found, and a few lines for people. A fact is about one function, or
about the whole module.
)";
}

}  // namespace cb::analyze
