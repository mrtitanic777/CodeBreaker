#include "java/emit/ownership.hpp"

#include <vector>

#include "core/error.hpp"
#include "core/testing.hpp"
#include "java/parse.hpp"

namespace {

namespace own = cb::java::emit::ownership;
namespace ir = cb::java::ir;
namespace types = cb::java::types;

// A resolver where Thing, Item and Owner are reference classes and Vec3D is a by-value struct.
types::TypeResolver sample_resolver() {
    types::ResolverData data;
    data.known = {"Thing", "Item", "Owner", "Vec3D"};
    data.values = {"Vec3D"};
    return types::TypeResolver(std::move(data));
}

const ir::Method& method_named(const ir::JClass& cls, std::string_view name) {
    const ir::Method* m = cls.method(name);
    CB_ASSERT(m != nullptr, "the sample class has this method");
    return *m;
}

constexpr const char* kParams = R"(public class P {
    void nullable(Vec3D v, int n) { if (v == null) { return; } }
    void nullable_left(Vec3D v) { if (null != v) { n(); } }
    void nullable_assigned(Vec3D v) { v = null; }
    void plain(Vec3D v, Vec3D w) { w.x = 1; }
    void reassigned(String s, String t) { s = s + "x"; t.length(); }
    void compared(String s) { if (s == other) { } }
    void member(String s) { this.s = s; foo.s = 1; }
    void bumped(int n) { n++; }
    void compound(String s) { s += "y"; }
    void collection(String list) { list.add(x); }
    Thing make() { return new Thing(); }
}
)";

CB_TEST("java.ownership.nullable_value_params", [] {
    const types::TypeResolver r = sample_resolver();
    const ir::JClass cls = cb::java::parse::parse_source(kParams);
    CB_CHECK(own::nullable_value_params(method_named(cls, "nullable"), r) == own::NameSet{"v"});
    CB_CHECK(own::nullable_value_params(method_named(cls, "nullable_left"), r) == own::NameSet{"v"});
    CB_CHECK(own::nullable_value_params(method_named(cls, "nullable_assigned"), r) == own::NameSet{"v"});
    CB_CHECK(own::nullable_value_params(method_named(cls, "plain"), r).empty());
    // `s == other` is not a null test, and a String is never promoted anyway
    CB_CHECK(own::nullable_value_params(method_named(cls, "compared"), r).empty());
});

// An Object compared with null stays a JObject (it is null itself), as its overrides take it.
CB_TEST("java.ownership.an_object_compared_with_null_stays_a_value", [] {
    const ir::JClass cls = cb::java::parse::parse_source(R"(public class Tag {
    public boolean equals(Object o) { if (o != null && o instanceof Tag) { return true; } return false; }
}
)");
    CB_CHECK(own::nullable_value_params(method_named(cls, "equals"), sample_resolver()).empty());
});

CB_TEST("java.ownership.reassigned_params", [] {
    const ir::JClass cls = cb::java::parse::parse_source(kParams);
    CB_CHECK(own::reassigned_params(method_named(cls, "reassigned")) == own::NameSet{"s"});
    CB_CHECK(own::reassigned_params(method_named(cls, "compared")).empty());   // `==` is a comparison
    CB_CHECK(own::reassigned_params(method_named(cls, "member")).empty());     // `this.s =`, `foo.s =` are members
    CB_CHECK(own::reassigned_params(method_named(cls, "bumped")) == own::NameSet{"n"});
    CB_CHECK(own::reassigned_params(method_named(cls, "compound")) == own::NameSet{"s"});
    CB_CHECK(own::reassigned_params(method_named(cls, "plain")).empty());
});

CB_TEST("java.ownership.params_str", [] {
    const types::TypeResolver r = sample_resolver();
    const ir::JClass cls = cb::java::parse::parse_source(kParams);
    CB_CHECK_EQ(own::params_str(method_named(cls, "nullable"), r), "Vec3D* v, int n");   // a value that can be null is a pointer
    CB_CHECK_EQ(own::params_str(method_named(cls, "plain"), r), "Vec3D v, Vec3D w");
    // a String is `const JavaString&` unless the body reassigns it
    CB_CHECK_EQ(own::params_str(method_named(cls, "reassigned"), r), "JavaString s, const JavaString& t");
    CB_CHECK_EQ(own::params_str(method_named(cls, "compared"), r), "const JavaString& s");

    ir::JClass by_ref = cb::java::parse::parse_source("public class Q { void add(Vec3D v, Thing t) { } }");
    by_ref.methods[0].params[0].by_ref = true;
    by_ref.methods[0].params[1].by_ref = true;   // a pointer is never given a second indirection
    CB_CHECK_EQ(own::params_str(by_ref.methods[0], r), "Vec3D& v, Thing* t");
});

