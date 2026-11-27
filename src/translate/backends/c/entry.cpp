// translate/backends/c/entry.cpp -- the library's C entry points answered by the C back end on the typed tree (entry.hpp): the
// arguments read as the library reads them (its JSON reader, its tagged form), each document made a tree (l2::from_json), the
// answer written by the library's own writer, a failure answered as the library answers it.
#include "translate/backends/c/entry.hpp"

#include <chrono>
#include <cstdlib>
#include <list>
#include <memory>
#include <functional>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>

#include "core/fs.hpp"
#include "core/json.hpp"
#include "translate/backends/c/ctree.hpp"
#include "translate/l2.hpp"

#if defined(_MSC_VER)
#pragma warning(push, 3)
#endif
#include "translate/l2/src/json.hpp"
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#ifdef _WIN32
#include <windows.h>
#undef far
#undef near
#endif

namespace cb::translate::ctree {

namespace {

// the keys of the facts backends/c.function writes (handed back; the others it only reads)
const char* FACT_OUT[] = {"novalue", "registers", "declared", "selector_decls", "selector_shared", "interrupts", "frame",
                          "selector_imports", "code_reads", "names", "heading", "structs_used"};

V facts_out(const V& facts) {
    V out = V::dict();
    if (!facts.is_dict()) return out;
    for (auto k : FACT_OUT)
        if (facts.has(k)) out.set(k, facts.get(k));
    return out;
}

const V& arg(const V& args, size_t n) { return n < args.size() ? args[n] : py::NONE; }

V err_of(const std::string& type, const std::string& msg) {
    V e = V::dict(), l = V::list();
    l.append(V(type));
    l.append(V(msg));
    e.set("$err", l);
    return e;
}

// A document as the JSON it was (the library's reader made it values; the tree's reader reads JSON).
Json json_of(const V& v) {
    switch (v.k) {
        case py::K::None: return Json(nullptr);
        case py::K::Bool: return Json(v.b);
        case py::K::Int: return v.u64 ? Json(static_cast<std::uint64_t>(v.i)) : Json(v.i);
        case py::K::Float: return Json(v.f);
        case py::K::Str: return Json(v.str());
        case py::K::List:
        case py::K::Tuple:
        case py::K::Set: {
            Json out = Json::array();
            auto& items = out.get_ref<Json::array_t&>();
            items.reserve(v.seq().size());
            for (const V& x : v.seq()) items.push_back(json_of(x));
            return out;
        }
        case py::K::Dict: {
            Json out = Json::object();
            auto& members = out.get_ref<Json::object_t&>();   // (the keys are a dict's: each once, in order -- no lookup)
            members.reserve(v.items().size());
            for (const auto& [k, x] : v.items()) members.emplace_back(k.is_str() ? k.str() : py::str_of(k), json_of(x));
            // (a function type written by hand may leave out its parameters and result: the back end read those as none, the
            // tree's reader wants them said)
            // (an inline fragment's assembly given as its lines: the tree holds the text, its lines joined)
            if (v.get("node") == "MachineFragment" && v.get("asm").is_list()) {
                Strs lines;
                for (const V& l : v.get("asm").seq()) lines.push_back(py::str_of(l));
                out["asm"] = py::join("\n", lines);
            }
            if (!v.has("node") && v.get("kind") == "fn") {
                if (!out.contains("params")) out["params"] = Json::array();
                if (!out.contains("result")) out["result"] = Json::object();
            }
            return out;
        }
    }
    return Json(nullptr);
}

// A document's tree, made once from its text and kept while later calls bring the same text: a module's functions are written in
// turn and asked of again (flags, function, the module's questions, the next pass over the module ...). The writer only reads the
// tree (what it makes is its own), so one tree serves them all. Kept by its text, the least recently used let go past a budget.
struct Held {
    std::string text;
    std::unique_ptr<Arena> arena;
    ir::Node* tree = nullptr;
};
constexpr std::size_t kHeldBytes = 8u << 20;    // (of documents' text, a thread; their trees are some times that)
struct HeldTrees {
    std::list<Held> order;                                                  // the most recently used first
    std::unordered_map<std::string_view, std::list<Held>::iterator> by_text;   // (the key is the held text's own)
    std::size_t bytes = 0;

