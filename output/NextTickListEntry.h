#ifndef NEXTTICKLISTENTRY_H
#define NEXTTICKLISTENTRY_H

// Auto-generated from NextTickListEntry.java
// Target: Xbox 360 / C++03 / DirectX 9

class NextTickListEntry {
public:
	NextTickListEntry();
	NextTickListEntry(int var1, int var2, int var3, int var4);
	virtual ~NextTickListEntry();

	int m_xCoord;
	int m_yCoord;
	int m_zCoord;
	int blockID;
	int m_scheduledTime;


};

#endif // NEXTTICKLISTENTRY_H