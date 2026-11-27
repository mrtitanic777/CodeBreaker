#include "dotnet/decompiler/documentation/xml_documentation_provider.hpp"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <stdexcept>

#include "core/fs.hpp"
#include "dotnet/decompiler/documentation/id_string_provider.hpp"
#include "dotnet/decompiler/documentation/xml_doc_loader.hpp"
#include "dotnet/decompiler/util/dotnet_path.hpp"
#include "dotnet/decompiler/util/resources_file.hpp"
#include "dotnet/decompiler/util/xml_text_reader.hpp"

namespace cb::dotnet::decompiler::documentation {

namespace {

// Hash algorithm used for the index. This is a custom implementation so that old index files work correctly even when the .NET string.GetHashCode
// implementation changes (e.g. due to .NET 4.5 hash randomization). It runs over the UTF-16 code units of the key.
int get_hash_code(std::string_view key) {
    std::uint32_t h = 0;
    auto add = [&h](std::uint32_t unit) { h = (h << 5) - h + unit; };
    for (std::size_t i = 0; i < key.size();) {
        const unsigned char c = static_cast<unsigned char>(key[i]);
        std::uint32_t cp;
        std::size_t extra;
        if (c < 0x80) {
            cp = c;
            extra = 0;
        } else if (c >= 0xF0) {
            cp = c & 0x07;
            extra = 3;
        } else if (c >= 0xE0) {
            cp = c & 0x0F;
            extra = 2;
        } else if (c >= 0xC0) {
            cp = c & 0x1F;
            extra = 1;
        } else {
            cp = 0xFFFD;
            extra = 0;
        }
        ++i;
        for (std::size_t k = 0; k < extra && i < key.size() && (static_cast<unsigned char>(key[i]) & 0xC0) == 0x80; ++k, ++i)
            cp = (cp << 6) | (static_cast<unsigned char>(key[i]) & 0x3F);
        if (cp >= 0x10000) {
            cp -= 0x10000;
            add(0xD800 + (cp >> 10));
            add(0xDC00 + (cp & 0x3FF));
        } else {
            add(cp);
        }
    }
    return static_cast<int>(h);
}

// Maps a line number of the file to the byte position where the line starts (as the stream is read: after the line break that ends the line before it,
// and after the CR of a CRLF pair, for a line that ends in one).
class LinePositionMapper {
public:
    explicit LinePositionMapper(ByteView bytes) : bytes_(bytes) {}

