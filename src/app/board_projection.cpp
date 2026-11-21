// app/board_projection.cpp -- see board_projection.hpp.
#include "app/board_projection.hpp"

#include <algorithm>
#include <cmath>
#include <deque>
#include <format>
#include <map>
#include <optional>
#include <set>
#include <tuple>
#include <unordered_set>

#include "core/text.hpp"

namespace cb::app::board {

namespace {

// The files every running Windows 95 desktop has loaded for the whole hour.
constexpr std::string_view kCore[] = {"kernel32.dll", "user32.dll", "gdi32.dll", "advapi32.dll", "shell32.dll", "comctl32.dll", "krnl386.exe", "user.exe", "gdi.exe", "explorer.exe", "shlwapi.dll"};

constexpr std::string_view kPump[] = {"getmessage", "peekmessage", "dispatchmessage", "translatemessage", "translateaccelerator", "sendmessage", "postmessage", "defwindowproc", "defdlgproc", "callwindowproc", "isdialogmessage", "msgwaitformultipleobjects"};
constexpr std::string_view kSync[] = {"entercriticalsection", "leavecriticalsection", "interlockedincrement", "interlockeddecrement", "interlockedexchange", "tlsgetvalue", "tlssetvalue", "waitforsingleobject", "waitformultipleobjects", "setevent", "resetevent", "getcurrentthreadid", "getcurrentprocess", "getcurrentthread", "getlasterror", "setlasterror", "gettickcount"};
constexpr std::string_view kMemory[] = {"heapalloc", "heapfree", "heaprealloc", "heapsize", "localalloc", "localfree", "localrealloc", "locallock", "localunlock", "globalalloc", "globalfree", "globallock", "globalunlock", "globalrealloc", "virtualalloc", "virtualfree", "lstrlen", "lstrcpy", "lstrcpyn", "lstrcat", "lstrcmp", "lstrcmpi", "multibytetowidechar", "widechartomultibyte", "wsprintf", "wvsprintf", "charnext", "charprev", "charupper", "charlower"};
constexpr std::string_view kPaint[] = {"beginpaint", "endpaint", "getdc", "releasedc", "bitblt", "stretchblt", "patblt", "textout", "exttextout", "drawtext", "selectobject", "deleteobject", "createcompatibledc", "deletedc", "setbkcolor", "setbkmode", "settextcolor", "fillrect", "invalidaterect", "updatewindow", "getsyscolor", "getsyscolorbrush", "drawicon", "movetoex", "lineto", "rectangle", "gettextextentpoint", "gettextmetrics", "setcursor"};
constexpr std::string_view kQuery[] = {"getwindowlong", "setwindowlong", "getparent", "getdlgitem", "getclientrect", "getwindowrect", "getsystemmetrics", "iswindow", "iswindowvisible", "iswindowenabled", "getfocus", "setfocus", "getkeystate", "getcursorpos", "screentoclient", "clienttoscreen", "getwindow", "getclasslong", "getprop", "setprop", "getcapture", "setcapture", "releasecapture", "enablewindow", "showwindow", "setwindowpos", "movewindow", "getwindowtext", "setwindowtext", "senddlgitemmessage", "getdlgitemtext", "setdlgitemtext", "killtimer", "settimer"};
constexpr std::string_view kFile[] = {"createfile", "readfile", "writefile", "closehandle", "setfilepointer", "getfilesize", "getfileattributes", "findfirstfile", "findnextfile", "findclose", "deletefile", "regopenkey", "regopenkeyex", "regclosekey", "regqueryvalue", "regqueryvalueex", "regsetvalue", "regsetvalueex", "regenumkey", "regenumkeyex", "regenumvalue", "regcreatekey", "regcreatekeyex", "getprivateprofilestring", "getprivateprofileint", "getprofilestring", "getprofileint", "writeprivateprofilestring"};
constexpr std::string_view kLifecycle[] = {"loadlibrary", "loadlibraryex", "freelibrary", "getprocaddress", "getmodulehandle", "getmodulefilename", "createwindow", "createwindowex", "destroywindow", "registerclass", "registerclassex", "unregisterclass", "createprocess", "exitprocess", "createthread", "exitthread", "dialogbox", "dialogboxparam", "createdialog", "createdialogparam", "enddialog", "loadstring", "loadicon", "loadcursor", "loadbitmap", "loadmenu", "loadaccelerators", "loadresource", "findresource", "lockresource", "messagebox", "messageboxex", "getversion", "getversionex", "getcommandline", "getstartupinfo", "getsysteminfo", "coinitialize", "couninitialize", "cocreateinstance"};

template <std::size_t N>
bool listed(const std::string_view (&names)[N], std::string_view name) {
    return std::find(std::begin(names), std::end(names), name) != std::end(names);
}

double fraction(double x) { return x - std::floor(x); }

std::uint64_t pair_key(std::uint32_t a, std::uint32_t b) { return (static_cast<std::uint64_t>(a) << 32) | b; }

std::string function_key(std::uint32_t module, std::string_view key) { return std::to_string(module) + ":" + std::string(key); }

}  // namespace

ApiKind api_kind(std::string_view function) {
    std::string name = text::lower(function);
    const auto kind_of = [](std::string_view n) -> std::optional<ApiKind> {
        if (listed(kPump, n)) return ApiKind::MessagePump;
        if (listed(kSync, n)) return ApiKind::Sync;
        if (listed(kMemory, n)) return ApiKind::Memory;
        if (listed(kPaint, n)) return ApiKind::Paint;
        if (listed(kQuery, n)) return ApiKind::Query;
        if (listed(kFile, n)) return ApiKind::FileRegistry;
        if (listed(kLifecycle, n)) return ApiKind::Lifecycle;
        return std::nullopt;
    };
    if (const auto exact = kind_of(name)) return *exact;
    if (name.size() > 1 && (name.back() == 'a' || name.back() == 'w'))   // the ANSI and wide forms of the same function
        if (const auto stem = kind_of(std::string_view(name).substr(0, name.size() - 1))) return *stem;
    return ApiKind::Other;
}

double api_rate(ApiKind kind) {
    switch (kind) {
        case ApiKind::MessagePump: return 3600;    // about once a second per site
        case ApiKind::Sync: return 1800;
        case ApiKind::Memory: return 900;
        case ApiKind::Paint: return 600;
        case ApiKind::Query: return 120;
        case ApiKind::FileRegistry: return 30;
        case ApiKind::Lifecycle: return 2;
        default: return 20;
    }
}

bool Projection::channel_measured(std::uint32_t from, std::uint32_t to) const { return measured_channels.contains(pair_key(from, to)); }

bool Projection::function_measured(std::uint32_t module, std::string_view key) const {
    const auto id = function_ids.find(function_key(module, key));
    return id != function_ids.end() && measured_functions.contains(id->second);
}

bool Projection::function_user_measured(std::uint32_t module, std::string_view key, std::uint32_t user) const {
    const auto id = function_ids.find(function_key(module, key));
    return id != function_ids.end() && measured_function_users.contains(pair_key(id->second, user));
}

double Projection::channel(std::uint32_t from, std::uint32_t to) const {
    const auto it = channels.find(pair_key(from, to));
    return it == channels.end() ? 0 : it->second;
}

double Projection::function(std::uint32_t module, std::string_view key) const {
    const auto it = function_ids.find(function_key(module, key));
    return it == function_ids.end() ? 0 : functions[it->second];
}

double Projection::function_user(std::uint32_t module, std::string_view key, std::uint32_t user) const {
    const auto id = function_ids.find(function_key(module, key));
    if (id == function_ids.end()) return 0;
    const auto it = function_users.find(pair_key(id->second, user));
    return it == function_users.end() ? 0 : it->second;
}

Projection build_projection(const Board& board, const Index& index, const Signals* signals, const Measured* measured) {
    Projection p;
    const Graph graph = build_graph(board, index, kLinkImports, Width::Files);
    const std::size_t modules = graph.modules.size();
    p.residency.assign(modules, kIdleResidency);
    std::deque<std::uint32_t> queue;
    const auto resident = [&](std::uint32_t module) {
        if (p.residency[module] < 1.0) {
            p.residency[module] = 1.0;
            queue.push_back(module);
        }
    };
    for (std::uint32_t m = 0; m < modules; ++m)
        if (std::find(std::begin(kCore), std::end(kCore), graph.modules[m].name) != std::end(kCore)) resident(m);
    if (signals != nullptr)
        for (const NodeSignal& l : signals->links)
            if (signals->signals[l.signal].type == SignalType::Autostart && l.direction == Direction::Registers && l.node < graph.module_of_card.size()) resident(graph.module_of_card[l.node]);
    while (!queue.empty()) {   // what a resident file imports is loaded with it
        const std::uint32_t m = queue.front();
        queue.pop_front();
        const double next = p.residency[m] * kResidencyDecay;
        for (const ModuleEdge& e : graph.modules[m].out) {
            if (next > p.residency[e.module]) {
                p.residency[e.module] = next;
                queue.push_back(e.module);
            }
        }
    }

    // ---- each use of each function; identical copies of a calling file count once, at the larger count
    std::vector<std::uint32_t> function_module;
    std::vector<ApiKind> function_kind;
    std::map<std::tuple<std::uint32_t, std::uint32_t, std::uint32_t>, double> uses;   // (function, user module, user's bytes)
    p.function_ids.reserve(index.exports.size());
    for (std::uint32_t s = 0; s < index.exports.size(); ++s) {
        const ExportedSymbol& symbol = index.exports[s];
        const std::uint32_t module = graph.module_of_card[symbol.card];
        const std::string key = symbol.name.empty() ? "#" + std::to_string(symbol.ordinal) : text::lower(symbol.name);
        const auto [id, fresh] = p.function_ids.try_emplace(function_key(module, key), static_cast<std::uint32_t>(function_module.size()));
        if (fresh) {
            function_module.push_back(module);
            function_kind.push_back(api_kind(key));
        }
        const double per_site = api_rate(api_kind(key));
        for (const ExportUse& use : index.uses_of(s)) {
            const std::uint32_t user = graph.module_of_card[use.from];
            if (user == module) continue;
            double& rate = uses[{id->second, user, index.nodes[use.from].group}];
            rate = std::max(rate, static_cast<double>(use.calls) * per_site * p.residency[user]);
        }
    }
    p.functions.assign(function_module.size(), 0);
    std::map<std::pair<std::uint32_t, std::uint32_t>, double> modeled, observed;   // (function, user)
    for (const auto& [key, rate] : uses) modeled[{std::get<0>(key), std::get<1>(key)}] += rate;
    if (measured != nullptr && measured->seconds > 0) {
        p.has_measured = true;
        p.measured_seconds = measured->seconds;
        std::unordered_map<std::string, std::uint32_t> module_id;
        module_id.reserve(modules);
        for (std::uint32_t m = 0; m < modules; ++m) module_id.emplace(graph.modules[m].name, m);
        for (const MeasuredFunction& f : measured->functions) {
            const auto target = module_id.find(f.module), user = module_id.find(f.user);
            if (target == module_id.end() || user == module_id.end()) continue;
            const double rate = measured->per_hour(f.count);
            p.measured_channels.insert(pair_key(user->second, target->second));
            const auto id = p.function_ids.find(function_key(target->second, text::lower(f.function)));
            if (id != p.function_ids.end()) observed[{id->second, user->second}] += rate;
            else p.channels[pair_key(user->second, target->second)] += rate;   // (a function of the installed copy that no card on the board exports)
        }
        // the model, scaled per kind of function so that it agrees with the trace where both have a number
        std::array<double, 8> seen{}, thought{};
        double all_seen = 0, all_thought = 0;
        for (const auto& [key, rate] : observed) {
            const auto it = modeled.find(key);
            if (it == modeled.end() || it->second <= 0) continue;
            const auto kind = static_cast<std::size_t>(function_kind[key.first]);
            seen[kind] += rate;
            thought[kind] += it->second;
            all_seen += rate;
            all_thought += it->second;
        }
        const double overall = all_thought > 0 ? all_seen / all_thought : 1;
        for (std::size_t k = 0; k < p.kind_scale.size(); ++k) p.kind_scale[k] = thought[k] > 0 ? seen[k] / thought[k] : overall;
    }
    for (const auto& [key, rate] : observed) {
        p.functions[key.first] += rate;
        p.function_users[pair_key(key.first, key.second)] += rate;
        p.channels[pair_key(key.second, function_module[key.first])] += rate;
        p.measured_functions.insert(key.first);
        p.measured_function_users.insert(pair_key(key.first, key.second));
    }
    for (const auto& [key, model_rate] : modeled) {
        if (observed.contains(key)) continue;
        const double rate = model_rate * p.kind_scale[static_cast<std::size_t>(function_kind[key.first])];
        p.functions[key.first] += rate;
        p.function_users[pair_key(key.first, key.second)] += rate;
        const std::uint64_t channel = pair_key(key.second, function_module[key.first]);
        if (!p.measured_channels.contains(channel)) p.channels[channel] += rate;   // (a channel the trace saw is what it saw)
    }
    for (const auto& [key, rate] : p.channels) p.total += rate;

    // ---- the hot path: the busiest channel, on along the busiest channel out of each end
    std::vector<std::vector<std::pair<std::uint32_t, double>>> out(modules), in(modules);
    std::uint64_t busiest = 0;
    double busiest_rate = 0;
    p.module_of_card = graph.module_of_card;
    p.activity.assign(modules, 0);
    for (const auto& [key, rate] : p.channels) {
        p.max_channel = std::max(p.max_channel, rate);
        if (p.has_measured && !p.measured_channels.contains(key)) continue;   // (with a trace, the hot path is what really ran)
        const auto from = static_cast<std::uint32_t>(key >> 32), to = static_cast<std::uint32_t>(key);
        p.activity[from] += rate;
        p.activity[to] += rate;
        out[from].emplace_back(to, rate);
        in[to].emplace_back(from, rate);
        if (rate > busiest_rate || (rate == busiest_rate && key < busiest)) {
            busiest_rate = rate;
            busiest = key;
        }
    }
    if (busiest_rate > 0) {
        std::deque<std::uint32_t> path{static_cast<std::uint32_t>(busiest >> 32), static_cast<std::uint32_t>(busiest)};
        std::unordered_set<std::uint32_t> seen(path.begin(), path.end());
        const auto best = [&](const std::vector<std::pair<std::uint32_t, double>>& edges) {
            std::optional<std::pair<std::uint32_t, double>> pick;
            for (const auto& e : edges)
                if (!seen.contains(e.first) && (!pick || e.second > pick->second || (e.second == pick->second && e.first < pick->first))) pick = e;
            if (pick && p.has_measured && pick->second < busiest_rate * kHotPathShare) pick.reset();   // (a trickle is not part of a hot path)
            return pick;
        };
        for (int hop = 0; hop < 2; ++hop) {
            if (const auto next = best(out[path.back()])) {
                seen.insert(next->first);
                path.push_back(next->first);
            }
            if (const auto before = best(in[path.front()])) {
                seen.insert(before->first);
                path.push_front(before->first);
            }
        }
        p.hot_path.assign(path.begin(), path.end());
    }
    return p;
}

double flow_intensity(double rate, double busiest) {
    if (rate <= 0 || busiest <= 0) return 0;
    return std::sqrt(std::min(1.0, rate / busiest));
}

std::vector<float> flow_dots(double rate, double busiest, double seconds, std::uint32_t seed) {
    const double intensity = flow_intensity(rate, busiest);
    if (intensity <= 0) return {};
    const int count = std::clamp(static_cast<int>(std::ceil(intensity * kMaxFlowDots)), 1, kMaxFlowDots);
    const double speed = 0.08 + 0.72 * intensity;   // lengths of the channel per second
    const double phase = fraction(static_cast<double>(seed) * 0.6180339887);
    std::vector<float> dots(static_cast<std::size_t>(count));
    for (int i = 0; i < count; ++i) dots[static_cast<std::size_t>(i)] = static_cast<float>(fraction(phase + seconds * speed + static_cast<double>(i) / count));
    return dots;
}

std::string rate_words(double rate) {
    if (rate >= 1e9) return std::format("{:.1f}B", rate / 1e9);
    if (rate >= 1e6) return std::format("{:.1f}M", rate / 1e6);
    if (rate >= 1e3) return std::format("{:.0f}K", rate / 1e3);
    return std::format("{:.0f}", rate);
}

}  // namespace cb::app::board
