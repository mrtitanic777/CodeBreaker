// lib_mcp.cc -- the part of the `mcp` package (and mcp_types) CodeBreaker's MCP server uses, in C++, with
// the same wire behaviour as mcp 2.3 over stdio. The package itself is not translated: it stands on pydantic,
// whose core is Rust (state/work/pyrt-async-ui/thirdparty.md). What is here:
//
//   mcp.server.mcpserver.MCPServer   tool / resource / prompt decorators; run("stdio"): JSON-RPC 2.0, one
//                                    message per line, initialize / ping / tools / prompts / resources
//   mcp.server.mcpserver.Image, mcp.server.mcpserver.exceptions.ToolError, mcp_types.ToolAnnotations,
//   mcp.server.transport_security.TransportSecuritySettings
//   mcp.Client                       the in-process client the tests use (async with Client(server) as c:
//                                    await c.call_tool(...)): the same dispatch, the answers as objects
//
// A tool's input schema is what pydantic writes for its signature (titles, defaults, required, the types
// of the annotations' text), its arguments validated and converted as pydantic's lax mode does (with its
// error text), its result turned into content as mcp's _convert_to_content does (pydantic_core.to_json
// with indent 2 for anything but text). Streamable HTTP (mcp --http) is not here yet.
#include <windows.h>
#include <io.h>

#include <algorithm>
#include <memory>
#include <mutex>
#include <cmath>
#include <cstring>

#include "rt.hpp"

