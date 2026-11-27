// dotnet/decompiler/il/transforms/index_range_transform.cxx -- see index_range_transform.hpp.
#include "dotnet/decompiler/il/transforms/index_range_transform.hpp"

#include <cassert>
#include <stdexcept>

#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/il/transforms/il_inlining.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::il::transforms {

using typesystem::KnownTypeCode;
using typesystem::MethodSemanticsAttributes;
using typesystem::SymbolKind;

namespace {

// new SyntheticRangeIndexAccessor(method, indexOrRangeType, slicing): the method as taking a System.Index or System.Range.
const IMethod* make_synthetic_range_index_accessor(const IMethod*, const IType*, bool) {
    // WAITS: SyntheticRangeIndexAccessor (type system; without it the call is not replaced)
    return nullptr;
}

// `method is SyntheticRangeIndexAccessor`.
bool is_synthetic_range_index_accessor(const IMethod*) {
    // WAITS: SyntheticRangeIndexAccessor (type system; no method is taken for one)
    return false;
}

}  // namespace

// The methods of System.Index and System.Range the transform makes calls to.
class IndexRangeTransform::IndexMethods {
public:
    const IMethod* index_ctor = nullptr;
    const IMethod* index_implicit_conv = nullptr;
    const IMethod* range_ctor = nullptr;
    const IType* index_type() const { return index_ctor != nullptr ? index_ctor->declaring_type() : nullptr; }
    const IType* range_type() const { return range_ctor != nullptr ? range_ctor->declaring_type() : nullptr; }
    bool is_valid() const { return index_ctor != nullptr && index_implicit_conv != nullptr && range_ctor != nullptr; }

    const IMethod* range_start_at = nullptr;
    const IMethod* range_end_at = nullptr;
    const IMethod* range_get_all = nullptr;

    explicit IndexMethods(const typesystem::ICompilation& compilation) {
        const IType* index_type = compilation.find_type(KnownTypeCode::Index);
        for (const IMethod* ctor : index_type->get_constructors([](const IMethod* m) { return m->parameters().size() == 2; })) {
            if (typesystem::is_known_type(ctor->parameters()[0]->type(), KnownTypeCode::Int32) &&
                typesystem::is_known_type(ctor->parameters()[1]->type(), KnownTypeCode::Boolean)) {
                index_ctor = ctor;
            }
        }
        for (const IMethod* op : index_type->get_methods([](const IMethod* m) { return m->is_operator() && m->name() == "op_Implicit"; })) {
            if (op->parameters().size() == 1 && typesystem::is_known_type(op->parameters()[0]->type(), KnownTypeCode::Int32)) {
                index_implicit_conv = op;
            }
        }
        const IType* range_type = compilation.find_type(KnownTypeCode::Range);
        for (const IMethod* ctor : range_type->get_constructors([](const IMethod* m) { return m->parameters().size() == 2; })) {
            if (typesystem::is_known_type(ctor->parameters()[0]->type(), KnownTypeCode::Index) &&
                typesystem::is_known_type(ctor->parameters()[1]->type(), KnownTypeCode::Index)) {
                range_ctor = ctor;
            }
        }
        for (const IMethod* m : range_type->get_methods([](const IMethod* candidate) { return candidate->parameters().size() == 1; })) {
            if (m->parameters().size() == 1 && typesystem::is_known_type(m->parameters()[0]->type(), KnownTypeCode::Index)) {
                if (m->name() == "StartAt")
                    range_start_at = m;
                else if (m->name() == "EndAt")
                    range_end_at = m;
            }
        }
        for (const typesystem::IProperty* p :
             range_type->get_properties([](const typesystem::IProperty* candidate) { return candidate->is_static() && candidate->name() == "All"; })) {
            range_get_all = p->getter();
        }
    }

