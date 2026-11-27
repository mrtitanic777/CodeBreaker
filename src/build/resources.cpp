#include "build/resources.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <unordered_map>

#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/log.hpp"
#include "core/process.hpp"
#include "core/text.hpp"
#include "formats/pe.hpp"

namespace cb::build::resources {

namespace {

namespace stdfs = std::filesystem;

using Lines = std::vector<std::string>;
using Ident = fr::Ident;

// The memory flags of a 16-bit resource: the bit, the word when it is set, the word when it is clear.
struct MemoryFlag {
    std::uint16_t bit;
    std::string_view on, off;
};
constexpr MemoryFlag kMemory[] = {{0x40, "PRELOAD", "LOADONCALL"},
                                  {0x10, "MOVEABLE", "FIXED"},
                                  {0x1000, "DISCARDABLE", ""},
                                  {0x20, "PURE", "IMPURE"}};

struct NamedBit {
    std::uint32_t bit;
    std::string_view name;
};
constexpr NamedBit kMenuOptions[] = {{0x01, "GRAYED"},       {0x02, "INACTIVE"},  {0x08, "CHECKED"},
                                     {0x20, "MENUBARBREAK"}, {0x40, "MENUBREAK"}, {0x4000, "HELP"}};
constexpr NamedBit kKeyOptions[] = {{0x02, "NOINVERT"}, {0x04, "SHIFT"}, {0x08, "CONTROL"}, {0x10, "ALT"}};
constexpr std::uint32_t kMenuOptionBits = 0x01 | 0x02 | 0x08 | 0x20 | 0x40 | 0x4000;

constexpr std::uint32_t kVersionSignature = 0xFEEF04BD;
constexpr std::uint32_t kStyleChild = 0x40000000, kStyleVisible = 0x10000000;

// The compiler takes this long at most.
constexpr std::chrono::minutes kCompilerTimeout{10};

constexpr char kEol = '\n';

bool is_number(const Ident& v, std::uint32_t n) {
    const std::uint32_t* p = std::get_if<std::uint32_t>(&v);
    return p != nullptr && *p == n;
}

bool is_empty_text(const Ident& v) {
    const std::string* p = std::get_if<std::string>(&v);
    return p != nullptr && p->empty();
}

bool is_type(const fr::Resource& r, std::uint32_t type) { return is_number(r.type, type); }

// The error is "text beyond Latin-1", which write_rc answers by starting over in UTF-8.
bool is_beyond_latin1(const Error& e) { return e.kind() == "BeyondLatin1"; }

// A key with its names in upper case: the compiler is not particular about the case of a name.
Key folded(const Key& key) {
    Key out = key;
    if (auto* t = std::get_if<std::string>(&out.type)) *t = text::upper(*t);
    if (auto* n = std::get_if<std::string>(&out.name)) *n = text::upper(*n);
    return out;
}

// Run a decoder-using writer; the decoders throw FormatError for damaged bytes, which for the writer is a resource
// with no source form (it is carried as a payload).
template <class F>
cb::Result<Lines> damaged_is_failure(F&& f) {
    try {
        return f();
    } catch (const FormatError& e) {
        return Failure(e);
    }
}

// ---- code points -------------------------------------------------------------------------------------------------

// The next code point of UTF-8 text (the text of the readers is valid UTF-8; a stray byte is U+FFFD).
char32_t next_code_point(std::string_view s, std::size_t& i) {
    const auto b = static_cast<unsigned char>(s[i]);
    std::size_t extra = 0;
    char32_t cp = b;
    if (b >= 0xF0) {
        extra = 3;
        cp = b & 0x07;
    } else if (b >= 0xE0) {
        extra = 2;
        cp = b & 0x0F;
    } else if (b >= 0xC0) {
        extra = 1;
        cp = b & 0x1F;
    } else if (b >= 0x80) {
        ++i;
        return 0xFFFD;
    }
    if (i + extra >= s.size()) {   // cut off in the middle of a character
        ++i;
        return 0xFFFD;
    }
    ++i;
    for (std::size_t k = 0; k < extra; ++k, ++i) cp = (cp << 6) | (static_cast<unsigned char>(s[i]) & 0x3F);
    return cp;
}

void octal(std::string& out, char32_t c) {
    out += '\\';
    out += static_cast<char>('0' + ((c >> 6) & 7));
    out += static_cast<char>('0' + ((c >> 3) & 7));
    out += static_cast<char>('0' + (c & 7));
}

// ---- the pieces of a statement ---------------------------------------------------------------------------------------

cb::Result<std::string> ident(const Ident& value, Text mode) {
    if (const auto* n = std::get_if<std::uint32_t>(&value)) return std::to_string(*n);
    return quoted(std::get<std::string>(value), mode);
}

std::string memory(std::uint16_t flags, Text mode) {
    if (mode != Text::Narrow) return {};
    std::string out;
    for (const MemoryFlag& m : kMemory) {
        const std::string_view word = (flags & m.bit) != 0 ? m.on : m.off;
        if (!word.empty()) out += " " + std::string(word);
    }
    return out;
}

// An identifier. An extended dialog or menu has room for 32 bits, but the compiler keeps 16 however the number is
// written (-1 comes out as 0xFFFF).
cb::Result<std::string> number(std::uint32_t value) {
    if (value > 0xFFFF) return NoSourceForm("an identifier wider than 16 bits");
    return std::to_string(value);
}

// A control's style: the two bits a CONTROL statement adds by itself are removed when the original does not have them.
std::string style(std::uint32_t value) {
    std::string s = std::format("0x{:08X}L", value);
    if ((value & kStyleVisible) == 0) s += " | NOT 0x10000000L";
    if ((value & kStyleChild) == 0) s += " | NOT 0x40000000L";
    return s;
}

// Every character that is not [A-Za-z0-9] becomes '_' (a character, not a byte: a name in Korean gives the same file
// name whatever its encoding).
std::string identifier_characters(std::string_view s) {
    std::string out;
    for (std::size_t i = 0; i < s.size();) {
        const char32_t c = next_code_point(s, i);
        const bool plain = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
        out += plain ? static_cast<char>(c) : '_';
    }
    return out;
}

// The file name a payload of this resource gets: type, name and language, in lower case.
std::string stem_of(const fr::Resource& r) {
    const std::string name = std::holds_alternative<std::uint32_t>(r.name) ? std::to_string(std::get<std::uint32_t>(r.name))
                                                                              : identifier_characters(std::get<std::string>(r.name));
    std::string s = identifier_characters(text::replace_all(fr::type_name(r.type), "#", "type") + "_" + name);
    s = text::lower(s);
    if (r.language && *r.language != 0) s += std::format("_{:04x}", *r.language);
    return s;
}

// ---- statements for the kinds that have a source form ---------------------------------------------------------------------

// Append to `out` the statements of one level of a plain menu.
cb::Result<void> plain_menu_level(const std::vector<fr::MenuItem>& items, std::size_t depth, Text mode, Lines& out) {
    const std::string pad(depth * 2, ' ');
    out.push_back(pad + "BEGIN");
    for (const fr::MenuItem& it : items) {
        CB_TRY_ASSIGN(const std::string text, quoted(it.text, mode));
        const std::uint32_t flags = it.flags & ~0x10u;
        if ((flags & ~kMenuOptionBits) != 0) return NoSourceForm(std::format("menu item flags 0x{:x}", flags));
        std::string options;
        for (const NamedBit& o : kMenuOptions)
            if ((flags & o.bit) != 0) options += ", " + std::string(o.name);
        if (it.items) {
            out.push_back(std::format("{}  POPUP {}{}", pad, text, options));
            CB_TRY(plain_menu_level(*it.items, depth + 1, mode, out));
        } else if (it.text.empty() && it.id == 0 && flags == 0) {
            out.push_back(pad + "  MENUITEM SEPARATOR");
        } else if (it.text.empty()) {
            return NoSourceForm("an item with no text that is not a separator");
        } else {
            out.push_back(std::format("{}  MENUITEM {}, {}{}", pad, text, it.id, options));
        }
    }
    out.push_back(pad + "END");
    return {};
}

cb::Result<void> extended_menu_level(const std::vector<fr::ExtMenuItem>& items, std::size_t depth, Text mode,
                                     Lines& out) {
    const std::string pad(depth * 2, ' ');
    out.push_back(pad + "BEGIN");
    for (const fr::ExtMenuItem& it : items) {
        CB_TRY_ASSIGN(const std::string text, quoted(it.text, mode));
        CB_TRY_ASSIGN(const std::string id, number(it.id));
        const std::string tail = std::format(", {}, 0x{:X}L, 0x{:X}L", id, it.type, it.state);
        if (it.items) {
            out.push_back(std::format("{}  POPUP {}{}, {}", pad, text, tail, it.help));
            CB_TRY(extended_menu_level(*it.items, depth + 1, mode, out));
        } else {
            out.push_back(std::format("{}  MENUITEM {}{}", pad, text, tail));
        }
    }
    out.push_back(pad + "END");
    return {};
}

// How deeply two decoded values are the same thing: what compare() asks of a resource the compiler wrote differently.
bool same_menu_items(const std::vector<fr::MenuItem>& a, const std::vector<fr::MenuItem>& b);
bool same_menu_items(const std::vector<fr::ExtMenuItem>& a, const std::vector<fr::ExtMenuItem>& b);

template <class Item>
bool same_submenus(const std::optional<std::vector<Item>>& a, const std::optional<std::vector<Item>>& b) {
    if (a.has_value() != b.has_value()) return false;
    return !a || same_menu_items(*a, *b);
}

bool same_menu_items(const std::vector<fr::MenuItem>& a, const std::vector<fr::MenuItem>& b) {
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i)
        if (!(a[i].text == b[i].text && a[i].id == b[i].id && a[i].flags == b[i].flags && same_submenus(a[i].items, b[i].items)))
            return false;
    return true;
}

bool same_menu_items(const std::vector<fr::ExtMenuItem>& a, const std::vector<fr::ExtMenuItem>& b) {
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i)
        if (!(a[i].text == b[i].text && a[i].id == b[i].id && a[i].type == b[i].type && a[i].state == b[i].state &&
              a[i].help == b[i].help && same_submenus(a[i].items, b[i].items)))
            return false;
    return true;
}

