// SSAConstructorSparseEx (de.fernflower.modules.decompiler.sforms).
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
}  // namespace

void SSAConstructorSparseEx::split_variables(RootStatement* root, const StructMethod* mt) {
    FlattenStatementsHelper flatthelper;
    DirectGraph* dgraph = flatthelper.build_direct_graph(root);

    std::vector<int> set_init;
    for (int i = 0; i < 64; i++) set_init.push_back(i);
    factory_ = make<FastSparseSetFactory<int>>(set_init);

    SFormsFastMapDirect* firstmap = create_first_map(mt);
    extra_var_versions_[dgraph->first->id] = firstmap;

    set_catch_maps(root, dgraph, &flatthelper);

    std::unordered_set<std::string> updated;
    do {
        ssa_statements(dgraph, updated);
    } while (!updated.empty());
}

void SSAConstructorSparseEx::ssa_statements(DirectGraph* dgraph, std::unordered_set<std::string>& updated) {
    for (DirectNode* node : dgraph->nodes) {
        updated.erase(node->id);
        merge_in_var_maps(node, dgraph);

        SFormsFastMapDirect* varmap = in_var_versions_.at(node->id);
        varmap = make<SFormsFastMapDirect>(*varmap);
        MapArr varmaparr{varmap, nullptr};

        if (node->exprents != nullptr) {
            for (Exprent* expr : *node->exprents) process_exprent(expr, varmaparr);
        }

        if (varmaparr[1] == nullptr) varmaparr[1] = varmaparr[0];

        const bool this_updated = !maps_equal(varmaparr[0], find_map(out_var_versions_, node->id)) ||
                                  (out_neg_var_versions_.contains(node->id) && !maps_equal(varmaparr[1], find_map(out_neg_var_versions_, node->id)));

        if (this_updated) {
            out_var_versions_[node->id] = varmaparr[0];
            if (dgraph->map_neg_if_branch.contains(node->id)) out_neg_var_versions_[node->id] = varmaparr[1];

            for (DirectNode* nd : node->succs) updated.insert(nd->id);
        }
    }
}

void SSAConstructorSparseEx::process_exprent(Exprent* expr, MapArr& varmaparr) {
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
                    process_exprent(func->get_lst_operands().at(0), varmaparr);

                    SFormsFastMapDirect* varmap_false;
                    if (varmaparr[1] == nullptr) {
                        varmap_false = make<SFormsFastMapDirect>(*varmaparr[0]);
                    } else {
                        varmap_false = varmaparr[1];
                        varmaparr[1] = nullptr;
                    }

                    process_exprent(func->get_lst_operands().at(1), varmaparr);

                    MapArr varmaparr_neg{varmap_false, nullptr};
                    process_exprent(func->get_lst_operands().at(2), varmaparr_neg);

                    merge_maps(varmaparr[0], varmaparr_neg[0]);
                    varmaparr[1] = nullptr;
                    finished = true;
                    break;
                }
                case FunctionExprent::FUNCTION_CADD: {
                    process_exprent(func->get_lst_operands().at(0), varmaparr);

                    MapArr varmaparr_and{make<SFormsFastMapDirect>(*varmaparr[0]), nullptr};
                    process_exprent(func->get_lst_operands().at(1), varmaparr_and);

                    // false map
                    varmaparr[1] = merge_maps(varmaparr[varmaparr[1] == nullptr ? 0 : 1], varmaparr_and[varmaparr_and[1] == nullptr ? 0 : 1]);
                    // true map
                    varmaparr[0] = varmaparr_and[0];

                    finished = true;
                    break;
                }
                case FunctionExprent::FUNCTION_COR: {
                    process_exprent(func->get_lst_operands().at(0), varmaparr);

                    MapArr varmaparr_or{make<SFormsFastMapDirect>(*varmaparr[varmaparr[1] == nullptr ? 0 : 1]), nullptr};
                    process_exprent(func->get_lst_operands().at(1), varmaparr_or);

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

    if (finished) return;

    std::vector<Exprent*> lst = expr->get_all_exprents();
    remove_first_equal(lst, varassign);

    for (Exprent* ex : lst) process_exprent(ex, varmaparr);

    SFormsFastMapDirect* varmap = varmaparr[0];

    if (varassign != nullptr) {
        const int varindex = varassign->get_index();

        if (varassign->get_version() == 0) {
            // get next version
            const int nextver = get_next_free_version(varindex);

            // set version
            varassign->set_version(nextver);

            set_current_var(varmap, varindex, nextver);
        } else {
            set_current_var(varmap, varindex, varassign->get_version());
        }
    } else if (expr->type == Exprent::EXPRENT_VAR) {
        VarExprent* vardest = static_cast<VarExprent*>(expr);

        const int varindex = vardest->get_index();
        FastSparseSet<int>* vers = varmap->get(varindex);
        if (vers == nullptr) throw std::runtime_error("no versions for variable " + std::to_string(varindex));

        const int cardinality = vers->get_cardinality();

        if (cardinality == 1) {  // == 1
            // set version
            auto it = vers->iterator();
            vardest->set_version(it.next());
        } else if (cardinality == 2) {  // size > 1
            const int current_vers = vardest->get_version();

            const VarVersionPair currpaar(varindex, current_vers);

            if (current_vers != 0 && phi_.contains_key(currpaar)) {
                set_current_var(varmap, varindex, current_vers);

                // update phi node
                (*phi_.get(currpaar))->union_(vers);
            } else {
                // increase version
                const int nextver = get_next_free_version(varindex);

                // set version
                vardest->set_version(nextver);

                set_current_var(varmap, varindex, nextver);

                // create new phi node
                phi_.put(VarVersionPair(varindex, nextver), vers);
            }
        }  // 0 means uninitialized variable, which is impossible
    }
}

