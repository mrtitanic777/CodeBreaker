// java/decompiler/nested_class_processor.cpp -- see classes.hpp. Ported from Fernflower (de.fernflower.main.rels.NestedClassProcessor), Copyright
// 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <algorithm>
#include <deque>
#include <functional>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include "java/decompiler/attributes.hpp"
#include "java/decompiler/classes.hpp"
#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/exprents.hpp"
#include "java/decompiler/sforms.hpp"
#include "java/decompiler/statements.hpp"
#include "java/decompiler/vars.hpp"

namespace cb::java::decompiler {

namespace {

// InterpreterUtil.equalObjects of two (nullable) pairs.
template <class P>
bool equal_pairs(const P* a, const P* b) {
    if (a == nullptr || b == nullptr) return a == b;
    return a->equals(b);
}

// new HashSet<>(values) after removeAll(taken): the names that are new.
std::set<std::string> to_set(const std::vector<std::string>& v) { return std::set<std::string>(v.begin(), v.end()); }

}  // namespace

void NestedClassProcessor::process_class(ClassNode* root, ClassNode* node) {
    // hide synthetic lambda content methods
    if (node->type == ClassNode::CLASS_LAMBDA && !node->lambda_information->is_method_reference) {
        ClassNode* node_content = DecompilerContext::current().class_processor->get_class_node(node->class_struct->qualified_name);
        if (node_content != nullptr && node_content->get_wrapper() != nullptr) {
            node_content->get_wrapper()->get_hidden_members().insert(node->lambda_information->content_method_key);
        }
    }

    if (node->nested.empty()) return;

    if (node->type != ClassNode::CLASS_LAMBDA) {
        compute_local_vars_and_definitions(node);

        // for each local or anonymous class ensure not empty enclosing method
        check_not_found_classes(root, node);
    }

    int nameless = 0, synthetics = 0;
    for (ClassNode* child : node->nested) {
        // ensure not-empty class name
        if ((child->type == ClassNode::CLASS_LOCAL || child->type == ClassNode::CLASS_MEMBER) && !child->simple_name) {
            const StructClass* cl = child->class_struct;
            if ((child->access & cc::ACC_SYNTHETIC) != 0 || cl->is_synthetic()) {
                child->simple_name = "SyntheticClass_" + std::to_string(++synthetics);
            } else {
                const std::string message = "Nameless local or member class " + cl->qualified_name + "!";
                DecompilerContext::get_logger().write_message(message, IFernflowerLogger::Severity::WARN);
                child->simple_name = "NamelessClass_" + std::to_string(++nameless);
            }
            child->nameless_constructor_stub = !cl->has_modifier(cc::ACC_STATIC) && cl->methods().size() + cl->fields().size() == 0;
        }
    }

    for (ClassNode* child : node->nested) {
        if (child->type == ClassNode::CLASS_LAMBDA) {
            set_lambda_vars(node, child);
        } else if (child->type != ClassNode::CLASS_MEMBER || (child->access & cc::ACC_STATIC) == 0) {
            insert_local_vars(node, child);

            if (child->type == ClassNode::CLASS_LOCAL) {
                set_local_class_definition(node->get_wrapper()->get_methods().get_with_key(child->enclosing_method.value_or(std::string())), child);
            }
        }
    }

    for (ClassNode* child : node->nested) process_class(root, child);
}

void NestedClassProcessor::set_lambda_vars(ClassNode* parent, ClassNode* child) {
    if (child->lambda_information->is_method_reference) {   // method reference, no code and no parameters
        return;
    }

    MethodWrapper* const meth = parent->get_wrapper()->get_methods().get_with_key(child->lambda_information->content_method_key);
    MethodWrapper* const encmeth = parent->get_wrapper()->get_methods().get_with_key(child->enclosing_method.value_or(std::string()));

    const MethodDescriptor md_lambda = MethodDescriptor::parse(child->lambda_information->method_descriptor);
    const MethodDescriptor md_content = MethodDescriptor::parse(child->lambda_information->content_method_descriptor);

    const int vars_count = static_cast<int>(md_content.params.size()) - static_cast<int>(md_lambda.params.size());
    //		if(vars_count < 0) { // should not happen, but just in case...
    //			vars_count = 0;
    //		}

    const bool is_static_lambda_content = child->lambda_information->is_content_method_static;

    const std::string parent_class_name = parent->get_wrapper()->get_class_struct()->qualified_name;
    const std::string lambda_class_name = child->simple_name.value_or("null");

    const VarType lambda_class_type = VarType::parse(lambda_class_name, true);

    // this pointer
    if (!is_static_lambda_content && DecompilerContext::option(pref::LAMBDA_TO_ANONYMOUS_CLASS)) {
        meth->varproc->get_this_vars().put(VarVersionPair(0, 0), parent_class_name);
        meth->varproc->set_var_name(VarVersionPair(0, 0), parent->simple_name.value_or("null") + ".this");
    }

    // local variables
    DirectGraph* graph = encmeth->get_or_build_graph();

    VarVersionMap<std::string> map_new_names;

    graph->iterate_exprents([&](Exprent* exprent) {
        std::vector<Exprent*> lst = exprent->get_all_exprents(true);
        lst.push_back(exprent);

        for (Exprent* expr : lst) {
            if (expr->type == Exprent::EXPRENT_NEW) {
                auto* new_expr = static_cast<NewExprent*>(expr);
                if (new_expr->is_lambda() && lambda_class_type == new_expr->get_new_type()) {
                    InvocationExprent* inv_dynamic = new_expr->get_constructor();

                    const int param_index = is_static_lambda_content ? 0 : 1;
                    int varindex = is_static_lambda_content ? 0 : 1;

                    for (int i = 0; i < vars_count; ++i) {
                        Exprent* param = inv_dynamic->get_lst_parameters().at(static_cast<std::size_t>(param_index + i));

                        if (param->type == Exprent::EXPRENT_VAR) {
                            const VarVersionPair enc_varpaar(static_cast<VarExprent*>(param));
                            const std::string enc_varname = encmeth->varproc->get_var_name(enc_varpaar).value();

                            //meth.varproc.setVarName(new VarVersionPair(varindex, 0), enc_varname);
                            map_new_names.put(VarVersionPair(varindex, 0), enc_varname);
                        }

                        varindex += md_content.params[static_cast<std::size_t>(i)].stack_size;
                    }
                }
            }
        }

        return 0;
    });

    // update names of local variables
    JavaHashSet<std::string, JavaStringHash> set_new_outer_names = JavaHashSet<std::string, JavaStringHash>::of(map_new_names.values());
    for (const std::string& n : meth->set_outer_var_names) set_new_outer_names.remove(n);

    VarNamesCollector outer_names(to_set(set_new_outer_names.items()));
    meth->varproc->refresh_var_names(outer_names);
    for (const std::string& n : set_new_outer_names.items()) meth->set_outer_var_names.insert(n);

    for (const auto& [pair, name] : map_new_names.entries()) meth->varproc->set_var_name(pair, name);
}

void NestedClassProcessor::check_not_found_classes(ClassNode* root, ClassNode* node) {
    const std::vector<ClassNode*> lst_children = node->nested;

    for (ClassNode* child : lst_children) {
        if ((child->type == ClassNode::CLASS_LOCAL || child->type == ClassNode::CLASS_ANONYMOUS) && !child->enclosing_method) {
            JavaHashSet<std::string, JavaStringHash>& set_enclosing = child->enclosing_classes;

            if (set_enclosing.size() == 1) {
                const EnclosingMethodAttribute* attr_em = child->class_struct->attribute<EnclosingMethodAttribute>(attr::ENCLOSING_METHOD);
                if (attr_em != nullptr && attr_em->method_name()) {
                    if (node->class_struct->qualified_name == attr_em->class_name() &&
                        node->class_struct->get_method(*attr_em->method_name(), attr_em->method_descriptor().value()) != nullptr) {
                        child->enclosing_method = InterpreterUtil::make_unique_key(*attr_em->method_name(), attr_em->method_descriptor().value());
                        continue;
                    }
                }
            }

            node->nested.erase(std::find(node->nested.begin(), node->nested.end(), child));
            child->parent = nullptr;
            set_enclosing.remove(node->class_struct->qualified_name);

            bool has_enclosing = !set_enclosing.empty();
            if (has_enclosing) has_enclosing = insert_nested_class(root, child);

            if (!has_enclosing) {
                if (child->type == ClassNode::CLASS_ANONYMOUS) {
                    if (!child->class_struct->has_modifier(cc::ACC_SYNTHETIC)) {
                        const std::string message = "Unreferenced anonymous class " + child->class_struct->qualified_name + "!";
                        DecompilerContext::get_logger().write_message(message, IFernflowerLogger::Severity::WARN);
                    }
                } else if (child->type == ClassNode::CLASS_LOCAL) {
                    const std::string message = "Unreferenced local class " + child->class_struct->qualified_name + "!";
                    DecompilerContext::get_logger().write_message(message, IFernflowerLogger::Severity::WARN);
                }
            }
        }
    }
}

bool NestedClassProcessor::insert_nested_class(ClassNode* root, ClassNode* child) {
    const JavaHashSet<std::string, JavaStringHash>& set_enclosing = child->enclosing_classes;

    std::deque<ClassNode*> stack;
    stack.push_back(root);

    while (!stack.empty()) {
        ClassNode* node = stack.front();
        stack.pop_front();

        if (set_enclosing.contains(node->class_struct->qualified_name)) {
            node->nested.push_back(child);
            child->parent = node;

            return true;
        }

        // note: ordered list
        for (ClassNode* nd : node->nested) stack.push_back(nd);
    }

    return false;
}

void NestedClassProcessor::compute_local_vars_and_definitions(ClassNode* node) {
    // local var masks
    // class name, constructor descriptor, field mask
    JavaHashMap<std::string, FieldMasks, JavaStringHash> map_var_masks;

    int cltypes = 0;

    for (ClassNode* nd : node->nested) {
        if (nd->type != ClassNode::CLASS_LAMBDA) {
            if ((nd->access & cc::ACC_STATIC) == 0 && (nd->access & cc::ACC_INTERFACE) == 0) {
                cltypes |= nd->type;

                FieldMasks mask = get_mask_local_vars(nd->get_wrapper());
                if (mask.empty()) {
                    if (!nd->class_struct->has_modifier(cc::ACC_SYNTHETIC)) {
                        const std::string message = "Nested class " + nd->class_struct->qualified_name + " has no constructor!";
                        DecompilerContext::get_logger().write_message(message, IFernflowerLogger::Severity::WARN);
                    }
                } else {
                    map_var_masks.put(nd->class_struct->qualified_name, mask);
                }
            }
        }
    }

    // local var masks
    JavaHashMap<std::string, FieldMasks, JavaStringHash> map_var_field_pairs;

    if (cltypes != ClassNode::CLASS_MEMBER) {
        // iterate enclosing class
        for (MethodWrapper* meth : node->get_wrapper()->get_methods()) {
            if (meth->root != nullptr) {   // neither abstract, nor native
                DirectGraph* graph = meth->get_or_build_graph();

                graph->iterate_exprents([&](Exprent* exprent) {
                    std::vector<Exprent*> lst = exprent->get_all_exprents(true);
                    lst.push_back(exprent);

                    for (Exprent* expr : lst) {
                        if (expr->type == Exprent::EXPRENT_NEW) {
                            InvocationExprent* constr = static_cast<NewExprent*>(expr)->get_constructor();

                            if (constr != nullptr && map_var_masks.contains_key(constr->get_classname())) {   // non-static inner class constructor
                                const std::string refclname = constr->get_classname();

                                ClassNode* nested_class_node = node->get_class_node(refclname);

                                if (nested_class_node->type != ClassNode::CLASS_MEMBER) {
                                    const std::vector<VarFieldPair*>* mask_ptr = map_var_masks.get(refclname)->get(constr->get_string_descriptor());
                                    if (mask_ptr == nullptr) throw std::runtime_error("NullPointerException");
                                    const std::vector<VarFieldPair*> mask = *mask_ptr;

                                    if (!map_var_field_pairs.contains_key(refclname)) map_var_field_pairs.put(refclname, FieldMasks());

                                    std::vector<VarFieldPair*> lst_temp;

                                    for (std::size_t i = 0; i < mask.size(); ++i) {
                                        Exprent* param = constr->get_lst_parameters().at(i);
                                        VarFieldPair* pair = nullptr;

                                        if (param->type == Exprent::EXPRENT_VAR && mask[i] != nullptr) {
                                            const VarVersionPair varpaar(static_cast<VarExprent*>(param));

                                            // FIXME (the original's): final flags of variables are wrong! Correct the entire final functionality.
                                            //													if(meth.varproc.getVarFinal(varpaar) != VarTypeProcessor.VAR_NON_FINAL) {
                                            pair = make<VarFieldPair>(mask[i]->keyfield, varpaar);
                                            //													}
                                        }

                                        lst_temp.push_back(pair);
                                    }

                                    FieldMasks* pair_masks = map_var_field_pairs.get(refclname);
                                    const std::vector<VarFieldPair*>* existing = pair_masks->get(constr->get_string_descriptor());

                                    std::vector<VarFieldPair*> pairmask;
                                    if (existing == nullptr) {
                                        pairmask = lst_temp;
                                    } else {
                                        pairmask = *existing;
                                        for (std::size_t i = 0; i < pairmask.size(); ++i) {
                                            if (!equal_pairs(pairmask[i], lst_temp.at(i))) pairmask[i] = nullptr;
                                        }
                                    }

                                    pair_masks->put(constr->get_string_descriptor(), pairmask);
                                    nested_class_node->enclosing_method = InterpreterUtil::make_unique_key(meth->method_struct->name(), meth->method_struct->descriptor());
                                }
                            }
                        }
                    }

                    return 0;
                });
            }
        }
    }

    // merge var masks
    for (const auto& entcl : map_var_masks.entries()) {
        ClassNode* nested_node = node->get_class_node(entcl.first);

        // intersection
        std::optional<std::vector<VarFieldPair*>> intr_pair_mask;
        // merge referenced constructors
        if (map_var_field_pairs.contains_key(entcl.first)) {
            for (std::vector<VarFieldPair*> mask : map_var_field_pairs.get(entcl.first)->values()) {
                if (!intr_pair_mask) {
                    intr_pair_mask = mask;
                } else {
                    merge_list_signatures(*intr_pair_mask, mask, false);
                }
            }
        }

        std::optional<std::vector<VarFieldPair*>> intr_mask;
        // merge all constructors
        for (std::vector<VarFieldPair*> mask : entcl.second.values()) {
            if (!intr_mask) {
                intr_mask = mask;
            } else {
                merge_list_signatures(*intr_mask, mask, false);
            }
        }

        if (!intr_pair_mask) {   // member or local and never instantiated
            intr_pair_mask = *intr_mask;

            bool found = false;

            for (std::size_t i = 0; i < intr_pair_mask->size(); ++i) {
                if ((*intr_pair_mask)[i] != nullptr) {
                    if (found) (*intr_pair_mask)[i] = nullptr;
                    found = true;
                }
            }
        }

        merge_list_signatures(*intr_pair_mask, *intr_mask, true);

        for (std::size_t i = 0; i < intr_pair_mask->size(); ++i) {
            VarFieldPair* pair = (*intr_pair_mask)[i];
            if (pair != nullptr && !pair->keyfield.empty()) nested_node->map_fields_to_vars.put(pair->keyfield, pair->varpaar);
        }

        // set resulting constructor signatures
        for (auto entmt : entcl.second.entries()) {
            merge_list_signatures(entmt.second, *intr_pair_mask, false);

            MethodWrapper* meth = nested_node->get_wrapper()->get_method_wrapper(std::string(cc::INIT_NAME), entmt.first);
            meth->signature_fields = std::vector<std::optional<VarVersionPair>>();

            for (VarFieldPair* pair : entmt.second) {
                meth->signature_fields->push_back(pair == nullptr ? std::nullopt : std::optional<VarVersionPair>(pair->varpaar));
            }
        }
    }
}

void NestedClassProcessor::insert_local_vars(ClassNode* parent, ClassNode* child) {
    // enclosing method, is null iff member class
    MethodWrapper* encmeth = parent->get_wrapper()->get_methods().get_with_key(child->enclosing_method.value_or(std::string()));

    // iterate all child methods
    for (MethodWrapper* meth : child->get_wrapper()->get_methods()) {
        if (meth->root != nullptr) {   // neither abstract nor native
            // local var names (a name is none where the original's is null)
            VarVersionMap<std::optional<std::string>> map_new_names;
            // local var types
            VarVersionMap<std::optional<VarType>> map_new_types;

            std::map<int, VarVersionPair> map_params_to_new_vars;
            if (meth->signature_fields) {
                std::size_t index = 0;
                int varindex = 1;
                const MethodDescriptor md = MethodDescriptor::parse(meth->method_struct->descriptor());

                for (const std::optional<VarVersionPair>& paar : *meth->signature_fields) {
                    if (paar) {
                        const VarVersionPair newvar(meth->counter->get_counter_and_increment(CounterContainer::VAR_COUNTER), 0);

                        map_params_to_new_vars[varindex] = newvar;

                        std::optional<std::string> varname;
                        std::optional<VarType> vartype;

                        if (child->type != ClassNode::CLASS_MEMBER) {
                            varname = encmeth->varproc->get_var_name(*paar);
                            vartype = encmeth->varproc->get_var_type(*paar);

                            encmeth->varproc->set_var_final(*paar, VarTypeProcessor::VAR_EXPLICIT_FINAL);
                        }

                        if (paar->var == -1 || (varname && *varname == "this")) {
                            if (!parent->simple_name) {
                                // anonymous enclosing class, no access to this
                                varname = std::string(VarExprent::VAR_NAMELESS_ENCLOSURE);
                            } else {
                                varname = *parent->simple_name + ".this";
                            }
                            meth->varproc->get_this_vars().put(newvar, parent->class_struct->qualified_name);
                        }

                        map_new_names.put(newvar, varname);
                        map_new_types.put(newvar, vartype);
                    }
                    varindex += md.params.at(index++).stack_size;
                }
            }

            // new vars
            std::map<std::string, VarVersionPair> map_fields_to_new_vars;

            for (ClassNode* clnode = child; clnode != nullptr; clnode = clnode->parent) {
                for (const auto& entr : clnode->map_fields_to_vars.entries()) {
                    const VarVersionPair newvar(meth->counter->get_counter_and_increment(CounterContainer::VAR_COUNTER), 0);

                    map_fields_to_new_vars[InterpreterUtil::make_unique_key(clnode->class_struct->qualified_name, entr.first)] = newvar;

                    std::optional<std::string> varname;
                    std::optional<VarType> vartype;

                    if (clnode->type != ClassNode::CLASS_MEMBER) {
                        MethodWrapper* enclosing_method = clnode->parent->get_wrapper()->get_methods().get_with_key(clnode->enclosing_method.value_or(std::string()));

                        varname = enclosing_method->varproc->get_var_name(entr.second);
                        vartype = enclosing_method->varproc->get_var_type(entr.second);

                        enclosing_method->varproc->set_var_final(entr.second, VarTypeProcessor::VAR_EXPLICIT_FINAL);
                    }

                    if (entr.second.var == -1 || (varname && *varname == "this")) {
                        if (!clnode->parent->simple_name) {
                            // anonymous enclosing class, no access to this
                            varname = std::string(VarExprent::VAR_NAMELESS_ENCLOSURE);
                        } else {
                            varname = *clnode->parent->simple_name + ".this";
                        }
                        meth->varproc->get_this_vars().put(newvar, clnode->parent->class_struct->qualified_name);
                    }

                    map_new_names.put(newvar, varname);
                    map_new_types.put(newvar, vartype);

                    // hide synthetic field
                    if (clnode == child) {   // fields higher up the chain were already handled with their classes
                        const std::shared_ptr<StructField> fd = child->class_struct->fields().get_with_key(entr.first);
                        child->get_wrapper()->get_hidden_members().insert(InterpreterUtil::make_unique_key(fd->name(), fd->descriptor()));
                    }
                }
            }

            std::vector<std::string> new_names;
            for (const std::optional<std::string>& n : map_new_names.values()) {
                if (n) new_names.push_back(*n);
            }
            JavaHashSet<std::string, JavaStringHash> set_new_outer_names = JavaHashSet<std::string, JavaStringHash>::of(new_names);
            for (const std::string& n : meth->set_outer_var_names) set_new_outer_names.remove(n);

            VarNamesCollector outer_names(to_set(set_new_outer_names.items()));
            meth->varproc->refresh_var_names(outer_names);
            for (const std::string& n : set_new_outer_names.items()) meth->set_outer_var_names.insert(n);

            for (const auto& entr : map_new_names.entries()) {
                const VarVersionPair& varpaar = entr.first;
                const std::optional<VarType>* vartype = map_new_types.get(varpaar);

                if (entr.second) meth->varproc->set_var_name(varpaar, *entr.second);
                if (vartype != nullptr && vartype->has_value()) meth->varproc->set_var_type(varpaar, **vartype);
            }

            DirectGraph* graph = meth->get_or_build_graph();

            // the original's recursive replaceExprent of its iterator
            std::function<Exprent*(Exprent*)> replace_exprent = [&](Exprent* exprent) -> Exprent* {
                if (exprent->type == Exprent::EXPRENT_VAR) {
                    const int varindex = static_cast<VarExprent*>(exprent)->get_index();
                    const auto found = map_params_to_new_vars.find(varindex);
                    if (found != map_params_to_new_vars.end()) {
                        const VarVersionPair newvar = found->second;
                        meth->varproc->get_external_vars().add(newvar);
                        return make<VarExprent>(newvar.var, meth->varproc->get_var_type(newvar), meth->varproc);
                    }
                } else if (exprent->type == Exprent::EXPRENT_FIELD) {
                    auto* fexpr = static_cast<FieldExprent*>(exprent);

                    const std::string key_field =
                        InterpreterUtil::make_unique_key(fexpr->get_classname(), InterpreterUtil::make_unique_key(fexpr->get_name(), fexpr->get_descriptor().descriptor_string));

                    const auto found = map_fields_to_new_vars.find(key_field);
                    if (found != map_fields_to_new_vars.end()) {
                        //if(fexpr.getClassname().equals(child.classStruct.qualifiedName) &&
                        //		mapFieldsToNewVars.containsKey(keyField)) {
                        const VarVersionPair newvar = found->second;
                        meth->varproc->get_external_vars().add(newvar);
                        return make<VarExprent>(newvar.var, meth->varproc->get_var_type(newvar), meth->varproc);
                    }
                }

                bool replaced = true;
                while (replaced) {
                    replaced = false;

                    for (Exprent* expr : exprent->get_all_exprents()) {
                        Exprent* retexpr = replace_exprent(expr);
                        if (retexpr != nullptr) {
                            exprent->replace_exprent(expr, retexpr);
                            replaced = true;
                            break;
                        }
                    }
                }

                return nullptr;
            };

            graph->iterate_exprents([&](Exprent* exprent) {
                if (exprent->type == Exprent::EXPRENT_ASSIGNMENT) {
                    auto* asexpr = static_cast<AssignmentExprent*>(exprent);
                    if (asexpr->get_left()->type == Exprent::EXPRENT_FIELD) {
                        auto* fexpr = static_cast<FieldExprent*>(asexpr->get_left());

                        if (fexpr->get_classname() == child->class_struct->qualified_name &&   // process this class only
                            map_fields_to_new_vars.contains(InterpreterUtil::make_unique_key(
                                child->class_struct->qualified_name, InterpreterUtil::make_unique_key(fexpr->get_name(), fexpr->get_descriptor().descriptor_string)))) {
                            return 2;
                        }

                        //if(fexpr.getClassname().equals(child.classStruct.qualifiedName) &&
                        //		mapFieldsToNewVars.containsKey(InterpreterUtil.makeUniqueKey(fexpr.getName(), fexpr.getDescriptor().descriptorString))) {
                        //	return 2;
                        //}
                    }
                }

                if (child->type == ClassNode::CLASS_ANONYMOUS && cc::INIT_NAME == meth->method_struct->name() && exprent->type == Exprent::EXPRENT_INVOCATION) {
                    auto* invexpr = static_cast<InvocationExprent*>(exprent);
                    if (invexpr->get_functype() == InvocationExprent::TYP_INIT) {
                        // invocation of the super constructor in an anonymous class
                        child->super_invocation = invexpr;   // FIXME (the original's): save original names of parameters
                        return 2;
                    }
                }

                replace_exprent(exprent);

                return 0;
            });
        }
    }
}

NestedClassProcessor::FieldMasks NestedClassProcessor::get_mask_local_vars(ClassWrapper* wrapper) {
    FieldMasks map_masks;

    const StructClass* cl = wrapper->get_class_struct();

    // iterate over constructors
    for (const auto& mt : cl->methods()) {
        if (cc::INIT_NAME == mt->name()) {
            const MethodDescriptor md = MethodDescriptor::parse(mt->descriptor());

            MethodWrapper* meth = wrapper->get_method_wrapper(std::string(cc::INIT_NAME), mt->descriptor());
            DirectGraph* graph = meth->get_or_build_graph();

            if (graph != nullptr) {   // something gone wrong, should not be null
                std::vector<VarFieldPair*> fields;

                int varindex = 1;
                for (std::size_t i = 0; i < md.params.size(); ++i) {   // no static methods allowed
                    const std::optional<std::string> key_field = get_enclosing_var_field(cl, meth, graph, varindex);
                    fields.push_back(!key_field ? nullptr : make<VarFieldPair>(*key_field, VarVersionPair(-1, 0)));   // TODO (the original's): null?

                    varindex += md.params[i].stack_size;
                }

                map_masks.put(mt->descriptor(), fields);
            }
        }
    }

    return map_masks;
}

std::optional<std::string> NestedClassProcessor::get_enclosing_var_field(const StructClass* cl, MethodWrapper* meth, DirectGraph* graph, int index) {
    std::string field;

    // parameter variable final
    if (meth->varproc->get_var_final(VarVersionPair(index, 0)) == VarTypeProcessor::VAR_NON_FINAL) return std::nullopt;

    const bool no_synth_flag = DecompilerContext::option(pref::SYNTHETIC_NOT_SET);

    // no loop at the begin
    DirectNode* firstnode = graph->first;
    if (firstnode->preds.empty()) {
        // assignment to a final synthetic field?
        for (Exprent* exprent : *firstnode->exprents) {
            if (exprent->type == Exprent::EXPRENT_ASSIGNMENT) {
                auto* asexpr = static_cast<AssignmentExprent*>(exprent);

                if (asexpr->get_right()->type == Exprent::EXPRENT_VAR && static_cast<VarExprent*>(asexpr->get_right())->get_index() == index) {
                    if (asexpr->get_left()->type == Exprent::EXPRENT_FIELD) {
                        auto* left = static_cast<FieldExprent*>(asexpr->get_left());
                        const StructField* fd = cl->get_field(left->get_name(), left->get_descriptor().descriptor_string);

                        if (fd != nullptr) {   // local (== not inherited) field
                            if (cl->qualified_name == left->get_classname() && fd->has_modifier(cc::ACC_FINAL) &&
                                (fd->is_synthetic() || (no_synth_flag && fd->has_modifier(cc::ACC_PRIVATE)))) {
                                field = InterpreterUtil::make_unique_key(left->get_name(), left->get_descriptor().descriptor_string);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    return field;
}

void NestedClassProcessor::merge_list_signatures(std::vector<VarFieldPair*>& first, std::vector<VarFieldPair*>& second, bool both) {
    int i = 1;
    while (true) {
        if (static_cast<int>(first.size()) <= i || static_cast<int>(second.size()) <= i) break;

        VarFieldPair* fobj = first[first.size() - static_cast<std::size_t>(i)];
        VarFieldPair* sobj = second[second.size() - static_cast<std::size_t>(i)];

        bool eq = false;
        if (fobj == nullptr || sobj == nullptr) {
            eq = (fobj == sobj);
        } else {
            eq = true;
            if (fobj->keyfield.empty()) {
                fobj->keyfield = sobj->keyfield;
            } else if (sobj->keyfield.empty()) {
                if (both) sobj->keyfield = fobj->keyfield;
            } else {
                eq = fobj->keyfield == sobj->keyfield;
            }
        }

        if (!eq) {
            first[first.size() - static_cast<std::size_t>(i)] = nullptr;
            if (both) second[second.size() - static_cast<std::size_t>(i)] = nullptr;
        } else {
            if (fobj != nullptr) {
                if (fobj->varpaar.var == -1) {
                    fobj->varpaar = sobj->varpaar;
                } else {
                    sobj->varpaar = fobj->varpaar;
                }
            }
        }

        ++i;
    }

    for (int j = 1; j <= static_cast<int>(first.size()) - i; ++j) first.at(static_cast<std::size_t>(j)) = nullptr;

    if (both) {
        for (int j = 1; j <= static_cast<int>(second.size()) - i; ++j) second.at(static_cast<std::size_t>(j)) = nullptr;
    }

    // first
    if (first.empty()) {
        if (!second.empty() && both) second[0] = nullptr;
    } else if (second.empty()) {
        first[0] = nullptr;
    } else {
        VarFieldPair* fobj = first[0];
        VarFieldPair* sobj = second[0];

        bool eq = false;
        if (fobj == nullptr || sobj == nullptr) {
            eq = (fobj == sobj);
        } else {
            eq = true;
            if (fobj->keyfield.empty()) {
                fobj->keyfield = sobj->keyfield;
            } else if (sobj->keyfield.empty()) {
                if (both) sobj->keyfield = fobj->keyfield;
            } else {
                eq = fobj->keyfield == sobj->keyfield;
            }
        }

        if (!eq) {
            first[0] = nullptr;
            if (both) second[0] = nullptr;
        } else if (fobj != nullptr) {
            if (fobj->varpaar.var == -1) {
                fobj->varpaar = sobj->varpaar;
            } else {
                sobj->varpaar = fobj->varpaar;
            }
        }
    }
}

void NestedClassProcessor::set_local_class_definition(MethodWrapper* meth, ClassNode* node) {
    RootStatement* root = meth->root;

    LinkedSet<Statement*> set_stats;
    const VarType classtype = VarType::parse(node->class_struct->qualified_name, true);

    Statement* stdef = get_def_statement(root, classtype, set_stats);
    if (stdef == nullptr) {
        // unreferenced local class
        stdef = root->get_first();
    }

    Statement* first = find_first_block(stdef, set_stats);

    std::vector<Exprent*>* lst;
    if (first == nullptr) {
        lst = &stdef->get_var_definitions();
    } else if (!first->get_exprents().has_value()) {
        lst = &first->get_var_definitions();
    } else {
        lst = &*first->get_exprents();
    }

    std::size_t addindex = 0;
    for (Exprent* expr : *lst) {
        if (search_for_class(expr, classtype)) break;
        ++addindex;
    }

    VarExprent* var = make<VarExprent>(meth->counter->get_counter_and_increment(CounterContainer::VAR_COUNTER), classtype, meth->varproc);
    var->set_definition(true);
    var->set_class_def(true);

    lst->insert(lst->begin() + static_cast<std::ptrdiff_t>(addindex), var);
}

Statement* NestedClassProcessor::find_first_block(Statement* stat, LinkedSet<Statement*>& set_stats) {
    std::deque<Statement*> stack;
    stack.push_back(stat);

    while (!stack.empty()) {
        Statement* st = stack.front();
        stack.pop_front();

        if (stack.empty() || set_stats.contains(st)) {
            if (st->is_labeled() && !stack.empty()) return st;

            if (st->get_exprents().has_value()) return st;

            stack.clear();

            switch (st->type) {
                case Statement::TYPE_SEQUENCE:
                    stack.insert(stack.begin(), st->get_stats().items().begin(), st->get_stats().items().end());
                    break;
                case Statement::TYPE_IF:
                case Statement::TYPE_ROOT:
                case Statement::TYPE_SWITCH:
                case Statement::TYPE_SYNCRONIZED:
                    stack.push_back(st->get_first());
                    break;
                default:
                    return st;
            }
        }
    }

    return nullptr;
}

Statement* NestedClassProcessor::get_def_statement(Statement* stat, const VarType& classtype, LinkedSet<Statement*>& set_stats) {
    std::vector<Exprent*> condlst;
    Statement* retstat = nullptr;

    if (!stat->get_exprents().has_value()) {
        int counter = 0;

        for (const SeqObject& obj : stat->get_sequential_objects()) {
            if (obj.stat != nullptr) {
                Statement* st = obj.stat;

                Statement* st_temp = get_def_statement(st, classtype, set_stats);

                if (st_temp != nullptr) {
                    if (counter == 1) {
                        retstat = stat;
                        break;
                    }
                    retstat = st_temp;
                    ++counter;
                }

                if (st->type == DoStatement::TYPE_DO) {
                    auto* dost = static_cast<DoStatement*>(st);

                    condlst.insert(condlst.end(), dost->get_init_exprent_list().begin(), dost->get_init_exprent_list().end());
                    condlst.insert(condlst.end(), dost->get_condition_exprent_list().begin(), dost->get_condition_exprent_list().end());
                }
            } else if (obj.expr != nullptr) {
                condlst.push_back(obj.expr);
            }
        }
    } else {
        condlst = *stat->get_exprents();
    }

    if (retstat != stat) {
        for (Exprent* exprent : condlst) {
            if (exprent != nullptr && search_for_class(exprent, classtype)) {
                retstat = stat;
                break;
            }
        }
    }

    if (retstat != nullptr) set_stats.add(stat);

    return retstat;
}

bool NestedClassProcessor::search_for_class(Exprent* exprent, const VarType& classtype) {
    std::vector<Exprent*> lst = exprent->get_all_exprents(true);
    lst.push_back(exprent);

    const std::string classname = classtype.value.value();

    for (Exprent* expr : lst) {
        bool res = false;

        switch (expr->type) {
            case Exprent::EXPRENT_CONST: {
                auto* cexpr = static_cast<ConstExprent*>(expr);
                const auto* value = std::get_if<std::string>(&cexpr->get_value());
                res = (VarType::CLASS == cexpr->get_const_type() && value != nullptr && classname == *value) || classtype == cexpr->get_const_type();
                break;
            }
            case Exprent::EXPRENT_FIELD:
                res = classname == static_cast<FieldExprent*>(expr)->get_classname();
                break;
            case Exprent::EXPRENT_INVOCATION:
                res = classname == static_cast<InvocationExprent*>(expr)->get_classname();
                break;
            case Exprent::EXPRENT_NEW: {
                const VarType new_type = expr->get_expr_type();
                res = new_type.type == cc::TYPE_OBJECT && new_type.value && classname == *new_type.value;
                break;
            }
            case Exprent::EXPRENT_VAR: {
                auto* vexpr = static_cast<VarExprent*>(expr);
                if (vexpr->is_definition()) {
                    const VarType vtype = vexpr->get_var_type();
                    if (classtype == vtype || (vtype.array_dim > 0 && classtype.value == vtype.value)) res = true;
                }
                break;
            }
            default:
                break;
        }

        if (res) return true;
    }

    return false;
}

}  // namespace cb::java::decompiler
