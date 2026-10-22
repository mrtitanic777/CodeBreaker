// XUR scene writer and XUI XML compiler (see xur_write.h).

#include "xui/xur_write.h"

#include <algorithm>
#include <bit>
#include <charconv>
#include <climits>
#include <format>
#include <map>
#include <memory>
#include <tuple>

namespace xui
{
    namespace
    {
        void Put32(std::vector<uint8_t>& out, uint32_t v)
        {
            for (int shift = 24; shift >= 0; shift -= 8)
                out.push_back(uint8_t(v >> shift));
        }
        void Put16(std::vector<uint8_t>& out, uint16_t v)
        {
            out.push_back(uint8_t(v >> 8));
            out.push_back(uint8_t(v));
        }
        void PutFloat(std::vector<uint8_t>& out, float f) { Put32(out, std::bit_cast<uint32_t>(f)); }
        // The reader's packed form: one byte below 0xF0, 0xFn + one byte up to 0xEFF, else 0xFF + four bytes.
        void PutPacked(std::vector<uint8_t>& out, uint32_t v)
        {
            if (v < 0xF0)
                out.push_back(uint8_t(v));
            else if (v < 0xF00)
            {
                out.push_back(uint8_t(0xF0 | v >> 8));
                out.push_back(uint8_t(v));
            }
            else
            {
                out.push_back(0xFF);
                Put32(out, v);
            }
        }

        // Which level of a class chain (base first) a property definition belongs to, and its index there.
        bool LevelOf(const std::vector<const ClassDef*>& chain, const PropDef* def, size_t& level, uint32_t& index)
        {
            for (size_t l = 0; l < chain.size(); l++)
            {
                const PropTable& t = chain[l]->props;
                if (t.count && def >= t.defs && def < t.defs + t.count)
                {
                    level = l;
                    index = uint32_t(def - t.defs);
                    return true;
                }
            }
            return false;
        }

        struct Writer
        {
            const FigureLookup& figure;
            std::string& error;

            std::vector<std::string> strings;
            std::map<std::string, uint32_t, std::less<>> stringIndex;
            std::vector<float> floats;
            std::map<uint32_t, uint32_t> floatIndex;
            std::vector<Vec3> vectors;
            std::map<std::tuple<uint32_t, uint32_t, uint32_t>, uint32_t> vectorIndex;
            std::vector<Quat> quats;
            std::map<std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>, uint32_t> quatIndex;
            std::vector<uint32_t> colors;
            std::map<uint32_t, uint32_t> colorIndex;
            std::vector<uint8_t> cust;
            std::map<uint32_t, uint32_t> figureOffset; // the caller's figure id -> CUST offset
            std::vector<uint8_t> keyp, keyd, names, data;
            uint32_t keyValues = 0, keyRecords = 0, nameRecords = 0;
            std::map<const Node*, uint32_t> objects; // property objects, numbered in first-use order

            uint32_t String(const std::string& s)
            {
                if (s.empty())
                    return 0;
                const auto [it, added] = stringIndex.emplace(s, uint32_t(strings.size() + 1));
                if (added)
                    strings.push_back(s);
                return it->second;
            }
            uint32_t Float(float f)
            {
                const auto [it, added] = floatIndex.emplace(std::bit_cast<uint32_t>(f), uint32_t(floats.size()));
                if (added)
                    floats.push_back(f);
                return it->second;
            }
            uint32_t Vector(Vec3 v)
            {
                const auto key = std::make_tuple(std::bit_cast<uint32_t>(v.x), std::bit_cast<uint32_t>(v.y),
                                                 std::bit_cast<uint32_t>(v.z));
                const auto [it, added] = vectorIndex.emplace(key, uint32_t(vectors.size()));
                if (added)
                    vectors.push_back(v);
                return it->second;
            }
            uint32_t Quaternion(Quat q)
            {
                const auto key = std::make_tuple(std::bit_cast<uint32_t>(q.x), std::bit_cast<uint32_t>(q.y),
                                                 std::bit_cast<uint32_t>(q.z), std::bit_cast<uint32_t>(q.w));
                const auto [it, added] = quatIndex.emplace(key, uint32_t(quats.size()));
                if (added)
                    quats.push_back(q);
                return it->second;
            }
            uint32_t Color(uint32_t c)
            {
                const auto [it, added] = colorIndex.emplace(c, uint32_t(colors.size()));
                if (added)
                    colors.push_back(c);
                return it->second;
            }
            bool Figure(uint32_t id, uint32_t& offset)
            {
                if (const auto it = figureOffset.find(id); it != figureOffset.end())
                {
                    offset = it->second;
                    return true;
                }
                const FigurePath* path = figure ? figure(id) : nullptr;
                if (!path)
                {
                    error = std::format("no figure path for figure {}", id);
                    return false;
                }
                offset = uint32_t(cust.size());
                Put32(cust, uint32_t(12 + path->points.size() * 24));
                PutFloat(cust, path->width);
                PutFloat(cust, path->height);
                Put32(cust, uint32_t(path->points.size()));
                for (const FigurePoint& p : path->points)
                    for (float f : { p.x, p.y, p.c1x, p.c1y, p.c2x, p.c2y })
                        PutFloat(cust, f);
                figureOffset.emplace(id, offset);
                return true;
            }

