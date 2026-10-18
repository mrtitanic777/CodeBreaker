#include "stdafx.h"
#include <winsockx.h>          // 360 Winsock + XNet (socket/connect/send/recv, XNetStartup, XNetDnsLookup)
#include "Socket.h"
#include "InetAddress.h"       // the host-carrying InetAddress shim (Socket(InetAddress,port) reads getHostName())
#include "InputStream.h"       // SocketInputStream base (Beta keeps InputStream in its own canonical header)
#include "FileIO.h"            // OutputStream base + the data/stream classes

// ============================================================================================
// Network stack bring-up. XNetStartup with BYPASS_SECURITY = we may talk to non-Xbox (PC) peers.
// ============================================================================================
bool Socket::s_netInit = false;

bool Socket::globalInit() {
	if (s_netInit) return true;
	XNetStartupParams xnsp;
	memset(&xnsp, 0, sizeof(xnsp));
	xnsp.cfgSizeOfStruct = sizeof(XNetStartupParams);
	xnsp.cfgFlags = XNET_STARTUP_BYPASS_SECURITY;   // REQUIRED to connect to a PC Java server
	INT e = XNetStartup(&xnsp);
	if (e != 0) { DbgPrintF("[net] XNetStartup failed: %d\n", e); return false; }
	WSADATA wsd;
	int w = WSAStartup(MAKEWORD(2, 2), &wsd);
	if (w != 0) { DbgPrintF("[net] WSAStartup failed: %d\n", w); XNetCleanup(); return false; }

	// Wait (bounded) for the console to actually acquire a network address before any
	// connect(). On a cold boot XNet is still doing DHCP; connecting before the stack has
	// an address fails for no reason but "asked too early" -- the #1 cause of flaky connects
	// on real hardware. Reference: HttpProgram/Matchmaking.cpp polls XNetGetTitleXnAddr past
	// XNET_GET_XNADDR_PENDING. We bound it (~5s) instead of looping forever so a genuinely
	// offline console fails gracefully rather than hanging boot; connect() still runs after
	// (and will fail cleanly via the connect timeout if there's truly no network).
	XNADDR xna; memset(&xna, 0, sizeof(xna));
	DWORD xst = XNET_GET_XNADDR_PENDING;
	for (int tries = 0; tries < 50; ++tries) {          // 50 * 100ms = ~5s max
		xst = XNetGetTitleXnAddr(&xna);
		if (xst != XNET_GET_XNADDR_PENDING) break;
		Sleep(100);
	}
	if (xst == XNET_GET_XNADDR_PENDING || (xst & XNET_GET_XNADDR_NONE)) {
		DbgPrintF("[net] WARNING: no network address acquired (state=0x%08X) -- trying anyway\n", xst);
	} else {
		char ips[16];
		unsigned char* ob = (unsigned char*)&xna.ina.s_addr;
		sprintf(ips, "%u.%u.%u.%u", ob[0], ob[1], ob[2], ob[3]);
		DbgPrintF("[net] local address acquired: %s (state=0x%08X)\n", ips, xst);
	}

	s_netInit = true;
	DbgPrintF("[net] network stack up (XNet bypass-security + WSA 2.2)\n");
	return true;
}

void Socket::globalShutdown() {
	if (s_netInit) { WSACleanup(); XNetCleanup(); s_netInit = false; }
}

// ============================================================================================
// Socket-backed byte streams (java.io.InputStream/OutputStream over recv/send). Blocking I/O --
// NetworkManager runs them on its own reader/writer threads, so blocking is correct here.
// ============================================================================================
namespace {
	class SocketInputStream : public InputStream {
		unsigned int _s;
	public:
		SocketInputStream(unsigned int s) : _s(s) {}
		int read() {
			unsigned char b;
			int n = recv((SOCKET)_s, (char*)&b, 1, 0);
			return (n == 1) ? (int)b : -1;                  // 0 = peer closed, SOCKET_ERROR = error -> -1
		}
		int read(unsigned char* b, int off, int len) {
			if (len <= 0) return 0;
			int n = recv((SOCKET)_s, (char*)(b + off), len, 0);
			return (n > 0) ? n : -1;
		}
		int available() { return 0; }
		void close() {}
	};
	class SocketOutputStream : public OutputStream {
		unsigned int _s;
	public:
		SocketOutputStream(unsigned int s) : _s(s) {}
		void write(int b) { unsigned char c = (unsigned char)b; send((SOCKET)_s, (const char*)&c, 1, 0); }
		void write(const unsigned char* b, int len) {
			int sent = 0;
			while (sent < len) {
				int n = send((SOCKET)_s, (const char*)(b + sent), len - sent, 0);
				if (n <= 0) break;
				sent += n;
			}
		}
		void flush() {}
		void close() {}
	};
}

// ============================================================================================
// Socket
// ============================================================================================
Socket::Socket() : _sock(0xFFFFFFFFu), _connected(false), _in(0), _out(0) {}

Socket::Socket(const JavaString& host, int port)
	: _sock(0xFFFFFFFFu), _connected(false), _in(0), _out(0) {
	_doConnect(host.c_str(), port);
}

