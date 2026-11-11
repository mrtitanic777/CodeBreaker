#include "cb/base/sqlite.hpp"

#include <sqlite3.h>

#include <functional>
#include <unordered_map>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"

namespace cb::base::sqlite {

namespace {

[[noreturn]] void fail(const std::filesystem::path& file, sqlite3* db, int code, std::string_view what) {
    std::string message(what);
    message += ": ";
    message += db != nullptr ? sqlite3_errmsg(db) : sqlite3_errstr(code);
    throw IoError(file, code, message);
}

struct StringHash {
    using is_transparent = void;
    std::size_t operator()(std::string_view s) const { return std::hash<std::string_view>{}(s); }
};

}  // namespace

// Statements are prepared once per connection and reused: the cache and the connection live and die together.
struct Database::Impl {
    struct Cached {
        sqlite3_stmt* stmt = nullptr;
        bool in_use = false;     // a statement runs once at a time: a nested use of the same text prepares its own
    };
    static constexpr std::size_t kMaxCached = 64;
    sqlite3* db = nullptr;
    std::filesystem::path file;
    std::unordered_map<std::string, Cached, StringHash, std::equal_to<>> statements;
    ~Impl() {
        for (auto& [sql, c] : statements) sqlite3_finalize(c.stmt);
        if (db != nullptr) sqlite3_close_v2(db);
    }
};

namespace {

// A prepared statement for one run: taken from the connection's cache (or prepared, and cached), reset on every exit.
class Statement {
public:
    Statement(Database::Impl& impl, std::string_view sql) : impl_(impl) {
        auto it = impl.statements.find(sql);
        if (it != impl.statements.end() && !it->second.in_use) {
            cached_ = &it->second;
            stmt_ = cached_->stmt;
        } else {
            const int rc = sqlite3_prepare_v2(impl.db, sql.data(), static_cast<int>(sql.size()), &stmt_, nullptr);
            if (rc != SQLITE_OK) fail(impl.file, impl.db, rc, "prepare");
            if (it == impl.statements.end() && impl.statements.size() < Database::Impl::kMaxCached)
                cached_ = &impl.statements.emplace(std::string(sql), Database::Impl::Cached{stmt_, false}).first->second;
        }
        if (cached_ != nullptr) cached_->in_use = true;
    }
    Statement(const Statement&) = delete;
    Statement& operator=(const Statement&) = delete;
    ~Statement() {
        if (cached_ != nullptr) {
            sqlite3_reset(stmt_);
            sqlite3_clear_bindings(stmt_);
            cached_->in_use = false;
        } else {
            sqlite3_finalize(stmt_);
        }
    }

    // The parameters must outlive the run: text and blobs are bound without a copy.
    void bind(const std::vector<Value>& params) {
        sqlite3_reset(stmt_);
        sqlite3_clear_bindings(stmt_);
        for (std::size_t i = 0; i < params.size(); ++i) {
            const int column = static_cast<int>(i) + 1;
            const int rc = std::visit(
                [&](const auto& v) -> int {
                    using T = std::decay_t<decltype(v)>;
                    if constexpr (std::is_same_v<T, std::monostate>) return sqlite3_bind_null(stmt_, column);
                    else if constexpr (std::is_same_v<T, std::int64_t>) return sqlite3_bind_int64(stmt_, column, v);
                    else if constexpr (std::is_same_v<T, double>) return sqlite3_bind_double(stmt_, column, v);
                    else if constexpr (std::is_same_v<T, std::string>)
                        return sqlite3_bind_text(stmt_, column, v.data(), static_cast<int>(v.size()), SQLITE_STATIC);
                    else if (v.empty())   // (a null pointer would bind NULL, not an empty blob)
                        return sqlite3_bind_zeroblob(stmt_, column, 0);
                    else
                        return sqlite3_bind_blob(stmt_, column, v.data(), static_cast<int>(v.size()), SQLITE_STATIC);
                },
                params[i]);
            if (rc != SQLITE_OK) fail(impl_.file, impl_.db, rc, "bind");
        }
    }

