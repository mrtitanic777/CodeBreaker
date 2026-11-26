// dotnet/decompiler/il/memory_instructions.cpp -- see memory_instructions.hpp.
#include "dotnet/decompiler/il/memory_instructions.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

#include "dotnet/decompiler/il/il_type_extensions.hpp"
#include "dotnet/decompiler/il/instruction_output_il.hpp"
#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/il/patterns.hpp"
#include "dotnet/decompiler/il/semantic_helper.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"

namespace cb::dotnet::decompiler::il {

namespace {

[[noreturn]] void index_out_of_range() { throw std::out_of_range("child index"); }

// ExpressionBuilder.IsUnboxAnyWithIsInst upstream: `unbox.any T(isinst T(expr))` is C#'s `expr as T` for generic and nullable types.
bool is_unbox_any_with_is_inst(const UnboxAny& unbox_any, const IType* is_inst_type) {
    return typesystem::type_equals(unbox_any.type(), is_inst_type) &&
           (typesystem::is_known_type(unbox_any.type(), typesystem::KnownTypeCode::NullableOfT) ||
            is_inst_type->is_reference_type() == true);
}

// `<opcode> <type>(<argument>)`, the text of the instructions with a type operand and one argument.
void write_type_and_argument(const ILInstruction& inst, const IType* type, ILInstruction* argument, ITextOutput& output,
                             const ILAstWritingOptions& options) {
    inst.write_il_range(output, options);
    write(output, inst.op_code);
    output.write(' ');
    il::write_to(*type, output);
    output.write('(');
    argument->write_to(output, options);
    output.write(')');
}

void write_prefixes(ITextOutput& output, bool is_volatile, std::uint8_t unaligned_prefix) {
    if (is_volatile) output.write("volatile.");
    if (unaligned_prefix > 0) output.write("unaligned(" + std::to_string(static_cast<int>(unaligned_prefix)) + ").");
}

std::vector<ILInstruction*> clone_all(const InstructionCollection<ILInstruction>& list) {
    std::vector<ILInstruction*> result;
    for (ILInstruction* arg : list) result.push_back(arg->clone());
    return result;
}

InstructionFlags indices_flags(const InstructionCollection<ILInstruction>& list) {
    InstructionFlags f = InstructionFlags::None;
    for (ILInstruction* arg : list) f = f | arg->flags();
    return f;
}

void write_indices(ITextOutput& output, const ILAstWritingOptions& options, const InstructionCollection<ILInstruction>& list) {
    for (ILInstruction* index : list) {
        output.write(", ");
        index->write_to(output, options);
    }
}

}  // namespace

// ---- Arglist

ILInstruction* Arglist::clone() const { return make<Arglist>(*this); }
bool Arglist::perform_match(ILInstruction* other, patterns::Match&) { return as<Arglist>(other) != nullptr; }

// ---- LocAlloc

ILInstruction* LocAlloc::clone() const {
    auto* clone = make<LocAlloc>(*this);
    clone_children_into(*clone);
    return clone;
}
bool LocAlloc::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<LocAlloc>(other);
    return o != nullptr && argument()->perform_match(o->argument(), match);
}

// ---- LocAllocSpan

ILInstruction* LocAllocSpan::clone() const {
    auto* clone = make<LocAllocSpan>(*this);
    clone_children_into(*clone);
    return clone;
}
void LocAllocSpan::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_type_and_argument(*this, type_, argument(), output, options);
}
bool LocAllocSpan::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<LocAllocSpan>(other);
    return o != nullptr && argument()->perform_match(o->argument(), match) && typesystem::type_equals(type_, o->type_);
}

// ---- Cpblk

const SlotInfo Cpblk::dest_address_slot{"DestAddress", true};
const SlotInfo Cpblk::source_address_slot{"SourceAddress", true};
const SlotInfo Cpblk::size_slot{"Size", true};

Cpblk::Cpblk(ILInstruction* dest_address, ILInstruction* source_address, ILInstruction* size) : ILInstruction(opcode) {
    set_dest_address(dest_address);
    set_source_address(source_address);
    set_size(size);
}

ILInstruction* Cpblk::get_child(int index) const {
    switch (index) {
        case 0: return dest_address_;
        case 1: return source_address_;
        case 2: return size_;
        default: index_out_of_range();
    }
}
void Cpblk::set_child(int index, ILInstruction* value) {
    switch (index) {
        case 0: return set_dest_address(value);
        case 1: return set_source_address(value);
        case 2: return set_size(value);
        default: index_out_of_range();
    }
}
const SlotInfo& Cpblk::get_child_slot(int index) const {
    switch (index) {
        case 0: return dest_address_slot;
        case 1: return source_address_slot;
        case 2: return size_slot;
        default: index_out_of_range();
    }
}
ILInstruction* Cpblk::clone() const {
    auto* clone = make<Cpblk>(*this);
    clone->set_dest_address(dest_address_->clone());
    clone->set_source_address(source_address_->clone());
    clone->set_size(size_->clone());
    return clone;
}
void Cpblk::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write_prefixes(output, is_volatile_, unaligned_prefix_);
    write(output, op_code);
    output.write('(');
    dest_address_->write_to(output, options);
    output.write(", ");
    source_address_->write_to(output, options);
    output.write(", ");
    size_->write_to(output, options);
    output.write(')');
}
bool Cpblk::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<Cpblk>(other);
    return o != nullptr && dest_address_->perform_match(o->dest_address_, match) && source_address_->perform_match(o->source_address_, match) &&
           size_->perform_match(o->size_, match) && is_volatile_ == o->is_volatile_ && unaligned_prefix_ == o->unaligned_prefix_;
}
void Cpblk::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    assert(dest_address_->result_type() == StackType::I || dest_address_->result_type() == StackType::Ref);
    assert(source_address_->result_type() == StackType::I || source_address_->result_type() == StackType::Ref);
    assert(size_->result_type() == StackType::I4);
}

