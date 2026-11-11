#include "cb/java/apimap.hpp"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/text.hpp"
#include "cb/java/textutil.hpp"

namespace cb::java::apimap {

namespace stdfs = std::filesystem;

// ---- Kind, Mapping --------------------------------------------------------------------------------------

namespace {
constexpr std::pair<Kind, std::string_view> kKinds[] = {
    {Kind::Direct, "direct"},     {Kind::SignatureAdapted, "signature-adapted"}, {Kind::Macro, "macro"},
    {Kind::Constant, "constant"}, {Kind::Shim, "shim"},                          {Kind::D3d9Helper, "d3d9-helper"},
    {Kind::Unsupported, "unsupported"},
};
}  // namespace

std::string_view name(Kind k) {
    for (const auto& [kind, text] : kKinds) {
        if (kind == k) return text;
    }
    return "direct";
}

std::optional<Kind> parse_kind(std::string_view text) {
    for (const auto& [kind, spelled] : kKinds) {
        if (spelled == text) return kind;
    }
    return std::nullopt;
}

std::string to_string(const Mapping& m) {
    return std::format("Mapping({}:{} {} {}:{}{} [{}])", m.src_lang, m.name, m.reverse ? "<-" : "->", m.dst_lang,
                       m.target, m.dialect ? "@" + *m.dialect : std::string(), name(m.kind));
}

// ---- keys -----------------------------------------------------------------------------------------------

std::vector<std::string> lookup_keys(std::string_view name) {
    name = text::trim(name);
    std::vector<std::string> keys = {std::string(name)};
    const std::vector<std::string_view> parts = text::split(name, '.');
    auto add = [&](std::string key) {
        if (std::find(keys.begin(), keys.end(), key) == keys.end()) keys.push_back(std::move(key));
    };
    if (parts.size() >= 2) add(std::string(parts[parts.size() - 2]) + "." + std::string(parts.back()));
    add(std::string(parts.back()));
    return keys;
}

std::string target_leaf(std::string_view target) {
    target = text::trim(target);
    // split on "::" or "."; the leaf is what follows the last separator
    std::size_t start = 0;
    for (std::size_t i = 0; i < target.size(); ++i) {
        if (target[i] == '.') {
            start = i + 1;
        } else if (target[i] == ':' && i + 1 < target.size() && target[i + 1] == ':') {
            start = i + 2;
            ++i;
        }
    }
    std::string_view leaf = target.substr(start);
    if (const auto paren = leaf.find('('); paren != std::string_view::npos) leaf = leaf.substr(0, paren);
    return std::string(text::trim(leaf));
}

// ---- JSON rows ------------------------------------------------------------------------------------------

namespace {

[[noreturn]] void bad(std::string_view file, const std::string& what) { throw ParseError(file, 0, 0, what); }

std::optional<std::string> opt_string(const Json& j, const char* key, std::string_view file) {
    auto it = j.find(key);
    if (it == j.end() || it->is_null()) return std::nullopt;
    if (!it->is_string()) bad(file, std::string("\"") + key + "\" is not a string");
    return it->get<std::string>();
}

std::optional<Kind> opt_kind(const Json& j, std::string_view file) {
    auto text = opt_string(j, "kind", file);
    if (!text) return std::nullopt;
    auto kind = parse_kind(*text);
    if (!kind) bad(file, "unknown mapping kind \"" + *text + "\"");
    return kind;
}

TargetMap parse_target_map(const Json& m, std::string_view file) {
    if (!m.is_object()) bad(file, "a mapping row is not an object");
    TargetMap row;
    row.target = opt_string(m, "target", file).value_or("");
    row.kind = opt_kind(m, file).value_or(Kind::Direct);
    row.note = opt_string(m, "note", file).value_or("");
    row.adapter = opt_string(m, "adapter", file);
    row.since = opt_string(m, "since", file);
    row.stmt = opt_string(m, "stmt", file);
    row.prec = opt_string(m, "prec", file);
    if (auto it = m.find("ordered"); it != m.end() && !it->is_null()) {
        if (!it->is_boolean()) bad(file, "\"ordered\" is not a boolean");
        row.ordered = it->get<bool>();
    }
    if (auto it = m.find("by_version"); it != m.end() && it->is_object()) {
        for (const auto& [dialect, o] : it->items()) {
            if (!o.is_object()) bad(file, "a by_version override is not an object");
            VersionOverride ov;
            ov.target = opt_string(o, "target", file);
            ov.kind = opt_kind(o, file);
            ov.note = opt_string(o, "note", file);
            ov.adapter = opt_string(o, "adapter", file);
            ov.since = opt_string(o, "since", file);
            row.by_version.emplace_back(dialect, std::move(ov));
        }
    }
    return row;
}

SymbolRecord parse_symbol(const Json& entry, std::string_view file) {
    if (!entry.is_object()) bad(file, "a symbol entry is not an object");
    SymbolRecord rec;
    rec.doc = opt_string(entry, "doc", file).value_or("");
    rec.signature = opt_string(entry, "signature", file).value_or("");
    if (auto it = entry.find("maps"); it != entry.end() && it->is_object()) {
        for (const auto& [dst, m] : it->items()) rec.maps.insert_or_assign(dst, parse_target_map(m, file));
    }
    return rec;
}

}  // namespace

// ---- Registry: population -------------------------------------------------------------------------------

void Registry::add_subsystem_doc(std::string sub_id, std::string language, std::string description) {
    subsystems_[std::move(sub_id)] = SubsystemDoc{std::move(language), std::move(description)};
}

void Registry::add_symbol(std::string_view src_lang, std::string_view name, SymbolRecord record) {
    const std::size_t index = records_.size();
    records_.push_back(std::move(record));
    const SymbolRecord& rec = records_.back();
    for (const std::string& key : lookup_keys(name)) {
        symbol_index_.emplace(std::make_pair(std::string(src_lang), key), index);   // the first claim on a key stays
    }
    for (const auto& [dst_lang, m] : rec.maps) {
        // index the base target AND every by_version override target for reverse lookup
        std::vector<std::pair<std::string, Kind>> variants = {{m.target, m.kind}};
        for (const auto& [dialect, o] : m.by_version) {
            if (o.target && !o.target->empty()) variants.emplace_back(*o.target, o.kind.value_or(m.kind));
        }
        for (const auto& [target, kind] : variants) {
            if (target.empty()) continue;
            Mapping reverse;
            reverse.name = target;
            reverse.src_lang = dst_lang;
            reverse.dst_lang = std::string(src_lang);
            reverse.target = std::string(name);
            reverse.kind = kind;
            reverse.note = m.note;
            reverse.subsystem = rec.subsystem;
            reverse.reverse = true;
            reverse_.emplace(std::make_tuple(dst_lang, target_leaf(target), std::string(src_lang)), std::move(reverse));
        }
    }
}

void Registry::add_legacy_equivalents(std::span<const LegacyEquivalent> rows) {
    for (const LegacyEquivalent& e : rows) {
        const Kind kind = e.status == "dead" ? Kind::Unsupported : Kind::Shim;   // "real" and "stub" are shims
        const std::string note(text::trim(e.note));
        SymbolRecord rec;
        rec.subsystem = "java.platform:" + e.subsystem;
        rec.doc = note;
        TargetMap row;
        row.target = e.xdk;
        row.kind = kind;
        row.note = std::string(text::trim(note + " [" + e.lib + "]"));
        rec.maps.insert_or_assign("xbox360", std::move(row));
        add_symbol("java", e.basename, std::move(rec));
    }
    add_subsystem_doc("java.platform", "java",
                      "Legacy per-class Java-platform -> Xbox 360 XDK intent map (bridged from platform_equivalents.EQUIV).");
}

Registry Registry::load(const stdfs::path& api_maps_dir) {
    Registry reg;
    std::error_code ec;
    if (!stdfs::is_directory(api_maps_dir, ec)) return reg;

    // 1. the language / API catalog
    const stdfs::path catalog = api_maps_dir / "languages.json";
    if (stdfs::is_regular_file(catalog, ec)) {
        try {
            const Json doc = json::load(catalog);
            if (auto langs = doc.find("languages"); langs != doc.end() && langs->is_object()) {
                for (const auto& [id, spec] : langs->items()) {
                    Language lang;
                    lang.kind = opt_string(spec, "kind", "languages.json").value_or("");
                    lang.desc = opt_string(spec, "desc", "languages.json").value_or("");
                    if (auto d = spec.find("dialects"); d != spec.end() && d->is_array()) {
                        for (const Json& v : *d) lang.dialects.push_back(v.get<std::string>());
                    }
                    reg.languages_.emplace(id, std::move(lang));
                }
            }
        } catch (const Error& e) {
            reg.problems_.push_back(std::string("languages.json: ") + e.what());
        } catch (const nlohmann::json::exception& e) {
            reg.problems_.push_back(std::string("languages.json: ") + e.what());
        }
    }

    // 2. every subsystem file (any *.json except the catalog), in name order
    std::vector<std::string> names;
    for (const auto& entry : stdfs::directory_iterator(api_maps_dir)) {
        const std::string name = fs::to_text(entry.path().filename());
        if (entry.is_regular_file() && name.ends_with(".json") && name != "languages.json") names.push_back(name);
    }
    std::sort(names.begin(), names.end());
    for (const std::string& file : names) {
        try {
            const Json doc = json::load(api_maps_dir / fs::from_text(file));
            if (!doc.is_object()) bad(file, "not a JSON object");
            std::string sub_id = file.substr(0, file.size() - 5);
            if (auto it = doc.find("subsystem"); it != doc.end() && it->is_string()) sub_id = it->get<std::string>();
            const std::string language =
                doc.contains("language") && doc["language"].is_string() ? doc["language"].get<std::string>()
                                                                        : std::string(text::split(sub_id, '.')[0]);
            // parse the whole file before adding any of it
            std::vector<std::pair<std::string, SymbolRecord>> parsed;
            if (auto syms = doc.find("symbols"); syms != doc.end() && syms->is_object()) {
                for (const auto& [name, entry] : syms->items()) {
                    SymbolRecord rec = parse_symbol(entry, file);
                    rec.subsystem = sub_id;
                    parsed.emplace_back(name, std::move(rec));
                }
            }
            reg.add_subsystem_doc(sub_id, language, opt_string(doc, "description", file).value_or(""));
            for (auto& [name, rec] : parsed) reg.add_symbol(language, name, std::move(rec));
        } catch (const Error& e) {
            reg.problems_.push_back(file + ": " + e.what());
        } catch (const nlohmann::json::exception& e) {
            reg.problems_.push_back(file + ": " + e.what());
        }
    }
    return reg;
}

// ---- Registry: queries ----------------------------------------------------------------------------------

const std::vector<std::string>& Registry::dialect_order(std::string_view lang) const {
    static const std::vector<std::string> kNone;
    auto it = languages_.find(std::string(lang));
    return it == languages_.end() ? kNone : it->second.dialects;
}

Selected Registry::select_version(const TargetMap& row, std::string_view dst_lang,
                                  std::optional<std::string_view> dialect) const {
    Selected base{row.target, row.kind, row.note, row.adapter, row.since, std::nullopt};
    if (row.by_version.empty() || !dialect || dialect->empty()) return base;

    auto find_override = [&](std::string_view id) -> const VersionOverride* {
        for (const auto& [dialect_id, o] : row.by_version) {
            if (dialect_id == id) return &o;
        }
        return nullptr;
    };
    auto apply = [&](const VersionOverride& o, const std::string& chosen) {
        return Selected{o.target.value_or(base.target), o.kind.value_or(base.kind), o.note.value_or(base.note),
                        o.adapter ? o.adapter : base.adapter, o.since ? o.since : std::optional<std::string>(chosen), chosen};
    };

    const std::vector<std::string>& order = dialect_order(dst_lang);
    const auto at = std::find(order.begin(), order.end(), *dialect);
    if (at != order.end()) {
        // walk newest -> oldest within the prefix up to the requested dialect
        for (auto it = at + 1; it != order.begin();) {
            --it;
            if (const VersionOverride* o = find_override(*it)) return apply(*o, *it);
        }
        return base;
    }
    // no ordering info: honour only an exact match
    if (const VersionOverride* o = find_override(*dialect)) return apply(*o, std::string(*dialect));
    return base;
}

const SymbolRecord* Registry::record(std::string_view src_lang, std::string_view key) const {
    auto it = symbol_index_.find(std::make_pair(std::string(src_lang), std::string(key)));
    return it == symbol_index_.end() ? nullptr : &records_[it->second];
}

std::optional<Mapping> Registry::resolve(std::string_view name, std::string_view src_lang, std::string_view dst_lang,
                                         std::optional<std::string_view> dialect) const {
    for (const std::string& key : lookup_keys(name)) {
        const SymbolRecord* rec = record(src_lang, key);
        if (rec == nullptr) continue;
        const TargetMap* row = rec->maps.find(std::string(dst_lang));
        if (row == nullptr) continue;
        Selected sel = select_version(*row, dst_lang, dialect);
        Mapping m;
        m.name = key;
        m.src_lang = std::string(src_lang);
        m.dst_lang = std::string(dst_lang);
        m.target = std::move(sel.target);
        m.kind = sel.kind;
        m.note = std::move(sel.note);
        m.adapter = std::move(sel.adapter);
        m.since = std::move(sel.since);
        m.subsystem = rec->subsystem;
        m.dialect = std::move(sel.dialect);
        return m;
    }
    return std::nullopt;
}

std::optional<Mapping> Registry::reverse(std::string_view target, std::string_view dst_lang,
                                         std::string_view src_lang) const {
    auto it = reverse_.find(std::make_tuple(std::string(dst_lang), target_leaf(target), std::string(src_lang)));
    if (it == reverse_.end()) return std::nullopt;
    return it->second;
}

bool Registry::known(std::string_view name, std::string_view src_lang) const {
    for (const std::string& key : lookup_keys(name)) {
        if (record(src_lang, key) != nullptr) return true;
    }
    return false;
}

Coverage Registry::coverage(const std::set<std::string>& names, std::string_view src_lang, std::string_view dst_lang,
                            std::optional<std::string_view> dialect) const {
    Coverage out;
    for (const std::string& name : names) {   // a std::set: name order
        std::optional<Mapping> m = resolve(name, src_lang, dst_lang, dialect);
        if (!m) {
            out.unmapped.push_back(name);
        } else if (m->kind == Kind::Unsupported) {
            out.unsupported.emplace_back(name, std::move(*m));
        } else {
            out.mapped.emplace_back(name, std::move(*m));
        }
    }
    return out;
}

std::vector<Registry::SymbolRow> Registry::symbols_in(std::string_view subsystem) const {
    // each record is indexed under several keys (qualified + leaf aliases): one row per record, under the most
    // qualified key (a dotted one beats an undotted one, then the longer beats the shorter)
    std::map<const SymbolRecord*, std::string> best;
    for (const auto& [lang_key, index] : symbol_index_) {
        const SymbolRecord& rec = records_[index];
        if (rec.subsystem != subsystem) continue;
        const std::string& key = lang_key.second;
        auto [it, inserted] = best.emplace(&rec, key);
        if (inserted) continue;
        const std::string& cur = it->second;
        if ((key.find('.') != std::string::npos && cur.find('.') == std::string::npos) || key.size() > cur.size()) it->second = key;
    }
    std::vector<SymbolRow> rows;
    for (const auto& [rec, key] : best) rows.push_back({key, rec});
    std::sort(rows.begin(), rows.end(), [](const SymbolRow& a, const SymbolRow& b) { return a.name < b.name; });
    return rows;
}

std::map<std::pair<std::string, std::string>, std::size_t> Registry::stats() const {
    // a record counts once however many keys index it, and not at all if every key it claims was taken first by
    // another record (a duplicate definition)
    std::set<std::size_t> reachable;
    for (const auto& [key, index] : symbol_index_) reachable.insert(index);
    std::map<std::pair<std::string, std::string>, std::size_t> pairs;
    for (std::size_t index : reachable) {
        const SymbolRecord& rec = records_[index];
        for (const auto& [dst, row] : rec.maps) ++pairs[{rec.subsystem, dst}];
    }
    return pairs;
}

std::vector<std::pair<std::pair<std::string, std::string>, const SymbolRecord*>> Registry::all_keys() const {
    std::vector<std::pair<std::pair<std::string, std::string>, const SymbolRecord*>> out;
    for (const auto& [key, index] : symbol_index_) out.emplace_back(key, &records_[index]);
    return out;
}

// ---- symbol extraction ----------------------------------------------------------------------------------

namespace {

bool is_upper(char c) { return c >= 'A' && c <= 'Z'; }
bool is_lower(char c) { return c >= 'a' && c <= 'z'; }
bool is_digit(char c) { return c >= '0' && c <= '9'; }
bool is_word(char c) { return is_upper(c) || is_lower(c) || is_digit(c) || c == '_'; }

// Where a `Holder` (an identifier starting with a capital, at a word boundary) at `i` ends, or npos.
std::size_t holder_end(std::string_view s, std::size_t i) {
    if (!is_upper(s[i]) || (i > 0 && is_word(s[i - 1]))) return std::string_view::npos;
    std::size_t j = i + 1;
    while (j < s.size() && is_word(s[j])) ++j;
    return j;
}

}  // namespace

std::set<std::string> extract_symbols(std::string_view s) {
    std::set<std::string> out;

    // Holder.method(   -- a capitalised holder, a lowercase-initial member, then an opening parenthesis
    for (std::size_t i = 0; i < s.size();) {
        const std::size_t h = holder_end(s, i);
        if (h == std::string_view::npos || h >= s.size() || s[h] != '.' || h + 1 >= s.size() || !is_lower(s[h + 1])) {
            ++i;
            continue;
        }
        std::size_t m = h + 2;
        while (m < s.size() && is_word(s[m])) ++m;
        std::size_t p = m;
        while (p < s.size() && text::is_space(s[p])) ++p;
        if (p < s.size() && s[p] == '(') {
            out.insert(std::string(s.substr(i, h - i)) + "." + std::string(s.substr(h + 1, m - h - 1)));
            i = p + 1;
        } else {
            ++i;
        }
    }

    // Holder.CONSTANT  -- the constant is CAPS/digits/underscore, at least three characters, and ends at a word boundary
    for (std::size_t i = 0; i < s.size();) {
        const std::size_t h = holder_end(s, i);
        if (h == std::string_view::npos || h >= s.size() || s[h] != '.' || h + 1 >= s.size() || !is_upper(s[h + 1])) {
            ++i;
            continue;
        }
        std::size_t c = h + 2;
        while (c < s.size() && (is_upper(s[c]) || is_digit(s[c]) || s[c] == '_')) ++c;
        const bool long_enough = c - (h + 1) >= 3;
        const bool at_boundary = c >= s.size() || !is_word(s[c]);
        if (long_enough && at_boundary) {
            out.insert(std::string(s.substr(i, h - i)) + "." + std::string(s.substr(h + 1, c - h - 1)));
            i = c;
        } else {
            ++i;
        }
    }
    return out;
}

std::set<std::string> extract_symbols_from_dir(const stdfs::path& java_dir) {
    std::set<std::string> out;
    std::error_code ec;
    if (!stdfs::is_directory(java_dir, ec)) return out;
    for (const auto& entry : stdfs::recursive_directory_iterator(java_dir, stdfs::directory_options::skip_permission_denied)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".java") continue;
        if (auto content = textutil::try_read_text(entry.path())) {
            std::set<std::string> found = extract_symbols(*content);
            out.insert(found.begin(), found.end());
        }
    }
    return out;
}

