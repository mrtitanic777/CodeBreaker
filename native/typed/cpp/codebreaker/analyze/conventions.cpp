// codebreaker/analyze/conventions.cpp -- typed C++ from codebreaker.analyze.conventions (see the header).
//
// How each function is called and what kind of function it is. The facts are worked out in C++ (native/analyze,
// conventions.cc) over the image cbanalyze.dll holds; duplicates and the bridge imports are found here.
#include "codebreaker/analyze/conventions.hpp"

#include <algorithm>
#include <set>

#include "json.hpp"
#include "py_os.hpp"

namespace codebreaker::analyze::conventions {

const py::list<py::str> _THUNK_APIS = {py::str("THUNKCONNECT32"), py::str("THUNKCONNECT16"), py::str("C16THKSL01"),
                                       py::str("C32THKSL01"),     py::str("THUNKINITLS"),    py::str("QT_THUNK"),
                                       py::str("FT_THUNK"),       py::str("FT_PROLOG"),      py::str("FT_EXIT"),
                                       py::str("SSONBIGSTACK"),   py::str("SSCALL"),         py::str("MAPLS"),
                                       py::str("UNMAPLS"),        py::str("MAPSL"),          py::str("GETVDMPOINTER32W"),
                                       py::str("ALLOCSLCALLBACK"), py::str("GETPROCADDRESS32W"), py::str("K32THK1632PROLOG"),
                                       py::str("FT_EXIT0")};

std::optional<int64_t> stack_delta(const image::Ins& ins) {
    const py::str& mn = ins.mn;
    const py::list<image::Op>& ops = ins.ops;
    if (mn == "push") return !ops.empty() ? ops[ops.size() - 1].size : int64_t(4);
    if (mn == "pop") return -(!ops.empty() ? ops[ops.size() - 1].size : int64_t(4));
    if ((mn == "add" || mn == "sub") && ops.size() == 2 && ops[0].kind == "r" && image::family(ops[0].name) == "esp" && ops[1].kind == "i") {
        int64_t n = image::signed_(ops[1].value, ops[1].size);
        return mn == "sub" ? n : -n;
    }
    if (!ins.wr.contains(py::str("esp"))) return int64_t(0);
    return std::nullopt;
}

py::str _signature(image::Image& img, const image::Function& fn) {
    std::string joined;
    bool first = true;
    for (const image::InsP& ip : fn.insns.vec()) {
        const image::Ins& ins = *ip;
        if (!first) joined += "\n";
        first = false;
        if (ins.target && ins.flow) {
            py::str where = (fn.start <= *ins.target && *ins.target < fn.limit) ? py::percent(py::str("+%d"), *ins.target - fn.start)
                                                                                : img.label(*ins.target);
            joined += (ins.mn + py::str(" ") + where).utf8();
        } else {
            joined += ins.text.utf8();
        }
    }
    return py::hashlib::sha1(py::bytes(joined)).hexdigest();
}

namespace {

std::optional<py::str> opt_str(const py::V& v) {
    if (v.is_none()) return std::nullopt;
    return py::str(v.str());
}
std::optional<int64_t> opt_int(const py::V& v) {
    if (v.is_none()) return std::nullopt;
    return v.as_int();
}
py::list<py::str> str_list(const py::V& v) {
    py::list<py::str> out;
    for (const py::V& x : v.seq()) out.append(py::str(x.str()));
    return out;
}

Fact make_fact(const py::V& f) {
    Fact o;
    o.addr = f.get("addr").as_int();
    if (f.has("kind")) o.kind = py::str(f.get("kind").str());
    o.name = py::str(f.get("name").str());
    o.shape = py::str(f.get("shape").str());
    if (f.has("thunk_of")) o.thunk_of = opt_str(f.get("thunk_of"));
    if (f.has("constant")) o.constant = opt_int(f.get("constant"));
    if (f.has("tail_calls") && !f.get("tail_calls").is_none()) o.tail_calls = str_list(f.get("tail_calls"));
    o.convention = opt_str(f.get("convention"));
    if (f.has("reg_args")) o.reg_args = str_list(f.get("reg_args"));
    o.removes = opt_int(f.get("removes"));
    o.args = opt_int(f.get("args"));
    o.arg_source = opt_str(f.get("arg_source"));
    if (f.has("arg_slots") && !f.get("arg_slots").is_none()) {
        py::dict<py::str, int64_t> slots;
        for (auto& kv : f.get("arg_slots").items()) slots.set(py::str(kv.first.str()), kv.second.as_int());
        o.arg_slots = slots;
    }
    o.frame = py::truthy(f.get("frame"));
    o.locals = f.has("locals") ? f.get("locals").as_int() : 0;
    if (f.has("saved")) o.saved = str_list(f.get("saved"));
    o.result = opt_str(f.get("result"));
    if (f.has("result_evidence") && !f.get("result_evidence").is_none()) {
        ResultEvidence r;
        r.callers_use = f.get("result_evidence").get("callers_use").as_int();
        r.callers_ignore = f.get("result_evidence").get("callers_ignore").as_int();
        o.result_evidence = r;
    }
    if (f.has("loops")) o.loops = opt_int(f.get("loops"));
    if (f.has("string_ops")) o.string_ops = str_list(f.get("string_ops"));
    if (f.has("float_instructions")) o.float_instructions = opt_int(f.get("float_instructions"));
    if (f.has("float_args")) {
        py::list<int64_t> fl;
        for (const py::V& x : f.get("float_args").seq()) fl.append(x.as_int());
        o.float_args = fl;
    }
    o.variadic = py::truthy(f.get("variadic"));
    if (f.has("entry")) o.entry = str_list(f.get("entry"));
    if (f.has("procedure")) o.procedure = opt_str(f.get("procedure"));
    return o;
}

bool has(const std::optional<py::str>& s) { return s && s->size() != 0; }

}  // namespace

std::shared_ptr<Facts> _functions(image::Image& img) {
    auto out = std::make_shared<Facts>();
    auto text = img._handle().ask(py::str("conventions.functions"));
    if (!text) return out;
    py::V rows = py::json_parse(text->utf8());
    for (const py::V& f : rows.seq()) {
        Fact fact = make_fact(f);
        out->set(fact.addr, fact);
    }
    return out;
}

std::shared_ptr<Facts> functions(image::Image& img) {
    return img.memo<Facts>(py::str("conventions"), [](image::Image& i) { return _functions(i); });
}

py::list<py::list<int64_t>> duplicates(image::Image& img, int64_t least) {
    std::vector<py::str> order;
    std::map<std::string, std::vector<int64_t>> groups;
    for (const image::FunctionP& fn : img.each().vec()) {
        if (fn->insns.size() >= least) {
            py::str sig = _signature(img, *fn);
            if (!groups.count(sig.utf8())) order.push_back(sig);
            groups[sig.utf8()].push_back(fn->start);
        }
    }
    std::vector<std::vector<int64_t>> multi;
    for (const py::str& k : order) if (groups[k.utf8()].size() > 1) multi.push_back(groups[k.utf8()]);
    std::stable_sort(multi.begin(), multi.end(), [](const auto& a, const auto& b) { return a[0] < b[0]; });
    py::list<py::list<int64_t>> out;
    for (auto& g : multi) out.append(py::list<int64_t>(g));
    return out;
}

py::list<py::str> thunk_layer(const image::Image& img) {
    std::set<std::string> names;
    for (const py::str& v : img.imports.values()) {
        const std::u32string& u = v.u32();
        size_t p = u.rfind(U'!');
        names.insert(py::str(p == std::u32string::npos ? u : u.substr(p + 1)).upper().utf8());
    }
    std::set<std::string> apis;
    for (const py::str& a : _THUNK_APIS.vec()) apis.insert(a.utf8());
    py::list<py::str> out;
    for (const std::string& n : names) if (apis.count(n)) out.append(py::str(n));   // (sorted: std::set of UTF-8 text keeps code point order)
    return out;
}

Analysis analyse(image::Image& img) {
    auto facts = functions(img);
    auto dup = duplicates(img);
    auto bridge = thunk_layer(img);
    std::vector<Fact> rows = facts->values();
    auto n = [&](auto test) {
        int64_t c = 0;
        for (const Fact& f : rows) if (test(f)) c++;
        return c;
    };
    Analysis a;
    py::dict<py::str, int64_t>& counts = a.counts;
    auto put = [&](const char* k, int64_t v) { counts.set(py::str(k), v); };
    put("functions", static_cast<int64_t>(rows.size()));
    put("convention_known", n([](const Fact& f) { return has(f.convention); }));
    put("callee_removes_arguments", n([](const Fact& f) { return f.removes && *f.removes != 0; }));
    put("caller_removes_arguments", n([](const Fact& f) { return f.removes && *f.removes == 0; }));
    put("argument_count_known", n([](const Fact& f) { return f.args.has_value(); }));
    put("argument_count_from_return", n([](const Fact& f) { return f.arg_source && *f.arg_source == "return"; }));
    put("argument_count_from_callers", n([](const Fact& f) { return f.arg_source && *f.arg_source == "callers"; }));
    put("argument_count_from_frame", n([](const Fact& f) { return f.arg_source && *f.arg_source == "frame"; }));
    put("register_arguments", n([](const Fact& f) { return !f.reg_args.empty(); }));
    put("this_in_ecx", n([](const Fact& f) { return f.convention && *f.convention == "thiscall"; }));
    put("result_known", n([](const Fact& f) { return has(f.result); }));
    put("result_none", n([](const Fact& f) { return f.result && *f.result == "none"; }));
    put("import_thunks", n([](const Fact& f) { return f.shape == "import thunk"; }));
    put("jump_thunks", n([](const Fact& f) { return f.shape == "jump thunk"; }));
    put("tail_calling", n([](const Fact& f) { return f.tail_calls && !f.tail_calls->empty(); }));
    put("empty_or_constant", n([](const Fact& f) { return f.shape == "empty" || f.shape == "constant"; }));
    put("leaf", n([](const Fact& f) { return f.shape == "leaf"; }));
    put("with_loops", n([](const Fact& f) { return f.loops && *f.loops != 0; }));
    put("inline_string_operations", n([](const Fact& f) { return f.string_ops && !f.string_ops->empty(); }));
    put("floating_point", n([](const Fact& f) { return f.float_instructions && *f.float_instructions != 0; }));
    put("variadic", n([](const Fact& f) { return f.variadic; }));
    put("system_called_procedures", n([](const Fact& f) { return has(f.procedure); }));
    int64_t dupf = 0;
    for (const auto& g : dup.vec()) dupf += g.size();
    put("duplicate_groups", dup.size());
    put("duplicate_functions", dupf);
    put("bridge_imports", bridge.size());
    for (const Fact& f : rows) a.facts.append(AnyFact(f));
    for (const auto& g : dup.vec()) {
        DuplicatesRow r;
        r.addr = g[0];
        for (int64_t f : g.vec()) r.functions.append(img.label(f));
        a.facts.append(AnyFact(r));
    }
    if (!bridge.empty()) {
        ThunkLayerRow r;
        r.imports = bridge;
        a.facts.append(AnyFact(r));
    }
    std::vector<std::pair<std::string, int64_t>> by;
    for (const Fact& f : rows) {
        std::string k = has(f.convention) ? f.convention->utf8() : std::string("unknown");
        auto it = std::find_if(by.begin(), by.end(), [&](auto& p) { return p.first == k; });
        if (it == by.end()) by.emplace_back(k, 1);
        else it->second++;
    }
    // sorted(by.items(), key=lambda kv: -kv[1]) on items sorted by name first? No: Python sorts by.items() (insertion order) by -count
    std::stable_sort(by.begin(), by.end(), [](auto& x, auto& y) { return x.second > y.second; });
    std::string head = "conventions: ";
    for (size_t i = 0; i < by.size(); i++) head += (i ? ", " : "") + by[i].first + " " + std::to_string(by[i].second);
    a.text.append(py::str(head));
    std::vector<const Fact*> procs;
    for (const Fact& f : rows) if (has(f.procedure)) procs.push_back(&f);
    if (!procs.empty()) {
        std::string t = "system-called procedures: ";
        for (size_t i = 0; i < procs.size() && i < 12; i++) t += (i ? ", " : "") + procs[i]->name.utf8() + " (" + procs[i]->procedure->utf8() + ")";
        a.text.append(py::str(t));
    }
    if (!bridge.empty()) {
        std::string t = "16/32-bit bridge imports: ";
        for (int64_t i = 0; i < bridge.size(); i++) t += (i ? ", " : "") + bridge[i].utf8();
        a.text.append(py::str(t));
    }
    if (!dup.empty()) {
        int64_t big = 0;
        for (const auto& g : dup.vec()) big = std::max<int64_t>(big, g.size());
        a.text.append(py::percent(py::str("identical functions: %d groups, largest %d copies"), dup.size(), big));
    }
    return a;
}

}  // namespace codebreaker::analyze::conventions
