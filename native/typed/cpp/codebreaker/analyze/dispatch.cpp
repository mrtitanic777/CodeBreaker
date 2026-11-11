// codebreaker/analyze/dispatch.cpp -- typed C++ from codebreaker.analyze.dispatch (see the header).
//
// How code chooses what to do: jump tables, compare chains, message and command dispatch, keyword dispatch, and the
// system services a function asks for by number. call_graph / strings / globals are reached through
// analyze/image_bridge.hpp (their private Image stand-ins).
#include "codebreaker/analyze/dispatch.hpp"

#include <algorithm>
#include <map>
#include <set>

#include "codebreaker/analyze/conventions.hpp"
#include "codebreaker/analyze/image_bridge.hpp"
#include "codebreaker/decompile/describe.hpp"

namespace codebreaker::analyze::dispatch {

using image::Function;
using image::Image;
using image::Op;
namespace describe = decompile::describe;

const py::dict<int64_t, py::str> MESSAGES = {
    {1, py::str("WM_CREATE")}, {2, py::str("WM_DESTROY")}, {3, py::str("WM_MOVE")}, {5, py::str("WM_SIZE")},
    {6, py::str("WM_ACTIVATE")}, {7, py::str("WM_SETFOCUS")}, {8, py::str("WM_KILLFOCUS")}, {10, py::str("WM_ENABLE")},
    {11, py::str("WM_SETREDRAW")}, {12, py::str("WM_SETTEXT")}, {13, py::str("WM_GETTEXT")}, {14, py::str("WM_GETTEXTLENGTH")},
    {15, py::str("WM_PAINT")}, {16, py::str("WM_CLOSE")}, {17, py::str("WM_QUERYENDSESSION")}, {18, py::str("WM_QUIT")},
    {19, py::str("WM_QUERYOPEN")}, {20, py::str("WM_ERASEBKGND")}, {21, py::str("WM_SYSCOLORCHANGE")}, {22, py::str("WM_ENDSESSION")},
    {24, py::str("WM_SHOWWINDOW")}, {25, py::str("WM_CTLCOLOR")}, {26, py::str("WM_WININICHANGE")}, {27, py::str("WM_DEVMODECHANGE")},
    {28, py::str("WM_ACTIVATEAPP")}, {29, py::str("WM_FONTCHANGE")}, {30, py::str("WM_TIMECHANGE")}, {31, py::str("WM_CANCELMODE")},
    {32, py::str("WM_SETCURSOR")}, {33, py::str("WM_MOUSEACTIVATE")}, {34, py::str("WM_CHILDACTIVATE")}, {36, py::str("WM_GETMINMAXINFO")},
    {38, py::str("WM_PAINTICON")}, {39, py::str("WM_ICONERASEBKGND")}, {40, py::str("WM_NEXTDLGCTL")}, {43, py::str("WM_DRAWITEM")},
    {44, py::str("WM_MEASUREITEM")}, {45, py::str("WM_DELETEITEM")}, {46, py::str("WM_VKEYTOITEM")}, {47, py::str("WM_CHARTOITEM")},
    {48, py::str("WM_SETFONT")}, {49, py::str("WM_GETFONT")}, {55, py::str("WM_QUERYDRAGICON")}, {57, py::str("WM_COMPAREITEM")},
    {65, py::str("WM_COMPACTING")}, {70, py::str("WM_WINDOWPOSCHANGING")}, {71, py::str("WM_WINDOWPOSCHANGED")}, {74, py::str("WM_COPYDATA")},
    {78, py::str("WM_NOTIFY")}, {83, py::str("WM_HELP")}, {85, py::str("WM_NOTIFYFORMAT")}, {123, py::str("WM_CONTEXTMENU")},
    {124, py::str("WM_STYLECHANGING")}, {125, py::str("WM_STYLECHANGED")}, {126, py::str("WM_DISPLAYCHANGE")}, {127, py::str("WM_GETICON")},
    {128, py::str("WM_SETICON")}, {129, py::str("WM_NCCREATE")}, {130, py::str("WM_NCDESTROY")}, {131, py::str("WM_NCCALCSIZE")},
    {132, py::str("WM_NCHITTEST")}, {133, py::str("WM_NCPAINT")}, {134, py::str("WM_NCACTIVATE")}, {135, py::str("WM_GETDLGCODE")},
    {160, py::str("WM_NCMOUSEMOVE")}, {161, py::str("WM_NCLBUTTONDOWN")}, {162, py::str("WM_NCLBUTTONUP")}, {163, py::str("WM_NCLBUTTONDBLCLK")},
    {164, py::str("WM_NCRBUTTONDOWN")}, {256, py::str("WM_KEYDOWN")}, {257, py::str("WM_KEYUP")}, {258, py::str("WM_CHAR")},
    {259, py::str("WM_DEADCHAR")}, {260, py::str("WM_SYSKEYDOWN")}, {261, py::str("WM_SYSKEYUP")}, {262, py::str("WM_SYSCHAR")},
    {272, py::str("WM_INITDIALOG")}, {273, py::str("WM_COMMAND")}, {274, py::str("WM_SYSCOMMAND")}, {275, py::str("WM_TIMER")},
    {276, py::str("WM_HSCROLL")}, {277, py::str("WM_VSCROLL")}, {278, py::str("WM_INITMENU")}, {279, py::str("WM_INITMENUPOPUP")},
    {287, py::str("WM_MENUSELECT")}, {288, py::str("WM_MENUCHAR")}, {289, py::str("WM_ENTERIDLE")}, {306, py::str("WM_CTLCOLORMSGBOX")},
    {307, py::str("WM_CTLCOLOREDIT")}, {308, py::str("WM_CTLCOLORLISTBOX")}, {309, py::str("WM_CTLCOLORBTN")}, {310, py::str("WM_CTLCOLORDLG")},
    {311, py::str("WM_CTLCOLORSCROLLBAR")}, {312, py::str("WM_CTLCOLORSTATIC")}, {512, py::str("WM_MOUSEMOVE")}, {513, py::str("WM_LBUTTONDOWN")},
    {514, py::str("WM_LBUTTONUP")}, {515, py::str("WM_LBUTTONDBLCLK")}, {516, py::str("WM_RBUTTONDOWN")}, {517, py::str("WM_RBUTTONUP")},
    {518, py::str("WM_RBUTTONDBLCLK")}, {519, py::str("WM_MBUTTONDOWN")}, {520, py::str("WM_MBUTTONUP")}, {521, py::str("WM_MBUTTONDBLCLK")},
    {528, py::str("WM_PARENTNOTIFY")}, {529, py::str("WM_ENTERMENULOOP")}, {530, py::str("WM_EXITMENULOOP")}, {532, py::str("WM_SIZING")},
    {533, py::str("WM_CAPTURECHANGED")}, {534, py::str("WM_MOVING")}, {537, py::str("WM_DEVICECHANGE")}, {544, py::str("WM_MDICREATE")},
    {545, py::str("WM_MDIDESTROY")}, {546, py::str("WM_MDIACTIVATE")}, {561, py::str("WM_ENTERSIZEMOVE")}, {562, py::str("WM_EXITSIZEMOVE")},
    {563, py::str("WM_DROPFILES")}, {768, py::str("WM_CUT")}, {769, py::str("WM_COPY")}, {770, py::str("WM_PASTE")},
    {771, py::str("WM_CLEAR")}, {772, py::str("WM_UNDO")}, {775, py::str("WM_DESTROYCLIPBOARD")}, {776, py::str("WM_DRAWCLIPBOARD")},
    {781, py::str("WM_CHANGECBCHAIN")}, {783, py::str("WM_QUERYNEWPALETTE")}, {785, py::str("WM_PALETTECHANGED")}, {786, py::str("WM_HOTKEY")},
    {791, py::str("WM_PRINT")}, {792, py::str("WM_PRINTCLIENT")},
};

const py::dict<int64_t, py::str> CONTROL_MESSAGES = {
    {0, py::str("Sys_Critical_Init")}, {1, py::str("Device_Init")}, {2, py::str("Init_Complete")}, {3, py::str("Sys_VM_Init")},
    {4, py::str("Sys_VM_Terminate")}, {5, py::str("System_Exit")}, {6, py::str("Sys_Critical_Exit")}, {7, py::str("Create_VM")},
    {8, py::str("VM_Critical_Init")}, {9, py::str("VM_Init")}, {10, py::str("VM_Terminate")}, {11, py::str("VM_Not_Executeable")},
    {12, py::str("Destroy_VM")}, {13, py::str("VM_Suspend")}, {14, py::str("VM_Resume")}, {15, py::str("Set_Device_Focus")},
    {16, py::str("Begin_Message_Mode")}, {17, py::str("End_Message_Mode")}, {18, py::str("Reboot_Processor")}, {19, py::str("Query_Destroy")},
    {20, py::str("Debug_Query")}, {21, py::str("Begin_PM_App")}, {22, py::str("End_PM_App")}, {23, py::str("Device_Reboot_Notify")},
    {24, py::str("Crit_Reboot_Notify")}, {25, py::str("Close_VM_Notify")}, {26, py::str("Power_Event")}, {27, py::str("Sys_Dynamic_Device_Init")},
    {28, py::str("Sys_Dynamic_Device_Exit")}, {29, py::str("Create_Thread")}, {30, py::str("Thread_Init")}, {31, py::str("Terminate_Thread")},
    {32, py::str("Thread_Not_Executeable")}, {33, py::str("Destroy_Thread")}, {34, py::str("PNP_New_Devnode")}, {35, py::str("W32_DeviceIoControl")},
    {36, py::str("Sys_VM_Terminate2")}, {37, py::str("System_Exit2")}, {38, py::str("Sys_Critical_Exit2")}, {39, py::str("VM_Terminate2")},
    {40, py::str("VM_Not_Executeable2")}, {41, py::str("Destroy_VM2")}, {42, py::str("VM_Suspend2")}, {43, py::str("End_Message_Mode2")},
    {44, py::str("End_PM_App2")}, {45, py::str("Device_Reboot_Notify2")}, {46, py::str("Crit_Reboot_Notify2")}, {47, py::str("Close_VM_Notify2")},
    {48, py::str("Get_Contention_Handler")}, {49, py::str("Kernel32_Initialized")}, {50, py::str("Kernel32_Shutdown")},
};

const std::map<int64_t, std::string> DOS_ = {
    {0, ("terminate")}, {2, ("display character")}, {9, ("display string")}, {14, ("select disk")},
    {25, ("get current disk")}, {26, ("set transfer address")}, {37, ("set interrupt vector")}, {42, ("get date")},
    {44, ("get time")}, {47, ("get transfer address")}, {48, ("get DOS version")}, {51, ("break and boot drive")},
    {53, ("get interrupt vector")}, {54, ("get free disk space")}, {56, ("country information")}, {57, ("create directory")},
    {58, ("remove directory")}, {59, ("set current directory")}, {60, ("create file")}, {61, ("open file")},
    {62, ("close file")}, {63, ("read file")}, {64, ("write file")}, {65, ("delete file")},
    {66, ("seek")}, {67, ("file attributes")}, {68, ("device control")}, {69, ("duplicate handle")},
    {70, ("redirect handle")}, {71, ("get current directory")}, {72, ("allocate memory")}, {73, ("free memory")},
    {74, ("resize memory")}, {75, ("execute program")}, {76, ("exit")}, {77, ("get exit code")},
    {78, ("find first file")}, {79, ("find next file")}, {80, ("set PSP")}, {81, ("get PSP")},
    {82, ("get list of lists")}, {86, ("rename file")}, {87, ("file date and time")}, {89, ("extended error")},
    {90, ("create temporary file")}, {91, ("create new file")}, {92, ("lock file region")}, {94, ("network machine name")},
    {95, ("network redirection")}, {96, ("canonical path")}, {98, ("get PSP")}, {101, ("extended country information")},
    {103, ("set handle count")}, {104, ("commit file")}, {108, ("extended open")}, {113, ("long file name function")},
    {115, ("FAT32 function")},
};

const std::map<int64_t, std::string> BIOS_ = {
    {16, ("video")}, {17, ("equipment list")}, {18, ("memory size")}, {19, ("disk")},
    {20, ("serial port")}, {21, ("system services")}, {22, ("keyboard")}, {23, ("printer")},
    {26, ("clock")}, {32, ("DOS terminate")}, {37, ("absolute disk read")}, {38, ("absolute disk write")},
    {42, ("network")}, {47, ("multiplex")}, {49, ("DPMI")}, {51, ("mouse")},
    {63, ("overlay manager")}, {65, ("debugger hook")}, {103, ("expanded memory")},
};

const std::map<int64_t, std::string> DPMI_ = {
    {0, ("allocate descriptors")}, {1, ("free descriptor")}, {2, ("segment to descriptor")}, {6, ("get segment base")},
    {7, ("set segment base")}, {8, ("set segment limit")}, {9, ("set descriptor access rights")}, {10, ("create alias descriptor")},
    {11, ("get descriptor")}, {12, ("set descriptor")}, {256, ("allocate DOS memory")}, {257, ("free DOS memory")},
    {512, ("get real-mode vector")}, {513, ("set real-mode vector")}, {514, ("get exception handler")}, {515, ("set exception handler")},
    {516, ("get protected-mode vector")}, {517, ("set protected-mode vector")}, {768, ("simulate real-mode interrupt")}, {769, ("call real-mode procedure")},
    {770, ("call real-mode interrupt procedure")}, {771, ("allocate real-mode callback")}, {772, ("free real-mode callback")}, {1024, ("get version")},
    {1280, ("get free memory")}, {1281, ("allocate memory")}, {1282, ("free memory")}, {1283, ("resize memory")},
    {1536, ("lock region")}, {1537, ("unlock region")}, {2048, ("map physical address")},
};


namespace {

const std::set<std::string>& comparers() {
    static const std::set<std::string> s = {"lstrcmp", "lstrcmpi", "strcmp",  "_stricmp", "stricmp",  "_strcmpi", "strcmpi", "strncmp",
                                            "_strnicmp", "strnicmp", "wcscmp", "_wcsicmp", "wcsicmp", "_mbscmp",  "_mbsicmp", "comparestring"};
    return s;
}
const std::set<int64_t> kCore = {0x000F, 0x0010, 0x0081, 0x0082, 0x0110, 0x0111, 0x0112};   // _CORE

const Op& op_at(const py::list<Op>& ops, int64_t k) {
    if (k >= ops.size()) throw py::IndexError("tuple index out of range");
    return ops[k];
}

std::string hex_lower(int64_t v) {
    char b[32];
    std::snprintf(b, sizeof b, "%llx", static_cast<unsigned long long>(v < 0 ? -v : v));
    return b;
}
// "%+#x"
std::string signed_alt_hex(int64_t v) { return std::string(v < 0 ? "-" : "+") + "0x" + hex_lower(v); }
// "%#x"
std::string alt_hex(int64_t v) { return std::string(v < 0 ? "-" : "") + "0x" + hex_lower(v); }

bool flow_is(const image::Ins& i, const char* f) { return i.flow && *i.flow == f; }

}  // namespace

// ---- SwitchKey ----------------------------------------------------------------------------------------------------------
bool SwitchKey::operator==(const SwitchKey& o) const { return kind == o.kind && base == o.base && index == o.index && disp == o.disp; }

namespace {
// Python's repr of the key tuple
std::string key_repr(const SwitchKey& k) {
    auto q = [](const std::optional<py::str>& s) { return s ? py::repr(*s).utf8() : std::string("None"); };
    if (k.kind == "r") return "('r', " + q(k.base) + ")";
    if (k.index) return "('m', " + q(k.base) + ", " + q(k.index) + ", " + std::to_string(k.disp) + ")";
    return "('m', " + q(k.base) + ", " + std::to_string(k.disp) + ")";
}
}  // namespace

bool SwitchKey::operator<(const SwitchKey& o) const { return key_repr(*this) < key_repr(o); }

// ---- _origin / _chains ------------------------------------------------------------------------------------------------
std::tuple<SwitchKey, py::str> _origin(const Function& fn, int64_t i, const Op& op_in) {
    Op op = op_in;
    if (op.kind == "r") {
        auto d = image::definition(fn, i, op.name);
        if (d && d->kind == "m") {
            op = *d;
        } else {
            SwitchKey k;
            k.kind = py::str("r");
            k.base = image::family(op.name);
            return {k, image::family(op.name)};
        }
    }
    std::optional<py::str> base;
    if (op.base) base = image::family(*op.base);
    SwitchKey k;
    k.kind = py::str("m");
    k.base = base;
    py::str b = base ? *base : py::str("None");
    if (op.index) {
        k.index = image::family(*op.index);
        k.disp = op.disp;
        return {k, py::str("[" + b.utf8() + "+" + image::family(*op.index).utf8() + "*" + std::to_string(op.scale) + signed_alt_hex(op.disp) + "]")};
    }
    k.disp = op.disp;
    return {k, base ? py::str("[" + b.utf8() + signed_alt_hex(op.disp) + "]") : py::str("[" + alt_hex(op.disp) + "]")};
}

py::dict<SwitchKey, Chain> _chains(Image&, const Function& fn) {
    py::dict<SwitchKey, Chain> out;
    std::set<int64_t> labels;
    for (const image::InsP& i : fn.insns.vec()) if ((flow_is(*i, "jcc") || flow_is(*i, "jmp")) && i->target) labels.insert(*i->target);
    struct Acc { SwitchKey key; py::str text; int64_t amount; };
    struct Pending { SwitchKey key; py::str text; int64_t value; };
    std::map<std::string, Acc> acc;
    std::optional<Pending> pending;
    for (int64_t i = 0; i < fn.insns.size(); i++) {
        const image::Ins& ins = *fn.insns[i];
        const py::str& mn = ins.mn;
        const py::list<Op>& ops = ins.ops;
        if (labels.count(ins.addr)) acc.clear();
        if (flow_is(ins, "jcc")) {
            if (pending && (mn == "je" || mn == "jz" || mn == "jne" || mn == "jnz") && ins.target) {
                auto got = out.get(pending->key);
                Chain slot;
                if (got) {
                    slot = *got;
                } else {
                    slot.on = pending->text;
                    out.set(pending->key, slot);
                }
                slot.cases.setdefault(pending->value, (mn == "je" || mn == "jz") ? *ins.target : ins.addr + ins.size);
            }
            continue;   // flags survive a conditional jump
        }
        pending.reset();
        if (mn == "cmp" && ops.size() == 2 && ops[1].kind == "i" && (ops[0].kind == "r" || ops[0].kind == "m")) {
            SwitchKey key;
            py::str text;
            int64_t base = 0;
            auto a = ops[0].kind == "r" ? acc.find(image::family(ops[0].name).utf8()) : acc.end();
            if (ops[0].kind == "r" && a != acc.end()) {
                key = a->second.key;
                text = a->second.text;
                base = a->second.amount;
            } else {
                std::tie(key, text) = _origin(fn, i, ops[0]);
            }
            pending = Pending{key, text, (ops[1].value + base) & 0xFFFFFFFFll};
        } else if ((mn == "sub" || mn == "dec" || mn == "add" || mn == "inc") && !ops.empty() && ops[0].kind == "r" &&
                   (ops.size() == 1 || ops[1].kind == "i")) {
            py::str fam = image::family(ops[0].name);
            if (fam == "esp" || fam == "ebp") continue;
            if (!acc.count(fam.utf8())) {
                auto [key, text] = _origin(fn, i, ops[0]);
                acc[fam.utf8()] = Acc{key, text, 0};
            }
            Acc cur = acc[fam.utf8()];
            int64_t step = mn == "dec" ? 1 : mn == "inc" ? -1 : mn == "sub" ? op_at(ops, 1).value : -image::signed_(op_at(ops, 1).value, op_at(ops, 1).size);
            acc[fam.utf8()] = Acc{cur.key, cur.text, cur.amount + step};
            pending = Pending{cur.key, cur.text, (cur.amount + step) & 0xFFFFFFFFll};
        } else if ((mn == "test" || mn == "or" || mn == "and") && ops.size() == 2 && ops[0] == ops[1] && ops[0].kind == "r") {
            py::str fam = image::family(ops[0].name);
            auto a = acc.find(fam.utf8());
            if (a != acc.end()) {
                pending = Pending{a->second.key, a->second.text, a->second.amount};
            } else {
                auto [key, text] = _origin(fn, i, ops[0]);
                pending = Pending{key, text, 0};
            }
        } else {
            for (const py::str& r : ins.wr) acc.erase(r.utf8());
        }
    }
    return out;
}

// ---- switches ---------------------------------------------------------------------------------------------------------
std::shared_ptr<py::list<Switch>> _switches(Image& img) {
    auto out = std::make_shared<py::list<Switch>>();
    for (const image::FunctionP& fnp : img.each().vec()) {
        const Function& fn = *fnp;
        py::dict<SwitchKey, Chain> chains = _chains(img, fn);
        std::vector<std::optional<SwitchKey>> tabled;
        for (const image::JumpTable& t : fn.tables.vec()) {
            auto at = fn.at(t.site);
            if (!at) throw py::TypeError("unsupported operand type(s) for -: 'NoneType' and 'int'");
            int64_t i = *at;
            std::optional<SwitchKey> key;
            py::str text = t.selector;
            for (int64_t j = i - 1; j > std::max<int64_t>(-1, i - 10); j--) {
                const image::Ins& b = *fn.insns[j];
                if (b.mn == "cmp" && op_at(b.ops, 1).kind == "i") {
                    auto [k, tx] = _origin(fn, j, b.ops[0]);
                    key = k;
                    text = tx;
                    for (int64_t p = j - 1; p > std::max<int64_t>(-1, j - 4); p--) {   // past the `sub reg, low`
                        const image::Ins& q = *fn.insns[p];
                        if ((q.mn == "sub" || q.mn == "dec" || q.mn == "add" || q.mn == "lea") && !q.ops.empty() && q.ops[0].kind == "r" &&
                            b.ops[0].kind == "r" && image::family(q.ops[0].name) == image::family(b.ops[0].name)) {
                            auto [k2, tx2] = _origin(fn, p, q.ops[0]);
                            key = k2;
                            text = tx2;
                        }
                    }
                    break;
                }
            }
            py::dict<int64_t, int64_t> cases;
            if (t.bytes) {
                int64_t n = 0;
                for (int64_t slot : t.bytes->vec()) {
                    if (slot < t.targets.size() && (!t.default_ || t.targets[slot] != *t.default_)) cases.set(t.low + n, t.targets[slot]);
                    n++;
                }
            } else {
                int64_t n = 0;
                for (int64_t tg : t.targets.vec()) cases.set(t.low + n++, tg);
            }
            tabled.push_back(key);
            py::dict<int64_t, int64_t> merged;
            if (key) {
                auto c = chains.get(*key);
                if (c) merged = c->cases.copy();
            }
            merged.update(cases);
            Switch s;
            s.function = fn.start;
            s.on = text;
            s.key = key;
            s.cases = merged;
            s.default_ = t.default_;
            s.table = t.table;
            s.site = t.site;
            s.bounded = t.bounded;
            out->append(s);
        }
        for (const auto& kv : chains.items()) {
            const SwitchKey& key = std::get<0>(kv);
            const Chain& c = std::get<1>(kv);
            bool seen = false;
            for (const auto& tk : tabled) if (tk && *tk == key) seen = true;
            if (!seen && c.cases.size() >= 3) {
                Switch s;
                s.function = fn.start;
                s.on = c.on;
                s.key = key;
                s.cases = c.cases;
                s.bounded = true;
                out->append(s);
            }
        }
    }
    return out;
}

std::shared_ptr<py::list<Switch>> switches(Image& img) {
    return img.memo<py::list<Switch>>(py::str("dispatch.switches"), [](Image& i) { return _switches(i); });
}

bool _is_message_argument(const Image& img, const std::optional<SwitchKey>& key, bool far) {
    if (!key || key->kind != "m" || key->index) return false;
    if (img.bits == 16) return key->base && *key->base == "ebp" && key->disp == (far ? 0x0C : 0x0A);
    return (key->base && *key->base == "ebp" && key->disp == 0x0C) || (key->base && *key->base == "esp" && key->disp == 8);
}

bool _is_command_argument(const Image& img, const std::optional<SwitchKey>& key, bool far) {
    if (!key || key->kind != "m" || key->index) return false;
    if (img.bits == 16) return key->base && *key->base == "ebp" && key->disp == (far ? 0x0A : 0x08);
    return (key->base && *key->base == "ebp" && key->disp == 0x10) || (key->base && *key->base == "esp" && key->disp == 0x0C);
}

// ---- messages ---------------------------------------------------------------------------------------------------------
std::shared_ptr<py::list<MessageDispatch>> _messages(Image& img) {
    auto facts = conventions::functions(img);
    std::map<int64_t, std::vector<Switch>> by_fn;
    for (const Switch& s : switches(img)->vec()) by_fn[s.function].push_back(s);
    auto out = std::make_shared<py::list<MessageDispatch>>();
    for (auto& kv : by_fn) {   // (sorted by function)
        int64_t f = kv.first;
        const conventions::Fact& fact = facts->at(f);
        std::string conv = fact.convention ? fact.convention->utf8() : std::string();
        bool far = conv.size() >= 3 && conv.compare(conv.size() - 3, 3, "far") == 0;
        std::optional<py::str> proc = fact.procedure;
        bool has_proc = proc && proc->size() != 0;
        py::dict<int64_t, int64_t> found, commands;
        std::optional<py::str> why;
        for (const Switch& s : kv.second) {
            std::set<int64_t> values, known, telling;
            for (int64_t v : s.cases.keys()) values.insert(v);
            for (int64_t v : values) if (MESSAGES.contains(v)) known.insert(v);
            for (int64_t v : known) if (v >= 0x0F) telling.insert(v);   // 1, 2, 3 are also just small numbers
            bool on_message = _is_message_argument(img, s.key, far) &&
                              ((fact.removes && *fact.removes == (img.bits == 32 ? 16 : 10)) || !telling.empty());
            if (img.kind == "le") continue;
            bool core = false;
            for (int64_t v : telling) if (kCore.count(v)) core = true;
            if ((on_message && known.size() >= 2) || (has_proc && telling.size() >= 2 && known.size() * 2 >= values.size()) ||
                (telling.size() >= 3 && core && known.size() * 4 >= values.size() * 3)) {
                if (!why || why->size() == 0) {
                    why = on_message ? py::str("switch on the message argument") : has_proc ? py::str("a ") + *proc : py::str("cases are message numbers");
                }
                for (const auto& cv : s.cases.items()) found.set(std::get<0>(cv), std::get<1>(cv));
            } else if (_is_command_argument(img, s.key, far)) {
                for (const auto& cv : s.cases.items()) commands.set(std::get<0>(cv), std::get<1>(cv));
            }
        }
        if (!found.empty()) {
            MessageDispatch m;
            m.function = f;
            m.procedure = proc;
            m.why = why ? *why : py::str();
            m.messages = found;
            m.commands = commands;
            out->append(m);
        }
    }
    return out;
}

std::shared_ptr<py::list<MessageDispatch>> messages(Image& img) {
    return img.memo<py::list<MessageDispatch>>(py::str("dispatch.messages"), [](Image& i) { return _messages(i); });
}

// ---- keywords ---------------------------------------------------------------------------------------------------------
std::shared_ptr<py::list<Keyword>> _keywords(Image& img) {
    auto graph = bridge::graph(img);
    auto out = std::make_shared<py::list<Keyword>>();
    for (int64_t f : std::vector<int64_t>(img.functions.vec())) {
        image::FunctionP fn = img.function(f);
        struct Word { int64_t site; py::str text; py::str api; };
        std::vector<Word> words;
        for (const auto& c : graph->calls.at(f).vec()) {
            if (!c.destination || !img.imports.contains(*c.destination)) continue;
            std::u32string imp = img.imports.at(*c.destination).u32();
            size_t p = imp.rfind(U'!');
            py::str api = py::str(p == std::u32string::npos ? imp : imp.substr(p + 1)).lower();
            if (api.size() > 0) {
                char32_t last = api.u32().back();
                if ((last == U'a' || last == U'w') && comparers().count(api.slice(0, api.size() - 1).utf8())) api = api.slice(0, api.size() - 1);
            }
            if (!comparers().count(api.utf8())) continue;
            auto i = fn->at(c.site);
            py::list<Op> args = image::arguments(*fn, *i, img.bits == 16 ? 4 : 2);
            for (int64_t k : {0, 1}) {
                auto lit = bridge::string_literal(img, args, k);
                if (lit) {
                    words.push_back({c.site, std::get<1>(*lit), api});
                    break;
                }
            }
        }
        std::set<std::string> distinct;
        for (const Word& w : words) distinct.insert(w.text.utf8());
        if (distinct.size() >= 2) {
            Keyword k;
            k.function = f;
            k.compares_with = words[0].api;
            for (const Word& w : words) k.keywords.append(w.text);
            out->append(k);
        }
    }
    // tables in the data that pair a string with a function
    strings::Table text = bridge::string_table(img);
    for (const globals::RawTable& t : bridge::pointer_tables(img).vec()) {
        if (t.shape == "run" && t.count >= 4 && t.count % 2 == 0) {
            bool all = true;
            for (int64_t k = 0; k < t.count; k++) {
                if (!(t.kinds[k] == (k % 2 == 0 ? "string" : "function"))) { all = false; break; }
            }
            if (!all) continue;
            Keyword k;
            k.table = t.at;
            for (int64_t q = 0; q < t.count; q += 2) k.keywords.append(std::get<0>(text.at(t.targets[q])));
            py::list<int64_t> handlers;
            for (int64_t q = 1; q < t.count; q += 2) handlers.append(t.targets[q]);
            k.handlers = handlers;
            out->append(k);
        }
    }
    return out;
}

std::shared_ptr<py::list<Keyword>> keywords(Image& img) {
    return img.memo<py::list<Keyword>>(py::str("dispatch.keywords"), [](Image& i) { return _keywords(i); });
}

// ---- interrupts -------------------------------------------------------------------------------------------------------
py::list<Interrupt> interrupts(Image& img) {
    py::list<Interrupt> out;
    for (const image::FunctionP& fnp : img.each().vec()) {
        const Function& fn = *fnp;
        for (int64_t i = 0; i < fn.insns.size(); i++) {
            const image::Ins& ins = *fn.insns[i];
            if (ins.mn != "int" || ins.ops.empty() || ins.ops[0].kind != "i") continue;
            int64_t n = ins.ops[0].value & 0xFF;
            if (img.kind == "le" && n == 0x20) continue;   // a driver's service call, already an import
            std::optional<int64_t> code;
            for (int64_t q = i - 1; q >= std::max<int64_t>(0, i - 8); q--) {
                const image::Ins& b = *fn.insns[q];
                if (b.flow || b.mn == "int") break;
                if (b.mn == "mov" && b.ops.size() == 2 && b.ops[0].kind == "r" && b.ops[1].kind == "i") {
                    if (b.ops[0].name == "ah") {
                        code = (n != 0x31 && n != 0x2F) ? std::optional<int64_t>(b.ops[1].value) : std::nullopt;
                        break;
                    }
                    if (b.ops[0].name == "ax" || b.ops[0].name == "eax") {
                        code = (n == 0x31 || n == 0x2F) ? (b.ops[1].value & 0xFFFF) : ((b.ops[1].value >> 8) & 0xFF);
                        break;
                    }
                }
            }
            auto fmt = [](const char* f, int64_t v) { char b[32]; std::snprintf(b, sizeof b, f, static_cast<long long>(v)); return std::string(b); };
            std::string name;
            if (n == 0x21) {
                auto it = code ? DOS_.find(*code) : DOS_.end();
                name = "DOS: " + (it != DOS_.end() ? it->second : "function " + (code ? fmt("%02llXh", *code) : std::string("?")));
            } else if (n == 0x31) {
                auto it = code ? DPMI_.find(*code) : DPMI_.end();
                name = "DPMI: " + (it != DPMI_.end() ? it->second : "function " + (code ? fmt("%04llXh", *code) : std::string("?")));
            } else {
                auto it = BIOS_.find(n);
                name = (it != BIOS_.end() ? it->second : "interrupt " + fmt("%02llX", n)) + (code ? " function " + fmt("%02llXh", *code) : std::string());
            }
            Interrupt x;
            x.function = fn.start;
            x.site = ins.addr;
            x.interrupt = n;
            x.code = code;
            x.name = py::str(name);
            out.append(x);
        }
    }
    return out;
}

// ---- control messages -------------------------------------------------------------------------------------------------
std::optional<ControlMessages> control_messages(Image& img) {
    if (img.kind != "le" || !img.device) return std::nullopt;
    auto proc = img.u32(describe::le_address(img.device->obj, img.device->rva) + 0x18);
    if (!proc || !img.names.contains(*proc)) return std::nullopt;
    py::dict<int64_t, int64_t> best;
    for (const Switch& s : switches(img)->vec()) {
        if (s.function == *proc && s.key && s.key->kind == "r" && s.key->base && *s.key->base == "eax") {
            for (const auto& cv : s.cases.items()) best.set(std::get<0>(cv), std::get<1>(cv));
        }
    }
    image::FunctionP fn = img.function(*proc);   // short procedures test one or two messages only
    for (const auto& kv : _chains(img, *fn).items()) {
        const Chain& c = std::get<1>(kv);
        if (c.on == "eax") for (const auto& cv : c.cases.items()) best.setdefault(std::get<0>(cv), std::get<1>(cv));
    }
    std::vector<std::tuple<int64_t, int64_t>> sorted = best.items();
    std::sort(sorted.begin(), sorted.end());
    ControlMessages r;
    r.procedure = *proc;
    for (const auto& vt : sorted) {
        auto nm = CONTROL_MESSAGES.get(std::get<0>(vt));
        r.handles.set(nm ? *nm : py::str("message " + alt_hex(std::get<0>(vt))), std::get<1>(vt));
    }
    return r;
}

// ---- analyse ----------------------------------------------------------------------------------------------------------
Analysis analyse(Image& img) {
    auto sw = switches(img);
    auto msgs = messages(img);
    auto kw = keywords(img);
    py::list<Interrupt> ints = interrupts(img);
    auto ctl = control_messages(img);
    Analysis a;
    auto sorted_cases = [](const py::dict<int64_t, int64_t>& d) {
        std::vector<std::tuple<int64_t, int64_t>> v = d.items();
        std::sort(v.begin(), v.end());
        return v;
    };
    for (const Switch& s : sw->vec()) {
        SwitchFact f;
        f.addr = s.function;
        f.on = s.on;
        f.site = s.site;
        f.table = s.table;
        f.default_ = s.default_;
        for (const auto& cv : sorted_cases(s.cases)) f.cases.set(py::str(alt_hex(std::get<0>(cv))), std::get<1>(cv));
        a.facts.append(AnyFact(f));
    }
    for (const MessageDispatch& m : msgs->vec()) {
        MessageFact f;
        f.addr = m.function;
        f.procedure = m.procedure;
        f.why = m.why;
        for (const auto& cv : sorted_cases(m.messages)) {
            auto nm = MESSAGES.get(std::get<0>(cv));
            f.messages.set(nm ? *nm : py::percent(py::str("%#06x"), std::get<0>(cv)), std::get<1>(cv));
        }
        for (const auto& cv : sorted_cases(m.commands)) f.commands.set(py::percent(py::str("%d"), std::get<0>(cv)), std::get<1>(cv));
        a.facts.append(AnyFact(f));
    }
    for (const Keyword& k : kw->vec()) {
        KeywordFact f;
        f.addr = k.function;
        f.keyword = k;
        a.facts.append(AnyFact(f));
    }
    std::map<int64_t, std::vector<py::str>> by_fn;
    for (const Interrupt& x : ints.vec()) by_fn[x.function].push_back(x.name);
    for (auto& kv : by_fn) {
        ServicesFact f;
        f.addr = kv.first;
        std::set<std::string> uniq;
        for (const py::str& n : kv.second) uniq.insert(n.utf8());
        for (const std::string& n : uniq) f.services.append(py::str(n));
        a.facts.append(AnyFact(f));
    }
    if (ctl) {
        DriverControlFact f;
        f.addr = ctl->procedure;
        f.device = img.device->name;
        f.handles = ctl->handles;
        a.facts.append(AnyFact(f));
    }
    std::vector<const Switch*> tables, chainsw;
    for (const Switch& s : sw->vec()) (s.table ? tables : chainsw).push_back(&s);
    auto put = [&](const char* k, int64_t v) { a.counts.set(py::str(k), v); };
    int64_t jt_cases = 0, bounded = 0, cc_cases = 0, on_arg = 0;
    for (auto* s : tables) { jt_cases += s->cases.size(); if (s->bounded) bounded++; }
    for (auto* s : chainsw) cc_cases += s->cases.size();
    for (const Switch& s : sw->vec()) {
        if (s.key && s.key->kind == "m" && s.key->base && (*s.key->base == "ebp" || *s.key->base == "esp") && s.key->disp > 0) on_arg++;
    }
    int64_t nmsg = 0, named = 0, ncmd = 0, ncmdids = 0, nkw = 0;
    for (const MessageDispatch& m : msgs->vec()) {
        nmsg += m.messages.size();
        for (int64_t v : m.messages.keys()) if (MESSAGES.contains(v)) named++;
        if (!m.commands.empty()) ncmd++;
        ncmdids += m.commands.size();
    }
    for (const Keyword& k : kw->vec()) nkw += k.keywords.size();
    int64_t services = 0;
    for (const Interrupt& x : ints.vec()) {
        if (!x.name.contains(py::str("function ?")) && !x.name.contains(py::str("interrupt "))) services++;
    }
    put("jump_tables", static_cast<int64_t>(tables.size()));
    put("jump_table_cases", jt_cases);
    put("jump_tables_with_bound", bounded);
    put("compare_chains", sw->size() - static_cast<int64_t>(tables.size()));
    put("compare_chain_cases", cc_cases);
    put("switches_on_an_argument", on_arg);
    put("message_dispatchers", msgs->size());
    put("messages_handled", nmsg);
    put("named_messages", named);
    put("command_dispatchers", ncmd);
    put("command_ids", ncmdids);
    put("keyword_dispatchers", kw->size());
    put("keywords", nkw);
    put("interrupt_sites", ints.size());
    put("named_services", services);
    put("driver_control_messages", ctl ? ctl->handles.size() : 0);
    auto join = [](const std::vector<std::string>& v, const char* sep) {
        std::string s;
        for (size_t i = 0; i < v.size(); i++) s += (i ? sep : "") + v[i];
        return s;
    };
    for (int64_t q = 0; q < std::min<int64_t>(6, msgs->size()); q++) {
        const MessageDispatch& m = (*msgs)[q];
        std::vector<int64_t> keys = m.messages.keys();
        std::sort(keys.begin(), keys.end());
        std::vector<std::string> names;
        for (size_t i = 0; i < keys.size() && i < 10; i++) {
            auto nm = MESSAGES.get(keys[i]);
            names.push_back(nm ? nm->utf8() : alt_hex(keys[i]));
        }
        a.text.append(py::str(img.label(m.function).utf8() + " (" + m.why.utf8() + "): " + join(names, ", ")));
    }
    for (int64_t q = 0; q < std::min<int64_t>(4, kw->size()); q++) {
        const Keyword& k = (*kw)[q];
        std::string where = (k.function && *k.function != 0) ? img.label(*k.function).utf8() : "table " + hex_lower(k.table ? *k.table : 0);
        std::vector<std::string> ws;
        for (int64_t i = 0; i < std::min<int64_t>(8, k.keywords.size()); i++) ws.push_back(py::repr(k.keywords[i]).utf8());
        a.text.append(py::str("keywords in " + where + ": " + join(ws, ", ")));
    }
    if (ctl) {
        std::vector<std::string> hs;
        for (const py::str& h : ctl->handles.keys()) hs.push_back(h.utf8());
        a.text.append(py::str("device " + (img.device->name.utf8()) + " control procedure " + img.label(ctl->procedure).utf8() + " handles: " + join(hs, ", ")));
    }
    if (!ints.empty()) {
        std::vector<std::pair<std::string, int64_t>> seen;
        for (const Interrupt& x : ints.vec()) {
            auto it = std::find_if(seen.begin(), seen.end(), [&](auto& p) { return p.first == x.name.utf8(); });
            if (it == seen.end()) seen.emplace_back(x.name.utf8(), 1);
            else it->second++;
        }
        std::stable_sort(seen.begin(), seen.end(), [](auto& x, auto& y) { return x.second > y.second; });
        std::vector<std::string> parts;
        for (size_t i = 0; i < seen.size() && i < 10; i++) parts.push_back(seen[i].first + " x" + std::to_string(seen[i].second));
        a.text.append(py::str("system services: " + join(parts, ", ")));
    }
    return a;
}

}  // namespace codebreaker::analyze::dispatch

size_t std::hash<codebreaker::analyze::dispatch::SwitchKey>::operator()(const codebreaker::analyze::dispatch::SwitchKey& k) const {
    size_t h = std::hash<std::string>()(k.kind.utf8());
    auto mix = [&](size_t x) { h ^= x + 0x9e3779b97f4a7c15ull + (h << 6) + (h >> 2); };
    mix(std::hash<std::string>()(k.base ? k.base->utf8() : std::string("\1")));
    mix(std::hash<std::string>()(k.index ? k.index->utf8() : std::string("\1")));
    mix(std::hash<int64_t>()(k.disp));
    return h;
}
