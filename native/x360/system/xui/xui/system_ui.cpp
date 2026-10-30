// The system UI overlay (see system_ui.h).

#include "xui/system_ui.h"

#include "xui/assets.h"
#include "xui/element.h"
#include "xui/host.h"
#include "xui/xui_tests.h"
#include "xui/xur_write.h"
#include "log.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <ctime>
#include <cwctype>
#include <deque>
#include <format>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <tuple>

namespace xui
{
    namespace
    {
        constexpr float kDesignWidth = 852, kDesignHeight = 480;
        // The gamerscore symbol after an achievement's points: the circled G of the Guide's font (xenonclatin U+E00A;
        // U+E000-E009 are emoticons).
        constexpr char16_t kGamerscoreGlyph[] = u"\uE00A";
        // XAM darkens the title behind its HUD in two layers (measured on the owner's console videos of TU46's popups,
        // Resources/From Patrick/Xbox 360 UI Popups in the Minecraft port; NOTES session 19 job 2): one black layer at
        // once, from the first frame of the opening to the last frame of the closing, and a second one that fades in
        // with the opening and out with the closing; both go in the same frame when the closing animation ends.
        // Message boxes (HUD state Error): ~33% of the brightness kept at once, ~12% when open, the fade over 16 frames.
        // The keyboard: ~21% at once, ~8.5% when open, the fade over 12 frames. Popups no video shows yet (the Guide,
        // sign-in, achievements) keep one layer fading to 0xDC over 16 frames, as before.
        struct DimLayers
        {
            float hold, fade, frames;
        };
        constexpr DimLayers kDimError{ float(0xAB) / 255.0f, float(0xA2) / 255.0f, 16 };
        constexpr DimLayers kDimKeyboard{ float(0xC9) / 255.0f, float(0x99) / 255.0f, 12 };
        // The device selector (Change Storage Device): ~10% at once, ~3.5% open, 12 frames.
        constexpr DimLayers kDimDeviceSelector{ float(0xE5) / 255.0f, float(0xA8) / 255.0f, 12 };
        constexpr DimLayers kDimOther{ 0, float(0xDC) / 255.0f, 16 };

        // XamStrings.xus (huduiskin.xam) entries.
        constexpr size_t kStringBack = 8, kStringSelect = 152;

        std::u16string Utf16(const std::string& s)
        {
            std::u16string out;
            for (size_t i = 0; i < s.size();)
            {
                uint32_t c = uint8_t(s[i]);
                const int extra = c >= 0xF0 ? 3 : c >= 0xE0 ? 2 : c >= 0xC0 ? 1 : 0;
                c &= extra == 3 ? 0x07 : extra == 2 ? 0x0F : extra == 1 ? 0x1F : 0x7F;
                i++;
                for (int k = 0; k < extra && i < s.size(); k++, i++)
                    c = c << 6 | (uint8_t(s[i]) & 0x3F);
                out += char16_t(c);
            }
            return out;
        }

        enum class Phase { kClosed, kOpen, kClosing };

        struct State
        {
            std::mutex mutex;
            bool tried = false;
            std::unique_ptr<Assets> assets;

            // The runtime's own full-screen scene (ShowFullScreenScene; the scenes outlive their elements) and the
            // layers added over it (AddFullScreenLayer).
            std::unique_ptr<Scene> fullScene;
            std::vector<std::unique_ptr<Scene>> fullLayers;
            std::unique_ptr<Element> full;

            // The HUD root (hudbkgnd.xur) and what it hosts.
            std::unique_ptr<Element> hud;       // the canvas; its scene is hudScene
            Element* hudScene = nullptr;        // HUDRootScene: plays the state animations
            Element* hosted = nullptr;          // the popup (a child of its host element)
            Element* host = nullptr;
            std::string hudState;               // "Error" (message boxes) or "Half"
            Phase phase = Phase::kClosed;
            // A popup requested while the previous one plays its closing animation opens when that has finished
            // (the next request reaches the HUD once the previous one is dismissed).
            std::function<void(State&)> pending;
            // A screen asked for by a button's A opens once the button's Press state has played (PlayPress): on the
            // console the next screen comes from another module and the pressed button is seen first (skin: Press ..
            // EndPress, 10 frames). Input waits meanwhile.
            std::function<void(State&)> afterPress;
            float pressLeft = 0; // seconds of the Press state still to play
            // A full scene left for another full one from another module (ShowAfterUnloading): the HUD's panel, legend
            // and clock stay, empty, while the next module loads; then it opens in place.
            std::function<void(State&)> afterLoad;
            float loadLeft = 0;
            // The Community scene was left for a drill-down or the keyboard (its tab's Tab<N>Close brings it back).
            bool communityLeft = false;
            std::vector<std::function<void()>> closed; // ClosePopup callbacks for the animation playing
            std::vector<std::function<void()>> due;    // callbacks to run once the lock is released
            float dim = 0;                       // 0..1 of the fading layer's alpha (dimLayers)
            DimLayers dimLayers = kDimOther;     // the open popup's darkening
            std::vector<Element*> buttons;       // message box buttons in order
            uint32_t focus = 0;

            // The Guide pane open in AppHostElementId (none for message boxes).
            enum class Pane { kNone, kSignIn, kKeyboard, kAchievements, kGuide, kCommunity, kProfile, kGamerZone,
                              kDrill, kDeviceSelector, kWait, kQuickLaunch } // kDrill: a drill-down scene (its controls in buttons)
                pane = Pane::kNone;
            std::u16string clockText;            // what DateTimeTextId shows
            std::unique_ptr<Image> gamerPicture; // hudbkgnd's GamerPic (SetGamerPicture)
            // The Xbox Guide (hud.xex's GuideMainScene) while it is the pane.
            struct Guide
            {
                Element* main = nullptr;         // GuideMain.xur's HUDScene
                Element* tabs = nullptr;         // Tabscene (XuiTabScene: Tab1..Tab<count>)
                Element* tab[5] = {};
                int count = 0;                   // 4 (17559: Games, Home, Media, Settings); NXE 9199 5 (Marketplace first)
                // The blades by their class (each TabN's ClassOverride), indices into tab; -1 if the Guide has none.
                int games = -1, home = -1, media = -1, settings = -1, marketplace = -1;
                // The Blades dashboards' Guide (hud.xex 1888 ... 6770: MainMenuSignedIn / Out.xur): one panel in
                // tab[0], no blades to switch or animate.
                bool blades = false;
                Element* focus = nullptr;
                int cur = 1;
                Element* box = nullptr;          // a message box over the Guide, in ErrorHostElement
                Element* boxHost = nullptr;
                bool boxClosing = false;
            } guide;
            // hud.xex saves the blade and the focused button (+0x68 / +0x6E) only when the Guide opens a scene over
            // itself (0x913E84A0, 0x913E6180 -> 0x913E8130); the next opening goes back there and clears them
            // (0x913E79F8 -> 0x913E75B8), so a Guide closed with B reopens on the default blade.
            int guideTab = -1;                   // -1: GuideMain's Tabscene DefaultTab (17559: 2, Home)
            std::string guideFocus;
            bool guideKeep = false;
            struct Keyboard
            {
                Element* group = nullptr;        // KeysGroup
                Element* edit = nullptr;         // XuiEdit1
                Element* focus = nullptr;
                int mode = 0;                    // kLayouts index: alphabet, symbols, accents
                bool caps = false;
                bool gamertag = false;           // gamertag characters only (ShowKeyboard)
                std::u16string text;
                size_t cursor = 0;
                size_t maxLength = 0;
                std::vector<Element*> pressed;   // keys playing a press without the focus
                // vk.xex 1888 ... 9199's Keyboard.xur: one fixed grid of keys (Key.A ... Key._, each its character
                // as its text) in the scene itself, which is the group; Caps changes the letters' case.
                bool legacy = false;
            } keyboard;
            // The list of a list pane (XuiList: items laid out from its visual's template, TU46's XuiListElement).
            struct List
            {
                Element* list = nullptr;
                std::vector<Element*> slots;     // item controls, children of the list; slot i shows item top + i
                Element* up = nullptr;           // scroll ends (in the list's visual)
                Element* down = nullptr;
                Element* focused = nullptr;      // the slot playing the focus
                float x0 = 0, y0 = 0, itemW = 1, itemH = 1, liveW = 1, liveH = 1, spaceX = 0, spaceY = 0;
                int layout = 0, cols = 1, rows = 1, count = 0, top = 0, cur = 0;
                bool wrap = false;
                bool inactive = false;           // the focus is outside the list (the Party tab's buttons)
                int upState = -1, downState = -1;
                std::function<void(Element* item, int index)> fill;
                std::function<void(int index)> onFocus;
            } list;
            std::vector<std::unique_ptr<Image>> images; // images the open pane shows (gamer pictures, achievement tiles)
            // The real Community scene (community.xur) while it is the pane: its root, the tab shown, the Party tab's
            // buttons and the focus over buttons + list, label_Total (the Friends tab's "n of N").
            struct CommunityScene
            {
                Element* root = nullptr;
                int tab = -1;
                std::vector<Element*> buttons;
                int focus = 0;
                // Each tab's gamer pictures, kept while its list shows them: the tab sliding out still draws its rows
                // (freeing them with the next tab's crashed in SampleBilinear, owner 2026-10-16).
                std::array<std::vector<std::unique_ptr<Image>>, 4> pictures;
                // The shown tab's rows (its list items are filled from them; SetCommunityRowImage changes one).
                std::vector<CommunityRow> rows;
            } community;
            // hud's QuickLaunch.xur while it is the pane (Games & Apps > Recent).
            struct QuickLaunchScene
            {
                Element* tabs = nullptr;         // Tabscene (XuiTabScene: Tab1..Tab3)
                Element* tab[3] = {};
                int cur = 0;
                QuickLaunchInfo info;
                bool hideTrials = false;         // the All tab's Y (0x913F74B8)
                std::vector<int> shown;          // the shown tab's rows: indices into its row vector
                std::array<std::vector<std::unique_ptr<Image>>, 3> pictures;
            } quick;
            // Guide.Friends' ProfileRoot.xur while it is the pane (its tab, for the LB / RB transitions).
            struct ProfileRootScene
            {
                Element* root = nullptr;
                int tab = -1;
            } profileRoot;
            // A drill-down's reaction to its focus moving (the recipients' gamercard follows the focused friend).
            std::function<void(State&, uint32_t)> buttonFocus;

            // Notification toasts (notify.xur): the queue, the one showing, where it is drawn.
            struct Toast { uint32_t type, user; std::u16string param; };
            std::deque<Toast> toasts;
            std::unique_ptr<Element> toastScene; // notify.xur canvas while a toast shows
            Element* toastControl = nullptr;
            float toastTime = 0;
            bool toastClosing = false;
            int toastX = 0, toastY = 0;          // HUD-space offset of the canvas
            uint32_t notifyPosition = 2;         // XNOTIFYUI_POS_BOTTOMCENTER (XAM resets it when a title starts)

            uint64_t lastVblank = 0;
            bool dirty = true;
            std::unique_ptr<Canvas> canvas;      // the overlay's render target, kept between frames
            std::vector<uint8_t> last;
            uint32_t lastWidth = 0, lastHeight = 0;
        };

        using Pane = State::Pane;

        State& S()
        {
            static State s;
            return s;
        }

        // Runs the callbacks that became due under the lock once it is released (declare it before the lock).
        struct RunDue
        {
            State& s;
            ~RunDue()
            {
                std::vector<std::function<void()>> run;
                {
                    std::lock_guard lock(s.mutex);
                    run.swap(s.due);
                }
                for (auto& f : run)
                    f();
            }
        };

        // Caller holds the mutex.
        bool Ready(State& s)
        {
            if (!s.tried)
            {
                s.tried = true;
                auto assets = std::make_unique<Assets>();
                std::string error;
                const auto dir = AssetDirectory();
                if (!assets->Load(dir, error))
                    Log("System UI: no XAM assets ({}: {}); popups are shown as native dialogs. Create the folder "
                        "with tools/scripts/extract_system_ui.py from the console's system update.", dir.string(), error);
                else
                {
                    s.assets = std::move(assets);
                    Log("System UI: drawing the Guide's popups from XAM's assets in {}", dir.string());
                }
            }
            return s.assets != nullptr;
        }

        // A package's 17559 copy: the base folder's when an older system update's replaced it (Assets::ReplacePackages),
        // for the string tables read by 17559's indices.
        std::string Base17559(State& s, const std::string& package)
        {
            const std::string base = Assets::kBasePrefix + package;
            return s.assets->FindPackage(base) ? base : package;
        }

        std::u16string XamString(State& s, size_t index, const char16_t* fallback)
        {
            const auto* table = s.assets->Strings(Base17559(s, "huduiskin.xam"), "XamStrings.xus");
            return table && index < table->size() ? Utf16((*table)[index]) : std::u16string(fallback);
        }

        // hud.xex's string table (Strings.xus).
        std::u16string HudStringOf(State& s, size_t index, const char16_t* fallback)
        {
            const auto* table = s.assets->Strings(Base17559(s, "hud.hud"), "Strings.xus");
            return table && index < table->size() ? Utf16((*table)[index]) : std::u16string(fallback);
        }

        // Builds the HUD root scene if needed.
        bool EnsureHud(State& s)
        {
            if (s.hud)
                return true;
            // XAM's (17559, and the updates before 8495) or, from 8495 to 9199, huduiskin.xex's "xam" package's.
            const char* package = "xam.xam";
            if (const Package* xam = s.assets->FindPackage(package); !xam || !xam->Find("hudbkgnd.xur"))
                package = "huduiskin.xam";
            const Scene* scene = s.assets->LoadScene(package, "hudbkgnd.xur");
            if (!scene)
                return false;
            s.hud = Element::Create(*s.assets, *scene, scene->Root(), package, nullptr);
            s.hudScene = s.hud->Find("HUDRootScene");
            // The Blades HUD's blade label (GamerTag) holds the author's placeholder ("SniperMonkey"): empty until
            // the Guide names the signed-in gamer (the sign-in pane opened from a dashboard shows no name).
            if (s.hudScene)
                if (Element* tag = s.hudScene->Find("GamerTag"))
                    tag->SetString("Text", "");
            return s.hudScene != nullptr;
        }

        void DropPopup(State& s)
        {
            if (s.guide.box && s.guide.boxHost)
                s.guide.boxHost->Release(s.guide.box);
            if (s.hosted && s.host)
                s.host->Release(s.hosted);
            s.hosted = nullptr;
            s.host = nullptr;
            s.buttons.clear();
            // The pane state points into the popup's elements.
            s.pane = State::Pane::kNone;
            s.keyboard = {};
            s.list = {};
            s.guide = {};
            s.images.clear();
        }

        // Opens the HUD in `state` ("Error", "Half") hosting `popup` in `hostId`.
        void Open(State& s, const std::string& state, const std::string& hostId, std::unique_ptr<Element> popup)
        {
            DropPopup(s);
            s.afterLoad = nullptr; // another popup came first
            s.loadLeft = 0;
            s.community = {};
            s.profileRoot = {};
            s.buttonFocus = {};
            s.host = s.hudScene->Find(hostId);
            if (!s.host)
            {
                Log("System UI: hudbkgnd.xur has no {}", hostId);
                return;
            }
            const float w = popup->Width(), h = popup->Height();
            popup->SetPosition(0, 0);
            popup->SetSize(w, h);
            s.hosted = s.host->Adopt(std::move(popup));
            const std::string from = s.phase == Phase::kClosed || s.hudState.empty() ? "Closed" : s.hudState;
            if (from != state)
                s.hudScene->PlayNamed(from + "To" + state, "End" + from + "To" + state, true);
            s.hudState = state;
            s.dimLayers = state == "Error" ? kDimError : kDimOther;
            s.phase = Phase::kOpen;
            s.dirty = true;
        }

        // Opens a popup now, after the closing animation of the previous one, or after a pressed button's Press.
        void ShowWhenClosed(State& s, std::function<void(State&)> show)
        {
            if (s.phase == Phase::kClosing)
                s.pending = std::move(show);
            else if (s.phase == Phase::kOpen && s.pressLeft > 0)
                s.afterPress = std::move(show);
            else
                show(s);
        }

        // Opens a popup after the open one has closed with its HUD's closing animation (two XAM UIs in a row, each
        // opening and closing on its own), after a pressed button's Press.
        void ShowAfterClosing(State& s, std::function<void(State&)> show)
        {
            ShowWhenClosed(s, [show = std::move(show)](State& s) {
                if (s.phase != Phase::kOpen || !s.hudScene || s.hudState.empty())
                    return show(s);
                s.hudScene->PlayNamed(s.hudState + "ToClosed", "End" + s.hudState + "ToClosed", true);
                s.phase = Phase::kClosing;
                s.pending = show;
                s.dirty = true;
            });
        }

        // The HUD state a pane scene opens in (OpenPane): its OpenType 2 Full, else Half (empty: no such scene).
        std::string PaneState(State& s, std::string_view package, std::string_view entry);

        // Opens a pane after the open one has been unloaded, after a pressed button's Press, when both are the same
        // size (the open HUD state is `state`, the next scene's): the console keeps the HUD's panel (legend and clock
        // too), drops the scene at once, shows the empty panel while the next module loads (`seconds`) and then shows
        // the next scene in it, without a HUD animation (owner's "create profile.mp4": Select a Device -> 107 frames
        // empty -> Enter Profile Name). A popup of another size closes and the next opens as ShowAfterClosing.
        void ShowAfterUnloading(State& s, const std::string& state, float seconds, std::function<void(State&)> show)
        {
            ShowWhenClosed(s, [state, seconds, show = std::move(show)](State& s) {
                if (s.phase != Phase::kOpen || state.empty() || s.hudState != state || s.pane == State::Pane::kGuide)
                    return ShowAfterClosing(s, show);
                DropPopup(s);
                s.community = {};
                s.profileRoot = {};
                s.buttonFocus = {};
                s.afterLoad = show;
                s.loadLeft = seconds;
                s.dirty = true;
            });
        }

        // A's Press state on a control (XuiControlPlayOptionalVisual "Press" .. "EndPress"); a screen the press opens
        // waits for it (ShowWhenClosed).
        void PlayPress(State& s, Element* control)
        {
            control->PlayVisualState("Press");
            s.pressLeft = 10.0f / 60.0f;
            s.dirty = true;
        }

        void Focus(State& s, uint32_t index, bool initial)
        {
            if (s.buttons.empty())
                return;
            index = std::min<uint32_t>(index, uint32_t(s.buttons.size() - 1));
            for (uint32_t i = 0; i < s.buttons.size(); i++)
            {
                Element* b = s.buttons[i];
                const bool now = i == index, was = b->focused;
                b->focused = now;
                if (now && (initial || !was))
                    b->PlayVisualState(initial ? "InitFocus" : "Focus");
                else if (!now && was)
                    b->PlayVisualState("KillFocus");
                else if (!now && initial)
                    b->PlayVisualState("Normal");
            }
            s.focus = index;
            s.dirty = true;
        }

        std::string Utf8(const std::u16string& text)
        {
            std::string out;
            for (size_t i = 0; i < text.size(); i++)
            {
                uint32_t c = text[i];
                if (c >= 0xD800 && c < 0xDC00 && i + 1 < text.size())
                    c = 0x10000 + ((c - 0xD800) << 10) + (text[++i] - 0xDC00);
                if (c < 0x80)
                    out += char(c);
                else if (c < 0x800)
                    out += { char(0xC0 | c >> 6), char(0x80 | (c & 0x3F)) };
                else if (c < 0x10000)
                    out += { char(0xE0 | c >> 12), char(0x80 | ((c >> 6) & 0x3F)), char(0x80 | (c & 0x3F)) };
                else
                    out += { char(0xF0 | c >> 18), char(0x80 | ((c >> 12) & 0x3F)), char(0x80 | ((c >> 6) & 0x3F)),
                             char(0x80 | (c & 0x3F)) };
            }
            return out;
        }

        // XuiTextElementSetText: a text element's Text property; a control's text.
        void SetText(Element* e, const std::u16string& text)
        {
            if (!e)
                return;
            if (e->ClassName() == "XuiText")
                e->SetString("Text", Utf8(text));
            else
                e->text = text;
        }

        void SetShow(Element* e, bool show)
        {
            if (e)
                e->SetBool("Show", show);
        }

        // The HUD legend (hudbkgnd's Legend group), as XAM packs it (0x81692CC0): for A, B, X, Y in order, a button
        // with text shows both, the button at the running x, its text right after it sized to the measured text, then
        // 8 units of space; buttons without text are hidden. (XAM also narrows texts that overflow the 560-unit group.)
        void SetLegend(State& s, const std::u16string (&texts)[4])
        {
            Element* legend = s.hudScene->Find("Legend");
            if (!legend)
            {
                // The Blades HUD (xam.xex 1888 ... 6770's hudbkgnd.xur): one control per button on its blade,
                // Legend_A ... Legend_Y, each with its text.
                static const char* const kControls[4] = { "Legend_A", "Legend_B", "Legend_X", "Legend_Y" };
                for (int i = 0; i < 4; i++)
                    if (Element* c = s.hudScene->Find(kControls[i]))
                    {
                        c->text = texts[i];
                        SetShow(c, !texts[i].empty());
                    }
                s.dirty = true;
                return;
            }
            static const char* const kButtons[4] = { "AButton", "BButton", "XButton", "YButton" };
            static const char* const kTexts[4] = { "AText", "BText", "XText", "YText" };
            float x = 0;
            for (int i = 0; i < 4; i++)
            {
                Element* button = legend->Find(kButtons[i]);
                Element* text = legend->Find(kTexts[i]);
                if (!button || !text)
                    continue;
                const bool visible = !texts[i].empty();
                SetShow(button, visible);
                SetShow(text, visible);
                if (!visible)
                    continue;
                button->SetPosition(x, 0);
                x += button->Width();
                SetText(text, texts[i]);
                const float measured = float(text->MeasureText(*s.assets, texts[i]));
                text->SetSize(measured, text->Height());
                text->SetPosition(x, 0);
                x += measured + 8;
            }
            s.dirty = true;
        }

        // The HUD clock (0x81693208, every second): XamFormatTimeString of the local time. The console's US formats:
        // XamStrings #176 "%d:%0*d" with the 24-hour clock (XCONFIG retail flag 8, which follows the PC's setting),
        // else #182 "%d:%0*d %s" with the 12-hour hour and " AM" / " PM" (#173 / #178).
        std::u16string ClockText(State& s)
        {
            const std::time_t t = std::time(nullptr);
            std::tm now{};
            localtime_s(&now, &t);
            const bool twentyFour = host::TwentyFourHourClock();
            const int hour = twentyFour ? now.tm_hour : now.tm_hour % 12 == 0 ? 12 : now.tm_hour % 12;
            std::u16string text = Utf16(std::format("{}:{:02}", hour, now.tm_min));
            if (!twentyFour)
                text += XamString(s, now.tm_hour >= 12 ? 178 : 173, now.tm_hour >= 12 ? u" PM" : u" AM");
            return text;
        }

        // ---- Guide panes (HUD scenes hosted in hudbkgnd's AppHostElementId) ----

        // Opens `entry` of `package`: its HUDScene hosted in AppHostElementId, hudbkgnd in the state of its OpenType
        // (1 Half, 2 Full), the legend from its LegendA..LegendY. Returns the scene.
        Element* OpenPane(State& s, std::string_view package, std::string_view entry)
        {
            const Scene* scene = s.assets->LoadScene(package, entry);
            if (!scene || scene->Root().children.empty())
                return nullptr;
            // Leaving the Community for one of its drill-downs or the keyboard (CommunityRootScene: Tab<N>Open).
            if (s.phase == Phase::kOpen && s.pane == State::Pane::kCommunity && s.community.root &&
                s.hosted == s.community.root && entry != "community.xur")
                s.communityLeft = true;
            auto pane = Element::Create(*s.assets, *scene, scene->Root().children.front(), std::string(package),
                                        nullptr);
            Element* p = pane.get();
            // A plain XuiScene (Guide.Friends' PartySettings, MessageType, Bio ...) is navigated to inside the
            // Community's full HUD scene: full as well.
            const bool full = pane->GetInt("OpenType", 1) == 2 || pane->ClassName() != "HUDScene";
            const std::u16string legend[4] = { Utf16(pane->GetString("LegendA")), Utf16(pane->GetString("LegendB")),
                                               Utf16(pane->GetString("LegendX")), Utf16(pane->GetString("LegendY")) };
            Open(s, full ? "Full" : "Half", "AppHostElementId", std::move(pane));
            SetLegend(s, legend);
            s.clockText.clear();
            if (Element* picture = s.hudScene->Find("GamerPic"))
                picture->image = s.gamerPicture.get();
            if (const std::string trans = p->GetString("TransTo"); !trans.empty())
                p->PlayNamed(trans, "End" + trans, true);
            return p;
        }

        std::string PaneState(State& s, std::string_view package, std::string_view entry)
        {
            const Scene* scene = s.assets->LoadScene(package, entry);
            if (!scene || scene->Root().children.empty())
                return {};
            const auto pane = Element::Create(*s.assets, *scene, scene->Root().children.front(), std::string(package),
                                              nullptr);
            return pane->GetInt("OpenType", 1) == 2 || pane->ClassName() != "HUDScene" ? "Full" : "Half";
        }

        // Loads `entry`'s scene into the element `host` of an open pane (KeyboardBase.xur into KeyboardPlaceholder,
        // the user list into Scene_Pane0): the scene element keeps its own position in the host.
        Element* LoadInto(State& s, Element* host, std::string_view package, std::string_view entry)
        {
            const Scene* scene = host ? s.assets->LoadScene(package, entry) : nullptr;
            if (!scene || scene->Root().children.empty())
                return nullptr;
            return host->Adopt(Element::Create(*s.assets, *scene, scene->Root().children.front(), std::string(package),
                                               host));
        }

        // ---- Keyboard (vk.xex: KeyboardMain.xur hosting KeyboardBase.xur) ----

        // vk.xex's Latin key layouts (17559, image offsets 0x1A88 / 0x1B10 / 0x1C20): the characters of keys 0_0..4_9
        // row by row (40 keys = rows 0-3, 50 = rows 0-4), then the virtual keys of the side keys (VK_LEFT, VK_RIGHT,
        // LT, RT, VK_RETURN, VK_SPACE, VK_BACK and VK_CAPITAL, which the symbols layout lacks). U+00BD / U+00BE are
        // VK_OEM_MINUS / VK_OEM_PERIOD ('-' and '.' on the console's US layout, as the owner's capture shows); 0x80 in
        // the symbols layout is taken as the euro sign (cp1252 0x80).
        struct KeyLayout { const char16_t* keys; bool caps; };
        constexpr KeyLayout kLayouts[3] = {
            { u"1234567890qwertyuiopasdfghjkl½zxcvbnm_@¾", true },
            { u"1234567890,;:'\"!?¡¿%[]{}`$£«»#<>()\u0080¥¤~^\\|=*/+-@_&.", false },
            { u"1234567890àáâãäåñèéêëþçýì"
              u"íîïùúûòóôõðø×œæßÿ"
              u"ºª.üµš_ö", true },
        };

        char16_t KeyChar(char16_t c, bool caps)
        {
            if (c == 0x00BD)
                return u'-';
            if (c == 0x00BE)
                return u'.';
            if (c == 0x0080)
                return 0x20AC;
            if (!caps)
                return c;
            if ((c >= u'a' && c <= u'z') || (c >= 0xE0 && c <= 0xFE && c != 0xF7))
                return char16_t(c - 0x20);
            if (c == 0x0153)
                return 0x0152;
            if (c == 0x0161)
                return 0x0160;
            if (c == 0x00FF)
                return 0x0178;
            return c;
        }

        // vk's string table (strings.xus: string, id pairs).
        std::u16string VkString(State& s, size_t index, const char16_t* fallback)
        {
            const auto* table = s.assets->Strings(Base17559(s, "vk.vk"), "strings.xus");
            return table && index < table->size() ? Utf16((*table)[index]) : std::u16string(fallback);
        }
        constexpr size_t kVkAccents = 0, kVkAlphabet = 1, kVkBackspace = 2, kVkCaps = 3, kVkDone = 18, kVkLeft = 54,
                         kVkRight = 55, kVkSpace = 65, kVkSymbols = 66;

