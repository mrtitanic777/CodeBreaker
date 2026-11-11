#include "cb/build/functions.hpp"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <functional>
#include <set>
#include <condition_variable>
#include <format>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/process.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"

namespace cb::build::functions {

namespace stdfs = std::filesystem;
using std::chrono::milliseconds;

namespace {

constexpr std::size_t kMaxReason = 100;           // how much of a compiler's message is kept
constexpr std::size_t kMaxSourceLine = 200;       // and of the source line it is about
constexpr std::size_t kMaxLinkReason = 200;
constexpr std::size_t kMaxLinkLogTail = 160;
constexpr int kMaxLinkRepairs = 80;               // how often a failed 16-bit link is repaired by standing in for a function
constexpr std::string_view kMarkedFolder = std::string_view("\0here\0", 6);   // a folder in a kept message
constexpr std::string_view kHeaderInclude = "#include \"m.h\"\n\n";
constexpr milliseconds kLinkTimeout = std::chrono::minutes(15);
constexpr milliseconds kAssemblerTimeout = std::chrono::minutes(5);

// OMF record kinds the object digest and omf_here single out: the header, comments, and the module end.
constexpr std::uint8_t kOmfTheadr = 0x80, kOmfComent = 0x88, kOmfModend = 0x8A, kOmfModend32 = 0x8B;
constexpr std::uint8_t kComentDependency = 0xE9;   // COMENT class of a dependency record: file, with its date

// ---- small helpers ----------------------------------------------------------------------------------------------------

std::string text_of(const Bytes& b) { return text::sanitize_utf8(as_text(b)); }

// The first `n` bytes, cut at a character boundary.
std::string head_of(std::string_view s, std::size_t n) {
    if (s.size() <= n) return std::string(s);
    while (n > 0 && (static_cast<unsigned char>(s[n]) & 0xC0) == 0x80) --n;
    return std::string(s.substr(0, n));
}

// The last `n` bytes, cut at a character boundary.
std::string tail_of(std::string_view s, std::size_t n) {
    if (s.size() <= n) return std::string(s);
    std::size_t from = s.size() - n;
    while (from < s.size() && (static_cast<unsigned char>(s[from]) & 0xC0) == 0x80) ++from;
    return std::string(s.substr(from));
}

std::string folder_text(const stdfs::path& folder) {
    std::error_code ec;
    stdfs::path p = stdfs::absolute(folder, ec).lexically_normal();
    std::string out = fs::to_text(p);
    std::replace(out.begin(), out.end(), '/', '\\');   // (the backslashes the tools write)
    return out;
}

void remove_quietly(const stdfs::path& p) {
    std::error_code ec;
    stdfs::remove(p, ec);
}

bool contains(std::string_view s, std::string_view part) { return s.find(part) != std::string_view::npos; }

std::string stem_name(std::size_t i) { return std::format("f{:04}", i); }

std::string get_or(const Env& env, const char* name) {
    const auto it = env.find(name);
    return it == env.end() ? std::string() : it->second;
}

std::vector<std::string> plus(std::span<const std::string> flags, std::span<const std::string> more) {
    std::vector<std::string> out(flags.begin(), flags.end());
    out.insert(out.end(), more.begin(), more.end());
    return out;
}

std::vector<std::string> plus(std::span<const std::string> flags, std::string extra) {
    std::vector<std::string> out(flags.begin(), flags.end());
    out.push_back(std::move(extra));
    return out;
}

std::vector<std::string> flags_for(std::span<const std::string> flags, const ExtraFlags& more, const std::string& name) {
    const auto it = more.find(name);
    return it == more.end() ? std::vector<std::string>(flags.begin(), flags.end()) : plus(flags, std::span<const std::string>(it->second));
}

// A digest of fields that cannot run into each other: each is written with its length.
class KeyBuilder {
public:
    KeyBuilder() : hasher_(crypto::Hasher::Algorithm::Sha256) {}
    KeyBuilder& add(std::string_view field) {
        std::uint8_t length[8];
        for (int i = 0; i < 8; ++i) length[i] = static_cast<std::uint8_t>(static_cast<std::uint64_t>(field.size()) >> (8 * i));
        hasher_.update(ByteView(length, 8));
        hasher_.update(as_bytes(field));
        return *this;
    }
    KeyBuilder& add(std::span<const std::string> fields) {
        add(std::to_string(fields.size()));
        for (const std::string& f : fields) add(f);
        return *this;
    }
    std::string finish() {
        const auto digest = hasher_.finish();
        return crypto::to_hex(ByteView(digest.data(), digest.size()));
    }

private:
    crypto::Hasher hasher_;
};

// A text file, UTF-8, '\n' line ends.
void write_source(const stdfs::path& p, std::string_view text) { fs::write_text(p, text); }

process::ProcessResult run_tool(const stdfs::path& exe, std::span<const std::string> args, const stdfs::path& cwd, const Env& env,
                                milliseconds timeout) {
    process::ProcessSpec spec;
    spec.argv.push_back(fs::to_text(exe));
    spec.argv.insert(spec.argv.end(), args.begin(), args.end());
    spec.cwd = cwd;
    spec.env = env;
    spec.timeout = timeout;
    return process::run(spec);
}

std::string log_of(const process::ProcessResult& r) { return text_of(r.out) + text_of(r.err); }

// ---- the compilers' messages ------------------------------------------------------------------------------------------

// What a compiler said about a rejected function: its first error, with the source line it names.
struct Complaint {
    std::string why;
    bool with_line = false;
};

Complaint complaint_wcc(std::string_view log, std::string_view text) {
    static const Regex error_line(R"(\((\d+)\): Error! (E\d+: [^\r\n]*))");
    Complaint c;
    if (const auto m = error_line.search(log)) {
        c.with_line = true;
        c.why = head_of(m->group(2), kMaxReason);
        // (with the line it is about: the source is the header include, a blank line, then the text)
        std::vector<std::string_view> source = {"#include \"m.h\"", ""};
        for (std::string_view line : text::lines(text)) source.push_back(line);
        const auto line_no = text::parse_int<std::size_t>(m->group(1));
        if (line_no && *line_no > 0 && *line_no <= source.size())
            c.why += " -- " + head_of(text::trim(source[*line_no - 1]), kMaxSourceLine);
    } else {
        c.why = head_of(tail_of(text::trim(log), kMaxReason), kMaxReason);
        if (c.why.empty()) c.why = "the compiler failed";
    }
    return c;
}

// ---- OMF objects ------------------------------------------------------------------------------------------------------

struct OmfRecord {
    std::uint8_t kind;
    Bytes body;
    std::uint8_t check;
};

// The records of an object file; nullopt if it is not one whole (records that end where they should).
std::optional<std::vector<OmfRecord>> omf_records(ByteView obj) {
    std::vector<OmfRecord> out;
    std::size_t i = 0;
    const std::size_t n = obj.size();
    while (i + 3 <= n) {
        const std::size_t end = i + 3 + (obj[i + 1] | (static_cast<std::size_t>(obj[i + 2]) << 8));
        if (end > n || end < i + 4) return std::nullopt;
        out.push_back({obj[i], Bytes(obj.begin() + static_cast<std::ptrdiff_t>(i) + 3, obj.begin() + static_cast<std::ptrdiff_t>(end) - 1), obj[end - 1]});
        i = end;
    }
    if (i != n) return std::nullopt;
    return out;
}

// (time, date) as DOS keeps them, of a file's modification (OpenWatcom's dependency record).
std::optional<std::pair<std::uint16_t, std::uint16_t>> dos_stamp(const stdfs::path& p) {
    std::error_code ec;
    const auto written = stdfs::last_write_time(p, ec);
    if (ec) return std::nullopt;
    // (OpenWatcom's C library: with no TZ, its local time is UTC)
    const auto when = std::chrono::floor<std::chrono::seconds>(std::chrono::clock_cast<std::chrono::system_clock>(written));
    const auto day = std::chrono::floor<std::chrono::days>(when);
    const std::chrono::year_month_day ymd(day);
    const std::chrono::hh_mm_ss<std::chrono::seconds> hms(when - day);
    const int year = static_cast<int>(ymd.year());
    const unsigned month = static_cast<unsigned>(ymd.month()), mday = static_cast<unsigned>(ymd.day());
    const auto clock = static_cast<std::uint16_t>((hms.hours().count() << 11) | (hms.minutes().count() << 5) | (hms.seconds().count() / 2));
    const auto date = static_cast<std::uint16_t>(((year - 1980) << 9) | (month << 5) | mday);
    return std::pair{clock, date};
}

bool starts_with_bytes(const Bytes& b, const Bytes& prefix, std::size_t from = 0) {
    return b.size() >= from + prefix.size() && std::equal(prefix.begin(), prefix.end(), b.begin() + static_cast<std::ptrdiff_t>(from));
}

}  // namespace

// ---- constants ------------------------------------------------------------------------------------------------------

const std::vector<std::string>& cflags16() {
    static const std::vector<std::string> flags = {"-bt=windows", "-mm", "-ecd", "-zu", "-zc", "-zl", "-zq", "-w0", "-s", "-3",
                                                   "-zfp", "-zgp", "-zastd=c99", "-e=20", "-j"};
    return flags;
}

const std::vector<std::string>& cflags_powerpc() {
    static const std::vector<std::string> flags = {"/nologo", "/c", "/TC", "/O2", "/GS-", "/Zl", "/Gy", "/W0", "/FIm.h"};
    return flags;
}

// ---- texts ----------------------------------------------------------------------------------------------------------

std::string prototype(std::string_view text) {
    std::string_view head = text.substr(0, text.find('{'));
    if (const auto close = head.find(')'); close != std::string_view::npos) head = head.substr(0, close + 1);   // without old-style argument lines
    return text::join(text::split_ws(head), " ") + ";";
}

std::string stand_in(std::string_view text) {
    std::string head = prototype(text);
    head.pop_back();
    // (with the old-style argument lines kept: a parameter they declare, left out, is an int -- not what the header's
    // prototype of the function says)
    const std::string_view whole = text::rtrim(text.substr(0, text.find('{')));
    const auto words = text::split_ws(head);
    const bool is_void = !words.empty() && words.front() == "void";
    return std::string(whole) + (is_void ? "\n{\n}\n" : "\n{\n  return 0;\n}\n");
}

std::string plain_symbol(std::string_view symbol) {
    const std::string_view before_at = symbol.substr(0, symbol.find('@'));
    return std::string(text::trim(before_at, "_"));
}

std::vector<std::string> adjust_flags(std::span<const std::string> cflags, std::string_view text) {
    std::vector<std::string> out(cflags.begin(), cflags.end());
    const auto has = [&](std::string_view flag) { return std::find(out.begin(), out.end(), flag) != out.end(); };
    if (contains(text, "__export ") && !has("-zW")) out.push_back("-zW");   // (the Windows prologue, which the loader rewrites for an export)
    if ((contains(text, "codebreaker_entry_sp(") || contains(text, "codebreaker_stack(")) && !has("-of+"))
        out.push_back("-of+");                                              // (it reads the frame: BP+2 is where SP stood on entry)
    return out;
}

// ---- objects --------------------------------------------------------------------------------------------------------

std::optional<Bytes> omf_here(ByteView object, const stdfs::path& folder) {
    if (fs::get_env("TZ")) return std::nullopt;   // (its dates are then of that zone: the tool says)
    const auto parsed = omf_records(object);
    if (!parsed || parsed->empty() || parsed->front().kind != kOmfTheadr || parsed->front().body.empty()) return std::nullopt;
    const std::vector<OmfRecord>& recs = *parsed;
    const Bytes& first = recs.front().body;
    const std::size_t name_length = first[0];
    if (1 + name_length > first.size()) return std::nullopt;
    const Bytes name(first.begin() + 1, first.begin() + 1 + static_cast<std::ptrdiff_t>(name_length));
    const auto cut_at = std::find(name.rbegin(), name.rend(), static_cast<std::uint8_t>('\\'));
    if (cut_at == name.rend()) return std::nullopt;
    const std::size_t cut = static_cast<std::size_t>(name.rend() - cut_at) - 1;
    const Bytes was(name.begin(), name.begin() + static_cast<std::ptrdiff_t>(cut));
    const Bytes now = text::to_latin1(folder_text(folder));
    Bytes was_slash = was;
    was_slash.push_back('\\');
    Bytes out;
    for (const OmfRecord& r : recs) {
        Bytes body = r.body;
        if (r.kind == kOmfTheadr) {
            if (now.size() + name.size() - cut > 255) return std::nullopt;
            body.assign(1, static_cast<std::uint8_t>(now.size() + name.size() - cut));
            body.insert(body.end(), now.begin(), now.end());
            body.insert(body.end(), name.begin() + static_cast<std::ptrdiff_t>(cut), name.end());
        } else if (r.kind == kOmfComent && body.size() > 7 && body[1] == kComentDependency) {
            const std::size_t dep_length = body[6];
            if (7 + dep_length > body.size()) return std::nullopt;
            Bytes dep(body.begin() + 7, body.begin() + 7 + static_cast<std::ptrdiff_t>(dep_length));
            if (starts_with_bytes(dep, was_slash)) {
                Bytes moved = now;
                moved.insert(moved.end(), dep.begin() + static_cast<std::ptrdiff_t>(was.size()), dep.end());
                if (moved.size() > 255) return std::nullopt;
                const stdfs::path there = fs::from_text(text::from_latin1(moved));
                std::error_code ec;
                if (!stdfs::is_regular_file(there, ec)) return std::nullopt;
                const auto stamp = dos_stamp(there);
                if (!stamp) return std::nullopt;
                Bytes rebuilt(body.begin(), body.begin() + 2);
                rebuilt.push_back(static_cast<std::uint8_t>(stamp->first & 0xFF));
                rebuilt.push_back(static_cast<std::uint8_t>(stamp->first >> 8));
                rebuilt.push_back(static_cast<std::uint8_t>(stamp->second & 0xFF));
                rebuilt.push_back(static_cast<std::uint8_t>(stamp->second >> 8));
                rebuilt.push_back(static_cast<std::uint8_t>(moved.size()));
                rebuilt.insert(rebuilt.end(), moved.begin(), moved.end());
                rebuilt.insert(rebuilt.end(), body.begin() + 7 + static_cast<std::ptrdiff_t>(dep_length), body.end());
                body = std::move(rebuilt);
            }
        }
        Bytes record;
        record.push_back(r.kind);
        const std::size_t size = body.size() + 1;
        record.push_back(static_cast<std::uint8_t>(size & 0xFF));
        record.push_back(static_cast<std::uint8_t>((size >> 8) & 0xFF));
        record.insert(record.end(), body.begin(), body.end());
        std::uint8_t sum = 0;
        for (std::uint8_t b : record) sum = static_cast<std::uint8_t>(sum + b);
        out.insert(out.end(), record.begin(), record.end());
        out.push_back(r.check ? static_cast<std::uint8_t>(0u - sum) : 0);
    }
    return out;
}

Bytes message_kept(std::string_view message, const stdfs::path& folder) {
    return to_bytes(text::replace_all(message, folder_text(folder), kMarkedFolder));
}

std::string message_here(ByteView kept, const stdfs::path& folder) {
    return text::replace_all(text::sanitize_utf8(as_text(kept)), kMarkedFolder, folder_text(folder));
}

std::optional<std::string> object_digest(const stdfs::path& object) {
    Bytes data;
    try {
        data = fs::read_file(object);
    } catch (const IoError&) {
        return std::nullopt;   // no such file: no digest
    }
    crypto::Hasher h(crypto::Hasher::Algorithm::Sha1);
    if (!data.empty() && data[0] == kOmfTheadr) {   // OMF: records; skip the header and comments
        std::size_t p = 0;
        while (p + 3 <= data.size()) {
            const std::uint8_t kind = data[p];
            const std::size_t size = data[p + 1] | (static_cast<std::size_t>(data[p + 2]) << 8);
            if (kind != kOmfTheadr && kind != kOmfComent && kind != kOmfModend && kind != kOmfModend32) {
                const std::size_t end = std::min(data.size(), p + 3 + size - (size > 0 ? 1 : 0));   // (without the record's checksum)
                h.update(ByteView(data.data() + p, end - p));
            }
            p += 3 + size;
        }
    } else {   // COFF: all but the time it was made
        h.update(ByteView(data.data(), std::min<std::size_t>(4, data.size())));
        if (data.size() > 8) h.update(ByteView(data.data() + 8, data.size() - 8));
    }
    const auto digest = h.finish();
    return crypto::to_hex(ByteView(digest.data(), digest.size()));
}

// ---- running the compilers ------------------------------------------------------------------------------------------

CompileOutcome run_compile(const Compiler& cc, std::span<const std::string> cflags, const stdfs::path& outdir,
                           std::string_view stem, std::string_view text, milliseconds timeout) {
    const std::string source = std::string(stem) + ".c";
    write_source(outdir / source, std::string(kHeaderInclude) + std::string(text) + "\n");
    remove_quietly(outdir / (std::string(stem) + ".obj"));
    remove_quietly(outdir / (std::string(stem) + ".err"));
    std::vector<std::string> args(cflags.begin(), cflags.end());
    args.push_back(source);
    const process::ProcessResult r = run_tool(cc.exe, args, outdir, cc.env, timeout);
    std::error_code ec;
    if (r.exit_code == 0 && !r.timed_out && stdfs::is_regular_file(outdir / (std::string(stem) + ".obj"), ec)) return {"", true};
    std::string log = log_of(r);
    try {
        log += fs::read_text(outdir / (std::string(stem) + ".err"));
    } catch (const IoError&) {
        // no listing file: the log is what was on the console
    }
    if (r.timed_out) return {"the compiler ran past its time limit and was stopped", false};
    const Complaint c = complaint_wcc(log, text);
    return {c.why, c.with_line};
}

std::vector<std::string> run_never_set(const Compiler& cc, std::span<const std::string> cflags, const stdfs::path& outdir,
                                       std::string_view stem, std::string_view text) {
    static const Regex never_set(R"(W200: '(\w+)' has been referenced but never assigned a value)");
    std::vector<std::string> args;
    for (const std::string& f : cflags)
        if (!f.starts_with("-w")) args.push_back(f);
    args.push_back("-w2");
    args.push_back("-zs");
    const std::string name = std::string(stem) + "_w.c";
    write_source(outdir / name, std::string(kHeaderInclude) + std::string(text) + "\n");
    args.push_back(name);
    const process::ProcessResult r = run_tool(cc.exe, args, outdir, cc.env, std::chrono::minutes(10));
    remove_quietly(outdir / name);
    std::set<std::string> found;   // (sorted, once each)
    const std::string log = log_of(r);
    never_set.for_each_match(log, [&](const RegexMatch& m) {
        found.emplace(m.group(1));
        return true;
    });
    return {found.begin(), found.end()};
}

// ---- the builder ----------------------------------------------------------------------------------------------------

struct FunctionBuilder::Impl {
    BuilderOptions options;

