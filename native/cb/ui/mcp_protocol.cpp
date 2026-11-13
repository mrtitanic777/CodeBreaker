// cb-unity: alone
// cb/ui/mcp_protocol.cpp -- the MCP wire: the tool table and its schemas, the checks of a call's arguments, JSON-RPC, the stdio transport.
#include <fcntl.h>
#include <io.h>

#include <algorithm>
#include <array>
#include <atomic>
#include <cstdio>
#include <format>
#include <iostream>
#include <mutex>
#include <nlohmann/json.hpp>
#include <random>
#include <set>
#include <thread>

#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "cb/ui/mcp_impl.hpp"

namespace cb::ui::mcp {

namespace sfs = std::filesystem;

const std::string_view kInstructions =
    R"(CodeBreaker reconstructs readable source from binaries (Windows 16/32-bit, DOS, VxD
drivers, Xbox, Xbox 360, 8/16-bit consoles, XNA, Java) and checks that the source behaves like the
original by running both in an emulator. Start with `triage` on a module (everything below at a
glance), `describe` on a file or `modules` for the corpus; `inspect` for headers/imports/exports;
`functions` and `function` for what the store holds (each function's reconstructed C and its verdict:
verified, weak, unverified, not-judged, no-compile, no-source); `decompile_function` for one function's
C now; `disasm` for its instructions, `xrefs` for who calls it and what it calls, `hex_dump` by address.
What was made lives in `projects`/`project` (Converted/), `source_tree` (the product's source tree)
and `reach_results`; `read_file` reads any of it. `compare_versions` matches two builds function by
function, `diff_function` shows one pair side by side. Anything long (decompile, measure, reach
--judge, pe-source, recompile-project) goes through `run_command`, then `job` to follow it.
A module is a corpus name (lz32.dll) or a path; an address is hex, or a function's name.)";

namespace {

constexpr std::string_view kPromptDescription = "Take a binary apart: what it is, what is in it, where its reconstruction stands";
constexpr std::string_view kResourceDescription = "One function's reconstructed source, as the store holds it";
constexpr std::string_view kPydanticDocs = "https://errors.pydantic.dev/2.12/v/";   // the version of the checks' wording

// ---- Python's json.dumps, for what the wire shows and what lengths are measured in ---------------------------------------------------
void escape_ascii(std::string_view s, std::string& out) {   // ensure_ascii: every character past 0x7E is \uXXXX (a pair past the plane 0)
    out += '"';
    for (std::size_t i = 0; i < s.size();) {
        const unsigned char c = static_cast<unsigned char>(s[i]);
        if (c < 0x80) {
            switch (c) {
                case '"': out += "\\\""; break;
                case '\\': out += "\\\\"; break;
                case '\n': out += "\\n"; break;
                case '\r': out += "\\r"; break;
                case '\t': out += "\\t"; break;
                case '\b': out += "\\b"; break;
                case '\f': out += "\\f"; break;
                default:
                    if (c < 0x20) out += std::format("\\u{:04x}", c);
                    else out += static_cast<char>(c);
            }
            ++i;
            continue;
        }
        std::uint32_t cp = 0xFFFD;
        std::size_t len = 1;
        if ((c >> 5) == 6 && i + 1 < s.size()) {
            cp = ((c & 0x1Fu) << 6) | (static_cast<unsigned char>(s[i + 1]) & 0x3Fu);
            len = 2;
        } else if ((c >> 4) == 14 && i + 2 < s.size()) {
            cp = ((c & 0x0Fu) << 12) | ((static_cast<unsigned char>(s[i + 1]) & 0x3Fu) << 6) | (static_cast<unsigned char>(s[i + 2]) & 0x3Fu);
            len = 3;
        } else if ((c >> 3) == 30 && i + 3 < s.size()) {
            cp = ((c & 0x07u) << 18) | ((static_cast<unsigned char>(s[i + 1]) & 0x3Fu) << 12) | ((static_cast<unsigned char>(s[i + 2]) & 0x3Fu) << 6) |
                 (static_cast<unsigned char>(s[i + 3]) & 0x3Fu);
            len = 4;
        }
        if (cp >= 0x10000) {
            cp -= 0x10000;
            out += std::format("\\u{:04x}\\u{:04x}", 0xD800 + (cp >> 10), 0xDC00 + (cp & 0x3FF));
        } else {
            out += std::format("\\u{:04x}", cp);
        }
        i += len;
    }
    out += '"';
}

void py_dumps(const Json& v, std::string& out) {   // json.dumps(value): ", " and ": " separators, ensure_ascii
    if (v.is_object()) {
        out += '{';
        bool first = true;
        for (const auto& [k, x] : v.items()) {
            if (!first) out += ", ";
            first = false;
            escape_ascii(k, out);
            out += ": ";
            py_dumps(x, out);
        }
        out += '}';
    } else if (v.is_array()) {
        out += '[';
        bool first = true;
        for (const Json& x : v) {
            if (!first) out += ", ";
            first = false;
            py_dumps(x, out);
        }
        out += ']';
    } else if (v.is_string()) {
        escape_ascii(v.get<std::string>(), out);
    } else {
        out += v.dump();
    }
}

// Python's repr of the JSON-derived value, for the checks' messages.
std::string py_repr_of(const Json& v) {
    if (v.is_null()) return "None";
    if (v.is_boolean()) return v.get<bool>() ? "True" : "False";
    if (v.is_string()) {
        const std::string s = v.get<std::string>();
        const bool single = s.find('\'') == std::string::npos || s.find('"') != std::string::npos;
        std::string out(1, single ? '\'' : '"');
        for (const char c : s) {
            if (c == '\\') out += "\\\\";
            else if (c == '\n') out += "\\n";
            else if (c == '\r') out += "\\r";
            else if (c == '\t') out += "\\t";
            else if (c == '\'' && single) out += "\\'";
            else out += c;
        }
        return out + (single ? "'" : "\"");
    }
    if (v.is_array()) {
        std::string out = "[";
        for (std::size_t i = 0; i < v.size(); ++i) out += (i ? ", " : "") + py_repr_of(v[i]);
        return out + "]";
    }
    if (v.is_object()) {
        std::string out = "{";
        bool first = true;
        for (const auto& [k, x] : v.items()) {
            out += (first ? "" : ", ") + py_repr_of(Json(k)) + ": " + py_repr_of(x);
            first = false;
        }
        return out + "}";
    }
    return v.dump();
}

std::string py_type_of(const Json& v) {
    if (v.is_null()) return "NoneType";
    if (v.is_boolean()) return "bool";
    if (v.is_number_integer()) return "int";
    if (v.is_number_float()) return "float";
    if (v.is_string()) return "str";
    if (v.is_array()) return "list";
    return "dict";
}

std::string clipped_repr(const Json& v) {   // pydantic shows at most 50 characters of a text, its start and its end
    const std::string r = py_repr_of(v);
    return r.size() > 50 && v.is_string() ? r.substr(0, 25) + "..." + r.substr(r.size() - 24) : r;
}

// ---- a call's arguments, checked the way pydantic checks them (lax mode) -----------------------------------------------------------
struct Problem {
    std::string where, message, type;
    const Json* value;   // none: the whole input
};

// Every parameter of the tool, defaults filled in, values as lax mode makes them; ToolFailure carries pydantic's own words when they are wrong.
class ToolFailure : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(ToolFailure)
};

Json bind_arguments(const ToolSpec& spec, const Json& raw) {
    const Json given = raw.is_object() ? raw : Json::object();
    Json out = Json::object();
    std::vector<Problem> bad;
    for (const Param& p : spec.params) {
        const std::string name(p.name);
        const auto it = given.find(name);
        if (it == given.end()) {
            if (!p.default_json) {
                bad.push_back({name, "Field required", "missing", nullptr});
                continue;
            }
            out[name] = Json::parse(std::string(*p.default_json));
            continue;
        }
        const Json& v = *it;
        switch (p.type) {
            case ParamType::String:
                if (!v.is_string()) bad.push_back({name, "Input should be a valid string", "string_type", &v});
                else out[name] = v;
                break;
            case ParamType::Integer:
                if (v.is_number_integer()) out[name] = v;
                else if (v.is_boolean()) out[name] = v.get<bool>() ? 1 : 0;
                else if (v.is_number_float() && v.get<double>() == static_cast<double>(static_cast<std::int64_t>(v.get<double>()))) out[name] = static_cast<std::int64_t>(v.get<double>());
                else if (v.is_number_float()) bad.push_back({name, "Input should be a valid integer, got a number with a fractional part", "int_from_float", &v});
                else if (v.is_string()) {
                    const std::optional<std::int64_t> n = text::parse_int<std::int64_t>(text::trim(v.get<std::string>()), 10);
                    if (n) out[name] = *n;
                    else bad.push_back({name, "Input should be a valid integer, unable to parse string as an integer", "int_parsing", &v});
                } else bad.push_back({name, "Input should be a valid integer", "int_type", &v});
                break;
            case ParamType::Boolean: {
                static const std::set<std::string> yes = {"true", "1", "yes", "on", "t", "y"}, no = {"false", "0", "no", "off", "f", "n"};
                const std::string low = v.is_string() ? text::lower(v.get<std::string>()) : std::string();
                if (v.is_boolean()) out[name] = v;
                else if (v.is_number_integer() && (v.get<std::int64_t>() == 0 || v.get<std::int64_t>() == 1)) out[name] = v.get<std::int64_t>() == 1;
                else if (v.is_string() && yes.contains(low)) out[name] = true;
                else if (v.is_string() && no.contains(low)) out[name] = false;
                else if (v.is_string() || v.is_number_integer()) bad.push_back({name, "Input should be a valid boolean, unable to interpret input", "bool_parsing", &v});
                else bad.push_back({name, "Input should be a valid boolean", "bool_type", &v});
                break;
            }
            case ParamType::OptionalStringList:
            case ParamType::StringList: {
                if (p.type == ParamType::OptionalStringList && v.is_null()) {
                    out[name] = v;
                    break;
                }
                if (!v.is_array()) {
                    bad.push_back({name, "Input should be a valid list", "list_type", &v});
                    break;
                }
                bool ok = true;
                for (std::size_t k = 0; k < v.size(); ++k) {
                    if (!v[k].is_string()) {
                        bad.push_back({name + "." + std::to_string(k), "Input should be a valid string", "string_type", &v[k]});
                        ok = false;
                    }
                }
                if (ok) out[name] = v;
                break;
            }
        }
    }
    if (!bad.empty()) {
        std::string m = std::format("{} validation error{} for {}Arguments", bad.size(), bad.size() > 1 ? "s" : "", spec.function);
        for (const Problem& b : bad) {
            const Json& val = b.value ? *b.value : given;
            m += std::format("\n{}\n  {} [type={}, input_value={}, input_type={}]\n    For further information visit {}{}", b.where, b.message, b.type, clipped_repr(val),
                             py_type_of(val), kPydanticDocs, b.type);
        }
        throw ToolFailure(m);
    }
    return out;
}

// ---- the tools' listing --------------------------------------------------------------------------------------------------------------
Json annotations_of(const ToolSpec& s) {
    Json a = Json::object();
    if (s.kind == Kind::Reading) {
        a["readOnlyHint"] = true;
    } else if (s.function == "run_command") {
        a["destructiveHint"] = true;
        a["openWorldHint"] = false;
        a["readOnlyHint"] = false;
    } else if (s.function == "stop_job") {
        a["destructiveHint"] = true;
        a["readOnlyHint"] = false;
    } else {
        a["destructiveHint"] = false;
        a["idempotentHint"] = true;
        a["readOnlyHint"] = false;
    }
    return a;
}

std::string title_of(std::string_view name) {   // "min_len" as "Min Len"
    std::string title;
    bool up = true;
    for (const char c : name) {
        if (c == '_') {
            title += ' ';
            up = true;
        } else {
            title += up ? static_cast<char>(std::toupper(static_cast<unsigned char>(c))) : c;
            up = false;
        }
    }
    return title;
}

Json listing_of(const ToolSpec& s) {
    Json props = Json::object(), required = Json::array();
    for (const Param& p : s.params) {
        Json one = Json::object();
        const Json items = Json{{"type", "string"}};
        if (p.type == ParamType::OptionalStringList) one["anyOf"] = Json::array({Json{{"items", items}, {"type", "array"}}, Json{{"type", "null"}}});
        if (p.default_json) one["default"] = Json::parse(std::string(*p.default_json));
        if (p.type == ParamType::StringList) one["items"] = items;
        one["title"] = title_of(p.name);
        switch (p.type) {
            case ParamType::String: one["type"] = "string"; break;
            case ParamType::Integer: one["type"] = "integer"; break;
            case ParamType::Boolean: one["type"] = "boolean"; break;
            case ParamType::StringList: one["type"] = "array"; break;
            case ParamType::OptionalStringList: break;
        }
        props[std::string(p.name)] = std::move(one);
        if (!p.default_json) required.push_back(std::string(p.name));
    }
    Json schema = Json::object();
    schema["properties"] = std::move(props);
    if (!required.empty()) schema["required"] = std::move(required);
    schema["type"] = "object";
    schema["title"] = std::string(s.function) + "Arguments";
    Json t = Json::object();
    t["annotations"] = annotations_of(s);
    t["description"] = std::string(s.doc);
    t["inputSchema"] = std::move(schema);
    t["name"] = std::string(s.name);
    return t;
}

Json response(const Json& id, const char* field, Json body) {
    Json r = Json::object();
    r["jsonrpc"] = "2.0";
    r["id"] = id;
    r[field] = std::move(body);
    return r;
}
Json error_response(const Json& id, std::int64_t code, const std::string& message, const Json* data = nullptr) {
    Json e = Json::object();
    e["code"] = code;
    e["message"] = message;
    if (data) e["data"] = *data;
    return response(id, "error", std::move(e));
}

Json content_text(const std::string& text) { return Json{{"text", text}, {"type", "text"}}; }
Json tool_result(Json content, bool is_error) {
    Json r = Json::object();
    r["content"] = Json::array({std::move(content)});
    r["isError"] = is_error;
    return r;
}

std::string base64(const Bytes& raw) {
    static constexpr std::string_view alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string out;
    for (std::size_t i = 0; i < raw.size(); i += 3) {
        const unsigned n = (static_cast<unsigned>(raw[i]) << 16) | (i + 1 < raw.size() ? static_cast<unsigned>(raw[i + 1]) << 8 : 0) | (i + 2 < raw.size() ? raw[i + 2] : 0u);
        out += alphabet[(n >> 18) & 63];
        out += alphabet[(n >> 12) & 63];
        out += i + 1 < raw.size() ? alphabet[(n >> 6) & 63] : '=';
        out += i + 2 < raw.size() ? alphabet[n & 63] : '=';
    }
    return out;
}

const std::vector<ToolSpec>& table() {
    static const std::vector<ToolSpec> specs = {
#include "cb/ui/mcp_specs.inc"
    };
    return specs;
}

}  // namespace

