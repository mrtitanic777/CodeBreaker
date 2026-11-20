// java/decompiler/context.hpp -- the decompiler's per-run state, reachable from every pass as the original's thread-local DecompilerContext is: the
// options (by their short keys, "rer", "din" ..., with the defaults the original has), the logger, and the run's struct context.
//
//     DecompilerScope run(DecompilerContext{.properties = options});   // current for this thread until `run` ends
//     DecompilerContext::option(pref::REMOVE_EMPTY_RANGES);                     // true: "1"
//
// Ported from Fernflower (de.fernflower.main.DecompilerContext, main.extern.IFernflowerPreferences), Copyright 2000-2015 JetBrains s.r.o., Apache
// License 2.0 (see NOTICE in this folder). The rest of the original's context (the class processor, the import and variable-name collectors, the
// counters, the bytecode mapper) joins as the layers that own it are ported.
#pragma once

#include <functional>
#include <map>
#include <string>
#include <string_view>

namespace cb::java::decompiler {

class StructContext;

namespace pref {
inline constexpr std::string_view REMOVE_BRIDGE = "rbr", REMOVE_SYNTHETIC = "rsy", DECOMPILE_INNER = "din", DECOMPILE_CLASS_1_4 = "dc4",
                                  DECOMPILE_ASSERTIONS = "das", HIDE_EMPTY_SUPER = "hes", HIDE_DEFAULT_CONSTRUCTOR = "hdc", DECOMPILE_GENERIC_SIGNATURES = "dgs",
                                  NO_EXCEPTIONS_RETURN = "ner", DECOMPILE_ENUM = "den", REMOVE_GET_CLASS_NEW = "rgn", LITERALS_AS_IS = "lit",
                                  BOOLEAN_TRUE_ONE = "bto", ASCII_STRING_CHARACTERS = "asc", SYNTHETIC_NOT_SET = "nns", UNDEFINED_PARAM_TYPE_OBJECT = "uto",
                                  USE_DEBUG_VAR_NAMES = "udv", REMOVE_EMPTY_RANGES = "rer", FINALLY_DEINLINE = "fdi", IDEA_NOT_NULL_ANNOTATION = "inn",
                                  LAMBDA_TO_ANONYMOUS_CLASS = "lac", BYTECODE_SOURCE_MAPPING = "bsm", USE_DEBUG_LINE_NUMBERS = "udl",
                                  NO_COMMENT_OUTPUT = "nco", OVERRIDE_ANNOTATION = "ovr", LOG_LEVEL = "log", MAX_PROCESSING_METHOD = "mpm",
                                  RENAME_ENTITIES = "ren", USER_RENAMER_CLASS = "urc", NEW_LINE_SEPARATOR = "nls", INDENT_STRING = "ind", BANNER = "ban",
                                  DUMP_ORIGINAL_LINES = "__dump_original_lines__", UNIT_TEST_MODE = "__unit_test_mode__";
// The defaults (the new-line separator: "0" on Windows, "1" elsewhere, as the original decides).
std::map<std::string, std::string> defaults();
}  // namespace pref

struct DecompilerContext {
    std::map<std::string, std::string> properties = pref::defaults();
    std::function<void(const std::string& message)> logger;
    StructContext* struct_context = nullptr;

    // The context current on this thread; std::logic_error when there is none.
    static DecompilerContext& current();
    // The option is "1".
    static bool option(std::string_view key);
    static std::string property(std::string_view key);
    static std::string new_line_separator() { return option(pref::NEW_LINE_SEPARATOR) ? "\n" : "\r\n"; }
    static void log(const std::string& message);
};

// Makes a context current on this thread for its lifetime (the one before it comes back after).
class DecompilerScope {
public:
    explicit DecompilerScope(DecompilerContext context);
    ~DecompilerScope();
    DecompilerScope(const DecompilerScope&) = delete;
    DecompilerScope& operator=(const DecompilerScope&) = delete;
    DecompilerContext& context() { return context_; }

private:
    DecompilerContext context_;
    DecompilerContext* previous_;
};

}  // namespace cb::java::decompiler
