// codebreaker/build/resources.cpp -- typed C++ of codebreaker.build.resources (see resources.hpp).

#include "codebreaker/build/resources.hpp"

#include <algorithm>
#include <functional>
#include <string>
#include <variant>
#include <vector>

#include "codebreaker/base/converted.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/watcom.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::build::resources {

namespace corpus = codebreaker::base::corpus;
namespace converted = codebreaker::base::converted;
namespace watcom = codebreaker::build::watcom;

namespace {

struct MemoryFlag {
    int64_t bit;
    const char* on;
    const char* off;
};
const MemoryFlag _MEMORY[] = {{0x40, "PRELOAD", "LOADONCALL"}, {0x10, "MOVEABLE", "FIXED"}, {0x1000, "DISCARDABLE", ""},
                              {0x20, "PURE", "IMPURE"}};
struct NamedBit {
    int64_t bit;
    const char* name;
};
const NamedBit _MENU_OPTIONS[] = {{0x01, "GRAYED"},      {0x02, "INACTIVE"},  {0x08, "CHECKED"},
                                  {0x20, "MENUBARBREAK"}, {0x40, "MENUBREAK"}, {0x4000, "HELP"}};
const NamedBit _KEY_OPTIONS[] = {{0x02, "NOINVERT"}, {0x04, "SHIFT"}, {0x08, "CONTROL"}, {0x10, "ALT"}};

const char* _DOC = R"DOC(resources.py -- a module's resources as source: `.rc` text and payload files, compiled back to `.res`.

    python -m codebreaker resources <module> [--out DIR]

    from codebreaker.build import resources
    r = resources.rebuild(module_path, outdir)      # write, compile, compare; r.res is the .res
    r.total, r.identical, r.equivalent, r.different, r.missing
    r.as_text, r.as_files, r.opaque, r.fallback     # how each resource was written

What is written, per kind of resource:

