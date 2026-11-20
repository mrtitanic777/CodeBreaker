#include "cb/core/nasm_capture.hpp"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <set>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"

namespace cb::process::nasm_capture {

namespace stdfs = std::filesystem;


namespace {

std::string lower(std::string s) {
    for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

bool is_source(const stdfs::path& p) {
    const std::string e = lower(p.extension().string());
    return e == ".asm" || e == ".inc" || e == ".mac" || e == ".s" || e == ".def" || e == ".i";
}

// The text files of one folder (not below it).
void sources_of(const stdfs::path& dir, const std::string& prefix, std::vector<std::pair<std::string, Bytes>>& out) {
    std::error_code ec;
    for (const auto& e : stdfs::directory_iterator(dir, ec)) {
        if (!e.is_regular_file(ec) || !is_source(e.path())) continue;
        out.emplace_back(prefix + e.path().filename().string(), fs::read_file(e.path()));
    }
}

// The value after `flag` in argv (`-o x`), or the text right after it (`-ox`).
std::vector<std::string> values_of(const std::vector<std::string>& argv, std::string_view flag) {
    std::vector<std::string> v;
    for (std::size_t i = 1; i < argv.size(); ++i) {
        if (argv[i] == flag && i + 1 < argv.size()) v.push_back(argv[++i]);
        else if (argv[i].size() > flag.size() && argv[i].starts_with(flag)) v.push_back(argv[i].substr(flag.size()));
    }
    return v;
}

stdfs::path resolve(const stdfs::path& cwd, const std::string& p) {
    const stdfs::path path(p);
    return path.is_absolute() ? path : cwd / path;
}

}  // namespace

std::unique_ptr<Pending> before(const ProcessSpec& spec) {
    try {
        const auto root = fs::get_env("CB_NASM_CAPTURE");
        if (!root || root->empty() || spec.argv.empty()) return nullptr;
        // (CodeBreaker's own NASM -- native/nasm's build, what x86asm replaces -- not another assembler a lane probes)
        const std::string program = lower(stdfs::path(spec.argv[0]).generic_string());
        if (!program.ends_with("native/nasm/build/bin/nasm.exe") && !program.ends_with("native/nasm/build/bin/nasm")) return nullptr;
        auto p = std::make_unique<Pending>();
        p->cwd = spec.cwd ? *spec.cwd : stdfs::current_path();
        p->started = stdfs::file_time_type::clock::now() - std::chrono::seconds(2);   // (a file system's times are coarse)
        std::vector<stdfs::path> dirs = {p->cwd};
        for (const std::string& inc : values_of(spec.argv, "-I")) dirs.push_back(resolve(p->cwd, inc));
        std::set<std::string> seen;
        auto add = [&](const stdfs::path& file) {
            std::error_code ec;
            const stdfs::path full = stdfs::weakly_canonical(file, ec);
            const std::string key = (ec ? file : full).generic_string();
            if (!seen.insert(lower(key)).second || !stdfs::is_regular_file(file, ec)) return;
            p->inputs.emplace_back(key, fs::read_file(file));
        };
        for (const stdfs::path& d : dirs) {   // (the sources of the working folder and each -I folder)
            std::vector<std::pair<std::string, Bytes>> here;
            sources_of(d, "", here);
            for (const auto& [name, bytes] : here) add(d / name);
        }
        for (std::size_t i = 1; i < spec.argv.size(); ++i)   // (a source named by its path elsewhere)
            if (is_source(stdfs::path(spec.argv[i]))) add(resolve(p->cwd, spec.argv[i]));
        // what the sources pull in by name: `incbin "file"`, `%include "file"`, MASM's `include file` -- found as NASM finds
        // them (as named, then in the working folder and each -I folder)
        for (std::size_t k = 0; k < p->inputs.size(); ++k) {
            const std::string text(p->inputs[k].second.begin(), p->inputs[k].second.end());
            std::size_t at = 0;
            while (at < text.size()) {
                std::size_t end = text.find('\n', at);
                if (end == std::string::npos) end = text.size();
                std::string line = text.substr(at, end - at);
                at = end + 1;
                const std::string l = lower(line);
                std::size_t w = l.find_first_not_of(" \t");
                if (w == std::string::npos) continue;
                std::size_t arg = std::string::npos;
                for (const std::string_view kw : {"incbin", "%include", "include"})
                    if (l.compare(w, kw.size(), kw) == 0 && w + kw.size() < l.size() && (l[w + kw.size()] == ' ' || l[w + kw.size()] == '\t'))
                        arg = l.find_first_not_of(" \t", w + kw.size());
                if (arg == std::string::npos) continue;
                std::string name;
                if (line[arg] == '"' || line[arg] == '\'') {
                    const std::size_t close = line.find(line[arg], arg + 1);
                    if (close == std::string::npos) continue;
                    name = line.substr(arg + 1, close - arg - 1);
                } else {
                    const std::size_t stop = line.find_first_of(" \t,;\r", arg);
                    name = line.substr(arg, stop == std::string::npos ? std::string::npos : stop - arg);
                }
                if (name.empty()) continue;
                std::error_code ec;
                if (stdfs::path(name).is_absolute()) { add(name); continue; }
                for (const stdfs::path& d : dirs)
                    if (stdfs::is_regular_file(d / name, ec)) { add(d / name); break; }
            }
        }
        std::sort(p->inputs.begin(), p->inputs.end());
        std::string key;
        for (std::size_t i = 1; i < spec.argv.size(); ++i) key += spec.argv[i] + '\n';
        Bytes all(key.begin(), key.end());
        for (const auto& [name, bytes] : p->inputs) {
            all.insert(all.end(), name.begin(), name.end());
            all.insert(all.end(), bytes.begin(), bytes.end());
        }
        p->folder = stdfs::path(*root) / crypto::to_hex(crypto::sha1(all)).substr(0, 16);
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
        fs::make_dirs(pending->folder / "in");
        fs::make_dirs(pending->folder / "out");
        nlohmann::json files = nlohmann::json::array();   // (each input: its path as the run saw it, and where it is kept)
        for (std::size_t i = 0; i < pending->inputs.size(); ++i) {
            const auto& [path, bytes] = pending->inputs[i];
            const std::string kept = std::to_string(i) + "_" + stdfs::path(path).filename().string();
            fs::write_file(pending->folder / "in" / kept, bytes);
            files.push_back({{"path", path}, {"file", kept}});
        }
        std::vector<std::string> outputs = values_of(spec.argv, "-o");
        for (const std::string& l : values_of(spec.argv, "-l")) outputs.push_back(l);
        // a map the source asks for: `[map symbols FILE]` / `[map all FILE]`
        for (const auto& [name, bytes] : pending->inputs) {
            const std::string text(bytes.begin(), bytes.end());
            for (std::size_t k = text.find("[map "); k != std::string::npos; k = text.find("[map ", k + 5)) {
                const std::size_t close = text.find(']', k);
                const std::size_t space = text.rfind(' ', close);
                if (close != std::string::npos && space != std::string::npos && space > k + 4) outputs.push_back(text.substr(space + 1, close - space - 1));
            }
        }
        nlohmann::json made = nlohmann::json::array();
        for (const std::string& o : outputs) {
            const stdfs::path path = resolve(pending->cwd, o);
            if (!stdfs::is_regular_file(path, ec)) continue;
            if (stdfs::last_write_time(path, ec) < pending->started) continue;   // (left there by an earlier run)
            fs::write_file(pending->folder / "out" / path.filename(), fs::read_file(path));
            made.push_back({{"as", o}, {"file", path.filename().string()}});
        }
        nlohmann::json c;
        std::vector<std::string> args(spec.argv.begin() + 1, spec.argv.end());
        c["args"] = args;
        c["cwd"] = pending->cwd.string();
        c["exit"] = result.exit_code;
        c["stdout"] = std::string(result.out.begin(), result.out.end());
        c["stderr"] = std::string(result.err.begin(), result.err.end());
        c["outputs"] = made;
        c["files"] = files;
        fs::write_text(pending->folder / "case.json", c.dump(1));
    } catch (...) {
        // (capturing never changes what the run does)
    }
}

}  // namespace cb::process::nasm_capture
