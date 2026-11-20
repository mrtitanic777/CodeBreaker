#include "base/commands.hpp"

#include "core/error.hpp"
#include "core/log.hpp"
#include "core/text.hpp"

namespace cb::base::commands {

int dispatch(const ToolTable& tools, std::span<const std::string> args, std::string_view usage) {
    const auto tool = args.empty() ? tools.end() : tools.find(args.front());
    if (tool == tools.end()) {
        std::string names;
        for (const auto& entry : tools) {
            if (!names.empty()) names += ", ";
            names += entry.first;
        }
        cb::out("{}\ntools: {}\n", usage, names);
        return kExitUsage;
    }
    return tool->second(args.subspan(1));
}

}  // namespace cb::base::commands