    text        string tables, dialogs, menus, accelerator tables and version blocks are decoded
                (formats/resources.py) and written as the `.rc` statements they came from
    files       an icon or cursor group with its member images becomes a real .ico / .cur file,
                a bitmap a .bmp, each named by an ICON / CURSOR / BITMAP statement
    opaque      kinds with no source form here (fonts, type libraries, raw data, anything
                user-defined) are carried as a payload file named by a raw-data statement

The `.rc` is compiled with OpenWatcom's resource compiler and the result is compared with the
original, resource by resource: IDENTICAL bytes, or EQUIVALENT (it decodes to the same thing --
the compiler pads, orders or numbers something differently). A resource that comes back
different, or whose statement the compiler rejects, is written again as a payload of its
original bytes ("fallback"), so the `.res` always holds the module's resources; how many needed
that is reported, because those are not yet source.

EQUIVALENT covers three things the compiler decides for itself: it numbers the member images of
an icon or cursor group, it pads, and it stamps the build time into a version block's file date
(the original's is zero).

Facts about the compiler, each found by a round trip that failed:

  * `-zm` makes it write the Microsoft `.res` layout (its default is a layout of its own). That
    file is for comparing; the LINKER must be given the default layout (`r.link_res`), because
    from a Microsoft-layout file it binds every resource with language 0.
  * In the Microsoft layout it does not pad bodies consistently, so entries do not always start
    on four-byte boundaries (`read_res` looks for the next entry rather than computing it).
  * A quoted type or name is taken literally: a resource called MENU is not the keyword.
  * A CONTROL statement's style has WS_CHILD and WS_VISIBLE unless they are taken out with NOT.
  * A string ends at a zero character, however it is written: text with one inside has no
    source form.
  * Text outside Latin-1 needs a UTF-8 source (`-zku8`), and there the compiler miscounts a
    string table's strings (UTF-8 bytes instead of characters): in a module that needs UTF-8,
    string tables with anything beyond ASCII are carried as payloads.
  * It writes a 16-bit version block with lengths that do not add up; 16-bit version blocks
    are carried as payloads.
  * It keeps only 16 bits of a control's or menu item's identifier, also in the extended
    templates that have room for 32 (where -1 is common): those are carried as payloads.
  * Without the preprocessor (`-zn`) a comment is a syntax error.
  * Like every OpenWatcom tool it has to be started by its full path.
)DOC";

// (the items of a list that is a temporary: a copy of them, which a range-for keeps alive)
template <class L>
std::vector<typename L::value_type> V(const L& l) {
    return l.vec();
}

bool is_int(const Ident& v, int64_t n) {
    const int64_t* p = std::get_if<int64_t>(&v);
    return p && *p == n;
}

bool is_empty_str(const Ident& v) {
    const py::str* p = std::get_if<py::str>(&v);
    return p && p->empty();
}

py::str D(int64_t v) { return py::str(std::to_string(v)); }

// subprocess.run(args, cwd=, env=, capture_output=True, text=True, errors="replace")
py::subprocess::CompletedProcess run_tool(const py::list<py::str>& args, const py::str& cwd,
                                          const py::dict<py::str, py::str>& env) {
    py::subprocess::RunOptions o;
    o.cwd = cwd;
    o.env = env;
    o.errors = "replace";
    return py::subprocess::run(args, o);
}

// bytes.decode("utf-16-le", "replace")
py::str utf16le_replace(const py::bytes& data) {
    const std::string& b = data.raw();
    std::u32string out;
    size_t n = b.size(), q = 0;
    auto unit = [&](size_t k) { return static_cast<char32_t>(static_cast<unsigned char>(b[k]) | (static_cast<unsigned char>(b[k + 1]) << 8)); };
    while (q < n) {
        if (n - q < 2) {
            out.push_back(0xFFFD);
            break;
        }
        char32_t ch = unit(q);
        q += 2;
        if (ch < 0xD800 || ch > 0xDFFF) {
            out.push_back(ch);
            continue;
        }
        if (ch < 0xDC00) {
            if (n - q < 2) {
                out.push_back(0xFFFD);
                q = n;
                continue;
            }
            char32_t ch2 = unit(q);
            if (ch2 >= 0xDC00 && ch2 <= 0xDFFF) {
                out.push_back(0x10000 + ((ch - 0xD800) << 10) + (ch2 - 0xDC00));
                q += 2;
                continue;
            }
            out.push_back(0xFFFD);
            continue;
        }
        out.push_back(0xFFFD);
    }
    return py::str(out);
}

// ---- a decoded resource is equal to another when the values the Python decoders gave would compare equal
bool same_menu_items(const py::list<R::MenuItem>& a, const py::list<R::MenuItem>& b);
bool same_menu_items(const py::list<R::ExtMenuItem>& a, const py::list<R::ExtMenuItem>& b);

bool same_item(const R::MenuItem& a, const R::MenuItem& b) {
    if (!(a.text == b.text && a.id == b.id && a.flags == b.flags)) return false;
    if (a.items.has_value() != b.items.has_value()) return false;
    return !a.items || same_menu_items(*a.items, *b.items);
}
bool same_item(const R::ExtMenuItem& a, const R::ExtMenuItem& b) {
    if (!(a.text == b.text && a.id == b.id && a.type == b.type && a.state == b.state && a.help == b.help)) return false;
    if (a.items.has_value() != b.items.has_value()) return false;
    return !a.items || same_menu_items(*a.items, *b.items);
}
template <class Item>
bool same_items_(const py::list<Item>& a, const py::list<Item>& b) {
    if (a.size() != b.size()) return false;
    for (int64_t i = 0; i < a.size(); ++i)
        if (!same_item(a[i], b[i])) return false;
    return true;
}
bool same_menu_items(const py::list<R::MenuItem>& a, const py::list<R::MenuItem>& b) { return same_items_(a, b); }
bool same_menu_items(const py::list<R::ExtMenuItem>& a, const py::list<R::ExtMenuItem>& b) { return same_items_(a, b); }

bool same_decoded(const R::Decoded& a, const R::Decoded& b) {
    if (a.index() != b.index()) return false;
    if (const auto* x = std::get_if<py::list<py::str>>(&a)) return *x == std::get<py::list<py::str>>(b);
    if (const auto* x = std::get_if<R::Dialog>(&a)) {
        const R::Dialog& y = std::get<R::Dialog>(b);
        if (!(x->extended == y.extended && x->style == y.style && x->exstyle == y.exstyle && x->help == y.help && x->x == y.x &&
              x->y == y.y && x->cx == y.cx && x->cy == y.cy && x->menu == y.menu && x->class_ == y.class_ && x->title == y.title &&
              x->font == y.font && x->controls.size() == y.controls.size()))
            return false;
        for (int64_t i = 0; i < x->controls.size(); ++i) {
            const R::DialogControl& c = x->controls[i];
            const R::DialogControl& d = y.controls[i];
            if (!(c.help == d.help && c.exstyle == d.exstyle && c.x == d.x && c.y == d.y && c.cx == d.cx && c.cy == d.cy &&
                  c.style == d.style && c.id == d.id && c.class_ == d.class_ && c.text == d.text && c.extra == d.extra))
                return false;
        }
        return true;
    }
    if (const auto* x = std::get_if<R::Menu>(&a)) {
        const R::Menu& y = std::get<R::Menu>(b);
        if (!(x->extended == y.extended && x->help == y.help && x->items.index() == y.items.index())) return false;
        if (const auto* l = std::get_if<py::list<R::MenuItem>>(&x->items)) return same_menu_items(*l, std::get<py::list<R::MenuItem>>(y.items));
        return same_menu_items(std::get<py::list<R::ExtMenuItem>>(x->items), std::get<py::list<R::ExtMenuItem>>(y.items));
    }
    if (const auto* x = std::get_if<py::list<std::tuple<int64_t, int64_t, int64_t>>>(&a))
        return *x == std::get<py::list<std::tuple<int64_t, int64_t, int64_t>>>(b);
    if (const auto* x = std::get_if<R::Version>(&a)) {
        const R::Version& y = std::get<R::Version>(b);
        if (x->fixed.has_value() != y.fixed.has_value()) return false;
        if (x->fixed && !(*x->fixed == *y.fixed)) return false;
        return x->strings == y.strings && x->translations == y.translations;
    }
    return false;
}

}  // namespace

bool Result::same() const {
    return !py::truthy(error) && different.empty() && missing.empty() && extra.empty() && identical + equivalent == total;
}

py::str Result::line() const {
    if (py::truthy(error)) return py::str("FAILED: ") + error;
    py::str extras = extra.empty() ? py::str("") : py::percent(py::str(", %d that the original does not have"), extra.size());
    return py::percent(py::str("%d resources: %d identical, %d equivalent, %d different, %d missing%s  "
                               "(written as text %d, as image files %d, opaque payload %d, fallback payload %d)"),
                       total, identical, equivalent, different.size(), missing.size(), extras, as_text, as_files, opaque,
                       fallback);
}

// ---------------------------------------------------------------------------------------------
// the pieces of a statement
// ---------------------------------------------------------------------------------------------
py::str quoted(const py::str& text, int64_t wide, bool counted) {
    std::u32string out;
    auto octal = [&](char32_t c) {
        out.push_back(U'\\');
        out.push_back(U'0' + ((c >> 6) & 7));
        out.push_back(U'0' + ((c >> 3) & 7));
        out.push_back(U'0' + (c & 7));
    };
    for (char32_t c : text.u32()) {
        if (c == U'"') {
            out += U"\"\"";
        } else if (c == U'\\') {
            out += U"\\\\";
        } else if (c == U'\n') {
            out += U"\\n";
        } else if (c == U'\t') {
            out += U"\\t";
        } else if (c == U'\r') {
            out += U"\\r";
        } else if (c == 0) {
            throw NoSourceForm("a zero character inside a string");
        } else if (c < 0x20 || c == 0x7F) {
            octal(c);
        } else if (c <= 0x7F) {
            out.push_back(c);
        } else if (wide != UNICODE) {
            if (c > 0xFF) throw BeyondLatin1();
            octal(c);
        } else if (counted || (0xD800 <= c && c <= 0xDFFF)) {
            throw NoSourceForm("text the compiler cannot take");
        } else {
            out.push_back(c);
        }
    }
    return py::str(U"\"" + out + U"\"");
}

