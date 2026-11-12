// cb/symbols/cpp_classes.hpp -- the C++ classes of a PowerPC (Xbox 360) program, from Microsoft's run-time type information:
// each class's bases, its virtual function tables, and the class that introduces each virtual function.
//
//     const cpp_classes::Classes c = cpp_classes::find_classes(image);
//     c.classes             {name: Class{bases, vtables ...}} in the order they were found
//     c.functions           {address: {class, "vf3" / "vf3_off8"}}: virtual functions, by the class that introduces them
//     c.found               functions no table of starts knew, which the layout requires
//
// A program built with RTTI (dynamic_cast, typeid) keeps, for every polymorphic class:
//
//     TypeDescriptor             { type_info's vftable, 0, ".?AVName@@" }
//     CompleteObjectLocator      { 0, offset, constructor-displacement offset, TypeDescriptor*, ClassHierarchyDescriptor* }
//     ClassHierarchyDescriptor   { 0, attributes, number of bases, BaseClassDescriptor** } (the class first)
//     BaseClassDescriptor        { TypeDescriptor*, contained bases, mdisp, pdisp, vdisp, attributes }
//
// and every virtual function table is preceded by a pointer to its locator. Nothing is guessed: a table's slot is taken
// while it points at a known function start, or where the C++ layout requires it -- a derived class's table for a base
// sub-object holds every slot of that base's table -- and those slots are reported as found functions. A function is named
// by the one class that introduces it (every other class whose tables hold it derives from that one); a function several
// unrelated classes share (a compiler-merged body) keeps no class name.
//
// The image is given as `Image`, what this reads of one (cb/formats/powerpc_image makes it; a .xex is read through its
// basefile), so this module does not depend on the reader.
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/ordered_map.hpp"

namespace cb::symbols::cpp_classes {

// What the search reads of a PowerPC image: where its sections are, which functions it states, and the memory of its data
// sections (big-endian words).
struct Image {
    struct Section {
        std::string name;                 // as the section header says, without its trailing zeros
        std::uint32_t address = 0;
        std::uint32_t size = 0;
    };
    struct Memory {
        std::uint32_t address = 0;
        Bytes bytes;
    };
    std::vector<Section> sections;
    // [first address, last address + 1) of the code: used when no section is named .text
    std::uint32_t code_begin = 0, code_end = 0;
    // The function starts the image states (.pdata's, and the targets of calls).
    std::set<std::uint32_t> function_starts;
    // The bytes of the sections the search reads (.data and .rdata); the words and bytes outside them read as nothing.
    std::vector<Memory> memory;

    // The big-endian word at `address`; nullopt where no memory holds it.
    std::optional<std::uint32_t> word(std::uint32_t address) const;
    // Up to `n` bytes from `address`, to the end of the memory that holds it.
    ByteView bytes_at(std::uint32_t address, std::uint32_t n) const;
};

// A base class as the class hierarchy descriptor lists it.
struct BaseClass {
    std::string name;
    std::uint32_t contained = 0;     // how many bases of its own it contains
    std::int32_t mdisp = 0;          // member displacement
    std::int32_t pdisp = 0;          // vbtable displacement (-1: not a virtual base)
    std::int32_t vdisp = 0;          // displacement inside the vbtable
    std::uint32_t attributes = 0;
};

struct VTable {
    std::uint32_t address = 0;                 // of the first slot
    std::uint32_t offset = 0;                  // of the sub-object it is for, in the object
    std::optional<std::string> for_base;       // the base class the table is for; empty for the class's own tables
    std::vector<std::uint32_t> slots;          // function addresses
};

struct Class {
    std::string name;                          // as written in C++ (demangled), or the decorated name when it cannot be read
    std::string mangled;                       // ".?AVName@@"
    std::vector<BaseClass> bases;              // the class itself not included
    std::vector<VTable> vtables;               // by offset, then address
};

// A virtual function named by the class that introduces it.
struct VirtualFunction {
    std::string class_name;
    std::string label;                         // "vf3" (slot 3 of the class's own table), "vf3_off8" (of the sub-object at 8)
    friend bool operator==(const VirtualFunction&, const VirtualFunction&) = default;
};

// A function the layout requires that no start table knew.
struct FoundFunction {
    std::string class_name;
    std::uint32_t vtable = 0;
    std::size_t slot = 0;
    std::uint32_t address = 0;
};

struct Classes {
    OrderedMap<std::string, Class> classes;               // in the order their locators are in the image
    std::map<std::uint32_t, VirtualFunction> functions;
    std::vector<FoundFunction> found;
};

// Read the classes an image's run-time type information holds. An image with no type descriptors has none.
Classes find_classes(const Image& image);

// The summary the `classes` command prints for a program: the counts, and the eight classes with the most bases.
std::string summary(const Classes& classes, std::string_view file_name);

// The classes as the JSON document `classes -o` writes (classes by name, functions by address, those found).
std::string to_json(const Classes& classes);

}  // namespace cb::symbols::cpp_classes
