// codebreaker/java/transpile.cpp -- typed C++ port of codebreaker.java.transpile (see transpile.hpp).

#include "codebreaker/java/transpile.hpp"

#include <algorithm>
#include <atomic>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <utility>

#include "codebreaker/base/cache.hpp"
#include "codebreaker/base/parallel.hpp"
#include "codebreaker/java/_paths.hpp"
#include "codebreaker/java/apimap.hpp"
#include "codebreaker/java/bugfix_ledger.hpp"
#include "codebreaker/java/cmake.hpp"
#include "codebreaker/java/console/platform_refs.hpp"
#include "codebreaker/java/console/sdk.hpp"
#include "codebreaker/java/console/vcxproj.hpp"
#include "codebreaker/java/debug.hpp"
#include "codebreaker/java/emit/header.hpp"
#include "codebreaker/java/emit/ownership.hpp"
#include "codebreaker/java/emit/source.hpp"
#include "codebreaker/java/emit/std_transforms.hpp"
#include "codebreaker/java/overlays.hpp"
#include "codebreaker/java/parse.hpp"
#include "codebreaker/java/platforms.hpp"
#include "codebreaker/java/probe.hpp"
#include "codebreaker/java/profiles/package.hpp"
#include "codebreaker/java/projects/package.hpp"
#include "codebreaker/java/reflection_factory.hpp"
#include "codebreaker/java/report.hpp"
#include "codebreaker/java/rules.hpp"
#include "codebreaker/java/runtime.hpp"
#include "codebreaker/java/textutil.hpp"
#include "codebreaker/java/twophase.hpp"
#include "codebreaker/java/typemap.hpp"
#include "codebreaker/translate/backends/cpp03.hpp"
#include "codebreaker/translate/backends/cpp17.hpp"
#include "codebreaker/translate/desugar.hpp"
#include "codebreaker/verify/lanes/java.hpp"
#include "py_settings.hpp"

#ifdef ERROR
#undef ERROR   // (wingdi.h: it is the name of a ledger Category)
#endif

