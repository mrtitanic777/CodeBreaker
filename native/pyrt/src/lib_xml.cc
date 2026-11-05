// lib_xml.cc -- pyexpat in C++: the XML parser xml.etree.ElementTree and xml.dom.minidom (CPython's own
// Python, translated with the program: stdlib.txt) call. A small conforming non-validating parser with
// expat's interface: ParserCreate(encoding, namespace_separator, intern), Parse / ParseFile, the handler
// attributes (StartElementHandler ... XmlDeclHandler), buffer_text, ordered_attributes, namespace
// processing (and namespace_prefixes), the internal DTD subset's general entities, expat's error codes,
// messages and positions (pyexpat.errors, ExpatError.code / lineno / offset), pyexpat.model's constants.
// Documents are UTF-8 (a BOM, UTF-16 with a BOM, and ISO-8859-1 / US-ASCII declared). External entities and
// DTD validation are not done (as with expat's default).
#include <cstring>
#include <map>

#include "rt.hpp"

namespace py {

TypeObj* T(const var& v);

namespace {

var nat(const char* n, native_fn f) { return new_native(n, std::move(f)); }
var S(const std::string& s) { return new_str(s); }

const char* const kErrors[] = {
    nullptr, "out of memory", "syntax error", "no element found", "not well-formed (invalid token)", "unclosed token",
    "partial character", "mismatched tag", "duplicate attribute", "junk after document element",
    "illegal parameter entity reference", "undefined entity", "recursive entity reference", "asynchronous entity",
    "reference to invalid character number", "reference to binary entity", "reference to external entity in attribute",
    "XML or text declaration not at start of entity", "unknown encoding", "encoding specified in XML declaration is incorrect",
    "unclosed CDATA section", "error in processing external entity reference", "document is not standalone",
    "unexpected parser state - please send a bug report", "entity declared in parameter entity",
    "requested feature requires XML_DTD support in Expat", "cannot change setting once parsing has begun", "unbound prefix",
    "must not undeclare prefix", "incomplete markup in parameter entity", "XML declaration not well-formed",
    "text declaration not well-formed", "illegal character(s) in public id", "parser suspended", "parser not suspended",
    "parsing aborted", "parsing finished", "cannot suspend in external parameter entity"};
const char* const kErrorNames[] = {
    nullptr, "XML_ERROR_NO_MEMORY", "XML_ERROR_SYNTAX", "XML_ERROR_NO_ELEMENTS", "XML_ERROR_INVALID_TOKEN",
    "XML_ERROR_UNCLOSED_TOKEN", "XML_ERROR_PARTIAL_CHAR", "XML_ERROR_TAG_MISMATCH", "XML_ERROR_DUPLICATE_ATTRIBUTE",
    "XML_ERROR_JUNK_AFTER_DOC_ELEMENT", "XML_ERROR_PARAM_ENTITY_REF", "XML_ERROR_UNDEFINED_ENTITY",
    "XML_ERROR_RECURSIVE_ENTITY_REF", "XML_ERROR_ASYNC_ENTITY", "XML_ERROR_BAD_CHAR_REF", "XML_ERROR_BINARY_ENTITY_REF",
    "XML_ERROR_ATTRIBUTE_EXTERNAL_ENTITY_REF", "XML_ERROR_MISPLACED_XML_PI", "XML_ERROR_UNKNOWN_ENCODING",
    "XML_ERROR_INCORRECT_ENCODING", "XML_ERROR_UNCLOSED_CDATA_SECTION", "XML_ERROR_EXTERNAL_ENTITY_HANDLING",
    "XML_ERROR_NOT_STANDALONE", "XML_ERROR_UNEXPECTED_STATE", "XML_ERROR_ENTITY_DECLARED_IN_PE",
    "XML_ERROR_FEATURE_REQUIRES_XML_DTD", "XML_ERROR_CANT_CHANGE_FEATURE_ONCE_PARSING", "XML_ERROR_UNBOUND_PREFIX",
    "XML_ERROR_UNDECLARING_PREFIX", "XML_ERROR_INCOMPLETE_PE", "XML_ERROR_XML_DECL", "XML_ERROR_TEXT_DECL",
    "XML_ERROR_PUBLICID", "XML_ERROR_SUSPENDED", "XML_ERROR_NOT_SUSPENDED", "XML_ERROR_ABORTED", "XML_ERROR_FINISHED",
    "XML_ERROR_SUSPEND_PE"};
constexpr int kNErrors = (int)(sizeof kErrors / sizeof kErrors[0]);

const char* const kHandlers[] = {
    "StartElementHandler", "EndElementHandler", "ProcessingInstructionHandler", "CharacterDataHandler",
    "UnparsedEntityDeclHandler", "NotationDeclHandler", "StartNamespaceDeclHandler", "EndNamespaceDeclHandler",
    "CommentHandler", "StartCdataSectionHandler", "EndCdataSectionHandler", "DefaultHandler", "DefaultHandlerExpand",
    "NotStandaloneHandler", "ExternalEntityRefHandler", "StartDoctypeDeclHandler", "EndDoctypeDeclHandler",
    "EntityDeclHandler", "XmlDeclHandler", "ElementDeclHandler", "AttlistDeclHandler", "SkippedEntityHandler"};

var g_error, g_parser_type;
TypeObj* T_parser = nullptr;

bool is_name_start(unsigned char c) { return isalpha(c) || c == '_' || c == ':' || c >= 0x80; }
bool is_name_char(unsigned char c) { return isalnum(c) || c == '_' || c == ':' || c == '-' || c == '.' || c >= 0x80; }
bool is_ws(unsigned char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }

void utf8_append(std::string& out, uint32_t c) {
    if (c < 0x80) out += (char)c;
    else if (c < 0x800) { out += (char)(0xC0 | (c >> 6)); out += (char)(0x80 | (c & 63)); }
    else if (c < 0x10000) { out += (char)(0xE0 | (c >> 12)); out += (char)(0x80 | ((c >> 6) & 63)); out += (char)(0x80 | (c & 63)); }
    else { out += (char)(0xF0 | (c >> 18)); out += (char)(0x80 | ((c >> 12) & 63)); out += (char)(0x80 | ((c >> 6) & 63)); out += (char)(0x80 | (c & 63)); }
}

struct QName { std::string raw, prefix, local; };

struct XP : DataObj {
    bool ns = false, prefixes = false;
    std::string sep;
    std::string buf;                       // the document so far, as UTF-8
    std::string raw16;                     // UTF-16 bytes not yet converted
    size_t pos = 0;
    int enc = 0;                           // 0 utf-8, 1 utf-16le, 2 utf-16be, 3 latin-1
    bool enc_known = false, forced_enc = false, str_input = false;
    bool finished = false, failed = false, deferral = true;
    int state = 0;                         // 0 prolog, 1 content, 2 epilog
    bool seen_doctype = false, external_dtd = false;
    std::string head;                      // the first bytes, until the XML declaration is whole
    std::string text;                      // buffered character data
    struct Open { std::string raw, reported; std::vector<std::string> prefixes; };
    std::vector<Open> stack;
    std::vector<std::vector<std::pair<std::string, std::string>>> scopes;   // prefix -> uri per element
    std::map<std::string, std::string> entities;
    std::string base;
    size_t ev_off = 0;
    int err_code = 0;
    int64_t err_line = 1, err_col = 0;

