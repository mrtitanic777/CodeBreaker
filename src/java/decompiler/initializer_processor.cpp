// java/decompiler/initializer_processor.cpp -- see classes.hpp. Ported from Fernflower (de.fernflower.main.InitializerProcessor), Copyright 2000-2015
// JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <string>
#include <vector>

#include "java/decompiler/classes.hpp"
#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/exprents.hpp"
#include "java/decompiler/statements.hpp"
#include "java/decompiler/vars.hpp"

namespace cb::java::decompiler {

void InitializerProcessor::extract_initializers(ClassWrapper* wrapper) {
    MethodWrapper* meth = wrapper->get_method_wrapper(std::string(cc::CLINIT_NAME), "()V");

    if (meth != nullptr && meth->root != nullptr) {   // successfully decompiled static constructor
        extract_static_initializers(wrapper, meth);
    }

    extract_dynamic_initializers(wrapper);

    // required e.g. if anonymous class is being decompiled as a standard one.
    // This can happen if InnerClasses attributes are erased
    lift_constructor(wrapper);

    if (DecompilerContext::option(pref::HIDE_EMPTY_SUPER)) hide_empty_super(wrapper);
}

void InitializerProcessor::lift_constructor(ClassWrapper* wrapper) {
    for (MethodWrapper* meth : wrapper->get_methods()) {
        if (cc::INIT_NAME == meth->method_struct->name() && meth->root != nullptr) {
            Statement* firstdata = find_first_data(meth->root);
            if (firstdata == nullptr) return;

            int index = 0;
            std::vector<Exprent*>& lst_exprents = *firstdata->get_exprents();

            for (std::size_t pos = 0; pos < lst_exprents.size(); ++pos) {
                Exprent* exprent = lst_exprents[pos];
                int action = 0;

                if (exprent->type == Exprent::EXPRENT_ASSIGNMENT) {
                    auto* asexpr = static_cast<AssignmentExprent*>(exprent);
                    if (asexpr->get_left()->type == Exprent::EXPRENT_FIELD && asexpr->get_right()->type == Exprent::EXPRENT_VAR) {
                        auto* fexpr = static_cast<FieldExprent*>(asexpr->get_left());
                        if (fexpr->get_classname() == wrapper->get_class_struct()->qualified_name) {
                            const StructField* struct_field = wrapper->get_class_struct()->get_field(fexpr->get_name(), fexpr->get_descriptor().descriptor_string);
                            if (struct_field != nullptr && struct_field->has_modifier(cc::ACC_FINAL)) action = 0;
                        }
                    }
                } else if (index > 0 && exprent->type == Exprent::EXPRENT_INVOCATION &&
                           is_invocation_init_constructor(static_cast<InvocationExprent*>(exprent), meth, wrapper, true)) {
                    // this() or super()
                    Exprent* removed = lst_exprents[static_cast<std::size_t>(index)];
                    lst_exprents.erase(lst_exprents.begin() + index);
                    lst_exprents.insert(lst_exprents.begin(), removed);
                    action = 1;
                }

                if (action != 0) break;

                ++index;
            }
        }
    }
}

void InitializerProcessor::hide_empty_super(ClassWrapper* wrapper) {
    for (MethodWrapper* meth : wrapper->get_methods()) {
        if (cc::INIT_NAME == meth->method_struct->name() && meth->root != nullptr) {
            Statement* firstdata = find_first_data(meth->root);
            if (firstdata == nullptr || firstdata->get_exprents()->empty()) return;

            Exprent* exprent = (*firstdata->get_exprents())[0];
            if (exprent->type == Exprent::EXPRENT_INVOCATION) {
                auto* invexpr = static_cast<InvocationExprent*>(exprent);
                if (is_invocation_init_constructor(invexpr, meth, wrapper, false) && invexpr->get_lst_parameters().empty()) {
                    firstdata->get_exprents()->erase(firstdata->get_exprents()->begin());
                }
            }
        }
    }
}

void InitializerProcessor::extract_static_initializers(ClassWrapper* wrapper, MethodWrapper* meth) {
    RootStatement* root = meth->root;
    const StructClass* cl = wrapper->get_class_struct();

    Statement* firstdata = find_first_data(root);
    if (firstdata != nullptr) {
        while (!firstdata->get_exprents()->empty()) {
            Exprent* exprent = (*firstdata->get_exprents())[0];

            bool found = false;

            if (exprent->type == Exprent::EXPRENT_ASSIGNMENT) {
                auto* asexpr = static_cast<AssignmentExprent*>(exprent);
                if (asexpr->get_left()->type == Exprent::EXPRENT_FIELD) {
                    auto* fexpr = static_cast<FieldExprent*>(asexpr->get_left());
                    if (fexpr->is_static() && fexpr->get_classname() == cl->qualified_name && cl->has_field(fexpr->get_name(), fexpr->get_descriptor().descriptor_string)) {
                        if (is_exprent_independent(asexpr->get_right(), meth)) {
                            const std::string key_field = InterpreterUtil::make_unique_key(fexpr->get_name(), fexpr->get_descriptor().descriptor_string);
                            if (!wrapper->get_static_field_initializers().contains_key(key_field)) {
                                wrapper->get_static_field_initializers().add_with_key(asexpr->get_right(), key_field);
                                firstdata->get_exprents()->erase(firstdata->get_exprents()->begin());
                                found = true;
                            }
                        }
                    }
                }
            }

            if (!found) break;
        }
    }
}

void InitializerProcessor::extract_dynamic_initializers(ClassWrapper* wrapper) {
    const StructClass* cl = wrapper->get_class_struct();

    const bool is_anonymous = DecompilerContext::current().class_processor->get_class_node(cl->qualified_name)->type == ClassNode::CLASS_ANONYMOUS;

    std::vector<std::vector<Exprent*>*> lst_first;
    std::vector<MethodWrapper*> lst_meth_wrappers;

    for (MethodWrapper* meth : wrapper->get_methods()) {
        if (cc::INIT_NAME == meth->method_struct->name() && meth->root != nullptr) {   // successfully decompiled constructor
            Statement* firstdata = find_first_data(meth->root);
            if (firstdata == nullptr || firstdata->get_exprents()->empty()) return;

            lst_first.push_back(&*firstdata->get_exprents());
            lst_meth_wrappers.push_back(meth);

            Exprent* exprent = (*firstdata->get_exprents())[0];
            if (!is_anonymous) {   // FIXME (the original's): doesn't make sense
                if (exprent->type != Exprent::EXPRENT_INVOCATION || !is_invocation_init_constructor(static_cast<InvocationExprent*>(exprent), meth, wrapper, false)) {
                    return;
                }
            }
        }
    }

    if (lst_first.empty()) return;

    while (true) {
        std::optional<std::string> field_with_descr;
        Exprent* value = nullptr;

        for (std::size_t i = 0; i < lst_first.size(); ++i) {
            std::vector<Exprent*>& lst = *lst_first[i];

            if (lst.size() < (is_anonymous ? 1u : 2u)) return;

            Exprent* exprent = lst[is_anonymous ? 0 : 1];

            bool found = false;

            if (exprent->type == Exprent::EXPRENT_ASSIGNMENT) {
                auto* asexpr = static_cast<AssignmentExprent*>(exprent);
                if (asexpr->get_left()->type == Exprent::EXPRENT_FIELD) {
                    auto* fexpr = static_cast<FieldExprent*>(asexpr->get_left());
                    if (!fexpr->is_static() && fexpr->get_classname() == cl->qualified_name &&
                        cl->has_field(fexpr->get_name(), fexpr->get_descriptor().descriptor_string)) {   // check for the physical existence of the field. Could be defined in a superclass.
                        if (is_exprent_independent(asexpr->get_right(), lst_meth_wrappers[i])) {
                            const std::string field_key = InterpreterUtil::make_unique_key(fexpr->get_name(), fexpr->get_descriptor().descriptor_string);
                            if (!field_with_descr) {
                                field_with_descr = field_key;
                                value = asexpr->get_right();
                            } else {
                                if (*field_with_descr != field_key || !value->equals(asexpr->get_right())) return;
                            }
                            found = true;
                        }
                    }
                }
            }

            if (!found) return;
        }

        if (!wrapper->get_dynamic_field_initializers().contains_key(*field_with_descr)) {
            wrapper->get_dynamic_field_initializers().add_with_key(value, *field_with_descr);
            for (std::vector<Exprent*>* lst : lst_first) lst->erase(lst->begin() + (is_anonymous ? 0 : 1));
        } else {
            return;
        }
    }
}

bool InitializerProcessor::is_exprent_independent(Exprent* exprent, MethodWrapper* meth) {
    std::vector<Exprent*> lst = exprent->get_all_exprents(true);
    lst.push_back(exprent);

    for (Exprent* expr : lst) {
        switch (expr->type) {
            case Exprent::EXPRENT_VAR: {
                const VarVersionPair varpaar(static_cast<VarExprent*>(expr));
                if (!meth->varproc->get_external_vars().contains(varpaar)) {
                    const std::string varname = meth->varproc->get_var_name(varpaar).value();
                    if (varname != "this" && !(varname.size() >= 5 && varname.compare(varname.size() - 5, 5, ".this") == 0)) {   // FIXME (the original's): remove direct comparison with strings
                        return false;
                    }
                }
                break;
            }
            default:
                break;
        }
    }

    return true;
}

Statement* InitializerProcessor::find_first_data(Statement* stat) {
    if (stat->get_exprents().has_value()) return stat;

    if (stat->is_labeled()) {   // FIXME (the original's): Why??
        return nullptr;
    }

    switch (stat->type) {
        case Statement::TYPE_SEQUENCE:
        case Statement::TYPE_IF:
        case Statement::TYPE_ROOT:
        case Statement::TYPE_SWITCH:
        case Statement::TYPE_SYNCRONIZED:
            return find_first_data(stat->get_first());
        default:
            return nullptr;
    }
}

bool InitializerProcessor::is_invocation_init_constructor(InvocationExprent* inv, MethodWrapper* meth, ClassWrapper* wrapper, bool with_this) {
    if (inv->get_functype() == InvocationExprent::TYP_INIT) {
        if (inv->get_instance()->type == Exprent::EXPRENT_VAR) {
            auto* instvar = static_cast<VarExprent*>(inv->get_instance());
            const VarVersionPair varpaar(instvar);

            const std::string* classname = meth->varproc->get_this_vars().get(varpaar);

            if (classname != nullptr) {   // any this instance. TODO (the original's): Restrict to current class?
                if (with_this || wrapper->get_class_struct()->qualified_name != inv->get_classname()) return true;
            }
        }
    }

    return false;
}

}  // namespace cb::java::decompiler
