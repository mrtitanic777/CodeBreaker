// match.cc -- what a translated match statement asks of its subject (PEP 634, as CPython's
// MATCH_SEQUENCE / MATCH_MAPPING / MATCH_KEYS / MATCH_CLASS do): each answers the values the
// sub-patterns go on to test, or None when the pattern cannot match.
#include "rt.hpp"

namespace py {

TypeObj* T(const var& v);   // object.cc

static bool abc_instance(const var& subj, const char* name) {
    var abc = import("collections.abc");
    return isinstance(subj, abc.getattr(name)).truthy();
}

var match_sequence_(const var& subj, size_t before, size_t after, bool star) {
    TypeObj* t = type_of(subj);
    if (isinstance_(subj, T_str) || isinstance_(subj, T_bytes) || isinstance_(subj, T_bytearray)) return None;
    bool seq = isinstance_(subj, T_list) || isinstance_(subj, T_tuple) || isinstance_(subj, T_range);
    if (!seq) {
        if (isinstance_(subj, T_dict) || isinstance_(subj, T_set) || isinstance_(subj, T_frozenset)) return None;
        if (t->name == "memoryview" || t->name == "deque" || t->name == "array") seq = true;
        else seq = abc_instance(subj, "Sequence");
    }
    if (!seq) return None;
    int64_t n = index_(len(subj));
    size_t need = before + after;
    if (star ? (size_t)n < need : (size_t)n != need) return None;
    std::vector<var> items = collect(subj);
    std::vector<var> out;
    for (size_t i = 0; i < before; i++) out.push_back(items[i]);
    if (star) out.push_back(new_list(std::vector<var>(items.begin() + (ptrdiff_t)before, items.end() - (ptrdiff_t)after)));
    for (size_t i = items.size() - after; i < items.size(); i++) out.push_back(items[i]);
    return new_list(out);
}

var match_mapping_(const var& subj, const var& keys, bool rest) {
    if (!isinstance_(subj, T_dict) && !abc_instance(subj, "Mapping")) return None;
    const std::vector<var>& ks = tuple_items(keys);
    if (index_(len(subj)) < (int64_t)ks.size()) return None;
    var missing = call_type(T_object, CallArgs());
    std::vector<var> out;
    for (auto& k : ks) {
        var v = subj.call("get", {k, missing});
        if (is(v, missing)) return None;
        out.push_back(v);
    }
    if (rest) {
        var d = new_dict_obj();
        dict_update_(d, subj);
        for (auto& k : ks) d.delitem(k);
        out.push_back(d);
    }
    return new_list(out);
}

var match_class_(const var& subj, const var& cls, size_t npos, const var& kwd_names) {
    if (!(cls.is_ref() && cls.ref()->type->layout == Layout::Type)) raise(E_TypeError, "called match pattern must be a class");
    if (!isinstance(subj, cls).truthy()) return None;
    TypeObj* ct = static_cast<TypeObj*>(cls.ref());
    std::vector<var> names;
    bool self_match = false;
    if (npos > 0) {
        var nomatch = call_type(T_object, CallArgs());
        var ma = getattr(cls, "__match_args__"_s, nomatch);
        if (is(ma, nomatch)) {
            // bool, bytearray, bytes, dict, float, frozenset, int, list, set, str, tuple (and their subclasses) match themselves
            for (TypeObj* b : {T_bool, T_bytearray, T_bytes, T_dict, T_float, T_frozenset, T_int, T_list, T_set, T_str, T_tuple})
                if (is_subtype(ct, b)) self_match = true;
            if (!self_match) raise(E_TypeError, ct->name + "() accepts 0 positional sub-patterns (" + std::to_string(npos) + " given)");
            if (npos > 1) raise(E_TypeError, ct->name + "() accepts 1 positional sub-pattern (" + std::to_string(npos) + " given)");
        } else {
            if (!is_tuple(ma)) raise(E_TypeError, ct->name + ".__match_args__ must be a tuple (got " + tname(ma) + ")");
            const auto& mv = tuple_items(ma);
            if (npos > mv.size())
                raise(E_TypeError, ct->name + "() accepts " + std::to_string(mv.size()) + " positional sub-pattern" + (mv.size() == 1 ? "" : "s") + " (" +
                                       std::to_string(npos) + " given)");
            for (size_t i = 0; i < npos; i++) names.push_back(mv[i]);
        }
    }
    for (auto& k : tuple_items(kwd_names)) names.push_back(k);
    std::vector<var> out;
    if (self_match) out.push_back(subj);
    for (auto& n : names) {
        var v;
        try {
            v = getattr(subj, n);
        } catch (error& e) {
            if (isinstance_(e.value, E_AttributeError)) return None;
            throw;
        }
        out.push_back(v);
    }
    return new_list(out);
}

}  // namespace py
