// decompose.cpp -- DominatorEngine, DominatorTreeExceptionFilter, FastExtendedPostdominanceHelper. Ported from Fernflower
// (de.fernflower.modules.decompiler.decompose), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <algorithm>
#include <deque>
#include <functional>
#include <stdexcept>
#include <unordered_map>

#include "java/decompiler/decompose.hpp"
#include "java/decompiler/fast_sets.hpp"

namespace cb::java::decompiler {

// ---- DominatorEngine

void DominatorEngine::order_statements() {
    for (Statement* stat : statement_->get_reverse_post_order_list()) {
        col_ordered_idoms_.add_with_key(0, stat->id);
    }
}

int DominatorEngine::get_common_idom(int key1, int key2, const KeyedList<int, int>& ordered_idoms) {
    if (key1 == 0) {
        return key2;
    } else if (key2 == 0) {
        return key1;
    }

    int index1 = static_cast<int>(ordered_idoms.index_by_key(key1));
    int index2 = static_cast<int>(ordered_idoms.index_by_key(key2));

    while (index1 != index2) {
        if (index1 > index2) {
            key1 = ordered_idoms.get_with_key(key1);
            index1 = static_cast<int>(ordered_idoms.index_by_key(key1));
        } else {
            key2 = ordered_idoms.get_with_key(key2);
            index2 = static_cast<int>(ordered_idoms.index_by_key(key2));
        }
    }

    return key1;
}

void DominatorEngine::calc_idoms() {
    order_statements();

    col_ordered_idoms_.put_with_key(statement_->get_first()->id, statement_->get_first()->id);

    // exclude first statement
    std::vector<int> lst_ids;
    for (std::size_t i = 1; i < col_ordered_idoms_.keys().size(); i++) lst_ids.push_back(*col_ordered_idoms_.key(i));

    while (true) {
        bool changed = false;

        for (const int id : lst_ids) {
            Statement* stat = statement_->get_stats().get_with_key(id);
            int idom = 0;

            for (StatEdge* edge : stat->get_all_predecessor_edges()) {
                if (col_ordered_idoms_.get_with_key(edge->get_source()->id) != 0) {
                    idom = get_common_idom(idom, edge->get_source()->id, col_ordered_idoms_);
                }
            }

            if (idom == 0) throw std::runtime_error("NullPointerException: no immediate dominator");

            const std::optional<int> oldidom = col_ordered_idoms_.put_with_key(idom, id);
            if (!oldidom || *oldidom != idom) changed = true;
        }

        if (!changed) break;
    }
}

bool DominatorEngine::is_dominator(int node, int dom) const {
    while (node != dom) {
        const int idom = col_ordered_idoms_.get_with_key(node);

        if (idom == 0) throw std::runtime_error("NullPointerException: no immediate dominator");

        if (idom == node) {
            return false;   // root node
        } else {
            node = idom;
        }
    }

    return true;
}

// ---- DominatorTreeExceptionFilter

void DominatorTreeExceptionFilter::initialize() {
    dom_engine_ = std::make_unique<DominatorEngine>(statement_);
    dom_engine_->initialize();

    build_dominator_tree();
    build_exception_ranges();

    build_filter(statement_->get_first()->id);

    // free resources
    map_tree_branches_.clear();
    map_exception_ranges_.clear();
}

bool DominatorTreeExceptionFilter::accept_statement_pair(int head, int exit) const {
    const IdMapH<int>* filter = map_exception_range_unique_exit_.get(head);
    if (filter == nullptr) throw std::runtime_error("NullPointerException: no filter");

    for (const auto& [handler, filter_exit] : filter->entries()) {
        const int* dom = map_exception_doms_.get(handler);
        if (!(dom != nullptr && *dom == head)) {
            if (filter_exit == -1 || filter_exit != exit) return false;
        }
    }

    return true;
}

void DominatorTreeExceptionFilter::build_dominator_tree() {
    KeyedList<int, int>& ordered_idoms = dom_engine_->get_ordered_idoms();

    const std::vector<std::optional<int>>& lst_keys = ordered_idoms.keys();
    for (int index = static_cast<int>(lst_keys.size()) - 1; index >= 0; index--) {
        const int key = *lst_keys[static_cast<std::size_t>(index)];
        const int idom = ordered_idoms.get(static_cast<std::size_t>(index));

        IdSetH* set = map_tree_branches_.get(idom);
        if (set == nullptr) {
            map_tree_branches_.put(idom, IdSetH());
            set = map_tree_branches_.get(idom);
        }

        set->add(key);
    }

    const int firstid = statement_->get_first()->id;
    map_tree_branches_.get(firstid)->remove(firstid);
}

void DominatorTreeExceptionFilter::build_exception_ranges() {
    for (Statement* stat : statement_->get_stats()) {
        const std::vector<Statement*> lst_preds = stat->get_neighbours(StatEdge::TYPE_EXCEPTION, Statement::DIRECTION_BACKWARD);

        if (!lst_preds.empty()) {
            IdSetH set;
            for (Statement* st : lst_preds) set.add(st->id);

            map_exception_ranges_.put(stat->id, set);
        }
    }

    map_exception_doms_ = build_exception_doms(statement_->get_first()->id);
}

IdMapH<int> DominatorTreeExceptionFilter::build_exception_doms(int id) {
    IdMapH<int> map;

    const IdSetH* children_set = map_tree_branches_.get(id);
    if (children_set != nullptr) {
        const std::vector<int> children = children_set->items();

        for (const int childid : children) {
            IdMapH<int> map_child = build_exception_doms(childid);

            for (const int handler : map_child.keys()) {
                map.put(handler, map.contains_key(handler) ? id : *map_child.get(handler));
            }
        }
    }

    for (const auto& [handler, range] : map_exception_ranges_.entries()) {
        if (range.contains(id)) map.put(handler, id);
    }

    return map;
}

void DominatorTreeExceptionFilter::build_filter(int id) {
    IdMapH<int> map;

    const IdSetH* children_set = map_tree_branches_.get(id);
    if (children_set != nullptr) {
        const std::vector<int> children = children_set->items();

        for (const int childid : children) {
            build_filter(childid);

            const IdMapH<int>* map_child = map_exception_range_unique_exit_.get(childid);

            for (const auto& [handler, range] : map_exception_ranges_.entries()) {
                if (range.contains(id)) {
                    std::optional<int> exit;

                    if (!range.contains(childid)) {
                        exit = childid;
                    } else {
                        if (map.contains_key(handler)) {
                            exit = -1;
                        } else {
                            const int* e = map_child->get(handler);
                            if (e != nullptr) exit = *e;
                        }
                    }

                    if (exit) map.put(handler, *exit);
                }
            }
        }
    }

    map_exception_range_unique_exit_.put(id, map);
}

// ---- FastExtendedPostdominanceHelper

struct FastExtendedPostdominanceHelper::Impl {
    using SetMap = std::unordered_map<int, FastFixedSet<int>*>;   // (a null value is kept: the original's containsKey is true for it)
    using Action = std::function<bool(Statement*, SetMap&)>;

