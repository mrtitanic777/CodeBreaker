// Tests of cb/symbols/cpp_classes on images written out word by word in the layout Microsoft's PowerPC compilers use for
// run-time type information (type descriptors in .data; hierarchy descriptors, locators and virtual tables in .rdata). A real
// Xbox 360 program is too large to keep as test data; the layout here is the one the module's comment describes, and every
// expected answer was worked out by hand from it. The classes: CBase (two virtual functions), CDerived (overrides nothing
// but adds one, and has a slot the function starts do not know), CIface, CMulti (derives CBase and CIface: a second table
// for the CIface sub-object at offset 8), CUnrelated (shares a merged body with CIface).
//
// DIVERGES from the Python module: a byte that is not ASCII in a decorated name becomes U+FFFD as the Python's decode with
// "replace" did, per byte; `functions` is kept sorted by address where the Python kept the order the occurrences were met.
#include "cb/symbols/cpp_classes.hpp"

#include <nlohmann/json.hpp>

#include "cb/core/testing.hpp"

namespace cc = cb::symbols::cpp_classes;
using cb::Bytes;

namespace {

constexpr std::uint32_t kRdata = 0x82000000, kData = 0x82100000, kText = 0x82200000;
constexpr std::uint32_t kTypeInfoVftable = 0x82000004;

// A section of memory written word by word (big-endian), with an allocator.
class Section {
public:
    Section(std::uint32_t base, std::size_t size) : base_(base), bytes_(size, 0) {}
    std::uint32_t alloc(std::size_t words) {
        const std::uint32_t at = base_ + static_cast<std::uint32_t>(used_);
        used_ += 4 * words;
        return at;
    }
    void set(std::uint32_t address, std::uint32_t value) {
        const std::size_t at = address - base_;
        for (int i = 0; i < 4; ++i) bytes_[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(value >> (24 - 8 * i));
    }
    void text(std::uint32_t address, std::string_view s) { std::copy(s.begin(), s.end(), bytes_.begin() + static_cast<std::ptrdiff_t>(address - base_)); }
    cc::Image::Memory memory() const { return {base_, bytes_}; }
    std::uint32_t used() const { return static_cast<std::uint32_t>(used_); }

private:
    std::uint32_t base_;
    Bytes bytes_;
    std::size_t used_ = 0;
};

struct ClassSpec {
    std::string name;                     // "CBase"
    std::uint32_t td = 0;
    std::vector<std::uint32_t> bcd;       // the base class descriptors of the hierarchy, the class itself first
    std::uint32_t chd = 0;
    std::uint32_t locator0 = 0;
};

class Rtti {
public:
    Rtti() : rdata_(kRdata, 0x800), data_(kData, 0x200) {}

    // The type descriptor of a class: type_info's vftable, a zero word, the decorated name.
    std::uint32_t descriptor(const std::string& decorated, std::uint32_t vftable = kTypeInfoVftable) {
        const std::uint32_t td = data_.alloc(8);
        data_.set(td, vftable);
        data_.set(td + 4, 0);
        data_.text(td + 8, decorated);
        return td;
    }
    // A base class descriptor.
    std::uint32_t base(std::uint32_t td, std::uint32_t contained, std::int32_t mdisp, std::int32_t pdisp = -1, std::int32_t vdisp = 0, std::uint32_t attributes = 0) {
        const std::uint32_t bcd = rdata_.alloc(6);
        rdata_.set(bcd, td), rdata_.set(bcd + 4, contained), rdata_.set(bcd + 8, static_cast<std::uint32_t>(mdisp));
        rdata_.set(bcd + 12, static_cast<std::uint32_t>(pdisp)), rdata_.set(bcd + 16, static_cast<std::uint32_t>(vdisp));
        rdata_.set(bcd + 20, attributes);
        return bcd;
    }
    // The class hierarchy descriptor over base descriptors (the class first).
    std::uint32_t hierarchy(const std::vector<std::uint32_t>& bcds, std::uint32_t attributes = 0) {
        const std::uint32_t array = rdata_.alloc(bcds.size());
        for (std::size_t i = 0; i < bcds.size(); ++i) rdata_.set(array + 4 * static_cast<std::uint32_t>(i), bcds[i]);
        const std::uint32_t chd = rdata_.alloc(4);
        rdata_.set(chd, 0), rdata_.set(chd + 4, attributes), rdata_.set(chd + 8, static_cast<std::uint32_t>(bcds.size())), rdata_.set(chd + 12, array);
        return chd;
    }
    // A complete object locator, then the table it heads: returns the address of the first slot.
    std::uint32_t table(std::uint32_t offset, std::uint32_t td, std::uint32_t chd, const std::vector<std::uint32_t>& slots) {
        const std::uint32_t locator = rdata_.alloc(5);
        rdata_.set(locator, 0), rdata_.set(locator + 4, offset), rdata_.set(locator + 8, 0), rdata_.set(locator + 12, td), rdata_.set(locator + 16, chd);
        const std::uint32_t pointer = rdata_.alloc(1 + slots.size());
        rdata_.set(pointer, locator);
        for (std::size_t i = 0; i < slots.size(); ++i) rdata_.set(pointer + 4 + 4 * static_cast<std::uint32_t>(i), slots[i]);
        rdata_.alloc(1);   // a zero word after the table
        return pointer + 4;
    }
    cc::Image image(std::set<std::uint32_t> starts) const {
        cc::Image img;
        img.sections = {{".data", kData, 0x200}, {".rdata", kRdata, 0x800}, {".text", kText, 0x1000}};
        img.code_begin = kText, img.code_end = kText + 0x1000;
        img.function_starts = std::move(starts);
        img.memory = {data_.memory(), rdata_.memory()};
        return img;
    }

private:
    Section rdata_, data_;
};

constexpr std::uint32_t kF1 = kText + 0x10, kF2 = kText + 0x20, kG = kText + 0x30, kX = kText + 0x50, kI1 = kText + 0x60,
                        kM1 = kText + 0x70, kJ = kText + 0x80, kS = kText + 0x90;

struct Program {
    Rtti rtti;
    std::uint32_t base_table = 0, derived_table = 0, iface_table = 0, multi_table0 = 0, multi_table8 = 0, unrelated_table = 0;
    cc::Image image;

