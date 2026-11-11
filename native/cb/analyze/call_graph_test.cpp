#include "cb/analyze/call_graph.hpp"

#include <algorithm>

#include "cb/analyze/test_image.hpp"
#include "cb/core/testing.hpp"

namespace {

using namespace cb::analyze;
using cb::analyze::test::Sample;
using call_graph::Access;
using call_graph::CallKind;

// The sample program with the names a person would give its functions.
struct Named {
    Sample s;
    Image image{make()};

    ImageSpec make() {
        ImageSpec spec = s.spec();
        spec.function_names = {{s.A, "main"}, {s.B, "helper"}, {s.C, "dispatch"}, {s.D, "recurse"}, {s.F, "quit"},
                               {s.G, "stack"}, {s.P, "format"}, {s.H, "log"},      {s.I, "log_x"},   {s.J, "log_y"}};
        return spec;
    }
};

CB_TEST("analyze.call_graph.calls_by_kind", [] {
    Named n;
    const Sample& s = n.s;
    const call_graph::Graph g = call_graph::build(n.image);
    CB_CHECK_EQ(g.calls.size(), std::size_t{10});            // every function has an entry, empty or not
    // A calls B
    CB_CHECK_EQ(g.calls.at(s.A).size(), std::size_t{1});
    CB_CHECK(g.calls.at(s.A)[0].kind == CallKind::Call);
    CB_CHECK(g.calls.at(s.A)[0].destination.has_value() && *g.calls.at(s.A)[0].destination == s.B);
    CB_CHECK_EQ(g.calls.at(s.A)[0].site, s.A + 0x0A);
    // D calls itself and jumps to B (a tail call)
    CB_CHECK_EQ(g.calls.at(s.D).size(), std::size_t{2});
    CB_CHECK(g.calls.at(s.D)[0].kind == CallKind::Call && *g.calls.at(s.D)[0].destination == s.D);
    CB_CHECK(g.calls.at(s.D)[1].kind == CallKind::Tail && *g.calls.at(s.D)[1].destination == s.B);
    // F calls through an import's slot, then through a register
    CB_CHECK_EQ(g.calls.at(s.F).size(), std::size_t{2});
    CB_CHECK(g.calls.at(s.F)[0].kind == CallKind::Import && *g.calls.at(s.F)[0].destination == s.exit_slot);
    CB_CHECK(g.calls.at(s.F)[1].kind == CallKind::Indirect && !g.calls.at(s.F)[1].destination.has_value());
    // a jump through a recognised table is not an unknown destination
    CB_CHECK(g.calls.at(s.C).empty());
    CB_CHECK(g.calls.at(s.B).empty());
    CB_CHECK(call_graph::name_of(CallKind::Tail) == "tail" && call_graph::name_of(Access::ReadWrite) == "rw");
});

CB_TEST("analyze.call_graph.callers_users_and_cycles", [] {
    Named n;
    const Sample& s = n.s;
    const call_graph::Graph g = call_graph::build(n.image);
    CB_CHECK((g.callers.at(s.B) == std::vector<Va>{s.A, s.D}));
    CB_CHECK((g.callers.at(s.D) == std::vector<Va>{s.D}));
    CB_CHECK((g.callers.at(s.H) == std::vector<Va>{s.I, s.J}));
    CB_CHECK(!g.callers.contains(s.A));
    CB_CHECK((g.users.at("KERNEL32!ExitProcess") == std::vector<Va>{s.F}));
    CB_CHECK((g.users.at("USER32!wsprintfA") == std::vector<Va>{s.P}));
    CB_CHECK_EQ(g.users.size(), std::size_t{2});
    CB_CHECK_EQ(g.cycles.size(), std::size_t{1});            // D calls itself
    CB_CHECK((g.cycles[0] == std::vector<Va>{s.D}));
    // callees come before callers
    auto at = [&](Va f) { return std::find(g.order.begin(), g.order.end(), f) - g.order.begin(); };
    CB_CHECK_EQ(g.order.size(), std::size_t{10});
    CB_CHECK(at(s.B) < at(s.A) && at(s.B) < at(s.D) && at(s.H) < at(s.I) && at(s.H) < at(s.J));
});

CB_TEST("analyze.call_graph.references_say_how_an_address_is_used", [] {
    Named n;
    const Sample& s = n.s;
    const call_graph::Graph g = call_graph::build(n.image);
    auto has = [&](Va function, Va instruction, Va address, Access how) {
        return std::any_of(g.refs.begin(), g.refs.end(), [&](const call_graph::Reference& r) {
            return r.function == function && r.instruction == instruction && r.address == address && r.how == how;
        });
    };
    CB_CHECK(has(s.A, s.A + 5, s.hello, Access::AddressTaken));          // push offset hello
    CB_CHECK(has(s.C, s.C + 9, s.jump_table, Access::Read));             // jmp [table + eax*4]
    CB_CHECK(has(s.P, s.P + 2, s.format, Access::AddressTaken));
    CB_CHECK(has(s.P, s.P + 7, s.buffer, Access::AddressTaken));
    CB_CHECK(has(s.I, s.I, s.format_x, Access::AddressTaken));
    CB_CHECK_EQ(g.refs.size(), std::size_t{6});                          // and nothing else: the import slots are not data
    for (const call_graph::Reference& r : g.refs) CB_CHECK(r.address != s.exit_slot && r.address != s.wsprintf_slot);
});

CB_TEST("analyze.call_graph.reach_and_unreferenced", [] {
    Named n;
    const Sample& s = n.s;
    const call_graph::Graph g = call_graph::build(n.image);
    // A is the root: it reaches B
    CB_CHECK((g.reachable == std::set<Va>{s.A, s.B}));
    // the data holds the addresses of B and F: entering there reaches F too
    CB_CHECK((g.reachable_with_tables == std::set<Va>{s.A, s.B, s.F}));
    CB_CHECK_EQ(g.taken.size(), std::size_t{2});
    CB_CHECK(!g.taken.at(s.B)[0].in_code && g.taken.at(s.B)[0].at == s.pointer_table);
    CB_CHECK(!g.taken.at(s.F)[0].in_code && g.taken.at(s.F)[0].at == s.pointer_table + 4);
    // nothing calls, takes or enters these
    CB_CHECK((g.unreferenced == std::vector<Va>{s.C, s.G, s.P, s.I, s.J}));
});

CB_TEST("analyze.call_graph.a_function_address_pushed_in_code_is_taken_there", [] {
    // F's body is replaced by `push B`: its address is taken by code, and B is then reached from F
    Sample s;
    ImageSpec spec = s.spec();
    cb::analyze::test::Asm a(s.kCode);
    a.raw({0x33, 0xC0}).ret().pad_to(s.B);   // A: xor eax,eax; ret
    a.raw({0x33, 0xC0}).ret().pad_to(s.C);   // B
    spec.spans[0].data.assign(a.bytes().begin(), a.bytes().end());
    spec.spans[0].data.resize(0x100, 0xCC);
    spec.spans[0].data[s.F - s.kCode] = 0x68;   // push imm32 (B)
    for (int i = 0; i < 4; ++i) spec.spans[0].data[s.F - s.kCode + 1 + i] = static_cast<std::uint8_t>(s.B >> (8 * i));
    spec.spans[0].data[s.F - s.kCode + 5] = 0xC3;
    spec.roots = {s.F};
    Image image(std::move(spec));
    const call_graph::Graph g = call_graph::build(image);
    const auto taken = g.taken.find(s.B);
    CB_CHECK(taken != g.taken.end());
    bool from_f = false;
    for (const call_graph::Taken& t : taken->second) from_f = from_f || (t.in_code && t.function && *t.function == s.F && t.at == s.F);
    CB_CHECK(from_f);
    CB_CHECK(g.reachable.contains(s.F) && g.reachable.contains(s.B));
});

CB_TEST("analyze.call_graph.a_module_with_no_functions_has_an_empty_graph", [] {
    Sample s;
    ImageSpec spec = s.spec();
    spec.functions.clear();
    spec.roots.clear();
    Image image(std::move(spec));
    const call_graph::Graph g = call_graph::build(image);
    CB_CHECK(g.calls.empty() && g.callers.empty() && g.refs.empty() && g.order.empty() && g.cycles.empty());
    CB_CHECK(g.reachable.empty() && g.unreferenced.empty());
    const call_graph::Report r = call_graph::analyse(image, g);
    CB_CHECK_EQ(r.counts.functions, 0);
    CB_CHECK(r.facts.empty());
});

CB_TEST("analyze.call_graph.the_report_counts_and_names", [] {
    Named n;
    const Sample& s = n.s;
    const call_graph::Graph g = call_graph::build(n.image);
    const call_graph::Report r = call_graph::analyse(n.image, g);
    const call_graph::Counts& c = r.counts;
    CB_CHECK_EQ(c.functions, 10);
    CB_CHECK_EQ(c.functions_found_from_calls, 0);
    CB_CHECK_EQ(c.call_sites, 4);            // A->B, D->D, I->H, J->H
    CB_CHECK_EQ(c.tail_jumps, 1);
    CB_CHECK_EQ(c.import_call_sites, 2);
    CB_CHECK_EQ(c.indirect_call_sites, 1);
    CB_CHECK_EQ(c.edges, 5);                 // distinct callees per function: A{B} D{D,B} I{H} J{H}
    CB_CHECK_EQ(c.imports_used, 2);
    CB_CHECK_EQ(c.leaf_functions, 4);        // B C G H
    CB_CHECK_EQ(c.recursive_groups, 1);
    CB_CHECK_EQ(c.address_taken_functions, 2);
    CB_CHECK_EQ(c.data_references, 6);
    CB_CHECK_EQ(c.roots, 1);
    CB_CHECK_EQ(c.reached_from_roots, 2);
    CB_CHECK_EQ(c.reached_with_data_tables, 3);
    CB_CHECK_EQ(c.unreferenced_functions, 5);
    CB_CHECK(c.code_bytes_decoded > 0 && c.code_bytes_decoded <= 0x100);
    CB_CHECK_EQ(c.code_section_bytes_not_reached, 0x100 - c.code_bytes_decoded);
    // facts: one per function
    CB_CHECK_EQ(r.facts.size(), std::size_t{10});
    const call_graph::CallsFact& d = r.facts[3];             // D
    CB_CHECK_EQ(d.addr, s.D);
    CB_CHECK((d.callees == std::vector<std::string>{"helper", "recurse"}));   // by address: B, then D
    CB_CHECK((d.callers == std::vector<std::string>{"recurse"}));
    CB_CHECK(!d.reached);
    const call_graph::CallsFact& f = r.facts[4];             // F
    CB_CHECK((f.imports == std::vector<std::string>{"KERNEL32!ExitProcess"}));
    CB_CHECK_EQ(f.indirect, 1);
    CB_CHECK_EQ(f.address_taken, 1);
    CB_CHECK(r.facts[0].reached && r.facts[1].reached);       // A, B
    // the summary for people
    CB_CHECK_EQ(r.text[0], std::string("most called:"));
    bool most_called = false, imports = false, recursive = false;
    for (const std::string& line : r.text) {
        most_called = most_called || (line.find("helper") != std::string::npos && line.find("2 callers") != std::string::npos);
        imports = imports || line.find("KERNEL32!ExitProcess") != std::string::npos;
        recursive = recursive || line == "recursive groups: recurse";
    }
    CB_CHECK(most_called && imports && recursive);
});

}  // namespace
