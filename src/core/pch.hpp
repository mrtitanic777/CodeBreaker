// The precompiled header of src (CB_PCH, CMakeLists.txt): the standard headers nearly every file includes,
// parsed once per build instead of once per file. Only standard and third-party headers that do not change: a project
// header here would make every file rebuild when it changes.
#pragma once

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <format>
#include <iostream>
#include <array>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <span>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#include <nlohmann/json.hpp>
