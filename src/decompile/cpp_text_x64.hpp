// decompile/cpp_text_x64.hpp -- an x64 C++ program's decompiled functions as the classes they were written in: the classes its
// run-time type information names (symbols/cpp_classes), declared with their virtual functions in table order so the layout
// is the original's, their data members where the methods' field accesses through `this` show them, and each function the
// evidence makes a method written as that class's member.
//
//     const Recovered r = recover({&classes, &names, &by_va, &texts, structs});
//     r.declarations        the classes, as C++ (after the C header: they use its types and structures)
//     r.methods             {the function's C name: Method{class, member, ...}}
//     r.texts               {the function's C name: its definition as a member}
//
// A function is a method when:
//   - it is in a slot of a class's own table (offset 0) where the C++ layout declares it: a slot the class adds, or one it
//     overrides (a function its base's table does not hold there) -- `virtual`, named `vf<slot>`;
//   - it stores the address of one class's own table into its first argument's first word (a constructor or destructor: the
//     last store is the most derived class's in a constructor, the first in a destructor) -- named `ctor_<rva>` / `dtor_<rva>`.
// A function several unrelated classes' tables hold (a body the linker merged), one a table for a sub-object at another offset
// holds (its `this` is that sub-object's), or one that is the runtime library's stays a function.
//
// Every slot of a virtual function takes one signature, the one of the class that introduces it, so an override overrides: an
// integer or pointer argument is `uint8` (a register's whole width), a floating-point one the decompiler's float type; each
// definition starts by giving its own parameters their own types. A method whose first parameter is the decompiler's structure
// of its fields reads those fields as the class's members (`this->f_18`); the first word, the table pointer, is `*(T *)this`.
// The classes are packed, each member at the offset the code reads it at; a field a derived class reads below the end of its
// base's members is the base's.
//
// A method keeps its C name as an alternate name for the linker, so C that calls it by that name still links to it.
// Library classes (std::, type_info, Concurrency::, WRL, WIL, ATL, every template instance) are not declared: they are the libraries'.
#pragma once

#include <cstdint>
#include <map>
#include <span>
#include <string>

#include "core/ordered_map.hpp"
#include "core16_types.hpp"
#include "symbols/cpp_classes.hpp"

namespace cb::decompile::cpp_text_x64 {

using Va = std::int64_t;

struct Input {
    const symbols::cpp_classes::x64::Classes* classes = nullptr;
    const std::map<Va, std::string>* names = nullptr;           // every function the module names, by address (its own and the library's)
    const std::map<Va, std::string>* by_va = nullptr;           // the functions there is C for, by address
    const OrderedMap<std::string, std::string>* texts = nullptr; // their C, by name
    std::span<const core16::StructNote> structs;                // the structures the decompiler recovered
};

struct Method {
    std::string cls;              // the class as C++ names it ("Shape", "ns::Widget")
    std::string member;           // "vf2", "ctor_001080"
    std::string qualified;        // "Shape::vf2"
    Va va = 0;
    bool is_virtual = false;
    std::size_t slot = 0;         // of a virtual function
};

struct Recovered {
    std::string declarations;                      // empty: no class to declare
    std::map<std::string, Method> methods;         // by the function's C name
    std::map<std::string, std::string> texts;      // by the function's C name
    std::size_t classes = 0;                       // declared
    std::size_t virtuals = 0;                      // virtual functions declared (overrides included)
    std::size_t fields = 0;                        // data members declared
};

Recovered recover(const Input& in);

}  // namespace cb::decompile::cpp_text_x64
