// gc.cc -- the cycle collector, as CPython's Modules/gcmodule.c: counting references frees what nothing
// points at, this frees what only points at itself (a node and its parent, a nested function that
// calls itself through its own cell, an instance whose bound method it keeps).
//
// The objects that can hold references are tracked from their constructors on (containers, functions,
// cells, bound methods, exceptions, generators; an instance once it has an attribute dict), each in
// the list of its generation. A collection of generation g takes the lists of 0..g and, CPython's
// way, works out which of them only the others point at:
//
//   1. each one's count of references, less those the others hold (gc_traverse says what each holds);
//   2. what is left above zero is pointed at from outside -- a variable, an untracked object, an older
//      generation -- and so is everything reachable from it; the rest is garbage;
//   3. weak references to the garbage die first, and the callbacks of those that are not garbage
//      themselves run; then the finalizers: __del__ of each one not finalized before (once per object,
//      ever), close() of each suspended generator;
//   4. a finalizer may have stored its object somewhere (resurrected it): the count is done again over
//      what is left, and what is now reachable lives on; the rest is cleared (gc_clear lets go of
//      every reference it holds), which breaks the cycles, and counting frees it all.
//
// Survivors move a generation up. Collections are not run inside an allocation (a container is tracked
// before its type and items are set) but at the next safe point: a call of a Python function, where
// everything the running code holds is a counted reference. They run on the thread holding the global
// lock. An object under construction (a count of 0) is never garbage.
//
// Generations, thresholds and the gc module's API are CPython's: 2000 new objects start a collection
// of generation 0, 10 of those one of generation 1, 10 of those a full one -- when the objects that
// would be looked at again are a quarter of the long-lived ones (so full collections stay linear).
//
// The weakref module is here too: weak references that die with their object (their callbacks run),
// weakref.finalize, and the collector clears them before it finalizes anything.
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <unordered_map>

#include "rt.hpp"

