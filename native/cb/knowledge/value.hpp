// cb/knowledge/value.hpp -- the value of one piece of knowledge.
//
// The knowledge base keeps (platform, kind, key, value) rows, and what a value looks like depends on the kind: a
// prototype record, a number of argument bytes, a list of runtime-library signatures, a table of ordinal names. The
// kinds are many and authored in data files, so the base cannot give each its own C++ type; `Value` is the closed
// document model they share: null, boolean, 64-bit integer, double, text, list, ordered map. A module that reads a
// kind converts it into its own typed struct once, at its edge.
//
// A Value is immutable once built and cheap to copy (a list or map is shared, never copied). Maps keep the order
// the producer wrote the keys in. `encode`/`decode` are the base's on-disk form: a 4-byte magic, a format version,
// then tagged items; `digest` is the SHA-1 of the encoding (what the engine compares to tell a piece changed).
// Text is UTF-8. An integer wider than 64 bits is not representable and is an error where a file is read.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/ordered_map.hpp"

namespace cb::knowledge {

class Value {
public:
    enum class Kind : std::uint8_t { Null, Bool, Int, Double, Text, List, Map };
    using List = std::vector<Value>;
    using Map = OrderedMap<std::string, Value>;

    Value() = default;
    Value(bool b) : kind_(Kind::Bool), int_(b ? 1 : 0) {}                       // NOLINT: values convert implicitly
    Value(std::int64_t i) : kind_(Kind::Int), int_(i) {}                        // NOLINT
    Value(int i) : kind_(Kind::Int), int_(i) {}                                 // NOLINT
    Value(std::uint32_t i) : kind_(Kind::Int), int_(i) {}                       // NOLINT
    Value(double d) : kind_(Kind::Double), double_(d) {}                        // NOLINT
    Value(std::string s) : kind_(Kind::Text), text_(std::make_shared<const std::string>(std::move(s))) {}   // NOLINT
    Value(std::string_view s) : Value(std::string(s)) {}                        // NOLINT
    Value(const char* s) : Value(std::string(s)) {}                             // NOLINT
    Value(List items);                                                          // NOLINT
    Value(Map items);                                                           // NOLINT

    Kind kind() const { return kind_; }
    bool is_null() const { return kind_ == Kind::Null; }

    // The typed views: empty/null when the value is of another kind (a wrong guess is data, not a bug).
    std::optional<bool> boolean() const;
    std::optional<std::int64_t> integer() const;      // Int only
    std::optional<double> number() const;             // Double, or an Int widened
    const std::string* text() const;
    const List* list() const;
    const Map* map() const;
    // The entry of a map by key, or nullptr (also nullptr when this is not a map).
    const Value* get(std::string_view key) const;

    friend bool operator==(const Value& a, const Value& b);

    // ---- the base's form -------------------------------------------------------------------------------------
    // The version of the encoding; a new version never reads an old one (the base is rebuilt: Logan, 2026-11-11).
    static constexpr std::uint8_t kFormatVersion = 1;
    Bytes encode() const;
    // Throws FormatError (reader "knowledge value") for a wrong magic or version, a truncated or malformed item,
    // or trailing bytes.
    static Value decode(ByteView data);
    // SHA-1 of encode(), lowercase hex.
    std::string digest() const;

    // ---- the files' form ---------------------------------------------------------------------------------------
    // A JSON document as a Value (the authored data files). Throws ParseError for malformed JSON and
    // FormatError for a number that does not fit (an integer past 64 bits).
    static Value from_json_text(std::string_view text, std::string_view file_name = "<json>");

private:
    Kind kind_ = Kind::Null;
    std::int64_t int_ = 0;
    double double_ = 0.0;
    std::shared_ptr<const std::string> text_;
    std::shared_ptr<const List> list_;
    std::shared_ptr<const Map> map_;
};

}  // namespace cb::knowledge
