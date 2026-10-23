#include "pads/pads.h"

#include "log.h"

#include <windows.h>
#include <hidsdi.h>
#include <setupapi.h>

#include <algorithm>
#include <atomic>
#include <cstdlib>
#include <format>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#pragma comment(lib, "hid.lib")
#pragma comment(lib, "setupapi.lib")

namespace pads
{
    // A setting: CB_<name>, or MC_<name> (the name it had before CodeBreaker; the dashboard's settings use it).
    static const char* Setting(const char* name)
    {
        const std::string cb = std::string("CB_") + name, mc = std::string("MC_") + name;
        if (const char* v = std::getenv(cb.c_str()))
            return v;
        return std::getenv(mc.c_str());
    }

    namespace
    {
        constexpr uint16_t kSony = 0x054C;
        enum class Model { kDualShock4, kDualSense };
        struct Known
        {
            uint16_t product;
            Model model;
            const char* name;
        };
        constexpr Known kKnown[] = {
            { 0x05C4, Model::kDualShock4, "DualShock 4" },          { 0x09CC, Model::kDualShock4, "DualShock 4 (v2)" },
            { 0x0BA0, Model::kDualShock4, "DualShock 4 USB adapter" }, { 0x0CE6, Model::kDualSense, "DualSense" },
            { 0x0DF2, Model::kDualSense, "DualSense Edge" },
        };

        // XINPUT_GAMEPAD_* bits.
        constexpr uint16_t kUp = 0x0001, kDown = 0x0002, kLeft = 0x0004, kRight = 0x0008, kStart = 0x0010,
                           kBack = 0x0020, kLeftThumb = 0x0040, kRightThumb = 0x0080, kLeftShoulder = 0x0100,
                           kRightShoulder = 0x0200, kA = 0x1000, kB = 0x2000, kX = 0x4000, kY = 0x8000;

        struct Device
        {
            std::wstring path;
            Model model = Model::kDualShock4;
            HANDLE handle = INVALID_HANDLE_VALUE;
            std::atomic<bool> connected = true;
            std::mutex mutex;
            Pad pad;
        };

        std::mutex g_mutex;
        std::vector<std::shared_ptr<Device>> g_devices; // in the order found; disconnected ones are removed

        int16_t Axis(uint8_t v, bool invert)
        {
            const int centred = (int(v) - 128) * 256 + (v > 128 ? int(v - 128) * 2 : 0); // 255 reaches 32767
            const int value = invert ? -centred - 1 : centred;
            return int16_t(std::clamp(value, -32768, 32767));
        }

        // The DualShock 4's report (USB 0x01, Bluetooth 0x01 / 0x11) and the DualSense's Bluetooth 0x01 share this
        // layout from `d`: sticks LX LY RX RY, then the d-pad (low nibble, 8 = none) with Square / Cross / Circle /
        // Triangle (0x10 / 0x20 / 0x40 / 0x80), L1 R1 L2 R2 Share Options L3 R3 (0x01 ... 0x80), PS (0x01),
        // then the L2 / R2 analog values.
        void ParseDualShock4(const uint8_t* d, Pad& p)
        {
            p.thumbLX = Axis(d[0], false);
            p.thumbLY = Axis(d[1], true);
            p.thumbRX = Axis(d[2], false);
            p.thumbRY = Axis(d[3], true);
            const uint8_t face = d[4], shoulders = d[5], system = d[6];
            p.leftTrigger = d[7];
            p.rightTrigger = d[8];
            uint16_t b = 0;
            static constexpr uint16_t kHat[9] = { kUp, kUp | kRight, kRight, kDown | kRight, kDown, kDown | kLeft, kLeft,
                                                  kUp | kLeft, 0 };
            b |= kHat[std::min<uint8_t>(face & 0x0F, 8)];
            b |= face & 0x10 ? kX : 0;
            b |= face & 0x20 ? kA : 0;
            b |= face & 0x40 ? kB : 0;
            b |= face & 0x80 ? kY : 0;
            b |= shoulders & 0x01 ? kLeftShoulder : 0;
            b |= shoulders & 0x02 ? kRightShoulder : 0;
            b |= shoulders & 0x10 ? kBack : 0;
            b |= shoulders & 0x20 ? kStart : 0;
            b |= shoulders & 0x40 ? kLeftThumb : 0;
            b |= shoulders & 0x80 ? kRightThumb : 0;
            p.buttons = b;
            p.guide = (system & 0x01) != 0;
        }

        // The DualSense's full report (USB 0x01, Bluetooth 0x31) from `d`: sticks, L2 / R2 analog, a counter, then
        // the d-pad with the face buttons, L1 R1 L2 R2 Create Options L3 R3, PS.
        void ParseDualSense(const uint8_t* d, Pad& p)
        {
            const uint8_t reordered[9] = { d[0], d[1], d[2], d[3], d[7], d[8], d[9], d[4], d[5] };
            ParseDualShock4(reordered, p);
        }

        bool Parse(Model model, bool usb, const uint8_t* report, DWORD size, Pad& p)
        {
            if (size < 10)
                return false;
            switch (report[0])
            {
            case 0x01:
                // The DualSense over USB sends its full report as 0x01; over Bluetooth (before an extended report
                // is asked for) both models send the short DualShock 4-style 0x01. (Reads may be padded to the
                // longest report, so the connection tells them apart: USB reports are 64 bytes.)
                if (model == Model::kDualSense && usb && size >= 11)
                    ParseDualSense(report + 1, p);
                else
                    ParseDualShock4(report + 1, p);
                return true;
            case 0x11: // DualShock 4 over Bluetooth, extended
                if (size < 12)
                    return false;
                ParseDualShock4(report + 3, p);
                return true;
            case 0x31: // DualSense over Bluetooth, extended
                if (size < 13)
                    return false;
                ParseDualSense(report + 2, p);
                return true;
            }
            return false;
        }

