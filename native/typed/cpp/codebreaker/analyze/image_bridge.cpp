// codebreaker/analyze/image_bridge.cpp -- see the header.
#include "codebreaker/analyze/image_bridge.hpp"

#include "json.hpp"

namespace codebreaker::analyze::bridge {

namespace {

py::V ask(image::Image& img, const char* question, bool with_names) {
    native::Handle& h = img._handle();
    if (with_names) h.names(img.as_native());   // (the image's names as they are now)
    auto text = h.ask(py::str(question));
    return text ? py::json_parse(text->utf8()) : py::V();
}

std::optional<int64_t> opt_int(const py::V& v) {
    if (v.is_none()) return std::nullopt;
    return v.as_int();
}

py::list<int64_t> ints(const py::V& v) {
    py::list<int64_t> out;
    for (const py::V& x : v.seq()) out.append(x.as_int());
    return out;
}

py::list<py::str> strs(const py::V& v) {
    py::list<py::str> out;
    for (const py::V& x : v.seq()) out.append(py::str(x.str()));
    return out;
}

// ---- call_graph::Image ----
class CallGraphImage : public call_graph::Image {
  public:
    explicit CallGraphImage(image::Image& img) : img_(img) {
        functions = img.functions;
        imports = img.imports;
        roots = img.roots;
        discovered = img.discovered;
        for (const native::Span& s : img.spans.vec()) {
            call_graph::Span c;
            c.va = std::get<0>(s);
            c.data = std::get<1>(s);
            c.executable = std::get<2>(s);
            c.writable = std::get<3>(s);
            c.name = std::get<4>(s) ? *std::get<4>(s) : py::str();
            c.initialised = std::get<5>(s);
            spans.append(c);
        }
    }

    call_graph::RawGraph ask_call_graph() override {
        py::V got = ask(img_, "call_graph.build", false);
        call_graph::RawGraph g;
        for (const py::V& row : got.get("calls").seq()) {
            py::list<call_graph::RawCall> cs;
            for (const py::V& c : row[1].seq()) {
                call_graph::RawCall rc;
                rc.site = c[0].as_int();
                rc.kind = py::str(c[1].str());
                rc.destination = opt_int(c[2]);
                cs.append(rc);
            }
            g.calls.append(std::make_tuple(row[0].as_int(), cs));
        }
        for (const py::V& row : got.get("callers").seq()) g.callers.append(std::make_tuple(row[0].as_int(), ints(row[1])));
        for (const py::V& row : got.get("users").seq()) g.users.append(std::make_tuple(row[0].as_int(), ints(row[1])));
        for (const py::V& r : got.get("refs").seq()) {
            call_graph::RawRef rr;
            rr.function = r[0].as_int();
            rr.at = r[1].as_int();
            rr.va = r[2].as_int();
            rr.how = py::str(r[3].str());
            g.refs.append(rr);
        }
        for (const py::V& row : got.get("taken").seq()) {
            py::list<call_graph::RawTaken> ts;
            for (const py::V& t : row[1].seq()) {
                call_graph::RawTaken rt;
                rt.kind = py::str(t[0].str());
                rt.function = opt_int(t[1]);
                rt.at = t[2].as_int();
                ts.append(rt);
            }
            g.taken.append(std::make_tuple(row[0].as_int(), ts));
        }
        g.order = ints(got.get("order"));
        for (const py::V& c : got.get("cycles").seq()) g.cycles.append(ints(c));
        g.reachable = ints(got.get("reachable"));
        g.reachable_with_tables = ints(got.get("reachable_with_tables"));
        g.unreferenced = ints(got.get("unreferenced"));
        return g;
    }

    std::shared_ptr<call_graph::Function> function(int64_t start) override {
        auto it = cache_.find(start);
        if (it != cache_.end()) return it->second;
        image::FunctionP fn = img_.function(start);
        auto out = std::make_shared<call_graph::Function>();
        for (const image::InsP& i : fn->insns.vec()) {
            auto c = std::make_shared<call_graph::Ins>();
            c->addr = i->addr;
            c->size = i->size;
            out->insns.append(c);
        }
        out->index = fn->index;
        cache_[start] = out;
        return out;
    }

    py::str label(int64_t va) override { return img_.label(va); }

  private:
    image::Image& img_;
    std::map<int64_t, std::shared_ptr<call_graph::Function>> cache_;
};

// ---- strings::Image / globals::Image ----
class GlobalsImage : public globals::Image {
  public:
    explicit GlobalsImage(image::Image& img) : img_(img) {
        bits = img.bits;
        data_segment = img.data_segment;
    }

    std::optional<strings::Span> span(int64_t va) override {
        const native::Span* s = img_.span(va);
        if (!s) return std::nullopt;
        strings::Span c;
        c.va = std::get<0>(*s);
        c.data = std::get<1>(*s);
        c.executable = std::get<2>(*s);
        c.writable = std::get<3>(*s);
        c.name = std::get<4>(*s) ? *std::get<4>(*s) : py::str();
        c.initialised = std::get<5>(*s);
        return c;
    }
    py::str label(int64_t va) override { return img_.label(va); }

