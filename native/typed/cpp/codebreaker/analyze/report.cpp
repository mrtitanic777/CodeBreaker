// codebreaker/analyze/report.cpp -- C++ from codebreaker.analyze.report (see report.hpp).

#include "codebreaker/analyze/report.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <type_traits>
#include <variant>

#include "codebreaker/analyze/call_graph.hpp"
#include "codebreaker/analyze/classes.hpp"
#include "codebreaker/analyze/conventions.hpp"
#include "codebreaker/analyze/dispatch.hpp"
#include "codebreaker/analyze/facts.hpp"
#include "codebreaker/analyze/globals.hpp"
#include "codebreaker/analyze/resources.hpp"
#include "codebreaker/analyze/strings.hpp"
#include "codebreaker/analyze/structures.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/store/functions.hpp"
#include "codebreaker/ui/service.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::analyze::report {

namespace {

// ---- JSON values
Json j_none() { return Json(); }
Json j_bool(bool v) {
    Json j;
    j.kind = Json::Bool;
    j.b = v;
    return j;
}
Json j_int(int64_t v) {
    Json j;
    j.kind = Json::Int;
    j.i = v;
    return j;
}
Json j_float(double v) {
    Json j;
    j.kind = Json::Float;
    j.d = v;
    return j;
}
Json j_str(const py::str& v) {
    Json j;
    j.kind = Json::Str;
    j.s = v;
    return j;
}

// has a record's field list: for_each_field(record, visitor)
struct Noop {
    template <class V> void operator()(const char*, const V&) const {}
};
template <class T, class = void> struct has_fields : std::false_type {};
template <class T> struct has_fields<T, std::void_t<decltype(for_each_field(std::declval<const T&>(), Noop()))>> : std::true_type {};

template <class T> struct is_py_list : std::false_type {};
template <class U> struct is_py_list<py::list<U>> : std::true_type {};
template <class T> struct is_py_set : std::false_type {};
template <class U> struct is_py_set<py::set<U>> : std::true_type {};
template <class T> struct is_py_dict : std::false_type {};
template <class K, class V> struct is_py_dict<py::dict<K, V>> : std::true_type {};
template <class T> struct is_optional : std::false_type {};
template <class U> struct is_optional<std::optional<U>> : std::true_type {};
template <class T> struct is_variant : std::false_type {};
template <class... U> struct is_variant<std::variant<U...>> : std::true_type {};
template <class T> struct is_tuple : std::false_type {};
template <class... U> struct is_tuple<std::tuple<U...>> : std::true_type {};

// Python's repr of a scalar Json (what sorting a set by repr compares)
py::str scalar_repr(const Json& j) {
    switch (j.kind) {
        case Json::Str: return py::repr(j.s);
        case Json::Int: return py::str(std::to_string(j.i));
        case Json::Bool: return py::str(j.b ? "True" : "False");
        case Json::Float: return py::repr(j.d);
        default: return py::str("None");
    }
}

// a record, a list, a dict ... as the Python holds it in a fact: JSON (a set as the list of its elements sorted by repr, as the fact store
// writes it; a tuple as a list)
template <class T> Json to_json(const T& v) {
    if constexpr (std::is_same_v<T, bool>) {
        return j_bool(v);
    } else if constexpr (std::is_integral_v<T>) {
        return j_int(static_cast<int64_t>(v));
    } else if constexpr (std::is_floating_point_v<T>) {
        return j_float(static_cast<double>(v));
    } else if constexpr (std::is_same_v<T, py::str>) {
        return j_str(v);
    } else if constexpr (is_optional<T>::value) {
        return v ? to_json(*v) : j_none();
    } else if constexpr (is_variant<T>::value) {
        return std::visit([](const auto& x) { return to_json(x); }, v);
    } else if constexpr (is_py_list<T>::value) {
        Json j;
        j.kind = Json::List;
        for (const auto& x : v) j.list.push_back(to_json(x));
        return j;
    } else if constexpr (is_py_set<T>::value) {
        Json j;
        j.kind = Json::List;
        std::vector<std::pair<py::str, Json>> items;
        for (const auto& x : v) {
            Json e = to_json(x);
            items.emplace_back(scalar_repr(e), e);
        }
        std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
        for (auto& it : items) j.list.push_back(it.second);
        return j;
    } else if constexpr (is_py_dict<T>::value) {
        Json j;
        j.kind = Json::Dict;
        for (const auto& kv : v.items()) {
            if constexpr (std::is_same_v<std::decay_t<decltype(std::get<0>(kv))>, py::str>) {
                j.dict.set(std::get<0>(kv), to_json(std::get<1>(kv)));
            } else {
                j.dict.set(py::str(std::to_string(std::get<0>(kv))), to_json(std::get<1>(kv)));
            }
        }
        return j;
    } else if constexpr (is_tuple<T>::value) {
        Json j;
        j.kind = Json::List;
        std::apply([&](const auto&... x) { (j.list.push_back(to_json(x)), ...); }, v);
        j.tuple = true;
        return j;
    } else if constexpr (has_fields<T>::value) {
        Json j;
        j.kind = Json::Dict;
        for_each_field(v, [&](const char* k, const auto& x) { j.dict.set(py::str(k), to_json(x)); });
        return j;
    } else {
        static_assert(sizeof(T) == 0, "no JSON form for this type");
    }
}

// a typed fact record as a Fact: addr and kind, then the other fields in order
template <class R> Fact fact_of(const R& rec) {
    Fact f;
    for_each_field(rec, [&](const char* k, const auto& v) {
        std::string key = k;
        using V = std::decay_t<decltype(v)>;
        if (key == "addr") {
            if constexpr (is_optional<V>::value) f.addr = v;
            else f.addr = static_cast<int64_t>(v);
        } else if (key == "kind") {
            f.kind = v;
        } else {
            f.fields.append(std::make_tuple(py::str(k), to_json(v)));
        }
    });
    return f;
}

template <class A> void add_facts(Result& out, const A& facts) {
    for (const auto& f : facts) std::visit([&](const auto& rec) { out.facts.append(fact_of(rec)); }, f);
}

template <class C> void add_counts(Result& out, const C& counts) {
    for_each_field(counts, [&](const char* k, const auto& v) { out.counts.append(std::make_tuple(py::str(k), to_json(v))); });
}

void add_dict_counts(Result& out, const py::dict<py::str, int64_t>& counts) {
    for (const auto& kv : counts.items()) out.counts.append(std::make_tuple(std::get<0>(kv), j_int(std::get<1>(kv))));
}

// ---- the analyses, each read into a Result
Result call_graph_result(image::Image& img) {
    call_graph::Analysis a = call_graph::analyse(img);
    Result out;
    const call_graph::Counts& c = a.counts;
    const std::pair<const char*, int64_t> counts[] = {
        {"functions", c.functions}, {"functions_found_from_calls", c.functions_found_from_calls}, {"edges", c.edges}, {"call_sites", c.call_sites},
        {"import_call_sites", c.import_call_sites}, {"tail_jumps", c.tail_jumps}, {"indirect_call_sites", c.indirect_call_sites},
        {"imports_used", c.imports_used}, {"leaf_functions", c.leaf_functions}, {"recursive_groups", c.recursive_groups},
        {"address_taken_functions", c.address_taken_functions}, {"data_references", c.data_references}, {"roots", c.roots},
        {"reached_from_roots", c.reached_from_roots}, {"reached_with_data_tables", c.reached_with_data_tables},
        {"unreferenced_functions", c.unreferenced_functions}, {"code_bytes_decoded", c.code_bytes_decoded},
        {"code_section_bytes_not_reached", c.code_section_bytes_not_reached}};
    for (const auto& kv : counts) out.counts.append(std::make_tuple(py::str(kv.first), j_int(kv.second)));
    for (const call_graph::Fact& f : a.facts) {
        Fact fact;
        fact.addr = f.addr;
        fact.kind = f.kind;
        fact.fields.append(std::make_tuple(py::str("callees"), to_json(f.callees)));
        fact.fields.append(std::make_tuple(py::str("imports"), to_json(f.imports)));
        fact.fields.append(std::make_tuple(py::str("indirect"), to_json(f.indirect)));
        fact.fields.append(std::make_tuple(py::str("callers"), to_json(f.callers)));
        fact.fields.append(std::make_tuple(py::str("address_taken"), to_json(f.address_taken)));
        fact.fields.append(std::make_tuple(py::str("reached"), to_json(f.reached)));
        out.facts.append(fact);
    }
    out.text = a.text;
    return out;
}

Result conventions_result(image::Image& img) {
    conventions::Analysis a = conventions::analyse(img);
    Result out;
    add_dict_counts(out, a.counts);
    out.text = a.text;      // (the facts are not converted: see the header)
    return out;
}

Result globals_result(image::Image& img) {
    globals::Analysis a = globals::analyse(img);
    Result out;
    add_counts(out, a.counts);
    add_facts(out, a.facts);
    out.text = a.text;
    return out;
}

Result strings_result(image::Image& img) {
    strings::Analysis a = strings::analyse(img);
    Result out;
    add_counts(out, a.counts);
    add_facts(out, a.facts);
    out.text = a.text;
    return out;
}

Result structures_result(image::Image& img) {
    structures::Analysis a = structures::analyse(img);
    Result out;
    add_dict_counts(out, a.counts);
    out.text = a.text;
    return out;
}

Result classes_result(image::Image& img) {
    classes::Analysis a = classes::analyse(img);
    Result out;
    add_dict_counts(out, a.counts);
    out.text = a.text;
    return out;
}

Result dispatch_result(image::Image& img) {
    dispatch::Analysis a = dispatch::analyse(img);
    Result out;
    add_dict_counts(out, a.counts);
    out.text = a.text;
    return out;
}

Result resources_result(image::Image& img) {
    resources::Analysis a = resources::analyse(img);
    Result out;
    add_counts(out, a.counts);
    add_facts(out, a.facts);
    out.text = a.text;
    return out;
}

double rounded(double x) { return std::round(x * 100.0) / 100.0; }

py::str str_of(const Json& v) {
    switch (v.kind) {
        case Json::Str: return v.s;
        case Json::Int: return py::str(std::to_string(v.i));
        case Json::Bool: return py::str(v.b ? "True" : "False");
        case Json::Float: return py::repr(v.d);
        case Json::None: return py::str("None");
        default: return _value(v);
    }
}

double now() { return py::time::time(); }

// the module's text (what `analyze` with no module, or -h, prints)
const py::str& DOC() {
    static const py::str d(
    "report.py -- run the analyses on a module and write what they found.\n"
    "\n"
    "    python -m codebreaker analyze <module> [--what a,b,c] [--out FILE] [--run N|all] [--facts]\n"
    "                                           [--starts FILE] [--json] [--no-store] [--summary]\n"
    "\n"
    "    <module>     any module the corpus names, or a path: a 32-bit module, a 16-bit module, or a\n"
    "                 driver (\"vmm32.vxd#vmm\" names one driver inside a bundle)\n"
    "    --what       which analyses, of: call_graph, conventions, globals, strings, structures,\n"
    "                 classes, dispatch, resources, observe      (default: all but observe)\n"
    "    --run N      also run N functions in the emulator (observe); \"all\" for every function.\n"
    "                 32-bit modules only; a few hundred functions take minutes.\n"
    "    --out FILE   where the report goes (default: state/analyze/<module>.txt)\n"
    "    --facts      list every fact in the report, not only the counts and highlights\n"
    "    --starts FILE  write the function starts found here that the decompiler's description lacks,\n"
    "                 as lines of that description (`F <address> <name> <bytes removed> [model]`)\n"
    "    --json       print the counts and timings as one JSON object\n"
    "    --summary    print one line per analysis instead of the report's path\n"
    "    --no-store   do not write the facts to the function store\n"
    "\n"
    "The facts go to the function store's `fact` table (analyze/facts.py), replacing what the same\n"
    "analysis stored for the module before.\n"
    "\n"
    "    from codebreaker.analyze import report\n"
    "    r = report.run(\"comctl32.dll\")          # {\"counts\": {analysis: {...}}, \"seconds\": {...}, ...}\n"
    "");
    return d;
}

}  // namespace

