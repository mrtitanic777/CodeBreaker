// emulate/cpu/pcode_ops.cpp -- see pcode_ops.hpp.
#include "emulate/cpu/pcode_ops.hpp"

#include <stdexcept>
#include <unordered_set>

namespace cb::emulate::pcode_ops {

Table::Table(std::initializer_list<Entry> entries) : entries_(entries) {
    for (std::size_t i = 0; i < entries_.size(); ++i) {
        if (entries_[i].handler == nullptr) throw std::logic_error("pcode_ops: no handler for " + std::string(entries_[i].name));
        if (!index_.emplace(entries_[i].name, i).second) throw std::logic_error("pcode_ops: operation listed twice: " + std::string(entries_[i].name));
    }
}

const Handler* Table::find(std::string_view name) const {
    const auto it = index_.find(name);
    return it == index_.end() ? nullptr : &entries_[it->second].handler;
}

Binding bind(Emu& emu, const Table& table, void* context) {
    Binding result;
    std::unordered_set<std::string_view> seen;
    const std::size_t count = emu.userOpCount();
    for (std::size_t i = 0; i < count; ++i) {
        const std::string& name = emu.userOpName(i);
        seen.insert(name);
        if (const Handler* h = table.find(name)) {
            emu.bindUserOp(i, *h, context);
            ++result.bound;
        } else {
            result.unbound.push_back(name);
        }
    }
    for (const Entry& e : table.entries())
        if (!seen.count(e.name)) result.unknown.push_back(std::string(e.name));
    return result;
}

}  // namespace cb::emulate::pcode_ops