            // A value's pool index (what the object stream and KEYP hold for every type but kObject).
            bool Index(const PropDef& def, const Value& v, uint32_t& index)
            {
                switch (def.type)
                {
                case Type::kBool: index = v.b ? 1 : 0; return true;
                case Type::kInteger: index = uint32_t(int32_t(v.i)); return true;
                case Type::kUnsigned: index = uint32_t(v.i); return true;
                case Type::kFloat: index = Float(v.f); return true;
                case Type::kString: index = String(v.s); return true;
                case Type::kColor: index = Color(v.color); return true;
                case Type::kVector: index = Vector(v.v); return true;
                case Type::kQuaternion: index = Quaternion(v.q); return true;
                case Type::kCustom: return Figure(v.custom, index);
                default:
                    error = std::format("{}: a value of type {} has no pool", def.name, int(def.type));
                    return false;
                }
            }

            bool WriteValue(std::vector<uint8_t>& out, const PropDef& def, const Value& v)
            {
                if (def.type == Type::kBool)
                {
                    out.push_back(v.b ? 1 : 0);
                    return true;
                }
                if (def.type != Type::kObject)
                {
                    uint32_t index = 0;
                    if (!Index(def, v, index))
                        return false;
                    PutPacked(out, index);
                    return true;
                }
                // A property object is written in full where it is first used, then referred to by its index.
                if (!v.object || !def.sub)
                {
                    error = std::format("{}: an object property without an object", def.name);
                    return false;
                }
                if (const auto it = objects.find(v.object.get()); it != objects.end())
                {
                    PutPacked(out, it->second);
                    return true;
                }
                const uint32_t index = uint32_t(objects.size());
                objects.emplace(v.object.get(), index);
                PutPacked(out, index);
                size_t count = 0;
                for (const Property& p : v.object->props)
                    count += p.values.size();
                PutPacked(out, uint32_t(count));
                return WriteValues(out, *def.sub, v.object->props);
            }

            // One class level: its mask, then the values of the properties set, in the level's order.
            bool WriteValues(std::vector<uint8_t>& out, const PropTable& table, const std::vector<Property>& props)
            {
                std::vector<const Property*> set(table.count, nullptr);
                uint32_t mask = 0;
                for (const Property& p : props)
                    if (table.count && p.def >= table.defs && p.def < table.defs + table.count && !p.values.empty())
                    {
                        const uint32_t i = uint32_t(p.def - table.defs);
                        if (i >= 32)
                        {
                            error = std::format("{}: property {} of a level has no mask bit", p.def->name, i);
                            return false;
                        }
                        set[i] = &p;
                        mask |= 1u << i;
                    }
                PutPacked(out, mask);
                for (uint32_t i = 0; i < table.count; i++)
                {
                    const Property* p = set[i];
                    if (!p)
                        continue;
                    if (p->def->flags & kPropIndexed)
                        PutPacked(out, uint32_t(p->values.size()));
                    else if (p->values.size() != 1)
                    {
                        error = std::format("{}: {} values for a property that is not indexed", p->def->name,
                                            p->values.size());
                        return false;
                    }
                    for (const Value& v : p->values)
                        if (!WriteValue(out, *p->def, v))
                            return false;
                }
                return true;
            }

            bool WriteObject(const Node& node)
            {
                PutPacked(data, String(node.className));
                size_t count = 0;
                for (const Property& p : node.props)
                    count += p.values.size();
                const bool timelines = !node.namedFrames.empty() || !node.timelines.empty();
                data.push_back(uint8_t((count ? 1 : 0) | (node.children.empty() ? 0 : 2) | (timelines ? 4 : 0)));
                if (count)
                {
                    const auto chain = ClassChain(node.className);
                    if (chain.empty())
                    {
                        error = std::format("class {} is not known", node.className);
                        return false;
                    }
                    for (const Property& p : node.props)
                    {
                        size_t level;
                        uint32_t index;
                        if (!LevelOf(chain, p.def, level, index))
                        {
                            error = std::format("{}: property {} is not one of its classes'", node.className,
                                                p.def->name);
                            return false;
                        }
                    }
                    PutPacked(data, uint32_t(count));
                    for (const ClassDef* c : chain)
                        if (!WriteValues(data, c->props, node.props))
                            return (error = std::format("{} ({} level): {}", node.className, c->name, error)), false;
                }
                if (!node.children.empty())
                {
                    PutPacked(data, uint32_t(node.children.size()));
                    for (const Node& child : node.children)
                        if (!WriteObject(child))
                            return false;
                }
                return !timelines || WriteTimelines(node);
            }

