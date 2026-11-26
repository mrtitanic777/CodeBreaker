// Tests of cb/symbols/proposals (the rules, over evidence made here) and of the proposal store over the knowledge base.
#include "symbols/proposals.hpp"

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "store/knowledge_proposals.hpp"

namespace sp = cb::symbols::proposals;
namespace stdfs = std::filesystem;
using cb::symbols::CallEvidence;
using cb::symbols::FunctionEvidence;

namespace {

FunctionEvidence forwarding(std::string import, bool purely) {
    FunctionEvidence e;
    e.address = 0x1000;
    e.calls.push_back({CallEvidence::Kind::Import, std::move(import), 0, 1});
    e.forwards_to_import = true;
    e.forwards_purely = purely;
    return e;
}

}  // namespace

CB_TEST("symbols.proposals.a_name_is_letters_digits_underscores_and_scopes", [] {
    CB_CHECK(sp::valid_name("CloseHandle_thunk"));
    CB_CHECK(sp::valid_name("_lclose"));
    CB_CHECK(sp::valid_name("CWnd::OnPaint"));
    CB_CHECK(!sp::valid_name(""));
    CB_CHECK(!sp::valid_name("9lives"));
    CB_CHECK(!sp::valid_name("two words"));
    CB_CHECK(!sp::valid_name("Bad:Name"));
    CB_CHECK(!sp::valid_name("Trailing::"));
});

CB_TEST("symbols.proposals.a_function_that_only_wraps_an_import_is_named_for_it", [] {
    const auto pure = sp::import_wrapper(forwarding("KERNEL32!CloseHandle", true));
    CB_CHECK(pure.has_value());
    CB_CHECK_EQ(pure->name, "CloseHandle_thunk");
    CB_CHECK_EQ(pure->rule, "import-wrapper");
    CB_CHECK_EQ(pure->confidence, 90);
    CB_CHECK(pure->state == cb::store::ProposalState::Proposed);
    CB_CHECK_EQ(pure->source, "proposal");
    const auto moved = sp::import_wrapper(forwarding("KERNEL!_lclose", false));
    CB_CHECK(moved.has_value());
    CB_CHECK_EQ(moved->name, "_lclose_thunk");   // (only a stdcall decoration is taken off)
    // a decorated stdcall name loses its decoration
    CB_CHECK_EQ(sp::import_wrapper(forwarding("KERNEL32!_Sleep@4", true))->name, "Sleep_thunk");
    // an import by number no table names gives no name, and neither does a function with more to it
    CB_CHECK(!sp::import_wrapper(forwarding("KERNEL32!#17", true)).has_value());
    FunctionEvidence more = forwarding("KERNEL32!CloseHandle", true);
    more.forwards_to_import = false;
    CB_CHECK(!sp::import_wrapper(more).has_value());
});

CB_TEST("symbols.proposals.a_message_that_opens_with_a_name_names_the_function_that_uses_it", [] {
    CB_CHECK_EQ(sp::name_in_message("LZInit: bad header"), "LZInit");
    CB_CHECK_EQ(sp::name_in_message("  [ReadBlock] short read"), "ReadBlock");
    CB_CHECK_EQ(sp::name_in_message("WriteBlock() failed"), "WriteBlock");
    CB_CHECK_EQ(sp::name_in_message("Read_Block: eof"), "Read_Block");
    // words of a sentence are not names
    CB_CHECK_EQ(sp::name_in_message("Error: out of memory"), "");
    CB_CHECK_EQ(sp::name_in_message("FATAL: stop"), "");
    CB_CHECK_EQ(sp::name_in_message("Cannot open the file"), "");
    CB_CHECK_EQ(sp::name_in_message("LZInit failed"), "");   // (could be the callee's name)
    CB_CHECK_EQ(sp::name_in_message("%s: %d"), "");

    FunctionEvidence e;
    e.strings = {"LZInit: bad header", "LZInit: no memory"};
    const auto p = sp::string_reference(e);
    CB_CHECK(p.has_value());
    CB_CHECK_EQ(p->name, "LZInit");
    CB_CHECK_EQ(p->confidence, 50);
    CB_CHECK_EQ(p->rule, "string-reference");
    // two different names: the strings are not its own
    e.strings = {"LZInit: bad header", "LZRead: eof"};
    CB_CHECK(!sp::string_reference(e).has_value());
    // a function that calls the import of that name is reporting on it
    e.strings = {"CloseHandle: failed"};
    e.calls.push_back({CallEvidence::Kind::Import, "KERNEL32!CloseHandle", 0, 1});
    CB_CHECK(!sp::string_reference(e).has_value());
});

CB_TEST("symbols.proposals.the_most_confident_rule_wins", [] {
    FunctionEvidence e = forwarding("KERNEL32!CloseHandle", true);
    e.strings = {"WidgetClose: failed"};
    const auto p = sp::by_rules(e);
    CB_CHECK(p.has_value());
    CB_CHECK_EQ(p->rule, "import-wrapper");
    CB_CHECK(!sp::by_rules(FunctionEvidence{}).has_value());
});

