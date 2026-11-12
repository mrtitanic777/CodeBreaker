// cb/decompile/decompiler.cpp -- the native program: what it is asked, how it is run, what is kept of it, what it said of each
// function; and a function's C written from its L2 document.
//
// The native program (native/decompiler/decompiler.exe) is given a description of a module (decompile/describe) and writes each
// function as a block of text: a line `/*@@ <name> <hex address> */`, the notes on lines of their own, then the C. See decompiler.hpp.
#include <algorithm>
#include <cctype>
#include <chrono>
#include <future>

#include <nlohmann/json.hpp>

#include "cb/build/watcom.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/parallel.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/asm_blocks.hpp"
#include "cb/decompile/decompiler.hpp"
#include "cb/decompile/decompiler_internal.hpp"
#include "cb/decompile/frame16.hpp"
#include "cb/decompile/shape16.hpp"
#include "cb/translate/backends/c.hpp"
#include "cb/translate/l2.hpp"

namespace cb::decompile::decompiler {

namespace ir = translate::ir;
namespace nl2 = translate::native_l2;
using internal::hex;
using internal::hex8;
using internal::is_lower_hex;
using internal::is_word_char;

// ---- settings and switches ------------------------------------------------------------------------------------------------

Settings Settings::from_config(const config::Config& config) {
    Settings s;
    s.checkout = config.checkout_root();
    s.program = config.path("decompiler_bin").value_or(s.checkout / "native" / "decompiler" / "build" / "bin") / "decompiler.exe";
    const std::optional<std::string> languages = fs::get_env("CB_LANGUAGES_DIR");
    s.languages = languages && !languages->empty() ? fs::from_text(*languages) : s.checkout / "native" / "decompiler" / "languages";
    const std::string recovery = config.text("struct_recovery");
    s.struct_recovery = recovery.empty() ? "off" : recovery;
    const std::string threads = config.text("threads");
    s.threads = threads == "0" ? std::string() : threads;
    return s;
}

Policy Policy::from_environment() {
    auto set = [](const char* name) {
        const std::optional<std::string> v = fs::get_env(name);
        return v && !v->empty();
    };
    Policy p;
    const std::optional<std::string> frame = fs::get_env("CB_FRAME16");
    p.frame16 = frame && !frame->empty() && *frame != "0";
    p.asm_blocks = set("CB_ASM_BLOCKS");
    p.c_from_l2 = set("CB_C_FROM_L2");
    p.l2_requested = set("CB_L2");
    return p;
}

// ---- the native program's output ------------------------------------------------------------------------------------------

namespace {

// `/*@@ <name> <hex address> */` as the whole of a line: the name and the address; nullopt for any other line.
std::optional<std::pair<std::string, Va>> block_header(std::string_view line) {
    constexpr std::string_view kOpen = "/*@@ ", kClose = " */";
    if (line.size() < kOpen.size() + kClose.size() || !line.starts_with(kOpen) || !line.ends_with(kClose)) return std::nullopt;
    const std::string_view middle = line.substr(kOpen.size(), line.size() - kOpen.size() - kClose.size());
    const std::size_t space = middle.find(' ');
    if (space == std::string_view::npos || space == 0 || middle.find(' ', space + 1) != std::string_view::npos) return std::nullopt;
    const std::string_view name = middle.substr(0, space), digits = middle.substr(space + 1);
    if (std::any_of(name.begin(), name.end(), [](char c) { return text::is_space(c); })) return std::nullopt;
    if (digits.empty() || !std::all_of(digits.begin(), digits.end(), is_lower_hex)) return std::nullopt;
    const std::optional<std::uint64_t> va = text::parse_int<std::uint64_t>(digits, 16);
    if (!va) return std::nullopt;
    return std::pair<std::string, Va>(std::string(name), static_cast<Va>(*va));
}

// The marker lines at the start of a line of `out`: where each starts and the address it names. Only the start of the line is
// read (`/*@@ <name> <hex> */` and anything after it).
std::vector<std::pair<std::size_t, Va>> marker_lines(std::string_view out) {
    std::vector<std::pair<std::size_t, Va>> found;
    for (std::size_t at = out.find("/*@@ "); at != std::string_view::npos; at = out.find("/*@@ ", at + 1)) {
        if (at != 0 && out[at - 1] != '\n') continue;
        std::size_t i = at + 5;
        const std::size_t name_start = i;
        while (i < out.size() && !text::is_space(out[i])) ++i;
        if (i == name_start || i >= out.size() || out[i] != ' ') continue;
        ++i;
        const std::size_t digits_start = i;
        while (i < out.size() && is_lower_hex(out[i])) ++i;
        if (i == digits_start || out.substr(i, 3) != " */") continue;
        const std::optional<std::uint64_t> va = text::parse_int<std::uint64_t>(out.substr(digits_start, i - digits_start), 16);
        if (va) found.emplace_back(at, static_cast<Va>(*va));
    }
    return found;
}

// A hex number of a note or a description line.
std::int64_t hex_field(std::string_view s, std::string_view what) {
    const std::optional<std::int64_t> v = text::parse_int<std::int64_t>(s, 16);
    if (!v) throw FormatError("decompiler", 0, std::format("{}: `{}` is not a hexadecimal number", what, s));
    return *v;
}
std::int64_t decimal_field(std::string_view s, std::string_view what) {
    const std::optional<std::int64_t> v = text::parse_int<std::int64_t>(s, 10);
    if (!v) throw FormatError("decompiler", 0, std::format("{}: `{}` is not a number", what, s));
    return *v;
}

// An integer of 2**63 .. 2**64-1 anywhere in a document: what the typed tree cannot keep whole.
bool holds_big_integer(const Json& j) {
    if (j.is_number_unsigned()) return j.get<std::uint64_t>() > static_cast<std::uint64_t>(INT64_MAX);
    if (j.is_array() || j.is_object())
        for (const Json& x : j)
            if (holds_big_integer(x)) return true;
    return false;
}

// What the notes of a function say, before its L2 document is made into a node.
struct ParsedNotes {
    std::string text;
    FunctionNotes notes;
    std::optional<Json> l2;
    std::optional<std::string> l2_exact;
};

// `/*@<kind> <rest> */` as the whole of a (trimmed) line, for kind G, P, S or L2.
std::optional<std::pair<std::string_view, std::string_view>> note_line(std::string_view line) {
    constexpr std::string_view kOpen = "/*@", kClose = " */";
    if (!line.starts_with(kOpen) || !line.ends_with(kClose)) return std::nullopt;
    const std::string_view inner = line.substr(kOpen.size());
    for (const std::string_view kind : {"G", "P", "S", "L2"}) {
        if (inner.size() >= kind.size() + 1 + kClose.size() && inner.starts_with(kind) && inner[kind.size()] == ' ')
            return std::pair<std::string_view, std::string_view>(kind, inner.substr(kind.size() + 1, inner.size() - kind.size() - 1 - kClose.size()));
    }
    return std::nullopt;
}

// `<hex offset>:<bytes>:<type>` of a structure note's field: the type is word characters, stars and `^`.
bool struct_field_of(std::string_view part, core16::StructField& field) {
    const std::size_t a = part.find(':');
    const std::size_t b = a == std::string_view::npos ? a : part.find(':', a + 1);
    if (b == std::string_view::npos) return false;
    const std::string_view offset = part.substr(0, a), bytes = part.substr(a + 1, b - a - 1), type = part.substr(b + 1);
    if (offset.empty() || !std::all_of(offset.begin(), offset.end(), is_lower_hex)) return false;
    if (bytes.empty() || !std::all_of(bytes.begin(), bytes.end(), [](char c) { return c >= '0' && c <= '9'; })) return false;
    if (type.empty() || !std::all_of(type.begin(), type.end(), [](char c) { return is_word_char(c) || c == '*' || c == '^'; })) return false;
    field.offset = *text::parse_int<std::int64_t>(offset, 16);
    field.bytes = *text::parse_int<std::int64_t>(bytes, 10);
    field.type = std::string(type);
    return true;
}

ParsedNotes parse_notes(std::string_view text) {
    ParsedNotes out;
    std::size_t begin = 0;
    while (begin <= text.size()) {
        const std::size_t end = text.find('\n', begin);
        const std::string_view line = text::trim(text.substr(begin, end == std::string_view::npos ? std::string_view::npos : end - begin));
        const auto note = note_line(line);
        if (!note) break;
        const auto& [kind, rest] = *note;
        if (kind == "L2") {
            Json doc = json::parse(rest);
            if (holds_big_integer(doc)) out.l2_exact = std::string(rest);
            out.l2 = std::move(doc);
        } else if (kind == "S") {
            const std::vector<std::string_view> parts = text::split_ws(rest);
            if (parts.size() < 2) throw FormatError("decompiler", 0, std::format("a structure note without a name and size: {}", rest));
            core16::StructNote s;
            s.name = std::string(parts[0]);
            s.bytes = decimal_field(parts[1], "structure size");
            for (std::size_t i = 2; i < parts.size(); ++i) {
                core16::StructField f;
                if (!struct_field_of(parts[i], f)) throw FormatError("decompiler", 0, std::format("a structure field that is not <offset>:<bytes>:<type>: {}", parts[i]));
                s.fields.push_back(std::move(f));
            }
            out.notes.structs.push_back(std::move(s));
        } else {
            const std::vector<std::string_view> parts = text::split_ws(rest);
            if (kind == "G") {
                if (parts.size() < 2) throw FormatError("decompiler", 0, std::format("a global note without an address and size: {}", rest));
                out.notes.globals.push_back({hex_field(parts[0], "global address"), decimal_field(parts[1], "global size"),
                                             parts.size() > 2 ? decimal_field(parts[2], "global element size") : 0});
            } else {
                if (parts.size() < 2) throw FormatError("decompiler", 0, std::format("a pointer note without a name and size: {}", rest));
                if (!out.notes.pointers.contains(std::string(parts[0]))) out.notes.pointers.insert_or_assign(std::string(parts[0]), decimal_field(parts[1], "pointer size"));
            }
        }
        if (end == std::string_view::npos) {
            begin = text.size() + 1;   // (every line was a note: nothing is left)
            break;
        }
        begin = end + 1;
    }
    out.text = begin <= text.size() ? std::string(text.substr(begin)) : std::string();
    return out;
}

}  // namespace

std::vector<Block> scan_blocks(std::string_view output) {
    std::vector<std::size_t> markers;
    for (std::size_t at = output.find("/*@@ "); at != std::string_view::npos; at = output.find("/*@@ ", at + 1))
        if (at == 0 || output[at - 1] == '\n') markers.push_back(at);
    std::vector<Block> out;
    for (std::size_t k = 0; k < markers.size(); ++k) {
        const std::size_t at = markers[k];
        const std::size_t newline = output.find('\n', at);
        if (newline == std::string_view::npos) continue;   // (the header must be followed by its newline)
        const auto header = block_header(output.substr(at, newline - at));
        if (!header) continue;
        out.push_back({header->first, header->second, at, newline + 1, k + 1 < markers.size() ? markers[k + 1] : output.size()});
    }
    return out;
}

Split split_notes(Arena& arena, std::string_view text) {
    ParsedNotes parsed = parse_notes(text);
    if (parsed.l2) parsed.notes.l2 = translate::l2::from_json(arena, *parsed.l2);
    parsed.notes.l2_exact_json = std::move(parsed.l2_exact);
    return {std::move(parsed.text), std::move(parsed.notes)};
}

// ---- helpers --------------------------------------------------------------------------------------------------------------

namespace internal {

bool has_word(std::string_view text, std::string_view name) {
    for (std::size_t at = text.find(name); at != std::string_view::npos; at = text.find(name, at + 1)) {
        const bool before = at == 0 || !is_word_char(text[at - 1]);
        const bool after = at + name.size() >= text.size() || !is_word_char(text[at + name.size()]);
        if (before && after) return true;
    }
    return false;
}

std::string tail_bytes(std::string_view text, std::size_t n) {
    if (text.size() <= n) return std::string(text);
    std::size_t from = text.size() - n;
    while (from < text.size() && (static_cast<unsigned char>(text[from]) & 0xC0) == 0x80) ++from;
    return std::string(text.substr(from));
}

std::vector<Placeholder> find_placeholders(std::string_view text) {
    std::vector<Placeholder> out;
    constexpr std::string_view kPrefix = "func_0x";
    for (std::size_t at = text.find(kPrefix); at != std::string_view::npos; at = text.find(kPrefix, at + 1)) {
        if (at != 0 && is_word_char(text[at - 1])) continue;
        const std::size_t digits = at + kPrefix.size();
        if (digits + 8 > text.size()) continue;
        const std::string_view eight = text.substr(digits, 8);
        if (!std::all_of(eight.begin(), eight.end(), is_lower_hex)) continue;
        if (digits + 8 < text.size() && is_word_char(text[digits + 8])) continue;
        out.push_back({at, std::string(eight)});
    }
    return out;
}

}  // namespace internal

// ---- the decompiler ---------------------------------------------------------------------------------------------------------

Decompiler::Decompiler(Context context) : context_(std::move(context)) {
    CB_ASSERT(context_.environment != nullptr, "a decompiler needs an Environment");
}

std::filesystem::path Decompiler::scratch_for(std::initializer_list<std::string_view> parts) const {
    if (context_.scratch.empty()) return build::watcom::scratch_dir(std::span<const std::string_view>(parts.begin(), parts.size()));
    std::filesystem::path p = context_.scratch;
    for (const std::string_view part : parts) p /= std::string(part);
    return p;
}

bool Decompiler::available() const {
    std::error_code ec;
    return std::filesystem::is_regular_file(context_.settings.program, ec) && std::filesystem::is_directory(context_.settings.languages, ec);
}

const std::string& Decompiler::language_stamp() const {
    std::lock_guard<std::mutex> hold(stamp_lock_);
    if (!language_stamp_) language_stamp_ = base::cache::folder_id(context_.settings.languages);
    return *language_stamp_;
}

Decompiler::Generation Decompiler::generation() const {
    Generation g;
    g.program = base::cache::tool_id(context_.settings.program);
    // (not the folder of CB_LANGUAGES_DIR: a run that compares another folder of them shares the generation of now)
    const std::filesystem::path own = context_.settings.checkout / "native" / "decompiler" / "languages";
    g.languages = fs::same_path(own, context_.settings.languages) ? language_stamp() : base::cache::folder_id(own);
    return g;
}

// "1" when the native program is to recover structures for this description (its CB_STRUCTS), else "0". The setting
// `struct_recovery`: "off", "32" (32-bit x86 only), or "all" (the default: 16-bit code too).
std::string Decompiler::structs_for(const std::filesystem::path& description) const {
    const std::string want = text::lower(text::trim(context_.settings.struct_recovery));
    if (want == "all") return "1";
    if (want != "32") return "0";
    std::string content;
    try {
        content = fs::read_text(description);
    } catch (const IoError&) {
        return "0";
    }
    for (const std::string_view line : text::lines(content)) {
        if (!line.starts_with("arch\t")) continue;
        const std::vector<std::string_view> fields = text::split(line, '\t');
        return fields.size() > 1 && fields[1].starts_with("x86:LE:32") ? "1" : "0";
    }
    return "0";
}

bool Decompiler::recovers_structures(const describe::NeDescription& description) { return structs_for(description.path) == "1"; }

// {segment: bytes} of a 16-bit module's code, from its description's `bytes` lines (describe16).
c_text::SegmentImages images_in_description(const std::filesystem::path& description) {
    c_text::SegmentImages images;
    const std::string content = fs::read_text(description);
    for (const std::string_view line : text::lines(content)) {
        const std::vector<std::string_view> p = text::split(line, '\t');
        if (p.size() < 5 || p[0] != "bytes") continue;
        const Bytes file = fs::read_file(fs::from_text(p[2]));
        const std::int64_t offset = hex_field(p[3], "bytes offset"), length = hex_field(p[4], "bytes length");
        const std::size_t from = std::min<std::size_t>(static_cast<std::size_t>(std::max<std::int64_t>(offset, 0)), file.size());
        const std::size_t to = std::min<std::size_t>(from + static_cast<std::size_t>(std::max<std::int64_t>(length, 0)), file.size());
        images[describe::where(hex_field(p[1], "bytes address")).seg] = Bytes(file.begin() + static_cast<std::ptrdiff_t>(from), file.begin() + static_cast<std::ptrdiff_t>(to));
    }
    return images;
}

c_text::SegmentImages Decompiler::segment_images(const describe::NeDescription& description) { return images_in_description(description.path); }

translate::ir::Node* Decompiler::node_of(const Json& l2_document) {
    std::lock_guard<std::mutex> hold(arena_lock_);
    return translate::l2::from_json(*arena_, l2_document);
}

// the extra parts a cache key carries when the L2 notes are asked for: a text made with them is another text
std::vector<base::cache::KeyPart> Decompiler::l2_key(bool l2) const {
    std::vector<base::cache::KeyPart> out;
    const bool asked = l2 || context_.policy.l2_requested;
    if (asked) out.emplace_back("l2 2");
    if (asked && context_.policy.frame16) out.emplace_back("frame16");
    return out;
}

// ---- running the native program ---------------------------------------------------------------------------------------------

namespace {

std::string joined(const std::vector<std::string>& parts) {
    std::string out;
    for (const std::string& p : parts) out += p;
    return out;
}

// What the native program wrote, as text: invalid UTF-8 replaced, and every line end a plain `\n` (it writes the Windows ones, and
// the blocks are found by lines).
std::string program_text(ByteView written) {
    const std::string clean = text::sanitize_utf8(as_text(written));
    std::string out;
    out.reserve(clean.size());
    for (std::size_t i = 0; i < clean.size(); ++i) {
        if (clean[i] != '\r') {
            out += clean[i];
        } else {
            out += '\n';
            if (i + 1 < clean.size() && clean[i + 1] == '\n') ++i;
        }
    }
    return out;
}

std::int64_t threads_in(const std::map<std::string, std::string>& env) {
    const auto it = env.find("CB_THREADS");
    if (it == env.end() || it->second.empty()) return 1;
    const std::optional<std::int64_t> n = text::parse_int<std::int64_t>(it->second, 10);
    return n && *n != 0 ? *n : 1;
}

// "/* decompile failed: over the memory budget (...) */" at the very end of the output: the governor stopped the function in hand.
std::optional<std::string> governed_note(std::string_view out) {
    const std::string_view trimmed = text::rtrim(out);
    if (!trimmed.ends_with("*/")) return std::nullopt;
    std::size_t at = std::string_view::npos;
    for (const std::string_view reason : {"memory budget", "time limit"}) {
        const std::string phrase = "/* decompile failed: over the " + std::string(reason);
        const std::size_t found = trimmed.rfind(phrase);
        if (found != std::string_view::npos && (at == std::string_view::npos || found > at)) at = found;
    }
    if (at == std::string_view::npos) return std::nullopt;
    const std::string_view rest = trimmed.substr(at);
    // nothing between the phrase and the closing `*/` but text without a star
    const std::size_t close = rest.size() - 2;
    if (rest.find('*', 2) != close) return std::nullopt;
    const std::size_t over = rest.find("over the ");
    return std::string(text::trim(rest.substr(over, close - over)));
}

}  // namespace

// The native program on the description; when it dies part way (a crash that depends on what it did before -- the function it
// died on decompiles on its own), it is started again from the next function it had not written, and so on. A function a fresh
// start dies on at once is left out, with a note on stderr.
Decompiler::NativeOutput Decompiler::resuming(const std::filesystem::path& description, bool first_has_at,
                                              std::map<std::string, std::string> env, std::vector<Va> left) const {
    bool first = true;
    std::vector<std::string> outs, errs;
    for (;;) {
        process::ProcessSpec spec;
        spec.argv = {fs::to_text(context_.settings.program), fs::to_text(description)};
        if (first_has_at || !first) {
            spec.argv.push_back("--at");
            for (const Va va : left) spec.argv.push_back(hex(va));
        }
        spec.env = env;
        const process::ProcessResult r = process::run(spec);
        const std::string out = program_text(r.out);
        outs.push_back(out);
        errs.push_back(internal::tail_bytes(program_text(r.err), 1000));
        if (r.exit_code == 0) return {joined(outs), internal::tail_bytes(joined(errs), 2000), true};
        const std::vector<std::pair<std::size_t, Va>> marks = marker_lines(out);
        std::set<Va> done;
        for (const auto& m : marks) done.insert(m.second);
        std::optional<Va> last;
        if (!marks.empty()) last = marks.back().second;
        // (the last one written may be cut off by the crash: it is done again. Last as it wrote them -- in the description's
        // order, not the addresses': a function found in a later round is described at the end.)
        if (threads_in(env) > 1) {
            // Several threads at work: what ended the process was any of the functions in hand, not the last one written.
            // What is left is done on one thread, where the last written is the one in hand.
            env["CB_THREADS"] = "1";
            std::vector<Va> remaining;
            for (const Va va : left)
                if (!done.contains(va) || (last && va == *last)) remaining.push_back(va);
            left = std::move(remaining);
            if (!done.empty()) outs.back() = out.substr(0, marks.back().first);
            first = false;
            continue;
        }
        // the governor stopped it on one function (over its memory budget or its time): that one is done -- failed, as
        // written -- and is not tried again
        const std::optional<std::string> governed = governed_note(out);
        if (governed && last) errs.push_back("\n[" + hex(*last) + " stopped by the governor: " + *governed + "]\n");
        const bool stopped_by_governor = governed && last;
        {
            std::vector<Va> remaining;
            for (const Va va : left)
                if (!done.contains(va) || (last && va == *last && !stopped_by_governor)) remaining.push_back(va);
            left = std::move(remaining);
        }
        if (!done.empty() && last && !stopped_by_governor) outs.back() = out.substr(0, marks.back().first);
        const bool in_left = last && std::find(left.begin(), left.end(), *last) != left.end();
        if (done.empty() || (done.size() == 1 && last && *done.begin() == *last && in_left)) {
            Va dead = 0;
            if (!done.empty()) {
                const auto it = std::find(left.begin(), left.end(), *last);
                dead = *it;
                left.erase(it);
            } else if (!left.empty()) {
                dead = left.front();
                left.erase(left.begin());
            }
            errs.push_back("\n[decompiler died on " + hex(dead) + "; left out]\n");
        }
        first = false;
        if (left.empty()) return {joined(outs), internal::tail_bytes(joined(errs), 2000), false};
    }
}

// Run the native program on the description; chunks of functions as processes of their own. -> (stdout, stderr, whether every
// function was attempted)
Decompiler::NativeOutput Decompiler::run_native(const std::filesystem::path& description, std::span<const Va> only,
                                                std::optional<std::int64_t> max_instructions, bool l2) const {
    // Settings "threads": functions analysed at once in the one process, by hand; unset, one thread, and the functions are
    // shared among processes instead (below).
    const std::string& threads = context_.settings.threads;
    std::map<std::string, std::string> env;
    env["CB_LANGUAGES"] = fs::to_text(context_.settings.languages);
    env["CB_THREADS"] = threads.empty() ? "1" : threads;
    env["CB_STRUCTS"] = structs_for(description);   // (structures: see structs_for)
    if (max_instructions && *max_instructions) env["CB_MAX_INSTRUCTIONS"] = std::to_string(*max_instructions);
    if (l2) env["CB_L2"] = "1";
    std::vector<Va> wanted;
    if (!only.empty()) {
        wanted.assign(only.begin(), only.end());
        std::sort(wanted.begin(), wanted.end());
    } else {
        std::set<Va> found;
        const std::string description_text = fs::read_text(description);   // (lines views it: keep it alive through the loop)
        for (const std::string_view line : text::lines(description_text))
            if (line.starts_with("F\t")) {
                const std::vector<std::string_view> p = text::split(line, '\t');
                if (p.size() > 1) found.insert(hex_field(p[1], "function address"));
            }
        wanted.assign(found.begin(), found.end());
    }
    if (wanted.empty()) return {};   // (no code: a font or resource-only module)
    if (!threads.empty() || wanted.size() <= kChunk) return resuming(description, !only.empty(), env, wanted);   // (set by hand: as it says)
    // More than a chunk: the functions in fixed chunks of kChunk, in address order, each chunk a process of its own on one
    // thread, as many at once as the governor gives slots and memory for. Not the native program's threads: each keeps its own
    // analysis, and the globals one function types stay typed for the next -- so with threads a function's text depended on
    // which thread it fell to. A chunk is always the same functions in the same order, so the texts are the same however many
    // run at once, on any machine.
    std::vector<std::vector<Va>> chunks;
    for (std::size_t i = 0; i < wanted.size(); i += kChunk)
        chunks.emplace_back(wanted.begin() + static_cast<std::ptrdiff_t>(i),
                            wanted.begin() + static_cast<std::ptrdiff_t>(std::min(wanted.size(), i + kChunk)));
    const base::budget::Slots slots(context_.limits, static_cast<int>(chunks.size()), kProcessMegabytes, "decompile");
    parallel::ThreadPool pool(static_cast<std::size_t>(slots.count()));
    std::vector<std::future<NativeOutput>> futures;
    for (const std::vector<Va>& chunk : chunks)
        futures.push_back(pool.submit([this, &description, &env, &chunk] { return resuming(description, true, env, chunk); }));
    std::vector<NativeOutput> parts;
    for (std::future<NativeOutput>& f : futures) parts.push_back(f.get());
    std::vector<std::pair<Va, std::string>> blocks;
    bool whole = true;
    std::vector<std::string> errs;
    for (const NativeOutput& p : parts) {
        for (const Block& b : scan_blocks(p.out)) blocks.emplace_back(b.va, p.out.substr(b.start, b.end - b.start));
        errs.push_back(p.err);
        whole = whole && p.whole;
    }
    std::sort(blocks.begin(), blocks.end());
    std::string out;
    for (const auto& b : blocks) out += b.second;
    return {std::move(out), internal::tail_bytes(joined(errs), 2000), whole};
}

namespace {

// the description's `bytes` lines name files; what the cache keys on is what the files hold, not where they are
std::string bytes_digest(std::map<std::string, std::string>& by_file, const std::string& path) {
    auto it = by_file.find(path);
    if (it == by_file.end()) it = by_file.emplace(path, base::cache::key({fs::read_file(fs::from_text(path))})).first;
    return it->second;
}

// the text of the description with each `bytes` line's file name replaced by the file's digest
std::string keyed_text(const std::string& description_text) {
    std::map<std::string, std::string> digests;
    std::string out;
    std::size_t at = 0;
    while (at < description_text.size()) {
        const std::size_t end = description_text.find('\n', at);
        const std::string_view line = std::string_view(description_text).substr(at, end == std::string::npos ? std::string::npos : end - at);
        bool replaced = false;
        if (line.starts_with("bytes\t")) {
            const std::size_t first = line.find('\t', 6);
            const std::string_view hex_part = line.substr(6, first == std::string_view::npos ? 0 : first - 6);
            if (first != std::string_view::npos && !hex_part.empty() && std::all_of(hex_part.begin(), hex_part.end(), [](char c) { return std::isxdigit(static_cast<unsigned char>(c)) != 0; })) {
                const std::size_t second = line.find_first_of("\t\r\n", first + 1);
                if (second != std::string_view::npos && second > first + 1 && line[second] == '\t') {
                    out += line.substr(0, first + 1);
                    out += bytes_digest(digests, std::string(line.substr(first + 1, second - first - 1)));
                    out += line.substr(second);
                    replaced = true;
                }
            }
        }
        if (!replaced) out += line;
        if (end == std::string::npos) break;
        out += '\n';
        at = end + 1;
    }
    return out;
}

constexpr std::string_view kBlockOfNothing = "/* decompile failed";
constexpr std::string_view kTooLong = "/* decompile failed: Flow exceeded maximum allowable instructions";

}  // namespace

// The native program on a description as a whole (or on `only`). The description holds the module's bytes and every fact given
// with them, so what the program says of it is kept under its hash: a module is decompiled once.
Decompiler::Texts Decompiler::run_whole(const std::filesystem::path& description, std::span<const Va> only,
                                        std::optional<std::int64_t> max_instructions, bool l2) const {
    const std::string description_text = fs::read_text(description);
    std::string only_hex;
    for (const Va va : only) only_hex += (only_hex.empty() ? "" : "\x1f") + hex(va);
    base::cache::Cache* cache = context_.cache;
    std::string key;
    std::string out, err;
    bool had = false;
    if (cache != nullptr) {
        std::vector<base::cache::KeyPart> parts{kRevision, base::cache::tool_id(context_.settings.program), language_stamp(), keyed_text(description_text), only_hex,
                                                max_instructions && *max_instructions ? std::to_string(*max_instructions) : std::string(), structs_for(description)};
        for (base::cache::KeyPart& p : l2_key(l2)) parts.push_back(std::move(p));
        key = base::cache::key(parts);
        if (const std::optional<Bytes> kept = cache->get("decompiled", key)) {
            const std::string all = text::sanitize_utf8(as_text(*kept));
            const std::size_t cut = all.find('\0');
            if (cut != std::string::npos) {
                out = all.substr(0, cut);
                err = all.substr(cut + 1);
                had = true;
            }
        }
    }
    if (!had) {
        const NativeOutput made = run_native(description, only, max_instructions, l2);
        out = made.out;
        err = made.err;
        if (cache != nullptr && made.whole && !out.empty()) {
            const std::string all = out + std::string(1, '\0') + err;
            cache->put("decompiled", key, as_bytes(all));
        }
    }
    Texts result;
    std::vector<Va> large;
    for (const Block& b : scan_blocks(out)) {
        const std::string_view t = text::trim(std::string_view(out).substr(b.body, b.end - b.body));
        if (t.starts_with(kTooLong)) large.push_back(b.va);
        if (t.empty() || t.starts_with(kBlockOfNothing)) continue;
        if (!result.texts.contains(b.name)) {
            result.texts.insert_or_assign(b.name, std::string(t));
            result.order.push_back(b.name);
            result.by_va[b.va] = b.name;
        }
    }
    if (!large.empty() && !(max_instructions && *max_instructions)) {   // longer than a wrong start would run: again, with room
        const Texts more = run_functions(description, large, kLargeInstructionLimit, l2);
        for (const auto& [name, text] : more.texts) result.texts.insert_or_assign(name, text);
        for (const std::string& n : more.order)
            if (std::find(result.order.begin(), result.order.end(), n) == result.order.end()) result.order.push_back(n);
        for (const auto& [va, name] : more.by_va) result.by_va[va] = name;
    }
    result.err = err;
    return result;
}

// Run the native program on a description. Works for any processor it has a language for; `only` limits it to those addresses.
// Each function's text is kept on its own with the lines of the description it was made from: its own line, the calls through
// pointers inside it, and the line of every function and import its text calls (or that there was none, for a `func_0x...` it
// calls). A function whose lines are all as they were is not decompiled again: a change to the description -- one function found
// to take its arguments off by hand -- has only the functions it touches made again.
Decompiler::Texts Decompiler::run_functions(const std::filesystem::path& description, std::span<const Va> only,
                                            std::optional<std::int64_t> max_instructions, bool l2) const {
    const std::string description_text = fs::read_text(description);
    std::map<Va, std::string> fn_line;
    std::map<std::string, std::string> name_line;
    std::vector<std::pair<Va, std::string>> o_lines;
    std::vector<std::string> base_lines;
    std::map<std::string, std::string> digests;
    for (const std::string_view ln : text::lines(description_text)) {
        const std::vector<std::string_view> p = text::split(ln, '\t');
        if (p[0] == "F" && p.size() > 2) {
            fn_line[hex_field(p[1], "function address")] = std::string(ln);
            name_line[std::string(p[2])] = std::string(ln);
        } else if (p[0] == "X" && p.size() > 2) {
            name_line[std::string(p[2])] = std::string(ln);
            base_lines.emplace_back(ln);   // (an import: every caller's as well)
        } else if (p[0] == "O" && p.size() > 2) {
            o_lines.emplace_back(hex_field(p[1], "call address"), std::string(ln));
        } else if (p[0] == "bytes" && p.size() > 2) {
            base_lines.push_back("bytes\t" + std::string(p[1]) + "\t" + bytes_digest(digests, std::string(p[2])));
        } else {
            base_lines.emplace_back(ln);
        }
    }
    if (fn_line.empty()) return run_whole(description, only, max_instructions, l2);
    base::cache::Cache* cache = context_.cache;
    std::string common;
    if (cache != nullptr) {
        std::vector<base::cache::KeyPart> parts{kRevision, base::cache::tool_id(context_.settings.program), language_stamp(), base_lines,
                                                max_instructions && *max_instructions ? std::to_string(*max_instructions) : std::string(), structs_for(description)};
        for (base::cache::KeyPart& p : l2_key(l2)) parts.push_back(std::move(p));   // (with the L2 notes: another text)
        common = base::cache::key(parts);
    }
    std::vector<Va> starts;
    for (const auto& kv : fn_line) starts.push_back(kv.first);   // (sorted: a map)
    // The lines of the calls through pointers a function holds: those at an address from its start to the next function's (in the
    // same 64K) -- or the end of its 64K.
    auto own = [&](Va va) {
        const auto it = std::upper_bound(starts.begin(), starts.end(), va);
        const Va end = (it != starts.end() && (*it >> 16) == (va >> 16)) ? *it : ((va >> 16) + 1) << 16;
        std::vector<std::string> lines;
        for (const auto& [site, ln] : o_lines)
            if (va <= site && site < end) lines.push_back(ln);
        return lines;
    };
    auto entry_key = [&](Va va) { return base::cache::key({common, fn_line.at(va), own(va)}); };
    std::vector<Va> wanted;
    if (!only.empty()) {
        wanted.assign(only.begin(), only.end());
        std::sort(wanted.begin(), wanted.end());
    } else {
        wanted = starts;
    }
    struct Reused {
        std::string name, text;
    };
    std::map<Va, Reused> reused;
    std::vector<Va> misses;
    for (const Va va : wanted) {
        if (!fn_line.contains(va) || cache == nullptr) {
            misses.push_back(va);
            continue;
        }
        // an entry: name, text, then the callees it was made with (name, line) and the placeholders it showed
        const std::optional<std::vector<std::string>> had = cache->value<std::vector<std::string>>("decompiled_fn", entry_key(va));
        if (!had || had->size() < 3) {
            misses.push_back(va);
            continue;
        }
        const std::vector<std::string>& e = *had;
        const std::size_t deps = static_cast<std::size_t>(decimal_field(e[2], "cache entry"));
        bool ok = e.size() >= 3 + 2 * deps;
        for (std::size_t i = 0; ok && i < deps; ++i) {
            const auto it = name_line.find(e[3 + 2 * i]);
            ok = it != name_line.end() && it->second == e[4 + 2 * i];
        }
        for (std::size_t i = 3 + 2 * deps; ok && i < e.size(); ++i) {   // (a function that was not there then must not be there now)
            const std::optional<std::int64_t> unknown = text::parse_int<std::int64_t>(e[i], 16);
            ok = unknown && !fn_line.contains(*unknown);
        }
        if (ok) reused[va] = Reused{e[0], e[1]};
        else misses.push_back(va);
    }
    Texts out;
    if (!misses.empty()) {
        const bool whole_set = !only.empty() || misses.size() < starts.size();
        out = run_whole(description, whole_set ? std::span<const Va>(misses) : std::span<const Va>(), max_instructions, l2);
    }
    if (cache != nullptr) {
        for (const auto& [va, name] : out.by_va) {   // (kept for the next time, with their lines)
            if (!fn_line.contains(va)) continue;
            const std::string& text = out.texts.at(name);
            std::vector<std::string> entry{name, text, ""};
            std::set<std::string> seen;
            for (std::size_t i = 0; i < text.size();) {
                if (!is_word_char(text[i])) {
                    ++i;
                    continue;
                }
                std::size_t j = i;
                while (j < text.size() && is_word_char(text[j])) ++j;
                const std::string word = text.substr(i, j - i);
                std::size_t k = j;
                while (k < text.size() && text::is_space(text[k])) ++k;
                const bool identifier = !(word[0] >= '0' && word[0] <= '9');
                if (identifier && k < text.size() && text[k] == '(' && word != name && !seen.contains(word)) {
                    const auto line = name_line.find(word);
                    if (line != name_line.end()) {
                        seen.insert(word);
                        entry.push_back(word);
                        entry.push_back(line->second);
                    }
                }
                i = j;
            }
            entry[2] = std::to_string(seen.size());
            for (const internal::Placeholder& p : internal::find_placeholders(text)) entry.push_back(p.digits);
            cache->keep("decompiled_fn", entry_key(va), entry);
        }
    }
    for (const auto& [va, r] : reused) {
        out.texts.insert_or_assign(r.name, r.text);
        out.by_va[va] = r.name;
    }
    out.order.clear();
    for (const auto& [va, name] : out.by_va) out.order.push_back(name);
    return out;
}

// ---- run() ------------------------------------------------------------------------------------------------------------------

Run Decompiler::run(const std::filesystem::path& description, std::span<const Va> only, const RunOptions& options) {
    std::call_once(kinds_once_, [this] {
        if (context_.cache == nullptr) return;
        // (the kinds this makes: results of another revision, program or language files are not asked for again)
        const Generation g = generation();
        const std::string generation_text = base::cache::key({std::to_string(g.revision), g.program, g.languages});
        context_.cache->declare_kind("decompiled", {.scope = base::cache::Scope::State, .generation = generation_text});
        context_.cache->declare_kind("decompiled_fn", {.scope = base::cache::Scope::State, .generation = generation_text});
    });
    const bool l2 = options.l2.value_or(context_.policy.c_from_l2);
    const Texts made = run_functions(description, only, options.max_instructions, l2);
    std::optional<c_text::SegmentImages> images;
    if (l2 && context_.policy.tree_images()) images = images_in_description(description);
    Run out;
    out.arena = arena_;
    out.order = made.order;
    out.by_va = made.by_va;
    out.stderr_text = made.err;
    for (const auto& [name, printed] : made.texts) {
        ParsedNotes parsed = parse_notes(printed);
        FunctionNotes notes = std::move(parsed.notes);
        if (parsed.l2) {
            std::lock_guard<std::mutex> hold(arena_lock_);
            notes.l2 = translate::l2::from_json(*arena_, *parsed.l2);
        }
        notes.l2_exact_json = std::move(parsed.l2_exact);
        std::string text = std::move(parsed.text);
        if (l2 && options.write) text = c_from_l2(text, notes, images, nullptr).value_or(text);
        out.notes.insert_or_assign(name, std::move(notes));
        out.texts.insert_or_assign(name, std::move(text));
    }
    return out;
}

// ---- a function's C from its L2 document -------------------------------------------------------------------------------------

// A function's C written by the C back end from its L2 document in place of the C the decompiler printed (`text`). Without a
// document, or one the back end declines, the printed C stays. With `images` ({segment: code}; tree_images) a 16-bit function's tree
// is first given the passes that read its instructions: with CB_ASM_BLOCKS its interrupts made the _asm blocks its instructions
// say; with CB_FRAME16=1 its locals laid out as the original's frame and its statements shaped as C 8 writes them (frame16,
// shape16) -- once: notes.l2 is the document so made (the module's L2 tree is made of them), notes.placed says it is. `module`:
// what the module says of the function, for the back end's passes that need it, and where the back end puts what it reports.
std::optional<std::string> Decompiler::c_from_l2(const std::optional<std::string>& text, FunctionNotes& notes,
                                                 const std::optional<c_text::SegmentImages>& images, c_text::ModuleFacts* module) {
    if (notes.l2 == nullptr) return text;
    const auto* segment_origin = notes.l2->origin ? std::get_if<ir::OriginSegment>(notes.l2->origin.get()) : nullptr;
    if (images && !images->empty() && segment_origin != nullptr && images->contains(segment_origin->segment) && !notes.placed) {
        const Bytes& code = images->at(segment_origin->segment);
        const auto offset = static_cast<std::uint32_t>(segment_origin->offset);
        if (context_.policy.asm_blocks || context_.policy.frame16) {
            Json doc = notes.l2_exact_json ? json::parse(*notes.l2_exact_json) : translate::l2::to_json(*notes.l2);
            if (context_.policy.asm_blocks) asm_blocks::place(doc, code, offset, static_cast<std::uint32_t>(code.size()));
            if (context_.policy.frame16) {   // (and its locals as the frame had them)
                frame16::lay_out(doc, code, offset, static_cast<std::uint32_t>(code.size()));
                shape16::reshape(doc, code);
            }
            std::lock_guard<std::mutex> hold(arena_lock_);
            notes.l2 = translate::l2::from_json(*arena_, doc);
        }
        notes.placed = true;
    }
    static const std::vector<std::string> kMoved = [] {
        const auto passes = translate::backends::c::moved_passes();
        return std::vector<std::string>(passes.begin(), passes.end());
    }();
    const nl2::Library& lib = library();
    std::string body;
    try {
        const Json doc = notes.l2_exact_json ? json::parse(*notes.l2_exact_json) : translate::l2::to_json(*notes.l2);   // (its far pointers: L2's types say which)
        if (module != nullptr) {
            Json facts = internal::facts_json(*module);
            try {
                body = lib.function(doc, kMoved, std::nullopt, &facts);
            } catch (...) {
                module->left = json::dump(facts, json::Style::Compact);
                throw;
            }
            internal::apply_left(*module, facts);
            module->left = json::dump(facts, json::Style::Compact);
        } else {
            body = lib.function(doc, kMoved);
        }
    } catch (const nl2::LibraryError& e) {
        const std::string& kind = e.type();
        if (kind == "Decline" || kind == "ValueError" || kind == "KeyError") return text;
        throw;
    }
    notes.from_l2 = true;
    return "\n" + body;
}

}  // namespace cb::decompile::decompiler
