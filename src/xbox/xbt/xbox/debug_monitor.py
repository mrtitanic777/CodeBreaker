"""Xbox 360 debug event monitor.

Ported from xbWatson DebugManager.
Connects via XBDM and monitors for debug events:
  - Execution breaks (breakpoints hit)
  - Debug strings (OutputDebugString)
  - Exceptions (access violations, etc.)
  - Assert failures
  - RIP events (fatal errors)
  - Execution state changes (reboot, title launch)

Uses XBDM's "notify" system to receive asynchronous debug events.
"""

import time
import threading
from typing import Optional, Callable
from dataclasses import dataclass
from xbdm import XbdmClient, XBDM_MULTI


# ── Debug event types ───────────────────────────────────

EVENT_EXEC_BREAK     = "break"
EVENT_DEBUG_STRING   = "debugstr"
EVENT_EXCEPTION      = "exception"
EVENT_ASSERT         = "assert"
EVENT_RIP            = "rip"
EVENT_STATE_CHANGE   = "execution"
EVENT_MODULE_LOAD    = "modload"
EVENT_MODULE_UNLOAD  = "modunload"
EVENT_CREATE_THREAD  = "createthread"
EVENT_DESTROY_THREAD = "destroythread"

# Exception codes
EXC_ACCESS_VIOLATION = 0xC0000005
EXC_BREAKPOINT       = 0x80000003
EXC_SINGLE_STEP      = 0x80000004
EXC_RIP              = 0x406D1388  # 1080890248
EXC_STACK_OVERFLOW   = 0xC00000FD


EXCEPTION_NAMES = {
    EXC_ACCESS_VIOLATION: "ACCESS_VIOLATION",
    EXC_BREAKPOINT: "BREAKPOINT",
    EXC_SINGLE_STEP: "SINGLE_STEP",
    EXC_RIP: "RIP (Fatal)",
    EXC_STACK_OVERFLOW: "STACK_OVERFLOW",
}


@dataclass
class DebugEvent:
    """A debug event from the console."""
    event_type: str = ""
    timestamp: float = 0.0
    thread_id: int = 0
    address: int = 0
    code: int = 0
    message: str = ""
    is_fatal: bool = False
    raw: str = ""


# ── Debug Monitor ───────────────────────────────────────

