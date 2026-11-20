// symbols/driver_services_command.cpp -- the `driver-services` command over the table (driver_services.hpp).
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "formats/le.hpp"
#include "symbols/driver_services.hpp"

namespace cb::symbols::driver_services {

namespace {

// {device number: name} as every driver of the corpus states them in its own descriptor (the first to say a number names it).
std::map<std::uint32_t, std::string> drivers_own_names(const std::vector<std::filesystem::path>& files) {
    std::map<std::uint32_t, std::string> out;
    for (const std::filesystem::path& file : files) {
        try {
            for (const auto& [name, module] : formats::le::open_modules(file)) {
                const std::optional<formats::le::Descriptor> d = module.descriptor();
                if (d && d->device_id != 0 && !d->name.empty()) out.emplace(d->device_id, d->name);
            }
        } catch (const Error&) {
            continue;   // a file that is not a driver says nothing
        }
    }
    return out;
}

// The service calls of every module a file holds.
std::vector<ServiceCall> calls_of(const std::filesystem::path& file) {
    std::vector<ServiceCall> out;
    for (const auto& [name, module] : formats::le::open_modules(file))
        for (const formats::le::ServiceCall& c : module.service_calls()) out.push_back({c.device, c.service});
    return out;
}

int generate_table(const Environment& env, const std::optional<std::string>& folder_given) {
    const std::filesystem::path folder = folder_given ? fs::from_text(*folder_given) : find_include_dir(env.kit_folder, env.reference_folder);
    if (folder.empty()) {
        err("the driver kit's include files were not found; set CB_DRIVER_KIT to its INC32 folder\n");
        return kExitFailed;
    }
    const KitTables kit = read_include_files(folder);
    const std::map<std::uint32_t, std::string> drivers = env.corpus_drivers ? drivers_own_names(env.corpus_drivers()) : std::map<std::uint32_t, std::string>{};
    fs::write_text(env.table_file, format_table(kit, drivers, read_messages(folder)));
    const Table::Size size = env.table().size();
    out("{}: {} service tables, {} services, {} device numbers\n", fs::to_text(env.table_file), size.service_tables, size.services_named,
        size.devices_named);
    return kExitOk;
}

}  // namespace

cli::Command command(const Environment& environment) {
    cli::Command c;
    c.name = "driver-services";
    c.summary = "the real names of the services ring-0 drivers call; for drivers, how many of their service calls it names";
    c.params = {
        cli::Option{"--generate", "", "make the table again from the driver kit's include files (the first word after it is the folder)", cli::Arg::Flag},
        cli::Positional{"files", "driver files (or corpus names), or with --generate the kit's INC32 folder", false, true},
    };
    c.handler = [environment](const cli::Args& a) {
        const std::vector<std::string> words = a.values("files");
        if (a.flag("generate")) return generate_table(environment, words.empty() ? std::nullopt : std::optional<std::string>(words.front()));
        const Table table = environment.table();
        out_text(size_line(table.size()) + "\n");
        for (const std::string& word : words) {
            const std::optional<std::filesystem::path> path = environment.retail ? environment.retail(word) : std::nullopt;
            if (!path) {
                out("{}: not found\n", word);
                continue;
            }
            const std::vector<ServiceCall> calls = calls_of(*path);
            out_text(coverage_line(table, fs::to_text(path->filename()), coverage(table, calls)) + "\n");
        }
        return kExitOk;
    };
    return c;
}

Table table_of(knowledge::Store& store) {
    return Table::from_rows(store.authored_table({"vxd", "device"}), store.authored_table({"vxd", "service_table"}),
                            store.authored_table({"vxd", "driver"}), store.authored_table({"vxd", "message"}));
}

}  // namespace cb::symbols::driver_services
