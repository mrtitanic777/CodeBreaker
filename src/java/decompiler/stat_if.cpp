// stat_if.cpp -- IfStatement. Ported from Fernflower (de.fernflower.modules.decompiler.stats.IfStatement), Copyright 2000-2015 JetBrains s.r.o.,
// Apache License 2.0 (see NOTICE in this folder).
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <stdexcept>

#include "java/decompiler/collectors.hpp"
#include "java/decompiler/dom_helper.hpp"
#include "java/decompiler/expr_processor.hpp"
#include "java/decompiler/statements.hpp"
#include "java/decompiler/text_buffer.hpp"

namespace cb::java::decompiler {

IfStatement::IfStatement() {
    type = TYPE_IF;
}

IfStatement::IfStatement(Statement* head, int regedges, Statement* postst) : IfStatement() {
    first_ = head;
    stats_.add_with_key(head, head->id);

    const std::vector<StatEdge*> lst_head_succs = head->get_successor_edges(STATEDGE_DIRECT_ALL);

    switch (regedges) {
        case 0:
            ifstat_ = nullptr;
            elsestat_ = nullptr;

            break;
        case 1: {
            ifstat_ = nullptr;
            elsestat_ = nullptr;

            StatEdge* edgeif = lst_head_succs.at(1);
            if (edgeif->get_type() != StatEdge::TYPE_REGULAR) {
                post_ = lst_head_succs.at(0)->get_destination();
            } else {
                post_ = edgeif->get_destination();
                negated_ = true;
            }
            break;
        }
        case 2: {
            elsestat_ = lst_head_succs.at(0)->get_destination();
            ifstat_ = lst_head_succs.at(1)->get_destination();

            const std::vector<StatEdge*> lst_succ = ifstat_->get_successor_edges(StatEdge::TYPE_REGULAR);
            const std::vector<StatEdge*> lst_succ1 = elsestat_->get_successor_edges(StatEdge::TYPE_REGULAR);

            if (ifstat_->get_predecessor_edges(StatEdge::TYPE_REGULAR).size() > 1 || lst_succ.size() > 1) {
                post_ = ifstat_;
            } else if (elsestat_->get_predecessor_edges(StatEdge::TYPE_REGULAR).size() > 1 || lst_succ1.size() > 1) {
                post_ = elsestat_;
            } else {
                if (lst_succ.size() == 0) {
                    post_ = elsestat_;
                } else if (lst_succ1.size() == 0) {
                    post_ = ifstat_;
                }
            }

            if (ifstat_ == post_) {
                if (elsestat_ != post_) {
                    ifstat_ = elsestat_;
                    negated_ = true;
                } else {
                    ifstat_ = nullptr;
                }
                elsestat_ = nullptr;
            } else if (elsestat_ == post_) {
                elsestat_ = nullptr;
            } else {
                post_ = postst;
            }

            if (elsestat_ == nullptr) {
                regedges = 1;   // if without else
            }
            break;
        }
        default:
            break;
    }

    ifedge_ = lst_head_succs.at(negated_ ? 0 : 1);
    elseedge_ = (regedges == 2) ? lst_head_succs.at(negated_ ? 1 : 0) : nullptr;

    iftype = (regedges == 2) ? IFTYPE_IFELSE : IFTYPE_IF;

    // regular edges
    if (iftype == IFTYPE_IF) {
        if (regedges == 0) {
            StatEdge* edge = lst_head_succs.at(0);
            head->remove_successor(edge);
            edge->set_source(this);
            this->add_successor(edge);
        } else if (regedges == 1) {
            StatEdge* edge = lst_head_succs.at(negated_ ? 1 : 0);
            head->remove_successor(edge);
        }
    }

    if (ifstat_ != nullptr) stats_.add_with_key(ifstat_, ifstat_->id);

    if (elsestat_ != nullptr) stats_.add_with_key(elsestat_, elsestat_->id);

    if (post_ == head) post_ = this;
}

Statement* IfStatement::is_head(Statement* head) {
    if (head->type == TYPE_BASICBLOCK && head->get_last_basic_type() == LASTBASICTYPE_IF) {
        const int regsize = static_cast<int>(head->get_successor_edges(StatEdge::TYPE_REGULAR).size());

        Statement* p = nullptr;

        bool ok = (regsize < 2);
        if (!ok) {
            std::vector<Statement*> lst;
            if (DecHelper::is_choice_statement(head, lst)) {
                p = lst.front();
                lst.erase(lst.begin());

                for (Statement* st : lst) {
                    if (st->is_monitor_enter()) return nullptr;
                }

                ok = DecHelper::check_statement_exceptions(lst);
            }
        }

        if (ok) return make<IfStatement>(head, regsize, p);
    }

    return nullptr;
}

TextBuffer IfStatement::to_java(int indent, BytecodeMappingTracer& tracer) {
    const std::string indstr = indent_string(indent);
    TextBuffer buf;

    buf.append(ExprProcessor::list_to_java(var_definitions_, indent, tracer));

    buf.append(first_->to_java(indent, tracer));

    if (is_labeled()) {
        buf.append_indent(indent).append("label").append(std::to_string(id)).append(":").append_line_separator();
        tracer.increment_current_source_line();
    }

    buf.append_indent(indent).append(headexprent_[0]->to_java(indent, tracer)).append(" {").append_line_separator();
    tracer.increment_current_source_line();

    if (ifstat_ == nullptr) {
        if (ifedge_->explicit_) {
            buf.append_indent(indent + 1);

            if (ifedge_->get_type() == StatEdge::TYPE_BREAK) {
                buf.append("break");
            } else {
                buf.append("continue");
            }

            if (ifedge_->labeled) {
                buf.append(" label").append(std::to_string(ifedge_->closure->id));
            }
            buf.append(";").append_line_separator();
            tracer.increment_current_source_line();
        }
    } else {
        buf.append(ExprProcessor::jmp_wrapper(ifstat_, indent + 1, false, tracer));
    }

    bool elseif = false;

    if (elsestat_ != nullptr) {
        if (elsestat_->type == Statement::TYPE_IF && elsestat_->get_var_definitions().empty() && elsestat_->get_first()->get_exprents().value().empty() &&
            !elsestat_->is_labeled() &&
            (elsestat_->get_successor_edges(STATEDGE_DIRECT_ALL).empty() || !elsestat_->get_successor_edges(STATEDGE_DIRECT_ALL).at(0)->explicit_)) {   // else if

            TextBuffer content = ExprProcessor::jmp_wrapper(elsestat_, indent, false, tracer);
            content.set_start(indstr.length());

            buf.append_indent(indent).append("} else ");

            buf.append(content);

            elseif = true;
        } else {
            BytecodeMappingTracer else_tracer(tracer.get_current_source_line() + 1);
            TextBuffer content = ExprProcessor::jmp_wrapper(elsestat_, indent + 1, false, else_tracer);

            if (content.length() > 0) {
                buf.append_indent(indent).append("} else {").append_line_separator();

                tracer.set_current_source_line(else_tracer.get_current_source_line());
                tracer.add_tracer(&else_tracer);

                buf.append(content);
            }
        }
    }

    if (!elseif) {
        buf.append_indent(indent).append("}").append_line_separator();
        tracer.increment_current_source_line();
    }

    return buf;
}

void IfStatement::init_exprents() {
    std::vector<Exprent*>& lst = first_->get_exprents().value();
    IfExprent* ifexpr = static_cast<IfExprent*>(lst.back());
    lst.pop_back();

    if (negated_) {
        ifexpr = static_cast<IfExprent*>(ifexpr->copy());
        ifexpr->negate_if();
    }

    headexprent_[0] = ifexpr;
}

std::vector<SeqObject> IfStatement::get_sequential_objects() {
    std::vector<SeqObject> lst = Statement::get_sequential_objects();

    lst.insert(lst.begin() + 1, SeqObject{nullptr, headexprent_[0]});

    return lst;
}

void IfStatement::replace_exprent(Exprent* oldexpr, Exprent* newexpr) {
    if (headexprent_[0] == oldexpr) headexprent_[0] = newexpr;
}

void IfStatement::replace_statement(Statement* oldstat, Statement* newstat) {
    Statement::replace_statement(oldstat, newstat);

    if (ifstat_ == oldstat) ifstat_ = newstat;

    if (elsestat_ == oldstat) elsestat_ = newstat;

    const std::vector<StatEdge*> lst_succs = first_->get_successor_edges(STATEDGE_DIRECT_ALL);

    if (iftype == IFTYPE_IF) {
        ifedge_ = lst_succs.at(0);
        elseedge_ = nullptr;
    } else {
        StatEdge* edge0 = lst_succs.at(0);
        StatEdge* edge1 = lst_succs.at(1);

        if (edge0->get_destination() == ifstat_) {
            ifedge_ = edge0;
            elseedge_ = edge1;
        } else {
            ifedge_ = edge1;
            elseedge_ = edge0;
        }
    }
}

Statement* IfStatement::get_simple_copy() {
    IfStatement* is = make<IfStatement>();

    is->iftype = this->iftype;
    is->negated_ = this->negated_;
    is->iffflag_ = this->iffflag_;

    return is;
}

void IfStatement::init_simple_copy() {
    first_ = stats_.get(0);

    const std::vector<StatEdge*> lst_succs = first_->get_successor_edges(STATEDGE_DIRECT_ALL);
    ifedge_ = lst_succs.at((iftype == IFTYPE_IF || negated_) ? 0 : 1);

    if (stats_.size() > 1) ifstat_ = stats_.get(1);

    if (iftype == IFTYPE_IFELSE) {
        elseedge_ = lst_succs.at(negated_ ? 1 : 0);
        elsestat_ = stats_.get(2);
    }
}

IMatchable* IfStatement::find_object(const MatchNode* match_node, int index) {
    IMatchable* object = Statement::find_object(match_node, index);
    if (object != nullptr) return object;

    if (match_node->get_type() == MatchNode::MATCHNODE_EXPRENT) {
        const MatchValue value = match_node->get_rule_value(MatchProperties::EXPRENT_POSITION);
        const std::string* position = std::get_if<std::string>(&value);
        if (position != nullptr && *position == "head") return get_headexprent();
    }

    return nullptr;
}

bool IfStatement::match(const MatchNode* match_node, MatchEngine& engine) {
    if (!Statement::match(match_node, engine)) return false;

    const MatchValue type_value = match_node->get_rule_value(MatchProperties::STATEMENT_IFTYPE);
    if (const auto* t = std::get_if<std::int32_t>(&type_value)) {
        if (this->iftype != *t) return false;
    }

    return true;
}

}  // namespace cb::java::decompiler
