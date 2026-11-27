// dotnet/decompiler/il/transforms/transform_array_initializers.cpp -- see transform_array_initializers.hpp.
#include "dotnet/decompiler/il/transforms/transform_array_initializers.hpp"

#include <algorithm>
#include <cstdint>
#include <format>
#include <functional>
#include <limits>
#include <stdexcept>

#include "core/error.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/il/pointer_arithmetic_offset.hpp"
#include "dotnet/decompiler/il/transforms/expression_transforms.hpp"
#include "dotnet/decompiler/il/transforms/il_inlining.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/known_type_reference.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::il::transforms {

using typesystem::ITypeDefinition;
using typesystem::KnownTypeCode;
using typesystem::TypeKind;

namespace {

constexpr std::uint16_t kHasFieldRva = 0x0100;

// System.TypeCode as ReflectionHelper.GetTypeCode answers: the known type code of a definition for the primitives and string (the two
// enums agree on those), else Empty.
enum class TypeCode : std::uint8_t {
    Empty = 0, Object = 1, DBNull = 2, Boolean = 3, Char = 4, SByte = 5, Byte = 6, Int16 = 7, UInt16 = 8, Int32 = 9, UInt32 = 10, Int64 = 11,
    UInt64 = 12, Single = 13, Double = 14, Decimal = 15, DateTime = 16, String = 18,
};

TypeCode get_type_code(const IType* type) {
    auto* def = dynamic_cast<const ITypeDefinition*>(type);
    if (def == nullptr) return TypeCode::Empty;
    KnownTypeCode code = def->known_type_code();
    if (code <= KnownTypeCode::String && code != KnownTypeCode::Void) return static_cast<TypeCode>(static_cast<int>(code));
    return TypeCode::Empty;
}

bool has_field_rva(const metadata::MetadataFile& file, srm::FieldDefinitionHandle field) {
    return (file.metadata().get_field_definition(field).attributes() & kHasFieldRva) != 0;
}

srm::BlobReader get_initial_value_reader(const metadata::MetadataFile& file, srm::FieldDefinitionHandle field) {
    return srm::BlobReader(get_initial_value(file.metadata().get_field_definition(field), file));
}

// ((PointerType)type).ElementType
const IType* pointer_element_type(const IType* type) {
    auto* pointer = dynamic_cast<const typesystem::PointerType*>(type);
    if (pointer == nullptr) throw UsageError("Unable to cast object to type PointerType");
    return pointer->element_type();
}

bool is_runtime_helpers(const IType* type) {
    return type->name() == "RuntimeHelpers" && type->namespace_() == "System.Runtime.CompilerServices" && type->type_parameter_count() == 0;
}

// Whether `inst` or a node below it reads, writes or takes the address of `variable`.
bool uses_variable(ILInstruction* inst, ILVariable* variable) {
    for (ILInstruction* descendant : inst->descendants()) {
        if (auto* with_variable = dynamic_cast<IInstructionWithVariableOperand*>(descendant); with_variable != nullptr && with_variable->variable() == variable)
            return true;
    }
    return false;
}

// Encoding.UTF8: whether the bytes are well-formed UTF-8 (so that decoding and encoding again gives the same bytes).
bool is_well_formed_utf8(const std::uint8_t* data, int size) {
    int i = 0;
    while (i < size) {
        std::uint8_t b = data[i];
        int length;
        std::uint32_t code;
        std::uint32_t minimum;
        if (b < 0x80) {
            i++;
            continue;
        } else if ((b & 0xE0) == 0xC0) {
            length = 2;
            code = b & 0x1Fu;
            minimum = 0x80;
        } else if ((b & 0xF0) == 0xE0) {
            length = 3;
            code = b & 0x0Fu;
            minimum = 0x800;
        } else if ((b & 0xF8) == 0xF0) {
            length = 4;
            code = b & 0x07u;
            minimum = 0x10000;
        } else {
            return false;
        }
        if (i + length > size) return false;
        for (int k = 1; k < length; k++) {
            if ((data[i + k] & 0xC0) != 0x80) return false;
            code = (code << 6) | (data[i + k] & 0x3Fu);
        }
        if (code < minimum || code > 0x10FFFF || (code >= 0xD800 && code <= 0xDFFF)) return false;
        i += length;
    }
    return true;
}

bool decode_utf8_string(const srm::BlobReader& blob, int size, std::string* text) {
    if (size > blob.remaining_bytes()) {
        text->clear();
        return false;
    }
    const std::uint8_t* current = blob.bytes().data() + blob.offset();
    for (int i = 0; i < size; i++) {
        std::uint8_t val = current[i];
        if (val == 0 && i == size - 1 && size > 1) {
            // Allow explicit null-termination character.
        } else if (val < 0x20 && !(val == '\r' || val == '\n' || val == '\t')) {
            // If the string has control characters, it's probably binary data and not a string.
            text->clear();
            return false;
        }
    }
    // Only use UTF8 string literal if we can perfectly roundtrip the data
    if (!is_well_formed_utf8(current, size)) {
        text->clear();
        return false;
    }
    text->assign(reinterpret_cast<const char*>(current), static_cast<std::size_t>(size));
    return true;
}

int element_size_of(TypeCode element_type) {
    switch (element_type) {
        case TypeCode::Boolean:
        case TypeCode::Byte:
        case TypeCode::SByte: return 1;
        case TypeCode::Char:
        case TypeCode::Int16:
        case TypeCode::UInt16: return 2;
        case TypeCode::Int32:
        case TypeCode::UInt32:
        case TypeCode::Single: return 4;
        case TypeCode::Int64:
        case TypeCode::UInt64:
        case TypeCode::Double: return 8;
        default: throw std::out_of_range("elementType");
    }
}

using ValueDecoder = std::function<ILInstruction*(srm::BlobReader&)>;

bool decode_array_initializer(srm::BlobReader initial_value, const std::vector<int>& array_length, std::vector<ILInstruction*>* output,
                              TypeCode element_type, const ValueDecoder& decoder) {
    int element_size = element_size_of(element_type);
    int total_length = 1;
    for (int l : array_length) total_length *= l;
    if (initial_value.remaining_bytes() < (total_length * element_size)) return false;

    output->reserve(output->size() + static_cast<std::size_t>(total_length) + static_cast<std::size_t>(total_length) * array_length.size());

    for (int i = 0; i < total_length; i++) {
        output->push_back(decoder(initial_value));
        int next = i;
        for (int j = static_cast<int>(array_length.size()) - 1; j >= 0; j--) {
            output->push_back(make<LdcI4>(next % array_length[static_cast<std::size_t>(j)]));
            next /= array_length[static_cast<std::size_t>(j)];
        }
    }
    return true;
}

bool decode_array_initializer(const IType* type, const srm::BlobReader& initial_value, const std::vector<int>& array_length,
                              std::vector<ILInstruction*>* output) {
    TypeCode type_code = get_type_code(type);
    switch (type_code) {
        case TypeCode::Boolean:
        case TypeCode::Byte:
            return decode_array_initializer(initial_value, array_length, output, type_code,
                                            [](srm::BlobReader& r) -> ILInstruction* { return make<LdcI4>(r.read_byte()); });
        case TypeCode::SByte:
            return decode_array_initializer(initial_value, array_length, output, type_code,
                                            [](srm::BlobReader& r) -> ILInstruction* { return make<LdcI4>(r.read_sbyte()); });
        case TypeCode::Int16:
            return decode_array_initializer(initial_value, array_length, output, type_code,
                                            [](srm::BlobReader& r) -> ILInstruction* { return make<LdcI4>(r.read_int16()); });
        case TypeCode::Char:
        case TypeCode::UInt16:
            return decode_array_initializer(initial_value, array_length, output, type_code,
                                            [](srm::BlobReader& r) -> ILInstruction* { return make<LdcI4>(r.read_uint16()); });
        case TypeCode::Int32:
        case TypeCode::UInt32:
            return decode_array_initializer(initial_value, array_length, output, type_code,
                                            [](srm::BlobReader& r) -> ILInstruction* { return make<LdcI4>(r.read_int32()); });
        case TypeCode::Int64:
        case TypeCode::UInt64:
            return decode_array_initializer(initial_value, array_length, output, type_code,
                                            [](srm::BlobReader& r) -> ILInstruction* { return make<LdcI8>(r.read_int64()); });
        case TypeCode::Single:
            return decode_array_initializer(initial_value, array_length, output, type_code,
                                            [](srm::BlobReader& r) -> ILInstruction* { return make<LdcF4>(r.read_single()); });
        case TypeCode::Double:
            return decode_array_initializer(initial_value, array_length, output, type_code,
                                            [](srm::BlobReader& r) -> ILInstruction* { return make<LdcF8>(r.read_double()); });
        case TypeCode::Object:
        case TypeCode::Empty: {
            const ITypeDefinition* type_def = type->get_definition();
            if (type_def != nullptr && type_def->kind() == TypeKind::Enum)
                return decode_array_initializer(type_def->enum_underlying_type(), initial_value, array_length, output);
            return false;
        }
        default: return false;
    }
}

ILInstruction* st_elem(ILInstruction* array, const std::vector<ILInstruction*>& indices, ILInstruction* value, const IType* type) {
    if (get_stack_type(*type) != value->result_type()) {
        value = make<Conv>(value, to_primitive_type(*type), false, Sign::None);
    }
    auto* ld_elema = make<LdElema>(type, array, indices);
    ld_elema->delay_exceptions = true;
    return make<StObj>(ld_elema, value, type);
}

Block* block_from_initializer(ILVariable* v, const IType* element_type, const std::vector<int>& array_length, const std::vector<ILInstruction*>& values) {
    auto* block = make<Block>(BlockKind::ArrayInitializer);
    std::vector<ILInstruction*> lengths;
    for (int l : array_length) lengths.push_back(make<LdcI4>(l));
    block->instructions.add(make<StLoc>(v, make<NewArr>(element_type, lengths)));
    std::size_t step = array_length.size() + 1;

    std::vector<ILInstruction*> indices;
    for (std::size_t i = 0; i < values.size() / step; i++) {
        // values array is filled backwards
        ILInstruction* value = values[step * i];

        for (std::size_t j = step - 1; j >= 1; j--) {
            indices.push_back(values[step * i + j]);
        }

        block->instructions.add(st_elem(make<LdLoc>(v), indices, value, element_type));
        indices.clear();
    }
    block->set_final_instruction(make<LdLoc>(v));
    return block;
}

Block* build_simple_array_initializer_block(ILVariable* v, const IType* element_type, const std::vector<int>& array_length,
                                            const TransformArrayInitializers::ArrayInitializerValues& values) {
    auto* block = make<Block>(BlockKind::ArrayInitializer);
    std::vector<ILInstruction*> lengths;
    for (int l : array_length) lengths.push_back(make<LdcI4>(l));
    block->instructions.add(make<StLoc>(v, make<NewArr>(element_type, lengths)));
    for (const auto& entry : values) {
        ILInstruction* value = entry.second != nullptr ? entry.second : TransformArrayInitializers::get_null_expression(element_type);
        block->instructions.add(st_elem(make<LdLoc>(v), entry.first, value, element_type));
    }
    block->set_final_instruction(make<LdLoc>(v));
    return block;
}

bool match_new_arr(ILInstruction* instruction, const IType** array_type, std::vector<int>* length) {
    length->clear();
    *array_type = nullptr;
    auto* new_arr = as<NewArr>(instruction);
    if (new_arr == nullptr) return false;
    *array_type = new_arr->type();
    auto& args = new_arr->indices();
    for (int i = 0; i < args.count(); i++) {
        int value = 0;
        if (!args[i]->match_ldc_i4(&value) || value <= 0) {
            return false;
        }
        length->push_back(value);
    }
    return true;
}

ILInstruction* decode_array_initializer_or_utf8_string_literal(StatementTransformContext& context, const IType* element_type,
                                                               const srm::BlobReader& initial_value, int size) {
    std::string text;
    if (context.settings().utf8_string_literals && typesystem::is_known_type(element_type, KnownTypeCode::Byte) &&
        decode_utf8_string(initial_value, size, &text)) {
        return make<LdStrUtf8>(text);
    }
    std::vector<ILInstruction*> values_list;
    if (decode_array_initializer(element_type, initial_value, {size}, &values_list)) {
        auto* temp_store = context.function()->register_variable(VariableKind::InitializerTarget,
                                                                 typesystem::make_array_type(context.type_system(), element_type));
        return block_from_initializer(temp_store, element_type, {size}, values_list);
    }

    return nullptr;
}

bool match_get_static_field_address(ILInstruction* input, const IField** field) {
    if (input->match_lds_flda(field)) return true;
    // call get_Item(addressof System.ReadOnlySpan`1[[T]](call CreateSpan(ldmembertoken field)), ldc.i4 0)
    auto* call = as<Call>(input);
    if (call == nullptr || call->method->name() != "get_Item" || call->arguments().count() != 2) return false;
    if (!typesystem::is_known_type(call->method->declaring_type(), KnownTypeCode::ReadOnlySpanOfT)) return false;
    if (!call->arguments()[1]->match_ldc_i4(0)) return false;
    auto* address_of = as<AddressOf>(call->arguments()[0]);
    if (address_of == nullptr) return false;
    auto* create_span_call = as<Call>(address_of->value());
    if (create_span_call == nullptr || create_span_call->method->name() != "CreateSpan" || create_span_call->arguments().count() != 1) return false;
    if (!is_runtime_helpers(create_span_call->method->declaring_type())) return false;
    const IMember* member = nullptr;
    if (!create_span_call->arguments()[0]->match_ld_member_token(&member)) return false;
    *field = dynamic_cast<const IField*>(member);
    return *field != nullptr;
}

bool is_sub_pattern_of_cpblk_initializer(Call* inst) {
    auto* address_of = as<AddressOf>(inst->parent());
    if (address_of == nullptr) return false;
    auto* get_item = as<Call>(address_of->parent());
    if (get_item == nullptr) return false;
    if (as<Cpblk>(get_item->parent()) == nullptr) return false;
    const IField* unused = nullptr;
    return match_get_static_field_address(get_item, &unused);
}

bool match_span_t_ctor_with_pointer_and_size(NewObj* new_obj, StatementTransformContext& context, const IType** element_type,
                                             srm::FieldDefinitionHandle* field, int* size) {
    *field = srm::FieldDefinitionHandle();
    *size = 0;
    *element_type = nullptr;
    const IType* type = new_obj->method->declaring_type();
    if (!typesystem::is_known_type(type, KnownTypeCode::ReadOnlySpanOfT)) return false;
    if (new_obj->arguments().count() != 2 || type->type_arguments().size() != 1) return false;
    *element_type = type->type_arguments()[0];
    const IField* member = nullptr;
    if (!new_obj->arguments()[0]->unwrap_conv(ConversionKind::StopGCTracking)->match_lds_flda(&member)) return false;
    if (member->metadata_token().is_nil()) return false;
    if (!new_obj->arguments()[1]->match_ldc_i4(size)) return false;
    *field = srm::FieldDefinitionHandle(member->metadata_token());
    (void)context;
    return true;
}

bool match_runtime_helpers_create_span(Call* inst, StatementTransformContext& context, const IType** element_type, srm::FieldDefinitionHandle* field) {
    *field = srm::FieldDefinitionHandle();
    *element_type = nullptr;
    if (!is_runtime_helpers(inst->method->declaring_type())) return false;
    if (inst->arguments().count() != 1) return false;
    if (!(inst->method->name() == "CreateSpan" && inst->method->type_arguments().size() == 1)) return false;
    *element_type = inst->method->type_arguments()[0];
    const IMember* member = nullptr;
    if (!inst->arguments()[0]->unwrap_conv(ConversionKind::StopGCTracking)->match_ld_member_token(&member)) return false;
    if (member->metadata_token().is_nil()) return false;
    *field = srm::FieldDefinitionHandle(member->metadata_token());
    (void)context;
    return true;
}

enum class SequentialStoreMatch : std::uint8_t {
    // The store belongs to the initializer sequence.
    Matched,
    // The store is not part of the sequence; it ends the scan, keeping what was matched.
    SequenceEnd,
    // The store's shape is unexpected; reject the whole transform.
    Abort,
};

SequentialStoreMatch get_sequential_store_offset(ILInstruction* target, ILVariable* store, const IType* element_type, long long* min_expected_offset) {
    // stobj T(ldloc store, value) writes at the current expected offset, initially 0.
    if (target->match_ld_loc(static_cast<const ILVariable*>(store))) return SequentialStoreMatch::Matched;

    // stobj T(binary.add(ldloc store, offset), value)
    // The offset is either sizeof(T) or an element index multiplied by sizeof(T).
    ILInstruction* left = nullptr;
    ILInstruction* right = nullptr;
    if (!target->match_binary_numeric_instruction(BinaryNumericOperator::Add, &left, &right)) return SequentialStoreMatch::Abort;
    if (!left->match_ld_loc(static_cast<const ILVariable*>(store))) return SequentialStoreMatch::SequenceEnd;
    ILInstruction* offset_inst =
        PointerArithmeticOffset::detect(right, element_type, static_cast<BinaryNumericInstruction*>(target)->check_for_overflow);
    if (offset_inst == nullptr) return SequentialStoreMatch::Abort;
    std::int64_t offset = 0;
    if (!offset_inst->match_ldc_i(&offset) || offset < 0 || offset < *min_expected_offset) return SequentialStoreMatch::SequenceEnd;
    *min_expected_offset = offset;
    return SequentialStoreMatch::Matched;
}

}  // namespace

