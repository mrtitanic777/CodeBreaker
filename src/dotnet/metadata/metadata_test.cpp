// Tests of dotnet/metadata: every table row, heap entry, signature, custom attribute value and method body of the test
// assemblies, as text, against the golden that .NET's own reader wrote (testdata/oracle/MetadataDump; src\dotnet\testdata\build.cmd
// remakes inputs and goldens). Sample.dll is PE32 (AnyCPU), Sample64.dll PE32+ (x64).
#include "dotnet/metadata/dump.hpp"

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "dotnet/metadata/signature_decoder.hpp"

namespace cb::dotnet::metadata {
namespace {

std::string golden(std::string_view name) { return fs::read_text(testing::data_path(std::string("dotnet/testdata/golden/") + std::string(name) + ".metadata.txt")); }

PEReader sample(std::string_view name) { return PEReader::open(testing::data_path(std::string("dotnet/testdata/inputs/") + std::string(name) + ".dll")); }

void check_dump(std::string_view name) {
    const std::string actual = dump(sample(name));
    const std::string expected = testing::regold_golden(testing::data_path(std::string("dotnet/testdata/golden/") + std::string(name) + ".metadata.txt"), actual);
    CB_CHECK(actual == expected);
    if (actual != expected) {   // the first line that differs, to say where
        std::size_t a = 0, e = 0;
        while (true) {
            const std::size_t an = actual.find('\n', a), en = expected.find('\n', e);
            const std::string_view al = std::string_view(actual).substr(a, an - a), el = std::string_view(expected).substr(e, en - e);
            if (al != el || an == std::string::npos || en == std::string::npos) {
                testing::record_failure(__FILE__, __LINE__, std::format("first difference:\n  ours:   {}\n  oracle: {}", al, el));
                break;
            }
            a = an + 1;
            e = en + 1;
        }
    }
}

CB_TEST("dotnet.metadata.dump_pe32_matches_system_reflection_metadata", [] { check_dump("Sample"); });
CB_TEST("dotnet.metadata.dump_pe32plus_matches_system_reflection_metadata", [] { check_dump("Sample64"); });

CB_TEST("dotnet.metadata.reader_basics", [] {
    const PEReader pe = sample("Sample64");
    CB_CHECK(pe.pe_image().header().is_plus);
    const MetadataReader& md = pe.metadata();
    CB_CHECK_EQ(md.metadata_version(), std::string("v4.0.30319"));
    CB_CHECK(md.is_assembly());
    CB_CHECK_EQ(std::string(md.get_string(md.get_assembly_definition().name())), std::string("Sample"));
    // Circle: a type with fields, methods, a property list, an event and an interface
    TypeDefinitionHandle circle;
    for (const TypeDefinitionHandle h : md.type_definitions())
        if (md.get_string(md.get_type_definition(h).name()) == "Circle") circle = h;
    CB_CHECK(!circle.is_nil());
    const TypeDefinition t = md.get_type_definition(circle);
    CB_CHECK_EQ(std::string(md.get_string(t.namespace_())), std::string("Sample"));
    CB_CHECK(t.get_fields().size() >= 4);
    for (const FieldDefinitionHandle f : t.get_fields()) CB_CHECK(md.get_field_definition(f).get_declaring_type() == circle);
    for (const MethodDefinitionHandle m : t.get_methods()) CB_CHECK(md.get_method_definition(m).get_declaring_type() == circle);
    CB_CHECK_EQ(t.get_interface_implementations().size(), std::size_t{2});
    CB_CHECK_EQ(t.get_events().size(), std::uint32_t{1});
});

CB_TEST("dotnet.metadata.compressed_integers", [] {
    const Bytes b = {0x03, 0x7F, 0x80, 0x80, 0xAE, 0x57, 0xBF, 0xFF, 0xC0, 0x00, 0x40, 0x00, 0xDF, 0xFF, 0xFF, 0xFF};
    BlobReader r(b);
    CB_CHECK_EQ(r.read_compressed_integer(), 0x03);
    CB_CHECK_EQ(r.read_compressed_integer(), 0x7F);
    CB_CHECK_EQ(r.read_compressed_integer(), 0x80);
    CB_CHECK_EQ(r.read_compressed_integer(), 0x2E57);
    CB_CHECK_EQ(r.read_compressed_integer(), 0x3FFF);
    CB_CHECK_EQ(r.read_compressed_integer(), 0x4000);
    CB_CHECK_EQ(r.read_compressed_integer(), 0x1FFFFFFF);
    // II.23.2's signed examples
    const Bytes s = {0x06, 0x7B, 0x80, 0x80, 0x01, 0xC0, 0x00, 0x40, 0x00, 0x80, 0x01, 0xDF, 0xFF, 0xFF, 0xFE, 0xC0, 0x00, 0x00, 0x01};
    BlobReader rs(s);
    CB_CHECK_EQ(rs.read_compressed_signed_integer(), 3);
    CB_CHECK_EQ(rs.read_compressed_signed_integer(), -3);
    CB_CHECK_EQ(rs.read_compressed_signed_integer(), 64);
    CB_CHECK_EQ(rs.read_compressed_signed_integer(), -64);
    CB_CHECK_EQ(rs.read_compressed_signed_integer(), 8192);
    CB_CHECK_EQ(rs.read_compressed_signed_integer(), -8192);
    CB_CHECK_EQ(rs.read_compressed_signed_integer(), 268435455);
    CB_CHECK_EQ(rs.read_compressed_signed_integer(), -268435456);
    const Bytes bad = {0xE0};
    BlobReader rb(bad);
    CB_CHECK_THROWS_AS(BadImageFormatError, rb.read_compressed_integer());
});

CB_TEST("dotnet.metadata.not_metadata", [] {
    const Bytes junk = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    CB_CHECK_THROWS_AS(BadImageFormatError, MetadataReader{junk});
});

}  // namespace
}  // namespace cb::dotnet::metadata