std::span<const ToolSpec> tool_specs() { return table(); }

std::size_t compact_length(const Json& value) {
    std::string text;
    py_dumps(value, text);
    return text.size();   // (bytes: the text is ASCII)
}

std::string answer_text(const Json& value) { return value.dump(2, ' ', false, Json::error_handler_t::replace); }

// ---- the server ---------------------------------------------------------------------------------------------------------------------

Server::Server(service::Service& service, service::ServiceLink& link, Services services, Options options)
    : impl_(std::make_unique<Impl>(service, link, std::move(services), options)) {
    // The tools are answered by the service: `mcp.<function>`, the same answer for every client.
    Impl* impl = impl_.get();
    for (const ToolSpec& spec : table()) {
        const std::string function(spec.function);
        service.add_call("mcp." + function, [impl, function, &spec](const Json& args, std::string_view) -> Json {
            // (a client of the wire sends only the arguments it was given, as the Python's does: the rest are the tool's defaults)
            const ToolResult r = impl->call(function, bind_arguments(spec, args));
            if (r.image) throw UsageError("a picture does not go through JSON");
            return *r.document;
        });
    }
}

Server::~Server() = default;

Json Server::call_tool(std::string_view function, const Json& arguments) {
    const ToolResult r = impl_->call(function, arguments);
    if (r.image) throw UsageError("a picture does not go through JSON");
    return *r.document;
}

