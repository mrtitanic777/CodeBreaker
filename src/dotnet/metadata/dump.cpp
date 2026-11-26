// dotnet/metadata/dump.cpp -- the metadata as text (testdata/oracle/MetadataDump/MetadataDump.cs writes the same, from .NET's
// reader; a change to one is a change to both).
#include "dotnet/metadata/dump.hpp"

#include <bit>
#include <format>

#include "core/text.hpp"
#include "dotnet/metadata/custom_attribute_decoder.hpp"
#include "dotnet/metadata/signature_decoder.hpp"

namespace cb::dotnet::metadata {
namespace {

// A type as the dump spells it (a struct, not a std::string, so a custom attribute's value variant tells a type from a string).
struct TypeName {
    std::string s;
};

std::string tok(EntityHandle h) { return h.is_nil() ? "nil" : std::format("{:08x}", h.token()); }

std::string hex(ByteView b) {
    if (b.empty()) return "-";
    std::string s;
    s.reserve(b.size() * 2);
    for (const std::uint8_t x : b) s += std::format("{:02x}", x);
    return s;
}

// UTF-16 code units as the oracle escapes them: printable ASCII as itself, \ and " escaped, the rest \uXXXX.
std::string esc16(const std::u16string& s) {
    std::string out = "\"";
    for (const char16_t c : s) {
        if (c == u'\\' || c == u'"') {
            out += '\\';
            out += static_cast<char>(c);
        } else if (c < 0x20 || c > 0x7E) {
            out += std::format("\\u{:04x}", static_cast<unsigned>(c));
        } else {
            out += static_cast<char>(c);
        }
    }
    return out + "\"";
}

// UTF-8 to UTF-16 as .NET decodes it (an invalid sequence is U+FFFD).
std::u16string utf16(std::string_view s) {
    std::u16string out;
    std::size_t i = 0;
    while (i < s.size()) {
        const auto b0 = static_cast<std::uint8_t>(s[i]);
        char32_t cp = 0xFFFD;
        std::size_t n = 1;
        auto cont = [&](std::size_t k) { return i + k < s.size() && (static_cast<std::uint8_t>(s[i + k]) & 0xC0) == 0x80; };
        if (b0 < 0x80) cp = b0;
        else if ((b0 & 0xE0) == 0xC0 && b0 >= 0xC2 && cont(1)) { cp = (b0 & 0x1F) << 6 | (s[i + 1] & 0x3F); n = 2; }
        else if ((b0 & 0xF0) == 0xE0 && cont(1) && cont(2)) {
            cp = (b0 & 0x0F) << 12 | (s[i + 1] & 0x3F) << 6 | (s[i + 2] & 0x3F);
            n = 3;
            if (cp < 0x800 || (cp >= 0xD800 && cp <= 0xDFFF)) { cp = 0xFFFD; n = 1; }
        } else if ((b0 & 0xF8) == 0xF0 && cont(1) && cont(2) && cont(3)) {
            cp = (b0 & 0x07) << 18 | (s[i + 1] & 0x3F) << 12 | (s[i + 2] & 0x3F) << 6 | (s[i + 3] & 0x3F);
            n = 4;
            if (cp < 0x10000 || cp > 0x10FFFF) { cp = 0xFFFD; n = 1; }
        }
        if (cp >= 0x10000) {
            out.push_back(static_cast<char16_t>(0xD800 + ((cp - 0x10000) >> 10)));
            out.push_back(static_cast<char16_t>(0xDC00 + ((cp - 0x10000) & 0x3FF)));
        } else {
            out.push_back(static_cast<char16_t>(cp));
        }
        i += n;
    }
    return out;
}

std::string esc(std::string_view utf8) { return esc16(utf16(utf8)); }

class Dumper {
public:
    explicit Dumper(const PEReader& pe) : pe_(pe), md_(pe.metadata()) {}

