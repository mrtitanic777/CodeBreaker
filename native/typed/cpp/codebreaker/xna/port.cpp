// codebreaker/xna/port.cpp -- typed C++ of codebreaker.xna.port (see port.hpp).

#include "codebreaker/xna/port.hpp"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include <algorithm>
#include <thread>

#include "codebreaker/formats/xbox/xma.hpp"
#include "codebreaker/formats/xbox/xwb.hpp"
#include "codebreaker/xna/mgfx.hpp"
#include "py_json.hpp"
#include "py_settings.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::xna::port {

namespace d = run::detail;
using d::jbool;
using d::jdict;
using d::jint;
using d::jlist;
using d::jnone;
using d::jstr;

const py::list<py::str> STUBBED = {py::str("GamerServices"), py::str("Net"), py::str("Storage"), py::str("Avatar")};
const py::list<py::str> HAND_WRITTEN = {py::str("Guide"),         py::str("Gamer"),          py::str("SignedInGamer"),
                                        py::str("SignedInGamerCollection"), py::str("GamerServicesDispatcher"),
                                        py::str("StorageDevice"), py::str("StorageContainer")};
const py::str MONOGAME("3.8.1.303");
const py::list<py::str> XACT_TYPES = {py::str("AudioEngine"), py::str("WaveBank"), py::str("SoundBank"), py::str("Cue"),
                                      py::str("AudioCategory")};
const py::list<py::str> LIFECYCLE = {py::str("Update"),      py::str("Draw"),     py::str("Initialize"), py::str("LoadContent"),
                                     py::str("UnloadContent"), py::str("Dispose"), py::str("BeginDraw"),  py::str("EndDraw")};
const py::dict<py::str, py::str> RENAMED_PARAMETERS = {{py::str("mipMap"), py::str("mipmap")},
                                                       {py::str("vector1"), py::str("value1")},
                                                       {py::str("vector2"), py::str("value2")}};

namespace {

const py::re::Pattern _ERROR(
    py::str("^(?P<file>[A-Za-z]:[^(]+\\.cs)\\((?P<line>\\d+),(?P<col>\\d+)\\): error (?P<code>CS\\d+): (?P<text>.*?)(?: \\[.*\\])?$"), 0);

py::str lifecycle_alternatives_() { return py::join(py::str("|"), LIFECYCLE); }

const py::re::Pattern _GAME_CLASS(py::str("\\bclass\\s+(\\w+)\\s*:\\s*(?:Microsoft\\.Xna\\.Framework\\.)?Game\\b"), 0);
const py::re::Pattern _BASE_CAST(
    py::str("\\(\\((?:[\\w.]+\\.)?(?:GameComponent|DrawableGameComponent|Game|IDisposable)\\)\\s*this\\)\\.(") + lifecycle_alternatives_() +
        py::str(")\\("),
    0);
const py::re::Pattern _NEW_EFFECT(py::str("\\bnew\\s+(?:Microsoft\\.Xna\\.Framework\\.Graphics\\.)?Effect\\s*\\("), 0);
const py::re::Pattern _DERIVES_EFFECT(py::str("\\bclass\\s+\\w+\\s*:\\s*(?:Microsoft\\.Xna\\.Framework\\.Graphics\\.)?Effect\\b"), 0);
const py::re::Pattern _BASE_CALL(py::str("\\)\\s*:\\s*base\\s*\\("), 0);

// open(path, encoding="utf-8-sig").read()
py::str read_sig(const py::str& path) { return d::read_text_sig(path, false, true); }

std::vector<std::pair<std::string, std::string>> dotnet_env_() {
    return {{"DOTNET_ROLL_FORWARD", "LatestMajor"},
            {"DOTNET_CLI_TELEMETRY_OPTOUT", "1"},
            {"DOTNET_NOLOGO", "1"},
            {"MSBUILDDISABLENODEREUSE", "1"}};
}

py::subprocess::RunOptions capture_() {
    py::subprocess::RunOptions o;
    o.encoding = "cp1252";
    return o;
}

py::list<py::str> split_lines_n_(const py::str& text) { return py::split(text, py::str("\n")); }

}  // namespace

py::str HERE() { return py::os::path::join(py::settings::checkout_root(), py::str("codebreaker"), py::str("xna")); }
py::str RUNTIME() { return py::os::path::join(HERE(), py::str("runtime")); }
py::str APISTUBS() { return py::os::path::join(HERE(), py::str("apistubs")); }

// ---- the generated XNA API stubs --------------------------------------------------------------------------------
std::optional<py::str> _xna_assembly(const py::str& name) {
    py::str windir = py::os::environ_::get(py::str("WINDIR")).value_or(py::str("C:\\Windows"));
    py::str root = py::os::path::join(windir, py::str("Microsoft.NET"), py::str("assembly"));
    py::str wanted = py::str("microsoft.xna.framework.") + name + py::str(".dll");
    for (const auto& w : py::os::walk(root)) {
        for (const py::str& f : std::get<2>(w))
            if (f.lower() == wanted.lower()) return py::os::path::join(std::get<0>(w), f);
    }
    return std::nullopt;
}

py::str _stub_tool() {
    py::str out = d::state({py::str("xna"), py::str("apistubs")});
    py::os::makedirs(out, 0777, true);
    py::str exe = py::os::path::join(out, py::str("apistubs.exe"));
    double src = std::max(py::os::path::getmtime(py::os::path::join(APISTUBS(), py::str("Program.cs"))),
                          py::os::path::getmtime(py::os::path::join(APISTUBS(), py::str("apistubs.csproj"))));
    if (!py::os::path::isfile(exe) || py::os::path::getmtime(exe) < src) {
        d::EnvGuard env(dotnet_env_());
        py::subprocess::CompletedProcess r = py::subprocess::run(
            py::list<py::str>{py::str("dotnet"), py::str("build"), APISTUBS(), py::str("-c"), py::str("Release"), py::str("-o"), out,
                              py::str("--nologo"), py::str("-v"), py::str("q")},
            capture_());
        if (r.returncode != 0)
            throw py::RuntimeError(py::str("apistubs did not build: ").utf8() + (*r.stdout_ + *r.stderr_).slice(-800, py::none).utf8());
    }
    return exe;
}

void stubs(const py::str& out_dir) {
    py::str cache = d::state({py::str("xna"), py::str("stubs")});
    py::os::makedirs(cache, 0777, true);
    std::optional<py::str> tool;
    py::os::makedirs(out_dir, 0777, true);
    for (const py::str& name : STUBBED) {
        py::str cached = py::os::path::join(cache, name + py::str(".stubs.cs"));
        if (!py::os::path::isfile(cached)) {
            std::optional<py::str> asm_ = _xna_assembly(name);
            if (!asm_)
                throw py::RuntimeError(py::percent(py::str("XNA Game Studio 4.0's Microsoft.Xna.Framework.%s.dll is not installed (the stubs are "
                                                           "generated from its metadata)"),
                                                   name)
                                           .utf8());
            if (!tool) tool = _stub_tool();
            py::list<py::str> args{*tool, *asm_, cached, py::str("Microsoft.Xna.Framework")};
            args.extend(HAND_WRITTEN);
            d::EnvGuard env(dotnet_env_());
            py::subprocess::CompletedProcess r = py::subprocess::run(args, capture_());
            if (r.returncode != 0)
                throw py::RuntimeError(py::percent(py::str("apistubs failed on %s: %s"), name, (!r.stderr_->empty() ? *r.stderr_ : *r.stdout_)).utf8());
        }
        py::shutil::copy2(cached, py::os::path::join(out_dir, name + py::str(".stubs.cs")));
    }
}

