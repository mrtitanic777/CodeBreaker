// java/decompiler/enum_processor.cpp -- see classes.hpp. Ported from Fernflower (de.fernflower.main.EnumProcessor), Copyright 2000-2015 JetBrains s.r.o.,
// Apache License 2.0 (see NOTICE in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <stdexcept>
#include <string>

#include "java/decompiler/classes.hpp"
#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/exprents.hpp"
#include "java/decompiler/statements.hpp"
#include "java/decompiler/vars.hpp"

namespace cb::java::decompiler {

void EnumProcessor::clear_enum(ClassWrapper* wrapper) {
    const StructClass* cl = wrapper->get_class_struct();

    // hide values/valueOf methods and super() invocations
    for (MethodWrapper* method : wrapper->get_methods()) {
        const StructMethod* mt = method->method_struct;

        const std::string& name = mt->name();
        const std::string& descriptor = mt->descriptor();

        if (name == "values") {
            if (descriptor == "()[L" + cl->qualified_name + ";") {
                wrapper->get_hidden_members().insert(InterpreterUtil::make_unique_key(name, descriptor));
            }
        } else if (name == "valueOf") {
            if (descriptor == "(Ljava/lang/String;)L" + cl->qualified_name + ";") {
                wrapper->get_hidden_members().insert(InterpreterUtil::make_unique_key(name, descriptor));
            }
        } else if (cc::INIT_NAME == name) {
            Statement* first_data = find_first_data(method->root);
            if (first_data != nullptr && !first_data->get_exprents()->empty()) {
                Exprent* exprent = (*first_data->get_exprents())[0];
                if (exprent->type == Exprent::EXPRENT_INVOCATION) {
                    auto* invexpr = static_cast<InvocationExprent*>(exprent);
                    if (is_invocation_super_constructor(invexpr, method, wrapper)) {
                        first_data->get_exprents()->erase(first_data->get_exprents()->begin());
                    }
                }
            }
        }
    }

    // hide synthetic fields of enum and it's constants
    for (const auto& fd : cl->fields()) {
        const std::string& descriptor = fd->descriptor();
        if (fd->is_synthetic() && descriptor == "[L" + cl->qualified_name + ";") {
            wrapper->get_hidden_members().insert(InterpreterUtil::make_unique_key(fd->name(), descriptor));
        }
    }
}

// FIXME (the original's): move to a util class (see also InitializerProcessor)
Statement* EnumProcessor::find_first_data(Statement* stat) {
    if (stat == nullptr) throw std::runtime_error("NullPointerException");   // (a constructor that failed to decompile: the original throws here too)
    if (stat->get_exprents().has_value()) return stat;

    if (stat->is_labeled()) return nullptr;

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

// FIXME (the original's): move to util class (see also InitializerProcessor)
bool EnumProcessor::is_invocation_super_constructor(InvocationExprent* inv, MethodWrapper* meth, ClassWrapper* wrapper) {
    if (inv->get_functype() == InvocationExprent::TYP_INIT) {
        if (inv->get_instance()->type == Exprent::EXPRENT_VAR) {
            auto* instvar = static_cast<VarExprent*>(inv->get_instance());
            const VarVersionPair varpaar(instvar);

            const std::string* classname = meth->varproc->get_this_vars().get(varpaar);

            if (classname != nullptr) {   // any this instance. TODO (the original's): Restrict to current class?
                if (wrapper->get_class_struct()->qualified_name != inv->get_classname()) return true;
            }
        }
    }

    return false;
}

}  // namespace cb::java::decompiler