    // ---- the provider (signature and custom attribute sides)
    TypeName get_primitive_type(PrimitiveTypeCode t) {
        switch (t) {
        case PrimitiveTypeCode::Void: return {"void"};
        case PrimitiveTypeCode::Boolean: return {"bool"};
        case PrimitiveTypeCode::Char: return {"char"};
        case PrimitiveTypeCode::SByte: return {"int8"};
        case PrimitiveTypeCode::Byte: return {"uint8"};
        case PrimitiveTypeCode::Int16: return {"int16"};
        case PrimitiveTypeCode::UInt16: return {"uint16"};
        case PrimitiveTypeCode::Int32: return {"int32"};
        case PrimitiveTypeCode::UInt32: return {"uint32"};
        case PrimitiveTypeCode::Int64: return {"int64"};
        case PrimitiveTypeCode::UInt64: return {"uint64"};
        case PrimitiveTypeCode::Single: return {"float32"};
        case PrimitiveTypeCode::Double: return {"float64"};
        case PrimitiveTypeCode::String: return {"string"};
        case PrimitiveTypeCode::TypedReference: return {"typedref"};
        case PrimitiveTypeCode::IntPtr: return {"intptr"};
        case PrimitiveTypeCode::UIntPtr: return {"uintptr"};
        case PrimitiveTypeCode::Object: return {"object"};
        }
        return {std::format("?{}", static_cast<int>(t))};
    }
    TypeName get_type_from_definition(const MetadataReader&, TypeDefinitionHandle h, std::uint8_t kind) {
        return {"T:" + tok(h) + std::format("@{:02x}", kind)};
    }
    TypeName get_type_from_reference(const MetadataReader&, TypeReferenceHandle h, std::uint8_t kind) {
        return {"T:" + tok(h) + std::format("@{:02x}", kind)};
    }
    TypeName get_type_from_specification(const MetadataReader&, const int&, TypeSpecificationHandle h, std::uint8_t kind) {
        return {"S:" + tok(h) + std::format("@{:02x}", kind)};
    }
    TypeName get_sz_array_type(TypeName e) { return {e.s + "[]"}; }
    TypeName get_array_type(TypeName e, const ArrayShape& s) {
        std::vector<std::string> sizes, bounds;
        for (const int x : s.sizes) sizes.push_back(std::to_string(x));
        for (const int x : s.lower_bounds) bounds.push_back(std::to_string(x));
        return {std::format("{}[{};{};{}]", e.s, s.rank, text::join(sizes, ","), text::join(bounds, ","))};
    }
    TypeName get_pointer_type(TypeName e) { return {e.s + "*"}; }
    TypeName get_by_reference_type(TypeName e) { return {e.s + "&"}; }
    TypeName get_pinned_type(TypeName e) { return {"pinned(" + e.s + ")"}; }
    TypeName get_modified_type(TypeName m, TypeName t, bool required) { return {t.s + (required ? " modreq(" : " modopt(") + m.s + ")"}; }
    TypeName get_generic_instantiation(TypeName g, const std::vector<TypeName>& args) {
        std::vector<std::string> a;
        for (const TypeName& x : args) a.push_back(x.s);
        return {g.s + "<" + text::join(a, ",") + ">"};
    }
    TypeName get_generic_type_parameter(const int&, int i) { return {"!" + std::to_string(i)}; }
    TypeName get_generic_method_parameter(const int&, int i) { return {"!!" + std::to_string(i)}; }
    TypeName get_function_pointer_type(const MethodSignature<TypeName>& s) { return {"fnptr(" + sig(s) + ")"}; }
    TypeName get_system_type() { return {"[System.Type]"}; }
    // System.Type: the provider's own, or a reference to System.Type (as a constructor's parameter names it).
    bool is_system_type(const TypeName& t) {
        if (t.s == "[System.Type]") return true;
        if (!t.s.starts_with("T:01")) return false;
        const TypeReference r = md_.get_type_reference(TypeReferenceHandle(static_cast<std::uint32_t>(std::stoul(t.s.substr(4, 6), nullptr, 16))));
        return md_.get_string(r.namespace_()) == "System" && md_.get_string(r.name()) == "Type";
    }
    TypeName get_type_from_serialized_name(const std::string& name) { return {"ser:" + esc(name)}; }
    PrimitiveTypeCode get_underlying_enum_type(const TypeName& t) { return enum_underlying(t.s); }