    std::vector<Statement*> lst_reverse_post_order;
    IdMapH<FastFixedSet<int>*> map_support_points;
    LinkedMap<int, FastFixedSet<int>*> map_ext_postdominators;
    Statement* statement = nullptr;
    FastFixedSetFactory<int>* factory = nullptr;

    static bool equal_sets(const FastFixedSet<int>* a, const FastFixedSet<int>* b) {
        if (a == nullptr) return b == nullptr;
        if (a == b) return true;
        return b != nullptr && a->equals(b);
    }

    ExtPostMap get_extended_postdominators(Statement* st) {
        statement = st;

        std::vector<int> set;
        for (Statement* s : statement->get_stats()) {
            if (std::find(set.begin(), set.end(), s->id) == set.end()) set.push_back(s->id);
        }
        factory = make<FastFixedSetFactory<int>>(set);

        lst_reverse_post_order = statement->get_reverse_post_order_list();

        calc_default_reachable_sets();

        remove_erroneous_nodes();

        DominatorTreeExceptionFilter filter(statement);
        filter.initialize();

        filter_on_exception_ranges(filter);

        filter_on_dominance(filter);

        ExtPostMap res(static_cast<int>(map_ext_postdominators.size()));

        for (const auto& [key, value] : map_ext_postdominators) {
            std::vector<int> lst = value->to_plain_set().items();
            std::sort(lst.begin(), lst.end());   // Order Matters!

            res.put(key, IdSet::linked_of(lst));
        }

        return res;
    }