bool same_font(const std::optional<fr::DialogFont>& a, const std::optional<fr::DialogFont>& b) {
    if (a.has_value() != b.has_value()) return false;
    return !a || (a->points == b->points && a->weight == b->weight && a->italic == b->italic && a->charset == b->charset &&
                  a->face == b->face);
}

bool same_decoded(const fr::Decoded& a, const fr::Decoded& b) {
    if (a.index() != b.index()) return false;
    if (const auto* x = std::get_if<std::vector<std::string>>(&a)) return *x == std::get<std::vector<std::string>>(b);
    if (const auto* x = std::get_if<fr::Dialog>(&a)) {
        const auto& y = std::get<fr::Dialog>(b);
        if (!(x->extended == y.extended && x->style == y.style && x->exstyle == y.exstyle && x->help == y.help && x->x == y.x &&
              x->y == y.y && x->cx == y.cx && x->cy == y.cy && x->menu == y.menu && x->window_class == y.window_class &&
              x->title == y.title && same_font(x->font, y.font) && x->controls.size() == y.controls.size()))
            return false;
        for (std::size_t i = 0; i < x->controls.size(); ++i) {
            const fr::DialogControl &c = x->controls[i], &d = y.controls[i];
            if (!(c.help == d.help && c.exstyle == d.exstyle && c.style == d.style && c.x == d.x && c.y == d.y &&
                  c.cx == d.cx && c.cy == d.cy && c.id == d.id && c.window_class == d.window_class && c.text == d.text &&
                  c.extra == d.extra))
                return false;
        }
        return true;
    }
    if (const auto* x = std::get_if<fr::Menu>(&a)) {
        const auto& y = std::get<fr::Menu>(b);
        if (!(x->extended == y.extended && x->help == y.help && x->items.index() == y.items.index())) return false;
        if (const auto* l = std::get_if<std::vector<fr::MenuItem>>(&x->items))
            return same_menu_items(*l, std::get<std::vector<fr::MenuItem>>(y.items));
        return same_menu_items(std::get<std::vector<fr::ExtMenuItem>>(x->items), std::get<std::vector<fr::ExtMenuItem>>(y.items));
    }
    if (const auto* x = std::get_if<std::vector<fr::Accelerator>>(&a)) {
        const auto& y = std::get<std::vector<fr::Accelerator>>(b);
        if (x->size() != y.size()) return false;
        for (std::size_t i = 0; i < x->size(); ++i)
            if (!((*x)[i].flags == y[i].flags && (*x)[i].key == y[i].key && (*x)[i].id == y[i].id)) return false;
        return true;
    }
    const auto& x = std::get<fr::Version>(a);
    const auto& y = std::get<fr::Version>(b);
    if (x.fixed.has_value() != y.fixed.has_value() || (x.fixed && !(*x.fixed == *y.fixed))) return false;
    if (x.translations != y.translations || x.strings.size() != y.strings.size()) return false;
    for (std::size_t i = 0; i < x.strings.size(); ++i)
        if (!(x.strings[i].language_block == y.strings[i].language_block && x.strings[i].entries == y.strings[i].entries))
            return false;
    return true;
}

// ---- icon and cursor groups ----------------------------------------------------------------------------------------

// The group a resource holds; nullopt when its bytes do not read as one.
std::optional<fr::Group> try_group(ByteView data) {
    try {
        return fr::group(data);
    } catch (const FormatError&) {
        return std::nullopt;
    }
}

