// cb/ui/progress_store.cpp -- see progress_store.hpp.
#include "cb/ui/progress_store.hpp"

#include <algorithm>
#include <memory>

#include "cb/engine/function_store.hpp"

namespace cb::ui::progress {

ReadFn store_reader(const coverage::ledger::Services& services) {
    return [services](Clock::time_point now) {
        StoreReading reading;
        reading.store_path = services.store_index.generic_string();
        std::error_code ec;
        std::unique_ptr<engine::FunctionStore> store;
        if (std::filesystem::exists(services.store_index, ec))
            store = std::make_unique<engine::FunctionStore>(services.store_index, services.store_folder, /*readonly*/ true);
        coverage::ledger::Ledger ledger(services, store.get());
        const coverage::ledger::Report report = ledger.measure();
        for (const coverage::ledger::Summary& s : report.kinds) {
            const auto profile = report.profiles.find(s.kind);
            if (profile == report.profiles.end() || !profile->second || profile->second->empty()) continue;
            KindReading k;
            k.kind = s.kind;
            k.name = s.name;
            k.modules = static_cast<std::uint64_t>(s.modules);
            k.measured = static_cast<std::uint64_t>(s.measured);
            k.functions = static_cast<std::uint64_t>(s.functions);
            for (const verify::Class c : verify::kClassOrder)
                k.classes.push_back({std::string(verify::word(c)), std::string(verify::title(c)),
                                     static_cast<std::uint64_t>(s.classes.at(c))});
            reading.kinds.push_back(std::move(k));
        }
        if (!store) return reading;
        reading.opened = true;
        for (const engine::RecentModule& m : store->recent(kRecentModules)) {
            RecentResult r;
            r.module = m.name;
            r.kind = std::string(coverage::ledger::kind_title(m.kind).empty() ? std::string_view(m.kind) : coverage::ledger::kind_title(m.kind));
            r.results = static_cast<std::uint64_t>(m.judged);
            r.last_written = Clock::time_point(std::chrono::duration_cast<Clock::duration>(std::chrono::duration<double>(m.last)));
            reading.recent.push_back(std::move(r));
        }
        const double since = std::chrono::duration<double>(now.time_since_epoch()).count() - static_cast<double>(kWindow.count());
        reading.fresh = static_cast<std::uint64_t>(std::max<std::int64_t>(0, store->verdicts_since(since)));
        store.reset();
        return reading;
    };
}

}  // namespace cb::ui::progress