    py::list<strings::RawTableRow> ask_table() override {
        py::list<strings::RawTableRow> out;
        for (const py::V& r : ask(img_, "strings.table", true).seq()) {
            out.append(strings::RawTableRow(r[0].as_int(), py::str(r[1].str()), py::str(r[2].str()), py::truthy(r[3])));
        }
        return out;
    }
    py::list<strings::RawUsesRow> ask_uses() override {
        py::list<strings::RawUsesRow> out;
        for (const py::V& r : ask(img_, "strings.uses", true).seq()) {
            py::list<strings::Use> rows;
            for (const py::V& u : r[1].seq()) rows.append(strings::Use(u[0].as_int(), u[1].as_int(), py::str(u[2].str())));
            out.append(strings::RawUsesRow(r[0].as_int(), rows));
        }
        return out;
    }
    py::list<strings::StackString> ask_stack() override {
        py::list<strings::StackString> out;
        for (const py::V& r : ask(img_, "strings.stack", true).seq()) {
            out.append(strings::StackString(r[0].as_int(), py::str(r[1].str()), py::str(r[2].str()), py::truthy(r[3])));
        }
        return out;
    }
    strings::RawFormats ask_formats() override {
        py::V got = ask(img_, "strings.formats", true);
        strings::RawFormats out;
        for (const py::V& r : got.get("rows").seq()) {
            out.rows.append(strings::RawFormatRow(r[0].as_int(), r[1].as_int(), py::str(r[2].str()), py::str(r[3].str()),
                                                  py::truthy(r[4]), py::truthy(r[5])));
        }
        out.sites = got.get("sites").as_int();
        for (const py::V& o : got.get("own").seq()) out.own.append(std::make_tuple(o[0].as_int(), o[1].as_int()));
        return out;
    }
    py::list<globals::RawVariable> ask_variables() override {
        strings::table(*this);   // (globals' _ask: the strings table first)
        py::list<globals::RawVariable> out;
        for (const py::V& g : ask(img_, "globals.variables", true).seq()) {
            globals::RawVariable v;
            v.at = g.get("at").as_int();
            v.size = g.get("size").as_int();
            v.readers = ints(g.get("readers"));
            v.writers = ints(g.get("writers"));
            v.taken = g.get("taken").as_int();
            v.notes = strs(g.get("notes"));
            v.values = ints(g.get("values"));
            v.writable = py::truthy(g.get("writable"));
            v.element = g.get("element").as_int();
            v.extent = g.get("extent").as_int();
            const py::V& in = g.get("initial");
            if (in.is_str()) v.initial = py::str(in.str());
            else v.initial = in.as_int();
            v.type = py::str(g.get("type").str());
            out.append(v);
        }
        return out;
    }
    py::list<globals::RawTable> ask_tables() override {
        strings::table(*this);
        py::list<globals::RawTable> out;
        for (const py::V& t : ask(img_, "globals.tables", true).seq()) {
            globals::RawTable r;
            r.at = t.get("at").as_int();
            r.count = t.get("count").as_int();
            r.stride = t.get("stride").as_int();
            r.shape = py::str(t.get("shape").str());
            r.kinds = strs(t.get("kinds"));
            r.targets = ints(t.get("targets"));
            out.append(r);
        }
        return out;
    }

  private:
    image::Image& img_;
};

}  // namespace

call_graph::Image& call_graph_of(image::Image& img) {
    return *img.memo<CallGraphImage>(py::str("bridge.call_graph"), [](image::Image& i) { return std::make_shared<CallGraphImage>(i); });
}

globals::Image& globals_of(image::Image& img) {
    return *img.memo<GlobalsImage>(py::str("bridge.globals"), [](image::Image& i) { return std::make_shared<GlobalsImage>(i); });
}

std::shared_ptr<call_graph::Graph> graph(image::Image& img) { return call_graph::build(call_graph_of(img)); }

std::optional<std::tuple<py::str, py::str>> string_at(image::Image& img, int64_t va, int64_t least) {
    const native::Span* s = img.span(va);
    if (!s || va - std::get<0>(*s) >= std::get<5>(*s)) return std::nullopt;
    int64_t off = va - std::get<0>(*s);
    const py::bytes& data = std::get<1>(*s);
    if (img.bits == 32) {
        auto w = strings::_wide(data, off, least);
        if (w) return std::make_tuple(*w, py::str("W"));
    }
    auto a = strings::_ascii(data, off, least);
    if (a) {
        unsigned char prev = off != 0 ? static_cast<unsigned char>(data.raw()[static_cast<size_t>(off - 1)]) : 0;
        if (!(py::len(*a) < 4 && off != 0 && 0x20 <= prev && prev < 0x7F)) return std::make_tuple(*a, py::str("A"));   // (not the tail of a longer one)
    }
    return std::nullopt;
}

strings::Table string_table(image::Image& img) { return strings::table(globals_of(img)); }

std::optional<std::tuple<int64_t, py::str, py::str>> string_literal(image::Image& img, const py::list<image::Op>& args, int64_t k) {
    py::list<strings::Arg> conv;
    for (const image::Op& o : args.vec()) {
        strings::Arg a;
        a.kind = o.kind;
        if (o.kind == "r") a.value = o.name;
        else a.value = o.value;
        conv.append(a);
    }
    return strings::literal(globals_of(img), conv, k);
}

globals::Tables pointer_tables(image::Image& img) { return globals::pointer_tables(globals_of(img)); }

}  // namespace codebreaker::analyze::bridge