// Per icon or cursor group that can be put back together as a file, the members it takes (indexes into the module's
// resources, in the group's order). A group with a member that is not there is left as it is. Lookup only.
std::map<Key, std::vector<std::size_t>> members_of(std::span<const fr::Resource> found) {
    std::map<std::pair<Ident, Ident>, std::vector<std::size_t>> by_id;   // (member type, id) -> resources
    for (std::size_t i = 0; i < found.size(); ++i)
        if (is_type(found[i], fr::kIcon) || is_type(found[i], fr::kCursor)) by_id[{found[i].type, found[i].name}].push_back(i);
    std::map<Key, std::vector<std::size_t>> taken;
    for (const fr::Resource& g : found) {
        const std::uint32_t* group_type = std::get_if<std::uint32_t>(&g.type);
        if (group_type == nullptr) continue;
        const std::optional<std::uint32_t> member_type = fr::member_type_of(*group_type);
        if (!member_type) continue;
        const std::optional<fr::Group> group = try_group(g.data);
        if (!group) continue;
        std::vector<std::size_t> mine;
        bool complete = true;
        for (const fr::GroupEntry& e : group->entries) {
            const auto candidates = by_id.find({Ident(*member_type), Ident(std::uint32_t{e.member_id})});
            if (candidates == by_id.end()) {
                complete = false;
                break;
            }
            // the member of the group's own language, else the first
            std::size_t pick = candidates->second.front();
            for (std::size_t c : candidates->second)
                if (found[c].language == g.language) {
                    pick = c;
                    break;
                }
            mine.push_back(pick);
        }
        if (complete) taken[g.key()] = std::move(mine);
    }
    return taken;
}

// ---- writing the .rc -------------------------------------------------------------------------------------------------

// A payload file beside the .rc, named relative to the output folder.
struct PayloadFile {
    std::string path;   // "res/<name>"
    Bytes data;
};

// The statements of a module's .rc and the files they name, worked out in memory (so that starting over in another
// text mode leaves nothing behind), then written.
class RcPlan {
public:
    RcPlan(const Module& module, Text mode, std::set<Key> verbatim)
        : module_(&module), mode_(mode), verbatim_(std::move(verbatim)) {}

    // The plan, or the first error that stops it (BeyondLatin1: start over in Unicode).
    cb::Result<void> build();

    Text mode() const { return mode_; }
    const Lines& lines() const { return lines_; }
    const std::vector<Span>& spans() const { return spans_; }
    const How& how() const { return how_; }
    const std::vector<PayloadFile>& files() const { return files_; }

private:
    // One entry of the string tables the module holds, by language and memory flags (the tables are written last,
    // in the order their first string table was met; there are few, so a vector is searched).
    struct TableBlock {
        Key key;
        std::vector<std::pair<std::int64_t, std::string>> strings;
    };
    struct Table {
        std::optional<std::uint32_t> language;
        std::uint16_t flags = 0;
        std::vector<TableBlock> blocks;
    };

    void emit(const std::vector<Key>& keys, Lines statements, std::vector<Key> How::*kind, std::optional<std::uint32_t> language);
    std::string payload(const fr::Resource& r, std::string_view extension, ByteView data);
    cb::Result<void> raw(const fr::Resource& r, std::vector<Key> How::*kind);
    cb::Result<void> add_group(const fr::Resource& group, const std::vector<std::size_t>& members, std::string_view word,
                               std::string_view extension, bool* written);
    cb::Result<void> add_string_table(const fr::Resource& r, std::vector<Table>& tables);

    const Module* module_;
    Text mode_;
    std::set<Key> verbatim_;
    Lines lines_;
    std::vector<Span> spans_;
    How how_;
    std::vector<PayloadFile> files_;
    std::unordered_map<std::string, std::size_t> file_index_;
    std::optional<std::uint32_t> language_;
    std::map<Key, std::vector<std::size_t>> groups_;
};

void RcPlan::emit(const std::vector<Key>& keys, Lines statements, std::vector<Key> How::*kind,
                  std::optional<std::uint32_t> language) {
    if (mode_ != Text::Narrow && language != language_) {
        const std::uint32_t lang = language.value_or(0);
        lines_.push_back(std::format("LANGUAGE 0x{:02X}, 0x{:02X}", lang & 0x3FF, lang >> 10));
        language_ = language;
    }
    const std::size_t first = lines_.size() + 1;
    for (std::string& s : statements) lines_.push_back(std::move(s));
    for (const Key& k : keys) {
        spans_.push_back({first, lines_.size(), k});
        (how_.*kind).push_back(k);
    }
}

// The file a payload goes to: res/<stem><extension>, numbered when another resource has already taken that name with
// other bytes. Returns the name as the statement spells it (quoted).
std::string RcPlan::payload(const fr::Resource& r, std::string_view extension, ByteView data) {
    const std::string stem = stem_of(r);
    std::string name = std::format("res/{}{}", stem, extension);
    for (int n = 2;; ++n) {
        const auto taken = file_index_.find(name);
        if (taken == file_index_.end()) {
            file_index_[name] = files_.size();
            files_.push_back({name, Bytes(data.begin(), data.end())});
            break;
        }
        const Bytes& have = files_[taken->second].data;
        if (std::equal(have.begin(), have.end(), data.begin(), data.end())) break;
        name = std::format("res/{}_{}{}", stem, n, extension);
    }
    return "\"" + name + "\"";
}

cb::Result<void> RcPlan::raw(const fr::Resource& r, std::vector<Key> How::*kind) {
    const std::optional<fr::TypeLibrary> library = fr::type_library(r.data);
    const std::string file = payload(r, library ? ".tlb" : ".bin", r.data);
    if (library) {   // beside the payload, an outline to read
        const std::string outline = fr::type_library_outline(*library);
        const std::string idl = file.substr(1, file.size() - 6) + ".idl";   // (without the quotes and the ".tlb")
        const auto taken = file_index_.find(idl);
        if (taken == file_index_.end()) {
            file_index_[idl] = files_.size();
            files_.push_back({idl, Bytes(outline.begin(), outline.end())});
        } else {
            files_[taken->second].data.assign(outline.begin(), outline.end());
        }
    }
    CB_TRY_ASSIGN(const std::string name, ident(r.name, mode_));
    CB_TRY_ASSIGN(const std::string type, ident(r.type, mode_));
    emit({r.key()}, {std::format("{} {}{} {}", name, type, memory(r.flags, mode_), file)}, kind, r.language);
    return {};
}