// ---- source rewrites --------------------------------------------------------------------------------------------
py::set<py::str> declared_types(const py::str& folder) {
    py::set<py::str> names;
    py::re::Pattern rx(py::str("\\b(?:class|struct|enum|interface)\\s+(\\w+)"), 0);
    for (const auto& w : py::os::walk(folder)) {
        for (const py::str& f : std::get<2>(w)) {
            if (f.endswith(py::str(".cs"))) {
                py::str text = d::read_text_sig(py::os::path::join(std::get<0>(w), f), true, true);
                for (const py::str& n : rx.findall(text)) names.add(n);
            }
        }
    }
    return names;
}

std::tuple<py::str, py::list<py::str>> rewrite(const py::str& text0, const py::set<py::str>& own) {
    py::str text = text0;
    py::list<py::str> changed;
    std::optional<py::re::Match> m = _GAME_CLASS.search(text);
    if (m) {
        py::str name = m->group(1);
        py::re::Pattern ctor(py::str("\\b(?:public|protected|internal|private)?\\s*") + py::re::escape(name) +
                                 py::str("\\s*\\([^)]*\\)\\s*(?::\\s*base\\s*\\([^)]*\\)\\s*)?\\{"),
                             0);
        std::optional<py::re::Match> cm = ctor.search(text, m->end());
        if (cm && !text.contains(py::str("XnaPort.Runtime.Install"))) {
            text = text.slice(0, cm->end()) + py::str("\n\t\tXnaPort.Runtime.Install(this);") + text.slice(cm->end(), py::none);
            changed.append(py::percent(py::str("Install in %s()"), name));
        }
    }
    int64_t n;
    std::tie(text, n) = py::re::subn(py::str("\\bnew\\s+(?:Microsoft\\.Xna\\.Framework\\.)?GraphicsDeviceManager\\s*\\(\\s*this\\s*\\)"),
                                     py::str("XnaPort.Runtime.Graphics(this)"), text);
    if (n) changed.append(py::percent(py::str("GraphicsDeviceManager x%d"), n));
    py::list<py::str> xact;
    for (const py::str& t : XACT_TYPES)
        if (!own.contains(t)) xact.append(t);
    bool any = false;
    if (!xact.empty() && text.contains(py::str("Microsoft.Xna.Framework.Audio"))) {
        for (const py::str& t : xact)
            if (py::re::search(py::str("\\b") + t + py::str("\\b"), text)) any = true;
    }
    if (any) {
        py::str aliases;
        for (const py::str& t : xact) aliases = aliases + py::percent(py::str("using %s = XnaPort.Xact.%s;\n"), t, t);
        text = py::re::Pattern(py::str("(?m)^(using Microsoft\\.Xna\\.Framework\\.Audio;\\s*\\n)"), 0)
                   .sub([&](const py::re::Match& mm) { return mm.group(1) + aliases; }, text, 1);
        text = py::re::sub(py::str("\\bMicrosoft\\.Xna\\.Framework\\.Audio\\.(") + py::join(py::str("|"), xact) + py::str(")\\b"),
                           py::str("XnaPort.Xact.\\1"), text);
        changed.append(py::str("XACT aliases"));
    }
    // the game's own content managers read through the runtime's, which knows the converted content
    int64_t n2;
    std::tie(text, n) = py::re::subn(
        py::str("(\\bclass\\s+\\w+\\s*(?:<[^>]*>)?\\s*:\\s*)(?:Microsoft\\.Xna\\.Framework\\.Content\\.)?ContentManager\\b"),
        py::str("\\1XnaPort.ContentManager"), text);
    std::tie(text, n2) = py::re::subn(py::str("\\bnew\\s+(?:Microsoft\\.Xna\\.Framework\\.Content\\.)?ContentManager\\s*\\("),
                                      py::str("new XnaPort.ContentManager("), text);
    if (n + n2) changed.append(py::percent(py::str("content manager x%d"), n + n2));
    std::tie(text, n) = _BASE_CAST.subn(py::str("base.\\1("), text);
    if (n) changed.append(py::percent(py::str("base calls x%d"), n));
    // an effect made from the game's own bytes (Xbox 360 microcode) gets the PC's compiled one instead
    std::tie(text, n) = _effect_bytes(text);
    if (n) changed.append(py::percent(py::str("effect bytes x%d"), n));
    // a stock effect's ShaderIndex picks its technique on the PC (the effect has one per value)
    if (_DERIVES_EFFECT.search(text)) {
        std::optional<py::re::Match> mm = py::re::search(
            py::str("\\b(\\w+)\\s*=\\s*(?:base\\.|this\\.)?Parameters\\s*\\[\\s*\\\"ShaderIndex\\\"\\s*\\]"), text);
        if (mm) {
            std::tie(text, n) = py::re::subn(py::str("\\b") + py::re::escape(mm->group(1)) + py::str("\\.SetValue\\s*\\("),
                                             py::percent(py::str("XnaPort.Effects.SetShaderIndex(this, %s, "), mm->group(1)), text);
            if (n) changed.append(py::percent(py::str("ShaderIndex x%d"), n));
        }
    }
    // the controller is read through the runtime's, which adds the keyboard and scripted presses
    if (!own.contains(py::str("GamePad"))) {
        std::tie(text, n) = py::re::subn(py::str("\\b(?:Microsoft\\.Xna\\.Framework\\.Input\\.)?GamePad\\.GetState\\s*\\("),
                                         py::str("XnaPort.Input.GetState("), text);
        if (n) changed.append(py::percent(py::str("GamePad.GetState x%d"), n));
    }
    return {text, changed};
}

std::tuple<py::str, int64_t> _effect_bytes(const py::str& text0) {
    py::str text = text0;
    std::vector<int64_t> spots;
    for (const py::re::Match& m : _NEW_EFFECT.finditer(text)) spots.push_back(m.end() - 1);
    if (_DERIVES_EFFECT.search(text))
        for (const py::re::Match& m : _BASE_CALL.finditer(text)) spots.push_back(m.end() - 1);
    int64_t n = 0;
    std::sort(spots.begin(), spots.end(), std::greater<int64_t>());
    for (int64_t paren : spots) {
        auto [spans, _end] = _args(text, paren);
        if (!spans || spans->empty() || py::len(*spans) != 2) continue;
        auto [a, b] = (*spans)[1];
        py::str arg = text.slice(a, b).strip();
        if (arg.startswith(py::str("XnaPort.Effects.ForPC("))) continue;
        int64_t lead = py::len(text.slice(a, b)) - py::len(text.slice(a, b).lstrip());
        text = text.slice(0, a + lead) + py::str("XnaPort.Effects.ForPC(") + arg + py::str(")") + text.slice(b, py::none);
        n += 1;
    }
    return {text, n};
}

int64_t fix_access(const py::list<BuildError>& errors) {
    int64_t n = 0;
    for (const BuildError& e : errors) {
        if (e.code == py::str("CS0507") && e.text.contains(py::str("'protected internal' inherited member"))) {
            py::list<py::str> lines = split_lines_n_(read_sig(e.file));
            int64_t ln = py::int_parse(e.line, 10) - 1;
            py::str fixed = py::re::Pattern(py::str("\\bprotected\\s+override\\b"), 0).sub(py::str("protected internal override"), lines[ln], 1);
            if (fixed != lines[ln]) {
                lines[ln] = fixed;
                n += 1;
                d::write_text(e.file, py::join(py::str("\n"), lines));
            }
        }
    }
    return n;
}

std::tuple<std::optional<py::list<std::tuple<int64_t, int64_t>>>, int64_t> _args(const py::str& text, int64_t open_paren) {
    int64_t depth = 0, start = open_paren + 1;
    py::list<std::tuple<int64_t, int64_t>> spans;
    std::optional<py::str> quote;
    for (int64_t i = open_paren; i < py::len(text); i++) {
        py::str c = text[i];
        if (quote) {
            if (c == *quote && text[i - 1] != py::str("\\")) quote.reset();
            continue;
        }
        if (c == py::str("\"") || c == py::str("'")) {
            quote = c;
        } else if (c == py::str("(") || c == py::str("[") || c == py::str("{")) {
            depth += 1;
        } else if (c == py::str(")") || c == py::str("]") || c == py::str("}")) {
            depth -= 1;
            if (depth == 0) {
                if (!text.slice(start, i).strip().empty()) spans.append({start, i});
                return {spans, i + 1};
            }
        } else if (c == py::str(",") && depth == 1) {
            spans.append({start, i});
            start = i + 1;
        }
    }
    return {std::nullopt, -1};
}

