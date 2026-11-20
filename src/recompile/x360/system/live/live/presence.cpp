#include "live/presence.h"

#include "log.h"
#include "xui/inflate.h"

#include <cctype>
#include <cstring>
#include <format>
#include <fstream>
#include <optional>
#include <string_view>
#include <vector>

namespace live::presence
{
    namespace
    {
        uint32_t Be32(const uint8_t* p) { return uint32_t(p[0]) << 24 | uint32_t(p[1]) << 16 | uint32_t(p[2]) << 8 | p[3]; }

        // The deflate data of a gzip member (RFC 1952): the header's optional fields skipped.
        bool Gunzip(const uint8_t* data, size_t size, std::vector<uint8_t>& out, std::string& error)
        {
            if (size < 18 || data[0] != 0x1F || data[1] != 0x8B || data[2] != 8)
            {
                error = "not a gzip stream";
                return false;
            }
            const uint8_t flags = data[3];
            size_t at = 10;
            if (flags & 4) // FEXTRA
                at += 2 + size_t(data[at] | data[at + 1] << 8);
            for (const uint8_t bit : { uint8_t(8), uint8_t(16) }) // FNAME, FCOMMENT: zero-terminated
                if (flags & bit)
                {
                    while (at < size && data[at] != 0)
                        at++;
                    at++;
                }
            if (flags & 2) // FHCRC
                at += 2;
            if (at >= size || !xui::Inflate(data + at, size - at, out, false))
            {
                error = "bad deflate data";
                return false;
            }
            return true;
        }

        std::optional<uint32_t> Number(std::string_view text)
        {
            if (text.empty())
                return std::nullopt;
            const bool hex = text.size() > 2 && text[0] == '0' && (text[1] == 'x' || text[1] == 'X');
            if (hex)
                text.remove_prefix(2);
            uint64_t v = 0;
            for (const char c : text)
            {
                const int d = c >= '0' && c <= '9' ? c - '0'
                            : hex && c >= 'a' && c <= 'f' ? c - 'a' + 10
                            : hex && c >= 'A' && c <= 'F' ? c - 'A' + 10 : -1;
                if (d < 0)
                    return std::nullopt;
                v = v * (hex ? 16 : 10) + uint64_t(d);
                if (v > 0xFFFFFFFFull)
                    return std::nullopt;
            }
            return uint32_t(v);
        }

        // One start tag: its name and attributes (values as written; the ones read here are numbers).
        struct Tag
        {
            std::string_view name;
            std::map<std::string_view, std::string_view> attributes;
            bool closing = false, empty = false;
        };

        // The next tag at or after `at` (comments, declarations and processing instructions skipped).
        bool NextTag(std::string_view xml, size_t& at, Tag& tag)
        {
            for (;;)
            {
                const size_t open = xml.find('<', at);
                if (open == std::string_view::npos)
                    return false;
                if (xml.compare(open, 4, "<!--") == 0)
                {
                    const size_t end = xml.find("-->", open);
                    at = end == std::string_view::npos ? xml.size() : end + 3;
                    continue;
                }
                const size_t close = xml.find('>', open);
                if (close == std::string_view::npos)
                    return false;
                at = close + 1;
                if (xml[open + 1] == '?' || xml[open + 1] == '!')
                    continue;
                std::string_view body = xml.substr(open + 1, close - open - 1);
                tag = {};
                if (!body.empty() && body.front() == '/')
                {
                    tag.closing = true;
                    body.remove_prefix(1);
                }
                if (!body.empty() && body.back() == '/')
                {
                    tag.empty = true;
                    body.remove_suffix(1);
                }
                size_t i = 0;
                while (i < body.size() && !std::isspace(uint8_t(body[i])))
                    i++;
                tag.name = body.substr(0, i);
                while (i < body.size())
                {
                    while (i < body.size() && std::isspace(uint8_t(body[i])))
                        i++;
                    const size_t eq = body.find('=', i);
                    if (eq == std::string_view::npos)
                        break;
                    std::string_view name = body.substr(i, eq - i);
                    while (!name.empty() && std::isspace(uint8_t(name.back())))
                        name.remove_suffix(1);
                    size_t q = eq + 1;
                    while (q < body.size() && std::isspace(uint8_t(body[q])))
                        q++;
                    if (q >= body.size() || (body[q] != '"' && body[q] != '\''))
                        break;
                    const size_t end = body.find(body[q], q + 1);
                    if (end == std::string_view::npos)
                        break;
                    tag.attributes[name] = body.substr(q + 1, end - q - 1);
                    i = end + 1;
                }
                return true;
            }
        }

        std::optional<uint32_t> Attribute(const Tag& tag, std::string_view name)
        {
            const auto it = tag.attributes.find(name);
            return it == tag.attributes.end() ? std::nullopt : Number(it->second);
        }
    }

