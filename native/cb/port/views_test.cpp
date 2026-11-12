// Tests of the structure passes of views.hpp that work on a program the port has already read: pass_through_params,
// offset_views, bss_ends, copied_members, ptr2_names. The state is the Python original's at the moment each was first
// called inside Port.modernize (testdata/views/<case>/state.json); the expected answers are the Python's.
//
// The constructor's own structure passes (anon_tags ... array_tails) are checked with the constructor, in
// program_prep_test.cpp. Some checks are written by hand here: the cases the passes name in their comments.
#include <set>

#include "cb/core/testing.hpp"
#include "cb/port/views.hpp"
#include "cb/port/views_case.hpp"

using namespace cb;
using namespace cb::port::pre_ansi;
using namespace cb::port::pre_ansi::vcase;

namespace {

// The Port as it was at one point of modernize(): the pieces the passes of views.hpp work on.
struct Restored {
    Files files, originals;
    Notes notes, mended;
    Aliases aliases;
    std::set<std::string> view_vars, view_headers;
    std::vector<std::string> view_wide, view_puns_refused;
    std::map<std::string, std::set<std::string>> need_headers;
    OrderedMap<std::string, Fn> fns;
    ParamTypes param_types;
    BssSizes bss;
    ViewCaches caches;
    ParamTypes types_at_start;