const py::list<Analysis>& ANALYSES() {
    static const py::list<Analysis> a = [] {
        py::list<Analysis> out;
        auto add = [&](const char* name, std::function<Result(image::Image&, const std::optional<int64_t>&)> fn) { out.append(Analysis{py::str(name), fn}); };
        add("call_graph", [](image::Image& i, const std::optional<int64_t>&) { return call_graph_result(i); });
        add("conventions", [](image::Image& i, const std::optional<int64_t>&) { return conventions_result(i); });
        add("globals", [](image::Image& i, const std::optional<int64_t>&) { return globals_result(i); });
        add("strings", [](image::Image& i, const std::optional<int64_t>&) { return strings_result(i); });
        add("structures", [](image::Image& i, const std::optional<int64_t>&) { return structures_result(i); });
        add("classes", [](image::Image& i, const std::optional<int64_t>&) { return classes_result(i); });
        add("dispatch", [](image::Image& i, const std::optional<int64_t>&) { return dispatch_result(i); });
        add("resources", [](image::Image& i, const std::optional<int64_t>&) { return resources_result(i); });
        add("observe", [](image::Image& i, const std::optional<int64_t>& limit) {
            if (!observe_hook()) throw py::ImportError("No module named 'unicorn'");   // (the emulator is not linked here)
            return observe_hook()(i, limit);
        });
        return out;
    }();
    return a;
}

