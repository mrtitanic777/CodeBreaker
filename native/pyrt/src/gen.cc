// gen.cc -- generators: each runs its body on a fiber of its own (Windows fibers, pooled), so a
// generator is written as the plain function it is in Python (yield anywhere: in loops, in try /
// finally, in nested calls through yield from). next() switches to it and back; send() and throw()
// as Python's; an unfinished generator that is dropped is closed (GeneratorExit raised at its yield,
// its finally blocks run) before its fiber is reused.
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
    ~GenObj() override;
};

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
    g->type = T_generator;
    g->f = f;
    g->func = var(static_cast<Obj*>(f));
    g->slots = std::move(slots);
    g->y.impl_ = g;
    return var(static_cast<Obj*>(g));
}

// run the generator until its next yield or its end: true with g->value at a yield
static bool resume(GenObj* g) {
    if (g->state == GenObj::Done) return false;
    if (g->state == GenObj::Running) raise(E_ValueError, "generator already executing");
    if (g->state == GenObj::Fresh) {
        g->fiber = get_fiber();
        g->fiber->job = g;
    }
    g->state = GenObj::Running;
    g->fiber->caller = GetCurrentFiber();
    SwitchToFiber(g->fiber->handle);
    if (g->state == GenObj::Done) {
        put_fiber(g->fiber);
        g->fiber = nullptr;
        if (g->exc) {
            std::exception_ptr e = g->exc;
            g->exc = nullptr;
            // a StopIteration escaping a generator is a RuntimeError (PEP 479)
            try {
                std::rethrow_exception(e);
            } catch (error& err) {
                if (isinstance_(err.value, E_StopIteration)) raise(E_RuntimeError, "generator raised StopIteration");
                throw;
            }
        }
        return false;
    }
    return true;
}

// inside the generator: hand v to the caller, wait to be resumed
var yielder::operator()(const var& v) {
    GenObj* g = static_cast<GenObj*>(impl_);
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
var yielder::yield_from(const var& src) {
    // a generator delegates send and throw; any other iterable is iterated
    if (src.is_ref() && src.ref()->type == T_generator) {
        auto* inner = static_cast<GenObj*>(src.ref());
        var sent = None;
        for (;;) {
            inner->sent = sent;
            bool more;
            try {
                more = resume(inner);
            } catch (error& e) {
                throw;
            }
            if (!more) return inner->returned.is_unbound() ? var(None) : inner->returned;
            try {
                sent = (*this)(inner->value);
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
    var it = iter(src);
    var x;
    while (next_(it, x)) (*this)(x);
    return None;
}

bool gen_next_(const var& gv, var& out) {
    auto* g = static_cast<GenObj*>(gv.ref());
    g->sent = None;
    if (!resume(g)) return false;
    out = g->value;
    return true;
}

GenObj::~GenObj() {
    if (state == Suspended && fiber) {
        // close(): GeneratorExit at the yield, the body's finally blocks run
        throw_in = make_exc_args(E_GeneratorExit, {});
        try {
            resume(this);
        } catch (...) {
        }
        if (state != Done) {
            // it yielded again while closing: abandon the fiber (its stack's references leak)
            fiber = nullptr;
        }
    }
    if (fiber) put_fiber(fiber);
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
    add_method(t, "send", [](const CallArgs& a) {
        auto* g = static_cast<GenObj*>(a.pos[0].ref());
        if (g->state == GenObj::Fresh && !a.pos.at(1).is_none()) raise(E_TypeError, "can't send non-None value to a just-started generator");
        g->sent = a.pos.at(1);
        if (!resume(g)) raise_stop(g->returned.is_unbound() ? var(None) : g->returned);
        return g->value;
    });
    add_method(t, "throw", [](const CallArgs& a) {
        auto* g = static_cast<GenObj*>(a.pos[0].ref());
        var e = a.pos.at(1);
        if (e.is_ref() && e.ref()->type->layout == Layout::Type) e = call_type(static_cast<TypeObj*>(e.ref()), CallArgs());
        if (g->state != GenObj::Suspended) {
            g->state = GenObj::Done;
            throw error(e);
        }
        g->throw_in = e;
        if (!resume(g)) raise_stop(g->returned.is_unbound() ? var(None) : g->returned);
        return g->value;
    });
    add_method(t, "close", [](const CallArgs& a) {
        auto* g = static_cast<GenObj*>(a.pos[0].ref());
        if (g->state != GenObj::Suspended) { if (g->state == GenObj::Fresh) g->state = GenObj::Done; return None; }
        g->throw_in = make_exc_args(E_GeneratorExit, {});
        try {
            if (resume(g)) raise(E_RuntimeError, "generator ignored GeneratorExit");
        } catch (error& e) {
            if (!isinstance_(e.value, E_GeneratorExit) && !isinstance_(e.value, E_StopIteration)) throw;
        }
        return None;
    });
}

}  // namespace py
