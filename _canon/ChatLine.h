#ifndef CHATLINE_H
#define CHATLINE_H

// Auto-generated from ChatLine.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class ChatLine {
public:
	ChatLine(const JavaString& var1);
	virtual ~ChatLine();

	JavaString message;
	int updateCounter;

	static void initStatics();
};

#endif // CHATLINE_H