            bool WriteTimelines(const Node& node)
            {
                PutPacked(data, uint32_t(node.namedFrames.size()));
                if (!node.namedFrames.empty())
                {
                    PutPacked(data, nameRecords);
                    for (const NamedFrame& n : node.namedFrames)
                    {
                        PutPacked(names, String(n.name));
                        PutPacked(names, n.frame);
                        names.push_back(uint8_t(n.command));
                        if (n.command >= Command::kGoto)
                            PutPacked(names, String(n.target));
                        nameRecords++;
                    }
                }
                if (node.children.empty())
                {
                    if (!node.timelines.empty())
                    {
                        error = std::format("{}: timelines without children to animate", node.className);
                        return false;
                    }
                    return true;
                }
                PutPacked(data, uint32_t(node.timelines.size()));
                for (const Timeline& t : node.timelines)
                {
                    PutPacked(data, String(t.target));
                    PutPacked(data, uint32_t(t.tracks.size()));
                    for (const Track& tr : t.tracks)
                    {
                        if (!tr.def || tr.path.size() >= 0x7F)
                        {
                            error = std::format("{}: timeline {} has an unresolved track {}", node.className, t.target,
                                                tr.name);
                            return false;
                        }
                        data.push_back(uint8_t((tr.path.size() + 1) | (tr.hasIndex ? 0x80 : 0)));
                        data.push_back(tr.level);
                        data.push_back(tr.index);
                        data.insert(data.end(), tr.path.begin(), tr.path.end());
                        if (tr.hasIndex)
                            PutPacked(data, tr.element);
                    }
                    PutPacked(data, uint32_t(t.keys.size()));
                    PutPacked(data, keyRecords);
                    for (const KeyFrame& k : t.keys)
                    {
                        if (k.values.size() != t.tracks.size())
                        {
                            error = std::format("{}: timeline {} key {} has {} values for {} tracks", node.className,
                                                t.target, k.frame, k.values.size(), t.tracks.size());
                            return false;
                        }
                        PutPacked(keyd, k.frame);
                        keyd.push_back(uint8_t(uint8_t(k.interpolation) | uint8_t(k.direction) << 6));
                        switch (k.interpolation)
                        {
                        case Interpolation::kEase:
                            keyd.push_back(uint8_t(k.easeIn));
                            keyd.push_back(uint8_t(k.easeOut));
                            keyd.push_back(uint8_t(k.easeScale));
                            break;
                        case Interpolation::kPower:
                        case Interpolation::kExponential:
                        case Interpolation::kElastic:
                        case Interpolation::kBounce:
                            PutPacked(keyd, Vector({ k.p1, k.p2, 0 }));
                            break;
                        default:
                            break;
                        }
                        PutPacked(keyd, keyValues);
                        for (size_t i = 0; i < t.tracks.size(); i++)
                        {
                            uint32_t index = 0;
                            if (!Index(*t.tracks[i].def, k.values[i], index))
                                return false;
                            PutPacked(keyp, index);
                            keyValues++;
                        }
                        keyRecords++;
                    }
                }
                return true;
            }
        };

        struct Counts
        {
            uint32_t nodes = 0;
        };
        void CountNodes(const Node& node, Counts& c)
        {
            c.nodes++;
            for (const Node& child : node.children)
                CountNodes(child, c);
        }
    }

