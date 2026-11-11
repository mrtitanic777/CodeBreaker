// cb/source/vxd/ddk.cpp -- where the Windows 95 DDK is (see ddk.hpp).
#include "cb/source/vxd/ddk.hpp"

namespace cb::source::vxd {

std::optional<std::filesystem::path> DdkFolders::first_existing(const std::vector<std::filesystem::path>& candidates,
                                                                const std::filesystem::path& leaf) {
    for (const auto& candidate : candidates) {
        const std::filesystem::path p = leaf.empty() ? candidate : candidate / leaf;
        std::error_code ec;
        if (!candidate.empty() && std::filesystem::exists(p, ec)) return p;
    }
    return std::nullopt;
}

DdkFolders DdkFolders::standard(const base::Corpus& corpus, const std::filesystem::path& checkout_parent) {
    DdkFolders ddk;
    ddk.inc = {corpus.reference({"Microsoft Windows 95 DDK", "INC32"}),
               checkout_parent / "Windows95" / "design" / "reference" / "Microsoft Windows 95 DDK" / "INC32",
               checkout_parent / "docs" / "reference" / "win95ddk" / "INC32",
               checkout_parent / "docs" / "reference" / "loganxp" / "win95ddk" / "INC32"};
    ddk.debug = {corpus.reference({"Microsoft Windows 95 DDK", "DEBUG"}),
                 checkout_parent / "docs" / "reference" / "win95ddk" / "DEBUG"};
    return ddk;
}

}  // namespace cb::source::vxd
