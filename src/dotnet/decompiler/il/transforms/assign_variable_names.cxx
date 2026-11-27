// dotnet/decompiler/il/transforms/assign_variable_names.cxx -- see assign_variable_names.hpp.
#include "dotnet/decompiler/il/transforms/assign_variable_names.hpp"

#include <windows.h>

#include <algorithm>
#include <cassert>
#include <cctype>
#include <charconv>
#include <string_view>

#include "core/error.hpp"
#include "dotnet/decompiler/humanizer/vocabularies.hpp"
#include "dotnet/decompiler/il/il_type_extensions.hpp"
#include "dotnet/decompiler/il/transforms/high_level_loop_transform.hpp"
#include "dotnet/decompiler/il/transforms/local_function_decompiler.hpp"
#include "dotnet/decompiler/typesystem/compilation.hpp"
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"
#include "dotnet/decompiler/typesystem/known_attributes.hpp"
#include "dotnet/decompiler/typesystem/local_function_method.hpp"
#include "dotnet/decompiler/typesystem/type_provider.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"
#include "dotnet/decompiler/util/char_info.hpp"

namespace cb::dotnet::decompiler::il::transforms {

namespace {

using util::UnicodeCategory;

constexpr char max_loop_variable_name = 'n';

// ---- strings as .NET sees them (UTF-16)

std::u16string w(const std::string& text) { return util::utf16(text); }
std::string u(const std::u16string& text) { return util::utf8(text); }

// char.ToLower of a UTF-16 code unit (the invariant culture's mapping)
char16_t to_lower_char(char16_t c) {
    wchar_t source = static_cast<wchar_t>(c);
    wchar_t mapped = source;
    if (LCMapStringEx(LOCALE_NAME_INVARIANT, LCMAP_LOWERCASE, &source, 1, &mapped, 1, nullptr, nullptr, 0) != 1) return c;
    return static_cast<char16_t>(mapped);
}

bool is_upper_char(char16_t c) { return util::get_unicode_category(c) == UnicodeCategory::UppercaseLetter; }
bool is_lower_char(char16_t c) { return util::get_unicode_category(c) == UnicodeCategory::LowercaseLetter; }

// string.IsNullOrWhiteSpace
bool is_null_or_white_space(const std::string& text) {
    for (char16_t c : w(text)) {
        if (!util::is_white_space(c)) return false;
    }
    return true;
}

bool starts_with_ignore_case(const std::string& text, std::string_view prefix) {
    if (text.size() < prefix.size()) return false;
    for (std::size_t i = 0; i < prefix.size(); i++) {
        if (std::tolower(static_cast<unsigned char>(text[i])) != std::tolower(static_cast<unsigned char>(prefix[i]))) return false;
    }
    return true;
}

bool ends_with_ignore_case(const std::string& text, std::string_view suffix) {
    if (text.size() < suffix.size()) return false;
    return starts_with_ignore_case(text.substr(text.size() - suffix.size()), suffix);
}

// `name.Length >= 4 && char.IsUpper(name[3])` (the UTF-16 length and character)
bool has_upper_case_fourth_char(const std::string& name) {
    std::u16string text = w(name);
    return text.size() >= 4 && is_upper_char(text[3]);
}

// ---- helpers of the C# stage (copies; see the header)

// CSharpOutputVisitor.IsKeyword(identifier) with no context: every keyword is unconditional.
bool is_keyword(const std::string& identifier) {
    static const std::unordered_set<std::string> unconditional_keywords = {
        "abstract", "as",       "base",     "bool",     "break",     "byte",     "case",     "catch",    "char",   "checked", "class",
        "const",    "continue", "decimal",  "default",  "delegate",  "do",       "double",   "else",     "enum",   "event",   "explicit",
        "extern",   "false",    "finally",  "fixed",    "float",     "for",      "foreach",  "goto",     "if",     "implicit", "in",
        "int",      "interface", "internal", "is",      "lock",      "long",     "namespace", "new",     "null",   "object",  "operator",
        "out",      "override", "params",   "private",  "protected", "public",   "readonly", "ref",      "return", "sbyte",   "sealed",
        "short",    "sizeof",   "stackalloc", "static", "string",    "struct",   "switch",   "this",     "throw",  "true",    "try",
        "typeof",   "uint",     "ulong",    "unchecked", "unsafe",   "ushort",   "using",    "virtual",  "void",   "volatile", "while"};
    static const std::unordered_set<std::string> query_keywords = {"from", "where",     "join",   "on",     "equals", "into", "let",
                                                                   "orderby", "ascending", "descending", "select", "group", "by"};
    static const std::size_t max_keyword_length = 10;  // "stackalloc" and "descending" are the longest
    // only 2-10 char lower-case identifiers can be keywords
    if (identifier.size() > max_keyword_length || identifier.size() < 2 || static_cast<unsigned char>(identifier[0]) < 'a') return false;
    if (unconditional_keywords.contains(identifier)) return true;
    if (query_keywords.contains(identifier)) return true;  // context == null
    if (identifier == "await") return true;                 // context == null
    return false;
}

// TextWriterTokenWriter.IsPrintableIdentifierChar
bool is_printable_identifier_char(const std::u16string& identifier, std::size_t index) {
    switch (identifier[index]) {
    case u'\\': return false;
    case u' ':
    case u'_':
    case u'`':
    case u'^': return true;
    default: break;
    }
    // char.GetUnicodeCategory(string, index) reads a surrogate pair as one character; the table here is per UTF-16 unit, so a pair is a
    // surrogate (not printable).
    switch (util::get_unicode_category(identifier[index])) {
    case UnicodeCategory::NonSpacingMark:
    case UnicodeCategory::SpacingCombiningMark:
    case UnicodeCategory::EnclosingMark:
    case UnicodeCategory::LineSeparator:
    case UnicodeCategory::ParagraphSeparator:
    case UnicodeCategory::Control:
    case UnicodeCategory::Format:
    case UnicodeCategory::Surrogate:
    case UnicodeCategory::PrivateUse:
    case UnicodeCategory::ConnectorPunctuation:
    case UnicodeCategory::ModifierSymbol:
    case UnicodeCategory::OtherNotAssigned:
    case UnicodeCategory::SpaceSeparator: return false;
    default: return true;
    }
}

// TextWriterTokenWriter.ContainsNonPrintableIdentifierChar
bool contains_non_printable_identifier_char(const std::u16string& identifier) {
    if (identifier.empty()) return false;

    for (std::size_t i = 0; i < identifier.size(); i++) {
        if (util::is_white_space(identifier[i])) return true;
        if (!is_printable_identifier_char(identifier, i)) return true;
    }

    return false;
}

// CSharpDecompiler.IsTransparentIdentifier
bool is_transparent_identifier(const std::string& identifier) {
    if (identifier.starts_with("<>")) {
        return identifier.find("TransparentIdentifier") != std::string::npos || identifier.find("TranspIdent") != std::string::npos;
    }
    // The VB compiler names the carriers of its query range variables
    // $VB$It, $VB$It1, $VB$It2 and $VB$ItAnonymous.
    return identifier.starts_with("$VB$It");
}

// CSharpDecompiler.IsWindowsFormsInitializeComponentMethod
bool is_windows_forms_initialize_component_method(const IMethod* method) {
    if (method == nullptr) return false;
    if (!(method->return_type()->kind() == typesystem::TypeKind::Void && method->name() == "InitializeComponent")) return false;
    for (const IType* t : typesystem::get_non_interface_base_types(method->declaring_type_definition())) {
        if (t->full_name() == "System.Windows.Forms.Control") return true;
    }
    return false;
}

// TransformFieldAndConstructorInitializers.IsGeneratedPrimaryConstructorBackingField
bool is_generated_primary_constructor_backing_field(const IField* field) {
    std::string name = field->name();
    return name.starts_with("<") && name.ends_with(">P") && field->has_attribute(typesystem::KnownAttribute::CompilerGenerated);
}

// IEntity.IsCompilerGenerated and IsCompilerGeneratedOrIsInCompilerGeneratedClass (NRExtensions)
bool is_compiler_generated(const typesystem::IEntity* entity) {
    return entity != nullptr && entity->has_attribute(typesystem::KnownAttribute::CompilerGenerated);
}

bool is_compiler_generated_or_is_in_compiler_generated_class(const typesystem::IEntity* entity) {
    for (; entity != nullptr; entity = entity->declaring_type_definition()) {
        if (is_compiler_generated(entity)) return true;
    }
    return false;
}

bool has_only_read_only_properties(const typesystem::ITypeDefinition* type) {
    for (const typesystem::IProperty* property : type->properties()) {
        if (property->can_set()) return false;
    }
    return true;
}

// IType.IsAnonymousType (NRExtensions)
bool is_anonymous_type(const IType* type) {
    if (type == nullptr) return false;
    // SRMExtensions.IsGeneratedName(type.Name)
    std::string name = type->name();
    bool has_generated_name = name.starts_with("<") || name.find('$') != std::string::npos;
    if (type->namespace_().empty() && has_generated_name && (name.find("AnonType") != std::string::npos || name.find("AnonymousType") != std::string::npos)) {
        const typesystem::ITypeDefinition* td = type->get_definition();
        return td != nullptr && is_compiler_generated(td) && has_only_read_only_properties(td);
    }
    return false;
}

// ---- names

bool is_lower_case(const std::string& name) {
    std::u16string text = w(name);
    return !text.empty() && to_lower_char(text[0]) == text[0];
}

std::vector<std::string> collect_all_lower_case_member_names(const typesystem::ITypeDefinition* type) {
    std::vector<std::string> names;
    typesystem::Filter<typesystem::IMember> filter = [](const typesystem::IMember* m) { return is_lower_case(m->name()); };
    for (const typesystem::IMember* item : type->get_members(filter)) names.push_back(item->name());
    return names;
}

std::vector<std::string> collect_all_lower_case_type_names(const typesystem::ITypeDefinition* type) {
    std::vector<std::string> names;
    const typesystem::INamespace* ns = typesystem::get_namespace_by_full_name(type->parent_module()->compilation(), type->namespace_());
    if (ns != nullptr) {
        for (const typesystem::ITypeDefinition* item : ns->types()) {
            if (is_lower_case(item->name())) names.push_back(item->name());
        }
    }
    const typesystem::ITypeDefinition* current = type;
    while (current != nullptr) {
        for (const typesystem::ITypeDefinition* nested : current->nested_types()) {
            if (is_lower_case(nested->name())) names.push_back(nested->name());
        }
        current = current->declaring_type_definition();
    }
    return names;
}

// string.IsNullOrWhiteSpace(varName) is false and the name is a C# identifier
bool is_valid_name_impl(const std::string& var_name) {
    if (is_null_or_white_space(var_name)) return false;
    std::u16string text = w(var_name);
    if (!(util::is_letter(text[0]) || text[0] == u'_')) return false;
    for (std::size_t i = 1; i < text.size(); i++) {
        if (!(util::is_letter_or_digit(text[i]) || text[i] == u'_')) return false;
    }
    return true;
}

// The name without trailing digits, and the number they make (1 if there are none).
std::string split_name(const std::string& name, int* number) {
    // First, identify whether the name already ends with a number:
    std::size_t pos = name.size();
    while (pos > 0 && name[pos - 1] >= '0' && name[pos - 1] <= '9') pos--;
    if (pos < name.size()) {
        int parsed = 0;
        const char* first = name.data() + pos;
        const char* last = name.data() + name.size();
        auto [end, error] = std::from_chars(first, last, parsed);
        if (error == std::errc() && end == last) {
            *number = parsed;
            return name.substr(0, pos);
        }
    }
    *number = 1;
    return name;
}

void add_existing_name(std::unordered_map<std::string, int>& reserved_variable_names, const std::string& name) {
    if (name.empty()) return;
    int number = 0;
    std::string name_without_digits = split_name(name, &number);
    auto it = reserved_variable_names.find(name_without_digits);
    if (it != reserved_variable_names.end()) {
        it->second = std::max(number, it->second);
    } else {
        reserved_variable_names.emplace(name_without_digits, number);
    }
}

std::optional<std::string> clean_up_variable_name(std::string name_utf8) {
    std::u16string name = w(name_utf8);
    // remove the backtick (generics)
    std::size_t pos = name.find(u'`');
    if (pos != std::u16string::npos) name = name.substr(0, pos);

    // remove field prefix:
    if (name.size() > 2 && name.starts_with(u"m_"))
        name = name.substr(2);
    else if (name.size() > 1 && name[0] == u'_' && (util::is_letter(name[1]) || name[1] == u'_'))
        name = name.substr(1);

    if (contains_non_printable_identifier_char(name)) {
        return std::nullopt;
    }

    if (name.empty()) return std::string("obj");
    if (!is_valid_name_impl(u(name))) {
        // A name taken from metadata may be legal there but not in C#; VB, for one,
        // separates the parts of its generated names with '$'.
        return std::nullopt;
    }
    std::u16string lower_case_name = name;
    lower_case_name[0] = to_lower_char(name[0]);
    std::string result = u(lower_case_name);
    if (is_keyword(result)) return std::nullopt;
    return result;
}

bool exclude_method_from_candidates(const IMethod* m) {
    if (m->symbol_kind() == typesystem::SymbolKind::Operator) return true;
    if (m->name() == "ToString") return true;
    if (m->name() == "Concat" && typesystem::is_known_type(m->declaring_type(), typesystem::KnownTypeCode::String)) return true;
    if (m->name() == "GetPinnableReference") return true;
    return false;
}

std::optional<std::string> get_name_from_instruction(ILInstruction* inst) {
    if (auto* get_pinnable_reference = as<GetPinnableReference>(inst)) {
        return get_name_from_instruction(get_pinnable_reference->argument());
    }
    if (auto* ldobj = as<LdObj>(inst)) {
        return get_name_from_instruction(ldobj->target());
    }
    if (auto* ldflda = as<LdFlda>(inst)) {
        if (is_compiler_generated_or_is_in_compiler_generated_class(ldflda->field())) return get_name_from_instruction(ldflda->target());
        return clean_up_variable_name(ldflda->field()->name());
    }
    if (auto* ldsflda = as<LdsFlda>(inst)) {
        return clean_up_variable_name(ldsflda->field()->name());
    }
    if (auto* call = as<CallInstruction>(inst)) {
        if (is<NewObj>(call)) return std::nullopt;
        const IMethod* m = call->method;
        if (exclude_method_from_candidates(m)) return std::nullopt;
        std::string method_name = m->name();
        if (starts_with_ignore_case(method_name, "get_") && m->parameters().empty()) {
            // use name from properties, but not from indexers
            return clean_up_variable_name(method_name.substr(4));
        } else if (starts_with_ignore_case(method_name, "Get") && has_upper_case_fourth_char(method_name)) {
            // use name from Get-methods
            return clean_up_variable_name(method_name.substr(3));
        }
        return std::nullopt;
    }
    if (auto* dyn_invoke_member = as<DynamicInvokeMemberInstruction>(inst)) {
        const std::string& dyn_name = dyn_invoke_member->name();
        if (starts_with_ignore_case(dyn_name, "Get") && has_upper_case_fourth_char(dyn_name)) {
            // use name from Get-methods
            return clean_up_variable_name(dyn_name.substr(3));
        }
        return std::nullopt;
    }
    return std::nullopt;
}

std::optional<std::string> get_name_for_argument(ILInstruction* parent, int i) {
    if (auto* stobj = as<StObj>(parent)) {
        const IField* field;
        if (auto* ldflda = as<LdFlda>(stobj->target()))
            field = ldflda->field();
        else if (auto* ldsflda = as<LdsFlda>(stobj->target()))
            field = ldsflda->field();
        else
            return std::nullopt;
        return clean_up_variable_name(field->name());
    }
    if (auto* call = as<CallInstruction>(parent)) {
        const IMethod* m = call->method;
        if (exclude_method_from_candidates(m)) return std::nullopt;
        if (m->parameters().size() == 1 && i == call->arguments().count() - 1) {
            // argument might be value of a setter
            std::string method_name = m->name();
            if (starts_with_ignore_case(method_name, "set_")) {
                return clean_up_variable_name(method_name.substr(4));
            } else if (starts_with_ignore_case(method_name, "Set") && method_name.size() >= 4 && has_upper_case_fourth_char(method_name)) {
                return clean_up_variable_name(method_name.substr(3));
            }
        }
        const typesystem::IParameter* p = call->get_parameter(i);
        if (p != nullptr && !p->name().empty()) return clean_up_variable_name(p->name());
        return std::nullopt;
    }
    if (is<Leave>(parent)) return std::string("result");
    return std::nullopt;
}

std::string get_name_by_type(const IType* type) {
    static const std::unordered_map<std::string, std::string> type_name_to_variable_name_dict = {
        {"System.Boolean", "flag"}, {"System.Byte", "b"},    {"System.SByte", "b"},   {"System.Int16", "num"}, {"System.Int32", "num"},
        {"System.Int64", "num"},    {"System.UInt16", "num"}, {"System.UInt32", "num"}, {"System.UInt64", "num"}, {"System.Single", "num"},
        {"System.Double", "num"},   {"System.Decimal", "num"}, {"System.String", "text"}, {"System.Object", "obj"}, {"System.Char", "c"}};

    type = typesystem::nullable_type::get_underlying_type(type);
    while (dynamic_cast<const typesystem::ModifiedType*>(type) != nullptr || dynamic_cast<const typesystem::PinnedType*>(type) != nullptr) {
        type = typesystem::nullable_type::get_underlying_type(dynamic_cast<const typesystem::TypeWithElementType*>(type)->element_type());
    }

    std::string name;
    std::string type_name = type->name();
    if (is_anonymous_type(type)) {
        name = "anon";
    } else if (type_name.ends_with("Exception")) {
        name = "ex";
    } else if (type_name.ends_with("EventArgs")) {
        name = "e";
    } else if (typesystem::is_csharp_native_integer_type(*type)) {
        name = "num";
    } else if (auto it = type_name_to_variable_name_dict.find(type->full_name()); it != type_name_to_variable_name_dict.end()) {
        name = it->second;
    } else {
        switch (type->kind()) {
        case typesystem::TypeKind::Array: name = "array"; break;
        case typesystem::TypeKind::Pointer: name = "ptr"; break;
        case typesystem::TypeKind::TypeParameter: name = "val"; break;
        case typesystem::TypeKind::Unknown: name = "val"; break;
        case typesystem::TypeKind::Dynamic: name = "val"; break;
        case typesystem::TypeKind::ByReference: name = "reference"; break;
        case typesystem::TypeKind::Tuple: name = "tuple"; break;
        case typesystem::TypeKind::NInt: name = "num"; break;
        case typesystem::TypeKind::NUInt: name = "num"; break;
        default: name = type_name; break;
        }
        // remove the 'I' for interfaces
        std::u16string name16 = w(name);
        if (name16.size() >= 3 && name16[0] == u'I' && is_upper_char(name16[1]) && is_lower_char(name16[2])) name = u(name16.substr(1));
        name = clean_up_variable_name(name).value_or("obj");
    }
    return name;
}

const IType* guess_type(const IType* variable_type, ILInstruction* inst, ILTransformContext& context) {
    if (!typesystem::is_known_type(variable_type, typesystem::KnownTypeCode::Object)) return variable_type;

    const IType* inferred_type = infer_type(inst, &context.type_system());
    if (inferred_type->kind() != typesystem::TypeKind::Unknown)
        return inferred_type;
    else
        return variable_type;
}

// the Vocabulary's singular of the name; false when it is none (or the same)
bool is_plural(const std::string& base_name, std::string* proposed_name) {
    std::string new_name = humanizer::vocabularies::get_default().singularize(base_name, false);
    if (is_null_or_white_space(new_name) || new_name == base_name) return false;
    *proposed_name = new_name;
    return true;
}

std::unordered_map<std::string, int> collect_reserved_variable_names(ILFunction* function, ILVariable* existing_variable, bool must_resolve_conflicts) {
    std::unordered_map<std::string, int> reserved_variable_names;
    // function.Ancestors.OfType<ILFunction>().Single(f => f.Parent == null)
    ILFunction* root_function = nullptr;
    for (ILInstruction* ancestor : function->ancestors()) {
        auto* f = as<ILFunction>(ancestor);
        if (f != nullptr && f->parent() == nullptr) {
            if (root_function != nullptr) throw UsageError("Sequence contains more than one matching element");
            root_function = f;
        }
    }
    if (root_function == nullptr) throw UsageError("Sequence contains no matching element");
    std::vector<ILFunction*> functions;
    for (ILInstruction* inst : root_function->descendants()) {
        if (auto* f = as<ILFunction>(inst)) functions.push_back(f);
    }
    for (ILFunction* f : functions) {
        for (const typesystem::IParameter* p : root_function->parameters) {
            add_existing_name(reserved_variable_names, p->name());
        }
        for (ILVariable* v : f->variables()) {
            if (v->kind() == VariableKind::Parameter) continue;
            if (v != existing_variable) add_existing_name(reserved_variable_names, v->name.value_or(""));
        }
    }
    if (must_resolve_conflicts) {
        std::vector<std::string> member_names = collect_all_lower_case_member_names(function->method->declaring_type_definition());
        std::vector<std::string> type_names = collect_all_lower_case_type_names(function->method->declaring_type_definition());
        member_names.insert(member_names.end(), type_names.begin(), type_names.end());
        // (the lower-case type names of the C# using scope come with the C# stage)
        for (const std::string& name : member_names) add_existing_name(reserved_variable_names, name);
    }
    return reserved_variable_names;
}

// the distinct values in order (Enumerable.Except and HashSet<string> keep a null as one value)
using OptionalNames = std::vector<std::optional<std::string>>;

void add_distinct(OptionalNames& names, const std::optional<std::string>& name) {
    if (std::find(names.begin(), names.end(), name) == names.end()) names.push_back(name);
}

bool function_contains_name(const std::unordered_set<std::string>& set, const std::optional<std::string>& name) {
    return name.has_value() && set.contains(*name);
}

}  // namespace

// ---- VariableScope

AssignVariableNames::VariableScope::VariableScope(ILFunction* function, ILTransformContext& context, VariableScope* parent_scope)
    : context_(&context), parent_scope_(parent_scope), function_(function) {
    num_display_class_locals_ = 0;
    assigned_local_signature_indices_.resize(static_cast<std::size_t>(std::max(0, function->local_variable_signature_length)));

    // find all loop counters in the current function
    // (TreeTraversal.PreOrder over the instruction tree, which ends at the first nested function)
    bool stop = false;
    auto visit = [&](auto& self, ILInstruction* inst) -> void {
        if (stop) return;
        if (is<ILFunction>(inst) && inst != function) {
            stop = true;
            return;
        }
        if (auto* container = as<BlockContainer>(inst); container != nullptr && container->kind() == ContainerKind::For && container->blocks.count() >= 1) {
            Block* increment_block = container->blocks.last();
            for (ILInstruction* i : increment_block->instructions) {
                ILVariable* variable = nullptr;
                if (HighLevelLoopTransform::match_increment(i, &variable)) loop_counters_.insert(variable);
            }
        }
        for (ILInstruction* child : inst->children()) {
            self(self, child);
            if (stop) return;
        }
    };
    visit(visit, function);

    // if this is the root scope, we also collect all lower-case type and member names
    // and fixed parameter names to avoid conflicts when naming local variables.
    if (parent_scope == nullptr) {
        auto lower_case_names = std::make_shared<std::unordered_set<std::string>>();
        for (const std::string& name : collect_all_lower_case_member_names(function->method->declaring_type_definition()))
            lower_case_names->insert(name);
        for (const IField* item : function->method->declaring_type_definition()->fields()) {
            if (is_generated_primary_constructor_backing_field(item)) {
                std::string item_name = item->name();
                // item.Name.Substring(1, item.Name.Length - 3)
                std::string name = item_name.substr(1, item_name.size() - 3);
                lower_case_names->insert(name);
                add_existing_name(reserved_variable_names_, name);
            }
        }
        for (const std::string& name : collect_all_lower_case_type_names(function->method->declaring_type_definition())) {
            lower_case_names->insert(name);
            add_existing_name(reserved_variable_names_, name);
        }
        // WAITS: UsingScope (CollectAllLowerCaseTypeNames(context.UsingScope): the lower-case types of the using declarations)
        current_lower_case_type_or_member_names_ = lower_case_names;

        // handle implicit parameters of set or event accessors
        auto is_set_or_event_accessor = [](const IMethod* method) {
            switch (method->accessor_kind()) {
            case typesystem::MethodSemanticsAttributes::Setter:
            case typesystem::MethodSemanticsAttributes::Adder:
            case typesystem::MethodSemanticsAttributes::Remover: return true;
            default: return false;
            }
        };
        if (function->method != nullptr && is_set_or_event_accessor(function->method) && !function->parameters.empty()) {
            const auto& method_parameters = function->method->parameters();
            for (std::size_t i = 0; i + 1 < method_parameters.size(); i++) {
                add_existing_name(reserved_variable_names_, method_parameters[i]->name());
            }
            const typesystem::IParameter* last_parameter = method_parameters.back();
            auto variable_for_last_parameter = [&]() -> ILVariable* {
                for (ILVariable* v : function->variables()) {
                    if (v->function() == function && v->kind() == VariableKind::Parameter && v->index().has_value() &&
                        *v->index() == static_cast<int>(method_parameters.size()) - 1)
                        return v;
                }
                return nullptr;
            };
            const typesystem::IMember* accessor_owner = function->method->accessor_owner();
            if (const auto* prop = dynamic_cast<const typesystem::IProperty*>(accessor_owner)) {
                if (function->method->accessor_kind() == typesystem::MethodSemanticsAttributes::Setter) {
                    bool has_value_parameter = false;
                    for (const typesystem::IParameter* p : prop->parameters()) {
                        if (p->name() == "value") has_value_parameter = true;
                    }
                    if (has_value_parameter) {
                        function->warnings.push_back("Parameter named \"value\" already present in property signature!");
                    } else {
                        ILVariable* variable_for_last = variable_for_last_parameter();
                        if (variable_for_last == nullptr) {
                            add_existing_name(reserved_variable_names_, last_parameter->name());
                        } else {
                            if (variable_for_last->name != "value") {
                                variable_for_last->name = "value";
                            }
                            add_existing_name(reserved_variable_names_, variable_for_last->name.value_or(""));
                        }
                    }
                }
            } else if (dynamic_cast<const typesystem::IEvent*>(accessor_owner) != nullptr) {
                if (function->method->accessor_kind() != typesystem::MethodSemanticsAttributes::Raiser) {
                    ILVariable* variable_for_last = variable_for_last_parameter();
                    if (variable_for_last == nullptr) {
                        add_existing_name(reserved_variable_names_, last_parameter->name());
                    } else {
                        if (variable_for_last->name != "value") {
                            variable_for_last->name = "value";
                        }
                        add_existing_name(reserved_variable_names_, variable_for_last->name.value_or(""));
                    }
                }
            } else {
                add_existing_name(reserved_variable_names_, last_parameter->name());
            }
        } else {
            // function.Variables.Where(v => v.Kind == VariableKind.Parameter && v.Index >= 0).ToDictionary(v => v.Index)
            std::unordered_map<int, ILVariable*> variables;
            for (ILVariable* v : function->variables()) {
                if (v->kind() == VariableKind::Parameter && v->index().has_value() && *v->index() >= 0) {
                    if (!variables.emplace(*v->index(), v).second) throw UsageError("An item with the same key has already been added.");
                }
            }
            for (std::size_t i = 0; i < function->parameters.size(); i++) {
                const typesystem::IParameter* p = function->parameters[i];
                std::string name = p->name();
                if (is_null_or_white_space(name) && p->type() != &typesystem::SpecialType::arg_list()) {
                    // needs to be consistent with logic in ILReader.CreateILVariable
                    name = "P_" + std::to_string(i);
                }
                if (auto it = variables.find(static_cast<int>(i)); it != variables.end()) variable_mapping_[it->second] = name;
                add_existing_name(reserved_variable_names_, name);
            }
        }
    } else {
        current_lower_case_type_or_member_names_ = parent_scope->current_lower_case_type_or_member_names_;
        std::unordered_map<int, ILVariable*> variables;
        for (ILVariable* v : function->variables()) {
            if (v->kind() == VariableKind::Parameter) {
                // (the key is the nullable Index; a parameter always has one)
                if (!variables.emplace(v->index().value_or(0), v).second) throw UsageError("An item with the same key has already been added.");
            }
        }

        for (std::size_t i = 0; i < function->parameters.size(); i++) {
            const typesystem::IParameter* p = function->parameters[i];
            std::string name;
            if (is_null_or_white_space(p->name())) {
                auto found = variables.find(static_cast<int>(i));
                if (found == variables.end()) throw UsageError("The given key was not present in the dictionary.");
                name = found->second->name.value_or("");
            } else {
                name = p->name();
            }

            if ((function->kind() == ILFunctionKind::Delegate || function->kind() == ILFunctionKind::ExpressionTree) && is_transparent_identifier(name)) {
                add_existing_name(reserved_variable_names_, name);
                if (auto it = variables.find(static_cast<int>(i)); it != variables.end()) variable_mapping_[it->second] = name;
            }
            int new_index = 0;
            std::string name_without_number = split_name(name, &new_index);
            if (!parent_scope->is_reserved_variable_name(name_without_number, nullptr)) {
                add_existing_name(reserved_variable_names_, name);
                if (auto it = variables.find(static_cast<int>(i)); it != variables.end()) variable_mapping_[it->second] = name;
            } else if (auto it = variables.find(static_cast<int>(i)); it != variables.end()) {
                it->second->has_generated_name = true;
            }
        }
    }
}

void AssignVariableNames::VariableScope::add(std::uint32_t local_function_token, const std::string& name) { local_functions_[local_function_token] = name; }

std::optional<std::string> AssignVariableNames::VariableScope::try_get_existing_name_of_local_function(std::uint32_t local_function_token) const {
    if (auto it = local_functions_.find(local_function_token); it != local_functions_.end()) return it->second;
    return parent_scope_ != nullptr ? parent_scope_->try_get_existing_name_of_local_function(local_function_token) : std::nullopt;
}

std::optional<std::string> AssignVariableNames::VariableScope::try_get_existing_name(ILVariable* v) const {
    if (auto it = variable_mapping_.find(v); it != variable_mapping_.end()) return it->second;
    return parent_scope_ != nullptr ? parent_scope_->try_get_existing_name(v) : std::nullopt;
}

std::optional<std::string> AssignVariableNames::VariableScope::try_get_existing_name(ILFunction* function, int index) const {
    if (function_ == function) {
        return assigned_local_signature_indices_.at(static_cast<std::size_t>(index));
    } else {
        return parent_scope_ != nullptr ? parent_scope_->try_get_existing_name(function, index) : std::nullopt;
    }
}

void AssignVariableNames::VariableScope::assign_name_to_local_signature_index(ILFunction* function, int index, const std::string& name) {
    VariableScope* scope = this;
    while (scope != nullptr && scope->function_ != function) scope = scope->parent_scope_;
    assert(scope != nullptr);
    scope->assigned_local_signature_indices_.at(static_cast<std::size_t>(index)) = name;
}

bool AssignVariableNames::VariableScope::is_reserved_variable_name(const std::string& name, int* index) const {
    if (auto it = reserved_variable_names_.find(name); it != reserved_variable_names_.end()) {
        if (index != nullptr) *index = it->second;
        return true;
    }
    if (index != nullptr) *index = 0;
    if (parent_scope_ != nullptr) return parent_scope_->is_reserved_variable_name(name, index);
    return false;
}

void AssignVariableNames::VariableScope::reserve_variable_name(const std::string& name, int index) { reserved_variable_names_[name] = index; }

std::string AssignVariableNames::VariableScope::next_display_class_local() {
    if (parent_scope_ != nullptr) return parent_scope_->next_display_class_local();
    return "CS$<>8__locals" + std::to_string(num_display_class_locals_++);
}

bool AssignVariableNames::VariableScope::is_loop_counter(ILVariable* v) const {
    return loop_counters_.contains(v) || (parent_scope_ != nullptr && parent_scope_->is_loop_counter(v));
}

std::string AssignVariableNames::VariableScope::assign_name_if_unassigned(ILVariable* v) {
    if (auto it = variable_mapping_.find(v); it != variable_mapping_.end()) return it->second;
    return assign_name(v);
}

std::string AssignVariableNames::VariableScope::assign_name(ILVariable* v) {
    // variable has no valid name
    std::optional<std::string> new_name_value = v->name;
    if (v->has_generated_name || !new_name_value.has_value() || !is_valid_name_impl(*new_name_value)) {
        // don't use the name from the debug symbols if it looks like a generated name
        // generate a new one based on how the variable is used
        new_name_value = generate_name_for_variable(v);
    }
    std::string new_name = *new_name_value;
    // use the existing name and update index appended to future conflicts
    int new_index = 0;
    std::string name_without_number = split_name(new_name, &new_index);
    int last_used_index = 0;
    if (is_reserved_variable_name(name_without_number, &last_used_index)) {
        if (typesystem::is_known_type(v->type(), typesystem::KnownTypeCode::Int32) && is_loop_counter(v)) {
            // special case for loop counters,
            // we don't want them to be named i, i2, ..., but i, j, ...
            new_name = generate_name_for_variable(v);
            name_without_number = new_name;
            new_index = 1;
        }
    }
    if (is_reserved_variable_name(name_without_number, &last_used_index)) {
        // name without number was already used
        if (new_index > last_used_index) {
            // new index is larger than last, so we can use it
        } else {
            // new index is smaller or equal, so we use the next value
            new_index = last_used_index + 1;
        }
        // resolve conflicts by appending the index to the new name:
        new_name = name_without_number + std::to_string(new_index);
    }
    // update the last used index
    reserve_variable_name(name_without_number, new_index);
    variable_mapping_[v] = new_name;
    return new_name;
}

std::string AssignVariableNames::VariableScope::generate_name_for_variable(ILVariable* variable) {
    std::optional<std::string> proposed_name;
    auto proposed_name_is_empty = [&]() { return !proposed_name.has_value() || proposed_name->empty(); };
    const std::unordered_set<std::string>& lower_case_names = *current_lower_case_type_or_member_names_;
    if (typesystem::is_known_type(variable->type(), typesystem::KnownTypeCode::Int32)) {
        // test whether the variable might be a loop counter
        if (loop_counters_.contains(variable)) {
            // For loop variables, use i,j,k,l,m,n
            for (char c = 'i'; c <= max_loop_variable_name; c++) {
                if (!is_reserved_variable_name(std::string(1, c), nullptr)) {
                    proposed_name = std::string(1, c);
                    break;
                }
            }
        }
    }
    // The ComponentResourceManager inside InitializeComponent must be named "resources",
    // otherwise the WinForms designer won't load the Form.
    if (is_windows_forms_initialize_component_method(context_->function()->method) &&
        variable->type()->full_name() == "System.ComponentModel.ComponentResourceManager") {
        proposed_name = "resources";
    }
    if (proposed_name_is_empty()) {
        // variable.AddressInstructions.OfType<LdLoca>().Select(arg => arg.Parent is CallInstruction c ? c.GetParameter(arg.ChildIndex)?.Name : null)
        //     .Where(arg => !string.IsNullOrWhiteSpace(arg)).Except(currentLowerCaseTypeOrMemberNames).ToList()
        OptionalNames proposed_name_for_address;
        for (LdLoca* arg : variable->address_instructions()) {
            std::optional<std::string> candidate;
            if (auto* c = as<CallInstruction>(arg->parent())) {
                const typesystem::IParameter* parameter = c->get_parameter(arg->child_index());
                if (parameter != nullptr) candidate = parameter->name();
            }
            if (!candidate.has_value() || is_null_or_white_space(*candidate)) continue;
            if (function_contains_name(lower_case_names, candidate)) continue;
            add_distinct(proposed_name_for_address, candidate);
        }
        if (!proposed_name_for_address.empty()) {
            proposed_name = proposed_name_for_address[0];
        }
    }
    if (proposed_name_is_empty()) {
        OptionalNames proposed_name_for_stores;  // a HashSet<string>
        for (IStoreInstruction* store : variable->store_instructions()) {
            ILInstruction* store_inst = store->as_instruction();
            if (auto* stloc = as<StLoc>(store_inst)) {
                std::optional<std::string> name = get_name_from_instruction(stloc->value());
                if (!function_contains_name(lower_case_names, name)) add_distinct(proposed_name_for_stores, name);
            } else if (auto* match = as<MatchInstruction>(store_inst); match != nullptr && match->slot_info() == &MatchInstruction::sub_patterns_slot) {
                std::optional<std::string> name = get_name_from_instruction(match->tested_operand());
                if (!function_contains_name(lower_case_names, name)) add_distinct(proposed_name_for_stores, name);
            } else if (auto* pinned_region = as<PinnedRegion>(store_inst)) {
                std::optional<std::string> name = get_name_from_instruction(pinned_region->init());
                if (!function_contains_name(lower_case_names, name)) add_distinct(proposed_name_for_stores, name);
            }
        }
        if (proposed_name_for_stores.size() == 1) {
            proposed_name = proposed_name_for_stores[0];
        }
    }
    if (proposed_name_is_empty()) {
        OptionalNames proposed_name_for_loads;
        for (LdLoc* arg : variable->load_instructions()) {
            std::optional<std::string> name = get_name_for_argument(arg->parent(), arg->child_index());
            if (!function_contains_name(lower_case_names, name)) add_distinct(proposed_name_for_loads, name);
        }
        if (proposed_name_for_loads.size() == 1) {
            proposed_name = proposed_name_for_loads[0];
        }
    }
    if (proposed_name_is_empty() && variable->kind() == VariableKind::StackSlot) {
        OptionalNames proposed_name_for_stores_from_new_obj;
        for (IStoreInstruction* store : variable->store_instructions()) {
            auto* expr = as<StLoc>(store->as_instruction());
            if (expr == nullptr) continue;
            std::optional<std::string> name = get_name_by_type(guess_type(variable->type(), expr->value(), *context_));
            if (!function_contains_name(lower_case_names, name)) add_distinct(proposed_name_for_stores_from_new_obj, name);
        }
        if (proposed_name_for_stores_from_new_obj.size() == 1) {
            proposed_name = proposed_name_for_stores_from_new_obj[0];
        }
    }
    if (proposed_name_is_empty()) {
        proposed_name = get_name_by_type(variable->type());
    }

    // for generated names remove number-suffixes
    int ignored_number = 0;
    return split_name(*proposed_name, &ignored_number);
}

// ---- the transform

void AssignVariableNames::run(ILFunction* function, ILTransformContext& context) {
    context_ = &context;
    work_list_.clear();
    scopes_.clear();
    work_list_.emplace_back(function, nullptr);
    Visitor visitor(*this);

    while (!work_list_.empty()) {
        auto [current_function, parent_context] = work_list_.front();
        work_list_.pop_front();

        scopes_.push_back(std::make_unique<VariableScope>(current_function, *context_, parent_context));
        VariableScope* nested_context = scopes_.back().get();

        for (ILFunction* local_function : std::vector<ILFunction*>(current_function->local_functions.begin(), current_function->local_functions.end())) {
            assign_name_to_local_function(local_function, *nested_context);
            work_list_.emplace_back(local_function, nested_context);
        }

        accept_visitor<VariableScope*, Unit>(*current_function->body(), visitor, nested_context);

        if (current_function->kind() != ILFunctionKind::TopLevelFunction) {
            std::vector<ILVariable*> parameters;
            for (ILVariable* v : current_function->variables()) {
                if (v->kind() == VariableKind::Parameter && v->index().has_value() && *v->index() >= 0) parameters.push_back(v);
            }
            for (ILVariable* p : parameters) {
                p->name = nested_context->assign_name_if_unassigned(p);
            }
        }
    }
    scopes_.clear();
}

void AssignVariableNames::assign_name_to_local_function(ILFunction* function, VariableScope& parent_context) {
    std::string function_name;
    std::string ignored_caller_name;
    std::optional<std::string> new_name;
    if (LocalFunctionDecompiler::parse_local_function_name(function->name, &ignored_caller_name, &function_name)) new_name = function_name;
    if (!new_name.has_value() || !is_valid_name_impl(*new_name)) new_name = std::nullopt;
    std::string name_without_number;
    int number;
    if (new_name.has_value() && !new_name->empty()) {
        name_without_number = split_name(*new_name, &number);
    } else {
        name_without_number = "f";
        number = 1;
    }
    int count;
    int current_index = 0;
    if (!parent_context.is_reserved_variable_name(name_without_number, &current_index)) {
        count = 1;
    } else {
        if (current_index < number)
            count = number;
        else
            count = std::max(number, current_index) + 1;
    }
    parent_context.reserve_variable_name(name_without_number, count);
    std::string result_name;
    if (count > 1) {
        result_name = name_without_number + std::to_string(count);
    } else {
        result_name = name_without_number;
    }
    function->name = result_name;
    dynamic_cast<const typesystem::LocalFunctionMethod*>(function->reduced_method)->set_name(result_name);
    parent_context.add(function->reduced_method->metadata_token().token(), result_name);
}

bool AssignVariableNames::is_valid_name(const std::string& var_name) { return is_valid_name_impl(var_name); }

/// Must be in sync with GetNameFromInstruction.
bool AssignVariableNames::is_supported_instruction(const ILInstruction* arg) {
    return is<GetPinnableReference>(arg) || is<LdObj>(arg) || is<LdFlda>(arg) || is<LdsFlda>(arg) || dynamic_cast<const CallInstruction*>(arg) != nullptr;
}

std::string AssignVariableNames::generate_foreach_variable_name(ILFunction* function, ILInstruction* value_context, ILVariable* existing_variable,
                                                                bool must_resolve_conflicts) {
    if (function == nullptr) throw std::invalid_argument("function");
    if (existing_variable != nullptr && !existing_variable->has_generated_name) {
        return existing_variable->name.value_or("");
    }
    std::unordered_map<std::string, int> reserved_variable_names = collect_reserved_variable_names(function, existing_variable, must_resolve_conflicts);

    std::optional<std::string> base_name = get_name_from_instruction(value_context);
    if (!base_name.has_value() || base_name->empty()) {
        auto* ldloc = as<LdLoc>(value_context);
        if (ldloc != nullptr && ldloc->variable()->kind() == VariableKind::Parameter) {
            base_name = ldloc->variable()->name;
        }
    }
    std::string proposed_name = "item";

    if (base_name.has_value() && !base_name->empty()) {
        const std::string& base = *base_name;
        if (!is_plural(base, &proposed_name)) {
            if (base.size() > 4 && base.ends_with("List")) {
                proposed_name = base.substr(0, base.size() - 4);
            } else if (base.size() == 4 && ends_with_ignore_case(base, "list")) {
                // baseName.Equals("list", StringComparison.OrdinalIgnoreCase)
                proposed_name = "item";
            } else if (ends_with_ignore_case(base, "children")) {
                proposed_name = base.substr(0, base.size() - 3);
            }
        }
    }

    // remove any numbers from the proposed name
    int number = 0;
    proposed_name = split_name(proposed_name, &number);

    if (!reserved_variable_names.contains(proposed_name)) {
        reserved_variable_names.emplace(proposed_name, 0);
    }
    int count = ++reserved_variable_names[proposed_name];
    assert(!is_null_or_white_space(proposed_name));
    if (count > 1) {
        return proposed_name + std::to_string(count);
    } else {
        return proposed_name;
    }
}

std::string AssignVariableNames::generate_variable_name(ILFunction* function, const IType* type, ILInstruction* value_context,
                                                        ILVariable* existing_variable, bool must_resolve_conflicts) {
    if (function == nullptr) throw std::invalid_argument("function");
    std::unordered_map<std::string, int> reserved_variable_names = collect_reserved_variable_names(function, existing_variable, must_resolve_conflicts);

    std::optional<std::string> base_name;
    if (value_context != nullptr) {
        base_name = get_name_from_instruction(value_context);
        if (!base_name.has_value()) base_name = get_name_by_type(type);
    } else {
        base_name = get_name_by_type(type);
    }
    std::string proposed_name = "obj";

    if (base_name.has_value() && !base_name->empty()) {
        const std::string& base = *base_name;
        if (!is_plural(base, &proposed_name)) {
            if (base.size() > 4 && base.ends_with("List")) {
                proposed_name = base.substr(0, base.size() - 4);
            } else if (base.size() == 4 && ends_with_ignore_case(base, "list")) {
                proposed_name = "item";
            } else if (ends_with_ignore_case(base, "children")) {
                proposed_name = base.substr(0, base.size() - 3);
            } else {
                proposed_name = base;
            }
        }
    }

    // remove any numbers from the proposed name
    int number = 0;
    proposed_name = split_name(proposed_name, &number);

    if (!reserved_variable_names.contains(proposed_name)) {
        reserved_variable_names.emplace(proposed_name, 0);
    }
    int count = ++reserved_variable_names[proposed_name];
    assert(!is_null_or_white_space(proposed_name));
    if (count > 1) {
        return proposed_name + std::to_string(count);
    } else {
        return proposed_name;
    }
}

// ---- the visitor

Unit AssignVariableNames::Visitor::visit_children(ILInstruction& inst, VariableScope* context) {
    for (ILInstruction* child : inst.children()) {
        accept_visitor<VariableScope*, Unit>(*child, *this, context);
    }

    return Unit{};
}

Unit AssignVariableNames::Visitor::default_visit(ILInstruction& inst, VariableScope* context) {
    if (auto* with_variable = dynamic_cast<IInstructionWithVariableOperand*>(&inst)) {
        ILVariable* v = with_variable->variable();
        // if there is already a valid name for the variable slot, just use it
        std::optional<std::string> name = context->try_get_existing_name(v);
        if (name.has_value() && !name->empty()) {
            v->name = *name;
            return visit_children(inst, context);
        }

        if (v->kind() == VariableKind::Parameter && !v->has_generated_name && v->function()->kind() == ILFunctionKind::TopLevelFunction) {
            // Parameter names of top-level functions are handled in ILReader.CreateILVariable
            // and CSharpDecompiler.FixParameterNames
        } else if (v->kind() == VariableKind::InitializerTarget /* keep generated names */ || v->kind() == VariableKind::NamedArgument) {
            context->reserve_variable_name(v->name.value_or(""));
        } else if (v->kind() == VariableKind::UsingLocal && v->address_count() == 0 && v->load_count() == 0) {
            // using variables that are not read, will not be declared in source source
        } else if (v->kind() == VariableKind::DisplayClassLocal) {
            v->name = context->next_display_class_local();
        } else if (v->kind() == VariableKind::Local && v->index().has_value()) {
            name = context->try_get_existing_name(v->function(), *v->index());
            if (name.has_value()) {
                // make sure all local ILVariables that refer to the same slot in the locals signature
                // are assigned the same name.
                v->name = *name;
            } else {
                v->name = context->assign_name(v);
                context->assign_name_to_local_signature_index(v->function(), *v->index(), *v->name);
            }
        } else {
            v->name = context->assign_name(v);
        }
    }

    return visit_children(inst, context);
}

Unit AssignVariableNames::Visitor::visit_il_function(ILFunction& function, VariableScope* context) {
    owner_.work_list_.emplace_back(&function, context);
    return Unit{};
}

Unit AssignVariableNames::Visitor::visit_call(Call& inst, VariableScope* context) {
    if (const auto* m = dynamic_cast<const typesystem::LocalFunctionMethod*>(inst.method)) {
        std::optional<std::string> name = context->try_get_existing_name_of_local_function(m->metadata_token().token());
        if (name.has_value() && !name->empty()) m->set_name(*name);
    }

    return ILVisitorCT<VariableScope*, Unit>::visit_call(inst, context);
}

Unit AssignVariableNames::Visitor::visit_ld_ftn(LdFtn& inst, VariableScope* context) {
    if (const auto* m = dynamic_cast<const typesystem::LocalFunctionMethod*>(inst.method())) {
        std::optional<std::string> name = context->try_get_existing_name_of_local_function(m->metadata_token().token());
        if (name.has_value() && !name->empty()) m->set_name(*name);
    }

    return ILVisitorCT<VariableScope*, Unit>::visit_ld_ftn(inst, context);
}

}  // namespace cb::dotnet::decompiler::il::transforms