    var self() { return var(static_cast<Obj*>(this)); }
    var handler(const char* n) { var h = self().getattr(n); return h; }
    bool flag(const char* n) { return self().getattr(n).truthy(); }

    void position(size_t off, int64_t& line, int64_t& col) const {
        line = 1;
        size_t ls = 0;
        off = std::min(off, buf.size());
        for (size_t i = 0; i < off; i++) {
            if (buf[i] == '\n') { line++; ls = i + 1; }
            else if (buf[i] == '\r') { if (i + 1 < buf.size() && buf[i + 1] == '\n') { if (i + 1 < off) { i++; line++; ls = i + 1; } else { line++; ls = i + 1; } } else { line++; ls = i + 1; } }
        }
        col = (int64_t)(off - ls);
    }
    [[noreturn]] void fail(int code, size_t off) {
        failed = true;
        err_code = code;
        position(off, err_line, err_col);
        std::string msg = std::string(kErrors[code]) + ": line " + std::to_string(err_line) + ", column " + std::to_string(err_col);
        var e = make_exc_args(T(g_error), {S(msg)});
        e.setattr("code", var((int64_t)code));
        e.setattr("lineno", var(err_line));
        e.setattr("offset", var(err_col));
        throw error(e);
    }

    // ---- events
    void flush_text() {
        if (text.empty()) return;
        std::string t;
        t.swap(text);
        var h = handler("CharacterDataHandler");
        if (!h.is_none()) h(S(t));
    }
    void emit_text(const std::string& piece) {
        if (piece.empty()) return;
        if (flag("buffer_text")) { text += piece; return; }
        var h = handler("CharacterDataHandler");
        if (!h.is_none()) h(S(piece));
    }
    // character data with expat's unbuffered splitting at newlines
    void emit_chars(const std::string& s) {
        if (s.empty()) return;
        if (flag("buffer_text")) { text += s; return; }
        size_t a = 0;
        while (a < s.size()) {
            size_t nl = s.find('\n', a);
            if (nl == a) { emit_text("\n"); a++; continue; }
            size_t e = nl == std::string::npos ? s.size() : nl;
            emit_text(s.substr(a, e - a));
            a = e;
        }
    }
    bool default_handler(const std::string& s) {
        var h = handler("DefaultHandlerExpand");
        if (h.is_none()) h = handler("DefaultHandler");
        if (h.is_none()) return false;
        flush_text();
        h(S(s));
        return true;
    }

    // ---- references: &name; &#N; &#xN; at buf[q] == '&'. Appends the replacement to out; q moves past ';'.
    // returns 0 done, 1 incomplete (need more input), 2 undefined entity (name in undef)
    int reference(size_t& q, std::string& out, bool in_attr, std::string& undef, int depth = 0) {
        size_t start = q;
        size_t i = q + 1;
        if (i >= buf.size()) return 1;
        if (buf[i] == '#') {
            i++;
            bool hex = false;
            if (i < buf.size() && buf[i] == 'x') { hex = true; i++; }
            size_t d = i;
            uint64_t v = 0;
            while (i < buf.size() && (hex ? isxdigit((unsigned char)buf[i]) : isdigit((unsigned char)buf[i]))) {
                v = v * (hex ? 16 : 10) + (isdigit((unsigned char)buf[i]) ? buf[i] - '0' : (tolower(buf[i]) - 'a' + 10));
                if (v > 0x10FFFF) v = 0x110000;
                i++;
            }
            if (i >= buf.size()) return 1;
            if (buf[i] != ';' || i == d) fail(4, start);
            bool ok = v == 9 || v == 10 || v == 13 || (v >= 0x20 && v <= 0xD7FF) || (v >= 0xE000 && v <= 0xFFFD && v != 0xFFFE && v != 0xFFFF) || (v >= 0x10000 && v <= 0x10FFFF);
            if (!ok) fail(14, start);
            utf8_append(out, (uint32_t)v);
            q = i + 1;
            return 0;
        }
        if (!is_name_start((unsigned char)buf[i])) fail(4, i);
        size_t n0 = i;
        while (i < buf.size() && is_name_char((unsigned char)buf[i])) i++;
        if (i >= buf.size()) return 1;
        if (buf[i] != ';') fail(4, start);
        std::string name = buf.substr(n0, i - n0);
        q = i + 1;
        if (name == "lt") out += '<';
        else if (name == "gt") out += '>';
        else if (name == "amp") out += '&';
        else if (name == "apos") out += '\'';
        else if (name == "quot") out += '"';
        else {
            auto it = entities.find(name);
            if (it == entities.end()) { undef = name; return 2; }
            // expat leaves internal entities unexpanded for a plain DefaultHandler (the reference goes to it)
            if (!in_attr && depth == 0 && !handler("DefaultHandler").is_none() && handler("DefaultHandlerExpand").is_none()) { undef = name; return 3; }
            if (depth > 20) fail(12, start);
            expand_value(it->second, out, in_attr, start, depth + 1);
        }
        return 0;
    }
    // an entity's replacement text: char refs and nested entities expanded; markup in it is taken as text
    void expand_value(const std::string& v, std::string& out, bool in_attr, size_t at, int depth) {
        for (size_t i = 0; i < v.size();) {
            if (v[i] == '&') {
                size_t e = v.find(';', i);
                if (e == std::string::npos) fail(4, at);
                std::string nm = v.substr(i + 1, e - i - 1);
                if (!nm.empty() && nm[0] == '#') {
                    uint64_t cp = nm.size() > 1 && nm[1] == 'x' ? strtoull(nm.c_str() + 2, nullptr, 16) : strtoull(nm.c_str() + 1, nullptr, 10);
                    utf8_append(out, (uint32_t)cp);
                } else if (nm == "lt") out += '<';
                else if (nm == "gt") out += '>';
                else if (nm == "amp") out += '&';
                else if (nm == "apos") out += '\'';
                else if (nm == "quot") out += '"';
                else {
                    auto it = entities.find(nm);
                    if (it == entities.end()) fail(11, at);
                    if (depth > 20) fail(12, at);
                    expand_value(it->second, out, in_attr, at, depth + 1);
                }
                i = e + 1;
            } else out += v[i++];
        }
    }

