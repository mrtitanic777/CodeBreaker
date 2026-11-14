// cb/decompile/decompiler_facts.cpp -- the facts a function is written with, as the L2 library takes them, and what it leaves in them.
//
// The C back end is asked for a function's C with the module's facts of it (what the module says: the function's model in the
// description, the globals of the module, the structures recovered ...) and answers, besides the C, what writing it found
// (the heading as written, the names used, the interrupts taken ...) by leaving it in the same facts. The library's value
// form has more than JSON has: a tuple, a set, a dict with keys that are not text. They are carried as the library tags them --
// {"$t": [...]}, {"$s": [...]}, {"$d": [[key, value], ...]} -- so the facts cross as `Json` and are typed here, at the edge.
#include <nlohmann/json.hpp>

#include "cb/core/json.hpp"
#include "cb/decompile/decompiler_internal.hpp"

namespace cb::decompile::decompiler::internal {

namespace {

using c_text::ModuleFacts;

Json facts_tuple(std::initializer_list<Json> items) { return Json{{"$t", Json(items)}}; }

Json facts_set_of_text(const std::set<std::string>& items) {
    Json list = Json::array();
    list.get_ref<Json::array_t&>().reserve(items.size());
    for (const std::string& s : items) list.push_back(s);
    Json out = Json::object();
    out["$s"] = std::move(list);   // (moved: `Json{{"$s", list}}` copies the whole list)
    return out;
}

Json facts_set_of_numbers(const std::set<std::int64_t>& items) {
    Json list = Json::array();
    for (const std::int64_t n : items) list.push_back(n);
    Json out = Json::object();
    out["$s"] = std::move(list);
    return out;
}

template <class Map>
Json facts_text_to_number(const Map& map) {
    // (Json keeps its keys in insertion order in a vector, and `out[k] =` searches it for the key: quadratic in the map's size, and the
    // module's maps run to thousands of names, rebuilt for every function. The keys of a std::map are distinct, so they go in straight.)
    typename Json::object_t object;
    object.reserve(map.size());
    for (const auto& [k, v] : map) object.emplace_back(k, Json(v));
    return Json(std::move(object));
}

// A dict of the library's with keys that are not text: {"$d": [[key, value], ...]}; an empty one is the plain `{}`.
Json facts_dict_of(const std::vector<std::pair<Json, Json>>& items) {
    if (items.empty()) return Json::object();
    Json list = Json::array();
    for (const auto& [k, v] : items) list.push_back(Json::array({k, v}));
    return Json{{"$d", list}};
}

// ---- reading the library's form -----------------------------------------------------------------------------------------

bool is_tagged(const Json& j, const char* tag) { return j.is_object() && j.size() == 1 && j.contains(tag); }

// The items of a list, tuple or set (null: none).
const Json::array_t& seq_of(const Json& j) {
    static const Json::array_t none;
    if (j.is_array()) return *j.get_ptr<const Json::array_t*>();
    for (const char* tag : {"$t", "$s"})
        if (is_tagged(j, tag) && j.at(tag).is_array()) return *j.at(tag).get_ptr<const Json::array_t*>();
    return none;
}

// The (key, value) pairs of a dict: a plain object, or {"$d": [[key, value], ...]}.
std::vector<std::pair<Json, const Json*>> pairs_of(const Json& j) {
    std::vector<std::pair<Json, const Json*>> out;
    if (is_tagged(j, "$d")) {
        for (const Json& pair : j.at("$d")) out.emplace_back(pair.at(0), &pair.at(1));
    } else if (j.is_object()) {
        for (const auto& [k, v] : j.items()) out.emplace_back(Json(k), &v);
    }
    return out;
}

std::string text_of(const Json& j) { return j.is_string() ? j.get<std::string>() : std::string(); }
std::int64_t number_of(const Json& j) { return j.is_number() ? j.get<std::int64_t>() : 0; }

// Python's truth: null, false, zero, empty text, an empty list or dict are false.
bool truthy(const Json& j) {
    if (j.is_null()) return false;
    if (j.is_boolean()) return j.get<bool>();
    if (j.is_number_integer() || j.is_number_unsigned()) return j.get<std::int64_t>() != 0;
    if (j.is_number_float()) return j.get<double>() != 0;
    if (j.is_string()) return !j.get_ref<const std::string&>().empty();
    if (is_tagged(j, "$t") || is_tagged(j, "$s")) return !seq_of(j).empty();
    if (is_tagged(j, "$d")) return !j.at("$d").empty();
    return !j.empty();
}

// A key the back end wrote that says something: present and not empty (the facts hold None and empty values as absent).
bool given(const Json& facts, const char* key) { return facts.contains(key) && truthy(facts.at(key)); }

std::set<std::string> text_set_of(const Json& j) {
    std::set<std::string> out;
    for (const Json& x : seq_of(j)) out.insert(text_of(x));
    return out;
}

}  // namespace

Json facts_json(const ModuleFacts& m) {
    Json w = Json::object();
    if (m.path == ModuleFacts::Path::Win16) {
        w["distance"] = m.distance ? Json(*m.distance) : Json(nullptr);
        w["model"] = m.model ? Json(*m.model) : Json(nullptr);
        w["far_return"] = m.far_return;
        w["selector"] = m.selector ? Json(*m.selector) : Json(nullptr);
        if (m.have_code)
            w["widths"] = facts_dict_of({{Json(true), facts_text_to_number(m.widths_far)}, {Json(false), facts_text_to_number(m.widths_near)}});
        else
            w["widths"] = Json::object();
        w["loads"] = facts_set_of_numbers(m.loads);
        w["export"] = m.exported;
        w["loadds"] = m.loadds;
        Json stores = Json::array();
        for (const std::int64_t v : m.ss_stores) stores.push_back(v);
        w["ss_stores"] = stores;
        w["selector_structs"] = m.selector_structs;
        w["callee_pops"] = m.callee_pops;
        w["without_notes"] = m.without_notes;
        std::vector<std::pair<Json, Json>> values;
        for (const auto& [value, import] : m.selector_values)
            values.emplace_back(Json(value), facts_tuple({Json(import.symbol), Json(import.use == describe::SelectorUse::Segment ? "segment" : "offset")}));
        w["selector_values"] = facts_dict_of(values);
    } else if (m.path == ModuleFacts::Path::PowerPc) {
        w["lane"] = "powerpc";
        w["functions"] = facts_set_of_text(m.functions);
        return w;   // (the module's own plans are not given to it)
    } else {
        const c_text::FlatShared* s = m.shared.get();
        w["functions"] = facts_set_of_text(s ? s->functions : m.functions);
        w["struct_sizes"] = facts_text_to_number(m.struct_sizes);
        w["called_through"] = facts_set_of_text(s ? s->called_through : m.called_through);
        w["without_notes"] = m.without_notes;
        w["aliases"] = facts_text_to_number(m.aliases);
        w["pointer_globals"] = facts_set_of_text(s ? s->pointer_globals : m.pointer_globals);
        w["takes"] = facts_text_to_number(s ? s->takes : m.takes);
        if (m.struct_fields) {
            Json structures = Json::object();
            for (const auto& [param, planned] : *m.struct_fields) {
                std::vector<std::pair<Json, Json>> fields;
                for (const auto& [offset, field] : planned.fields)
                    fields.emplace_back(Json(offset), facts_tuple({Json(field.type), Json(field.size)}));
                structures[param] = facts_tuple({Json(planned.structure), facts_dict_of(fields)});
            }
            w["struct_fields"] = structures;
        }
    }
    // (the plans the module carries from one pass to the next: the library's own documents, kept as text)
    for (const auto& [key, text] : m.plans) w[key] = json::parse(text);
    return w;
}

namespace {
std::string wire_text(const Json& j) { return j.dump(-1, ' ', true); }   // (as native_l2 writes what it sends: ASCII only)
}  // namespace

std::string facts_text(const ModuleFacts& m) {
    const c_text::FlatShared* s = m.shared.get();
    if (m.path != ModuleFacts::Path::Flat32 || s == nullptr) return wire_text(facts_json(m));
    std::call_once(s->text_once, [s] {
        s->functions_text = wire_text(facts_set_of_text(s->functions));
        s->called_through_text = wire_text(facts_set_of_text(s->called_through));
        s->pointer_globals_text = wire_text(facts_set_of_text(s->pointer_globals));
        s->takes_text = wire_text(facts_text_to_number(s->takes));
    });
    // (the members in the order facts_json puts them)
    std::string out = "{\"functions\":";
    out += s->functions_text;
    out += ",\"struct_sizes\":";
    out += wire_text(facts_text_to_number(m.struct_sizes));
    out += ",\"called_through\":";
    out += s->called_through_text;
    out += ",\"without_notes\":";
    out += m.without_notes ? "true" : "false";
    out += ",\"aliases\":";
    out += wire_text(facts_text_to_number(m.aliases));
    out += ",\"pointer_globals\":";
    out += s->pointer_globals_text;
    out += ",\"takes\":";
    out += s->takes_text;
    if (m.struct_fields) {
        Json structures = Json::object();
        for (const auto& [param, planned] : *m.struct_fields) {
            std::vector<std::pair<Json, Json>> fields;
            for (const auto& [offset, field] : planned.fields)
                fields.emplace_back(Json(offset), facts_tuple({Json(field.type), Json(field.size)}));
            structures[param] = facts_tuple({Json(planned.structure), facts_dict_of(fields)});
        }
        out += ",\"struct_fields\":";
        out += wire_text(structures);
    }
    for (const auto& [key, text] : m.plans) {
        out += ',';
        out += wire_text(Json(key));
        out += ':';
        out += wire_text(json::parse(text));
    }
    out += '}';
    return out;
}

void apply_left(ModuleFacts& base, const Json& w) {
    if (w.contains("declared") && !w.at("declared").is_null()) base.declared = text_of(w.at("declared"));
    if (w.contains("registers") && !w.at("registers").is_null()) base.registers = text_of(w.at("registers"));
    base.structs_used.clear();
    if (w.contains("structs_used")) base.structs_used = text_set_of(w.at("structs_used"));
    base.novalue = w.contains("novalue") && truthy(w.at("novalue"));
    if (given(w, "interrupts")) {
        for (const Json& t : seq_of(w.at("interrupts"))) {
            c_text::Interrupt i;
            const Json::array_t& parts = seq_of(t);
            if (parts.size() < 2) continue;
            i.vector = static_cast<std::uint32_t>(number_of(parts[0]));
            for (const Json& r : seq_of(parts[1])) i.registers.push_back(text_of(r));
            base.interrupts.insert(i);
        }
    }
    base.selector_decls.clear();
    if (given(w, "selector_decls"))
        for (const Json& x : seq_of(w.at("selector_decls"))) base.selector_decls.push_back(text_of(x));
    if (given(w, "selector_shared")) {
        for (const auto& [structure, per] : pairs_of(w.at("selector_shared")))
            for (const auto& [function, fields] : pairs_of(*per))
                for (const auto& [offset, type] : pairs_of(*fields))
                    base.selector_shared[text_of(structure)][text_of(function)][number_of(offset)] = text_of(*type);
    }
    if (given(w, "selector_imports"))
        for (const auto& [import, how] : pairs_of(w.at("selector_imports"))) base.selector_imports[text_of(import)] = text_of(*how);
    if (given(w, "names")) {
        const Json& u = w.at("names");
        c_text::NamesUsed n;
        n.called = text_set_of(u.at("called"));
        n.called_through = text_set_of(u.at("called_through"));
        for (const Json& g : seq_of(u.at("globals"))) {
            const Json::array_t& pair = seq_of(g);
            if (pair.size() >= 2) n.globals.insert({text_of(pair[0]), text_of(pair[1])});
        }
        for (const Json& s : seq_of(u.at("segments"))) n.segments.insert(number_of(s));
        n.mk_fp = truthy(u.at("mk_fp"));
        base.names = std::move(n);
    }
    if (given(w, "heading")) {
        const Json& x = w.at("heading");
        c_text::Heading h;
        if (truthy(x.at("params"))) {
            const Json::array_t& params = seq_of(x.at("params"));
            c_text::Heading::Params p;
            p.text = text_of(params.at(0));
            for (const Json& pair : seq_of(params.at(1))) {
                const Json::array_t& np = seq_of(pair);
                if (np.size() >= 2) p.list.emplace_back(text_of(np[0]), text_of(np[1]));
            }
            h.params = std::move(p);
        }
        for (const auto& [name, type] : pairs_of(x.at("types"))) h.types.insert_or_assign(text_of(name), text_of(*type));
        for (const Json& c : seq_of(x.at("calls"))) {
            const Json::array_t& call = seq_of(c);
            if (call.size() < 2) continue;
            std::vector<std::string> args;
            for (const Json& a : seq_of(call[1])) args.push_back(text_of(a));
            h.calls.emplace_back(text_of(call[0]), std::move(args));
        }
        h.declaration = text_of(x.at("declaration"));
        h.fastcall = truthy(x.at("fastcall"));
        base.heading = std::move(h);
    }
    // (the plans the back end made for the module to carry)
    for (const char* key : {"frame", "code_reads"})
        if (w.contains(key)) base.plans.insert_or_assign(key, json::dump(w.at(key), json::Style::Compact));
}

}  // namespace cb::decompile::decompiler::internal
