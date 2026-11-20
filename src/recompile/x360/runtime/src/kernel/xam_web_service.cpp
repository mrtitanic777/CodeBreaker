// XLiveBase message 0x58072, XOnlineGetWebServiceTaskBufferSize (kernel/xam_web_service.h).
//
// XAM 17559 0x818E1828 sizes the task buffer a web-service call (XOnlineCallWebService, message 0x5806A) will need
// from the call's URL template and its request / response schemas. It is arithmetic only: no network, no sign-in.
// The dash does it for every cloud saved-games request when System Settings > Storage starts an operation
// (dash 0x92300FE0 -> 0x927E2760) and mallocs the largest size, so this must not fail offline.
//
// Schemas are comma-separated fields `name{key:value,...}`, a field optionally followed by `[children]`:
//   t   (0x81A62298, record +0x2C) stream 1, item 2, str 3, wstr 4, int 5, uint 6, bool 7, datetime 8, bin 9, xuid 10,
//       hrfacility 11, hrcode 12
//   loc (record +0x24) url 1, qs 2, hdr 3, dom 4, body 5, part<N> 6 (N at +0x28)
//   enc (record +0x30) url 1, json 2, xml 3, binary 4, hex 5, base64 6, wav16000 7, sirensr 8
//   size (record +0x3C); offset, max, opt, countoffset, countsize, optflag are not used for sizing.
//
// size = 0x1B68 + 0x5C * (records(response) + 1) + 0x5C * (records(request) + 1)     records counted with children
//      + align4(A) + align4(B) + align4(A + 0xC00)                                      0x818F7958, 0x818F65E0
//      + 0x400 if a top-level request field is in a part, + 0x1000 if one in a body or part is not a stream,
//      + 0x1000 if a top-level response field is a body item, + 0x426C0 for flag 0x10, + 0xB530 for flag 0x20;
//      flag 0x1000: x + 0x100, then + ((x + 0x20) & ~0x1F) + 0x254.
// B (the URL, 0x818F74B8): the template's literal characters; each `{token}`: `{LiveHive:...}` (10 or more characters)
//   0x80, a request field at loc url with that name its value size in URL form (0x818FA4F0 mode 1), anything else 0;
//   each request field at loc qs: 2 + name length + its value size in URL form; +1 if not 0.
// A (headers, 0x818F7958): each request field at loc hdr: 4 + name length + its value size (mode 0); at loc body: the
//   length of its Content-Type line(s) for its enc; 0x30 for a response body item when no request header is
//   "Accept"; 0x5A when a request field is in a part; flags 2 +0x401A, 0x880 +0x2800, 0x50 +0x1C, 0x10 +0x18,
//   0x20 +0x17, 0x1000 +0x55; +1 if not 0.
// Name lengths: a field named "?" counts 4 (0x81A61C48).

#include "kernel/kernel.h"

