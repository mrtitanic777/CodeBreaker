// cb/port/verify.cpp -- see verify.hpp.

#include "cb/port/verify.hpp"

#include <algorithm>
#include <format>
#include <set>

#include <nlohmann/json.hpp>

#include "cb/core/crypto.hpp"
#include "cb/core/dynlib.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/log.hpp"
#include "cb/core/parallel.hpp"
#include "cb/core/process.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/core/win.hpp"
#include "dosexec/cbdos.h"

namespace cb::port::verify {

namespace stdfs = std::filesystem;

// ---- CASES.json ---------------------------------------------------------------------------------------------------

namespace {

[[noreturn]] void bad_config(std::string_view file, const std::string& what) { throw FormatError(file, 0, what); }

const Json& member(const Json& object, const char* key) {
    static const Json none;
    const auto it = object.find(key);
    return it == object.end() ? none : *it;
}

std::optional<std::string> optional_string(const Json& object, const char* key, std::string_view file) {
    const Json& v = member(object, key);
    if (v.is_null()) return std::nullopt;
    if (!v.is_string()) bad_config(file, std::format("\"{}\" is not text", key));
    return v.get<std::string>();
}

std::string string_or(const Json& object, const char* key, std::string_view file) {
    return optional_string(object, key, file).value_or("");
}

std::vector<std::string> string_list(const Json& object, const char* key, std::string_view file) {
    std::vector<std::string> out;
    const Json& v = member(object, key);
    if (v.is_null()) return out;
    if (!v.is_array()) bad_config(file, std::format("\"{}\" is not a list", key));
    for (const Json& item : v) {
        if (!item.is_string()) bad_config(file, std::format("\"{}\" holds something that is not text", key));
        out.push_back(item.get<std::string>());
    }
    return out;
}

std::map<std::string, std::string> string_map(const Json& object, const char* key, std::string_view file) {
    std::map<std::string, std::string> out;
    const Json& v = member(object, key);
    if (v.is_null()) return out;
    if (!v.is_object()) bad_config(file, std::format("\"{}\" is not a table", key));
    for (auto it = v.begin(); it != v.end(); ++it) {
        if (!it.value().is_string()) bad_config(file, std::format("\"{}\": \"{}\" is not text", key, it.key()));
        out[it.key()] = it.value().get<std::string>();
    }
    return out;
}

Case case_of(const Json& j, std::string_view file) {
    if (!j.is_object()) bad_config(file, "a case is not a table");
    Case c;
    c.name = string_or(j, "name", file);
    c.cmd = string_or(j, "cmd", file);
    c.stdin_text = string_or(j, "stdin", file);
    c.files = string_list(j, "files", file);
    c.make = string_map(j, "make", file);
    c.ignore = string_list(j, "ignore", file);
    c.program = optional_string(j, "program", file);
    const Json& then = member(j, "then");
    if (!then.is_null()) {
        if (!then.is_array()) bad_config(file, "\"then\" is not a list");
        for (const Json& next : then) c.then.push_back(case_of(next, file));
    }
    return c;
}

void check_case(const Config& config, const Case& c, std::string_view file) {
    for (const std::string& n : c.files)
        if (!config.inputs.contains(n)) bad_config(file, std::format("case \"{}\" is given the input \"{}\", which \"inputs\" does not have", c.name, n));
    if (c.program && !config.programs.contains(*c.program))
        bad_config(file, std::format("case \"{}\" names the program \"{}\", which \"programs\" does not have", c.name, *c.program));
    for (const Case& next : c.then) check_case(config, next, file);
}

}  // namespace

Config parse_config(std::string_view json_text, std::string_view file_name) {
    const Json j = json::parse(json_text);
    if (!j.is_object()) bad_config(file_name, "not a table");
    Config config;
    config.original = optional_string(j, "original", file_name);
    config.port = optional_string(j, "port", file_name);
    config.inputs = string_map(j, "inputs", file_name);
    config.env = string_map(j, "env", file_name);
    config.volatile_patterns = string_list(j, "volatile", file_name);
    const Json& programs = member(j, "programs");
    if (!programs.is_null()) {
        if (!programs.is_object()) bad_config(file_name, "\"programs\" is not a table");
        for (auto it = programs.begin(); it != programs.end(); ++it) {
            Program p;
            const auto original = optional_string(it.value(), "original", file_name);
            const auto port = optional_string(it.value(), "port", file_name);
            if (!original || !port) bad_config(file_name, std::format("program \"{}\" needs an original and a port", it.key()));
            p.original = *original;
            p.port = *port;
            config.programs[it.key()] = p;
        }
    }
    const Json& cases = member(j, "cases");
    if (!cases.is_array()) bad_config(file_name, "has no list of \"cases\"");
    for (const Json& c : cases) config.cases.push_back(case_of(c, file_name));
    for (const Case& c : config.cases) check_case(config, c, file_name);
    return config;
}

Config load_config(const stdfs::path& path) { return parse_config(fs::read_text(path), fs::to_text(path)); }

// ---- the ends of runs -----------------------------------------------------------------------------------------------

std::string Exit::text() const {
    switch (kind) {
        case Kind::Unsaid: return "(none)";
        case Kind::Code: return std::to_string(code);
        case Kind::DidNotEnd: return "did not end";
    }
    return "?";
}

// ---- the emulator of native/dosexec -------------------------------------------------------------------------------

struct CbdosRunner::Impl {
    Settings settings;
    DynLib lib;
    cbd_result* (*run)(const cbd_args*) = nullptr;
    void (*free_result)(cbd_result*) = nullptr;
    std::string id;