    bool LoadXsrc(const uint8_t* xsrc, size_t size, Definitions& out, std::string& error)
    {
        out = {};
        if (size < 16 || std::memcmp(xsrc, "XSRC", 4) != 0)
        {
            error = "not an XSRC entry";
            return false;
        }
        const uint64_t nameLength = Be32(xsrc + 12);
        const uint64_t header = 16 + nameLength + 8;
        if (header > size)
        {
            error = "XSRC header runs past the entry";
            return false;
        }
        const uint32_t xmlSize = Be32(xsrc + 16 + nameLength), compressed = Be32(xsrc + 20 + nameLength);
        if (header + compressed > size)
        {
            error = std::format("XSRC data ({} bytes) runs past the entry", compressed);
            return false;
        }
        std::vector<uint8_t> xml16;
        if (!Gunzip(xsrc + header, compressed, xml16, error))
            return false;
        if (xml16.size() != xmlSize)
        {
            error = std::format("XLAST is {} bytes, the header says {}", xml16.size(), xmlSize);
            return false;
        }
        // UTF-16LE to bytes: the tags and attributes read here are ASCII (other characters become '?').
        std::string xml;
        xml.reserve(xml16.size() / 2);
        for (size_t i = 0; i + 1 < xml16.size(); i += 2)
        {
            const uint16_t c = uint16_t(xml16[i] | xml16[i + 1] << 8);
            if (c != 0xFEFF)
                xml += c < 0x80 ? char(c) : '?';
        }

        size_t at = 0;
        Tag tag;
        std::optional<uint32_t> context; // inside <Context id=...>
        while (NextTag(xml, at, tag))
        {
            if (tag.closing)
            {
                if (tag.name == "Context")
                    context.reset();
                continue;
            }
            if (tag.name == "PresenceMode")
            {
                const auto value = Attribute(tag, "contextValue"), string = Attribute(tag, "stringId");
                if (value && string)
                    out.modes[*value] = *string;
            }
            else if (tag.name == "Context" && !tag.empty)
                context = Attribute(tag, "id");
            else if (tag.name == "ContextValue" && context)
            {
                const auto value = Attribute(tag, "value"), string = Attribute(tag, "stringId");
                if (value && string)
                    out.contextValues[*context][*value] = *string;
            }
        }
        return true;
    }

    std::string Format(const Definitions& definitions, const std::map<uint32_t, uint32_t>& contexts,
                       const std::map<uint32_t, std::string>& properties,
                       const std::function<std::string(uint32_t)>& string)
    {
        const auto mode = contexts.find(kContextPresence);
        if (mode == contexts.end())
            return {};
        const auto modeString = definitions.modes.find(mode->second);
        if (modeString == definitions.modes.end())
            return {};
        const std::string text = string(modeString->second);
        std::string out;
        for (size_t i = 0; i < text.size();)
        {
            const size_t close = text[i] == '{' ? text.find('}', i) : std::string::npos;
            if (close == std::string::npos)
            {
                out += text[i++];
                continue;
            }
            const std::string_view token = std::string_view(text).substr(i + 1, close - i - 1);
            i = close + 1;
            if (token.size() > 1 && token[0] == 'c')
            {
                // A context the title has not set yet has its default value, 0 for the ones seen so far.
                if (const auto id = Number(token.substr(1)))
                {
                    const auto value = contexts.find(*id);
                    const auto values = definitions.contextValues.find(*id);
                    if (values != definitions.contextValues.end())
                        if (const auto s = values->second.find(value == contexts.end() ? 0 : value->second);
                            s != values->second.end())
                            out += string(s->second);
                }
            }
            else if (token.size() > 1 && token[0] == 'p')
            {
                if (const auto id = Number(token.substr(1)))
                    if (const auto p = properties.find(*id); p != properties.end())
                        out += p->second;
            }
            else
                out += "{" + std::string(token) + "}";
        }
        return out;
    }
}

namespace live::presence
{
    int RunTest(const std::string& spaPath)
    {
        std::ifstream file(spaPath, std::ios::binary);
        const std::vector<uint8_t> spa((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        if (spa.size() < 24 || std::memcmp(spa.data(), "XDBF", 4) != 0)
        {
            Log("Presence test: {} is not an XDBF file", spaPath);
            return 1;
        }
        auto be16 = [&](size_t at) { return uint32_t(spa[at] << 8 | spa[at + 1]); };
        const uint32_t entryCount = Be32(&spa[8]), used = Be32(&spa[12]), freeCount = Be32(&spa[16]);
        const size_t data = 24 + size_t(entryCount) * 18 + size_t(freeCount) * 8;
        Definitions definitions;
        std::map<uint32_t, std::string> strings;
        for (uint32_t i = 0; i < used; i++)
        {
            const size_t e = 24 + size_t(i) * 18;
            const uint32_t section = be16(e), id = Be32(&spa[e + 6]), offset = Be32(&spa[e + 10]),
                           size = Be32(&spa[e + 14]);
            const uint8_t* p = &spa[data + offset];
            if (section == 1 && id == 0x58535243)
            {
                std::string error;
                if (!LoadXsrc(p, size, definitions, error))
                {
                    Log("Presence test: {}", error);
                    return 1;
                }
            }
            else if (section == 3 && id == 1) // English
                for (size_t at = 14, k = 0, count = size_t(p[12] << 8 | p[13]); k < count; k++)
                {
                    const uint32_t sid = uint32_t(p[at] << 8 | p[at + 1]), length = uint32_t(p[at + 2] << 8 | p[at + 3]);
                    strings[sid] = std::string(reinterpret_cast<const char*>(p + at + 4), length);
                    at += 4 + length;
                }
        }
        const auto string = [&](uint32_t id) { const auto it = strings.find(id); return it == strings.end() ? std::string() : it->second; };
        Log("Presence test: {} modes, {} contexts with value strings, {} strings", definitions.modes.size(),
            definitions.contextValues.size(), strings.size());
        for (const auto& [mode, stringId] : definitions.modes)
            for (uint32_t value = 0; value < 3; value++)
                Log("Presence test: mode {} (string {}), contexts = {}: \"{}\"", mode, stringId, value,
                    Format(definitions, { { kContextPresence, mode }, { 0, value } }, {}, string));
        return definitions.modes.empty() ? 1 : 0;
    }
}