// ---- Initblk

const SlotInfo Initblk::address_slot{"Address", true};
const SlotInfo Initblk::value_slot{"Value", true};
const SlotInfo Initblk::size_slot{"Size", true};

Initblk::Initblk(ILInstruction* address, ILInstruction* value, ILInstruction* size) : ILInstruction(opcode) {
    set_address(address);
    set_value(value);
    set_size(size);
}

ILInstruction* Initblk::get_child(int index) const {
    switch (index) {
        case 0: return address_;
        case 1: return value_;
        case 2: return size_;
        default: index_out_of_range();
    }
}
void Initblk::set_child(int index, ILInstruction* value) {
    switch (index) {
        case 0: return set_address(value);
        case 1: return set_value(value);
        case 2: return set_size(value);
        default: index_out_of_range();
    }
}
const SlotInfo& Initblk::get_child_slot(int index) const {
    switch (index) {
        case 0: return address_slot;
        case 1: return value_slot;
        case 2: return size_slot;
        default: index_out_of_range();
    }
}
ILInstruction* Initblk::clone() const {
    auto* clone = make<Initblk>(*this);
    clone->set_address(address_->clone());
    clone->set_value(value_->clone());
    clone->set_size(size_->clone());
    return clone;
}
void Initblk::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write_prefixes(output, is_volatile_, unaligned_prefix_);
    write(output, op_code);
    output.write('(');
    address_->write_to(output, options);
    output.write(", ");
    value_->write_to(output, options);
    output.write(", ");
    size_->write_to(output, options);
    output.write(')');
}
bool Initblk::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<Initblk>(other);
    return o != nullptr && address_->perform_match(o->address_, match) && value_->perform_match(o->value_, match) &&
           size_->perform_match(o->size_, match) && is_volatile_ == o->is_volatile_ && unaligned_prefix_ == o->unaligned_prefix_;
}
void Initblk::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    assert(address_->result_type() == StackType::I || address_->result_type() == StackType::Ref);
    assert(value_->result_type() == StackType::I4);
    assert(size_->result_type() == StackType::I4);
}

// ---- LdFlda

const SlotInfo LdFlda::target_slot{"Target", true};

LdFlda::LdFlda(ILInstruction* target, const IField* field) : ILInstruction(opcode), field_(field) { set_target(target); }

ILInstruction* LdFlda::get_child(int index) const {
    if (index == 0) return target_;
    index_out_of_range();
}
void LdFlda::set_child(int index, ILInstruction* value) {
    if (index == 0) return set_target(value);
    index_out_of_range();
}
const SlotInfo& LdFlda::get_child_slot(int index) const {
    if (index == 0) return target_slot;
    index_out_of_range();
}
ILInstruction* LdFlda::clone() const {
    auto* clone = make<LdFlda>(*this);
    clone->set_target(target_->clone());
    return clone;
}
StackType LdFlda::result_type() const { return is_integer_type(target_->result_type()) ? StackType::I : StackType::Ref; }
void LdFlda::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    if (delay_exceptions) output.write("delayex.");
    write(output, op_code);
    output.write(' ');
    il::write_to(*field_, output);
    output.write('(');
    target_->write_to(output, options);
    output.write(')');
}
bool LdFlda::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<LdFlda>(other);
    return o != nullptr && target_->perform_match(o->target_, match) && delay_exceptions == o->delay_exceptions &&
           typesystem::member_equals(field_, o->field_);
}
void LdFlda::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    std::optional<bool> is_reference = field_->declaring_type()->is_reference_type();
    StackType target_type = target_->result_type();
    if (!is_reference.has_value()) {
        // field of unresolved type
        assert((target_type == StackType::O || target_type == StackType::I || target_type == StackType::Ref || target_type == StackType::Unknown) &&
               "Field of unresolved type with invalid target");
    } else if (*is_reference) {
        assert(target_type == StackType::O && "Class fields can only be accessed with an object on the stack");
    } else {
        assert((target_type == StackType::I || target_type == StackType::Ref) && "Struct fields can only be accessed with a pointer on the stack");
    }
    (void)target_type;
}

// ---- LdsFlda

