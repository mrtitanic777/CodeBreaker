// base/tool_catalog.cpp -- see tool_catalog.hpp.
#include "base/tool_catalog.hpp"

#include <algorithm>
#include <chrono>

#include <nlohmann/json.hpp>

#include "base/host_tools.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/process.hpp"
#include "core/text.hpp"

namespace stdfs = std::filesystem;

namespace cb::config::tools {

namespace {

bool tc_exists(const stdfs::path& p) {
    std::error_code ec;
    return !p.empty() && stdfs::exists(p, ec);
}

bool tc_is_dir(const stdfs::path& p) {
    std::error_code ec;
    return !p.empty() && stdfs::is_directory(p, ec);
}

std::string tc_string(const Json& j, const char* key) {
    const auto it = j.find(key);
    return it != j.end() && it->is_string() ? it->get<std::string>() : std::string();
}

std::vector<std::string> tc_strings(const Json& j, const char* key) {
    std::vector<std::string> out;
    const auto it = j.find(key);
    if (it != j.end() && it->is_array())
        for (const Json& v : *it)
            if (v.is_string()) out.push_back(v.get<std::string>());
    return out;
}

Tool tc_tool(const Json& j) {
    Tool t;
    t.id = tc_string(j, "id");
    t.name = tc_string(j, "name");
    t.role = tc_string(j, "role");
    t.description = tc_string(j, "description");
    t.setting = tc_string(j, "setting");
    t.value = tc_string(j, "value");
    t.needed_for = tc_string(j, "needed_for");
    t.download = tc_string(j, "download");
    t.pick = tc_string(j, "pick");
    t.built_in = j.value("built_in", false);
    t.developer = j.value("developer", false);
    t.order = j.value("order", 1000);
    t.needs = tc_strings(j, "requires");
    t.needs_any = tc_strings(j, "requires_any");
    if (const auto v = j.find("version"); v != j.end() && v->is_object()) {
        t.version_file = tc_string(*v, "file");
        t.version_key = tc_string(*v, "key");
        t.least = v->value("least", 0);
    }
    if (const auto f = j.find("find"); f != j.end() && f->is_array())
        for (const Json& r : *f) {
            if (!r.is_object()) continue;
            Rule rule;
            for (const char* kind : {"env", "program_files", "user", "dir", "on_path", "vswhere", "under_tool"})
                if (r.contains(kind) && r[kind].is_string()) {
                    rule.kind = kind;
                    rule.what = r[kind].get<std::string>();
                }
            if (rule.kind.empty()) continue;
            rule.sub = tc_string(r, "sub");
            rule.up = r.value("up", 0);
            rule.children = r.value("children", false);
            t.find.push_back(std::move(rule));
        }
    if (t.name.empty()) t.name = t.id;
    return t;
}

std::optional<stdfs::path> tc_env_path(std::string_view name) {
    const std::optional<std::string> v = fs::get_env(name);
    if (!v || text::trim(*v).empty()) return std::nullopt;
    return fs::from_text(std::string(text::trim(*v, "\" ")));
}

std::vector<stdfs::path> tc_program_files() {
    std::vector<stdfs::path> out;
    for (const char* name : {"ProgramFiles", "ProgramW6432", "ProgramFiles(x86)"})
        if (const auto p = tc_env_path(name); p && std::find(out.begin(), out.end(), *p) == out.end()) out.push_back(*p);
    if (out.empty()) out.push_back("C:/Program Files");
    return out;
}

std::vector<stdfs::path> tc_path_dirs() {
    std::vector<stdfs::path> out;
    const std::string path_var = fs::get_env("PATH").value_or("");   // (kept: the pieces are views of it)
    for (std::string_view dir : text::split(path_var, ';')) {
        dir = text::trim(dir, "\" ");
        if (!dir.empty()) out.push_back(fs::from_text(std::string(dir)));
    }
    return out;
}

std::vector<stdfs::path> tc_children(const stdfs::path& dir) {
    std::vector<stdfs::path> out;
    std::error_code ec;
    if (!tc_is_dir(dir)) return out;
    for (stdfs::directory_iterator it(dir, ec), end; !ec && it != end; it.increment(ec))
        if (it->is_directory(ec)) out.push_back(it->path());
    std::sort(out.begin(), out.end());
    return out;
}

std::optional<stdfs::path> tc_vswhere(const std::string& component) {
    for (const stdfs::path& pf : tc_program_files()) {
        const stdfs::path vswhere = pf / "Microsoft Visual Studio" / "Installer" / "vswhere.exe";
        if (!tc_exists(vswhere)) continue;
        try {
            const process::ProcessResult r = process::run({.argv = {fs::to_text(vswhere), "-latest", "-prerelease", "-products", "*", "-requires", component,
                                                                    "-property", "installationPath", "-utf8"},
                                                           .timeout = std::chrono::seconds(20)});
            const std::string out(text::trim(std::string_view(reinterpret_cast<const char*>(r.out.data()), r.out.size())));
            const std::string first(text::trim(out.substr(0, out.find('\n'))));
            if (r.ok() && !first.empty() && tc_is_dir(fs::from_text(first))) return fs::from_text(first);
        } catch (const std::exception&) {
        }
    }
    return std::nullopt;
}

// The candidate folders one rule names, each with where it came from.
std::vector<std::pair<stdfs::path, std::string>> tc_candidates(const Rule& r, const std::map<std::string, stdfs::path>& found) {
    std::vector<std::pair<stdfs::path, std::string>> out;
    const auto add = [&](const stdfs::path& base, const std::string& how) {
        if (r.children)
            for (const stdfs::path& c : tc_children(base)) out.emplace_back(c, how);
        else
            out.emplace_back(base, how);
    };
    if (r.kind == "env") {
        if (const auto p = tc_env_path(r.what)) add(*p, r.what);
    } else if (r.kind == "program_files") {
        for (const stdfs::path& pf : tc_program_files()) add(pf / fs::from_text(r.what), fs::to_text(pf / fs::from_text(r.what)));
    } else if (r.kind == "user") {
        if (const auto user = tc_env_path("USERPROFILE")) add(*user / fs::from_text(r.what), "~/" + r.what);
    } else if (r.kind == "dir") {
        add(fs::from_text(r.what), r.what);
    } else if (r.kind == "on_path") {
        for (const stdfs::path& dir : tc_path_dirs())
            if (tc_exists(dir / fs::from_text(r.what))) {
                stdfs::path home = dir;
                for (int i = 0; i < r.up; ++i) home = home.parent_path();
                add(home, "PATH");
            }
    } else if (r.kind == "vswhere") {
        if (const auto vs = tc_vswhere(r.what)) add(*vs, "Visual Studio Installer");
    } else if (r.kind == "under_tool") {
        if (const auto it = found.find(r.what); it != found.end()) add(it->second / fs::from_text(r.sub), "with " + r.what);
    }
    return out;
}

std::optional<int> tc_version(const Tool& t, const stdfs::path& home) {
    if (t.version_file.empty() || t.version_key.empty()) return std::nullopt;
    std::string body;
    try {
        body = fs::read_text(home / fs::from_text(t.version_file));
    } catch (const std::exception&) {
        return std::nullopt;
    }
    const std::string prefix = t.version_key + "=";
    for (std::string_view line : text::lines(body)) {
        line = text::trim(line);
        if (line.starts_with(prefix)) return host_tools::java_major(line.substr(prefix.size()));
    }
    return std::nullopt;
}

}  // namespace

std::vector<Tool> load_catalog(const stdfs::path& dir) {
    std::vector<Tool> out;
    std::error_code ec;
    if (!tc_is_dir(dir)) return out;
    for (stdfs::directory_iterator it(dir, ec), end; !ec && it != end; it.increment(ec)) {
        if (it->path().extension() != ".json") continue;
        try {
            const Json j = json::load(it->path());
            if (!j.is_object()) continue;
            Tool t = tc_tool(j);
            if (!t.id.empty()) out.push_back(std::move(t));
        } catch (const std::exception&) {
        }
    }
    std::sort(out.begin(), out.end(), [](const Tool& a, const Tool& b) { return a.order != b.order ? a.order < b.order : a.name < b.name; });
    return out;
}

std::vector<Tool> catalog_of(const stdfs::path& checkout_root) { return load_catalog(checkout_root / "config" / "tools"); }

stdfs::path home_of(const Tool& tool, const stdfs::path& value) {
    if (tool.value.empty()) return value;
    const std::string v = text::lower(fs::to_text(value.lexically_normal()));
    const std::string tail = text::lower(fs::to_text(fs::from_text(tool.value).lexically_normal()));
    if (v.size() > tail.size() && v.ends_with(tail)) {
        stdfs::path home = value;
        for (const auto& part : fs::from_text(tool.value)) {
            (void)part;
            home = home.parent_path();
        }
        return home;
    }
    return value;
}

Check check(const Tool& tool, const stdfs::path& home) {
    Check c;
    if (!tc_is_dir(home)) {
        c.problem = fs::to_text(home) + " is not there";
        return c;
    }
    for (const std::string& r : tool.needs)
        if (!tc_exists(home / fs::from_text(r))) {
            c.problem = r + " is not there";
            return c;
        }
    if (!tool.needs_any.empty()
        && std::none_of(tool.needs_any.begin(), tool.needs_any.end(), [&](const std::string& r) { return tc_exists(home / fs::from_text(r)); })) {
        c.problem = tool.needs_any.front() + " is not there";
        return c;
    }
    c.version = tc_version(tool, home);
    if (tool.least > 0) {
        if (!c.version) {
            c.problem = "its version could not be read";
            return c;
        }
        if (*c.version < tool.least) {
            c.problem = "version " + std::to_string(*c.version) + " is older than " + std::to_string(tool.least);
            return c;
        }
    }
    c.ok = true;
    return c;
}

std::vector<Detected> scan(const std::vector<Tool>& catalog) {
    std::vector<Detected> out;
    std::map<std::string, stdfs::path> found;   // the homes found so far (under_tool looks inside them)
    for (const Tool& t : catalog) {
        if (t.built_in || t.find.empty()) continue;
        std::optional<Detected> best;
        std::vector<stdfs::path> seen;
        for (const Rule& r : t.find) {
            for (auto& [home, how] : tc_candidates(r, found)) {
                const stdfs::path h = home.lexically_normal();
                if (std::find(seen.begin(), seen.end(), h) != seen.end()) continue;
                seen.push_back(h);
                const Check c = check(t, h);
                if (!c.ok) continue;
                if (best && !(t.pick == "newest" && c.version.value_or(0) > best->version.value_or(0))) continue;
                const stdfs::path value = t.value.empty() ? h : h / fs::from_text(t.value);
                best = Detected{t.id, h, fs::to_text(value.lexically_normal()), c.version, how};
            }
            if (best && t.pick != "newest") break;
        }
        if (!best) continue;
        found[t.id] = best->home;
        out.push_back(std::move(*best));
    }
    return out;
}

stdfs::path attached_file(const stdfs::path& main_checkout) {
    if (const auto named = fs::get_env("CB_TOOLS_FILE"); named && !named->empty()) return fs::from_text(*named);
    return main_checkout / "config" / "attached_tools.json";
}

std::map<std::string, Attached> read_attached(const stdfs::path& file) {
    std::map<std::string, Attached> out;
    try {
        if (!tc_exists(file)) return out;
        const Json doc = json::load(file);
        const auto it = doc.find("tools");
        if (it == doc.end() || !it->is_object()) return out;
        for (const auto& [id, v] : it->items())
            if (v.is_object()) out[id] = Attached{tc_string(v, "value"), tc_string(v, "how"), tc_string(v, "version")};
    } catch (const std::exception&) {
    }
    return out;
}

void write_attached(const stdfs::path& file, const std::map<std::string, Attached>& tools) {
    Json all = Json::object();
    for (const auto& [id, a] : tools) {
        Json v = Json{{"value", a.value}, {"how", a.how}};
        if (!a.version.empty()) v["version"] = a.version;
        all[id] = std::move(v);
    }
    fs::make_dirs(file.parent_path());
    json::save(file, Json{{"tools", all}});
}

}  // namespace cb::config::tools