    bool WriteScene(const Node& root, const FigureLookup& figure, std::vector<uint8_t>& out, std::string& error)
    {
        Writer w{ figure, error };
        if (!w.WriteObject(root))
            return false;
        if (w.strings.size() > 0xFFFF)
        {
            error = std::format("{} strings (a scene holds at most 65535)", w.strings.size());
            return false;
        }

        std::vector<std::pair<const char*, std::vector<uint8_t>>> sections;
        {
            std::vector<uint8_t> s;
            size_t bytes = 0;
            for (const std::string& str : w.strings)
                bytes += str.size() + 1;
            Put32(s, uint32_t(bytes));
            Put16(s, uint16_t(w.strings.size()));
            for (const std::string& str : w.strings)
            {
                s.insert(s.end(), str.begin(), str.end());
                s.push_back(0);
            }
            sections.emplace_back("STRN", std::move(s));
        }
        auto add = [&sections](const char* tag, std::vector<uint8_t> bytes) {
            if (!bytes.empty())
                sections.emplace_back(tag, std::move(bytes));
        };
        std::vector<uint8_t> v;
        for (const Vec3& x : w.vectors)
            for (float f : { x.x, x.y, x.z })
                PutFloat(v, f);
        add("VECT", std::move(v));
        std::vector<uint8_t> q;
        for (const Quat& x : w.quats)
            for (float f : { x.x, x.y, x.z, x.w })
                PutFloat(q, f);
        add("QUAT", std::move(q));
        add("CUST", std::move(w.cust));
        std::vector<uint8_t> f;
        for (float x : w.floats)
            PutFloat(f, x);
        add("FLOT", std::move(f));
        std::vector<uint8_t> c;
        for (uint32_t x : w.colors)
            Put32(c, x);
        add("COLR", std::move(c));
        add("KEYP", std::move(w.keyp));
        add("KEYD", std::move(w.keyd));
        add("NAME", std::move(w.names));
        sections.emplace_back("DATA", std::move(w.data));

        // The header's pool sizes are allocation hints that the reader skips.
        Counts counts;
        CountNodes(root, counts);
        std::vector<uint8_t> hints;
        const uint32_t hint[12] = { uint32_t(w.strings.size()), uint32_t(w.vectors.size()), uint32_t(w.quats.size()),
                                    uint32_t(w.figureOffset.size()), uint32_t(w.floats.size()),
                                    uint32_t(w.colors.size()), w.keyValues, w.keyRecords, w.nameRecords,
                                    uint32_t(w.objects.size()), counts.nodes, 0 };
        for (uint32_t h : hint)
            PutPacked(hints, h);

        out.clear();
        out.insert(out.end(), { 'X', 'U', 'I', 'B' });
        Put32(out, 8);
        Put32(out, 0);
        Put16(out, 0x0E);
        const size_t totalAt = out.size();
        Put32(out, 0);
        Put16(out, uint16_t(sections.size()));
        out.insert(out.end(), hints.begin(), hints.end());
        size_t offset = out.size() + sections.size() * 12;
        for (const auto& [tag, bytes] : sections)
        {
            out.insert(out.end(), tag, tag + 4);
            Put32(out, uint32_t(offset));
            Put32(out, uint32_t(bytes.size()));
            offset += bytes.size();
        }
        for (const auto& [tag, bytes] : sections)
            out.insert(out.end(), bytes.begin(), bytes.end());
        const uint32_t total = uint32_t(out.size());
        for (int i = 0; i < 4; i++)
            out[totalAt + i] = uint8_t(total >> (24 - i * 8));
        return true;
    }

    // ---- XML ----

    namespace
    {
        struct XmlElement
        {
            std::string name;
            std::map<std::string, std::string, std::less<>> attributes;
            std::string text; // character data directly inside the element
            std::vector<XmlElement> children;
            size_t line = 0;

            const XmlElement* Child(std::string_view n) const
            {
                for (const XmlElement& c : children)
                    if (c.name == n)
                        return &c;
                return nullptr;
            }
        };

        struct XmlParser
        {
            std::string_view s;
            size_t at = 0;
            std::string& error;

