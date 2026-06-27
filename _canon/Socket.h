#ifndef SOCKET_H
#define SOCKET_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for java.net.Socket.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>
class InputStream;
class OutputStream;

class Socket {
public:
	Socket(...) {}
	virtual ~Socket() {}
	virtual void close(...) {}
	virtual void setSoTimeout(...) {}
	virtual void setTrafficClass(...) {}
	virtual void setTcpNoDelay(...) {}
	virtual void setKeepAlive(...) {}
	virtual void connect(...) {}
	virtual InputStream* getInputStream(...) { return 0; }
	virtual OutputStream* getOutputStream(...) { return 0; }
	virtual void* getRemoteSocketAddress(...) { return 0; }
	virtual void* getInetAddress(...) { return 0; }
	virtual int getPort(...) { return 0; }
	virtual bool isConnected(...) { return false; }
	virtual bool isClosed(...) { return false; }
};
#endif // SOCKET_H