        void ReadLoop(std::shared_ptr<Device> dev)
        {
            HIDP_CAPS caps{};
            PHIDP_PREPARSED_DATA preparsed = nullptr;
            if (HidD_GetPreparsedData(dev->handle, &preparsed))
            {
                HidP_GetCaps(preparsed, &caps);
                HidD_FreePreparsedData(preparsed);
            }
            std::vector<uint8_t> buffer(std::max<USHORT>(caps.InputReportByteLength, 128));
            const bool usb = caps.InputReportByteLength == 64;
            OVERLAPPED ov{};
            ov.hEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);
            for (;;)
            {
                ResetEvent(ov.hEvent);
                DWORD got = 0;
                if (!ReadFile(dev->handle, buffer.data(), DWORD(buffer.size()), &got, &ov))
                {
                    if (GetLastError() != ERROR_IO_PENDING)
                        break;
                    if (WaitForSingleObject(ov.hEvent, 2000) == WAIT_TIMEOUT)
                    {
                        CancelIo(dev->handle);
                        GetOverlappedResult(dev->handle, &ov, &got, TRUE);
                        continue; // nothing new (a controller sends steadily while connected)
                    }
                    if (!GetOverlappedResult(dev->handle, &ov, &got, FALSE))
                        break;
                }
                Pad pad;
                if (Parse(dev->model, usb, buffer.data(), got, pad))
                {
                    std::lock_guard lock(dev->mutex);
                    dev->pad = pad;
                }
            }
            CloseHandle(ov.hEvent);
            CloseHandle(dev->handle);
            dev->connected = false;
            Log("Input: a PlayStation controller was disconnected");
        }

        // Finds the Sony controllers not open yet.
        void Scan()
        {
            GUID hid;
            HidD_GetHidGuid(&hid);
            HDEVINFO set = SetupDiGetClassDevsW(&hid, nullptr, nullptr, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
            if (set == INVALID_HANDLE_VALUE)
                return;
            SP_DEVICE_INTERFACE_DATA iface{ sizeof(iface) };
            for (DWORD i = 0; SetupDiEnumDeviceInterfaces(set, nullptr, &hid, i, &iface); i++)
            {
                DWORD size = 0;
                SetupDiGetDeviceInterfaceDetailW(set, &iface, nullptr, 0, &size, nullptr);
                std::vector<uint8_t> storage(size);
                auto* detail = reinterpret_cast<SP_DEVICE_INTERFACE_DETAIL_DATA_W*>(storage.data());
                detail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_W);
                if (!SetupDiGetDeviceInterfaceDetailW(set, &iface, detail, size, nullptr, nullptr))
                    continue;
                const std::wstring path = detail->DevicePath;
                {
                    std::lock_guard lock(g_mutex);
                    if (std::any_of(g_devices.begin(), g_devices.end(),
                                    [&](const auto& d) { return d->connected && d->path == path; }))
                        continue;
                }
                HANDLE h = CreateFileW(path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
                                       nullptr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr);
                if (h == INVALID_HANDLE_VALUE)
                    h = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                                    OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr);
                if (h == INVALID_HANDLE_VALUE)
                    continue;
                HIDD_ATTRIBUTES attrs{ sizeof(attrs) };
                const Known* known = nullptr;
                if (HidD_GetAttributes(h, &attrs) && attrs.VendorID == kSony)
                    for (const Known& k : kKnown)
                        if (k.product == attrs.ProductID)
                            known = &k;
                // The gamepad collection only (the DualShock 4 USB adapter and some drivers list more than one).
                bool gamepad = false;
                PHIDP_PREPARSED_DATA preparsed = nullptr;
                if (known && HidD_GetPreparsedData(h, &preparsed))
                {
                    HIDP_CAPS caps{};
                    HidP_GetCaps(preparsed, &caps);
                    gamepad = caps.UsagePage == 0x01 && (caps.Usage == 0x05 || caps.Usage == 0x04);
                    HidD_FreePreparsedData(preparsed);
                }
                if (!gamepad)
                {
                    CloseHandle(h);
                    continue;
                }
                auto dev = std::make_shared<Device>();
                dev->path = path;
                dev->model = known->model;
                dev->handle = h;
                {
                    std::lock_guard lock(g_mutex);
                    g_devices.erase(std::remove_if(g_devices.begin(), g_devices.end(),
                                                   [](const auto& d) { return !d->connected; }),
                                    g_devices.end());
                    g_devices.push_back(dev);
                }
                Log("Input: {} connected (HID {:04X}:{:04X})", known->name, attrs.VendorID, attrs.ProductID);
                std::thread(ReadLoop, dev).detach();
            }
            SetupDiDestroyDeviceInfoList(set);
        }
    }

    void Start()
    {
        static std::once_flag once;
        std::call_once(once, [] {
            if (const char* off = Setting("NO_HID_PADS"); off && off[0] == '1')
            {
                Log("Input: PlayStation controllers off (CB_NO_HID_PADS=1)");
                return;
            }
            std::thread([] {
                for (;;)
                {
                    Scan();
                    Sleep(3000);
                }
            }).detach();
        });
    }

    bool Read(uint32_t index, Pad& pad)
    {
        std::shared_ptr<Device> dev;
        {
            std::lock_guard lock(g_mutex);
            uint32_t n = 0;
            for (const auto& d : g_devices)
                if (d->connected && n++ == index)
                {
                    dev = d;
                    break;
                }
        }
        if (!dev)
            return false;
        std::lock_guard lock(dev->mutex);
        pad = dev->pad;
        return true;
    }
}