    // ---- scanning helpers
    size_t skip_ws(size_t p) const { while (p < buf.size() && is_ws((unsigned char)buf[p])) p++; return p; }
    // a name at p: returns its end (p if none)
    size_t scan_name(size_t p) const {
        if (p >= buf.size() || !is_name_start((unsigned char)buf[p])) return p;
        size_t q = p + 1;
        while (q < buf.size() && is_name_char((unsigned char)buf[q])) q++;
        return q;
    }
    // the end of a start / end tag (position of its '>'), or npos when more input is needed
    size_t tag_end(size_t p) const {
        char quote = 0;
        for (size_t q = p; q < buf.size(); q++) {
            char c = buf[q];
            if (quote) { if (c == quote) quote = 0; }
            else if (c == '"' || c == '\'') quote = c;
            else if (c == '>') return q;
        }
        return std::string::npos;
    }
    static QName split(const std::string& raw) {
        QName q;
        q.raw = raw;
        size_t c = raw.find(':');
        if (c == std::string::npos) q.local = raw;
        else { q.prefix = raw.substr(0, c); q.local = raw.substr(c + 1); }
        return q;
    }
    const std::string* lookup_prefix(const std::string& p) const {
        for (size_t i = scopes.size(); i-- > 0;)
            for (size_t j = scopes[i].size(); j-- > 0;)
                if (scopes[i][j].first == p) return &scopes[i][j].second;
        return nullptr;
    }
    std::string expand_name(const QName& q, bool is_attr, size_t at) const {
        if (!ns) return q.raw;
        if (q.prefix.empty()) {
            if (is_attr) return q.raw;
            const std::string* u = lookup_prefix("");
            if (!u || u->empty()) return q.raw;
            return *u + sep + q.local;
        }
        const std::string* u;
        static const std::string xmluri = "http://www.w3.org/XML/1998/namespace";
        if (q.prefix == "xml") u = &xmluri;
        else u = lookup_prefix(q.prefix);
        if (!u) const_cast<XP*>(this)->fail(27, at);
        return *u + sep + q.local + (const_cast<XP*>(this)->flag("namespace_prefixes") ? sep + q.prefix : "");
    }

