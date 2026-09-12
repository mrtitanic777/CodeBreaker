"""Xbox 360 controller automation over XBDM `autoinput` — virtual pad, button
macros, and turbo/rapid-fire.

Drives the debug monitor's `autoinput` command family (register a virtual
gamepad, stream button/stick packets, queue timed macros). The command wire
format is reimplemented from the XBDM autoinput protocol; the button bitmask is
the standard XInput layout. Command strings are built by pure functions so they
can be verified without a live console.

Commands: connect / disconnect / press / turbo / macro / profile.
"""

import sys
import time

DESCRIPTION = "Xbox 360 controller automation over XBDM (press/turbo/macro via a virtual pad)"

# Standard XInput gamepad button bitmask.
BUTTONS = {
    "UP": 0x0001, "DOWN": 0x0002, "LEFT": 0x0004, "RIGHT": 0x0008,
    "START": 0x0010, "BACK": 0x0020, "LS": 0x0040, "RS": 0x0080,
    "LB": 0x0100, "RB": 0x0200, "A": 0x1000, "B": 0x2000,
    "X": 0x4000, "Y": 0x8000,
}
# friendly aliases
_ALIAS = {"LTHUMB": "LS", "RTHUMB": "RS", "L3": "LS", "R3": "RS",
          "LSHOULDER": "LB", "RSHOULDER": "RB", "L1": "LB", "R1": "RB",
          "DPADUP": "UP", "DPADDOWN": "DOWN", "DPADLEFT": "LEFT", "DPADRIGHT": "RIGHT"}


def buttons_mask(names):
    """OR together a list/space-string of button names into a 16-bit mask."""
    if isinstance(names, str):
        names = names.replace(",", " ").split()
    m = 0
    for n in names:
        key = _ALIAS.get(n.strip().upper(), n.strip().upper())
        if key not in BUTTONS:
            raise ValueError("unknown button: %s (have %s)" % (n, ", ".join(BUTTONS)))
        m |= BUTTONS[key]
    return m & 0xFFFF


class Gamepad:
    def __init__(self, buttons=0, lt=0, rt=0, lx=0, ly=0, rx=0, ry=0):
        self.buttons = buttons & 0xFFFF
        self.lt = lt & 0xFF
        self.rt = rt & 0xFF
        self.lx, self.ly, self.rx, self.ry = lx, ly, rx, ry


# ── pure command builders (testable without a console) ─────────────────
def cmd_bind(user, queuelen=20):
    return "autoinput user=%d bind queuelen=%d" % (user, queuelen)


def cmd_unbind(user):
    return "autoinput user=%d unbind" % user


def cmd_connect(user):
    return "autoinput user=%d connect" % user


def cmd_disconnect(user):
    return "autoinput user=%d disconnect" % user


def cmd_setpacket(user, pad):
    return ("autoinput user=%d setpacket "
            "buttons=0x%04X lt=0x%02X rt=0x%02X "
            "lthumbx=%d lthumby=%d rthumbx=%d rthumby=%d" % (
                user, pad.buttons, pad.lt, pad.rt,
                pad.lx, pad.ly, pad.rx, pad.ry))


def cmd_queuepackets(user, count, duration):
    return "autoinput user=%d queuepackets count=%d duration=%d" % (user, count, duration)


def cmd_clearqueue(user):
    return "autoinput user=%d clearqueue" % user


def cmd_autoprof(xuid=None):
    return "autoprof" if xuid is None else "autoprof xuid=0x%X" % xuid


