// symbols/cpp_classes_x64.hpp -- the C++ classes of an x64 Windows program, from Visual C++'s run-time type information: each
// class's name, its bases, and its virtual function tables in slot order.
//
//     const cpp_classes_x64::Classes c = cpp_classes_x64::find_classes(image);
//     c.classes                  {decorated name: Class{name, bases, vtables ...}} in the order their locators are in the image
//     c.slot_functions           every function a table holds, sorted (a leaf function has no .pdata entry: this is how it is found)
//
// x64's records hold image-relative addresses (RVAs) where 32-bit ones hold pointers:
//
//     TypeDescriptor             { type_info's vftable (a pointer), 0, ".?AVName@@" }
//     CompleteObjectLocator      { signature 1, offset, constructor-displacement offset, TypeDescriptor, ClassHierarchyDescriptor, itself }
//     ClassHierarchyDescriptor   { 0, attributes (1 multiple, 2 virtual inheritance), number of bases, BaseClassArray }
//     BaseClassArray             BaseClassDescriptor RVAs, the class itself first, then its bases in pre-order
//     BaseClassDescriptor        { TypeDescriptor, contained bases, mdisp, pdisp, vdisp, attributes, ClassHierarchyDescriptor }
//
// and every virtual function table is preceded by a pointer to its locator. A locator is taken only where its signature is 1 and
// its own RVA is itself, its type descriptor's name is a decorated class name, and its hierarchy descriptor names the same class
// first; a table's slots are the pointers after it that point into code. Nothing is guessed.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "core/ordered_map.hpp"
#include "formats/pe.hpp"

namespace cb::symbols::cpp_classes_x64 {

// A base class as the hierarchy descriptor lists it.
struct BaseClass {
    std::string name;            // as C++ writes it ("Shape", "std::exception")
    std::string decorated;       // ".?AVShape@@"
    std::uint32_t contained = 0; // how many bases of its own follow it in the array
    std::int32_t mdisp = 0;      // where its sub-object is in the class's
    std::int32_t pdisp = -1;     // the vbtable's place (-1: not a virtual base)
    std::int32_t vdisp = 0;      // the place inside the vbtable
    std::uint32_t attributes = 0;
    bool direct = false;         // a base the class names itself (not a base of a base)
    bool is_virtual() const { return pdisp >= 0; }
};

struct VTable {
    std::uint64_t address = 0;           // of the first slot
    std::uint64_t locator = 0;           // of its complete object locator
    std::uint32_t offset = 0;            // of the sub-object it is for
    std::vector<std::uint64_t> slots;    // the functions, in slot order
};

struct Class {
    std::string name;                    // demangled; the decorated name when it cannot be read
    std::string decorated;
    std::uint32_t attributes = 0;        // the hierarchy descriptor's: 1 multiple inheritance, 2 virtual inheritance
    std::vector<BaseClass> bases;        // every base, the class itself not included, in the array's order
    std::vector<VTable> vtables;         // by sub-object offset, then address

    // Its own table (the one for offset 0), if it has one.
    const VTable* primary() const;
    // The direct base whose sub-object starts the class's (mdisp 0, not virtual): the one whose table its primary table extends.
    const BaseClass* primary_base() const;
};

struct Classes {
    OrderedMap<std::string, Class> classes;   // by decorated name
    std::vector<std::uint64_t> slot_functions; // sorted, unique

    // The class whose primary table is at `vtable` (its first slot's address).
    const Class* by_vtable(std::uint64_t vtable) const;
};

// Read the classes an x64 image's run-time type information holds. Empty for an image that is not x64 or has none.
Classes find_classes(const formats::pe::Image& image);

}  // namespace cb::symbols::cpp_classes_x64
