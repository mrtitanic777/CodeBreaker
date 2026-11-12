// cb-unity: alone
#include "cb/exact/mspascal.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <set>

#include <nlohmann/json.hpp>

#include "cb/base/cache.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/omf.hpp"
#include "cb/exact/plugin.hpp"

namespace cb::exact::mspascal {

namespace stdfs = std::filesystem;

namespace {

Bytes from_hex(std::string_view hex) {
    Bytes out;
    for (std::size_t i = 0; i + 1 < hex.size(); i += 2) out.push_back(static_cast<std::uint8_t>(text::parse_int<unsigned>(hex.substr(i, 2), 16).value()));
    return out;
}

bool pas_is_word_char(char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_'; }

std::string pas_upper_word(std::string_view s) { return text::upper(s); }

// Pass one says "No Errors" after its last banner when the source is good.
bool pas_no_errors(const std::string& console) {
    const std::size_t at = console.rfind("Pass One");
    const std::string_view after = at == std::string::npos ? std::string_view(console) : std::string_view(console).substr(at + 8);
    return after.find("No Errors") != std::string_view::npos;
}

// At most `max` bytes of UTF-8 text, never in the middle of a character.
std::string pas_prefix(std::string_view s, std::size_t max) {
    if (s.size() <= max) return std::string(s);
    std::size_t n = max;
    while (n > 0 && (static_cast<unsigned char>(s[n]) & 0xC0) == 0x80) --n;
    return std::string(s.substr(0, n));
}

// Every position `needle` is found at in `hay[0, end)`, the matches not overlapping.
std::vector<std::size_t> pas_find_all(ByteView hay, std::size_t end, ByteView needle) {
    std::vector<std::size_t> out;
    const ByteView scope = hay.subspan(0, std::min(end, hay.size()));
    for (std::size_t at = 0; at + needle.size() <= scope.size();) {
        const auto it = std::search(scope.begin() + static_cast<std::ptrdiff_t>(at), scope.end(), needle.begin(), needle.end());
        if (it == scope.end()) break;
        const std::size_t found = static_cast<std::size_t>(it - scope.begin());
        out.push_back(found);
        at = found + needle.size();
    }
    return out;
}

}  // namespace

const std::vector<Bytes>& entry_helpers() {
    static const std::vector<Bytes> helpers = {
        from_hex("5b32e42e8a07433c807208247f8ae02e8a07432e8b0f4343558bec2be9"),   // 1.10, 2.0x
        from_hex("5b2e8b0f4343558bec2be9"),                                       // 3.0x: only the frame size
        from_hex("5e2e8b0c4646558bec2be9")};                                      // 3.11: through SI
    return helpers;
}

const Bytes& inline_frame() {
    static const Bytes frame = from_hex("558bec81ec");   // (3.11: the frame set up in line)
    return frame;
}

std::vector<std::string> emit_order(std::string_view source) {
    static const Regex comments(R"(\{[^}]*\}|\(\*.*?\*\))", RegexFlags::DotAll);
    static const Regex quoted(R"('[^'\n]*')");
    static const Regex token(R"([A-Za-z_][A-Za-z_0-9]*|[;()])");
    const std::string no_comments = comments.replace_all(source, [](const RegexMatch&) { return std::string(" "); });
    const std::string plain = quoted.replace_all(no_comments, [](const RegexMatch&) { return std::string("''"); });
    std::vector<std::string> toks;
    token.for_each_match(plain, [&](const RegexMatch& m) {
        toks.emplace_back(m.whole());
        return true;
    });
    // each procedure being read: its name, whether its body (BEGIN ...) is open, and the depth in it (or in a RECORD before it)
    struct Open {
        std::string name;
        bool in_body = false;
        int depth = 0;
    };
    std::vector<std::string> out;
    std::vector<Open> st;
    std::size_t k = 0;
    while (k < toks.size()) {
        const std::string w = pas_upper_word(toks[k]);
        if ((w == "PROCEDURE" || w == "FUNCTION") && (st.empty() || !st.back().in_body) && !(!st.empty() && st.back().depth)) {
            if (k + 1 >= toks.size()) break;
            std::size_t m = k + 2;
            if (m < toks.size() && toks[m] == "(") {   // the parameters
                int d = 1;
                ++m;
                while (d && m < toks.size()) {
                    d += (toks[m] == "(") - (toks[m] == ")");
                    ++m;
                }
            }
            while (m < toks.size() && toks[m] != ";") ++m;
            if (m >= toks.size()) break;   // a heading that never ends: nothing more to find
            ++m;
            if (m < toks.size()) {
                const std::string next = pas_upper_word(toks[m]);
                if (next == "FORWARD" || next == "EXTERN" || next == "EXTERNAL") {
                    k = m + 1;
                    continue;
                }
            }
            st.push_back({pas_upper_word(toks[k + 1]), false, 0});
            k = m;
            continue;
        }
        if (!st.empty()) {
            Open& top = st.back();
            if (!top.in_body) {
                if (w == "RECORD") {
                    ++top.depth;
                } else if (w == "END" && top.depth) {
                    --top.depth;
                } else if (w == "BEGIN" && !top.depth) {
                    top.in_body = true;
                    top.depth = 1;
                }
            } else if (w == "BEGIN" || w == "CASE") {
                ++top.depth;
            } else if (w == "END") {
                if (--top.depth == 0) {
                    out.push_back(top.name);
                    st.pop_back();
                }
            }
        }
        ++k;
    }
    return out;
}

Bytes publish(ByteView object, const OrderedMap<std::string, std::uint32_t>& procs, std::string_view code_seg) {
    const formats::omf::Object o = formats::omf::read(object);
    std::size_t index = 0;
    for (; index < o.segdefs.size(); ++index)
        if (o.segdefs[index].name == code_seg) break;
    if (index == o.segdefs.size()) throw FormatError("mspascal object", 0, std::format("the code segment {} is not defined", code_seg));
    const std::size_t si = index + 1;
    // (no group; the segment)
    Bytes body = {0};
    if (si < 0x80) body.push_back(static_cast<std::uint8_t>(si));
    else {
        body.push_back(static_cast<std::uint8_t>(0x80 | (si >> 8)));
        body.push_back(static_cast<std::uint8_t>(si & 0xFF));
    }
    std::vector<std::pair<std::string, std::uint32_t>> sorted(procs.begin(), procs.end());
    std::stable_sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) { return a.second < b.second; });
    for (const auto& [name, offset] : sorted) {
        append(body, formats::omf::name(as_text(text::to_latin1(name))));
        body.push_back(static_cast<std::uint8_t>(offset & 0xFF));
        body.push_back(static_cast<std::uint8_t>((offset >> 8) & 0xFF));
        body.push_back(0);
    }
    const Bytes pubdef = formats::omf::record(0x90, body);
    Bytes out;
    for (const formats::omf::Record& r : formats::omf::records(object)) {
        if (r.type == 0x8A || r.type == 0x8B) append(out, pubdef);   // before the MODEND
        append(out, formats::omf::record(r.type, r.payload));
    }
    return out;
}

