// Tests of the mappings reader: a tiny v1 file reads as the same mappings as the tiny v2 file of the same names (the v2 reader is the one RetroMCP's
// versions use, proven against the original on six of them: work/retire-retromcp/B); the namespaces of a header; the provider of a file.
#include "java/projects/minecraft/prep/remap.hpp"

#include <filesystem>
#include <memory>

#include "core/fs.hpp"
#include "core/testing.hpp"

namespace cb::java::minecraft::prep {
namespace {

// The same names: a class, a field and a method of it (with a parameter in v2), a class in a package, in both formats.
constexpr const char* kTinyV1 =
    "v1\tofficial\tnamed\n"
    "CLASS\ta\tnet/minecraft/src/Block\n"
    "CLASS\tb\tnet/minecraft/client/Minecraft\n"
    "FIELD\ta\tI\tc\tblockID\n"
    "FIELD\ta\tLb;\td\tgame\n"
    "METHOD\ta\t(Lb;)V\te\tonAdded\n";
constexpr const char* kTinyV2 =
    "tiny\t2\t0\tofficial\tnamed\n"
    "c\ta\tnet/minecraft/src/Block\n"
    "\tf\tI\tc\tblockID\n"
    "\tf\tLb;\td\tgame\n"
    "\tm\t(Lb;)V\te\tonAdded\n"
    "c\tb\tnet/minecraft/client/Minecraft\n";

CB_TEST("java.minecraft.prep.remap.tiny_v1_reads_as_tiny_v2", [] {
    fs::ScopedTempDir dir("cb-prep-remap");
    const std::filesystem::path v1 = dir.path() / "v1.tiny", v2 = dir.path() / "v2.tiny";
    fs::write_text(v1, kTinyV1);
    fs::write_text(v2, kTinyV2);
    CB_CHECK(MappingsIO::get_provider(v1) == Mappings::Provider::TINY1);
    CB_CHECK(MappingsIO::get_provider(v2) == Mappings::Provider::TINY2);
    CB_CHECK(read_namespaces(v1) == (std::vector<std::string>{"official", "named"}));
    CB_CHECK(read_namespaces(v2) == (std::vector<std::string>{"official", "named"}));
    for (const auto& path : {v1, v2}) {
        const Mappings m = MappingsIO::read(path, "official", "named");
        CB_CHECK_EQ(m.classes.size(), std::size_t{2});
        CB_CHECK(m.classes.at("a") == "net/minecraft/src/Block");
        CB_CHECK(m.classes.at("b") == "net/minecraft/client/Minecraft");
        CB_CHECK(m.fields.get("a", "I", "c") == std::optional<std::string>("blockID"));
        CB_CHECK(m.fields.get("a", "Lb;", "d") == std::optional<std::string>("game"));
        CB_CHECK(m.methods.get("a", "(Lb;)V", "e") == std::optional<std::string>("onAdded"));
        CB_CHECK(!m.methods.get("a", "()V", "e"));
        // by index (-1 the first column, 0 the next), the same
        const Mappings by_index = MappingsIO::read(path, -1, 0);
        CB_CHECK(by_index.classes.at("a") == "net/minecraft/src/Block");
        CB_CHECK(by_index.methods.get("a", "(Lb;)V", "e") == std::optional<std::string>("onAdded"));
    }
});

// A class of methods (name, descriptor; public), under `super_name`, implementing `interfaces`.
std::unique_ptr<bc::ClassNode> klass(std::string name, std::optional<std::string> super_name, std::vector<std::string> interfaces,
                                    std::vector<std::pair<std::string, std::string>> methods) {
    auto node = std::make_unique<bc::ClassNode>();
    node->name = std::move(name);
    node->super_name = std::move(super_name);
    node->interfaces = std::move(interfaces);
    for (auto& [n, d] : methods) {
        bc::MethodNode m;
        m.access = bc::op::ACC_PUBLIC;
        m.name = n;
        m.desc = d;
        node->methods.push_back(std::move(m));
    }
    return node;
}

CB_TEST("java.minecraft.prep.remap.a_name_the_mappings_give_an_implementation_is_given_the_interface_too", [] {
    std::vector<std::unique_ptr<bc::ClassNode>> nodes;
    nodes.push_back(klass("IWorldAccess", "java/lang/Object", {}, {{"a", "(III)V"}, {"b", "()V"}}));
    nodes.push_back(klass("RenderGlobal", "java/lang/Object", {"IWorldAccess"}, {{"a", "(III)V"}, {"b", "()V"}}));
    const ClassStorage storage(std::move(nodes));
    Mappings given;
    given.methods.put("RenderGlobal", "(III)V", "a", "markBlockNeedsUpdate");
    given.methods.put("IWorldAccess", "()V", "b", "loadRenderers");
    given.methods.put("RenderGlobal", "()V", "b", "loadRenderers");
    const Mappings done = completed_for_source(given, storage);
    CB_CHECK(done.methods.get("IWorldAccess", "(III)V", "a") == std::optional<std::string>("markBlockNeedsUpdate"));
    CB_CHECK(done.methods.get("IWorldAccess", "()V", "b") == std::optional<std::string>("loadRenderers"));   // (named already: as it was)
    CB_CHECK_EQ(done.methods.size(), std::size_t{4});
});

CB_TEST("java.minecraft.prep.remap.methods_told_apart_only_by_their_return_type_get_two_names", [] {
    std::vector<std::unique_ptr<bc::ClassNode>> nodes;
    nodes.push_back(klass("Block", "java/lang/Object", {}, {{"a", "(LWorld;III)Z"}, {"a", "(LWorld;III)V"}, {"a", "(I)V"}}));
    nodes.push_back(klass("BlockFlower", "Block", {}, {{"a", "(LWorld;III)V"}}));
    nodes.push_back(klass("BlockFluid", "Block", {}, {{"a", "(LWorld;III)Z"}}));
    const ClassStorage storage(std::move(nodes));
    const Mappings done = completed_for_source(Mappings{}, storage);
    CB_CHECK(!done.methods.get("Block", "(LWorld;III)Z", "a"));                                       // the first keeps its name
    CB_CHECK(done.methods.get("Block", "(LWorld;III)V", "a") == std::optional<std::string>("a_V"));
    CB_CHECK(!done.methods.get("Block", "(I)V", "a"));                                                // other parameters: no clash
    CB_CHECK(done.methods.get("BlockFlower", "(LWorld;III)V", "a") == std::optional<std::string>("a_V"));   // the override is named the same
    CB_CHECK(!done.methods.get("BlockFluid", "(LWorld;III)Z", "a"));
});

CB_TEST("java.minecraft.prep.remap.not_a_tiny_file", [] {
    fs::ScopedTempDir dir("cb-prep-remap");
    const std::filesystem::path p = dir.path() / "x.txt";
    fs::write_text(p, "not mappings\n");
    CB_CHECK(!MappingsIO::get_provider(p));
    CB_CHECK_THROWS_AS(std::runtime_error, (void)read_namespaces(p));
});

}  // namespace
}  // namespace cb::java::minecraft::prep
