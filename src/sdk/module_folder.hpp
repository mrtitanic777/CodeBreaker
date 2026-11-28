// sdk/module_folder.hpp -- the folder the SDK's library is loaded from: cbsdk.dll's (the installed bin/), or the program's when the
// SDK is linked in statically (the tests).
#pragma once

#include <filesystem>

namespace cb::sdk_impl {

std::filesystem::path module_folder();

}  // namespace cb::sdk_impl