Json Server::handle(const Json& msg, bool* initialized) {
    Impl& I = *impl_;
    // A line that is not a JSON-RPC request or notification (the library drops it with a log line and answers nothing): not an object,
    // "jsonrpc" not "2.0", no string "method", an "id" that is not an int or a str, "params" that is not an object.
    if (!msg.is_object()) return nullptr;
    const auto ver = msg.find("jsonrpc");
    if (ver == msg.end() || !ver->is_string() || ver->get<std::string>() != "2.0") return nullptr;
    const auto idp = msg.find("id");
    const auto method = msg.find("method");
    if (method == msg.end() || !method->is_string()) return nullptr;
    if (idp != msg.end() && !(idp->is_number_integer() || idp->is_string())) return nullptr;
    const auto params_p = msg.find("params");
    if (params_p != msg.end() && !params_p->is_null() && !params_p->is_object()) return nullptr;
    const std::string m = method->get<std::string>();
    if (idp == msg.end()) return nullptr;   // a notification: no answer
    const Json id = *idp;
    const bool has_params = params_p != msg.end() && params_p->is_object();
    const Json params = has_params ? *params_p : Json::object();
    auto invalid = [&] {
        const Json data = "";
        return error_response(id, -32602, "Invalid request parameters", &data);
    };
    auto member = [&](const char* k) -> const Json* {
        const auto it = params.find(k);
        return it == params.end() ? nullptr : &*it;
    };
    // before the initialize request only that and ping are answered
    if (initialized && !*initialized && m != "initialize" && m != "ping") return invalid();
    if (m == "initialize") {
        const Json* pv = member("protocolVersion");
        const Json* caps = member("capabilities");
        const Json* ci = member("clientInfo");
        if (!has_params || !pv || !pv->is_string() || !caps || !caps->is_object() || !ci || !ci->is_object() || !ci->contains("name") || !ci->contains("version") ||
            !(*ci)["name"].is_string() || !(*ci)["version"].is_string())
            return invalid();
        if (initialized) *initialized = true;
        static const std::array<std::string_view, 4> known = {"2024-11-05", "2025-03-26", "2025-06-18", "2025-11-25"};
        const std::string want = pv->get<std::string>();
        const std::string use = std::find(known.begin(), known.end(), want) != known.end() ? want : std::string(known.back());
        auto flag = [](bool subscribe) {
            Json o = Json::object();
            o["listChanged"] = false;
            if (subscribe) o["subscribe"] = false;
            return o;
        };
        Json capabilities = Json::object();
        capabilities["prompts"] = flag(false);
        capabilities["resources"] = flag(true);
        capabilities["tools"] = flag(false);
        Json info = Json::object();
        info["name"] = "codebreaker";
        info["title"] = "CodeBreaker";
        std::string version;
        try {
            if (I.s.version) version = I.s.version();
        } catch (const std::exception&) {   // a version that cannot be read is empty
        }
        info["version"] = version;
        Json r = Json::object();
        r["capabilities"] = std::move(capabilities);
        r["instructions"] = std::string(kInstructions);
        r["protocolVersion"] = use;
        r["serverInfo"] = std::move(info);
        return response(id, "result", std::move(r));
    }
    if (m == "ping") return response(id, "result", Json::object());
    auto offered = [&](const ToolSpec& s) { return I.options.writes || s.kind == Kind::Reading; };
    if (m == "tools/list") {
        Json tools = Json::array();
        for (const ToolSpec& s : table())
            if (offered(s)) tools.push_back(listing_of(s));
        return response(id, "result", Json{{"tools", std::move(tools)}});
    }
    if (m == "tools/call") {
        const Json* nm = member("name");
        const Json* argp = member("arguments");
        if (!nm || !nm->is_string() || (argp && !argp->is_null() && !argp->is_object())) return invalid();
        const std::string name = nm->get<std::string>();
        const ToolSpec* spec = nullptr;
        for (const ToolSpec& s : table())
            if (name == s.name && offered(s)) spec = &s;
        if (!spec) return response(id, "result", tool_result(content_text("Unknown tool: " + name), true));
        try {
            const Json bound = bind_arguments(*spec, argp && argp->is_object() ? *argp : Json::object());
            ToolResult v;
            const auto job = bound.find("job_id");
            const bool own_job = job != bound.end() && I.service.jobs().view(job->get<std::string>()).has_value();
            if (spec->function == "resource" || own_job) {   // a picture does not go through JSON; a job this process started itself stays here
                v = I.call(spec->function, bound);
            } else {
                v.document = std::make_shared<Json>(I.link.call("mcp." + std::string(spec->function), bound));
            }
            if (v.image) {
                Json c = Json::object();
                c["data"] = base64(v.image->bytes);
                c["mimeType"] = "image/" + v.image->format;
                c["type"] = "image";
                return response(id, "result", tool_result(std::move(c), false));
            }
            return response(id, "result", tool_result(content_text(answer_text(*v.document)), false));
        } catch (const std::exception& e) {   // the tool's own reports, and any other failure, as the library words them
            return response(id, "result", tool_result(content_text(std::format("Error executing tool {}: {}", name, e.what())), true));
        }
    }
    if (m == "prompts/list") {
        Json p = Json::object();
        p["arguments"] = Json::array({Json{{"name", "path"}, {"required", true}}});
        p["description"] = std::string(kPromptDescription);
        p["name"] = "triage";
        return response(id, "result", Json{{"prompts", Json::array({std::move(p)})}});
    }
    if (m == "prompts/get") {
        const Json* nm = member("name");
        const Json* args = member("arguments");
        if (!nm || !nm->is_string() || (args && !args->is_null() && !args->is_object())) return invalid();
        if (args && args->is_object())
            for (const auto& [k, v] : args->items())
                if (!v.is_string()) return invalid();   // every prompt argument is a string
        const std::string name = nm->get<std::string>();
        if (name != "triage") return error_response(id, 0, "Unknown prompt: " + name);
        const Json* path = args && args->is_object() && args->contains("path") ? &(*args)["path"] : nullptr;
        if (!path) return error_response(id, 0, "Missing required arguments: {'path'}");
        if (args->size() > 1) return error_response(id, 0, "Error rendering prompt triage");   // the function takes `path` only
        const std::string text =
            "Triage " + path->get<std::string>() +
            " with the CodeBreaker tools: start with the `triage` tool (what it is, its imports and exports, the store's counts and each unverified function with its "
            "verdict); if nothing is stored, offer to `run_command` measure on it and follow the job; then for each function to look at, `disasm` and `xrefs` it and say "
            "what most likely went wrong and what the original code was for.";
        Json r = Json::object();
        r["description"] = std::string(kPromptDescription);
        r["messages"] = Json::array({Json{{"content", content_text(text)}, {"role", "user"}}});
        return response(id, "result", std::move(r));
    }
    if (m == "resources/list") return response(id, "result", Json{{"resources", Json::array()}});
    if (m == "resources/templates/list") {
        Json t = Json::object();
        t["description"] = std::string(kResourceDescription);
        t["mimeType"] = "text/x-c";
        t["name"] = "function_source";
        t["uriTemplate"] = "codebreaker://function/{module}/{addr}";
        return response(id, "result", Json{{"resourceTemplates", Json::array({std::move(t)})}});
    }
    if (m == "resources/read") {
        const Json* up = member("uri");
        if (!up || !up->is_string()) return invalid();
        const std::string uri = up->get<std::string>();
        const Json data = Json{{"uri", uri}};
        const std::string prefix = "codebreaker://function/";
        const std::string rest = uri.starts_with(prefix) ? uri.substr(prefix.size()) : "";
        const std::size_t slash = rest.find('/');
        if (!uri.starts_with(prefix) || slash == std::string::npos || rest.find('/', slash + 1) != std::string::npos)
            return error_response(id, -32602, "Unknown resource: " + uri, &data);
        try {
            const Json f = I.link.call("mcp.function", Json{{"module", rest.substr(0, slash)}, {"addr", rest.substr(slash + 1)}});
            const bool has_source = f.is_object() && f.contains("source") && f["source"].is_string() && !f["source"].get<std::string>().empty();
            const std::string text = has_source ? f["source"].get<std::string>() : "/* no source: see the verdict (the `function` tool) */";
            Json c = Json::object();
            c["mimeType"] = "text/x-c";
            c["text"] = text;
            c["uri"] = uri;
            return response(id, "result", Json{{"contents", Json::array({std::move(c)})}});
        } catch (const std::exception&) {
            return error_response(id, -32603, "Error creating resource from template " + uri, &data);
        }
    }
    const Json data = m;
    return error_response(id, -32601, "Method not found", &data);
}