py::str ident(const Ident& value, int64_t wide) {
    if (const int64_t* n = std::get_if<int64_t>(&value)) return D(*n);
    return quoted(std::get<py::str>(value), wide);
}

py::str _memory(int64_t flags, int64_t wide) {
    if (wide) return py::str("");
    std::string out;
    for (const MemoryFlag& m : _MEMORY) {
        const char* w = (flags & m.bit) ? m.on : m.off;
        if (*w) out += std::string(" ") + w;
    }
    return py::str(out);
}

py::str _number(int64_t value) {
    if (value > 0xFFFF) throw NoSourceForm("an identifier wider than 16 bits");
    return D(value);
}

py::str _style(int64_t value) {
    py::str s = py::percent(py::str("0x%08XL"), value);
    if (!(value & 0x10000000)) s += " | NOT 0x10000000L";
    if (!(value & 0x40000000)) s += " | NOT 0x40000000L";
    return s;
}

py::str _stem(const R::Resource& resource) {
    auto part = [](const Ident& v) -> py::str {
        if (const int64_t* n = std::get_if<int64_t>(&v)) return D(*n);
        return py::re::sub(py::str("[^A-Za-z0-9]"), py::str("_"), std::get<py::str>(v));
    };
    py::str s = py::percent(py::str("%s_%s"), R::type_name(resource.type).replace(py::str("#"), py::str("type")), part(resource.name));
    py::str out = py::re::sub(py::str("[^A-Za-z0-9_]"), py::str("_"), s).lower();
    if (resource.language && *resource.language) out += py::percent(py::str("_%04x"), *resource.language);
    return out;
}

// ---------------------------------------------------------------------------------------------
// statements for the kinds that have a source form
// ---------------------------------------------------------------------------------------------
py::list<py::str> dialog_text(const R::Resource& r, int64_t wide) {
    R::Dialog d = R::dialog(r.data, wide != 0);
    py::str name = ident(r.name, wide);
    py::str mem = _memory(r.flags, wide);
    py::str head = py::percent(py::str("%s %s%s %d, %d, %d, %d"), name, py::str(d.extended ? "DIALOGEX" : "DIALOG"), mem, d.x, d.y,
                               d.cx, d.cy);
    if (d.help) head += py::percent(py::str(", %d"), d.help);
    py::list<py::str> out{head, py::percent(py::str("STYLE 0x%08XL"), d.style)};
    if (d.exstyle) out.append(py::percent(py::str("EXSTYLE 0x%08XL"), d.exstyle));
    if (!d.title.empty()) out.append(py::str("CAPTION ") + quoted(d.title, wide));
    if (!is_empty_str(d.menu)) out.append(py::str("MENU ") + ident(d.menu, wide));
    if (!is_empty_str(d.class_)) out.append(py::str("CLASS ") + ident(d.class_, wide));
    if (d.font) {
        if (const auto* f = std::get_if<R::DialogFontPlain>(&*d.font)) {
            out.append(py::percent(py::str("FONT %d, %s"), std::get<0>(*f), quoted(std::get<1>(*f), wide)));
        } else {
            const auto& e = std::get<R::DialogFontExtended>(*d.font);
            py::str line = py::percent(py::str("FONT %d, %s"), std::get<0>(e), quoted(std::get<4>(e), wide));
            line += py::percent(py::str(", %d, %d, %d"), std::get<1>(e), std::get<2>(e), std::get<3>(e));
            out.append(line);
        }
    }
    out.append(py::str("BEGIN"));
    for (const R::DialogControl& c : d.controls.vec()) {
        if (!c.extra.empty()) throw NoSourceForm("a control with creation data");
        py::str cls;
        if (const int64_t* atom = std::get_if<int64_t>(&c.class_)) {
            if (!R::CONTROL_CLASSES().contains(*atom)) throw NoSourceForm("control class atom 0x" + py::percent(py::str("%x"), *atom).utf8());
            cls = R::CONTROL_CLASSES().at(*atom);
        } else {
            cls = std::get<py::str>(c.class_);
            py::str up = cls.upper();
            for (const py::str& v : R::CONTROL_CLASSES().values())
                if (v == up) throw NoSourceForm("a standard control class spelled out");   // the compiler would make it an atom
        }
        py::str text = ident(c.text, wide);
        py::str id = _number(c.id);
        py::str klass = quoted(cls, wide);
        py::str style = _style(c.style);
        py::str line = py::percent(py::str("  CONTROL %s, %s, %s, %s, %d, %d, %d, %d"), text, id, klass, style, c.x, c.y, c.cx, c.cy);
        if (c.exstyle || c.help) line += py::percent(py::str(", 0x%08XL"), c.exstyle);
        if (c.help) line += py::percent(py::str(", %d"), c.help);
        out.append(line);
    }
    out.append(py::str("END"));
    return out;
}

