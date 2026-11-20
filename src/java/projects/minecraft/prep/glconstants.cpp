// GLConstants (constants.hpp): the OpenGL and Keyboard constants put where a method has their numbers, and the data it reads (gl_constants.json).
// Ported from RetroMCP (org.mcphackers.mcp.tools.injector.GLConstants, tools.JSONUtil), MIT License, (c) MCPHackers.
// cb-unity: alone (file-local helpers of the port)
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_set>

#include <nlohmann/json.hpp>

#include "java/decompiler/java_util.hpp"
#include "java/projects/minecraft/prep/constants.hpp"

namespace cb::java::minecraft::prep {

namespace {

using bc::InsnList;
using bc::InsnNode;
namespace op = bc::op;

struct StringHash {
    std::int32_t operator()(const std::string& s) const { return java_string_hash(s); }
};

constexpr char OPERATORS[] = {'|', '&', '^'};

// GLConstants.indexOf(char[], int, String)
int index_of(const char* ch, std::size_t ch_count, int from_index, const std::string& string) {
    const int max = static_cast<int>(string.size());
    if (from_index < 0) from_index = 0;
    else if (from_index >= max) return -1;
    for (int i = from_index; i < max; i++) {
        for (std::size_t k = 0; k < ch_count; ++k) {
            if (string[static_cast<std::size_t>(i)] == ch[k]) return i;
        }
    }
    return -1;
}

// String.trim: what is at or below a space is cut from both ends
std::string java_trim(const std::string& s) {
    std::size_t a = 0, b = s.size();
    while (a < b && static_cast<unsigned char>(s[a]) <= ' ') ++a;
    while (b > a && static_cast<unsigned char>(s[b - 1]) <= ' ') --b;
    return s.substr(a, b - a);
}

// Integer.parseInt of a key: an optional sign and decimal digits that fit an int (else the key is skipped)
std::optional<std::int32_t> parse_int(const std::string& s) {
    std::size_t i = 0;
    bool neg = false;
    if (i < s.size() && (s[i] == '-' || s[i] == '+')) {
        neg = s[i] == '-';
        ++i;
    }
    if (i >= s.size()) return std::nullopt;
    std::int64_t v = 0;
    for (; i < s.size(); ++i) {
        if (s[i] < '0' || s[i] > '9') return std::nullopt;
        v = v * 10 + (s[i] - '0');
        if (v > 2147483648LL) return std::nullopt;
    }
    if (neg) v = -v;
    if (v > 2147483647LL || v < -2147483648LL) return std::nullopt;
    return static_cast<std::int32_t>(v);
}

// JSONObject.optString(key, null): the text of a value (a string as it is, another value as its JSON text)
std::string opt_string(const nlohmann::ordered_json& v) { return v.is_string() ? v.get<std::string>() : v.dump(); }

// JSONUtil.toMap(JSONObject): the keys that are ints, with their values
std::unordered_map<std::int32_t, std::string> to_map(const nlohmann::ordered_json& object) {
    std::unordered_map<std::int32_t, std::string> map;
    if (!object.is_object()) return map;
    for (const auto& [key, value] : object.items()) {
        if (value.is_null()) continue;   // (optString of a JSON null is the text "null" in org.json; the data has none)
        if (const auto i = parse_int(key)) map[*i] = opt_string(value);
    }
    return map;
}

// JSONUtil.toList(JSONArray)
std::vector<std::string> to_list(const nlohmann::ordered_json& array) {
    std::vector<std::string> list;
    if (!array.is_array()) return list;
    for (const auto& v : array) {
        if (v.is_null()) continue;
        list.push_back(opt_string(v));
    }
    return list;
}

}  // namespace

// ---- GLConstantsData

GLConstantsData GLConstantsData::load(const std::filesystem::path& file) {
    std::ifstream in(file, std::ios::binary);
    if (!in) throw std::runtime_error("cannot read " + file.string());
    std::ostringstream text;
    text << in.rdbuf();
    return parse(text.str());
}

GLConstantsData GLConstantsData::parse(std::string_view json_text) {
    nlohmann::ordered_json json;
    try {
        json = nlohmann::ordered_json::parse(json_text);
    } catch (const nlohmann::json::exception& ex) {
        throw std::runtime_error(std::string("gl_constants.json: ") + ex.what());
    }
    if (!json.is_object()) throw std::runtime_error("gl_constants.json is not an object");
    GLConstantsData data;
    // CONSTANTS_KEYBOARD
    if (json.contains("CONSTANTS_KEYBOARD")) data.keyboard_ = to_map(json["CONSTANTS_KEYBOARD"]);
    // CONSTANTS
    if (json.contains("CONSTANTS") && json["CONSTANTS"].is_array()) {
        for (const auto& a : json["CONSTANTS"]) {
            if (!a.is_array() || a.size() < 2) continue;
            const auto& method_keys = a[0];
            const auto& method_values = a[1];
            if (!method_keys.is_object() || !method_values.is_object()) continue;

            Group group;
            for (const auto& [key, value] : method_keys.items()) {
                if (!value.is_array()) continue;
                group.methods[key] = to_list(value);
            }

            // org.json keeps an object's keys in a HashMap; keys() walks it, and the group's map is built in that order
            decompiler::JavaHashMap<std::string, nlohmann::ordered_json, StringHash> json_order;
            for (const auto& [key, value] : method_values.items()) json_order.put(key, value);
            decompiler::JavaHashMap<std::string, std::unordered_map<std::int32_t, std::string>, StringHash> map2;
            for (const auto& [key, value] : json_order.entries()) {
                if (!value.is_object()) continue;
                map2.put(key, to_map(value));
            }
            for (auto& entry : map2.entries()) group.values.push_back(std::move(entry));

            for (const auto& [key, unused] : group.methods) data.packages_.insert("org/lwjgl/opengl/" + key);
            data.constants_.push_back(std::move(group));
        }
    }
    return data;
}

InsnNode* GLConstantsData::get_keyboard_insn(InsnList& owner, std::int32_t constant) const {
    const auto it = keyboard_.find(constant);
    if (it == keyboard_.end()) return nullptr;
    return owner.make<bc::FieldInsnNode>(op::GETSTATIC, "org/lwjgl/input/Keyboard", it->second, "I");
}

std::optional<InsnList> GLConstantsData::get_gl_insn(const bc::MethodInsnNode& invoke, std::int32_t constant) const {
    const std::string pkg = invoke.owner.substr(17);   // invoke.owner.replace("org/lwjgl/opengl/", "")
    for (const Group& group : constants_) {
        const auto methods = group.methods.find(pkg);
        if (methods != group.methods.end() && std::find(methods->second.begin(), methods->second.end(), invoke.name) != methods->second.end()) {
            for (const auto& [key, value] : group.values) {
                const auto found = value.find(constant);
                if (found == value.end()) continue;
                const std::string& constant_value = found->second;

                InsnList instructions;
                int i = -1;
                do {
                    const char operator_ = i == -1 ? 0 : constant_value[static_cast<std::size_t>(i)];
                    const int index1 = i + 1;
                    i = index_of(OPERATORS, sizeof OPERATORS, index1, constant_value);
                    const int index2 = i == -1 ? static_cast<int>(constant_value.size()) : i;
                    // (substring throws for a start past the end; the data has none)
                    const std::string name = java_trim(constant_value.substr(static_cast<std::size_t>(index1), static_cast<std::size_t>(index2 - index1)));
                    instructions.add(instructions.make<bc::FieldInsnNode>(op::GETSTATIC, "org/lwjgl/opengl/" + key, name, "I"));
                    switch (operator_) {
                    case '|': instructions.add(instructions.make<bc::ZeroInsnNode>(op::IOR)); break;
                    case '&': instructions.add(instructions.make<bc::ZeroInsnNode>(op::IAND)); break;
                    case '^': instructions.add(instructions.make<bc::ZeroInsnNode>(op::IXOR)); break;
                    default: break;
                    }
                } while (i != -1);
                return instructions;
            }
        }
    }
    return std::nullopt;
}

// ---- GLConstants

bool GLConstants::is_icmp(int opcode) {
    switch (opcode) {
    case op::IF_ICMPEQ:
    case op::IF_ICMPGE:
    case op::IF_ICMPGT:
    case op::IF_ICMPLE:
    case op::IF_ICMPLT:
    case op::IF_ICMPNE:
        return true;
    }
    return false;
}

std::optional<std::int32_t> GLConstants::int_value(const InsnNode* insn) {
    switch (insn->get_opcode()) {
    case op::ICONST_0: return 0;
    case op::ICONST_1: return 1;
    case op::ICONST_2: return 2;
    case op::ICONST_3: return 3;
    case op::ICONST_4: return 4;
    case op::ICONST_5: return 5;
    case op::ICONST_M1: return -1;
    case op::SIPUSH:
    case op::BIPUSH: return static_cast<const bc::IntInsnNode*>(insn)->operand;
    case op::LDC: {
        const auto* ldc = static_cast<const bc::LdcInsnNode*>(insn);
        if (std::holds_alternative<std::int32_t>(ldc->cst)) return std::get<std::int32_t>(ldc->cst);
        return std::nullopt;
    }
    default: return std::nullopt;
    }
}

void GLConstants::visit_method(bc::MethodNode& node) {
    if (data_ == nullptr) return;   // (INIT false)
    InsnList& instructions = node.instructions;
    std::vector<bc::MethodInsnNode*> gl_calls;
    std::vector<std::pair<InsnNode*, InsnNode*>> keyboard_constants;
    for (InsnNode* insn : instructions) {
        if (insn->get_type() != InsnNode::METHOD_INSN) continue;
        auto* invoke = static_cast<bc::MethodInsnNode*>(insn);
        if (data_->is_gl_package(invoke->owner)) gl_calls.push_back(invoke);
        if (invoke->owner == "org/lwjgl/input/Keyboard") {
            if (invoke->name == "isKeyDown" || invoke->name == "getKeyName") {
                InsnNode* iconst = invoke->get_previous();
                if (iconst == nullptr) continue;
                const auto value = int_value(iconst);
                if (value) {
                    if (InsnNode* get_field = data_->get_keyboard_insn(instructions, *value)) keyboard_constants.emplace_back(iconst, get_field);
                }
            } else if (invoke->name == "getEventKey") {
                InsnNode* iconst = invoke->get_next();
                if (iconst == nullptr) continue;
                InsnNode* insn2 = iconst->get_next();
                // INVOKE, ICONST, (ANY INSTRUCTION), IADD, ICMP
                // or
                // INVOKE, ICONST, ICMP
                bool has_compare = false;
                int count = 0;
                while (insn2 != nullptr && count < 3 && !has_compare) {
                    if (count == 1 && insn2->get_opcode() != op::IADD) break;
                    if (is_icmp(insn2->get_opcode())) has_compare = true;
                    count++;
                    insn2 = insn2->get_next();
                }
                if (has_compare) {
                    const auto value = int_value(iconst);
                    if (value) {
                        if (InsnNode* get_field = data_->get_keyboard_insn(instructions, *value)) keyboard_constants.emplace_back(iconst, get_field);
                    }
                }
            }
        }
    }

    std::unordered_set<const InsnNode*> replaced;   // (a number found twice is set once: setting a node already replaced corrupts the list in ASM)
    for (const auto& [number, field] : keyboard_constants) {
        if (!replaced.insert(number).second) continue;
        instructions.set(number, field);
    }

    for (bc::MethodInsnNode* invoke : gl_calls) {
        const IdentifyCall identified_call(invoke);
        for (const IdentifyCall::Argument& insns : identified_call.get_arguments()) {
            if (!insns) throw std::runtime_error("GLConstants: an argument of a call was not found");   // (a NullPointerException)
            for (InsnNode* insn : *insns) {
                if (insn == nullptr) continue;
                const auto value = int_value(insn);
                if (value) {
                    std::optional<InsnList> newinsns = data_->get_gl_insn(*invoke, *value);
                    if (newinsns) {
                        instructions.insert(insn, *newinsns);
                        instructions.remove(insn);
                    }
                }
            }
        }
    }
}

}  // namespace cb::java::minecraft::prep