int64_t _edit_lines(const py::list<BuildError>& errors, const py::str& code, const py::str& wanted,
                    const std::function<py::str(const py::str&)>& edit) {
    std::vector<std::pair<py::str, std::vector<int64_t>>> by_file;   // file -> the set of lines (kept sorted, unique)
    for (const BuildError& e : errors) {
        if (e.code == code && e.text.contains(wanted)) {
            auto it = std::find_if(by_file.begin(), by_file.end(), [&](const auto& p) { return p.first == e.file; });
            if (it == by_file.end()) {
                by_file.push_back({e.file, {}});
                it = by_file.end() - 1;
            }
            int64_t ln = py::int_parse(e.line, 10);
            if (std::find(it->second.begin(), it->second.end(), ln) == it->second.end()) it->second.push_back(ln);
        }
    }
    int64_t n = 0;
    for (auto& pf : by_file) {
        py::list<py::str> lines = split_lines_n_(read_sig(pf.first));
        std::sort(pf.second.begin(), pf.second.end());
        for (int64_t ln : pf.second) {
            py::str neu = edit(lines[ln - 1]);
            if (neu != lines[ln - 1]) {
                lines[ln - 1] = neu;
                n += 1;
            }
        }
        d::write_text(pf.first, py::join(py::str("\n"), lines));
    }
    return n;
}

int64_t fix_color(const py::list<BuildError>& errors) {
    py::re::Pattern rx(py::str("\\bnew\\s+(?:Microsoft\\.Xna\\.Framework\\.)?Color\\s*\\("), 0);
    auto edit = [&](const py::str& line) -> py::str {
        py::str out = line;
        int64_t i = 0;
        while (true) {
            std::optional<py::re::Match> m = rx.search(out.slice(i, py::none));
            if (!m) return out;
            int64_t at = i + m->end() - 1;
            auto [spans, end] = _args(out, at);
            if (!spans || (py::len(*spans) != 3 && py::len(*spans) != 4) || out.slice(at, end).contains(py::str("(int)("))) {
                i = at + 1;
                continue;
            }
            std::vector<py::str> parts;
            for (const auto& ab : *spans) parts.push_back(py::str("(int)(") + out.slice(std::get<0>(ab), std::get<1>(ab)).strip() + py::str(")"));
            py::str body = py::join(py::str(", "), parts);
            out = out.slice(0, at + 1) + body + py::str(")") + out.slice(end, py::none);
            i = at + py::len(body) + 2;
        }
    };
    return _edit_lines(errors, py::str("CS0121"), py::str("Color.Color(byte"), edit);
}

int64_t fix_vertex_binding(const py::list<BuildError>& errors) {
    py::re::Pattern rx(py::str("new\\s+VertexBufferBinding\\s*\\[[^\\]]*\\]\\s*\\{([^}]*)\\}"), 0);
    auto edit = [&](const py::str& line) -> py::str {
        std::optional<py::re::Match> m = rx.search(line);
        if (!m) return line;
        std::vector<py::str> items;
        for (const py::str& x : py::split(m->group(1), py::str(","))) {
            if (x.strip().empty()) continue;
            items.push_back(x.strip().startswith(py::str("new ")) ? x.strip() : py::percent(py::str("new VertexBufferBinding(%s)"), x.strip()));
        }
        return line.slice(0, m->start(1)) + py::str(" ") + py::join(py::str(", "), items) + py::str(" ") + line.slice(m->end(1), py::none);
    };
    return _edit_lines(errors, py::str("CS0029"), py::str("to 'Microsoft.Xna.Framework.Graphics.VertexBufferBinding'"), edit);
}

py::dict<py::str, py::list<std::tuple<py::str, py::str>>> extension_methods(const py::str& folder) {
    py::dict<py::str, py::list<std::tuple<py::str, py::str>>> out;
    py::re::Pattern ns_rx(py::str("^\\s*namespace\\s+([\\w.]+)"), py::re::M);
    py::re::Pattern cls_rx(py::str("\\bstatic\\s+(?:partial\\s+)?class\\s+(\\w+)"), 0);
    py::re::Pattern ext_rx(
        py::str("\\bstatic\\s+[\\w<>\\[\\],. ]+?\\s+(\\w+)\\s*(?:<[^>]*>)?\\s*\\(\\s*this\\s+(?:ref\\s+)?([\\w.<>\\[\\]]+)"), 0);
    for (const auto& w : py::os::walk(folder)) {
        for (const py::str& f : std::get<2>(w)) {
            if (!f.endswith(py::str(".cs"))) continue;
            py::str text = d::read_text_sig(py::os::path::join(std::get<0>(w), f), true, true);
            std::optional<py::re::Match> ns = ns_rx.search(text);
            for (const py::re::Match& cm : cls_rx.finditer(text)) {
                py::str cls = (ns ? ns->group(1) + py::str(".") : py::str("")) + cm.group(1);
                int64_t end = text.find(py::str("\nclass "), cm.end());
                int64_t endpos = end > 0 ? end : py::len(text);
                // ext_rx.finditer(text, cm.end(), endpos): searches from pos, each match found after the one before
                int64_t pos = cm.end();
                while (pos <= endpos) {
                    std::optional<py::re::Match> em = ext_rx.search(text, pos, endpos);
                    if (!em) break;
                    out.setdefault(em->group(1), py::list<std::tuple<py::str, py::str>>()).append({py::str("global::") + cls, em->group(2)});
                    pos = em->end() > pos ? em->end() : pos + 1;
                }
            }
        }
    }
    return out;
}

int64_t fix_shadowed_extensions(const py::list<BuildError>& errors, const py::dict<py::str, py::list<std::tuple<py::str, py::str>>>& extensions) {
    if (extensions.empty()) return 0;
    std::vector<py::str> escaped;
    for (const auto& kv : extensions.items()) escaped.push_back(py::re::escape(std::get<0>(kv)));
    py::re::Pattern rx(py::str("((?:[A-Za-z_]\\w*)(?:\\.[A-Za-z_]\\w*|\\[[^\\]]*\\])*)\\.(") + py::join(py::str("|"), escaped) + py::str(")\\s*\\("), 0);
    auto edit = [&](const py::str& line) -> py::str {
        py::str out = line;
        int64_t i = 0;
        while (true) {
            std::optional<py::re::Match> m = rx.search(out, i);
            if (!m) return out;
            py::str receiver = m->group(1), name = m->group(2);
            py::str cls = std::get<0>(extensions.at(name)[0]);
            py::list<py::str> rparts = py::split(receiver, py::str("."));
            py::list<py::str> cparts = py::split(cls, py::str("."));
            if (rparts[0] == py::str("this") || rparts[0] == py::str("base") || receiver.endswith(cparts[-1])) {
                i = m->end();
                continue;
            }
            int64_t at = m->end() - 1;
            auto [spans, end] = _args(out, at);
            if (!spans) return out;
            std::vector<py::str> args{receiver};
            for (const auto& ab : *spans) args.push_back(out.slice(std::get<0>(ab), std::get<1>(ab)).strip());
            py::str call = py::percent(py::str("%s.%s(%s)"), cls, name, py::join(py::str(", "), args));
            out = out.slice(0, m->start()) + call + out.slice(end, py::none);
            i = m->start() + py::len(call);
        }
    };
    int64_t n = 0;
    n += _edit_lines(errors, py::str("CS0029"), py::str("type 'void'"), edit);
    n += _edit_lines(errors, py::str("CS1503"), py::str("from 'void'"), edit);
    n += _edit_lines(errors, py::str("CS0023"), py::str("type 'void'"), edit);
    return n;
}