            size_t Line() const
            {
                size_t line = 1;
                for (size_t i = 0; i < at && i < s.size(); i++)
                    line += s[i] == '\n';
                return line;
            }
            bool Fail(std::string what)
            {
                if (error.empty())
                    error = std::format("line {}: {}", Line(), what);
                return false;
            }
            bool StartsWith(std::string_view p) const { return s.substr(at, p.size()) == p; }
            void SkipSpace()
            {
                while (at < s.size() && (s[at] == ' ' || s[at] == '\t' || s[at] == '\r' || s[at] == '\n'))
                    at++;
            }
            // Comments, processing instructions and whitespace between elements.
            bool SkipMisc()
            {
                for (;;)
                {
                    SkipSpace();
                    const char* end = nullptr;
                    if (StartsWith("<!--"))
                        end = "-->";
                    else if (StartsWith("<?"))
                        end = "?>";
                    else
                        return true;
                    const size_t e = s.find(end, at);
                    if (e == std::string_view::npos)
                        return Fail("unterminated comment or declaration");
                    at = e + std::string_view(end).size();
                }
            }
            bool Decode(std::string_view raw, std::string& out)
            {
                for (size_t i = 0; i < raw.size(); i++)
                {
                    if (raw[i] != '&')
                    {
                        out += raw[i];
                        continue;
                    }
                    const size_t semi = raw.find(';', i);
                    if (semi == std::string_view::npos)
                        return Fail("an entity without ';'");
                    const std::string_view e = raw.substr(i + 1, semi - i - 1);
                    if (e == "lt") out += '<';
                    else if (e == "gt") out += '>';
                    else if (e == "amp") out += '&';
                    else if (e == "quot") out += '"';
                    else if (e == "apos") out += '\'';
                    else if (e.size() > 1 && e[0] == '#')
                    {
                        uint32_t cp = 0;
                        const bool hex = e[1] == 'x' || e[1] == 'X';
                        const std::string_view digits = e.substr(hex ? 2 : 1);
                        const auto r = std::from_chars(digits.data(), digits.data() + digits.size(), cp, hex ? 16 : 10);
                        if (r.ec != std::errc() || r.ptr != digits.data() + digits.size() || cp > 0x10FFFF)
                            return Fail(std::format("character reference &{};", e));
                        if (cp < 0x80)
                            out += char(cp);
                        else if (cp < 0x800)
                        {
                            out += char(0xC0 | cp >> 6);
                            out += char(0x80 | (cp & 0x3F));
                        }
                        else if (cp < 0x10000)
                        {
                            out += char(0xE0 | cp >> 12);
                            out += char(0x80 | (cp >> 6 & 0x3F));
                            out += char(0x80 | (cp & 0x3F));
                        }
                        else
                        {
                            out += char(0xF0 | cp >> 18);
                            out += char(0x80 | (cp >> 12 & 0x3F));
                            out += char(0x80 | (cp >> 6 & 0x3F));
                            out += char(0x80 | (cp & 0x3F));
                        }
                    }
                    else
                        return Fail(std::format("entity &{}; is not known", e));
                    i = semi;
                }
                return true;
            }
            static bool NameChar(char c)
            {
                return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' ||
                       c == '-' || c == '.' || c == ':';
            }
            bool Name(std::string& out)
            {
                const size_t start = at;
                while (at < s.size() && NameChar(s[at]))
                    at++;
                if (at == start)
                    return Fail("a name was expected");
                out = std::string(s.substr(start, at - start));
                return true;
            }
            bool Element(XmlElement& e)
            {
                e.line = Line();
                if (!StartsWith("<"))
                    return Fail("an element was expected");
                at++;
                if (!Name(e.name))
                    return false;
                for (;;)
                {
                    SkipSpace();
                    if (StartsWith("/>"))
                    {
                        at += 2;
                        return true;
                    }
                    if (StartsWith(">"))
                    {
                        at++;
                        break;
                    }
                    std::string attribute, value;
                    if (!Name(attribute))
                        return false;
                    SkipSpace();
                    if (!StartsWith("="))
                        return Fail(std::format("attribute {} has no value", attribute));
                    at++;
                    SkipSpace();
                    if (at >= s.size() || (s[at] != '"' && s[at] != '\''))
                        return Fail(std::format("attribute {}: a quoted value was expected", attribute));
                    const char quote = s[at++];
                    const size_t end = s.find(quote, at);
                    if (end == std::string_view::npos)
                        return Fail(std::format("attribute {}: unterminated value", attribute));
                    if (!Decode(s.substr(at, end - at), value))
                        return false;
                    at = end + 1;
                    e.attributes[attribute] = std::move(value);
                }
                for (;;)
                {
                    if (at >= s.size())
                        return Fail(std::format("<{}> is not closed", e.name));
                    if (StartsWith("<!--"))
                    {
                        if (!SkipMisc())
                            return false;
                        continue;
                    }
                    if (StartsWith("</"))
                    {
                        at += 2;
                        std::string closing;
                        if (!Name(closing))
                            return false;
                        if (closing != e.name)
                            return Fail(std::format("</{}> closes <{}>", closing, e.name));
                        SkipSpace();
                        if (!StartsWith(">"))
                            return Fail("'>' was expected");
                        at++;
                        return true;
                    }
                    if (StartsWith("<"))
                    {
                        XmlElement child;
                        if (!Element(child))
                            return false;
                        e.children.push_back(std::move(child));
                        continue;
                    }
                    const size_t next = s.find('<', at);
                    const size_t end = next == std::string_view::npos ? s.size() : next;
                    if (!Decode(s.substr(at, end - at), e.text))
                        return false;
                    at = end;
                }
            }
        };

        std::string_view Trim(std::string_view t)
        {
            while (!t.empty() && (t.front() == ' ' || t.front() == '\t' || t.front() == '\r' || t.front() == '\n'))
                t.remove_prefix(1);
            while (!t.empty() && (t.back() == ' ' || t.back() == '\t' || t.back() == '\r' || t.back() == '\n'))
                t.remove_suffix(1);
            return t;
        }

        // Comma-separated numbers (empty fields, as after a trailing comma, are skipped).
        bool Numbers(std::string_view text, std::vector<float>& out)
        {
            size_t at = 0;
            while (at <= text.size())
            {
                const size_t comma = std::min(text.find(',', at), text.size());
                const std::string_view field = Trim(text.substr(at, comma - at));
                if (!field.empty())
                {
                    float f = 0;
                    const auto r = std::from_chars(field.data(), field.data() + field.size(), f);
                    if (r.ec != std::errc() || r.ptr != field.data() + field.size())
                        return false;
                    out.push_back(f);
                }
                at = comma + 1;
            }
            return true;
        }

        struct Compiler
        {
            std::string& error;
            std::vector<FigurePath> figures;

            bool Fail(const XmlElement& at, std::string what)
            {
                if (error.empty())
                    error = std::format("line {}: <{}>: {}", at.line, at.name, what);
                return false;
            }

