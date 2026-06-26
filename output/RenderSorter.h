#ifndef RENDERSORTER_H
#define RENDERSORTER_H

// Auto-generated from RenderSorter.java
// Target: Xbox 360 / C++03 / DirectX 9

class EntityPlayer;

class RenderSorter {
public:
	RenderSorter();
	RenderSorter(EntityPlayer* var1);
	virtual ~RenderSorter();

	virtual int compare(void* /* PORT: was Object */ var1, void* /* PORT: was Object */ var2);

private:
	EntityPlayer* m_baseEntity;

};

#endif // RENDERSORTER_H