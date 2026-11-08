// codebreaker/symbols/cpp_classes.hpp -- C++17 port of codebreaker.symbols.cpp_classes.
//
// cpp_classes.py -- the C++ classes of a PowerPC (Xbox 360) program, from Microsoft's run-time type
// information: each class's bases, its virtual function tables, and the class that introduces each
// virtual function.
//
//     python -m codebreaker classes <program.xex or image> [-o classes.json]
//     auto c = cpp_classes::read(image_path);
//     c.classes            {name: Class{bases, vtables ...}}
//     c.functions          {address: (class, "vf3" / "vf3_off8")}: virtual functions, by the class that introduces them
//     c.found              [(class, vtable, slot, address)]: functions no table of starts knew, which the layout requires
//
// A program built with RTTI (dynamic_cast, typeid) keeps, for every polymorphic class:
//
//     TypeDescriptor             { type_info's vftable, 0, ".?AVName@@" }
//     CompleteObjectLocator      { 0, offset, constructor-displacement offset, TypeDescriptor*, ClassHierarchyDescriptor* }
//     ClassHierarchyDescriptor   { 0, attributes, number of bases, BaseClassDescriptor** } (the class first)
//     BaseClassDescriptor        { TypeDescriptor*, contained bases, mdisp, pdisp, vdisp, attributes }
//
// and every virtual function table is preceded by a pointer to its locator. Nothing is guessed: a
// table's slot is taken while it points at a known function start, or where the C++ layout requires
// it -- a derived class's table for a base sub-object holds every slot of that base's table -- and
// those slots are reported as found functions. A function is named by the one class that introduces
// it (every other class whose tables hold it derives from that one); a function several unrelated
// classes share (a compiler-merged body) keeps no class name.
//
// Typing notes:
//   * The Python's dicts with fixed keys are records: BaseClass ({name, contained, mdisp, pdisp, vdisp,
//     attributes}), VTable ({address, offset, for, slots}; `for` is std::nullopt for None) and Class
//     ({name, mangled, bases, vtables}). A VTable's `slots` and a Class's `bases` and `vtables` are
//     py::list handles, shared by the copies of the record, so that the record read from `classes` and
//     the one in `classes` change together as the Python's one dict does.
//   * `img` is the powerpc_image Image the Python duck-types: from_image takes the typed Image.
//   * main(argv): argv is sys.argv[1:], as the Python's; the return value is the exit status.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "codebreaker/formats/powerpc_image.hpp"
#include "py.hpp"

namespace codebreaker::symbols::cpp_classes {

struct BaseClass {
    py::str name;
    int64_t contained = 0, mdisp = 0, pdisp = 0, vdisp = 0, attributes = 0;
};

struct VTable {
    int64_t address = 0;
    int64_t offset = 0;
    std::optional<py::str> for_;   // the base class the table is for ("for" in the Python), None for the class's own
    py::list<int64_t> slots;
};

struct Class {
    py::str name;
    py::str mangled;
    py::list<BaseClass> bases;
    py::list<VTable> vtables;
};

struct Classes {
    py::dict<py::str, Class> classes;
    py::dict<int64_t, std::tuple<py::str, py::str>> functions;      // address -> (class, "vf3" / "vf3_off8")
    py::list<std::tuple<py::str, int64_t, int64_t, int64_t>> found; // (class, vtable address, slot, function address)
};

Classes read(const py::str& path);
Classes from_image(const codebreaker::formats::powerpc_image::Image& img);

int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::symbols::cpp_classes
