// cb-unity: alone (helpers named for what they do here)
#include "knowledge/package.hpp"

#include <array>
#include <map>
#include <mutex>
#include <set>

#include "base/config.hpp"
#include "core/error.hpp"
#include "knowledge/reads.hpp"
#include "knowledge/sources.hpp"

namespace cb::knowledge {

namespace proto = cb::symbols::prototypes;
namespace api = cb::symbols::api_arguments;
namespace rtl = cb::symbols::runtime_library;

// ---- platforms ----------------------------------------------------------------------------------------------------------

std::span<const PlatformInfo> known_platforms() {
    static constexpr std::array<PlatformInfo, 6> kAll = {{{"win16", "16-bit Windows"},
                                                          {"win32", "32-bit Windows"},
                                                          {"windows", "16- and 32-bit Windows"},
                                                          {"vxd", "ring-0 drivers"},
                                                          {"dos", "DOS"},
                                                          {"xbox360", "Xbox 360"}}};
    return kAll;
}

std::string platform(std::string_view name) {
    // the lanes' names for a platform
    static constexpr std::array<std::pair<std::string_view, std::string_view>, 7> kAliases = {{{"ne", "win16"},
                                                                                              {"pe", "win32"},
                                                                                              {"le", "vxd"},
                                                                                              {"mz", "dos"},
                                                                                              {"powerpc", "xbox360"},
                                                                                              {"16", "win16"},
                                                                                              {"32", "win32"}}};
    for (const auto& [alias, base_name] : kAliases)
        if (alias == name) return std::string(base_name);
    return std::string(name);
}

std::string platform(int bits) {
    if (bits == 16) return "win16";
    if (bits == 32) return "win32";
    return std::to_string(bits);
}

std::string bare_name(std::string_view name) {
    // \w+[.!](\w+)
    const auto is_word = [](char c) { return c == '_' || (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); };
    const std::size_t at = name.find_first_of(".!");
    if (at == 0 || at == std::string_view::npos || at + 1 >= name.size()) return std::string(name);
    for (std::size_t i = 0; i < name.size(); ++i) {
        if (i == at) continue;
        if (!is_word(name[i])) return std::string(name);
    }
    return std::string(name.substr(at + 1));
}

// ---- the prototypes as rows of the base ---------------------------------------------------------------------------------

namespace {

Value row_of(const proto::Prototype& p) {
    Value::List parameters;
    for (const proto::Parameter& a : p.parameters) parameters.push_back(Value(Value::List{Value(a.type), Value(a.name)}));
    Value::Map m;
    m.insert_or_assign("returns", Value(p.returns));
    m.insert_or_assign("convention", Value(p.convention));
    m.insert_or_assign("marker", Value(p.marker));
    m.insert_or_assign("parameters", Value(std::move(parameters)));
    m.insert_or_assign("header", Value(p.header));
    return Value(std::move(m));
}

std::string text_field(const Value& row, std::string_view key) {
    const Value* v = row.get(key);
    const std::string* s = v != nullptr ? v->text() : nullptr;
    return s != nullptr ? *s : std::string();
}

proto::Prototype prototype_of(const Value& row) {
    proto::Prototype p;
    p.returns = text_field(row, "returns");
    p.convention = text_field(row, "convention");
    p.marker = text_field(row, "marker");
    p.header = text_field(row, "header");
    const Value* parameters = row.get("parameters");
    const Value::List* list = parameters != nullptr ? parameters->list() : nullptr;
    if (list != nullptr) {
        for (const Value& a : *list) {
            const Value::List* pair = a.list();
            if (pair == nullptr || pair->size() != 2) continue;
            const std::string* type = (*pair)[0].text();
            const std::string* name = (*pair)[1].text();
            p.parameters.push_back({type != nullptr ? *type : std::string(), name != nullptr ? *name : std::string()});
        }
    }
    return p;
}

proto::PrototypeMap map_of(const Rows& rows) {
    proto::PrototypeMap out;
    out.reserve(rows.size());
    for (const auto& [name, row] : rows) out.insert_or_assign(name, prototype_of(row));
    return out;
}

Rows rows_of(const proto::PrototypeMap& map) {
    Rows out;
    out.reserve(map.size());
    for (const auto& [name, p] : map) out.insert_or_assign(name, row_of(p));
    return out;
}

std::string text_cell(const Value& list, std::size_t at) {
    const Value::List* items = list.list();
    const std::string* s = items != nullptr && at < items->size() ? (*items)[at].text() : nullptr;
    return s != nullptr ? *s : std::string();
}

std::optional<std::uint32_t> bytes_of(const Value& v) {
    const std::optional<std::int64_t> n = v.integer();
    if (!n || *n < 0) return std::nullopt;
    return static_cast<std::uint32_t>(*n);
}

Value value_of(std::optional<std::uint32_t> bytes) { return bytes ? Value(static_cast<std::int64_t>(*bytes)) : Value(); }

// ---- the memories the argument-size readers take ----------------------------------------------------------------------------

class Win32Kinds : public api::Win32Memory {
public:
    explicit Win32Kinds(Store& store) : store_(store) {}