    // Step to the next row; false when the statement is done.
    bool step() {
        const int rc = sqlite3_step(stmt_);
        if (rc == SQLITE_ROW) return true;
        if (rc == SQLITE_DONE) return false;
        fail(impl_.file, impl_.db, rc, "step");
    }

    Row row() const {
        const int n = sqlite3_column_count(stmt_);
        Row out;
        out.reserve(static_cast<std::size_t>(n));
        for (int i = 0; i < n; ++i) {
            switch (sqlite3_column_type(stmt_, i)) {
                case SQLITE_INTEGER: out.emplace_back(static_cast<std::int64_t>(sqlite3_column_int64(stmt_, i))); break;
                case SQLITE_FLOAT: out.emplace_back(sqlite3_column_double(stmt_, i)); break;
                case SQLITE_TEXT:
                    out.emplace_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt_, i)),
                                                 static_cast<std::size_t>(sqlite3_column_bytes(stmt_, i))));
                    break;
                case SQLITE_BLOB: {
                    const auto* p = static_cast<const std::uint8_t*>(sqlite3_column_blob(stmt_, i));
                    out.emplace_back(Bytes(p, p + sqlite3_column_bytes(stmt_, i)));
                    break;
                }
                default: out.emplace_back(std::monostate{}); break;
            }
        }
        return out;
    }

private:
    Database::Impl& impl_;
    Database::Impl::Cached* cached_ = nullptr;
    sqlite3_stmt* stmt_ = nullptr;
};

}  // namespace

Database::Database(std::unique_ptr<Impl> impl) : impl_(std::move(impl)) {}
Database::Database(Database&&) noexcept = default;
Database& Database::operator=(Database&&) noexcept = default;
Database::~Database() = default;

Database Database::open(const std::filesystem::path& file, std::chrono::milliseconds busy_timeout) {
    if (file.has_parent_path()) fs::make_dirs(file.parent_path());
    auto impl = std::make_unique<Impl>();
    impl->file = file;
    const int rc = sqlite3_open_v2(fs::to_text(file).c_str(), &impl->db,
                                   SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX, nullptr);
    if (rc != SQLITE_OK) fail(file, impl->db, rc, "open");
    sqlite3_busy_timeout(impl->db, static_cast<int>(busy_timeout.count()));
    return Database(std::move(impl));
}

const std::filesystem::path& Database::path() const { return impl_->file; }

void Database::exec(std::string_view sql, const std::vector<Value>& params) {
    Statement st(*impl_, sql);
    st.bind(params);
    while (st.step()) {
    }
}

std::vector<Row> Database::query(std::string_view sql, const std::vector<Value>& params) {
    Statement st(*impl_, sql);
    st.bind(params);
    std::vector<Row> rows;
    while (st.step()) rows.push_back(st.row());
    return rows;
}

void Database::exec_many(std::string_view sql, const std::vector<std::vector<Value>>& rows) {
    Transaction tx(*this);
    Statement st(*impl_, sql);
    for (const auto& params : rows) {
        st.bind(params);
        while (st.step()) {
        }
    }
    tx.commit();
}

std::int64_t Database::changes() const { return sqlite3_changes64(impl_->db); }

Database::Transaction::Transaction(Database& db) : db_(&db) { db.exec("BEGIN IMMEDIATE"); }

void Database::Transaction::commit() {
    if (db_ == nullptr) return;
    Database* db = db_;
    db_ = nullptr;
    db->exec("COMMIT");
}

Database::Transaction::~Transaction() {
    if (db_ == nullptr) return;
    // Roll back; a failure here (the connection is already broken) must not escape a destructor.
    sqlite3_exec(db_->impl_->db, "ROLLBACK", nullptr, nullptr, nullptr);
}

}  // namespace cb::base::sqlite
