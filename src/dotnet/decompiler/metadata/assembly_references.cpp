// dotnet/decompiler/metadata/assembly_references.cpp -- AssemblyNameReference and AssemblyReference.
#include "dotnet/decompiler/metadata/assembly_references.hpp"

#include <algorithm>
#include <cctype>
#include <format>

#include "core/crypto.hpp"
#include "dotnet/decompiler/metadata/metadata_extensions.hpp"

namespace cb::dotnet::decompiler::metadata {
namespace {

std::string_view trim(std::string_view s) {
    auto white = [](char c) { return c == ' ' || (c >= '\t' && c <= '\r'); };
    while (!s.empty() && white(s.front())) s.remove_prefix(1);
    while (!s.empty() && white(s.back())) s.remove_suffix(1);
    return s;
}

std::vector<std::string_view> split(std::string_view s, char sep) {
    std::vector<std::string_view> parts;
    std::size_t start = 0;
    while (true) {
        const std::size_t at = s.find(sep, start);
        parts.push_back(s.substr(start, at == std::string_view::npos ? std::string_view::npos : at - start));
        if (at == std::string_view::npos) return parts;
        start = at + 1;
    }
}

constexpr std::uint32_t kFlagPublicKey = 0x0001, kFlagRetargetable = 0x0100, kFlagWindowsRuntime = 0x0200;

}  // namespace

AssemblyVersion AssemblyVersion::parse(std::string_view text) {
    const std::vector<std::string_view> parts = split(text, '.');
    if (parts.size() < 2 || parts.size() > 4) throw UsageError(std::format("Version string portion was too short or too long: {}", text));
    int v[4] = {0, 0, -1, -1};
    for (std::size_t i = 0; i < parts.size(); ++i) {
        const std::string_view p = trim(parts[i]);
        if (p.empty()) throw UsageError(std::format("Input string was not in a correct format: {}", text));
        long long n = 0;
        for (const char c : p) {
            if (c < '0' || c > '9') throw UsageError(std::format("Input string was not in a correct format: {}", text));
            n = n * 10 + (c - '0');
            if (n > INT32_MAX) throw UsageError(std::format("Value was either too large or too small for an Int32: {}", text));
        }
        v[i] = static_cast<int>(n);
    }
    return {v[0], v[1], v[2], v[3]};
}

std::string AssemblyVersion::to_string4() const {
    if (build < 0 || revision < 0) throw UsageError("Version: the field count is larger than the version's parts");
    return std::format("{}.{}.{}.{}", major, minor, build, revision);
}

std::string AssemblyVersion::to_string() const {
    std::string s = std::format("{}.{}", major, minor);
    if (build >= 0) s += "." + std::to_string(build);
    if (build >= 0 && revision >= 0) s += "." + std::to_string(revision);
    return s;
}

AssemblyNameReference AssemblyNameReference::parse(std::string_view full_name) {
    if (full_name.empty()) throw UsageError("Name can not be empty");
    AssemblyNameReference name;
    const std::vector<std::string_view> tokens = split(full_name, ',');
    for (std::size_t i = 0; i < tokens.size(); ++i) {
        const std::string_view token = trim(tokens[i]);
        if (i == 0) {
            name.name_ = std::string(token);
            continue;
        }
        const std::vector<std::string_view> parts = split(token, '=');
        if (parts.size() != 2) throw UsageError("Malformed name");
        std::string key(parts[0]);
        std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        if (key == "version") {
            name.version_ = AssemblyVersion::parse(parts[1]);
        } else if (key == "culture") {
            name.culture_ = parts[1] == "neutral" ? std::string() : std::string(parts[1]);
        } else if (key == "publickeytoken") {
            if (parts[1] == "null") continue;
            Bytes token_bytes(parts[1].size() / 2);
            for (std::size_t j = 0; j < token_bytes.size(); ++j) {
                const std::string hex(parts[1].substr(j * 2, 2));
                std::size_t used = 0;
                const unsigned long b = std::stoul(hex, &used, 16);
                if (used != 2) throw UsageError("Malformed public key token");
                token_bytes[j] = static_cast<std::uint8_t>(b);
            }
            name.public_key_token_ = std::move(token_bytes);
        }
    }
    return name;
}

std::string AssemblyNameReference::full_name() const {
    std::string b = name_ + ", Version=" + version_.value_or(AssemblyVersion{0, 0, 0, 0}).to_string4();
    b += ", Culture=";
    b += culture_ && !culture_->empty() ? *culture_ : std::string("neutral");
    b += ", PublicKeyToken=";
    if (public_key_token_ && !public_key_token_->empty()) b += to_hex_string(*public_key_token_);
    else b += "null";
    if (is_retargetable()) b += ", Retargetable=Yes";
    return b;
}

AssemblyReference::AssemblyReference(const srm::MetadataReader& metadata, srm::AssemblyReferenceHandle handle) : metadata_(&metadata), handle_(handle) {
    if (handle.is_nil()) throw UsageError("AssemblyReference: a nil handle");
}

std::string AssemblyReference::name() const {
    try {
        return std::string(metadata_->get_string(metadata_->get_assembly_reference(handle_).name()));
    } catch (const srm::BadImageFormatError&) {
        return std::format("AR:{}", handle_.row());
    }
}

std::string AssemblyReference::full_name() const {
    try {
        return get_full_assembly_name(metadata_->get_assembly_reference(handle_), *metadata_);
    } catch (const srm::BadImageFormatError&) {
        return std::format("fullname(AR:{})", handle_.row());
    }
}

std::optional<AssemblyVersion> AssemblyReference::version() const {
    const srm::Version v = metadata_->get_assembly_reference(handle_).version();
    return AssemblyVersion{v.major, v.minor, v.build, v.revision};
}

std::optional<std::string> AssemblyReference::culture() const {
    return std::string(metadata_->get_string(metadata_->get_assembly_reference(handle_).culture()));
}

std::optional<Bytes> AssemblyReference::public_key_token() const {
    const srm::AssemblyReference entry = metadata_->get_assembly_reference(handle_);
    if (entry.public_key_or_token().is_nil()) return std::nullopt;
    const ByteView bytes = metadata_->get_blob_bytes(entry.public_key_or_token());
    if ((entry.flags() & kFlagPublicKey) != 0) {
        const crypto::Sha1Digest hash = crypto::sha1(bytes);
        return Bytes(hash.begin() + 12, hash.end());
    }
    return Bytes(bytes.begin(), bytes.end());
}

bool AssemblyReference::is_windows_runtime() const { return (metadata_->get_assembly_reference(handle_).flags() & kFlagWindowsRuntime) != 0; }
bool AssemblyReference::is_retargetable() const { return (metadata_->get_assembly_reference(handle_).flags() & kFlagRetargetable) != 0; }

}  // namespace cb::dotnet::decompiler::metadata
