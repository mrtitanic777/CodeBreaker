// Tests of the bytecode tree: a class read and written back decompiles to the same Java (the Gradle wrapper's 31 classes against the original
// Fernflower's text); the instruction list edits as ASM's; types as ASM's.
#include "java/bytecode/tree.hpp"

#include <filesystem>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "java/console/zip_archive.hpp"
#include "java/decompiler/decompiler.hpp"

namespace cb::java::bytecode {
namespace {

CB_TEST("java.bytecode.tree.a_jar_written_again_decompiles_as_before", [] {
    const auto jar = java::console::ZipArchive::open(testing::data_path("java/retromcp/gradle/wrapper/gradle-wrapper.jar"));
    java::console::ZipWriter z;
    int classes = 0;
    for (const auto& e : jar.entries()) {
        Bytes data = e.is_directory() ? Bytes{} : jar.read(e);
        if (e.name.ends_with(".class")) {
            data = write(read(data, e.name));
            ++classes;
        }
        z.add(e.name, data);
    }
    CB_CHECK(classes > 30);
    fs::ScopedTempDir dir{"cb-bytecode-tree"};
    const std::filesystem::path out = dir.path() / "gradle-wrapper.jar";
    fs::write_file(out, z.finish());
    java::decompiler::DecompileRequest req;
    req.sources.push_back(out);
    req.options = {{"nco", "1"}, {"rbr", "0"}, {"asc", "1"}, {"ovr", "0"}, {"dgs", "1"}, {"ind", "\t"}};
    const auto texts = java::decompiler::decompile(req).java_texts();
    const std::filesystem::path golden = testing::data_path("java/decompiler/testdata/java/gradle-wrapper");
    int same = 0, total = 0;
    for (const auto& e : std::filesystem::recursive_directory_iterator(golden)) {
        if (!e.is_regular_file() || e.path().extension() != ".java") continue;
        ++total;
        const std::string rel = fs::to_text(std::filesystem::relative(e.path(), golden).generic_string());
        const auto it = texts.find("gradle-wrapper.jar!/" + rel);
        if (it != texts.end() && it->second == fs::read_text(e.path())) ++same;
    }
    CB_CHECK_EQ(total, 31);
    CB_CHECK_EQ(same, total);
});

CB_TEST("java.bytecode.tree.the_instruction_list_edits_as_asm", [] {
    InsnList list;
    auto* a = list.make<ZeroInsnNode>(op::NOP);
    auto* b = list.make<ZeroInsnNode>(op::POP);
    auto* c = list.make<ZeroInsnNode>(op::RETURN);
    list.add(a);
    list.add(c);
    list.insert(a, b);   // after a
    CB_CHECK_EQ(list.size(), 3);
    CB_CHECK(list.get(1) == b && b->get_previous() == a && b->get_next() == c);
    auto* d = list.make<ZeroInsnNode>(op::DUP);
    list.set(b, d);
    CB_CHECK(list.get(1) == d && !list.contains(b));
    list.remove(a);
    CB_CHECK(list.get_first() == d && list.size() == 2);
    InsnList more;
    auto* e = more.make<LabelNode>();
    more.add(e);
    list.insert_before(c, more);
    CB_CHECK(more.size() == 0 && list.get(1) == e && list.size() == 3);
    int n = 0;
    for (InsnNode* i : list) {   // removing the current node while iterating
        list.remove(i);
        ++n;
    }
    CB_CHECK(n == 3 && list.size() == 0);
});

CB_TEST("java.bytecode.tree.types_are_asms", [] {
    CB_CHECK_EQ(Type::get_type("[Ljava/lang/String;").get_class_name(), "java.lang.String[]");
    CB_CHECK_EQ(Type::get_object_type("a/B").get_descriptor(), "La/B;");
    CB_CHECK_EQ(Type::get_return_type("(IJ)D").get_sort(), int{Type::DOUBLE});
    CB_CHECK_EQ(Type::get_argument_types("(IJ[I)V").size(), std::size_t{3});
    CB_CHECK_EQ(Type::get_arguments_and_return_sizes("(IJ)D"), (1 + 1 + 2) << 2 | 2);
    CB_CHECK_EQ(Type::LONG_TYPE.get_opcode(op::ILOAD), op::LLOAD);
    CB_CHECK_EQ(Type::get_type("Ljava/lang/Object;").get_opcode(op::IRETURN), op::ARETURN);
    CB_CHECK_EQ(Type::BYTE_TYPE.get_opcode(op::IALOAD), op::BALOAD);
});

}  // namespace
}  // namespace cb::java::bytecode