    void start_tag(size_t p, size_t e, bool empty) {
        // buf[p] == '<'; e = position of '>' (or the '/' of "/>" when empty)
        size_t q = p + 1;
        size_t ne = scan_name(q);
        if (ne == q) fail(4, q);
        std::string name = buf.substr(q, ne - q);
        std::vector<std::pair<std::string, std::string>> attrs;
        q = ne;
        for (;;) {
            size_t w = skip_ws(q);
            if (w >= e) { q = w; break; }
            if (w == q) fail(4, q);                     // attributes need whitespace between them
            size_t an = scan_name(w);
            if (an == w) fail(4, w);
            std::string aname = buf.substr(w, an - w);
            size_t eq = skip_ws(an);
            if (eq >= e || buf[eq] != '=') fail(4, eq < e ? eq : e);
            size_t vq = skip_ws(eq + 1);
            if (vq >= e || (buf[vq] != '"' && buf[vq] != '\'')) fail(4, vq < e ? vq : e);
            char quote = buf[vq];
            size_t ve = buf.find(quote, vq + 1);
            if (ve == std::string::npos || ve >= e) fail(4, vq);
            std::string val;
            for (size_t i = vq + 1; i < ve;) {
                unsigned char c = (unsigned char)buf[i];
                if (c == '<') fail(4, i);
                if (c == '&') {
                    std::string undef;
                    size_t r = i;
                    int rc = reference(r, val, true, undef);
                    if (rc == 1) fail(4, i);
                    if (rc == 2) fail(11, i);
                    i = r;
                } else if (c == '\r') { val += ' '; i += (i + 1 < ve && buf[i + 1] == '\n') ? 2 : 1; }
                else if (c == '\n' || c == '\t') { val += ' '; i++; }
                else { val += (char)c; i++; }
            }
            for (auto& a : attrs) if (a.first == aname) fail(8, w);
            attrs.emplace_back(aname, val);
            q = ve + 1;
        }
        if (state == 2) fail(9, p);
        if (state == 0) state = 1;
        flush_text();
        Open open;
        open.raw = name;
        std::vector<std::pair<std::string, std::string>> declared;
        std::vector<std::pair<std::string, std::string>> kept;
        if (ns) {
            for (auto& a : attrs) {
                if (a.first == "xmlns") declared.emplace_back("", a.second);
                else if (a.first.compare(0, 6, "xmlns:") == 0) {
                    if (a.second.empty()) fail(28, p);
                    declared.emplace_back(a.first.substr(6), a.second);
                } else kept.push_back(a);
            }
            scopes.push_back(declared);
            for (auto& d : declared) {
                var h = handler("StartNamespaceDeclHandler");
                open.prefixes.push_back(d.first);
                if (!h.is_none()) h(d.first.empty() ? var(None) : S(d.first), d.second.empty() ? var(None) : S(d.second));
            }
        } else kept = attrs;
        open.reported = expand_name(split(name), false, p);
        var h = handler("StartElementHandler");
        if (!h.is_none()) {
            var av;
            if (flag("ordered_attributes")) {
                std::vector<var> l;
                for (auto& a : kept) { l.push_back(S(expand_name(split(a.first), true, p + 1))); l.push_back(S(a.second)); }
                av = new_list(l);
            } else {
                av = new_dict_obj();
                for (auto& a : kept) av.setitem(S(expand_name(split(a.first), true, p + 1)), S(a.second));
            }
            h(S(open.reported), av);
        }
        if (empty) {
            var he = handler("EndElementHandler");
            if (!he.is_none()) he(S(open.reported));
            end_scope(open);
            if (stack.empty()) state = 2;
        } else stack.push_back(open);
    }
    void end_scope(const Open& o) {
        if (!ns) return;
        scopes.pop_back();
        for (size_t i = o.prefixes.size(); i-- > 0;) {
            var h = handler("EndNamespaceDeclHandler");
            if (!h.is_none()) h(o.prefixes[i].empty() ? var(None) : S(o.prefixes[i]));
        }
    }
    void end_tag(size_t p, size_t e) {
        size_t q = p + 2, ne = scan_name(q);
        if (ne == q) fail(4, q);
        std::string name = buf.substr(q, ne - q);
        size_t w = skip_ws(ne);
        if (w != e) fail(4, w);
        if (stack.empty()) { if (state == 0) fail(4, p + 1); fail(9, p); }
        if (stack.back().raw != name) fail(7, q);
        flush_text();
        Open o = stack.back();
        stack.pop_back();
        var h = handler("EndElementHandler");
        if (!h.is_none()) h(S(o.reported));
        end_scope(o);
        if (stack.empty()) state = 2;
    }

    void xml_decl(size_t p, size_t e) {
        // <?xml ... ?> at the very start
        std::string d = buf.substr(p + 5, e - p - 5);
        auto attr = [&](const char* n, std::string& out) -> bool {
            size_t i = d.find(n);
            if (i == std::string::npos) return false;
            size_t eq = d.find('=', i);
            if (eq == std::string::npos) fail(30, p);
            size_t q = d.find_first_of("\"'", eq);
            if (q == std::string::npos) fail(30, p);
            size_t qe = d.find(d[q], q + 1);
            if (qe == std::string::npos) fail(30, p);
            out = d.substr(q + 1, qe - q - 1);
            return true;
        };
        std::string version, encoding, standalone;
        if (!attr("version", version)) fail(30, p);
        bool has_enc = attr("encoding", encoding), has_sa = attr("standalone", standalone);
        var h = handler("XmlDeclHandler");
        if (!h.is_none()) {
            flush_text();
            h(S(version), has_enc ? S(encoding) : var(None), var((int64_t)(has_sa ? (standalone == "yes" ? 1 : 0) : -1)));
        } else default_handler(buf.substr(p, e + 2 - p));
    }

