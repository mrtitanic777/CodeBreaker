#include "cb/decompile/jar.hpp"

#include <algorithm>
#include <format>
#include <system_error>

#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/process.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"

namespace cb::decompile::jar {

namespace stdfs = std::filesystem;

namespace {

bool is_java_file(const stdfs::directory_entry& e, std::error_code& ec) {
    return e.is_regular_file(ec) && e.path().extension() == ".java";
}

// How many .java files lie under `dir`, at any depth. Unreadable directories count nothing.
std::size_t count_java_files(const stdfs::path& dir) {
    std::size_t n = 0;
    std::error_code ec;
    for (stdfs::recursive_directory_iterator it(dir, stdfs::directory_options::skip_permission_denied, ec), end;
         !ec && it != end; it.increment(ec)) {
        std::error_code entry_ec;
        if (is_java_file(*it, entry_ec)) ++n;
    }
    return n;
}

// The directories named `src` at or under `dir`, parents before children, siblings in name order.
void collect_src_dirs(const stdfs::path& dir, std::vector<stdfs::path>& out) {
    if (dir.filename() == "src") out.push_back(dir);
    std::vector<stdfs::path> children;
    std::error_code ec;
    for (stdfs::directory_iterator it(dir, stdfs::directory_options::skip_permission_denied, ec), end;
         !ec && it != end; it.increment(ec)) {
        std::error_code entry_ec;
        if (it->is_directory(entry_ec) && !it->is_symlink(entry_ec)) children.push_back(it->path());
    }
    std::sort(children.begin(), children.end());
    for (const stdfs::path& child : children) collect_src_dirs(child, out);
}

// `name` on PATH: the first directory holding name + one of PATHEXT's extensions. Does not look in the current
// directory (a program of that name there is not what a user asking for `java` means).
std::optional<stdfs::path> which(std::string_view name) {
    const std::string path_var = fs::get_env("PATH").value_or("");
    const std::string ext_var = fs::get_env("PATHEXT").value_or(".COM;.EXE;.BAT;.CMD");
    for (std::string_view dir : text::split(path_var, ';')) {
        dir = text::trim(dir, "\" ");
        if (dir.empty()) continue;
        for (std::string_view ext : text::split(ext_var, ';')) {
            if (ext.empty()) continue;
            stdfs::path candidate = fs::from_text(dir) / fs::from_text(std::string(name) + std::string(ext));
            std::error_code ec;
            if (stdfs::is_regular_file(candidate, ec)) return candidate;
        }
    }
    return std::nullopt;
}

// A best-effort append to the run's log: a log that cannot be written is skipped, never fatal.
void append_log(const stdfs::path& log, std::string_view text) {
    if (text.empty() || !stdfs::is_directory(log.parent_path())) return;   // (never create the workspace for a log)
    try {
        fs::append_file(log, as_bytes(text));
    } catch (const IoError&) {
        // (the workspace may be unwritable or the log locked; the run does not depend on it)
    }
}

// The mangled array parameter `Block[] block;4` / `byte[] [B4`: type, junk name, the trailing digits the body's
// `varN` uses. Followed by `,` or `)` (checked without consuming it).
const Regex& mangled_param() {
    static const Regex re(R"re((\w+(?:\[\])+)\s+([^\s,()]*[;\[][^\s,()]*?)(\d+)(?=\s*[,)]))re");
    return re;
}

// Remove every `/* $VF was: ... */` comment (the text between has no `*`).
std::string strip_vf_comments(std::string_view source) {
    constexpr std::string_view kOpen = "/* $VF was: ";
    std::string out;
    std::size_t at = 0;
    for (;;) {
        std::size_t open = source.find(kOpen, at);
        if (open == std::string_view::npos) break;
        std::size_t star = source.find('*', open + kOpen.size());
        if (star == std::string_view::npos || star + 1 >= source.size() || source[star + 1] != '/') {
            out.append(source.substr(at, open + kOpen.size() - at));   // not a comment this repair knows: keep it
            at = open + kOpen.size();
            continue;
        }
        out.append(source.substr(at, open - at));
        at = star + 2;
    }
    out.append(source.substr(at));
    return out;
}

}  // namespace

std::optional<stdfs::path> find_java(const Settings& settings, const std::optional<stdfs::path>& explicit_java) {
    auto is_file = [](const stdfs::path& p) {
        std::error_code ec;
        return !p.empty() && stdfs::is_regular_file(p, ec);
    };
    const stdfs::path& wanted = (explicit_java && !explicit_java->empty()) ? *explicit_java : settings.java;
    if (is_file(wanted)) return wanted;
    if (auto home = fs::get_env("JAVA_HOME"); home && !home->empty()) {
        stdfs::path candidate = fs::from_text(*home) / "bin" / "java.exe";
        if (is_file(candidate)) return candidate;
    }
    return which("java");
}

bool available(const Settings& settings) {
    std::error_code ec;
    return find_java(settings).has_value() && !settings.retromcp_jar.empty() &&
           stdfs::is_regular_file(settings.retromcp_jar, ec);
}

std::optional<stdfs::path> find_src(const stdfs::path& workspace) {
    std::vector<stdfs::path> candidates;
    collect_src_dirs(workspace, candidates);
    std::optional<stdfs::path> best;
    std::size_t best_count = 0;
    for (const stdfs::path& dir : candidates) {
        const std::size_t n = count_java_files(dir);
        if (n > best_count) {
            best = dir;
            best_count = n;
        }
    }
    return best;
}

void LineSplitter::feed(ByteView chunk, const std::function<void(std::string_view)>& emit) {
    pending_.append(as_text(chunk));
    std::size_t start = 0, i = 0;
    while (i < pending_.size()) {
        const char c = pending_[i];
        if (c == '\n') {
            emit(std::string_view(pending_).substr(start, i - start));
            start = ++i;
        } else if (c == '\r') {
            if (i + 1 >= pending_.size()) break;   // a "\n" may follow in the next chunk
            emit(std::string_view(pending_).substr(start, i - start));
            i += (pending_[i + 1] == '\n') ? 2 : 1;
            start = i;
        } else {
            ++i;
        }
    }
    pending_.erase(0, start);
}

void LineSplitter::finish(const std::function<void(std::string_view)>& emit) {
    if (pending_.empty()) return;
    if (pending_.back() == '\r') pending_.pop_back();   // a lone "\r" at the very end is a line end
    emit(pending_);
    pending_.clear();
}

RunResult run_java(const stdfs::path& java, const stdfs::path& jar, const std::vector<std::string>& args,
                   const stdfs::path& cwd, const OutputFn& on_output, const std::optional<std::string>& stdin_text) {
    process::ProcessSpec spec;
    spec.argv = {fs::to_text(java), "-jar", fs::to_text(jar)};
    spec.argv.insert(spec.argv.end(), args.begin(), args.end());
    spec.cwd = cwd;
    spec.out = process::Capture::Collect;
    spec.err = process::Capture::Merge;

    if (on_output) {
        std::string shown = "$ java -jar " + fs::to_text(jar.filename());
        for (const std::string& a : args) shown += " " + a;
        on_output(shown);
    }
    const stdfs::path log = cwd / "codebreaker-decompile.log";
    append_log(log, "\n$ " + text::join(spec.argv, " ") + "\n");

    process::Process child = process::start(spec);
    // RetroMCP's setup task prompts (Scanner.nextLine) when it can't resolve the version from the argument.
    // Pre-feed the answer so it doesn't hang or crash on missing input; closing the pipe makes an unknown version
    // fail fast (end of input) instead of looping forever. Text goes in as the console's line ends and code page.
    if (stdin_text) child.write(text::to_cp1252(text::replace_all(*stdin_text, "\n", "\r\n")));
    child.close_stdin();

    RunResult result;
    std::string log_batch;
    auto line_out = [&](std::string_view raw) {
        std::string line = text::from_cp1252(as_bytes(raw));
        log_batch += line;
        log_batch += '\n';
        if (on_output) on_output(line);
        result.lines.push_back(std::move(line));
    };
    LineSplitter splitter;
    Bytes buf(4096);
    for (;;) {
        const std::size_t got = child.read(process::Process::Stream::Out, buf);
        if (got == 0) break;
        splitter.feed(ByteView(buf).first(got), line_out);
        append_log(log, log_batch);
        log_batch.clear();
    }
    splitter.finish(line_out);
    result.exit_code = child.wait().value_or(0);
    append_log(log, log_batch + std::format("[exit {}]\n", result.exit_code));
    return result;
}

stdfs::path decompile(std::string_view version, const stdfs::path& workspace_in, const DecompileOptions& options) {
    std::optional<stdfs::path> java = find_java(options.settings, options.java);
    if (!java) {
        throw DecompileError(
            "Java runtime not found. Install a JRE/JDK (8+) and put `java` on PATH or set JAVA_HOME. "
            "Only the decompile backend needs Java; the C++ transpiler does not.");
    }
    const stdfs::path& jar = options.settings.retromcp_jar;
    std::error_code ec;
    if (jar.empty() || !stdfs::is_regular_file(jar, ec)) {
        throw DecompileError("Bundled RetroMCP backend jar not found at: " + fs::to_text(jar));
    }

    const stdfs::path workspace = stdfs::absolute(workspace_in).lexically_normal();
    fs::make_dirs(workspace);
    auto say = [&](const std::string& line) {
        if (options.on_output) options.on_output(line);
    };

    if (options.setup) {
        say(std::format("[decompile] setting up Minecraft {} (downloads version + mappings)...", version));
        // Feed the version through stdin too: TaskSetup prompts for it if the argument doesn't resolve.
        RunResult r = run_java(*java, jar, {"setup", "setup=" + std::string(version)}, workspace, options.on_output,
                               std::string(version) + "\n");
        if (r.exit_code != 0) {
            const std::string lowered = text::lower(text::join(r.lines, "\n"));
            if (lowered.find("nosuchelementexception") != std::string::npos ||
                lowered.find("selectversion") != std::string::npos ||
                lowered.find("select version") != std::string::npos) {
                throw DecompileError(std::format(
                    "Minecraft version '{}' was not found in the configured version manifest. "
                    "Check the version id (e.g. a1.2.6, b1.6.6, c0.30-c-1900).", version));
            }
            throw DecompileError(std::format("RetroMCP setup failed for version '{}' (exit code {}).", version, r.exit_code));
        }
    }

    say("[decompile] decompiling with Fernflower...");
    RunResult r = run_java(*java, jar, {"decompile"}, workspace, options.on_output);
    if (r.exit_code != 0) throw DecompileError(std::format("RetroMCP decompile failed (exit code {}).", r.exit_code));

    std::optional<stdfs::path> src = find_src(workspace);
    if (!src) throw DecompileError("Decompile produced no Java source under " + fs::to_text(workspace) + ".");
    const std::size_t fixed = repair_array_params(*src);
    say(std::format("[decompile] done: {} .java files at {}", count_java_files(*src), fs::to_text(*src)));
    if (fixed) say(std::format("[decompile] repaired {} mangled array parameter name(s)", fixed));
    return *src;
}

std::string repair_array_params_text(std::string_view source, std::size_t& renamed) {
    renamed = 0;
    std::string out = mangled_param().replace_all(source, [&](const RegexMatch& m) {
        ++renamed;
        return std::format("{} var{}", m.group(1), m.group(3));
    });
    return renamed ? strip_vf_comments(out) : out;
}

std::size_t repair_array_params(const stdfs::path& src) {
    std::size_t count = 0;
    std::error_code ec;
    for (stdfs::recursive_directory_iterator it(src, stdfs::directory_options::skip_permission_denied, ec), end;
         !ec && it != end; it.increment(ec)) {
        std::error_code entry_ec;
        if (!is_java_file(*it, entry_ec)) continue;
        const std::string source = fs::read_text(it->path());
        std::size_t n = 0;
        const std::string repaired = repair_array_params_text(source, n);
        if (n) {
            fs::write_text(it->path(), repaired);
            count += n;
        }
    }
    return count;
}

}  // namespace cb::decompile::jar
