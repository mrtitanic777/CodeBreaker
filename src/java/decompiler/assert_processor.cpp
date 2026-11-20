// java/decompiler/assert_processor.cpp -- see classes.hpp. Ported from Fernflower (de.fernflower.main.AssertProcessor), Copyright 2000-2015 JetBrains
// s.r.o., Apache License 2.0 (see NOTICE in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <string>
#include <variant>
#include <vector>

#include "java/decompiler/cfg.hpp"
#include "java/decompiler/classes.hpp"
#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/exprents.hpp"
#include "java/decompiler/helpers.hpp"
#include "java/decompiler/statements.hpp"

namespace cb::java::decompiler {

namespace {

// A basic block of a statement made here (the statement points at it; blocks of the graph belong to it, this one to the run).
struct OwnedBlock : ArenaObject {
    BasicBlock block;
};

}  // namespace

const VarType& AssertProcessor::class_assertion_error() {
    static const VarType t(cc::TYPE_OBJECT, 0, std::string("java/lang/AssertionError"));
    return t;
}

void AssertProcessor::build_assertions(ClassNode* node) {
    ClassWrapper* wrapper = node->get_wrapper();

    const StructField* field = find_assertion_field(node);
    if (field != nullptr) {
        const std::string key = InterpreterUtil::make_unique_key(field->name(), field->descriptor());

        bool res = false;

        for (MethodWrapper* meth : wrapper->get_methods()) {
            RootStatement* root = meth->root;

            if (root != nullptr) {
                res |= replace_assertions(root, wrapper->get_class_struct()->qualified_name, key);
            }
        }

        if (res) {
            // hide the helper field
            wrapper->get_hidden_members().insert(key);
        }
    }
}

const StructField* AssertProcessor::find_assertion_field(ClassNode* node) {
    ClassWrapper* wrapper = node->get_wrapper();

    const bool no_synth_flag = DecompilerContext::option(pref::SYNTHETIC_NOT_SET);

    for (const auto& fd : wrapper->get_class_struct()->fields()) {
        const std::string key_field = InterpreterUtil::make_unique_key(fd->name(), fd->descriptor());

        // initializer exists
        if (wrapper->get_static_field_initializers().contains_key(key_field)) {
            // access flags set
            if (fd->has_modifier(cc::ACC_STATIC) && fd->has_modifier(cc::ACC_FINAL) && (no_synth_flag || fd->is_synthetic())) {
                // field type boolean
                const FieldDescriptor fdescr = FieldDescriptor::parse(fd->descriptor());

                if (VarType::BOOLEAN == fdescr.type) {
                    Exprent* initializer = wrapper->get_static_field_initializers().get_with_key(key_field);

                    if (initializer->type == Exprent::EXPRENT_FUNCTION) {
                        auto* fexpr = static_cast<FunctionExprent*>(initializer);

                        if (fexpr->get_func_type() == FunctionExprent::FUNCTION_BOOL_NOT && fexpr->get_lst_operands()[0]->type == Exprent::EXPRENT_INVOCATION) {
                            auto* invexpr = static_cast<InvocationExprent*>(fexpr->get_lst_operands()[0]);

                            if (invexpr->get_instance() != nullptr && invexpr->get_instance()->type == Exprent::EXPRENT_CONST &&
                                "desiredAssertionStatus" == invexpr->get_name() && "java/lang/Class" == invexpr->get_classname() &&
                                invexpr->get_lst_parameters().empty()) {
                                auto* cexpr = static_cast<ConstExprent*>(invexpr->get_instance());

                                if (VarType::CLASS == cexpr->get_const_type()) {
                                    ClassNode* nd = node;
                                    while (nd != nullptr) {
                                        const auto* value = std::get_if<std::string>(&cexpr->get_value());
                                        if (value != nullptr && nd->get_wrapper()->get_class_struct()->qualified_name == *value) break;
                                        nd = nd->parent;
                                    }

                                    if (nd != nullptr) {   // found enclosing class with the same name
                                        return fd.get();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return nullptr;
}

bool AssertProcessor::replace_assertions(Statement* statement, const std::string& classname, const std::string& key) {
    bool res = false;

    for (std::size_t i = 0; i < statement->get_stats().size(); ++i) {
        res |= replace_assertions(statement->get_stats().get(i), classname, key);
    }

    bool replaced = true;
    while (replaced) {
        replaced = false;

        for (std::size_t i = 0; i < statement->get_stats().size(); ++i) {
            Statement* st = statement->get_stats().get(i);
            if (st->type == Statement::TYPE_IF) {
                if (replace_assertion(statement, static_cast<IfStatement*>(st), classname, key)) {
                    replaced = true;
                    break;
                }
            }
        }

        res |= replaced;
    }

    return res;
}

bool AssertProcessor::replace_assertion(Statement* parent, IfStatement* stat, const std::string& classname, const std::string& key) {
    Statement* ifstat = stat->get_ifstat();
    InvocationExprent* throw_error = is_assertion_error(ifstat);

    if (throw_error == nullptr) return false;

    const AssertionExprent exprres = get_assertion_exprent(stat->get_headexprent()->get_condition()->copy(), classname, key);
    if (!exprres.found) return false;

    std::vector<Exprent*> lst_params;

    Exprent* ascond = nullptr;
    Exprent* retcond = nullptr;
    if (exprres.exprent != nullptr) {
        ascond = make<FunctionExprent>(FunctionExprent::FUNCTION_BOOL_NOT, exprres.exprent, throw_error->bytecode);
        retcond = SecondaryFunctionsHelper::propagate_bool_not(ascond);
    }

    lst_params.push_back(retcond == nullptr ? ascond : retcond);
    if (!throw_error->get_lst_parameters().empty()) {
        lst_params.push_back(throw_error->get_lst_parameters()[0]);
    }

    AssertExprent* asexpr = make<AssertExprent>(lst_params);

    OwnedBlock* owned = make<OwnedBlock>();
    owned->block.id = DecompilerContext::counter_container().get_counter_and_increment(CounterContainer::STATEMENT_COUNTER);
    Statement* newstat = make<BasicBlockStatement>(&owned->block);
    newstat->set_exprents(std::vector<Exprent*>{asexpr});

    Statement* first = stat->get_first();

    if (stat->iftype == IfStatement::IFTYPE_IFELSE || (first->get_exprents().has_value() && !first->get_exprents()->empty())) {
        first->remove_successor(stat->get_if_edge());
        first->remove_successor(stat->get_else_edge());

        std::vector<Statement*> lst_statements;
        if (first->get_exprents().has_value() && !first->get_exprents()->empty()) lst_statements.push_back(first);

        lst_statements.push_back(newstat);

        if (stat->iftype == IfStatement::IFTYPE_IFELSE) lst_statements.push_back(stat->get_elsestat());

        SequenceStatement* sequence = make<SequenceStatement>(lst_statements);
        sequence->set_all_parent();

        for (std::size_t i = 0; i + 1 < sequence->get_stats().size(); ++i) {
            sequence->get_stats().get(i)->add_successor(
                make<StatEdge>(StatEdge::TYPE_REGULAR, sequence->get_stats().get(i), sequence->get_stats().get(i + 1)));
        }

        if (stat->iftype == IfStatement::IFTYPE_IFELSE) {
            Statement* ifelse = stat->get_elsestat();

            const std::vector<StatEdge*> lst_succs = ifelse->get_all_successor_edges();
            if (!lst_succs.empty()) {
                StatEdge* endedge = lst_succs[0];

                if (endedge->closure == stat) sequence->add_labeled_edge(endedge);
            }
        }

        newstat = sequence;
    }

    for (Exprent* def : stat->get_var_definitions()) newstat->get_var_definitions().push_back(def);

    parent->replace_statement(stat, newstat);

    return true;
}

InvocationExprent* AssertProcessor::is_assertion_error(Statement* stat) {
    if (stat == nullptr || !stat->get_exprents().has_value() || stat->get_exprents()->size() != 1) return nullptr;

    Exprent* expr = (*stat->get_exprents())[0];

    if (expr->type == Exprent::EXPRENT_EXIT) {
        auto* exexpr = static_cast<ExitExprent*>(expr);

        if (exexpr->get_exit_type() == ExitExprent::EXIT_THROW && exexpr->get_value()->type == Exprent::EXPRENT_NEW) {
            auto* nexpr = static_cast<NewExprent*>(exexpr->get_value());

            if (class_assertion_error() == nexpr->get_new_type() && nexpr->get_constructor() != nullptr) {
                return nexpr->get_constructor();
            }
        }
    }

    return nullptr;
}

AssertProcessor::AssertionExprent AssertProcessor::get_assertion_exprent(Exprent* exprent, const std::string& classname, const std::string& key) {
    if (exprent->type == Exprent::EXPRENT_FUNCTION) {
        auto* fexpr = static_cast<FunctionExprent*>(exprent);

        if (fexpr->get_func_type() == FunctionExprent::FUNCTION_CADD) {
            for (std::size_t i = 0; i < 2; ++i) {
                Exprent* param = fexpr->get_lst_operands()[i];

                if (is_assertion_field(param, classname, key)) {
                    return {fexpr->get_lst_operands()[1 - i], true};
                }
            }

            for (std::size_t i = 0; i < 2; ++i) {
                Exprent* param = fexpr->get_lst_operands()[i];

                const AssertionExprent res = get_assertion_exprent(param, classname, key);
                if (res.found) {
                    if (param != res.exprent) fexpr->get_lst_operands()[i] = res.exprent;

                    return {fexpr, true};
                }
            }
        } else if (is_assertion_field(fexpr, classname, key)) {
            // assert false;
            return {nullptr, true};
        }
    }

    return {exprent, false};
}

bool AssertProcessor::is_assertion_field(Exprent* exprent, const std::string& classname, const std::string& key) {
    if (exprent->type == Exprent::EXPRENT_FUNCTION) {
        auto* fparam = static_cast<FunctionExprent*>(exprent);

        if (fparam->get_func_type() == FunctionExprent::FUNCTION_BOOL_NOT && fparam->get_lst_operands()[0]->type == Exprent::EXPRENT_FIELD) {
            auto* fdparam = static_cast<FieldExprent*>(fparam->get_lst_operands()[0]);

            if (classname == fdparam->get_classname() && key == InterpreterUtil::make_unique_key(fdparam->get_name(), fdparam->get_descriptor().descriptor_string)) {
                return true;
            }
        }
    }

    return false;
}

}  // namespace cb::java::decompiler