    static bool is_range_ctor(const IMethod* method) {
        if (!(method->symbol_kind() == SymbolKind::Constructor && method->parameters().size() == 2 &&
              typesystem::is_known_type(method->declaring_type(), KnownTypeCode::Range)))
            return false;
        for (const typesystem::IParameter* p : method->parameters()) {
            if (!typesystem::is_known_type(p->type(), KnownTypeCode::Index)) return false;
        }
        return true;
    }
};

bool IndexRangeTransform::handle_ld_elema(LdElema* ldelema, ILTransformContext& context) {
    if (!context.settings().ranges) return false;
    ILVariable* array = nullptr;
    if (!ldelema->array()->match_ld_loc(&array)) return false;
    if (ldelema->indices().count() != 1) return false;  // the index/range feature doesn't support multi-dimensional arrays
    ILInstruction* index = ldelema->indices()[0];
    auto* call = as<CallInstruction>(index);
    auto* bni = as<BinaryNumericInstruction>(index);
    if (call != nullptr && call->method->name() == "GetOffset" && typesystem::is_known_type(call->method->declaring_type(), KnownTypeCode::Index)) {
        // ldelema T(ldloc array, call GetOffset(..., ldlen.i4(ldloc array)))
        // -> withsystemindex.ldelema T(ldloc array, ...)
        if (call->arguments().count() != 2) return false;
        ILInstruction* array_load = nullptr;
        if (!(call->arguments()[1]->match_ld_len(StackType::I4, &array_load) && array_load->match_ld_loc(array))) return false;
        context.step("ldelema with System.Index", ldelema);
        for (ILInstruction* node : call->arguments()[1]->descendants()) ldelema->add_il_range(*node);
        ldelema->add_il_range(*call);
        ldelema->with_system_index = true;
        // The method call had a `ref System.Index` argument for the this pointer, but we want a `System.Index` by-value.
        ldelema->indices().set(0, make<LdObj>(call->arguments()[0], call->method->declaring_type()));
        return true;
    } else if (bni != nullptr && bni->op == BinaryNumericOperator::Sub && !bni->is_lifted() && !bni->check_for_overflow) {
        // ldelema T(ldloc array, binary.sub.i4(ldlen.i4(ldloc array), ...))
        // -> withsystemindex.ldelema T(ldloc array, newobj System.Index(..., fromEnd: true))
        ILInstruction* array_load = nullptr;
        if (!(bni->left()->match_ld_len(StackType::I4, &array_load) && array_load->match_ld_loc(array))) return false;
        IndexMethods index_methods(context.type_system());
        if (!index_methods.is_valid()) return false;  // don't use System.Index if not supported by the target framework
        context.step("ldelema indexed from end", ldelema);
        for (ILInstruction* node : bni->left()->descendants()) ldelema->add_il_range(*node);
        ldelema->add_il_range(*bni);
        ldelema->with_system_index = true;
        ldelema->indices().set(0, make_index(IndexKind::FromEnd, bni->right(), index_methods));
        return true;
    }

    return false;
}