int64_t fix_named_arguments(const py::list<BuildError>& errors) {
    int64_t n = 0;
    for (const auto& kv : RENAMED_PARAMETERS.items()) {
        py::str old = std::get<0>(kv), neu = std::get<1>(kv);
        n += _edit_lines(errors, py::str("CS1739"), py::percent(py::str("named '%s'"), old), [&](const py::str& line) {
            return py::re::sub(py::str("\\b") + old + py::str("\\s*:"), neu + py::str(":"), line);
        });
    }
    return n;
}

int64_t fix_moved_members(const py::list<BuildError>& errors) {
    auto stencil = [](const py::str& line0) -> py::str {
        py::str line = py::re::sub(py::str("([\\w.\\[\\]()]+)\\.ReferenceStencil\\s*=\\s*([^;]+);"), py::str("XnaPort.Compat.SetReferenceStencil(\\1, \\2);"), line0);
        return py::re::sub(py::str("([\\w.\\[\\]()]+)\\.ReferenceStencil\\b"), py::str("XnaPort.Compat.GetReferenceStencil(\\1)"), line);
    };
    auto cube_face = [](const py::str& line) -> py::str {
        return py::re::sub(py::str("([\\w.\\[\\]()]+)\\.CubeMapFace\\b"), py::str("((Microsoft.Xna.Framework.Graphics.CubeMapFace)\\1.ArraySlice)"), line);
    };
    return _edit_lines(errors, py::str("CS1061"), py::str("definition for 'ReferenceStencil'"), stencil) +
           _edit_lines(errors, py::str("CS1061"), py::str("definition for 'CubeMapFace'"), cube_face);
}

int64_t fix_ref_out(const py::list<BuildError>& errors) {
    // {file: {(line, col): "ref" | "out"}}, each in the order first met
    struct Site { int64_t ln, col; py::str kw; };
    std::vector<std::pair<py::str, std::vector<Site>>> want;
    py::re::Pattern rx(py::str("must be passed with the '(ref|out)' keyword"), 0);
    for (const BuildError& e : errors) {
        if (e.code == py::str("CS1620")) {
            std::optional<py::re::Match> m = rx.search(e.text);
            if (m) {
                auto it = std::find_if(want.begin(), want.end(), [&](const auto& p) { return p.first == e.file; });
                if (it == want.end()) {
                    want.push_back({e.file, {}});
                    it = want.end() - 1;
                }
                int64_t ln = py::int_parse(e.line, 10), col = py::int_parse(e.col, 10);
                auto s = std::find_if(it->second.begin(), it->second.end(), [&](const Site& x) { return x.ln == ln && x.col == col; });
                if (s == it->second.end()) it->second.push_back({ln, col, m->group(1)});
                else s->kw = m->group(1);
            }
        }
    }
    int64_t n = 0;
    for (const auto& pf : want) {
        py::list<py::str> lines = split_lines_n_(read_sig(pf.first));
        for (const Site& site : pf.second) {
            py::str line = lines[site.ln - 1];
            int64_t at = site.col - 1;
            bool done = false;
            for (const char* other : {"ref ", "out ", "in "}) {
                py::str o(other);
                if (line.slice(at, at + py::len(o)) == o) {
                    lines[site.ln - 1] = line.slice(0, at) + site.kw + py::str(" ") + line.slice(at + py::len(o), py::none);
                    n += 1;
                    done = true;
                    break;
                }
            }
            if (!done) {
                if (line.slice(at, py::none).startswith(py::str("ref")) || line.slice(at, py::none).startswith(py::str("out"))) {
                    lines[site.ln - 1] = line.slice(0, at) + site.kw + line.slice(at + 3, py::none);
                    n += 1;
                }
            }
        }
        d::write_text(pf.first, py::join(py::str("\n"), lines));
    }
    return n;
}

// ---- content ----------------------------------------------------------------------------------------------------
py::dict<py::str, int64_t> convert_xact(const py::str& game_dir, const py::str& converted, const Hooks& hooks) {
    py::dict<py::str, int64_t> counts;
    counts[py::str("wave banks")] = 0;
    counts[py::str("sound banks")] = 0;
    for (const auto& w : py::os::walk(game_dir)) {
        const py::str& base = std::get<0>(w);
        for (const py::str& f : std::get<2>(w)) {
            py::str low = f.lower(), src = py::os::path::join(base, f);
            py::str rel = py::os::path::relpath(base, game_dir);
            if (low.endswith(py::str(".xwb"))) {
                py::str out = py::os::path::join(converted, rel, std::get<0>(py::os::path::splitext(f)));
                if (!py::os::path::isfile(py::os::path::join(out, py::str("bank.txt")))) {
                    formats::xbox::xma::decode_bank(src, out);
                    py::bytes raw;
                    {
                        py::file h = py::open(src, py::str("rb"));
                        raw = h.read();
                        h.close();
                    }
                    d::write_text(py::os::path::join(out, py::str("bank.txt")), formats::xbox::xwb::XWB(raw).bank_name);
                }
                counts[py::str("wave banks")] += 1;
            } else if (low.endswith(py::str(".xsb"))) {
                py::os::makedirs(py::os::path::join(converted, rel), 0777, true);
                d::write_json(py::os::path::join(converted, rel, std::get<0>(py::os::path::splitext(f)) + py::str(".cues.json")), hooks.xsb_read(src));
                counts[py::str("sound banks")] += 1;
            }
        }
    }
    return counts;
}

namespace {
// mgfx::Parameter from the JSON a `.fx.json` holds (a key left out is None / empty)
mgfx::Parameter parameter_(const Json& j) {
    auto get = [&](const char* k) -> const Json* { return j.dict.contains(py::str(k)) ? &j.dict.at(py::str(k)) : nullptr; };
    mgfx::Parameter p;
    if (auto* v = get("name")) p.name = v->s;
    if (auto* v = get("semantic")) if (v->kind == Json::Str) p.semantic = v->s;
    if (auto* v = get("type")) p.type = v->s;
    if (auto* v = get("cls")) p.cls = v->s;
    auto num = [&](const char* k) -> std::optional<int64_t> {
        const Json* v = get(k);
        if (!v || v->kind == Json::None) return std::nullopt;
        return v->kind == Json::Float ? static_cast<int64_t>(v->d) : v->i;
    };
    p.rows = num("rows");
    p.columns = num("columns");
    p.elements = num("elements");
    if (auto* v = get("value")) {
        if (v->kind == Json::List) {
            py::list<double> vals;
            for (const Json& x : v->list) vals.append(x.kind == Json::Float ? x.d : static_cast<double>(x.i));
            p.value = vals;
        }
    }
    if (auto* v = get("members")) {
        if (v->kind == Json::List)
            for (const Json& m : v->list) p.members.append(parameter_(m));
    }
    return p;
}
}  // namespace