        // A gamertag's characters: letters, digits and spaces.
        bool GamertagChar(char16_t c)
        {
            return (c >= u'a' && c <= u'z') || (c >= u'A' && c <= u'Z') || (c >= u'0' && c <= u'9') || c == u' ';
        }

        // A key greyed (the skin's ...Disable states; not pressable) or not.
        void SetKeyEnabled(Element* key, bool enabled)
        {
            if (!key || key->enabled == enabled)
                return;
            key->enabled = enabled;
            key->PlayVisualState(key->focused ? (enabled ? "Focus" : "FocusDisable") : (enabled ? "Normal" : "NormalDisable"));
        }

        void KeyboardRefreshEdit(State& s);

        void KeyboardRefresh(State& s)
        {
            auto& k = s.keyboard;
            if (k.legacy)
            {
                for (const auto& key : k.group->Children())
                    if (key->Id().size() == 5 && key->Id().starts_with("Key.") && key->text.size() == 1)
                    {
                        const char16_t c = key->text[0];
                        if (c >= u'a' && c <= u'z' && k.caps)
                            key->text[0] = char16_t(c - 0x20);
                        else if (c >= u'A' && c <= u'Z' && !k.caps)
                            key->text[0] = char16_t(c + 0x20);
                    }
                return KeyboardRefreshEdit(s);
            }
            const KeyLayout& layout = kLayouts[k.mode];
            const std::u16string keys = layout.keys;
            for (int row = 0; row < 5; row++)
                for (int col = 0; col < 10; col++)
                    if (Element* key = k.group->Find(std::format("Key.{}_{}", row, col)))
                    {
                        const size_t i = size_t(row * 10 + col);
                        SetShow(key, i < keys.size());
                        key->text = i < keys.size() ? std::u16string(1, KeyChar(keys[i], k.caps && layout.caps)) : u"";
                        SetKeyEnabled(key, !k.gamertag || (!key->text.empty() && GamertagChar(key->text[0])));
                    }
            // A gamertag has no symbols or accents (owner's Patch Notes: greyed on the Add Friend keyboard).
            SetKeyEnabled(k.group->Find("Key.Prev"), !k.gamertag);
            SetKeyEnabled(k.group->Find("Key.Next"), !k.gamertag);
            SetText(k.group->Find("Key.Left"), VkString(s, kVkLeft, u"Left"));
            SetText(k.group->Find("Key.Right"), VkString(s, kVkRight, u"Right"));
            SetText(k.group->Find("Key.Prev"), k.mode == 1 ? VkString(s, kVkAlphabet, u"Alphabet") :
                                                            VkString(s, kVkSymbols, u"Symbols"));
            SetText(k.group->Find("Key.Next"), k.mode == 2 ? VkString(s, kVkAlphabet, u"Alphabet") :
                                                            VkString(s, kVkAccents, u"Accents"));
            SetText(k.group->Find("Key.Caps"), VkString(s, kVkCaps, u"Caps"));
            SetKeyEnabled(k.group->Find("Key.Caps"), layout.caps);
            SetText(k.group->Find("Key.OK"), VkString(s, kVkDone, u"Done"));
            SetText(k.group->Find("Key.BS"), VkString(s, kVkBackspace, u"Backspace"));
            SetText(k.group->Find("Key.Spc"), VkString(s, kVkSpace, u"Space"));
            KeyboardRefreshEdit(s);
        }

        void KeyboardRefreshEdit(State& s)
        {
            auto& k = s.keyboard;
            // The edit shows the text (its presenter is bound to association 1) and the caret after the cursor's
            // character: scr_Edit places the caret control 4 units after the text's end (Text at x 6, Caret at
            // x 10 with no text); its visual draws the bar 4 units into it (owner: the bar sat on the letter).
            if (k.edit)
            {
                k.edit->text = k.text;
                k.edit->dataText[1] = k.text;
                Element* presenter = k.edit->FindInVisual("Text");
                Element* caret = k.edit->FindInVisual("Caret");
                if (presenter && caret)
                {
                    const Vec3 p = presenter->GetVector("Position", {});
                    const float x = p.x + float(presenter->MeasureText(*s.assets, k.text.substr(0, k.cursor)));
                    caret->SetPosition(x + 4, caret->GetVector("Position", {}).y);
                }
            }
            s.dirty = true;
        }

        void KeyboardFocus(State& s, Element* key, bool initial)
        {
            auto& k = s.keyboard;
            if (!key || key == k.focus)
                return;
            if (k.focus)
            {
                k.focus->focused = false;
                k.focus->PlayVisualState(k.focus->enabled ? "KillFocus" : "NormalDisable");
            }
            k.focus = key;
            key->focused = true;
            key->PlayVisualState(initial ? "InitFocus" : key->enabled ? "Focus" : "FocusDisable");
            s.dirty = true;
        }

        // XUI navigation: the control named by the focused one's NavLeft/NavRight/NavUp/NavDown; a hidden target
        // passes the navigation on in the same direction.
        void KeyboardNavigate(State& s, const char* direction)
        {
            auto& k = s.keyboard;
            Element* at = k.focus;
            for (int step = 0; at && step < 12; step++)
            {
                const std::string next = at->GetString(direction);
                at = next.empty() ? nullptr : k.group->Find(next);
                if (at && at->Shown())
                    break;
            }
            if (at && at->Shown())
                KeyboardFocus(s, at, false);
        }

        void KeyboardType(State& s, char16_t c)
        {
            auto& k = s.keyboard;
            if (k.text.size() >= k.maxLength)
                return;
            k.text.insert(k.text.begin() + std::ptrdiff_t(k.cursor), c);
            k.cursor++;
        }

        void KeyboardPressKey(State& s, Element* key, KeyboardAction& action)
        {
            auto& k = s.keyboard;
            if (!key || !key->enabled)
                return;
            // A key pressed through its controller button while another one has the focus plays NormalPress, then
            // returns to Normal; the focused key plays Press, then returns to Focus (its EndPress frame is the
            // bright green: owner's report, it stayed so).
            key->PlayVisualState(key->focused ? "Press" : "NormalPress");
            if (std::find(k.pressed.begin(), k.pressed.end(), key) == k.pressed.end())
                k.pressed.push_back(key);
            const std::string& id = key->Id();
            if (id == "Key.OK")
                action = KeyboardAction::kDone;
            else if (id == "Key.BS")
            {
                if (k.cursor > 0)
                    k.text.erase(--k.cursor, 1);
            }
            else if (id == "Key.Spc")
                KeyboardType(s, u' ');
            else if (id == "Key.Left")
                k.cursor = k.cursor > 0 ? k.cursor - 1 : 0;
            else if (id == "Key.Right")
                k.cursor = std::min(k.text.size(), k.cursor + 1);
            else if (id == "Key.Prev" && !k.legacy)
                k.mode = k.mode == 1 ? 0 : 1;
            else if (id == "Key.Next" && !k.legacy)
                k.mode = k.mode == 2 ? 0 : 2;
            else if ((id == "Key.Prev" || id == "Key.Next") && k.legacy)
                Log("System UI: keyboard {}: the older keyboard's other key pages are not known yet", id);
            else if (id == "Key.Caps")
                k.caps = !k.caps;
            else if (!key->text.empty())
                KeyboardType(s, key->text[0]);
            KeyboardRefresh(s);
            Log("System UI: keyboard {}: \"{}\" (cursor {})", id, Utf8(k.text), k.cursor);
        }

        // Keeps the caret blinking (its Focus timeline runs once).
        void KeyboardTick(State& s)
        {
            auto& k = s.keyboard;
            if (s.pane != Pane::kKeyboard || !k.edit)
                return;
            if (Element* caret = k.edit->FindInVisual("Caret"); caret && !caret->Playing())
                caret->PlayVisualState("Focus");
            std::erase_if(k.pressed, [](Element* key) {
                if (key->Playing())
                    return false;
                key->PlayVisualState(key->focused ? (key->enabled ? "Focus" : "FocusDisable") : "Normal");
                return true;
            });
        }

        // ---- Lists (TU46's XuiListElement: GenerateListParts, LayoutListParts, GetItemSizeAndPosition, navigation) ----

        // How many items fit (SetupLayoutInformation): q = (extent - start) / (item + spacing), rounded down unless the
        // partial item shows at least a whole item's size; at least 1.
        int ListFit(float extent, float start, float item, float spacing)
        {
            const float step = item + spacing;
            const float q = (extent - start) / step;
            const float whole = std::floor(q);
            const int n = (q - whole) * step < item ? int(whole) : int(std::ceil(q));
            return std::max(1, n);
        }

        // Visual states of the slots and scroll ends after the selection or the top item changed.
        void ListRefresh(State& s, bool initial)
        {
            auto& v = s.list;
            const int slots = int(v.slots.size());
            for (int i = 0; i < slots; i++)
            {
                const int index = v.top + i;
                Element* slot = v.slots[size_t(i)];
                SetShow(slot, index < v.count);
                if (index < v.count && v.fill)
                    v.fill(slot, index);
            }
            Element* now = !v.inactive && v.cur - v.top >= 0 && v.cur - v.top < slots ? v.slots[size_t(v.cur - v.top)]
                                                                                    : nullptr;
            if (now != v.focused || initial)
            {
                if (v.focused && v.focused != now)
                {
                    v.focused->focused = false;
                    v.focused->PlayVisualState(v.focused->enabled ? "KillFocus" : "NormalDisable");
                }
                if (now)
                {
                    now->focused = true;
                    now->PlayVisualState(std::string(initial ? "InitFocus" : "Focus") + (now->enabled ? "" : "Disable"));
                }
                v.focused = now;
            }
            // XuiScrollEnd: ScrollMore while items are hidden in its direction, else its standard visual.
            const int up = v.top > 0 ? 1 : 0, down = v.top + slots < v.count ? 1 : 0;
            if (v.up && up != v.upState)
                v.up->PlayVisualState(up ? "ScrollMore" : "Normal");
            if (v.down && down != v.downState)
                v.down->PlayVisualState(down ? "ScrollMore" : "Normal");
            v.upState = up;
            v.downState = down;
            if (v.onFocus)
                v.onFocus(v.cur);
            s.dirty = true;
        }

        // Builds the list's slots from the template item of its visual (control_ListItem, which is then removed):
        // item size = the template's authored size (the live width for vertical lists), slot positions per layout
        // (0 vertical, 4 row-major grid) from the template's position, InterItemSpacing between them. `visual` /
        // `height` (when given) replace the template's visual and height: the slots are that skin visual instead.
        // `inactive`: no slot has the focus yet (it is on controls outside the list).
        void ListBuild(State& s, Element* list, int count, int focus, std::function<void(Element*, int)> fill,
                       std::function<void(int)> onFocus = {}, const std::string& visual = {}, float height = 0,
                       bool inactive = false)
        {
            auto& v = s.list;
            v = {};
            v.list = list;
            v.inactive = inactive;
            Element* listVisual = list ? list->Visual() : nullptr;
            Element* item = nullptr;
            if (listVisual)
                for (const auto& c : listVisual->Children())
                {
                    if (c->ClassName() == "XuiListItem" && !item)
                        item = c.get();
                    else if (c->ClassName() == "XuiScrollEnd")
                        (c->GetInt("Direction", 0) & 1 ? v.down : v.up) = c.get();
                }
            if (!item)
            {
                Log("System UI: list {} has no item template", list ? list->Id() : std::string());
                return;
            }
            SetShow(item, false);
            const Vec3 p = item->GetVector("Position", {});
            const Vec3 spacing = item->GetVector("InterItemSpacing", {});
            v.x0 = p.x;
            v.y0 = p.y;
            v.itemW = std::max(1.0f, item->DesignWidth());
            v.itemH = std::max(1.0f, height > 0 ? height : item->DesignHeight());
            v.liveW = item->Width();
            v.liveH = item->Height();
            v.spaceX = std::max(0.0f, spacing.x);
            v.spaceY = std::max(0.0f, spacing.y);
            v.layout = int(item->GetInt("Layout", 0));
            v.wrap = list->GetBool("Wrap", false);
            v.rows = ListFit(list->Height(), v.y0, v.itemH, v.spaceY);
            v.cols = v.layout == 4 ? ListFit(list->Width(), v.x0, v.itemW, v.spaceX) : 1;
            v.count = count;
            v.fill = std::move(fill);
            v.onFocus = std::move(onFocus);
            const std::string visualName = visual.empty() ? item->GetString("Visual") : visual;
            for (int i = 0; i < v.rows * v.cols; i++)
            {
                const float w = v.layout == 4 ? v.itemW : v.liveW;
                auto slot = Element::CreateControl(*s.assets, "XuiListItem", visualName, w, v.itemH);
                if (v.layout == 4)
                    slot->SetPosition(v.x0 + float(i % v.cols) * (v.itemW + v.spaceX),
                                      v.y0 + float(i / v.cols) * (v.itemH + v.spaceY));
                else
                    slot->SetPosition(v.x0, v.y0 + float(i) * (v.itemH + v.spaceY));
                slot->PlayVisualState("Normal");
                v.slots.push_back(list->Adopt(std::move(slot)));
            }
            list->PlayVisualState("Focus");
            v.cur = std::clamp(focus, 0, std::max(0, count - 1));
            const int stride = v.cols, slots = v.rows * v.cols;
            if (v.cur >= slots)
                v.top = std::min(v.cur / stride * stride - (v.rows - 1) * stride,
                                 std::max(0, (count - slots + stride - 1) / stride * stride));
            ListRefresh(s, true);
        }

        // XuiListElement navigation: up/down move by a row (scrolling one row at the page edge; with Wrap, up from the
        // first item goes to the last and down from the last to the first); left/right move inside a grid row.
        void ListMove(State& s, int dx, int dy)
        {
            auto& v = s.list;
            if (!v.list || v.count == 0)
                return;
            const int stride = v.cols, slots = int(v.slots.size());
            const int topLimit = std::max(0, (v.count - slots + stride - 1) / stride * stride);
            const int before = v.cur;
            if (dx < 0 && v.cur % stride > 0)
                v.cur--;
            else if (dx > 0 && v.cur % stride < stride - 1 && v.cur + 1 < v.count)
                v.cur++;
            else if (dy < 0)
            {
                if (v.cur - stride >= 0)
                {
                    v.cur -= stride;
                    if (v.cur < v.top)
                        v.top = std::max(0, v.top - stride);
                }
                else if (v.wrap && stride == 1)
                {
                    v.cur = v.count - 1;
                    v.top = topLimit;
                }
                else
                    return;
            }
            else if (dy > 0)
            {
                if (v.cur / stride < (v.count - 1) / stride)
                {
                    v.cur = std::min(v.cur + stride, v.count - 1);
                    if (v.cur >= v.top + slots)
                        v.top = std::min(topLimit, v.top + stride);
                }
                else if (v.wrap)
                {
                    v.cur %= stride;
                    v.top = 0;
                }
                else
                    return;
            }
            else
                return;
            Element* const focusedBefore = v.focused;
            ListRefresh(s, false);
            // XUI's list recycles its item controls: a scroll puts the focus on the control the next item went to,
            // which plays Focus (and its sound). Ours keeps the same slot, so it plays Focus again (owner: the move
            // sound stopped while the sign-in list scrolled).
            if (v.cur != before && v.focused && v.focused == focusedBefore)
            {
                // A fresh control's sound has no File yet: clear ours, so a quick second move sounds too.
                if (Element* visual = v.focused->Visual())
                    for (const auto& c : visual->Children())
                        if (c->ClassName() == "XuiSoundXAudio")
                            c->SetString("File", "");
                v.focused->PlayVisualState(v.focused->enabled ? "Focus" : "FocusDisable");
            }
        }

        // ---- Message boxes ----

        // XAM's message box: a XuiMessageBox drawn through the skin visual for its button count (XuiMessageBox2..4,
        // whose last button is the hidden B handler). `found` gets the buttons in order.
        std::unique_ptr<Element> BuildMessageBox(State& s, const std::u16string& title, const std::u16string& text,
                                                 const std::vector<std::u16string>& buttons, MessageIcon icon,
                                                 std::vector<Element*>& found)
        {
            const std::string visual = std::format("XuiMessageBox{}", buttons.size() + 1);
            auto box = Element::CreateControl(*s.assets, "XuiMessageBox", visual, 360, 410);
            box->text = title;
            // XAM's icon field (0x81692380): 1 error, 2 warning, 3 alert, 4 information (no XMB_ name; XAM's own
            // boxes use it, e.g. Data Sharing), others none.
            static const char* const kIcons[] = { "", "xam://ico_32x_error.png", "xam://ico_32x_warning.png",
                                                  "xam://ico_32x_alert.png", "xam://ico_32x_information.png" };
            box->imagePath = unsigned(icon) < std::size(kIcons) ? kIcons[int(icon)] : "";
            box->imageBase = "xam.xam";
            if (Element* message = box->FindInVisual("MessageText"))
                message->text = text;
            for (size_t i = 0; i < buttons.size(); i++)
                if (Element* b = box->FindInVisual(std::format("Button{}", i)))
                {
                    b->text = buttons[i];
                    found.push_back(b);
                }
            if (Element* a = box->FindInVisual("btnA"))
                a->text = XamString(s, kStringSelect, u"Select");
            if (Element* b = box->FindInVisual("btnB"))
                b->text = XamString(s, kStringBack, u"Back");
            return box;
        }

        // ---- The Xbox Guide (hud.xex 17559: GuideMainScene and its blade scenes; TU46's XuiTabSceneElement) ----

        bool IsButton(const Element* e)
        {
            for (const ClassDef* c : ClassChain(e->ClassName()))
                if (std::string_view(c->name) == "XuiButton")
                    return true;
            return false;
        }

        // Where a navigation into `e` lands: `e` if it is a shown button (disabled ones take the focus too, with
        // their FocusDisable look: Account Management on a local profile), else the scene's DefaultFocus or its first
        // such button.
        Element* FocusTarget(Element* e)
        {
            if (!e || !e->Shown())
                return nullptr;
            if (IsButton(e))
                return e;
            if (const std::string id = e->GetString("DefaultFocus"); !id.empty())
                if (Element* d = FocusTarget(e->Find(id)))
                    return d;
            for (const auto& c : e->Children())
                if (Element* d = FocusTarget(c.get()))
                    return d;
            return nullptr;
        }

        void PlayControlState(Element* e, bool focused, bool initial)
        {
            if (focused)
                e->PlayVisualState(!e->enabled ? (initial ? "InitFocusDisable" : "FocusDisable") :
                                   initial ? "InitFocus" : "Focus");
            else
                e->PlayVisualState(e->enabled ? "KillFocus" : "NormalDisable");
        }

        void GuideFocus(State& s, Element* target, bool initial)
        {
            auto& g = s.guide;
            if (!target || target == g.focus)
                return;
            if (g.focus)
            {
                g.focus->focused = false;
                PlayControlState(g.focus, false, false);
            }
            g.focus = target;
            target->focused = true;
            PlayControlState(target, true, initial);
            s.guideTab = g.cur;
            s.guideFocus = target->Id();
            s.dirty = true;
        }

        // XUI navigation from the focused control: its Nav<direction> names the target in the blade; a control without
        // one passes the navigation to its parent scene's (the music player's NavUp); a target that cannot take the
        // focus (hidden: Kinect Tuner) passes it on in the same direction. False if nothing takes it.
        bool GuideNavigate(State& s, const char* direction)
        {
            auto& g = s.guide;
            Element* blade = g.tab[g.cur];
            Element* at = g.focus;
            for (int step = 0; at && step < 16; step++)
            {
                std::string next;
                for (Element* e = at; e && e != blade && next.empty(); e = e->Parent())
                    next = e->GetString(direction);
                Element* target = next.empty() ? nullptr : blade->Find(next);
                if (!target)
                    return false;
                if (Element* landing = FocusTarget(target))
                {
                    GuideFocus(s, landing, false);
                    return true;
                }
                at = target;
            }
            return false;
        }

        // GuideMainScene's blade animation (0x913E7AA8): "<tab>Open" fades the blades out (the Guide closing, or a
        // scene opening over it), "<tab>Close" brings them in; GuideMain and its tab scene play the same frames.
        void GuidePlay(State& s, const char* kind)
        {
            auto& g = s.guide;
            if (g.blades)
                return;
            const std::string name = std::format("{}{}", g.cur + 1, kind);
            g.main->PlayNamed(name, name + "End", false);
            g.tabs->PlayNamed(name, name + "End", false);
            s.dirty = true;
        }

        // XuiTabSceneElement::GotoNextTab / GotoPrevTab (TU46 0x82A9C0C8 / 0x82A9C1E0): to the neighbouring tab when
        // there is one (the Guide's tab scene does not wrap), playing "<from>To<to>" up to "<from>To<to>End"; the new
        // tab is shown and takes the user's focus (its default).
        void GuideSwitchTab(State& s, int delta)
        {
            auto& g = s.guide;
            const int next = g.cur + delta;
            if (next < 0 || next >= g.count)
                return;
            const std::string name = std::format("{}To{}", g.cur + 1, next + 1);
            g.tabs->PlayNamed(name, name + "End", false);
            SetShow(g.tab[next], true);
            g.cur = next;
            if (g.focus)
            {
                g.focus->focused = false;
                PlayControlState(g.focus, false, false);
                g.focus = nullptr;
            }
            GuideFocus(s, FocusTarget(g.tab[next]), false);
        }

        // Every button of a blade in its standard state.
        void GuideResetButtons(Element* e)
        {
            if (!e)
                return;
            if (IsButton(e))
            {
                e->focused = false;
                e->PlayVisualState(e->enabled ? "Normal" : "NormalDisable");
                return;
            }
            for (const auto& c : e->Children())
                GuideResetButtons(c.get());
        }

        // The tray button's label (0x913E55C0: hud string table 0x913E0BF8 by XamLoaderGetDvdTrayState): closed 48
        // "Open Tray", closing 43, open 42, opening 49, reading 50.
        std::u16string TrayLabel(State& s, uint32_t trayState)
        {
            switch (trayState)
            {
            case 1: return HudStringOf(s, 43, u"Closing...");
            case 2: return HudStringOf(s, 42, u"Close Tray");
            case 3: return HudStringOf(s, 49, u"Opening...");
            case 4: return HudStringOf(s, 50, u"Reading...");
            default: return HudStringOf(s, 48, u"Open Tray");
            }
        }

        // The Blades dashboards' Guide (hud.xex 1888 ... 6770): MainMenuSignedIn.xur (the gamer card, the community
        // bar, Connect to Xbox LIVE, Chat, Personal Settings, the music player, the game banner) or, signed out,
        // MainMenuSignedOut.xur (Sign In, Create Profile, Recover Profile, Personal Settings), on the HUD's blade with
        // the gamertag; the legend is its btnA / btnB / btnX / btnY's texts.
        Element* FillGamerCard(State& s, Element* host, const GamerCardInfo& card, bool extended);

        void GuideBuildBlades(State& s, const GuideInfo& info)
        {
            const bool signedOut = info.state == 6;
            Element* pane = OpenPane(s, "hud.hud", signedOut ? "MainMenuSignedOut.xur" : "MainMenuSignedIn.xur");
            if (!pane)
                return;
            s.pane = Pane::kGuide;
            auto& g = s.guide;
            g.main = pane;
            g.tabs = pane;
            g.tab[0] = pane;
            g.count = 1;
            g.cur = 0;
            g.home = g.games = g.media = g.settings = 0;
            g.blades = true;
            LoadInto(s, pane->Find("scnMusic"), "hud.hud", "MiniMediaPlayer.xur");
            if (!signedOut)
            {
                LoadInto(s, pane->Find("scnCommunity"), "hud.hud", "CommunityBar.xur");
                GamerCardInfo card;
                card.gamertag = info.gamertag;
                card.gamerscore = info.gamerscore;
                card.picture = info.picture;
                if (Element* host = pane->Find("ctlGamerCard"))
                    if (Element* gc = FillGamerCard(s, host, card, false))
                        if (const std::string format = host->GetString("Format"); !format.empty())
                            gc->PlayNamed(format, format, true); // "Mini": the card's top row only
                // Not on our Xbox LIVE: Connect to Xbox LIVE and "Not connected to Xbox LIVE"; on it, Chat.
                const bool live = info.state == 7;
                SetShow(pane->Find("btnSignInLive"), !live);
                SetShow(pane->Find("txtSignInLive"), !live);
                SetShow(pane->Find("btnQuickChat"), live);
            }
            SetText(s.hudScene->Find("GamerTag"), signedOut ? std::u16string() : info.gamertag);
            SetShow(s.hudScene->Find("GamerTag"), !signedOut);
            std::u16string legend[4];
            const char* const kLegend[4] = { "btnA", "btnB", "btnX", "btnY" };
            for (int i = 0; i < 4; i++)
                if (const Element* b = pane->Find(kLegend[i]))
                    legend[i] = b->text;
            SetLegend(s, legend);
            GuideResetButtons(pane);
            if (!s.guideKeep)
                s.guideFocus.clear();
            s.guideKeep = false;
            Element* focus = s.guideFocus.empty() ? nullptr : FocusTarget(pane->Find(s.guideFocus));
            GuideFocus(s, focus ? focus : FocusTarget(pane), true);
            Log("System UI: the Blades Guide opens ({})", signedOut ? "signed out" : "signed in");
        }