    Impl(Settings s, DynLib l) : settings(std::move(s)), lib(std::move(l)) {}
};

CbdosRunner::CbdosRunner(std::unique_ptr<Impl> impl) : impl_(std::move(impl)) {}
CbdosRunner::~CbdosRunner() = default;

std::unique_ptr<CbdosRunner> CbdosRunner::load(const Settings& settings) {
    for (const stdfs::path& p : {settings.dll, settings.unicorn})
        if (!stdfs::exists(p)) throw NotFoundError(p, 2, "the DOS emulator needs this file");
    auto impl = std::make_unique<Impl>(settings, DynLib::load(stdfs::absolute(settings.dll)));
    using Init = int (*)(const wchar_t*, char*, int);
    const auto init = impl->lib.function<Init>("cbd_init");
    impl->run = impl->lib.function<cbd_result* (*)(const cbd_args*)>("cbd_run");
    impl->free_result = impl->lib.function<void (*)(cbd_result*)>("cbd_free");
    if (!init || !impl->run || !impl->free_result) throw ToolError(fs::to_text(settings.dll), 1, "", "is not the DOS emulator");
    char err[256] = {};
    if (init(stdfs::absolute(settings.unicorn).wstring().c_str(), err, sizeof err) != 0)
        throw ToolError(fs::to_text(settings.dll), 1, err, "could not start");
    // what the emulator is: its bytes and the limits it runs under (a run kept under another is not this one)
    const Bytes dll = fs::read_file(settings.dll);
    impl->id = std::format("cbdos:{}:{}:{}", crypto::to_hex(crypto::sha256(dll)), settings.max_blocks, settings.time_limit.count());
    return std::unique_ptr<CbdosRunner>(new CbdosRunner(std::move(impl)));
}

std::string CbdosRunner::identity() const { return impl_->id; }

Run CbdosRunner::run(const stdfs::path& exe, const stdfs::path& folder, std::string_view cmd, ByteView stdin_data) const {
    const std::wstring wexe = stdfs::absolute(exe).wstring();
    const std::wstring wfolder = stdfs::absolute(folder).wstring();
    const Bytes tail = text::to_latin1(cmd.empty() ? std::string() : " " + std::string(cmd));   // (DOS gives the tail a leading blank)
    static constexpr std::uint8_t kEnvironment[] = {'P', 'A', 'T', 'H', '=', 0};                // the program's one variable
    cbd_args a{};
    a.exe = wexe.c_str();
    a.workdir = wfolder.c_str();
    a.cmdline = tail.data();
    a.ncmdline = static_cast<std::int64_t>(tail.size());
    a.stdin_data = stdin_data.data();
    a.nstdin = static_cast<std::int64_t>(stdin_data.size());
    a.env = kEnvironment;
    a.nenv = sizeof kEnvironment;
    a.max_blocks = static_cast<std::int64_t>(impl_->settings.max_blocks);
    a.fast = 1;   // (no hook on every block: the run is stopped by the time limit if it does not end)
    a.drive = 2;
    a.fast_limit = static_cast<double>(impl_->settings.time_limit.count());
    a.dpmi_limit = 300;
    cbd_result* r = impl_->run(&a);
    if (r == nullptr) throw EmulationError("the DOS emulator gave no result");
    struct Release {
        const CbdosRunner::Impl& impl;
        cbd_result* r;
        ~Release() { impl.free_result(r); }
    } release{*impl_, r};
    Run out;
    if (r->status == CBD_OK) {
        out.console.assign(r->console, r->console + r->nconsole);
        out.exit = r->has_exit ? Exit::of(static_cast<std::uint32_t>(r->exit)) : Exit::unsaid();
        return out;
    }
    const std::string message = r->message != nullptr ? r->message : "";
    if (r->status == CBD_RUNTIME || r->status == CBD_OTHER) {   // (a run that does not end: what it showed, and that)
        out.console = text::to_latin1(message);
        out.exit = Exit::did_not_end();
        return out;
    }
    throw EmulationError(message.empty() ? std::format("the DOS emulator failed (status {})", r->status) : message);
}

// ---- the port ------------------------------------------------------------------------------------------------------

Run run_port(const stdfs::path& exe, const stdfs::path& folder, std::string_view cmd, ByteView stdin_data,
             const std::map<std::string, std::string>& env, std::chrono::milliseconds timeout) {
    process::ProcessSpec spec;
    spec.argv.push_back(win::from_wide(stdfs::path(exe).make_preferred().native()));   // (a program name as the system spells it: cmd.exe takes the slashes for switches)
    for (const std::string_view arg : text::split_ws(cmd)) spec.argv.emplace_back(arg);
    spec.cwd = folder;
    spec.env = env;
    spec.stdin_data.assign(stdin_data.begin(), stdin_data.end());
    spec.err = process::Capture::Merge;
    spec.timeout = timeout;
    process::ProcessResult r = process::run(spec);
    Run out;
    out.console = std::move(r.out);
    out.exit = r.timed_out ? Exit::did_not_end() : Exit::of(r.exit_code);
    return out;
}

// ---- the original's run, kept -------------------------------------------------------------------------------------

namespace {

// What is kept of an original's run: what it showed, how it ended, every file it left.
struct Recorded {
    Bytes console;
    Exit exit;
    std::map<std::string, Bytes> files;
};

// The regular files of a folder by name (as the system spells them), in the order of the names.
std::map<std::string, Bytes> read_folder(const stdfs::path& folder) {
    std::map<std::string, Bytes> out;
    std::error_code ec;
    for (stdfs::directory_iterator it(folder, ec), end; !ec && it != end; it.increment(ec)) {
        std::error_code ec2;
        if (it->is_regular_file(ec2)) out[fs::to_text(it->path().filename())] = fs::read_file(it->path());
    }
    return out;
}

void clear_files(const stdfs::path& folder) {
    std::error_code ec;
    std::vector<stdfs::path> doomed;
    for (stdfs::directory_iterator it(folder, ec), end; !ec && it != end; it.increment(ec)) {
        std::error_code ec2;
        if (it->is_regular_file(ec2)) doomed.push_back(it->path());
    }
    for (const stdfs::path& p : doomed) stdfs::remove(p);
}

}  // namespace

}  // namespace cb::port::verify