cb::Result<void> RcPlan::add_group(const fr::Resource& group, const std::vector<std::size_t>& members,
                                   std::string_view word, std::string_view extension, bool* written) {
    std::map<std::uint32_t, const Bytes*> by_id;
    for (std::size_t m : members)
        if (const auto* id = std::get_if<std::uint32_t>(&module_->resources[m].name)) by_id[*id] = &module_->resources[m].data;
    Bytes image;
    try {
        image = fr::icon_file(group.data, [&](std::uint16_t id) -> Bytes {
            const auto it = by_id.find(id);
            if (it == by_id.end()) throw FormatError("build/resources", 0, std::format("member {} of an image group is missing", id));
            return *it->second;
        });
    } catch (const FormatError&) {
        *written = false;   // the group and all its members are carried as they are
        return {};
    }
    *written = true;
    CB_TRY_ASSIGN(const std::string name, ident(group.name, mode_));
    std::vector<Key> keys{group.key()};
    for (std::size_t m : members) keys.push_back(module_->resources[m].key());
    emit(keys, {std::format("{} {}{} {}", name, word, memory(group.flags, mode_), payload(group, extension, image))}, &How::files,
         group.language);
    return {};
}

cb::Result<void> RcPlan::add_string_table(const fr::Resource& r, std::vector<Table>& tables) {
    // A string table holds 16 strings; the empty ones are not there. A table with no source form is carried.
    std::vector<std::pair<std::int64_t, std::string>> texts;
    try {
        const std::vector<std::string> strings = fr::strings(r.data, mode_ != Text::Narrow);
        const std::int64_t first = 16 * (static_cast<std::int64_t>(std::get<std::uint32_t>(r.name)) - 1);
        for (std::size_t i = 0; i < strings.size(); ++i) {
            if (strings[i].empty()) continue;
            cb::Result<std::string> q = quoted(strings[i], mode_, true);
            if (!q.ok()) {
                if (is_beyond_latin1(q.error())) return Failure(q.shared_error());
                return raw(r, &How::fallback);
            }
            texts.emplace_back(first + static_cast<std::int64_t>(i), std::move(q).value());
        }
    } catch (const FormatError&) {
        return raw(r, &How::fallback);
    }
    auto table = std::find_if(tables.begin(), tables.end(),
                              [&](const Table& t) { return t.language == r.language && t.flags == r.flags; });
    if (table == tables.end()) {
        tables.push_back({r.language, r.flags, {}});
        table = tables.end() - 1;
    }
    table->blocks.push_back({r.key(), std::move(texts)});
    return {};
}

cb::Result<void> RcPlan::build() {
    const std::vector<fr::Resource>& found = module_->resources;
    const std::string stem = text::lower(module_->path.stem().string());
    groups_ = members_of(found);
    // Member numbering is the compiler's: all image groups are written as files, or all are carried as they are.
    const bool all_verbatim = std::any_of(groups_.begin(), groups_.end(), [&](const auto& g) { return verbatim_.contains(g.first); });
    if (all_verbatim)
        for (const auto& g : groups_) verbatim_.insert(g.first);
    for (auto it = groups_.begin(); it != groups_.end();)
        it = verbatim_.contains(it->first) ? groups_.erase(it) : std::next(it);
    std::set<std::size_t> in_group;
    for (const auto& g : groups_) in_group.insert(g.second.begin(), g.second.end());

    lines_.push_back(std::format("/* {}.rc -- the resources of {}. Generated by `codebreaker resources`. */", stem,
                                 text::lower(module_->path.filename().string())));
    std::vector<Table> tables;
    for (std::size_t i = 0; i < found.size(); ++i) {
        const fr::Resource& r = found[i];
        if (in_group.contains(i)) continue;
        if (verbatim_.contains(r.key())) {
            CB_TRY(raw(r, &How::fallback));
        } else if (is_type(r, fr::kString) && std::holds_alternative<std::uint32_t>(r.name)) {
            CB_TRY(add_string_table(r, tables));
        } else if (is_type(r, fr::kDialog) || is_type(r, fr::kMenu) || is_type(r, fr::kAccelerator) || is_type(r, fr::kVersion)) {
            cb::Result<Lines> written = is_type(r, fr::kDialog)       ? dialog_text(r, mode_)
                                        : is_type(r, fr::kMenu)       ? menu_text(r, mode_)
                                        : is_type(r, fr::kAccelerator) ? accelerators_text(r, mode_)
                                                                      : version_text(r, mode_);
            if (written.ok()) {
                emit({r.key()}, written.value(), &How::text, r.language);
            } else if (is_beyond_latin1(written.error())) {
                return Failure(written.shared_error());
            } else {
                CB_TRY(raw(r, &How::fallback));   // no source form: its original bytes
            }
        } else if (groups_.contains(r.key())) {
            const bool is_icon = is_type(r, fr::kGroupIcon);
            bool written = false;
            const std::vector<std::size_t>& members = groups_.at(r.key());
            CB_TRY(add_group(r, members, is_icon ? "ICON" : "CURSOR", is_icon ? ".ico" : ".cur", &written));
            if (!written) {
                CB_TRY(raw(r, &How::fallback));
                for (std::size_t m : members) CB_TRY(raw(found[m], &How::fallback));
            }
        } else if (is_type(r, fr::kBitmap)) {
            CB_TRY_ASSIGN(const std::string name, ident(r.name, mode_));
            emit({r.key()}, {std::format("{} BITMAP{} {}", name, memory(r.flags, mode_), payload(r, ".bmp", fr::bitmap_file(r.data)))},
                 &How::files, r.language);
        } else {
            const bool has_source_form_but_is_carried = is_type(r, fr::kString) || is_type(r, fr::kGroupIcon) || is_type(r, fr::kGroupCursor);
            CB_TRY(raw(r, has_source_form_but_is_carried ? &How::fallback : &How::opaque));
        }
    }
    for (const Table& t : tables) {
        Lines statement{std::format("STRINGTABLE{}", memory(t.flags, mode_)), "BEGIN"};
        std::vector<Key> keys;
        for (const TableBlock& b : t.blocks) {
            keys.push_back(b.key);
            for (const auto& [id, text] : b.strings) statement.push_back(std::format("  {}, {}", id, text));
        }
        statement.push_back("END");
        emit(keys, std::move(statement), &How::text, t.language);
    }
    return {};
}

}  // namespace

// ---- Rebuilt ---------------------------------------------------------------------------------------------------------------

bool Rebuilt::same() const {
    return error.empty() && different.empty() && missing.empty() && extra.empty() && identical + equivalent == total;
}

std::string Rebuilt::line() const {
    if (!error.empty()) return "FAILED: " + error;
    const std::string extras = extra.empty() ? std::string() : std::format(", {} that the original does not have", extra.size());
    return std::format(
        "{} resources: {} identical, {} equivalent, {} different, {} missing{}  (written as text {}, as image files {}, "
        "opaque payload {}, fallback payload {})",
        total, identical, equivalent, different.size(), missing.size(), extras, as_text, as_files, opaque, fallback);
}

// ---- the pieces of a statement ---------------------------------------------------------------------------------------------