Bytes name_procedures(ByteView object, std::string_view listing, std::string_view source) {
    static const Regex procedure_line(R"(^\s*Procedure/Function : (\S+))");
    static const Regex first_address(R"(^\s*\*\*\s*([0-9A-F]{6})\s)");
    static const Regex program_line(R"(^\s*(?:PROGRAM|MODULE)\s+([A-Z_0-9]+))", RegexFlags::Multiline | RegexFlags::IgnoreCase);
    const formats::omf::Object o = formats::omf::read(object);
    // the first code segment that holds code: not the compiler's data segments
    std::string seg;
    for (const formats::omf::SegDef& s : o.segdefs) {
        if (s.name == "HEAP" || s.name == "STACK" || s.name == "DATA" || s.name == "CONST" || s.name == "MEMORY") continue;
        const auto filled = o.filled.find(s.name);
        if (filled && *filled) {
            seg = s.name;
            break;
        }
    }
    if (seg.empty()) return Bytes(object.begin(), object.end());
    const Bytes& whole = *o.segments.find(seg);
    const ByteView code = ByteView(whole).subspan(0, std::min<std::size_t>(whole.size(), *o.filled.find(seg)));
    OrderedMap<std::string, std::uint32_t> procs;
    std::string current;
    for (const std::string_view line : text::split(text::replace_all(text::replace_all(std::string(listing), "\r", ""), "\f", ""), '\n')) {   // (the code listing: each procedure's first address)
        if (const auto m = procedure_line.search(line)) {
            current = pas_upper_word(m->group(1));
            continue;
        }
        if (const auto m = first_address.search(line); m && !current.empty() && !procs.contains(current))
            procs.insert_or_assign(current, text::parse_int<std::uint32_t>(m->group(1), 16).value());
    }
    // where the entry helper is, in the version that has one
    std::optional<std::size_t> helper;
    for (const Bytes& h : entry_helpers()) {
        const auto it = std::search(code.begin(), code.end(), h.begin(), h.end());
        if (it != code.end()) helper = std::max(helper.value_or(0), static_cast<std::size_t>(it - code.begin()));
    }
    if (procs.empty()) {   // (no listing: where the code calls the entry helper)
        const std::size_t end = helper.value_or(code.size());
        std::vector<std::size_t> starts;
        if (helper) {
            for (std::size_t a = 0; a < std::min(end, code.size()); ++a) {
                if (code[a] != 0xE8 || a + 3 > end || a + 3 > code.size()) continue;
                const std::int16_t rel = static_cast<std::int16_t>(code[a + 1] | (code[a + 2] << 8));
                if (static_cast<std::int64_t>(a) + 3 + rel == static_cast<std::int64_t>(*helper)) starts.push_back(a);
            }
        }
        for (const std::size_t at : pas_find_all(code, end, inline_frame())) starts.push_back(at);
        std::sort(starts.begin(), starts.end());
        starts.erase(std::unique(starts.begin(), starts.end()), starts.end());
        std::vector<std::string> names = emit_order(source);
        if (const auto prog = program_line.search(source)) names.push_back(pas_upper_word(prog->group(1)));
        if (starts.size() + 1 == names.size()) names.pop_back();   // (a module's empty block is no code)
        for (std::size_t i = 0; i < std::min(names.size(), starts.size()); ++i) procs.insert_or_assign(names[i], static_cast<std::uint32_t>(starts[i]));
    }
    std::set<std::string> have;
    for (const formats::omf::Public& p : o.publics) have.insert(p.name);
    OrderedMap<std::string, std::uint32_t> wanted;
    for (const auto& [n, a] : procs)
        if (!have.contains(n)) wanted.insert_or_assign(n, a);
    if (helper) wanted.insert_or_assign("(helper)", static_cast<std::uint32_t>(*helper));
    return wanted.empty() ? Bytes(object.begin(), object.end()) : publish(object, wanted, seg);
}