namespace py {

TypeObj* T(const var& v);   // object.cc
static var natf(const char* n, native_fn f) { return new_native(n, std::move(f)); }

// ------------------------------------------------------------------------------------------ JSON out
// compact (the wire: model_dump_json) or indented by 2 (pydantic_core.to_json(indent=2)); non-ASCII kept
static void jstr(std::string& o, const std::string& s) {
    o += '"';
    for (unsigned char c : s) {
        switch (c) {
        case '"': o += "\\\""; break;
        case '\\': o += "\\\\"; break;
        case '\n': o += "\\n"; break;
        case '\r': o += "\\r"; break;
        case '\t': o += "\\t"; break;
        case '\b': o += "\\b"; break;
        case '\f': o += "\\f"; break;
        default:
            if (c < 0x20) {
                char b[8];
                snprintf(b, sizeof b, "\\u%04x", c);
                o += b;
            } else o += (char)c;
        }
    }
    o += '"';
}
// a float as serde_json writes it (ryu: 1e16, 1.5e-7, 0.00001)
static std::string jfloat(double d) {
    if (std::isnan(d)) return "NaN";
    if (std::isinf(d)) return d > 0 ? "Infinity" : "-Infinity";
    std::string r = repr_float(d);
    size_t e = r.find('e');
    if (e == std::string::npos) return r;
    std::string mant = r.substr(0, e), ex = r.substr(e + 1);
    int x = std::stoi(ex);
    if (x == -5) {   // (Python writes 1.5e-05; ryu 0.000015)
        std::string digits = mant;
        bool neg = !digits.empty() && digits[0] == '-';
        if (neg) digits.erase(0, 1);
        std::string intp = digits.substr(0, digits.find('.')), frac = digits.find('.') == std::string::npos ? "" : digits.substr(digits.find('.') + 1);
        return std::string(neg ? "-" : "") + "0.0000" + intp + frac;
    }
    return mant + "e" + std::to_string(x);
}
static void jval(std::string& o, const var& v, int indent, int depth);
static void nl(std::string& o, int indent, int depth) {
    if (indent <= 0) return;
    o += '\n';
    o.append((size_t)(indent * depth), ' ');
}
static void jval(std::string& o, const var& v, int indent, int depth) {
    if (v.is_none() || v.is_unbound()) { o += "null"; return; }
    if (v.is_bool()) { o += v.truthy() ? "true" : "false"; return; }
    if (is_int(v)) { o += str_(v); return; }
    if (isinstance_(v, T_float)) { o += jfloat(v.as_double()); return; }
    if (is_str(v)) { jstr(o, str_utf8(v)); return; }
    if (is_bytes(v)) { jstr(o, raw_of(v)); return; }
    if (isinstance_(v, T_dict)) {
        std::vector<std::pair<var, var>> items;
        for (var k : each(v)) items.push_back({k, v.getitem(k)});
        if (items.empty()) { o += "{}"; return; }
        o += '{';
        bool first = true;
        for (auto& [k, x] : items) {
            if (!first) o += ',';
            first = false;
            nl(o, indent, depth + 1);
            jstr(o, is_str(k) ? str_utf8(k) : str_(k));
            o += indent > 0 ? ": " : ":";
            jval(o, x, indent, depth + 1);
        }
        nl(o, indent, depth);
        o += '}';
        return;
    }
    if (is_list(v) || is_tuple(v) || isinstance_(v, T_set) || isinstance_(v, T_frozenset)) {
        std::vector<var> xs = collect(v);
        if (xs.empty()) { o += "[]"; return; }
        o += '[';
        for (size_t i = 0; i < xs.size(); i++) {
            if (i) o += ',';
            nl(o, indent, depth + 1);
            jval(o, xs[i], indent, depth + 1);
        }
        nl(o, indent, depth);
        o += ']';
        return;
    }
    jstr(o, str_(v));   // (fallback=str)
}
static std::string to_json(const var& v, int indent = 0) {
    std::string o;
    jval(o, v, indent, 0);
    return o;
}

// ------------------------------------------------------------------------------------------ schemas
// The JSON schema pydantic writes for an annotation's text: str, int, float, bool, dict, list[X], X | None,
// Optional[X], Union[...], Any (or none: {})
struct TypeParser {
    std::string s;
    size_t i = 0;
    void ws() { while (i < s.size() && s[i] == ' ') i++; }
    std::string name() {
        ws();
        size_t a = i;
        while (i < s.size() && (isalnum((unsigned char)s[i]) || s[i] == '_' || s[i] == '.')) i++;
        return s.substr(a, i - a);
    }
    var one() {
        std::string n = name();
        if (n.rfind("typing.", 0) == 0) n = n.substr(7);
        std::vector<var> args;
        ws();
        if (i < s.size() && s[i] == '[') {
            i++;
            for (;;) {
                args.push_back(uni());
                ws();
                if (i < s.size() && s[i] == ',') { i++; continue; }
                if (i < s.size() && s[i] == ']') i++;
                break;
            }
        }
        var d = new_dict_obj();
        if (n == "str") d.setitem("type"_s, "string"_s);
        else if (n == "int") d.setitem("type"_s, "integer"_s);
        else if (n == "float") d.setitem("type"_s, "number"_s);
        else if (n == "bool") d.setitem("type"_s, "boolean"_s);
        else if (n == "None" || n == "NoneType") d.setitem("type"_s, "null"_s);
        else if (n == "dict" || n == "Dict") {
            d.setitem("additionalProperties"_s, args.size() == 2 ? args[1] : var(true));
            d.setitem("type"_s, "object"_s);
        } else if (n == "list" || n == "List" || n == "Sequence" || n == "set" || n == "Set" || n == "tuple" || n == "Tuple") {
            d.setitem("items"_s, args.empty() ? new_dict_obj() : args[0]);
            if (n == "set" || n == "Set") d.setitem("type"_s, "array"_s), d.setitem("uniqueItems"_s, var(true));
            else d.setitem("type"_s, "array"_s);
        } else if (n == "Optional" && args.size() == 1) {
            d.setitem("anyOf"_s, new_list({args[0], [] { var n = new_dict_obj(); n.setitem("type"_s, "null"_s); return n; }()}));
        } else if (n == "Union") {
            d.setitem("anyOf"_s, new_list(args));
        }
        return d;
    }
    var uni() {
        std::vector<var> alts{one()};
        ws();
        while (i < s.size() && s[i] == '|') {
            i++;
            alts.push_back(one());
            ws();
        }
        if (alts.size() == 1) return alts[0];
        var d = new_dict_obj();
        d.setitem("anyOf"_s, new_list(alts));
        return d;
    }
};
static var schema_of_type(const std::string& text) {
    TypeParser p;
    p.s = text;
    return p.uni();
}
static var sorted_dict(const var& d) {
    std::vector<var> keys = collect(d);
    std::sort(keys.begin(), keys.end(), [](const var& a, const var& b) { return str_utf8(a) < str_utf8(b); });
    var o = new_dict_obj();
    for (auto& k : keys) o.setitem(k, d.getitem(k));
    return o;
}
static std::string title_of(const std::string& n) {
    std::string t;
    bool up = true;
    for (char c : n) {
        if (c == '_') { t += ' '; up = true; continue; }
        if (isalpha((unsigned char)c)) { t += up ? (char)toupper((unsigned char)c) : (char)tolower((unsigned char)c); up = false; }
        else { t += c; up = true; }
    }
    return t;
}

// a tool's (or prompt's) parameters: name, its schema (or {} unannotated), its default (unbound: required)
struct ParamInfo { std::string name; var schema; var dflt; };
static std::vector<ParamInfo> params_of(const var& fn) {
    var sig = import("inspect").call("signature", fn);
    var empty = import("inspect").getattr("_empty");
    std::vector<ParamInfo> out;
    var ps = sig.getattr("parameters");
    for (var k : each(ps)) {
        var p = ps.getitem(k);
        int64_t kind = index_(p.getattr("kind"));
        if (kind == 2 || kind == 4) continue;   // (*args, **kwargs: not arguments of the schema)
        ParamInfo pi;
        pi.name = str_utf8(k);
        var ann = p.getattr("annotation");
        pi.schema = is(ann, empty) || !is_str(ann) ? new_dict_obj() : schema_of_type(str_utf8(ann));
        var d = p.getattr("default");
        if (!is(d, empty)) pi.dflt = d;
        out.push_back(pi);
    }
    return out;
}
static var input_schema(const std::vector<ParamInfo>& ps, const std::string& fn_name) {
    var props = new_dict_obj();
    std::vector<var> req;
    for (auto& p : ps) {
        var s = new_dict_obj();
        dict_update_(s, p.schema);
        if (!p.dflt.is_unbound()) s.setitem("default"_s, p.dflt);
        s.setitem("title"_s, new_str(title_of(p.name)));
        props.setitem(new_str(p.name), sorted_dict(s));
        if (p.dflt.is_unbound()) req.push_back(new_str(p.name));
    }
    var o = new_dict_obj();
    o.setitem("properties"_s, props);
    if (!req.empty()) o.setitem("required"_s, new_list(req));
    o.setitem("type"_s, "object"_s);
    o.setitem("title"_s, new_str(fn_name + "Arguments"));
    return o;
}

// ------------------------------------------------------------------------------------------ validation
// pydantic's lax mode on JSON input, with its messages: the converted value, or an error appended
struct VErr { std::string loc, msg, type; var input; };
static std::string tname_of(const var& v) {
    if (isinstance_(v, T_dict)) return "dict";
    if (is_list(v)) return "list";
    return tname(v);
}
static bool validate(const var& schema, const var& in, const std::string& loc, std::vector<VErr>& errs, var& out) {
    var any = schema.is_ref() && isinstance_(schema, T_dict) ? getattr(schema, "get"_s, None) : var(None);
    var anyof = isinstance_(schema, T_dict) ? schema.call("get", "anyOf"_s) : var(None);
    if (!anyof.is_none()) {
        std::vector<VErr> all;
        for (var alt : each(anyof)) {
            std::vector<VErr> e;
            var o;
            if (validate(alt, in, loc, e, o)) { out = o; return true; }
            all.insert(all.end(), e.begin(), e.end());
        }
        errs.insert(errs.end(), all.begin(), all.end());
        return false;
    }
    var ty = isinstance_(schema, T_dict) ? schema.call("get", "type"_s) : var(None);
    std::string t = ty.is_none() ? "" : str_utf8(ty);
    auto fail = [&](const std::string& tag, const std::string& msg) {
        std::string l = loc;
        if (tag == "none_required") l += (l.empty() ? "" : ".") + std::string("none");
        else if (!t.empty() && (in.is_none()) && false) l += "";
        errs.push_back({l, msg, tag, in});
        return false;
    };
    if (t.empty()) { out = in; return true; }
    if (t == "null") {
        if (in.is_none()) { out = in; return true; }
        return fail("none_required", "Input should be None");
    }
    if (t == "string") {
        if (is_str(in)) { out = in; return true; }
        return fail("string_type", "Input should be a valid string");
    }
    if (t == "integer") {
        if (in.is_bool()) { out = var((int64_t)(in.truthy() ? 1 : 0)); return true; }
        if (is_int(in)) { out = in; return true; }
        if (isinstance_(in, T_float)) {
            double d = in.as_double();
            if (std::floor(d) == d && std::isfinite(d)) { out = var((int64_t)d); return true; }
            return fail("int_from_float", "Input should be a valid integer, got a number with a fractional part");
        }
        if (is_str(in)) {
            std::string s = str_utf8(in);
            size_t a = s.find_first_not_of(" \t\n\r"), b = s.find_last_not_of(" \t\n\r");
            std::string x = a == std::string::npos ? "" : s.substr(a, b - a + 1);
            bool ok = !x.empty();
            for (size_t k = 0; k < x.size(); k++)
                if (!(isdigit((unsigned char)x[k]) || (k == 0 && (x[k] == '-' || x[k] == '+')) || x[k] == '_')) ok = false;
            if (ok && x != "-" && x != "+") { out = int_from(x); return true; }
            return fail("int_parsing", "Input should be a valid integer, unable to parse string as an integer");
        }
        return fail("int_type", "Input should be a valid integer");
    }
    if (t == "number") {
        if (is_int(in)) { out = in; return true; }
        if (isinstance_(in, T_float)) { out = in; return true; }
        if (is_str(in)) {
            try {
                out = call1(t::float_, in);
                return true;
            } catch (error&) {
                return fail("float_parsing", "Input should be a valid number, unable to parse string as a number");
            }
        }
        return fail("float_type", "Input should be a valid number");
    }
    if (t == "boolean") {
        if (in.is_bool()) { out = in; return true; }
        if (is_int(in)) {
            int64_t v = in.as_int64();
            if (v == 0 || v == 1) { out = var(v == 1); return true; }
            return fail("bool_parsing", "Input should be a valid boolean, unable to interpret input");
        }
        if (is_str(in)) {
            std::string s = str_utf8(in);
            for (auto& c : s) c = (char)tolower((unsigned char)c);
            for (const char* y : {"1", "on", "t", "true", "y", "yes"}) if (s == y) { out = var(true); return true; }
            for (const char* n : {"0", "off", "f", "false", "n", "no"}) if (s == n) { out = var(false); return true; }
            return fail("bool_parsing", "Input should be a valid boolean, unable to interpret input");
        }
        return fail("bool_type", "Input should be a valid boolean");
    }
    if (t == "array") {
        if (!is_list(in) && !is_tuple(in)) return fail("list_type", "Input should be a valid list");
        var items = schema.call("get", "items"_s);
        std::vector<var> got;
        bool ok = true;
        size_t k = 0;
        for (var x : each(in)) {
            var o;
            if (!validate(items.is_none() ? new_dict_obj() : items, x, loc + "." + std::to_string(k), errs, o)) ok = false;
            got.push_back(o);
            k++;
        }
        out = new_list(got);
        return ok;
    }
    if (t == "object") {
        if (!isinstance_(in, T_dict)) return fail("dict_type", "Input should be a valid dictionary");
        out = in;
        return true;
    }
    (void)any;
    out = in;
    return true;
}
static std::string validation_text(const std::vector<VErr>& errs, const std::string& model) {
    std::string o = std::to_string(errs.size()) + " validation error" + (errs.size() == 1 ? "" : "s") + " for " + model;
    for (auto& e : errs) {
        o += "\n" + e.loc + "\n  " + e.msg + " [type=" + e.type + ", input_value=" + repr_(e.input) + ", input_type=" + tname_of(e.input) + "]";
        o += "\n    For further information visit https://errors.pydantic.dev/2.12/v/" + e.type;
    }
    return o;
}

// ------------------------------------------------------------------------------------------ the server
static var g_tool_error, g_image_cls, g_annotations_cls;

static var dict_of(std::initializer_list<std::pair<const char*, var>> kv) {
    var d = new_dict_obj();
    for (auto& [k, v] : kv) d.setitem(new_str(k), v);
    return d;
}
static std::string doc_of(const var& fn) {
    var d = getattr(fn, "__doc__"_s, None);
    if (d.is_none() || !is_str(d)) return "";
    return str_utf8(import("inspect").call("cleandoc", d));
}
static var annotations_wire(const var& a) {
    var d = new_dict_obj();
    if (a.is_none() || a.is_unbound()) return d;
    struct K { const char* py; const char* wire; };
    static const K ks[] = {{"destructive_hint", "destructiveHint"}, {"idempotent_hint", "idempotentHint"}, {"open_world_hint", "openWorldHint"},
                           {"read_only_hint", "readOnlyHint"}, {"title", "title"}};
    for (auto& k : ks) {
        var v = getattr(a, new_str(k.py), None);
        if (!v.is_none()) d.setitem(new_str(k.wire), v);
    }
    return d;
}

// one result's content blocks (mcp's _convert_to_content)
static void content_of(const var& r, std::vector<var>& out) {
    if (r.is_none()) return;
    if (!g_image_cls.is_unbound() && isinstance(r, g_image_cls).truthy()) {
        var b64 = import("base64").call("b64encode", r.getattr("data")).call("decode");
        out.push_back(dict_of({{"data", b64}, {"mimeType", r.call("_mime_type")}, {"type", "image"_s}}));
        return;
    }
    if (is_list(r) || is_tuple(r)) {
        for (var x : each(r)) content_of(x, out);
        return;
    }
    std::string text = is_str(r) ? str_utf8(r) : to_json(r, 2);
    out.push_back(dict_of({{"text", new_str(text)}, {"type", "text"_s}}));
}

struct Server {
    var self;
    std::string name, title, instructions, version;
    struct Tool { std::string name, fn_name, description; var fn, schema, annotations; std::vector<ParamInfo> params; };
    struct Prompt { std::string name, description; var fn; std::vector<ParamInfo> params; };
    struct Template { std::string uri, name, description, mime; var fn; std::vector<std::string> vars; };
    std::vector<Tool> tools;
    std::vector<Prompt> prompts;
    std::vector<Template> templates;
};
static Server* S(const var& self) { return reinterpret_cast<Server*>((intptr_t)getattr(self, "_pyrt_server"_s, var(0)).as_int64()); }

static var rpc_error(int code, const std::string& msg, const var& data = var()) {
    var d = dict_of({{"code", var(code)}, {"message", new_str(msg)}});
    if (!data.is_unbound()) d.setitem("data"_s, data);
    return d;
}

static var call_tool(Server* s, const std::string& name, const var& args, bool& is_error) {
    is_error = false;
    const Server::Tool* t = nullptr;
    for (auto& x : s->tools)
        if (x.name == name) t = &x;
    std::vector<var> content;
    auto err_text = [&](const std::string& m) {
        is_error = true;
        return new_list({dict_of({{"text", new_str(m)}, {"type", "text"_s}})});
    };
    if (!t) return err_text("Unknown tool: " + name);
    // the arguments: validated and converted, by the parameters' schemas
    std::vector<VErr> errs;
    CallArgs ca;
    for (auto& p : t->params) {
        bool has = !args.is_none() && in(new_str(p.name), args);
        if (!has) {
            if (p.dflt.is_unbound()) errs.push_back({p.name, "Field required", "missing", args.is_none() ? new_dict_obj() : args});
            continue;
        }
        var v;
        if (validate(p.schema, args.getitem(new_str(p.name)), p.name, errs, v)) ca.kw.push_back({interned(p.name.c_str()), v});
    }
    if (!errs.empty()) return err_text("Error executing tool " + name + ": " + validation_text(errs, t->fn_name + "Arguments"));
    var r;
    try {
        r = call_obj(t->fn, ca);
    } catch (error& e) {
        if (!g_tool_error.is_unbound() && isinstance(e.value, g_tool_error).truthy())
            return err_text("Error executing tool " + name + ": " + str_(e.value));
        // an unexpected failure: said without its text, the traceback to stderr (as mcp logs it)
        std::string tb = tname(e.value) + ": " + str_(e.value);
        fprintf(stderr, "ERROR    Tool '%s' raised an unexpected exception: %s\n", name.c_str(), tb.c_str());
        fflush(stderr);
        return err_text("Error executing tool " + name);
    }
    content_of(r, content);
    return new_list(content);
}

static bool match_template(const Server::Template& t, const std::string& uri, std::vector<std::string>& vals) {
    // {name} matches one path segment
    size_t i = 0, j = 0;
    const std::string& p = t.uri;
    vals.clear();
    while (i < p.size()) {
        if (p[i] == '{') {
            size_t e = p.find('}', i);
            size_t k = j;
            while (k < uri.size() && uri[k] != '/') k++;
            if (k == j) return false;
            vals.push_back(uri.substr(j, k - j));
            j = k;
            i = e + 1;
            continue;
        }
        if (j >= uri.size() || uri[j] != p[i]) return false;
        i++;
        j++;
    }
    return j == uri.size();
}

static const char* g_versions[] = {"2024-11-05", "2025-03-26", "2025-06-18", "2025-11-25"};

// one JSON-RPC message (a dict) -> the response (a dict), or None for a notification
static var dispatch(Server* s, const var& msg) {
    var id = msg.call("get", "id"_s);
    var mv = msg.call("get", "method"_s);
    bool notification = !in("id"_s, msg);
    if (mv.is_none() || !is_str(mv)) return None;   // (a response to us: nothing asked)
    std::string method = str_utf8(mv);
    var params = msg.call("get", "params"_s);
    if (params.is_none()) params = new_dict_obj();
    var result, err;
    if (method == "initialize") {
        var want = params.call("get", "protocolVersion"_s);
        std::string v = g_versions[3];
        for (const char* x : g_versions)
            if (is_str(want) && str_utf8(want) == x) v = x;
        var caps = dict_of({{"prompts", dict_of({{"listChanged", var(false)}})},
                            {"resources", dict_of({{"listChanged", var(false)}, {"subscribe", var(false)}})},
                            {"tools", dict_of({{"listChanged", var(false)}})}});
        var info = new_dict_obj();
        info.setitem("name"_s, new_str(s->name));
        if (!s->title.empty()) info.setitem("title"_s, new_str(s->title));
        info.setitem("version"_s, new_str(s->version));
        result = new_dict_obj();
        result.setitem("capabilities"_s, caps);
        if (!s->instructions.empty()) result.setitem("instructions"_s, new_str(s->instructions));
        result.setitem("protocolVersion"_s, new_str(v));
        result.setitem("serverInfo"_s, info);
    } else if (method == "ping") {
        result = new_dict_obj();
    } else if (method.rfind("notifications/", 0) == 0) {
        return None;
    } else if (method == "tools/list") {
        std::vector<var> ts;
        for (auto& t : s->tools) {
            var d = new_dict_obj();
            d.setitem("annotations"_s, annotations_wire(t.annotations));
            if (!t.description.empty()) d.setitem("description"_s, new_str(t.description));
            d.setitem("inputSchema"_s, t.schema);
            d.setitem("name"_s, new_str(t.name));
            ts.push_back(d);
        }
        result = dict_of({{"tools", new_list(ts)}});
    } else if (method == "tools/call") {
        var nm = params.call("get", "name"_s);
        bool is_err;
        var content = call_tool(s, is_str(nm) ? str_utf8(nm) : "", params.call("get", "arguments"_s), is_err);
        result = new_dict_obj();
        result.setitem("content"_s, content);
        result.setitem("isError"_s, var(is_err));
    } else if (method == "prompts/list") {
        std::vector<var> ps;
        for (auto& p : s->prompts) {
            std::vector<var> as;
            for (auto& a : p.params) as.push_back(dict_of({{"name", new_str(a.name)}, {"required", var(a.dflt.is_unbound())}}));
            var d = new_dict_obj();
            d.setitem("arguments"_s, new_list(as));
            if (!p.description.empty()) d.setitem("description"_s, new_str(p.description));
            d.setitem("name"_s, new_str(p.name));
            ps.push_back(d);
        }
        result = dict_of({{"prompts", new_list(ps)}});
    } else if (method == "prompts/get") {
        var nm = params.call("get", "name"_s);
        const Server::Prompt* p = nullptr;
        for (auto& x : s->prompts)
            if (is_str(nm) && x.name == str_utf8(nm)) p = &x;
        if (!p) err = rpc_error(-32602, "Unknown prompt: " + (is_str(nm) ? str_utf8(nm) : str_(nm)));
        else {
            var args = params.call("get", "arguments"_s);
            CallArgs ca;
            if (isinstance_(args, T_dict))
                for (var k : each(args)) ca.kw.push_back({interned(str_utf8(k).c_str()), args.getitem(k)});
            try {
                var r = call_obj(p->fn, ca);
                std::vector<var> msgs;
                auto one = [&](const var& m) {
                    if (is_str(m)) msgs.push_back(dict_of({{"content", dict_of({{"text", m}, {"type", "text"_s}})}, {"role", "user"_s}}));
                    else msgs.push_back(m);
                };
                if (is_list(r) || is_tuple(r)) for (var m : each(r)) one(m);
                else one(r);
                result = new_dict_obj();
                if (!p->description.empty()) result.setitem("description"_s, new_str(p->description));
                result.setitem("messages"_s, new_list(msgs));
            } catch (error& e) {
                err = rpc_error(-32603, "Error rendering prompt " + p->name);
            }
        }
    } else if (method == "resources/list") {
        result = dict_of({{"resources", new_list({})}});
    } else if (method == "resources/templates/list") {
        std::vector<var> ts;
        for (auto& t : s->templates) {
            var d = new_dict_obj();
            if (!t.description.empty()) d.setitem("description"_s, new_str(t.description));
            if (!t.mime.empty()) d.setitem("mimeType"_s, new_str(t.mime));
            d.setitem("name"_s, new_str(t.name));
            d.setitem("uriTemplate"_s, new_str(t.uri));
            ts.push_back(d);
        }
        result = dict_of({{"resourceTemplates", new_list(ts)}});
    } else if (method == "resources/read") {
        var u = params.call("get", "uri"_s);
        std::string uri = is_str(u) ? str_utf8(u) : "";
        const Server::Template* hit = nullptr;
        std::vector<std::string> vals;
        for (auto& t : s->templates)
            if (match_template(t, uri, vals)) { hit = &t; break; }
        if (!hit) err = rpc_error(-32602, "Unknown resource: " + uri, dict_of({{"uri", new_str(uri)}}));
        else {
            CallArgs ca;
            for (size_t i = 0; i < hit->vars.size() && i < vals.size(); i++) ca.kw.push_back({interned(hit->vars[i].c_str()), new_str(vals[i])});
            try {
                var r = call_obj(hit->fn, ca);
                var c = new_dict_obj();
                if (!hit->mime.empty()) c.setitem("mimeType"_s, new_str(hit->mime));
                c.setitem("text"_s, is_str(r) ? r : new_str(to_json(r, 2)));
                c.setitem("uri"_s, new_str(uri));
                result = dict_of({{"contents", new_list({c})}});
            } catch (error& e) {
                err = rpc_error(-32603, "Error creating resource from template " + uri, dict_of({{"uri", new_str(uri)}}));
                fprintf(stderr, "ERROR    reading resource %s: %s: %s\n", uri.c_str(), tname(e.value).c_str(), str_(e.value).c_str());
            }
        }
    } else if (method == "completion/complete") {
        result = dict_of({{"completion", dict_of({{"hasMore", var(false)}, {"values", new_list({})}})}});
    } else if (method == "logging/setLevel") {
        result = new_dict_obj();
    } else {
        err = rpc_error(-32601, "Method not found", new_str(method));
    }
    if (notification) return None;
    var r = new_dict_obj();
    r.setitem("jsonrpc"_s, "2.0"_s);
    r.setitem("id"_s, id);
    if (!err.is_unbound()) r.setitem("error"_s, err);
    else r.setitem("result"_s, result);
    return r;
}

// run("stdio"): one message per line in, one per line out. File 1 is pointed at stderr while serving (as mcp
// does) so nothing a tool prints reaches the protocol; the protocol keeps the original stdout.
static void run_stdio(Server* s) {
    HANDLE in = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE out;
    DuplicateHandle(GetCurrentProcess(), GetStdHandle(STD_OUTPUT_HANDLE), GetCurrentProcess(), &out, 0, FALSE, DUPLICATE_SAME_ACCESS);
    fflush(stdout);
    SetStdHandle(STD_OUTPUT_HANDLE, GetStdHandle(STD_ERROR_HANDLE));
    _dup2(_fileno(stderr), _fileno(stdout));
    var json = import("json");
    std::string buf;
    char chunk[65536];
    // the protocol's stdout: a line written whole (calls answer from their own threads)
    struct Wire {
        HANDLE out;
        std::mutex mu;
    };
    auto wire = std::make_shared<Wire>();
    wire->out = out;
    auto write_line = [wire](const std::string& line) {
        std::string l = line + "\n";
        DWORD n;
        gil_release g;
        std::lock_guard<std::mutex> lock(wire->mu);
        WriteFile(wire->out, l.data(), (DWORD)l.size(), &n, nullptr);
        FlushFileBuffers(wire->out);
    };
    // tools/call runs on a thread of its own, as mcp runs a tool in a worker thread: a long tool does not
    // hold back the requests after it (the others are answered in order, on this thread)
    var thread_cls = import("threading").getattr("Thread");
    std::vector<var> calls;
    auto answer = [s, write_line](const var& m) {
        var r;
        try {
            r = dispatch(s, m);
        } catch (error& e) {
            r = dict_of({{"jsonrpc", "2.0"_s}, {"id", m.call("get", "id"_s)}, {"error", rpc_error(-32603, str_(e.value))}});
        }
        if (!r.is_none()) write_line(to_json(r));
    };
    for (;;) {
        size_t nlp = buf.find('\n');
        if (nlp == std::string::npos) {
            DWORD n = 0;
            BOOL ok;
            {
                gil_release g;
                ok = ReadFile(in, chunk, sizeof chunk, &n, nullptr);
            }
            if (!ok || n == 0) break;
            buf.append(chunk, n);
            continue;
        }
        std::string line = buf.substr(0, nlp);
        buf.erase(0, nlp + 1);
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.find_first_not_of(" \t") == std::string::npos) continue;
        var msg;
        try {
            msg = json.call("loads", new_str(line));
        } catch (error&) {
            write_line(to_json(dict_of({{"jsonrpc", "2.0"_s}, {"id", None}, {"error", rpc_error(-32700, "Parse error")}})));
            continue;
        }
        std::vector<var> batch = is_list(msg) ? collect(msg) : std::vector<var>{msg};
        for (auto& m : batch) {
            if (!isinstance_(m, T_dict)) continue;
            var method = m.call("get", "method"_s);
            if (is_str(method) && str_utf8(method) == "tools/call") {
                calls.erase(std::remove_if(calls.begin(), calls.end(), [](const var& t) { return !t.call("is_alive").truthy(); }),
                            calls.end());
                CallArgs a;
                a.kw.push_back({"target"_s, new_native("tools/call", [answer, m](const CallArgs&) -> var {
                                    answer(m);
                                    return None;
                                })});
                a.kw.push_back({"daemon"_s, var(true)});
                var t = call_obj(thread_cls, a);
                t.call("start");
                calls.push_back(t);
            } else answer(m);
        }
    }
    for (auto& t : calls) t.call("join");   // (the calls still running answer before the server ends)
    CloseHandle(out);
}

