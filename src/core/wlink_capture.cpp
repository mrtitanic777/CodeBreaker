// core/wlink_capture.cpp -- see wlink_capture.hpp.
#include "core/wlink_capture.hpp"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "core/crypto.hpp"
#include "core/fs.hpp"

namespace cb::process::wlink_capture {

namespace stdfs = std::filesystem;

namespace {

std::string lower(std::string s) {
    for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

// The variable as the child sees it: the spec's environment when it sets one, else this process's.
std::optional<std::string> env_of(const ProcessSpec& spec, const std::string& name) {
    for (const auto& [k, v] : spec.env)
        if (lower(k) == lower(name)) return v;
    return fs::get_env(name);
}

// %VAR% expanded from the child's environment (wlink's own syntax for variables in a directive).
std::string expand(const ProcessSpec& spec, const std::string& text) {
    std::string out;
    for (std::size_t i = 0; i < text.size(); ++i) {
        const std::size_t close = text[i] == '%' ? text.find('%', i + 1) : std::string::npos;
        if (close == std::string::npos) {
            out += text[i];
            continue;
        }
        out += env_of(spec, text.substr(i + 1, close - i - 1)).value_or("");
        i = close;
    }
    return out;
}

// A directive file's words: whitespace and commas separate them, quotes keep spaces in one, `#` starts a comment.
std::vector<std::string> words_of(const std::string& text) {
    std::vector<std::string> words;
    std::string w;
    char quote = 0;
    auto flush = [&] {
        if (!w.empty()) words.push_back(w);
        w.clear();
    };
    for (std::size_t i = 0; i < text.size(); ++i) {
        const char c = text[i];
        if (quote) {
            if (c == quote) quote = 0;
            else w += c;
        } else if (c == '\'' || c == '"') {
            quote = c;
        } else if (c == '#') {
            flush();
            while (i < text.size() && text[i] != '\n') ++i;
        } else if (c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == ',') {
            flush();
        } else {
            w += c;
        }
    }
    flush();
    return words;
}

stdfs::path resolve(const stdfs::path& cwd, const std::string& p) {
    const stdfs::path path(p);
    return path.is_absolute() ? path : cwd / path;
}

bool is_file(const stdfs::path& p) {
    std::error_code ec;
    return stdfs::is_regular_file(p, ec);
}

// What a run names, read the way wlink reads its directives (the command line, @files, the system definitions).
struct Reader {
    const ProcessSpec& spec;
    stdfs::path cwd;
    std::vector<std::pair<std::string, stdfs::path>> inputs;
    std::vector<std::string> files, libraries, outputs;
    std::vector<stdfs::path> libpaths;
    std::set<std::string> seen;

    void add(const std::string& as, const stdfs::path& file) {
        std::error_code ec;
        const std::string key = lower(stdfs::weakly_canonical(file, ec).generic_string());
        if (!is_file(file) || !seen.insert(key).second) return;
        inputs.emplace_back(as, file);
    }

    void read(const std::vector<std::string>& words, int depth) {
        for (std::size_t i = 0; i < words.size(); ++i) {
            const std::string w = lower(words[i]);
            auto next = [&]() -> std::optional<std::string> {
                if (i + 1 >= words.size()) return std::nullopt;
                return expand(spec, words[++i]);
            };
            if (!w.empty() && w[0] == '@') {   // a directive file
                const std::string name = expand(spec, words[i].substr(1));
                stdfs::path p = resolve(cwd, name);
                if (!is_file(p) && p.extension().empty()) p += ".lnk";
                if (is_file(p) && depth < 8) {
                    add(name, p);
                    read(words_of(fs::read_text(p)), depth + 1);
                }
            } else if (w == "file" || w == "f") {
                if (auto v = next()) files.push_back(*v);
            } else if (w == "library" || w == "lib" || w == "l" || w == "libfile") {
                if (auto v = next()) libraries.push_back(*v);
            } else if (w == "libpath" || w == "libp") {
                if (auto v = next()) {
                    for (std::size_t a = 0, b; a <= v->size(); a = b + 1) {   // (a libpath may list several folders, ';' between)
                        b = v->find(';', a);
                        if (b == std::string::npos) b = v->size();
                        if (b > a) libpaths.push_back(resolve(cwd, v->substr(a, b - a)));
                    }
                }
            } else if (w == "name" || w == "n") {
                if (auto v = next()) outputs.push_back(*v);
            } else if ((w == "option" || w == "op") && i + 1 < words.size()) {
                const std::string o = lower(words[i + 1]);
                if (o.starts_with("map=")) outputs.push_back(words[i + 1].substr(4));
                // the resources linked in (option resource=x.res, res=): an input too
                for (const char* key : {"resource=", "res="})
                    if (o.starts_with(key)) files.push_back(words[i + 1].substr(std::string_view(key).size()));
            }
        }
    }

    // The objects and libraries named, found as wlink finds them: as named (an object without an extension is .obj), then in
    // the working folder, each libpath, and the LIB variable's folders.
    void find_inputs() {
        for (const std::string& f : files) {
            stdfs::path p = resolve(cwd, f);
            if (!is_file(p) && p.extension().empty()) p += ".obj";
            add(f, p);
        }
        std::vector<stdfs::path> dirs = {cwd};
        dirs.insert(dirs.end(), libpaths.begin(), libpaths.end());
        if (auto lib = env_of(spec, "LIB")) {
            for (std::size_t a = 0, b; a <= lib->size(); a = b + 1) {
                b = lib->find(';', a);
                if (b == std::string::npos) b = lib->size();
                if (b > a) dirs.push_back(lib->substr(a, b - a));
            }
        }
        for (const std::string& l : libraries) {
            stdfs::path name(l);
            if (name.extension().empty()) name += ".lib";
            if (name.is_absolute()) {
                add(l, name);
                continue;
            }
            for (const stdfs::path& d : dirs)
                if (is_file(d / name)) {
                    add(l, d / name);
                    break;
                }
        }
    }
};

}  // namespace

std::unique_ptr<Pending> before(const ProcessSpec& spec) {
    try {
        const auto root = fs::get_env("CB_WLINK_CAPTURE");
        if (!root || root->empty() || spec.argv.empty()) return nullptr;
        const std::string program = lower(stdfs::path(spec.argv[0]).filename().string());
        if (program != "wlink.exe" && program != "wlink") return nullptr;
        Reader r{spec, spec.cwd ? *spec.cwd : stdfs::current_path(), {}, {}, {}, {}, {}, {}};
        // the system definitions every link reads (`system dos` and the like are blocks of directives in them)
        if (auto watcom = env_of(spec, "WATCOM")) {
            for (const char* sub : {"binw", "binnt64", "binnt"}) {
                const stdfs::path sys = stdfs::path(*watcom) / sub / "wlsystem.lnk";
                if (is_file(sys)) {
                    r.add(std::string("%WATCOM%/") + sub + "/wlsystem.lnk", sys);
                    for (const std::string& w : words_of(fs::read_text(sys)))   // (their libpaths, for the libraries they name)
                        if (lower(w).find("%watcom%") != std::string::npos) r.libpaths.push_back(expand(spec, w));
                    break;
                }
            }
        }
        const std::vector<std::string> args(spec.argv.begin() + 1, spec.argv.end());
        std::string command_line;
        for (const std::string& a : args) command_line += a + '\n';
        r.read(words_of(command_line), 0);
        r.find_inputs();

        auto p = std::make_unique<Pending>();
        p->root = *root;
        p->cwd = r.cwd;
        p->started = stdfs::file_time_type::clock::now() - std::chrono::seconds(2);   // (a file system's times are coarse)
        p->inputs = std::move(r.inputs);
        p->outputs = std::move(r.outputs);
        std::sort(p->inputs.begin(), p->inputs.end());
        Bytes all(command_line.begin(), command_line.end());
        for (const auto& [as, file] : p->inputs) {
            all.insert(all.end(), as.begin(), as.end());
            const Bytes b = fs::read_file(file);
            all.insert(all.end(), b.begin(), b.end());
        }
        p->folder = p->root / crypto::to_hex(crypto::sha1(all)).substr(0, 16);
        return p;
    } catch (...) {
        return nullptr;
    }
}

void after(std::unique_ptr<Pending> pending, const ProcessSpec& spec, const ProcessResult& result) {
    if (!pending) return;
    try {
        std::error_code ec;
        if (stdfs::exists(pending->folder / "case.json", ec)) return;   // (seen before)
        fs::make_dirs(pending->folder / "out");
        fs::make_dirs(pending->root / "blobs");
        nlohmann::json files = nlohmann::json::array();   // (each input: its path as the run named it, where it was, its content)
        for (const auto& [as, file] : pending->inputs) {
            const Bytes bytes = fs::read_file(file);
            const std::string sha = crypto::to_hex(crypto::sha1(bytes));
            const stdfs::path blob = pending->root / "blobs" / sha;
            if (!stdfs::exists(blob, ec)) fs::write_file(blob, bytes);
            files.push_back({{"as", as}, {"path", file.generic_string()}, {"sha1", sha}, {"size", bytes.size()}});
        }
        nlohmann::json made = nlohmann::json::array();
        for (const std::string& o : pending->outputs) {
            stdfs::path path = resolve(pending->cwd, o);
            if (!is_file(path)) continue;
            if (stdfs::last_write_time(path, ec) < pending->started) continue;   // (left there by an earlier run)
            fs::write_file(pending->folder / "out" / path.filename(), fs::read_file(path));
            made.push_back({{"as", o}, {"file", path.filename().string()}});
        }
        nlohmann::json env = nlohmann::json::object();
        for (const char* name : {"WATCOM", "LIB", "INCLUDE"})
            if (auto v = env_of(spec, name)) env[name] = *v;
        nlohmann::json c;
        c["args"] = std::vector<std::string>(spec.argv.begin() + 1, spec.argv.end());
        c["cwd"] = pending->cwd.generic_string();
        c["env"] = env;
        c["exit"] = result.exit_code;
        c["stdout"] = std::string(result.out.begin(), result.out.end());
        c["stderr"] = std::string(result.err.begin(), result.err.end());
        c["outputs"] = made;
        c["inputs"] = files;
        fs::write_text(pending->folder / "case.json", c.dump(1));
    } catch (...) {
        // (capturing never changes what the run does)
    }
}

}  // namespace cb::process::wlink_capture
