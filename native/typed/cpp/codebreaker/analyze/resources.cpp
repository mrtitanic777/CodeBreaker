// codebreaker/analyze/resources.cpp -- typed C++ port of codebreaker.analyze.resources.
// (The module's description is in resources.hpp.)

#include "codebreaker/analyze/resources.hpp"

namespace codebreaker::analyze::resources {

const py::dict<int64_t, py::str> TYPES_ = {
    {1, "CURSOR"}, {2, "BITMAP"}, {3, "ICON"}, {4, "MENU"}, {5, "DIALOG"}, {6, "STRING"},
    {9, "ACCELERATOR"}, {10, "RCDATA"}, {12, "GROUP_CURSOR"}, {14, "GROUP_ICON"}, {16, "VERSION"}};

// {type name: set of identifiers and (upper-case) names} the module itself holds.
py::dict<py::str, py::set<Ident>> carried(Image& img) {
    py::dict<py::str, py::set<Ident>> out;
    if (img.kind == "pe") {
        for (const PeType& t : img.pe_resource_tree()) {
            py::str name;
            if (t.id && TYPES_.contains(*t.id)) name = TYPES_.at(*t.id);
            else if (!t.id) name = t.type;
            else name = py::percent(py::str("#%s"), *t.id);
            for (const PeItem& item : t.items) {
                out.setdefault(name, py::set<Ident>()).add(item.id ? Ident(*item.id) : Ident(item.name.upper()));
            }
        }
    } else if (img.kind == "ne") {
        for (const NeResource& r : img.ne_resources()) {
            const py::str& ident = r.id;
            Ident key = (ident.startswith("#") && ident.slice_from(1).isdigit())
                            ? Ident(py::int_from_str(ident.slice_from(1)))
                            : Ident(ident.upper());
            out.setdefault(r.type, py::set<Ident>()).add(key);
        }
    }
    return out;
}

Uses _uses(Image& img) {
    py::dict<py::str, py::set<Ident>> have = carried(img);
    RawUses got = img.ask_uses();
    py::list<Load> loads;
    for (const RawLoad& raw : got.loads) {
        const int64_t f = std::get<0>(raw);
        const int64_t site = std::get<1>(raw);
        const py::str& stem = std::get<2>(raw);
        const std::optional<py::str>& kind = std::get<3>(raw);
        const Ident& ident = std::get<4>(raw);
        py::set<Ident> pool = have.get(kind ? *kind : py::str(""), py::set<Ident>());
        const int64_t* number = std::get_if<int64_t>(&ident);
        std::optional<bool> present;
        if (kind && *kind == "STRING" && number) {
            present = pool.contains(Ident(py::floordiv(*number, 16) + 1));
        } else if (kind && (*kind == "GROUP_ICON" || *kind == "GROUP_CURSOR") && number && *number >= 32000) {
            present = std::nullopt;   // one of the system's stock icons and cursors
        } else {
            present = pool.contains(ident);
        }
        loads.append(Load{f, site, stem, kind, ident, present});
    }
    py::list<Control> controls;
    for (const RawControl& c : got.controls) {
        controls.append(Control{std::get<0>(c), std::get<1>(c), std::get<2>(c), std::get<3>(c)});
    }
    return Uses(loads, controls);
}

Uses uses(Image& img) {
    if (!img.memo_resources) img.memo_resources = _uses(img);
    return *img.memo_resources;
}

Analysis analyse(Image& img) {
    Uses got = uses(img);
    const py::list<Load>& found = std::get<0>(got);
    const py::list<Control>& controls = std::get<1>(got);
    py::dict<py::str, py::set<Ident>> have = carried(img);

    py::list<Fact> facts;
    for (const Load& u : found) {
        facts.append(ResourceUseFact{u.function, u.site, u.call, u.type, u.id, u.present, u.function, "resource_use"});
    }
    for (const Control& c : controls) {
        facts.append(DialogControlFact{c.function, c.site, c.call, c.control, c.function, "dialog_control_use"});
    }
    py::list<py::str> typeNames = py::sorted(have);   // sorted(have.items()): keys are unique
    py::dict<py::str, int64_t> types;
    for (const py::str& t : typeNames) types.set(t, py::len(have.at(t)));
    facts.append(CarriedFact{std::nullopt, "resources_carried", types});

    py::dict<std::optional<py::str>, py::set<Ident>> referenced;
    for (const Load& u : found) {
        if (u.present && *u.present) referenced.setdefault(u.type, py::set<Ident>()).add(u.id);
    }
    int64_t carriedTotal = 0;
    for (const auto& kv : have.items()) carriedTotal += py::len(std::get<1>(kv));
    int64_t referencedTotal = 0;
    for (const auto& kv : referenced.items()) referencedTotal += py::len(std::get<1>(kv));
    int64_t presentCount = 0, absentCount = 0, stockCount = 0;
    py::set<int64_t> loaders;
    for (const Load& u : found) {
        if (u.present && *u.present) ++presentCount;
        if (u.present && !*u.present) ++absentCount;
        if (!u.present) ++stockCount;
        loaders.add(u.function);
    }
    py::set<int64_t> distinctControls;
    for (const Control& c : controls) distinctControls.add(c.control);

    Counts counts;
    counts.resource_types = py::len(have);
    counts.resources_carried = carriedTotal;
    counts.loads_read = py::len(found);
    counts.present_in_module = presentCount;
    counts.absent_from_module = absentCount;
    counts.system_stock = stockCount;
    counts.distinct_resources_referenced = referencedTotal;
    counts.functions_loading_resources = py::len(loaders);
    counts.dialog_control_uses = py::len(controls);
    counts.distinct_controls = py::len(distinctControls);

    py::dict<py::str, py::set<Ident>> by;
    for (const Load& u : found) {
        py::str key = (u.type && !u.type->empty()) ? *u.type : py::str("?");
        by.setdefault(key, py::set<Ident>()).add(u.id);
    }
    py::list<py::str> text;
    py::list<py::str> byNames = py::sorted(by);
    for (const py::str& t : byNames) {
        py::list<Ident> ids = py::sorted(by.at(t), [](const Ident& x) {
            return std::holds_alternative<int64_t>(x) ? py::to_str(std::get<int64_t>(x)) : std::get<py::str>(x);
        });
        py::list<py::str> shown;
        for (int64_t i = 0; i < py::min(py::len(ids), static_cast<int64_t>(14)); ++i) {
            const Ident& x = ids[i];
            shown.append(std::holds_alternative<int64_t>(x) ? py::to_str(std::get<int64_t>(x)) : std::get<py::str>(x));
        }
        text.append(py::percent(py::str("%s: %s"), t, py::join(py::str(", "), shown)));
    }
    return Analysis{counts, facts, text};
}

}  // namespace codebreaker::analyze::resources
