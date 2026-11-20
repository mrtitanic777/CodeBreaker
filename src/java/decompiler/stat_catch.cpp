// stat_catch.cpp -- CatchStatement, CatchAllStatement. Ported from Fernflower (de.fernflower.modules.decompiler.stats), Copyright 2000-2015 JetBrains
// s.r.o., Apache License 2.0 (see NOTICE in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <stdexcept>

#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/collectors.hpp"
#include "java/decompiler/dom_helper.hpp"
#include "java/decompiler/expr_processor.hpp"
#include "java/decompiler/statements.hpp"

namespace cb::java::decompiler {

namespace {

VarExprent* new_catch_var(const std::string& class_name) {
    return make<VarExprent>(DecompilerContext::counter_container().get_counter_and_increment(CounterContainer::VAR_COUNTER),
                            VarType(cc::TYPE_OBJECT, 0, class_name), DecompilerContext::current().current_var_processor);
}

}  // namespace

// ---- CatchStatement

CatchStatement::CatchStatement(Statement* head, Statement* next, const LinkedSet<Statement*>& set_handlers) : CatchStatement() {
    first_ = head;
    stats_.add_with_key(first_, first_->id);

    for (StatEdge* edge : head->get_successor_edges(StatEdge::TYPE_EXCEPTION)) {
        Statement* stat = edge->get_destination();

        if (set_handlers.contains(stat)) {
            stats_.add_with_key(stat, stat->id);
            exctstrings_.push_back(edge->get_exceptions().value());
            vars_.push_back(new_catch_var(edge->get_exceptions().value().at(0)));
        }
    }

    if (next != nullptr) post_ = next;
}

Statement* CatchStatement::is_head(Statement* head) {
    if (head->get_last_basic_type() != LASTBASICTYPE_GENERAL) return nullptr;

    LinkedSet<Statement*> set_handlers = DecHelper::get_unique_pred_exceptions(head);

    if (!set_handlers.empty()) {
        int hnextcount = 0;   // either no statements with connection to next, or more than 1

        Statement* next = nullptr;

        const std::vector<StatEdge*> lst_head_succs = head->get_successor_edges(STATEDGE_DIRECT_ALL);
        if (!lst_head_succs.empty() && lst_head_succs[0]->get_type() == StatEdge::TYPE_REGULAR) {
            next = lst_head_succs[0]->get_destination();
            hnextcount = 2;
        }

        for (StatEdge* edge : head->get_successor_edges(StatEdge::TYPE_EXCEPTION)) {
            Statement* stat = edge->get_destination();

            bool handlerok = true;

            if (edge->get_exceptions() && set_handlers.contains(stat)) {
                if (stat->get_last_basic_type() != LASTBASICTYPE_GENERAL) {
                    handlerok = false;
                } else {
                    const std::vector<StatEdge*> lst_stat_succs = stat->get_successor_edges(STATEDGE_DIRECT_ALL);
                    if (!lst_stat_succs.empty() && lst_stat_succs[0]->get_type() == StatEdge::TYPE_REGULAR) {
                        Statement* statn = lst_stat_succs[0]->get_destination();

                        if (next == nullptr) {
                            next = statn;
                        } else if (next != statn) {
                            handlerok = false;
                        }

                        if (handlerok) hnextcount++;
                    }
                }
            } else {
                handlerok = false;
            }

            if (!handlerok) set_handlers.remove(stat);
        }

        if (hnextcount != 1 && !set_handlers.empty()) {
            std::vector<Statement*> lst;
            lst.push_back(head);
            for (Statement* st : set_handlers) lst.push_back(st);

            for (Statement* st : lst) {
                if (st->is_monitor_enter()) return nullptr;
            }

            if (DecHelper::check_statement_exceptions(lst)) return make<CatchStatement>(head, next, set_handlers);
        }
    }

    return nullptr;
}

TextBuffer CatchStatement::to_java(int indent, BytecodeMappingTracer& tracer) {
    TextBuffer buf;

    buf.append(ExprProcessor::list_to_java(var_definitions_, indent, tracer));

    if (is_labeled()) {
        buf.append_indent(indent).append("label").append(std::to_string(id)).append(":").append_line_separator();
        tracer.increment_current_source_line();
    }

    buf.append_indent(indent).append("try {").append_line_separator();
    tracer.increment_current_source_line();

    buf.append(ExprProcessor::jmp_wrapper(first_, indent + 1, false, tracer));

    buf.append_indent(indent).append("}");

    for (std::size_t i = 1; i < stats_.size(); i++) {
        const std::vector<std::string>& exception_types = exctstrings_.at(i - 1);

        buf.append(" catch (");

        if (exception_types.size() > 1) {   // multi-catch, Java 7 style
            for (std::size_t exc_index = 1; exc_index < exception_types.size(); ++exc_index) {
                const VarType exc_type(cc::TYPE_OBJECT, 0, exception_types[exc_index]);
                const std::string exc_type_name = ExprProcessor::get_cast_type_name(exc_type);

                buf.append(exc_type_name).append(" | ");
            }
        }

        buf.append(vars_.at(i - 1)->to_java(indent, tracer));
        buf.append(") {").append_line_separator();
        tracer.increment_current_source_line();

        buf.append(ExprProcessor::jmp_wrapper(stats_[i], indent + 1, false, tracer)).append_indent(indent).append("}");
    }

    buf.append_line_separator();
    tracer.increment_current_source_line();

    return buf;
}

Statement* CatchStatement::get_simple_copy() {
    CatchStatement* cs = make<CatchStatement>();

    for (const std::vector<std::string>& exc : this->exctstrings_) {
        cs->exctstrings_.push_back(exc);
        cs->vars_.push_back(new_catch_var(exc.at(0)));
    }

    return cs;
}

// ---- CatchAllStatement

CatchAllStatement::CatchAllStatement(Statement* head, Statement* handler) : CatchAllStatement() {
    first_ = head;
    stats_.add_with_key(head, head->id);

    handler_ = handler;
    stats_.add_with_key(handler, handler->id);

    const std::vector<StatEdge*> lst_succs = head->get_successor_edges(STATEDGE_DIRECT_ALL);
    if (!lst_succs.empty()) {
        StatEdge* edge = lst_succs[0];
        if (edge->get_type() == StatEdge::TYPE_REGULAR) post_ = edge->get_destination();
    }

    vars_.push_back(new_catch_var("java/lang/Throwable"));
}

Statement* CatchAllStatement::is_head(Statement* head) {
    if (head->get_last_basic_type() != LASTBASICTYPE_GENERAL) return nullptr;

    LinkedSet<Statement*> set_handlers = DecHelper::get_unique_pred_exceptions(head);
    if (set_handlers.size() != 1) return nullptr;

    for (StatEdge* edge : head->get_successor_edges(StatEdge::TYPE_EXCEPTION)) {
        Statement* exc = edge->get_destination();

        if (!edge->get_exceptions() && set_handlers.contains(exc) && exc->get_last_basic_type() == LASTBASICTYPE_GENERAL) {
            const std::vector<StatEdge*> lst_succs = exc->get_successor_edges(STATEDGE_DIRECT_ALL);
            if (lst_succs.empty() || lst_succs[0]->get_type() != StatEdge::TYPE_REGULAR) {
                if (head->is_monitor_enter() || exc->is_monitor_enter()) return nullptr;

                if (DecHelper::check_statement_exceptions({head, exc})) return make<CatchAllStatement>(head, exc);
            }
        }
    }

    return nullptr;
}

TextBuffer CatchAllStatement::to_java(int indent, BytecodeMappingTracer& tracer) {
    const std::string new_line_separator = DecompilerContext::new_line_separator();

    TextBuffer buf;

    buf.append(ExprProcessor::list_to_java(var_definitions_, indent, tracer));

    const bool labeled = is_labeled();
    if (labeled) {
        buf.append_indent(indent).append("label").append(std::to_string(id)).append(":").append_line_separator();
        tracer.increment_current_source_line();
    }

    const std::vector<StatEdge*> lst_succs = first_->get_successor_edges(STATEDGE_DIRECT_ALL);

    if (first_->type == TYPE_TRYCATCH && first_->get_var_definitions().empty() && is_finally_ && !labeled && !first_->is_labeled() &&
        (lst_succs.empty() || !lst_succs[0]->explicit_)) {
        TextBuffer content = ExprProcessor::jmp_wrapper(first_, indent, false, tracer);
        content.set_length(content.length() - new_line_separator.length());
        tracer.increment_current_source_line(-1);
        buf.append(content);
    } else {
        buf.append_indent(indent).append("try {").append_line_separator();
        tracer.increment_current_source_line();

        buf.append(ExprProcessor::jmp_wrapper(first_, indent + 1, false, tracer));

        buf.append_indent(indent).append("}");
    }

    buf.append(is_finally_ ? std::string(" finally") : " catch (" + vars_.at(0)->to_java(indent, tracer).to_string() + ")").append(" {").append_line_separator();
    tracer.increment_current_source_line();

    if (monitor_ != nullptr) {
        buf.append_indent(indent + 1).append("if(").append(monitor_->to_java(indent, tracer)).append(") {").append_line_separator();
        tracer.increment_current_source_line();
    }

    buf.append(ExprProcessor::jmp_wrapper(handler_, indent + 1 + (monitor_ != nullptr ? 1 : 0), false, tracer));

    if (monitor_ != nullptr) {
        buf.append_indent(indent + 1).append("}").append_line_separator();
        tracer.increment_current_source_line();
    }

    buf.append_indent(indent).append("}").append_line_separator();
    tracer.increment_current_source_line();

    return buf;
}

void CatchAllStatement::replace_statement(Statement* oldstat, Statement* newstat) {
    if (handler_ == oldstat) handler_ = newstat;

    Statement::replace_statement(oldstat, newstat);
}

Statement* CatchAllStatement::get_simple_copy() {
    CatchAllStatement* cas = make<CatchAllStatement>();

    cas->is_finally_ = this->is_finally_;

    if (this->monitor_ != nullptr) {
        cas->monitor_ = make<VarExprent>(DecompilerContext::counter_container().get_counter_and_increment(CounterContainer::VAR_COUNTER), VarType::INT,
                                         DecompilerContext::current().current_var_processor);
    }

    if (!this->vars_.empty()) {   // (the original adds it to this statement's list, not the copy's)
        vars_.push_back(new_catch_var("java/lang/Throwable"));
    }

    return cas;
}

void CatchAllStatement::init_simple_copy() {
    first_ = stats_.get(0);
    handler_ = stats_.get(1);
}

}  // namespace cb::java::decompiler
