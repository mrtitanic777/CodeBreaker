// gen.cc -- generators: each runs its body on a fiber of its own (Windows fibers, pooled), so a
// generator is written as the plain function it is in Python (yield anywhere: in loops, in try /
// finally, in nested calls through yield from). next() switches to it and back; send() and throw()
// as Python's; an unfinished generator that is dropped is closed (GeneratorExit raised at its yield,
// its finally blocks run) before its fiber is reused.
//
// Coroutines (async def) are the same objects on the same fibers, of type coroutine: `await x`
// delegates to x's __await__ iterator as `yield from` does (what it yields goes up to whoever drives
// the outermost coroutine -- asyncio's Task -- and what that sends comes back down). An async
// generator (async def with yield) tells its two kinds of switch apart: a `yield v` completes the
// awaitable its __anext__() / asend() returned (StopIteration(v)), an await inside it passes up.
#include <windows.h>

#include <exception>

#include "rt.hpp"

namespace py {

struct GenObj;

struct Fiber {
    LPVOID handle = nullptr;
    GenObj* job = nullptr;
    LPVOID caller = nullptr;
};

struct GenObj : Obj {
    FuncObj* f = nullptr;
    var func;                       // (keeps f)
    std::vector<var> slots;
    Fiber* fiber = nullptr;
    yielder y;
    enum State { Fresh, Suspended, Running, Done } state = Fresh;
    var sent;                       // what send() passes in
    var value;                      // what the last yield gave
    var returned;                   // StopIteration's value
    std::exception_ptr exc;         // raised inside, to rethrow in the caller
    var throw_in;                   // an exception to raise at the yield (throw / close)
    int kind = 0;                   // 0 generator, 1 coroutine, 2 async generator
    bool iterable_coro = false;     // a generator of a @types.coroutine function: awaitable
    bool value_yield = false;       // async generator: the last switch out was a `yield v`
    bool hooked = false;            // async generator: sys's firstiter hook called
    var awaiting;                   // what it awaits now (cr_await / ag_await)
    int depth = 0;                  // its own call depth (kept apart from the caller's)
    void* handling = nullptr;       // its own stack of exceptions being handled
    void* calls = nullptr;          // its own stack of functions being run (tracebacks)
    bool closing = false;           // close(): a GeneratorExit that ends the body is the close working, not raised again
    GenObj() { gc_track(this); }
    ~GenObj() override;
    // (what its body's own stack holds while it is suspended is not followed: a cycle through a suspended
    // generator's locals is not found)
    void gc_traverse(gc_visit visit, void* arg) override {
        Obj::gc_traverse(visit, arg);
        gc_visit_var(func, visit, arg);
        for (const var& s : slots) gc_visit_var(s, visit, arg);
        gc_visit_var(sent, visit, arg);
        gc_visit_var(value, visit, arg);
        gc_visit_var(returned, visit, arg);
        gc_visit_var(throw_in, visit, arg);
        gc_visit_var(awaiting, visit, arg);
        if (handling)
            for (const var& e : *static_cast<std::vector<var>*>(handling)) gc_visit_var(e, visit, arg);
    }
    // (its function stays: the body's code is its lambda; the function's own clear breaks a cycle through it)
    void gc_clear() override {
        if (state == Running) return;
        std::vector<var> s;
        s.swap(slots);
        var a = std::move(sent), b = std::move(value), c = std::move(returned), d = std::move(throw_in), e = std::move(awaiting);
        Obj::gc_clear();
    }
};

struct WrapperObj : Obj {               // what coroutine.__await__() returns
    var coro;
    WrapperObj() { gc_track(this); }
    void gc_traverse(gc_visit visit, void* arg) override {
        Obj::gc_traverse(visit, arg);
        gc_visit_var(coro, visit, arg);
    }
    void gc_clear() override {
        var c = std::move(coro);
        Obj::gc_clear();
    }
};

TypeObj* T(const var& v);   // object.cc
int& depth_slot_();      // call.cc
void*& calls_slot_();    // call.cc
void*& handling_slot_(); // exc.cc

TypeObj *T_coroutine = nullptr, *T_coroutine_wrapper = nullptr, *T_async_generator = nullptr, *T_async_gen_asend = nullptr;

static thread_local std::vector<Fiber*>* t_pool = nullptr;
static thread_local LPVOID t_main_fiber = nullptr;
static thread_local yielder* t_starting = nullptr;

yielder& current_yielder_() { return *t_starting; }

static void ensure_thread_fiber() {
    if (t_main_fiber) return;
    t_main_fiber = IsThreadAFiber() ? GetCurrentFiber() : ConvertThreadToFiber(nullptr);
}

static void WINAPI fiber_main(LPVOID p) {
    Fiber* fb = static_cast<Fiber*>(p);
    for (;;) {
        GenObj* g = fb->job;
        try {
            t_starting = &g->y;
            var r = g->f->fn(g->slots.data());
            g->returned = r;
        } catch (error& e) {
            // (a close that ended the body: nothing to hand back -- rethrowing it in the caller only to drop it
            // there was a second C++ throw for every generator dropped unfinished: next(gen, None) on a decoder)
            if (!(g->closing && isinstance_(e.value, E_GeneratorExit))) g->exc = std::current_exception();
        } catch (...) {
            g->exc = std::current_exception();
        }
        g->state = GenObj::Done;
        g->slots.clear();
        fb->job = nullptr;
        SwitchToFiber(fb->caller);
    }
}

static Fiber* get_fiber() {
    ensure_thread_fiber();
    if (!t_pool) t_pool = new std::vector<Fiber*>();
    if (!t_pool->empty()) {
        Fiber* f = t_pool->back();
        t_pool->pop_back();
        return f;
    }
    auto* f = new Fiber();
    // 64 KB committed, 16 MB reserved: deep recursion inside a generator still has room
    f->handle = CreateFiberEx(64 * 1024, 16 * 1024 * 1024, FIBER_FLAG_FLOAT_SWITCH, fiber_main, f);
    if (!f->handle) raise(E_MemoryError, "cannot make a generator's fiber");
    return f;
}
static void put_fiber(Fiber* f) {
    if (!t_pool) t_pool = new std::vector<Fiber*>();
    if (t_pool->size() < 64) t_pool->push_back(f);
    else DeleteFiber(f->handle), delete f;
}

var make_generator_(FuncObj* f, std::vector<var> slots) {
    auto* g = new GenObj();
    g->kind = f->async_kind == 1 || f->async_kind == 2 ? f->async_kind : 0;
    g->iterable_coro = f->async_kind == 3;
    g->type = g->kind == 1 ? T_coroutine : g->kind == 2 ? T_async_generator : T_generator;
    g->f = f;
    g->func = var(static_cast<Obj*>(f));
    g->slots = std::move(slots);
    g->y.impl_ = g;
    return var(static_cast<Obj*>(g));
}

static const char* noun(const GenObj* g) { return g->kind == 1 ? "coroutine" : g->kind == 2 ? "async generator" : "generator"; }

// run the generator until its next yield or its end: true with g->value at a yield
static bool resume(GenObj* g) {
    if (g->state == GenObj::Done) return false;
    if (g->state == GenObj::Running) raise(E_ValueError, std::string(noun(g)) + " already executing");
    if (g->state == GenObj::Fresh) {
        g->fiber = get_fiber();
        g->fiber->job = g;
    }
    g->state = GenObj::Running;
    g->fiber->caller = GetCurrentFiber();
    // the generator's own call depth and exception / call stacks while it runs; the caller's back after
    int caller_depth = depth_slot_();
    depth_slot_() = caller_depth + g->depth;
    std::swap(handling_slot_(), g->handling);
    std::swap(calls_slot_(), g->calls);
    SwitchToFiber(g->fiber->handle);
    std::swap(calls_slot_(), g->calls);
    std::swap(handling_slot_(), g->handling);
    g->depth = depth_slot_() - caller_depth;
    depth_slot_() = caller_depth;
    if (g->state == GenObj::Done) {
        put_fiber(g->fiber);
        g->fiber = nullptr;
        g->awaiting = var();
        if (g->exc) {
            std::exception_ptr e = g->exc;
            g->exc = nullptr;
            // a StopIteration escaping a generator is a RuntimeError (PEP 479); StopAsyncIteration an async generator's
            try {
                std::rethrow_exception(e);
            } catch (error& err) {
                if (isinstance_(err.value, E_StopIteration)) {
                    var ne = make_exc(E_RuntimeError, std::string(noun(g)) + " raised StopIteration");
                    static_cast<ExcObj*>(ne.ref())->cause = err.value;
                    throw error(ne);
                }
                if (g->kind == 2 && isinstance_(err.value, T(t::StopAsyncIteration))) {
                    var ne = make_exc(E_RuntimeError, "async generator raised StopAsyncIteration");
                    static_cast<ExcObj*>(ne.ref())->cause = err.value;
                    throw error(ne);
                }
                throw;
            }
        }
        return false;
    }
    return true;
}

// inside the generator: hand v to the caller, wait to be resumed
static var raw_yield(GenObj* g, const var& v) {
    g->value = v;
    g->state = GenObj::Suspended;
    SwitchToFiber(g->fiber->caller);
    g->state = GenObj::Running;
    if (!g->throw_in.is_unbound()) {
        var e = g->throw_in;
        g->throw_in = var();
        throw error(e);
    }
    var s = g->sent;
    g->sent = var();
    return s.is_unbound() ? var(None) : s;
}
var yielder::operator()(const var& v) {
    GenObj* g = static_cast<GenObj*>(impl_);
    g->value_yield = true;
    return raw_yield(g, v);
}

static bool is_gen_obj(const var& v) {
    if (!v.is_ref()) return false;
    TypeObj* t = v.ref()->type;
    return t == T_generator || t == T_coroutine || t == T_async_generator;
}
static var stop_value(const error& e) {
    var v = e.value.getattr("value");
    return v.is_unbound() ? var(None) : v;
}

// yield from / await: what inner yields goes up, what is sent (or thrown) comes down; its return value
static var delegate(GenObj* g, const var& inner_v) {
    g->awaiting = inner_v;
    struct clear_ { GenObj* g; ~clear_() { g->awaiting = var(); } } clear{g};
    var src = inner_v;
    if (src.is_ref() && src.ref()->type == T_coroutine_wrapper) src = static_cast<WrapperObj*>(src.ref())->coro;
    if (is_gen_obj(src)) {
        auto* inner = static_cast<GenObj*>(src.ref());
        var sent = None;
        for (;;) {
            inner->sent = sent;
            if (!resume(inner)) return inner->returned.is_unbound() ? var(None) : inner->returned;
            try {
                sent = raw_yield(g, inner->value);
            } catch (error& e) {
                if (isinstance_(e.value, E_GeneratorExit)) {
                    if (inner->state == GenObj::Suspended) { inner->throw_in = e.value; try { resume(inner); } catch (...) {} }
                    throw;
                }
                inner->throw_in = e.value;
                sent = None;
            }
        }
    }
    // any other iterator: next / send / throw / close called as PEP 380 says
    var it = src;
    var y;
    try {
        y = it.call("__next__");
    } catch (error& e) {
        if (isinstance_(e.value, E_StopIteration)) return stop_value(e);
        throw;
    }
    for (;;) {
        var s;
        bool thrown = false;
        var exc;
        try {
            s = raw_yield(g, y);
        } catch (error& e) {
            if (isinstance_(e.value, E_GeneratorExit)) {
                var cl;
                if (lookup_special(it, "close", cl)) call0(cl);
                throw;
            }
            thrown = true;
            exc = e.value;
        }
        try {
            if (thrown) {
                var th;
                if (!lookup_special(it, "throw", th)) throw error(exc);
                y = call1(th, exc);
            } else if (s.is_none()) {
                y = it.call("__next__");
            } else {
                y = it.call("send", s);
            }
        } catch (error& e) {
            if (isinstance_(e.value, E_StopIteration)) return stop_value(e);
            throw;
        }
    }
}

var yielder::yield_from(const var& src) {
    GenObj* g = static_cast<GenObj*>(impl_);
    if (src.is_ref() && src.ref()->type == T_generator) return delegate(g, src);
    if (src.is_ref() && src.ref()->type == T_coroutine) raise(E_TypeError, "cannot 'yield from' a coroutine object in a non-coroutine generator");
    var it = iter(src);
    if (it.is_ref() && (it.ref()->type == T_iter)) {   // (a builtin iterator: nothing to send to)
        var x;
        while (next_(it, x)) raw_yield(g, x);
        return None;
    }
    return delegate(g, it);
}

// the iterator `await x` runs: a coroutine itself, a @types.coroutine generator, else x.__await__()
static var await_iter(const var& x) {
    if (x.is_ref() && x.ref()->type == T_coroutine) {
        auto* c = static_cast<GenObj*>(x.ref());
        if (c->state == GenObj::Done) raise(E_RuntimeError, "cannot reuse already awaited coroutine");
        return x;
    }
    if (x.is_ref() && x.ref()->type == T_generator && static_cast<GenObj*>(x.ref())->iterable_coro) return x;
    var aw;
    if (!lookup_special(x, "__await__", aw)) raise(E_TypeError, "'" + tname(x) + "' object can't be awaited");
    var it = call0(aw);
    if (it.is_ref() && (it.ref()->type == T_coroutine || (it.ref()->type == T_generator && static_cast<GenObj*>(it.ref())->iterable_coro)))
        raise(E_TypeError, "__await__() returned a coroutine");
    var nx;
    if (!lookup_special(it, "__next__", nx)) raise(E_TypeError, "__await__() returned non-iterator of type '" + tname(it) + "'");
    return it;
}
var yielder::await_(const var& x) {
    GenObj* g = static_cast<GenObj*>(impl_);
    return delegate(g, await_iter(x));
}

var aiter_(const var& x) {
    var ai;
    if (!lookup_special(x, "__aiter__", ai)) raise(E_TypeError, "'async for' requires an object with __aiter__ method, got " + tname(x));
    var it = call0(ai);
    var an;
    if (!lookup_special(it, "__anext__", an))
        raise(E_TypeError, "'async for' received an object from __aiter__ that does not implement __anext__: " + tname(it));
    return it;
}
bool yielder::async_next(const var& ait, var& out) {
    var an;
    if (!lookup_special(ait, "__anext__", an)) raise(E_TypeError, "'async for' requires an iterator with __anext__ method, got " + tname(ait));
    var aw = call0(an);
    try {
        out = await_(aw);
    } catch (error& e) {
        if (isinstance_(e.value, T(t::StopAsyncIteration))) return false;
        throw;
    }
    return true;
}
var awith_exit_of(const var& mgr) {
    TypeObj* t = type_of(mgr);
    const var* ex = t->lookup("__aexit__");
    if (!ex || !t->lookup("__aenter__"))
        raise(E_TypeError, "'" + t->name + "' object does not support the asynchronous context manager protocol");
    return bind(*ex, mgr, t);
}
var awith_enter(const var& mgr) {
    TypeObj* t = type_of(mgr);
    return call0(bind(*t->lookup("__aenter__"), mgr, t));
}
var with_exit_call(const var& exit_fn, const error& e) {
    CallArgs a;
    var tb = e.value.is_ref() ? e.value.getattr("__traceback__") : var(None);
    a.pos = {tv(type_of(e.value)), e.value, tb.is_unbound() ? var(None) : tb};
    return call_obj(exit_fn, a);
}

bool gen_next_(const var& gv, var& out) {
    auto* g = static_cast<GenObj*>(gv.ref());
    g->sent = None;
    if (!resume(g)) return false;
    out = g->value;
    return true;
}

// close() of an unfinished generator that is going: GeneratorExit at the yield, the body's finally blocks run
static bool close_dropped(GenObj* g) {
    if (g->state != GenObj::Suspended || !g->fiber) return false;
    g->throw_in = make_exc_args(E_GeneratorExit, {});
    g->closing = true;
    try {
        resume(g);
    } catch (...) {
    }
    if (g->state != GenObj::Done) {
        // it yielded again while closing: abandon the fiber (its stack's references leak)
        g->fiber = nullptr;
    }
    return true;
}
// the collector's finalizer of a generator in a cycle of garbage (before the cycle is cleared)
bool gen_finalize_(Obj* g) { return close_dropped(static_cast<GenObj*>(g)); }

GenObj::~GenObj() {
    close_dropped(this);
    if (fiber) put_fiber(fiber);
    delete static_cast<std::vector<var>*>(handling);
    delete static_cast<std::vector<const FuncObj*>*>(calls);
}

var mark_async_(const var& f, int kind) {
    if (f.is_ref() && f.ref()->type == T_function) static_cast<FuncObj*>(f.ref())->async_kind = (uint8_t)kind;
    return f;
}

// ---------------------------------------------------------------- the methods
static GenObj* G(const var& v) { return static_cast<GenObj*>(v.ref()); }

static var gen_send(GenObj* g, const var& v) {
    if (g->kind == 1 && g->state == GenObj::Done) raise(E_RuntimeError, "cannot reuse already awaited coroutine");
    if (g->state == GenObj::Fresh && !v.is_none())
        raise(E_TypeError, std::string("can't send non-None value to a just-started ") + noun(g));
    g->sent = v;
    if (!resume(g)) raise_stop(g->returned.is_unbound() ? var(None) : g->returned);
    return g->value;
}
static var exc_of(const CallArgs& a, size_t i) {
    var e = a.pos.at(i);
    if (e.is_ref() && e.ref()->type->layout == Layout::Type) {
        CallArgs c;
        if (a.pos.size() > i + 1 && !a.pos[i + 1].is_none()) {
            var v = a.pos[i + 1];
            if (isinstance(v, e).truthy()) return v;
            c.pos = {v};
        }
        e = call_type(static_cast<TypeObj*>(e.ref()), c);
    }
    return e;
}
static var gen_throw(GenObj* g, const var& e) {
    if (g->state != GenObj::Suspended) {
        g->state = GenObj::Done;
        throw error(e);
    }
    g->throw_in = e;
    if (!resume(g)) raise_stop(g->returned.is_unbound() ? var(None) : g->returned);
    return g->value;
}
static var gen_close(GenObj* g) {
    if (g->state != GenObj::Suspended) { if (g->state == GenObj::Fresh) g->state = GenObj::Done; return None; }
    g->throw_in = make_exc_args(E_GeneratorExit, {});
    g->closing = true;
    try {
        bool again = resume(g);
        g->closing = false;
        if (again) raise(E_RuntimeError, std::string(noun(g)) + " ignored GeneratorExit");
    } catch (error& e) {
        if (!isinstance_(e.value, E_GeneratorExit) && !isinstance_(e.value, E_StopIteration)) throw;
    }
    return None;
}
var gen_name_(const var& g, bool qual) { return new_str(qual ? G(g)->f->qualname : G(g)->f->name); }   // (object.cc's getattr)

// sys.set_asyncgen_hooks / get_asyncgen_hooks: what asyncio's loop uses to finish async generators
static var g_firstiter = None, g_finalizer = None;
static void first_iter(GenObj* g) {
    if (g->hooked) return;
    g->hooked = true;
    if (!g_firstiter.is_none()) call1(g_firstiter, var(static_cast<Obj*>(g)));
}

// the awaitable of an async generator's __anext__ / asend / athrow / aclose
struct AsendObj : Obj {
    var gen;
    var arg;                 // what to send, or the exception to throw
    int mode = 0;            // 0 send, 1 throw, 2 close
    int state = 0;           // 0 not started, 1 running, 2 done
    AsendObj() { gc_track(this); }
    void gc_traverse(gc_visit visit, void* arg) override {
        Obj::gc_traverse(visit, arg);
        gc_visit_var(gen, visit, arg);
        gc_visit_var(this->arg, visit, arg);
    }
    void gc_clear() override {
        var g = std::move(gen), a = std::move(arg);
        Obj::gc_clear();
    }
};
static var asend_step(AsendObj* a, const var& v, const var* thrown) {
    if (a->state == 2) raise(E_RuntimeError, a->mode == 0 ? "cannot reuse already awaited __anext__()/asend()" : "cannot reuse already awaited aclose()/athrow()");
    GenObj* g = G(a->gen);
    if (g->state == GenObj::Done) {
        bool fresh = a->state == 0;
        a->state = 2;
        if (a->mode == 2) raise_stop(None);
        if (a->mode == 1 && fresh) throw error(a->arg);
        throw error(make_exc_args(T(t::StopAsyncIteration), {}));
    }
    if (g->state == GenObj::Running) raise(E_RuntimeError, "anext(): asynchronous generator is already running");
    if (a->state == 0) {
        a->state = 1;
        if (a->mode == 0) {
            if (g->state == GenObj::Fresh && !a->arg.is_none()) raise(E_TypeError, "can't send non-None value to a just-started async generator");
            g->sent = a->arg;
        } else {
            if (g->state == GenObj::Fresh) {   // never started: nothing to run its finally blocks
                g->state = GenObj::Done;
                a->state = 2;
                if (a->mode == 2) raise_stop(None);
                throw error(a->arg);
            }
            g->throw_in = a->mode == 2 ? make_exc_args(E_GeneratorExit, {}) : a->arg;
        }
    } else if (thrown) {
        g->throw_in = *thrown;
    } else {
        g->sent = v;
    }
    g->value_yield = false;
    bool more;
    try {
        more = resume(g);
    } catch (error& e) {
        a->state = 2;
        if (a->mode == 2 && (isinstance_(e.value, E_GeneratorExit) || isinstance_(e.value, T(t::StopAsyncIteration)))) raise_stop(None);
        throw;
    }
    if (!more) {
        a->state = 2;
        if (a->mode == 2) raise_stop(None);
        throw error(make_exc_args(T(t::StopAsyncIteration), {}));
    }
    if (g->value_yield) {
        g->value_yield = false;
        a->state = 2;
        if (a->mode == 2) raise(E_RuntimeError, "async generator ignored GeneratorExit");
        raise_stop(g->value);
    }
    return g->value;   // an await inside it, passed up
}
static var make_asend(const var& gen, int mode, const var& arg) {
    first_iter(G(gen));
    auto* a = new AsendObj();
    a->type = T_async_gen_asend;
    a->gen = gen;
    a->mode = mode;
    a->arg = arg;
    return var(static_cast<Obj*>(a));
}

void init_gen() {
    TypeObj* t = T_generator;
    add_method(t, "__iter__", [](const CallArgs& a) { return a.pos[0]; });
    add_method(t, "__next__", [](const CallArgs& a) {
        var out;
        if (!gen_next_(a.pos[0], out)) {
            auto* g = static_cast<GenObj*>(a.pos[0].ref());
            raise_stop(g->returned.is_unbound() ? var(None) : g->returned);
        }
        return out;
    });
    add_method(t, "send", [](const CallArgs& a) { return gen_send(G(a.pos[0]), a.pos.at(1)); });
    add_method(t, "throw", [](const CallArgs& a) { return gen_throw(G(a.pos[0]), exc_of(a, 1)); });
    add_method(t, "close", [](const CallArgs& a) { return gen_close(G(a.pos[0])); });
    add_getter(t, "gi_running", [](const var& s) { return var(G(s)->state == GenObj::Running); });
    add_getter(t, "gi_suspended", [](const var& s) { return var(G(s)->state == GenObj::Suspended); });
    add_getter(t, "gi_frame", [](const var& s) { return var(None); });
    add_getter(t, "gi_code", [](const var& s) { return var(None); });
    add_getter(t, "gi_yieldfrom", [](const var& s) { return G(s)->awaiting.is_unbound() ? var(None) : G(s)->awaiting; });

    // coroutine
    T_coroutine = new_type("coroutine", {}, Layout::Generator);
    t = T_coroutine;
    add_method(t, "send", [](const CallArgs& a) { return gen_send(G(a.pos[0]), a.pos.at(1)); });
    add_method(t, "throw", [](const CallArgs& a) { return gen_throw(G(a.pos[0]), exc_of(a, 1)); });
    add_method(t, "close", [](const CallArgs& a) { return gen_close(G(a.pos[0])); });
    add_method(t, "__await__", [](const CallArgs& a) {
        auto* w = new WrapperObj();
        w->type = T_coroutine_wrapper;
        w->coro = a.pos[0];
        return var(static_cast<Obj*>(w));
    });
    add_method(t, "__repr__", [](const CallArgs& a) {
        char buf[32];
        snprintf(buf, sizeof buf, "%p", (void*)a.pos[0].ref());
        return new_str("<coroutine object " + G(a.pos[0])->f->qualname + " at 0x" + buf + ">");
    });
    add_getter(t, "cr_running", [](const var& s) { return var(G(s)->state == GenObj::Running); });
    add_getter(t, "cr_suspended", [](const var& s) { return var(G(s)->state == GenObj::Suspended); });
    add_getter(t, "cr_await", [](const var& s) { return G(s)->awaiting.is_unbound() ? var(None) : G(s)->awaiting; });
    add_getter(t, "cr_frame", [](const var& s) { return var(None); });
    add_getter(t, "cr_code", [](const var& s) { return var(None); });
    add_getter(t, "cr_origin", [](const var& s) { return var(None); });
    // the iterator coroutine.__await__() returns
    {
        T_coroutine_wrapper = new_type("coroutine_wrapper", {}, Layout::NativeData);
        TypeObj* w = T_coroutine_wrapper;
        auto coro = [](const var& self) { return G(static_cast<WrapperObj*>(self.ref())->coro); };
        add_method(w, "__iter__", [](const CallArgs& a) { return a.pos[0]; });
        add_method(w, "__next__", [coro](const CallArgs& a) { return gen_send(coro(a.pos[0]), None); });
        add_method(w, "send", [coro](const CallArgs& a) { return gen_send(coro(a.pos[0]), a.pos.at(1)); });
        add_method(w, "throw", [coro](const CallArgs& a) { return gen_throw(coro(a.pos[0]), exc_of(a, 1)); });
        add_method(w, "close", [coro](const CallArgs& a) { return gen_close(coro(a.pos[0])); });
    }

    // async generator
    T_async_generator = new_type("async_generator", {}, Layout::Generator);
    t = T_async_generator;
    add_method(t, "__aiter__", [](const CallArgs& a) { return a.pos[0]; });
    add_method(t, "__anext__", [](const CallArgs& a) { return make_asend(a.pos[0], 0, None); });
    add_method(t, "asend", [](const CallArgs& a) { return make_asend(a.pos[0], 0, a.pos.at(1)); });
    add_method(t, "athrow", [](const CallArgs& a) { return make_asend(a.pos[0], 1, exc_of(a, 1)); });
    add_method(t, "aclose", [](const CallArgs& a) { return make_asend(a.pos[0], 2, None); });
    add_method(t, "__repr__", [](const CallArgs& a) {
        char buf[32];
        snprintf(buf, sizeof buf, "%p", (void*)a.pos[0].ref());
        return new_str("<async_generator object " + G(a.pos[0])->f->qualname + " at 0x" + buf + ">");
    });
    add_getter(t, "ag_running", [](const var& s) { return var(G(s)->state == GenObj::Running); });
    add_getter(t, "ag_await", [](const var& s) { return G(s)->awaiting.is_unbound() ? var(None) : G(s)->awaiting; });
    add_getter(t, "ag_frame", [](const var& s) { return var(None); });
    add_getter(t, "ag_code", [](const var& s) { return var(None); });
    T_async_gen_asend = new_type("async_generator_asend", {}, Layout::NativeData);
    t = T_async_gen_asend;
    auto A = [](const var& v) { return static_cast<AsendObj*>(v.ref()); };
    add_method(t, "__await__", [](const CallArgs& a) { return a.pos[0]; });
    add_method(t, "__iter__", [](const CallArgs& a) { return a.pos[0]; });
    add_method(t, "__next__", [A](const CallArgs& a) { return asend_step(A(a.pos[0]), None, nullptr); });
    add_method(t, "send", [A](const CallArgs& a) { return asend_step(A(a.pos[0]), a.pos.at(1), nullptr); });
    add_method(t, "throw", [A](const CallArgs& a) {
        var e = exc_of(a, 1);
        return asend_step(A(a.pos[0]), None, &e);
    });
    add_method(t, "close", [A](const CallArgs& a) { A(a.pos[0])->state = 2; return var(None); });
}

// the parts other modules show: types.CoroutineType ..., sys's async generator hooks, types.coroutine
var coroutine_type_() { return tv(T_coroutine); }
var async_generator_type_() { return tv(T_async_generator); }
var set_asyncgen_hooks_(const CallArgs& a) {
    var fi = arg(a, 0, "firstiter", var()), fz = arg(a, 1, "finalizer", var());
    if (!fi.is_unbound()) g_firstiter = fi;
    if (!fz.is_unbound()) g_finalizer = fz;
    return None;
}
var get_asyncgen_hooks_() { return new_tuple({g_firstiter, g_finalizer}); }
// @types.coroutine: a generator function whose generators can be awaited
var types_coroutine_(const var& f) {
    if (f.is_ref() && f.ref()->type == T_function) {
        auto* fo = static_cast<FuncObj*>(f.ref());
        if (fo->generator && fo->async_kind == 0) fo->async_kind = 3;
        if (fo->generator) return f;
    }
    var inner = f;
    return new_native("coroutine", [inner](const CallArgs& a) {
        var r = call_obj(inner, a);
        if (r.is_ref() && r.ref()->type == T_generator) G(r)->iterable_coro = true;
        return r;
    });
}
bool is_coroutine_function_(const var& f, int kind) {
    var x = f;
    for (int i = 0; i < 100; i++) {
        if (x.is_ref() && x.ref()->type == T_method) x = static_cast<MethodObj*>(x.ref())->func;
        else break;
    }
    return x.is_ref() && x.ref()->type == T_function && static_cast<FuncObj*>(x.ref())->async_kind == kind;
}

}  // namespace py