    std::string run() {
        header();
        heaps();
        rows();
        return out_;
    }

private:
    void line(std::string s) {
        out_ += s;
        out_ += '\n';
    }
    std::string str(StringHandle h) const { return esc(md_.get_string(h)); }
    std::string blob(BlobHandle h) const { return h.is_nil() ? "nil" : hex(md_.get_blob_bytes(h)); }
    std::string guid(GuidHandle h) const { return md_.get_guid(h).to_string(); }

    static std::string sig(const MethodSignature<TypeName>& s) {
        std::vector<std::string> p;
        for (const TypeName& t : s.parameter_types) p.push_back(t.s);
        return std::format("{:02x} g{} r{} {}({})", s.header.raw_value(), s.generic_parameter_count, s.required_parameter_count, s.return_type.s,
                           text::join(p, ", "));
    }

    template <class List>
    static std::string toks(const List& list) {
        std::vector<std::string> s;
        for (const auto h : list) s.push_back(tok(h));
        return text::join(s, " ");
    }

    // The underlying type of an enum a custom attribute names (the oracle's rule: an enum of this module by its instance field,
    // found by token or by the serialized name; any other is int32).
    PrimitiveTypeCode enum_underlying(const std::string& t) {
        TypeDefinitionHandle def;
        if (t.starts_with("T:02")) {
            def = TypeDefinitionHandle(static_cast<std::uint32_t>(std::stoul(t.substr(4, 6), nullptr, 16)));
        } else if (t.starts_with("ser:")) {
            std::string name = t.substr(5, t.size() - 6);
            name = name.substr(0, name.find(','));
            name = std::string(text::trim(name));
            for (const TypeDefinitionHandle h : md_.type_definitions()) {
                const TypeDefinition d = md_.get_type_definition(h);
                const std::string ns(md_.get_string(d.namespace_()));
                const std::string full = ns.empty() ? std::string(md_.get_string(d.name())) : ns + "." + std::string(md_.get_string(d.name()));
                if (full == name) def = h;
            }
        }
        if (!def.is_nil())
            for (const FieldDefinitionHandle f : md_.get_type_definition(def).get_fields()) {
                const FieldDefinition fd = md_.get_field_definition(f);
                if ((fd.attributes() & 0x10) == 0) {   // not static: value__
                    BlobReader r = md_.get_blob_reader(fd.signature());
                    r.read_signature_header();
                    return static_cast<PrimitiveTypeCode>(r.read_byte());
                }
            }
        return PrimitiveTypeCode::Int32;
    }

    std::string value(const CustomAttributeTypedArgument<TypeName>& a) {
        return a.type.s + ":" + value_text(a.value);
    }
    std::string value_text(const CustomAttributeArgumentValue<TypeName>& v) {
        return std::visit(
            [&](const auto& x) -> std::string {
                using X = std::decay_t<decltype(x)>;
                if constexpr (std::is_same_v<X, std::monostate>) return "null";
                else if constexpr (std::is_same_v<X, std::string>) return esc(x);
                else if constexpr (std::is_same_v<X, TypeName>) return esc(x.s);   // a type argument: the provider's text, a string upstream
                else if constexpr (std::is_same_v<X, bool>) return x ? "true" : "false";
                else if constexpr (std::is_same_v<X, char16_t>) return std::to_string(static_cast<int>(x));
                else if constexpr (std::is_same_v<X, float>) return std::format("f{:08x}", std::bit_cast<std::uint32_t>(x));
                else if constexpr (std::is_same_v<X, double>) return std::format("d{:016x}", std::bit_cast<std::uint64_t>(x));
                else if constexpr (std::is_same_v<X, std::vector<CustomAttributeTypedArgument<TypeName>>>) {
                    std::vector<std::string> items;
                    for (const auto& e : x) items.push_back(value(e));
                    return "[" + text::join(items, ", ") + "]";
                } else if constexpr (std::is_same_v<X, std::shared_ptr<const CustomAttributeTypedArgument<TypeName>>>) {
                    return x ? value(*x) : "null";
                } else {
                    return std::to_string(x);
                }
            },
            v);
    }