    std::optional<std::uint32_t> header_bytes(std::string_view name) override {
        record("win32", "argument_bytes.header", name);
        const Rows header = store_.authored_table({"win32", "argument_bytes.header"});
        const Value* v = header.find(std::string(name));
        return v != nullptr ? bytes_of(*v) : std::nullopt;
    }
    std::optional<api::Known> scanned(std::string_view stem, std::string_view name) override {
        const std::string key = std::string(stem) + "." + std::string(name);
        record("win32", "argument_bytes.code", key);
        const std::optional<Value> v = store_.get("win32", "argument_bytes.code", key);
        if (!v) return std::nullopt;
        return api::Known{bytes_of(*v)};
    }
    void remember(std::string_view stem, std::string_view name, std::optional<std::uint32_t> bytes) override {
        const std::string key = std::string(stem) + "." + std::string(name);
        store_.put("win32", "argument_bytes.code", key, value_of(bytes), "the exporting module's code", Origin::Learned);
        record("win32", "argument_bytes.code", key);
    }

private:
    static void record(std::string_view platform_name, std::string_view kind, std::string_view key) {
        knowledge::record(platform_name, kind, key);
    }
    Store& store_;
};

class Win16Kinds : public api::Win16Memory {
public:
    Win16Kinds(Store& store, std::string scope) : store_(store), scope_(std::move(scope)) {}

    std::optional<api::Known> remembered(std::string_view key) override {
        const std::string scoped = scope_ + std::string(key);
        knowledge::record("win16", "argument_bytes", scoped);
        const std::optional<Value> v = store_.get("win16", "argument_bytes", scoped);
        if (!v) return std::nullopt;
        return api::Known{bytes_of(*v)};
    }
    std::map<std::string, std::optional<std::uint32_t>> table() override {
        std::map<std::string, std::optional<std::uint32_t>> out;
        for (const auto& [key, value] : store_.table("win16", "argument_bytes"))
            if (key.starts_with(scope_)) out[key.substr(scope_.size())] = bytes_of(value);
        return out;
    }
    void remember(std::string_view key, std::optional<std::uint32_t> bytes, std::string_view source) override {
        const std::string scoped = scope_ + std::string(key);
        store_.put("win16", "argument_bytes", scoped, value_of(bytes), source, Origin::Learned);
        knowledge::record("win16", "argument_bytes", scoped);
    }

private:
    Store& store_;
    std::string scope_;   // "" or the product's prefix
};

}  // namespace

// ---- the object -------------------------------------------------------------------------------------------------------------

struct Knowledge::Impl {
    explicit Impl(proto::HeaderFolders h) : folders(std::move(h)) {}

