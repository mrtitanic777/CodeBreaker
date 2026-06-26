#ifndef UNEXPECTEDTHROWABLE_H
#define UNEXPECTEDTHROWABLE_H

// Auto-generated from UnexpectedThrowable.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class Throwable;

class UnexpectedThrowable {
public:
	UnexpectedThrowable();
	UnexpectedThrowable(const std::string& var1, Throwable* var2);
	virtual ~UnexpectedThrowable();

	std::string m_description;
	Throwable* m_exception;


};

#endif // UNEXPECTEDTHROWABLE_H