template <>
struct cb::base::cache::Codec<cb::port::verify::Recorded> {
    static constexpr std::uint32_t kSchema = 0x7100;
    static void write(io::ByteWriter& w, const cb::port::verify::Recorded& r) {
        Codec<Bytes>::write(w, r.console);
        w.u8(static_cast<std::uint8_t>(r.exit.kind));
        w.u32(r.exit.code);
        Codec<std::map<std::string, Bytes>>::write(w, r.files);
    }
    static cb::port::verify::Recorded read(io::ByteReader& r) {
        cb::port::verify::Recorded out;
        out.console = Codec<Bytes>::read(r);
        const std::uint8_t kind = r.u8();
        if (kind > static_cast<std::uint8_t>(cb::port::verify::Exit::Kind::DidNotEnd)) throw FormatError("port-originals", r.pos(), "bad exit kind");
        out.exit.kind = static_cast<cb::port::verify::Exit::Kind>(kind);
        out.exit.code = r.u32();
        out.files = Codec<std::map<std::string, Bytes>>::read(r);
        return out;
    }
};

namespace cb::port::verify {

void declare_cache(base::cache::Cache& cache) {
    cache.declare_kind(kCacheKind, {.scope = base::cache::Scope::State, .generation = std::string(kCacheVersion)});
}

Run run_original(const OriginalRunner& runner, base::cache::Cache* cache, const base::budget::Limits* limits,
                 const stdfs::path& exe, const stdfs::path& folder, std::string_view cmd, ByteView stdin_data) {
    std::string key;
    if (cache != nullptr) {
        // the program, the emulator, the command, the input and every file it is given
        std::vector<base::cache::KeyPart> parts{cache->tool_bytes_id(exe), runner.identity(), std::string(cmd), stdin_data};
        for (const auto& [name, data] : read_folder(folder)) {
            parts.emplace_back(name);
            parts.emplace_back(ByteView(data));
        }
        key = base::cache::key(parts);
        if (const std::optional<Recorded> kept = cache->value<Recorded>(kCacheKind, key)) {
            try {
                clear_files(folder);   // (the folder as the run left it)
                for (const auto& [name, data] : kept->files) fs::write_file(folder / fs::from_text(name), data);
                return {kept->console, kept->exit};
            } catch (const IoError&) {   // (the folder cannot be made so: run it)
            }
        }
    }
    if (limits != nullptr) base::budget::wait_for_memory(*limits, "verify");
    const Run run = runner.run(exe, folder, cmd, stdin_data);
    if (cache != nullptr) {
        try {
            cache->keep(kCacheKind, key, Recorded{run.console, run.exit, read_folder(folder)});
        } catch (const IoError&) {   // (what was left cannot be read: it is not kept)
        }
    }
    return run;
}

// ---- small helpers ------------------------------------------------------------------------------------------------

std::map<std::string, Bytes> files_of(const stdfs::path& folder) {
    std::map<std::string, Bytes> out;
    for (auto& [name, data] : read_folder(folder)) out[text::upper(name)] = std::move(data);
    return out;
}

std::vector<std::string> console_lines(ByteView console) {
    const std::string text = text::from_latin1(console);
    std::vector<std::string> out;
    std::size_t begin = 0;
    for (std::size_t i = 0; i < text.size(); ++i) {
        if (text[i] != '\n' && text[i] != '\r') continue;
        out.push_back(text.substr(begin, i - begin));
        if (text[i] == '\r' && i + 1 < text.size() && text[i + 1] == '\n') ++i;
        begin = i + 1;
    }
    if (begin < text.size()) out.push_back(text.substr(begin));
    return out;
}

namespace {

// ---- the shortest list of changes between two lists of lines (Myers) ---------------------------------------------

enum class Step : std::uint8_t { Keep, Remove, Add };

// The steps that turn a into b. Lines are compared whole. `limit`: the most changes looked for before the middle is
// given up as one replacement.
std::vector<Step> edit_script(const std::vector<std::string>& a, const std::vector<std::string>& b, std::size_t limit = 4000) {
    std::size_t head = 0;
    while (head < a.size() && head < b.size() && a[head] == b[head]) ++head;
    std::size_t tail = 0;
    while (tail < a.size() - head && tail < b.size() - head && a[a.size() - 1 - tail] == b[b.size() - 1 - tail]) ++tail;
    const std::size_t n = a.size() - head - tail;
    const std::size_t m = b.size() - head - tail;
    std::vector<Step> middle;
    const auto replace_all_of_middle = [&] {
        middle.assign(n, Step::Remove);
        middle.insert(middle.end(), m, Step::Add);
    };
    if (n == 0 || m == 0) {
        replace_all_of_middle();
    } else {
        const std::ptrdiff_t max = static_cast<std::ptrdiff_t>(std::min(n + m, limit));
        const std::ptrdiff_t offset = max;
        std::vector<std::ptrdiff_t> v(static_cast<std::size_t>(2 * max + 2), 0);
        std::vector<std::vector<std::ptrdiff_t>> trace;
        bool found = false;
        std::ptrdiff_t reached = 0;
        for (std::ptrdiff_t d = 0; d <= max && !found; ++d) {
            trace.push_back(v);
            for (std::ptrdiff_t k = -d; k <= d; k += 2) {
                std::ptrdiff_t x;
                const auto at = [&](std::ptrdiff_t diag) -> std::ptrdiff_t& { return v[static_cast<std::size_t>(diag + offset)]; };
                if (k == -d || (k != d && at(k - 1) < at(k + 1))) x = at(k + 1);
                else x = at(k - 1) + 1;
                std::ptrdiff_t y = x - k;
                while (x < static_cast<std::ptrdiff_t>(n) && y < static_cast<std::ptrdiff_t>(m) &&
                       a[head + static_cast<std::size_t>(x)] == b[head + static_cast<std::size_t>(y)]) {
                    ++x;
                    ++y;
                }
                at(k) = x;
                if (x >= static_cast<std::ptrdiff_t>(n) && y >= static_cast<std::ptrdiff_t>(m)) {
                    found = true;
                    reached = d;
                    break;
                }
            }
        }
        if (!found) {
            replace_all_of_middle();   // (too different to align)
        } else {
            std::ptrdiff_t x = static_cast<std::ptrdiff_t>(n), y = static_cast<std::ptrdiff_t>(m);
            std::vector<Step> back;
            for (std::ptrdiff_t d = reached; d > 0; --d) {
                const std::vector<std::ptrdiff_t>& prev = trace[static_cast<std::size_t>(d)];
                const std::ptrdiff_t k = x - y;
                const auto at = [&](std::ptrdiff_t diag) { return prev[static_cast<std::size_t>(diag + offset)]; };
                const std::ptrdiff_t prev_k = (k == -d || (k != d && at(k - 1) < at(k + 1))) ? k + 1 : k - 1;
                const std::ptrdiff_t prev_x = at(prev_k);
                const std::ptrdiff_t prev_y = prev_x - prev_k;
                while (x > prev_x && y > prev_y) {
                    back.push_back(Step::Keep);
                    --x;
                    --y;
                }
                back.push_back(x == prev_x ? Step::Add : Step::Remove);
                x = prev_x;
                y = prev_y;
            }
            while (x > 0 && y > 0) {
                back.push_back(Step::Keep);
                --x;
                --y;
            }
            middle.assign(back.rbegin(), back.rend());
        }
    }
    std::vector<Step> out(head, Step::Keep);
    out.insert(out.end(), middle.begin(), middle.end());
    out.insert(out.end(), tail, Step::Keep);
    return out;
}

// "3,5" for lines 3 to 7 (counted from 0 at `start`); the form of a hunk header: a single line is "3", none is "2,0".
std::string hunk_range(std::size_t start, std::size_t length) {
    std::size_t first = start + 1;
    if (length == 1) return std::to_string(first);
    if (length == 0) --first;
    return std::format("{},{}", first, length);
}

}  // namespace

std::vector<std::string> unified_diff(const std::vector<std::string>& a, const std::vector<std::string>& b, std::string_view from_name,
                                      std::string_view to_name, std::size_t context) {
    const std::vector<Step> steps = edit_script(a, b);
    std::vector<std::string> out;
    if (std::ranges::all_of(steps, [](Step s) { return s == Step::Keep; })) return out;
    out.push_back("--- " + std::string(from_name));
    out.push_back("+++ " + std::string(to_name));
    // positions in a and b at each step
    std::vector<std::size_t> ai(steps.size() + 1, 0), bi(steps.size() + 1, 0);
    for (std::size_t i = 0; i < steps.size(); ++i) {
        ai[i + 1] = ai[i] + (steps[i] != Step::Add ? 1 : 0);
        bi[i + 1] = bi[i] + (steps[i] != Step::Remove ? 1 : 0);
    }
    std::size_t i = 0;
    while (i < steps.size()) {
        while (i < steps.size() && steps[i] == Step::Keep) ++i;
        if (i >= steps.size()) break;
        // a hunk: from `context` lines before this change to `context` lines after the last one that is within
        // twice that of the next
        const std::size_t first = i >= context ? i - context : 0;
        std::size_t last = i;   // one past the last changed step so far
        std::size_t j = i;
        while (j < steps.size()) {
            if (steps[j] != Step::Keep) {
                last = j + 1;
                ++j;
                continue;
            }
            std::size_t run = 0;
            while (j + run < steps.size() && steps[j + run] == Step::Keep) ++run;
            if (j + run >= steps.size() || run > 2 * context) break;
            j += run;
        }
        const std::size_t end = std::min(last + context, steps.size());
        out.push_back(std::format("@@ -{} +{} @@", hunk_range(ai[first], ai[end] - ai[first]), hunk_range(bi[first], bi[end] - bi[first])));
        for (std::size_t s = first; s < end; ++s) {
            switch (steps[s]) {
                case Step::Keep: out.push_back(" " + a[ai[s]]); break;
                case Step::Remove: out.push_back("-" + a[ai[s]]); break;
                case Step::Add: out.push_back("+" + b[bi[s]]); break;
            }
        }
        i = end;
    }
    return out;
}

// ---- file-name patterns ---------------------------------------------------------------------------------------------

namespace {

char normal_char(char c) {
    if (c == '/') return '\\';
    return (c >= 'A' && c <= 'Z') ? static_cast<char>(c - 'A' + 'a') : c;
}

// Whether `c` is in the set that opens at pat[at] == '[': -> (matches, index after the closing ']'), or nothing when the
// bracket never closes (then it is a plain '[').
std::optional<std::pair<bool, std::size_t>> in_set(std::string_view pat, std::size_t at, char c) {
    std::size_t j = at + 1;
    const bool negate = j < pat.size() && pat[j] == '!';
    if (negate) ++j;
    const std::size_t first = j;
    if (j < pat.size() && pat[j] == ']') ++j;   // (a ']' first is a member)
    while (j < pat.size() && pat[j] != ']') ++j;
    if (j >= pat.size()) return std::nullopt;
    bool hit = false;
    for (std::size_t k = first; k < j; ++k) {
        if (k + 2 < j && pat[k + 1] == '-') {
            if (pat[k] <= c && c <= pat[k + 2]) hit = true;
            k += 2;
        } else if (pat[k] == c) {
            hit = true;
        }
    }
    return std::pair{hit != negate, j + 1};
}

}  // namespace

bool glob_match(std::string_view name_in, std::string_view pattern_in) {
    std::string name(name_in), pat(pattern_in);
    for (char& c : name) c = normal_char(c);
    for (char& c : pat) c = normal_char(c);
    std::size_t n = 0, p = 0;
    std::size_t star_p = std::string::npos, star_n = 0;   // where the last '*' was, and how much of the name it has taken
    while (true) {
        bool advanced = false;
        if (p < pat.size()) {
            const char c = pat[p];
            if (c == '*') {
                star_p = p++;
                star_n = n;
                continue;
            }
            if (n < name.size()) {
                if (c == '?') {
                    ++p, ++n;
                    advanced = true;
                } else if (c == '[') {
                    if (const auto set = in_set(pat, p, name[n])) {
                        if (set->first) {
                            p = set->second;
                            ++n;
                            advanced = true;
                        }
                    } else if (name[n] == '[') {
                        ++p, ++n;
                        advanced = true;
                    }
                } else if (c == name[n]) {
                    ++p, ++n;
                    advanced = true;
                }
            }
        } else if (n == name.size()) {
            return true;
        }
        if (advanced) continue;
        if (star_p == std::string::npos || star_n >= name.size()) return false;
        p = star_p + 1;   // (the '*' takes one more)
        n = ++star_n;
    }
}

// ---- the cases ------------------------------------------------------------------------------------------------------

namespace {

stdfs::path at(const stdfs::path& base, const std::string& p) {
    const stdfs::path path = fs::from_text(p);
    return path.is_absolute() ? path : base / path;
}

struct Programs {
    stdfs::path original;
    stdfs::path port;
};

// The original and the port a case runs: another program of the set, or the cases file's own.
Programs programs_of(const Config& config, const stdfs::path& base, const Case& c, const stdfs::path& original, const stdfs::path& port) {
    if (!c.program) return {original, port};
    const Program& p = config.programs.at(*c.program);   // (named in the file: checked when it was read)
    return {at(base, p.original), at(base, p.port)};
}

void put_inputs(const Config& config, const stdfs::path& base, const Case& c, std::span<const stdfs::path> folders) {
    for (const std::string& n : c.files) {
        const stdfs::path from = at(base, config.inputs.at(n));
        for (const stdfs::path& folder : folders) {
            std::error_code ec;
            stdfs::copy_file(from, folder / fs::from_text(n), stdfs::copy_options::overwrite_existing, ec);
            if (ec) throw IoError(from, ec.value(), "cannot be copied to " + fs::to_text(folder));
        }
    }
    for (const auto& [name, contents] : c.make)   // (small inputs written out: a response file)
        for (const stdfs::path& folder : folders) fs::write_file(folder / fs::from_text(name), text::to_latin1(contents));
}

std::vector<Regex> compile_volatile(const Config& config) {
    std::vector<Regex> out;
    for (const std::string& pattern : config.volatile_patterns) out.emplace_back(pattern);   // (a pattern that does not compile: ParseError)
    return out;
}

Bytes mask(ByteView data, const std::vector<Regex>& patterns) {
    std::string text(as_text(data));
    for (const Regex& re : patterns) text = re.replace_all(text, "<varies>");
    return to_bytes(text);
}

bool masked_name(const std::string& upper_name) {
    return upper_name.ends_with(".LST") || upper_name.ends_with(".MAP") || upper_name.ends_with(".TXT");
}

struct Pair {
    std::vector<CaseRow> rows;
    std::vector<std::string> details;
};

class Comparison {
public:
    Comparison(const Config& config, const stdfs::path& base, const stdfs::path& original, const stdfs::path& port, const Services& services)
        : config_(config), base_(base), original_(original), port_(port), services_(services), patterns_(compile_volatile(config)) {}