void IndexRangeTransform::run(Block* block, int pos, StatementTransformContext& context) {
    if (!context.settings().ranges) return;
    int start_pos = pos;
    ILVariable* container_var = nullptr;
    ILVariable* container_length_var = nullptr;
    // The container length access may be a separate instruction, or it may be inline with the variable's use
    if (match_container_length_store(block->instructions[pos], &container_length_var, &container_var)) {
        //  stloc containerLengthVar(call get_Length/get_Count(ldloc container))
        pos++;
    } else {
        // Reset if MatchContainerLengthStore only had a partial match. MatchGetOffset() will then set `containerVar`.
        container_length_var = nullptr;
        container_var = nullptr;
    }
    ILVariable* range_var = nullptr;
    ILInstruction* range_var_init = nullptr;
    if (block->instructions[pos]->match_st_loc(&range_var, &range_var_init) && typesystem::is_known_type(range_var->type(), KnownTypeCode::Range)) {
        // stloc rangeVar(rangeVarInit)
        pos++;
    } else {
        range_var = nullptr;
        range_var_init = nullptr;
    }

    // stloc startOffsetVar(call GetOffset(startIndexLoad, ldloc length))
    ILVariable* start_offset_var = nullptr;
    ILInstruction* start_offset_var_init = nullptr;

    // The local functions of the upstream method, in the order they are needed.
    ILInstruction* start_index_load = nullptr;
    IndexKind start_index_kind = IndexKind::FromStart;

    auto make_range = [&](IndexKind start_kind, ILInstruction* start_load, IndexKind end_kind, ILInstruction* end_load,
                          const IndexMethods& special_methods) -> ILInstruction* {
        if (range_var != nullptr) {
            return range_var_init;
        } else if (start_kind == IndexKind::TheStart && end_kind == IndexKind::TheEnd && special_methods.range_get_all != nullptr) {
            return make<Call>(special_methods.range_get_all);
        } else if (start_kind == IndexKind::TheStart && special_methods.range_end_at != nullptr) {
            auto* range_ctor_call = make<Call>(special_methods.range_end_at);
            range_ctor_call->arguments().add(make_index(end_kind, end_load, special_methods));
            return range_ctor_call;
        } else if (end_kind == IndexKind::TheEnd && special_methods.range_start_at != nullptr) {
            auto* range_ctor_call = make<Call>(special_methods.range_start_at);
            range_ctor_call->arguments().add(make_index(start_kind, start_load, special_methods));
            return range_ctor_call;
        } else {
            auto* range_ctor_call = make<NewObj>(special_methods.range_ctor);
            range_ctor_call->arguments().add(make_index(start_kind, start_load, special_methods));
            range_ctor_call->arguments().add(make_index(end_kind, end_load, special_methods));
            return range_ctor_call;
        }
    };

    auto extend_slicing = [&]() {
        // We might be dealing with a situation where we executed TransformSlicing() in a previous run of this transform that only looked
        // at a part of the instructions making up the slicing pattern. The first run would have mis-detected slicing from end as slicing
        // from start. This results in code like:
        //   int length = span.Length;
        //   Console.WriteLine(span[GetIndex(1).GetOffset(length)..GetIndex(2).GetOffset(length)].ToString());
        // or:
        //   int length = span.Length;
        //   Range range = GetRange();
        //   Console.WriteLine(span[range.Start.GetOffset(length)..range.End.GetOffset(length)].ToString());
        if (container_length_var == nullptr) {
            return;  // need a container length to extend with
        }
        assert(container_length_var->is_single_definition());
        assert(container_length_var->load_count() == 1 || container_length_var->load_count() == 2);
        NewObj* range_ctor_call = nullptr;
        for (ILInstruction* inst : container_length_var->load_instructions()[0]->ancestors()) {
            auto* newobj = as<NewObj>(inst);
            if (newobj != nullptr && IndexMethods::is_range_ctor(newobj->method)) {
                range_ctor_call = newobj;
                break;
            }
            if (inst == block) break;
        }
        if (range_ctor_call == nullptr) return;
        // Now match the pattern that TransformSlicing() generated in the IndexKind.FromStart case
        auto* slicing_call = as<CallInstruction>(range_ctor_call->parent());
        if (!(slicing_call != nullptr && is_synthetic_range_index_accessor(slicing_call->method))) return;
        if (!match_container_var(slicing_call->arguments()[0], &container_var)) return;
        if (!slicing_call->is_descendant_of(block->instructions[pos])) return;
        assert(range_ctor_call->arguments().count() == 2);
        ILInstruction* start_offset_inst = nullptr;
        if (!match_index_implicit_conv(range_ctor_call->arguments()[0], &start_offset_inst)) return;
        ILInstruction* end_offset_inst = nullptr;
        if (!match_index_implicit_conv(range_ctor_call->arguments()[1], &end_offset_inst)) return;
        ILInstruction* ext_start_index_load = nullptr;
        IndexKind ext_start_index_kind = match_get_offset(start_offset_inst, &ext_start_index_load, container_length_var, &container_var);
        ILInstruction* ext_end_index_load = nullptr;
        IndexKind ext_end_index_kind = match_get_offset(end_offset_inst, &ext_end_index_load, container_length_var, &container_var);
        if (!check_container_length_variable_use_count(container_length_var, ext_start_index_kind, ext_end_index_kind)) {
            return;
        }
        // holds because we've used containerLengthVar at least once
        assert(ext_start_index_kind != IndexKind::FromStart || ext_end_index_kind != IndexKind::FromStart);
        if (range_var != nullptr) {
            if (!ILInlining::can_move_into(range_var_init, block->instructions[pos], ext_start_index_load)) return;
            if (!match_index_from_range(ext_start_index_kind, ext_start_index_load, range_var, "get_Start")) return;
            if (!match_index_from_range(ext_end_index_kind, ext_end_index_load, range_var, "get_End")) return;
        }
        IndexMethods special_methods(context.type_system());
        if (!special_methods.is_valid()) return;
        context.step("Merge containerLengthVar into slicing", slicing_call);
        range_ctor_call->replace_with(make_range(ext_start_index_kind, ext_start_index_load, ext_end_index_kind, ext_end_index_load, special_methods));
        for (int i = start_pos; i < pos; i++) {
            slicing_call->add_il_range(*block->instructions[i]);
        }
        block->instructions.remove_range(start_pos, pos - start_pos);
    };

    if (!(block->instructions[pos]->match_st_loc(&start_offset_var, &start_offset_var_init) && start_offset_var->is_single_definition() &&
          start_offset_var->stack_type == StackType::I4)) {
        // Not our primary indexing/slicing pattern.
        // However, we might be dealing with a partially-transformed pattern that needs to be extended.
        extend_slicing();
        return;
    }
    start_index_kind = match_get_offset(start_offset_var_init, &start_index_load, container_length_var, &container_var);
    pos++;

    auto transform_slicing = [&](bool slice_length_was_misdetected_as_start_offset) {
        ILVariable* slice_length_var = nullptr;
        ILInstruction* slice_length_var_init = nullptr;
        if (slice_length_was_misdetected_as_start_offset) {
            // Special case: when slicing without a start point, the slice length calculation is mis-detected as the start offset, and
            // since it only has a single use, we end in TransformIndexing(), which then calls TransformSlicing on this code path.
            slice_length_var = start_offset_var;
            slice_length_var_init = as<StLoc>(slice_length_var->store_instructions()[0]->as_instruction())->value();
            start_offset_var = nullptr;
            start_index_load = make<LdcI4>(0);
            start_index_kind = IndexKind::TheStart;
        } else {
            // stloc containerLengthVar(call get_Length(ldloc containerVar))
            // stloc startOffset(call GetOffset(startIndexLoad, ldloc length))
            // -- we are here --
            // stloc sliceLengthVar(binary.sub.i4(call GetOffset(endIndexLoad, ldloc length), ldloc startOffset))
            // complex_expr(call Slice(ldloc containerVar, ldloc startOffset, ldloc sliceLength))

            if (!block->instructions[pos]->match_st_loc(&slice_length_var, &slice_length_var_init)) return;
            pos++;
        }

        if (!(slice_length_var->is_single_definition() && slice_length_var->load_count() == 1)) return;
        IndexKind end_index_kind = IndexKind::FromStart;
        ILInstruction* end_index_load = nullptr;
        if (!match_slice_length(slice_length_var_init, &end_index_kind, &end_index_load, container_length_var, &container_var, start_offset_var)) return;
        if (!check_container_length_variable_use_count(container_length_var, start_index_kind, end_index_kind)) {
            return;
        }
        if (range_var != nullptr) {
            return;  // this should only ever happen in the second step (ExtendSlicing)
        }
        auto* call = as<CallInstruction>(slice_length_var->load_instructions()[0]->parent());
        if (call == nullptr) return;
        if (!call->is_descendant_of(block->instructions[pos])) return;
        if (!is_slicing_method(call->method)) return;
        if (call->arguments().count() != 3) return;
        if (!match_container_var(call->arguments()[0], &container_var)) return;
        if (start_offset_var == nullptr) {
            assert(start_index_kind == IndexKind::TheStart);
            if (!call->arguments()[1]->match_ldc_i4(0)) return;
        } else {
            if (!call->arguments()[1]->match_ld_loc(start_offset_var)) return;
            if (!ILInlining::can_move_into(start_offset_var_init, block->instructions[pos], call->arguments()[1])) return;
        }
        if (!call->arguments()[2]->match_ld_loc(slice_length_var)) return;
        if (!ILInlining::can_move_into(slice_length_var_init, block->instructions[pos], call->arguments()[2])) return;
        if (!csharp_will_generate_indexer(call->method->declaring_type(), true)) return;
        IndexMethods special_methods(context.type_system());
        if (!special_methods.is_valid()) return;

        context.step(call->method->name() + " sliced with range", call);
        const IMethod* new_method = make_synthetic_range_index_accessor(call->method, special_methods.range_type(), true);
        if (new_method == nullptr) return;
        auto* new_call = CallInstruction::create(call->op_code, new_method);
        new_call->constrained_to = call->constrained_to;
        new_call->il_stack_was_empty = call->il_stack_was_empty;
        new_call->arguments().add(call->arguments()[0]);
        new_call->arguments().add(make_range(start_index_kind, start_index_load, end_index_kind, end_index_load, special_methods));
        new_call->add_il_range(*call);
        for (int i = start_pos; i < pos; i++) {
            new_call->add_il_range(*block->instructions[i]);
        }
        call->replace_with(new_call);
        context.end_step(new_call);
        block->instructions.remove_range(start_pos, pos - start_pos);
    };

    auto transform_indexing = [&]() {
        // complex_expr(call get_Item(ldloc container, ldloc startOffsetVar))

        if (range_var != nullptr) return;
        auto* call = as<CallInstruction>(start_offset_var->load_instructions()[0]->parent());
        if (call == nullptr) return;
        if (call->method->accessor_kind() == MethodSemanticsAttributes::Getter && call->arguments().count() == 2) {
            const typesystem::IMember* owner = call->method->accessor_owner();
            if (owner == nullptr || owner->symbol_kind() != SymbolKind::Indexer) return;
            if (call->method->parameters().size() != 1) return;
        } else if (call->method->accessor_kind() == MethodSemanticsAttributes::Setter && call->arguments().count() == 3) {
            const typesystem::IMember* owner = call->method->accessor_owner();
            if (owner == nullptr || owner->symbol_kind() != SymbolKind::Indexer) return;
            if (call->method->parameters().size() != 2) return;
        } else if (is_slicing_method(call->method)) {
            transform_slicing(true);
            return;
        } else {
            return;
        }
        if (start_index_kind == IndexKind::FromStart) {
            // FromStart is only relevant for slicing; indexing from the start does not involve System.Index at all.
            return;
        }
        if (!check_container_length_variable_use_count(container_length_var, start_index_kind)) {
            return;
        }
        if (!call->is_descendant_of(block->instructions[pos])) return;
        // startOffsetVar might be used deep inside a complex statement, ensure we can inline up to that point:
        for (int i = start_pos; i < pos; i++) {
            if (!ILInlining::can_inline_into(block->instructions[pos], start_offset_var, block->instructions[i])) return;
        }
        if (!typesystem::is_known_type(call->method->parameters()[0]->type(), KnownTypeCode::Int32)) return;
        if (!match_container_var(call->arguments()[0], &container_var)) return;
        if (!call->arguments()[1]->match_ld_loc(start_offset_var)) return;
        IndexMethods special_methods(context.type_system());
        if (!special_methods.is_valid()) return;
        if (!csharp_will_generate_indexer(call->method->declaring_type(), false)) return;

        context.step(call->method->name() + " indexed", call);
        const IMethod* new_method = make_synthetic_range_index_accessor(call->method, special_methods.index_type(), false);
        if (new_method == nullptr) return;
        auto* new_call = CallInstruction::create(call->op_code, new_method);
        new_call->constrained_to = call->constrained_to;
        new_call->il_stack_was_empty = call->il_stack_was_empty;
        new_call->arguments().add(call->arguments()[0]);
        new_call->arguments().add(make_index(start_index_kind, start_index_load, special_methods));
        std::vector<ILInstruction*> remaining_arguments;
        for (int i = 2; i < call->arguments().count(); i++) remaining_arguments.push_back(call->arguments()[i]);
        new_call->arguments().add_range(remaining_arguments);
        new_call->add_il_range(*call);
        for (int i = start_pos; i < pos; i++) {
            new_call->add_il_range(*block->instructions[i]);
        }
        call->replace_with(new_call);
        context.end_step(new_call);
        block->instructions.remove_range(start_pos, pos - start_pos);
    };

    if (start_offset_var->load_count() == 1) {
        transform_indexing();
    } else if (start_offset_var->load_count() == 2) {
        // might be slicing: startOffset is used once for the slice length calculation, and once for the Slice() method call
        transform_slicing(false);
    }
}