// ------------------------------------------------------------------------------------------ registration
static void add_tool(Server* s, const var& fn, const var& name, const var& description, const var& annotations) {
    Server::Tool t;
    t.fn = fn;
    t.fn_name = str_utf8(fn.getattr("__name__"));
    t.name = name.is_none() ? t.fn_name : str_utf8(name);
    t.description = description.is_none() ? doc_of(fn) : str_utf8(description);
    t.params = params_of(fn);
    t.schema = input_schema(t.params, t.fn_name);
    t.annotations = annotations;
    for (auto& x : s->tools)
        if (x.name == t.name) return;   // (mcp warns and keeps the first)
    s->tools.push_back(t);
}

static var make_server_class() {
    var ns = new_dict();
    ns.setitem("__module__"_s, "mcp.server.mcpserver"_s);
    ns.setitem("__init__"_s, def("__init__", "self, name=, instructions=, *, title=, version=, **settings", {None, None, None, None},
                                 [](var self, var name, var instructions, var title, var version, var settings) -> var {
        (void)settings;
        auto* s = new Server();
        s->name = name.is_none() ? "mcp-server" : str_utf8(name);
        s->instructions = instructions.is_none() ? "" : str_utf8(instructions);
        s->title = title.is_none() ? "" : str_utf8(title);
        s->version = version.is_none() ? "" : str_utf8(version);
        if (s->version.empty()) s->version = "2.3.0";   // (mcp's own version when the server gives none)
        self.setattr("_pyrt_server", var((int64_t)(intptr_t)s));
        self.setattr("name", new_str(s->name));
        self.setattr("instructions", instructions);
        return None;
    }));
    ns.setitem("tool"_s, def("tool", "self, name=, title=, description=, annotations=, **rest", {None, None, None, None},
                             [](var self, var name, var title, var description, var annotations, var rest) -> var {
        (void)title;
        (void)rest;
        if (name.is_ref() && type_of(name)->name != "str" && callable(name).truthy())
            raise(E_TypeError, "The @tool decorator was used incorrectly. Did you forget to call it? Use @tool() instead of @tool");
        Server* s = S(self);
        return natf("decorator", [s, name, description, annotations](const CallArgs& a) {
            add_tool(s, a.pos.at(0), name, description, annotations);
            return a.pos[0];
        });
    }));
    ns.setitem("add_tool"_s, def("add_tool", "self, fn, name=, title=, description=, annotations=, **rest", {None, None, None, None},
                                 [](var self, var fn, var name, var title, var description, var annotations, var rest) -> var {
        (void)title;
        (void)rest;
        add_tool(S(self), fn, name, description, annotations);
        return None;
    }));
    ns.setitem("prompt"_s, def("prompt", "self, name=, title=, description=, **rest", {None, None, None},
                               [](var self, var name, var title, var description, var rest) -> var {
        (void)title;
        (void)rest;
        Server* s = S(self);
        return natf("decorator", [s, name, description](const CallArgs& a) {
            Server::Prompt p;
            p.fn = a.pos.at(0);
            p.name = name.is_none() ? str_utf8(p.fn.getattr("__name__")) : str_utf8(name);
            p.description = description.is_none() ? doc_of(p.fn) : str_utf8(description);
            p.params = params_of(p.fn);
            s->prompts.push_back(p);
            return a.pos[0];
        });
    }));
    ns.setitem("resource"_s, def("resource", "self, uri, *, name=, title=, description=, mime_type=, **rest", {None, None, None, None},
                                 [](var self, var uri, var name, var title, var description, var mime, var rest) -> var {
        (void)title;
        (void)rest;
        Server* s = S(self);
        return natf("decorator", [s, uri, name, description, mime](const CallArgs& a) {
            Server::Template t;
            t.fn = a.pos.at(0);
            t.uri = str_utf8(uri);
            t.name = name.is_none() ? str_utf8(t.fn.getattr("__name__")) : str_utf8(name);
            t.description = description.is_none() ? doc_of(t.fn) : str_utf8(description);
            t.mime = mime.is_none() ? "text/plain" : str_utf8(mime);
            for (size_t i = t.uri.find('{'); i != std::string::npos; i = t.uri.find('{', i + 1))
                t.vars.push_back(t.uri.substr(i + 1, t.uri.find('}', i) - i - 1));
            s->templates.push_back(t);
            return a.pos[0];
        });
    }));
    ns.setitem("run"_s, def("run", "self, transport=, **kw", {"stdio"_s}, [](var self, var transport, var kw) -> var {
        (void)kw;
        std::string tr = str_utf8(transport);
        if (tr != "stdio") raise(E_NotImplementedError, "MCPServer.run(" + repr_(transport) + "): only stdio is in the C++ MCP layer yet");
        run_stdio(S(self));
        return None;
    }));
    ns.setitem("streamable_http_app"_s, def("streamable_http_app", "self, **kw", {}, [](var, var) -> var {
        raise(E_NotImplementedError, "streamable HTTP (mcp --http) is not in the C++ MCP layer yet: stdio only");
    }));
    // (the in-process client's way in: one JSON-RPC message, its answer)
    ns.setitem("_pyrt_dispatch"_s, def("_pyrt_dispatch", "self, message", {}, [](var self, var msg) -> var { return dispatch(S(self), msg); }));
    return make_class("MCPServer", "MCPServer", {}, ns, {}, "mcp.server.mcpserver");
}