CB_TEST("java.ownership.aggregate_array_body", [] {
    const ir::JClass cls = cb::java::parse::parse_source(R"(public class A {
    static int[] a = new int[] { 1, 2, 3 };
    static int[] b = new int[4];
    static String[] c = new String[]{"x", "y"};
    static int d;
}
)");
    CB_CHECK_EQ(own::aggregate_array_body(cls.fields[0]).value_or("-"), " 1, 2, 3 ");
    CB_CHECK(!own::aggregate_array_body(cls.fields[1]));
    CB_CHECK_EQ(own::aggregate_array_body(cls.fields[2]).value_or("-"), "\"x\", \"y\"");
    CB_CHECK(!own::aggregate_array_body(cls.fields[3]));
});

constexpr const char* kOwner = R"(public class Owner {
    private Thing made = new Thing();
    private Thing borrowed;
    private Thing mixed;
    private Thing later;
    private Thing neutral;
    private Thing viaFactory;
    private int count;
    private static Thing shared = new Thing();
    private Vec3D value = new Vec3D();
    public Owner(Thing t) { borrowed = new Thing(); this.borrowed = t; mixed = new Thing(); }
    public void init() { this.later = new Thing(); neutral = null; viaFactory = Buffers.createBuf(16); count = 3; }
    public void again() { mixed = other.getThing(); later = new Thing[2]; }
}
)";

CB_TEST("java.ownership.owned_pointers", [] {
    const types::TypeResolver r = sample_resolver();
    const ir::JClass cls = cb::java::parse::parse_source(kOwner);
    const auto owned = own::owned_pointers(cls, r);
    // made (declared with new), later (new, then new[]: an array wins), viaFactory (an owning factory), in the order
    // they are first allocated. borrowed and mixed are aliased to something else; neutral is only nulled (never
    // allocated); shared is static; value is a by-value struct; count is an int.
    std::vector<std::pair<std::string, bool>> got(owned.begin(), owned.end());
    const std::vector<std::pair<std::string, bool>> want{{"made", false}, {"later", true}, {"viaFactory", false}};
    CB_CHECK(got == want);
});

CB_TEST("java.ownership.owned_containers", [] {
    const types::TypeResolver r = sample_resolver();
    const ir::JClass cls = cb::java::parse::parse_source(R"(public class C {
    private ArrayList<Item> items;
    private Item[] slots;
    private ArrayList<Item> listeners;
    private ArrayList<Item> mixed;
    private static ArrayList<Item> shared;
    public void a() { items.add(new Item()); slots[0] = new Item(); listeners.add(observer); mixed.add(new Item()); mixed.add(other); shared.add(new Item()); }
    public void b() { items.add(Buffers.createItem()); slots[1] = new Item(); }
}
)");
    const auto owned = own::owned_containers(cls, r);
    std::vector<std::pair<std::string, std::string>> got(owned.begin(), owned.end());
    const std::vector<std::pair<std::string, std::string>> want{{"items", ".size()"}, {"slots", ".length"}};
    CB_CHECK(got == want);
    CB_CHECK(own::owns_heap(cls, r));
    CB_CHECK(!own::owns_heap(cb::java::parse::parse_source("public class N { int x; Thing t; void f() { t = other; } }"), r));
    CB_CHECK(own::owns_heap(cb::java::parse::parse_source("public class N { Thing t; void f() { t = new Thing(); } }"), r));
});

constexpr const char* kReturns = R"(public class R {
    public ArrayList cubes() { ArrayList out = new ArrayList(); out.add(new Box()); out.add(Box.createBox(1)); return out; }
    public ArrayList borrowed() { ArrayList out = new ArrayList(); out.add(this.box); return out; }
    public ArrayList field() { return items; }
    public ArrayList none() { ArrayList out = new ArrayList<Box>(); return out; }
    public Box expand() { if (x) { return null; } return new Box(1); }
    public Box mixed() { if (x) { return box; } return new Box(1); }
    public Box nothing() { return; }
    public Thing thing() { return new Thing(); }
    public Vec3D vec(int n) { return new Vec3D(n); }
    public Vec3D vecNull() { if (x) { return null; } return v; }
    public Vec3D vecPlain() { return v; }
    public String str() { return null; }
    public int[] arr() { return null; }
    public void v() { return; }
    public R() { }
}
)";