    void filter_on_dominance(DominatorTreeExceptionFilter& filter) {
        DominatorEngine* engine = filter.get_dom_engine();

        std::deque<Statement*> stack;
        std::deque<FastFixedSet<int>*> stack_path;

        LinkedSet<Statement*> set_visited;

        for (const int head : map_ext_postdominators.keys()) {
            FastFixedSet<int>* set_postdoms = *map_ext_postdominators.get(head);

            stack.clear();
            stack_path.clear();

            stack.push_back(statement->get_stats().get_with_key(head));
            stack_path.push_back(factory->create_empty_set());

            set_visited.clear();
            set_visited.add(stack.front());

            while (!stack.empty()) {
                Statement* stat = stack.front();
                stack.pop_front();
                FastFixedSet<int>* path = stack_path.front();
                stack_path.pop_front();

                if (set_postdoms->contains(stat->id)) path->add(stat->id);

                if (path->contains(set_postdoms)) continue;

                if (!engine->is_dominator(stat->id, head)) {
                    set_postdoms->complement(path);
                    continue;
                }

                for (StatEdge* edge : stat->get_successor_edges(StatEdge::TYPE_REGULAR)) {
                    Statement* edge_destination = edge->get_destination();

                    if (!set_visited.contains(edge_destination)) {
                        stack.push_back(edge_destination);
                        stack_path.push_back(path->get_copy());

                        set_visited.add(edge_destination);
                    }
                }
            }

            if (set_postdoms->is_empty()) map_ext_postdominators.remove(head);
        }
    }

    void filter_on_exception_ranges(DominatorTreeExceptionFilter& filter) {
        for (const int head : map_ext_postdominators.keys()) {
            FastFixedSet<int>* set = *map_ext_postdominators.get(head);

            for (FastFixedSetIterator<int> it = set->iterator(); it.has_next();) {
                if (!filter.accept_statement_pair(head, it.next())) it.remove();
            }

            if (set->is_empty()) map_ext_postdominators.remove(head);
        }
    }

    void remove_erroneous_nodes() {
        map_support_points = IdMapH<FastFixedSet<int>*>();
        calc_reachability_supp_points(StatEdge::TYPE_REGULAR);

        iterate_reachability(
            [this](Statement* node, SetMap& map_sets) {
                const int nodeid = node->id;

                FastFixedSet<int>* set_reachability = map_sets.at(nodeid);

                std::vector<FastFixedSet<int>*> lst_pred_sets;

                for (StatEdge* prededge : node->get_predecessor_edges(StatEdge::TYPE_REGULAR)) {
                    FastFixedSet<int>* set_pred = nullptr;
                    if (const auto it = map_sets.find(prededge->get_source()->id); it != map_sets.end()) set_pred = it->second;
                    if (set_pred == nullptr) {
                        if (FastFixedSet<int>** sp = map_support_points.get(prededge->get_source()->id)) set_pred = *sp;
                    }

                    // setPred cannot be empty as it is a reachability set
                    lst_pred_sets.push_back(set_pred);
                }

                for (FastFixedSetIterator<int> it = set_reachability->iterator(); it.has_next();) {
                    const int id = it.next();

                    FastFixedSet<int>* set_reachability_copy = set_reachability->get_copy();

                    FastFixedSet<int>* set_intersection = factory->create_empty_set();
                    bool is_intersection_initialized = false;

                    for (FastFixedSet<int>* predset : lst_pred_sets) {
                        if (predset->contains(id)) {
                            if (!is_intersection_initialized) {
                                set_intersection->union_(predset);
                                is_intersection_initialized = true;
                            } else {
                                set_intersection->intersection(predset);
                            }
                        }
                    }

                    if (nodeid != id) {
                        set_intersection->add(nodeid);
                    } else {
                        set_intersection->remove(nodeid);
                    }

                    set_reachability_copy->complement(set_intersection);

                    (*map_ext_postdominators.get(id))->complement(set_reachability_copy);
                }

                return false;
            },
            StatEdge::TYPE_REGULAR);

        // exception handlers cannot be postdominator nodes
        // TODO: replace with a standard set?
        FastFixedSet<int>* set_handlers = factory->create_empty_set();
        bool handlerfound = false;

        for (Statement* stat : statement->get_stats()) {
            if (stat->get_predecessor_edges(Statement::STATEDGE_DIRECT_ALL).empty() &&
                !stat->get_predecessor_edges(StatEdge::TYPE_EXCEPTION).empty()) {   // exception handler
                set_handlers->add(stat->id);
                handlerfound = true;
            }
        }

        if (handlerfound) {
            for (auto& [key, set] : map_ext_postdominators) set->complement(set_handlers);
        }
    }

