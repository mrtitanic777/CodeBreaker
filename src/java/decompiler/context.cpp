// java/decompiler/context.cpp -- see context.hpp. Ported from Fernflower (DecompilerContext, IFernflowerPreferences), Copyright 2000-2015 JetBrains
// s.r.o., Apache License 2.0.
#include "java/decompiler/context.hpp"

#include <stdexcept>

namespace cb::java::decompiler {

namespace {
thread_local DecompilerContext* g_dc_current = nullptr;
}

std::map<std::string, std::string> pref::defaults() {
    std::map<std::string, std::string> d;
    for (const auto& [k, v] : std::initializer_list<std::pair<std::string_view, std::string_view>>{
             {REMOVE_BRIDGE, "1"}, {REMOVE_SYNTHETIC, "0"}, {DECOMPILE_INNER, "1"}, {DECOMPILE_CLASS_1_4, "1"}, {DECOMPILE_ASSERTIONS, "1"},
             {HIDE_EMPTY_SUPER, "1"}, {HIDE_DEFAULT_CONSTRUCTOR, "1"}, {DECOMPILE_GENERIC_SIGNATURES, "0"}, {NO_EXCEPTIONS_RETURN, "1"},
             {DECOMPILE_ENUM, "1"}, {REMOVE_GET_CLASS_NEW, "1"}, {LITERALS_AS_IS, "0"}, {BOOLEAN_TRUE_ONE, "1"}, {ASCII_STRING_CHARACTERS, "0"},
             {SYNTHETIC_NOT_SET, "1"}, {UNDEFINED_PARAM_TYPE_OBJECT, "1"}, {USE_DEBUG_VAR_NAMES, "1"}, {REMOVE_EMPTY_RANGES, "1"},
             {FINALLY_DEINLINE, "1"}, {IDEA_NOT_NULL_ANNOTATION, "1"}, {LAMBDA_TO_ANONYMOUS_CLASS, "0"}, {BYTECODE_SOURCE_MAPPING, "0"},
             {USE_DEBUG_LINE_NUMBERS, "0"}, {NO_COMMENT_OUTPUT, "0"}, {OVERRIDE_ANNOTATION, "0"}, {LOG_LEVEL, "INFO"}, {MAX_PROCESSING_METHOD, "0"},
             {RENAME_ENTITIES, "0"},
#ifdef _WIN32
             {NEW_LINE_SEPARATOR, "0"},
#else
             {NEW_LINE_SEPARATOR, "1"},
#endif
             {INDENT_STRING, "   "}, {BANNER, ""}, {UNIT_TEST_MODE, "0"}, {DUMP_ORIGINAL_LINES, "0"}})
        d.emplace(k, v);
    return d;
}

DecompilerContext& DecompilerContext::current() {
    if (g_dc_current == nullptr) throw std::logic_error("no decompiler context on this thread");
    return *g_dc_current;
}

bool DecompilerContext::option(std::string_view key) {
    const auto& p = current().properties;
    const auto it = p.find(std::string(key));
    return it != p.end() && it->second == "1";
}

std::string DecompilerContext::property(std::string_view key) {
    const auto& p = current().properties;
    const auto it = p.find(std::string(key));
    return it == p.end() ? std::string() : it->second;
}

void DecompilerContext::log(const std::string& message) {
    if (g_dc_current != nullptr && g_dc_current->logger) g_dc_current->logger(message);
}

DecompilerScope::DecompilerScope(DecompilerContext context) : context_(std::move(context)), previous_(g_dc_current) { g_dc_current = &context_; }

DecompilerScope::~DecompilerScope() { g_dc_current = previous_; }

}  // namespace cb::java::decompiler