CB_TEST("java.ownership.returned_ownership", [] {
    types::ResolverData data;
    data.known = {"Box", "Thing", "Vec3D"};
    data.values = {"Vec3D"};
    const types::TypeResolver r(std::move(data));
    const ir::JClass cls = cb::java::parse::parse_source(kReturns);
    CB_CHECK(own::returns_owned_container(method_named(cls, "cubes"), r));
    CB_CHECK(!own::returns_owned_container(method_named(cls, "borrowed"), r));
    CB_CHECK(!own::returns_owned_container(method_named(cls, "field"), r));
    CB_CHECK(!own::returns_owned_container(method_named(cls, "none"), r));   // built, but no adds
    CB_CHECK(own::returns_owned_scalar(method_named(cls, "expand"), r));
    CB_CHECK(!own::returns_owned_scalar(method_named(cls, "mixed"), r));
    CB_CHECK(!own::returns_owned_scalar(method_named(cls, "nothing"), r));
    CB_CHECK(!own::returns_owned_scalar(method_named(cls, "cubes"), r));   // a container is not a scalar
});

CB_TEST("java.ownership.returns_pointer_and_return_cpp_type", [] {
    types::ResolverData data;
    data.known = {"Box", "Thing", "Vec3D"};
    data.values = {"Vec3D"};
    const types::TypeResolver r(std::move(data));
    const ir::JClass cls = cb::java::parse::parse_source(kReturns);
    CB_CHECK(own::returns_pointer(method_named(cls, "thing"), r));
    CB_CHECK(own::returns_pointer(method_named(cls, "vec"), r));        // a value type that the body `new`s: promoted
    CB_CHECK(own::returns_pointer(method_named(cls, "vecNull"), r));    // ... or that returns null
    CB_CHECK(!own::returns_pointer(method_named(cls, "vecPlain"), r));
    CB_CHECK(!own::returns_pointer(method_named(cls, "str"), r));       // std::string never
    CB_CHECK(!own::returns_pointer(method_named(cls, "arr"), r));
    CB_CHECK(!own::returns_pointer(method_named(cls, "cubes"), r));     // containers never
    CB_CHECK(!own::returns_pointer(method_named(cls, "v"), r));
    CB_CHECK_EQ(own::return_cpp_type(method_named(cls, "thing"), r), "Thing*");
    CB_CHECK_EQ(own::return_cpp_type(method_named(cls, "vec"), r), "Vec3D*");
    CB_CHECK_EQ(own::return_cpp_type(method_named(cls, "vecPlain"), r), "Vec3D");
    CB_CHECK_EQ(own::return_cpp_type(method_named(cls, "v"), r), "void");
    CB_CHECK_EQ(own::return_cpp_type(method_named(cls, "str"), r), "JavaString");
});

CB_TEST("java.ownership.program_wide_catalogues", [] {
    types::ResolverData data;
    data.known = {"Box", "Thing", "Vec3D", "R", "S"};
    const types::TypeResolver r(std::move(data));
    std::vector<ir::JClass> classes;
    classes.push_back(cb::java::parse::parse_source(kReturns));
    // S repeats the name `cubes` with a definition that does not own: unanimity drops it.
    classes.push_back(cb::java::parse::parse_source(R"(public class S {
    public ArrayList cubes() { return items; }
    public ArrayList others() { ArrayList o = new ArrayList(); o.add(new Box()); return o; }
    public Box expand() { return new Box(2); }
    public Thing thing() { return new Thing(); }
    public Box wrap(Box b, Box c, Thing d) { b.touch(); c.refs[0] = 1; store = d; return b; }
    public void keep(Box a) { items.add(a); }
    public void read(Box a) { a.touch(); a.items[0] = 1; }
}
)"));
    CB_CHECK(own::owned_return_container_methods(classes, r) == own::NameSet{"others"});
    const auto scalars = own::owned_scalar_return_methods(classes, r);
    // expand: both definitions return `new Box` (or null); thing: both `new Thing`; mixed returns a field once
    CB_CHECK_EQ(scalars.at("expand"), "Box");
    CB_CHECK_EQ(scalars.at("thing"), "Thing");
    CB_CHECK(!scalars.contains("mixed"));
    const auto retained = own::retained_param_positions(classes);
    CB_CHECK(retained.at("wrap") == std::set<std::size_t>{0, 2});   // b is returned, d stored; c is only read
    CB_CHECK(retained.at("keep") == std::set<std::size_t>{0});
    CB_CHECK(!retained.contains("read"));
});

}  // namespace
