// cb/base/sqlite.hpp -- a small typed wrapper over the SQLite amalgamation.
//
// SQLite is CodeBreaker's one embedded database: the result cache keeps its entries in it (base/cache.hpp) and
// the function store keeps its facts in it. A Database is one open file; statements are plain text with `?`
// placeholders, parameters and result cells are a `Value` (SQLite's own five storage classes). Every failure is a
// cb::IoError carrying the file, SQLite's result code and its message.
//
// A Database is not thread-safe: the owner serialises access (the cache holds one mutex for it). The connection
// is in autocommit mode; group writes with a Transaction (or exec_many, which is one).
#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::base::sqlite {

// One SQLite value: NULL, INTEGER, REAL, TEXT (UTF-8) or BLOB. This really is a closed union of the database's
// storage classes -- a column may hold any of them.
using Value = std::variant<std::monostate, std::int64_t, double, std::string, Bytes>;
using Row = std::vector<Value>;

class Database {
public:
    // Open (creating the file and its folder if needed). `busy_timeout` is how long a statement waits for another
    // connection's lock before failing.
    static Database open(const std::filesystem::path& file,
                         std::chrono::milliseconds busy_timeout = std::chrono::seconds(10));
    Database(Database&&) noexcept;
    Database& operator=(Database&&) noexcept;
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    ~Database();

    const std::filesystem::path& path() const;

    // Run a statement to its end, discarding any rows.
    void exec(std::string_view sql, const std::vector<Value>& params = {});
    // Run a statement and return all its rows.
    std::vector<Row> query(std::string_view sql, const std::vector<Value>& params = {});
    // Run one statement once per parameter row, all in a single transaction.
    void exec_many(std::string_view sql, const std::vector<std::vector<Value>>& rows);
    // Rows changed by the most recent INSERT/UPDATE/DELETE.
    std::int64_t changes() const;

    // An explicit transaction (nesting is not supported). Destroyed without commit(), it rolls back.
    class Transaction {
    public:
        explicit Transaction(Database& db);
        Transaction(const Transaction&) = delete;
        Transaction& operator=(const Transaction&) = delete;
        ~Transaction();
        void commit();

    private:
        Database* db_;
    };

    struct Impl;   // (an implementation detail: public only so that sqlite.cpp's helpers may name it)

private:
    explicit Database(std::unique_ptr<Impl> impl);
    std::unique_ptr<Impl> impl_;
};

}  // namespace cb::base::sqlite