bool IndexRangeTransform::match_index_implicit_conv(ILInstruction* inst, ILInstruction** offset_inst) {
    *offset_inst = nullptr;
    auto* call = as<CallInstruction>(inst);
    if (call == nullptr) return false;
    if (!(call->method->is_operator() && call->method->name() == "op_Implicit")) return false;
    const IMethod* op = call->method;
    if (!(op->parameters().size() == 1 && typesystem::is_known_type(op->parameters()[0]->type(), KnownTypeCode::Int32))) return false;
    if (!typesystem::is_known_type(op->declaring_type(), KnownTypeCode::Index)) return false;
    // (Single() throws unless there is exactly one argument)
    if (call->arguments().count() != 1) throw std::invalid_argument("the call does not have exactly one argument");
    *offset_inst = call->arguments()[0];
    return true;
}

bool IndexRangeTransform::is_slicing_method(const IMethod* method) {
    if (method->is_extension_method()) return false;
    if (method->parameters().size() != 2) return false;
    for (const typesystem::IParameter* p : method->parameters()) {
        if (!typesystem::is_known_type(p->type(), KnownTypeCode::Int32)) return false;
    }
    return method->name() == "Slice" || (method->name() == "Substring" && typesystem::is_known_type(method->declaring_type(), KnownTypeCode::String));
}

