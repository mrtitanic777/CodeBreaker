// dotnet/decompiler/typesystem/full_type_name.cpp -- TopLevelTypeName and FullTypeName.
#include "dotnet/decompiler/typesystem/full_type_name.hpp"

#include <charconv>

#include "core/error.hpp"

namespace cb::dotnet::decompiler::typesystem {
namespace {

// int.TryParse with NumberStyles.Integer and the invariant culture: white space around, an optional sign, ASCII digits, no overflow.
bool try_parse_int(std::string_view s, int& value) {
    auto white = [](char c) { return c == ' ' || (c >= '\t' && c <= '\r'); };
    while (!s.empty() && white(s.front())) s.remove_prefix(1);
    while (!s.empty() && white(s.back())) s.remove_suffix(1);
    bool negative = false;
    if (!s.empty() && (s.front() == '+' || s.front() == '-')) {
        negative = s.front() == '-';
        s.remove_prefix(1);
    }
    if (s.empty()) return false;
    long long v = 0;
    for (const char c : s) {
        if (c < '0' || c > '9') return false;
        v = v * 10 + (c - '0');
        if (v > 2147483648LL) return false;
    }
    if (negative) v = -v;
    if (v > 2147483647LL || v < -2147483648LL) return false;
    value = static_cast<int>(v);
    return true;
}

}  // namespace

std::string split_type_parameter_count_from_reflection_name(std::string_view reflection_name, int& type_parameter_count) {
    const std::size_t pos = reflection_name.rfind('`');
    type_parameter_count = 0;
    if (pos == std::string_view::npos) return std::string(reflection_name);
    int count = 0;
    if (try_parse_int(reflection_name.substr(pos + 1), count)) {
        type_parameter_count = count;
        return std::string(reflection_name.substr(0, pos));
    }
    return std::string(reflection_name);
}

std::string split_type_parameter_count_from_reflection_name(std::string_view reflection_name) {
    const std::size_t pos = reflection_name.rfind('`');
    return std::string(pos == std::string_view::npos ? reflection_name : reflection_name.substr(0, pos));
}

TopLevelTypeName::TopLevelTypeName(std::string_view reflection_name) {
    const std::size_t pos = reflection_name.rfind('.');
    std::string_view name = reflection_name;
    if (pos != std::string_view::npos) {
        ns_ = std::string(reflection_name.substr(0, pos));
        name = reflection_name.substr(pos + 1);
    }
    name_ = split_type_parameter_count_from_reflection_name(name, type_parameter_count_);
}

std::string TopLevelTypeName::reflection_name() const {
    std::string b;
    if (!ns_.empty()) {
        b += ns_;
        b += '.';
    }
    b += name_;
    if (type_parameter_count_ > 0) {
        b += '`';
        b += std::to_string(type_parameter_count_);
    }
    return b;
}

FullTypeName::FullTypeName(std::string_view reflection_name) {
    const std::size_t pos = reflection_name.find('+');
    if (pos == std::string_view::npos) {
        top_level_ = TopLevelTypeName(reflection_name);
        return;
    }
    std::vector<std::string_view> parts;
    std::size_t start = 0;
    while (true) {
        const std::size_t plus = reflection_name.find('+', start);
        parts.push_back(reflection_name.substr(start, plus == std::string_view::npos ? std::string_view::npos : plus - start));
        if (plus == std::string_view::npos) break;
        start = plus + 1;
    }
    top_level_ = TopLevelTypeName(parts[0]);
    nested_.emplace();
    for (std::size_t i = 1; i < parts.size(); ++i) {
        int tpc = 0;
        std::string name = split_type_parameter_count_from_reflection_name(parts[i], tpc);
        nested_->push_back({std::move(name), tpc});
    }
}

std::string FullTypeName::reflection_name() const {
    if (!nested_) return top_level_.reflection_name();
    std::string b = top_level_.reflection_name();
    for (const NestedTypeName& nt : *nested_) {
        b += '+';
        b += nt.name;
        if (nt.additional_type_parameter_count > 0) {
            b += '`';
            b += std::to_string(nt.additional_type_parameter_count);
        }
    }
    return b;
}

std::string FullTypeName::full_name() const {
    std::string b = top_level_.namespace_() + "." + top_level_.name();
    if (nested_)
        for (const NestedTypeName& nt : *nested_) b += "." + nt.name;
    return b;
}

int FullTypeName::type_parameter_count() const {
    int tpc = top_level_.type_parameter_count();
    if (nested_)
        for (const NestedTypeName& nt : *nested_) tpc += nt.additional_type_parameter_count;
    return tpc;
}

const std::string& FullTypeName::get_nested_type_name(int level) const {
    if (!nested_) throw UsageError("FullTypeName: a top-level type has no nested names");
    return nested_->at(static_cast<std::size_t>(level)).name;
}

int FullTypeName::get_nested_type_additional_type_parameter_count(int level) const {
    if (!nested_) throw UsageError("FullTypeName: a top-level type has no nested names");
    return nested_->at(static_cast<std::size_t>(level)).additional_type_parameter_count;
}

FullTypeName FullTypeName::get_declaring_type() const {
    if (!nested_) throw UsageError("FullTypeName: a top-level type has no declaring type");
    if (nested_->size() == 1) return top_level_;
    return FullTypeName(top_level_, std::vector<NestedTypeName>(nested_->begin(), nested_->end() - 1));
}

FullTypeName FullTypeName::nested_type(std::string name, int additional_type_parameter_count) const {
    std::vector<NestedTypeName> n = nested_.value_or(std::vector<NestedTypeName>{});
    n.push_back({std::move(name), additional_type_parameter_count});
    return FullTypeName(top_level_, std::move(n));
}

bool operator==(const FullTypeName& a, const FullTypeName& b) {
    // FullTypeNameComparer.Ordinal: the nesting level, the top-level name, then each nested name and count
    return a.nesting_level() == b.nesting_level() && a.top_level_ == b.top_level_ &&
           (a.nesting_level() == 0 || *a.nested_ == *b.nested_);
}

}  // namespace cb::dotnet::decompiler::typesystem
