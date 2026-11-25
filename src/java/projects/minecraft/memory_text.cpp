// java/projects/minecraft/memory_text.cpp -- see memory_text.hpp.
#include "java/projects/minecraft/memory_text.hpp"

#include <cstdio>
#include <regex>

namespace cb::java::projects::minecraft::memory_text {

namespace {

// The helper a rewritten file defines once: the same rule as size_text.
constexpr std::string_view kHelper = R"cpp(
// (CodeBreaker: Minecraft's memory line in gigabytes when there are that many; the JVM's heap was megabytes)
#include <cstdio>
static JavaString j2xMemorySize(long long bytes) {
	char text[32];
	const long long mb = bytes / 1024LL / 1024LL;
	if (mb < 1024LL) std::snprintf(text, sizeof text, "%lldMB", mb);
	else if (mb < 100LL * 1024LL) std::snprintf(text, sizeof text, "%.1fGB", (double)mb / 1024.0);
	else std::snprintf(text, sizeof text, "%lldGB", mb / 1024LL);
	return JavaString(text);
}
)cpp";

}  // namespace

std::string size_text(long long bytes) {
    char text[32];
    const long long mb = bytes / 1024LL / 1024LL;
    if (mb < 1024LL) std::snprintf(text, sizeof text, "%lldMB", mb);
    else if (mb < 100LL * 1024LL) std::snprintf(text, sizeof text, "%.1fGB", static_cast<double>(mb) / 1024.0);
    else std::snprintf(text, sizeof text, "%lldGB", mb / 1024LL);
    return text;
}

std::string rewrite(std::string_view text_in) {
    static const std::regex kMegabytes(R"(([A-Za-z_][\w>\-\.]*(?:\(\))?) / 1024LL / 1024LL \+ "MB)");
    std::string text(text_in);
    if (text.find("j2xMemorySize(") != std::string::npos || !std::regex_search(text, kMegabytes)) return text;
    text = std::regex_replace(text, kMegabytes, "j2xMemorySize($1) + \"");
    // the helper after the last #include (the file's own types, JavaString among them, are declared by then)
    std::size_t at = 0;
    for (std::size_t pos = text.find("#include"); pos != std::string::npos; pos = text.find("#include", pos + 1))
        if (pos == 0 || text[pos - 1] == '\n') at = pos;
    const std::size_t eol = text.find('\n', at);
    const std::size_t insert = eol == std::string::npos ? text.size() : eol + 1;
    text.insert(insert, kHelper);
    return text;
}

}  // namespace cb::java::projects::minecraft::memory_text
