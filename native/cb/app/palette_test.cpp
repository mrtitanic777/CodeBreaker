// Tests of palette.hpp: the items, scoring, the sections and their order, ranking, and moving through the result.
#include "cb/app/palette.hpp"

#include "cb/core/testing.hpp"

namespace cb::app::palette {
namespace {

std::vector<tools::Tool> pl_tools() {
    std::vector<tools::Tool> t(4);
    t[0].name = "disasm";
    t[0].title = "Disassemble a program";
    t[0].group = "Inspect";
    t[0].description = "Turns the code of a program back into assembly.";
    t[1].name = "disasm-fn";
    t[1].title = "Disassemble one function";
    t[1].group = "Inspect";
    t[2].name = "iso";
    t[2].title = "Disc image: list and extract";
    t[2].group = "Files";
    t[2].description = "Lists the files of a disc image.";
    t[3].name = "hexdump";
    t[3].title = "Hex dump";
    t[3].group = "Inspect";
    return t;
}

std::vector<RecentEntry> pl_recent() { return {{"lz32.dll", "converted 2 minutes ago", false}, {"gone.dll", "converted today", true}}; }

std::string pl_titles(const std::vector<Item>& items, const Result& r) {
    std::string out;
    for (const Row& row : r.rows) out += (row.heading ? "[" + row.text + "]" : items[static_cast<std::size_t>(row.item)].title) + "|";
    return out;
}

CB_TEST("app.palette.items_are_the_tools_the_recent_conversions_still_there_and_the_places", [] {
    const auto items = make_items(pl_tools(), pl_recent());
    CB_CHECK_EQ(items.size(), 4u + 1u + 4u);   // (the missing folder is not listed)
    CB_CHECK(items[0].kind == Kind::Tool && items[0].index == 0 && items[0].note == "Inspect");
    CB_CHECK(items[4].kind == Kind::Recent && items[4].title == "lz32.dll" && items[4].index == 0);
    CB_CHECK(items[8].kind == Kind::GoTo && items[8].title == "Settings: Advanced" && items[8].hint == "Ctrl+,");
});

CB_TEST("app.palette.scores_rank_a_prefix_over_a_word_start_over_the_middle_of_a_word", [] {
    CB_CHECK(score("dis", "Disassemble") > score("dis", "Hex dump display"));
    CB_CHECK(score("dis", "Hex dis") > score("dis", "Indisposed"));
    CB_CHECK_EQ(score("zzz", "Disassemble"), 0);
    CB_CHECK_EQ(score("", "anything"), 1);
    CB_CHECK_EQ(score("   ", "anything"), 1);
    CB_CHECK(score("DISASSEMBLE", "disassemble a program") > 0);   // case is ignored
    CB_CHECK_EQ(score("dis zzz", "Disassemble"), 0);               // every word must be there
    CB_CHECK(score("disc image", "Disc image: list and extract") > 0);
});

CB_TEST("app.palette.sections_come_in_the_order_tools_recent_go_to_and_empty_ones_are_left_out", [] {
    const auto items = make_items(pl_tools(), pl_recent());
    const Result r = search(items, "dis");
    CB_CHECK_EQ(pl_titles(items, r), "[Tools]|Disassemble a program|Disassemble one function|Disc image: list and extract|");
    CB_CHECK_EQ(r.picks.size(), 3u);
    const Result s = search(items, "s");
    CB_CHECK(pl_titles(items, s).find("[Recent]|lz32.dll|") != std::string::npos);   // (found by "minutes" in its note)
    CB_CHECK(pl_titles(items, s).find("[Go to]|") != std::string::npos);
    CB_CHECK(search(items, "qqqq").rows.empty() && search(items, "qqqq").picks.empty());
});

CB_TEST("app.palette.a_match_in_the_title_ranks_above_one_in_the_description", [] {
    const auto items = make_items(pl_tools(), {});
    const Result r = search(items, "assembly");   // only the first tool says it, in its description
    CB_CHECK_EQ(r.picks.size(), 1u);
    CB_CHECK_EQ(items[static_cast<std::size_t>(r.picks[0])].title, "Disassemble a program");
    const Result d = search(items, "disc");   // "Disc image" starts with it; "Disassemble" does not contain it
    CB_CHECK_EQ(items[static_cast<std::size_t>(d.picks[0])].title, "Disc image: list and extract");
});

CB_TEST("app.palette.every_section_is_capped", [] {
    std::vector<tools::Tool> many(12);
    for (std::size_t i = 0; i < many.size(); ++i) many[i].title = "Tool " + std::to_string(i);
    std::vector<RecentEntry> recent;
    for (int i = 0; i < 6; ++i) recent.push_back({"f" + std::to_string(i), "", false});
    const auto items = make_items(many, recent);
    const Result r = search(items, "");
    int tools = 0, recents = 0;
    for (const int pick : r.picks) {
        tools += items[static_cast<std::size_t>(pick)].kind == Kind::Tool;
        recents += items[static_cast<std::size_t>(pick)].kind == Kind::Recent;
    }
    CB_CHECK_EQ(tools, static_cast<int>(kMaxTools));
    CB_CHECK_EQ(recents, static_cast<int>(kMaxRecent));
    CB_CHECK_EQ(r.picks.size(), kMaxTools + kMaxRecent + 4u);   // an empty query: the first of each, and all four places
});

CB_TEST("app.palette.ties_keep_the_order_of_the_list", [] {
    const auto items = make_items(pl_tools(), {});
    const Result r = search(items, "i");
    for (std::size_t k = 1; k < r.picks.size(); ++k) {
        if (items[static_cast<std::size_t>(r.picks[k])].kind == Kind::Tool && items[static_cast<std::size_t>(r.picks[k - 1])].kind == Kind::Tool &&
            score("i", items[static_cast<std::size_t>(r.picks[k])].title) == score("i", items[static_cast<std::size_t>(r.picks[k - 1])].title))
            CB_CHECK(r.picks[k] > r.picks[k - 1]);
    }
});

CB_TEST("app.palette.the_arrows_move_and_stay_at_the_ends", [] {
    const auto items = make_items(pl_tools(), pl_recent());
    const Result r = search(items, "dis");
    CB_CHECK_EQ(move(r, 0, 1), 1);
    CB_CHECK_EQ(move(r, 2, 1), 2);
    CB_CHECK_EQ(move(r, 0, -1), 0);
    CB_CHECK_EQ(move(r, 7, 0), 2);   // a position past the end is brought back
    CB_CHECK_EQ(move(Result{}, 3, 1), 0);
});

}  // namespace
}  // namespace cb::app::palette
