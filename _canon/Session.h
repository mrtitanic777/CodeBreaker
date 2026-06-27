#ifndef SESSION_H
#define SESSION_H

// Auto-generated from Session.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class Session {
public:
	Session(const JavaString& var1, const JavaString& var2);
	virtual ~Session();

	JavaString inventory;
	JavaString sessionId;
	JavaString mpPassword;
	static JList<void*> s_registeredBlocksList;

	static void initStatics();
};

#endif // SESSION_H