// build/module_tools.cpp -- the linker, the assembler and the small things the module builder shares.

#include <algorithm>
#include <cstdlib>
#include <set>

#include "base/cache.hpp"
#include "build/module_internal.hpp"
#include "core/fs.hpp"
#include "core/process.hpp"
#include "core/regex.hpp"
#include "core/text.hpp"
#include "formats/detect.hpp"
#include "formats/omf.hpp"

namespace cb::build::module::internal {

namespace {

bool is_word_byte(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

bool all_digits(std::string_view s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), [](char c) { return c >= '0' && c <= '9'; });
}

std::string decoded(const Bytes& bytes) { return text::sanitize_utf8(as_text(bytes)); }

}  // namespace

// ---- small things ---------------------------------------------------------------------------------------------------------

std::string tail_of(std::string_view s, std::size_t n) { return std::string(s.size() > n ? s.substr(s.size() - n) : s); }

std::string trimmed_tail(std::string_view s, std::size_t n) { return tail_of(text::trim(s), n); }

std::string_view after_brace(std::string_view text) {
    const std::size_t at = text.find('{');
    return at == std::string_view::npos ? text : text.substr(at + 1);
}

std::optional<std::string> env_value(const char* name) { return fs::get_env(name); }

bool env_is_set(const char* name) {
    const std::optional<std::string> v = fs::get_env(name);
    return v && !v->empty();
}

std::string env_text(const char* name) { return fs::get_env(name).value_or(""); }

std::string quote_symbol(std::string_view symbol) { return "'" + std::string(symbol) + "'"; }

std::string read_text_lossy(const stdfs::path& file) { return text::sanitize_utf8(fs::read_text(file)); }

bool has_identifier(std::string_view text, std::string_view word) {
    for (std::size_t at = text.find(word); at != std::string_view::npos; at = text.find(word, at + 1)) {
        const bool before = at == 0 || !is_word_byte(text[at - 1]);
        const std::size_t end = at + word.size();
        const bool after = end >= text.size() || !is_word_byte(text[end]);
        if (before && after) return true;
    }
    return false;
}

bool reads_code_data(std::string_view text, std::int64_t segment) { return has_identifier(text, "codebreaker_code_" + std::to_string(segment)); }

// ---- running the tools ----------------------------------------------------------------------------------------------------

ToolRun run_tool(const stdfs::path& exe, std::vector<std::string> args, const stdfs::path& cwd, const Env& env,
                 std::chrono::milliseconds timeout) {
    process::ProcessSpec spec;
    spec.argv.push_back(fs::to_text(exe));
    spec.argv.insert(spec.argv.end(), std::make_move_iterator(args.begin()), std::make_move_iterator(args.end()));
    spec.cwd = cwd;
    spec.env = env;
    spec.timeout = timeout;
    const process::ProcessResult r = process::run(spec);
    return {r.exit_code, decoded(r.out), decoded(r.err)};
}

Linked link_image(const stdfs::path& linker, const Env& env, const stdfs::path& folder, const std::vector<std::string>& lines) {
    fs::write_text(folder / "m.lnk", text::join(lines, "\n") + "\n");
    for (const char* stale : {"m.dll", "m.map"}) {
        std::error_code ec;
        stdfs::remove(folder / stale, ec);
    }
    const ToolRun r = run_tool(linker, {"@m.lnk"}, folder, env);
    const stdfs::path out = folder / "m.dll";
    std::error_code ec;
    return {stdfs::is_regular_file(out, ec) ? std::optional<stdfs::path>(out) : std::nullopt, r.said()};
}

namespace {

// The names the linker says it cannot find, sorted and each once.
std::vector<std::string> undefined_in(std::string_view log) {
    static const Regex undefined(R"((\S+) is an undefined reference)");
    std::set<std::string> found;
    undefined.for_each_match(log, [&](const RegexMatch& m) {
        found.emplace(m.group(1));
        return true;
    });
    return {found.begin(), found.end()};
}

}  // namespace

