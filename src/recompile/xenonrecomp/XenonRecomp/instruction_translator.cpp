#include "recompile/xenonrecomp/XenonRecomp/instruction_translator.h"

namespace xenon_translate {
namespace {
thread_local Translator g_translator;
thread_local Counts g_counts;
}  // namespace

void set(Translator translator) { g_translator = std::move(translator); }
const Translator& current() { return g_translator; }
Counts& counts() { return g_counts; }

}  // namespace xenon_translate
