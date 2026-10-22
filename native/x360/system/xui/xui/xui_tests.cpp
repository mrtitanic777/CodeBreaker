// CB_XUI_TEST=1: checks of the XUI readers (xur.h, package.h, image.h).
//
// Every package of the system UI assets: every scene reads to the exact end of its object stream (the Guide's own
// scenes of hud.xex, whose classes are not known and which a title never opens, are listed but not counted), every
// string table reads, and every PNG decodes to exactly what Windows' WIC decoder gives. Each scene is dumped as text
// to analysis/xui/<package>/<entry>.txt (Microsoft data: not committed).

#include "xui/xui_tests.h"

#include "xui/element.h"
#include "xui/image.h"
#include "xui/package.h"
#include "xui/system_ui.h"
#include "xui/xur.h"
#include "xui/xur_write.h"
#include "log.h"

#include <windows.h>
#include <shlwapi.h>
#include <wincodec.h>
#include <wrl/client.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <format>
#include <fstream>

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "windowscodecs.lib")

namespace xui
{
    namespace
    {
        // Reference decoding with WIC: non-premultiplied RGBA.
        bool DecodeWithWic(const uint8_t* data, size_t size, Image& out)
        {
            using Microsoft::WRL::ComPtr;
            static ComPtr<IWICImagingFactory> factory = [] {
                CoInitializeEx(nullptr, COINIT_MULTITHREADED);
                ComPtr<IWICImagingFactory> f;
                CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&f));
                return f;
            }();
            if (!factory)
                return false;
            ComPtr<IStream> stream;
            stream.Attach(SHCreateMemStream(data, UINT(size)));
            ComPtr<IWICBitmapDecoder> decoder;
            ComPtr<IWICBitmapFrameDecode> frame;
            ComPtr<IWICFormatConverter> converter;
            if (!stream || FAILED(factory->CreateDecoderFromStream(stream.Get(), nullptr, WICDecodeMetadataCacheOnDemand,
                                                                   &decoder)) ||
                FAILED(decoder->GetFrame(0, &frame)) || FAILED(factory->CreateFormatConverter(&converter)) ||
                FAILED(converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr,
                                             0, WICBitmapPaletteTypeCustom)))
                return false;
            UINT w = 0, h = 0;
            converter->GetSize(&w, &h);
            out.width = w;
            out.height = h;
            out.rgba.resize(size_t(w) * h * 4);
            return SUCCEEDED(converter->CopyPixels(nullptr, w * 4, UINT(out.rgba.size()), out.rgba.data()));
        }

        // A scene's dump with each figure reference replaced by the figure's path (a written scene stores its
        // figures at other offsets).
        std::string DumpWithFigures(const Scene& scene)
        {
            std::string dump = DumpNode(scene.Root()), out;
            size_t at = 0;
            for (size_t f; (f = dump.find("figure@", at)) != std::string::npos;)
            {
                out.append(dump, at, f - at);
                size_t end = f + 7;
                uint32_t offset = 0;
                while (end < dump.size() && dump[end] >= '0' && dump[end] <= '9')
                    offset = offset * 10 + uint32_t(dump[end++] - '0');
                const FigurePath* path = scene.Figure(offset);
                out += path ? std::format("figure({}x{}", path->width, path->height) : std::string("figure(none");
                if (path)
                    for (const FigurePoint& p : path->points)
                        out += std::format(" {},{},{},{},{},{}", p.x, p.y, p.c1x, p.c1y, p.c2x, p.c2y);
                out += ")";
                at = end;
            }
            out.append(dump, at);
            return out;
        }

        // A scene written again (xur_write.h) reads back to the same tree.
        bool RoundTrip(const Scene& scene, std::string& error)
        {
            std::vector<uint8_t> written;
            if (!WriteScene(scene.Root(), [&scene](uint32_t offset) { return scene.Figure(offset); }, written, error))
                return false;
            Scene again;
            if (!again.Load(written.data(), written.size(), error))
                return false;
            const std::string a = DumpWithFigures(scene), b = DumpWithFigures(again);
            if (a == b)
                return true;
            size_t line = 1, i = 0;
            for (; i < a.size() && i < b.size() && a[i] == b[i]; i++)
                line += a[i] == '\n';
            error = std::format("differs at line {} of the dump", line);
            return false;
        }

        // Scene packages beyond the system UI's (CB_XUI_EXTRA: folders separated by ';', such as the dashboard's own
        // packages): each scene reads and survives a write; dumps in analysis/xui/extra/<folder>/<package>/.
        int TestExtraScenes()
        {
            const char* extra = std::getenv("CB_XUI_EXTRA");
            if (!extra)
                return 0;
            int failures = 0, scenes = 0, unread = 0;
            std::string_view list = extra;
            while (!list.empty())
            {
                const size_t semi = std::min(list.find(';'), list.size());
                const std::filesystem::path dir = std::string(list.substr(0, semi));
                list.remove_prefix(std::min(semi + 1, list.size()));
                std::error_code ec;
                for (const auto& file : std::filesystem::recursive_directory_iterator(dir, ec))
                {
                    if (file.path().extension() != ".xzp")
                        continue;
                    Package package;
                    std::string error;
                    if (!package.Load(file.path(), error))
                        continue; // not every .xzp is a scene package (encrypted language packs)
                    for (const Package::Entry& e : package.Entries())
                    {
                        if (!e.name.ends_with(".xur"))
                            continue;
                        Scene scene;
                        if (!scene.Load(e.data, e.size, error))
                        {
                            Log("XUI test: ({}/{} not read: {})", file.path().string(), e.name, error);
                            unread++;
                            continue;
                        }
                        scenes++;
                        const std::filesystem::path dump = std::filesystem::path("analysis/xui/extra") /
                                                           file.path().parent_path().filename() / file.path().stem();
                        std::filesystem::create_directories(dump);
                        std::ofstream(dump / (e.name + ".txt")) << DumpNode(scene.Root());
                        if (!RoundTrip(scene, error))
                        {
                            Log("XUI test: FAILED: {}/{}: written again: {}", file.path().string(), e.name, error);
                            failures++;
                        }
                    }
                }
            }
            Log("XUI test: {} further scenes written again ({} not read): {} failures", scenes, unread, failures);
            return failures;
        }

        // The XDK's XUI XML samples compile (xur_write.h CompileScene) and read back.
        int TestCompile()
        {
            const std::filesystem::path dir = "C:/Program Files (x86)/Microsoft Xbox 360 SDK/bin/win32/xuimedia";
            std::error_code ec;
            if (!std::filesystem::is_directory(dir, ec))
                return 0;
            int failures = 0, scenes = 0, skipped = 0;
            for (const auto& file : std::filesystem::directory_iterator(dir, ec))
            {
                if (file.path().extension() != ".xui")
                    continue;
                std::vector<uint8_t> xml, compiled;
                std::string error;
                Scene scene;
                scenes++;
                if (!ReadFile(file.path(), xml) ||
                    !CompileScene(std::string_view(reinterpret_cast<const char*>(xml.data()), xml.size()), compiled,
                                  error) ||
                    !scene.Load(compiled.data(), compiled.size(), error))
                {
                    // The XDK's HTML element and shader effects have no property tables here (xur.cpp); scenes
                    // with them are listed, not counted.
                    if (error.ends_with("not a known XUI class"))
                    {
                        Log("XUI test: ({} not compiled: {})", file.path().filename().string(), error);
                        skipped++;
                        continue;
                    }
                    Log("XUI test: FAILED: compiling {}: {}", file.path().filename().string(), error);
                    failures++;
                    continue;
                }
                // The XDK's own binary of its default skin (version 5, its xui2bin's): the same tree.
                if (file.path().stem() == "skin_default")
                {
                    Package package;
                    const Package::Entry* e = nullptr;
                    Scene reference;
                    if (!package.Load(dir / "skin_default.xzp", error) || !(e = package.Find("skin_default.xur")) ||
                        !reference.Load(e->data, e->size, error))
                    {
                        Log("XUI test: FAILED: the XDK's skin_default.xzp: {}", error.empty() ? "no skin_default.xur" : error);
                        failures++;
                    }
                    else if (DumpWithFigures(reference) != DumpWithFigures(scene))
                    {
                        const std::string a = DumpWithFigures(reference), b = DumpWithFigures(scene);
                        size_t line = 1;
                        for (size_t i = 0; i < a.size() && i < b.size() && a[i] == b[i]; i++)
                            line += a[i] == '\n';
                        Log("XUI test: FAILED: skin_default.xui compiled differs from the XDK's binary at dump line {}", line);
                        std::ofstream("analysis/xui/skin_default_xdk.txt") << a;
                        std::ofstream("analysis/xui/skin_default_ours.txt") << b;
                        failures++;
                    }
                    else
                        Log("XUI test: skin_default.xui compiled = the XDK's skin_default.xzp (version 5) read");
                }
            }
            Log("XUI test: {} XDK XML scenes compiled ({} with classes not known skipped): {} failures", scenes,
                skipped, failures);
            return failures;
        }

        int TestAssets()
        {
            const std::filesystem::path dir = AssetDirectory();
            if (!std::filesystem::is_directory(dir))
            {
                Log("XUI test: FAILED: no system UI assets at {}", dir.string());
                return 1;
            }
            int failures = 0, scenes = 0, tables = 0, images = 0, guideScenes = 0;
            const std::filesystem::path out = "analysis/xui";
            for (const auto& file : std::filesystem::directory_iterator(dir))
            {
                if (file.path().extension() != ".xzp")
                    continue;
                Package package;
                std::string error;
                if (!package.Load(file.path(), error))
                {
                    Log("XUI test: FAILED: {}: {}", file.path().filename().string(), error);
                    failures++;
                    continue;
                }
                const std::string packageName = file.path().stem().string();
                for (const Package::Entry& e : package.Entries())
                {
                    if (e.name.ends_with(".xur"))
                    {
                        scenes++;
                        Scene scene;
                        if (!scene.Load(e.data, e.size, error))
                        {
                            if (packageName == "hud.hud" && error.starts_with("class Guide"))
                            {
                                Log("XUI test: (Guide scene {} not read: {})", e.name, error);
                                guideScenes++;
                                continue;
                            }
                            Log("XUI test: FAILED: {}/{}: {}", packageName, e.name, error);
                            failures++;
                            continue;
                        }
                        std::filesystem::create_directories(out / packageName);
                        std::ofstream(out / packageName / (e.name + ".txt")) << DumpNode(scene.Root());
                        if (!RoundTrip(scene, error))
                        {
                            Log("XUI test: FAILED: {}/{}: written again: {}", packageName, e.name, error);
                            failures++;
                        }
                    }
                    else if (e.size >= 8 && std::memcmp(e.data, "\x89PNG", 4) == 0)
                    {
                        images++;
                        Image mine, reference;
                        if (!DecodePng(e.data, e.size, mine, error))
                        {
                            Log("XUI test: FAILED: {}/{}: {}", packageName, e.name, error);
                            failures++;
                            continue;
                        }
                        if (!DecodeWithWic(e.data, e.size, reference))
                        {
                            Log("XUI test: FAILED: {}/{}: WIC cannot decode it", packageName, e.name);
                            failures++;
                            continue;
                        }
                        size_t differ = 0;
                        if (mine.width != reference.width || mine.height != reference.height)
                            differ = SIZE_MAX;
                        else
                            for (size_t i = 0; i < mine.rgba.size(); i += 4)
                                differ += std::memcmp(&mine.rgba[i], &reference.rgba[i], 4) != 0;
                        if (differ)
                        {
                            Log("XUI test: FAILED: {}/{} ({}x{}): {} pixels differ from WIC", packageName, e.name,
                                mine.width, mine.height, differ == SIZE_MAX ? std::string("size") : std::to_string(differ));
                            failures++;
                        }
                    }
                    else if (e.name.ends_with(".xus"))
                    {
                        tables++;
                        std::vector<std::string> strings;
                        if (!ReadStringTable(e.data, e.size, strings, error))
                        {
                            Log("XUI test: FAILED: {}/{}: {}", packageName, e.name, error);
                            failures++;
                            continue;
                        }
                        if (e.name.find('\\') == std::string::npos)
                        {
                            std::filesystem::create_directories(out / packageName);
                            std::ofstream o(out / packageName / (e.name + ".txt"));
                            for (size_t i = 0; i < strings.size(); i++)
                                o << i << ": " << strings[i] << "\n";
                        }
                    }
                }
            }
            Log("XUI test: {} scenes ({} Guide scenes skipped), {} string tables, {} PNG images in {}: {} failures "
                "(dumps in {})", scenes, guideScenes, tables, images, dir.string(), failures, out.string());
            return failures;
        }

        // XAM's key curves (CurveFraction): every curve and direction runs from 0 to 1 with the parameters the system
        // UI uses, and spot values match XAM's formulas.
        int TestCurves()
        {
            int failures = 0;
            auto check = [&](const char* what, float got, float want) {
                if (std::abs(got - want) > 0.001f)
                {
                    Log("XUI test: FAILED: curve {}: {} instead of {}", what, got, want);
                    failures++;
                }
            };
            const struct { Interpolation type; float p1, p2; } curves[] = {
                { Interpolation::kLinear, 0, 0 }, { Interpolation::kQuadratic, 0, 0 }, { Interpolation::kCubic, 0, 0 },
                { Interpolation::kQuartic, 0, 0 }, { Interpolation::kQuintic, 0, 0 }, { Interpolation::kPower, 2.5f, 0 },
                { Interpolation::kSine, 0, 0 }, { Interpolation::kCircular, 0, 0 }, { Interpolation::kExponential, 5, 0 },
                { Interpolation::kElastic, 3, 2 }, { Interpolation::kBounce, 3, 2 },
            };
            for (const auto& c : curves)
                for (int d = 0; d < 3; d++)
                {
                    KeyFrame k;
                    k.interpolation = c.type;
                    k.direction = EaseDirection(d);
                    k.p1 = c.p1;
                    k.p2 = c.p2;
                    const std::string name = std::format("{}d{}", int(c.type), d);
                    check((name + " at 0").c_str(), CurveFraction(k, 0.0f), 0.0f);
                    check((name + " at 1").c_str(), CurveFraction(k, 1.0f), 1.0f);
                }
            KeyFrame k;
            k.interpolation = Interpolation::kExponential;
            k.direction = EaseDirection::kOut;
            k.p1 = 5;
            check("exponential out (5) at 0.2", CurveFraction(k, 0.2f), 1.0f - (std::exp(4.0f) - 1) / (std::exp(5.0f) - 1));
            k.interpolation = Interpolation::kQuadratic;
            check("quadratic out at 0.5", CurveFraction(k, 0.5f), 0.75f);
            k.direction = EaseDirection::kInOut;
            check("quadratic in-out at 0.25", CurveFraction(k, 0.25f), 0.125f);
            check("ease (0,0,0) at 0.5", EaseFraction(0, 0, 0, 0.5f), 0.5f);
            Log("XUI test: key curves: {} failures", failures);
            return failures;
        }
    }

    namespace
    {
        void WriteBmp(const std::filesystem::path& path, uint32_t width, uint32_t height, const std::vector<uint8_t>& rgb)
        {
            std::ofstream out(path, std::ios::binary);
            const uint32_t rowBytes = (width * 3 + 3) & ~3u, size = 54 + rowBytes * height;
            uint8_t header[54] = { 'B', 'M' };
            auto put32 = [&](int at, uint32_t v) { for (int i = 0; i < 4; i++) header[at + i] = uint8_t(v >> (8 * i)); };
            put32(2, size);
            put32(10, 54);
            put32(14, 40);
            put32(18, width);
            put32(22, height);
            header[26] = 1;
            header[28] = 24;
            out.write(reinterpret_cast<const char*>(header), 54);
            std::vector<uint8_t> row(rowBytes);
            for (uint32_t y = 0; y < height; y++)
            {
                const uint8_t* src = &rgb[size_t(height - 1 - y) * width * 3];
                for (uint32_t x = 0; x < width; x++)
                {
                    row[x * 3] = src[x * 3 + 2];
                    row[x * 3 + 1] = src[x * 3 + 1];
                    row[x * 3 + 2] = src[x * 3];
                }
                out.write(reinterpret_cast<const char*>(row.data()), rowBytes);
            }
        }

        // Renders the overlay over a flat background colour after `frames` vblanks and writes it as a BMP.
        void RenderFrames(const std::filesystem::path& path, uint32_t frames, uint64_t& vblank)
        {
            constexpr uint32_t kWidth = 1280, kHeight = 720;
            std::vector<uint8_t> overlay;
            bool changed = false;
            bool visible = false;
            double total = 0, worst = 0;
            for (uint32_t i = 0; i < frames; i++)
            {
                const auto started = std::chrono::steady_clock::now();
                visible = RenderOverlay(kWidth, kHeight, ++vblank, overlay, changed);
                const double ms =
                    std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - started).count();
                total += ms;
                worst = std::max(worst, ms);
            }
            std::vector<uint8_t> rgb(size_t(kWidth) * kHeight * 3);
            for (size_t i = 0; i < size_t(kWidth) * kHeight; i++)
            {
                const uint8_t background[3] = { 96, 120, 80 };
                for (int c = 0; c < 3; c++)
                {
                    const float a = visible ? overlay[i * 4 + 3] / 255.0f : 0.0f;
                    const float v = (visible ? overlay[i * 4 + c] : 0) + background[c] * (1 - a);
                    rgb[i * 3 + c] = uint8_t(std::min(255.0f, v + 0.5f));
                }
            }
            WriteBmp(path, kWidth, kHeight, rgb);
            Log("XUI test: rendered {} ({} frames, {:.2f} ms per frame on average, {:.2f} ms at most)", path.string(),
                frames, frames ? total / frames : 0.0, worst);
        }

        int TestRender()
        {
            if (!SystemUiAvailable())
                return 1;
            uint64_t vblank = 1000;
            std::filesystem::create_directories("analysis/xui");
            ShowMessageBox(u"Gamer profile not online", u"This feature requires a gamer profile which is signed into "
                           u"Xbox Live.", { u"OK" }, 0, MessageIcon::kAlert);
            RenderFrames("analysis/xui/render_messagebox_opening.bmp", 8, vblank);
            RenderFrames("analysis/xui/render_messagebox.bmp", 60, vblank);
            ClosePopup();
            RenderFrames("analysis/xui/render_closed.bmp", 60, vblank);
            ShowMessageBox(u"Unlock Full Game", u"You're playing the trial game, but you'll need the full game to be "
                           u"able to play Mini Games.\r\nWould you like to unlock the full game now?",
                           { u"Unlock Full Game", u"Cancel" }, 0, MessageIcon::kAlert);
            RenderFrames("analysis/xui/render_messagebox2.bmp", 60, vblank);
            ClosePopup();
            RenderFrames("analysis/xui/render_closed2.bmp", 60, vblank);
            QueueNotification(kNotifySignedIn, 1, u"Player2");
            RenderFrames("analysis/xui/render_toast1.bmp", 50, vblank);
            RenderFrames("analysis/xui/render_toast2.bmp", 100, vblank);
            RenderFrames("analysis/xui/render_closed3.bmp", 400, vblank);
            ShowKeyboard(u"Enter Sign Text", u"Enter a line of text for your sign", u"", 15);
            RenderFrames("analysis/xui/render_keyboard.bmp", 60, vblank);
            for (uint16_t button : { uint16_t(0x0008), uint16_t(0x1000), uint16_t(0x0002), uint16_t(0x1000),
                                     uint16_t(0x8000), uint16_t(0x0040), uint16_t(0x1000) })
            {
                KeyboardPress(button);
                RenderFrames("analysis/xui/render_keyboard2.bmp", 2, vblank);
            }
            RenderFrames("analysis/xui/render_keyboard2.bmp", 30, vblank);
            ClosePopup();
            // A popup requested while the keyboard is still closing opens after that animation.
            ShowMessageBox(u"Title", u"Text", { u"OK" }, 0, MessageIcon::kNone);
            RenderFrames("analysis/xui/render_replaced.bmp", 30, vblank);
            ClosePopup();
            RenderFrames("analysis/xui/render_closed4.bmp", 60, vblank);
            ShowSignIn({ { u"Player", u"", u"" }, { u"Player2", u"", u"" }, { u"Player3", u"", u"" },
                         { u"Player4", u"", u"" } }, 0);
            RenderFrames("analysis/xui/render_signin.bmp", 60, vblank);
            for (int i = 0; i < 5; i++)
            {
                PanePress(0x0002);
                RenderFrames("analysis/xui/render_signin2.bmp", 10, vblank);
            }
            RenderFrames("analysis/xui/render_signin2.bmp", 30, vblank);
            ClosePopup();
            SetNotificationPosition(1);
            QueueNotification(kNotifySignedIn, 1, u"Player2");
            RenderFrames("analysis/xui/render_toast_top.bmp", 60, vblank);
            SetNotificationPosition(2);
            RenderFrames("analysis/xui/render_closed7.bmp", 400, vblank);
            QueueNotification(kNotifyAchievement, 0, u"20|Castaway");
            QueueNotification(kNotifyAchievements, 0, u"2|35");
            RenderFrames("analysis/xui/render_toast_achievement.bmp", 60, vblank);
            RenderFrames("analysis/xui/render_closed8.bmp", 340, vblank);
            RenderFrames("analysis/xui/render_toast_achievements.bmp", 60, vblank);
            RenderFrames("analysis/xui/render_closed5.bmp", 60, vblank);
            std::vector<AchievementEntry> achievements;
            for (int i = 0; i < 30; i++)
            {
                AchievementEntry a;
                a.label = u"Achievement " + std::u16string(1, char16_t(u'A' + i % 26));
                a.description = u"Description of the achievement.";
                a.unachieved = u"How to unlock it.";
                a.gamerscore = 10 + uint32_t(i);
                a.earned = i % 3 == 0;
                a.showUnachieved = i % 2 == 0;
                a.unlockTime = 132100000000000000ull;
                achievements.push_back(a);
            }
            ShowAchievements(u"Minecraft", achievements);
            RenderFrames("analysis/xui/render_achievements.bmp", 60, vblank);
            for (uint16_t button : { uint16_t(0x0008), uint16_t(0x0002), uint16_t(0x0002), uint16_t(0x0002) })
            {
                PanePress(button);
                RenderFrames("analysis/xui/render_achievements2.bmp", 10, vblank);
            }
            ClosePopup();
            RenderFrames("analysis/xui/render_closed6.bmp", 60, vblank);

            // The Xbox Guide: a local profile's Home blade (capture 3:37:14), Games & Apps (3:37:33), Media (3:37:22),
            // Settings (3:37:27), the Xbox Home message box over it and back, a signed-out Guide (3:36:41).
            GuideInfo guide;
            guide.state = 9;
            guide.gamertag = u"Player1";
            guide.profiles = 4;
            ShowGuide(guide);
            RenderFrames("analysis/xui/render_guide_home.bmp", 60, vblank);
            GuidePress(0x0004);
            RenderFrames("analysis/xui/render_guide_games.bmp", 40, vblank);
            GuidePress(0x0008);
            RenderFrames("analysis/xui/render_guide_back_home.bmp", 40, vblank);
            GuidePress(0x0008);
            RenderFrames("analysis/xui/render_guide_media.bmp", 40, vblank);
            GuidePress(0x0008);
            RenderFrames("analysis/xui/render_guide_settings.bmp", 40, vblank);
            ShowGuideMessageBox(u"Xbox Home", CommonString(23, u""), { CommonString(298, u"Yes"),
                                CommonString(120, u"No") }, 1, MessageIcon::kWarning);
            RenderFrames("analysis/xui/render_guide_messagebox.bmp", 60, vblank);
            GuideMessageBoxClosed();
            RenderFrames("analysis/xui/render_guide_back.bmp", 60, vblank);
            ClosePopup();
            RenderFrames("analysis/xui/render_closed9.bmp", 60, vblank);
            guide.state = 6;
            guide.gamertag.clear();
            ShowGuide(guide);
            GuidePress(0x0004);
            RenderFrames("analysis/xui/render_guide_signedout_games.bmp", 40, vblank);
            GuidePress(0x0008);
            RenderFrames("analysis/xui/render_guide_signedout.bmp", 40, vblank);
            ClosePopup();
            RenderFrames("analysis/xui/render_closed10.bmp", 60, vblank);

            // The Guide's Community, party tab (owner's captures reference/.../party chat/"start a party.png",
            // "in a party.png", "party settings *.png").
            const std::vector<CommunityTab> partyTabs = {
                { "sharedres://ico_32x_StartParty.png", u"Xbox Live Party (0)" }, { "sharedres://ico_32x_Friend.png", u"0" },
                { "sharedres://ico_32x_Mail.png", u"0" }, { "sharedres://ico_32x_controller.png", u"0" } };
            CommunityRow start;
            start.line1 = u"Start a Party";
            CommunityOptions oneLine;
            oneLine.layout = CommunityLayout::kOneLine;
            ShowCommunity(u"Community", partyTabs, 0, { start }, 0,
                          u"Start a new party. A party is a group of players that can play games and chat together.",
                          oneLine);
            RenderFrames("analysis/xui/render_party_start.bmp", 60, vblank);
            ClosePopup();
            RenderFrames("analysis/xui/render_closed11.bmp", 60, vblank);
            std::vector<CommunityTab> inParty = partyTabs;
            inParty[0].text = u"Xbox Live Party (1)";
            std::vector<CommunityRow> partyRows(4);
            partyRows[0].line1 = u"Invite Players to Party";
            partyRows[1].line1 = u"Invite Party to Game";
            partyRows[1].enabled = false;
            partyRows[2].line1 = u"Party Options:  Party Chat";
            partyRows[3].member = partyRows[3].leader = true;
            partyRows[3].line1 = u"Hijack Assassin";
            partyRows[3].right = u"COD: Black Ops II";
            partyRows[3].status = "sharedres://ico_32x_controller.png";
            partyRows[3].statusColor = 0xFF92C542;
            CommunityOptions inPartyOptions = oneLine;
            inPartyOptions.legendX = u"Leave Party";
            inPartyOptions.legendY = u"Chat Channel";
            ShowCommunity(u"Community", inParty, 0, partyRows, 0, u"", inPartyOptions);
            RenderFrames("analysis/xui/render_party_in.bmp", 60, vblank);
            ClosePopup();
            RenderFrames("analysis/xui/render_closed12.bmp", 60, vblank);
            std::vector<CommunityRow> options(3);
            options[0].line1 = u"Chat Channel";
            options[0].value = u"Party Chat";
            options[1].line1 = u"Party Status";
            options[1].value = u"Invite Only";
            options[2].line1 = u"Leave Party";
            ShowCommunity(u"Party Options", {}, 0, options, 1, u"", oneLine);
            RenderFrames("analysis/xui/render_party_options.bmp", 60, vblank);
            PanePress(0x0001); // up: Chat Channel
            RenderFrames("analysis/xui/render_party_options2.bmp", 30, vblank);
            ClosePopup();
            RenderFrames("analysis/xui/render_closed13.bmp", 60, vblank);
            return 0;
        }
    }

    namespace
    {
        // CB_XUI_ERA=<system_ui/<update>>: an older system update's system UI in place of 17559's (an older
        // dashboard's, UseSystemUiPackages): its Guide on every blade, a message box and a toast, the keyboard and
        // sign-in, in analysis/xui/era/<update>_*.bmp. Run alone: the replaced packages stay for the process.
        int TestEra()
        {
            const char* era = std::getenv("CB_XUI_ERA");
            if (!era)
                return 0;
            std::string error;
            if (!SystemUiAvailable() || !UseSystemUiPackages(era, error))
            {
                Log("XUI era test: {}", error);
                return 1;
            }
            const std::string name = std::filesystem::path(era).filename().string();
            const std::filesystem::path dir = "analysis/xui/era";
            std::filesystem::create_directories(dir);
            auto out = [&](const char* what) { return dir / std::format("{}_{}.bmp", name, what); };
            uint64_t vblank = 1000;
            GuideInfo guide;
            guide.state = 9;
            guide.gamertag = u"Player1";
            guide.profiles = 4;
            ShowGuide(guide);
            RenderFrames(out("guide"), 60, vblank);
            for (int i = 0; i < 4; i++)
            {
                GuidePress(0x0004);
                RenderFrames(out(std::format("guide_left{}", i + 1).c_str()), 40, vblank);
            }
            for (int i = 0; i < 6; i++)
            {
                GuidePress(0x0008);
                RenderFrames(out(std::format("guide_right{}", i + 1).c_str()), 40, vblank);
            }
            ClosePopup();
            RenderFrames(out("closed"), 60, vblank);
            guide.state = 6;
            guide.gamertag.clear();
            ShowGuide(guide);
            RenderFrames(out("guide_signedout"), 60, vblank);
            ClosePopup();
            RenderFrames(out("closed2"), 60, vblank);
            ShowMessageBox(u"Gamer profile not online", u"This feature requires a gamer profile which is signed into "
                           u"Xbox Live.", { u"OK" }, 0, MessageIcon::kAlert);
            RenderFrames(out("messagebox"), 60, vblank);
            ClosePopup();
            RenderFrames(out("closed3"), 60, vblank);
            QueueNotification(kNotifySignedIn, 1, u"Player2");
            RenderFrames(out("toast"), 60, vblank);
            RenderFrames(out("closed4"), 400, vblank);
            ShowKeyboard(u"Enter Sign Text", u"Enter a line of text for your sign", u"", 15);
            RenderFrames(out("keyboard"), 60, vblank);
            ClosePopup();
            RenderFrames(out("closed5"), 60, vblank);
            return 0;
        }
    }

    std::filesystem::path AssetDirectory()
    {
        if (const char* v = std::getenv("CB_XUI_ASSETS"))
            return v;
        return "system_ui";
    }

    int RunXuiTests()
    {
        if (std::getenv("CB_XUI_ERA"))
            return TestEra();
        int failures = TestAssets();
        failures += TestExtraScenes();
        failures += TestCompile();
        failures += TestCurves();
        failures += TestRender();
        Log("XUI tests: {}", failures == 0 ? "all passed" : std::format("{} failed", failures));
        return failures;
    }
}