// ---- coverage report ------------------------------------------------------------------------------------

namespace {
std::string escape_pipes(std::string_view s) { return text::replace_all(s, "|", "\\|"); }
}  // namespace

std::string render_coverage_md(const Registry& registry, const std::set<std::string>& names, std::string_view src_lang,
                               std::string_view dst_lang, std::optional<std::string_view> dialect,
                               std::optional<std::string_view> title) {
    const Coverage cov = registry.coverage(names, src_lang, dst_lang, dialect);
    const std::string dia = dialect && !dialect->empty() ? std::format(" ({})", *dialect) : std::string();
    std::vector<std::string> L = {
        "# API Mapping Coverage",
        "",
        std::format("**{} -> {}{}**", src_lang, dst_lang, dia),
        "",
        std::string(title && !title->empty() ? *title : "Symbols referenced by the source, resolved against the API-mapping registry."),
        "",
        "| status | count |",
        "|---|---|",
        std::format("| mapped | {} |", cov.mapped.size()),
        std::format("| unsupported (no target) | {} |", cov.unsupported.size()),
        std::format("| unmapped (unknown / local) | {} |", cov.unmapped.size()),
        "",
    };
    if (!cov.mapped.empty()) {
        L.insert(L.end(), {"## Mapped", "", "| symbol | target | kind | note |", "|---|---|---|---|"});
        for (const auto& [name, m] : cov.mapped) {
            L.push_back(std::format("| `{}` | `{}` | {} | {} |", name, m.target, apimap::name(m.kind), escape_pipes(m.note)));
        }
        L.push_back("");
    }
    if (!cov.unsupported.empty()) {
        L.insert(L.end(), {"## Unsupported (registered, but no target equivalent)", "", "| symbol | note |", "|---|---|"});
        for (const auto& [name, m] : cov.unsupported) L.push_back(std::format("| `{}` | {} |", name, escape_pipes(m.note)));
        L.push_back("");
    }
    if (!cov.unmapped.empty()) {
        L.insert(L.end(), {"## Unmapped (unknown to the registry -- mostly the source's own classes)", ""});
        for (const std::string& n : cov.unmapped) L.push_back(std::format("- `{}`", n));
        L.push_back("");
    }
    return text::join(L, "\n");
}

}  // namespace cb::java::apimap


