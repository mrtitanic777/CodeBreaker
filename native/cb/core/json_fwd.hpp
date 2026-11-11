// cb/core/json_fwd.hpp -- a hand-written forward declaration of nlohmann's ordered_json, and the cb::Json alias.
//
// nlohmann::json is an expensive header; CodeBreaker headers never include it. They include this file and
// declare `load(path)` / `save(path)` in terms of typed structs, or take `const Json&` by reference. A .cpp that
// builds or reads a Json includes <nlohmann/json.hpp> (from native/third_party) itself.
//
// The declarations below are the ones nlohmann 3.10.5 makes in its own json_fwd.hpp and under the same include
// guard, so including the real header before or after this one is harmless. Moving the vendored copy to another
// version means re-checking this file against the new json_fwd.hpp; json_test.cpp fails to compile if they differ.
#pragma once

#ifndef INCLUDE_NLOHMANN_JSON_FWD_HPP_
#define INCLUDE_NLOHMANN_JSON_FWD_HPP_

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace nlohmann {
template <typename T = void, typename SFINAE = void>
struct adl_serializer;

template <template <typename U, typename V, typename... Args> class ObjectType = std::map,
          template <typename U, typename... Args> class ArrayType = std::vector, class StringType = std::string,
          class BooleanType = bool, class NumberIntegerType = std::int64_t, class NumberUnsignedType = std::uint64_t,
          class NumberFloatType = double, template <typename U> class AllocatorType = std::allocator,
          template <typename T, typename SFINAE = void> class JSONSerializer = adl_serializer,
          class BinaryType = std::vector<std::uint8_t>>
class basic_json;

template <typename BasicJsonType>
class json_pointer;

using json = basic_json<>;

template <class Key, class T, class IgnoredLess, class Allocator>
struct ordered_map;

using ordered_json = basic_json<nlohmann::ordered_map>;
}  // namespace nlohmann

#endif  // INCLUDE_NLOHMANN_JSON_FWD_HPP_

namespace cb {

// The one JSON type: insertion order of object keys is preserved, so written files are stable.
using Json = nlohmann::ordered_json;

}  // namespace cb