    mutable std::mutex headers_mutex;
    std::map<stdfs::path, std::string> headers;   // build folder -> the key of the header in it

    std::mutex tool_mutex;
    std::map<std::string, std::string> tool_inputs;   // by compiler, INCLUDE and option variable

    mutable std::mutex served_mutex;
    std::map<std::string, ServedObject> served_objects;
    std::map<std::string, std::vector<std::string>> served_never_sets;
    int serve_depth = 0;
    ServedCounts counts;

    mutable std::mutex digests_mutex;
    std::map<stdfs::path, std::map<std::string, std::string>> digests;   // the last build of each folder

    std::mutex links_mutex;
    std::map<stdfs::path, std::string> link_logs;   // what the linker said last, by folder

    int jobs_for(std::optional<int> jobs) const { return jobs && *jobs > 0 ? *jobs : parallel::workers(options.workers); }

    // Link: <outdir>/m.dll from the lines of m.lnk. Returns the DLL's path, or nullopt and the reason.
    std::pair<std::optional<stdfs::path>, std::string> link(const stdfs::path& wlink, const Env& env, const stdfs::path& outdir,
                                                            const std::vector<std::string>& lines) {
        write_source(outdir / "m.lnk", text::join(lines, "\n") + "\n");
        remove_quietly(outdir / "m.dll");
        remove_quietly(outdir / "m.map");
        const std::vector<std::string> args = {"@m.lnk"};
        const process::ProcessResult r = run_tool(wlink, args, outdir, env, kLinkTimeout);
        const std::string log = log_of(r);
        {
            std::lock_guard lock(links_mutex);
            link_logs[outdir] = log;
        }
        std::error_code ec;
        if (!r.timed_out && stdfs::is_regular_file(outdir / "m.dll", ec)) return {outdir / "m.dll", ""};
        if (r.timed_out) return {std::nullopt, "the linker ran past its time limit and was stopped"};
        for (std::string_view line : text::lines(text::trim(log)))
            if (contains(line, "Error!")) return {std::nullopt, head_of(text::trim(line), kMaxLinkReason)};   // not the warnings around them
        std::string why = tail_of(text::trim(log), kMaxLinkLogTail);
        return {std::nullopt, why.empty() ? "the linker produced nothing" : why};
    }