cb::Result<std::string> quoted(std::string_view text, Text mode, bool counted) {
    std::string out;
    for (std::size_t i = 0; i < text.size();) {
        const char32_t c = next_code_point(text, i);
        if (c == U'"') {
            out += "\"\"";
        } else if (c == U'\\') {
            out += "\\\\";
        } else if (c == U'\n') {
            out += "\\n";
        } else if (c == U'\t') {
            out += "\\t";
        } else if (c == U'\r') {
            out += "\\r";
        } else if (c == 0) {
            return NoSourceForm("a zero character inside a string");
        } else if (c < 0x20 || c == 0x7F) {
            octal(out, c);
        } else if (c <= 0x7F) {
            out += static_cast<char>(c);
        } else if (mode != Text::Unicode) {
            if (c > 0xFF) return BeyondLatin1();
            octal(out, c);
        } else if (counted) {
            return NoSourceForm("text the compiler cannot take");
        } else {
            text::append_utf8(out, c);
        }
    }
    return "\"" + out + "\"";
}

cb::Result<Lines> dialog_text(const fr::Resource& r, Text mode) {
    return damaged_is_failure([&]() -> cb::Result<Lines> {
        const fr::Dialog d = fr::dialog(r.data, mode != Text::Narrow);
        CB_TRY_ASSIGN(const std::string name, ident(r.name, mode));
        std::string head = std::format("{} {}{} {}, {}, {}, {}", name, d.extended ? "DIALOGEX" : "DIALOG", memory(r.flags, mode), d.x,
                                       d.y, d.cx, d.cy);
        if (d.help != 0) head += std::format(", {}", d.help);
        Lines out{head, std::format("STYLE 0x{:08X}L", d.style)};
        if (d.exstyle != 0) out.push_back(std::format("EXSTYLE 0x{:08X}L", d.exstyle));
        if (!d.title.empty()) {
            CB_TRY_ASSIGN(const std::string title, quoted(d.title, mode));
            out.push_back("CAPTION " + title);
        }
        if (!is_empty_text(d.menu)) {
            CB_TRY_ASSIGN(const std::string menu, ident(d.menu, mode));
            out.push_back("MENU " + menu);
        }
        if (!is_empty_text(d.window_class)) {
            CB_TRY_ASSIGN(const std::string window_class, ident(d.window_class, mode));
            out.push_back("CLASS " + window_class);
        }
        if (d.font) {
            CB_TRY_ASSIGN(const std::string face, quoted(d.font->face, mode));
            std::string font = std::format("FONT {}, {}", d.font->points, face);
            if (d.extended) font += std::format(", {}, {}, {}", d.font->weight, d.font->italic, d.font->charset);
            out.push_back(font);
        }
        out.push_back("BEGIN");
        for (const fr::DialogControl& c : d.controls) {
            if (!c.extra.empty()) return NoSourceForm("a control with creation data");
            std::string window_class;
            if (const auto* atom = std::get_if<std::uint32_t>(&c.window_class)) {
                const auto standard = fr::control_class_name(*atom);
                if (!standard) return NoSourceForm(std::format("control class atom 0x{:x}", *atom));
                window_class = std::string(*standard);
            } else {
                window_class = std::get<std::string>(c.window_class);
                for (std::uint32_t standard_atom = 0x80; standard_atom <= 0x85; ++standard_atom)
                    if (text::iequals(window_class, *fr::control_class_name(standard_atom)))
                        return NoSourceForm("a standard control class spelled out");   // the compiler would make it an atom
            }
            CB_TRY_ASSIGN(const std::string control_text, ident(c.text, mode));
            CB_TRY_ASSIGN(const std::string id, number(c.id));
            CB_TRY_ASSIGN(const std::string class_name, quoted(window_class, mode));
            std::string line = std::format("  CONTROL {}, {}, {}, {}, {}, {}, {}, {}", control_text, id, class_name, style(c.style), c.x,
                                           c.y, c.cx, c.cy);
            if (c.exstyle != 0 || c.help != 0) line += std::format(", 0x{:08X}L", c.exstyle);
            if (c.help != 0) line += std::format(", {}", c.help);
            out.push_back(line);
        }
        out.push_back("END");
        return out;
    });
}

cb::Result<Lines> menu_text(const fr::Resource& r, Text mode) {
    return damaged_is_failure([&]() -> cb::Result<Lines> {
        const fr::Menu m = fr::menu(r.data, mode != Text::Narrow);
        if (m.help != 0) return NoSourceForm("a menu with a help id");
        CB_TRY_ASSIGN(const std::string name, ident(r.name, mode));
        Lines out{std::format("{} {}{}", name, m.extended ? "MENUEX" : "MENU", memory(r.flags, mode))};
        if (const auto* plain = std::get_if<std::vector<fr::MenuItem>>(&m.items)) {
            CB_TRY(plain_menu_level(*plain, 0, mode, out));
        } else {
            CB_TRY(extended_menu_level(std::get<std::vector<fr::ExtMenuItem>>(m.items), 0, mode, out));
        }
        return out;
    });
}

cb::Result<Lines> accelerators_text(const fr::Resource& r, Text mode) {
    return damaged_is_failure([&]() -> cb::Result<Lines> {
        const std::vector<fr::Accelerator> table = fr::accelerators(r.data, mode != Text::Narrow);
        CB_TRY_ASSIGN(const std::string name, ident(r.name, mode));
        Lines out{std::format("{} ACCELERATORS{}", name, memory(r.flags, mode)), "BEGIN"};
        for (const fr::Accelerator& a : table) {
            if ((a.flags & ~0x1Fu) != 0) return NoSourceForm(std::format("accelerator flags 0x{:x}", a.flags));
            std::string options;
            for (const NamedBit& o : kKeyOptions)
                if ((a.flags & o.bit) != 0) options += ", " + std::string(o.name);
            out.push_back(std::format("  {}, {}, {}{}", a.key, a.id, (a.flags & 1) != 0 ? "VIRTKEY" : "ASCII", options));
        }
        out.push_back("END");
        return out;
    });
}