const py::list<py::str>& DEFAULT() {
    static const py::list<py::str> d = [] {
        py::list<py::str> out;
        for (const Analysis& a : ANALYSES())
            if (a.name != py::str("observe")) out.append(a.name);
        return out;
    }();
    return d;
}

std::function<Result(image::Image&, const std::optional<int64_t>&)>& observe_hook() {
    static std::function<Result(image::Image&, const std::optional<int64_t>&)> h;
    return h;
}

double Run::second(const py::str& name) const {
    for (const auto& s : seconds)
        if (std::get<0>(s) == name) return std::get<1>(s);
    throw py::KeyError(py::repr(name).utf8());
}

const py::list<std::tuple<py::str, Json>>* Run::count_of(const py::str& analysis) const {
    for (const auto& c : counts)
        if (std::get<0>(c) == analysis) return &std::get<1>(c);
    return nullptr;
}

const Result* Run::result_of(const py::str& analysis) const {
    for (const auto& r : results)
        if (std::get<0>(r) == analysis) return &std::get<1>(r);
    return nullptr;
}

Run run(const py::str& module, const std::optional<py::list<py::str>>& what_in, bool store, const std::optional<int64_t>& observed) {
    py::list<py::str> what = what_in ? *what_in : DEFAULT();
    py::list<py::str> unknown;
    py::list<py::str> known;
    for (const Analysis& a : ANALYSES()) known.append(a.name);
    for (const py::str& w : what)
        if (!known.contains(w)) unknown.append(w);
    if (py::truthy(unknown)) {
        throw py::ValueError(py::percent(py::str("no analysis called %s (there are: %s)"), py::join(py::str(", "), unknown), py::join(py::str(", "), known)));
    }
    double t0 = now();
    image::ImageP img = image::load(module);
    Run out;
    out.module = img->name;
    out.path = img->path;
    out.kind = img->kind;
    out.bits = img->bits;
    out.functions = py::len(img->functions);
    out.image = img;
    out.seconds.append(std::make_tuple(py::str("load"), rounded(now() - t0)));
    for (const Analysis& a : ANALYSES()) {
        if (!what.contains(a.name)) continue;
        double t = now();
        Result res;
        try {
            res = a.run(*img, observed);
        } catch (const py::ImportError&) {
            throw;
        } catch (const py::Exception& e) {   // one analysis failing on a module is a finding, not the end
            res = Result();
            res.counts.append(std::make_tuple(py::str("failed"), j_str(py::percent(py::str("%s: %s"), py::str(e.type()), py::str(e.what())))));
        }
        out.seconds.append(std::make_tuple(a.name, rounded(now() - t)));
        out.counts.append(std::make_tuple(a.name, res.counts));
        out.results.append(std::make_tuple(a.name, res));
    }
    out.seconds.append(std::make_tuple(py::str("total"), rounded(now() - t0)));
    if (store) {
        std::optional<py::str> sha = base::corpus::sha(img->path);
        py::str key = sha ? *sha : py::str("None");
        if (img->name != py::os::path::basename(img->path).lower()) {   // one driver of a bundle
            key = py::percent(py::str("%s#%s"), key, std::get<2>(img->name.partition(py::str("#"))));
        }
        auto fs = store::functions::open();
        fs->add_module(key, img->name, img->path, img->kind);
        fs->close();
        auto table = analyze::facts::open();
        for (const auto& r : out.results) {
            py::list<analyze::facts::Row> rows;
            for (const Fact& f : std::get<1>(r).facts) {
                Json value;
                value.kind = Json::Dict;
                for (const auto& kv : f.fields) value.dict.set(std::get<0>(kv), std::get<1>(kv));
                rows.append(std::make_tuple(analyze::facts::key(*img, f.addr), f.kind, value));
            }
            table->put(key, std::get<0>(r), rows);
        }
        table->close();
        out.sha = key;
    }
    return out;
}

