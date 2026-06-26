#ifndef ICOMMANDLISTENER_H
#define ICOMMANDLISTENER_H

// Auto-generated from ICommandListener.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class ICommandListener {
public:
	virtual ~ICommandListener() {}
	virtual void log(const std::string& var1) = 0;
	virtual const std::string& getUsername() = 0;
};

#endif // ICOMMANDLISTENER_H