Socket::Socket(InetAddress* addr, int port)
	: _sock(0xFFFFFFFFu), _connected(false), _in(0), _out(0) {
	// FIXED BUG socket-inetaddress-host-dropped: the InetAddress shim now CARRIES the host (runtime
	// InetAddress.h), so the standard `new Socket(InetAddress.getByName(host), port)` idiom that
	// NetClientHandler emits connects for real instead of silently staying unconnected (MP hang).
	JavaString _h = addr ? addr->getHostName() : JavaString();   // hold the temporary alive for c_str()
	const char* host = _h.c_str();
	if (host && host[0]) { _doConnect(host, port); }
	else { DbgPrintF("[net] Socket(InetAddress,port): host unavailable -> not connected\n"); }
}

Socket::~Socket() {
	close();
	delete _in;  _in = 0;
	delete _out; _out = 0;
}

void Socket::_doConnect(const char* host, int port) {
	if (!globalInit()) return;

	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET) { DbgPrintF("[net] socket() failed\n"); return; }

	// Xbox 360: disable XNet encryption so we can talk to a standard PC server.
	// 0x5802 = disable VDP encryption, 0x5801 = enable insecure (cleartext) traffic.
	// Must be set before connect(). Without these, XNet encrypts all traffic and a
	// standard PC server sees garbage.
	BOOL bInsecure = TRUE;
	setsockopt(s, SOL_SOCKET, 0x5802, (const char*)&bInsecure, sizeof(BOOL));
	setsockopt(s, SOL_SOCKET, 0x5801, (const char*)&bInsecure, sizeof(BOOL));

	// Resolve the host: dotted-quad IP directly, else XNet DNS.
	IN_ADDR ina; memset(&ina, 0, sizeof(ina));
	unsigned a = 0, b = 0, c = 0, d = 0;
	if (host && sscanf(host, "%u.%u.%u.%u", &a, &b, &c, &d) == 4) {
		ina.s_addr = htonl((a << 24) | (b << 16) | (c << 8) | d);
	} else if (host) {
		XNDNS* dns = 0;
		if (XNetDnsLookup(host, 0, &dns) == 0 && dns) {
			while (dns->iStatus == WSAEINPROGRESS) Sleep(10);   // async lookup -> poll to completion
			if (dns->iStatus == 0 && dns->cina > 0) ina = dns->aina[0];
			XNetDnsRelease(dns);
		}
	}
	if (ina.s_addr == 0) {
		DbgPrintF("[net] could not resolve host: %s\n", host ? host : "(null)");
		closesocket(s);
		return;
	}

	sockaddr_in sa; memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port   = htons((unsigned short)port);
	sa.sin_addr   = ina;

	// Bounded connect: a blocking connect() to a dead/wrong IP hangs GuiConnecting forever.
	// Go non-blocking, then select() for writability with a timeout, so a bad host fails
	// cleanly. Restore blocking afterwards -- the reader/writer threads want blocking I/O.
	u_long nb = 1; ioctlsocket(s, FIONBIO, &nb);
	int cr = ::connect(s, (const struct sockaddr*)&sa, sizeof(sa));
	if (cr != 0) {
		int werr = WSAGetLastError();
		if (werr != WSAEWOULDBLOCK) {
			DbgPrintF("[net] connect() to %s:%d failed: %d\n", host, port, werr);
			closesocket(s);
			return;
		}
		// connect in progress -> wait up to 10s for the socket to become writable (connected)
		fd_set wfds; FD_ZERO(&wfds); FD_SET(s, &wfds);
		fd_set efds; FD_ZERO(&efds); FD_SET(s, &efds);
		timeval tv; tv.tv_sec = 10; tv.tv_usec = 0;
		// Winsock signals a failed non-blocking connect via the except set and a successful
		// one via the write set (SO_ERROR isn't available in the 360 winsockx.h). So: writable
		// AND not in the except set == connected; anything else == timed out / refused.
		int sel = select(0, NULL, &wfds, &efds, &tv);
		if (sel <= 0 || FD_ISSET(s, &efds) || !FD_ISSET(s, &wfds)) {
			DbgPrintF("[net] connect() to %s:%d timed out/refused\n", host, port);
			closesocket(s);
			return;
		}
	}
	u_long bl = 0; ioctlsocket(s, FIONBIO, &bl);   // back to blocking for the net threads

	int one = 1;
	setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (const char*)&one, sizeof(one));   // low latency for packets

	_sock = (unsigned int)s;
	_connected = true;
	_in  = new SocketInputStream(_sock);
	_out = new SocketOutputStream(_sock);
	DbgPrintF("[net] connected to %s:%d\n", host, port);
}

void Socket::close(...) {
	if (_sock != 0xFFFFFFFFu) { closesocket((SOCKET)_sock); _sock = 0xFFFFFFFFu; }
	_connected = false;
}

InputStream*  Socket::getInputStream(...)  { return _in; }
OutputStream* Socket::getOutputStream(...) { return _out; }
