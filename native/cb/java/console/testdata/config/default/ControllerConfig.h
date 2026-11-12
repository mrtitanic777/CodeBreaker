// ControllerConfig.h -- CodeBreaker-emitted controller mapping (edit platform.config.json + re-emit).
// The Input platform layer (Input.reference) reads these to map the 360 pad to the PC controls.
#ifndef J2X_CONTROLLER_CONFIG_H
#define J2X_CONTROLLER_CONFIG_H

// Stick layout: default   (Default / Southpaw / Legacy / Legacy-Southpaw)
#define CTRL_LAYOUT_DEFAULT 1

#define CTRL_INVERT_Y 0
#define CTRL_LOOK_SENSITIVITY 1.000f
#define CTRL_STICK_DEADZONE 0.240f
#define CTRL_TRIGGER_THRESHOLD 0.300f

// action -> 360 control binding:
#define CTRL_BIND_LOOK         RSTICK         // Mouse look (camera)
#define CTRL_BIND_MOVE         LSTICK         // Move (WASD)
#define CTRL_BIND_ATTACK       RTRIGGER       // Attack / break (left mouse)
#define CTRL_BIND_USE          LTRIGGER       // Use / place (right mouse)
#define CTRL_BIND_JUMP         A              // Jump (space)
#define CTRL_BIND_SNEAK        LSTICK_CLICK   // Sneak (shift)
#define CTRL_BIND_INVENTORY    Y              // Inventory (E)
#define CTRL_BIND_DROP         B              // Drop item (Q)
#define CTRL_BIND_HOTBAR_PREV  LBUMPER        // Hotbar left (wheel up)
#define CTRL_BIND_HOTBAR_NEXT  RBUMPER        // Hotbar right (wheel down)
#define CTRL_BIND_PAUSE        START          // Pause / menu (esc)
#define CTRL_BIND_CHAT         DPAD_UP        // Chat / command (T)
#define CTRL_BIND_PERSPECTIVE  DPAD_DOWN      // Toggle view (F5)
#define CTRL_BIND_SPRINT       RSTICK_CLICK   // Sprint
#define CTRL_BIND_GUI_CURSOR   LSTICK         // Virtual cursor in menus

#endif // J2X_CONTROLLER_CONFIG_H