    void calc_default_reachable_sets() {
        const int edgetype = StatEdge::TYPE_REGULAR | StatEdge::TYPE_EXCEPTION;

        calc_reachability_supp_points(edgetype);

        for (Statement* stat : statement->get_stats()) {
            map_ext_postdominators.put(stat->id, factory->create_empty_set());
        }

        iterate_reachability(
            [this](Statement* node, SetMap& map_sets) {
                const int nodeid = node->id;

                FastFixedSet<int>* set_reachability = map_sets.at(nodeid);

                for (FastFixedSetIterator<int> it = set_reachability->iterator(); it.has_next();) {
                    const int id = it.next();
                    (*map_ext_postdominators.get(id))->add(nodeid);
                }

                return false;
            },
            edgetype);
    }

    void calc_reachability_supp_points(int edgetype) {
        iterate_reachability(
            [this, edgetype](Statement* node, SetMap& map_sets) {
                // consider to be a support point
                for (StatEdge* sucedge : node->get_all_successor_edges()) {
                    if ((sucedge->get_type() & edgetype) != 0) {
                        if (map_sets.contains(sucedge->get_destination()->id)) {
                            FastFixedSet<int>* set_reachability = map_sets.at(node->id);

                            FastFixedSet<int>** supp = map_support_points.get(node->id);
                            if (!equal_sets(set_reachability, supp != nullptr ? *supp : nullptr)) {
                                map_support_points.put(node->id, set_reachability);
                                return true;
                            }
                        }
                    }
                }

                return false;
            },
            edgetype);
    }

    void iterate_reachability(const Action& action, int edgetype) {
        SetMap map_sets;

        while (true) {
            bool iterate = false;

            map_sets.clear();

            for (Statement* stat : lst_reverse_post_order) {
                FastFixedSet<int>* set = factory->create_empty_set();
                set->add(stat->id);

                for (StatEdge* prededge : stat->get_all_predecessor_edges()) {
                    if ((prededge->get_type() & edgetype) != 0) {
                        Statement* pred = prededge->get_source();

                        FastFixedSet<int>* set_pred = nullptr;
                        if (const auto it = map_sets.find(pred->id); it != map_sets.end()) set_pred = it->second;
                        if (set_pred == nullptr) {
                            if (FastFixedSet<int>** sp = map_support_points.get(pred->id)) set_pred = *sp;
                        }

                        if (set_pred != nullptr) set->union_(set_pred);
                    }
                }

                map_sets[stat->id] = set;

                if (action) iterate |= action(stat, map_sets);

                // remove reachability information of fully processed nodes (saves memory)
                for (StatEdge* prededge : stat->get_all_predecessor_edges()) {
                    if ((prededge->get_type() & edgetype) != 0) {
                        Statement* pred = prededge->get_source();

                        if (map_sets.contains(pred->id)) {
                            bool remstat = true;

                            for (StatEdge* sucedge : pred->get_all_successor_edges()) {
                                if ((sucedge->get_type() & edgetype) != 0) {
                                    if (!map_sets.contains(sucedge->get_destination()->id)) {
                                        remstat = false;
                                        break;
                                    }
                                }
                            }

                            if (remstat) map_sets[pred->id] = nullptr;
                        }
                    }
                }
            }

            if (!iterate) break;
        }
    }
};

FastExtendedPostdominanceHelper::FastExtendedPostdominanceHelper() : impl_(std::make_unique<Impl>()) {}

FastExtendedPostdominanceHelper::~FastExtendedPostdominanceHelper() = default;

ExtPostMap FastExtendedPostdominanceHelper::get_extended_postdominators(Statement* statement) {
    // (the original's helper holds its maps for one call: a call on a used helper starts them anew)
    impl_ = std::make_unique<Impl>();
    return impl_->get_extended_postdominators(statement);
}

}  // namespace cb::java::decompiler