py::list<py::str> starts(image::Image& img) {
    auto facts_c = conventions::functions(img);
    py::list<py::str> lines;
    py::list<int64_t> order;
    for (int64_t va : img.discovered) order.append(va);
    for (int64_t va : py::sorted(order)) {
        const conventions::Fact& fact = (*facts_c)[va];
        std::optional<int64_t> pop = fact.removes;
        py::str line = py::percent(py::str("F\t%x\t%s\t%d"), va, img.names[va], pop ? *pop : int64_t(-1));
        if (img.kind == py::str("ne")) {
            py::str conv = fact.convention ? *fact.convention : py::str("");
            line = line + py::percent(py::str("\t__%s16%s"), (pop && *pop == 0) ? py::str("cdecl") : py::str("stdcall"),
                                      conv.endswith(py::str("far")) ? py::str("far") : py::str("near"));
        }
        lines.append(line);
    }
    return lines;
}

py::list<py::str> _feedback(const Run& result) {
    const auto* c = result.count_of(py::str("conventions"));
    auto count = [&](const char* name) -> std::optional<int64_t> {
        if (!c) return std::nullopt;
        for (const auto& kv : *c)
            if (std::get<0>(kv) == py::str(name)) return std::get<1>(kv).kind == Json::Int ? std::optional<int64_t>(std::get<1>(kv).i) : std::optional<int64_t>(0);
        return std::nullopt;
    };
    std::vector<std::pair<py::str, int64_t>> rows;
    rows.emplace_back(py::str("function starts the description does not have"), py::len(result.image->discovered));
    if (count("caller_removes_arguments")) {
        rows.emplace_back(py::str("caller-cleans functions whose argument count is now known"),
                          count("argument_count_from_callers").value_or(0) + count("argument_count_from_frame").value_or(0));
        rows.emplace_back(py::str("functions taking arguments in registers"), count("register_arguments").value_or(0));
        rows.emplace_back(py::str("functions returning nothing"), count("result_none").value_or(0));
        rows.emplace_back(py::str("functions with a variable argument list"), count("variadic").value_or(0));
    }
    py::list<py::str> out;
    out.append(py::str("what the decompiler's description could be told:"));
    for (const auto& r : rows) out.append(py::percent(py::str("  %-58s %d"), r.first, r.second));
    out.append(py::str(""));
    return out;
}

