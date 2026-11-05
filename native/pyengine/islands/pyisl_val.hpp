// pyisl_val.hpp -- what crosses between a typed island and the dynamic program (pyengine --dyn-typed).
//
// A typed island is a function the typed path proved, compiled from its typed C++ (l2cpp17 over
// native/ported/include/py.hpp) and called from the dynamic build (native/pyrt) by a wrapper with the
// dynamic signature. The two runtimes both call their namespace `py`, so they never meet in one
// translation unit: the typed side is compiled with py renamed (pyt), and the boundary is plain C++ --
// int64_t, double, bool, UTF-8 text in std::string, and for the rest this value. It holds a copy, never
// a reference into either side (an island only takes and returns values no one else can see changed).
#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace pyisl {

struct val {
    enum class K : uint8_t { None, Bool, Int, Float, Str, Bytes, ByteArray, Tuple, List, Dict };
    K k = K::None;
    int64_t i = 0;              // Bool, Int
    double f = 0;               // Float
    std::string s;              // Str (UTF-8), Bytes, ByteArray (the raw bytes)
    std::vector<val> items;     // Tuple, List; Dict: key, value, key, value ... in the dict's order
};

}  // namespace pyisl
