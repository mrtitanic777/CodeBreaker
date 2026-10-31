// unicode.cpp -- see unicode.hpp.
#include "unicode.hpp"

#include <cctype>
#include <cstring>
#include <mutex>
#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

namespace pyfront::uni {

namespace {

// UTF-8 <-> code points (generalized UTF-8: surrogates pass through).
std::vector<uint32_t> decode(std::string_view s) {
    std::vector<uint32_t> out;
    for (size_t i = 0; i < s.size();) {
        unsigned char c = (unsigned char)s[i];
        int n = c < 0x80 ? 1 : c >= 0xF0 ? 4 : c >= 0xE0 ? 3 : c >= 0xC0 ? 2 : 1;
        if (i + n > s.size()) n = 1;
        uint32_t cp = n == 1 ? c : (uint32_t)(c & (0x7F >> n));
        for (int k = 1; k < n; ++k) cp = (cp << 6) | ((unsigned char)s[i + k] & 0x3F);
        out.push_back(cp);
        i += (size_t)n;
    }
    return out;
}

void encode(std::string& out, uint32_t cp) {
    if (cp < 0x80) out.push_back((char)cp);
    else if (cp < 0x800) { out.push_back((char)(0xC0 | (cp >> 6))); out.push_back((char)(0x80 | (cp & 0x3F))); }
    else if (cp < 0x10000) { out.push_back((char)(0xE0 | (cp >> 12))); out.push_back((char)(0x80 | ((cp >> 6) & 0x3F))); out.push_back((char)(0x80 | (cp & 0x3F))); }
    else { out.push_back((char)(0xF0 | (cp >> 18))); out.push_back((char)(0x80 | ((cp >> 12) & 0x3F))); out.push_back((char)(0x80 | ((cp >> 6) & 0x3F))); out.push_back((char)(0x80 | (cp & 0x3F))); }
}

#ifdef _WIN32
// The few ICU4C entry points used (C API, stable across ICU versions in icu.dll).
using UErrorCode = int;
using UChar = wchar_t;
using FnCharFromName = int32_t(__cdecl*)(int nameChoice, const char* name, UErrorCode* err);
using FnCharType = int8_t(__cdecl*)(int32_t c);
using FnHasBinaryProperty = int8_t(__cdecl*)(int32_t c, int which);
using FnGetNFKC = const void*(__cdecl*)(UErrorCode* err);
using FnNormalize = int32_t(__cdecl*)(const void* norm2, const UChar* src, int32_t length, UChar* dest, int32_t capacity, UErrorCode* err);

struct Icu {
    bool ok = false;
    FnCharFromName charFromName = nullptr;
    FnCharType charType = nullptr;
    FnHasBinaryProperty hasBinaryProperty = nullptr;
    const void* nfkc = nullptr;
    FnNormalize normalize = nullptr;
};

const Icu& icu() {
    static Icu I;
    static std::once_flag once;
    std::call_once(once, [] {
        HMODULE h = LoadLibraryExW(L"icu.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if (!h) return;
        I.charFromName = (FnCharFromName)(void*)GetProcAddress(h, "u_charFromName");
        I.charType = (FnCharType)(void*)GetProcAddress(h, "u_charType");
        I.hasBinaryProperty = (FnHasBinaryProperty)(void*)GetProcAddress(h, "u_hasBinaryProperty");
        auto getNfkc = (FnGetNFKC)(void*)GetProcAddress(h, "unorm2_getNFKCInstance");
        I.normalize = (FnNormalize)(void*)GetProcAddress(h, "unorm2_normalize");
        if (getNfkc) {
            UErrorCode e = 0;
            I.nfkc = getNfkc(&e);
            if (e > 0) I.nfkc = nullptr;
        }
        I.ok = I.charFromName && I.charType && I.hasBinaryProperty && I.nfkc && I.normalize;
    });
    return I;
}

constexpr int U_UNICODE_CHAR_NAME = 0;
constexpr int U_CHAR_NAME_ALIAS = 3;
constexpr int UCHAR_XID_CONTINUE = 32;
constexpr int UCHAR_XID_START = 33;
#endif

}  // namespace

bool available() {
#ifdef _WIN32
    return icu().ok;
#else
    return false;
#endif
}

bool lookup_name(std::string_view name, uint32_t& cp) {
#ifdef _WIN32
    if (!available()) return false;
    std::string up;
    for (char c : name) up.push_back((char)std::toupper((unsigned char)c));
    for (int choice : {U_UNICODE_CHAR_NAME, U_CHAR_NAME_ALIAS}) {
        UErrorCode e = 0;
        int32_t c = icu().charFromName(choice, up.c_str(), &e);
        if (e <= 0 && c >= 0) { cp = (uint32_t)c; return true; }
    }
#else
    (void)name; (void)cp;
#endif
    return false;
}

int printable(uint32_t cp) {
#ifdef _WIN32
    if (!available()) return -1;
    if (cp == ' ') return 1;
    switch (icu().charType((int32_t)cp)) {
    case 0:   // Cn
    case 12:  // Zs
    case 13:  // Zl
    case 14:  // Zp
    case 15:  // Cc
    case 16:  // Cf
    case 17:  // Co
    case 18:  // Cs
        return 0;
    default:
        return 1;
    }
#else
    (void)cp;
    return -1;
#endif
}

int xid_start(uint32_t cp) {
#ifdef _WIN32
    if (!available()) return -1;
    return icu().hasBinaryProperty((int32_t)cp, UCHAR_XID_START) ? 1 : 0;
#else
    (void)cp;
    return -1;
#endif
}

int xid_continue(uint32_t cp) {
#ifdef _WIN32
    if (!available()) return -1;
    return icu().hasBinaryProperty((int32_t)cp, UCHAR_XID_CONTINUE) ? 1 : 0;
#else
    (void)cp;
    return -1;
#endif
}

bool nfkc(std::string_view utf8, std::string& out) {
    out.assign(utf8);
#ifdef _WIN32
    if (!available()) return false;
    std::wstring w;
    for (uint32_t cp : decode(utf8)) {
        if (cp >= 0x10000) {
            w.push_back((wchar_t)(0xD800 + ((cp - 0x10000) >> 10)));
            w.push_back((wchar_t)(0xDC00 + ((cp - 0x10000) & 0x3FF)));
        } else {
            w.push_back((wchar_t)cp);
        }
    }
    std::wstring r(w.size() * 4 + 16, L'\0');
    UErrorCode e = 0;
    int32_t n = icu().normalize(icu().nfkc, w.data(), (int32_t)w.size(), r.data(), (int32_t)r.size(), &e);
    if (e > 0) return false;
    out.clear();
    for (int32_t i = 0; i < n; ++i) {
        uint32_t c = (uint16_t)r[(size_t)i];
        if (c >= 0xD800 && c < 0xDC00 && i + 1 < n) {
            uint32_t lo = (uint16_t)r[(size_t)i + 1];
            if (lo >= 0xDC00 && lo < 0xE000) { c = 0x10000 + ((c - 0xD800) << 10) + (lo - 0xDC00); ++i; }
        }
        encode(out, c);
    }
    return true;
#else
    return false;
#endif
}

bool to_utf8(std::string_view encoding, std::string_view bytes, std::string& out) {
    std::string key;
    for (char c : encoding)
        if (std::isalnum((unsigned char)c)) key.push_back((char)std::tolower((unsigned char)c));
    if (key == "latin1" || key == "iso88591" || key == "isolatin1" || key == "l1" || key == "8859" || key == "cp819") {
        out.clear();
        for (char c : bytes) encode(out, (unsigned char)c);
        return true;
    }
    if (key == "utf8" || key == "utf8sig") { out.assign(bytes); return true; }
#ifdef _WIN32
    struct CP { const char* name; UINT cp; };
    static const CP table[] = {
        {"ascii", 20127}, {"usascii", 20127}, {"cp1250", 1250}, {"windows1250", 1250}, {"cp1251", 1251}, {"windows1251", 1251},
        {"cp1252", 1252}, {"windows1252", 1252}, {"cp1253", 1253}, {"windows1253", 1253}, {"cp1254", 1254}, {"windows1254", 1254},
        {"cp1255", 1255}, {"windows1255", 1255}, {"cp1256", 1256}, {"windows1256", 1256}, {"cp1257", 1257}, {"windows1257", 1257},
        {"cp1258", 1258}, {"windows1258", 1258}, {"koi8r", 20866}, {"koi8u", 21866}, {"cp437", 437}, {"cp850", 850},
        {"cp852", 852}, {"cp855", 855}, {"cp857", 857}, {"cp860", 860}, {"cp861", 861}, {"cp862", 862}, {"cp863", 863},
        {"cp865", 865}, {"cp866", 866}, {"cp869", 869}, {"cp874", 874}, {"iso88592", 28592}, {"iso88593", 28593},
        {"iso88594", 28594}, {"iso88595", 28595}, {"iso88596", 28596}, {"iso88597", 28597}, {"iso88598", 28598},
        {"iso88599", 28599}, {"iso885913", 28603}, {"iso885915", 28605}, {"latin2", 28592}, {"latin9", 28605},
        {"cyrillic", 28595}, {"greek", 28597}, {"hebrew", 28598}, {"shiftjis", 932}, {"sjis", 932}, {"cp932", 932},
        {"eucjp", 20932}, {"gbk", 936}, {"gb2312", 936}, {"cp936", 936}, {"gb18030", 54936}, {"big5", 950}, {"cp950", 950},
        {"euckr", 51949}, {"cp949", 949}, {"macroman", 10000}, {"maccyrillic", 10007},
    };
    for (const CP& c : table) {
        if (key != c.name) continue;
        out.clear();
        if (bytes.empty()) return true;
        int n = MultiByteToWideChar(c.cp, 0, bytes.data(), (int)bytes.size(), nullptr, 0);
        if (n <= 0) return false;
        std::wstring w((size_t)n, L'\0');
        MultiByteToWideChar(c.cp, 0, bytes.data(), (int)bytes.size(), w.data(), n);
        int m = WideCharToMultiByte(CP_UTF8, 0, w.data(), n, nullptr, 0, nullptr, nullptr);
        out.resize((size_t)m);
        WideCharToMultiByte(CP_UTF8, 0, w.data(), n, out.data(), m, nullptr, nullptr);
        return true;
    }
#endif
    return false;
}

}  // namespace pyfront::uni
