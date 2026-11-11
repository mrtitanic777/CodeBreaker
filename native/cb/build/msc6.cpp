#include "cb/build/msc6.hpp"

#include <algorithm>
#include <array>
#include <cstdlib>
#include <format>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/process.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/omf.hpp"

namespace cb::build::msc6 {

namespace stdfs = std::filesystem;

namespace {

// Bump when what is kept under the "msc6" cache kind changes shape.
constexpr std::string_view kCacheKind = "msc6";
constexpr std::string_view kCacheGeneration = "msc6-1";
constexpr std::uint32_t kCacheSchema = 1;

constexpr std::string_view kFlagSets[] = {"/AM /Gcsw /Os",  "/AM /Gcsw /Ox", "/AS /Gcsw /Os",  "/AL /Gcsw /Os", "/AM /Gcsw /Od",
                                          "/AM /Gc2sw /Os", "/AM /Gc2sw /Ox", "/AM /Gcs /Os",   "/AS /Gcs /Os"};   // (/G2: SUWIN has push imm and leave)

// OMF record types.
constexpr std::uint8_t kModEnd = 0x8A, kModEnd32 = 0x8B, kFixupp = 0x9C, kFixupp32 = 0x9D, kLeData = 0xA0, kLeData32 = 0xA1;

// How many bytes of the place a fix-up's location field covers (a location the table does not know is a word).
std::size_t fixup_location_width(unsigned loc) {
    switch (loc) {
        case 0: return 1;
        case 1: return 2;
        case 2: return 2;
        case 3: return 4;
        case 4: return 1;
        case 5: return 2;
        case 9: return 4;
        case 11: return 6;
        case 13: return 4;
        default: return 2;
    }
}

// An index of an OMF record: one byte, or two when the first has its high bit.
void skip_index(io::ByteReader& r) {
    const std::uint8_t first = r.u8();
    if (first & 0x80) r.u8();
}

std::optional<Bytes> read_optional(const stdfs::path& path) {
    std::error_code ec;
    if (!stdfs::is_regular_file(path, ec)) return std::nullopt;
    return fs::read_file(path);
}

// A C file as DOS reads it: Latin-1, CR LF line ends.
void write_dos_text(const stdfs::path& path, std::string_view text) {
    fs::write_file(path, text::to_latin1(text::replace_all(text, "\n", "\r\n")));
}

// Files of an earlier session in a work folder go (the objects, the sources, the messages).
void clear_session_files(const stdfs::path& folder, bool headers_too) {
    std::error_code ec;
    for (const auto& entry : stdfs::directory_iterator(folder, ec)) {
        if (!entry.is_regular_file(ec)) continue;
        const std::string name = text::lower(entry.path().filename().string());
        if (name.ends_with(".obj") || name.ends_with(".c") || name.ends_with(".txt") || (headers_too && name.ends_with(".h"))) {
            stdfs::remove(entry.path(), ec);
        }
    }
}

// The names of the variables of our environment: a native session starts the compiler with only the few it needs.
std::vector<std::string> inherited_variable_names() {
    std::vector<std::string> names;
    for (char** e = _environ; e && *e; ++e) {
        const std::string_view entry(*e);
        const std::size_t eq = entry.find('=', 1);   // (the drive-current-directory variables begin with '=')
        if (eq != std::string_view::npos && !entry.starts_with('=')) names.emplace_back(entry.substr(0, eq));
    }
    return names;
}

std::string system_root() {
    if (const auto root = fs::get_env("SystemRoot")) return *root;
    return "C:\\Windows";
}

// `files` compiled by the compiler run on Windows itself, one CL a file, in a short work folder; the objects and messages
// copied back into `work` as an emulator session leaves them.
bool native_session(const stdfs::path& work, std::string_view header, const std::vector<BatchFiles::File>& files, std::string_view flags,
                    const NativePaths& native, std::chrono::seconds timeout) {
    const crypto::Sha1Digest digest = crypto::sha1(as_bytes(stdfs::absolute(work).generic_string()));
    const stdfs::path short_work = native.work / crypto::to_hex(digest).substr(0, 8);
    for (const stdfs::path& folder : {work, short_work}) {
        fs::make_dirs(folder);
        clear_session_files(folder, true);
    }
    write_dos_text(short_work / "M.H", header);
    process::ProcessSpec spec;
    spec.cwd = short_work;
    spec.env_remove = inherited_variable_names();
    spec.env = {{"PATH", (native.tools / "BIN").string()}, {"INCLUDE", (native.tools / "INCLUDE").string()},
                {"TMP", short_work.string()}, {"TEMP", short_work.string()}, {"SystemRoot", system_root()}};
    spec.timeout = std::chrono::duration_cast<std::chrono::milliseconds>(timeout);
    for (const BatchFiles::File& file : files) {
        write_dos_text(short_work / (file.stem + ".C"), "#include \"m.h\"\n\n" + file.text + "\n");
        spec.argv = {(native.tools / "BIN" / "CL.EXE").string(), "/nologo", "/c", "/W0"};
        for (const std::string_view flag : text::split_ws(flags)) spec.argv.emplace_back(flag);
        spec.argv.push_back("/Fo" + file.stem + ".OBJ");
        spec.argv.push_back(file.stem + ".C");
        Bytes said;
        const process::ProcessResult r = process::run(spec);
        if (r.timed_out) {
            said = to_bytes("(timed out)");
        } else {
            said = r.out;
            append(said, r.err);
        }
        fs::write_file(work / (file.stem + ".TXT"), said);
        if (const auto obj = read_optional(short_work / (file.stem + ".OBJ"))) fs::write_file(work / (file.stem + ".OBJ"), *obj);
    }
    return true;
}

// One emulator session compiling `files` -> whether it ran to its end. A compiler that runs on Windows itself (native_paths)
// is run so instead -- no emulator.
bool run_session(const Context& ctx, const stdfs::path& work, std::string_view header, const std::vector<BatchFiles::File>& files,
                 std::string_view flags, Compiler compiler, std::chrono::seconds timeout) {
    if (const auto native = native_paths(ctx.tools, compiler)) return native_session(work, header, files, flags, *native, timeout);
    fs::make_dirs(work);
    clear_session_files(work, false);
    write_dos_text(work / "M.H", header);
    DosRunSpec spec;
    for (const BatchFiles::File& file : files) {
        write_dos_text(work / (file.stem + ".C"), "#include \"m.h\"\n\n" + file.text + "\n");
        spec.commands.push_back(std::format("cl /nologo /c /W0 {} /Fo{}.OBJ {}.C > {}.TXT", flags, file.stem, file.stem, file.stem));
    }
    spec.mounts = {{"c", ctx.tools.home(compiler)}, {"d", work}};
    spec.timeout = timeout;
    spec.work = work;
    spec.cpu = {{"core", "dynamic"}, {"cputype", "386"}, {"cycles", "max"}};
    spec.variables = {{"PATH", "C:\\BIN"}, {"INCLUDE", "C:\\INCLUDE"}, {"LIB", "C:\\LIB"}};
    ctx.runner.run(spec);
    std::error_code ec;
    return stdfs::is_regular_file(work / "done.txt", ec);
}

// The code a compiler's object file holds: the public function's, or (no name) the first code segment's. An object that cannot be read
// is that source's verdict -- no code -- not the run's: this is the isolation boundary of one function of a batch.
std::optional<Code> code_of(ByteView object, const std::optional<std::string>& function) {
    try {
        return function ? function_code(object, *function) : std::optional<Code>(object_code(object));
    } catch (const FormatError&) {
        return std::nullopt;
    }
}

// What the cache holds of a compile: 4 bytes of the code's length, the code, the mask -- or one '-' for "the compiler refused it".
Bytes pack_code(const Code& c) {
    Bytes out;
    const std::uint32_t n = static_cast<std::uint32_t>(c.code.size());
    for (int i = 0; i < 4; ++i) out.push_back(static_cast<std::uint8_t>(n >> (8 * i)));
    append(out, c.code);
    append(out, c.mask);
    return out;
}

// nullopt: the entry is damaged (a miss). Otherwise the answer, which may be "refused".
std::optional<std::optional<Code>> unpack_code(ByteView had) {
    if (had.size() == 1 && had[0] == '-') return std::optional<Code>{};
    if (had.size() < 4) return std::nullopt;
    const std::size_t n = had[0] | (had[1] << 8) | (had[2] << 16) | (static_cast<std::size_t>(had[3]) << 24);
    if (4 + n > had.size()) return std::nullopt;
    return std::optional<Code>{Code{Bytes(had.begin() + 4, had.begin() + 4 + n), Bytes(had.begin() + 4 + n, had.end())}};
}

}  // namespace

// ---- tools --------------------------------------------------------------------------------------------------------------

bool available(const Tools& tools, Compiler compiler) {
    std::error_code ec;
    const stdfs::path& home = tools.home(compiler);
    return !tools.dosbox_x.empty() && stdfs::is_regular_file(tools.dosbox_x, ec) && !home.empty() &&
           stdfs::is_regular_file(home / "BIN" / "CL.EXE", ec);
}

std::span<const std::string_view> flag_sets() { return kFlagSets; }

std::optional<std::string> windows_h(const Tools& tools) {
    if (tools.msvc152.empty()) return std::nullopt;
    const stdfs::path path = tools.msvc152 / "INCLUDE" / "WINDOWS.H";
    std::error_code ec;
    if (!stdfs::is_regular_file(path, ec)) return std::nullopt;
    return text::replace_all(text::from_latin1(fs::read_file(path)), "\r\n", "\n");
}

std::optional<NativePaths> native_paths(const Tools& tools, Compiler compiler, bool make_junction) {
    if (compiler != Compiler::Msvc152 || !tools.native16) return std::nullopt;
    std::error_code ec;
    if (tools.msvc152.empty() || !stdfs::is_regular_file(tools.msvc152 / "BIN" / "CL.EXE", ec)) return std::nullopt;
    const stdfs::path profile = fs::from_text(fs::get_env("USERPROFILE").value_or(fs::get_env("HOME").value_or(".")));
    const stdfs::path short_tools = tools.native16_tools.value_or(profile / ".cbt");
    const stdfs::path root = tools.native16_work.value_or(profile / ".cbw");
    if (!stdfs::is_directory(short_tools, ec) && make_junction) {
        process::run({.argv = {"cmd", "/c", "mklink", "/J", short_tools.string(), stdfs::absolute(tools.msvc152).string()},
                      .out = process::Capture::Discard,
                      .err = process::Capture::Discard});
    }
    fs::make_dirs(root);
    if (!stdfs::is_regular_file(short_tools / "BIN" / "CL.EXE", ec)) return std::nullopt;
    return NativePaths{short_tools, root};
}

// ---- object files ---------------------------------------------------------------------------------------------------

Code object_code(ByteView obj) {
    Code out;
    std::optional<std::size_t> last;   // where the data of the last record of the first segment begins in `code`
    io::ByteReader file(obj, "omf object");
    while (file.remaining() >= 3) {
        const std::uint8_t kind = file.u8();
        const std::uint16_t size = file.u16();
        const bool data_record = kind == kLeData || kind == kLeData32 || kind == kFixupp || kind == kFixupp32;
        if (size == 0) {   // (no body, not even a checksum: nothing to read in it)
            if (kind == kModEnd || kind == kModEnd32) break;
            continue;
        }
        // A record that runs past the end of the file is damaged: for one that holds data it is an error, for any other the scan
        // ends here (it is the end of what the file says).
        if (size > file.remaining() && !data_record) break;
        io::ByteReader body(file.bytes(size), "omf record");   // (the record's last byte is its checksum)
        if (kind == kLeData || kind == kLeData32) {            // LEDATA: segment, offset, bytes
            std::uint32_t seg = body.u8();
            if (seg & 0x80) seg = ((seg & 0x7F) << 8) | body.u8();
            const std::size_t off = kind == kLeData32 ? body.u32() : body.u16();
            if (seg != 1) {
                last.reset();
                continue;
            }
            const ByteView data = body.bytes(body.remaining() - 1);
            if (out.code.size() < off + data.size()) {
                out.code.resize(off + data.size(), 0);
                out.mask.resize(off + data.size(), 0);
            }
            std::copy(data.begin(), data.end(), out.code.begin() + static_cast<std::ptrdiff_t>(off));
            std::fill_n(out.mask.begin() + static_cast<std::ptrdiff_t>(off), data.size(), std::uint8_t{0});
            last = off;
        } else if ((kind == kFixupp || kind == kFixupp32) && last) {   // FIXUPP: the places in the record before it
            const std::size_t end = body.size() - 1;
            while (body.pos() < end) {
                const std::uint8_t b = body.u8();
                if (!(b & 0x80)) {   // a thread: one byte, and an index for most methods
                    if (((b >> 2) & 7) < 4 && body.pos() < end) skip_index(body);
                    continue;
                }
                const unsigned where = ((b & 3u) << 8) | body.u8();
                const unsigned loc = (b >> 2) & 0xF;
                const std::uint8_t fix = body.u8();
                if (!(fix & 0x80) && ((fix >> 4) & 7) < 3) skip_index(body);   // the frame is given here
                if (!(fix & 0x08)) skip_index(body);                          // the target is given here
                if (!(fix & 0x04)) body.skip(kind == kFixupp32 ? 4 : 2);       // with a displacement
                const std::size_t at = *last + where;
                const std::size_t n = fixup_location_width(loc);
                for (std::size_t i = at; i < std::min(at + n, out.mask.size()); ++i) out.mask[i] = 1;
            }
        } else if (kind == kModEnd || kind == kModEnd32) {
            break;
        }
    }
    return out;
}

std::optional<Code> function_code(ByteView obj, std::string_view name) {
    const auto stripped = [](std::string_view s) {
        while (!s.empty() && (s.front() == '_' || s.front() == '@')) s.remove_prefix(1);
        return text::lower(s);
    };
    const std::string want = stripped(name);   // (a C name _f, a fastcall one @f)
    for (const auto& [pub, function] : formats::omf::function_bytes(obj)) {
        if (stripped(pub) != want) continue;
        Code out{function.code, Bytes(function.code.size(), 0)};
        for (const std::uint64_t at : function.fixup_mask) {
            if (at < out.mask.size()) out.mask[at] = 1;
        }
        return out;
    }
    return std::nullopt;
}

// ---- compiling ------------------------------------------------------------------------------------------------------

BatchFiles batch_files(std::span<const std::vector<std::size_t>> groups, const std::map<std::size_t, std::string>& todo, std::string_view prefix) {
    BatchFiles out;
    for (std::size_t i = 0; i < groups.size(); ++i) {
        std::size_t at = 3;   // (the file starts with the #include and a blank line)
        std::vector<std::pair<std::size_t, std::size_t>> here;
        std::vector<std::string_view> parts;
        for (const std::size_t k : groups[i]) {
            here.emplace_back(at, k);
            const std::string& text = todo.at(k);
            at += static_cast<std::size_t>(std::count(text.begin(), text.end(), '\n')) + 2;   // (its lines, and the blank one between)
            parts.push_back(text);
        }
        out.files.push_back({std::format("{}{}", prefix, i), text::join(parts, "\n\n")});
        out.starts.push_back(std::move(here));
    }
    return out;
}

std::vector<std::size_t> refused(std::string_view compiler_text, std::span<const std::pair<std::size_t, std::size_t>> starts) {
    static const Regex error_line(R"(^(\S+)\((\d+)\) : (?:fatal )?error)", RegexFlags::Multiline);
    std::vector<std::size_t> bad;
    bool outside = false;
    error_line.for_each_match(compiler_text, [&](const RegexMatch& m) {
        // the file the error is in: B<n>.C or R<n>.C (the batch files), not the header
        std::string_view file = m.group(1);
        const std::size_t slash = file.find_last_of("/\\");
        if (slash != std::string_view::npos) file.remove_prefix(slash + 1);
        const bool ours = file.size() >= 4 && (file[0] == 'B' || file[0] == 'R' || file[0] == 'b' || file[0] == 'r') &&
                          text::iequals(file.substr(file.size() - 2), ".C") &&
                          std::all_of(file.begin() + 1, file.end() - 2, [](char c) { return c >= '0' && c <= '9'; }) && file.size() > 3;
        if (!ours) {
            outside = true;
            return false;
        }
        const std::size_t line = text::parse_int<std::size_t>(m.group(2)).value_or(0);
        const std::pair<std::size_t, std::size_t>* best = nullptr;
        for (const auto& s : starts) {
            if (s.first <= line && (!best || s.first > best->first)) best = &s;
        }
        if (best && std::find(bad.begin(), bad.end(), best->second) == bad.end()) bad.push_back(best->second);
        return true;
    });
    return outside ? std::vector<std::size_t>{} : bad;
}

std::vector<std::optional<Code>> compile_many(const Context& ctx, std::span<const Source> sources, std::string_view header,
                                              std::string_view flags, const CompileOptions& options) {
    if (!available(ctx.tools, options.compiler)) {
        throw NotFoundError(ctx.tools.home(options.compiler), 2,
                            std::format("{} is not set up (settings.json: dosbox_x, {})",
                                        options.compiler == Compiler::Msc6 ? "msc6" : "msvc152",
                                        options.compiler == Compiler::Msc6 ? "msc6" : "msvc152"));
    }
    const std::size_t count = sources.size();
    std::vector<std::optional<Code>> out(count);
    std::vector<bool> known(count, false);   // a cached answer (the compiler's code, or "refused") is in `out`
    std::map<std::size_t, std::pair<std::string, std::string>> todo;   // source -> (cache key, text)

    std::string stamp;   // (what the compiler is, by its bytes)
    if (ctx.cache) {
        ctx.cache->declare_kind(kCacheKind, {.scope = base::cache::Scope::Shared, .generation = std::string(kCacheGeneration)});
        stamp = ctx.cache->tool_bytes_id(ctx.tools.home(options.compiler) / "BIN" / "CL.EXE");
    }
    std::vector<std::size_t> named;
    for (std::size_t i = 0; i < count; ++i) {
        if (sources[i].function) named.push_back(i);
    }
    const bool any_named = !named.empty();
    std::vector<std::vector<std::size_t>> files_of;
    if (any_named && options.batch > 1) {
        if (!options.together.empty()) {
            for (const auto& g : options.together) {
                std::vector<std::size_t> in;
                for (const std::size_t k : g) {
                    if (k < count) in.push_back(k);
                }
                files_of.push_back(std::move(in));
            }
        } else {
            for (std::size_t i = 0; i < named.size(); i += options.batch) {
                files_of.emplace_back(named.begin() + static_cast<std::ptrdiff_t>(i),
                                      named.begin() + static_cast<std::ptrdiff_t>(std::min(named.size(), i + options.batch)));
            }
        }
        std::set<std::size_t> seen;
        for (const auto& g : files_of) seen.insert(g.begin(), g.end());
        std::erase_if(files_of, [](const auto& g) { return g.empty(); });
        for (const std::size_t k : named) {
            if (!seen.contains(k)) files_of.push_back({k});
        }
    } else {
        for (const std::size_t k : named) files_of.push_back({k});
    }
    std::map<std::size_t, std::string> context;   // source -> the key of its whole file (when it shares one)
    for (const auto& g : files_of) {
        std::string whole;
        if (g.size() > 1) {
            std::vector<base::cache::KeyPart> parts;
            for (const std::size_t k : g) parts.emplace_back(sources[k].text);
            whole = base::cache::key(parts);
        }
        for (const std::size_t k : g) context[k] = whole;
    }
    for (std::size_t i = 0; i < count; ++i) {
        std::string k;
        if (ctx.cache) {
            const auto ctxt = context.find(i);
            k = base::cache::key({stamp, flags, header, sources[i].text, sources[i].function ? "function " + *sources[i].function : std::string(),
                                  ctxt == context.end() ? std::string() : ctxt->second});
            if (const auto had = ctx.cache->get(kCacheKind, k, kCacheSchema)) {
                if (auto answer = unpack_code(*had)) {
                    out[i] = std::move(*answer);
                    known[i] = true;
                }
            }
        }
        todo[i] = {k, sources[i].text};
    }
    for (const auto& g : files_of) {   // (a file not wholly known is compiled again whole)
        if (std::all_of(g.begin(), g.end(), [&](std::size_t k) { return known[k]; })) {
            for (const std::size_t k : g) todo.erase(k);
        }
    }
    for (auto it = todo.begin(); it != todo.end();) {
        it = (!context.contains(it->first) && known[it->first]) ? todo.erase(it) : std::next(it);
    }
    if (todo.empty()) return out;

    std::vector<std::vector<std::size_t>> keep_files;
    for (const auto& g : files_of) {
        std::vector<std::size_t> kept;
        for (const std::size_t k : g) {
            if (todo.contains(k)) kept.push_back(k);
        }
        if (!kept.empty()) keep_files.push_back(std::move(kept));
    }
    const stdfs::path work = stdfs::absolute(options.work);
    std::map<std::size_t, std::optional<Code>> built;
    std::set<std::size_t> unsettled;   // no verdict -- the session was cut off -- so not cached
    std::vector<std::size_t> single;
    for (const auto& [k, entry] : todo) {
        if (!sources[k].function) single.push_back(k);
    }
    std::vector<std::vector<std::size_t>> groups = any_named ? keep_files : std::vector<std::vector<std::size_t>>{};
    // A refused file: the functions its errors are in go on their own, the rest together again -- round after round, for
    // the compiler stops at a file's first errors and a second bad function shows only once the first is out (one
    // emulator session a round).
    std::map<std::size_t, std::string> texts;
    for (const auto& [k, entry] : todo) texts[k] = entry.second;
    for (int round = 0; round < 6 && !groups.empty(); ++round) {
        const BatchFiles batch = batch_files(groups, texts, "B");
        const bool finished = run_session(ctx, work, header, batch.files, flags, options.compiler, options.timeout);
        std::vector<std::vector<std::size_t>> again;
        for (std::size_t i = 0; i < groups.size(); ++i) {
            const auto& g = groups[i];
            const std::optional<Bytes> obj = read_optional(work / std::format("B{}.OBJ", i));
            if (obj) {
                for (const std::size_t k : g) built[k] = code_of(*obj, sources[k].function);
            } else if (!finished) {
                unsettled.insert(g.begin(), g.end());
            } else if (g.size() == 1) {
                built[g[0]] = std::nullopt;   // (refused on its own: it does not compile)
            } else {
                const std::optional<Bytes> said = read_optional(work / std::format("B{}.TXT", i));
                const std::vector<std::size_t> bad = said ? refused(text::from_latin1(*said), batch.starts[i]) : std::vector<std::size_t>{};
                if (bad.empty()) {   // (errors not told apart: each on its own)
                    for (const std::size_t k : g) again.push_back({k});
                    continue;
                }
                for (const std::size_t k : bad) again.push_back({k});
                std::vector<std::size_t> rest;
                for (const std::size_t k : g) {
                    if (std::find(bad.begin(), bad.end(), k) == bad.end()) rest.push_back(k);
                }
                if (!rest.empty()) again.push_back(std::move(rest));
            }
        }
        groups = std::move(again);
    }
    for (const auto& g : groups) unsettled.insert(g.begin(), g.end());   // (still refused after every round: no verdict)
    if (!single.empty()) {
        std::vector<BatchFiles::File> files;
        for (std::size_t j = 0; j < single.size(); ++j) files.push_back({std::format("F{}", j), texts[single[j]]});
        const bool finished = run_session(ctx, work, header, files, flags, options.compiler, options.timeout);
        for (std::size_t j = 0; j < single.size(); ++j) {
            const std::size_t k = single[j];
            const std::optional<Bytes> obj = read_optional(work / std::format("F{}.OBJ", j));
            if (!obj) {
                built[k] = std::nullopt;
                if (!finished) unsettled.insert(k);
            } else {
                std::optional<Code> c = code_of(*obj, std::nullopt);
                built[k] = c && !c->code.empty() ? std::move(c) : std::nullopt;
            }
        }
    }
    for (const auto& [key, entry] : todo) {
        const auto got = built.find(key);
        out[key] = got == built.end() ? std::nullopt : got->second;
        if (!ctx.cache) continue;
        if (out[key]) {
            ctx.cache->put(kCacheKind, entry.first, pack_code(*out[key]), kCacheSchema);
        } else if (got != built.end() && !unsettled.contains(key)) {
            ctx.cache->put(kCacheKind, entry.first, to_bytes("-"), kCacheSchema);   // the compiler refused it (not: the session was cut off)
        }
    }
    return out;
}

}  // namespace cb::build::msc6