namespace {

// one level of a menu: the plain items and the extended items differ in what they carry
template <class Item>
void menu_level(const py::list<Item>& items, int64_t depth, bool extended, int64_t wide, py::list<py::str>& out) {
    std::u32string padding;
    for (int64_t i = 0; i < depth; ++i) padding += U"  ";
    py::str pad{padding};
    out.append(pad + "BEGIN");
    for (const Item& it : items.vec()) {
        py::str text = quoted(it.text, wide);
        if constexpr (std::is_same_v<Item, R::ExtMenuItem>) {
            (void)extended;
            py::str id = _number(it.id);
            py::str tail = py::str(", ") + id + py::percent(py::str(", 0x%XL, 0x%XL"), it.type, it.state);
            if (it.items) {
                out.append(py::percent(py::str("%s  POPUP %s%s, %d"), pad, text, tail, it.help));
                menu_level(*it.items, depth + 1, extended, wide, out);
            } else {
                out.append(py::percent(py::str("%s  MENUITEM %s%s"), pad, text, tail));
            }
        } else {
            int64_t flags = it.flags & ~int64_t(0x10);
            int64_t all = 0;
            for (const NamedBit& o : _MENU_OPTIONS) all += o.bit;
            if (flags & ~all) throw NoSourceForm("menu item flags 0x" + py::percent(py::str("%x"), flags).utf8());
            std::string options;
            for (const NamedBit& o : _MENU_OPTIONS)
                if (flags & o.bit) options += std::string(", ") + o.name;
            if (it.items) {
                out.append(py::percent(py::str("%s  POPUP %s%s"), pad, text, py::str(options)));
                menu_level(*it.items, depth + 1, extended, wide, out);
            } else if (it.text.empty() && !it.id && !flags) {
                out.append(pad + "  MENUITEM SEPARATOR");
            } else if (it.text.empty()) {
                throw NoSourceForm("an item with no text that is not a separator");
            } else {
                out.append(py::percent(py::str("%s  MENUITEM %s, %d%s"), pad, text, it.id, py::str(options)));
            }
        }
    }
    out.append(pad + "END");
}

}  // namespace

py::list<py::str> menu_text(const R::Resource& r, int64_t wide) {
    R::Menu m = R::menu(r.data, wide != 0);
    if (m.help) throw NoSourceForm("a menu with a help id");
    py::str name = ident(r.name, wide);
    py::list<py::str> out{py::percent(py::str("%s %s%s"), name, py::str(m.extended ? "MENUEX" : "MENU"), _memory(r.flags, wide))};
    if (m.extended)
        menu_level(std::get<py::list<R::ExtMenuItem>>(m.items), 0, true, wide, out);
    else
        menu_level(std::get<py::list<R::MenuItem>>(m.items), 0, false, wide, out);
    return out;
}

py::list<py::str> accelerators_text(const R::Resource& r, int64_t wide) {
    py::str name = ident(r.name, wide);
    py::list<py::str> out{py::percent(py::str("%s ACCELERATORS%s"), name, _memory(r.flags, wide)), py::str("BEGIN")};
    for (const auto& a : V(R::accelerators(r.data, wide != 0))) {
        int64_t flags = std::get<0>(a), key = std::get<1>(a), id = std::get<2>(a);
        if (flags & ~int64_t(0x1F)) throw NoSourceForm("accelerator flags 0x" + py::percent(py::str("%x"), flags).utf8());
        std::string opts;
        for (const NamedBit& o : _KEY_OPTIONS)
            if (flags & o.bit) opts += std::string(", ") + o.name;
        out.append(py::percent(py::str("  %d, %d, %s%s"), key, id, py::str((flags & 1) ? "VIRTKEY" : "ASCII"), py::str(opts)));
    }
    out.append(py::str("END"));
    return out;
}

py::list<py::str> version_text(const R::Resource& r, int64_t wide) {
    if (!wide) {
        // the compiler leaves the last string of a 16-bit block without its terminator and
        // padding while counting them in every enclosing length: what it writes cannot be read
        throw NoSourceForm("the compiler writes an unsound 16-bit version block");
    }
    R::Version v = R::version(r.data, wide != 0);
    if (!v.fixed || v.fixed->empty() || (*v.fixed)[0] != 0xFEEF04BDLL || (*v.fixed)[11] || (*v.fixed)[12])
        throw NoSourceForm("a version block without the usual fixed part");
    const py::list<int64_t>& fixed = *v.fixed;
    py::list<py::str> out{py::percent(py::str("%s VERSIONINFO"), ident(r.name, wide))};
    const char* words1[] = {"FILEVERSION", "PRODUCTVERSION"};
    for (int k = 0; k < 2; ++k) {
        int64_t ms = fixed[2 + 2 * k], ls = fixed[3 + 2 * k];
        out.append(py::percent(py::str("%s %d, %d, %d, %d"), py::str(words1[k]), ms >> 16, ms & 0xFFFF, ls >> 16, ls & 0xFFFF));
    }
    const char* words2[] = {"FILEFLAGSMASK", "FILEFLAGS", "FILEOS", "FILETYPE", "FILESUBTYPE"};
    for (int k = 0; k < 5; ++k) out.append(py::percent(py::str("%s 0x%XL"), py::str(words2[k]), fixed[6 + k]));
    out.append(py::str("BEGIN"));
    if (!v.strings.empty()) {
        out.extend(py::list<py::str>{py::str("  BLOCK \"StringFileInfo\""), py::str("  BEGIN")});
        for (const auto& kv : v.strings.items()) {
            out.extend(py::list<py::str>{py::str("    BLOCK ") + quoted(std::get<0>(kv), wide), py::str("    BEGIN")});
            for (const auto& e : std::get<1>(kv).vec()) {
                py::str k = quoted(std::get<0>(e), wide);
                py::str val = quoted(std::get<1>(e), wide);
                out.append(py::percent(py::str("      VALUE %s, %s"), k, val));
            }
            out.append(py::str("    END"));
        }
        out.append(py::str("  END"));
    }
    if (!v.translations.empty()) {
        std::string ws;
        bool first = true;
        for (int64_t w : v.translations.vec()) {
            ws += (first ? "" : ", ") + py::percent(py::str("0x%X"), w).utf8();
            first = false;
        }
        out.extend(py::list<py::str>{py::str("  BLOCK \"VarFileInfo\""), py::str("  BEGIN"),
                                     py::str("    VALUE \"Translation\", ") + py::str(ws), py::str("  END")});
    }
    out.append(py::str("END"));
    return out;
}

