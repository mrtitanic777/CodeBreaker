#ifndef THROWABLE_H
#define THROWABLE_H
// PORT stub: java.lang.Throwable. 360 build is /EH OFF; this is just a carrier
// base so exception classes (MinecraftError extends Throwable) compile.
#include <string>

class Throwable {
public:
	Throwable() {}
	Throwable(const std::string& message) : m_message(message) {}
	virtual ~Throwable() {}
	const std::string& getMessage() const { return m_message; }
	std::string m_message;
};

#endif // THROWABLE_H