void TransformArrayInitializers::run(Block* block, int pos, StatementTransformContext& context) {
    if (!context.settings().array_initializers) return;
    context_ = &context;
    struct Reset {
        TransformArrayInitializers* self;
        ~Reset() { self->context_ = nullptr; }
    } reset{this};
    if (do_transform(context.function(), block, pos)) return;
    if (do_transform_multi_dim(context.function(), block, pos)) return;
    if (context.settings().stack_alloc_initializers && do_transform_stack_alloc_initializer(block, pos)) return;
    if (do_transform_inline_runtime_helpers_initialize_array(block, pos)) return;
}

bool TransformArrayInitializers::do_transform(ILFunction* function, Block* body, int pos) {
    if (pos >= body->instructions.count() - 2) return false;
    ILInstruction* inst = body->instructions[pos];
    ILVariable* v = nullptr;
    ILInstruction* newarr_expr = nullptr;
    const IType* element_type = nullptr;
    std::vector<int> array_length;
    if (inst->match_st_loc(&v, &newarr_expr) && match_new_arr(newarr_expr, &element_type, &array_length)) {
        std::vector<ILInstruction*> values;
        int init_array_pos = 0;
        if (handle_runtime_helpers_initialize_array(body, pos + 1, v, element_type, array_length, &values, &init_array_pos)) {
            context_->step("HandleRuntimeHelperInitializeArray: single-dim", inst);
            auto* temp_store = context_->function()->register_variable(VariableKind::InitializerTarget, v->type());
            auto* block = block_from_initializer(temp_store, element_type, array_length, values);
            auto* new_store = make<StLoc>(v, block);
            body->instructions.set(pos, new_store);
            body->instructions.remove_at(init_array_pos);
            context_->end_step(new_store);
            ILInlining::inline_if_possible(body, pos, *context_);
            return true;
        }
        if (array_length.size() == 1) {
            ArrayInitializerValues array_values;
            int instructions_to_remove = 0;
            if (handle_simple_array_initializer(function, body, pos + 1, v, array_length, &array_values, &instructions_to_remove)) {
                context_->step("HandleSimpleArrayInitializer: single-dim", inst);
                auto* temp_store = context_->function()->register_variable(VariableKind::InitializerTarget, v->type());
                auto* block = build_simple_array_initializer_block(temp_store, element_type, array_length, array_values);
                auto* new_store = make<StLoc>(v, block);
                body->instructions.set(pos, new_store);
                body->instructions.remove_range(pos + 1, instructions_to_remove);
                context_->end_step(new_store);
                ILInlining::inline_if_possible(body, pos, *context_);
                return true;
            }
            ILVariable* final_store = nullptr;
            if (handle_jagged_array_initializer(body, pos + 1, v, element_type, array_length[0], &final_store, &values, &instructions_to_remove)) {
                context_->step("HandleJaggedArrayInitializer: single-dim", inst);
                auto* temp_store = context_->function()->register_variable(VariableKind::InitializerTarget, v->type());
                ArrayInitializerValues indexed_values;
                for (std::size_t i = 0; i < values.size(); i++)
                    indexed_values.push_back({{make<LdcI4>(static_cast<int>(i))}, values[i]});
                auto* block = build_simple_array_initializer_block(temp_store, element_type, array_length, indexed_values);
                auto* new_store = make<StLoc>(final_store, block);
                body->instructions.set(pos, new_store);
                body->instructions.remove_range(pos + 1, instructions_to_remove);
                context_->end_step(new_store);
                ILInlining::inline_if_possible(body, pos, *context_);
                return true;
            }
        }
    }
    return false;
}