    ViewContext context() {
        return ViewContext{files, originals, notes, mended, aliases, view_vars, view_headers, view_wide,
                           view_puns_refused, need_headers, fns, param_types, bss, caches};
    }
};

bool truthy(const J& j) {
    if (j.is_boolean()) return j.get<bool>();
    if (j.is_number()) return j.get<double>() != 0;
    return !j.is_null() && !j.empty();
}

Fn fn_of(const J& j) {
    Fn f;
    f.name = j["name"].get<std::string>();
    f.ret = j["ret"].get<std::string>();
    for (const J& p : j["params"]) f.params.push_back({p[0].get<std::string>(), p[1].get<std::string>()});
    f.is_static = j["static"].get<bool>();
    f.line = j["line"].get<std::size_t>();
    f.returns_void = j["void"].get<bool>();
    f.loose = j["loose"].get<bool>();
    f.bare = j["bare"].get<std::size_t>();
    f.varargs = truthy(j["varargs"]);   // (the Python keeps the names of the variable arguments there)
    f.va_slots = truthy(j["va_slots"]);
    f.ret_set = truthy(j["ret_set"]);
    f.int_ret = truthy(j["int_ret"]);
    f.keep_int = truthy(j["keep_int"]);
    for (const J& x : j["implicit"]) f.implicit.insert(x.get<std::string>());
    if (!j["file"].is_null()) f.file = j["file"].get<std::string>();
    f.body = j["body"].get<std::string>();
    return f;
}

std::unique_ptr<Restored> restore(const Case& c, const char* moment) {
    auto r = std::make_unique<Restored>();
    const J state = json::load(c.dir / "state.json")[moment];
    r->originals = c.originals;
    for (const J& n : state["kept"]) {
        const std::string name = n.get<std::string>();
        r->files.insert_or_assign(name, state["files_changed"].contains(name) ? state["files_changed"][name].get<std::string>() : c.originals.at(name));
    }
    for (const J& f : state["fns"]) r->fns.insert_or_assign(f["name"].get<std::string>(), fn_of(f));
    for (const J& t : state["param_types"]) r->param_types[{t[0].get<std::string>(), t[1].get<std::size_t>()}] = t[2].get<std::string>();
    r->types_at_start = r->param_types;
    for (const J& b : state["bss"]) r->bss.insert_or_assign(b[0].get<std::string>(), b[1].get<std::uint64_t>());
    for (const J& a : state["aliases"]) r->aliases.insert_or_assign(a[0].get<std::string>(), Alias{a[1].get<std::string>(), a[2].get<std::string>()});
    for (const J& v : state["view_vars"]) r->view_vars.insert(v.get<std::string>());
    for (const J& v : state["view_headers"]) r->view_headers.insert(v.get<std::string>());
    return r;
}

J mapping_json(const std::map<std::string, ViewMember>& mapping) {
    // (the Python's dict order is the order of the other structure's layout; compared as sorted lists)
    std::vector<J> items;
    for (const auto& [k, v] : mapping)
        items.push_back(J::array({k, v.pun ? J::array({"pun", v.base, v.offset, v.type}) : J::array({"names", v.names})}));
    std::sort(items.begin(), items.end(), [](const J& a, const J& b) { return a.dump() < b.dump(); });
    return J(items);
}

J views_json(const ViewTable& views) {
    std::vector<J> out;
    for (const auto& [x, v] : views) out.push_back(J::array({x, v.other, v.file, mapping_json(v.mapping)}));
    return J(out);
}

J sorted_json(J list) {
    std::vector<J> items(list.begin(), list.end());
    std::sort(items.begin(), items.end(), [](const J& a, const J& b) { return a.dump() < b.dump(); });
    return J(items);
}

void check_case(const std::string& name) {
    const Case c = load(name);
    const J& want = c.expected["modernize"];
    // pass_through_params, from the state it was first called in
    {
        auto r = restore(c, "pass_through");
        ViewContext cx = r->context();
        pass_through_params(cx);
        J added = J::array();
        for (const auto& [key, type] : r->param_types) {
            const auto before = r->types_at_start.find(key);
            if (before == r->types_at_start.end() || before->second != type) added.push_back(J::array({key.first, key.second, type}));
        }
        CHECK_J(sorted_json(added), sorted_json(want["param_types_new"]));
        CHECK_J(notes_json(r->mended), want["mended_new"]);
        pass_through_params(cx);   // (once: a second call changes nothing)
        CB_CHECK_EQ(r->mended.size(), want["mended_new"].size());
        CHECK_J(J(std::vector<std::string>(copied_members(cx).begin(), copied_members(cx).end())), want["copied"]);
        CHECK_J(J(std::vector<std::string>(ptr2_names(cx).begin(), ptr2_names(cx).end())), want["ptr2"]);
    }
    // offset_views and bss_ends, from the state offset_views was first called in
    auto r = restore(c, "offset_views");
    ViewContext cx = r->context();
    if (!c.real) {   // (the calls the original made inside modernize, in their order, on the texts they were given)
        for (const J& call : want["calls"]) {
            const std::string kind = call[0].get<std::string>(), file = call[1].get<std::string>();
            const std::string input = fs::read_text(c.dir / "in" / kind / file);
            CHECK_J(crc(input), call[2]);
            const std::string out = kind == "offset_views" ? offset_views(cx, input, file) : bss_ends(cx, input, file);
            CHECK_J(crc(out), call[3]);
            CB_CHECK_EQ(out, fs::read_text(c.dir / "out" / kind / file));
        }
        J needed = J::object();
        for (const J& e : want["need_headers"]) needed[e[0].get<std::string>()] = e[1];
        CHECK_J(J(r->need_headers), needed);
        CHECK_J(J(r->view_wide), want["view_wide"]);
        CHECK_J(J(r->view_puns_refused), want["puns_refused"]);
    } else {
        offset_views(cx, "", "");   // (the first call works the views out)
    }
    CB_CHECK(r->caches.views.has_value());
    // the views as the original has them (the Python's dict order is the order they were found in)
    J got_views = J::array();
    for (const auto& [x, v] : *r->caches.views) {
        J entry = J::array({x, v.other, v.file, J::array()});
        std::vector<J> items;
        for (const auto& [k, m] : v.mapping) items.push_back(J::array({k, m.pun ? J::array({"pun", m.base, m.offset, m.type}) : J::array({"names", m.names})}));
        entry[3] = sorted_json(J(items));
        got_views.push_back(entry);
    }
    J want_views = J::array();
    for (const J& v : want["views"]) {
        J entry = v;
        entry[3] = sorted_json(v[3]);
        want_views.push_back(entry);
    }
    CHECK_J(got_views, want_views);
    CHECK_J(J(r->caches.weak), want["views_weak"]);
    CHECK_J(J(r->caches.fresh), want["views_new"]);
    // every file, as the port has it now, through both passes
    std::size_t checked = 0;
    for (const auto& [n, expect] : want["post"].items()) {
        const std::string t = r->files.at(n);
        CHECK_J(crc(offset_views(cx, t, n)), expect[0]);
        CHECK_J(crc(bss_ends(cx, t, n)), expect[1]);
        ++checked;
    }
    CB_CHECK_EQ(checked, r->files.size());
}

}  // namespace