ILInstruction* LdsFlda::clone() const { return make<LdsFlda>(*this); }
void LdsFlda::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write(' ');
    il::write_to(*field_, output);
}
bool LdsFlda::perform_match(ILInstruction* other, patterns::Match&) {
    auto* o = as<LdsFlda>(other);
    return o != nullptr && typesystem::member_equals(field_, o->field_);
}

// ---- CastClass

ILInstruction* CastClass::clone() const {
    auto* clone = make<CastClass>(*this);
    clone_children_into(*clone);
    return clone;
}
StackType CastClass::result_type() const { return get_stack_type(*type_); }
void CastClass::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_type_and_argument(*this, type_, argument(), output, options);
}
bool CastClass::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<CastClass>(other);
    return o != nullptr && argument()->perform_match(o->argument(), match) && typesystem::type_equals(type_, o->type_);
}

// ---- IsInst

ILInstruction* IsInst::clone() const {
    auto* clone = make<IsInst>(*this);
    clone_children_into(*clone);
    return clone;
}
void IsInst::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_type_and_argument(*this, type_, argument(), output, options);
}
bool IsInst::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<IsInst>(other);
    return o != nullptr && argument()->perform_match(o->argument(), match) && typesystem::type_equals(type_, o->type_);
}
bool IsInst::can_inline_into_slot(int child_index, ILInstruction* new_child) {
    assert(child_index == 0);
    assert(ILInstruction::can_inline_into_slot(child_index, new_child));
    (void)child_index;
    if (type_->is_reference_type() == true) {
        return true;  // reference-type isinst is always supported
    }
    if (SemanticHelper::is_pure(new_child->flags())) {
        return true;  // emulated via "expr is T ? (T)expr : null"
    } else if (auto* box = as<Box>(new_child); box != nullptr && SemanticHelper::is_pure(box->argument()->flags()) && argument()->children().count() == 0) {
        // Also emulated via "expr is T ? (T)expr : null". This duplicates the boxing side-effect, but that is harmless as one of the
        // boxes is only used in the `expr is T` type test where the object identity can never be observed. This appears as part of C#
        // pattern matching, inlining early makes those code patterns easier to detect. We can only do this if the Box appears directly
        // top-level in the IsInst, we cannot inline Box instructions deeper into our Argument subtree. So restrict to the case where
        // the previous argument has no children (which means inlining can only replace the argument, not insert within it).
        return true;
    }
    if (auto* unbox_any = as<UnboxAny>(parent()); unbox_any != nullptr && is_unbox_any_with_is_inst(*unbox_any, type_)) {
        return true;  // supported pattern "expr as T?"
    }
    if (parent() != nullptr) {
        ILInstruction* ignored = nullptr;
        if (parent()->match_comp_equals_null(&ignored) || parent()->match_comp_not_equals_null(&ignored)) {
            return true;  // supported pattern "expr is T"
        }
    }
    if (auto* block = as<Block>(parent()); block != nullptr && block->kind == BlockKind::ControlFlow) {
        return true;  // supported via StatementBuilder.VisitIsInst
    }
    return false;
}

// ---- LdObj

const SlotInfo LdObj::target_slot{"Target", true};

LdObj::LdObj(ILInstruction* target, const IType* type) : ILInstruction(opcode), type_(type) { set_target(target); }

ILInstruction* LdObj::get_child(int index) const {
    if (index == 0) return target_;
    index_out_of_range();
}
void LdObj::set_child(int index, ILInstruction* value) {
    if (index == 0) return set_target(value);
    index_out_of_range();
}
const SlotInfo& LdObj::get_child_slot(int index) const {
    if (index == 0) return target_slot;
    index_out_of_range();
}
ILInstruction* LdObj::clone() const {
    auto* clone = make<LdObj>(*this);
    clone->set_target(target_->clone());
    return clone;
}
StackType LdObj::result_type() const { return get_stack_type(*type_); }
void LdObj::original_write_to(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write_prefixes(output, is_volatile_, unaligned_prefix_);
    write(output, op_code);
    output.write(' ');
    il::write_to(*type_, output);
    output.write('(');
    target_->write_to(output, options);
    output.write(')');
}
void LdObj::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    if (options.use_field_sugar) {
        ILInstruction* target = nullptr;
        const IField* field = nullptr;
        if (match_ld_fld(&target, &field)) {
            write_il_range(output, options);
            output.write("ldfld ");
            il::write_to(*field, output);
            output.write('(');
            target->write_to(output, options);
            output.write(')');
            return;
        } else if (match_lds_fld(&field)) {
            write_il_range(output, options);
            output.write("ldsfld ");
            il::write_to(*field, output);
            return;
        }
    }
    original_write_to(output, options);
}
bool LdObj::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<LdObj>(other);
    return o != nullptr && target_->perform_match(o->target_, match) && typesystem::type_equals(type_, o->type_) &&
           is_volatile_ == o->is_volatile_ && unaligned_prefix_ == o->unaligned_prefix_;
}
void LdObj::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    assert(target_->result_type() == StackType::Ref || target_->result_type() == StackType::I);
}