bool TransformArrayInitializers::transform_span_t_array_initialization(NewObj* inst, StatementTransformContext& context, ILInstruction** replacement) {
    *replacement = nullptr;
    if (!context.settings().array_initializers) return false;
    const IType* element_type = nullptr;
    srm::FieldDefinitionHandle field;
    int size = 0;
    if (!match_span_t_ctor_with_pointer_and_size(inst, context, &element_type, &field, &size)) return false;
    if (!has_field_rva(context.pe_file(), field)) return false;
    srm::BlobReader initial_value = get_initial_value_reader(context.pe_file(), field);
    *replacement = decode_array_initializer_or_utf8_string_literal(context, element_type, initial_value, size);
    return *replacement != nullptr;
}

bool TransformArrayInitializers::transform_runtime_helpers_create_span_initialization(Call* inst, StatementTransformContext& context,
                                                                                      ILInstruction** replacement) {
    *replacement = nullptr;
    if (!context.settings().array_initializers) return false;
    const IType* element_type = nullptr;
    srm::FieldDefinitionHandle field;
    if (!match_runtime_helpers_create_span(inst, context, &element_type, &field)) return false;
    if (!has_field_rva(context.pe_file(), field)) return false;
    if (is_sub_pattern_of_cpblk_initializer(inst)) return false;
    srm::BlobReader initial_value = get_initial_value_reader(context.pe_file(), field);
    int element_type_size = typesystem::get_size(*element_type);
    if (element_type_size <= 0 || initial_value.length() % element_type_size != 0) return false;
    int size = initial_value.length() / element_type_size;
    *replacement = decode_array_initializer_or_utf8_string_literal(context, element_type, initial_value, size);
    return *replacement != nullptr;
}

