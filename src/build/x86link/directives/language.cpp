// build/x86link/directives/language.cpp -- see language.hpp. A port of wlink's cmdutils (procOne, MatchOne, ProcArgListEx,
// WildCard, CheckFence, getatol, getatoi, HaveEquals, GetLong, GetGenVersion, GetFileName, IsSystemBlock).
#include "build/x86link/directives/language.hpp"

#include <cctype>

#include "build/x86link/directives/state.hpp"
#include "build/x86link/directives/common.hpp"
#include "build/x86link/directives/cmdline.h"
#include "build/x86link/base/linkutil.h"
#include "build/x86link/messages/msg.h"
#include "pathgrp2.h"
#include "wdirent.h"
#include "wio.h"
#include "build/x86link/messages/wlnkmsg.h"

#include "clibext.h"

namespace cb::x86link::directives {

namespace {

// `word` matches `keyword`: all of the word, case aside, is the keyword's start, and the capitals of the keyword are in it.
bool keyword_matches(std::string_view keyword, std::string_view word) {
    std::size_t i = 0;
    for (;; ++i) {
        if (i == word.size()) return i == keyword.size() || !std::isupper(static_cast<unsigned char>(keyword[i]));
        if (i == keyword.size() ||
            std::tolower(static_cast<unsigned char>(word[i])) != std::tolower(static_cast<unsigned char>(keyword[i])))
            return false;
    }
}

bool process(Table table, Sep required, bool or_fail, bool subset) {
    Reader& r = reader();
    if (!r.get(required, Want::include_dot)) return false;
    const std::string_view word = r.text();
    for (const Entry& e : table) {
        if (subset && !any(e.flags & Flag::subset)) continue;
        if (!keyword_matches(e.keyword, word)) continue;
        if (!HintFormat(e.formats)) {
            LnkMsg(WRN + LOC + LINE + MSG_FORMAT_BAD_OPTION, "s", e.keyword.data());
            return true;
        }
        const bool ret = e.handler();
        cmd_flags() |= e.flags & ~Flag::subset;
        return ret;
    }
    if (or_fail) {
        Syntax();
        return true;
    }
    r.push_back();   // (the next table may have it)
    return false;
}

// The handler, for each file a wildcard in the token names (hidden, system and folder entries left out), or once.
bool wildcard(Handler handler, Want want) {
    Reader& r = reader();
    bool wild = false;
    if (has(want, Want::file_name)) {
        // (up to the token's end in its source, a quote or a blank, as wlink looks)
        for (std::size_t i = 0;; ++i) {
            const char c = r.at(i);
            if (c == '\0' || c == '\'' || c == ' ') break;
            if (c == '?' || c == '*') {
                wild = true;
                break;
            }
        }
    }
    if (!wild) return handler();
    const std::string pattern = r.str();
    DIR* dir = opendir(pattern.c_str());
    if (dir == nullptr) return handler();
    pgroup2 pg;
    _splitpath2(pattern.c_str(), pg.buffer, &pg.drive, &pg.dir, nullptr, nullptr);
    bool ret = true;
    while (const struct dirent* entry = readdir(dir)) {
        if (entry->d_attr & (_A_HIDDEN | _A_SYSTEM | _A_VOLID | _A_SUBDIR)) continue;
        char path[_MAX_PATH];
        _makepath(path, pg.drive, pg.dir, entry->d_name, nullptr);
        r.give_text(path);
        if (!handler()) {
            r.clear_token();
            r.forget_push_back();
            ret = false;
            break;
        }
    }
    closedir(dir);
    return ret;
}

// A `}` ending a `{...}` list: taken (from a token pushed back, or read).
bool fence() {
    Reader& r = reader();
    if (r.pushed_back()) {
        if (r.quoted()) return false;   // (none inside quotes)
        if (r.at(0) == '}') {
            r.drop_front(1);   // (as wlink: the token's start moves past it)
            return true;
        }
        return false;
    }
    return r.get(Sep::rcurly);
}

Number to_number(std::string_view t, std::uint32_t& out) {
    if (t.empty()) return Number::not_a_number;
    unsigned long value = 0;
    unsigned radix = 10;
    bool got_digit = false;
    if (t.size() >= 2 && t[0] == '0' && (t[1] == 'x' || t[1] == 'X')) {
        radix = 16;
        t.remove_prefix(2);
    }
    for (std::size_t i = 0; i < t.size(); ++i) {
        const int c = std::tolower(static_cast<unsigned char>(t[i]));
        const bool last = i + 1 == t.size();
        if (c == 'k' || c == 'm') {   // 64k, 1m
            if (!last || !got_digit) return Number::not_a_number;
            value <<= c == 'k' ? 10 : 20;
            continue;
        }
        const bool digit = std::isdigit(c) != 0;
        if (!(radix == 16 ? std::isxdigit(c) != 0 : digit)) return Number::not_a_number;
        value = value * radix + (digit ? c - '0' : c - 'a' + 10);
        got_digit = true;
    }
    out = static_cast<std::uint32_t>(value);
    return Number::ok;
}

}  // namespace

bool process_one(Table table, Sep required) { return process(table, required, false, false); }
bool process_one_subset(Table table, Sep required) { return process(table, required, false, true); }
bool process_one_or_fail(Table table, Sep required) { return process(table, required, true, false); }

bool matches(Table table, std::string_view word) {
    for (const Entry& e : table) {
        if (keyword_matches(e.keyword, word)) return true;
    }
    return false;
}

bool process_list(Handler handler, Want want, const Source* reset_point) {
    Reader& r = reader();
    bool reset = false;   // the reset point's file ended
    if (r.get(Sep::lcurly, want, reset_point, &reset)) {
        for (;;) {
            if (!wildcard(handler, want)) return false;
            if (fence()) break;
            if (!r.get(Sep::none, want, reset_point, &reset)) {
                LnkMsg(ERR + LOC + LINE + MSG_BAD_CURLY_LIST, nullptr);
                break;
            }
        }
        return true;
    }
    if (reset_point != nullptr && reset) return true;
    if (!r.get(Sep::none, want, reset_point, &reset)) return false;
    do {
        if (reset_point != nullptr && reset) return true;
        if (!wildcard(handler, want)) return false;
    } while (r.get(Sep::comma, want, reset_point, &reset));
    return true;
}

Number number(std::uint32_t& value) { return to_number(reader().text(), value); }

Number number16(std::uint16_t& value) {
    std::uint32_t v = 0;
    const Number n = number(v);
    if (n != Number::ok) return n;
    if (v > 0xffff) return Number::too_big;
    value = static_cast<std::uint16_t>(v);
    return Number::ok;
}

bool have_equals(Want want) {
    Reader& r = reader();
    if (!r.get(Sep::equals, want)) {
        r.start_at_next();
        r.get(Sep::none, want);   // (the token that is there instead, for the message)
        return false;
    }
    return true;
}

bool get_number(std::uint32_t& value) {
    if (!have_equals(Want::normal)) return false;
    std::uint32_t v = 0;
    if (number(v) != Number::ok) return false;
    value = v;
    return true;
}

VersionPart get_version(Version& v, VersionPart wanted, bool novell_revision) {
    Reader& r = reader();
    VersionPart got = VersionPart::none;
    if (!r.get(Sep::equals)) return got;
    if (wanted == VersionPart::none) return got;
    auto bad = [&] { LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", v.what); };
    std::uint32_t value = 0;
    if (number(value) != Number::ok) {
        bad();
        return got;
    }
    const std::uint32_t major_limit = v.major, minor_limit = v.minor, revision_limit = v.revision;
    v.major = v.minor = v.revision = 0;
    got = VersionPart::major;   // (from here every result counts, warned of or not)
    if (has(wanted, VersionPart::major)) {
        if (major_limit != 0 && value > major_limit) {
            bad();
            return got;
        }
        v.major = value;
    }
    if (!has(wanted, VersionPart::minor) && !has(wanted, VersionPart::revision)) return got;
    if (!r.get(Sep::period)) return got;   // (no minor: that is fine)
    if (number(value) != Number::ok) {
        bad();
        return got;
    }
    got = got | VersionPart::minor;
    if (has(wanted, VersionPart::minor)) {
        if (minor_limit != 0 && value > minor_limit) {
            bad();
            return got;
        }
        v.minor = value;
    }
    if (!has(wanted, VersionPart::revision)) return got;
    if (!r.get(Sep::period)) return got;
    const Number n = number(value);
    if (novell_revision) {   // Netware: a revision 0-26, or a letter
        if (n == Number::not_a_number && r.text().size() == 1) {
            value = static_cast<std::uint32_t>(static_cast<char>(std::tolower(static_cast<unsigned char>(r.at(0)))) - 'a' + 1);
        } else if (n == Number::not_a_number) {
            bad();
            return got;
        }
    } else if (n != Number::ok) {
        bad();
        return got;
    }
    got = got | VersionPart::revision;
    if (revision_limit != 0 && value > revision_limit) {
        bad();
        return got;
    }
    v.revision = value;
    return got;
}

char* file_name(char** member) {
    Reader& r = reader();
    const std::string object = r.str();
    if (r.get(Sep::paren, Want::include_dot)) {   // LIBRARY(MEMBER)
        const std::string_view m = r.text();
        size_t base_length = 0;
        const char* base = GetBaseName(m.data(), m.size(), &base_length);
        *member = MemToStringSafe(base, base_length);
        return FileName(object.data(), object.size(), E_LIBRARY, false);
    }
    *member = nullptr;
    return FileName(object.data(), object.size(), E_OBJECT, false);
}

bool in_system_block() {
    return reader().reading_from(SourceKind::system);
}

}  // namespace cb::x86link::directives
