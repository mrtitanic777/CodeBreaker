// VarProcessor, VarVersionsProcessor, VarTypeProcessor (de.fernflower.modules.decompiler.vars).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <algorithm>
#include <deque>
#include <stdexcept>

#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/collectors.hpp"
#include "java/decompiler/sforms.hpp"
#include "java/decompiler/structs.hpp"
#include "java/decompiler/vars.hpp"

namespace cb::java::decompiler {

namespace {
// the expressions below `exprent` and the expression itself (the original's getAllExprents(true) then add)
std::vector<Exprent*> all_with_self(Exprent* exprent) {
    std::vector<Exprent*> lst = exprent->get_all_exprents(true);
    lst.push_back(exprent);
    return lst;
}

// a Java null for a value that may be absent from the map or hold a null
const VarType* min_type_of(const VarVersionMap<VarType>& m, const VarVersionPair& pair) { return m.get(pair); }
const VarType* max_type_of(const VarVersionMap<std::optional<VarType>>& m, const VarVersionPair& pair) {
    const std::optional<VarType>* p = m.get(pair);
    return (p != nullptr && p->has_value()) ? &**p : nullptr;
}
}  // namespace

// ---- VarProcessor

void VarProcessor::set_var_versions(RootStatement* root) {
    var_versions_ = make<VarVersionsProcessor>();
    var_versions_->set_var_versions(root);
}

void VarProcessor::set_var_definitions(Statement* root) {
    map_var_names_ = VarVersionMap<std::string>();

    const StructMethod* mt = DecompilerContext::current().current_method;
    VarDefinitionHelper(root, mt, this).set_var_definitions();
}

void VarProcessor::set_debug_var_names(const std::map<int, std::string>& map_debug_var_names) {
    if (var_versions_ == nullptr) return;

    const std::map<int, int>& map_original_var_indices = var_versions_->get_map_original_var_indices();

    std::vector<VarVersionPair> list_vars = map_var_names_.keys();
    java_sort(list_vars, [](const VarVersionPair& o1, const VarVersionPair& o2) { return o1.var - o2.var; });

    std::map<std::string, int> map_names;

    for (const VarVersionPair& pair : list_vars) {
        std::string name = *map_var_names_.get(pair);

        const auto index = map_original_var_indices.find(pair.var);
        if (index != map_original_var_indices.end()) {
            const auto dn = map_debug_var_names.find(index->second);
            if (dn != map_debug_var_names.end()) name = dn->second;
        }

        int counter;
        const auto cit = map_names.find(name);
        if (cit == map_names.end()) counter = 0;
        else counter = cit->second + 1;
        map_names[name] = counter;

        if (counter > 0) name += std::to_string(counter);

        map_var_names_.put(pair, name);
    }
}

void VarProcessor::refresh_var_names(VarNamesCollector& vc) {
    const VarVersionMap<std::string> temp_var_names = VarVersionMap<std::string>::copy_of(map_var_names_);
    for (const auto& [key, value] : temp_var_names.entries()) map_var_names_.put(key, vc.get_free_name(value));
}

std::optional<VarType> VarProcessor::get_var_type(const VarVersionPair& pair) const {
    return var_versions_ == nullptr ? std::nullopt : var_versions_->get_var_type(pair);
}

void VarProcessor::set_var_type(const VarVersionPair& pair, const VarType& type) {
    if (var_versions_ == nullptr) throw std::runtime_error("VarProcessor: no var versions");
    var_versions_->set_var_type(pair, type);
}

std::optional<std::string> VarProcessor::get_var_name(const VarVersionPair& pair) const {
    const std::string* name = map_var_names_.get(pair);
    return name != nullptr ? std::optional<std::string>(*name) : std::nullopt;
}

void VarProcessor::set_var_name(const VarVersionPair& pair, const std::string& name) { map_var_names_.put(pair, name); }

int VarProcessor::get_var_final(const VarVersionPair& pair) const {
    return var_versions_ == nullptr ? VarTypeProcessor::VAR_FINAL : var_versions_->get_var_final(pair);
}

void VarProcessor::set_var_final(const VarVersionPair& pair, int final_type) {
    if (var_versions_ == nullptr) throw std::runtime_error("VarProcessor: no var versions");
    var_versions_->set_var_final(pair, final_type);
}

// ---- VarVersionsProcessor

void VarVersionsProcessor::set_var_versions(RootStatement* root) {
    const StructMethod* mt = DecompilerContext::current().current_method;

    SSAConstructorSparseEx ssa;
    ssa.split_variables(root, mt);

    FlattenStatementsHelper flatten_helper;
    DirectGraph* graph = flatten_helper.build_direct_graph(root);

    merge_phi_versions(&ssa, graph);

    type_processor_ = make<VarTypeProcessor>();
    type_processor_->calculate_var_types(root, graph);

    simple_merge(type_processor_, graph, mt);

    // FIXME: advanced merging

    eliminate_non_java_types(type_processor_);

    set_new_var_indices(type_processor_, graph);
}

void VarVersionsProcessor::merge_phi_versions(SSAConstructorSparseEx* ssa, DirectGraph* graph) {
    // collect phi versions
    std::vector<VarVersionSet> lst;

    for (const auto& ent : ssa->get_phi().entries()) {
        VarVersionSet set;
        set.add(ent.first);

        auto it = ent.second->iterator();
        while (it.has_next()) set.add(VarVersionPair(ent.first.var, it.next()));

        for (int i = static_cast<int>(lst.size()) - 1; i >= 0; i--) {
            const VarVersionSet& tset = lst[static_cast<std::size_t>(i)];

            bool intersects = false;
            for (const VarVersionPair& p : set.items()) {
                if (tset.contains(p)) {
                    intersects = true;
                    break;
                }
            }

            if (intersects) {
                for (const VarVersionPair& p : tset.items()) set.add(p);
                lst.erase(lst.begin() + i);
            }
        }

        lst.push_back(set);
    }

    VarVersionMap<int> phi_versions;
    for (const VarVersionSet& set : lst) {
        int min = INT32_MAX;
        for (const VarVersionPair& paar : set.items()) {
            if (paar.version < min) min = paar.version;
        }

        for (const VarVersionPair& paar : set.items()) phi_versions.put(VarVersionPair(paar.var, paar.version), min);
    }

    update_versions(graph, phi_versions);
}

void VarVersionsProcessor::update_versions(DirectGraph* graph, const VarVersionMap<int>& versions) {
    graph->iterate_exprents([&versions](Exprent* exprent) {
        for (Exprent* expr : all_with_self(exprent)) {
            if (expr->type == Exprent::EXPRENT_VAR) {
                VarExprent* var = static_cast<VarExprent*>(expr);
                const int* version = versions.get(VarVersionPair(var));
                if (version != nullptr) var->set_version(*version);
            }
        }
        return 0;
    });
}

void VarVersionsProcessor::eliminate_non_java_types(VarTypeProcessor* type_processor) {
    VarVersionMap<std::optional<VarType>>& map_exprent_max_types = type_processor->get_map_exprent_max_types();
    VarVersionMap<VarType>& map_exprent_min_types = type_processor->get_map_exprent_min_types();

    const std::vector<VarVersionPair> set = VarVersionSet::of(map_exprent_min_types.keys()).items();

    for (const VarVersionPair& paar : set) {
        VarType type = *map_exprent_min_types.get(paar);
        const VarType* max_type = max_type_of(map_exprent_max_types, paar);

        if (type.type == cc::TYPE_BYTECHAR || type.type == cc::TYPE_SHORTCHAR) {
            if (max_type != nullptr && max_type->type == cc::TYPE_CHAR) {
                type = VarType::CHAR;
            } else {
                type = type.type == cc::TYPE_BYTECHAR ? VarType::BYTE : VarType::SHORT;
            }
            map_exprent_min_types.put(paar, type);
        } else if (type.type == cc::TYPE_NULL) {
            map_exprent_min_types.put(paar, VarType::OBJECT);
        }
    }
}

void VarVersionsProcessor::simple_merge(VarTypeProcessor* type_processor, DirectGraph* graph, const StructMethod* mt) {
    VarVersionMap<std::optional<VarType>>& map_exprent_max_types = type_processor->get_map_exprent_max_types();
    VarVersionMap<VarType>& map_exprent_min_types = type_processor->get_map_exprent_min_types();

    IntHashMap<IntHashSet> map_var_versions;
    for (const VarVersionPair& pair : map_exprent_min_types.keys()) {
        if (pair.version >= 0) {  // don't merge constants
            IntHashSet* set = map_var_versions.get(pair.var);
            if (set == nullptr) {
                map_var_versions.put(pair.var, IntHashSet());
                set = map_var_versions.get(pair.var);
            }
            set->add(pair.version);
        }
    }

    const bool is_method_static = mt->has_modifier(cc::ACC_STATIC);

    VarVersionMap<int> map_merged_versions;

    for (const auto& ent : map_var_versions.entries()) {
        if (ent.second.size() > 1) {
            std::vector<int> lst_versions = ent.second.items();
            std::sort(lst_versions.begin(), lst_versions.end());

            for (std::size_t i = 0; i < lst_versions.size(); i++) {
                const VarVersionPair first_pair(ent.first, lst_versions[i]);
                VarType first_type = *map_exprent_min_types.get(first_pair);

                if (first_pair.var == 0 && first_pair.version == 1 && !is_method_static) {
                    continue;  // don't merge 'this' variable
                }

                for (std::size_t j = i + 1; j < lst_versions.size(); j++) {
                    const VarVersionPair second_pair(ent.first, lst_versions[j]);
                    const VarType second_type = *map_exprent_min_types.get(second_pair);

                    if (first_type == second_type || (first_type == VarType::NULL_TYPE && second_type.type == cc::TYPE_OBJECT) ||
                        (second_type == VarType::NULL_TYPE && first_type.type == cc::TYPE_OBJECT)) {
                        const VarType* first_max_type = max_type_of(map_exprent_max_types, first_pair);
                        const VarType* second_max_type = max_type_of(map_exprent_max_types, second_pair);

                        std::optional<VarType> type;
                        if (first_max_type == nullptr) {
                            if (second_max_type != nullptr) type = *second_max_type;
                        } else if (second_max_type == nullptr) {
                            type = *first_max_type;
                        } else {
                            type = VarType::common_min_type(*first_max_type, *second_max_type);
                        }

                        map_exprent_max_types.put(first_pair, type);

                        map_merged_versions.put(second_pair, first_pair.version);

                        map_exprent_max_types.remove(second_pair);
                        map_exprent_min_types.remove(second_pair);

                        if (first_type == VarType::NULL_TYPE) {
                            map_exprent_min_types.put(first_pair, second_type);
                            first_type = second_type;
                        }

                        type_processor->get_map_final_vars().put(first_pair, VarTypeProcessor::VAR_NON_FINAL);

                        lst_versions.erase(lst_versions.begin() + static_cast<std::ptrdiff_t>(j));
                        j--;
                    }
                }
            }
        }
    }

    if (map_merged_versions.size() > 0) update_versions(graph, map_merged_versions);
}

void VarVersionsProcessor::set_new_var_indices(VarTypeProcessor* type_processor, DirectGraph* graph) {
    VarVersionMap<std::optional<VarType>>& map_exprent_max_types = type_processor->get_map_exprent_max_types();
    VarVersionMap<VarType>& map_exprent_min_types = type_processor->get_map_exprent_min_types();
    VarVersionMap<int>& map_final_vars = type_processor->get_map_final_vars();

    CounterContainer& counters = DecompilerContext::counter_container();

    std::map<VarVersionPair, int> map_var_paar;
    std::map<int, int> map_original_var_indices;

    // map var-version pairs on new var indexes
    const std::vector<VarVersionPair> set = VarVersionSet::of(map_exprent_min_types.keys()).items();

    for (const VarVersionPair& pair : set) {
        if (pair.version >= 0) {
            const int new_index = pair.version == 1 ? pair.var : counters.get_counter_and_increment(CounterContainer::VAR_COUNTER);
            const VarVersionPair new_var(new_index, 0);

            const VarType min_type = *map_exprent_min_types.get(pair);
            map_exprent_min_types.put(new_var, min_type);

            const std::optional<VarType>* max_p = map_exprent_max_types.get(pair);
            const std::optional<VarType> max_type = max_p != nullptr ? *max_p : std::nullopt;
            map_exprent_max_types.put(new_var, max_type);

            if (map_final_vars.contains_key(pair)) {
                const std::optional<int> fin = map_final_vars.remove(pair);
                map_final_vars.put(new_var, *fin);
            }

            map_var_paar[pair] = new_index;
            map_original_var_indices[new_index] = pair.var;
        }
    }

    // set new vars
    graph->iterate_exprents([&map_var_paar, &map_exprent_max_types](Exprent* exprent) {
        for (Exprent* expr : all_with_self(exprent)) {
            if (expr->type == Exprent::EXPRENT_VAR) {
                VarExprent* new_var = static_cast<VarExprent*>(expr);

                const auto it = map_var_paar.find(VarVersionPair(new_var));
                if (it != map_var_paar.end()) {
                    new_var->set_index(it->second);
                    new_var->set_version(0);
                }
            } else if (expr->type == Exprent::EXPRENT_CONST) {
                const VarType* max_type = max_type_of(map_exprent_max_types, VarVersionPair(expr->id, -1));
                if (max_type != nullptr && *max_type == VarType::CHAR) static_cast<ConstExprent*>(expr)->set_const_type(*max_type);
            }
        }
        return 0;
    });

    map_original_var_indices_ = map_original_var_indices;
}

std::optional<VarType> VarVersionsProcessor::get_var_type(const VarVersionPair& pair) const {
    return type_processor_ == nullptr ? std::nullopt : type_processor_->get_var_type(pair);
}

void VarVersionsProcessor::set_var_type(const VarVersionPair& pair, const VarType& type) {
    if (type_processor_ == nullptr) throw std::runtime_error("VarVersionsProcessor: no type processor");
    type_processor_->set_var_type(pair, type);
}

int VarVersionsProcessor::get_var_final(const VarVersionPair& pair) const {
    int ret = VarTypeProcessor::VAR_FINAL;

    if (type_processor_ != nullptr) {
        const int* fin = type_processor_->get_map_final_vars().get(pair);
        ret = fin == nullptr ? VarTypeProcessor::VAR_FINAL : *fin;
    }

    return ret;
}

void VarVersionsProcessor::set_var_final(const VarVersionPair& pair, int final_type) {
    if (type_processor_ == nullptr) throw std::runtime_error("VarVersionsProcessor: no type processor");
    type_processor_->get_map_final_vars().put(pair, final_type);
}

// ---- VarTypeProcessor

void VarTypeProcessor::set_init_vars(RootStatement* root) {
    DecompilerContext& ctx = DecompilerContext::current();
    const StructMethod* mt = ctx.current_method;

    const bool this_var = !mt->has_modifier(cc::ACC_STATIC);

    const MethodDescriptor& md = ctx.current_method_descriptor.value();

    if (this_var) {
        const StructClass* cl = ctx.current_class;
        const VarType cl_type(cc::TYPE_OBJECT, 0, cl->qualified_name);

        map_exprent_min_types_.put(VarVersionPair(0, 1), cl_type);
        map_exprent_max_types_.put(VarVersionPair(0, 1), cl_type);
    }

    int var_index = 0;
    for (std::size_t i = 0; i < md.params.size(); i++) {
        map_exprent_min_types_.put(VarVersionPair(var_index + (this_var ? 1 : 0), 1), md.params[i]);
        map_exprent_max_types_.put(VarVersionPair(var_index + (this_var ? 1 : 0), 1), md.params[i]);

        var_index += md.params[i].stack_size;
    }

    // catch variables
    std::deque<Statement*> stack;
    stack.push_back(root);

    while (!stack.empty()) {
        Statement* stat = stack.front();
        stack.pop_front();

        std::vector<VarExprent*>* lst_vars = nullptr;

        if (stat->type == Statement::TYPE_CATCHALL) lst_vars = &static_cast<CatchAllStatement*>(stat)->get_vars();
        else if (stat->type == Statement::TYPE_TRYCATCH) lst_vars = &static_cast<CatchStatement*>(stat)->get_vars();

        if (lst_vars != nullptr) {
            for (VarExprent* var : *lst_vars) {
                map_exprent_min_types_.put(VarVersionPair(var->get_index(), 1), var->get_var_type());
                map_exprent_max_types_.put(VarVersionPair(var->get_index(), 1), var->get_var_type());
            }
        }

        for (Statement* st : stat->get_stats()) stack.push_back(st);
    }
}

void VarTypeProcessor::calculate_var_types(RootStatement* root, DirectGraph* graph) {
    set_init_vars(root);

    reset_exprent_types(graph);

    //noinspection StatementWithEmptyBody
    while (!process_var_types(graph)) {
    }
}

void VarTypeProcessor::reset_exprent_types(DirectGraph* graph) {
    graph->iterate_exprents([](Exprent* exprent) {
        for (Exprent* expr : all_with_self(exprent)) {
            if (expr->type == Exprent::EXPRENT_VAR) {
                static_cast<VarExprent*>(expr)->set_var_type(VarType::UNKNOWN);
            } else if (expr->type == Exprent::EXPRENT_CONST) {
                ConstExprent* const_expr = static_cast<ConstExprent*>(expr);
                if (const_expr->get_const_type().type_family == cc::TYPE_FAMILY_INTEGER) {
                    // (the original's `new ConstExprent(...)` takes an expression id too)
                    const ConstExprent fresh(const_expr->get_int_value(), const_expr->is_bool_permitted(), std::nullopt);
                    const_expr->set_const_type(fresh.get_const_type());
                }
            }
        }
        return 0;
    });
}

bool VarTypeProcessor::process_var_types(DirectGraph* graph) {
    return graph->iterate_exprents([this](Exprent* exprent) { return check_type_exprent(exprent) ? 0 : 1; });
}

bool VarTypeProcessor::check_type_exprent(Exprent* exprent) {
    for (Exprent* expr : exprent->get_all_exprents()) {
        if (!check_type_exprent(expr)) return false;
    }

    if (exprent->type == Exprent::EXPRENT_CONST) {
        ConstExprent* const_expr = static_cast<ConstExprent*>(exprent);
        if (const_expr->get_const_type().type_family <= cc::TYPE_FAMILY_INTEGER) {  // boolean or integer
            const VarVersionPair pair(const_expr->id, -1);
            if (!map_exprent_min_types_.contains_key(pair)) map_exprent_min_types_.put(pair, const_expr->get_const_type());
        }
    }

    const CheckTypesResult result = exprent->check_expr_type_bounds();

    for (const ExprentTypePair& entry : result.lst_max_type_exprents) {
        if (entry.type.type_family != cc::TYPE_FAMILY_OBJECT) change_exprent_type(entry.exprent, entry.type, 1);
    }

    bool res = true;
    for (const ExprentTypePair& entry : result.lst_min_type_exprents) {
        const bool r = change_exprent_type(entry.exprent, entry.type, 0);
        res = res && r;
    }

    return res;
}

bool VarTypeProcessor::change_exprent_type(Exprent* exprent, VarType new_type, int min_max) {
    bool res = true;

    switch (exprent->type) {
        case Exprent::EXPRENT_CONST:
        case Exprent::EXPRENT_VAR: {
            if (exprent->type == Exprent::EXPRENT_CONST) {
                ConstExprent* const_expr = static_cast<ConstExprent*>(exprent);
                const VarType const_type = const_expr->get_const_type();

                if (new_type.type_family > cc::TYPE_FAMILY_INTEGER || const_type.type_family > cc::TYPE_FAMILY_INTEGER) {
                    return true;
                } else if (new_type.type_family == cc::TYPE_FAMILY_INTEGER) {
                    const ConstExprent fresh(const_expr->get_int_value(), false, std::nullopt);
                    const VarType min_integer = fresh.get_const_type();
                    if (min_integer.is_strict_superset(new_type)) new_type = min_integer;
                }
            }

            VarVersionPair pair;
            if (exprent->type == Exprent::EXPRENT_CONST) pair = VarVersionPair(exprent->id, -1);
            else pair = VarVersionPair(static_cast<const VarExprent*>(exprent));

            if (min_max == 0) {  // min
                const VarType* current_min_p = min_type_of(map_exprent_min_types_, pair);
                std::optional<VarType> current_min_type;
                if (current_min_p != nullptr) current_min_type = *current_min_p;

                VarType new_min_type;
                if (!current_min_type || new_type.type_family > current_min_type->type_family) {
                    new_min_type = new_type;
                } else if (new_type.type_family < current_min_type->type_family) {
                    return true;
                } else {
                    new_min_type = VarType::common_supertype(*current_min_type, new_type).value();
                }

                map_exprent_min_types_.put(pair, new_min_type);

                if (exprent->type == Exprent::EXPRENT_CONST) static_cast<ConstExprent*>(exprent)->set_const_type(new_min_type);

                if (current_min_type && (new_min_type.type_family > current_min_type->type_family || new_min_type.is_strict_superset(*current_min_type))) {
                    return false;
                }
            } else {  // max
                const VarType* current_max_p = max_type_of(map_exprent_max_types_, pair);
                std::optional<VarType> current_max_type;
                if (current_max_p != nullptr) current_max_type = *current_max_p;

                std::optional<VarType> new_max_type;
                if (!current_max_type || new_type.type_family < current_max_type->type_family) {
                    new_max_type = new_type;
                } else if (new_type.type_family > current_max_type->type_family) {
                    return true;
                } else {
                    new_max_type = VarType::common_min_type(*current_max_type, new_type);
                }

                map_exprent_max_types_.put(pair, new_max_type);
            }
            break;
        }
        case Exprent::EXPRENT_ASSIGNMENT:
            return change_exprent_type(static_cast<AssignmentExprent*>(exprent)->get_right(), new_type, min_max);
        case Exprent::EXPRENT_FUNCTION: {
            FunctionExprent* func = static_cast<FunctionExprent*>(exprent);
            switch (func->get_func_type()) {
                case FunctionExprent::FUNCTION_IIF: {  // FIXME:
                    const bool a = change_exprent_type(func->get_lst_operands().at(1), new_type, min_max);
                    const bool b = change_exprent_type(func->get_lst_operands().at(2), new_type, min_max);
                    res = a && b;
                    break;
                }
                case FunctionExprent::FUNCTION_AND:
                case FunctionExprent::FUNCTION_OR:
                case FunctionExprent::FUNCTION_XOR: {
                    const bool a = change_exprent_type(func->get_lst_operands().at(0), new_type, min_max);
                    const bool b = change_exprent_type(func->get_lst_operands().at(1), new_type, min_max);
                    res = a && b;
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

    return res;
}

}  // namespace cb::java::decompiler
