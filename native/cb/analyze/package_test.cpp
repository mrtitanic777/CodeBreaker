#include "cb/analyze/package.hpp"

#include <string>

#include "cb/core/testing.hpp"

namespace {

CB_TEST("analyze.package.the_overview_names_the_analyses", [] {
    const std::string text(cb::analyze::overview());
    for (const char* name : {"image", "native", "call_graph", "conventions", "globals", "strings", "structures", "classes",
                             "dispatch", "resources", "observe", "facts"})
        CB_CHECK(text.find(name) != std::string::npos);
    CB_CHECK(text.starts_with("What can be worked out ABOUT a module"));
    CB_CHECK(text.ends_with("about the whole module.\n"));
});

}  // namespace