namespace py {

void print_exception_(const var& e);   // io.cc
bool gen_finalize_(Obj* g);             // gen.cc: close a suspended generator (false: nothing to do)

bool g_gc_due = false;
static bool g_exiting = false;   // (runtime_exiting_: static destruction frees what is left, running no Python)

namespace {

constexpr int kGenerations = 3;
constexpr int kCollecting = 3;   // the generation number of the objects a collection is looking at

struct GcState {
    std::vector<Obj*> gen[4];                 // 0..2, and 3: those being collected
    int64_t threshold[kGenerations] = {2000, 10, 10};
    int64_t count[kGenerations] = {0, 0, 0};  // [1]: generation 0 collections since generation 1's; [2]: likewise
    size_t frozen = 0;                        // gen[2]'s first `frozen`: the permanent generation (gc.freeze)
    bool enabled = true;
    bool collecting = false;
    int64_t long_lived_total = 0, long_lived_pending = 0;
    int64_t collections[kGenerations] = {0, 0, 0}, collected[kGenerations] = {0, 0, 0};
    int debug = 0;
    var callbacks, garbage;                   // gc.callbacks, gc.garbage (lists, made with the module)
    // scratch of one collection (kept: no allocation per collection once they are big enough)
    std::vector<int64_t> refs;
    std::vector<Obj*> stack;
};
GcState* g_state = nullptr;   // (constant-initialised: objects made during static initialisation are tracked too)
GcState& S() {
    if (!g_state) {
        g_state = new GcState();   // (never freed: objects are untracked during the program's exit)
        // PYRT_GC=0: no collection unless gc.collect() asks (to tell a collector bug from another)
        char e[8];
        size_t n = 0;
        if (getenv_s(&n, e, sizeof e, "PYRT_GC") == 0 && n > 1 && e[0] == '0') g_state->enabled = false;
    }
    return *g_state;
}

inline int gen_of(const Obj* o) { return (int)((o->gc & GC_GEN_MASK) >> GC_GEN_SHIFT); }
inline size_t index_of(const Obj* o) { return o->gc & GC_INDEX_MASK; }
inline void place(Obj* o, int g, size_t i) {
    o->gc = (o->gc & ~(GC_GEN_MASK | GC_INDEX_MASK)) | ((uint32_t)g << GC_GEN_SHIFT) | (uint32_t)i;
}
inline bool collecting_(const Obj* o) { return (o->gc & GC_TRACKED) && gen_of(o) == kCollecting; }
inline void append(GcState& s, int g, Obj* o) {
    place(o, g, s.gen[g].size());
    s.gen[g].push_back(o);
}

}  // namespace

// ------------------------------------------------------------------------------------------ tracking
void gc_track(Obj* o) {
    if (o->gc & GC_TRACKED) return;
    GcState& s = S();
    std::vector<Obj*>& g0 = s.gen[0];
    size_t i = g0.size();
    if (i >= GC_INDEX_MASK) return;   // (past 134 million in one generation: not followed)
    g0.push_back(o);
    o->gc = (o->gc & (GC_FINALIZED | GC_WEAKREFD)) | GC_TRACKED | (uint32_t)i;
    if ((int64_t)i >= s.threshold[0] && s.threshold[0] > 0 && s.enabled && !s.collecting) g_gc_due = true;
}

void gc_untrack(Obj* o) {
    uint32_t w = o->gc;
    if (!(w & GC_TRACKED)) return;
    GcState& s = S();
    int g = gen_of(o);
    size_t i = index_of(o);
    std::vector<Obj*>& v = s.gen[g];
    if (g == 2 && i < s.frozen) {
        // (the permanent generation stays the list's prefix: its last one takes the place)
        size_t f = --s.frozen;
        v[i] = v[f];
        place(v[i], 2, i);
        i = f;
    }
    Obj* last = v.back();
    v.pop_back();
    if (i < v.size()) {
        v[i] = last;
        place(last, g, i);
    }
    o->gc = w & (GC_FINALIZED | GC_WEAKREFD);
}

// ------------------------------------------------------------------------------------------ what objects hold
void Obj::gc_traverse(gc_visit visit, void* arg) {
    if (attrs) visit(attrs, arg);
}
void Obj::gc_clear() {
    if (Obj* a = attrs) {
        attrs = nullptr;
        decref(a);
    }
}
// (each clear moves what it holds out first and lets go of it after: what a release runs -- a __del__,
// a weak reference's callback -- sees the object already empty, never half taken apart)
void ListObj::gc_traverse(gc_visit visit, void* arg) {
    Obj::gc_traverse(visit, arg);
    for (const var& x : v) gc_visit_var(x, visit, arg);
}
void ListObj::gc_clear() {
    std::vector<var> old;
    old.swap(v);
    Obj::gc_clear();
}
void TupleObj::gc_traverse(gc_visit visit, void* arg) {
    Obj::gc_traverse(visit, arg);
    for (const var& x : v) gc_visit_var(x, visit, arg);
}
void TupleObj::gc_clear() {
    std::vector<var> old;
    old.swap(v);
    Obj::gc_clear();
}
void DictObj::gc_traverse(gc_visit visit, void* arg) {
    Obj::gc_traverse(visit, arg);
    for (const Entry& e : entries) {
        gc_visit_var(e.key, visit, arg);
        gc_visit_var(e.value, visit, arg);
    }
    gc_visit_var(default_factory, visit, arg);
}
void DictObj::gc_clear() {
    std::vector<Entry> old;
    old.swap(entries);
    index.assign(8, -1);
    used = 0;
    version++;
    var f = std::move(default_factory);
    Obj::gc_clear();
}
void SetObj::gc_traverse(gc_visit visit, void* arg) {
    Obj::gc_traverse(visit, arg);
    for (const Slot& e : table) gc_visit_var(e.key, visit, arg);
}
void SetObj::gc_clear() {
    std::vector<Slot> old(8);
    old.swap(table);
    fill = used = 0;
    Obj::gc_clear();
}
void FuncObj::gc_traverse(gc_visit visit, void* arg) {
    Obj::gc_traverse(visit, arg);
    for (const var& d : defaults) gc_visit_var(d, visit, arg);
    gc_visit_var(doc, visit, arg);
    gc_visit_var(annotations, visit, arg);
    gc_visit_var(wrapped, visit, arg);
    gc_visit_var(dict_extra, visit, arg);
    for (Obj* c : closure) visit(c, arg);
}
void FuncObj::gc_clear() {
    std::vector<var> old;
    old.swap(defaults);
    defaults.resize(params.size());   // (still one per parameter, all unbound)
    invoker f;
    f.swap(fn);                        // (the lambda, and the cells it captured)
    closure.clear();
    var d = std::move(doc), a = std::move(annotations), w = std::move(wrapped), x = std::move(dict_extra);
    Obj::gc_clear();
}
void NativeObj::gc_traverse(gc_visit visit, void* arg) {
    Obj::gc_traverse(visit, arg);
    gc_visit_var(self, visit, arg);
}
void NativeObj::gc_clear() {
    var s = std::move(self);
    Obj::gc_clear();
}
void MethodObj::gc_traverse(gc_visit visit, void* arg) {
    Obj::gc_traverse(visit, arg);
    gc_visit_var(func, visit, arg);
    gc_visit_var(self, visit, arg);
}
void MethodObj::gc_clear() {
    var f = std::move(func), s = std::move(self);
    Obj::gc_clear();
}
void CellObj::gc_traverse(gc_visit visit, void* arg) {
    Obj::gc_traverse(visit, arg);
    gc_visit_var(v, visit, arg);
}
void CellObj::gc_clear() {
    var x = std::move(v);
    Obj::gc_clear();
}
void ExcObj::gc_traverse(gc_visit visit, void* arg) {
    Obj::gc_traverse(visit, arg);
    gc_visit_var(args, visit, arg);
    gc_visit_var(cause, visit, arg);
    gc_visit_var(context, visit, arg);
    gc_visit_var(traceback, visit, arg);
}
void ExcObj::gc_clear() {
    var a = std::move(args), c = std::move(cause), x = std::move(context), t = std::move(traceback);
    args = new_tuple();
    Obj::gc_clear();
}
void PropertyObj::gc_traverse(gc_visit visit, void* arg) {
    Obj::gc_traverse(visit, arg);
    gc_visit_var(fget, visit, arg);
    gc_visit_var(fset, visit, arg);
    gc_visit_var(fdel, visit, arg);
    gc_visit_var(doc, visit, arg);
}
void PropertyObj::gc_clear() {
    var a = std::move(fget), b = std::move(fset), c = std::move(fdel), d = std::move(doc);
    Obj::gc_clear();
}
void WrapObj::gc_traverse(gc_visit visit, void* arg) {
    Obj::gc_traverse(visit, arg);
    gc_visit_var(f, visit, arg);
}
void WrapObj::gc_clear() {
    var x = std::move(f);
    Obj::gc_clear();
}
void SuperObj::gc_traverse(gc_visit visit, void* arg) {
    Obj::gc_traverse(visit, arg);
    gc_visit_var(cls, visit, arg);
    gc_visit_var(self, visit, arg);
}
void SuperObj::gc_clear() {
    var c = std::move(cls), s = std::move(self);
    Obj::gc_clear();
}
void set_closure_(const var& f, std::vector<Obj*> cells) {
    if (f.is_ref() && f.ref()->type == T_function) static_cast<FuncObj*>(f.ref())->closure = std::move(cells);
}

// ------------------------------------------------------------------------------------------ finalizers
// (where: a printf format of the object's repr, as CPython's sys.unraisablehook writes it)
void write_unraisable_(const char* where, const var& obj, const var& exc) {
    fflush(stdout);
    std::string what;
    try {
        what = repr_(obj);
    } catch (...) {
        what = "<object repr() failed>";
    }
    fprintf(stderr, where, what.c_str());
    fputc('\n', stderr);
    print_exception_(exc);
}

static bool type_has_del(TypeObj* t) {
    extern uint64_t g_type_epoch;
    if (t->del_epoch != g_type_epoch) {
        t->has_del = t->lookup("__del__") != nullptr;
        t->del_epoch = g_type_epoch;
    }
    return t->has_del;
}

// an object's finalizer: __del__ of an instance (once), close() of a suspended generator. false: none ran
static bool run_finalizer(Obj* o) {
    TypeObj* t = o->type;
    if (!t) return false;
    // (a generator is closed whenever it is garbage and suspended -- never cleared while its body could run on)
    if (t->layout == Layout::Generator) return gen_finalize_(o);
    if (o->gc & GC_FINALIZED) return false;
    if (t->builtin || !type_has_del(t)) return false;
    o->gc |= GC_FINALIZED;
    var self(o);
    var m;
    try {
        if (lookup_special(self, "__del__", m)) call0(m);
    } catch (error& e) {
        var f = t->lookup("__del__") ? *t->lookup("__del__") : var(None);
        write_unraisable_("Exception ignored while calling deallocator %s:", f, e.value);
    }
    return true;
}

// an instance with a __del__ whose count reached 0: run it with the object alive again for the while (it
// may store itself somewhere: then it lives on, finalized, and is freed later without a second call)
bool finalize_from_dealloc_(Obj* o) {
    if (g_exiting || !type_has_del(o->type)) return false;
    o->rc = 1;
    run_finalizer(o);
    return --o->rc != 0;
}

// ------------------------------------------------------------------------------------------ weak references
namespace {

struct WeakRefObj : Obj {
    Obj* target = nullptr;            // not counted: cleared when it goes
    var callback;
    var key;                          // (a WeakValueDictionary's: the key its value is under)
    int64_t hash = 0;
    bool hashed = false;
    WeakRefObj() { gc_track(this); }
    ~WeakRefObj() override;
    void gc_traverse(gc_visit visit, void* arg) override {
        Obj::gc_traverse(visit, arg);
        gc_visit_var(callback, visit, arg);
        gc_visit_var(key, visit, arg);
    }
    void gc_clear() override {
        var c = std::move(callback), k = std::move(key);
        Obj::gc_clear();
    }
};
TypeObj* T_weakref = nullptr;

// the weak references to each object that has any (it carries GC_WEAKREFD), oldest first
std::unordered_map<Obj*, std::vector<WeakRefObj*>>& registry() {
    static auto* r = new std::unordered_map<Obj*, std::vector<WeakRefObj*>>();
    return *r;
}

WeakRefObj::~WeakRefObj() {
    if (!target) return;
    auto& reg = registry();
    auto it = reg.find(target);
    if (it != reg.end()) {
        auto& v = it->second;
        v.erase(std::remove(v.begin(), v.end(), this), v.end());
        if (v.empty()) {
            target->gc &= ~GC_WEAKREFD;
            reg.erase(it);
        }
    }
}

// o's weak references die now; those with callbacks are handed back (counted) for the caller to run
void kill_weakrefs(Obj* o, std::vector<var>& with_callbacks, bool skip_collecting) {
    auto& reg = registry();
    auto it = reg.find(o);
    o->gc &= ~GC_WEAKREFD;
    if (it == reg.end()) return;
    std::vector<WeakRefObj*> refs;
    refs.swap(it->second);
    reg.erase(it);
    for (size_t i = refs.size(); i-- > 0;) {   // (newest first, as CPython's list is)
        WeakRefObj* w = refs[i];
        w->target = nullptr;
        if (w->callback.is_unbound() || w->callback.is_none()) continue;
        if (w->rc <= 0) continue;                           // (going itself)
        if (skip_collecting && collecting_(w)) continue;   // (garbage itself: its callback is never called)
        with_callbacks.emplace_back(static_cast<Obj*>(w));
    }
}
void run_weakref_callbacks(std::vector<var>& refs) {
    for (var& w : refs) {
        var cb = static_cast<WeakRefObj*>(w.ref())->callback;
        try {
            call1(cb, w);
        } catch (error& e) {
            write_unraisable_("Exception ignored while calling weakref callback %s:", cb, e.value);
        }
    }
    refs.clear();
}

}  // namespace

void clear_weakrefs_(Obj* o) {
    std::vector<var> cbs;
    kill_weakrefs(o, cbs, false);
    if (g_exiting) return;
    run_weakref_callbacks(cbs);
}

void runtime_exiting_() {
    g_exiting = true;
    g_gc_due = false;
    S().enabled = false;
}

// a weak reference hashes as its object (computed while it lives, kept after)
bool weakref_hash_(Obj* o, int64_t& h) {
    if (!T_weakref || !is_subtype(o->type, T_weakref)) return false;
    auto* w = static_cast<WeakRefObj*>(o);
    if (!w->hashed) {
        if (!w->target) raise(E_TypeError, "weak object has gone away");
        w->hash = hash_(var(w->target));
        w->hashed = true;
    }
    h = w->hash;
    return true;
}
// two weak references are equal when their objects are (both alive), else when they are the same reference
bool weakref_eq_(const var& a, const var& b, bool& eq) {
    if (!T_weakref || !a.is_ref() || !b.is_ref() || !is_subtype(a.ref()->type, T_weakref) || !is_subtype(b.ref()->type, T_weakref))
        return false;
    auto *x = static_cast<WeakRefObj*>(a.ref()), *y = static_cast<WeakRefObj*>(b.ref());
    eq = x->target && y->target ? eq_(var(x->target), var(y->target)) : x == y;
    return true;
}

// ------------------------------------------------------------------------------------------ a collection
namespace {

void visit_decref(Obj* r, void* a) {
    if (collecting_(r)) static_cast<GcState*>(a)->refs[index_of(r)]--;
}
// (refs[i] is set to this once object i is known reachable)
constexpr int64_t kReachable = INT64_MIN;
void visit_reach(Obj* r, void* a) {
    if (!collecting_(r)) return;
    auto* s = static_cast<GcState*>(a);
    int64_t& n = s->refs[index_of(r)];
    if (n == kReachable) return;
    n = kReachable;
    s->stack.push_back(r);
}

// the objects of gen[3] reachable from outside it: refs[i] == kReachable. `held`: references to each of them
// the collection itself holds (not from outside)
void mark_reachable(GcState& s, int64_t held) {
    std::vector<Obj*>& work = s.gen[kCollecting];
    size_t n = work.size();
    s.refs.resize(n);
    for (size_t i = 0; i < n; i++) s.refs[i] = (int64_t)work[i]->rc - held;
    for (size_t i = 0; i < n; i++) work[i]->gc_traverse(visit_decref, &s);
    s.stack.clear();
    for (size_t i = 0; i < n; i++) {
        // (a count above what the others hold is a reference from outside; below it, a holder that says it
        // holds more than it does: kept, to be safe. A count of 0 is an object still being made)
        if (s.refs[i] != 0 || work[i]->rc == 0) {
            if (s.refs[i] == kReachable) continue;
            s.refs[i] = kReachable;
            s.stack.push_back(work[i]);
        }
    }
    while (!s.stack.empty()) {
        Obj* o = s.stack.back();
        s.stack.pop_back();
        o->gc_traverse(visit_reach, &s);
    }
}

// a tuple of plain values only (no item the collector follows) can be in no cycle: it is not followed again,
// as CPython untracks it (most tuples hold strings and numbers)
bool untrackable_tuple(Obj* o) {
    if (!o->type || o->type->layout != Layout::Tuple || o->attrs || o->rc <= 0) return false;
    for (const var& x : static_cast<TupleObj*>(o)->v)
        if (x.is_ref() && gc_is_tracked(x.ref())) return false;
    return true;
}

// gen[3]'s reachable objects to generation g; the rest stay in gen[3] (renumbered). Returns how many moved
size_t move_reachable(GcState& s, int g, bool untrack_tuples) {
    std::vector<Obj*>& work = s.gen[kCollecting];
    std::vector<Obj*> rest;
    size_t moved = 0;
    for (size_t i = 0; i < work.size(); i++) {
        Obj* o = work[i];
        if (s.refs[i] == kReachable) {
            if (untrack_tuples && untrackable_tuple(o)) {
                o->gc &= GC_FINALIZED | GC_WEAKREFD;
                continue;
            }
            append(s, g, o);
            moved++;
        } else {
            place(o, kCollecting, rest.size());
            rest.push_back(o);
        }
    }
    work.swap(rest);
    return moved;
}

void invoke_callbacks(GcState& s, const char* phase, int generation, int64_t collected) {
    if (s.callbacks.is_unbound() || list_items(s.callbacks).empty()) return;
    var info = new_dict();
    info.setitem("generation"_s, var(generation));
    info.setitem("collected"_s, var(collected));
    info.setitem("uncollectable"_s, var(0));
    std::vector<var> cbs = list_items(s.callbacks);   // (a copy: a callback may change the list)
    for (var& cb : cbs) {
        try {
            call2(cb, new_str(phase), info);
        } catch (error& e) {
            write_unraisable_("Exception ignored in garbage collector callback %s:", cb, e.value);
        }
    }
}

// what a collection leaves in gen[3] (an error halfway, an object a clear could not let go of) goes old
void flush_collecting(GcState& s) {
    std::vector<Obj*>& work = s.gen[kCollecting];
    for (Obj* o : work) append(s, 2, o);
    work.clear();
}

int64_t collect(int generation) {
    GcState& s = S();
    if (s.collecting) return 0;
    s.collecting = true;
    g_gc_due = false;
    struct Done {
        GcState& s;
        ~Done() {
            flush_collecting(s);
            s.collecting = false;
            g_gc_due = s.enabled && s.threshold[0] > 0 && (int64_t)s.gen[0].size() > s.threshold[0];
        }
    } done{s};
    invoke_callbacks(s, "start", generation, 0);
    int older = generation < 2 ? generation + 1 : 2;
    // 1. the generations collected, as one list
    std::vector<Obj*>& work = s.gen[kCollecting];
    for (int g = 0; g <= generation; g++) {
        std::vector<Obj*>& v = s.gen[g];
        size_t from = g == 2 ? s.frozen : 0;
        for (size_t i = from; i < v.size(); i++) append(s, kCollecting, v[i]);
        v.resize(from);
    }
    if (generation + 1 < kGenerations) s.count[generation + 1]++;
    for (int g = 0; g <= generation; g++) s.count[g] = 0;
    s.collections[generation]++;
    // 2. what is reachable from outside lives on, a generation older
    mark_reachable(s, 0);
    size_t survived = move_reachable(s, older, true);
    if (generation == 1) s.long_lived_pending += (int64_t)survived;
    if (generation == 2) {
        s.long_lived_pending = 0;
        s.long_lived_total = (int64_t)(s.gen[2].size() - s.frozen);
    }
    int64_t n = 0;
    if (!work.empty()) {
        // the garbage, held while its weak references, finalizers and clears run
        std::vector<var> held;
        held.reserve(work.size());
        for (Obj* o : work) held.emplace_back(o);
        // 3. weak references to it die; callbacks of those that are not garbage themselves run
        std::vector<var> cbs;
        for (var& h : held)
            if (h.ref()->gc & GC_WEAKREFD) kill_weakrefs(h.ref(), cbs, true);
        run_weakref_callbacks(cbs);
        // finalizers, once per object
        bool finalized = false;
        for (var& h : held) finalized |= run_finalizer(h.ref());
        // 4. what a finalizer made reachable again lives on
        if (finalized) {
            mark_reachable(s, 1);
            move_reachable(s, 2, false);
        }
        n = (int64_t)work.size();
        // the rest: each lets go of what it holds, and counting frees it all
        std::vector<Obj*> garbage(work);
        for (Obj* o : garbage)
            if (collecting_(o)) o->gc_clear();
        std::vector<var> release;
        release.swap(held);
        release.clear();
    }
    s.collected[generation] += n;
    invoke_callbacks(s, "stop", generation, n);
    return n;
}

}  // namespace

void gc_collect_due_() {
    g_gc_due = false;
    GcState& s = S();
    if (!s.enabled || s.collecting || s.threshold[0] <= 0 || (int64_t)s.gen[0].size() <= s.threshold[0]) return;
    // the oldest generation past its threshold (a full collection only when it is worth it)
    int generation = 0;
    for (int g = kGenerations - 1; g > 0; g--) {
        if (s.count[g] > s.threshold[g]) {
            if (g == 2 && s.long_lived_pending < s.long_lived_total / 4) continue;
            generation = g;
            break;
        }
    }
    collect(generation);
}

// ------------------------------------------------------------------------------------------ the gc module
namespace {

var objects_of(GcState& s, int g) {
    std::vector<var> out;
    for (Obj* o : s.gen[g])
        if (o->rc > 0 && o->type) out.emplace_back(o);
    return new_list(std::move(out));
}
void visit_collect(Obj* r, void* a) {
    if (r->rc > 0) static_cast<std::vector<var>*>(a)->emplace_back(r);
}
struct Referrers {
    const std::vector<Obj*>* targets;
    bool found;
};
void visit_find(Obj* r, void* a) {
    auto* f = static_cast<Referrers*>(a);
    if (std::find(f->targets->begin(), f->targets->end(), r) != f->targets->end()) f->found = true;
}

void init_gc(const var& m) {
    GcState& s = S();
    if (s.callbacks.is_unbound()) s.callbacks = new_list();
    if (s.garbage.is_unbound()) s.garbage = new_list();
    mod_set(m, "callbacks", s.callbacks);
    mod_set(m, "garbage", s.garbage);
    mod_set(m, "DEBUG_STATS", var(1));
    mod_set(m, "DEBUG_COLLECTABLE", var(2));
    mod_set(m, "DEBUG_UNCOLLECTABLE", var(4));
    mod_set(m, "DEBUG_SAVEALL", var(32));
    mod_set(m, "DEBUG_LEAK", var(38));
    mod_def(m, "collect", [](const CallArgs& a) {
        var g = arg(a, 0, "generation", var(2));
        int64_t gen = g.as_int64();
        if (gen < 0 || gen > 2) raise(E_ValueError, "invalid generation");
        return var(collect((int)gen));
    });
    mod_def(m, "enable", [](const CallArgs&) {
        S().enabled = true;
        return None;
    });
    mod_def(m, "disable", [](const CallArgs&) {
        S().enabled = false;
        g_gc_due = false;
        return None;
    });
    mod_def(m, "isenabled", [](const CallArgs&) { return var(S().enabled); });
    mod_def(m, "get_count", [](const CallArgs&) {
        GcState& s = S();
        return new_tuple({var((int64_t)s.gen[0].size()), var(s.count[1]), var(s.count[2])});
    });
    mod_def(m, "get_threshold", [](const CallArgs&) {
        GcState& s = S();
        return new_tuple({var(s.threshold[0]), var(s.threshold[1]), var(s.threshold[2])});
    });
    mod_def(m, "set_threshold", [](const CallArgs& a) {
        GcState& s = S();
        check_args(a, "set_threshold", 1, 3);
        for (size_t i = 0; i < a.pos.size(); i++) s.threshold[i] = a.pos[i].as_int64();
        return None;
    });
    mod_def(m, "get_objects", [](const CallArgs& a) {
        GcState& s = S();
        var g = arg(a, 0, "generation", None);
        if (!g.is_none()) {
            int64_t gen = g.as_int64();
            if (gen >= 3) raise(E_ValueError, "generation parameter must be less than the number of available generations (3)");
            if (gen < 0) raise(E_ValueError, "generation parameter cannot be negative");
            return objects_of(s, (int)gen);
        }
        var out = new_list();
        for (int i = 0; i < kGenerations; i++) list_items(out).reserve(list_items(out).size() + s.gen[i].size());
        for (int i = 0; i < kGenerations; i++)
            for (Obj* o : s.gen[i])
                if (o->rc > 0 && o->type) list_items(out).emplace_back(o);
        return out;
    });
    mod_def(m, "get_stats", [](const CallArgs&) {
        GcState& s = S();
        std::vector<var> out;
        for (int i = 0; i < kGenerations; i++) {
            var d = new_dict();
            d.setitem("collections"_s, var(s.collections[i]));
            d.setitem("collected"_s, var(s.collected[i]));
            d.setitem("uncollectable"_s, var(0));
            out.push_back(d);
        }
        return new_list(std::move(out));
    });
    mod_def(m, "is_tracked", [](const CallArgs& a) {
        const var& x = a.pos.at(0);
        return var(x.is_ref() && gc_is_tracked(x.ref()));
    });
    mod_def(m, "is_finalized", [](const CallArgs& a) {
        const var& x = a.pos.at(0);
        return var(x.is_ref() && (x.ref()->gc & GC_FINALIZED) != 0);
    });
    mod_def(m, "get_referents", [](const CallArgs& a) {
        std::vector<var> out;
        for (const var& x : a.pos)
            if (x.is_ref() && gc_is_tracked(x.ref())) x.ref()->gc_traverse(visit_collect, &out);
        return new_list(std::move(out));
    });
    mod_def(m, "get_referrers", [](const CallArgs& a) {
        GcState& s = S();
        std::vector<Obj*> targets;
        for (const var& x : a.pos)
            if (x.is_ref()) targets.push_back(x.ref());
        std::vector<var> out;
        for (int i = 0; i < kGenerations; i++)
            for (Obj* o : s.gen[i]) {
                if (o->rc <= 0 || !o->type) continue;
                Referrers f{&targets, false};
                o->gc_traverse(visit_find, &f);
                if (f.found) out.emplace_back(o);
            }
        return new_list(std::move(out));
    });
    // gc.freeze(): everything tracked now goes to the permanent generation, which no collection looks at
    mod_def(m, "freeze", [](const CallArgs&) {
        GcState& s = S();
        for (int g = 0; g < 2; g++) {
            for (Obj* o : s.gen[g]) append(s, 2, o);
            s.gen[g].clear();
        }
        s.frozen = s.gen[2].size();
        s.long_lived_total = 0;
        return None;
    });
    mod_def(m, "unfreeze", [](const CallArgs&) {
        GcState& s = S();
        s.frozen = 0;
        s.long_lived_total = (int64_t)s.gen[2].size();
        return None;
    });
    mod_def(m, "get_freeze_count", [](const CallArgs&) { return var((int64_t)S().frozen); });
    mod_def(m, "set_debug", [](const CallArgs& a) {
        S().debug = (int)a.pos.at(0).as_int64();
        return None;
    });
    mod_def(m, "get_debug", [](const CallArgs&) { return var((int64_t)S().debug); });
}

// ------------------------------------------------------------------------------------------ the weakref module
// (types that cannot be weakly referenced in CPython: the plain builtin values)
bool weakrefable(const var& x) {
    if (!x.is_ref()) return false;
    TypeObj* t = x.ref()->type;
    return !(t == T_int || t == T_str || t == T_bytes || t == T_tuple || t == T_list || t == T_dict || t == T_float ||
             t == T_bool || t == T_none || t == T_complex || t == T_bytearray);
}
WeakRefObj* WR(const var& v) {
    if (!v.is_ref() || !isinstance_(v, T_weakref)) raise(E_TypeError, "a weak reference is expected, not " + tname(v));
    return static_cast<WeakRefObj*>(v.ref());
}
var new_weakref(TypeObj* cls, const var& target, const var& callback) {
    if (!weakrefable(target)) raise(E_TypeError, "cannot create weak reference to '" + tname(target) + "' object");
    Obj* o = target.ref();
    bool plain = callback.is_none() || callback.is_unbound();
    auto& reg = registry();
    if (plain && cls == T_weakref) {
        // (a plain reference is shared, as CPython's: ref(x) is ref(x))
        auto it = reg.find(o);
        if (it != reg.end())
            for (WeakRefObj* w : it->second)
                if (w->type == T_weakref && (w->callback.is_unbound() || w->callback.is_none())) return var(static_cast<Obj*>(w));
    }
    auto* w = new WeakRefObj();
    w->type = cls;
    w->target = o;
    if (!plain) w->callback = callback;
    reg[o].push_back(w);
    o->gc |= GC_WEAKREFD;
    return var(static_cast<Obj*>(w));
}
var deref(const var& w) {
    Obj* t = WR(w)->target;
    return t ? var(t) : var(None);
}

// weakref.finalize: func(*args, **kwargs) once, when obj goes (or at exit, or when called)
TypeObj* T_finalize = nullptr;
struct FinalizeObj : Obj {
    var ref, func, args, kwargs;
    bool alive = false, atexit = true;
    int64_t index = 0;
    FinalizeObj() { gc_track(this); }
    void gc_traverse(gc_visit visit, void* arg) override {
        Obj::gc_traverse(visit, arg);
        gc_visit_var(ref, visit, arg);
        gc_visit_var(func, visit, arg);
        gc_visit_var(args, visit, arg);
        gc_visit_var(kwargs, visit, arg);
    }
    void gc_clear() override {
        var a = std::move(ref), b = std::move(func), c = std::move(args), d = std::move(kwargs);
        alive = false;
        Obj::gc_clear();
    }
};
FinalizeObj* FZ(const var& v) {
    if (!v.is_ref() || !isinstance_(v, T_finalize)) raise(E_TypeError, "a finalize object is expected");
    return static_cast<FinalizeObj*>(v.ref());
}
// the live finalizers by their index (finalize._registry: they live until they run or are detached)
std::unordered_map<int64_t, var>& finalizers() {
    static auto* r = new std::unordered_map<int64_t, var>();
    return *r;
}
int64_t g_finalize_next = 0;
var finalize_take(const var& self) {
    FinalizeObj* f = FZ(self);
    if (!f->alive) return None;
    f->alive = false;
    finalizers().erase(f->index);
    return self;
}
var finalize_run(const var& self) {
    if (finalize_take(self).is_none()) return None;
    FinalizeObj* f = FZ(self);
    var fn = f->func, args = f->args, kwargs = f->kwargs;
    f->func = None;
    f->args = None;
    f->kwargs = None;
    CallArgs c;
    if (!args.is_none()) c.pos = tuple_items(args);
    if (!kwargs.is_none())
        for (auto& e : static_cast<DictObj*>(kwargs.ref())->entries)
            if (!e.key.is_unbound()) c.kw.push_back({e.key, e.value});
    return call_obj(fn, c);
}
void finalize_at_exit() {
    // (the ones still alive whose atexit is set, newest first, as CPython's _exitfunc)
    std::vector<std::pair<int64_t, var>> live(finalizers().begin(), finalizers().end());
    std::sort(live.begin(), live.end(), [](const auto& a, const auto& b) { return a.first > b.first; });
    for (auto& [i, f] : live) {
        if (!FZ(f)->atexit) continue;
        try {
            finalize_run(f);
        } catch (error& e) {
            write_unraisable_("Exception ignored in: %s", f, e.value);
        }
    }
}

// WeakValueDictionary, WeakKeyDictionary, WeakSet: weakref.py's, over the same references. `data` is a dict:
// {key: ref(value)} (each reference knows its key), {ref(key): value}, {ref(item): None}. An entry whose object
// dies is taken out by its reference's callback, which reaches the container through a weak reference to it
// (so the container is not held by what it holds). Keys, values and items are lists of what is alive.
enum WeakKind { WVD, WKD, WSET };
TypeObj* T_weakmap[3] = {};
struct WeakMapObj : Obj {
    int kind = WVD;
    var data;                         // the dict
    var remove;                       // the callback its references share
    WeakMapObj() { gc_track(this); }
    void gc_traverse(gc_visit visit, void* arg) override {
        Obj::gc_traverse(visit, arg);
        gc_visit_var(data, visit, arg);
        gc_visit_var(remove, visit, arg);
    }
    void gc_clear() override {
        var d = std::move(data), r = std::move(remove);
        data = new_dict();
        Obj::gc_clear();
    }
};
WeakMapObj* WM(const var& v) {
    if (!v.is_ref() || v.ref()->type->layout != Layout::NativeData ||
        !(isinstance_(v, T_weakmap[WVD]) || isinstance_(v, T_weakmap[WKD]) || isinstance_(v, T_weakmap[WSET])))
        raise(E_TypeError, "a weak container is expected, not " + tname(v));
    return static_cast<WeakMapObj*>(v.ref());
}
DictObj* DATA(const var& v) { return static_cast<DictObj*>(WM(v)->data.ref()); }
Obj* alive_target(const var& ref) { return static_cast<WeakRefObj*>(ref.ref())->target; }

var weakmap_new(int kind, TypeObj* cls) {
    auto* w = new WeakMapObj();
    w->type = cls;
    w->kind = kind;
    var self(static_cast<Obj*>(w));
    w->data = new_dict();
    var selfref = new_weakref(T_weakref, self, None);
    w->remove = new_native("_remove", [selfref](const CallArgs& a) -> var {
        Obj* c = WR(selfref)->target;
        if (!c) return None;
        auto* m = static_cast<WeakMapObj*>(c);
        auto* d = static_cast<DictObj*>(m->data.ref());
        const var& wr = a.pos.at(0);
        if (m->kind == WVD) {
            var k = WR(wr)->key;
            var* cur = d->get(k);
            if (cur && cur->ref() == wr.ref()) d->del(k);
        } else {
            d->del(wr);   // (its hash was kept while its object lived; a dead reference equals only itself)
        }
        return None;
    });
    return self;
}
// the entries whose objects live, in order: (key, value) as the container's user sees them
std::vector<std::pair<var, var>> live_items(const var& self) {
    WeakMapObj* w = WM(self);
    std::vector<std::pair<var, var>> out;
    for (auto& e : static_cast<DictObj*>(w->data.ref())->entries) {
        if (e.key.is_unbound()) continue;
        if (w->kind == WVD) {
            if (Obj* o = alive_target(e.value)) out.push_back({e.key, var(o)});
        } else if (Obj* o = alive_target(e.key)) {
            out.push_back({var(o), e.value});
        }
    }
    return out;
}
var plain_ref(const var& x) { return new_weakref(T_weakref, x, None); }
// the data key of x in a WeakKeyDictionary / WeakSet (null: x cannot be weakly referenced, so it is not in it)
bool data_key(const var& x, var& out) {
    if (!weakrefable(x)) return false;
    out = plain_ref(x);
    return true;
}
void weakmap_set(const var& self, const var& k, const var& v) {
    WeakMapObj* w = WM(self);
    auto* d = static_cast<DictObj*>(w->data.ref());
    if (w->kind == WVD) {
        var r = new_weakref(T_weakref, v, w->remove);
        static_cast<WeakRefObj*>(r.ref())->key = k;
        d->set(k, r);
    } else {
        d->set(new_weakref(T_weakref, k, w->remove), w->kind == WSET ? var(None) : v);
    }
}
// the value under k (false: none, or its object is gone)
bool weakmap_get(const var& self, const var& k, var& out) {
    WeakMapObj* w = WM(self);
    auto* d = static_cast<DictObj*>(w->data.ref());
    if (w->kind == WVD) {
        var* r = d->get(k);
        Obj* o = r ? alive_target(*r) : nullptr;
        if (!o) return false;
        out = var(o);
        return true;
    }
    var key;
    if (!data_key(k, key)) return false;
    var* v = d->get(key);
    if (!v) return false;
    out = *v;
    return true;
}
bool weakmap_del(const var& self, const var& k) {
    WeakMapObj* w = WM(self);
    auto* d = static_cast<DictObj*>(w->data.ref());
    if (w->kind == WVD) return d->del(k);
    var key;
    return data_key(k, key) && d->del(key);
}
void weakmap_update(const var& self, const CallArgs& a, size_t from) {
    if (a.pos.size() > from && !a.pos[from].is_none()) {
        const var& other = a.pos[from];
        if (WM(self)->kind == WSET) {
            for (var x : each(other)) weakmap_set(self, x, None);
        } else if (hasattr(other, "items"_s).truthy()) {
            for (var kv : each(other.call("items"))) {
                auto p = unpack(kv, 2);
                weakmap_set(self, p[0], p[1]);
            }
        } else {
            for (var kv : each(other)) {
                auto p = unpack(kv, 2);
                weakmap_set(self, p[0], p[1]);
            }
        }
    }
    for (auto& [k, v] : a.kw) weakmap_set(self, k, v);
}
var weakmap_repr(const var& self) {
    static const char* names[] = {"WeakValueDictionary", "WeakKeyDictionary", "_weakrefset.WeakSet object"};
    char b[96];
    snprintf(b, sizeof b, "<%s at %p>", names[WM(self)->kind], (void*)self.ref());
    return new_str(b);
}

void init_weak_containers() {
    if (T_weakmap[WVD]) return;
    const char* names[] = {"WeakValueDictionary", "WeakKeyDictionary", "WeakSet"};
    for (int kind = 0; kind < 3; kind++) {
        TypeObj* t = T_weakmap[kind] = new_type(names[kind], {}, Layout::NativeData, true, "weakref");
        add_static(t, "__new__", [kind](const CallArgs& a) { return weakmap_new(kind, T(a.pos.at(0))); });
        add_method(t, "__init__", [](const CallArgs& a) {
            weakmap_update(a.pos[0], a, 1);
            return None;
        });
        add_method(t, "__len__", [](const CallArgs& a) { return var((int64_t)live_items(a.pos[0]).size()); });
        add_method(t, "__contains__", [](const CallArgs& a) {
            var v;
            return var(weakmap_get(a.pos[0], a.pos.at(1), v));
        });
        add_method(t, "__iter__", [](const CallArgs& a) {
            std::vector<var> keys;
            for (auto& [k, v] : live_items(a.pos[0])) keys.push_back(k);
            return iter(new_list(std::move(keys)));
        });
        add_method(t, "clear", [](const CallArgs& a) {
            DATA(a.pos[0])->clear();
            return None;
        });
        add_method(t, "update", [](const CallArgs& a) {
            weakmap_update(a.pos[0], a, 1);
            return None;
        });
        add_method(t, "copy", [kind](const CallArgs& a) {
            var c = weakmap_new(kind, a.pos[0].ref()->type);
            for (auto& [k, v] : live_items(a.pos[0])) weakmap_set(c, k, v);
            return c;
        });
        add_method(t, "__copy__", [kind](const CallArgs& a) {
            var c = weakmap_new(kind, a.pos[0].ref()->type);
            for (auto& [k, v] : live_items(a.pos[0])) weakmap_set(c, k, v);
            return c;
        });
        add_method(t, "__repr__", [](const CallArgs& a) { return weakmap_repr(a.pos[0]); });
        add_classmethod(t, "__class_getitem__", [](const CallArgs& a) { return a.pos[0]; });
        if (kind == WSET) {
            add_method(t, "add", [](const CallArgs& a) {
                weakmap_set(a.pos[0], a.pos.at(1), None);
                return None;
            });
            add_method(t, "discard", [](const CallArgs& a) {
                weakmap_del(a.pos[0], a.pos.at(1));
                return None;
            });
            add_method(t, "remove", [](const CallArgs& a) {
                if (!weakmap_del(a.pos[0], a.pos.at(1))) raise(make_exc_args(E_KeyError, {a.pos[1]}));
                return None;
            });
            add_method(t, "pop", [](const CallArgs& a) -> var {
                for (auto& [k, v] : live_items(a.pos[0])) {
                    weakmap_del(a.pos[0], k);
                    return k;
                }
                raise(E_KeyError, "pop from empty WeakSet");
            });
            continue;
        }
        add_method(t, "__getitem__", [](const CallArgs& a) {
            var v;
            if (!weakmap_get(a.pos[0], a.pos.at(1), v)) raise(make_exc_args(E_KeyError, {a.pos[1]}));
            return v;
        });
        add_method(t, "__setitem__", [](const CallArgs& a) {
            weakmap_set(a.pos[0], a.pos.at(1), a.pos.at(2));
            return None;
        });
        add_method(t, "__delitem__", [](const CallArgs& a) {
            if (!weakmap_del(a.pos[0], a.pos.at(1))) raise(make_exc_args(E_KeyError, {a.pos[1]}));
            return None;
        });
        add_method(t, "get", [](const CallArgs& a) {
            var v;
            return weakmap_get(a.pos[0], a.pos.at(1), v) ? v : (a.pos.size() > 2 ? a.pos[2] : var(None));
        });
        add_method(t, "setdefault", [](const CallArgs& a) {
            var v;
            if (weakmap_get(a.pos[0], a.pos.at(1), v)) return v;
            var d = a.pos.size() > 2 ? a.pos[2] : var(None);
            weakmap_set(a.pos[0], a.pos[1], d);
            return d;
        });
        add_method(t, "pop", [](const CallArgs& a) {
            var v;
            bool got = weakmap_get(a.pos[0], a.pos.at(1), v);
            weakmap_del(a.pos[0], a.pos[1]);
            if (got) return v;
            if (a.pos.size() > 2) return a.pos[2];
            raise(make_exc_args(E_KeyError, {a.pos[1]}));
        });
        add_method(t, "popitem", [](const CallArgs& a) -> var {
            auto items = live_items(a.pos[0]);
            if (items.empty()) raise(E_KeyError, "popitem(): dictionary is empty");
            auto& [k, v] = items.back();
            weakmap_del(a.pos[0], k);
            return new_tuple({k, v});
        });
        add_method(t, "keys", [](const CallArgs& a) {
            std::vector<var> out;
            for (auto& [k, v] : live_items(a.pos[0])) out.push_back(k);
            return new_list(std::move(out));
        });
        add_method(t, "values", [](const CallArgs& a) {
            std::vector<var> out;
            for (auto& [k, v] : live_items(a.pos[0])) out.push_back(v);
            return new_list(std::move(out));
        });
        add_method(t, "items", [](const CallArgs& a) {
            std::vector<var> out;
            for (auto& [k, v] : live_items(a.pos[0])) out.push_back(new_tuple({k, v}));
            return new_list(std::move(out));
        });
        // the references themselves: valuerefs() of a WeakValueDictionary, keyrefs() of a WeakKeyDictionary
        auto refs = [](const CallArgs& a) {
            std::vector<var> out;
            bool values = WM(a.pos[0])->kind == WVD;
            for (auto& e : DATA(a.pos[0])->entries)
                if (!e.key.is_unbound()) out.push_back(values ? e.value : e.key);
            return new_list(std::move(out));
        };
        add_method(t, kind == WVD ? "valuerefs" : "keyrefs", refs);
        add_method(t, kind == WVD ? "itervaluerefs" : "iterkeyrefs", [refs](const CallArgs& a) { return iter(refs(a)); });
    }
}

void init_weakref(const var& m) {
    if (!T_weakref) {
        T_weakref = new_type("ReferenceType", {}, Layout::NativeData, true, "weakref");
        add_static(T_weakref, "__new__", [](const CallArgs& a) {
            check_args(a, "ref", 2, 3);
            return new_weakref(T(a.pos[0]), a.pos[1], a.pos.size() > 2 ? a.pos[2] : var(None));
        });
        add_method(T_weakref, "__init__", [](const CallArgs&) { return None; });
        add_method(T_weakref, "__call__", [](const CallArgs& a) { return deref(a.pos[0]); });
        add_method(T_weakref, "__hash__", [](const CallArgs& a) {
            int64_t h = 0;
            weakref_hash_(WR(a.pos[0]), h);
            return var(h);
        });
        add_method(T_weakref, "__eq__", [](const CallArgs& a) -> var {
            if (!a.pos.at(1).is_ref() || !isinstance_(a.pos[1], T_weakref)) return NotImplemented;
            WeakRefObj *x = WR(a.pos[0]), *y = WR(a.pos[1]);
            if (x->target && y->target) return eq(var(x->target), var(y->target));
            return var(x == y);
        });
        add_method(T_weakref, "__ne__", [](const CallArgs& a) -> var {
            if (!a.pos.at(1).is_ref() || !isinstance_(a.pos[1], T_weakref)) return NotImplemented;
            WeakRefObj *x = WR(a.pos[0]), *y = WR(a.pos[1]);
            if (x->target && y->target) return var(!eq_(var(x->target), var(y->target)));
            return var(x != y);
        });
        add_method(T_weakref, "__repr__", [](const CallArgs& a) {
            WeakRefObj* w = WR(a.pos[0]);
            char b[64];
            snprintf(b, sizeof b, "<weakref at %p; ", (void*)w);
            std::string s = b;
            if (!w->target) return new_str(s + "dead>");
            snprintf(b, sizeof b, "' at %p>", (void*)w->target);
            return new_str(s + "to '" + w->target->type->name + b);
        });
        add_getter(T_weakref, "__callback__", [](const var& self) {
            WeakRefObj* w = WR(self);
            return w->callback.is_unbound() ? var(None) : w->callback;
        });
        add_classmethod(T_weakref, "__class_getitem__", [](const CallArgs& a) { return a.pos[0]; });

        T_finalize = new_type("finalize", {}, Layout::NativeData, true, "weakref");
        add_static(T_finalize, "__new__", [](const CallArgs& a) {
            if (a.pos.size() < 3) raise(E_TypeError, "finalize expected at least 2 positional arguments, got " + std::to_string(a.pos.size() ? a.pos.size() - 1 : 0));
            auto* f = new FinalizeObj();
            f->type = T(a.pos[0]);
            var self(static_cast<Obj*>(f));
            f->func = a.pos[2];
            f->args = new_tuple(std::vector<var>(a.pos.begin() + 3, a.pos.end()));
            var kw = new_dict();
            for (auto& [k, v] : a.kw) kw.setitem(k, v);
            f->kwargs = kw;
            f->index = g_finalize_next++;
            int64_t index = f->index;
            // (the callback names the finalizer by its index: holding it would make a cycle through the reference)
            f->ref = new_weakref(T_weakref, a.pos[1], new_native("finalize_callback", [index](const CallArgs&) -> var {
                auto it = finalizers().find(index);
                if (it == finalizers().end()) return None;
                var fz = it->second;
                return finalize_run(fz);
            }));
            f->alive = true;
            static bool hooked = false;
            if (!hooked) {
                hooked = true;
                import("atexit").call("register", new_native("_exitfunc", [](const CallArgs&) {
                    finalize_at_exit();
                    return None;
                }));
            }
            finalizers()[index] = self;
            return self;
        });
        add_method(T_finalize, "__init__", [](const CallArgs&) { return None; });
        add_method(T_finalize, "__call__", [](const CallArgs& a) { return finalize_run(a.pos[0]); });
        add_method(T_finalize, "detach", [](const CallArgs& a) -> var {
            FinalizeObj* f = FZ(a.pos[0]);
            var obj = f->alive ? deref(f->ref) : var(None);
            if (obj.is_none() || finalize_take(a.pos[0]).is_none()) return None;
            return new_tuple({obj, f->func, f->args, f->kwargs});
        });
        add_method(T_finalize, "peek", [](const CallArgs& a) -> var {
            FinalizeObj* f = FZ(a.pos[0]);
            var obj = f->alive ? deref(f->ref) : var(None);
            if (obj.is_none()) return None;
            return new_tuple({obj, f->func, f->args, f->kwargs});
        });
        add_getter(T_finalize, "alive", [](const var& self) { return var(FZ(self)->alive); });
        add_method(T_finalize, "__repr__", [](const CallArgs& a) {
            FinalizeObj* f = FZ(a.pos[0]);
            var obj = f->alive ? deref(f->ref) : var(None);
            char b[64];
            snprintf(b, sizeof b, "<finalize object at %p; ", (void*)f);
            std::string s = b;
            if (obj.is_none()) return new_str(s + "dead>");
            snprintf(b, sizeof b, "' at %p>", (void*)obj.ref());
            return new_str(s + "for '" + tname(obj) + b);
        });
    }
    mod_set(m, "ref", tv(T_weakref));
    mod_set(m, "ReferenceType", tv(T_weakref));
    mod_set(m, "finalize", tv(T_finalize));
    mod_def(m, "getweakrefcount", [](const CallArgs& a) {
        const var& x = a.pos.at(0);
        if (!x.is_ref()) return var(0);
        auto it = registry().find(x.ref());
        return var((int64_t)(it == registry().end() ? 0 : it->second.size()));
    });
    mod_def(m, "getweakrefs", [](const CallArgs& a) {
        const var& x = a.pos.at(0);
        std::vector<var> out;
        if (x.is_ref()) {
            auto it = registry().find(x.ref());
            if (it != registry().end())
                for (WeakRefObj* w : it->second) out.emplace_back(static_cast<Obj*>(w));
        }
        return new_list(std::move(out));
    });
    // (a proxy is the object itself here: it holds strongly)
    mod_def(m, "proxy", [](const CallArgs& a) { return a.pos.at(0); });
    mod_set(m, "ProxyType", t::function);
    mod_set(m, "CallableProxyType", t::function);
    init_weak_containers();
    mod_set(m, "WeakValueDictionary", tv(T_weakmap[WVD]));
    mod_set(m, "WeakKeyDictionary", tv(T_weakmap[WKD]));
    mod_set(m, "WeakSet", tv(T_weakmap[WSET]));
}

}  // namespace

void init_gc_modules() {
    register_native_module("gc", init_gc);
    register_native_module("weakref", init_weakref);
    register_native_module("_weakref", init_weakref);
}

}  // namespace py