    std::string last_link_log(const stdfs::path& outdir) {
        std::lock_guard lock(links_mutex);
        const auto it = link_logs.find(outdir);
        return it == link_logs.end() ? std::string() : it->second;
    }

    void note_objects(const stdfs::path& outdir, const std::map<std::string, std::string>& stems, const Reasons& dropped,
                      const std::vector<std::string>& gone = {}) {
        std::map<std::string, std::string> out;
        for (const auto& [name, stem] : stems) {
            const auto digest = object_digest(outdir / (stem + ".obj"));
            out[name] = (dropped.contains(name) ? "stand-in:" : "") + digest.value_or("none");
        }
        for (const std::string& name : gone) out[name] = "the original's";
        std::lock_guard lock(digests_mutex);
        digests[outdir] = std::move(out);
    }
};

FunctionBuilder::FunctionBuilder(BuilderOptions options) : impl_(std::make_unique<Impl>()) { impl_->options = options; }

FunctionBuilder::~FunctionBuilder() = default;

std::string FunctionBuilder::header_digest(std::string_view header_text) { return KeyBuilder().add(header_text).finish(); }

void FunctionBuilder::write_header(const stdfs::path& outdir, std::string_view header_text) {
    fs::make_dirs(outdir);
    write_source(outdir / "m.h", header_text);
    set_header_key(outdir, header_digest(header_text));
}

void FunctionBuilder::set_header_key(const stdfs::path& outdir, std::string key) {
    std::lock_guard lock(impl_->headers_mutex);
    impl_->headers[outdir] = std::move(key);
}

std::optional<std::string> FunctionBuilder::header_key(const stdfs::path& outdir) const {
    std::lock_guard lock(impl_->headers_mutex);
    const auto it = impl_->headers.find(outdir);
    if (it == impl_->headers.end()) return std::nullopt;
    return it->second;
}

std::string FunctionBuilder::tool_inputs(const Compiler& cc, std::string_view options_variable) {
    const std::string variable(options_variable);
    const std::string include = get_or(cc.env, "INCLUDE");
    const std::string options = get_or(cc.env, variable.c_str());
    const std::string memo_key = fs::to_text(cc.exe) + '\x01' + include + '\x01' + options + '\x01' + variable;
    std::lock_guard lock(impl_->tool_mutex);   // (held while it is worked out: the hashing runs once, and not in two threads at a time)
    if (const auto it = impl_->tool_inputs.find(memo_key); it != impl_->tool_inputs.end()) return it->second;
    crypto::Hasher h(crypto::Hasher::Algorithm::Sha256);
    const auto add_file = [&h](const stdfs::path& p) {
        try {
            const fs::MappedFile file(p);
            h.update(file.view());
        } catch (const IoError&) {
            h.update(as_bytes("(unreadable)"));
        }
    };
    h.update(as_bytes("exe"));
    add_file(cc.exe);
    for (std::string_view folder_text_part : text::split(include, ';')) {
        if (folder_text_part.empty()) continue;
        const stdfs::path folder = fs::from_text(folder_text_part);
        std::error_code ec;
        if (!stdfs::is_directory(folder, ec)) continue;
        std::vector<stdfs::path> files;
        for (stdfs::recursive_directory_iterator it(folder, stdfs::directory_options::skip_permission_denied, ec), end; !ec && it != end;
             it.increment(ec)) {
            std::error_code entry_ec;
            if (it->is_regular_file(entry_ec)) files.push_back(it->path());
        }
        std::sort(files.begin(), files.end());
        for (const stdfs::path& f : files) {   // every file of the INCLUDE folders, by its bytes
            h.update(as_bytes(fs::to_text(stdfs::relative(f, folder, ec))));
            add_file(f);
        }
    }
    h.update(as_bytes(variable));
    h.update(as_bytes(options));
    const auto digest = h.finish();
    std::string key = crypto::to_hex(ByteView(digest.data(), digest.size()));
    impl_->tool_inputs.emplace(memo_key, key);
    return key;
}

CompileOutcome FunctionBuilder::compile_kept(const Compiler& cc, std::span<const std::string> cflags, const stdfs::path& outdir,
                                             std::string_view stem, std::string_view text, const std::optional<std::string>& header) {
    BuildCache* cache = impl_->options.cache;
    std::optional<std::string> key;
    if (cache && header && !header->empty())
        key = KeyBuilder().add("wcc-object").add(tool_inputs(cc, "WCC")).add(cflags).add(*header).add(stem).add(text).finish();
    const stdfs::path object = outdir / (std::string(stem) + ".obj");
    if (key) {
        if (const auto had = cache->get("objects", *key)) {
            write_source(outdir / (std::string(stem) + ".c"), std::string(kHeaderInclude) + std::string(text) + "\n");
            if (had->size() >= 2 && (*had)[0] == 'E' && (*had)[1] == 'R')
                return {message_here(ByteView(had->data() + 2, had->size() - 2), outdir), true};
            if (had->size() >= 2) {
                // (as wcc writes it here, now)
                if (const auto here = omf_here(ByteView(had->data() + 2, had->size() - 2), outdir)) {
                    fs::write_file(object, *here);
                    return {"", true};
                }
            }
        }
    }
    const CompileOutcome outcome = run_compile(cc, cflags, outdir, stem, text);
    if (outcome.why.empty()) {
        if (key) {
            Bytes value = {'O', 'K'};
            const Bytes made = fs::read_file(object);
            value.insert(value.end(), made.begin(), made.end());
            cache->put("objects", *key, value);
        }
    } else if (key && outcome.keepable) {   // a rejection is kept; a compiler that fell over is not
        Bytes value = {'E', 'R'};
        const Bytes message = message_kept(outcome.why, outdir);
        value.insert(value.end(), message.begin(), message.end());
        cache->put("objects", *key, value);
    }
    return outcome;
}

std::string FunctionBuilder::served_key(std::string_view header_key, std::span<const std::string> cflags, std::string_view text) {
    std::string key(header_key);
    key += '\x1f';
    key += text::join(cflags, "\x1e");
    key += '\x1f';
    key += text;
    return key;
}

void FunctionBuilder::serve(const std::map<std::string, ServedObject>& objects,
                            const std::map<std::string, std::vector<std::string>>& never_sets) {
    std::lock_guard lock(impl_->served_mutex);
    for (const auto& [key, object] : objects) impl_->served_objects[key] = object;
    for (const auto& [key, names] : never_sets) impl_->served_never_sets[key] = names;
    ++impl_->serve_depth;
}

void FunctionBuilder::unserve() {
    std::lock_guard lock(impl_->served_mutex);
    if (--impl_->serve_depth <= 0) {
        impl_->serve_depth = 0;
        impl_->served_objects.clear();
        impl_->served_never_sets.clear();
    }
}

ServedCounts FunctionBuilder::served_counts() const {
    std::lock_guard lock(impl_->served_mutex);
    return impl_->counts;
}

std::string FunctionBuilder::compile(const Compiler& cc, std::span<const std::string> cflags, const stdfs::path& outdir,
                                     std::string_view stem, std::string_view text) {
    const std::optional<std::string> header = header_key(outdir);
    {
        std::lock_guard lock(impl_->served_mutex);
        if (impl_->serve_depth > 0) {
            if (header) {
                const auto it = impl_->served_objects.find(served_key(*header, cflags, text));
                if (it != impl_->served_objects.end()) {
                    ++impl_->counts.served;
                    if (!it->second.error.empty()) return it->second.error;
                    write_source(outdir / (std::string(stem) + ".c"), std::string(kHeaderInclude) + std::string(text) + "\n");
                    fs::write_file(outdir / (std::string(stem) + ".obj"), it->second.object);
                    return "";
                }
            }
            ++impl_->counts.compiled;
        }
    }
    return compile_kept(cc, adjust_flags(cflags, text), outdir, stem, text, header).why;
}

std::vector<std::string> FunctionBuilder::never_set_kept(const Compiler& cc, std::span<const std::string> cflags,
                                                         const stdfs::path& outdir, std::string_view stem, std::string_view text,
                                                         const std::optional<std::string>& header) {
    BuildCache* cache = impl_->options.cache;
    std::vector<std::string> flags;
    for (const std::string& f : cflags)
        if (!f.starts_with("-w")) flags.push_back(f);
    flags.push_back("-w2");
    flags.push_back("-zs");
    std::optional<std::string> key;
    if (cache && header && !header->empty())
        key = KeyBuilder().add("wcc-never-set").add(tool_inputs(cc, "WCC")).add(std::span<const std::string>(flags)).add(*header).add(text).finish();
    if (key) {
        if (const auto had = cache->get("never_set", *key)) {   // (the names come out of its warnings: the stem is not in them)
            std::vector<std::string> names;
            for (std::string_view name : text::split(as_text(*had), ','))
                if (!name.empty()) names.emplace_back(name);
            return names;
        }
    }
    std::vector<std::string> found = run_never_set(cc, cflags, outdir, stem, text);
    if (key) cache->put("never_set", *key, to_bytes(text::join(found, ",")));
    return found;
}

std::vector<std::string> FunctionBuilder::never_set(const Compiler& cc, std::span<const std::string> cflags, const stdfs::path& outdir,
                                                    std::string_view stem, std::string_view text) {
    const std::optional<std::string> header = header_key(outdir);
    if (header) {
        std::lock_guard lock(impl_->served_mutex);
        if (impl_->serve_depth > 0) {
            const auto it = impl_->served_never_sets.find(served_key(*header, cflags, text));
            if (it != impl_->served_never_sets.end()) return it->second;
        }
    }
    return never_set_kept(cc, cflags, outdir, stem, text, header);
}

// ---- every function -------------------------------------------------------------------------------------------------

Compiled FunctionBuilder::compile_all(const Compiler& cc, std::span<const std::string> cflags, std::string_view header,
                                      const Texts& texts, std::span<const std::string> order, const stdfs::path& outdir,
                                      std::optional<int> jobs, const ExtraFlags& more) {
    const Group group{std::vector<std::string>(cflags.begin(), cflags.end()), std::vector<std::string>(order.begin(), order.end()), outdir};
    return compile_groups(cc, header, texts, std::span<const Group>(&group, 1), jobs, more).front();
}

std::vector<Compiled> FunctionBuilder::compile_groups(const Compiler& cc, std::string_view header, const Texts& texts,
                                                      std::span<const Group> groups, std::optional<int> jobs, const ExtraFlags& more) {
    struct Item {
        const Group* group;
        std::string stem;
        const std::string* name;
    };
    std::vector<Item> work;
    std::vector<std::map<std::string, std::string>> group_stems(groups.size());
    for (std::size_t g = 0; g < groups.size(); ++g) {
        write_header(groups[g].outdir, header);
        for (std::size_t i = 0; i < groups[g].order.size(); ++i) {
            group_stems[g][groups[g].order[i]] = stem_name(i);
            work.push_back({&groups[g], stem_name(i), &groups[g].order[i]});
        }
    }
    // (the largest first: a long function started last would be the one the rest wait for)
    std::vector<std::size_t> run_order(work.size());
    for (std::size_t i = 0; i < run_order.size(); ++i) run_order[i] = i;
    std::stable_sort(run_order.begin(), run_order.end(),
                     [&](std::size_t a, std::size_t b) { return texts.at(*work[a].name).size() > texts.at(*work[b].name).size(); });
    std::vector<std::string> errors(work.size());
    {
        parallel::ThreadPool pool(static_cast<std::size_t>(impl_->jobs_for(jobs)));
        parallel::parallel_for(pool, run_order.size(), [&](std::size_t j) {
            const Item& w = work[run_order[j]];
            const std::vector<std::string> flags = flags_for(w.group->cflags, more, *w.name);
            errors[run_order[j]] = compile(cc, flags, w.group->outdir, w.stem, texts.at(*w.name));
        });
    }
    std::vector<Compiled> out;
    std::size_t k = 0;
    for (std::size_t g = 0; g < groups.size(); ++g) {
        Compiled c;
        c.stems = std::move(group_stems[g]);
        for (std::size_t i = 0; i < groups[g].order.size(); ++i)
            if (!errors[k + i].empty()) c.dropped.insert_or_assign(groups[g].order[i], errors[k + i]);
        k += groups[g].order.size();
        for (const auto& [name, why] : c.dropped) {   // callers of a rejected function still link
            (void)why;
            const std::vector<std::string> flags = flags_for(groups[g].cflags, more, name);
            if (!compile(cc, flags, groups[g].outdir, c.stems.at(name), stand_in(texts.at(name))).empty())
                c.stems.erase(name);   // not even a stand-in: left unresolved
        }
        out.push_back(std::move(c));
    }
    return out;
}

std::optional<std::map<std::string, std::string>> FunctionBuilder::object_digests(const stdfs::path& outdir) const {
    std::lock_guard lock(impl_->digests_mutex);
    const auto it = impl_->digests.find(outdir);
    if (it == impl_->digests.end()) return std::nullopt;
    return it->second;
}

// ---- building a DLL -------------------------------------------------------------------------------------------------

namespace {

// The numbers of a C array, 32 to a line.
std::string chunked(const Bytes& image) {
    std::string out;
    for (std::size_t i = 0; i < image.size(); i += 32) {
        if (i) out += ",\n";
        for (std::size_t j = i; j < std::min(image.size(), i + 32); ++j) {
            if (j > i) out += ",";
            out += std::to_string(image[j]);
        }
    }
    return out;
}

void require_tool(const stdfs::path& exe, const char* what) {
    std::error_code ec;
    if (exe.empty() || !stdfs::exists(exe, ec)) throw UsageError(std::string(what) + " is not set up (settings.json: openwatcom)");
}

bool contains_name(const std::vector<std::string>& v, const std::string& x) { return std::find(v.begin(), v.end(), x) != v.end(); }

}  // namespace

Built16 FunctionBuilder::build16(const Tools16& tools, const Build16Input& in) {
    require_tool(tools.wcc.exe, "OpenWatcom's 16-bit compiler");
    require_tool(tools.wlink, "OpenWatcom's linker");
    const Compiler& wcc = tools.wcc;
    const stdfs::path& outdir = in.outdir;
    ExtraFlags more;
    for (const auto& [name, seg] : in.code_segment) more[name] = {"-nt=CODE" + std::to_string(seg)};
    Compiled c = compile_all(wcc, cflags16(), in.header, in.texts, in.order, outdir, in.jobs, more);
    std::map<std::string, std::string>& stems = c.stems;
    Reasons dropped = std::move(c.dropped);
    std::vector<std::string> live;
    for (const std::string& n : in.order)
        if (!dropped.contains(n)) live.push_back(n);
    // One initialised byte, so the DLL has a data segment at all: that segment is where the original's data is put when
    // the DLL is run for judging.
    std::vector<std::string> data = {"char codebreaker_data_segment = 1;"};
    for (const auto& [n, image] : in.segments)   // other segments the code reads from
        data.push_back(std::format("uint1 __based(__segname(\"CODEBREAKER_SEGMENT_{0}\")) codebreaker_segment_{0}[] = {{\n{1}\n}};", n, chunked(image)));
    (void)compile(wcc, cflags16(), outdir, "data", text::join(data, "\n"));
    std::vector<std::string> lines = {"system windows dll", "name m.dll", "option quiet, map, nodefaultlibs, caseexact, undefsok",
                                      "libpath " + fs::to_text(tools.lib_dir)};
    std::vector<std::pair<int, std::string>> images;   // (first in their segments: at offset 0)
    for (const auto& [seg, image] : in.code_image) {
        const std::string stem = std::format("image{:02}", seg);
        const std::string source = std::format("uint1 __based(__segname(\"_CODE\")) codebreaker_code_image_{}[] = {{\n{}\n}};", seg, chunked(image));
        if (compile(wcc, plus(cflags16(), "-nt=CODE" + std::to_string(seg)), outdir, stem, source).empty())
            images.emplace_back(seg, "file " + stem + ".obj");
    }
    if (!images.empty()) lines.push_back("option packcode=0");   // (each its own segment, not packed after another)
    for (const auto& image : images) lines.push_back(image.second);
    for (const std::string& n : in.order)
        if (stems.contains(n)) lines.push_back("file " + stems.at(n) + ".obj");
    for (const auto& [seg, source] : in.code_data) {   // (beside the functions of its segment)
        const std::string stem = std::format("code{:02}", seg);
        if (compile(wcc, plus(cflags16(), "-nt=CODE" + std::to_string(seg)), outdir, stem, source).empty())
            lines.push_back("file " + stem + ".obj");
    }
    lines.push_back("file data.obj");
    lines.push_back("library clibm.lib");
    // The link lines for the routines the interrupt helpers call, when the module's C takes an interrupt (the helpers
    // are in the header): without them a helper's call went to 0:0 and its registers to offset 0 of the data segment.
    if (!in.interrupts.empty() && tools.interrupt_routines) {
        const Tools16::InterruptCode code = tools.interrupt_routines(in.interrupts);
        write_source(outdir / "interrupts.asm", code.assembly);
        const std::vector<std::string> args = {"-q", "interrupts.asm"};
        const process::ProcessResult r = run_tool(tools.wasm, args, outdir, wcc.env, kAssemblerTimeout);
        const std::string c_text = text::replace_all(code.c, "#include \"m.h\"\n", "");
        if (r.exit_code == 0 && compile(wcc, cflags16(), outdir, "interrupt_registers", c_text).empty()) {
            lines.push_back("file interrupts.obj");
            lines.push_back("file interrupt_registers.obj");
        }
    }
    // a C name compiled as __stdcall with an open argument list is the symbol `_name`
    for (const auto& [name, import] : in.imports) lines.push_back(std::format("import _{} {}.{}", name, import.module, import.ordinal));
    auto [dll, why] = impl_->link(tools.wlink, wcc.env, outdir, lines);
    // A segment holds 64K. Where the rebuilt functions of one of the original's segments come to more (other compiler; or
    // the decompiler took a table for code and wrote pages of it), the largest are replaced by stand-ins until the rest
    // fit: those few are not built, the others are judged.
    std::map<std::string, std::string> by_stem;
    for (const auto& [name, stem] : stems) by_stem[stem] = name;
    static const Regex apart_rx(R"(file (f[0-9]+)[.]obj.*not in the same segment)");
    static const Regex over_rx(R"(size of segment CODE(\d+) exceeds 64k by (\d+) bytes)");
    for (int attempt = 0; attempt < kMaxLinkRepairs; ++attempt) {
        // ... and a function that reaches near for something that is in another segment (the original called it far; the
        // decompiler's text has lost that) cannot be linked at all
        const auto apart = apart_rx.search(why);
        if (!dll && apart) {
            const auto stem = by_stem.find(std::string(apart->group(1)));
            if (stem != by_stem.end() && contains_name(live, stem->second)) {
                const std::string& n = stem->second;
                if (!compile(wcc, flags_for(cflags16(), more, n), outdir, stems.at(n), stand_in(in.texts.at(n))).empty()) break;
                dropped.insert_or_assign(n, "reaches near for something in another segment");
                live.erase(std::remove(live.begin(), live.end(), n), live.end());
                std::tie(dll, why) = impl_->link(tools.wlink, wcc.env, outdir, lines);
                continue;
            }
        }
        const auto over = over_rx.search(why);
        if (dll || !over || in.code_segment.empty()) break;
        const auto seg = text::parse_int<int>(over->group(1));
        const auto excess = text::parse_int<std::int64_t>(over->group(2));
        if (!seg || !excess) break;
        const std::int64_t need = *excess + 512;
        const auto image = std::find_if(images.begin(), images.end(), [&](const auto& im) {
            return im.first == *seg && std::find(lines.begin(), lines.end(), im.second) != lines.end();
        });
        if (image != images.end()) {   // (the original's image goes first)
            lines.erase(std::find(lines.begin(), lines.end(), image->second));
            std::tie(dll, why) = impl_->link(tools.wlink, wcc.env, outdir, lines);
            continue;
        }
        std::vector<std::pair<std::uint64_t, std::string>> sized;
        for (const std::string& n : live) {
            const auto in_segment = in.code_segment.find(n);
            if (in_segment == in.code_segment.end() || in_segment->second != *seg || !stems.contains(n) || dropped.contains(n)) continue;
            sized.emplace_back(fs::file_size(outdir / (stems.at(n) + ".obj")), n);
        }
        std::sort(sized.begin(), sized.end(), std::greater<>());   // the largest first
        std::int64_t freed = 0;
        for (const auto& [size, n] : sized) {
            if (freed >= need) break;
            if (compile(wcc, flags_for(cflags16(), more, n), outdir, stems.at(n), stand_in(in.texts.at(n))).empty()) {
                freed += static_cast<std::int64_t>(size);
                dropped.insert_or_assign(n, "too large: the functions of its segment do not fit in 64K when rebuilt");
            }
        }
        if (!freed) break;
        std::vector<std::string> kept;
        for (const std::string& n : live)
            if (!dropped.contains(n)) kept.push_back(n);
        live = std::move(kept);
        std::tie(dll, why) = impl_->link(tools.wlink, wcc.env, outdir, lines);
    }
    impl_->note_objects(outdir, stems, dropped);
    Built16 built;
    if (!dll) {
        built.dropped = std::move(dropped);
        for (const std::string& n : live) built.dropped.insert_or_assign(n, "link: " + why);
        return built;
    }
    built.dll = dll;
    built.dropped = std::move(dropped);
    std::string map_text;
    try {
        map_text = text::sanitize_utf8(fs::read_text(outdir / "m.map"));
    } catch (const IoError&) {
        // no map: no addresses
    }
    // lines of the map: <segment>:<offset>[+*] <symbol>
    static const Regex symbol_line(R"((?m)^([0-9A-Fa-f]{4}):([0-9A-Fa-f]{4,8})[+*]?\s+(\S+)\s*$)");
    const std::set<std::string> live_set(live.begin(), live.end());
    symbol_line.for_each_match(map_text, [&](const RegexMatch& m) {
        const std::string name = plain_symbol(m.group(3));
        const auto seg = text::parse_int<std::uint16_t>(m.group(1), 16);
        const auto off = text::parse_int<std::uint32_t>(m.group(2), 16);
        if (seg && off && live_set.contains(name)) built.where.try_emplace(name, SegmentOffset{*seg, *off});
        return true;
    });
    return built;
}

namespace {

// An object file in which each name in `undefined` that is a function of the original (`originals` {name: address}, and
// `func_0x<address>`) is a jump to it. Returns its file name, or nullopt.
//
// A function that is called but is not in the build -- the compiler rejected its text, the decompiler gave none, the
// decompiler did not know it was a function -- used to be nothing at all, and every caller that reached it "called into
// nothing" and was counted as behaving differently. The original's code is in memory beside the rebuilt image whenever
// that is run, so the call can simply go there: the caller is then judged with its real callee.
std::optional<std::string> in_the_original(const Tools32& tools, const stdfs::path& outdir,
                                           const std::vector<std::string>& undefined,
                                           const std::map<std::string, std::uint64_t>& originals) {
    const auto is_assembler_symbol = [](std::string_view s) {   // [A-Za-z_@?$][\w@?$]*
        if (s.empty()) return false;
        const auto word = [](char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '@' || c == '?' || c == '$'; };
        return !std::isdigit(static_cast<unsigned char>(s[0])) && std::all_of(s.begin(), s.end(), word);
    };
    std::vector<std::string> lines = {".386", ".model flat", "_TEXT segment dword public use32 'CODE'"};
    int n = 0;
    for (const std::string& symbol : std::set<std::string>(undefined.begin(), undefined.end())) {
        const std::string plain = plain_symbol(symbol);
        std::optional<std::uint64_t> at;
        if (const auto it = originals.find(plain); it != originals.end()) at = it->second;
        if (!at && plain.starts_with("func_0x")) {
            const std::string_view digits = std::string_view(plain).substr(7);
            if (digits.size() >= 6 && digits.size() <= 8) at = text::parse_int<std::uint64_t>(digits, 16);
        }
        if (!at || !is_assembler_symbol(symbol)) continue;
        lines.push_back("public " + symbol);
        lines.push_back(symbol + ":");
        lines.push_back(std::format("        push 0{:08X}h", *at));
        lines.push_back("        ret");
        ++n;
    }
    if (!n) return std::nullopt;
    lines.push_back("_TEXT ends");
    lines.push_back("end");
    write_source(outdir / "original.asm", text::join(lines, "\n") + "\n");
    if (tools.wasm.empty()) return std::nullopt;
    remove_quietly(outdir / "original.obj");
    const std::vector<std::string> args = {"-zq", "-fo=original.obj", "original.asm"};
    (void)run_tool(tools.wasm, args, outdir, tools.wcc.env, kAssemblerTimeout);
    std::error_code ec;
    return stdfs::is_regular_file(outdir / "original.obj", ec) ? std::optional<std::string>("original.obj") : std::nullopt;
}

bool is_decorated(std::string_view symbol, std::string_view name) {   // [_@]name@\d+
    if (symbol.size() < name.size() + 3 || (symbol[0] != '_' && symbol[0] != '@')) return false;
    if (symbol.substr(1, name.size()) != name || symbol[1 + name.size()] != '@') return false;
    const std::string_view digits = symbol.substr(2 + name.size());
    return !digits.empty() && std::all_of(digits.begin(), digits.end(), [](char c) { return c >= '0' && c <= '9'; });
}

}  // namespace

Built32 FunctionBuilder::build32(const Tools32& tools, const Build32Input& in) {
    require_tool(tools.wcc.exe, "OpenWatcom's 32-bit compiler");
    require_tool(tools.wlink, "OpenWatcom's linker");
    const stdfs::path& outdir = in.outdir;
    Compiled c = compile_all(tools.wcc, plus(in.cflags, "-e=20"), in.header, in.texts, in.order, outdir, in.jobs);
    std::map<std::string, std::string>& stems = c.stems;
    Reasons dropped = std::move(c.dropped);
    std::vector<std::string> live;
    for (const std::string& n : in.order)
        if (!dropped.contains(n)) live.push_back(n);
    std::vector<std::string> gone;
    for (const auto& [name, why] : dropped) {
        (void)why;
        if (in.originals.contains(name) && stems.contains(name)) gone.push_back(name);
    }
    for (const std::string& n : gone) stems.erase(n);   // no stand-in where the original itself can be called
    impl_->note_objects(outdir, stems, dropped, gone);
    // undefsok: a call to a function that is not there is left unresolved (it then fails its test)
    std::vector<std::string> lines = {"system nt_dll", "name m.dll", "option quiet", "option undefsok"};
    lines.insert(lines.end(), tools.link_head.begin(), tools.link_head.end());
    for (const std::string& n : in.order)
        if (stems.contains(n)) lines.push_back("file " + stems.at(n) + ".obj");
    for (const std::string& x : tools.runtime_lines)
        if (!x.empty()) lines.push_back(x);
    lines.push_back("library math387r.lib");   // sin, sqrt ... for a function that uses them
    std::set<std::string> seen;
    std::vector<std::string> exports;
    for (const std::string& n : in.order) {
        if (!stems.contains(n)) continue;
        const Bytes obj = fs::read_file(outdir / (stems.at(n) + ".obj"));
        if (tools.import_lines)
            for (const std::string& line : tools.import_lines(obj))
                if (!line.empty() && seen.insert(line).second) lines.push_back(line);
        if (dropped.contains(n)) continue;
        // the public symbol as the compiler decorated it: _name (cdecl), _name@N (stdcall)
        if (!tools.publics) continue;
        for (const std::string& s : tools.publics(obj)) {
            if (s == "_" + n || s == n || is_decorated(s, n)) {
                exports.push_back("export " + n + "='" + s + "'");
                break;
            }
        }
    }
    const auto with_exports = [&](std::vector<std::string> head) {
        head.insert(head.end(), exports.begin(), exports.end());
        return head;
    };
    auto [dll, why] = impl_->link(tools.wlink, tools.wcc.env, outdir, with_exports(lines));
    if (dll && !in.originals.empty()) {
        static const Regex undefined_rx(R"(W1028: (\S+) is an undefined reference)");
        std::vector<std::string> missing;
        const std::string log = impl_->last_link_log(outdir);
        undefined_rx.for_each_match(log, [&](const RegexMatch& m) {
            missing.emplace_back(m.group(1));
            return true;
        });
        if (!missing.empty()) {
            if (const auto there = in_the_original(tools, outdir, missing, in.originals)) {
                std::vector<std::string> more_lines = lines;
                more_lines.push_back("file " + *there);
                std::tie(dll, why) = impl_->link(tools.wlink, tools.wcc.env, outdir, with_exports(more_lines));
            }
        }
    }
    Built32 built;
    built.dropped = std::move(dropped);
    if (!dll) {
        for (const std::string& n : live) built.dropped.insert_or_assign(n, "link: " + why);
        return built;
    }
    built.dll = dll;
    if (tools.exports) {
        const Tools32::PeExports pe = tools.exports(*dll);
        for (const std::string& n : live)
            if (const auto it = pe.rvas.find(n); it != pe.rvas.end()) built.where[n] = pe.image_base + it->second;
    }
    return built;
}

// ---- PowerPC --------------------------------------------------------------------------------------------------------

namespace {

// A cheap fingerprint of a folder: the names, sizes and times of what is in it (cl.exe runs c1/c2 beside it).
std::string folder_fingerprint(const stdfs::path& folder) {
    std::vector<std::string> entries;
    std::error_code ec;
    for (stdfs::recursive_directory_iterator it(folder, stdfs::directory_options::skip_permission_denied, ec), end; !ec && it != end;
         it.increment(ec)) {
        std::error_code entry_ec;
        if (!it->is_regular_file(entry_ec)) continue;
        const auto size = it->file_size(entry_ec);
        const auto written = it->last_write_time(entry_ec);
        entries.push_back(std::format("{}|{}|{}", fs::to_text(stdfs::relative(it->path(), folder, ec)), size,
                                      written.time_since_epoch().count()));
    }
    std::sort(entries.begin(), entries.end());
    return KeyBuilder().add(std::span<const std::string>(entries)).finish();
}

}  // namespace

BuiltPowerPc FunctionBuilder::build_powerpc(const PowerPcTools& tools, std::string_view header, const Texts& texts,
                                            std::span<const std::string> order, std::span<const std::string> externs,
                                            const stdfs::path& outdir, std::uint64_t base, std::optional<int> jobs) {
    static const Regex cl_error(R"(\((\d+)\)\s*:\s*(?:fatal )?error (C\d+: [^\r\n]*))");
    BuildCache* cache = impl_->options.cache;
    // cl.exe reads INCLUDE and CL from the environment it is started in
    Compiler ppc{tools.cl, {}};
    for (const char* name : {"INCLUDE", "CL"})
        if (const auto v = fs::get_env(name)) ppc.env[name] = *v;
    const std::string folder_key = folder_fingerprint(tools.cl.parent_path());
    const auto compile_ppc = [&](std::string_view stem, std::string_view text) -> std::string {
        // (the key, every input of the object: the compiler by its bytes and its folder, the INCLUDE headers and CL
        // variable it reads, the options, the header, the stem the object and an error name, and the text)
        std::optional<std::string> key;
        if (cache) {
            if (const auto h = header_key(outdir))
                key = KeyBuilder().add("cl-ppc-object").add(tool_inputs(ppc, "CL")).add(folder_key).add(cflags_powerpc()).add(*h).add(stem).add(text).finish();
        }
        const stdfs::path object = outdir / (std::string(stem) + ".obj");
        if (key) {
            if (const auto had = cache->get("objects", *key); had && had->size() >= 2) {
                if ((*had)[0] == 'E' && (*had)[1] == 'R') return message_here(ByteView(had->data() + 2, had->size() - 2), outdir);
                fs::write_file(object, Bytes(had->begin() + 2, had->end()));
                return "";
            }
        }
        write_source(outdir / (std::string(stem) + ".c"), std::string(text) + "\n");
        remove_quietly(object);
        std::vector<std::string> args = cflags_powerpc();
        args.push_back("/Fo" + std::string(stem) + ".obj");
        args.push_back(std::string(stem) + ".c");
        const process::ProcessResult r = run_tool(tools.cl, args, outdir, {}, std::chrono::minutes(10));
        std::error_code ec;
        if (r.exit_code == 0 && !r.timed_out && stdfs::is_regular_file(object, ec)) {
            if (key) {
                Bytes value = {'O', 'K'};
                const Bytes made = fs::read_file(object);
                value.insert(value.end(), made.begin(), made.end());
                cache->put("objects", *key, value);
            }
            return "";
        }
        if (r.timed_out) return "the compiler ran past its time limit and was stopped";
        const std::string log = log_of(r);
        const auto m = cl_error.search(log);
        std::string why;
        if (m) {
            why = std::string(m->group(2));
        } else {
            why = tail_of(text::trim(log), kMaxReason);
            if (why.empty()) why = "the compiler failed";
        }
        why = head_of(why, kMaxReason);
        if (key && m) {
            Bytes value = {'E', 'R'};
            const Bytes message = message_kept(why, outdir);
            value.insert(value.end(), message.begin(), message.end());
            cache->put("objects", *key, value);
        }
        return why;
    };

    fs::make_dirs(outdir);
    write_header(outdir, header);
    std::map<std::string, std::string> stems;
    for (std::size_t i = 0; i < order.size(); ++i) stems[order[i]] = stem_name(i);
    std::vector<std::string> errors(order.size());
    {
        parallel::ThreadPool pool(static_cast<std::size_t>(impl_->jobs_for(jobs)));
        parallel::parallel_for(pool, order.size(), [&](std::size_t i) { errors[i] = compile_ppc(stems.at(order[i]), texts.at(order[i])); });
    }
    Reasons dropped;
    for (std::size_t i = 0; i < order.size(); ++i)
        if (!errors[i].empty()) dropped.insert_or_assign(order[i], errors[i]);
    for (const auto& [name, why] : dropped) {
        (void)why;
        if (!compile_ppc(stems.at(name), stand_in(texts.at(name))).empty()) stems.erase(name);
    }
    // every extern is a function of its own that does nothing; running stops at its address
    const std::set<std::string> extern_names(externs.begin(), externs.end());
    {
        std::vector<std::string> parts;
        int e = 0;
        for (const std::string& n : extern_names) parts.push_back(std::format("#pragma optimize(\"\", off)\nuint8 {}() {{ return {}; }}", n, e++));
        (void)compile_ppc("externs", text::join(parts, "\n"));
    }
    impl_->note_objects(outdir, stems, dropped);
    std::vector<std::string> objects;
    for (const std::string& n : order)
        if (stems.contains(n)) objects.push_back(stems.at(n) + ".obj");
    objects.push_back("externs.obj");
    write_source(outdir / "m.rsp", text::join(objects, "\n") + "\n");
    remove_quietly(outdir / "m.dll");
    remove_quietly(outdir / "m.map");
    const std::vector<std::string> link_args = {"/nologo", "/DLL", "/NOENTRY", "/NODEFAULTLIB", "/FIXED", "/XEX:NO", "/OPT:NOREF",
                                                "/OPT:NOICF", "/FORCE", "/IGNORE:4006,4088", "/MAP:m.map",
                                                std::format("/BASE:0x{:X}", base), "/OUT:m.dll", "@m.rsp", fs::to_text(tools.lib)};
    const process::ProcessResult r = run_tool(tools.link, link_args, outdir, {}, kLinkTimeout);
    std::vector<std::string> live;
    for (const std::string& n : order)
        if (!dropped.contains(n)) live.push_back(n);
    BuiltPowerPc built;
    built.dropped = std::move(dropped);
    std::error_code ec;
    if (r.timed_out || !stdfs::is_regular_file(outdir / "m.dll", ec)) {
        std::string why = r.timed_out ? "the linker ran past its time limit and was stopped" : tail_of(text::trim(log_of(r)), kMaxLinkLogTail);
        if (why.empty()) why = "the linker produced nothing";
        for (const std::string& n : live) built.dropped.insert_or_assign(n, "link: " + why);
        return built;
    }
    built.dll = outdir / "m.dll";
    std::string map_text;
    try {
        map_text = text::sanitize_utf8(fs::read_text(outdir / "m.map"));
    } catch (const IoError&) {
        // no map: no addresses
    }
    // lines of the map: ` <seg>:<offset>  <symbol>  <address> `
    static const Regex map_symbol(R"((?m)^ [0-9A-Fa-f]{4}:[0-9A-Fa-f]{8}\s+(\S+)\s+([0-9A-Fa-f]{8}) )");
    std::set<std::string> want(order.begin(), order.end());
    want.insert(extern_names.begin(), extern_names.end());
    map_symbol.for_each_match(map_text, [&](const RegexMatch& m) {
        const std::string name(m.group(1));
        if (want.contains(name)) {
            if (const auto address = text::parse_int<std::uint64_t>(m.group(2), 16)) built.where.try_emplace(name, *address);
        }
        return true;
    });
    return built;
}

}  // namespace cb::build::functions