// ---- the toolchain --------------------------------------------------------------------------------------------------

MsPascal::MsPascal(toolchain::Profile profile, std::optional<std::string> variant, const toolchain::Context& context, Hooks hooks)
    : toolchain::Toolchain(profile, variant, context), ctx_(context), hooks_(std::move(hooks)),
      raw_(std::make_unique<Json>(plugin::profile_json(*context.locations, profile.name))) {
    const Json& variants = (*raw_)["variants"];
    const Json empty = Json::object();
    const Json& mine = variants.is_object() && variants.contains(this->variant()) ? variants[this->variant()] : empty;
    version_ = plugin::text_or(mine, "version", "1.10");
    const auto helper_data = [](const Json& o) -> std::optional<std::uint32_t> {
        return o.is_object() && o.contains("helper_data") && o["helper_data"].is_number_unsigned() ? std::optional<std::uint32_t>(o["helper_data"].get<std::uint32_t>()) : std::nullopt;
    };
    helper_data_ = helper_data(mine).value_or(helper_data(*raw_).value_or(3));
    plugin_key_ = crypto::to_hex(crypto::sha1(to_bytes(name() + version_ + std::string(kCacheVersion)))).substr(0, 12);
}

MsPascal::~MsPascal() = default;

stdfs::path MsPascal::kit() const {
    CB_ASSERT(ctx_.locations != nullptr, "a toolchain needs the locations of its kit");
    const auto kit_name = profile_data().kit;
    if (!kit_name) throw UsageError(name() + " names no kit to run its tool from");
    const stdfs::path k = ctx_.locations->kit(version_ != "1.10" ? *kit_name : plugin::text_or(*raw_, "kit110", *kit_name));
    if (version_ != "1.10") return k / fs::from_text(version_);
    // (the pre-release kit: its files in zips)
    const stdfs::path d = ctx_.locations->state / "exact" / "stage" / "mspascal110";
    if (!stdfs::exists(d / "PAS2.EXE")) {
        if (!hooks_.extract_zip) throw UnsupportedError("mspascal 1.10", 0, "the pre-release kit is kept as zips and needs Hooks::extract_zip");
        stdfs::create_directories(d);
        std::vector<stdfs::path> zips;
        for (const stdfs::directory_entry& e : stdfs::directory_iterator(k))
            if (text::iends_with(e.path().filename().string(), ".zip")) zips.push_back(e.path());
        std::sort(zips.begin(), zips.end());
        for (const stdfs::path& z : zips) hooks_.extract_zip(z, d);
    }
    return d;
}

