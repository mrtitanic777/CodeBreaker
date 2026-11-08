// codebreaker/java/projects/minecraft/sound_fix.cpp -- C++ port of codebreaker.java.projects.minecraft.sound_fix
// (see the header).

#include "codebreaker/java/projects/minecraft/sound_fix.hpp"

#include <array>
#include <map>
#include <memory>

#include "codebreaker/translate/ir.hpp"
#include "codebreaker/translate/rewrite.hpp"

namespace codebreaker::java::projects::minecraft::sound_fix {

namespace ir = codebreaker::translate::ir;
namespace rw = codebreaker::translate::rewrite;

const py::re::Pattern _SFX_SITE = py::re::compile(py::str(
    R"((SoundPoolEntry\s*\*\s*(\w+)\s*=\s*[^;]*?\bsoundPoolSounds\s*->\s*)"
    R"(getRandomSoundFromSoundPool\s*\(\s*(\w+)\s*\)\s*;))"
    R"((\s*)if\s*\(\s*\2\s*!=\s*NULL\s*(?:&&\s*([^)]*?)\s*)?\))"));

const py::str _NOTE = py::str(
    "   // J2X sfx-bypass: console OGG pool is empty; drop the null-guard and route the sound name to "
    "the XACT bridge (newSource identifier == cue). [generator: sound_fix.py]");

const py::re::Pattern _SURVIVOR =
    py::re::compile(py::str(R"(soundPoolSounds\s*->\s*getRandomSoundFromSoundPool[^;]*;\s*if\s*\([^)]*!=\s*NULL)"));

const py::re::Pattern _MUSIC_SITE = py::re::compile(py::str(
    R"((SoundPoolEntry\s*\*\s*(\w+)\s*=\s*[^;]*?\bsoundPoolMusic\s*->\s*getRandomSound\s*\(\s*\)\s*;))"
    R"((\s*)if\s*\(\s*\2\s*!=\s*NULL\s*\)\s*\{([^{}]*)\})"));

const py::str _MUSIC_NOTE = py::str(
    "   // J2X music-bypass: console music pool is empty; run the play() path unconditionally so "
    "the XACT playlist starts, keeping only the SoundPoolEntry deref (backgroundMusic) guarded. "
    "[generator: sound_fix.py]");

const py::re::Pattern _MUSIC_SURVIVOR =
    py::re::compile(py::str(R"(soundPoolMusic\s*->\s*getRandomSound\s*\(\s*\)\s*;\s*if\s*\([^)]*!=\s*NULL)"));

std::tuple<py::str, int64_t, int64_t> process(const py::str& text) {
    py::list<std::tuple<py::str, py::str>> sfx;

    auto sfx_repl = [&](const py::re::Match& m) {
        py::str V = m.group(2);
        py::str NAME = m.group(3);
        py::str ws = m.group(4);
        std::optional<py::str> extra = m.group_opt(5);
        // keep any volume gate; else run always
        py::str guard = (extra && py::truthy(extra->strip())) ? extra->strip() : py::str("true");
        sfx.append({V, NAME});
        // DROP the empty-pool query itself, don't just ignore its result. On console soundPoolSounds is empty
        // and getRandomSoundFromSoundPool(absent) HANGS: JMap.get(absent) returns a NON-null garbage value,
        // JList::operator==(long long) is hardcoded false so the `<V> == NULL` guard never fires, and it reaches
        // `list[rand.nextInt(garbageSize)]` where nextInt(negative bound) infinite-loops. <V> is unused after the
        // guard becomes if(true) (SFX routes the NAME to the XACT cue), so declare it NULL instead of querying.
        return py::percent(py::str("SoundPoolEntry* %s = NULL;%s%sif(%s)"), V, _NOTE, ws, guard);
    };

    py::str out = _SFX_SITE.sub(sfx_repl, text);
    // SFX: kill the now-NULL SoundPoolEntry derefs for each rewritten site -- the entry is NULL on console, so
    // route (void*)NULL as the url and the sound NAME as the cue identifier (matches the hand-fix). <V> is a
    // unique per-site local, so a whole-file replace of `<V>->soundUrl/soundName` only touches this method.
    for (const auto& vn : sfx) {
        const py::str& V = std::get<0>(vn);
        const py::str& NAME = std::get<1>(vn);
        out = py::re::sub(py::percent(py::str(R"(\b%s\b\s*->\s*soundUrl\b)"), py::re::escape(V)), py::str("(void*)NULL"), out);
        out = py::re::sub(py::percent(py::str(R"(\b%s\b\s*->\s*soundName\b)"), py::re::escape(V)), NAME, out);
    }

    // MUSIC: make the outer guard unconditional (play("BgMusic") -> XACT playlist fires) and inline-guard only
    // the SoundPoolEntry deref line (backgroundMusic), which is simply skipped on the empty console pool.
    int64_t n_music = 0;

    auto music_repl = [&](const py::re::Match& m) {
        py::str decl = m.group(1);
        py::str V = m.group(2);
        py::str ws = m.group(3);
        py::str body = m.group(4);
        py::str newbody = py::re::sub(py::percent(py::str(R"((\n[ \t]*)([^\n]*\b%s\s*->[^\n]*;))"), py::re::escape(V)),
                                      py::percent(py::str(R"(\1if(%s != NULL) \2)"), V), body);
        n_music += 1;
        return py::percent(py::str("%s%s%sif(true) {%s}"), decl, _MUSIC_NOTE, ws, newbody);
    };

    out = _MUSIC_SITE.sub(music_repl, out);
    return {out, py::len(sfx), n_music};
}

Result run(const py::str& out_dir) {
    py::list<py::str> files;
    for (const py::str& f : py::sorted(py::os::listdir(out_dir))) {
        if (f.endswith(py::str(".cpp"))) {
            files.append(f);
        }
    }
    Result res;
    for (const py::str& f : files) {
        py::str path = py::os::path::join(out_dir, f);
        py::str text;
        try {
            text = py::open(path, py::str("r"), py::str("utf-8"), py::str("replace")).read_text();
        } catch (const py::OSError&) {                     // (IOError, OSError)
            continue;
        }
        if (!text.contains(py::str("getRandomSound")) ||
            (!text.contains(py::str("soundPoolSounds")) && !text.contains(py::str("soundPoolMusic")))) {
            continue;
        }
        auto [new_text, n_sfx, n_music] = process(text);
        if (new_text != text) {
            py::file fh = py::open(path, py::str("w"), py::str("utf-8"));
            fh.write(new_text);
            fh.close();
            res.files.append(Touched{f, n_sfx, n_music});
            res.sites += n_sfx + n_music;
        }
        if (_SURVIVOR.search(new_text) || _MUSIC_SURVIVOR.search(new_text)) {
            res.stranded.append(f);
        }
    }
    return res;
}

namespace {

[[noreturn]] void none_iter() { throw py::TypeError("'NoneType' object is not iterable"); }

template <class T> py::ref<T> as(const ir::NodeP& n) { return std::dynamic_pointer_cast<T>(n); }

}  // namespace

Result patch(const py::list<ir::NodeP>& units) {
    // class name (None possible) -> [sfx, music]
    std::map<std::optional<py::str>, std::array<int64_t, 2>> per;
    for (const auto& cm : rw::methods(units)) {
        const py::ref<ir::ClassDecl>& cls = std::get<0>(cm);
        const py::ref<ir::MethodDecl>& m = std::get<1>(cm);
        for (ir::Nodes ss : rw::stmt_lists(m->body)) {
            if (!ss) {
                throw py::TypeError("object of type 'NoneType' has no len()");
            }
            for (int64_t i = 0; i < py::len(*ss) - 1; ++i) {
                ir::NodeP loc_n = (*ss)[i];
                ir::NodeP nxt_n = (*ss)[i + 1];
                auto loc = as<ir::Local>(loc_n);
                auto nxt = as<ir::If>(nxt_n);
                if (!(loc && loc->init && nxt)) {
                    continue;
                }
                ir::SymP v = loc->sym;
                if (rw::is_call(loc->init, py::str("getRandomSoundFromSoundPool"), py::str("soundPoolSounds"))) {
                    ir::NodeP cond = rw::strip(nxt->cond);
                    ir::NodeP rest;
                    if (rw::is_null_test(cond, v)) {
                        rest = nullptr;
                    } else if (as<ir::Binary>(cond) && as<ir::Binary>(cond)->op && *as<ir::Binary>(cond)->op == py::str("&&") &&
                               rw::is_null_test(as<ir::Binary>(cond)->left, v)) {
                        rest = as<ir::Binary>(cond)->right;     // keep the volume gate
                    } else {
                        continue;
                    }
                    // the sound name: the XACT cue
                    auto call = as<ir::Call>(rw::strip(loc->init));
                    if (!call->args) {
                        throw py::TypeError("'NoneType' object is not subscriptable");
                    }
                    if (py::len(*call->args) == 0) {
                        throw py::IndexError("list index out of range");
                    }
                    ir::NodeP sound = (*call->args)[0];
                    loc->init = rw::null();                    // no query: the empty pool's lookup hangs on console
                    nxt->cond = rest ? rest : rw::true_();

                    auto route = [&](const ir::NodeP& n) -> ir::NodeP {
                        auto sel = as<ir::Select>(n);
                        if (sel && rw::is_name(sel->obj, v) && sel->sym) {
                            if (sel->sym->name == py::str("soundUrl")) {
                                return rw::null();
                            }
                            if (sel->sym->name == py::str("soundName")) {
                                return sound;
                            }
                        }
                        return nullptr;
                    };
                    rw::replace(nxt, route);
                    per[cls->name][0] += 1;
                } else if (rw::is_call(loc->init, py::str("getRandomSound"), py::str("soundPoolMusic")) &&
                           rw::is_null_test(nxt->cond, v) && !nxt->orelse && as<ir::Block>(nxt->then)) {
                    nxt->cond = rw::true_();                   // play("BgMusic") starts the XACT playlist: always
                    auto then = as<ir::Block>(nxt->then);
                    if (!then->stmts) {
                        none_iter();
                    }
                    py::list<ir::NodeP> rebuilt;
                    for (const ir::NodeP& s : *then->stmts) {
                        if (rw::mentions(s, v)) {
                            rebuilt.append(rw::if_(rw::binary(py::str("!="), rw::name(v, loc->type), rw::null(), ir::BOOL), s));
                        } else {
                            rebuilt.append(s);
                        }
                    }
                    then->stmts = rebuilt;
                    per[cls->name][1] += 1;
                }
            }
        }
    }
    Result res;
    for (const auto& kv : per) {
        if (!kv.first) {
            throw py::TypeError("'<' not supported between instances of 'NoneType' and 'str'");   // sorted / c + '.cpp'
        }
    }
    for (const auto& kv : per) {
        res.files.append(Touched{*kv.first + py::str(".cpp"), kv.second[0], kv.second[1]});
        res.sites += kv.second[0] + kv.second[1];
    }
    return res;
}

py::list<py::str> stranded(const py::str& out_dir) {
    py::list<py::str> out;
    for (const py::str& f : py::sorted(py::os::listdir(out_dir))) {
        if (f.endswith(py::str(".cpp"))) {
            py::str text;
            try {
                text = py::open(py::os::path::join(out_dir, f), py::str("r"), py::str("utf-8"), py::str("replace")).read_text();
            } catch (const py::OSError&) {                 // (IOError, OSError)
                continue;
            }
            if (_SURVIVOR.search(text) || _MUSIC_SURVIVOR.search(text)) {
                out.append(f);
            }
        }
    }
    return out;
}

}  // namespace codebreaker::java::projects::minecraft::sound_fix