cb::Result<Lines> version_text(const fr::Resource& r, Text mode) {
    return damaged_is_failure([&]() -> cb::Result<Lines> {
        if (mode == Text::Narrow) {
            // the compiler leaves the last string of a 16-bit block without its terminator and padding while counting
            // them in every enclosing length: what it writes cannot be read
            return NoSourceForm("the compiler writes an unsound 16-bit version block");
        }
        const fr::Version v = fr::version(r.data, true);
        if (!v.fixed || v.fixed->signature != kVersionSignature || v.fixed->date_ms != 0 || v.fixed->date_ls != 0)
            return NoSourceForm("a version block without the usual fixed part");
        const fr::FixedFileInfo& f = *v.fixed;
        CB_TRY_ASSIGN(const std::string name, ident(r.name, mode));
        Lines out{std::format("{} VERSIONINFO", name)};
        out.push_back(std::format("FILEVERSION {}, {}, {}, {}", f.file_version_ms >> 16, f.file_version_ms & 0xFFFF,
                                  f.file_version_ls >> 16, f.file_version_ls & 0xFFFF));
        out.push_back(std::format("PRODUCTVERSION {}, {}, {}, {}", f.product_version_ms >> 16, f.product_version_ms & 0xFFFF,
                                  f.product_version_ls >> 16, f.product_version_ls & 0xFFFF));
        out.push_back(std::format("FILEFLAGSMASK 0x{:X}L", f.file_flags_mask));
        out.push_back(std::format("FILEFLAGS 0x{:X}L", f.file_flags));
        out.push_back(std::format("FILEOS 0x{:X}L", f.os));
        out.push_back(std::format("FILETYPE 0x{:X}L", f.type));
        out.push_back(std::format("FILESUBTYPE 0x{:X}L", f.subtype));
        out.push_back("BEGIN");
        if (!v.strings.empty()) {
            out.push_back("  BLOCK \"StringFileInfo\"");
            out.push_back("  BEGIN");
            for (const fr::VersionStringTable& table : v.strings) {
                CB_TRY_ASSIGN(const std::string language, quoted(table.language_block, mode));
                out.push_back("    BLOCK " + language);
                out.push_back("    BEGIN");
                for (const auto& [key, value] : table.entries) {
                    CB_TRY_ASSIGN(const std::string k, quoted(key, mode));
                    CB_TRY_ASSIGN(const std::string val, quoted(value, mode));
                    out.push_back(std::format("      VALUE {}, {}", k, val));
                }
                out.push_back("    END");
            }
            out.push_back("  END");
        }
        if (!v.translations.empty()) {
            std::string words;
            for (std::uint16_t w : v.translations) words += (words.empty() ? "" : ", ") + std::format("0x{:X}", w);
            out.push_back("  BLOCK \"VarFileInfo\"");
            out.push_back("  BEGIN");
            out.push_back("    VALUE \"Translation\", " + words);
            out.push_back("  END");
        }
        out.push_back("END");
        return out;
    });
}

// ---- reading the module --------------------------------------------------------------------------------------------------

Module read_module(const stdfs::path& path) {
    Module m;
    m.path = path;
    const Bytes file = fs::read_file(path);
    switch (fr::kind_of(file)) {
        case fr::ExecutableKind::Pe: {
            m.text = Text::Wide;
            const formats::pe::Image pe = formats::pe::Image::parse(ByteView(file), fs::to_text(path));
            std::vector<fr::PeResourceEntry> entries;
            for (const formats::pe::ResourceType& t : pe.resource_tree()) {
                const Ident type = t.id ? Ident(*t.id) : Ident(t.name.value_or(std::string()));
                for (const formats::pe::ResourceItem& item : t.items) {
                    fr::PeResourceEntry e;
                    e.type = type;
                    e.name = item.id ? Ident(*item.id) : Ident(item.name.value_or(std::string()));
                    e.language = item.language;
                    e.file_offset = item.file_offset;
                    e.size = item.size;
                    entries.push_back(std::move(e));
                }
            }
            m.resources = fr::from_pe_entries(file, entries);
            break;
        }
        case fr::ExecutableKind::Ne: m.resources = fr::read_ne(file); break;
        case fr::ExecutableKind::Other: break;
    }
    return m;
}

// ---- writing the .rc ----------------------------------------------------------------------------------------------------

RcFiles write_rc(const Module& module, const stdfs::path& outdir, const std::set<Key>& verbatim) {
    RcPlan plan(module, module.text, verbatim);
    cb::Result<void> planned = plan.build();
    if (!planned.ok() && is_beyond_latin1(planned.error()) && module.text != Text::Unicode) {
        plan = RcPlan(module, Text::Unicode, verbatim);   // the text goes beyond Latin-1: the source has to be UTF-8
        planned = plan.build();
    }
    planned.value();   // (any other error is the caller's: a name with no source form)
    stdfs::create_directories(outdir / "res");
    for (const PayloadFile& f : plan.files()) fs::write_file(outdir / f.path, f.data);
    std::string source = text::join(plan.lines(), "\n");
    source += '\n';
    RcFiles out;
    out.rc = outdir / (text::lower(module.path.stem().string()) + ".rc");
    fs::write_file(out.rc, as_bytes(source));
    out.spans = plan.spans();
    out.how = plan.how();
    out.text = plan.mode();
    return out;
}

// ---- compiling, and reading what the compiler wrote --------------------------------------------------------------------

Compiled compile_rc(const watcom::OpenWatcom& open_watcom, const stdfs::path& rc, Text text, bool microsoft) {
    const bool wide = text != Text::Narrow;   // (the resource compiler and the environment of the class it is for)
    const stdfs::path wrc = open_watcom.tool("wrc");
    if (wrc.empty()) throw ToolError("wrc", 0, "", "OpenWatcom's resource compiler is not set up (settings.json: openwatcom)");
    const stdfs::path res = rc.parent_path() / (rc.stem().string() + (microsoft ? ".res" : "_link.res"));
    std::error_code ignored;
    stdfs::remove(res, ignored);
    // -r: only the .res; -zm: the Microsoft layout; -zku8: the source is UTF-8 and becomes UTF-16.
    // (The preprocessor is left on: without it a comment is a syntax error.)
    process::ProcessSpec spec;
    spec.argv = {fs::to_text(wrc), "-q", "-r", wide ? "-bt=nt" : "-bt=windows"};
    if (microsoft) spec.argv.push_back("-zm");
    if (text == Text::Unicode) spec.argv.push_back("-zku8");
    spec.argv.push_back("-fo=" + res.filename().string());
    spec.argv.push_back(rc.filename().string());
    spec.cwd = rc.parent_path();
    spec.env = open_watcom.environment(wide ? watcom::Target::Win32 : watcom::Target::Win16);
    spec.timeout = kCompilerTimeout;
    const process::ProcessResult ran = process::run(spec);
    Compiled out;
    out.log = text::from_latin1(ran.out) + text::from_latin1(ran.err);
    if (ran.exit_code == 0 && !ran.timed_out && stdfs::is_regular_file(res)) out.res = res;
    return out;
}

namespace {

// Where the Microsoft-layout reader is: a cursor over the file that says what it found and where it stopped.
class ResReader {
public:
    ResReader(ByteView d, bool wide) : d_(d), wide_(wide) {}
    std::vector<fr::Resource> read();

private:
    std::uint16_t u16(std::size_t at) const { return static_cast<std::uint16_t>(d_[at] | (d_[at + 1] << 8)); }
    std::uint32_t u32(std::size_t at) const { return u16(at) | (static_cast<std::uint32_t>(u16(at + 2)) << 16); }
    // A name or type of the 16-bit layout: 0xFF and an id, or a zero-ended string. nullopt: it runs off the file.
    std::optional<std::pair<Ident, std::size_t>> name16(std::size_t at) const;
    // The 32-bit layout: 0xFFFF and an id, or a UTF-16 string ended by a zero word.
    std::optional<std::pair<Ident, std::size_t>> name32(std::size_t at) const;
    // Does a 32-bit entry start here: a header as long as its names make it, and its body inside the file.
    bool entry_at(std::size_t at) const;