            bool ParseValue(const XmlElement& x, const PropDef& def, Value& v, float width, float height)
            {
                v.type = def.type;
                const std::string_view t = Trim(x.text);
                switch (def.type)
                {
                case Type::kBool:
                    if (t == "true" || t == "1")
                        v.b = true;
                    else if (t == "false" || t == "0")
                        v.b = false;
                    else
                        return Fail(x, std::format("\"{}\" is not a bool", t));
                    return true;
                case Type::kInteger:
                case Type::kUnsigned:
                {
                    int64_t i = 0;
                    const bool hex = t.starts_with("0x") || t.starts_with("0X");
                    const std::string_view digits = hex ? t.substr(2) : t;
                    const auto r = std::from_chars(digits.data(), digits.data() + digits.size(), i, hex ? 16 : 10);
                    if (r.ec != std::errc() || r.ptr != digits.data() + digits.size() || digits.empty())
                        return Fail(x, std::format("\"{}\" is not an integer", t));
                    if ((def.type == Type::kUnsigned && (i < 0 || i > 0xFFFFFFFFll)) ||
                        (def.type == Type::kInteger && (i < INT32_MIN || i > INT32_MAX)))
                        return Fail(x, std::format("{} is out of range", i));
                    v.i = i;
                    return true;
                }
                case Type::kFloat:
                {
                    std::vector<float> n;
                    if (!Numbers(t, n) || n.size() != 1)
                        return Fail(x, std::format("\"{}\" is not a number", t));
                    v.f = n[0];
                    return true;
                }
                case Type::kString:
                    v.s = x.text;
                    return true;
                case Type::kColor:
                {
                    const bool hex = t.starts_with("0x") || t.starts_with("0X");
                    const std::string_view digits = hex ? t.substr(2) : t;
                    const auto r = std::from_chars(digits.data(), digits.data() + digits.size(), v.color, hex ? 16 : 10);
                    if (r.ec != std::errc() || r.ptr != digits.data() + digits.size() || digits.empty())
                        return Fail(x, std::format("\"{}\" is not a colour (0xAARRGGBB)", t));
                    return true;
                }
                case Type::kVector:
                {
                    std::vector<float> n;
                    if (!Numbers(t, n) || n.size() != 3)
                        return Fail(x, std::format("\"{}\" is not a vector (x,y,z)", t));
                    v.v = { n[0], n[1], n[2] };
                    return true;
                }
                case Type::kQuaternion:
                {
                    std::vector<float> n;
                    if (!Numbers(t, n) || n.size() != 4)
                        return Fail(x, std::format("\"{}\" is not a quaternion (x,y,z,w)", t));
                    v.q = { n[0], n[1], n[2], n[3] };
                    return true;
                }
                case Type::kCustom:
                {
                    // count, then per point: x, y, control 1, control 2, and the point's type (XuiTool's; not stored).
                    std::vector<float> n;
                    if (!Numbers(t, n) || n.empty() || n[0] < 0 || n.size() != size_t(n[0]) * 7 + 1)
                        return Fail(x, "Points must be a count and 7 numbers per point");
                    FigurePath path;
                    path.width = width;
                    path.height = height;
                    for (size_t i = 0; i < size_t(n[0]); i++)
                    {
                        const float* p = &n[1 + i * 7];
                        path.points.push_back({ p[0], p[1], p[2], p[3], p[4], p[5] });
                    }
                    v.custom = uint32_t(figures.size());
                    figures.push_back(std::move(path));
                    return true;
                }
                case Type::kObject:
                {
                    const XmlElement* props = x.Child("Properties");
                    auto object = std::make_shared<Node>();
                    object->className = def.name;
                    if (props && !ParseProperties(*props, { def.sub }, object->props, width, height))
                        return false;
                    v.object = std::move(object);
                    return true;
                }
                default:
                    return Fail(x, "a property of an unknown type");
                }
            }