std::optional<stdfs::path> link_resolving(Built& b, const stdfs::path& linker, const Env& env, const stdfs::path& folder,
                                          std::vector<std::string> lines,
                                          const std::function<std::optional<std::string>(const std::string&)>& stand_in,
                                          const std::function<std::string(const std::string&, const std::string&)>& compile_extra) {
    Linked linked = link_image(linker, env, folder, lines);
    std::vector<std::string> names = undefined_in(linked.log);
    if (!names.empty()) {
        // `__imp__Name@N` is asked for by the compiler's own runtime library, pulled in by a library routine the functions use: it
        // needs those system functions for itself. They come from the import libraries, and show up in the check as imports the
        // original does not have.
        const bool imp = std::any_of(names.begin(), names.end(), [](const std::string& s) { return s.starts_with("__imp_"); });
        if (imp) {
            lines.push_back("library kernel32.lib, user32.lib, gdi32.lib, advapi32.lib");
            std::erase_if(names, [](const std::string& s) { return s.starts_with("__imp_"); });
        }
        std::vector<std::pair<std::string, std::string>> texts;
        for (const std::string& s : names) {
            const std::optional<std::string> t = stand_in(s);
            if (t && !t->empty()) texts.emplace_back(s, *t);
        }
        std::vector<std::string> objects;
        std::vector<std::string> all_texts;
        for (const auto& st : texts) all_texts.push_back(st.second);
        if (!texts.empty() && compile_extra("absent", text::join(all_texts, "\n")).empty()) {
            objects = {"absent.obj"};
            b.absent.clear();
            for (const auto& st : texts) b.absent.push_back(st.first);
        } else {   // one of them does not compile: each on its own
            for (std::size_t i = 0; i < texts.size(); ++i) {
                const std::string stem = std::format("absent{:03d}", i);
                if (compile_extra(stem, texts[i].second).empty()) {
                    objects.push_back(stem + ".obj");
                    b.absent.push_back(texts[i].first);
                }
            }
        }
        std::vector<std::string> plain, libs;
        for (const std::string& ln : lines) (ln.starts_with("library ") ? libs : plain).push_back(ln);
        lines = plain;
        for (const std::string& o : objects) lines.push_back("file " + o);
        lines.insert(lines.end(), libs.begin(), libs.end());
        linked = link_image(linker, env, folder, lines);
        b.undefined = undefined_in(linked.log);
        if (!b.undefined.empty()) {
            std::vector<std::string> forgiving = lines;
            forgiving.push_back("option undefsok");
            linked = link_image(linker, env, folder, forgiving);
        }
    }
    if (!linked.image) {
        const std::string said = trimmed_tail(linked.log, 400);
        b.error = "link: " + (!said.empty() ? said : "the linker produced nothing");
    }
    return linked.image;
}

std::optional<std::string> stand_in_from_header(std::string_view header, std::string_view symbol) {
    const std::string name = functions::plain_symbol(symbol);
    if (!text::is_identifier(name)) return std::nullopt;
    // (one pattern for each name the link could not find: a handful in a build)
    const Regex declaration("^(?:extern\\s+)?([^;{}#\\n]*\\b" + name + "\\s*\\([^;{}\\n]*\\))\\s*;", RegexFlags::Multiline);
    if (const auto m = declaration.search(header)) return std::format("{}\n{{\n  return 0;\n}}\n", m->group(1));
    // _name@N: __stdcall with N bytes of arguments
    if (symbol.size() > 3 && symbol[0] == '_') {
        const std::size_t at = symbol.find('@');
        if (at != std::string_view::npos && at >= 2 && all_digits(symbol.substr(at + 1)) &&
            std::all_of(symbol.begin() + 1, symbol.begin() + static_cast<std::ptrdiff_t>(at), is_word_byte)) {
            const std::size_t words = text::parse_int<std::size_t>(symbol.substr(at + 1)).value_or(0) / 4;
            std::vector<std::string> params;
            for (std::size_t i = 0; i < words; ++i) params.push_back(std::format("int a{}", i));
            return std::format("int __stdcall {}({})\n{{\n  return 0;\n}}\n", name, params.empty() ? "void" : text::join(params, ", "));
        }
    }
    return std::format("int {}()\n{{\n  return 0;\n}}\n", name);
}

std::optional<std::string> write_dos_stub(const stdfs::path& original, const stdfs::path& folder) {
    const Bytes file = fs::read_file(original);
    const std::optional<formats::detect::DosHeader> h = formats::detect::dos_header(ByteView(file).first(std::min<std::size_t>(file.size(), 0x40)));
    if (!h) return std::nullopt;
    if (h->new_header > file.size()) return std::nullopt;
    Bytes stub(file.begin(), file.begin() + h->new_header);
    if (stub.size() < 0x40) return std::nullopt;
    const std::uint16_t last = static_cast<std::uint16_t>(stub.size() % 512), pages = static_cast<std::uint16_t>((stub.size() + 511) / 512);
    stub[2] = static_cast<std::uint8_t>(last & 0xFF);
    stub[3] = static_cast<std::uint8_t>(last >> 8);
    stub[4] = static_cast<std::uint8_t>(pages & 0xFF);
    stub[5] = static_cast<std::uint8_t>(pages >> 8);
    fs::write_file(folder / "stub.exe", stub);
    return "stub.exe";
}

// ---- the assembler -------------------------------------------------------------------------------------------------------

std::string Assembler::tool_inputs() const {
    return services_.builder == nullptr ? std::string() : services_.builder->tool_inputs({exe_, env_}, "WASM");
}