int Server::serve_stdio() {
    // Stdout is the protocol: file 1 is pointed at stderr while serving, so anything the commands print cannot corrupt it.
    std::fflush(stdout);
    const int proto = _dup(1);
    _dup2(2, 1);
    _setmode(proto, _O_BINARY);
    // Each request is answered on a thread of its own, as the library does, so a slow tool does not hold up ping; a client that waits for
    // each reply sees the order it asked in. What is still running at the end of input is dropped with the process.
    struct Shared {
        std::mutex write;
        std::atomic<bool> initialized{false};
    };
    const auto shared = std::make_shared<Shared>();
    std::vector<std::thread> workers;
    std::string line;
    while (std::getline(std::cin, line)) {
        while (!line.empty() && (line.back() == '\r' || line.back() == '\n')) line.pop_back();
        if (text::trim(line).empty()) continue;
        Json message;
        try {
            message = Json::parse(line);
        } catch (const std::exception&) {   // a line that is not JSON is dropped: the library logs it and answers nothing
            continue;
        }
        const auto answer_one = [this, shared, proto](const Json& one) {
            bool flag = shared->initialized.load();
            const Json answer = handle(one, &flag);
            if (flag) shared->initialized.store(true);
            if (answer.is_null()) return;
            const std::string text = answer.dump(-1, ' ', false, Json::error_handler_t::replace) + "\n";
            const std::lock_guard<std::mutex> hold(shared->write);
            _write(proto, text.data(), static_cast<unsigned>(text.size()));
        };
        // The handshake is answered here, before anything after it is read: a request that follows `initialize` on the same pipe
        // must find the session initialized (on a thread of its own it could be handled first and refused).
        const std::string method = message.contains("method") && message["method"].is_string() ? message["method"].get<std::string>() : "";
        if (method == "initialize" || method == "notifications/initialized") {
            answer_one(message);
            continue;
        }
        workers.emplace_back([answer_one, message = std::move(message)] { answer_one(message); });
    }
    // Every request read is answered before the server goes (as the Python's does): a worker uses this server, so none may outlive it.
    for (std::thread& w : workers) w.join();
    impl_->link.stop_mine();   // only the jobs this server started
    return 0;
}

std::string token(const sfs::path& state) {
    if (const std::optional<std::string> env = fs::get_env("CB_MCP_TOKEN"); env && !env->empty()) return *env;
    const sfs::path file = state / "mcp" / "http_token.txt";
    std::error_code ec;
    if (!sfs::is_regular_file(file, ec)) {
        static constexpr std::string_view alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
        std::random_device rd;
        std::string t;
        std::array<unsigned char, 32> b{};
        for (unsigned char& x : b) x = static_cast<unsigned char>(rd());
        for (std::size_t i = 0; i < b.size(); i += 3) {   // 32 random bytes in url-safe base64, no padding
            const unsigned n = (static_cast<unsigned>(b[i]) << 16) | (i + 1 < b.size() ? static_cast<unsigned>(b[i + 1]) << 8 : 0) | (i + 2 < b.size() ? b[i + 2] : 0u);
            t += alphabet[(n >> 18) & 63];
            t += alphabet[(n >> 12) & 63];
            if (i + 1 < b.size()) t += alphabet[(n >> 6) & 63];
            if (i + 2 < b.size()) t += alphabet[n & 63];
        }
        fs::write_text(file, t);
    }
    return std::string(text::trim(fs::read_text(file)));
}

}  // namespace cb::ui::mcp
