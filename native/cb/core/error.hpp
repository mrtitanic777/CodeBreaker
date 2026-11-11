// cb/core/error.hpp -- CodeBreaker's one error vocabulary.
//
// Every exception CodeBreaker throws derives from cb::Error (a std::runtime_error). The *type* is the
// contract; the message is for humans and is free to change. `cb::Result<T>` carries the same errors by
// value for failure that is routine on a hot path (probing a file format, trial decoding): the caller
// checks `ok()`, and `value()` throws the stored error if it was not checked. See docs/CPP_DESIGN.md 3.6.
#pragma once

#include <concepts>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>

namespace cb {

// The exit codes of codebreaker.exe. `cb::app::main` is the only place that maps errors to them.
inline constexpr int kExitOk = 0;
inline constexpr int kExitFailed = 1;     // the command ran and reports failure (a gate failed, a build failed)
inline constexpr int kExitUsage = 2;      // the command line or API was misused
inline constexpr int kExitNotFound = 3;   // a requested thing does not exist
inline constexpr int kExitInternal = 70;  // an invariant of CodeBreaker's own broke (a bug)

// Base of every CodeBreaker exception.
class Error : public std::runtime_error {
public:
    explicit Error(const std::string& message) : std::runtime_error(message) {}
    // The exit code this error maps to at the top level.
    virtual int exit_code() const { return kExitFailed; }
    // A short stable name of the error's type ("FormatError"), for logs and test expectations.
    virtual std::string_view kind() const { return "Error"; }
    // Throw a copy of this error with its dynamic type intact (turns a stored Result error back into
    // an exception). Every subclass overrides it with the same one line.
    [[noreturn]] virtual void raise() const { throw *this; }
    // A heap copy with the dynamic type intact (how Result stores an error).
    virtual std::shared_ptr<const Error> clone() const { return std::make_shared<Error>(*this); }
};

// Each subclass repeats these three overrides so that the dynamic type survives copying.
#define CB_ERROR_BOILERPLATE(Type)                                                                  \
    std::string_view kind() const override { return #Type; }                                        \
    [[noreturn]] void raise() const override { throw *this; }                                       \
    std::shared_ptr<const Error> clone() const override { return std::make_shared<Type>(*this); }

// A path as UTF-8 text with '/' separators (what messages show).
std::string path_text(const std::filesystem::path& p);

// An operating-system level failure on a file or device: path + OS error code.
class IoError : public Error {
public:
    IoError(const std::filesystem::path& path, int os_code, const std::string& what)
        : Error(path_text(path) + ": " + what + " (os error " + std::to_string(os_code) + ")"),
          path_(path), os_code_(os_code) {}
    const std::filesystem::path& path() const { return path_; }
    int os_code() const { return os_code_; }
    CB_ERROR_BOILERPLATE(IoError)

private:
    std::filesystem::path path_;
    int os_code_;
};

// The file or directory does not exist.
class NotFoundError : public IoError {
public:
    NotFoundError(const std::filesystem::path& path, int os_code, const std::string& what = "not found")
        : IoError(path, os_code, what) {}
    int exit_code() const override { return kExitNotFound; }
    CB_ERROR_BOILERPLATE(NotFoundError)
};

// The file exists but may not be opened this way.
class AccessError : public IoError {
public:
    AccessError(const std::filesystem::path& path, int os_code, const std::string& what = "access denied")
        : IoError(path, os_code, what) {}
    CB_ERROR_BOILERPLATE(AccessError)
};

// A binary or data file is malformed: which reader noticed, at what offset, and what was wrong.
class FormatError : public Error {
public:
    FormatError(std::string_view reader, std::uint64_t offset, const std::string& what)
        : Error(std::string(reader) + " at offset " + hex(offset) + ": " + what),
          reader_(reader), offset_(offset) {}
    const std::string& reader() const { return reader_; }
    std::uint64_t offset() const { return offset_; }
    CB_ERROR_BOILERPLATE(FormatError)

private:
    static std::string hex(std::uint64_t v);
    std::string reader_;
    std::uint64_t offset_;
};

// A read ran past the end of the data.
class TruncatedError : public FormatError {
public:
    TruncatedError(std::string_view reader, std::uint64_t offset, std::uint64_t wanted, std::uint64_t available)
        : FormatError(reader, offset,
                      "truncated: wanted " + std::to_string(wanted) + " bytes, " + std::to_string(available) +
                          " available"),
          wanted_(wanted), available_(available) {}
    std::uint64_t wanted() const { return wanted_; }
    std::uint64_t available() const { return available_; }
    CB_ERROR_BOILERPLATE(TruncatedError)

private:
    std::uint64_t wanted_;
    std::uint64_t available_;
};

// The data is valid but this variant of the format is not handled.
class UnsupportedError : public FormatError {
public:
    using FormatError::FormatError;
    CB_ERROR_BOILERPLATE(UnsupportedError)
};

// Source or text input did not parse: file, line, column (1-based; 0 = unknown).
class ParseError : public Error {
public:
    ParseError(std::string_view file, std::uint32_t line, std::uint32_t column, const std::string& what)
        : Error(location(file, line, column) + ": " + what), file_(file), line_(line), column_(column) {}
    const std::string& file() const { return file_; }
    std::uint32_t line() const { return line_; }
    std::uint32_t column() const { return column_; }
    CB_ERROR_BOILERPLATE(ParseError)

private:
    static std::string location(std::string_view file, std::uint32_t line, std::uint32_t column);
    std::string file_;
    std::uint32_t line_;
    std::uint32_t column_;
};

// An external program failed (or could not be started): tool, exit code, captured output.
class ToolError : public Error {
public:
    ToolError(std::string_view tool, std::uint32_t exit_code, std::string_view output,
              const std::string& what = "failed")
        : Error(std::string(tool) + ": " + what + " (exit " + std::to_string(exit_code) + ")"),
          tool_(tool), tool_exit_(exit_code), output_(output) {}
    const std::string& tool() const { return tool_; }
    std::uint32_t tool_exit_code() const { return tool_exit_; }
    const std::string& output() const { return output_; }
    CB_ERROR_BOILERPLATE(ToolError)

private:
    std::string tool_;
    std::uint32_t tool_exit_;
    std::string output_;
};

// An external program ran past its time limit and was terminated.
class TimeoutError : public ToolError {
public:
    TimeoutError(std::string_view tool, std::string_view output) : ToolError(tool, 0xFFFFFFFFu, output, "timed out") {}
    CB_ERROR_BOILERPLATE(TimeoutError)
};

// The guest faulted in a way the harness cannot continue from (unmapped memory, bad opcode, cap exceeded).
class EmulationError : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(EmulationError)
};

// The command line or an API was misused.
class UsageError : public Error {
public:
    using Error::Error;
    int exit_code() const override { return kExitUsage; }
    CB_ERROR_BOILERPLATE(UsageError)
};

// An invariant of CodeBreaker's own was violated: a bug. Never caught except at the top level.
class InternalError : public Error {
public:
    using Error::Error;
    int exit_code() const override { return kExitInternal; }
    CB_ERROR_BOILERPLATE(InternalError)
};

// The exit code for any exception: Error types say their own; anything else is an internal error.
int exit_code_of(const std::exception& e);

// Throw InternalError carrying file:line. Backs CB_ASSERT.
[[noreturn]] void assertion_failed(const char* condition, std::string_view why, const char* file, int line);

// Always-on invariant check (not for input validation: throw a FormatError/UsageError for that).
#define CB_ASSERT(cond, why)                                                                        \
    do {                                                                                            \
        if (!(cond)) ::cb::assertion_failed(#cond, (why), __FILE__, __LINE__);                      \
    } while (false)

// ---------------------------------------------------------------------------------------------------
// Result<T>: a value or an Error, for failure that is routine and expected.
// ---------------------------------------------------------------------------------------------------

// A failure on its way into a Result. Built from any cb::Error subtype (the dynamic type is kept) or
// from the error of another Result.
class Failure {
public:
    template <class E>
        requires std::derived_from<std::remove_cvref_t<E>, Error>
    Failure(E&& e) : error_(std::make_shared<std::remove_cvref_t<E>>(std::forward<E>(e))) {}   // NOLINT: implicit
    explicit Failure(std::shared_ptr<const Error> e) : error_(std::move(e)) {}
    const std::shared_ptr<const Error>& share() const { return error_; }

private:
    std::shared_ptr<const Error> error_;
};

template <class T>
class [[nodiscard]] Result {
public:
    Result(T value) : state_(std::move(value)) {}                       // NOLINT: implicit on purpose
    Result(Failure failure) : state_(failure.share()) {}                 // NOLINT: implicit on purpose
    template <class E>
        requires std::derived_from<std::remove_cvref_t<E>, Error>
    Result(E&& e) : state_(Failure(std::forward<E>(e)).share()) {}       // NOLINT: implicit on purpose

    bool ok() const { return state_.index() == 0; }
    explicit operator bool() const { return ok(); }
    // The value; throws the stored error (with its own type) if there is none.
    T& value() & { check(); return std::get<0>(state_); }
    const T& value() const& { check(); return std::get<0>(state_); }
    T&& value() && { check(); return std::move(std::get<0>(state_)); }
    // The error; asserts if the result is ok.
    const Error& error() const { return *shared_error(); }
    const std::shared_ptr<const Error>& shared_error() const {
        CB_ASSERT(!ok(), "Result::error() on a successful result");
        return std::get<1>(state_);
    }
    T value_or(T fallback) const { return ok() ? std::get<0>(state_) : std::move(fallback); }

private:
    void check() const { if (!ok()) std::get<1>(state_)->raise(); }
    std::variant<T, std::shared_ptr<const Error>> state_;
};

// Result<void>: success or an error.
template <>
class [[nodiscard]] Result<void> {
public:
    Result() = default;
    Result(Failure failure) : error_(failure.share()) {}                 // NOLINT: implicit on purpose
    template <class E>
        requires std::derived_from<std::remove_cvref_t<E>, Error>
    Result(E&& e) : error_(Failure(std::forward<E>(e)).share()) {}       // NOLINT: implicit on purpose
    bool ok() const { return error_ == nullptr; }
    explicit operator bool() const { return ok(); }
    void value() const { if (!ok()) error_->raise(); }
    const Error& error() const { return *shared_error(); }
    const std::shared_ptr<const Error>& shared_error() const {
        CB_ASSERT(!ok(), "Result::error() on a successful result");
        return error_;
    }

private:
    std::shared_ptr<const Error> error_;
};

// CB_TRY(expr): evaluate a Result-returning expression; on failure return its error from the enclosing
// function (which must itself return a Result).
//     CB_TRY(write_header(out));
// CB_TRY_ASSIGN(decl, expr): the same, and on success initialise `decl` with the value.
//     CB_TRY_ASSIGN(auto header, parse_header(view));
#define CB_TRY(expr)                                                                                \
    do {                                                                                            \
        auto cb_try_result_ = (expr);                                                               \
        if (!cb_try_result_.ok()) return ::cb::Failure(cb_try_result_.shared_error());              \
    } while (false)
#define CB_TRY_CAT2_(a, b) a##b
#define CB_TRY_CAT_(a, b) CB_TRY_CAT2_(a, b)
#define CB_TRY_ASSIGN(decl, expr)                                                                   \
    auto CB_TRY_CAT_(cb_try_tmp_, __LINE__) = (expr);                                               \
    if (!CB_TRY_CAT_(cb_try_tmp_, __LINE__).ok())                                                   \
        return ::cb::Failure(CB_TRY_CAT_(cb_try_tmp_, __LINE__).shared_error());                    \
    decl = std::move(CB_TRY_CAT_(cb_try_tmp_, __LINE__)).value()

}  // namespace cb