        void GuideBuild(State& s, const GuideInfo& info)
        {
            if (const Package* hud = s.assets->FindPackage("hud.hud");
                hud && !hud->Find("GuideMain.xur") && hud->Find("MainMenuSignedIn.xur"))
                return GuideBuildBlades(s, info);
            Element* pane = OpenPane(s, "hud.hud", "GuideMain.xur");
            if (!pane)
                return;
            s.pane = Pane::kGuide;
            auto& g = s.guide;
            g.main = pane;
            g.tabs = pane->Find("Tabscene");
            g.count = g.tabs ? std::clamp(int(g.tabs->GetInt("TabCount", 4)), 0, int(std::size(g.tab))) : 0;
            for (int i = 0; i < g.count; i++)
            {
                g.tab[i] = g.tabs->Find(std::format("Tab{}", i + 1));
                const std::string cls = g.tab[i] ? g.tab[i]->GetString("ClassOverride") : std::string();
                int* role = cls == "GamesTabScene" ? &g.games : cls == "HomeTabHostScene" || cls == "HomeTabScene" ? &g.home :
                            cls == "MediaTabScene" ? &g.media : cls == "SettingsTabScene" ? &g.settings :
                            cls == "MarketplaceTabScene" ? &g.marketplace : nullptr;
                if (role)
                    *role = i;
            }
            if (g.games < 0 || g.home < 0 || g.media < 0 || g.settings < 0)
            {
                Log("System UI: GuideMain.xur's Tabscene lacks the Games, Home, Media or Settings blade");
                return;
            }
            // The blade scenes for the user's state (0x913E5B40, 0x913E8C70, 0x913F8018); the Media blade's buttons
            // are GuideMain's own, with the music player (MiniMediaPlayer.xur) in its scnMusic. NXE's Games blade has
            // a LIVE variant (GamesTabSignedInLive.xur) where it has one; 9199's Marketplace blade is its own scene
            // (MarketplaceTabNoAvatar.xur without the avatar system: XamAvatarInitialize fails here), 7258 ... 8955's
            // is GuideMain's own buttons.
            const bool signedOut = info.state == 6;
            const bool gamesLive = info.state == 7 && s.assets->FindPackage("hud.hud")->Find("GamesTabSignedInLive.xur");
            LoadInto(s, g.tab[g.games], "hud.hud", signedOut ? "GamesTabSignedOut.xur" :
                                                   gamesLive ? "GamesTabSignedInLive.xur" : "GamesTabSignedIn.xur");
            LoadInto(s, g.tab[g.home], "hud.hud", signedOut ? "HomeTabSignedOut.xur" :
                                                  info.state == 7 ? "HomeTabSignedInLive.xur" : "HomeTabSignedIn.xur");
            LoadInto(s, g.tab[g.media]->Find("scnMusic"), "hud.hud", "MiniMediaPlayer.xur");
            LoadInto(s, g.tab[g.settings], "hud.hud", signedOut ? "SettingsTabSignedOut.xur" : "SettingsTabSignedIn.xur");
            if (g.marketplace >= 0 && s.assets->FindPackage("hud.hud")->Find("MarketplaceTabNoAvatar.xur"))
                LoadInto(s, g.tab[g.marketplace], "hud.hud", "MarketplaceTabNoAvatar.xur");
            auto find = [&](int tab, const char* id) { return g.tab[tab]->Find(id); };
            const int kGames = g.games, kHome = g.home, kMedia = g.media, kSettings = g.settings;

            // The profile's blade is named after the gamertag (0x913E7D00; hud string 40 "Home" when signed out).
            const std::u16string name = signedOut || info.gamertag.empty() ? HudStringOf(s, 40, u"Home") : info.gamertag;
            SetText(g.tabs->Find("txt_home"), name);
            SetText(g.tabs->Find("txt_homeSel"), name);
            // Sign In (0x913E6A28): hud strings 38 / 39 "Sign In (%d Profiles)" / "(%d Profile)", the profile count.
            if (Element* b = find(kHome, "btnSignIn"))
            {
                std::u16string text = HudStringOf(s, info.profiles == 1 ? 39 : 38, u"Sign In (%d Profiles)");
                if (const size_t at = text.find(u"%d"); at != std::u16string::npos)
                    text.replace(at, 2, Utf16(std::to_string(info.profiles)));
                b->text = text;
            }
            // Connect to Xbox Live (0x913E6AA8): "Join Xbox Live" (hud 15) unless it is a LIVE profile offline.
            if (Element* b = find(kHome, "btnConnectToLive"); b && info.state != 8)
                b->text = HudStringOf(s, 15, u"Join Xbox Live");
            // The disc tray (0x913E55C0 / 0x913E54C0): the label by the tray state (TrayLabel; closed: "Open Tray", hud 48) and the
            // optical-media icon in image association 1 (capture 3:37:14).
            if (Element* b = find(kHome, "btnDiscInTray"))
            {
                b->text = TrayLabel(s, info.trayState);
                b->dataImage[1] = s.assets->LoadImage("sharedres://ico_32x_OpticalMedia.png", "xam.shrdres");
            }
            // On Xbox LIVE (HomeTabSignedInLive.xur): Friends / Party / Messages show their count and icon (btn_Count:
            // text and image association 1), Beacons & Activity and Chat their icon (capture "guide start page.png").
            if (info.state == 7)
            {
                const std::pair<const char*, const char*> icons[] = {
                    { "btnFriends", "ico_32x_Friend.png" }, { "btnParty", "ico_32x_StartParty.png" },
                    { "btnMessages", "ico_32x_Mail.png" }, { "btnBeacons", "ico_32x_Beacons.png" },
                    { "btnChatIM", "ico_32x_messenger.png" } };
                for (const auto& [id, icon] : icons)
                    if (Element* b = find(kHome, id))
                        b->dataImage[1] = s.assets->LoadImage(icon, "xam.shrdres");
                const std::pair<const char*, uint32_t> counts[] = {
                    { "btnFriends", info.friendsOnline }, { "btnParty", info.partyMembers },
                    { "btnMessages", info.unreadMessages } };
                for (const auto& [id, count] : counts)
                    if (Element* b = find(kHome, id))
                        b->dataText[1] = Utf16(std::to_string(count));
            }
            // Achievements (AchievementsNavButton, 0x913E5E88): the count "%d" in text association 1; the visual adds
            // the gamerscore glyph.
            if (Element* b = find(kGames, "btnAchievements"))
                b->dataText[1] = Utf16(std::to_string(info.gamerscore));
            // Account Management only for LIVE profiles (0x913E68B8).
            if (Element* b = find(kSettings, "btnAccountManagement"))
                b->enabled = info.state == 7 || info.state == 8;
            // Kinect Tuner only with a Kinect sensor (0x913F80F8): hidden, and Turn Off Console moves to its place.
            if (Element* kinect = find(kSettings, "btnKinectTuner"))
            {
                SetShow(kinect, false);
                if (Element* off = find(kSettings, "btnShutdown"))
                {
                    const Vec3 p = off->GetVector("Position", {});
                    off->SetPosition(p.x, kinect->GetVector("Position", {}).y);
                }
            }
            // The music player: "Select Music" (hud 22), its volume slider hidden until Volume is pressed.
            if (Element* b = find(kMedia, "btnPlaylist"))
                b->text = HudStringOf(s, 22, u"Select Music");
            SetShow(find(kMedia, "sliderVolume"), false);
            // Nothing is playing: the transport buttons are disabled (capture 3:37:22).
            for (const char* id : { "btnPlay", "btnPrev", "btnStop", "btnNext" })
                if (Element* b = find(kMedia, id))
                    b->enabled = false;
            // MusicPlaybackModeButton: the playback order's icon (in order, until the gamer picks shuffle).
            if (Element* b = find(kMedia, "btnPlaybackMode"))
            {
                b->imagePath = "sharedres://ico_32x_inorder.png";
                b->imageBase = "xam.shrdres";
            }
            SetShow(find(kMedia, "imgVolume"), false);
            for (int i = 0; i < g.count; i++)
                GuideResetButtons(g.tab[i]);

            // Legend: A Select, B Back, X Sign In / Sign Out (XamStrings 157 / 159, 0x913E8438), Y Xbox Home.
            const std::u16string legend[4] = { Utf16(pane->GetString("LegendA")), Utf16(pane->GetString("LegendB")),
                                               signedOut ? XamString(s, 157, u"Sign In") : XamString(s, 159, u"Sign Out"),
                                               Utf16(pane->GetString("LegendY")) };
            SetLegend(s, legend);
            // The ring of light (0x913E7E68): the Guide's controller quadrant lit.
            // (The visual's own light1..4, over the dim ROL_Off set.)
            if (Element* rol = pane->Find("ROL"); rol && rol->Visual())
                for (const auto& light : rol->Visual()->Children())
                    for (uint32_t i = 0; i < 4; i++)
                        if (light->Id() == std::format("light{}", i + 1))
                            light->SetFloat("Opacity", info.user == i || info.user >= 4 ? 1.0f : 0.0f);

            // The blade it was left on (XuiTabSceneElement::GotoTab, TU46 0x82A9C2E8: the tab's resting frame,
            // "2To1End" for the first, "<t>To<t+1>End" otherwise) and the button that had the focus (0x913E79F8).
            if (!s.guideKeep)
            {
                s.guideTab = -1;
                s.guideFocus.clear();
            }
            g.cur = std::clamp(s.guideTab >= 0 ? s.guideTab : int(g.tabs->GetInt("DefaultTab", g.home + 1)) - 1, 0, g.count - 1);
            Log("System UI: the Guide opens on blade {}{}", g.cur + 1, s.guideKeep ? " (where it was left)" : "");
            s.guideKeep = false;
            const std::string rest = g.cur == 0 ? "2To1End" : std::format("{}To{}End", g.cur, g.cur + 1);
            g.tabs->PlayNamed(rest, rest, false);
            SetShow(g.tab[g.cur], true);
            Element* focus = s.guideFocus.empty() ? nullptr : FocusTarget(g.tab[g.cur]->Find(s.guideFocus));
            GuideFocus(s, focus ? focus : FocusTarget(g.tab[g.cur]), true);
            // The blades come in (GuideMainScene's transition handler 0x913E83C0).
            GuidePlay(s, "Close");
        }

        // XAM's notification table (xam.xex 17559, 0x815F82B0): type -> icon and XamStrings format. The toasts this
        // runtime raises; XAM has ~60 more (friends, messages, downloads, party, ...).
        struct Notification { uint32_t type; const char* icon; size_t format; };
        constexpr Notification kNotifications[] = {
            { kNotifyAchievement, "xam://Achievement.png", 66 }, { kNotifyAchievements, "xam://Achievement.png", 66 },
            { kNotifySignedOut, "xam://signed_out.png", 119 }, { kNotifySignedIn, "xam://signed_in.png", 116 },
            { kNotifySignedInLive, "xam://signed_in.png", 117 }, { kNotifySignedInOffline, "xam://signed_in.png", 118 },
            { kNotifyFriendRequest, "xam://ico_32x_Friend.png", 62 }, { kNotifyMessage, "sharedres://ico_32x_Mail.png", 136 },
            { kNotifyGameInvite, "sharedres://ico_32x_Mail.png", 63 },
            { kNotifyGameInviteSent, "sharedres://ico_32x_Mail.png", 64 },
            { kNotifyLiveDisconnected, "xam://connection_lost.png", 112 },
            { kNotifyPartyInvite, "xam://party.png", 131 }, { kNotifyPartyInviteSent, "xam://party.png", 114 },
            { kNotifyPartyMemberJoined, "xam://party.png", 132 }, { kNotifyPartyMemberLeft, "xam://party.png", 134 },
            { kNotifyPartyJoined, "xam://party.png", 130 },
            { kNotifyMuted, "xam://chat.png", 103 }, { kNotifyUnmuted, "xam://chat.png", 104 },
            { kNotifyPreferredPlayer, "PreferredPlayer.png", 0 },
        };

        // _snwprintf with %s arguments only.
        std::u16string Format(const std::u16string& format, const std::vector<std::u16string>& args)
        {
            std::u16string out;
            size_t next = 0;
            for (size_t i = 0; i < format.size(); i++)
            {
                if (format[i] == u'%' && i + 1 < format.size() && format[i + 1] == u's')
                {
                    if (next < args.size())
                        out += args[next++];
                    i++;
                }
                else if (format[i] == u'%' && i + 1 < format.size() && format[i + 1] == u'%')
                    out += format[++i];
                else
                    out += format[i];
            }
            return out;
        }

        // The toast's text as XAM builds it (notify scene setup, 0x816AA470): the type's XamStrings format with the
        // parameter; an achievement's "a|b" fills XamStrings 0 ("Achievement unlocked\n%sG - %s") or, for several,
        // 1 ("Achievements unlocked\n%s for %sG"); the strings' "\n" escapes become line breaks (0x816A9680).
        std::u16string ToastText(State& s, const State::Toast& toast, size_t format)
        {
            std::u16string text;
            if (toast.type == kNotifyAchievement || toast.type == kNotifyAchievements)
            {
                const size_t bar = toast.param.find(u'|');
                const std::u16string a = toast.param.substr(0, bar);
                const std::u16string b = bar == std::u16string::npos ? u"" : toast.param.substr(bar + 1);
                text = Format(XamString(s, toast.type == kNotifyAchievement ? 0 : 1, u"%s %s"), { a, b });
            }
            else if (toast.type == kNotifyPreferredPlayer)
                text = toast.param + u"\nis now a preferred player.";
            else
                text = Format(XamString(s, format, u"%s"), { toast.param });
            std::u16string out;
            for (size_t i = 0; i < text.size(); i++)
            {
                if (text[i] == u'\\' && i + 1 < text.size() && (text[i + 1] == u'n' || text[i + 1] == u'r'))
                    out += text[++i] == u'n' ? u'\n' : u'\r';
                else
                    out += text[i];
            }
            return out;
        }

        void StartToast(State& s)
        {
            if (s.toastScene || s.toasts.empty())
                return;
            const State::Toast toast = s.toasts.front();
            s.toasts.pop_front();
            const Notification* n = nullptr;
            for (const Notification& entry : kNotifications)
                if (entry.type == toast.type)
                    n = &entry;
            const Scene* scene = s.assets->LoadScene("xam.xam", "notify.xur");
            if (!n || !scene)
                return;
            s.toastScene = Element::Create(*s.assets, *scene, scene->Root(), "xam.xam", nullptr);
            s.toastControl = s.toastScene->Find("PopupControl");
            Element* visual = s.toastControl ? s.toastControl->Visual() : nullptr;
            Element* presenter = nullptr;
            if (visual)
                for (const auto& c : visual->Children())
                    if (c->ClassName() == "XuiTextPresenter")
                    {
                        presenter = c.get();
                        break;
                    }
            if (!presenter)
            {
                s.toastScene.reset();
                s.toastControl = nullptr;
                return;
            }
            s.toastControl->text = ToastText(s, toast, n->format);
            s.toastControl->imagePath = n->icon;
            s.toastControl->imageBase = std::string_view(n->icon).starts_with("sharedres://") ? "xam.shrdres" :
                                        toast.type == kNotifyPreferredPlayer ? "guide.friends.friends" : "xam.xam";
            // The user's quadrant of the ring of light (0x816A9968; the others transparent, all four for no user).
            const uint32_t mask = toast.user < 4 ? 1u << toast.user : 0xFu;
            for (int i = 0; i < 4; i++)
                if (Element* indicator = visual->Find(std::format("Indicator{}", i + 1)))
                    indicator->SetFloat("Opacity", (mask >> i) & 1 ? 1.0f : 0.0f);
            // Fit the control to its text (0x816AA280): width = control - presenter + text extent, height = the
            // presenter's. Then place it (0x816AA328) by XNotifyPositionUI's bits in the 852x480 HUD target: the
            // control at (0, 0) of its canvas, the canvas offset by whole units and kept inside the safe area (85% of
            // a 480-line target, 0x816C9088 / 0x816CB1E8).
            const float width = s.toastControl->Width() - presenter->Width() +
                float(presenter->MeasureText(*s.assets, s.toastControl->text));
            const float height = presenter->Height();
            s.toastControl->SetSize(width, height);
            s.toastControl->SetPosition(0, 0);
            const uint32_t pos = s.notifyPosition;
            const float x = pos & 4 ? 0.0f : pos & 8 ? kDesignWidth - width : (kDesignWidth - width) * 0.5f;
            const float y = pos & 1 ? 0.0f : pos & 2 ? kDesignHeight - height : (kDesignHeight - height) * 0.5f;
            const int percent = int(kDesignHeight) == 480 ? 85 : 90;
            const int safeW = int(kDesignWidth) * percent / 100, safeH = int(kDesignHeight) * percent / 100;
            const int left = (int(kDesignWidth) - safeW) >> 1, top = (int(kDesignHeight) - safeH) >> 1;
            s.toastX = int(x);
            s.toastY = int(y);
            if (s.toastX < left)
                s.toastX = left;
            else if (float(s.toastX) + width > float(left + safeW))
                s.toastX = int(float(left + safeW) - width);
            if (s.toastY < top)
                s.toastY = top;
            else if (float(s.toastY) + height > float(top + safeH))
                s.toastY = int(float(top + safeH) - height);
            std::string logged = Utf8(s.toastControl->text);
            std::replace(logged.begin(), logged.end(), '\n', '/');
            Log("System UI: toast {:02X} for user {}: \"{}\" at ({}, {}) in the HUD", toast.type, toast.user, logged,
                s.toastX, s.toastY);
            // The pop-up sound (the skin visual's PopUpSound: TransTo's frame 0 sets its File to NotifyPopup.xma, frame 1
            // clears it) plays when its File changes. XAM keeps its toast scene, so the file is clear when the next toast
            // opens; the scene is new here, made with the file already set: cleared first, or no toast had a sound.
            if (Element* sound = visual->Find("PopUpSound"))
                sound->SetString("File", "");
            s.toastControl->PlayVisualState("TransTo");
            visual->PlayNamed("TransTo", "", true);
            s.toastTime = 0;
            s.toastClosing = false;
            s.dirty = true;
        }

        void AdvanceToast(State& s, float dt)
        {
            if (!s.toastScene)
            {
                StartToast(s);
                return;
            }
            s.toastTime += dt;
            s.toastScene->Advance(dt);
            Element* v = s.toastControl->Visual();
            // The scene's transition has no frames, so XAM's 5000 ms timer starts as the toast opens (0x816AB760);
            // when it fires the control plays TransFrom..EndTransFrom (0x816A9F80) and the scene leaves after it.
            if (!s.toastClosing && s.toastTime >= 5.0f && v)
            {
                v->PlayNamed("TransFrom", "EndTransFrom", true);
                s.toastClosing = true;
            }
            if (s.toastClosing && v && !v->Playing())
            {
                s.toastScene.reset();
                s.toastControl = nullptr;
                StartToast(s);
            }
            s.dirty = true;
        }
    }

