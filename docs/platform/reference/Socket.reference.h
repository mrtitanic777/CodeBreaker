#ifndef SOCKET_H
#define SOCKET_H
// =============================== REAL Xbox 360 TCP socket =====================================
// Replaces the old java.net.Socket symbol-stub. Backs the converted Alpha SMP netcode
// (NetworkManager + Packet*) with a real Winsock/XNet TCP connection so the 360 can join an
// Alpha-protocol-6 Java server. globalInit() calls XNetStartup with XNET_STARTUP_BYPASS_SECURITY
// so the console will talk to a non-Xbox (PC) host -- without it the 360 refuses insecure peers.
// HARDWARE-VERIFY: the Nexia360 emulator may or may not route XNet TCP to the host network; if it
// doesn't, this only connects on real RGH hardware. The platform layer owns the real behaviour.
// =============================================================================================
#include "Common.h"
#include "JavaString.h"
class InputStream;
class OutputStream;

class Socket {
	unsigned int  _sock;        // SOCKET handle; 0xFFFFFFFF = invalid/not connected
	bool          _connected;
	InputStream*  _in;
	OutputStream* _out;
	void _doConnect(const char* host, int port);
public:
	static bool s_netInit;
	static bool globalInit();      // XNetStartup(BYPASS_SECURITY) + WSAStartup; idempotent; true on success
	static void globalShutdown();

	Socket();
	Socket(const JavaString& host, int port);   // java.net.Socket(host,port): resolve + connect now
	Socket(void* /*InetAddress*/, int /*port*/);// legacy shim form (host was dropped) -> stays unconnected
	virtual ~Socket();

	virtual void close(...);
	virtual void setSoTimeout(...) {}
	virtual void setTrafficClass(...) {}
	virtual void setTcpNoDelay(...) {}          // NODELAY is set unconditionally in _doConnect (low latency)
	virtual void setKeepAlive(...) {}
	virtual void connect(...) {}
	virtual InputStream*  getInputStream(...);
	virtual OutputStream* getOutputStream(...);
	virtual void* getRemoteSocketAddress(...) { return _connected ? (void*)this : (void*)0; }
	virtual void* getInetAddress(...) { return (void*)this; }
	virtual int   getPort(...) { return 0; }
	virtual bool  isConnected(...) { return _connected; }
	virtual bool  isClosed(...) { return !_connected; }
};
#endif // SOCKET_H