    // the internal subset: general internal entities; the rest is skipped
    void internal_subset(size_t a, size_t b) {
        size_t i = a;
        while (i < b) {
            if (buf.compare(i, 4, "<!--") == 0) { size_t e = buf.find("-->", i + 4); i = e == std::string::npos ? b : e + 3; continue; }
            if (buf.compare(i, 2, "<?") == 0) { size_t e = buf.find("?>", i + 2); i = e == std::string::npos ? b : e + 2; continue; }
            if (buf.compare(i, 8, "<!ENTITY") == 0) {
                size_t j = skip_ws(i + 8);
                bool param = false;
                if (j < b && buf[j] == '%') { param = true; j = skip_ws(j + 1); }
                size_t ne = scan_name(j);
                std::string nm = buf.substr(j, ne - j);
                j = skip_ws(ne);
                size_t end = tag_end(j);
                if (end == std::string::npos || end > b) end = b;
                if (j < end && (buf[j] == '"' || buf[j] == '\'')) {
                    size_t qe = buf.find(buf[j], j + 1);
                    std::string val = buf.substr(j + 1, qe - j - 1);
                    if (!param && !entities.count(nm)) entities[nm] = val;
                    var h = handler("EntityDeclHandler");
                    if (!h.is_none()) {
                        h(S(nm), var((int64_t)param), S(val), base.empty() ? var(None) : S(base), var(None), var(None), var(None));
                        i = end + 1;
                        continue;
                    }
                }
                // (no handler for the declaration: it reaches the default handler in pieces, below)
            }
            if (buf.compare(i, 2, "<!") == 0) {
                char quote = 0;
                size_t j = i + 2;
                for (; j < b; j++) {
                    char c = buf[j];
                    if (quote) { if (c == quote) quote = 0; }
                    else if (c == '"' || c == '\'') quote = c;
                    else if (c == '>') break;
                }
                // an element / attribute-list declaration, with no handler for it, reaches the default handler in pieces
                var dh = handler("DefaultHandlerExpand");
                if (dh.is_none()) dh = handler("DefaultHandler");
                if (!dh.is_none()) {
                    size_t k = i + 2;
                    while (k < buf.size() && is_name_char((unsigned char)buf[k])) k++;
                    dh(S(buf.substr(i, k - i)));
                    while (k < j) {
                        size_t m = k;
                        if (is_ws((unsigned char)buf[k])) { while (m < j && is_ws((unsigned char)buf[m])) m++; }
                        else if (buf[k] == '"' || buf[k] == '\'') { m = buf.find(buf[k], k + 1); m = (m == std::string::npos || m >= j) ? j : m + 1; }
                        else { while (m < j && !is_ws((unsigned char)buf[m]) && buf[m] != '"' && buf[m] != '\'') m++; }
                        dh(S(buf.substr(k, m - k)));
                        k = m;
                    }
                    dh(S(">"));
                }
                i = j + 1;
                continue;
            }
            i++;
        }
    }
    // <!DOCTYPE ...> from p; returns its end (past '>'), or npos when more input is needed
    size_t doctype(size_t p) {
        size_t q = p + 9;
        char quote = 0;
        int depth = 0;
        size_t sub_a = 0, sub_b = 0, e = std::string::npos;
        for (size_t i = q; i < buf.size(); i++) {
            char c = buf[i];
            if (quote) { if (c == quote) quote = 0; continue; }
            if (depth > 0) {
                if (c == '"' || c == '\'') quote = c;
                else if (buf.compare(i, 4, "<!--") == 0) { size_t ce = buf.find("-->", i + 4); if (ce == std::string::npos) return std::string::npos; i = ce + 2; }
                else if (c == ']') { depth = 0; sub_b = i; }
                continue;
            }
            if (c == '"' || c == '\'') quote = c;
            else if (c == '[') { depth = 1; sub_a = i + 1; }
            else if (c == '>') { e = i; break; }
        }
        if (e == std::string::npos) return e;
        // name, external id
        size_t j = skip_ws(q);
        size_t ne = scan_name(j);
        if (ne == j) fail(4, j);
        std::string name = buf.substr(j, ne - j), sysid, pubid;
        bool has_sys = false, has_pub = false;
        j = skip_ws(ne);
        auto literal = [&](std::string& out) {
            j = skip_ws(j);
            if (j >= e || (buf[j] != '"' && buf[j] != '\'')) fail(4, j);
            size_t qe = buf.find(buf[j], j + 1);
            out = buf.substr(j + 1, qe - j - 1);
            j = qe + 1;
        };
        if (buf.compare(j, 6, "SYSTEM") == 0) { j += 6; literal(sysid); has_sys = true; }
        else if (buf.compare(j, 6, "PUBLIC") == 0) { j += 6; literal(pubid); has_pub = true; literal(sysid); has_sys = true; }
        bool has_sub = sub_a != 0;
        external_dtd = has_sys;
        var sh = handler("StartDoctypeDeclHandler");
        flush_text();
        if (!sh.is_none()) sh(S(name), has_sys ? S(sysid) : var(None), has_pub ? S(pubid) : var(None), var((int64_t)has_sub));
        else {
            var dh = handler("DefaultHandlerExpand");
            if (dh.is_none()) dh = handler("DefaultHandler");
            if (!dh.is_none()) {
                dh(S("<!DOCTYPE"));
                dh(S(" "));
                dh(S(name));
                if (has_pub) { dh(S(" ")); dh(S("PUBLIC")); dh(S(" ")); dh(S("\"" + pubid + "\"")); }
                if (has_sys) { dh(S(" ")); if (!has_pub) { dh(S("SYSTEM")); dh(S(" ")); } dh(S("\"" + sysid + "\"")); }
            }
        }
        if (has_sub) internal_subset(sub_a, sub_b);
        var eh = handler("EndDoctypeDeclHandler");
        if (!eh.is_none()) eh();
        else if (sh.is_none()) default_handler(">");
        return e + 1;
    }