std::string MsPascal::cache_key(ByteView source, std::string_view file_name) const { return plugin::compile_key(plugin::crlf(source), {}, file_name, plugin_key_); }

pascal_join::CompileResult MsPascal::build(ByteView source, std::string_view file_name) const {
    const modules::CompileResult r = run_passes(plugin::crlf(source), std::string(file_name));
    return {r.object, r.console};
}

pascal_join::CompileResult MsPascal::compile(ByteView source, std::string_view file_name) const {
    const std::string key = cache_key(source, file_name);
    if (ctx_.cache) {
        if (auto got = ctx_.cache->value<plugin::Built>(toolchain::kCacheKind, key)) return {std::move(got->object), std::move(got->console)};
    }
    const pascal_join::CompileResult built = build(source, file_name);
    if (ctx_.cache) ctx_.cache->keep(toolchain::kCacheKind, key, plugin::Built{built.object, built.console});
    return built;
}

modules::CompileResult MsPascal::compile(ByteView source, const modules::CompileOptions& options) const {
    (void)options;   // (the headers, include folders and switches mean nothing to this compiler)
    const pascal_join::CompileResult r = compile(source, std::string_view("T.PAS"));
    return {r.object, r.console};
}

modules::CompileResult MsPascal::run_passes(const Bytes& source, const std::string& file_name) const {
    CB_ASSERT(ctx_.runner != nullptr, "a toolchain needs a runner to run its passes");
    const stdfs::path k = kit();
    const fs::ScopedTempDir work("pas");   // gone when this returns, whatever happens
    for (const char* f : {"PAS1.EXE", "PAS2.EXE", "PASKEY"}) {
        const stdfs::path from = stdfs::exists(k / f) ? k / f : k / "PAS1EM.EXE";   // (3.11: PAS1EM and PAS187)
        if (stdfs::exists(from)) stdfs::copy_file(from, work.path() / f, stdfs::copy_options::overwrite_existing);
    }
    fs::write_file(work.path() / file_name, source);
    const std::string stem = stdfs::path(file_name).stem().string();
    const auto run = [&](const char* program, const std::string& tail) {
        toolchain::PassRun pass;
        pass.exe = work.path() / program;
        pass.workdir = work.path();
        pass.command_tail = tail.empty() ? std::string() : " " + tail;   // (the PSP's command tail begins with a blank)
        pass.environment = to_bytes(std::string("PATH=\0", 6));
        pass.fast = true;   // no hook on every basic block: the blocks are not wanted here
        return ctx_.runner->run(pass);
    };
    const emu::dos::Result r1 = run("PAS1.EXE", std::format("{},T,T,T;", file_name != "T.PAS" ? stem : std::string("T")));
    std::string console = text::from_latin1(r1.console);
    if (!pas_no_errors(console)) {
        const stdfs::path listing = work.path() / "T.LST";
        const std::string errs = stdfs::is_regular_file(listing) ? text::from_latin1(fs::read_file(listing)) : std::string();
        std::string shown;
        for (const std::string_view line : text::split(text::replace_all(errs, "\r", ""), '\n')) {
            if (line.find('^') == std::string_view::npos && line.find("Error") == std::string_view::npos) continue;
            shown += (shown.empty() ? "" : "\n") + std::string(line);
        }
        return {std::nullopt, console + "\n" + pas_prefix(shown, 1500)};
    }
    console += text::from_latin1(run("PAS2.EXE", "").console);
    const stdfs::path object = work.path() / "T.OBJ";
    if (!stdfs::is_regular_file(object)) return {std::nullopt, console};
    const Bytes obj = fs::read_file(object);
    const stdfs::path cod = work.path() / "T.COD";
    const std::string listing = stdfs::is_regular_file(cod) ? text::from_latin1(fs::read_file(cod)) : std::string();
    return {name_procedures(obj, listing, text::from_latin1(source)), console};
}

void register_plugin(toolchain::Toolchains& toolchains, Hooks hooks) {
    toolchains.register_plugin(std::string(kPluginName), [hooks](const toolchain::Profile& p, const std::optional<std::string>& variant, const toolchain::Context& c) {
        return std::make_shared<MsPascal>(p, variant, c, hooks);
    });
}

}  // namespace cb::exact::mspascal
