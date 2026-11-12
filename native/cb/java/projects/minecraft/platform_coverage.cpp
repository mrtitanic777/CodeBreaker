#include "cb/java/projects/minecraft/platform_coverage.hpp"

#include <algorithm>
#include <format>
#include <system_error>

#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "cb/java/console/platform_equivalents.hpp"
#include "cb/java/textutil.hpp"

namespace cb::java::projects::minecraft::platform_coverage {

namespace stdfs = std::filesystem;
namespace pe = console::platform_equivalents;

std::span<const Subsystem> subsystems() {
    static const std::vector<Subsystem> all = {
        {
            .key = "video",
            .name = "Video  --  OpenGL (LWJGL) -> Direct3D 9",
            .holders = {"GL11", "GL12", "GL15", "GLU", "GLContext", "ARBOcclusionQuery", "ARBVertexBufferObject",
                        "BufferUtils"},
            .types = {"GLAllocation", "Tessellator"},
            .reference = {"GL11", "Shaders", "Mipmaps", "NIOBuffers", "ImageIO", "BufferedImage"},
            .options =
                {"A) REUSE the Alpha GL->D3D9 emulator (docs/platform/reference/GL11.reference.cpp) -- the GL API is "
                 "identical between Alpha and Beta, so this is the recommended path; only the MISSING symbols below "
                 "are new work.",
                 "B) Write a fresh Direct3D 9 backend from scratch.",
                 "C) Stub to no-ops (renders nothing / placeholder) -- for bring-up only."},
            .notes =
                {"HALF-PIXEL OFFSET (Direct3D 9). D3D9 maps screen coordinates to pixel CENTERS but texture samples "
                 "to the top-left CORNER, so any 2D/HUD/text drawn without a -0.5 screen-space shift lands half a "
                 "texel off -> blurry / smeared, MOST visible on font atlases and text. OpenGL (the source) has no "
                 "such offset, so the GL->D3D9 port introduces it. On the Xbox 360 the fix is the hardware render "
                 "state `D3DRS_HALFPIXELOFFSET = TRUE` (applied once at device setup -- see Main.reference.cpp) "
                 "rather than baking -0.5 into every GUI/font vertex. D3D10/11 removed the offset entirely; a D3D9 "
                 "backend on PC still needs the -0.5 vertex shift. VERIFY ON HARDWARE with the in-game font/GUI."},
        },
        {
            .key = "input",
            .name = "Input  --  controller / keyboard / mouse",
            .holders = {"Keyboard", "Mouse", "Controllers"},
            .types = {"MouseHelper", "MovementInput"},
            .reference = {"Input", "OSK"},
            .options = {"A) XInput2 controller mapping + on-screen keyboard (docs/platform/reference/Input, OSK).",
                        "B) Stub (no input) -- boots but is unplayable."},
            .notes = {},
        },
        {
            .key = "audio",
            .name = "Audio  --  music / sound effects",
            .holders = {"SoundSystem", "SoundSystemConfig"},
            .types = {"SoundSystem", "SoundManager", "CodecJOrbis", "SoundPool"},
            .reference = {"Audio"},
            .options =
                {"A) XACT3 -- authored wave banks (.xwb) + sound banks (.xsb), cue-driven. The Audio.reference routes "
                 "SoundManager -> XACT cues; the `codebreaker java xact` tool builds the banks from Content.md + the "
                 ".ogg/.wav sources. RECOMMENDED (the Alpha port uses XACT3).",
                 "B) XAudio2 -- low-level source voices; you own mixing/streaming/format decode yourself (more code, "
                 "no authoring pipeline).",
                 "C) Stub-silent (no audio)."},
            .notes = {},
        },
        {
            .key = "display",
            .name = "Display / window / timing",
            .holders = {"Display", "Sys"},
            .types = {"DisplayMode", "Canvas", "Frame", "Applet", "Panel", "Timer"},
            .reference = {"Display"},
            .options = {"A) Fixed 1280x720 back buffer + vsync (docs/platform/reference/Display.reference).",
                        "B) Stub."},
            .notes = {},
        },
        {
            .key = "fileio",
            .name = "File I/O & world save / load",
            .holders = {},
            .types = {"File", "RandomAccessFile", "FileInputStream", "FileOutputStream", "FileReader", "FileWriter",
                      "RegionFile", "RegionFileCache", "ZipFile", "SaveHandler", "ChunkLoader",
                      "McRegionChunkLoader"},
            .reference = {"FileIO", "ZipFile", "Inflate"},
            .options =
                {"A) STFS / Save360 XContent -- persistent saves across boots (needs the writable Save: mount).",
                 "B) In-memory only -- worlds play but do NOT persist across reboots.",
                 "C) Stub -- no save/load at all."},
            .notes = {},
        },
        {
            .key = "network",
            .name = "Networking (SMP)",
            .holders = {},
            .types = {"Socket", "URL", "URLConnection", "HttpURLConnection", "NetworkManager", "NetClientHandler",
                      "InetAddress"},
            .reference = {"Socket"},
            .options = {"A) XNet insecure socket -- LAN SMP (docs/platform/reference/Socket.reference.cpp). "
                        "Off-subnet needs a portproxy relay.",
                        "B) Stub-offline -- singleplayer only."},
            .notes = {},
        },
        {
            .key = "memory",
            .name = "Memory / allocation",
            .holders = {"Runtime"},
            .types = {},
            .reference = {},
            .options = {"A) Report the real Xbox 360 title memory budget (XMemGetAllocationInfo / retail 512MB).",
                        "B) Fixed constants (freeMemory/totalMemory return a nominal budget) -- fine unless the game "
                        "gates behaviour on free memory."},
            .notes = {},
        },
        {
            .key = "user",
            .name = "User / profile / session",
            .holders = {},   // System.* is java.lang, provided by the JavaRT.h prelude (not a platform gap)
            .types = {"Session", "StatsSyncher", "MessageDigest"},
            .reference = {"XboxUser"},
            .options = {"A) XUserGetName / gamertag + local profile (docs/platform/reference/XboxUser.reference).",
                        "B) Stub (fixed \"Player\" identity)."},
            .notes = {},
        },
    };
    return all;
}

namespace {

bool cov_is_word(char c) { return c == '_' || (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
bool cov_is_upper(char c) { return c >= 'A' && c <= 'Z'; }
bool cov_is_digit(char c) { return c >= '0' && c <= '9'; }
bool cov_is_letter_or_underscore(char c) { return c == '_' || (c >= 'a' && c <= 'z') || cov_is_upper(c); }

// The end of the run of word characters that starts at `pos`.
std::size_t cov_word_end(std::string_view s, std::size_t pos) {
    while (pos < s.size() && cov_is_word(s[pos])) ++pos;
    return pos;
}

std::size_t cov_skip_space(std::string_view s, std::size_t pos) {
    while (pos < s.size() && text::is_space(s[pos])) ++pos;
    return pos;
}

// ALL_CAPS name: [A-Z][A-Z0-9_]{2,}, the whole word.
bool cov_is_constant_name(std::string_view word) {
    if (word.size() < 3 || !cov_is_upper(word[0])) return false;
    for (char c : word) {
        if (!cov_is_upper(c) && !cov_is_digit(c) && c != '_') return false;
    }
    return true;
}

// A constant name that starts at `pos` and is a whole word: its text, or empty.
std::string_view cov_constant_at(std::string_view s, std::size_t pos) {
    const std::string_view word = s.substr(pos, cov_word_end(s, pos) - pos);
    return cov_is_constant_name(word) ? word : std::string_view();
}

std::optional<std::string> cov_read(const stdfs::path& p) { return textutil::try_read_text(p); }

// A directory's entries that are files named *.cpp or *.h, sorted by name; none for a missing directory.
std::vector<std::string> cov_source_names(const stdfs::path& dir) {
    std::vector<std::string> names;
    std::error_code ec;
    for (stdfs::directory_iterator it(dir, ec), end; !ec && it != end; it.increment(ec)) {
        const std::string name = fs::to_text(it->path().filename());
        if (name.ends_with(".cpp") || name.ends_with(".h")) names.push_back(name);
    }
    std::sort(names.begin(), names.end());
    return names;
}

// At most `max_bytes` bytes of `s`, never cutting a UTF-8 sequence in the middle.
std::string_view cov_clip(std::string_view s, std::size_t max_bytes) {
    if (s.size() <= max_bytes) return s;
    std::size_t n = max_bytes;
    while (n > 0 && (static_cast<unsigned char>(s[n]) & 0xC0) == 0x80) --n;
    return s.substr(0, n);
}

// `J2X-STUB[tag]: note` somewhere in the line: the tag and the trimmed note.
std::optional<std::pair<std::string_view, std::string_view>> cov_stub_marker(std::string_view line) {
    constexpr std::string_view kMark = "J2X-STUB[";
    for (std::size_t at = line.find(kMark); at != std::string_view::npos; at = line.find(kMark, at + 1)) {
        std::size_t p = at + kMark.size();
        const std::size_t tag_start = p;
        while (p < line.size() && ((line[p] >= 'a' && line[p] <= 'z') || cov_is_digit(line[p]) || line[p] == '-')) ++p;
        if (p == tag_start || p >= line.size() || line[p] != ']') continue;
        const std::string_view tag = line.substr(tag_start, p - tag_start);
        p = cov_skip_space(line, p + 1);
        if (p >= line.size() || line[p] != ':') continue;
        return std::make_pair(tag, text::trim(line.substr(p + 1)));
    }
    return std::nullopt;
}

}  // namespace

StubMarkers scan_stub_markers(const stdfs::path& output_dir) {
    StubMarkers out;
    for (const std::string& name : cov_source_names(output_dir)) {
        const auto content = cov_read(output_dir / fs::from_text(name));
        if (!content || content->find("J2X-STUB") == std::string::npos) continue;
        std::size_t line_no = 0;
        std::size_t start = 0;
        while (start <= content->size()) {
            const std::size_t nl = content->find('\n', start);
            const std::size_t end = nl == std::string::npos ? content->size() : nl;
            ++line_no;
            if (const auto marker = cov_stub_marker(std::string_view(*content).substr(start, end - start))) {
                out[std::string(marker->first)].push_back({name, line_no, std::string(cov_clip(marker->second, 100))});
            }
            if (nl == std::string::npos) break;
            start = nl + 1;
        }
    }
    return out;
}

std::set<std::string> provided_symbols(std::string_view text) {
    std::set<std::string> syms;
    std::size_t i = 0;
    while (i < text.size()) {
        const char c = text[i];
        if (c == '#') {   // `#define NAME`
            std::size_t p = cov_skip_space(text, i + 1);
            if (text.substr(p).starts_with("define")) {
                p += 6;
                const std::size_t q = cov_skip_space(text, p);
                if (q > p) {
                    if (const auto name = cov_constant_at(text, q); !name.empty()) syms.insert(std::string(name));
                }
            }
            ++i;
            continue;
        }
        if (!cov_is_word(c)) {
            ++i;
            continue;
        }
        const std::size_t end = cov_word_end(text, i);
        const std::string_view word = text.substr(i, end - i);
        const std::size_t after = cov_skip_space(text, end);
        // a function declaration or call: an identifier then `(`
        if (cov_is_letter_or_underscore(word[0]) && after < text.size() && text[after] == '(') syms.insert(std::string(word));
        // an enumerator or initialised constant: NAME =
        if (cov_is_constant_name(word) && after < text.size() && text[after] == '=') syms.insert(std::string(word));
        // `const <type> NAME`
        if (word == "const" && after > end) {
            std::size_t t = after;
            if (t < text.size() && cov_is_word(text[t])) {
                while (t < text.size() && (cov_is_word(text[t]) || text[t] == ':' || text[t] == '<' || text[t] == '>')) ++t;
                const std::size_t n = cov_skip_space(text, t);
                if (n > t) {
                    if (const auto name = cov_constant_at(text, n); !name.empty()) syms.insert(std::string(name));
                }
            }
        }
        i = end;
    }
    for (const char* keyword : {"if", "for", "while", "return", "sizeof", "switch"}) syms.erase(keyword);
    return syms;
}

OrderedMap<std::string, std::size_t> used_holder_symbols(std::string_view game_text, std::string_view holder) {
    OrderedMap<std::string, std::size_t> out;
    for (std::size_t at = game_text.find(holder); at != std::string_view::npos; at = game_text.find(holder, at + 1)) {
        if (at > 0 && cov_is_word(game_text[at - 1])) continue;
        std::size_t p = cov_skip_space(game_text, at + holder.size());
        if (!game_text.substr(p).starts_with("::")) continue;
        p = cov_skip_space(game_text, p + 2);
        if (p >= game_text.size() || !cov_is_letter_or_underscore(game_text[p])) continue;
        const std::size_t end = cov_word_end(game_text, p);
        ++out[std::string(game_text.substr(p, end - p))];
    }
    return out;
}

std::size_t type_usage_count(std::string_view game_text, std::string_view type) {
    std::size_t count = 0;
    for (std::size_t at = game_text.find(type); at != std::string_view::npos; at = game_text.find(type, at + 1)) {
        if (at > 0 && (cov_is_word(game_text[at - 1]) || game_text[at - 1] == ':')) continue;
        const std::size_t end = at + type.size();
        if (end < game_text.size() && cov_is_word(game_text[end])) continue;
        ++count;
    }
    return count;
}

std::vector<const SymbolUse*> Coverage::with_status(Status s) const {
    std::vector<const SymbolUse*> out;
    for (const SymbolUse& u : symbols) {
        if (u.status == s) out.push_back(&u);
    }
    return out;
}

std::vector<Coverage> analyze(const stdfs::path& output_dir, const std::optional<stdfs::path>& reference_dir,
                              const std::set<std::string>& runtime_files) {
    // The game code is everything except the emitted runtime/stub headers.
    std::vector<std::string> game_texts;
    for (const std::string& name : cov_source_names(output_dir)) {
        if (runtime_files.contains(name)) continue;
        game_texts.push_back(cov_read(output_dir / fs::from_text(name)).value_or(std::string()));
    }

    auto stub_provides = [&](std::string_view holder) {
        return provided_symbols(cov_read(output_dir / (std::string(holder) + ".h")).value_or(std::string()));
    };
    auto reference_provides = [&](const std::vector<std::string_view>& names) {
        std::set<std::string> s;
        if (!reference_dir) return s;
        for (std::string_view n : names) {
            for (const char* ext : {".reference.h", ".reference.cpp"}) {
                const auto text = cov_read(*reference_dir / (std::string(n) + ext));
                if (text) s.merge(provided_symbols(*text));
            }
        }
        return s;
    };

    std::vector<Coverage> results;
    for (const Subsystem& sub : subsystems()) {
        Coverage cov;
        cov.sub = &sub;
        const std::set<std::string> ref_syms = reference_provides(sub.reference);
        cov.ref_exists = !sub.reference.empty() && !ref_syms.empty();
        for (std::string_view holder : sub.holders) {
            const std::set<std::string> provided = stub_provides(holder);
            OrderedMap<std::string, std::size_t> used;   // symbol -> uses over all the game files
            for (const std::string& text : game_texts) {
                for (const auto& [sym, n] : used_holder_symbols(text, holder)) used[sym] += n;
            }
            std::vector<std::pair<std::string, std::size_t>> by_use;
            for (const auto& [sym, n] : used) by_use.emplace_back(sym, n);
            std::stable_sort(by_use.begin(), by_use.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
            for (const auto& [sym, n] : by_use) {
                const Status status = ref_syms.contains(sym) ? Status::Reusable
                                      : provided.contains(sym) ? Status::Stub
                                                               : Status::Missing;
                cov.symbols.push_back({std::string(holder) + "::" + sym, n, status});
            }
        }
        for (std::string_view type : sub.types) {
            std::size_t n = 0;
            for (const std::string& text : game_texts) n += type_usage_count(text, type);
            if (n == 0) continue;
            const bool in_reference = std::find(sub.reference.begin(), sub.reference.end(), type) != sub.reference.end() ||
                                      ref_syms.contains(std::string(type));
            cov.types.push_back({std::string(type), n, in_reference});
        }
        results.push_back(std::move(cov));
    }
    return results;
}

std::vector<std::string> render_stub_markers(const StubMarkers& markers) {
    std::vector<std::string> L;
    if (markers.empty()) return L;
    std::size_t total = 0;
    for (const auto& kv : markers) total += kv.second.size();
    L.push_back("---");
    L.push_back("");
    L.push_back(std::format("## J2X-STUB markers -- the explicit placeholder surface ({})", total));
    L.push_back("");
    L.push_back(
        "Every no-op / default-return in the emitted stubs and the hand-written references is marked "
        "`// J2X-STUB[tag]: ...` at the exact function. `grep -rn \"J2X-STUB\" .` is the full audit list; grouped by "
        "tag here. A no-op is only safe if its effect is cosmetic -- verify the load-bearing ones (the "
        "glPolygonOffset z-fight class of bug lived in an unmarked no-op).");
    L.push_back("");
    L.push_back("| Tag | Count | Example site |");
    L.push_back("|---|---:|---|");
    std::vector<const StubMarkers::value_type*> tags;
    for (const auto& kv : markers) tags.push_back(&kv);
    std::stable_sort(tags.begin(), tags.end(), [](const auto* a, const auto* b) { return a->second.size() > b->second.size(); });
    for (const auto* kv : tags) {
        const StubSite& first = kv->second.front();
        L.push_back(std::format("| `{}` | {} | `{}:{}` -- {} |", kv->first, kv->second.size(), first.file, first.line, first.note));
    }
    L.push_back("");
    return L;
}

namespace {

// The subsystem name up to its first "  --" ("Video  --  OpenGL..." -> "Video"), trimmed.
std::string_view cov_short_name(std::string_view name) {
    const std::size_t cut = name.find("  --");
    return text::trim(cut == std::string_view::npos ? name : name.substr(0, cut));
}

std::string cov_join_ticked(std::span<const std::string_view> names, std::string_view suffix) {
    std::string out = "`";
    for (std::size_t i = 0; i < names.size(); ++i) {
        if (i > 0) out += "`, `";
        out += names[i];
        out += suffix;
    }
    return out + "`";
}

std::string_view cov_severity_text(const std::optional<console::xdk_scan::Severity>& s) {
    using console::xdk_scan::Severity;
    if (!s) return "";
    return *s == Severity::Correctness ? "CORRECTNESS" : *s == Severity::Cert ? "CERT" : "COMPAT";
}

std::string_view cov_effort_text(const std::optional<console::xdk_scan::Effort>& e) {
    using console::xdk_scan::Effort;
    if (!e) return "?";
    return *e == Effort::Small ? "S" : "M";
}

}  // namespace

std::string render_report(const std::vector<Coverage>& results, const std::optional<stdfs::path>& reference_dir,
                          const StubMarkers& stub_markers, const std::optional<console::xdk_scan::ScanResult>& sdk_caps) {
    using console::xdk_scan::Kind;
    std::error_code ec;
    const bool have_ref = reference_dir && stdfs::is_directory(*reference_dir, ec);
    std::vector<std::string> L;
    auto add = [&L](std::string line) { L.push_back(std::move(line)); };

    add("# PLATFORM_COVERAGE.md -- platform-layer coverage & decisions");
    add("");
    add("Every platform subsystem the converted game touches, and what still has to be answered to");
    add("make it run on real hardware. The converter emits **no-op stubs** for all of these; this");
    add("report says, per symbol, whether the real implementation already exists in the Alpha");
    add("`docs/platform/reference/` layer (**REUSABLE**), is only a placeholder (**STUB**), or is");
    add("**MISSING** (new to this version -> new work).");
    add("");
    if (!have_ref) {
        add("> NOTE: the Alpha reference layer was not found, so nothing could be marked REUSABLE. Pass its path "
            "(`--reference docs/platform/reference`) to classify against the real impls.");
        add("");
    }

    // scoreboard
    add("## Scoreboard");
    add("");
    add("| Subsystem | Reference impl | REUSABLE | STUB | MISSING |");
    add("|---|---|---:|---:|---:|");
    std::size_t total_missing = 0;
    for (const Coverage& r : results) {
        add(std::format("| {} | {} | {} | {} | {} |", cov_short_name(r.sub->name), r.ref_exists ? "yes" : "no",
                        r.with_status(Status::Reusable).size(), r.with_status(Status::Stub).size(),
                        r.with_status(Status::Missing).size()));
        total_missing += r.with_status(Status::Missing).size();
    }
    add("");
    add(std::format("**{} symbol(s) are MISSING** (used by the game, absent from both the stub and the Alpha reference) "
                    "-- those are the concrete new work items, listed per subsystem below.",
                    total_missing));
    add("");

    // Platform REQUIREMENTS -- AUTO-DETECTED from the installed SDK (xdk_scan), NOT hardcoded. Each is claimed ONLY
    // because its XDK function was actually found in the scanned SDK headers.
    std::vector<const console::xdk_scan::Detection*> requirements, opportunities;
    if (sdk_caps) {
        for (const auto& d : sdk_caps->found) {
            if (d.kind == Kind::Requirement) requirements.push_back(&d);
            if (d.kind == Kind::Opportunity) opportunities.push_back(&d);
        }
    }
    if (sdk_caps && sdk_caps->include && !sdk_caps->include->empty()) {
        add("---");
        add("");
        add("## Xbox 360 platform requirements -- auto-detected from the installed SDK");
        add("");
        add(std::format("Rules the console/XDK imposes that are NOT a single stub-swap. **Each row is here because its "
                        "API was found in the scanned SDK** (`{}`, {} functions) -- derived from the SDK, not from "
                        "memory. Ignore one and the feature silently breaks or the title fails cert.",
                        fs::to_text(*sdk_caps->include), sdk_caps->funcs.size()));
        add("");
        add("| Requirement | Detected in SDK | Severity | If ignored |");
        add("|---|---|:-:|---|");
        for (const auto* r : requirements) {
            const std::string fam = r->func_count > 1 ? std::format("  (+{} in family)", r->func_count - 1) : "";
            add(std::format("| **{}** | `{}`{} -- {} | {} | {} |", r->title, r->needs, fam, r->header,
                            cov_severity_text(r->sev), r->why));
        }
        add("");
    } else {
        add("---");
        add("");
        add("## Xbox 360 platform requirements");
        add("");
        add("> No SDK was scanned, so requirements could not be auto-detected. (They are derived from the SDK headers, "
            "not hardcoded -- supply the SDK to populate this.)");
        add("");
    }

    // Xbox 360 XDK equivalents (the authoritative Java-symbol -> XDK-API map)
    add("---");
    add("");
    add("## Xbox 360 XDK equivalents -- the real API for every platform symbol");
    add("");
    add("For each Java/LWJGL platform type the port stubs, the exact XDK function to resolve it to (this same line is "
        "emitted in the stub header as `// [360] equivalent:`). **real** = the port implements it; **stub** = still a "
        "no-op but the API named is the drop-in; **dead** = no console equivalent.");
    add("");
    add("| Subsystem | Java symbol | Xbox 360 XDK API | Lib | Status |");
    add("|---|---|---|---|---|");
    struct EquivRow {
        std::size_t subsystem;
        const pe::Equiv* equiv;
        std::string_view subsystem_name;
    };
    std::vector<EquivRow> rows;
    const auto subs = pe::subsystems();
    for (std::size_t s = 0; s < subs.size(); ++s) {
        for (const pe::Equiv& e : subs[s].entries) rows.push_back({s, &e, subs[s].name});
    }
    std::stable_sort(rows.begin(), rows.end(), [](const EquivRow& a, const EquivRow& b) {
        if (a.subsystem != b.subsystem) return a.subsystem < b.subsystem;
        if (a.equiv->status != b.equiv->status) return a.equiv->status < b.equiv->status;   // real, stub, dead
        return a.equiv->basename < b.equiv->basename;
    });
    for (const EquivRow& row : rows) {
        const char* badge = row.equiv->status == pe::Status::Real ? "REAL" : row.equiv->status == pe::Status::Stub ? "stub" : "dead";
        add(std::format("| {} | `{}` | {} | {} | {} |", row.subsystem_name, row.equiv->basename, row.equiv->xdk,
                        row.equiv->lib, badge));
    }
    add("");

    // Untapped XDK capabilities -- AUTO-DETECTED in the scanned SDK (not hardcoded)
    if (sdk_caps && !opportunities.empty()) {
        add("---");
        add("");
        add("## Untapped XDK capabilities -- 360-native wins, auto-detected in the SDK");
        add("");
        add("Found in the scanned SDK headers; not stubs to fill -- features/optimizations worth adopting.");
        add("");
        add("| Feature | Detected in SDK | Effort | Why it helps |");
        add("|---|---|:-:|---|");
        for (const auto* o : opportunities) {
            add(std::format("| **{}** | `{}` -- {} | {} | {} |", o->title, o->needs, o->header, cov_effort_text(o->effort), o->why));
        }
        add("");
    }

    for (const Coverage& r : results) {
        const Subsystem& sub = *r.sub;
        if (r.symbols.empty() && r.types.empty() && sub.notes.empty()) continue;
        add("---");
        add("");
        add(std::format("## {}", sub.name));
        add("");
        add(std::format("**Reference implementation:** {}",
                        sub.reference.empty() ? std::string("none (platform-native only)")
                                              : cov_join_ticked(sub.reference, ".reference.*")));
        add("");
        add("**Choose an implementation:**");
        for (std::string_view opt : sub.options) add(std::format("- {}", opt));
        add("");
        if (!sub.notes.empty()) {
            add("**Known considerations (verify these even when the symbols are all covered):**");
            add("");
            for (std::string_view note : sub.notes) add(std::format("- ⚠️ {}", note));
            add("");
        }
        const auto missing = r.with_status(Status::Missing);
        const auto reusable = r.with_status(Status::Reusable);
        const auto stub = r.with_status(Status::Stub);
        if (!missing.empty()) {
            add(std::format("### MISSING -- new work for this version ({})", missing.size()));
            add("");
            add("| Symbol | Used | Note |");
            add("|---|---:|---|");
            for (const SymbolUse* u : missing) {
                add(std::format("| `{}` | {} | not in the stub OR the Alpha reference -- implement it |", u->name, u->count));
            }
            add("");
        }
        if (!reusable.empty()) {
            add(std::format("### REUSABLE -- already implemented in the Alpha reference ({})", reusable.size()));
            add("");
            add("These compile+work as soon as the reference file replaces the stub. Top by use:");
            add("");
            add("| Symbol | Used |");
            add("|---|---:|");
            for (std::size_t i = 0; i < reusable.size() && i < 20; ++i) {
                add(std::format("| `{}` | {} |", reusable[i]->name, reusable[i]->count));
            }
            if (reusable.size() > 20) add(std::format("| … ({} more) | |", reusable.size() - 20));
            add("");
        }
        if (!stub.empty()) {
            add(std::format("### STUB -- placeholder only, verify it is not load-bearing ({})", stub.size()));
            add("");
            add("A no-op here is fine if the effect is cosmetic, but silently wrong if it matters (the glPolygonOffset "
                "z-fight class). Review the high-use ones:");
            add("");
            add("| Symbol | Used |");
            add("|---|---:|");
            for (std::size_t i = 0; i < stub.size() && i < 15; ++i) {
                add(std::format("| `{}` | {} |", stub[i]->name, stub[i]->count));
            }
            add("");
        }
        if (!r.types.empty()) {
            add("### Platform types referenced");
            add("");
            add("| Type | Used | In reference? |");
            add("|---|---:|---|");
            for (const TypeUse& t : r.types) add(std::format("| `{}` | {} | {} |", t.name, t.count, t.in_reference ? "yes" : "no"));
            add("");
        }
    }
    for (std::string& line : render_stub_markers(stub_markers)) add(std::move(line));
    return text::join(L, "\n") + "\n";
}

Written write_report(const stdfs::path& output_dir, const WriteOptions& options) {
    Written out;
    out.results = analyze(output_dir, options.reference_dir, options.runtime_files);
    const StubMarkers markers = scan_stub_markers(output_dir);

    // AUTO-DETECT platform requirements/capabilities from the installed SDK (not hardcoded). Best effort: if no SDK is
    // resolvable, the report says so rather than inventing requirements.
    std::optional<console::xdk_scan::ScanResult> sdk_caps;
    try {
        std::optional<stdfs::path> root = options.sdk_root;
        if (!root) {
            if (const auto found = console::sdk::detect("xbox360", options.sdk_hints)) root = found->root();
        }
        if (root && !root->empty()) sdk_caps = console::xdk_scan::detect(root);
    } catch (const Error& e) {   // an isolation boundary: the SDK scan is auxiliary to the report
        log::warn(std::format("platform coverage: the SDK scan failed, the report has no detected requirements: {}", e.what()));
        sdk_caps.reset();
    }

    out.path = output_dir / "PLATFORM_COVERAGE.md";
    textutil::write_text(out.path, render_report(out.results, options.reference_dir, markers, sdk_caps));
    return out;
}

}  // namespace cb::java::projects::minecraft::platform_coverage