namespace {

struct ProposalWorld {
    cb::fs::ScopedTempDir dir{"cb-proposals"};
    cb::knowledge::Store kb{dir.path() / "db" / "knowledge.v2.sqlite", cb::knowledge::DataFolders::under(stdfs::path(CB_SOURCE_DIR).parent_path())};
    cb::store::KnowledgeProposals proposals{kb};

    static cb::store::Proposal make(std::string module, std::string key, std::string name) {
        cb::store::Proposal p;
        p.module = std::move(module);
        p.key = std::move(key);
        p.name = std::move(name);
        p.description = "Does a thing.";
        p.rule = "import-wrapper";
        p.confidence = 90;
        p.evidence = "only jumps to X!Y";
        p.digest = "0123456789abcdef";
        return p;
    }
};

}  // namespace

CB_TEST("symbols.proposals.a_proposal_is_kept_apart_from_the_names_and_reads_back", [] {
    ProposalWorld w;
    CB_CHECK(w.proposals.of_module("sha-a").empty());
    w.proposals.put(ProposalWorld::make("sha-a", "1:0116", "Close_thunk"));
    w.proposals.put(ProposalWorld::make("sha-a", "2:0000", "Open_thunk"));
    w.proposals.put(ProposalWorld::make("sha-b", "1:0116", "Elsewhere"));
    const auto got = w.proposals.get("sha-a", "1:0116");
    CB_CHECK(got.has_value());
    CB_CHECK_EQ(got->name, "Close_thunk");
    CB_CHECK_EQ(got->description, "Does a thing.");
    CB_CHECK_EQ(got->source, "proposal");
    CB_CHECK_EQ(got->confidence, 90);
    CB_CHECK_EQ(got->digest.value_or(""), "0123456789abcdef");
    CB_CHECK(got->state == cb::store::ProposalState::Proposed);
    CB_CHECK_EQ(w.proposals.of_module("sha-a").size(), std::size_t{2});
    CB_CHECK_EQ(w.proposals.of_module("sha-b").size(), std::size_t{1});
    // it is not a name: the body store has none, whoever asks
    CB_CHECK(!w.kb.name("0123456789abcdef").has_value());
});

CB_TEST("symbols.proposals.a_decision_is_kept_with_where_the_name_came_from", [] {
    ProposalWorld w;
    w.proposals.put(ProposalWorld::make("sha-a", "1:0116", "Close_thunk"));
    CB_CHECK(!w.proposals.set_state("sha-a", "9:9999", cb::store::ProposalState::Accepted));   // none there
    CB_CHECK(w.proposals.set_state("sha-a", "1:0116", cb::store::ProposalState::Accepted));
    const auto accepted = w.proposals.get("sha-a", "1:0116");
    CB_CHECK(accepted->state == cb::store::ProposalState::Accepted);
    CB_CHECK_EQ(accepted->source, "accepted proposal (import-wrapper)");
    CB_CHECK_EQ(accepted->description, "Does a thing.");   // (the record stays: it is the provenance)
    CB_CHECK(w.proposals.set_state("sha-a", "1:0116", cb::store::ProposalState::Rejected));
    CB_CHECK(w.proposals.get("sha-a", "1:0116")->state == cb::store::ProposalState::Rejected);
    CB_CHECK_EQ(std::string(cb::store::state_name(cb::store::ProposalState::Rejected)), "rejected");
    CB_CHECK(cb::store::parse_state("accepted") == cb::store::ProposalState::Accepted);
    CB_CHECK(!cb::store::parse_state("maybe").has_value());
});

CB_TEST("symbols.proposals.a_function_that_only_forwards_to_a_named_function_is_its_thunk", [] {
    FunctionEvidence e;
    e.address = 0x2000;
    e.forwards_to_function = 0x3000;
    const auto p = sp::function_forward(e, "ReadBlock");
    CB_CHECK(p.has_value());
    CB_CHECK_EQ(p->name, "ReadBlock_thunk");
    CB_CHECK_EQ(p->rule, "function-forward");
    CB_CHECK_EQ(p->confidence, 70);
    // a target with no name of its own, or one that is a thunk already, gives no name
    CB_CHECK(!sp::function_forward(e, "").has_value());
    CB_CHECK(!sp::function_forward(e, "sub_3000").has_value());
    CB_CHECK(!sp::function_forward(e, "ReadBlock_thunk").has_value());
    // the phase 1 rule set does not have this rule
    CB_CHECK(!sp::by_rules(e, "ReadBlock", true).has_value());
    CB_CHECK(sp::by_rules(e, "ReadBlock", false).has_value());
    e.forwards_to_function.reset();
    CB_CHECK(!sp::function_forward(e, "ReadBlock").has_value());
});

CB_TEST("symbols.proposals.names_compare_without_suffix_decoration_or_case", [] {
    CB_CHECK_EQ(sp::comparable("_Sleep@4"), "sleep");
    CB_CHECK_EQ(sp::comparable("CloseHandle_thunk"), "closehandle");
    CB_CHECK_EQ(sp::comparable("Sleep_thunk"), "sleep");
});

