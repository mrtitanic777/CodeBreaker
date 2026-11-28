// Tests of cb/decompile/cpp_text_x64 on two classes written out as the RTTI reader gives them and C as the x64 decompiler writes it:
// Base (a destructor, a pure virtual function, one more) and Derived : Base (overrides the first two, inherits the third, adds a
// fourth), and a constructor of Derived that stores Base's table and then its own.
#include "decompile/cpp_text_x64.hpp"

#include "core/testing.hpp"

namespace cx = cb::symbols::cpp_classes::x64;
namespace ct = cb::decompile::cpp_text_x64;

namespace {

bool cpp64_has(const std::string& s, std::string_view part) { return s.find(part) != std::string::npos; }

struct Cpp64Fixture {
    cx::Classes classes;
    std::map<ct::Va, std::string> names, by_va;
    cb::OrderedMap<std::string, std::string> texts;
    std::vector<cb::core16::StructNote> structs;

    Cpp64Fixture() {
        cx::Class base;
        base.name = "Base";
        base.decorated = ".?AVBase@@";
        base.vtables.push_back({0x140003000, 0x140002000, 0, {0x140001000, 0x140001010, 0x140001020}});
        cx::Class derived;
        derived.name = "Derived";
        derived.decorated = ".?AVDerived@@";
        cx::BaseClass b;
        b.name = "Base";
        b.decorated = ".?AVBase@@";
        b.direct = true;
        derived.bases.push_back(b);
        derived.vtables.push_back({0x140003040, 0x140002020, 0, {0x140001100, 0x140001110, 0x140001020, 0x140001130}});
        classes.classes.insert_or_assign(base.decorated, base);
        classes.classes.insert_or_assign(derived.decorated, derived);
        names = {{0x140001000, "sub_001000"}, {0x140001010, "_purecall"}, {0x140001020, "sub_001020"}, {0x140001100, "sub_001100"},
                 {0x140001110, "sub_001110"}, {0x140001130, "sub_001130"}, {0x140001200, "sub_001200"}};
        for (const ct::Va va : {0x140001100, 0x140001110, 0x140001130, 0x140001200}) by_va[va] = names.at(va);
        texts.insert_or_assign("sub_001100", "uint8 sub_001100(xunknown8 *param_1,uint8 param_2)\n\n{\n  *param_1 = 0x140003000;\n  return 0;\n}");
        texts.insert_or_assign("sub_001110", "float8 sub_001110(int8 param_1)\n\n{\n  return 2.5;\n}");
        texts.insert_or_assign("sub_001130",
                               "int4 sub_001130(s_sub_001130_param_1 *param_1,int4 param_2)\n\n{\n  param_1->f_8 = param_2;\n  return param_1->f_8;\n}");
        texts.insert_or_assign("sub_001200", "s_sub_001200_param_1 * sub_001200(s_sub_001200_param_1 *param_1)\n\n{\n"
                                             "  param_1->f_0 = 0x140003000;\n  param_1->f_0 = 0x140003040;\n  param_1->f_10 = 0;\n  return param_1;\n}");
        structs.push_back({"s_sub_001130_param_1", 12, {{8, 4, "int4"}}});
        structs.push_back({"s_sub_001200_param_1", 24, {{0, 8, "xunknown8"}, {0x10, 8, "uint8"}}});
    }
    ct::Recovered recover() const { return ct::recover({&classes, &names, &by_va, &texts, structs}); }
};

}  // namespace

CB_TEST("decompile.cpp_text_x64.classes_are_declared_with_their_virtual_functions_in_table_order", [] {
    const ct::Recovered r = Cpp64Fixture().recover();
    CB_CHECK_EQ(r.classes, std::size_t{2});
    const std::string& d = r.declarations;
    CB_CHECK(cpp64_has(d, "class Base {"));
    CB_CHECK(cpp64_has(d, "class Derived : public ::Base {"));
    CB_CHECK(cpp64_has(d, "virtual float8 vf1() = 0;"));                        // _purecall: pure; its override returns float8
    CB_CHECK(cpp64_has(d, "virtual float8 vf1() override;"));
    CB_CHECK(cpp64_has(d, "virtual uint8 vf0(uint8 arg2) override;"));
    CB_CHECK(cpp64_has(d, "virtual uint8 vf3(uint8 arg2);"));
    CB_CHECK(!cpp64_has(d, "vf2() override"));                                  // inherited: the same function in both tables
    CB_CHECK(d.find("virtual uint8 vf0(uint8 arg2);") < d.find("virtual float8 vf1() = 0;"));
    CB_CHECK(cpp64_has(d, "int4 f_8;"));                                        // the members the methods read
    CB_CHECK(cpp64_has(d, "uint1 _pad_c[4];"));
    CB_CHECK(cpp64_has(d, "uint8 f_10;"));
});

CB_TEST("decompile.cpp_text_x64.methods_are_written_as_members_reading_fields_through_this", [] {
    const ct::Recovered r = Cpp64Fixture().recover();
    CB_CHECK_EQ(r.methods.size(), std::size_t{4});
    CB_CHECK_EQ(r.methods.at("sub_001130").qualified, std::string("Derived::vf3"));
    CB_CHECK(r.methods.at("sub_001130").is_virtual);
    const std::string& vf3 = r.texts.at("sub_001130");
    CB_CHECK(cpp64_has(vf3, "uint8 Derived::vf3(uint8 arg2)"));
    CB_CHECK(cpp64_has(vf3, "int4 param_2 = (int4)arg2;"));
    CB_CHECK(cpp64_has(vf3, "this->f_8 = param_2;"));
    CB_CHECK(cpp64_has(vf3, "#pragma comment(linker, \"/ALTERNATENAME:sub_001130=\" __FUNCDNAME__)"));
    // a first parameter that is not the fields' structure is `this` as its own type
    CB_CHECK(cpp64_has(r.texts.at("sub_001100"), "xunknown8 *param_1 = (xunknown8 *)this;"));
    // the constructor: its last table store is Derived's
    CB_CHECK_EQ(r.methods.at("sub_001200").qualified, std::string("Derived::ctor_001200"));
    const std::string& ctor = r.texts.at("sub_001200");
    CB_CHECK(cpp64_has(ctor, "::Derived * Derived::ctor_001200()"));
    CB_CHECK(cpp64_has(ctor, "*(xunknown8 *)this = 0x140003040;"));
    CB_CHECK(cpp64_has(ctor, "this->f_10 = 0;"));
    CB_CHECK(cpp64_has(ctor, "return (::Derived *)(this);"));
    CB_CHECK(cpp64_has(r.declarations, "Derived * ctor_001200();"));
});

CB_TEST("decompile.cpp_text_x64.runtime_library_classes_are_not_declared", [] {
    Cpp64Fixture f;
    cx::Class e;
    e.name = "std::exception";
    e.decorated = ".?AVexception@std@@";
    e.vtables.push_back({0x140003100, 0x140002100, 0, {0x140001300}});
    cx::Classes only;
    only.classes.insert_or_assign(e.decorated, e);
    f.classes = only;
    const ct::Recovered r = f.recover();
    CB_CHECK(r.declarations.empty());
    CB_CHECK(r.methods.empty());
});
