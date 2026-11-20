// Tests of the mappings reader: a tiny v1 file reads as the same mappings as the tiny v2 file of the same names (the v2 reader is the one RetroMCP's
// versions use, proven against the original on six of them: work/retire-retromcp/B); the namespaces of a header; the provider of a file.
#include "java/projects/minecraft/prep/remap.hpp"

#include <filesystem>

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

CB_TEST("java.minecraft.prep.remap.not_a_tiny_file", [] {
    fs::ScopedTempDir dir("cb-prep-remap");
    const std::filesystem::path p = dir.path() / "x.txt";
    fs::write_text(p, "not mappings\n");
    CB_CHECK(!MappingsIO::get_provider(p));
    CB_CHECK_THROWS_AS(std::runtime_error, (void)read_namespaces(p));
});

}  // namespace
}  // namespace cb::java::minecraft::prep