namespace {

FunctionEvidence handed(std::string import, int argument, std::map<int, std::int64_t> numbers = {}, std::map<int, std::string> texts = {}) {
    FunctionEvidence e;
    e.address = 0x4000;
    cb::symbols::HandedEvidence h;
    h.import_name = std::move(import);
    h.argument = argument;
    h.by = 0x5000;
    h.numbers = std::move(numbers);
    h.texts = std::move(texts);
    e.handed_to.push_back(std::move(h));
    return e;
}

}  // namespace

CB_TEST("symbols.proposals.a_function_the_api_calls_back_is_what_the_api_calls_that_argument", [] {
    const auto thread = sp::callback_role(handed("KERNEL32!CreateThread", 3));
    CB_CHECK(thread.has_value());
    CB_CHECK_EQ(thread->name, "ThreadProc");
    CB_CHECK_EQ(thread->rule, "callback-role");
    CB_CHECK_EQ(thread->confidence, 80);
    CB_CHECK_EQ(thread->evidence, "argument 3 of KERNEL32!CreateThread");
    CB_CHECK_EQ(sp::callback_role(handed("USER32!EnumChildWindows", 2))->name, "EnumChildProc");
    CB_CHECK_EQ(sp::callback_role(handed("GDI32!EnumFontFamiliesA", 3))->name, "EnumFontFamProc");
    // the wrong argument of a callback's API says nothing
    CB_CHECK(!sp::callback_role(handed("KERNEL32!CreateThread", 4)).has_value());
    CB_CHECK(!sp::callback_role(handed("KERNEL32!CloseHandle", 1)).has_value());
    // a hook is named for its WH_* number
    CB_CHECK_EQ(sp::callback_role(handed("USER32!SetWindowsHookExA", 2, {{1, 2}}))->name, "KeyboardProc");
    CB_CHECK_EQ(sp::callback_role(handed("USER32!SetWindowsHookExA", 2, {{1, -1}}))->name, "MessageProc");
    CB_CHECK_EQ(sp::callback_role(handed("USER32!SetWindowsHookExA", 2))->name, "HookProc");
    // subclassing is SetWindowLong with GWL_WNDPROC only
    CB_CHECK_EQ(sp::callback_role(handed("USER32!SetWindowLongA", 3, {{2, -4}}))->name, "SubclassWndProc");
    CB_CHECK(!sp::callback_role(handed("USER32!SetWindowLongA", 3, {{2, -21}})).has_value());
    // a dialog procedure by its template
    CB_CHECK_EQ(sp::callback_role(handed("USER32!DialogBoxParamA", 4, {{2, 104}}))->name, "Dialog104Proc");
    CB_CHECK_EQ(sp::callback_role(handed("USER32!DialogBoxParamW", 4, {}, {{2, "about box"}}))->name, "aboutBoxDlgProc");
    CB_CHECK_EQ(sp::callback_role(handed("USER32!CreateDialogIndirectParamA", 4))->name, "DialogProc");
    // ... and by the title of the template the module carries, when it has one
    FunctionEvidence run = handed("USER32!DialogBoxParamA", 4, {{2, 1003}});
    run.handed_to[0].caption = "Run";
    CB_CHECK_EQ(sp::callback_role(run)->name, "RunDlgProc");
    run.handed_to[0].caption = "&Shut down Windows";
    CB_CHECK_EQ(sp::callback_role(run)->name, "ShutDownWindowsDlgProc");
    run.handed_to[0].caption = "%s Properties";
    CB_CHECK_EQ(sp::callback_role(run)->name, "PropertiesDlgProc");
    run.handed_to[0].caption = "1";
    CB_CHECK_EQ(sp::callback_role(run)->name, "Dialog1003Proc");
});

CB_TEST("symbols.proposals.a_window_procedure_is_named_for_its_class", [] {
    FunctionEvidence e = handed("USER32!RegisterClassA", 0, {}, {{0, "Shell_TrayWnd"}});
    e.handed_to[0].field = "lpfnWndProc";
    const auto p = sp::callback_role(e);
    CB_CHECK(p.has_value());
    CB_CHECK_EQ(p->name, "Shell_TrayWndProc");
    CB_CHECK_EQ(p->confidence, 85);
    e.handed_to[0].texts = {{0, "Progman"}};
    CB_CHECK_EQ(sp::callback_role(e)->name, "ProgmanWndProc");
    e.handed_to[0].texts.clear();
    CB_CHECK_EQ(sp::callback_role(e)->name, "WndProc");
    CB_CHECK_EQ(sp::callback_role(e)->confidence, 75);
    // two places that disagree on what it is say nothing; two that agree are one
    FunctionEvidence both = e;
    both.handed_to.push_back(handed("KERNEL32!CreateThread", 3).handed_to[0]);
    CB_CHECK(!sp::callback_role(both).has_value());
    both.handed_to[1] = e.handed_to[0];
    both.handed_to[1].by = 0x6000;
    CB_CHECK_EQ(sp::callback_role(both)->name, "WndProc");
    // the phase 1 rule set does not have this rule
    CB_CHECK(!sp::by_rules(e, "", true).has_value());
    CB_CHECK_EQ(sp::by_rules(e, "", false)->rule, "callback-role");
});