            // Properties of the tables `levels` (base first; the most derived level wins a name both have), in level
            // and index order.
            bool ParseProperties(const XmlElement& block, const std::vector<const PropTable*>& levels,
                                 std::vector<Property>& out, float width, float height)
            {
                std::map<std::pair<size_t, uint32_t>, Property> found;
                for (const XmlElement& x : block.children)
                {
                    // XuiTool's editor data ("[VisualClass]XuiScrollBar[/VisualClass]"): not a runtime property.
                    if (x.name == "DesignTimeData")
                        continue;
                    const PropDef* def = nullptr;
                    size_t level = 0;
                    uint32_t index = 0;
                    for (size_t l = levels.size(); l-- > 0 && !def;)
                        for (uint32_t i = 0; i < levels[l]->count; i++)
                            if (x.name == levels[l]->defs[i].name)
                            {
                                def = &levels[l]->defs[i];
                                level = l;
                                index = i;
                                break;
                            }
                    if (!def)
                        return Fail(x, "not a property of this class");
                    Property& p = found[{ level, index }];
                    p.def = def;
                    size_t slot = 0;
                    if (const auto it = x.attributes.find("index"); it != x.attributes.end())
                    {
                        if (!(def->flags & kPropIndexed))
                            return Fail(x, "index= on a property that is not indexed");
                        const auto r = std::from_chars(it->second.data(), it->second.data() + it->second.size(), slot);
                        if (r.ec != std::errc() || slot > 255)
                            return Fail(x, std::format("index \"{}\"", it->second));
                    }
                    else if (def->flags & kPropIndexed)
                        slot = p.values.size();
                    else if (!p.values.empty())
                        return Fail(x, "set twice");
                    if (p.values.size() <= slot)
                        p.values.resize(slot + 1);
                    if (!ParseValue(x, *def, p.values[slot], width, height))
                        return false;
                }
                for (auto& [key, p] : found)
                {
                    for (const Value& v : p.values)
                        if (v.type == Type::kEmpty)
                            return Fail(block, std::format("{}: an index is missing", p.def->name));
                    out.push_back(std::move(p));
                }
                return true;
            }

            bool ParseNode(const XmlElement& x, Node& node)
            {
                node.className = x.name;
                const auto chain = ClassChain(x.name);
                if (chain.empty())
                    return Fail(x, "not a known XUI class");
                if (const XmlElement* props = x.Child("Properties"))
                {
                    // A figure's path is stored with the figure's size.
                    float width = 60, height = 30;
                    for (const XmlElement& p : props->children)
                    {
                        std::vector<float> n;
                        if (p.name == "Width" && Numbers(p.text, n) && n.size() == 1)
                            width = n[0];
                        else if (p.name == "Height" && Numbers(p.text, n) && n.size() == 1)
                            height = n[0];
                    }
                    std::vector<const PropTable*> levels;
                    for (const ClassDef* c : chain)
                        levels.push_back(&c->props);
                    if (!ParseProperties(*props, levels, node.props, width, height))
                        return false;
                }
                for (const XmlElement& c : x.children)
                {
                    if (c.name == "Properties" || c.name == "Timelines")
                        continue;
                    node.children.emplace_back();
                    if (!ParseNode(c, node.children.back()))
                        return false;
                }
                if (const XmlElement* t = x.Child("Timelines"))
                    return ParseTimelines(*t, node);
                return true;
            }

            static bool ParseUnsigned(const XmlElement* x, uint32_t& out)
            {
                if (!x)
                    return true;
                const std::string_view t = Trim(x->text);
                const auto r = std::from_chars(t.data(), t.data() + t.size(), out);
                return r.ec == std::errc() && r.ptr == t.data() + t.size();
            }

            bool ResolveTrack(const Node& target, std::string_view path, Track& track)
            {
                const auto chain = ClassChain(target.className);
                std::vector<std::string_view> parts;
                for (size_t at = 0; at <= path.size();)
                {
                    const size_t dot = std::min(path.find('.', at), path.size());
                    parts.push_back(path.substr(at, dot - at));
                    at = dot + 1;
                }
                const PropDef* def = nullptr;
                for (size_t l = chain.size(); l-- > 0 && !def;)
                    for (uint32_t i = 0; i < chain[l]->props.count; i++)
                        if (parts[0] == chain[l]->props.defs[i].name)
                        {
                            def = &chain[l]->props.defs[i];
                            track.level = uint8_t(chain.size() - 1 - l);
                            track.index = uint8_t(i);
                            break;
                        }
                for (size_t p = 1; p < parts.size() && def; p++)
                {
                    const PropDef* next = nullptr;
                    if (def->type == Type::kObject && def->sub)
                        for (uint32_t i = 0; i < def->sub->count; i++)
                            if (parts[p] == def->sub->defs[i].name)
                            {
                                next = &def->sub->defs[i];
                                track.path.push_back(uint8_t(i));
                                break;
                            }
                    def = next;
                }
                if (!def || def->type == Type::kObject)
                    return false;
                track.def = def;
                track.name = std::string(path);
                return true;
            }