    void header() {
        const CorHeader& c = pe_.cor_header();
        line(std::format("cor runtime={}.{} metadata={:x}+{:x} flags={:x} entry={:08x} resources={:x}+{:x} strongname={:x}+{:x}", c.major_runtime_version,
                         c.minor_runtime_version, c.metadata_directory.relative_virtual_address, c.metadata_directory.size, c.flags,
                         c.entry_point_token_or_relative_virtual_address, c.resources_directory.relative_virtual_address, c.resources_directory.size,
                         c.strong_name_signature_directory.relative_virtual_address, c.strong_name_signature_directory.size));
        line(std::format("metadata version={} kind={} assembly={}", esc(md_.metadata_version()), static_cast<int>(md_.metadata_kind()), md_.is_assembly() ? 1 : 0));
        const std::pair<HeapIndex, const char*> heaps[] = {
            {HeapIndex::UserString, "UserString"}, {HeapIndex::String, "String"}, {HeapIndex::Blob, "Blob"}, {HeapIndex::Guid, "Guid"}};
        for (const auto& [h, name] : heaps)
            line(std::format("heap {} offset={:x} size={:x}", name, md_.get_heap_metadata_offset(h), md_.get_heap_size(h)));
        for (int t = 0; t <= 0x2C; ++t) {
            const auto ti = static_cast<TableIndex>(t);
            if (md_.get_table_row_count(ti) > 0)
                line(std::format("table {:02x} rows={} rowsize={} offset={:x}", t, md_.get_table_row_count(ti), md_.get_table_row_size(ti),
                                 md_.get_table_metadata_offset(ti)));
        }
    }

    void heaps() {
        for (StringHandle h = md_.get_next_handle(StringHandle()); !h.is_nil(); h = md_.get_next_handle(h))
            line(std::format("string {:x} {}", h.offset(), str(h)));
        if (md_.get_heap_size(HeapIndex::UserString) > 1)
            for (UserStringHandle h = md_.get_next_handle(UserStringHandle()); !h.is_nil(); h = md_.get_next_handle(h))
                line(std::format("userstring {:x} {}", h.offset(), esc16(md_.get_user_string(h))));
        for (BlobHandle h = md_.get_next_handle(BlobHandle()); !h.is_nil(); h = md_.get_next_handle(h))
            line(std::format("blob {:x} {}", h.offset(), blob(h)));
        for (int i = 1; i <= md_.get_heap_size(HeapIndex::Guid) / 16; ++i)
            line(std::format("guid {} {}", i, guid(GuidHandle(static_cast<std::uint32_t>(i)))));
    }