namespace {

using TextFn = py::list<py::str> (*)(const R::Resource&, int64_t);

// _TEXT: the kinds written as statements
TextFn text_function(const Ident& type) {
    const int64_t* t = std::get_if<int64_t>(&type);
    if (!t) return nullptr;
    if (*t == R::DIALOG) return dialog_text;
    if (*t == R::MENU) return menu_text;
    if (*t == R::ACCELERATOR) return accelerators_text;
    if (*t == R::VERSION) return version_text;
    return nullptr;
}

// the errors a resource's decoding or its statement can raise that mean "no source form": (NoSourceForm, ValueError,
// IndexError, struct.error, UnicodeError) -- run `f`, and say whether it raised one
template <class F>
bool no_source(F&& f) {
    try {
        f();
        return false;
    } catch (const NoSourceForm&) {
    } catch (const py::ValueError&) {
    } catch (const py::IndexError&) {
    } catch (const py::StructError&) {
    }
    return true;
}

Key fold(const Key& key) {
    auto up = [](const Ident& v) -> Ident {
        if (const py::str* s = std::get_if<py::str>(&v)) return s->upper();
        return v;
    };
    return Key(up(std::get<0>(key)), up(std::get<1>(key)), std::get<2>(key));
}

}  // namespace

// ---------------------------------------------------------------------------------------------
// writing the .rc
// ---------------------------------------------------------------------------------------------
py::dict<Key, py::list<int64_t>> _members(const py::list<R::Resource>& found, int64_t) {
    py::dict<std::tuple<int64_t, Ident>, py::list<int64_t>> by_id;
    for (int64_t i = 0; i < found.size(); ++i) {
        const R::Resource& r = found[i];
        const int64_t* t = std::get_if<int64_t>(&r.type);
        if (t && (*t == R::ICON || *t == R::CURSOR)) by_id.setdefault(std::make_tuple(*t, r.name), py::list<int64_t>()).append(i);
    }
    py::dict<Key, py::list<int64_t>> taken;
    for (int64_t gi = 0; gi < found.size(); ++gi) {
        const R::Resource& g = found[gi];
        const int64_t* gt = std::get_if<int64_t>(&g.type);
        if (!gt || !R::MEMBER_OF().contains(*gt)) continue;
        py::list<R::GroupEntry> entries;
        try {
            entries = std::get<1>(R::group(g.data));
        } catch (const py::ValueError&) {
            continue;
        } catch (const py::StructError&) {
            continue;
        }
        py::list<int64_t> mine;
        bool complete = true;
        for (const R::GroupEntry& e : entries.vec()) {
            py::list<int64_t> cands = by_id.get(std::make_tuple(R::MEMBER_OF().at(*gt), Ident(std::get<6>(e))), py::list<int64_t>());
            std::optional<int64_t> pick;
            for (int64_t m : cands.vec())
                if (found[m].language == g.language) {
                    pick = m;
                    break;
                }
            if (!pick && !cands.empty()) pick = cands[0];
            if (!pick) {
                complete = false;
                break;
            }
            mine.append(*pick);
        }
        if (complete) taken.set(g.key(), mine);
    }
    return taken;
}

std::tuple<py::str, py::list<Span>, How, int64_t> write_rc(const py::str& module_path, const py::str& outdir,
                                                           const py::set<Key>& verbatim) {
    auto lane = corpus::lane(module_path);
    int64_t wide = (lane && *lane == "pe") ? WIDE : 0;
    try {
        return _write_rc(module_path, outdir, py::set<Key>::from(verbatim), wide);
    } catch (const BeyondLatin1&) {
        return _write_rc(module_path, outdir, py::set<Key>::from(verbatim), UNICODE);
    }
}

