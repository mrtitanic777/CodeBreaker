// translate/support/l2schema.cc -- the L2 schema check (l2schema.hpp).
#include "translate/support/l2schema.hpp"

#include <cmath>

#include "translate/support/pystr.hpp"

namespace l2schema {

using py::V;

Schema::Schema(const V& schema) : root_(schema) {}

const V& Schema::resolve(const std::string& ref) const {
    if (ref == "#") return root_;
    if (!py::startswith(ref, "#/")) py::raise("ValueError", "schema: unsupported $ref " + ref);
    const V* cur = &root_;
    for (auto& part : py::split(ref.substr(2), "/")) cur = &cur->at(part.c_str());
    return *cur;
}

static bool same(const V& a, const V& b) {
    // jsonschema's equal: a bool is not a number
    if (a.is_bool() != b.is_bool()) return false;
    if (a.is_list() && b.is_list()) {
        if (a.size() != b.size()) return false;
        for (size_t n = 0; n < a.size(); n++)
            if (!same(a[n], b[n])) return false;
        return true;
    }
    if (a.is_dict() && b.is_dict()) {
        if (a.size() != b.size()) return false;
        for (auto& kv : a.items())
            if (!b.has(kv.first) || !same(kv.second, b.get(kv.first))) return false;
        return true;
    }
    return py::eq(a, b);
}

static bool type_is(const std::string& t, const V& v) {
    if (t == "object") return v.is_dict();
    if (t == "array") return v.is_list();
    if (t == "string") return v.is_str();
    if (t == "boolean") return v.is_bool();
    if (t == "null") return v.is_none();
    if (t == "integer") return v.is_int() || (v.is_float() && std::isfinite(v.f) && v.f == std::floor(v.f));
    if (t == "number") return v.is_int() || v.is_float();
    return false;
}

bool Schema::check(const V& s, const V& v, std::string& why, const std::string& at) const {
    if (s.is_bool()) {
        if (!s.b) why = at + ": not allowed";
        return s.b;
    }
    for (auto& kv : s.items()) {
        const std::string& key = kv.first.str();
        const V& w = kv.second;
        if (key == "$ref") {
            if (!check(resolve(w.str()), v, why, at)) return false;
        } else if (key == "type") {
            bool ok = false;
            if (w.is_str()) ok = type_is(w.str(), v);
            else
                for (auto& t : w.seq()) ok = ok || type_is(t.str(), v);
            if (!ok) {
                why = at + ": " + py::repr(v).substr(0, 80) + " is not of type " + py::repr(w);
                return false;
            }
        } else if (key == "const") {
            if (!same(w, v)) {
                why = at + ": " + py::repr(w) + " was expected";
                return false;
            }
        } else if (key == "enum") {
            bool ok = false;
            for (auto& e : w.seq()) ok = ok || same(e, v);
            if (!ok) {
                why = at + ": " + py::repr(v).substr(0, 80) + " is not one of " + py::repr(w).substr(0, 120);
                return false;
            }
        } else if (key == "anyOf" || key == "oneOf" || key == "allOf") {
            int good = 0;
            std::string sub;
            for (auto& alt : w.seq()) {
                std::string w2;
                if (check(alt, v, w2, at)) good++;
                else if (sub.empty()) sub = w2;
                else if (key == "allOf") {
                }
                if (key == "allOf" && good == 0 && !w2.empty()) {
                    why = w2;
                    return false;
                }
            }
            bool ok = key == "anyOf" ? good > 0 : key == "oneOf" ? good == 1 : good == (int)w.size();
            if (!ok) {
                why = at + ": " + py::repr(v).substr(0, 80) + " is not valid under " + key + (sub.empty() ? "" : " (" + sub + ")");
                return false;
            }
        } else if (key == "if") {
            std::string w2;
            bool cond = check(w, v, w2, at);
            const V& then = s.get(cond ? "then" : "else");
            if (!then.is_none() && !check(then, v, why, at)) return false;
        } else if (key == "properties") {
            if (!v.is_dict()) continue;
            for (auto& p : w.items())
                if (v.has(p.first) && !check(p.second, v.get(p.first), why, at + "/" + p.first.str())) return false;
        } else if (key == "additionalProperties") {
            if (!v.is_dict()) continue;
            const V& props = s.get("properties");
            for (auto& p : v.items()) {
                if (props.is_dict() && props.has(p.first)) continue;
                if (w.is_bool() && !w.b) {
                    why = at + ": additional property " + py::repr(p.first) + " not allowed";
                    return false;
                }
                if (!w.is_bool() && !check(w, p.second, why, at + "/" + p.first.str())) return false;
            }
        } else if (key == "required") {
            if (!v.is_dict()) continue;
            for (auto& r : w.seq())
                if (!v.has(r)) {
                    why = at + ": " + py::repr(r) + " is a required property";
                    return false;
                }
        } else if (key == "items") {
            if (!v.is_list()) continue;
            size_t n = 0;
            for (auto& x : v.seq())
                if (!check(w, x, why, at + "/" + std::to_string(n++))) return false;
        } else if (key == "minItems") {
            if (v.is_list() && (int64_t)v.size() < w.as_int()) {
                why = at + ": too short";
                return false;
            }
        } else if (key == "minimum") {
            if ((v.is_int() || v.is_float()) && v.as_float() < w.as_float()) {
                why = at + ": below the minimum";
                return false;
            }
        }
        // (description, title, default, $schema, $id, $defs, then, else: no assertion)
    }
    return true;
}

bool Schema::valid(const V& doc, std::string* why) const {
    std::string w;
    bool ok = check(root_, doc, w, "");
    if (why) *why = w;
    return ok;
}

}  // namespace l2schema