bool TransformArrayInitializers::do_transform_multi_dim(ILFunction* function, Block* body, int pos) {
    if (pos >= body->instructions.count() - 2) return false;
    ILInstruction* inst = body->instructions[pos];
    ILVariable* v = nullptr;
    ILInstruction* newarr_expr = nullptr;
    const IType* element_type = nullptr;
    std::vector<int> length;
    if (inst->match_st_loc(&v, &newarr_expr) && match_new_arr(newarr_expr, &element_type, &length)) {
        std::vector<ILInstruction*> values;
        int init_array_pos = 0;
        if (handle_runtime_helpers_initialize_array(body, pos + 1, v, element_type, length, &values, &init_array_pos)) {
            context_->step("HandleRuntimeHelpersInitializeArray: multi-dim", inst);
            auto* block = block_from_initializer(v, element_type, length, values);
            auto* new_store = make<StLoc>(v, block);
            body->instructions[pos]->replace_with(new_store);
            body->instructions.remove_at(init_array_pos);
            context_->end_step(new_store);
            ILInlining::inline_if_possible(body, pos, *context_);
            return true;
        }
        ArrayInitializerValues array_values;
        int instructions_to_remove = 0;
        if (handle_simple_array_initializer(function, body, pos + 1, v, length, &array_values, &instructions_to_remove)) {
            context_->step("HandleSimpleArrayInitializer: multi-dim", inst);
            auto* temp_store = context_->function()->register_variable(VariableKind::InitializerTarget, v->type());
            auto* block = build_simple_array_initializer_block(temp_store, element_type, length, array_values);
            auto* new_store = make<StLoc>(v, block);
            body->instructions.set(pos, new_store);
            body->instructions.remove_range(pos + 1, instructions_to_remove);
            context_->end_step(new_store);
            ILInlining::inline_if_possible(body, pos, *context_);
            return true;
        }
    }
    return false;
}

bool TransformArrayInitializers::do_transform_stack_alloc_initializer(Block* body, int pos) {
    if (pos >= body->instructions.count() - 2) return false;
    ILInstruction* inst = body->instructions[pos];
    ILVariable* v = nullptr;
    ILInstruction* localloc_expr = nullptr;
    ILInstruction* length_inst = nullptr;
    if (inst->match_st_loc(&v, &localloc_expr) && localloc_expr->match_loc_alloc(&length_inst)) {
        std::int64_t length_in_bytes = 0;
        srm::BlobReader blob;
        const IType* element_type = nullptr;
        if (length_inst->match_ldc_i(&length_in_bytes) && handle_cpblk_initializer(body, pos + 1, v, length_in_bytes, &blob, &element_type)) {
            context_->step("HandleCpblkInitializer", inst);
            auto* block = make<Block>(BlockKind::StackAllocInitializer);
            auto* temp_store =
                context_->function()->register_variable(VariableKind::InitializerTarget, typesystem::make_pointer_type(element_type));
            block->instructions.add(make<StLoc>(temp_store, localloc_expr));

            while (blob.remaining_bytes() > 0) {
                int offset = blob.offset();
                ILInstruction* element = read_element(blob, element_type);
                block->instructions.add(st_elem_ptr(temp_store, offset, element, element_type));
            }

            block->set_final_instruction(make<LdLoc>(temp_store));
            auto* new_store = make<StLoc>(v, block);
            body->instructions.set(pos, new_store);
            body->instructions.remove_at(pos + 1);
            context_->end_step(new_store);
            ILInlining::inline_if_possible(body, pos, *context_);
            ExpressionTransforms::run_on_single_statement(body->instructions[pos], *context_);
            return true;
        }
        std::vector<StObj*> values;
        int instructions_to_remove = 0;
        if (handle_sequential_loc_alloc_initializer(body, pos + 1, v, localloc_expr, &element_type, &values, &instructions_to_remove)) {
            context_->step("HandleSequentialLocAllocInitializer", inst);
            auto* block = make<Block>(BlockKind::StackAllocInitializer);
            auto* temp_store =
                context_->function()->register_variable(VariableKind::InitializerTarget, typesystem::make_pointer_type(element_type));
            block->instructions.add(make<StLoc>(temp_store, localloc_expr));
            for (StObj* value : values) {
                if (value != nullptr) block->instructions.add(rewrap_store(temp_store, value, element_type));
            }
            block->set_final_instruction(make<LdLoc>(temp_store));
            auto* new_store = make<StLoc>(v, block);
            body->instructions.set(pos, new_store);
            body->instructions.remove_range(pos + 1, instructions_to_remove);
            context_->end_step(new_store);
            ILInlining::inline_if_possible(body, pos, *context_);
            ExpressionTransforms::run_on_single_statement(body->instructions[pos], *context_);
            return true;
        }
    }
    return false;
}

