// strings.h -- a module's strings, who uses them, strings built on the stack, format strings
// (strings.cc; was the core of codebreaker/analyze/strings.py).
#pragma once
#include <string>
#include <vector>

#include "image.h"

namespace cba {

// what: "table", "uses", "stack" or "formats"; the answer as JSON in out -> false: unknown
bool strings_answer(Image &img, const char *what, std::string &out);

// the strings table (built once per image unless asked again), and the entry at an address
void ensure_table(Image &img);
const Image::Str *in_table(const Image &img, int64_t va);

std::string fam(const std::string &r);   // image.family

// an argument as image.arguments gives it: an operand, ("lea", ...), ("call", target or slot)
struct Arg {
    char kind = 0;          // 'r', 'i', 'm', 'l' (lea), 'c' (call)
    std::string reg;
    int64_t value = 0;
    bool has_value = false; // a call's target (or the slot it calls through) is known
};
// image.definition / image.arguments
bool definition(const Function &fn, int i, const std::string &reg, int depth, Arg &out);
std::vector<Arg> arguments(const Function &fn, int i, size_t n);

}  // namespace cba
