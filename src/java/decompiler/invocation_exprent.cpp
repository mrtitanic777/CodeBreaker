// java/decompiler/invocation_exprent.cpp -- see exprents.hpp: InvocationExprent. Ported from Fernflower
// (de.fernflower.modules.decompiler.exps.InvocationExprent), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0.
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <stdexcept>

#include "java/decompiler/class_writer.hpp"
#include "java/decompiler/classes.hpp"
#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/collectors.hpp"
#include "java/decompiler/expr_processor.hpp"
#include "java/decompiler/exprents.hpp"
#include "java/decompiler/structs.hpp"
#include "java/decompiler/vars.hpp"

namespace cb::java::decompiler {

InvocationExprent::InvocationExprent(int opcode, const LinkConstant& cn, ListStack<Exprent*>& stack, int dynamic_invocation_type, const Offsets& offsets)
    : Exprent(EXPRENT_INVOCATION) {
    name_ = cn.elementname;
    classname_ = cn.classname;

    switch (opcode) {
        case cc::opc_invokestatic:
            invocation_typ_ = INVOKE_STATIC;
            break;
        case cc::opc_invokespecial:
            invocation_typ_ = INVOKE_SPECIAL;
            break;
        case cc::opc_invokevirtual:
            invocation_typ_ = INVOKE_VIRTUAL;
            break;
        case cc::opc_invokeinterface:
            invocation_typ_ = INVOKE_INTERFACE;
            break;
        case cc::opc_invokedynamic:
            invocation_typ_ = INVOKE_DYNAMIC;
            classname_ = "java/lang/Class";   // dummy class name
            invoke_dynamic_class_suffix_ = "##Lambda_" + std::to_string(cn.index1) + "_" + std::to_string(cn.index2);
            break;
        default:
            break;
    }

    if (cc::INIT_NAME == name_) {
        functype_ = TYP_INIT;
    } else if (cc::CLINIT_NAME == name_) {
        functype_ = TYP_CLINIT;
    }

    string_descriptor_ = cn.descriptor;
    descriptor_ = MethodDescriptor::parse(cn.descriptor);

    for (std::size_t i = 0; i < descriptor_.params.size(); i++) lst_parameters_.insert(lst_parameters_.begin(), stack.pop());

    if (opcode == cc::opc_invokedynamic) {
        if (dynamic_invocation_type == cc::CONSTANT_MethodHandle_REF_invokeStatic) {
            is_static_ = true;
        } else {
            // FIXME: remove the first parameter completely from the list. It's the object type for a virtual lambda method.
            if (!lst_parameters_.empty()) instance_ = lst_parameters_[0];
        }
    } else if (opcode == cc::opc_invokestatic) {
        is_static_ = true;
    } else {
        instance_ = stack.pop();
    }

    add_bytecode_offsets(offsets);
}

InvocationExprent::InvocationExprent(const InvocationExprent& expr) : Exprent(EXPRENT_INVOCATION) {
    name_ = expr.get_name();
    classname_ = expr.get_classname();
    is_static_ = expr.is_static();
    functype_ = expr.get_functype();
    instance_ = expr.get_instance();
    if (instance_ != nullptr) instance_ = instance_->copy();
    invocation_typ_ = expr.get_invocation_typ();
    invoke_dynamic_class_suffix_ = expr.get_invoke_dynamic_class_suffix();
    string_descriptor_ = expr.get_string_descriptor();
    descriptor_ = expr.get_descriptor();
    lst_parameters_ = expr.get_lst_parameters();
    ExprProcessor::copy_entries(lst_parameters_);
    add_bytecode_offsets(expr.bytecode);
}

CheckTypesResult InvocationExprent::check_expr_type_bounds() {
    CheckTypesResult result;
    for (std::size_t i = 0; i < lst_parameters_.size(); i++) {
        Exprent* parameter = lst_parameters_[i];
        const VarType left_type = descriptor_.params.at(i);
        result.add_min_type_exprent(parameter, VarType::min_type_in_family(left_type.type_family));
        result.add_max_type_exprent(parameter, left_type);
    }
    return result;
}

std::vector<Exprent*> InvocationExprent::get_all_exprents() {
    std::vector<Exprent*> lst;
    if (instance_ != nullptr) lst.push_back(instance_);
    lst.insert(lst.end(), lst_parameters_.begin(), lst_parameters_.end());
    return lst;
}

Exprent* InvocationExprent::copy() const { return make<InvocationExprent>(*this); }

TextBuffer InvocationExprent::to_java(int indent, BytecodeMappingTracer& tracer) {
    TextBuffer buf;
    std::optional<std::string> super_qualifier;
    bool is_instance_this = false;

    tracer.add_mapping(bytecode);

    if (is_static_) {
        ClassNode* node = DecompilerContext::current().current_class_node;
        if (node == nullptr || classname_ != node->class_struct->qualified_name) {
            buf.append(DecompilerContext::current().import_collector->get_short_name(ExprProcessor::build_java_class_name(classname_)));
        }
    } else {
        if (instance_ != nullptr && instance_->type == Exprent::EXPRENT_VAR) {
            const VarExprent* instvar = static_cast<const VarExprent*>(instance_);
            const VarVersionPair varpaar(instvar);

            VarProcessor* vproc = instvar->get_processor();
            if (vproc == nullptr) {
                MethodWrapper* current_meth = DecompilerContext::current().current_method_wrapper;
                if (current_meth != nullptr) vproc = current_meth->varproc;
            }

            const std::string* this_classname = nullptr;
            if (vproc != nullptr) this_classname = vproc->get_this_vars().get(varpaar);

            if (this_classname != nullptr) {
                is_instance_this = true;

                if (invocation_typ_ == INVOKE_SPECIAL) {
                    if (classname_ != *this_classname) {   // TODO: direct comparison to the super class?
                        super_qualifier = *this_classname;
                    }
                }
            }
        }

        if (functype_ == TYP_GENERAL) {
            if (super_qualifier) {
                TextUtil::write_qualified_super(buf, *super_qualifier);
            } else if (instance_ != nullptr) {
                TextBuffer res = instance_->to_java(indent, tracer);

                const VarType right_type = instance_->get_expr_type();
                const VarType left_type(cc::TYPE_OBJECT, 0, classname_);

                if (right_type == VarType::OBJECT && !(left_type == right_type)) {
                    buf.append("((").append(ExprProcessor::get_cast_type_name(left_type)).append(")");

                    if (instance_->get_precedence() >= FunctionExprent::get_precedence(FunctionExprent::FUNCTION_CAST)) res.enclose("(", ")");

                    buf.append(res).append(")");
                } else if (instance_->get_precedence() > get_precedence()) {
                    buf.append("(").append(res).append(")");
                } else {
                    buf.append(res);
                }
            }
        }
    }

    switch (functype_) {
        case TYP_GENERAL:
            if (buf.to_string() == VarExprent::VAR_NAMELESS_ENCLOSURE) buf = TextBuffer();

            if (buf.length() > 0) buf.append(".");

            buf.append(name_);
            if (invocation_typ_ == INVOKE_DYNAMIC) buf.append("<invokedynamic>");
            buf.append("(");
            break;
        case TYP_CLINIT:
            throw std::runtime_error("Explicit invocation of " + std::string(cc::CLINIT_NAME));
        case TYP_INIT:
            if (super_qualifier) buf.append("super(");
            else if (is_instance_this) buf.append("this(");
            else throw std::runtime_error("Unrecognized invocation of " + std::string(cc::INIT_NAME));
            break;
        default:
            break;
    }

    SignatureFields sig_fields;
    bool is_enum = false;
    if (functype_ == TYP_INIT) {
        ClassNode* new_node = DecompilerContext::current().class_processor->get_class_node(classname_);
        if (new_node != nullptr) {   // own class
            if (new_node->get_wrapper() != nullptr) {
                sig_fields = new_node->get_wrapper()->get_method_wrapper(std::string(cc::INIT_NAME), string_descriptor_)->signature_fields;
            } else {
                if (new_node->type == ClassNode::CLASS_MEMBER && (new_node->access & cc::ACC_STATIC) == 0) {   // non-static member class
                    sig_fields = std::vector<std::optional<VarVersionPair>>(lst_parameters_.size());
                    sig_fields->at(0) = VarVersionPair(-1, 0);
                }
            }

            is_enum = new_node->class_struct->has_modifier(cc::ACC_ENUM) && DecompilerContext::option(pref::DECOMPILE_ENUM);
        }
    }

    const std::vector<bool> set_ambiguous_parameters = get_ambiguous_parameters();

    bool first_parameter = true;
    const int start = is_enum ? 2 : 0;
    for (std::size_t i = static_cast<std::size_t>(start); i < lst_parameters_.size(); i++) {
        if (!sig_fields) {
            if (!first_parameter) buf.append(", ");

            TextBuffer buff;
            const bool ambiguous = i < set_ambiguous_parameters.size() && set_ambiguous_parameters[i];
            ExprProcessor::get_casted_exprent(lst_parameters_[i], descriptor_.params.at(i), buff, indent, true, ambiguous, tracer);

            buf.append(buff);

            first_parameter = false;
        }
    }

    buf.append(")");

    return buf;
}

std::vector<bool> InvocationExprent::get_ambiguous_parameters() const {
    StructClass* cl = DecompilerContext::current().struct_context->get_class(classname_);
    if (cl == nullptr) return {};

    // check number of matches
    std::vector<MethodDescriptor> matches;

    for (const std::shared_ptr<StructMethod>& mt : cl->methods().items()) {
        if (mt->has_modifier(cc::ACC_BRIDGE) && DecompilerContext::option(pref::REMOVE_BRIDGE)) continue;

        if (name_ == mt->name()) {
            const MethodDescriptor md = MethodDescriptor::parse(mt->descriptor());
            if (md.params.size() == descriptor_.params.size()) {
                bool next_method = false;
                for (std::size_t i = 0; i < md.params.size(); i++) {
                    if (md.params[i].type_family != descriptor_.params[i].type_family) {
                        next_method = true;
                        break;
                    }
                }
                if (next_method) continue;
                matches.push_back(md);
            }
        }
    }

    if (matches.size() == 1) return {};

    // check if a call is unambiguous
    const StructMethod* mt = cl->get_method(InterpreterUtil::make_unique_key(name_, string_descriptor_));
    if (mt != nullptr) {
        const MethodDescriptor md = MethodDescriptor::parse(mt->descriptor());
        if (md.params.size() == lst_parameters_.size()) {
            bool exact = true;
            for (std::size_t i = 0; i < md.params.size(); i++) {
                if (!(md.params[i] == lst_parameters_[i]->get_expr_type())) {
                    exact = false;
                    break;
                }
            }
            if (exact) return {};
        }
    }

    // mark parameters
    std::vector<bool> ambiguous(descriptor_.params.size(), false);
    for (std::size_t i = 0; i < descriptor_.params.size(); i++) {
        const VarType& param_type = descriptor_.params[i];
        for (const MethodDescriptor& md : matches) {
            if (!(param_type == md.params.at(i))) {
                ambiguous[i] = true;
                break;
            }
        }
    }

    return ambiguous;
}

void InvocationExprent::replace_exprent(Exprent* old_expr, Exprent* new_expr) {
    if (old_expr == instance_) instance_ = new_expr;

    for (std::size_t i = 0; i < lst_parameters_.size(); i++) {
        if (old_expr == lst_parameters_[i]) lst_parameters_[i] = new_expr;
    }
}

bool InvocationExprent::equals(const Exprent* o) const {
    if (o == this) return true;
    const InvocationExprent* it = dynamic_cast<const InvocationExprent*>(o);
    if (it == nullptr) return false;
    return name_ == it->get_name() && classname_ == it->get_classname() && is_static_ == it->is_static() && equal_exprents(instance_, it->get_instance()) &&
           descriptor_ == it->get_descriptor() && functype_ == it->get_functype() && equal_exprent_lists(lst_parameters_, it->get_lst_parameters());
}

bool InvocationExprent::match(const MatchNode* match_node, MatchEngine& engine) {
    if (!Exprent::match(match_node, engine)) return false;

    for (const auto& [key, value] : match_node->get_rules()) {
        switch (key) {
            case MatchProperties::EXPRENT_INVOCATION_PARAMETER:
                if (value.is_variable()) {
                    if (value.parameter < static_cast<int>(lst_parameters_.size())) {
                        if (!engine.check_and_set_variable_value(value.to_string(),
                                                                 MatchValue(static_cast<IMatchable*>(lst_parameters_[static_cast<std::size_t>(value.parameter)])))) {
                            return false;
                        }
                    } else {
                        return false;
                    }
                }
                break;
            case MatchProperties::EXPRENT_INVOCATION_CLASS:
                if (!equal_match_values(value.value, MatchValue(classname_))) return false;
                break;
            case MatchProperties::EXPRENT_INVOCATION_SIGNATURE:
                if (!equal_match_values(value.value, MatchValue(name_ + string_descriptor_))) return false;
                break;
            default:
                break;
        }
    }
    return true;
}

}  // namespace cb::java::decompiler