ILInstruction* TransformArrayInitializers::read_element(srm::BlobReader& blob, const IType* element_type) {
    // Floating-point elements must be read as their actual type: their stored bytes are not the integer of the same width, and the
    // resulting constant has to match the element type's stack type (see StObj.CheckInvariant).
    switch (get_type_code(element_type)) {
        case TypeCode::Single: return make<LdcF4>(blob.read_single());
        case TypeCode::Double: return make<LdcF8>(blob.read_double());
        default: break;
    }
    switch (typesystem::get_size(*element_type)) {
        case 1: return make<LdcI4>(blob.read_byte());
        case 2: return make<LdcI4>(blob.read_int16());
        case 4: return make<LdcI4>(blob.read_int32());
        case 8: return make<LdcI8>(blob.read_int64());
        default: throw UsageError("not supported");
    }
}

bool TransformArrayInitializers::handle_cpblk_initializer(Block* block, int pos, ILVariable* v, long long length, srm::BlobReader* blob,
                                                          const IType** element_type) {
    *blob = srm::BlobReader();
    *element_type = nullptr;
    ILInstruction* dest = nullptr;
    ILInstruction* src = nullptr;
    ILInstruction* size = nullptr;
    if (!block->instructions[pos]->match_cpblk(&dest, &src, &size)) return false;
    const IField* field = nullptr;
    if (!dest->match_ld_loc(static_cast<const ILVariable*>(v)) || !match_get_static_field_address(src, &field) ||
        !size->match_ldc_i4(static_cast<std::int32_t>(length)))
        return false;
    if (!(v->is_single_definition() && v->load_count() == 2)) return false;
    if (field->metadata_token().is_nil()) return false;
    ILVariable* final_store = nullptr;
    ILInstruction* value = nullptr;
    if (!block->instructions[pos + 1]->match_st_loc(&final_store, &value)) {
        LdLoc* other_load_of_v = nullptr;
        for (LdLoc* l : v->load_instructions()) {
            if (as<Cpblk>(l->parent()) == nullptr) {
                other_load_of_v = l;
                break;
            }
        }
        if (other_load_of_v == nullptr) return false;
        final_store = other_load_of_v->parent()->extract(*context_);
        if (final_store == nullptr) return false;
        value = static_cast<StLoc*>(final_store->store_instructions()[0])->value();
    }
    srm::FieldDefinitionHandle fd(field->metadata_token());
    if (!has_field_rva(context_->pe_file(), fd)) return false;
    auto* new_obj = as<NewObj>(value);
    std::int32_t element_count = 0;
    if (value->match_ld_loc(static_cast<const ILVariable*>(v))) {
        *element_type = pointer_element_type(final_store->type());
    } else if (new_obj != nullptr && new_obj->arguments().count() == 2 &&
               typesystem::is_known_type(new_obj->method->declaring_type(), KnownTypeCode::SpanOfT) &&
               new_obj->arguments()[0]->match_ld_loc(static_cast<const ILVariable*>(v)) && new_obj->arguments()[1]->match_ldc_i4(&element_count)) {
        *element_type = new_obj->method->declaring_type()->type_arguments()[0];
        if (element_count != length / typesystem::get_size(**element_type)) return false;
    } else {
        return false;
    }
    *blob = get_initial_value_reader(context_->pe_file(), fd);
    return true;
}

bool TransformArrayInitializers::handle_sequential_loc_alloc_initializer(Block* block, int pos, ILVariable* store, ILInstruction* loc_alloc_instruction,
                                                                         const IType** element_type, std::vector<StObj*>* values,
                                                                         int* instructions_to_remove) {
    int element_count = 0;
    long long min_expected_offset = 0;
    values->clear();
    *element_type = nullptr;
    *instructions_to_remove = 0;

    ILInstruction* length_instruction = nullptr;
    if (!loc_alloc_instruction->match_loc_alloc(&length_instruction)) return false;

    srm::BlobReader blob;
    bool has_blob = false;

    std::int64_t byte_count = 0;
    if (length_instruction->match_ldc_i(&byte_count)) {
        // An initblk/cpblk that does not fit the expected shape is left in place; the per-element stobj scan below then fails to match
        // it and rejects the transform on its own.
        handle_loc_alloc_initializer_prefix(block, &pos, store, byte_count, &blob, &has_blob, instructions_to_remove);
    }

    bool values_set = false;
    for (int i = pos; i < block->instructions.count(); i++) {
        // match the basic stobj pattern
        ILInstruction* target = nullptr;
        ILInstruction* value = nullptr;
        const IType* current_type = nullptr;
        if (!block->instructions[i]->match_st_obj(&target, &value, &current_type) || uses_variable(value, store)) break;
        // first
        if (*element_type == nullptr) {
            *element_type = current_type;
            if (has_blob) {
                ILInstruction* count_instruction = PointerArithmeticOffset::detect(length_instruction, *element_type, true);
                std::int64_t values_length = 0;
                if (count_instruction == nullptr || !count_instruction->match_ldc_i(&values_length) || values_length < 1) return false;
                values->assign(static_cast<std::size_t>(static_cast<int>(values_length)), nullptr);
                values_set = true;
                int value_index = 0;
                while (blob.remaining_bytes() > 0 && value_index < static_cast<int>(values->size())) {
                    int offset = blob.offset();
                    ILInstruction* element = read_element(blob, *element_type);
                    (*values)[static_cast<std::size_t>(value_index)] = st_elem_ptr(store, offset, element, *element_type);
                    value_index++;
                }
            }
        } else if (!current_type->equals(*element_type)) {
            break;
        }
        SequentialStoreMatch match = get_sequential_store_offset(target, store, *element_type, &min_expected_offset);
        if (match == SequentialStoreMatch::Abort) return false;
        if (match != SequentialStoreMatch::Matched) break;
        if (!values_set) {
            ILInstruction* count_instruction = PointerArithmeticOffset::detect(length_instruction, *element_type, true);
            std::int64_t values_length = 0;
            if (count_instruction == nullptr || !count_instruction->match_ldc_i(&values_length) || values_length < 1) return false;
            values->assign(static_cast<std::size_t>(static_cast<int>(values_length)), nullptr);
            values_set = true;
        }
        if (min_expected_offset >= static_cast<long long>(values->size())) break;
        (*values)[static_cast<std::size_t>(min_expected_offset)] = static_cast<StObj*>(block->instructions[i]);
        element_count++;
    }

    // Every element must be written, either from the constant data blob or by an explicit store. A gap means the buffer is only
    // partially initialized (e.g. a 'stackalloc byte[16]' reinterpreted and written through a few elements), which cannot be represented
    // as an initializer and has to stay a sequence of individual stores.
    if (!values_set || std::find(values->begin(), values->end(), nullptr) != values->end() || store->kind() != VariableKind::StackSlot ||
        store->store_count() != 1 || store->address_count() != 0 || store->load_count() > static_cast<int>(values->size()) + 1)
        return false;

    if (store->load_instructions().empty()) throw UsageError("Sequence contains no elements");
    if (auto* final_store = as<StLoc>(store->load_instructions().back()->parent())) {
        *element_type = pointer_element_type(final_store->variable()->type());
    }
    *instructions_to_remove += element_count;

    return element_count <= static_cast<int>(values->size());
}

