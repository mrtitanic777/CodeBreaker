// platforms/described.cpp -- the machines described as data (package.hpp: described()): config/platforms/<name>.json, the one file a console
// is described in, holds the machine's facts (its "machine" section) beside what its source writer needs.

#include <format>
#include <map>
#include <mutex>
#include <nlohmann/json.hpp>

#include "base/config.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "platforms/package.hpp"

namespace cb::platforms {

namespace {

constexpr std::pair<std::string_view, OwnDecoder> kOwnDecoders[] = {
    {"z80", OwnDecoder::Z80},         {"sm83", OwnDecoder::Sm83},     {"m6502", OwnDecoder::M6502},     {"m68k", OwnDecoder::M68k},
    {"mips", OwnDecoder::Mips},       {"arm", OwnDecoder::Arm},       {"jar", OwnDecoder::Jar},         {"powerpc", OwnDecoder::PowerPc},
    {"huc6280", OwnDecoder::Huc6280}, {"w65816", OwnDecoder::W65816}, {"sh2", OwnDecoder::Sh2},         {"sh4", OwnDecoder::Sh4},
    {"m6809", OwnDecoder::M6809},     {"v810", OwnDecoder::V810},     {"cp1610", OwnDecoder::Cp1610},   {"i8048", OwnDecoder::I8048},
    {"f8", OwnDecoder::F8},           {"tlcs900", OwnDecoder::Tlcs900}, {"a64", OwnDecoder::A64},
};

OwnDecoder own_decoder(std::string_view name, std::string_view file) {
    for (const auto& [n, d] : kOwnDecoders) {
        if (n == name) return d;
    }
    throw UsageError(std::format("{}: no decoder {}", file, name));
}

std::vector<std::string> strings(const nlohmann::ordered_json& j, const char* key) {
    std::vector<std::string> out;
    if (const auto it = j.find(key); it != j.end()) {
        for (const auto& s : *it) out.push_back(s.get<std::string>());
    }
    return out;
}

Platform read_described(std::string_view name) {
    const std::string file_name = std::format("config/platforms/{}.json", name);
    const std::filesystem::path file = config::current().config_root() / "platforms" / (std::string(name) + ".json");
    if (!std::filesystem::exists(file)) throw UsageError(std::format("no platform {} ({})", name, file_name));
    const nlohmann::ordered_json j = nlohmann::ordered_json::parse(fs::read_text(file));
    const auto m = j.find("machine");
    if (m == j.end()) throw UsageError(std::format("{} has no \"machine\" section", file_name));
    MachineSpec s;
    s.name = std::string(name);
    s.title = j.value("title", std::string());
    s.family = m->value("family", std::string());
    s.container = m->value("container", std::string());
    const nlohmann::ordered_json& cpu = m->at("cpu");
    s.cpu.language = cpu.value("language", std::string());
    if (cpu.contains("bits")) s.cpu.bits = cpu.at("bits").get<unsigned>();
    if (cpu.contains("endian")) s.cpu.endian = cpu.at("endian").get<std::string>() == "big" ? Endian::Big : Endian::Little;
    if (cpu.contains("decoder")) s.cpu.decoder = own_decoder(cpu.at("decoder").get<std::string>(), file_name);
    s.cpu.registers = strings(cpu, "registers");
    s.cpu.stack = cpu.value("stack", std::string());
    s.results = strings(*m, "results");
    if (m->contains("toolchain")) s.toolchain = m->at("toolchain").get<std::string>();
    return machine(s);
}

}  // namespace

Platform described(std::string_view name) {
    static std::mutex lock;
    static std::map<std::string, Platform, std::less<>> cache;
    std::lock_guard hold(lock);
    if (const auto it = cache.find(name); it != cache.end()) return it->second;
    Platform p = read_described(name);
    cache.emplace(std::string(name), p);
    return p;
}

}  // namespace cb::platforms
