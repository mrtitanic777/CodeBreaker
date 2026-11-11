#include "cb/analyze/resources.hpp"

#include "cb/analyze/test_image.hpp"
#include "cb/core/testing.hpp"

namespace {

using namespace cb::analyze;
using cb::analyze::test::Asm;
using cb::analyze::test::Sample;
using resources::Presence;
using resources::ResourceId;

// Six functions that load resources, one that names dialog controls, and an unrelated import.
struct Loads {
    Sample s;
    Va load_string = 0x402308, dialog_box = 0x40230C, find_resource = 0x402310, get_dlg_item = 0x402314,
       load_icon = 0x402318, other = 0x40231C, load_image = 0x402320;
    Va L = 0x401000, D = 0x401020, Fr = 0x401040, C = 0x401060, Ic = 0x401080, Li = 0x4010A0, O = 0x4010C0, kEnd = 0x4010E0;

    ImageSpec spec() {
        Asm a(s.kCode);
        // LoadStringA(0, 0x12, buffer, 0x40)
        a.raw({0x6A, 0x40}).push_imm(static_cast<std::uint32_t>(s.buffer)).raw({0x6A, 0x12, 0x6A, 0x00}).call_indirect(load_string).ret().pad_to(D);
        // DialogBoxParamA(0, "Hello", 0, 0, 0)
        a.raw({0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00}).push_imm(static_cast<std::uint32_t>(s.hello)).raw({0x6A, 0x00});
        a.call_indirect(dialog_box).ret().pad_to(Fr);
        // FindResourceA(0, "Hello", 5)
        a.raw({0x6A, 0x05}).push_imm(static_cast<std::uint32_t>(s.hello)).raw({0x6A, 0x00}).call_indirect(find_resource).ret().pad_to(C);
        // GetDlgItem(0, 105), and one with an identifier too big to be a control's
        a.raw({0x6A, 0x69, 0x6A, 0x00}).call_indirect(get_dlg_item);
        a.push_imm(0x20000).raw({0x6A, 0x00}).call_indirect(get_dlg_item).ret().pad_to(Ic);
        // LoadIconW(0, 32512)
        a.push_imm(32512).raw({0x6A, 0x00}).call_indirect(load_icon).ret().pad_to(Li);
        // LoadImageA(0, 0x77, 1, 0, 0, 0)
        a.raw({0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x00, 0x6A, 0x01, 0x6A, 0x77, 0x6A, 0x00}).call_indirect(load_image).ret().pad_to(O);
        // something else entirely
        a.raw({0x6A, 0x01, 0x6A, 0x02}).call_indirect(other).ret().pad_to(kEnd);
        ImageSpec spec;
        spec.kind = ImageKind::Pe;
        spec.bits = 32;
        spec.flat = true;
        Span code = s.code;
        code.data = a.bytes();
        code.initialised = static_cast<Va>(code.data.size());
        spec.spans = {code, s.data};
        const Va starts[] = {L, D, Fr, C, Ic, Li, O};
        for (std::size_t i = 0; i < std::size(starts); ++i) spec.functions.emplace_back(starts[i], i + 1 < std::size(starts) ? starts[i + 1] : kEnd);
        spec.imports = {{load_string, "USER32!LoadStringA"},        {dialog_box, "USER32!DialogBoxParamA"},
                        {find_resource, "KERNEL32!FindResourceA"},  {get_dlg_item, "USER32!GetDlgItem"},
                        {load_icon, "USER32!LoadIconW"},            {other, "USER32!SomethingElse"},
                        {load_image, "USER32!LoadImageA"}};
        return spec;
    }
};

CB_TEST("analyze.resources.reads_the_loads_and_the_controls", [] {
    Loads m;
    Image image(m.spec());
    const call_graph::Graph g = call_graph::build(image);
    const strings::Table table = strings::build_table(image, g);
    const resources::Uses uses = resources::find_uses(image, g, table);
    CB_CHECK_EQ(uses.loads.size(), std::size_t{5});
    auto load = [&](std::size_t i) -> const resources::Load& { return uses.loads[i]; };
    CB_CHECK(load(0).function == m.L && load(0).call == "loadstring" && load(0).type == "STRING");
    CB_CHECK(std::get<std::int64_t>(load(0).id) == 0x12);
    CB_CHECK_EQ(load(0).site, m.L + 0x0B);                                     // the call through the slot
    CB_CHECK(load(1).function == m.D && load(1).call == "dialogboxparam" && load(1).type == "DIALOG");
    CB_CHECK(std::get<std::string>(load(1).id) == "HELLO");                    // an address is a name, upper-cased
    CB_CHECK(load(2).call == "findresource" && load(2).type == "DIALOG");      // the type is an argument: 5 is DIALOG
    CB_CHECK(std::get<std::string>(load(2).id) == "HELLO");
    CB_CHECK(load(3).call == "loadicon" && load(3).type == "GROUP_ICON");      // LoadIconW: the W is not part of the name
    CB_CHECK(std::get<std::int64_t>(load(3).id) == 32512);
    CB_CHECK(load(4).call == "loadimage" && !load(4).type.has_value());        // LoadImage's type is not known here
    CB_CHECK(std::get<std::int64_t>(load(4).id) == 0x77);
    CB_CHECK_EQ(uses.controls.size(), std::size_t{1});                         // the big identifier is not a control id
    CB_CHECK(uses.controls[0].function == m.C && uses.controls[0].call == "getdlgitem" && uses.controls[0].control == 105);
});

CB_TEST("analyze.resources.the_module_carries_some_of_what_it_loads", [] {
    Loads m;
    Image image(m.spec());
    const call_graph::Graph g = call_graph::build(image);
    const resources::Uses uses = resources::find_uses(image, g, strings::build_table(image, g));
    const resources::Carried carried = {{"STRING", {ResourceId(std::int64_t{2})}}, {"DIALOG", {ResourceId(std::string("HELLO"))}}};
    const auto resolved = resources::resolve(uses.loads, carried);
    CB_CHECK_EQ(resolved.size(), std::size_t{5});
    CB_CHECK(resolved[0].presence == Presence::Present);          // string 0x12 = 18: block 18/16+1 = 2 is carried
    CB_CHECK(resolved[1].presence == Presence::Present);
    CB_CHECK(resolved[2].presence == Presence::Present);
    CB_CHECK(resolved[3].presence == Presence::SystemStock);      // IDI_APPLICATION and its kind: the system's
    CB_CHECK(resolved[4].presence == Presence::Absent);
});

CB_TEST("analyze.resources.strings_are_carried_in_blocks_of_sixteen", [] {
    auto load = [](std::int64_t id) { return resources::Load{0, 0, "loadstring", "STRING", ResourceId(id)}; };
    const resources::Carried carried = {{"STRING", {ResourceId(std::int64_t{1}), ResourceId(std::int64_t{3})}}};
    const auto r = resources::resolve({load(0), load(15), load(16), load(31), load(32), load(47), load(48)}, carried);
    CB_CHECK(r[0].presence == Presence::Present && r[1].presence == Presence::Present);    // block 1: strings 0..15
    CB_CHECK(r[2].presence == Presence::Absent && r[3].presence == Presence::Absent);      // block 2: not carried
    CB_CHECK(r[4].presence == Presence::Present && r[5].presence == Presence::Present);    // block 3
    CB_CHECK(r[6].presence == Presence::Absent);                                            // block 4
});

CB_TEST("analyze.resources.a_negative_string_id_rounds_its_block_down", [] {
    // (a loaded identifier is never negative in a real module; the block number is floor(id / 16) + 1 all the same)
    auto load = [](std::int64_t id) { return resources::Load{0, 0, "loadstring", "STRING", ResourceId(id)}; };
    const resources::Carried carried = {{"STRING", {ResourceId(std::int64_t{0})}}};
    const auto r = resources::resolve({load(-1), load(-16), load(-17)}, carried);
    CB_CHECK(r[0].presence == Presence::Present);      // -1: floor(-1/16) = -1, block 0 (C++ `/` would say block 1)
    CB_CHECK(r[1].presence == Presence::Present);      // -16: floor(-16/16) = -1, block 0
    CB_CHECK(r[2].presence == Presence::Absent);       // -17: floor(-17/16) = -2, block -1: not carried
});

CB_TEST("analyze.resources.stock_icons_and_cursors_are_the_systems", [] {
    auto load = [](const char* type, std::int64_t id) { return resources::Load{0, 0, "x", type, ResourceId(id)}; };
    const auto r = resources::resolve({load("GROUP_ICON", 32512), load("GROUP_CURSOR", 32649), load("GROUP_ICON", 31999),
                                       load("BITMAP", 32512)},
                                      {});
    CB_CHECK(r[0].presence == Presence::SystemStock && r[1].presence == Presence::SystemStock);
    CB_CHECK(r[2].presence == Presence::Absent);                  // below 32000 it is the module's own
    CB_CHECK(r[3].presence == Presence::Absent);                  // and only icons and cursors have stock ids
});

CB_TEST("analyze.resources.type_names", [] {
    CB_CHECK_EQ(resources::type_name(5), std::string("DIALOG"));
    CB_CHECK_EQ(resources::type_name(14), std::string("GROUP_ICON"));
    CB_CHECK_EQ(resources::type_name(11), std::string("#11"));
    CB_CHECK_EQ(resources::type_name(0), std::string("#0"));
});

CB_TEST("analyze.resources.the_report_counts_and_lists", [] {
    Loads m;
    Image image(m.spec());
    const call_graph::Graph g = call_graph::build(image);
    const resources::Uses uses = resources::find_uses(image, g, strings::build_table(image, g));
    const resources::Carried carried = {{"STRING", {ResourceId(std::int64_t{2})}}, {"DIALOG", {ResourceId(std::string("HELLO"))}}};
    const resources::Report r = resources::analyse(uses, carried);
    const resources::Counts& c = r.counts;
    CB_CHECK_EQ(c.resource_types, 2);
    CB_CHECK_EQ(c.resources_carried, 2);
    CB_CHECK_EQ(c.loads_read, 5);
    CB_CHECK_EQ(c.present_in_module, 3);
    CB_CHECK_EQ(c.absent_from_module, 1);
    CB_CHECK_EQ(c.system_stock, 1);
    CB_CHECK_EQ(c.distinct_resources_referenced, 2);          // string 18 and the dialog "HELLO" (loaded twice)
    CB_CHECK_EQ(c.functions_loading_resources, 5);
    CB_CHECK_EQ(c.dialog_control_uses, 1);
    CB_CHECK_EQ(c.distinct_controls, 1);
    CB_CHECK_EQ(r.carried.at("STRING"), 1);
    CB_CHECK_EQ(r.loads.size(), std::size_t{5});
    CB_CHECK((r.text == std::vector<std::string>{"?: 119", "DIALOG: HELLO", "GROUP_ICON: 32512", "STRING: 18"}));
});

CB_TEST("analyze.resources.nothing_loaded_is_an_empty_report", [] {
    const resources::Report r = resources::analyse({}, {});
    CB_CHECK_EQ(r.counts.loads_read, 0);
    CB_CHECK(r.text.empty() && r.loads.empty() && r.controls.empty());
});

}  // namespace
