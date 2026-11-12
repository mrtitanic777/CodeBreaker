// cb/verify/concolic.cpp -- see concolic.hpp.
#include "cb/verify/concolic.hpp"

#include <algorithm>
#include <format>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "cb/core/error.hpp"
#include "cb/core/z3_api.hpp"

namespace cb::verify::concolic {

std::string Stats::summary() const {
    const std::string what = complete ? std::format("all {} paths", paths) : std::format("{} paths (sampled)", paths);
    return std::format("{}, {} runs, {} questions ({} impossible)", what, runs, queries, unsat);
}

namespace {

namespace symbolic = ::cb::emulate::symbolic;
using symbolic::Expr;
using symbolic::Op;

// The conditions a question may turn: a jump's, a string instruction's.
bool turned(Kind k) { return k == Kind::Jump || k == Kind::Inner; }

struct TrailHash {
    std::size_t operator()(const Trail& t) const {
        std::size_t h = 1469598103934665603ull;
        for (const Step& s : t) {
            h = (h ^ static_cast<std::size_t>(s.site)) * 1099511628211ull;
            h = (h ^ (s.taken ? 0x9E3779B97F4A7C15ull : 0x7F4A7C15ull)) * 1099511628211ull;
        }
        return h;
    }
};
using TrailSet = std::unordered_set<Trail, TrailHash>;

// One exploration's solver: a context of its own, the terms of the conditions (made once each), and a solver per run.
class Solver {
public:
    Solver() : terms_(context_.handle()) {}
    ~Solver() {
        for (auto& [run, held] : solvers_) release(held.solver);
    }
    Solver(const Solver&) = delete;
    Solver& operator=(const Solver&) = delete;

    // The questions of one run come mostly in the order of its path, each one condition further on, so one solver
    // per run keeps the conditions of its path asserted as far as asked. A question about condition `j` of the run's
    // path: its first `j` conditions as they went, and this one the other way. The inputs that answer it, or the
    // result when they cannot.
    enum class Answer { Sat, Unsat, Unknown };
    struct Result {
        Answer answer = Answer::Unknown;
        Values values;
    };
    Result ask(std::size_t run, const std::vector<PathEntry>& path, std::size_t j, const Vars& vars, std::uint32_t steps) {
        const auto& z = z3::api();
        Z3_context c = context_.handle();
        Held* held = nullptr;
        if (auto it = solvers_.find(run); it != solvers_.end()) held = &it->second;
        if (held == nullptr || held->asserted > j) {
            if (held != nullptr) release(held->solver);
            Held fresh;
            fresh.solver = z.Z3_mk_solver(c);
            z.Z3_solver_inc_ref(c, fresh.solver);
            Z3_params params = z.Z3_mk_params(c);
            z.Z3_params_inc_ref(c, params);
            z.Z3_params_set_uint(c, params, z.Z3_mk_string_symbol(c, "rlimit"), steps);
            z.Z3_solver_set_params(c, fresh.solver, params);
            z.Z3_params_dec_ref(c, params);
            held = &(solvers_[run] = fresh);
        }
        for (std::size_t p = held->asserted; p < j; ++p) z.Z3_solver_assert(c, held->solver, as_went(path[p]));
        held->asserted = j;
        const Z3_solver s = held->solver;
        z.Z3_solver_push(c, s);
        z.Z3_solver_assert(c, s, z.Z3_mk_not(c, as_went(path[j])));
        Result r;
        const Z3_lbool first = z.Z3_solver_check(c, s);
        if (first == Z3_L_TRUE) {
            r.answer = Answer::Sat;
            r.values = model_values(s, vars);
            // (and on the edge: of the inputs that turn it, one where what was compared is what it was compared with,
            // or one past -- a bound written one off is seen there)
            for (Z3_ast edge : edges(path[j].cond)) {
                z.Z3_solver_push(c, s);
                z.Z3_solver_assert(c, s, edge);
                const bool sat = z.Z3_solver_check(c, s) == Z3_L_TRUE;
                if (sat) r.values = model_values(s, vars);
                z.Z3_solver_pop(c, s, 1);
                if (sat) break;
            }
        } else if (first == Z3_L_FALSE) {
            r.answer = Answer::Unsat;
        }
        z.Z3_solver_pop(c, s, 1);
        return r;
    }

private:
    struct Held {
        Z3_solver solver = nullptr;
        std::size_t asserted = 0;   // how many of the run's conditions the solver holds
    };

    void release(Z3_solver s) { z3::api().Z3_solver_dec_ref(context_.handle(), s); }

    // The term of a condition as the run went: itself if the jump was taken, else its negation.
    Z3_ast as_went(const PathEntry& e) {
        Z3_ast t = terms_.term(e.cond);
        return e.taken ? t : z3::api().Z3_mk_not(context_.handle(), t);
    }