void TransformArrayInitializers::handle_loc_alloc_initializer_prefix(Block* block, int* pos, ILVariable* store, long long byte_count,
                                                                     srm::BlobReader* blob, bool* has_blob, int* instructions_to_remove) {
    // initblk(ldloc store, value, byteCount)
    ILInstruction* dest = nullptr;
    ILInstruction* size = nullptr;
    ILInstruction* src = nullptr;
    if (block->instructions[*pos]->match_initblk(&dest, &src, &size)) {
        if (!dest->match_ld_loc(static_cast<const ILVariable*>(store)) || !size->match_ldc_i(static_cast<std::int64_t>(byte_count))) return;
        (*instructions_to_remove)++;
        (*pos)++;
        return;
    }

    // cpblk(ldloc store, ldsflda/call get_Item(CreateSpan(field)) data, byteCount)
    if (block->instructions[*pos]->match_cpblk(&dest, &src, &size)) {
        if (!dest->match_ld_loc(static_cast<const ILVariable*>(store)) || !size->match_ldc_i(static_cast<std::int64_t>(byte_count))) return;
        const IField* field = nullptr;
        if (!match_get_static_field_address(src, &field)) return;
        if (field->metadata_token().is_nil()) return;
        srm::FieldDefinitionHandle fd(field->metadata_token());
        if (!has_field_rva(context_->pe_file(), fd)) return;
        *blob = get_initial_value_reader(context_->pe_file(), fd);
        *has_blob = true;
        (*instructions_to_remove)++;
        (*pos)++;
    }
}

ILInstruction* TransformArrayInitializers::rewrap_store(ILVariable* target, StObj* store_instruction, const IType* type) {
    ILInstruction* target_inst = nullptr;
    ILInstruction* left = nullptr;
    ILInstruction* right = nullptr;
    ILVariable* unused_variable = nullptr;
    (void)type;
    if (store_instruction->target()->match_ld_loc(&unused_variable))
        target_inst = make<LdLoc>(target);
    else if (store_instruction->target()->match_binary_numeric_instruction(BinaryNumericOperator::Add, &left, &right)) {
        auto* old = static_cast<BinaryNumericInstruction*>(store_instruction->target());
        target_inst = make<BinaryNumericInstruction>(BinaryNumericOperator::Add, make<LdLoc>(target), right, old->check_for_overflow, old->sign);
    } else
        throw UsageError("This should never happen: Bug in HandleSequentialLocAllocInitializer!");

    return make<StObj>(target_inst, store_instruction->value(), store_instruction->type());
}

StObj* TransformArrayInitializers::st_elem_ptr(ILVariable* target, int offset, ILInstruction* value, const IType* type) {
    ILInstruction* target_inst;
    if (offset == 0) {
        target_inst = make<LdLoc>(target);
    } else {
        auto* ld = make<LdLoc>(target);
        auto* conv = make<Conv>(make<LdcI4>(offset), PrimitiveType::I, false, Sign::Signed);
        target_inst = make<BinaryNumericInstruction>(BinaryNumericOperator::Add, ld, conv, false, Sign::None);
    }
    return make<StObj>(target_inst, value, type);
}

