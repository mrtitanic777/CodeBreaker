"""A small client for QEMU's gdb remote protocol (i386 target): registers, memory, breakpoints.

    g = GDB(port); g.halt(); regs = g.regs(); data = g.read(addr, n)
    g.bp_add(linear); g.cont(); stop = g.wait(timeout)    # stop: the reply ("T05..." at a breakpoint)
"""
import socket
import struct
import time

REGS = ("eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", "eip", "eflags",
        "cs", "ss", "ds", "es", "fs", "gs")


class GDB(object):
    def __init__(self, port, host="127.0.0.1", connect_timeout=30):
        until = time.time() + connect_timeout
        while True:
            try:
                self.s = socket.create_connection((host, port), timeout=5)
                break
            except OSError:
                if time.time() > until:
                    raise
                time.sleep(0.5)
        self.s.settimeout(30)
        self.buf = b""
        self.no_ack = False
        self._cmd("QStartNoAckMode")
        self.no_ack = True

    # -- packets ---------------------------------------------------------------------------
    def _send(self, body):
        data = body.encode("latin1")
        pkt = b"$" + data + b"#" + ("%02x" % (sum(data) & 0xFF)).encode()
        self.s.sendall(pkt)

    def _recv_packet(self, timeout=30):
        self.s.settimeout(timeout)
        while True:
            start = self.buf.find(b"$")
            if start >= 0:
                end = self.buf.find(b"#", start)
                if end >= 0 and len(self.buf) >= end + 3:
                    body = self.buf[start + 1:end]
                    self.buf = self.buf[end + 3:]
                    if not self.no_ack:
                        self.s.sendall(b"+")
                    return body.decode("latin1")
            chunk = self.s.recv(65536)
            if not chunk:
                raise ConnectionError("gdb stub closed")
            self.buf += chunk

    def _cmd(self, body, timeout=30):
        self._send(body)
        while True:
            r = self._recv_packet(timeout)
            if r.startswith("O") and r != "OK":             # (console output: not the answer)
                continue
            return r

    # -- control ---------------------------------------------------------------------------
    def halt(self, timeout=10):
        """Stop the machine; its stop reply (None when it gave none: stopped already)."""
        self.s.sendall(b"\x03")
        try:
            return self._recv_packet(timeout)
        except socket.timeout:
            return None

    def cont(self):
        self._send("c")

    def wait(self, timeout):
        """The stop reply after `cont`, or None when nothing stopped within `timeout`."""
        try:
            return self._recv_packet(timeout)
        except socket.timeout:
            return None

    # -- state -----------------------------------------------------------------------------
    def regs(self):
        r = self._cmd("g")
        out = {}
        for i, name in enumerate(REGS):
            out[name] = struct.unpack("<I", bytes.fromhex(r[8 * i:8 * i + 8]))[0]
        return out

    def read(self, addr, n):
        out = b""
        while n > 0:
            k = min(n, 0x800)
            r = self._cmd("m%x,%x" % (addr, k))
            if r.startswith("E"):
                raise IOError("read %x: %s" % (addr, r))
            out += bytes.fromhex(r)
            addr += k
            n -= k
        return out

    def bp_add(self, addr):
        return self._cmd("Z0,%x,1" % addr) == "OK"

    def bp_del(self, addr):
        return self._cmd("z0,%x,1" % addr) == "OK"

    def monitor(self, text):
        """A QEMU monitor command through the stub (qRcmd): its output as text."""
        self._send("qRcmd," + text.encode().hex())
        out = ""
        while True:
            r = self._recv_packet(30)
            if r.startswith("O") and r != "OK":
                out += bytes.fromhex(r[1:]).decode("latin1")
                continue
            return out