    // For a comparison, the inputs at its bound: what was compared equal to what it was compared with, one above, one
    // below (a zero test: the value 1) -- as conditions to try in turn.
    std::vector<Z3_ast> edges(const Expr* cond) {
        const auto& z = z3::api();
        Z3_context c = context_.handle();
        switch (cond->op) {
            case Op::BNot: return edges(cond->arg[0]);
            case Op::Nz: {
                Z3_ast x = terms_.term(cond->arg[0]);
                return {z.Z3_mk_eq(c, x, z.Z3_mk_unsigned_int64(c, 1, z.Z3_get_sort(c, x)))};
            }
            case Op::Eq: case Op::ULt: case Op::ULe: case Op::SLt: case Op::SLe: {
                Z3_ast a = terms_.term(cond->arg[0]);
                Z3_ast b = terms_.term(cond->arg[1]);
                Z3_ast one = z.Z3_mk_unsigned_int64(c, 1, z.Z3_get_sort(c, b));
                return {z.Z3_mk_eq(c, a, b), z.Z3_mk_eq(c, a, z.Z3_mk_bvadd(c, b, one)), z.Z3_mk_eq(c, a, z.Z3_mk_bvsub(c, b, one))};
            }
            default: return {};
        }
    }

    // What the model gives the named inputs that it constrains.
    Values model_values(Z3_solver s, const Vars& vars) {
        const auto& z = z3::api();
        Z3_context c = context_.handle();
        Z3_model m = z.Z3_solver_get_model(c, s);
        z.Z3_model_inc_ref(c, m);
        Values values;
        for (const auto& [name, var] : vars) {
            Z3_ast input = z.Z3_mk_const(c, z.Z3_mk_string_symbol(c, name.c_str()), z.Z3_mk_bv_sort(c, var.bits));
            Z3_ast value = nullptr;
            if (!z.Z3_model_eval(c, m, input, false, &value) || value == nullptr || !z.Z3_is_numeral_ast(c, value)) continue;   // unconstrained
            std::uint64_t v = 0;
            if (z.Z3_get_numeral_uint64(c, value, &v)) values[name] = v;
        }
        z.Z3_model_dec_ref(c, m);
        return values;
    }

    symbolic::Z3Context context_;
    symbolic::Z3Terms terms_;
    std::unordered_map<std::size_t, Held> solvers_;   // by the number of the run whose path they hold
};

struct QueueItem {
    int priority = 0;
    int order = 0;
    std::size_t observation = 0;   // which run's path
    std::size_t index = 0;         // the condition in it
    Trail node;                    // the decisions that lead to it, the last one turned
    bool operator>(const QueueItem& o) const { return std::tie(priority, order) > std::tie(o.priority, o.order); }
};

class Search {
public:
    Search(const Hooks& hooks, const Options& options) : hooks_(hooks), options_(options) {}

    IdExploration run(std::size_t seeds, const std::map<std::size_t, Trail>& known);

private:
    bool own(std::int64_t site) const { return !options_.own || options_.own(site); }

    std::optional<Observation> attempt(std::size_t input) {
        if (stats_.runs >= options_.budget) {
            stats_.budget_spent = true;
            return std::nullopt;
        }
        ++stats_.runs;
        return hooks_.run(input);
    }

    bool took(std::size_t input, Observation o);

