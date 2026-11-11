#include "cb/knowledge/value.hpp"

#include <bit>
#include <cmath>
#include <limits>

#include <nlohmann/json.hpp>

#include "cb/core/crypto.hpp"
#include "cb/core/io.hpp"
#include "cb/core/json.hpp"

namespace cb::knowledge {

namespace {

constexpr char kMagic[4] = {'C', 'B', 'K', 'V'};
constexpr const char* kReader = "knowledge value";
// A list or map claiming more entries than there are bytes left is damaged; refuse before allocating for it.
constexpr std::size_t kMinItemBytes = 1;

enum Tag : std::uint8_t { kNull = 0, kFalse = 1, kTrue = 2, kInt = 3, kDouble = 4, kText = 5, kList = 6, kMap = 7 };

void write_text(io::ByteWriter& w, std::string_view s) {
    w.u32(static_cast<std::uint32_t>(s.size()));
    w.bytes(as_bytes(s));
}

void write_item(io::ByteWriter& w, const Value& v) {
    switch (v.kind()) {
        case Value::Kind::Null: w.u8(kNull); break;
        case Value::Kind::Bool: w.u8(*v.boolean() ? kTrue : kFalse); break;
        case Value::Kind::Int:
            w.u8(kInt);
            w.i64(*v.integer());
            break;
        case Value::Kind::Double:
            w.u8(kDouble);
            w.u64(std::bit_cast<std::uint64_t>(*v.number()));
            break;
        case Value::Kind::Text:
            w.u8(kText);
            write_text(w, *v.text());
            break;
        case Value::Kind::List:
            w.u8(kList);
            w.u32(static_cast<std::uint32_t>(v.list()->size()));
            for (const Value& item : *v.list()) write_item(w, item);
            break;
        case Value::Kind::Map:
            w.u8(kMap);
            w.u32(static_cast<std::uint32_t>(v.map()->size()));
            for (const auto& [key, item] : *v.map()) {
                write_text(w, key);
                write_item(w, item);
            }
            break;
    }
}

constexpr int kMaxDepth = 256;   // the encoding is data from a file: a hostile nesting must not exhaust the stack

std::string read_text(io::ByteReader& r) {
    const std::uint32_t n = r.u32();
    return std::string(as_text(r.bytes(n)));
}

Value read_item(io::ByteReader& r, int depth) {
    if (depth > kMaxDepth) throw FormatError(kReader, r.pos(), "nested too deeply");
    const std::size_t at = r.pos();
    switch (r.u8()) {
        case kNull: return Value();
        case kFalse: return Value(false);
        case kTrue: return Value(true);
        case kInt: return Value(r.i64());
        case kDouble: return Value(std::bit_cast<double>(r.u64()));
        case kText: return Value(read_text(r));
        case kList: {
            const std::uint32_t n = r.u32();
            if (n > r.remaining() / kMinItemBytes) throw FormatError(kReader, at, "list longer than the data");
            Value::List items;
            items.reserve(n);
            for (std::uint32_t i = 0; i < n; ++i) items.push_back(read_item(r, depth + 1));
            return Value(std::move(items));
        }
        case kMap: {
            const std::uint32_t n = r.u32();
            if (n > r.remaining() / kMinItemBytes) throw FormatError(kReader, at, "map larger than the data");
            Value::Map items;
            items.reserve(n);
            for (std::uint32_t i = 0; i < n; ++i) {
                std::string key = read_text(r);
                items.insert_or_assign(key, read_item(r, depth + 1));
            }
            return Value(std::move(items));
        }
        default: throw FormatError(kReader, at, "unknown item tag");
    }
}

// Builds a Value straight from the parser's events: no intermediate document, and a map's keys go into an OrderedMap
// (hashed) instead of nlohmann's ordered_json (a linear scan per key: quadratic on the 7,000-key tables).
class Builder {
public:
    bool null() { return put(Value()); }
    bool boolean(bool v) { return put(Value(v)); }
    bool number_integer(std::int64_t v) { return put(Value(v)); }
    bool number_unsigned(std::uint64_t v) {
        if (v > static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max())) {
            too_big_ = true;
            return false;
        }
        return put(Value(static_cast<std::int64_t>(v)));
    }
    bool number_float(double v, const std::string&) { return put(Value(v)); }
    bool string(std::string& v) { return put(Value(std::move(v))); }
    bool binary(std::vector<std::uint8_t>&) { return false; }   // JSON text has none
    bool start_object(std::size_t) { return open(true); }
    bool key(std::string& k) {
        stack_.back().key = std::move(k);
        return true;
    }
    bool end_object() { return close(); }
    bool start_array(std::size_t) { return open(false); }
    bool end_array() { return close(); }
    bool parse_error(std::size_t, const std::string&, const nlohmann::detail::exception&) { return false; }