// ---- LdObjIfRef

const SlotInfo LdObjIfRef::target_slot{"Target", true};

LdObjIfRef::LdObjIfRef(ILInstruction* target, const IType* type) : ILInstruction(opcode), type_(type) { set_target(target); }

ILInstruction* LdObjIfRef::get_child(int index) const {
    if (index == 0) return target_;
    index_out_of_range();
}
void LdObjIfRef::set_child(int index, ILInstruction* value) {
    if (index == 0) return set_target(value);
    index_out_of_range();
}
const SlotInfo& LdObjIfRef::get_child_slot(int index) const {
    if (index == 0) return target_slot;
    index_out_of_range();
}
ILInstruction* LdObjIfRef::clone() const {
    auto* clone = make<LdObjIfRef>(*this);
    clone->set_target(target_->clone());
    return clone;
}
void LdObjIfRef::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_type_and_argument(*this, type_, target_, output, options);
}
bool LdObjIfRef::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<LdObjIfRef>(other);
    return o != nullptr && target_->perform_match(o->target_, match) && typesystem::type_equals(type_, o->type_);
}
void LdObjIfRef::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    assert(target_->result_type() == StackType::Ref || target_->result_type() == StackType::I);
}

// ---- StObj

const SlotInfo StObj::target_slot{"Target", true};
const SlotInfo StObj::value_slot{"Value", true};

StObj::StObj(ILInstruction* target, ILInstruction* value, const IType* type) : ILInstruction(opcode), type_(type) {
    set_target(target);
    set_value(value);
}

ILInstruction* StObj::get_child(int index) const {
    switch (index) {
        case 0: return target_;
        case 1: return value_;
        default: index_out_of_range();
    }
}
void StObj::set_child(int index, ILInstruction* value) {
    switch (index) {
        case 0: return set_target(value);
        case 1: return set_value(value);
        default: index_out_of_range();
    }
}
const SlotInfo& StObj::get_child_slot(int index) const {
    switch (index) {
        case 0: return target_slot;
        case 1: return value_slot;
        default: index_out_of_range();
    }
}
ILInstruction* StObj::clone() const {
    auto* clone = make<StObj>(*this);
    clone->set_target(target_->clone());
    clone->set_value(value_->clone());
    return clone;
}
StackType StObj::result_type() const { return unaligned_prefix_ == 0 ? get_stack_type(*type_) : StackType::Void; }
void StObj::original_write_to(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write_prefixes(output, is_volatile_, unaligned_prefix_);
    write(output, op_code);
    output.write(' ');
    il::write_to(*type_, output);
    output.write('(');
    target_->write_to(output, options);
    output.write(", ");
    value_->write_to(output, options);
    output.write(')');
}
void StObj::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    if (options.use_field_sugar) {
        ILInstruction* target = nullptr;
        ILInstruction* value = nullptr;
        const IField* field = nullptr;
        if (match_st_fld(&target, &field, &value)) {
            write_il_range(output, options);
            output.write("stfld ");
            il::write_to(*field, output);
            output.write('(');
            target->write_to(output, options);
            output.write(", ");
            value->write_to(output, options);
            output.write(')');
            return;
        } else if (match_sts_fld(&field, &value)) {
            write_il_range(output, options);
            output.write("stsfld ");
            il::write_to(*field, output);
            output.write('(');
            value->write_to(output, options);
            output.write(')');
            return;
        }
    }
    original_write_to(output, options);
}
bool StObj::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<StObj>(other);
    return o != nullptr && target_->perform_match(o->target_, match) && value_->perform_match(o->value_, match) &&
           typesystem::type_equals(type_, o->type_) && is_volatile_ == o->is_volatile_ && unaligned_prefix_ == o->unaligned_prefix_;
}
void StObj::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    assert(target_->result_type() == StackType::Ref || target_->result_type() == StackType::I);
    assert(value_->result_type() == get_stack_type(*type_));
    check_target_slot();
}
bool StObj::satisfies_slot_restriction_for_inlining(int child_index, ILInstruction* new_child) {
    if (child_index == 0) {  // target slot
        assert(&get_child_slot(child_index) == &target_slot);
        // For a store to a field or array element, C# will only throw NullReferenceException/IndexOfBoundsException after the
        // value-to-be-stored has been computed. This means a LdFlda/LdElema used as target for StObj must have DelayExceptions==true
        // to allow a translation to C# without changing the program semantics. See https://github.com/icsharpcode/ILSpy/issues/2050
        switch (new_child->op_code) {
            case OpCode::LdElema:
            case OpCode::LdFlda:
                assert(new_child->has_direct_flag(InstructionFlags::MayThrow));
                // If the ldelema/ldflda may throw a non-delayed exception, inlining will cause it to turn into a delayed exception
                // after the translation to C#. This is only valid if the value computation doesn't involve any side effects.
                if (!SemanticHelper::is_pure(value()->flags())) return false;
                // Note that after inlining such a ldelema/ldflda, the normal inlining rules will prevent us from inlining an effectful
                // instruction into the value slot.
                break;
            default: break;
        }
    }
    return ILInstruction::satisfies_slot_restriction_for_inlining(child_index, new_child);
}
void StObj::check_target_slot() const {
    switch (target_->op_code) {
        case OpCode::LdElema:
        case OpCode::LdFlda:
            if (target_->has_direct_flag(InstructionFlags::MayThrow)) assert(SemanticHelper::is_pure(value_->flags()));
            break;
        default: break;
    }
}