    const Hooks& hooks_;
    const Options& options_;
    Stats stats_;
    std::vector<std::size_t> kept_;
    std::set<Step> covered_;       // the function's own jumps, any run
    std::set<Step> other_;         // ... of the code it calls
    TrailSet trails_;              // the paths: own jumps in order
    TrailSet explored_;            // every prefix of decisions some run took
    std::deque<std::pair<std::size_t, Observation>> pool_;   // (input, what its run observed); a deque: indices and references stay
    std::priority_queue<QueueItem, std::vector<QueueItem>, std::greater<QueueItem>> queue_;
    int order_ = 0;
};

// A run's path recorded; -> whether it is a new one.
bool Search::took(std::size_t input, Observation o) {
    Trail trail;
    for (const Step& t : o.trail) {
        if (own(t.site)) trail.push_back(t);
    }
    const bool is_new = trails_.insert(trail).second;
    for (const Step& t : o.trail) (own(t.site) ? covered_ : other_).insert(t);
    Trail decisions;
    for (const PathEntry& e : o.path) {
        if (turned(e.kind)) decisions.push_back({e.site, e.taken});
    }
    for (std::size_t k = 1; k <= decisions.size(); ++k) explored_.insert(Trail(decisions.begin(), decisions.begin() + static_cast<std::ptrdiff_t>(k)));
    stats_.lost.insert(o.lost.begin(), o.lost.end());
    for (const PathEntry& e : o.path) stats_.pins += e.kind == Kind::Pin ? 1 : 0;
    pool_.emplace_back(input, std::move(o));
    const std::size_t at = pool_.size() - 1;
    const std::vector<PathEntry>& path = pool_.back().second.path;
    std::size_t k = 0;
    for (std::size_t j = 0; j < path.size(); ++j) {
        const PathEntry& e = path[j];
        if (!turned(e.kind)) continue;
        const Step flip{e.site, !e.taken};
        Trail node(decisions.begin(), decisions.begin() + static_cast<std::ptrdiff_t>(k));
        node.push_back(flip);
        ++k;
        if (explored_.contains(node)) continue;
        // (a string instruction's count last: another number of passes of the same code)
        int priority = 2;
        if (e.kind == Kind::Inner) priority = 3;
        else if (own(e.site) && !covered_.contains(flip)) priority = 0;
        else if (!covered_.contains(flip) && !other_.contains(flip)) priority = 1;
        queue_.push({priority, ++order_, at, j, std::move(node)});
    }
    return is_new;
}

IdExploration Search::run(std::size_t seeds, const std::map<std::size_t, Trail>& known) {
    // the seeds: of those already run, one per path they took; the others each run
    std::set<Trail> by_trail;
    for (std::size_t i = 0; i < seeds; ++i) {
        if (auto t = known.find(i); t != known.end()) {
            Trail own_steps;
            for (const Step& s : t->second) {
                if (own(s.site)) own_steps.push_back(s);
            }
            if (!by_trail.insert(own_steps).second) continue;
        }
        if (std::optional<Observation> o = attempt(i)) took(i, std::move(*o));
    }
    Solver solver;
    while (!queue_.empty() && kept_.size() < static_cast<std::size_t>(options_.keep)) {
        QueueItem item = queue_.top();
        queue_.pop();
        if (explored_.contains(item.node)) continue;
        // (the pool is a deque: what it holds stays where it is as took() adds to it)
        const std::size_t input = pool_[item.observation].first;
        const Observation& o = pool_[item.observation].second;
        const PathEntry e = o.path[item.index];
        if (item.priority == 0 && covered_.contains({e.site, !e.taken})) {   // (reached meanwhile: now only another path)
            item.priority = 2;
            item.order = ++order_;
            queue_.push(std::move(item));
            continue;
        }
        if (stats_.runs >= options_.budget || stats_.queries >= options_.questions) {
            stats_.budget_spent = true;
            break;
        }
        ++stats_.queries;
        const Solver::Result r = solver.ask(item.observation, o.path, item.index, o.vars, options_.steps);
        explored_.insert(item.node);   // (asked: whatever the answer)
        if (r.answer == Solver::Answer::Unsat) {
            ++stats_.unsat;
            continue;
        }
        if (r.answer != Solver::Answer::Sat) {
            ++stats_.unknown;
            continue;
        }
        const std::optional<std::size_t> made = hooks_.derive(input, r.values, o.vars);
        if (!made) {
            ++stats_.missed;
            continue;
        }
        std::optional<Observation> o2 = attempt(*made);
        if (!o2) {
            ++stats_.missed;
            continue;
        }
        Trail decisions;
        for (const PathEntry& p : o2->path) {
            if (turned(p.kind)) decisions.push_back({p.site, p.taken});
        }
        if (decisions.size() < item.node.size() || !std::equal(item.node.begin(), item.node.end(), decisions.begin())) {
            ++stats_.missed;   // (the run did not go where asked)
        }
        const bool cut = o2->cut;
        if (took(*made, std::move(*o2)) && !cut) kept_.push_back(*made);
    }
    stats_.paths = static_cast<int>(trails_.size());
    stats_.edges = covered_;
    bool queued_all_explored = true;   // (the heap cannot be walked: pop what is left)
    while (!queue_.empty() && queued_all_explored) {
        queued_all_explored = explored_.contains(queue_.top().node);
        queue_.pop();
    }
    stats_.complete = queued_all_explored && !stats_.budget_spent && stats_.unknown == 0 && stats_.missed == 0 && stats_.lost.empty() &&
                      kept_.size() < static_cast<std::size_t>(options_.keep) && !pool_.empty();
    return {std::move(stats_), std::move(kept_)};
}

}  // namespace

IdExploration explore_ids(const Hooks& hooks, std::size_t seeds, const std::map<std::size_t, Trail>& known, const Options& options) {
    CB_ASSERT(hooks.run && hooks.derive, "concolic::explore needs run and derive");
    return Search(hooks, options).run(seeds, known);
}

}  // namespace cb::verify::concolic
