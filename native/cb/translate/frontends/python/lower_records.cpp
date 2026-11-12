// cb-unity: alone (file-local helpers named for this module)
// What every container site of the program is in C++: a record struct (named), a dict, a list, a set.
#include <algorithm>
#include <unordered_map>

#include "cb/core/text.hpp"
#include "cb/translate/frontends/python/lower.hpp"

namespace cb::translate::frontends::python::lower {

namespace pt = pytypes;

namespace {

// A word as a type name: each run of characters that are not letters or digits separates words, each word capitalised.
std::string camel_case(std::string_view s) {
    std::string out;
    bool start = true;
    for (const char c : s) {
        const bool alnum = (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
        if (!alnum) {
            start = true;
            continue;
        }
        out += start ? static_cast<char>(c >= 'a' && c <= 'z' ? c - 'a' + 'A' : c) : c;
        start = false;
    }
    return out.empty() ? "Record" : out;
}

// The singular of an English plural, as far as the names in a program need it.
std::string singular_of(std::string_view w) {
    const auto ends = [&](std::string_view suffix) { return w.ends_with(suffix); };
    if (ends("ies")) return std::string(w.substr(0, w.size() - 3)) + "y";
    if (ends("sses") || ends("xes")) return std::string(w.substr(0, w.size() - 2));
    if (ends("s") && !ends("ss")) return std::string(w.substr(0, w.size() - 1));
    return std::string(w);
}

// The verbs a function's name starts with that say how it makes what it returns.
constexpr std::string_view kVerbs[] = {"parse_", "read_", "get_", "make_", "build_", "load_", "collect_", "find_", "scan_", "compute_"};
// Names that say nothing about what a record is.
bool is_generic_name(std::string_view lower_name) {
    static constexpr std::string_view generic[] = {"out", "r", "d", "e", "x", "h", "s", "o", "v", "res", "result", "info", "rec", "item", "entry", "m",
                                                   "p",   "n", "it", "data", "obj", "tmp", "row", "c", "f", "t"};
    return std::find(std::begin(generic), std::end(generic), lower_name) != std::end(generic);
}

}  // namespace

// Whether a function's return type holds the container of root `s` (the container itself, or items of one that does, down
// four levels).
static bool type_holds(Program& p, const Type& t, SiteId s, int depth = 0) {
    if (depth > 4) return false;
    if (t.kind == pt::Kind::Dict) return t.site && p.find(*t.site) == s;
    if (t.kind == pt::Kind::List || t.kind == pt::Kind::Set) return t.site && type_holds(p, p.site(p.find(*t.site)).elem, s, depth + 1);
    return std::any_of(t.args.begin(), t.args.end(), [&](const Type& a) { return type_holds(p, a, s, depth + 1); });
}

// The record's struct name: from what it holds, the function that makes it and the name it was given in the source.
static std::string record_name(Program& p, SiteId s, const std::optional<std::string>& function) {
    std::vector<std::string> hints;
    for (const std::string& h : p.site(s).hints)
        if (!is_generic_name(text::lower(h)) && h.size() > 2) hints.push_back(h);
    std::optional<std::string> stem;
    if (function) {
        std::string fname = function->substr(function->rfind('.') == std::string::npos ? 0 : function->rfind('.') + 1);
        fname.erase(0, fname.find_first_not_of('_') == std::string::npos ? fname.size() : fname.find_first_not_of('_'));
        for (const std::string_view verb : kVerbs)
            if (fname.starts_with(verb)) fname.erase(0, verb.size());
        if (!fname.empty() && fname != "_init__") stem = camel_case(singular_of(fname));
        const auto returned = p.returns.find(*function);
        const bool flows_out = returned != p.returns.end() && type_holds(p, returned->second, s);
        if (flows_out && stem) return *stem;
        if (stem && !hints.empty()) return *stem + camel_case(singular_of(hints.front()));
        if (stem && !p.site(s).hints.empty()) {
            const std::string& h = p.site(s).hints.front();
            return h.size() > 2 ? *stem + camel_case(h) : *stem + "Record";
        }
    }
    if (!hints.empty()) return camel_case(singular_of(hints.front()));
    return stem.value_or("Anon") + "Record";
}

Records::Records(Program& program) : p_(program) {
    // module -> line -> the function whose lines they are; the innermost function (the shortest span) where several hold a line
    std::unordered_map<std::string, std::unordered_map<std::uint32_t, std::string>> function_of_line;
    for (const auto& [qname, f] : p_.fns) {
        auto& lines = function_of_line[f->module->name];
        const std::uint32_t first = static_cast<std::uint32_t>(f->node->pos.lineno);
        const std::uint32_t last = static_cast<std::uint32_t>(f->node->pos.end_lineno ? f->node->pos.end_lineno : f->node->pos.lineno);
        for (std::uint32_t line = first; line <= last; ++line) lines.try_emplace(line, f->qname);
    }
    std::vector<const program::Fn*> by_size;
    for (const auto& [qname, f] : p_.fns) by_size.push_back(f.get());
    std::stable_sort(by_size.begin(), by_size.end(), [](const program::Fn* a, const program::Fn* b) { return a->lines > b->lines; });
    for (const program::Fn* f : by_size) {
        auto& lines = function_of_line[f->module->name];
        const std::uint32_t last = static_cast<std::uint32_t>(f->node->pos.end_lineno ? f->node->pos.end_lineno : f->node->pos.lineno);
        for (std::uint32_t line = static_cast<std::uint32_t>(f->node->pos.lineno); line <= last; ++line) lines[line] = f->qname;
    }
    const auto function_at = [&](const std::string& module, std::uint32_t line) -> std::optional<std::string> {
        const auto m = function_of_line.find(module);
        if (m == function_of_line.end()) return std::nullopt;
        const auto l = m->second.find(line);
        return l == m->second.end() ? std::nullopt : std::optional<std::string>(l->second);
    };

    std::map<SiteId, std::vector<SiteId>> members;   // root -> its sites, ascending
    for (SiteId x = 1; x <= p_.site_count(); ++x) members[p_.find(x)].push_back(x);
    std::map<std::pair<std::string, std::string>, int> used;   // (module, name) -> how many records have it
    for (const auto& [root, sites] : members) {
        if (!p_.is_record(root)) continue;
        // the record is declared where a function that returns it makes it
        std::optional<SiteId> home;
        for (const SiteId m : sites) {
            const auto& where = p_.site(m).where;
            const auto fq = function_at(where ? where->module : "?", where ? where->line : 0);
            if (!fq) continue;
            const auto returned = p_.returns.find(*fq);
            if (type_holds(p_, returned != p_.returns.end() ? returned->second : pt::bottom(), root)) {
                home = m;
                break;
            }
        }
        if (!home) home = sites.front();
        homes_[root] = *home;
        const auto& where = p_.site(*home).where;
        const std::string module = where ? where->module : "?";
        const auto owner = function_at(module, where ? where->line : 0);
        owners_[root] = owner;
        modules_[root] = module;
        std::string name = record_name(p_, root, owner);
        const int count = ++used[{module, name}];
        if (count > 1) name += std::to_string(count);   // two records of one name in one module are told apart by number
        names_[root] = name;
    }
}

std::vector<RecordField> Records::fields(SiteId root) {
    root = p_.find(root);
    const program::Site& site = p_.site(root);
    const auto required = [&](const std::string& key) { return site.required && site.required->contains(key); };
    std::vector<std::string> order;
    // the keys in the order the record's own site writes them, then the rest
    const auto home = homes_.find(root);
    if (home != homes_.end() && p_.site(home->second).node != nullptr) {
        const auto keys = p_.literal_keys.find(p_.site(home->second).node);
        if (keys != p_.literal_keys.end())
            for (const std::string& k : keys->second)
                if (site.fields.contains(k)) order.push_back(k);
    }
    for (const auto& [key, type] : site.fields)
        if (std::find(order.begin(), order.end(), key) == order.end()) order.push_back(key);
    std::vector<RecordField> out;
    for (const std::string& key : order) out.push_back({key, site.fields.at(key), required(key)});
    return out;
}

bool Records::is_ref(SiteId root) { return p_.root(root).mutated; }

}  // namespace cb::translate::frontends::python::lower
