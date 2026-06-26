#ifndef CHATLINE_H
#define CHATLINE_H

// Auto-generated from ChatLine.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class ChatLine {
public:
	ChatLine();
	ChatLine(const std::string& var1);
	virtual ~ChatLine();

	std::string m_message;
	int m_updateCounter;


};

#endif // CHATLINE_H