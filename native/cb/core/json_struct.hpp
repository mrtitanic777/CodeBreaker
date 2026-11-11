// cb/core/json_struct.hpp -- CB_JSON_STRUCT: from_json/to_json for a plain struct, over any nlohmann JSON type.
//
// nlohmann 3.10.5's NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE only defines conversions for the default `nlohmann::json`
// (sorted keys), which cannot convert to or from cb::Json (ordered_json). This macro defines the same pair as
// templates over the JSON type, so a typed struct converts to and from cb::Json, preserving member order.
//
//     struct Section { std::string name; std::uint32_t rva = 0; };
//     CB_JSON_STRUCT(Section, name, rva)      // in the struct's own namespace, in the .cpp that reads the JSON
//
// A missing key on reading throws nlohmann's type_error; use j.value("key", default) by hand where a key is
// optional. Includes <nlohmann/json.hpp>: only .cpp files may include this header.
#pragma once

#include <nlohmann/json.hpp>

#define CB_JSON_STRUCT(Type, ...)                                                                   \
    template <class CbJson>                                                                         \
    void to_json(CbJson& nlohmann_json_j, const Type& nlohmann_json_t) {                            \
        NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_TO, __VA_ARGS__))                    \
    }                                                                                               \
    template <class CbJson>                                                                         \
    void from_json(const CbJson& nlohmann_json_j, Type& nlohmann_json_t) {                          \
        NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, __VA_ARGS__))                  \
    }