EffectsReport compile_effects(const py::str& converted, const py::str& shaders) {
    py::str cache = d::state({py::str("xna"), py::str("mgfxo")});
    py::os::makedirs(cache, 0777, true);
    py::os::makedirs(shaders, 0777, true);
    std::optional<py::str> tool = py::shutil::which(py::str("mgfxc"));
    EffectsReport report;
    for (const auto& w : py::os::walk(converted)) {
        for (const py::str& f : std::get<2>(w)) {
            if (!f.endswith(py::str(".fx"))) continue;
            py::str src = py::os::path::join(std::get<0>(w), f);
            py::str text = d::read_text_sig(src, false, false);
            py::str first = std::get<0>(py::str(text).partition(py::str("\n")));
            if (!first.startswith(py::str("// sha1 "))) continue;
            py::str digest = first.slice(py::len(py::str("// sha1 ")), py::none).strip();
            py::str built = py::os::path::join(cache, py::hashlib::sha1(text.encode("utf-8")).hexdigest() + py::str(".mgfxo"));
            if (!py::os::path::isfile(built)) {
                if (!tool) {
                    report.failed[py::os::path::relpath(src, converted)] = py::str("mgfxc is not installed (NOT_IN_REPO.md)");
                    continue;
                }
                d::EnvGuard env(dotnet_env_());
                py::subprocess::CompletedProcess r =
                    py::subprocess::run(py::list<py::str>{*tool, src, built, py::str("/Profile:OpenGL")}, capture_());
                if (r.returncode != 0 || !py::os::path::isfile(built)) {
                    report.failed[py::os::path::relpath(src, converted)] = (*r.stdout_ + *r.stderr_).strip().slice(-600, py::none);
                    continue;
                }
            }
            py::bytes data;
            {
                py::file h = py::open(built, py::str("rb"));
                data = h.read();
                h.close();
            }
            py::str params = src + py::str(".json");
            if (py::os::path::isfile(params)) {
                Json j = knowledge::sources::_json(params);
                py::list<mgfx::Parameter> ps;
                for (const Json& x : j.list) ps.append(parameter_(x));
                data = mgfx::add_parameters(data, ps);
            }
            {
                py::file h = py::open(py::os::path::join(shaders, digest + py::str(".mgfxo")), py::str("wb"));
                h.write(data);
                h.close();
            }
            report.compiled += 1;
        }
    }
    return report;
}

int64_t convert_songs(const py::str& game_dir, const py::str& converted) {
    int64_t n = 0;
    for (const auto& w : py::os::walk(game_dir)) {
        const py::str& base = std::get<0>(w);
        for (const py::str& f : std::get<2>(w)) {
            if (!f.lower().endswith(py::str(".wma"))) continue;
            py::str out = py::os::path::join(converted, py::os::path::relpath(base, game_dir), std::get<0>(py::os::path::splitext(f)) + py::str(".ogg"));
            if (!py::os::path::isfile(out)) {
                py::os::makedirs(py::os::path::dirname(out), 0777, true);
                py::subprocess::CompletedProcess r = py::subprocess::run(
                    py::list<py::str>{formats::xbox::xma::_need_ffmpeg(), py::str("-y"), py::str("-hide_banner"), py::str("-loglevel"), py::str("error"),
                                      py::str("-i"), py::os::path::join(base, f), py::str("-c:a"), py::str("libvorbis"), py::str("-q:a"), py::str("5"), out},
                    capture_());
                if (r.returncode != 0)
                    throw py::RuntimeError(py::percent(py::str("ffmpeg could not convert %s: %s"), f, r.stderr_->strip().slice(-300, py::none)).utf8());
            }
            n += 1;
        }
    }
    return n;
}

// ---- the project ------------------------------------------------------------------------------------------------
py::str _csproj(const py::str& name, const py::str& /*exe_rel*/, const py::list<py::str>& /*assemblies*/, const py::list<py::str>& defines) {
    py::str sources(
        "    <Compile Include=\"src/**/*.cs\" Exclude=\"src/**/obj/**;src/**/bin/**;src/**/AssemblyInfo.cs\" />");
    py::str def;
    for (const py::str& x : defines) def = def + py::str(";") + x;
    return py::str(
               "<Project Sdk=\"Microsoft.NET.Sdk\">\n"
               "  <!-- Written by CodeBreaker's xna port step (codebreaker/xna/port.py). -->\n"
               "  <PropertyGroup>\n"
               "    <OutputType>WinExe</OutputType>\n"
               "    <TargetFramework>net8.0</TargetFramework>\n"
               "    <RollForward>Major</RollForward>\n"
               "    <Nullable>disable</Nullable>\n"
               "    <AllowUnsafeBlocks>true</AllowUnsafeBlocks>\n"
               "    <GenerateAssemblyInfo>false</GenerateAssemblyInfo>\n"
               "    <EnableDefaultCompileItems>false</EnableDefaultCompileItems>\n"
               "    <AssemblyName>") +
           name + py::str("</AssemblyName>\n    <RootNamespace>") + name +
           py::str("</RootNamespace>\n    <DefineConstants>WINDOWS;XNAPORT") + def +
           py::str(
               "</DefineConstants>\n"
               "    <NoWarn>$(NoWarn);CS0108;CS0114;CS0162;CS0168;CS0169;CS0219;CS0414;CS0436;CS0649;CS0067;CS0618;CS1998;CA1416</NoWarn>\n"
               "  </PropertyGroup>\n"
               "  <ItemGroup>\n"
               "    <PackageReference Include=\"MonoGame.Framework.DesktopGL\" Version=\"") +
           MONOGAME +
           py::str(
               "\" />\n"
               "  </ItemGroup>\n"
               "  <ItemGroup>\n") +
           sources +
           py::str(
               "\n"
               "    <Compile Include=\"XnaPort/**/*.cs\" />\n"
               "    <Compile Include=\"Stubs/**/*.cs\" />\n"
               "    <Compile Include=\"Port/**/*.cs\" Exclude=\"Port/**/obj/**;Port/**/bin/**\" />\n"
               "  </ItemGroup>\n"
               "  <ItemGroup>\n"
               "    <None Include=\"game/**/*\" Exclude=\"game/**/*.dll;game/**/*.exe;game/**/*.pdb;game/*.xml\" CopyToOutputDirectory=\"PreserveNewest\" "
               "Link=\"%(RecursiveDir)%(Filename)%(Extension)\" />\n"
               "    <None Include=\"ContentConverted/**/*\" CopyToOutputDirectory=\"PreserveNewest\" "
               "Link=\"ContentConverted/%(RecursiveDir)%(Filename)%(Extension)\" />\n"
               "    <None Include=\"Content/**/*\" CopyToOutputDirectory=\"PreserveNewest\" "
               "Link=\"Content/%(RecursiveDir)%(Filename)%(Extension)\" />\n"
               "  </ItemGroup>\n"
               "</Project>\n");
}

std::optional<py::str> _overlay_dir(const base::converted::Project& project, const std::optional<py::str>& overlay) {
    if (overlay && !overlay->empty()) return overlay;
    py::str named = py::settings::get_str(py::str("xna_overlays"));
    py::str root = !named.empty() ? named : d::state({py::str("xna"), py::str("overlays")});
    py::str dir = py::os::path::join(root, project.name());
    return py::os::path::isdir(dir) ? std::optional<py::str>(dir) : std::nullopt;
}

void _copy_tree(const py::str& src, const py::str& dst) {
    for (const auto& w : py::os::walk(src)) {
        const py::str& base = std::get<0>(w);
        const py::list<py::str>& dirs = std::get<1>(w);
        // dirs[:] = [d for d in dirs if d not in ("obj", "bin", ".git")]
        std::vector<py::str> keep;
        for (const py::str& x : dirs)
            if (!(x == py::str("obj") || x == py::str("bin") || x == py::str(".git"))) keep.push_back(x);
        dirs.vec() = keep;
        py::str rel = py::os::path::relpath(base, src);
        py::os::makedirs(py::os::path::join(dst, rel), 0777, true);
        for (const py::str& f : std::get<2>(w)) {
            if (f == py::str("overlay.json")) continue;
            py::str a = py::os::path::join(base, f), b = py::os::path::join(dst, rel, f);
            if (py::os::path::isfile(b)) {
                if (py::os::path::getsize(a) == py::os::path::getsize(b) &&
                    static_cast<int64_t>(py::os::path::getmtime(a)) == static_cast<int64_t>(py::os::path::getmtime(b)))
                    continue;
            }
            py::shutil::copy2(a, b);
        }
    }
}

// XNA assemblies whose types MonoGame does not have; the port's stubs give them, in the program itself
const py::re::Pattern STUB_ASSEMBLIES(
    py::str(", Microsoft\\.Xna\\.Framework\\.(?:Avatar|GamerServices|Net|Storage)(?:, Version=[^\\],]*)?"
            "(?:, Culture=[^\\],]*)?(?:, PublicKeyToken=[^\\],]*)?"),
    0);