    bool SystemUiAvailable()
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        return Ready(s);
    }

    void PreloadSystemUi()
    {
        std::thread([] {
            State& s = S();
            std::lock_guard lock(s.mutex);
            const auto start = std::chrono::steady_clock::now();
            if (!Ready(s) || !EnsureHud(s))
                return;
            static const std::pair<const char*, const char*> kScenes[] = {
                { "xam.xam", "notify.xur" }, { "hud.hud", "GuideMain.xur" }, { "hud.hud", "GamesTabSignedIn.xur" },
                { "hud.hud", "GamesTabSignedOut.xur" }, { "hud.hud", "HomeTabSignedIn.xur" },
                { "hud.hud", "HomeTabSignedInLive.xur" }, { "hud.hud", "HomeTabSignedOut.xur" },
                { "hud.hud", "SettingsTabSignedIn.xur" }, { "hud.hud", "SettingsTabSignedOut.xur" },
                { "hud.hud", "MiniMediaPlayer.xur" }, { "signin.signin", "signin1.xur" },
                { "signin.signin", "UserListSinglePane.xur" }, { "vk.vk", "KeyboardMain.xur" },
                { "vk.vk", "KeyboardBase.xur" }, { "gamerprofile.gp", "802_Achievements.xur" } };
            for (const auto& [package, entry] : kScenes)
                s.assets->LoadScene(package, entry);
            size_t images = 0;
            for (const char* package : { "xam.skin", "xam.xam", "xam.shrdres", "hud.hud", "signin.signin", "vk.vk",
                                         "gamerprofile.gp" })
                if (const Package* p = s.assets->FindPackage(package))
                    for (const Package::Entry& e : p->Entries())
                        if (e.name.size() > 4 && e.name.compare(e.name.size() - 4, 4, ".png") == 0 &&
                            s.assets->LoadImage(e.name, package))
                            images++;
            Log("System UI: preloaded in {} ms ({} images)", std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - start).count(), images);
        }).detach();
    }

    void ShowPasscodeBox(const std::u16string& title, const std::u16string& text, MessageIcon icon)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!Ready(s) || !EnsureHud(s))
            return;
        ShowWhenClosed(s, [=](State& s) {
            auto box = Element::CreateControl(*s.assets, "XuiMessageBox", "PasscodeMessageBox", 360, 410);
            box->text = title;
            static const char* const kIcons[] = { "", "xam://ico_32x_error.png", "xam://ico_32x_warning.png",
                                                  "xam://ico_32x_alert.png" };
            box->imagePath = kIcons[int(icon) & 3];
            box->imageBase = "xam.xam";
            if (Element* message = box->FindInVisual("MessageText"))
                message->text = text;
            if (Element* b = box->FindInVisual("btnB"))
                b->text = XamString(s, kStringBack, u"Back");
            Open(s, "Error", "ErrorHostElement", std::move(box));
            s.buttons.clear();
        });
    }

    namespace
    {
        void DrillFocus(State& s, uint32_t index, bool initial);

        // An XuiSlider's value: its visual's SliderBody shows frame value * 100 / RangeMax of its Normal range
        // (0-100), or of its Focus range (101-201) while focused (the skin's XuiSlider visual).
        void SetSlider(Element* e, int value)
        {
            const int hi = std::max<int>(1, int(e->GetInt("RangeMax", 100))), lo = int(e->GetInt("RangeMin", 0));
            value = std::clamp(value, lo, hi);
            Value v{ Type::kInteger };
            v.i = value;
            e->Set("Value", v);
            Element* body = e->FindInVisual("SliderBody");
            if (body)
                body->ShowFrame((e->focused ? 101 : 0) + (value - lo) * 100 / std::max(1, hi - lo));
        }
    }

    void ShowControlScene(const std::string& package, const std::string& scene, const std::vector<SceneControl>& controls,
                          const std::vector<std::string>& hidden, uint32_t focus)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!Ready(s) || !EnsureHud(s))
            return;
        ShowWhenClosed(s, [=](State& s) {
            Element* pane = OpenPane(s, package, scene);
            if (!pane)
                return;
            s.pane = State::Pane::kDrill;
            s.buttons.clear();
            for (const std::string& id : hidden)
                SetShow(pane->Find(id), false);
            for (const SceneControl& c : controls)
                if (Element* e = pane->Find(c.id))
                {
                    e->checked = c.checked;
                    e->enabled = c.enabled;
                    s.buttons.push_back(e);
                }
                else
                    Log("System UI: {}: no control {}", scene, c.id);
            // Sliders follow the focus (their bar's Focus range).
            s.buttonFocus = [](State& s, uint32_t) {
                for (Element* b : s.buttons)
                    if (b->ClassName() == "XuiSlider")
                        SetSlider(b, int(b->GetInt("Value", 0)));
            };
            DrillFocus(s, focus, true);
            for (size_t i = 0; i < s.buttons.size() && i < controls.size(); i++)
                if (s.buttons[i]->ClassName() == "XuiSlider")
                    SetSlider(s.buttons[i], controls[i].value);
        });
    }

    void UpdateSceneControl(uint32_t index, const SceneControl& control)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (s.pane != State::Pane::kDrill || index >= s.buttons.size())
            return;
        Element* e = s.buttons[index];
        if (e->ClassName() == "XuiSlider")
            SetSlider(e, control.value);
        else if (e->checked != control.checked)
        {
            e->checked = control.checked;
            e->PlayVisualState(e->focused ? "Focus" : "Normal");
        }
        s.dirty = true;
    }

    void SetPasscodeEntered(uint32_t count, const std::u16string& text)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        Element* host = s.hudScene ? s.hudScene->Find("ErrorHostElement") : nullptr;
        if (!host || host->Children().empty())
            return;
        Element* box = host->Children().back().get();
        for (uint32_t i = 0; i < 4; i++)
            if (Element* dot = box->FindInVisual(std::format("Passcode{}", i)))
                dot->PlayVisualState(i < count ? "NormalCheck" : "Normal");
        if (!text.empty())
            if (Element* message = box->FindInVisual("MessageText"))
                message->text = text;
        s.dirty = true;
    }

    void ShowMessageBox(const std::u16string& title, const std::u16string& text,
                        const std::vector<std::u16string>& buttons, uint32_t focus, MessageIcon icon)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!Ready(s) || !EnsureHud(s) || buttons.empty() || buttons.size() > 3)
            return;
        ShowWhenClosed(s, [=](State& s) {
            // Hosted in hudbkgnd's error pane.
            std::vector<Element*> found;
            Open(s, "Error", "ErrorHostElement", BuildMessageBox(s, title, text, buttons, icon, found));
            s.buttons = found;
            Focus(s, focus, true);
        });
    }

    void SetGamerPicture(const std::vector<uint8_t>& png)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!Ready(s))
            return;
        s.gamerPicture.reset();
        if (!png.empty())
        {
            auto image = std::make_unique<Image>();
            std::string error;
            if (DecodePng(png.data(), png.size(), *image, error))
                s.gamerPicture = std::move(image);
            else
                Log("System UI: gamer picture: {}", error);
        }
        if (Element* picture = s.hudScene ? s.hudScene->Find("GamerPic") : nullptr)
            picture->image = s.gamerPicture.get();
        s.dirty = true;
    }

    void ShowGuide(const GuideInfo& info)
    {
        State& s = S();
        {
            std::lock_guard lock(s.mutex);
            if (!Ready(s) || !EnsureHud(s))
                return;
        }
        SetGamerPicture(info.picture);
        std::lock_guard lock(s.mutex);
        ShowWhenClosed(s, [info](State& s) { GuideBuild(s, info); });
    }

    void SetGuideTrayState(uint32_t trayState)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        auto& g = s.guide;
        if (s.pane != Pane::kGuide || !g.tabs || g.home < 0)
            return;
        if (Element* b = g.tab[g.home]->Find("btnDiscInTray"))
        {
            b->text = TrayLabel(s, trayState);
            s.dirty = true;
        }
    }

    void GuideKeepPosition()
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        s.guideKeep = true;
    }

    void GuideForgetPosition()
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        s.guideKeep = false;
        s.guideTab = 1;
        s.guideFocus.clear();
    }

    GuideAction GuidePress(uint16_t pressed)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        auto& g = s.guide;
        if (s.pane != Pane::kGuide || s.phase != Phase::kOpen || !g.tabs || (g.box && !g.boxClosing) || s.afterPress)
            return {};
        constexpr uint16_t kUp = 0x0001, kDown = 0x0002, kLeft = 0x0004, kRight = 0x0008, kBack = 0x0020,
                           kLeftTrigger = 0x0400, kRightTrigger = 0x0800, kA = 0x1000, kB = 0x2000, kX = 0x4000,
                           kY = 0x8000;
        if (pressed & (kB | kBack))
            return { GuideAction::kClose };
        if (pressed & kUp)
            GuideNavigate(s, "NavUp");
        if (pressed & kDown)
            GuideNavigate(s, "NavDown");
        // The focused control takes left / right first (the music player's buttons); otherwise the tab scene does
        // (XuiTabSceneElement::ObjectProc: left / right and LT / RT).
        if ((pressed & kLeft) && !GuideNavigate(s, "NavLeft"))
            GuideSwitchTab(s, -1);
        else if (pressed & kLeftTrigger)
            GuideSwitchTab(s, -1);
        if ((pressed & kRight) && !GuideNavigate(s, "NavRight"))
            GuideSwitchTab(s, 1);
        else if (pressed & kRightTrigger)
            GuideSwitchTab(s, 1);
        if ((pressed & kA) && g.focus && g.focus->enabled)
        {
            PlayPress(s, g.focus);
            return { GuideAction::kButton, g.focus->Id(), g.focus->text };
        }
        if (pressed & kX)
            return { GuideAction::kLegendX };
        if (pressed & kY)
        {
            const Element* y = g.main->Find("btnY");
            const std::string text = y ? y->GetString("Text") : std::string();
            return { GuideAction::kLegendY, "btnY", text.empty() ? u"Xbox Home" : Utf16(text) };
        }
        return {};
    }

    void ShowGuideMessageBox(const std::u16string& title, const std::u16string& text,
                             const std::vector<std::u16string>& buttons, uint32_t focus, MessageIcon icon)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        auto& g = s.guide;
        if (!s.assets || s.pane != Pane::kGuide || s.phase != Phase::kOpen || !g.tabs || buttons.empty() ||
            buttons.size() > 3)
            return;
        if (g.box && g.boxHost)
            g.boxHost->Release(g.box);
        g.box = nullptr;
        g.boxHost = s.hudScene->Find("ErrorHostElement");
        if (!g.boxHost)
            return;
        std::vector<Element*> found;
        auto box = BuildMessageBox(s, title, text, buttons, icon, found);
        const float w = box->Width(), h = box->Height();
        box->SetPosition(0, 0);
        box->SetSize(w, h);
        g.box = g.boxHost->Adopt(std::move(box));
        g.boxClosing = false;
        // hud.xex before it opens a scene over the Guide (0x913E84A0): the blades fade; XAM's HUD goes Half -> Error.
        GuidePlay(s, "Open");
        s.hudScene->PlayNamed("HalfToError", "EndHalfToError", true);
        s.hudState = "Error";
        s.buttons = found;
        Focus(s, focus, true);
    }

    void GuideMessageBoxClosed()
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        auto& g = s.guide;
        if (!s.assets || s.pane != Pane::kGuide || !g.box || g.boxClosing)
            return;
        // Back to the Guide (0x913E85E0 -> 0x913E7AA8 mode 3): HUD Error -> Half, the blades come back; the box goes
        // when the HUD's animation has finished.
        s.hudScene->PlayNamed("ErrorToHalf", "EndErrorToHalf", true);
        s.hudState = "Half";
        GuidePlay(s, "Close");
        s.buttons.clear();
        g.boxClosing = true;
    }

    std::u16string CommonString(size_t index, const char16_t* fallback)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        return Ready(s) ? XamString(s, index, fallback) : std::u16string(fallback);
    }

    std::u16string HudString(size_t index, const char16_t* fallback)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        return Ready(s) ? HudStringOf(s, index, fallback) : std::u16string(fallback);
    }

    std::u16string SigninString(size_t index, const char16_t* fallback)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        const auto* table = Ready(s) ? s.assets->Strings(Base17559(s, "signin.signin"), "strings.xus") : nullptr;
        return table && index < table->size() ? Utf16((*table)[index]) : std::u16string(fallback);
    }

    void ShowSignIn(const std::vector<SignInEntry>& entries, uint32_t focus)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!Ready(s) || !EnsureHud(s))
        {
            Log("System UI: no sign-in pane (the HUD is not ready)");
            return;
        }
        ShowWhenClosed(s, [=](State& s) {
            Element* pane = OpenPane(s, "signin.signin", "signin1.xur");
            if (!pane)
            {
                Log("System UI: signin1.xur cannot be opened");
                return;
            }
            s.pane = Pane::kSignIn;
            // The one-pane user list (17559); the Blades updates' signin.xex (1888 ... 6770) has UserList.xur.
            const Package* signin = s.assets->FindPackage("signin.signin");
            const bool singlePane = signin && signin->Find("UserListSinglePane.xur");
            Element* users = LoadInto(s, pane->Find("Scene_Pane0"), "signin.signin",
                                      singlePane ? "UserListSinglePane.xur" : "UserList.xur");
            Element* list = users ? users->Find("listUser") : nullptr;
            if (!list)
            {
                Log("System UI: sign-in pane without its user list ({} {}, Scene_Pane0 {})",
                    singlePane ? "UserListSinglePane.xur" : "UserList.xur", users ? "has no listUser" : "not loaded",
                    pane->Find("Scene_Pane0") ? "found" : "missing");
                return;
            }
            // signin.xex's strings: #9 "Create Profile", #64 "Download Profile", with the module's icons.
            const auto* strings = s.assets->Strings(Base17559(s, "signin.signin"), "strings.xus");
            auto text = [&](size_t i, const char16_t* fallback) {
                return strings && i < strings->size() ? Utf16((*strings)[i]) : std::u16string(fallback);
            };
            const std::u16string create = text(9, u"Create Profile"), download = text(64, u"Download Profile");
            const std::u16string hdd = text(17, u"HDD"); // the storage line of a profile on the hard drive
            const std::u16string guest = text(16, u"Guest");
            const Image* createIcon = s.assets->LoadImage("CreateProfileIcon.png", "signin.signin");
            const Image* downloadIcon = s.assets->LoadImage("DownloadProfileIcon.png", "signin.signin");
            const int profiles = int(entries.size());
            // Each profile row shows its gamer picture where the last two rows show their icons.
            s.images.clear();
            std::vector<const Image*> pictures(entries.size());
            for (size_t i = 0; i < entries.size(); i++)
            {
                if (entries[i].picture.empty())
                    continue;
                auto image = std::make_unique<Image>();
                std::string error;
                if (!DecodePng(entries[i].picture.data(), entries[i].picture.size(), *image, error))
                {
                    Log("System UI: gamer picture of row {}: {}", i, error);
                    continue;
                }
                pictures[i] = image.get();
                s.images.push_back(std::move(image));
            }
            ListBuild(s, list, profiles + 2, int(focus), [=](Element* item, int index) {
                item->dataText[1].clear();
                item->dataText[2].clear();
                item->dataImage[3] = nullptr;
                const bool enabled = index >= profiles || entries[size_t(index)].enabled;
                if (item->enabled != enabled && !item->focused)
                    item->PlayVisualState(enabled ? "Normal" : "NormalDisable");
                item->enabled = enabled;
                if (index < profiles)
                {
                    const SignInEntry& entry = entries[size_t(index)];
                    item->text = entry.guest ? guest : entry.gamertag;
                    item->dataText[1] = entry.status;
                    item->dataText[2] = entry.guest ? std::u16string() : entry.storage.empty() ? hdd : entry.storage;
                    item->dataImage[3] = pictures[size_t(index)];
                }
                else
                {
                    item->text = index == profiles ? create : download;
                    item->dataImage[3] = index == profiles ? createIcon : downloadIcon;
                }
            });
        });
    }
    void ShowSignInStatus(const SignInEntry& entry)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!Ready(s) || !EnsureHud(s))
            return;
        ShowWhenClosed(s, [=](State& s) {
            Element* pane = OpenPane(s, "signin.signin", "signin1.xur");
            if (!pane)
                return;
            s.pane = Pane::kWait;
            s.buttons.clear();
            Element* status = LoadInto(s, pane->Find("Scene_Pane0"), "signin.signin", "status.xur");
            if (!status)
                return;
            // StatusScene while a profile signs in: the profile's row, "Signing in" over the skin's txt_Starting wait
            // cursor (its XuiControl1, a Loading_Small: it fades in and turns from its Normal state on, as in
            // createprofile's wait scene); the Start button and "Waiting for others" belong to the multi-pane sign-in.
            SetShow(status->Find("btnStart"), false);
            SetShow(status->Find("txtWaiting"), false);
            SetShow(status->Find("txtDataCenterMode"), false);
            if (Element* starting = status->Find("txtStarting"); starting && starting->Visual())
                if (Element* cursor = starting->Visual()->Find("XuiControl1"))
                    cursor->PlayVisualState("Normal");
            s.images.clear();
            if (Element* row = status->Find("SelectedProfile"))
            {
                row->text = entry.gamertag;
                row->dataText[1] = entry.status;
                // The storage line as the list's rows show it ("HDD", signin.xex string 17).
                const auto* strings = s.assets->Strings("signin.signin", "strings.xus");
                row->dataText[2] = !entry.storage.empty()            ? entry.storage
                                   : strings && strings->size() > 17 ? Utf16((*strings)[17])
                                                                      : std::u16string(u"HDD");
                if (!entry.picture.empty())
                {
                    auto image = std::make_unique<Image>();
                    std::string error;
                    if (DecodePng(entry.picture.data(), entry.picture.size(), *image, error))
                    {
                        row->dataImage[3] = image.get();
                        s.images.push_back(std::move(image));
                    }
                }
            }
            const std::u16string legend[4] = {};
            SetLegend(s, legend);
        });
    }

    namespace
    {
        // deviceselector.xex's string table (Strings.xus), or `fallback` (no assets).
        std::u16string DeviceSelectorStringOf(State& s, size_t index, const char16_t* fallback)
        {
            const auto* table = s.assets ? s.assets->Strings(Base17559(s, "deviceselector.media"), "Strings.xus")
                                         : nullptr;
            return table && index < table->size() ? Utf16((*table)[index]) : std::u16string(fallback);
        }

        std::u16string DeviceSelectorFreeOf(State& s, uint64_t bytes)
        {
            // Strings.xus: 25 bytes, 27 KB, 28 MB, 26 GB; 31 "%.01f %s" (30 "%d %s" for bytes); 5 "%s free".
            const char16_t* units[] = { u"bytes", u"KB", u"MB", u"GB" };
            const size_t ids[] = { 25, 27, 28, 26 };
            double value = double(bytes);
            size_t unit = 0;
            while (unit < 3 && value >= 1024.0)
            {
                value /= 1024.0;
                unit++;
            }
            const std::u16string name = DeviceSelectorStringOf(s, ids[unit], units[unit]);
            std::string number = unit == 0 ? std::format("{}", bytes) : std::format("{:.1f}", value);
            std::u16string size(number.begin(), number.end());
            size += u" " + name;
            std::u16string free = DeviceSelectorStringOf(s, 5, u"%s free");
            if (const size_t at = free.find(u"%s"); at != std::u16string::npos)
                free.replace(at, 2, size);
            return free;
        }
    }

    std::u16string DeviceSelectorString(size_t index, const char16_t* fallback)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        return Ready(s) ? DeviceSelectorStringOf(s, index, fallback) : std::u16string(fallback);
    }

    std::u16string DeviceSelectorFree(uint64_t bytes)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        Ready(s); // loads the assets; without them the texts are the fallbacks
        return DeviceSelectorFreeOf(s, bytes);
    }

    void ShowDeviceSelector(const std::vector<DeviceEntry>& devices, uint32_t focus)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!Ready(s) || !EnsureHud(s))
            return;
        ShowWhenClosed(s, [=](State& s) {
            Element* pane = OpenPane(s, "deviceselector.media", "DeviceSelector.xur");
            if (!pane)
                return;
            s.pane = Pane::kDeviceSelector;
            s.dimLayers = kDimDeviceSelector;
            Element* list = pane->Find("DeviceListId");
            if (!list)
                return;
            // deviceselector.xex's icons, one per kind of device (XDEVICE_DATA type: 1 hard drive, 2 memory unit,
            // 8 transfer cable, others USB).
            const Image* hdd = s.assets->LoadImage("ico_64x_HD_black.png", "deviceselector.media");
            const Image* mu = s.assets->LoadImage("ico_64x_BuiltInMU.png", "deviceselector.media");
            if (!mu) // 1888's package: its memory unit's icon by its older name
                mu = s.assets->LoadImage("ico_64x_MU_black.png", "deviceselector.media");
            const Image* cable = s.assets->LoadImage("ico_64x_TransferCable.png", "deviceselector.media");
            const Image* usb = s.assets->LoadImage("ico_96x_USB.png", "deviceselector.media");
            // Rows are filled again whenever the list scrolls or moves focus: their status lines are made here.
            std::vector<std::u16string> status;
            for (const DeviceEntry& d : devices)
                status.push_back(d.status.empty() ? DeviceSelectorFreeOf(s, d.freeBytes) : d.status);
            ListBuild(s, list, int(devices.size()), int(focus), [=](Element* item, int index) {
                const DeviceEntry& d = devices[size_t(index)];
                item->text.clear();
                item->dataText[1] = d.name;
                item->dataText[2] = status[size_t(index)];
                item->image = d.kind == 1 ? hdd : d.kind == 2 ? mu : d.kind == 8 ? cable : usb;
                item->enabled = d.enabled;
                // btn_3Line_Icon_prgbr's XuiProgressBar1: prgbr_Memory's ProgressBody timeline shows Bar_mid (the used
                // share, dark over the light background) at frames 1-100 = percent.
                if (Element* bar = item->FindInVisual("XuiProgressBar1"))
                {
                    const int used = d.totalBytes ?
                        int((d.totalBytes - std::min(d.freeBytes, d.totalBytes)) * 100 / d.totalBytes) : -1;
                    SetShow(bar, used >= 0);
                    if (Element* body = used >= 0 && bar->Visual() ? bar->Visual()->Find("ProgressBody") : nullptr)
                        body->ShowFrame(std::clamp(used, 1, 100));
                }
            });
        });
    }

    void ShowKeyboard(const std::u16string& title, const std::u16string& description, const std::u16string& text,
                      size_t maxLength, bool afterUnloading, bool gamertag)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!Ready(s) || !EnsureHud(s))
            return;
        auto show = [=](State& s) {
            // An older system update's (vk.xex 1888 ... 9199): Keyboard.xur, the keys in the scene itself.
            if (const Package* vk = s.assets->FindPackage("vk.vk"); vk && !vk->Find("KeyboardMain.xur") &&
                vk->Find("Keyboard.xur"))
            {
                Element* pane = OpenPane(s, "vk.vk", "Keyboard.xur");
                if (!pane)
                    return;
                s.pane = Pane::kKeyboard;
                s.dimLayers = kDimKeyboard;
                auto& k = s.keyboard;
                k = {};
                k.legacy = true;
                k.group = pane;
                k.edit = pane->Find("XuiEdit1");
                SetText(pane->Find("Text_Header"), title);
                SetText(pane->Find("XuiText1"), description);
                for (const char* id : { "XuiText2", "idChatHistory", "FormattedText" })
                    SetShow(pane->Find(id), false);
                k.maxLength = maxLength;
                k.text = text.substr(0, maxLength);
                k.cursor = k.text.size();
                // Its letters start in the scene's case.
                if (const Element* a = pane->Find("Key.A"); a && !a->text.empty())
                    k.caps = a->text[0] >= u'A' && a->text[0] <= u'Z';
                for (const auto& key : pane->Children())
                    if (key->Id().starts_with("Key."))
                        key->PlayVisualState("Normal");
                KeyboardRefresh(s);
                KeyboardFocus(s, FocusTarget(pane->Find(pane->GetString("DefaultFocus"))), true);
                if (k.edit)
                    k.edit->PlayVisualState("Focus");
                KeyboardTick(s);
                return;
            }
            Element* pane = OpenPane(s, "vk.vk", "KeyboardMain.xur");
            if (!pane)
                return;
            s.pane = Pane::kKeyboard;
            s.dimLayers = kDimKeyboard;
            SetText(pane->Find("Text_Header"), title);
            Element* base = LoadInto(s, pane->Find("KeyboardPlaceholder"), "vk.vk", "KeyboardBase.xur");
            auto& k = s.keyboard;
            k = {};
            k.group = base ? base->Find("KeysGroup") : nullptr;
            if (!k.group)
                return;
            k.edit = base->Find("XuiEdit1");
            SetShow(base->Find("FormattedText"), false); // the IME's formatted line, unused by the Latin layouts
            SetText(base->Find("XuiText1"), description);
            k.maxLength = maxLength;
            k.gamertag = gamertag;
            k.text = text.substr(0, maxLength);
            k.cursor = k.text.size();
            // KeyboardMain names X Backspace and Y Space, but the console's legend shows only A and B there (capture
            // 4:19:08: the Backspace and Space keys carry the X and Y buttons).
            const std::u16string legend[4] = { Utf16(pane->GetString("LegendA")), Utf16(pane->GetString("LegendB")), u"",
                                               u"" };
            SetLegend(s, legend);
            // The Latin layouts have no Japanese comma / full stop keys; Backspace and Space each span five key columns
            // (measured on capture 4:19:08: 168 units at x 100 and 270 of the key group).
            SetShow(k.group->Find("Key.JComma"), false);
            SetShow(k.group->Find("Key.JPeriod"), false);
            if (Element* backspace = k.group->Find("Key.BS"))
            {
                backspace->SetPosition(100, 227);
                backspace->SetSize(168, 25);
            }
            if (Element* space = k.group->Find("Key.Spc"))
            {
                space->SetPosition(270, 227);
                space->SetSize(168, 25);
            }
            for (const auto& key : k.group->Children())
                key->PlayVisualState("Normal");
            KeyboardRefresh(s);
            KeyboardFocus(s, k.group->Find("Key.0_0"), true);
            if (k.edit)
                k.edit->PlayVisualState("Focus");
            KeyboardTick(s);
        };
        if (afterUnloading)
            ShowAfterUnloading(s, PaneState(s, "vk.vk", "KeyboardMain.xur"), 107.0f / 60.0f, show);
        else
            ShowWhenClosed(s, show);
    }

    KeyboardAction KeyboardPress(uint16_t pressed)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        KeyboardAction action = KeyboardAction::kNone;
        auto& k = s.keyboard;
        if (s.pane != Pane::kKeyboard || s.phase != Phase::kOpen || !k.group || s.afterPress)
            return action;
        constexpr uint16_t kUp = 0x0001, kDown = 0x0002, kLeft = 0x0004, kRight = 0x0008, kStart = 0x0010,
                           kBack = 0x0020, kLeftThumb = 0x0040, kLeftShoulder = 0x0100, kRightShoulder = 0x0200,
                           kLeftTrigger = 0x0400, kRightTrigger = 0x0800, kA = 0x1000, kB = 0x2000, kX = 0x4000,
                           kY = 0x8000;
        if (pressed & (kB | kBack))
            return KeyboardAction::kCancel;
        if (pressed & kUp)
            KeyboardNavigate(s, "NavUp");
        if (pressed & kDown)
            KeyboardNavigate(s, "NavDown");
        if (pressed & kLeft)
            KeyboardNavigate(s, "NavLeft");
        if (pressed & kRight)
            KeyboardNavigate(s, "NavRight");
        if (pressed & kA)
            KeyboardPressKey(s, k.focus, action);
        // The buttons the side keys show: X Backspace, Y Space, LB / RB the cursor, LT Symbols, RT Accents, the left
        // stick Caps, START Done.
        static const std::pair<uint16_t, const char*> kShortcuts[] = {
            { kX, "Key.BS" }, { kY, "Key.Spc" }, { kLeftShoulder, "Key.Left" }, { kRightShoulder, "Key.Right" },
            { kLeftTrigger, "Key.Prev" }, { kRightTrigger, "Key.Next" }, { kLeftThumb, "Key.Caps" },
            { kStart, "Key.OK" } };
        for (const auto& [button, id] : kShortcuts)
            if (pressed & button)
                KeyboardPressKey(s, k.group->Find(id), action);
        return action;
    }

    std::u16string KeyboardText()
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        return s.keyboard.text;
    }

    bool KeyboardOpen()
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        return s.pane == Pane::kKeyboard && s.phase == Phase::kOpen && s.keyboard.group;
    }

    void KeyboardTypeChar(char16_t c)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        auto& k = s.keyboard;
        if (s.pane != Pane::kKeyboard || s.phase != Phase::kOpen || !k.group)
            return;
        if (c == u'\b')
        {
            if (k.cursor > 0)
                k.text.erase(--k.cursor, 1);
        }
        else if (c >= 0x20 && c != 0x7F && (!k.gamertag || GamertagChar(c)))
            KeyboardType(s, c);
        else
            return;
        KeyboardRefresh(s);
        // Like a chatpad on the console: the focus jumps to the typed character's key (Backspace, Space) and the key
        // shows its Press, green as if selected there (owner's Patch Notes, "Not Fixed" 1). A character the page
        // does not show leaves the focus where it is.
        Element* typed = nullptr;
        if (c == u'\b')
            typed = k.group->Find("Key.BS");
        else if (c == u' ')
            typed = k.group->Find("Key.Spc");
        else
            for (int row = 0; row < 5 && !typed; row++)
                for (int col = 0; col < 10 && !typed; col++)
                    if (Element* key = k.group->Find(std::format("Key.{}_{}", row, col));
                        key && key->Shown() && !key->text.empty() && std::towlower(key->text[0]) == std::towlower(c))
                        typed = key;
        if (typed && typed->Shown())
        {
            KeyboardFocus(s, typed, false);
            if (typed->enabled)
            {
                typed->PlayVisualState("Press");
                if (std::find(k.pressed.begin(), k.pressed.end(), typed) == k.pressed.end())
                    k.pressed.push_back(typed);
            }
        }
        Log("System UI: keyboard typed: \"{}\" (cursor {})", Utf8(k.text), k.cursor);
    }
    namespace
    {
        // Icons the console's LIVE-delivered Guide modules have and the system update does not, drawn from XAM's own
        // images (64 x 64, white like XAM's, tinted where they are shown): "builtin:AddFriend" (the person with a
        // circled plus, "guide friends.png"), "builtin:Pencil" (Edit Text) and "builtin:SendMail" (the envelope with
        // speed lines, "guide send message.png"). Any other name: an image of xam.shrdres, or "<package>/<image>".
        const Image* IconImage(Assets& assets, std::string_view name, uint32_t tint = 0xFFFFFFFF)
        {
            if (!name.starts_with("builtin:"))
            {
                const size_t slash = name.find('/');
                return slash == std::string_view::npos ? assets.LoadImage(name, "xam.shrdres")
                                                       : assets.LoadImage(name.substr(slash + 1), name.substr(0, slash));
            }
            static std::map<std::string, std::unique_ptr<Image>, std::less<>> cache;
            const std::string key = std::format("{}#{:08X}", name, tint);
            if (auto it = cache.find(key); it != cache.end())
                return it->second.get();
            constexpr int kSize = 64;
            auto image = std::make_unique<Image>();
            image->width = image->height = kSize;
            image->rgba.assign(size_t(kSize) * kSize * 4, 0);
            // Coverage by 4 x 4 samples per pixel; paint raises the alpha (white), clear lowers it.
            auto shape = [&](auto inside, bool clear) {
                for (int y = 0; y < kSize; y++)
                    for (int x = 0; x < kSize; x++)
                    {
                        int hits = 0;
                        for (int j = 0; j < 4; j++)
                            for (int i = 0; i < 4; i++)
                                hits += inside(float(x) + (float(i) + 0.5f) / 4, float(y) + (float(j) + 0.5f) / 4);
                        if (!hits)
                            continue;
                        uint8_t* p = &image->rgba[(size_t(y) * kSize + size_t(x)) * 4];
                        const float a = float(hits) / 16;
                        p[0] = p[1] = p[2] = 255;
                        p[3] = clear ? uint8_t(float(p[3]) * (1 - a))
                                     : std::max<uint8_t>(p[3], uint8_t(a * 255 + 0.5f));
                    }
            };
            // An XAM image's alpha into the square (x0, y0, size), bilinear.
            auto copy = [&](const Image* src, float x0, float y0, float size) {
                if (!src || !src->width || !src->height)
                    return;
                for (int y = 0; y < kSize; y++)
                    for (int x = 0; x < kSize; x++)
                    {
                        const float u = (float(x) + 0.5f - x0) / size * float(src->width) - 0.5f;
                        const float v = (float(y) + 0.5f - y0) / size * float(src->height) - 0.5f;
                        if (u < -0.5f || v < -0.5f || u > float(src->width) - 0.5f || v > float(src->height) - 0.5f)
                            continue;
                        auto at = [&](int sx, int sy) {
                            sx = std::clamp(sx, 0, int(src->width) - 1);
                            sy = std::clamp(sy, 0, int(src->height) - 1);
                            return float(src->rgba[(size_t(sy) * src->width + size_t(sx)) * 4 + 3]);
                        };
                        const int ix = int(std::floor(u)), iy = int(std::floor(v));
                        const float fx = u - float(ix), fy = v - float(iy);
                        const float a = (at(ix, iy) * (1 - fx) + at(ix + 1, iy) * fx) * (1 - fy) +
                                        (at(ix, iy + 1) * (1 - fx) + at(ix + 1, iy + 1) * fx) * fy;
                        uint8_t* p = &image->rgba[(size_t(y) * kSize + size_t(x)) * 4];
                        p[0] = p[1] = p[2] = 255;
                        p[3] = std::max<uint8_t>(p[3], uint8_t(std::min(255.0f, a)));
                    }
            };
            auto segment = [](float px, float py, float ax, float ay, float bx, float by) {
                const float dx = bx - ax, dy = by - ay;
                const float t = std::clamp(((px - ax) * dx + (py - ay) * dy) / (dx * dx + dy * dy), 0.0f, 1.0f);
                return std::hypot(px - ax - t * dx, py - ay - t * dy);
            };
            if (name == "builtin:AddFriend")
            {
                copy(assets.LoadImage("ico_64x_friend.png", "xam.shrdres"), 0, 0, 58);
                shape([](float x, float y) { return std::hypot(x - 46, y - 46) < 17.5f; }, true);
                shape([](float x, float y) { const float r = std::hypot(x - 46, y - 46); return r < 14 && r > 10.5f; },
                      false);
                shape([](float x, float y) {
                    return (std::abs(x - 46) < 1.75f && std::abs(y - 46) < 7) ||
                           (std::abs(y - 46) < 1.75f && std::abs(x - 46) < 7);
                }, false);
            }
            else if (name == "builtin:Pencil")
            {
                // The body from the top right down to the tip at the bottom left, a gap before the tip.
                shape([&](float x, float y) { return segment(x, y, 52, 12, 22, 42) < 6.5f; }, false);
                shape([](float x, float y) {
                    // The tip: the triangle (8, 56), (15, 38), (26, 49).
                    auto side = [](float ax, float ay, float bx, float by, float px, float py) {
                        return (bx - ax) * (py - ay) - (by - ay) * (px - ax);
                    };
                    const float a = side(8, 56, 15, 38, x, y), b = side(15, 38, 26, 49, x, y),
                                c = side(26, 49, 8, 56, x, y);
                    return (a >= 0 && b >= 0 && c >= 0) || (a <= 0 && b <= 0 && c <= 0);
                }, false);
                shape([&](float x, float y) { return segment(x, y, 13, 37, 29, 53) < 1.5f; }, true);
            }
            else if (name == "builtin:SendMail")
            {
                copy(assets.LoadImage("ico_32x_Mail.png", "xam.shrdres"), 18, 10, 44);
                shape([](float x, float y) {
                    return x > 2 && x < 15 && ((y > 23 && y < 26.5f) || (y > 30.5f && y < 34) || (y > 38 && y < 41.5f));
                }, false);
            }
            else if (name == "builtin:Gavel" || name == "builtin:GavelQuestion")
            {
                // File a Complaint's gavel (owner's capture 3_17_24): the head across the top left, its two faces
                // marked, the handle down to the bottom right; More Information's smaller one with a "?".
                const bool question = name == "builtin:GavelQuestion";
                const float k = question ? 0.8f : 1.0f;
                shape([&](float x, float y) { return segment(x / k, y / k, 10, 30, 30, 10) < 9.0f; }, false);
                shape([&](float x, float y) {
                    return segment(x / k, y / k, 12, 13, 27, 28) < 1.6f || segment(x / k, y / k, 4, 21, 19, 36) < 1.2f ||
                           segment(x / k, y / k, 21, 4, 36, 19) < 1.2f;
                }, true);
                shape([&](float x, float y) { return segment(x / k, y / k, 27, 27, 56, 56) < 4.0f; }, false);
                if (question)
                {
                    shape([](float x, float y) {
                        const float r = std::hypot(x - 50, y - 22);
                        const float angle = std::atan2(y - 22, x - 50);
                        return r > 4.5f && r < 8.5f && !(angle > 0.6f && angle < 2.6f);
                    }, false);
                    shape([&](float x, float y) { return segment(x, y, 52, 29, 50, 36) < 2.2f; }, false);
                    shape([](float x, float y) { return std::hypot(x - 50, y - 43) < 2.8f; }, false);
                }
            }
            else if (name == "builtin:NoEntry")
            {
                // Don't File: a ring with a bar from its top left to its bottom right.
                shape([](float x, float y) { const float r = std::hypot(x - 32, y - 32); return r < 28 && r > 21; },
                      false);
                shape([&](float x, float y) { return std::hypot(x - 32, y - 32) < 24 && segment(x, y, 16, 16, 48, 48) < 4.0f; },
                      false);
            }
            else if (name == "builtin:RepQuestion")
            {
                // What Is Rep?: the gamercard's star with a "?" at its bottom right.
                copy(assets.LoadImage("star_full.png", "xam.gamercrd"), 0, 0, 50);
                shape([](float x, float y) {
                    const float r = std::hypot(x - 52, y - 44);
                    const float angle = std::atan2(y - 44, x - 52);
                    return r > 4 && r < 7.5f && !(angle > 0.6f && angle < 2.6f);
                }, false);
                shape([&](float x, float y) { return segment(x, y, 53.5f, 50, 52, 55) < 2.0f; }, false);
                shape([](float x, float y) { return std::hypot(x - 52, y - 60) < 2.5f; }, false);
            }
            else
                Log("System UI: no built-in image {}", name);
            // Tinted (a list's image presenter shows it as it is: dark on a normal row, white on the focused one).
            for (size_t i = 0; i < image->rgba.size(); i += 4)
            {
                image->rgba[i] = uint8_t(tint >> 16);
                image->rgba[i + 1] = uint8_t(tint >> 8);
                image->rgba[i + 2] = uint8_t(tint);
            }
            return cache.emplace(key, std::move(image)).first->second.get();
        }
    }

    // ---- The Guide's real Community scene (Guide.Friends.xex 17559's community.xur; NOTES session 20) ----

    namespace
    {
        constexpr const char* kFriendsPackage = "guide.friends.friends";

        // CommunityRootScene's tabs (its tab controller's table at 0x901033C8): the sub-scene, its list, the strip's
        // long label and number; the frame a tab is snapped to when the scene opens on it (0x901356F8). Next /
        // previous play "1To2" .. "3To4" / "2To1" .. "4To3" to their End frame (0x90135248 / 0x90135310); the strip
        // does not wrap.
        struct CommunityTabIds
        {
            const char* scene;
            const char* list;
            const char* label;
            const char* count;
            const char* snap;
        };
        constexpr CommunityTabIds kCommunityTabs[4] = {
            { "scene_Tab1", "control_List", "text_Party", "text_NumParty", "2To1End" },
            { "scene_Tab2", "control_list", "text_friends", "text_NumFriends", "1To2End" },
            { "scene_Tab3", "control_List", "text_messages", "text_NumMessages", "2To3End" },
            { "scene_Tab4", "control_list", "text_recentplayers", "text_NumRecentPlayers", "3To4End" },
        };
        // PartyScene's buttons above its member list (FUN_901295C8 shows them by party state).
        constexpr const char* kPartyButtons[3] = { "btnInvite", "btnInviteParty", "btnSettings" };

        // Guide.Friends.xex's strings.xus entry (the lock held), or `fallback`.
        std::u16string FriendsStringLocked(State& s, size_t index, const char16_t* fallback)
        {
            const auto* table = s.assets ? s.assets->Strings(kFriendsPackage, "strings.xus") : nullptr;
            return table && index < table->size() ? Utf16((*table)[index]) : std::u16string(fallback);
        }

        bool CommunitySceneReady(State& s)
        {
            return s.assets && s.assets->LoadScene(kFriendsPackage, "community.xur");
        }

        // The Party tab's focus: its buttons (0 .. n-1), then the member list (n ..), as the scene's NavDown / NavUp
        // chain them (btnSettings <-> control_List).
        void CommunityFocus(State& s, int index, bool initial)
        {
            auto& c = s.community;
            const int nb = int(c.buttons.size());
            index = std::clamp(index, 0, std::max(0, nb + s.list.count - 1));
            for (int i = 0; i < nb; i++)
            {
                Element* b = c.buttons[size_t(i)];
                const bool now = i == index, was = b->focused;
                const std::string off = b->enabled ? "" : "Disable";
                b->focused = now;
                if (now && (initial || !was))
                    b->PlayVisualState((initial ? "InitFocus" : "Focus") + off);
                else if (!now && (was || initial))
                    b->PlayVisualState(b->enabled ? (was ? "KillFocus" : "Normal") : "NormalDisable");
            }
            c.focus = index;
            if (s.list.list)
            {
                s.list.inactive = index < nb;
                if (!s.list.inactive)
                {
                    s.list.cur = index - nb;
                    s.list.top = std::min(s.list.top, s.list.cur);
                }
                ListRefresh(s, initial);
            }
            s.dirty = true;
        }

        void ShowCommunityScene(State& s, const std::vector<CommunityTab>& tabs, uint32_t activeTab,
                                const std::vector<CommunityRow>& rows, uint32_t focus, const std::u16string& message,
                                const CommunityOptions& options)
        {
            const int tab = int(std::min<uint32_t>(activeTab, 3));
            // Changing tabs on the open scene plays its transition; anything else opens it again.
            Element* root = s.phase == Phase::kOpen && s.community.root && s.hosted == s.community.root
                                ? s.community.root
                                : nullptr;
            const int previous = root ? s.community.tab : -1;
            if (!root)
            {
                root = OpenPane(s, kFriendsPackage, "community.xur");
                if (!root)
                    return;
                s.community.root = root;
                for (auto& p : s.community.pictures)
                    p.clear();
            }
            s.pane = State::Pane::kCommunity;
            s.community.tab = tab;
            if (previous >= 0 && std::abs(tab - previous) == 1)
                root->PlayNamed(std::format("{}To{}", previous + 1, tab + 1),
                                std::format("{}To{}End", previous + 1, tab + 1), false);
            else if (previous < 0 && s.communityLeft)
            {
                // Back from a drill-down (Guide.Friends' tab table 0x901033C8: Tab<N>Open hides the strip and the
                // tab when one opens, Tab<N>Close brings them back: 17 frames hidden, then 18 fading in).
                const std::string close = std::format("Tab{}Close", tab + 1);
                root->PlayNamed(close, "End" + close, false);
            }
            else if (previous != tab)
                root->PlayNamed(kCommunityTabs[tab].snap, kCommunityTabs[tab].snap, false);
            s.communityLeft = false;

            // The strip: every tab's long label and number (its list class sets both, FUN_9011B6A0 / FUN_90127ED0 ...).
            for (size_t i = 0; i < 4 && i < tabs.size(); i++)
            {
                SetText(root->Find(kCommunityTabs[i].label), tabs[i].label.empty() ? tabs[i].text : tabs[i].label);
                SetText(root->Find(kCommunityTabs[i].count), tabs[i].count);
            }

            const CommunityTabIds& ids = kCommunityTabs[tab];
            Element* sub = root->Find(ids.scene);
            Element* list = sub ? sub->Find(ids.list) : nullptr;
            auto& c = s.community;
            c.buttons.clear();
            const int nb = tab == 0 ? std::clamp(options.sceneButtons, 0, std::min(3, int(rows.size()))) : 0;
            if (tab == 0 && sub)
            {
                for (int k = 0; k < 3; k++)
                {
                    Element* b = sub->Find(kPartyButtons[k]);
                    SetShow(b, k < nb);
                    if (!b || k >= nb)
                        continue;
                    const CommunityRow& row = rows[size_t(k)];
                    const auto t = row.slotText.find(0);
                    b->text = t != row.slotText.end() ? t->second : row.line1;
                    b->enabled = row.enabled;
                    b->focused = false;
                    c.buttons.push_back(b);
                }
                Element* description = sub->Find("text_PartyDescription");
                SetText(description, message);
                SetShow(description, !message.empty());
                // PartyScene never fills text_PercentFull (FUN_901291D8 does not look it up).
                SetText(sub->Find("text_PercentFull"), u"");
            }
            if (tab == 1 && sub)
                SetText(sub->Find("label_Sort"), options.footer);
            if (tab == 2 && sub)
                SetText(sub->Find("text_PercentFull"), u""); // MessageList blanks it (FUN_9011E770)
            Element* total = tab == 1 && sub ? sub->Find("label_Total") : nullptr;
            SetShow(total, false);
            if (!list)
                return;
            // A list shown again keeps nothing of its previous slots.
            while (!list->Children().empty())
                list->Release(list->Children().back().get());
            const int count = int(rows.size()) - nb;
            SetShow(list, tab != 0 || count > 0);

            // Gamer pictures of the rows, decoded once (this tab's own: its list items were released above).
            auto& owned = c.pictures[size_t(tab)];
            owned.clear();
            std::vector<const Image*> pictures(rows.size());
            for (size_t i = 0; i < rows.size(); i++)
            {
                if (rows[i].picture.empty() || rows[i].pictureSlot < 0)
                    continue;
                auto image = std::make_unique<Image>();
                std::string error;
                if (!DecodePng(rows[i].picture.data(), rows[i].picture.size(), *image, error))
                {
                    Log("System UI: Community row {}: {}", i, error);
                    continue;
                }
                pictures[i] = image.get();
                owned.push_back(std::move(image));
            }
            Assets* assets = &*s.assets;
            c.rows = rows;
            auto fill = [=](Element* item, int index) {
                const CommunityRow& row = S().community.rows[size_t(nb + index)];
                item->text.clear();
                for (auto& t : item->dataText)
                    t.clear();
                for (auto& i : item->dataImage)
                    i = nullptr;
                for (auto& v : item->dataVisual)
                    v.reset();
                item->image = nullptr;
                for (const auto& [slot, v] : row.slotVisual)
                    if (slot > 0 && slot < Element::kDataSlots)
                    {
                        item->dataVisual[slot] = Element::CreateControl(*assets, "XuiControl", v.visual, 0, 0);
                        if (Element* visual = item->dataVisual[slot]->Visual())
                            visual->ShowFrame(v.frame);
                    }
                for (const auto& [slot, text] : row.slotText)
                    if (slot == 0)
                        item->text = text;
                    else if (slot > 0 && slot < Element::kDataSlots)
                        item->dataText[slot] = text;
                for (const auto& [slot, path] : row.slotImage)
                {
                    const Image* image = assets->LoadImage(path, kFriendsPackage);
                    if (slot == 0)
                        item->image = image;
                    else if (slot > 0 && slot < Element::kDataSlots)
                        item->dataImage[slot] = image;
                }
                if (const Image* picture = pictures[size_t(nb + index)])
                {
                    if (row.pictureSlot == 0)
                        item->image = picture;
                    else if (row.pictureSlot < Element::kDataSlots)
                        item->dataImage[row.pictureSlot] = picture;
                }
                if (item->enabled != row.enabled || item->checked != row.checked)
                {
                    item->enabled = row.enabled;
                    item->checked = row.checked;
                    item->PlayVisualState(item->focused ? (row.enabled ? "Focus" : "FocusDisable")
                                                        : (row.enabled ? "Normal" : "NormalDisable"));
                }
            };
            // The legend follows the focused row (X per row: Accept Friend Request, Delete ...); the Friends tab's
            // label_Total ("%1!d! of %2!d!", string 144) counts the friends, hidden on Add Friend and while every
            // row fits (FUN_9011CA18).
            const std::u16string ofFormat = [&] {
                const auto* strings = assets->Strings(kFriendsPackage, "strings.xus");
                return strings && strings->size() > 144 ? Utf16((*strings)[144]) : std::u16string(u"%1!d! of %2!d!");
            }();
            auto legendFor = [=](int combined) {
                std::u16string x = options.legendX;
                if (combined >= 0 && size_t(combined) < rows.size() && !rows[size_t(combined)].legendX.empty())
                    x = rows[size_t(combined)].legendX;
                const std::u16string texts[4] = { u"Select", u"Back", x, options.legendY };
                State& st = S();
                SetLegend(st, texts);
                if (total)
                {
                    const int listIndex = combined - nb;
                    const bool show = options.counterFrom >= 0 && listIndex >= options.counterFrom &&
                                      int(st.list.slots.size()) < count;
                    std::u16string text = ofFormat;
                    for (const auto& [key, value] : { std::pair{ std::u16string(u"%1!d!"), listIndex - options.counterFrom + 1 },
                                                      std::pair{ std::u16string(u"%2!d!"), count - options.counterFrom } })
                        if (const size_t at = text.find(key); at != std::u16string::npos)
                            text.replace(at, key.size(), Utf16(std::to_string(value)));
                    SetText(total, text);
                    SetShow(total, show);
                }
            };
            const bool startInButtons = nb > 0 && int(focus) < nb;
            ListBuild(
                s, list, count, startInButtons ? 0 : int(focus) - nb, fill,
                [=](int index) {
                    State& st = S();
                    legendFor(st.list.inactive ? st.community.focus : nb + index);
                },
                {}, 0, startInButtons);
            if (nb > 0)
                CommunityFocus(s, int(focus), true);
            else
                c.focus = s.list.cur;
        }
    }

    bool SetCommunityRowImage(uint32_t tab, uint32_t row, int slot, const std::string& path)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        auto& c = s.community;
        if (s.phase != Phase::kOpen || !c.root || s.hosted != c.root || s.pane != State::Pane::kCommunity ||
            c.tab != int(tab) || row >= c.rows.size() || row < c.buttons.size() || slot <= 0 ||
            slot >= Element::kDataSlots)
            return false;
        auto& images = c.rows[row].slotImage;
        const auto it = images.find(slot);
        if (path.empty() ? it == images.end() : it != images.end() && it->second == path)
            return true;
        if (path.empty())
            images.erase(it);
        else
            images[slot] = path;
        ListRefresh(s, false);
        return true;
    }

    bool CommunitySceneAvailable()
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        return Ready(s) && CommunitySceneReady(s);
    }

    std::u16string FriendsString(size_t index, const char16_t* fallback)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        const auto* table = Ready(s) ? s.assets->Strings(kFriendsPackage, "strings.xus") : nullptr;
        return table && index < table->size() ? Utf16((*table)[index]) : std::u16string(fallback);
    }

    void ShowCommunity(const std::u16string& title, const std::vector<CommunityTab>& tabs, uint32_t activeTab,
                       const std::vector<CommunityRow>& rows, uint32_t focus, const std::u16string& message,
                       const CommunityOptions& options)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!Ready(s) || !EnsureHud(s))
            return;
        ShowWhenClosed(s, [=](State& s) {
            // The console's own scene when the module's package is there, for the four-tab Community only (Party
            // Options and the Guide's games list use this pane without tabs; a message on another tab than Party,
            // "Sign in to Xbox Live ...", has no place in the scene): else drawn below.
            if (tabs.size() == 4 && CommunitySceneReady(s) && (message.empty() || activeTab == 0))
                return ShowCommunityScene(s, tabs, activeTab, rows, focus, message, options);
            const std::u16string& footer = options.footer;
            const bool oneLine = options.layout == CommunityLayout::kOneLine;
            const float rowHeight = oneLine ? 28 : 46; // btn_oneline_3strings / btn_twoline-icon_RightGS
            Element* pane = OpenPane(s, "gamerprofile.gp", "820_GameShowcaseMe.xur");
            if (!pane)
                return;
            s.pane = Pane::kCommunity;
            auto color = [](uint32_t argb) {
                Value v;
                v.type = Type::kColor;
                v.color = argb;
                return v;
            };
            auto integer = [](int64_t i) {
                Value v;
                v.type = Type::kInteger;
                v.i = i;
                return v;
            };
            SetText(pane->Find("headerText"), title);
            SetShow(pane->Find("waitCursorControl"), false);
            Element* list = pane->Find("gameList");
            // The tab strip across the list's top (x 138, width 574; tabs 31 high at y 61, as QuickLaunch.xur's):
            // the active tab 260 wide (tab_activeGS1's own width) with a dark icon and its text, the others share
            // the rest with a white icon and a count. The list starts under it (rows at y 99, six of them), the
            // footer under the rows.
            // (The active tab is 258 wide and its icon at 18 on the console: captures 7_47_01, "party chat/*.png".)
            constexpr float kLeft = 138, kWidth = 574, kTop = 61, kTabHeight = 31, kActiveWidth = 258;
            // The console's inactive tabs are darker than the skin's tab_inactiveGS2 (76,91,103 against 91,104,117)
            // and one that follows another has a soft shadow along its left edge.
            static const Image kShade = [] {
                Image i;
                i.width = i.height = 1;
                i.rgba = { 0, 0, 0, 38 };
                return i;
            }();
            static const Image kSeam = [] {
                Image i;
                i.width = 16;
                i.height = 1;
                for (uint32_t px = 0; px < i.width; px++)
                {
                    const float a = px < 3 ? float(px + 1) / 3.0f : std::max(0.0f, 1.0f - float(px - 3) / 12.0f);
                    i.rgba.insert(i.rgba.end(), { 0, 0, 0, uint8_t(40.0f * a + 0.5f) });
                }
                return i;
            }();
            float x = kLeft;
            for (size_t i = 0; i < tabs.size(); i++)
            {
                const bool active = i == activeTab;
                const float w = active ? kActiveWidth
                                       : tabs.size() == 1 ? kWidth
                                       : std::round((kWidth - kActiveWidth) / float(tabs.size() - 1));
                const float width = i + 1 == tabs.size() ? kLeft + kWidth - x : w;
                auto tab = Element::CreateControl(*s.assets, "XuiControl", active ? "tab_activeGS1" : "tab_inactiveGS2",
                                                  width, kTabHeight);
                tab->SetPosition(x, kTop);
                pane->Adopt(std::move(tab));
                if (!active)
                {
                    auto shade = Element::CreateControl(*s.assets, "XuiImage", "", width, kTabHeight);
                    shade->SetPosition(x, kTop);
                    shade->Set("SizeMode", integer(4));
                    shade->image = &kShade;
                    pane->Adopt(std::move(shade));
                    if (i > 0 && i - 1 != activeTab)
                    {
                        auto seam = Element::CreateControl(*s.assets, "XuiImage", "", 7, kTabHeight);
                        seam->SetPosition(x - 1, kTop);
                        seam->Set("SizeMode", integer(4));
                        seam->image = &kSeam;
                        pane->Adopt(std::move(seam));
                    }
                }
                const float iconX = x + (active ? 18 : 30), textX = x + (active ? 49 : 53);
                auto icon = Element::CreateControl(*s.assets, "XuiImage", "", 21, 21);
                icon->SetPosition(iconX, kTop + 5);
                icon->SetString("ImagePath", tabs[i].icon);
                icon->Set("SizeMode", integer(4));
                if (active)
                    icon->Set("ColorFactor", color(0xFF111B21));
                pane->Adopt(std::move(icon));
                auto label = Element::CreateControl(*s.assets, "XuiLabel",
                                                    active ? "XuiLabelLeftJustify1" : "Label_Head",
                                                    std::max(1.0f, x + width - textX - 4), 21);
                label->SetPosition(textX, kTop + 4);
                label->text = tabs[i].text;
                pane->Adopt(std::move(label));
                x += width;
            }
            // Without tabs (Party Options) the list starts where they would be.
            const float listTop = tabs.empty() ? kTop : 99;
            if (list && (!tabs.empty() || !rows.empty()))
            {
                list->SetPosition(kLeft, listTop);
                list->SetSize(list->Width(), 6 * 46 + 99 - listTop + (tabs.empty() ? 8 : 0)); // 7 rows without tabs
            }
            Element* empty = pane->Find("noGamesText");
            SetText(empty, message);
            SetShow(empty, !message.empty());
            // Under the rows: 40 units below two-line rows; 79 below one-line rows ("start a party.png").
            if (empty && !rows.empty())
                empty->SetPosition(empty->GetVector("Position", {}).x,
                                   listTop + rowHeight * float(rows.size()) + (oneLine ? 79 : 40));
            if (!footer.empty())
            {
                auto label = Element::CreateControl(*s.assets, "XuiLabel", "XuiLabelLeftJustify", 538, 26);
                label->SetPosition(156, 376);
                label->text = footer;
                pane->Adopt(std::move(label));
            }
            // "n of N" at the footer's right (the friends counted from counterFrom) once the list has more rows than
            // its page shows (six two-line rows).
            Element* counter = nullptr;
            const int counted = options.counterFrom < 0 ? 0 : int(rows.size()) - options.counterFrom;
            if (options.counterFrom >= 0 && int(rows.size()) > (oneLine ? 9 : 6) && counted > 0)
            {
                auto label = Element::CreateControl(*s.assets, "XuiText", "", 120, 26);
                label->SetPosition(560, 376);
                label->SetFloat("PointSize", 12);
                label->Set("TextColor", color(0xFF333A40));
                label->Set("DropShadowColor", color(0xFFEBEBEB));
                label->Set("TextStyle", integer(0x5211)); // shadow, vertical centre, right-aligned
                counter = pane->Adopt(std::move(label));
            }
            // The legend, its X following the focused row (CommunityRow::legendX), and the counter.
            auto onFocus = [=](int index) {
                std::u16string x = options.legendX;
                if (index >= 0 && size_t(index) < rows.size() && !rows[size_t(index)].legendX.empty())
                    x = rows[size_t(index)].legendX;
                const std::u16string texts[4] = { u"Select", u"Back", x, options.legendY };
                SetLegend(S(), texts);
                if (counter)
                    counter->SetString("Text", index >= options.counterFrom
                                                   ? std::format("{} of {}", index - options.counterFrom + 1, counted)
                                                   : std::string());
            };
            const std::u16string legend[4] = { u"Select", u"Back", options.legendX, options.legendY };
            SetLegend(s, legend);
            if (!list)
                return;
            s.images.clear();
            std::vector<const Image*> pictures(rows.size());
            for (size_t i = 0; i < rows.size(); i++)
            {
                if (!rows[i].icon.empty())
                {
                    pictures[i] = rows[i].icon.starts_with("builtin:") ? IconImage(*s.assets, rows[i].icon)
                                                                       : s.assets->LoadImage(rows[i].icon,
                                                                                             rows[i].iconPackage);
                    continue;
                }
                if (rows[i].picture.empty())
                    continue;
                auto image = std::make_unique<Image>();
                std::string error;
                if (!DecodePng(rows[i].picture.data(), rows[i].picture.size(), *image, error))
                {
                    Log("System UI: Community row {}: {}", i, error);
                    continue;
                }
                pictures[i] = image.get();
                s.images.push_back(std::move(image));
            }
            Assets* assets = &*s.assets;
            if (oneLine)
            {
                // btn_oneline_3strings: txt_line1 (the item text) at x 18, txt_line2 (data 1) at 213, txt_line3
                // (data 2) at 408, one line each; no image. A row with text only spans the width. Party members and
                // settings get parts of their own, made once per list slot and found by their "Part" tag (measured
                // on the owner's captures "in a party.png" / "party settings *.png", in list units): a member's
                // 22-unit picture at x 26 with the gamertag from 53, the tinted controller at 279, the title from
                // 339, the leader's mark at 544 (the console's icon, a seated person with a star, is in the
                // LIVE-delivered Guide module: XAM's person and reputation star here); a setting's name keeps the
                // normal colours on the focused row (the Disable timelines' colours: dark text, light shadow) and
                // its value is centred from 242 to 560 between < > on the focused row.
                const Image* person = assets->LoadImage("ico_32x_Friend.png", "xam.shrdres");
                const Image* star = assets->LoadImage("ico_32x_RepStar.png", "xam.skin");
                ListBuild(s, list, int(rows.size()), int(focus), [=](Element* item, int index) {
                    const CommunityRow& row = rows[size_t(index)];
                    const bool setting = !row.value.empty();
                    item->text = setting ? std::u16string() : row.line1;
                    // A message row (a status image, not a member): the sender, the envelope at 300, the text from
                    // 325 ("guide messages.png").
                    const bool messageRow = !row.member && !setting && !row.status.empty();
                    item->dataText[1] = setting ? row.value : row.member ? row.right : row.line2;
                    item->dataText[2] = row.member || messageRow ? std::u16string() : row.right;
                    Element* visual = item->Visual();
                    if (visual)
                    {
                        auto part = [&](const char* tag, const char* className, float x, float y, float w, float h) {
                            for (const auto& c : visual->Children())
                                if (c->GetString("Part") == tag)
                                    return c.get();
                            auto e = Element::CreateControl(*assets, className, "", w, h);
                            e->SetPosition(x, y);
                            e->SetString("Part", tag);
                            if (std::string_view(className) == "XuiImage")
                                e->Set("SizeMode", integer(4));
                            else
                            {
                                e->SetFloat("PointSize", 12);
                                e->Set("TextStyle", integer(0x5011)); // shadow, vertical centre, no wrap, ellipsis
                            }
                            return visual->Adopt(std::move(e));
                        };
                        Element* line1 = visual->Find("txt_line1");
                        Element* line2 = visual->Find("txt_line2");
                        // The Community's text is 12 points (the captures' widths), the skin's 11.
                        for (const char* id : { "txt_line1", "txt_line2", "txt_line3" })
                            if (Element* t = visual->Find(id))
                                t->SetFloat("PointSize", 12);
                        const bool textOnly = row.line2.empty() && row.right.empty() && !row.member && !setting;
                        if (line1)
                        {
                            line1->SetPosition(row.member ? 53 : 18, 0);
                            line1->SetSize(row.member ? 220 : textOnly ? 540 : messageRow ? 270 : 187, 26);
                        }
                        if (line2)
                        {
                            line2->SetPosition(setting ? 242 : row.member ? 339 : messageRow ? 325 : 213, 0);
                            line2->SetSize(setting ? 318 : row.member ? 200 : messageRow ? 240 : 187, 26);
                            line2->Set("TextStyle", integer(setting ? 0x5411 : 0x5011)); // 0x400: centred
                        }
                        Element* picture = part("picture", "XuiImage", 26, 3, 22, 22);
                        picture->image = row.member ? pictures[size_t(index)] : nullptr;
                        Element* status = part("status", "XuiImage", 278, 6, 16, 16);
                        status->SetPosition(messageRow ? 300 : 278, 6);
                        SetShow(status, (row.member || messageRow) && !row.status.empty());
                        status->SetString("ImagePath", row.status);
                        status->Set("ColorFactor", color(row.statusColor));
                        // The leader's mark: the person, then the star on a light outline (a larger star in the
                        // row's colour behind it), as the console's icon separates them.
                        Element* mark = part("leader", "XuiImage", 539, 3, 22, 22);
                        mark->image = row.leader ? person : nullptr;
                        mark->Set("ColorFactor", color(0xFF1E2328));
                        Element* outline = part("starOutline", "XuiImage", 551, 9, 17, 17);
                        outline->image = row.leader ? star : nullptr;
                        outline->Set("ColorFactor", color(0xFFDDE1E3));
                        Element* markStar = part("star", "XuiImage", 553, 11, 13, 13);
                        markStar->image = row.leader ? star : nullptr;
                        markStar->Set("ColorFactor", color(0xFF1E2328));
                        const bool focused = S().list.cur == index;
                        Element* name = part("name", "XuiText", 18, 0, 220, 26);
                        name->SetString("Text", setting ? Utf8(row.line1) : std::string());
                        name->Set("TextColor", color(focused ? 0xFF656D72 : 0xFF333A40));
                        name->Set("DropShadowColor", color(focused ? 0x96EBEBEB : 0xFFEBEBEB));
                        for (const auto& [tag, x, text] : { std::tuple{ "less", 236.0f, "<" },
                                                            std::tuple{ "more", 554.0f, ">" } })
                        {
                            Element* arrow = part(tag, "XuiText", x, 0, 16, 26);
                            arrow->SetString("Text", text);
                            arrow->SetFloat("PointSize", 10); // the console's chevrons are smaller than the text
                            arrow->Set("TextColor", color(0xFFEBEBEB));
                            arrow->Set("TextStyle", integer(0x1010)); // vertical centre, no wrap, no shadow
                            SetShow(arrow, setting && focused);
                        }
                    }
                    // Greyed rows play the skin's Disable states (ListRefresh); a slot that changes plays it now.
                    if (item->enabled != row.enabled)
                    {
                        item->enabled = row.enabled;
                        item->PlayVisualState(item->focused ? (row.enabled ? "Focus" : "FocusDisable")
                                                            : (row.enabled ? "Normal" : "NormalDisable"));
                    }
                }, onFocus, "btn_oneline_3strings", rowHeight);
                return;
            }
            // btn_twoline-icon_RightGS: txt_line1 (the item text), txt_line2 (data 2), txt_line3 (data 1, the right
            // column), image_presenter3 (data 3). The right column is laid out as the console's Community rows: its
            // text left-aligned from x 284 level with line 1 (two lines: the Players tab's title and "Last met"),
            // the gamerscore icon (XuiImage1) turned into the status person at x 257.
            ListBuild(s, list, int(rows.size()), int(focus), [=](Element* item, int index) {
                const CommunityRow& row = rows[size_t(index)];
                item->text = row.line1;
                item->dataText[1] = row.right;
                item->dataText[2] = row.line2;
                item->dataImage[3] = pictures[size_t(index)];
                // A built-in icon (Add Friend) is dark, white on the focused row ("guide friends.png" / "2.png").
                if (row.icon.starts_with("builtin:"))
                    item->dataImage[3] = IconImage(*assets, row.icon, S().list.cur == index ? 0xFFFFFFFF : 0xFF3C4146);
                Element* visual = item->Visual();
                if (!visual)
                    return;
                if (Element* right = visual->Find("txt_line3"))
                {
                    right->SetPosition(284, 0);
                    right->SetSize(item->Width() - 284 - (options.rightAligned ? 18 : 8), 47);
                    // shadow, no wrap, ellipsis; top-left (or right-aligned, centred)
                    right->Set("TextStyle", integer(options.rightAligned ? 0x5211 : 0x4011));
                }
                // (XuiImage1's ColorFactor belongs to the visual's state timelines, so the tinted person is an image
                // of its own, made once per list slot.)
                SetShow(visual->Find("XuiImage1"), false);
                Element* status = nullptr;
                for (const auto& c : visual->Children())
                    if (c->ClassName() == "XuiImage" && c->Id().empty())
                        status = c.get();
                if (!status)
                {
                    auto image = Element::CreateControl(*assets, "XuiImage", "", 14, 14);
                    image->SetPosition(257, 6);
                    image->Set("SizeMode", integer(16));
                    status = visual->Adopt(std::move(image));
                }
                SetShow(status, !row.status.empty());
                status->SetString("ImagePath", row.status);
                status->Set("ColorFactor", color(row.statusColor));
                // The Community's text is 12 points (the owner's captures), the skin's two-line rows 11.
                for (const char* id : { "txt_line1", "txt_line2", "txt_line3" })
                    if (Element* t = visual->Find(id))
                        t->SetFloat("PointSize", 12);
            }, onFocus);
        });
    }

    namespace
    {
        // A Profile row's visual state: btn_standard_list's Disable variants for greyed rows.
        void ProfileFocus(State& s, uint32_t index, bool initial)
        {
            if (s.buttons.empty())
                return;
            index = std::min<uint32_t>(index, uint32_t(s.buttons.size() - 1));
            for (uint32_t i = 0; i < s.buttons.size(); i++)
            {
                Element* b = s.buttons[i];
                const bool now = i == index, was = b->focused;
                const std::string off = b->enabled ? "" : "Disable";
                b->focused = now;
                if (now && (initial || !was))
                    b->PlayVisualState((initial ? "InitFocus" : "Focus") + off);
                else if (!now && (was || initial))
                    b->PlayVisualState(b->enabled ? (was ? "KillFocus" : "Normal") : "NormalDisable");
            }
            s.focus = index;
            s.dirty = true;
        }

        // GamerCard_Custom.xus #4 "Offline", #5 "Online", #6 "%1!s! since %2!u!/%3!u!", #7 / #8 "... for 1 hour / %2!u!
        // hours", #9 / #10 "... for 1 min / %2!u! mins": the card's status line.
        std::u16string GamerCardStatus(State& s, const GamerCardInfo& card)
        {
            const auto* strings = s.assets->Strings(Base17559(s, "xam.gamercrd"), "GamerCard_Custom.xus");
            auto text = [&](size_t i, const char16_t* fallback) {
                return strings && i < strings->size() ? Utf16((*strings)[i]) : std::u16string(fallback);
            };
            auto fill = [](std::u16string f, std::initializer_list<std::u16string> args) {
                int n = 1;
                for (const std::u16string& a : args)
                {
                    for (const char16_t* spec : { u"!s!", u"!u!" })
                    {
                        const std::u16string key = u"%" + Utf16(std::to_string(n)) + spec;
                        if (const size_t at = f.find(key); at != std::u16string::npos)
                            f.replace(at, key.size(), a);
                    }
                    n++;
                }
                return f;
            };
            if (card.online)
                return text(5, u"Online");
            const std::u16string offline = text(4, u"Offline");
            const int64_t t = card.offlineSeconds;
            if (t < 0)
                return offline;
            if (t < 3600)
            {
                const int64_t m = std::max<int64_t>(1, t / 60);
                return m == 1 ? fill(text(9, u"%1!s! for 1 min"), { offline })
                              : fill(text(10, u"%1!s! for %2!u! mins"), { offline, Utf16(std::to_string(m)) });
            }
            if (t < 24 * 3600)
            {
                const int64_t h = t / 3600;
                return h == 1 ? fill(text(7, u"%1!s! for 1 hour"), { offline })
                              : fill(text(8, u"%1!s! for %2!u! hours"), { offline, Utf16(std::to_string(h)) });
            }
            const std::time_t since = std::time(nullptr) - std::time_t(t);
            std::tm local{};
            localtime_s(&local, &since);
            return fill(text(6, u"%1!s! since %2!u!/%3!u!"),
                        { offline, Utf16(std::to_string(local.tm_mon + 1)), Utf16(std::to_string(local.tm_mday)) });
        }
    }

    std::u16string GamerProfileString(size_t index, const char16_t* fallback)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        const auto* table = Ready(s) ? s.assets->Strings(Base17559(s, "gamerprofile.gp"), "GamerProfile_Custom.xus") : nullptr;
        return table && index < table->size() ? Utf16((*table)[index]) : std::u16string(fallback);
    }

    namespace
    {
        Element* FillGamerCard(State& s, Element* host, const GamerCardInfo& card, bool extended);
    }

    namespace
    {
        // A short list's rows as controls (Guide.Friends' option lists hold at most a page): the visual's template
        // item (control_ListItem: its visual, size and position, then hidden), one per row stacked under it.
        std::vector<Element*> ListRows(State& s, Element* list, const std::vector<ProfileRow>& rows,
                                       const std::vector<std::string>* icons = nullptr)
        {
            std::vector<Element*> out;
            if (!list)
                return out;
            while (!list->Children().empty())
                list->Release(list->Children().back().get());
            Element* item = nullptr;
            if (Element* visual = list->Visual())
                for (const auto& c : visual->Children())
                    if (c->ClassName() == "XuiListItem" && !item)
                        item = c.get();
            if (!item)
            {
                Log("System UI: list {} has no item template", list->Id());
                return out;
            }
            SetShow(item, false);
            const Vec3 p = item->GetVector("Position", {});
            const float w = item->Width(), h = std::max(1.0f, item->DesignHeight());
            const std::string visual = item->GetString("Visual");
            for (size_t i = 0; i < rows.size(); i++)
            {
                auto row = Element::CreateControl(*s.assets, "XuiListItem", visual, w, h);
                row->SetPosition(p.x, p.y + h * float(i));
                row->text = rows[i].text;
                row->enabled = rows[i].enabled;
                if (icons && i < icons->size())
                {
                    row->imagePath = (*icons)[i];
                    row->imageBase = kFriendsPackage;
                }
                out.push_back(list->Adopt(std::move(row)));
            }
            return out;
        }

        // A player's Profile as Guide.Friends.xex draws it (ProfileRoot.xur: ProfileRootScene's two tabs, each a
        // FriendOptionsScene with its option list and XAM's gamercard; 2To1End / 1To2End snapped on opening, 1To2 /
        // 2To1 played on LB / RB). Tab 1: the card with its extended panel; tab 2: the base card and Personal_Card
        // (name, location, bio).
        void ShowProfileRoot(State& s, uint32_t activeTab, const std::vector<ProfileRow>& rows, uint32_t focus,
                             const GamerCardInfo& card)
        {
            const int tab = activeTab ? 1 : 0;
            Element* root = s.phase == Phase::kOpen && s.profileRoot.root && s.hosted == s.profileRoot.root
                                ? s.profileRoot.root
                                : nullptr;
            const int previous = root ? s.profileRoot.tab : -1;
            if (!root)
            {
                root = OpenPane(s, kFriendsPackage, "ProfileRoot.xur");
                if (!root)
                    return;
                s.profileRoot.root = root;
                s.images.clear();
            }
            s.pane = State::Pane::kProfile;
            s.profileRoot.tab = tab;
            const char* end = tab ? "1To2End" : "2To1End";
            if (previous >= 0 && previous != tab)
                root->PlayNamed(tab ? "1To2" : "2To1", end, false);
            else if (previous != tab)
                root->PlayNamed(end, end, false);
            Element* sub = root->Find(tab ? "scene_Tab2" : "scene_Tab1");
            if (!sub)
                return;
            s.buttons = ListRows(s, sub->Find("XuiList"), rows);
            // (Pictures stay in s.images while the pane is open: the other tab's card shows during the slide.)
            if (Element* host = sub->Find("gamerCard"))
            {
                while (!host->Children().empty())
                    host->Release(host->Children().back().get());
                FillGamerCard(s, host, card, tab == 0);
            }
            if (tab == 1)
            {
                SetText(sub->Find("labelRealName"), card.name);
                SetText(sub->Find("labelRealLocation"), card.location);
                SetText(sub->Find("labelRealBio"), card.bio);
            }
            const std::u16string legend[4] = { u"Select", u"Back", u"", u"" };
            SetLegend(s, legend);
            ProfileFocus(s, focus, true);
        }
    }


    namespace
    {
        // The composer as Guide.Friends.xex draws it (NewMessage.xur, SendMessageScene FUN_9012C860): list1's rows
        // (List_2Line-icon: recipients, Add Voice, Add / Edit Text, Add Picture, with the module's 64-unit icons),
        // btn1 Send Message under them, "To:" with the gamertags (RecipientsId), the type in MessageTitle, the text
        // in MessageId. `page.bottomRow` is btn1.
        void ShowComposeScene(State& s, const std::vector<ProfileRow>& rows, uint32_t focus, const ProfilePage& page,
                              const std::u16string& header)
        {
            Element* pane = OpenPane(s, kFriendsPackage, "NewMessage.xur");
            if (!pane)
                return;
            s.pane = State::Pane::kProfile;
            SetText(pane->Find("MessageTitle"), header);
            SetText(pane->Find("RecipientsId"), page.to);
            SetText(pane->Find("MessageId"), page.text);
            if (Element* icon = pane->Find("MessageIcon"); icon && !page.icon.empty())
                icon->SetString("ImagePath", page.icon);
            std::vector<ProfileRow> listRows;
            std::vector<std::string> icons;
            for (size_t i = 0; i < rows.size(); i++)
                if (int(i) != page.bottomRow)
                {
                    listRows.push_back(rows[i]);
                    icons.push_back(i < page.rowIcons.size() ? page.rowIcons[i] : std::string());
                }
            std::vector<Element*> list = ListRows(s, pane->Find("list1"), listRows, &icons);
            Element* send = pane->Find("btn1");
            SetShow(send, page.bottomRow >= 0 && size_t(page.bottomRow) < rows.size());
            s.buttons.clear();
            size_t next = 0;
            for (size_t i = 0; i < rows.size(); i++)
                if (int(i) == page.bottomRow)
                {
                    if (send)
                    {
                        send->text = rows[i].text;
                        send->enabled = rows[i].enabled;
                        s.buttons.push_back(send);
                    }
                }
                else if (next < list.size())
                    s.buttons.push_back(list[next++]);
            const std::u16string legend[4] = { u"Select", page.legendB.empty() ? u"Back" : page.legendB, u"", u"" };
            SetLegend(s, legend);
            ProfileFocus(s, focus, true);
        }

        // A received message as Guide.Friends.xex draws it (Messages_Options2.xur, MessageOptionsScene
        // FUN_90121480): the details ("Message Received", then the date, time and "Expires in ..." in
        // labSentDateTime), the option rows (CommandList), the sender's 85-unit card (no extended panel), the type
        // in MessageTitle with its icon, the text under "Message Text". `page.info`'s first line is the title.
        void ShowMessageScene(State& s, const std::vector<ProfileRow>& rows, uint32_t focus, const GamerCardInfo& card,
                              const ProfilePage& page, const std::u16string& header)
        {
            Element* pane = OpenPane(s, kFriendsPackage, "Messages_Options2.xur");
            if (!pane)
                return;
            s.pane = State::Pane::kProfile;
            s.images.clear();
            SetText(pane->Find("MessageTitle"), header);
            const size_t firstBreak = page.info.find(u'\n');
            SetText(pane->Find("labSentDateTime"),
                    firstBreak == std::u16string::npos ? std::u16string() : page.info.substr(firstBreak + 1));
            SetText(pane->Find("MessageTextId"), page.text);
            if (Element* icon = pane->Find("MessageIcon"))
                icon->SetString("ImagePath", page.icon.empty() ? std::string("sharedres://ico_32x_Mail.png") : page.icon);
            // No pictures or voice on our LIVE's messages (the scene shows both attachment icons until told).
            for (const char* id : { "PhotoAttachmentIcon", "VoiceAttachmentIcon" })
                SetShow(pane->Find(id), false);
            FillGamerCard(s, pane->Find("gamerCard"), card, false);
            s.buttons = ListRows(s, pane->Find("CommandList"), rows);
            const std::u16string legend[4] = { u"Select", u"Back", u"", u"" };
            SetLegend(s, legend);
            ProfileFocus(s, focus, true);
        }
    }

    void ShowGamerProfile(const std::vector<std::u16string>& tabs, uint32_t activeTab,
                          const std::vector<ProfileRow>& rows, uint32_t focus, const GamerCardInfo& card,
                          ProfilePanel panel, const std::u16string& header, const ProfilePage& page)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!Ready(s) || !EnsureHud(s))
            return;
        ShowWhenClosed(s, [=](State& s) {
            // Another player's Profile (two tabs, no page) is Guide.Friends.xex's ProfileRoot.xur when it is there.
            if (tabs.size() == 2 && page.info.empty() && page.card && page.to.empty() && page.textTitle.empty() &&
                page.text.empty() && s.assets->LoadScene(kFriendsPackage, "ProfileRoot.xur"))
                return ShowProfileRoot(s, activeTab, rows, focus, card);
            // The composer and a received message: the module's NewMessage.xur / Messages_Options2.xur.
            if (!page.card && tabs.empty() && s.assets->LoadScene(kFriendsPackage, "NewMessage.xur"))
                return ShowComposeScene(s, rows, focus, page, header);
            if (page.card && !page.info.empty() && tabs.empty() && page.rowDescriptions.empty() &&
                s.assets->LoadScene(kFriendsPackage, "Messages_Options2.xur"))
                return ShowMessageScene(s, rows, focus, card, page, header);
            Element* pane = OpenPane(s, "gamerprofile.gp", "GamerCardScene.xur");
            if (!pane)
                return;
            s.pane = Pane::kProfile;
            if (!header.empty())
                SetText(pane->Find("headerText"), header);
            s.images.clear();
            // Its own menus (the signed-in player's: View Games, Edit Profile ...) give way to the caller's rows.
            for (const char* id : { "meOfflineButtonGroup", "meOnlineButtonGroup", "onlineTextGroup" })
                SetShow(pane->Find(id), false);
            // The tab strip across the top (x 138, width 574, y 61, 31 high as the Community's); the menu and the
            // right panel start under it (7_47_09: the card's top at y 101).
            constexpr float kLeft = 138, kWidth = 574, kTop = 61, kTabHeight = 31;
            const float shift = tabs.empty() ? 0 : kTabHeight;
            float x = kLeft;
            for (size_t i = 0; i < tabs.size(); i++)
            {
                const bool active = i == activeTab;
                const float width = i + 1 == tabs.size() ? kLeft + kWidth - x : std::round(kWidth / float(tabs.size()));
                auto tab = Element::CreateControl(*s.assets, "XuiControl", active ? "tab_activeGS1" : "tab_inactiveGS2",
                                                  width, kTabHeight);
                tab->SetPosition(x, kTop);
                pane->Adopt(std::move(tab));
                auto label = Element::CreateControl(*s.assets, "XuiLabel",
                                                    active ? "XuiLabelLeftJustify1" : "Label_Head", width - 36, 21);
                const float measured = float(label->MeasureText(*s.assets, tabs[i]));
                label->SetPosition(active ? x + 18 : x + std::max(18.0f, (width - measured) / 2), kTop + 4);
                label->text = tabs[i];
                pane->Adopt(std::move(label));
                x += width;
            }
            for (const char* id : { "graphic_metapane", "GamerCardBackground", "gamerCard" })
                if (Element* e = pane->Find(id))
                {
                    const Vec3 p = e->GetVector("Position", {});
                    e->SetPosition(p.x, p.y + shift);
                    if (std::string_view(id) == "graphic_metapane")
                        e->SetSize(e->Width(), e->Height() - shift);
                }
            auto color = [](uint32_t argb) {
                Value v;
                v.type = Type::kColor;
                v.color = argb;
                return v;
            };
            auto text = [&](float x, float y, float w, float h, const std::u16string& value, uint32_t argb,
                            uint32_t shadow, int64_t style) {
                auto e = Element::CreateControl(*s.assets, "XuiText", "", w, h);
                e->SetPosition(x, y);
                e->SetString("Text", Utf8(value));
                e->SetFloat("PointSize", 12);
                e->Set("TextColor", color(argb));
                e->Set("DropShadowColor", color(shadow));
                Value v;
                v.type = Type::kInteger;
                v.i = style;
                e->Set("TextStyle", v);
                return pane->Adopt(std::move(e));
            };
            // A message's details over the rows: dark lines, 20 units apart ("guide message.png").
            float rowsTop = 62 + shift;
            if (!page.info.empty())
            {
                const size_t lines = size_t(std::count(page.info.begin(), page.info.end(), u'\n')) + 1;
                // The feedback pages' message wraps above the rows at the bottom (owner's capture 3_17_24).
                text(kLeft + 18, rowsTop + 4, 260, page.rowsAtBottom ? 180 : 20 * float(lines), page.info, 0xFF333A40,
                     0xFFEBEBEB, 0x0001);
                rowsTop += 20 * float(lines) + 12;
            }
            // The rows: btn_standard_list buttons (287 x 28, as the scene's own menus), from y 62 + the strip; with
            // icons 46 high, the text after the icon; the bottom row at the menu's bottom (the panel's, y 411).
            const bool icons = !page.rowIcons.empty();
            const float rowHeight = icons ? 46 : 28;
            s.buttons.clear();
            for (size_t i = 0; i < rows.size(); i++)
            {
                auto button = Element::CreateControl(*s.assets, "XuiButton", "btn_standard_list", 287, rowHeight);
                const bool bottom = int(i) == page.bottomRow;
                button->SetPosition(kLeft, page.rowsAtBottom ? 411 - rowHeight * float(rows.size() - i) :
                                           bottom ? 411 - rowHeight : rowsTop + rowHeight * float(i));
                button->text = rows[i].text;
                button->enabled = rows[i].enabled;
                Element* added = pane->Adopt(std::move(button));
                if (icons && i < page.rowIcons.size() && !page.rowIcons[i].empty())
                    if (Element* visual = added->Visual())
                    {
                        if (Element* label = visual->Find("text_Label"))
                            label->SetPosition(56, -1);
                        if (Element* line = visual->Find("Bottom"))
                            line->SetPosition(0, rowHeight - 1); // the separator under the taller row
                        auto image = Element::CreateControl(*s.assets, "XuiImage", "", 32, 32);
                        image->SetPosition(12, 7);
                        Value mode;
                        mode.type = Type::kInteger;
                        mode.i = 4;
                        image->Set("SizeMode", mode);
                        image->image = IconImage(*s.assets, page.rowIcons[i]);
                        image->Set("ColorFactor", color(rows[i].enabled ? 0xFF1E2328 : 0xFFA4AAAE));
                        visual->Adopt(std::move(image));
                    }
                s.buttons.push_back(added);
            }
            if (!page.rowDescriptions.empty())
            {
                // The focused row's description under the card (x 443, y 176, white as the scene's texts).
                Element* description = text(443, 176, 251, 220, u"", 0xFFEBEBEB, 0x00000000, 0x0000);
                const std::vector<std::u16string> descriptions = page.rowDescriptions;
                s.buttonFocus = [description, descriptions](State& s, uint32_t focus) {
                    description->SetString("Text", Utf8(focus < descriptions.size() ? descriptions[focus] : u""));
                    s.dirty = true;
                };
                s.buttonFocus(s, std::min<uint32_t>(focus, uint32_t(rows.empty() ? 0 : rows.size() - 1)));
            }
            ProfileFocus(s, focus, true);
            if (!page.legendB.empty() || !page.legendX.empty())
            {
                const std::u16string legend[4] = { u"Select", page.legendB.empty() ? u"Back" : page.legendB,
                                                   page.legendX, u"" };
                SetLegend(s, legend);
            }

            // The right panel's text (x 443, 251 wide, as the scene's onlineTextGroup): "To:" and the gamertags at
            // its top without a card; the envelope, the text's title and the text under the card (y 176) or "To".
            const float panelX = 443, panelW = 251;
            if (!page.card || !page.to.empty())
            {
                const float y = page.card ? 176 : 72 + shift;
                text(panelX, y, panelW, 24, u"To:", 0xFFEBEBEB, 0x00000000, 0x1010);
                text(panelX, y + 22, panelW, 24, page.to, 0xFFEBEBEB, 0x00000000, 0x5010);
            }
            if (!page.textTitle.empty() || !page.text.empty())
            {
                const float y = page.card ? 176 : 176 + shift;
                auto mail = Element::CreateControl(*s.assets, "XuiImage", "", 22, 16);
                mail->SetPosition(panelX, y + 4);
                Value mode;
                mode.type = Type::kInteger;
                mode.i = 4;
                mail->Set("SizeMode", mode);
                mail->image = s.assets->LoadImage("ico_32x_Mail.png", "xam.shrdres");
                pane->Adopt(std::move(mail));
                text(panelX + 28, y, panelW - 28, 24, page.textTitle, 0xFFEBEBEB, 0x00000000, 0x1010);
                text(panelX, y + 24, panelW, 400 - y - 24 - 12, page.text, 0xFFEBEBEB, 0x00000000, 0x0000);
            }
            if (!page.card)
            {
                for (const char* id : { "gamerCard", "GamerCardBackground" })
                    SetShow(pane->Find(id), false);
                s.dirty = true;
                return;
            }
            const bool extended = panel == ProfilePanel::kExtended && page.textTitle.empty() && page.text.empty() &&
                                  page.rowDescriptions.empty();
            if (!FillGamerCard(s, pane->Find("gamerCard"), card, extended))
                return;
            if (!extended && page.rowDescriptions.empty())
                if (Element* text = pane->Find("onlineTextGroup"))
                {
                    SetShow(text, true);
                    const Vec3 p = text->GetVector("Position", {});
                    text->SetPosition(p.x, p.y + shift);
                    SetText(text->Find("labMotto"), card.motto);
                    SetText(text->Find("labName"), card.name);
                    SetText(text->Find("labLocation"), card.location);
                    SetText(text->Find("labBio"), card.bio);
                }
            s.dirty = true;
        });
    }

    namespace
    {
        // The gamercard (an XuiGamerCard `host` hosts xam.gamercrd's gamercard.xur): online / offline base card, and
        // the extended panel when `extended` (its Online / Offline frames place the lines). Null: no gamercard.xur.
        Element* FillGamerCard(State& s, Element* host, const GamerCardInfo& card, bool extended)
        {
            Element* gc = LoadInto(s, host, "xam.gamercrd", "gamercard.xur");
            if (!gc)
                return nullptr;
            Element* base = gc->Find("onlineBaseScene");
            SetShow(gc->Find("offlineBaseScene"), false);
            if (base)
            {
                SetText(base->Find("gamerTagText"), card.gamertag);
                SetText(base->Find("credText"), Utf16(std::to_string(card.gamerscore)) + u" " + kGamerscoreGlyph);
                // LiveProfile.xus: #18 Recreation, #16 Pro, #14 Family, #20 Underground, #22 None.
                static constexpr size_t kZones[] = { 22, 18, 16, 14, 20 };
                static const char16_t* const kZoneNames[] = { u"None", u"Recreation", u"Pro", u"Family", u"Underground" };
                const size_t zone = card.zone < 5 ? card.zone : 0;
                const auto* profileStrings = s.assets->Strings(Base17559(s, "xam.shrdres"), "LiveProfile.xus");
                SetText(base->Find("zoneText"), profileStrings && kZones[zone] < profileStrings->size()
                                                    ? Utf16((*profileStrings)[kZones[zone]])
                                                    : std::u16string(kZoneNames[zone]));
                if (Element* rep = base->Find("reputationScene"))
                    for (int i = 1; i <= 5; i++)
                        SetShow(rep->Find("Star_full" + std::to_string(i)), card.reputation >= float(i) - 0.125f);
                // Tenure (years on Xbox LIVE): the orange "Gold" header with the big number on the right, as the
                // console showed a member's gamercard. The number image is our server's content (tenure/crown<N>.png).
                const bool gold = card.tenure > 0 && !card.tenureBadge.empty();
                if (Element* tier = base->Find("membershipTierScene"))
                {
                    SetShow(tier->Find("graphic_gold1"), gold);
                    SetShow(tier->Find("graphic_silver1"), !gold);
                }
                if (Element* level = base->Find("TenureLevel"))
                {
                    SetShow(level, gold);
                    if (gold)
                    {
                        auto image = std::make_unique<Image>();
                        std::string error;
                        if (DecodePng(card.tenureBadge.data(), card.tenureBadge.size(), *image, error))
                        {
                            level->image = image.get();
                            s.images.push_back(std::move(image));
                        }
                        else
                            Log("System UI: tenure number image: {}", error);
                    }
                }
                if (Element* tile = base->Find("gamerTileImage"); tile && !card.picture.empty())
                {
                    auto image = std::make_unique<Image>();
                    std::string error;
                    if (DecodePng(card.picture.data(), card.picture.size(), *image, error))
                    {
                        tile->image = image.get();
                        s.images.push_back(std::move(image));
                    }
                    else
                        Log("System UI: gamercard picture: {}", error);
                }
            }
            if (Element* ext = gc->Find("extendedScene"))
            {
                SetShow(ext, extended);
                // The Online layout (status at y 192), also for an offline card: the owner's 7_48_27 has "Offline for
                // 9 hours" there (720p rows 574-591, 12 above the pane's edge). The Offline frame's y 201 puts the
                // status past the Profile pane's edge, which clips the enlarged card (owner's report: cut off).
                const char* frame = "Online";
                ext->PlayNamed(frame, frame, true);
                SetText(ext->Find("topText"), card.motto);
                SetText(ext->Find("titleText"), card.title);
                SetText(ext->Find("richPresenceText"), card.online ? card.presence : std::u16string());
                SetText(ext->Find("bottomText"), GamerCardStatus(s, card));
                // No title icon or achievement pictures from our Xbox LIVE yet.
                for (const char* id : { "gameImage", "achievement1Image", "achievement2Image", "achievement3Image",
                                        "achievement4Image", "consoleTypeImage" })
                    SetShow(ext->Find(id), false);
            }
            return gc;
        }
    }

    void ShowGamerZone(const GamerCardInfo& card, uint32_t focus)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!Ready(s) || !EnsureHud(s))
            return;
        ShowWhenClosed(s, [=](State& s) {
            Element* pane = OpenPane(s, "gamerprofile.gp", "SelectGamerzone.xur");
            if (!pane)
                return;
            s.pane = Pane::kGamerZone;
            s.images.clear();
            FillGamerCard(s, pane->Find("gamerCard"), card, false);
            // LiveProfile.xus: the zone's name, its description after it.
            struct Zone { size_t name; const char16_t* fallback; const char* icon; };
            static constexpr Zone kZones[4] = { { 18, u"Recreation", "sharedres://ico_32x_zone_RR.png" },
                                                { 16, u"Pro", "sharedres://ico_32x_zone_Pro.png" },
                                                { 14, u"Family", "sharedres://ico_32x_zone_Family.png" },
                                                { 20, u"Underground", "sharedres://ico_32x_zone_Underground.png" } };
            const auto* strings = s.assets->Strings(Base17559(s, "xam.shrdres"), "LiveProfile.xus");
            auto text = [strings](size_t i, const char16_t* fallback) {
                return strings && i < strings->size() ? Utf16((*strings)[i]) : std::u16string(fallback);
            };
            std::vector<std::u16string> names, descriptions;
            std::vector<const Image*> icons;
            for (const Zone& z : kZones)
            {
                names.push_back(text(z.name, z.fallback));
                descriptions.push_back(text(z.name + 1, u""));
                icons.push_back(s.assets->LoadImage(z.icon, "xam.shrdres"));
            }
            Element* meta = pane->Find("metaPanelScene");
            Element* description = meta ? meta->Find("Text1") : nullptr;
            ListBuild(s, pane->Find("zoneList"), 4, int(std::min<uint32_t>(focus, 3)),
                      [=](Element* item, int index) {
                          item->text = names[size_t(index)];
                          item->image = icons[size_t(index)]; // btn_standard's image_presenter3
                      },
                      [=](int index) { SetText(description, descriptions[size_t(index)]); });
            s.dirty = true;
        });
    }

    void ShowGamerZoneInfo()
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!Ready(s) || !EnsureHud(s))
            return;
        ShowWhenClosed(s, [](State& s) {
            if (OpenPane(s, "gamerprofile.gp", "GamerZoneInfo.xur"))
                s.pane = Pane::kProfile; // no rows: B / BACK through PanePress
        });
    }

    void ShowAchievements(const std::u16string& titleName, const std::vector<AchievementEntry>& achievements)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!Ready(s) || !EnsureHud(s))
            return;
        ShowWhenClosed(s, [=](State& s) {
            Element* pane = OpenPane(s, "gamerprofile.gp", "802_Achievements.xur");
            if (!pane)
                return;
            s.pane = Pane::kAchievements;
            s.images.clear();
            // XAM's shared strings (LiveAll.xus): #0 "Secret", #1 the secret achievement's description, #4 "%u of %u
            // unlocked"; images: the SPA tile when earned, unearnedAchievement.png / secretAchievement.png otherwise.
            const auto* live = s.assets->Strings(Base17559(s, "xam.shrdres"), "LiveAll.xus");
            auto liveText = [&](size_t i, const char16_t* fallback) {
                return live && i < live->size() ? Utf16((*live)[i]) : std::u16string(fallback);
            };
            const Image* unearned = s.assets->LoadImage("sharedres://unearnedAchievement.png", "xam.shrdres");
            const Image* secret = s.assets->LoadImage("sharedres://secretAchievement.png", "xam.shrdres");
            std::vector<const Image*> tiles;
            uint32_t earned = 0;
            for (const AchievementEntry& a : achievements)
            {
                const Image* tile = a.showUnachieved ? unearned : secret;
                if (a.earned)
                {
                    earned++;
                    auto image = std::make_unique<Image>();
                    std::string error;
                    if (!a.image.empty() && DecodePng(a.image.data(), a.image.size(), *image, error))
                    {
                        tile = image.get();
                        s.images.push_back(std::move(image));
                    }
                }
                tiles.push_back(tile);
            }
            SetText(pane->Find("labGameTitle"), titleName);
            SetShow(pane->Find("HeaderShader"), true);
            SetShow(pane->Find("ctlLoading"), false);
            SetShow(pane->Find("labEmpty"), achievements.empty());
            Element* list = pane->Find("AchievementList");
            SetShow(list, !achievements.empty());
            std::u16string count = liveText(4, u"%u of %u unlocked");
            for (const uint32_t n : { earned, uint32_t(achievements.size()) })
                if (const size_t at = count.find(u"%u"); at != std::u16string::npos)
                    count.replace(at, 2, Utf16(std::to_string(n)));
            SetText(pane->Find("labCountText"), count);
            if (achievements.empty() || !list)
                return;
            const std::u16string secretName = liveText(0, u"Secret"), secretText = liveText(1, u"");
            Element* name = pane->Find("labAchievementName");
            Element* description = pane->Find("labAchievementDescription");
            Element* points = pane->Find("labPoints");
            Element* date = pane->Find("labAcquiredDate");
            ListBuild(s, list, int(achievements.size()), 0,
                [tiles](Element* item, int index) { item->dataImage[1] = tiles[size_t(index)]; },
                [=](int index) {
                    const AchievementEntry& a = achievements[size_t(index)];
                    const bool hidden = !a.earned && !a.showUnachieved;
                    SetText(name, hidden ? secretName : a.label);
                    SetText(description, a.earned ? a.description : hidden ? secretText : a.unachieved);
                    SetText(points, Utf16(std::to_string(a.gamerscore)) + u" " + kGamerscoreGlyph);
                    std::u16string when;
                    if (a.earned && a.unlockTime)
                    {
                        // The local date the achievement was unlocked, M/D/YYYY (the console's US format).
                        const std::time_t t = std::time_t((a.unlockTime - 116444736000000000ull) / 10000000ull);
                        std::tm local{};
                        localtime_s(&local, &t);
                        when = Utf16(std::format("{}/{}/{}", local.tm_mon + 1, local.tm_mday, local.tm_year + 1900));
                    }
                    SetText(date, when);
                });
        });
    }

    void ShowCompareGames(const std::vector<uint8_t>& theirPicture, uint32_t theirGamerscore,
                          const std::vector<uint8_t>& myPicture, uint32_t myGamerscore,
                          const std::vector<CompareGameRow>& rows, uint32_t focus)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!Ready(s) || !EnsureHud(s))
            return;
        ShowWhenClosed(s, [=](State& s) {
            Element* pane = OpenPane(s, "gamerprofile.gp", "825_GameShowcaseCompare.xur");
            if (!pane)
                return;
            s.pane = Pane::kCommunity; // a plain list: up / down, A, B (PanePress)
            s.images.clear();
            auto decode = [&](const std::vector<uint8_t>& png) -> const Image* {
                if (png.empty())
                    return nullptr;
                auto image = std::make_unique<Image>();
                std::string error;
                if (!DecodePng(png.data(), png.size(), *image, error))
                    return nullptr;
                const Image* p = image.get();
                s.images.push_back(std::move(image));
                return p;
            };
            if (Element* tile = pane->Find("youTileImage"))
                tile->image = decode(theirPicture);
            if (Element* tile = pane->Find("meTileImage"))
                tile->image = decode(myPicture);
            SetText(pane->Find("youCredText"), Utf16(std::to_string(theirGamerscore)));
            SetText(pane->Find("meCredText"), Utf16(std::to_string(myGamerscore)));
            SetShow(pane->Find("waitCursorControl"), false);
            SetShow(pane->Find("noGamesText"), rows.empty());
            Element* list = pane->Find("titleList");
            SetShow(list, !rows.empty());
            if (rows.empty() || !list)
                return;
            // btn_2Line_ico_Right2Strings: the item's text (the title), slot 1 the other player's gamerscore (txt_line2,
            // the dimmed gamerscore icon beside it), slot 2 yours (txt_line3), slot 3 the tile (image_presenter0).
            std::vector<const Image*> tiles;
            for (const CompareGameRow& r : rows)
                tiles.push_back(decode(r.tile));
            ListBuild(s, list, int(rows.size()), int(std::min<uint32_t>(focus, uint32_t(rows.size() - 1))),
                [rows, tiles](Element* item, int index) {
                    const CompareGameRow& r = rows[size_t(index)];
                    item->text = r.name;
                    item->dataText[1] = r.theirs;
                    item->dataText[2] = r.mine;
                    item->dataImage[3] = tiles[size_t(index)];
                },
                [](int) {});
        });
    }

    void ShowCompareAchievements(const std::u16string& titleName, const std::vector<uint8_t>& theirPicture,
                                 uint32_t theirGamerscore, const std::vector<uint8_t>& myPicture, uint32_t myGamerscore,
                                 const std::vector<CompareAchievementRow>& rows, uint32_t focus)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!Ready(s) || !EnsureHud(s))
            return;
        ShowWhenClosed(s, [=](State& s) {
            Element* pane = OpenPane(s, "gamerprofile.gp", "826-7_GameAchievStatsYou.xur");
            if (!pane)
                return;
            s.pane = Pane::kCommunity; // a plain list: up / down, A, B (PanePress)
            s.images.clear();
            auto decode = [&](const std::vector<uint8_t>& png) -> const Image* {
                if (png.empty())
                    return nullptr;
                auto image = std::make_unique<Image>();
                std::string error;
                if (!DecodePng(png.data(), png.size(), *image, error))
                    return nullptr;
                const Image* p = image.get();
                s.images.push_back(std::move(image));
                return p;
            };
            SetText(pane->Find("headerText"), titleName);
            if (Element* tile = pane->Find("youTileImage"))
                tile->image = decode(theirPicture);
            if (Element* tile = pane->Find("meTileImage"))
                tile->image = decode(myPicture);
            SetText(pane->Find("youCredText"), Utf16(std::to_string(theirGamerscore)));
            SetText(pane->Find("meCredText"), Utf16(std::to_string(myGamerscore)));
            SetShow(pane->Find("waitCursorControl"), false);
            bool any = false;
            for (const CompareAchievementRow& r : rows)
                any = any || r.theirs || r.mine;
            SetShow(pane->Find("noAchievementsText"), !any);
            Element* list = pane->Find("achievementList");
            SetShow(list, any);
            if (!any || !list)
                return;
            // btn_2Line_ico_Right2Icons: the item's text (the name), slot 1 the other player's gamerscore and slot 4
            // their image (image_lg1), slot 2 yours and slot 3 your image (image_lg). XAM's shared strings
            // (LiveAll.xus) #0 "Secret" for a secret achievement neither has.
            const Image* unearned = s.assets->LoadImage("sharedres://unearnedAchievement.png", "xam.shrdres");
            const Image* secret = s.assets->LoadImage("sharedres://secretAchievement.png", "xam.shrdres");
            const auto* live = s.assets->Strings(Base17559(s, "xam.shrdres"), "LiveAll.xus");
            const std::u16string secretName = live && !live->empty() ? Utf16((*live)[0]) : std::u16string(u"Secret");
            struct Shown
            {
                std::u16string name;
                const Image *theirs, *mine;
            };
            std::vector<Shown> shown;
            for (const CompareAchievementRow& r : rows)
            {
                const Image* tile = decode(r.image);
                const Image* locked = r.shown ? unearned : secret;
                const bool hidden = !r.shown && !r.theirs && !r.mine;
                shown.push_back({ hidden ? secretName : r.name, r.theirs && tile ? tile : locked,
                                  r.mine && tile ? tile : locked });
            }
            ListBuild(s, list, int(rows.size()), int(std::min<uint32_t>(focus, uint32_t(rows.size() - 1))),
                [rows, shown](Element* item, int index) {
                    const CompareAchievementRow& r = rows[size_t(index)];
                    const Shown& v = shown[size_t(index)];
                    item->text = v.name;
                    // Not earned: "--" (owner's capture 3_14_20).
                    item->dataText[1] = r.theirs ? Utf16(std::to_string(r.gamerscore)) : u"--";
                    item->dataText[2] = r.mine ? Utf16(std::to_string(r.gamerscore)) : u"--";
                    item->dataImage[4] = v.theirs;
                    item->dataImage[3] = v.mine;
                },
                [](int) {});
        });
    }

    void ShowAchievementDetails(const std::u16string& titleName, const std::u16string& name,
                                const std::u16string& description, uint32_t gamerscore,
                                const std::vector<uint8_t>& image, bool earned, bool shown)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!Ready(s) || !EnsureHud(s))
            return;
        ShowWhenClosed(s, [=](State& s) {
            Element* pane = OpenPane(s, "gamerprofile.gp", "828_AchievDetails.xur");
            if (!pane)
                return;
            s.pane = Pane::kDrill; // no rows: B / BACK (PanePress)
            s.buttons.clear();
            s.images.clear();
            const Image* tile = s.assets->LoadImage(shown ? "sharedres://unearnedAchievement.png" :
                                                    "sharedres://secretAchievement.png", "xam.shrdres");
            if (earned && !image.empty())
            {
                auto decoded = std::make_unique<Image>();
                std::string error;
                if (DecodePng(image.data(), image.size(), *decoded, error))
                {
                    tile = decoded.get();
                    s.images.push_back(std::move(decoded));
                }
            }
            SetText(pane->Find("headerText"), u""); // no heading (owner's capture 3_14_25)
            SetText(pane->Find("achievementTitleText"), name);
            SetText(pane->Find("achievementDescriptionText"), description);
            SetText(pane->Find("credText"), Utf16(std::to_string(gamerscore)) + u" " + kGamerscoreGlyph);
            if (Element* e = pane->Find("achievementImage"))
                e->image = tile;
        });
    }

    // ---- The Guide's drill-down scenes from Guide.Friends.xex ----

    namespace
    {
        // A scene of the module's package: false (nothing shown) when it is not there.
        bool FriendsSceneReady(State& s, std::string_view entry)
        {
            return Ready(s) && EnsureHud(s) && s.assets->LoadScene(kFriendsPackage, entry);
        }

        // PartySettings' spinners (SpinValueList, btn_spinner_party): one item with the value, the visual's wrap
        // arrows shown while the list has the focus (the scroll ends start hidden; the list class shows them).
        Element* SpinnerItem(State& s, Element* list, const std::u16string& value)
        {
            if (!list)
                return nullptr;
            while (!list->Children().empty())
                list->Release(list->Children().back().get());
            if (Element* visual = list->Visual())
                for (const auto& c : visual->Children())
                    if (c->ClassName() == "XuiListItem")
                        SetShow(c.get(), false); // the template
            auto item = Element::CreateControl(*s.assets, "XuiListItem", "btn_itemSpinnerParty", list->Width(),
                                               list->Height());
            item->SetPosition(0, 0);
            item->text = value;
            Element* added = list->Adopt(std::move(item));
            // The wrap arrows are drawn over the item (the capture's light < > on the green bar): moved from the
            // visual (drawn before the list's children) to after the item, same place (the visual fills the list).
            if (Element* visual = list->Visual())
                for (const char* id : { "ScrollLeft", "ScrollRight" })
                    if (Element* arrow = visual->Find(id))
                        list->Adopt(visual->Release(arrow));
            return added;
        }

        // The focus over a drill-down's controls (s.buttons), as ProfileFocus, plus the spinner arrows.
        void DrillFocus(State& s, uint32_t index, bool initial)
        {
            ProfileFocus(s, index, initial);
            if (s.buttonFocus)
                s.buttonFocus(s, s.focus);
            for (uint32_t i = 0; i < s.buttons.size(); i++)
            {
                Element* b = s.buttons[i];
                Element* list = b->Parent();
                if (!list || list->ClassName() != "XuiList")
                    continue;
                for (const char* id : { "ScrollLeft", "ScrollRight" })
                    if (Element* arrow = list->Find(id))
                    {
                        SetShow(arrow, i == s.focus);
                        arrow->PlayVisualState("ScrollMore");
                    }
            }
        }
    }

    bool ShowPartySettings(const std::u16string& chatChannel, const std::u16string& partyStatus, bool leader,
                           uint32_t focus)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!FriendsSceneReady(s, "PartySettings.xur"))
            return false;
        ShowWhenClosed(s, [=](State& s) {
            Element* pane = OpenPane(s, kFriendsPackage, "PartySettings.xur");
            if (!pane)
                return;
            s.pane = State::Pane::kDrill;
            // FUN_90124328: the leader's Party Status spinner, else btnNonLeader with the status (119 / 118).
            Element* voice = SpinnerItem(s, pane->Find("btnVoiceValue"), chatChannel);
            Element* status = nullptr;
            SetShow(pane->Find("btnPartyValue"), leader);
            SetShow(pane->Find("btnNonLeader"), !leader);
            if (leader)
                status = SpinnerItem(s, pane->Find("btnPartyValue"), partyStatus);
            else if ((status = pane->Find("btnNonLeader")))
                status->text = partyStatus;
            // The 17559 console shows no Quickplay here: Leave Party takes its place (owner's "party settings
            // party chat.png": Chat Channel, Party Status, Leave Party).
            Element* quick = pane->Find("btnQuickLaunch");
            Element* quit = pane->Find("btnQuitParty");
            if (quick && quit)
            {
                quit->SetVector("Position", quick->GetVector("Position", {}));
                SetShow(quick, false);
            }
            s.buttons.clear();
            for (Element* e : { voice, status, quit })
                if (e)
                    s.buttons.push_back(e);
            if (s.buttons.size() != 3)
                Log("System UI: PartySettings.xur: {} of its 3 controls found", s.buttons.size());
            const std::u16string legend[4] = { u"Select", u"Back", u"", u"" }; // X / Y cleared (FUN_90124328)
            SetLegend(s, legend);
            DrillFocus(s, focus, true);
        });
        return true;
    }

    bool ShowMessageCreate(const std::vector<MessageTypeEntry>& entries, uint32_t focus)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!FriendsSceneReady(s, "MessageType.xur"))
            return false;
        ShowWhenClosed(s, [=](State& s) {
            Element* pane = OpenPane(s, kFriendsPackage, "MessageType.xur");
            if (!pane)
                return;
            s.pane = State::Pane::kCommunity; // a list pane: PanePress / PaneFocus as the Community's
            Element* list = pane->Find("CommandList");
            Element* description = pane->Find("MsgTypeDesc");
            if (!list)
                return;
            // XuiCommonList: its ItemsText / ItemsImage lines are the rows (Message, Private Chat Invite, Party
            // Invite, Game Invite, Friend Request; images relative to the package).
            auto lines = [](std::string text) {
                std::vector<std::string> out;
                size_t at = 0;
                while (at <= text.size())
                {
                    size_t end = text.find('\n', at);
                    if (end == std::string::npos)
                        end = text.size();
                    std::string line = text.substr(at, end - at);
                    while (!line.empty() && (line.back() == '\r' || line.back() == ' '))
                        line.pop_back();
                    while (!line.empty() && line.front() == ' ')
                        line.erase(0, 1);
                    if (!line.empty())
                        out.push_back(line);
                    at = end + 1;
                }
                return out;
            };
            const auto texts = lines(list->GetString("ItemsText"));
            const auto images = lines(list->GetString("ItemsImage"));
            const int count = int(std::min(texts.size(), entries.size()));
            ListBuild(
                s, list, count, int(focus),
                [=](Element* item, int index) {
                    item->text = Utf16(texts[size_t(index)]);
                    item->imagePath = size_t(index) < images.size() ? images[size_t(index)] : std::string();
                    item->imageBase = kFriendsPackage;
                    const bool enabled = entries[size_t(index)].enabled;
                    if (item->enabled != enabled)
                    {
                        item->enabled = enabled;
                        item->PlayVisualState(item->focused ? (enabled ? "Focus" : "FocusDisable")
                                                            : (enabled ? "Normal" : "NormalDisable"));
                    }
                },
                [=](int index) {
                    if (index >= 0 && index < count)
                        SetText(description, entries[size_t(index)].description);
                    const std::u16string legend[4] = { u"Select", u"Back", u"", u"" };
                    SetLegend(S(), legend);
                });
        });
        return true;
    }

    bool ShowFriendsOf(const std::u16string& heading, const std::vector<CommunityRow>& rows, uint32_t focus)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!FriendsSceneReady(s, "FofScene.xur"))
            return false;
        ShowWhenClosed(s, [=](State& s) {
            Element* pane = OpenPane(s, kFriendsPackage, "FofScene.xur");
            if (!pane)
                return;
            s.pane = State::Pane::kCommunity; // a list pane
            SetText(pane->Find("txtHeading"), heading);
            SetShow(pane->Find("txtEmptyList"), rows.empty());
            Element* list = pane->Find("listItems");
            Element* total = pane->Find("label_Total");
            if (!list)
                return;
            s.images.clear();
            std::vector<const Image*> pictures(rows.size());
            for (size_t i = 0; i < rows.size(); i++)
            {
                if (rows[i].picture.empty())
                    continue;
                auto image = std::make_unique<Image>();
                std::string error;
                if (DecodePng(rows[i].picture.data(), rows[i].picture.size(), *image, error))
                {
                    pictures[i] = image.get();
                    s.images.push_back(std::move(image));
                }
            }
            Assets* assets = &*s.assets;
            const auto* strings = assets->Strings(kFriendsPackage, "strings.xus");
            const std::u16string ofFormat =
                strings && strings->size() > 144 ? Utf16((*strings)[144]) : std::u16string(u"%1!d! of %2!d!");
            const int count = int(rows.size());
            ListBuild(
                s, list, count, int(focus),
                [=](Element* item, int index) {
                    const CommunityRow& row = rows[size_t(index)];
                    item->text.clear();
                    for (auto& t : item->dataText)
                        t.clear();
                    for (auto& i : item->dataImage)
                        i = nullptr;
                    for (const auto& [slot, text] : row.slotText)
                        if (slot == 0)
                            item->text = text;
                        else if (slot > 0 && slot < Element::kDataSlots)
                            item->dataText[slot] = text;
                    for (const auto& [slot, path] : row.slotImage)
                        if (slot > 0 && slot < Element::kDataSlots)
                            item->dataImage[slot] = assets->LoadImage(path, kFriendsPackage);
                    if (row.pictureSlot > 0 && row.pictureSlot < Element::kDataSlots)
                        item->dataImage[row.pictureSlot] = pictures[size_t(index)];
                },
                [=](int index) {
                    std::u16string text = ofFormat;
                    for (const auto& [key, value] : { std::pair{ std::u16string(u"%1!d!"), index + 1 },
                                                      std::pair{ std::u16string(u"%2!d!"), count } })
                        if (const size_t at = text.find(key); at != std::u16string::npos)
                            text.replace(at, key.size(), Utf16(std::to_string(value)));
                    SetText(total, count > 0 ? text : std::u16string());
                    const std::u16string legend[4] = { u"Select", u"Back", u"", u"" };
                    SetLegend(S(), legend);
                });
        });
        return true;
    }

    bool ShowMessageRecipients(const std::vector<RecipientEntry>& entries, uint32_t focus)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!FriendsSceneReady(s, "MessageRecipients.xur"))
            return false;
        ShowWhenClosed(s, [=](State& s) {
            Element* pane = OpenPane(s, kFriendsPackage, "MessageRecipients.xur");
            if (!pane)
                return;
            s.pane = State::Pane::kDrill;
            std::vector<ProfileRow> rows;
            for (const RecipientEntry& e : entries)
                rows.push_back({ e.gamertag, true });
            std::vector<Element*> list = ListRows(s, pane->Find("Recipients"), rows);
            s.buttons.clear();
            if (Element* b = pane->Find("btnGamerTag"))
                s.buttons.push_back(b);
            const size_t offset = s.buttons.size();
            s.buttons.insert(s.buttons.end(), list.begin(), list.end());
            Element* help = pane->Find("HelpText");
            Element* host = pane->Find("GamerCard");
            SetText(pane->Find("DescText"), FriendsStringLocked(s, 149, u""));
            const std::u16string legend[4] = { u"Select", u"Back", u"", FriendsStringLocked(s, 171, u"Enter Gamertag") };
            SetLegend(s, legend);
            // FUN_90122968: the focused friend's gamercard (extended), the help text on Enter Gamertag or without
            // friends; the check marks (XuiCheckbox's *Check states) on the chosen ones.
            s.buttonFocus = [entries, help, host, offset, list](State& s, uint32_t index) {
                const bool onRow = index >= offset && index - offset < entries.size();
                SetShow(help, !onRow);
                if (host)
                {
                    while (!host->Children().empty())
                        host->Release(host->Children().back().get());
                    if (onRow)
                        FillGamerCard(s, host, entries[index - offset].card, true);
                }
                for (size_t i = 0; i < list.size() && i < entries.size(); i++)
                    if (entries[i].checked)
                        list[i]->PlayVisualState(i + offset == index ? "FocusCheck" : "NormalCheck");
            };
            DrillFocus(s, focus, true);
        });
        return true;
    }

    bool ShowBio(const std::u16string& heading, const std::u16string& bio)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!FriendsSceneReady(s, "Bio.xur"))
            return false;
        ShowWhenClosed(s, [=](State& s) {
            Element* pane = OpenPane(s, kFriendsPackage, "Bio.xur");
            if (!pane)
                return;
            s.pane = State::Pane::kDrill;
            s.buttons.clear();
            SetText(pane->Find("txtHeading"), heading);
            SetText(pane->Find("txtBody"), bio);
            const std::u16string legend[4] = { u"Select", u"Back", u"", u"" }; // owner's 7_47_21
            SetLegend(s, legend);
        });
        return true;
    }

    std::u16string CreateProfileString(size_t index, const char16_t* fallback)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        const auto* table = Ready(s) ? s.assets->Strings(Base17559(s, "createprofile.cp"), "CreateProfile_Custom.xus") : nullptr;
        return table && index < table->size() ? Utf16((*table)[index]) : std::u16string(fallback);
    }

    void ShowProfileWait(const std::u16string& status)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!Ready(s) || !EnsureHud(s))
            return;
        ShowWhenClosed(s, [=](State& s) {
            // The scene stays while its text changes ("Creating profile", "Signing in", "Updating profile").
            if (s.pane == Pane::kWait && s.phase == Phase::kOpen && s.hosted)
            {
                SetText(s.hosted->Find("statusText"), status);
                s.dirty = true;
                return;
            }
            Element* pane = OpenPane(s, "createprofile.cp", "AddXboxLive.xur");
            if (!pane)
                return;
            s.pane = Pane::kWait;
            s.buttons.clear();
            SetText(pane->Find("statusText"), status);
            SetShow(pane->Find("backButton"), false);
            // The wait cursor (the skin's Loading_Small: its ring fades in and turns from its Normal state on).
            if (Element* cursor = pane->Find("waitCursorControl"))
                cursor->PlayVisualState("Normal");
            const std::u16string legend[4] = {};
            SetLegend(s, legend);
        });
    }

    namespace
    {
        // A QuickLaunch tab's contents (QLRecentlyPlayedTabScene / QLRecentlyDownloadedTabScene / QLAllGamesTabScene,
        // hud 0x913F6110 / 0x913F61C8 / 0x913F5770 / 0x913F5CA0): Tab1 without a signed-in profile shows labelSignIn;
        // no rows show labelNoGames; else the list (Tab1 at most 6 rows). The wait cursors are hidden once loaded;
        // text_PercentFull and ProgressAnimation are never used by hud. Legend: A "Launch" (hud 36) once the tab has
        // rows, B the scene's "Back", Y on All "Hide Trial Games" (35) / "Show Trial Games" (37) (0x913F73A8).
        void QuickLaunchFill(State& s)
        {
            auto& q = s.quick;
            static const char* const kLists[3] = { "gameList", "downloadList", "listItems" };
            Element* tab = q.tab[q.cur];
            const std::vector<QuickLaunchRow>& rows =
                q.cur == 0 ? q.info.played : q.cur == 1 ? q.info.downloads : q.info.all;
            const bool signIn = q.cur == 0 && !q.info.signedIn;
            q.shown.clear();
            for (size_t i = 0; i < rows.size() && !signIn; i++)
                if (!(q.cur == 2 && q.hideTrials && rows[i].trial) && !(q.cur == 0 && q.shown.size() >= 6))
                    q.shown.push_back(int(i));
            const bool empty = !signIn && q.shown.empty();
            SetShow(tab->Find("labelSignIn"), signIn);
            SetShow(tab->Find("labelNoGames"), empty);
            SetShow(tab->Find("waitCursorControl"), false);
            SetShow(tab->Find("ProgressAnimation"), false);
            SetText(tab->Find("text_PercentFull"), u"");
            Element* list = tab->Find(kLists[q.cur]);
            SetShow(list, !signIn && !empty);
            auto& owned = q.pictures[size_t(q.cur)];
            owned.clear();
            std::vector<const Image*> pictures(rows.size());
            for (int i : q.shown)
            {
                if (rows[size_t(i)].picture.empty())
                    continue;
                auto image = std::make_unique<Image>();
                std::string error;
                if (!DecodePng(rows[size_t(i)].picture.data(), rows[size_t(i)].picture.size(), *image, error))
                {
                    Log("System UI: Recent: row {}: {}", i, error);
                    continue;
                }
                pictures[size_t(i)] = image.get();
                owned.push_back(std::move(image));
            }
            const int tabIndex = q.cur;
            if (list)
            {
                while (!list->Children().empty())
                    list->Release(list->Children().back().get());
                ListBuild(s, list, int(q.shown.size()), 0, [=](Element* item, int index) {
                    const auto& qq = S().quick;
                    const std::vector<QuickLaunchRow>& r =
                        tabIndex == 0 ? qq.info.played : tabIndex == 1 ? qq.info.downloads : qq.info.all;
                    const int at = qq.shown[size_t(index)];
                    const QuickLaunchRow& row = r[size_t(at)];
                    item->text = row.name;
                    for (auto& t : item->dataText)
                        t.clear();
                    for (auto& i : item->dataImage)
                        i = nullptr;
                    item->image = nullptr;
                    if (tabIndex == 0)
                    {
                        item->dataText[2] = row.line2; // btn_twoline-icon_RightGS: txt_line2
                        item->dataText[1] = row.right; // txt_line3, the right column
                        item->dataImage[3] = pictures[size_t(at)];
                    }
                    else if (tabIndex == 1)
                    {
                        item->dataText[1] = row.right; // btn_2Line_ico_Right1String: txt_line2, the date
                        item->dataImage[3] = pictures[size_t(at)];
                    }
                    else
                        item->image = pictures[size_t(at)]; // btn_standard1: its image presenter has no association
                });
            }
            std::u16string legend[4] = { q.shown.empty() ? u"" : HudStringOf(s, 36, u"Launch"), u"Back", u"", u"" };
            if (q.cur == 2)
                legend[3] = q.hideTrials ? HudStringOf(s, 37, u"Show Trial Games") : HudStringOf(s, 35, u"Hide Trial Games");
            SetLegend(s, legend);
            s.dirty = true;
        }
    }

    void ShowQuickLaunch(const QuickLaunchInfo& info)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!Ready(s) || !EnsureHud(s))
            return;
        ShowWhenClosed(s, [=](State& s) {
            Element* pane = OpenPane(s, "hud.hud", "QuickLaunch.xur");
            if (!pane)
                return;
            s.pane = Pane::kQuickLaunch;
            s.buttons.clear();
            auto& q = s.quick;
            q.tabs = pane->Find("Tabscene");
            for (int i = 0; i < 3; i++)
                q.tab[i] = q.tabs ? q.tabs->Find(std::format("Tab{}", i + 1)) : nullptr;
            if (!q.tab[0] || !q.tab[1] || !q.tab[2])
            {
                Log("System UI: QuickLaunch.xur: its three tabs not found");
                return;
            }
            q.info = info;
            q.hideTrials = false;
            q.cur = 0;
            q.tabs->PlayNamed("2To1End", "2To1End", false); // Tab1's resting frame (XuiTabSceneElement::GotoTab)
            for (int i = 0; i < 3; i++)
                SetShow(q.tab[i], i == 0);
            QuickLaunchFill(s);
        });
    }

    QuickLaunchAction QuickLaunchPress(uint16_t pressed)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        constexpr uint16_t kUp = 0x0001, kDown = 0x0002, kLeft = 0x0004, kRight = 0x0008, kBack = 0x0020,
                           kLeftShoulder = 0x0100, kRightShoulder = 0x0200, kLeftTrigger = 0x0400,
                           kRightTrigger = 0x0800, kA = 0x1000, kB = 0x2000, kY = 0x8000;
        if (s.phase != Phase::kOpen || s.pane != State::Pane::kQuickLaunch || s.afterPress)
            return {};
        auto& q = s.quick;
        if (pressed & (kB | kBack))
            return { QuickLaunchAction::kCancelled };
        // XuiTabSceneElement (TU46 0x82A9C0C8 / 0x82A9C1E0): RB / RT next, LB / LT previous, "<from>To<to>" up to its
        // End; the old tab is hidden, the new one shown (TabTransition 0x82A9BF80). The lists only go up and down, so
        // the d-pad / left stick left and right reach the tab scene too (XuiTabSceneElement::ObjectProc, as on the
        // Guide's blades; owner, 2026-10-22).
        const int delta = (pressed & (kRightShoulder | kRightTrigger | kRight)) ? 1
                          : (pressed & (kLeftShoulder | kLeftTrigger | kLeft)) ? -1
                                                                                : 0;
        if (delta != 0)
        {
            const int next = q.cur + delta;
            if (next < 0 || next > 2)
                return {};
            const std::string name = std::format("{}To{}", q.cur + 1, next + 1);
            q.tabs->PlayNamed(name, name + "End", false);
            SetShow(q.tab[q.cur], false);
            SetShow(q.tab[next], true);
            q.cur = next;
            QuickLaunchFill(s);
            return {};
        }
        if ((pressed & kY) && q.cur == 2)
        {
            q.hideTrials = !q.hideTrials;
            QuickLaunchFill(s);
            return {};
        }
        if (pressed & kUp)
            ListMove(s, 0, -1);
        if (pressed & kDown)
            ListMove(s, 0, 1);
        if ((pressed & kA) && !q.shown.empty() && s.list.cur < int(q.shown.size()))
        {
            if (s.list.focused)
                PlayPress(s, s.list.focused);
            s.dirty = true;
            return { QuickLaunchAction::kLaunch, uint32_t(q.cur), uint32_t(q.shown[size_t(s.list.cur)]) };
        }
        return {};
    }

    void ShowSaveProfile(const std::u16string& gamertag, const std::vector<uint8_t>& picture, uint32_t focus)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!Ready(s) || !EnsureHud(s))
            return;
        ShowWhenClosed(s, [=](State& s) {
            Element* pane = OpenPane(s, "createprofile.cp", "Congratulations.xur");
            if (!pane)
                return;
            s.pane = Pane::kDrill;
            s.buttons.clear();
            for (const char* id : { "getLiveButton", "editProfileButton", "doneButton" })
                if (Element* b = pane->Find(id))
                    s.buttons.push_back(b);
            if (s.buttons.size() != 3)
                Log("System UI: Congratulations.xur: {} of its 3 buttons found", s.buttons.size());
            // The new profile's gamercard: its gamertag and picture, nothing earned yet.
            SetText(pane->Find("gamerTagText"), gamertag);
            for (const char* id : { "GSTextnumber", "titlesPlayednumber", "achievementsnumber" })
                SetText(pane->Find(id), u"0");
            s.images.clear();
            if (Element* tile = pane->Find("gamerTileImage"); tile && !picture.empty())
            {
                auto image = std::make_unique<Image>();
                std::string error;
                if (DecodePng(picture.data(), picture.size(), *image, error))
                {
                    tile->image = image.get();
                    s.images.push_back(std::move(image));
                }
                else
                    Log("System UI: Save Profile's gamer picture: {}", error);
            }
            // The text beside the buttons follows the focus: the scene's timelines between the three panels
            // ("GetLiveToEditProfile" ... "EndGetLiveToEditProfile"; Join Xbox Live and Done each border Customize
            // Profile). Opening shows the focused button's panel at rest.
            static const char* const kPanel[3] = { "GetLive", "EditProfile", "Done" };
            const std::string rest = focus == 1 ? "EndGetLiveToEditProfile" :
                                     std::format("EndEditProfileTo{}", kPanel[std::min<uint32_t>(focus, 2)]);
            pane->PlayNamed(rest, rest, true);
            auto shown = std::make_shared<uint32_t>(std::min<uint32_t>(focus, 2));
            s.buttonFocus = [pane, shown](State& s, uint32_t index) {
                if (index == *shown || index > 2)
                    return;
                // The focus moves one button at a time (up / down), so each move is one of the four timelines.
                const std::string name = std::format("{}To{}", kPanel[*shown], kPanel[index]);
                pane->PlayNamed(name, "End" + name, true);
                *shown = index;
                s.dirty = true;
            };
            DrillFocus(s, focus, true);
        });
    }

    void ShowJoinLive()
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!Ready(s) || !EnsureHud(s))
            return;
        ShowWhenClosed(s, [=](State& s) {
            Element* pane = OpenPane(s, "hud.hud", "InfoUpsellLive.xur");
            if (!pane)
                return;
            s.pane = Pane::kDrill;
            s.buttons.clear();
            if (Element* b = pane->Find("btnJoinLive"))
                s.buttons.push_back(b);
            const std::u16string legend[4] = { u"Select", u"Back", u"", u"" };
            SetLegend(s, legend);
            DrillFocus(s, 0, true);
        });
    }

    void ShowHudStatus(const std::u16string& text)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!Ready(s) || !EnsureHud(s))
            return;
        ShowWhenClosed(s, [=](State& s) {
            Element* pane = OpenPane(s, "hud.hud", "Status.xur");
            if (!pane)
                return;
            s.pane = Pane::kWait;
            s.buttons.clear();
            // StatusScene (hud 0x913F8590): txtMessage (visual txt_Starting) gets the text; its wait cursor
            // (XuiControl1, a Loading_Small) fades in and turns from its Normal state on, as in signin's status.xur.
            if (Element* message = pane->Find("txtMessage"))
            {
                SetText(message, text);
                if (message->Visual())
                    if (Element* cursor = message->Visual()->Find("XuiControl1"))
                        cursor->PlayVisualState("Normal");
            }
            const std::u16string legend[4] = {};
            SetLegend(s, legend);
        });
    }

    PaneAction PanePress(uint16_t pressed)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        PaneAction action;
        if (s.afterPress)
            return action;
        constexpr uint16_t kUp = 0x0001, kDown = 0x0002, kLeft = 0x0004, kRight = 0x0008, kBack = 0x0020,
                           kA = 0x1000, kB = 0x2000;
        if (s.phase == Phase::kOpen && (s.pane == State::Pane::kProfile || s.pane == State::Pane::kDrill))
        {
            // The Profile's rows (buttons): up / down, A on an enabled row, B / BACK.
            if (pressed & (kB | kBack))
                return { PaneAction::kCancelled, 0 };
            if ((pressed & kUp) && s.focus > 0)
                DrillFocus(s, s.focus - 1, false);
            if ((pressed & kDown) && s.focus + 1 < s.buttons.size())
                DrillFocus(s, s.focus + 1, false);
            if ((pressed & kA) && s.focus < s.buttons.size())
            {
                Element* b = s.buttons[s.focus];
                if (b->enabled)
                    PlayPress(s, b);
                else
                    b->PlayVisualState("PressDisable");
                s.dirty = true;
                if (b->enabled)
                    return { PaneAction::kChosen, s.focus };
            }
            return action;
        }
        if (s.phase == Phase::kOpen && s.pane == State::Pane::kCommunity && !s.community.buttons.empty())
        {
            // The real Party tab: its buttons, then the member list.
            auto& c = s.community;
            const int nb = int(c.buttons.size());
            if (pressed & (kB | kBack))
                return { PaneAction::kCancelled, 0 };
            if (pressed & kUp)
            {
                if (c.focus > 0 && (c.focus < nb || s.list.cur == 0))
                    CommunityFocus(s, c.focus - 1, false);
                else if (c.focus >= nb)
                {
                    ListMove(s, 0, -1);
                    c.focus = nb + s.list.cur;
                }
            }
            if (pressed & kDown)
            {
                if (c.focus < nb - 1 || (c.focus == nb - 1 && s.list.count > 0))
                    CommunityFocus(s, c.focus + 1, false);
                else if (c.focus >= nb)
                {
                    ListMove(s, 0, 1);
                    c.focus = nb + s.list.cur;
                }
            }
            if (pressed & kA)
            {
                if (c.focus < nb)
                {
                    Element* b = c.buttons[size_t(c.focus)];
                    if (b->enabled)
                        PlayPress(s, b);
                    else
                        b->PlayVisualState("PressDisable");
                    s.dirty = true;
                    if (b->enabled)
                        return { PaneAction::kChosen, uint32_t(c.focus) };
                }
                else if (s.list.count > 0)
                {
                    if (s.list.focused)
                        PlayPress(s, s.list.focused);
                    return { PaneAction::kChosen, uint32_t(nb + s.list.cur) };
                }
            }
            return action;
        }
        if (s.phase != Phase::kOpen || !s.list.list)
            return action;
        if (pressed & (kB | kBack))
            return { PaneAction::kCancelled, 0 };
        if (pressed & kUp)
            ListMove(s, 0, -1);
        if (pressed & kDown)
            ListMove(s, 0, 1);
        if (pressed & kLeft)
            ListMove(s, -1, 0);
        if (pressed & kRight)
            ListMove(s, 1, 0);
        if ((pressed & kA) && s.list.count > 0)
        {
            if (s.list.focused)
                PlayPress(s, s.list.focused);
            return { PaneAction::kChosen, uint32_t(s.list.cur) };
        }
        return action;
    }

    uint32_t PaneFocus()
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (s.pane == State::Pane::kCommunity && !s.community.buttons.empty())
            return uint32_t(s.community.focus);
        return s.list.list ? uint32_t(s.list.cur) : s.focus;
    }

    void SetFocus(uint32_t index)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (s.assets && s.phase == Phase::kOpen)
            Focus(s, index, false);
    }

    void Press(uint32_t index)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (s.assets && s.phase == Phase::kOpen && index < s.buttons.size())
            PlayPress(s, s.buttons[index]);
    }

    bool PopupShowing()
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        return s.phase == Phase::kOpen;
    }

    void ClosePopup(std::function<void()> closed)
    {
        State& s = S();
        RunDue due{ s };
        std::lock_guard lock(s.mutex);
        const bool animating = !s.pending && s.assets && s.phase == Phase::kOpen && s.hudScene;
        if (closed)
            (animating ? s.closed : s.due).push_back(std::move(closed));
        if (s.pending)
        {
            s.pending = nullptr; // closed before it was shown
            return;
        }
        s.afterPress = nullptr; // the close comes first
        s.afterLoad = nullptr;
        s.loadLeft = 0;
        if (!animating)
            return;
        if (s.pane == Pane::kGuide && s.guide.tabs && !s.guide.box)
            GuidePlay(s, "Open"); // the blades fade out with the HUD
        s.hudScene->PlayNamed(s.hudState + "ToClosed", "End" + s.hudState + "ToClosed", true);
        s.phase = Phase::kClosing;
        s.dirty = true;
    }

    void QueueNotification(uint32_t type, uint32_t user, const std::u16string& param)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        if (!Ready(s))
            return;
        s.toasts.push_back({ type, user, param });
        s.dirty = true;
    }

    void SetNotificationPosition(uint32_t position)
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        s.notifyPosition = position & 0xF;
        s.dirty = true;
    }

    namespace
    {
        // Runs draw(0) .. draw(count - 1) on a small pool of threads and the caller; returns when all are done.
        void DrawBands(int count, const std::function<void(int)>& draw)
        {
            struct Pool
            {
                std::mutex m;
                std::condition_variable start, finished;
                const std::function<void(int)>* job = nullptr;
                int next = 0, count = 0, done = 0;
                uint64_t generation = 0;

                // Takes and runs items of the current job (lock held on entry and exit).
                void Work(std::unique_lock<std::mutex>& lock)
                {
                    while (job && next < count)
                    {
                        const int item = next++;
                        const auto* f = job;
                        lock.unlock();
                        (*f)(item);
                        lock.lock();
                        if (++done == count)
                            finished.notify_all();
                    }
                }
            };
            static Pool* pool = [] {
                auto* p = new Pool;
                const unsigned threads = std::clamp(std::thread::hardware_concurrency(), 2u, 9u) - 1;
                for (unsigned i = 0; i < threads; i++)
                    std::thread([p] {
                        uint64_t seen = 0;
                        std::unique_lock lock(p->m);
                        for (;;)
                        {
                            p->start.wait(lock, [&] { return p->generation != seen; });
                            seen = p->generation;
                            p->Work(lock);
                        }
                    }).detach();
                return p;
            }();
            std::unique_lock lock(pool->m);
            pool->job = &draw;
            pool->next = 0;
            pool->count = count;
            pool->done = 0;
            pool->generation++;
            pool->start.notify_all();
            pool->Work(lock);
            pool->finished.wait(lock, [&] { return pool->done == pool->count; });
            pool->job = nullptr;
        }

        // Advances the HUD's animations, toasts and clock to `vblank` (caller holds the mutex).
        void Advance(State& s, uint64_t vblank)
        {
            // Emulated time since the previous call (XAM's HUD runs on its own clock, whatever the title presents).
            const float dt = s.lastVblank == 0 || vblank < s.lastVblank ? 0.0f : float(vblank - s.lastVblank) / 60.0f;
            s.lastVblank = vblank;

            if (s.full)
            {
                const bool wasPlaying = s.full->Playing();
                s.full->Advance(dt);
                if (wasPlaying || s.full->Playing())
                    s.dirty = true;
            }

            // Advance the animations.
            if (s.hud && s.phase != Phase::kClosed)
            {
                // Redrawn only when something changed: a timeline that was or is playing, the clock, the dim level,
                // the phase (navigation, text and popups mark the overlay dirty where they change it). A redraw is
                // ~9 ms on 8 threads at 720p; redrawing an unchanged Guide every frame held the dash to 30 frames/s.
                const bool wasPlaying = s.hud->Playing();
                const Phase phaseBefore = s.phase;
                const float dimBefore = s.dim;
                bool changed = false;
                s.hud->Advance(dt);
                if (s.pressLeft > 0 && (s.pressLeft -= dt) <= 0)
                {
                    s.pressLeft = 0;
                    auto show = std::move(s.afterPress);
                    s.afterPress = nullptr;
                    if (show && s.phase == Phase::kOpen)
                        show(s);
                }
                if (s.afterLoad && (s.loadLeft -= dt) <= 0)
                {
                    s.loadLeft = 0;
                    auto show = std::move(s.afterLoad);
                    s.afterLoad = nullptr;
                    if (s.phase == Phase::kOpen)
                        show(s);
                }
                if (s.phase == Phase::kClosing && !s.hudScene->TimelinePlaying())
                {
                    DropPopup(s);
                    s.phase = Phase::kClosed;
                    s.communityLeft = false;
                    s.pressLeft = 0;
                    s.dim = 0; // both layers go with the last frame of the closing
                    s.hudState.clear();
                    for (auto& f : s.closed)
                        s.due.push_back(std::move(f));
                    s.closed.clear();
                    if (auto show = std::move(s.pending))
                    {
                        s.pending = nullptr;
                        show(s);
                    }
                }
                if (s.guide.boxClosing && !s.hudScene->TimelinePlaying())
                {
                    if (s.guide.box && s.guide.boxHost)
                        s.guide.boxHost->Release(s.guide.box);
                    s.guide.box = nullptr;
                    s.guide.boxClosing = false;
                }
                if (s.phase == Phase::kOpen)
                {
                    // The clock (XAM refreshes it every second) and the keyboard's caret.
                    if (std::u16string clock = ClockText(s); clock != s.clockText)
                    {
                        SetText(s.hudScene->Find("DateTimeTextId"), clock);
                        s.clockText = std::move(clock);
                        changed = true;
                    }
                    KeyboardTick(s);
                }
                // The fading layer follows the open / close animations.
                const float target = s.phase == Phase::kOpen ? 1.0f : 0.0f;
                const float step = dt * 60.0f / s.dimLayers.frames;
                s.dim = s.dim < target ? std::min(target, s.dim + step) : std::max(target, s.dim - step);
                if (changed || wasPlaying || (s.hud && s.hud->Playing()) || s.phase != phaseBefore || s.dim != dimBefore)
                    s.dirty = true;
            }
            AdvanceToast(s, dt);
        }
    }

    void AdvanceOverlay(uint64_t vblank)
    {
        State& s = S();
        RunDue due{ s };
        std::lock_guard lock(s.mutex);
        if (s.assets)
            Advance(s, vblank);
    }

    bool ShowFullScreenScene(std::string_view xml, std::string& error)
    {
        State& s = S();
        RunDue due{ s };
        std::lock_guard lock(s.mutex);
        if (!Ready(s))
        {
            error = "the system UI assets are missing";
            return false;
        }
        std::vector<uint8_t> bytes;
        auto scene = std::make_unique<Scene>();
        if (!CompileScene(xml, bytes, error) || !scene->Load(bytes.data(), bytes.size(), error))
            return false;
        s.full.reset();
        s.fullLayers.clear();
        s.fullScene = std::move(scene);
        s.full = Element::Create(*s.assets, *s.fullScene, s.fullScene->Root(), "xam.shrdres", nullptr);
        s.dirty = true;
        return true;
    }

    bool AddSystemUiPackage(std::string name, std::vector<uint8_t> data, std::string& error, bool nxe)
    {
        State& s = S();
        RunDue due{ s };
        std::lock_guard lock(s.mutex);
        if (!Ready(s))
        {
            error = "the system UI assets are missing";
            return false;
        }
        return s.assets->AddPackage(std::move(name), std::move(data), error, nxe);
    }

    bool UseSystemUiPackages(const std::filesystem::path& dir, std::string& error)
    {
        State& s = S();
        RunDue due{ s };
        std::lock_guard lock(s.mutex);
        if (!Ready(s))
        {
            error = "the system UI assets are missing";
            return false;
        }
        if (!s.assets->ReplacePackages(dir, error))
            return false;
        Log("System UI: packages of {} in place of the base ones", dir.string());
        return true;
    }

    bool SetFullScreenSkins(const std::vector<std::pair<std::string, std::string>>& skins, std::string& error)
    {
        State& s = S();
        RunDue due{ s };
        std::lock_guard lock(s.mutex);
        if (!Ready(s))
        {
            error = "the system UI assets are missing";
            return false;
        }
        return s.assets->SetThemeSkins(skins, error);
    }

    void SetFullScreenSchemes(std::vector<std::pair<std::string, std::string>> schemes)
    {
        State& s = S();
        RunDue due{ s };
        std::lock_guard lock(s.mutex);
        if (Ready(s))
            s.assets->SetThemeSchemes(std::move(schemes));
    }

    bool ShowFullScreenScene(const std::string& package, const std::string& entry, std::string& error)
    {
        State& s = S();
        RunDue due{ s };
        std::lock_guard lock(s.mutex);
        if (!Ready(s))
        {
            error = "the system UI assets are missing";
            return false;
        }
        const Scene* scene = s.assets->LoadScene(package, entry);
        if (!scene)
        {
            error = std::format("scene {}/{} is missing or unreadable (log)", package, entry);
            return false;
        }
        s.full.reset();
        s.fullLayers.clear();
        s.fullScene.reset();
        s.full = Element::Create(*s.assets, *scene, scene->Root(), package, nullptr);
        s.dirty = true;
        return true;
    }

    void EditFullScreenScene(const std::function<void(Element& root)>& edit)
    {
        State& s = S();
        RunDue due{ s };
        std::lock_guard lock(s.mutex);
        if (!s.full)
            return;
        edit(*s.full);
        s.dirty = true;
    }

    bool AddFullScreenLayer(std::string_view xml, std::string& error)
    {
        State& s = S();
        RunDue due{ s };
        std::lock_guard lock(s.mutex);
        if (!s.full)
        {
            error = "no full-screen scene to add a layer to";
            return false;
        }
        std::vector<uint8_t> bytes;
        auto scene = std::make_unique<Scene>();
        if (!CompileScene(xml, bytes, error) || !scene->Load(bytes.data(), bytes.size(), error))
            return false;
        s.full->Adopt(Element::Create(*s.assets, *scene, scene->Root(), "xam.shrdres", s.full.get()));
        s.fullLayers.push_back(std::move(scene));
        s.dirty = true;
        return true;
    }

    void CloseFullScreenScene()
    {
        State& s = S();
        RunDue due{ s };
        std::lock_guard lock(s.mutex);
        s.full.reset();
        s.fullLayers.clear();
        s.fullScene.reset();
        s.dirty = true;
    }

    namespace
    {
        // Where the overlay's layers go on a target of width x height.
        struct OverlayLayout
        {
            Affine design, hudDesign, fullDesign, grid;
        };

        OverlayLayout Layout(State& s, uint32_t width, uint32_t height)
        {
            OverlayLayout l;
            const float scale = float(height) / kDesignHeight;
            l.design = Affine::Scale(scale, scale).Then(Affine::Translate((float(width) - kDesignWidth * scale) / 2, 0));
            // The HUD: 17559's (852x480) by the height, centred; the Blades HUD (xam.xex 1888 ... 6770, 1120x770) as
            // the Blades dashboards show their scenes: scaled to cover, the top and bottom cropped.
            l.hudDesign = l.design;
            if (s.hud && (s.hud->Width() != kDesignWidth || s.hud->Height() != kDesignHeight) && s.hud->Width() > 0 &&
                s.hud->Height() > 0)
            {
                const float hw = s.hud->Width(), hh = s.hud->Height();
                const float hs = std::max(float(width) / hw, float(height) / hh);
                l.hudDesign = Affine::Scale(hs, hs).Then(Affine::Translate((float(width) - hw * hs) / 2,
                                                                           (float(height) - hh * hs) / 2));
            }
            l.grid = l.hudDesign;
            // The full-screen scene: its root's size scaled to cover the output, centred, on black. The Blades
            // dashboards design at 1120x770 and show it so: scaled by the width, the top and bottom cropped; a
            // 1280x720 scene is drawn 1:1.
            if (s.full)
            {
                const float fw = s.full->Width(), fh = s.full->Height();
                const float fs = std::max(float(width) / fw, float(height) / fh);
                l.fullDesign = Affine::Scale(fs, fs).Then(Affine::Translate((float(width) - fw * fs) / 2,
                                                                            (float(height) - fh * fs) / 2));
                // Its images snap to its own units unless the HUD (on its grid) is drawn over it.
                if (s.phase == Phase::kClosed && !s.toastScene)
                    l.grid = l.fullDesign;
            }
            return l;
        }

        // The overlay's layers within `clip`: the full-screen scene on black, the dim, the HUD, the toast.
        void DrawLayers(State& s, Surface& surface, const OverlayLayout& l, const ClipRect& clip)
        {
            const float width = float(surface.Width()), height = float(surface.Height());
            const std::vector<Vec2> screen = { { 0, 0 }, { width, 0 }, { width, height }, { 0, height } };
            Paint paint;
            if (s.full)
            {
                paint.color = { 0, 0, 0, 1 };
                surface.FillPaint(screen, paint, clip);
                s.full->Render(*s.assets, surface, l.fullDesign, 1.0f, clip);
            }
            if (s.phase != Phase::kClosed)
            {
                // The held layer and the fading one over it (black over black: one fill of the combined alpha).
                const DimLayers& layers = s.dimLayers;
                paint.color = { 0, 0, 0, 1.0f - (1.0f - layers.hold) * (1.0f - layers.fade * s.dim) };
                surface.FillPaint(screen, paint, clip);
            }
            if (s.hud && s.phase != Phase::kClosed)
                s.hud->Render(*s.assets, surface, l.hudDesign, 1.0f, clip);
            if (s.toastScene)
                s.toastScene->Render(*s.assets, surface,
                                     Affine::Translate(float(s.toastX), float(s.toastY)).Then(l.design), 1.0f, clip);
        }
    }

    bool OverlayVisible()
    {
        State& s = S();
        std::lock_guard lock(s.mutex);
        return s.assets && (s.phase != Phase::kClosed || s.toastScene || s.full);
    }

    bool DrawOverlay(Surface& surface, const ClipRect& clip, bool& changed)
    {
        State& s = S();
        RunDue due{ s };
        std::lock_guard lock(s.mutex);
        changed = s.dirty;
        s.dirty = false;
        if (!s.assets || (s.phase == Phase::kClosed && !s.toastScene && !s.full))
            return false;
        const OverlayLayout layout = Layout(s, uint32_t(surface.Width()), uint32_t(surface.Height()));
        surface.SetUnitGrid(layout.grid);
        DrawLayers(s, surface, layout, clip);
        return true;
    }

    bool RenderOverlay(uint32_t width, uint32_t height, uint64_t vblank, std::vector<uint8_t>& rgba, bool& changed)
    {
        State& s = S();
        RunDue due{ s };
        std::lock_guard lock(s.mutex);
        changed = false;
        if (!s.assets)
            return false;
        Advance(s, vblank);

        const bool visible = s.phase != Phase::kClosed || s.toastScene || s.full;
        if (!visible)
        {
            changed = !s.last.empty();
            s.last.clear();
            return false;
        }
        if (!s.dirty && width == s.lastWidth && height == s.lastHeight && !s.last.empty())
        {
            rgba = s.last;
            return true;
        }
        if (!s.canvas || s.canvas->Width() != int(width) || s.canvas->Height() != int(height))
            s.canvas = std::make_unique<Canvas>(int(width), int(height));
        Canvas& canvas = *s.canvas;
        rgba.resize(size_t(width) * height * 4);
        const OverlayLayout layout = Layout(s, width, height);
        canvas.SetUnitGrid(layout.grid);
        // Drawn in horizontal bands on several threads: every draw is clipped to its band's rows, so each pixel
        // sees the same draws in the same order as in one pass (identical result). The overlay is drawn on the
        // GPU thread, which the title waits for: in one pass a message box took ~11 ms and the keyboard ~20 ms,
        // which held the game (and the popup's animation) to 30 frames/s.
        const int bands = 16;
        DrawBands(bands, [&](int band) {
            const ClipRect clip{ 0, int(height) * band / bands, int(width), int(height) * (band + 1) / bands };
            canvas.ClearRows(clip.y0, clip.y1);
            DrawLayers(s, canvas, layout, clip);
            canvas.ToRgba8Rows(rgba, clip.y0, clip.y1);
        });
        s.last = rgba;
        s.lastWidth = width;
        s.lastHeight = height;
        s.dirty = false;
        changed = true;
        return true;
    }
}
