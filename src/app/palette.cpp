// app/palette.cpp -- see palette.hpp.
#include "app/palette.hpp"

#include <algorithm>
#include <cctype>

namespace cb::app::palette {
namespace {

std::string pl_lower(std::string_view s) {
    std::string out(s);
    for (char& c : out) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return out;
}

std::vector<std::string> pl_words(std::string_view text) {
    std::vector<std::string> words;
    std::string w;
    for (const char c : text) {
        if (std::isspace(static_cast<unsigned char>(c))) {
            if (!w.empty()) words.push_back(std::move(w));
            w.clear();
        } else {
            w += c;
        }
    }
    if (!w.empty()) words.push_back(std::move(w));
    return words;
}

bool pl_word_char(char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0; }

// One lower-case word against lower-case text: 3 the text starts with it, 2 a word of the text does, 1 it is inside a word, 0 not found.
int pl_word_score(const std::string& word, const std::string& text) {
    if (word.empty()) return 1;
    if (text.compare(0, word.size(), word) == 0) return 3;
    int best = 0;
    for (std::size_t at = text.find(word); at != std::string::npos; at = text.find(word, at + 1)) {
        if (at == 0 || !pl_word_char(text[at - 1])) return 2;
        best = 1;
    }
    return best;
}

}  // namespace

int score(std::string_view query, std::string_view text) {
    const std::vector<std::string> words = pl_words(pl_lower(query));
    if (words.empty()) return 1;
    const std::string low = pl_lower(text);
    int total = 0;
    for (const std::string& w : words) {
        const int s = pl_word_score(w, low);
        if (s == 0) return 0;
        total += s;
    }
    return total * 10 / static_cast<int>(words.size());
}

std::vector<Item> make_items(const std::vector<tools::Tool>& tool_list, const std::vector<RecentEntry>& recent, bool developer) {
    std::vector<Item> items;
    for (std::size_t i = 0; i < tool_list.size(); ++i) {
        const tools::Tool& t = tool_list[i];
        if (t.developer && !developer) continue;   // (hidden unless "Show developer tools" is on)
        Item it;
        it.kind = Kind::Tool;
        it.title = t.title;
        it.note = t.group;
        it.search = std::string(t.command()) + " " + t.description;
        it.index = static_cast<int>(i);
        items.push_back(std::move(it));
    }
    for (std::size_t i = 0; i < recent.size(); ++i) {
        if (recent[i].missing) continue;
        Item it;
        it.kind = Kind::Recent;
        it.title = recent[i].name;
        it.note = recent[i].note;
        it.index = static_cast<int>(i);
        it.glyph_file = true;
        items.push_back(std::move(it));
    }
    const struct {
        const char* title;
        const char* hint;
        Place place;
    } places[] = {{"Convert", "Ctrl+1", Place::Convert},
                  {"Tools", "Ctrl+2", Place::Tools},
                  {"Settings", "Ctrl+3", Place::Settings},
                  {"Settings: Advanced", "Ctrl+,", Place::SettingsAdvanced}};
    for (const auto& p : places) {
        Item it;
        it.kind = Kind::GoTo;
        it.title = p.title;
        it.hint = p.hint;
        it.index = static_cast<int>(p.place);
        items.push_back(std::move(it));
    }
    return items;
}

Result search(const std::vector<Item>& items, std::string_view query) {
    Result result;
    struct Hit {
        int item, score;
    };
    const struct {
        Kind kind;
        const char* heading;
        std::size_t cap;
    } sections[] = {{Kind::Tool, "Tools", kMaxTools}, {Kind::Recent, "Recent", kMaxRecent}, {Kind::GoTo, "Go to", kMaxPlaces}};
    for (const auto& section : sections) {
        std::vector<Hit> hits;
        for (std::size_t i = 0; i < items.size(); ++i) {
            const Item& it = items[i];
            if (it.kind != section.kind) continue;
            // every word must be somewhere in the title or the other words; a match in the title ranks above one in the rest
            const int in_title = score(query, it.title);
            int s = in_title;
            if (s == 0) s = score(query, it.title + " " + it.note + " " + it.search);
            else s += 20;
            if (s > 0) hits.push_back({static_cast<int>(i), s});
        }
        std::stable_sort(hits.begin(), hits.end(), [](const Hit& a, const Hit& b) { return a.score > b.score; });
        if (hits.size() > section.cap) hits.resize(section.cap);
        if (hits.empty()) continue;
        Row heading;
        heading.heading = true;
        heading.text = section.heading;
        result.rows.push_back(std::move(heading));
        for (const Hit& h : hits) {
            Row row;
            row.item = h.item;
            result.rows.push_back(std::move(row));
            result.picks.push_back(h.item);
        }
    }
    return result;
}

int move(const Result& result, int from, int delta) {
    if (result.picks.empty()) return 0;
    return std::clamp(from + delta, 0, static_cast<int>(result.picks.size()) - 1);
}

}  // namespace cb::app::palette
