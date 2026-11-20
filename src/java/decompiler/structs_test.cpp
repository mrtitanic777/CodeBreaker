// Tests of the decompiler's struct layer (structs, constant_pool, attributes, generics, var_type): every class of a jar read as the original Fernflower
// reads it. The golden is the original's own dump (work: oracle/StructDump.java run on Fernflower built from its source), the same text this file
// writes: the pool resolved, the attributes, the generic signatures parsed, the descriptors.
#include "java/decompiler/structs.hpp"

#include <bit>
#include <cstdio>
#include <format>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "java/decompiler/code_constants.hpp"
#include "java/decompiler/generics.hpp"

namespace cb::java::decompiler {
namespace {

std::string sd_opt(const std::optional<std::string>& s) { return s.value_or("null"); }
std::string sd_bool(bool b) { return b ? "true" : "false"; }

std::string sd_g(const GenericType* t) {
    if (t == nullptr) return "null";
    std::string b = std::format("{}/{}/{}", t->type, t->array_dim, sd_opt(t->value));
    for (const GenericType& e : t->enclosing_classes) b += " enc(" + sd_g(&e) + ")";
    if (!t->arguments.empty()) {
        b += " <";
        for (std::size_t i = 0; i < t->arguments.size(); ++i) b += std::format("{}{}:{}", i > 0 ? ", " : "", t->wildcards[i], sd_g(t->arguments[i].get()));
        b += '>';
    }
    return b;
}

std::string sd_gl(const std::vector<GenericType>& l) {
    std::string b = "[";
    for (std::size_t i = 0; i < l.size(); ++i) b += (i > 0 ? "; " : "") + sd_g(&l[i]);
    return b + "]";
}

std::string sd_list(const std::vector<std::string>& l) { return "[" + text::join(l, ", ") + "]"; }

void sd_pool(std::string& out, const ConstantPool& pool) {
    for (std::size_t i = 1; i < pool.size(); ++i) {
        const PooledConstant* k = pool.constant(i);
        if (k == nullptr) continue;
        if (const auto* p = dynamic_cast<const PrimitiveConstant*>(k)) {
            std::string v;
            if (const float* f = std::get_if<float>(&p->value)) v = std::format("{:x}", std::bit_cast<std::uint32_t>(*f));
            else if (const double* d = std::get_if<double>(&p->value)) v = std::format("{:x}", std::bit_cast<std::uint64_t>(*d));
            else if (const auto* s = std::get_if<std::string>(&p->value)) v = *s;
            else if (const auto* n = std::get_if<std::int32_t>(&p->value)) v = std::to_string(*n);
            else if (const auto* l = std::get_if<std::int64_t>(&p->value)) v = std::to_string(*l);
            else v = "null";
            out += std::format("  pool {} {} {}{}\n", i, p->type, v, p->is_array ? " array" : "");
        } else if (const auto* l = dynamic_cast<const LinkConstant*>(k)) {
            const bool no_class = l->type == cc::CONSTANT_NameAndType || l->type == cc::CONSTANT_InvokeDynamic;
            out += std::format("  pool {} {} {} {} {} {} {} {}\n", i, l->type, no_class ? "null" : l->classname, l->elementname, l->descriptor, l->param_count, sd_bool(l->is_void),
                               sd_bool(l->return_category2));
        }
    }
}

enum class SdOwner { Class, Field, Method };

void sd_attributes(std::string& out, const std::string& indent, const StructMember& m, const ConstantPool& pool, SdOwner owner) {
    for (const std::shared_ptr<Attribute>& a : m.attributes()) {
        std::string d;
        if (const auto* sf = dynamic_cast<const SourceFileAttribute*>(a.get())) {
            d = sf->file_name();
        } else if (const auto* sg = dynamic_cast<const GenericSignatureAttribute*>(a.get())) {
            const std::string& s = sg->signature();
            d = s;
            if (owner == SdOwner::Class) {
                const auto c = generics::parse_class_signature(s);
                d += !c ? " => null" : std::format(" => {} {} super {} if {}", sd_list(c->fparameters), c->fbounds.size(), sd_g(&c->superclass), sd_gl(c->superinterfaces));
            } else if (owner == SdOwner::Field) {
                const auto f = generics::parse_field_signature(s);
                d += !f ? " => null" : " => " + sd_g(&f->type);
            } else {
                const auto x = generics::parse_method_signature(s);
                d += !x ? " => null"
                        : std::format(" => {} {} {} ret {} exc {}", sd_list(x->fparameters), x->fbounds.size(), sd_gl(x->params), sd_g(&x->ret), sd_gl(x->exceptions));
            }
        } else if (const auto* ic = dynamic_cast<const InnerClassesAttribute*>(a.get())) {
            for (const InnerClassesAttribute::Entry& e : ic->entries())
                d += std::format(" [{} {} {} {} {} {} {}]", e.inner_class, e.outer_class, e.inner_name, e.access, e.inner_class_name, sd_opt(e.outer_class_name), sd_opt(e.simple_name));
        } else if (const auto* em = dynamic_cast<const EnclosingMethodAttribute*>(a.get())) {
            d = em->class_name() + " " + sd_opt(em->method_name()) + " " + sd_opt(em->method_descriptor());
        } else if (const auto* ln = dynamic_cast<const LineNumberTableAttribute*>(a.get())) {
            std::vector<std::string> v;
            for (const int x : ln->raw_data()) v.push_back(std::to_string(x));
            d = sd_list(v);
        } else if (const auto* lv = dynamic_cast<const LocalVariableTableAttribute*>(a.get())) {
            std::vector<std::string> v;
            for (const auto& [i, n] : lv->var_names()) v.push_back(std::format("{}={}", i, n));
            d = "{" + text::join(v, ", ") + "}";
        } else if (const auto* ex = dynamic_cast<const ExceptionsAttribute*>(a.get())) {
            for (std::size_t i = 0; i < ex->throws_exceptions().size(); ++i) d += " " + ex->exception_class_name(i, pool);
        } else if (const auto* cv = dynamic_cast<const ConstantValueAttribute*>(a.get())) {
            d = std::to_string(cv->index());
        } else if (const auto* bm = dynamic_cast<const BootstrapMethodsAttribute*>(a.get())) {
            for (std::size_t i = 0; i < bm->methods_number(); ++i) d += std::format(" [{} {}]", bm->method_reference(i)->elementname, bm->method_arguments(i).size());
        }
        out += indent + "attr " + a->name() + (d.empty() ? "" : " " + d) + "\n";
    }
}

std::string sd_dump(const StructContext& ctx) {
    std::string out;
    for (const auto& [name, c] : ctx.classes()) {
        const ConstantPool& pool = c->pool();
        out += std::format("class {} access={} super={} bc={} ge15={} ge17={} synthetic={}\n", c->qualified_name, c->access_flags(),
                           c->super_class != nullptr ? c->super_class->get_string() : "null", c->bytecode_version(), sd_bool(c->is_version_ge_1_5()),
                           sd_bool(c->is_version_ge_1_7()), sd_bool(c->is_synthetic()));
        for (const std::string& i : c->interface_names()) out += "  interface " + i + "\n";
        sd_pool(out, pool);
        sd_attributes(out, "  ", *c, pool, SdOwner::Class);
        for (const auto& f : c->fields()) {
            out += std::format("  field {} {} access={} type={}\n", f->name(), f->descriptor(), f->access_flags(), FieldDescriptor::parse(f->descriptor()).type.to_string());
            sd_attributes(out, "    ", *f, pool, SdOwner::Field);
        }
        for (const auto& m : c->methods()) {
            const MethodDescriptor md = MethodDescriptor::parse(m->descriptor());
            std::vector<std::string> params;
            for (const VarType& p : md.params) params.push_back(p.to_string());
            out += std::format("  method {} {} access={} code={} locals={} params={} ret={}\n", m->name(), m->descriptor(), m->access_flags(), sd_bool(m->contains_code()),
                               m->local_variables(), sd_list(params), md.ret.to_string());
            sd_attributes(out, "    ", *m, pool, SdOwner::Method);
        }
    }
    return out;
}

CB_TEST("java.decompiler.structs.every_class_of_a_jar_is_read_as_the_original_reads_it", [] {
    StructContext ctx([](const std::string& m, const std::exception& e) { std::fprintf(stderr, "%s\n%s\n", m.c_str(), e.what()); });
    ctx.add_space(testing::data_path("java/retromcp/gradle/wrapper/gradle-wrapper.jar"), true);
    CB_CHECK_EQ(ctx.classes().size(), 31u);
    const std::string got = sd_dump(ctx);
    const std::string want = fs::read_text(testing::data_path("java/decompiler/testdata/struct/gradle-wrapper.txt"));   // (as the original wrote it: '\n' lines)
    if (got != want) fs::write_text(std::filesystem::temp_directory_path() / "cb-struct-dump.txt", got);   // (to diff)
    CB_CHECK(got == want);
    // a larger reference when it is there: the decompile engine's jar and its own dump (work/fernflower-src)
    const std::filesystem::path big = testing::data_path("../work/fernflower-src/retromcp.struct.txt");
    std::error_code ec;
    if (std::filesystem::is_regular_file(big, ec)) {
        StructContext all;
        all.add_space(testing::data_path("../work/fernflower-src/retromcp.jar"), true);   // (the jar the dump was made of, kept beside it)
        const std::string big_got = sd_dump(all);
        const std::string big_want = fs::read_text(big);
        if (big_got != big_want) fs::write_text(std::filesystem::temp_directory_path() / "cb-struct-dump-big.txt", big_got);
        CB_CHECK(big_got == big_want);
    }
});

CB_TEST("java.decompiler.structs.types_and_descriptors", [] {
    const VarType t = VarType::parse("[[Ljava/lang/String;");
    CB_CHECK(t.type == cc::TYPE_OBJECT && t.array_dim == 2 && t.value == "java/lang/String" && t.to_string() == "[[Ljava/lang/String;");
    CB_CHECK(VarType::parse("J").stack_size == 2 && VarType::parse("J").type_family == cc::TYPE_FAMILY_LONG);
    CB_CHECK(VarType::parse("Foo", true).type == cc::TYPE_OBJECT);
    const MethodDescriptor m = MethodDescriptor::parse("(I[JLjava/lang/Object;[[Z)V");
    CB_CHECK(m.params.size() == 4 && m.params[1].to_string() == "[J" && m.params[3].array_dim == 2 && m.ret == VarType::VOID);
    CB_CHECK(VarType::INT.is_strict_superset(VarType::SHORT) && VarType::INT.is_strict_superset(VarType::BOOLEAN) && !VarType::SHORT.is_strict_superset(VarType::CHAR));
    CB_CHECK(VarType::common_supertype(VarType::SHORTCHAR, VarType::BYTE) == VarType::SHORT);
    CB_CHECK(VarType::common_min_type(VarType::CHAR, VarType::SHORT) == VarType::SHORTCHAR);
    CB_CHECK(!VarType::common_supertype(VarType::INT, VarType::LONG));
    const auto renamed = m.build_new_descriptor([](const std::string& n) -> std::optional<std::string> { return n == "java/lang/Object" ? std::optional<std::string>("a/B") : std::nullopt; });
    CB_CHECK_EQ(renamed.value_or(""), "(I[JLa/B;[[Z)V");
    // generics written as source
    const auto f = generics::parse_field_signature("Ljava/util/Map<Ljava/lang/String;+Ljava/util/List<TT;>;>;");
    CB_CHECK(f.has_value());
    generics::NameContext names{[](const std::string& n) { return n.substr(n.rfind('.') + 1); }, {}};
    CB_CHECK_EQ(generics::cast_type_name(f->type, names), "Map<String, ? extends List<T>>");
    CB_CHECK(!generics::parse_method_signature("(I").has_value());
});

}  // namespace
}  // namespace cb::java::decompiler