bool TransformArrayInitializers::handle_simple_array_initializer(ILFunction* function, Block* block, int pos, ILVariable* store,
                                                                 const std::vector<int>& array_length, ArrayInitializerValues* values,
                                                                 int* instructions_to_remove) {
    *instructions_to_remove = 0;
    int element_count = 0;
    // The dimensions come from the IL and need not multiply within int range. This is only a capacity hint, so saturate rather than overflow.
    int length = 1;
    for (int l : array_length) length = static_cast<int>(std::min(static_cast<long long>(length) * l, static_cast<long long>(std::numeric_limits<int>::max())));
    // Cannot pre-allocate the result array, because we do not know yet, whether there is in fact an array initializer. To prevent
    // excessive allocations, use min(|block|, arraySize) als initial capacity. This should prevent list-resizing as well as
    // out-of-memory errors.
    values->clear();
    ArrayInitializerValues values_list;
    values_list.reserve(static_cast<std::size_t>(std::max(0, std::min(block->instructions.count(), length))));

    std::vector<int> next_minimum_index(array_length.size(), 0);

    // The indices of the next element, or none when the store's indices are out of order or out of range.
    auto calculate_next_indices = [&](const InstructionCollection<ILInstruction>* indices, bool* exact_match) -> std::optional<std::vector<ILInstruction*>> {
        std::vector<ILInstruction*> next_indices(array_length.size(), nullptr);
        *exact_match = true;
        if (indices == nullptr) {
            for (std::size_t k = 0; k < next_indices.size(); k++) {
                next_indices[k] = make<LdcI4>(next_minimum_index[k]);
            }
        } else {
            bool previous_component_was_greater = false;
            for (int k = 0; k < indices->count(); k++) {
                std::int32_t index = 0;
                if (!(*indices)[k]->match_ldc_i4(&index)) return std::nullopt;
                std::size_t uk = static_cast<std::size_t>(k);
                // index must be in range [0..length[ and must be greater than or equal to nextMinimumIndex to avoid running out of
                // bounds or accidentally reordering instructions or overwriting previous instructions. However, leaving array slots
                // empty is allowed, as those are filled with default values when the initializer block is generated.
                if (index < 0 || index >= array_length[uk] || (!previous_component_was_greater && index < next_minimum_index[uk])) return std::nullopt;
                next_indices[uk] = make<LdcI4>(next_minimum_index[uk]);
                if (index != next_minimum_index[uk]) {
                    *exact_match = false;
                    // this flag allows us to check whether the whole set of indices is smaller: [3, 3] should be smaller than [4, 0] even
                    // though 3 > 0.
                    if (index > next_minimum_index[uk]) previous_component_was_greater = true;
                }
            }
        }

        for (int k = static_cast<int>(next_minimum_index.size()) - 1; k >= 0; k--) {
            std::size_t uk = static_cast<std::size_t>(k);
            next_minimum_index[uk]++;
            if (next_minimum_index[uk] < array_length[uk] || k == 0) break;
            next_minimum_index[uk] = 0;
        }

        return next_indices;
    };

    int i = pos;
    int step = 0;
    while (i < block->instructions.count()) {
        InstructionCollection<ILInstruction>* indices = nullptr;
        ILInstruction* target = nullptr;
        ILInstruction* value = nullptr;
        const IType* type = nullptr;
        ILVariable* address_temporary = nullptr;
        ILInstruction* address_value = nullptr;
        // stobj elementType(ldelema elementType(ldloc store, indices), value)
        if (block->instructions[i]->match_st_obj(&target, &value, &type)) {
            auto* ldelem = as<LdElema>(target);
            if (!(ldelem != nullptr && ldelem->array()->match_ld_loc(static_cast<const ILVariable*>(store)))) break;
            indices = &ldelem->indices();
            step = 1;
            // stloc s(ldelema elementType(ldloc store, indices))
            // stobj elementType(ldloc s, value)
        } else if (block->instructions[i]->match_st_loc(&address_temporary, &address_value)) {
            if (!(address_temporary->is_single_definition() && address_temporary->load_count() == 1)) break;
            auto* ldelem = as<LdElema>(address_value);
            if (!(ldelem != nullptr && ldelem->array()->match_ld_loc(static_cast<const ILVariable*>(store)))) break;
            if (!(i + 1 < block->instructions.count())) break;
            if (!block->instructions[i + 1]->match_st_obj(&target, &value, &type)) break;
            if (!(target->match_ld_loc(static_cast<const ILVariable*>(address_temporary)) &&
                  ldelem->array()->match_ld_loc(static_cast<const ILVariable*>(store))))
                break;
            indices = &ldelem->indices();
            step = 2;
        } else {
            break;
        }
        if (uses_variable(value, store)) break;
        if (indices->count() != static_cast<int>(array_length.size())) break;
        bool exact = false;
        if (length <= 0) break;
        do {
            auto next_indices = calculate_next_indices(indices, &exact);
            if (!next_indices) return false;
            if (exact) {
                values_list.push_back({*next_indices, value});
                element_count++;
                *instructions_to_remove += step;
            } else {
                values_list.push_back({*next_indices, nullptr});
            }
        } while (static_cast<int>(values_list.size()) < length && !exact);
        i += step;
    }
    if (i < block->instructions.count()) {
        ILInstruction* target = nullptr;
        ILInstruction* value = nullptr;
        const IType* type = nullptr;
        if (block->instructions[i]->match_st_obj(&target, &value, &type)) {
            // An element of the array is modified directly after the initializer: Abort transform, so that partial initializers are not
            // constructed.
            auto* ldelem = as<LdElema>(target);
            if (ldelem != nullptr && ldelem->array()->match_ld_loc(static_cast<const ILVariable*>(store))) return false;
        }
    }
    if (pos + *instructions_to_remove >= block->instructions.count()) return false;
    if (element_count == 0) return false;

    bool must_transform = ILInlining::is_catch_when_block(block) || ILInlining::is_in_constructor_initializer(function, block->instructions[pos]);
    // If there are not enough user-defined elements after scanning all instructions, we can abort the transform. This avoids unnecessary
    // allocations and OOM crashes (in case of int.MaxValue).
    if (element_count < length / 3 - 5) {
        if (!must_transform) return false;
        // .NET does not allow allocation of arrays >= 2 GB.
        if (length >= std::numeric_limits<int>::max()) return false;
    }

    for (int j = static_cast<int>(values_list.size()); j < length; j++) {
        bool unused = false;
        auto next_indices = calculate_next_indices(nullptr, &unused);
        if (!next_indices) return false;
        values_list.push_back({*next_indices, nullptr});
    }
    *values = std::move(values_list);
    return true;
}

bool TransformArrayInitializers::handle_jagged_array_initializer(Block* block, int pos, ILVariable* store, const IType* element_type, int length,
                                                                 ILVariable** final_store, std::vector<ILInstruction*>* values,
                                                                 int* instructions_to_remove) {
    *instructions_to_remove = 0;
    *final_store = nullptr;
    // Cannot pre-allocate the result array, because we do not know yet, whether there is in fact an array initializer. To prevent
    // excessive allocations, use min(|block|, arraySize) als initial capacity. This should prevent list-resizing as well as
    // out-of-memory errors.
    values->clear();
    std::vector<ILInstruction*> values_list;
    values_list.reserve(static_cast<std::size_t>(std::max(0, std::min(block->instructions.count(), length))));

    for (int i = 0; i < length; i++) {
        // 1. Instruction: (optional) temporary copy of store
        ILVariable* temp = nullptr;
        ILInstruction* store_load = nullptr;
        bool has_temporary_copy = block->instructions[pos]->match_st_loc(&temp, &store_load) && store_load->match_ld_loc(static_cast<const ILVariable*>(store));
        ILInstruction* initializer = nullptr;
        const IType* unused_type = nullptr;
        if (has_temporary_copy) {
            if (!match_jagged_array_store(block, pos + 1, temp, i, &initializer, &unused_type)) return false;
        } else {
            if (!match_jagged_array_store(block, pos, store, i, &initializer, &unused_type)) return false;
        }
        values_list.push_back(initializer);
        int inc = has_temporary_copy ? 3 : 2;
        pos += inc;
        *instructions_to_remove += inc;
    }
    // In case there is an extra copy of the store variable. Remove it and use its value instead.
    ILInstruction* array = nullptr;
    if (block->instructions[pos]->match_st_loc(final_store, &array)) {
        if (!array->match_ld_loc(static_cast<const ILVariable*>(store))) return false;
        (*instructions_to_remove)++;
    } else {
        *final_store = store;
    }
    *values = std::move(values_list);
    (void)element_type;
    return true;
}