std::tuple<py::str, py::list<Span>, How, int64_t> _write_rc(const py::str& module_path, const py::str& outdir, py::set<Key> verbatim,
                                                            int64_t wide) {
    py::list<R::Resource> found = R::read(module_path);
    py::str stem = std::get<0>(py::os::path::splitext(py::os::path::basename(module_path))).lower();
    py::str files = py::os::path::join(outdir, py::str("res"));
    py::os::makedirs(files, 0777, true);
    py::dict<Key, py::list<int64_t>> groups = _members(found, wide);
    {
        bool any = false;
        for (const Key& k : groups)
            if (verbatim.contains(k)) {
                any = true;
                break;
            }
        if (any)                                             // member numbering is the compiler's: all or none
            for (const Key& k : groups) verbatim.add(k);
    }
    {
        py::dict<Key, py::list<int64_t>> kept;
        for (const auto& kv : groups.items())
            if (!verbatim.contains(std::get<0>(kv))) kept.set(std::get<0>(kv), std::get<1>(kv));
        groups = kept;
    }
    py::set<int64_t> in_group;
    for (const py::list<int64_t>& members : groups.values())
        for (int64_t m : members.vec()) in_group.add(m);
    py::list<py::str> lines{py::percent(py::str("/* %s -- the resources of %s. Generated; see codebreaker/build/resources.py. */"),
                                        stem + ".rc", py::os::path::basename(module_path).lower())};
    py::list<Span> spans;
    How how;
    for (const char* k : {"text", "files", "opaque", "fallback"}) how.set(py::str(k), py::list<Key>());
    std::optional<int64_t> language;
    bool language_set = false;                               // (language[0] starts as None)

    auto emit = [&](const py::list<Key>& keys, const py::list<py::str>& text, const char* kind, const std::optional<int64_t>& lang) {
        if (wide && (!language_set || lang != language)) {
            lines.append(py::percent(py::str("LANGUAGE 0x%02X, 0x%02X"), *lang & 0x3FF, *lang >> 10));
            language = lang;
            language_set = true;
        }
        int64_t first = lines.size() + 1;
        lines.extend(text);
        for (const Key& k : keys.vec()) {
            spans.append(Span(first, lines.size(), k));
            how.at(py::str(kind)).append(k);
        }
    };

    auto payload = [&](const R::Resource& r, const char* ext, const py::bytes& data) -> py::str {
        py::str name = _stem(r) + ext;
        int64_t n = 1;
        while (py::os::path::exists(py::os::path::join(files, name)) && py::open_rb(py::os::path::join(files, name)).read() != data) {
            n += 1;
            name = py::percent(py::str("%s_%d%s"), _stem(r), n, py::str(ext));
        }
        {
            py::file f = py::open(py::os::path::join(files, name), py::str("wb"));
            f.write(data);
        }
        return py::percent(py::str("\"res/%s\""), name);
    };

    auto raw = [&](const R::Resource& r, const char* kind) {
        std::optional<R::TypeLibrary> library = R::type_library(r.data);
        py::str name = payload(r, library ? ".tlb" : ".bin", r.data);
        if (library) {                                       // beside the payload, an outline to read
            py::file f = py::open(py::os::path::join(outdir, name.strip(py::str("\"")).slice(0, name.strip(py::str("\"")).size() - 4) + ".idl"),
                                  py::str("w"), std::nullopt, std::nullopt, py::str("\n"));
            f.write(R::type_library_outline(*library));
        }
        py::str n = ident(r.name, wide);
        py::str t = ident(r.type, wide);
        emit(py::list<Key>{r.key()}, py::list<py::str>{py::percent(py::str("%s %s%s %s"), n, t, _memory(r.flags, wide), name)}, kind,
             r.language);
    };

    // (language, flags) -> [(key, [(id, text)])]
    using Block = std::tuple<Key, py::list<std::tuple<int64_t, py::str>>>;
    py::dict<std::tuple<std::optional<int64_t>, int64_t>, py::list<Block>> tables;
    for (int64_t ri = 0; ri < found.size(); ++ri) {
        const R::Resource& r = found[ri];
        if (in_group.contains(ri)) continue;
        const int64_t* type = std::get_if<int64_t>(&r.type);
        const int64_t* rname = std::get_if<int64_t>(&r.name);
        if (verbatim.contains(r.key())) {
            raw(r, "fallback");
        } else if (type && *type == R::STRING && rname) {
            py::list<std::tuple<int64_t, py::str>> texts;
            bool failed = false;
            try {
                py::list<py::str> strings = R::strings(r.data, wide != 0);
                for (int64_t i = 0; i < strings.size(); ++i)
                    if (!strings[i].empty()) texts.append(std::make_tuple(16 * (*rname - 1) + i, quoted(strings[i], wide, true)));
            } catch (const NoSourceForm&) {
                failed = true;
            } catch (const py::ValueError&) {
                failed = true;
            } catch (const py::IndexError&) {
                failed = true;
            } catch (const py::StructError&) {
                failed = true;
            }
            if (failed) {
                raw(r, "fallback");
                continue;
            }
            tables.setdefault(std::make_tuple(r.language, r.flags), py::list<Block>()).append(Block(r.key(), texts));
        } else if (TextFn fn = text_function(r.type)) {
            py::list<py::str> text;
            if (no_source([&] { text = fn(r, wide); }))
                raw(r, "fallback");
            else
                emit(py::list<Key>{r.key()}, text, "text", r.language);
        } else if (groups.contains(r.key())) {
            py::list<int64_t> member_ids = groups.at(r.key());
            py::dict<Ident, py::bytes> members;
            for (int64_t m : member_ids.vec()) members.set(found[m].name, found[m].data);
            bool is_icon = type && *type == R::GROUP_ICON;
            const char* word = is_icon ? "ICON" : "CURSOR";
            const char* ext = is_icon ? ".ico" : ".cur";
            py::bytes image;
            if (no_source([&] {
                    image = R::icon_file(r.data, [&](int64_t id) -> py::bytes {
                        if (!members.contains(Ident(id))) throw py::KeyError(std::to_string(id));
                        return members.at(Ident(id));
                    });
                })) {
                raw(r, "fallback");
                for (int64_t m : member_ids.vec()) raw(found[m], "fallback");
                continue;
            }
            py::list<Key> keys{r.key()};
            for (int64_t m : member_ids.vec()) keys.append(found[m].key());
            py::str nm = ident(r.name, wide);
            py::str mem = _memory(r.flags, wide);
            py::str pl = payload(r, ext, image);
            emit(keys, py::list<py::str>{py::percent(py::str("%s %s%s %s"), nm, py::str(word), mem, pl)}, "files", r.language);
        } else if (type && *type == R::BITMAP) {
            py::str nm = ident(r.name, wide);
            py::str mem = _memory(r.flags, wide);
            py::str pl = payload(r, ".bmp", R::bitmap_file(r.data));
            emit(py::list<Key>{r.key()}, py::list<py::str>{py::percent(py::str("%s BITMAP%s %s"), nm, mem, pl)}, "files", r.language);
        } else {
            raw(r, (type && (*type == R::STRING || *type == R::GROUP_ICON || *type == R::GROUP_CURSOR)) ? "fallback" : "opaque");
        }
    }
    for (const auto& kv : tables.items()) {
        const std::optional<int64_t>& lang = std::get<0>(std::get<0>(kv));
        int64_t flags = std::get<1>(std::get<0>(kv));
        const py::list<Block>& blocks = std::get<1>(kv);
        py::list<py::str> text{py::str("STRINGTABLE") + _memory(flags, wide), py::str("BEGIN")};
        py::list<Key> keys;
        for (const Block& b : blocks.vec()) {
            for (const auto& pair : std::get<1>(b).vec()) text.append(py::percent(py::str("  %d, %s"), std::get<0>(pair), std::get<1>(pair)));
            keys.append(std::get<0>(b));
        }
        text.append(py::str("END"));
        emit(keys, text, "text", lang);
    }
    py::str rc = py::os::path::join(outdir, stem + ".rc");
    {
        py::file f = py::open(rc, py::str("w"), py::str(wide == UNICODE ? "utf-8" : "ascii"), std::nullopt, py::str("\n"));
        f.write(py::join(py::str("\n"), lines) + "\n");
    }
    return std::make_tuple(rc, spans, how, wide);
}

