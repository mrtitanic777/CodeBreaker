// symbols/cpp_classes.hpp -- the C++ classes of a program built by Visual C++, from its run-time type information: each class's
// name, its bases, its virtual function tables in slot order, and the class that introduces each virtual function. One reading of
// the records for every target that has them; what differs is how a record refers to another and where the reader looks.
//
//     const cpp_classes::Classes c = cpp_classes::find_classes(image);         a PowerPC (Xbox 360) program: 32-bit pointers, big-endian
//     const cpp_classes::x64::Classes c = cpp_classes::x64::find_classes(pe);  an x64 Windows program: image-relative addresses (RVAs)
//     c.classes             {key: Class{name, bases, vtables ...}} in the order their locators are in the image
//     c.functions           {address: {class, "vf3" / "vf3_off8"}}: virtual functions, by the class that introduces them
//     c.slot_functions      every function a table holds, sorted (an x64 leaf function has no .pdata entry: this is how it is found)
//     c.found               (PowerPC) functions no table of starts knew, which the layout requires
//
// A program built with RTTI (dynamic_cast, typeid) keeps, for every polymorphic class (a "ref" is a pointer in a 32-bit program, an
// RVA in an x64 one):
//
//     TypeDescriptor             { type_info's vftable (a pointer), 0, ".?AVName@@" }
//     CompleteObjectLocator      { signature (0; x64: 1), offset, constructor-displacement offset, TypeDescriptor ref, ClassHierarchyDescriptor ref
//                                  (x64: and its own RVA) }
//     ClassHierarchyDescriptor   { 0, attributes (1 multiple, 2 virtual inheritance), number of bases, BaseClassArray ref }
//     BaseClassArray             BaseClassDescriptor refs, the class itself first, then its bases in pre-order
//     BaseClassDescriptor        { TypeDescriptor ref, contained bases, mdisp, pdisp, vdisp, attributes }
//
// and every virtual function table is preceded by a pointer to its locator. Nothing is guessed. The PowerPC reader finds the type
// descriptors by their names (the vftable most of them share is type_info's), takes a table's slot while it points at a known
// function start or where the C++ layout requires it -- a derived class's table for a base sub-object holds every slot of that base's
// table -- and reports those slots as found functions. The x64 reader takes a locator only where its signature is 1 and its own RVA is
// itself, its type descriptor's name is a decorated class name, and its hierarchy descriptor names the same class first; a table's
// slots are the pointers after it that point into code. Either way a function is named by the one class that introduces it (every
// other class whose tables hold it derives from that one); a function several unrelated classes share (a compiler-merged body) keeps
// no class name.
//
// The PowerPC image is given as `Image`, what this reads of one (cb/formats/powerpc_image makes it; a .xex is read through its
// basefile), so this module does not depend on that reader.
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "core/ordered_map.hpp"
#include "formats/pe.hpp"

namespace cb::symbols::cpp_classes {

// ---- the classes, whatever the target ------------------------------------------------------------------------------------

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

// `Address` is the target's: 32 bits for a PowerPC program, 64 for an x64 one.
template <class Address>
struct BasicVTable {
    Address address = 0;                       // of the first slot
    Address locator = 0;                       // of its complete object locator
    std::uint32_t offset = 0;                  // of the sub-object it is for, in the object
    std::vector<Address> slots;                // the functions, in slot order
    std::optional<std::string> for_base;       // the base class the table is for; empty for the class's own tables
};

template <class Address>
struct BasicClass {
    std::string name;                          // as written in C++ (demangled), or the decorated name when it cannot be read
    std::string decorated;                     // ".?AVName@@"
    std::uint32_t attributes = 0;              // the hierarchy descriptor's: 1 multiple inheritance, 2 virtual inheritance
    std::vector<BaseClass> bases;              // every base, the class itself not included, in the array's order
    std::vector<BasicVTable<Address>> vtables; // by sub-object offset, then address

    // Its own table (the one for offset 0), if it has one.
    const BasicVTable<Address>* primary() const {
        for (const BasicVTable<Address>& t : vtables)
            if (t.offset == 0) return &t;
        return nullptr;
    }
    // The direct base whose sub-object starts the class's (mdisp 0, not virtual): the one whose table its primary table extends.
    const BaseClass* primary_base() const {
        for (const BaseClass& b : bases)
            if (b.direct && b.mdisp == 0 && !b.is_virtual()) return &b;
        return nullptr;
    }
};

// A virtual function named by the class that introduces it.
struct VirtualFunction {
    std::string class_name;
    std::string label;                         // "vf3" (slot 3 of the class's own table), "vf3_off8" (of the sub-object at 8)
    friend bool operator==(const VirtualFunction&, const VirtualFunction&) = default;
};

// A function the layout requires that no start table knew.
template <class Address>
struct BasicFoundFunction {
    std::string class_name;
    Address vtable = 0;
    std::size_t slot = 0;
    Address address = 0;
};

template <class Address>
struct BasicClasses {
    // In the order their locators are in the image. Keyed by the C++ name in a PowerPC program, by the decorated name in an x64 one.
    OrderedMap<std::string, BasicClass<Address>> classes;
    std::map<Address, VirtualFunction> functions;
    std::vector<Address> slot_functions;                 // sorted, unique
    std::vector<BasicFoundFunction<Address>> found;

    // The class whose primary table is at `vtable` (its first slot's address).
    const BasicClass<Address>* by_vtable(Address vtable) const {
        for (const auto& [key, c] : classes)
            if (const BasicVTable<Address>* t = c.primary(); t != nullptr && t->address == vtable) return &c;
        return nullptr;
    }
};

// The summary the `classes` command prints for a program: the counts, and the eight classes with the most bases.
template <class Address>
std::string summary(const BasicClasses<Address>& classes, std::string_view file_name);

// The classes as the JSON document `classes -o` writes (classes by name, functions by address, those found).
template <class Address>
std::string to_json(const BasicClasses<Address>& classes);

// ---- a PowerPC (Xbox 360) program ---------------------------------------------------------------------------------------

using VTable = BasicVTable<std::uint32_t>;
using Class = BasicClass<std::uint32_t>;
using FoundFunction = BasicFoundFunction<std::uint32_t>;
using Classes = BasicClasses<std::uint32_t>;

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

// Read the classes an image's run-time type information holds. An image with no type descriptors has none.
Classes find_classes(const Image& image);

// ---- an x64 Windows program -------------------------------------------------------------------------------------------

namespace x64 {

using VTable = BasicVTable<std::uint64_t>;
using Class = BasicClass<std::uint64_t>;
using Classes = BasicClasses<std::uint64_t>;
using cpp_classes::BaseClass;

// Read the classes an x64 image's run-time type information holds. Empty for an image that is not x64 or has none.
Classes find_classes(const formats::pe::Image& image);

}  // namespace x64

}  // namespace cb::symbols::cpp_classes
