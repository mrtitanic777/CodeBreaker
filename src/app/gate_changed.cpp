// app/gate_changed.cpp -- see gate_changed.hpp.
#include "app/gate_changed.hpp"

#include <algorithm>
#include <charconv>
#include <format>

#include "core/crypto.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"

namespace cb::app::gate_changed {

namespace stdfs = std::filesystem;

namespace {

std::string slashes(std::string s) {
    std::replace(s.begin(), s.end(), '\\', '/');
    return s;
}

bool starts(std::string_view s, std::string_view p) { return s.substr(0, p.size()) == p; }

// "app:window" -> "app": the node of the include graph.
std::string node_of(const std::string& area) { return area.substr(0, area.find(':')); }

// Folders that are build output or somebody else's code, never part of what the gate depends on.
// ("build" is output next to other people's code, but a folder of ours under src (the build area), config and tests.)
bool skipped_folder(const std::string& name, const std::string& rel) {
    if (name == "build") return !(starts(rel, "src/") || starts(rel, "config/") || starts(rel, "tests/"));
    return name == ".git" || name == "state" || name == ".gradle" || name == "__pycache__" || name == "node_modules";
}

void walk(const stdfs::path& repo, const stdfs::path& root, bool recurse, const Record* previous, Record& out,
          const std::map<std::string, std::string>& extra) {
    std::error_code ec;
    if (!stdfs::exists(root, ec)) return;
    auto stamp_file = [&](const stdfs::path& file) {
        std::error_code e;
        const std::string rel = slashes(fs::to_text(stdfs::relative(file, repo, e)));
        if (rel == "tests/exe_gate/cases.txt") return;   // its lines are the entries "case:NAME"
        FileStamp st;
        st.size = static_cast<std::uint64_t>(stdfs::file_size(file, e));
        st.mtime = fs::last_write_time(file, e).time_since_epoch().count();
        if (previous != nullptr) {
            const auto it = previous->files.find(rel);
            if (it != previous->files.end() && it->second.size == st.size && it->second.mtime == st.mtime) {
                out.files[rel] = it->second;
                return;
            }
        }
        try {
            const Bytes data = fs::read_file(file);
            st.sha1 = crypto::to_hex(crypto::sha1(data));
        } catch (const std::exception&) {
            st.sha1 = "unreadable";
        }
        out.files[rel] = st;
    };
    if (stdfs::is_regular_file(root, ec)) {
        stamp_file(root);
        return;
    }
    if (!recurse) {
        for (stdfs::directory_iterator it(root, ec), end; !ec && it != end; it.increment(ec))
            if (it->is_regular_file(ec)) stamp_file(it->path());
        return;
    }
    for (stdfs::recursive_directory_iterator it(root, ec), end; !ec && it != end; it.increment(ec)) {
        std::error_code er;
        if (it->is_directory(ec) && skipped_folder(fs::to_text(it->path().filename()), slashes(fs::to_text(stdfs::relative(it->path(), repo, er))))) {
            it.disable_recursion_pending();
            continue;
        }
        if (it->is_regular_file(ec)) stamp_file(it->path());
    }
    (void)extra;
}

}  // namespace

std::string record_text(const Record& record) {
    std::string out = "gate-record 1 " + record.commit + "\n";
    for (const auto& [path, s] : record.files) out += std::format("{}\t{}\t{}\t{}\n", path, s.size, s.mtime, s.sha1);
    return out;
}

Record read_record(std::string_view data) {
    Record r;
    bool first = true;
    for (std::string_view line : text::lines(data)) {
        if (first) {
            first = false;
            const std::vector<std::string_view> w = text::split(line, ' ');
            if (w.size() < 2 || w[0] != "gate-record" || w[1] != "1") return {};   // another format: no record (everything runs)
            if (w.size() > 2) r.commit = std::string(w[2]);
            continue;
        }
        const std::vector<std::string_view> p = text::split(line, '\t');
        if (p.size() != 4) continue;
        FileStamp s;
        std::from_chars(p[1].data(), p[1].data() + p[1].size(), s.size);
        std::from_chars(p[2].data(), p[2].data() + p[2].size(), s.mtime);
        s.sha1 = std::string(p[3]);
        r.files[std::string(p[0])] = s;
    }
    return r;
}

Record scan_tree(const stdfs::path& repo, const Record* previous, const std::map<std::string, std::string>& extra) {
    Record out;
    const stdfs::path native = repo / "src";
    for (const char* dir : {"src", "config", "tests"})
        walk(repo, repo / dir, true, previous, out, extra);
    walk(repo, native, false, previous, out, extra);   // src/CMakeLists.txt and the *.cmake beside it
    walk(repo, repo / "tools", false, previous, out, extra);
    for (const auto& [name, sha] : extra) {
        FileStamp s;
        s.sha1 = sha;
        out.files[name] = s;
    }
    return out;
}

std::vector<std::string> changed_paths(const Record& was, const Record& now) {
    std::vector<std::string> out;
    for (const auto& [path, s] : now.files) {
        const auto it = was.files.find(path);
        // (equal content is unchanged whatever the write time: a file saved again or a branch switched back)
        if (it == was.files.end() || it->second.sha1 != s.sha1) out.push_back(path);
    }
    for (const auto& [path, s] : was.files)
        if (!now.files.contains(path)) out.push_back(path);
    return out;
}

// ---- the include graph ---------------------------------------------------------------------------------------------------

Graph graph_of(const std::vector<std::pair<std::string, std::string>>& sources) {
    Graph g;
    for (const auto& [path, body] : sources) {
        const std::string p = slashes(path);
        const std::size_t at = p.find("src/");
        if (at == std::string::npos) continue;
        const std::string rest = p.substr(at + 4);
        const std::size_t slash = rest.find('/');
        if (slash == std::string::npos) continue;   // a file at the top of src (core16_types_test)
        const std::string area = rest.substr(0, slash);
        const bool is_test = rest.ends_with("_test.cpp") || rest.ends_with("_main.cpp");
        g.all[area];
        if (!is_test) g.code[area];
        for (std::string_view line : text::lines(body)) {
            const std::string_view t = text::trim(line);
            if (!starts(t, "#include \"cb/")) continue;
            const std::string_view target = t.substr(13);
            const std::size_t end = target.find('/');
            if (end == std::string_view::npos) continue;
            const std::string other(target.substr(0, end));
            if (other == area) continue;
            g.all[area].insert(other);
            if (!is_test) g.code[area].insert(other);
        }
    }
    return g;
}

Graph read_graph(const stdfs::path& repo) {
    std::vector<std::pair<std::string, std::string>> sources;
    std::error_code ec;
    for (stdfs::recursive_directory_iterator it(repo / "src", ec), end; !ec && it != end; it.increment(ec)) {
        if (!it->is_regular_file(ec)) continue;
        const std::string ext = fs::to_text(it->path().extension());
        if (ext != ".cpp" && ext != ".hpp") continue;
        try {
            sources.emplace_back(fs::to_text(it->path()), fs::read_text(it->path()));
        } catch (const std::exception&) {   // isolation boundary: an unreadable file adds no edge (the scan hashed it already)
        }
    }
    return graph_of(sources);
}

std::set<std::string> reach_down(const std::map<std::string, std::set<std::string>>& edges, const std::set<std::string>& seeds) {
    std::set<std::string> seen;
    std::vector<std::string> todo(seeds.begin(), seeds.end());
    while (!todo.empty()) {
        const std::string a = todo.back();
        todo.pop_back();
        if (!seen.insert(a).second) continue;
        const auto it = edges.find(a);
        if (it == edges.end()) continue;
        for (const std::string& b : it->second) todo.push_back(b);
    }
    return seen;
}

std::set<std::string> reach_up(const std::map<std::string, std::set<std::string>>& edges, const std::set<std::string>& targets) {
    std::set<std::string> seen = targets;
    for (bool grew = true; grew;) {
        grew = false;
        for (const auto& [area, to] : edges) {
            if (seen.contains(area)) continue;
            for (const std::string& t : to)
                if (seen.contains(t)) {
                    seen.insert(area);
                    grew = true;
                    break;
                }
        }
    }
    return seen;
}

// ---- the command map -----------------------------------------------------------------------------------------------------

CommandAreas parse_areas(std::string_view data) {
    CommandAreas m;
    for (std::string_view raw : text::lines(data)) {
        const std::string_view line = text::trim(raw);
        if (line.empty() || line[0] == '#') continue;
        const std::size_t colon = line.find(':');
        if (colon == std::string_view::npos) continue;
        std::vector<std::string>& areas = m.by_command[std::string(text::trim(line.substr(0, colon)))];
        for (std::string_view a : text::split_ws(line.substr(colon + 1))) areas.emplace_back(a);
    }
    return m;
}

// ---- selection -----------------------------------------------------------------------------------------------------------

namespace {

bool is_everything_in_app(std::string_view file) {
    // the window's own files and the convert pipeline are named below; every other file of app/ is the command table, its wiring and the program's start
    return !(starts(file, "window_") || starts(file, "tools") || starts(file, "settings_edit") || starts(file, "convert") || file == "window.hpp");
}

}  // namespace

Selection select(const std::vector<std::string>& changed) {
    Selection s;
    auto all = [&](const std::string& path) {
        if (!s.everything) s.why = path;
        s.everything = true;
    };
    for (const std::string& path : changed) {
        if (starts(path, "case:")) {
            s.cases.insert(path.substr(5));
        } else if (path == "case-vars") {
            all(path);
        } else if (starts(path, "tests/exe_gate/golden/")) {
            std::string name = path.substr(22);
            if (name.ends_with(".txt")) name.resize(name.size() - 4);
            s.cases.insert(name);
        } else if (starts(path, "src/")) {
            const std::string rest = path.substr(4);
            const std::size_t slash = rest.find('/');
            if (slash == std::string::npos) {   // src/CMakeLists.txt, core16_types.hpp
                all(path);
                continue;
            }
            const std::string area = rest.substr(0, slash);
            const std::string file = rest.substr(slash + 1);
            if (rest.find("/testdata/") != std::string::npos || area == "core" || area == "base" || area == "third_party") {   // (third_party: the libraries every area links)
                all(path);
            } else if (area == "app") {
                if (file.find('/') != std::string::npos) all(path);   // app/res/
                else if (file == "exe_gate_main.cpp" || starts(file, "gate_changed")) all(path);
                else if (file.ends_with("_test.cpp") && !starts(file, "commands") && !starts(file, "native_commands")) {
                    // a test file of the window or the convert pipeline: those tests only
                    s.unit_only.insert("app");
                } else if (is_everything_in_app(file)) all(path);
                else if (starts(file, "convert")) s.areas.insert("app:convert");
                else s.areas.insert("app:window");
            } else if (file.ends_with("_test.cpp")) {
                s.unit_only.insert(area);
            } else {
                s.areas.insert(area);
            }
        } else if (starts(path, "config/")) {
            const std::string rest = path.substr(7);
            const std::size_t slash = rest.find('/');
            static const std::set<std::string> owned = {"build", "exact", "java", "knowledge", "port", "source", "symbols", "translate", "xna"};
            const std::string owner = slash == std::string::npos ? std::string() : rest.substr(0, slash);
            if (owned.contains(owner)) s.areas.insert(owner);
            else all(path);
        } else {
            all(path);   // tools/, src/CMakeLists.txt, third_party, tests/exe_gate/in, tests/fixtures, any other native/ folder
        }
    }
    return s;
}

std::set<std::string> unit_areas(const Selection& selection, const Graph& graph) {
    std::set<std::string> nodes;
    for (const std::string& a : selection.areas) nodes.insert(node_of(a));
    std::set<std::string> out = reach_up(graph.all, nodes);
    out.insert(selection.unit_only.begin(), selection.unit_only.end());
    return out;
}

bool case_selected(const Selection& selection, const Graph& graph, const CommandAreas& map, const std::string& name, const std::string& command,
                   const std::vector<std::string>& extra) {
    if (selection.everything || selection.cases.contains(name)) return true;
    if (selection.areas.empty()) return false;
    std::vector<std::string> seeds = extra;
    const auto it = map.by_command.find(command);
    if (it != map.by_command.end()) seeds.insert(seeds.end(), it->second.begin(), it->second.end());
    if (seeds.empty()) return true;   // a command nobody described: unsure, so it runs
    std::set<std::string> reached;
    for (const std::string& seed : seeds) {
        reached.insert(seed);
        for (const std::string& a : reach_down(graph.code, {node_of(seed)})) reached.insert(a);
    }
    for (const std::string& a : selection.areas)
        if (reached.contains(a)) return true;
    return false;
}

}  // namespace cb::app::gate_changed