Assembler::Result Assembler::assemble(const std::string& stem, std::string_view text, const std::vector<std::string>& flags) const {
    fs::write_text(folder_ / (stem + ".asm"), text);
    const stdfs::path obj = folder_ / (stem + ".obj");
    // (every input of the object, in the shared build cache: the assembler by its bytes, the INCLUDE files it may read and the WASM
    // variable of options, the options, the text, and the stem -- the object names its source file, stem.asm, and the messages of a
    // failure name it too)
    std::string key;
    if (services_.cache != nullptr) {
        key = base::cache::key({"wasm16 cb 1", tool_inputs(), flags, stem, text});
        if (const std::optional<Bytes> had = services_.cache->get("wasm16", key); had && had->size() >= 2) {
            const ByteView rest = ByteView(*had).subspan(2);
            if ((*had)[0] == 'O' && (*had)[1] == 'K') {
                // (as wasm writes it in this folder, now)
                if (const std::optional<Bytes> here = functions::omf_here(rest, obj.parent_path())) {
                    fs::write_file(obj, *here);
                    note(stem, text, flags);
                    return {0, ""};
                }
            } else {
                return {1, functions::message_here(rest, folder_)};
            }
        }
    }
    {
        std::error_code ec;
        stdfs::remove(obj, ec);
    }
    std::vector<std::string> args = flags;
    args.push_back("-fo=" + stem + ".obj");
    args.push_back(stem + ".asm");
    const ToolRun r = run_tool(exe_, std::move(args), folder_, env_);
    const std::string message = r.said();
    std::error_code ec;
    if (r.code == 0 && stdfs::is_regular_file(obj, ec)) {
        note(stem, text, flags);
        if (services_.cache != nullptr) {
            Bytes kept = {'O', 'K'};
            const Bytes object = fs::read_file(obj);
            kept.insert(kept.end(), object.begin(), object.end());
            services_.cache->put("wasm16", key, kept);
        }
        return {0, message};
    }
    // (a failure that says nothing is the assembler stopped from outside -- a build interrupted -- not the text: kept, every later
    // build of the module found that piece "not assembling" and gave up its whole frame, USER's segment 1 with all its C)
    if (services_.cache != nullptr && !text::trim(message).empty()) {
        Bytes kept = {'E', 'R'};
        const Bytes said = functions::message_kept(message, folder_);
        kept.insert(kept.end(), said.begin(), said.end());
        services_.cache->put("wasm16", key, kept);
    }
    return {r.code != 0 ? r.code : 1, message};
}

void Assembler::note(const std::string& stem, std::string_view text, const std::vector<std::string>& flags) const {
    if (services_.builder != nullptr)
        services_.builder->note_made(folder_ / (stem + ".obj"), {fs::to_text(exe_.stem()), flags, stem + ".asm", std::string(text), folder_});
}

std::string write_filler(const Assembler& assembler, std::uint32_t seg, std::string_view stem, std::uint32_t n) {
    const std::string name = "asm/" + std::string(stem);
    assembler.assemble(name, std::format("CODE{} segment byte public 'MCODE'\n        db   {} dup (0CCh)\nCODE{} ends\nend\n", seg, n, seg),
                       {"-q"});
    return name;
}

// ---- what an object defines ----------------------------------------------------------------------------------------------

std::map<std::string, PublicSymbol> publics_of(const Services& services, const stdfs::path& folder,
                                               const std::map<std::string, std::string>& stems) {
    std::map<std::string, PublicSymbol> out;
    for (const auto& [name, stem] : stems) {
        Bytes body;
        try {
            body = fs::read_file(folder / (stem + ".obj"));
        } catch (const IoError&) {
            continue;   // (no object: it defines nothing)
        }
        // (an object's publics: read once, kept by its bytes and the reader)
        const std::string key = base::cache::key({body, "build/module omf publics 1"});
        std::vector<std::string> symbols;
        std::optional<Bytes> had;
        if (services.cache != nullptr) had = services.cache->get("omf_publics", key);
        if (!had) {
            symbols = formats::omf::publics(body);
            std::sort(symbols.begin(), symbols.end());
            symbols.erase(std::unique(symbols.begin(), symbols.end()), symbols.end());
            if (services.cache != nullptr) {
                const std::string joined = text::join(symbols, "\n");
                services.cache->put("omf_publics", key, ByteView(reinterpret_cast<const std::uint8_t*>(joined.data()), joined.size()));
            }
        } else {
            for (std::string_view s : text::split(as_text(*had), '\n'))
                if (!s.empty()) symbols.emplace_back(s);
        }
        const auto is_decorated = [&](const std::string& s) {   // [_@]NAME@\d+
            return s.size() > name.size() + 2 && (s[0] == '_' || s[0] == '@') && s.compare(1, name.size(), name) == 0 &&
                   s[1 + name.size()] == '@' && all_digits(std::string_view(s).substr(2 + name.size()));
        };
        for (const std::string& s : symbols) {
            if (s == "_" + name || s == name || s == name + "_" || is_decorated(s)) {
                out[name] = {s, stem + ".obj"};
                break;
            }
        }
    }
    return out;
}

}  // namespace cb::build::module::internal