int SSAConstructorSparseEx::get_next_free_version(int var) {
    int nextver;
    const auto it = lastversion_.find(var);
    if (it == lastversion_.end()) nextver = 1;
    else nextver = it->second + 1;

    lastversion_[var] = nextver;
    return nextver;
}

void SSAConstructorSparseEx::merge_in_var_maps(DirectNode* node, DirectGraph* dgraph) {
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

SFormsFastMapDirect* SSAConstructorSparseEx::get_filtered_out_map(const std::string& nodeid, const std::string& predid, DirectGraph* dgraph,
                                                                  const std::string& destid) {
    SFormsFastMapDirect* map_new = make<SFormsFastMapDirect>();

    const std::string* neg = find_string(dgraph->map_neg_if_branch, predid);
    if (neg != nullptr && nodeid == *neg) {
        if (out_neg_var_versions_.contains(predid)) map_new = out_neg_var_versions_.at(predid)->get_copy();
    } else if (out_var_versions_.contains(predid)) {
        map_new = out_var_versions_.at(predid)->get_copy();
    }

    const bool is_finally_exit = dgraph->map_short_range_finally_paths.contains(predid);

    if (is_finally_exit && !map_new->is_empty()) {
        SFormsFastMapDirect* map_new_temp = map_new->get_copy();

        SFormsFastMapDirect* map_true_source = make<SFormsFastMapDirect>();

        const std::string* exception_dest = find_string(dgraph->map_finally_monitor_exception_path_exits, predid);
        const bool is_exception_monitor_exit = (exception_dest != nullptr && nodeid != *exception_dest);

        std::unordered_set<std::string> set_long_path_wrapper;
        for (const FinallyPathWrapper& finwraplong : dgraph->map_long_range_finally_paths.at(predid)) {
            set_long_path_wrapper.insert(finwraplong.destination + "##" + finwraplong.source);
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

            SFormsFastMapDirect* old_in_map = find_map(in_var_versions_, nodeid);
            if (old_in_map != nullptr) map_new_temp->union_(old_in_map);

            map_new->intersection(map_new_temp);
        }
    }

    return map_new;
}

SFormsFastMapDirect* SSAConstructorSparseEx::merge_maps(SFormsFastMapDirect* map_to, const SFormsFastMapDirect* map2) {
    if (map2 != nullptr && !map2->is_empty()) map_to->union_(map2);
    return map_to;
}

bool SSAConstructorSparseEx::maps_equal(const SFormsFastMapDirect* map1, const SFormsFastMapDirect* map2) {
    if (map1 == nullptr) return map2 == nullptr;
    if (map2 == nullptr) return false;

    if (map1->size() != map2->size()) return false;

    for (const auto& ent2 : map2->entry_list()) {
        const FastSparseSet<int>* a = map1->get(ent2.first);
        if (a == nullptr ? ent2.second != nullptr : !a->equals(ent2.second)) return false;
    }
    return true;
}

void SSAConstructorSparseEx::set_current_var(SFormsFastMapDirect* varmap, int var, int vers) {
    FastSparseSet<int>* set = factory_->spawn_empty_set();
    set->add(vers);
    varmap->put(var, set);
}

void SSAConstructorSparseEx::set_catch_maps(Statement* stat, DirectGraph* dgraph, FlattenStatementsHelper* flatthelper) {
    switch (stat->type) {
        case Statement::TYPE_CATCHALL:
        case Statement::TYPE_TRYCATCH: {
            std::vector<VarExprent*>& lst_vars =
                stat->type == Statement::TYPE_CATCHALL ? static_cast<CatchAllStatement*>(stat)->get_vars() : static_cast<CatchStatement*>(stat)->get_vars();

            for (std::size_t i = 1; i < stat->get_stats().size(); i++) {
                const int varindex = lst_vars.at(i - 1)->get_index();
                const int version = get_next_free_version(varindex);  // == 1

                SFormsFastMapDirect* map = make<SFormsFastMapDirect>();
                set_current_var(map, varindex, version);

                DirectNode* nd = dgraph->nodes.get_with_key(flatthelper->get_map_destination_nodes().at(stat->get_stats()[i]->id)[0].value());
                if (nd == nullptr) throw std::runtime_error("no direct node of a handler");
                extra_var_versions_[nd->id] = map;

                start_vars_.push_back(VarVersionPair(varindex, version));
            }
            break;
        }
        default:
            break;
    }

    for (Statement* st : stat->get_stats()) set_catch_maps(st, dgraph, flatthelper);
}

SFormsFastMapDirect* SSAConstructorSparseEx::create_first_map(const StructMethod* mt) {
    const bool thisvar = !mt->has_modifier(cc::ACC_STATIC);

    const MethodDescriptor md = MethodDescriptor::parse(mt->descriptor());
    const int paramcount = static_cast<int>(md.params.size()) + (thisvar ? 1 : 0);

    int varindex = 0;
    SFormsFastMapDirect* map = make<SFormsFastMapDirect>();

    for (int i = 0; i < paramcount; i++) {
        const int version = get_next_free_version(varindex);  // == 1

        FastSparseSet<int>* set = factory_->spawn_empty_set();
        set->add(version);
        map->put(varindex, set);

        start_vars_.push_back(VarVersionPair(varindex, version));

        if (thisvar) {
            if (i == 0) varindex++;
            else varindex += md.params[static_cast<std::size_t>(i - 1)].stack_size;
        } else {
            varindex += md.params[static_cast<std::size_t>(i)].stack_size;
        }
    }

    return map;
}

}  // namespace cb::java::decompiler