    bool too_big() const { return too_big_; }
    Value take() { return std::move(result_); }

private:
    struct Frame {
        bool is_object = false;
        Value::List items;
        Value::Map map;
        std::string key;
    };
    bool open(bool is_object) {
        if (stack_.size() >= static_cast<std::size_t>(kMaxDepth)) return false;   // too deep
        stack_.emplace_back();
        stack_.back().is_object = is_object;
        return true;
    }
    bool close() {
        Frame f = std::move(stack_.back());
        stack_.pop_back();
        return put(f.is_object ? Value(std::move(f.map)) : Value(std::move(f.items)));
    }
    bool put(Value v) {
        if (stack_.empty()) {
            result_ = std::move(v);
        } else if (Frame& top = stack_.back(); top.is_object) {
            top.map.insert_or_assign(top.key, std::move(v));
        } else {
            top.items.push_back(std::move(v));
        }
        return true;
    }
    std::vector<Frame> stack_;
    Value result_;
    bool too_big_ = false;
};

}  // namespace

Value::Value(List items) : kind_(Kind::List), list_(std::make_shared<const List>(std::move(items))) {}
Value::Value(Map items) : kind_(Kind::Map), map_(std::make_shared<const Map>(std::move(items))) {}

std::optional<bool> Value::boolean() const {
    if (kind_ != Kind::Bool) return std::nullopt;
    return int_ != 0;
}
std::optional<std::int64_t> Value::integer() const {
    if (kind_ != Kind::Int) return std::nullopt;
    return int_;
}
std::optional<double> Value::number() const {
    if (kind_ == Kind::Double) return double_;
    if (kind_ == Kind::Int) return static_cast<double>(int_);
    return std::nullopt;
}
const std::string* Value::text() const { return kind_ == Kind::Text ? text_.get() : nullptr; }
const Value::List* Value::list() const { return kind_ == Kind::List ? list_.get() : nullptr; }
const Value::Map* Value::map() const { return kind_ == Kind::Map ? map_.get() : nullptr; }
const Value* Value::get(std::string_view key) const {
    if (kind_ != Kind::Map) return nullptr;
    return map_->find(std::string(key));
}

bool operator==(const Value& a, const Value& b) {
    if (a.kind_ != b.kind_) return false;
    switch (a.kind_) {
        case Value::Kind::Null: return true;
        case Value::Kind::Bool:
        case Value::Kind::Int: return a.int_ == b.int_;
        // bitwise, so that a value equals its own decoded copy (NaN included) and 0.0 differs from -0.0 as encoded
        case Value::Kind::Double: return std::bit_cast<std::uint64_t>(a.double_) == std::bit_cast<std::uint64_t>(b.double_);
        case Value::Kind::Text: return *a.text_ == *b.text_;
        case Value::Kind::List: return *a.list_ == *b.list_;
        case Value::Kind::Map: return *a.map_ == *b.map_;
    }
    return false;
}

Bytes Value::encode() const {
    io::ByteWriter w;
    w.bytes(as_bytes(std::string_view(kMagic, sizeof kMagic)));
    w.u8(kFormatVersion);
    write_item(w, *this);
    return w.take();
}

Value Value::decode(ByteView data) {
    io::ByteReader r(data, kReader);
    const ByteView magic = r.bytes(sizeof kMagic);
    if (as_text(magic) != std::string_view(kMagic, sizeof kMagic)) throw FormatError(kReader, 0, "not a knowledge value");
    const std::uint8_t version = r.u8();
    if (version != kFormatVersion) throw FormatError(kReader, 4, "format version " + std::to_string(version) + " is not read");
    Value v = read_item(r, 0);
    if (!r.eof()) throw FormatError(kReader, r.pos(), "bytes after the value");
    return v;
}

std::string Value::digest() const { return crypto::to_hex(crypto::sha1(encode())); }

Value Value::from_json_text(std::string_view text, std::string_view file_name) {
    Builder builder;
    if (!Json::sax_parse(text, &builder)) {
        if (builder.too_big()) throw FormatError(file_name, 0, "integer does not fit 64 bits");
        (void)json::parse(text);   // malformed: the parser that knows lines and columns names the fault (ParseError)
        throw FormatError(file_name, 0, "JSON nested too deeply");
    }
    return builder.take();
}

}  // namespace cb::knowledge
