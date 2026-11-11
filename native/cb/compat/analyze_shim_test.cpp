// The old analyze/{strings, call_graph, resources, package} API (codebreaker::analyze::*, over py::) as the compat shims at
// native/typed/cpp/codebreaker/analyze/*.hpp give it: conversion and forwarding to cb/analyze. Builds at /W3.
// (Their old harnesses, which this file only keeps compiling, ran 7,214 / 7,344 / 56 recorded calls against them: see
// the refactor report.)
#include "codebreaker/analyze/call_graph.hpp"
#include "codebreaker/analyze/package.hpp"
#include "codebreaker/analyze/resources.hpp"
#include "codebreaker/analyze/strings.hpp"

#include "cb/core/testing.hpp"

namespace {

namespace ostr = codebreaker::analyze::strings;
namespace ocg = codebreaker::analyze::call_graph;
namespace ores = codebreaker::analyze::resources;

CB_TEST("compat.analyze.package_doc_is_the_overview", [] {
    CB_CHECK(codebreaker::analyze::DOC.utf8().starts_with("What can be worked out ABOUT a module"));
});

CB_TEST("compat.analyze.strings_pure_functions_forward", [] {
    const py::list<py::str> t = ostr::conversions(py::str("%s = %ld"));
    CB_CHECK(t.size() == 2 && t[0] == py::str("char *") && t[1] == py::str("long"));
    CB_CHECK(ostr::_ascii(py::bytes(std::string("Hello\0", 6)), 0, 2).value() == py::str("Hello"));
    CB_CHECK(!ostr::_ascii(py::bytes(std::string("H\0", 2)), 0, 2).has_value());
    CB_CHECK(ostr::_wide(py::bytes(std::string("W\0i\0d\0e\0\0\0", 10)), 0, 2).value() == py::str("Wide"));
    // _assemble: the old keys are (base as a number, offset); "Hello" by two stores
    py::dict<std::tuple<int64_t, int64_t>, std::tuple<int64_t, int64_t>> stores;
    const char* hello = "Hello";
    for (int i = 0; i < 5; ++i) stores.set(std::make_tuple(int64_t(7), int64_t(-8 + i)), std::make_tuple(int64_t(hello[i]), int64_t(i < 3 ? 1 : 2)));
    const auto pieces = ostr::_assemble(stores);
    CB_CHECK(pieces.size() == 1 && std::get<0>(pieces[0]) == py::str("7") && std::get<1>(pieces[0]) == -8 &&
             std::get<2>(pieces[0]) == py::str("Hello") && !std::get<3>(pieces[0]));
});

// The module as strings.hpp's Image asks for it, with canned answers.
class StringsImage : public ostr::Image {
public:
    std::optional<ostr::Span> span(int64_t va) override {
        if (va < 0x402000 || va >= 0x402010) return std::nullopt;
        ostr::Span s;
        s.va = 0x402000;
        s.data = py::bytes(std::string("Hello\0World\0\0\0\0\0", 16));
        s.initialised = 16;
        return s;
    }
    py::str label(int64_t va) override { return py::str(std::string("f") + std::to_string(va)); }
    py::list<ostr::RawTableRow> ask_table() override {
        py::list<ostr::RawTableRow> out;
        out.append(ostr::RawTableRow(0x402000, py::str("Hello"), py::str("A"), true));
        out.append(ostr::RawTableRow(0x402006, py::str("World"), py::str("A"), false));
        return out;
    }
    py::list<ostr::RawUsesRow> ask_uses() override {
        py::list<ostr::RawUsesRow> out;
        py::list<ostr::Use> u;
        u.append(ostr::Use(0x401005, 0x402000, py::str("Hello")));
        out.append(ostr::RawUsesRow(0x401000, u));
        return out;
    }
    py::list<ostr::StackString> ask_stack() override { return {}; }
    ostr::RawFormats ask_formats() override { return {}; }
};

CB_TEST("compat.analyze.strings_at_and_analyse_through_the_old_image", [] {
    StringsImage img;
    CB_CHECK(std::get<0>(ostr::at(img, 0x402000).value()) == py::str("Hello"));
    CB_CHECK(!ostr::at(img, 0x402005).has_value());                    // the terminator
    CB_CHECK(!ostr::at(img, 0x402008).has_value());                    // "rld": the tail of a longer string
    const ostr::Analysis a = ostr::analyse(img);
    CB_CHECK(a.counts.strings == 2 && a.counts.referenced == 1 && a.counts.functions_using_strings == 1 && a.counts.string_uses == 1);
    CB_CHECK(a.facts.size() == 3);                                     // two strings and one function's uses
    CB_CHECK(std::get<ostr::StringFact>(a.facts[0]).text == py::str("Hello"));
});

class GraphImage : public ocg::Image {
public:
    GraphImage() {
        functions.append(0x401000);
        functions.append(0x401020);
        roots.append(0x401000);
    }
    ocg::RawGraph ask_call_graph() override {
        ocg::RawGraph g;
        py::list<ocg::RawCall> a, b;
        a.append(ocg::RawCall{0x401005, py::str("call"), 0x401020});
        g.calls.append(std::make_tuple(int64_t(0x401000), a));
        g.calls.append(std::make_tuple(int64_t(0x401020), b));
        py::list<int64_t> callers;
        callers.append(0x401000);
        g.callers.append(std::make_tuple(int64_t(0x401020), callers));
        g.order.append(0x401020);
        g.order.append(0x401000);
        g.reachable.append(0x401000);
        g.reachable.append(0x401020);
        g.reachable_with_tables = g.reachable;
        return g;
    }
    std::shared_ptr<ocg::Function> function(int64_t) override {
        auto f = std::make_shared<ocg::Function>();
        auto ins = std::make_shared<ocg::Ins>();
        ins->addr = 0x401000;
        ins->size = 6;
        f->insns.append(ins);
        f->index.set(0x401000, 0);
        return f;
    }
    py::str label(int64_t va) override { return py::str(std::string("f") + std::to_string(va)); }
};

CB_TEST("compat.analyze.call_graph_builds_and_reports_through_the_old_image", [] {
    GraphImage img;
    const auto g = ocg::build(img);
    CB_CHECK(g->calls.size() == 2 && g->callers.at(0x401020).size() == 1);
    const ocg::Analysis a = ocg::analyse(img);
    CB_CHECK(a.counts.functions == 2 && a.counts.call_sites == 1 && a.counts.edges == 1 && a.counts.leaf_functions == 1);
    CB_CHECK(a.counts.reached_from_roots == 2 && a.facts.size() == 2);
    CB_CHECK(a.facts[0].callees.size() == 1 && a.facts[0].callees[0] == py::str("f4198432"));
    CB_CHECK(a.text.size() >= 3 && a.text[0] == py::str("most called:"));
});

class ResImage : public ores::Image {
public:
    ResImage() { kind = py::str("pe"); }
    py::list<ores::PeType> pe_resource_tree() override {
        ores::PeType t;
        t.id = 6;
        t.type = py::str("");
        ores::PeItem item;
        item.id = 2;
        t.items.append(item);
        py::list<ores::PeType> out;
        out.append(t);
        return out;
    }
    ores::RawUses ask_uses() override {
        ores::RawUses u;
        u.loads.append(ores::RawLoad(0x401000, 0x401010, py::str("loadstring"), std::optional<py::str>(py::str("STRING")), ores::Ident(int64_t(18))));
        u.loads.append(ores::RawLoad(0x401000, 0x401020, py::str("loadicon"), std::optional<py::str>(py::str("GROUP_ICON")), ores::Ident(int64_t(32512))));
        u.controls.append(ores::RawControl(0x401000, 0x401030, py::str("getdlgitem"), 105));
        return u;
    }
};

CB_TEST("compat.analyze.resources_resolve_and_report_through_the_old_image", [] {
    ResImage img;
    const auto uses = ores::uses(img);
    const auto& loads = std::get<0>(uses);
    CB_CHECK(loads.size() == 2);
    CB_CHECK(loads[0].present.has_value() && *loads[0].present);       // string 18 is in block 2, which is carried
    CB_CHECK(!loads[1].present.has_value());                           // a stock icon
    const ores::Analysis a = ores::analyse(img);
    CB_CHECK(a.counts.loads_read == 2 && a.counts.present_in_module == 1 && a.counts.system_stock == 1 && a.counts.dialog_control_uses == 1);
    CB_CHECK(a.facts.size() == 4);                                     // two loads, one control, what is carried
    CB_CHECK(a.text.size() == 2 && a.text[0] == py::str("GROUP_ICON: 32512"));
});

}  // namespace
