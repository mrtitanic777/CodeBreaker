// translate/backends/cpp17_tree/entry.cpp -- the library's C++17 entry points answered by the C++17 back end on the typed tree
// (entry.hpp): the arguments read as the library reads them (its JSON reader, its tagged form), each unit's document made a tree
// (l2::from_json), the answer written by the library's own writer, a failure answered as the library answers it.
#include "translate/backends/cpp17_tree/entry.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "core/json.hpp"
#include "translate/backends/cpp17_tree/cpp17tree.hpp"
#include "translate/l2.hpp"

#if defined(_MSC_VER)
#pragma warning(push, 3)
#endif
#include "translate/support/pyjson.hpp"
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#ifdef _WIN32
#include <windows.h>
#undef far
#undef near
#endif

namespace cb::translate::cpp17tree {

namespace {

const V& arg(const V& args, size_t n) { return n < args.size() ? args[n] : py::NONE; }

// A document as the JSON it was (the library's reader made it values; the tree's reader reads JSON).
Json json_of(const V& v) {
    switch (v.k) {
        case py::K::None: return Json(nullptr);
        case py::K::Bool: return Json(v.b);
        case py::K::Int: return v.u64 ? Json(static_cast<std::uint64_t>(v.i)) : Json(static_cast<std::int64_t>(v.i));
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
            return out;
        }
    }
    return Json(nullptr);
}

// The tree of a unit's document, made in the call's arena.
NodeP tree_of(Arena& arena, const V& doc) { return l2::from_json(arena, json_of(doc)); }

V dispatch(const std::string& fn, const V& args, Arena& arena) {
    Made made(arena);
    if (fn == "cpp17_unit")
        return write_unit(tree_of(arena, arg(args, 0)), py::truthy(arg(args, 1)) ? arg(args, 1).str() : std::string());
    if (fn == "cpp17_program") {
        std::vector<NodeP> units;
        for (const V& d : arg(args, 0).seq()) units.push_back(tree_of(arena, d));
        return V(program(units, arg(args, 1)));
    }
    py::raise("AttributeError", "the C++17 back end has no entry " + fn);
}

struct Call {
    std::string fn, in;
    std::string out;
    int status = 0;
};

V error_answer(const std::string& type, const std::string& message) {
    V err = V::dict();
    err.set("type", V(type));
    err.set("message", V(message));
    err.set("facts", V::dict());
    return err;
}

void run(Call* c) {
    try {
        Arena arena;
        const V args = py::json_parse(c->in, true);
        V result = dispatch(c->fn, args, arena);
        c->out = py::json_tagged(result);
        c->status = 0;
        return;
    } catch (Decline& e) {
        c->out = py::json_tagged(error_answer("Cpp17Decline", e.what()));
    } catch (py::PyError& e) {
        c->out = py::json_tagged(error_answer(e.type, e.what()));
    } catch (std::exception& e) {
        c->out = py::json_tagged(error_answer("RuntimeError", std::string("native: ") + e.what()));
    }
    c->status = 1;
}

#ifdef _WIN32
// A large stack for the writer (it recurses as the Python did), on the caller's own thread: a fiber per thread, made once (512 MB
// reserved), switched to for the call and back, as the library does.
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

bool handles(std::string_view fn) { return fn == "cpp17_unit" || fn == "cpp17_program"; }

std::pair<int, std::string> call(std::string_view fn, std::string_view arguments_json) {
    Call c;
    c.fn = std::string(fn);
    c.in = std::string(arguments_json);
    run_deep(&c);
    return {c.status, std::move(c.out)};
}

}  // namespace cb::translate::cpp17tree
