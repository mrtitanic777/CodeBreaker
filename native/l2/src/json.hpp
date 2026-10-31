// json.hpp -- JSON read and written as Python's json module does (json.loads / json.dumps with its
// defaults: ", " and ": ", ensure_ascii, a float as its repr, NaN and Infinity), and the tagged form
// the shim hands Python values over in (a tuple {"$t": [...]}, a set {"$s": [...]}, a dict whose keys
// are not all strings {"$d": [[k, v], ...]}, a float that is not finite {"$f": "nan"}).
#pragma once
#include <string>

#include "value.hpp"

namespace py {

V json_parse(const std::string& text, bool tagged = false);   // ValueError on bad JSON
struct DumpOpts {
    const char* item_sep = ", ";
    const char* key_sep = ": ";
    bool ensure_ascii = true;
    bool tagged = false;          // tuples, sets, other keys in the tagged form (else as lists, str keys)
};
std::string json_dump(const V& v, const DumpOpts& o = DumpOpts());
inline std::string json_tagged(const V& v) {
    DumpOpts o;
    o.item_sep = ",";
    o.key_sep = ":";
    o.ensure_ascii = false;
    o.tagged = true;
    return json_dump(v, o);
}

}  // namespace py