    ByteView d_;
    bool wide_;
};

std::optional<std::pair<Ident, std::size_t>> ResReader::name16(std::size_t at) const {
    if (at >= d_.size()) return std::nullopt;
    if (d_[at] == 0xFF) {
        if (at + 3 > d_.size()) return std::nullopt;
        return std::make_pair(Ident(std::uint32_t{u16(at + 1)}), at + 3);
    }
    std::size_t e = at;
    while (e < d_.size() && d_[e] != 0) ++e;
    if (e >= d_.size()) return std::nullopt;
    return std::make_pair(Ident(text::from_latin1(d_.subspan(at, e - at))), e + 1);
}

std::optional<std::pair<Ident, std::size_t>> ResReader::name32(std::size_t at) const {
    if (at + 2 > d_.size()) return std::nullopt;
    if (u16(at) == 0xFFFF) {
        if (at + 4 > d_.size()) return std::nullopt;
        return std::make_pair(Ident(std::uint32_t{u16(at + 2)}), at + 4);
    }
    std::size_t e = at;
    while (e + 2 <= d_.size() && !(d_[e] == 0 && d_[e + 1] == 0)) e += 2;
    if (e + 2 > d_.size()) return std::nullopt;
    return std::make_pair(Ident(text::from_utf16le(d_.subspan(at, e - at))), e + 2);
}

bool ResReader::entry_at(std::size_t at) const {
    if (at + 32 > d_.size()) return false;
    const std::uint32_t size = u32(at), header = u32(at + 4);
    if (header < 24 || header > 4096 || at + header + std::uint64_t{size} > d_.size()) return false;
    const auto type = name32(at + 8);
    if (!type) return false;
    const auto name = name32(type->second);
    if (!name) return false;
    return ((name->second - at + 3) & ~std::size_t{3}) + 16 == header;   // the header is as long as its names make it
}

std::vector<fr::Resource> ResReader::read() {
    std::vector<fr::Resource> out;
    std::size_t p = 0;
    while (p + (wide_ ? 16 : 8) <= d_.size()) {
        fr::Resource r;
        if (wide_) {
            // The Microsoft compiler pads every body to four bytes, so every entry starts on a four-byte boundary. This
            // compiler leaves some bodies unpadded: an entry can start anywhere, the fixed part of its header is then
            // moved up to the next boundary of the file, and the next entry is looked for in the bytes after the body.
            std::optional<std::size_t> found;
            for (std::size_t k = 0; k < 16; ++k)
                if (entry_at(p + k)) {
                    found = p + k;
                    break;
                }
            if (!found) break;
            p = *found;
            const std::uint32_t size = u32(p);
            const auto type = *name32(p + 8);
            const auto name = *name32(type.second);
            const std::size_t q = (name.second + 3) & ~std::size_t{3};   // padding the header's length does not count
            r.type = type.first;
            r.name = name.first;
            r.flags = u16(q + 4);
            r.language = u16(q + 6);
            const std::size_t body = q + 16;
            // entry_at() has seen the entry lie inside the file by its own header's arithmetic; moving the fixed part up
            // to the file's boundary can push the body of the last entry up to three bytes past the end. It keeps the
            // bytes there are (the same clamp the layout has always been read with, and the only one).
            const std::size_t available = body < d_.size() ? d_.size() - body : 0;
            const std::size_t taken = std::min<std::size_t>(size, available);
            r.data.assign(d_.begin() + static_cast<std::ptrdiff_t>(std::min(body, d_.size())),
                          d_.begin() + static_cast<std::ptrdiff_t>(std::min(body, d_.size()) + taken));
            p = body + size;
            if (is_number(r.type, 0) && is_number(r.name, 0) && size == 0) continue;   // the marker a 32-bit .res starts with
        } else {
            while (p < d_.size() && d_[p] == 0) ++p;   // padding between entries
            if (p + 8 > d_.size()) break;
            const auto type = name16(p);
            if (!type) throw TruncatedError("res", p, 1, d_.size() - p);
            const auto name = name16(type->second);
            if (!name) throw TruncatedError("res", type->second, 1, d_.size() - type->second);
            const std::size_t q = name->second;
            if (q + 6 > d_.size()) throw TruncatedError("res", q, 6, d_.size() - std::min(q, d_.size()));
            const std::uint32_t size = u32(q + 2);
            if (q + 6 + std::uint64_t{size} > d_.size()) throw TruncatedError("res", q + 6, size, d_.size() - q - 6);
            r.type = type->first;
            r.name = name->first;
            r.flags = u16(q);
            r.data.assign(d_.begin() + static_cast<std::ptrdiff_t>(q + 6), d_.begin() + static_cast<std::ptrdiff_t>(q + 6 + size));
            p = q + 6 + size;
        }
        out.push_back(std::move(r));
    }
    return out;
}

// The member images of every icon or cursor group of a set of resources, in the group's order, by folded key.
struct ImageGroups {
    std::map<Key, std::pair<fr::GroupKind, std::vector<const Bytes*>>> groups;
    std::set<std::size_t> used;   // the members the groups took
};

ImageGroups image_groups(std::span<const fr::Resource> found) {
    ImageGroups out;
    std::map<Key, std::size_t> by_key;   // (a later resource of the same key takes the name)
    for (std::size_t i = 0; i < found.size(); ++i) by_key[folded(found[i].key())] = i;
    for (const auto& [key, members] : members_of(found)) {
        const std::optional<fr::Group> group = try_group(found[by_key.at(folded(key))].data);
        if (!group) continue;
        std::vector<const Bytes*> images;
        for (std::size_t m : members) {
            images.push_back(&found[m].data);
            out.used.insert(m);
        }
        out.groups[folded(key)] = {group->kind, std::move(images)};
    }
    return out;
}

// The compiler's message "(<line>): Error!": the first line number it names, or 0.
std::size_t error_line(std::string_view log) {
    constexpr std::string_view marker = "): Error!";
    for (std::size_t at = log.find(marker); at != std::string_view::npos; at = log.find(marker, at + 1)) {
        std::size_t start = at;
        while (start > 0 && log[start - 1] >= '0' && log[start - 1] <= '9') --start;
        if (start == at || start == 0 || log[start - 1] != '(') continue;
        return text::parse_int<std::size_t>(log.substr(start, at - start)).value_or(0);
    }
    return 0;
}

}  // namespace

std::vector<fr::Resource> read_res(ByteView file, bool wide) { return ResReader(file, wide).read(); }

