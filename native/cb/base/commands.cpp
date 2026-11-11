#include "cb/base/commands.hpp"

#include "cb/core/error.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"

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