// ------------------------------------------------------------------------------------------ the client
// Client(server): async with ... as c; await c.call_tool(name, args) / list_tools() / list_prompts() /
// list_resource_templates() / read_resource(uri) / get_prompt(name, args) / ping(): each a JSON-RPC request
// through the server's dispatch, its result as objects with the model's (snake_case) attribute names
static std::string snake(const std::string& k) {
    std::string o;
    for (char c : k) {
        if (isupper((unsigned char)c)) { o += '_'; o += (char)tolower((unsigned char)c); }
        else o += c;
    }
    return o;
}
static var g_ns_cls;
static var objectify(const var& v, bool raw_dicts = false) {
    if (isinstance_(v, T_dict) && !raw_dicts) {
        var o = call0(g_ns_cls);
        for (var k : each(v)) {
            std::string key = str_utf8(k);
            bool keep = key == "inputSchema" || key == "outputSchema" || key == "structuredContent" || key == "_meta";
            o.setattr(snake(key).c_str(), objectify(v.getitem(k), keep));
        }
        return o;
    }
    if (is_list(v) && !raw_dicts) {
        std::vector<var> xs;
        for (var x : each(v)) xs.push_back(objectify(x));
        return new_list(xs);
    }
    return v;
}


void init_mcp() {
    register_native_module("mcp_types", [](const var& m) {
        var ns = new_dict();
        ns.setitem("__module__"_s, "mcp_types"_s);
        ns.setitem("__init__"_s, def("__init__", "self, *, title=, read_only_hint=, destructive_hint=, idempotent_hint=, open_world_hint=, **rest",
                                     {None, None, None, None, None}, [](var self, var title, var ro, var de, var id, var ow, var rest) -> var {
            (void)rest;
            self.setattr("title", title);
            self.setattr("read_only_hint", ro);
            self.setattr("destructive_hint", de);
            self.setattr("idempotent_hint", id);
            self.setattr("open_world_hint", ow);
            return None;
        }));
        g_annotations_cls = make_class("ToolAnnotations", "ToolAnnotations", {}, ns, {}, "mcp_types");
        mod_set(m, "ToolAnnotations", g_annotations_cls);
    });
    register_native_module("mcp.server.mcpserver.exceptions", [](const var& m) {
        var ns = new_dict();
        ns.setitem("__module__"_s, "mcp.server.mcpserver.exceptions"_s);
        var base = make_class("MCPServerError", "MCPServerError", {t::Exception}, ns, {}, "mcp.server.mcpserver.exceptions");
        var ns2 = new_dict();
        ns2.setitem("__module__"_s, "mcp.server.mcpserver.exceptions"_s);
        g_tool_error = make_class("ToolError", "ToolError", {base}, ns2, {}, "mcp.server.mcpserver.exceptions");
        mod_set(m, "MCPServerError", base);
        mod_set(m, "ToolError", g_tool_error);
        var ns3 = new_dict();
        ns3.setitem("__module__"_s, "mcp.server.mcpserver.exceptions"_s);
        mod_set(m, "ResourceError", make_class("ResourceError", "ResourceError", {base}, ns3, {}, "mcp.server.mcpserver.exceptions"));
    });
    register_native_module("mcp.server.mcpserver", [](const var& m) {
        import("mcp.server.mcpserver.exceptions");
        mod_set(m, "MCPServer", make_server_class());
        // Image(path=None, data=None, format=None): a picture a tool returns
        var ns = new_dict();
        ns.setitem("__module__"_s, "mcp.server.mcpserver"_s);
        ns.setitem("__init__"_s, def("__init__", "self, path=, data=, format=", {None, None, None}, [](var self, var path, var data, var format) -> var {
            if (path.is_none() == data.is_none()) raise(E_ValueError, "Either path or data must be provided, but not both");
            self.setattr("path", path);
            self.setattr("data", data.is_none() ? call1(import("builtins").getattr("open"), path).call("read") : data);
            self.setattr("_format", format);
            return None;
        }));
        ns.setitem("_mime_type"_s, def("_mime_type", "self", {}, [](var self) -> var {
            var f = self.getattr("_format");
            if (!f.is_none()) return new_str("image/" + str_utf8(f.call("lower")));
            var p = self.getattr("path");
            if (!p.is_none()) {
                std::string s = str_utf8(call1(t::str, p));
                std::string ext = s.substr(s.rfind('.') == std::string::npos ? s.size() : s.rfind('.'));
                for (auto& c : ext) c = (char)tolower((unsigned char)c);
                if (ext == ".png") return "image/png"_s;
                if (ext == ".jpg" || ext == ".jpeg") return "image/jpeg"_s;
                if (ext == ".gif") return "image/gif"_s;
                if (ext == ".webp") return "image/webp"_s;
            }
            return "image/png"_s;
        }));
        g_image_cls = make_class("Image", "Image", {}, ns, {}, "mcp.server.mcpserver");
        mod_set(m, "Image", g_image_cls);
    });
    register_native_module("mcp.server", [](const var&) {});
    register_native_module("mcp.server.transport_security", [](const var& m) {
        var ns = new_dict();
        ns.setitem("__module__"_s, "mcp.server.transport_security"_s);
        ns.setitem("__init__"_s, def("__init__", "self, **kw", {}, [](var self, var kw) -> var {
            for (var k : each(kw)) self.setattr(str_utf8(k).c_str(), kw.getitem(k));
            return None;
        }));
        mod_set(m, "TransportSecuritySettings", make_class("TransportSecuritySettings", "TransportSecuritySettings", {}, ns, {}, "mcp.server.transport_security"));
    });
    register_native_module("mcp", [](const var& m) {
        g_ns_cls = import("types").getattr("SimpleNamespace");
        var ns = new_dict();
        ns.setitem("__module__"_s, "mcp"_s);
        ns.setitem("__init__"_s, def("__init__", "self, server, **kw", {}, [](var self, var server, var kw) -> var {
            (void)kw;
            self.setattr("_server", server);
            self.setattr("_next", var(1));
            return None;
        }));
        // a request through the server: its result as objects (an error answer raised as mcp's MCPError would be)
        auto request = [](const var& self, const char* method, const var& params) -> var {
            var id = self.getattr("_next");
            self.setattr("_next", id + 1);
            var msg = dict_of({{"jsonrpc", "2.0"_s}, {"id", id}, {"method", new_str(method)}, {"params", params}});
            var r = self.getattr("_server").call("_pyrt_dispatch", msg);
            if (r.is_none()) return None;
            var e = r.call("get", "error"_s);
            if (!e.is_none()) raise(E_RuntimeError, str_utf8(e.getitem("message"_s)));
            return objectify(r.getitem("result"_s));
        };
        auto coro = [](const char* name, std::function<var()> f) {
            return def_coroutine(name, "", {}, [f](yielder& y) -> var { (void)y; return f(); })();
        };
        ns.setitem("__aenter__"_s, def("__aenter__", "self", {}, [request, coro](var self) -> var {
            return coro("__aenter__", [self, request]() {
                request(self, "initialize", dict_of({{"protocolVersion", "2025-11-25"_s}, {"capabilities", new_dict_obj()},
                                                    {"clientInfo", dict_of({{"name", "mcp"_s}, {"version", "2.3.0"_s}})}}));
                return self;
            });
        }));
        ns.setitem("__aexit__"_s, def("__aexit__", "self, *exc", {}, [coro](var self, var exc) -> var {
            (void)self;
            (void)exc;
            return coro("__aexit__", []() { return var(false); });
        }));
        ns.setitem("call_tool"_s, def("call_tool", "self, name, arguments=, **kw", {None}, [request, coro](var self, var name, var arguments, var kw) -> var {
            (void)kw;
            return coro("call_tool", [self, name, arguments, request]() {
                return request(self, "tools/call", dict_of({{"name", name}, {"arguments", arguments.is_none() ? new_dict_obj() : arguments}}));
            });
        }));
        ns.setitem("list_tools"_s, def("list_tools", "self, **kw", {}, [request, coro](var self, var) -> var {
            return coro("list_tools", [self, request]() { return request(self, "tools/list", new_dict_obj()); });
        }));
        ns.setitem("list_prompts"_s, def("list_prompts", "self, **kw", {}, [request, coro](var self, var) -> var {
            return coro("list_prompts", [self, request]() { return request(self, "prompts/list", new_dict_obj()); });
        }));
        ns.setitem("get_prompt"_s, def("get_prompt", "self, name, arguments=, **kw", {None}, [request, coro](var self, var name, var arguments, var) -> var {
            return coro("get_prompt", [self, name, arguments, request]() {
                return request(self, "prompts/get", dict_of({{"name", name}, {"arguments", arguments.is_none() ? new_dict_obj() : arguments}}));
            });
        }));
        ns.setitem("list_resources"_s, def("list_resources", "self, **kw", {}, [request, coro](var self, var) -> var {
            return coro("list_resources", [self, request]() { return request(self, "resources/list", new_dict_obj()); });
        }));
        ns.setitem("list_resource_templates"_s, def("list_resource_templates", "self, **kw", {}, [request, coro](var self, var) -> var {
            return coro("list_resource_templates", [self, request]() { return request(self, "resources/templates/list", new_dict_obj()); });
        }));
        ns.setitem("read_resource"_s, def("read_resource", "self, uri, **kw", {}, [request, coro](var self, var uri, var) -> var {
            return coro("read_resource", [self, uri, request]() { return request(self, "resources/read", dict_of({{"uri", call1(t::str, uri)}})); });
        }));
        ns.setitem("ping"_s, def("ping", "self, **kw", {}, [request, coro](var self, var) -> var {
            return coro("ping", [self, request]() { return request(self, "ping", new_dict_obj()); });
        }));
        mod_set(m, "Client", make_class("Client", "Client", {}, ns, {}, "mcp"));
        mod_set(m, "__version__", "2.3.0"_s);
    });
}

}  // namespace py
