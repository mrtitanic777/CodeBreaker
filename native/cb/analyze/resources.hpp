// cb/analyze/resources.hpp -- the resources code asks for.
//
//     const resources::Uses uses = resources::find_uses(image, graph, table);
//     const resources::Carried carried = ...;                  // what the module itself holds (the format readers)
//     const resources::Report report = resources::analyse(uses, carried);
//
// A module's resources are data; which function loads which one is in the code. Every call to a function that takes
// a resource identifier (LoadString, LoadIcon, DialogBoxParam, FindResource, CreateDialog, LoadMenu,
// LoadAccelerators, ...) is read for that argument: a small integer is an identifier, an address is a resource name.
// Each is then looked up in the module's own resource table -- `present` says whether the module really carries
// that dialog, that string, that icon -- so a reference to another module's resources shows as absent rather than
// being silently wrong.
//
// Calls that name a dialog control (GetDlgItem, SetDlgItemText, CheckDlgButton, ...) are listed by control
// identifier; matching those against the dialog templates needs the templates parsed, which is the resource
// extractor's work, not this module's.
//
// Where the identifier sits among the pushes differs by convention. 32-bit calls push the last argument first, so
// argument k is the k-th push back from the call. 16-bit Pascal calls push the first argument first and a far pointer
// as two words, segment then offset: the tables in the .cpp give, for each function, how many words back from the
// call the identifier's word is (and its segment word, which is 0 for an integer identifier).
//
// What a module carries is read by the format readers; strings are held in blocks of sixteen, so string N is present
// when block N/16+1 is. This module takes that as `Carried` and does not read files.
#pragma once

#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "cb/analyze/call_graph.hpp"
#include "cb/analyze/native.hpp"
#include "cb/analyze/strings.hpp"

namespace cb::analyze::resources {

// A resource identifier is either an integer (MAKEINTRESOURCE) or a name, upper-cased: the Win32 model, a closed
// union of the two.
using ResourceId = std::variant<std::int64_t, std::string>;

// The name of a standard resource type ("DIALOG" for 5), or "#<n>" for one that has no name here.
std::string type_name(std::int64_t id);

// One load the code makes: which function, at which instruction, through which call.
struct Load {
    Va function = 0;
    Va site = 0;
    std::string call;                     // the API without its A/W, lower case: "loadstring"
    std::optional<std::string> type;      // "STRING", "DIALOG", ... (none when a FindResource's type is not known)
    ResourceId id;
};

// A call that names a dialog control by its integer identifier.
struct ControlUse {
    Va function = 0;
    Va site = 0;
    std::string call;
    std::int64_t control = 0;
};

struct Uses {
    std::vector<Load> loads;
    std::vector<ControlUse> controls;
};

// The calls that load resources or name dialog controls, read from the arguments pushed before each.
Uses find_uses(Image& image, const call_graph::Graph& graph, const strings::Table& table);

// What a module carries: resource type name -> the integer identifiers and (upper-case) names it holds.
using Carried = std::map<std::string, std::set<ResourceId>>;

enum class Presence {
    Present,       // the module carries it
    Absent,        // it does not: another module's resource
    SystemStock,   // one of the system's stock icons and cursors
};

struct ResolvedLoad {
    Load load;
    Presence presence = Presence::Absent;
};

// Is each load's resource in the module itself?
std::vector<ResolvedLoad> resolve(const std::vector<Load>& loads, const Carried& carried);

struct Counts {
    std::int64_t resource_types = 0;
    std::int64_t resources_carried = 0;
    std::int64_t loads_read = 0;
    std::int64_t present_in_module = 0;
    std::int64_t absent_from_module = 0;
    std::int64_t system_stock = 0;
    std::int64_t distinct_resources_referenced = 0;
    std::int64_t functions_loading_resources = 0;
    std::int64_t dialog_control_uses = 0;
    std::int64_t distinct_controls = 0;
};

struct Report {
    Counts counts;
    std::vector<ResolvedLoad> loads;
    std::vector<ControlUse> controls;
    std::map<std::string, std::int64_t> carried;         // type -> how many the module carries
    std::vector<std::string> text;
};

Report analyse(const Uses& uses, const Carried& carried);

}  // namespace cb::analyze::resources