bool IndexRangeTransform::check_container_length_variable_use_count(ILVariable* container_length_var, IndexKind start_index_kind,
                                                                    IndexKind end_index_kind) {
    int expected_uses = 0;
    if (start_index_kind != IndexKind::FromStart && start_index_kind != IndexKind::TheStart) expected_uses += 1;
    if (end_index_kind != IndexKind::FromStart && end_index_kind != IndexKind::TheStart) expected_uses += 1;
    if (container_length_var != nullptr) {
        return container_length_var->load_count() == expected_uses;
    } else {
        return expected_uses <= 1;  // can have one inline use
    }
}

bool IndexRangeTransform::match_index_from_range(IndexKind index_kind, ILInstruction* index_load, ILVariable* range_var, const char* accessor_name) {
    if (index_kind != IndexKind::RefSystemIndex) return false;
    auto* address_of = as<AddressOf>(index_load);
    if (address_of == nullptr) return false;
    if (!typesystem::is_known_type(address_of->type(), KnownTypeCode::Index)) return false;
    auto* call = as<Call>(address_of->value());
    if (call == nullptr) return false;
    if (call->method->name() != accessor_name) return false;
    if (!typesystem::is_known_type(call->method->declaring_type(), KnownTypeCode::Range)) return false;
    if (call->arguments().count() != 1) return false;
    return call->arguments()[0]->match_ld_loca(range_var);
}