    Program() {
        const std::uint32_t stray_td = rtti.descriptor(".?AVStray@@", 0x1111);   // not a type_info descriptor: ignored
        (void)stray_td;
        const std::uint32_t td_base = rtti.descriptor(".?AVCBase@@"), td_derived = rtti.descriptor(".?AVCDerived@@");
        const std::uint32_t td_iface = rtti.descriptor(".?AVCIface@@"), td_multi = rtti.descriptor(".?AVCMulti@@");
        const std::uint32_t td_unrelated = rtti.descriptor(".?AVCUnrelated@@");
        // base class descriptors: (the class itself, contained bases, member displacement)
        const std::uint32_t b_base = rtti.base(td_base, 0, 0), b_iface = rtti.base(td_iface, 0, 0);
        const std::uint32_t b_derived = rtti.base(td_derived, 1, 0), b_multi = rtti.base(td_multi, 2, 0);
        const std::uint32_t b_unrelated = rtti.base(td_unrelated, 0, 0);
        const std::uint32_t b_base_in_derived = rtti.base(td_base, 0, 0), b_base_in_multi = rtti.base(td_base, 0, 0);
        const std::uint32_t b_iface_in_multi = rtti.base(td_iface, 0, 8);
        const std::uint32_t h_base = rtti.hierarchy({b_base}), h_iface = rtti.hierarchy({b_iface});
        const std::uint32_t h_derived = rtti.hierarchy({b_derived, b_base_in_derived});
        const std::uint32_t h_multi = rtti.hierarchy({b_multi, b_base_in_multi, b_iface_in_multi}, 1);
        const std::uint32_t h_unrelated = rtti.hierarchy({b_unrelated});
        base_table = rtti.table(0, td_base, h_base, {kF1, kF2});
        // CDerived: f1 inherited; slot 1 is a function nobody started (the layout requires it); then its own g
        derived_table = rtti.table(0, td_derived, h_derived, {kF1, kX, kG});
        iface_table = rtti.table(0, td_iface, h_iface, {kI1, kS});
        multi_table0 = rtti.table(0, td_multi, h_multi, {kF1, kM1});
        multi_table8 = rtti.table(8, td_multi, h_multi, {kI1, kJ});
        unrelated_table = rtti.table(0, td_unrelated, h_unrelated, {kS});
        image = rtti.image({kF1, kF2, kG, kI1, kM1, kJ, kS});
    }
};

}  // namespace

CB_TEST("symbols.cpp_classes.the_image_reads_words_and_bytes", [] {
    cc::Image img;
    img.memory = {{0x1000, {0x12, 0x34, 0x56, 0x78, 0x9A}}};
    CB_CHECK_EQ(img.word(0x1000).value_or(0), std::uint32_t{0x12345678});
    CB_CHECK_EQ(img.word(0x1001).value_or(0), std::uint32_t{0x3456789A});
    CB_CHECK(!img.word(0x1002).has_value());    // a word must lie wholly in the memory
    CB_CHECK(!img.word(0xFFF).has_value());
    CB_CHECK(!img.word(0x2000).has_value());
    CB_CHECK_EQ(img.bytes_at(0x1001, 100).size(), std::size_t{4});
    CB_CHECK_EQ(img.bytes_at(0x1000, 2).size(), std::size_t{2});
    CB_CHECK(img.bytes_at(0x1005, 2).empty());
});

CB_TEST("symbols.cpp_classes.finds_classes_bases_and_tables", [] {
    const Program p;
    const cc::Classes c = cc::find_classes(p.image);
    CB_CHECK_EQ(c.classes.size(), std::size_t{5});   // Stray has another type_info vftable: it is no class
    CB_CHECK(!c.classes.contains("Stray"));
    const cc::Class& base = c.classes.at("CBase");
    CB_CHECK_EQ(base.mangled, ".?AVCBase@@");
    CB_CHECK(base.bases.empty());
    CB_CHECK_EQ(base.vtables.size(), std::size_t{1});
    CB_CHECK_EQ(base.vtables[0].address, p.base_table);
    CB_CHECK_EQ(base.vtables[0].offset, std::uint32_t{0});
    CB_CHECK(!base.vtables[0].for_base.has_value());
    CB_CHECK((base.vtables[0].slots == std::vector<std::uint32_t>{kF1, kF2}));

    const cc::Class& derived = c.classes.at("CDerived");
    CB_CHECK_EQ(derived.bases.size(), std::size_t{1});
    CB_CHECK_EQ(derived.bases[0].name, "CBase");
    CB_CHECK_EQ(derived.bases[0].pdisp, -1);
    // slot 1 is not a function start, so the table first stops at one slot; the layout rule (CBase's table has two) takes it
    CB_CHECK((derived.vtables[0].slots == std::vector<std::uint32_t>{kF1, kX, kG}));

    const cc::Class& multi = c.classes.at("CMulti");
    CB_CHECK_EQ(multi.bases.size(), std::size_t{2});
    CB_CHECK_EQ(multi.bases[1].name, "CIface");
    CB_CHECK_EQ(multi.bases[1].mdisp, 8);
    CB_CHECK_EQ(multi.vtables.size(), std::size_t{2});
    CB_CHECK_EQ(multi.vtables[0].offset, std::uint32_t{0});     // sorted by offset
    CB_CHECK(!multi.vtables[0].for_base.has_value());
    CB_CHECK_EQ(multi.vtables[1].offset, std::uint32_t{8});
    CB_CHECK_EQ(multi.vtables[1].address, p.multi_table8);
    CB_CHECK(multi.vtables[1].for_base.has_value());
    CB_CHECK_EQ(*multi.vtables[1].for_base, "CIface");
    CB_CHECK((multi.vtables[1].slots == std::vector<std::uint32_t>{kI1, kJ}));
    // the classes are in the order their locators are in the image
    std::vector<std::string> names;
    for (const auto& [name, k] : c.classes) names.push_back(name);
    CB_CHECK((names == std::vector<std::string>{"CBase", "CDerived", "CIface", "CMulti", "CUnrelated"}));
});

CB_TEST("symbols.cpp_classes.finds_functions_the_layout_requires", [] {
    const Program p;
    const cc::Classes c = cc::find_classes(p.image);
    CB_CHECK_EQ(c.found.size(), std::size_t{1});
    CB_CHECK_EQ(c.found[0].class_name, "CDerived");
    CB_CHECK_EQ(c.found[0].vtable, p.derived_table);
    CB_CHECK_EQ(c.found[0].slot, std::size_t{1});
    CB_CHECK_EQ(c.found[0].address, kX);
});

CB_TEST("symbols.cpp_classes.names_each_function_by_the_class_that_introduces_it", [] {
    const Program p;
    const cc::Classes c = cc::find_classes(p.image);
    auto named = [&](std::uint32_t f) { return c.functions.find(f) == c.functions.end() ? std::string("-") : c.functions.at(f).class_name + " " + c.functions.at(f).label; };
    CB_CHECK_EQ(named(kF1), "CBase vf0");          // inherited by CDerived and CMulti: the class every one derives from
    CB_CHECK_EQ(named(kF2), "CBase vf1");
    CB_CHECK_EQ(named(kX), "CDerived vf1");        // found by layout, introduced in slot 1
    CB_CHECK_EQ(named(kG), "CDerived vf2");
    CB_CHECK_EQ(named(kM1), "CMulti vf1");
    CB_CHECK_EQ(named(kI1), "CIface vf0");         // in CIface's own table and CMulti's sub-object table
    CB_CHECK_EQ(named(kJ), "CMulti vf1_off8");     // introduced in the table of a sub-object
    CB_CHECK_EQ(named(kS), "-");                   // a merged body shared by unrelated classes: no class
    CB_CHECK_EQ(c.functions.size(), std::size_t{7});
});

CB_TEST("symbols.cpp_classes.an_image_without_rtti_has_no_classes", [] {
    cc::Image img;
    img.sections = {{".rdata", kRdata, 0x100}, {".text", kText, 0x100}};
    img.memory = {{kRdata, Bytes(0x100, 0)}};
    const cc::Classes c = cc::find_classes(img);
    CB_CHECK(c.classes.empty());
    CB_CHECK(c.functions.empty());
    CB_CHECK(c.found.empty());
    // nothing at all
    CB_CHECK(cc::find_classes(cc::Image{}).classes.empty());
});

CB_TEST("symbols.cpp_classes.a_damaged_hierarchy_is_not_a_class", [] {
    // the type descriptor is there, but its hierarchy descriptor claims 300 bases
    Rtti again;
    const std::uint32_t td2 = again.descriptor(".?AVCBroken@@");
    const std::uint32_t b2 = again.base(td2, 0, 0);
    const std::uint32_t chd2 = again.hierarchy({b2});
    again.table(0, td2, chd2, {kF1});
    cc::Image good = again.image({kF1});
    CB_CHECK_EQ(cc::find_classes(good).classes.size(), std::size_t{1});
    for (auto& m : good.memory) {
        if (m.address == kRdata) {
            const std::size_t at = chd2 + 8 - kRdata;
            m.bytes[at + 2] = 1, m.bytes[at + 3] = 44;
        }
    }
    CB_CHECK(cc::find_classes(good).classes.empty());
});

CB_TEST("symbols.cpp_classes.summary_and_json", [] {
    // one class, two virtual functions
    Rtti rtti;
    const std::uint32_t td = rtti.descriptor(".?AVCBase@@");
    const std::uint32_t chd = rtti.hierarchy({rtti.base(td, 0, 0)});
    const std::uint32_t table = rtti.table(0, td, chd, {kF1, kF2});
    const cc::Classes c = cc::find_classes(rtti.image({kF1, kF2}));
    CB_CHECK_EQ(cc::summary(c, "game.xex"),
                "game.xex: 1 classes, 1 virtual function tables, 2 virtual functions named by class, 0 functions found by layout\n" +
                    std::format("  {:<40} bases: \n", "CBase"));
    const std::string expected = std::format(
        "{{\n \"classes\": [\n  {{\n   \"name\": \"CBase\",\n   \"mangled\": \".?AVCBase@@\",\n   \"bases\": [],\n   \"vtables\": [\n    {{\n"
        "     \"address\": {},\n     \"offset\": 0,\n     \"for\": null,\n     \"slots\": [\n      {},\n      {}\n     ]\n    }}\n   ]\n  }}\n ],\n"
        " \"functions\": {{\n  \"{:08X}\": [\n   \"CBase\",\n   \"vf0\"\n  ],\n  \"{:08X}\": [\n   \"CBase\",\n   \"vf1\"\n  ]\n }},\n \"found\": []\n}}\n",
        table, kF1, kF2, kF1, kF2);
    CB_CHECK_EQ(cc::to_json(c), expected);
    // and it is JSON
    const nlohmann::json parsed = nlohmann::json::parse(cc::to_json(c));
    CB_CHECK_EQ(parsed.at("classes").at(0).at("vtables").at(0).at("slots").size(), std::size_t{2});
    // none
    CB_CHECK_EQ(cc::to_json(cc::Classes{}), "{\n \"classes\": [],\n \"functions\": {},\n \"found\": []\n}\n");
});

CB_TEST("symbols.cpp_classes.summary_lists_the_deepest_classes", [] {
    const Program p;
    const cc::Classes c = cc::find_classes(p.image);
    const std::string s = cc::summary(c, "p.xex");
    CB_CHECK(s.starts_with("p.xex: 5 classes, 6 virtual function tables, 7 virtual functions named by class, 1 functions found by layout\n"));
    // CMulti has two bases, CDerived one, then the rest in the order they were found
    const std::size_t multi = s.find("CMulti"), derived = s.find("CDerived  "), base = s.find("  CBase ");
    CB_CHECK(multi != std::string::npos && derived != std::string::npos && base != std::string::npos);
    CB_CHECK(multi < derived && derived < base);
    CB_CHECK(s.find("bases: CBase, CIface") != std::string::npos);
});
