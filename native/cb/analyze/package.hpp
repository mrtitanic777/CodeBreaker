// cb/analyze/package.hpp -- what can be worked out ABOUT a module, beside decompiling it: the area's overview.
//
// The analyses of cb/analyze, each a header and a source file of its own:
//
//     image.hpp       a module as the analyses see it: its bytes at their addresses, its functions decoded by
//                     following the code, its imports (built from the format readers; the decoder is native.hpp)
//     native.hpp      the address space and the decoder an analysis runs on, and what is held at a call
//     call_graph.hpp  who calls whom, references to data and functions, what exports reach
//     conventions.hpp how each function is called, its arguments, frame, result and shape
//     globals.hpp     global variables and initialised tables of pointers
//     strings.hpp     strings, strings built on the stack, format strings and their arguments
//     structures.hpp  structures recovered from how pointers are used
//     classes.hpp     virtual tables, run-time type information, MFC classes and message maps,
//                     exception-handling tables
//     dispatch.hpp    jump tables, compare chains, message and command dispatch, system calls
//     resources.hpp   the resources code asks for
//     observe.hpp     what running the functions shows: fields touched, indirect call targets,
//                     functions that behave as strlen, memcpy, ...
//     facts.hpp       the fact table of the function store
//
// Every analysis takes the image (and what it needs of the analyses before it) and returns a typed report: counts,
// the facts it found, and a few lines for people. A fact is about one function, or about the whole module.
#pragma once

#include <string_view>

namespace cb::analyze {

// The overview above as text (what `codebreaker help analyze` prints).
std::string_view overview();

}  // namespace cb::analyze