const py::str _REFLECTIVE("Microsoft.Xna.Framework.Content.ReflectiveReader`1[");

py::str port_reader_name(const py::str& name0, const py::str& program) {
    py::str name = STUB_ASSEMBLIES.sub([&](const py::re::Match&) { return py::str(", ") + program; }, name0);
    if (name.startswith(_REFLECTIVE))
        name = py::str("XnaPort.XnaReflectiveReader`1[") + name.slice(py::len(_REFLECTIVE), py::none) + py::str(", ") + program;
    return name;
}

int64_t port_reader_names(const py::str& game_dir, const py::str& program, const Hooks& hooks) {
    py::str stamp = py::os::path::join(py::os::path::dirname(game_dir), py::str("reader-names.stamp"));
    if (py::os::path::isfile(stamp)) return 0;
    int64_t n = 0;
    for (const auto& w : py::os::walk(game_dir)) {
        for (const py::str& f : std::get<2>(w)) {
            if (!f.lower().endswith(py::str(".xnb"))) continue;
            py::str path = py::os::path::join(std::get<0>(w), f);
            py::bytes raw;
            {
                py::file h = py::open(path, py::str("rb"));
                raw = h.read();
                h.close();
            }
            if (raw.slice(0, 3) != py::bytes("XNB")) continue;
            std::optional<py::bytes> neu = hooks.xnb_rename_readers(raw, [&](const py::str& nm) { return port_reader_name(nm, program); });
            if (neu) {
                py::file h = py::open(path, py::str("wb"));
                h.write(*neu);
                h.close();
                n += 1;
            }
        }
    }
    d::write_text(stamp, py::percent(py::str("%d\n"), n));
    return n;
}

std::tuple<py::str, py::str, Json> prepare(const base::converted::Project& project, const std::optional<py::str>& overlay, const Hooks& hooks) {
    py::str game_dir = py::os::path::join(project.assets, py::str("game"));
    py::list<py::str> games = run::assemblies(game_dir);
    if (games.empty())
        throw py::ValueError(py::percent(py::str("%s: no game assemblies (run `codebreaker xna` on the game first)"), project.root).utf8());
    py::str exe = games[0];
    for (const py::str& g : games)
        if (g.lower().endswith(py::str(".exe"))) {
            exe = g;
            break;
        }
    py::str exe_dir = py::os::path::dirname(exe);
    py::str exe_rel = py::os::path::relpath(exe_dir, game_dir);
    py::str name = std::get<0>(py::os::path::splitext(py::os::path::basename(exe))).replace(py::str(" "), py::str(""));
    py::str port = py::os::path::join(project.build, py::str("port"));
    py::os::makedirs(port, 0777, true);
    Json report = jdict({{py::str("rewrites"), jdict({})}, {py::str("content"), jdict({})}});
    // the game's C#, rewritten
    py::list<py::str> assemblies;
    py::set<py::str> own = declared_types(project.src);
    for (const py::str& asm_ : py::sorted(py::os::listdir(project.src))) {
        py::str src = py::os::path::join(project.src, asm_);
        if (!py::os::path::isdir(src)) continue;
        py::str dst = py::os::path::join(port, py::str("src"), asm_);
        if (py::os::path::isdir(dst)) py::shutil::rmtree(dst);
        _copy_tree(src, dst);
        for (const auto& w : py::os::walk(dst)) {
            for (const py::str& f : std::get<2>(w)) {
                if (f.endswith(py::str(".csproj"))) {
                    py::os::remove(py::os::path::join(std::get<0>(w), f));
                    continue;
                }
                if (!f.endswith(py::str(".cs"))) continue;
                py::str p = py::os::path::join(std::get<0>(w), f);
                py::str text = read_sig(p);
                auto [neu, changed] = rewrite(text, own);
                if (!changed.empty()) {
                    std::vector<Json> items;
                    for (const py::str& c : changed) items.push_back(jstr(c));
                    report.dict.at(py::str("rewrites")).dict.set(py::os::path::relpath(p, port), jlist(items));
                    d::write_text(p, neu);
                }
            }
        }
        assemblies.append(asm_);
    }
    // the runtime and the stubs
    py::str rt = py::os::path::join(port, py::str("XnaPort"));
    py::os::makedirs(rt, 0777, true);
    for (const py::str& f : py::os::listdir(RUNTIME()))
        if (f.endswith(py::str(".cs"))) py::shutil::copy2(py::os::path::join(RUNTIME(), f), py::os::path::join(rt, f));
    stubs(py::os::path::join(port, py::str("Stubs")));
    // the game's own files beside the program, and the converted content
    py::str game_link = py::os::path::join(port, py::str("game"));
    if (!py::os::path::isdir(game_link)) _copy_tree(exe_dir, game_link);
    report.dict.set(py::str("xnb reader names"), jint(port_reader_names(game_link, name, hooks)));
    py::str converted = py::os::path::join(port, py::str("ContentConverted"));
    py::str content_src = py::os::path::join(project.assets, py::str("content"), exe_rel);
    if (py::os::path::isdir(content_src)) _copy_tree(content_src, converted);
    py::os::makedirs(converted, 0777, true);
    {
        py::dict<py::str, int64_t> counts = convert_xact(exe_dir, converted, hooks);
        Json content = jdict({});
        for (const auto& kv : counts.items()) content.dict.set(std::get<0>(kv), jint(std::get<1>(kv)));
        content.dict.set(py::str("songs"), jint(convert_songs(exe_dir, converted)));
        EffectsReport eff = compile_effects(converted, py::os::path::join(port, py::str("Content"), py::str("shaders")));
        Json failed = jdict({});
        for (const auto& kv : eff.failed.items()) failed.dict.set(std::get<0>(kv), jstr(std::get<1>(kv)));
        content.dict.set(py::str("effects"), jdict({{py::str("compiled"), jint(eff.compiled)}, {py::str("failed"), failed}}));
        report.dict.set(py::str("content"), content);
    }
    // the game's own hand work
    std::optional<py::str> ov = _overlay_dir(project, overlay);
    if (ov) {
        Json manifest = jdict({});
        if (py::os::path::isfile(py::os::path::join(*ov, py::str("overlay.json"))))
            manifest = knowledge::sources::_json(py::os::path::join(*ov, py::str("overlay.json")));
        auto key = [&](const char* k) -> const Json* { return manifest.dict.contains(py::str(k)) ? &manifest.dict.at(py::str(k)) : nullptr; };
        if (auto* v = key("replace"))
            for (const Json& rel : v->list) py::shutil::rmtree(py::os::path::join(port, rel.s), true);
        if (auto* v = key("sources"))
            for (const auto& kv : v->dict.items()) _copy_tree(std::get<1>(kv).s, py::os::path::join(port, std::get<0>(kv)));
        _copy_tree(*ov, port);
        if (auto* v = key("drop"))
            for (const Json& rel : v->list) {
                py::str p = py::os::path::join(port, rel.s);
                if (py::os::path::isdir(p)) py::shutil::rmtree(p);
                else if (py::os::path::isfile(p)) py::os::remove(p);
            }
        if (auto* v = key("assets")) {
            bool truthy = v->kind == Json::Str ? !v->s.empty() : v->kind != Json::None;
            if (truthy && !py::os::path::isdir(py::os::path::join(port, py::str("assets"))))
                _copy_tree(v->s, py::os::path::join(port, py::str("assets")));
        }
        report.dict.set(py::str("overlay"), jstr(*ov));
    }
    d::write_text(py::os::path::join(port, name + py::str(".Port.csproj")), _csproj(name, exe_rel, assemblies, py::list<py::str>()));
    return {port, name, report};
}

