// cb/translate/frontends/python/methods.hpp -- the types of Python's builtins, the methods of its builtin types and the
// standard library calls the front end maps.
//
//     in::Inference inference(methods::library());       // infer.hpp: the library the interpreter asks
//     program.solve(inference);
//
// A call that is not typed here is refused with its name (stdlib:<name>), so the coverage report lists exactly what is
// missing. How each one is written in C++ is a row of the API map (codebreaker/java/api_maps/python.json), keyed by the
// same name: the `Res` a typed call carries (`BMethod "str.split"`, `Builtin "len"`, `Std "struct.unpack"`) is that key.
//
// What a call is checked for: the kinds of its arguments (an argument of the wrong kind is a `conflict` naming the method
// and the kind), keywords (only the few a call takes; any other is `syntax`), and an argument that is a lambda or a
// generator where the call does not take one (`dynamic`). A method that changes a container (`list.append`, `dict.update`,
// `set.add`) grows what the program knows of the container's site, so the types of its items follow what is put in.
#pragma once

#include "cb/translate/frontends/python/infer.hpp"

namespace cb::translate::frontends::python::methods {

// The builtins (len, range, sorted, ...), the methods of str, bytes, bytearray, list, dict, set and int, and the calls of
// os.path, struct, zlib/binascii crc32, bisect and math the front end maps. Stateless; valid for the life of the program.
const infer::Library& library();

}  // namespace cb::translate::frontends::python::methods
