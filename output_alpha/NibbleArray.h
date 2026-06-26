#ifndef NIBBLEARRAY_H
#define NIBBLEARRAY_H

// Auto-generated from NibbleArray.java
// Target: Xbox 360 / C++03 / DirectX 9

class NibbleArray {
public:
	NibbleArray();
	NibbleArray(int var1);
	NibbleArray(unsigned char* var1);
	virtual ~NibbleArray();

	unsigned char* m_data;

	virtual int getNibble(int var1, int var2, int var3);
	virtual void setNibble(int var1, int var2, int var3, int var4);
	virtual bool isValid();

public:  // array-length companions
	int m_data_count;

};

#endif // NIBBLEARRAY_H