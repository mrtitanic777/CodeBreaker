// dotnet/decompiler/ilspycmd/dotnet_enum.cpp -- see dotnet_enum.hpp. The name tables are .NET's own enums, as Enum.GetNames lists them.
#include "dotnet/decompiler/ilspycmd/dotnet_enum.hpp"

#include <algorithm>
#include <vector>

namespace cb::dotnet::decompiler::ilspycmd {
namespace {

inline constexpr EnumName k_language_version[] = {
    {"CSharp1", 1},       {"CSharp2", 2},       {"CSharp3", 3},       {"CSharp4", 4},       {"CSharp5", 5},       {"CSharp6", 6},
    {"CSharp7", 7},       {"CSharp7_1", 701},   {"CSharp7_2", 702},   {"CSharp7_3", 703},   {"CSharp8_0", 800},   {"CSharp9_0", 900},
    {"CSharp10_0", 1000}, {"CSharp11_0", 1100}, {"CSharp12_0", 1200}, {"CSharp13_0", 1300}, {"CSharp14_0", 1400}, {"CSharp15_0", 1500},
    {"Preview", 1500},    {"Latest", 0x7FFFFFFF},
};

inline constexpr EnumName k_type_attributes[] = {
    {"NotPublic", 0},
    {"AutoLayout", 0},
    {"AnsiClass", 0},
    {"Class", 0},
    {"Public", 1},
    {"NestedPublic", 2},
    {"NestedPrivate", 3},
    {"NestedFamily", 4},
    {"NestedAssembly", 5},
    {"NestedFamANDAssem", 6},
    {"VisibilityMask", 7},
    {"NestedFamORAssem", 7},
    {"SequentialLayout", 8},
    {"ExplicitLayout", 16},
    {"LayoutMask", 24},
    {"ExtendedLayout", 24},
    {"Interface", 32},
    {"ClassSemanticsMask", 32},
    {"Abstract", 128},
    {"Sealed", 256},
    {"SpecialName", 1024},
    {"RTSpecialName", 2048},
    {"Import", 4096},
    {"Serializable", 8192},
    {"WindowsRuntime", 16384},
    {"UnicodeClass", 65536},
    {"AutoClass", 131072},
    {"StringFormatMask", 196608},
    {"CustomFormatClass", 196608},
    {"HasSecurity", 262144},
    {"ReservedMask", 264192},
    {"BeforeFieldInit", 1048576},
    {"CustomFormatMask", 12582912},
};  // flags=True
inline constexpr EnumName k_field_attributes[] = {
    {"PrivateScope", 0},
    {"Private", 1},
    {"FamANDAssem", 2},
    {"Assembly", 3},
    {"Family", 4},
    {"FamORAssem", 5},
    {"Public", 6},
    {"FieldAccessMask", 7},
    {"Static", 16},
    {"InitOnly", 32},
    {"Literal", 64},
    {"NotSerialized", 128},
    {"HasFieldRVA", 256},
    {"SpecialName", 512},
    {"RTSpecialName", 1024},
    {"HasFieldMarshal", 4096},
    {"PinvokeImpl", 8192},
    {"HasDefault", 32768},
    {"ReservedMask", 38144},
};  // flags=True
inline constexpr EnumName k_method_impl_attributes[] = {
    {"IL", 0},
    {"Managed", 0},
    {"Native", 1},
    {"OPTIL", 2},
    {"CodeTypeMask", 3},
    {"Runtime", 3},
    {"ManagedMask", 4},
    {"Unmanaged", 4},
    {"NoInlining", 8},
    {"ForwardRef", 16},
    {"Synchronized", 32},
    {"NoOptimization", 64},
    {"PreserveSig", 128},
    {"AggressiveInlining", 256},
    {"AggressiveOptimization", 512},
    {"InternalCall", 4096},
    {"Async", 8192},
    {"MaxMethodImplVal", 65535},
};  // flags=False
inline constexpr EnumName k_method_attributes[] = {
    {"PrivateScope", 0},
    {"ReuseSlot", 0},
    {"Private", 1},
    {"FamANDAssem", 2},
    {"Assembly", 3},
    {"Family", 4},
    {"FamORAssem", 5},
    {"Public", 6},
    {"MemberAccessMask", 7},
    {"UnmanagedExport", 8},
    {"Static", 16},
    {"Final", 32},
    {"Virtual", 64},
    {"HideBySig", 128},
    {"NewSlot", 256},
    {"VtableLayoutMask", 256},
    {"CheckAccessOnOverride", 512},
    {"Abstract", 1024},
    {"SpecialName", 2048},
    {"RTSpecialName", 4096},
    {"PinvokeImpl", 8192},
    {"HasSecurity", 16384},
    {"RequireSecObject", 32768},
    {"ReservedMask", 53248},
};  // flags=True
inline constexpr EnumName k_parameter_attributes[] = {
    {"None", 0},
    {"In", 1},
    {"Out", 2},
    {"Lcid", 4},
    {"Retval", 8},
    {"Optional", 16},
    {"HasDefault", 4096},
    {"HasFieldMarshal", 8192},
    {"Reserved3", 16384},
    {"Reserved4", 32768},
    {"ReservedMask", 61440},
};  // flags=True
inline constexpr EnumName k_event_attributes[] = {
    {"None", 0},
    {"SpecialName", 512},
    {"RTSpecialName", 1024},
    {"ReservedMask", 1024},
};  // flags=True
inline constexpr EnumName k_property_attributes[] = {
    {"None", 0},
    {"SpecialName", 512},
    {"RTSpecialName", 1024},
    {"HasDefault", 4096},
    {"Reserved2", 8192},
    {"Reserved3", 16384},
    {"Reserved4", 32768},
    {"ReservedMask", 62464},
};  // flags=True
inline constexpr EnumName k_manifest_resource_attributes[] = {
    {"Public", 1},
    {"Private", 2},
    {"VisibilityMask", 7},
};  // flags=True
inline constexpr EnumName k_generic_parameter_attributes[] = {
    {"None", 0},
    {"Covariant", 1},
    {"Contravariant", 2},
    {"VarianceMask", 3},
    {"ReferenceTypeConstraint", 4},
    {"NotNullableValueTypeConstraint", 8},
    {"DefaultConstructorConstraint", 16},
    {"SpecialConstraintMask", 28},
    {"AllowByRefLike", 32},
};  // flags=True
inline constexpr EnumName k_assembly_flags[] = {
    {"PublicKey", 1},
    {"Retargetable", 256},
    {"WindowsRuntime", 512},
    {"ContentTypeMask", 3584},
    {"DisableJitCompileOptimizer", 16384},
    {"EnableJitCompileTracking", 32768},
};  // flags=True
inline constexpr EnumName k_method_import_attributes[] = {
    {"None", 0},
    {"ExactSpelling", 1},
    {"CharSetAnsi", 2},
    {"CharSetUnicode", 4},
    {"CharSetAuto", 6},
    {"CharSetMask", 6},
    {"BestFitMappingEnable", 16},
    {"BestFitMappingDisable", 32},
    {"BestFitMappingMask", 48},
    {"SetLastError", 64},
    {"CallingConventionWinApi", 256},
    {"CallingConventionCDecl", 512},
    {"CallingConventionStdCall", 768},
    {"CallingConventionThisCall", 1024},
    {"CallingConventionFastCall", 1280},
    {"CallingConventionMask", 1792},
    {"ThrowOnUnmappableCharEnable", 4096},
    {"ThrowOnUnmappableCharDisable", 8192},
    {"ThrowOnUnmappableCharMask", 12288},
};  // flags=True
inline constexpr EnumName k_assembly_hash_algorithm[] = {
    {"None", 0},
    {"MD5", 32771},
    {"Sha1", 32772},
    {"Sha256", 32780},
    {"Sha384", 32781},
    {"Sha512", 32782},
};  // flags=False
inline constexpr EnumName k_constant_type_code[] = {
    {"Invalid", 0},
    {"Boolean", 2},
    {"Char", 3},
    {"SByte", 4},
    {"Byte", 5},
    {"Int16", 6},
    {"UInt16", 7},
    {"Int32", 8},
    {"UInt32", 9},
    {"Int64", 10},
    {"UInt64", 11},
    {"Single", 12},
    {"Double", 13},
    {"String", 14},
    {"NullReference", 18},
};  // flags=False
inline constexpr EnumName k_declarative_security_action[] = {
    {"None", 0},
    {"Demand", 2},
    {"Assert", 3},
    {"Deny", 4},
    {"PermitOnly", 5},
    {"LinkDemand", 6},
    {"InheritanceDemand", 7},
    {"RequestMinimum", 8},
    {"RequestOptional", 9},
    {"RequestRefuse", 10},
};  // flags=False
inline constexpr EnumName k_method_semantics_attributes[] = {
    {"Setter", 1},
    {"Getter", 2},
    {"Other", 4},
    {"Adder", 8},
    {"Remover", 16},
    {"Raiser", 32},
};  // flags=True
inline constexpr EnumName k_table_index[] = {
    {"Module", 0},
    {"TypeRef", 1},
    {"TypeDef", 2},
    {"FieldPtr", 3},
    {"Field", 4},
    {"MethodPtr", 5},
    {"MethodDef", 6},
    {"ParamPtr", 7},
    {"Param", 8},
    {"InterfaceImpl", 9},
    {"MemberRef", 10},
    {"Constant", 11},
    {"CustomAttribute", 12},
    {"FieldMarshal", 13},
    {"DeclSecurity", 14},
    {"ClassLayout", 15},
    {"FieldLayout", 16},
    {"StandAloneSig", 17},
    {"EventMap", 18},
    {"EventPtr", 19},
    {"Event", 20},
    {"PropertyMap", 21},
    {"PropertyPtr", 22},
    {"Property", 23},
    {"MethodSemantics", 24},
    {"MethodImpl", 25},
    {"ModuleRef", 26},
    {"TypeSpec", 27},
    {"ImplMap", 28},
    {"FieldRva", 29},
    {"EncLog", 30},
    {"EncMap", 31},
    {"Assembly", 32},
    {"AssemblyProcessor", 33},
    {"AssemblyOS", 34},
    {"AssemblyRef", 35},
    {"AssemblyRefProcessor", 36},
    {"AssemblyRefOS", 37},
    {"File", 38},
    {"ExportedType", 39},
    {"ManifestResource", 40},
    {"NestedClass", 41},
    {"GenericParam", 42},
    {"MethodSpec", 43},
    {"GenericParamConstraint", 44},
    {"Document", 48},
    {"MethodDebugInformation", 49},
    {"LocalScope", 50},
    {"LocalVariable", 51},
    {"LocalConstant", 52},
    {"ImportScope", 53},
    {"StateMachineMethod", 54},
    {"CustomDebugInformation", 55},
};  // flags=False

std::uint64_t width_mask(int bits) { return bits >= 64 ? ~std::uint64_t(0) : (std::uint64_t(1) << bits) - 1; }

bool is_white_space(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f'; }

std::string_view trim(std::string_view s) {
    while (!s.empty() && is_white_space(s.front())) s.remove_prefix(1);
    while (!s.empty() && is_white_space(s.back())) s.remove_suffix(1);
    return s;
}

char lower(char c) { return c >= 'A' && c <= 'Z' ? static_cast<char>(c - 'A' + 'a') : c; }

bool names_equal(std::string_view a, std::string_view b, bool ignore_case) {
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i)
        if (ignore_case ? lower(a[i]) != lower(b[i]) : a[i] != b[i]) return false;
    return true;
}

}  // namespace

std::string enum_to_string(const EnumInfo& info, std::uint64_t value) {
    const std::span<const EnumName> names = info.names;
    if (!info.flags) {
        for (const EnumName& n : names)
            if (n.value == value) return n.name;
        return std::to_string(value);
    }
    if (value == 0) return !names.empty() && names[0].value == 0 ? names[0].name : "0";
    std::uint64_t rest = value;
    std::vector<std::size_t> found;
    for (std::size_t i = names.size(); i-- > 0;) {
        if (i == 0 && names[i].value == 0) break;
        const std::uint64_t v = names[i].value;
        if ((rest & v) == v) {
            rest &= ~v;
            found.push_back(i);
        }
        if (rest == 0) break;
    }
    if (rest != 0) return std::to_string(value);
    std::string text;
    for (std::size_t k = found.size(); k-- > 0;) {
        if (!text.empty()) text += ", ";
        text += names[found[k]].name;
    }
    return text;
}

bool enum_try_parse(const EnumInfo& info, std::string_view text, bool ignore_case, int bits, bool is_signed, std::uint64_t* value) {
    text = trim(text);
    if (text.empty()) return false;
    const char first = text.front();
    if ((first >= '0' && first <= '9') || first == '-' || first == '+') {
        std::size_t i = 0;
        bool negative = false;
        if (text[0] == '-' || text[0] == '+') negative = text[i++] == '-';
        if (i == text.size()) return false;
        std::uint64_t magnitude = 0;
        for (; i < text.size(); ++i) {
            if (text[i] < '0' || text[i] > '9') return false;
            const std::uint64_t digit = static_cast<std::uint64_t>(text[i] - '0');
            if (magnitude > (~std::uint64_t(0) - digit) / 10) return false;
            magnitude = magnitude * 10 + digit;
        }
        if (is_signed) {
            const std::uint64_t limit = std::uint64_t(1) << (bits - 1);
            if (negative ? magnitude > limit : magnitude >= limit) return false;
            *value = (negative ? std::uint64_t(0) - magnitude : magnitude) & width_mask(bits);
        } else {
            if (negative ? magnitude != 0 : magnitude > width_mask(bits)) return false;
            *value = magnitude;
        }
        return true;
    }
    std::uint64_t result = 0;
    std::size_t start = 0;
    while (true) {
        const std::size_t comma = text.find(',', start);
        const std::string_view part = trim(text.substr(start, comma == std::string_view::npos ? std::string_view::npos : comma - start));
        if (part.empty()) return false;
        const auto it = std::find_if(info.names.begin(), info.names.end(), [&](const EnumName& n) { return names_equal(n.name, part, ignore_case); });
        if (it == info.names.end()) return false;
        result |= it->value;
        if (comma == std::string_view::npos) break;
        start = comma + 1;
    }
    *value = result;
    return true;
}

namespace enums {
#define CB_ILSPYCMD_ENUM(function, table, is_flags)  \
    const EnumInfo& function() {                     \
        static const EnumInfo info{table, is_flags}; \
        return info;                                 \
    }
CB_ILSPYCMD_ENUM(type_attributes, k_type_attributes, true)
CB_ILSPYCMD_ENUM(field_attributes, k_field_attributes, true)
CB_ILSPYCMD_ENUM(method_impl_attributes, k_method_impl_attributes, false)
CB_ILSPYCMD_ENUM(method_attributes, k_method_attributes, true)
CB_ILSPYCMD_ENUM(parameter_attributes, k_parameter_attributes, true)
CB_ILSPYCMD_ENUM(event_attributes, k_event_attributes, true)
CB_ILSPYCMD_ENUM(property_attributes, k_property_attributes, true)
CB_ILSPYCMD_ENUM(manifest_resource_attributes, k_manifest_resource_attributes, true)
CB_ILSPYCMD_ENUM(generic_parameter_attributes, k_generic_parameter_attributes, true)
CB_ILSPYCMD_ENUM(assembly_flags, k_assembly_flags, true)
CB_ILSPYCMD_ENUM(method_import_attributes, k_method_import_attributes, true)
CB_ILSPYCMD_ENUM(assembly_hash_algorithm, k_assembly_hash_algorithm, false)
CB_ILSPYCMD_ENUM(constant_type_code, k_constant_type_code, false)
CB_ILSPYCMD_ENUM(declarative_security_action, k_declarative_security_action, false)
CB_ILSPYCMD_ENUM(method_semantics_attributes, k_method_semantics_attributes, true)
CB_ILSPYCMD_ENUM(table_index, k_table_index, false)
CB_ILSPYCMD_ENUM(language_version, k_language_version, false)
#undef CB_ILSPYCMD_ENUM
}  // namespace enums

}  // namespace cb::dotnet::decompiler::ilspycmd