# ── macro script -> command sequence ───────────────────────────────────
def build_macro(user, script):
    """Translate a small macro script into an ordered list of XBDM commands.

    Lines (one per line, '#' comments):
      press <buttons> [ms]   set buttons for ms (default 100), then release
      hold  <buttons>        set buttons (stay until changed)
      stick L|R <x> <y>      set a thumbstick (-32768..32767)
      trigger L|R <0-255>    set a trigger
      wait  <ms>             idle for ms
      release                clear all buttons/sticks/triggers
    """
    pad = Gamepad()
    out = []
    for ln in script.splitlines():
        ln = ln.split("#", 1)[0].strip()
        if not ln:
            continue
        t = ln.split()
        op = t[0].lower()
        if op == "press":
            ms = int(t[-1]) if t[-1].lstrip("-").isdigit() else 100
            names = t[1:-1] if (t[-1].lstrip("-").isdigit()) else t[1:]
            pad.buttons = buttons_mask(names)
            out.append(cmd_setpacket(user, pad))
            out.append(cmd_queuepackets(user, 1, ms))
            pad.buttons = 0
            out.append(cmd_setpacket(user, pad))
        elif op == "hold":
            pad.buttons = buttons_mask(t[1:])
            out.append(cmd_setpacket(user, pad))
        elif op == "stick":
            side, x, y = t[1].upper(), int(t[2]), int(t[3])
            if side == "L":
                pad.lx, pad.ly = x, y
            else:
                pad.rx, pad.ry = x, y
            out.append(cmd_setpacket(user, pad))
        elif op == "trigger":
            side, v = t[1].upper(), int(t[2])
            if side == "L":
                pad.lt = v & 0xFF
            else:
                pad.rt = v & 0xFF
            out.append(cmd_setpacket(user, pad))
        elif op == "wait":
            out.append(cmd_queuepackets(user, 1, int(t[1])))
        elif op == "release":
            pad = Gamepad()
            out.append(cmd_setpacket(user, pad))
        else:
            raise ValueError("unknown macro op: %s" % op)
    return out


# ── live driver ────────────────────────────────────────────────────────
class Pad:
    def __init__(self, ip, user=0):
        import xbdm
        self.c = xbdm.XbdmClient(ip)
        self.user = user

    def _send(self, cmd):
        return self.c.command(cmd)

    def open(self, queuelen=20):
        self._send(cmd_bind(self.user, queuelen))
        self._send(cmd_connect(self.user))

    def close(self):
        self._send(cmd_disconnect(self.user))
        self._send(cmd_unbind(self.user))

    def press(self, names, hold_ms=100):
        self._send(cmd_setpacket(self.user, Gamepad(buttons=buttons_mask(names))))
        self._send(cmd_queuepackets(self.user, 1, hold_ms))
        self._send(cmd_setpacket(self.user, Gamepad()))

    def run_macro(self, script):
        for cmd in build_macro(self.user, script):
            self._send(cmd)


# ── CLI ────────────────────────────────────────────────────────────────
def main():
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage:\n"
              "  xinput <ip> connect|disconnect        [--user N]\n"
              "  xinput <ip> press <buttons> [--hold MS] [--user N]\n"
              "  xinput <ip> turbo <button> [--hz N]   [--user N]   (Ctrl-C to stop)\n"
              "  xinput <ip> macro <script.txt>        [--user N]\n"
              "  xinput <ip> profile [<xuid-hex>]\n"
              "buttons: " + " ".join(BUTTONS))
        return
    ip, op = a[0], a[1]
    user = int(a[a.index("--user") + 1]) if "--user" in a else 0
    pad = Pad(ip, user)
    if op in ("connect", "open"):
        pad.open(); print("virtual pad %d connected" % user); return
    if op in ("disconnect", "close"):
        pad.close(); print("virtual pad %d disconnected" % user); return
    if op == "press":
        hold = int(a[a.index("--hold") + 1]) if "--hold" in a else 100
        names = []                       # button names, up to the first --flag
        for x in a[2:]:
            if x.startswith("--"):
                break
            names.append(x)
        pad.open(); pad.press(names, hold)
        print("pressed %s for %dms" % ("+".join(names), hold)); return
    if op == "turbo":
        btn = a[2]
        hz = int(a[a.index("--hz") + 1]) if "--hz" in a else 10
        period = 1.0 / max(1, hz)
        pad.open()
        print("turbo %s @ %dHz on pad %d — Ctrl-C to stop" % (btn, hz, user))
        try:
            while True:
                pad.press([btn], int(period * 500))
                time.sleep(period)
        except KeyboardInterrupt:
            pad.close(); print("\nstopped"); return
    if op == "macro":
        pad.open()
        pad.run_macro(open(a[2], encoding="utf-8").read())
        print("macro complete"); return
    if op == "profile":
        xuid = int(a[2], 16) if len(a) > 2 else None
        pad._send(cmd_autoprof(xuid)); print("autoprof set"); return
    print("unknown op: %s" % op)


if __name__ == "__main__":
    main()
