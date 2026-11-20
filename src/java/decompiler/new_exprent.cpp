// java/decompiler/new_exprent.cpp -- see exprents.hpp: NewExprent. Ported from Fernflower (de.fernflower.modules.decompiler.exps.NewExprent),
// Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0.
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <stdexcept>

#include "java/decompiler/class_writer.hpp"
#include "java/decompiler/classes.hpp"
#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/collectors.hpp"
#include "java/decompiler/expr_processor.hpp"
#include "java/decompiler/exprents.hpp"
#include "java/decompiler/vars.hpp"

namespace cb::java::decompiler {

namespace {

ClassNode* root_class(const std::optional<std::string>& name) {
    if (!name) return nullptr;
    return DecompilerContext::current().class_processor->get_class_node(*name);
}

}  // namespace

NewExprent::NewExprent(VarType new_type, ListStack<Exprent*>& stack, int array_dim, const Offsets& offsets)
    : NewExprent(std::move(new_type), get_dimensions(array_dim, stack), offsets) {}

NewExprent::NewExprent(VarType new_type, std::vector<Exprent*> lst_dims, const Offsets& offsets)
    : Exprent(EXPRENT_NEW), new_type_(std::move(new_type)), lst_dims_(std::move(lst_dims)) {
    anonymous_ = false;
    lambda_ = false;

    if (new_type_.type == cc::TYPE_OBJECT && new_type_.array_dim == 0) {
        ClassNode* node = root_class(new_type_.value);
        if (node != nullptr && (node->type == ClassNode::CLASS_ANONYMOUS || node->type == ClassNode::CLASS_LAMBDA)) {
            anonymous_ = true;
            if (node->type == ClassNode::CLASS_LAMBDA) lambda_ = true;
        }
    }

    add_bytecode_offsets(offsets);
}

std::vector<Exprent*> NewExprent::get_dimensions(int array_dim, ListStack<Exprent*>& stack) {
    std::vector<Exprent*> lst_dims;
    for (int i = 0; i < array_dim; i++) lst_dims.insert(lst_dims.begin(), stack.pop());
    return lst_dims;
}

VarType NewExprent::get_expr_type() const {
    if (anonymous_) return root_class(new_type_.value)->anonymous_class_type.value();
    return new_type_;
}

CheckTypesResult NewExprent::check_expr_type_bounds() {
    CheckTypesResult result;

    if (new_type_.array_dim != 0) {
        for (Exprent* dim : lst_dims_) {
            result.add_min_type_exprent(dim, VarType::BYTECHAR);
            result.add_max_type_exprent(dim, VarType::INT);
        }

        if (new_type_.array_dim == 1) {
            const VarType left_type = new_type_.decrease_array_dim();
            for (Exprent* element : lst_array_elements_) {
                result.add_min_type_exprent(element, VarType::min_type_in_family(left_type.type_family));
                result.add_max_type_exprent(element, left_type);
            }
        }
    } else {
        if (constructor_ != nullptr) return constructor_->check_expr_type_bounds();
    }

    return result;
}

std::vector<Exprent*> NewExprent::get_all_exprents() {
    std::vector<Exprent*> lst;

    if (new_type_.array_dim == 0) {
        if (constructor_ != nullptr) {
            Exprent* constructor_instance = constructor_->get_instance();
            if (constructor_instance != nullptr) {   // should be true only for a lambda expression with a virtual content method
                lst.push_back(constructor_instance);
            }

            lst.insert(lst.end(), constructor_->get_lst_parameters().begin(), constructor_->get_lst_parameters().end());
        }
    } else {
        lst.insert(lst.end(), lst_dims_.begin(), lst_dims_.end());
        lst.insert(lst.end(), lst_array_elements_.begin(), lst_array_elements_.end());
    }

    return lst;
}

Exprent* NewExprent::copy() const {
    std::vector<Exprent*> lst;
    for (const Exprent* expr : lst_dims_) lst.push_back(expr->copy());

    NewExprent* ret = make<NewExprent>(new_type_, lst, bytecode);
    ret->set_constructor(constructor_ == nullptr ? nullptr : static_cast<InvocationExprent*>(constructor_->copy()));
    ret->set_lst_array_elements(lst_array_elements_);
    ret->set_direct_array_init(direct_array_init_);
    ret->set_anonymous(anonymous_);
    ret->set_enum_const(enum_const_);
    return ret;
}

TextBuffer NewExprent::to_java(int indent, BytecodeMappingTracer& tracer) {
    TextBuffer buf;

    if (anonymous_) {
        ClassNode* child = root_class(new_type_.value);

        buf.append("(");

        if (!lambda_ && constructor_ != nullptr) {
            InvocationExprent* invsuper = child->super_invocation;

            ClassNode* newnode = DecompilerContext::current().class_processor->get_class_node(invsuper->get_classname());

            SignatureFields sig_fields;
            if (newnode != nullptr) {   // own class
                if (newnode->get_wrapper() != nullptr) {
                    sig_fields = newnode->get_wrapper()->get_method_wrapper(std::string(cc::INIT_NAME), invsuper->get_string_descriptor())->signature_fields;
                } else {
                    if (newnode->type == ClassNode::CLASS_MEMBER && (newnode->access & cc::ACC_STATIC) == 0 &&
                        !constructor_->get_lst_parameters().empty()) {   // member non-static class invoked with enclosing class instance
                        sig_fields = std::vector<std::optional<VarVersionPair>>(constructor_->get_lst_parameters().size());
                        sig_fields->at(0) = VarVersionPair(-1, 0);
                    }
                }
            }

            bool firstpar = true;
            int start = 0, end = static_cast<int>(invsuper->get_lst_parameters().size());
            if (enum_const_) {
                start += 2;
                end -= 1;
            }

            for (int i = start; i < end; i++) {
                if (!sig_fields || !sig_fields->at(static_cast<std::size_t>(i))) {
                    if (!firstpar) buf.append(", ");

                    Exprent* param = invsuper->get_lst_parameters().at(static_cast<std::size_t>(i));
                    if (param->type == Exprent::EXPRENT_VAR) {
                        const int varindex = static_cast<VarExprent*>(param)->get_index();
                        if (varindex > 0 && varindex <= static_cast<int>(constructor_->get_lst_parameters().size())) {
                            param = constructor_->get_lst_parameters()[static_cast<std::size_t>(varindex - 1)];
                        }
                    }

                    TextBuffer buff;
                    ExprProcessor::get_casted_exprent(param, invsuper->get_descriptor().params.at(static_cast<std::size_t>(i)), buff, indent, true, tracer);

                    buf.append(buff);

                    firstpar = false;
                }
            }
        }

        if (!enum_const_) {
            std::optional<std::string> enclosing;
            if (!lambda_ && constructor_ != nullptr) {
                enclosing = get_qualified_new_instance(child->anonymous_class_type->value.value(), constructor_->get_lst_parameters(), indent, tracer);
            }

            std::string type_name = ExprProcessor::get_cast_type_name(child->anonymous_class_type.value());
            if (enclosing) {
                ClassNode* anonimous_node = root_class(child->anonymous_class_type->value);
                if (anonimous_node != nullptr) {
                    type_name = anonimous_node->simple_name.value();
                } else {
                    type_name = type_name.substr(type_name.rfind('.') + 1);   // (npos + 1 is 0)
                }
            }

            buf.prepend("new " + type_name);

            if (enclosing) buf.prepend(*enclosing + ".");
        }

        buf.append(")");

        if (enum_const_ && buf.length() == 2) buf.set_length(0);

        if (lambda_) {
            if (!DecompilerContext::option(pref::LAMBDA_TO_ANONYMOUS_CLASS)) {
                buf.set_length(0);   // remove the usual 'new <class>()', it will be replaced with lambda style '() ->'
            }

            Exprent* method_object = constructor_ == nullptr ? nullptr : constructor_->get_instance();
            TextBuffer cls_buf;
            ClassWriter().class_lambda_to_java(child, cls_buf, method_object, indent, tracer);
            buf.append(cls_buf);
            tracer.increment_current_source_line(cls_buf.count_lines());
        } else {
            TextBuffer cls_buf;
            ClassWriter().class_to_java(child, cls_buf, indent, &tracer);
            buf.append(cls_buf);
            tracer.increment_current_source_line(cls_buf.count_lines());
        }
    } else if (direct_array_init_) {
        const VarType left_type = new_type_.decrease_array_dim();

        buf.append("{");
        for (std::size_t i = 0; i < lst_array_elements_.size(); i++) {
            if (i > 0) buf.append(", ");
            ExprProcessor::get_casted_exprent(lst_array_elements_[i], left_type, buf, indent, false, tracer);
        }
        buf.append("}");
    } else {
        if (new_type_.array_dim == 0) {
            if (constructor_ != nullptr) {
                const std::vector<Exprent*>& lst_parameters = constructor_->get_lst_parameters();

                ClassNode* newnode = DecompilerContext::current().class_processor->get_class_node(constructor_->get_classname());

                SignatureFields sig_fields;
                if (newnode != nullptr) {   // own class
                    if (newnode->get_wrapper() != nullptr) {
                        sig_fields = newnode->get_wrapper()->get_method_wrapper(std::string(cc::INIT_NAME), constructor_->get_string_descriptor())->signature_fields;
                    } else {
                        if (newnode->type == ClassNode::CLASS_MEMBER && (newnode->access & cc::ACC_STATIC) == 0 &&
                            !constructor_->get_lst_parameters().empty()) {   // member non-static class invoked with enclosing class instance
                            sig_fields = std::vector<std::optional<VarVersionPair>>(lst_parameters.size());
                            sig_fields->at(0) = VarVersionPair(-1, 0);
                        }
                    }
                }

                const int start = enum_const_ ? 2 : 0;
                if (!enum_const_ || start < static_cast<int>(lst_parameters.size())) {
                    buf.append("(");

                    bool first_param = true;
                    for (std::size_t i = static_cast<std::size_t>(start); i < lst_parameters.size(); i++) {
                        if (!sig_fields || !sig_fields->at(i)) {
                            Exprent* expr = lst_parameters[i];
                            const VarType left_type = constructor_->get_descriptor().params.at(i);

                            if (i == lst_parameters.size() - 1 && expr->get_expr_type().is_constant(VarType::NULL_TYPE)) {
                                ClassNode* node = root_class(left_type.value);
                                if (node != nullptr && node->nameless_constructor_stub) {
                                    break;   // skip last parameter of synthetic constructor call
                                }
                            }

                            if (!first_param) buf.append(", ");

                            TextBuffer buff;
                            ExprProcessor::get_casted_exprent(expr, left_type, buff, indent, true, tracer);

                            buf.append(buff);

                            first_param = false;
                        }
                    }

                    buf.append(")");
                }
            }

            if (!enum_const_) {
                std::optional<std::string> enclosing;
                if (constructor_ != nullptr) {
                    enclosing = get_qualified_new_instance(new_type_.value.value(), constructor_->get_lst_parameters(), indent, tracer);
                }

                std::string type_name = ExprProcessor::get_type_name(new_type_);
                if (enclosing) {
                    ClassNode* new_node = root_class(new_type_.value);
                    if (new_node != nullptr) {
                        type_name = new_node->simple_name.value();
                    } else {
                        type_name = type_name.substr(type_name.rfind('.') + 1);
                    }
                }

                buf.prepend("new " + type_name);

                if (enclosing) buf.prepend(*enclosing + ".");
            }
        } else {
            buf.append("new ").append(ExprProcessor::get_type_name(new_type_));

            if (lst_array_elements_.empty()) {
                for (int i = 0; i < new_type_.array_dim; i++) {
                    buf.append("[");
                    if (i < static_cast<int>(lst_dims_.size())) buf.append(lst_dims_[static_cast<std::size_t>(i)]->to_java(indent, tracer));
                    buf.append("]");
                }
            } else {
                for (int i = 0; i < new_type_.array_dim; i++) buf.append("[]");

                const VarType left_type = new_type_.decrease_array_dim();

                buf.append("{");
                for (std::size_t i = 0; i < lst_array_elements_.size(); i++) {
                    if (i > 0) buf.append(", ");

                    TextBuffer buff;
                    ExprProcessor::get_casted_exprent(lst_array_elements_[i], left_type, buff, indent, false, tracer);
                    buf.append(buff);
                }
                buf.append("}");
            }
        }
    }

    return buf;
}

std::optional<std::string> NewExprent::get_qualified_new_instance(const std::string& classname, const std::vector<Exprent*>& lst_params, int indent,
                                                                  BytecodeMappingTracer& tracer) {
    ClassNode* node = DecompilerContext::current().class_processor->get_class_node(classname);

    if (node != nullptr && node->type != ClassNode::CLASS_ROOT && node->type != ClassNode::CLASS_LOCAL && (node->access & cc::ACC_STATIC) == 0) {
        if (!lst_params.empty()) {
            Exprent* enclosing = lst_params[0];

            bool is_qualified_new = false;
            if (enclosing->type == Exprent::EXPRENT_VAR) {
                const VarExprent* var_enclosing = static_cast<const VarExprent*>(enclosing);

                const StructClass* current_class = DecompilerContext::current().current_class_node->class_struct;
                const std::string* this_classname = var_enclosing->get_processor()->get_this_vars().get(VarVersionPair(var_enclosing));

                if (this_classname == nullptr || current_class->qualified_name != *this_classname) is_qualified_new = true;
            } else {
                is_qualified_new = true;
            }

            if (is_qualified_new) return enclosing->to_java(indent, tracer).to_string();
        }
    }

    return std::nullopt;
}

void NewExprent::replace_exprent(Exprent* old_expr, Exprent* new_expr) {
    if (old_expr == constructor_) constructor_ = static_cast<InvocationExprent*>(new_expr);

    if (constructor_ != nullptr) constructor_->replace_exprent(old_expr, new_expr);

    for (std::size_t i = 0; i < lst_dims_.size(); i++) {
        if (old_expr == lst_dims_[i]) lst_dims_[i] = new_expr;
    }

    for (std::size_t i = 0; i < lst_array_elements_.size(); i++) {
        if (old_expr == lst_array_elements_[i]) lst_array_elements_[i] = new_expr;
    }
}

bool NewExprent::equals(const Exprent* o) const {
    if (o == this) return true;
    const NewExprent* ne = dynamic_cast<const NewExprent*>(o);
    if (ne == nullptr) return false;
    return new_type_ == ne->get_new_type() && equal_exprent_lists(lst_dims_, ne->lst_dims_) && equal_exprents(constructor_, ne->get_constructor()) &&
           direct_array_init_ == ne->direct_array_init_ && equal_exprent_lists(lst_array_elements_, ne->lst_array_elements_);
}

}  // namespace cb::java::decompiler
