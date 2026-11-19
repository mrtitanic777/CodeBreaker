// cb-unity: alone
// cb/ui/mcp_tools.cpp -- the MCP server's tools (see mcp_server.hpp): what each reads, how it pages and cuts, what it says when it cannot.
#include <algorithm>
#include <ctime>
#include <format>
#include <fstream>
#include <nlohmann/json.hpp>
#include <set>

#include "cb/core/fs.hpp"
#include "cb/core/sys.hpp"
#include "cb/core/json.hpp"
#include "cb/core/text.hpp"
#include "cb/symbols/demangle.hpp"
#include "cb/verify/verdicts.hpp"
#include "cb/ui/mcp_impl.hpp"

namespace cb::ui::mcp {

namespace sfs = std::filesystem;

namespace {

using nlohmann::ordered_json;

// The commands that are interfaces and servers, not jobs.
constexpr std::array<std::string_view, 3> kInteractive = {"app", "progress", "mcp"};

std::string py_repr(std::string_view s) {   // the quoting a repr-style string gets: ' unless the text holds one and no "
    const bool single = s.find('\'') == std::string_view::npos || s.find('"') != std::string_view::npos;
    std::string out(1, single ? '\'' : '"');
    for (const char c : s) {
        if (c == '\\') out += "\\\\";
        else if (c == '\n') out += "\\n";
        else if (c == '\r') out += "\\r";
        else if (c == '\t') out += "\\t";
        else if (c == '\'' && single) out += "\\'";
        else out += c;
    }
    out += single ? '\'' : '"';
    return out;
}

std::string str_of(const Json& a, const char* key) { return a.at(key).get<std::string>(); }
std::int64_t int_of(const Json& a, const char* key) { return a.at(key).get<std::int64_t>(); }
bool bool_of(const Json& a, const char* key) { return a.at(key).get<bool>(); }

// The text of an object's member, or "".
std::string text_member(const Json& o, const char* key) {
    if (!o.is_object()) return "";
    const auto it = o.find(key);
    return it != o.end() && it->is_string() ? it->get<std::string>() : std::string();
}
Json member(const Json& o, const char* key) {   // `.get(key)`: null when it is not there
    if (!o.is_object()) return nullptr;
    const auto it = o.find(key);
    return it == o.end() ? Json(nullptr) : *it;
}
bool truthy(const Json& v) {
    if (v.is_null()) return false;
    if (v.is_boolean()) return v.get<bool>();
    if (v.is_number_integer()) return v.get<std::int64_t>() != 0;
    if (v.is_number_float()) return v.get<double>() != 0;
    if (v.is_string()) return !v.get<std::string>().empty();
    return !v.empty();
}
std::string lowered(std::string_view s) { return text::lower(s); }

std::string hex_text(std::int64_t v) { return std::format("0x{:x}", v); }
Json hexed(const Json& v) { return v.is_number_integer() ? Json(hex_text(v.get<std::int64_t>())) : v; }   // `"0x%x" % v if int else v`

std::string address_of(const Json& v) {   // "%x" % a if an int, else str(a)
    if (v.is_number_integer()) return std::format("{:x}", v.get<std::int64_t>());
    return v.is_string() ? v.get<std::string>() : v.dump();
}

std::string strip_text(std::string_view s) { return std::string(text::trim(s)); }

// The real path, for the purposes of containment: an absolute path, symbolic links resolved where the file exists.
sfs::path real_path(const sfs::path& p) {
    std::error_code ec;
    sfs::path out = sfs::weakly_canonical(p, ec);
    return ec ? sfs::absolute(p) : out;
}
bool inside(const sfs::path& path, const sfs::path& root) {   // a Windows path compares without regard to case
    const std::string p = lowered(path.generic_string()), r = lowered(root.generic_string());
    return p == r || (p.size() > r.size() && p.starts_with(r) && (r.ends_with('/') || p[r.size()] == '/'));
}

bool is_interactive(std::string_view command) { return std::find(kInteractive.begin(), kInteractive.end(), command) != kInteractive.end(); }

std::string file_kind(const std::string& rel) {   // "built" (made by a build, or in a folder a build writes), "notes", or "source"
    static const std::set<std::string> built = {".obj", ".o",   ".lib", ".exp", ".res", ".map", ".dll", ".exe", ".drv", ".vxd", ".386",
                                                ".sym", ".log", ".pdb", ".ilk", ".err", ".lst", ".com", ".sys", ".bin"};
    const std::string low = lowered(rel);   // (named: the pieces are views into it)
    const std::vector<std::string_view> parts = text::split(low, '/');
    const std::string name(parts.back());
    const std::size_t dot = name.rfind('.');
    const std::string ext = dot == std::string::npos || dot == 0 ? "" : name.substr(dot);
    bool in_build_folder = false;
    for (std::size_t i = 0; i + 1 < parts.size(); ++i) {
        const std::string_view p = parts[i];
        if (p.starts_with("_build") || p == "build" || p == "out" || p == "obj") in_build_folder = true;
    }
    if (built.contains(ext) || in_build_folder) return "built";
    return ext == ".md" || ext == ".txt" ? "notes" : "source";
}

Json read_json_or_empty(const sfs::path& p) {   // (except (OSError, ValueError): {})
    try {
        return json::load(p);
    } catch (const Error&) {
        return Json::object();
    }
}

}  // namespace

// ---- text -------------------------------------------------------------------------------------------------------------------------------

std::size_t utf8_length(std::string_view text) {
    std::size_t n = 0;
    for (const char c : text)
        if ((static_cast<unsigned char>(c) & 0xC0) != 0x80) ++n;
    return n;
}

std::string utf8_slice(std::string_view text, std::size_t from, std::size_t count) {
    std::size_t at = 0, index = 0;
    auto step = [&] {   // to the start of the next code point
        ++at;
        while (at < text.size() && (static_cast<unsigned char>(text[at]) & 0xC0) == 0x80) ++at;
    };
    while (at < text.size() && index < from) {
        step();
        ++index;
    }
    const std::size_t begin = at;
    for (std::size_t taken = 0; at < text.size() && taken < count; ++taken) step();
    return std::string(text.substr(begin, at - begin));
}

// ---- the helpers every tool shares ---------------------------------------------------------------------------------------------

std::string normalize_address(std::string_view text) {
    std::string a = lowered(strip_text(text));
    if (a.starts_with("0x")) return a.substr(2);
    if (a.size() > 1 && a.ends_with('h')) {
        while (a.ends_with('h')) a.pop_back();
        return a;
    }
    return a;
}

Json page(const Json& rows, std::int64_t offset, std::int64_t limit, std::int64_t most) {
    offset = std::max<std::int64_t>(0, offset);
    limit = std::max<std::int64_t>(1, std::min(limit, most));
    const std::int64_t total = static_cast<std::int64_t>(rows.size());
    const std::int64_t end = std::min(total, offset + limit);
    Json out = Json::object();
    out["total"] = total;
    out["offset"] = offset;
    out["count"] = std::max<std::int64_t>(0, end - offset);
    Json part = Json::array();
    for (std::int64_t i = offset; i < end; ++i) part.push_back(rows[static_cast<std::size_t>(i)]);
    out["rows"] = std::move(part);
    out["next"] = offset + limit < total ? Json(offset + limit) : Json(nullptr);
    return out;
}

Json fit(const Json& value, std::size_t most) {
    if (compact_length(value) <= most) return value;
    if (value.is_object()) {
        Json keep = Json::object(), parts = Json::object();
        for (const auto& [k, v] : value.items()) {
            const std::size_t n = compact_length(v);
            if (n < 2000) keep[k] = v;
            else parts[k] = n;
        }
        Json cut = Json::object();
        cut["say"] = "too long for one answer; the long parts are named with their sizes. Ask for one part (a section, or offset/limit).";
        cut["parts"] = std::move(parts);
        keep["_cut"] = std::move(cut);
        return keep;
    }
    // A list or a text: the start of its JSON.
    std::string text = answer_text(value);
    Json out = Json::object();
    out["_cut"] = std::format("too long for one answer ({} characters)", compact_length(value));
    out["start"] = utf8_slice(text, 0, most);
    return out;
}

std::filesystem::path Server::Impl::path_of(std::string_view module_in) {
    std::string module = strip_text(module_in);
    while (!module.empty() && module.front() == '"') module.erase(module.begin());
    while (!module.empty() && module.back() == '"') module.pop_back();
    if (module.empty()) throw UsageError("name a module (a corpus name such as lz32.dll, or a path)");
    std::error_code ec;
    if (sfs::is_directory(sfs::path(module), ec)) return sfs::path(module);
    std::optional<sfs::path> found = s.retail ? s.retail(module) : std::nullopt;
    if (!found) found = service.stored_path(module);
    if (!found) throw UsageError(std::format("no module {}: not a file, not in the corpus ({}), not in the store", py_repr(module), s.corpus_root.string()));
    return *found;
}

Json Server::Impl::not_ready(std::string_view what, std::int64_t seconds) const {
    Json out = Json::object();
    out["status"] = "computing";
    out["what"] = std::string(what);
    out["seconds_so_far"] = seconds;
    out["say"] = "still being worked out in the background; call again in a little while";
    return out;
}

std::optional<Json> Server::Impl::store_view(const std::string& module) {
    // The service's `functions` for a module, remembered a few seconds (it reads the ledger).
    const Slow::Result r = slow.get("functions\x01" + module, [&] { return link.call("functions", Json{{"module", module}}); }, std::chrono::seconds{15},
                                    std::chrono::seconds{120});
    if (!r.value || r.value->is_null()) return std::nullopt;
    return r.value;
}

std::map<std::string, std::string> Server::Impl::identified_of(const sfs::path& p) {
    // {addr hex: library} of a module's functions a runtime library or tool made (identify).
    std::map<std::string, std::string> out;
    if (!s.identified) return out;
    const Slow::Result r = slow.get("identify\x01" + p.string(), [&] { return s.identified(p); }, std::chrono::seconds{3600}, std::chrono::seconds{120});
    if (!r.value || !r.value->is_object() || r.value->contains("error")) return out;
    const Json rows = member(*r.value, "identified");
    if (rows.is_array())
        for (const Json& row : rows)
            if (row.is_array() && row.size() >= 3) out[address_of(row[0])] = row[2].get<std::string>();
    return out;
}

Json Server::Impl::hex_addresses(Json info) {
    // The inspector's description with addresses as hex text and by-number imports named. File offsets and sizes stay numbers.
    if (!info.is_object()) return info;
    auto touch = [&](Json& part) {
        if (!part.is_object()) return;
        for (const char* k : {"image_base", "entry", "entry_point", "code_base", "data_base"})
            if (part.contains(k)) part[k] = hexed(part[k]);
    };
    if (info.contains("summary")) touch(info["summary"]);
    if (info.contains("headers") && info["headers"].is_object() && info["headers"].contains("optional")) touch(info["headers"]["optional"]);
    if (info.contains("sections") && info["sections"].is_array())
        for (Json& sec : info["sections"]) sec["address"] = hexed(member(sec, "address"));
    if (info.contains("exports") && info["exports"].is_object() && info["exports"].contains("symbols") && info["exports"]["symbols"].is_array())
        for (Json& sym : info["exports"]["symbols"]) sym["address"] = hexed(member(sym, "address"));
    if (info.contains("imports") && info["imports"].is_array()) {
        for (Json& imp : info["imports"]) {
            std::string stem = text_member(imp, "module");
            if (const std::size_t dot = stem.rfind('.'); dot != std::string::npos && dot != 0) stem.resize(dot);
            if (!imp.contains("symbols") || !imp["symbols"].is_array()) continue;
            for (Json& sym : imp["symbols"]) {
                sym["slot"] = hexed(member(sym, "slot"));
                if (!truthy(member(sym, "name")) && member(sym, "ordinal").is_number_integer() && sym["ordinal"].get<std::int64_t>() >= 0 && s.ordinal_name) {
                    const Json named = s.ordinal_name(stem, sym["ordinal"].get<std::int64_t>());
                    if (named.is_object() && truthy(member(named, "name"))) {
                        sym["name"] = named["name"];
                        sym["named_from"] = member(named, "source");
                    }
                }
            }
        }
    }
    return info;
}

std::pair<std::optional<Json>, Json> Server::Impl::ledger_now(bool refresh) {
    // (ledger, none) when it is known; (the one kept on disk, a note how old) while it is being worked out again; (none, a not-ready
    // answer) when there is neither.
    if (!s.ledger) throw UsageError("the coverage ledger is not available in this server");
    const Slow::Result r = slow.get("ledger", [&] {
        Json led = s.ledger(refresh);
        if (!(led.is_object() && led.contains("error"))) {
            const sfs::path file = s.state / "mcp" / "ledger.json";
            try {
                Json kept = Json::object();
                kept["time"] = std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
                kept["ledger"] = led;
                json::save(file, kept, json::Style::PythonCompatible);
            } catch (const Error&) {   // the copy on disk is a convenience
            }
        }
        return led;
    }, std::chrono::seconds{600}, std::chrono::seconds{20}, refresh);
    if (r.value) return {r.value, Json(nullptr)};
    try {
        const Json kept = json::load(s.state / "mcp" / "ledger.json");
        const std::time_t when = static_cast<std::time_t>(kept.at("time").get<double>());
        const std::tm tm = sys::local_tm(when);
        char buf[64];
        std::strftime(buf, sizeof buf, "%Y-%m-%d %I:%M %p", &tm);
        Json note = Json::object();
        note["as_of"] = std::string(buf);
        note["say"] = "the ledger kept from then; a fresh one is being worked out";
        return std::pair<std::optional<Json>, Json>{kept.at("ledger"), note};
    } catch (const std::exception&) {
        return std::pair<std::optional<Json>, Json>{std::nullopt, not_ready("the coverage ledger", r.seconds)};
    }
}

std::vector<sfs::path> Server::Impl::roots() {
    // The folders read_file may read: project folders, the source tree, state, this checkout.
    std::vector<sfs::path> out;
    for (const sfs::path& p : {s.converted, s.state, s.root}) {
        std::error_code ec;
        if (!p.empty() && sfs::is_directory(p, ec)) out.push_back(real_path(p));
    }
    if (s.tree_root) {
        try {
            if (const std::optional<sfs::path> t = s.tree_root()) {
                std::error_code ec;
                if (sfs::is_directory(*t, ec)) out.push_back(real_path(*t));
            }
        } catch (const Error&) {   // no source tree here
        }
    }
    return out;
}

std::pair<Json, bool> Server::Impl::listing(const sfs::path& folder, std::string_view find, std::size_t most) const {
    // The files under a folder, depth first, each folder's files sorted and then its folders (those that start with a dot left out).
    Json rows = Json::array();
    const std::string want = lowered(find);
    std::function<bool(const sfs::path&)> walk = [&](const sfs::path& dir) -> bool {
        std::vector<sfs::path> files, dirs;
        std::error_code ec;
        for (sfs::directory_iterator it(dir, ec), end; !ec && it != end; it.increment(ec)) {
            std::error_code e2;
            (it->is_directory(e2) ? dirs : files).push_back(it->path());
        }
        auto by_name = [](const sfs::path& a, const sfs::path& b) { return a.filename().generic_string() < b.filename().generic_string(); };
        std::sort(files.begin(), files.end(), by_name);
        std::sort(dirs.begin(), dirs.end(), by_name);
        for (const sfs::path& f : files) {
            std::string rel = f.lexically_relative(folder).generic_string();
            if (!want.empty() && lowered(rel).find(want) == std::string::npos) continue;
            std::error_code e3;
            Json row = Json::object();
            row["file"] = rel;
            row["size"] = static_cast<std::int64_t>(sfs::file_size(f, e3));
            rows.push_back(std::move(row));
            if (rows.size() >= most) return true;
        }
        for (const sfs::path& d : dirs) {
            if (d.filename().generic_string().starts_with('.')) continue;
            if (walk(d)) return true;
        }
        return false;
    };
    const bool more = walk(folder);
    return {rows, more};
}

// ---- reading ------------------------------------------------------------------------------------------------------------------------

namespace {
template <class F>
const F& need(const F& f, std::string_view what) {
    if (!f) throw UsageError(std::format("{} is not available in this server", what));
    return f;
}
}  // namespace

Json Server::Impl::describe(const Json& a) {
    const sfs::path p = path_of(str_of(a, "path"));
    std::error_code ec;
    if (sfs::is_directory(p, ec)) {
        Json out = Json::object();
        out["path"] = p.string();
        out["found"] = "a folder";
        out["say"] = "a folder: use reach through run_command to go through it";
        return out;
    }
    Json out = need(s.reach_describe, "what a file is")(p);
    if (!out.is_object()) out = Json::object();
    out["path"] = p.string();
    out["size"] = static_cast<std::int64_t>(sfs::file_size(p));
    Json wrapper = Json::object();
    Json description = need(s.inspect, "the inspector")(p);
    wrapper["summary"] = member(description, "summary").is_null() ? Json::object() : member(description, "summary");
    out["summary"] = hex_addresses(wrapper)["summary"];
    return out;
}

Json Server::Impl::inspect_module(const Json& a) {
    const sfs::path p = path_of(str_of(a, "module"));
    std::error_code ec;
    if (sfs::is_directory(p, ec)) {
        Json out = Json::object();
        out["path"] = p.string();
        out["kind"] = "java";
        out["classes"] = s.java_classes ? s.java_classes(p) : 0;
        return out;
    }
    Json info = hex_addresses(need(s.inspect, "the inspector")(p));
    const std::string section = str_of(a, "section");
    if (!section.empty()) {
        if (!info.contains(section)) {
            std::vector<std::string> keys;
            for (const auto& [k, v] : info.items()) keys.push_back(k);
            throw UsageError(std::format("no part {}; there are: {}", py_repr(section), text::join(keys, ", ")));
        }
        Json one = Json::object();
        one[section] = info[section];
        return fit(one);
    }
    return fit(info);
}

Json Server::Impl::strings(const Json& a) {
    const sfs::path p = path_of(str_of(a, "module"));
    Json rows = need(s.strings, "the strings of a file")(p, std::max<std::int64_t>(2, std::min<std::int64_t>(int_of(a, "min_len"), 64)));
    const std::string find = lowered(str_of(a, "find"));
    if (!find.empty()) {
        Json kept = Json::array();
        for (const Json& r : rows)
            if (lowered(text_member(r, "text")).find(find) != std::string::npos) kept.push_back(r);
        rows = std::move(kept);
    }
    return page(rows, int_of(a, "offset"), int_of(a, "limit"));
}

Json Server::Impl::hex_dump(const Json& a) {
    const sfs::path p = path_of(str_of(a, "module"));
    const std::int64_t n = std::max<std::int64_t>(1, std::min<std::int64_t>(int_of(a, "length"), 16384));
    std::int64_t offset = int_of(a, "offset");
    Json out = Json::object();
    const std::string addr = str_of(a, "addr");
    if (!addr.empty()) {
        const auto place = need(s.locate, "addresses")(p, addr);   // (UsageError for an address it cannot place)
        if (!place) throw UsageError(std::format("no address or function named {} in {}", py_repr(addr), p.filename().string()));
        if (!place->second)
            throw UsageError(std::format("{:x} is not stored in the file (or {} is not a 32-bit module: give `offset`)", place->first, p.filename().string()));
        offset = *place->second;
        out["addr"] = std::format("{:x}", place->first);
    }
    out["offset"] = offset;
    out["length"] = n;
    out["size"] = static_cast<std::int64_t>(sfs::file_size(p));
    out["text"] = need(s.hexdump, "the hex dump")(p, std::max<std::int64_t>(0, offset), n);
    return out;
}

ToolResult Server::Impl::resource(const Json& a) {
    const sfs::path p = path_of(str_of(a, "module"));
    ResourceBody body = need(s.resource_body, "resources")(p, int_of(a, "offset"), int_of(a, "size"), int_of(a, "type_id"), bool_of(a, "wide"));
    ToolResult out;
    if (body.image) {
        out.image = std::move(body);
        return out;
    }
    out.document = std::make_shared<Json>(fit(body.document ? *body.document : Json::object()));
    return out;
}

Json Server::Impl::modules(const Json& a) {
    const auto [led, note] = ledger_now(bool_of(a, "refresh"));
    if (!led) return note;
    static const std::vector<std::string> keep = {"name", "kind", "size", "in_store", "measured", "judged", "functions", "decompiled", "compiled", "verified",
                                                  "weak", "unverified", "not-judged", "no-compile", "no-source", "code_bytes", "bytes_known",
                                                  "bytes_verified", "bytes_weak"};
    const std::string kind = str_of(a, "kind"), find = lowered(str_of(a, "find"));
    const bool only_in_store = bool_of(a, "only_in_store");
    Json rows = Json::array();
    const Json all = member(*led, "modules");
    if (all.is_array()) {
        for (const Json& r : all) {
            if (!kind.empty() && text_member(r, "kind") != kind) continue;
            if (!find.empty() && text_member(r, "name").find(find) == std::string::npos) continue;   // (the name as it is: not lowered)
            if (!truthy(member(r, "in_store")) && only_in_store) continue;
            Json row = Json::object();
            for (const std::string& k : keep) row[k] = member(r, k.c_str());
            rows.push_back(std::move(row));
        }
    }
    Json out = page(rows, int_of(a, "offset"), int_of(a, "limit"));
    if (!note.is_null()) out["kept"] = note;
    return out;
}

Json Server::Impl::coverage(const Json& a) {
    const auto [led, note] = ledger_now(bool_of(a, "refresh"));
    if (!led) return note;
    Json out = Json::object();
    for (const char* k : {"total", "kinds", "profiles", "error"})
        if (led->is_object() && led->contains(k)) out[k] = (*led)[k];
    if (!note.is_null()) out["kept"] = note;
    return out;
}

Json Server::Impl::progress(const Json&) {
    const Slow::Result r = slow.get("progress", [&] { return need(s.progress, "the progress reading")(); }, std::chrono::seconds{30});
    return r.value ? *r.value : not_ready("the progress reading", r.seconds);
}

Json Server::Impl::functions(const Json& a) {
    const std::string module = str_of(a, "module");
    const sfs::path p = path_of(module);
    const std::optional<Json> found = store_view(p.filename().string());
    if (!found) throw UsageError(std::format("{} is not a module of the corpus or the store", module));
    if (found->contains("error") && !found->contains("functions")) return *found;
    const std::map<std::string, std::string> made = identified_of(p);
    const std::string cls = str_of(a, "cls"), find = str_of(a, "find");
    const std::string find_low = lowered(find), find_addr = normalize_address(find);
    Json rows = Json::array();
    std::int64_t identified = 0;
    for (Json r : (*found)["functions"]) {
        const auto lib = made.find(text_member(r, "addr"));
        if (lib != made.end() && !lib->second.empty()) {   // made by a tool or a runtime library: not authored code
            r["library"] = lib->second;
            ++identified;
            if (text_member(r, "class") == "no-compile") r["why"] = "identified, not reconstructed by design (" + lib->second + ")";
        }
        const bool class_ok = cls.empty() || text_member(r, "class") == cls || (cls == "identified" && truthy(member(r, "library")));
        const bool find_ok = find.empty() || lowered(text_member(r, "name")).find(find_low) != std::string::npos || text_member(r, "addr").find(find_addr) != std::string::npos;
        if (class_ok && find_ok) rows.push_back(std::move(r));
    }
    Json out = Json::object();
    for (const char* k : {"module", "kind_title", "sha", "in_store", "measured", "counts", "reasons"}) out[k] = (*found)[k];
    out["counts"]["identified"] = identified;
    if (!(*found)["in_store"].get<bool>()) out["say"] = std::format("not measured yet: run_command('measure', ['{}'])", module);
    const Json paged = page(rows, int_of(a, "offset"), int_of(a, "limit"));   // (a named value: its items are read below)
    for (const auto& [k, v] : paged.items()) out[k] = v;
    return out;
}

Json Server::Impl::function(const Json& a) {
    const std::string module = str_of(a, "module"), addr = str_of(a, "addr");
    const sfs::path p = path_of(module);
    const std::string name = p.filename().string();
    const std::optional<Json> found = store_view(name);
    if (!found || !truthy(member(*found, "in_store"))) throw UsageError(std::format("{} has nothing in the store yet; measure it first", module));
    const std::string wanted = normalize_address(addr);
    const Json& rows = (*found)["functions"];
    auto find_row = [&](auto&& match) -> const Json* {
        for (const Json& r : rows)
            if (match(r)) return &r;
        return nullptr;
    };
    auto strip_zeros = [](std::string s) {
        const std::size_t k = s.find_first_not_of('0');
        return k == std::string::npos ? std::string() : s.substr(k);
    };
    const Json* hit = find_row([&](const Json& r) { return text_member(r, "addr") == wanted; });
    if (!hit)
        hit = find_row([&](const Json& r) {
            const std::string ra = text_member(r, "addr");
            return ra.ends_with(":" + wanted) || strip_zeros(ra) == strip_zeros(wanted);
        });
    if (!hit) hit = find_row([&](const Json& r) { return !text_member(r, "name").empty() && lowered(text_member(r, "name")) == lowered(strip_text(addr)); });
    if (!hit)
        throw UsageError(std::format("no function {} in {} ({} functions; `functions` lists them)", py_repr(addr), name, rows.size()));
    const Json answer = link.call("functions", Json{{"module", name}, {"addr", text_member(*hit, "addr")}});
    Json one = answer.is_object() && answer.contains("function") ? answer["function"] : Json::object();
    if (!truthy(member(one, "source"))) {   // not judged under these settings: what was made of it
        Json made = Json::array();
        if (s.stored_candidates) {
            const Json candidates = s.stored_candidates(text_member(*found, "sha"), text_member(*hit, "addr"));
            if (candidates.is_array()) {
                for (const Json& c : candidates) {
                    Json item = Json::object();
                    for (const char* k : {"engine", "rev", "name", "source", "problem"}) item[k] = member(c, k);
                    Json older = Json::array();
                    const Json verdicts = member(c, "verdicts");
                    for (std::size_t i = 0; verdicts.is_array() && i < verdicts.size() && i < 5; ++i) {
                        Json v = Json::object();
                        for (const char* k : {"profile", "verdict", "reason", "limits"}) v[k] = member(verdicts[i], k);
                        // what the earlier verdict amounts to, in the one vocabulary (a client shows it, it does not work it out)
                        const Json word = member(verdicts[i], "verdict"), limits = member(verdicts[i], "limits");
                        v["class"] = std::string(verify::word(verify::classify(
                            word.is_string() ? verify::parse_verdict(word.get<std::string>()) : std::nullopt,
                            limits.is_string() ? limits.get<std::string>() : std::string()).cls));
                        older.push_back(std::move(v));
                    }
                    item["older_verdicts"] = std::move(older);
                    made.push_back(std::move(item));
                }
            }
        }
        if (!made.empty()) {
            one["candidates"] = made;
            const Json profile = member(*found, "profile");
            one["say"] = std::format("no verdict under the verifier settings in force (profile {}); these are the stored reconstructions, with any verdicts from earlier settings",
                                     profile.is_string() ? profile.get<std::string>() : std::string("None"));
        }
    }
    return fit(one);
}

Json Server::Impl::names(const Json& a) {
    const sfs::path p = path_of(str_of(a, "module"));
    const Json got = need(s.names, "the names of a module")(p);
    const std::string find = lowered(str_of(a, "find"));
    Json fns = Json::array();
    for (const Json& row : member(got, "functions")) {
        if (!find.empty() && lowered(row[2].get<std::string>()).find(find) == std::string::npos) continue;
        Json r = Json::object();
        r["addr"] = address_of(row[0]);
        r["key"] = row[1];
        r["name"] = row[2];
        r["source"] = row[3];
        if (row.size() > 4 && !row[4].is_null()) r["proposed"] = row[4];   // (a proposal is not the name)
        fns.push_back(std::move(r));
    }
    Json out = page(fns, int_of(a, "offset"), int_of(a, "limit"));
    const Json imports = member(got, "imports");
    out["imports"] = imports.is_array() ? imports.size() : 0;
    return out;
}

Json Server::Impl::proposals(const Json& a) {
    const sfs::path p = path_of(str_of(a, "module"));
    const std::string state = lowered(str_of(a, "state"));
    if (!state.empty() && state != "proposed" && state != "accepted" && state != "rejected") throw UsageError("state is proposed, accepted or rejected");
    Json rows = need(s.proposals, "the proposed names")(p, state);
    Json out = Json::object();
    out["count"] = rows.size();
    out["proposals"] = std::move(rows);
    out["next"] = "names MODULE --accept ADDR (a person decides); propose_name adds one";
    return out;
}

Json Server::Impl::identify(const Json& a) {
    const sfs::path p = path_of(str_of(a, "module"));
    const Json found = need(s.identified, "the runtime libraries' signatures")(p);
    std::vector<std::pair<std::int64_t, Json>> sorted;   // by address, as the numbers sort
    for (const Json& row : member(found, "identified")) {
        const std::string addr = address_of(row[0]);
        const std::int64_t value = text::parse_int<std::int64_t>(addr, 16).value_or(0);
        Json r = Json::object();
        r["addr"] = addr;
        r["name"] = row[1];
        r["library"] = row[2];
        sorted.emplace_back(value, std::move(r));
    }
    std::stable_sort(sorted.begin(), sorted.end(), [](const auto& x, const auto& y) { return x.first < y.first; });
    Json rows = Json::array();
    for (std::size_t i = 0; i < sorted.size() && i < 2000; ++i) rows.push_back(sorted[i].second);
    Json out = Json::object();
    out["identified"] = sorted.size();
    out["rows"] = std::move(rows);
    const Json compiler = member(found, "compiler");
    out["compiler"] = compiler.is_null() ? Json("unknown (no compiler)") : compiler;
    return fit(out);
}

Json Server::Impl::demangle(const Json& a) {
    Json out = Json::object();
    std::size_t n = 0;
    for (const Json& item : a.at("decorated")) {
        if (n++ >= 200) break;
        const std::string name = item.get<std::string>();
        Json d = Json::object();
        d["readable"] = symbols::demangle::readable(name);
        const std::optional<std::string> decl = symbols::demangle::declaration(name);
        d["declaration"] = decl ? Json(*decl) : Json(nullptr);
        const std::optional<std::uint32_t> bytes = symbols::demangle::stack_bytes(name);
        d["stack_bytes"] = bytes ? Json(*bytes) : Json(nullptr);
        out[name] = std::move(d);
    }
    return out;
}

Json Server::Impl::commands(const Json&) {
    Json out = Json::object();
    out["usage"] = need(s.usage, "the command table")();
    std::vector<std::string> runnable;
    for (const std::string& c : need(s.command_names, "the command table")())
        if (!is_interactive(c)) runnable.push_back(c);
    std::sort(runnable.begin(), runnable.end());
    runnable.erase(std::unique(runnable.begin(), runnable.end()), runnable.end());
    out["runnable"] = runnable;
    return out;
}

Json Server::Impl::command_help(const Json& a) {
    const std::string command = str_of(a, "command");
    const std::optional<std::pair<std::string, std::string>> help = need(s.command_help, "the command table")(command);
    if (!help) throw UsageError(std::format("no command {}; `commands` lists them", py_repr(command)));
    Json out = Json::object();
    out["command"] = command;
    out["module"] = help->first;
    out["fixed_args"] = Json::array();
    out["help"] = utf8_slice(help->second.empty() ? "(no description)" : help->second, 0, kMost);
    return out;
}

// ---- what was made: reach runs, project folders, the source tree -----------------------------------------------------------------

Json Server::Impl::reach_results(const Json& a) {
    const sfs::path folder = s.state / "reach";
    const std::string run = str_of(a, "run");
    if (run.empty()) {
        Json runs = Json::array();
        std::vector<std::string> names;
        std::error_code ec;
        for (sfs::directory_iterator it(folder, ec), end; !ec && it != end; it.increment(ec)) names.push_back(it->path().filename().string());
        std::sort(names.begin(), names.end());
        for (const std::string& name : names) {
            if (!name.ends_with(".json")) continue;
            Json d;
            try {
                d = json::load(folder / name);
            } catch (const Error&) {
                continue;
            }
            const Json progs = member(d, "programs");
            std::int64_t judged = 0;
            for (const Json& r : progs.is_array() ? progs : Json::array())
                if (truthy(member(r, "judged"))) ++judged;
            Json item = Json::object();
            item["run"] = name.substr(0, name.size() - 5);
            item["when"] = member(d, "when");
            item["inputs"] = member(d, "inputs");
            item["programs"] = progs.is_array() ? progs.size() : 0;
            item["judged"] = judged;
            runs.push_back(std::move(item));
        }
        Json out = Json::object();
        out["runs"] = std::move(runs);
        return out;
    }
    const sfs::path path = folder / (sfs::path(run).filename().string() + ".json");
    std::error_code ec;
    if (!sfs::is_regular_file(path, ec)) throw UsageError(std::format("no reach run {} (reach_results without a run lists them)", py_repr(run)));
    const Json d = json::load(path);
    const std::string platform = lowered(str_of(a, "platform")), find = lowered(str_of(a, "find"));
    const bool judged_only = bool_of(a, "judged_only");
    Json per = Json::object(), rows = Json::array();
    for (const Json& r : member(d, "programs").is_array() ? d["programs"] : Json::array()) {
        const std::string plat = truthy(member(r, "platform")) ? text_member(r, "platform") : "(unknown)";
        if (!per.contains(plat)) {
            Json one = Json::object();
            one["programs"] = 0;
            one["judged"] = 0;
            one["lane"] = member(r, "lane");
            per[plat] = std::move(one);
        }
        per[plat]["programs"] = per[plat]["programs"].get<std::int64_t>() + 1;
        per[plat]["judged"] = per[plat]["judged"].get<std::int64_t>() + (truthy(member(r, "judged")) ? 1 : 0);
        if (!platform.empty() && lowered(text_member(r, "platform")).find(platform) == std::string::npos) continue;
        if (!find.empty() && lowered(text_member(r, "path")).find(find) == std::string::npos) continue;
        if (judged_only && !truthy(member(r, "judged"))) continue;
        Json row = Json::object();
        for (const char* k : {"path", "origin", "found", "platform", "lane", "family", "size", "picked", "judged"}) {
            const Json v = member(r, k);
            if (!v.is_null() && !(v.is_string() && v.get<std::string>().empty())) row[k] = v;
        }
        rows.push_back(std::move(row));
    }
    Json out = page(rows, int_of(a, "offset"), int_of(a, "limit"), 5000);
    out["when"] = member(d, "when");
    out["inputs"] = member(d, "inputs");
    out["platforms"] = std::move(per);
    return out;
}

Json Server::Impl::projects(const Json& a) {
    const std::string find = lowered(str_of(a, "find"));
    Json out_list = Json::array();
    std::error_code ec;
    if (sfs::is_directory(s.converted, ec)) {
        std::vector<std::string> names;
        for (sfs::directory_iterator it(s.converted, ec), end; !ec && it != end; it.increment(ec)) names.push_back(it->path().filename().string());
        std::sort(names.begin(), names.end());
        for (const std::string& name : names) {
            const sfs::path pj = s.converted / name / "project.json";
            if (!sfs::is_regular_file(pj, ec)) continue;
            const Json d = read_json_or_empty(pj);
            Json commands = Json::array();
            const Json steps = member(d, "steps");
            if (steps.is_array())
                for (const Json& step : steps) commands.push_back(member(step, "command"));
            Json row = Json::object();
            row["name"] = name;
            row["file"] = d.is_object() && d.contains("file") ? d["file"] : Json("");
            row["kind"] = d.is_object() && d.contains("kind") ? d["kind"] : Json("");
            row["size"] = member(d, "size");
            row["commands"] = std::move(commands);
            const std::string hay = lowered(name + " " + (row["file"].is_string() ? row["file"].get<std::string>() : std::string()));
            if (find.empty() || hay.find(find) != std::string::npos) out_list.push_back(std::move(row));
        }
    }
    Json out = Json::object();
    out["root"] = s.converted.string();
    out["projects"] = std::move(out_list);
    return out;
}

Json Server::Impl::project(const Json& a) {
    const sfs::path folder = s.converted / sfs::path(str_of(a, "name")).filename();
    std::error_code ec;
    if (!sfs::is_directory(folder, ec))
        throw UsageError(std::format("no project {} under {} (`projects` lists them)", py_repr(str_of(a, "name")), s.converted.string()));
    Json d = Json::object();
    if (sfs::is_regular_file(folder / "project.json", ec)) d = json::load(folder / "project.json");
    if (d.is_object() && d.contains("steps") && d["steps"].is_array()) {
        for (Json& step : d["steps"]) {
            const Json outs = member(step, "outputs");
            if (outs.is_array() && outs.size() > 20) {
                Json cut = Json::array();
                for (std::size_t i = 0; i < 20; ++i) cut.push_back(outs[i]);
                cut.push_back(std::format("... {} more", outs.size() - 20));
                step["outputs"] = std::move(cut);
            }
        }
    }
    auto [files, more] = listing(folder, str_of(a, "find"));
    Json out = Json::object();
    out["folder"] = folder.string();
    out["project"] = std::move(d);
    out["files"] = std::move(files);
    out["more_files"] = more;
    return fit(out);
}

Json Server::Impl::source_tree(const Json& a) {
    const sfs::path p = path_of(str_of(a, "module"));
    const auto [relative, how] = need(s.placement, "the source tree")(p.filename().string(), p);
    const sfs::path tree = need(s.tree_root, "the source tree")().value_or(sfs::path());
    sfs::path folder = tree;
    for (const std::string_view part : text::split(relative, '/')) folder /= std::string(part);
    std::error_code ec;
    const bool exists = sfs::is_directory(folder, ec);
    Json files = Json::array();
    bool more = false;
    if (exists) std::tie(files, more) = listing(folder, str_of(a, "find"));
    static const std::array<std::string, 3> order = {"source", "notes", "built"};
    std::vector<Json> kinds;
    for (Json f : files) {
        f["kind"] = file_kind(text_member(f, "file"));
        kinds.push_back(std::move(f));
    }
    auto rank = [&](const Json& f) { return std::find(order.begin(), order.end(), text_member(f, "kind")) - order.begin(); };
    std::stable_sort(kinds.begin(), kinds.end(), [&](const Json& x, const Json& y) { return rank(x) < rank(y); });
    Json counts = Json::object();
    for (const std::string& k : order) {
        std::int64_t n = 0;
        for (const Json& f : kinds) n += text_member(f, "kind") == k ? 1 : 0;
        counts[k] = n;
    }
    Json out = Json::object();
    out["tree"] = tree.string();
    out["folder"] = folder.string();
    out["relative"] = relative;
    out["decided"] = how;
    out["exists"] = exists;
    out["counts"] = std::move(counts);
    out["files"] = Json(kinds);
    out["more_files"] = more;
    return out;
}

Json Server::Impl::read_file(const Json& a) {
    const std::string path = str_of(a, "path");
    const sfs::path real = real_path(sfs::path(path));
    const std::vector<sfs::path> allowed = roots();
    if (!std::any_of(allowed.begin(), allowed.end(), [&](const sfs::path& r) { return inside(real, r); })) {
        std::vector<std::string> shown;
        for (const sfs::path& r : allowed) shown.push_back(r.string());
        throw UsageError(std::format("{} is outside the folders this reads: {}", path, text::join(shown, ", ")));
    }
    std::error_code ec;
    if (!sfs::is_regular_file(real, ec)) throw UsageError(std::format("no file {}", path));
    const Bytes data = fs::read_file(real);
    if (std::find(data.begin(), data.begin() + static_cast<std::ptrdiff_t>(std::min<std::size_t>(data.size(), 4096)), std::uint8_t{0}) !=
        data.begin() + static_cast<std::ptrdiff_t>(std::min<std::size_t>(data.size(), 4096)))
        throw UsageError(std::format("{} is binary; hex_dump reads it", path));
    // UTF-8 when it is; else the period's files: Windows-1252.
    std::string text(reinterpret_cast<const char*>(data.data()), data.size());
    bool valid = true;
    for (std::size_t i = 0; i < text.size() && valid;) {
        const unsigned char c = static_cast<unsigned char>(text[i]);
        const std::size_t len = c < 0x80 ? 1 : (c >> 5) == 6 ? 2 : (c >> 4) == 14 ? 3 : (c >> 3) == 30 ? 4 : 0;
        if (len == 0 || i + len > text.size()) valid = false;
        for (std::size_t k = 1; valid && k < len; ++k) valid = (static_cast<unsigned char>(text[i + k]) & 0xC0) == 0x80;
        i += len ? len : 1;
    }
    if (!valid) text = text::from_cp1252(data);
    const std::int64_t at = std::max<std::int64_t>(0, int_of(a, "offset"));
    const std::int64_t n = std::max<std::int64_t>(1, std::min<std::int64_t>(int_of(a, "length"), static_cast<std::int64_t>(kMost)));
    const std::int64_t characters = static_cast<std::int64_t>(utf8_length(text));
    Json out = Json::object();
    out["path"] = real.string();
    out["characters"] = characters;
    out["offset"] = at;
    out["text"] = utf8_slice(text, static_cast<std::size_t>(at), static_cast<std::size_t>(n));
    out["next"] = at + n < characters ? Json(at + n) : Json(nullptr);
    return out;
}

// ---- one function now; two builds side by side --------------------------------------------------------------------------------------

Json Server::Impl::decompile_function(const Json& a) {
    const sfs::path p = path_of(str_of(a, "module"));
    const std::string addr = str_of(a, "addr");
    const std::int64_t wait = std::max<std::int64_t>(1, std::min<std::int64_t>(int_of(a, "wait"), 240));
    const Slow::Result r = slow.get("decompile\x01" + p.string() + "\x01" + addr, [&] { return need(s.decompile_function, "the decompiler")(p, addr); },
                                    std::chrono::seconds{3600}, std::chrono::seconds{wait});
    if (!r.value) return not_ready(std::format("decompiling {} at {}", p.filename().string(), addr), r.seconds);
    if (r.value->is_object() && r.value->contains("error")) throw UsageError(text_member(*r.value, "error"));
    Json out = Json::object();
    for (const auto& [k, v] : r.value->items())
        if (k != "addr") out[k] = v;
    out["module"] = p.filename().string();
    out["addr"] = r.value->contains("addr") ? (*r.value)["addr"] : Json(normalize_address(addr));
    out["note"] = "decompiled now; not built or judged";
    return out;
}

Json Server::Impl::versions_of(const sfs::path& pa, const sfs::path& pb, std::int64_t wait, bool& ready) {
    const Slow::Result r = slow.get("versions\x01" + pa.string() + "\x01" + pb.string(), [&] { return need(s.compare, "the version comparison")(pa, pb); },
                                    std::chrono::seconds{3600}, std::chrono::seconds{std::max<std::int64_t>(1, std::min<std::int64_t>(wait, 240))});
    ready = r.value.has_value();
    return r.value ? *r.value : not_ready("comparing the two builds", r.seconds);
}

Json Server::Impl::compare_versions(const Json& a) {
    bool ready = false;
    const Json value = versions_of(path_of(str_of(a, "module_a")), path_of(str_of(a, "module_b")), int_of(a, "wait"), ready);
    if (!ready || (value.is_object() && value.contains("error"))) return value;
    const std::string status = str_of(a, "status");
    std::vector<Json> pairs;
    for (const Json& x : member(value, "pairs"))
        if (status.empty() || text_member(x, "status") == status) pairs.push_back(x);
    std::stable_sort(pairs.begin(), pairs.end(), [](const Json& x, const Json& y) { return (text_member(x, "status") != "changed") < (text_member(y, "status") != "changed"); });
    Json out = Json::object();
    for (const char* k : {"report", "counts", "code"}) out[k] = member(value, k);
    out["fields"] =
        "level: what agrees -- bytes (the same machine code but for what the loader fills in), text (the same instructions, addresses aside), shape (the same "
        "kinds of instruction and operand: registers or constants differ); how: which matching step paired them (bytes, name, text, shape, calls, likeness, "
        "position); score: likeness of the shapes, so a changed pair at 1.0 differs only in registers or constants. diff_function shows one pair side by side.";
    for (const char* k : {"added", "removed", "names"}) {
        Json first = Json::array();
        const Json all = member(value, k);
        for (std::size_t i = 0; all.is_array() && i < all.size() && i < 500; ++i) first.push_back(all[i]);
        out[k] = std::move(first);
    }
    const Json paged = page(Json(pairs), int_of(a, "offset"), int_of(a, "limit"));
    for (const auto& [k, v] : paged.items()) out[k] = v;
    return fit(out);
}

Json Server::Impl::diff_function(const Json& a) {
    const sfs::path pa = path_of(str_of(a, "module_a")), pb = path_of(str_of(a, "module_b"));
    bool ready = false;
    const Json value = versions_of(pa, pb, int_of(a, "wait"), ready);
    if (!ready || (value.is_object() && value.contains("error"))) return value;
    return fit(need(s.diff, "the version comparison")(pa, pb, str_of(a, "addr")));
}

// ---- code: disassembly, cross-references, a module at a glance ------------------------------------------------------------------

Json Server::Impl::disasm(const Json& a) {
    return fit(need(s.disasm, "the disassembler")(path_of(str_of(a, "module")), str_of(a, "addr"), int_of(a, "count")));
}

Json Server::Impl::xrefs(const Json& a) { return fit(need(s.xrefs, "the cross-references")(path_of(str_of(a, "module")), str_of(a, "addr"))); }

Json Server::Impl::triage(const Json& a) {
    const sfs::path p = path_of(str_of(a, "module"));
    const std::string name = p.filename().string();
    const Json d = describe(Json{{"path", p.string()}});
    Json out = Json::object();
    out["module"] = name;
    out["path"] = p.string();
    Json is = Json::object();
    for (const char* k : {"found", "platform", "lane", "family", "size"}) is[k] = member(d, k);
    out["is"] = std::move(is);
    out["summary"] = member(d, "summary");
    try {
        const Json info = hex_addresses(need(s.inspect, "the inspector")(p));
        Json exports = Json::array();
        const Json ex = member(member(info, "exports"), "symbols");
        for (std::size_t i = 0; ex.is_array() && i < ex.size() && i < 200; ++i)
            exports.push_back(truthy(member(ex[i], "name")) ? ex[i]["name"] : Json("#" + (member(ex[i], "ordinal").is_null() ? std::string("None") : member(ex[i], "ordinal").dump())));
        out["exports"] = std::move(exports);
        Json imports = Json::object();
        const Json imps = member(info, "imports");
        for (const Json& imp : imps.is_array() ? imps : Json::array()) {
            Json names = Json::array();
            const Json syms = member(imp, "symbols");
            for (std::size_t i = 0; syms.is_array() && i < syms.size() && i < 100; ++i)
                names.push_back(truthy(member(syms[i], "name")) ? syms[i]["name"] : Json("#" + (member(syms[i], "ordinal").is_null() ? std::string("None") : member(syms[i], "ordinal").dump())));
            imports[text_member(imp, "module")] = std::move(names);
        }
        out["imports"] = std::move(imports);
    } catch (const std::exception& e) {
        out["inspect"] = std::string(typeid(e).name()) + ": " + e.what();
    }
    try {
        const Json ident = identify(Json{{"module", p.string()}});
        Json libs = Json::object();
        for (const Json& r : member(ident, "rows")) {
            const std::string lib = text_member(r, "library");
            libs[lib] = libs.contains(lib) ? libs[lib].get<std::int64_t>() + 1 : 1;
        }
        Json made = Json::object();
        made["compiler"] = member(ident, "compiler");
        made["identified"] = std::move(libs);
        out["made_by"] = std::move(made);
    } catch (const std::exception& e) {
        out["made_by"] = std::string(typeid(e).name()) + ": " + e.what();
    }
    try {
        const Json fns = functions(Json{{"module", p.string()}, {"cls", ""}, {"find", ""}, {"offset", 0}, {"limit", 5000}});
        Json store = Json::object();
        store["counts"] = member(fns, "counts");
        store["reasons"] = member(fns, "reasons");
        out["store"] = std::move(store);
        Json look = Json::array();
        std::size_t bad = 0;
        for (const Json& r : member(fns, "rows")) {
            const std::string cls = text_member(r, "class");
            if (!(cls == "unverified" || cls == "weak" || cls == "no-compile" || cls == "no-source") || truthy(member(r, "library"))) continue;
            ++bad;
            if (look.size() >= 12) continue;
            const Json one = function(Json{{"module", p.string()}, {"addr", text_member(r, "addr")}});
            Json item = Json::object();
            item["addr"] = member(r, "addr");
            item["name"] = member(r, "name");
            item["class"] = member(r, "class");
            item["why"] = member(r, "why");
            item["detail"] = one.is_object() && one.contains("detail") ? one["detail"] : Json("");
            item["source"] = utf8_slice(text_member(one, "source"), 0, 1500);
            look.push_back(std::move(item));
        }
        out["to_look_at"] = std::move(look);
        if (bad > 12) out["more_to_look_at"] = bad - 12;
        if (!truthy(member(fns, "in_store"))) out["store"]["say"] = member(fns, "say");
    } catch (const UsageError& e) {
        out["store"] = std::string(e.what());
    }
    try {
        const Json st = source_tree(Json{{"module", p.string()}, {"find", ""}});
        Json source = Json::object();
        source["folder"] = member(st, "folder");
        source["exists"] = member(st, "exists");
        source["counts"] = member(st, "counts");
        Json notes = Json::array(), code = Json::array();
        for (const Json& f : member(st, "files")) {
            if (text_member(f, "kind") == "notes" && notes.size() < 20) notes.push_back(f["file"]);
            if (text_member(f, "kind") == "source" && code.size() < 40) code.push_back(f["file"]);
        }
        source["notes"] = std::move(notes);
        source["source"] = std::move(code);
        out["source"] = std::move(source);
    } catch (const std::exception& e) {
        out["source"] = std::string(typeid(e).name()) + ": " + e.what();
    }
    out["next"] = "disasm and xrefs at each function to look at; read_file the notes; compare_versions against another build of it";
    return fit(out);
}

// ---- jobs ------------------------------------------------------------------------------------------------------------------------

Json Server::Impl::run_command(const Json& a) {
    const std::string command = str_of(a, "command");
    std::vector<std::string> args;
    if (a.at("args").is_array())
        for (const Json& x : a["args"]) args.push_back(x.get<std::string>());
    const std::vector<std::string> known = need(s.command_names, "the command table")();
    if (std::find(known.begin(), known.end(), command) == known.end()) throw UsageError(std::format("no command {}; `commands` lists them", py_repr(command)));
    if (is_interactive(command)) throw UsageError(std::format("{} is an interface or a server; it is not run as a job", py_repr(command)));
    std::string label;
    for (const std::string& arg : args) {
        if (!arg.starts_with('-')) {
            label = sfs::path(arg).lexically_normal().filename().string();
            if (label.empty()) label = sfs::path(arg).lexically_normal().parent_path().filename().string();
            break;
        }
    }
    return service::to_json(service.jobs().launch(command, args, lowered(label), "", service::AnsweringFor::current().empty() ? link.owner() : service::AnsweringFor::current()));
}

Json Server::Impl::jobs(const Json&) {
    Json list = Json::array();
    for (const service::JobView& j : service.jobs().all()) list.push_back(service::to_json(j));
    Json out = Json::object();
    out["jobs"] = std::move(list);
    return out;
}

Json Server::Impl::job(const Json& a) {
    const std::string id = str_of(a, "job_id");
    const std::int64_t tail = int_of(a, "tail"), offset = int_of(a, "offset");
    const std::optional<service::JobView> j = service.jobs().view(id, static_cast<std::uint64_t>(tail ? 0 : std::max<std::int64_t>(0, offset)));
    if (!j) throw UsageError(std::format("no job {} (this server's jobs: `jobs`)", py_repr(id)));
    Json out = service::to_json(*j);
    const std::string log = j->log.value_or("");
    if (tail) {
        const std::size_t len = utf8_length(log), keep = static_cast<std::size_t>(std::max<std::int64_t>(1, std::min<std::int64_t>(tail, static_cast<std::int64_t>(kMost))));
        out["log"] = utf8_slice(log, len > keep ? len - keep : 0, keep);
    } else if (utf8_length(log) > kMost) {
        const std::string head = utf8_slice(log, 0, kMost);
        out["next"] = offset + static_cast<std::int64_t>(head.size());
        out["log"] = head;
    }
    return out;
}

Json Server::Impl::job_file(const Json& a) {
    const std::string id = str_of(a, "job_id");
    const auto files = service.jobs().files(id);
    std::error_code ec;
    if (!files || files->second.empty() || !sfs::is_regular_file(files->second, ec)) throw UsageError(std::format("job {} wrote no file", py_repr(id)));
    std::string text = fs::read_text(files->second);
    const std::int64_t offset = int_of(a, "offset");
    const std::int64_t n = std::max<std::int64_t>(1, std::min<std::int64_t>(int_of(a, "length"), static_cast<std::int64_t>(kMost)));
    const std::int64_t characters = static_cast<std::int64_t>(utf8_length(text));
    Json out = Json::object();
    out["path"] = files->second.string();
    out["characters"] = characters;
    out["offset"] = offset;
    out["text"] = utf8_slice(text, static_cast<std::size_t>(std::max<std::int64_t>(0, offset)), static_cast<std::size_t>(n));
    out["next"] = offset + n < characters ? Json(offset + n) : Json(nullptr);
    return out;
}

Json Server::Impl::stop_job(const Json& a) {
    const std::string id = str_of(a, "job_id");
    if (!service.jobs().stop(id)) throw UsageError(std::format("no job {}", py_repr(id)));
    const std::optional<service::JobView> j = service.jobs().view(id);
    return j ? service::to_json(*j) : Json::object();
}

// ---- writing -------------------------------------------------------------------------------------------------------------------------

Json Server::Impl::name_function(const Json& a) {
    const std::string name = str_of(a, "name");
    std::string probe = text::replace_all(text::replace_all(name, "_", "a"), "::", "a");
    const bool plain = !probe.empty() && std::all_of(probe.begin(), probe.end(), [](char c) { return std::isalnum(static_cast<unsigned char>(c)) || static_cast<unsigned char>(c) >= 0x80; });
    if (!plain) throw UsageError("a name is letters, digits, _ and ::");
    const sfs::path p = path_of(str_of(a, "module"));
    const std::string addr = str_of(a, "addr");
    const std::string number = normalize_address(addr);
    const std::string last(text::split(number, ':').back());
    const std::optional<std::int64_t> address = text::parse_int<std::int64_t>(last, 16);
    if (!address) throw UsageError(std::format("invalid literal for int() with base 16: {}", py_repr(last)));
    const std::string digest = need(s.set_name, "naming a function")(p, *address, name);
    if (digest.empty()) throw UsageError(std::format("no body recorded at {} in {} (too small to tell apart, or not a function start)", addr, p.filename().string()));
    Json out = Json::object();
    out["named"] = name;
    out["body"] = digest;
    out["modules_with_this_body"] = need(s.modules_with_body, "the function store")(digest);
    return out;
}

Json Server::Impl::propose_name(const Json& a) {
    const std::string name = str_of(a, "name");
    const sfs::path p = path_of(str_of(a, "module"));
    const std::string addr = str_of(a, "addr");
    const std::string number = normalize_address(addr);
    const std::string last(text::split(number, ':').back());
    const std::optional<std::int64_t> address = text::parse_int<std::int64_t>(last, 16);
    if (!address) throw UsageError(std::format("invalid literal for int() with base 16: {}", py_repr(last)));
    return need(s.propose_name, "proposing a name")(p, *address, name, str_of(a, "description"), int_of(a, "confidence"));
}

// ---- the dispatch ---------------------------------------------------------------------------------------------------------------------

ToolResult Server::Impl::call(std::string_view function, const Json& a) {
    ToolResult out;
    if (function == "resource") return resource(a);
    using Method = Json (Server::Impl::*)(const Json&);
    static const std::map<std::string_view, Method> methods = {
        {"describe", &Server::Impl::describe},
        {"inspect_module", &Server::Impl::inspect_module},
        {"strings", &Server::Impl::strings},
        {"hex_dump", &Server::Impl::hex_dump},
        {"modules", &Server::Impl::modules},
        {"coverage", &Server::Impl::coverage},
        {"progress", &Server::Impl::progress},
        {"functions", &Server::Impl::functions},
        {"function", &Server::Impl::function},
        {"names", &Server::Impl::names},
        {"proposals", &Server::Impl::proposals},
        {"identify", &Server::Impl::identify},
        {"demangle", &Server::Impl::demangle},
        {"commands", &Server::Impl::commands},
        {"command_help", &Server::Impl::command_help},
        {"jobs", &Server::Impl::jobs},
        {"job", &Server::Impl::job},
        {"job_file", &Server::Impl::job_file},
        {"reach_results", &Server::Impl::reach_results},
        {"projects", &Server::Impl::projects},
        {"project", &Server::Impl::project},
        {"source_tree", &Server::Impl::source_tree},
        {"read_file", &Server::Impl::read_file},
        {"decompile_function", &Server::Impl::decompile_function},
        {"compare_versions", &Server::Impl::compare_versions},
        {"diff_function", &Server::Impl::diff_function},
        {"disasm", &Server::Impl::disasm},
        {"xrefs", &Server::Impl::xrefs},
        {"triage", &Server::Impl::triage},
        {"run_command", &Server::Impl::run_command},
        {"stop_job", &Server::Impl::stop_job},
        {"name_function", &Server::Impl::name_function},
        {"propose_name", &Server::Impl::propose_name},
    };
    const auto it = methods.find(function);
    if (it == methods.end()) throw UsageError(std::format("Unknown tool: {}", function));
    out.document = std::make_shared<Json>((this->*(it->second))(a));
    return out;
}

// ---- Slow -----------------------------------------------------------------------------------------------------------------------------

Slow::Result Slow::get(const std::string& key, std::function<Json()> work, std::chrono::seconds age, std::chrono::seconds wait, bool again) {
    const std::shared_ptr<State> state = state_;
    std::shared_ptr<Task> task;
    {
        const std::lock_guard<std::mutex> hold(state->lock);
        const auto got = state->got.find(key);
        if (got != state->got.end() && !again && std::chrono::steady_clock::now() - got->second.when < age) return {got->second.value, 0};
        auto& slot = state->working[key];
        if (!slot) {
            slot = std::make_shared<Task>();
            task = slot;
            std::thread([state, key, work = std::move(work), task] {
                Json value;
                try {
                    value = work();
                } catch (const Error& e) {   // remembered, said once
                    value = Json{{"error", std::format("{}: {}", e.kind(), utf8_slice(e.what(), 0, 300))}};
                } catch (const std::exception& e) {
                    value = Json{{"error", std::format("Exception: {}", utf8_slice(e.what(), 0, 300))}};
                }
                {
                    const std::lock_guard<std::mutex> hold(state->lock);
                    state->got[key] = Got{std::chrono::steady_clock::now(), std::move(value)};
                    state->working.erase(key);
                }
                const std::lock_guard<std::mutex> done_hold(task->lock);
                task->finished = true;
                task->done.notify_all();
            }).detach();
        } else {
            task = slot;
        }
    }
    std::unique_lock<std::mutex> wait_lock(task->lock);
    task->done.wait_for(wait_lock, wait, [&] { return task->finished; });
    const bool finished = task->finished;
    const auto started = task->started;
    wait_lock.unlock();
    const std::lock_guard<std::mutex> hold(state->lock);
    if (finished) {
        const auto got = state->got.find(key);
        if (got != state->got.end()) return {got->second.value, 0};
    }
    return {std::nullopt, std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - started).count()};
}

}  // namespace cb::ui::mcp