            bool ParseTimelines(const XmlElement& x, Node& node)
            {
                static constexpr std::pair<const char*, Command> kCommands[] = {
                    { "play", Command::kPlay }, { "stop", Command::kStop }, { "goto", Command::kGoto },
                    { "gotoandplay", Command::kGotoAndPlay }, { "gotoandstop", Command::kGotoAndStop },
                };
                if (const XmlElement* named = x.Child("NamedFrames"))
                    for (const XmlElement& f : named->children)
                    {
                        NamedFrame n;
                        if (const XmlElement* name = f.Child("Name"))
                            n.name = name->text;
                        if (!ParseUnsigned(f.Child("Time"), n.frame))
                            return Fail(f, "Time is not a frame number");
                        if (const XmlElement* command = f.Child("Command"))
                        {
                            const std::string_view c = Trim(command->text);
                            bool known = false;
                            for (const auto& [text, value] : kCommands)
                                if (c == text)
                                {
                                    n.command = value;
                                    known = true;
                                }
                            if (!known)
                                return Fail(f, std::format("command \"{}\" is not known", c));
                        }
                        if (n.command >= Command::kGoto)
                        {
                            const XmlElement* params = f.Child("CommandParams");
                            if (!params)
                                return Fail(f, "a goto without CommandParams");
                            n.target = params->text;
                        }
                        node.namedFrames.push_back(std::move(n));
                    }
                for (const XmlElement& t : x.children)
                {
                    if (t.name != "Timeline")
                        continue;
                    Timeline timeline;
                    if (const XmlElement* id = t.Child("Id"))
                        timeline.target = id->text;
                    const Node* target = nullptr;
                    for (const Node& child : node.children)
                        if (child.Id() == timeline.target)
                        {
                            target = &child;
                            break;
                        }
                    if (!target)
                        return Fail(t, std::format("no child \"{}\" to animate", timeline.target));
                    for (const XmlElement& c : t.children)
                    {
                        if (c.name == "TimelineProp")
                        {
                            Track track;
                            if (!ResolveTrack(*target, Trim(c.text), track))
                                return Fail(c, std::format("{} is not a property of {}", Trim(c.text),
                                                           target->className));
                            if (const auto it = c.attributes.find("index"); it != c.attributes.end())
                            {
                                track.hasIndex = true;
                                const auto r = std::from_chars(it->second.data(), it->second.data() + it->second.size(),
                                                               track.element);
                                if (r.ec != std::errc())
                                    return Fail(c, std::format("index \"{}\"", it->second));
                            }
                            timeline.tracks.push_back(std::move(track));
                        }
                        else if (c.name == "KeyFrame")
                        {
                            KeyFrame key;
                            uint32_t interpolation = 0;
                            if (!ParseUnsigned(c.Child("Time"), key.frame) ||
                                !ParseUnsigned(c.Child("Interpolation"), interpolation))
                                return Fail(c, "Time and Interpolation must be numbers");
                            // XuiTool writes the XDK's three: linear, none, ease (the parametric curves XAM adds
                            // need parameters it has no names for).
                            if (interpolation > uint32_t(Interpolation::kEase))
                                return Fail(c, std::format("interpolation {} (0 linear, 1 none, 2 ease)", interpolation));
                            key.interpolation = Interpolation(interpolation);
                            if (key.interpolation == Interpolation::kEase)
                            {
                                int ease[3] = {};
                                const char* names[3] = { "EaseIn", "EaseOut", "EaseScale" };
                                for (int i = 0; i < 3; i++)
                                    if (const XmlElement* e = c.Child(names[i]))
                                    {
                                        const std::string_view et = Trim(e->text);
                                        const auto r = std::from_chars(et.data(), et.data() + et.size(), ease[i]);
                                        if (r.ec != std::errc() || ease[i] < -100 || ease[i] > 100)
                                            return Fail(*e, std::format("\"{}\" (-100 ... 100)", et));
                                    }
                                key.easeIn = int8_t(ease[0]);
                                key.easeOut = int8_t(ease[1]);
                                key.easeScale = int8_t(ease[2]);
                                EaseControls(key.easeIn, key.easeOut, key.easeScale, key.p1, key.p2);
                            }
                            for (const XmlElement& p : c.children)
                            {
                                if (p.name != "Prop")
                                    continue;
                                if (key.values.size() >= timeline.tracks.size())
                                    return Fail(p, "more values than the timeline has properties");
                                Value v;
                                if (!ParseValue(p, *timeline.tracks[key.values.size()].def, v, 0, 0))
                                    return false;
                                key.values.push_back(std::move(v));
                            }
                            if (key.values.size() != timeline.tracks.size())
                                return Fail(c, std::format("{} values for {} properties", key.values.size(),
                                                           timeline.tracks.size()));
                            timeline.keys.push_back(std::move(key));
                        }
                    }
                    node.timelines.push_back(std::move(timeline));
                }
                return true;
            }
        };
    }

    bool CompileScene(std::string_view xml, std::vector<uint8_t>& out, std::string& error)
    {
        if (xml.starts_with("\xEF\xBB\xBF"))
            xml.remove_prefix(3);
        XmlParser parser{ xml, 0, error };
        XmlElement root;
        if (!parser.SkipMisc() || !parser.Element(root) || !parser.SkipMisc())
            return false;
        if (parser.at != xml.size())
            return parser.Fail("text after the root element");
        Compiler compiler{ error };
        Node node;
        if (!compiler.ParseNode(root, node))
            return false;
        return WriteScene(node,
                          [&compiler](uint32_t id) { return id < compiler.figures.size() ? &compiler.figures[id] : nullptr; },
                          out, error);
    }
}