#include <cstring>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace
{
    struct Field
    {
        std::string name;
        uint32_t type = 0, loc = 0, enc = 0, size = 0;
    };

    // 0x81A61D48: the length of the item at `s` (up to a comma outside braces and brackets).
    size_t ItemLength(std::string_view s)
    {
        int depth = 0;
        for (size_t i = 0; i < s.size(); i++)
        {
            const char c = s[i];
            if (c == '{' || c == '[')
                depth++;
            else if (c == '}' || c == ']')
                depth--;
            else if (depth == 0 && c == ',')
                return i;
        }
        return s.size();
    }

    // 0x81A61DB8: the records a schema makes, children included.
    uint32_t CountRecords(std::string_view s)
    {
        uint32_t count = 0;
        while (!s.empty())
        {
            const size_t length = ItemLength(s);
            const size_t open = s.find('{'), close = s.find('}');
            if (open == std::string_view::npos || close == std::string_view::npos || close > length)
                Fatal("XOnlineGetWebServiceTaskBufferSize: schema item \"{}\" has no {{...}} (XAM loops forever)",
                      std::string(s.substr(0, length)));
            if (close + 1 < s.size() && s[close + 1] == '[')
                count += CountRecords(s.substr(close + 2, length - close - 3));
            count++;
            s.remove_prefix(std::min(length + 1, s.size()));
        }
        return count;
    }

    uint32_t Code(const std::map<std::string, uint32_t>& names, const std::string& value, const char* key,
                  std::string_view item)
    {
        const auto it = names.find(value);
        if (it == names.end())
            Fatal("XOnlineGetWebServiceTaskBufferSize: {}:{} in \"{}\" is not a name XAM knows (0x80004001)", key,
                  value, std::string(item));
        return it->second;
    }

    // The top-level fields (0x81A632E0 parses children into their parent's list, which sizing does not walk).
    std::vector<Field> TopLevelFields(std::string_view s)
    {
        static const std::map<std::string, uint32_t> types = { { "stream", 1 }, { "item", 2 }, { "str", 3 },
            { "wstr", 4 }, { "int", 5 }, { "uint", 6 }, { "bool", 7 }, { "datetime", 8 }, { "bin", 9 },
            { "xuid", 10 }, { "hrfacility", 11 }, { "hrcode", 12 } };
        static const std::map<std::string, uint32_t> locs = { { "url", 1 }, { "qs", 2 }, { "hdr", 3 }, { "dom", 4 },
            { "body", 5 }, { "part", 6 } };
        static const std::map<std::string, uint32_t> encs = { { "url", 1 }, { "json", 2 }, { "xml", 3 },
            { "binary", 4 }, { "hex", 5 }, { "base64", 6 }, { "wav16000", 7 }, { "sirensr", 8 } };
        std::vector<Field> fields;
        while (!s.empty())
        {
            const size_t length = ItemLength(s);
            const std::string_view item = s.substr(0, length);
            const size_t open = item.find('{'), close = item.find('}');
            Field f;
            f.name = std::string(item.substr(0, open));
            if (f.name.starts_with('*'))
                Fatal("XOnlineGetWebServiceTaskBufferSize: field \"{}\" (a '*' name makes no record; not traced)",
                      std::string(item));
            if (f.name.starts_with('@'))
                Fatal("XOnlineGetWebServiceTaskBufferSize: field \"{}\" (an '@' name; not traced)", std::string(item));
            std::string_view attributes = item.substr(open + 1, close - open - 1);
            while (!attributes.empty())
            {
                const size_t comma = std::min(attributes.find(','), attributes.size());
                const std::string_view pair = attributes.substr(0, comma);
                const size_t colon = pair.find(':');
                if (colon == std::string_view::npos)
                    Fatal("XOnlineGetWebServiceTaskBufferSize: attribute \"{}\" in \"{}\"", std::string(pair),
                          std::string(item));
                const std::string key(pair.substr(0, colon)), value(pair.substr(colon + 1));
                if (key == "t")
                    f.type = Code(types, value, "t", item);
                else if (key == "loc")
                    f.loc = value.starts_with("part") ? 6 : Code(locs, value, "loc", item); // part<N>: part N
                else if (key == "enc")
                    f.enc = Code(encs, value, "enc", item);
                else if (key == "size")
                    f.size = uint32_t(std::stoul(value));
                else if (key != "offset" && key != "max" && key != "opt" && key != "countoffset" &&
                         key != "countsize" && key != "optflag")
                    Fatal("XOnlineGetWebServiceTaskBufferSize: key \"{}\" in \"{}\" is not traced", key,
                          std::string(item));
                attributes.remove_prefix(std::min(comma + 1, attributes.size()));
            }
            if (f.type == 0 || f.loc == 0)
                Fatal("XOnlineGetWebServiceTaskBufferSize: field \"{}\" lacks t or loc (0x80070057)",
                      std::string(item));
            fields.push_back(std::move(f));
            s.remove_prefix(std::min(length + 1, s.size()));
        }
        return fields;
    }

    // 0x818FA4F0: the characters a value takes as text; `url` = escaped for a URL (mode 1).
    uint32_t ValueSize(const Field& f, bool url)
    {
        switch (f.type)
        {
        case 3: return url ? f.size * 3 : f.size;          // str
        case 4: return url ? f.size * 9 : f.size * 3;      // wstr
        case 5: return f.size == 1 ? 5 : f.size == 2 ? 7 : f.size == 4 ? 12 : f.size == 8 ? 21 : 0; // int
        case 6:                                            // uint
        case 10:                                           // xuid
            return f.size == 1 ? 4 : f.size == 2 ? 6 : f.size == 4 ? 11 : f.size == 8 ? 21 : 0;
        case 7: return 6;                                  // bool
        case 8: return 0x1D;                               // datetime
        case 9:                                            // bin
            if (f.enc == 5)
                return (f.size & 0x7FFFFFFF) * 2 + 1;
            if (f.enc == 6)
                Fatal("XOnlineGetWebServiceTaskBufferSize: base64 field \"{}\" (0x818C4BF0 not traced)", f.name);
            return 0;
        default: return 0;                                 // stream, item, hrfacility, hrcode
        }
    }

    uint32_t NameLength(const Field& f)
    {
        return f.name == "?" ? 4 : uint32_t(f.name.size());
    }

    // 0x818F6F68: the Content-Type line(s) a body field's encoding sends.
    uint32_t ContentTypeLength(uint32_t enc)
    {
        switch (enc)
        {
        case 2: return 32; // "Content-Type: application/json\r\n"
        case 3: return 31; // "Content-Type: application/xml\r\n"
        case 4: return 75; // "...application/octet-stream\r\nContent-Transfer-Encoding: binary\r\n"
        case 7: return 66; // "Content-Type: audio/x-wav; codec=\"audio/pcm\"; samplingrate=16000\r\n"
        case 8: return 29; // "Content-Type: audio/sirensr\r\n"
        default: return 0;
        }
    }

    uint32_t Align4(uint32_t v)
    {
        return (v + 3) & ~3u;
    }

    std::string GuestString(uint32_t address)
    {
        return address ? std::string(reinterpret_cast<const char*>(guest::g_memory.Translate(address))) : std::string();
    }
}

