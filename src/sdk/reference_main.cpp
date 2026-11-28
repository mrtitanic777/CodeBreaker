// sdk/reference_main.cpp -- cb_sdk_reference [checkout]: writes docs/sdk/reference from the comments of include/cb (tools\sdk\reference.cmd).
// The test sdk.reference.is_current fails when the pages in the tree are not what this would write.
#include <cstdio>
#include <exception>
#include <string>

#include "core/fs.hpp"
#include "sdk/reference.hpp"

int main(int argc, char** argv) {
    try {
        const std::filesystem::path checkout = argc > 1 ? cb::fs::from_text(argv[1]) : std::filesystem::current_path();
        const auto pages = cb::sdk_impl::reference::of_checkout(checkout);
        const std::size_t n = cb::sdk_impl::reference::write(pages, checkout / "docs" / "sdk" / "reference");
        std::printf("wrote %zu pages to %s\n", n, cb::fs::to_text(checkout / "docs" / "sdk" / "reference").c_str());
        return 0;
    } catch (const std::exception& e) {
        std::fprintf(stderr, "cb_sdk_reference: %s\n", e.what());
        return 1;
    }
}
