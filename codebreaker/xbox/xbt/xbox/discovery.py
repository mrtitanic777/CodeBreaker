"""Xbox 360 console discovery.

Ported from XeCLI's Xbox360.Remote.ConsoleDiscovery.
Two methods:
  1. NAP (Name and Address Protocol) — UDP broadcast on port 731
  2. TCP scan — probe port 730 on local subnet
"""

import socket
import struct
import time
import threading
import ipaddress
from typing import Optional
from dataclasses import dataclass, field


@dataclass
class DiscoveredConsole:
    ip: str
    port: int = 730
    debug_name: Optional[str] = None
    console_id: Optional[str] = None
    source: str = "scan"


def _get_local_subnets() -> list:
    """Get local IPv4 subnets by probing the default gateway range.

    Falls back to 10.0.0.0/24 and 192.168.1.0/24 if detection fails.
    """
    subnets = []

    # Try to detect local IP by connecting to a public address (no data sent)
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.settimeout(0.1)
        s.connect(("8.8.8.8", 80))
        local_ip = s.getsockname()[0]
        s.close()

        # Assume /24 subnet
        parts = local_ip.split(".")
        if len(parts) == 4:
            subnet = f"{parts[0]}.{parts[1]}.{parts[2]}.0/24"
            subnets.append(ipaddress.IPv4Network(subnet, strict=False))
    except Exception:
        pass

    if not subnets:
        subnets = [
            ipaddress.IPv4Network("192.168.1.0/24"),
            ipaddress.IPv4Network("10.0.0.0/24"),
        ]

    return subnets


def discover_nap(timeout: float = 1.5) -> list:
    """Discover consoles via NAP (Name and Address Protocol).

    Sends a UDP broadcast to port 731 and listens for responses.
    NAP packet: type=3 (wildcard), length=0
    Response: type=2, length=N, then N bytes of debug name.
    """
    results = []
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
        sock.settimeout(timeout)
        sock.bind(("", 0))

        # Send NAP wildcard query
        sock.sendto(bytes([3, 0]), ("<broadcast>", 731))

        deadline = time.time() + timeout
        while time.time() < deadline:
            try:
                sock.settimeout(max(0.01, deadline - time.time()))
                data, addr = sock.recvfrom(512)
                if len(data) < 2 or data[0] != 2:
                    continue

                name_len = data[1]
                name = None
                if name_len > 0 and len(data) >= 2 + name_len:
                    name = data[2:2 + name_len].decode("ascii", errors="replace")

                # Deduplicate by IP
                ip = addr[0]
                if not any(c.ip == ip for c in results):
                    results.append(DiscoveredConsole(
                        ip=ip, port=731, debug_name=name, source="nap"))
            except socket.timeout:
                break
            except OSError:
                break

        sock.close()
    except Exception:
        pass

    return results


def _probe_tcp(ip: str, port: int, timeout: float) -> Optional[DiscoveredConsole]:
    """Try to connect to XBDM on a single IP."""
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.settimeout(timeout)
        sock.connect((ip, port))

        # Read handshake
        data = sock.recv(256)
        line = data.decode("ascii", errors="replace").strip()
        if not line or len(line) < 3:
            sock.close()
            return None

        code = int(line[:3])
        if code not in (200, 201):
            sock.close()
            return None

        # Try to get debug name
        debug_name = None
        console_id = None
        try:
            sock.sendall(b"dbgname\r\n")
            resp = sock.recv(256).decode("ascii", errors="replace").strip()
            if len(resp) > 4:
                debug_name = resp[4:]

            sock.sendall(b"getconsoleid\r\n")
            resp = sock.recv(256).decode("ascii", errors="replace").strip()
            if "consoleid=" in resp:
                idx = resp.find("consoleid=")
                console_id = resp[idx + 10:].strip()
            elif len(resp) > 4:
                console_id = resp[4:]
        except Exception:
            pass

        try:
            sock.sendall(b"bye\r\n")
        except Exception:
            pass
        sock.close()

        return DiscoveredConsole(
            ip=ip, port=port, debug_name=debug_name,
            console_id=console_id, source="tcp")

    except (socket.timeout, ConnectionRefusedError, OSError):
        return None


def discover_tcp(subnets: list = None, port: int = 730,
                 timeout: float = 0.4, max_threads: int = 64) -> list:
    """Discover consoles by scanning local subnet(s) on XBDM port.

    Scans up to 254 hosts per /24 subnet concurrently.
    """
    if subnets is None:
        subnets = _get_local_subnets()

    results = []
    lock = threading.Lock()
    semaphore = threading.Semaphore(max_threads)

    def probe(ip_str):
        with semaphore:
            console = _probe_tcp(ip_str, port, timeout)
            if console:
                with lock:
                    results.append(console)

    threads = []
    for subnet in subnets:
        for host in subnet.hosts():
            ip_str = str(host)
            t = threading.Thread(target=probe, args=(ip_str,), daemon=True)
            threads.append(t)
            t.start()

    for t in threads:
        t.join(timeout=timeout + 1.0)

    return results


def discover(use_nap: bool = True, use_tcp: bool = True,
             tcp_timeout: float = 0.4, nap_timeout: float = 1.5) -> list:
    """Run full console discovery. Returns list of DiscoveredConsole."""
    found = {}  # keyed by IP for dedup

    if use_nap:
        for c in discover_nap(timeout=nap_timeout):
            found[c.ip] = c

    if use_tcp:
        for c in discover_tcp(timeout=tcp_timeout):
            if c.ip not in found:
                found[c.ip] = c
            else:
                # Merge — TCP probe has richer data
                existing = found[c.ip]
                if not existing.debug_name and c.debug_name:
                    found[c.ip] = c

    return sorted(found.values(), key=lambda c: c.ip)
