#include "cb/exact/omf.hpp"

#include <type_traits>

#include "cb/core/testing.hpp"

// exact::omf is formats::omf, not a copy of it.
static_assert(std::is_same_v<cb::exact::omf::Object, cb::formats::omf::Object>);

CB_TEST("exact.omf.is_the_formats_reader", [] {
    // a minimal object written through the exact name reads back through the formats name
    const cb::Bytes code{0x90, 0xC3};
    const cb::Bytes obj = cb::exact::omf::raw_object("t", code, {{"_f", 0}});
    const cb::formats::omf::Object o = cb::formats::omf::read(obj);
    CB_CHECK_EQ(o.publics.size(), std::size_t{1});
    CB_CHECK_EQ(o.publics[0].name, "_f");
});
