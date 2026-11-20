#pragma once

#include <filesystem>

namespace xui
{
    // CB_XUI_TEST=1 (xui_tests.cpp). Returns the number of failures.
    int RunXuiTests();

    // Where the dashboard's XUI packages and fonts are (CB_XUI_ASSETS overrides).
    std::filesystem::path AssetDirectory();
}
