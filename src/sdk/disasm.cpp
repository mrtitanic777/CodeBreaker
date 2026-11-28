// sdk/disasm.cpp -- disasm.h over the one disassembler: every processor of source/processors, read through its SLEIGH description.
#include <algorithm>
#include <format>
#include <string>
#include <vector>

#include "core/error.hpp"
#include "core/text.hpp"
#include "sdk/runtime.hpp"
#include "source/processors.hpp"

struct cb_instructions {
    std::vector<cb::decompile::sleigh::Instruction> read;   // the reader's own instructions; `list` points into their text
    std::vector<cb_instruction> list;
};

namespace cb::sdk_impl {
namespace {

namespace processors = source::processors;

// The processor table as C records, made once (the table's names are views of literals; these are NUL-terminated copies).
struct ArchTable {
    struct Row {
        std::string name, aliases, summary;
    };
    std::vector<Row> rows;
    std::vector<cb_arch> list;

    ArchTable() {
        const std::span<const processors::Processor> all = processors::all();
        rows.reserve(all.size());
        for (const processors::Processor& p : all) {
            Row row{std::string(p.name), {}, std::string(p.summary)};
            for (const std::string_view alias : p.aliases) {
                if (!row.aliases.empty()) row.aliases += ' ';
                row.aliases += alias;
            }
            rows.push_back(std::move(row));
        }
        list.reserve(all.size());
        for (std::size_t i = 0; i < all.size(); ++i)
            list.push_back(cb_arch{rows[i].name.c_str(), rows[i].aliases.c_str(), rows[i].summary.c_str(), all[i].big_endian ? 1u : 0u,
                                   all[i].sla.empty() ? 0u : 1u});
    }
};

const ArchTable& arch_table() {
    static const ArchTable table;
    return table;
}

std::string arch_names() {
    std::string names;
    for (const cb_arch& a : arch_table().list) names += std::format("{}{}", names.empty() ? "" : ", ", a.name);
    return names;
}

cb_flow flow_of(const decompile::sleigh::Instruction& i) {
    if (i.length == 0) return CB_FLOW_INVALID;
    switch (i.flow) {
        case decompile::sleigh::Flow::Next: return CB_FLOW_NEXT;
        case decompile::sleigh::Flow::Branch: return CB_FLOW_BRANCH;
        case decompile::sleigh::Flow::Jump: return CB_FLOW_JUMP;
        case decompile::sleigh::Flow::Call: return CB_FLOW_CALL;
        case decompile::sleigh::Flow::CallIndirect: return CB_FLOW_CALL_INDIRECT;
        case decompile::sleigh::Flow::Return: return CB_FLOW_RETURN;
        case decompile::sleigh::Flow::Indirect: return CB_FLOW_INDIRECT;
    }
    return CB_FLOW_NEXT;
}

}  // namespace
}  // namespace cb::sdk_impl

using namespace cb::sdk_impl;

extern "C" {

const cb_arch* cb_arch_list(size_t* count) {
    try {
        const ArchTable& t = arch_table();
        if (count != nullptr) *count = t.list.size();
        return t.list.data();
    } catch (...) {
        if (count != nullptr) *count = 0;
        from_exception("cb_arch_list");
        return nullptr;
    }
}

cb_status cb_arch_find(const char* name, const cb_arch** out) {
    if (out != nullptr) *out = nullptr;
    return guarded("cb_arch_find", [&]() -> cb_status {
        if (name == nullptr || out == nullptr) return fail(CB_ERROR_INVALID_ARGUMENT, "cb_arch_find", "the name or the out pointer is NULL");
        const cb::source::processors::Processor* p = cb::source::processors::lookup(name);
        if (p == nullptr) return fail(CB_ERROR_NOT_FOUND, "cb_arch_find", std::format("no processor is called {}; the names are: {}", name, arch_names()));
        const std::span<const cb::source::processors::Processor> all = cb::source::processors::all();
        *out = &arch_table().list[static_cast<std::size_t>(p - all.data())];
        return CB_OK;
    });
}

cb_status cb_disasm(const char* arch, const void* code, size_t size, uint64_t address, size_t count, cb_instructions** out) {
    if (out != nullptr) *out = nullptr;
    return guarded("cb_disasm", [&]() -> cb_status {
        if (const cb_status s = require_init("cb_disasm"); s != CB_OK) return s;
        if (arch == nullptr || out == nullptr || (code == nullptr && size != 0))
            return fail(CB_ERROR_INVALID_ARGUMENT, "cb_disasm", "the processor's name, the out pointer or the bytes (with a size) is NULL");
        const cb::source::processors::Processor* p = cb::source::processors::lookup(arch);
        if (p == nullptr) return fail(CB_ERROR_NOT_FOUND, "cb_disasm", std::format("no processor is called {}; the names are: {}", arch, arch_names()));
        if (p->sla.empty())
            return fail(CB_ERROR_UNSUPPORTED, "cb_disasm",
                        std::format("{} has no SLEIGH description yet, so it cannot be read here (cb_arch_list: `described`)", p->name));
        const cb::ByteView bytes(static_cast<const std::uint8_t*>(code), size);
        cb::source::processors::Described read;
        try {
            read = cb::source::processors::read_instructions(*p, bytes, address, count);
        } catch (const cb::NotFoundError& e) {
            return fail(CB_ERROR_IO, "cb_disasm",
                        std::format("{}'s description cannot be read ({}): the data root's languages folder is not complete", p->name, e.what()));
        }
        auto list = std::make_unique<cb_instructions>();
        list->read = std::move(read.instructions);
        list->list.reserve(list->read.size());
        for (const cb::decompile::sleigh::Instruction& i : list->read) {
            const std::size_t at = static_cast<std::size_t>((i.address - address) * read.unit);
            const std::size_t length = i.length != 0 ? i.length : std::min(read.step, size - std::min(at, size));
            list->list.push_back(cb_instruction{i.address, static_cast<std::uint32_t>(length), static_cast<std::uint32_t>(flow_of(i)), i.mnemonic.c_str(),
                                                i.operands.c_str(), i.target.value_or(0), i.target ? 1u : 0u});
        }
        *out = list.release();
        return CB_OK;
    });
}

const cb_instruction* cb_instructions_data(const cb_instructions* instructions, size_t* count) {
    if (count != nullptr) *count = instructions ? instructions->list.size() : 0;
    return instructions && !instructions->list.empty() ? instructions->list.data() : nullptr;
}

void cb_instructions_free(cb_instructions* instructions) { delete instructions; }

}  // extern "C"
