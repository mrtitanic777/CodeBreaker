// build/x86link/directives/reader.hpp -- the reader of wlink's directive language (docs/X86LINK_DESIGN.md phase 4b): the
// sources a link's directives come from and the token read from them, as wlink reads them (it ports wlink's cmdutils:
// GetTokenEx's state machine, MakeToken, the sources' stack).
//
// The sources are a stack: the command line (or the buffer an IDE hands over), each `@file`, an environment variable a
// `%NAME%` or an `@NAME` names, and a system block being run. Reading past a source's end goes back to the one under it
// (`pop`); a `^` goes back to the one under it for the next token only, `forward` returns to where it was. Each source
// keeps its own text (escapes are decoded in it as they are read, as wlink does) and the cursor it was left at.
//
// The token is a span of the current source's text -- or a text of its own, when a handler puts one there (a wildcard's
// expansion, a path element). `push_back` has the next `get` give the same token again ("thumb" in wlink).
#pragma once

#include <cstddef>
#include <cstdint>
#include <list>
#include <optional>
#include <string>
#include <string_view>

#include "build/x86link/link/cbcxx.h"

namespace cb::x86link::directives {

// What must come before the token: none (and the token must not start with `,` or `=`), one of the separators, or the
// end of the command (`end`: get succeeds only there). `space` reads a token for a system block's text (`@`, `^`, `%`,
// `#` and `=`, `,` ... kept as they are); `dot_ext` reads after a `.` and may be empty.
enum class Sep : std::uint8_t { none, comma, equals, period, end, quote, paren, space, percent, dot_ext, lcurly, rcurly };

// How the token is read: `include_dot` keeps `.` in it (a keyword, a number with a `.`); `file_name` keeps `.`, `@` and
// `\` and does not decode escapes, and wildcards in it are expanded (process_list).
enum class Want : std::uint8_t { normal = 0, include_dot = 1, file_name = 2 };
constexpr Want operator|(Want a, Want b) { return static_cast<Want>(static_cast<unsigned>(a) | static_cast<unsigned>(b)); }
constexpr bool has(Want w, Want bit) { return (static_cast<unsigned>(w) & static_cast<unsigned>(bit)) != 0; }

// Where a source's text comes from. A message's location names a file and its line only for a command file or the
// command line; for the others it names the system block or the environment variable.
enum class SourceKind : std::uint8_t { command_line, buffered, environment, system };

class Reader;

// One source of directives.
class Source {
public:
    const std::string& name() const { return name_; }   // the file, environment variable or system block; "" for none
    SourceKind kind() const { return kind_; }
    std::string symbol_prefix;                          // Novell `module`/`import` lists: the prefix a `(PREFIX)` gave

private:
    friend class Reader;
    enum class Place : std::uint8_t { midst, end_of_line, end_of_file, end_of_command };
    struct Cursor {                     // where reading is in the text, and the token there
        std::size_t next = 0;           // the next character to read
        std::size_t start = 0;          // the token's first character
        std::size_t length = 0;
        std::optional<std::string> own; // a text the token was given (it then is that, not the span)
        Place where = Place::midst;
        bool pushed_back = false;
        bool locked = false;            // a source that must not be left at its end (an export file: only it is read)
        bool quoted = false;
        bool skip_separator = false;    // Novell import files: the next comma-separated token needs no comma
        unsigned line = 1;
        bool none = true;               // no token read yet (or the one there was taken away)
    };
    std::string name_;
    std::string text_;                  // (always ends in '\0' past size(), as std::string does)
    SourceKind kind_ = SourceKind::buffered;
    int file_ = -1;                     // the f_handle the source was read from, closed when it is done (-1: none)
    Cursor saved_;                      // the cursor while another source is current
};

class Reader {
public:
    Reader() = default;
    Reader(const Reader&) = delete;
    Reader& operator=(const Reader&) = delete;
    ~Reader();

    // ---- the token
    bool get(Sep required, Want want = Want::normal);
    // ... and with a reset point: reading past the end of that source's file is said in *reset and fails (a list the
    // file ends)
    bool get(Sep required, Want want, const Source* reset_point, bool* reset);
    std::string_view text() const;
    std::string str() const { return std::string(text()); }
    // The token's character i, or past its end the source's next characters (wlink reads `Token.this_[len]`).
    char at(std::size_t i) const;
    void give_text(std::string text);   // the token is this text from now on
    void clear_token() { cur_.none = true; cur_.own.reset(); }   // no token (wlink: Token.this_ = NULL)
    bool has_token() const { return !cur_.none; }
    void drop_front(std::size_t n);     // the token less its first n characters
    void push_back() { cur_.pushed_back = true; }
    void forget_push_back() { cur_.pushed_back = false; }
    bool pushed_back() const { return cur_.pushed_back; }
    bool quoted() const { return cur_.quoted; }
    void set_quoted(bool on) { cur_.quoted = on; }
    void lock(bool on) { cur_.locked = on; }
    void skip_next_separator(bool on) { cur_.skip_separator = on; }
    bool skipping_next_separator() const { return cur_.skip_separator; }
    unsigned line() const { return cur_.line; }
    // The token starts where reading is (its length kept): what a failed `=` leaves for the message after it.
    void start_at_next() {
        cur_.own.reset();
        cur_.start = cur_.next;
    }
    // The current source, or one under it, is of this kind.
    bool reading_from(SourceKind kind) const;

    // The text not read yet on the current line, and moving past some of it (the command line's `?`).
    std::string_view rest() const;
    void advance(std::size_t n) { cur_.next += n; }
    void eat_white();

    // ---- the sources
    // A new source on top, read from its start: `text` is copied. No name for the command line.
    void push(std::string_view name, std::string_view text, SourceKind kind);
    // A command file: its whole text read from the open file (closed when the source is done).
    void push_file(int file, std::string_view name);
    // The source under the current one becomes current again; the current one is done. At the bottom source this only
    // marks the end of the command.
    void pop();
    // The source a `^` went back from is current again (wlink's RestoreParser).
    void forward();
    // Nothing more is to be read from the current source's line (a system block is run on an empty command line).
    void end_command();
    Source* current() { return at_ == sources_.end() ? nullptr : &*at_; }
    const Source* current() const { return at_ == sources_.end() ? nullptr : &*at_; }
    bool above_current() const;         // a source the current one was backed up from (`^`) is still open
    // Every source done: what is left of the stack (a `^` that was never returned from) is said.
    void finish();
    bool empty() const { return sources_.empty(); }

private:
    bool make_token(Want want, Sep separator);
    void map_escape();
    unsigned map_double_byte(unsigned char c);
    void expand_environment(Want want, Sep required);
    void new_line();
    void start_file();
    void back_up();
    void save() { if (at_ != sources_.end()) at_->saved_ = cur_; }
    void restore() { cur_ = at_->saved_; }
    char ch(std::size_t at) const;      // the current source's character (its '\0' past the end)
    void set_ch(std::size_t at, char c);
    void remove(std::size_t at, std::size_t n);
    void close(Source& s);

    std::list<Source> sources_;         // bottom first
    std::list<Source>::iterator at_ = sources_.end();
    Source::Cursor cur_;
};

// The reader of the link being run (directives/state.cpp).
Reader& reader();

}  // namespace cb::x86link::directives