    int get_position_for_line(int line) {
        while (line > current_line_) {
            if (pos_ >= bytes_.size()) throw util::EndOfStreamError();
            const std::uint8_t b = bytes_[pos_++];
            // only line breaks matter: every other byte is, or is part of, a character that is none
            const char c = b == '\n' || b == '\r' ? static_cast<char>(b) : 'x';
            if ((prev_char_ != '\r' && c == '\n') || c == '\r') ++current_line_;
            prev_char_ = c;
        }
        if (pos_ > 0x7FFFFFFF) throw std::overflow_error("the documentation file is too large");
        return static_cast<int>(pos_);
    }

private:
    ByteView bytes_;
    std::size_t pos_ = 0;
    int current_line_ = 1;
    char prev_char_ = '\0';
};

// The `member` elements of the `members` section, with where each starts (the reader is on `members`).
struct MemberPosition {
    int hash_code;
    int position;
};

void read_members_section(util::XmlTextReader& reader, LinePositionMapper& line_pos_mapper, std::vector<MemberPosition>& index_list) {
    while (reader.read()) {
        switch (reader.node_type()) {
        case util::XmlNodeType::EndElement:
            if (reader.local_name() == "members") return;
            break;
        case util::XmlNodeType::Element:
            if (reader.local_name() == "member") {
                const int pos = line_pos_mapper.get_position_for_line(reader.line_number()) + std::max(reader.line_position() - 2, 0);
                const std::optional<std::string> member_attr = reader.get_attribute("name");
                if (member_attr) index_list.push_back({get_hash_code(*member_attr), pos});
                reader.skip();
            }
            break;
        default: break;
        }
    }
}

std::string append_directory_separator(std::string dir) {
    if (dir.ends_with('\\') || dir.ends_with('/')) return dir;
    return dir + util::path::kDirectorySeparatorChar;
}

std::string replace_all(std::string text, const std::string& what, const std::string& with) {
    std::size_t pos = 0;
    while (!what.empty() && (pos = text.find(what, pos)) != std::string::npos) {
        text.replace(pos, what.size(), with);
        pos += with.size();
    }
    return text;
}

std::string program_files_x86() {
    char* value = nullptr;
    std::size_t length = 0;
    std::string result;
#ifdef _WIN32
    if (_dupenv_s(&value, &length, "ProgramFiles(x86)") == 0 && value != nullptr) result = value;
    if (result.empty() && _dupenv_s(&value, &length, "ProgramFiles") == 0 && value != nullptr) result = value;
    std::free(value);
#endif
    return result;
}

}  // namespace

XmlDocumentationProvider::Cache::Cache(int size) {
    if (size <= 0) throw std::out_of_range("size: Value must be positive");
    entries_.resize(static_cast<std::size_t>(size));
}

bool XmlDocumentationProvider::Cache::try_get(const std::string& key, std::optional<std::string>& value) const {
    for (const CacheEntry& pair : entries_) {
        if (pair.used && pair.key == key) {
            value = pair.value;
            return true;
        }
    }
    value = std::nullopt;
    return false;
}

void XmlDocumentationProvider::Cache::add(const std::string& key, const std::optional<std::string>& value) {
    entries_[pos_++] = CacheEntry{true, key, value};
    if (pos_ == entries_.size()) pos_ = 0;
}

XmlDocumentationProvider::XmlDocumentationProvider() = default;

XmlDocumentationProvider::XmlDocumentationProvider(const std::filesystem::path& file_name) {
    const cb::fs::MappedFile file(file_name);
    util::XmlTextReader xml_reader(file.view());
    xml_reader.move_to_content();   // no DTD resolving: the reader has none
    const std::optional<std::string> redirect = xml_reader.get_attribute("redirect");
    if (!redirect || redirect->empty()) {
        file_name_ = file_name;
        encoding_ = xml_reader.encoding_web_name();
        read_xml_doc(file_name, xml_reader);
    } else {
        const std::filesystem::path redirection_target = get_redirection_target(file_name, *redirect);
        if (!redirection_target.empty()) {
            const cb::fs::MappedFile redirected(redirection_target);
            util::XmlTextReader redirected_xml_reader(redirected.view());
            redirected_xml_reader.move_to_content();
            file_name_ = redirection_target;
            encoding_ = redirected_xml_reader.encoding_web_name();
            read_xml_doc(redirection_target, redirected_xml_reader);
        } else {
            throw XmlException("XmlDoc " + util::path::native_text(file_name) + " is redirecting to " + *redirect + ", but that file was not found.");
        }
    }
}

std::filesystem::path XmlDocumentationProvider::get_redirection_target(const std::filesystem::path& xml_file_name, const std::string& target) {
    const std::string program_files_dir = append_directory_separator(program_files_x86());

    const std::optional<std::filesystem::path> runtime = runtime_directory();
    const std::string cor_sys_dir = append_directory_separator(runtime ? util::path::native_text(*runtime) : std::string());

    std::string file_name = replace_all(replace_all(target, "%PROGRAMFILESDIR%", program_files_dir), "%CORSYSDIR%", cor_sys_dir);
    if (!util::path::is_path_rooted(file_name)) {
        const std::optional<std::string> directory = util::path::get_directory_name(util::path::native_text(xml_file_name));
        file_name = util::path::combine(directory.value_or(std::string()), file_name);
    }
    const std::optional<std::string> found = xml_doc_loader::lookup_localized_xml_doc(file_name);
    return found ? util::path::from_text(*found) : std::filesystem::path();
}

void XmlDocumentationProvider::read_xml_doc(const std::filesystem::path& path, util::XmlTextReader& reader) const {
    // Open up a second view of the file for the line<->position mapping
    const cb::fs::MappedFile file(path);
    LinePositionMapper line_pos_mapper(file.view());
    std::vector<MemberPosition> index_list;
    while (reader.read()) {
        if (reader.is_start_element()) {
            if (reader.local_name() == "members") read_members_section(reader, line_pos_mapper, index_list);
        }
    }
    Index index;
    index.reserve(index_list.size());
    for (const MemberPosition& m : index_list) index.push_back(IndexEntry{m.hash_code, m.position});
    std::stable_sort(index.begin(), index.end(), [](const IndexEntry& a, const IndexEntry& b) { return a.hash_code < b.hash_code; });
    set_index(std::move(index));
}

std::shared_ptr<const XmlDocumentationProvider::Index> XmlDocumentationProvider::index() const {
    const std::lock_guard<std::mutex> lock(index_mutex_);
    return index_;
}

void XmlDocumentationProvider::set_index(Index index) const {
    auto fresh = std::make_shared<const Index>(std::move(index));
    const std::lock_guard<std::mutex> lock(index_mutex_);
    index_ = std::move(fresh);
}

std::optional<std::string> XmlDocumentationProvider::get_documentation(std::string_view key) const { return get_documentation_core(std::string(key), true); }

std::optional<std::string> XmlDocumentationProvider::get_documentation(const typesystem::IEntity* entity) const {
    if (entity == nullptr) throw UsageError("entity");
    const typesystem::IModule* parent = entity->parent_module();
    const metadata::MetadataFile* module = parent != nullptr ? parent->metadata_file() : nullptr;
    if (module == nullptr || entity->metadata_token().is_nil()) return std::nullopt;
    // The member keys depend on the compiler that wrote the xml file (C# vs the MSVC C++/CLI dialect), so try each candidate form.
    for (const std::string& id_string : id_string_provider::get_id_string_candidates(*module, entity->metadata_token())) {
        const std::optional<std::string> documentation = get_documentation(std::string_view(id_string));
        if (documentation) return documentation;
    }
    return std::nullopt;
}

std::optional<std::string> XmlDocumentationProvider::get_documentation_core(const std::string& key, bool allow_reload) const {
    const int hashcode = get_hash_code(key);
    const std::shared_ptr<const Index> index = this->index();
    // index is sorted, so we can use binary search; possibly there are multiple items with the same hash, so go to the first.
    auto it = std::lower_bound(index->begin(), index->end(), hashcode, [](const IndexEntry& entry, int hash) { return entry.hash_code < hash; });
    if (it == index->end() || it->hash_code != hashcode) return std::nullopt;
    std::size_t m = static_cast<std::size_t>(it - index->begin());

    const std::lock_guard<std::recursive_mutex> lock(cache_mutex_);
    std::optional<std::string> val;
    if (!cache_.try_get(key, val)) {
        try {
            // go through all items that have the correct hash
            for (; m < index->size() && (*index)[m].hash_code == hashcode; ++m) {
                val = load_documentation(key, (*index)[m].position_in_file);
                if (val) break;
            }
            // cache the result (even if it is null)
            cache_.add(key, val);
        } catch (const IoError&) {
            // may happen if the documentation file was deleted/is inaccessible/changed (EndOfStreamException)
            return allow_reload ? reload_and_get_documentation(key) : std::nullopt;
        } catch (const util::EndOfStreamError&) {
            return allow_reload ? reload_and_get_documentation(key) : std::nullopt;
        } catch (const XmlException&) {
            // may happen if the documentation file was changed so that the file position no longer starts on a valid XML element
            return allow_reload ? reload_and_get_documentation(key) : std::nullopt;
        }
    }
    return val;
}

std::optional<std::string> XmlDocumentationProvider::reload_and_get_documentation(const std::string& key) const {
    try {
        // Reload the index
        const cb::fs::MappedFile file(file_name_);
        util::XmlTextReader xml_reader(file.view());
        xml_reader.move_to_content();
        read_xml_doc(file_name_, xml_reader);
    } catch (const IoError&) {
        // Ignore errors on reload; IEntity.Documentation callers aren't prepared to handle exceptions
        set_index(Index());   // clear index to avoid future load attempts
        return std::nullopt;
    } catch (const util::EndOfStreamError&) {
        set_index(Index());
        return std::nullopt;
    } catch (const XmlException&) {
        set_index(Index());
        return std::nullopt;
    }
    return get_documentation_core(key, /*allow_reload=*/false);   // prevent infinite reload loops
}

std::optional<std::string> XmlDocumentationProvider::load_documentation(const std::string& key, int position_in_file) const {
    const cb::fs::MappedFile file(file_name_);
    const ByteView bytes = file.view();
    // an element at the position: the text is read as a fragment from there
    const ByteView fragment = static_cast<std::size_t>(position_in_file) <= bytes.size() ? bytes.subspan(static_cast<std::size_t>(position_in_file)) : ByteView();
    util::XmlReaderOptions options;
    options.fragment = true;
    util::XmlTextReader r(fragment, options);
    while (r.read()) {
        if (r.node_type() == util::XmlNodeType::Element) {
            const std::optional<std::string> member_attr = r.get_attribute("name");
            if (member_attr && *member_attr == key) return r.read_inner_xml();
            return std::nullopt;
        }
    }
    return std::nullopt;
}

}  // namespace cb::dotnet::decompiler::documentation