// ---- Box

ILInstruction* Box::clone() const {
    auto* clone = make<Box>(*this);
    clone_children_into(*clone);
    return clone;
}
void Box::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_type_and_argument(*this, type_, argument(), output, options);
}
bool Box::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<Box>(other);
    return o != nullptr && argument()->perform_match(o->argument(), match) && typesystem::type_equals(type_, o->type_);
}

// ---- Unbox

ILInstruction* Unbox::clone() const {
    auto* clone = make<Unbox>(*this);
    clone_children_into(*clone);
    return clone;
}
void Unbox::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_type_and_argument(*this, type_, argument(), output, options);
}
bool Unbox::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<Unbox>(other);
    return o != nullptr && argument()->perform_match(o->argument(), match) && typesystem::type_equals(type_, o->type_);
}

// ---- UnboxAny

ILInstruction* UnboxAny::clone() const {
    auto* clone = make<UnboxAny>(*this);
    clone_children_into(*clone);
    return clone;
}
StackType UnboxAny::result_type() const { return get_stack_type(*type_); }
void UnboxAny::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_type_and_argument(*this, type_, argument(), output, options);
}
bool UnboxAny::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<UnboxAny>(other);
    return o != nullptr && argument()->perform_match(o->argument(), match) && typesystem::type_equals(type_, o->type_);
}

// ---- NewArr

const SlotInfo NewArr::indices_slot{"Indices", true};

NewArr::NewArr(const IType* type, const std::vector<ILInstruction*>& indices) : ILInstruction(opcode), type_(type), indices_(this, 0) {
    indices_.add_range(indices);
}
NewArr::NewArr(const NewArr& other) : ILInstruction(other), type_(other.type_), indices_(this, 0) {}

ILInstruction* NewArr::get_child(int index) const { return indices_[index]; }
void NewArr::set_child(int index, ILInstruction* value) { indices_.set(index, value); }
const SlotInfo& NewArr::get_child_slot(int) const { return indices_slot; }
ILInstruction* NewArr::clone() const {
    auto* clone = make<NewArr>(*this);
    clone->indices_.add_range(clone_all(indices_));
    return clone;
}
InstructionFlags NewArr::compute_flags() const { return indices_flags(indices_) | InstructionFlags::MayThrow; }
void NewArr::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write(' ');
    il::write_to(*type_, output);
    output.write('(');
    bool first = true;
    for (ILInstruction* index : indices_) {
        if (!first)
            output.write(", ");
        else
            first = false;
        index->write_to(output, options);
    }
    output.write(')');
}
bool NewArr::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<NewArr>(other);
    return o != nullptr && typesystem::type_equals(type_, o->type_) && patterns::ListMatch::do_match(indices_, o->indices_, match);
}

// ---- DefaultValue

ILInstruction* DefaultValue::clone() const { return make<DefaultValue>(*this); }
StackType DefaultValue::result_type() const { return get_stack_type(*type_); }
void DefaultValue::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write(' ');
    il::write_to(*type_, output);
}
bool DefaultValue::perform_match(ILInstruction* other, patterns::Match&) {
    auto* o = as<DefaultValue>(other);
    return o != nullptr && typesystem::type_equals(type_, o->type_);
}

// ---- Throw

ILInstruction* Throw::clone() const {
    auto* clone = make<Throw>(*this);
    clone_children_into(*clone);
    return clone;
}
bool Throw::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<Throw>(other);
    return o != nullptr && argument()->perform_match(o->argument(), match);
}

// ---- Rethrow

ILInstruction* Rethrow::clone() const { return make<Rethrow>(*this); }
bool Rethrow::perform_match(ILInstruction* other, patterns::Match&) { return as<Rethrow>(other) != nullptr; }

// ---- SizeOf

ILInstruction* SizeOf::clone() const { return make<SizeOf>(*this); }
void SizeOf::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write(' ');
    il::write_to(*type_, output);
}
bool SizeOf::perform_match(ILInstruction* other, patterns::Match&) {
    auto* o = as<SizeOf>(other);
    return o != nullptr && typesystem::type_equals(type_, o->type_);
}

// ---- LdLen

const SlotInfo LdLen::array_slot{"Array", true};

LdLen::LdLen(StackType type, ILInstruction* array) : ILInstruction(opcode), result_type_(type) {
    assert(type == StackType::I || type == StackType::I4 || type == StackType::I8);
    set_array(array);
}

