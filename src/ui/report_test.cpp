// Tests of ui/report: the page says what the triage reading says, and nothing in a module's words can break out of the page.
#include "ui/report.hpp"

#include <nlohmann/json.hpp>

#include "core/testing.hpp"

namespace {

constexpr const char* kRptLz32 =
    "{\"module\": \"lz32.dll\", \"is\": {\"platform\": \"Windows 95 (32-bit)\", \"lane\": \"pe\", \"size\": 9936}, \"summary\": \"LZ decompression for Windows\", \"store\": {\"counts\": {\"verified\": 17, \"unverified\": 3, \"identified\": 19}}, \"made_by\": {\"compiler\": \"MSVC 2.0\", \"identified\": {\"thunk compiler\": 19}}, \"imports\": {\"KERNEL32.dll\": [\"CommonUnimpStub\", \"ThunkInitLSF\"]}, \"exports\": [\"LZOpenFileA\", \"LZClose\"], \"to_look_at\": [{\"addr\": \"0x1a2b\", \"name\": \"GetExpandedNameW\", \"class\": \"unverified\", \"why\": \"stack cleanup 4 vs 12\"}], \"source\": {\"folder\": \"src/lz32\", \"source\": [\"lz32.c\"]}}";
constexpr const char* kRptOdd = "{\"module\": \"<b>odd</b>.exe\", \"store\": {\"counts\": {}, \"say\": \"run measure first\"}}";

bool rpt_has(const std::string& page, std::string_view part) { return page.find(part) != std::string::npos; }

}  // namespace

CB_TEST("ui.report.the_page_says_what_came_back_and_what_is_left", [] {
    const cb::Json t = cb::Json::parse(kRptLz32);
    const std::string page = cb::ui::report::html(t);
    CB_CHECK(page.starts_with("<!doctype html>") && page.ends_with("</html>\n"));
    CB_CHECK(rpt_has(page, "<h1>lz32.dll</h1>") && rpt_has(page, "LZ decompression for Windows"));
    CB_CHECK(rpt_has(page, "<b>36 of 39</b> functions"));   // (verified + identified: proven or known library code)
    CB_CHECK(rpt_has(page, "GetExpandedNameW") && rpt_has(page, "stack cleanup 4 vs 12"));
    CB_CHECK(rpt_has(page, "MSVC 2.0") && rpt_has(page, "KERNEL32.dll (2)") && rpt_has(page, "Offers 2 functions"));
    CB_CHECK(!rpt_has(page, "<script") && !rpt_has(page, "http://") && !rpt_has(page, "https://"));   // (one file: nothing loaded from anywhere)
});

CB_TEST("ui.report.a_module_not_judged_yet_says_so_and_words_are_escaped", [] {
    const cb::Json t = cb::Json::parse(kRptOdd);
    const std::string page = cb::ui::report::html(t);
    CB_CHECK(rpt_has(page, "&lt;b&gt;odd&lt;/b&gt;.exe") && !rpt_has(page, "<b>odd</b>"));
    CB_CHECK(rpt_has(page, "No function of it has been judged yet. run measure first."));
});
