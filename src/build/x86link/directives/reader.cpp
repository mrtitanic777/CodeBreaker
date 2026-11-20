// build/x86link/directives/reader.cpp -- see reader.hpp. A port of wlink's cmdutils (GetTokenEx, MakeToken, the escapes,
// the command sources), keeping what it reads and every message it gives; positions into each source's own text take the
// place of its pointers into buffers it allocated.
#include "build/x86link/directives/reader.hpp"

#include <cctype>

#include "build/x86link/directives/state.hpp"
#include "linkstd.h"
#include "fileio.h"
#include "ideentry.h"
#include "loadfile.h"
#include "msg.h"
#include "wlnkmsg.h"

namespace cb::x86link::directives {

namespace {

bool reads_lines(SourceKind k) { return k == SourceKind::buffered || k == SourceKind::environment || k == SourceKind::system; }

}  // namespace

Reader::~Reader() {
    for (Source& s : sources_) close(s);
}

char Reader::ch(std::size_t at) const {
    const Source* s = current();
    return s == nullptr || at >= s->text_.size() ? '\0' : s->text_[at];
}

void Reader::set_ch(std::size_t at, char c) {
    if (Source* s = current(); s != nullptr && at < s->text_.size()) s->text_[at] = c;
}

void Reader::remove(std::size_t at, std::size_t n) {
    if (Source* s = current(); s != nullptr && at < s->text_.size()) s->text_.erase(at, n);
}

void Reader::close(Source& s) {
    if (s.file_ != -1 && s.file_ != NIL_FHANDLE) QClose(s.file_, s.name_.c_str());
    s.file_ = -1;
}

// ---- the token

std::string_view Reader::text() const {
    if (cur_.own) return *cur_.own;
    const Source* s = current();
    if (s == nullptr || cur_.start > s->text_.size()) return {};
    return std::string_view(s->text_).substr(cur_.start, cur_.length);
}

char Reader::at(std::size_t i) const {
    if (cur_.own) return i < cur_.own->size() ? (*cur_.own)[i] : '\0';
    return ch(cur_.start + i);
}

void Reader::give_text(std::string text) {
    cur_.own = std::move(text);
    cur_.none = false;
}

void Reader::drop_front(std::size_t n) {
    if (cur_.own) {
        cur_.own->erase(0, n);
    } else {
        cur_.start += n;
        cur_.length -= n;
    }
}

std::string_view Reader::rest() const {
    const Source* s = current();
    if (s == nullptr || cur_.next >= s->text_.size()) return {};
    return std::string_view(s->text_).substr(cur_.next);
}

void Reader::eat_white() {
    while (std::isspace(static_cast<unsigned char>(ch(cur_.next)))) ++cur_.next;
}

// A (possibly hexadecimal or octal) number at `at`; shift grows by its length.
static unsigned parse_number(const Reader& r, std::size_t at, int radix, int& shift,
                             char (Reader::*get)(std::size_t) const) {
    unsigned value = 0;
    int size = 0;
    for (;; ++at) {
        const int c = std::tolower(static_cast<unsigned char>((r.*get)(at)));
        const bool digit = std::isdigit(c) != 0;
        const bool valid = radix == 8 ? digit && c != '8' && c != '9' : std::isxdigit(c) != 0;
        if (!valid) break;
        value = value * radix + (digit ? c - '0' : c - 'a' + 10);
        ++size;
    }
    shift += size;
    return value;
}

// The character at the cursor, a backslash, made the character its C escape names; what the escape spelled is removed.
// (As wlink: an octal escape also takes the character after its digits.)
void Reader::map_escape() {
    int shift = 2;
    std::size_t at = cur_.next + 1;
    char c;
    switch (ch(at)) {
        case 'a': c = '\a'; break;
        case 'b': c = '\b'; break;
        case 'f': c = '\f'; break;
        case 'n': c = '\n'; break;
        case 'r': c = '\r'; break;
        case 't': c = '\t'; break;
        case 'v': c = '\v'; break;
        case 'x': c = static_cast<char>(parse_number(*this, ++at, 16, shift, &Reader::ch)); break;
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            c = static_cast<char>(parse_number(*this, at, 8, shift, &Reader::ch));
            break;
        default: c = ch(at); break;
    }
    set_ch(cur_.next, c);
    remove(cur_.next + 1, static_cast<std::size_t>(shift - 1));
}

// With a double-byte language chosen (`language`), a lead byte takes the byte after it with it.
unsigned Reader::map_double_byte(unsigned char c) {
    switch (cmd_flags() & Flag::language_mask) {
        case Flag::language_japanese:
            if ((c >= 0x81 && c <= 0x9F) || (c >= 0xE0 && c <= 0xFC)) {
                ++cur_.next;
                return 1;
            }
            break;
        case Flag::language_chinese:
            if (c > 0xFC) break;
            [[fallthrough]];
        case Flag::language_korean:
            if (c > 0xFD || c < 0x81) break;
            ++cur_.next;
            return 1;
        default: break;
    }
    return 0;
}

bool Reader::make_token(Want want, Sep separator) {
    cur_.own.reset();
    cur_.none = false;
    cur_.start = cur_.next;
    std::size_t len = 0;
    const bool forcematch = separator == Sep::quote || separator == Sep::paren || separator == Sep::percent;
    const bool keepspecial = separator == Sep::space || separator == Sep::dot_ext;
    if (separator == Sep::dot_ext) {   // a token after a `.` may be empty (wlib files): back up one, as wlink does
        --len;
        --cur_.next;
    }
    if (ch(cur_.next) == '\\' && separator == Sep::quote && !has(want, Want::file_name)) map_escape();
    len += map_double_byte(static_cast<unsigned char>(ch(cur_.next)));
    bool hitmatch = false;
    for (bool quit = false; !quit;) {
        ++len;
        const char c = ch(++cur_.next);
        switch (c) {
            case '\'':
                if (separator == Sep::quote) {
                    ++cur_.next;   // the end quote is not the next token's
                    hitmatch = quit = true;
                }
                break;
            case ')':
                if (separator == Sep::paren) {
                    ++cur_.next;
                    hitmatch = quit = true;
                }
                break;
            case '%':
                if (separator == Sep::percent) {
                    ++cur_.next;
                    hitmatch = quit = true;
                }
                break;
            case '.':
                if (has(want, Want::include_dot) || has(want, Want::file_name)) break;
                if (!forcematch) quit = true;
                break;
            case '@':
                if (has(want, Want::file_name)) break;
                [[fallthrough]];
            case '{': case '}': case '(': case ',': case '=': case '#':
                if (keepspecial) break;
                [[fallthrough]];
            case '\t': case ' ':
                if (!forcematch) quit = true;
                break;
            case '\\':
                if (has(want, Want::file_name)) break;
                if (separator == Sep::quote) map_escape();
                break;
            case '\0': case '\r': case '\n':
                quit = true;
                break;
            default:
                len += map_double_byte(static_cast<unsigned char>(c));
                break;
        }
    }
    cur_.length = len;
    return !(forcematch && !hitmatch);   // (the end quote, paren or percent not found before the token's end)
}

void Reader::expand_environment(Want want, Sep required) {
    ++cur_.next;
    if (!make_token(Want::include_dot, Sep::percent)) LnkMsg(FTL + LOC + LINE + MSG_ENV_NAME_INCORRECT, nullptr);
    const std::string name = str();
    const char* value = GetEnvString(name.c_str());
    if (value == nullptr) {
        LnkMsg(WRN + LOC + LINE + MSG_ENV_NOT_FOUND, "s", name.c_str());
        return;
    }
    // the variable's value, then what follows the `%NAME%` to the token's end, read as a source of its own
    make_token(want, required);
    std::size_t length = cur_.length;
    if (required == Sep::quote && at(length) == '\'') ++length;
    std::string text = value;
    for (std::size_t i = 0; i < length; ++i) text += at(i);
    push(name, text, SourceKind::environment);
}

void Reader::new_line() {
    const Source* s = current();
    if (s != nullptr && reads_lines(s->kind())) {
        cur_.where = Source::Place::midst;
        for (; ch(cur_.next) != '\n'; ++cur_.next) {
            if (ch(cur_.next) == '\0') {
                cur_.where = Source::Place::end_of_file;
                break;
            }
        }
        ++cur_.next;
    } else {   // the command line is one line
        cur_.where = Source::Place::end_of_command;
        cur_.next = 0;
    }
}

void Reader::start_file() {
    const std::string_view t = text();
    char* fname = FileName(t.data(), t.size(), E_NONE, false);
    const f_handle file = QObjOpen(fname);
    if (file == NIL_FHANDLE) {
        MemFree(fname);
        const std::string name = str();   // not a file: an environment variable of that name, if there is one
        if (const char* value = GetEnvString(name.c_str())) {
            push(name, value, SourceKind::environment);
        } else {
            LnkMsg(ERR + LOC + LINE + MSG_CANT_OPEN_NO_REASON, "s", name.c_str());
            Suicide();
        }
    } else {
        push_file(file, fname);
        MemFree(fname);
    }
}

void Reader::back_up() {
    if (at_ == sources_.begin()) {
        LnkMsg(WRN + LOC + LINE + MSG_NO_PREVIOUS_INPUT, nullptr);
        return;
    }
    save();
    --at_;
    restore();
}

bool Reader::get(Sep required, Want want) { return get(required, want, nullptr, nullptr); }

bool Reader::get(Sep req, Want want, const Source* reset_point, bool* reset) {
    if (cur_.pushed_back) {
        cur_.pushed_back = false;
        if (cur_.quoted) return true;
        cur_.next = cur_.start;   // the token again
    }
    bool need_sep = true;
    for (;;) {
        // Novell import files may leave out the comma after a `(PREFIX)`: the next token then needs none
        if (cur_.skip_separator && req == Sep::comma) {
            cur_.skip_separator = false;
            need_sep = false;
        }
        const SourceKind kind = current() != nullptr ? current()->kind() : SourceKind::command_line;
        switch (cur_.where) {
            case Source::Place::midst: {
                eat_white();
                const char c = ch(cur_.next);
                switch (c) {
                    case '\0':
                        if (reads_lines(kind)) {
                            cur_.where = Source::Place::end_of_file;
                            break;
                        }
                        [[fallthrough]];
                    case '\n':
                        if (reads_lines(kind)) {
                            ++cur_.next;
                        } else {
                            cur_.where = Source::Place::end_of_command;
                        }
                        ++cur_.line;
                        break;
                    case '@':
                        if (req != Sep::space) {
                            ++cur_.next;
                            get(Sep::none, Want::file_name);
                            start_file();
                            break;
                        }
                        --cur_.next;   // (a system block's text keeps it)
                        {
                            const bool ret = make_token(want, req);
                            cur_.quoted = false;
                            return ret;
                        }
                    case '#':   // a comment, to the line's end
                        cur_.where = Source::Place::end_of_line;
                        ++cur_.line;
                        break;
                    case '^':
                        if (req != Sep::space) {
                            ++cur_.next;
                            back_up();
                            break;
                        }
                        --cur_.next;
                        {
                            const bool ret = make_token(want, req);
                            cur_.quoted = false;
                            return ret;
                        }
                    case '%':
                        if (req != Sep::space) {
                            expand_environment(want, req);
                            break;
                        }
                        [[fallthrough]];
                    default:
                        if (need_sep) {
                            cur_.quoted = false;
                            switch (req) {
                                case Sep::none:
                                    if (c == ',' || c == '=') return false;
                                    break;
                                case Sep::comma:
                                    if (c != ',') return false;
                                    ++cur_.next;
                                    break;
                                case Sep::equals:
                                    if (c != '=') return false;
                                    ++cur_.next;
                                    break;
                                case Sep::period:
                                case Sep::dot_ext:
                                    if (c != '.') return false;
                                    ++cur_.next;
                                    break;
                                case Sep::paren:
                                    if (c != '(') return false;
                                    ++cur_.next;
                                    break;
                                case Sep::lcurly:
                                    if (c != '{') return false;
                                    ++cur_.next;
                                    break;
                                case Sep::quote:
                                    if (c != '\'') return false;
                                    ++cur_.next;
                                    cur_.quoted = true;
                                    break;
                                case Sep::rcurly:
                                    if (c != '}') return false;
                                    ++cur_.next;
                                    return true;
                                case Sep::end: return false;
                                case Sep::space:
                                case Sep::percent: break;
                            }
                            need_sep = false;
                            eat_white();
                        } else {   // the token itself
                            if (c == '\'' && req != Sep::paren && req != Sep::space) {
                                req = Sep::quote;   // a quoted token
                                ++cur_.next;
                                cur_.quoted = true;
                                if (has(want, Want::file_name)) break;   // (read again: a %NAME% in a quoted file name)
                            }
                            return make_token(want, req);
                        }
                        break;
                }
                break;
            }
            case Source::Place::end_of_line: new_line(); break;
            case Source::Place::end_of_file:
                if (cur_.locked) return false;
                pop();
                if (cur_.pushed_back) {
                    cur_.pushed_back = false;
                    cur_.next = cur_.start;
                }
                cur_.quoted = false;
                if (reset_point != nullptr && current() == reset_point) {
                    if (ch(cur_.next) == ',') break;
                    if (reset != nullptr) *reset = true;   // the file a directive's list was in has ended
                    return false;
                }
                break;
            case Source::Place::end_of_command:
                if (above_current()) {
                    forward();
                    break;
                }
                cur_.quoted = false;
                return req == Sep::end;
        }
    }
}

// ---- the sources

void Reader::push(std::string_view name, std::string_view text, SourceKind kind) {
    // the token read so far stays what it was (wlink's points into the source it was read from)
    std::string kept(this->text());
    save();
    Source s;
    s.name_ = std::string(name);
    s.kind_ = kind;
    s.text_ = std::string(text.substr(0, text.find('\0')));   // (wlink copies it as a C string)
    at_ = sources_.insert(at_ == sources_.end() ? sources_.end() : std::next(at_), std::move(s));
    const bool skip = cur_.skip_separator, none = cur_.none;
    cur_ = Source::Cursor{};
    cur_.own = std::move(kept);
    cur_.length = cur_.own->size();
    cur_.skip_separator = skip;
    cur_.none = none;
}

void Reader::push_file(int file, std::string_view name) {
    const std::string fname(name);
    unsigned long size = QIsDevice(file) ? 0x10000 : QFileSize(file);
    std::string text(size, '\0');
    size_t got = size == 0 ? 0 : QRead(file, text.data(), size, fname.c_str());
    if (got == IOERROR) got = 0;
    text.resize(got);
    push(fname, text, SourceKind::buffered);
    at_->file_ = file;
}

void Reader::pop() {
    if (at_ == sources_.end()) return;
    if (at_ == sources_.begin()) {   // the last source is not done: the command has ended
        cur_.where = Source::Place::end_of_command;
        return;
    }
    const auto done = at_;
    --at_;
    close(*done);
    sources_.erase(done);
    restore();
}

void Reader::forward() {
    if (at_ == sources_.end() || std::next(at_) == sources_.end()) return;
    save();
    ++at_;
    restore();
}

void Reader::end_command() { cur_.where = Source::Place::end_of_command; }

bool Reader::above_current() const { return at_ != sources_.end() && std::next(at_) != sources_.end(); }

bool Reader::reading_from(SourceKind kind) const {
    if (at_ == sources_.end()) return false;
    for (auto it = at_;; --it) {
        if (it->kind() == kind) return true;
        if (it == sources_.begin()) return false;
    }
}

void Reader::finish() {
    if (above_current()) LnkMsg(ERR + LOC + LINE + MSG_NO_INPUT_LEFT, nullptr);
    for (Source& s : sources_) close(s);
    sources_.clear();
    at_ = sources_.end();
    cur_ = Source::Cursor{};
}

}  // namespace cb::x86link::directives
