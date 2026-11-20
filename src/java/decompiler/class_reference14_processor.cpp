// java/decompiler/class_reference14_processor.cpp -- see classes.hpp. Ported from Fernflower (de.fernflower.main.ClassReference14Processor), Copyright
// 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <algorithm>
#include <string>
#include <vector>

#include "java/decompiler/classes.hpp"
#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/exprents.hpp"
#include "java/decompiler/sforms.hpp"
#include "java/decompiler/statements.hpp"

namespace cb::java::decompiler {

ClassReference14Processor::ClassReference14Processor() {
    InvocationExprent* invfor = make<InvocationExprent>();
    invfor->set_name("forName");
    invfor->set_classname("java/lang/Class");
    invfor->set_string_descriptor("(Ljava/lang/String;)Ljava/lang/Class;");
    invfor->set_descriptor(MethodDescriptor::parse("(Ljava/lang/String;)Ljava/lang/Class;"));
    invfor->set_static(true);
    invfor->set_lst_parameters({make<VarExprent>(0, VarType::STRING, nullptr)});

    bodyexprent = make<ExitExprent>(ExitExprent::EXIT_RETURN, invfor, VarType::CLASS, std::nullopt);

    InvocationExprent* constr = make<InvocationExprent>();
    constr->set_name(std::string(cc::INIT_NAME));
    constr->set_classname("java/lang/NoClassDefFoundError");
    constr->set_string_descriptor("()V");
    constr->set_functype(InvocationExprent::TYP_INIT);
    constr->set_descriptor(MethodDescriptor::parse("()V"));

    NewExprent* newexpr = make<NewExprent>(VarType(cc::TYPE_OBJECT, 0, std::string("java/lang/NoClassDefFoundError")), std::vector<Exprent*>(), std::nullopt);
    newexpr->set_constructor(constr);

    InvocationExprent* invcause = make<InvocationExprent>();
    invcause->set_name("initCause");
    invcause->set_classname("java/lang/NoClassDefFoundError");
    invcause->set_string_descriptor("(Ljava/lang/Throwable;)Ljava/lang/Throwable;");
    invcause->set_descriptor(MethodDescriptor::parse("(Ljava/lang/Throwable;)Ljava/lang/Throwable;"));
    invcause->set_instance(newexpr);
    invcause->set_lst_parameters({make<VarExprent>(2, VarType(cc::TYPE_OBJECT, 0, std::string("java/lang/ClassNotFoundException")), nullptr)});

    handlerexprent = make<ExitExprent>(ExitExprent::EXIT_THROW, invcause, VarType(), std::nullopt);   // (a null return type: a default one)
}

void ClassReference14Processor::process_class_references(ClassNode* node) {
    ClassWrapper* wrapper = node->get_wrapper();

    if (wrapper->get_class_struct()->is_version_ge_1_5()) {
        // version 1.5 or above
        return;
    }

    // find the synthetic method Class class$(String) if present
    LinkedMap<ClassWrapper*, MethodWrapper*> map_class_meths;
    map_class_methods(node, map_class_meths);
    if (map_class_meths.empty()) return;

    LinkedSet<ClassWrapper*> set_found;

    process_class_rec(node, map_class_meths, set_found);

    if (!set_found.empty()) {
        for (ClassWrapper* wrp : set_found) {
            const StructMethod* mt = (*map_class_meths.get(wrp))->method_struct;
            wrp->get_hidden_members().insert(InterpreterUtil::make_unique_key(mt->name(), mt->descriptor()));
        }
    }
}

void ClassReference14Processor::process_class_rec(ClassNode* node, LinkedMap<ClassWrapper*, MethodWrapper*>& map_class_meths, LinkedSet<ClassWrapper*>& set_found) {
    ClassWrapper* wrapper = node->get_wrapper();

    // search code
    for (MethodWrapper* meth : wrapper->get_methods()) {
        RootStatement* root = meth->root;

        if (root != nullptr) {
            DirectGraph* graph = meth->get_or_build_graph();

            graph->iterate_exprents([&](Exprent* exprent) {
                for (const auto& ent : map_class_meths) {
                    if (replace_invocations(exprent, ent.first, ent.second)) set_found.add(ent.first);
                }

                return 0;
            });
        }
    }

    // search initializers
    for (int j = 0; j < 2; ++j) {
        KeyedList<Exprent*, std::string>& initializers = j == 0 ? wrapper->get_static_field_initializers() : wrapper->get_dynamic_field_initializers();

        for (std::size_t i = 0; i < initializers.size(); ++i) {
            for (const auto& ent : map_class_meths) {
                Exprent* exprent = initializers.get(i);

                if (replace_invocations(exprent, ent.first, ent.second)) set_found.add(ent.first);

                const std::optional<std::string> cl = is_class14_invocation(exprent, ent.first, ent.second);
                if (cl) {
                    std::string slashed = *cl;
                    std::replace(slashed.begin(), slashed.end(), '.', '/');
                    initializers[i] = make<ConstExprent>(VarType::CLASS, ConstExprent::Value(slashed), exprent->bytecode);
                    set_found.add(ent.first);
                }
            }
        }
    }

    // iterate nested classes
    for (ClassNode* nd : node->nested) process_class_rec(nd, map_class_meths, set_found);
}

void ClassReference14Processor::map_class_methods(ClassNode* node, LinkedMap<ClassWrapper*, MethodWrapper*>& map) {
    const bool no_synth_flag = DecompilerContext::option(pref::SYNTHETIC_NOT_SET);

    ClassWrapper* wrapper = node->get_wrapper();

    for (MethodWrapper* method : wrapper->get_methods()) {
        const StructMethod* mt = method->method_struct;

        if ((no_synth_flag || mt->is_synthetic()) && mt->descriptor() == "(Ljava/lang/String;)Ljava/lang/Class;" && mt->has_modifier(cc::ACC_STATIC)) {
            RootStatement* root = method->root;

            if (root != nullptr && root->get_first()->type == Statement::TYPE_TRYCATCH) {
                auto* cst = static_cast<CatchStatement*>(root->get_first());

                if (cst->get_stats().size() == 2 && cst->get_first()->type == Statement::TYPE_BASICBLOCK &&
                    cst->get_stats().get(1)->type == Statement::TYPE_BASICBLOCK &&
                    cst->get_vars()[0]->get_var_type() == VarType(cc::TYPE_OBJECT, 0, std::string("java/lang/ClassNotFoundException"))) {
                    auto* body = static_cast<BasicBlockStatement*>(cst->get_first());
                    auto* handler = static_cast<BasicBlockStatement*>(cst->get_stats().get(1));

                    if (body->get_exprents()->size() == 1 && handler->get_exprents()->size() == 1) {
                        if (bodyexprent->equals((*body->get_exprents())[0]) && handlerexprent->equals((*handler->get_exprents())[0])) {
                            map.put(wrapper, method);
                            break;
                        }
                    }
                }
            }
        }
    }

    // iterate nested classes
    for (ClassNode* nd : node->nested) map_class_methods(nd, map);
}

bool ClassReference14Processor::replace_invocations(Exprent* exprent, ClassWrapper* wrapper, MethodWrapper* meth) {
    bool res = false;

    while (true) {
        bool found = false;

        for (Exprent* expr : exprent->get_all_exprents()) {
            const std::optional<std::string> cl = is_class14_invocation(expr, wrapper, meth);
            if (cl) {
                std::string slashed = *cl;
                std::replace(slashed.begin(), slashed.end(), '.', '/');
                exprent->replace_exprent(expr, make<ConstExprent>(VarType::CLASS, ConstExprent::Value(slashed), expr->bytecode));
                found = true;
                res = true;
                break;
            }

            res |= replace_invocations(expr, wrapper, meth);
        }

        if (!found) break;
    }

    return res;
}

std::optional<std::string> ClassReference14Processor::is_class14_invocation(Exprent* exprent, ClassWrapper* wrapper, MethodWrapper* meth) {
    if (exprent->type == Exprent::EXPRENT_FUNCTION) {
        auto* fexpr = static_cast<FunctionExprent*>(exprent);

        if (fexpr->get_func_type() == FunctionExprent::FUNCTION_IIF) {
            if (fexpr->get_lst_operands()[0]->type == Exprent::EXPRENT_FUNCTION) {
                auto* headexpr = static_cast<FunctionExprent*>(fexpr->get_lst_operands()[0]);

                if (headexpr->get_func_type() == FunctionExprent::FUNCTION_EQ) {
                    if (headexpr->get_lst_operands()[0]->type == Exprent::EXPRENT_FIELD && headexpr->get_lst_operands()[1]->type == Exprent::EXPRENT_CONST &&
                        static_cast<ConstExprent*>(headexpr->get_lst_operands()[1])->get_const_type() == VarType::NULL_TYPE) {
                        auto* field = static_cast<FieldExprent*>(headexpr->get_lst_operands()[0]);

                        ClassNode* fieldnode = DecompilerContext::current().class_processor->get_class_node(field->get_classname());

                        if (fieldnode != nullptr && fieldnode->class_struct->qualified_name == wrapper->get_class_struct()->qualified_name) {   // source class
                            const StructField* fd = wrapper->get_class_struct()->get_field(field->get_name(), field->get_descriptor().descriptor_string);   // FIXME (the original's): can be null! why??

                            if (fd != nullptr && fd->has_modifier(cc::ACC_STATIC) && (fd->is_synthetic() || DecompilerContext::option(pref::SYNTHETIC_NOT_SET))) {
                                if (fexpr->get_lst_operands()[1]->type == Exprent::EXPRENT_ASSIGNMENT && fexpr->get_lst_operands()[2]->equals(field)) {
                                    auto* asexpr = static_cast<AssignmentExprent*>(fexpr->get_lst_operands()[1]);

                                    if (asexpr->get_left()->equals(field) && asexpr->get_right()->type == Exprent::EXPRENT_INVOCATION) {
                                        auto* invexpr = static_cast<InvocationExprent*>(asexpr->get_right());

                                        if (invexpr->get_classname() == wrapper->get_class_struct()->qualified_name && invexpr->get_name() == meth->method_struct->name() &&
                                            invexpr->get_string_descriptor() == meth->method_struct->descriptor()) {
                                            if (invexpr->get_lst_parameters()[0]->type == Exprent::EXPRENT_CONST) {
                                                wrapper->get_hidden_members().insert(InterpreterUtil::make_unique_key(fd->name(), fd->descriptor()));   // hide synthetic field

                                                return java_value_string(static_cast<ConstExprent*>(invexpr->get_lst_parameters()[0])->get_value());
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return std::nullopt;
}

}  // namespace cb::java::decompiler
