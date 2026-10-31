// api.cc -- cbl2.dll's C interface (l2api.h): one entry, l2_call, dispatching by name to the L2 core
// and the C back end. Each call runs on a thread of its own with a large stack (the writer and the
// pattern matcher recurse as the Python did) and its own state.
#include "l2api.h"

#include <cstdlib>
#include <cstring>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#ifdef CBL2_CBACK
#include "cback.hpp"
#endif
#include "json.hpp"
#include "l2.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#undef far
#undef near
#endif

using py::V;

namespace {

std::mutex schema_mu;
std::shared_ptr<l2::Schema> schema;

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

V dispatch(const std::string& fn, const V& args, V& facts_seen) {
#ifdef CBL2_CBACK
    using namespace cb;
    if (fn == "function") {
        facts_seen = arg(args, 3);
        std::string text = cb::function(arg(args, 0), arg(args, 1), arg(args, 2), facts_seen);
        return V::tuple({V(text), facts_out(facts_seen)});
    }
    Scope scope;
    if (fn == "flags") return cb::flags(arg(args, 0), arg(args, 1));
    if (fn == "signature") return V(cb::signature(arg(args, 0)));
    if (fn == "type_text") return V(cb::type_text(arg(args, 0), py::truthy(arg(args, 1)) ? arg(args, 1).str() : std::string(), py::truthy(arg(args, 2))));
    if (fn == "expr") return V(cb::expr(arg(args, 0)));
    if (fn == "condition") return V(cb::condition(arg(args, 0), py::truthy(arg(args, 1))));
    if (fn == "module_names32") return cb::module_names32(arg(args, 0));
    if (fn == "type_names") return cb::type_names(arg(args, 0));
    if (fn == "parameter_names") return cb::parameter_names(arg(args, 0));
    if (fn == "struct_accesses") return cb::struct_accesses(arg(args, 0), arg(args, 1));
    if (fn == "takes32") return cb::takes32(arg(args, 0).str());
    if (fn == "prototypes") return cb::prototypes(arg(args, 0), arg(args, 1));
    if (fn == "segment_plan")
        return cb::segment_plan(arg(args, 0), arg(args, 1), arg(args, 2), arg(args, 3), arg(args, 4), arg(args, 5), arg(args, 6), arg(args, 7));
    if (fn == "caller_frame_plan") return cb::caller_frame_plan(arg(args, 0), arg(args, 1));
    if (fn == "seg_layout") return cb::seg_layout(arg(args, 0));
    if (fn == "arguments_at") return cb::arguments_at(arg(args, 0).str(), arg(args, 1).as_int());
    if (fn == "words") return cb::words_of(arg(args, 0).str());
    if (fn == "parameters") return cb::parameters(arg(args, 0).str());
    if (fn == "argument_words") return cb::argument_words(arg(args, 0).str(), arg(args, 1));
    if (fn == "fragment") return V(cb::fragment(arg(args, 0)));
    if (fn == "module") return cb::module(arg(args, 0), arg(args, 1));
#else
    (void)facts_seen;
#endif
    // the L2 core
    if (fn == "roundtrip") return l2::roundtrip(arg(args, 0));
    if (fn == "roundtrip_text") return V(py::json_dump(l2::roundtrip(arg(args, 0))));
    if (fn == "dumps") return V(py::json_dump(arg(args, 0)));
    if (fn == "set_schema") {
        std::lock_guard<std::mutex> lock(schema_mu);
        schema = std::make_shared<l2::Schema>(arg(args, 0));
        return V(true);
    }
    if (fn == "validate") {
        std::shared_ptr<l2::Schema> s;
        {
            std::lock_guard<std::mutex> lock(schema_mu);
            s = schema;
        }
        if (!s) py::raise("RuntimeError", "no schema: call set_schema first");
        std::string why;
        bool ok = s->valid(arg(args, 0), &why);
        return V::tuple({V(ok), V(why)});
    }
    py::raise("AttributeError", "cbl2 has no entry " + fn);
}

struct Call {
    std::string fn, in;
    std::string out;
    int status = 0;
};

void run(Call* c) {
    V facts;
    try {
        V args = py::json_parse(c->in, true);
        V result = dispatch(c->fn, args, facts);
        c->out = py::json_tagged(result);
        c->status = 0;
        return;
#ifdef CBL2_CBACK
    } catch (cb::Decline& e) {
        V err = V::dict();
        err.set("type", V("Decline"));
        err.set("message", V(std::string(e.what())));
        err.set("facts", facts_out(facts));
        c->out = py::json_tagged(err);
#endif
    } catch (py::PyError& e) {
        V err = V::dict();
        err.set("type", V(e.type));
        err.set("message", V(std::string(e.what())));
        err.set("facts", facts_out(facts));
        c->out = py::json_tagged(err);
    } catch (std::exception& e) {
        V err = V::dict();
        err.set("type", V("RuntimeError"));
        err.set("message", V(std::string("native: ") + e.what()));
        err.set("facts", facts_out(facts));
        c->out = py::json_tagged(err);
    }
    c->status = 1;
}

#ifdef _WIN32
// Threads with a large stack, kept and reused: a call is handed to an idle one (made the first time
// none is idle), so calls on several threads at once each get their own.
struct Worker {
    HANDLE thread = nullptr, go = nullptr, done = nullptr;
    Call* job = nullptr;
};
std::mutex pool_mu;
std::vector<Worker*> idle;

DWORD WINAPI worker_main(LPVOID p) {
    Worker* w = static_cast<Worker*>(p);
    while (true) {
        WaitForSingleObject(w->go, INFINITE);
        run(w->job);
        SetEvent(w->done);
    }
}

void run_on_worker(Call* c) {
    Worker* w = nullptr;
    {
        std::lock_guard<std::mutex> lock(pool_mu);
        if (!idle.empty()) {
            w = idle.back();
            idle.pop_back();
        }
    }
    if (!w) {
        w = new Worker();
        w->go = CreateEventW(nullptr, FALSE, FALSE, nullptr);
        w->done = CreateEventW(nullptr, FALSE, FALSE, nullptr);
        w->thread = CreateThread(nullptr, 512u << 20, worker_main, w, STACK_SIZE_PARAM_IS_A_RESERVATION, nullptr);
        if (!w->thread) {
            run(c);
            return;
        }
    }
    w->job = c;
    SetEvent(w->go);
    WaitForSingleObject(w->done, INFINITE);
    std::lock_guard<std::mutex> lock(pool_mu);
    idle.push_back(w);
}
#else
void run_on_worker(Call* c) { run(c); }
#endif

}  // namespace

extern "C" {

int l2_version(void) { return 1; }

int l2_call(const char* fn, const char* in, size_t inlen, char** out, size_t* outlen) {
    Call c;
    c.fn = fn;
    c.in.assign(in, inlen);
    run_on_worker(&c);
    char* buf = static_cast<char*>(std::malloc(c.out.size() + 1));
    std::memcpy(buf, c.out.data(), c.out.size());
    buf[c.out.size()] = 0;
    *out = buf;
    *outlen = c.out.size();
    return c.status;
}

void l2_free(char* p) { std::free(p); }
}