ILInstruction* LdLen::get_child(int index) const {
    if (index == 0) return array_;
    index_out_of_range();
}
void LdLen::set_child(int index, ILInstruction* value) {
    if (index == 0) return set_array(value);
    index_out_of_range();
}
const SlotInfo& LdLen::get_child_slot(int index) const {
    if (index == 0) return array_slot;
    index_out_of_range();
}
ILInstruction* LdLen::clone() const {
    auto* clone = make<LdLen>(*this);
    clone->set_array(array_->clone());
    return clone;
}
void LdLen::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write('.');
    write(output, result_type_);
    output.write('(');
    array_->write_to(output, options);
    output.write(')');
}
bool LdLen::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<LdLen>(other);
    return o != nullptr && array_->perform_match(o->array_, match);
}
void LdLen::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    assert(array_->result_type() == StackType::O);
}

// ---- LdElema

const SlotInfo LdElema::array_slot{"Array", true};
const SlotInfo LdElema::indices_slot{"Indices", true};

LdElema::LdElema(const IType* type, ILInstruction* array, const std::vector<ILInstruction*>& indices)
    : ILInstruction(opcode), type_(type), indices_(this, 1) {
    set_array(array);
    indices_.add_range(indices);
}
LdElema::LdElema(const LdElema& other)
    : ILInstruction(other), with_system_index(other.with_system_index), delay_exceptions(other.delay_exceptions), is_read_only(other.is_read_only),
      type_(other.type_), array_(other.array_), indices_(this, 1) {}

ILInstruction* LdElema::get_child(int index) const {
    if (index == 0) return array_;
    return indices_[index - 1];
}
void LdElema::set_child(int index, ILInstruction* value) {
    if (index == 0) return set_array(value);
    indices_.set(index - 1, value);
}
const SlotInfo& LdElema::get_child_slot(int index) const { return index == 0 ? array_slot : indices_slot; }
ILInstruction* LdElema::clone() const {
    auto* clone = make<LdElema>(*this);
    clone->set_array(array_->clone());
    clone->indices_.add_range(clone_all(indices_));
    return clone;
}
InstructionFlags LdElema::compute_flags() const {
    return array_->flags() | indices_flags(indices_) | (delay_exceptions ? InstructionFlags::None : InstructionFlags::MayThrow);
}
void LdElema::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    if (with_system_index) output.write("withsystemindex.");
    if (delay_exceptions) output.write("delayex.");
    if (is_read_only) output.write("readonly.");
    write(output, op_code);
    output.write(' ');
    il::write_to(*type_, output);
    output.write('(');
    array_->write_to(output, options);
    write_indices(output, options, indices_);
    output.write(')');
}
bool LdElema::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<LdElema>(other);
    return o != nullptr && typesystem::type_equals(type_, o->type_) && array_->perform_match(o->array_, match) &&
           patterns::ListMatch::do_match(indices_, o->indices_, match) && with_system_index == o->with_system_index &&
           delay_exceptions == o->delay_exceptions && is_read_only == o->is_read_only;
}

// ---- LdElemaInlineArray

const SlotInfo LdElemaInlineArray::array_slot{"Array", true};
const SlotInfo LdElemaInlineArray::indices_slot{"Indices", true};

LdElemaInlineArray::LdElemaInlineArray(const IType* type, ILInstruction* array, const std::vector<ILInstruction*>& indices)
    : ILInstruction(opcode), type_(type), indices_(this, 1) {
    set_array(array);
    indices_.add_range(indices);
}
LdElemaInlineArray::LdElemaInlineArray(const LdElemaInlineArray& other)
    : ILInstruction(other), is_read_only(other.is_read_only), type_(other.type_), array_(other.array_), indices_(this, 1) {}

ILInstruction* LdElemaInlineArray::get_child(int index) const {
    if (index == 0) return array_;
    return indices_[index - 1];
}
void LdElemaInlineArray::set_child(int index, ILInstruction* value) {
    if (index == 0) return set_array(value);
    indices_.set(index - 1, value);
}
const SlotInfo& LdElemaInlineArray::get_child_slot(int index) const { return index == 0 ? array_slot : indices_slot; }
ILInstruction* LdElemaInlineArray::clone() const {
    auto* clone = make<LdElemaInlineArray>(*this);
    clone->set_array(array_->clone());
    clone->indices_.add_range(clone_all(indices_));
    return clone;
}
InstructionFlags LdElemaInlineArray::compute_flags() const {
    return array_->flags() | indices_flags(indices_) | InstructionFlags::MayThrow;
}
void LdElemaInlineArray::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    if (is_read_only) output.write("readonly.");
    write(output, op_code);
    output.write(' ');
    il::write_to(*type_, output);
    output.write('(');
    array_->write_to(output, options);
    write_indices(output, options, indices_);
    output.write(')');
}
bool LdElemaInlineArray::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<LdElemaInlineArray>(other);
    return o != nullptr && typesystem::type_equals(type_, o->type_) && array_->perform_match(o->array_, match) &&
           patterns::ListMatch::do_match(indices_, o->indices_, match) && is_read_only == o->is_read_only;
}

// ---- GetPinnableReference

const SlotInfo GetPinnableReference::argument_slot{"Argument", true};