bool TransformArrayInitializers::match_jagged_array_store(Block* block, int pos, ILVariable* store, int index, ILInstruction** initializer,
                                                          const IType** type) {
    *initializer = nullptr;
    *type = nullptr;
    // 3. Instruction: stobj(ldelema(ldloc temp, ldc.i4 0), ldloc tempArrayLoad)
    ILInstruction* final_instruction = block->instructions.element_at_or_default(pos + 1);
    ILInstruction* temp_access = nullptr;
    ILInstruction* temp_array_load = nullptr;
    ILVariable* initializer_store = nullptr;
    if (final_instruction == nullptr || !final_instruction->match_st_obj(&temp_access, &temp_array_load, type) ||
        !temp_array_load->match_ld_loc(&initializer_store)) {
        return false;
    }
    auto* elem_load = as<LdElema>(temp_access);
    if (elem_load == nullptr || !elem_load->array()->match_ld_loc(static_cast<const ILVariable*>(store)) || elem_load->indices().count() != 1 ||
        !elem_load->indices()[0]->match_ldc_i4(index)) {
        return false;
    }
    // 2. Instruction: stloc(temp) with block (array initializer)
    ILInstruction* next_instruction = block->instructions.element_at_or_default(pos);
    return next_instruction != nullptr && next_instruction->match_st_loc(static_cast<const ILVariable*>(initializer_store), initializer) &&
           (*initializer)->op_code == OpCode::Block;
}

bool TransformArrayInitializers::match_initialize_array_call(ILInstruction* instruction, ILInstruction** array, srm::FieldDefinitionHandle* field) {
    *array = nullptr;
    *field = srm::FieldDefinitionHandle();
    auto* call = as<CallInstruction>(instruction);
    if (call == nullptr || call->arguments().count() != 2) return false;
    const IMethod* method = call->method;
    if (!method->is_static() || method->name() != "InitializeArray" || method->declaring_type_definition() == nullptr) return false;
    if (!is_runtime_helpers(method->declaring_type())) return false;
    *array = call->arguments()[0];
    const IMember* member = nullptr;
    if (!call->arguments()[1]->match_ld_member_token(&member)) return false;
    if (member->metadata_token().is_nil()) return false;
    *field = srm::FieldDefinitionHandle(member->metadata_token());
    return true;
}

bool TransformArrayInitializers::handle_runtime_helpers_initialize_array(Block* body, int pos, ILVariable* array, const IType* array_type,
                                                                         const std::vector<int>& array_length, std::vector<ILInstruction*>* values,
                                                                         int* found_pos) {
    values->clear();
    *found_pos = -1;
    ILInstruction* array_inst = nullptr;
    srm::FieldDefinitionHandle field;
    if (match_initialize_array_call(body->instructions[pos], &array_inst, &field) && array_inst->match_ld_loc(static_cast<const ILVariable*>(array))) {
        if (has_field_rva(context_->pe_file(), field)) {
            std::vector<ILInstruction*> values_list;
            srm::BlobReader initial_value;
            try {
                initial_value = get_initial_value_reader(context_->pe_file(), field);
            } catch (const srm::BadImageFormatError& ex) {
                ILInstruction* at = body->instructions[pos];
                int start = at->il_range_is_empty() ? 0 : at->start_il_offset();
                array->function()->warnings.push_back(std::format("IL_{:04x}: {}", start, ex.message()));
                return false;
            }
            if (decode_array_initializer(array_type, initial_value, array_length, &values_list)) {
                *values = std::move(values_list);
                *found_pos = pos;
                return true;
            }
        }
    }
    return false;
}

// call InitializeArray(newarr T(size), ldmembertoken fieldToken)
// =>
// Block (ArrayInitializer) {
//     stloc i(newarr T(size))
//     stobj T(ldelema T(... indices ...), value)
//     final: ldloc i
// }
bool TransformArrayInitializers::do_transform_inline_runtime_helpers_initialize_array(Block* body, int pos) {
    ILInstruction* inst = body->instructions[pos];
    ILInstruction* array_inst = nullptr;
    srm::FieldDefinitionHandle field;
    if (!match_initialize_array_call(inst, &array_inst, &field)) return false;
    const IType* element_type = nullptr;
    std::vector<int> array_length;
    if (!match_new_arr(array_inst, &element_type, &array_length)) return false;
    if (!has_field_rva(context_->pe_file(), field)) return false;
    std::vector<ILInstruction*> values_list;
    srm::BlobReader initial_value = get_initial_value_reader(context_->pe_file(), field);
    if (!decode_array_initializer(element_type, initial_value, array_length, &values_list)) return false;
    context_->step("InlineRuntimeHelpersInitializeArray: single-dim", inst);
    auto* temp_store = context_->function()->register_variable(
        VariableKind::InitializerTarget,
        typesystem::make_array_type(context_->type_system(), element_type, static_cast<int>(array_length.size())));
    auto* block = block_from_initializer(temp_store, element_type, array_length, values_list);
    body->instructions.set(pos, block);
    context_->end_step(block);
    ILInlining::inline_if_possible(body, pos, *context_);
    return true;
}

ILInstruction* TransformArrayInitializers::get_null_expression(const IType* element_type) {
    const ITypeDefinition* type_def = typesystem::get_enum_underlying_type(*element_type)->get_definition();
    if (type_def == nullptr) return make<DefaultValue>(element_type);
    switch (type_def->known_type_code()) {
        case KnownTypeCode::Boolean:
        case KnownTypeCode::Char:
        case KnownTypeCode::SByte:
        case KnownTypeCode::Byte:
        case KnownTypeCode::Int16:
        case KnownTypeCode::UInt16:
        case KnownTypeCode::Int32:
        case KnownTypeCode::UInt32: return make<LdcI4>(0);
        case KnownTypeCode::Int64:
        case KnownTypeCode::UInt64: return make<LdcI8>(0);
        case KnownTypeCode::Single: return make<LdcF4>(0.0f);
        case KnownTypeCode::Double: return make<LdcF8>(0.0);
        case KnownTypeCode::Decimal: return make<LdcDecimal>(util::Decimal(0, 0, 0, false, 0));
        case KnownTypeCode::Void: throw std::invalid_argument("void is not a valid element type!");
        case KnownTypeCode::IntPtr:
        case KnownTypeCode::UIntPtr:
        default: return make<DefaultValue>(element_type);
    }
}

}  // namespace cb::dotnet::decompiler::il::transforms