    void rows() {
        const int ctx = 0;
        const ModuleDefinition mod = md_.get_module_definition();
        line(std::format("module gen={} name={} mvid={} enc={} encbase={}", mod.generation(), str(mod.name()), guid(mod.mvid()),
                         mod.generation_id().is_nil() ? "nil" : guid(mod.generation_id()),
                         mod.base_generation_id().is_nil() ? "nil" : guid(mod.base_generation_id())));
        for (const TypeReferenceHandle h : md_.type_references()) {
            const TypeReference r = md_.get_type_reference(h);
            line(std::format("typeref {} scope={} ns={} name={}", tok(h), tok(r.resolution_scope()), str(r.namespace_()), str(r.name())));
        }
        for (const TypeDefinitionHandle h : md_.type_definitions()) {
            const TypeDefinition d = md_.get_type_definition(h);
            const TypeLayout layout = d.get_layout();
            line(std::format("typedef {} flags={:x} ns={} name={} base={} declaring={} layout={},{}", tok(h), d.attributes(), str(d.namespace_()),
                             str(d.name()), tok(d.base_type()), tok(d.get_declaring_type()), layout.packing_size, layout.size));
            line("  fields " + toks(d.get_fields()));
            line("  methods " + toks(d.get_methods()));
            line("  properties " + toks(d.get_properties()));
            line("  events " + toks(d.get_events()));
            line("  nested " + toks(d.get_nested_types()));
            std::vector<std::string> ifaces;
            for (const InterfaceImplementationHandle i : d.get_interface_implementations())
                ifaces.push_back(tok(i) + "=" + tok(md_.get_interface_implementation(i).interface_()));
            line("  interfaces " + text::join(ifaces, " "));
            line("  generic " + toks(d.get_generic_parameters()));
            std::vector<std::string> impls;
            for (const MethodImplementationHandle i : d.get_method_implementations()) {
                const MethodImplementation mi = md_.get_method_implementation(i);
                impls.push_back(tok(i) + "=" + tok(mi.method_body()) + "/" + tok(mi.method_declaration()));
            }
            line("  methodimpls " + text::join(impls, " "));
            line("  security " + toks(d.get_declarative_security_attributes()));
        }
        for (const FieldDefinitionHandle h : md_.field_definitions()) {
            const FieldDefinition f = md_.get_field_definition(h);
            line(std::format("field {} flags={:x} name={} sig={} owner={} const={} offset={} rva={:x} marshal={}", tok(h), f.attributes(), str(f.name()),
                             f.decode_signature(*this, ctx).s, tok(f.get_declaring_type()), tok(f.get_default_value()), f.get_offset(),
                             static_cast<std::uint32_t>(f.get_relative_virtual_address()), blob(f.get_marshalling_descriptor())));
        }
        for (const MethodDefinitionHandle h : md_.method_definitions()) {
            const MethodDefinition m = md_.get_method_definition(h);
            const MethodImport imp = m.get_import();
            line(std::format("method {} flags={:x} impl={:x} rva={:x} name={} sig={} owner={}", tok(h), m.attributes(), m.impl_attributes(),
                             static_cast<std::uint32_t>(m.relative_virtual_address()), str(m.name()), sig(m.decode_signature(*this, ctx)),
                             tok(m.get_declaring_type())));
            line("  params " + toks(m.get_parameters()));
            line("  generic " + toks(m.get_generic_parameters()));
            line("  security " + toks(m.get_declarative_security_attributes()));
            if (!imp.module.is_nil()) line(std::format("  import flags={:x} name={} module={}", imp.attributes, str(imp.name), tok(imp.module)));
            if (m.relative_virtual_address() != 0) {
                std::optional<MethodBodyBlock> body;
                try {
                    body = pe_.get_method_body(static_cast<std::uint32_t>(m.relative_virtual_address()));
                } catch (const BadImageFormatError&) {
                    line("  body undecodable");   // (a native body: C++/CLI)
                    continue;
                }
                const MethodBodyBlock& b = *body;
                line(std::format("  body maxstack={} init={} locals={} size={} il={}", b.max_stack(), b.local_variables_initialized() ? 1 : 0,
                                 tok(b.local_signature()), b.size(), hex(b.il_bytes())));
                for (const ExceptionRegion& r : b.exception_regions())
                    line(std::format("  region {} try={:x}+{:x} handler={:x}+{:x} catch={} filter={:x}", static_cast<int>(r.kind),
                                     static_cast<std::uint32_t>(r.try_offset), static_cast<std::uint32_t>(r.try_length),
                                     static_cast<std::uint32_t>(r.handler_offset), static_cast<std::uint32_t>(r.handler_length), tok(r.catch_type),
                                     static_cast<std::uint32_t>(r.filter_offset)));
            }
        }
        for (std::uint32_t i = 1; i <= md_.row_count(TableIndex::Param); ++i) {
            const ParameterHandle h(i);
            const Parameter p = md_.get_parameter(h);
            line(std::format("param {} flags={:x} seq={} name={} const={} marshal={}", tok(h), p.attributes(), p.sequence_number(), str(p.name()),
                             tok(p.get_default_value()), blob(p.get_marshalling_descriptor())));
        }
        for (const MemberReferenceHandle h : md_.member_references()) {
            const MemberReference r = md_.get_member_reference(h);
            const std::string s = r.get_kind() == MemberReferenceKind::Method ? sig(r.decode_method_signature(*this, ctx)) : r.decode_field_signature(*this, ctx).s;
            line(std::format("memberref {} parent={} name={} kind={} sig={}", tok(h), tok(r.parent()), str(r.name()), static_cast<int>(r.get_kind()), s));
        }
        for (std::uint32_t i = 1; i <= md_.row_count(TableIndex::Constant); ++i) {
            const ConstantHandle h(i);
            const Constant c = md_.get_constant(h);
            line(std::format("constant {} type={:02x} parent={} value={}", tok(h), static_cast<int>(c.type_code()), tok(c.parent()), blob(c.value())));
        }
        for (const CustomAttributeHandle h : md_.custom_attributes()) {
            const CustomAttribute a = md_.get_custom_attribute(h);
            std::string v;
            try {
                const CustomAttributeValue<TypeName> d = a.decode_value(*this);
                std::vector<std::string> fixed, named;
                for (const auto& f : d.fixed_arguments) fixed.push_back(value(f));
                for (const auto& n : d.named_arguments)
                    named.push_back(std::string(n.kind == CustomAttributeNamedArgumentKind::Field ? "F " : "P ") + n.name.value_or("") + "=" + n.type.s + ":" +
                                    value_text(n.value));
                v = "(" + text::join(fixed, ", ") + ") {" + text::join(named, ", ") + "}";
            } catch (const BadImageFormatError&) {
                v = "undecodable";
            }
            line(std::format("attribute {} parent={} ctor={} blob={} value={}", tok(h), tok(a.parent()), tok(a.constructor()), blob(a.value()), v));
        }
        for (const DeclarativeSecurityAttributeHandle h : md_.declarative_security_attributes()) {
            const DeclarativeSecurityAttribute s = md_.get_declarative_security_attribute(h);
            line(std::format("security {} action={} parent={} blob={}", tok(h), s.action(), tok(s.parent()), blob(s.permission_set())));
        }
        for (std::uint32_t i = 1; i <= md_.row_count(TableIndex::StandAloneSig); ++i) {
            const StandaloneSignatureHandle h(i);
            const StandaloneSignature s = md_.get_standalone_signature(h);
            std::string text;
            try {
                if (s.get_kind() == StandaloneSignatureKind::LocalVariables) {
                    std::vector<std::string> locals;
                    for (const TypeName& t : s.decode_local_signature(*this, ctx)) locals.push_back(t.s);
                    text = "locals(" + text::join(locals, ", ") + ")";
                } else {
                    text = sig(s.decode_method_signature(*this, ctx));
                }
            } catch (const BadImageFormatError&) {
                text = "undecodable";   // (old compilers wrote field signatures here)
            }
            line(std::format("standalonesig {} sig={}", tok(h), text));
        }
        for (const EventDefinitionHandle h : md_.event_definitions()) {
            const EventDefinition e = md_.get_event_definition(h);
            const EventAccessors acc = e.get_accessors();
            std::vector<std::string> others;
            for (const auto m : acc.others) others.push_back(tok(m));
            line(std::format("event {} flags={:x} name={} type={} add={} remove={} raise={} others={}", tok(h), e.attributes(), str(e.name()), tok(e.type()),
                             tok(acc.adder), tok(acc.remover), tok(acc.raiser), text::join(others, ",")));
        }
        for (const PropertyDefinitionHandle h : md_.property_definitions()) {
            const PropertyDefinition p = md_.get_property_definition(h);
            const PropertyAccessors acc = p.get_accessors();
            std::vector<std::string> others;
            for (const auto m : acc.others) others.push_back(tok(m));
            line(std::format("property {} flags={:x} name={} sig={} const={} get={} set={} others={}", tok(h), p.attributes(), str(p.name()),
                             sig(p.decode_signature(*this, ctx)), tok(p.get_default_value()), tok(acc.getter), tok(acc.setter), text::join(others, ",")));
        }
        for (const ModuleReferenceHandle h : md_.module_references())
            line(std::format("moduleref {} name={}", tok(h), str(md_.get_module_reference(h).name())));
        for (std::uint32_t i = 1; i <= md_.row_count(TableIndex::TypeSpec); ++i) {
            const TypeSpecificationHandle h(i);
            line(std::format("typespec {} sig={}", tok(h), md_.get_type_specification(h).decode_signature(*this, ctx).s));
        }
        if (md_.is_assembly()) {
            const AssemblyDefinition a = md_.get_assembly_definition();
            line(std::format("assembly name={} version={} culture={} flags={:x} hash={:x} key={}", str(a.name()), a.version().to_string(), str(a.culture()),
                             a.flags(), a.hash_algorithm(), blob(a.public_key())));
        }
        for (const AssemblyReferenceHandle h : md_.assembly_references()) {
            const AssemblyReference a = md_.get_assembly_reference(h);
            line(std::format("assemblyref {} name={} version={} culture={} flags={:x} key={} hash={}", tok(h), str(a.name()), a.version().to_string(),
                             str(a.culture()), a.flags(), blob(a.public_key_or_token()), blob(a.hash_value())));
        }
        for (const AssemblyFileHandle h : md_.assembly_files()) {
            const AssemblyFile f = md_.get_assembly_file(h);
            line(std::format("file {} name={} metadata={} hash={}", tok(h), str(f.name()), f.contains_metadata() ? 1 : 0, blob(f.hash_value())));
        }
        for (const ExportedTypeHandle h : md_.exported_types()) {
            const ExportedType t = md_.get_exported_type(h);
            line(std::format("exportedtype {} flags={:x} ns={} name={} impl={} typedef={:x}", tok(h), t.attributes(), str(t.namespace_()), str(t.name()),
                             tok(t.implementation()), static_cast<std::uint32_t>(t.get_type_definition_id())));
        }
        for (const ManifestResourceHandle h : md_.manifest_resources()) {
            const ManifestResource r = md_.get_manifest_resource(h);
            line(std::format("resource {} name={} offset={:x} flags={:x} impl={}", tok(h), str(r.name()), r.offset(), r.attributes(), tok(r.implementation())));
        }
        for (std::uint32_t i = 1; i <= md_.row_count(TableIndex::GenericParam); ++i) {
            const GenericParameterHandle h(i);
            const GenericParameter g = md_.get_generic_parameter(h);
            std::vector<std::string> cs;
            for (const auto c : g.get_constraints()) cs.push_back(tok(c) + "=" + tok(md_.get_generic_parameter_constraint(c).type()));
            line(std::format("genericparam {} index={} flags={:x} name={} parent={} constraints={}", tok(h), g.index(), g.attributes(), str(g.name()),
                             tok(g.parent()), text::join(cs, ",")));
        }
        for (std::uint32_t i = 1; i <= md_.row_count(TableIndex::MethodSpec); ++i) {
            const MethodSpecificationHandle h(i);
            const MethodSpecification s = md_.get_method_specification(h);
            std::vector<std::string> inst;
            for (const TypeName& t : s.decode_signature(*this, ctx)) inst.push_back(t.s);
            line(std::format("methodspec {} method={} inst=<{}>", tok(h), tok(s.method()), text::join(inst, ",")));
        }
        for (const TypeDefinitionHandle h : md_.type_definitions()) line("attributes-of " + tok(h) + " " + toks(md_.get_custom_attributes(h)));
        for (const MethodDefinitionHandle h : md_.method_definitions()) line("attributes-of " + tok(h) + " " + toks(md_.get_custom_attributes(h)));
        for (const FieldDefinitionHandle h : md_.field_definitions()) line("attributes-of " + tok(h) + " " + toks(md_.get_custom_attributes(h)));
    }

    const PEReader& pe_;
    const MetadataReader& md_;
    std::string out_;
};

}  // namespace

std::string dump(const PEReader& pe) { return Dumper(pe).run(); }

}  // namespace cb::dotnet::metadata
