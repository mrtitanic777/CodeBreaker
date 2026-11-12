// cb/translate/backends/java_libc.hpp -- a C library call as Java: which method (the API map's C rows, api_maps/libc.json) and
// how its arguments travel.
//
// crt.LibC's methods take C's parameters by their Java types (a pointer is an int address, size_t a long), except that a `char *`
// parameter is an Object: a string literal travels as a Java String, so `printf("%d\n", x)` stays `printf("%d\n", x)`; any other
// char pointer is its address. Arguments past a variadic prototype's last parameter go as they are (boxed), and LibC's printf
// reads them as the format says, as C's does. A function pointer given to the library (qsort's comparator) is a Java functional
// value. A row of kind `direct` names the JDK's own method (Math.sqrt); `unsupported` rows and functions with no row are
// refused, with the reason (java::Refused), which gaps the function that called them (or refuses the program, if it is main).
//
//     const apimap::Registry registry = apimap::Registry::load(api_maps_dir);
//     java_libc::LibC library(registry);
//     java::Program p = java::program(units, "Hello", {.library = &library});
#pragma once

#include "cb/java/apimap.hpp"
#include "cb/translate/backends/java.hpp"

namespace cb::translate::backends::java_libc {

namespace jb = cb::translate::backends::java;

// The Java back end's library mapping over an API-mapping registry. The registry must outlive it.
class LibC : public jb::LibraryCalls {
public:
    explicit LibC(const cb::java::apimap::Registry& registry) : registry_(registry) {}

    // `call`: the C call node; `name`: the function it calls; `args`: its arguments (the va builtins' already reduced).
    jb::Ex call(jb::FnWriter& writer, ir::Node& call, std::string_view name, std::span<ir::Node* const> args) const override;

private:
    const cb::java::apimap::Registry& registry_;
};

}  // namespace cb::translate::backends::java_libc