ILInstruction* IndexRangeTransform::make_index(IndexKind index_kind, ILInstruction* index_load, const IndexMethods& special_methods) {
    if (index_kind == IndexKind::RefSystemIndex) {
        //  stloc containerLengthVar(call get_Length/get_Count(ldloc container))
        //  stloc startOffsetVar(call GetOffset(startIndexLoad, ldloc length))
        //  complex_expr(call get_Item(ldloc container, ldloc startOffsetVar))
        // -->
        //  complex_expr(call get_Item(ldloc container, ldobj startIndexLoad))
        return make<LdObj>(index_load, special_methods.index_type());
    } else if (index_kind == IndexKind::FromEnd || index_kind == IndexKind::TheEnd) {
        //  stloc offsetVar(binary.sub.i4(call get_Length/get_Count(ldloc container), startIndexLoad))
        //  complex_expr(call get_Item(ldloc container, ldloc startOffsetVar))
        // -->
        //  complex_expr(call get_Item(ldloc container, newobj System.Index(startIndexLoad, fromEnd: true)))
        auto* new_obj = make<NewObj>(special_methods.index_ctor);
        new_obj->arguments().add(index_load);
        new_obj->arguments().add(make<LdcI4>(1));
        return new_obj;
    } else {
        assert(index_kind == IndexKind::FromStart || index_kind == IndexKind::TheStart);
        auto* call = make<Call>(special_methods.index_implicit_conv);
        call->arguments().add(index_load);
        return call;
    }
}