// ---------------------------------------------------------------------------------------------
// compiling, and reading what the compiler wrote
// ---------------------------------------------------------------------------------------------
std::tuple<std::optional<py::str>, py::str> compile_rc(const py::str& rc, int64_t wide, bool microsoft) {
    // (the resource compiler and the environment of the class it is for: platforms.of("pe" or "ne"))
    py::str wrc = watcom::ow_tool(py::str("wrc"));
    if (wrc.empty()) throw py::RuntimeError("OpenWatcom's resource compiler is not set up (settings.json: openwatcom)");
    py::str res = std::get<0>(py::os::path::splitext(rc)) + (microsoft ? ".res" : "_link.res");
    try {
        py::os::remove(res);
    } catch (const py::OSError&) {
    }
    // -r: only the .res; -zm: the Microsoft layout; -zku8: the source is UTF-8 and becomes UTF-16.
    // (The preprocessor is left on: without it a comment is a syntax error.)
    py::list<py::str> args{wrc, py::str("-q"), py::str("-r"), py::str(wide ? "-bt=nt" : "-bt=windows")};
    if (microsoft) args.append(py::str("-zm"));
    if (wide == UNICODE) args.append(py::str("-zku8"));
    args.append(py::str("-fo=") + py::os::path::basename(res));
    args.append(py::os::path::basename(rc));
    py::subprocess::CompletedProcess p =
        run_tool(args, py::os::path::dirname(rc), watcom::ow_env(py::str(wide ? "pe" : "ne")));
    py::str log = (p.stdout_ ? *p.stdout_ : py::str("")) + (p.stderr_ ? *p.stderr_ : py::str(""));
    if (p.returncode == 0 && py::os::path::isfile(res)) return std::make_tuple(std::optional<py::str>(res), log);
    return std::make_tuple(std::optional<py::str>(), log);
}

py::list<R::Resource> read_res(const py::str& path, int64_t wide) {
    py::bytes d;
    {
        py::file f = py::open_rb(path);
        d = f.read();
    }
    const int64_t len = d.size();
    py::list<R::Resource> out;
    int64_t p = 0;

    auto u16 = [&](int64_t at) { return std::get<0>(py::unpack_from<uint16_t>("<H", d, at)); };

    auto name16 = [&](int64_t at) -> std::tuple<Ident, int64_t> {
        if (d[at] == 0xFF) return std::make_tuple(Ident(int64_t(u16(at + 1))), at + 3);
        int64_t e = d.index(py::bytes(std::string(1, '\0')), at);
        return std::make_tuple(Ident(d.slice(at, e).decode("latin-1")), e + 1);
    };

    auto name32 = [&](int64_t at) -> std::tuple<Ident, int64_t> {
        if (u16(at) == 0xFFFF) return std::make_tuple(Ident(int64_t(u16(at + 2))), at + 4);
        int64_t e = at;
        while (!(e + 2 <= len && d[e] == 0 && d[e + 1] == 0)) {
            if (e > len) throw py::IndexError(std::to_string(e));
            e += 2;
        }
        return std::make_tuple(Ident(utf16le_replace(d.slice(at, e))), e + 2);
    };

    auto entry_at = [&](int64_t at) -> bool {
        if (at + 32 > len) return false;
        auto [size, header] = py::unpack_from<uint32_t, uint32_t>("<II", d, at);
        if (!(24 <= int64_t(header) && int64_t(header) <= 4096) || at + int64_t(header) + int64_t(size) > len) return false;
        int64_t q;
        try {                                                // the header is as long as its names make it
            q = std::get<1>(name32(at + 8));
            q = std::get<1>(name32(q));
        } catch (const py::StructError&) {
            return false;
        } catch (const py::IndexError&) {
            return false;
        }
        return (((q - at + 3) & ~int64_t(3)) + 16) == int64_t(header);
    };

    while (p + (wide ? 16 : 8) <= len) {
        Ident rtype, name;
        std::optional<int64_t> lang;
        int64_t flags;
        py::bytes data;
        if (wide) {
            // The Microsoft compiler pads every body to four bytes, so every entry starts on a
            // four-byte boundary. This compiler leaves some bodies unpadded: an entry can start
            // anywhere, the fixed part of its header is then moved up to the next boundary of the
            // file, and the next entry is looked for in the bytes after the body.
            std::optional<int64_t> found;
            for (int64_t k = 0; k < 16; ++k)
                if (entry_at(p + k)) {
                    found = p + k;
                    break;
                }
            if (!found) break;
            p = *found;
            auto [size, header] = py::unpack_from<uint32_t, uint32_t>("<II", d, p);
            (void)header;
            int64_t q;
            std::tie(rtype, q) = name32(p + 8);
            std::tie(name, q) = name32(q);
            q = (q + 3) & ~int64_t(3);                       // padding the header's length does not count
            auto [dv, fl, lg] = py::unpack_from<uint32_t, uint16_t, uint16_t>("<IHH", d, q);
            (void)dv;
            flags = fl;
            lang = int64_t(lg);
            int64_t body = q + 16;
            data = d.slice(body, body + int64_t(size));
            p = body + int64_t(size);
            if (is_int(rtype, 0) && is_int(name, 0) && !size) continue;   // the marker a 32-bit .res starts with
        } else {
            while (p < len && d[p] == 0) p += 1;             // padding between entries
            if (p + 8 > len) break;
            int64_t q;
            std::tie(rtype, q) = name16(p);
            std::tie(name, q) = name16(q);
            auto [fl, size] = py::unpack_from<uint16_t, uint32_t>("<HI", d, q);
            flags = fl;
            data = d.slice(q + 6, q + 6 + int64_t(size));
            p = q + 6 + int64_t(size);
        }
        out.append(R::Resource(rtype, name, lang, flags, data));
    }
    return out;
}

