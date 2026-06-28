#include "stdafx.h"
#include <winsockx.h>          // 360 Winsock + XNet (socket/connect/send/recv, XNetStartup, XNetDnsLookup)
#include "Socket.h"
#include "FileIO.h"            // InputStream / OutputStream base classes

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

Socket::Socket(void*, int)
	: _sock(0xFFFFFFFFu), _connected(false), _in(0), _out(0) {
	// The InetAddress shim returned NULL (host dropped). Callers should use Socket(JavaString,port).
	DbgPrintF("[net] Socket(InetAddress,port): host unavailable from shim -> not connected\n");
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
	if (::connect(s, (const struct sockaddr*)&sa, sizeof(sa)) != 0) {
		DbgPrintF("[net] connect() to %s:%d failed: %d\n", host, port, WSAGetLastError());
		closesocket(s);
		return;
	}

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
