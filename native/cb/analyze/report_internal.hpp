// cb/analyze/report_internal.hpp -- what the pieces of the `analyze` report share (report.cpp, report_load.cpp, report_facts.cpp).
#pragma once

#include <filesystem>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <variant>

#include "cb/analyze/call_graph.hpp"
#include "cb/analyze/conventions.hpp"
#include "cb/analyze/dispatch.hpp"
#include "cb/analyze/globals.hpp"
#include "cb/analyze/observe.hpp"
#include "cb/analyze/report.hpp"
#include "cb/analyze/resources.hpp"
#include "cb/analyze/strings.hpp"
#include "cb/analyze/structures.hpp"
#include "cb/formats/detect.hpp"

namespace cb::analyze::report::detail {

struct Loaded {
    ImageKind kind = ImageKind::Pe;
    std::shared_ptr<Image> image;
};

// The module's file as an Image. UnsupportedError for a driver, a DOS program or a file that is none of the modules the analyses read.
Loaded load_module(const std::filesystem::path& path, const Environment& environment);

// The counts of each analysis under the names the report prints them by (report_facts.cpp).
Counts call_graph_counts(const call_graph::Counts& c);
Counts conventions_counts(const conventions::Counts& c);
Counts globals_counts(const globals::Counts& c);
Counts strings_counts(const strings::Counts& c);
Counts structures_counts(const structures::Counts& c);
Counts dispatch_counts(const dispatch::Counts& c);
Counts resources_counts(const resources::Counts& c);
Counts observe_counts(const observe::Counts& c);

// The rows of the fact table an analysis's facts come to, appended to the section.
void call_graph_rows(Section& s, const call_graph::Report& r, ImageKind kind);
void strings_rows(Section& s, const strings::Report& r, ImageKind kind);
void globals_rows(Section& s, const globals::Report& r, ImageKind kind);
void structures_rows(Section& s, const structures::Report& r, ImageKind kind);
void dispatch_rows(Section& s, const dispatch::Report& r, ImageKind kind);
void resources_rows(Section& s, const resources::Report& r, ImageKind kind);

// What a module carries of resources by type: the integer identifiers and the upper-case names it holds.
std::map<std::string, std::set<std::variant<std::int64_t, std::string>>> carried_resources(const std::filesystem::path& path, ImageKind kind);

}  // namespace cb::analyze::report::detail