bool IndexRangeTransform::csharp_will_generate_indexer(const IType* declaring_type, bool slicing) {
    bool found_int32_overload = false;
    bool found_index_overload = false;
    bool found_range_overload = false;
    bool found_count_property = false;
    for (const typesystem::IProperty* prop : declaring_type->get_properties([](const typesystem::IProperty* p) {
             return p->is_indexer() || (p->name() == "Length" || p->name() == "Count");
         })) {
        if (prop->is_indexer() && prop->parameters().size() == 1) {
            const typesystem::IParameter* p = prop->parameters()[0];
            if (typesystem::is_known_type(p->type(), KnownTypeCode::Int32)) {
                found_int32_overload = true;
            } else if (typesystem::is_known_type(p->type(), KnownTypeCode::Index)) {
                found_index_overload = true;
            } else if (typesystem::is_known_type(p->type(), KnownTypeCode::Range)) {
                found_range_overload = true;
            }
        } else if (prop->name() == "Length" || prop->name() == "Count") {
            found_count_property = true;
        }
    }
    if (slicing) {
        return /* foundSlicingMethod && */ found_count_property && !found_range_overload;
    } else {
        return found_int32_overload && found_count_property && !found_index_overload;
    }
}

bool IndexRangeTransform::match_container_length_store(ILInstruction* inst, ILVariable** length_var, ILVariable** container_var) {
    ILInstruction* init = nullptr;
    if (!inst->match_st_loc(length_var, &init)) return false;
    if (!((*length_var)->is_single_definition() && (*length_var)->stack_type == StackType::I4)) return false;
    if ((*length_var)->load_count() == 0 || (*length_var)->load_count() > 2) return false;
    return match_container_length(init, nullptr, container_var);
}