    proto::HeaderFolders folders;
    std::mutex mutex;
    std::unique_ptr<proto::PrototypeTable> table;           // made at the first ask
    std::map<std::string, rtl::SignatureTable> signatures;   // by platform
    std::set<std::string> authored_loaded;                    // "platform:kind" of the authored kinds this object has made sure are in the store
};

Knowledge::Knowledge(Store& store, proto::HeaderFolders headers) : store_(store), impl_(std::make_unique<Impl>(std::move(headers))) {}

Knowledge::~Knowledge() = default;

std::optional<Value> Knowledge::known(std::string_view platform_name, std::string_view kind, std::string_view key) {
    const std::string p = platform(platform_name);
    record(p, kind, key);
    // A kind that is authored has its rows in the store only once its file has been read: a store that has not been asked for the whole kind
    // (`table`) holds none of them, and a lookup would find nothing.
    bool first = false;
    {
        const std::lock_guard lock(impl_->mutex);
        first = impl_->authored_loaded.insert(p + ":" + std::string(kind)).second;
    }
    if (first && store_.sources().lookup({p, std::string(kind)})) (void)store_.authored_table({p, std::string(kind)});
    return store_.get(p, kind, key);
}

void Knowledge::learn(std::string_view platform_name, std::string_view kind, std::string_view key, const Value& value,
                      std::string_view source) {
    store_.put(platform(platform_name), kind, key, value, source, Origin::Learned);
}

Rows Knowledge::table(std::string_view platform_name, std::string_view kind) {
    const std::string p = platform(platform_name);
    record_kind(p, kind);
    return store_.authored_table({p, std::string(kind)});
}

std::optional<Value> Knowledge::entry(std::string_view platform_name, std::string_view domain, std::string_view id) {
    return known(platform_name, domain, id);
}

Rows Knowledge::entries(std::string_view platform_name, std::string_view domain) { return table(platform_name, domain); }

std::vector<HardwareRegister> Knowledge::hardware_registers(std::string_view platform_name, std::string_view table_name) {
    const auto text = [](const Value& v, std::string_view key) -> std::string {
        const Value* field = v.get(key);
        const std::string* t = field != nullptr ? field->text() : nullptr;
        return t != nullptr ? *t : std::string();
    };
    std::vector<HardwareRegister> out;
    for (const auto& [id, entry] : entries(platform_name, "registers")) {
        if (text(entry, "kind") != "register") continue;
        HardwareRegister r;
        r.table = text(entry, "table");
        if (!table_name.empty() && r.table != table_name) continue;
        const std::optional<std::uint64_t> address = hex_value(text(entry, "address"));
        if (!address) continue;
        r.address = static_cast<std::uint32_t>(*address);
        r.name = text(entry, "name");
        if (r.name.empty()) r.name = id;
        r.space = text(entry, "space");
        if (r.space.empty()) r.space = "cpu";
        if (const Value* w = entry.get("width"); w != nullptr && w->integer()) r.width = static_cast<int>(*w->integer());
        if (const Value* n = entry.get("names"); n != nullptr && n->map() != nullptr)
            for (const auto& [dialect, spelling] : *n->map())
                if (spelling.text() != nullptr) r.names[dialect] = *spelling.text();
        out.push_back(std::move(r));
    }
    return out;
}

std::map<std::uint32_t, std::string> Knowledge::hardware_names(std::string_view platform_name, std::string_view table_name, std::string_view dialect) {
    std::map<std::uint32_t, std::string> out;
    for (const HardwareRegister& r : hardware_registers(platform_name, table_name)) {
        const auto spelling = r.names.find(std::string(dialect));
        out.try_emplace(r.address, spelling != r.names.end() ? spelling->second : r.name);
    }
    return out;
}

std::vector<HardwareRegion> Knowledge::hardware_regions(std::string_view platform_name, std::string_view table_name) {
    const auto text = [](const Value& v, std::string_view key) -> std::string {
        const Value* field = v.get(key);
        const std::string* t = field != nullptr ? field->text() : nullptr;
        return t != nullptr ? *t : std::string();
    };
    std::vector<HardwareRegion> out;
    for (const auto& [id, entry] : entries(platform_name, "registers")) {
        if (text(entry, "kind") != "region") continue;
        HardwareRegion r;
        r.table = text(entry, "table");
        if (!table_name.empty() && r.table != table_name) continue;
        const std::optional<std::uint64_t> address = hex_value(text(entry, "address"));
        const std::optional<std::uint64_t> size = hex_value(text(entry, "size"));
        if (!address || !size) continue;
        r.address = static_cast<std::uint32_t>(*address);
        r.size = static_cast<std::uint32_t>(*size);
        r.name = text(entry, "name");
        if (r.name.empty()) r.name = id;
        r.space = text(entry, "space");
        if (r.space.empty()) r.space = "cpu";
        out.push_back(std::move(r));
    }
    return out;
}

std::optional<std::string> Knowledge::export_name(std::string_view platform_name, std::string_view module, std::uint32_t ordinal) {
    const std::optional<Value> found = entry(platform_name, "ordinals", std::string(module) + "#" + std::to_string(ordinal));
    if (!found) return std::nullopt;
    const Value* name = found->get("name");
    if (name == nullptr || name->text() == nullptr) return std::nullopt;
    return *name->text();
}

std::map<std::string, std::string> Knowledge::export_names(std::string_view platform_name, std::string_view module) {
    std::map<std::string, std::string> out;
    for (const auto& [id, found] : entries(platform_name, "ordinals")) {
        const Value* m = found.get("module");
        const Value* number = found.get("ordinal");
        const Value* name = found.get("name");
        if (m == nullptr || m->text() == nullptr || *m->text() != module || number == nullptr || !number->integer() || name == nullptr || name->text() == nullptr) continue;
        out.emplace(std::to_string(*number->integer()), *name->text());
    }
    return out;
}

std::map<std::uint32_t, std::string> Knowledge::table_names(std::string_view platform_name, std::string_view table_name, std::string_view dialect) {
    const auto text = [](const Value& v, std::string_view key) -> std::string {
        const Value* field = v.get(key);
        const std::string* t = field != nullptr ? field->text() : nullptr;
        return t != nullptr ? *t : std::string();
    };
    std::map<std::uint32_t, std::string> out;
    for (const auto& [id, entry] : entries(platform_name, "registers")) {
        if (text(entry, "table") != table_name) continue;
        std::string place = text(entry, "address");
        if (place.empty())
            if (const Value* select = entry.get("select"); select != nullptr && select->map() != nullptr && !select->map()->empty())
                if (const std::string* first = select->map()->begin()->second.text()) place = *first;
        const std::optional<std::uint64_t> at = hex_value(place);
        if (!at) continue;
        std::string name = text(entry, "name");
        if (name.empty()) name = id;
        if (const Value* n = entry.get("names"); n != nullptr && n->map() != nullptr)
            if (const Value* spelling = n->get(dialect); spelling != nullptr && spelling->text() != nullptr) name = *spelling->text();
        out.try_emplace(static_cast<std::uint32_t>(*at), std::move(name));
    }
    return out;
}

std::vector<std::string> Knowledge::domains(std::string_view platform_name) {
    const std::string p = platform(platform_name);
    std::vector<std::string> out;
    for (const AuthoredKind& k : store_.sources().kinds())
        if (k.layout == Layout::Described && k.id.platform == p) out.push_back(k.id.kind);
    return out;
}

ValidationReport Knowledge::validate() {
    const std::filesystem::path& root = store_.sources().folders().described;
    return root.empty() ? ValidationReport{} : validate_descriptions(root);
}

Knowledge& standard() {
    struct Own {
        DataFolders folders = DataFolders::under(config::find_checkout());
        Store store{Store::default_path(folders), folders};
        Knowledge knowledge{store};
    };
    static Own own;
    return own.knowledge;
}

std::optional<Value> Knowledge::api_model(std::string_view platform_name, std::string_view api_name) {
    const std::string p = platform(platform_name);
    record(p, "api_model", api_name);
    const Rows models = store_.authored_table({p, "api_model"});
    const Value* model = models.find(std::string(api_name));
    const Value::Map* fields = model != nullptr ? model->map() : nullptr;
    if (fields == nullptr || fields->empty()) return std::nullopt;
    Value::Map out;
    for (const auto& [key, value] : *fields)
        if (!key.starts_with("_")) out.insert_or_assign(key, value);
    return Value(std::move(out));
}

const proto::PrototypeTable& Knowledge::prototypes() {
    std::lock_guard lock(impl_->mutex);
    if (impl_->table) return *impl_->table;
    const std::string stamp = proto::stamp(impl_->folders);
    // the headers are read once, whichever width asks first, and only when the base has none made from these folders
    std::optional<proto::Headers> read;
    const auto headers = [&]() -> const proto::Headers& {
        if (!read) read = proto::read_headers(impl_->folders);
        return *read;
    };
    const Rows w32 = store_.ensure({"win32", "prototype"}, stamp, [&] { return rows_of(headers().w32); }, "the headers", Origin::Derived);
    const Rows w16 = store_.ensure({"win16", "prototype"}, stamp, [&] { return rows_of(headers().w16); }, "the headers", Origin::Derived);
    impl_->table = std::make_unique<proto::PrototypeTable>(proto::Headers{map_of(w32), map_of(w16)});
    return *impl_->table;
}

const proto::Prototype* Knowledge::prototype(std::string_view platform_name, std::string_view name) {
    const bool sixteen = platform(platform_name) == "win16";
    return prototypes().of(bare_name(name), sixteen ? proto::Bits::W16 : proto::Bits::W32);
}

std::optional<std::string> Knowledge::declaration(std::string_view platform_name, std::string_view name) {
    const bool sixteen = platform(platform_name) == "win16";
    return prototypes().declaration(bare_name(name), sixteen ? proto::Bits::W16 : proto::Bits::W32);
}

std::string Knowledge::spelling(std::string_view name) { return prototypes().spelling(name); }

std::optional<BodyName> Knowledge::name(std::string_view body_digest) {
    record("bodies", "name", body_digest);
    return store_.name(body_digest);
}

void Knowledge::name_body(std::string_view body_digest, std::string_view name_text, std::string_view source) {
    store_.name_body(body_digest, name_text, source);
}

std::vector<rtl::SignatureRow> Knowledge::signatures(std::string_view platform_name) {
    std::vector<rtl::SignatureRow> out;
    for (const auto& [key, row] : table(platform_name, "signature")) {
        const Value::List* f = row.list();
        if (f == nullptr || f->size() != 6) throw FormatError("knowledge signature", 0, "row " + key + " is not six fields");
        const std::optional<std::int64_t> size = (*f)[1].integer();
        out.push_back({text_cell(row, 0), size && *size > 0 ? static_cast<std::size_t>(*size) : 0, text_cell(row, 2),
                       text_cell(row, 3), text_cell(row, 4), text_cell(row, 5)});
    }
    return out;
}

std::optional<rtl::Identified> Knowledge::signature_match(std::string_view platform_name, ByteView code) {
    const std::string p = platform(platform_name);
    std::lock_guard lock(impl_->mutex);
    auto it = impl_->signatures.find(p);
    if (it == impl_->signatures.end()) {
        const std::vector<rtl::SignatureRow> rows = signatures(p);
        it = impl_->signatures.emplace(p, rtl::SignatureTable::from_rows(rows)).first;
    }
    return it->second.match(code);
}

std::unique_ptr<api::Win32Memory> Knowledge::win32_memory() { return std::make_unique<Win32Kinds>(store_); }

std::unique_ptr<api::Win16Memory> Knowledge::win16_memory(std::string_view scope) {
    return std::make_unique<Win16Kinds>(store_, std::string(scope));
}

}  // namespace cb::knowledge