    ir::Node* find(std::string_view text) {
        const auto it = by_text.find(text);
        if (it == by_text.end()) return nullptr;
        order.splice(order.begin(), order, it->second);
        return it->second->tree;
    }
    ir::Node* add(Held h) {
        bytes += h.text.size();
        order.push_front(std::move(h));
        by_text[order.front().text] = order.begin();
        while (bytes > kHeldBytes && order.size() > 1) {
            by_text.erase(order.back().text);
            bytes -= order.back().text.size();
            order.pop_back();
        }
        return order.front().tree;
    }
};
thread_local HeldTrees held_trees;

ir::Node* held_tree(std::string_view text) { return held_trees.find(text); }

// One call's documents: the trees of its arguments, kept while it is answered. The first argument, where it is a document, is the
// tree its text made (`first`, held for the next call: `keep`), the others' made from the values the arguments were read as.
struct Docs {
    Arena arena;
    std::vector<ir::TypeP> types;
    std::optional<X> first;
    std::string_view keep;       // the first document's text, when its tree is to be held
    X node(const V& doc) {
        if (first) return *first;
        if (doc.is_none()) return X();
        if (keep.empty()) return X::node(l2::from_json(arena, json_of(doc)));
        Held h;
        h.text = std::string(keep);
        h.arena = std::make_unique<Arena>();
        h.tree = l2::from_json(*h.arena, json_of(doc));
        first = X::node(held_trees.add(std::move(h)));
        return *first;
    }
    const ir::Type* type(const V& doc) {
        types.push_back(l2::type_from_json(json_of(doc)));
        return types.back().get();
    }
};
V dispatch(const std::string& fn, const V& args, V& facts_seen, Docs& docs) {
    if (fn == "function") {
        facts_seen = arg(args, 3);
        const X doc = docs.node(arg(args, 0));
        std::string text = function(doc, arg(args, 1), arg(args, 2), facts_seen);
        return V::tuple({V(text), facts_out(facts_seen)});
    }
    if (fn == "module") return module(docs.node(arg(args, 0)), arg(args, 1));
    if (fn == "describe") {
        // what a module asks of each function's document, loop after loop, in one call: each entry's value, or {"$err": [its
        // exception's name, message]}
        const X doc = docs.node(arg(args, 0));
        V out = V::dict();
        auto one = [&](const char* name, const std::function<V()>& f) {
            try {
                Scope s;
                out.set(name, f());
            } catch (Decline& e) {
                out.set(name, err_of("Decline", e.what()));
            } catch (py::PyError& e) {
                out.set(name, err_of(e.type, e.what()));
            }
        };
        one("module_names32", [&] { return module_names32(doc); });
        one("type_names", [&] { return type_names(doc); });
        one("parameter_names", [&] { return parameter_names(doc); });
        one("signature", [&] { return V(signature(doc)); });
        if (out.get("signature").is_str()) one("takes32", [&] { return takes32(out.get("signature").str()); });
        return out;
    }
    Scope scope;
    if (fn == "flags") return flags(docs.node(arg(args, 0)), arg(args, 1));
    if (fn == "signature") return V(signature(docs.node(arg(args, 0))));
    if (fn == "type_text")
        return V(type_text(docs.type(arg(args, 0)), py::truthy(arg(args, 1)) ? arg(args, 1).str() : std::string(), py::truthy(arg(args, 2))));
    if (fn == "expr") return V(expr(docs.node(arg(args, 0))));
    if (fn == "condition") return V(condition(docs.node(arg(args, 0)), py::truthy(arg(args, 1))));
    if (fn == "module_names32") return module_names32(docs.node(arg(args, 0)));
    if (fn == "type_names") return type_names(docs.node(arg(args, 0)));
    if (fn == "parameter_names") return parameter_names(docs.node(arg(args, 0)));
    if (fn == "struct_accesses") return struct_accesses(docs.node(arg(args, 0)), arg(args, 1));
    if (fn == "takes32") return takes32(arg(args, 0).str());
    if (fn == "prototypes") return prototypes(arg(args, 0), arg(args, 1));
    if (fn == "segment_plan")
        return segment_plan(arg(args, 0), arg(args, 1), arg(args, 2), arg(args, 3), arg(args, 4), arg(args, 5), arg(args, 6), arg(args, 7));
    if (fn == "caller_frame_plan") return caller_frame_plan(arg(args, 0), arg(args, 1));
    if (fn == "seg_layout") return seg_layout(arg(args, 0));
    if (fn == "arguments_at") return arguments_at(arg(args, 0).str(), arg(args, 1).as_int());
    if (fn == "words") return words_of(arg(args, 0).str());
    if (fn == "parameters") return parameters(arg(args, 0).str());
    if (fn == "argument_words") return argument_words(arg(args, 0).str(), arg(args, 1));
    if (fn == "fragment") return V(fragment(docs.node(arg(args, 0))));
    py::raise("AttributeError", "the C back end has no entry " + fn);
}

// L2 documents handed over once and then by number ({"$put": n, "$v": doc}, then {"$doc": n}), as the library takes them: a function's
// document goes to several entry points in turn (module_names32, signature, function ...)
std::mutex docs_mu;
std::vector<std::pair<int64_t, V>> held;              // the latest few, newest last

void held_documents(V& args) {
    if (!args.is_list()) return;
    for (auto& a : args.seq()) {
        if (!a.is_dict() || a.size() > 2) continue;
        std::lock_guard<std::mutex> lock(docs_mu);
        if (a.has("$put")) {
            int64_t n = a.get("$put").as_int();
            V doc = a.get("$v");
            held.push_back({n, doc});
            if (held.size() > 2048) held.erase(held.begin());
            a = doc;
        } else if (a.has("$doc") && a.size() == 1) {
            int64_t n = a.get("$doc").as_int();
            bool found = false;
            for (auto& d : held)
                if (d.first == n) {
                    a = d.second;
                    found = true;
                }
            if (!found) py::raise("RuntimeError", "native/l2: document " + std::to_string(n) + " no longer held");
        }
    }
}

// The entries whose first argument is a node's document.
bool takes_document(const std::string& fn) {
    static const char* const names[] = {"function", "module", "describe", "flags", "signature", "expr", "condition",
                                        "module_names32", "type_names", "parameter_names", "struct_accesses", "fragment"};
    for (const char* n : names)
        if (fn == n) return true;
    return false;
}

// Where the first item of a JSON array ends, when it is an object other than a document handed over by number ({"$put" ...},
// {"$doc" ...}): the index just past its "}"; 0 otherwise.
std::size_t first_object_end(const std::string& text) {
    if (text.size() < 3 || text[0] != '[' || text[1] != '{') return 0;
    if (text.compare(2, 5, "\"$put") == 0 || text.compare(2, 5, "\"$doc") == 0) return 0;
    int depth = 0;
    bool in_string = false;
    for (std::size_t i = 1; i < text.size(); i++) {
        const char ch = text[i];
        if (in_string) {
            if (ch == '\\') i++;
            else if (ch == '"') in_string = false;
        } else if (ch == '"') {
            in_string = true;
        } else if (ch == '{' || ch == '[') {
            depth++;
        } else if (ch == '}' || ch == ']') {
            if (--depth == 0) return i + 1;
        }
    }
    return 0;
}

struct Call {
    std::string fn, in;
    std::string out;
    int status = 0;
};

V error_answer(const std::string& type, const std::string& message, const V& facts) {
    V err = V::dict();
    err.set("type", V(type));
    err.set("message", V(message));
    err.set("facts", facts_out(facts));
    return err;
}

void run(Call* c) {
    V facts;
    try {
        Docs docs;
        V args;
        bool read = false;
        const std::size_t end = takes_document(c->fn) ? first_object_end(c->in) : 0;
        if (end != 0) {
            const std::string_view text = std::string_view(c->in).substr(1, end - 1);
            if (ir::Node* tree = held_tree(text)) {   // (the document a call before this one brought: its tree, and the rest read)
                docs.first = X::node(tree);
                args = py::json_parse("[null" + c->in.substr(end), true);
                read = true;
            } else
                docs.keep = text;
        }
        if (!read) {
            args = py::json_parse(c->in, true);
            held_documents(args);
        }
        V result = dispatch(c->fn, args, facts, docs);
        c->out = py::json_tagged(result);
        c->status = 0;
        return;
    } catch (Decline& e) {
        c->out = py::json_tagged(error_answer("Decline", e.what(), facts));
    } catch (py::PyError& e) {
        c->out = py::json_tagged(error_answer(e.type, e.what(), facts));
    } catch (std::exception& e) {
        c->out = py::json_tagged(error_answer("RuntimeError", std::string("native: ") + e.what(), facts));
    }
    c->status = 1;
}

#ifdef _WIN32
// A large stack for the deep entries (the writer recurses as the Python did), on the caller's own thread: a fiber per thread, made
// once (512 MB reserved), switched to for the call and back, as the library does.
thread_local LPVOID big_fiber = nullptr;
thread_local LPVOID caller_fiber = nullptr;
thread_local Call* fiber_job = nullptr;

VOID CALLBACK fiber_main(LPVOID) {
    while (true) {
        run(fiber_job);
        SwitchToFiber(caller_fiber);
    }
}

void run_deep(Call* c) {
    bool converted = false;
    LPVOID me;
    if (IsThreadAFiber())
        me = GetCurrentFiber();
    else {
        me = ConvertThreadToFiber(nullptr);
        converted = me != nullptr;
    }
    if (!me) {
        run(c);
        return;
    }
    if (!big_fiber) big_fiber = CreateFiberEx(1u << 20, 512u << 20, FIBER_FLAG_FLOAT_SWITCH, fiber_main, nullptr);
    if (!big_fiber) {
        if (converted) ConvertFiberToThread();
        run(c);
        return;
    }
    caller_fiber = me;
    fiber_job = c;
    SwitchToFiber(big_fiber);
    if (converted) ConvertFiberToThread();
}
#else
void run_deep(Call* c) { run(c); }
#endif

}  // namespace

bool enabled() {
    static const bool on = [] {
        const std::optional<std::string> v = fs::get_env("CB_C_TREE");
        return v && *v == "1";
    }();
    return on;
}

bool handles(std::string_view fn) {
    static const char* const names[] = {"function", "module",        "describe",  "flags",         "signature",       "type_text",
                                        "expr",     "condition",     "module_names32", "type_names", "parameter_names", "struct_accesses",
                                        "takes32",  "prototypes",    "segment_plan", "caller_frame_plan", "seg_layout", "arguments_at",
                                        "words",    "parameters",    "argument_words", "fragment"};
    for (const char* n : names)
        if (fn == n) return true;
    return false;
}

std::pair<int, std::string> call(std::string_view fn, std::string_view arguments_json) {
    Call c;
    c.fn = std::string(fn);
    c.in = std::string(arguments_json);
    static const char* const deep[] = {"function", "module", "expr", "condition", "flags"};
    bool big = false;
    for (const char* d : deep) big = big || c.fn == d;
    if (big) run_deep(&c);
    else run(&c);
    return {c.status, std::move(c.out)};
}

}  // namespace cb::translate::ctree
