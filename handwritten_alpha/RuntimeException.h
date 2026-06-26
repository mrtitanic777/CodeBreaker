#ifndef RUNTIMEEXCEPTION_H
#define RUNTIMEEXCEPTION_H
// PORT stub: java.lang.RuntimeException. Xbox 360 build uses /EH OFF (no C++
// exceptions), so this is just a carrier base for MinecraftException et al.
// Holds the message; throws are converted to `if(false)` dead blocks elsewhere.
#include <string>

class RuntimeException {
public:
	RuntimeException() {}
	RuntimeException(const std::string& message) : m_message(message) {}
	virtual ~RuntimeException() {}
	const std::string& getMessage() const { return m_message; }
	std::string m_message;
};

#endif // RUNTIMEEXCEPTION_H
