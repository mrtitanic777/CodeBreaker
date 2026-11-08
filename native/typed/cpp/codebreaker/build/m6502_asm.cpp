// codebreaker/build/m6502_asm.cpp -- C++17 port of codebreaker.build.m6502_asm (see m6502_asm.hpp).

#include "codebreaker/build/m6502_asm.hpp"

#include "codebreaker/decompile/m6502.hpp"

namespace codebreaker::build::m6502_asm {

const py::re::Pattern _AREA = py::re::compile(py::str("(\\w+)\\s*:\\s*([^;]*);"));

namespace {

// int(text, base) as Python reads it (base 0: a 0x / 0o / 0b prefix or decimal; a nonzero decimal
// with a leading zero is an error; one "_" allowed between digits, or after the prefix).
// (A local copy of the helper decompile/m6502.cpp keeps to itself.)
int64_t py_int(const py::str& text_in, int64_t base) {
    std::string t = text_in.strip().utf8();
    const int64_t shown_base = base;   // (the base as the caller gave it, in every message)
    auto bad = [&]() { return py::ValueError(py::percent("invalid literal for int() with base %d: %r", shown_base, text_in)); };
    size_t i = 0;
    bool neg = false;
    if (i < t.size() && (t[i] == '+' || t[i] == '-')) neg = t[i++] == '-';
    if (i + 1 < t.size() && t[i] == '0' && (base == 0 || base == 16 || base == 8 || base == 2)) {
        char c = static_cast<char>(std::tolower(static_cast<unsigned char>(t[i + 1])));
        int64_t b = c == 'x' ? 16 : c == 'o' ? 8 : c == 'b' ? 2 : 0;
        if (b != 0 && (base == 0 || base == b)) {
            base = b;
            i += 2;
            if (i < t.size() && t[i] == '_') i++;
        }
    }
    bool decimal_zero_rule = false;
    if (base == 0) {
        base = 10;
        decimal_zero_rule = true;  // (a decimal with a leading zero must be all zeros)
    }
    if (i >= t.size()) throw bad();
    size_t first = i;
    uint64_t v = 0;
    bool any_digit = false, last_us = false, nonzero = false;
    for (; i < t.size(); ++i) {
        char c = t[i];
        if (c == '_') {
            if (!any_digit || last_us) throw bad();
            last_us = true;
            continue;
        }
        int d = (c >= '0' && c <= '9') ? c - '0' : (std::isalpha(static_cast<unsigned char>(c)) ? std::tolower(static_cast<unsigned char>(c)) - 'a' + 10 : 99);
        if (d >= base) throw bad();
        last_us = false;
        any_digit = true;
        if (d != 0) nonzero = true;
        if (v > (static_cast<uint64_t>(INT64_MAX) + 1 - static_cast<uint64_t>(d)) / static_cast<uint64_t>(base))
            throw py::OverflowError("int too large for int64");
        v = v * static_cast<uint64_t>(base) + static_cast<uint64_t>(d);
    }
    if (!any_digit || last_us) throw bad();
    if (decimal_zero_rule && nonzero && t[first] == '0') throw bad();
    if (!neg && v > static_cast<uint64_t>(INT64_MAX)) throw py::OverflowError("int too large for int64");
    return neg ? static_cast<int64_t>(0 - v) : static_cast<int64_t>(v);
}

// d[key] for a str-keyed dict: a KeyError (whose text is the key's repr, as Python's) when absent
template <class V> const V& lookup(const py::dict<py::str, V>& d, const py::str& key) {
    if (!d.contains(key)) {
        throw py::KeyError(py::repr(key).utf8());
    }
    return d.at(key);
}

// line.split(None, 1): the first word and the rest (nullopt for the rest of a one-word line)
std::tuple<py::str, std::optional<py::str>> split_word(const py::str& line) {
    const std::u32string& u = line.u32();
    size_t i = 0;
    while (i < u.size() && py::str::space_(u[i])) ++i;
    size_t j = i;
    while (j < u.size() && !py::str::space_(u[j])) ++j;
    py::str word(u.substr(i, j - i));
    size_t k = j;
    while (k < u.size() && py::str::space_(u[k])) ++k;
    if (j >= u.size()) {
        return {word, std::nullopt};
    }
    return {word, py::str(u.substr(k))};
}

// line.split(None, 1)[1]: IndexError for a line of one word
py::str second_word(const py::str& line) {
    std::optional<py::str> rest = std::get<1>(split_word(line));
    if (!rest) {
        throw py::IndexError("list index out of range");
    }
    return *rest;
}

const py::re::Pattern& re_label() {
    static const py::re::Pattern p = py::re::compile(py::str("^([A-Za-z_]\\w*):\\s*(.*)$"));
    return p;
}
const py::re::Pattern& re_constant() {
    static const py::re::Pattern p = py::re::compile(py::str("^([A-Za-z_]\\w*)\\s*=\\s*(\\S+)$"));
    return p;
}
const py::re::Pattern& re_quoted_word() {
    static const py::re::Pattern p = py::re::compile(py::str("\"(\\w+)\""));
    return p;
}
const py::re::Pattern& re_quoted_name() {
    static const py::re::Pattern p = py::re::compile(py::str("\"([^\"]+)\""));
    return p;
}
const py::re::Pattern& re_identifier() {
    static const py::re::Pattern p = py::re::compile(py::str("^[A-Za-z_]\\w*$"));
    return p;
}
const py::re::Pattern& re_words() {
    static const py::re::Pattern p = py::re::compile(py::str("[A-Za-z_]\\w*"));
    return p;
}

// re.search(...).group(1): an AttributeError when nothing matched
py::str group1(const std::optional<py::re::Match>& m) {
    if (!m) {
        throw py::AttributeError("'NoneType' object has no attribute 'group'");
    }
    return m->group(1);
}

// s.encode("latin1"): a UnicodeEncodeError (a ValueError) worded as Python words it
py::bytes encode_latin1(const py::str& s) {
    const std::u32string& u = s.u32();
    std::string out;
    for (size_t i = 0; i < u.size(); ++i) {
        if (u[i] < 256) {
            out.push_back(static_cast<char>(u[i]));
            continue;
        }
        size_t j = i;
        while (j < u.size() && u[j] >= 256) ++j;
        if (j - i == 1) {
            char buf[16];
            std::snprintf(buf, sizeof buf, u[i] < 0x10000 ? "\\u%04x" : "\\U%08x", static_cast<unsigned>(u[i]));
            throw py::UnicodeEncodeError(py::percent("'latin-1' codec can't encode character '%s' in position %d: ordinal not in range(256)", py::str(buf), static_cast<int64_t>(i)));
        }
        throw py::UnicodeEncodeError(py::percent("'latin-1' codec can't encode characters in position %d-%d: ordinal not in range(256)", static_cast<int64_t>(i), static_cast<int64_t>(j - 1)));
    }
    return py::bytes(out);
}

// {key: value} from the "key=value, key=value" attributes of a MEMORY or SEGMENTS entry
py::dict<py::str, py::str> attributes(const py::str& body) {
    py::dict<py::str, py::str> attrs;
    for (const py::str& a : py::split(py::re::sub(py::str("\\s+"), py::str(""), body), py::str(","))) {
        if (a.contains(py::str("="))) {
            std::tuple<py::str, py::str, py::str> kv = a.partition(py::str("="));
            attrs.set(std::get<0>(kv), std::get<2>(kv));
        }
    }
    return attrs;
}

}  // namespace

int64_t _number(const py::str& s_in) {
    py::str s = s_in.strip();
    if (s.startswith(py::str("$"))) {
        return py_int(s.slice(1, py::len(s)), 16);
    }
    if (s.startswith(py::str("%"))) {
        return py_int(s.slice(1, py::len(s)), 2);
    }
    return py_int(s, 0);
}

std::tuple<py::list<Area>, py::dict<py::str, py::str>> config(const py::str& text_in) {
    py::str text = py::re::sub(py::str("#.*"), py::str(""), text_in);
    py::dict<py::str, py::str> blocks;
    for (const std::tuple<py::str, py::str>& b : py::re::Pattern(py::str("(MEMORY|SEGMENTS)\\s*\\{([^}]*)\\}"), 0).findall_groups<py::str, py::str>(text)) {
        blocks.set(std::get<0>(b), std::get<1>(b));
    }
    py::list<Area> areas;
    py::dict<py::str, py::str> segments;
    for (const std::tuple<py::str, py::str>& nb : _AREA.findall_groups<py::str, py::str>(blocks.get(py::str("MEMORY"), py::str("")))) {
        py::dict<py::str, py::str> attrs = attributes(std::get<1>(nb));
        areas.append(Area{std::get<0>(nb), _number(attrs.get(py::str("start"), py::str("0"))),
                          _number(attrs.get(py::str("size"), py::str("0"))),
                          attrs.get(py::str("fill"), py::str("no")) == py::str("yes")});
    }
    for (const std::tuple<py::str, py::str>& nb : _AREA.findall_groups<py::str, py::str>(blocks.get(py::str("SEGMENTS"), py::str("")))) {
        py::dict<py::str, py::str> attrs = attributes(std::get<1>(nb));
        segments.set(std::get<0>(nb), lookup(attrs, py::str("load")));
    }
    return {areas, segments};
}

py::list<py::str> _split_args(const py::str& s) {
    py::list<py::str> out;
    std::u32string cur;
    bool quoted = false;
    for (char32_t ch : s.u32()) {
        if (ch == U'"') {
            quoted = !quoted;
        }
        if (ch == U',' && !quoted) {
            out.append(py::str(cur).strip());
            cur.clear();
        } else {
            cur.push_back(ch);
        }
    }
    if (py::truthy(py::str(cur).strip())) {
        out.append(py::str(cur).strip());
    }
    return out;
}

py::str _strip_comment(const py::str& line) {
    bool quoted = false;
    const std::u32string& u = line.u32();
    for (size_t i = 0; i < u.size(); ++i) {
        if (u[i] == U'"') {
            quoted = !quoted;
        } else if (u[i] == U';' && !quoted) {
            return py::str(u.substr(0, i));
        }
    }
    return line;
}

py::bytes build(const py::str& source, const py::str& config_text, const py::str& folder) {
    py::list<Area> areas;
    py::dict<py::str, py::str> segments;
    std::tie(areas, segments) = config(config_text);
    py::dict<py::str, int64_t> starts;
    for (const Area& a : areas) {
        starts.set(std::get<0>(a), std::get<1>(a));
    }
    py::dict<py::str, int64_t> labels;
    py::dict<py::str, py::bytearray> out;
    for (int64_t pass_ : {1, 2}) {
        out = py::dict<py::str, py::bytearray>();
        for (const Area& a : areas) {
            out.set(std::get<0>(a), py::bytearray());
        }
        std::optional<py::str> area;
        int64_t n = 0;
        for (const py::str& raw : py::splitlines(source)) {
            ++n;
            py::str line = _strip_comment(raw).strip();
            if (!py::truthy(line)) {
                continue;
            }
            try {
                std::optional<py::re::Match> m = re_label().match(line);
                if (m) {
                    if (!area) {
                        throw py::ValueError("a label outside any segment");
                    }
                    labels.set(m->group(1), lookup(starts, *area) + py::len(lookup(out, *area)));
                    line = m->group(2).strip();
                    if (!py::truthy(line)) {
                        continue;
                    }
                }
                std::optional<py::re::Match> c = re_constant().match(line);
                if (c) {                                     // a constant: NAME = value
                    labels.set(c->group(1), _number(c->group(2)));
                    continue;
                }
                if (line.startswith(py::str(".segment"))) {
                    py::str seg = group1(re_quoted_word().search(line));
                    area = lookup(segments, seg);
                    continue;
                }
                if (!area) {
                    throw py::ValueError("code outside any segment");
                }
                py::bytearray here = lookup(out, *area);
                int64_t pc = lookup(starts, *area) + py::len(here);
                if (line.startswith(std::make_tuple(py::str(".byte"), py::str(".byt")))) {
                    for (const py::str& a : _split_args(second_word(line))) {
                        if (a.startswith(py::str("\""))) {
                            here.extend(encode_latin1(a.slice(1, py::len(a) - 1)));
                        } else {
                            here.append(_number(a) & 0xFF);
                        }
                    }
                } else if (line.startswith(std::make_tuple(py::str(".word"), py::str(".addr")))) {
                    for (const py::str& a : _split_args(second_word(line))) {
                        bool ident = static_cast<bool>(re_identifier().match(a));
                        int64_t v = ident ? labels.get(a, 0) : _number(a);
                        if (pass_ == 2 && ident && !labels.contains(a)) {
                            throw py::ValueError(py::percent("unknown label %s", a));
                        }
                        here.append(v & 0xFF);
                        here.append((v >> 8) & 0xFF);
                    }
                } else if (line.startswith(py::str(".incbin"))) {
                    py::file f = py::open_rb(py::os::path::join(folder, group1(re_quoted_name().search(line))));
                    py::bytes data = f.read();
                    f.close();
                    here.extend(data);
                } else if (line.startswith(py::str("."))) {
                    throw py::ValueError("directive not understood");
                } else {
                    if (pass_ == 1) {                        // (sizes only: every label is far, every branch near)
                        py::dict<py::str, int64_t> known = labels.copy();
                        std::optional<py::str> rest = std::get<1>(split_word(line));
                        for (const py::str& word : re_words().findall(line.contains(py::str(" ")) && rest ? *rest : py::str(""))) {
                            py::str low = word.lower();
                            if (low != py::str("a") && low != py::str("x") && low != py::str("y") && !known.contains(word)) {
                                known.set(word, 0xFFFF);
                            }
                        }
                        py::str mnemonic = std::get<0>(split_word(line)).upper();
                        if (decompile::m6502::BRANCHES.contains(mnemonic)) {
                            here.extend(py::bytes("\0\0", 2));
                            continue;
                        }
                        here.extend(decompile::m6502::assemble(line, pc, known));
                    } else {
                        here.extend(decompile::m6502::assemble(line, pc, labels));
                    }
                }
            } catch (const py::ValueError& e) {
                throw py::ValueError(py::percent("line %d: %s (%s)", n, raw.strip(), py::str(e.what())));
            } catch (const py::KeyError& e) {
                throw py::ValueError(py::percent("line %d: %s (%s)", n, raw.strip(), py::str(e.what())));
            } catch (const py::AttributeError& e) {
                throw py::ValueError(py::percent("line %d: %s (%s)", n, raw.strip(), py::str(e.what())));
            }
        }
    }
    py::bytearray image;
    for (const Area& a : areas) {
        const py::str& name = std::get<0>(a);
        int64_t size = std::get<2>(a);
        py::bytearray body = lookup(out, name);
        if (py::len(body) > size) {
            throw py::ValueError(py::percent("%s holds %d bytes, more than its %d", name, py::len(body), size));
        }
        image.extend(body.view());
        if (std::get<3>(a)) {
            image.extend(py::bytes::zeros(size - py::len(body)));
        }
    }
    return image.view();
}

}  // namespace codebreaker::build::m6502_asm
