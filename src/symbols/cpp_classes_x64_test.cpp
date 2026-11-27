// Tests of cb/symbols/cpp_classes_x64 on testdata/x64_classes/shapes.exe, a small C++ program built from shapes.cpp beside it with
// Visual C++ x64 (cl /O2 /GR /EHsc /MD): Shape (abstract), Rect : Shape, Square : Rect, Circle : Shape, Named, and Label : Rect, Named
// (a second table for the Named sub-object). Every expected answer is read off the source: a table has the class's virtual
// functions in declaration order, a base's first, the destructor where it is declared.
#include "symbols/cpp_classes_x64.hpp"

#include <algorithm>

#include "core/testing.hpp"

namespace cx = cb::symbols::cpp_classes_x64;

namespace {

cx::Classes cx64_shapes() {
    return cx::find_classes(cb::formats::pe::Image::open(cb::testing::data_path("symbols/testdata/x64_classes/shapes.exe")));
}

const cx::Class& cx64_named(const cx::Classes& c, const std::string& decorated) {
    const cx::Class* k = c.classes.find(decorated);
    CB_CHECK(k != nullptr);
    return *k;
}

}  // namespace

CB_TEST("symbols.cpp_classes_x64.the_classes_and_their_names_come_from_the_type_information", [] {
    const cx::Classes c = cx64_shapes();
    for (const char* d : {".?AVShape@@", ".?AVRect@@", ".?AVSquare@@", ".?AVCircle@@", ".?AVLabel@@"}) CB_CHECK(c.classes.contains(d));
    // Named is never made alone (Label's constructor inlines its own and drops the store of Named's table): it has no table, no
    // locator, and is known only as Label's base
    CB_CHECK(!c.classes.contains(".?AVNamed@@"));
    CB_CHECK_EQ(cx64_named(c, ".?AVSquare@@").name, std::string("Square"));
});

CB_TEST("symbols.cpp_classes_x64.each_table_has_its_virtual_functions_in_order", [] {
    const cx::Classes c = cx64_shapes();
    // Shape: ~Shape, area, sides, grow; Rect adds perimeter; Square adds set; Circle adds nothing
    CB_CHECK_EQ(cx64_named(c, ".?AVShape@@").primary()->slots.size(), std::size_t{4});
    CB_CHECK_EQ(cx64_named(c, ".?AVRect@@").primary()->slots.size(), std::size_t{5});
    CB_CHECK_EQ(cx64_named(c, ".?AVSquare@@").primary()->slots.size(), std::size_t{6});
    CB_CHECK_EQ(cx64_named(c, ".?AVCircle@@").primary()->slots.size(), std::size_t{4});
    const cx::VTable& rect = *cx64_named(c, ".?AVRect@@").primary();
    const cx::VTable& square = *cx64_named(c, ".?AVSquare@@").primary();
    // Square overrides perimeter (slot 4) and inherits area and sides (slots 1, 2) from Rect
    CB_CHECK(square.slots[1] == rect.slots[1]);
    CB_CHECK(square.slots[2] == rect.slots[2]);
    CB_CHECK(square.slots[4] != rect.slots[4]);
    // Circle overrides grow; Rect does not
    CB_CHECK(cx64_named(c, ".?AVCircle@@").primary()->slots[3] != cx64_named(c, ".?AVShape@@").primary()->slots[3]);
    CB_CHECK(rect.slots[3] == cx64_named(c, ".?AVShape@@").primary()->slots[3]);
    // every slot function is reported, sorted
    CB_CHECK(std::is_sorted(c.slot_functions.begin(), c.slot_functions.end()));
    for (const std::uint64_t f : square.slots) CB_CHECK(std::binary_search(c.slot_functions.begin(), c.slot_functions.end(), f));
});

CB_TEST("symbols.cpp_classes_x64.bases_and_the_second_table_of_multiple_inheritance", [] {
    const cx::Classes c = cx64_shapes();
    const cx::Class& label = cx64_named(c, ".?AVLabel@@");
    CB_CHECK_EQ(label.attributes & 1u, 1u);   // multiple inheritance
    std::vector<std::string> direct;
    for (const cx::BaseClass& b : label.bases)
        if (b.direct) direct.push_back(b.name);
    CB_CHECK(direct == (std::vector<std::string>{"Rect", "Named"}));
    CB_CHECK_EQ(label.bases.size(), std::size_t{3});   // Rect, Shape (inside Rect), Named
    CB_CHECK_EQ(label.primary_base()->name, std::string("Rect"));
    CB_CHECK_EQ(label.vtables.size(), std::size_t{2});
    CB_CHECK_EQ(label.vtables[1].offset, 0x20u);       // Rect's members end at 0x20: Named's sub-object
    CB_CHECK_EQ(label.vtables[1].slots.size(), std::size_t{2});
    const cx::Class* by_table = c.by_vtable(cx64_named(c, ".?AVRect@@").primary()->address);
    CB_CHECK(by_table != nullptr && by_table->name == "Rect");
    CB_CHECK(cx64_named(c, ".?AVShape@@").primary_base() == nullptr);
});

CB_TEST("symbols.cpp_classes_x64.an_image_without_type_information_has_no_classes", [] {
    const cx::Classes c = cx::find_classes(cb::formats::pe::Image::open(cb::testing::data_path("formats/testdata/lz32.dll")));
    CB_CHECK(c.classes.empty());
    CB_CHECK(c.slot_functions.empty());
});
