#pragma once

// PlayStation controllers for the ports (owner, 2026-10-16): DualShock 4 and DualSense (and DualSense Edge) read
// straight from Windows' HID, over USB or Bluetooth, without Steam Input or DS4Windows. XInput only knows Xbox
// controllers, so each runtime's input layer gives these to the players that have no XInput controller.
//
// Mapped like the Xbox 360 controller in the same places: Cross A, Circle B, Square X, Triangle Y, L1 / R1 the
// shoulders, L2 / R2 the triggers (analog), L3 / R3 the stick clicks, Options START, Share / Create BACK, the PS
// button the Guide button, the d-pad, both sticks. The touchpad and motion are not used. CB_NO_HID_PADS=1 turns it
// off (e.g. when Steam Input or DS4Windows already shows the controller as an Xbox one, so it is not read twice).
// Runtime-neutral (logs through the runtime's "log.h").

#include <cstdint>

namespace pads
{
    struct Pad
    {
        uint16_t buttons = 0;     // XINPUT_GAMEPAD_* bits (no Guide)
        bool guide = false;       // the PS button
        uint8_t leftTrigger = 0, rightTrigger = 0;
        int16_t thumbLX = 0, thumbLY = 0, thumbRX = 0, thumbRY = 0; // XInput's ranges, up = positive
    };

    // Starts finding controllers (now and every few seconds, so one plugged in or paired later is found).
    void Start();
    // The `index`-th connected PlayStation controller (in the order they were found); false if there is none.
    bool Read(uint32_t index, Pad& pad);
}