CB_TEST("port.views.the_synthetic_programs_as_the_original_made_them", [] {
    for (const std::string& name : synthetic_names()) check_case(name);
});

CB_TEST("port.views.the_ms_c_3_00_trees_as_the_original_made_them", [] {
    for (const std::string& name : real_names()) check_case(name);
});

CB_TEST("port.views.a_file_reads_another_structure_through_its_own_by_the_old_places", [] {
    // struct lab (x.c's own) is struct targ (t.h) by its old places: p->a is the other's kind, p->c its flags
    Restored r;
    r.files.insert_or_assign("t.h", "struct targ {\n\tint kind;\n\tchar *name;\n\tint flags;\n};\nstruct sym {\n\tstruct targ *type;\n\tint cls;\n};\n");
    r.files.insert_or_assign("x.c", "#include \"t.h\"\nstruct lab {\n\tint a;\n\tchar *b;\n\tint c;\n};\nf(s)\nstruct sym *s;\n{\n\tstruct lab *p;\n"
                                    "\tp = (struct lab *)s->type;\n\treturn p->a + p->c;\n}\n");
    r.originals = r.files;
    ViewContext cx = r.context();
    CB_CHECK_EQ(offset_views(cx, r.files.at("x.c"), "x.c"),
                "#include \"t.h\"\nstruct lab {\n\tint a;\n\tchar *b;\n\tint c;\n};\nf(s)\nstruct sym *s;\n{\n\tstruct lab *p;\n"
                "\tp = (struct lab *)s->type;\n\treturn ((struct targ *)p)->kind + ((struct targ *)p)->flags;\n}\n");
    CB_CHECK(r.need_headers.at("x.c").contains("t.h"));
    // another file has no view: it is left as it is
    CB_CHECK_EQ(offset_views(cx, "int g(void) { return 0; }\n", "y.c"), "int g(void) { return 0; }\n");
});

CB_TEST("port.views.the_place_after_a_char_array_is_the_arrays_end", [] {
    Restored r;
    r.files.insert_or_assign("a.c", "#include \"port.h\"\nchar g_1000[10];\nint g_100a;\nint f() { char *p; p = (char *)&g_100a; return 0; }\n");
    r.originals.insert_or_assign("a.c", "char g_1000[10];\nint g_100a;\n");
    r.bss.insert_or_assign("g_1000", 10);
    r.bss.insert_or_assign("g_100a", 10);
    ViewContext cx = r.context();
    const std::string out = bss_ends(cx, r.files.at("a.c"), "a.c");
    CB_CHECK_EQ(out, "#include \"port.h\"\nchar g_1000[10];\nint g_100a;\nint f() { char *p; p = (g_1000 + 10); return 0; }\n");
    CB_CHECK_EQ(r.notes.size(), 1u);
    // the array itself, a [ index, ->: not its end
    CB_CHECK_EQ(bss_ends(cx, "p = (char *)&g_100a[1];\nq = (char *)&g_1000;\n", "b.c"), "p = (char *)&g_100a[1];\nq = (char *)&g_1000;\n");
    // no BSS list: nothing to do
    Restored none;
    ViewContext quiet = none.context();
    CB_CHECK_EQ(bss_ends(quiet, "p = (char *)&g_100a;\n", "c.c"), "p = (char *)&g_100a;\n");
});

CB_TEST("port.views.a_name_is_declared_where_an_arrays_end_is_taken", [] {
    // the array is not declared in the text: its extern goes after the port.h include
    Restored r;
    r.originals.insert_or_assign("a.c", "char g_1000[10];\nint g_100a;\n");
    r.bss.insert_or_assign("g_1000", 10);
    r.bss.insert_or_assign("g_100a", 10);
    ViewContext cx = r.context();
    CB_CHECK_EQ(bss_ends(cx, "#include \"port.h\"\nint f() { return (int)(char *)&g_100a; }\n", "b.c"),
                "#include \"port.h\"\nextern char g_1000[];\nint f() { return (int)(g_1000 + 10); }\n");
});