void compare(std::span<const fr::Resource> original, std::span<const fr::Resource> rebuilt, bool wide, Rebuilt& into) {
    const ImageGroups old_images = image_groups(original);
    const ImageGroups new_images = image_groups(rebuilt);
    // what the compiler wrote, by folded key (its first appearance orders the extras)
    std::vector<std::pair<Key, std::vector<std::size_t>>> written;
    std::map<Key, std::size_t> written_at;
    for (std::size_t i = 0; i < rebuilt.size(); ++i) {
        if (new_images.used.contains(i)) continue;
        const Key key = folded(rebuilt[i].key());
        auto [it, fresh] = written_at.try_emplace(key, written.size());
        if (fresh) written.push_back({key, {}});
        written[it->second].second.push_back(i);
    }
    std::set<Key> seen;
    for (std::size_t i = 0; i < original.size(); ++i) {
        const fr::Resource& r = original[i];
        if (old_images.used.contains(i)) continue;
        ++into.total;
        const Key key = folded(r.key());
        seen.insert(key);
        const auto twin_at = written_at.find(key);
        if (twin_at == written_at.end()) {
            into.missing.push_back(r.key());
            continue;
        }
        const std::vector<std::size_t>& twins = written[twin_at->second].second;
        if (twins.size() > 1) {
            into.different.push_back(r.key());
            continue;
        }
        const fr::Resource& twin = rebuilt[twins.front()];
        if (const auto old_group = old_images.groups.find(key); old_group != old_images.groups.end()) {
            bool same = false;
            if (const auto new_group = new_images.groups.find(key); new_group != new_images.groups.end()) {
                const auto& [old_kind, old_data] = old_group->second;
                const auto& [new_kind, new_data] = new_group->second;
                same = old_kind == new_kind && old_data.size() == new_data.size();
                for (std::size_t k = 0; same && k < old_data.size(); ++k) same = fr::same_data(*old_data[k], *new_data[k]);
            }
            if (same && r.data == twin.data) ++into.identical;
            else if (same) ++into.equivalent;
            else into.different.push_back(r.key());
        } else if (r.data == twin.data || (!wide && fr::same_data(r.data, twin.data))) {
            ++into.identical;
        } else {
            const std::optional<fr::Decoded> a = fr::decode(r, wide), b = fr::decode(twin, wide);
            if (a && b && same_decoded(*a, *b)) ++into.equivalent;
            else into.different.push_back(r.key());
        }
    }
    into.extra.clear();
    for (const auto& [key, indexes] : written)
        if (!seen.contains(key))
            for (std::size_t i : indexes) into.extra.push_back(rebuilt[i].key());
}

Rebuilt rebuild(const watcom::OpenWatcom& open_watcom, const stdfs::path& path, const stdfs::path& outdir, int rounds) {
    const Module module = read_module(path);
    Rebuilt result;
    if (module.resources.empty()) {
        result.error = "the module has no resources";
        return result;
    }
    std::set<Key> verbatim;
    for (int round = 0; round < rounds; ++round) {
        result = Rebuilt();
        RcFiles rc;
        try {
            rc = write_rc(module, outdir, verbatim);
        } catch (const NoSourceForm& e) {
            result.error = std::string("a resource cannot be written as source: ") + e.what();
            return result;
        }
        result.rc = rc.rc;
        const bool wide = rc.text != Text::Narrow;
        const Compiled compiled = compile_rc(open_watcom, rc.rc, rc.text);
        if (!compiled.res) {
            const std::size_t line = error_line(compiled.log);
            std::set<Key> bad;
            for (const Span& s : rc.spans)
                if (s.first <= line && line <= s.last && !verbatim.contains(s.key)) bad.insert(s.key);
            if (bad.empty()) {
                const std::string log(text::trim(compiled.log));
                const std::string tail = log.size() > 200 ? log.substr(log.size() - 200) : log;
                result.error = "the resource compiler rejected the file: " + (tail.empty() ? std::string("no message") : tail);
                return result;
            }
            verbatim.insert(bad.begin(), bad.end());
            continue;
        }
        result.res = compiled.res;
        const Bytes res_bytes = fs::read_file(*compiled.res);
        compare(module.resources, read_res(res_bytes, wide), wide, result);
        result.as_text = rc.how.text.size();
        result.as_files = rc.how.files.size();
        result.opaque = rc.how.opaque.size();
        result.fallback = rc.how.fallback.size();
        result.not_source = rc.how.fallback;
        std::set<Key> again;
        for (const Key& k : result.different)
            if (!verbatim.contains(k)) again.insert(k);
        if (again.empty()) {
            result.link_res = compile_rc(open_watcom, rc.rc, rc.text, false).res;
            break;
        }
        verbatim.insert(again.begin(), again.end());
    }
    return result;
}

// ---- the command -------------------------------------------------------------------------------------------------------

cli::Command command(const Environment& environment) {
    return cli::Command{
        "resources",
        "write a module's resources as an .rc script and files, compile them back and compare",
        {cli::Positional{"module", "a corpus name or a file", true, true},
         cli::Option{"--out", "", "where the resources go (default: the module's project folder, assets/resources)",
                     cli::Arg::Value, "DIR"}},
        [environment](const cli::Args& args) {
            const std::optional<std::string> out = args.value("out");
            int bad = 0;
            for (const std::string& name : args.values("module")) {
                const std::optional<stdfs::path> path = environment.retail ? environment.retail(name) : std::nullopt;
                if (!path) {
                    cb::out("{}: no such module in the corpus\n", name);
                    ++bad;
                    continue;
                }
                std::optional<store::converted::Project> project;
                stdfs::path where;
                if (out) {
                    where = fs::from_text(*out);
                } else {
                    project = environment.project(*path);
                    where = project->assets() / "resources";
                }
                const Rebuilt r = rebuild(environment.open_watcom, *path, where);
                if (project)
                    project->record("resources", {where}, "its resources as files and a .rc script: " + r.line());
                cb::out("{}: {}{}", text::lower(path->filename().string()), r.line(), kEol);
                std::vector<Key> not_reproduced = r.different;
                not_reproduced.insert(not_reproduced.end(), r.missing.begin(), r.missing.end());
                for (const Key& key : not_reproduced) {
                    const std::string shown = std::holds_alternative<std::uint32_t>(key.name)
                                                 ? std::to_string(std::get<std::uint32_t>(key.name))
                                                 : "'" + std::get<std::string>(key.name) + "'";
                    cb::out("    not reproduced: {} {}{}", fr::type_name(key.type), shown, kEol);
                }
                if (r.rc) cb::out("    {}{}", fs::to_text(*r.rc), kEol);
                if (!r.same()) ++bad;
            }
            return bad != 0 ? 1 : 0;
        },
        {},
        "Build",
        ""};
}

}  // namespace cb::build::resources