std::tuple<bool, py::list<BuildError>, py::str> _build_once(const py::str& port, const py::str& name) {
    py::subprocess::RunOptions o = capture_();
    o.cwd = port;
    d::EnvGuard env(dotnet_env_());
    py::subprocess::CompletedProcess r = py::subprocess::run(
        py::list<py::str>{py::str("dotnet"), py::str("build"), py::os::path::join(port, name + py::str(".Port.csproj")), py::str("-c"), py::str("Release"),
                          py::str("--nologo"), py::str("-v"), py::str("q"), py::str("-clp:NoSummary")},
        o);
    py::list<BuildError> errors;
    py::set<py::str> seen;
    py::str all = *r.stdout_ + py::str("\n") + *r.stderr_;
    for (const py::str& line : py::splitlines(all)) {
        std::optional<py::re::Match> m = _ERROR.match(line.strip());
        if (m && !seen.contains(line.strip())) {
            seen.add(line.strip());
            errors.append(BuildError{m->group(1), m->group(2), m->group(3), m->group(4), m->group(5)});
        }
    }
    return {r.returncode == 0 && errors.empty(), errors, *r.stdout_ + *r.stderr_};
}

Json BuildResult::to_json() const {
    Json errs = jdict({});
    for (const auto& kv : errors.items()) errs.dict.set(std::get<0>(kv), jint(std::get<1>(kv)));
    std::vector<Json> firsts;
    for (const py::str& s : first) firsts.push_back(jstr(s));
    Json out = jdict({{py::str("ok"), jbool(ok)},
                      {py::str("exe"), exe ? jstr(*exe) : jnone()},
                      {py::str("errors"), errs},
                      {py::str("first"), jlist(firsts)},
                      {py::str("build_output_tail"), jstr(build_output_tail)}});
    for (const auto& kv : report.dict.items()) out.dict.set(std::get<0>(kv), std::get<1>(kv));
    return out;
}

BuildResult build(const base::converted::Project& project, const std::optional<py::str>& overlay, int64_t rounds, const Hooks& hooks) {
    auto [port, name, report] = prepare(project, overlay, hooks);
    bool ok = false;
    py::list<BuildError> errors;
    py::str out;
    std::vector<Json> round_list;
    auto extensions = extension_methods(py::os::path::join(port, py::str("src")));
    for (int64_t r = 0; r < rounds; r++) {
        std::tie(ok, errors, out) = _build_once(port, name);
        int64_t fixed = 0;
        if (!ok) {
            fixed = fix_ref_out(errors);
            fixed += fix_access(errors);
            fixed += fix_color(errors);
            fixed += fix_vertex_binding(errors);
            fixed += fix_named_arguments(errors);
            fixed += fix_moved_members(errors);
            fixed += fix_shadowed_extensions(errors, extensions);
        }
        round_list.push_back(jdict({{py::str("errors"), jint(py::len(errors))}, {py::str("fixed"), jint(fixed)}}));
        if (ok || !fixed) break;
    }
    report.dict.set(py::str("rounds"), jlist(round_list));
    BuildResult result;
    for (const BuildError& e : errors) result.errors[e.code] = result.errors.get(e.code, 0) + 1;
    py::str exe = py::os::path::join(port, py::str("bin"), py::str("Release"), py::str("net8.0"), name + py::str(".exe"));
    result.ok = ok;
    if (ok) result.exe = exe;
    for (int64_t i = 0; i < std::min<int64_t>(60, py::len(errors)); i++) {
        const BuildError& e = errors[i];
        result.first.append(py::percent(py::str("%s(%s): %s %s"), py::os::path::relpath(e.file, port), e.line, e.code, e.text));
    }
    result.build_output_tail = (ok || !errors.empty()) ? py::str("") : out.slice(-2000, py::none);
    result.report = report;
    d::write_json(py::os::path::join(project.reports, py::str("port.json")), result.to_json());
    return result;
}

RunResult run_for(const py::str& exe, int64_t seconds, const std::vector<std::pair<std::string, std::string>>& env) {
    py::str folder = py::os::path::dirname(exe);
    py::str log = py::os::path::join(folder, py::str("xnaport.log"));
    if (py::os::path::isfile(log)) py::os::remove(log);
    std::vector<std::pair<std::string, std::string>> all = dotnet_env_();
    for (const auto& kv : env) all.push_back(kv);
    d::EnvGuard guard(all);

    SECURITY_ATTRIBUTES sa{sizeof sa, nullptr, TRUE};
    HANDLE rd = nullptr, wr = nullptr;
    if (!CreatePipe(&rd, &wr, &sa, 0)) py::raise_winerror(GetLastError());
    SetHandleInformation(rd, HANDLE_FLAG_INHERIT, 0);
    HANDLE in = CreateFileW(L"NUL", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, OPEN_EXISTING, 0, nullptr);
    STARTUPINFOW si{};
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = in;
    si.hStdOutput = wr;
    si.hStdError = wr;
    PROCESS_INFORMATION pi{};
    std::wstring cmd = py::subprocess::to_wide(py::subprocess::list2cmdline(py::list<py::str>{exe}));
    std::wstring wcwd = py::subprocess::to_wide(folder);
    BOOL ok = CreateProcessW(nullptr, cmd.data(), nullptr, nullptr, TRUE, CREATE_UNICODE_ENVIRONMENT, nullptr, wcwd.c_str(), &si, &pi);
    DWORD err = GetLastError();
    CloseHandle(wr);
    if (in != INVALID_HANDLE_VALUE) CloseHandle(in);
    if (!ok) {
        CloseHandle(rd);
        py::raise_winerror(err);
    }
    CloseHandle(pi.hThread);
    std::string collected;
    std::thread reader([&] {
        char buf[4096];
        DWORD got;
        while (ReadFile(rd, buf, sizeof buf, &got, nullptr) && got) collected.append(buf, got);
    });
    DWORD wait = WaitForSingleObject(pi.hProcess, static_cast<DWORD>(seconds * 1000));
    std::optional<int64_t> code;
    if (wait == WAIT_TIMEOUT) {
        TerminateProcess(pi.hProcess, 1);
        WaitForSingleObject(pi.hProcess, INFINITE);
    } else {
        DWORD c = 0;
        GetExitCodeProcess(pi.hProcess, &c);
        code = static_cast<int64_t>(c);
    }
    reader.join();
    CloseHandle(rd);
    CloseHandle(pi.hProcess);
    // text=True: decoded, newlines translated
    py::str out = py::bytes(collected).decode("cp1252");
    {
        const std::u32string& u = out.u32();
        std::u32string t;
        for (size_t i = 0; i < u.size(); i++) {
            if (u[i] == U'\r') {
                t.push_back(U'\n');
                if (i + 1 < u.size() && u[i + 1] == U'\n') i++;
            } else {
                t.push_back(u[i]);
            }
        }
        out = py::str(t);
    }
    py::str text = py::os::path::isfile(log) ? d::read_text_sig(log, true, false) : py::str("");
    return RunResult{code, out.slice(-6000, py::none), text.slice(-6000, py::none)};
}

std::optional<Json> check_content(const py::str& exe, int64_t seconds) {
    py::str out = py::os::path::join(py::os::path::dirname(exe), py::str("content_check.json"));
    if (py::os::path::isfile(out)) py::os::remove(out);
    run_for(exe, seconds, {{"XNAPORT_CHECK_CONTENT", "1"}});
    if (!py::os::path::isfile(out)) return std::nullopt;
    return knowledge::sources::_json(out);
}

