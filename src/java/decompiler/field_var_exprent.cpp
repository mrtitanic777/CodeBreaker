// java/decompiler/field_var_exprent.cpp -- see exprents.hpp: FieldExprent and VarExprent. Ported from Fernflower
// (de.fernflower.modules.decompiler.exps), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0.
// cb-unity: alone (file-local helpers share names with the other decompiler sources)
#include <stdexcept>

#include "java/decompiler/classes.hpp"
#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/collectors.hpp"
#include "java/decompiler/class_writer.hpp"
#include "java/decompiler/expr_processor.hpp"
#include "java/decompiler/exprents.hpp"
#include "java/decompiler/vars.hpp"

namespace cb::java::decompiler {

// ---- FieldExprent

FieldExprent::FieldExprent(const LinkConstant& cn, Exprent* instance, const Offsets& offsets)
    : FieldExprent(cn.elementname, cn.classname, instance == nullptr, instance, FieldDescriptor::parse(cn.descriptor), offsets) {}

FieldExprent::FieldExprent(std::string name, std::string classname, bool is_static, Exprent* instance, FieldDescriptor descriptor, const Offsets& offsets)
    : Exprent(EXPRENT_FIELD),
      name_(std::move(name)),
      classname_(std::move(classname)),
      is_static_(is_static),
      instance_(instance),
      descriptor_(std::move(descriptor)) {
    add_bytecode_offsets(offsets);
}

int FieldExprent::get_exprent_use() const { return instance_ == nullptr ? Exprent::MULTIPLE_USES : instance_->get_exprent_use() & Exprent::MULTIPLE_USES; }

std::vector<Exprent*> FieldExprent::get_all_exprents() {
    std::vector<Exprent*> lst;
    if (instance_ != nullptr) lst.push_back(instance_);
    return lst;
}

Exprent* FieldExprent::copy() const {
    return make<FieldExprent>(name_, classname_, is_static_, instance_ == nullptr ? nullptr : instance_->copy(), descriptor_, bytecode);
}

TextBuffer FieldExprent::to_java(int indent, BytecodeMappingTracer& tracer) {
    TextBuffer buf;

    if (is_static_) {
        ClassNode* node = DecompilerContext::current().current_class_node;
        if (node == nullptr || classname_ != node->class_struct->qualified_name) {
            buf.append(DecompilerContext::current().import_collector->get_short_name(ExprProcessor::build_java_class_name(classname_)));
            buf.append(".");
        }
    } else {
        std::optional<std::string> super_qualifier;

        if (instance_ != nullptr && instance_->type == Exprent::EXPRENT_VAR) {
            const VarExprent* inst_var = static_cast<const VarExprent*>(instance_);
            const VarVersionPair pair(inst_var);

            MethodWrapper* current_method = DecompilerContext::current().current_method_wrapper;

            if (current_method != nullptr) {   // FIXME: remove
                const std::string* this_classname = current_method->varproc->get_this_vars().get(pair);

                if (this_classname != nullptr) {
                    if (classname_ != *this_classname) {   // TODO: direct comparison to the super class?
                        super_qualifier = *this_classname;
                    }
                }
            }
        }

        if (super_qualifier) {
            TextUtil::write_qualified_super(buf, *super_qualifier);
        } else {
            TextBuffer buff;
            const bool casted = ExprProcessor::get_casted_exprent(instance_, VarType(cc::TYPE_OBJECT, 0, classname_), buff, indent, true, tracer);
            std::string res = buff.to_string();

            if (casted || instance_->get_precedence() > get_precedence()) res = "(" + res + ")";

            buf.append(res);
        }

        if (buf.to_string() == VarExprent::VAR_NAMELESS_ENCLOSURE) {   // FIXME: workaround for field access of an anonymous enclosing class. Find a better way.
            buf.set_length(0);
        } else {
            buf.append(".");
        }
    }

    buf.append(name_);

    tracer.add_mapping(bytecode);

    return buf;
}

void FieldExprent::replace_exprent(Exprent* old_expr, Exprent* new_expr) {
    if (old_expr == instance_) instance_ = new_expr;
}

bool FieldExprent::equals(const Exprent* o) const {
    if (o == this) return true;
    const FieldExprent* ft = dynamic_cast<const FieldExprent*>(o);
    if (ft == nullptr) return false;
    return name_ == ft->get_name() && classname_ == ft->get_classname() && is_static_ == ft->is_static() && equal_exprents(instance_, ft->get_instance()) &&
           descriptor_ == ft->get_descriptor();
}

bool FieldExprent::match(const MatchNode* match_node, MatchEngine& engine) {
    if (!Exprent::match(match_node, engine)) return false;

    const MatchNode::RuleValue* it = match_node->find_rule(MatchProperties::EXPRENT_FIELD_NAME);
    if (it != nullptr) {
        const MatchNode::RuleValue& rule = *it;
        if (rule.is_variable()) {
            if (!engine.check_and_set_variable_value(std::get<std::string>(rule.value), MatchValue(name_))) return false;
        } else {
            if (!equal_match_values(rule.value, MatchValue(name_))) return false;
        }
    }
    return true;
}

// ---- VarExprent

VarExprent::VarExprent(int index, std::optional<VarType> var_type, VarProcessor* processor)
    : Exprent(EXPRENT_VAR), index_(index), var_type_(std::move(var_type)), processor_(processor) {}

Exprent* VarExprent::copy() const {
    VarExprent* var = make<VarExprent>(index_, get_var_type(), processor_);
    var->set_definition(definition_);
    var->set_version(version_);
    var->set_class_def(class_def_);
    var->set_stack(stack_);
    return var;
}

TextBuffer VarExprent::to_java(int indent, BytecodeMappingTracer& tracer) {
    TextBuffer buffer;

    tracer.add_mapping(bytecode);

    if (class_def_) {
        ClassNode* child = DecompilerContext::current().class_processor->get_class_node(var_type_->value.value());
        ClassWriter().class_to_java(child, buffer, indent, &tracer);
        tracer.increment_current_source_line(buffer.count_lines());
    } else {
        std::optional<std::string> name;
        if (processor_ != nullptr) name = processor_->get_var_name(VarVersionPair(index_, version_));

        if (definition_) {
            if (processor_ != nullptr && processor_->get_var_final(VarVersionPair(index_, version_)) == VarTypeProcessor::VAR_EXPLICIT_FINAL) {
                buffer.append("final ");
            }
            buffer.append(ExprProcessor::get_cast_type_name(get_var_type())).append(" ");
        }

        buffer.append(!name ? ("var" + std::to_string(index_) + (version_ == 0 ? "" : "_" + std::to_string(version_))) : *name);
    }

    return buffer;
}

bool VarExprent::equals(const Exprent* o) const {
    if (o == this) return true;
    const VarExprent* ve = dynamic_cast<const VarExprent*>(o);
    if (ve == nullptr) return false;
    return index_ == ve->get_index() && version_ == ve->get_version() && get_var_type() == ve->get_var_type();   // FIXME: varType comparison redundant?
}

VarType VarExprent::get_var_type() const {
    std::optional<VarType> vt;
    if (processor_ != nullptr) vt = processor_->get_var_type(VarVersionPair(index_, version_));

    if (!vt || (var_type_ && var_type_->type != cc::TYPE_UNKNOWN)) vt = var_type_;

    return vt ? *vt : VarType::UNKNOWN;
}

bool VarExprent::match(const MatchNode* match_node, MatchEngine& engine) {
    if (!Exprent::match(match_node, engine)) return false;

    const MatchNode::RuleValue* it = match_node->find_rule(MatchProperties::EXPRENT_VAR_INDEX);
    if (it != nullptr) {
        const MatchNode::RuleValue& rule = *it;
        if (rule.is_variable()) {
            if (!engine.check_and_set_variable_value(std::get<std::string>(rule.value), MatchValue(static_cast<std::int32_t>(index_)))) return false;
        } else {
            if (index_ != std::stoi(std::get<std::string>(rule.value))) return false;
        }
    }
    return true;
}

}  // namespace cb::java::decompiler
