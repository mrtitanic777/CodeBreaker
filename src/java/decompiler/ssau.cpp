// SSAUConstructorSparseEx (de.fernflower.modules.decompiler.sforms).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <stdexcept>

#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/sforms.hpp"
#include "java/decompiler/structs.hpp"

namespace cb::java::decompiler {

namespace {
using MapArr = std::array<SFormsFastMapDirect*, 2>;

// List.remove(Object): the first element the object equals (a null object: the first null element).
void remove_first_equal(std::vector<Exprent*>& lst, Exprent* o) {
    for (auto it = lst.begin(); it != lst.end(); ++it) {
        if (o == nullptr ? *it == nullptr : (*it != nullptr && o->equals(*it))) {
            lst.erase(it);
            return;
        }
    }
}

SFormsFastMapDirect* find_map(const std::unordered_map<std::string, SFormsFastMapDirect*>& m, const std::string& key) {
    const auto it = m.find(key);
    return it == m.end() ? nullptr : it->second;
}

const std::string* find_string(const std::unordered_map<std::string, std::string>& m, const std::string& key) {
    const auto it = m.find(key);
    return it == m.end() ? nullptr : &it->second;
}

VarVersionNode* need_node(VarVersionsGraph& graph, const VarVersionPair& pair) {
    VarVersionNode* node = graph.nodes.get_with_key(pair);
    if (node == nullptr) throw std::runtime_error("no version node " + pair.to_string());
    return node;
}
}  // namespace

void SSAUConstructorSparseEx::split_variables(RootStatement* root, const StructMethod* mt) {
    FlattenStatementsHelper flatthelper;
    DirectGraph* dgraph = flatthelper.build_direct_graph(root);

    std::vector<int> set_init;
    for (int i = 0; i < 64; i++) set_init.push_back(i);
    factory_ = make<FastSparseSetFactory<int>>(set_init);

    extra_var_versions_[dgraph->first->id] = create_first_map(mt, root);
    set_catch_maps(root, dgraph, &flatthelper);

    std::unordered_set<std::string> updated;
    do {
        ssa_statements(dgraph, updated, false);
    } while (!updated.empty());

    ssa_statements(dgraph, updated, true);

    ssuversions_.init_dominators();
}

void SSAUConstructorSparseEx::ssa_statements(DirectGraph* dgraph, std::unordered_set<std::string>& updated, bool calc_live_vars) {
    for (DirectNode* node : dgraph->nodes) {
        updated.erase(node->id);
        merge_in_var_maps(node, dgraph);

        SFormsFastMapDirect* varmap = make<SFormsFastMapDirect>(*in_var_versions_.at(node->id));
        MapArr varmaparr{varmap, nullptr};

        if (node->exprents != nullptr) {
            for (Exprent* expr : *node->exprents) process_exprent(expr, varmaparr, node->statement, calc_live_vars);
        }

        if (varmaparr[1] == nullptr) varmaparr[1] = varmaparr[0];

        // quick solution: 'dummy' field variables should not cross basic block borders (otherwise problems e.g. with finally loops - usage without
        // assignment in a loop). For the full solution consider adding a dummy assignment at the entry point of the method
        const bool allow_field_propagation = node->succs.empty() || (node->succs.size() == 1 && node->succs[0]->preds.size() == 1);

        if (!allow_field_propagation && varmaparr[0] != nullptr) {
            varmaparr[0]->remove_all_fields();
            varmaparr[1]->remove_all_fields();
        }

        const bool this_updated = !maps_equal(varmaparr[0], find_map(out_var_versions_, node->id)) ||
                                  (out_neg_var_versions_.contains(node->id) && !maps_equal(varmaparr[1], find_map(out_neg_var_versions_, node->id)));

        if (this_updated) {
            out_var_versions_[node->id] = varmaparr[0];
            if (dgraph->map_neg_if_branch.contains(node->id)) out_neg_var_versions_[node->id] = varmaparr[1];

            for (DirectNode* nd : node->succs) updated.insert(nd->id);
        }
    }
}

void SSAUConstructorSparseEx::process_exprent(Exprent* expr, MapArr& varmaparr, Statement* stat, bool calc_live_vars) {
    if (expr == nullptr) return;

    VarExprent* varassign = nullptr;
    bool finished = false;

    switch (expr->type) {
        case Exprent::EXPRENT_ASSIGNMENT: {
            AssignmentExprent* assexpr = static_cast<AssignmentExprent*>(expr);
            if (assexpr->get_cond_type() == AssignmentExprent::CONDITION_NONE) {
                Exprent* dest = assexpr->get_left();
                if (dest->type == Exprent::EXPRENT_VAR) varassign = static_cast<VarExprent*>(dest);
            }
            break;
        }
        case Exprent::EXPRENT_FUNCTION: {
            FunctionExprent* func = static_cast<FunctionExprent*>(expr);
            switch (func->get_func_type()) {
                case FunctionExprent::FUNCTION_IIF: {
                    process_exprent(func->get_lst_operands().at(0), varmaparr, stat, calc_live_vars);

                    SFormsFastMapDirect* varmap_false;
                    if (varmaparr[1] == nullptr) {
                        varmap_false = make<SFormsFastMapDirect>(*varmaparr[0]);
                    } else {
                        varmap_false = varmaparr[1];
                        varmaparr[1] = nullptr;
                    }

                    process_exprent(func->get_lst_operands().at(1), varmaparr, stat, calc_live_vars);

                    MapArr varmaparr_neg{varmap_false, nullptr};
                    process_exprent(func->get_lst_operands().at(2), varmaparr_neg, stat, calc_live_vars);

                    merge_maps(varmaparr[0], varmaparr_neg[0]);
                    varmaparr[1] = nullptr;
                    finished = true;
                    break;
                }
                case FunctionExprent::FUNCTION_CADD: {
                    process_exprent(func->get_lst_operands().at(0), varmaparr, stat, calc_live_vars);

                    MapArr varmaparr_and{make<SFormsFastMapDirect>(*varmaparr[0]), nullptr};
                    process_exprent(func->get_lst_operands().at(1), varmaparr_and, stat, calc_live_vars);

                    // false map
                    varmaparr[1] = merge_maps(varmaparr[varmaparr[1] == nullptr ? 0 : 1], varmaparr_and[varmaparr_and[1] == nullptr ? 0 : 1]);
                    // true map
                    varmaparr[0] = varmaparr_and[0];

                    finished = true;
                    break;
                }
                case FunctionExprent::FUNCTION_COR: {
                    process_exprent(func->get_lst_operands().at(0), varmaparr, stat, calc_live_vars);

                    MapArr varmaparr_or{make<SFormsFastMapDirect>(*varmaparr[varmaparr[1] == nullptr ? 0 : 1]), nullptr};
                    process_exprent(func->get_lst_operands().at(1), varmaparr_or, stat, calc_live_vars);

                    // false map
                    varmaparr[1] = varmaparr_or[varmaparr_or[1] == nullptr ? 0 : 1];
                    // true map
                    varmaparr[0] = merge_maps(varmaparr[0], varmaparr_or[0]);

                    finished = true;
                    break;
                }
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }

    if (!finished) {
        std::vector<Exprent*> lst = expr->get_all_exprents();
        remove_first_equal(lst, varassign);

        for (Exprent* ex : lst) process_exprent(ex, varmaparr, stat, calc_live_vars);
    }

    SFormsFastMapDirect* varmap = varmaparr[0];

    // field access
    if (expr->type == Exprent::EXPRENT_FIELD) {
        int index;
        if (map_field_vars_.contains(expr->id)) {
            index = map_field_vars_.at(expr->id);
        } else {
            index = fieldvarcounter_--;
            map_field_vars_[expr->id] = index;

            // ssu graph
            ssuversions_.create_node(VarVersionPair(index, 1));
        }

        set_current_var(varmap, index, 1);
    } else if (expr->type == Exprent::EXPRENT_INVOCATION ||
               (expr->type == Exprent::EXPRENT_ASSIGNMENT && static_cast<AssignmentExprent*>(expr)->get_left()->type == Exprent::EXPRENT_FIELD) ||
               (expr->type == Exprent::EXPRENT_NEW && static_cast<NewExprent*>(expr)->get_new_type().type == cc::TYPE_OBJECT) ||
               expr->type == Exprent::EXPRENT_FUNCTION) {
        bool ismmpp = true;

        if (expr->type == Exprent::EXPRENT_FUNCTION) {
            ismmpp = false;

            FunctionExprent* fexpr = static_cast<FunctionExprent*>(expr);
            if (fexpr->get_func_type() >= FunctionExprent::FUNCTION_IMM && fexpr->get_func_type() <= FunctionExprent::FUNCTION_PPI) {
                if (fexpr->get_lst_operands().at(0)->type == Exprent::EXPRENT_FIELD) ismmpp = true;
            }
        }

        if (ismmpp) varmap->remove_all_fields();
    }

    if (varassign != nullptr) {
        const int varindex = varassign->get_index();

        if (varassign->get_version() == 0) {
            // get next version
            const int nextver = get_next_free_version(varindex, stat);

            // set version
            varassign->set_version(nextver);

            // ssu graph
            ssuversions_.create_node(VarVersionPair(varindex, nextver));

            set_current_var(varmap, varindex, nextver);
        } else {
            if (calc_live_vars) var_map_to_graph(VarVersionPair(varindex, varassign->get_version()), varmap);

            set_current_var(varmap, varindex, varassign->get_version());
        }
    } else if (expr->type == Exprent::EXPRENT_FUNCTION) {  // MM or PP function
        FunctionExprent* func = static_cast<FunctionExprent*>(expr);

        switch (func->get_func_type()) {
            case FunctionExprent::FUNCTION_IMM:
            case FunctionExprent::FUNCTION_MMI:
            case FunctionExprent::FUNCTION_IPP:
            case FunctionExprent::FUNCTION_PPI:
                if (func->get_lst_operands().at(0)->type == Exprent::EXPRENT_VAR) {
                    VarExprent* var = static_cast<VarExprent*>(func->get_lst_operands()[0]);
                    const int varindex = var->get_index();
                    const VarVersionPair varpaar(varindex, var->get_version());

                    // ssu graph
                    if (!phantomppnodes_.contains(varpaar)) {
                        // get next version
                        const int nextver = get_next_free_version(varindex, nullptr);
                        const VarVersionPair phantomver(varindex, nextver);

                        ssuversions_.create_node(phantomver);

                        VarVersionNode* vernode = need_node(ssuversions_, varpaar);

                        FastSparseSet<int>* vers = factory_->spawn_empty_set();
                        if (vernode->preds.size() == 1) {
                            vers->add(vernode->preds.first()->source->version);
                        } else {
                            for (VarVersionEdge* edge : vernode->preds) vers->add(edge->source->preds.first()->source->version);
                        }
                        vers->add(nextver);

                        create_or_update_phi_node(varpaar, vers, stat);

                        phantomppnodes_[varpaar] = phantomver;
                    }

                    if (calc_live_vars) var_map_to_graph(varpaar, varmap);

                    set_current_var(varmap, varindex, var->get_version());
                }
                break;
            default:
                break;
        }
    } else if (expr->type == Exprent::EXPRENT_VAR) {
        VarExprent* vardest = static_cast<VarExprent*>(expr);

        const int varindex = vardest->get_index();
        int current_vers = vardest->get_version();

        FastSparseSet<int>* vers = varmap->get(varindex);
        if (vers == nullptr) throw std::runtime_error("no versions for variable " + std::to_string(varindex));

        const int cardinality = vers->get_cardinality();

        if (cardinality == 1) {  // size == 1
            if (current_vers != 0) {
                if (calc_live_vars) var_map_to_graph(VarVersionPair(varindex, current_vers), varmap);

                set_current_var(varmap, varindex, current_vers);
            } else {
                // split last version
                const int usever = get_next_free_version(varindex, stat);

                // set version
                vardest->set_version(usever);
                set_current_var(varmap, varindex, usever);

                // ssu graph
                auto it = vers->iterator();
                const int lastver = it.next();
                VarVersionNode* prenode = need_node(ssuversions_, VarVersionPair(varindex, lastver));
                VarVersionNode* usenode = ssuversions_.create_node(VarVersionPair(varindex, usever));

                VarVersionEdge* edge = make<VarVersionEdge>(VarVersionEdge::EDGE_GENERAL, prenode, usenode);
                prenode->add_successor(edge);
                usenode->add_predecessor(edge);
            }
        } else if (cardinality == 2) {  // size > 1
            if (current_vers != 0) {
                if (calc_live_vars) var_map_to_graph(VarVersionPair(varindex, current_vers), varmap);

                set_current_var(varmap, varindex, current_vers);
            } else {
                // split version
                const int usever = get_next_free_version(varindex, stat);

                // set version
                vardest->set_version(usever);

                // ssu node
                ssuversions_.create_node(VarVersionPair(varindex, usever));

                set_current_var(varmap, varindex, usever);

                current_vers = usever;
            }

            create_or_update_phi_node(VarVersionPair(varindex, current_vers), vers, stat);
        }  // vers.size() == 0 means uninitialized variable, which is impossible
    }
}

void SSAUConstructorSparseEx::create_or_update_phi_node(const VarVersionPair& phivar, FastSparseSet<int>* vers, Statement* stat) {
    FastSparseSet<int>* vers_copy = vers->get_copy();
    IntHashSet phi_vers;

    // take into account the corresponding mm/pp node if existing
    const auto pp = phantomppnodes_.find(phivar);
    const int ppvers = pp != phantomppnodes_.end() ? pp->second.version : -1;

    // ssu graph
    VarVersionNode* phinode = need_node(ssuversions_, phivar);
    const std::vector<VarVersionEdge*> lst_preds = phinode->preds.items();

    if (lst_preds.size() == 1) {
        // not yet a phi node
        VarVersionEdge* edge = lst_preds[0];
        edge->source->remove_successor(edge);
        phinode->remove_predecessor(edge);
    } else {
        for (VarVersionEdge* edge : lst_preds) {
            const int verssrc = edge->source->preds.first()->source->version;
            if (!vers->contains(verssrc) && verssrc != ppvers) {
                edge->source->remove_successor(edge);
                phinode->remove_predecessor(edge);
            } else {
                vers_copy->remove(verssrc);
                phi_vers.add(verssrc);
            }
        }
    }

    std::vector<VarVersionNode*> colnodes;
    std::vector<VarVersionPair> colpaars;

    auto it = vers_copy->iterator();
    while (it.has_next()) {
        const int ver = it.next();
        VarVersionNode* prenode = need_node(ssuversions_, VarVersionPair(phivar.var, ver));

        const int tempver = get_next_free_version(phivar.var, stat);
        VarVersionNode* tempnode = make<VarVersionNode>(phivar.var, tempver);

        colnodes.push_back(tempnode);
        colpaars.push_back(VarVersionPair(phivar.var, tempver));

        VarVersionEdge* edge = make<VarVersionEdge>(VarVersionEdge::EDGE_GENERAL, prenode, tempnode);
        prenode->add_successor(edge);
        tempnode->add_predecessor(edge);

        edge = make<VarVersionEdge>(VarVersionEdge::EDGE_GENERAL, tempnode, phinode);
        tempnode->add_successor(edge);
        phinode->add_predecessor(edge);

        phi_vers.add(tempver);
    }

    ssuversions_.add_nodes(colnodes, colpaars);

    // update phi node
    phi_[phivar] = phi_vers;
}

void SSAUConstructorSparseEx::var_map_to_graph(const VarVersionPair& varpaar, SFormsFastMapDirect* varmap) {
    VarVersionNode* node = need_node(ssuversions_, varpaar);
    node->live = make<SFormsFastMapDirect>(*varmap);
}

int SSAUConstructorSparseEx::get_next_free_version(int var, Statement* stat) {
    int nextver;
    const auto it = lastversion_.find(var);
    if (it == lastversion_.end()) nextver = 1;
    else nextver = it->second + 1;

    lastversion_[var] = nextver;

    // save the first protected range, containing current statement
    if (stat != nullptr) {  // null iff phantom version
        const std::optional<int> first_range_id = get_first_protected_range(stat);
        if (first_range_id) map_version_first_range_[VarVersionPair(var, nextver)] = *first_range_id;
    }

    return nextver;
}

void SSAUConstructorSparseEx::merge_in_var_maps(DirectNode* node, DirectGraph* dgraph) {
    SFormsFastMapDirect* map_new = make<SFormsFastMapDirect>();

    for (DirectNode* pred : node->preds) {
        SFormsFastMapDirect* map_out = get_filtered_out_map(node->id, pred->id, dgraph, node->id);

        if (map_new->is_empty()) map_new = map_out->get_copy();
        else merge_maps(map_new, map_out);
    }

    if (extra_var_versions_.contains(node->id)) {
        SFormsFastMapDirect* map_extra = extra_var_versions_.at(node->id);

        if (map_new->is_empty()) map_new = map_extra->get_copy();
        else merge_maps(map_new, map_extra);
    }

    in_var_versions_[node->id] = map_new;
}

SFormsFastMapDirect* SSAUConstructorSparseEx::get_filtered_out_map(const std::string& nodeid, const std::string& predid, DirectGraph* dgraph,
                                                                   const std::string& destid) {
    SFormsFastMapDirect* map_new = make<SFormsFastMapDirect>();

    const bool is_finally_exit = dgraph->map_short_range_finally_paths.contains(predid);

    const std::string* neg = find_string(dgraph->map_neg_if_branch, predid);
    if (neg != nullptr && nodeid == *neg) {
        if (out_neg_var_versions_.contains(predid)) map_new = out_neg_var_versions_.at(predid)->get_copy();
    } else if (out_var_versions_.contains(predid)) {
        map_new = out_var_versions_.at(predid)->get_copy();
    }

    if (is_finally_exit) {
        SFormsFastMapDirect* map_new_temp = map_new->get_copy();

        SFormsFastMapDirect* map_true_source = make<SFormsFastMapDirect>();

        const std::string* exception_dest = find_string(dgraph->map_finally_monitor_exception_path_exits, predid);
        const bool is_exception_monitor_exit = (exception_dest != nullptr && nodeid != *exception_dest);

        std::unordered_set<std::string> set_long_path_wrapper;
        for (const auto& [key, lstwrapper] : dgraph->map_long_range_finally_paths) {
            for (const FinallyPathWrapper& finwraplong : lstwrapper) set_long_path_wrapper.insert(finwraplong.destination + "##" + finwraplong.source);
        }

        for (const FinallyPathWrapper& finwrap : dgraph->map_short_range_finally_paths.at(predid)) {
            SFormsFastMapDirect* map;

            const bool rec_finally = dgraph->map_short_range_finally_paths.contains(finwrap.source);

            if (rec_finally) {
                // recursion
                map = get_filtered_out_map(finwrap.entry, finwrap.source, dgraph, destid);
            } else {
                const std::string* neg_src = find_string(dgraph->map_neg_if_branch, finwrap.source);
                if (neg_src != nullptr && finwrap.entry == *neg_src) map = find_map(out_neg_var_versions_, finwrap.source);
                else map = find_map(out_var_versions_, finwrap.source);
            }

            // false path?
            bool is_false_path = true;
            if (rec_finally) is_false_path = finwrap.destination != nodeid;
            else is_false_path = !set_long_path_wrapper.contains(destid + "##" + finwrap.source);

            if (is_false_path) {
                map_new_temp->complement(map);
            } else {
                if (map_true_source->is_empty()) {
                    if (map != nullptr) map_true_source = map->get_copy();
                } else {
                    merge_maps(map_true_source, map);
                }
            }
        }

        if (is_exception_monitor_exit) {
            map_new = map_true_source;
        } else {
            map_new_temp->union_(map_true_source);
            map_new->intersection(map_new_temp);

            if (!map_true_source->is_empty() && !map_new->is_empty()) {  // FIXME: what for??
                // replace phi versions with corresponding phantom ones
                std::map<VarVersionPair, VarVersionPair> map_phantom;
                if (const auto pit = phantomexitnodes_.find(predid); pit != phantomexitnodes_.end()) map_phantom = pit->second;

                SFormsFastMapDirect* map_exit_var = map_new->get_copy();
                map_exit_var->complement(map_true_source);

                for (const auto& ent : map_exit_var->entry_list()) {
                    auto it = ent.second->iterator();
                    while (it.has_next()) {
                        const int version = it.next();
                        const int varindex = ent.first;

                        const VarVersionPair exitvar(varindex, version);

                        FastSparseSet<int>* new_set = map_new->get(varindex);

                        // remove the actual exit version
                        new_set->remove(version);

                        // get or create phantom version
                        VarVersionPair phantomvar;
                        const auto ph = map_phantom.find(exitvar);
                        if (ph == map_phantom.end()) {
                            const int newversion = get_next_free_version(exitvar.var, nullptr);
                            phantomvar = VarVersionPair(exitvar.var, newversion);

                            VarVersionNode* exitnode = need_node(ssuversions_, exitvar);
                            VarVersionNode* phantomnode = ssuversions_.create_node(phantomvar);
                            phantomnode->flags |= VarVersionNode::FLAG_PHANTOM_FINEXIT;

                            VarVersionEdge* edge = make<VarVersionEdge>(VarVersionEdge::EDGE_PHANTOM, exitnode, phantomnode);
                            exitnode->add_successor(edge);
                            phantomnode->add_predecessor(edge);

                            map_phantom[exitvar] = phantomvar;
                        } else {
                            phantomvar = ph->second;
                        }

                        // add phantom version
                        new_set->add(phantomvar.version);
                    }
                }

                if (!map_phantom.empty()) phantomexitnodes_[predid] = map_phantom;
            }
        }
    }

    return map_new;
}

SFormsFastMapDirect* SSAUConstructorSparseEx::merge_maps(SFormsFastMapDirect* map_to, const SFormsFastMapDirect* map2) {
    if (map2 != nullptr && !map2->is_empty()) map_to->union_(map2);
    return map_to;
}

bool SSAUConstructorSparseEx::maps_equal(const SFormsFastMapDirect* map1, const SFormsFastMapDirect* map2) {
    if (map1 == nullptr) return map2 == nullptr;
    if (map2 == nullptr) return false;

    if (map1->size() != map2->size()) return false;

    for (const auto& ent2 : map2->entry_list()) {
        const FastSparseSet<int>* a = map1->get(ent2.first);
        if (a == nullptr ? ent2.second != nullptr : !a->equals(ent2.second)) return false;
    }
    return true;
}

void SSAUConstructorSparseEx::set_current_var(SFormsFastMapDirect* varmap, int var, int vers) {
    FastSparseSet<int>* set = factory_->spawn_empty_set();
    set->add(vers);
    varmap->put(var, set);
}

void SSAUConstructorSparseEx::set_catch_maps(Statement* stat, DirectGraph* dgraph, FlattenStatementsHelper* flatthelper) {
    switch (stat->type) {
        case Statement::TYPE_CATCHALL:
        case Statement::TYPE_TRYCATCH: {
            std::vector<VarExprent*>& lst_vars =
                stat->type == Statement::TYPE_CATCHALL ? static_cast<CatchAllStatement*>(stat)->get_vars() : static_cast<CatchStatement*>(stat)->get_vars();

            for (std::size_t i = 1; i < stat->get_stats().size(); i++) {
                const int varindex = lst_vars.at(i - 1)->get_index();
                const int version = get_next_free_version(varindex, stat);  // == 1

                SFormsFastMapDirect* map = make<SFormsFastMapDirect>();
                set_current_var(map, varindex, version);

                DirectNode* nd = dgraph->nodes.get_with_key(flatthelper->get_map_destination_nodes().at(stat->get_stats()[i]->id)[0].value());
                if (nd == nullptr) throw std::runtime_error("no direct node of a handler");
                extra_var_versions_[nd->id] = map;

                ssuversions_.create_node(VarVersionPair(varindex, version));
            }
            break;
        }
        default:
            break;
    }

    for (Statement* st : stat->get_stats()) set_catch_maps(st, dgraph, flatthelper);
}

SFormsFastMapDirect* SSAUConstructorSparseEx::create_first_map(const StructMethod* mt, RootStatement* root) {
    const bool thisvar = !mt->has_modifier(cc::ACC_STATIC);

    const MethodDescriptor md = MethodDescriptor::parse(mt->descriptor());
    const int paramcount = static_cast<int>(md.params.size()) + (thisvar ? 1 : 0);

    int varindex = 0;
    SFormsFastMapDirect* map = make<SFormsFastMapDirect>();

    for (int i = 0; i < paramcount; i++) {
        const int version = get_next_free_version(varindex, root);  // == 1

        FastSparseSet<int>* set = factory_->spawn_empty_set();
        set->add(version);
        map->put(varindex, set);

        ssuversions_.create_node(VarVersionPair(varindex, version));

        if (thisvar) {
            if (i == 0) varindex++;
            else varindex += md.params[static_cast<std::size_t>(i - 1)].stack_size;
        } else {
            varindex += md.params[static_cast<std::size_t>(i)].stack_size;
        }
    }

    return map;
}

std::optional<int> SSAUConstructorSparseEx::get_first_protected_range(Statement* stat) {
    while (true) {
        Statement* parent = stat->get_parent();

        if (parent == nullptr) break;

        if (parent->type == Statement::TYPE_CATCHALL || parent->type == Statement::TYPE_TRYCATCH) {
            if (parent->get_first() == stat) return parent->id;
        } else if (parent->type == Statement::TYPE_SYNCRONIZED) {
            if (static_cast<SynchronizedStatement*>(parent)->get_body() == stat) return parent->id;
        }

        stat = parent;
    }

    return std::nullopt;
}

SFormsFastMapDirect* SSAUConstructorSparseEx::get_live_var_versions_map(const VarVersionPair& varpaar) {
    VarVersionNode* node = ssuversions_.nodes.get_with_key(varpaar);
    if (node != nullptr) return node->live;
    return nullptr;
}

}  // namespace cb::java::decompiler