    // ---- the main loop: consume every complete token
    void run(bool final) {
        while (pos < buf.size()) {
            size_t p = pos;
            ev_off = p;
            if (buf[p] == '<') {
                size_t avail = buf.size() - p;
                auto need = [&](size_t n) { return avail < n; };
                if (need(2)) { if (final) fail(5, p); return; }
                char c1 = buf[p + 1];
                if (c1 == '?') {
                    if (avail > 2 && !is_name_start((unsigned char)buf[p + 2])) fail(4, p + 2);
                    size_t e = buf.find("?>", p + 2);
                    if (e == std::string::npos) { if (final) fail(5, p); return; }
                    size_t ne = scan_name(p + 2);
                    std::string target = buf.substr(p + 2, ne - p - 2);
                    if (ne == p + 2) fail(4, p + 2);
                    if (target.size() == 3 && tolower(target[0]) == 'x' && tolower(target[1]) == 'm' && tolower(target[2]) == 'l') {
                        if (p != bom_skip || target != "xml") fail(17, p);
                        if (ne < buf.size() && !is_ws((unsigned char)buf[ne])) fail(30, p);
                        xml_decl(p, e);
                    } else {
                        size_t ds = skip_ws(ne);
                        if (ds == ne && ne != e) fail(4, ne);
                        std::string data = buf.substr(ds, e - ds);
                        std::string norm;
                        for (size_t i = 0; i < data.size(); i++) { if (data[i] == '\r') { norm += '\n'; if (i + 1 < data.size() && data[i + 1] == '\n') i++; } else norm += data[i]; }
                        var h = handler("ProcessingInstructionHandler");
                        if (!h.is_none()) { flush_text(); h(S(target), S(norm)); }
                        else default_handler(buf.substr(p, e + 2 - p));
                    }
                    pos = e + 2;
                } else if (c1 == '!') {
                    if (need(4)) { if (final) fail(5, p); return; }
                    if (buf.compare(p, 4, "<!--") == 0) {
                        size_t e = buf.find("-->", p + 4);
                        if (e == std::string::npos) { if (final) fail(5, p); return; }
                        std::string body = buf.substr(p + 4, e - p - 4);
                        size_t dd = body.find("--");
                        if (dd != std::string::npos) fail(4, p + 4 + dd + 2);
                        if (!body.empty() && body.back() == '-') fail(4, e - 1);
                        std::string norm;
                        for (size_t i = 0; i < body.size(); i++) { if (body[i] == '\r') { norm += '\n'; if (i + 1 < body.size() && body[i + 1] == '\n') i++; } else norm += body[i]; }
                        if (state == 1 || true) {
                            var h = handler("CommentHandler");
                            if (!h.is_none()) { flush_text(); h(S(norm)); }
                            else default_handler(buf.substr(p, e + 3 - p));
                        }
                        pos = e + 3;
                    } else if (buf.compare(p, 9, "<![CDATA[") == 0 || (avail < 9 && std::string("<![CDATA[").compare(0, avail, buf, p, avail) == 0)) {
                        if (avail < 9) { if (final) fail(5, p); return; }
                        if (state != 1) fail(state == 2 ? 9 : 2, p);
                        size_t e = buf.find("]]>", p + 9);
                        bool unclosed = e == std::string::npos;
                        if (unclosed && !final) return;
                        if (unclosed) e = buf.size();
                        var hs = handler("StartCdataSectionHandler"), he = handler("EndCdataSectionHandler");
                        flush_text();
                        if (!hs.is_none()) hs();
                        std::string norm;
                        for (size_t i = p + 9; i < e; i++) { if (buf[i] == '\r') { norm += '\n'; if (i + 1 < e && buf[i + 1] == '\n') i++; } else norm += buf[i]; }
                        emit_chars(norm);
                        if (unclosed) fail(20, buf.size());
                        flush_text();
                        if (!he.is_none()) he();
                        pos = e + 3;
                    } else if (buf.compare(p, 9, "<!DOCTYPE") == 0 || (avail < 9 && std::string("<!DOCTYPE").compare(0, avail, buf, p, avail) == 0)) {
                        if (avail < 9) { if (final) fail(5, p); return; }
                        if (state != 0 || seen_doctype) fail(2, p);
                        size_t e = doctype(p);
                        if (e == std::string::npos) { if (final) fail(5, p); return; }
                        seen_doctype = true;
                        pos = e;
                    } else fail(4, p);
                } else if (c1 == '/') {
                    size_t e = tag_end(p + 2);
                    if (e == std::string::npos) { if (final) fail(5, p); return; }
                    end_tag(p, e);
                    pos = e + 1;
                } else {
                    if (!is_name_start((unsigned char)c1)) fail(4, p + 1);
                    size_t e = tag_end(p + 1);
                    if (e == std::string::npos) { if (final) fail(5, p); return; }
                    bool empty = buf[e - 1] == '/';
                    start_tag(p, empty ? e - 1 : e, empty);
                    pos = e + 1;
                }
                continue;
            }
            // character data
            if (state != 1) {
                size_t q = p;
                while (q < buf.size() && buf[q] != '<') q++;
                for (size_t i = p; i < q; i++)
                    if (!is_ws((unsigned char)buf[i])) { if (state == 2) fail(9, i); fail(4, q); }
                if (q > p) {   // (white space outside the root element goes to the default handler, as expat's)
                    std::string normal;
                    for (size_t i = p; i < q; i++) {
                        if (buf[i] == '\r') { normal += '\n'; if (i + 1 < q && buf[i + 1] == '\n') i++; }
                        else normal += buf[i];
                    }
                    default_handler(normal);
                }
                pos = q;
                if (q == buf.size()) return;
                continue;
            }
            std::string cur;
            size_t q = p;
            bool stop = false;
            try {   // (the text before a malformed token has been delivered when the error is raised)
            while (q < buf.size() && !stop) {
                unsigned char c = (unsigned char)buf[q];
                if (c == '<') break;
                if (c == '&') {
                    std::string undef;
                    size_t r = q;
                    std::string rep;
                    int rc = reference(r, rep, false, undef);
                    if (rc == 1) { if (final) fail(5, q); stop = true; break; }
                    if (rc == 2 || rc == 3) {
                        emit_chars(cur); cur.clear();
                        // an undefined entity is an error unless the DTD may have declared it elsewhere (an external subset)
                        if (rc == 2 && !external_dtd) fail(11, q);
                        default_handler("&" + undef + ";");
                        q = r;
                        continue;
                    }
                    emit_chars(cur); cur.clear();
                    // a replacement is delivered apart from the text around it
                    if (flag("buffer_text")) text += rep; else emit_text(rep);
                    q = r;
                    continue;
                }
                if (c == ']' && buf.compare(q, 3, "]]>") == 0) fail(4, q + 2);
                if (c == ']' && q + 2 >= buf.size() && !final) { stop = true; break; }
                if (c == '\r') {
                    if (q + 1 >= buf.size() && !final) { stop = true; break; }
                    cur += '\n';
                    q += (q + 1 < buf.size() && buf[q + 1] == '\n') ? 2 : 1;
                    continue;
                }
                if (c < 0x20 && c != '\t' && c != '\n') fail(4, q);
                if (c >= 0x80) {
                    int n = c >= 0xF0 ? 4 : c >= 0xE0 ? 3 : c >= 0xC0 ? 2 : 1;
                    if (n == 1) fail(4, q);
                    if (q + n > buf.size()) { if (final) fail(6, q); stop = true; break; }
                    cur.append(buf, q, n);
                    q += n;
                    continue;
                }
                cur += (char)c;
                q++;
            }
            } catch (error&) { emit_chars(cur); throw; }
            emit_chars(cur);
            pos = q;
            if (stop) return;
        }
    }
    size_t bom_skip = 0;

