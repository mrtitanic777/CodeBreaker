// dom_helper.cpp -- StrongConnectivityHelper, IrreducibleCFGDeobfuscator, DomHelper, DecHelper. Ported from Fernflower
// (de.fernflower.modules.decompiler: StrongConnectivityHelper, DomHelper, DecHelper, deobfuscator.IrreducibleCFGDeobfuscator), Copyright 2000-2015
// JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <algorithm>
#include <climits>
#include <deque>
#include <memory>
#include <stdexcept>
#include <unordered_map>

#include "java/decompiler/dom_helper.hpp"
#include "java/decompiler/fast_sets.hpp"
#include "java/decompiler/helpers.hpp"

namespace cb::java::decompiler {

// ---- StrongConnectivityHelper

std::vector<std::vector<Statement*>>& StrongConnectivityHelper::find_components(Statement* stat) {
    components_.clear();   // (a new list: `components_` is the original's field, replaced)
    set_processed_.clear();

    visit_tree(stat->get_first());

    for (Statement* st : stat->get_stats()) {
        if (!set_processed_.contains(st) && st->get_predecessor_edges(Statement::STATEDGE_DIRECT_ALL).empty()) visit_tree(st);
    }

    // should not find any more nodes! FIXME: ??
    for (Statement* st : stat->get_stats()) {
        if (!set_processed_.contains(st)) visit_tree(st);
    }

    return components_;
}

bool StrongConnectivityHelper::is_exit_component(const std::vector<Statement*>& lst) {
    LinkedSet<Statement*> set;

    for (Statement* stat : lst) set.add_all(stat->get_neighbours(StatEdge::TYPE_REGULAR, Statement::DIRECTION_FORWARD));

    set.remove_all(lst);

    return (set.size() == 0);
}

std::vector<Statement*> StrongConnectivityHelper::get_exit_reps(const std::vector<std::vector<Statement*>>& lst) {
    std::vector<Statement*> res;

    for (const std::vector<Statement*>& comp : lst) {
        if (is_exit_component(comp)) res.push_back(comp.at(0));
    }

    return res;
}

void StrongConnectivityHelper::visit_tree(Statement* stat) {
    lstack_ = ListStack<Statement*>();
    ncounter_ = 0;
    tset_ = LinkedSet<Statement*>();
    dfsnummap_.clear();
    lowmap_.clear();

    visit(stat);

    set_processed_.add_all(tset_.items());
    set_processed_.add(stat);
}

void StrongConnectivityHelper::visit(Statement* stat) {
    lstack_.push(stat);
    dfsnummap_[stat] = ncounter_;
    lowmap_[stat] = ncounter_;
    ncounter_++;

    std::vector<Statement*> lst_succs = stat->get_neighbours(StatEdge::TYPE_REGULAR, Statement::DIRECTION_FORWARD);   // TODO: set?
    lst_succs.erase(std::remove_if(lst_succs.begin(), lst_succs.end(), [&](Statement* s) { return set_processed_.contains(s); }), lst_succs.end());

    for (std::size_t i = 0; i < lst_succs.size(); i++) {
        Statement* succ = lst_succs[i];
        int secvalue;

        if (tset_.contains(succ)) {
            secvalue = dfsnummap_.at(succ);
        } else {
            tset_.add(succ);
            visit(succ);

            secvalue = lowmap_.at(succ);
        }

        lowmap_[stat] = std::min(lowmap_.at(stat), secvalue);
    }

    if (lowmap_.at(stat) == dfsnummap_.at(stat)) {
        std::vector<Statement*> lst;
        Statement* v;
        do {
            v = lstack_.pop();
            lst.push_back(v);
        } while (v != stat);

        components_.push_back(lst);
    }
}

// ---- IrreducibleCFGDeobfuscator

namespace {

struct IrrNode {
    int id;
    LinkedSet<IrrNode*> preds;
    LinkedSet<IrrNode*> succs;
    explicit IrrNode(int id) : id(id) {}
};

}  // namespace

bool IrreducibleCFGDeobfuscator::is_statement_irreducible(Statement* statement) {
    std::vector<std::unique_ptr<IrrNode>> owned;
    IdMapH<IrrNode*> map_nodes;

    for (Statement* stat : statement->get_stats()) {
        if (!stat->get_successor_edges(StatEdge::TYPE_EXCEPTION).empty()) return false;

        owned.push_back(std::make_unique<IrrNode>(stat->id));
        map_nodes.put(stat->id, owned.back().get());
    }

    for (Statement* stat : statement->get_stats()) {
        IrrNode* node = *map_nodes.get(stat->id);

        for (Statement* succ : stat->get_neighbours(StatEdge::TYPE_REGULAR, Statement::DIRECTION_FORWARD)) {
            IrrNode* node_succ = *map_nodes.get(succ->id);

            node->succs.add(node_succ);
            node_succ->preds.add(node);
        }
    }

    while (true) {
        int ttype = 0;
        IrrNode* node = nullptr;

        for (IrrNode* nd : map_nodes.values()) {
            if (nd->succs.contains(nd)) {   // T1
                ttype = 1;
            } else if (nd->preds.size() == 1) {   // T2
                ttype = 2;
            }

            if (ttype != 0) {
                node = nd;
                break;
            }
        }

        if (node != nullptr) {
            if (ttype == 1) {
                node->succs.remove(node);
                node->preds.remove(node);
            } else {
                IrrNode* pred = node->preds.first();
                pred->succs.add_all(node->succs.items());
                pred->succs.remove(node);

                for (IrrNode* succ : node->succs.items()) {
                    succ->preds.remove(node);
                    succ->preds.add(pred);
                }

                map_nodes.remove(node->id);
            }
        } else {   // no transformation applicable
            return map_nodes.size() > 1;   // reducible iff one node remains
        }
    }
}

Statement* IrreducibleCFGDeobfuscator::get_candidate_for_splitting(Statement* statement) {
    Statement* candidate_for_splitting = nullptr;
    int size_candidate_for_splitting = INT_MAX;
    int succs_candidate_for_splitting = INT_MAX;

    for (Statement* stat : statement->get_stats()) {
        const LinkedSet<Statement*> set_preds = stat->get_neighbours_set(StatEdge::TYPE_REGULAR, Statement::DIRECTION_BACKWARD);

        if (set_preds.size() > 1) {
            const int succ_count = static_cast<int>(stat->get_neighbours_set(StatEdge::TYPE_REGULAR, Statement::DIRECTION_FORWARD).size());

            if (succ_count <= succs_candidate_for_splitting) {
                const int size = get_statement_size(stat) * (static_cast<int>(set_preds.size()) - 1);

                if (succ_count < succs_candidate_for_splitting || size < size_candidate_for_splitting) {
                    candidate_for_splitting = stat;
                    size_candidate_for_splitting = size;
                    succs_candidate_for_splitting = succ_count;
                }
            }
        }
    }

    return candidate_for_splitting;
}

bool IrreducibleCFGDeobfuscator::split_irreducible_node(Statement* statement) {
    Statement* splitnode = get_candidate_for_splitting(statement);
    if (splitnode == nullptr) return false;

    StatEdge* enteredge = splitnode->get_predecessor_edges(StatEdge::TYPE_REGULAR).at(0);

    std::unordered_map<Statement*, Statement*> map_alt_to_copies;
    Statement* splitcopy = copy_statement(splitnode, nullptr, map_alt_to_copies);

    init_copied_statement(splitcopy);

    splitcopy->set_parent(statement);
    statement->get_stats().add_with_key(splitcopy, splitcopy->id);

    // redirect entering edges
    for (StatEdge* prededge : splitnode->get_predecessor_edges(Statement::STATEDGE_DIRECT_ALL)) {
        if (prededge->get_source() == enteredge->get_source() || prededge->closure == enteredge->get_source()) {
            splitnode->remove_predecessor(prededge);
            prededge->get_source()->change_edge_node(Statement::DIRECTION_FORWARD, prededge, splitcopy);
            splitcopy->add_predecessor(prededge);
        }
    }

    // connect successors
    for (StatEdge* succ : splitnode->get_successor_edges(Statement::STATEDGE_DIRECT_ALL)) {
        splitcopy->add_successor(make<StatEdge>(succ->get_type(), splitcopy, succ->get_destination(), succ->closure));
    }

    return true;
}

int IrreducibleCFGDeobfuscator::get_statement_size(Statement* statement) {
    int res = 0;

    if (statement->type == Statement::TYPE_BASICBLOCK) {
        res = static_cast<int>(static_cast<BasicBlockStatement*>(statement)->get_block()->seq->length());
    } else {
        for (Statement* stat : statement->get_stats()) res += get_statement_size(stat);
    }

    return res;
}

Statement* IrreducibleCFGDeobfuscator::copy_statement(Statement* from, Statement* to, std::unordered_map<Statement*, Statement*>& map_alt_to_copies) {
    if (to == nullptr) {
        to = from->get_simple_copy();
        map_alt_to_copies[from] = to;
    }

    // copy statements
    for (Statement* st : from->get_stats()) {
        Statement* stcopy = st->get_simple_copy();
        to->get_stats().add_with_key(stcopy, stcopy->id);

        map_alt_to_copies[st] = stcopy;
    }

    for (std::size_t i = 0; i < from->get_stats().size(); i++) {
        // copy edges
        Statement* stold = from->get_stats().get(i);
        Statement* stnew = to->get_stats().get(i);

        for (StatEdge* edgeold : stold->get_successor_edges(Statement::STATEDGE_DIRECT_ALL)) {
            const auto dest = map_alt_to_copies.find(edgeold->get_destination());
            const auto clos = map_alt_to_copies.find(edgeold->closure);

            StatEdge* edgenew = make<StatEdge>(edgeold->get_type(), stnew, dest != map_alt_to_copies.end() ? dest->second : edgeold->get_destination(),
                                               clos != map_alt_to_copies.end() ? clos->second : edgeold->closure);

            stnew->add_successor(edgenew);
        }
    }

    for (std::size_t i = 0; i < from->get_stats().size(); i++) {
        // recursion
        Statement* stold = from->get_stats().get(i);
        Statement* stnew = to->get_stats().get(i);

        copy_statement(stold, stnew, map_alt_to_copies);
    }

    return to;
}

void IrreducibleCFGDeobfuscator::init_copied_statement(Statement* statement) {
    statement->init_simple_copy();
    statement->set_copied(true);

    for (Statement* st : statement->get_stats()) {
        st->set_parent(statement);
        init_copied_statement(st);
    }
}

// ---- DomHelper

RootStatement* DomHelper::graph_to_statement(ControlFlowGraph& graph) {
    KeyedList<Statement*, int> stats;
    KeyedList<BasicBlock*, int>& blocks = graph.blocks();

    for (BasicBlock* block : blocks) {
        stats.add_with_key(make<BasicBlockStatement>(block), block->id);
    }

    // head statement
    BasicBlock* firstblock = graph.first();
    Statement* firstst = stats.get_with_key(firstblock->id);

    // dummy exit statement
    Statement* dummyexit = make<Statement>();
    dummyexit->type = Statement::TYPE_DUMMYEXIT;

    Statement* general;

    // iterate over the graph
    if (stats.size() > 1 || firstblock->is_successor(firstblock)) {   // multiple basic blocks or an infinite loop of one block
        general = make<GeneralStatement>(firstst, stats.items(), nullptr);
    } else {   // one straightforward basic block
        RootStatement* root = make<RootStatement>(firstst, dummyexit);
        firstst->add_successor(make<StatEdge>(StatEdge::TYPE_BREAK, firstst, dummyexit, root));

        return root;
    }

    for (BasicBlock* block : blocks) {
        Statement* stat = stats.get_with_key(block->id);

        for (BasicBlock* succ : block->succs) {
            Statement* stsucc = stats.get_with_key(succ->id);

            int type;
            const std::vector<BasicBlock*>& finally_exits = graph.finally_exits();

            if (stsucc == firstst) {
                type = StatEdge::TYPE_CONTINUE;
            } else if (std::find(finally_exits.begin(), finally_exits.end(), block) != finally_exits.end()) {
                type = StatEdge::TYPE_FINALLYEXIT;
                stsucc = dummyexit;
            } else if (succ->id == graph.last()->id) {
                type = StatEdge::TYPE_BREAK;
                stsucc = dummyexit;
            } else {
                type = StatEdge::TYPE_REGULAR;
            }

            stat->add_successor(make<StatEdge>(type, stat, (type == StatEdge::TYPE_CONTINUE) ? general : stsucc,
                                               (type == StatEdge::TYPE_REGULAR) ? nullptr : general));
        }

        // exceptions edges
        for (BasicBlock* succex : block->succ_exceptions) {
            Statement* stsuccex = stats.get_with_key(succex->id);

            ExceptionRangeCFG* range = graph.exception_range(succex, block);

            if (!range->is_circular()) {
                stat->add_successor(make<StatEdge>(stat, stsuccex, range->exception_types));
            }
        }
    }

    general->build_continue_set();
    general->build_monitor_flags();

    return make<RootStatement>(general, dummyexit);
}

KeyedList<std::vector<int>, int> DomHelper::calc_post_dominators(Statement* container) {
    std::unordered_map<Statement*, FastFixedSet<Statement*>*> lists;

    StrongConnectivityHelper schelper(container);
    std::vector<std::vector<Statement*>>& components = schelper.get_components();

    std::vector<Statement*> lst_stats = container->get_post_reverse_post_order_list(StrongConnectivityHelper::get_exit_reps(components));

    FastFixedSetFactory<Statement*>* factory = make<FastFixedSetFactory<Statement*>>(lst_stats);

    FastFixedSet<Statement*>* set_flag_nodes = factory->spawn_empty_set();
    set_flag_nodes->set_all_elements();

    FastFixedSet<Statement*>* init_set = factory->spawn_empty_set();
    init_set->set_all_elements();

    // the initial set of the exit components is the component itself; of the others, all
    for (const std::vector<Statement*>& lst : components) {
        FastFixedSet<Statement*>* tmp_set;

        if (StrongConnectivityHelper::is_exit_component(lst)) {
            tmp_set = factory->spawn_empty_set();
            tmp_set->add_all(lst);
        } else {
            tmp_set = init_set->get_copy();
        }

        for (Statement* stat : lst) lists[stat] = tmp_set;
    }

    do {
        for (Statement* stat : lst_stats) {
            if (!set_flag_nodes->contains(stat)) continue;

            set_flag_nodes->remove(stat);

            FastFixedSet<Statement*>* doms = lists.at(stat);
            FastFixedSet<Statement*>* doms_succs = factory->spawn_empty_set();

            const std::vector<Statement*> lst_succs = stat->get_neighbours(StatEdge::TYPE_REGULAR, Statement::DIRECTION_FORWARD);
            for (std::size_t j = 0; j < lst_succs.size(); j++) {
                Statement* succ = lst_succs[j];
                FastFixedSet<Statement*>* succlst = lists.at(succ);

                if (j == 0) {
                    doms_succs->union_(succlst);
                } else {
                    doms_succs->intersection(succlst);
                }
            }

            if (!doms_succs->contains(stat)) doms_succs->add(stat);

            if (!doms_succs->equals(doms)) {
                lists[stat] = doms_succs;

                const std::vector<Statement*> lst_preds = stat->get_neighbours(StatEdge::TYPE_REGULAR, Statement::DIRECTION_BACKWARD);
                for (Statement* pred : lst_preds) set_flag_nodes->add(pred);
            }
        }
    } while (!set_flag_nodes->is_empty());

    KeyedList<std::vector<int>, int> ret;

    const std::vector<Statement*> lst_rev_post = container->get_reverse_post_order_list();   // sort order crucial!

    std::unordered_map<int, int> map_sort_order;
    for (std::size_t i = 0; i < lst_rev_post.size(); i++) map_sort_order[lst_rev_post[i]->id] = static_cast<int>(i);

    for (Statement* st : lst_stats) {
        std::vector<int> lst_posts;

        for (FastFixedSetIterator<Statement*> it = lists.at(st)->iterator(); it.has_next();) {
            lst_posts.push_back(it.next()->id);
        }

        std::stable_sort(lst_posts.begin(), lst_posts.end(), [&](int o1, int o2) { return map_sort_order.at(o1) < map_sort_order.at(o2); });

        if (lst_posts.size() > 1 && lst_posts[0] == st->id) {
            lst_posts.push_back(lst_posts.front());
            lst_posts.erase(lst_posts.begin());
        }

        ret.add_with_key(lst_posts, st->id);
    }

    return ret;
}

RootStatement* DomHelper::parse_graph(ControlFlowGraph& graph) {
    RootStatement* root = graph_to_statement(graph);

    if (!process_statement(root, std::make_shared<ExtPostMap>())) {
        throw std::runtime_error("parsing failure!");
    }

    LinkedSet<StatEdge*> edges;
    LabelHelper::low_continue_labels(root, edges);

    SequenceHelper::condense_sequences(root);

    root->build_monitor_flags();

    // build synchronized statements
    build_synchronized(root);

    return root;
}

void DomHelper::remove_synchronized_handler(Statement* stat) {
    for (Statement* st : stat->get_stats()) remove_synchronized_handler(st);

    if (stat->type == Statement::TYPE_SYNCRONIZED) static_cast<SynchronizedStatement*>(stat)->remove_exc();
}

void DomHelper::build_synchronized(Statement* stat) {
    for (Statement* st : stat->get_stats()) build_synchronized(st);

    if (stat->type == Statement::TYPE_SEQUENCE) {
        while (true) {
            bool found = false;

            KeyedList<Statement*, int>& lst = stat->get_stats();
            for (int i = 0; i < static_cast<int>(lst.size()) - 1; i++) {
                Statement* current = lst[static_cast<std::size_t>(i)];   // basic block

                if (current->is_monitor_enter()) {
                    Statement* next = lst[static_cast<std::size_t>(i + 1)];
                    Statement* next_direct = next;

                    while (next->type == Statement::TYPE_SEQUENCE) next = next->get_first();

                    if (next->type == Statement::TYPE_CATCHALL) {
                        CatchAllStatement* ca = static_cast<CatchAllStatement*>(next);

                        if (ca->get_first()->is_contains_monitor_exit() && ca->get_handler()->is_contains_monitor_exit()) {
                            current->remove_successor(current->get_successor_edges(Statement::STATEDGE_DIRECT_ALL).at(0));

                            for (StatEdge* edge : current->get_predecessor_edges(Statement::STATEDGE_DIRECT_ALL)) {
                                current->remove_predecessor(edge);
                                edge->get_source()->change_edge_node(Statement::DIRECTION_FORWARD, edge, next_direct);
                                next_direct->add_predecessor(edge);
                            }

                            stat->get_stats().remove_with_key(current->id);
                            stat->set_first(stat->get_stats().get(0));

                            SynchronizedStatement* sync = make<SynchronizedStatement>(current, ca->get_first(), ca->get_handler());

                            sync->set_all_parent();

                            for (StatEdge* edge : LinkedSet<StatEdge*>(ca->get_label_edges()).items()) sync->add_labeled_edge(edge);

                            current->add_successor(make<StatEdge>(StatEdge::TYPE_REGULAR, current, ca->get_first()));

                            ca->get_parent()->replace_statement(ca, sync);

                            found = true;
                            break;
                        }
                    }
                }
            }

            if (!found) break;
        }
    }
}

bool DomHelper::process_statement(Statement* general, ExtPostMapPtr map_ext_post) {
    if (general->type == Statement::TYPE_ROOT) {
        Statement* stat = general->get_first();

        if (stat->type != Statement::TYPE_GENERAL) {
            return true;
        } else {
            const bool complete = process_statement(stat, map_ext_post);

            if (complete) general->replace_statement(stat, stat->get_first());

            return complete;
        }
    }

    bool map_refreshed = map_ext_post->empty();

    for (int mapstage = 0; mapstage < 2; mapstage++) {
        for (int reducibility = 0; reducibility < 5; reducibility++) {   // FIXME: implement proper node splitting. For now up to 5 nodes in sequence are splitted.

            // take care of irreducible control flow graphs
            if (reducibility > 0) {
                if (IrreducibleCFGDeobfuscator::is_statement_irreducible(general)) {
                    if (!IrreducibleCFGDeobfuscator::split_irreducible_node(general)) {
                        DecompilerContext::log("Irreducible statement cannot be decomposed!");
                        break;
                    }
                } else {
                    if (mapstage == 2 || map_refreshed) {   // last chance lost
                        DecompilerContext::log("Statement cannot be decomposed although reducible!");
                    }

                    break;
                }

                map_ext_post = std::make_shared<ExtPostMap>();
                map_refreshed = true;
            }

            for (int i = 0; i < 2; i++) {
                const bool forceall = i != 0;

                while (true) {
                    if (find_simple_statements(general, *map_ext_post)) reducibility = 0;

                    if (general->type == Statement::TYPE_PLACEHOLDER) return true;

                    Statement* stat = find_general_statement(general, forceall, *map_ext_post);

                    if (stat != nullptr) {
                        const bool complete = process_statement(stat, general->get_first() == stat ? map_ext_post : std::make_shared<ExtPostMap>());

                        if (complete) {
                            general->replace_statement(stat, stat->get_first());
                        } else {
                            return false;
                        }

                        map_ext_post = std::make_shared<ExtPostMap>();
                        map_refreshed = true;

                        reducibility = 0;
                    } else {
                        break;
                    }
                }
            }
        }

        if (map_refreshed) {
            break;
        } else {
            map_ext_post = std::make_shared<ExtPostMap>();
        }
    }

    return false;
}

Statement* DomHelper::find_general_statement(Statement* stat, bool forceall, ExtPostMap& map_ext_post) {
    KeyedList<Statement*, int>& stats = stat->get_stats();

    KeyedList<std::vector<int>, int> vb_post;

    if (map_ext_post.empty()) {
        FastExtendedPostdominanceHelper extpost;
        // (putAll into an empty map: sized for the entries, as `new HashMap<>(other)`)
        map_ext_post = ExtPostMap::copy_of(extpost.get_extended_postdominators(stat));
    }

    if (forceall) {
        const std::vector<Statement*> lst_all = stat->get_post_reverse_post_order_list();

        for (Statement* st : lst_all) {
            const IdSet* set = map_ext_post.get(st->id);
            if (set != nullptr) {
                vb_post.add_with_key(set->items(), st->id);   // FIXME: sort order!!
            }
        }

        const IdSet* set_first = map_ext_post.get(stat->get_first()->id);
        if (set_first != nullptr) {
            for (const int id : set_first->items()) {
                if (!vb_post.contains_key(id)) vb_post.add_with_key(std::vector<int>(), id);

                vb_post[vb_post.index_by_key(id)].push_back(id);
            }
        }
    } else {
        vb_post = calc_post_dominators(stat);
    }

    for (std::size_t k = 0; k < vb_post.size(); k++) {
        const int headid = *vb_post.key(k);
        const std::vector<int> posts = vb_post[k];

        if (!map_ext_post.contains_key(headid) && !(posts.size() == 1 && posts[0] == headid)) continue;

        Statement* head = stats.get_with_key(headid);
        const IdSet* set_ext_posts = map_ext_post.get(headid);

        for (std::size_t i = 0; i < posts.size(); i++) {
            const int postid = posts[i];

            if (postid != headid) {
                if (set_ext_posts == nullptr) throw std::runtime_error("NullPointerException: no extended postdominators");
                if (!set_ext_posts->contains(postid)) continue;
            }

            Statement* post = stats.get_with_key(postid);

            if (post == nullptr) {   // possible in case of an inherited postdominance set
                continue;
            }

            const bool same = (post == head);

            LinkedSet<Statement*> set_nodes;
            LinkedSet<Statement*> set_preds;
            LinkedSet<Statement*> set_handlers;

            // collect statement nodes
            set_handlers.add(head);
            while (true) {
                bool hdfound = false;

                for (Statement* handler : set_handlers.items()) {
                    if (set_nodes.contains(handler)) continue;

                    bool addhd = (set_nodes.size() == 0);   // first handler == head
                    if (!addhd) {
                        const std::vector<Statement*> hdsupp = handler->get_neighbours(StatEdge::TYPE_EXCEPTION, Statement::DIRECTION_BACKWARD);

                        bool contains_all = true;   // setNodes.containsAll(hdsupp)
                        for (Statement* n : hdsupp) {
                            if (!set_nodes.contains(n)) {
                                contains_all = false;
                                break;
                            }
                        }

                        addhd = (contains_all && (set_nodes.size() > hdsupp.size() || set_nodes.size() == 1));   // strict subset
                    }

                    if (addhd) {
                        std::deque<Statement*> lst_stack;
                        lst_stack.push_back(handler);

                        while (!lst_stack.empty()) {
                            Statement* st = lst_stack.front();
                            lst_stack.pop_front();

                            if (!(set_nodes.contains(st) || (!same && st == post))) {
                                set_nodes.add(st);

                                if (st != head) {
                                    // record predeccessors except for the head
                                    set_preds.add_all(st->get_neighbours(StatEdge::TYPE_REGULAR, Statement::DIRECTION_BACKWARD));
                                }

                                // put successors on the stack
                                for (Statement* s : st->get_neighbours(StatEdge::TYPE_REGULAR, Statement::DIRECTION_FORWARD)) lst_stack.push_back(s);

                                // exception edges
                                set_handlers.add_all(st->get_neighbours(StatEdge::TYPE_EXCEPTION, Statement::DIRECTION_FORWARD));
                            }
                        }

                        hdfound = true;
                        set_handlers.remove(handler);
                        break;
                    }
                }

                if (!hdfound) break;
            }

            // check exception handlers
            set_handlers.clear();
            for (Statement* st : set_nodes) set_handlers.add_all(st->get_neighbours(StatEdge::TYPE_EXCEPTION, Statement::DIRECTION_FORWARD));
            set_handlers.remove_all(set_nodes.items());

            bool excok = true;
            for (Statement* handler : set_handlers) {
                const std::vector<Statement*> preds = handler->get_neighbours(StatEdge::TYPE_EXCEPTION, Statement::DIRECTION_BACKWARD);
                for (Statement* n : set_nodes) {
                    if (std::find(preds.begin(), preds.end(), n) == preds.end()) {
                        excok = false;
                        break;
                    }
                }
                if (!excok) break;
            }

            if (excok) {
                set_preds.remove_all(set_nodes.items());

                if (set_preds.size() == 0) {
                    const std::vector<Statement*> head_preds = head->get_neighbours(StatEdge::TYPE_REGULAR, Statement::DIRECTION_BACKWARD);

                    if ((set_nodes.size() > 1 || std::find(head_preds.begin(), head_preds.end(), head) != head_preds.end()) && set_nodes.size() < stats.size()) {
                        if (check_synchronized_completeness(head, set_nodes)) {
                            Statement* res = make<GeneralStatement>(head, set_nodes.items(), same ? nullptr : post);
                            stat->collapse_nodes_to_statement(res);

                            return res;
                        }
                    }
                }
            }
        }
    }

    return nullptr;
}

bool DomHelper::check_synchronized_completeness(Statement* head, const LinkedSet<Statement*>& set_nodes) {
    (void)head;

    for (Statement* stat : set_nodes) {
        if (stat->is_monitor_enter()) {
            const std::vector<StatEdge*> lst_succs = stat->get_successor_edges(Statement::STATEDGE_DIRECT_ALL);

            if (lst_succs.size() != 1 || lst_succs[0]->get_type() != StatEdge::TYPE_REGULAR) return false;

            if (!set_nodes.contains(lst_succs[0]->get_destination())) return false;
        }
    }

    return true;
}

bool DomHelper::find_simple_statements(Statement* stat, ExtPostMap& map_ext_post) {
    bool found, success = false;

    do {
        found = false;

        const std::vector<Statement*> lst_stats = stat->get_post_reverse_post_order_list();

        for (Statement* st : lst_stats) {
            Statement* result = detect_statement(st);

            if (result != nullptr) {
                if (stat->type == Statement::TYPE_GENERAL && result->get_first() == stat->get_first() && stat->get_stats().size() == result->get_stats().size()) {
                    stat->type = Statement::TYPE_PLACEHOLDER;
                }

                // collapse and replace
                stat->collapse_nodes_to_statement(result);

                // update the postdominator map
                if (!map_ext_post.empty()) {
                    IdSet set_old_nodes(IdSet::Order::HASH);
                    for (Statement* old : result->get_stats()) set_old_nodes.add(old->id);

                    const int newid = result->id;

                    for (const int key : map_ext_post.keys()) {
                        IdSet* set = map_ext_post.get(key);
                        const std::size_t oldsize = set->size();

                        set->remove_all(set_old_nodes);

                        if (set_old_nodes.contains(key)) {
                            const IdSet set_copy = *set;   // (a put below may move the map's entries)

                            IdSet* set_new = map_ext_post.get(newid);
                            if (set_new == nullptr) {
                                map_ext_post.put(newid, IdSet(IdSet::Order::HASH));
                                set_new = map_ext_post.get(newid);
                            }

                            set_new->add_all(set_copy);

                            map_ext_post.remove(key);
                        } else {
                            if (set->size() < oldsize) set->add(newid);
                        }
                    }
                }

                found = true;
                break;
            }
        }

        if (found) success = true;
    } while (found);

    return success;
}

Statement* DomHelper::detect_statement(Statement* head) {
    Statement* res;

    if ((res = DoStatement::is_head(head)) != nullptr) return res;

    if ((res = SwitchStatement::is_head(head)) != nullptr) return res;

    if ((res = IfStatement::is_head(head)) != nullptr) return res;

    if ((res = SequenceStatement::is_head2_block(head)) != nullptr) return res;

    if ((res = CatchStatement::is_head(head)) != nullptr) return res;

    if ((res = CatchAllStatement::is_head(head)) != nullptr) return res;

    return nullptr;
}

// ---- DecHelper

bool DecHelper::check_statement_exceptions(const std::vector<Statement*>& lst) {
    // all statements must belong to the same exception ranges
    const LinkedSet<Statement*> all = LinkedSet<Statement*>::of(lst);

    LinkedSet<Statement*> handlers;
    std::optional<LinkedSet<Statement*>> intersection;

    for (Statement* stat : lst) {
        LinkedSet<Statement*> set_new = stat->get_neighbours_set(StatEdge::TYPE_EXCEPTION, Statement::DIRECTION_FORWARD);

        if (!intersection) {
            intersection = set_new;
        } else {
            LinkedSet<Statement*> interclone = *intersection;
            interclone.remove_all(set_new.items());

            intersection->retain_all(set_new);

            set_new.remove_all(intersection->items());

            handlers.add_all(interclone.items());
            handlers.add_all(set_new.items());
        }
    }

    for (Statement* stat : handlers) {
        if (!all.contains(stat)) return false;

        for (Statement* pred : stat->get_neighbours(StatEdge::TYPE_EXCEPTION, Statement::DIRECTION_BACKWARD)) {
            if (!all.contains(pred)) return false;
        }
    }

    // check for other handlers (excluding head)
    for (std::size_t i = 1; i < lst.size(); i++) {
        Statement* stat = lst[i];
        if (!stat->get_predecessor_edges(StatEdge::TYPE_EXCEPTION).empty() && !handlers.contains(stat)) return false;
    }

    return true;
}

bool DecHelper::is_choice_statement(Statement* head, std::vector<Statement*>& lst) {
    Statement* post = nullptr;

    LinkedSet<Statement*> set_dest = head->get_neighbours_set(StatEdge::TYPE_REGULAR, Statement::DIRECTION_FORWARD);

    if (set_dest.contains(head)) return false;

    while (true) {
        lst.clear();

        bool repeat = false;

        set_dest.remove(post);

        for (Statement* stat : set_dest.items()) {
            // check for loop edges
            if (stat->get_last_basic_type() != Statement::LASTBASICTYPE_GENERAL) {
                if (post == nullptr) {
                    post = stat;
                    repeat = true;
                    break;
                } else {
                    return false;
                }
            }

            LinkedSet<Statement*> set_pred = stat->get_neighbours_set(StatEdge::TYPE_REGULAR, Statement::DIRECTION_BACKWARD);
            set_pred.remove(head);

            if (set_pred.contains(stat)) return false;

            bool pred_in_dest = true;
            for (Statement* p : set_pred) {
                if (!set_dest.contains(p)) {
                    pred_in_dest = false;
                    break;
                }
            }

            if (!pred_in_dest || set_pred.size() > 1) {
                // multiple predecessors, must be a post
                if (post == nullptr) {
                    post = stat;
                    repeat = true;
                    break;
                } else {
                    return false;
                }
            } else if (set_pred.size() == 1) {
                Statement* pred = set_pred.first();

                while (std::find(lst.begin(), lst.end(), pred) != lst.end()) {
                    LinkedSet<Statement*> set_pred_temp = pred->get_neighbours_set(StatEdge::TYPE_REGULAR, Statement::DIRECTION_BACKWARD);
                    set_pred_temp.remove(head);

                    if (!set_pred_temp.empty()) {   // at most 1 predecessor
                        pred = set_pred_temp.first();
                        if (pred == stat) {
                            return false;   // loop found
                        }
                    } else {
                        break;
                    }
                }
            }

            // check for exits
            const std::vector<StatEdge*> lst_edges = stat->get_successor_edges(Statement::STATEDGE_DIRECT_ALL);
            if (lst_edges.size() > 1) {
                LinkedSet<Statement*> set_succ = stat->get_neighbours_set(Statement::STATEDGE_DIRECT_ALL, Statement::DIRECTION_FORWARD);
                set_succ.retain_all(set_dest);

                if (set_succ.size() > 0) {
                    return false;
                } else {
                    if (post == nullptr) {
                        post = stat;
                        repeat = true;
                        break;
                    } else {
                        return false;
                    }
                }
            } else if (lst_edges.size() == 1) {
                StatEdge* edge = lst_edges[0];
                if (edge->get_type() == StatEdge::TYPE_REGULAR) {
                    Statement* statd = edge->get_destination();
                    if (head == statd) return false;

                    if (!set_dest.contains(statd) && post != statd) {
                        if (post != nullptr) {
                            return false;
                        } else {
                            const LinkedSet<Statement*> set = statd->get_neighbours_set(StatEdge::TYPE_REGULAR, Statement::DIRECTION_BACKWARD);
                            if (set.size() > 1) {
                                post = statd;
                                repeat = true;
                                break;
                            } else {
                                return false;
                            }
                        }
                    }
                }
            }

            lst.push_back(stat);
        }

        if (!repeat) break;
    }

    lst.push_back(head);
    {
        const auto it = std::find(lst.begin(), lst.end(), post);
        if (it != lst.end()) lst.erase(it);
    }
    lst.insert(lst.begin(), post);

    return true;
}

LinkedSet<Statement*> DecHelper::get_unique_pred_exceptions(Statement* head) {
    LinkedSet<Statement*> set_handlers = LinkedSet<Statement*>::of(head->get_neighbours(StatEdge::TYPE_EXCEPTION, Statement::DIRECTION_FORWARD));

    for (Statement* handler : set_handlers.items()) {
        if (handler->get_predecessor_edges(StatEdge::TYPE_EXCEPTION).size() > 1) set_handlers.remove(handler);
    }

    return set_handlers;
}

std::vector<Exprent*> DecHelper::copy_exprent_list(const std::vector<Exprent*>& lst) {
    std::vector<Exprent*> ret;
    for (Exprent* expr : lst) ret.push_back(expr->copy());
    return ret;
}

}  // namespace cb::java::decompiler