bool IndexRangeTransform::match_container_length(ILInstruction* init, ILVariable* length_var, ILVariable** container_var) {
    if (length_var != nullptr) {
        assert(*container_var != nullptr);
        return init->match_ld_loc(length_var);
    }
    auto* call = as<CallInstruction>(init);
    if (call == nullptr) return false;
    if (call->result_type() != StackType::I4) return false;
    if (!(call->method->is_accessor() && call->method->accessor_kind() == MethodSemanticsAttributes::Getter)) return false;
    const auto* length_prop = dynamic_cast<const typesystem::IProperty*>(call->method->accessor_owner());
    if (length_prop == nullptr) return false;
    if (length_prop->name() == "Length") {
        // OK, Length is preferred
    } else if (length_prop->name() == "Count") {
        // Also works, but only if the type doesn't have "Length"
        if (!length_prop->declaring_type()->get_properties([](const typesystem::IProperty* p) { return p->name() == "Length"; }).empty()) return false;
    }
    if (!typesystem::is_known_type(length_prop->return_type(), KnownTypeCode::Int32)) return false;
    if (length_prop->is_virtual() && call->op_code != OpCode::CallVirt) return false;
    if (call->arguments().count() != 1) return false;
    return match_container_var(call->arguments()[0], container_var);
}

bool IndexRangeTransform::match_container_var(ILInstruction* inst, ILVariable** container_var) {
    if (*container_var != nullptr) {
        return inst->match_ld_loc(*container_var) || inst->match_ld_loca(*container_var);
    } else {
        return inst->match_ld_loc(container_var) || inst->match_ld_loca(container_var);
    }
}

IndexRangeTransform::IndexKind IndexRangeTransform::match_get_offset(ILInstruction* inst, ILInstruction** index_load,
                                                                     ILVariable* container_length_var, ILVariable** container_var) {
    *index_load = inst;
    if (match_container_length(inst, container_length_var, container_var)) {
        *index_load = make<LdcI4>(0);
        return IndexKind::TheEnd;
    } else if (auto* call = as<CallInstruction>(inst)) {
        // call System.Index.GetOffset(indexLoad, ldloc containerLengthVar)
        if (call->method->name() != "GetOffset") return IndexKind::FromStart;
        if (!typesystem::is_known_type(call->method->declaring_type(), KnownTypeCode::Index)) return IndexKind::FromStart;
        if (call->arguments().count() != 2) return IndexKind::FromStart;
        if (!match_container_length(call->arguments()[1], container_length_var, container_var)) return IndexKind::FromStart;
        *index_load = call->arguments()[0];
        return IndexKind::RefSystemIndex;
    } else if (auto* bni = as<BinaryNumericInstruction>(inst); bni != nullptr && bni->op == BinaryNumericOperator::Sub) {
        if (bni->check_for_overflow || bni->result_type() != StackType::I4 || bni->is_lifted()) return IndexKind::FromStart;
        // binary.sub.i4(ldloc containerLengthVar, indexLoad)
        if (!match_container_length(bni->left(), container_length_var, container_var)) return IndexKind::FromStart;
        *index_load = bni->right();
        return IndexKind::FromEnd;
    } else {
        return IndexKind::FromStart;
    }
}

bool IndexRangeTransform::match_slice_length(ILInstruction* inst, IndexKind* end_index_kind, ILInstruction** end_index_load,
                                             ILVariable* container_length_var, ILVariable** container_var, ILVariable* start_offset_var) {
    *end_index_kind = IndexKind::FromStart;
    *end_index_load = nullptr;
    auto* bni = as<BinaryNumericInstruction>(inst);
    if (bni != nullptr && bni->op == BinaryNumericOperator::Sub) {
        if (bni->check_for_overflow || bni->result_type() != StackType::I4 || bni->is_lifted()) return false;
        if (start_offset_var == nullptr) {
            // When slicing without explicit start point: `a[..endIndex]`
            if (!bni->right()->match_ldc_i4(0)) return false;
        } else {
            if (!bni->right()->match_ld_loc(start_offset_var)) return false;
        }
        *end_index_kind = match_get_offset(bni->left(), end_index_load, container_length_var, container_var);
        return true;
    } else if (start_offset_var == nullptr) {
        // When slicing without explicit start point: `a[..endIndex]`, the compiler doesn't always emit the "- 0".
        *end_index_kind = match_get_offset(inst, end_index_load, container_length_var, container_var);
        return true;
    } else {
        return false;
    }
}

}  // namespace cb::dotnet::decompiler::il::transforms