py::str _value(const Json& v) {
    switch (v.kind) {
        case Json::None: return py::str("None");
        case Json::Bool: return py::str(v.b ? "True" : "False");
        case Json::Int: return v.i > 0xFFFF ? py::percent(py::str("%#x"), v.i) : py::str(std::to_string(v.i));
        case Json::Dict: {
            py::list<py::str> parts;
            int64_t n = 0;
            for (const auto& kv : v.dict.items()) {
                if (n++ >= 40) break;
                parts.append(py::percent(py::str("%s: %s"), std::get<0>(kv), _value(std::get<1>(kv))));
            }
            return py::str("{") + py::join(py::str(", "), parts) + (py::len(v.dict) > 40 ? py::str(", ...") : py::str("")) + py::str("}");
        }
        case Json::List: {
            py::list<py::str> parts;
            for (size_t i = 0; i < v.list.size() && i < 40; ++i) parts.append(_value(v.list[i]));
            return py::str("[") + py::join(py::str(", "), parts) + (v.list.size() > 40 ? py::str(", ...") : py::str("")) + py::str("]");
        }
        case Json::Float: return py::repr(v.d);
        default: return v.s;
    }
}

py::str text(const Run& result, bool all_facts) {
    image::Image& img = *result.image;
    py::str kind_name;
    if (result.kind == py::str("pe")) kind_name = py::str("32-bit Windows module");
    else if (result.kind == py::str("ne")) kind_name = py::str("16-bit Windows module");
    else if (result.kind == py::str("le")) kind_name = py::str("virtual device driver");
    else throw py::KeyError(py::repr(result.kind).utf8());
    py::list<py::str> lines;
    lines.append(py::percent(py::str("%s  (%s, %d-bit)  %s"), result.module, kind_name, result.bits, result.path));
    lines.append(py::percent(py::str("%d functions, %d of them found from calls and held addresses beyond the decompiler's description"), result.functions,
                             py::len(img.discovered)));
    lines.append(py::percent(py::str("analysed in %.1f s (loading and decoding %.1f s)"), result.second(py::str("total")), result.second(py::str("load"))));
    lines.append(py::str(""));
    for (const py::str& l : _feedback(result)) lines.append(l);
    for (const Analysis& a : ANALYSES()) {
        const Result* res = result.result_of(a.name);
        if (!res) continue;
        lines.append(py::percent(py::str("== %s  (%.1f s)"), a.name.replace(py::str("_"), py::str(" ")), result.second(a.name)));
        int64_t width = 0;
        for (const auto& kv : res->counts) width = std::max(width, py::len(std::get<0>(kv)));
        for (const auto& kv : res->counts)
            lines.append(py::percent(py::str("  %-*s  %s"), width, std::get<0>(kv).replace(py::str("_"), py::str(" ")), str_of(std::get<1>(kv))));
        if (py::truthy(res->text)) {
            lines.append(py::str(""));
            for (const py::str& t : res->text) lines.append(py::str("  ") + t);
        }
        if (all_facts) {
            lines.append(py::str(""));
            for (const Fact& f : res->facts) {
                py::str where = f.addr ? img.label(*f.addr) : py::str("(module)");
                py::list<py::str> rest;
                for (const auto& kv : f.fields)
                    if (std::get<0>(kv) != py::str("name")) rest.append(py::percent(py::str("%s=%s"), std::get<0>(kv), _value(std::get<1>(kv))));
                lines.append(py::percent(py::str("  %-18s %-22s %s"), f.kind, where, py::join(py::str(", "), rest)));
            }
        }
        lines.append(py::str(""));
    }
    return py::join(py::str("\n"), lines);
}

