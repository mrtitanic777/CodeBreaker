// codebreaker/java/projects/minecraft/applet_ping.cpp -- C++ port of codebreaker.java.projects.minecraft.applet_ping
// (see the header).

#include "codebreaker/java/projects/minecraft/applet_ping.hpp"

#include <map>
#include <memory>
#include <set>

#include "codebreaker/translate/ir.hpp"
#include "codebreaker/translate/rewrite.hpp"

namespace codebreaker::java::projects::minecraft::applet_ping {

namespace ir = codebreaker::translate::ir;
namespace rw = codebreaker::translate::rewrite;

const py::re::Pattern _PING = py::re::compile(py::str(
    R"(try\s*\{[^{}]*?getDocumentBase\(\)[^{}]*?openStream\b[^{}]*?)"
    R"(if\s*\([^{}]*?\)\s*\{\s*(this->\w+\s*=\s*\w+\s*;)\s*\}\s*)"
    R"(\}\s*catch\s*\([^)]*\)\s*\{\s*\})"));

const py::str _NOTE = py::str(
    "   // J2X applet-ping-bypass: console has no applet -- getDocumentBase()->openStream() is a "
    "NULL-returning stub and readLine() on it FAULTS (an access violation, NOT caught by try/catch). "
    "Drop the ping and set the level directly (matches the OG Indev hand-port). [generator: applet_ping.py]");

const py::re::Pattern _SURVIVOR = py::re::compile(py::str(R"(try\s*\{[^{}]*?getDocumentBase\(\)[^{}]*?openStream\b)"));

std::tuple<py::str, int64_t> process(const py::str& text) {
    return _PING.subn([](const py::re::Match& m) { return m.group(1) + _NOTE; }, text);
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
        if (!text.contains(py::str("getDocumentBase")) || !text.contains(py::str("openStream"))) {
            continue;
        }
        auto [new_text, n] = process(text);
        if (new_text != text) {
            py::file fh = py::open(path, py::str("w"), py::str("utf-8"));
            fh.write(new_text);
            fh.close();
            res.files.append(Touched{f, n});
            res.sites += n;
        }
        if (_SURVIVOR.search(new_text)) {
            res.stranded.append(f);
        }
    }
    return res;
}

namespace {

[[noreturn]] void none_iter() { throw py::TypeError("'NoneType' object is not iterable"); }
[[noreturn]] void no_attr(const std::string& type, const char* attr) {
    throw py::AttributeError("'" + type + "' object has no attribute '" + attr + "'");
}

template <class T> py::ref<T> as(const ir::NodeP& n) { return std::dynamic_pointer_cast<T>(n); }

// `x.stmts` of a node that must be a Block (AttributeError otherwise); the optional list is None when unset.
const ir::Nodes& stmts_of(const ir::NodeP& n) {
    if (!n) no_attr("NoneType", "stmts");
    auto b = std::dynamic_pointer_cast<ir::Block>(n);
    if (!b) no_attr(n->class_name().utf8(), "stmts");
    return b->stmts;
}

}  // namespace

Result patch(const py::list<ir::NodeP>& units) {
    std::map<std::optional<py::str>, int64_t> per;   // class name (None possible) -> sites
    for (const auto& cm : rw::methods(units)) {
        const py::ref<ir::ClassDecl>& cls = std::get<0>(cm);
        const py::ref<ir::MethodDecl>& m = std::get<1>(cm);
        for (ir::Nodes ss : rw::stmt_lists(m->body)) {
            if (!ss) {
                none_iter();                               // enumerate(None)
            }
            for (int64_t i = 0; i < py::len(*ss); ++i) {
                auto s = as<ir::Try>((*ss)[i]);
                if (!s || s->final || (s->resources && py::len(*s->resources) > 0)) {
                    continue;
                }
                std::set<py::str> calls;
                for (const ir::NodeP& n : ir::walk(s->body)) {
                    auto c = as<ir::Call>(n);
                    if (c && c->sym) {
                        calls.insert(c->sym->name);
                    }
                }
                if (!(calls.count(py::str("getDocumentBase")) && calls.count(py::str("openStream")))) {
                    continue;
                }
                if (!s->catches) {
                    none_iter();
                }
                bool any_body = false;
                for (const ir::NodeP& cn : *s->catches) {
                    if (!cn) no_attr("NoneType", "body");
                    auto c = as<ir::Catch>(cn);
                    if (!c) no_attr(cn->class_name().utf8(), "body");
                    if (c->body) {
                        const ir::Nodes& cs = stmts_of(c->body);
                        if (cs && py::len(*cs) > 0) {
                            any_body = true;
                            break;
                        }
                    }
                }
                if (any_body) {
                    continue;
                }
                const ir::Nodes& tb = stmts_of(s->body);
                if (!tb) none_iter();
                py::list<ir::NodeP> guarded;
                for (const ir::NodeP& x : *tb) {
                    if (as<ir::If>(x)) {
                        guarded.append(x);
                    }
                }
                if (py::len(guarded) != 1) {
                    continue;
                }
                auto g = as<ir::If>(guarded[0]);
                if (g->orelse) {
                    continue;
                }
                py::list<ir::NodeP> body;
                if (as<ir::Block>(g->then)) {
                    const ir::Nodes& gs = as<ir::Block>(g->then)->stmts;
                    if (!gs) none_iter();
                    body = *gs;
                } else {
                    body.append(g->then);
                }
                if (py::len(body) != 1 || !as<ir::ExprStmt>(body[0])) {
                    continue;
                }
                ir::NodeP a = rw::strip(as<ir::ExprStmt>(body[0])->expr);
                auto asg = as<ir::Assign>(a);
                if (!(asg && !asg->op)) {
                    continue;
                }
                auto tgt = as<ir::Select>(asg->target);
                if (!(tgt && as<ir::This>(tgt->obj) && as<ir::Name>(rw::strip(asg->value)))) {
                    continue;
                }
                (*ss)[i] = body[0];                        // set the level directly: no applet on console
                per[cls->name] += 1;
            }
        }
    }
    for (const auto& kv : per) {
        if (!kv.first) {
            throw py::TypeError("unsupported operand type(s) for +: 'NoneType' and 'str'");   // sorted / c + '.cpp'
        }
    }
    Result res;
    for (const auto& kv : per) {
        res.files.append(Touched{*kv.first + py::str(".cpp"), kv.second});
        res.sites += kv.second;
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
            if (_SURVIVOR.search(text)) {
                out.append(f);
            }
        }
    }
    return out;
}

}  // namespace codebreaker::java::projects::minecraft::applet_ping