Result compare(const py::list<R::Resource>& original, const py::list<R::Resource>& rebuilt, int64_t wide,
               const std::optional<Result>& result_in) {
    Result result = result_in ? *result_in : Result();

    using Groups = py::dict<Key, std::tuple<int64_t, py::list<py::bytes>>>;
    auto images = [&](const py::list<R::Resource>& found) -> std::tuple<Groups, py::set<int64_t>> {
        py::dict<Key, py::list<int64_t>> groups = _members(found, wide);
        py::dict<Key, int64_t> by_key;
        for (int64_t i = 0; i < found.size(); ++i) by_key.set(fold(found[i].key()), i);
        Groups out;
        py::set<int64_t> used;
        for (const auto& kv : groups.items()) {
            const Key& key = std::get<0>(kv);
            int64_t kind = std::get<0>(R::group(found[by_key.at(fold(key))].data));
            py::list<py::bytes> datas;
            for (int64_t m : std::get<1>(kv).vec()) {
                datas.append(found[m].data);
                used.add(m);
            }
            out.set(fold(key), std::make_tuple(kind, datas));
        }
        return std::make_tuple(out, used);
    };

    auto [old_groups, old_used] = images(original);
    auto [new_groups, new_used] = images(rebuilt);
    py::dict<Key, py::list<R::Resource>> new_;
    for (int64_t i = 0; i < rebuilt.size(); ++i)
        if (!new_used.contains(i)) new_.setdefault(fold(rebuilt[i].key()), py::list<R::Resource>()).append(rebuilt[i]);
    py::set<Key> seen;
    for (int64_t i = 0; i < original.size(); ++i) {
        const R::Resource& r = original[i];
        if (old_used.contains(i)) continue;
        result.total += 1;
        Key key = fold(r.key());
        seen.add(key);
        py::list<R::Resource> twin = new_.get(key, py::list<R::Resource>());
        if (twin.empty()) {
            result.missing.append(r.key());
        } else if (twin.size() > 1) {
            result.different.append(r.key());
        } else if (old_groups.contains(key)) {
            bool same = false;
            if (new_groups.contains(key)) {
                const auto& og = old_groups.at(key);
                const auto& ng = new_groups.at(key);
                same = std::get<0>(og) == std::get<0>(ng) && std::get<1>(og).size() == std::get<1>(ng).size();
                for (int64_t k = 0; same && k < std::get<1>(og).size(); ++k)
                    if (!R::same_data(std::get<1>(og)[k], std::get<1>(ng)[k])) same = false;
            }
            if (same && r.data == twin[0].data)
                result.identical += 1;
            else if (same)
                result.equivalent += 1;
            else
                result.different.append(r.key());
        } else if (r.data == twin[0].data || (!wide && R::same_data(r.data, twin[0].data))) {
            result.identical += 1;
        } else {
            R::Decoded a = R::decode(r, wide != 0), b = R::decode(twin[0], wide != 0);
            if (!std::holds_alternative<std::monostate>(a) && same_decoded(a, b))
                result.equivalent += 1;
            else
                result.different.append(r.key());
        }
    }
    py::list<Key> extra;
    for (const auto& kv : new_.items())
        if (!seen.contains(std::get<0>(kv)))
            for (const R::Resource& r : std::get<1>(kv).vec()) extra.append(r.key());
    result.extra = extra;
    return result;
}

Result rebuild(const py::str& module_path, const py::str& outdir, int64_t rounds) {
    py::list<R::Resource> original = R::read(module_path);
    py::set<Key> verbatim;
    Result result;
    if (original.empty()) {
        result.error = py::str("the module has no resources");
        return result;
    }
    for (int64_t round = 0; round < rounds; ++round) {
        result = Result();
        auto [rc, spans, how, wide] = write_rc(module_path, outdir, verbatim);
        result.rc = rc;
        auto [res, log] = compile_rc(rc, wide);
        if (!res) {
            std::optional<py::re::Match> m = py::re::search(py::str("\\((\\d+)\\): Error!"), log);
            int64_t line = m ? py::int_parse(m->group(1)) : -1;
            py::set<Key> bad;
            for (const Span& s : spans.vec())
                if (std::get<0>(s) <= line && line <= std::get<1>(s) && !verbatim.contains(std::get<2>(s))) bad.add(std::get<2>(s));
            if (bad.empty()) {
                py::str tail = log.strip();
                tail = tail.slice(std::max<int64_t>(tail.size() - 200, 0), tail.size());
                result.error = py::str("the resource compiler rejected the file: ") + (tail.empty() ? py::str("no message") : tail);
                return result;
            }
            verbatim.update(bad);
            continue;
        }
        result.res = res;
        result = compare(original, read_res(*res, wide), wide, result);
        result.as_text = how.at(py::str("text")).size();
        result.as_files = how.at(py::str("files")).size();
        result.opaque = how.at(py::str("opaque")).size();
        result.fallback = how.at(py::str("fallback")).size();
        result.not_source = how.at(py::str("fallback"));
        py::set<Key> again;
        for (const Key& k : result.different)
            if (!verbatim.contains(k)) again.add(k);
        if (again.empty()) {
            result.link_res = std::get<0>(compile_rc(rc, wide, false));
            break;
        }
        verbatim.update(again);
    }
    return result;
}

int64_t main(const py::list<py::str>& argv) {
    std::optional<py::str> out;
    if (argv.contains(py::str("--out"))) out = argv[argv.index(py::str("--out")) + 1];
    py::list<py::str> names;
    for (const py::str& a : argv.vec())
        if (!a.startswith("--") && !(out && a == *out)) names.append(a);
    if (names.empty()) {
        py::print(py::str(_DOC));
        return 2;
    }
    int64_t bad = 0;
    for (const py::str& name : names.vec()) {
        std::optional<py::str> path = corpus::retail(name);
        if (!path) {
            py::print(py::percent(py::str("%s: no such module in the corpus"), name));
            bad += 1;
            continue;
        }
        std::optional<converted::Project> proj;
        py::str where;
        if (out) {
            where = *out;
        } else {
            proj = converted::project(*path);
            where = py::os::path::join(proj->assets, py::str("resources"));
        }
        Result r = rebuild(*path, where);
        if (proj) proj->record(py::str("resources"), py::list<py::str>{where},
                               py::percent(py::str("its resources as files and a .rc script: %s"), r.line()));
        py::print(py::percent(py::str("%s: %s"), py::os::path::basename(*path).lower(), r.line()));
        py::list<Key> not_reproduced = r.different + r.missing;
        for (const Key& key : not_reproduced.vec())
            py::print(py::percent(py::str("    not reproduced: %s %r"), R::type_name(std::get<0>(key)), std::get<1>(key)));
        if (r.rc) py::print(py::percent(py::str("    %s"), *r.rc));
        bad += r.same() ? 0 : 1;
    }
    return bad ? 1 : 0;
}

}  // namespace codebreaker::build::resources