const py::str DOC(R"DOC(port.py -- an XNA game's decompiled C# (Converted/<name>/src) built and run on a PC with MonoGame.

    codebreaker xna port <Converted project or name> [--overlay DIR] [--run SECONDS] [--check-content]
    from codebreaker.xna import port
    result = port.build(project_dir)        {"exe", "errors": {code: count}, "first": [error lines]}

The project's src/ stays the decompiled source. The port is built from a copy in build/port/:

  src/<assembly>/      each assembly's C#, rewritten for the PC where it must be:
                         - the Game class's constructor first calls XnaPort.Runtime.Install(this) (the content
                           manager below), and `new GraphicsDeviceManager(this)` is XnaPort.Runtime.Graphics(this)
                           (HiDef, as the Xbox 360 was);
                         - a file using XACT (AudioEngine, WaveBank, SoundBank, Cue, AudioCategory) gets `using`
                           aliases to XnaPort.Xact (xna/runtime/Xact.cs), which plays its cues from WAV;
                         - ((GameComponent)this).Update(...) and the other lifecycle calls ILSpy writes as a cast to
                           the base class are base.Update(...) -- as a cast they call the override again, forever;
  XnaPort/             CodeBreaker's XNA runtime (xna/runtime/*.cs): the content manager, players and the Guide,
                       storage on disk, XACT, Xbox-only calls;
  Stubs/               the rest of XNA's Xbox-side API -- GamerServices, Net, Storage, Avatar -- generated from the
                       XNA 4.0 assemblies' metadata (xna/apistubs; the assemblies are XNA Game Studio's, found in
                       the Windows GAC, read and never copied), members doing nothing;
  <name>.Port.csproj   MonoGame DesktopGL 3.8.1 on .NET 8 (DOTNET_ROLL_FORWARD=LatestMajor), the game's own files
                       copied beside the program, ContentConverted/ the converted content (textures, fonts and
                       sounds the runtime loads in place of the Xbox's .xnb, the XACT banks as WAV, and the
                       songs' music, WMA on the Xbox, as Ogg Vorbis, which MonoGame plays, and the effects'
                       recovered HLSL compiled by mgfxc to Content/shaders/<sha1>.mgfxo); an .xnb
                       naming a type in an XNA assembly MonoGame does not have (Avatar, GamerServices, Net,
                       Storage: the stubs, in this program) names this program instead -- MonoGame would make
                       of "Microsoft.Xna.Framework.Avatar" an assembly name nothing can parse -- and one read by
                       XNA's ReflectiveReader names the runtime's XnaReflectiveReader, which keeps XNA's rules;
  overlay              a folder of the game's own hand work, copied over all of it last (setting `xna_overlays`,
                       <dir>/<name>/; or --overlay): shaders the Xbox had only as microcode, fixes no rewrite makes.
                       Its overlay.json may say more: "replace" [paths removed first], "sources" {path in the port:
                       a folder elsewhere, copied there -- a hand port's own tree}, "drop" [paths removed after: the
                       generic runtime's pieces a hand port replaces], "assets" (a folder copied to assets/ beside
                       the build, where a hand port's runtime looks for open-format assets).

The build runs until it compiles or stops changing: an error the compiler itself names the fix for (CS1620: an
argument passed `ref` where the method wants `out`, or the other way) is fixed and the build run again; what is
left is listed by error code in reports/port.json. `--run` starts the program for that many seconds and keeps its
log (xnaport.log, and what it printed): the first missing piece is what it stops at -- a shader names itself.
`--check-content` has the built program load every .xnb under Content/ through the runtime (models, buffers and
custom types read by MonoGame's readers and the game's own) and lists each one's result in reports/content_check.json.
)DOC");

int64_t main(const py::list<py::str>& argv, const Hooks& hooks) {
    if (argv.empty() || argv[0] == py::str("-h") || argv[0] == py::str("--help")) {
        py::print(DOC);
        return argv.empty() ? 2 : 0;
    }
    py::str target = argv[0];
    py::str path = py::os::path::isdir(target) ? target : py::os::path::join(base::converted::root(), target);
    if (!py::os::path::isfile(py::os::path::join(path, py::str("project.json")))) {
        py::print(py::percent(py::str("xna port: %s is not a Converted project"), target));
        return 2;
    }
    base::converted::Project project(path);
    std::optional<py::str> overlay;
    if (argv.contains(py::str("--overlay"))) overlay = argv[argv.index(py::str("--overlay")) + 1];
    double t0 = py::time::time();
    BuildResult result;
    try {
        result = build(project, overlay, 6, hooks);
    } catch (const py::ValueError& e) {
        py::print(py::percent(py::str("xna port: %s"), py::str(e.what())));
        return 1;
    } catch (const py::RuntimeError& e) {
        py::print(py::percent(py::str("xna port: %s"), py::str(e.what())));
        return 1;
    }
    if (result.ok) {
        py::print(py::percent(py::str("built %s (%.0f s)"), *result.exe, py::time::time() - t0));
    } else {
        int64_t total = 0;
        std::vector<std::tuple<py::str, int64_t>> kv;
        for (const auto& x : result.errors.items()) {
            total += std::get<1>(x);
            kv.push_back(x);
        }
        std::stable_sort(kv.begin(), kv.end(), [](const auto& a, const auto& b) { return -std::get<1>(a) < -std::get<1>(b); });
        std::vector<py::str> parts;
        for (const auto& x : kv) parts.push_back(py::percent(py::str("%s %d"), std::get<0>(x), std::get<1>(x)));
        py::print(py::percent(py::str("not built: %d errors (%s); reports/port.json lists them"), total, py::join(py::str(", "), parts)));
        for (int64_t i = 0; i < std::min<int64_t>(15, py::len(result.first)); i++) py::print(py::str("  ") + result.first[i]);
    }
    if (result.ok && argv.contains(py::str("--run"))) {
        int64_t seconds = py::int_parse(argv[argv.index(py::str("--run")) + 1], 10);
        RunResult r = run_for(*result.exe, seconds);
        py::print(py::percent(py::str("ran %d s: %s"), seconds, !r.exit ? py::str("still running") : py::percent(py::str("exited %s"), py::to_str(*r.exit))));
        py::print((!r.log.empty() ? r.log : r.output).slice(-2500, py::none));
        d::write_json(py::os::path::join(project.reports, py::str("port_run.json")),
                      jdict({{py::str("exit"), r.exit ? jint(*r.exit) : jnone()}, {py::str("output"), jstr(r.output)}, {py::str("log"), jstr(r.log)}}));
    }
    if (result.ok && argv.contains(py::str("--check-content"))) {
        std::optional<Json> checked = check_content(*result.exe);
        if (!checked) {
            py::print(py::str("content check: the program wrote no result"));
        } else {
            int64_t failed = 0, other = 0, all = checked->dict.size();
            std::vector<std::pair<py::str, py::str>> bad;
            for (const auto& kv : checked->dict.items()) {
                const py::str& v = std::get<1>(kv).s;
                if (v.startswith(py::str("error:"))) {
                    failed++;
                    bad.push_back({std::get<0>(kv), v});
                }
                if (v.startswith(std::make_tuple(py::str("not an XNB"), py::str("empty")))) other++;
            }
            py::print(py::percent(py::str("content check: %d of %d .xnb loaded, %d failed; %d are not XNB content (empty, or a file the "
                                          "game reads itself)"),
                                  all - failed - other, all - other, failed, other));
            std::sort(bad.begin(), bad.end());
            for (int64_t i = 0; i < std::min<int64_t>(20, static_cast<int64_t>(bad.size())); i++)
                py::print(py::percent(py::str("  %s: %s"), bad[i].first, bad[i].second.slice(py::len(py::str("error: ")), py::none).slice(0, 160)));
            d::write_json(py::os::path::join(project.reports, py::str("content_check.json")), *checked);
        }
    }
    std::vector<py::str> outs;
    if (result.ok) outs.push_back(*result.exe);
    int64_t left = 0;
    for (const auto& x : result.errors.items()) left += std::get<1>(x);
    project.record(py::str("xna port"), py::list<py::str>(outs),
                   py::percent(py::str("MonoGame build of the decompiled C# in build/port: %s"),
                               result.ok ? py::str("built") : py::percent(py::str("%d errors left (reports/port.json)"), left)));
    return result.ok ? 0 : 1;
}

}  // namespace codebreaker::xna::port
