#include "cb/analyze/resources.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <unordered_map>

#include "cb/core/intmath.hpp"
#include "cb/core/text.hpp"

namespace cb::analyze::resources {

namespace {

constexpr std::array<std::pair<int, std::string_view>, 11> kTypes = {{{1, "CURSOR"},
                                                                      {2, "BITMAP"},
                                                                      {3, "ICON"},
                                                                      {4, "MENU"},
                                                                      {5, "DIALOG"},
                                                                      {6, "STRING"},
                                                                      {9, "ACCELERATOR"},
                                                                      {10, "RCDATA"},
                                                                      {12, "GROUP_CURSOR"},
                                                                      {14, "GROUP_ICON"},
                                                                      {16, "VERSION"}}};

constexpr int kNone = -1;

// A function that loads a resource: its type (none: an argument says), and where the identifier is among the
// arguments -- 32-bit: the argument number; 16-bit: words back from the call to the identifier, and to its segment.
struct Loader {
    std::string_view name;
    std::optional<std::string_view> type;
    int position32, position16, segment16;
};
constexpr std::array<Loader, 12> kLoaders = {{{"loadstring", "STRING", 1, 3, kNone},
                                              {"loadicon", "GROUP_ICON", 1, 0, 1},
                                              {"loadcursor", "GROUP_CURSOR", 1, 0, 1},
                                              {"loadbitmap", "BITMAP", 1, 0, 1},
                                              {"loadmenu", "MENU", 1, 0, 1},
                                              {"loadaccelerators", "ACCELERATOR", 1, 0, 1},
                                              {"dialogbox", "DIALOG", 1, 3, 4},
                                              {"createdialog", "DIALOG", 1, 3, 4},
                                              {"dialogboxparam", "DIALOG", 1, 5, 6},
                                              {"createdialogparam", "DIALOG", 1, 5, 6},
                                              {"findresource", std::nullopt, 1, 2, 3},
                                              {"loadimage", std::nullopt, 1, kNone, kNone}}};

// A function that names a dialog control: where its control identifier is (32-bit: argument number; 16-bit: words back).
struct Control {
    std::string_view name;
    int position32, position16;
};
constexpr std::array<Control, 8> kControls = {{{"getdlgitem", 1, 0},
                                               {"setdlgitemtext", 1, 2},
                                               {"getdlgitemtext", 1, 3},
                                               {"setdlgitemint", 1, 2},
                                               {"getdlgitemint", 1, 3},
                                               {"senddlgitemmessage", 1, 4},
                                               {"checkdlgbutton", 1, 1},
                                               {"isdlgbuttonchecked", 1, 0}}};

const Loader* loader(std::string_view name) {
    for (const Loader& l : kLoaders)
        if (l.name == name) return &l;
    return nullptr;
}
const Control* control(std::string_view name) {
    for (const Control& c : kControls)
        if (c.name == name) return &c;
    return nullptr;
}

// The import's name without its module, lower case, without its A/W; empty when no name here can be it.
std::string stem(std::string_view api) {
    const std::size_t bang = api.rfind('!');
    const std::string_view bare = bang == std::string_view::npos ? api : api.substr(bang + 1);
    if (std::any_of(bare.begin(), bare.end(), [](char c) { return static_cast<unsigned char>(c) >= 0x80; })) return {};
    std::string low = text::lower(bare);
    if (low.empty()) return low;
    const std::string cut = low.substr(0, low.size() - 1);
    if ((low.back() == 'a' || low.back() == 'w') && (loader(cut) != nullptr || control(cut) != nullptr)) return cut;
    return low;
}

// A name: the string the argument points at, upper-cased.
std::optional<ResourceId> as_name(const strings::Table& table, bool usable, Va va, bool use_text) {
    if (!usable || !use_text) return std::nullopt;
    const auto it = table.find(va);
    if (it == table.end()) return std::nullopt;
    return ResourceId(text::upper(it->second.text));
}

// The identifier the argument at `at` holds. `seg` is the argument holding its segment (16-bit); `use_text` allows an
// address to be read as a name.
std::optional<ResourceId> identifier(const Image& img, const strings::Table& table, const std::vector<Argument>& args,
                                     int at, int seg, bool use_text) {
    if (at == kNone || static_cast<std::size_t>(at) >= args.size() || args[static_cast<std::size_t>(at)].kind != Argument::Kind::Immediate)
        return std::nullopt;
    const Va value = args[static_cast<std::size_t>(at)].value;
    if (img.bits() == 32) {
        if (value < 0x10000) return ResourceId(value);
        return as_name(table, true, value, use_text);
    }
    if (seg == kNone) return ResourceId(value);
    if (static_cast<std::size_t>(seg) >= args.size()) return std::nullopt;
    const Argument& s = args[static_cast<std::size_t>(seg)];
    if (s.kind == Argument::Kind::Immediate && s.value == 0) return ResourceId(value);
    return as_name(table, s.kind == Argument::Kind::Register && s.reg == "ds", img.data_segment() + value, use_text);
}

}  // namespace

std::string type_name(std::int64_t id) {
    for (const auto& [n, name] : kTypes)
        if (n == id) return std::string(name);
    return std::format("#{}", id);
}

Uses find_uses(Image& img, const call_graph::Graph& graph, const strings::Table& table) {
    Uses out;
    for (Va f : std::vector<Va>(img.functions())) {
        const Function& fn = img.function(f);
        std::unordered_map<Va, int> index;
        for (std::size_t k = 0; k < fn.insns.size(); ++k) index.emplace(fn.insns[k].addr, static_cast<int>(k));
        const auto calls = graph.calls.find(f);
        if (calls == graph.calls.end()) continue;
        for (const call_graph::Call& c : calls->second) {
            if (!c.destination || !img.is_import(*c.destination)) continue;
            const std::string name = stem(img.import_name(*c.destination));
            const Loader* l = loader(name);
            const Control* ctl = control(name);
            if (l == nullptr && ctl == nullptr) continue;
            const auto at = index.find(c.site);
            const auto args = arguments(fn, at == index.end() ? 0 : at->second, 8);
            if (ctl != nullptr) {
                const auto id = identifier(img, table, args, img.bits() == 32 ? ctl->position32 : ctl->position16, kNone, false);
                if (id) {
                    if (const auto* v = std::get_if<std::int64_t>(&*id); v != nullptr && *v < 0x10000)
                        out.controls.push_back({f, c.site, name, *v});
                }
                continue;
            }
            const auto id = identifier(img, table, args, img.bits() == 32 ? l->position32 : l->position16, l->segment16, true);
            if (!id) continue;
            std::optional<std::string> type;
            if (l->type) type = std::string(*l->type);
            if (name == "findresource") {   // the type is an argument too
                const auto t = identifier(img, table, args, img.bits() == 32 ? 2 : 0, 1, true);
                type.reset();
                if (t) {
                    if (const auto* v = std::get_if<std::int64_t>(&*t)) type = type_name(*v);
                    else type = std::get<std::string>(*t);
                }
            }
            out.loads.push_back({f, c.site, name, std::move(type), *id});
        }
    }
    return out;
}

std::vector<ResolvedLoad> resolve(const std::vector<Load>& loads, const Carried& carried) {
    static const std::set<ResourceId> none;
    std::vector<ResolvedLoad> out;
    for (const Load& load : loads) {
        const auto it = carried.find(load.type.value_or(""));
        const std::set<ResourceId>& pool = it == carried.end() ? none : it->second;
        const auto* number = std::get_if<std::int64_t>(&load.id);
        Presence presence;
        if (load.type == "STRING" && number != nullptr) {
            presence = pool.contains(ResourceId(floor_div<std::int64_t>(*number, 16) + 1)) ? Presence::Present : Presence::Absent;   // blocks of sixteen
        } else if ((load.type == "GROUP_ICON" || load.type == "GROUP_CURSOR") && number != nullptr && *number >= 32000) {
            presence = Presence::SystemStock;
        } else {
            presence = pool.contains(load.id) ? Presence::Present : Presence::Absent;
        }
        out.push_back({load, presence});
    }
    return out;
}

Report analyse(const Uses& uses, const Carried& carried) {
    Report report;
    report.loads = resolve(uses.loads, carried);
    report.controls = uses.controls;
    for (const auto& [type, ids] : carried) report.carried[type] = static_cast<std::int64_t>(ids.size());

    std::map<std::optional<std::string>, std::set<ResourceId>> referenced;   // what the module carries and loads
    std::set<Va> loading;
    for (const ResolvedLoad& u : report.loads) {
        loading.insert(u.load.function);
        if (u.presence == Presence::Present) referenced[u.load.type].insert(u.load.id);
    }
    std::set<std::int64_t> controls;
    for (const ControlUse& c : uses.controls) controls.insert(c.control);

    Counts& c = report.counts;
    c.resource_types = static_cast<std::int64_t>(carried.size());
    for (const auto& [type, ids] : carried) c.resources_carried += static_cast<std::int64_t>(ids.size());
    c.loads_read = static_cast<std::int64_t>(report.loads.size());
    for (const ResolvedLoad& u : report.loads) {
        if (u.presence == Presence::Present) ++c.present_in_module;
        else if (u.presence == Presence::Absent) ++c.absent_from_module;
        else ++c.system_stock;
    }
    for (const auto& [type, ids] : referenced) c.distinct_resources_referenced += static_cast<std::int64_t>(ids.size());
    c.functions_loading_resources = static_cast<std::int64_t>(loading.size());
    c.dialog_control_uses = static_cast<std::int64_t>(uses.controls.size());
    c.distinct_controls = static_cast<std::int64_t>(controls.size());

    // for people: the identifiers loaded, by type
    std::map<std::string, std::set<ResourceId>> by_type;
    for (const ResolvedLoad& u : report.loads)
        by_type[u.load.type && !u.load.type->empty() ? *u.load.type : "?"].insert(u.load.id);
    for (const auto& [type, ids] : by_type) {
        std::vector<std::string> shown;
        for (const ResourceId& id : ids) shown.push_back(std::holds_alternative<std::int64_t>(id) ? std::to_string(std::get<std::int64_t>(id)) : std::get<std::string>(id));
        std::sort(shown.begin(), shown.end());   // as text, so 10 comes before 9
        if (shown.size() > 14) shown.resize(14);
        report.text.push_back(std::format("{}: {}", type, text::join(shown, ", ")));
    }
    return report;
}

}  // namespace cb::analyze::resources
