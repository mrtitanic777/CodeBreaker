// dotnet/decompiler/il/transforms/tuple_transform.cpp -- see tuple_transform.hpp.
#include "dotnet/decompiler/il/transforms/tuple_transform.hpp"

#include <cassert>
#include <charconv>
#include <string>

#include "dotnet/decompiler/typesystem/tuple_type.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::il::transforms {

using typesystem::TupleType;

namespace {

// int.TryParse(text, out value) in the invariant culture, for the digits of a field name like "Item12".
bool try_parse_int(const std::string& text, int* value) {
    *value = 0;
    if (text.empty()) return false;
    const char* first = text.data();
    const char* last = first + text.size();
    if (*first == '+') ++first;
    int parsed = 0;
    auto result = std::from_chars(first, last, parsed);
    if (result.ec != std::errc() || result.ptr != last) return false;
    *value = parsed;
    return true;
}

}  // namespace

bool TupleTransform::match_tuple_field_access(LdFlda* inst, const IType** tuple_type, ILInstruction** target, int* position) {
    *tuple_type = inst->field()->declaring_type();
    *target = inst->target();
    const std::string field_name = inst->field()->name();
    if (!field_name.starts_with("Item")) {
        *position = 0;
        return false;
    }
    if (!try_parse_int(field_name.substr(4), position)) return false;
    int cardinality = 0;
    if (!TupleType::is_tuple_compatible(*tuple_type, cardinality)) return false;
    while (true) {
        auto* ldflda = as<LdFlda>(*target);
        if (ldflda == nullptr || ldflda->field()->name() != "Rest") break;
        int rest_cardinality = 0;
        if (!TupleType::is_tuple_compatible(ldflda->field()->declaring_type(), rest_cardinality)) break;
        *tuple_type = ldflda->field()->declaring_type();
        *target = ldflda->target();
        *position += TupleType::kRestPosition - 1;
    }
    return true;
}

bool TupleTransform::match_tuple_construction(NewObj* newobj, std::vector<ILInstruction*>* arguments) {
    arguments->clear();
    if (newobj == nullptr) return false;
    int element_count = 0;
    if (!TupleType::is_tuple_compatible(newobj->method->declaring_type(), element_count)) return false;
    arguments->assign(static_cast<std::size_t>(element_count), nullptr);
    int out_index = 0;
    while (element_count >= TupleType::kRestPosition) {
        if (newobj->arguments().count() != TupleType::kRestPosition) return false;
        for (int pos = 1; pos < TupleType::kRestPosition; pos++) {
            (*arguments)[static_cast<std::size_t>(out_index++)] = newobj->arguments()[pos - 1];
        }
        element_count -= TupleType::kRestPosition - 1;
        assert(out_index + element_count == static_cast<int>(arguments->size()));
        newobj = as<NewObj>(newobj->arguments()[newobj->arguments().count() - 1]);
        if (newobj == nullptr) return false;
        int rest_element_count = 0;
        if (!TupleType::is_tuple_compatible(newobj->method->declaring_type(), rest_element_count)) return false;
        if (rest_element_count != element_count) return false;
    }
    assert(out_index + element_count == static_cast<int>(arguments->size()));
    if (newobj->arguments().count() != element_count) return false;
    for (int i = 0; i < element_count; i++) {
        (*arguments)[static_cast<std::size_t>(out_index++)] = newobj->arguments()[i];
    }
    return true;
}

}  // namespace cb::dotnet::decompiler::il::transforms