    // A case and those run after it, in one pair of folders.
    Pair run_top(const Case& c) const {
        fs::ScopedTempDir original_dir("cbv_o"), port_dir("cbv_p");
        Pair out;
        run_case(c, original_dir.path(), port_dir.path(), out);
        return out;
    }

private:
    void run_case(const Case& c, const stdfs::path& fa, const stdfs::path& fb, Pair& out) const {
        const stdfs::path both[] = {fa, fb};
        put_inputs(config_, base_, c, both);
        const Programs prog = programs_of(config_, base_, c, original_, port_);
        const Bytes typed = text::to_latin1(c.stdin_text);
        Run original = run_original(*services_.original, services_.cache, services_.limits, prog.original, fa, c.cmd, typed);
        Run port = run_port(prog.port, fb, c.cmd, typed, config_.env, services_.port_timeout);
        if (port.exit.kind == Exit::Kind::DidNotEnd) port.console = to_bytes("(did not end)");   // (a port that does not end: so recorded)
        std::map<std::string, Bytes> a = files_of(fa), b = files_of(fb);
        if (!patterns_.empty()) {   // (what depends on the machine - free memory - masked alike in both)
            original.console = mask(original.console, patterns_);
            port.console = mask(port.console, patterns_);
            for (auto* side : {&a, &b})
                for (auto& [name, data] : *side)
                    if (masked_name(name)) data = mask(data, patterns_);
        }
        std::vector<std::string> bad;
        if (original.console != port.console) {
            bad.emplace_back("console");
            const std::vector<std::string> diff = unified_diff(console_lines(original.console), console_lines(port.console), "original", "port", 2);
            out.details.push_back(std::format("### {}: console\n\n```diff\n{}\n```\n", c.name, text::join(diff, "\n")));
        }
        if (!(original.exit == port.exit)) bad.push_back(std::format("exit {} / {}", original.exit.text(), port.exit.text()));
        std::set<std::string> names;
        for (const auto& kv : a) names.insert(kv.first);
        for (const auto& kv : b) names.insert(kv.first);
        for (const std::string& n : names) {
            if (std::ranges::any_of(c.ignore, [&](const std::string& g) { return glob_match(n, g); })) continue;   // (not compared: the case says so)
            const auto ia = a.find(n), ib = b.find(n);
            if (ia != a.end() && ib != b.end() && ia->second == ib->second) continue;
            std::string what;
            if (ib == b.end()) {
                what = "only the original's";
            } else if (ia == a.end()) {
                what = "only the port's";
            } else {
                const auto [x, y] = std::mismatch(ia->second.begin(), ia->second.end(), ib->second.begin(), ib->second.end());
                what = std::format("differs ({} / {} bytes, first at {})", ia->second.size(), ib->second.size(), x - ia->second.begin());
                (void)y;
            }
            bad.push_back(n + " " + what);
        }
        std::set<std::string> given;
        for (const std::string& n : c.files) given.insert(text::upper(n));
        for (const auto& kv : c.make) given.insert(text::upper(kv.first));
        std::string what = text::join(bad, ", ");
        if (what.empty()) {
            what = "same: console";
            for (const auto& kv : a)
                if (!given.contains(kv.first)) what += ", " + kv.first;
        }
        out.rows.push_back({c.name, c.cmd, ::cb::verify::pair_verdict(bad.empty()), what});
        for (const Case& next : c.then) run_case(next, fa, fb, out);
    }