namespace codebreaker::java::transpile {

namespace {

using py::str;

str S(const char* s) { return str(s); }

namespace cpp03 = translate::backends::cpp03;

str join_nl(const py::list<str>& xs) { return py::join(S("\n"), xs); }

}  // namespace

bool _TRACE_THROWS = false;
bool _GL_TO_D3D9 = false;
py::list<str> _GL_NAMESPACES{S("GL11"), S("GLU")};
py::set<str> _THIS_ESCAPES;
std::shared_ptr<projects::Project> _PROJECT;

// ================================================================================================== profiles
// The profile registry (java/profiles) and the project layer (java/projects, the Minecraft project) are the
// packages' own. What this module adds is what the Python's `profiles.on_change` observers do here: when the
// active profile changes, the typed ports that hold profile data (typemap, rules, console/vcxproj) and this
// module's static-init seed are re-derived from it. (java.signatures has no port; its package-role table is
// not reseeded.)
namespace {

void on_change_(const profiles::Profile& p) {
    typemap::Profile t;
    t.nullable = py::set<str>::from(p.nullable);
    t.nullable_fields = py::set<str>::from(p.nullable_fields);
    t.nullable_getters = py::set<str>::from(p.nullable_getters);
    t.nullable_getter_return_type = p.nullable_getter_return_type.copy();
    // (a profile's unsigned_byte_scalar_fields are (ClassName, fieldName) pairs; the profile record holds
    // str, and every profile has none)
    t.default_value_types = py::set<str>::from(p.default_value_types);
    typemap::_reseed(t);
    rules::_reseed(p.heavy_types, p.heavy_types_high);
    console::vcxproj::_reseed(p.vcxproj_filters);
    _reseed_static_init_order(p);
}

// registered once, as the Python modules do at import
const bool observers_registered_ = [] {
    profiles::on_change(on_change_);
    return true;
}();

}  // namespace

namespace {

str state_(const py::list<str>& parts) {   // settings.state(*parts)
    str path = py::settings::get_str(S("state"));
    for (const str& p : parts) path = py::os::path::join(path, p);
    py::os::makedirs(py::truthy(parts) ? py::os::path::dirname(path) : path, 0777, true);
    return path;
}

}  // namespace

// ================================================================================================== the passes
str _convert_gl_to_d3d9(const str& text) {
    py::list<str> esc;
    for (const str& n : _GL_NAMESPACES) esc.append(py::re::escape(n));
    py::re::Pattern pat = py::re::compile(S("\\b(?:") + py::join(S("|"), esc) + S(")::"));
    return pat.sub(S("d3d9::"), text);
}

str _trace_guest_throws(const str& text) {
    py::list<str> out;
    for (const str& ln : py::split(text, S("\n"))) {
        str s = ln.lstrip();
        if (ln.contains(S("J2X_GUEST_THROW")) || !s.startswith(S("throw"))) {
            out.append(ln);
            continue;
        }
        str after = s.slice(5, 6);   // char right after `throw`
        if (!(after == S(" ") || after == S("\t") || after == S(";"))) {   // 'throw <expr>' or bare 'throw;' only (not 'throws')
            out.append(ln);
            continue;
        }
        str indent = ln.slice(0, ln.size() - s.size());
        out.append(indent + S("J2X_GUEST_THROW(); ") + s);
    }
    return join_nl(out);
}

str _finalize_emitted(const str& text_in) {
    str text = emit::std_transforms::sanitize_reserved_members(text_in);
    if (_GL_TO_D3D9) {
        text = _PROJECT->finalize_text(text, S("xbox360"));   // the project's last word (Minecraft: GL picking
        //                                                        -> a raycast), BEFORE the ns rename
        text = _convert_gl_to_d3d9(text);
    }
    if (_TRACE_THROWS) {
        text = _trace_guest_throws(text);   // tag every throw with its site (throws are no-op landmines here)
    }
    text = text.replace(str(char32_t(39)) + str(char32_t(92)) + S("u0000") + str(char32_t(39)),
                        str(char32_t(39)) + str(char32_t(92)) + S("0") + str(char32_t(39)));
    // `new Object()` (a bare java.lang.Object, used only as a monitor/lock) -> a JObject VALUE. Object
    // has no C++ class; the universal box default-constructs, and the sync object is never used on the port.
    static const py::re::Pattern new_object = py::re::compile(S("\\bnew\\s+Object\\s*\\(\\s*\\)"));
    text = new_object.sub(S("JObject()"), text);
    // DISCARDED-NEW LEAK: a statement that is a bare `new X(...);` (allocation whose result is never stored)
    // is a no-op in Java (the temporary is GC'd immediately) but LEAKS in C++. Faithful even in the original
    // -- e.g. RubyDung.render() has Notch's stray `new Cube(0, 0);`. Rewrite to `delete new X(...);` so the
    // ctor's side effects (if any) still run and the object is freed. Skips args containing a nested `new`
    // (deleting the outer object could double-free the inner one) -- left as-is for manual review -- and
    // FIXED BUG discarded-new-escapes: a class whose constructor hands out `this` (Item registers itself
    // in Item.itemsList) is kept by someone else; deleting it left them a dangling pointer.
    static const py::re::Pattern bare_new = py::re::compile(S("(?m)^(\\s*)(new\\s+[A-Za-z_][\\w:]*\\s*\\([^;]*\\)\\s*;)\\s*$"));
    static const py::re::Pattern word_new = py::re::compile(S("\\bnew\\b"));
    static const py::re::Pattern lead_new = py::re::compile(S("new\\s+([A-Za-z_]\\w*)"));
    return bare_new.sub(
        [&](const py::re::Match& m) -> str {
            str indent = m.group(1), stmt = m.group(2);
            if (word_new.search(stmt.slice(3, stmt.size()))) return m.group(0);
            if (_THIS_ESCAPES.contains(lead_new.match(stmt)->group(1))) return m.group(0);
            return py::percent(S("%sdelete %s  // J2X-BUGFIX discarded-new-leak"), indent, stmt);
        },
        text);
}

py::list<str> discover(const str& java_dir) {
    py::dict<str, str> paths;
    for (const auto& w : py::os::walk(java_dir)) {
        for (const str& f : std::get<2>(w)) {
            if (f.endswith(S(".java")) && !paths.contains(f)) paths.set(f, py::os::path::join(std::get<0>(w), f));
        }
    }
    py::list<str> out;
    for (const str& k : py::sorted(paths.keys())) out.append(paths.at(k));
    return out;
}

std::tuple<py::list<ClassRef>, py::set<str>> drop_synthetic_artifacts(const py::list<ClassRef>& classes, ledger::Ledger* ledger) {
    // names used as a base -> never droppable (NoiseGenerator is `class NoiseGenerator {}` but is
    // extended by NoiseGeneratorOctaves/Perlin).
    py::set<str> used_as_base;
    for (const ClassRef& c : classes) {
        if (c->parent) used_as_base.add(*c->parent);
        used_as_base.update(c->interfaces);
    }
    // sole-parameter constructor types -> the bridge-ctor signal
    py::set<str> bridge_param_types;
    for (const ClassRef& c : classes)
        for (const MethodRef& m : c->methods)
            if (m->is_ctor && m->params.size() == 1) bridge_param_types.add(m->params[0]->jtype->name);
    // a marker is EMPTY (no members/parent/interfaces), never a base, AND used only as a bridge param
    py::set<str> markers;
    for (const ClassRef& c : classes) {
        if (!c->is_enum && !c->is_interface && !py::truthy(c->fields) && !py::truthy(c->methods) && !c->parent.has_value() &&
            !py::truthy(c->interfaces) && !used_as_base.contains(c->name) && bridge_param_types.contains(c->name))
            markers.add(c->name);
    }
    if (!py::truthy(markers)) return {classes, py::set<str>()};
    int64_t dropped_ctors = 0;
    for (const ClassRef& c : classes) {
        py::list<MethodRef> kept;
        for (const MethodRef& m : c->methods) {
            if (m->is_ctor && m->params.size() == 1 && markers.contains(m->params[0]->jtype->name)) {
                dropped_ctors++;
                continue;   // synthetic bridge ctor -> drop
            }
            kept.append(m);
        }
        c->methods = kept;
    }
    py::list<ClassRef> result;
    for (const ClassRef& c : classes)
        if (!markers.contains(c->name)) result.append(c);
    if (ledger != nullptr) {
        py::list<str> sorted_markers = py::list<str>::from(py::sorted(markers));
        ledger->add(S("(pipeline)"), ledger::Category::PORTED,
                    py::percent(S("dropped %d empty marker class(es) [%s] + %d synthetic bridge constructor(s)"), markers.size(),
                                py::join(S(", "), sorted_markers), dropped_ctors),
                    0, std::nullopt, S("pipeline"), S("drop-synthetic"));
    }
    return {result, markers};
}

void resolve_inheritance(const py::list<ClassRef>& classes) {
    py::dict<str, ClassRef> by_name;
    for (const ClassRef& c : classes) by_name.set(c->name, c);
    for (const ClassRef& cls : classes) {
        py::set<str> seen;
        for (const auto& f : cls->fields) seen.add(f->name);
        py::list<py::ref<ir::Field>> inherited;
        std::optional<str> parent = cls->parent;
        py::set<str> guard{cls->name};
        while (parent && !parent->empty() && by_name.contains(*parent) && !guard.contains(*parent)) {
            guard.add(*parent);
            const ClassRef& pc = by_name.at(*parent);
            for (const auto& f : pc->fields) {
                if (!f->is_static && f->access != ir::PRIVATE && !seen.contains(f->name)) {
                    inherited.append(f);
                    seen.add(f->name);
                }
            }
            parent = pc->parent;
        }
        cls->inherited_fields = inherited;
    }
}

void compute_mi_forwards(const py::list<ClassRef>& classes) {
    py::dict<str, ClassRef> by_name;
    for (const ClassRef& c : classes) by_name.set(c->name, c);

    // all methods from implemented interfaces (transitive)
    auto iface_methods = [&](const ClassRef& cls) {
        py::dict<str, MethodRef> out;
        py::list<str> stack = cls->interfaces.copy();
        py::set<str> seen;
        while (py::truthy(stack)) {
            str nm = stack.pop();
            if (seen.contains(nm)) continue;
            seen.add(nm);
            if (!by_name.contains(nm)) continue;
            const ClassRef& ic = by_name.at(nm);
            for (const MethodRef& m : ic->methods)
                if (!m->is_ctor) out.setdefault(m->name, m);
            stack.extend(ic->interfaces);
            if (ic->parent && !ic->parent->empty()) stack.append(*ic->parent);
        }
        return out;
    };

    for (const ClassRef& cls : classes) {
        cls->mi_forwards = py::list<std::tuple<MethodRef, str>>();
        if (cls->is_interface || cls->is_enum || !py::truthy(cls->interfaces)) continue;
        py::set<str> own;
        for (const MethodRef& m : cls->methods)
            if (!m->is_ctor) own.add(m->name);
        // nearest concrete impl from the (non-interface) base-class chain -> (class, arity)
        py::dict<str, std::tuple<str, int64_t>> base_concrete;
        const ClassRef* cur = (cls->parent && by_name.contains(*cls->parent)) ? &by_name.at(*cls->parent) : nullptr;
        py::set<str> guard;
        while (cur != nullptr && !guard.contains((*cur)->name)) {
            guard.add((*cur)->name);
            if (!(*cur)->is_interface) {
                for (const MethodRef& m : (*cur)->methods)
                    if (!m->is_ctor && !m->is_abstract) base_concrete.setdefault(m->name, std::make_tuple((*cur)->name, m->arity()));
            }
            cur = ((*cur)->parent && by_name.contains(*(*cur)->parent)) ? &by_name.at(*(*cur)->parent) : nullptr;
        }
        py::dict<str, MethodRef> im = iface_methods(cls);
        for (const str& name : im.keys()) {
            const MethodRef& imethod = im.at(name);
            if (own.contains(name) || !base_concrete.contains(name)) continue;   // C overrides it, or no inherited concrete impl
            const auto& bc = base_concrete.at(name);
            if (std::get<1>(bc) != imethod->arity()) continue;   // same name, different signature -> not the diamond
            cls->mi_forwards.append(std::make_tuple(imethod, std::get<0>(bc)));
        }
    }
}

namespace {

using JTypeRef = py::ref<ir::JType>;

// A JType for a runtime-stub method's C++ return spelling, ONLY when it is unambiguous enough to drive
// chain inference: a single reference-type pointer (`File*` -> File) or `std::string` (-> String, a value).
JTypeRef stub_return_jtype_(const str& cpp_in) {
    str cpp = cpp_in.strip();
    if (cpp == S("std::string")) return std::make_shared<ir::JType>(S("String"));
    static const py::re::Pattern ptr = py::re::compile(S("^([A-Za-z_]\\w*)\\s*\\*$"));
    auto m = ptr.match(cpp);
    if (m && m->group(1) != S("void")) return std::make_shared<ir::JType>(m->group(1));
    return nullptr;
}

// A class-like record of the member-env flatten: a corpus class or a runtime-stub class.
struct MemberRec {
    str name;
    std::optional<str> parent;
    py::list<py::ref<ir::Field>> fields;
    py::list<std::tuple<str, bool, JTypeRef>> methods;   // (name, is_ctor, return type)
};

}  // namespace

py::dict<str, types::MemberEnv> build_member_env(const py::list<ClassRef>& classes) {
    std::unordered_map<std::string, MemberRec> by_name;
    std::vector<std::string> order;
    for (const ClassRef& c : classes) {
        MemberRec r;
        r.name = c->name;
        r.parent = c->parent;
        r.fields = c->fields;
        for (const MethodRef& m : c->methods) r.methods.append(std::make_tuple(m->name, m->is_ctor, m->return_type));
        std::string k = c->name.utf8();
        if (by_name.find(k) == by_name.end()) order.push_back(k);
        by_name[k] = r;   // (dict(...) of the classes: a later class of the same name wins)
    }
    // Runtime-stub classes (URL/File/Runtime/Applet/Node/...) aren't in the parsed corpus, but their
    // method result types must resolve chains through them (`getDocumentBase()->getHost()`, a corpus
    // class extending Applet inheriting getDocumentBase, etc.). Adopt them as class-like records so the
    // SAME flatten handles corpus->stub inheritance and stub->stub bases uniformly. Corpus names win.
    std::vector<std::string> stub_names;
    auto& stubs = runtime::STUB_MEMBERS();
    for (const str& name : stubs.keys()) {
        if (by_name.find(name.utf8()) != by_name.end()) continue;
        const runtime::StubMembers& rec = stubs.at(name);
        MemberRec r;
        r.name = name;
        r.parent = rec.parent;
        for (const str& mname : rec.methods.keys()) {
            JTypeRef jt = stub_return_jtype_(rec.methods.at(mname));
            if (jt) r.methods.append(std::make_tuple(mname, false, jt));
        }
        by_name[name.utf8()] = r;
        stub_names.push_back(name.utf8());
    }
    py::dict<str, types::MemberEnv> env;
    std::vector<std::string> everything;
    for (const ClassRef& c : classes) everything.push_back(c->name.utf8());
    everything.insert(everything.end(), stub_names.begin(), stub_names.end());
    for (const std::string& key : everything) {
        const MemberRec& cls = by_name.at(key);
        types::MemberEnv e;
        const MemberRec* cur = &cls;
        py::set<str> guard;
        while (cur != nullptr && !guard.contains(cur->name)) {   // most-derived first; don't overwrite
            guard.add(cur->name);
            for (const auto& f : cur->fields) e.fields.setdefault(f->name, f->jtype);
            for (const auto& m : cur->methods) {
                if (!std::get<1>(m) && std::get<2>(m)) e.methods.setdefault(std::get<0>(m), std::get<2>(m));
            }
            if (!cur->parent) break;
            auto it = by_name.find(cur->parent->utf8());
            cur = it == by_name.end() ? nullptr : &it->second;
        }
        e.parent = cls.parent;
        env.set(cls.name, e);
    }
    // Runtime types that aren't in the parsed corpus but whose chained methods must resolve `.`->`->`
    // (StringBuilder.append() returns the builder). Return types drive chain inference.
    types::MemberEnv sb;
    auto jt = [](const char* n) { return std::make_shared<ir::JType>(str(n)); };
    sb.methods.set(S("append"), jt("StringBuilder"));
    sb.methods.set(S("insert"), jt("StringBuilder"));
    sb.methods.set(S("deleteCharAt"), jt("StringBuilder"));
    sb.methods.set(S("reverse"), jt("StringBuilder"));
    sb.methods.set(S("toString"), jt("String"));
    sb.methods.set(S("charAt"), jt("char"));
    sb.methods.set(S("length"), jt("int"));
    env.set(S("StringBuilder"), sb);
    return env;
}

py::dict<str, py::set<str>> build_method_renames(const py::list<ClassRef>& classes) {
    py::dict<str, ClassRef> by_name;
    for (const ClassRef& c : classes) by_name.set(c->name, c);
    py::dict<str, py::set<str>> own;
    for (const ClassRef& c : classes) {
        py::set<str> fields;
        for (const auto& f : c->fields) fields.add(f->name);
        py::set<str> s;
        for (const MethodRef& m : c->methods)
            if (!m->is_ctor && fields.contains(m->name)) s.add(m->name);
        own.set(c->name, s);
    }
    // A STATIC method with the SAME NAME AND SIGNATURE as an inherited INSTANCE (virtual) method is C++
    // C4526 (the static "cannot override" the base virtual -> it's hidden). Java allows it (BlockFlowing's
    // private static flow(World,int,int,int) vs BlockFluid's instance flow(World,int,int,int)); rename the
    // static so it no longer collides. Guards, so we don't break legit overloads:
    //  * SIGNATURE must match -- a static helper with a DIFFERENT signature (NoiseGeneratorPerlin's static
    //    generateNoise(double) vs the base virtual generateNoise(double,double)) legally coexists in C++
    //    and does NOT trip C4526; renaming it would kill the class's real generateNoise(double,double) OVERRIDE.
    //  * the name must NOT also be an INSTANCE method of this class -- the rename is by name, so renaming
    //    would clobber that instance method (the actual override) too.
    auto sig = [](const MethodRef& m) {
        py::list<str> ts;
        for (const auto& p : m->params) ts.append(p->jtype->name);
        return std::make_tuple(m->name, py::join(S("\x1f"), ts));
    };
    for (const ClassRef& c : classes) {
        py::set<str> instance_names;
        for (const MethodRef& m : c->methods)
            if (!m->is_ctor && !m->is_static) instance_names.add(m->name);
        py::list<MethodRef> static_methods;
        for (const MethodRef& m : c->methods)
            if (!m->is_ctor && m->is_static && !instance_names.contains(m->name)) static_methods.append(m);
        if (!py::truthy(static_methods)) continue;
        py::set<std::tuple<str, str>> inherited_sigs;
        const ClassRef* cur = (c->parent && by_name.contains(*c->parent)) ? &by_name.at(*c->parent) : nullptr;
        py::set<str> guard;
        while (cur != nullptr && !guard.contains((*cur)->name)) {
            guard.add((*cur)->name);
            for (const MethodRef& m : (*cur)->methods)
                if (!m->is_ctor && !m->is_static) inherited_sigs.add(sig(m));
            cur = ((*cur)->parent && by_name.contains(*(*cur)->parent)) ? &by_name.at(*(*cur)->parent) : nullptr;
        }
        for (const MethodRef& m : static_methods)
            if (inherited_sigs.contains(sig(m))) own.at(c->name).add(m->name);
    }
    py::dict<str, py::set<str>> renames;
    for (const ClassRef& c : classes) {
        py::set<str> s;
        const ClassRef* cur = &c;
        py::set<str> guard;
        while (cur != nullptr && !guard.contains((*cur)->name)) {
            guard.add((*cur)->name);
            if (own.contains((*cur)->name)) s.update(own.at((*cur)->name));
            cur = ((*cur)->parent && by_name.contains(*(*cur)->parent)) ? &by_name.at(*(*cur)->parent) : nullptr;
        }
        renames.set(c->name, s);
    }
    return renames;
}

py::set<str> compute_thread_like(const py::list<ClassRef>& classes) {
    py::dict<str, ClassRef> by_name;
    for (const ClassRef& c : classes) by_name.set(c->name, c);
    py::set<str> thread_like;
    for (const ClassRef& cls : classes) {
        const ClassRef* cur = &cls;
        py::set<str> guard;
        while (cur != nullptr && !guard.contains((*cur)->name)) {
            guard.add((*cur)->name);
            const ClassRef& k = *cur;
            if ((k->parent && (*k->parent == S("Thread") || *k->parent == S("Runnable"))) || k->interfaces.contains(S("Runnable")) ||
                k->interfaces.contains(S("Thread"))) {
                thread_like.add(cls->name);
                break;
            }
            cur = (k->parent && by_name.contains(*k->parent)) ? &by_name.at(*k->parent) : nullptr;
        }
    }
    return thread_like;
}

const py::re::Pattern& _RETURN_NULL() {
    static const py::re::Pattern p = py::re::compile(S("\\breturn\\s+null\\s*;"));
    return p;
}

// Java collection mutators. Calling one of these ON a collection PARAM mutates the CALLER's collection
// (Java passes the reference), so that param is an out / in-out param. The converter models collections as
// value structs (JList<T>), so a by-value param silently COPIES -- the callee's add()/remove()/put() are
// lost. That is the getCollidingBoundingBoxes "player falls through the world" class of bug.
const py::list<str> _COLL_MUTATORS = {S("add"),        S("addAll"),       S("addElement"),    S("addFirst"),    S("addLast"),
                                      S("insertElementAt"), S("set"),     S("setElementAt"),  S("replaceAll"),  S("sort"),
                                      S("remove"),     S("removeAll"),    S("removeElement"), S("removeElementAt"),
                                      S("removeAllElements"), S("removeFirst"), S("removeLast"), S("retainAll"), S("clear"),
                                      S("put"),        S("putAll"),       S("putIfAbsent"),   S("merge"),       S("replace"),
                                      S("push"),       S("pop"),          S("offer"),         S("poll")};

namespace {

// (patterns made once for a name; Python's re module caches them the same way)
const py::re::Pattern& call_pattern_(const str& name) {
    static std::unordered_map<std::string, py::re::Pattern> cache;
    std::string k = name.utf8();
    auto it = cache.find(k);
    if (it == cache.end()) it = cache.emplace(k, py::re::compile(S("\\b") + py::re::escape(name) + S("\\s*\\("))).first;
    return it->second;
}

}  // namespace

py::list<std::tuple<int64_t, str>> _iter_call_args(const str& body, const str& name, int64_t arity) {
    py::list<std::tuple<int64_t, str>> out;
    const py::re::Pattern& pat = call_pattern_(name);
    for (const py::re::Match& mt : pat.finditer(body)) {
        int64_t k = mt.end() - 1;   // index of the opening '('
        int64_t depth = 0;
        py::list<str> args;
        std::u32string cur;
        int64_t n = body.size();
        while (k < n) {
            char32_t ch = body.at_(k);
            if (ch == U'(') {
                depth += 1;
                if (depth == 1) {
                    k += 1;
                    continue;
                }
            } else if (ch == U')') {
                depth -= 1;
                if (depth == 0) {
                    args.append(str(cur));
                    break;
                }
            }
            if (depth == 1 && ch == U',') {
                args.append(str(cur));
                cur.clear();
                k += 1;
                continue;
            }
            if (depth >= 1) cur.push_back(ch);
            k += 1;
        }
        py::list<str> stripped;
        for (const str& a : args) stripped.append(a.strip());
        if (stripped.size() == 1 && stripped[0].empty()) stripped = py::list<str>();   // zero-arg call
        if (stripped.size() == arity) {
            int64_t idx = 0;
            for (const str& a : stripped) out.append(std::make_tuple(idx++, a));
        }
    }
    return out;
}

int64_t compute_collection_out_params(const py::list<ClassRef>& classes, const types::TypeResolver&) {
    auto is_coll_value_param = [](const py::ref<ir::Param>& p) {
        return p->jtype && !p->jtype->is_array() && typemap::CONTAINERS.contains(p->jtype->name);
    };

    py::list<std::tuple<ClassRef, MethodRef>> methods;
    for (const ClassRef& c : classes)
        for (const MethodRef& m : c->methods) methods.append(std::make_tuple(c, m));
    py::list<str> mut_alt;
    for (const str& m : _COLL_MUTATORS) mut_alt.append(m);
    str mut_pattern = S("\\.\\s*(?:") + py::join(S("|"), mut_alt) + S(")\\s*\\(");

    // inheritance closure (parent chain + interfaces) -> relate override-family members
    py::dict<str, ClassRef> by_name;
    for (const ClassRef& c : classes) by_name.set(c->name, c);

    auto ancestors = [&](const str& cname) {
        py::set<str> seen;
        py::list<str> stack{cname};
        while (py::truthy(stack)) {
            str top = stack.pop();
            if (!by_name.contains(top)) continue;
            const ClassRef& cur = by_name.at(top);
            py::list<str> pars;
            if (cur->parent && !cur->parent->empty()) pars.append(*cur->parent);
            pars.extend(cur->interfaces);
            for (const str& par : pars) {
                if (!par.empty() && !seen.contains(par)) {
                    seen.add(par);
                    stack.append(par);
                }
            }
        }
        return seen;
    };

    py::dict<str, py::set<str>> anc;
    for (const ClassRef& c : classes) anc.set(c->name, ancestors(c->name));

    auto related = [&](const str& n1, const str& n2) {
        return n1 == n2 || (anc.contains(n1) && anc.at(n1).contains(n2)) || (anc.contains(n2) && anc.at(n2).contains(n1));
    };

    using MKey = int64_t;   // id(method)
    auto id_of = [](const MethodRef& m) { return static_cast<int64_t>(reinterpret_cast<std::uintptr_t>(m.get())); };
    py::dict<MKey, py::set<int64_t>> out_pos;   // id(method) -> set of out positions
    py::dict<MKey, str> body_cache;
    for (const auto& cm : methods) {
        const MethodRef& m = std::get<1>(cm);
        py::list<str> lines;
        for (const auto& t : m->body) lines.append(std::get<1>(t));
        body_cache.set(id_of(m), join_nl(lines));
    }

    // (1) seed: direct mutation of a collection param in the body
    for (const auto& cm : methods) {
        const MethodRef& m = std::get<1>(cm);
        py::set<int64_t>& s = out_pos.setdefault(id_of(m), py::set<int64_t>());
        const str& body = body_cache.at(id_of(m));
        int64_t i = 0;
        for (const auto& p : m->params) {
            if (is_coll_value_param(p)) {
                py::re::Pattern re = py::re::compile(S("\\b") + py::re::escape(p->name) + S("\\b") + mut_pattern);
                if (re.search(body)) s.add(i);
            }
            i++;
        }
    }

    py::dict<std::tuple<str, int64_t>, py::list<std::tuple<ClassRef, MethodRef>>> by_key;   // (name, arity) -> [(cls, method)]
    for (const auto& cm : methods) {
        const MethodRef& m = std::get<1>(cm);
        by_key.setdefault(std::make_tuple(m->name, m->arity()), py::list<std::tuple<ClassRef, MethodRef>>()).append(cm);
    }

    // (2)+(3) propagate to a fixpoint
    bool changed = true;
    while (changed) {
        changed = false;
        // override-family union
        for (const auto& kv : by_key.items()) {
            const auto& group = std::get<1>(kv);
            int64_t arity = std::get<1>(std::get<0>(kv));
            if (group.size() < 2) continue;
            for (int64_t i = 0; i < arity; i++) {
                py::list<std::tuple<ClassRef, MethodRef>> holders;
                for (const auto& cm : group) {
                    const MethodRef& m = std::get<1>(cm);
                    if (i < m->params.size() && is_coll_value_param(m->params[i])) holders.append(cm);
                }
                if (holders.size() < 2) continue;
                py::list<std::tuple<ClassRef, MethodRef>> hot;
                for (const auto& cm : holders) {
                    auto it = out_pos.get(id_of(std::get<1>(cm)));
                    if (it && it->contains(i)) hot.append(cm);
                }
                for (const auto& c1m1 : hot) {
                    for (const auto& c2m2 : holders) {
                        const MethodRef& m2 = std::get<1>(c2m2);
                        auto it = out_pos.get(id_of(m2));
                        if (related(std::get<0>(c1m1)->name, std::get<0>(c2m2)->name) && !(it && it->contains(i))) {
                            out_pos.setdefault(id_of(m2), py::set<int64_t>()).add(i);
                            changed = true;
                        }
                    }
                }
            }
        }
        // forwarding: param passed into a callee's out slot
        py::dict<std::tuple<str, int64_t>, py::set<int64_t>> name_out;   // (name, arity) -> set of out positions (unioned)
        for (const auto& cm : methods) {
            const MethodRef& m = std::get<1>(cm);
            auto it = out_pos.get(id_of(m));
            if (it) {
                for (int64_t i : *it) name_out.setdefault(std::make_tuple(m->name, m->arity()), py::set<int64_t>()).add(i);
            }
        }
        for (const auto& cm : methods) {
            const MethodRef& m = std::get<1>(cm);
            py::list<int64_t> todo;
            {
                auto it = out_pos.get(id_of(m));
                int64_t i = 0;
                for (const auto& p : m->params) {
                    if (is_coll_value_param(p) && !(it && it->contains(i))) todo.append(i);
                    i++;
                }
            }
            if (!py::truthy(todo)) continue;
            const str& body = body_cache.at(id_of(m));
            py::dict<str, int64_t> pname_at;
            for (int64_t i : todo) pname_at.set(m->params[i]->name, i);
            for (const auto& kv : name_out.items()) {
                const str& callname = std::get<0>(std::get<0>(kv));
                int64_t callarity = std::get<1>(std::get<0>(kv));
                const py::set<int64_t>& outposs = std::get<1>(kv);
                for (const auto& ia : _iter_call_args(body, callname, callarity)) {
                    int64_t argi = std::get<0>(ia);
                    const str& argtext = std::get<1>(ia);
                    if (outposs.contains(argi) && pname_at.contains(argtext)) {
                        int64_t i = pname_at.at(argtext);
                        auto it = out_pos.get(id_of(m));
                        if (!(it && it->contains(i))) {
                            out_pos.setdefault(id_of(m), py::set<int64_t>()).add(i);
                            changed = true;
                        }
                    }
                }
            }
        }
    }

    // finalize -> Param.by_ref
    int64_t count = 0;
    for (const auto& cm : methods) {
        const MethodRef& m = std::get<1>(cm);
        auto it = out_pos.get(id_of(m));
        if (!it) continue;
        for (int64_t i : *it) {
            if (i < m->params.size() && !m->params[i]->by_ref) {
                m->params[i]->by_ref = true;
                count += 1;
            }
        }
    }
    return count;
}

std::tuple<py::set<str>, py::dict<str, str>> derive_nullability(const py::list<ClassRef>& classes, const types::TypeResolver& resolver) {
    py::dict<str, py::set<std::tuple<str, bool>>> rt_by_name;
    for (const ClassRef& c : classes) {
        for (const MethodRef& m : c->methods) {
            if (m->is_ctor || !m->return_type) continue;
            rt_by_name.setdefault(m->name, py::set<std::tuple<str, bool>>()).add(std::make_tuple(m->return_type->name, m->return_type->is_array()));
        }
    }
    py::set<str> getters;
    py::dict<str, str> gtypes;
    for (const ClassRef& c : classes) {
        for (const MethodRef& m : c->methods) {
            const auto& rt = m->return_type;
            if (m->is_ctor || !rt || rt->is_array()) continue;
            if (typemap::PRIMITIVES.contains(rt->name) || rt->name == S("String") || typemap::CONTAINERS.contains(rt->name)) continue;   // value return can't form getter()->deref
            if (!resolver.is_pointer(rt)) continue;
            if (rt_by_name.contains(m->name) && rt_by_name.at(m->name).size() > 1) continue;   // ambiguous name across classes -> don't guard globally
            py::list<str> lines;
            for (const auto& t : m->body) lines.append(std::get<1>(t));
            str body = join_nl(lines);
            if (_RETURN_NULL().search(body)) {
                getters.add(m->name);
                gtypes.setdefault(m->name, rt->name);
            }
        }
    }
    return {getters, gtypes};
}

py::set<str> compute_this_escapes(const py::list<ClassRef>& classes) {
    static const py::re::Pattern this_as_value = py::re::compile(S("\\bthis\\b(?!\\s*(?:\\.|->|\\())"));
    py::dict<str, ClassRef> by_name;
    for (const ClassRef& c : classes) by_name.set(c->name, c);
    py::set<str> own;
    for (const ClassRef& c : classes) {
        py::list<str> parts;
        for (const MethodRef& k : c->constructors())
            for (const auto& t : k->body) parts.append(std::get<1>(t));
        for (const auto& f : c->instance_fields())
            if (py::truthy(f->initializer)) parts.append(*f->initializer);
        for (const auto& block : c->inst_inits)
            for (const auto& t : block) parts.append(std::get<1>(t));
        bool hit = false;
        for (const str& t : parts)
            if (this_as_value.search(t)) {
                hit = true;
                break;
            }
        if (hit) own.add(c->name);
        // a Thread is kept by its running thread: `new ThreadSleepForever(..)` starts itself in its
        // constructor (Alpha's timer hack), so deleting it frees a live thread
        if ((c->parent && (*c->parent == S("Thread") || *c->parent == S("TimerTask"))) || c->interfaces.contains(S("Runnable")))
            own.add(c->name);
    }
    py::set<str> out;
    for (const ClassRef& c : classes) {
        const ClassRef* k = &c;
        py::set<str> guard;   // (the Python has none: a parent cycle would not end)
        while (k != nullptr) {
            if (own.contains((*k)->name)) {
                out.add(c->name);
                break;
            }
            if (guard.contains((*k)->name)) break;
            guard.add((*k)->name);
            k = ((*k)->parent && by_name.contains(*(*k)->parent)) ? &by_name.at(*(*k)->parent) : nullptr;
        }
    }
    return out;
}

py::ref<types::TypeResolver> build_resolver(const py::list<ClassRef>& classes, const std::optional<py::set<str>>& value_types) {
    py::set<str> names, enums, typed_enums, ifaces;
    for (const ClassRef& c : classes) {
        names.add(c->name);
        if (c->is_enum) enums.add(c->name);
        if (c->is_enum && c->is_typed_enum()) typed_enums.add(c->name);
        if (c->is_interface) ifaces.add(c->name);
    }
    py::set<str> vals = value_types ? py::set<str>::from(*value_types) : py::set<str>::from(typemap::DEFAULT_VALUE_TYPES);
    auto resolver = std::make_shared<types::TypeResolver>(names, vals, ifaces, enums, build_member_env(classes), build_method_renames(classes),
                                                          typed_enums, compute_thread_like(classes));
    // whole-program no-GC caller-frees catalog for the source emitter: owned containers (Level.getCubes ->
    // new AABBs) get an RAII element-deleter at their call sites; owned scalar returns used inline as
    // arguments (getCubes(bb.expand(...))) get hoisted + RAII-deleted, but only where the receiving method
    // provably just reads the arg (retained_params / corpus_methods gate).
    types::OwnershipCatalog cat;
    cat.return_methods = emit::ownership::owned_return_container_methods(classes, *resolver);
    cat.scalar_methods = emit::ownership::owned_scalar_return_methods(classes, *resolver);
    cat.retained_params = emit::ownership::retained_param_positions(classes);
    for (const ClassRef& c : classes)
        for (const MethodRef& m : c->methods)
            if (!m->is_ctor) cat.corpus_methods.add(m->name);
    resolver->own_catalog = cat;
    return resolver;
}

// ================================================================================================== the standard
CppStandard::CppStandard(int64_t year_) : year(year_), label(py::percent(S("C++%02d"), year_ % 100)) {}

const py::dict<str, int64_t> _STD_MAP = {{S("C++98"), 1998}, {S("C++03"), 2003}, {S("C++11"), 2011}, {S("C++14"), 2014},
                                         {S("C++17"), 2017}, {S("C++20"), 2020}, {S("C++23"), 2023}};

CppStandard _parse_std_version(const str& label) { return CppStandard(_STD_MAP.get(label, 2003)); }

// ================================================================================================== prune & coalesce
str _read(const str& path) {
    // strict decode + raise on missing (matches the original pipeline behavior)
    return textutil::read_text(path, S("strict"), false);
}

void _write(const str& path, const str& text) { textutil::write_text(path, text); }

py::set<str> _defined_symbols(const str& text) {
    static const py::re::Pattern classes = py::re::compile(S("\\b(?:class|struct)\\s+([A-Za-z_]\\w*)"));
    // method/ctor/operator DEFINITION owner class: `Class::member(args) {` (has a body). Only DEFINITIONS
    // match (trailing `{`), so `std::vector v;` / `d3d9::glClear(...);` USAGES are NOT captured -- otherwise
    // every file would "provide" std/d3d9 and nothing could ever be pruned.
    static const py::re::Pattern owners = py::re::compile(S("\\b([A-Za-z_]\\w*)::[~A-Za-z_]\\w*\\s*\\([^;{}]*\\)\\s*(?:const\\s*)?\\{"));
    // free-function DEFINITION at column 0: `<ret> name(args) {`. No `:` allowed in the prefix, so a method
    // definition (which has `Class::`) is NOT re-captured here as its bare method name.
    static const py::re::Pattern frees =
        py::re::compile(S("(?m)^[A-Za-z_][\\w<>,\\*&\\s]*?\\b([A-Za-z_]\\w*)\\s*\\([^;{}]*\\)\\s*(?:const\\s*)?\\{"));
    py::set<str> syms;
    for (const auto& m : classes.finditer(text)) syms.add(m.group(1));
    for (const auto& m : owners.finditer(text)) syms.add(m.group(1));
    for (const auto& m : frees.finditer(text)) syms.add(m.group(1));
    return syms;
}

py::list<str> _coalesce_forwarders(const str& out_dir, const Progress& prog) {
    static const py::re::Pattern block_comment = py::re::compile(S("/\\*.*?\\*/"), py::re::S);
    static const py::re::Pattern line_comment = py::re::compile(S("//[^\\n]*"));
    auto strip = [&](const str& s) { return line_comment.sub(S(""), block_comment.sub(S(""), s)); };
    static const py::re::Pattern inc_re = py::re::compile(S("#\\s*include\\s+\"([^\"]+\\.h)\""));
    static const py::re::Pattern guard_re = py::re::compile(S("^\\s*#\\s*(?:ifndef|define|endif|pragma\\b).*$"));
    static const py::re::Pattern onlyinc_re = py::re::compile(S("^\\s*#\\s*include\\s+\"[^\"]+\\.h\"\\s*$"));
    py::dict<str, str> raw;
    for (const str& f : py::os::listdir(out_dir)) {
        if (f.endswith(S(".h")) || f.endswith(S(".cpp"))) raw.set(f, _read(py::os::path::join(out_dir, f)));
    }

    py::dict<str, str> fwd;   // forwarder filename -> the single target header it #includes
    for (const str& f : raw.keys()) {
        if (!f.endswith(S(".h"))) continue;
        str body = strip(raw.at(f));
        py::list<str> incs = inc_re.findall(body);
        py::list<str> meaningful;
        for (const str& ln : py::splitlines(body))
            if (py::truthy(ln.strip()) && !guard_re.match(ln)) meaningful.append(ln);
        bool all_inc = true;
        for (const str& ln : meaningful)
            if (!onlyinc_re.match(ln)) all_inc = false;
        if (incs.size() == 1 && py::truthy(meaningful) && all_inc) fwd.set(f, incs[0]);
    }
    if (!py::truthy(fwd)) return py::list<str>();

    // follow forwarder chains to the real (non-fwd) target
    auto resolve = [&](str h) {
        py::set<str> seen;
        while (fwd.contains(h) && !seen.contains(h)) {
            seen.add(h);
            h = fwd.at(h);
        }
        return h;
    };
    py::dict<str, str> alias;
    for (const str& f : fwd.keys()) alias.set(f, resolve(f));

    for (const str& f : raw.keys()) {
        if (fwd.contains(f)) continue;
        str neu = inc_re.sub(
            [&](const py::re::Match& m) { return S("#include \"") + alias.get(m.group(1), m.group(1)) + S("\""); }, raw.at(f));
        if (neu != raw.at(f)) _write(py::os::path::join(out_dir, f), neu);
    }

    py::list<str> removed;
    for (const str& f : py::sorted(fwd.keys())) {
        try {
            py::os::remove(py::os::path::join(out_dir, f));
            removed.append(f);
        } catch (const py::OSError&) {
        }
    }
    if (py::truthy(removed)) {
        prog(py::percent(S("[coalesce] folded %d forwarder header(s) into their targets: %s"), removed.size(), py::join(S(", "), removed)));
    }
    return removed;
}

py::list<str> _prune_unreferenced(const str& out_dir, const py::set<str>& game_classes, const Progress& prog, const py::list<str>& overlay_files) {
    py::list<str> files;
    for (const str& f : py::os::listdir(out_dir))
        if (f.endswith(S(".h")) || f.endswith(S(".cpp"))) files.append(f);
    py::set<str> fileset = py::set<str>::from(files);
    auto base = [](const str& f) { return f.endswith(S(".h")) ? f.slice(0, f.size() - 2) : f.slice(0, f.size() - 4); };
    static const py::re::Pattern inc_re = py::re::compile(S("#\\s*include\\s+\"([^\"]+\\.h)\""));
    static const py::re::Pattern block_comment = py::re::compile(S("/\\*.*?\\*/"), py::re::S);
    static const py::re::Pattern line_comment = py::re::compile(S("//[^\\n]*"));
    static const py::re::Pattern ident = py::re::compile(S("\\b[A-Za-z_]\\w*\\b"));
    py::dict<str, py::set<str>> incs, provides, idents;
    py::dict<str, str> raw, code;
    for (const str& f : files) {
        try {
            raw.set(f, textutil::read_text(py::os::path::join(out_dir, f), S("strict"), false));
        } catch (const py::Exception&) {
            raw.set(f, str());
        }
        incs.set(f, py::set<str>::from(inc_re.findall(raw.at(f))));
        code.set(f, line_comment.sub(S(""), block_comment.sub(S(""), raw.at(f))));
        provides.set(f, _defined_symbols(code.at(f)));
    }

    // Build-critical spine + entry glue: always roots. These are reachable from the game anyway (it
    // #includes JavaRT etc.); listing them is insurance so a scan gap can never drop the foundation.
    py::list<str> FOUNDATION{S("stdafx"), S("JavaRT"), S("JavaString"), S("JArray"), S("JList"), S("JMap"), S("JSet"), S("JObject"),
                             S("JCat"),   S("Common"), S("j2x_init"),   S("j2x_rtti"), S("Main")};
    py::set<str> roots = py::set<str>::from(game_classes);
    roots.update(FOUNDATION);
    for (const str& f : files) {   // link-critical files self-declare (HeapTrack, ...)
        if (raw.at(f).contains(S("J2X-ALWAYS-LINK"))) roots.add(base(f));
    }
    // Overlay-written files are roots by definition -- a deliberate hand-placed file outranks the include
    // graph. (Only the basename matters: the prune works on the flat out_dir listing.)
    for (const str& rel : overlay_files) {
        str fn = py::os::path::basename(rel);
        if (fn.endswith(S(".h")) || fn.endswith(S(".cpp"))) roots.add(base(fn));
    }

    py::set<str> reachable;

    auto walk = [&](const py::list<str>& seed_files) {
        py::list<str> stack = seed_files.copy();
        while (py::truthy(stack)) {
            str f = stack.pop();
            if (reachable.contains(f) || !fileset.contains(f)) continue;
            reachable.add(f);
            if (f.endswith(S(".h"))) {   // a reached .h drags its .cpp partner (same TU)
                str cpp = base(f) + S(".cpp");
                if (fileset.contains(cpp) && !reachable.contains(cpp)) stack.append(cpp);
            }
            for (const str& inc : incs.at(f)) {
                if (fileset.contains(inc) && !reachable.contains(inc)) stack.append(inc);
            }
        }
    };

    {
        py::list<str> seeds;
        for (const str& f : files)
            if (roots.contains(base(f))) seeds.append(f);
        walk(seeds);
    }
    // Symbol-defense fixpoint: keep any file that DEFINES a name referenced by already-reachable CODE
    // (catches cross-named implementation files the #include graph misses), then re-walk their includes.
    bool changed = true;
    while (changed) {
        changed = false;
        py::set<str> ref_idents;
        for (const str& f : reachable) {
            if (!idents.contains(f)) idents.set(f, py::set<str>::from(ident.findall(code.at(f))));   // (once per file: same union)
            ref_idents.update(idents.at(f));
        }
        for (const str& f : files) {
            if (reachable.contains(f)) continue;
            bool hit = false;
            for (const str& s : provides.at(f))
                if (ref_idents.contains(s)) {
                    hit = true;
                    break;
                }
            if (hit) {
                walk(py::list<str>{f});
                changed = true;
            }
        }
    }

    py::list<str> dropped;
    for (const str& f : py::sorted(files)) {
        if (reachable.contains(f) || roots.contains(base(f))) continue;
        try {
            py::os::remove(py::os::path::join(out_dir, f));
            dropped.append(f);
        } catch (const py::OSError&) {
        }
    }
    if (py::truthy(dropped)) {
        py::set<str> bases;
        for (const str& f : dropped) bases.add(base(f));
        prog(py::percent(S("[prune] cut %d file(s) in dead subsystems no live #include reaches (%d subsystem(s)): %s"), dropped.size(),
                         bases.size(), py::join(S(", "), py::list<str>::from(py::sorted(bases)))));
    }
    return dropped;
}

// ================================================================================================== static-init order
// Dependency order for static initialization (the static-init-order crash class). Seeded from the old
// port's proven Main.cpp order: enums are header-only const-int (no runtime init, omitted); Material before
// Block (block ctors read Material statics); Item + CraftingManager after Block (items/recipes read block
// statics); then the class registries. Anything not listed runs after, in a stable sorted order. To fix a
// missed dependency found on hardware, edit THIS list.
py::list<str> _STATIC_INIT_ORDER;

void _reseed_static_init_order(const profiles::Profile& p) { _STATIC_INIT_ORDER = p.static_init_order.copy(); }

py::list<str> compute_static_init_order(const py::list<ClassRef>& classes, const py::set<str>& callable_init) {
    py::dict<str, ClassRef> by_name;
    for (const ClassRef& c : classes) by_name.set(c->name, c);
    static const py::re::Pattern caps = py::re::compile(S("\\b([A-Z]\\w*)\\b"));
    static const py::re::Pattern news = py::re::compile(S("\\bnew\\s+([A-Z]\\w*)"));

    // Text that runs when an object of `cls` is constructed: its ctor bodies + instance-field initializers
    // + instance `{ }` blocks (all execute during construction).
    auto ctor_and_inst_text = [](const ClassRef& cls) {
        py::list<str> t;
        for (const MethodRef& m : cls->constructors())
            for (const auto& b : m->body) t.append(std::get<1>(b));
        for (const auto& f : cls->fields)
            if (!f->is_static && py::truthy(f->initializer)) t.append(*f->initializer);
        for (const auto& blk : cls->inst_inits)
            for (const auto& b : blk) t.append(std::get<1>(b));
        return t;
    };

    // J2X-BUGFIX 2026-08-13 static-init-order-ctor: a class whose initStatics() CONSTRUCTS objects
    // (`new X`) runs X's ctor chain -- and those ctors read other classes' statics. That dependency is
    // invisible if you only scan the class's own static-init text: BiomeGenBase::initStatics() does
    // `new BiomeGenRainforest()`, whose (inherited BiomeGenBase) ctor reads Block::s_grass -> BiomeGenBase
    // must init AFTER Block, but its static-init text names Block zero times. So FOLLOW `new X` transitively
    // into X's ctor + instance inits + ancestor ctors, collecting their static refs as real deps.
    auto direct_refs = [&](const ClassRef& c) {
        py::list<str> txt;
        for (const auto& f : c->fields)
            if (f->is_static && py::truthy(f->initializer)) txt.append(*f->initializer);
        for (const auto& blk : c->static_inits)
            for (const auto& b : blk) txt.append(std::get<1>(b));
        for (const auto& ec : c->enum_consts)   // typed-enum constant args build in initStatics()
            txt.extend(ec->args);               // (e.g. EnumCreatureType reads Material.air)
        py::list<str> nonempty;
        for (const str& t : txt)
            if (py::truthy(t)) nonempty.append(t);
        str joined = py::join(S(" "), nonempty);
        py::set<str> refs = py::set<str>::from(caps.findall(joined));
        // transitively follow `new X` into constructed classes' ctor/instance/ancestor bodies
        py::list<str> todo = news.findall(joined);
        py::set<str> seen;
        while (py::truthy(todo)) {
            str x = todo.pop();
            if (seen.contains(x)) continue;
            seen.add(x);
            const ClassRef* anc = by_name.contains(x) ? &by_name.at(x) : nullptr;
            while (anc != nullptr && (*anc)->name != S("")) {   // walk x + its superclass chain
                py::list<str> bj_parts;
                for (const str& t : ctor_and_inst_text(*anc))
                    if (py::truthy(t)) bj_parts.append(t);
                str bj = py::join(S(" "), bj_parts);
                refs.update(caps.findall(bj));
                todo.extend(news.findall(bj));
                const ClassRef* nxt = ((*anc)->parent && by_name.contains(*(*anc)->parent)) ? &by_name.at(*(*anc)->parent) : nullptr;
                if (nxt == nullptr || seen.contains((*nxt)->name)) break;
                anc = nxt;
            }
        }
        py::set<str> out;   // only OTHER initable classes are real deps
        for (const str& r : refs)
            if (callable_init.contains(r) && r != c->name) out.add(r);
        return out;
    };

    py::dict<str, py::set<str>> deps;
    for (const str& n : callable_init) {
        if (by_name.contains(n)) deps.set(n, direct_refs(by_name.at(n)));
    }

    // J2X-BUGFIX 2026-08-13 static-init-order-seed: the SEED must be a HARD ordering chain, not just a
    // tiebreaker priority. CraftingManager::initStatics() is only `s_instance = new CraftingManager()`, so
    // direct_refs sees no deps (the real Item::/Block:: reads are buried in the ctor -> RecipesTools::
    // addRecipes) -> it floated to position 2, ran before Item/Block, and derefed NULL Item statics ->
    // crash in j2x_initAll (black screen). Chain consecutive seed members as real deps so those ctor-
    // mediated orderings hold: each seeded class present in the graph must run after the previous one.
    py::list<str> seed_present;
    for (const str& n : _STATIC_INIT_ORDER)
        if (deps.contains(n)) seed_present.append(n);
    for (int64_t i = 1; i < seed_present.size(); i++) {
        py::set<str> d = py::set<str>::from(deps.at(seed_present[i]));
        d.add(seed_present[i - 1]);
        deps.set(seed_present[i], d);
    }

    py::dict<str, int64_t> prio;
    {
        int64_t i = 0;
        for (const str& name : _STATIC_INIT_ORDER) prio.set(name, i++);
    }
    int64_t last = _STATIC_INIT_ORDER.size();
    auto order_key = [&](const str& n) { return std::make_tuple(prio.get(n, last), n); };

    py::list<str> ordered;
    py::set<str> placed;
    py::set<str> remaining = py::set<str>::from(deps.keys());
    while (py::truthy(remaining)) {
        std::vector<str> ready;
        for (const str& n : remaining)
            if (deps.at(n).issubset(placed)) ready.push_back(n);
        if (ready.empty()) {   // dependency cycle -> break by priority
            for (const str& n : remaining) ready.push_back(n);
        }
        str pick = *std::min_element(ready.begin(), ready.end(), [&](const str& a, const str& b) { return order_key(a) < order_key(b); });
        ordered.append(pick);
        placed.add(pick);
        remaining.discard(pick);
    }
    // any callable class not present in `classes` (shouldn't happen) -> append stably
    py::list<str> rest;
    for (const str& c : callable_init)
        if (!placed.contains(c)) rest.append(c);
    for (const str& c : py::sorted(rest)) ordered.append(c);
    return ordered;
}

void _write_init_driver(const str& out_dir, const py::list<str>& init_classes, const py::list<ClassRef>& classes) {
    py::set<str> callable_init = py::set<str>::from(init_classes);
    py::list<str> ordered = compute_static_init_order(classes, callable_init);

    _write(py::os::path::join(out_dir, S("j2x_init.h")),
           S("#ifndef J2X_INIT_H\n#define J2X_INIT_H\n"
             "// Runs every class's static initializers (Java static{} + static field inits) in\n"
             "// dependency order. CALL ONCE at startup, before any game code. Generated by CodeBreaker.\n"
             "void j2x_initAll();\n#endif // J2X_INIT_H\n"));

    // J2X-BUGFIX 2026-08-13 instanceof-isa: C++ typeid is EXACT; Java instanceof is IS-A. is<T>() (JObject)
    // therefore missed every subclass (x instanceof Block was false for BlockOreStorage) -- silently
    // breaking recipes/entities/GUI. Emit a class-ancestry registry (each class -> its base + interfaces)
    // that is<T>() walks. Separate TU (all headers) so a bad include can't break the init driver.
    py::set<str> names;
    for (const ClassRef& c : classes) names.add(c->name);
    py::list<std::tuple<str, str>> edges;
    for (const ClassRef& c : classes) {
        if (c->parent && py::truthy(*c->parent)) {
            str b = py::split(*c->parent, S("<"))[0].strip();
            if (names.contains(b) && b != c->name) edges.append(std::make_tuple(c->name, b));
        }
        for (const str& iface : c->interfaces) {
            str b = py::split(iface, S("<"))[0].strip();
            if (names.contains(b) && b != c->name) edges.append(std::make_tuple(c->name, b));
        }
    }
    _write(py::os::path::join(out_dir, S("j2x_rtti.h")),
           S("#ifndef J2X_RTTI_H\n#define J2X_RTTI_H\n"
             "// Registers each class's base/interface ancestry so JObject::is<T>() (instanceof) matches\n"
             "// subclasses, not just the exact dynamic type. Call ONCE, before any instanceof runs.\n"
             "void j2xRegisterRtti();\n"
             "void j2xPrepopulateClassReg();   // pre-fill _j2xClassReg at BOOT (thread-safe RTTI under MP net threads)\n"
             "#endif // J2X_RTTI_H\n"));
    py::list<str> rtti{S("#include \"stdafx.h\""), S("#include \"JavaRT.h\""), S("#include \"j2x_rtti.h\""), S("#include \"Class.h\""), S("")};
    py::list<str> sorted_names = py::list<str>::from(py::sorted(names));
    for (const str& n : sorted_names) rtti.append(S("#include \"") + n + S(".h\""));
    // Java value equality: each concrete class that defines equals(Object) or hashCode() (a HashMap key such as
    // ChunkPosition) -- JObject compares and orders by them (JavaRT.h j2xRegisterValue)
    py::list<str> valued;
    for (const ClassRef& c : classes) {
        if (!names.contains(c->name) || c->is_interface || c->is_abstract) continue;
        py::list<MethodRef> ms;
        for (const MethodRef& m : c->methods)
            if (!m->is_ctor && !m->is_static && !m->is_abstract) ms.append(m);
        bool eq = false, hc = false;
        for (const MethodRef& m : ms) {
            if (m->name == S("equals") && m->params.size() == 1 && m->params[0]->jtype->name == S("Object")) eq = true;
        }
        for (const MethodRef& m : ms) {
            if (m->name == S("hashCode") && !py::truthy(m->params)) hc = true;
        }
        if (eq || hc) {
            valued.append(c->name);
            rtti.append(py::percent(S("static bool j2xEq_%s(void* s, const JObject& o) { return %s; }"), c->name,
                                    eq ? py::percent(S("((%s*)s)->equals(o)"), c->name) : S("s == o.p")));
            rtti.append(py::percent(S("static int j2xHash_%s(void* s) { return %s; }"), c->name,
                                    hc ? py::percent(S("((%s*)s)->hashCode()"), c->name) : S("(int)(std::size_t)s")));
        }
    }
    rtti.extend(py::list<str>{S(""), S("void j2xRegisterRtti()"), S("{")});
    for (const auto& e : edges)
        rtti.append(py::percent(S("\tj2xRegisterBase(typeid(%s).name(), typeid(%s).name());"), std::get<0>(e), std::get<1>(e)));
    for (const str& n : valued)
        rtti.append(py::percent(S("\tj2xRegisterValue(typeid(%s).name(), j2xEq_%s, j2xHash_%s);"), n, n, n));
    rtti.extend(py::list<str>{S("}"), S("")});
    // FIXED BUG classreg-multithread-race: pre-materialise EVERY class into _j2xClassReg AT BOOT (before the MP
    // network reader/writer threads start), so j2xClassOf never lazily push_backs the shared registry during play.
    // That keeps it READ-ONLY under concurrency -> no reader-thread/main-thread use-after-free (the non-deterministic
    // MP heap-corruption hang). Alpha's runtime had no such registry, which is why Alpha MP never corrupted.
    rtti.extend(py::list<str>{S("void j2xPrepopulateClassReg()"), S("{")});
    for (const str& n : sorted_names) rtti.append(py::percent(S("\tj2xClassOf(typeid(%s));"), n));
    rtti.extend(py::list<str>{S("}"), S("")});
    _write(py::os::path::join(out_dir, S("j2x_rtti.cpp")), join_nl(rtti));

    py::list<str> lines{S("#include \"stdafx.h\""), S("#include \"JavaRT.h\""), S("#include \"j2x_init.h\""), S("#include \"j2x_rtti.h\""), S("")};
    for (const str& c : ordered) lines.append(S("#include \"") + c + S(".h\""));
    // J2X-BUGFIX 2026-08-13 init-trace: emit a per-class boot trace so a crash INSIDE j2x_initAll (a class
    // whose initStatics/ctor derefs a not-yet-initialized static) is pinpointed -- the last [INIT] line
    // logged names the offender. One-shot at boot (sparse -> safe per the DbgPrintF rule); gate J2X_INIT_TRACE
    // to 0 to strip it from a release build. Uses raw DbgPrint (surfaces once main() is running).
    lines.extend(py::list<str>{
        S(""),
        S("extern \"C\" unsigned long __cdecl DbgPrint(const char* Format, ...);"),
        S("#ifndef J2X_INIT_TRACE"),
        S("#define J2X_INIT_TRACE 0   // default OFF (floods boot log). Set 1 to pinpoint a static-init crash."),
        S("#endif"),
        S(""),
        S("void j2x_initAll()"),
        S("{"),
        S("\tj2xRegisterRtti();   // instanceof ancestry -- MUST precede any is<T>() (recipe/entity init)"),
        S("\tj2xPrepopulateClassReg();   // pre-fill the class registry at BOOT so RTTI is read-only under MP net threads")});
    for (const str& c : ordered) {
        lines.append(S("#if J2X_INIT_TRACE"));
        lines.append(py::percent(S("\tDbgPrint(\"[INIT] %s\\n\");"), c));
        lines.append(S("#endif"));
        lines.append(py::percent(S("\t%s::initStatics();"), c));
    }
    lines.extend(py::list<str>{S("}"), S("")});
    _write(py::os::path::join(out_dir, S("j2x_init.cpp")), join_nl(lines));
}

// ================================================================================================== convert
namespace {

std::mutex& one_at_a_time_() {
    static std::mutex m;
    return m;
}

str e_what_(const std::exception& e) { return str(std::string(e.what())); }

Summary convert_(const str& java_dir, const str& out_dir, const std::optional<py::set<str>>& value_types, py::ref<ledger::Ledger> ledger,
                 bool write_report, const str& cpp_std, const std::optional<str>& platform, const Progress& progress_cb, bool platform_refs,
                 const std::optional<str>& sdk_override, bool emit_debug, const std::optional<str>& overlay_dir,
                 const std::optional<str>& edition, const std::optional<str>& mc_version);

}  // namespace

Summary convert(const str& java_dir, const str& out_dir, const std::optional<py::set<str>>& value_types, const py::ref<ledger::Ledger>& ledger,
                bool write_report, const str& cpp_std, const std::optional<str>& platform, const Progress& progress_cb, bool platform_refs,
                const std::optional<str>& sdk_override, bool emit_debug, const std::optional<str>& overlay_dir,
                const std::optional<str>& edition, const std::optional<str>& mc_version) {
    std::lock_guard<std::mutex> hold(one_at_a_time_());
    // _convert_restoring: the per-conversion module state is put back when it ends, even by an exception
    str prev_profile = profiles::active()->name;
    py::set<str> base_getters = py::set<str>::from(typemap::KNOWN_NULLABLE_GETTERS);
    py::dict<str, str> base_gtypes = typemap::NULLABLE_GETTER_RETURN_TYPE.copy();
    auto restore = [&]() {
        typemap::KNOWN_NULLABLE_GETTERS = py::set<str>::from(base_getters);
        typemap::NULLABLE_GETTER_RETURN_TYPE = base_gtypes.copy();
        emit::source::TREE_BODIES = nullptr;   // phase 1's IR bodies never outlive their conversion
        _PROJECT = projects::GENERIC();          // nor does its project
        if (profiles::active()->name != prev_profile) profiles::use(prev_profile);
    };
    try {
        Summary s = convert_(java_dir, out_dir, value_types, ledger, write_report, cpp_std, platform, progress_cb, platform_refs, sdk_override,
                             emit_debug, overlay_dir, edition, mc_version);
        restore();
        return s;
    } catch (...) {
        restore();
        throw;
    }
}

namespace {

Summary convert_(const str& java_dir, const str& out_dir, const std::optional<py::set<str>>& value_types, py::ref<ledger::Ledger> ledger,
                 bool write_report, const str& cpp_std, const std::optional<str>& platform, const Progress& progress_cb, bool platform_refs,
                 const std::optional<str>& sdk_override, bool emit_debug, const std::optional<str>& overlay_dir,
                 const std::optional<str>& edition, const std::optional<str>& mc_version) {
    using ledger::Category;
    using ledger::Severity;
    if (!ledger) ledger = std::make_shared<ledger::Ledger>();
    CppStandard std_version = _parse_std_version(cpp_std);
    Progress _prog = progress_cb ? progress_cb : Progress([](const str&) {});
    py::os::makedirs(out_dir, 0777, true);
    // The 360 target ships a COMPLETE, self-contained platform layer by default (entry point, GL->D3D9
    // emulator, shaders, textures, audio, input, file IO) -- the user never hand-writes/copies it. Opt
    // out is still possible by passing platform_refs=False explicitly for a non-xbox target.
    auto is_platform = [&](const char* id) { return platform && *platform == str(id); };
    if (is_platform("xbox360") || is_platform("pc_opengl")) platform_refs = true;

    // On the 360 emulator a C++ `throw` is a no-op that doesn't unwind -> tag every emitted throw with
    // its site (JavaRT.h's J2X_GUEST_THROW) so a fired landmine names its file:line in the debug log.
    _TRACE_THROWS = is_platform("xbox360");
    // The 360 runs NO OpenGL: convert every GL call in the emitted game to the Direct3D 9 layer (d3d9).
    _GL_TO_D3D9 = is_platform("xbox360");

    // SDK GATE: refuse platform-specific conversion for a console target with no SDK on the machine.
    if (console::sdk::requires_sdk(platform)) {
        console::sdk::SdkInfo sdk_info = console::sdk::require(platform, sdk_override);   // raises SdkNotFound -> caller reports
        _prog(py::percent(S("[sdk] %s located at %s (via %s)"), sdk_info.name, sdk_info.root, sdk_info.source));
    }

    _prog(py::percent(S("[config] target C++ standard: %s (year %d)"), std_version.label, std_version.year));
    _prog(S("[parse] discovering .java files..."));
    py::list<str> java_paths = discover(java_dir);
    _prog(py::percent(S("[parse] found %d .java files"), java_paths.size()));

    py::list<ClassRef> classes;
    for (const str& path : java_paths) {
        try {
            classes.append(parse::parse_file(path));
        } catch (const py::Exception& e) {   // one bad file shouldn't abort
            _prog(py::percent(S("[parse] FAIL %s: %s"), py::os::path::basename(path), e_what_(e)));
            ledger->add(py::os::path::basename(path), Category::ERROR, py::percent(S("parse failed: %s"), e_what_(e)), 0, Severity::HIGH,
                        S("pipeline"), S("parse-error"));
        }
    }
    _prog(py::percent(S("[parse] parsed %d classes OK"), classes.size()));

    int64_t before = classes.size();
    py::set<str> markers;
    std::tie(classes, markers) = drop_synthetic_artifacts(classes, ledger.get());
    if (classes.size() != before) {
        _prog(py::percent(S("[parse] dropped %d empty marker class(es) + synthetic bridge ctors"), before - classes.size()));
    }

    // Select the per-edition target profile for THIS conversion (explicit `edition` > `mc_version` id
    // > source auto-detection > keep default). profiles.use() reseeds the frozen profile globals in
    // typemap/rules/vcxproj/signatures/pipeline, so it MUST run before build_resolver (which reads
    // typemap.DEFAULT_VALUE_TYPES). convert() restores it in a `finally` (even on an exception), so
    // repeated in-process conversions can't drift.
    _PROJECT = projects::choose(classes, std::nullopt, edition, mc_version);
    if (_PROJECT != projects::GENERIC()) {
        _prog(py::percent(S("[project] %s"), _PROJECT->name));
    }
    str _prev_profile = profiles::active()->name;
    std::optional<str> _ed_name = _PROJECT->profile_for(edition, mc_version, classes);
    if (_ed_name && py::truthy(*_ed_name) && *_ed_name != profiles::active()->name) {
        profiles::use(*_ed_name);
        _prog(py::percent(S("[edition] active target profile: %s"), *_ed_name));
    } else if ((edition && py::truthy(*edition)) || (mc_version && py::truthy(*mc_version))) {
        _prog(py::percent(S("[edition] target profile: %s"), profiles::active()->name));
    }

    _prog(S("[resolve] inheritance + MI forwards + type resolver..."));
    resolve_inheritance(classes);
    compute_mi_forwards(classes);
    py::ref<types::TypeResolver> resolver = build_resolver(classes, value_types);
    resolver->markers = markers;   // dropped EmptyN markers -> strip their call-site args
    // classes whose constructor makes a virtual call Java would dispatch to the override (Block's
    // isOpaqueCube, Entity's entityInit): built in two phases, `(new C())->j2xInit(args)` (see twophase.py)
    py::set<str> two_phase = twophase::compute(classes, resolver->values);
    if (py::truthy(two_phase)) {
        _prog(py::percent(S("[resolve] %d class(es) built in two phases (virtual calls in constructors)"), two_phase.size()));
    }
    _prog(py::percent(S("[resolve] done (%d known classes, %d interfaces, %d enums)"), resolver->known.size(), resolver->interfaces.size(),
                      resolver->enums.size()));

    _THIS_ESCAPES = compute_this_escapes(classes);

    // Java collection out-params -> C++ references. A method that MUTATES a List/Map/Set param writes the
    // caller's collection; modelled as a value struct it would copy instead, silently dropping the writes
    // (the getCollidingBoundingBoxes collision-dead bug). Mark them so the emitter passes them by reference.
    int64_t _coll_ref = compute_collection_out_params(classes, *resolver);
    _prog(py::percent(S("[resolve] %d collection out-param(s) promoted to by-reference"), _coll_ref));

    // DERIVE nullable getters from the corpus and union into the guarded set for THIS conversion, so
    // emit-time null-guarding + the null-deref rule cover getters that can `return null` on any corpus
    // (not just the Alpha whitelist). convert()'s `finally` restores the module globals across calls.
    py::set<str> _base_getters = py::set<str>::from(typemap::KNOWN_NULLABLE_GETTERS);
    auto derived = derive_nullability(classes, *resolver);
    const py::set<str>& d_getters = std::get<0>(derived);
    typemap::KNOWN_NULLABLE_GETTERS.update(d_getters);
    for (const str& k : std::get<1>(derived).keys()) typemap::NULLABLE_GETTER_RETURN_TYPE.setdefault(k, std::get<1>(derived).at(k));
    int64_t new_getters = 0;
    for (const str& g : d_getters)
        if (!_base_getters.contains(g)) new_getters++;
    _prog(py::percent(S("[nullability] derived %d additional nullable getter(s) from the corpus"), new_getters));

    std::optional<str> plat_label;
    if (platform && py::truthy(*platform)) {
        auto plat_info = platforms::get_platform(*platform);
        plat_label = plat_info ? plat_info->label : *platform;
        _prog(py::percent(S("[config] target platform: %s"), *plat_label));
    }

    py::dict<str, str> rt_files = runtime::generate_files(std_version.std(), platform);
    _prog(py::percent(S("[runtime] writing %d support headers..."), rt_files.size()));
    for (const auto& kv : rt_files.items()) _write(py::os::path::join(out_dir, std::get<0>(kv)), std::get<1>(kv));

    // optionally REPLACE the platform stubs with the real hand-written implementations (GL->D3D9
    // emulator, XACT audio, XInput, XNet socket, ...) from docs/platform/reference/. The port stops
    // being blind: it ships the proven Alpha platform layer; PLATFORM_COVERAGE.md reports the delta.
    if (platform_refs) {
        namespace pref = console::platform_refs;
        // the platform's own layer: the 360's (Direct3D 9 / XInput / XACT) or the PC's (SDL2 / OpenGL)
        str _ref_dir0 = is_platform("pc_opengl") ? _paths::data_path(py::list<str>{S("platform"), S("pc"), S("reference")})
                                                 : _paths::data_path(py::list<str>{S("platform"), S("reference")});
        // DETECTION-DRIVEN (no game hardcoding): tell the platform emitter which classes the game
        // defines itself + its main class, so it skips any platform file that collides with a
        // game-defined class and generates a generic entry point instead of the Beta-specific one.
        py::set<str> _game_classes;
        for (const ClassRef& c : classes) _game_classes.add(c->name);
        std::optional<str> _main_class = pref::detect_main_class(classes);
        // No static main? A concrete top-level Runnable game shell (Indev's Minecraft) gets a
        // construct-and-run() entry; Alpha/Beta (abstract Minecraft + MinecraftImpl) return None here
        // and keep the Beta-specific Main.reference.cpp.
        ClassRef _runnable_game = (!_main_class || _main_class->empty()) ? pref::detect_runnable_game_class(classes) : nullptr;
        py::set<str> emitted = pref::emit_references(out_dir, _ref_dir0, _game_classes, _main_class, _runnable_game, platform ? *platform : S("xbox360"));
        _prog(py::percent(S("[platform] emitted %d platform references (agnostic layer, replacing stubs): %s"), emitted.size(),
                          py::join(S(", "), py::list<str>::from(py::sorted(emitted)))));
        py::list<str> _skipped = pref::emit_references_last_skipped;
        if (py::truthy(_skipped)) {
            _prog(py::percent(S("[platform] skipped %d platform file(s) the game defines itself (its converted class "
                                "wins): %s"),
                              _skipped.size(), py::join(S(", "), _skipped)));
        }
        if (_main_class && py::truthy(*_main_class)) {
            _prog(py::percent(S("[platform] generic entry point -> %s::main (detected)"), *_main_class));
        }
        // INTEGRATION GUARD: never let a reference silently fail to emit again (the GLU flat-world bug --
        // a .reference.cpp with no matching .reference.h). Audit for orphans + VERIFY every reference's stub
        // was really replaced in the output; surface any gap LOUDLY.
        pref::AuditResult _audit = pref::audit_references(_ref_dir0);
        py::set<str> _expect;
        for (const str& b : py::sorted(_audit.complete)) {
            if (!pref::REFERENCE_SKIP.contains(b) && !_game_classes.contains(b)) _expect.add(b);
        }
        py::list<str> _expect_sorted = py::list<str>::from(py::sorted(_expect));
        py::list<str> _vfail = pref::verify_integrated(out_dir, py::set<str>::from(_expect_sorted));
        if (py::truthy(_audit.orphans)) {
            _prog(py::percent(S("[platform] !! ORPHAN REFERENCE(S) NOT INTEGRATED (have .reference.cpp but NO .reference.h "
                                "-> game keeps the no-op stub): %s  --> add the matching .reference.h"),
                              py::join(S(", "), _audit.orphans)));
        }
        if (py::truthy(_vfail)) {
            _prog(py::percent(S("[platform] !! REFERENCE(S) FAILED TO REPLACE THEIR STUB in the output: %s"), py::join(S(", "), _vfail)));
        }
        if (!py::truthy(_audit.orphans) && !py::truthy(_vfail)) {
            _prog(py::percent(S("[platform] integration verified: all %d reference(s) replaced their stub"), _expect_sorted.size()));
        }
    } else {
        // The runtime's Inflater/GZIPInputStream include "Inflate.h" -- portable C (puff.c, no platform
        // in it) that otherwise only arrives with the platform layer. Without a platform it comes alone.
        for (const char* ext : {".h", ".cpp"}) {
            _write(py::os::path::join(out_dir, S("Inflate") + str(ext)),
                   _read(_paths::data_path(py::list<str>{S("platform"), S("reference"), S("Inflate.reference") + str(ext)})));
        }
    }

    // THE TRANSLATOR (docs/TRANSLATOR_DESIGN.md): every method body, constructor and initialiser comes from
    // javac's typed tree through codebreaker/translate's IR and C++03 back end.
    py::list<str> _cp = probe::libraries();
    str _shim = py::truthy(_cp) ? probe::shims(state_(py::list<str>{S("translate")}), std::get<0>(verify::lanes::java::jdk())) : str();
    auto _ctx = std::make_shared<cpp03::Context>(resolver, classes);
    _ctx->ledger = ledger;
    _ctx->char_map = _PROJECT->char_map.copy();
    py::list<str> dump_cp;
    if (py::truthy(_shim)) dump_cp.append(_shim);
    dump_cp.extend(_cp);
    py::list<translate::ir::NodeP> _units = cpp03::java_units(java_dir, dump_cp);
    // the console's fixes to the game's own logic, made on the tree before it is written (by shape, so no
    // printing of the C++ hides a site): sound routed to XACT, the applet ping, the render budget and NaN
    projects::Patched _patched = _PROJECT->ir_patches(_units, platform);
    std::shared_ptr<cpp03::Bodies> tree;
    if (std_version.year >= 2017) {   // the C++17 forms (backends/cpp17.Bodies: cpp03.Bodies17)
        tree = translate::backends::cpp17::Bodies(_ctx, _units);
    } else {   // modern Java lowered to what C++03 can say (desugar.py)
        translate::desugar::run(_units);
        tree = std::make_shared<cpp03::Bodies>(_ctx, _units);
    }
    emit::source::TREE_BODIES = tree.get();
    _prog(py::percent(S("[translate] the IR front end lowered %d classes"), tree->decls.size()));

    int64_t headers = 0, sources = 0;
    py::list<str> init_classes;   // concrete classes whose initStatics() does real work
    int64_t total = classes.size();
    int64_t idx = 0;
    for (const ClassRef& cls : classes) {
        str tag = py::percent(S("[%d/%d] %s"), idx + 1, total, cls->name);
        idx++;
        try {
            int64_t ev_before = ledger->size();
            _write(py::os::path::join(out_dir, cls->name + S(".h")),
                   _finalize_emitted(emit::header::render_header(cls, *resolver, ledger.get(), std_version.std(), plat_label, two_phase)));
            py::list<py::ref<ledger::Event>> new_evs = ledger->events.slice(ev_before, ledger->events.size());
            if (py::truthy(new_evs)) {
                for (const auto& ev : new_evs) {
                    if (ev->category != Category::PORTED) _prog(py::percent(S("%s .h %s: %s"), tag, ev->category, ev->message));
                }
            } else {
                _prog(py::percent(S("%s .h generated"), tag));
            }
            headers += 1;

            ev_before = ledger->size();
            std::optional<str> cpp = emit::source::render_source(cls, *resolver, ledger.get(), std_version.std(), plat_label, two_phase);
            if (cpp) {
                _write(py::os::path::join(out_dir, cls->name + S(".cpp")), _finalize_emitted(*cpp));
                sources += 1;
                if (cls->has_init_statics) init_classes.append(cls->name);   // generated class with deferred static init
            }
            new_evs = ledger->events.slice(ev_before, ledger->events.size());
            py::list<py::ref<ledger::Event>> notable;
            for (const auto& ev : new_evs)
                if (ev->category != Category::PORTED) notable.append(ev);
            if (py::truthy(notable)) {
                for (const auto& ev : notable) _prog(py::percent(S("%s .cpp %s: %s"), tag, ev->category, ev->message));
            } else {
                _prog(py::percent(S("%s .cpp generated (clean)"), tag));
            }
        } catch (const py::Exception& e) {
            _prog(py::percent(S("%s ERROR: %s"), tag, e_what_(e)));
            ledger->add(cls->name, Category::ERROR, py::percent(S("emit failed: %s"), e_what_(e)), 0, Severity::HIGH, S("pipeline"), S("emit-error"));
        }
    }

    // ORDERED STATIC-INIT DRIVER: a single j2x_initAll() that calls each class's initStatics() in
    // DEPENDENCY order (C++ gives no cross-TU static-init order -> the static-init-order crash class).
    _write_init_driver(out_dir, init_classes, classes);

    // reflection factories: Java class-registry reflection (getConstructor/newInstance -> NULL stub,
    // the entity-spawn / tile-load / packet-parse crash) becomes a real generated factory switch built
    // from the registration table, and reuse sites (BlockSign/SpawnerAnimals) call it via a shared
    // header. Runs before the rules scan so the reflection-unsupported findings drop.
    auto _rf = reflection_factory::run(out_dir, _PROJECT->registrars);
    if (_rf.total_sites) {
        int64_t reused = 0;
        reused = _rf.reused.size();
        _prog(py::percent(S("[reflect] %d registries -> factories (%d classes), %d sites rewritten (%d reused)"), _rf.registries.size(),
                          _rf.total_classes, _rf.total_sites, reused));
    }

    // the project's fixes and checks on the written program (Minecraft: the console's sound, menu
    // navigation, applet ping and render fixes), before overlays so an overlay still has the last word
    _PROJECT->after_emit(out_dir, platform, _patched, _prog);

    // PROJECT OVERLAYS: re-apply game/version-specific hand-fixes to GENERATED files -- leak `delete`s in
    // specific methods, the scheduled-tick value-comparing std::set, UI tweaks (OSK/FPS/tooltip), input
    // remaps, and hand-written net files (Socket.cpp) -- that do NOT generalize into an emitter rule and
    // would otherwise be WIPED by a reconversion. Runs LAST over the generated tree (after emission +
    // references + reflection) so overlays are the final say; ADDED .cpp are compiled because vcxproj
    // enumerates out_dir afterward. See codebreaker/java/overlays.py. When an overlay turns out to be a general
    // pattern, migrate it into the emitter/runtime/reference and drop the overlay.
    std::optional<str> _ov_dir = overlays::resolve_overlay_dir(out_dir, java_dir, overlay_dir);
    py::list<str> _applied = overlays::apply_overlays(out_dir, _ov_dir, _prog);
    if (py::truthy(_applied)) {
        _prog(py::percent(S("[overlay] %d project overlay file(s) re-applied from %s"), _applied.size(), _ov_dir ? *_ov_dir : S("None")));
    }

    // TWO-PHASE CONSTRUCTION: every `new C(args)` of a two-phase class -- the emitted code's, the generated
    // factories', the reference layer's and the overlays' -- becomes `(new C())->j2xInit(args)`. Last of the
    // rewrites, so nothing after it makes a plain `new C(...)` again.
    twophase::RunResult _tp = twophase::run(out_dir, two_phase);
    if (_tp.sites) {
        _prog(py::percent(S("[twophase] %d construction site(s) across %d file(s) -> (new C())->j2xInit(...)"), _tp.sites, _tp.files.size()));
    }
    py::list<str> _stale;
    for (const str& c : py::sorted(two_phase)) {
        if (py::os::path::isfile(py::os::path::join(out_dir, c + S(".cpp"))) &&
            !_read(py::os::path::join(out_dir, c + S(".cpp"))).contains(py::percent(S("%s::j2xInit("), c)))
            _stale.append(c);
    }
    if (py::truthy(_stale)) {
        _prog(py::percent(S("[twophase] WARNING: %s has no j2xInit (an overlay replaced it?) -- it will not link"), py::join(S(", "), _stale)));
    }

    // the project's markers over the final code (Minecraft: keyboard keys the console does not have)
    _PROJECT->after_construction(out_dir, platform, _prog);

    // FORWARDER COALESCE: fold pure redirect headers (ByteBuffer.h -> NIOBuffers.h, Keyboard.h -> Input.h)
    // into their targets and delete them -- fewer files, no shims. Runs BEFORE the prune so the prune sees
    // the rewritten (direct) include graph.
    if (platform && py::truthy(*platform)) {
        _coalesce_forwarders(out_dir, _prog);
    }

    // DEAD-SUBSYSTEM PRUNE: full-product #include-reachability from the game corpus + entry. Any platform
    // bundle or stub the game never reaches (audio, networking, unused java.* libs, dead desktop types) is
    // cut whole -- no curated list. Runs LAST, after every file exists (emission + references + overlays),
    // so it sees the final include graph. See _prune_unreferenced.
    if (platform && py::truthy(*platform)) {
        py::set<str> all_names;
        for (const ClassRef& c : classes) all_names.add(c->name);
        _prune_unreferenced(out_dir, all_names, _prog, _applied);
    }

    // the PC build: one CMake executable named after the program's main class
    if (is_platform("pc_opengl")) {
        std::optional<str> mc = console::platform_refs::detect_main_class(classes);
        str _exe = (mc && py::truthy(*mc)) ? *mc : S("game");
        _prog(py::percent(S("[cmake] %s (executable %s)"), cmake::write_cmakelists(out_dir, _exe), _exe));
    }

    // verify: run the crash-class rules over the emitted output (acceptance check)
    _prog(S("[rules] scanning crash-class rules on emitted output..."));
    int64_t rule_findings = rules::scan_rules(*ledger, out_dir, platform);
    _prog(py::percent(S("[rules] %d crash-class findings"), rule_findings));

    std::optional<str> rep;
    if (write_report) {
        _prog(S("[report] writing conversion report..."));
        py::dict<str, str> meta;
        meta.set(S("output_dir"), py::os::path::abspath(out_dir));
        meta.set(S("profile"), S("rewrite (CodeBreaker)"));
        rep = report::write_reports(*ledger, out_dir, std::nullopt, meta);
        _prog(py::percent(S("[report] written to %s"), *rep));
    }

    // the single debug file: one master switch (DEBUG_ENABLED) gating every DBG_* macro, so all
    // debug instrumentation across the codebase is toggled from one place. Concise + rate-limited.
    // Opt-in: the UI/CLI decides whether the port gets debug support at all.
    if (emit_debug) {
        debug::write_debug(out_dir);
        _prog(S("[debug] emitted Debug.h/.cpp + DEBUG.md (master toggle DEBUG_ENABLED; J2X-DEBUG markers)"));
    } else {
        _prog(S("[debug] debug support NOT emitted (opt-in disabled)"));
    }

    // the project's own reports (Minecraft: asset manifest, achievements, platform-layer coverage)
    _PROJECT->reports(java_dir, out_dir, py::list<str>::from(rt_files.keys()), _prog);

    // API-mapping coverage: which external Java/library symbols the source references, resolved
    // against the API-mapping registry for the target language+dialect. APIMAP.md is the enumerable
    // gap list -- what the engine can already translate vs. what still needs a mapping row/shim.
    try {
        str lbl = std_version.label.lower();
        std::optional<str> _dialect = py::truthy(lbl) ? std::optional<str>(lbl) : std::nullopt;
        py::set<str> _syms = apimap::extract_symbols_from_dir(java_dir);
        str _md = apimap::render_coverage_md(_syms, S("java"), S("cpp"), _dialect);
        _write(py::os::path::join(out_dir, S("APIMAP.md")), _md);
        apimap::Coverage _amcov = apimap::load()->coverage(_syms, S("java"), S("cpp"), _dialect);
        _prog(py::percent(S("[apimap] %d symbols: %d mapped, %d unsupported, %d unknown -> APIMAP.md"), _syms.size(), _amcov.mapped.size(),
                          _amcov.unsupported.size(), _amcov.unmapped.size()));
    } catch (const py::Exception& _e) {
        _prog(py::percent(S("[apimap] skipped: %s"), e_what_(_e)));
    }

    // Regenerate the bug-fix ledger from the live FIXED BUG / J2X-BUGFIX markers in the
    // converter's authored sources (source of truth is the CONVERTER repo, not the tree).
    // Keeps BUGFIX_LEDGER.md perfectly in sync so no game-breaking fix is ever forgotten.
    try {
        auto _bf = bugfix_ledger::run();
        py::set<str> slugs;
        for (const auto& e : _bf) slugs.add(e.slug);
        _prog(py::percent(S("[bugfix-ledger] %d markers / %d distinct fixes -> BUGFIX_LEDGER.md"), _bf.size(), slugs.size()));
    } catch (const py::Exception& _e) {
        _prog(py::percent(S("[bugfix-ledger] skipped: %s"), e_what_(_e)));
    }

    // (the nullable-getter globals and the target profile are restored by convert()'s `finally`)

    emit::source::TREE_BODIES = nullptr;
    {
        // ''.join(', %s %d' % kv for kv in tree.declined.most_common(8))
        std::vector<std::tuple<str, int64_t>> top = tree->declined.items();
        std::stable_sort(top.begin(), top.end(), [](const auto& a, const auto& b) { return std::get<1>(a) > std::get<1>(b); });
        if (top.size() > 8) top.resize(8);
        str common;
        int64_t declined_total = 0;
        for (const auto& kv : top) common += py::percent(S(", %s %d"), std::get<0>(kv), std::get<1>(kv));
        for (int64_t v : tree->declined.values()) declined_total += v;
        _prog(py::percent(S("[translate] %d method bodies from the IR; %d untranslated (J2X-GAP%s)"), tree->written, declined_total, common));
    }

    py::dict<str, int64_t> cats = ledger->totals_by_category();
    py::dict<str, int64_t> sevs = ledger->totals_by_severity();
    _prog(py::percent(S("[done] %d events: %d PORTED, %d GUESSED, %d STUBBED, %d RISK, %d HIGH-sev"), ledger->size(), cats.get(Category::PORTED, 0),
                      cats.get(Category::GUESSED, 0), cats.get(Category::STUBBED, 0), cats.get(Category::RISK, 0),
                      sevs.get(Severity::HIGH, 0)));
    Summary s;
    s.parsed = classes.size();
    s.headers = headers;
    s.sources = sources;
    s.markers = py::list<str>::from(py::sorted(markers));   // empty decompiler marker classes, dropped on purpose
    s.tree_bodies = std::make_tuple(tree->written, tree->declined.copy());
    s.rule_findings = rule_findings;
    s.events = ledger->size();
    s.high = sevs.get(Severity::HIGH, 0);
    s.report = rep;
    s.ledger = ledger;
    s.cpp_std = std_version.label;
    s.platform = (platform && !platform->empty()) ? *platform : S("none");   // (Python: platform or 'none')
    s.profile = (_ed_name && py::truthy(*_ed_name)) ? *_ed_name : _prev_profile;   // the target profile active DURING this conversion
    return s;
}

}  // namespace

}  // namespace codebreaker::java::transpile