    // ---- input: bytes in, UTF-8 into buf
    void feed(const std::string& data, bool final) {
        if (finished) { failed = true; err_code = 36; raise(T(g_error), "parsing finished: line 1, column 0"); }
        std::string in = data;
        if (!enc_known) {   // the declaration (which may name the encoding) is wanted whole before the first byte is read
            head += in;
            in.swap(head);
            head.clear();
            bool maybe_decl = in.size() >= 5 ? in.compare(0, 5, "<?xml") == 0 : std::string("<?xml").compare(0, in.size(), in) == 0;
            if (!final && maybe_decl && in.size() < 200 && in.find("?>") == std::string::npos) { head.swap(in); return; }
        }
        if (!enc_known && !in.empty()) {
            enc_known = true;
            size_t off = 0;
            if (!str_input) {
                if (in.size() >= 3 && (uint8_t)in[0] == 0xEF && (uint8_t)in[1] == 0xBB && (uint8_t)in[2] == 0xBF) { off = 3; }
                else if (in.size() >= 2 && (uint8_t)in[0] == 0xFF && (uint8_t)in[1] == 0xFE) { enc = 1; off = 2; }
                else if (in.size() >= 2 && (uint8_t)in[0] == 0xFE && (uint8_t)in[1] == 0xFF) { enc = 2; off = 2; }
                else if (in.size() >= 2 && in[0] == '<' && in[1] == 0) enc = 1;
                else if (in.size() >= 2 && in[0] == 0 && in[1] == '<') enc = 2;
                if (enc == 0 && !forced_enc) {
                    size_t d = in.find("encoding");
                    if (in.compare(0, 5, "<?xml") == 0 && d != std::string::npos && d < 100) {
                        size_t q = in.find_first_of("\"'", d);
                        if (q != std::string::npos) {
                            size_t qe = in.find(in[q], q + 1);
                            std::string e = qe == std::string::npos ? "" : in.substr(q + 1, qe - q - 1);
                            for (auto& ch : e) ch = (char)tolower((unsigned char)ch);
                            if (e == "iso-8859-1" || e == "latin-1" || e == "latin1" || e == "iso8859-1" || e == "windows-1252" || e == "cp1252") enc = 3;
                            else if (e == "utf-16" || e == "utf-16le") enc = 1;
                            else if (e == "utf-16be") enc = 2;
                            else if (e != "utf-8" && e != "utf8" && e != "us-ascii" && e != "ascii" && !e.empty()) fail_unknown_encoding();
                        }
                    }
                }
            }
            bom_skip = 0;
            in.erase(0, off);
        }
        if (enc == 1 || enc == 2) {
            raw16 += in;
            size_t n = raw16.size() / 2 * 2;
            std::string out;
            for (size_t i = 0; i + 1 < n; i += 2) {
                uint32_t u = enc == 1 ? ((uint8_t)raw16[i] | ((uint8_t)raw16[i + 1] << 8)) : (((uint8_t)raw16[i] << 8) | (uint8_t)raw16[i + 1]);
                if (u >= 0xD800 && u < 0xDC00) {
                    if (i + 3 >= n) { n = i; break; }
                    uint32_t u2 = enc == 1 ? ((uint8_t)raw16[i + 2] | ((uint8_t)raw16[i + 3] << 8)) : (((uint8_t)raw16[i + 2] << 8) | (uint8_t)raw16[i + 3]);
                    u = 0x10000 + ((u - 0xD800) << 10) + (u2 - 0xDC00);
                    i += 2;
                }
                utf8_append(out, u);
            }
            raw16.erase(0, n);
            buf += out;
        } else if (enc == 3) {
            for (unsigned char c : in) utf8_append(buf, c);
        } else buf += in;
        if (final) finished_input = true;
        try {
            run(final);
            if (final) {
                if (pos < buf.size()) fail(5, pos);
                if (state != 2 || !stack.empty()) fail(3, buf.size());
                flush_text();
                finished = true;
            }
        } catch (...) {
            text.clear();
            throw;
        }
    }
    bool finished_input = false;
    [[noreturn]] void fail_unknown_encoding() {
        failed = true;
        err_code = 18;
        err_line = 1; err_col = 0;
        var e = make_exc_args(T(g_error), {S("unknown encoding: line 1, column 0")});
        e.setattr("code", var((int64_t)18));
        e.setattr("lineno", var((int64_t)1));
        e.setattr("offset", var((int64_t)0));
        throw error(e);
    }
};

XP* P(const var& v) { return static_cast<XP*>(v.ref()); }

var new_parser(const var& encoding, const var& sep, const var& intern) {
    auto* x = new XP();
    x->type = T_parser;
    var self(static_cast<Obj*>(x));
    if (!sep.is_none()) {
        x->ns = true;
        x->sep = str_utf8(sep);
        if (x->sep.size() > 1) raise(E_ValueError, "namespace_separator must be at most one character, omitted, or None");
    }
    if (!encoding.is_none()) {
        std::string e = str_utf8(encoding);
        for (auto& c : e) c = (char)tolower((unsigned char)c);
        x->forced_enc = true;
        if (e == "iso-8859-1" || e == "latin-1") x->enc = 3;
        else if (e == "utf-16" || e == "utf-16-le") x->enc = 1;
        else if (e == "utf-16-be") x->enc = 2;
        else if (e != "utf-8" && e != "us-ascii" && e != "ascii" && e != "utf8") x->fail_unknown_encoding();
    }
    for (const char* h : kHandlers) self.setattr(h, None);
    self.setattr("buffer_text", false);
    self.setattr("ordered_attributes", false);
    self.setattr("specified_attributes", false);
    self.setattr("namespace_prefixes", false);
    self.setattr("returns_unicode", true);
    self.setattr("buffer_size", var((int64_t)8192));
    self.setattr("intern", intern.is_none() ? new_dict_obj() : intern);
    return self;
}

void init_pyexpat(const var& m) {
    g_error = make_class("ExpatError", "ExpatError", {t::Exception}, new_dict(), {}, "xml.parsers.expat");
    mod_set(m, "ExpatError", g_error);
    mod_set(m, "error", g_error);
    T_parser = new_type("xmlparser", {}, Layout::NativeData, true, "pyexpat");
    g_parser_type = tv(T_parser);
    mod_set(m, "XMLParserType", g_parser_type);
    mod_set(m, "version_info", new_tuple({var(2), var(7), var(1)}));
    mod_set(m, "EXPAT_VERSION", S("expat_2.7.1"));
    mod_set(m, "native_encoding", S("UTF-8"));
    mod_set(m, "XML_PARAM_ENTITY_PARSING_NEVER", var(0));
    mod_set(m, "XML_PARAM_ENTITY_PARSING_UNLESS_STANDALONE", var(1));
    mod_set(m, "XML_PARAM_ENTITY_PARSING_ALWAYS", var(2));
    mod_set(m, "features", new_list({new_tuple({S("sizeof(XML_Char)"), var(1)}), new_tuple({S("sizeof(XML_LChar)"), var(1)}),
                                     new_tuple({S("XML_DTD"), var(0)}), new_tuple({S("XML_CONTEXT_BYTES"), var(1024)}),
                                     new_tuple({S("XML_NS"), var(0)})}));
    mod_def(m, "ErrorString", [](const CallArgs& a) -> var {
        int64_t c = a.pos.at(0).as_int64();
        if (c <= 0 || c >= kNErrors) return None;
        return S(kErrors[c]);
    });
    mod_def(m, "ParserCreate", [](const CallArgs& a) {
        var enc = arg(a, 0, "encoding", None), sep = arg(a, 1, "namespace_separator", None), intern = arg(a, 2, "intern", None);
        return new_parser(enc, sep, intern);
    });
    // errors and model submodules (xml.parsers.expat imports them from here)
    mod_set(m, "errors", import("pyexpat.errors"));
    mod_set(m, "model", import("pyexpat.model"));

    add_method(T_parser, "Parse", [](const CallArgs& a) {
        XP* x = P(a.pos.at(0));
        var data = arg(a, 1, "data");
        bool final = arg(a, 2, "isfinal", var(0)).truthy();
        if (is_str(data)) { x->str_input = true; x->feed(str_utf8(data), final); }
        else x->feed(raw_of(data), final);
        return var(1);
    });
    add_method(T_parser, "ParseFile", [](const CallArgs& a) {
        XP* x = P(a.pos.at(0));
        var f = a.pos.at(1);
        for (;;) {
            var chunk = f.call("read", var(65536));
            const std::string& s = is_str(chunk) ? str_utf8(chunk) : raw_of(chunk);
            if (is_str(chunk)) x->str_input = true;
            if (s.empty()) break;
            x->feed(s, false);
        }
        x->feed("", true);
        return var(1);
    });
    add_method(T_parser, "SetBase", [](const CallArgs& a) { P(a.pos.at(0))->base = str_utf8(a.pos.at(1)); return None; });
    add_method(T_parser, "GetBase", [](const CallArgs& a) -> var { XP* x = P(a.pos.at(0)); return x->base.empty() ? var(None) : S(x->base); });
    add_method(T_parser, "GetInputContext", [](const CallArgs&) { return var(None); });
    add_method(T_parser, "UseForeignDTD", [](const CallArgs&) { return None; });
    add_method(T_parser, "SetParamEntityParsing", [](const CallArgs&) { return var(0); });
    add_method(T_parser, "GetReparseDeferralEnabled", [](const CallArgs& a) { return var(P(a.pos.at(0))->deferral); });
    add_method(T_parser, "SetReparseDeferralEnabled", [](const CallArgs& a) { P(a.pos.at(0))->deferral = a.pos.at(1).truthy(); return None; });
    add_method(T_parser, "ExternalEntityParserCreate", [](const CallArgs& a) -> var {
        XP* x = P(a.pos.at(0));
        var enc = a.pos.size() > 2 ? a.pos[2] : var(None);
        var np = new_parser(enc, x->ns ? S(x->sep) : var(None), x->self().getattr("intern"));
        P(np)->entities = x->entities;
        for (const char* h : kHandlers) np.setattr(h, x->self().getattr(h));
        return np;
    });
    add_getter(T_parser, "CurrentLineNumber", [](const var& v) {
        XP* x = P(v);
        int64_t l, c;
        x->position(x->finished || x->failed ? x->buf.size() : x->ev_off, l, c);
        return var(l);
    });
    add_getter(T_parser, "CurrentColumnNumber", [](const var& v) {
        XP* x = P(v);
        int64_t l, c;
        x->position(x->finished || x->failed ? x->buf.size() : x->ev_off, l, c);
        return var(c);
    });
    add_getter(T_parser, "CurrentByteIndex", [](const var& v) { return var((int64_t)P(v)->ev_off); });
    add_getter(T_parser, "ErrorCode", [](const var& v) { return var((int64_t)P(v)->err_code); });
    add_getter(T_parser, "ErrorLineNumber", [](const var& v) { return var(P(v)->err_line); });
    add_getter(T_parser, "ErrorColumnNumber", [](const var& v) { return var(P(v)->err_col); });
    add_getter(T_parser, "ErrorByteIndex", [](const var& v) { return var((int64_t)P(v)->ev_off); });
    add_getter(T_parser, "buffer_used", [](const var& v) { return var((int64_t)P(v)->text.size()); });
}

void init_errors(const var& m) {
    var messages = new_dict_obj(), codes = new_dict_obj();
    for (int i = 1; i < kNErrors; i++) {
        mod_set(m, kErrorNames[i], S(kErrors[i]));
        messages.setitem(var((int64_t)i), S(kErrors[i]));
        codes.setitem(S(kErrors[i]), var((int64_t)i));
    }
    mod_set(m, "messages", messages);
    mod_set(m, "codes", codes);
}

void init_model(const var& m) {
    struct { const char* n; int v; } c[] = {{"XML_CTYPE_EMPTY", 1}, {"XML_CTYPE_ANY", 2}, {"XML_CTYPE_MIXED", 3}, {"XML_CTYPE_NAME", 4},
                                            {"XML_CTYPE_CHOICE", 5}, {"XML_CTYPE_SEQ", 6}, {"XML_CQUANT_NONE", 0}, {"XML_CQUANT_OPT", 1},
                                            {"XML_CQUANT_REP", 2}, {"XML_CQUANT_PLUS", 3}};
    for (auto& e : c) mod_set(m, e.n, var((int64_t)e.v));
}

}  // namespace

void init_xml_modules() {
    register_native_module("pyexpat", init_pyexpat);
    register_native_module("pyexpat.errors", init_errors);
    register_native_module("pyexpat.model", init_model);
}

}  // namespace py