GetPinnableReference::GetPinnableReference(ILInstruction* argument, const IMethod* method) : ILInstruction(opcode), method_(method) {
    set_argument(argument);
}

ILInstruction* GetPinnableReference::get_child(int index) const {
    if (index == 0) return argument_;
    index_out_of_range();
}
void GetPinnableReference::set_child(int index, ILInstruction* value) {
    if (index == 0) return set_argument(value);
    index_out_of_range();
}
const SlotInfo& GetPinnableReference::get_child_slot(int index) const {
    if (index == 0) return argument_slot;
    index_out_of_range();
}
ILInstruction* GetPinnableReference::clone() const {
    auto* clone = make<GetPinnableReference>(*this);
    clone->set_argument(argument_->clone());
    return clone;
}
void GetPinnableReference::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    if (method_ != nullptr) {
        output.write(' ');
        il::write_to(*method_, output);
    }
    output.write('(');
    argument_->write_to(output, options);
    output.write(')');
}
bool GetPinnableReference::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<GetPinnableReference>(other);
    return o != nullptr && argument_->perform_match(o->argument_, match) && typesystem::member_equals(method_, o->method_);
}
void GetPinnableReference::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    assert(argument_->result_type() == StackType::O);
}

// ---- MakeRefAny

ILInstruction* MakeRefAny::clone() const {
    auto* clone = make<MakeRefAny>(*this);
    clone_children_into(*clone);
    return clone;
}
void MakeRefAny::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_type_and_argument(*this, type_, argument(), output, options);
}
bool MakeRefAny::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<MakeRefAny>(other);
    return o != nullptr && argument()->perform_match(o->argument(), match) && typesystem::type_equals(type_, o->type_);
}

// ---- RefAnyType

ILInstruction* RefAnyType::clone() const {
    auto* clone = make<RefAnyType>(*this);
    clone_children_into(*clone);
    return clone;
}
bool RefAnyType::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<RefAnyType>(other);
    return o != nullptr && argument()->perform_match(o->argument(), match);
}

// ---- RefAnyValue

ILInstruction* RefAnyValue::clone() const {
    auto* clone = make<RefAnyValue>(*this);
    clone_children_into(*clone);
    return clone;
}
void RefAnyValue::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_type_and_argument(*this, type_, argument(), output, options);
}
bool RefAnyValue::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<RefAnyValue>(other);
    return o != nullptr && argument()->perform_match(o->argument(), match) && typesystem::type_equals(type_, o->type_);
}

// ---- the Match* methods of ILInstruction

bool ILInstruction::match_arglist() const { return as<Arglist>(this) != nullptr; }

bool ILInstruction::match_loc_alloc(ILInstruction** argument) const {
    if (auto* inst = as<LocAlloc>(this)) {
        *argument = inst->argument();
        return true;
    }
    *argument = nullptr;
    return false;
}

bool ILInstruction::match_loc_alloc_span(ILInstruction** argument, const IType** type) const {
    if (auto* inst = as<LocAllocSpan>(this)) {
        *argument = inst->argument();
        *type = inst->type();
        return true;
    }
    *argument = nullptr;
    *type = nullptr;
    return false;
}

bool ILInstruction::match_cpblk(ILInstruction** dest_address, ILInstruction** source_address, ILInstruction** size) const {
    if (auto* inst = as<Cpblk>(this)) {
        *dest_address = inst->dest_address();
        *source_address = inst->source_address();
        *size = inst->size();
        return true;
    }
    *dest_address = nullptr;
    *source_address = nullptr;
    *size = nullptr;
    return false;
}

bool ILInstruction::match_initblk(ILInstruction** address, ILInstruction** value, ILInstruction** size) const {
    if (auto* inst = as<Initblk>(this)) {
        *address = inst->address();
        *value = inst->value();
        *size = inst->size();
        return true;
    }
    *address = nullptr;
    *value = nullptr;
    *size = nullptr;
    return false;
}

bool ILInstruction::match_ld_flda(ILInstruction** target, const IField** field) const {
    if (auto* inst = as<LdFlda>(this)) {
        *target = inst->target();
        *field = inst->field();
        return true;
    }
    *target = nullptr;
    *field = nullptr;
    return false;
}

bool ILInstruction::match_lds_flda(const IField** field) const {
    if (auto* inst = as<LdsFlda>(this)) {
        *field = inst->field();
        return true;
    }
    *field = nullptr;
    return false;
}

bool ILInstruction::match_cast_class(ILInstruction** argument, const IType** type) const {
    if (auto* inst = as<CastClass>(this)) {
        *argument = inst->argument();
        *type = inst->type();
        return true;
    }
    *argument = nullptr;
    *type = nullptr;
    return false;
}

bool ILInstruction::match_is_inst(ILInstruction** argument, const IType** type) const {
    if (auto* inst = as<IsInst>(this)) {
        *argument = inst->argument();
        *type = inst->type();
        return true;
    }
    *argument = nullptr;
    *type = nullptr;
    return false;
}

