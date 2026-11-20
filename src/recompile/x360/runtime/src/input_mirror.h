#pragma once

#include "input.h"

// Debugging two copies of a game on one PC (Minecraft: a host and a friend in its world): the first user's controller
// of one copy also drives the first user of another, so both players move and act at once.
//   CB_INPUT_MIRROR_TO=<port> or <host>:<port>  this copy sends user 0's controller every presented frame (UDP;
//                                               host 127.0.0.1 by default)
//   CB_INPUT_MIRROR_FROM=<port>                 this copy receives on 127.0.0.1:<port>; its user 0 reads the sum of
//                                               its own controller and the mirrored one (buttons together, the
//                                               larger trigger, the stick pushed further), while packets arrive
// The Guide button is never mirrored (each copy's Guide stays its own). The sending copy starts paused (so menus can be
// set up in each window on its own) and F9 in its window turns the mirroring on and off; with CB_INPUT_SCRIPT it starts
// on. Not a console feature.
namespace input::mirror
{
    // Reads the switches; starts the receiver when CB_INPUT_MIRROR_FROM is set.
    void Start(bool scripted);
    // F9: mirroring on / off (the sending copy).
    void Toggle();
    // Whether this copy sends (CB_INPUT_MIRROR_TO).
    bool Sending();
    void Send(const PadState& pad);
    // The mirrored controller, if a packet came in the last second.
    bool Received(PadState& pad);
    // `own` with `mirrored` added in (see above).
    PadState Merge(const PadState& own, const PadState& mirrored);
}