class DebugMonitor:
    """Monitor debug events from an Xbox 360 via XBDM.

    Usage:
        xbdm = XbdmClient("10.0.0.225")
        xbdm.connect()
        monitor = DebugMonitor(xbdm)
        monitor.on_event = lambda evt: print(f"[{evt.event_type}] {evt.message}")
        monitor.start()
        # ... events stream in ...
        monitor.stop()
    """

    def __init__(self, xbdm: XbdmClient):
        self.xbdm = xbdm
        self.on_event: Optional[Callable[[DebugEvent], None]] = None
        self.events: list[DebugEvent] = []
        self.max_events = 1000
        self._running = False
        self._thread: Optional[threading.Thread] = None
        self._connected_as_debugger = False

    def start(self):
        """Start monitoring debug events."""
        if self._running:
            return

        # Connect as debugger
        try:
            self.xbdm.command("debugger connect")
            self._connected_as_debugger = True
        except IOError:
            pass  # May already be connected or not supported

        # Enable debug notifications
        try:
            self.xbdm.command("notify reconnectport=0")
        except IOError:
            pass

        self._running = True
        self._thread = threading.Thread(target=self._poll_loop, daemon=True)
        self._thread.start()

    def stop(self):
        """Stop monitoring."""
        self._running = False
        if self._thread:
            self._thread.join(timeout=3.0)
            self._thread = None

        if self._connected_as_debugger:
            try:
                self.xbdm.command("debugger disconnect")
            except Exception:
                pass
            self._connected_as_debugger = False

    def _poll_loop(self):
        """Poll for debug events via XBDM notify."""
        while self._running:
            try:
                # Use "getd" (get debug output) or poll with notify
                resp, lines = self.xbdm.raw("notify")
                if lines:
                    for line in lines:
                        evt = self._parse_event(line)
                        if evt:
                            self._dispatch(evt)
                else:
                    # Also check for break state
                    self._check_break_state()
            except (IOError, ConnectionError):
                time.sleep(0.5)
            except Exception:
                time.sleep(0.5)

            time.sleep(0.1)

    def _check_break_state(self):
        """Check if the console has stopped at a break."""
        try:
            resp = self.xbdm.command("isstopped")
            if resp.code == 200 and "stopped" in resp.message.lower():
                evt = DebugEvent()
                evt.event_type = EVENT_EXEC_BREAK
                evt.timestamp = time.time()
                evt.message = resp.message
                self._dispatch(evt)

                # Get context of the stopped thread
                self._get_break_context(evt)
        except IOError:
            pass

    def _get_break_context(self, evt: DebugEvent):
        """Enrich a break event with thread context."""
        try:
            threads = self.xbdm.get_threads()
            for t in threads:
                if t.suspend_count > 0:
                    evt.thread_id = t.thread_id
                    ctx = self.xbdm.get_thread_context(t.thread_id)
                    if "Iar" in ctx:
                        evt.address = ctx["Iar"]
                    if "Lr" in ctx:
                        evt.message += f" IAR=0x{ctx.get('Iar', 0):08X} LR=0x{ctx.get('Lr', 0):08X}"
                    break
        except IOError:
            pass

    def _parse_event(self, line: str) -> Optional[DebugEvent]:
        """Parse an XBDM notification line into a DebugEvent."""
        evt = DebugEvent()
        evt.timestamp = time.time()
        evt.raw = line

        lower = line.lower()

        if "break " in lower or "stopped" in lower:
            evt.event_type = EVENT_EXEC_BREAK
            evt.message = line
            # Try to extract address
            if "addr=" in lower:
                try:
                    idx = lower.index("addr=")
                    addr_str = line[idx+5:].split()[0].strip()
                    evt.address = int(addr_str, 16)
                except (ValueError, IndexError):
                    pass
            return evt

        elif lower.startswith("debugstr"):
            evt.event_type = EVENT_DEBUG_STRING
            evt.message = line[9:] if len(line) > 9 else line
            return evt

        elif "exception" in lower:
            evt.event_type = EVENT_EXCEPTION
            evt.message = line
            # Try to extract exception code
            if "code=" in lower:
                try:
                    idx = lower.index("code=")
                    code_str = line[idx+5:].split()[0].strip()
                    evt.code = int(code_str, 0)
                    evt.message = f"{EXCEPTION_NAMES.get(evt.code, f'0x{evt.code:08X}')}: {line}"
                    evt.is_fatal = evt.code in (EXC_ACCESS_VIOLATION, EXC_STACK_OVERFLOW)
                except (ValueError, IndexError):
                    pass
            return evt

        elif "assert" in lower:
            evt.event_type = EVENT_ASSERT
            evt.message = line
            evt.is_fatal = True
            return evt

        elif "rip" in lower:
            evt.event_type = EVENT_RIP
            evt.message = line
            evt.is_fatal = True
            return evt

        elif "execution" in lower or "exec" in lower:
            evt.event_type = EVENT_STATE_CHANGE
            evt.message = line
            return evt

        elif "modload" in lower:
            evt.event_type = EVENT_MODULE_LOAD
            evt.message = line
            return evt

        elif "create" in lower and "thread" in lower:
            evt.event_type = EVENT_CREATE_THREAD
            evt.message = line
            return evt

        return None

    def _dispatch(self, evt: DebugEvent):
        """Store event and call handler."""
        self.events.append(evt)
        if len(self.events) > self.max_events:
            self.events = self.events[-self.max_events:]

        if self.on_event:
            try:
                self.on_event(evt)
            except Exception:
                pass

    # ── Actions ─────────────────────────────────────────

    def continue_execution(self):
        """Continue execution after a break."""
        self.xbdm.debug_go()

    def reboot_console(self):
        """Reboot the console."""
        self.xbdm.reboot()

    def dump_threads(self) -> list:
        """Get a snapshot of all threads with context."""
        result = []
        threads = self.xbdm.get_threads()
        for t in threads:
            info = {"thread_id": t.thread_id, "suspend": t.suspend_count}
            try:
                ctx = self.xbdm.get_thread_context(t.thread_id)
                info["context"] = ctx
            except IOError:
                pass
            result.append(info)
        return result

    def get_event_log(self, last_n: int = 50) -> list:
        """Get recent events."""
        return self.events[-last_n:]
