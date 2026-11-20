// analyze/dispatch.cpp -- see dispatch.hpp.
#include "analyze/dispatch.hpp"

#include <algorithm>
#include <format>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include "analyze/call_graph.hpp"
#include "analyze/globals.hpp"
#include "analyze/strings.hpp"
#include "core/text.hpp"

namespace cb::analyze::dispatch {

namespace {

struct NamedValue {
    Va value;
    std::string_view name;
};

constexpr NamedValue kMessages[] = {
    {0x0001, "WM_CREATE"}, {0x0002, "WM_DESTROY"}, {0x0003, "WM_MOVE"}, {0x0005, "WM_SIZE"}, {0x0006, "WM_ACTIVATE"},
    {0x0007, "WM_SETFOCUS"}, {0x0008, "WM_KILLFOCUS"}, {0x000A, "WM_ENABLE"}, {0x000B, "WM_SETREDRAW"},
    {0x000C, "WM_SETTEXT"}, {0x000D, "WM_GETTEXT"}, {0x000E, "WM_GETTEXTLENGTH"}, {0x000F, "WM_PAINT"},
    {0x0010, "WM_CLOSE"}, {0x0011, "WM_QUERYENDSESSION"}, {0x0012, "WM_QUIT"}, {0x0013, "WM_QUERYOPEN"},
    {0x0014, "WM_ERASEBKGND"}, {0x0015, "WM_SYSCOLORCHANGE"}, {0x0016, "WM_ENDSESSION"}, {0x0018, "WM_SHOWWINDOW"},
    {0x0019, "WM_CTLCOLOR"}, {0x001A, "WM_WININICHANGE"}, {0x001B, "WM_DEVMODECHANGE"}, {0x001C, "WM_ACTIVATEAPP"},
    {0x001D, "WM_FONTCHANGE"}, {0x001E, "WM_TIMECHANGE"}, {0x001F, "WM_CANCELMODE"}, {0x0020, "WM_SETCURSOR"},
    {0x0021, "WM_MOUSEACTIVATE"}, {0x0022, "WM_CHILDACTIVATE"}, {0x0024, "WM_GETMINMAXINFO"}, {0x0026, "WM_PAINTICON"},
    {0x0027, "WM_ICONERASEBKGND"}, {0x0028, "WM_NEXTDLGCTL"}, {0x002B, "WM_DRAWITEM"}, {0x002C, "WM_MEASUREITEM"},
    {0x002D, "WM_DELETEITEM"}, {0x002E, "WM_VKEYTOITEM"}, {0x002F, "WM_CHARTOITEM"}, {0x0030, "WM_SETFONT"},
    {0x0031, "WM_GETFONT"}, {0x0037, "WM_QUERYDRAGICON"}, {0x0039, "WM_COMPAREITEM"}, {0x0041, "WM_COMPACTING"},
    {0x0046, "WM_WINDOWPOSCHANGING"}, {0x0047, "WM_WINDOWPOSCHANGED"}, {0x004A, "WM_COPYDATA"}, {0x004E, "WM_NOTIFY"},
    {0x0053, "WM_HELP"}, {0x0055, "WM_NOTIFYFORMAT"}, {0x007B, "WM_CONTEXTMENU"}, {0x007C, "WM_STYLECHANGING"},
    {0x007D, "WM_STYLECHANGED"}, {0x007E, "WM_DISPLAYCHANGE"}, {0x007F, "WM_GETICON"}, {0x0080, "WM_SETICON"},
    {0x0081, "WM_NCCREATE"}, {0x0082, "WM_NCDESTROY"}, {0x0083, "WM_NCCALCSIZE"}, {0x0084, "WM_NCHITTEST"},
    {0x0085, "WM_NCPAINT"}, {0x0086, "WM_NCACTIVATE"}, {0x0087, "WM_GETDLGCODE"}, {0x00A0, "WM_NCMOUSEMOVE"},
    {0x00A1, "WM_NCLBUTTONDOWN"}, {0x00A2, "WM_NCLBUTTONUP"}, {0x00A3, "WM_NCLBUTTONDBLCLK"}, {0x00A4, "WM_NCRBUTTONDOWN"},
    {0x0100, "WM_KEYDOWN"}, {0x0101, "WM_KEYUP"}, {0x0102, "WM_CHAR"}, {0x0103, "WM_DEADCHAR"}, {0x0104, "WM_SYSKEYDOWN"},
    {0x0105, "WM_SYSKEYUP"}, {0x0106, "WM_SYSCHAR"}, {0x0110, "WM_INITDIALOG"}, {0x0111, "WM_COMMAND"},
    {0x0112, "WM_SYSCOMMAND"}, {0x0113, "WM_TIMER"}, {0x0114, "WM_HSCROLL"}, {0x0115, "WM_VSCROLL"}, {0x0116, "WM_INITMENU"},
    {0x0117, "WM_INITMENUPOPUP"}, {0x011F, "WM_MENUSELECT"}, {0x0120, "WM_MENUCHAR"}, {0x0121, "WM_ENTERIDLE"},
    {0x0132, "WM_CTLCOLORMSGBOX"}, {0x0133, "WM_CTLCOLOREDIT"}, {0x0134, "WM_CTLCOLORLISTBOX"}, {0x0135, "WM_CTLCOLORBTN"},
    {0x0136, "WM_CTLCOLORDLG"}, {0x0137, "WM_CTLCOLORSCROLLBAR"}, {0x0138, "WM_CTLCOLORSTATIC"},
    {0x0200, "WM_MOUSEMOVE"}, {0x0201, "WM_LBUTTONDOWN"}, {0x0202, "WM_LBUTTONUP"}, {0x0203, "WM_LBUTTONDBLCLK"},
    {0x0204, "WM_RBUTTONDOWN"}, {0x0205, "WM_RBUTTONUP"}, {0x0206, "WM_RBUTTONDBLCLK"}, {0x0207, "WM_MBUTTONDOWN"},
    {0x0208, "WM_MBUTTONUP"}, {0x0209, "WM_MBUTTONDBLCLK"}, {0x0210, "WM_PARENTNOTIFY"}, {0x0211, "WM_ENTERMENULOOP"},
    {0x0212, "WM_EXITMENULOOP"}, {0x0214, "WM_SIZING"}, {0x0215, "WM_CAPTURECHANGED"}, {0x0216, "WM_MOVING"},
    {0x0219, "WM_DEVICECHANGE"}, {0x0220, "WM_MDICREATE"}, {0x0221, "WM_MDIDESTROY"}, {0x0222, "WM_MDIACTIVATE"},
    {0x0231, "WM_ENTERSIZEMOVE"}, {0x0232, "WM_EXITSIZEMOVE"}, {0x0233, "WM_DROPFILES"}, {0x0300, "WM_CUT"},
    {0x0301, "WM_COPY"}, {0x0302, "WM_PASTE"}, {0x0303, "WM_CLEAR"}, {0x0304, "WM_UNDO"}, {0x0307, "WM_DESTROYCLIPBOARD"},
    {0x0308, "WM_DRAWCLIPBOARD"}, {0x030D, "WM_CHANGECBCHAIN"}, {0x030F, "WM_QUERYNEWPALETTE"},
    {0x0311, "WM_PALETTECHANGED"}, {0x0312, "WM_HOTKEY"}, {0x0317, "WM_PRINT"}, {0x0318, "WM_PRINTCLIENT"},
};

// The handful of messages every window handles.
constexpr Va kCoreMessages[] = {0x000F, 0x0010, 0x0081, 0x0082, 0x0110, 0x0111, 0x0112};

constexpr NamedValue kControlMessages[] = {
    {0x00, "Sys_Critical_Init"}, {0x01, "Device_Init"}, {0x02, "Init_Complete"}, {0x03, "Sys_VM_Init"},
    {0x04, "Sys_VM_Terminate"}, {0x05, "System_Exit"}, {0x06, "Sys_Critical_Exit"}, {0x07, "Create_VM"},
    {0x08, "VM_Critical_Init"}, {0x09, "VM_Init"}, {0x0A, "VM_Terminate"}, {0x0B, "VM_Not_Executeable"},
    {0x0C, "Destroy_VM"}, {0x0D, "VM_Suspend"}, {0x0E, "VM_Resume"}, {0x0F, "Set_Device_Focus"},
    {0x10, "Begin_Message_Mode"}, {0x11, "End_Message_Mode"}, {0x12, "Reboot_Processor"}, {0x13, "Query_Destroy"},
    {0x14, "Debug_Query"}, {0x15, "Begin_PM_App"}, {0x16, "End_PM_App"}, {0x17, "Device_Reboot_Notify"},
    {0x18, "Crit_Reboot_Notify"}, {0x19, "Close_VM_Notify"}, {0x1A, "Power_Event"}, {0x1B, "Sys_Dynamic_Device_Init"},
    {0x1C, "Sys_Dynamic_Device_Exit"}, {0x1D, "Create_Thread"}, {0x1E, "Thread_Init"}, {0x1F, "Terminate_Thread"},
    {0x20, "Thread_Not_Executeable"}, {0x21, "Destroy_Thread"}, {0x22, "PNP_New_Devnode"}, {0x23, "W32_DeviceIoControl"},
    {0x24, "Sys_VM_Terminate2"}, {0x25, "System_Exit2"}, {0x26, "Sys_Critical_Exit2"}, {0x27, "VM_Terminate2"},
    {0x28, "VM_Not_Executeable2"}, {0x29, "Destroy_VM2"}, {0x2A, "VM_Suspend2"}, {0x2B, "End_Message_Mode2"},
    {0x2C, "End_PM_App2"}, {0x2D, "Device_Reboot_Notify2"}, {0x2E, "Crit_Reboot_Notify2"}, {0x2F, "Close_VM_Notify2"},
    {0x30, "Get_Contention_Handler"}, {0x31, "Kernel32_Initialized"}, {0x32, "Kernel32_Shutdown"},
};

// The DOS functions (AH of int 21h), the BIOS and other interrupts, and the DPMI functions (AX of int 31h).
constexpr NamedValue kDosServices[] = {
    {0x00, "terminate"}, {0x02, "display character"}, {0x09, "display string"}, {0x0E, "select disk"},
    {0x19, "get current disk"}, {0x1A, "set transfer address"}, {0x25, "set interrupt vector"}, {0x2A, "get date"},
    {0x2C, "get time"}, {0x2F, "get transfer address"}, {0x30, "get DOS version"}, {0x33, "break and boot drive"},
    {0x35, "get interrupt vector"}, {0x36, "get free disk space"}, {0x38, "country information"},
    {0x39, "create directory"}, {0x3A, "remove directory"}, {0x3B, "set current directory"}, {0x3C, "create file"},
    {0x3D, "open file"}, {0x3E, "close file"}, {0x3F, "read file"}, {0x40, "write file"}, {0x41, "delete file"},
    {0x42, "seek"}, {0x43, "file attributes"}, {0x44, "device control"}, {0x45, "duplicate handle"},
    {0x46, "redirect handle"}, {0x47, "get current directory"}, {0x48, "allocate memory"}, {0x49, "free memory"},
    {0x4A, "resize memory"}, {0x4B, "execute program"}, {0x4C, "exit"}, {0x4D, "get exit code"}, {0x4E, "find first file"},
    {0x4F, "find next file"}, {0x50, "set PSP"}, {0x51, "get PSP"}, {0x52, "get list of lists"}, {0x56, "rename file"},
    {0x57, "file date and time"}, {0x59, "extended error"}, {0x5A, "create temporary file"}, {0x5B, "create new file"},
    {0x5C, "lock file region"}, {0x5E, "network machine name"}, {0x5F, "network redirection"}, {0x60, "canonical path"},
    {0x62, "get PSP"}, {0x65, "extended country information"}, {0x67, "set handle count"}, {0x68, "commit file"},
    {0x6C, "extended open"}, {0x71, "long file name function"}, {0x73, "FAT32 function"},
};
constexpr NamedValue kBiosServices[] = {
    {0x10, "video"}, {0x11, "equipment list"}, {0x12, "memory size"}, {0x13, "disk"}, {0x14, "serial port"},
    {0x15, "system services"}, {0x16, "keyboard"}, {0x17, "printer"}, {0x1A, "clock"}, {0x20, "DOS terminate"},
    {0x25, "absolute disk read"}, {0x26, "absolute disk write"}, {0x2A, "network"}, {0x2F, "multiplex"},
    {0x31, "DPMI"}, {0x33, "mouse"}, {0x3F, "overlay manager"}, {0x41, "debugger hook"}, {0x67, "expanded memory"},
};
constexpr NamedValue kDpmiServices[] = {
    {0x0000, "allocate descriptors"}, {0x0001, "free descriptor"}, {0x0002, "segment to descriptor"},
    {0x0006, "get segment base"}, {0x0007, "set segment base"}, {0x0008, "set segment limit"},
    {0x0009, "set descriptor access rights"}, {0x000A, "create alias descriptor"}, {0x000B, "get descriptor"},
    {0x000C, "set descriptor"}, {0x0100, "allocate DOS memory"}, {0x0101, "free DOS memory"},
    {0x0200, "get real-mode vector"}, {0x0201, "set real-mode vector"}, {0x0202, "get exception handler"},
    {0x0203, "set exception handler"}, {0x0204, "get protected-mode vector"}, {0x0205, "set protected-mode vector"},
    {0x0300, "simulate real-mode interrupt"}, {0x0301, "call real-mode procedure"}, {0x0302, "call real-mode interrupt procedure"},
    {0x0303, "allocate real-mode callback"}, {0x0304, "free real-mode callback"}, {0x0400, "get version"},
    {0x0500, "get free memory"}, {0x0501, "allocate memory"}, {0x0502, "free memory"}, {0x0503, "resize memory"},
    {0x0600, "lock region"}, {0x0601, "unlock region"}, {0x0800, "map physical address"},
};

template <std::size_t N>
std::optional<std::string_view> lookup(const NamedValue (&table)[N], Va value) {
    for (const NamedValue& row : table) {
        if (row.value == value) return row.name;
    }
    return std::nullopt;
}

// The string-comparing functions whose call with literals makes a keyword dispatch.
bool is_comparer(std::string_view api) {
    static constexpr std::string_view names[] = {"lstrcmp", "lstrcmpi", "strcmp", "_stricmp", "stricmp", "_strcmpi", "strcmpi",
                                                 "strncmp", "_strnicmp", "strnicmp", "wcscmp", "_wcsicmp", "wcsicmp",
                                                 "_mbscmp", "_mbsicmp", "comparestring"};
    return std::find(std::begin(names), std::end(names), api) != std::end(names);
}

// An immediate as a signed number of its own size.
Va signed_immediate(Va value, int size) {
    if (size <= 0 || size >= 8) return value;
    const Va half = Va{1} << (8 * size - 1);
    return value >= half ? value - (half << 1) : value;
}

// "0x10" with the sign always shown: "+0x10", "-0x8".
std::string signed_hex(Va v) { return std::format("{:+#x}", v); }

// The position in a function's instructions of the one at `addr`.
std::optional<std::size_t> position(const Function& fn, Va addr) {
    const auto it = std::lower_bound(fn.insns.begin(), fn.insns.end(), addr, [](const Instruction& i, Va a) { return i.addr < a; });
    if (it == fn.insns.end() || it->addr != addr) return std::nullopt;
    return static_cast<std::size_t>(it - fn.insns.begin());
}

bool same_register(const Operand& a, const Operand& b) {
    return a.kind == Operand::Kind::Register && b.kind == Operand::Kind::Register && a.reg == b.reg && a.size == b.size;
}

// What an operand of fn.insns[i] really is: the place a register was loaded from when that is in sight. -> a key and a text.
std::pair<SwitchKey, std::string> origin(const Function& fn, std::size_t i, const Operand& operand) {
    const Operand* op = &operand;
    std::optional<Definition> loaded;
    if (op->kind == Operand::Kind::Register) {
        loaded = definition_operand(fn, static_cast<int>(i), op->reg);
        if (loaded && loaded->kind == Argument::Kind::Memory) {
            op = &loaded->operand;
        } else {
            SwitchKey key;
            key.kind = SwitchKey::Kind::Register;
            key.base = std::string(family(op->reg));
            return {key, key.base};
        }
    }
    SwitchKey key;
    key.kind = SwitchKey::Kind::Memory;
    key.base = op->base.empty() ? std::string() : std::string(family(op->base));
    key.disp = op->value;
    if (!op->index.empty()) {
        key.index = std::string(family(op->index));
        const std::string base = key.base.empty() ? std::string() : key.base + "+";
        return {key, std::format("[{}{}*{}{}]", base, *key.index, op->scale, signed_hex(op->value))};
    }
    if (!key.base.empty()) return {key, std::format("[{}{}]", key.base, signed_hex(op->value))};
    return {key, std::format("[{:#x}]", op->value)};
}

// What a chain test has accumulated about a register: the thing tested, and how much has been subtracted from it.
struct Tested {
    SwitchKey key;
    std::string text;
    Va amount = 0;
};

bool is_jump_flow(Flow f) { return f == Flow::CondJump || f == Flow::Jump; }

}  // namespace

std::optional<std::string_view> message_name(Va message) { return lookup(kMessages, message); }
std::optional<std::string_view> control_message_name(Va message) { return lookup(kControlMessages, message); }

std::size_t SwitchKeyHash::operator()(const SwitchKey& key) const {
    std::size_t h = std::hash<std::string>()(key.base);
    h = h * 31 + (key.index ? std::hash<std::string>()(*key.index) + 1 : 0);
    h = h * 31 + std::hash<Va>()(key.disp);
    return h * 31 + static_cast<std::size_t>(key.kind);
}

Chains chains(const Function& fn) {
    Chains out;
    std::unordered_set<Va> labels;
    for (const Instruction& i : fn.insns) {
        if (is_jump_flow(i.flow) && i.target) labels.insert(*i.target);
    }
    std::map<std::string, Tested> acc;                 // register -> what it holds and how much was subtracted so far
    std::optional<Tested> pending;                     // what the last test compared: a following je/jne is a case
    for (std::size_t i = 0; i < fn.insns.size(); ++i) {
        const Instruction& ins = fn.insns[i];
        const std::string& mn = ins.mnemonic;
        const std::vector<Operand>& ops = ins.operands;
        if (labels.contains(ins.addr)) acc.clear();
        if (ins.flow == Flow::CondJump) {
            if (pending && (mn == "je" || mn == "jz" || mn == "jne" || mn == "jnz") && ins.target) {
                Chain& slot = out.try_emplace(pending->key, Chain{pending->text, {}}).first->second;
                slot.cases.try_emplace(pending->amount, (mn == "je" || mn == "jz") ? *ins.target : ins.addr + ins.size);
            }
            continue;   // flags survive a conditional jump
        }
        pending.reset();
        if (mn == "cmp" && ops.size() == 2 && ops[1].kind == Operand::Kind::Immediate &&
            (ops[0].kind == Operand::Kind::Register || ops[0].kind == Operand::Kind::Memory)) {
            Tested t;
            Va base = 0;
            const auto held = ops[0].kind == Operand::Kind::Register ? acc.find(std::string(family(ops[0].reg))) : acc.end();
            if (held != acc.end()) {
                t = held->second;
                base = t.amount;
            } else {
                auto [key, text] = origin(fn, i, ops[0]);
                t.key = std::move(key);
                t.text = std::move(text);
            }
            t.amount = (ops[1].value + base) & 0xFFFFFFFF;
            pending = std::move(t);
        } else if ((mn == "sub" || mn == "dec" || mn == "add" || mn == "inc") && !ops.empty() &&
                   ops[0].kind == Operand::Kind::Register && (ops.size() == 1 || ops[1].kind == Operand::Kind::Immediate)) {
            const std::string fam(family(ops[0].reg));
            if (fam == "esp" || fam == "ebp") continue;
            auto held = acc.find(fam);
            if (held == acc.end()) {
                auto [key, text] = origin(fn, i, ops[0]);
                held = acc.emplace(fam, Tested{std::move(key), std::move(text), 0}).first;
            }
            const Va step = mn == "dec"   ? 1
                            : mn == "inc" ? -1
                            : mn == "sub" ? ops[1].value
                                          : -signed_immediate(ops[1].value, ops[1].size);
            held->second.amount += step;
            pending = held->second;
            pending->amount = held->second.amount & 0xFFFFFFFF;
        } else if ((mn == "test" || mn == "or" || mn == "and") && ops.size() == 2 && same_register(ops[0], ops[1])) {
            const auto held = acc.find(std::string(family(ops[0].reg)));
            if (held != acc.end()) {
                pending = held->second;                 // (the amount subtracted so far, as it stands)
            } else {
                auto [key, text] = origin(fn, i, ops[0]);
                pending = Tested{std::move(key), std::move(text), 0};
            }
        } else {
            for (const std::string& r : ins.writes) acc.erase(r);
        }
    }
    return out;
}

std::vector<Switch> switches(Image& image) {
    std::vector<Switch> out;
    for (const Va start : std::vector<Va>(image.functions())) {
        const Function& fn = image.function(start);
        Chains found = chains(fn);
        std::set<std::optional<SwitchKey>> tabled;
        for (const JumpTable& t : fn.tables) {
            std::optional<SwitchKey> key;
            std::string text = t.selector;
            if (const auto site = position(fn, t.site)) {
                const std::size_t i = *site;
                for (std::size_t j = i; j-- > 0 && i - j < 10;) {   // the ten instructions before the jump, nearest first
                    const Instruction& b = fn.insns[j];
                    if (b.mnemonic != "cmp" || b.operands.size() < 2 || b.operands[1].kind != Operand::Kind::Immediate) continue;
                    auto [k, tx] = origin(fn, j, b.operands[0]);
                    key = std::move(k);
                    text = std::move(tx);
                    for (std::size_t p = j; p-- > 0 && j - p < 4;) {   // past the `sub reg, low`
                        const Instruction& q = fn.insns[p];
                        if ((q.mnemonic == "sub" || q.mnemonic == "dec" || q.mnemonic == "add" || q.mnemonic == "lea") && !q.operands.empty() &&
                            q.operands[0].kind == Operand::Kind::Register && b.operands[0].kind == Operand::Kind::Register &&
                            family(q.operands[0].reg) == family(b.operands[0].reg)) {
                            auto [k2, tx2] = origin(fn, p, q.operands[0]);
                            key = std::move(k2);
                            text = std::move(tx2);
                        }
                    }
                    break;
                }
            }
            std::map<Va, Va> cases;
            if (t.index_bytes) {
                for (std::size_t n = 0; n < t.index_bytes->size(); ++n) {
                    const std::size_t slot = (*t.index_bytes)[n];
                    if (slot < t.targets.size() && (!t.default_target || t.targets[slot] != *t.default_target)) {
                        cases[t.low + static_cast<Va>(n)] = t.targets[slot];
                    }
                }
            } else {
                for (std::size_t n = 0; n < t.targets.size(); ++n) cases[t.low + static_cast<Va>(n)] = t.targets[n];
            }
            tabled.insert(key);
            std::map<Va, Va> merged;
            if (key) {
                if (const Chain* c = found.find(*key)) merged = c->cases;
            }
            for (const auto& [v, target] : cases) merged[v] = target;
            Switch s;
            s.function = fn.start;
            s.on = std::move(text);
            s.key = key;
            s.cases = std::move(merged);
            s.default_target = t.default_target;
            s.table = t.table;
            s.site = t.site;
            s.bounded = t.bounded;
            out.push_back(std::move(s));
        }
        for (const auto& [key, c] : found) {
            if (tabled.contains(key) || c.cases.size() < 3) continue;
            Switch s;
            s.function = fn.start;
            s.on = c.on;
            s.key = key;
            s.cases = c.cases;
            out.push_back(std::move(s));
        }
    }
    return out;
}

namespace {

// Is `key` the message argument of a window or dialog procedure? (The second argument; a 16-bit far one is at bp+0Ch.)
bool is_message_argument(const Image& image, const std::optional<SwitchKey>& key, bool far) {
    if (!key || key->kind != SwitchKey::Kind::Memory || key->index) return false;
    if (image.bits() == 16) return key->base == "ebp" && key->disp == (far ? 0x0C : 0x0A);
    return (key->base == "ebp" && key->disp == 0x0C) || (key->base == "esp" && key->disp == 8);
}

// Is `key` the command argument (wParam)?
bool is_command_argument(const Image& image, const std::optional<SwitchKey>& key, bool far) {
    if (!key || key->kind != SwitchKey::Kind::Memory || key->index) return false;
    if (image.bits() == 16) return key->base == "ebp" && key->disp == (far ? 0x0A : 0x08);
    return (key->base == "ebp" && key->disp == 0x10) || (key->base == "esp" && key->disp == 0x0C);
}

bool is_core_message(Va v) { return std::find(std::begin(kCoreMessages), std::end(kCoreMessages), v) != std::end(kCoreMessages); }

}  // namespace

std::vector<MessageDispatch> messages(Image& image, const std::vector<Switch>& all, const ConventionFacts& conventions) {
    std::map<Va, std::vector<const Switch*>> by_function;
    for (const Switch& s : all) by_function[s.function].push_back(&s);
    std::vector<MessageDispatch> out;
    for (const auto& [function, sws] : by_function) {
        const auto fact_it = conventions.find(function);
        const ConventionFact fact = fact_it != conventions.end() ? fact_it->second : ConventionFact{};
        const bool far = fact.is_far();
        const bool proc = fact.procedure && !fact.procedure->empty();
        MessageDispatch found;
        found.function = function;
        found.procedure = fact.procedure;
        bool matched = false;
        for (const Switch* s : sws) {
            std::size_t known = 0, telling = 0;
            bool telling_core = false;
            for (const auto& [v, target] : s->cases) {
                if (!message_name(v)) continue;
                ++known;
                if (v >= 0x0F) {   // 1, 2, 3 are also just small numbers
                    ++telling;
                    telling_core = telling_core || is_core_message(v);
                }
            }
            const std::size_t values = s->cases.size();
            const bool on_message = is_message_argument(image, s->key, far) &&
                                    (fact.removes == (image.bits() == 32 ? 16 : 10) || telling > 0);
            if (image.kind() == ImageKind::Le) continue;
            if ((on_message && known >= 2) || (proc && telling >= 2 && known * 2 >= values) ||
                (telling >= 3 && telling_core && known * 4 >= values * 3)) {
                if (!matched) {
                    found.why = on_message ? "switch on the message argument" : proc ? "a " + *fact.procedure : "cases are message numbers";
                }
                matched = true;
                for (const auto& [v, target] : s->cases) found.messages[v] = target;
            } else if (is_command_argument(image, s->key, far)) {
                for (const auto& [v, target] : s->cases) found.commands[v] = target;
            }
        }
        if (matched) out.push_back(std::move(found));
    }
    return out;
}

Keywords keywords(Image& image) {
    Keywords out;
    const call_graph::Graph graph = call_graph::build(image);
    strings::Table table = strings::build_table(image, graph);
    for (const Va start : std::vector<Va>(image.functions())) {
        const Function& fn = image.function(start);
        struct Compared {
            std::string text, api;
        };
        std::vector<Compared> words;
        const auto calls = graph.calls.find(start);
        if (calls == graph.calls.end()) continue;
        for (const call_graph::Call& call : calls->second) {
            if (!call.destination || !image.is_import(*call.destination)) continue;
            const std::string& full = image.import_name(*call.destination);
            const std::size_t bang = full.rfind('!');
            std::string api = text::lower(full.substr(bang == std::string::npos ? 0 : bang + 1));
            if (api.size() > 1 && (api.back() == 'a' || api.back() == 'w') && is_comparer(std::string_view(api).substr(0, api.size() - 1))) api.pop_back();
            if (!is_comparer(api)) continue;
            const auto at = position(fn, call.site);
            if (!at) continue;
            const std::vector<Argument> args = arguments(fn, static_cast<int>(*at), image.bits() == 16 ? 4 : 2);
            for (std::size_t k = 0; k < 2; ++k) {
                if (const auto lit = strings::literal(image, table, args, k)) {
                    words.push_back({lit->entry->text, api});
                    break;
                }
            }
        }
        std::set<std::string_view> distinct;
        for (const Compared& w : words) distinct.insert(w.text);
        if (distinct.size() >= 2) {
            KeywordFunction k;
            k.function = start;
            k.compares_with = words.front().api;
            for (const Compared& w : words) k.keywords.push_back(w.text);
            out.functions.push_back(std::move(k));
        }
    }
    // tables in the data that pair a string with a function
    for (const globals::PointerTable& t : globals::pointer_tables(image, table)) {
        if (t.shape != globals::PointerTable::Shape::Run || t.count < 4 || t.count % 2 != 0) continue;
        bool pairs = true;
        for (std::size_t k = 0; k < static_cast<std::size_t>(t.count) && pairs; ++k) {
            pairs = t.kinds[k] == (k % 2 == 0 ? globals::SlotKind::String : globals::SlotKind::Function);
        }
        if (!pairs) continue;
        KeywordTable k;
        k.table = t.at;
        for (std::size_t n = 0; n < static_cast<std::size_t>(t.count); n += 2) {
            k.keywords.push_back(table.at(t.targets[n]).text);
            k.handlers.push_back(t.targets[n + 1]);
        }
        out.tables.push_back(std::move(k));
    }
    return out;
}

std::vector<Interrupt> interrupts(Image& image) {
    std::vector<Interrupt> out;
    for (const Va start : std::vector<Va>(image.functions())) {
        const Function& fn = image.function(start);
        for (std::size_t i = 0; i < fn.insns.size(); ++i) {
            const Instruction& ins = fn.insns[i];
            if (ins.mnemonic != "int" || ins.operands.empty() || ins.operands[0].kind != Operand::Kind::Immediate) continue;
            const auto n = static_cast<std::uint8_t>(ins.operands[0].value & 0xFF);
            if (image.kind() == ImageKind::Le && n == 0x20) continue;   // a driver's service call, already an import
            const bool wide = n == 0x31 || n == 0x2F;                      // these take a number in AX
            std::optional<std::uint32_t> code;
            for (std::size_t back = i; back-- > 0 && i - back <= 8;) {
                const Instruction& b = fn.insns[back];
                if (b.flow != Flow::None || b.mnemonic == "int") break;
                if (b.mnemonic == "mov" && b.operands.size() == 2 && b.operands[0].kind == Operand::Kind::Register &&
                    b.operands[1].kind == Operand::Kind::Immediate) {
                    const Va v = b.operands[1].value;
                    if (b.operands[0].reg == "ah") {
                        if (!wide) code = static_cast<std::uint32_t>(v);
                        break;
                    }
                    if (b.operands[0].reg == "ax" || b.operands[0].reg == "eax") {
                        code = wide ? static_cast<std::uint32_t>(v & 0xFFFF) : static_cast<std::uint32_t>((v >> 8) & 0xFF);
                        break;
                    }
                }
            }
            Interrupt it;
            it.function = start;
            it.site = ins.addr;
            it.number = n;
            it.code = code;
            if (n == 0x21) {
                const auto known = code ? lookup(kDosServices, *code) : std::nullopt;
                it.name = "DOS: " + (known ? std::string(*known) : "function " + (code ? std::format("{:02X}h", *code) : std::string("?")));
                it.named = code.has_value();
            } else if (n == 0x31) {
                const auto known = code ? lookup(kDpmiServices, *code) : std::nullopt;
                it.name = "DPMI: " + (known ? std::string(*known) : "function " + (code ? std::format("{:04X}h", *code) : std::string("?")));
                it.named = code.has_value();
            } else {
                const auto bios = lookup(kBiosServices, n);
                it.name = bios ? std::string(*bios) : std::format("interrupt {:02X}h", n);
                if (code) it.name += std::format(" function {:02X}h", *code);
                it.named = bios.has_value();
            }
            out.push_back(std::move(it));
        }
    }
    return out;
}

std::optional<ControlMessages> control_messages(Image& image, const std::vector<Switch>& all, const std::optional<DeviceBlock>& device) {
    if (image.kind() != ImageKind::Le || !device) return std::nullopt;
    const std::optional<std::uint32_t> proc_address = image.u32(device->descriptor + 0x18);
    if (!proc_address || !image.is_function(*proc_address)) return std::nullopt;
    const Va proc = *proc_address;
    std::map<Va, Va> best;
    for (const Switch& s : all) {
        if (s.function == proc && s.key && s.key->kind == SwitchKey::Kind::Register && s.key->base == "eax") {
            for (const auto& [v, target] : s.cases) best[v] = target;
        }
    }
    // short procedures test one or two messages only
    for (const auto& [key, c] : chains(image.function(proc))) {
        if (c.on != "eax") continue;
        for (const auto& [v, target] : c.cases) best.try_emplace(v, target);
    }
    ControlMessages out;
    out.procedure = proc;
    for (const auto& [v, target] : best) {
        const auto name = control_message_name(v);
        out.handles.push_back({v, name ? std::string(*name) : std::format("message {:#x}", v), target});
    }
    return out;
}

Report analyse(Image& image, const ConventionFacts& conventions, const std::optional<DeviceBlock>& device) {
    const std::vector<Switch> sw = switches(image);
    const std::vector<MessageDispatch> msgs = messages(image, sw, conventions);
    const Keywords kw = keywords(image);
    const std::vector<Interrupt> ints = interrupts(image);
    const std::optional<ControlMessages> ctl = control_messages(image, sw, device);

    Report r;
    for (const Switch& s : sw) {
        SwitchFact f;
        f.function = s.function;
        f.on = s.on;
        f.site = s.site;
        f.table = s.table;
        f.default_target = s.default_target;
        f.cases.assign(s.cases.begin(), s.cases.end());
        r.facts.emplace_back(std::move(f));
    }
    for (const MessageDispatch& m : msgs) {
        MessageFact f;
        f.function = m.function;
        f.procedure = m.procedure;
        f.why = m.why;
        for (const auto& [v, handler] : m.messages) {
            const auto name = message_name(v);
            f.messages.push_back({v, name ? std::string(*name) : std::format("{:#06x}", v), handler});
        }
        f.commands.assign(m.commands.begin(), m.commands.end());
        r.facts.emplace_back(std::move(f));
    }
    for (const KeywordFunction& k : kw.functions) r.facts.emplace_back(KeywordFunctionFact{k});
    for (const KeywordTable& k : kw.tables) r.facts.emplace_back(KeywordTableFact{k});
    std::map<Va, std::set<std::string>> services;
    for (const Interrupt& i : ints) services[i.function].insert(i.name);
    for (const auto& [function, names] : services) r.facts.emplace_back(ServicesFact{function, {names.begin(), names.end()}});
    if (ctl) r.facts.emplace_back(DriverControlFact{ctl->procedure, device ? std::optional<std::string>(device->name) : std::nullopt, ctl->handles});

    Counts& c = r.counts;
    for (const Switch& s : sw) {
        if (s.table) {
            ++c.jump_tables;
            c.jump_table_cases += static_cast<std::int64_t>(s.cases.size());
            if (s.bounded) ++c.jump_tables_with_bound;
        } else {
            ++c.compare_chains;
            c.compare_chain_cases += static_cast<std::int64_t>(s.cases.size());
        }
        if (s.key && s.key->kind == SwitchKey::Kind::Memory && (s.key->base == "ebp" || s.key->base == "esp") && s.key->disp > 0) {
            ++c.switches_on_an_argument;
        }
    }
    c.message_dispatchers = static_cast<std::int64_t>(msgs.size());
    for (const MessageDispatch& m : msgs) {
        c.messages_handled += static_cast<std::int64_t>(m.messages.size());
        for (const auto& [v, handler] : m.messages) c.named_messages += message_name(v) ? 1 : 0;
        if (!m.commands.empty()) ++c.command_dispatchers;
        c.command_ids += static_cast<std::int64_t>(m.commands.size());
    }
    c.keyword_dispatchers = static_cast<std::int64_t>(kw.functions.size() + kw.tables.size());
    for (const KeywordFunction& k : kw.functions) c.keywords += static_cast<std::int64_t>(k.keywords.size());
    for (const KeywordTable& k : kw.tables) c.keywords += static_cast<std::int64_t>(k.keywords.size());
    c.interrupt_sites = static_cast<std::int64_t>(ints.size());
    for (const Interrupt& i : ints) c.named_services += i.named ? 1 : 0;
    c.driver_control_messages = ctl ? static_cast<std::int64_t>(ctl->handles.size()) : 0;

    // a few lines saying what was found
    auto wm = [](Va v) { const auto n = message_name(v); return n ? std::string(*n) : std::format("{:#x}", v); };
    for (std::size_t i = 0; i < msgs.size() && i < 6; ++i) {
        std::vector<std::string> shown;
        for (const auto& [v, handler] : msgs[i].messages) {
            if (shown.size() == 10) break;
            shown.push_back(wm(v));
        }
        r.text.push_back(std::format("{} ({}): {}", image.label(msgs[i].function), msgs[i].why, text::join(shown, ", ")));
    }
    auto quoted = [](const std::string& w) { return "'" + w + "'"; };
    for (std::size_t i = 0; i < kw.functions.size() + kw.tables.size() && i < 4; ++i) {
        const bool from_function = i < kw.functions.size();
        const std::vector<std::string>& words = from_function ? kw.functions[i].keywords : kw.tables[i - kw.functions.size()].keywords;
        std::vector<std::string> shown;
        for (std::size_t k = 0; k < words.size() && k < 8; ++k) shown.push_back(quoted(words[k]));
        const std::string where = from_function ? image.label(kw.functions[i].function)
                                                : std::format("table {:x}", kw.tables[i - kw.functions.size()].table);
        r.text.push_back(std::format("keywords in {}: {}", where, text::join(shown, ", ")));
    }
    if (ctl) {
        std::vector<std::string> names;
        for (const HandledMessage& h : ctl->handles) names.push_back(h.name);
        r.text.push_back(std::format("device {} control procedure {} handles: {}", device ? device->name : std::string(),
                                     image.label(ctl->procedure), text::join(names, ", ")));
    }
    if (!ints.empty()) {
        std::vector<std::pair<std::string, int>> seen;   // in the order first met
        for (const Interrupt& i : ints) {
            const auto it = std::find_if(seen.begin(), seen.end(), [&](const auto& p) { return p.first == i.name; });
            if (it == seen.end()) seen.emplace_back(i.name, 1);
            else ++it->second;
        }
        std::stable_sort(seen.begin(), seen.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
        std::vector<std::string> shown;
        for (std::size_t k = 0; k < seen.size() && k < 10; ++k) shown.push_back(std::format("{} x{}", seen[k].first, seen[k].second));
        r.text.push_back("system services: " + text::join(shown, ", "));
    }
    return r;
}

}  // namespace cb::analyze::dispatch
