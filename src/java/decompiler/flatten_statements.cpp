// DirectGraph, FinallyPathWrapper and FlattenStatementsHelper (de.fernflower.modules.decompiler.sforms).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <algorithm>
#include <deque>
#include <memory>
#include <stdexcept>

#include "java/decompiler/sforms.hpp"

namespace cb::java::decompiler {

namespace {
// graph.nodes.getWithKey(id): the original goes on to dereference the node, so a missing one throws here.
DirectNode* node_of(DirectGraph* graph, const std::string& id) {
    DirectNode* node = graph->nodes.get_with_key(id);
    if (node == nullptr) throw std::runtime_error("no direct node " + id);
    return node;
}
std::string wrapper_text(const FinallyPathWrapper& w) { return w.source + ":" + w.destination + ":" + w.entry; }
}  // namespace

// ---- FinallyPathWrapper

bool FinallyPathWrapper::operator==(const FinallyPathWrapper& o) const { return wrapper_text(*this) == wrapper_text(o); }
bool FinallyPathWrapper::operator<(const FinallyPathWrapper& o) const { return wrapper_text(*this) < wrapper_text(o); }
std::int32_t FinallyPathWrapper::java_hash() const { return java_string_hash(wrapper_text(*this)); }
std::string FinallyPathWrapper::to_string() const { return source + "->(" + entry + ")->" + destination; }

// ---- DirectGraph

void DirectGraph::sort_reverse_post_order() {
    std::vector<DirectNode*> res;
    add_to_reverse_post_order_list_iterative(first, res);
    nodes.clear();
    for (DirectNode* node : res) nodes.add_with_key(node, node->id);
}

void DirectGraph::add_to_reverse_post_order_list_iterative(DirectNode* root, std::vector<DirectNode*>& lst) {
    std::vector<DirectNode*> stack_node;
    std::vector<int> stack_index;
    std::unordered_set<DirectNode*> set_visited;
    stack_node.push_back(root);
    stack_index.push_back(0);
    std::vector<DirectNode*> post;   // the original adds each finished node at the front of lst: here at the back, reversed at the end
    while (!stack_node.empty()) {
        DirectNode* node = stack_node.back();
        int index = stack_index.back();
        stack_index.pop_back();
        set_visited.insert(node);
        for (; index < static_cast<int>(node->succs.size()); index++) {
            DirectNode* succ = node->succs[static_cast<std::size_t>(index)];
            if (!set_visited.contains(succ)) {
                stack_index.push_back(index + 1);
                stack_node.push_back(succ);
                stack_index.push_back(0);
                break;
            }
        }
        if (index == static_cast<int>(node->succs.size())) {
            post.push_back(node);
            stack_node.pop_back();
        }
    }
    lst.insert(lst.begin(), post.rbegin(), post.rend());
}

bool DirectGraph::iterate_exprents(const ExprentIterator& iter) {
    std::deque<DirectNode*> stack;
    stack.push_back(first);
    std::unordered_set<DirectNode*> set_visited;
    while (!stack.empty()) {
        DirectNode* node = stack.front();
        stack.pop_front();
        if (set_visited.contains(node)) continue;
        set_visited.insert(node);
        for (std::size_t i = 0; i < node->exprents->size(); i++) {
            const int res = iter((*node->exprents)[i]);
            if (res == 1) return false;
            if (res == 2) {
                node->exprents->erase(node->exprents->begin() + static_cast<std::ptrdiff_t>(i));
                i--;   // (size_t wraps for 0 and the loop's i++ brings it back)
            }
        }
        for (DirectNode* succ : node->succs) stack.push_back(succ);
    }
    return true;
}

// ---- FlattenStatementsHelper

DirectGraph* FlattenStatementsHelper::build_direct_graph(RootStatement* root) {
    root_ = root;
    graph_ = make<DirectGraph>();
    flatten_statement();

    // dummy exit node
    Statement* dummyexit = root->get_dummy_exit();
    DirectNode* node = make<DirectNode>(DirectNode::NODE_DIRECT, dummyexit, std::to_string(dummyexit->id));
    node->exprents = &node->own_exprents;
    graph_->nodes.add_with_key(node, node->id);
    map_destination_nodes_[dummyexit->id] = DestinationNodeIds{node->id, std::nullopt};

    set_edges();

    graph_->first = node_of(graph_, *map_destination_nodes_.at(root->id)[0]);
    graph_->sort_reverse_post_order();
    return graph_;
}

void FlattenStatementsHelper::flatten_statement() {
    struct StatementStackEntry {
        Statement* statement;
        std::vector<StackEntry> stack_finally;
        std::vector<Exprent*>* tail_exprents;
        int statement_index = 0;
        int edge_index = 0;
        std::optional<std::vector<StatEdge*>> succ_edges;
        StatementStackEntry(Statement* statement, std::vector<StackEntry> stack_finally, std::vector<Exprent*>* tail_exprents)
            : statement(statement), stack_finally(std::move(stack_finally)), tail_exprents(tail_exprents) {}
    };
    using EntryPtr = std::shared_ptr<StatementStackEntry>;

    std::deque<EntryPtr> lst_stack_statements;
    lst_stack_statements.push_back(std::make_shared<StatementStackEntry>(root_, std::vector<StackEntry>{}, nullptr));

    while (!lst_stack_statements.empty()) {
        EntryPtr stat_entry = lst_stack_statements.front();
        lst_stack_statements.pop_front();

        Statement* stat = stat_entry->statement;
        const std::vector<StackEntry>& stack_finally = stat_entry->stack_finally;
        const int statement_break_index = stat_entry->statement_index;

        DirectNode* node;
        DirectNode* nd;
        std::vector<StatEdge*> lst_succ_edges;
        DirectNode* sourcenode = nullptr;

        if (!stat_entry->succ_edges) {
            switch (stat->type) {
                case Statement::TYPE_BASICBLOCK: {
                    node = make<DirectNode>(DirectNode::NODE_DIRECT, stat, static_cast<BasicBlockStatement*>(stat));
                    if (stat->get_exprents()) node->exprents = &*stat->get_exprents();
                    graph_->nodes.put_with_key(node, node->id);
                    map_destination_nodes_[stat->id] = DestinationNodeIds{node->id, std::nullopt};
                    lst_succ_edges = stat->get_successor_edges(Statement::STATEDGE_DIRECT_ALL);
                    sourcenode = node;

                    std::vector<Exprent*>* tail_exprent_list = stat_entry->tail_exprents;
                    if (tail_exprent_list != nullptr) {
                        DirectNode* tail = make<DirectNode>(DirectNode::NODE_TAIL, stat, std::to_string(stat->id) + "_tail");
                        tail->exprents = tail_exprent_list;
                        graph_->nodes.put_with_key(tail, tail->id);
                        map_destination_nodes_[-stat->id] = DestinationNodeIds{tail->id, std::nullopt};
                        list_edges_.push_back({node->id, -stat->id, StatEdge::TYPE_REGULAR});
                        sourcenode = tail;
                    }

                    // 'if' statement: record positive branch
                    if (stat->get_last_basic_type() == Statement::LASTBASICTYPE_IF) {
                        map_pos_if_branch_[sourcenode->id] = lst_succ_edges.at(0)->get_destination()->id;
                    }
                    break;
                }
                case Statement::TYPE_CATCHALL:
                case Statement::TYPE_TRYCATCH: {
                    DirectNode* firstnd = make<DirectNode>(DirectNode::NODE_TRY, stat, std::to_string(stat->id) + "_try");
                    map_destination_nodes_[stat->id] = DestinationNodeIds{firstnd->id, std::nullopt};
                    graph_->nodes.add_with_key(firstnd, firstnd->id);

                    std::vector<EntryPtr> lst;
                    for (Statement* st : stat->get_stats()) {
                        list_edges_.push_back({firstnd->id, st->id, StatEdge::TYPE_REGULAR});

                        std::vector<StackEntry> stack = stack_finally;
                        if (stat->type == Statement::TYPE_CATCHALL && static_cast<CatchAllStatement*>(stat)->is_finally()) {
                            if (st == stat->get_first()) {   // catch head
                                stack.push_back(StackEntry(static_cast<CatchAllStatement*>(stat), false));
                            } else {   // handler
                                stack.push_back(StackEntry(static_cast<CatchAllStatement*>(stat), true, StatEdge::TYPE_BREAK, root_->get_dummy_exit(), st, st, firstnd,
                                                           firstnd, true));
                            }
                        }
                        lst.push_back(std::make_shared<StatementStackEntry>(st, stack, nullptr));
                    }
                    lst_stack_statements.insert(lst_stack_statements.begin(), lst.begin(), lst.end());
                    break;
                }
                case Statement::TYPE_DO: {
                    if (statement_break_index == 0) {
                        stat_entry->statement_index = 1;
                        lst_stack_statements.push_front(stat_entry);
                        lst_stack_statements.push_front(std::make_shared<StatementStackEntry>(stat->get_first(), stack_finally, nullptr));
                        continue;
                    }

                    nd = node_of(graph_, *map_destination_nodes_.at(stat->get_first()->id)[0]);

                    DoStatement* dostat = static_cast<DoStatement*>(stat);
                    const int looptype = dostat->get_looptype();

                    if (looptype == DoStatement::LOOP_DO) {
                        map_destination_nodes_[stat->id] = DestinationNodeIds{nd->id, nd->id};
                        break;
                    }

                    lst_succ_edges.push_back(stat->get_successor_edges(Statement::STATEDGE_DIRECT_ALL).at(0));  // exactly one edge

                    switch (looptype) {
                        case DoStatement::LOOP_WHILE:
                        case DoStatement::LOOP_DOWHILE: {
                            node = make<DirectNode>(DirectNode::NODE_CONDITION, stat, std::to_string(stat->id) + "_cond");
                            node->exprents = &dostat->get_condition_exprent_list();
                            graph_->nodes.put_with_key(node, node->id);

                            list_edges_.push_back({node->id, stat->get_first()->id, StatEdge::TYPE_REGULAR});

                            if (looptype == DoStatement::LOOP_WHILE) {
                                map_destination_nodes_[stat->id] = DestinationNodeIds{node->id, node->id};
                            } else {
                                map_destination_nodes_[stat->id] = DestinationNodeIds{nd->id, node->id};

                                bool found = false;
                                for (const Edge& edge : list_edges_) {
                                    if (edge.statid == stat->id && edge.edgetype == StatEdge::TYPE_CONTINUE) {
                                        found = true;
                                        break;
                                    }
                                }
                                if (!found) list_edges_.push_back({nd->id, stat->id, StatEdge::TYPE_CONTINUE});
                            }
                            sourcenode = node;
                            break;
                        }
                        case DoStatement::LOOP_FOR: {
                            DirectNode* nodeinit = make<DirectNode>(DirectNode::NODE_INIT, stat, std::to_string(stat->id) + "_init");
                            if (dostat->get_init_exprent() != nullptr) nodeinit->exprents = &dostat->get_init_exprent_list();
                            graph_->nodes.put_with_key(nodeinit, nodeinit->id);

                            DirectNode* nodecond = make<DirectNode>(DirectNode::NODE_CONDITION, stat, std::to_string(stat->id) + "_cond");
                            nodecond->exprents = &dostat->get_condition_exprent_list();
                            graph_->nodes.put_with_key(nodecond, nodecond->id);

                            DirectNode* nodeinc = make<DirectNode>(DirectNode::NODE_INCREMENT, stat, std::to_string(stat->id) + "_inc");
                            nodeinc->exprents = &dostat->get_inc_exprent_list();
                            graph_->nodes.put_with_key(nodeinc, nodeinc->id);

                            map_destination_nodes_[stat->id] = DestinationNodeIds{nodeinit->id, nodeinc->id};
                            map_destination_nodes_[-stat->id] = DestinationNodeIds{nodecond->id, std::nullopt};

                            list_edges_.push_back({nodecond->id, stat->get_first()->id, StatEdge::TYPE_REGULAR});
                            list_edges_.push_back({nodeinit->id, -stat->id, StatEdge::TYPE_REGULAR});
                            list_edges_.push_back({nodeinc->id, -stat->id, StatEdge::TYPE_REGULAR});

                            bool found = false;
                            for (const Edge& edge : list_edges_) {
                                if (edge.statid == stat->id && edge.edgetype == StatEdge::TYPE_CONTINUE) {
                                    found = true;
                                    break;
                                }
                            }
                            if (!found) list_edges_.push_back({nd->id, stat->id, StatEdge::TYPE_CONTINUE});

                            sourcenode = nodecond;
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }
                case Statement::TYPE_SYNCRONIZED:
                case Statement::TYPE_SWITCH:
                case Statement::TYPE_IF:
                case Statement::TYPE_SEQUENCE:
                case Statement::TYPE_ROOT: {
                    int statsize = static_cast<int>(stat->get_stats().size());
                    if (stat->type == Statement::TYPE_SYNCRONIZED) {
                        statsize = 2;  // exclude the handler if synchronized
                    }

                    if (statement_break_index <= statsize) {
                        std::vector<Exprent*>* tailexprlst = nullptr;

                        switch (stat->type) {
                            case Statement::TYPE_SYNCRONIZED:
                                tailexprlst = &static_cast<SynchronizedStatement*>(stat)->get_headexprent_list();
                                break;
                            case Statement::TYPE_SWITCH:
                                tailexprlst = &static_cast<SwitchStatement*>(stat)->get_headexprent_list();
                                break;
                            case Statement::TYPE_IF:
                                tailexprlst = &static_cast<IfStatement*>(stat)->get_headexprent_list();
                                break;
                            default:
                                break;
                        }

                        if (statement_break_index < statsize) {
                            const int i = statement_break_index;
                            stat_entry->statement_index = i + 1;
                            lst_stack_statements.push_front(stat_entry);
                            lst_stack_statements.push_front(std::make_shared<StatementStackEntry>(
                                stat->get_stats()[static_cast<std::size_t>(i)], stack_finally,
                                (i == 0 && tailexprlst != nullptr && (*tailexprlst)[0] != nullptr) ? tailexprlst : nullptr));
                            continue;
                        }

                        node = node_of(graph_, *map_destination_nodes_.at(stat->get_first()->id)[0]);
                        map_destination_nodes_[stat->id] = DestinationNodeIds{node->id, std::nullopt};

                        if (stat->type == Statement::TYPE_IF && static_cast<IfStatement*>(stat)->iftype == IfStatement::IFTYPE_IF) {
                            lst_succ_edges.push_back(stat->get_successor_edges(Statement::STATEDGE_DIRECT_ALL).at(0));  // exactly one edge
                            sourcenode = (*tailexprlst)[0] == nullptr ? node : node_of(graph_, node->id + "_tail");
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }

        // no successor edges
        if (sourcenode != nullptr) {
            if (stat_entry->succ_edges) lst_succ_edges = *stat_entry->succ_edges;

            for (int edgeindex = stat_entry->edge_index; edgeindex < static_cast<int>(lst_succ_edges.size()); edgeindex++) {
                StatEdge* edge = lst_succ_edges[static_cast<std::size_t>(edgeindex)];

                std::vector<StackEntry> stack = stack_finally;
                int edgetype = edge->get_type();
                Statement* destination = edge->get_destination();

                DirectNode* finally_short_range_source = sourcenode;
                DirectNode* finally_long_range_source = sourcenode;
                Statement* finally_short_range_entry = nullptr;
                Statement* finally_long_range_entry = nullptr;

                bool is_finally_monitor_exception_path = false;
                bool is_finally_exit = false;

                while (true) {
                    const StackEntry* entry = nullptr;
                    StackEntry entry_copy = StackEntry(nullptr, false);
                    if (!stack.empty()) {
                        entry_copy = stack.back();
                        entry = &entry_copy;
                    }

                    bool created = true;

                    if (entry == nullptr) {
                        save_edge(sourcenode, destination, edgetype, is_finally_exit ? finally_short_range_source : nullptr, finally_long_range_source,
                                  finally_short_range_entry, finally_long_range_entry, is_finally_monitor_exception_path);
                    } else {
                        CatchAllStatement* catchall = entry->catchstatement;

                        if (entry->state) {  // finally handler statement
                            if (edgetype == StatEdge::TYPE_FINALLYEXIT) {
                                stack.pop_back();

                                destination = entry->destination;
                                edgetype = entry->edgetype;

                                finally_short_range_source = entry->finally_short_range_source;
                                finally_long_range_source = entry->finally_long_range_source;
                                finally_short_range_entry = entry->finally_short_range_entry;
                                finally_long_range_entry = entry->finally_long_range_entry;

                                is_finally_exit = true;
                                is_finally_monitor_exception_path = (catchall->get_monitor() != nullptr) & entry->is_finally_exception_path;

                                created = false;
                            } else {
                                if (!catchall->contains_statement_strict(destination)) {
                                    stack.pop_back();
                                    created = false;
                                } else {
                                    save_edge(sourcenode, destination, edgetype, is_finally_exit ? finally_short_range_source : nullptr, finally_long_range_source,
                                              finally_short_range_entry, finally_long_range_entry, is_finally_monitor_exception_path);
                                }
                            }
                        } else {  // finally protected try statement
                            if (!catchall->contains_statement_strict(destination)) {
                                save_edge(sourcenode, catchall->get_handler(), StatEdge::TYPE_REGULAR, is_finally_exit ? finally_short_range_source : nullptr,
                                          finally_long_range_source, finally_short_range_entry, finally_long_range_entry, is_finally_monitor_exception_path);

                                stack.pop_back();
                                stack.push_back(StackEntry(catchall, true, edgetype, destination, catchall->get_handler(),
                                                           finally_long_range_entry == nullptr ? catchall->get_handler() : finally_long_range_entry, sourcenode,
                                                           finally_long_range_source, false));

                                stat_entry->edge_index = edgeindex + 1;
                                stat_entry->succ_edges = lst_succ_edges;
                                lst_stack_statements.push_front(stat_entry);
                                lst_stack_statements.push_front(std::make_shared<StatementStackEntry>(catchall->get_handler(), stack, nullptr));

                                goto mainloop_continue;
                            } else {
                                save_edge(sourcenode, destination, edgetype, is_finally_exit ? finally_short_range_source : nullptr, finally_long_range_source,
                                          finally_short_range_entry, finally_long_range_entry, is_finally_monitor_exception_path);
                            }
                        }
                    }

                    if (created) break;
                }
            }
        }
    mainloop_continue:;
    }
}

void FlattenStatementsHelper::save_edge(DirectNode* sourcenode, Statement* destination, int edgetype, DirectNode* finally_short_range_source,
                                        DirectNode* finally_long_range_source, Statement* finally_short_range_entry, Statement* finally_long_range_entry,
                                        bool is_finally_monitor_exception_path) {
    if (edgetype != StatEdge::TYPE_FINALLYEXIT) list_edges_.push_back({sourcenode->id, destination->id, edgetype});

    if (finally_short_range_source != nullptr) {
        const bool is_continue_edge = (edgetype == StatEdge::TYPE_CONTINUE);

        std::vector<FinallyPathIds>* lst = map_short_range_finally_path_ids_.get(sourcenode->id);
        if (lst == nullptr) {
            map_short_range_finally_path_ids_.put(sourcenode->id, {});
            lst = map_short_range_finally_path_ids_.get(sourcenode->id);
        }
        lst->push_back({finally_short_range_source->id, destination->id, finally_short_range_entry->id, is_finally_monitor_exception_path, is_continue_edge});

        lst = map_long_range_finally_path_ids_.get(sourcenode->id);
        if (lst == nullptr) {
            map_long_range_finally_path_ids_.put(sourcenode->id, {});
            lst = map_long_range_finally_path_ids_.get(sourcenode->id);
        }
        lst->push_back({finally_long_range_source->id, destination->id, finally_long_range_entry->id, false, is_continue_edge});
    }
}

void FlattenStatementsHelper::set_edges() {
    for (const Edge& edge : list_edges_) {
        const std::string& sourceid = edge.sourceid;
        const int statid = edge.statid;

        DirectNode* source = node_of(graph_, sourceid);
        DirectNode* dest = node_of(graph_, map_destination_nodes_.at(statid)[edge.edgetype == StatEdge::TYPE_CONTINUE ? 1 : 0].value());

        if (std::find(source->succs.begin(), source->succs.end(), dest) == source->succs.end()) source->succs.push_back(dest);
        if (std::find(dest->preds.begin(), dest->preds.end(), source) == dest->preds.end()) dest->preds.push_back(source);

        if (const auto it = map_pos_if_branch_.find(sourceid); it != map_pos_if_branch_.end() && statid != it->second) {
            graph_->map_neg_if_branch[sourceid] = dest->id;
        }
    }

    for (int i = 0; i < 2; i++) {
        for (const auto& [key, lst] : (i == 0 ? map_short_range_finally_path_ids_ : map_long_range_finally_path_ids_).entries()) {
            std::vector<FinallyPathWrapper> new_lst;

            for (const FinallyPathIds& arr : lst) {
                const bool is_continue_edge = arr.is_continue_edge;

                DirectNode* dest = node_of(graph_, map_destination_nodes_.at(arr.destination)[is_continue_edge ? 1 : 0].value());
                DirectNode* enter = node_of(graph_, map_destination_nodes_.at(arr.entry)[0].value());

                new_lst.push_back(FinallyPathWrapper(arr.source, dest->id, enter->id));

                if (i == 0 && arr.is_finally_monitor_exception_path) graph_->map_finally_monitor_exception_path_exits[key] = dest->id;
            }

            if (!new_lst.empty()) {
                (i == 0 ? graph_->map_short_range_finally_paths : graph_->map_long_range_finally_paths)[key] =
                    JavaHashSet<FinallyPathWrapper, FinallyPathWrapperHash>::of(new_lst).items();
            }
        }
    }
}

}  // namespace cb::java::decompiler