bool ILInstruction::match_ld_obj(ILInstruction** target, const IType** type) const {
    if (auto* inst = as<LdObj>(this)) {
        *target = inst->target();
        *type = inst->type();
        return true;
    }
    *target = nullptr;
    *type = nullptr;
    return false;
}

bool ILInstruction::match_ld_obj(ILInstruction** target, const IType* type) const {
    auto* inst = as<LdObj>(this);
    if (inst != nullptr && typesystem::type_equals(inst->type(), type)) {
        *target = inst->target();
        return true;
    }
    *target = nullptr;
    return false;
}

bool ILInstruction::match_ld_obj_if_ref(ILInstruction** target, const IType** type) const {
    if (auto* inst = as<LdObjIfRef>(this)) {
        *target = inst->target();
        *type = inst->type();
        return true;
    }
    *target = nullptr;
    *type = nullptr;
    return false;
}

bool ILInstruction::match_st_obj(ILInstruction** target, ILInstruction** value, const IType** type) const {
    if (auto* inst = as<StObj>(this)) {
        *target = inst->target();
        *value = inst->value();
        *type = inst->type();
        return true;
    }
    *target = nullptr;
    *value = nullptr;
    *type = nullptr;
    return false;
}

bool ILInstruction::match_box(ILInstruction** argument, const IType** type) const {
    if (auto* inst = as<Box>(this)) {
        *argument = inst->argument();
        *type = inst->type();
        return true;
    }
    *argument = nullptr;
    *type = nullptr;
    return false;
}

bool ILInstruction::match_unbox(ILInstruction** argument, const IType** type) const {
    if (auto* inst = as<Unbox>(this)) {
        *argument = inst->argument();
        *type = inst->type();
        return true;
    }
    *argument = nullptr;
    *type = nullptr;
    return false;
}

bool ILInstruction::match_unbox_any(ILInstruction** argument, const IType** type) const {
    if (auto* inst = as<UnboxAny>(this)) {
        *argument = inst->argument();
        *type = inst->type();
        return true;
    }
    *argument = nullptr;
    *type = nullptr;
    return false;
}

bool ILInstruction::match_new_arr(const IType** type) const {
    if (auto* inst = as<NewArr>(this)) {
        *type = inst->type();
        return true;
    }
    *type = nullptr;
    return false;
}

bool ILInstruction::match_default_value(const IType** type) const {
    if (auto* inst = as<DefaultValue>(this)) {
        *type = inst->type();
        return true;
    }
    *type = nullptr;
    return false;
}

bool ILInstruction::match_throw(ILInstruction** argument) const {
    if (auto* inst = as<Throw>(this)) {
        *argument = inst->argument();
        return true;
    }
    *argument = nullptr;
    return false;
}

bool ILInstruction::match_rethrow() const { return as<Rethrow>(this) != nullptr; }

bool ILInstruction::match_size_of(const IType** type) const {
    if (auto* inst = as<SizeOf>(this)) {
        *type = inst->type();
        return true;
    }
    *type = nullptr;
    return false;
}

bool ILInstruction::match_ld_elema(const IType** type, ILInstruction** array) const {
    if (auto* inst = as<LdElema>(this)) {
        *type = inst->type();
        *array = inst->array();
        return true;
    }
    *type = nullptr;
    *array = nullptr;
    return false;
}

bool ILInstruction::match_ld_elema_inline_array(const IType** type, ILInstruction** array) const {
    if (auto* inst = as<LdElemaInlineArray>(this)) {
        *type = inst->type();
        *array = inst->array();
        return true;
    }
    *type = nullptr;
    *array = nullptr;
    return false;
}

bool ILInstruction::match_get_pinnable_reference(ILInstruction** argument, const IMethod** method) const {
    if (auto* inst = as<GetPinnableReference>(this)) {
        *argument = inst->argument();
        *method = inst->method();
        return true;
    }
    *argument = nullptr;
    *method = nullptr;
    return false;
}

bool ILInstruction::match_make_ref_any(ILInstruction** argument, const IType** type) const {
    if (auto* inst = as<MakeRefAny>(this)) {
        *argument = inst->argument();
        *type = inst->type();
        return true;
    }
    *argument = nullptr;
    *type = nullptr;
    return false;
}

bool ILInstruction::match_ref_any_type(ILInstruction** argument) const {
    if (auto* inst = as<RefAnyType>(this)) {
        *argument = inst->argument();
        return true;
    }
    *argument = nullptr;
    return false;
}

bool ILInstruction::match_ref_any_value(ILInstruction** argument, const IType** type) const {
    if (auto* inst = as<RefAnyValue>(this)) {
        *argument = inst->argument();
        *type = inst->type();
        return true;
    }
    *argument = nullptr;
    *type = nullptr;
    return false;
}

// from PatternMatching.cs
bool ILInstruction::match_ld_len(StackType type, ILInstruction** array) const {
    auto* inst = as<LdLen>(this);
    if (inst != nullptr && inst->result_type() == type) {
        *array = inst->array();
        return true;
    }
    *array = nullptr;
    return false;
}

}  // namespace cb::dotnet::decompiler::il
