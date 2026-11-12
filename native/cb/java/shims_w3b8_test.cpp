// The shims at the old header paths for java/runtime and java/emit/source: they must compile with the old layer and forward
// to the new code, through the old back end's interface. Built at /W3 (the old layer is not warning-clean).
#include <string>

#include "cb/core/testing.hpp"
#include "codebreaker/java/_paths.hpp"
#include "codebreaker/java/emit/source.hpp"
#include "codebreaker/java/parse.hpp"
#include "codebreaker/java/runtime.hpp"

namespace {

namespace jv = codebreaker::java;
using cb::compat::from_py;
using cb::compat::to_py;

void sw3_point_at_the_java_data() { jv::_paths::set_here(to_py(cb::testing::data_path("../../codebreaker/java").string())); }

CB_TEST("java.shims_w3b8.runtime_views", [] {
    sw3_point_at_the_java_data();
    const auto& members = jv::runtime::STUB_MEMBERS();
    CB_CHECK(members.contains(to_py("Element")));
    CB_CHECK_EQ(from_py(*members.at(to_py("Element")).parent), "Node");
    CB_CHECK_EQ(from_py(members.at(to_py("Element")).methods.at(to_py("getAttribute"))), "std::string");
    CB_CHECK(jv::runtime::FILES().contains(to_py("JavaRT.h")));
    CB_CHECK(jv::runtime::_GENERATED().contains(to_py("Keyboard.h")));
    CB_CHECK(jv::runtime::unwired_canonicals().empty());
    // the file set of a conversion for a platform and a standard
    const auto files = jv::runtime::generate_files(jv::emit::header::CppStd(2011), to_py("xbox360"));
    CB_CHECK(files.contains(to_py("stdafx.cpp")));
    CB_CHECK(!files.contains(to_py("j2x_host_io.cpp")));
    CB_CHECK(from_py(files.at(to_py("Keyboard.h"))).find("EXTERNAL-LIBRARY STUB") != std::string::npos);
});

// An old back end that answers a body for each method and declines everything else.
class Sw3Bodies : public jv::emit::source::Bodies {
  public:
    using Parts = jv::emit::source::CtorParts;
    std::optional<py::str> field_init(const py::str&, const py::str&) override { return std::nullopt; }
    std::optional<py::list<py::str>> array_items(const py::str&, const py::str&) override { return std::nullopt; }
    std::optional<py::list<py::str>> instance_inits(const py::str&) override { return std::nullopt; }
    std::optional<Parts> ctor(const py::str&, const jv::emit::source::MethodRef& old, bool) override {
        Parts p;
        if (old) p.kind = py::str("super");
        if (old) p.args = to_py("a0");
        p.body = py::list<py::str>{to_py(old ? "// ctor with params" : "// implicit")};
        return p;
    }
    std::optional<py::list<py::str>> enum_args(const py::str&, const py::str&) override { return std::nullopt; }
    std::optional<py::list<py::list<py::str>>> static_blocks(const py::str&) override { return std::nullopt; }
    std::optional<py::list<py::str>> body(const py::str& cls, const jv::emit::source::MethodRef& old) override {
        // (the method's position in the class is how the adapter finds it again: the answer names the method it was asked for)
        return py::list<py::str>{py::str("// body of ") + cls + py::str(".") + old->name};
    }
};

CB_TEST("java.shims_w3b8.render_source_through_the_old_back_end", [] {
    const auto cls = jv::parse::parse_source(to_py(R"(package t;
public class Widget extends Base {
    private int count;
    public static Foo shared = new Foo();
    public Widget(int count) { this.count = count; }
    public int size() { return count; }
    public void tick() { count++; }
}
)"));
    py::set<py::str> known;
    known.add(to_py("Widget"));
    known.add(to_py("Base"));
    known.add(to_py("Foo"));
    const jv::types::TypeResolver resolver(known, py::set<py::str>());
    jv::ledger::Ledger led;
    Sw3Bodies bodies;
    jv::emit::source::TREE_BODIES = &bodies;
    const auto text = jv::emit::source::render_source(cls, resolver, &led, std::nullopt, to_py("xbox360"));
    jv::emit::source::TREE_BODIES = nullptr;
    CB_CHECK(text.has_value());
    const std::string cpp = from_py(*text);
    // the back end's answers reach the right members, the one it declined is a ledgered gap, and the ledger came back
    CB_CHECK(cpp.find("// body of Widget.size\n") != std::string::npos || cpp.find("\t// body of Widget.size") != std::string::npos);
    CB_CHECK(cpp.find("\t// body of Widget.tick") != std::string::npos);
    CB_CHECK(cpp.find("\t// ctor with params") != std::string::npos);
    CB_CHECK(cpp.find("Widget::Widget(int count)\n\t: Base(a0)") != std::string::npos);
    CB_CHECK(cpp.find("// J2X-GAP: static field Widget.shared was not translated") != std::string::npos);
    CB_CHECK(led.events.size() > 0);
    CB_CHECK(cls->has_init_statics);   // the gap of the static field is a statement in initStatics()
    // no back end: every body is a gap
    const auto bare = jv::emit::source::render_source(cls, resolver);
    CB_CHECK(from_py(*bare).find("// J2X-GAP: method size was not translated") != std::string::npos);
});

}  // namespace
