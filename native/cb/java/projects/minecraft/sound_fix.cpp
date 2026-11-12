#include "cb/java/projects/minecraft/sound_fix.hpp"

#include <map>
#include <set>
#include <utility>

#include "cb/core/fs.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/java/projects/minecraft/converted_tree.hpp"
#include "cb/java/projects/minecraft/ir_edit.hpp"

namespace cb::java::projects::minecraft::sound_fix {

namespace ir = translate::ir;
namespace edit = ir_edit;
namespace tree = converted_tree;

namespace {

// The SFX guard site the generator emits:
//     SoundPoolEntry* <V> = <recv>->soundPoolSounds->getRandomSoundFromSoundPool(<NAME>);
//     if(<V> != NULL[ && <EXTRA>]) {
// <recv> is usually `this` but kept permissive. <NAME> is the sound-name argument (a simple identifier). ANCHORED to
// `soundPoolSounds` so the streaming (soundPoolStreaming) and music (soundPoolMusic->getRandomSound) sites are NOT
// matched.
const Regex& sfx_site() {
    static const Regex re(
        R"((SoundPoolEntry\s*\*\s*(\w+)\s*=\s*[^;]*?\bsoundPoolSounds\s*->\s*)"
        R"(getRandomSoundFromSoundPool\s*\(\s*(\w+)\s*\)\s*;))"
        R"((\s*)if\s*\(\s*\2\s*!=\s*NULL\s*(?:&&\s*([^)]*?)\s*)?\))");
    return re;
}

constexpr std::string_view kSfxNote =
    "   // J2X sfx-bypass: console OGG pool is empty; drop the null-guard and route the sound name to the XACT bridge "
    "(newSource identifier == cue). [generator: sound_fix]";

// Fail loud: a soundPoolSounds SFX guard still gated on `!= NULL` after the pass ran (a future decompile whose guard
// shape changed) -- a silent-SFX regression this rule no longer catches.
const Regex& sfx_survivor() {
    static const Regex re(R"(soundPoolSounds\s*->\s*getRandomSoundFromSoundPool[^;]*;\s*if\s*\([^)]*!=\s*NULL)");
    return re;
}

// The music site the generator emits:
//     SoundPoolEntry* <V> = <recv>->soundPoolMusic->getRandomSound();
//     if(<V> != NULL) { <BODY: flat statements, no nested braces> }
// On console soundPoolMusic is EMPTY, so <V> == NULL and the whole block (including play("BgMusic")) is skipped -> the
// XACT music playlist never starts. UNLIKE SFX, the fix is NOT (void*)NULL routing: the proven Alpha playRandomMusic
// hand-fix hoists play/setVolume/delay OUT of the guard so play("BgMusic") (which starts the XACT playlist) fires
// UNCONDITIONALLY, while backgroundMusic -- the only call that needs a real SoundPoolEntry -- stays guarded and is
// simply skipped. The pass reproduces that more robustly by making the outer guard `if(true)` and inline-guarding ONLY
// the <V>-> deref line, with no statement code-motion. It needs a flat (no nested-brace) block, which the paulscode
// playRandomMusic body is.
const Regex& music_site() {
    static const Regex re(
        R"((SoundPoolEntry\s*\*\s*(\w+)\s*=\s*[^;]*?\bsoundPoolMusic\s*->\s*getRandomSound\s*\(\s*\)\s*;))"
        R"((\s*)if\s*\(\s*\2\s*!=\s*NULL\s*\)\s*\{([^{}]*)\})");
    return re;
}

constexpr std::string_view kMusicNote =
    "   // J2X music-bypass: console music pool is empty; run the play() path unconditionally so the XACT playlist "
    "starts, keeping only the SoundPoolEntry deref (backgroundMusic) guarded. [generator: sound_fix]";

// Fail loud, music: a soundPoolMusic guard still gated on `!= NULL` immediately after the declaration.
const Regex& music_survivor() {
    static const Regex re(R"(soundPoolMusic\s*->\s*getRandomSound\s*\(\s*\)\s*;\s*if\s*\([^)]*!=\s*NULL)");
    return re;
}

bool sound_is_word(char c) { return c == '_' || (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

// The position just after `->` and the whitespace that follows it, when `text` has `\s*->\s*` at `pos`.
std::size_t sound_skip_arrow(std::string_view text, std::size_t pos) {
    while (pos < text.size() && text::is_space(text[pos])) ++pos;
    if (!text.substr(pos).starts_with("->")) return std::string_view::npos;
    pos += 2;
    while (pos < text.size() && text::is_space(text[pos])) ++pos;
    return pos;
}

// Where `var->member` (as a whole word `var`, optional space around the arrow, `member` a whole word) starts at or
// after `from`: the range [start, end) it covers, or npos.
struct MemberAccess {
    std::size_t start = std::string_view::npos;
    std::size_t end = std::string_view::npos;
};
MemberAccess sound_find_access(std::string_view text, std::string_view var, std::string_view member, std::size_t from) {
    for (std::size_t i = text.find(var, from); i != std::string_view::npos; i = text.find(var, i + 1)) {
        if (i > 0 && sound_is_word(text[i - 1])) continue;
        std::size_t after_var = i + var.size();
        if (after_var < text.size() && sound_is_word(text[after_var])) continue;
        std::size_t m = sound_skip_arrow(text, after_var);
        if (m == std::string_view::npos || !text.substr(m).starts_with(member)) continue;
        if (m + member.size() < text.size() && sound_is_word(text[m + member.size()])) continue;
        return {i, m + member.size()};
    }
    return {};
}

// Replace every `var->member` with `replacement`.
std::string sound_replace_access(std::string_view text, std::string_view var, std::string_view member,
                                 std::string_view replacement) {
    std::string out;
    std::size_t at = 0;
    for (MemberAccess a = sound_find_access(text, var, member, 0); a.start != std::string_view::npos;
         a = sound_find_access(text, var, member, at)) {
        out.append(text.substr(at, a.start - at));
        out.append(replacement);
        at = a.end;
    }
    out.append(text.substr(at));
    return out;
}

// The end of the first `var\s*->` in the line (`var` not the tail of a longer name), or npos.
std::size_t sound_first_deref_end(std::string_view line, std::string_view var) {
    for (std::size_t i = line.find(var); i != std::string_view::npos; i = line.find(var, i + 1)) {
        if (i > 0 && sound_is_word(line[i - 1])) continue;
        std::size_t p = i + var.size();
        while (p < line.size() && text::is_space(line[p])) ++p;
        if (line.substr(p).starts_with("->")) return p + 2;
    }
    return std::string_view::npos;
}

// In the body of a music guard, put `if(<var> != NULL) ` in front of every line that dereferences `var` (and ends a
// statement after the arrow).
std::string sound_guard_music_body(std::string_view body, std::string_view var) {
    std::string out;
    std::size_t line_start = 0;
    bool first = true;
    while (line_start <= body.size()) {
        std::size_t nl = body.find('\n', line_start);
        const std::size_t line_end = nl == std::string_view::npos ? body.size() : nl;
        std::string_view line = body.substr(line_start, line_end - line_start);
        // The first piece has no newline before it: it is not a line of the body.
        bool guard = false;
        std::size_t indent = 0;
        if (!first) {
            while (indent < line.size() && (line[indent] == ' ' || line[indent] == '\t')) ++indent;
            const std::string_view rest = line.substr(indent);
            const std::size_t arrow_end = sound_first_deref_end(rest, var);
            guard = arrow_end != std::string_view::npos && rest.find(';', arrow_end) != std::string_view::npos;
        }
        if (guard) {
            out.append(line.substr(0, indent));
            out.append("if(").append(var).append(" != NULL) ");
            out.append(line.substr(indent));
        } else {
            out.append(line);
        }
        if (nl == std::string_view::npos) break;
        out.push_back('\n');
        line_start = nl + 1;
        first = false;
    }
    return out;
}

bool sound_mentions_pool(std::string_view text) {
    return text.find("getRandomSound") != std::string_view::npos &&
           (text.find("soundPoolSounds") != std::string_view::npos ||
            text.find("soundPoolMusic") != std::string_view::npos);
}

}  // namespace

Rewritten rewrite(std::string_view text) {
    Rewritten out;
    std::vector<std::pair<std::string, std::string>> routed;   // (entry local, sound-name argument) per SFX site
    std::string current = sfx_site().replace_all(text, [&](const RegexMatch& m) {
        const std::string var(m.group(2));
        const std::string name(m.group(3));
        const std::string_view ws = m.group(4);
        // Keep any volume gate; otherwise run always.
        const std::string_view extra = m.has(5) ? text::trim(m.group(5)) : std::string_view();
        const std::string guard = extra.empty() ? "true" : std::string(extra);
        routed.emplace_back(var, name);
        // DROP the empty-pool query itself, don't just ignore its result. On console soundPoolSounds is empty and
        // getRandomSoundFromSoundPool(absent) HANGS: JMap.get(absent) returns a NON-null garbage value,
        // JList::operator==(long long) is hardcoded false so the `<V> == NULL` guard never fires, and it reaches
        // `list[rand.nextInt(garbageSize)]` where nextInt(negative bound) infinite-loops. <V> is unused after the
        // guard becomes if(true) (SFX routes the NAME to the XACT cue), so declare it NULL instead of querying.
        return "SoundPoolEntry* " + var + " = NULL;" + std::string(kSfxNote) + std::string(ws) + "if(" + guard + ")";
    });
    out.sfx = routed.size();
    // SFX: kill the now-NULL SoundPoolEntry derefs for each rewritten site -- the entry is NULL on console, so route
    // (void*)NULL as the url and the sound NAME as the cue identifier (matches the hand-fix). <V> is a unique per-site
    // local, so a whole-file replace of `<V>->soundUrl/soundName` only touches this method.
    // A local that names several sites is routed by the first of them: the whole-file replace leaves nothing for a
    // later site to find, so the later ones are skipped.
    std::set<std::string_view> done;
    for (const auto& [var, name] : routed) {
        if (!done.insert(var).second) continue;
        current = sound_replace_access(current, var, "soundUrl", "(void*)NULL");
        current = sound_replace_access(current, var, "soundName", name);
    }
    // MUSIC: make the outer guard unconditional (play("BgMusic") -> XACT playlist fires) and inline-guard only the
    // SoundPoolEntry deref line (backgroundMusic), which is simply skipped on the empty console pool.
    out.text = music_site().replace_all(current, [&](const RegexMatch& m) {
        ++out.music;
        return std::string(m.group(1)) + std::string(kMusicNote) + std::string(m.group(3)) + "if(true) {" +
               sound_guard_music_body(m.group(4), m.group(2)) + "}";
    });
    return out;
}

Report run(const std::filesystem::path& out_dir) {
    Report report;
    for (const std::string& name : tree::cpp_files(out_dir)) {
        const std::filesystem::path path = out_dir / fs::from_text(name);
        auto file = tree::read_cpp(path);
        if (!file || !sound_mentions_pool(file->text)) continue;
        Rewritten r = rewrite(file->text);
        if (r.text != file->text) {
            tree::write_cpp(path, r.text, file->crlf);
            report.files.push_back({name, r.sfx, r.music});
            report.sites += r.sfx + r.music;
        }
        if (sfx_survivor().test(r.text) || music_survivor().test(r.text)) report.stranded.push_back(name);
    }
    return report;
}

namespace {

// The rewrite of one `Local` + `If` pair of a statement list. Returns which kind of site it was (and rewrites it).
enum class SoundSite { None, Sfx, Music };

SoundSite sound_patch_pair(Arena& arena, ir::Node* loc, ir::Node* nxt) {
    ir::Local* local = ir::get<ir::Local>(loc);
    ir::If* guard = ir::get<ir::If>(nxt);
    if (local == nullptr || local->init == nullptr || guard == nullptr || local->sym == nullptr) return SoundSite::None;
    const ir::SymP v = local->sym;
    if (edit::is_call(local->init, "getRandomSoundFromSoundPool", "soundPoolSounds")) {
        ir::Node* cond = edit::strip(guard->cond);
        ir::Node* rest = nullptr;   // the volume gate to keep, if any
        if (!edit::is_null_test(cond, *v)) {
            const ir::Binary* b = ir::get<ir::Binary>(cond);
            if (b == nullptr || b->op != "&&" || !edit::is_null_test(b->left, *v)) return SoundSite::None;
            rest = b->right;
        }
        const ir::Call* query = ir::get<ir::Call>(edit::strip(local->init));
        if (query->args.empty()) return SoundSite::None;
        ir::Node* sound = query->args[0];                 // the sound name: the XACT cue
        local->init = edit::null_lit(arena);               // no query: the empty pool's lookup hangs on console
        guard->cond = rest != nullptr ? rest : edit::true_lit(arena);
        edit::replace(nxt, [&](ir::Node* n) -> ir::Node* {
            const ir::Select* sel = ir::get<ir::Select>(n);
            if (sel == nullptr || sel->sym == nullptr || !edit::is_name(sel->obj, *v)) return nullptr;
            if (sel->sym->name == "soundUrl") return edit::null_lit(arena);
            if (sel->sym->name == "soundName") return sound;
            return nullptr;
        });
        return SoundSite::Sfx;
    }
    if (edit::is_call(local->init, "getRandomSound", "soundPoolMusic") && edit::is_null_test(guard->cond, *v) &&
        !ir::present(guard->orelse)) {
        const auto* then = std::get_if<ir::Node*>(&guard->then);
        ir::Block* body = then != nullptr ? ir::get<ir::Block>(*then) : nullptr;
        if (body == nullptr) return SoundSite::None;
        guard->cond = edit::true_lit(arena);   // play("BgMusic") starts the XACT playlist: always
        for (ir::Node*& s : body->stmts) {
            if (!edit::mentions(s, *v)) continue;
            ir::Node* test = edit::binary(arena, "!=", edit::name_of(arena, v, local->type), edit::null_lit(arena),
                                          ir::bool_type());
            s = edit::if_then(arena, test, s);
        }
        return SoundSite::Music;
    }
    return SoundSite::None;
}

}  // namespace

Report patch(Arena& arena, const std::vector<ir::Node*>& units) {
    std::map<std::string, std::pair<std::size_t, std::size_t>> per_class;   // sorted: class -> (sfx, music)
    for (const edit::MethodInClass& m : edit::methods(units)) {
        const std::string cls = m.cls->name.value_or("(anonymous)");
        for (ir::Node* root : ir::statements(m.method->body)) {
            for (std::vector<ir::Node*>* list : edit::stmt_lists(root)) {
                for (std::size_t i = 0; i + 1 < list->size(); ++i) {
                    switch (sound_patch_pair(arena, (*list)[i], (*list)[i + 1])) {
                        case SoundSite::Sfx: ++per_class[cls].first; break;
                        case SoundSite::Music: ++per_class[cls].second; break;
                        case SoundSite::None: break;
                    }
                }
            }
        }
    }
    Report report;
    for (const auto& [cls, counts] : per_class) {
        report.files.push_back({cls + ".cpp", counts.first, counts.second});
        report.sites += counts.first + counts.second;
    }
    return report;
}

std::vector<std::string> stranded(const std::filesystem::path& out_dir) {
    std::vector<std::string> out;
    for (const std::string& name : tree::cpp_files(out_dir)) {
        auto file = tree::read_cpp(out_dir / fs::from_text(name));
        if (file && (sfx_survivor().test(file->text) || music_survivor().test(file->text))) out.push_back(name);
    }
    return out;
}

}  // namespace cb::java::projects::minecraft::sound_fix