    const Config& config_;
    const stdfs::path& base_;
    const stdfs::path& original_;
    const stdfs::path& port_;
    const Services& services_;
    std::vector<Regex> patterns_;
};

}  // namespace

std::size_t Report::same() const {
    return static_cast<std::size_t>(std::ranges::count_if(rows, [](const CaseRow& r) { return r.verdict == ::cb::verify::Verdict::Equivalent; }));
}

std::string Report::markdown() const {
    std::vector<std::string> lines;
    lines.emplace_back("# Behaviour: the original against the port");
    lines.emplace_back();
    lines.push_back(std::format("`{}` (under CodeBreaker's emulator) against `{}` (native): **{} of {} cases the same**.", original_name,
                                port_name, same(), rows.size()));
    lines.emplace_back();
    lines.emplace_back("| case | command | | what |");
    lines.emplace_back("|---|---|---|---|");
    for (const CaseRow& r : rows) {
        const std::string said(::cb::verify::said(r.verdict));
        const std::string mark = r.verdict == ::cb::verify::Verdict::Equivalent ? said : "**" + text::upper(said) + "**";
        lines.push_back(std::format("| {} | `{}` | {} | {} |", r.name, r.cmd.empty() ? "(none)" : r.cmd, mark, r.what));
    }
    if (!details.empty()) {
        lines.emplace_back();
        lines.emplace_back("## Differences");
        lines.emplace_back();
        for (const std::string& d : details) lines.push_back(d);
    }
    return text::join(lines, "\n") + "\n";
}

Report compare(const Config& config, const stdfs::path& base, const stdfs::path& original_exe, const stdfs::path& port_exe,
               const Services& services) {
    if (services.original == nullptr) throw UsageError("compare needs an emulator for the original");
    const Comparison comparison(config, base, original_exe, port_exe, services);
    parallel::ThreadPool pool(static_cast<std::size_t>(parallel::workers(services.limits != nullptr ? services.limits->workers : parallel::WorkerPolicy{})));
    std::vector<Pair> pairs = parallel::parallel_map(pool, config.cases, [&](const Case& c) { return comparison.run_top(c); });
    Report report;
    report.original_name = fs::to_text(original_exe.filename());
    std::error_code ec;
    const stdfs::path relative = stdfs::relative(port_exe, base, ec);
    report.port_name = (!ec && !relative.empty()) ? fs::to_text(relative) : fs::to_text(port_exe);
    for (Pair& p : pairs) {   // (in the order of the cases)
        for (CaseRow& r : p.rows) report.rows.push_back(std::move(r));
        for (std::string& d : p.details) report.details.push_back(std::move(d));
    }
    return report;
}

std::vector<CaseConsole> run_cases(const Config& config, const stdfs::path& base, const stdfs::path& port_exe,
                                   const std::map<std::string, std::string>& more_env, const Services& services) {
    std::map<std::string, std::string> env = config.env;
    for (const auto& [k, v] : more_env) env[k] = v;
    const std::function<void(const Case&, const stdfs::path&, std::vector<CaseConsole>&)> one =
        [&](const Case& c, const stdfs::path& folder, std::vector<CaseConsole>& out) {
            const stdfs::path both[] = {folder};
            put_inputs(config, base, c, both);
            const Programs prog = programs_of(config, base, c, {}, port_exe);
            std::error_code ec;
            if (c.program && !stdfs::is_regular_file(prog.port, ec)) return;   // (another pass whose port is not built)
            Run r = run_port(prog.port, folder, c.cmd, text::to_latin1(c.stdin_text), env, services.port_timeout);
            Bytes console = std::move(r.console);
            if (r.exit.kind == Exit::Kind::DidNotEnd) {   // (what it said before it was stopped: the sanitizer's reports among it)
                constexpr std::size_t kKeep = 200000;
                if (console.size() > kKeep) console.erase(console.begin(), console.end() - static_cast<std::ptrdiff_t>(kKeep));
                const std::string note = "\n(did not end)";
                console.insert(console.end(), note.begin(), note.end());
            }
            out.push_back({c.name, text::from_latin1(console)});
            for (const Case& next : c.then) one(next, folder, out);
        };
    parallel::ThreadPool pool(static_cast<std::size_t>(parallel::workers(services.limits != nullptr ? services.limits->workers : parallel::WorkerPolicy{})));
    const std::vector<std::vector<CaseConsole>> all = parallel::parallel_map(pool, config.cases, [&](const Case& c) {
        fs::ScopedTempDir dir("cbs");
        std::vector<CaseConsole> out;
        one(c, dir.path(), out);
        return out;
    });
    std::vector<CaseConsole> out;
    for (const auto& part : all) out.insert(out.end(), part.begin(), part.end());
    return out;
}

// ---- the command ------------------------------------------------------------------------------------------------------

Outcome execute(const stdfs::path& cases_path, const std::optional<stdfs::path>& report_path, const std::optional<stdfs::path>& port_override,
                const Services& services) {
    const stdfs::path base = stdfs::absolute(cases_path).parent_path();
    const Config config = load_config(cases_path);
    if (!config.original) throw UsageError("the cases file names no \"original\"");
    const stdfs::path original = at(base, *config.original);
    stdfs::path port;
    if (port_override) {
        port = stdfs::absolute(*port_override);
    } else {
        if (!config.port) throw UsageError("the cases file names no \"port\", and --port was not given");
        port = at(base, *config.port);
    }
    Outcome outcome;
    std::error_code ec;
    if (!stdfs::is_regular_file(port, ec)) {
        outcome.exit_code = kExitNotFound;
        outcome.err = std::format("the port is not built: {}\n", fs::to_text(port));
        return outcome;
    }
    const Report report = compare(config, base, original, port, services);
    const std::string text = report.markdown();
    if (report_path) {
        fs::write_text(*report_path, text);
        outcome.out = std::format("{} of {} cases the same ({})\n", report.same(), report.rows.size(), fs::to_text(*report_path));
    } else {
        outcome.out = text;
    }
    outcome.exit_code = report.all_same() ? kExitOk : kExitFailed;
    return outcome;
}

cli::Command command(const Services& services) {
    cli::Command c;
    c.name = "verify";
    c.summary = "run an original and its port on the same inputs and compare what each shows and writes";
    c.area = "Port";
    c.description =
        "CASES is a cases file (see cb/port/verify.hpp): the original, the port, the inputs and the cases. Each case\n"
        "runs both in folders of their own; the console, the exit code and every file left behind are compared.";
    c.params = {cli::Positional{"cases", "the cases file (CASES.json)"},
                cli::Option{"--report", "", "write the report here (markdown) instead of printing it", cli::Arg::Value, "FILE"},
                cli::Option{"--port", "", "the port's program (instead of the cases file's)", cli::Arg::Value, "EXE"}};
    c.handler = [services](const cli::Args& args) {
        parallel::throttle(0, true);   // (a check runs below everything else on the machine)
        std::optional<stdfs::path> report, port;
        if (const std::optional<std::string> v = args.value("--report")) report = fs::from_text(*v);
        if (const std::optional<std::string> v = args.value("--port")) port = fs::from_text(*v);
        const Outcome o = execute(fs::from_text(args.required("cases")), report, port, services);
        out_text(o.out);
        err_text(o.err);
        return o.exit_code;
    };
    return c;
}

}  // namespace cb::port::verify