namespace {

// settings.state(*parts)
py::str state_path(const py::str& a, const py::str& b) {
    py::str path = py::os::path::join(py::settings::get_str(py::str("state")), a, b);
    py::os::makedirs(py::os::path::dirname(path), 0777, true);
    return path;
}

void write_text(const py::str& path, const py::str& text) {
    py::file f = py::open(path, py::str("w"), py::str("utf-8"), std::nullopt, py::str("\n"));
    f.write(text);
    f.close();
}

}  // namespace

int64_t main(const py::list<py::str>& argv_in) {
    py::list<py::str> args = argv_in.copy();
    py::dict<py::str, std::optional<py::str>> opts;
    for (const char* k : {"what", "out", "run", "starts"}) opts.set(py::str(k), std::nullopt);
    py::set<py::str> flags;
    py::list<py::str> rest;
    while (py::truthy(args)) {
        py::str a = args.pop(0);
        if (a == py::str("--what") || a == py::str("--out") || a == py::str("--run") || a == py::str("--starts")) {
            if (!py::truthy(args)) {
                py::print(py::percent(py::str("analyze: %s needs a value"), a));
                return 2;
            }
            opts.set(a.slice(2, py::len(a)), args.pop(0));
        } else if (a == py::str("--facts") || a == py::str("--json") || a == py::str("--no-store") || a == py::str("--summary")) {
            flags.add(a);
        } else if (a == py::str("-h") || a == py::str("--help")) {
            py::print(DOC());
            return 0;
        } else {
            rest.append(a);
        }
    }
    if (py::len(rest) != 1) {
        py::print(DOC());
        return 2;
    }
    py::list<py::str> what;
    if (opts.get(py::str("what"), std::nullopt)) {
        for (const py::str& w : py::split(*opts.get(py::str("what"), std::nullopt), py::str(","))) {
            if (py::truthy(w.strip())) what.append(w.strip());
        }
    } else {
        what = DEFAULT().copy();
    }
    std::optional<int64_t> limit;
    std::optional<py::str> run_opt = opts.get(py::str("run"), std::nullopt);
    if (run_opt) {
        if (!what.contains(py::str("observe"))) what.append(py::str("observe"));
        limit = *run_opt == py::str("all") ? std::nullopt : std::optional<int64_t>(py::int_parse(*run_opt));
    } else if (what.contains(py::str("observe"))) {
        limit = 100;
    }
    Run result;
    try {
        result = run(rest[0], what, !flags.contains(py::str("--no-store")), limit);
    } catch (const py::ValueError& e) {
        py::print(py::percent(py::str("analyze: %s"), py::str(e.what())));
        return 1;
    }
    std::optional<py::str> out_opt = opts.get(py::str("out"), std::nullopt);
    py::str out = (out_opt && py::truthy(*out_opt)) ? *out_opt : state_path(py::str("analyze"), result.module.replace(py::str("#"), py::str("-")) + py::str(".txt"));
    py::os::makedirs(py::os::path::dirname(py::os::path::abspath(out)), 0777, true);
    write_text(out, text(result, flags.contains(py::str("--facts"))));
    std::optional<py::str> starts_opt = opts.get(py::str("starts"), std::nullopt);
    if (starts_opt && py::truthy(*starts_opt)) write_text(*starts_opt, py::join(py::str("\n"), starts(*result.image)) + py::str("\n"));
    if (flags.contains(py::str("--json"))) {
        using J = ui::service::Json;
        J all = J::object();
        all.set("module", J(result.module));
        all.set("kind", J(result.kind));
        all.set("functions", J(result.functions));
        J counts = J::object();
        for (const auto& c : result.counts) {
            J one = J::object();
            for (const auto& kv : std::get<1>(c)) {
                const Json& v = std::get<1>(kv);
                one.set(std::get<0>(kv).utf8(), v.kind == Json::Str ? J(v.s) : J(v.i));
            }
            counts.set(std::get<0>(c).utf8(), one);
        }
        all.set("counts", counts);
        J seconds = J::object();
        for (const auto& s : result.seconds) seconds.set(std::get<0>(s).utf8(), J(std::get<1>(s)));
        all.set("seconds", seconds);
        ui::service::Dump d;
        d.indent = 1;
        d.item = ",";
        py::print(py::str(ui::service::dumps(all, d)));
    } else if (flags.contains(py::str("--summary"))) {
        for (const auto& c : result.counts) {
            py::list<py::str> parts;
            int64_t n = 0;
            for (const auto& kv : std::get<1>(c)) {
                if (n++ >= 7) break;
                parts.append(py::percent(py::str("%s %s"), std::get<0>(kv).replace(py::str("_"), py::str(" ")), str_of(std::get<1>(kv))));
            }
            py::print(py::percent(py::str("%-12s %5.1fs  %s"), std::get<0>(c), result.second(std::get<0>(c)), py::join(py::str(", "), parts)));
        }
    } else {
        py::list<py::str> parts;
        for (const auto& c : result.counts) parts.append(py::percent(py::str("%s %.1fs"), std::get<0>(c), result.second(std::get<0>(c))));
        py::print(py::percent(py::str("%s: %d functions, %s; report in %s"), result.module, result.functions, py::join(py::str(", "), parts), out));
    }
    return 0;
}

}  // namespace codebreaker::analyze::report