namespace kernel
{
    // XLiveBase 0x58072 with its argument block {url, request schema, response schema, flags, DWORD* size}.
    uint32_t WebServiceTaskBufferSize(uint32_t args)
    {
        auto& mem = guest::g_memory;
        const std::string url = GuestString(mem.Read32(args)), requestSchema = GuestString(mem.Read32(args + 4)),
                          responseSchema = GuestString(mem.Read32(args + 8));
        const uint32_t flags = mem.Read32(args + 12), sizePtr = mem.Read32(args + 16);
        const std::vector<Field> request = TopLevelFields(requestSchema), response = TopLevelFields(responseSchema);

        uint32_t size = 0x1B68;
        if (mem.Read32(args + 8))
            size += 0x5C * (CountRecords(responseSchema) + 1);
        if (mem.Read32(args + 4))
            size += 0x5C * (CountRecords(requestSchema) + 1);
        bool part = false, notStream = false, responseBodyItem = false, accept = false;
        for (const Field& f : response)
            if (f.loc == 5 && f.type == 2)
                responseBodyItem = true;
        for (const Field& f : request)
            if (f.loc == 5 || f.loc == 6)
            {
                if (f.type != 1)
                    notStream = true;
                if (f.loc != 5)
                    part = true;
            }

        uint32_t a = 0, b = 0;
        for (size_t at = 0; at < url.size();)
        {
            const size_t open = url.find('{', at);
            const size_t close = open == std::string::npos ? std::string::npos : url.find('}', open);
            if (close == std::string::npos)
            {
                b += uint32_t(url.size() - at);
                break;
            }
            b += uint32_t(open - at);
            const std::string token = url.substr(open + 1, close - open - 1);
            if (token.size() >= 10 && token.starts_with("LiveHive:"))
                b += 0x80;
            else
                for (const Field& f : request)
                    if (f.loc == 1 && f.name == token)
                    {
                        b += ValueSize(f, true);
                        break;
                    }
            at = close + 1;
        }
        for (const Field& f : request)
        {
            if (f.loc == 2)
                b += 2 + NameLength(f) + ValueSize(f, true);
            else if (f.loc == 3)
            {
                if (f.name == "Accept")
                    accept = true;
                a += 4 + NameLength(f) + ValueSize(f, false);
            }
            else if (f.loc == 5)
                a += ContentTypeLength(f.enc);
        }
        if (responseBodyItem && !accept)
            a += 0x30;
        if (part)
            a += 0x5A;
        if (flags & 2)
            a += 0x401A;
        if (flags & 0x880)
            a += 0x2800;
        if (flags & 0x50)
            a += 0x1C;
        if (flags & 0x10)
            a += 0x18;
        if (flags & 0x20)
            a += 0x17;
        if (flags & 0x1000)
            a += 0x55;
        if (a)
            a++;
        if (b)
            b++;

        size += Align4(a) + Align4(b) + Align4(a + 0xC00);
        if (part)
            size += 0x400;
        if (notStream)
            size += 0x1000;
        if (responseBodyItem)
            size += 0x1000;
        if (flags & 0x10)
            size += 0x426C0;
        if (flags & 0x20)
            size += 0xB530;
        if (flags & 0x1000)
        {
            size += 0x100;
            size += ((size + 0x20) & ~0x1Fu) + 0x254;
        }
        Log("XOnlineGetWebServiceTaskBufferSize({}, flags {:X}): {} bytes", url, flags, size);
        if (sizePtr)
            mem.Write32(sizePtr, size);
        return 0;
    }
}
